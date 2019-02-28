<?php

    $cAcao = new Monitora_Controller_Acao();
    switch ($_REQUEST['req']) {
        case 'salvar':
            $cAcao->salvar($_REQUEST);
            die;
        case 'excluir':
            $cAcao->excluir($_REQUEST['acaid']);
            die;
    }

    $mAcao = new Monitora_Model_Acao($_REQUEST['acaid']);
    $mUnidadeOrcamentaria = new Public_Model_UnidadeOrcamentaria();
    $mPrograma = new Monitora_Model_Programa();
    $mLocalizador = new Public_Model_Localizador();
    $mObjetivo = new Public_Model_ObjetivoPpa();

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
                        <input name="acaid" id="acaid" type="hidden" value="<?php echo $mAcao->acaid; ?>">

                        <?php
                            echo $simec->select('unicod', 'Unidade Orçamentária', $mAcao->unicod, $mUnidadeOrcamentaria->recuperarSqlComboUnocod(['unocod', 'unonome'], null, 'unocod'), ['required']);
                            echo $simec->input('acacod', 'Código', $mAcao->acacod, ['required', 'maxlength' => 4]);
                            echo $simec->input('acatitulo', 'Nome', $mAcao->acatitulo, ['required', 'maxlength' => 255]);
                            echo $simec->select('prgcod', 'Programa', $mAcao->prgcod, $mPrograma->recuperaSqlComboPrgcod((object)['exercicio' => $_SESSION['exercicio']]));
                            echo $simec->select('loccod', 'Localizador', $mAcao->loccod, $mLocalizador->recuperarSqlCombo(['codigolocalizador', 'descricao'], ["exercicio = '". (int)$_SESSION['exercicio']. "'"], 'codigolocalizador'), ['required']);
                            echo $simec->select('acaobjetivocod', 'Objetivo', $mAcao->acaobjetivocod, $mObjetivo->recuperarSqlComboPorOppcod());
                            echo $simec->textarea('acanomeproduto', 'Nome do Produto', $mAcao->acanomeproduto, ['maxlength' => 200, 'cols'=>30, 'rows'=>3]);
                            echo $simec->textarea('acanomeunidade', 'Nome da Unidade de Medida', $mAcao->acanomeunidade, ['maxlength' => 200, 'cols'=>30, 'rows'=>3]);
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/acao&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mAcao->acaid){ ?>
                                    <a href="?modulo=apoio/acao_form&acao=A&req=excluir&acaid=<?php echo $mAcao->acaid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>
