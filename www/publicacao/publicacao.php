<?php

//Carrega parametros iniciais do simec
include_once "controleInicio.inc";
/**
 * Autoload de classes dos M�dulos SPO.
 * @see autoload.php
 */
require_once APPRAIZ . 'spo/autoload.php';

// carrega as fun��es espec�ficas do m�dulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

/*
 * Layout Novo
 */
//initAutoload();
////Carrega as fun��es de controle de acesso
//$_SESSION['sislayoutbootstrap'] = 'zimec';
//require_once APPRAIZ . 'includes/library/simec/view/Helper.php';
//$simec = new Simec_View_Helper();
/*
 * FIM Layout Novo
 */

// -- Export de XLS autom�tico da Listagem
Simec_Listagem::monitorarExport($_SESSION['sisdiretorio']);

//Carrega as fun��es de controle de acesso
include_once "controleAcesso.inc";
?>