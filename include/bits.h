#ifndef BITS_H
#define BITS_H

typedef struct LeitorBits LeitorBits;
typedef struct EscritorBits EscritorBits;

int bits_iniciar_leitura(
    LeitorBits **leitor,
    const char *arquivo
);

int bits_ler(
    LeitorBits *leitor,
    unsigned int *bit
);

int bits_iniciar_escrita(
    EscritorBits **escritor,
    const char *arquivo
);

int bits_escrever(
    EscritorBits *escritor,
    unsigned int bit
);

int bits_finalizar_escrita(
    EscritorBits *escritor
);

void bits_liberar_leitura(LeitorBits *leitor);
void bits_liberar_escrita(EscritorBits *escritor);

#endif