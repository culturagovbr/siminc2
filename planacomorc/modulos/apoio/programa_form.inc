<?php

$cPrograma = new Planacomorc_Controller_Programa();
switch ($_REQUEST['req']) {
    case 'salvar':
        $cPrograma->salvar($_REQUEST);
        die;
    case 'excluir':
        $cPrograma->excluir($_REQUEST['prgid']);
        die;
}

$mPrograma = new Planacomorc_Model_Programa($_REQUEST['prgid']);

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
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="prgid" id="prgid" type="hidden" value="<?= $mPrograma->prgid; ?>">
                        
                        <?php
                            echo $simec->input('prgcod', 'Código', $mPrograma->prgcod, ['required','maxlength' => 4, 'class' => 'inteiro']);
                            echo $simec->input('prgdsc', 'Descrição', $mPrograma->prgdsc, ['required','maxlength' => 200]);
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/programa&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mPrograma->prgid){ ?>
                                    <a href="?modulo=apoio/programa_form&acao=A&req=excluir&prgid=<?php echo $mPrograma->prgid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>
                    </form>

                </div>
            </div>
        </div>
    </div>
</div>