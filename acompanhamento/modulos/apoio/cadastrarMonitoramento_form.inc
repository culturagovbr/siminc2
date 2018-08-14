<?php

$cMonitoramento = new Acompanhamento_Controller_CadastrarMonitoramento();
$mMonitoramento = new Acompanhamento_Model_Tipo($_REQUEST['tipid']);

switch ($_REQUEST['req']) {
    case 'salvar-monitora':
        $cMonitoramento->salvar($_REQUEST);
        break;
    case 'excluir-monitora':
        $cMonitoramento->excluir($_REQUEST['tipid']);
        break;
    case 'validar-monitora':
        $resposta = $cMonitoramento->validarmonitora();
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
                        <input type="hidden" name="req" id="req" value="salvar-monitora" />
                        <input name="tipid" id="tipid" type="hidden" value="<?php echo $mMonitoramento->tipid; ?>">
                        <div id="codigo-monitoramento">
                        <?php
                        echo $simec->input('tipdsc', 'Descrição', $mMonitoramento->tipdsc, ['required', 'maxlength' => 200]);
                        ?>
                        </div>
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/cadastrarMonitoramento&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mMonitoramento->tipid){ ?>
                                    <a href="?modulo=apoio/cadastrarMonitoramento_form&acao=A&req=excluir-monitora&tipid=<?php echo $mMonitoramento->tipid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
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
    $(function () {
        $('#codigo-monitoramento').change(function () {

            $.ajax({
                url: 'acompanhamento.php?modulo=apoio/cadastrarMonitoramento&acao=A&req=validar-monitora&tipid=' + $('#tipid').val() + '&monicod=' + $('#monicod').val(),
                dataType: 'json',
                success: function (response) {
                    if (response.type == 'error') {
                        swal({
                            title: response.title,
                            text: response.text,
                            type: response.type
                        });
                        $('#monicod').val('');
                    }
                }
            });
        });

    });

</script>
