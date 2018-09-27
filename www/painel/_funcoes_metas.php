<?php
function filtrarAcao()
{
	global $db;
	$secid = $_POST['secid'];
	if($secid){
		$habilitado = "S";
		$sql = "select
					acaid as codigo,
					acadsc as descricao
				from
					painel.acao
				where
					acastatus = 'A'
				and
					acaid in ( select acaid from painel.acaosecretaria where secid = $secid)
				order by
					acadsc";
	}else{
		$habilitado = "N";
		$sql = array();
	}
	$db->monta_combo('acaid',$sql,$habilitado,'Selecione a Ação','filtrarIndicador','');
}

function pesquisarMetas()
{

}

function filtrarIndicador()
{
	global $db;
	$secid = $_POST['secid'];
	$acaid = $_POST['acaid'];
	if($secid && $acaid){
		$habilitado = "S";
		$sql = "select
					indid as codigo,
					indid || ' - ' || indnome as descricao
				from
					painel.indicador
				where
					indstatus = 'A'
				and
					secid = $secid
				and
					acaid = $acaid
				order by
					indid,indnome";
	}else{
		$habilitado = "N";
		$sql = array();
	}
	$db->monta_combo('indid',$sql,$habilitado,'Selecione o Indicador','filtrarPeriodicidade','');
}

function filtrarPeriodicidade()
{
	global $db;
	$indid = $_POST['indid'];
	if($indid){
		$habilitado = "S";
		$sql = "select
					perid as codigo,
					perdsc as descricao
				from
					painel.periodicidade
				where
					perstatus = 'A'
				and
					pernivel >= (select pernivel from  painel.periodicidade where perid = (select perid from painel.indicador where indid = $indid))
				order by
					pernivel";
	}else{
		$habilitado = "N";
		$sql = array();
	}
	$db->monta_combo('perid',$sql,$habilitado,'Selecione a Periodicidade','verificaMetaAnterior','');
}

function verificaMetaAnterior()
{
	global $db;
	$indid = $_POST['indid'];
	$perid = $_POST['perid'];
	$sql = "select metid from painel.metaindicador where indid = $indid and perid = $perid and metstatus = 'A'";
	$metid = $db->pegaUm($sql);
	if($metid){
		echo $metid;
	}
	die;
}

function salvarMetas()
{
	global $db;

	$metid = $_POST['metid'];
	$perid = $_POST['perid'];
	$indid = $_POST['indid'];
	$metdesc = $_POST['metdesc'];
	$metcumulativa = $_POST['metcumulativa'];

	if(!$perid || !$indid || !$metdesc || !$metcumulativa){
		return array("msg" => "Favor preencher todos os campos obrigatórios!", "erro" => true);
	}

	if($metid){
		$sql = "update
					painel.metaindicador
				set
					indid = $indid,
					perid = $perid,
					metdesc = '$metdesc',
					metcumulativa = '$metcumulativa'
				where
					metid = $metid";
		$db->executar($sql);
	}else{
		$sql = "insert into
					painel.metaindicador
				(indid,perid,metdesc,metstatus,metcumulativa)
					values
				($indid,$perid,'$metdesc','A','$metcumulativa') returning metid";
	}
		$metid = $db->pegaUm($sql);
	if($db->commit()){
		return array("msg" => "Operação realizado com sucesso!", "metid" => $metid);
	}else{
		return array("msg" => "Não foi possível realizar a operação!", "erro" => true);
	}

}

