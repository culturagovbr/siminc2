<?php

require_once APPRAIZ . 'includes/workflow.php';
include_once '_funcoes.php';

$listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));

$cPrePlanoInterno = new Proposta_Controller_PrePlanoInterno();
$mPrePlanoInterno = new Proposta_Model_PrePlanoInterno($_REQUEST['pliid']);
$aLocalizacoes = $mPrePlanoInterno->recuperarLocalizacao();

$limites = array('limite_unidade' => NULL, 'disponivel_unidade' => NULL);

switch($_REQUEST['req']) {
    case 'salvar':
        $cPrePlanoInterno->salvar($_REQUEST);
        die;
    case 'excluir':
        $cPrePlanoInterno->excluir($_REQUEST['pliid']);
        die;
    case 'carregar-item':
        echo $simec->select('maiid', 'Item', null, (new Planacomorc_Model_ManutencaoItem())->recuperarSqlCombo(null, ["eqdid = " . (int) $_REQUEST['eqdid']]));
        die;
    case 'carregar-subitem':
        echo $simec->select('masid', 'SubItem', null, (new Planacomorc_Model_ManutencaoSubItem())->recuperarSqlCombo(null, ["maiid = " . (int) $_REQUEST['maiid'] ]));
        die;
    case 'carregar-funcional':
        echo $simec->select('ptrid', 'Funcional', null, (new Monitora_Model_Ptres())->recuperarSqlPorSubunidadeEEnquadramento($_REQUEST['suoid'], $_REQUEST['eqdid']), ['required']);
        die;
    case 'carregar-segmento':
        echo $simec->select('secid', 'Segmento Cultural', null, (new Monitora_Model_PiNivelEtapaEnsino())->recuperarSqlCombo(null, ['arceid = ' . (int)$_REQUEST['arceid']]), ['required']);
        die;
    case 'carregar-metapnc':
        echo $simec->select('mpnid', 'Meta PNC', null, (new Public_Model_MetaPnc())->recuperarSqlCombo(null, ['suoid'=>$_REQUEST['suoid']]), ['required']);
        die;
    case 'carregar-indicadorpnc':
        echo $simec->select('ipnid', 'Indicador PNC', null, (new Public_Model_IndicadorPnc())->recuperarSqlCombo(null, ['mpnid'=>(int)$_REQUEST['mpnid']]), ['required']);
        die;
    case 'recuperar-objetivoppa':
        echo (new Monitora_Model_Ptres($_REQUEST['ptrid']))->recuperarObjetivoPorPtres();
        die;
    case 'recuperar-valores-ptres':
        echo (new Monitora_Model_Ptres($_REQUEST['ptrid']))->recuperarValoresPrePi();
        die;
    case 'carregar-metappa':
        echo $simec->select('mppid', 'Metas PPA', null, (new Public_Model_MetaPpa())->recuperarSqlCombo(null, ['suoid'=> (int)$_REQUEST['suoid'], 'oppid'=> (int)$_REQUEST['oppid']]), ['required']);
        die;
    case 'carregar-iniciativappa':
        echo $simec->select('ippid', 'Iniciativa PPA', null, (new Public_Model_IniciativaPpa())->recuperarSqlCombo(null, ['oppid'=>(int)$_REQUEST['oppid']]), []);
        die;
    case 'recuperar-limite':
        $resultadoLimites = (new Proposta_Model_Subunidaderplimite())
            ->recuperarLimitePorSuoid( (object)array(
                'noPliid' => $_REQUEST['pliid'],
                'suoid' => $_REQUEST['suoid'],
                'eqdid' => $_REQUEST['eqdid']));
        if($resultadoLimites){
            $resultadoLimites['disponivel_unidade'] = (int)$resultadoLimites['limite_unidade'] - (int)$resultadoLimites['total_pre_pi'];
            $limites = array_map("utf8_encode", $resultadoLimites);
        }
        echo json_encode($limites);
        die;
    case 'proposta_modal':
        include 'preplanointerno_modal.inc';
        die;
    case 'importar-pi':
        $mPrePlanoInterno->importarPI((int)$_REQUEST['pliid']);
        $aLocalizacoes = $mPrePlanoInterno->recuperarLocalizacaoPi((int)$_REQUEST['pliid']);
}

$perfis = pegaPerfilGeral();

