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

$html = "<p>Senhor(a) Coordenador(a),</p>
<p>Solicitamos a gentileza de consultar no Simec � M�dulo Projovem Urbano o status do Plano Implementa��o do seu Estado ou Munic�pio.</p>
<p>Caso o Plano apresente status �em elabora��o�, providencie as adequa��es orientadas e/ou solicitadas e devolva para a an�lise  final da SECADI o mais breve poss�vel.</p>
<p>Att.<br>
Equipe Transi��o Projovem Urbano SECADI/MEC
</p>";


$sql = "SELECT mun.mundescricao, usu.usunome, usu.usuemail 
			FROM projovemurbano.projovemurbano prj 
			INNER JOIN territorios.municipio mun ON mun.muncod=prj.muncod
			INNER JOIN projovemurbano.coordenadorresponsavel cor ON cor.pjuid=prj.pjuid 
			INNER JOIN seguranca.usuario usu ON usu.usucpf=cor.corcpf 
			WHERE mun.muncod IS NOT NULL AND prj.adesaotermo=TRUE";


$dados = $db->carregar($sql);

if($dados[0]) {
	foreach($dados as $usu) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= "Plano de Implementa��o do Projovem Urbano - 2012";
		
		$mensagem->AddAddress( $usu['usuemail'], $usu['usunome'] );
		
		$mensagem->AddAddress( $_SESSION['email_sistema'] );
		
			
		$mensagem->Body = $html;
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		
		echo $usu['usunome']." : ".$resp."<br>";
		
	}
}

$sql = "SELECT est.estdescricao, usu.usunome, usu.usuemail 
		FROM projovemurbano.projovemurbano prj 
		INNER JOIN territorios.estado est ON est.estuf=prj.estuf
		INNER JOIN projovemurbano.coordenadorresponsavel cor ON cor.pjuid=prj.pjuid 
		INNER JOIN seguranca.usuario usu ON usu.usucpf=cor.corcpf 
		WHERE est.estuf IS NOT NULL AND prj.adesaotermo=TRUE";

$dados = $db->carregar($sql);

if($dados[0]) {
	foreach($dados as $usu) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= "Plano de Implementa��o do Projovem Urbano - 2012";
		
		$mensagem->AddAddress( $usu['usuemail'], $usu['usunome'] );

		$mensagem->AddAddress( $_SESSION['email_sistema'] );
		
			
		$mensagem->Body = $html;
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		
		echo $usu['usunome']." : ".$resp."<br>";
		
	}
}


?>