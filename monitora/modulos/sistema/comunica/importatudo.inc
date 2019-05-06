<?php
	
	// lista de itens que serão importados
	$itens = array(
		'apoio' => array(
			'CargaTipoAcao',
			'CargaTipoOrgao',
			'CargaTipoPrograma',
			'CargaTipoRestricao',
//			'CargaTipoSituacao',
			'CargaUnidade',
			'CargaUnidadeMedida',
			'CargaBaseGeografica',
			'CargaFontePPA',
			'CargaMacroObjetivo',
			'CargaOpcaoEstrategica',
			'CargaOrgao',
			'CargaPeriodicidade',
			'CargaProduto',
			'CargaRegiao'
		),
		'principal' => array(
			'CargaProgramacao',
		),
		'qualitativa' => array(
//			'CargaEsfera',
//			'CargaMunicipio',
//			'CargaNatureza',
//			'CargaOrgaoSIORG',
//			'CargaProduto',
//			'CargaRegiao',
//			'CargaTipoAcao',
//			'CargaPPAFuncao',
//			'CargaTipoAtualizacao',
//			'CargaTipoDespesa',
//			'CargaTipoInclusaoAcao',
//			'CargaTipoOrgao',
//			'CargaTipoPrograma',
//			'CargaUnidadeMedida',
//			'CargaPPAPrograma',
//			'CargaPPAAcao',
//			'CargaPPALocalizador',
//			'CargaPPAIndicador',
//			'CargaPPADadoFinanceiro',
//			'CargaPPADadoFisico',
//			'CargaPPAEtapa', # não existe tabela no módulo elaboração
//			'CargaPPAUnidade', # não existe tabela no módulo elaboração
//			'CargaPPAFonte',
//			'CargaPPAOrgao',
//			'CargaPPASubFuncao',
		),
	);
	
	// inicializa o objeto responsável pela importação
	include APPRAIZ.'includes/Sigplan.php';
	$ano     = $_REQUEST['anodestino'];
	$sigplan = new Sigplan( $ano );
	
	// importa os arquivos indicados na lista de itens
	foreach ( $itens as $natureza => $arquivos ) {
		foreach ( $arquivos as 	$arquivo ) {
			echo $arquivo.' >> ';
			flush();
			ob_flush();
			if ( !$sigplan->importar( $natureza, $arquivo ) ) {
				$db->rollback();
				$db->insucesso( "Ocorreu um erro ao importar o arquivo $arquivo.xml de natureza $natureza", array(), $modulo);
			}
			echo $arquivo."<br>";
			flush();
			ob_flush();
		}
	}
		
	// retira do nome da ação a descrição RAP, pois a mesma é controlada pelo campo 'acasnrap'. 
	$sql = "update monitora.acao set acadsc = replace(acadsc, '(RAP " . ($ano - 1) . ") ', '') where prgano = '$ano'";
	$db->executar($sql);
	
	// SCRIPT (PROGRAMA) ATRIBUI AS RESPONSABILIDADES DOS PROGRAMAS DO ANO ANTERIOR
	$sql = "insert into monitora.usuarioresponsabilidade (pjeid, prgid, acaid, usucpf, rpustatus, rpudata_inc, pflcod, prsano, rpucompromisso, rpudata_ini, rpudata_fim, unicod)
		
		select 	ur.pjeid, p2.prgid, ur.acaid, ur.usucpf, 
			ur.rpustatus, ur.rpudata_inc, ur.pflcod, p2.prgano AS prsano,
			ur.rpucompromisso, ur.rpudata_ini, 
			ur.rpudata_fim, ur.unicod
		
		from 	monitora.usuarioresponsabilidade ur 
		
		inner join planejamento.programa p1 on p1.prgid = ur.prgid and p1.prgano = '".($ano - 1)."' and p1.prgstatus = 'A'
		inner join planejamento.programa p2 on p1.prgcod = p2.prgcod and p2.prgano = '$ano'
		where ur.rpustatus='A'";	
	$db->executar($sql);
	
	
	// SCRIPT (AÇÃO) ATRIBUI AS RESPONSABILIDADES DAS AÇÕES DO ANO ANTERIOR
	$sql =	"insert into monitora.usuarioresponsabilidade (pjeid, prgid, acaid, usucpf, rpustatus, rpudata_inc, pflcod, prsano, rpucompromisso, rpudata_ini, rpudata_fim, unicod)
		
		select 	ur.pjeid, ur.prgid, a2.acaid, ur.usucpf, 
			ur.rpustatus, now() as rpudata_inc, ur.pflcod, a2.prgano AS prsano,
			ur.rpucompromisso, ur.rpudata_ini, 
			ur.rpudata_fim, ur.unicod
		
		from 	monitora.usuarioresponsabilidade ur 
		
		inner join monitora.acao a1 on a1.acaid = ur.acaid and a1.prgano = '".($ano - 1)."' and a1.acastatus = 'A'
		
		inner join monitora.acao a2 on a1.prgcod = a2.prgcod and a1.acacod = a2.acacod and 
			   a1.unicod = a2.unicod and a1.loccod = a2.loccod and a2.prgano = '$ano' and 
			   a2.acasnrap = a1.acasnrap
		
		where ur.rpustatus='A'";
	$db->executar($sql);
	
	$db->commit();
	exit;
	
	// abaixo crio os registros novos na tabela ppaprograma_proposta caso eles ainda não tenham sido criados	
	$sql = "insert into elabrev.ppaprograma_proposta 
 			(prgcod,tpocod,tprcod,
  			orgcod,prgdsc,
			  prgmesinicio,prganoinicio,prgmestermino,prganotermino,
			  prgdscobjetivo, prgdscpublicoalvo, 
			  prgdscestrategia,prgvlrprograma,
			  prgsntemporario,prgsnpadronizado,
			  prgobs,prsano,descod )
			select prgcod,tpocod,tprcod,
			  orgcod,prgdsc,
			  prgmesinicio,prganoinicio,prgmestermino,prganotermino,
			  prgdscobjetivo, prgdscpublicoalvo, 
			  prgdscestrategia,prgvlrprograma,
			  prgsntemporario,prgsnpadronizado,
			  prgobs,prsano,descod 
 			from elabrev.ppaprograma where prsano='$ano' and elabrev.ppaprograma.prgcod||'-'||elabrev.ppaprograma.prsano not in (select elabrev.ppaprograma_proposta.prgcod||'-'||elabrev.ppaprograma_proposta.prsano from elabrev.ppaprograma_proposta)";
	$saida = $db->executar($sql);
	
	// agora atualizo o prgid na tabela ppaacao
	
	$sql = "update elabrev.ppaacao set prgid=p.prgid from elabrev.ppaprograma p 
