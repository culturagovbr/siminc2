<?php
include_once "_funcoesdetalhamentoindicador.php";

/* configurações */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações */

?>
<html>
<head>
<meta http-equiv="Cache-Control" content="no-cache">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Connection" content="Keep-Alive">
<meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
<title><?= $titulo ?></title>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/jquery.min.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/painel.js"></script>
<script language="JavaScript" src="./js/detalhamentoindicador.js"></script>
<script language="javascript" type="text/javascript" src="/includes/open_flash_chart/swfobject.js"></script>

<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

<?php
function curPageURL() {
	$pageURL = 'http';
	if ($_SERVER["HTTPS"] == "on") {$pageURL .= "s";}
		$pageURL .= "://";
	if ($_SERVER["SERVER_PORT"] != "80") {
		$pageURL .= $_SERVER["SERVER_NAME"].":".$_SERVER["SERVER_PORT"].$_SERVER["REQUEST_URI"];
	} else {
		$pageURL .= $_SERVER["SERVER_NAME"].$_SERVER["REQUEST_URI"];
	}
	return $pageURL;
}

$local= explode("/",curPageURL());
?>
<?if ($local[2]=="simec.mec.gov.br" ){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxQhVwj8ALbvbyVgNcB-R-H_S2MIRxTIdhrqjcwTK3xxl_Nu_YMC5SdLWg" type="text/javascript"></script>
	<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxQhVwj8ALbvbyVgNcB-R-H_S2MIRxTIdhrqjcwTK3xxl_Nu_YMC5SdLWg"; ?>
<? } ?>
<?if ($local[2]=="simec-d.mec.gov.br"){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxRYtD8tuHxswJ_J7IRZlgTxP-EUtxT_Cz5IMSBe6d3M1dq-XAJNIvMcpg" type="text/javascript"></script>
	<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxRYtD8tuHxswJ_J7IRZlgTxP-EUtxT_Cz5IMSBe6d3M1dq-XAJNIvMcpg"; ?> 
<? } ?>
<?if ($local[2]=="simec" ){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxTNzTBk8zukZFuO3BxF29LAEN1D1xSIcGWxF7HCjMwks0HURg6MTfdk1A" type="text/javascript"></script>
	<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxTNzTBk8zukZFuO3BxF29LAEN1D1xSIcGWxF7HCjMwks0HURg6MTfdk1A"; ?>
<? } ?>
<?if ($local[2]=="simec-d"){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxTFm3qU4CVFuo3gZaqihEzC-0jfaRTY9Fe8UfzYeoYDxtThvI3nGbbZEw" type="text/javascript"></script>
	<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxTFm3qU4CVFuo3gZaqihEzC-0jfaRTY9Fe8UfzYeoYDxtThvI3nGbbZEw"; ?> 
<? } ?>
<?if ($local[2]=="simec-local"){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxRzjpIxsx3o6RYEdxEmCzeJMTc4zBSMifny_dJtMKLfrwCcYh5B01Pq_g" type="text/javascript"></script>
	<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxRzjpIxsx3o6RYEdxEmCzeJMTc4zBSMifny_dJtMKLfrwCcYh5B01Pq_g"; ?> 	
<? } ?>
<?if ($local[2]=="painel.mec.gov.br"){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxTPkFYZwQy2nvpGvFj08HQmPOt9ZBT2EJmQsTms0WQqU_5GvEj7bMZd7g" type="text/javascript"></script>
	<? $GKey = "ABQIAAAAwN0kvNsueYw8CBs704pusxTPkFYZwQy2nvpGvFj08HQmPOt9ZBT2EJmQsTms0WQqU_5GvEj7bMZd7g"; ?> 	
<? } ?>

<script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false"></script>
<script type="text/javascript" src="/includes/maps/markerclusterer.js"></script>

<script>

var marcadores = new Array();
var detalhesMarcadores = new Array();

function controleAcoes(ac) {
	for(i=0;i<document.getElementById("tabela").rows.length;i++) {
		if(document.getElementById("tabela").rows[i].id.substr(0,5) == "tr_m_") {
			document.getElementById("tabela").rows[i].cells[0].childNodes[0].title=ac;
			document.getElementById("tabela").rows[i].cells[0].childNodes[0].onclick();
		}
	}
}

