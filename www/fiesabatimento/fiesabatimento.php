<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

// carrega as fun��es espec�ficas do m�dulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';
require_once APPRAIZ . 'includes/workflow.php';

//Carrega as fun��es de controle de acesso
//http://simec-local/fiesabatimento/fiesabatimento.php?modulo=inicio&acao=C
include_once "controleAcesso.inc";
?>