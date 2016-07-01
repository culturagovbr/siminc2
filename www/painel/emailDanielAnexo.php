<?php
include_once "config.inc";
include_once "_constantes.php";

include APPRAIZ . 'includes/classes/EmailAgendado.class.inc';

$e = new EmailAgendado();
$e->setTitle("Dilig�ncia - Proinf�ncia e Quadras");
$html = 'Senhor(a) Dirigente Municipal,<br /><br />
 

O seu munic�pio cadastrou e enviou projeto(s) de infraestrutura referente(s) ao PAC 2 (Proinf�ncia e/ou constru��o de quadras cobertas) pelo Simec - M�dulo PAR 2010.<br /><br />
 

Ap�s a an�lise do FNDE, verificamos que h� proposta(s) na situa��o "em dilig�ncia".<br /><br />


Solicitamos que a equipe municipal acesse o PAR 2010, clique na obra que est� em dilig�ncia e, depois, na aba "An�lise de Engenharia" (abrir todos).<br /><br /> 


Nos itens da an�lise de engenharia em que a resposta � "n�o", deve-se ler a "Observa��o", ajustar o que � solicitado e tramitar para nova an�lise at� �s 23horas e 59 minutos do dia 09 de dezembro de 2010. Ap�s esta data o sistema ser� fechado para recebimento de resposta das dilig�ncias, resultando no indeferimento da a��o nesta etapa do PAC 2.<br /><br />
 

Caso o munic�pio tenha outro(s) projeto(s) que se encontra(m) na situa��o "Aguardando an�lise - FNDE", a equipe municipal deve acompanhar a situa��o. Se essa(s) obra(s) entrar(em) "em dilig�ncia", o mesmo procedimento deve ser seguido.<br /><br />

 
Atenciosamente,<br /><br />


Equipe T�cnica do PAR';
echo $html;
$e->setText($html);
$e->setName("Dilig�ncia - Proinf�ncia e Quadras");
$e->setEmailOrigem($_SESSION['email_sistema']);
$e->addAnexo(APPRAIZ."www/painel/emailsDaniel.txt");
$e->addAnexo(APPRAIZ."www/painel/email.txt");
$e->setEmailsDestinoPorArquivo(APPRAIZ."www/painel/emailsDanielAnexo.txt");
$e->setEmailsDestino(array("julianomeinen.souza@gmail.com"));
$e->enviarEmails();