function carregarAcao(acaid, indids, indpne, obj) {

	var tabela = obj.parentNode.parentNode.parentNode;
	var linha  = obj.parentNode.parentNode;
	if(obj.title == "mais") {
		obj.title="menos";
		obj.src="../imagens/menos.gif";
		var nlinha = tabela.insertRow(linha.rowIndex+1);
		nlinha.style.background = '#f5f5f5';
		var col0 = nlinha.insertCell(0);
		col0.colSpan=tabela.rows[0].cells.length;
		col0.id="colid_"+acaid;
		divCarregando(col0);
		ajaxatualizar('requisicao=listaIndicadores&detalhes=<? echo $_REQUEST['detalhes']; ?>&acaid='+acaid+'&indids='+indids+'&indpne='+indpne, 'colid_'+acaid);
		divCarregado(col0);
	} else {
		obj.title="mais";
		obj.src="../imagens/mais.gif"
		tabela.deleteRow(linha.rowIndex+1);
	}

}

function createMarker(posn, title, icon, html,muncod) {
	var marker = new GMarker(posn, {title: title, icon: icon, draggable:false });
	if(html != false){
		GEvent.addListener(marker, "click", function() {
		marker.openInfoWindowHtml(html);
		});
	}
	return marker;
}

function exibePontos(obj){
	
	if(obj.checked == true){
		if(markerGroups[obj.value]){
			for (var i = 0; i < markerGroups[obj.value].length; i++) {
		        var marker = markerGroups[obj.value][i];
		        marker.show();
			}
		}	
	}else{
		if(markerGroups[obj.value]){
			for (var i = 0; i < markerGroups[obj.value].length; i++) {
		        var marker = markerGroups[obj.value][i];
		        marker.hide();
			}
		}
	}		

}


var markerGroups = { '1': []};

function initialize(indid) {
	var myLatLng = new google.maps.LatLng(-14.689881, -52.373047);
   	var center = myLatLng;
   	var myOptions = {
     zoom: 4,
     center: myLatLng,
     mapTypeId: google.maps.MapTypeId.ROADMAP
   };
    document.getElementById('linha_mapa_'+indid).style.display='';
	document.getElementById('linha_mapa_'+indid+'_').style.display='';
	map = new google.maps.Map(document.getElementById('div_exibe_mapa_'+indid),myOptions);
}

function fecharGlobo(indid) {
	document.getElementById('linha_mapa_'+indid).style.display = 'none';
	document.getElementById('linha_mapa_'+indid+'_').style.display = 'none';
	document.getElementById('div_exibe_mapa_'+indid).innerHTML = '';
	document.getElementById('info_mapa_'+indid).innerHTML = '';
}

var markerClusterer = null;
var arrMarkers = new Array();

function exibeGrafico(indid, params) {

	initialize(indid);
	divCarregando(document.getElementById('linha_mapa_'+indid));	
	var icon = new google.maps.MarkerImage('/imagens/tachinha_y.png');
	
	xml_filtro = 'painel.php?modulo=principal/detalhamentoIndicador&acao=A&requisicao=exibeMapaDetalhamento&indid='+indid+'&filtro='+params;
	
	jQuery.ajax({
  		type: "POST",
  		url: xml_filtro,
  		async: false,
  		success: function(data) {  		  
      		jQuery(data).find("marker").each(function(){
				var marker = jQuery(this);	
				var mundsc = marker.attr("mundsc");
				var estuf = marker.attr("estuf");
				var muncod = marker.attr("muncod");
				var qtde = marker.attr("qtde");
				var valor = marker.attr("valor");
				var descricao = marker.attr("descricao");
				var endereco = marker.attr("endereco");
				var telefone = marker.attr("telefone");
				title = mundsc; 
				
				var lat = marker.attr("lat");
				var lng = marker.attr("lng");
				
				var html;
						
				html = "<div style=\"font-family:verdana;font-size:11px;padding:10px\" >";
				html += "<b>Localização:</b> " + mundsc + "/" + estuf + "<br /><br />";
				html += "<b>Quantidade:</b> " + qtde + "<br /><br />";
				if(valor != '') {
					html += "<b>Valor R$:</b> " + valor + "<br /><br />";
				}
				html += "</div>";
				
				if(!lat_ant){
					var lat_ant = "";
				}
				if(!lng_ant){
					var lng_ant = "";
				}

				// Verifica pontos em um mesmo lugar e move o seguinte para a direita
				if(lat_ant==marker.attr("lat") && lng_ant==marker.attr("lng")){
					var latlng = new google.maps.LatLng(parseFloat(marker.attr("lat")),
                                    		parseFloat(marker.attr("lng")));
				}else{
					var latlng = new google.maps.LatLng(parseFloat(marker.attr("lat")),
                                    		parseFloat(marker.attr("lng"))+0.0005);	
				}
				lat_ant=marker.attr("lat");
				lng_ant=marker.attr("lng");
				
				// Cria o marcador na tela
				 var marker = new google.maps.Marker({
			        position: latlng,
			        map: map,
			        icon: icon,
			        zIndex: 3
			    });
				
				markerGroups['1'].push(marker);
				
				marcadores[muncod]=marker;
				detalhesMarcadores[muncod]=html;
				
				arrMarkers.push(marker);
				
				google.maps.event.addListener(marker, "click", function() { abrebalao(muncod); });
				
  			});
	  	}
	});
	
	markerClusterer = new MarkerClusterer(map,arrMarkers);
		
	
	var myAjax = new Ajax.Request(
		'painel.php?modulo=principal/detalhamentoIndicador&acao=A',
		{
			method: 'post',
			parameters: 'requisicao=exibeInformacoesMapa&indid='+indid+'&filtro='+params,
			asynchronous: false,
			onComplete: function(resp) {
				extrairScript(resp.responseText);
				document.getElementById('info_mapa_'+indid).innerHTML = resp.responseText;
			},
			onLoading: function(){
				document.getElementById(iddestinatario).innerHTML = 'Carregando...';
			}
		});
	
	
	divCarregado(document.getElementById('linha_mapa_'+indid));


}