function listarEstrategias()
{
	global $db;

	$sql = "select
			(CASE
				WHEN (select count(*) from painel.acaoestrategia aca where aca.estid = est.estid) > 0
				 THEN '<img id=\"img_estid_' || estid || '\" onclick=\"carregarAcaoPorEstrategia(\'' || estid || '\')\" class=\"link\" src =\"../imagens/mais.gif\" />'
				 ELSE ''
			END) as mais,
			'<img src=\"../imagens/gif_inclui.gif\" class=\"link\" onclick=\"addAcao(\'' || estid || '\')\" /> <img src=\"../imagens/alterar.gif\" class=\"link\" onclick=\"editarEstrategia(\'' || estid || '\')\" /> <img src=\"../imagens/excluir.gif\" class=\"link\" onclick=\"excluirEstrategia(\'' || estid || '\')\" />' as acao,
			estdesc ||
			(CASE
				WHEN (select count(*) from painel.acaoestrategia aca where aca.estid = est.estid) > 0
				 THEN '</td></tr><tr id=\"tr_estid_' || est.estid || '\" class=\"hidden\" ><td id=\"td_estid_' || est.estid || '\" colspan=\"3\" > as as as '
				 ELSE ''
			END) as estdesc
		from
			painel.estrategiameta est
		where
			 metid = {$_SESSION['painel']['metid']}
		and
			eststatus = 'A'
		order by
			estdesc";
	$arrDados = $db->carregar($sql);
	$cabecalho = array("", "Ações", "Estratégia");
	$db->monta_lista_array($arrDados,$cabecalho,100,5,'N','center');
}

function salvarEstrategia(){
	global $db;

	$metid   = $_POST['metid'];
	$estid   = $_POST['id'];
	$estdesc = utf8_decode($_POST['descricao']);

	if(!$metid || !$estdesc){
		echo "false";
	}

	if($estid){
		$sql = "update
					painel.estrategiameta
				set
					estdesc = '$estdesc'
				where
					estid = $estid";
	}else{
		$sql = "insert into
					painel.estrategiameta
				(metid,estdesc,eststatus)
					values
				($metid,'$estdesc','A')";
	}

	$db->executar($sql);
	if($db->commit()){
		echo "true";
	}else{
		echo "false";
	}
}

function getEstrategia()
{
	global $db;
	$estid   = $_POST['estid'];
	$sql = "select
				estdesc
			from
				painel.estrategiameta
			where
				estid = $estid";
	echo $db->pegaUm($sql);
}

function excluirEstrategia()
{
	global $db;

	$estid = $_POST['estid'];
	$sql = "update painel.estrategiameta set eststatus = 'I' where estid = $estid;";
	$sql.= "delete from painel.acaoestrategia where estid = $estid;";
	$db->executar($sql);
	if($db->commit()){
		echo "true";
	}else{
		echo "false";
	}
}

function salvarAcao()
{
	global $db;

	$estid   = $_POST['estid'];
	$acaid   = $_POST['acaid'];
	$aceid   = $_POST['id'];

	if(!$estid || !$acaid){
		echo "false";
	}

	if($aceid){
		$sql = "update
					painel.acaoestrategia
				set
					estid = $estid,
					acaid = $acaid
				where
					aceid = $aceid";
	}else{
		$sql = "insert into
					painel.acaoestrategia
				(estid,acaid)
					values
				($estid,$acaid)";
	}

	$db->executar($sql);
	if($db->commit()){
		echo "true";
	}else{
		echo "false";
	}
}

function getTxtEstrategia()
{
	global $db;

	$estid = $_POST['estid'];

	if($estid){
		$sql = "select
				estdesc
			from
				painel.estrategiameta
			where
				estid = $estid";
		$campo_descricao = $db->pegaUm($sql);
	}

	echo campo_textarea( 'campo_descricao', 'S', 'S', '', 60, 2, 1000 ,'','','','','',$campo_descricao);
}

function getComboAcao()
{
	global $db;

	$acaid = $_POST['acaid'];

	$sql = "select
				acaid as codigo,
				acadsc as descricao
			from
				painel.acao
			where
				acastatus = 'A'
			order by
				acadsc";

	$db->monta_combo('acaid',$sql,"S",'Selecione a Ação','','','','','','','',$acaid);
}

function getAcaoPorEstrategia()
{
	global $db;
	$estid = $_POST['estid'];

	$sql = "select
			'<img src=\"../imagens/alterar.gif\" class=\"link\" onclick=\"editarAcao(\'' || ace.aceid || '\',\'' || ace.estid || '\',\'' || ace.acaid || '\')\" /> <img src=\"../imagens/excluir.gif\" class=\"link\" onclick=\"excluirAcao(\'' || ace.aceid || '\')\" />' as acao,
			aca.acadsc
		from
			painel.acaoestrategia ace
		inner join
			painel.acao aca ON aca.acaid = ace.acaid
		where
			 estid = $estid
		and
			aca.acastatus = 'A'
		order by
			aca.acadsc";
	$cabecalho = array("Ações", "Ação Estratégica");
	$db->monta_lista($sql,$cabecalho,100,5,'N','center');

}

