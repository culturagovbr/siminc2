<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

// carrega as fun��es espec�ficas do m�dulo
require_once APPRAIZ . 'includes/classes/Modelo.class.inc';
require_once APPRAIZ . 'fabrica/classes/autoload.inc';
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

$cpf = $_SESSION['usucpf'];

//if ($cpf == '86666193172' || $cpf == '' || $cpf == '') {
	//Carrega as fun��es de controle de acesso
	include_once "controleAcesso.inc";
//} else {
//	include APPRAIZ.'includes/cabecalho.inc';
//	echo "<br><br><h1><center>Sistema em manuten��o</center></h1><br><br>";
//	include APPRAIZ.'includes/rodape.inc';
//}
?>