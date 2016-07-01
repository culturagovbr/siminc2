<?PHP
    include "config.inc";
    include "../_funcoes.php";
    header('Content-Type: text/html; charset=iso-8859-1');

    include APPRAIZ . "includes/classes_simec.inc";
    include APPRAIZ . "includes/funcoes.inc";

    $db = new cls_banco();

    $usucpf = $_REQUEST['usucpf'];
    $pflcod = $_REQUEST['pflcod'];

    /**
     * INICIO REGISTRO RESPONSABILIDADES
     */

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }


    /**
     * functionName atualizaComboMunicipio
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $estuf sigla do estado.
     * @return string  retorna a o combo com os munic�pios referente ao estado (UF).
     *
     * @version v1
    */
    function atualizaMunicipioResponsabilidade( $estuf ){
        global $db;

        $estuf = $estuf['estuf'];

        $sql = "
            SELECT distinct  m.muncod as codigo,
                    m.mundescricao as descricao
            FROM gestaodocumentos.mantenedoras ma
			inner join entidade.entidade e on e.entnumcpfcnpj = ma.mntcnpj
			inner join entidade.endereco ed on ed.entid = e.entid
            LEFT JOIN territorios.municipio AS m ON m.muncod = ed.muncod
            LEFT JOIN territorios.estado AS u ON u.estuf = m.estuf
            WHERE m.estuf = '{$estuf}'
            ORDER BY 2
        ";
        $db->monta_combo("muncod", $sql, 'S', 'Selecione...', '', '', '', '340', 'N', 'muncod', false, $muncod, null);
        die();
    }

    if (is_array($_POST['edtSelecionadas'])) {
        $sql = "
            UPDATE maismedicomec.usuarioresponsabilidade SET rpustatus = 'I' WHERE usucpf = '{$usucpf}' AND pflcod = {$pflcod}
        ";
        $db->executar($sql);

        if ($_POST['edtSelecionadas'][0]) {
            foreach ($_POST['edtSelecionadas'] as $mntid) {

                $sql = "
                    INSERT INTO maismedicomec.usuarioresponsabilidade(
                        pflcod, usucpf, rpustatus, rpudata_inc, mntid
                    ) VALUES (
                        {$pflcod}, '{$usucpf}', 'A', 'now()', {$mntid}
                    )
                ";
                $db->executar($sql);
            }
        }
        $db->commit();
        $db->sucesso('', '&acao=A&pflcod='.$pflcod.'&usucpf='.$usucpf, 'Opera��o realizada com sucesso', 'S', 'S', 'S');
    }

/*
 * ** FIM REGISTRO RESPONSABILIDADES ***
 */
?>
<html>
    <head>
        <META http-equiv="Pragma" content="no-cache">
        <title>Munic�pio</title>

        <script type="text/javascript" src="../../includes/funcoes.js"></script>
        <script type="text/javascript" src="../../includes/JQuery/jquery-1.4.2.js"></script>

        <link rel="stylesheet" type="text/css" href="/includes/Estilo.css">
        <link rel='stylesheet' type='text/css' href='/includes/listagem.css'>

        <style type="text/css">
            input, select, td{
                font-size: 11px !important;
            }
        </style>

    </head>

    <body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" margineight="0" bgcolor="#ffffff">

        <div align=center id="aguarde" style="visibility:hidden; display:none;">
            <img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle" > <font color=blue size="2">Aguarde! Carregando Dados...</font>
        </div>

        <div style="overflow:auto; width:100%; height:auto; border:2px solid #ECECEC; background-color: White; border: 1px solid black;">
            <form action="" method="POST" id="formulario" name="formulario">
                <input type="hidden" id="requisicao" name="requisicao" value=""/>
                <input type="hidden" id="usucpf" name="usucpf" value="<?=$usucpf;?>">
                <input type="hidden" id="pflcod" name="pflcod" value="<?=$pflcod;?>">

                <div style="position:fixed; background-color: #FFFFFF; height:9px; width:100%;">
                    <table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="tabela">
