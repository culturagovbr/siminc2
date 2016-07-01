<?php
$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configura��es */
ini_set("memory_limit", "2048M");
set_time_limit(30000);

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/workflow.php";

session_start();
 
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '';
$_SESSION['usucpf'] = '';

$db = new cls_banco();

/*
$sql = "DELETE FROM  workflow.documento WHERE docid IN (SELECT DISTINCT docid FROM par.subacao WHERE docid is not null LIMIT 50 );";
if($db->executar($sql)){
	$sql = "UPDATE par.subacao SET docid = null;";
	$db->executar($sql);
	echo "atualiza docid suba��o para null";
	$db->commit();
}else{
	echo "Erro ao excluir documento.";
	$db->rollback();
}
*/

//////////////////////////// Em Elabora��o

$erro = false;
$sql = "SELECT distinct s.sbaid -- , s.docid --,  iu.inuid, iu.docid, ed.*
		FROM
			par.subacao s
		INNER JOIN par.acao 	 a  ON a.aciid  = s.aciid AND a.acistatus = 'A'
		INNER JOIN par.pontuacao p  ON p.ptoid  = a.ptoid AND p.ptostatus = 'A'
		INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
		INNER JOIN workflow.documento  d ON d.docid = iu.docid
		INNER JOIN workflow.estadodocumento ed on ed.esdid = d.esdid
		WHERE ed.esdid = 314 -- Em elabora��o a entidade
		-- and iu.inuid = 641
		";
$lista = $db->carregar($sql);

if($lista[0]) {
	foreach($lista as $l) {
	//	 Em elabora��o
			$sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
					VALUES (62, 451, 'Em Elabora��o', now()) returning docid";

			$docid = $db->carregar($sql);
			if($docid[0]['docid']){
				echo "Inserindo documento para suba��o: ".$l['sbaid']." <br>";
				$sql = "UPDATE par.subacao SET docid = ".$docid[0]['docid']." WHERE sbaid in (".$l['sbaid'].")";
				if(!$db->executar($sql)){
					$db->rollback();
					$erro = true;
				}
			}else{
				$db->rollback();
				$erro = true;
			}
			echo "Atualizando docid na tabela de suba��o a suba��o:".$l['sbaid']." <br>";
		}
}else{
	echo "Nada foi Executado 1... <br>";	
}
if($erro == false){
	$db->commit();
	echo "Commit <br>";
}


//////////////////////////// Em An�lise

$erro = false;
$sql = "SELECT distinct s.sbaid -- , s.docid --,  iu.inuid, iu.docid, ed.*
		FROM
			par.subacao s
		INNER JOIN par.acao 	 a  ON a.aciid  = s.aciid AND a.acistatus = 'A'
		INNER JOIN par.pontuacao p  ON p.ptoid  = a.ptoid AND p.ptostatus = 'A'
		INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
		INNER JOIN workflow.documento  d ON d.docid = iu.docid
		INNER JOIN workflow.estadodocumento ed on ed.esdid = d.esdid
		WHERE ed.esdid = 315 -- Em elabora��o a entidade
		-- and iu.inuid = 641
		";
$lista = $db->carregar($sql);

if($lista[0]) {
	foreach($lista as $l) {
	
			$sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
					VALUES (62, 452, 'Em An�lise', now()) returning docid";

			$docid = $db->carregar($sql);
			if($docid[0]['docid']){
				echo "Inserindo documento para suba��o: ".$l['sbaid']." <br>";
				$sql = "UPDATE par.subacao SET docid = ".$docid[0]['docid']." WHERE sbaid in (".$l['sbaid'].")";
				if(!$db->executar($sql)){
					$db->rollback();
					$erro = true;
				}
			}else{
				$db->rollback();
				$erro = true;
			}
			echo "Atualizando docid na tabela de suba��o a suba��o:".$l['sbaid']." <br>";
		}
}else{
	echo "Nada foi Executado 2...";	
}
if($erro == false){
	$db->commit();
	echo "Commit";
}

?>