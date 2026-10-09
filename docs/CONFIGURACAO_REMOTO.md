# Configuração inicial do remoto (GitHub)

Hipótese de hospedagem: GitHub. Adaptar as mesmas regras se o grupo escolher outro servidor.
Este documento é um roteiro de configuração; sua presença no Git **não ativa** proteção.

## Publicar a base

1. O responsável cria um repositório remoto vazio, sem README gerado pelo servidor, com o nome combinado e a visibilidade escolhida pelo grupo.
2. Na pasta local, já inicializada com branch `main`, confira `git status`, revise e publique a base. Se o commit inicial já tiver sido criado, pule os comandos git add e git commit. Substitua `URL_DO_REMOTO` pela URL real:

```sh
git add README.md CONTRIBUTING.md docs/CONFIGURACAO_REMOTO.md .github/pull_request_template.md .gitignore
git commit -m "Documenta fluxo de colaboracao do grupo"
git remote add origin URL_DO_REMOTO
git push -u origin main
```

Esse é o push de bootstrap, antes de proteger a branch e iniciar as contribuições. Se o remoto já tiver histórico, reconciliar os históricos antes; não usar force push para sobrescrevê-lo.

3. Confirmar `main` como branch padrão. Convidar os outros quatro integrantes com acesso de escrita (Write, onde houver seleção de papel). Não conceder Admin para o trabalho cotidiano.

## Proteger a main antes de começar

Em **Settings → Branches → Add branch protection rule**, criar uma regra para o nome exato `main`:

- Ativar **Require a pull request before merging**.
- Exigir **1 aprovação** de outro integrante.
- Ativar **Dismiss stale pull request approvals when new commits are pushed**.
- Ativar **Require conversation resolution before merging**.
- Ativar **Do not allow bypassing the above settings**, incluindo administradores na aplicação das regras.
- Deixar **Allow force pushes** e **Allow deletions** desativados.
- Deixar checks obrigatórios desativados nesta fase, pois não há CI.

Não configurar lista de pessoas autorizadas a ignorar PRs. Não bloquear toda atualização da branch: o merge de PRs aprovados precisa continuar permitido.

Em **Settings → General → Pull Requests**, habilitar merge commits e desabilitar squash/rebase para manter um único procedimento e preservar commits individuais. Habilitar exclusão automática de branches após merge. Não exigir histórico linear, pois o fluxo usa merge commits.

## Verificar que a proteção está ativa

Criar uma branch de documentação, enviar um pequeno ajuste real e abrir PR contra `main`. Conferir que o merge fica bloqueado antes da aprovação; pedir a outro integrante que revise. Enviar uma correção adicional e conferir que a aprovação anterior é invalidada. Após nova aprovação e resolução das conversas, integrar pelo botão de merge.

Conferir também nas configurações que force push e exclusão estão proibidos e que administradores não têm bypass. Não é necessário tentar um push destrutivo para validar essas opções.

## Disponibilidade do recurso

A documentação do GitHub informa que proteção de branches está disponível em repositórios públicos no Free; para privados, depende de Pro, Team ou Enterprise. Conferir o plano do proprietário e se a regra é efetivamente aplicada. Não tornar público o trabalho apenas para habilitar a proteção sem uma decisão do grupo.

Se o plano não permitir proteção no privado, o acordo de PRs continua útil, mas não bloqueia tecnicamente pushes diretos. Uma alternativa é manter escrita no repositório central apenas com um integrador e receber PRs de forks, se a política de forks privados permitir, até escolher uma hospedagem/plano compatível. CI também não resolve essa limitação de permissão.

Documentação oficial consultada em 09/10/2026:

- [Sobre branches protegidas](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/managing-protected-branches/about-protected-branches)
- [Gerenciar uma regra de proteção](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/managing-protected-branches/managing-a-branch-protection-rule)
