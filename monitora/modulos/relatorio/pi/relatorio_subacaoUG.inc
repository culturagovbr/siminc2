<?php
ini_set("memory_limit", "1024M");
set_time_limit(0);

include APPRAIZ . 'monitora/www/planotrabalhoUG/_constantes.php';
include APPRAIZ . 'monitora/www/planotrabalhoUG/_funcoes.php';

/******************
 * Pega array com perfis
 ******************/
function arrayPerfil(){
	global $db;

	$sql = sprintf("SELECT pu.pflcod
					FROM seguranca.perfilusuario pu
					INNER JOIN seguranca.perfil p ON p.pflcod = pu.pflcod AND p.sisid = 1
					WHERE pu.usucpf = '%s' 
					ORDER BY p.pflnivel",
	$_SESSION['usucpf']);
	return (array) $db->carregarColuna($sql,'pflcod');
}

//recupera os perfis do usuario
$perfil = arrayPerfil();

// Filtros do relatório
if ( $_REQUEST['requisicao'] == 1 ){
	
	$filtro = array();
	$filtropi = array();
	$filtrosubacao = array();
	
	if( $_REQUEST["codsubacao"] ){
		array_push($filtro, " UPPER(sba.sbacod) ilike '%" . strtoupper($_REQUEST['codsubacao']) . "%' ");
		array_push($filtropi, " UPPER(pli.plicod) ilike '%" . strtoupper($_REQUEST['codsubacao']) . "%' ");
		array_push($filtrosubacao, " UPPER(sa.sbacod) ilike '%" . strtoupper($_REQUEST['codsubacao']) . "%' ");
	}
	
	if( $_REQUEST["titulosubacao"] ){
		array_push($filtro, " UPPER(sba.sbatitulo) ilike '%" . strtoupper($_REQUEST['titulosubacao']) . "%' ");
		array_push($filtropi, " UPPER(pli.plititulo) ilike '%" . strtoupper($_REQUEST['titulosubacao']) . "%' ");
		array_push($filtrosubacao, " UPPER(sa.sbatitulo) ilike '%" . strtoupper($_REQUEST['titulosubacao']) . "%' ");
	}
	
	if( $_REQUEST["acaid"][0] ){
		$aux = implode(",", $_REQUEST['acaid'] );  
		array_push($filtro, " ptr.acaid in (" . $aux . ") ");
		array_push($filtropi, " ptr.acaid in (" . $aux . ") ");
		array_push($filtrosubacao, " ptr.acaid in (" . $aux . ") ");
	}

	if( $_REQUEST["ungcod"][0] ){
		$aux = "'" . implode("', '", $_REQUEST['ungcod']) . "'" ;  
		//array_push($filtro, " e.entid in (" . $aux . ") ");
		array_push($filtro, " u.ungcod in(" . $aux . ") ");
	}
	else{
		$stWhere = "";
		if(!$db->testa_superuser() && !possui_perfil(PERFIL_UNIDMONITORAAVALIA, PERFIL_EQUIPAPOIOGESTORUP)){
			$arUngcodTemp = perfil_unidade();
			$arUngcodTemp = ($arUngcodTemp) ? $arUngcodTemp : array();
			if($arUngcodTemp[0]){
				$arUngcod = array();
				foreach($arUngcodTemp as $uniid){
					if($uniid['ungcod']){
						array_push($arUngcod,$uniid['ungcod']);								
					}
				}
				if($arUngcod && $arUngcod[0]){
					$ungcodTemp = "'" . implode("', '",$arUngcod) . "'";
					if( $ungcodTemp ){
						array_push($filtro, " u.ungcod in(" . $ungcodTemp . ") ");
					}
				} else { ?>
					<script>
						alert('Não exite unidade vinculada para seu perfil. \nPor favor, contate o administrador do sistema.');
						history.back();
					</script>
					<?
					exit;
				}
			}
		}
	}
	
	if( $_REQUEST["pieid"] ){
		array_push($filtro, " sba.pieid = " . $_REQUEST['pieid'] . " ");
	}
	
	if( $_REQUEST["pigid"] ){
		array_push($filtro, " sba.pigid = " . $_REQUEST['pigid'] . " ");
	}
	
	if( $_REQUEST["eqdid"] ){
		array_push($filtro, " pi.eqdid = " . $_REQUEST['eqdid'] . " ");
	}
	
	if( $_REQUEST["neeid"] ){
		array_push($filtro, " pi.neeid = " . $_REQUEST["neeid"] . " ");
	}
	
	if( $_REQUEST["capid"] ){
		array_push($filtro, " pi.capid = " . $_REQUEST["capid"] . " ");
		array_push($filtropi, " pli.capid = " . $_REQUEST["capid"] . " ");
	}

	if( $_REQUEST["mdeid"] ){
		array_push($filtro, " pi.mdeid = " . $_REQUEST["mdeid"] . " ");
		array_push($filtropi, " pli.mdeid = " . $_REQUEST["mdeid"] . " ");
	}

	if( $_REQUEST["plilivre"] ){
		array_push($filtro, " pi.plilivre = '" . $_REQUEST["plilivre"] . "' ");
		array_push($filtropi, " pli.plilivre = '" . $_REQUEST["plilivre"] . "' ");
	}
	
	if( $_REQUEST["plisituacao"] ){
		array_push($filtro, " pi.plisituacao = '" . $_REQUEST['plisituacao'] . "' ");
		array_push($filtropi, " pli.plisituacao = '" . $_REQUEST['plisituacao'] . "' ");
	}
	
}