function excluirAcao()
{
	global $db;

	$aceid = $_POST['aceid'];

	$sql = "delete from painel.acaoestrategia where aceid = $aceid;";
	$db->executar($sql);
	if($db->commit()){
		echo "true";
	}else{
		echo "false";
	}
}

function getComboPeriodoPorPerid($perid,$removeUtilizados = true,$dmiid = null,$metid = null)
{
	global $db;

	$dpeid = $_POST['dpeid'];

	if(!$dpeid){
		$removeUtilizados = false;
	}

	if($perid == 5){
		if($dpeid && $removeUtilizados == false){
			$data = $db->pegaUm("select dpedsc from painel.detalheperiodicidade where dpeid = $dpeid");
		}
		echo campo_data2("dpeid","S","S","","","","",$data);
		return true;
	}

	$sql = "select
				dpeid as codigo,
				dpedsc as descricao
			from
				painel.detalheperiodicidade
			where
				dpestatus = 'A'
			and
				perid = $perid
			".($removeUtilizados ? "and
										dpeid not in (	select
															distinct dpe.dpeid
														from
															painel.detalheperiodicidade dpe
														inner join
															painel.detalhemetaindicador dmi ON dpe.dpeid = dmi.dpeid
														where
															dpe.dpestatus = 'A'
														and
															dmi.dmistatus = 'A'
														".($dmiid ? " and
															dmi.dmiid != $dmiid " : " ")."
														and
															dmi.metid = $metid
													  )" : "and
																dpeid not in (	select
																					distinct dpe.dpeid
																				from
																					painel.detalheperiodicidade dpe
																				inner join
																					painel.detalhemetaindicador dmi ON dpe.dpeid = dmi.dpeid
																				where
																					dpe.dpestatus = 'A'
																				and
																					dmi.dmistatus = 'A'
																				".($dmiid ? " and
																					dmi.dmiid != $dmiid " : " ")."
																				and
																					dmi.metid = $metid
																			  )")."
			order by
				dpeordem,
				dpeanoref";

	$db->monta_combo('dpeid',$sql,"S",'Selecione o Período','','','','','S','','',$dpeid);
}

