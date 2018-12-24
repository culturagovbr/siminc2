<?php
include_once APPRAIZ.'/painel/classes/controller/Indicador.inc';
include_once APPRAIZ.'/painel/classes/model/Indicador.inc';
//$cIndicador = new Painel_Controller_Indicador();
$mIndicador = new Painel_Model_Indicador();
$cAvaliacaoGestor = new Painel_Controller_AvaliacaoGestor();
$mAvaliacaoGestor = new Painel_Model_AvaliacaoGestor();
$listaPerfis = $mIndicador->RetornaPerfil();
/**
 * Tratamento das requisições
 */
include APPRAIZ . 'includes/cabecalho.inc';
$requisicao = $_REQUEST['requisicao'];
switch ($requisicao) {
    case 'gravar':
        if ($cAvaliacaoGestor->salvar($_REQUEST)){
            echo '<script>
            swal({
                title: "",
                text: "Dados Salvos com sucesso!!!",
                type: "success",
                showCancelButton: false,
                confirmButtonText: "Ok",
                closeOnConfirm: true
            },
            function(){
                window.location.href="?modulo=principal/avaliacaoGestor&acao=A";
            });
            </script>';
        }else{
            echo "<script>swal('', 'Erro ao salvar dados!!!', 'error');</script>";
        }
        break; 
    case 'excluir':
        if ($cAvaliacaoGestor->excluir($_REQUEST['agid'])){
            echo '<script>
            swal({
                title: "",
                text: "Dados excluidos com sucesso!!!",
                type: "success",
                showCancelButton: false,
                confirmButtonText: "Ok",
                closeOnConfirm: true
            },
            function(){
                window.location.href="?modulo=principal/avaliacaoGestor&acao=A";
            });
            </script>';
        }else{
            echo "<script>swal('', 'Erro ao excluir dados!!!', 'error');</script>";
        }
        break; 
    default:
        break;
}
$nuanoreferencia = $_REQUEST['nuanoreferencia']?$_REQUEST['nuanoreferencia']:$_SESSION['exercicio'];
$numesreferencia = $_REQUEST['numesreferencia']?$_REQUEST['numesreferencia']:date("m");    
if($_REQUEST['agid']){
    $dados = $mAvaliacaoGestor->RetornaRegistro($_REQUEST['agid']);
    $indobsgestor = $dados['indobsgestor'];
    $indavalgestor = $dados['indavalgestor'];
    $nuanoreferencia = $dados['nuanoreferencia'];
    $numesreferencia = $dados['numesreferencia'];
}
// monta cabeçalho 
print '<br/>';
$db->cria_aba(57105,$url,'');
?>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<style>
.corAzul
{
    color: #133368;
    TEXT-DECORATION: none;
}    
</style>
<form method="POST" id="formAvaliacao" name="formAvaliacao">
    <input type="hidden" name="evento" id="evento" value="I">
    <input type="hidden" name="indid" id="indid" value="<?=$_SESSION['indid']?>">
    <input type="hidden" name="agid" id="agid" value="<?=$_REQUEST['agid']?>">
    
    <table style="border-top:0px;border-top:0px;border-bottom:0px"  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="TituloTela" align="center" colspan="2">Avaliação do Gestor</td>
        </tr>
        <tr>
            <td class="TituloTela" align="center" colspan="2"><br></td>
        </tr> 
        <tr>
            <?php
                $anoInicial = date("Y")-7;
                $anoFinal = date("Y")+5;
                $j=0;
                for ($i=$anoInicial;$i<=$anoFinal;$i++){
                    $listaAnos[$j]['codigo']=$i;
                    $listaAnos[$j]['descricao']=$i;
                    $j++;
                }
                echo "<td class='SubTituloDireita' style='width:150px;'>Ano Referência:</td>";
                echo "<td>";
                   $db->monta_combo('nuanoreferencia', $listaAnos, 'S', 'Selecione', '', '', '', '200', 'S', 'nuanoreferencia','',$nuanoreferencia);
                echo "</td>";
            ?>
        </tr>
        <tr>
            <?php
                $listaMeses = Array(
                                    array('codigo' => 1, 'descricao'=>'Janeiro'),
                                    array('codigo' => 2, 'descricao'=>'Fevereiro'),
                                    array('codigo' => 3, 'descricao'=>'Março'),
                                    array('codigo' => 4, 'descricao'=>'Abril'),
                                    array('codigo' => 5, 'descricao'=>'Maio'),
                                    array('codigo' => 6, 'descricao'=>'Junho'),
                                    array('codigo' => 7, 'descricao'=>'Julho'),
                                    array('codigo' => 8, 'descricao'=>'Agosto'),
                                    array('codigo' => 9, 'descricao'=>'Setembro'),
                                    array('codigo' => 10, 'descricao'=>'Outubro'),
                                    array('codigo' => 11, 'descricao'=>'Novembro'),
                                    array('codigo' => 12, 'descricao'=>'Dezembro')
                                   );
                echo "<td class='SubTituloDireita' style='width:150px;'>Mês Referência:</td>";
                echo "<td>";
                   $db->monta_combo('numesreferencia', $listaMeses, 'S', 'Selecione', '', '', '', '200', 'S', 'numesreferencia','',$numesreferencia);
                echo "</td>";
            ?>
        </tr>
        <?php
        $classUp='fa-thumbs-o-up';
        $classDown='fa-thumbs-o-down'; 
        $classExclamation='fa-exclamation';
        $value = "";
        if ($indavalgestor=='S'){
            $classUp='fa-thumbs-up';
            $classDown='fa-thumbs-o-down';
            $classExclamation='fa-exclamation';
            $value = "S";
        }else if ($indavalgestor=='N'){
            $classUp='fa-thumbs-o-up';
            $classDown='fa-thumbs-down';
            $classExclamation='fa-exclamation';
            $value = "N";
        }else if ($indavalgestor=='E'){
            $classUp='fa-thumbs-o-up';
            $classDown='fa-thumbs-o-down';
            $classExclamation='fa-exclamation-circle';
            $value = "E";            
        }
        ?>    
        <input type="hidden" name="indavalgestor" id="indavalgestor" value="<?=$value;?>">
        <?php
        if (!in_array(PAINEL_PERFIL_GESTOR_INDICADOR, $listaPerfis)){            
        ?>    
            <tr>
                <td align="right" width="25%" class="SubTituloDireita">Avaliação do Gestor:</td>
                <td>
                    <a href="javascript:alteraClasseThumbs('S');"><i style="font-size:2.5em" id="indthumbsup" class="fa <?php echo $classUp;?>"></i></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                    <a href="javascript:alteraClasseThumbs('N');"><i style="font-size:2.5em" id="indthumbsdown" class="fa <?php echo $classDown;?>"></i></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                    <a href="javascript:alteraClasseThumbs('E');"><i style="font-size:2.5em" id="indthumbsexclamation" class="fa <?php echo $classExclamation;?>"></i></a>
                </td>
            </tr>
        <?php
        }else{
        ?>   
            <tr>
                <td align="right" width="25%" class="SubTituloDireita">Avaliação do Gestor:</td>
                <td>
                    <i style="font-size:2.5em" id="indthumbsup" class="fa <?php echo $classUp;?>"></i>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                    <i style="font-size:2.5em" id="indthumbsdown" class="fa <?php echo $classDown;?>"></i>
                    <i style="font-size:2.5em" id="indthumbsexclamation" class="fa <?php echo $classExclamation;?>"></i>
                </td>
            </tr>    
        <?php
        }
        ?>
        <tr>
            <?php
            $acessoGestor='S';
            if (in_array(PAINEL_PERFIL_GESTOR_INDICADOR, $listaPerfis)){
                $acessoGestor='N';
            }
            ?>
            <td align="right" width="25%" class="SubTituloDireita">Observação do Gestor:</td>
            <td><?=campo_textarea( 'indobsgestor', 'N', $acessoGestor, '', 98, 5, 125 );?></td>
        </tr>
        <tr>
            <?php
            $sql = "select secid 
              from painel.usuarioresponsabilidade 
             where usucpf = '".$_SESSION['usucpf']."' 
               and pflcod = ".PAINEL_PERFIL_GESTOR_INDICADOR." 
               and secid = (select secid from painel.indicador where indid='".$_SESSION['indid']."')";
            $secid_confirma = $db->pegaUm($sql);
            $acessoGestor='S';
            if (in_array(PAINEL_PERFIL_GESTOR_INDICADOR, $listaPerfis) && !$secid_confirma){
                $acessoGestor='N';             
            }
            ?>
            <td align="right" width="25%" class="SubTituloDireita">Observação do Gestor Indicador:</td>
            <td><?=campo_textarea( 'indobsgestorindicador', 'N', $acessoGestor, '', 98, 5, 255 );?></td>
        </tr>
        <tr>
            <td align="center" colspan="2">
                <?php
                if ($secid_confirma || (in_array(PAINEL_PERFIL_SUPER_USUARIO, $listaPerfis) || in_array(PAINEL_PERFIL_GESTOR_ACOMPANHAMENTO, $listaPerfis))){
                    echo '<input type="button" class="botao" name="btassociar" value="Gravar" onclick="submeter(\'gravar\');">';
                }
                ?>
            </td>
        </tr>        
    </table>
    <?php
    	$cabecalho = array("Ações","Ano","Mês","Avaliação Gestor","Obs. Gestor","Obs. Indicador");
        $arrAvaliacoes = $mAvaliacaoGestor->retornaListaAvaliacoes();
        for($i=0;$i<count($arrAvaliacoes);$i++){
            foreach($listaMeses as $value){
                if ($value['codigo']==$arrAvaliacoes[$i]['numesreferencia']){
                    $arrAvaliacoes[$i]['dsmesreferencia'] = $value['descricao'];
                }
            }
        }
	$db->monta_lista_array($arrAvaliacoes,$cabecalho,100,5,'N','center');
    ?>
