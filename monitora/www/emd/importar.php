<pre><?php
//die("<h1>Cuidado ao executar este script</h1>");
//exit;

class CodigoAcaoNaoEncontradoException extends Exception {}

define("TESTE",false);
set_time_limit(0);

$data = "2006-05-16";
$exercicio = '2006';
$erroacaid=0;
$arquivo = "emendas2006.csv";



$servidor_bd = '10.210.3.15';
$porta_bd = '5432';
$nome_bd = 'simec';
$usuario_db = 'phpsimec';
$senha_bd = 'pgphpsimecspo';

$pgconn = pg_connect("host=".$servidor_bd." port=".$porta_bd." dbname=".$nome_bd."  user=".$usuario_db." password=".$senha_bd);
pg_set_client_encoding($pgconn,'LATIN2');
//pg_query($pgconn, 'begin; ');



$fp = fopen($arquivo, "r") or die("erro ao abrir $arquivo");
$campos = fgetcsv($fp, 1000, ";", "\"");
//var_dump($campos);
$linha = 1;

$sqlSelAcao = "SELECT acaid FROM planejamento.acao WHERE unicod = '%s' AND prgcod = '%s' AND acacod = '%s' AND loccod = '%s' AND prgano = '$exercicio'";
$sqlUpdAcao = "UPDATE planejamento.acao SET acasnemenda = 't' WHERE acaid = '%s' AND prgano = '$exercicio'";
$sqlUpdAcaoFuncao = "UPDATE planejamento.acao SET funcod = '%s' WHERE acaid = '%s' AND funcod IS NULL OR funcod = ''";
$sqlUpdAcaoSubFuncao = "UPDATE planejamento.acao SET sfucod = '%s' WHERE acaid = '%s' AND sfucod IS NULL OR sfucod = ''";
$sqlSelEmenda = "SELECT emdid FROM emenda.emenda WHERE emdcod = '%s'";
$sqlSelEmendaOID = "SELECT emdid FROM emenda.emenda WHERE oid = '%s'";
$sqlInsEmenda = "INSERT INTO emenda.emenda (
						emdcod, 
						acaid, 
						emdcodautor,
						emdcodtipoautor,
						emdufautor,
						emdnomeautor,
						emdsglpartidoautor,
						emdsaldo,
						prgano
					) VALUES (
						'%s', 
						'%s', 
						'%s', 
						'%s', 
						'%s', 
						'%s', 
						'%s', 
						'%s',
						'$exercicio'
				)";
$sqlSelExecucao = "SELECT * FROM emenda.execucao WHERE emdid = '%s' AND exedata = '%s'";
$sqlInsExecucao = "INSERT INTO emenda.execucao (
						exedata,
						emdid,
						exedotainicial,
						exeautorizado,
						exeempenhado,
						exeliquidado,
						exepago
					) VALUES (
						'%s', 
						'%s', 
						'%s', 
						'%s', 
						'%s', 
						'%s', 
						'%s'
				)";
