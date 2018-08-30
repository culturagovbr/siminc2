<?php

$cProposta = new Proposta_Controller_Proposta();
$mProposta = new Proposta_Model_Proposta($_REQUEST['proid']);
$listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));
switch ($_REQUEST['req']) {
    case 'salvar':
        $cProposta->salvar($_REQUEST);
        die;
    case 'excluir':
        $cProposta->excluir($_REQUEST['proid']);
        die;
    case 'carregar-funcional':
        echo $simec->select('ptrid', 'Funcional', null, (new Monitora_Model_Ptres())->recuperarSqlPorSubunidadeEEnquadramento($_REQUEST['suoid'], $_REQUEST['eqdid']), ['required']);
        die;
    case 'carregar-funcional-filtros':
        echo $simec->select('ptrid', 'Funcional', null, (new Proposta_Model_Proposta())->recuperarSqlFuncional($_REQUEST['suoid'], $_REQUEST['eqdid']));
        die;        
    case 'formulario-detalhe':
        $cProposta->montarFormularioDetalhes($_REQUEST['proid'], $_REQUEST['prdid']);
        die;
    case 'listar-detalhe':
        $cProposta->montarListagemDetalhes($mProposta->proid);
        die;
    case 'salvar-detalhe':
        $cPropostaDetalhe = new Proposta_Controller_PropostaDetalhe();
        $cPropostaDetalhe->salvar($_POST);
        die;
    case 'excluir-detalhe':
        $cPropostaDetalhe = new Proposta_Controller_PropostaDetalhe();
        echo $cPropostaDetalhe->excluir($_REQUEST['prdid']);
        die;
    case 'carregar-rp':
        $rp = (new Monitora_Model_PiEnquadramentoDespesa())->recuperarDescricaoRp($_REQUEST['eqdid']);
        echo '  <div class="form-group">
                    <label class="col-sm-3 control-label">RP: </label>
                    <div class="col-sm-9">
                      <p class="form-control-static">' . $rp . '</p>
                    </div>
                  </div>';
        die;
    case 'recuperar-dados-ptres':
        # Busca dados de produto e unidade de medida da funcional
        $modelPtres = new Monitora_Model_Ptres($_REQUEST['ptrid']);
        $modelPtres->ptrano = (int)$_SESSION['exercicio'];
        $funcional = $modelPtres->recuperarProdutosPorPtres();
        
        # Corrigindo campos de textos pra retornar no formato UTF8
        $funcional['produto_acao'] = utf8_encode($funcional['produto_acao']);
        $funcional['produto_po'] = utf8_encode($funcional['produto_po']);
        $funcional['unidade_po'] = utf8_encode($funcional['unidade_po']);
        $funcional['unidade_acao'] = utf8_encode($funcional['unidade_acao']);

        # Resposta da requisição ajax
        echo json_encode($funcional);
        die;
    case 'recuperar-valores-ptres':
        echo json_encode((new Proposta_Model_PrePlanoInterno())->recuperarValoresPorSubnidadeEPtres($_REQUEST['suoid'], $_REQUEST['ptrid']));
        die;
    case 'recuperar-subunidades-ptres':
        $subunidades = (new Monitora_Model_Ptres($_REQUEST['ptrid']))->recuperarSubunidades();
        echo '  <div class="form-group">
                    <label class="col-sm-3 control-label">Subunidades Vinculadas à Funcional: </label>
                    <div class="col-sm-9">
                      <p class="form-control-static">' . $subunidades . '</p>
                    </div>
                  </div>';
        die;
}

include APPRAIZ . "includes/cabecalho.inc";
?>

