<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

// carrega as fun��es espec�ficas do m�dulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

initAutoload();

//Carrega as fun��es de controle de acesso
$_SESSION['sislayoutbootstrap'] = 'zimec';

require_once APPRAIZ . 'includes/library/simec/view/Helper.php';
$simec = new Simec_View_Helper();

//Carrega as fun��es de controle de acesso
include_once "controleAcesso.inc";
?>