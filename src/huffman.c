
#include "huffman.h"
#include <limits.h>
#include <stdlib.h>

/* Compara dois nos, usando frequencia e menor simbolo. */
static int comparar_nos(
    const NoHuffman *a,
    const NoHuffman *b)
{
    if (a->frequencia < b->frequencia) {
        return 1;
    }

    if (a->frequencia > b->frequencia) {
        return 0;
    }

    return a->menor_simbolo < b->menor_simbolo;
}

/* Libera recursivamente os nos de uma arvore. */
static void liberar_no(NoHuffman *no)
{
    if (no == NULL) {
        return;
    }

    liberar_no(no->esquerda);
    liberar_no(no->direita);
    free(no);
}

/* Retira da lista o no de menor frequencia. */
static NoHuffman *retirar_menor(
    NoHuffman *nos[],
    unsigned int *quantidade)
{
    unsigned int i;
    unsigned int menor;
    NoHuffman *resultado;

    menor = 0;

    for (i = 1; i < *quantidade; i++) {
        if (comparar_nos(nos[i], nos[menor])) {
            menor = i;
        }
    }

    resultado = nos[menor];

    (*quantidade)--;
    nos[menor] = nos[*quantidade];

    return resultado;
}

/* Constroi a arvore a partir das frequencias dos simbolos. */
NoHuffman *huffman_construir(
    const unsigned long frequencias[QUANTIDADE_SIMBOLOS])
{
    NoHuffman *nos[2 * QUANTIDADE_SIMBOLOS - 1];
    NoHuffman *no;
    NoHuffman *esquerda;
    NoHuffman *direita;
    NoHuffman *pai;
    unsigned int quantidade;
    unsigned int i;
    unsigned int j;
    unsigned char menor_simbolo;

    if (frequencias == NULL) {
        return NULL;
    }

    quantidade = 0;

    /* Cria uma folha para cada simbolo com frequencia positiva. */
    for (i = 0; i < QUANTIDADE_SIMBOLOS; i++) {
        if (frequencias[i] == 0) {
            continue;
        }

        no = (NoHuffman *)malloc(sizeof(NoHuffman));

        if (no == NULL) {
            for (j = 0; j < quantidade; j++) {
                liberar_no(nos[j]);
            }

            return NULL;
        }

        no->frequencia = frequencias[i];
        no->simbolo = (unsigned char)i;
        no->menor_simbolo = (unsigned char)i;
        no->esquerda = NULL;
        no->direita = NULL;

        nos[quantidade] = no;
        quantidade++;
    }

    /* Frequencias todas iguais a zero: arvore vazia. */
    if (quantidade == 0) {
        return NULL;
    }

    /* Combina os dois nos de menor frequencia. */
    while (quantidade > 1) {
        esquerda = retirar_menor(nos, &quantidade);
        direita = retirar_menor(nos, &quantidade);

        /* Verifica se a soma das frequencias causa overflow. */
        if (ULONG_MAX - esquerda->frequencia <
            direita->frequencia) {

            liberar_no(esquerda);
            liberar_no(direita);

            for (j = 0; j < quantidade; j++) {
                liberar_no(nos[j]);
            }

            return NULL;
        }

        pai = (NoHuffman *)malloc(sizeof(NoHuffman));

        if (pai == NULL) {
            liberar_no(esquerda);
            liberar_no(direita);

            for (j = 0; j < quantidade; j++) {
                liberar_no(nos[j]);
            }

            return NULL;
        }

        pai->frequencia =
            esquerda->frequencia + direita->frequencia;

        pai->simbolo = 0;

        if (esquerda->menor_simbolo < direita->menor_simbolo) {
            menor_simbolo = esquerda->menor_simbolo;
        } else {
            menor_simbolo = direita->menor_simbolo;
        }

        pai->menor_simbolo = menor_simbolo;
        pai->esquerda = esquerda;
        pai->direita = direita;

        nos[quantidade] = pai;
        quantidade++;
    }

    return nos[0];
}


