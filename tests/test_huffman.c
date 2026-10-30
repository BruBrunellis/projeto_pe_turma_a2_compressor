
#include <stdio.h>
#include <string.h>

#include "huffman.h"

/* Testa a construcao de uma arvore com apenas um simbolo. */
static int testar_um_simbolo(void)
{
    unsigned long frequencias[QUANTIDADE_SIMBOLOS] = {0};
    CodigoHuffman tabela[QUANTIDADE_SIMBOLOS];
    NoHuffman *arvore;

    frequencias['A'] = 10;

    arvore = huffman_construir(frequencias);

    if (arvore == NULL) {
        printf("ERRO: nao foi possivel construir a arvore.\n");
        return -1;
    }

    if (huffman_gerar_codigos(arvore, tabela) != 0) {
        printf("ERRO: nao foi possivel gerar os codigos.\n");
        huffman_liberar(arvore);
        return -1;
    }

    if (tabela['A'].tamanho != 1) {
        printf("ERRO: codigo do simbolo A deveria ter tamanho 1.\n");
        huffman_liberar(arvore);
        return -1;
    }

    huffman_liberar(arvore);

    printf("OK: teste de um simbolo.\n");
    return 0;
}

/* Testa a construcao de uma arvore com dois simbolos. */
static int testar_dois_simbolos(void)
{
    unsigned long frequencias[QUANTIDADE_SIMBOLOS] = {0};
    CodigoHuffman tabela[QUANTIDADE_SIMBOLOS];
    NoHuffman *arvore;

    frequencias['A'] = 5;
    frequencias['B'] = 3;

    arvore = huffman_construir(frequencias);

    if (arvore == NULL) {
        printf("ERRO: nao foi possivel construir a arvore.\n");
        return -1;
    }

    if (huffman_gerar_codigos(arvore, tabela) != 0) {
        printf("ERRO: nao foi possivel gerar os codigos.\n");
        huffman_liberar(arvore);
        return -1;
    }

    if (tabela['A'].tamanho != 1 ||
        tabela['B'].tamanho != 1) {
        printf("ERRO: os codigos deveriam ter tamanho 1.\n");
        huffman_liberar(arvore);
        return -1;
    }

    huffman_liberar(arvore);

    printf("OK: teste de dois simbolos.\n");
    return 0;
}


static int testar_serializacao(void)
{
    unsigned long frequencias[QUANTIDADE_SIMBOLOS] = {0};
    unsigned char buffer[TAMANHO_ARVORE_MAX];
    unsigned int tamanho;
    NoHuffman *arvore;
    NoHuffman *arvore_reconstruida;

    frequencias['A'] = 5;
    frequencias['B'] = 3;

    arvore = huffman_construir(frequencias);

    if (arvore == NULL) {
        printf("ERRO: falha ao construir arvore.\n");
        return -1;
    }

    if (huffman_serializar(
            arvore,
            buffer,
            TAMANHO_ARVORE_MAX,
            &tamanho) != 0) {
        printf("ERRO: falha na serializacao.\n");
        huffman_liberar(arvore);
        return -1;
    }

    if (huffman_desserializar(
            buffer,
            tamanho,
            &arvore_reconstruida) != 0) {
        printf("ERRO: falha na desserializacao.\n");
        huffman_liberar(arvore);
        return -1;
    }

    if (arvore_reconstruida == NULL) {
        printf("ERRO: arvore reconstruida esta vazia.\n");
        huffman_liberar(arvore);
        return -1;
    }

    huffman_liberar(arvore);
    huffman_liberar(arvore_reconstruida);

    printf("OK: teste de serializacao e desserializacao.\n");
    return 0;
}


static int testar_arvore_vazia(void)
{
    unsigned long frequencias[QUANTIDADE_SIMBOLOS] = {0};
    unsigned char buffer[TAMANHO_ARVORE_MAX];
    unsigned int tamanho = 99;
    NoHuffman *arvore;
    NoHuffman *arvore_reconstruida;

    arvore = huffman_construir(frequencias);

    if (arvore != NULL) {
        printf("ERRO: arvore deveria estar vazia.\n");
        huffman_liberar(arvore);
        return -1;
    }

    if (huffman_serializar(NULL, buffer,
                           TAMANHO_ARVORE_MAX, &tamanho) != 0) {
        printf("ERRO: falha ao serializar arvore vazia.\n");
        return -1;
    }

    if (tamanho != 0) {
        printf("ERRO: tamanho da arvore vazia deveria ser zero.\n");
        return -1;
    }

    arvore_reconstruida = NULL;

    if (huffman_desserializar(NULL, 0, &arvore_reconstruida) != 0) {
        printf("ERRO: falha ao desserializar arvore vazia.\n");
        return -1;
    }

    if (arvore_reconstruida != NULL) {
        printf("ERRO: arvore reconstruida deveria estar vazia.\n");
        huffman_liberar(arvore_reconstruida);
        return -1;
    }

    printf("OK: teste de arvore vazia.\n");
    return 0;
}


