<?
/*
Sistema Simec
Setor respons�vel: SPO-MEC
Desenvolvedor: Equipe Consultores Simec
Analista: Cristiano Cabral
Programador: Cristiano Cabral (e-mail: cristiano.cabral@gmail.com)
M�dulo:seleciona_unid_perfilresp.php
*/

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$db     = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
$acao   = $_REQUEST["acao"];

if ($_REQUEST["cocresp"]){
    $cocresp = $_REQUEST["cocresp"];
    atribuirItensSelecionados($usucpf, $pflcod, $cocresp);
}

/**
 * Fun��o que atriui Itens ao usu�rio.
 **/
function atribuirItensSelecionados($usucpf, $pflcod, $cocresp){
    $db = new cls_banco();

    $sql = "UPDATE pde.usuarioresponsabilidade SET rpustatus = 'I' WHERE usucpf = '{$usucpf}' AND pflcod = {$pflcod} RETURNING rpuid;";
    $rpuid_del = $db->pegaUm($sql);

    if ($cocresp[0]){
        foreach($cocresp as $dados){

            $dadosur = $db->carregar("SELECT * FROM pde.usuarioresponsabilidade WHERE usucpf = '{$usucpf}' AND pflcod = {$pflcod} AND cocid = {$dados}");

            if($dadosur) {
                $sql = "
                    UPDATE pde.usuarioresponsabilidade
                            SET rpustatus   = 'A',
                                rpudata_inc = NOW()

                    WHERE usucpf = '{$usucpf}' AND pflcod = '{$pflcod}' AND cocid = {$dados} RETURNING rpuid;
                ";
                $rpuid = $db->pegaUm($sql);
            }else{
                $sql = "
                    INSERT INTO pde.usuarioresponsabilidade(
                            pflcod,
                            usucpf,
                            cocid,
                            rpustatus,
                            rpudata_inc
                        )VALUES(
                            {$pflcod},
                            '{$usucpf}',
                            {$dados},
                            'A',
                            NOW()
                    ) RETURNING rpuid;
                ";
                $rpuid = $db->pegaUm($sql);
            }
        }
    }
    
    if( $rpuid > 0 || $rpuid_del > 0){
	$db->commit();
        echo "
            <script>
                alert('Opera��o realizada com sucesso!');
		window.parent.opener.location.reload();
		self.close();
            </script>
        ";
    }else{
        echo "
            <script>
                alert('N�o foi possiv�l realizar a opera��o!');
		self.close();
            </script>
        ";
    }
}

/**
 * Fun��o que busca o itens relacionados com o usu�rio
 **/
function buscaItensAtribuido($usucpf, $pflcod){
    $db = new cls_banco();

    $sql = "
        SELECT  c.cocid AS codigo,
                c.cocnome AS descricao
        FROM pde.usuarioresponsabilidade ur

        JOIN pde.cockpit c ON c.cocid = ur.cocid

        WHERE ur.rpustatus = 'A' AND ur.usucpf = '$usucpf' AND ur.pflcod = $pflcod
    ";
    $RS = @$db->carregar($sql);

    if(is_array($RS)) {
        $nlinhas = count($RS)-1;
        if ($nlinhas>=0) {
            for ($i=0; $i<=$nlinhas;$i++) {
                foreach($RS[$i] as $k=>$v) ${$k}=$v;

                echo " <option value=\"$codigo\">$codigo - $descricao</option>";
            }
        }
    }else{
        print '<option value="">Clique no Item para selecion�-lo.</option>';
    }
}

/**
 * Fun��o que lista os Cocpits cadasstrados ao PDE
 **/
function listaCockpit(){
    $db = new cls_banco();

    $sql = "
        SELECT  cocid,
                cocnome
        FROM pde.cockpit
        ORDER BY 2
    ";
    $cockpit = $db->carregar($sql);

    $count = count($cockpit);

    for ($i = 0; $i < $count; $i++){
        $codigo    = $cockpit[$i]["cocid"];
        $descricao = $cockpit[$i]["cocnome"];

        if (fmod($i,2) == 0){
            $cor = '#f4f4f4';
        } else {
            $cor='#e0e0e0';
        }

        echo "
            <tr bgcolor=\"".$cor."\">
                <td align=\"right\" width=\"10%\">
                    <input type=\"Checkbox\" name=\"cocid\" id=\"".$codigo."\" value=\"".$codigo."\" onclick=\"retorna('".$i."');\">
                    <input type=\"hidden\" name=\"cocnome\" value=\"".$codigo." - ".$descricao."\">
                </td>
                <td align=\"right\" style=\"color:blue;\" width=\"10%\">".$codigo."</td>
                <td>".$descricao."</td>
            </tr>
        ";
    }
}

flush();

?>

<html>
    <head>
        <meta http-equiv="Pragma" content="no-cache">
        <title>Cockpits</title>
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
                            <td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="4"><strong>Selecione o tipo de ensino</strong></td>
                        </tr>
                    </thead>
                    <?php listaCockpit(); ?>
                </table>
            </form>
        </div>

        <!-- Estados Selecionadas -->
        <form name="formassocia" action="cadastro_responsabilidade_cockpit.php" method="post">
            <input type="hidden" name="usucpf" value="<?= $usucpf ?>">
            <input type="hidden" name="pflcod" value="<?= $pflcod ?>">
            <select multiple size="8" name="cocresp[]" id="cocresp" style="width:500px;" class="CampoEstilo">
                <?php buscaItensAtribuido($usucpf, $pflcod); ?>
            </select>
        </form>

        <!-- Submit do Formul�rio -->
        <table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
            <tr bgcolor="#c0c0c0">
                <td align="right" style="padding:3px;" colspan="3">
                    <input type="Button" name="ok" id="ok" value="OK" onclick="selectAllOptions(campoSelect); document.formassocia.submit();">
                </td>
            </tr>
        </table>
    </body>
</html>

<script language="JavaScript">

    var campoSelect = document.getElementById("cocresp");

    if (campoSelect.options[0].value != ''){
        for(var i=0; i<campoSelect.options.length; i++){
            document.getElementById(campoSelect.options[i].value).checked = true;
        }
    }

    function abreconteudo(objeto){
        if (document.getElementById('img'+objeto).name=='+'){
            document.getElementById('img'+objeto).name='-';
            document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('mais.gif', 'menos.gif');
            document.getElementById(objeto).style.visibility = "visible";
            document.getElementById(objeto).style.display  = "";
        }else{
            document.getElementById('img'+objeto).name='+';
            document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('menos.gif', 'mais.gif');
            document.getElementById(objeto).style.visibility = "hidden";
            document.getElementById(objeto).style.display  = "none";
        }
    }

    function retorna(objeto){
        tamanho = campoSelect.options.length;

        if (campoSelect.options[0].value=='') {
            tamanho--;
        }

        if (document.formulario.cocid[objeto].checked == true){
            campoSelect.options[tamanho] = new Option(document.formulario.cocnome[objeto].value, document.formulario.cocid[objeto].value, false, false);
            sortSelect(campoSelect);
        }else{
            for(var i=0; i<=campoSelect.length-1; i++){
                if (document.formulario.cocid[objeto].value == campoSelect.options[i].value){
                    campoSelect.options[i] = null;
                }
            }

            if (!campoSelect.options[0]){
                campoSelect.options[0] = new Option('Clique no Cockpit.', '', false, false);
            }
            sortSelect(campoSelect);
        }
    }

    function moveto(obj){
        if (obj.options[0].value != ''){
            if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
                abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
            }
            document.getElementById(obj.value).focus();
        }
    }

</script>
<?php die; ?>