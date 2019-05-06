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
	$txtprogramasComCoordenador = "";
	$confirmarprogramas = 0;
	$concluido = 0; // -1 erro, 0 nao concluido, 1 sucesso
	$programasConfirmados = $_REQUEST["programasConfirmados"];
	//SQL para verificar as responsabilidades
	$sqlSelResp = "SELECT 
					distinct ur.rpuid, 
					ur.usucpf, 
					ur.rpustatus, 
					p.prgdsc, 
					p.prgcod, 
					p.prgid 
				   FROM 
				    monitora.usuarioresponsabilidade ur 
					INNER JOIN programa p ON p.prgid = ur.prgid AND p.prgid = %s
					inner join seguranca.perfil pfl on pfl.pflcod=ur.pflcod and pfl.pflsncumulativo='f' and pfl.pflcod='".$pflcod."' 			
				   WHERE 
				    ur.rpustatus = 'A' AND 
				    ur.prsano='".$_SESSION['exercicio']."' AND
				    ur.usucpf <> '" . $usucpf . "'";
	

	$sqlSelPrograma	= "SELECT p.prgid, p.prgcod FROM programa p WHERE p.prgid = %s AND prgano='".$_SESSION['exercicio']."'";
	
	$sqlInsRpu = "INSERT INTO monitora.usuarioresponsabilidade (prgid, usucpf, rpustatus, rpudata_inc, pflcod, prsano) VALUES ('%s', '%s', '%s', '%s', '%s','".$_SESSION['exercicio']."')";

	$sqlUpdRpu = "UPDATE monitora.usuarioresponsabilidade SET rpustatus = 'I' WHERE prgid = '%s' AND prsano='".$_SESSION['exercicio']."' AND pflcod = ".$pflcod;

	$sqlUpdRpuUsu = "UPDATE monitora.usuarioresponsabilidade SET rpustatus = 'I' WHERE usucpf = '".$usucpf."'  AND prgid IS NOT NULL AND prsano='".$_SESSION['exercicio']."' AND pflcod = ".$pflcod;

	//
	// verificar quais programas possuem outro com o mesm o perfil
	foreach ($_POST['usuprogresp'] as $respcod) {
		if (!$respcod){
			continue;
		}
		$sql='';
		$sql = vsprintf($sqlSelResp, $respcod);
		if ($sql<>"" && ($linhasRpu = @$db->carregar($sql))) {
			foreach ($linhasRpu as $rpu) {
				$confirmarprogramas = true;
				$txtprogramasComCoordenador .= $rpu["prgcod"] . ' - ' . $rpu["prgdsc"] . ' CPF: ' . $rpu["usucpf"] . '\n ';
			}
		}
	}

	// caso nao existam outros coordenadores de planejamento, registrar os itens selecionados
	if(!$confirmarprogramas) {
		$sql = $sqlUpdRpuUsu;
		$db->executar($sql);
		foreach ($_POST['usuprogresp'] as $respcod) {
		   $sql = "";
		   if ($respcod>0){
		   $sql = vsprintf($sqlSelPrograma, $respcod);
           $linha = $db->carregar($sql);
			if(is_array($linha) && count($linha)>=1) {
				foreach ($linha as $programa) {
					$prgid = $programa["prgid"]; 		
					$dados = array($prgid, $usucpf, 'A', date("Y-m-d H:i:s"), $pflcod); 						$sql = vsprintf($sqlInsRpu, $dados);
					$db->executar($sql);
				}
				
			}}
		}
		$concluido = 1;
	}
	//
	// verificar se foi confirmado a substituição do coordenador atual pelo
	// usuario que está sendo liberado e/ou alterado
	else
	if($programasConfirmados) {
		if (is_array($_REQUEST['usuprogresp'])){
		
		foreach ($_REQUEST['usuprogresp'] as $rpu) {
			$sql = sprintf($sqlUpdRpu, $rpu);
			$db->executar($sql);
			$dados = array($rpu, $usucpf, 'A', date("Y-m-d H:i:s"), $pflcod);
			$sql = vsprintf($sqlInsRpu, $dados);
			$db->executar($sql);
		}
		$concluido = 1;	
	}	
	}
	//
	// exibir a tela de aviso dos itens que já possuem coordenador e confirmar
	// a substituição pelo usuario que está sendo liberado e/ou alterado
	else {
		$msg = 'Existem usuários ativos com o perfil selecionado para este Programa:\n\n';
		$msg .= $txtprogramasComCoordenador;
		$msg .= '\nDeseja sobrescrevê-los?\n\n';
		$msg .= 'Ao confirmar, o perfil dos usuários atuais (listados acima) será desativado.';
		?>
		<html>
		<body>
		<form name="formassocia" style="margin:0px;" method="POST">
		<input type="hidden" name="usucpf" value="<?=$usucpf?>">
		<input type="hidden" name="pflcod" value="<?=$pflcod?>">
		<input type="hidden" name="programasConfirmados" value="1">
		<?
			foreach ($_POST['usuprogresp'] as $respcod) {
				?><input type="hidden" name="usuprogresp[]" value="<?=$respcod?>"><?
			}
		?>
		</form>
		</body>
		</html>
		
		<script>
			if (confirm("<?=$msg?>")) {		
				document.formassocia.submit();
			} else
			{
				self.close();		
			}
		</script>

		<?
		exit(0);
	}

	if ($concluido>0) {
		$db->commit();
		?>
	<script>
	window.parent.opener.location.reload();self.close();
	</script>
		<?
		exit(0);
	}
}
/*
*** FIM REGISTRO RESPONSABILIDADES ***
*/
?>
<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>Programas</title>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='/includes/listagem.css'>

