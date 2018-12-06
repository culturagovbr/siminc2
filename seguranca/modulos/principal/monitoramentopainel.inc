<?php

include_once "./_funcoesmonitoramento.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

/* montando cabeçalho */
monta_titulo( "Monitoramento dos módulos do ". SIGLA_SISTEMA, "Painel de monitoramento de funcionamento dos módulos do ". SIGLA_SISTEMA);

?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/seguranca.js"></script>
<script language="javascript" type="text/javascript" src="../includes/open_flash_chart/swfobject.js"></script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class='SubTituloDireita'>Ano :</td>
	<td>
	<?
	$ano=date("Y");
	$sql = "SELECT ano as codigo, ano as descricao FROM public.anos";
	$db->monta_combo('ano', $sql, 'S', 'Selecione', 'atualizarGridMonitoramento', '', '', '', 'S', 'ano');
	?>
	</td>

	<td class='SubTituloDireita'>Mês :</td>
	<td>
	<?
	$mes=date("m");
	$sql = "SELECT mescod as codigo, mesdsc as descricao FROM public.meses";
	$db->monta_combo('mes', $sql, 'S', 'Selecione', 'atualizarGridMonitoramento', '', '', '', 'S', 'mes');
	?>
	</td>
	
	<td class='SubTituloDireita'>Período :</td>
	<td>
	<? 
	$diaini = "01";
	echo campo_texto('diaini', "N", "S", "Dia inicío", 2, 2, "", "", '', '', 0, 'id="diaini"' ); ?> / <span id="permesini"><? echo $mes; ?></span> / <span id="peranoini"><? echo $ano; ?></span> 
	
	&nbsp;&nbsp;&nbsp;&nbsp;até&nbsp;&nbsp;&nbsp;&nbsp; 
	
	<? 
//	$diafim = cal_days_in_month(CAL_GREGORIAN, $mes, $ano);
	echo campo_texto('diafim', "N", "S", "Dia fim", 2, 2, "", "", '', '', 0, 'id="diafim"' ); ?> / <span id="permesfim"><? echo $mes; ?></span> / <span id="peranofim"><? echo $ano; ?></span>
	
	&nbsp;&nbsp;&nbsp;&nbsp;
	<input type="button" value="Filtrar" onclick="atualizarGridMonitoramentoPeriodo();">

	</td>


</tr>
</table>

<div id="gridMonitoramento"></div>

<?
$sql = "SELECT * FROM seguranca.tipomonitoramento";
$tp = $db->carregar($sql);

if($tp[0]) {
	
	//$i=0;
	
	foreach($tp as $key => $t) {
		$_TD[] .= "<td align=center><img src=\"chart-data3.php?tmoid=".$t['tmoid']."\"></td>";
		//if($key%2 == 1) $i++;
	}
}
?>

<table class="tabela" cellSpacing="1" cellPadding="3"	align="center">
<?
if($_TD) {
	foreach($_TD as $t) {
		echo "<tr>".$t."</tr>";
		
	}
}
?>
</table>
<script>
document.observe("dom:loaded", function() {
	ajaxatualizar('requisicao=monitoramentoGRID&mes='+document.getElementById('mes').value+'&ano='+document.getElementById('ano').value,'gridMonitoramento');
});
</script>