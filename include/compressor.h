#ifndef COMPRESSOR_H
#define COMPRESSOR_H

/* A assinatura ocupa quatro bytes no arquivo, sem o terminador nulo. */
#define COMPRESSOR_ASSINATURA "BIFT"
#define COMPRESSOR_TAMANHO_ASSINATURA 4
#define COMPRESSOR_VERSAO 1
#define COMPRESSOR_TAMANHO_ORIGINAL_MAX 4294967295UL

/* Tamanhos em bytes: preenchidos apenas quando a operacao tem sucesso. */
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

/* Exige caminhos nao vazios e estatisticas nao nulo. Recusa destino existente,
   fecha os arquivos e libera a arvore; em erro, nao altera as estatisticas. */
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
