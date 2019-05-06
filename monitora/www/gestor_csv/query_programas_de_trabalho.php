<?php

header('Content-Type: text/html; charset=utf-8');

// carrega as funções gerais
include_once "config.inc";
include "verificasistema.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

// exit($_SESSION['baselogin']);

// echo '<pre>';
// print_r($_POST);
// echo '</pre>';
// exit;

$dados = array();
foreach ($_POST as $key => $value) {
	foreach ($value as $key2 => $value2) {
		$dados[ $key2 ][ $key ] = $value2;
	}
}

// echo '<pre>';
// print_r($dados);
// echo '</pre>';
// exit;

$resultado = array();
foreach ($dados as $key => $value) {
	$sql = "
		select acaid from
			planejamento.acao ac
		where 
			ac.funcod = '".$value['funcao']."'
			and ac.sfucod = '".$value['subfuncao']."'
			and ac.prgcod = '".$value['programa']."'
			and ac.acacod = '".$value['acao']."'
			and ac.loccod = '".$value['localizador']."'
			and ac.prgano='2013'; ";
	// exit($sql);
	// echo $sql.'<br/>';
	$resultado[] = $db->carregar( $sql );
}
// exit;

// echo '<pre>';
// print_r($resultado);
// echo '</pre>';
// exit;

$num_sem_acaid = 0;
echo 'Query resultante:<br/><textarea style=\'width:900px;height:600px;\'>';
foreach ($resultado as $key => $value) {
	if( !empty($value) && isset($value[0]['acaid']) ){
		// echo '
		// 	insert into monitora.ptres ( 
		// 		unicod , 
		// 		irpcod , 
		// 		ptrdotacao, 
		// 		acaid ,
		// 		funcod , 
		// 		sfucod , 
		// 		acacod , 
		// 		loccod 
		// 	) values ( 
		// 		\'26298\' , 
		// 		\'2\' , 
		// 		\''.$dados[ $key ]['dotacao_inicial'].'\', 
		// 		\''.$value.'\',  
		// 		\''.$dados[ $key ]['funcao'].'\',
		// 		\''.$dados[ $key ]['subfuncao'].'\',
		// 		\''.$dados[ $key ]['acao'].'\',
		// 		\''.$dados[ $key ]['localizador'].'\'
		// 	);
		// ';
		echo ' insert into monitora.ptres ( unicod , irpcod , ptrdotacao, acaid , funcod , sfucod , acacod , loccod ) values ( \'26298\' , \'2\' , \''.$dados[ $key ]['dotacao_inicial'].'\', \''.$value[0]['acaid'].'\',  \''.$dados[ $key ]['funcao'].'\',\''.$dados[ $key ]['subfuncao'].'\',\''.$dados[ $key ]['acao'].'\',\''.$dados[ $key ]['localizador'].'\');
		';
	}else{
		$num_sem_acaid++;
	}
}
echo '</textarea>';
echo '<br/>'.$num_sem_acaid.' sem acaid. ';

