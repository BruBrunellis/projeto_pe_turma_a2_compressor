# BitForest

BitForest é um compressor sem perdas para arquivos de texto e binários. Você escolhe a origem e o destino em janelas de arquivo, vê os resultados no terminal e restaura o conteúdo original byte por byte a partir de um `.bift`.

O projeto foi desenvolvido em C na disciplina de Programação Estruturada da UFABC, turma A2, quadrimestre 2026.3.

## Como funciona

O BitForest usa **codificação de Huffman por byte**. Na primeira leitura, ele conta quantas vezes cada valor de byte aparece. Em seguida, combina repetidamente os dois nós de menor peso para construir uma árvore. Os caminhos da raiz às folhas viram códigos binários, de modo que os bytes mais frequentes tendem a ocupar menos bits.

Na segunda leitura, o programa grava a sequência de bits codificada em um arquivo `.bift`, junto com a árvore e o tamanho original. Ao descomprimir, ele reconstrói a árvore, percorre seus ramos de acordo com os bits e restaura os bytes até atingir o tamanho informado no cabeçalho. Em arquivos pequenos ou pouco redundantes, o custo de armazenar o cabeçalho e a árvore pode superar a economia nos dados.

## Experimente no Windows

Com o Git instalado, use os comandos abaixo no PowerShell para clonar o repositório.

```powershell
git clone https://github.com/BruBrunellis/projeto_pe_turma_a2_compressor.git
cd projeto_pe_turma_a2_compressor
```

Com o GCC e o GNU Make do MSYS2 UCRT64 instalados no local padrão, compile a partir dessa pasta. O [guia de compilação](Readme.txt) traz os passos de instalação e a alternativa para compilar só com o GCC.

```powershell
$env:Path = "C:\msys64\ucrt64\bin;$env:Path"
mingw32-make
```

Quando a compilação terminar com sucesso, execute o programa.

```powershell
.\bin\bitforest.exe
```

1. Para comprimir, digite **1**, selecione `dados.txt` como exemplo de origem e salve com o nome `dados.txt.bift`.
2. Para restaurar, digite **2**, selecione `dados.txt.bift` e salve como `dados-restaurados.txt`.
3. Digite **3** para sair. Se o original e o restaurado estiverem na pasta do projeto, compare os bytes com `fc.exe /b dados.txt dados-restaurados.txt` no PowerShell.

A cada operação concluída, o terminal mostra o tempo, os tamanhos e a razão entre o tamanho comprimido e o original.

## Arquitetura

O menu recebe os caminhos do explorador e aciona o compressor. O compressor usa os módulos de Huffman e de leitura e escrita de bits para processar os arquivos sem carregar todo o conteúdo em memória.

| Módulo | Responsabilidade |
| --- | --- |
| [`main.c`](src/main.c) | Exibe o menu, coordena as operações e apresenta as métricas. |
| [`explorador.c`](src/explorador.c) | Abre os diálogos de origem e destino por meio da biblioteca [tinyfiledialogs](src/tinyfiledialogs.c), incluída no repositório. |
| [`compressor.c`](src/compressor.c) | Conta frequências, lê e grava o formato BIFT e coordena a compressão e a restauração. |
| [`huffman.c`](src/huffman.c) | Constrói a árvore, gera os códigos e serializa ou reconstrói a árvore. |
| [`bits.c`](src/bits.c) | Lê e escreve bits sobre arquivos com um buffer de um byte. |

## Trabalho em equipe

O fluxo de branches, commits e revisão de pull requests está no [guia de contribuição](CONTRIBUTING.md).