if( $_REQUEST['excel'] )
{
	function mostraDadosDetalhamentoOrcamentarioExcel($subacao, $codigo, $filtropi, $html) {
		global $db;
		
		$sql = "SELECT
				sd.sbaid,
				sa.sbacod,
				ptr.ptrid,
				ptr.ptres,
				trim(aca.prgcod||'.'||aca.acacod||'.'||aca.unicod||'.'||aca.loccod||' - '||aca.acadsc) as descricao,
				uni.unidsc,
				SUM(ptr.ptrdotacao) as dotacaoinicial,
				coalesce(cast(SUM(dt.valor) as varchar),'0.00') as detalhamento,
				coalesce(cast(SUM(dt2.valorpi) as varchar),'0.00') as detalhamento2,
				(SUM(ptr.ptrdotacao)-coalesce(SUM(dt.valor),0.00)) as diferenca,
				sum(sd.sadvalor) as valorcadastrado 	
			  FROM planejamento.acao aca 
				inner JOIN monitora.ptres ptr ON aca.acaid = ptr.acaid
				inner JOIN ( select sbaid, ptrid, sadvalor from monitora.pi_subacaodotacao ) sd ON ptr.ptrid = sd.ptrid
				inner join monitora.pi_subacao sa ON sa.sbaid = sd.sbaid
				inner JOIN public.unidade uni ON uni.unicod = ptr.unicod 
				LEFT JOIN ( select ptrid,
							sum( sadvalor ) as valor
						from  monitora.pi_subacaodotacao
						group by ptrid ) dt ON ptr.ptrid = dt.ptrid 
				LEFT JOIN ( select sbaid, ptrid, sum(dtl.valorpi) as valorpi from monitora.v_pi_detalhepiptres dtl group by sbaid, dtl.ptrid ) dt2 ON ptr.ptrid = dt2.ptrid and dt2.sbaid = sd.sbaid
				WHERE aca.prgano='".$_SESSION['exercicio']."' and aca.acasnrap = false
				and sd.sbaid = ".$subacao."
			  	and aca.unicod IN('".AD."','".CAPES."','".INEP."','".FNDE."','".FIES."','".SUPERVISAOMEC."')
			  	" . ( !empty($filtrosubacao) ? "AND" . implode(" AND ", $filtrosubacao) : '' ) . "
			    GROUP BY sd.sbaid,sa.sbacod,ptr.ptrid,ptr.ptres,descricao,uni.unidsc
		  		ORDER BY 1
			  ";
		$arPtresAcao = $db->carregar($sql);
		
		if($arPtresAcao[0]) {
			$html .= '<tr>
						<td colspan="6">
							<table border="1"><tr><td colspan="6"><strong>Detalhamento Orçamentário da Subação: '.$codigo.'</strong></td></tr></table>
						</td>
					</tr>
					<tr>
						<td colspan="6">
							<table border="1">
								<tr>
									<td bgcolor="#c0c0c0"><strong>PTRES</strong></td>
									<td bgcolor="#c0c0c0"><strong>Ação</strong></td>
									<td bgcolor="#c0c0c0"><strong>Dotação Autorizada</strong></td>
									<td bgcolor="#c0c0c0"><strong>Detalhado em SubAção</strong></td>
									<td bgcolor="#c0c0c0"><strong>Detalhado no PI</strong></td>
									<td bgcolor="#c0c0c0"><strong>Dotação Disponível</strong></td>
									<td bgcolor="#c0c0c0"><strong>Valor Previsto(Anual)</strong></td>
								</tr>';
			
			$vArrayDados = array();
			$valortotal = 0;
			
			foreach($arPtresAcao as $ptresacao)
			{
				$html .= "<tr>
							<td>".$ptresacao["ptres"]."</td>
							<td>".$ptresacao["descricao"]."</td>
							<td>".number_format($ptresacao["dotacaoinicial"],2,',','.')."</td>
							<td>".number_format($ptresacao["detalhamento"],2,',','.')."</td>
							<td>".number_format($ptresacao["detalhamento2"],2,',','.')."</td>
							<td>".number_format(($ptresacao["dotacaoinicial"]-$ptresacao["detalhamento"]),2,',','.') ."</td>
							<td>".number_format($ptresacao["valorcadastrado"],2,',','.')."</td>
						</tr>";
				$valortotal = $valortotal + $ptresacao['valorcadastrado'];
			}
				
			$html .= "<tr>
						 <td bgcolor=\"#c0c0c0\" align=\"right\" colspan=\"6\"><b>TOTAL:</b></td>
						 <td bgcolor=\"#c0c0c0\" align=\"right\">".number_format($valortotal,2,',','.')."</td>
					   </tr>";
				
				
			$html .= "</table></td></tr>";
		}
	}
	
	function mostraDadosPISubacaoExcel($subacao, $codigo, $filtropi, $html) {
		global $db;
		
		$sqlpi = "SELECT
					pli.pliid AS pi,
					pli.plicod AS codigo,
					pli.plititulo AS titulo,
					pli.plidsc AS descricao,
					ptr.prgcod AS programa,
					ptr.acacod AS acao,
					ptr.ptres AS ptres
				FROM
					monitora.pi_planointerno pli
				INNER JOIN
					monitora.pi_planointernoptres pip ON pip.pliid = pli.pliid
				INNER JOIN
					monitora.ptres ptr ON ptr.ptrid = pip.ptrid
				WHERE
					pli.sbaid = {$subacao} " . ( !empty($filtropi) ? "AND" . implode(" AND ", $filtropi) : '' ) . "
					AND ptr.ptrano = '".$_SESSION["exercicio"]."'";
		$dadosPI = $db->carregar($sqlpi);
		
		if($dadosPI) {
			$html .= '<tr>
						<td colspan="6">
							<table border="1"><tr><td colspan="6"><strong>Dados dos PI\'s da Subação: '.$codigo.'</strong></td></tr></table>
						</td>
					</tr>
					<tr>
						<td colspan="6">
							<table border="1">
								<tr>
									<td bgcolor="#c0c0c0"><strong>Código</strong></td>
									<td bgcolor="#c0c0c0"><strong>Título</strong></td>
									<td bgcolor="#c0c0c0"><strong>Descrição</strong></td>
									<td bgcolor="#c0c0c0"><strong>Programas</strong></td>
									<td bgcolor="#c0c0c0"><strong>Ações</strong></td>
									<td bgcolor="#c0c0c0"><strong>Ptres</strong></td>
								</tr>';
			
			$vArrayDados = array();
			
			for($i=0; $i<=count($dadosPI); $i++)
			{
				$html .= "<tr>
							<td>".$dadosPI[$i]["codigo"]."</td>
							<td>".$dadosPI[$i]["titulo"]."</td>
							<td>".$dadosPI[$i]["descricao"]."</td>
							<td>".$dadosPI[$i]["programa"]."</td>
							<td>".$dadosPI[$i]["acao"]."</td>
							<td>".$dadosPI[$i]["ptres"]."</td>
						</tr>";
			}
		
			$html .= "</table></td></tr>";
		}
	}
	
	$sql = "SELECT
				sba.sbaid AS subacao,
				sba.sbacod AS codigo,
				sba.sbatitulo AS titulo,
				sba.sbadsc AS descricao,
				ptr.prgcod AS programa,
				ptr.acacod AS acao,
				ptr.ptres AS ptres
			FROM
				monitora.pi_subacao sba
			INNER JOIN
				monitora.pi_subacaodotacao sbd ON sbd.sbaid = sba.sbaid
			INNER JOIN
				monitora.ptres ptr ON ptr.ptrid = sbd.ptrid
			LEFT JOIN
				monitora.pi_planointerno pi ON pi.sbaid = sba.sbaid
			LEFT JOIN
				monitora.pi_subacaounidade su ON su.sbaid = sba.sbaid
			LEFT JOIN
				public.unidadegestora u on su.ungcod = u.ungcod								
			WHERE
				sba.sbastatus = 'A' " . ( !empty($filtro) ? "AND" . implode(" AND ", $filtro) : '' ) . "
			ORDER BY
				sba.sbaid";
	$dadosSubAcao = $db->carregar($sql);
	
	$html = '<table border="1">
				<tr>
					<td align="center" bgcolor="gray"><strong>Código</strong></td>
					<td align="center" bgcolor="gray"><strong>Título</strong></td>
					<td align="center" bgcolor="gray"><strong>Descrição</strong></td>
					<td align="center" bgcolor="gray"><strong>Programas</strong></td>
					<td align="center" bgcolor="gray"><strong>Ações</strong></td>
					<td align="center" bgcolor="gray"><strong>Ptres</strong></td>
				</tr>';
	
	$vArrayDados = array();
	
	for($i=0; $i<=count($dadosSubAcao); $i++)
	{
		if( empty($vArrayDados) ) {
			$vArrayDados["subacao"] = $dadosSubAcao[$i]["subacao"];
			$vArrayDados["codigo"] = $dadosSubAcao[$i]["codigo"];
			$vArrayDados["titulo"] = $dadosSubAcao[$i]["titulo"];
			$vArrayDados["descricao"] = $dadosSubAcao[$i]["descricao"];
			
			$vArrayDados["programa"][0] = $dadosSubAcao[$i]["programa"];
			$vArrayDados["acao"][0] = $dadosSubAcao[$i]["acao"];
			$vArrayDados["ptres"][0] = $dadosSubAcao[$i]["ptres"];
		} 
		else {
			if( $dadosSubAcao[$i]["subacao"] != $vArrayDados["subacao"] )
			{
				$html.= "<tr>
							<td bgcolor=\"#e9e9e9\">".$vArrayDados["codigo"]."</td>
							<td bgcolor=\"#e9e9e9\">".$vArrayDados["titulo"]."</td>
							<td bgcolor=\"#e9e9e9\">".$vArrayDados["descricao"]."</td>
							<td bgcolor=\"#e9e9e9\">".implode(" - ", $vArrayDados["programa"])."</td>
							<td bgcolor=\"#e9e9e9\">".implode(" - ", $vArrayDados["acao"])."</td>
							<td bgcolor=\"#e9e9e9\">".implode(" - ", $vArrayDados["ptres"])."</td>
						  </tr>";
				
				mostraDadosDetalhamentoOrcamentarioExcel($vArrayDados["subacao"], $vArrayDados["codigo"], $filtropi, &$html);
				
				if($_POST['pi'])
					mostraDadosPISubacaoExcel($vArrayDados["subacao"], $vArrayDados["codigo"], $filtropi, &$html);
				
				$vArrayDados = array();
				$vArrayDados["subacao"] = $dadosSubAcao[$i]["subacao"];
				$vArrayDados["codigo"] = $dadosSubAcao[$i]["codigo"];
				$vArrayDados["titulo"] = $dadosSubAcao[$i]["titulo"];
				$vArrayDados["descricao"] = $dadosSubAcao[$i]["descricao"];
				
				$vArrayDados["programa"][0] = $dadosSubAcao[$i]["programa"];
				$vArrayDados["acao"][0] = $dadosSubAcao[$i]["acao"];
				$vArrayDados["ptres"][0] = $dadosSubAcao[$i]["ptres"];
			}
			else {
				$vArrayDados["subacao"] = $dadosSubAcao[$i]["subacao"];
				$vArrayDados["codigo"] = $dadosSubAcao[$i]["codigo"];
				$vArrayDados["titulo"] = $dadosSubAcao[$i]["titulo"];
				$vArrayDados["descricao"] = $dadosSubAcao[$i]["descricao"];
				
				if( !in_array($dadosSubAcao[$i]["programa"], $vArrayDados["programa"]) )
					array_push($vArrayDados["programa"], $dadosSubAcao[$i]["programa"]);
					
				if( !in_array($dadosSubAcao[$i]["acao"], $vArrayDados["acao"]) )
					array_push($vArrayDados["acao"], $dadosSubAcao[$i]["acao"]);
					
				if( !in_array($dadosSubAcao[$i]["ptres"], $vArrayDados["ptres"]) )
					array_push($vArrayDados["ptres"], $dadosSubAcao[$i]["ptres"]);
			}
		}
	}
	
	$html.= "</table>";
	
	header("Content-type: application/vnd.ms-excel");
	header("Content-type: application/force-download");
	header("Content-Disposition: attachment; filename=relatorio.xls");
	header("Pragma: no-cache");
	
	echo $html;
	die;
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );

?>
<script>
 	
 	function filtra_pi(tipo){
 		var form = document.getElementById("formulario");
		var excel = document.getElementById('excel');
		
		if(tipo != '') excel.value = '1';
		else excel.value = '';
 		
		selectAllOptions( form.ungcod );
		selectAllOptions( form.acaid );
		 		
 		form.submit();
 	}

 	function detfin(ptrid,sbaid)
 	{
 		if(sbaid)
 	 	{
 			janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUG/detalhafinanceiro_subacaoUG&acao=A&ptrid='+ptrid+'&sbaid='+sbaid, 'janela2', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width='+(screen.width-420)+',height=280' ); janela.focus();
 		}
 	}
 	
</script>
<form id="formulario" name="formulario" method="post" action="">
	<input type=hidden name="requisicao" value="1">
	<input type=hidden name="excel" id="excel" value="">
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"	align="center">
		<tr>
			<td bgcolor="#CCCCCC" colspan="2"><b>Filtros do Relatório</b></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Código Subação:</td>
			<td>
				<?php 
					$codsubacao = $_REQUEST['codsubacao'];
					echo campo_texto( 'codsubacao', 'N', 'S', '', 5, 4, '', '', 'left', '', 0);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Título Subação:</td>
			<td>
				<?php
					$titulosubacao = $_REQUEST['titulosubacao'];
					echo campo_texto( 'titulosubacao', 'N', 'S', '', 65, 60, '', '', 'left', '', 0);
				?>
			</td>
		</tr>
		
			<tr>
				<td class="SubTituloDireita" valign="top">Unidades Orçamentárias:</td>
				<td>
				<?php
					$stWhere = "";
					
					 if(!$db->testa_superuser() && !possui_perfil(PERFIL_UNIDMONITORAAVALIA) && !possui_perfil(PERFIL_MONITORA_GESTORSUBACOES)){
						$arUngcodTemp = perfil_unidade();
						$arUngcodTemp = ($arUngcodTemp) ? $arUngcodTemp : array();
						if($arUngcodTemp[0]){
							$arUngcod = array();
							foreach($arUngcodTemp as $uniid){
								if($uniid['ungcod']){
									array_push($arUngcod,$uniid['ungcod']);								
								}
							}
							if($arUngcod && $arUngcod[0]){
								$ungcodTemp = "'" . implode("', '",$arUngcod) . "'";
								if( $ungcodTemp ){
									$stWhere = " AND ungcod in(" . $ungcodTemp . ") ";
								}
							}
						}
					}
				
					if ( $_REQUEST['ungcod'] && $_REQUEST['ungcod'][0] != '' )
					{
						$sql_carregados = "SELECT
										 		ungcod as codigo,
												CASE WHEN (unicod = '26101') THEN ungcod ELSE unicod END ||' - '|| ungdsc as descricao
										   FROM
										   		public.unidadegestora
										   WHERE
										   		ungstatus = 'A' and 
										   		ungcod in ('".implode("','", $_REQUEST['ungcod'])."')";
						$ungcod = $db->carregar( $sql_carregados );
					}
					
					//$ungcod = $_REQUEST['ungcod'];
					$sqlComboEntidade = "SELECT 
								ungcod AS codigo, CASE WHEN (unicod = '26101') THEN ungcod ELSE unicod END ||' - '|| ungdsc as descricao FROM public.unidadegestora where ungstatus = 'A' $stWhere ";  
				
					combo_popup( "ungcod", $sqlComboEntidade, "Unidades Orçamentárias", "600x600", 0, array(), "", "S", false, false, 5, 400 );				
				?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top">Ações:</td>
			    <td>
					<?
					if($_POST['acaid'][0] && !$_POST['ordemlista']) {
						$acaid = $db->carregar("SELECT
													aca.acaid as codigo,
													'(PTRES:'||pt.ptres||') - '|| aca.unicod ||'.'|| aca.prgcod ||'.'|| aca.acacod ||' - '|| aca.acadsc as descricao
												  FROM planejamento.acao aca 
												  	inner join monitora.ptres pt on pt.acaid=aca.acaid  
												WHERE aca.prgano='".$_SESSION['exercicio']."' and aca.acasnrap = false
												and aca.unicod IN('".AD."','".CAPES."','".INEP."','".FNDE."')
												and aca.acaid in (".implode(",", $_POST['acaid']).")
												GROUP BY codigo , descricao
												ORDER BY 
													1");
					}
					$sqlComboAcoes = "SELECT
										aca.acaid as codigo,
										'(PTRES:'||pt.ptres||') - '|| aca.unicod ||'.'|| aca.prgcod ||'.'|| aca.acacod ||' - '|| aca.acadsc as descricao
									  FROM planejamento.acao aca 
									  	inner join monitora.ptres pt on pt.acaid=aca.acaid
									  WHERE aca.prgano='".$_SESSION['exercicio']."' and aca.acasnrap = false
									  	and aca.unicod IN('".AD."','".CAPES."','".INEP."','".FNDE."')
									  GROUP BY codigo , descricao
									  ORDER BY 
										1";
					combo_popup( "acaid", $sqlComboAcoes, "Ações", "600x600", 0, array(), "", "S", false, false, 5, 400 );
					?>    	
			    </td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Executor Orçamentário e Financeiro:</td>
			    <td>
			    	
					<?
					$pieid = $_REQUEST['pieid'];
					$sql = "SELECT pieid as codigo, piecod ||' - '|| piedsc as descricao FROM  monitora.pi_executor";
					$db->monta_combo('pieid', $sql, 'S', 'Selecione', '', '', '', '100', 'N', '000000003_0'); 
					?>    	
			    </td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Gestor da Subação:</td>
			    <td>
			    	
					<?
					$pigid = $_REQUEST['pigid'];
					$sql = "SELECT pigid as codigo, pigcod ||' - '|| pigdsc as descricao FROM  monitora.pi_gestor";  
					$db->monta_combo('pigid', $sql, 'S', 'Selecione', '', '', '', '100', 'N', '000000003_1'); 
					?>    	
			    </td>
			</tr>
		
		
				<tr>
					<td class='SubTituloDireita'>Enquadramento da Despesa:</td>
					<td>
						<?
						$eqdid = $_REQUEST['eqdid'];
						$sql = "SELECT eqdid as codigo, eqdcod ||' - '|| eqddsc as descricao FROM planejamento.enquadramento_despesa WHERE eqdano = '{$_SESSION['exercicio']}' ORDER BY eqdcod";
						
						$db->monta_combo('eqdid', $sql, 'S', 'Selecione', '', '', '', '240', 'N', '000000005_0'); 
						?>
					</td>
				</tr>
				<tr>
					<td class='SubTituloDireita'>Nível/Etapa de Ensino:</td>
					<td>
						<?
						$sql = "SELECT neeid as codigo, neecod ||' - '|| needsc as descricao FROM monitora.pi_niveletapaensino WHERE neeano = '{$_SESSION['exercicio']}' ORDER BY neecod ";						
						$db->monta_combo('neeid', $sql, 'S', 'Selecione', '', '', '', '240', 'N', '000000005_1');
						?>
					</td>
				</tr>
				<tr>
					<td class='SubTituloDireita'>Categoria de Apropriação:</td>
					<td>
						<?
						$capid = $_REQUEST['capid'];
						$sql = "SELECT capid as codigo, capcod ||' - '|| capdsc as descricao FROM monitora.pi_categoriaapropriacao WHERE capano = '{$_SESSION['exercicio']}' ORDER BY capcod ";
						$db->monta_combo('capid', $sql, 'S', 'Selecione', '', '', '', '340', 'N', '000000005_2');
						?>
					</td>
				</tr>	
				<tr>
					<td class="SubTituloDireita">Codifição da Unidade(livre):</td>
				    <td>
				    	<?$plilivre = $_REQUEST['plilivre']; ?>
				    	<?=campo_texto('plilivre','N','S','',3,2,'','',null,null,null,'','');?>
				    </td>
				</tr>
				
				<tr>
					<td align='right' class="SubTituloDireita">Modalidade de Ensino / Tema / Público:</td>
				    <td>
						<?
						$mdeid = $_REQUEST['mdeid'];
						$sql = "SELECT mdeid as codigo, mdecod ||' - '|| mdedsc as descricao FROM monitora.pi_modalidadeensino WHERE mdeano = '{$_SESSION['exercicio']}'";
						$db->monta_combo('mdeid', $sql, 'S', 'Selecione', '', '', '', '240', 'N', '000000005_3');
			    	 	?>
				    </td>
				</tr>
				
		
		<tr>
			<td class="SubTituloDireita">Situação:</td>
			<td>
				<input type="radio" name="plisituacao" id="plisituacao" value="" <?if($_REQUEST["plisituacao"] == '') echo 'checked';?>> Qualquer
				<input type="radio" name="plisituacao" id="plisituacao" value="A" <?if($_REQUEST["plisituacao"] == 'A') echo 'checked';?>> Aprovado
				<input type="radio" name="plisituacao" id="plisituacao" value="H" <?if($_REQUEST["plisituacao"] == 'H') echo 'checked';?>> Homologado
				<input type="radio" name="plisituacao" id="plisituacao" value="E" <?if($_REQUEST["plisituacao"] == 'E') echo 'checked';?>> Enviado para Revisão
				<input type="radio" name="plisituacao" id="plisituacao" value="C" <?if($_REQUEST["plisituacao"] == 'C') echo 'checked';?>> Cadastrado no SIAFI
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Mostrar Planos Internos:</td>
			<td><input type="checkbox" name="pi" value="1" /></td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td></td>
			<td>
				<input type="button" value="Pesquisar" onclick="filtra_pi('');" style="cursor: pointer;"/>
				<input type="button" value="Gerar XLS" onclick="filtra_pi('excel');" style="cursor: pointer;"/>
			</td>
		</tr>	
	</table>
	
</form>
<?
if ( $_REQUEST['requisicao'] == 1 ){

		$sql = "SELECT
										sba.sbaid AS subacao,
										sba.sbacod AS codigo,
										sba.sbatitulo AS titulo,
										sba.sbadsc AS descricao,
										ptr.prgcod AS programa,
										ptr.acacod AS acao,
										ptr.ptres AS ptres
									FROM
										monitora.pi_subacao sba
									INNER JOIN
										monitora.pi_subacaodotacao sbd ON sbd.sbaid = sba.sbaid
									INNER JOIN
										monitora.ptres ptr ON ptr.ptrid = sbd.ptrid
									LEFT JOIN
										monitora.pi_planointerno pi ON pi.sbaid = sba.sbaid
									LEFT JOIN
										monitora.pi_subacaounidade su ON su.sbaid = sba.sbaid
									LEFT JOIN
										public.unidadegestora u on su.ungcod = u.ungcod								
									WHERE
										sba.sbastatus = 'A' " . ( !empty($filtro) ? "AND" . implode(" AND ", $filtro) : '' ) . "
									ORDER BY
										sba.sbaid";
		//ver($sql,d);
		$dadosSubAcao = $db->carregar($sql);
		
		echo "<script type=\"text/javascript\">
				function exibeDadosPI(id, tipoacao) {
					var janela = window.open( '?modulo=principal/planotrabalhoUG/popuphistoricoplanointernoUG&acao=A&pliid=' + id + '&tipoacao=' + tipoacao, 
							 			  'DadosdoPI','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=500');
					janela.focus();
				}
			</script>
			<table width=\"95%\" align=\"center\" border=\"0\" cellspacing=\"2\" cellpadding=\"2\" class=\"listagem\">
				<thead>
					<tr>
						<td width=\"5%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Código</strong></td>
						<td width=\"20%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Título</strong></td>
						<td width=\"20%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Descrição</strong></td>
						<td width=\"15%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Programas</strong></td>
						<td width=\"15%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Ações</strong></td>
						<td width=\"25%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Ptres</strong></td>
					</tr>
				  </thead>
				  <tbody>";
		
		$vArrayDados = array();
		$cont = 1;
		for($i=0; $i<=count($dadosSubAcao); $i++) {
			if( empty($vArrayDados) ) {
				$vArrayDados["subacao"] = $dadosSubAcao[$i]["subacao"];
				$vArrayDados["codigo"] = $dadosSubAcao[$i]["codigo"];
				$vArrayDados["titulo"] = $dadosSubAcao[$i]["titulo"];
				$vArrayDados["descricao"] = $dadosSubAcao[$i]["descricao"];
				
				$vArrayDados["programa"][0] = $dadosSubAcao[$i]["programa"];
				$vArrayDados["acao"][0] = $dadosSubAcao[$i]["acao"];
				$vArrayDados["ptres"][0] = $dadosSubAcao[$i]["ptres"];
			} 
			else {
				if( $dadosSubAcao[$i]["subacao"] != $vArrayDados["subacao"] ) {
					$cont++;
					$cor = ($cont % 2) ? "#e0e0e0" : "#f4f4f4"; 
					
					echo "<tr bgcolor='".$cor."';\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".$cor."';\">
							<td align=\"center\">".$vArrayDados["codigo"]."</td>
							<td>".$vArrayDados["titulo"]."</td>
							<td>".$vArrayDados["descricao"]."</td>
							<td>".implode(" - ", $vArrayDados["programa"])."</td>
							<td>".implode(" - ", $vArrayDados["acao"])."</td>
							<td>".implode(" - ", $vArrayDados["ptres"])."</td>
						  </tr>";
					
					//ver($vArrayDados["subacao"],d);
					
					mostraDadosDetalhamentoOrcamentario($vArrayDados["subacao"], $vArrayDados["codigo"], $filtropi);
					
					if($_POST['pi'])
						mostraDadosPISubacao($vArrayDados["subacao"], $vArrayDados["codigo"], $filtropi);
					
					$vArrayDados = array();
					$vArrayDados["subacao"] = $dadosSubAcao[$i]["subacao"];
					$vArrayDados["codigo"] = $dadosSubAcao[$i]["codigo"];
					$vArrayDados["titulo"] = $dadosSubAcao[$i]["titulo"];
					$vArrayDados["descricao"] = $dadosSubAcao[$i]["descricao"];
					
					$vArrayDados["programa"][0] = $dadosSubAcao[$i]["programa"];
					$vArrayDados["acao"][0] = $dadosSubAcao[$i]["acao"];
					$vArrayDados["ptres"][0] = $dadosSubAcao[$i]["ptres"];
				}
				else {
					$vArrayDados["subacao"] = $dadosSubAcao[$i]["subacao"];
					$vArrayDados["codigo"] = $dadosSubAcao[$i]["codigo"];
					$vArrayDados["titulo"] = $dadosSubAcao[$i]["titulo"];
					$vArrayDados["descricao"] = $dadosSubAcao[$i]["descricao"];
					
					if( !in_array($dadosSubAcao[$i]["programa"], $vArrayDados["programa"]) )
						array_push($vArrayDados["programa"], $dadosSubAcao[$i]["programa"]);
						
					if( !in_array($dadosSubAcao[$i]["acao"], $vArrayDados["acao"]) )
						array_push($vArrayDados["acao"], $dadosSubAcao[$i]["acao"]);
						
					if( !in_array($dadosSubAcao[$i]["ptres"], $vArrayDados["ptres"]) )
						array_push($vArrayDados["ptres"], $dadosSubAcao[$i]["ptres"]);
				}
			}
		}
		
		echo "</thead>
			  <tfoot>
			  	<tr bgcolor=\"#C0C0C0\">
			  		<td colspan=\"6\">&nbsp;</td>
			  	</tr>
			  </tfoot>
			</table>";
		
		
		
		
} //fecha if ( $_REQUEST['requisicao'] == 1 )


function mostraDadosPISubacao($subacao, $codigo, $filtropi) {
	global $db;
	
	$sqlpi = "SELECT
								pli.pliid AS pi,
								pli.plicod AS codigo,
								pli.plititulo AS titulo,
								pli.plidsc AS descricao,
								ptr.prgcod AS programa,
								ptr.acacod AS acao,
								ptr.ptres AS ptres
							FROM
								monitora.pi_planointerno pli
							INNER JOIN
								monitora.pi_planointernoptres pip ON pip.pliid = pli.pliid
							INNER JOIN
								monitora.ptres ptr ON ptr.ptrid = pip.ptrid
							WHERE
								pli.sbaid = {$subacao} " . ( !empty($filtropi) ? "AND" . implode(" AND ", $filtropi) : '' ) . "
								AND ptr.ptrano = '".$_SESSION["exercicio"]."'";
	$dadosPI = $db->carregar($sqlpi);
	
	if($dadosPI) {
		echo "<tr>
				<td colspan=\"6\">
					<strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Dados dos PI's da Subação: ".$codigo."</strong>
					<table width=\"95%\" align=\"center\" border=\"0\" cellspacing=\"2\" cellpadding=\"2\" class=\"listagem\">
					<thead>
						<tr>
							<td width=\"5%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Código</strong></td>
							<td width=\"40%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Título</strong></td>
							<td width=\"40%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Descrição</strong></td>
							<td width=\"5%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Programas</strong></td>
							<td width=\"5%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Ações</strong></td>
							<td width=\"5%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Ptres</strong></td>
						</tr>
					  </thead>
					  <tbody>";
		
		$vArrayDados = array();
		$cont = 1;
		for($i=0; $i<=count($dadosPI); $i++) {
			$cont++;
			$cor = ($cont % 2) ? "#e0e0e0" : "#f4f4f4"; 
			
			echo  "<tr bgcolor='".$cor."';\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".$cor."';\">
						<td align=\"center\"><a href=\"javascript:void(0);\" onclick=\"exibeDadosPI('".$dadosPI[$i]["pi"]."', 'consulta');\">".$dadosPI[$i]["codigo"]."</a></td>
						<td>".$dadosPI[$i]["titulo"]."</td>
						<td>".$dadosPI[$i]["descricao"]."</td>
						<td>".$dadosPI[$i]["programa"]."</td>
						<td>".$dadosPI[$i]["acao"]."</td>
						<td>".$dadosPI[$i]["ptres"]."</td>
					</tr>";
		}
	
		echo "</tbody>
			 </table>
	   	    </td>
	 	   </tr>";
	}
}

function mostraDadosDetalhamentoOrcamentario($subacao, $codigo, $filtropi) {
	global $db;
	
	$sql = "SELECT
						sd.sbaid,
						sa.sbacod,
						ptr.ptrid,
						ptr.ptres,
						trim(aca.prgcod||'.'||aca.acacod||'.'||aca.unicod||'.'||aca.loccod||' - '||aca.acadsc) as descricao,
						uni.unidsc,
						SUM(ptr.ptrdotacao) as dotacaoinicial,
						coalesce(cast(SUM(dt.valor) as varchar),'0.00') as detalhamento,
						coalesce(cast(SUM(dt2.valorpi) as varchar),'0.00') as detalhamento2,
						(SUM(ptr.ptrdotacao)-coalesce(SUM(dt.valor),0.00)) as diferenca,
						sum(sd.sadvalor) as valorcadastrado 	
					  FROM planejamento.acao aca 
						inner JOIN monitora.ptres ptr ON aca.acaid = ptr.acaid
						inner JOIN ( select sbaid, ptrid, sadvalor from monitora.pi_subacaodotacao ) sd ON ptr.ptrid = sd.ptrid
						inner join monitora.pi_subacao sa ON sa.sbaid = sd.sbaid
						inner JOIN public.unidade uni ON uni.unicod = ptr.unicod 
						LEFT JOIN ( select ptrid,
									sum( sadvalor ) as valor
								from  monitora.pi_subacaodotacao
								group by ptrid ) dt ON ptr.ptrid = dt.ptrid 
						LEFT JOIN ( select sbaid, ptrid, sum(dtl.valorpi) as valorpi from monitora.v_pi_detalhepiptres dtl group by sbaid, dtl.ptrid ) dt2 ON ptr.ptrid = dt2.ptrid and dt2.sbaid = sd.sbaid
						WHERE aca.prgano='".$_SESSION['exercicio']."' and aca.acasnrap = false
						and sd.sbaid = ".$subacao."
					  	and aca.unicod IN('".AD."','".CAPES."','".INEP."','".FNDE."','".FIES."','".SUPERVISAOMEC."')
					  	" . ( !empty($filtrosubacao) ? "AND" . implode(" AND ", $filtrosubacao) : '' ) . "
					    GROUP BY sd.sbaid,sa.sbacod,ptr.ptrid,ptr.ptres,descricao,uni.unidsc
				  		ORDER BY 1
					  ";
	//ver($sql,d);
	$arPtresAcao = $db->carregar($sql);
	if($arPtresAcao[0]) {
		echo "<tr>
				<td colspan=\"6\">
					<strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Detalhamento Orçamentário da Subação: ".$codigo."</strong>
					<table width=\"95%\" align=\"center\" border=\"0\" cellspacing=\"2\" cellpadding=\"2\" class=\"listagem\">
					<thead>
						<tr>
							<td width=\"5%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>PTRES</strong></td>
							<td width=\"50%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Ação</strong></td>
							<td width=\"10%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Dotação Autorizada</strong></td>
							<td width=\"10%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Detalhado em SubAção</strong></td>
							<td width=\"10%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Detalhado no PI</strong></td>
							<td width=\"10%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Dotação Disponível</strong></td>
							<td width=\"10%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Valor Previsto(Anual)</strong></td>
						</tr>
					  </thead>
					  <tbody>";
		
		$vArrayDados = array();
		$cont = 1;
		$valortotal = 0;
		foreach($arPtresAcao as $ptresacao){
			$cont++;
			$cor = ($cont % 2) ? "#e0e0e0" : "#f4f4f4"; 
			
			echo  "<tr bgcolor='".$cor."';\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".$cor."';\">
						<td align=\"center\">".$ptresacao["ptres"]."</td>
						<td align=\"left\"><a href=\"javascript:void(0);\" onclick=\"exibeDadosPI('".$ptresacao["pi"]."', 'consulta');\">".$ptresacao["descricao"]."</a></td>
						<td align=\"right\">".number_format($ptresacao["dotacaoinicial"],2,',','.')."</td>
						<td align=\"right\"><a href=\"javascript:detfin('".$ptresacao['ptrid']."', '".$ptresacao['sbaid']."');\">".number_format($ptresacao["detalhamento"],2,',','.')."</a></td>
						<td align=\"right\">".number_format($ptresacao["detalhamento2"],2,',','.')."</td>
						<td align=\"right\">".number_format(($ptresacao["dotacaoinicial"]-$ptresacao["detalhamento"]),2,',','.') ."</td>
						<td align=\"right\">".number_format($ptresacao["valorcadastrado"],2,',','.')."</td>
					</tr>";
			$valortotal = $valortotal + $ptresacao['valorcadastrado'];
		}
			
			echo  "<tr bgcolor='#e0e0e0'>
					   <td align=\"right\" colspan=\"6\"><b>TOTAL:</b></td>
					   <td align=\"right\">".number_format($valortotal,2,',','.')."</td>
				   </tr>";
			
			
		echo "</tbody>
			 </table>
	   	    </td>
	 	   </tr>";
	}
}

?>