<!--                        <tr>-->
<!--                            <td class ="SubTituloDireita">UF:</td>-->
<!--                            <td>-->
<!--                                --><?PHP
//                                	$estuf = $_POST['estuf'];
//                                    $sql = "
//                                        SELECT  estuf AS codigo,
//                                                estuf ||' - '|| estdescricao AS descricao
//                                        FROM territorios.estado
//                                        ORDER BY estuf
//                                    ";
//                                    $db->monta_combo("estuf", $sql, 'S', 'Selecione...', 'atualizaMunicipioResponsabilidade', '', '', '320', 'N', 'estuf', false, $estuf, null);
//                                ?>
<!--                            </td>-->
<!--                        </tr>-->
<!--                        <tr>-->
<!--                            <td class ="SubTituloDireita">Munic�pio:</td>-->
<!--                            <td id="td_combo_municipio">-->
<!--                                --><?PHP
//                                	$muncod = $_POST['muncod'];
//                                    $sql = "
//                                        SELECT  muncod AS codigo,
//                                                mundescricao AS descricao
//                                        FROM territorios.municipio
//                                        ORDER BY mundescricao
//                                        --LIMIT 500
//                                    ";
//                                    $db->monta_combo("muncod", $sql, 'S', 'Selecione...', '', '', '', '320', 'N', 'muncod', false, $muncod, null);
//                                ?>
<!--                            </td>-->
<!--                        </tr>-->
                        <tr>
                            <td class ="SubTituloDireita" width="30%">Mantenedora:</td>
                            <td>
                            	<?
								$mntdsc = $_POST['mntdsc'];
                            	?>
                                <?= campo_texto('mntdsc', 'N', 'S', '', 38, 20, '', '', '', '', 0, 'id="mntdsc"', '', $mntdsc, null, '', null); ?>
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloCentro" colspan="2">
                                <input type="button" name="pesquisar" value="Pesquisar" onclick="document.formulario.submit();"/>
                            </td>
                        </tr>
                    </table>
                </div>

                <div style="background-color: #FFFFFF; position:fixed; top:27%; width:100%; height:41%; overflow:auto;">
                    <table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="tabela listagem">
                        <tr>
                            <td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="4">
                                <strong>Selecione a(s) Mantenedora(s)</strong>
                            </td>
                        </tr>
                        <tr>
                            <td>&nbsp;</td>
                            <td><b>C�digo</b></td>
                            <td><b>Mantenedora</b></td>
                        </tr>

                        <?PHP

                            if ($_REQUEST['estuf']) {
                                $estuf = $_REQUEST['estuf'];
                                $where[] = " m.estuf = '{$estuf}'";
                            }
                            if ($_REQUEST['mntid']) {
                                $mntid = $_REQUEST['mntid'];
                                $where[] = " m.muncod = '{$muncod}'";
                            }
                            if ($_REQUEST['mntdsc']) {
                                $mntdsc = $_REQUEST['mntdsc'];
                                $where[] = " public.removeacento(mntdsc) ILIKE public.removeacento( ('%{$mntdsc}%') ) ";
                            }

                            if($where != ""){
                                $WHERE = "WHERE " . implode(' AND ', $where);
                            }

                            $cabecalho = 'Selecione o(s) Municip�os(s)';

                            $sql = "
                                SELECT distinct  mntid as codigo,
                                        mntdsc as descricao
                                FROM gestaodocumentos.mantenedoras ma
								--inner join entidade.entidade e on e.entnumcpfcnpj = ma.mntcnpj
								--inner join entidade.endereco ed on ed.entid = e.entid
                                --LEFT JOIN territorios.municipio AS m ON m.muncod = ed.muncod
                                --LEFT JOIN territorios.estado AS u ON u.estuf = m.estuf

                                {$WHERE}
                                ORDER BY 2
                            ";
                                
                            //dbg($sql);

                            $RS = @$db->carregar($sql);

                            if ($RS) {
                                $nlinhas = count($RS) - 1;

                                for ($i = 0; $i <= $nlinhas; $i++) {
                                    extract($RS[$i]);

                                    if (fmod($i, 2) == 0){
                                        $cor = '#f4f4f4';
                                    }else{
                                        $cor = '#e0e0e0';
                                    }
                        ?>
                                    <tr bgcolor="<?= $cor ?>">
                                        <td align="center">
                                            <input type="checkbox" name="codigo" id="<?= $codigo ?>" value="<?= $codigo ?>" onclick="retorna(<?= $i ?>);">
                                            <input type="Hidden" name="descricao_select" value="<?= $codigo . ' - ' . $descricao ?>"></td>
                                        <td align="right" style="color:blue; width:85px; text-align:center;"> <?= $codigo ?> </td>
                                        <td><?= $descricao?></td>
                                    </tr>

                        <?PHP
                                }
                            } else {
                        ?>
                                <tr>
                                    <td align="center" colspan="3" style="color: rgb(204, 0, 0);">N�o foram encontrados Registros.</td>
                                </tr>
                                <?php
                            }
                        ?>
                    </table>
                </div>
            </form>

            <div style="background-color: #FFFFFF; position:fixed; top:69%; width:100%; height:25%;">
                <form name="formassocia" method="POST">
                    <input type="hidden" name="usucpf" value="<?= $usucpf ?>">
                    <input type="hidden" name="pflcod" value="<?= $pflcod ?>">
                    <select multiple size="8" name="edtSelecionadas[]" id="edtSelecionadas" style="width:99%;" class="CampoEstilo" onkeydown="javascript:combo_popup_remove_selecionados( event, 'edtSelecionadas' );">
                        <?PHP
                            $sql = "
                                SELECT  DISTINCT m.mntid as codigo,
                                        m.mntdsc as descricao
                                FROM maismedicomec.usuarioresponsabilidade ur
                                INNER JOIN gestaodocumentos.mantenedoras m ON m.mntid = ur.mntid
                               WHERE ur.rpustatus='A' AND ur.usucpf = '{$usucpf}' AND ur.pflcod = {$pflcod}
                            ";
                            $RS = @$db->carregar($sql);

                            if (is_array($RS)) {
                                $nlinhas = count($RS) - 1;
                                if ($nlinhas >= 0) {
                                    for ($i = 0; $i <= $nlinhas; $i++) {
                                        foreach ($RS[$i] as $k => $v)
                                            ${$k} = $v;
                                        print " <option value=\"$codigo\">$codigo - $descricao</option>";
                                    }
                                }
                            } else {
                        ?>
                            <option value="">Clique na Mantenedora.</option>
                        <?PHP
                            }
                        ?>
                    </select>
                </form>
            </div>

            <div style="background-color: #FFFFFF; position:fixed; top:94%; width:100%; height:25%;">
                <table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
                    <tr bgcolor="#c0c0c0">
                        <td align="right" style="padding:3px;" colspan="3">
                            <input type="Button" name="ok" value="OK"  id="ok">
                        </td>
                    </tr>
                </table>
            </div>
        </div>

        <script language="JavaScript">

            var campoSelect = document.getElementById("edtSelecionadas");

            $('#ok').click(function(){
                selectAllOptions(campoSelect); document.formassocia.submit();
//                alert('teste');
//                window.opener.reload();
                window.opener.location.reload();
//                window.onbeforeunload = function (event) {
//                               var message = 'Realmente deseja sair??';
//                               if (typeof event == 'undefined') {
//                                  event = window.event;
//                               }
//                               if (event) {
//                                  event.returnValue = message;
//                               }
//                               return message;
//                            }
            });

            if (campoSelect.options[0].value != ''){
                for(var i=0; i<campoSelect.options.length; i++){
                    document.getElementById(campoSelect.options[i].value).checked = true;
                }
            }

            function abreconteudo(objeto){
                if (document.getElementById('img' + objeto).name == '+'){
                    document.getElementById('img' + objeto).name = '-';
                    document.getElementById('img' + objeto).src = document.getElementById('img' + objeto).src.replace('mais.gif', 'menos.gif');
                    document.getElementById(objeto).style.visibility = "visible";
                    document.getElementById(objeto).style.display = "";
                }else{
                    document.getElementById('img' + objeto).name = '+';
                    document.getElementById('img' + objeto).src = document.getElementById('img' + objeto).src.replace('menos.gif', 'mais.gif');
                    document.getElementById(objeto).style.visibility = "hidden";
                    document.getElementById(objeto).style.display = "none";
                }
            }

            function retorna(objeto){
                tamanho = campoSelect.options.length;
                if (campoSelect.options[0].value == '') {
                    tamanho--;
                }

                var codigo = document.getElementsByName('codigo');
                var descricao = document.getElementsByName('descricao_select');

                if (codigo[objeto].checked == true) {
                    campoSelect.options[tamanho] = new Option(descricao[objeto].value, codigo[objeto].value, false, false);
                    sortSelect(campoSelect);
                } else {
                    for (var i = 0; i <= campoSelect.length - 1; i++) {
                        if (codigo[objeto].value == campoSelect.options[i].value){
                            campoSelect.options[i] = null;
                        }
                    }

                    if (!campoSelect.options[0]){
                        campoSelect.options[0] = new Option('Clique na Mantenedora.', '', false, false);
                    }
                    sortSelect(campoSelect);
                }
            }

            function moveto(obj) {
                /*
                if (obj.options[0].value != '') {
                    if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
                        abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
                    }
                    document.getElementById(obj.value).focus();
                 }
                 */
            }

            function pesquisar(param){
                alert('oi');
                if(trim(param) == 'fil'){
                    $('#formulario').submit();
                }else{
                    $('#formulario').submit();
                }
            }

            function atualizaMunicipioResponsabilidade( estuf ){
                $.ajax({
                    type    : "POST",
                    url     : window.location,
                    data    : "requisicao=atualizaMunicipioResponsabilidade&estuf="+estuf,
                    async: false,
                    success: function(resp){
                        $('#td_combo_municipio').html(resp);
                    }
                });
            }

        </script>
