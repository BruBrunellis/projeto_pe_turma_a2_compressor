

#include <string.h>
#include "explorador.h"
#include "tinyfiledialogs.h"

static int copiar_caminho(char *destino, int capacidade, const char *origem)
{
    int tamanho;

    if (destino == NULL || origem == NULL || capacidade <= 0)
        return 0;

    tamanho = (int)strlen(origem);

    if (tamanho == 0 || tamanho >= capacidade)
        return 0;

    strcpy(destino, origem);
    return 1;
}

int explorador_selecionar_arquivo(char *caminho, int capacidade)
{
    const char *selecionado;

    if (caminho == NULL || capacidade <= 0)
        return 0;

    selecionado = tinyfd_openFileDialog(
        "Selecionar arquivo", "", 0, NULL, NULL, 0
    );

    return copiar_caminho(caminho, capacidade, selecionado);
}

int explorador_selecionar_destino(char *caminho, int capacidade)
{
    const char *selecionado;

    if (caminho == NULL || capacidade <= 0)
        return 0;

    selecionado = tinyfd_saveFileDialog(
        "Salvar arquivo", "", 0, NULL, NULL
    );

    return copiar_caminho(caminho, capacidade, selecionado);
}
