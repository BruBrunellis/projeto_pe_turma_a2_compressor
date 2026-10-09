# Como contribuir

## Acordo do grupo

Fluxo: **tarefa → branch → commits → push → pull request → revisão → merge na main**.

- A `main` representa a versão integrada e deve continuar compilando quando houver código.
- Cada tarefa usa uma branch curta, por exemplo `feat/menu`, `feat/decoder`, `fix/arquivo-vazio` ou `docs/relatorio`. Evitem branches permanentes por integrante e uma branch `develop` adicional.
- Os cinco podem enviar commits às branches do remoto. Ninguém envia diretamente à `main` após a configuração inicial.
- Cada PR precisa de uma aprovação de outro integrante e de todas as conversas resolvidas. Correções enviadas depois da aprovação exigem nova revisão.
- Façam PRs pequenos, com um objetivo e instruções para verificar. Abram como Draft se precisarem de ajuda antes de terminar.
- Usem **Create a merge commit** para preservar os commits e a autoria individual. Não reescrevam o histórico compartilhado com force push.
- O autor pode fazer o merge após a aprovação e as verificações; não precisa esperar um único líder. Apaguem a branch remota depois do merge.

## Primeira vez em cada computador

Substituam `URL_DO_REMOTO`, o nome e o e-mail pelos valores reais. Cada pessoa usa sua própria conta e identidade Git.

```sh
git clone URL_DO_REMOTO
cd projeto_pe_turma_a2_compressor
git config user.name "Seu Nome"
git config user.email "seu-email"
```

## Começar uma tarefa

Com a árvore de trabalho limpa (confira `git status`):

```sh
git switch main
git pull --ff-only origin main
git switch -c feat/menu
```

Combinem a tarefa em uma issue ou no canal do grupo, principalmente antes de alterar headers ou o formato do arquivo comprimido.

## Enviar trabalho

Verifiquem o diff e adicionem explicitamente os arquivos da tarefa. Os caminhos abaixo são exemplos para quando existirem.

```sh
git diff
git add src/menu.c include/menu.h
git commit -m "Adiciona menu de compressao e descompressao"
git push -u origin feat/menu
```

No GitHub, abram um PR com destino `main`, preencham o modelo e solicitem revisão de outro integrante. Novos commits na mesma branch atualizam o mesmo PR.

## Atualizar a branch e resolver conflitos

Com as alterações locais já commitadas:

```sh
git fetch origin
git switch feat/menu
git merge origin/main
```

Se houver conflitos, ajustem os arquivos junto ao responsável pelo trecho, removam os marcadores, adicionem os arquivos resolvidos com `git add` e finalizem com `git commit`. Compilem e repitam os testes antes de `git push`. Para desistir de uma resolução em andamento, usem `git merge --abort`.

Depois que o PR for integrado:

```sh
git switch main
git pull --ff-only origin main
git branch -d feat/menu
```

## Revisão e validação manual

O revisor lê o diff e reproduz a validação relevante. No início, PRs de documentação exigem apenas conferência do conteúdo. Quando houver programa:

- Compilar a versão integrada com GCC e o comando oficial do projeto. Acordar a interpretação de ANSI C com o docente; até lá, considerar C90 estrito como referência e registrar a decisão.
- Para alterações do codec: comprimir, descomprimir e comparar bytes. No Windows, usar `fc.exe /b original restaurado`; no Linux, `cmp original restaurado`. A comparação precisa terminar com código zero.
- Cobrir progressivamente: vazio, um único símbolo repetido, texto, binário com todos os valores de byte, arquivo maior e entrada inválida/truncada.
- Conferir preservação do original, mensagens de erro, liberação de memória e contratos dos headers afetados.
- Registrar no PR comandos, resultado e limitações. Não exigir melhoria de compressão para toda entrada: cabeçalhos podem aumentar arquivos pequenos ou pouco redundantes.

## Configuração local adicional

A pasta inicial usa `pull.ff=only` (evita merges involuntários no pull) e `fetch.prune=true` (limpa referências de branches removidas no remoto). Cada integrante deve repetir estas configurações após clonar, pois elas não são versionadas:

```sh
git config --local pull.ff only
git config --local fetch.prune true
```