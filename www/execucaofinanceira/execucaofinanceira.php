<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

// carrega as fun��es espec�ficas do m�dulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

//Carrega as fun��es de controle de acesso
include_once "controleAcesso.inc";

function __autoload( $classe ){
	if( file_exists( APPRAIZ . "execucaofinanceira/classes/{$classe}.class.inc" ) ){
		include_once( APPRAIZ . "execucaofinanceira/classes/{$classe}.class.inc");
	} elseif( file_exists( APPRAIZ . "includes/classes/{$classe}.class.inc" ) ){
		include_once( APPRAIZ . "includes/classes/{$classe}.class.inc");
	}
}

?>