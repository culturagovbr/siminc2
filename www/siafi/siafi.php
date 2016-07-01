<?php

/*
 * Banco de dados SIAFI em Produ��o
 */
$configDbSiafi = new stdClass();
$configDbSiafi->host = '';
$configDbSiafi->port = 5432;
$configDbSiafi->dbname = '';
$configDbSiafi->user = '';
$configDbSiafi->password = '';
$configDbSiafi->clientEncoding = 'LATIN5';

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

// -- Export de XLS autom�tico da Listagem
Simec_Listagem::monitorarExport($_SESSION['sisdiretorio']);

//Carrega as fun��es de controle de acesso
include_once "controleAcesso.inc";

