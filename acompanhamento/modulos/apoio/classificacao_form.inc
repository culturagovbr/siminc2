<?php

$cClassificacao = new Acompanhamento_Controller_Classificacao();
switch ($_REQUEST['req']){
    case 'salvar':
        $cClassificacao->salvar($_REQUEST);
    die;
    case 'excluir':
        $cClassificacao->excluir($_REQUEST['claid']);
    die;
    case 'formulario-medida':
        $cClassificacao->montarFormularioMedida($_REQUEST['claid'], $_REQUEST['medid']);
    die;
    case 'listar-medida':
        $cClassificacao->montarListagemMedida($_REQUEST['claid']);
    die;
    case 'salvar-medida':
        $cMedida = new Acompanhamento_Controller_Medida();
        $cMedida->salvar($_REQUEST);
    die;
    case 'excluir-medida':
        $cMedida = new Acompanhamento_Controller_Medida();
        echo $cMedida->excluir($_REQUEST['medid']);
    die;
}

$mClassificacao = new Acompanhamento_Model_Classificacao($_REQUEST['claid']);

include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-8" id="div_classificacao">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="claid" id="claid" type="hidden" value="<?php echo $mClassificacao->claid; ?>">

                        <?php                         
                            echo $simec->input('cladsc', 'Classificação', $mClassificacao->cladsc, ['required', 'maxlength' => 200]);
                            echo $simec->select('tipid', 'Tipo', $mClassificacao->tipid, (new Acompanhamento_Model_Tipo())->recuperarSqlCombo(array(), null, 'tipdsc'), ['required']);
                            echo $simec->radioBoolean('clamedida', 'Possui medida a ser adotada', $mClassificacao->clamedida);
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/classificacao&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mClassificacao->claid){ ?>
                                    <a href="?modulo=apoio/classificacao_form&acao=A&req=excluir&claid=<?php echo $mClassificacao->claid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>

        <div class="col-md-4" id="div_medida">
            <div class="ibox float-e-margins">
            	<div class="ibox-title">
            		<h5>Dados de Medidas</h5>
            	</div>
            	<div class="ibox-content" id="">
                    <div id="div_formulario_medida">
                        <?php
                            if($mClassificacao->claid){
                                $cClassificacao->montarFormularioMedida($mClassificacao->claid);
                            }
                        ?>
                    </div>
                    <div id="div_listagem_medida">
                        <?php $cClassificacao->montarListagemMedida($mClassificacao->claid); ?>
                    </div>
            	</div>
            </div>
        </div>

    </div>
</div>

<script>

    $(document).ready(function(){

        toggleMedida();
        $('#clamedida').change(function(){
            toggleMedida();
        });

    });

    function toggleMedida(){
        if($('#clamedida').is(':checked')){
            $('#div_classificacao').removeClass('col-md-12').addClass('col-md-8');
            $('#div_medida').show('slow');
        } else {
            $('#div_classificacao').removeClass('col-md-8').addClass('col-md-12');
            $('#div_medida').hide('slow');
        }
    }

</script>