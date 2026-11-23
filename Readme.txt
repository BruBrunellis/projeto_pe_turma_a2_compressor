BitForest - Programação Estruturada, UFABC, turma A2

Integrantes

Bruno Vital Brunelli (11202420920)
Eduardo Dal Molin Morales (11202422080)
Giovanna da Silva Champam (11202131886)
João Paulo Sanches Lira (11202520348)
Kauann Antevere Ramos (11202521805)

============================================================================

Ambiente de compilação (Windows / PowerShell)

  Validado com GCC 16.1.0 e GNU Make 4.4.1 do MSYS2 UCRT64.
  A compilação usa C90 (-std=c90 -pedantic-errors), com -Wall -Wextra.
  O código do explorador e o da biblioteca tinyfiledialogs já acompanham
  o projeto. O GCC é necessário nas duas opções. O Make é opcional.

1. Instalar as ferramentas (uma vez por computador)

  Se ainda não possui as ferramentas, instale o MSYS2 pelo site oficial:
  https://www.msys2.org/

  As instruções abaixo usam a instalação em C:\msys64, para Windows x64.
  Abra "MSYS2 UCRT64" pelo menu Iniciar. Nesse terminal, execute:

  pacman -Syu

  Se a atualização pedir para fechar o terminal, confirme, abra novamente
  o MSYS2 UCRT64 e repita pacman -Syu para concluir a atualização.
  Depois, instale o GCC e o GNU Make:

  pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-make

  Para compilar manualmente, basta instalar mingw-w64-ucrt-x86_64-gcc.

  Aguarde a conclusão da instalação. Use o PowerShell do Windows para
  os comandos das próximas seções.

  Referência para atualização: https://www.msys2.org/docs/updating/

2. Preparar o PowerShell (em cada nova sessão)

  Extraia o pacote inteiro para uma pasta, preservando src/, include/
  e o Makefile.
  Se clonou o repositório, use a pasta criada pelo Git. Abra o PowerShell
  e substitua o caminho de exemplo pelo local do projeto:

  Set-Location "C:\caminho\para\BitForest"
  $env:Path = "C:\msys64\ucrt64\bin;$env:Path"

  Se instalou o MSYS2 em outro local, ajuste o caminho terminado em
  ucrt64\bin. O ajuste do PATH vale somente para a sessão atual.
  Confira as ferramentas:

  gcc --version
  mingw32-make --version
  Get-Command gcc, mingw32-make | Select-Object Name, Source

  Os executáveis devem estar em C:\msys64\ucrt64\bin ou na pasta
  equivalente da sua instalação. Se for compilar manualmente, basta
  verificar o GCC.

3. Opção A - Compilar pelo Makefile

  Na raiz do projeto, no PowerShell preparado na seção 2, execute:

  mingw32-make

  O Makefile cria bin/ quando necessário e gera bin/bitforest.exe.
  No Windows, ele vincula as bibliotecas usadas pelos diálogos de arquivo.
  Após alterar os fontes ou cabeçalhos, execute mingw32-make novamente.
  A mensagem "Nothing to be done for 'all'" indica que o executável já
  está atualizado.

  Para recompilar do zero, feche o programa e execute uma linha por vez:

  mingw32-make clean
  mingw32-make

  O comando clean remove a pasta bin/. Reserve-a para arquivos gerados.

4. Opção B - Compilar manualmente, sem Make

  Na mesma raiz, com o PATH preparado na seção 2, execute:

  New-Item -ItemType Directory -Force -Path .\bin | Out-Null
  gcc -std=c90 -pedantic-errors -Wall -Wextra -Iinclude src/main.c src/huffman.c src/compressor.c src/bits.c src/explorador.c src/tinyfiledialogs.c -o bin/bitforest.exe -lcomdlg32 -lole32 -lgdi32

  Copie o comando gcc inteiro como uma única linha. Ele compila os seis
  arquivos .c e vincula as bibliotecas do Windows usadas pelos diálogos.
  Para recompilar, feche o programa e execute o mesmo comando novamente.

5. Conferir o resultado e executar

  Imediatamente depois do comando mingw32-make ou gcc, confira:

  $LASTEXITCODE

  O valor 0 indica sucesso. O GCC pode emitir avisos da tinyfiledialogs
  e concluir a compilação. Se o código for diferente de 0, corrija a
  falha antes de executar, pois bin/ pode conter um executável antigo.
  Confira o arquivo gerado e abra o programa:

  Get-Item .\bin\bitforest.exe
  .\bin\bitforest.exe

============================================================================

