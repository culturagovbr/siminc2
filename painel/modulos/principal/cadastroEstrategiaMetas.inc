<?php

include_once APPRAIZ.'/painel/classes/controller/EstrategiaMetaIndicador.inc';
include_once APPRAIZ.'/painel/classes/model/EstrategiaMetaIndicador.inc';
$cEstrategiaMeta = new Painel_Controller_Estrategia_Meta_Indicador();
$mEstrategiaMeta = new Painel_Model_Estrategia_Meta_Indicador();

/**
 * Tratamento das requisições
 */
$requisicao = $_REQUEST['req'];
switch ($requisicao) {
    case 'gravar':
        $cEstrategiaMeta->salvar($_REQUEST);
        break;
    case 'excluir':
        $cEstrategiaMeta->excluir($_REQUEST['emiid']);
        break;    
    case 'alterar':
        $mEstrategiaMeta->carregarPorId($_REQUEST['emiid']);
        $mEstrategiaMeta->popularDadosObjeto($_REQUEST);
        $emidsc = trim($mEstrategiaMeta->emidsc);
        $emiid = $mEstrategiaMeta->emiid;
        break;   
    /**
     * Requisição consultar utilizada pelo arquivo metaIndicador.inc
     */
    case 'consultar':
        $arrEstrategias = $mEstrategiaMeta->carregarPorIndid($_SESSION['indid']);
        if ($arrEstrategias){
            $table = '<table>';
            foreach($arrEstrategias as $value){
                $table .= '<tr><td>'.$value['emidsc'].'</td></tr>';
            }
            $table .= '</table>';
            echo $table;
        }
        die;       
    default:
        break;
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba(57105,$url,'');
?>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<form method="POST" id="formulario" name="formulario">
    <input type="hidden" name="evento" id="evento" value="I">
    <input type="hidden" name="indid" id="indid" value="<?=$_SESSION['indid']?>">
    <input type="hidden" name="emiid" id="emiid" value="<?=$emiid?>">
    <input type="hidden" name="req" id="req">
    <table style="border-bottom:0px"  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="TituloTela" align="center" colspan="2">Estratégia das Metas do Indicador</td>
        </tr>
        <tr>
            <td class="TituloTela" align="center" colspan="2"><br></td>
        </tr>        
        <tr>
            <td width="25%" align='right' class="SubTituloDireita">Descrição:</td>
            <td><?=campo_texto('emidsc','S','S','Digite a Descrição da Estratégia da Meta',100,255,'','','','',0,'','',$emidsc);?></td>
        </tr>
        <tr>
            <td align="center" colspan="2">
                <input type="button" class="botao" name="btassociar" value="Gravar" onclick="submeter('gravar');">
            </td>
        </tr>
    </table>
    <?php 
    $sql = $mEstrategiaMeta->retornaSqlEtrategiaMetas($_SESSION['indid']);
    $cabecalho = array("&nbsp;Ações&nbsp;&nbsp;&nbsp;&nbsp;", "Estratégia das Metas");
    $db->monta_lista($sql,$cabecalho,70,5,'N','left','','',array('5%','95%')); ?>
</form>
<script>
    function submeter(req){
        var emidsc = document.formulario.emidsc.value;
        if (emidsc.trim()!=''){
            document.formulario.req.value=req;
            document.formulario.submit();
        }else{
            alert('Digite uma descrição para a estratégia!');
        }
    }
</script>