</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">
<div align=center id="aguarde"><img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle"> <font color=blue size="2">Aguarde! Carregando Dados...</font></div>
<?flush();?>
<DIV style="OVERFLOW:AUTO; WIDTH:496px; HEIGHT:350px; BORDER:2px SOLID #ECECEC; background-color: White;">
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
<script language="JavaScript">
document.getElementById('tabela').style.visibility = "hidden";
document.getElementById('tabela').style.display  = "none";
</script>
<form name="formulario">
<thead><tr>
<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione o(s) Programas(s)</strong></td>
</tr>
<tr>
<?

	  $cabecalho = 'Selecione o(s) Programa(s)';
	  $sql = "select prgid, prgcod, prgcod, prgdsc from planejamento.programa where prgstatus='A' and prgano='".$_SESSION['exercicio']."' order by prgcod";

	  $RS = @$db->carregar($sql);
	  $nlinhas = count($RS)-1;
	  for ($i=0; $i<=$nlinhas;$i++)
		 {
			foreach($RS[$i] as $k=>$v) ${$k}=$v;
			if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
	   ?>
	   		
		   		<tr bgcolor="<?=$cor?>">
				<td align="right"><input type="Checkbox" name="prgid" id="<?=$prgid?>" value="<?=$prgid?>" onclick="retorna(<?=$i?>);"><input type="Hidden" name="prgdsc" value="<?=$prgcod.' - '.$prgdsc?>"></td>
				<td align="right" style="color:blue;"><?=$prgcod?></td>
				<td><?=$prgdsc?></td>
				</tr>
	   
	   <?}
?>
</form>
</table>
</div>
<form name="formassocia" style="margin:0px;" method="POST">
<input type="hidden" name="usucpf" value="<?=$usucpf?>">
<input type="hidden" name="pflcod" value="<?=$pflcod?>">
<select multiple size="8" name="usuprogresp[]" id="usuprogresp" style="width:500px;" class="CampoEstilo" onchange="moveto(this);">
<?
$sql = "select 
		 distinct p.prgid as codigo, 
		 p.prgcod||' - '||p.prgdsc as descricao 
		from 
		 monitora.usuarioresponsabilidade ur 
		 inner join planejamento.programa p on ur.prgid=p.prgid 
		where 
		 ur.rpustatus='A' and 
		 ur.usucpf = '$usucpf' and 
		 ur.pflcod=$pflcod and 
		 ur.prsano='".$_SESSION['exercicio']."' 
		order by 2";

$RS = @$db->carregar($sql);

//dbg($RS,1);

if(is_array($RS)) {
	$nlinhas = count($RS)-1;
	if ($nlinhas>=0) {
		for ($i=0; $i<=$nlinhas;$i++) {
			foreach($RS[$i] as $k=>$v) ${$k}=$v;
    		print " <option value=\"$codigo\">$descricao</option>";		
		}
	}
} else {
//	$sql = "select 
//	         distinct p.prgid as codigo, 
//	         p.prgdsc as descricao 
//	        from 
//	         planejamento.programa p
//	         inner join monitora.progacaoproposto pp on p.prgid = pp.prgid and 
//	         									        pp.usucpf = '".$usucpf."' 
//			where 
//			 pp.prgid is not null";
//	$RS = @$db->carregar($sql);
//	if(is_array($RS)) {
//		$nlinhas = count($RS)-1;
//		if ($nlinhas>=0) {
//			for ($i=0; $i<=$nlinhas;$i++) {
//				foreach($RS[$i] as $k=>$v) ${$k}=$v;
//				print " <option value=\"$codigo\">$codigo - $descricao</option>";
//			}
//		}
//	} else {
//	<option value="">Selecione o(s) Programa(s).</option>
//	
//	}
?>
	<option value="">Selecione o(s) Programa(s).</option>
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
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Selecione o(s) Programa(s).', '', false, false);}
			sortSelect(campoSelect);
	}
}

function moveto(obj) {
	if (obj.options[0].value != '') {
		document.getElementById(obj.value).focus();}
}




</script>








