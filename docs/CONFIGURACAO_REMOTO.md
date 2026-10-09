# Remoto e proteção da main

Repositório público: https://github.com/BruBrunellis/projeto_pe_turma_a2_compressor

## Regras aplicadas no servidor

- Branch padrão: `main`.
- Alterações entram por pull request, com pelo menos uma aprovação de outro integrante.
- Novos commits invalidam aprovações anteriores; conversas devem estar resolvidas.
- O check **Verificacoes basicas** precisa passar e a branch deve estar atualizada com a `main`.
- As regras também se aplicam a administradores.
- Force push e exclusão da `main` ficam bloqueados.
- Merge commits habilitados; squash e rebase desabilitados para preservar os commits individuais.
- Branches de PRs integrados são apagadas automaticamente.

O responsável pode inspecionar as regras em **Settings → Branches**. Alterar este documento não altera as regras do GitHub.

## Verificações automáticas pequenas

O arquivo `.github/workflows/basico.yml` executa um único job em PRs para `main`, em pushes na `main` e por acionamento manual:

1. `git diff --check` em todos os arquivos rastreados: detecta espaços no fim das linhas e marcadores de conflito introduzidos no diff.
2. Compilação individual de cada `.c` rastreado com GCC, `-std=c90 -pedantic-errors -Wall -Wextra`, incluindo as pastas raiz, `include/` e `src/`.

Enquanto não houver fontes C, a segunda etapa informa explicitamente que não se aplica. Ela gera objetos temporários: não faz a ligação do executável nem testa o comportamento do compressor. Quando existir a implementação, acrescentar o comando oficial de build e casos de comprimir/descomprimir com comparação byte a byte. C90 é a referência inicial para ANSI C, a confirmar com o docente.

Para executar as mesmas verificações localmente, com Python 3, Git e GCC no PATH:

```sh
python scripts/check_basics.py
```

Adicionar arquivos novos com `git add` antes da execução para incluí-los na verificação. Os objetos temporários são removidos ao terminar.

## Entrada dos demais integrantes

Convidar os outros quatro integrantes em **Settings → Collaborators**, com acesso de escrita. Cada integrante clona o repositório com sua própria conta e segue o [guia de contribuição](../CONTRIBUTING.md).

É necessário ao menos outro integrante com acesso para aprovar PRs: o autor não pode aprovar o próprio PR, e o administrador também está sujeito à proteção.

## Referências

- [Branches protegidas](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/managing-protected-branches/about-protected-branches)
- [API de proteção](https://docs.github.com/en/rest/branches/branch-protection)