Execução

  Digite uma opção por linha no terminal: 1 para Comprimir, 2 para
  Descomprimir ou 3 para Sair. EOF na leitura do menu encerra o programa.

  Ao escolher 1 ou 2, a janela "Selecionar arquivo" pede a origem.
  Em seguida, a janela "Salvar arquivo" permite escolher a pasta e
  informar o nome da saída. Na descompressão, escolha o arquivo .bift
  como origem e dê um nome novo ao arquivo restaurado.

  O BitForest volta ao menu depois de cada operação. Cancelar qualquer
  uma das janelas também volta ao menu, sem iniciar a operação.

Uso dos caminhos

  Navegue até a pasta desejada nas janelas. Nomes com espaços podem
  ser selecionados normalmente.

  Ao comprimir, se você salvar com o nome dados.bin, o BitForest grava
  dados.bin.bift. Um nome que já termina em .bift ou .BIFT é mantido.
  Ao descomprimir, o programa usa o nome escolhido para o restaurado.

  Cada caminho deve caber em 512 bytes, incluindo o terminador nulo.
  Na compressão, a extensão acrescentada também precisa caber.

  Escolha um destino novo. O BitForest preserva o original e recusa
  destinos existentes, mesmo que o diálogo ofereça a opção de substituir
  um arquivo. Se houver falha de abertura, confira a origem, as permissões
  e se o nome final do destino já existe.

============================================================================

Formato do arquivo comprimido (BIFT v1)

  Os inteiros são gravados em big-endian e os dados são lidos do bit mais
  significativo ao menos significativo em cada byte.

  4 bytes  assinatura "BIFT" (42 49 46 54 em hexadecimal)
  1 byte   versão (1)
  4 bytes  tamanho original em bytes
  2 bytes  tamanho da árvore serializada em bytes (0 se o original é vazio)
  N bytes  árvore em pré-ordem: nó interno = 0, folha = 1 seguida do byte do símbolo
  ...      códigos de Huffman concatenados, com zeros até completar o último byte

  Um arquivo vazio tem árvore e dados vazios. Quando a entrada tem um
  único símbolo, o código é 0, com um bit por ocorrência.

  A descompressão termina ao restaurar a quantidade original de bytes.
  O programa valida os zeros de preenchimento e recusa bytes adicionais
  após os dados compactados.

Retornos de comprimir_arquivo/descomprimir_arquivo

  COMPRESSOR_OK (0) ou um código de erro definido em include/compressor.h.

============================================================================

Limites e métricas

  O tamanho original da v1 é limitado a 4294967295 bytes (2^32 - 1).
  A segunda leitura da compressão deve processar a mesma quantidade de
  bytes contada na primeira. O tamanho comprimido deve caber em unsigned
  long. Os tamanhos são contados durante o processamento, sem usar ftell.

  Após uma operação bem-sucedida, o programa mostra os tamanhos, o tempo e
  a razão tamanho comprimido / tamanho original. Para arquivo original
  vazio, a razão aparece como não aplicável. A estrutura Estatisticas
  armazena somente os tamanhos.

  O tempo é calculado com clock(), em segundos, e inclui abertura,
  processamento e fechamento dos arquivos. A escolha nas janelas ocorre
  antes da contagem. No ambiente conferido, CLOCKS_PER_SEC é 1000.
  A exibição tem seis casas decimais e intervalos curtos podem aparecer
  como zero. Se clock() falhar ou retroceder, o tempo aparece como
  indisponível e as demais métricas são mantidas.

  Arquivos pequenos ou pouco redundantes podem aumentar por causa do
  cabeçalho e da árvore armazenados.

============================================================================

Problemas frequentes

  - "gcc" ou "mingw32-make" não é reconhecido:
    confira a instalação da seção 1 e repita o ajuste do PATH da seção 2.
    Se instalou apenas o GCC, use a compilação manual da seção 4.

  - GCC termina sem criar o executável e $LASTEXITCODE é diferente de 0:
    coloque ucrt64\bin no início do PATH, como na seção 2, e tente novamente.
    Se persistir, acrescente -v após gcc e confira a mensagem de erro.

  - Makefile ou fonte não encontrado:
    use Set-Location para entrar na pasta que contém Makefile, src/ e include/.

  - "undefined reference":
    use o Makefile fornecido ou confira os seis arquivos .c e as opções
    -lcomdlg32, -lole32 e -lgdi32 ao final do comando da seção 4.

  - "Permission denied" ao gerar bitforest.exe:
    feche uma instância do programa que ainda esteja aberta e confira se
    você tem permissão de escrita na pasta do projeto.

  - Falha de download ou certificado durante pacman:
    consulte a documentação do MSYS2 da seção 1 antes de prosseguir.
