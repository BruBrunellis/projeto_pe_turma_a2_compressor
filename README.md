# BitForest — PE, turma A2

Trabalho de Programação Estruturada da UFABC, quadrimestre 2026.3.
Implementação em C de um compressor e descompressor de arquivos de texto e binários.
O executável é `bin/bitforest.exe` e a extensão dos arquivos comprimidos é `.bift`.

## Trabalho em equipe

Leia [CONTRIBUTING.md](CONTRIBUTING.md) para criar branches, enviar commits e revisar pull requests.
O responsável pelo remoto deve seguir [a configuração de proteção da main](docs/CONFIGURACAO_REMOTO.md).

**Estado atual:** a compressão Huffman em duas passagens está implementada em `src/compressor.c`. A descompressão e o menu completo ainda estão pendentes. Consulte [Readme.txt](Readme.txt) para compilação, formato BIFT v1, contratos e limites, e [configuração do remoto](docs/CONFIGURACAO_REMOTO.md) para as verificações automáticas.
Remoto público: https://github.com/BruBrunellis/projeto_pe_turma_a2_compressor. A main exige PR, revisão e verificações básicas aprovadas no servidor.

## Requisitos extraídos da proposta

Fonte: `PE_2026_3_Trabalho.pdf`, disponível na pasta PE. Este resumo não substitui o roteiro.

- Código de autoria dos integrantes, em ANSI C, compilável com GCC; não usar bibliotecas que implementem as etapas essenciais de compressão, processamento de cadeias ou ordenação pronta.
- Escolher e fundamentar um algoritmo tradicional, como Huffman ou Ziv-Lempel; modularizar com TADs e cuidar de alocação/liberação de memória.
- Nome exclusivo e menu no terminal: comprimir, descomprimir e sair.
- Aceitar caminhos absolutos e relativos; usar extensão personalizada no arquivo comprimido e permitir escolher o nome do restaurado.
- Preservar os originais e restaurar o conteúdo exatamente, byte a byte.
- Exibir tempo e tamanhos em bytes; calcular a razão `tamanho comprimido / tamanho original`. Definir tratamento para entrada vazia, sem dividir por zero.
- Avaliar baixa, média e alta redundância e comparar com gzip ou equivalente no Windows.
- Discutir uma funcionalidade adicional: o roteiro a apresenta como possibilidade/incentivo, mas também a menciona nos critérios de avaliação; confirmar a expectativa com o docente.

## Entrega e apresentação

- Um integrante envia pelo Moodle o código `.c`/`.h`, `leia-me.txt` com instruções de compilação ou `Makefile`, relatório PDF e slides, se usados.
- Identificar nominalmente todos os integrantes. Relatório: até 20 páginas, Arial 10, espaçamento duplo, decisões técnicas, testes ilustrados e análise de desempenho.
- Todos devem participar e dominar o trabalho; haverá compilação e execução no laboratório. Apresentação: 10 minutos de exposição e 10 de arguição.
- Grupos normalmente têm quatro integrantes; cinco são admitidos excepcionalmente. Todos devem ser da mesma turma.
- Reserva de apresentação: 23–30/10/2026; entrega: 30/10–05/11/2026; apresentações: 06 e 13/11/2026, das 8h às 10h. Acompanhar eventuais alterações no Moodle.

## Organização sugerida para cinco pessoas

Definam primeiro o algoritmo, formato do arquivo e contratos dos módulos (headers, retornos de erro e responsabilidade por liberar memória).

| Frente | Responsabilidade inicial |
| --- | --- |
| Integrante 1 | Menu, caminhos, integração e instruções de compilação |
| Integrante 2 | Estruturas de dados e construção do modelo |
| Integrante 3 | Codificação e escrita do arquivo comprimido |
| Integrante 4 | Decodificação e reconstrução do original |
| Integrante 5 | Testes de integridade, experimentos e comparação de desempenho |

A divisão é por responsabilidade inicial, não por exclusividade. Todos programam, revisam e escrevem a parte do relatório referente às próprias decisões. Rodiziem revisores para compartilhar conhecimento.

Comecem com um caminho completo pequeno (comprimir e restaurar uma entrada simples), ampliem os casos e depois façam os experimentos. Evitem integrar os cinco módulos apenas perto da entrega.
