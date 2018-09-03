<?php

include_once APPRAIZ."/www/painel/_funcoes.php";

function processarLinhaDetalhamentoIndicadores($indicador, $request, $exibelinha=false) {
	global $db, $filtro1, $i;

	$estrutura = getEstruturaRegionalizacao($indicador['regid']);
	
	$estloop = $estrutura; 
	
	if($estloop) {
		do {
			$estor[] = $estloop['atu'];
			$estloop = $estloop['sub'];
		} while ($estloop['atu']);
	}
	
	/*
	 * MONTA O QUADRO DE EXPANSÃO AO CLICAR NO +
	 */
	$acesso = false;
	$exibeGlobo = false;
	
	for($j=(count($estor)-1);$j >= 0;$j--) {
		if($acesso) {
			$combodetalhar[] = "<option value='".$estor[$j]['rgavisao']."'>".$estor[$j]['regdescricao']."</option>";
		}
		
		if($estor[$j]['rgavisao'] == "municipio") {
			$exibeGlobo = true;
		}
		
		if($estor[$j]['rgavisao'] == $request['detalhes']) {
			$acesso = true;
			$params = "&".$estor[$j]['rgaidentificador']."=".$_REQUEST[$estor[$j]['rgaidentificador']];
		} elseif($request['detalhes'] == 'tipomunicipio' && $estor[$j]['rgavisao'] == 'estado') {
			$acesso = true;
			$params = "&tpmid=".$_REQUEST['tpmid'];
		}
	}
	
	unset($detalhes);
	$detalhes = $db->carregar("SELECT tdiid, tdidsc FROM painel.detalhetipoindicador WHERE indid='".$indicador['indid']."'");
	
	
	if($detalhes[0]) {
		foreach($detalhes as $det) {
			$combodetalhar[] = "<option value='detalhes&&".$det['tdiid']."'>Por ".$det['tdidsc']."</option>";
			$funcaounica = "detalharPorDetalhes(this,'".$dadosi['indid']."','".$det['tdiid']."')";
		}
	}
	
	if($combodetalhar) {
		$combo .= "<select id='combo".$indicador['indid']."'>";
		foreach($combodetalhar as $cc) {
 			$combo .= $cc;
		}
		$combo .= "</select>";
	}
	/*
	 * FIM -  MONTA O QUADRO DE EXPANSÃO AO CLICAR NO +
	 */
	
	/*
	 * Pega os valores de detalhe para exibição do gráfico
	 */
	$detalheGrafico = $request['rgaidentificador'];
	$valorDetalheGrafico = $_REQUEST[$detalheGrafico];
	$sql = "select 
				tdiid,
				tdidsc
			from
				painel.detalhetipoindicador
			where
				indid = {$indicador['indid']}
			and
				tdistatus = 'A'";
	
	$det = $db->carregar($sql);
	
	$sqlMin = "select 
				distinct seh.dpeid
			from 
				painel.seriehistorica seh
			inner join
				painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
			where
				indid = {$indicador['indid']}
			and
				dpedatainicio = (
			select 
				min(dpedatainicio) as data
			from 
				painel.detalheperiodicidade 
			where 
				dpeid in (  select dpeid from painel.seriehistorica where indid = {$indicador['indid']} ) )";
	$dataMin = $db->pegaUm($sqlMin);
	
	$sqlMax = "select 
				distinct seh.dpeid
			from 
				painel.seriehistorica seh
			inner join
				painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
			where
				indid = {$indicador['indid']}
			and
				dpedatainicio = (
			select 
				max(dpedatainicio) as data
			from 
				painel.detalheperiodicidade 
			where 
				dpeid in (  select dpeid from painel.seriehistorica where indid = {$indicador['indid']} ) )";
	$dataMax = $db->pegaUm($sqlMax);
	
	$arrEstufMuncod = retornaEstUfMuncodGrafico($request['detalhes'],$valorDetalheGrafico);
	
	if($det[0]['tdiid'] && $det[0]['tdiid'] != ""){
		$option_barra .= "<option value=\"barra_1_{$det[0]['tdiid']}\" >Gráfico em Barra - {$det[0]['tdidsc']}</option>";
		$option_barra .= "<option value=\"barracomp_1_{$det[0]['tdiid']}\" >Gráfico em Barra Comparativa - {$det[0]['tdidsc']}</option>";
		$option_pizza .= "<option value=\"pizza_1_{$det[0]['tdiid']}\" >Gráfico em Pizza - {$det[0]['tdidsc']}</option>";
	}
	if($det[1]['tdiid'] && $det[1]['tdiid'] != ""){
		$option_barra .= "<option value=\"barra_2_{$det[1]['tdiid']}\" >Gráfico em Barra - {$det[1]['tdidsc']}</option>";
		$option_barra .= "<option value=\"barracomp_2_{$det[1]['tdiid']}\" >Gráfico em Barra Comparativa - {$det[1]['tdidsc']}</option>";
		$option_pizza .= "<option value=\"pizza_2_{$det[1]['tdiid']}\" >Gráfico em Pizza - {$det[1]['tdidsc']}</option>";
	}
	
	/* Início - Aplica opção de visualizar dados anualmente */
	
	if($indicador['unmid'] != UNIDADEMEDICAO_PERCENTUAL && $indicador['unmid'] != UNIDADEMEDICAO_RAZAO):
	
		$arrPeriodicidade = getPeriodicidadeIndicador($indicador['indid']);
		$arrPeriodicidade = !$arrPeriodicidade ? array() : $arrPeriodicidade;
		
		$html_ano = '<span id="opc_periodicidade_'.$indicador['indid'].'" style="margin-left:20px;">
							 Periodicidade: 
							 <select onchange="exibeGraficoPorIndicador(\''.$indicador['indid'].'\',\''.$indicador['acaid'].'\',\''.$request['detalhes'].'\',\''.$detalheGrafico.'\',\''.$valorDetalheGrafico.'\',document.getElementById(\'selectGrafico_'.$indicador['indid'].'\').value,\''.$dataMin.'\',\''.$dataMax.'\',\''.$arrEstufMuncod['estuf'].'\',\''.$arrEstufMuncod['muncod'].'\',\''.$indicador['indqtdevalor'].'\')" id="sel_periodicidade_'.$indicador['indid'].'" name="sel_periodicidade" >';
							 foreach($arrPeriodicidade as $arrPer):
							 	$html_ano .= '<option '.($arrPer['perid'] == PERIODO_ANUAL ? "selected=\"selected\"" : "").'  value="'.$arrPer['perid'].'" >'.$arrPer['perdsc'].'</option>';
							 	$arrPerid[] = $arrPer['perid'];
							 endforeach;
							 $arrPerid = !$arrPerid ? array() : $arrPerid;
							 if(!in_array(PERIODO_ANUAL,$arrPerid)):
							 	$html_ano .= '<option selected="selected"  value="'.PERIODO_ANUAL.'" >'.Anual.'</option>';
							 endif;
							$html_ano .= '</select></span>';
	endif;
	
	/* Fim - Aplica opção de visualizar dados anualmente */
	
	$htmlGrafico = '<fieldset style="padding:5px;margin: 5px;">';
	$htmlGrafico .= '<legend>Exibição de Gráfico:</legend>';
	$htmlGrafico .= 'Selecione o tipo de gráfico: <select id="selectGrafico_'.$indicador['indid'].'" style="width:200px" onchange="exibeGraficoPorIndicador(\''.$indicador['indid'].'\',\''.$indicador['acaid'].'\',\''.$request['detalhes'].'\',\''.$detalheGrafico.'\',\''.$valorDetalheGrafico.'\',this.value,\''.$dataMin.'\',\''.$dataMax.'\',\''.$arrEstufMuncod['estuf'].'\',\''.$arrEstufMuncod['muncod'].'\',\''.$indicador['indqtdevalor'].'\')" >';
	$htmlGrafico .= '<option value="" >Selecione...</option>';
	$htmlGrafico .= '<option value="linha" >Gráfico em Linha</option>';
	$htmlGrafico .= '<option value="barra" >Gráfico em Barra</option>';
	$htmlGrafico .= $option_barra;
	$htmlGrafico .= $option_pizza;
	$htmlGrafico .= '</select>';
	$htmlGrafico .= $html_ano;
	$htmlGrafico .= '<div style="margin-top:10px"  ><span style="display:none" id="grafico_linha_financeiro_'.$indicador['indid'].'"><input type="radio" onclick="checkFinanceiro(this,'.$indicador['indid'].');exibeGraficoPorIndicador(\''.$indicador['indid'].'\',\''.$indicador['acaid'].'\',\''.$request['detalhes'].'\',\''.$detalheGrafico.'\',\''.$valorDetalheGrafico.'\',document.getElementById(\'selectGrafico_'.$indicador['indid'].'\').value,\''.$dataMin.'\',\''.$dataMax.'\',\''.$arrEstufMuncod['estuf'].'\',\''.$arrEstufMuncod['muncod'].'\',\''.$indicador['indqtdevalor'].'\')" id="finac_qtde_'.$indicador['indid'].'" name="finac_tipo" value="qtde" />Quantidade  <input onclick="checkFinanceiro(this,'.$indicador['indid'].');exibeGraficoPorIndicador(\''.$indicador['indid'].'\',\''.$indicador['acaid'].'\',\''.$request['detalhes'].'\',\''.$detalheGrafico.'\',\''.$valorDetalheGrafico.'\',document.getElementById(\'selectGrafico_'.$indicador['indid'].'\').value,\''.$dataMin.'\',\''.$dataMax.'\',\''.$arrEstufMuncod['estuf'].'\',\''.$arrEstufMuncod['muncod'].'\',\''.$indicador['indqtdevalor'].'\')" style="margin-left:10px;" type="radio" id="finac_valor_'.$indicador['indid'].'" name="finac_tipo" value="valor" />Valor Monetário R$ <input onclick="checkFinanceiro(this,'.$indicador['indid'].');exibeGraficoPorIndicador(\''.$indicador['indid'].'\',\''.$indicador['acaid'].'\',\''.$request['detalhes'].'\',\''.$detalheGrafico.'\',\''.$valorDetalheGrafico.'\',document.getElementById(\'selectGrafico_'.$indicador['indid'].'\').value,\''.$dataMin.'\',\''.$dataMax.'\',\''.$arrEstufMuncod['estuf'].'\',\''.$arrEstufMuncod['muncod'].'\',\''.$indicador['indqtdevalor'].'\')" style="margin-left:10px;" type="radio" id="finac_ambos_'.$indicador['indid'].'" name="finac_tipo" checked="checked" value="ambos" />Ambos</span></div><br /><br />';
	$htmlGrafico .= '<input type="button" value="Fechar" onclick="document.getElementById(\'exibeGrafico_'.$indicador['indid'].'\').style.display = \'none\';" /><br />';
	$htmlGrafico .= '</fieldset>';
	
	$htmlGrafico2  = '<fieldset style="padding:5px;margin: 5px;">';
	$htmlGrafico2 .= '<legend>Exibição de Gráfico:</legend>';
	$htmlGrafico2 .= 'Selecione o tipo de gráfico: <select style="width:200px" onchange="exibeGraficoPorIndicador(\''.$indicador['indid'].'\',\''.$indicador['acaid'].'\',\''.$request['detalhes'].'\',\''.$detalheGrafico.'\',\''.$valorDetalheGrafico.'\',this.value,\''.$dataMin.'\',\''.$dataMax.'\',\''.$arrEstufMuncod['estuf'].'\',\''.$arrEstufMuncod['muncod'].'\',\''.$indicador['indqtdevalor'].'\')" >';
	$htmlGrafico2 .= '<option value="" >Selecione...</option>';
	$htmlGrafico2 .= '<option value="linha" >Gráfico em Linha</option>';
	$htmlGrafico2 .= '<option value="barra" >Gráfico em Barra</option>';
	$htmlGrafico2 .= $option_barra;
	$htmlGrafico2 .= $option_pizza;
	$htmlGrafico2 .= '</select><br /><br />';
	$htmlGrafico2 .= '<input type="button" value="Fechar" onclick="document.getElementById(\'exibeGrafico_'.$indicador['indid'].'\').style.display = \'none\';" /><br />';
	$htmlGrafico2 .= '</fieldset>';
	
	//Pega periodicidade do Indicador
	$sql = "select 
				per.perdsc, 
				per.perid 
			from 
				painel.periodicidade per 
			inner join
				painel.indicador ind ON per.perid = ind.perid
			where 
				ind.indid = {$indicador['indid']} 
			and 
				per.perid <> ".PERIODO_ANUAL;
	$per = $db->pegaLinha($sql);
	if($det){
		$divTipoDetalhe = "<img src=\"../imagens/edit_on.gif\" style=\"cursor:pointer\" onclick=\"document.getElementById('detalhe_{$indicador['indid']}_{$indicador['acaid']}').style.display='block'\" />";
		$divTipoDetalhe.= "<div id=\"detalhe_{$indicador['indid']}_{$indicador['acaid']}\" class=\"boxindicador\" style=z-index:1000;>";
		$divTipoDetalhe.= "<table border=0 ><tr> <td> Selecione o detalhe: </td><td> <select id=\"sel_det_{$indicador['indid']}_{$indicador['acaid']}\" ><option value=\"{$det[0]['tdiid']}\" >Por {$det[0]['tdidsc']}</option>";
		$divTipoDetalhe.= $det[1]['tdiid'] ? "<option value=\"{$det[1]['tdiid']}\" >Por {$det[1]['tdidsc']}</option> <option value=\"{$det[0]['tdiid']},{$det[1]['tdiid']}\" >{$det[0]['tdidsc']} por {$det[1]['tdidsc']}</option> <option value=\"{$det[1]['tdiid']},{$det[0]['tdiid']}\" >{$det[1]['tdidsc']} por {$det[0]['tdidsc']}</option> " : "";
		$divTipoDetalhe.= "<option selected='selected' value=0 >Sem Detalhe</option></select> </td></tr>";		
		if($per){
			if($indicador['unmid'] != UNIDADEMEDICAO_PERCENTUAL && $indicador['unmid'] != UNIDADEMEDICAO_RAZAO):
				$divTipoDetalhe.= "<tr> <td> Selecione a periodicidade: </td><td> 
					<select id=\"sel_perid_{$indicador['indid']}_{$indicador['acaid']}\" >";
					foreach($arrPeriodicidade as $arrPer):
					 	$divTipoDetalhe .= '<option value="'.$arrPer['perid'].'" >'.$arrPer['perdsc'].'</option>';
					 	$arrPerid[] = $arrPer['perid'];
					endforeach;
					$arrPerid = !$arrPerid ? array() : $arrPerid;
					if(!in_array(PERIODO_ANUAL,$arrPerid)):
						$divTipoDetalhe .= '<option value="'.PERIODO_ANUAL.'" >Anual</option>';
					endif;
					$divTipoDetalhe .= "</select> </td></tr>";
			endif;
		}
		$divTipoDetalhe.= "<tr> <td colspan=2 > <input type=\"button\" value=\"OK\" onclick=\"exibeTabelaIndicador('".$indicador['indid']."','".$indicador['acaid']."','".str_replace(array("/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A","/pde/estrategico.php?modulo=principal/painel_estrategico&acao=A"),"",$_SERVER['REQUEST_URI'])."','sel_det_{$indicador['indid']}_{$indicador['acaid']}','sel_perid_{$indicador['indid']}_{$indicador['acaid']}');document.getElementById('detalhe_{$indicador['indid']}_{$indicador['acaid']}').style.display='none'\" /> <input type=\"button\" value=\"Fechar\" onclick=\"document.getElementById('detalhe_{$indicador['indid']}_{$indicador['acaid']}').style.display='none'\" /> </td></tr></table> </div>";
	}else{
		if(!$per)
			$divTipoDetalhe = "<img src=\"../imagens/edit_on.gif\" style=\"cursor:pointer\" onclick=\"exibeTabelaIndicador('".$indicador['indid']."','".$indicador['acaid']."','".str_replace(array("/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A","/pde/estrategico.php?modulo=principal/painel_estrategico&acao=A"),"",$_SERVER['REQUEST_URI'])."');\" />";
		else{
			$divTipoDetalhe = "<img src=\"../imagens/edit_on.gif\" style=\"cursor:pointer\" onclick=\"document.getElementById('detalhe_{$indicador['indid']}_{$indicador['acaid']}').style.display='block'\" />";
			$divTipoDetalhe.= "<div id=\"detalhe_{$indicador['indid']}_{$indicador['acaid']}\" class=\"boxindicador\" style=z-index:1000;>";
			if($indicador['unmid'] != UNIDADEMEDICAO_PERCENTUAL && $indicador['unmid'] != UNIDADEMEDICAO_RAZAO):
				$divTipoDetalhe.= "Selecione a periodicidade: 
						<select id=\"sel_perid_{$indicador['indid']}_{$indicador['acaid']}\" >";
						foreach($arrPeriodicidade as $arrPer):
						 	$divTipoDetalhe .= '<option value="'.$arrPer['perid'].'" >'.$arrPer['perdsc'].'</option>';
						endforeach;
						$arrPerid = !$arrPerid ? array() : $arrPerid;
						if(!in_array(PERIODO_ANUAL,$arrPerid)):
							$divTipoDetalhe .= '<option value="'.PERIODO_ANUAL.'" >Anual</option>';
						endif;
				$divTipoDetalhe .= "</select> <br />";
			endif;
			$divTipoDetalhe.= "<input type=\"button\" value=\"OK\" onclick=\"exibeTabelaIndicador('".$indicador['indid']."','".$indicador['acaid']."','".str_replace(array("/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A","/pde/estrategico.php?modulo=principal/painel_estrategico&acao=A"),"",$_SERVER['REQUEST_URI'])."','','sel_perid_{$indicador['indid']}_{$indicador['acaid']}');document.getElementById('detalhe_{$indicador['indid']}_{$indicador['acaid']}').style.display='none'\" /> <input type=\"button\" value=\"Fechar\" onclick=\"document.getElementById('detalhe_{$indicador['indid']}_{$indicador['acaid']}').style.display='none'\" /></div>";
		}
		
	}
	
	
	// controle de cores das linhas
	$cor   = $i%2 ? "#FCFCFC" : "#F5F5F5";
	
	$html .= "<tr ".(($exibelinha)?"":"id=\"tr_".$indicador['acaid']."_".$indicador['indid']."\"")." bgcolor='".$cor."' onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".$cor."';\" ".(($exibelinha)?"":"style=\"display:none;\"").">";
	
	$params_map = md5_encrypt($filtro1);
	// coluna com icones de expansão
	$html .= "<td align=\"center\" style=\"white-space:nowrap\" >".(($combo && ($indicador['qtde']>0 || $indicador['qtde']=='-'))?"<img src=\"../imagens/mais.gif\" id=\"img".$indicador['indid']."\" title=\"mais\" onclick=\"abrebox('".$indicador['indid']."');\"><div class=\"boxindicador\" id=\"box".$indicador['indid']."\" style=z-index:1000;>Selecione a forma de detalhar: ".$combo."<br/><br/><input type=button value=Ok onclick=processabox('".$indicador['acaid']."_".$indicador['indid']."','".$params."');> <input type=button value=Fechar onclick=document.getElementById('box".$indicador['indid']."').style.display='none';></div>":"&nbsp;")." <img src=\"../imagens/seriehistorica_ativa.gif\" style=\"cursor:pointer\" onclick=\"exibeGraficoIndicador('".$indicador['indid']."','".$request['detalhes']."','".$detalheGrafico."','".$valorDetalheGrafico."');\" /> <img src=\"../imagens/info.gif\" style=\"cursor:pointer\" onclick=\"exibedadosIndicador('".$indicador['indid']."','".$indicador['acaid']."');\" /> $divTipoDetalhe <div class=\"boxindicador\" id=\"exibeGrafico_{$indicador['indid']}\" style=z-index:1000;>$htmlGrafico2</div> ".(($exibeGlobo)?"<a href=\"javascript:void(0);\" onclick=\"exibeGrafico('".$indicador['indid']."', '".$params_map."');\"><img border=\"0\" title=\"Exibir Mapa\" src=\"/imagens/icone_br.png\"></a>":"")."<a href=\"javascript:void(0);\" onclick=\"exportarIndicadorExcel('".$indicador['indid']."', '".$params_map."');\"><img border=\"0\" title=\"Exportar\" src=\"/imagens/excel.gif\"></a></td> ";
	// coluna com ID do indicador
	$html .= "<td align=\"center\" ><div style=\"width:100%;text-align:center;color:#0066CC\" >".$indicador['indid']."</div></td>";
	// coluna com nome do indicador
	$html .= "<td style=\"font-size:13px;font-family:verdana;color:#000000;\">".$indicador['indnome']."</td>";
	// coluna com o nome da secretaria
	$html .= "<td style=\"white-space: nowrap;\" align=\"left\" ><div style=\"margin-left:10px\" >".(!$indicador['secdsc']? "<span style=\"color:#AA0000;\" >N/A</span>" : "<span>".$indicador['secdsc']."</span>")." </div></td>";
	// coluna com a descrição do regionalizador
	$html .= "<td align=\"left\" ><div style=\"margin-left:10px\" >".(!$indicador['regdescricao']? "<span style=\"color:#AA0000;\" >N/A</span>" : "<span>".$indicador['regdescricao']."</span>")." </div></td>";
	// coluna com a descrição da unidade de medida
	$html .= "<td align=\"left\" ><div style=\"margin-left:10px;color:#000000;\" >".(!$indicador['umedesc']? "<span style=\"color:#AA0000;\" >N/A</span>" : "<span>".$indicador['umedesc']."</span>")." </div></td>";
	
	// coletando dados da periodicidade (ultima serie historica coletada)
	$ult  = $db->pegaLinha("SELECT d.dpedsc, d.dpeanoref FROM painel.v_detalheindicadorsh d   
						    WHERE d.indid='".$indicador['indid']."' ".str_replace(array("zon.zonid","sub.subid","dis.disid","s.setid"),array("zonid","subid","disid","setid"),$filtro1)." ORDER BY d.dpedatainicio DESC LIMIT 1");
	
	// coletando informações sobre a periodicidade da quantidade
	if($indicador['indcumulativo'] == "N") {
		
		$qtdeinfo = "<span style=\"font-size:9px;\">Não cumulativo <br>(".$ult['dpedsc'].")</span>";
		
	} elseif($indicador['indcumulativo'] == "S") {
		
		$pri  = $db->pegaUm("SELECT dpe.dpedsc FROM painel.seriehistorica seh 
							 LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid 
							 WHERE seh.indid='".$indicador['indid']."' AND seh.sehstatus='H' ORDER BY dpedatainicio");

		
		//se nao tiver serie historica no status de H ( eh pq possui apenas 1 serie historica e é ativo A)
		if(!$pri) $pri = $ult['dpedsc'];
		
		$qtdeinfo = "<span style=\"font-size:9px;\">Cumulativo<br>(".(!$ult['dpedsc']? "N/A" : $pri." - ".$ult['dpedsc']).")</span>";
		
	} else {
		
		$qtdeinfo = "<span style=\"font-size:9px;\">Cumulativo por ano<br>(".$ult['dpeanoref'].")</span>";
		
	}
	
	// coletando informações sobre a periodicidade da quantidade
	if($indicador['indqtdevalor']=="t") {
		if($indicador['indcumulativovalor'] == "N") {
			
			$vlrinfo = "<span style=\"font-size:9px;\">Não cumulativo <br>(".$ult['dpedsc'].")</span>";
			
		} elseif($indicador['indcumulativovalor'] == "S") {
			
			$pri  = $db->pegaUm("SELECT dpe.dpedsc FROM painel.seriehistorica seh LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid WHERE seh.indid='".$indicador['indid']."' AND seh.sehstatus='H' ORDER BY dpedatainicio");
			
			$vlrinfo = "<span style=\"font-size:9px;\">Cumulativo<br>(".$pri." - ".$ult['dpedsc'].")</span>";
			
		} else {
			
			$vlrinfo = "<span style=\"font-size:9px;\">Cumulativo por ano<br>(".$ult['dpeanoref'].")</span>";
			
		}
	}
	
    // coletando a penultima serie historica informada
    $sehidold = $db->pegaUm("SELECT sehid FROM painel.seriehistorica seh
                              INNER JOIN painel.detalheperiodicidade dlp ON dlp.dpeid=seh.dpeid
                              WHERE seh.indid='".$indicador['indid']."' AND seh.sehstatus!='A'
                              ORDER BY dlp.dpedatainicio DESC LIMIT 1");
    
    unset($seta);
    
    // pegando o valor da penultima, e analisando a tendencia
    if($sehidold) {
        $valorold  = $db->pegaUm("SELECT SUM(dshqtde) as qtde FROM painel.detalheseriehistorica d
                                  INNER JOIN painel.seriehistorica seh ON seh.sehid = d.sehid 
								  LEFT JOIN territorios.municipio mun ON mun.muncod = d.dshcodmunicipio
								  LEFT JOIN territorios.estado est ON est.estuf = d.dshuf 
								  LEFT JOIN territorios.regiao reg ON reg.regcod = est.regcod  
								  LEFT JOIN territorios.mesoregiao mes ON mes.mescod = mun.mescod 
								  LEFT JOIN territorios.microregiao mic ON mic.miccod = mun.miccod
                                  WHERE seh.sehid='".$sehidold."' AND seh.sehstatus!='I' ".$filtro1);
        
        $estilo = !$estilo ? 1 : $dadosi['estid'];
        
        $valor1 = (float)$indicador['qtde']; //ultimo
        $valor2 = (float)$valorold; //penultimo
        
        if($valor1 < $valor2 && $estilo == 1){ 
        	// Se o $valor 1 for menor q o $valor 2 e a tendência for MAIOR MELHOR, a tendência indica queda (seta vermelha p/ baixo)
            $seta = " <img style=\"cursor:pointer\" src=\"../imagens/indicador-vermelha2.png\" align=\"absmiddle\" >";
        }
        if($valor1 < $valor2 && $estilo == 2){ 
        	// Se o $valor 1 for menor q    o $valor 2 e a tendência for MENOR MELHOR, a tendência indica aumento (seta verde p/ baixo)
            $seta = " <img style=\"cursor:pointer\" src=\"../imagens/indicador-verde2_para_baixo.png\" align=\"absmiddle\" >";
        }
        if($valor1 > $valor2 && $estilo == 1){ 
        	// Se o $valor 1 for maior q    o $valor 2 e a tendência for MAIOR MELHOR, a tendência indica aumento (seta verde p/ cima)
            $seta = " <img style=\"cursor:pointer\" src=\"../imagens/indicador-verde2.png\" align=\"absmiddle\" >";
        }
        if($valor1 > $valor2 && $estilo == 2){ 
        	// Se o $valor 1 for maior q    o $valor 2 e a tendência for MENOR MELHOR, a tendência indica queda (seta vermelha p/ cima)
            $seta = " <img style=\"cursor:pointer\" src=\"../imagens/indicador-vermelha2_para_cima.png\" align=\"absmiddle\" >";
        }
        if($valor1 > $valor2 && $estilo == 3){ 
        	// Se o $valor 1 for maior q    o $valor 2 e a tendência for N/A, a tendência indica aumento (seta cinza p/ cima)
            $seta = " <img style=\"cursor:pointer\" src=\"../imagens/indicador-cinza.png\" align=\"absmiddle\" >";
        }
        if($valor1 < $valor2 && $estilo == 3){ 
        	// Se o $valor 1 for menor q    o $valor 2 e a tendência for N/A, a tendência indica queda (seta cinza p/ baixo)
            $seta = " <img style=\"cursor:pointer\" src=\"../imagens/indicador-cinza2.png\" align=\"absmiddle\" >";
        }
        
    }
	// coluna com a quantidade e valor
	if($indicador['indqtdevalor']=="t") {
		
		$html .= "<td style=\"white-space: nowrap;\" align=\"right\" ><div style=\"width:100%;text-align:right;color:#0066CC;font-size:13px;\" >".number_format($indicador['qtde'],0,",",".")." ".$seta."<br/>".$qtdeinfo."</div></td>";
		$html .= "<td style=\"white-space: nowrap;\" align=\"right\" ><div style=\"width:100%;text-align:right;color:#0066CC;font-size:13px;\" >".$indicador['valor']. "<br/>".$vlrinfo."</div></td>";
		
	} elseif($indicador['unmid'] == UNIDADEMEDICAO_MOEDA) {
		
		$html .= "<td style=\"white-space: nowrap;\" align=\"right\" ><div style=\"width:100%;text-align:right;color:#0066CC;font-size:13px;\" >-</div></td>";
		$html .= "<td style=\"white-space: nowrap;\" align=\"right\" ><div style=\"width:100%;text-align:right;color:#0066CC;font-size:13px;\" >".number_format($indicador['qtde'],2,",","."). " ".$seta."<br/>".$qtdeinfo."</div></td>";
	
	} else {
		
		// se for
		if($indicador['unmid'] == UNIDADEMEDICAO_PERCENTUAL || 
		   $indicador['unmid'] == UNIDADEMEDICAO_RAZAO) {
		   	
		   	if($indicador['indshformula'] != "t" && $estor[0]['rgavisao'] != $request['detalhes']) {
		    	$html .= "<td style=\"white-space: nowrap;\" align=\"right\" ><div style=\"width:100%;text-align:right;color:#0066CC;font-size:13px;\" >-</div></td>";
		   	} else {
				$html .= "<td style=\"white-space: nowrap;\" align=\"right\" ><div style=\"width:100%;text-align:right;color:#0066CC;font-size:13px;\" >".number_format($indicador['qtde'],2,",","."). " ".$seta."<br/>".$qtdeinfo."</div></td>";
		   	}
		} else {
			$html .= "<td style=\"white-space: nowrap;\" align=\"right\" ><div style=\"width:100%;text-align:right;color:#0066CC;font-size:13px;\" >".number_format($indicador['qtde'],0,",","."). " ".$seta."<br/>".$qtdeinfo."</div></td>";			
		}
		$html .= "<td style=\"white-space: nowrap;\" align=\"right\" ><div style=\"width:100%;text-align:right;color:#0066CC;font-size:13px;\" >-</div></td>";
		
	}
	
	$html .= "</tr>";
	
	
	
	$html .= "<tr style=\"display:none\" id=\"tr_".$indicador['acaid']."_".$indicador['indid']."_grafico\"\ ><td colspan=9> <div style=\"width:100%;height:100%;auto;background-color:#FFFFFF\">
								<div id=\"tr_".$indicador['acaid']."_".$indicador['indid']."_grafico_grafico\"\ >
			  	  					".str_replace('<input type="button" value="Fechar" onclick="document.getElementById(\'exibeGrafico_'.$indicador['indid'].'\').style.display = \'none\';" /><br />','<br /><br />',$htmlGrafico)."
									<fieldset style=\"padding:5px;margin: 5px;\"><legend>Gráfico</legend>
									<div style=\"width:20px;height:20px;float:right;margin-right:40px;\"><input type=\"button\" value=\"Fechar\" onclick=\"document.getElementById('tr_".$indicador['acaid']."_".$indicador['indid']."_grafico').style.display = 'none';\" /></div>
			  	  					<div id=\"grafico_indicador_{$indicador['indid']}\">Grafico</div>
									<div id=\"legenda_grafico_{$indicador['indid']}\">Legenda</div></fieldset>
								</div>
								<div id=\"tr_".$indicador['acaid']."_".$indicador['indid']."_grafico_dados\"\ >
								<div id=\"exibe_dados_indicador_".$indicador['indid']."\" ></div>
								</div>
								</td></tr>";
	
	$html .= "<tr style=\"display:none\" id='linha_mapa_".$indicador['indid']."_'><td colspan=8><input type=\"button\" value=\"Fechar\" onclick=\"fecharGlobo('".$indicador['indid']."');\" /></td></tr>
				<tr style=\"display:none\" id='linha_mapa_".$indicador['indid']."'>
				<td colspan=4 valign=top>
				<div id='div_exibe_mapa_".$indicador['indid']."' style='height:300px'></div>
				</td>
				<td colspan=4 valign=top>
				<div style='height:300px;overflow:auto' id=info_mapa_".$indicador['indid']." ></div>
				</td>
			  </tr>";
	
	$i++;
	
	return $html;
	
}

function getEstruturaRegionalizacao($regid) {
	global $db;

	$sql = "SELECT r.rgafiltroreg, r.regidpai, r.rgavisao, re.regicones, re.regsql, re.regid, re.regdescricao, r.rgasqlindicadores, r.rgaidentificador, r.rgafiltro FROM painel.regagreg r 
			LEFT JOIN painel.regionalizacao re ON re.regid=r.regid 
			WHERE r.regid='".$regid."'";
	
	$regionalizador = $db->pegaLinha($sql);
	
	$_arvore['atu'] = $regionalizador;
	
	if($regionalizador['regidpai']) {
		$_arvore['sub'] = getEstruturaRegionalizacao($regionalizador['regidpai']);
	}
	
	return $_arvore;
}

function detalhar($dados) {
	global $db;
	
	$sql = "SELECT * FROM painel.indicador WHERE indid = '".trim($dados['indid'])."' ";
	$indicador = $db->pegaLinha($sql);
	
	$ult  = $db->pegaUm("SELECT dpe.dpeanoref FROM painel.seriehistorica seh 
					  	 LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid  
						 WHERE seh.indid='".$dados['indid']."' AND seh.sehstatus='A'");
	
	if($dados['muncod']) {
		$where .= "mun.muncod='".$dados['muncod']."' AND ";
		$parametros .= "&muncod=".$dados['muncod'];
	}
	if($dados['estuf']) {
		$where .= "mun.estuf='".$dados['estuf']."' AND ";
		$parametros .= "&estuf=".$dados['estuf'];
	}
	if($dados['unicod']) {
		$where .= "uni.unicod='".$dados['unicod']."' AND ";
		$parametros .= "&unicod=".$dados['unicod'];
	}
	if($dados['entid']) {
		$where .= "ent.entid='".$dados['entid']."' AND ";
	}
	if($dados['polid']) {
		$where .= "pol.polid='".$dados['polid']."' AND ";
	}
	if($dados['iecid']) {
		$where .= "iec.iecid='".$dados['iecid']."' AND ";
	}
	if($dados['polid']) {
		$where .= "pol.polid='".$dados['polid']."' AND ";
	}
	if($dados['regcod']) {
		$where .= "reg.regcod='".$dados['regcod']."' AND ";
	}
	if($dados['mescod']) {
		$where .= "mes.mescod='".$dados['mescod']."' AND ";
	}
	if($dados['miccod']) {
		$where .= "mic.miccod='".$dados['miccod']."' AND ";
	}
	if($dados['tpmid']) {
		$where .= "tpm.tpmid='".$dados['tpmid']."' AND ";
		$tipomunicipio_join = "INNER JOIN territorios.muntipomunicipio mtm ON mtm.muncod = mun.muncod AND mtm.estuf = est.estuf 
				 		 	   INNER JOIN territorios.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid"; 
		
	}
	if($dados['zonid']) {
		$where .= "zon.zonid='".$dados['zonid']."' AND ";
		$zona_join = "INNER JOIN territoriosgeo.zona zon ON zon.muncod = mun.muncod";
	}
	if($dados['subid']) {
		$where .= "sub.subid='".$dados['subid']."' AND ";
		$subprefeitura_join = "INNER JOIN territoriosgeo.zona zon ON zon.muncod = mun.muncod
					  		   INNER JOIN territoriosgeo.subprefeitura sub ON sub.zonid = zon.zonid";
	}
	if($dados['disid']) {
		$where .= "dis.disid='".$dados['disid']."' AND ";
		$distrito_join = "INNER JOIN territoriosgeo.zona zon ON zon.muncod = mun.muncod
						  INNER JOIN territoriosgeo.subprefeitura sub ON sub.zonid = zon.zonid
					  	  INNER JOIN territoriosgeo.distrito dis ON dis.subid = sub.subid";
	}
	if($dados['setid']) {
		$where .= "s.setid='".$dados['setid']."' AND ";
		$setor_join = "INNER JOIN territoriosgeo.zona zon ON zon.muncod = mun.muncod
						  INNER JOIN territoriosgeo.subprefeitura sub ON sub.zonid = zon.zonid
					  	  INNER JOIN territoriosgeo.distrito dis ON dis.subid = sub.subid
					  	  INNER JOIN territoriosgeo.setor s ON s.disid = dis.disid";
	}
	
	if($dados['dpes'][0]) {
		$sehstatus="sehstatus='A' OR sehstatus='H'";
		foreach($dados['dpes'] as $dpeinfo) {
			$dpe[] = $dpeinfo['dpeid'];
			$parametros .= "&dpes[][dpeid]=".$dpeinfo['dpeid'];
		}
		if($indicador['indcumulativo']=="N") {
			$where .= "d.dpeid='".$dpeinfo['dpeid']."' AND";
		} elseif($indicador['indcumulativo']=="S") {
			$where .= "d.dpeid IN('".implode("','",$dpe)."') AND";
		} else {
			$sehstatus="dpe.dpedatainicio >= '".$ult."-01-01' AND dpedatafim <= '31-12-".$ult."'";
		}
	} else {
		if($indicador['indcumulativo']=="N") {
			$sehstatus="sehstatus='A'";
		} elseif($indicador['indcumulativo']=="S") {
			$sehstatus="sehstatus='A' OR sehstatus='H'";
		} else {
			$sehstatus="dpe.dpedatainicio >= '".$ult."-01-01' AND dpedatafim <= '".$ult."-12-31'";
		}
		
	}
	
	$complemento = $db->pegaLinha("SELECT rgacomplemento, rgaidentificador FROM painel.regagreg WHERE rgavisao='".$dados['tipo']."'");
	
	$rgacomplemento = explode(";",$complemento['rgacomplemento']);
	$codigo = $rgacomplemento[0];
	$descricao = $rgacomplemento[1];

	$identificador = $complemento['rgaidentificador'];
	
	$estrutura = getEstruturaRegionalizacao($indicador['regid']);
	
	$estloop = $estrutura; 
	
	if($estloop) {
		do {
			$estor[] = $estloop['atu'];
			$estloop = $estloop['sub'];
		} while ($estloop['atu']);
	}
		
	// verificando se existe detalhes
	unset($detalhes);
	$detalhes = $db->carregar("SELECT tdiid, tdidsc FROM painel.detalhetipoindicador WHERE indid='".$indicador['indid']."'");	
	
	$acesso = false;	
	for($j=(count($estor)-1);$j >= 0;$j--) {
		
		if($acesso) {
			$combodetalhar[] = "<option value=\'".$estor[$j]['rgavisao']."\'>".$estor[$j]['regdescricao']."</option>";
		}
		
		if($estor[$j]['rgavisao'] == $dados['tipo']) {
			$acesso = true;
			$params = "&".$estor[$j]['rgaidentificador']."=".$_REQUEST[$estor[$j]['rgaidentificador']];
		}
	}
	
	if($detalhes[0]) {
		foreach($detalhes as $det) {
			$combodetalhar[] = "<option value=\'detalhes&&".$det['tdiid']."\'>Por ".$det['tdidsc']."</option>";
		}
	}
	
	if($combodetalhar) {
		$imgmais = $db->pegaUm("SELECT rgaimgmais FROM painel.regagreg WHERE rgavisao='".$dados['tipo']."'");
		foreach($combodetalhar as $cc) {
 			$combo .= $cc;
		}
	}
	if($indicador['unmid'] == UNIDADEMEDICAO_MOEDA) {
		$formato = "999g999g999g999d99";
		$monetario = "S";
	}elseif($indicador['unmid'] == UNIDADEMEDICAO_PERCENTUAL) {
		$formato = "999g999g999g999d99";
		$monetario = "N";
	} else {
		$formato = "9999999999999";
		$monetario = "N";
	}
		
	$sqldetalhar = $db->pegaUm("SELECT regsqldetalhar FROM painel.regionalizacao WHERE regid='".$indicador['regid']."'");	
	$sql =  str_replace(array("{imgmais}",
							  "{combo}",
							  "{parametros}",
							  "{where}",
							  "{indid}",
							  "{status}",
							  "{codigo}",
							  "{codigo1}",
							  "{descricao}",
							  "{descricao1}",
							  "{identificador}",
							  "{detalhes}",
							  "{formatoqtde}",
							  "{tipomunicipio_join}",
							  "{zona_join}",
							  "{subprefeitura_join}",
							  "{distrito_join}",
							  "{setor_join}",
							  "{ano}",
							  "{rgavisaoini}"),
						array($imgmais,
							  $combo,
							  $parametros,
							  $where,
							  $dados['indid'],
							  $sehstatus,
							  $codigo,
							  substr($codigo,(strpos($codigo,".")+1),strlen($codigo)),
							  $descricao,
							  substr($descricao,(strpos($codigo,".")+1),strlen($descricao)),
							  $identificador,
							  $dados['tipo'],
							  $formato,
							  $tipomunicipio_join,
							  $zona_join,
							  $subprefeitura_join,
							  $distrito_join,
							  $setor_join,
							  $ult,
							  $estor[0]['rgavisao']),$sqldetalhar);
	//dbg(simec_htmlentities($sql));						  
	$cabec = array("&nbsp;","Qtde");
	if($indicador['indqtdevalor'] == "t") {
		$cabec[] = "R$";
		$tam = array('60%','20%','20%');
	} else $tam = array('60%','40%');
	
	$soma = "S";
	
	if($indicador['unmid'] == UNIDADEMEDICAO_PERCENTUAL || $indicador['unmid'] == UNIDADEMEDICAO_RAZAO){
		
		switch($dados['tipo']){
			case "regiao":
					$where_reg = "reg.regcod";
				break;
			case "estado":
					$where_reg = "est.estuf";
				break;
			case "zona":
					$where_reg = "zon.zonid";
				break;
			case "subprefeitura":
					$where_reg = "sub.subid";
				break;
			case "distrito":
					$where_reg = "dis.disid";
				break;
			case "setor":
					$where_reg = "s.setid";
					$regiao_igual = true;
				break;
		}
		
		if($indicador['indshformula'] == "t" && $indicador['formulash']){
			if($regiao_igual){
				$arrQTde = trataFormulaPorcentagem($indicador['formulash'],"d",$where_reg,$regiao_igual);
				$qtde = $arrQTde['coluna'];
				$join = $arrQTde['join'];
				$sql = str_replace("WHERE"," $join WHERE ",$sql);
				$arrGB = explode("left join",$join);
				//dbg($arrGB);
				if($arrGB){
					foreach($arrGB as $n => $a){
						if($a)
							$group_setor.= "dsh$n.qtde,";
					}
				}
			}else{
				$qtde = trataFormulaPorcentagem($indicador['formulash'],"d",$where_reg);
			}
			$sql = str_replace("sum(d.qtde)","($qtde)",$sql);
			$sql = str_replace("GROUP BY reg.regcod","GROUP BY dpeid,reg.regcod",$sql);
			$sql = str_replace("GROUP BY est.estuf","GROUP BY dpeid,est.estuf",$sql);
			$sql = str_replace("GROUP BY mes.mescod","GROUP BY dpeid, mes.mescod",$sql);
			$sql = str_replace("GROUP BY mic.miccod","GROUP BY dpeid, mic.miccod",$sql);
			$sql = str_replace("GROUP BY mun.muncod","GROUP BY dpeid, mun.muncod",$sql);
			$sql = str_replace("GROUP BY zon.zonid","GROUP BY dpeid, zon.zonid",$sql);
			$sql = str_replace("GROUP BY sub.subid","GROUP BY dpeid, sub.subid",$sql);
			$sql = str_replace("GROUP BY dis.disid","GROUP BY dpeid, dis.disid",$sql);
			$sql = str_replace("GROUP BY s.setid","GROUP BY d.dpeid,s.setid",$sql);
			$sql = str_replace("GROUP BY d.dpeid,s.setid","GROUP BY $group_setor d.dpeid,s.setid",$sql);
			$sql = str_replace(" indcumulativovalor,"," d.indcumulativovalor,",$sql);
		}
		$soma = "N";
	}
	//dbg(simec_htmlentities($sql));
	monta_lista_detalhamento($sql,$cabec,6000,1,$soma,'100%',$monetario, $tam);
	
}


function detalharPorDetalhes($dados) {
	global $db;
	
	$sql = "SELECT * FROM painel.indicador WHERE indid='".$dados['indid']."' and indpublicado is true";
	$indicador = $db->pegaLinha($sql);
	
	$ult  = $db->pegaUm("SELECT dpe.dpeanoref FROM painel.seriehistorica seh 
					  	 LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid  
						 WHERE seh.indid='".$dados['indid']."' AND seh.sehstatus='A'");
	
	if($dados['iesid']) {
		$where  .= "d.dshcod='".$dados['iesid']."' AND ";
		$params .= "&dshcod=".$dados['iesid'];
	}
	
	if($dados['estuf']) {
		$where  .= "d.dshuf='".$dados['estuf']."' AND ";
		$params .= "&estuf=".$dados['estuf'];
	}
	if($dados['muncod']) {
		$where  .= "d.dshcodmunicipio='".$dados['muncod']."' AND ";
		$params .= "&muncod=".$dados['muncod'];
	}
	if($dados['entid']) {
		$where  .= "d.entid='".$dados['entid']."' AND ";
		$params .= "&entid=".$dados['entid'];
	}
	if($dados['esccodinep']) {
		$where  .= "d.dshcod='".$dados['esccodinep']."' AND ";
		$params .= "&esccodinep=".$dados['esccodinep'];
	}
	if($dados['unicod']) {
		$where .= "d.unicod='".$dados['unicod']."' AND ";
		$params .= "&unicod=".$dados['unicod'];
	}
	if($dados['polid']) {
		$where .= "d.polid='".$dados['polid']."' AND ";
		$params .= "&polid=".$dados['polid'];
	}
	if($dados['iecid']) {
		$where .= "d.iecid='".$dados['iecid']."' AND ";
		$params .= "&iecid=".$dados['iecid'];
	}
	if($dados['iepid']) {
		$where .= "d.iepid='".$dados['iepid']."' AND ";
		$params .= "&iepid=".$dados['iepid'];
	}
	
	if($dados['tpmid']) {
		$where .= "tpm.tpmid='".$dados['tpmid']."' AND ";
		$tipomunicipio_join = "INNER JOIN territorios.muntipomunicipio mtm ON mtm.muncod = mun.muncod AND mtm.estuf = est.estuf 
				 		 	   INNER JOIN territorios.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid";
		$tipomunicipio_join1 = "INNER JOIN territorios.muntipomunicipio mtm1 ON mtm1.muncod = mun1.muncod AND mtm1.estuf = est1.estuf 
				 		 	    INNER JOIN territorios.tipomunicipio tpm1 ON tpm1.tpmid = mtm1.tpmid"; 
		
		
	}
	if($dados['regcod']) {
		$where .= "reg.regcod='".$dados['regcod']."' AND ";
	}
	if($dados['mescod']) {
		$where .= "mes.mescod='".$dados['mescod']."' AND ";
	}
	if($dados['miccod']) {
		$where .= "mic.miccod='".$dados['miccod']."' AND ";
	}
	
	if($dados['dpes'][0]) {
		$sehstatus="sehstatus='A' OR sehstatus='H'";
		foreach($dados['dpes'] as $dpeinfo) {
			$dpe[] = $dpeinfo['dpeid'];
		}
		if($indicador['indcumulativo']=="S") {
			$where .= "d.dpeid='".$dpeinfo['dpeid']."' AND";
		} elseif($indicador['indcumulativo']=="N") {
			$where .= "d.dpeid IN('".implode("','",$dpe)."') AND";
		} else {
			$sehstatus="dpe.dpedatainicio >= '".date("Y")."-01-01' AND dpedatafim <= '31-12-".date("Y")."'";
		}
	} else {
		if($indicador['indcumulativo']=="S") {
			$sehstatus="sehstatus='A'";
		} elseif($indicador['indcumulativo']=="N") {
			$sehstatus="sehstatus='A' OR sehstatus='H'";
		} else {
			$sehstatus="dpe.dpedatainicio >= '".date("Y")."-01-01' AND dpedatafim <= '31-12-".date("Y")."'";
		}
		
	}
	
	switch($indicador['regid']) {
		case REGIONALIZACAO_UF:
			$territorios_join = "INNER JOIN territorios.estado est ON est.estuf=d.dshuf
								 INNER JOIN territorios.regiao reg ON reg.regcod=est.regcod";

			break;
		default:
			$territorios_join = "INNER JOIN territorios.municipio mun ON mun.muncod=d.dshcodmunicipio 
							  	 INNER JOIN territorios.estado est ON est.estuf=d.dshuf
							  	 INNER JOIN territorios.regiao reg ON reg.regcod=est.regcod
							  	 INNER JOIN territorios.mesoregiao mes ON mes.mescod = mun.mescod 
								 INNER JOIN territorios.microregiao mic ON mic.miccod = mun.miccod";

	}
	
	if($indicador['unmid'] == UNIDADEMEDICAO_MOEDA) {
		$formato = "999g999g999g999d99";
		$monetario = "S";
	} else {
		$formato = "9999999999999";
		$monetario = "N";
	}
	
	// quando for subnivel dos detalhes
	if($dados['tdiid'] && $dados['tidid']) {
		
		$numero = $db->pegaUm("SELECT tdinumero FROM painel.detalhetipodadosindicador d 
							   INNER JOIN painel.detalhetipoindicador dd ON d.tdiid=dd.tdiid 
							   WHERE d.tidid='".$dados['tidid']."'");
		
		$sql = "SELECT tiddsc, trim(to_char(sum(qtde),'".$formato."')) as qtde, CASE WHEN indqtdevalor = TRUE THEN to_char(sum(valor), '999g999g999g999d99') END as valor FROM (
					SELECT case when d.tdiid1=".$dados['tdiid']." then tiddsc1 when d.tdiid2=".$dados['tdiid']." then tiddsc2 end as tiddsc,
					 	   case when d.tdiid1=".$dados['tdiid']." then tidid1 when d.tdiid2=".$dados['tdiid']." then tidid2 end as tidid,
					case 	when d.indcumulativo='S' then sum(d.qtde)
						when d.indcumulativo='N' then
							case when d.sehstatus='A' then sum(d.qtde)
							else 0 end
						when d.indcumulativo='A' then
							case when d.dpeanoref='".$ult."' then sum(d.qtde)
							else 0 end
						end as qtde,
					case 	when d.indcumulativovalor='S' then sum(d.valor)
						when d.indcumulativovalor='N' then
							case when d.sehstatus='A' then sum(d.valor)
							else 0 end
						when d.indcumulativovalor='A' then
							case when d.dpeanoref='".$ult."' then sum(d.valor)
							else 0 end
						end as valor,
						d.indqtdevalor
						from painel.v_detalheindicadorsh d
							".$territorios_join." 
							".$tipomunicipio_join."
							WHERE ".$where." d.indid=".$dados['indid']." AND (d.tdiid1=".$dados['tdiid']." or d.tdiid2=".$dados['tdiid'].") AND d.tidid".$numero."='".$dados['tidid']."'
							GROUP BY d.tiddsc1, d.tidid1, d.tidid2, d.tiddsc2, d.tdiid1, d.tdiid2, d.indcumulativo, d.indcumulativovalor,d.sehstatus, d.dpeanoref,d.indqtdevalor
							ORDER BY 1
					
					) foo 
					
					GROUP BY tiddsc, indqtdevalor, tidid";
		
		
		$cabec[] = array("Detalhes","Qtde");
		
		if($indicador['indqtdevalor'] == "t") {
			$cabec[] = "Qtde";
			$cabec[] = "R$";
			$tam = array('60%','20%','20%');
		} else {
			if($monetario == 'S') $cabec[] = "R$";
			else $cabec[] = "Qtde";
			$tam = array('60%','40%');
		}
		
		monta_lista_detalhamento($sql,$cabec,900,1,'S','100%',$monetario, $tam);
		
	} else {
		
		$detalhes = $db->carregar("SELECT tdiid, tdidsc FROM painel.detalhetipoindicador WHERE indid='".$dados['indid']."' AND tdiid!='".$dados['tdiid']."'");
		if($detalhes[0]) {
			$params .= "&tdiid=".$detalhes[0]['tdiid'];
			$tiddsc = "'<img style=\"cursor:pointer;\" src=\"../imagens/mais.gif\" title=\"mais\" onclick=\"detalharPorSubDetalhes(this,\'".$indicador['acaid']."_".$dados['indid']."\', \'".$params."\', \''|| tidid ||'\')\"> '||tiddsc";
		} else {
			$tiddsc = "tiddsc";
		}

	
		
		$sql = "SELECT ".$tiddsc.", to_char(sum(qtde),'".$formato."') as qtde, CASE WHEN indqtdevalor = TRUE THEN to_char(sum(valor), '999g999g999g999d99') END as valor FROM (
					SELECT case when d.tdiid1=".$dados['tdiid']." then tiddsc1 when d.tdiid2=".$dados['tdiid']." then tiddsc2 end as tiddsc,
					 	   case when d.tdiid1=".$dados['tdiid']." then tidid1 when d.tdiid2=".$dados['tdiid']." then tidid2 end as tidid,
					case 	when d.indcumulativo='S' then sum(d.qtde)
						when d.indcumulativo='N' then
							case when d.sehstatus='A' then sum(d.qtde)
							else 0 end
						when d.indcumulativo='A' then
							case when d.dpeanoref='".$ult."' then sum(d.qtde)
							else 0 end
						end as qtde,
					case 	when d.indcumulativovalor='S' then sum(d.valor)
						when d.indcumulativovalor='N' then
							case when d.sehstatus='A' then sum(d.valor)
							else 0 end
						when d.indcumulativovalor='A' then
							case when d.dpeanoref='".$ult."' then sum(d.valor)
							else 0 end
						end as valor,
						d.indqtdevalor
						from painel.v_detalheindicadorsh d
							".$territorios_join." 
							".$tipomunicipio_join."
							WHERE ".$where." d.indid=".$dados['indid']." AND (d.tdiid1=".$dados['tdiid']." or d.tdiid2=".$dados['tdiid'].")
							GROUP BY d.tiddsc1, d.tidid1, d.tidid2, d.tiddsc2, d.tdiid1, d.tdiid2, d.indcumulativo, d.indcumulativovalor,d.sehstatus, d.dpeanoref,d.indqtdevalor
							ORDER BY 1
					
					) foo 
					GROUP BY tiddsc, indqtdevalor, tidid";
		
		$cabec[] = "Detalhes";
		
		if($indicador['indqtdevalor'] == "t") {
			$cabec[] = "Qtde";
			$cabec[] = "R$";
			$tam = array('60%','20%','20%');
		} else {
			if($monetario == 'S') $cabec[] = "R$";
			else $cabec[] = "Qtde";
			$tam = array('60%','40%');
		}
		
		monta_lista_detalhamento($sql,$cabec,900,1,'S','100%',$monetario, $tam);
	}
			
}

function retornaEstUfMuncodGrafico($detalhe,$valor){
	global $db;
	
	switch($detalhe){
		case "pais":
			return array("estuf" => "", "muncod" => "");
		break;
		
		case "estado":
			return array("estuf" => $valor, "muncod" => "");
		break;
		
		case "municipio":
			$sql = "select estuf from territorios.municipio where muncod = '$valor'";
			$estuf = $db->pegaUm($sql);
			return array("estuf" => $estuf, "muncod" => $valor);
		break; 
		
		case "escola":
			return array("estuf" => "", "muncod" => "");
		break;
		
		case "ies":
			return array("estuf" => "", "muncod" => "");
		break;
		
		case "posgraduacao":
			return array("estuf" => "", "muncod" => "");
		break;
		
		case "universidade":
			return array("estuf" => "", "muncod" => "");
		break;
		
		case "instituto":
			return array("estuf" => "", "muncod" => "");
		break;
		
		case "hospital":
			return array("estuf" => "", "muncod" => "");
		break;
		
		case "campussuperior":
			return array("estuf" => "", "muncod" => "");
		break;
		
		case "campusprofissional":
			return array("estuf" => "", "muncod" => "");
		break;
		
		default:
			return array("estuf" => "", "muncod" => "");
	}	
}



function monta_lista_detalhamento($sql,$cabecalho="",$perpage,$pages,$soma='N',$largura='95%', $valormonetario='S', $cabectamanho=false, $static=false) {
	global $db;
	
	if(!(bool)$largura) $largura = '95%';
	// este método monta uma listagem na tela baseado na sql passada
	//Registro Atual (instanciado na chamada)
	if ($_REQUEST['numero']=='') $numero = 1; else $numero = intval($_REQUEST['numero']);

    if (is_array($sql))
        $RS = $sql;
    else
        $RS = $db->carregar($sql);

	$nlinhas = $RS ? count($RS) : 0;
	if (! $RS) $nl = 0; else $nl=$nlinhas;
	if (($numero+$perpage)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $numero+$perpage-1;
	print '<table width="'. $largura . '" align="center" border="0" cellspacing="0" cellpadding="0" style="color:333333;" class="listagem">';
	if ($nlinhas>0)
	{
		//Monta Cabeçalho
		if(is_array($cabecalho))
		{
			if($static) print '<thead>';
			print '<tr>';
			for ($i=0;$i<count($cabecalho);$i++)
			{
				print '<td align="center" valign="top" class="title" style="'.(($cabectamanho[$i])?'width:'.$cabectamanho[$i].';':'').'border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">'.((!$i)?"TOTAL DE REGISTROS : ".$nlinhas:"")." ".(is_array($cabecalho[$i]) ? "" : $cabecalho[$i] ).'</td>';
			}
			print '</tr>';
			if($static) print '</thead>';
		}

		//Monta Listagem
		$totais = array();
		$tipovl = array();
		
		if($static) print '<tbody style='.$static['tbody'].'>';
		for ($i=($numero-1);$i<$reg_fim;$i++)
		{
			$c = 0;
			if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
			print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';
			foreach($RS[$i] as $k=>$v) {
				// fim - Clausula que faz soma numeros monetários
				if(isset($v)) {
					if (is_numeric(str_replace(array(".",","),array("","."),$v))) {
						//cria o array totalizador
						if (!$totais['0'.$c]) {$coluna = array('0'.$c => str_replace(array(".",","),array("","."),$v)); $totais = array_merge($totais, $coluna);} else $totais['0'.$c] = $totais['0'.$c] + str_replace(array(".",","),array("","."),$v);
						//Mostra o resultado
						if (strpos(str_replace(array(".",","),array("","."),$v),'.')) {
							$v = number_format(str_replace(array(".",","),array("","."),$v), 2, ',', '.'); 
							if (!$tipovl['0'.$c]) {
								$coluna = array('0'.$c => 'vl'); 
								$tipovl = array_merge($totais, $coluna);
							} else $tipovl['0'.$c] = 'vl';
						} else {
							$v = number_format($v, 0, '', '.');
						}
						if ($v<0) print '<td align="right" style="color:#cc0000;" title="'.$cabecalho[$c].'">('.$v.')'; else print '<td align="right" style="color:#999999;" title="'.$cabecalho[$c].'">'.(($v)?$v:"&nbsp;");
						print ('<br>'.$totais[$c]);
					} else {
						print '<td title="'.$cabecalho[$c].'">'.(($v)?$v:"&nbsp;");
					}
					print '</td>';
					$c = $c + 1;
				}
			}
			print '</tr>';
		}
		if($static) print '</tbody>';
				
		$somarCampos = $soma!='S' && is_array($soma) && (@count($soma)>0);
		
		if ($soma=='S' || $somarCampos){
			//totaliza (imprime totais dos campos numericos)
			if($static) print '<tfoot>';			
			print '<tr>';
			for ($i=0;$i<$c;$i++)
			{
				print '<td align="right" title="'.$cabecalho[$i].'">';
				if ($i==0) print 'Totais:   ';
				if(($somarCampos && $soma[$i]) || $soma=='S') {
					if (is_numeric($totais['0'.$i])) 
						if($valormonetario == 'S' || $i > 1){
							print number_format($totais['0'.$i], 2, ',', '.'); 
						}else{
							print number_format($totais['0'.$i], 0, '', '.'); 
						}
					else 
						print $totais['0'.$i];
				}
				print '</td>';
			}
			print '</tr>';
			//fim totais
			if($static) print '</tfoot>';
		}

	}
	else {
		print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
	}
	print '</table>';
}



function listaIndicadores($dados) {
	global $db, $filtro1;
	
	if($dados['indid']) {
		$sql = "SELECT * FROM painel.indicador WHERE indid='".$dados['indid']."'";
		$dadosi = $db->pegaLinha($sql);
		$estrutura = getEstruturaRegionalizacao($dadosi['regid']);
		$estloop = $estrutura;
	}
	
	if($estloop) {
		
		do {
			$estor[] = $estloop['atu'];
			$estloop = $estloop['sub'];
		} while ($estloop['atu']);
	
		for($i=(count($estor)-1);$i >= 0;$i--) {
			$estruturanomeordenada[] = "<b>".$estor[$i]['regdescricao']."</b>";
			
			if($estor[$i]['rgavisao'] == $dados['detalhes']) {
				if($estor[$i]['regid'] != REGIONALIZACAO_BRASIL){
					$filtro1 .= str_replace(array("{".$estor[$i]['rgaidentificador']."}"),array($dados[$estor[$i]['rgaidentificador']]),$estor[$i]['rgafiltro']);
				}
				$dadosreg = $db->pegaLinha(str_replace(array("{".$estor[$i]['rgaidentificador']."}"),array($_REQUEST[$estor[$i]['rgaidentificador']]),$estor[$i]['regsql']));
				
				$icones = str_replace(array("{ano}","{estuf}","{municipiocod}","{entnumcpfcnpj}","{estcod}","{anos}","{mundescricao}","{muncod}","{muncodcompleto}","{unicod}","{entid}","{estdescricao}"),array(date("Y"),$dadosreg['estuf'],substr($dadosreg['muncod'],0,6),$dadosreg['entnumcpfcnpj'],$dadosreg['estcod'],(date("Y")-1),str_replace(Array(' ','\''),Array('_',''),$dadosreg['mundescricao']),$dadosreg['muncod'],$dadosreg['muncodcompleto'],$dadosreg['unicod'],$dadosreg['entid'],$dadosreg['estdescricao']),stripslashes($estor[$i]['regicones']));
				
			}
		}
		
	} else {
		
		$qry = "SELECT * FROM painel.regagreg rga LEFT JOIN painel.regionalizacao reg ON reg.regid=rga.regid WHERE rgavisao='".$dados['detalhes']."'";
		$rga = $db->pegaLinha($qry); 
		
		$dadosreg = $db->pegaLinha(str_replace(array("{".$rga['rgaidentificador']."}"),array($dados[$rga['rgaidentificador']]),$rga['regsql']));
		if($rga['regid'] != REGIONALIZACAO_BRASIL){
			if($rga['rgafiltroreg']) {
				$filtro1  = str_replace(array("{".$rga['rgaidentificador']."}"),array($dados[$rga['rgaidentificador']]),$rga['rgafiltroreg']);
			} else {
				$filtro1  = str_replace(array("{".$rga['rgaidentificador']."}"),array($dados[$rga['rgaidentificador']]),$rga['rgafiltro']);
			}
		}	
		
		$estrutura = getEstruturaRegionalizacao($rga['regid']);
		
		$dadosreg = $db->pegaLinha(str_replace(array("{".$rga['rgaidentificador']."}"),array($dados[$rga['rgaidentificador']]),$rga['regsql']));
		
		$icones = str_replace(array("{ano}","{estuf}","{municipiocod}","{entnumcpfcnpj}","{estcod}","{anos}","{mundescricao}","{muncod}","{muncodcompleto}","{unicod}","{entid}","{estdescricao}"),array(date("Y"),$dadosreg['estuf'],substr($dadosreg['muncod'],0,6),$dadosreg['entnumcpfcnpj'],$dadosreg['estcod'],(date("Y")-1),str_replace(Array(' ','\''),Array('_',''),$dadosreg['mundescricao']),$dadosreg['muncod'],$dadosreg['muncodcompleto'],$dadosreg['unicod'],$dadosreg['entid'],$dadosreg['estdescricao']),stripslashes($rga['regicones']));
		
		
		$estloop = $estrutura;
		
		do {
			$estor[] = $estloop['atu'];
			$estloop = $estloop['sub'];
		} while ($estloop['atu']);
	
		for($i=(count($estor)-1);$i >= 0;$i--) {
			$estruturanomeordenada[] = "<b>".$estor[$i]['regdescricao']."</b>";
		}
		
	}
	
	if($dados['indpne']){
		$fitropne = " AND d.indid IN (".$dados['indpne'].") ";
	}else{
		$fitropne = "";
	}

	$sql = "SELECT indid, acaid, unmid, foo.regid, indcumulativo, indcumulativovalor, indnome, sum(qtde) as qtde, indqtdevalor, CASE WHEN indqtdevalor = TRUE THEN to_char(sum(valor), '999g999g999g999d99') ELSE '-' END as valor, secdsc, umedesc, regdescricao from(
				SELECT d.indid, acaid, d.unmid, d.indnome, d.secid, d.umeid, d.regid, d.indcumulativo, d.indcumulativovalor, d.indqtdevalor, CASE WHEN d.indcumulativo='S' THEN sum(d.qtde)
					WHEN d.indcumulativo='N' THEN
						CASE WHEN d.sehstatus='A' THEN sum(d.qtde)
						ELSE 0 END
					WHEN d.indcumulativo='A' THEN
						CASE when d.dpeanoref=( SELECT dd.dpeanoref FROM painel.seriehistorica ss 
									   INNER JOIN painel.detalheperiodicidade dd ON dd.dpeid=ss.dpeid 
									   WHERE ss.indid = d.indid AND ss.sehstatus='A') THEN sum(d.qtde)
						ELSE 0 END
					END as qtde,
				CASE 	WHEN d.indcumulativovalor='S' THEN sum(d.valor)
					WHEN d.indcumulativovalor='N' THEN
						CASE when d.sehstatus='A' THEN sum(d.valor)
						ELSE 0 END
					WHEN d.indcumulativovalor='A' then
						CASE when d.dpeanoref=( SELECT dd.dpeanoref FROM painel.seriehistorica ss 
									   INNER JOIN painel.detalheperiodicidade dd ON dd.dpeid=ss.dpeid 
									   WHERE ss.indid = d.indid AND ss.sehstatus='A') THEN sum(d.valor)
						ELSE 0 end
					END as valor
				FROM painel.v_detalheindicadorsh d 
				LEFT JOIN territorios.municipio mun ON mun.muncod = d.dshcodmunicipio 
				LEFT JOIN territorios.estado est ON est.estuf = d.dshuf 
				LEFT JOIN territorios.regiao reg ON reg.regcod = est.regcod  
				LEFT JOIN territorios.mesoregiao mes ON mes.mescod = mun.mescod 
				LEFT JOIN territorios.microregiao mic ON mic.miccod = mun.miccod
				LEFT JOIN territorios.pais pa ON pa.paiid = reg.paiid
				WHERE d.acaid=".$dados['acaid']." ".$fitropne. " AND d.indpublicado=TRUE AND d.indstatus='A'
				".$filtro1." GROUP BY d.indid,acaid, d.unmid,d.indnome,d.indcumulativo,d.indcumulativovalor,d.sehstatus,d.dpeanoref,d.secid,d.umeid,d.regid,d.indqtdevalor
				) foo 
				INNER JOIN painel.secretaria sec ON sec.secid=foo.secid 
				INNER JOIN painel.unidademeta ume ON ume.umeid=foo.umeid
				INNER JOIN painel.regionalizacao reg ON reg.regid=foo.regid 
				GROUP BY indid, foo.unmid, acaid, indnome, secdsc, umedesc, regdescricao, indcumulativovalor, indcumulativo, indqtdevalor, foo.regid 
				ORDER BY indid";
	$arrIndInfo = $db->carregar($sql);
	
	if($arrIndInfo[0]) {
		foreach($arrIndInfo as $indicador) {
			if(!$indicador['qtde']) {
				$sql = "SELECT qtde FROM painel.v_detalheindicadorsh d 
						LEFT JOIN territorios.municipio mun ON mun.muncod = d.dshcodmunicipio 
						LEFT JOIN territorios.estado est ON est.estuf = d.dshuf 
						LEFT JOIN territorios.regiao reg ON reg.regcod = est.regcod  
						LEFT JOIN territorios.mesoregiao mes ON mes.mescod = mun.mescod 
						LEFT JOIN territorios.microregiao mic ON mic.miccod = mun.miccod
						WHERE d.indid=".$indicador['indid']." AND d.indpublicado=TRUE AND d.indstatus='A' ".$filtro1." ORDER BY dpedatainicio DESC LIMIT 1";
				$indicador['qtde'] = $db->pegaUm($sql);
			}
			
			if($dados['detalhes'] && !$rga['rgaidentificador'])
				$rga['rgaidentificador'] = $db->pegaUm("select rgaidentificador from painel.regagreg where rgavisao = '".$dados['detalhes']."'");
			$html .= processarLinhaDetalhamentoIndicadores($indicador, array('detalhes' => $dados['detalhes'], 
																			 'rgaidentificador' => $rga['rgaidentificador']));
		}
	}
	
	echo "<table cellSpacing=0 cellPadding=3 class=listagem style=\"width:100%;color:#888888;\">";
	echo "<tr>";
	echo "<td class=\"SubTituloCentro\">&nbsp;</td>";
	echo  "<td class=\"SubTituloCentro\">Cod</td>";
	echo "<td class=\"SubTituloCentro\">Nome do indicador</td>";
	echo "<td class=\"SubTituloCentro\">Secretaria</td>";
	echo "<td class=\"SubTituloCentro\">Regionalização</td>";
	echo "<td class=\"SubTituloCentro\">Produto</td>";
	echo "<td class=\"SubTituloCentro\">Qtde</td>";
	echo "<td class=\"SubTituloCentro\">R$</td>";
	echo "</tr>";
	echo str_replace("style=\"display:none;\"","",$html);
	echo "</table>";
	
}


function exibeMapaDetalhamento($dados) {
	global $db;
	
	$ano  = $db->pegaUm("SELECT dpe.dpeanoref FROM painel.seriehistorica seh 
					  	 LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid  
						 WHERE seh.indid='".$dados['indid']."' AND seh.sehstatus='A'");
	
	
	$sql = "SELECT muncod, mundescricao, estuf, munmedlat, munmedlog, CASE WHEN unmid IN('".UNIDADEMEDICAO_MOEDA."','".UNIDADEMEDICAO_PERCENTUAL."','".UNIDADEMEDICAO_RAZAO."') THEN CASE WHEN unmid='".UNIDADEMEDICAO_MOEDA."' THEN 'R$ '||trim(to_char(SUM(qtde), '999g999g999g999d99')) ELSE trim(to_char(SUM(qtde), '999g999g999g999d99')) END ELSE trim(to_char(SUM(qtde), '999g999g999g999')) END as qtde, CASE WHEN indqtdevalor = TRUE THEN trim(to_char(sum(valor), '999g999g999g999d99')) END as valor FROM (
			SELECT	mun.muncod,
					mun.mundescricao,
					est.estuf,
	 
					st_y(mun.munlatlong) as munmedlat,
	        
			        st_x(mun.munlatlong) as munmedlog,
			       
			       
				CASE 	WHEN d.indcumulativo='S' THEN sum(d.qtde)
					WHEN d.indcumulativo='N' THEN
						CASE WHEN d.sehstatus='A' THEN sum(d.qtde)
						ELSE 0 END
					WHEN d.indcumulativo='A' THEN
						CASE when d.dpeanoref='{$ano}' THEN sum(d.qtde)
						ELSE 0 END
					END as qtde,
				CASE 	WHEN d.indcumulativovalor='S' THEN sum(d.valor)
					WHEN d.indcumulativovalor='N' THEN
						CASE when d.sehstatus='A' THEN sum(d.valor)
						ELSE 0 END
					WHEN d.indcumulativovalor='A' then
						CASE when d.dpeanoref='{$ano}' THEN sum(d.valor)
						ELSE 0 end
					END as valor,
				d.indqtdevalor,
				d.unmid

	       FROM painel.v_detalheindicadorsh d 
	       LEFT JOIN territoriosgeo.municipio mun ON mun.muncod = d.dshcodmunicipio 
	       LEFT JOIN territorios.estado est ON est.estuf = d.dshuf
	       LEFT JOIN territorios.mesoregiao mes ON mes.mescod = mun.mescod
	       WHERE d.indid='".$dados['indid']."' ".md5_decrypt($dados['filtro'])."
		   GROUP BY mun.muncod, est.estuf, mundescricao, mun.munlatlong, d.indcumulativo, d.sehstatus, d.dpeanoref, d.indcumulativovalor, d.indqtdevalor, d.unmid
	       ORDER BY est.estuf, mundescricao
		) as foo 
		WHERE qtde!=0 OR valor!=0 
		GROUP BY muncod, mundescricao, estuf, munmedlat, munmedlog, indqtdevalor, unmid 
		ORDER BY estuf, mundescricao";
	
	$dados = $db->carregar($sql);
	
	if($dados[0]):
				
		$conteudo .= "<markers> "; // inicia o XML
				
		foreach($dados as $d):
		
			$conteudo .= "<marker "; //inicia um ponto no mapa
			$conteudo .= "mundsc=\"". simec_htmlspecialchars($d['mundescricao']) ."\" "; // adiciona a descrição do município;
			$conteudo .= "muncod=\"". $d['muncod'] ."\" "; // adiciona muncod;
			$conteudo .= "estuf=\"". $d['estuf'] ."\" "; // adiciona UF;
			$conteudo .= "qtde=\"". $d['qtde'] ."\" "; // adiciona UF;
			$conteudo .= "valor=\"". $d['valor'] ."\" "; // adiciona UF;
			$conteudo .= "lat=\"".$d['munmedlat']."\" "; // adiciona a latitude;
			$conteudo .= "lng=\"".$d['munmedlog']."\" "; //adiciona a longitude;
			$conteudo .= "/> ";
				
		endforeach;
				
		$conteudo .= "</markers> ";
		
		ob_clean();
		print $conteudo;
				
	endif;
	
}


function exibeInformacoesMapa($dados) {
	global $db;
	
	ob_clean();
	
	$ano  = $db->pegaUm("SELECT dpe.dpeanoref FROM painel.seriehistorica seh 
					  	 LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid  
						 WHERE seh.indid='".$dados['indid']."' AND seh.sehstatus='A'");
	
	$regid = $db->pegaUM("select regid from painel.indicador where indid = {$dados['indid']}");
	
	$sql = "SELECT '<a style=cursor:pointer; onclick=abrebalao(\''||muncod||'\')>'||mundescricao||'</a>' as mundescricao, estuf, SUM(qtde) as qtde, CASE WHEN indqtdevalor = TRUE THEN sum(valor) END as valor FROM (
		SELECT	mun.muncod,
				mun.mundescricao,
				est.estuf,
				CASE 	WHEN d.indcumulativo='S' THEN sum(d.qtde)
					WHEN d.indcumulativo='N' THEN
						CASE WHEN d.sehstatus='A' THEN sum(d.qtde)
						ELSE 0 END
					WHEN d.indcumulativo='A' THEN
						CASE when d.dpeanoref='{$ano}' THEN sum(d.qtde)
						ELSE 0 END
					END as qtde,
				CASE 	WHEN d.indcumulativovalor='S' THEN sum(d.valor)
					WHEN d.indcumulativovalor='N' THEN
						CASE when d.sehstatus='A' THEN sum(d.valor)
						ELSE 0 END
					WHEN d.indcumulativovalor='A' then
						CASE when d.dpeanoref='{$ano}' THEN sum(d.valor)
						ELSE 0 end
					END as valor,
				d.indqtdevalor,
				d.unmid
       FROM painel.v_detalheindicadorsh d 
       LEFT JOIN territorios.municipio mun ON mun.muncod = d.dshcodmunicipio
       LEFT JOIN territorios.estado est ON est.estuf = d.dshuf 
       WHERE d.indid='".$dados['indid']."' ".md5_decrypt($dados['filtro'])."
	   GROUP BY mun.muncod, est.estuf, mundescricao, d.indcumulativo, d.sehstatus, d.dpeanoref, d.indcumulativovalor, d.indqtdevalor, d.unmid
       ORDER BY est.estuf, mundescricao
	) as foo 
	WHERE qtde!=0 OR valor!=0 
	GROUP BY muncod, mundescricao, estuf, indqtdevalor, unmid 
	ORDER BY estuf, mundescricao";
//	ver($sql,d);
	// Inclui componente de relatórios
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	// instancia a classe de relatório
	$rel = new montaRelatorio();
	
	$indicador = $db->pegaLinha("SELECT i.unmid, i.indqtdevalor,u.umedesc, i.regid FROM painel.indicador i 
								 LEFT JOIN painel.unidademeta u ON u.umeid=i.umeid 
								 WHERE indid='".$dados['indid']."'");
	
	if($indicador['regid'] == REGIONALIZACAO_UF) {
		$agp = array("agrupador" => array(0 => array("campo" => "estuf", "label" => "UF")),
					 "agrupadoColuna" => array("qtde", "valor")
										 
					);
		
	} else {
		$agp = array("agrupador" => array(0 => array("campo" => "estuf", "label" => "UF"),
										  1 => array("campo" => "mundescricao", "label" => "Municipio")),
		
					"agrupadoColuna" => array("qtde", "valor")
										 
					);
		
	}
	
	if($indicador['unmid'] == UNIDADEMEDICAO_MOEDA) {
		
		$coluna[] = array("campo" 	  => "qtde",
			   		   	  "label" 	  => "Quantidade");
		
	} elseif($indicador['unmid'] == UNIDADEMEDICAO_PERCENTUAL || $indicador['unmid'] == UNIDADEMEDICAO_RAZAO) {
		
		if($regid == REGIONALIZACAO_UF){
			$coluna[] = array("campo" 	  => "qtde",
			   		   	  "label" 	  => $indicador['umedesc']);
			//$rel->setTolizadorLinha(true);			
		}else{
			$coluna[] = array("campo" 	  => "qtde",
			   		   	  "label" 	  => $indicador['umedesc'],
						  "blockAgp"  => "estuf");
			//$rel->setTolizadorLinha(false);
		}
		
	} else {

		$coluna[] = array("campo" 	  => "qtde",
			   		   	  "label" 	  => $indicador['umedesc'],
						  "type"      => "numeric");
		
		
	}
	
	if($indicador['indqtdevalor'] == "t") {
		if($indicador['unmid'] == UNIDADEMEDICAO_PERCENTUAL || 
		   $indicador['unmid'] == UNIDADEMEDICAO_RAZAO) {
			
		   	if($regid == REGIONALIZACAO_UF){
		   		$coluna[] = array("campo" 	  => "valor",
				   		   	  "label" 	  => "Valor");
		   	}else{
		   		$coluna[] = array("campo" 	  => "valor",
				   		   	  "label" 	  => "Valor",
							  "blockAgp"  => "estuf");
		   	}
		   	
		} else {
			
			$coluna[] = array("campo" 	  => "valor",
				   		   	  "label" 	  => "Valor");
			
		}
	
	}
	
	$dados 	   = $db->carregar( $sql );
		
	$rel->setAgrupador($agp, $dados);
	if($indicador['unmid'] != UNIDADEMEDICAO_PERCENTUAL && $indicador['unmid'] != UNIDADEMEDICAO_RAZAO) {
		$rel->setMonstrarTolizadorNivel(true);
		$rel->setTextoTotalizador("Total:");
		$rel->setTotNivel(true);
	}else{
		$rel->setMonstrarTolizadorNivel(false);
		$rel->setTotNivel(false);
		$rel->setTolizadorLinha(false);		
	}
	$rel->setColuna($coluna);
	$rel->setOverFlowTableHeight('220');
	echo $rel->getRelatorio();
	
	
}

function exportarIndicadorExcel($indid){
    global $db;

    $filtro = md5_decrypt($indid['filtro']);
    $filtro = str_replace('d.','', $filtro);

    $sql = "SELECT dpe.dpedsc AS periodicidade, tid1.tiddsc AS detalhe1, tid2.tiddsc AS detalhe2, dsh.dshuf AS uf, dsh.dshcodmunicipio AS ibge, SUM(dsh.dshqtde) AS quantidade, SUM(dsh.dshvalor) AS valor
            FROM painel.indicador ind
            INNER JOIN painel.seriehistorica sh ON sh.indid = ind.indid
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            LEFT JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
            LEFT JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
            WHERE sh.indid = {$indid['indid']}
            $filtro
            AND sh.sehstatus <> 'I'
            GROUP BY dpe.dpeanoref, dpe.dpeordem, periodicidade, detalhe1, detalhe2, uf, ibge
            ORDER BY dpe.dpeanoref, dpe.dpeordem, periodicidade, detalhe1, detalhe2, uf, ibge";
    $dadosExcel = $db->carregar( $sql );
    ob_clean();
    if($dadosExcel){
        $arqcsv = "Periodicidade;Detalhe1;Detalhe2;UF;IBGE;Quantidade;Valor";
        $arqcsv .= "\n";
        foreach($dadosExcel as $csv){
            $arqcsv .= $csv['periodicidade'].';'.$csv['detalhe1'].';'.$csv['detalhe2'].';'.$csv['uf'].';'.$csv['ibge'].';'.number_format($csv['quantidade'],2,",",".").';'.number_format($csv['valor'],0,",",".");
            $arqcsv .= "\n";
        }
        header("Content-Type: text/html; charset=ISO-8859-1");
        header("Content-type: application/octet-stream");
        header("Content-Disposition: attachment; filename=\"Indicador.csv\"");
        echo $arqcsv;
        exit;
    }
    //$db->sql_to_excel( $dadosExcel, "Dados_Indicador" );
}
?>