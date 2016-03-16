<?php

function carregarMenuProfeinep() {

	$menu[] = array("id" => 0, "descricao" => "Anexos de Processos", "link" => "/profeinep/profeinep.php?modulo=sistema/public_arquivo/profeinep_arquivo&acao=A"); 
	return $menu;
	
}

function montaListaArquivosProfeinep()
{
	global $db;
	
	$arrWhere[] = "a.arqid not in(select arqid from public.arquivo_recuperado)";
	$arrWhere[] = "a.arqid/1000 between 647 and 725";
	
	$cabecalho = array();
	
	if($db->testa_superuser()) {
		$cabecalho = array("CPF", "Nome");
		$arrCampos[] = "a.usucpf";
		$arrCampos[] = "u.usunome";
	}else{
		$arrWhere[] = "a.usucpf = '{$_SESSION['usucpf']}'";
	}
			
	monta_titulo( "Recupera��o dos arquivos anexos aos processos", "<span style=\"color:#0000FF\" >Depois de selecionar os arquivos, clique no bot�o <b>SALVAR</b> no final desta p�gina.</span>");
			
	array_push($cabecalho, "N� do Processo SIDOC", "Interessado","Prioridade","Coordena��o", "Situa��o PROFE/INEP","ID do Arquivo", "Nome do arquivo", "Descri��o do Arquivo", "Tamanho (bytes)", "Data da inclus�o (arquivo)", "");
			
	$sql = "SELECT distinct
				".($arrCampos ? implode(",",$arrCampos)."," : "" )." 
				prc.prcnumsidoc,
				prc.prcnomeinteressado,
				tpr.tipdsc as prioridade,
				coo.coodsc,
				esd.esddsc,
				a.arqid,
				a.arqnome||'.'||a.arqextensao,
				a.arqdescricao,
				a.arqtamanho,
				to_char(a.arqdata,'dd/mm/YYYY')||' '||a.arqhora as arqdata,
				'<span style=\"white-space: nowrap\" ><input type=\"file\" name=\"arquivo[' || a.arqid || ']\" id=\"arquivo_' ||  a.arqid || '\" > <img class=\"middle link\" onclick=\"limpaUpload(\'' || a.arqid || '\')\" src=\"../imagens/excluir.gif\" /></span>' as upload
			FROM 
				arquivo a
			INNER JOIN 
				profeinep.anexos anx ON anx.arqid = a.arqid
			INNER JOIN
				profeinep.processoprofeinep prc ON prc.prcid = anx.prcid
			LEFT JOIN 
				profeinep.estruturaprocesso esp ON prc.prcid = esp.prcid 
	    	LEFT JOIN 
	    		profeinep.coordenacao coo ON coo.coonid = prc.cooid
	    	LEFT JOIN 
	    		workflow.documento doc ON doc.docid = esp.docid 
	    	LEFT JOIN 
	    		(SELECT max(htddata) as data, docid FROM workflow.historicodocumento GROUP BY docid ) wd ON wd.docid = doc.docid
	    	LEFT JOIN 
	    		workflow.estadodocumento esd ON esd.esdid = doc.esdid
	    	LEFT JOIN 
	    		profeinep.expressaochave exp ON exp.prcid = prc.prcid  
	    	LEFT JOIN 
	    		profeinep.procedencia pro ON pro.proid = prc.proid
	        LEFT JOIN 
	        	profeinep.tipoprioridade tpr ON tpr.tipid = prc.tipid
			INNER JOIN
				seguranca.usuario u ON u.usucpf = a.usucpf 
			WHERE 
				anxstatus = 'A'::bpchar 
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "" );

	return array("sql" => $sql, "cabecalho" => $cabecalho);
}