$estadoAtual = $mPrePlanoInterno->gerarDocid(WF_TPDID_PREPI, 'Pré-PI');
$listaEqdidReduzido = simec_json_encode(buscarListaCodigoEnquadramentoReduzido((int)$_SESSION['exercicio']));
$codigoProdutoNaoAplica = buscarCodigoProdutoNaoAplica((int)$_SESSION['exercicio']);

if($mPrePlanoInterno->suoid){
    $resultadoLimites = (new Proposta_Model_Subunidaderplimite())
        ->recuperarLimitePorSuoid( (object)array(
            'noPliid' => $mPrePlanoInterno->pliid,
            'suoid' => $mPrePlanoInterno->suoid,
            'eqdid' => $mPrePlanoInterno->eqdid));
    if($resultadoLimites){
        $resultadoLimites['disponivel_unidade'] = (int)$resultadoLimites['limite_unidade'] - (int)$resultadoLimites['total_pre_pi'];
    }
    $limites = $resultadoLimites? $resultadoLimites: array('limite_unidade' => NULL, 'disponivel_unidade' => NULL);
}

include APPRAIZ . "includes/cabecalho.inc";
?>

<script src="js/principal/preplanointerno_form.js?v=4"></script>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-5">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
    <?php if(!$mPrePlanoInterno->pliid): ?>
        <div class="col-lg-7">
            <div style="padding-top: 10px;" class="text-right">
                <a class="btn btn-danger" href="#" id="importar-pi-btn"> Importar de PI <?php echo date('Y'); ?></a>
            </div>
        </div>
    <?php endif; ?>
</div>

