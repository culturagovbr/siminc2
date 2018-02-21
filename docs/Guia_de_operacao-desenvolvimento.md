# Fluxo comum de trabalho

Al�m da master, existem duas branches principais de trabalho no SIMINC2, identificadas com as duas frentes de trabalho:

               ---- modulo-novo ------
              /
    master   o-----------------------
              \
               ---- feature ---------

Cada desenvolvedor criar� uma branch a partir de uma das duas.

## Criando uma branch a partir da branch feature
    
        $ git checkout feature
    $ git checkout -b feature-nome-demanda


    master   o-------------------------------
              \
               o------- feature --------------
                \
                 o--- feature-nome-demanda ----

## Fa�a commits na sua branch e envie para o gitlab (origin)
    $ git commit -m 'fix: funcionalidade x'
    $ git commit -m 'fix: funcionalidade y'
    $ git push origin feature-nome-demanda

    master   o-----------------------------------------
              \
               o---------------- feature ---------------
                \
                 o---o----o----- feature-nome-demanda ---

## Atualizando sua branch com as altera��es mais recentes do feature

    $ git checkout feature-nome-demanda
    $ git fetch
    $ git merge feature
    $ git push origin feature-nome-demanda

    master   o----------------------------------------------
              \
               o-----o----o----o--- feature -----------------
                \               \
                 o---o----o------o--- feature-nome-demanda ---

## Enviando suas altera��es para a branch feature

    $ git checkout feature
    $ git fetch
    $ git merge feature-nome-demanda
    $ git push origin feature

    master   o-----------------------------------------------------------
              \
               o-----------------------o ------ feature ------------------
                \                     /
                 o---o----o------o---o -------- feature-nome-demanda ------


## Homologando e publicando uma vers�o para a master

Certifique-se de que est� na branch correta. Fa�a os testes; caso encontre bugs pequenos (1), n�o h� problema que a corre��o seja feita diretamente na branch feature mesmo. Caso se tratem de altera��es maiores (2), melhor voltar a trabalhar no seu branch para s� ent�o publicar o resultado na feature:

(1)

    $ git checkout feature
    $ git commit -m 'fix: pequena correcao'
    $ git push origin feature

(2)

    $ git checkout feature-nome-demanda
    $ git commit -m 'fix maior'
    $ git checkout feature
    $ git merge feature-nome-demanda
    $ git push origin feature-nome-demanda


Se estiver tudo ok, edite o CHANGELOG e crie a tag

    $ vim CHANGELOG
    $ git add CHANGELOG
    $ git commit -m 'adicionando alteracoes do CHANGELOG'
    $ git tag -a v1.7.1 -m 'release de correcoes na proposta'
    $ git push origin feature
    

Na produ��o:

    $ git fetch
    $ git checkout -b v1.7.1


