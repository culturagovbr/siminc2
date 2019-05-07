<?php

$cObjetivoEstrategico = new Planacomorc_Controller_ObjetivoEstrategico();

switch ($_REQUEST['req']) {
    case 'salvar':
        $cObjetivoEstrategico->salvar($_REQUEST);
        die;
    case 'excluir':
        $cObjetivoEstrategico->excluir($_REQUEST['obeid']);
        die;
}

$mObjetivoEstrategico = new Planacomorc_Model_ObjetivoEstrategico($_REQUEST['obeid']);
include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?= $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-lg-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Objetivo Estratégico</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="obeid" id="obeid" type="hidden" value="<?= $mObjetivoEstrategico->obeid; ?>">
                        <?php
                            echo $simec->input('obenome', 'Nome', $mObjetivoEstrategico->obenome, ['required', 'maxlength' => 1000]);
                        ?>
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" id="btn-salvar" type="submit">
                                    <i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/objetivo-estrategico&acao=A" class="btn btn-warning" id="btnVoltar" type="button">
                                    <i class="fa fa-arrow-left"></i>&nbsp;Voltar
                                </a>
                                <?php if($mObjetivoEstrategico->obeid){ ?>
                                    <a href="?modulo=apoio/objetivo-estrategico-form&acao=A&req=excluir&obeid=<?= $mObjetivoEstrategico->obeid; ?>" class="btn btn-danger link-excluir" id="btnExcluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>
