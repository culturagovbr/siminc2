<?php
/**
 * Gerencia o cadastro de responsabilidades para usu�rioXug.
 * $Id: cadastro_responsabilidade_ug.php 71163 2013-11-25 19:48:00Z wescleylima $
 */

/**
 * Configura��es do sistema
 */
require_once "config.inc";

include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

function gravarResponsabilidadeAcao($dados) {
    global $db;
    $sql = "UPDATE planacomorc.usuarioresponsabilidade SET rpustatus='I' WHERE usucpf='".$dados['usucpf']."' AND pflcod='".$dados['pflcod']."'";
    $db->executar($sql);

    if ($dados['usuacaresp']) {
        foreach($dados['usuacaresp'] as $ungcod) {

            $sql = <<<DML
INSERT INTO planacomorc.usuarioresponsabilidade(pflcod, usucpf, rpustatus, rpudata_inc, ungcod)
  VALUES ('{$dados['pflcod']}', '{$dados['usucpf']}', 'A', NOW(), '{$ungcod}')
DML;
            $db->executar($sql);
        }
    }
    $db->commit();

    echo "<script language=\"javascript\">
                alert(\"Opera��o realizada com sucesso!\");
                opener.location.reload();
                self.close();
          </script>";
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
?>
<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>Defini��o de responsabilidades - Unidade Gestora</title>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script src="../js/planacomorc.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='/includes/listagem.css'>
<style type="text/css">
.tabela{width:100%}
</style>
</head>
<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff" onload="self.focus()">
<script>
function carregarAcoesUnidade(orgcod,obj) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.title    = "menos";
		obj.src      = "../../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex+1);
		var ncol     = nlinha.insertCell(0);
		ncol.colSpan = 8;
		ncol.id      = 'tr_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=carregarAcoesUnidade&orgcod='+orgcod,ncol.id);
		jQuery("#usuacaresp option").each(function() {
			if ( jQuery("#chk_"+jQuery(this).val()).length ){
				jQuery("#chk_"+jQuery(this).val()).attr('checked',true);
			}
		});
	} else {
		obj.title    = "mais";
		obj.src      = "../../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex+1);
	}
}

function marcarAcao(obj) {
    if(obj.checked) {
        if (!jQuery('#usuacaresp option[value='+obj.value+']')[0]) {
            jQuery("#usuacaresp").append('<option value='+obj.value+'>'+obj.parentNode.parentNode.cells[1].innerHTML+'</option>');
        }
    } else {
        jQuery('#usuacaresp option[value='+obj.value+']').remove();
    }
}
</script>
<div style="overflow:auto;width:496px;height:350px;border:2px solid #ececec;background-color:white">
<?php
monta_titulo('Defini��o de responsabilidades - Unidade Gestora', '');

// -- � feita uma verifica��o no SQL para saber se aquele ungcod j� foi escolhido previamente
// -- com base nisso, � adicionado o atributo checked ao combo do ungcod selecionado previamente.
$sql = <<<DML
SELECT '<input type="checkbox" name="ungcod" id="chk_' || ung.ungcod || '" value="' || ung.ungcod || '" '
           || 'onclick="marcarAcao(this)"'
           || CASE WHEN urp.rpuid IS NOT NULL AND urp.rpustatus = 'A' THEN ' checked' ELSE '' END || '>' AS ungcod,
       ung.ungcod || ' - ' || ung.ungdsc AS descricao
  FROM public.unidadegestora ung
    LEFT JOIN planacomorc.usuarioresponsabilidade urp
      ON urp.ungcod = ung.ungcod AND urp.usucpf = '{$usucpf}' AND urp.pflcod = '{$pflcod}'
  WHERE ung.ungstatus = 'A'
  ORDER BY ung.ungcod
DML;

$cabecalho = array('', 'UG - Descri��o');
$db->monta_lista_simples($sql, $cabecalho, 500, 5, 'N', '100%', 'N');
?>
</div>
<form name="formassocia" style="margin:0px;" method="POST">
<input type="hidden" name="usucpf" value="<?=$usucpf?>">
<input type="hidden" name="pflcod" value="<?=$pflcod?>">
<input type="hidden" name="requisicao" value="gravarResponsabilidadeAcao">
<select multiple size="8" name="usuacaresp[]" id="usuacaresp" style="width:500px;" class="CampoEstilo">
<?php
$sql = <<<DML
SELECT ung.ungcod AS codigo,
       ung.ungcod || ' - ' || ung.ungdsc AS descricao
  FROM planacomorc.usuarioresponsabilidade ur
    INNER JOIN public.unidadegestora ung USING(ungcod)
  WHERE ur.usucpf = '{$usucpf}'
    AND ur.pflcod = '{$pflcod}'
    AND ur.rpustatus = 'A'
DML;
$usuarioresponsabilidade = $db->carregar($sql);

if($usuarioresponsabilidade[0]) {
	foreach($usuarioresponsabilidade as $ur) {
		echo '<option value="'.$ur['codigo'].'">'.$ur['descricao'].'</option>';
	}
}
?>
</select>
</form>
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
	<tr bgcolor="#c0c0c0">
		<td align="right" style="padding:3px;" colspan="3">
			<input type="Button" name="ok" value="OK"
                   onclick="selectAllOptions(document.getElementById('usuacaresp'));document.formassocia.submit();"
                   id="ok">
		</td>
	</tr>
</table>
