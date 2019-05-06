<?

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");


define("AD"   ,26101);
define("CAPES",26291);
define("INEP" ,26290);
define("FNDE" ,26298);

include APPRAIZ . 'monitora/www/planotrabalho/_constantes.php';

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
	
	if( $_REQUEST["codsubacao"] ){
		array_push($filtro, " UPPER(sba.sbacod) ilike '%" . strtoupper($_REQUEST['codsubacao']) . "%' ");
		array_push($filtropi, " UPPER(pli.plicod) ilike '%" . strtoupper($_REQUEST['codsubacao']) . "%' ");
	}
	
	if( $_REQUEST["titulosubacao"] ){
		array_push($filtro, " UPPER(sba.sbatitulo) ilike '%" . strtoupper($_REQUEST['titulosubacao']) . "%' ");
		array_push($filtropi, " UPPER(pli.plititulo) ilike '%" . strtoupper($_REQUEST['titulosubacao']) . "%' ");
	}
	
	if( $_REQUEST["acaid"][0] ){
		$aux = implode(",", $_REQUEST['acaid'] );  
		array_push($filtro, " pi.acaid in (" . $aux . ") ");
		array_push($filtropi, " pli.acaid in (" . $aux . ") ");
	}

	if( $_REQUEST["entid"][0] ){
		$aux = implode(",", $_REQUEST['entid'] );  
		array_push($filtro, " e.entid in (" . $aux . ") ");
	}
	else{
			
			
			if ( in_array(PERFIL_GESTORUNIDPLANEJAM, $perfil) ){
				$whereResp = " AND ur.pflcod IN (".PERFIL_GESTORUNIDPLANEJAM.") AND ur.usucpf = '".$_SESSION['usucpf']."' ";
			}
			
			if ( in_array(PERFIL_EQUIPAPOIOGESTORUP, $perfil) ){
				$whereResp = " AND ur.pflcod IN (".PERFIL_EQUIPAPOIOGESTORUP.") AND ur.usucpf = '".$_SESSION['usucpf']."' ";
			}	
			
			
							
			if($whereResp){
						$entid = array();
						
						$entid = $db->carregar("SELECT ent.entid as codigo,
										 			CASE fen.funid 
													when 34 then 
												   		ent.entungcod ||' - '|| ent.entnome 
												   	else 
												   		ent.entunicod ||' - '|| ent.entnome 
												   	end as descricao
											FROM entidade.entidade ent 
											INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
											INNER JOIN monitora.usuarioresponsabilidade ur ON ur.unicod = ent.entunicod or ur.ungcod = ent.entungcod
											WHERE ent.entid not in(391281) and ur.prsano = '".$_SESSION["exercicio"]."'
											$whereResp
											GROUP BY codigo, descricao, entunicod
											ORDER BY entunicod																		
											");	
						if($entid){
							foreach($entid as $entid2){
								$aux = 	",".$entid2['codigo'];
							}
							$aux2 = substr($aux, 1);
	
							array_push($filtro, " e.entid in (" . $aux2 . ") ");
						}
						else{?>
							<script>
								alert('Não exite unidade vinculada para seu perfil. \nPor favor, contate o administrador do sistema.');
								history.back();
							</script>
							<?
							exit;							
						}
			}
			
	
	}
	
	if( $_REQUEST["campo3"] ){
		array_push($filtro, " sba.exeid = " . $_REQUEST['campo3'] . " ");
	}
	
	if( $_REQUEST["campo4"] ){
		array_push($filtro, " sba.gstid = " . $_REQUEST['campo4'] . " ");
	}
	
	if( $_REQUEST["campo5"] ){
		array_push($filtro, " sba.eqdid = " . $_REQUEST['campo5'] . " ");
	}
	
	if( $_REQUEST["campo6"] ){
		//$posicao = strpos($_REQUEST["campo6"], '|');
		//$campo6 = substr($_REQUEST["campo6"], 0, $posicao);
		//array_push($filtro, " pl.dpiid = " . $campo6 . " ");
		array_push($filtro, " pi.dpiid = " . $_REQUEST["campo6"] . " ");
	}
	
	if( $_REQUEST["campo7"] ){
		//$posicao = strpos($_REQUEST["campo7"], '|');
		//$campo7 = substr($_REQUEST["campo7"], 0, $posicao);
		//array_push($filtro, " pl.cpiid = " . $campo7 . " ");
		array_push($filtro, " pi.cpiid = " . $_REQUEST["campo7"] . " ");
		array_push($filtropi, " pli.cpiid = " . $_REQUEST["campo7"] . " ");
	}

	if( $_REQUEST["campo8"] ){
		array_push($filtro, " pi.modid = " . $_REQUEST["campo8"] . " ");
		array_push($filtropi, " pli.modid = " . $_REQUEST["campo8"] . " ");
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



include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );

?>



<script>
 	
 	function filtra_pi(){
 		var form = document.getElementById("formulario");
 		
		selectAllOptions( form.entid );
		selectAllOptions( form.acaid );
		 		
 		form.submit();
 	}
 	
</script>

<form id="formulario" name="formulario" method="post" action="">
	<input type=hidden name="requisicao" value="1">
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"	align="center">
		<tr>
			<td  bgcolor="#CCCCCC" colspan="2"><b>Filtros do Relatório</b></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Código Subação:</td>
			<td>
				<?php 
					$codsubacao = $_REQUEST['codsubacao'];
					//echo campo_texto( 'plicod', 'N', 'S', '', 20, 15, '', '', 'left', '', 0);
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
				
					
					
					if ( in_array(PERFIL_GESTORUNIDPLANEJAM, $perfil) ){
						$whereResp = " AND ur.pflcod IN (".PERFIL_GESTORUNIDPLANEJAM.") AND ur.usucpf = '".$_SESSION['usucpf']."' ";
					}
					
					if ( in_array(PERFIL_EQUIPAPOIOGESTORUP, $perfil) ){
						$whereResp = " AND ur.pflcod IN (".PERFIL_EQUIPAPOIOGESTORUP.") AND ur.usucpf = '".$_SESSION['usucpf']."' ";
					}	
					
					
									
					if( ($_POST['entid'][0] && !$_POST['ordemlista']) || $whereResp){
							//$entidaux = $_REQUEST['entid'];
							if($_POST['entid'][0] && !$_POST['ordemlista']){
								$entid = $db->carregar("SELECT ent.entid as codigo,
												 			CASE fe.funid
														   	when 34 then 
														   		ent.entungcod ||' - '|| ent.entnome 
														   	else 
														   		ent.entunicod ||' - '|| ent.entnome 
														   	end as descricao
													FROM entidade.entidade ent 
													INNER JOIN monitora.usuarioresponsabilidade ur ON ur.unicod = ent.entunicod or ur.ungcod = ent.entungcod
													INNER JOIN entidade.funcaoentidade fe ON fe.entid = ent.entid
													WHERE ent.entid in (".implode(",", $_POST['entid']).")
													and ur.prsano = '".$_SESSION["exercicio"]."'
													$whereResp
													GROUP BY codigo, descricao, entunicod
													ORDER BY entunicod													
													");
							}else{
								$entid = $db->carregar("SELECT ent.entid as codigo,
												 			CASE fe.funid
														   	when 34 then 
														   		ent.entungcod ||' - '|| ent.entnome 
														   	else 
														   		ent.entunicod ||' - '|| ent.entnome 
														   	end as descricao
													FROM entidade.entidade ent 
													INNER JOIN monitora.usuarioresponsabilidade ur ON ur.unicod = ent.entunicod or ur.ungcod = ent.entungcod
													INNER JOIN entidade.funcaoentidade fe ON fe.entid = ent.entid
													where ent.entid not in(391281)
													and ur.prsano = '".$_SESSION["exercicio"]."'
													$whereResp	
													GROUP BY codigo, descricao, entunicod
													ORDER BY entunicod																			
													");							
							}
					
					}
					else{
						$entid = "";
					}
					
				
					$sqlComboEntidade = "SELECT
									   e.entid as codigo,
									   CASE funid
									   	when 34 then 
									   		entungcod ||' - '|| entnome 
									   	else 
									   		entunicod ||' - '|| entnome 
									   	end as descricao
									   FROM entidade.entidade e
									   INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
									   INNER JOIN monitora.usuarioresponsabilidade ur ON ur.unicod = e.entunicod or ur.ungcod = e.entungcod
									   WHERE funid <> 33 and e.entid not in(391281) 
									   and ur.prsano = '".$_SESSION["exercicio"]."'
									   $whereResp
									   GROUP BY codigo, descricao, entunicod
										ORDER BY entunicod";
					//dbg($sqlComboEntidade);
					combo_popup( "entid", $sqlComboEntidade, "Unidades Orçamentárias", "600x600", 0, array(), "", "S", false, false, 5, 400 );				
				
				?>
				</td>
			</tr>
			
			<tr>
				<td class="SubTituloDireita" valign="top">Ações:</td>
				<td>
				<?php
					if($_POST['acaid'][0] && !$_POST['ordemlista']){
							//$entidaux = $_REQUEST['entid'];
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
														1
													");
					
					}
					else{
						$acaid = "";
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
										1 ";
					combo_popup( "acaid", $sqlComboAcoes, "Ações", "600x600", 0, array(), "", "S", false, false, 5, 400 );
				?>
				</td>
			</tr>

		<? //echo $combo->pegarComboHTML(COMBO_REGRAGERAL); ?>
		
			<tr>
				<td align='right' class="SubTituloDireita">Executor Orçamentário e Financeiro:</td>
			    <td>
			    	
					<?
					$campo3 = $_REQUEST['campo3'];
					$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao FROM public.combodadostabela WHERE ctbid = 3 order by cdtcod";  
					$db->monta_combo('campo3', $sql, 'S', 'Selecione', '', '', '', '100', 'N', '000000003_0'); 
					?>    	
			    </td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Gestor da Subação:</td>
			    <td>
			    	
					<?
					$campo4 = $_REQUEST['campo4'];
					$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao FROM public.combodadostabela WHERE ctbid = 4 order by cdtcod";  
					$db->monta_combo('campo4', $sql, 'S', 'Selecione', '', '', '', '100', 'N', '000000003_1'); 
					?>    	
			    </td>
			</tr>
		
		
				<tr>
					<td class='SubTituloDireita'>Enquadramento da Despesa:</td>
					<td>
						<?
						$campo5 = $_REQUEST['campo5'];
						$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
							    FROM public.combodadostabela where ctbid=5 and cdtstatus='A' order by cdtcod";
						
						$db->monta_combo('campo5', $sql, 'S', 'Selecione', '', '', '', '240', 'N', '000000005_0'); 
						?>
					</td>
				</tr>
				<tr>
					<td class='SubTituloDireita'>Nível/Etapa de Ensino:</td>
					<td>
						<?
						$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
							    FROM public.combodadostabela where ctbid=6 and cdtstatus='A' order by cdtcod";
						
						$db->monta_combo('campo6', $sql, 'S', 'Selecione', '', '', '', '240', 'N', '000000005_1');
						?>
					</td>
				</tr>
				<tr>
					<td class='SubTituloDireita'>Categoria de Apropriação:</td>
					<td>
						<?
						$campo7 = $_REQUEST['campo7'];
						$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
							    FROM public.combodadostabela where ctbid=7 and cdtstatus='A' order by cdtcod";
						
						$db->monta_combo('campo7', $sql, 'S', 'Selecione', '', '', '', '340', 'N', '000000005_2');
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
						$campo8 = $_REQUEST['campo8'];
						$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
							    FROM public.combodadostabela where ctbid=8 and cdtstatus='A' order by cdtcod";
						$db->monta_combo('campo8', $sql, 'S', 'Selecione', '', '', '', '240', 'N', '000000005_3');
			    	 	?>
				    </td>
				</tr>
				
		
		<tr>
			<td class="SubTituloDireita">Situação:</td>
			<td>
				<input type="radio" name="plisituacao" id="plisituacao" value="" <?if($_REQUEST["plisituacao"] == '') echo 'checked';?>> Qualquer
				<input type="radio" name="plisituacao" id="plisituacao" value="C" <?if($_REQUEST["plisituacao"] == 'C') echo 'checked';?>> Aprovado
				<input type="radio" name="plisituacao" id="plisituacao" value="H" <?if($_REQUEST["plisituacao"] == 'H') echo 'checked';?>> Homologado
				<input type="radio" name="plisituacao" id="plisituacao" value="R" <?if($_REQUEST["plisituacao"] == 'R') echo 'checked';?>> Enviado para Revisão
				<input type="radio" name="plisituacao" id="plisituacao" value="S" <?if($_REQUEST["plisituacao"] == 'S') echo 'checked';?>> Cadastrado no SIAFI
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td></td>
			<td>
				<input type="button" value="Pesquisar" onclick="filtra_pi();" style="cursor: pointer;"/>
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
										monitora.subacao sba
									INNER JOIN
										monitora.subacaodotacao sbd ON sbd.sbaid = sba.sbaid
									INNER JOIN
										monitora.ptres ptr ON ptr.ptrid = sbd.ptrid
									LEFT JOIN
										monitora.planointerno pi ON pi.sbaid = sba.sbaid
									LEFT JOIN
										monitora.subacaounidade su ON su.sbaid = sba.sbaid
									LEFT JOIN
										entidade.entidade e ON e.entid = su.entid								
									WHERE
										sba.sbastatus = 'A' " . ( !empty($filtro) ? "AND" . implode(" AND ", $filtro) : '' ) . "
										--sba.exeid = 42
									ORDER BY
										sba.sbaid";
		//dbg($sql);
		$dadosSubAcao = $db->carregar($sql);
		
		echo "<script type=\"text/javascript\">
				function exibeDadosPI(id, tipoacao) {
					var janela = window.open( '?modulo=principal/planointerno/dados_pi&acao=A&plicod=' + id + '&tipoacao=' + tipoacao, 
								 			  'DadosdoPI','scrollbars=yes, width=800, height=650 ');
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
								pli.pliptres AS ptres
							FROM
								monitora.planointerno pli
							INNER JOIN
								monitora.ptres ptr ON ptr.ptres = pli.pliptres
							WHERE
								pli.sbaid = {$subacao} " . ( !empty($filtropi) ? "AND" . implode(" AND ", $filtropi) : '' ) . "
								AND ptr.ptrano = '".$_SESSION["exercicio"]."'";

	$dadosPI = $db->carregar($sqlpi);
	
	if($dadosPI) {
		echo "<tr>
				<td colspan=\"6\">
					<strong>Dados dos PI's da Subação: ".$codigo."</strong>
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
						<td align=\"center\"><a href=\"javascript:void(0);\" onclick=\"exibeDadosPI('".$dadosPI[$i]["codigo"]."', 'consulta');\">".$dadosPI[$i]["codigo"]."</a></td>
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

?>