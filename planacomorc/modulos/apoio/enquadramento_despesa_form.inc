<?php

$cEnquadramento = new Monitora_Controller_PiEnquadramentoDespesa();
switch ($_REQUEST['req']) {
    case 'salvar':
        $cEnquadramento->salvar($_REQUEST);
        die;
    case 'excluir':
        $cEnquadramento->excluir($_REQUEST['eqdid']);
        die;
}

$mEnquadramento = new Monitora_Model_PiEnquadramentoDespesa($_REQUEST['eqdid']);
$lista_rp = $mEnquadramento->recuperarRP($_REQUEST['eqdid']);

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
                        <input name="eqdid" id="eqdid" type="hidden" value="<?php echo $mEnquadramento->eqdid; ?>">

                        <?php
                          echo $simec->input('eqdcod', 'Código', $mEnquadramento->eqdcod, ['required', 'maxlength' => 2]);
                          echo $simec->input('eqddsc', 'Descrição', $mEnquadramento->eqddsc, ['required', 'maxlength' => 250]);
                          echo $simec->radio('eqdstreduzido', 'Reduzido', $mEnquadramento->eqdstreduzido, ['S'=>'Sim', 'N'=>'Não'], ['required']);
                          echo $simec->select('lista_rp', 'Resultado Primario', $lista_rp, (new Public_Model_IdentResultPrimario())->recuperarSqlCombo("irpcod, irpcod || ' - ' || irpdsc", ["irpstatus = 'A'"], 'descricao'), array('multiple' => 'true'));
                          echo $simec->select('tpdid', 'Tipo de Detalhamento/Grupo', $mEnquadramento->tpdid, (new Proposta_Model_TipoDetalhamento())->recuperarSqlCombo("tpdcod || ' - ' || tpdnome", ["tpdstatus = 'A'"], "tpdcod"));
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/enquadramento_despesa&acao=A" class="btn btn-warning" id="btnVoltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mEnquadramento->eqdid){ ?>
                                    <a href="?modulo=apoio/enquadramento_despesa_form&acao=A&req=excluir&eqdid=<?php echo $mEnquadramento->eqdid; ?>" class="btn btn-danger link-excluir" id="btnExcluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>