</form>
<script>
    function submeter(req){
        var dados = $("#formAvaliacao").serialize();
        window.location.href='?modulo=principal/avaliacaoGestor&acao=A&requisicao='+req+'&dados='+dados;
    }
    
    function alteraClasseThumbs(value){
        if (document.getElementById('indavalgestor').value==value){
            document.getElementById('indthumbsup').classList.add('fa-thumbs-o-up');
            document.getElementById('indthumbsup').classList.remove('fa-thumbs-up');
            document.getElementById('indthumbsdown').classList.add('fa-thumbs-o-down');        
            document.getElementById('indthumbsdown').classList.remove('fa-thumbs-down');
            document.getElementById('indthumbsexclamation').classList.add('fa-exclamation');        
            document.getElementById('indthumbsexclamation').classList.remove('fa-exclamation-circle');
            document.getElementById('indavalgestor').value = '';
        }else{
            document.getElementById('indavalgestor').value = value;
            if (value=='S'){
                document.getElementById('indthumbsup').classList.remove("fa-thumbs-o-up");
                document.getElementById('indthumbsup').classList.add('fa-thumbs-up');
                document.getElementById('indthumbsdown').classList.remove('fa-thumbs-down');
                document.getElementById('indthumbsdown').classList.add('fa-thumbs-o-down');        
                document.getElementById('indthumbsexclamation').classList.add('fa-exclamation');        
                document.getElementById('indthumbsexclamation').classList.remove('fa-exclamation-circle');
            }else if (value=='N'){
                document.getElementById('indthumbsup').classList.remove('fa-thumbs-up');
                document.getElementById('indthumbsup').classList.add('fa-thumbs-o-up');
                document.getElementById('indthumbsdown').classList.remove('fa-thumbs-o-down');
                document.getElementById('indthumbsdown').classList.add('fa-thumbs-down');
                document.getElementById('indthumbsexclamation').classList.add('fa-exclamation');        
                document.getElementById('indthumbsexclamation').classList.remove('fa-exclamation-circle');
            }else{
                document.getElementById('indthumbsup').classList.remove('fa-thumbs-up');
                document.getElementById('indthumbsup').classList.add('fa-thumbs-o-up');
                document.getElementById('indthumbsdown').classList.add('fa-thumbs-o-down');
                document.getElementById('indthumbsdown').classList.remove('fa-thumbs-down');
                document.getElementById('indthumbsexclamation').classList.remove('fa-exclamation');        
                document.getElementById('indthumbsexclamation').classList.add('fa-exclamation-circle');
            }
        }

    }    
    
    function alterarAvaliacao(agid){
        window.location.href='?modulo=principal/avaliacaoGestor&acao=A&agid='+agid;
    }  
    
    function excluirAvaliacao(agid){
        window.location.href='?modulo=principal/avaliacaoGestor&acao=A&requisicao=excluir&agid='+agid;
    }
</script>