<?php

$cManutencaoItem = new Public_Controller_ManutencaoItem();
switch ($_REQUEST['req']) {
	case 'salvar':
		$cManutencaoItem->salvar($_REQUEST);
		die;
	case 'excluir':
		$cManutencaoItem->excluir($_REQUEST['maiid']);
		die;
	case 'formulario-subitem':
		$cManutencaoItem->montarFormulario($_REQUEST['maiid'], $_REQUEST['masid']);
		die;
	case 'listar-subitem':
		$cManutencaoItem->montarListagem($_REQUEST['maiid']);
		die;
	case 'salvar-subitem':
		$cManutencaoSubItem = new Public_Controller_ManutencaoSubItem();
		$cManutencaoSubItem->salvar($_REQUEST);
		die;
	case 'excluir-subitem':
		$cManutencaoSubItem = new Public_Controller_ManutencaoSubItem();
		$cManutencaoSubItem->excluir($_REQUEST['masid']);
		die;
}

$mManutencaoItem = new Planacomorc_Model_ManutencaoItem($_REQUEST['maiid']);
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
                    <h5>Item</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="maiid" id="maiid" type="hidden" value="<?php echo $mManutencaoItem->maiid; ?>">

						<?php
                            echo $simec->input('mainome', 'Nome', $mManutencaoItem->mainome, ['required','maxlength' => 200]);
                            echo $simec->input('maidescricao', 'Descrição', $mManutencaoItem->maidescricao, ['maxlength' => 500]);
                            echo $simec->select('eqdid', 'Enquadramento da Despesa', $mManutencaoItem->eqdid, $mEnquadramentoDespesa = (new Monitora_Model_PiEnquadramentoDespesa)->recuperarSqlCombo(null, ["eqdcod NOT IN('C', 'E', 'F', 'I', 'N', 'A')"]));
						?>
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" id="btn-salvar" type="submit"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/item-manutencao&acao=A" class="btn btn-warning" id="btnVoltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
								<?php if($mManutencaoItem->maiid){ ?>
                                    <a href="?modulo=apoio/item-manutencao-form&acao=A&req=excluir&maiid=<?php echo $mManutencaoItem->maiid; ?>" class="btn btn-danger link-excluir" id="btnExcluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
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
                    <h5>Subitem</h5>
                </div>
                <div class="ibox-content">
                    <div id="div_formulario_subitem">
						<?php
                            if($mManutencaoItem->maiid){
                                $cManutencaoItem->montarFormulario($mManutencaoItem->maiid);
                            }
						?>
                    </div>
                    <div id="div_listagem_subitem">
						<?php $cManutencaoItem->montarListagem($mManutencaoItem->maiid); ?>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
