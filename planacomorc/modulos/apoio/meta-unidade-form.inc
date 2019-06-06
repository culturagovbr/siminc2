<?php

$cMetaUnidade = new Planacomorc_Controller_MetaUnidade();
$mMetaUnidade = new Planacomorc_Model_MetaUnidade($_REQUEST['meuid']);

switch ($_REQUEST['req']) {
    case 'salvar':
        $cMetaUnidade->salvar($_REQUEST);
        break;
    case 'excluir':
        $cMetaUnidade->excluir($_REQUEST['meuid']);
        break;
}

include APPRAIZ . "includes/cabecalho.inc";
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
                    <h5>Meta Unidade</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="meuid" id="meuid" type="hidden" value="<?= $mMetaUnidade->meuid; ?>">

                        <?php
                            echo $simec->textarea('meunome', 'Nome', $mMetaUnidade->meunome, ['required','maxlength' => 1000, 'cols' => 50, 'rows' => 6]);
                            echo $simec->select('obeid', 'Objetivo Estratégico',  $mMetaUnidade->obeid, (new Planacomorc_Model_ObjetivoEstrategico())->recuperarSqlCombo());
                            echo $simec->select('dimeid', 'Dimensão Estratégica',  $mMetaUnidade->dimeid, (new Planacomorc_Model_DimensaoEstrategica())->recuperarSqlCombo());
                            echo $simec->select('meeid', 'Meta Estratégica',  $mMetaUnidade->meeid, (new Planacomorc_Model_MetaEstrategica())->recuperarSqlCombo());
                        # Traz Sim por default
                            $radio = $mMetaUnidade->meuquantificavel ? $mMetaUnidade->meuquantificavel : 't';
                            echo $simec->radio('meuquantificavel', 'Quantificável', $radio, ['t' => 'Sim', 'f' => 'Não'], ['required']);
                            echo $simec->moedaInteiro('meuquantidade', 'Quantidade', $mMetaUnidade->meuquantidade, ['maxlength' => 9, 'class' => 'inteiro']);
                            # Traz Não por default
                            $radio = $mMetaUnidade->meuprioritario ? $mMetaUnidade->meuprioritario : 'f';
                            echo $simec->radio('meuprioritario', 'Prioritário', $radio, ['t' => 'Sim', 'f' => 'Não'], ['required']);
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/meta-unidade&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mMetaUnidade->meuid){ ?>
                                    <a href="?modulo=apoio/meta-unidade-form&acao=A&req=excluir&meuid=<?php echo $mMetaUnidade->meuid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>
