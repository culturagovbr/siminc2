<?php

$cObjetivoEstrategico = new Planacomorc_Controller_ObjetivoEstrategico();
$cMetaEstrategica = new Planacomorc_Controller_MetaEstrategica();
$cDiretrizEstrategica = new Planacomorc_Controller_DiretrizEstrategica();

switch ($_REQUEST['req']) {
    case 'salvar':
        $cObjetivoEstrategico->salvar($_REQUEST);
        die;
    case 'excluir':
        $cObjetivoEstrategico->excluir($_REQUEST['obeid']);
        die;
    case 'formulario-meta-estrategica':
        $cObjetivoEstrategico->montarFormularioMetaEstrategica($_REQUEST['obeid'], $_REQUEST['meeid']);
        die;
    case 'listar-meta-estrategica':
        $cObjetivoEstrategico->montarListagemMetaEstrategica($_REQUEST['obeid']);
        die;
    case 'salvar-meta-estrategica':
        $_REQUEST['meecod'] = utf8_decode($_REQUEST['meecod']);
        $_REQUEST['meenome'] = utf8_decode($_REQUEST['meenome']);
        $cMetaEstrategica->salvar($_REQUEST);
        die;
    case 'excluir-meta-estrategica':
        $cMetaEstrategica->excluir($_REQUEST['meeid']);
        die;
    case 'formulario-diretriz-estrategica':
        $cObjetivoEstrategico->montarFormularioDiretrizEstrategica($_REQUEST['obeid'], $_REQUEST['dieid']);
        die;
    case 'listar-diretriz-estrategica':
        $cObjetivoEstrategico->montarListagemDiretrizEstrategica($_REQUEST['obeid']);
        die;
    case 'salvar-diretriz-estrategica':
        $_REQUEST['diecod'] = utf8_decode($_REQUEST['diecod']);
        $_REQUEST['dienome'] = utf8_decode($_REQUEST['dienome']);
        $cDiretrizEstrategica->salvar($_REQUEST);
        die;
    case 'excluir-diretriz-estrategica':
        $cDiretrizEstrategica->excluir($_REQUEST['dieid']);
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
        <div class="col-lg-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Objetivo Estratégico</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="obeid" id="obeid" type="hidden" value="<?= $mObjetivoEstrategico->obeid; ?>">
                        <?php
                            echo $simec->input('obecod', 'Código', $mObjetivoEstrategico->obecod, ['required', 'class' => 'upper-text', 'maxlength' => 4]);
                            echo $simec->input('obenome', 'Nome', $mObjetivoEstrategico->obenome, ['required', 'maxlength' => 200]);
                        ?>
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" id="btn-salvar" type="submit">
                                    <i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/objetivo_meta_diretriz&acao=A" class="btn btn-warning" id="btnVoltar" type="button">
                                    <i class="fa fa-arrow-left"></i>&nbsp;Voltar
                                </a>
                                <?php if($mObjetivoEstrategico->obeid){ ?>
                                    <a href="?modulo=apoio/objetivo_meta_diretriz_form&acao=A&req=excluir&obeid=<?= $mObjetivoEstrategico->obeid; ?>" class="btn btn-danger link-excluir" id="btnExcluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
        <div class="col-lg-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Meta Estratégica</h5>
                </div>
                <div class="ibox-content">
                    <div id="div_formulario_meta_estrategica">
                        <?php
                        if($mObjetivoEstrategico->obeid){
                            $cObjetivoEstrategico->montarFormularioMetaEstrategica($mObjetivoEstrategico->obeid);
                        }
                        ?>
                    </div>
                    <div id="div_listagem_meta_estrategica">
                        <?php $cObjetivoEstrategico->montarListagemMetaEstrategica($mObjetivoEstrategico->obeid); ?>
                    </div>
                </div>
            </div>
        </div>
        <div class="col-lg-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Diretriz Estratégica</h5>
                </div>
                <div class="ibox-content">
                    <div id="div_formulario_diretriz_estrategica">
                        <?php
                            if($mObjetivoEstrategico->obeid){
                                $cObjetivoEstrategico->montarFormularioDiretrizEstrategica($mObjetivoEstrategico->obeid);
                            }
                        ?>
                    </div>
                    <div id="div_listagem_diretriz_estrategica">
                        <?php $cObjetivoEstrategico->montarListagemDiretrizEstrategica($mObjetivoEstrategico->obeid); ?>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
