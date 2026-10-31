BitForest - Programação Estruturada (MCTA028-15), UFABC 2026.3, turma A2

Integrantes
Bruno Vital Brunelli (11202420920);
Eduardo Dal Molin Morales (11202422080); Giovanna da Silva Champam (11202131886);
João Paulo Sanches Lira (11202520348); Kauann Antevere Ramos (11202521805).

Ambiente de compilação
  Conferido no Windows, pelo PowerShell, com GCC 16.1.0 e GNU Make 4.4.1
  (mingw32-make) do MSYS2 UCRT64. Ambos precisam estar no PATH da sessão.
  Padrão C90 (-std=c90 -pedantic-errors), com -Wall -Wextra.

Compilação
  A partir da raiz do projeto, no PowerShell com as ferramentas acima:
  mingw32-make          gera bin/bitforest.exe (cria bin/ se faltar)
  mingw32-make clean    remove bin/
  Se o GNU Make estiver disponível com o nome make, use make e make clean.

  Pacote de código: copiar somente os dez arquivos abaixo, mantendo as pastas:
    src/main.c, src/huffman.c, src/compressor.c, src/bits.c
    include/main.h, include/huffman.h, include/compressor.h, include/bits.h
    Makefile, Readme.txt
  Não incluir bin/, executáveis, temporários nem os demais apoios do repositório.
  Relatório e slides, se usados, são entregas separadas.

  Em 31/10/2026, esse pacote foi compilado pelo Makefile em uma pasta limpa com
  espaços no caminho. bin/ foi criada automaticamente e o executável abriu o menu.
  clean, recompilação e nova chamada sem alterações também passaram, sem avisos
  do GCC. Não foram necessários arquivos das demais pastas do repositório.

  Sem make, a partir da raiz do projeto (crie bin/ se faltar):
  mkdir bin
  gcc -std=c90 -pedantic-errors -Wall -Wextra -Iinclude src/*.c -o bin/bitforest.exe

Execução
  bin/bitforest.exe
  Menu: 1) Comprimir  2) Descomprimir  3) Sair.
  Digite uma opção por linha. O menu permite novas operações após sucesso ou erro.
  EOF encerra o programa, inclusive durante a solicitação dos caminhos.

Uso dos caminhos
  Informe origem e destino sem aspas, um por linha. São aceitos caminhos absolutos,
  relativos e com espaços; caminhos relativos partem da pasta em que o programa roda.
  Ao comprimir, .bift é acrescentada se o destino ainda não tiver esse sufixo.
  Exemplo: dados.bin gera dados.bin.bift. O sufixo existente também é reconhecido
  em maiúsculas, como dados.BIFT, preservando o nome digitado.
  Ao descomprimir, o destino é exatamente o nome/caminho escolhido para o restaurado.
  Os buffers têm 512 bytes, incluindo o terminador nulo; a extensão acrescentada
  também deve caber nesse limite. Linhas vazias ou longas são recusadas antes da
  operação, e o restante da linha longa é consumido para recuperar o menu.
  Os originais são preservados e os destinos existentes são recusados.
  Falha de abertura pode indicar origem inexistente, acesso recusado ou destino
  existente; o menu apresenta uma mensagem compatível com esse retorno agrupado.

Formato do arquivo comprimido (v1) - inteiros em big-endian, bits do mais ao menos significativo
  4 bytes  assinatura "BIFT" (42 49 46 54 em hexadecimal, sem terminador nulo)
  1 byte   versão (1)
  4 bytes  tamanho original em bytes
  2 bytes  tamanho da árvore serializada em bytes (0 se o original é vazio)
  N bytes  árvore em pré-ordem: nó interno = 0; folha = 1 seguido do byte do símbolo
  ...      dados: códigos de Huffman concatenados; último byte completado com zeros
  Arquivo vazio: árvore e dados vazios. Um único símbolo: código "0", um bit por ocorrência.
  A descompressão termina pelo tamanho original e valida os zeros de preenchimento.
  Bytes adicionais após os dados compactados são recusados.

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
  O chamador fornece o caminho completo; o menu aplica a extensão .bift na compressão.
  O módulo fecha os arquivos e libera a árvore em sucesso ou erro. Em falha após
  criar a saída, tenta removê-la; falha na remoção retorna COMPRESSOR_ERRO_ESCRITA.
  Estatisticas só é preenchida após escrita, flush e fechamento bem-sucedidos.
  tamanho_comprimido inclui os 11 bytes fixos do cabeçalho, a árvore e os dados.
  Os demais erros distinguem leitura/reposicionamento, escrita/fechamento,
  árvore inválida, tamanho excedido e falha de alocação, conforme compressor.h.

Contrato da descompressão
  descomprimir_arquivo recebe o caminho comprimido, o destino escolhido e o endereço
  de Estatisticas. Usa a mesma política de arquivos, recursos e estatísticas acima.
  A entrada é lida em rb; a nova saída é escrita em wb. O conteúdo é restaurado
  progressivamente, sem carregar o arquivo inteiro na memória.
  O cabeçalho deve ter assinatura BIFT, versão 1 e todos os campos completos.
  A árvore deve caber no buffer de 767 bytes e só pode ser vazia se o original
  também for vazio. A validação estrutural é feita por huffman_desserializar.
  Essa função retorna 0 em sucesso, HUFFMAN_ERRO_ARVORE (-1) para árvore inválida
  ou HUFFMAN_ERRO_MEMORIA (-2) para falha de alocação; o chamador libera a raiz.
  Para símbolo único, cada byte restaurado consome um bit 0. Para os demais casos,
  cada bit escolhe um ramo, e uma folha produz um byte, até o tamanho original.
  Cabeçalho/árvore inválidos, fim prematuro, bit 1 para símbolo único, preenchimento
  não zero e bytes adicionais retornam COMPRESSOR_ARQUIVO_INVALIDO.
  Falhas de leitura retornam COMPRESSOR_ERRO_LEITURA; escrita, flush ou fechamento
  da saída retornam COMPRESSOR_ERRO_ESCRITA. Falha de alocação é propagada como
  COMPRESSOR_ERRO_MEMORIA, com liberação de qualquer árvore parcial.
  Os tamanhos correspondem ao original restaurado e ao arquivo BIFT completo,
  incluindo cabeçalho, árvore e último byte de dados com preenchimento.

Limites e métricas
  O tamanho original da v1 é limitado a 4294967295 bytes (2^32 - 1).
  A segunda leitura deve processar exatamente a quantidade contada na primeira.
  Também é recusado um tamanho comprimido que não caiba em unsigned long.
  Os tamanhos são contados durante o processamento, sem depender de ftell.
  A interface mostra tamanhos, tempo e razão somente após uma operação bem-sucedida.
  A estrutura Estatisticas continua contendo apenas os tamanhos.
  Razão: tamanho_comprimido / tamanho_original; para vazio, não aplicável.
  O tempo é a diferença das leituras de clock() antes e depois da operação, dividida
  por CLOCKS_PER_SEC. Inclui abertura, processamento e fechamento dos arquivos.
  No ambiente verificado, CLOCKS_PER_SEC é 1000. A exibição usa seis casas decimais,
  sem garantir precisão de microssegundos; intervalos curtos podem aparecer como zero.
  Se clock() falhar ou retroceder, o tempo aparece como indisponível, preservando
  o resultado da operação e as demais métricas.
  Arquivos pequenos ou pouco redundantes podem aumentar por causa do cabeçalho.
  A v1 não tem checksum: a validação do formato não detecta toda alteração dos dados.
  O limite real de 4294967295 bytes não foi exercitado nesta conferência.

Conferência dos dois fluxos
  Compilação C90 integrada e scripts/check_basics.py aprovados.
  Foram conferidos vazio, símbolo único, texto com caminhos com espaços, todos os
  bytes, códigos longos e binário de 1 MiB. Os 11 ciclos usando comprimir_arquivo e
  descomprimir_arquivo tiveram comparação fc.exe /b com retorno zero e estatísticas
  corretas. Os arquivos comprimidos coincidiram com os gerados na etapa da #5.
  Três exemplos BIFT montados por bytes foram restaurados independentemente do
  compressor. Dezoito entradas inválidas e seis casos de abertura/destino foram
  recusados, preservando a entrada e sem deixar uma saída parcial.
  Dez falhas simuladas de leitura, escrita, flush, fechamento e alocação confirmaram
  os retornos, as estatísticas preservadas e a liberação dos nós alocados.

Conferência do menu
  Compilação C90 integrada e scripts/check_basics.py aprovados após a integração.
  Quatro ciclos pelo executável, com vazio, símbolo único, texto com espaços e todos
  os bytes, tiveram comparação fc.exe /b com retorno zero e métricas corretas.
  Foram conferidos opções inválidas, EOF, linhas vazias/longas, limites dos buffers,
  extensão existente, destinos preservados e arquivo comprimido inválido.
  Erros após uma operação bem-sucedida não reapresentaram estatísticas anteriores.
  Relógio indisponível no início/fim, retrocesso e intervalo zero foram simulados;
  as operações continuaram corretas, com a apresentação esperada do tempo.
  A conferência completa do pacote em pasta limpa fica para a issue #8.
