<?php

$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configura��es */
ini_set("memory_limit", "3000M");
set_time_limit(0);

include_once "/var/www/simec/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "/includes/classes/Fnde_Webservice_Client.class.inc";

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);}


$db = new cls_banco();


$sql = "select u.usuemail, u.usunome
from seguranca.usuario u
inner join seguranca.perfilusuario pu using(usucpf)
inner join seguranca.perfil p 
      on p.pflcod = pu.pflcod
where p.pflcod in (472,473,474,264,267,383,384,385,386,470,471)";

$us = $db->carregar($sql);

$us[] = array("usuemail"=>$_SESSION['email_sistema'],"usunome"=>SIGLA_SISTEMA);


require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

if($us[0]) :
	foreach($us as $u) :
	
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= "Minist�rio da Educa��o - MEC";
		$mensagem->From 		= "no-reply@mec.gov.br";
		$mensagem->AddAddress( $u['usuemail'], $u['usunome'] );
		$mensagem->Subject = "Convite 32a. videoconfer�ncia do Programa Mais Educa��o";
		$mensagem->Body = "<p><b>32a.Videoconfer�ncia do Programa Mais Educa��o</b></p>
		<p>Convidamos para assistirem a 32a.Videoconfer�ncia do Programa Mais Educa��o, cujo tema ser� \"as experi�ncias e especificidades das escolas do campo no Programa Mais Educa��o na constru��o da educa��o integral no Brasil\". A transmiss�o ser� via internet pelo endere�o http://portal.mec.gov.br/seb/transmissao</p>  
		<p>Quando? 02 de outubro de 2012, ter�a-feira, das 14:30h �s 17h</p> 
		<p>Onde? http://portal.mec.gov.br/seb/transmissao</p>  
		<p>Quem?<br>
		- Jaqueline Moll (Diretora de Curr�culo e Educa��o Integral do Minist�rio da Educa��o)<br>
		- Maca� Maria Evaristo dos Santos (Diretora de Pol�ticas de Educa��o no Campo, Ind�gena e para as Rela��es �tnico-Raciais do Minist�rio da Educa��o)<br>
		- Danilo de Melo Souza (Secret�rio de Educa��o e Cultura do Estado do Tocantins)<br>
		- Leandro Fialho (Coordenador Geral de Educa��o Integral do Minist�rio da Educa��o)</p> 
		<p>D�vidas? ".$_SESSION['email_sistema']."</p>";
		
		$mensagem->IsHTML( true );
		$mensagem->Send();
		
	endforeach;
endif;

?>