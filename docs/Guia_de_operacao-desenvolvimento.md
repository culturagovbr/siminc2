# Fluxo comum de trabalho para ser possível fazer entregas parciais(HOTFIX).

Cada desenvolvedor criará uma branch a partir da master.

## Criando uma branch a partir da branch master
    
    $ git branch
    $ git fetch
    $ git checkout master
    $ git pull origin master
    $ git checkout -b tipo-nºissue-modulo-nomeDemanda
    Eg: $ git checkout -b hotfix-007-planejamento-documentos

    master  o-------o-------------o---------------------------------------------
                     \
                      o------------------- hotfix-007-planejamento-documentos --
                    
<b>Obs:</b>Ao criar a branch, adicione o nome da branch no card(Issue) e suas respectivas observações, caso haja,
facilitando aos membros que forem versionar a demanda, e evitando erros em ambiente de produção.<br>
<b>Eg:</b> '- [ ] Publicar menu em produção.', ' - [ ] Enviar Script para produção.'

## Faça commits na sua branch e envie para a branch remota no Github (origin)
    
    $ git branch
    $ git status
    $ git add docs/Guia_de_operacao-desenvolvimento.md
    $ git status
    $ git commit -m '[ FIX ] Módulo X - funcionalidade y. Issue #007' -m 'Comentário livre e podendo ser com texto longo'
    $ git push origin hotfix-007-planejamento-documentos


Para visualizar uma lista completa do padrão de versionamento de código [clique aqui](https://github.com/devbrotherhood/codeversioningpattern).


    master  o-------o-------------o---------------------------------------------
                     \
                      o------o-o-o--o----- hotfix-007-planejamento-documentos --

## Atualizando a branch de teste com as alterações mais recentes da demanda feita.

    $ git branch
    $ git fetch
    $ git checkout homologacao
    $ git pull origin homologacao
    $ git merge hotfix-007-planejamento-documentos
    $ git push origin homologacao

    master  o-------o-------------o---------------------------------------------
                     \
            o----o------------o---o-o---o--------- homologacao -----------------
                       \               /
                        o------o-o-o--o--- hotfix-007-planejamento-documentos --


## Homologando e publicando uma versão para a master

Certifique-se de que a demanda está sem divergências e se foi criada do local certo como já foi citado acima, caso esteja tudo correto, siga os passoo a passos abaixo

### (1) Caso de sucesso

    Solicite um pull request da branch em que foi feito o trabalho hotfix-007-planejamento-documentos para a branch Master e solicite a revisão de código do pacote de atualização a um membro da equipe de desenvolvimento.
    
    Detalhe: Selecione apenas a branch que foi homologada e solicite um pull request para master.


    master  o-------o-------------o-----o---------------------------------------
                     \                 /
                      \               /
                       o------o-o-o--o---- hotfix-007-planejamento-documentos --


### (2) Caso de falha

    Utilize sua branch para realizar as alterações e depois prossiga o passo de mandar para teste.


## Termos e comandos usados neste documento

##### Buscar referências/meta-dados atualizados das branchs remotas(origin)
$ git fetch

##### Conferir qual é a branch local atual em que se está trabalhando
$ git branch

##### Mudar para a branch master
$ git checkout master
    
##### Atualizar a branch master de acordo com as mudanças no remoto (origin)
$ git pull origin master
    
##### Criar uma branch nova a partir da branch atual
$ git checkout -b tipo-nºissue-modulo-nomeDemanda

##### Conferir status da branch exibindo arquivos modificados, adicionados pra commit ou conflitos
$ git status

##### Revisar as alterações feitas nos arquivos modificados antes de adicionar para dar commit e push
$ git diff docs/Guia_de_operacao-desenvolvimento.md

##### Adicionar arquivos modificados na lista pra serem enviados no próximo commit
$ git add docs/Guia_de_operacao-desenvolvimento.md

##### Enviar os arquivos modificados para um marco histórico de mundaça na branch local
$ git commit -m '[ FIX ] Módulo X - funcionalidade y. Issue #007' -m 'Comentário livre e podendo ser com texto longo'

##### Mandar seus commits e sua branch para a branch no remoto do Github (origin)
$ git push origin hotfix-007-planejamento-documentos

##### Remover branch local
$ git branch -D hotfix-007-planejamento-documentos

##### Caso tenha começado a modificar arquivos na branch errada e deseje esconder as alterações pra mudar de branch ou fazer outras operações
$ git stash

##### Mostrar e voltar alterações escondidas pelo $ git stash
$ git stash pop
