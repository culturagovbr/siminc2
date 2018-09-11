<?php 
$indid = $_GET['indid'] ? $_GET['indid'] : $_SESSION['indid'];
$_SESSION['indid'] = $indid;

include (APPRAIZ.'www/painel/_funcoes_metas.php');

if($_POST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_POST['requisicaoAjax']();
	exit();
}

if(!$_SESSION['indid']){
	header("Location: painel.php?modulo=principal/lista&acao=A");
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$abacod_tela = 57279;
$db->cria_aba($abacod_tela,$url,'');

$titulo = "Metas do Indicador";
monta_titulo( $titulo, "&nbsp;" );
cabecalhoIndicador($_SESSION['indid']);

$arrInd = $db->pegaLinha("select ind.*, u.umedesc from painel.indicador ind inner join painel.unidademeta u ON u.umeid = ind.umeid where indid = $indid");

$sql = "select prtobj from public.parametros_tela where mnuid = {$_SESSION['mnuid']} and prtdsc = '$indid'";
$prtobj = $db->pegaUm($sql);

if($prtobj){
	$prtobj = unserialize($prtobj);
	extract($prtobj);
}

?>
<script language="javascript" type="text/javascript" src="/includes/open_flash_chart/swfobject.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../painel/js/metas_indicador.js"></script>

<script language="javascript" src="../includes/Highcharts-3.0.0/js/jquery.min.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

<script type="text/javascript" >
function exibirGraficoMeta()
{
	var indid	     = $("[name=indid]").val();
	var perid	     = $("[name=perid]").val();
	var dpeidinicio  = $("[name=dpeidinicio]").val();
	var dpeidfim     = $("[name=dpeidfim]").val();
	var projecao     = $("[name=projecao]").val();
	var projetar     = $("[name=rd_projetar]:checked").val();
	<?php if($arrInd['indqtdevalor'] == "t"): ?>
		var rd_qtdevalor = $("[name=rd_qtdevalor]:checked").val();
	<?php else: ?>
		var rd_qtdevalor = $("[name=rd_qtdevalor]").val();
	<?php endif; ?>
	var erro = false;
	
	if(!perid){
	 	alert('Selecione a periodicidade!');
	 	erro = 1;
	 	$("[name=perid]").focus();
	 	return false;
	}
	if(!dpeidinicio){
	 	alert('Selecione a data inicial!');
	 	erro = 1;
	 	$("[name=dpeidinicio]").focus();
	 	return false;
	}
	if(!projecao){
	 	alert('Selecione a data inicial da meta!');
	 	erro = 1;
	 	$("[name=projecao]").focus();
	 	return false;
	}
	
	if(!dpeidfim){
	 	alert('Selecione a data final da meta!');
	 	erro = 1;
	 	$("[name=dpeidfim]").focus();
	 	return false;
	}
	
	if(projecao != dpeidinicio){
		if(testaDataPorDpeid(projecao,dpeidinicio).responseText.search("false") >= 0){
			erro = 1;
			alert('A data inicial de meta deve ser menor ou igual a data inicial!');
			return false;
		}
	}
	
	if(dpeidfim != projecao){
		if(testaDataPorDpeid(dpeidfim,projecao).responseText.search("false") >= 0){
			erro = 1;
			alert('A data inicial de meta deve ser menor ou igual a data final de meta!');
			return false;
		}
	}
	
	if(rd_qtdevalor == "valor"){
		var finac_qtde = "";
		var finac_valor = 1;
	}if(rd_qtdevalor == "ambos"){
		var finac_qtde = 1;
		var finac_valor = 1;
	}if(rd_qtdevalor == "qtde" || rd_qtdevalor == "" || !rd_qtdevalor){
		var finac_qtde = 1;
		var finac_valor = "";
	}
	
	if(erro == false){
		var geraGrafico = "geraGrafico.php?tipo=meta;indid=" + indid + ";dpeid=" + dpeidinicio + ";dpeid2=" + dpeidfim + ";finac_qtde=" + finac_qtde + ";finac_valor=" + finac_valor + ";projecao=" + projecao + ";periodicidade=" + perid + ";projetar=" + projetar;
		<?php if(TIPO_GRAFICO == TIPO_GRAFICO_HIGHCHARTS):?>
			geraGraficoHighCharts(geraGrafico);									
		<?php elseif(TIPO_GRAFICO == TIPO_GRAFICO_FLASH):?>
			var altura = "250";
			var largura = "100%";
			swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_meta", largura, altura, "9.0.0", "expressInstall.swf", {"data-file":geraGrafico,"loading":"Carregando gráfico..."} );
		<?php endif; ?>
		
	}
	
}

function geraGraficoHighCharts(dados)
{
	dados = dados.replace("geraGrafico.php?","");
	document.getElementById('grafico_meta').innerHTML = "<center>Carregando...</center>";
	
	jQuery.ajax({
	  type: "POST",
	  url: 'painel.php?modulo=principal/painel_controle&acao=A',
	  cache: false,
	  data: "&geraGraficoJpGraph=true&dados=" + dados,
	  success: function(msg){
	   		 jQuery("#grafico_meta").html(msg);
                         exibeLegenda();
	   		 jQuery("#grafico_meta").find("script").each(function(i) {
                    eval(jQuery(this).text());
                });
	   }
	});
	
}

function exibeLegenda(){	
	jQuery.ajax({
	  type: "POST",
	  url: 'painel.php?modulo=principal/cadastroEstrategiaMetas&acao=A',
	  cache: false,
	  data: "&req=consultar&indid=" + $("[name=indid]").val(),
	  success: function(msg){
                jQuery("#legenda_indicador").html(msg);
                jQuery("#Titulo_legenda").html("Legenda:");
	   }
	});  
}

function testaDataPorDpeid(dpeid1,dpeid2){

	 return $.ajax({
		   type: "POST",
		   url: 'painel.php?modulo=principal/painel_controle&acao=A',
		   async: false,
		   data: "testaDatasDpeid=true&dpeid1=" + dpeid1 + "&dpeid2=" + dpeid2,
		   success: function(msg){
		   }
		 });
}

function salvarExibicaoGraficoMeta()
{
	var parms = $("#form_meta").serialize();
	$.ajax({
		type: "POST",
		url: window.location,
		async: false,
		data: "requisicaoAjax=salvarExibicaoMeta&" + parms,
		success: function(msg){
			alert('Operação realizada com sucesso!');
		}
	});
}

function exibeOpcoes()
{
	//$('#tr_periodo_inicial').show();
	//$('#tr_projecao').show();
}

function escondeOpcoes()
{
	//$('#tr_periodo_inicial').hide();
	//$('#tr_projecao').hide();
}

function cadastrarNovaMetaMeta()
{
	window.location.href='painel.php?modulo=principal/cadastrarMetasIndicador&acao=A&novameta=<?php echo $_SESSION['indid'] ?>';
}

<?php if($prtobj): ?>
	$(function() {
		exibirGraficoMeta();
	});
<?php endif; ?>

</script>
<style>
	.SubtituloTabela{background-color:#cccccc}
	.negrito{font-weight:bold}
	.bold{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.direita{text-align: right;}
	.esquerda{text-align: left;}
	.link{cursor: pointer;}
	.hidden{display:none}
</style>
<?php $arrDados = getMetaIndicador()?>
<form name="form_meta" id="form_meta" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<?php if($arrDados): ?>
	<tr>
		<td width="30%" class="SubTituloDireita" >Periodicidade:</td>
		<td>
			<?php
				$sql = "select
							per.perid as codigo,
							per.perdsc as descricao
						from
							painel.periodicidade per
						where
							per.pernivel >= ( 
											select 
												min(per.pernivel)
											from
												painel.periodicidade per
											inner join
												painel.detalheperiodicidade dpe ON per.perid = dpe.perid
											inner join
												painel.seriehistorica seh ON seh.dpeid = dpe.dpeid
											where
												seh.indid = $indid
											and
												per.perstatus = 'A'
											and
												seh.sehstatus <> 'I'
										  )
						and
							per.perstatus = 'A'
						order by
							per.pernivel";
				$db->monta_combo('perid',$sql,"S",'Selecione a Periodicidade','','','','','','','',1);
			 ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Projetar?</td>
		<td>
			<input type="radio" onclick="exibeOpcoes()" name="rd_projetar" <?php echo !$rd_projetar || $rd_projetar == "sim" ? "checked=\"checked\"" : "" ?> value="sim" />Sim 
			<input type="radio" onclick="escondeOpcoes()" name="rd_projetar" <?php echo $rd_projetar == "nao" ? "checked=\"checked\"" : "" ?> value="nao" />Não
		</td>
	</tr>
	<tr id="tr_periodo_inicial" style="display:<?php echo $rd_projetar == "nao" ? "" : "" ?>" >
		<td width="30%" class="SubTituloDireita" >Período Inicial:</td>
		<td>
			<?php
				$sql = "select  
							dpe.dpeid as codigo,
							dpe.dpedsc as descricao
						from 
							painel.seriehistorica seh
						inner join
							painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
						where
							indid = $indid
						and
							sehstatus <> 'I'
						order by
							dpe.dpedatainicio asc";
                                $dados = $db->pegaUm($sql);
				$db->monta_combo('dpeidinicio',$sql,"S",'Selecione o Período','','','','','','','', $dados);
			 ?>
		</td>
	</tr>
	<tr id="tr_projecao" style="display:<?php echo $rd_projetar == "nao" ? "" : "" ?>" >
		<td class="SubTituloDireita" >Meta:</td>
		<td>
			<?php
				 
				$sql = "select  
							dpe.dpeid as codigo,
							dpe.dpedsc as descricao
						from 
							painel.detalheperiodicidade dpe
						where
							dpe.perid in ( select perid from painel.indicador where indid = $indid)
						and
							dpedatainicio >= ( 	select  
													dpe.dpedatainicio
												from 
													painel.seriehistorica seh
												inner join
													painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
												where
													indid = $indid
												and
													sehstatus <> 'I'
												order by
													dpe.dpedatainicio asc
												limit 1 
											  )
						and
							dpedatafim <=    ( 	select  
													dpe.dpedatafim
												from 
													painel.detalhemetaindicador dmi
												inner join
													painel.detalheperiodicidade dpe ON dpe.dpeid = dmi.dpeid
												inner join
													painel.metaindicador met ON met.metid = dmi.metid
												where
													met.indid = $indid
												order by
													dpe.dpedatainicio desc
												limit 1 
											  )
						order by
							dpe.dpedatainicio asc";
                                $dados = $db->pegaUm($sql);
				$db->monta_combo('projecao',$sql,"S",'Selecione o Período','','','','','','','', $dados);?> até
				<span id="span_dpeidfim"> 
				<?php 
				$sql = "select  
							dpe.dpeid as codigo,
							dpe.dpedsc as descricao
						from 
							painel.detalheperiodicidade dpe
						where
							/*dpe.perid in ( select perid from painel.indicador where indid = $indid)
						and*/
							dpeid in ( 	select  
													dmi.dpeid
												from 
													painel.detalhemetaindicador dmi
												inner join
													painel.metaindicador met ON met.metid = dmi.metid
												where
													met.indid = $indid
												and
													met.metstatus = 'A'
												and
													dmi.dmistatus = 'A'
											  )
						order by
							dpe.dpedatainicio asc";
				$dados = $db->carregar($sql);
                                $dados = $dados[count($dados)-1]['codigo'];
				$db->monta_combo('dpeidfim',$sql,"S",'Selecione o Período','','','','','','','',$dados);?>
				</span>
		</td>
	</tr>
	<?php if($arrInd['indqtdevalor'] == "t"): ?>
	<tr>
		<td class="SubTituloDireita" >Exibir:</td>
		<td>
			<input type="radio" name="rd_qtdevalor" value="qtde" /><?php echo $arrInd['umedesc'] ?> 
			<input type="radio" name="rd_qtdevalor" value="valor" />Valor 
			<input type="radio" name="rd_qtdevalor" checked="checked" value="ambos" />Ambos
		</td>
	</tr>
	<?php else: ?>
		<input type="hidden" name="rd_qtdevalor" checked="checked" value="qtde" />
	<?php endif; ?>
	<tr>
		<td class="SubtituloTabela" ></td>
		<td class="SubtituloTabela" >
			<input type="hidden" name="indid" value="<?php echo $indid ?>"/>
			<input type="button" name="btn_exibir" value="Exibir" onclick="exibirGraficoMeta()" />
			<?php
			//$arrPflcod = pegaPerfil();
			//if(!in_array(PAINEL_PERFIL_CONSULTA,$arrPflcod)){
			if($db->testa_superuser()){
			?>
				<input type="button" name="btn_salvar_view" value="Definir Visualização Padrão" onclick="salvarExibicaoGraficoMeta()" />
				<input type="button" name="btn_cadastrar_meta" value="Cadastrar Nova Meta" onclick="cadastrarNovaMetaMeta()" />
			<?
			}
			?>
		</td>
	</tr>
	<tr>
		<td width="30%" class="SubTituloDireita" ></td>
		<td>
			<div style="width:100%;height:100%;background-color:#FFF" id="grafico_meta" >
				<div id="grafico_meta" ></div>
			</div>
		</td>
	</tr>
	<tr>
		<td width="30%" class="SubTituloDireita" id="Titulo_legenda" ></td>
		<td>
			<div style="vertical-align: middle; width:100%;height:100%;background-color:#FFF" id="legenda_indicador" >
				<div id="legenda_indicador" ></div>
			</div>
		</td>
	</tr>        
<?php else: ?>
	<tr>
		<td class="SubtituloTabela bold center"><input type="button" name="btn_cadastrar_meta" value="Cadastrar Nova Meta" onclick="cadastrarNovaMetaMeta()" /></td>
	</tr>
	<tr>
		<td class="SubtituloTabela bold center">Não existe(m) meta(s) para este indicador.</td>
	</tr>
<?php endif; ?>
</table>
</form>
<script>
    exibirGraficoMeta();
</script>