static int testar_256_simbolos(void)
{
    unsigned long frequencias[QUANTIDADE_SIMBOLOS];
    CodigoHuffman tabela[QUANTIDADE_SIMBOLOS];
    unsigned char buffer[TAMANHO_ARVORE_MAX];
    unsigned int tamanho;
    int i;
    NoHuffman *arvore;

    for (i = 0; i < QUANTIDADE_SIMBOLOS; i++) {
        frequencias[i] = 1;
    }

    arvore = huffman_construir(frequencias);

    if (arvore == NULL) {
        printf("ERRO: falha ao construir arvore com 256 simbolos.\n");
        return -1;
    }

    if (huffman_gerar_codigos(arvore, tabela) != 0) {
        printf("ERRO: falha ao gerar codigos.\n");
        huffman_liberar(arvore);
        return -1;
    }

    if (huffman_serializar(arvore, buffer,
                           TAMANHO_ARVORE_MAX, &tamanho) != 0) {
        printf("ERRO: falha ao serializar arvore.\n");
        huffman_liberar(arvore);
        return -1;
    }

    if (tamanho != TAMANHO_ARVORE_MAX) {
        printf("ERRO: tamanho inesperado da arvore serializada.\n");
        huffman_liberar(arvore);
        return -1;
    }

    huffman_liberar(arvore);

    printf("OK: teste com 256 simbolos.\n");
    return 0;
}


static int testar_buffer_insuficiente(void)
{
    unsigned long frequencias[QUANTIDADE_SIMBOLOS] = {0};
    unsigned char buffer[1];
    unsigned int tamanho;
    NoHuffman *arvore;
    int resultado;

    frequencias['A'] = 5;
    frequencias['B'] = 3;

    arvore = huffman_construir(frequencias);

    if (arvore == NULL) {
        printf("ERRO: falha ao construir arvore.\n");
        return -1;
    }

    tamanho = 99;

    resultado = huffman_serializar(
        arvore,
        buffer,
        sizeof(buffer),
        &tamanho
    );

    huffman_liberar(arvore);

    if (resultado != -1) {
        printf("ERRO: deveria rejeitar buffer insuficiente.\n");
        return -1;
    }

    if (tamanho != 0) {
        printf("ERRO: tamanho deveria permanecer zero.\n");
        return -1;
    }

    printf("OK: teste de buffer insuficiente.\n");
    return 0;
}


static int testar_marcador_invalido(void)
{
    unsigned char buffer[1];
    NoHuffman *arvore = NULL;

    buffer[0] = 2;

    if (huffman_desserializar(
            buffer,
            sizeof(buffer),
            &arvore) != -1) {
        printf("ERRO: marcador invalido deveria ser rejeitado.\n");
        huffman_liberar(arvore);
        return -1;
    }

    if (arvore != NULL) {
        printf("ERRO: arvore deveria permanecer NULL.\n");
        huffman_liberar(arvore);
        return -1;
    }

    printf("OK: teste de marcador invalido.\n");
    return 0;
}

int main(void)
{
    int resultado = 0;

    if (testar_um_simbolo() != 0) {
        resultado = 1;
    }

    if (testar_dois_simbolos() != 0) {
        resultado = 1;

    }
    if (testar_serializacao() != 0) {
    resultado = 1;
    }
    if (testar_arvore_vazia() != 0) {
    resultado = 1;
    }

    if (testar_256_simbolos() != 0) {
    resultado = 1;
    }

    if (testar_buffer_insuficiente() != 0) {
    resultado = 1;
    }

    if (testar_marcador_invalido() != 0) {
    resultado = 1;
    }
    if (resultado == 0) {
        printf("\nTodos os testes passaram.\n");
    } else {
        printf("\nUm ou mais testes falharam.\n");
    }

    return resultado;
}