function abrebalao(marcador){
	infowindow = new google.maps.InfoWindow({content: detalhesMarcadores[marcador]}); infowindow.open(map, marcadores[marcador]);
}

//FUNÇÃO QUE EXPORTA TODA A SÉRIE HISTÓRICA DO INDICADOR EM EXCEL
function exportarIndicadorExcel(indid, params) {
    window.location='painel.php?modulo=principal/detalhamentoIndicador&acao=A&requisicao=exportarIndicadorExcel&indid='+indid+'&filtro='+params;
}

</script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<style>
.boxindicador {
	width:400px;
	height:100px;
	position: absolute;
	background-color: #FCFCFC;
	border: solid 1px #000000;
	padding: 3px;
	margin-left:30px;
	display:none;
}
</style>
<?php
if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}
?>
<script type="text/javascript">
this._closeWindows = false;
</script>
</head>
<input type="hidden" name="hdn_tipo_grafico" id="hdn_tipo_grafico" value="<?php echo TIPO_GRAFICO ?>" />
<body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
<?php

if($_REQUEST['indid']) {
	$sql = "SELECT * FROM painel.indicador WHERE indid='".$_REQUEST['indid']."'";
	$dadosi = $db->pegaLinha($sql);
    //VERIFICA A EXISTENCIA DE DADOS
    $sql = "SELECT count(0) FROM painel.seriehistorica WHERE indid='".$_REQUEST['indid']."'";
    $possuiRegistro = $db->pegaUm($sql);
    if($possuiRegistro==0){
        exit;
    }
	$estrutura = getEstruturaRegionalizacao($dadosi['regid']);
	$estloop = $estrutura;
}

//PARÂMETRO QUE VEM NO PLANEJAMENTO ESTRATÉGICO
if($_REQUEST['mpneid']){
	$sql = "SELECT indid FROM pde.ae_metapnexindicador WHERE mpneid = " . $_REQUEST['mpneid'] . " ORDER BY indid";
	$arrDados = $db->carregar( $sql );
	$contador = 1;
	foreach($arrDados as $dado){
		if($contador==1){
			$indpne = $dado['indid'];
		}else{
			$indpne = $indpne . ', ' . $dado['indid'];
		}
		$contador++;
	}
}

