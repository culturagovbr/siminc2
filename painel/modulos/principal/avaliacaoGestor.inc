<?php

include_once APPRAIZ.'/painel/classes/controller/Indicador.inc';
include_once APPRAIZ.'/painel/classes/model/Indicador.inc';
$cIndicador = new Painel_Controller_Indicador();
$mIndicador = new Painel_Model_Indicador();
$listaPerfis = $mIndicador->RetornaPerfil();
/**
 * Tratamento das requisições
 */
include APPRAIZ . 'includes/cabecalho.inc';
$requisicao = $_REQUEST['req'];
switch ($requisicao) {
    case 'gravar':
        if ($cIndicador->salvar($_REQUEST)){
            echo "<script>swal('','Dados Salvos com sucesso!!!', 'success');</script>";
        }else{
            echo "<script>swal('', 'Erro ao salvar dados!!!', 'error');</script>";
        }
        break; 
    default:
        break;
}
$dados = $mIndicador->RetornaRegistro($_SESSION['indid']);
$indobsgestor = $dados['indobsgestor'];
$indavalgestor = $dados['indavalgestor'];
// monta cabeçalho 
print '<br/>';
$db->cria_aba(57105,$url,'');
?>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<form method="POST" id="formulario" name="formulario">
    <input type="hidden" name="evento" id="evento" value="I">
    <input type="hidden" name="indid" id="indid" value="<?=$_SESSION['indid']?>">
    <input type="hidden" name="req" id="req">
    
    <table style="border-top:0px;border-top:0px;border-bottom:0px"  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="TituloTela" align="center" colspan="2">Avaliação do Gestor</td>
        </tr>
        <tr>
            <td class="TituloTela" align="center" colspan="2"><br></td>
        </tr>          
        <?php
        $classUp='fa-thumbs-o-up';
        $classDown='fa-thumbs-o-down';        
        $value = "";
        if ($indavalgestor=='S'){
            $classUp='fa-thumbs-up';
            $classDown='fa-thumbs-o-down';
            $value = "S";
        }else if ($indavalgestor=='N'){
            $classUp='fa-thumbs-o-up';
            $classDown='fa-thumbs-down';
            $value = "N";
        }
        ?>    
        <input type="hidden" name="indavalgestor" id="indavalgestor" value="<?=$value;?>">
        <?php
        if (in_array(PAINEL_PERFIL_GESTOR_ACOMPANHAMENTO, $listaPerfis)){            
        ?>    
            <tr>
                <td align="right" width="25%" class="SubTituloDireita">Avaliação do Gestor:</td>
                <td><a href="javascript:alteraClasseThumbs('S');"><i style="font-size:2.5em" id="indthumbsup" class="fa <?php echo $classUp;?>"></i></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="javascript:alteraClasseThumbs('N');"><i style="font-size:2.5em" id="indthumbsdown" class="fa <?php echo $classDown;?>"></i></a></td>
            </tr>
        <?php
        }else{
        ?>   
            <tr>
                <td align="right" width="25%" class="SubTituloDireita">Avaliação do Gestor:</td>
                <td><i style="font-size:2.5em" id="indthumbsup" class="fa <?php echo $classUp;?>"></i>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<i style="font-size:2.5em" id="indthumbsdown" class="fa <?php echo $classDown;?>"></i></td>
            </tr>    
        <?php
        }
        ?>
        <tr>
            <?php
            $acessoGestor='S';
            if (!in_array(PAINEL_PERFIL_GESTOR_INDICADOR, $listaPerfis)){
                $acessoGestor='N';
                echo "<input type='hidden' name='ind'>";
            }
            ?>
            <td align="right" width="25%" class="SubTituloDireita">Observação do Gestor:</td>
            <td><?=campo_textarea( 'indobsgestor', 'N', $acessoGestor, '', 98, 5, 125 );?></td>
        </tr>
        <tr>
            <td align="center" colspan="2">
                <input type="button" class="botao" name="btassociar" value="Gravar" onclick="submeter('gravar');">
            </td>
        </tr>        
    </table>
</form>
<script>
    function submeter(req){
        document.formulario.req.value=req;
        document.formulario.submit();
    }
    
    function alteraClasseThumbs(value){
        if (document.getElementById('indavalgestor').value==value){
            document.getElementById('indthumbsup').classList.add('fa-thumbs-o-up');
            document.getElementById('indthumbsup').classList.remove('fa-thumbs-up');
            document.getElementById('indthumbsdown').classList.add('fa-thumbs-o-down');        
            document.getElementById('indthumbsdown').classList.remove('fa-thumbs-down');
            document.getElementById('indavalgestor').value = '';
        }else{
            document.getElementById('indavalgestor').value = value;
            if (value=='S'){
                document.getElementById('indthumbsup').classList.remove("fa-thumbs-o-up");
                document.getElementById('indthumbsup').classList.add('fa-thumbs-up');
                document.getElementById('indthumbsdown').classList.remove('fa-thumbs-down');
                document.getElementById('indthumbsdown').classList.add('fa-thumbs-o-down');        
            }else{
                document.getElementById('indthumbsup').classList.remove('fa-thumbs-up');
                document.getElementById('indthumbsup').classList.add('fa-thumbs-o-up');
                document.getElementById('indthumbsdown').classList.remove('fa-thumbs-o-down');
                document.getElementById('indthumbsdown').classList.add('fa-thumbs-down');                
            }
        }

    }     
</script>