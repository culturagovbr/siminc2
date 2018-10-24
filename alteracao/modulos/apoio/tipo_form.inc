<?php

$cTipo = new Alteracao_Controller_Tipo();
$mTipo = new Alteracao_Model_Tipo($_REQUEST['tpaid']);

switch ($_REQUEST['req']) {
    case 'salvar-tipo':
        $cTipo->salvar($_REQUEST);
    break;
    case 'excluir-tipo':
        $cTipo->excluir($_REQUEST['tpaid']);
    break;
    case 'valida-tipo':
        $resposta = $cTipo->validaTipo();
        echo $resposta;
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
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar-tipo" />
                        <input name="tpaid" id="tpaid" type="hidden" value="<?= $mTipo->tpaid; ?>">

                        <div id="codigo-tipo">
                            <?= $simec->input('tpacod', 'Código', $mTipo->tpacod, ['required' ,'maxlength' => 5]); ?>
                        </div>
                        <?php
                            echo $simec->input('tpadsc', 'Descrição', $mTipo->tpadsc, ['required' ,'maxlength' => 125]);
                            echo $simec->input('tpaobs', 'Observação', $mTipo->tpaobs, ['maxlength' => 125]);
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/tipo&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mTipo->tpaid){ ?>
                                    <a href="?modulo=apoio/tipo_form&acao=A&req=excluir-tipo&tpaid=<?= $mTipo->tpaid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>

<script>
    $(function(){
        // $('#codigo-tipo').change(function () {
        //
        //     $.ajax({
        //         url: 'proposta.php?modulo=apoio/fonte_form&acao=A&req=valida-tipo&tpacod='+ $('#tpacod').val()+ '&tpaid='+ $('#tpaid').val(),
        //         dataType: 'json',
        //         success: function(response){
        //             if(response.type == 'error'){
        //                 swal({
        //                     title: response.title,
        //                     text: response.text,
        //                     type: response.type
        //                 });
        //                 $('#tpacod').val('');
        //             }
        //         }
        //     });
        // });
    });
</script>