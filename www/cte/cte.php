<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

// carrega as fun��es do m�dulo
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . 'includes/workflow.php';

require_once(APPRAIZ.'includes/classes/depuradorCodigo.class.inc');

// carrega as fun��es do m�dulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

$dbug 		= new DepuradorCodigo(false);

//Carrega as fun��es de controle de acesso
include_once "controleAcesso.inc";

$dbugDB = new DepuradorCodigo(true, $db);

?>