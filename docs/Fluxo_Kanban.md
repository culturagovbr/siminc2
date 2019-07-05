# Workflow do Kanban

## Zenhub

<https://www.zenhub.com/>

Como ferramenta, utiliza-se o Zenhub, que facilita a comunicação
com o github e torna o processo de desenvolvimento mais fácil.


## Entendendo o processo

No projeto SIMINC2, as demandas são solicitadas e entregues por Sprints, da
metodologia Scrum, geralmente, quinzenalmente.
<br>Cada Sprint contém um pacote de demandas que o demandante faz
para o Time de Desenvolvimento.


## Entendendo as prateleiras

### Próxima Sprint

Contém apenas as demandas que o demandante solicitará para a Sprint seguinte.
Apenas o demandante move as Issue's(card's) para as outras prateleiras.


### Prateleira

Contém todas as demandas que foram solicitadas ao Time de Desenvolvimento.


### A Fazer

Contém todas as demandas que o Time de Desenvolvimento atenderá.<br>
<b>Obs:</b> Quando algum membro do Time de Desenvolvimento for realizar a demanda,
o mesmo deverá mover o card(Issue) para a prateleira de <b>Fazendo</b>!<br> Vale ressaltar que
quando houverem <b>observações a serem feitas na hora da publicação</b>, o responsável pela demanda deverá
deixar um comentário abaixo do nome da branch especificando as observações que o publicador deverá
atender ou tomar precaução na hora da publicação.


### Fazendo

Contém todas as demandas que o Time de Desenvolvimento está atendendo atualmente.<br>
<b>Obs:</b> Quando a demanda for finalizada, o responsável deverá mover o card(Issue) para
a prateleira de <b>Teste</b> e solicitar que outro membro do Time de Desenvolvimento possa testar para liberar
a demanda ao cliente.


### Impedimentos

Contém todas as demandas que foram paralizadas porque possuem um empecilho não solucionado.<br>
<b>Obs:</b> Quando o cliente ou algum membro do Time de Desenvolvimento mover algum card(Issue) para este item da
prateleira, o mesmo deverá especificar no card(Issue) o motivo do Impedimento, para que o
responsável pelo desenvolvimento possa corrigir a demanda. Após a correção, pôr a demanda para
<b>Teste</b> e seguir com o Fluxo a partir do teste.


### Teste

Contém todas as demandas que necessitam de teste da equipe de desenvolvimento para serem liberadas para o cliente validar.<br>
<b>Obs:</b> O responsável pela verificação do teste deverá ser, preferencialmente, algum membro
que não participou do desenvolvimento da demanda.<br>
Após a correção, o responsável pelo teste deverá mover a demanda para a prateleira <b>Feito</b>
e exibir a mensagem 'Testado em Homologação : ', informando também a url de onde ele realizou o teste <br>

<b>Eg:</b> Testado em Homologação :http://homologasiminc2.cultura.gov.br/planacomorc/planacomorc.php?modulo=apoio/unidadegestora-limite&acao=A


### Feito

Contém todas as demandas que estão prontas e testadas pela equipe de desenvolvimento aguardando o cliente validar.<br>
<b>Obs:</b> Neste Item de prateleira, o cliente fará o seu teste, onde caso a demanda não esteja de acordo
o mesmo mudará o card(Issue) para <b>Impedimento</b> e especificará o motivo do impedimento. Caso esteja tudo
de acordo, ele fechará o card(Issue).


### Closed

Contém todas as demandas que já foram aprovadas pelo cliente e estão prontas para serem publicadas.<br>
<b>Obs:</b> Verificar se o card(Issue) possui a label de <b>PUBLICADA</b>, caso tenha, significa que
a demanda já passou pela fase de publicação. Caso não contenha, deverá ser solicitado o pull request e marcar algum
membro do Time de Desenvolvimento para revisar e verificar o código antes da publicação.<br><br>
<b>Após a publicação, adicionar a label de PUBLICADA na demanda, através da ferramenta Zenhub ou pelo próprio GitHub.<b/>