<div class="wrapper wrapper-content animated fadeInRight">

    <form id="formulario" name="formulario" method="post" class="form-horizontal">
        <input type="hidden" name="req" id="req" value="salvar" />
        <input name="pliid" id="pliid" type="hidden" value="<?php echo $mPrePlanoInterno->pliid; ?>">

        <div class="row">
            <div class="col-md-6">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Dados do Pré-Plano Interno</h5>
                    </div>
                    <div class="ibox-content">
                        <?php
                        echo $simec->input('plititulo', 'Título', $mPrePlanoInterno->plititulo, ['required', 'maxlength' => 250]);
                        echo $simec->textarea('plidsc', 'Descrição / Finalidade', $mPrePlanoInterno->plidsc, ['required', 'maxlength' => 1000, 'cols'=>60, 'rows'=>6]);
                        ?>
                    </div>
                </div>

                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Produto PI</h5>
                    </div>
                    <div class="ibox-content">
                        <?php
                            echo $simec->select('pprid', 'Produto', $mPrePlanoInterno->pprid, (new Monitora_Model_PiProduto())->recuperarSqlCombo(), ['required']);
                            echo '<span id="span_unidade_medida">'. $simec->select('pumid', 'Unidade de Medida', $mPrePlanoInterno->pumid, (new Monitora_Model_PiUnidadeMedida())->recuperarSqlCombo(), ['required']). '</span>';
                            echo '<span id="span_quantidade">'. $simec->input('pliquantidade', 'Quantidade', $mPrePlanoInterno->pliquantidade? (int)$mPrePlanoInterno->pliquantidade: NULL, ['required', 'class'=>'inteiro']). '</span>';
                        ?>
                    </div>
                </div>

                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Custeio e Capital</h5>
                    </div>
                    <div class="ibox-content">

                        <table cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                            <tr>
                                <td style="text-align: left;"></td>
                                <td colspan="3" style="text-align: center;">
                                    <b>Total (Custeio e Capital)</b>
                                </td>
                            </tr>
                            <tr style="height: 30px;" class="tr_limite_autorizado_unidade">
                                <td style="text-align: left;">Limite RP da Unidade: </td>
                                <td colspan="3" id="td_autorizado_sub_unidade" style="text-align: center;"><?php echo number_format((int)$limites['limite_unidade'], 0, ',', '.'); ?></td>
                            </tr>
                            <tr style="height: 30px;" class="tr_limite_disponivel_unidade">
                                <td style="text-align: left;">
                                    Limite RP disponível na Unidade:
                                    <input id="input_bd_disponivel_unidade_bd" name="input_bd_disponivel_unidade_bd" type="hidden" value="<?php echo (int)$limites['disponivel_unidade']; ?>" />
                                </td>
                                <td colspan="3" id="td_disponivel_sub_unidade" style="text-align: center;" ><?php echo number_format(((int)$limites['disponivel_unidade'] -((int)$mPrePlanoInterno->plivalorcusteio + (int)$mPrePlanoInterno->plivalorcapital)), 0, ',', '.'); ?></td>
                            </tr>
                            <tr>
                                <td style="text-align: left;"></td>
                                <td style="text-align: center;"><b>Custeio</b></td>
                                <td style="text-align: center;"><b>Capital</b></td>
                            </tr>
                            <tr>
                                <td style="text-align: left;">
                                    Total Cadastrado na Funcional:
                                </td>
                                <td style="text-align: center;" id="td_disponivel_funcional_custeio" >0</td>
                                <td style="text-align: center;" id="td_disponivel_funcional_capital" >0</td>
                            </tr>
                            <tr>
                                <td id="" style="text-align: left;">Valor do Projeto: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></td>
                                <td style="text-align: center;">
                                    <input name="plivalorcusteio" id="plivalorcusteio" type="text" value="<?php echo number_format($mPrePlanoInterno->plivalorcusteio, 0, ',', '.'); ?>" class="form-control valorPlanoInterno moedaInteiro " required="required">
                                </td>
                                <td align="center">
                                    <input name="plivalorcapital" id="plivalorcapital" type="text" value="<?php echo number_format($mPrePlanoInterno->plivalorcapital, 0, ',', '.'); ?>" class="form-control valorPlanoInterno moedaInteiro " required="required">
                                </td>
                            </tr>
                        </table>
                    </div>
                </div>
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Solicitação de Valor Adicional</h5>
                    </div>
                    <div class="ibox-content">
                        <?php
                            echo $simec->input('pliquantidadeadicional', 'Quantidade', $mPrePlanoInterno->pliquantidadeadicional, ['class'=>'inteiro']);
                            echo $simec->moedaInteiro('plivalorcusteioadicional', 'Valor Custeio', $mPrePlanoInterno->plivalorcusteioadicional);
                            echo $simec->moedaInteiro('plivalorcapitaladicional', 'Valor Capital', $mPrePlanoInterno->plivalorcapitaladicional);
                            echo $simec->textarea('plijustificativaadicional', 'Justificativa', $mPrePlanoInterno->plijustificativaadicional);
                        ?>
                    </div>
                </div>

            </div>
            <div class="col-md-5">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Identificação</h5>
                    </div>
                    <div class="ibox-content">
                        <?php
                            echo $simec->select(
                                'suoid',
                                'SubUnidade',
                                $mPrePlanoInterno->suoid,
                                (new Public_Model_SubUnidadeOrcamentaria())->queryComboChaveSuoid((object)array(
                                    'unofundo' => 'FALSE',
                                    'exercicio' => $_SESSION['exercicio'],
                                    'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)),
                                ['required']
                            );
                            echo $simec->select('eqdid', 'Enquadramento da Despesa', $mPrePlanoInterno->eqdid, (new Planacomorc_Model_EnquadramentoDespesa())->recuperarSqlCombo(), ['required']);
                            echo '<span id="span-item">' . $simec->select('maiid', 'Item', $mPrePlanoInterno->maiid, (new Planacomorc_Model_ManutencaoItem())->recuperarSqlCombo(null, ["eqdid = " . (int) $mPrePlanoInterno->eqdid ])) . '</span>';
                            echo '<span id="span-subitem">' . $simec->select('masid', 'SubItem', $mPrePlanoInterno->masid, (new Planacomorc_Model_ManutencaoSubItem())->recuperarSqlCombo(null, ["maiid = " . (int) $mPrePlanoInterno->maiid ])) . '</span>';
                            echo '<span id="span-funcional">' . $simec->select('ptrid', 'Funcional', $mPrePlanoInterno->ptrid, (new Monitora_Model_Ptres())->recuperarSqlPorSubunidadeEEnquadramento($mPrePlanoInterno->suoid, $mPrePlanoInterno->eqdid), ['required']) . '</span>';
                        ?>
                    </div>
                </div>
            </div>

            <div class="col-md-1">
                <?php
                if($mPrePlanoInterno->pliid){
                    wf_desenhaBarraNavegacao($mPrePlanoInterno->docid, array('benid' => $mPrePlanoInterno->pliid));
                }
                ?>
            </div>

            <div class="col-md-6">

                <div class="ibox float-e-margins div_metas">
                    <div class="ibox-title">
                        <h5>Metas</h5>
                    </div>
                    <div class="ibox-content">
                        <?php
                            echo $simec->select('oppid', 'Objetivo PPA', $mPrePlanoInterno->oppid, (new Public_Model_ObjetivoPpa())->recuperarSqlCombo(), ['required']);
                            echo '<span id="span-metappa">' . $simec->select('mppid', 'Metas PPA', $mPrePlanoInterno->mppid, (new Public_Model_MetaPpa())->recuperarSqlCombo(null, [
                                'suoid'=>(int)$mPrePlanoInterno->suoid,
                                'oppid'=>(int)$mPrePlanoInterno->oppid]), ['required']). '</span>';
                            echo '<span id="span-iniciativappa">' . $simec->select('ippid', 'Iniciativa PPA', $mPrePlanoInterno->ippid, (new Public_Model_IniciativaPpa())->recuperarSqlCombo(null, [
                                'oppid'=>(int)$mPrePlanoInterno->oppid
                            ]), []) . '</span>';
                            echo '<span id="span-metapnc">' . $simec->select('mpnid', 'Meta PNC', $mPrePlanoInterno->mpnid, (new Public_Model_MetaPnc())->recuperarSqlCombo(null, [
                                'suoid' => $mPrePlanoInterno->suoid
                            ]), ['required']) . '</span>';
                            echo '<span id="span-indicadorpnc">' . $simec->select('ipnid', 'Indicador PNC', $mPrePlanoInterno->ipnid, (new Public_Model_IndicadorPnc())->recuperarSqlCombo(null, [
                                'mpnid'=>(int)$mPrePlanoInterno->mpnid
                            ]), ['required']) . '</span>';
                        ?>
                    </div>
                </div>

                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Informações Complementares</h5>
                    </div>
                    <div class="ibox-content">
                        <?php
                            echo '<span id="span-area">'. $simec->select('arceid', 'Área Cultural', $mPrePlanoInterno->arceid, (new Planacomorc_Model_AreaCultural())->recuperarSqlCombo(), ['required']). '</span>';
                            echo '<span id="span-segmento">' . $simec->select('secid', 'Segmento Cultural', $mPrePlanoInterno->secid, (new Monitora_Model_PiNivelEtapaEnsino())->recuperarSqlCombo(), ['required']). '</span>';
                            echo $simec->select('esfid', 'Localização', $mPrePlanoInterno->esfid, (new Territorios_Model_Esfera())->recuperarSqlCombo());
                        ?>
                        <div class="select-localizacao"  id="div-localizacao_<?php echo Territorios_Model_Esfera::K_EXTERIOR ?>">
                            <?php echo $simec->select('paiid[]', 'País', $aLocalizacoes['paiid'], (new Territorios_Model_Pais())->recuperarSqlCombo()); ?>
                        </div>
                        <div class="select-localizacao" id="div-localizacao_<?php echo Territorios_Model_Esfera::K_ESTADUAL?>">
                            <?php echo $simec->select('estuf[]', 'UF', $aLocalizacoes['estuf'], (new Territorios_Model_Estado())->recuperarSqlCombo(['estuf', 'estdescricao'])); ?>
                        </div>
                        <div class="select-localizacao"  id="div-localizacao_<?php echo Territorios_Model_Esfera::K_MUNICIPAL ?>">
                            <?php echo $simec->select('muncod[]', 'Município', $aLocalizacoes['muncod'], (new Territorios_Model_Municipio())->recuperarSqlCombo(['estuf', 'mundescricao'])); ?>
                        </div>
                    </div>
                </div>

            </div>
        </div>

        <div class="form-group">
            <div class="text-center">
                <button type="submit" style="display: none;" />
                <button class="btn btn-primary" type="button" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                <a href="?modulo=principal/preplanointerno&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                <?php if($mPrePlanoInterno->pliid){ ?>
                    <a href="?modulo=principal/preplanointerno_form&acao=A&req=excluir&pliid=<?php echo $mPrePlanoInterno->pliid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                <?php } ?>
            </div>
        </div>
    </form>
</div>

<?php bootstrapPopup('Importar de PI ' . date('Y'), 'preplanointerno_modal', '', array('fechar'), array('tamanho' => 'lg')); ?>

<script>
    
    listaEqdReduzido = <?php echo $listaEqdidReduzido; ?>;
    intProdNaoAplica = "<?php echo $codigoProdutoNaoAplica; ?>";

    $(document).ready(function(){
        initPreplanointernoForm();
    });

</script>