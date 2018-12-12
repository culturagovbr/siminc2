# Sistemas de versionamento

## Git

<https://github.com/culturagovbr/siminc2/tree/simec>

Sistema em implantação; novas alterações estão sendo feitas no Git

# Esquema de versionamento no Git

* Branch Simec: sistema em produção, segundo tags de versão de publicação
* Branch dev-backlog (operação / manutenção)
* Branch dev-novaIN (implementação de novas funcionalidades da nova instrução normativa)

Cada desenvolvedor faz uma branch a partir das branches dev-backlog ou dev-novaIN

# Tagueando versões

<https://git-scm.com/book/en/v2/Git-Basics-Tagging>


# Número de versões

    https://en.wikipedia.org/wiki/Software_versioning#Change_significance

    major.minor.minor-minor
    1.0.0 -> reestruturacao
    1.0.1 -> bugfixes
    1.1.0 -> novas funcionalidades
    1.1.1 -> bugfixes
    1.1.2 -> bugfixes
    1.1.3 -> bugfixes
    1.2.0 -> novas funcionalidades
    1.2.1 -> bugfixes
    2.0.0 -> reestruturacao
    
    Major: alteracao completa de funcionalidades / refatoracoes
    Minor: novas funcionalidades
    Minor-minor: bugfixes

# Versão inicial git:

O sistema anterior estava na versão:

    Branch|Tag: release-1.2 Revisao: 562

Portanto, o novo irá assumir a versão 1.3.0.
