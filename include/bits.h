#ifndef BITS_H
#define BITS_H

#include <stdio.h>

/* Estado de escrita: o arquivo pertence ao chamador. */
typedef struct {
    FILE *arquivo;
    unsigned char buffer;
    unsigned int usados;
} EscritorBits;

/* Estado de leitura: o arquivo pertence ao chamador. */
typedef struct {
    FILE *arquivo;
    unsigned char buffer;
    unsigned int disponiveis;
} LeitorBits;

void bits_iniciar_escrita(EscritorBits *escritor, FILE *arquivo);

int bits_escrever(EscritorBits *escritor, unsigned int bit);

int bits_finalizar_escrita(EscritorBits *escritor);

void bits_iniciar_leitura(LeitorBits *leitor, FILE *arquivo);

int bits_ler(LeitorBits *leitor, unsigned int *bit);

#endif
