
#ifndef HUFFMAN_H
#define HUFFMAN_H

#define QUANTIDADE_SIMBOLOS 256
#define TAMANHO_CODIGO_MAX 255
#define TAMANHO_VETOR_CODIGO 32

#define TAMANHO_ARVORE_MAX 767

#define HUFFMAN_ERRO_ARVORE -1
#define HUFFMAN_ERRO_MEMORIA -2

typedef struct NoHuffman NoHuffman;

/* No da arvore: folha quando nao tem filhos. */
struct NoHuffman {
    unsigned long frequencia;
    unsigned char simbolo;
    unsigned char menor_simbolo;
    struct NoHuffman *esquerda;
    struct NoHuffman *direita;
};

typedef struct {
    unsigned char bits[TAMANHO_VETOR_CODIGO];
    unsigned int tamanho;
} CodigoHuffman;

NoHuffman *huffman_construir(
    const unsigned long frequencias[QUANTIDADE_SIMBOLOS]
);

int huffman_gerar_codigos(
    NoHuffman *arvore,
    CodigoHuffman tabela[QUANTIDADE_SIMBOLOS]
);

int huffman_serializar(
    const NoHuffman *arvore,
    unsigned char *buffer,
    unsigned int capacidade,
    unsigned int *tamanho
);

/* Retorna 0, HUFFMAN_ERRO_ARVORE ou HUFFMAN_ERRO_MEMORIA.
   Em sucesso, o chamador libera *arvore; em erro, *arvore fica NULL. */
int huffman_desserializar(
    const unsigned char *buffer,
    unsigned int tamanho,
    NoHuffman **arvore
);

void huffman_liberar(NoHuffman *arvore);

#endif
