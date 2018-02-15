# Roteiro de publica��o de release


## Fluxo de desenvolvimento (simplificado)

Existem dois tipos de releases: hotfixes e features. Cada um segue um caminho diferente de desenvolvimento e publica��o.

## 1) Hotfix

Hotfix � uma altera��o emergencial a ser aplicada � produ��o. Envolve alguns commits no m�ximo e dificilmente dura mais que um dia.

Para criar uma hotfix, siga os passos abaixo:

    $ git checkout master
    $ git checkout -b hotfix-nome-da-correcao
    $ ... comite suas corre��es
    $ git push hotfix-nome-da-correcao

    No github:
    * crie um pull request da branch hotfix-nome-da-correcao em https://github.com/culturagovbr/siminc2/pulls
    * revise o pull request
    * aceite ou rejeite
    * crie uma nova release em https://github.com/culturagovbr/siminc2/releases
    (draft a new release)
    * preencha a release com:
      - n�mero da tag
      - t�tulo da release
      - texto descritivo contendo modelo anterior do CHANGELOG (listagem de novas funcionalidades):
        Release 2.x.x
	* [FIX] �rea do sistema: descri��o da altera��o (#3 [n�mero da issue])
	(O identificador da issue deveria ser informado sempre que houver)

    Publica��o da release
    * acesse os n�s
    * cd /var/www/html/siminc2
    * git pull origin master
    
## 2) Feature

Para criar uma feature, siga os passos abaixo:

    $ git checkout develop
    $ git checkout -b feature-nome-da-feature
    $ ... comite suas corre��es
    $ git checkout test
    $ git merge feature-nome-da-feature
    $ git push origin test
    
    ... homologa��o pelo cliente
    
    Se aceito:
    $ git checkout develop
    $ git merge feature-nome-da-feature
    $ git push origin develop
    
    Se rejeitado:
    * n�o sobe para develop e volta para o desenvolvimento da feature
    
    No github (ir� publicar todas as features aprovadas e mergeadas na develop):
    * crie um pull request da feature develop em https://github.com/culturagovbr/siminc2/pulls
    * revise o pull request
    * aceite ou rejeite
    * crie uma nova release em https://github.com/culturagovbr/siminc2/releases
    (draft a new release)
    * preencha a release com:
      - n�mero da tag
      - t�tulo da release
      - texto descritivo contendo modelo anterior do CHANGELOG (listagem de novas funcionalidades)
        Release 2.x.x
	* [FIX] �rea do sistema: descri��o da altera��o (#3 [n�mero da issue])
	(O identificador da issue deveria ser informado sempre que houver)

    Publica��o da release
    * acesse os n�s
    * cd /var/www/html/siminc2
    * git pull origin master
