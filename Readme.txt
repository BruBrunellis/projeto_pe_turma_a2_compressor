COMPRESSOR DE HUFFMAN - Programação Estruturada (MCTA028-15), UFABC 2026.3, turma A2

Integrantes
Bruno Vital Brunelli (11202420920); 
Eduardo Dal Molin Morales (11202422080); Giovanna da Silva Champam (11202131886);
João Paulo Sanches Lira (11202520348); Kauann Antevere Ramos (11202521805).

Ambiente de compilação
  GCC, padrão C90 (-std=c90 -pedantic-errors).

Compilação
  make            gera bin/compressor.exe (cria bin/ se faltar)
  make clean      remove bin/

  Sem make:
  gcc -std=c90 -pedantic-errors -Wall -Wextra -Iinclude src/*.c -o bin/compressor.exe

Execução
  bin/compressor.exe
  Menu: 1) Comprimir  2) Descomprimir  3) Sair
  Arquivo comprimido usa a extensão .hfz. Os arquivos originais não são alterados.

Formato do arquivo comprimido (v1) - inteiros em big-endian, bits do mais ao menos significativo
  4 bytes  assinatura "HUFZ"
  1 byte   versão (1)
  4 bytes  tamanho original em bytes
  2 bytes  tamanho da árvore serializada em bytes (0 se o original é vazio)
  N bytes  árvore em pré-ordem: nó interno = 0; folha = 1 seguido do byte do símbolo
  ...      dados: códigos de Huffman concatenados; último byte completado com zeros
  Arquivo vazio: árvore e dados vazios. Um único símbolo: código "0", um bit por ocorrência.
  A descompressão termina pelo tamanho original, ignorando os zeros de preenchimento.

Retornos de comprimir_arquivo/descomprimir_arquivo
  COMPRESSOR_OK (0) ou um código de erro definido em include/compressor.h.
