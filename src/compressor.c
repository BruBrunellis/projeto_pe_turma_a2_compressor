#include "compressor.h"
#include "huffman.h"
#include "bits.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>

static int contar_frequencias(
    FILE *entrada,
    unsigned long frequencias[QUANTIDADE_SIMBOLOS],
    unsigned long *tamanho)
{
    unsigned int i;
    int byte;

    *tamanho = 0;

    for (i = 0; i < QUANTIDADE_SIMBOLOS; i++) {
        frequencias[i] = 0;
    }

    while ((byte = fgetc(entrada)) != EOF) {
        if (*tamanho == COMPRESSOR_TAMANHO_ORIGINAL_MAX) {
            return COMPRESSOR_TAMANHO_EXCEDIDO;
        }

        frequencias[byte]++;
        (*tamanho)++;
    }

    if (ferror(entrada)) {
        return COMPRESSOR_ERRO_LEITURA;
    }

    return COMPRESSOR_OK;
}

static int escrever_cabecalho(
    FILE *saida,
    unsigned long tamanho_original,
    const unsigned char *arvore,
    unsigned int tamanho_arvore)
{
    unsigned int i;

    if (fwrite(COMPRESSOR_ASSINATURA, 1,
            COMPRESSOR_TAMANHO_ASSINATURA, saida) !=
            COMPRESSOR_TAMANHO_ASSINATURA ||
        fputc(COMPRESSOR_VERSAO, saida) == EOF) {
        return COMPRESSOR_ERRO_ESCRITA;
    }

    /* Grava os inteiros em big-endian, sem depender de sizeof. */
    for (i = 4; i > 0; i--) {
        if (fputc((int)((tamanho_original >> ((i - 1) * 8)) &
                255UL), saida) == EOF) {
            return COMPRESSOR_ERRO_ESCRITA;
        }
    }

    for (i = 2; i > 0; i--) {
        if (fputc((int)((tamanho_arvore >> ((i - 1) * 8)) &
                255u), saida) == EOF) {
            return COMPRESSOR_ERRO_ESCRITA;
        }
    }

    if (fwrite(arvore, 1, tamanho_arvore, saida) != tamanho_arvore) {
        return COMPRESSOR_ERRO_ESCRITA;
    }

    return COMPRESSOR_OK;
}

static int codificar_fluxo(
    FILE *entrada,
    FILE *saida,
    const CodigoHuffman tabela[QUANTIDADE_SIMBOLOS],
    unsigned long tamanho_esperado,
    unsigned long *tamanho_comprimido)
{
    EscritorBits escritor;
    const CodigoHuffman *codigo;
    unsigned long processados;
    unsigned int i;
    unsigned int bit;
    int byte;

    bits_iniciar_escrita(&escritor, saida);
    processados = 0;

    while ((byte = fgetc(entrada)) != EOF) {
        if (processados >= tamanho_esperado) {
            return COMPRESSOR_ERRO_LEITURA;
        }

        codigo = &tabela[byte];

        if (codigo->tamanho == 0 ||
            codigo->tamanho > TAMANHO_CODIGO_MAX) {
            return COMPRESSOR_ERRO_LEITURA;
        }

        for (i = 0; i < codigo->tamanho; i++) {
            if (escritor.usados == 7 &&
                *tamanho_comprimido == ULONG_MAX) {
                return COMPRESSOR_TAMANHO_EXCEDIDO;
            }

            bit = (codigo->bits[i / 8] >> (7 - (i % 8))) & 1u;

            if (bits_escrever(&escritor, bit) != 0) {
                return COMPRESSOR_ERRO_ESCRITA;
            }

            if (escritor.usados == 0) {
                (*tamanho_comprimido)++;
            }
        }

        processados++;
    }

    if (ferror(entrada) || processados != tamanho_esperado) {
        return COMPRESSOR_ERRO_LEITURA;
    }

    if (escritor.usados > 0) {
        if (*tamanho_comprimido == ULONG_MAX) {
            return COMPRESSOR_TAMANHO_EXCEDIDO;
        }

        (*tamanho_comprimido)++;
    }

    if (bits_finalizar_escrita(&escritor) != 0) {
        return COMPRESSOR_ERRO_ESCRITA;
    }

    return COMPRESSOR_OK;
}