$sqlDelExecucao = "DELETE FROM emenda.execucao WHERE emdid = '%s' AND exedata = '%s'";
$acasnemenda = array();
while($row = fgetcsv($fp, 1000, ";", "\"")) {
try {
//	var_dump($row);//exit;
	$funcprog = explode(".", $row[12]);
	if(@count($funcprog)<5) {
		throw new Exception("QUANTIDADE DE CAMPOS ERRADA NA FUNCIONAL PROGRAMATICA (" . count($funcprog) . "): " . $row[10]);
	}

	$query = sprintf($sqlSelAcao, $row[10], $funcprog[2], $funcprog[3], $funcprog[4]);
	if(TESTE)
		echo "$query\n";
	$rs = @pg_query($query) or die("ERRO CONSULTA: $query");
	$acaid = @pg_result($rs, 0, "acaid");
	if(!(bool)$acaid) {
		throw new CodigoAcaoNaoEncontradoException("CÓDIGO DA AÇÃO NÃO ENCONTRADO: $query");
	}

	$query = sprintf($sqlUpdAcao, $acaid);
	if(TESTE)
		echo "$query\n";
	$rs = pg_query($query);
	if(pg_affected_rows($rs)>0) {
		echo "ACAO $acaid ATUALIZADA acasnemenda='t' ACAID=(" . $acaid . ")\n";
		$acasnemenda[$acaid] = 't';
	}
	
	$query = vsprintf($sqlUpdAcaoFuncao, array($funcprog[0]	// código função
												,$acaid));
	if(TESTE)
		echo "$query\n";
	$rs = pg_query($query);
	if(pg_affected_rows($rs)>0)
		echo "ACAO $acaid ATUALIZADA FUNCOD=(" . $funcprog[0] . ")\n";

	$query = vsprintf($sqlUpdAcaoSubFuncao, array($funcprog[1]	// código subfunção
												,$acaid));
	if(TESTE)
		echo "$query\n";
	$rs = pg_query($query);
	if(pg_affected_rows($rs)>0)
		echo "ACAO $acaid ATUALIZADA SFUCOD=(" . $funcprog[1] . ")\n";
		
	$query = sprintf($sqlSelEmenda, $row[7]);
	if(TESTE)
		echo "$query\n";
	$rs = pg_query($query);
	if(pg_num_rows($rs)>0) {
		$emdid = pg_result($rs, 0, "emdid");
		echo "AÇÃO ($acaid) JÁ EXISTE\n";
	}
	else {
		$dadosEmenda = array($row[9]	// cod_emenda
			,$acaid
			,'9999'					// cod_autor
			,$row[2]					// cod_tipo_autor
			,$row[4]					// uf_autor
			,$row[8]					// nom_autor
			,$row[5]					// sgl_partido
			,$row[16]					// saldo
		);
		
		$query = vsprintf($sqlInsEmenda, $dadosEmenda);
		if(TESTE)
			echo "$query\n";
		$rs = pg_query($query);
		if(!$rs) {
			throw new Exception("Erro ao inserir emenda: $query\n" . pg_errormessage($pgconn));
		}
		
		$oid = pg_last_oid($rs);
		$query = sprintf($sqlSelEmendaOID, $oid);
		if(TESTE)
			echo "$query\n";

		$rs = pg_query($query);
		
		$emdid = pg_result($rs, 0, "emdid");
		if(!(bool)$emdid) {
			throw new Exception("Não foi possível recuperar a emenda inserida OID: $oid\nCONSULTA: $query\n" . pg_errormessage($pgconn));
		}
	}
	
	// execucao
	$query = sprintf($sqlSelExecucao, $emdid, $data);
	if(TESTE)
		echo "$query\n";

	$rs = pg_query($query);
	if(pg_num_rows($rs)>0) {
		$query = sprintf($sqlDelExecucao, $emdid, $data);
		if(TESTE)
			echo "$query\n";
		if(!pg_query($query)) {
			throw new Exception("ERRO AO EXCLUIR EXECUÇÃO PARA A EMENDA: $emdid ($data)\n$query");
		}
	}
	
/*	$dadosExecução = array($data
		,$emdid
		,number_format((float)$row[14], '2', '.', '')
		,number_format((float)$row[15], '2', '.', '')
		,number_format((float)$row[16], '2', '.', '')
		,number_format((float)$row[17], '2', '.', '')
		,number_format((float)$row[18], '2', '.', '')
	);*/
	$dadosExecução = array($data
		,$emdid
		,number_format((float)0, '2', '.', '')
		,number_format((float)0, '2', '.', '')
		,number_format((float)0, '2', '.', '')
		,number_format((float)0, '2', '.', '')
		,number_format((float)0, '2', '.', '')
	);	
	$query = vsprintf($sqlInsExecucao, $dadosExecução);
	if(TESTE)
		echo "$query\n";

	if(!pg_query($query)) {
		throw new Exception("ERRO AO INSERIR EXECUÇÃO PARA A EMENDA: $emdid ($data)\n$query");		
	}

	printf("LINHA: %100d OK\n", $linha++);
	if(TESTE)
		echo "\n";
	flush();
	}
	catch(CodigoAcaoNaoEncontradoException $e) {
		printf("\nERRO! %s\n\n", $e->getMessage());
		$erroacaid++;
		continue;
	}
	catch(Exception $e) {
		var_dump($dados);
		var_dump($row);
		var_dump($e);
		die();
	}
}

if(TESTE)
	pg_query($pgconn, 'rollback;');
else
	pg_query($pgconn, 'commit;');
	
pg_query($pgconn, "UPDATE planejamento.acao SET acasnemenda = 't' WHERE acaid IN (SELECT DISTINCT acaid FROM emenda.emenda)");
var_dump("UPDATE planejamento.acao SET acasnemenda = 't' WHERE acaid IN (SELECT DISTINCT acaid FROM dbemd.emenda WHERE prgcod = '$exercicio')");

pg_close($pgconn);
?>
</pre>