<script src="js/principal/proposta_form.js?v=1"></script>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <form id="formulario" name="formulario" method="post" class="form-horizontal">
            <div class="col-md-6">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Dados Gerais</h5>
                    </div>
                    <div class="ibox-content">
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="proid" id="proid" type="hidden" value="<?php echo $mProposta->proid; ?>">
                        <?php
                            echo $simec->select('suoid', 'SubUnidade', $mProposta->suoid, (new Public_Model_SubUnidadeOrcamentaria())->queryComboChaveSuoid((object)array(
                                'exercicio' => $_SESSION['exercicio'],
                                'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)), ['required']);
                            echo $simec->select('eqdid', 'Enquadramento da Despesa', $mProposta->eqdid, (new Monitora_Model_PiEnquadramentoDespesa())->recuperarSqlCombo(), ['required']);
                            echo '<span id="span-rp"></span>';
                            echo '<span id="span-funcional">' . $simec->select('ptrid', 'Funcional', $mProposta->ptrid, (new Monitora_Model_Ptres())->recuperarSqlPorSubunidadeEEnquadramento($mProposta->suoid, $mProposta->eqdid), ['required']) . '</span>';
                            echo '<span id="span-subunidades-ptres"></span>';
                        ?>
                        <div class="form-group ">
                            <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Limites Pré-PI</label>
                            <div class="col-sm-9 col-md-9 col-lg-9 ">
                                <div class="table-responsive">
                                    <table class="table table-bordered table-hover" >
                                        <thead>
                                        <tr class="text-center">
                                            <th width="20%">Limite</th>
                                            <th width="20%">Custeio</th>
                                            <th width="20%">Capital</th>
                                        </tr>
                                        </thead>
                                        <tbody>
                                        <tr>
                                            <td>Valor</td>
                                            <td id="limite_valor_custeio"></td>
                                            <td id="limite_valor_capital"></td>
                                        </tr>
                                        <tr>
                                            <td>Expansão</td>
                                            <td id="limite_expansao_custeio"></td>
                                            <td id="limite_expansao_capital"></td>
                                        </tr>
                                        </tbody>
                                    </table>
                                </div>
                            </div>
                            <div style="clear:both"></div>
                        </div>
                        <div class="form-group ">
                            <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Quantidades: <span class="campo-obrigatorio" title="Campo obrigatório">*</span></label>
                            <div class="col-sm-9 col-md-9 col-lg-9 ">
                                <div class="table-responsive">
                                    <table class="table table-bordered table-hover" >
                                        <thead>
                                        <tr class="text-center">
                                            <th width="20%">Nível</th>
                                            <th width="20%">Quantidade Física</th>
                                            <th>Produto</th>
                                            <th width="20%">Unidade</th>

                                        </tr>
                                        </thead>
                                        <tbody>
                                        <tr>
                                            <td>Ação</td>
                                            <td>
                                                <input name="locquantidadeproposta" id="locquantidadeproposta" type="text" value="" class="form-control inteiro " required="required">
                                            </td>
                                            <td id="produto_acao"></td>
                                            <td id="unidade_acao"></td>
                                            
                                        </tr>
                                        <tr>
                                            <td>PO</td>
                                            <td>
                                                <input name="proquantidade" id="proquantidade" type="text" value="<?php echo number_format($mProposta->proquantidade, 0, ',', '.'); ?>" class="form-control inteiro " required="required">
                                            </td>
                                            <td id="produto_po"></td>
                                            <td id="unidade_po"></td>
                                        </tr>
                                        </tbody>
                                    </table>
                                </div>

                            </div>
                            <div style="clear:both"></div>
                        </div>
                        <?php
                            echo $simec->textarea('projustificativa', 'Justificativa', $mProposta->projustificativa, ['cols'=>60, 'rows'=>6]);
                        ?>
                    </div>
                </div>
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Expansão</h5>
                    </div>
                    <div class="ibox-content">
                        <?php
                            echo $simec->input('proquantidadeexpansao', 'Quantidade Expansão PO', $mProposta->proquantidadeexpansao, ['class'=>'inteiro']);
                            echo $simec->textarea('projustificativaexpansao', 'Justificativa Expansão', $mProposta->projustificativaexpansao, ['cols'=>60, 'rows'=>6]);
                        ?>
                    </div>
                </div>
                <div class="form-group">
                    <div class="text-center">
                        <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                        <a href="?modulo=principal/proposta&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                        <?php if($mProposta->proid){ ?>
                            <a href="?modulo=principal/proposta_form&acao=A&req=excluir&proid=<?php echo $mProposta->proid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                        <?php } ?>
                    </div>
                </div>
            </div>
        </form>

        <div class="col-md-5">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Financeiros</h5>
                </div>
                <div class="ibox-content" id="">
                    <div id="div_formulario_detalhe">
                        <?php
                            if($mProposta->proid){
                                $cProposta->montarFormularioDetalhes($mProposta->proid);
                            }
                        ?>
                    </div>
                    <div id="div_listagem_detalhe">
                        <?php $cProposta->montarListagemDetalhes($mProposta->proid); ?>
                    </div>
                </div>
            </div>
        </div>

    </div>
</div>

<script>

    $(function(){

        initPropostaForm();

    });

</script>