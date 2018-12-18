<?php
if( $_REQUEST['requisicao'] == 'salvar' ){
	
	foreach ($_POST['mnuid'] as $mnuid) {
		$sql = "UPDATE seguranca.menu SET mnupaginainicial = 'S' WHERE mnuid = $mnuid";
		$db->executar($sql);
		echo $sql.';<br>';
		
		$sql = "SELECT COALESCE(usmqtdacesso, 0) FROM seguranca.usuariomenu where mnuid = {$mnuid} and usucpf = '{$_SESSION['usucpforigem']}'";
	  	$usmqtdacesso = $db->pegaUm($sql);
	  	
	  	if( $usmqtdacesso == 0 ){
			$sql = "INSERT INTO seguranca.usuariomenu(mnuid, usucpf, usmqtdacesso, sisid) 
					VALUES ({$mnuid}, '{$_SESSION['usucpf']}', 1, {$_POST['sisid']})";
		  	$db->executar( $sql );
		  	echo $sql.';<br>';
	  		$db->commit();
	  	}
	}
	
	if( $db->commit() ){
		$db->sucesso('principal/grupoPaginaInicial');
	}
}

include APPRAIZ."includes/cabecalho.inc";
echo '<br>';
?>
<form method="post" name="formulario" id="formulario" action="">
<input type="hidden" name="requisicao" id="requisicao" value="">
<table class="tabela" align="center" cellSpacing="1" cellPadding=3 >
	<tr>
		<td class="subtitulodireita">Sistema</td>
		<td><?
		$sisid = $_REQUEST['sisid'];
		$sql = "select distinct sisid as CODIGO,sisdsc as DESCRICAO from seguranca.sistema where sisstatus = 'A' order by DESCRICAO ";
	  	//Monta a combo
	  	$db->monta_combo("sisid",$sql,'S',"Selecione o Sistema",'submete_sistema',''); ?></td>
	</tr>
</table>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script type="text/javascript">
function submete_sistema(cod){
	document.getElementById('formulario').submit();
}

function salvarVinculoMenu(){
	if( $('[type="checkbox"]:checked').length ){
		$('#requisicao').val('salvar');
		$('#formulario').submit();
	} else {
		alert('Selecione pelo menos um menu!');
		return false;
	}
}
</script>
<?
if( $sisid ){
?>
<table class="tabela" align="center" cellSpacing="1" cellPadding=3 >
	<tr>
		<td class="subtitulocentro" colspan="2"><input type="button" name="btnSalvar" id="btnSalvar" value="Salvar" onclick="salvarVinculoMenu();"></td>
	</tr>
</table>
<?
	$sql= "select
				case when m.mnupaginainicial = 'N' then '<input type=\"Checkbox\" name=\"mnuid[]\" value=\"'||m.mnuid||'\">' 
					else 
				'<input type=\"Checkbox\" name=\"mnuid[]\" value=\"'||m.mnuid||'\" checked>' end as acao, 
				m.mnucod, 
				case when mnutipo=2 then '&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc when  mnutipo=3 then 
					'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc 
					when  mnutipo=4 then '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc 
				else mnudsc end as mnudsc, 
				case when mnushow=false then '<font color=#808080>Não</font>' 
					else '<font color=#008000>Sim</font>' end as mnushow, 
				mnulink, 
				mnutransacao 
			from seguranca.menu m 
			where  mnustatus = 'A' 
				and m.sisid=".$sisid."
				and m.mnushow = true
				and m.mnulink <> ''
			order by mnucod";
	//ver(simec_htmlentities($sql),d);
	$cabecalho = array('Sim/Não','Código','Menu / Módulo','Visível','Link','Transação');
	$db->monta_lista_simples($sql,$cabecalho,300,20,'','','');
?>
<table class="tabela" align="center" cellSpacing="1" cellPadding=3 >
	<tr>
		<td class="subtitulocentro" colspan="2"><input type="button" name="btnSalvar" id="btnSalvar" value="Salvar" onclick="salvarVinculoMenu();"></td>
	</tr>
</table>
<?} ?>
</form>