function listarValorMetas($metid)
{
	global $db;

	$sql = "select
			ind.indqtdevalor,
			ind.indid,
			ind.unmid,
			ume.umedesc
                        
		from
			painel.metaindicador met
		inner join
			painel.indicador ind ON ind.indid = met.indid
		inner join
			painel.unidademeta ume ON ind.umeid = ume.umeid
		where
			metid = $metid
		order by
			metid";

	$arrDados = $db->pegaLinha($sql);

	$indqtdevalor = $arrDados['indqtdevalor'];
	$indid = $arrDados['indid'];

	if(!$indid){
		echo "Não foi possível recuperar as metas.";
		return false;
	}

	$umedesc = $arrDados['umedesc'];
	$formatoinput = pegarFormatoInput($indid);
	$formatoinput['campovalor']['mascara'] = !$formatoinput['campovalor']['mascara'] ? "###.###.###.###.###,##" : $formatoinput['campovalor']['mascara'];

	if($arrDados['unmid'] != UNIDADEMEDICAO_BOLEANA){
		$sql = "select
				CASE WHEN dmi.dmivalor is not null
					THEN '<img src=\"../imagens/alterar.gif\" class=\"link\" onclick=\"editarValorMeta(\'' || dpe.perid || '\',\'' || dmi.dmiid || '\',\'' || dpe.dpeid || '\',\'' || coalesce(dmi.dmiobs,'') || '\',\'' || trim(to_char(dmi.dmiqtde, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."')) || '\'".($indqtdevalor == "t" ? ",\'' || trim(to_char(dmi.dmivalor, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['campovalor']['mascara'])."')) || '\'" : "").")\" /> <img src=\"../imagens/excluir.gif\" class=\"link\" onclick=\"excluirValorMeta(\'' || dmi.dmiid || '\')\" />'
					ELSE '<img src=\"../imagens/alterar.gif\" class=\"link\" onclick=\"editarValorMeta(\'' || dpe.perid || '\',\'' || dmi.dmiid || '\',\'' || dpe.dpeid || '\',\'' || coalesce(dmi.dmiobs,'') || '\',\'' || trim(to_char(dmi.dmiqtde, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."')) || '\')\" /> <img src=\"../imagens/excluir.gif\" class=\"link\" onclick=\"excluirValorMeta(\'' || dmi.dmiid || '\')\" />'
				END as acao,
				dpe.dpedsc,
				CASE WHEN dmi.dmidatameta IS NOT NULL
					THEN to_char(dmi.dmidatameta,'DD/MM/YYYY')
					ELSE 'N/A'
				END as dmidatameta,
				CASE WHEN dmi.dmidataexecucao IS NOT NULL
					THEN to_char(dmi.dmidataexecucao,'DD/MM/YYYY')
					ELSE 'N/A'
				END as dtexecucao,
				CASE WHEN dmi.dmidatavalidacao IS NOT NULL
					THEN to_char(dmi.dmidatavalidacao,'DD/MM/YYYY')
					ELSE 'N/A'
				END as dmidatavalidacao,
				CASE WHEN doc.esdid IS NOT NULL
					THEN esd.esddsc
					ELSE 'N/A'
				END as estado,
				trim(to_char(dmi.dmiqtde, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."')) as dmiqtde
				".($indqtdevalor == "t" ? ",trim(to_char(dmi.dmivalor, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['campovalor']['mascara'])."')) as dmivalor" : "").",
                                dmi.dmiobs
			from
				painel.detalhemetaindicador dmi
			inner join
				painel.detalheperiodicidade dpe ON dmi.dpeid = dpe.dpeid
			left join
				workflow.documento doc ON dmi.docid = doc.docid
			left join
				workflow.estadodocumento esd ON esd.esdid = doc.esdid
			where
				dmi.metid = $metid
			and
				dmi.dmistatus = 'A'
			order by
				dmi.dmidatameta";
		$arrDados = $db->carregar($sql);

		$cabecalho = array("Ações","Período","Data da Meta","Data de Execução","Data de Validação","Estado",$umedesc,'Observação');

		if($indqtdevalor == "t"){
			array_push($cabecalho,"Valor");
		}
	}else{
		$sql = "select
				CASE WHEN dmi.dmivalor is not null
					THEN '<img src=\"../imagens/alterar.gif\" class=\"link\" onclick=\"editarValorMeta(\'' || dpe.perid || '\',\'' || dmi.dmiid || '\',\'' || dpe.dpeid || '\',\'' || trim(to_char(dmi.dmiqtde, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."')) || '\'".($indqtdevalor == "t" ? ",\'' || trim(to_char(dmi.dmivalor, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['campovalor']['mascara'])."')) || '\'" : "").")\" /> <img src=\"../imagens/excluir.gif\" class=\"link\" onclick=\"excluirValorMeta(\'' || dmi.dmiid || '\')\" />'
					ELSE '<img src=\"../imagens/alterar.gif\" class=\"link\" onclick=\"editarValorMeta(\'' || dpe.perid || '\',\'' || dmi.dmiid || '\',\'' || dpe.dpeid || '\',\'' || trim(to_char(dmi.dmiqtde, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."')) || '\')\" /> <img src=\"../imagens/excluir.gif\" class=\"link\" onclick=\"excluirValorMeta(\'' || dmi.dmiid || '\')\" />'
				END as acao,
				dpe.dpedsc,
				CASE WHEN dmi.dmidatameta IS NOT NULL
					THEN to_char(dmi.dmidatameta,'DD/MM/YYYY')
					ELSE 'N/A'
				END as dmidatameta,
				CASE WHEN dmi.dmidataexecucao IS NOT NULL
					THEN to_char(dmi.dmidataexecucao,'DD/MM/YYYY')
					ELSE 'N/A'
				END as dtexecucao,
				CASE WHEN dmi.dmidatavalidacao IS NOT NULL
					THEN to_char(dmi.dmidatavalidacao,'DD/MM/YYYY')
					ELSE 'N/A'
				END as dmidatavalidacao,
				CASE WHEN doc.esdid IS NOT NULL
					THEN esd.esddsc
					ELSE 'N/A'
				END as estado
			from
				painel.detalhemetaindicador dmi
			inner join
				painel.detalheperiodicidade dpe ON dmi.dpeid = dpe.dpeid
			left join
				workflow.documento doc ON dmi.docid = doc.docid
			left join
				workflow.estadodocumento esd ON esd.esdid = doc.esdid
			where
				dmi.metid = $metid
			and
				dmi.dmistatus = 'A'
			order by
				dmi.dmidatameta";
		$arrDados = $db->carregar($sql);

		$cabecalho = array("Ações","Período","Data da Meta","Data de Execução","Data de Validação","Estado");
	}
	$db->monta_lista_array($arrDados,$cabecalho,100,5,'N','center');
}

function salvarValorMeta()
{
global $db;

	$dmiid 	  = $_POST['dmiid'];
	$dpeid 	  = $_POST['dpeid'];
	if($_POST['dmidataexecucao'] && !$dpeid){
		 $dpeid = $_POST['dmidataexecucao'];
		 $_POST['dpeid'] = $_POST['dmidataexecucao'];
	}
	if(!is_numeric($dpeid)){
		$data = explode("/",$dpeid);
		$ano = $data[2];
		$mes = $data[1];
		$dia = $data[0];
		$ordem = ((int)date("z",mktime(0, 0, 0,$mes,$dia,$ano)))+1;
		$sql = "select
					dpeid
				from
					painel.detalheperiodicidade
				where
					dpemesref = '$mes'
				and
					dpeanoref = '$ano'
				and
					dpedatainicio = '$ano-$mes-$dia'
				and
					dpedatafim = '$ano-$mes-$dia'
				and
					perid = 5";
		$dpeid = $db->pegaUm($sql);
		if(!$dpeid){
			$sql = "insert into
						painel.detalheperiodicidade
					(perid,dpedsc,dpestatus,dpeordem,dpemesref,dpeanoref,dpedatainicio,dpedatafim)
						values
					(5,'{$_POST['dpeid']}','A',$ordem,'$mes','$ano','$ano-$mes-$dia','$ano-$mes-$dia') returning dpeid";
			$dpeid = $db->pegaUm($sql);
		}
	}
	$metid	  = $_POST['metid'];
	$dmivalor = $_POST['dmivalor'] ? "'".str_replace(array(".",","),array("","."),$_POST['dmivalor'])."'" : "null";
	$dmiqtde  = str_replace(array(".",","),array("","."),$_POST['dmiqtde']);
	$dmidataexecucao = $_POST['dmidataexecucao'] ? "'".formata_data_sql($_POST['dmidataexecucao'])."'" : "NULL";
	$dmidatavalidacao = $_POST['dmidatavalidacao'] ? "'".formata_data_sql($_POST['dmidatavalidacao'])."'" : "NULL";
	$dmdestavel = $_POST['dmdestavel'] ? $_POST['dmdestavel'] : "NULL";
	$dmdcritico = $_POST['dmdcritico'] ? $_POST['dmdcritico'] : "NULL";
        $dmiobs = $_POST['dmiobs'] ? "'".$_POST['dmiobs']."'" : "NULL";

	if(!is_numeric($_POST['dpeid'])){
		$dmidatameta = $_POST['dpeid'] ? "'".formata_data_sql($_POST['dpeid'])."'" : "NULL";
	}else{
		$dmidatameta = $_POST['dmidatameta'] ? "'".formata_data_sql($_POST['dmidatameta'])."'" : "NULL";
	}

	if(!$dpeid || !$dmiqtde || !$metid){
		return array("msg" => "Favor preencher todos os campos obrigatórios!", "erro" => true);
	}

	if($dmiid){
		$sql = "update
					painel.detalhemetaindicador
				set
					dpeid = $dpeid,
					dmivalor = $dmivalor,
					dmiqtde = '$dmiqtde',
					dmidataexecucao  = $dmidataexecucao,
					dmidatavalidacao = $dmidatavalidacao,
					dmidatameta = $dmidatameta,
					dmdestavel = $dmdestavel,
					dmdcritico = $dmdcritico,
                                        dmiobs = $dmiobs
				where
					dmiid = $dmiid";
		$db->executar($sql);
	}else{
		$sql = "insert into
					painel.detalhemetaindicador
				(metid,dpeid,dmivalor,dmiqtde,dmistatus,dmidtcoleta,dmibloqueado,dmidataexecucao,dmidatavalidacao,dmidatameta,dmdestavel,dmdcritico, dmiobs)
					values
				($metid,$dpeid,$dmivalor,'$dmiqtde','A',now(),false,$dmidataexecucao,$dmidatavalidacao,$dmidatameta,$dmdestavel,$dmdcritico, $dmiobs) returning dmiid";
		$dmiid = $db->pegaUm($sql);
	}

	if($db->commit()){
		return array("dmiid" => $dmiid, "msg" => "Operação realizado com sucesso!");
	}else{
		return array("msg" => "Não foi possível realizar a operação!", "erro" => true);
	}
}

function salvarExibicaoMeta()
{
	global $db;

	$prtobj = serialize($_POST);
	$usucpf = $_SESSION['usucpf'];
	$mnuid =  $_SESSION['mnuid'];

	$sql = "select mnuid from public.parametros_tela where mnuid = $mnuid and prtdsc = '{$_SESSION['indid']}'";

	if($db->pegaUm($sql)){
		$sql = "update
				public.parametros_tela
			set
				prtobj = '$prtobj',
				prtdata = now(),
				usucpf = '$usucpf'
			where
				mnuid = $mnuid
			and
				prtdsc = '{$_SESSION['indid']}'";
	}else{
		$sql = "insert into
				public.parametros_tela
			(prtdsc,prtobj,prtpublico,prtdata,usucpf,mnuid)
				values
			('{$_SESSION['indid']}','$prtobj',false,now(),'$usucpf',$mnuid)";
	}
	$db->executar($sql);
	$db->commit($sql);
}

function getMetaIndicador($indid = null)
{
	global $db;

	$indid = !$indid ? $_SESSION['indid'] : $indid;

	if(!$indid){
		return false;
	}

	$sql = "select
				met.metid,
				dpe.perid,
				per.perdsc,
				dpe.dpeid,
				dpe.dpedsc
			from
				painel.metaindicador met
			inner join
				painel.detalhemetaindicador dmi ON dmi.metid = met.metid
			inner join
				painel.detalheperiodicidade dpe ON dpe.dpeid = dmi.dpeid
			inner join
				painel.periodicidade per ON per.perid = dpe.perid
			where
				met.indid = $indid
				--and per.perid = 1
			order by
				per.pernivel,
				dpe.dpeordem,
				dpe.dpeanoref";

	$arrDados = $db->carregar($sql);

	if($arrDados){
		foreach($arrDados as $dado){
			$arrResultado['periodo'][  $dado['perid'] ] = array("codigo" => $dado['perid'], "descricao" => $dado['perdsc']);
			$arrResultado['periodicidade'][ $dado['perid'] ][] = array("codigo" => $dado['dpeid'], "descricao" => $dado['dpedsc']);
		}
		return $arrResultado;
	}else{
		return false;
	}

}

function excluirValorMeta()
{
	global $db;

	$dmiid = $_POST['dmiid'];

	$sql = "update painel.detalhemetaindicador set dmistatus = 'I' where dmiid = $dmiid;";
	$db->executar($sql);
	if($db->commit()){
		echo "true";
	}else{
		echo "false";
	}

}

function excluirMeta()
{
	global $db;

	$metid = $_POST['metid'];

	$sql = "update painel.metaindicador set metstatus = 'I' where metid = $metid;";
	$db->executar($sql);
	if($db->commit()){
		return true;
	}else{
		return false;
	}

}

function alertlocation($dados)
{
    die("<script>
        ".(($dados['alert'])?"alert('".$dados['alert']."');":"")."
        ".(($dados['location'])?"window.location='".$dados['location']."';":"")."
        ".(($dados['javascript'])?$dados['javascript']:"")."
         </script>");
}
?>