if($estloop && !$_GET['cockpit']) {
	
	$html .= "<table cellSpacing=0 cellPadding=3 style=\"width:100%;background-color:#FCFCFC;\">";
	
	do {
		$estor[] = $estloop['atu'];
		$estloop = $estloop['sub'];
	} while ($estloop['atu']);
	
	for($i=(count($estor)-1);$i >= 0;$i--) {
		$estruturanomeordenada[] = "<b>".$estor[$i]['regdescricao']."</b>";
		
		if($estor[$i]['rgavisao'] == $_REQUEST['detalhes']) {
			
			if($estor[$i]['regid'] != REGIONALIZACAO_BRASIL){
				if($estor[$i]['rgafiltroreg']) {
					$filtro1 .= str_replace(array("{".$estor[$i]['rgaidentificador']."}"),array($_REQUEST[$estor[$i]['rgaidentificador']]),$estor[$i]['rgafiltroreg']);
				} else {
					$filtro1 .= str_replace(array("{".$estor[$i]['rgaidentificador']."}"),array($_REQUEST[$estor[$i]['rgaidentificador']]),$estor[$i]['rgafiltro']);
				}
			}
			
			$sql = str_replace(array("{".$estor[$i]['rgaidentificador']."}", "{clausulaindicador}","{clausulaacao}", "{clausulasecretaria}", "{clausulametapne}","{ano}"),array($_REQUEST[$estor[$i]['rgaidentificador']], (($_REQUEST['indid'])?"AND ind.indid!='".$_REQUEST['indid']."'":""), (($_REQUEST['acaid'])?"AND aca.acaid in (".$_REQUEST['acaid'].")":""), (($_REQUEST['secid'])?"AND ind.secid='".$_REQUEST['secid']."'":""), (($_REQUEST['mpneid'])?"AND ind.indid IN (".$indpne.")":""), date("Y")), $estor[$i]['rgasqlindicadores']);
			$dadosreg = $db->pegaLinha(str_replace(array("{".$estor[$i]['rgaidentificador']."}"),array($_REQUEST[$estor[$i]['rgaidentificador']]),$estor[$i]['regsql']));
			$icones = str_replace(array("{ano}","{estuf}","{municipiocod}","{entnumcpfcnpj}","{estcod}","{anos}","{mundescricao}","{muncod}","{muncodcompleto}","{unicod}","{entid}","{estdescricao}","{entcodent}"),array(date("Y"),$dadosreg['estuf'],substr($dadosreg['muncod'],0,6),$dadosreg['entnumcpfcnpj'],$dadosreg['estcod'],(date("Y")-1),str_replace(Array(' ','\''),Array('_',''),$dadosreg['mundescricao']),$dadosreg['muncod'],$dadosreg['muncodcompleto'],$dadosreg['unicod'],$dadosreg['entid'],$dadosreg['estdescricao'], $dadosreg['entcodent']),stripslashes($estor[$i]['regicones']));
		}
	}
	
	$html .= "<tr>
				<td rowspan='4' align='center' width='140'><img src=\"../painel/images/".$dadosi['regid'].".gif\"></td>
				<td>".implode(" >> ", $estruturanomeordenada)."</td></tr>";
	$html .= "<tr><td style=\"font-size:12px;\"><b>".str_replace(array("{indid}"),array($_REQUEST['indid']),$dadosreg['descricao'])."</b></td></tr>";
	$html .= "<tr><td>".$icones."</td></tr>";
	$html .= "</table>";
	
} else {
	
	$qry = "SELECT * FROM painel.regagreg rga LEFT JOIN painel.regionalizacao reg ON reg.regid=rga.regid WHERE rgavisao='".$_REQUEST['detalhes']."'";
	$rga = $db->pegaLinha($qry);
	
	$sql = str_replace(array("{".$rga['rgaidentificador']."}", "{clausulaindicador}","{clausulaacao}", "{clausulasecretaria}", "{clausulametapne}", "{ano}"),array($_REQUEST[$rga['rgaidentificador']], (($_REQUEST['indid'])?"AND ind.indid!='".$_REQUEST['indid']."'":""), (($_REQUEST['acaid'])?"AND aca.acaid IN (".$_REQUEST['acaid'].")":""), (($_REQUEST['secid'])?"AND ind.secid='".$_REQUEST['secid']."'":""), (($_REQUEST['mpneid'])?"AND ind.indid IN (".$indpne.")":""), date("Y")),$rga['rgasqlindicadores']);

	$dadosreg = $db->pegaLinha(str_replace(array("{".$rga['rgaidentificador']."}"),array($_REQUEST[$rga['rgaidentificador']]),$rga['regsql']));
	
	if($rga['regid'] != REGIONALIZACAO_BRASIL){
		if($rga['rgafiltroreg']){
			$filtro1 .= str_replace(array("{".$rga['rgaidentificador']."}"),array($_REQUEST[$rga['rgaidentificador']]),$rga['rgafiltroreg']);
		} else {
			$filtro1 .= str_replace(array("{".$rga['rgaidentificador']."}"),array($_REQUEST[$rga['rgaidentificador']]),$rga['rgafiltro']);
		}
	}
	
	$estrutura = getEstruturaRegionalizacao($rga['regid']);

	$icones = str_replace(array("{ano}","{estuf}","{municipiocod}","{entnumcpfcnpj}","{estcod}","{anos}","{mundescricao}","{muncod}","{muncodcompleto}","{unicod}","{entid}","{estdescricao}", "{entcodent}"),array(date("Y"),$dadosreg['estuf'],substr($dadosreg['muncod'],0,6),$dadosreg['entnumcpfcnpj'],$dadosreg['estcod'],(date("Y")-1),str_replace(Array(' ','\''),Array('_',''),$dadosreg['mundescricao']),$dadosreg['muncod'],$dadosreg['muncodcompleto'],$dadosreg['unicod'],$dadosreg['entid'],$dadosreg['estdescricao'], $dadosreg['entcodent']),stripslashes($rga['regicones']));
	
	$estloop = $estrutura;
	
	do {
		$estor[] = $estloop['atu'];
		$estloop = $estloop['sub'];
	} while ($estloop['atu']);

	for($i=(count($estor)-1);$i >= 0;$i--) {
		$estruturanomeordenada[] = "<b>".$estor[$i]['regdescricao']."</b>";
	}
	
	if(!$_GET['cockpit']){
		$html .= "<table cellSpacing=0 cellPadding=3 style=\"width:100%;background-color:#FCFCFC;\">";
		$html .= "<tr>
					<td rowspan='3' align='center' width='140'><img src=\"../painel/images/".$rga['regid'].".gif\"></td>
					<td>".implode(" >> ", $estruturanomeordenada)."</td></tr>";
		$html .= "<tr><td style=\"font-size:12px;\"><b>".str_replace(array("{indid}"),array($_REQUEST['indid']),$dadosreg['descricao'])."</b></td></tr>";
		$html .= "<tr><td>".$icones."</td></tr>";
		$html .= "</table>";
	}
	
}

