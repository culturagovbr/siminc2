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
//include_once "/var/www/simec/global/config.inc";
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

// CPF do administrador de sistemas
if(!$_SESSION['usucpf']) $_SESSION['usucpforigem'] = '00000000191';

// abre conex�o com o servidor de banco de dados
$db = new cls_banco();

$html = "
<p>Senhores Secret�rios Estaduais e Municipais de Educa��o</p>
<p>ATEN��O! Neste dia 01/12/2011, �s 18h (hor�rio de Bras�lia), encerra o prazo para solicita��o de cadastro no m�dulo Projovem Urbano e, �s 23h59, deste mesmo dia, finda o prazo para a ades�o ao Programa.</p> 
<p>Secret�rios j� cadastrados poder�o firmar a ades�o at� �s 23h59 (hor�rio de Bras�lia) do dia 01/12/2011.</p>
<p>Atenciosamente,</p>
<p align=center>Secret�ria de Educa��o Continuada, Alfabetiza��o, Diversidade e Inclus�o</p>
";


$sql = "select ent.entnome as nome_secretario, 
			   ent.entemail as email_secretario, 
			   ent2.entnome as nome_secretaria, 
			   ent2.entemail as email_secretaria, 
			   ende2.* 
		from entidade.entidade ent 
		inner join entidade.funcaoentidade fen on fen.entid=ent.entid and fen.funid=15 
		inner join entidade.funentassoc fea on fea.fueid=fen.fueid 
		inner join entidade.entidade ent2 on ent2.entid=fea.entid 
		inner join entidade.funcaoentidade fen2 on fen2.entid=ent2.entid and fen2.funid=7 
		inner join entidade.endereco ende2 on ende2.entid=ent2.entid 
		inner join projovemurbano.cargameta cma on cma.cmecodibge=ende2.muncod::numeric";


$secretarios = $db->carregar($sql);

if($secretarios[0]) {
	foreach($secretarios as $sec) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= "Programa Projovem Urbano";
		
		if($sec['email_secretario']) $mensagem->AddAddress( $sec['email_secretario'], $sec['nome_secretario'] );
		if($sec['email_secretaria']) $mensagem->AddAddress( $sec['email_secretaria'], $sec['nome_secretario'] );
		
		$mensagem->AddAddress( $_SESSION['email_sistema'] );		
			
		$mensagem->Body = $html;
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		
		echo $sec['nome_secretaria']." : ".$resp."<br>";
		
	}
}


$sql = "select ent.entnome as nome_secretario, 
			   ent.entemail as email_secretario, 
			   ent2.entnome as nome_secretaria, 
			   ent2.entemail as email_secretaria 
		from entidade.entidade ent 
		inner join entidade.funcaoentidade fen on fen.entid=ent.entid and fen.funid=25 
		inner join entidade.funentassoc fea on fea.fueid=fen.fueid 
		inner join entidade.entidade ent2 on ent2.entid=fea.entid 
		inner join entidade.funcaoentidade fen2 on fen2.entid=ent2.entid and fen2.funid=6";

$secretarios = $db->carregar($sql);

if($secretarios[0]) {
	foreach($secretarios as $sec) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= "Programa Projovem Urbano";
		
		if($sec['email_secretario']) $mensagem->AddAddress( $sec['email_secretario'], $sec['nome_secretario'] );
		if($sec['email_secretaria']) $mensagem->AddAddress( $sec['email_secretaria'], $sec['nome_secretario'] );
			
		$mensagem->Body = $html;
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		
		echo $sec['nome_secretaria']." : ".$resp."<br>";
		
	}
}

?>