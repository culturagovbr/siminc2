# Sistemas de versionamento

## Git

<http://git.cultura.gov.br/sistemas/siminc2>

Sistema em implanta��o; novas altera��es est�o sendo feitas no Git

# Esquema de versionamento no Git

* Branch Master: sistema em produ��o, segundo tags de vers�o de publica��o
* Branch dev-backlog (opera��o / manuten��o)
* Branch dev-novaIN (implementa��o de novas funcionalidades da nova instru��o normativa)

Cada desenvolvedor faz uma branch a partir das branches dev-backlog ou dev-novaIN

# Tagueando vers�es

<https://git-scm.com/book/en/v2/Git-Basics-Tagging>


# N�mero de vers�es

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

# Vers�o inicial git:

O sistema anterior estava na vers�o:

    Branch|Tag: release-1.2 Revisao: 562

Portanto, o novo ir� assumir a vers�o 1.3.0.
