<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   M�dulo:seleciona_unid_prefilresp.php
   
   */
include "includes/classes_simec.inc";
include "includes/funcoes.inc";
$db = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];

/*
*** INICIO REGISTRO RESPONSABILIDADES ***
*/ 	
if(is_array($_POST['usuacaresp']) && @count($_POST['usuacaresp'])>0) {
	$txtAcoesComCoordenador = "";
	$confirmarAcoes = false;
	$concluido = 0; // -1 erro, 0 nao concluido, 1 sucesso
	$acoesConfirmadas = (bool)$_REQUEST["acoesConfirmadas"];
	
	$sqlSelResp = "SELECT ur.rpuid, ur.usucpf, ur.rpustatus, a.acadsc, a.acaid FROM usuarioresponsabilidade ur 
		INNER JOIN acao a ON a.acaid = ur.acaid
			AND a.prgcod = '%s' AND a.acacod = '%s' AND a.unicod = '%s'
		WHERE ur.rpustatus = 'A' AND ur.usucpf <> '" . $usucpf . "'";
	$sqlSelAcao	= "SELECT a.acaid FROM acao a WHERE a.prgcod = '%s' AND a.acacod = '%s' AND a.unicod = '%s'";
	$sqlInsRpu = "INSERT INTO usuarioresponsabilidade (acaid, usucpf, rpustatus, rpudata_inc, pflcod) VALUES ('%s', '%s', '%s', '%s', '%s')";
	$sqlUpdRpu = "UPDATE usuarioresponsabilidade SET rpustatus = 'I' WHERE rpuid = '%s'";

	//
	// verificar quais itens possuem outro coordenador ativo
	foreach ($_POST['usuacaresp'] as $respcod) {
		$sql = "";
		$arrCodigoAcao = explode(".", $respcod);
		if(@count($arrCodigoAcao)==3)
			$sql = vsprintf($sqlSelResp, $arrCodigoAcao);
		if ($sql<>"" && ($linhasRpu = $db->carregar($sql))) {
			foreach ($linhasRpu as $rpu) {
				$confirmarAcoes = true;
				$txtAcoesComCoordenador .= $respcod . " - " . $rpu["acadsc"] . " CPF: " . $rpu["usucpf"] . "\n";
			}
		}
	}

	//
	// caso nao existam outros coordenadores, registrar os itens selecionados
	if(!$confirmarAcoes) {
		foreach ($_POST['usuacaresp'] as $respcod) {
			$sql = "";
			$arrCodigoAcao = explode(".", $respcod);
			$sql = vsprintf($sqlSelAcao, $arrCodigoAcao);
			$linha = $db->carregar($sql);

			if(is_array($linha) && count($linha)>1) {
				foreach ($linha as $acao) {
					$acaid = $acao["acaid"]; 				
					$dados = array($acaid, $usucpf, 'A', date("Y-m-d H:i:s"), $pflcod); 				
					$sql = vsprintf($sqlInsRpu, $dados);
					
					$db->executar($sql);
				}
				$concluido = 1;
			}
		}
	}
	//
	// verificar se foi confirmado a substitui��o do coordenador atual pelo
	// usuario que est� sendo liberado e/ou alterado
	elseif($acoesConfirmadas) {
		foreach ($linhasRpu as $rpu) {
			$sql = sprintf($sqlUpdRpu, $rpu["rpuid"]);
			$db->executar($sql);

			$dados = array($rpu["acaid"], $usucpf, 'A', date("Y-m-d H:i:s"), $pflcod);
			$sql = vsprintf($sqlInsRpu, $dados);
			$db->executar($sql);
		}
		$concluido = 1;		
	}
	//
	// exibir a tela de aviso dos itens que j� possuem coordenador e confirmar
	// a substitui��o pelo usuario que est� sendo liberado e/ou alterado
	else {
		$msg = "Existem usu�rios ativos com o perfil selecionado para estas a��es:\n\n";
		$msg .= $txtAcoesComCoordenador;
		$msg .= "\nDeseja sobrescrev�-los?\n\n";
		$msg .= "Ao confirmar, o perfil dos usu�rios atuais (listados acima) ser� desativado.";
		?><form name="formassocia" style="margin:0px;" method="POST">
		<input type="hidden" name="usucpf" value="<?=$usucpf?>">
		<input type="hidden" name="pflcod" value="<?=$pflcod?>">
		<input type="hidden" name="acoesConfirmadas" value="1">
		<?
			foreach ($_POST['usuacaresp'] as $respcod) {
				?><input type="hidden" name="usuacaresp[]" value="<?=$respcod?>"><?
			}
		?>
		<script>
			if (confirm("<?=$msg?>")) {
				document.formassocia.submit();
			}
		</script>
		<?
		exit(0);
	}
	
	if ($concluido>0) {
		$db->commit();
		?>
		<script language="javascript">
			alert("Opera��o realizada com sucesso!");
			window.opener.reload();
			self.close();
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
<title>Unidades</title>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>

</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">
<div align=center id="aguarde"><img src="../imagens/icon-aguarde.gif" border="0" align="absmiddle"> <font color=blue size="2">Aguarde! Carregando Dados...</font></div>
<?flush();?>
<DIV style="OVERFLOW:AUTO; WIDTH:496px; HEIGHT:350px; BORDER:2px SOLID #ECECEC; background-color: White;">
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
<script language="JavaScript">
document.getElementById('tabela').style.visibility = "hidden";
document.getElementById('tabela').style.display  = "none";
</script>
<form name="formulario">
<thead><tr>
<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Clique no Programa para selecionar as A��es</strong></td>
</tr>
<tr>
<?
	  $sql = "select a.prgcod, p.prgdsc, a.acacod, a.unicod, a.acadsc, u.unidsc, p.prgid, count(*) as totalaca from acao a inner join programa p on a.prgid = p.prgid inner join unidade u on a.unicod=u.unicod where acasnrap='f' group by a.prgcod, p.prgdsc, a.acacod, a.unicod, a.acadsc, u.unidsc, p.prgid order by a.prgcod, a.acacod, a.unicod, a.acadsc";
	  $RS = $db->carregar($sql);
	  $nlinhas = count($RS)-1;
	  for ($i=0; $i<=$nlinhas;$i++)
		 {
			foreach($RS[$i] as $k=>$v) ${$k}=$v;
			if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
			if ($v_prgcod<>$prgcod) {
				if ($corp == '#e0e0e0') $corp = '#f4f4f4' ; else $corp='#e0e0e0';
				if ($v_prgcod) {?>
			 </table>
	  			 </td></tr>
			   <script language="JavaScript">
				   document.getElementById('<?=$v_prgcod?>').style.visibility = "hidden";
				   document.getElementById('<?=$v_prgcod?>').style.display  = "none";
			   </script>
				<?}?>
	   		<tr bgcolor="<?=$corp?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$corp?>';">
				<td align="left" onclick="abreconteudo('<?=$prgcod?>');document.getElementById('ok').focus();"><img src="../imagens/mais.gif" border="0" width="9" height="9" align="absmiddle" vspace="3" id="img<?=$prgcod?>" name="+">&nbsp;&nbsp;<font color="#0000ff"><?=$prgcod?></font> - <?=$prgdsc?></td>
			</tr>
			<tr id="<?=$prgcod?>"><td>
			   <table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" >
	   <?$v_prgcod=$prgcod;}?>
<tr bgcolor="<?=$cor?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$cor?>';"><td align="left" nowrap style="color:#006666;"> <input type="Checkbox" name="prgid" id="<?=$prgcod.'.'.$acacod.'.'.$unicod?>" value="<?=$prgcod.'.'.$acacod.'.'.$unicod?>" onclick="retorna(<?=$i?>);"><input type="Hidden" name="prgdsc" value="<?=$prgcod.'.'.$acacod.'.'.$unicod?> - <?=$acadsc?>"><?=$acacod.'.'.$unicod?></td><td style="color:#666666;"><font color="#333333"><?=$acadsc?></font> (<?=$unidsc?>)</td><td align="right" style="color:#666666;">(<?=$totalaca?>)</td></tr><?}?>
<script language="JavaScript">
				   document.getElementById('<?=$v_prgcod?>').style.visibility = "hidden";
				   document.getElementById('<?=$v_prgcod?>').style.display  = "none";
</script>
 </table>
</td></tr>
</form>
</table>
</div>
<form name="formassocia" style="margin:0px;" method="POST">
<input type="hidden" name="usucpf" value="<?=$usucpf?>">
<input type="hidden" name="pflcod" value="<?=$pflcod?>">
<select multiple size="8" name="usuacaresp[]" id="usuacaresp" style="width:500px;" class="CampoEstilo" onchange="moveto(this);">
<?
$sql = "select distinct a.prgcod||'.'||a.acacod||'.'||a.unicod as codigo, a.acadsc as descricao from usuarioresponsabilidade u inner join acao a on u.acaid=a.acaid where rpustatus='A' and usucpf = '$usucpf' and u.pflcod=$pflcod";
$RS = $db->carregar($sql);
if(is_array($RS)) {
	$nlinhas = count($RS)-1;
	if ($nlinhas>=0) {
		for ($i=0; $i<=$nlinhas;$i++) {
			foreach($RS[$i] as $k=>$v) ${$k}=$v;
    		print " <option value=\"$codigo\">$codigo - $descricao</option>";		
		}
	}
}
else{
	$sql = "select distinct a.prgcod||'.'||a.acacod||'.'||a.unicod as codigo, a.acadsc as descricao from acao a inner join progacaoproposto p on a.acacod=p.acacod and a.prgid=p.prgid and a.unicod=p.unicod and p.usucpf='".$usucpf."' where p.acacod is not null";
	$RS = $db->carregar($sql);
	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {
			for ($i=0; $i<=$nlinhas;$i++) {
				foreach($RS[$i] as $k=>$v) ${$k}=$v;
				print " <option value=\"$codigo\">$codigo - $descricao</option>";
			}
		}
} else {?>
<option value="">Clique no Programa para selecionar as A��es.</option>
<?	}
}?>
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


var campoSelect = document.getElementById("usuacaresp");
if (campoSelect.options[0].value != ''){
	v_prg=0;
	for(var i=0; i<campoSelect.options.length; i++)
		{ 	
			document.getElementById(campoSelect.options[i].value).checked = true;
			
			if (v_prg!=campoSelect.options[i].value.slice(0,campoSelect.options[i].value.indexOf('.')))
				{ 
					v_prg = campoSelect.options[i].value.slice(0,campoSelect.options[i].value.indexOf('.'));
					abreconteudo(campoSelect.options[i].value.slice(0,campoSelect.options[i].value.indexOf('.')));
					document.getElementById('ok').focus();
				}
		}
}

function abreconteudo(objeto)
{
if (document.getElementById('img'+objeto).name=='+')
	{
	document.getElementById('img'+objeto).name='-';
    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('mais.gif', 'menos.gif');
	document.getElementById(objeto).style.visibility = "visible";
	document.getElementById(objeto).style.display  = "";
	}
	else
	{
	document.getElementById('img'+objeto).name='+';
    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('menos.gif', 'mais.gif');
	document.getElementById(objeto).style.visibility = "hidden";
	document.getElementById(objeto).style.display  = "none";
	}
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
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique no Programa para selecionar as A��es.', '', false, false);}
			sortSelect(campoSelect);
	}
}

function moveto(obj) {
	if (obj.options[0].value != '') {
		if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
			abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
		}
		document.getElementById(obj.value).focus();}
}
</script>