$inds = $db->carregar($sql);

// agrupando indices por eixo
if($inds[0]) {
	foreach($inds as $ind) {
		if($ind['indid'] != $_REQUEST['indid'])
			$arrIndAgrup[$ind['acaid']][] = array("indid" => $ind['indid']);
			$arrAcaInfo[$ind['acaid']] = $ind['acadsc'];
	}
}

// processando estrutura
$html .= "<table cellSpacing=0 cellPadding=3 class=listagem style=\"width:100%;color:#888888;\" id=\"tabela\">";
if(!$_GET['cockpit']){
	$html .= "<tr><td class=\"SubTituloEsquerda\" colspan=9><a style=\"cursor:pointer;\" onclick=\"controleAcoes('mais');\">Abrir todos</a> | <a style=\"cursor:pointer;\" onclick=\"controleAcoes('menos');\">Fechar todos</a></td></tr>";
}

if($_REQUEST['indid']) {
	
//Se o indicador for Percentual, e não for fórmula, não exibe o valor/quantidade
	$sql = "select unmid,indshformula,formulash from painel.indicador where indid = {$_REQUEST['indid']}";
	extract($db->pegaLinha($sql));
	
	if($unmid == UNIDADEMEDICAO_PERCENTUAL || $unmid == UNIDADEMEDICAO_RAZAO){
		if($indshformula != "t" || !$formulash){
			$qtde = "sum(qtde)";
			$coluna_dpe = "";
		}else{
			switch($_GET['detalhes']){
				case "zona":
						$where_reg = "foo.zonid";
						$campo = "zon.zonid,";
					break;
				case "subprefeitura":
						$where_reg = "foo.subid";
						$campo = "sub.subid,";
					break;
				case "setor":
						$where_reg = "foo.setid";
						$campo = "s.setid,";
					break;
				case "distrito":
						$where_reg = "foo.disid";
						$campo = "dis.disid,";
					break;
			}
			$qtde = "(".trataFormulaPorcentagem($formulash,"foo",$where_reg).")";
			$coluna_dpe = "dpeid,";
		}
	}else{
		$qtde = "sum(qtde)";
	}
	
	$sql = "SELECT indid, acaid, unmid, foo.regid, indcumulativo, indcumulativovalor, indnome, $qtde as qtde, indqtdevalor, CASE WHEN indqtdevalor = TRUE THEN to_char(sum(valor), '999g999g999g999d99') ELSE '-' END as valor, secdsc, umedesc, regdescricao from(
				SELECT $campo $coluna_dpe d.indid, acaid, d.unmid, d.indnome, d.secid, d.umeid, d.regid, d.indcumulativo, d.indcumulativovalor, d.indqtdevalor, CASE WHEN d.indcumulativo='S' THEN sum(d.qtde)
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
				WHERE d.indid=".$_REQUEST['indid']." and sehstatus <> 'I'
				".$filtro1." GROUP BY $campo $coluna_dpe d.indid, acaid, d.unmid,d.indnome,d.indcumulativo,d.indcumulativovalor,d.sehstatus,d.dpeanoref,d.secid,d.umeid,d.regid,d.indqtdevalor
				) foo 
				INNER JOIN painel.secretaria sec ON sec.secid=foo.secid 
				INNER JOIN painel.unidademeta ume ON ume.umeid=foo.umeid
				INNER JOIN painel.regionalizacao reg ON reg.regid=foo.regid 
				GROUP BY ".($where_reg ? $where_reg."," : "")." $coluna_dpe indid, acaid, foo.unmid, indnome, secdsc, umedesc, regdescricao, indcumulativovalor, indcumulativo, indqtdevalor, foo.regid 
				ORDER BY indid";
	//dbg($sql);
	$indicadorP = $db->pegaLinha($sql);
	
	$indicadorP['indshformula'] = $indshformula;
	$indicadorP['formulash'] = $formulash;
	
	$html .= "<tr>";
	$html .= "<td style=\"width:100%;text-align:center;font-weight:bold;background-color:#DBDBDB;font-size:14px;color: rgb(0, 85, 0);\" colspan=9 >";
	$html .= $indicadorP['acadsc'];
	$html .= "</td></tr>";
	
	$html .= "<tr>";
	$html .= "<td>";
	
	
	$html .= "<table cellSpacing=0 cellPadding=3 class=listagem style=\"width:100%;color:#888888;\">";
	
	$html .= "<tr>";
	$html .= "<td class=\"SubTituloCentro\">&nbsp;</td>";
	$html .= "<td class=\"SubTituloCentro\">Cod</td>";
	$html .= "<td class=\"SubTituloCentro\">Nome do indicador</td>";
	$html .= "<td class=\"SubTituloCentro\">Secretaria</td>";
	$html .= "<td class=\"SubTituloCentro\">Regionalização</td>";
	$html .= "<td class=\"SubTituloCentro\">Produto</td>";
	$html .= "<td class=\"SubTituloCentro\">Qtde</td>";
	$html .= "<td class=\"SubTituloCentro\">R$</td>";
	$html .= "</tr>";

	if($_REQUEST['detalhes'])
		$rgaidentificador = $db->pegaUm("select rgaidentificador from painel.regagreg where rgavisao = '".$_REQUEST['detalhes']."'");
	
	$html .= processarLinhaDetalhamentoIndicadores($indicadorP, array('detalhes' => $_REQUEST['detalhes'], "rgaidentificador" => $rgaidentificador), true);
	
	$html .= "</table>";
	
	$html .= "</td>";
	$html .= "</tr>";
	if(!$_GET['cockpit']){
		$html .= "<tr>";
		$html .= "<td class=\"SubTituloEsquerda\">Acesse outros indicadores...</td>";
		$html .= "</tr>";
	}
	

}

