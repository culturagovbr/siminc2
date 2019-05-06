<?php

$cAreaCultural = new Planacomorc_Controller_AreaCultural();
$cSegmentoCultural = new Public_Controller_SegmentoCultural();
$mAreaCultural = new Planacomorc_Model_AreaCultural($_REQUEST['mdeid']);

$mSegmentoCultural = new Monitora_Model_PiNivelEtapaEnsino($_REQUEST['neeid']);

if($_REQUEST['mdeid'])
    $lista = $mSegmentoCultural->RecuperaSegmentoPorArea($_REQUEST['mdeid']);

switch ($_REQUEST['req']) {
    case 'salvar-area':
        $cAreaCultural->salvar($_REQUEST);
        break;
    case 'excluir-area':
        $cAreaCultural->excluir($_REQUEST['mdeid']);
        break;
    case 'salvar-segmento':
        $cSegmentoCultural->salvar($_REQUEST);
        die;
    case 'excluir-segmento':
        $cSegmentoCultural->excluir($_REQUEST);
        break;
    case 'listar-segmento':
        $lista = $mSegmentoCultural->RecuperaSegmentoPorArea($_REQUEST['mdeid']);
        include_once(APPRAIZ.'/planacomorc/modulos/apoio/segmento-cultural.inc');
        die;

}

include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Área Cultural</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar-area" />
                        <input name="mdeid" id="mdeid" type="hidden" value="<?php echo $mAreaCultural->mdeid; ?>">
                        
                        <?php
                            echo $simec->input('mdecod', 'Código PI', $mAreaCultural->mdecod, ['required','maxlength' => 1, 'class' => 'upper-text']);
                            echo $simec->input('mdedsc', 'Descrição', $mAreaCultural->mdedsc, ['required', 'maxlength' => 250]);
                        ?>
                        
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/area_cultural&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mAreaCultural->mdeid){ ?>
                                    <a href="?modulo=apoio/area_cultural_form&acao=A&req=excluir-area&mdeid=<?php echo $mAreaCultural->mdeid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
        <div class="col-md-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Segmento Cultural</h5>
                </div>
                <?php if($mAreaCultural->mdeid){ ?>
                    <div class="ibox-content">
                        <form id="formulario-segmento" name="formulario-segmento" method="post" class="form-horizontal" action="?modulo=apoio/area_cultural_form&acao=A">
                            <input type="hidden" name="req" id="req" value="salvar-segmento" />
                            <input name="mdeid" id="mdeid" type="hidden" value="<?php echo $mAreaCultural->mdeid; ?>">
                            <input name="neeid" id="neeid" type="hidden" value="<?php echo $mSegmentoCultural->neeid; ?>">

                            <?php
                                echo $simec->input('neecod', 'Código', $mSegmentoCultural->neecod, ['maxlength' => 3, 'class' => 'upper-text', 'required']);
                                echo $simec->input('needsc', 'Descrição', $mSegmentoCultural->needsc, ['maxlength' => 250, 'required']);
                            ?>

                            <div class="form-group">
                                <div class="text-center">
                                    <button class="btn btn-primary" type="button" id="btn-salvar-segmento"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                </div>
                            </div>
                        </form>

                        <div id="lista-segmento" class="listagem-segmento-cultural">
                            <?php
                                include_once(APPRAIZ.'/planacomorc/modulos/apoio/segmento-cultural.inc');
                            ?>
                        </div>
                    </div>

                <?php } else { ?>
                <div class="ibox-content">
                    <div class="alert alert-danger">Nenhum registro cadastrado</div>
                </div>
                <?php } ?>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">
    $(function(){

        $("#btn-salvar-segmento").click(function(){
            var neecod = $("#neecod").val();
            var needsc = $("#needsc").val();
            if (neecod.trim().length==0){
                swal({
                    title: 'Atenção',
                    text: 'Favor informar o código!',
                    type: "error"
                });                
                return false;
            }
            if (needsc.trim().length==0){
                swal({
                    title: 'Atenção',
                    text: 'Favor informar a descrição!',
                    type: "error"
                });                
                return false;
            }
            options = {
                success: function() {
                    // Carregar a listagem
                    jQuery("#lista-segmento").load('?modulo=apoio/area_cultural_form&acao=A&req=listar-segmento&mdeid='+$('#mdeid').val());

                    // Zerar o formulário
                    LimparCamposSegmento();
                }
            }

            jQuery("#formulario-segmento").ajaxForm(options).submit();
            return false;
        });
    });

    function LimparCamposSegmento(){
        $("#neecod").val('');
        $("#needsc").val('');
    }
</script>