/* Falha de acesso ao destino nao significa que ele nao existe. */
static FILE *abrir_saida(const char *caminho)
{
    FILE *arquivo;

    errno = 0;
    arquivo = fopen(caminho, "rb");

    if (arquivo != NULL) {
        fclose(arquivo);
        return NULL;
    }

    if (errno != ENOENT) {
        return NULL;
    }

    return fopen(caminho, "wb");
}

int comprimir_arquivo(
    const char *entrada,
    const char *saida,
    Estatisticas *estatisticas)
{
    FILE *arquivo_entrada;
    FILE *arquivo_saida;
    NoHuffman *arvore;
    CodigoHuffman tabela[QUANTIDADE_SIMBOLOS];
    unsigned long frequencias[QUANTIDADE_SIMBOLOS];
    unsigned char arvore_serializada[TAMANHO_ARVORE_MAX];
    unsigned long tamanho_original;
    unsigned long tamanho_comprimido;
    unsigned int tamanho_arvore;
    int resultado;
    int saida_criada;

    if (entrada == NULL || saida == NULL || estatisticas == NULL ||
        entrada[0] == '\0' || saida[0] == '\0') {
        return COMPRESSOR_ERRO_ABRIR;
    }

    arquivo_entrada = fopen(entrada, "rb");

    if (arquivo_entrada == NULL) {
        return COMPRESSOR_ERRO_ABRIR;
    }

    arquivo_saida = NULL;
    arvore = NULL;
    saida_criada = 0;
    tamanho_arvore = 0;
    tamanho_comprimido = 0;

    resultado = contar_frequencias(
        arquivo_entrada, frequencias, &tamanho_original);

    if (resultado != COMPRESSOR_OK) {
        goto finalizar;
    }

    if (tamanho_original > 0) {
        arvore = huffman_construir(frequencias);

        if (arvore == NULL) {
            resultado = COMPRESSOR_ERRO_MEMORIA;
            goto finalizar;
        }

        if (huffman_gerar_codigos(arvore, tabela) != 0 ||
            huffman_serializar(arvore, arvore_serializada,
                TAMANHO_ARVORE_MAX, &tamanho_arvore) != 0) {
            resultado = COMPRESSOR_ARQUIVO_INVALIDO;
            goto finalizar;
        }
    }

    if (fseek(arquivo_entrada, 0L, SEEK_SET) != 0) {
        resultado = COMPRESSOR_ERRO_LEITURA;
        goto finalizar;
    }

    arquivo_saida = abrir_saida(saida);

    if (arquivo_saida == NULL) {
        resultado = COMPRESSOR_ERRO_ABRIR;
        goto finalizar;
    }

    saida_criada = 1;
    resultado = escrever_cabecalho(arquivo_saida, tamanho_original,
        arvore_serializada, tamanho_arvore);

    if (resultado == COMPRESSOR_OK) {
        tamanho_comprimido =
            COMPRESSOR_TAMANHO_ASSINATURA + 1UL + 4UL + 2UL +
            tamanho_arvore;
        resultado = codificar_fluxo(arquivo_entrada, arquivo_saida,
            tabela, tamanho_original, &tamanho_comprimido);
    }

finalizar:
    if (arquivo_saida != NULL) {
        if (resultado == COMPRESSOR_OK && fflush(arquivo_saida) != 0) {
            resultado = COMPRESSOR_ERRO_ESCRITA;
        }

        if (fclose(arquivo_saida) != 0 && resultado == COMPRESSOR_OK) {
            resultado = COMPRESSOR_ERRO_ESCRITA;
        }
    }

    if (fclose(arquivo_entrada) != 0 && resultado == COMPRESSOR_OK) {
        resultado = COMPRESSOR_ERRO_LEITURA;
    }

    huffman_liberar(arvore);

    if (resultado != COMPRESSOR_OK) {
        if (saida_criada && remove(saida) != 0) {
            return COMPRESSOR_ERRO_ESCRITA;
        }

        return resultado;
    }

    estatisticas->tamanho_original = tamanho_original;
    estatisticas->tamanho_comprimido = tamanho_comprimido;

    return COMPRESSOR_OK;
}

/* Implementacao reservada para a issue #6. */
int descomprimir_arquivo(
    const char *entrada,
    const char *saida,
    Estatisticas *estatisticas)
{
    (void)entrada;
    (void)saida;
    (void)estatisticas;

    return COMPRESSOR_ERRO_LEITURA;
}
