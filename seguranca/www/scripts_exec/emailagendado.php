<?php
/* configura��es */
ini_set("memory_limit", "3000M");
set_time_limit(0);
/* FIM configura��es */

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

// carrega as fun��es gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . 'www/painel/_constantes.php';
include APPRAIZ . 'includes/classes/EmailAgendado.class.inc';

$e = new EmailAgendado();
$e->setTitle("SCDP - ALERTA - SOLICITA�AO DE DI�RIAS E PASSAGENS");

$html = "<span style=\"font-weight:bold\" >Prezados(as) Senhores(as),</span><br /><br />
<center>
<div style=\"font-size:22px;font-weight:bold\" >
Hoje � o �ltimo dia para solicitar a emiss�o de passagens e di�rias para o dia ".date("d/m",mktime(0, 0, 0, date("m")  , date("d")+10, date("Y"))).".<br />
Fora deste prazo, somente ser� autorizada a solicita��o mediante justificativa escrita encaminhada por meio de Memorando ou Of�cio a <br />
Chefia de Gabinete da Secretaria Executiva.<br />
Informamos tamb�m que n�o ser� autorizada a emiss�o com pend�ncias de presta��o de contas.<br /><br />
</div>
<span style=\"font-weight:bold\" >
Secretaria Executiva<br />
MEC<br />
</span></center>";
echo $html;
$e->setText($html);
$e->setEmailOrigem("no-reply@mec.gov.br");
//$e->setEmailToReply($_SESSION['email_sistema']);
$e->setName("SCDP");
//$e->setEmailsDestinoPorArquivo(APPRAIZ . 'www/painel/email.txt');
//$e->setEmailsDestino(array($_SESSION['email_sistema']));
$e->setEmailsDestino(array($_SESSION['email_sistema']));
$e->enviarEmails();