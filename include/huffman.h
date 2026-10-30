
#ifndef HUFFMAN_H
#define HUFFMAN_H

#define QUANTIDADE_SIMBOLOS 256
#define TAMANHO_CODIGO_MAX 255
#define TAMANHO_VETOR_CODIGO 32

#define TAMANHO_ARVORE_MAX 767

typedef struct NoHuffman NoHuffman;

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

int huffman_desserializar(
    const unsigned char *buffer,
    unsigned int tamanho,
    NoHuffman **arvore
);

void huffman_liberar(NoHuffman *arvore);

#endif