/* Percorre a arvore e gera os codigos dos simbolos. */
static int percorrer_arvore(
    NoHuffman *no,
    CodigoHuffman tabela[QUANTIDADE_SIMBOLOS],
    unsigned char caminho[TAMANHO_VETOR_CODIGO],
    unsigned int profundidade)
{
    CodigoHuffman *codigo;
    unsigned int i;

    if (no == NULL) {
        return -1;
    }

    /* Verifica se o no e uma folha. */
    if (no->esquerda == NULL && no->direita == NULL) {
        codigo = &tabela[no->simbolo];

        if (profundidade == 0) {
            codigo->bits[0] = 0;
            codigo->tamanho = 1;
        } else {
            for (i = 0; i < TAMANHO_VETOR_CODIGO; i++) {
                codigo->bits[i] = caminho[i];
            }
            codigo->tamanho = profundidade;
        }

        return 0;
    }

    if (profundidade >= TAMANHO_CODIGO_MAX) {
        return -1;
    }

    /* Filho esquerdo: bit 0. */
    caminho[profundidade / 8] &=
        (unsigned char)~(1u << (7 - (profundidade % 8)));

    if (percorrer_arvore(
            no->esquerda,
            tabela,
            caminho,
            profundidade + 1) != 0) {
        return -1;
    }

    /* Filho direito: bit 1. */
    caminho[profundidade / 8] |=
        (unsigned char)(1u << (7 - (profundidade % 8)));

    if (percorrer_arvore(
            no->direita,
            tabela,
            caminho,
            profundidade + 1) != 0) {
        return -1;
    }

    return 0;
}

/* Funcao publica para gerar a tabela de codigos. */
int huffman_gerar_codigos(
    NoHuffman *arvore,
    CodigoHuffman tabela[QUANTIDADE_SIMBOLOS])
{
    unsigned char caminho[TAMANHO_VETOR_CODIGO];
    unsigned int i;
    unsigned int j;

    if (arvore == NULL || tabela == NULL) {
        return -1;
    }

    for (i = 0; i < QUANTIDADE_SIMBOLOS; i++) {
        for (j = 0; j < TAMANHO_VETOR_CODIGO; j++) {
            tabela[i].bits[j] = 0;
        }

        tabela[i].tamanho = 0;
    }

    for (j = 0; j < TAMANHO_VETOR_CODIGO; j++) {
        caminho[j] = 0;
    }

    return percorrer_arvore(
        arvore,
        tabela,
        caminho,
        0
    );
}


/* Serializa um no e seus descendentes em pre-ordem. */
static int serializar_no(
    const NoHuffman *no,
    unsigned char *buffer,
    unsigned int capacidade,
    unsigned int *posicao)
{
    if (no == NULL) {
        return -1;
    }

    if (no->esquerda == NULL && no->direita == NULL) {
        if (capacidade - *posicao < 2) {
            return -1;
        }

        buffer[(*posicao)++] = 1;
        buffer[(*posicao)++] = no->simbolo;

        return 0;
    }

    if (no->esquerda == NULL || no->direita == NULL) {
        return -1;
    }

    if (*posicao >= capacidade) {
        return -1;
    }

    buffer[(*posicao)++] = 0;

    if (serializar_no(
            no->esquerda, buffer, capacidade, posicao) != 0) {
        return -1;
    }

    if (serializar_no(
            no->direita, buffer, capacidade, posicao) != 0) {
        return -1;
    }

    return 0;
}

/* Funcao publica de serializacao da arvore. */
int huffman_serializar(
    const NoHuffman *arvore,
    unsigned char *buffer,
    unsigned int capacidade,
    unsigned int *tamanho)
{
    unsigned int posicao;

    if (tamanho == NULL) {
        return -1;
    }

    *tamanho = 0;

    if (buffer == NULL || capacidade == 0 ||
        capacidade > TAMANHO_ARVORE_MAX) {
        return -1;
    }

    /* Arvore vazia: nada a gravar, tamanho zero. */
    if (arvore == NULL) {
        return 0;
    }

    posicao = 0;

    if (serializar_no(
            arvore, buffer, capacidade, &posicao) != 0) {
        return -1;
    }

    *tamanho = posicao;

    return 0;
}


