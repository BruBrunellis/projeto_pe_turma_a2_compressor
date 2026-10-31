BitForest - Programação Estruturada (MCTA028-15), UFABC 2026.3, turma A2

Integrantes
Bruno Vital Brunelli (11202420920);
Eduardo Dal Molin Morales (11202422080); Giovanna da Silva Champam (11202131886);
João Paulo Sanches Lira (11202520348); Kauann Antevere Ramos (11202521805).

Ambiente de compilação
  Conferido no Windows com GCC 16.1.0 do MSYS2 UCRT64.
  Padrão C90 (-std=c90 -pedantic-errors), com -Wall -Wextra.

Compilação
  make            gera bin/bitforest.exe (cria bin/ se faltar)
  make clean      remove bin/

  Sem make, a partir da raiz do projeto (crie bin/ se faltar):
  mkdir bin
  gcc -std=c90 -pedantic-errors -Wall -Wextra -Iinclude src/*.c -o bin/bitforest.exe

Execução
  bin/bitforest.exe
  Nesta etapa, o executável apenas exibe o nome BitForest.
  A compressão está disponível pela função comprimir_arquivo.
  A descompressão (#6) e o menu completo (#7) ainda estão pendentes.
  Menu previsto: 1) Comprimir  2) Descomprimir  3) Sair.
  Arquivo comprimido usa a extensão .bift. Os arquivos originais não são alterados.

Formato do arquivo comprimido (v1) - inteiros em big-endian, bits do mais ao menos significativo
  4 bytes  assinatura "BIFT" (42 49 46 54 em hexadecimal, sem terminador nulo)
  1 byte   versão (1)
  4 bytes  tamanho original em bytes
  2 bytes  tamanho da árvore serializada em bytes (0 se o original é vazio)
  N bytes  árvore em pré-ordem: nó interno = 0; folha = 1 seguido do byte do símbolo
  ...      dados: códigos de Huffman concatenados; último byte completado com zeros
  Arquivo vazio: árvore e dados vazios. Um único símbolo: código "0", um bit por ocorrência.
  A descompressão termina pelo tamanho original, ignorando os zeros de preenchimento.

Retornos de comprimir_arquivo/descomprimir_arquivo
  COMPRESSOR_OK (0) ou um código de erro definido em include/compressor.h.

Contrato da compressão
  comprimir_arquivo recebe caminhos não vazios e um endereço válido de Estatisticas.
  A entrada é aberta em rb. São feitas duas leituras: frequências e codificação,
  com reposicionamento verificado entre elas. A entrada deve permanecer inalterada.
  A memória usada não cresce com o tamanho do arquivo: tabelas de 256 símbolos,
  árvore com até 511 nós e buffer de serialização com até 767 bytes.
  A saída é aberta em wb somente após a verificação de que o destino não existe.
  Destino existente ou falha de acesso retorna COMPRESSOR_ERRO_ABRIR.
  Essa verificação com fopen em C90 não reserva o caminho de forma atômica;
  nenhum outro processo deve criar ou alterar o destino durante a operação.
  O chamador fornece o caminho completo; a extensão .bift será aplicada pelo menu.
  O módulo fecha os arquivos e libera a árvore em sucesso ou erro. Em falha após
  criar a saída, tenta removê-la; falha na remoção retorna COMPRESSOR_ERRO_ESCRITA.
  Estatisticas só é preenchida após escrita, flush e fechamento bem-sucedidos.
  tamanho_comprimido inclui os 11 bytes fixos do cabeçalho, a árvore e os dados.
  Os demais erros distinguem leitura/reposicionamento, escrita/fechamento,
  árvore inválida, tamanho excedido e falha de alocação, conforme compressor.h.

Limites e métricas
  O tamanho original da v1 é limitado a 4294967295 bytes (2^32 - 1).
  A segunda leitura deve processar exatamente a quantidade contada na primeira.
  Também é recusado um tamanho comprimido que não caiba em unsigned long.
  Os tamanhos são contados durante o processamento, sem depender de ftell.
  Tempo e razão ficam para a interface (#7); a estrutura contém apenas tamanhos.
  Razão: tamanho_comprimido / tamanho_original; para vazio, não aplicável.
  Arquivos pequenos ou pouco redundantes podem aumentar por causa do cabeçalho.

Conferência desta etapa
  Compilação C90 integrada e scripts/check_basics.py aprovados.
  Foram conferidos vazio, símbolo único, texto com caminhos com espaços, todos os
  bytes, códigos longos e binário de 1 MiB. A restauração por um leitor independente
  foi comparada com fc.exe /b, com retorno zero nas 11 entradas conferidas.
  Destinos existentes foram preservados; falhas simuladas de escrita, flush,
  fechamento e leitura confirmaram os retornos e o descarte da saída parcial.
  A restauração pela função descomprimir_arquivo depende da implementação de #6.