if($arrAcaInfo && !$_GET['cockpit']) {
	foreach($arrAcaInfo as $acaid => $acadsc) {
		$html .= "<tr id=\"tr_m_".$acaid."\">";
		$html .= "<td style=\"width:100%;text-align:left;font-weight:bold;background-color:#F3F3F3;font-size:14px;color: rgb(0, 85, 0);\" colspan=8 >";
		$html .= "<img src=\"../imagens/mais.gif\" style=\"cursor:pointer;\" title=\"mais\" id=\"imgc_".$acaid."\" onclick=\"carregarAcao('".$acaid."', '".md5_encrypt(serialize($arrIndAgrup[$acaid]))."', '".$indpne."', this);\"> ";
		$html .= "<span style=\"cursor:pointer;\" onclick=\"document.getElementById('imgc_".$acaid."').onclick();\">".$acadsc."</span>";
		$html .= "</td></tr>";
	}
} else {
	if(!$_GET['cockpit']){
		$html .= "<tr>";
		$html .= "<td align=\"center\" colspan=\"9\">Não existem indicadores.</td>";
		$html .= "</tr>";
	}
}

$html .= "</table>";

if(count($indagrup) === 1) {
	$html .= "<script>controleAcoes('mais');</script>";
}

echo $html;

?>
<script>
	self.focus();
	<?php if($_GET['abreMapa']): ?>
		exibeGrafico('<?php echo $_GET['indid'] ?>', '');
	<?php endif; ?>
</script>
</body>
</html>