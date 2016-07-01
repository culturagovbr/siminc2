<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";

// carrega as fun��es espec�ficas do m�dulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

// carrega libs necessarias
include_once APPSAP . 'libs/Math.class.inc';

// carregando classes automaticamente
function __autoload($classe) {
	require_once(APPSAP . 'classes/' . $classe . '.class.inc');
}

//Carrega as fun��es de controle de acesso
include_once "controleAcesso.inc";

?>