/* Contexto utilizado durante a desserializacao. */
typedef struct {
    const unsigned char *buffer;
    unsigned int tamanho;
    unsigned int posicao;
    unsigned char simbolos[QUANTIDADE_SIMBOLOS];
    unsigned int quantidade_nos;
    unsigned int quantidade_folhas;
} ContextoDesserializacao;



/* Le um no e reconstrói seus descendentes recursivamente. */
static NoHuffman *desserializar_no(
    ContextoDesserializacao *contexto,
    unsigned int profundidade)
{
    NoHuffman *no;
    unsigned char marcador;
    unsigned char simbolo;

    if (contexto == NULL ||
        contexto->posicao >= contexto->tamanho ||
        contexto->quantidade_nos >= 511 ||
        profundidade > TAMANHO_CODIGO_MAX) {
        return NULL;
    }

    marcador = contexto->buffer[contexto->posicao];
    contexto->posicao++;

    contexto->quantidade_nos++;

    no = (NoHuffman *)malloc(sizeof(NoHuffman));

    if (no == NULL) {
        return NULL;
    }

    no->frequencia = 0;
    no->simbolo = 0;
    no->menor_simbolo = 0;
    no->esquerda = NULL;
    no->direita = NULL;

    /* Marcador 1: folha. */
    if (marcador == 1) {
        if (contexto->posicao >= contexto->tamanho ||
            contexto->quantidade_folhas >= QUANTIDADE_SIMBOLOS) {
            free(no);
            return NULL;
        }

        simbolo = contexto->buffer[contexto->posicao];
        contexto->posicao++;

        if (contexto->simbolos[simbolo] != 0) {
            free(no);
            return NULL;
        }

        contexto->simbolos[simbolo] = 1;
        contexto->quantidade_folhas++;

        no->simbolo = simbolo;
        no->menor_simbolo = simbolo;

        return no;
    }

    /* Marcador 0: no interno. */
    if (marcador == 0) {
        if (profundidade >= TAMANHO_CODIGO_MAX) {
            free(no);
            return NULL;
        }

        no->esquerda = desserializar_no(
            contexto, profundidade + 1);

        if (no->esquerda == NULL) {
            free(no);
            return NULL;
        }

        no->direita = desserializar_no(
            contexto, profundidade + 1);

        if (no->direita == NULL) {
            liberar_no(no->esquerda);
            free(no);
            return NULL;
        }

        if (no->esquerda->menor_simbolo <
            no->direita->menor_simbolo) {
            no->menor_simbolo = no->esquerda->menor_simbolo;
        } else {
            no->menor_simbolo = no->direita->menor_simbolo;
        }

        return no;
    }

    /* Marcador desconhecido. */
    free(no);
    return NULL;
}


/* Funcao publica para reconstruir uma arvore serializada. */
int huffman_desserializar(
    const unsigned char *buffer,
    unsigned int tamanho,
    NoHuffman **arvore)
{
    ContextoDesserializacao contexto;
    NoHuffman *raiz;
    unsigned int i;

    if (arvore == NULL) {
        return -1;
    }

    *arvore = NULL;

    /* Arvore vazia. */
    if (tamanho == 0) {
        return 0;
    }

    if (buffer == NULL || tamanho > TAMANHO_ARVORE_MAX) {
        return -1;
    }

    contexto.buffer = buffer;
    contexto.tamanho = tamanho;
    contexto.posicao = 0;
    contexto.quantidade_nos = 0;
    contexto.quantidade_folhas = 0;

    for (i = 0; i < QUANTIDADE_SIMBOLOS; i++) {
        contexto.simbolos[i] = 0;
    }

    raiz = desserializar_no(&contexto, 0);

    if (raiz == NULL) {
        return -1;
    }

    /* Verifica se todos os bytes foram consumidos. */
    if (contexto.posicao != contexto.tamanho) {
        liberar_no(raiz);
        return -1;
    }

    /* Uma arvore valida deve ter pelo menos uma folha. */
    if (contexto.quantidade_folhas == 0) {
        liberar_no(raiz);
        return -1;
    }

    *arvore = raiz;

    return 0;
}


/* Libera toda a memoria ocupada pela arvore de Huffman. */
void huffman_liberar(NoHuffman *arvore)
{
    liberar_no(arvore);
}