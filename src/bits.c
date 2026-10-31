#include "bits.h"

/* Prepara o escritor com o buffer vazio. */
void bits_iniciar_escrita(EscritorBits *escritor, FILE *arquivo)
{
    escritor->arquivo = arquivo;
    escritor->buffer = 0;
    escritor->usados = 0;
}

/* Acumula um bit (do mais para o menos significativo) e grava
   o byte quando ele fica completo. */
int bits_escrever(EscritorBits *escritor, unsigned int bit)
{
    if (bit > 1) {
        return -1;
    }

    if (bit == 1) {
        escritor->buffer |=
            (unsigned char)(1u << (7 - escritor->usados));
    }

    escritor->usados++;

    if (escritor->usados == 8) {
        if (fputc(escritor->buffer, escritor->arquivo) == EOF) {
            return -1;
        }

        escritor->buffer = 0;
        escritor->usados = 0;
    }

    return 0;
}

/* Grava o byte parcial, completado com zeros. */
int bits_finalizar_escrita(EscritorBits *escritor)
{
    if (escritor->usados == 0) {
        return 0;
    }

    if (fputc(escritor->buffer, escritor->arquivo) == EOF) {
        return -1;
    }

    escritor->buffer = 0;
    escritor->usados = 0;

    return 0;
}

/* Prepara o leitor sem bits disponiveis. */
void bits_iniciar_leitura(LeitorBits *leitor, FILE *arquivo)
{
    leitor->arquivo = arquivo;
    leitor->buffer = 0;
    leitor->disponiveis = 0;
}

/* Entrega um bit em *bit: retorna 1 se leu, 0 no fim do arquivo
   e -1 em erro de leitura. */
int bits_ler(LeitorBits *leitor, unsigned int *bit)
{
    int byte;

    if (leitor->disponiveis == 0) {
        byte = fgetc(leitor->arquivo);

        if (byte == EOF) {
            if (ferror(leitor->arquivo)) {
                return -1;
            }

            return 0;
        }

        leitor->buffer = (unsigned char)byte;
        leitor->disponiveis = 8;
    }

    leitor->disponiveis--;
    *bit = (leitor->buffer >> leitor->disponiveis) & 1u;

    return 1;
}
