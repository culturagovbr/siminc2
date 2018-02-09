<?php

$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configura��es */
ini_set("memory_limit", "3000M");
set_time_limit(0);

//include_once "/var/www/simec/global/config.inc";
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);}


$db = new cls_banco();


$sql = "select * from (
SELECT
COALESCE((SELECT apassituacao FROM sispacto.atividadepacto at WHERE at.picid=p.picid AND at.suaid=2),0) as porcentagem,
COALESCE((SELECT iusnome FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=826),'Coordenador Local n�o cadastrado') as coordenadorlocal,
COALESCE((SELECT iusemailprincipal FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=826),'') as email

FROM sispacto.pactoidadecerta p 
		INNER JOIN territorios.municipio m ON m.muncod = p.muncod 
		LEFT JOIN workflow.documento d ON d.docid = p.docid  
		LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid
		WHERE e.esdid=561) foo where foo.porcentagem!=0 and foo.email!=''";

$us = $db->carregar($sql);

$us[] = array("email"=>"alexandredourado03@gmail.com","coordenadorlocal"=>"Alexandre Dourado");


require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

if($us[0]) :
	foreach($us as $u) :
	
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= "Minist�rio da Educa��o - SIMEC - SISPACTO";
		$mensagem->From 		= "no-reply@mec.gov.br";
		$mensagem->AddAddress( $u['email'], $u['coordenadorlocal'] );
		$mensagem->Subject = "Acesso ao SIMEC - SISPACTO";
		$mensagem->Body = "<p>Prezado(a) ".$u['coordenadorlocal'].",</p>
						   <p>Identificamos que voc� iniciou as atividades a serem realizadas no sistema SISPACTO - SIMEC, por�m n�o enviou para an�lise da IES.</p>
						   <p>Para efetuar o envio para IES, voc� deve acessar o SIMEC(http://simec.mec.gov.br), ir no m�dulo do SISPACTO, acessar seu munic�pio e clicar na aba \"Resumo Orientadores de Estudo\".</p>
						   <p>Nesta tela existe um quadro informando o estado atual e o bot�o para Enviar para an�lise do IES. Verifique se todas as condi��es foram atendidas e clique no bot�o. Feito isso, basta aguardar a an�lise da IES.</p>
						   <p>Caso ja tenha enviado para IES, desconsidere esse e-mail.</p>
						   <p>Atenciosamente,<br>Equipe SISPACTO</p>";
		
		$mensagem->IsHTML( true );
		$x = $mensagem->Send();
		echo $x."<br>";
		
	endforeach;
endif;

echo "FIM"

?>