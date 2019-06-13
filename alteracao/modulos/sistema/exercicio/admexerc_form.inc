<?php
    $cExercicio = new Public_Controller_ProgramacaoExercicio();

    switch ($_REQUEST['req']) {
        case 'salvar':
            $cExercicio->salvar($_REQUEST);
        die;
        case 'excluir':
            $cExercicio->excluir($_REQUEST['prsano']);
        die;
    }
    $mExercicio = new Public_Model_ProgramacaoExercicio($_SESSION['sisdiretorio'], $_REQUEST['prsano']);

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
                        <input name="prsano" id="prsano" type="hidden" value="<?= $mExercicio->prsano; ?>">

                        <?php
                            echo $simec->input('prsano', 'Ano do exercício', $mExercicio->prsano, ['required', 'class' => 'inteiro', 'maxlength' => 4]);
                            echo $simec->data('prsdata_inicial', 'Data do Início', $mExercicio->prsdata_inicial, ['required']);
                            echo $simec->data('prsdata_termino', 'Data do Fim', $mExercicio->prsdata_termino, ['required']);
                            echo $simec->radio('prsexerccorrente', 'Exercício corrente ?', $mExercicio->prsexerccorrente, ['t' => 'Sim', 'f' => 'Não'], ['required']);
                        ?>
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=sistema/exercicio/admexerc&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mExercicio->prsano){ ?>
                                    <a href="?modulo=sistema/exercicio/admexerc_form&acao=A&req=excluir&prsano=<?= $mExercicio->prsano; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>
