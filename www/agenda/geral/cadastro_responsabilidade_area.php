<?php

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$db = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];

/*
*** INICIO REGISTRO RESPONSABILIDADES ***
*/ 	

if(is_array($_POST['usuprogresp']) && @count($_POST['usuprogresp'])>0) {
	
	$sqlInsRpu 	  = "INSERT INTO agenda.usuarioresponsabilidade 
						(aevid, usucpf, pflcod) 
					 VALUES 
					 	('%s', '%s', '%s')";

	$sqlUpdRpuUsu = "UPDATE agenda.usuarioresponsabilidade SET 
						rpustatus = 'I' 
					 WHERE 
					 	usucpf = '".$usucpf."' AND 
					 	aevid IS NOT NULL AND 
					 	pflcod = ".$pflcod;

	$sql = $sqlUpdRpuUsu;
	$db->executar($sql);
	foreach ($_POST['usuprogresp'] as $respcod) {
		$sql = "";
		if ($respcod>0){
			$aevid = $respcod; 		
			$dados = array($aevid, $usucpf, $pflcod); 						
			$sql   = vsprintf($sqlInsRpu, $dados);
			$db->executar($sql);
		}
	}
	
	$db->commit();
	?>
	<script>
	alert('Opera��o realizada com sucesso!');
	window.parent.opener.location.reload();self.close();
	</script>
	<?
	exit(0);
}
/*
*** FIM REGISTRO RESPONSABILIDADES ***
*/
?>
<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>�rea</title>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='/includes/listagem.css'>

</head>
<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
<div align=center id="aguarde">
	<img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle"> 
	<font color=blue size="2">Aguarde! Carregando Dados...</font>
</div>
<?flush();?>
<DIV style="OVERFLOW:AUTO; WIDTH:496px; HEIGHT:350px; BORDER:2px SOLID #ECECEC; background-color: White;">
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
<script language="JavaScript">
document.getElementById('tabela').style.visibility = "hidden";
document.getElementById('tabela').style.display  = "none";
</script>
<form name="formulario">
<thead>
	<tr>
		<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione a(s) �rea(s)</strong></td>
	</tr>
   	<tr bgcolor="<?=$cor?>">
		<th align="center">A��o</th>
		<th align="center">�rea</th>
	</tr>
</thead>
<?

	  $cabecalho = 'Selecione a(s) �rea(s)';
	  $sql = "SELECT 
	  			aevid, 
	  			aevsigla || ' - ' || aevdsc AS aevdsc 
	  		  FROM 
	  		  	agenda.areaenvolvida 
	  		  WHERE 
	  		  	aevstatus='A' 
	  		  ORDER BY aevdsc";

	  $RS 		= @$db->carregar($sql);
	  $nlinhas  = count($RS)-1;
	  
	  for ($i=0; $i<=$nlinhas;$i++){
			foreach($RS[$i] as $k=>$v) ${$k}=$v;
			if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
	   ?>
	   		
		   		<tr bgcolor="<?=$cor?>">
					<td align="center" width="10">
						<input type="Checkbox" name="prgid" id="<?=$aevid?>" value="<?=$aevid?>" onclick="retorna(<?=$i?>);">
						<input type="Hidden" name="prgdsc" value="<?=$aevdsc?>">
					</td>
					<td align="justify" ><?=$aevdsc?></td>
				</tr>
	   
	   <?}
?>
</table>
</form>
</div>
<form name="formassocia" style="margin:0px;" method="POST">
<input type="hidden" name="usucpf" value="<?=$usucpf?>">
<input type="hidden" name="pflcod" value="<?=$pflcod?>">
<select multiple size="8" name="usuprogresp[]" id="usuprogresp" style="width:500px;" class="CampoEstilo" onchange="moveto(this);">
<?
$sql = "SELECT DISTINCT 
		 a.aevid as codigo, 
		 a.aevsigla || ' - ' ||a.aevdsc AS descricao 
		FROM 
		 agenda.usuarioresponsabilidade ur 
		 INNER JOIN agenda.areaenvolvida a ON ur.aevid=a.aevid 
		WHERE 
		 ur.rpustatus='A' AND 
		 ur.usucpf='$usucpf' AND 
		 ur.pflcod=$pflcod 
		ORDER BY 2";

$RS = @$db->carregar($sql);

if(is_array($RS)) {
	$nlinhas = count($RS)-1;
	if ($nlinhas>=0) {
		for ($i=0; $i<=$nlinhas;$i++) {
			foreach($RS[$i] as $k=>$v) ${$k}=$v;
    		print " <option value=\"$codigo\">$descricao</option>";		
		}
	}
} else {
?>
	<option value="">Selecione a(s) �rea(s).</option>
<?
}
?>
</select>
</form>
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
<tr bgcolor="#c0c0c0">
<td align="right" style="padding:3px;" colspan="3">
<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);document.formassocia.submit();" id="ok">
</td></tr>
</table>
<script language="JavaScript">
document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";
document.getElementById('tabela').style.visibility = "visible";
document.getElementById('tabela').style.display  = "";


var campoSelect = document.getElementById("usuprogresp");


if (campoSelect.options[0].value != ''){
	for(var i=0; i<campoSelect.options.length; i++)
		{document.getElementById(campoSelect.options[i].value).checked = true;}
}



function retorna(objeto)
{

	tamanho = campoSelect.options.length;
	if (campoSelect.options[0].value=='') {tamanho--;}
	if (document.formulario.prgid[objeto].checked == true){
		campoSelect.options[tamanho] = new Option(document.formulario.prgdsc[objeto].value, document.formulario.prgid[objeto].value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (document.formulario.prgid[objeto].value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Selecione a(s) �rea(s).', '', false, false);}
			sortSelect(campoSelect);
	}
}

function moveto(obj) {
	if (obj.options[0].value != '') {
		document.getElementById(obj.value).focus();}
}




</script>








