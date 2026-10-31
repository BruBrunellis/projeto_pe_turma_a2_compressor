#include <stdio.h>
#include <time.h>

#include "main.h"
#include "compressor.h"

static int ler_caminho(char *destino, int capacidade)
{
    int tamanho;
    int byte;

    if (capacidade < 2 || fgets(destino, capacidade, stdin) == NULL) {
        return 0;
    }

    tamanho = 0;
    while (destino[tamanho] != '\0' && destino[tamanho] != '\n') {
        tamanho++;
    }

    if (destino[tamanho] == '\n') {
        destino[tamanho] = '\0';
    } else if (!feof(stdin)) {
        /* Uma linha no limite ainda pode ter apenas a quebra pendente. */
        byte = fgetc(stdin);
        if (byte == '\r') {
            byte = fgetc(stdin);
        }
        if (byte != '\n' && byte != EOF) {
            do {
                byte = fgetc(stdin);
            } while (byte != '\n' && byte != EOF);
            return 0;
        }
    }

    if (ferror(stdin)) {
        return 0;
    }
    if (tamanho > 0 && destino[tamanho - 1] == '\r') {
        destino[--tamanho] = '\0';
    }

    return tamanho > 0;
}

static int ler_opcao(void)
{
    char linha[16];
    int posicao;
    int opcao;

    printf("\n1) Comprimir\n2) Descomprimir\n3) Sair\nOpcao: ");
    fflush(stdout);

    if (!ler_caminho(linha, sizeof(linha))) {
        if (feof(stdin) || ferror(stdin)) {
            return -1;
        }
        return 0;
    }

    posicao = 0;
    while (linha[posicao] == ' ' || linha[posicao] == '\t') {
        posicao++;
    }
    if (linha[posicao] < '1' || linha[posicao] > '3') {
        return 0;
    }

    opcao = linha[posicao++] - '0';
    while (linha[posicao] == ' ' || linha[posicao] == '\t') {
        posicao++;
    }

    if (linha[posicao] != '\0') {
        return 0;
    }
    return opcao;
}

static int ajustar_extensao(char *caminho, int capacidade)
{
    const char *extensao;
    int tamanho;
    int tamanho_extensao;
    int i;
    int byte;

    extensao = EXTENSAO_COMPRIMIDO;
    tamanho = 0;
    while (tamanho < capacidade && caminho[tamanho] != '\0') {
        tamanho++;
    }
    if (tamanho == capacidade) {
        return 0;
    }

    tamanho_extensao = 0;
    while (extensao[tamanho_extensao] != '\0') {
        tamanho_extensao++;
    }

    if (tamanho >= tamanho_extensao) {
        for (i = 0; i < tamanho_extensao; i++) {
            byte = caminho[tamanho - tamanho_extensao + i];
            if (byte >= 'A' && byte <= 'Z') {
                byte += 'a' - 'A';
            }
            if (byte != extensao[i]) {
                break;
            }
        }
        if (i == tamanho_extensao) {
            return 1;
        }
    }

    if (tamanho_extensao >= capacidade - tamanho) {
        return 0;
    }
    for (i = 0; i <= tamanho_extensao; i++) {
        caminho[tamanho + i] = extensao[i];
    }

    return 1;
}

static void exibir_estatisticas(
    const Estatisticas *estatisticas,
    double segundos)
{
    printf("Tamanho original: %lu bytes\n", estatisticas->tamanho_original);
    printf("Tamanho comprimido: %lu bytes\n", estatisticas->tamanho_comprimido);

    if (segundos >= 0.0) {
        printf("Tempo: %.6f s\n", segundos);
    } else {
        printf("Tempo: indisponivel\n");
    }

    if (estatisticas->tamanho_original == 0) {
        printf("Razao comprimido/original: nao aplicavel (original vazio).\n");
    } else {
        printf("Razao comprimido/original: %.6f\n",
            (double)estatisticas->tamanho_comprimido /
            (double)estatisticas->tamanho_original);
    }
}

int main(void)
{
    char entrada[TAMANHO_CAMINHO_MAX];
    char saida[TAMANHO_CAMINHO_MAX];
    Estatisticas estatisticas;
    clock_t inicio;
    clock_t fim;
    double segundos;
    int opcao;
    int resultado;

    printf("%s\n", NOME_PROGRAMA);

    for (;;) {
        opcao = ler_opcao();
        if (opcao == OPCAO_SAIR || opcao == -1) {
            break;
        }
        if (opcao == 0) {
            printf("Opcao invalida. Digite 1, 2 ou 3.\n");
            continue;
        }

        printf("Arquivo de origem (sem aspas): ");
        fflush(stdout);
        if (!ler_caminho(entrada, sizeof(entrada))) {
            if (feof(stdin) || ferror(stdin)) {
                break;
            }
            printf("Caminho vazio ou muito longo. Operacao nao iniciada.\n");
            continue;
        }

        if (opcao == OPCAO_COMPRIMIR) {
            printf("Arquivo de destino (.bift sera acrescentada se necessario): ");
        } else {
            printf("Arquivo restaurado de destino: ");
        }
        fflush(stdout);
        if (!ler_caminho(saida, sizeof(saida))) {
            if (feof(stdin) || ferror(stdin)) {
                break;
            }
            printf("Caminho vazio ou muito longo. Operacao nao iniciada.\n");
            continue;
        }

        if (opcao == OPCAO_COMPRIMIR &&
            !ajustar_extensao(saida, sizeof(saida))) {
            printf("Destino muito longo para acrescentar .bift.\n");
            continue;
        }

        inicio = clock();
        if (opcao == OPCAO_COMPRIMIR) {
            resultado = comprimir_arquivo(entrada, saida, &estatisticas);
        } else {
            resultado = descomprimir_arquivo(entrada, saida, &estatisticas);
        }
        fim = clock();

        segundos = -1.0;
        if (inicio != (clock_t)-1 && fim != (clock_t)-1 && fim >= inicio) {
            segundos = ((double)fim - (double)inicio) / CLOCKS_PER_SEC;
        }

        switch (resultado) {
            case COMPRESSOR_OK:
                printf("Operacao concluida. Arquivo gerado: %s\n", saida);
                exibir_estatisticas(&estatisticas, segundos);
                break;
            case COMPRESSOR_ERRO_ABRIR:
                printf("Falha ao abrir arquivos. Confira a origem, as permissoes\n"
                       "e se o destino ja existe.\n");
                break;
            case COMPRESSOR_ERRO_LEITURA:
                printf("Falha ao ler ou reposicionar a entrada.\n");
                break;
            case COMPRESSOR_ERRO_ESCRITA:
                printf("Falha ao gravar, fechar ou remover a saida parcial.\n");
                break;
            case COMPRESSOR_ARQUIVO_INVALIDO:
                printf("Arquivo ou estrutura Huffman invalida.\n");
                break;
            case COMPRESSOR_TAMANHO_EXCEDIDO:
                printf("Arquivo excede os limites de tamanho suportados.\n");
                break;
            case COMPRESSOR_ERRO_MEMORIA:
                printf("Memoria insuficiente para a arvore Huffman.\n");
                break;
            default:
                printf("Erro desconhecido do compressor.\n");
                break;
        }
    }

    if (ferror(stdin)) {
        fprintf(stderr, "Falha na leitura da entrada.\n");
        return 1;
    }
    return 0;
}
