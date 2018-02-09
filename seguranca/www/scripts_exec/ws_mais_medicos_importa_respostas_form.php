<?php

// Iniciamos o "contador"
list($usec, $sec) = explode(' ', microtime());
$script_start = (float) $sec + (float) $usec;

date_default_timezone_set ('America/Sao_Paulo');

set_time_limit(100000);
ini_set("memory_limit", "10000M");

//strpos( $_SERVER['SERVER_NAME'], 'simec-d' ) !== false

// if(!$_SESSION['baselogin'] && in_array($_SERVER['HTTP_HOST'], array('simec-local','simec-d.mec.gov.br')))
$_SESSION['baselogin'] = "simec_espelho_producao";

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );
require_once BASE_PATH_SIMEC . "/global/config.inc";

if($_GET['exec']){	
	shell_exec("php ".APPRAIZ."seguranca/www/scripts_exec/ws_mais_medicos_importa_respostas_form.php &");
	echo 'Execu��o via client.';
	exit;
}

include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . 'maismedicos/classes/Ws_Tutor.class.inc';
include_once APPRAIZ . 'maismedicos/classes/Ws_Profissionais.class.inc';

if(!$_SESSION['usucpf']){
	$_SESSION['usucpforigem'] = '00000000191';
	$_SESSION['usucpf'] = '00000000191';
}

// abre conex�o com o servidor de banco de dados
$db = new cls_banco();

$ws_tutor = new Ws_Tutor();

if($_GET['formulario']){
	
	$msg  = $ws_tutor->atualizaRespostasFormularioMaisMedicos();	
	$assunto = "Atualiza��o dos Formularios do Programa Mais M�dicos pelo WS";
	$conteudo = 'Atualiza��o dos Formularios do Programa Mais M�dicos pelo WS.<br/></br>'.$msg;
	enviarEmailRespostasFormulario($assunto, $conteudo);
	
}elseif($_GET['resposta']){
	
	$msg = $ws_tutor->atualizaRespostasFormularioItensMaisMedicos();	
	$assunto = "Atualiza��o das Respostas dos Formularios do Programa Mais M�dicos pelo WS.";
	$conteudo = 'Atualiza��o das Respostas dos Formulario do Programa Mais M�dicos pelo WS.<br/></br>'.$msg;
	enviarEmailRespostasFormulario($assunto, $conteudo);
	
}elseif($_GET['profissional']){
	
	$msgP1 = $ws_tutor->atualizaProfissionaisMaisMedicos();
	$assunto = "Atualiza��o do Cadastro de Profissionais do Mais M�dicos";
	$conteudo = 'Atualiza��o do Cadastro de Profissionais do Mais M�dicos pelo WS.<br/></br>'.$msgP1;
	enviarEmailRespostasFormulario($assunto, $conteudo);

	$msgP2 = $ws_tutor->atualizarDadosProfissionaisMaisMedicos();
	$assunto = "Vincula��o das Universidades do Cadastro de Profissionais do Mais M�dicos";
	$conteudo = 'Vincula��o das Universidades do Cadastro de Profissionais do Mais M�dicos pelo WS.<br/></br>'.$msgP2;
	enviarEmailRespostasFormulario($assunto, $conteudo);
	
}elseif($_GET['atualiza']){
	
	$msg = $ws_tutor->atualizaCamposItensParaFormulario();	
	$assunto = "Atualiza��o das Informa��es dos M�dicos nos Formularios do Programa Mais M�dicos pelo WS.";
	$conteudo = 'Atualiza��o das Informa��es dos M�dicos nas Respotas Formulario do Programa Mais M�dicos pelo WS.<br/></br>'.$msg;
	enviarEmailRespostasFormulario($assunto, $conteudo);
	
}else{
	
	// Atualiza os formularios
	$msg1 = $ws_tutor->atualizaRespostasFormularioMaisMedicos();	
	$assunto = "Atualiza��o dos Formularios do Programa Mais M�dicos pelo WS.";
	$conteudo = 'Atualiza��o das Respotas Formulario do Programa Mais M�dicos pelo WS.<br/></br>'.$msg1;
	enviarEmailRespostasFormulario($assunto, $conteudo);
	
	$msg2 = $ws_tutor->atualizaRespostasFormularioItensMaisMedicos();	
	$assunto = "Atualiza��o dos Itens dos Formularios do Programa Mais M�dicos pelo WS.";
	$conteudo = 'Atualiza��o dos Itens das Respotas Formulario do Programa Mais M�dicos pelo WS.<br/></br>'.$msg2;
	enviarEmailRespostasFormulario($assunto, $conteudo);
	
	$msg3 = $ws_tutor->atualizaCamposItensParaFormulario();	
	$assunto = "Atualiza��o das Informa��es dos M�dicos nos Formularios do Programa Mais M�dicos pelo WS.";
	$conteudo = 'Atualiza��o das Informa��es dos M�dicos nas Respotas Formulario do Programa Mais M�dicos pelo WS.<br/></br>'.$msg3;
	enviarEmailRespostasFormulario($assunto, $conteudo);
}

// Terminamos o "contador" e exibimos
list($usec, $sec) = explode(' ', microtime());
$script_end = (float) $sec + (float) $usec;
$elapsed_time = round($script_end - $script_start, 5);
echo 'Tempo decorrido: ', $elapsed_time, ' secs. Mem�ria usada: ', round(((memory_get_peak_usage(true) / 1024) / 1024), 2), 'Mb';

function enviarEmailRespostasFormulario($assunto, $conteudo)
{
	global $db;
	
	/*******************************
	 * PREPARA PARA ENVIA OS E-MAILS
	 */	
	require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
	$mensagem = new PHPMailer();
	$mensagem->persistencia = $db;
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName		= "WS Mais M�dicos";
	$mensagem->From 		= $_SESSION['email_sistema'];
	$mensagem->AddAddress($_SESSION['email_sistema'], SIGLA_SISTEMA);
	$mensagem->Subject = $assunto;
	$mensagem->Body = $conteudo;
	$mensagem->IsHTML( true );
	
	if($_SERVER['HTTP_HOST']!='simec-local')
		$mensagem->Send();
	
}

?>