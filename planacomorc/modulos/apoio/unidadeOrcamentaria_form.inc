<?php

$cUnidadeOrcamentaria = new Public_Controller_UnidadeOrcamentaria();
switch ($_REQUEST['req']) {
    case 'salvar':
        $cUnidadeOrcamentaria->salvar($_REQUEST);
        die;
    case 'excluir':
    $cUnidadeOrcamentaria->excluir($_REQUEST['unoid']);
    die;
    case 'formulario-sub-unidade':
        $cUnidadeOrcamentaria->montarFormulario($_REQUEST['unoid'], $_REQUEST['suoid']);
    die;
    case 'listar-sub-unidade':
        $cUnidadeOrcamentaria->montarListagem($_REQUEST['unoid']);
    die;
    case 'salvar-sub-unidade':
        $cSubUnidade = new Public_Controller_SubUnidadeOrcamentaria();
        $cSubUnidade->salvar($_REQUEST);
    die;
    case 'excluir-sub-unidade':
        $cSubUnidade = new Public_Controller_SubUnidadeOrcamentaria();
        $cSubUnidade->excluir($_REQUEST);
    die;
}

$mUnidadeOrcamentaria = new Public_Model_UnidadeOrcamentaria($_REQUEST['unoid']);
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
                    <h5>Unidade Orçamentaria</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="unoid" id="unoid" type="hidden" value="<?php echo $mUnidadeOrcamentaria->unoid; ?>">

                        <?php
                            echo $simec->input('unocod', 'Código UO', $mUnidadeOrcamentaria->unocod, ['required','class'=>"inteiro" ,'maxlength' => 5]);
                            echo $simec->input('unonome', 'Nome UO', $mUnidadeOrcamentaria->unonome, ['required','maxlength' => 250]);
                            echo $simec->input('unosigla', 'Sigla UO', $mUnidadeOrcamentaria->unosigla, ['required','maxlength' => 20]);
                            echo $simec->radio('unofundo', 'FNC', $mUnidadeOrcamentaria->unofundo, ['t'=>'Sim', 'f'=>'Não']);
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" id="btn-salvar" type="submit"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/unidadeOrcamentaria&acao=A" class="btn btn-warning" id="btnVoltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mUnidadeOrcamentaria->unoid){ ?>
                                    <a href="?modulo=apoio/unidadeOrcamentaria_form&acao=A&req=excluir&unoid=<?php echo $mUnidadeOrcamentaria->unoid; ?>" class="btn btn-danger link-excluir" id="btnExcluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
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
                    <h5>Sub-Unidade Orçamentaria</h5>
                </div>
                <div class="ibox-content">
                  <div id="div_formulario_sub_unidade">
                      <?php
                          if($mUnidadeOrcamentaria->unoid){
                              $cUnidadeOrcamentaria->montarFormulario($mUnidadeOrcamentaria->unoid);
                          }
                      ?>
                  </div>
                  <div id="div_listagem_sub_unidade">
                      <?php $cUnidadeOrcamentaria->montarListagem($mUnidadeOrcamentaria->unoid); ?>
                  </div>
                </div>
            </div>
        </div>
    </div>
</div>
