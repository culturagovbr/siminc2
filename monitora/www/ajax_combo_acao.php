<?php

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

// carrega as ações
$sql = sprintf(
	"select acaid, prgcod, acacod, unicod, loccod, acadsc
	from planejamento.acao
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

// retorna as ações
header( "Content-Type: text/plain; charset=utf-8" );
echo simec_json_encode( $acoes );

?>