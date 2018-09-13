<?php

 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral
   Programador: Cristiano Cabral (e-mail: cristiano.cabral@gmail.com)
   Módulo:seleciona_unid_perfilresp.php
  
   */
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

# Verifica se a sessão não expirou, se tiver expirada envia pra tela de login.
controlarAcessoSemAutenticacao();

$db     = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
$secid   = $_REQUEST["secid"];

if ($_REQUEST["acoesresp"]){
    $acoesresp = $_REQUEST["acoesresp"];
    atribuiAcoes($usucpf, $pflcod, $acoesresp);
}

/**
 * Função que lista secretarias
 *
 */
function listaAcoes(){
    $db = new cls_banco();

    // SQL para buscar estados existentes
    $eixosExistentes = $db->carregar("
        SELECT
            secid,
            secdsc
        FROM painel.secretaria
        ORDER BY
            secordem ASC
    ");
    $count = count($eixosExistentes);

    // Monta as TR e TD com as unidades
    for ($i = 0; $i < $count; $i++){
        $codigo    = $eixosExistentes[$i]["secid"];
        $descricao = $eixosExistentes[$i]["secdsc"];
        if (fmod($i,2) == 0){ 
                $cor = '#f4f4f4';
        } else {
                $cor='#e0e0e0';
        }

        echo "<tr bgcolor=\"".$cor."\">
                        <td align=\"right\" width=\"10%\">
                                <input type=\"Checkbox\" name=\"secid\" id=\"".$codigo."\" value=\"".$codigo."\" onclick=\"retorna('".$i."');\">
                                <input type=\"hidden\" name=\"acadescricao\" value=\"".$codigo." - ".$descricao."\">
                        </td>
                        <td align=\"right\" style=\"color:blue;\" width=\"10%\">".$codigo."</td>
                        <td>".$descricao."</td>
                </tr>";
    }
			
}

function atribuiAcoes($usucpf, $pflcod, $sec){
    $db = new cls_banco();

    $data = date("Y-m-d H:i:s");

    $db->executar("UPDATE painel.usuarioresponsabilidade SET rpustatus = 'I' WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."'");
    if ($sec[0]){
        foreach($sec as $secid) {
            $dadosur = $db->carregar("SELECT * FROM painel.usuarioresponsabilidade WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."' AND secid = '". $secid ."'");
            if($dadosur) {
                // Se existir registro atualizar para ativo
                $db->carregar("UPDATE painel.usuarioresponsabilidade
                                           SET rpustatus = 'A', rpudata_inc= NOW()
                                           WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."' AND secid = '". $secid."'");
            } else {
                // Se não existir, inserir novo
                $db->executar("INSERT INTO painel.usuarioresponsabilidade(
                           pflcod, usucpf, secid, rpustatus, rpudata_inc)
                                   VALUES ('". $pflcod ."', '". $usucpf ."', '". $secid ."', 'A', NOW());");
            }
        }
    }
    $db->commit();

    echo '
        <script>
            alert(\'Operação realizada com sucesso!\');
            window.parent.opener.location.reload();
            self.close();
        </script>';
	
}

function buscaAcoesAtribuido($usucpf, $pflcod){
    $db = new cls_banco();

    $sql = "
        SELECT DISTINCT
            sec.secid AS codigo,
            sec.secdsc AS descricao
        FROM painel.usuarioresponsabilidade ur
            JOIN painel.secretaria sec ON ur.secid = sec.secid
        WHERE 
            ur.rpustatus = 'A'
            AND ur.usucpf = '$usucpf'
            AND ur.pflcod = $pflcod
        ORDER BY
            sec.secordem ASC
    ";

    $RS = @$db->carregar($sql);

    if(is_array($RS)) {
        $nlinhas = count($RS)-1;
        if ($nlinhas>=0) {
            for ($i=0; $i<=$nlinhas;$i++) {
                foreach($RS[$i] as $k=>$v) ${$k}=$v;
                    print " <option value=\"$codigo\">$codigo - $descricao</option>";		
            }
        }
    } else {
        print '<option value="">Clique no estado selecionar.</option>';
    }
}

?>

<?flush();?>
<html>
	<head>
		<meta http-equiv="Pragma" content="no-cache">
		<title>Secretarias</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	</head>
	<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<!-- Lista de Estados -->
		<div style="overflow:auto; width:496px; height:350px; border:2px solid #ececec; background-color: #ffffff;">
			<form name="formulario">
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
					<thead>
						<tr>
							<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="4"><strong>Selecione a Secretaria</strong></td>		
						</tr>
					</thead>
					<?php listaAcoes(); ?>
				</table>
			</form>
		</div>
		
		<!-- Estados Selecionadas -->
		<form name="formassocia" action="cadastro_responsabilidade_secretaria.php" method="post">
			<input type="hidden" name="usucpf" value="<?=$usucpf?>">
			<input type="hidden" name="pflcod" value="<?=$pflcod?>">
			<select multiple size="8" name="acoesresp[]" id="estresp" style="width:500px;" class="CampoEstilo">
				<?php 
					buscaAcoesAtribuido($usucpf, $pflcod);
				?>
			</select>
		</form>
		
		<!-- Submit do Formulário -->
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#c0c0c0">
				<td align="right" style="padding:3px;" colspan="3">
					<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);document.formassocia.submit();" id="ok">
				</td>
			</tr>
		</table>
	</body>
</html>

<script language="JavaScript">

var campoSelect = document.getElementById("estresp");

if (campoSelect.options[0].value != ''){
    for(var i=0; i<campoSelect.options.length; i++)
            {document.getElementById(campoSelect.options[i].value).checked = true;}
}



function abreconteudo(objeto){
    if (document.getElementById('img'+objeto).name=='+'){
        document.getElementById('img'+objeto).name='-';
        document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('mais.gif', 'menos.gif');
        document.getElementById(objeto).style.visibility = "visible";
        document.getElementById(objeto).style.display  = "";
    } else {
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
    if (document.formulario.secid[objeto].checked == true){
        campoSelect.options[tamanho] = new Option(document.formulario.acadescricao[objeto].value, document.formulario.secid[objeto].value, false, false);
        sortSelect(campoSelect);
    }
    else {
        for(var i=0; i<=campoSelect.length-1; i++){
            if (document.formulario.secid[objeto].value == campoSelect.options[i].value)
                    {campoSelect.options[i] = null;}
            }
            if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique no Estado.', '', false, false);}
            sortSelect(campoSelect);
    }
}

function moveto(obj) {
    if (obj.options[0].value != '') {
        if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
            abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
        }
        document.getElementById(obj.value).focus();
    }
}

</script>