where p.prgcod=elabrev.ppaacao.prgcod and p.prsano=elabrev.ppaacao.prsano and elabrev.ppaacao.prgid is null";
	$saida = $db->executar($sql);
	
	//atualizo o prgid na tabela ppaindicador
	$sql = "update elabrev.ppaindicador set prgid=p.prgid from elabrev.ppaprograma_proposta p where p.prgcod=elabrev.ppaindicador.prgcod and p.prsano=elabrev.ppaindicador.prsano and elabrev.ppaindicador.prgid is null";
	$saida = $db->executar($sql);

	// abaixo vou atualizar a tabela ppaacao_proposta em função dos novos registros da tabela ppaacao
	$sql = "select prgcod,
		acacod,funcod, 
		sfucod,
		taccod,
		procod,
		acadscproduto,
		unmcod,
		tincod,
		esfcod,
		acadsc,
		acafinalidade,
		acadescricao,
		acabaselegal,
		acarepercfinanceira,
        acavlrrepercfinanceira,
  		acasnpadronizada,
  		acasnsetpadronizada,
  		acasnmedireta,
  		acasnmedesc,
  		acasnmelincred,
  		acasntransfobrigatoria,
		acasntransfvoluntaria,
  		acasntransfoutras,
  		acasndespesaobrigatoria,
  		acasnbloqueioprogramacao,
  		acadetalhamento,
  		acamesinicio,
  		acaanoinicio,
  		acamestermino,
  		acaanotermino,
  		acavlrcustototal,
  		acaqtdcustototal,
  		acavlrcustoateanoanterior,
  		acaqtdateanoanterior,
  		acavlrcustoprevistoanocorrente,
  		acaqtdprevistoanocorrente,
  		acaordemprioridade,
        prsano
        from elabrev.ppaacao where prsano='$ano'";
	
	dbg($sql, 1);
	
	$registros = $db->carregar($sql);
	foreach ($registros as $registro)
	{
		// a linha abaixo transforma em variáveis todos os campos do array
		if(is_array($registro)) foreach($registro as $k=>$v) ${$k}=$v;
		$sql = "select acaid from elabrev.ppaacao_proposta where acacod='$acacod' and prgcod='$prgcod' and prsano='$ano' limit 1";
		if (! $db->pegaUm($sql))
		{
			// então tenho que inserir
			$sql = "insert into elabrev.ppaacao_proposta (prgcod,
		acacod,funcod, 
		sfucod,
		taccod,
		procod,
		acadscproduto,
		unmcod,
		tincod,
		esfcod,
		acadsc,
		acafinalidade,
		acadescricao,
		acabaselegal,
		acarepercfinanceira,
        acavlrrepercfinanceira,
  		acasnpadronizada,
  		acasnsetpadronizada,
  		acasnmedireta,
  		acasnmedesc,
  		acasnmelincred,
  		acasntransfobrigatoria,
		acasntransfvoluntaria,
  		acasntransfoutras,
  		acasndespesaobrigatoria,
  		acasnbloqueioprogramacao,
  		acadetalhamento,
  		acamesinicio,
  		acaanoinicio,
  		acamestermino,
  		acaanotermino,
  		acavlrcustototal,
  		acaqtdcustototal,
  		acavlrcustoateanoanterior,
  		acaqtdateanoanterior,
  		acavlrcustoprevistoanocorrente,
  		acaqtdprevistoanocorrente,
  		acaordemprioridade,
        prsano) 
  		
  		values (
  		'$prgcod',
		'$acacod',
		$funcod, 
		$sfucod,
		$taccod,
		$procod,
		'$acadscproduto',
		$unmcod,
		$tincod,
		$esfcod,
		'$acadsc',
		'$acafinalidade',
		'$acadescricao',
		'$acabaselegal',
		'$acarepercfinanceira',
        $acavlrrepercfinanceira,
  		'$acasnpadronizada',
  		'$acasnsetpadronizada',
  		'$acasnmedireta',
  		'$acasnmedesc',
  		'$acasnmelincred',
  		'$acasntransfobrigatoria',
		'$acasntransfvoluntaria',
  		'$acasntransfoutras',
  		'$acasndespesaobrigatoria',
  		'$acasnbloqueioprogramacao',
  		'$acadetalhamento',
  		'$acamesinicio',
  		'$acaanoinicio',
  		'$acamestermino',
  		'$acaanotermino',
  		$acavlrcustototal,
  		$acaqtdcustototal,
  		$acavlrcustoateanoanterior,
  		$acaqtdateanoanterior,
  		$acavlrcustoprevistoanocorrente,
  		$acaqtdprevistoanocorrente,
  		$acaordemprioridade,
        '$prsano')";
			dbg($sql);
			$saida=$db->executar($sql);
		}
	}
  
  // agora atualizo o prgid da tabela ppaacao_proposta com os contidos na tabela ppaprograma_proposta
  	$sql = "update elabrev.ppaacao_proposta set prgid=p.prgid from elabrev.ppaprograma_proposta p 
