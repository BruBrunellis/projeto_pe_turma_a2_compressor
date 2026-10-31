#ifndef COMPRESSOR_H
#define COMPRESSOR_H

typedef struct {
    unsigned long tamanho_original;
    unsigned long tamanho_comprimido;
} Estatisticas;

#define COMPRESSOR_OK 0
#define COMPRESSOR_ERRO_ABRIR 1
#define COMPRESSOR_ERRO_LEITURA 2
#define COMPRESSOR_ERRO_ESCRITA 3
#define COMPRESSOR_ARQUIVO_INVALIDO 4
#define COMPRESSOR_TAMANHO_EXCEDIDO 5
#define COMPRESSOR_ERRO_MEMORIA 6

int comprimir_arquivo(
    const char *entrada,
    const char *saida,
    Estatisticas *estatisticas
);

int descomprimir_arquivo(
    const char *entrada,
    const char *saida,
    Estatisticas *estatisticas
);

#endif