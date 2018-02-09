<?php

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);} 

date_default_timezone_set ('America/Sao_Paulo');

$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configura��es */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configura��es */

// carrega as fun��es gerais
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conex�o com o servidor de banco de dados
$db = new cls_banco();


$sql = "SELECT usunome, usuemail  
		FROM
		seguranca.usuario usu
		INNER JOIN
		seguranca.usuario_sistema ususis ON ususis.usucpf = usu.usucpf
		WHERE
		ususis.sisid = 98 AND 
		ususis.suscod = 'A'";

$usuarios = $db->carregar($sql);

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';


if($usuarios[0]) {
	foreach($usuarios as $usu) {
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - PDE Interativo";
		
		$mensagem->AddAddress( $usu['usuemail'], $usu['usunome'] );
		
			
		$mensagem->Body = "<p>O Minist�rio da Educa��o informa que no pr�ximo dia 04 de abril, �s 10h, ser� realizada uma videoconfer�ncia sobre o PDE Interativo e o Plano de Forma��o Continuada, conectando alguns audit�rios nas capitais dos estados. O evento ser� coordenado pelas equipe do PDE Escola e da Forma��o Continuada e transmitido pela internet. Para assistir, acesse o endere�o http://portal.mec.gov.br/transmissao. Durante a transmiss�o, quem desejar enviar perguntas poder� utilizar o e-mail do PDE Escola, a saber: ".$_SESSION['email_sistema']. ".</p>
						   <p>Equipe do PDE Escola</p>";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		
	}
}


$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= SIGLA_SISTEMA. " - PDEInterativo";
$mensagem->From 		= "noreply@mec.gov.br";
$mensagem->AddAddress( $_SESSION['email_sistema'], SIGLA_SISTEMA );
$mensagem->Subject = SIGLA_SISTEMA. " - PDEInterativo";
$mensagem->Body = "Todos os e-mails dos diretores pendentes foram enviados com sucesso";
$mensagem->IsHTML( true );
$mensagem->Send();

?>