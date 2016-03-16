<?php

// carrega as fun��es gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// abre conex�o com o servidor de banco de dados
$db = new cls_banco();

// carrega as a��es
$sql = sprintf(
	"select acaid, prgcod, acacod, unicod, loccod, acadsc
	from monitora.acao
	where prgcod like '%s%%' and acacod like '%s%%' and unicod like '%s%%' and loccod like '%s%%' and acasnrap = false and prgano = '%s'
	order by prgcod, acacod, unicod, loccod, acadsc",
	$_REQUEST['programa'],
	$_REQUEST['acao'],
	$_REQUEST['unidade'],
	$_REQUEST['localizador'],
	$_SESSION['exercicio']
);
$acoes = $db->carregar( $sql );
$acoes = $acoes ? $acoes : array();
foreach( $acoes as &$acao ){
	$acao = array_map( "utf8_encode", $acao );
}

// retorna as a��es
header( "Content-Type: text/plain; charset=utf-8" );
echo json_encode( $acoes );

?>