where p.prgcod=elabrev.ppaacao_proposta.prgcod and p.prsano=elabrev.ppaacao_proposta.prsano and elabrev.ppaacao_proposta.prgid is null";
	$saida = $db->executar($sql);
	
 
	// finalmente atualizo a tabela elabrev.localizadorgasto_acao
	$sql = "select a.acaid, aa.acacodppa, a.prsano from elabrev.ppaacao_proposta a inner join elabrev.ppaacao aa on a.acacod=aa.acacod and a.prgcod=aa.prgcod and a.prsano=aa.prsano where a.prsano ='$ano'";
	$registros = $db->carregar($sql);
	foreach ($registros as $registro)
	{
		 // a linha abaixo transforma em variáveis todos os campos do array
		if(is_array($registro)) foreach($registro as $k=>$v) ${$k}=$v;
		$sql = "select regcod from elabrev.ppalocalizador where acacodppa='$acacodppa' and prgcod='$prgcod' and prsano='$ano'";
		$regs = $db->carregar($sql);
	    foreach ($regs as $reg)
		{
			$sql = "select * from elabrev.localizadorgasto_acao where acaid = $acaid and regcod='$regcod' ";
			if (! $db->pegaum($sql))
			{
                if ($regcod) {
				$sql = "insert into elabrev.localizadorgasto_acao (acaid, regcod) values ($acaid, '$regcod')";
				dbg($sql);
				$db->executar($sql);
				}
			}
		}
	}

	$db->commit();
  
	// grava o relatório de importação e confirma a operação
	if( !$sigplan->gravarRelatorio() ){
		$db->insucesso( "Ocorreu um erro ao gravar o relatório de importação", array(), $modulo );
	}
	$db->sucesso( $modulo );
	
?>