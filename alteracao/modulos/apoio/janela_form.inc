<?php

    $mJanela = new Alteracao_Model_Janela($_REQUEST['janid']);
    $cJanela = new Alteracao_Controller_Janela();

    switch ($_REQUEST['req']) {
        case 'salvar':
            $cJanela->salvar($_REQUEST);
        break;
        case 'excluir':
            $cJanela->excluir($_REQUEST['janid']);
        break;
    }

    include APPRAIZ. "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?= $titulo_modulo; ?></h2>
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
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="janid" id="janid" type="hidden" value="<?= $mJanela->janid; ?>">

                        <?php
                            echo $simec->input('jannome', 'Janela', $mJanela->jannome, ['required', 'maxlength' => 125]);
                            echo $simec->data('janinicio', 'Data Início', $mJanela->janinicio, ['required']);
                            echo $simec->data('janfim', 'Data Fim', $mJanela->janfim, ['required']);
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/janela&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if( $mJanela->janid ):  ?>
                                    <a href="?modulo=apoio/janela_form&acao=A&req=excluir&janid=<?= $mJanela->janid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button">
                                        <i class="fa fa-close"></i>&nbsp;Excluir
                                    </a>
                                <? endif;?>
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>
