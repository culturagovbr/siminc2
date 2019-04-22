<?php

    /**
     * Funções de apoio ao cadastro de PIs.
     * @see _funcoespi.php
     */
    include_once APPRAIZ. 'www/planacomorc/_funcoespi.php';
    include_once APPRAIZ. 'www/planacomorc/_funcoes.php';

    require_once APPRAIZ. 'includes/workflow.php';
    include_once APPRAIZ. "planacomorc/classes/Cronograma.class.inc";
    include_once APPRAIZ. "planacomorc/classes/Pi_Sniic.class.inc";
    include_once APPRAIZ. "planacomorc/classes/Pi_Localizacao.class.inc";
    include_once APPRAIZ. "planacomorc/classes/Pi_Convenio.class.inc";
    include_once APPRAIZ. "planacomorc/classes/Pi_Responsavel.class.inc";
    include_once APPRAIZ. "planacomorc/classes/Pi_Acao.class.inc";
    include_once APPRAIZ. "planacomorc/classes/Pi_Cronograma.class.inc";
    include_once APPRAIZ. "planacomorc/classes/Pi_Anexo.class.inc";
    include_once APPRAIZ. "monitora/classes/Pi_PlanoInterno.class.inc";

    $dadosPI = array();
    $perfis = pegaPerfilGeral();
    $listaDeAnexos = $aSniic = $aSei = $aPronac = $aConvenio = $aLocalizacao = $aResponsavel = array();

    $pliid = $_REQUEST['pliid'];

    // -- Carrega dados do PI e libera apenas o titulo e a descrição para edição
    $dadosPI = carregarPiComDetalhes((object) array('pliid' => $pliid));
    $enquadramento_pi = carregarEnquadramentoPI($pliid);

    $aSniic = (new Pi_Sniic())->recuperarTodos('*', array('pliid = ' . $pliid));
    $aSei = (new Planacomorc_Model_PiSei())->recuperarTodos('*', array('pliid = ' . $pliid));
    $aPronac = (new Planacomorc_Model_PiPronac())->recuperarTodos('*', array('pliid = ' . $pliid));
    $aConvenio = (new Pi_Convenio())->recuperarTodos('*', array('pliid = ' . $pliid));
    $aLocalizacao = (new Pi_Localizacao())->recuperarPorPlanoInterno($pliid, $dadosPI['esfid']);
    $aResponsavel = (new Pi_Responsavel())->recuperarPorPlanoInterno($pliid);
    $listaDeAnexos = (new Pi_Anexo())->recuperarPorPlanoInterno($pliid);
    $listaSubUnidadesDelegadas = (new Planacomorc_Model_PiDelegacao())->recuperarIdPorPlanoInterno($pliid, $_SESSION['exercicio']);

    $estadoAtual = wf_pegarEstadoAtual($dadosPI['docid']);
    $codigoEnquadramentoFinalistico = buscarCodigoEnquadramento((int)$_SESSION['exercicio'], 'F');
    $codigoEnquadramentoNaoOrcamentario = buscarCodigoEnquadramento((int)$_SESSION['exercicio'], 'N');
    $codigoEnquadramentoEmenda = buscarCodigoEnquadramento((int)$_SESSION['exercicio'], 'E');
    $codigoProdutoNaoAplica = buscarCodigoProdutoNaoAplica((int)$_SESSION['exercicio']);
    $listaEqdidReduzido = simec_json_encode(buscarListaCodigoEnquadramentoReduzido((int)$_SESSION['exercicio']));
    
    include APPRAIZ . "includes/cabecalho_bootstrap_v3_popup.inc";

    $autorizadoSubUnidade = carregarLimiteAutorizadoSubUnidade(
        (object) array(
            'ungcod' => $dadosPI['ungcod']
    ));
    $detalhadoSubUnidade = carregarLimiteDetalhadoSubUnidade(
        (object) array(
            'ungcod' => $dadosPI['ungcod']
    ));
    $disponivelSubUnidade = ($autorizadoSubUnidade - $detalhadoSubUnidade);

?>
<style>
    .input_fisico, .input_orcamentario, .input_financeiro, #user-info-table {
        display: none;
    }
    .titulo_espelho {
        text-align: center;
        background-color: #c0c0c0c0 !important;
    }

    div.footer{
        position: fixed;
        left: 0;
        bottom: 0;
    }
</style>

<script src="../planacomorc/js/cadastro_pi_funcoes.js"></script>
<!--
    @todo REFATORAR
    DETALHES FINANCEIROS DA FUNCIONAL APENAS PARA EXECUTAR OS CALCULOS AUTOMATICOS VIA JS VIDE carregarSaldoFuncional();
-->
<table cellpadding="5" border="0" width="98%" align="center" id="orcamento" style="display: none;" onmouseover="tabindexcampo();"  class="table table-hover table-bordered table-hover" >
    <tr>
        <td rowspan="2" nowrap>
            <b>PTRES</b>
        </td>
        <td rowspan="2" style="width:45%" nowrap>
            <b>Funcional</b>
        </td>
        <td rowspan="2" nowrap>
            <b>Dotação Atual(R$)</b>
        </td>
        <td colspan="2" nowrap>
            <b><span id="detPi">Detalhado em PI (R$)</span></b>
        </td>
        <td colspan="2">
            <b>Empenho (R$)</b>
        </td>
        <td rowspan="2" align="center">
            <b>Valor do Projeto</b>
        </td>
    </tr>
    <tr>
        <td nowrap><b>Detalhado</b></td>
        <td nowrap><b>Não&nbsp;Detalhado</b></td>
        <td nowrap><b>Empenhado</b></td>
        <td nowrap><b>Não&nbsp;Empenhado</b></td>
    </tr>
    <?php
//ver($dadosPI['pliid'], $dadosPI['ptrid']);
    if ($dadosPI['pliid'] || $dadosPI['ptrid']) {
        $dadosPTRES = buscarPTRES((object) array(
            'exercicio' => $_SESSION['exercicio'],
            'pliid' => $dadosPI['pliid'],
            'ptrid' => $dadosPI['ptrid'],
            'importar' => FALSE
        ));
        if($dadosPTRES) {
//ver($dadosPTRES);
            foreach ($dadosPTRES as $ptres):
                $vlrPiDetalhado = strip_tags($ptres['det_pi']);
                $vlrPiDetalhadoCusteio = strip_tags($ptres['det_pi_custeio']);
                $vlrPiDetalhadoCapital = strip_tags($ptres['det_pi_capital']);
                $vlrPtresCusteio = strip_tags($ptres['ptrdotacaocusteio']);
                $vlrPtresCapital = strip_tags($ptres['ptrdotacaocapital']);
                $vlrPiNaoDetalhado = strip_tags($ptres['nao_det_pi']);
                $vlrPiNaoDetalhadoCusteio = strip_tags($ptres['nao_det_pi_custeio']);
                $vlrPiNaoDetalhadoCapital = strip_tags($ptres['nao_det_pi_capital']);
                ?>
                <tr style="height:30px;" id="ptres_<?php echo $ptres['ptrid']; ?>">
                    <td align="center">
                        <span class="link btnVisualizarDetalhes" ptrid="<?php echo $ptres['ptrid']; ?>" ><?php echo $ptres['ptres']; ?></span>
                    </td>
                    <td align="left">
                        <span class="link btnVisualizarDetalhes" ptrid="<?php echo $ptres['ptrid']; ?>" ><?php echo $ptres['descricao']; ?></span>
                    </td>
                    <td align="right"><?php echo $ptres['dotacaoatual']; ?></td>
                    <td align="right" id="td_pi_detalhado">
                        <?php echo $ptres['det_pi']; ?>
                    </td>
                    <td align="right" id="td_pi_nao_detalhado">
                        <?php echo $ptres['nao_det_pi']; ?>
                    </td>
                    <td align="right"><?php echo $ptres['empenhado']; ?></td>
                    <td align="right">
                        <?php echo $ptres['nao_empenhado']; ?>
                        <input type="hidden" name="ptrid" value="<?php echo $ptres['ptrid']; ?>" />
                    </td>
                    <td align="center" id="td_valor_projeto">
                        <?php echo 'R$ '. $ptres['pipvalor']; ?>
                    </td>
                </tr>
                <?php
            endforeach;
        }
    }
    ?>
</table>

<input type="hidden" id="ptresCusteio" value="<?php echo $vlrPtresCusteio; ?>" />
<input type="hidden" id="ptresCapital" value="<?php echo $vlrPtresCapital; ?>" />
<input type="hidden" id="piDetalhado" value="<?php echo $vlrPiDetalhado; ?>" />
<input type="hidden" id="piDetalhadoCusteio" value="<?php echo $vlrPiDetalhadoCusteio; ?>" />
<input type="hidden" id="piDetalhadoCapital" value="<?php echo $vlrPiDetalhadoCapital; ?>" />
<input type="hidden" id="piNaoDetalhadoCusteio" value="<?php echo $vlrPiNaoDetalhadoCusteio; ?>" />
<input type="hidden" id="piNaoDetalhadoCapital" value="<?php echo $vlrPiNaoDetalhadoCapital; ?>" />
<input type="hidden" id="piNaoDetalhado" value="<?php echo $vlrPiNaoDetalhado; ?>" />

<div class="wrapper wrapper-content animated fadeInRight">
    <form id="formulario" name="formulario" method="post" action="" class="form-horizontal">
        <div class="row">

            <div class="col-sm-12">
                <div class="ibox float-e-margins">
                    <div class="ibox-heading">
                        <div class="form-group">
                            <div class="col-sm-4 text-left">
                                <button class="btn btn-primary noprint" type="button" id="btnImprimir">
                                    <i class="fa fa-print"></i>&nbsp;Imprimir
                                </button>
                            </div>
                            <div class="col-sm-8 text-right">
                                <h7>
                                    Impresso por: <?php echo $_SESSION['usunome']; ?>
                                    <br />
                                    Em: <?php echo date('d/m/Y'); ?>
                                </h7>
                            </div>
                        </div>
                    </div>
                    <div class="ibox-title">
                        <h5>Plano de Ação
                        <?php if($dadosPI['plicod']): ?>
                            <span style="color: red; padding: 5px;" id="span-plicod">(<?php echo $dadosPI['plicod']; ?>)</span>
                        <?php else: ?>
                            <span style="padding: 5px;" id="span-plicod">(<?php echo $dadosPI['pliid']; ?>)</span>
                        <?php endif; ?>
                        <?php if($estadoAtual['esddsc']): ?>
                            <span style="padding: 5px;" id="span-plicod"><?php echo $estadoAtual['esddsc']; ?></span>
                        <?php endif; ?>
                        </h5>
                    </div>
                    <div class="ibox-content">
                        <div class="form-group">
                            <div class="col-lg-12">
                                <table cellpadding="5" border="0" width="98%" align="center" id="orcamento" style="border:#c9c9c9 1px solid" class="table table-hover table-bordered table-hover" >
                                    <tr class="titulo_espelho">
                                        <td colspan="2">
                                            <b>Dados do Planejamento</b>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>ID do Projeto:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['pliid']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Código</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['plicod']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Unidade Solicitante:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['unidade']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Unidade delegada:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['sub_unidade']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Enquadramento:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['eqddsc']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Metas PPA:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['meta']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Meta PNC:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['meta_pnc']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Indicador PNC:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['iniciativa_pnc']; ?></span>
                                        </td>
                                    </tr>
                                    <tr class="titulo_espelho">
                                        <td colspan="2">
                                            <b>Funcional Programática</b>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>UO:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['uo']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Programa:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['programa']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Objetivo:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['objetivo']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Iniciativa:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['iniciativa']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Ação:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['acao']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Localizador:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['localizador']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Plano Orçamentário:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['po']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>PTRES:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['ptres']? $dadosPI['ptres']: NULL; ?></span>
                                        </td>
                                    </tr>
                                    <tr class="titulo_espelho">
                                        <td colspan="2">
                                            <b>Dados do Plano de Ação</b>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Título:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['plititulo']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Descrição / Finalidade:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['plidsc']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Produto PA:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['produto']; ?></span>
                                            <br />
                                            <strong>Unidade de medida:</strong> <span><?php echo $dadosPI['unidade_medida']; ?></span>
                                            <strong>Quantidade:</strong> <span><?php echo $dadosPI['picquantidade']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Modalidade de Pactuação:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['modalidade_pactuacao']; ?></span>
                                        </td>
                                    </tr>
                                    <tr class="titulo_espelho">
                                        <td colspan="2">
                                            <b>Forma de Execução</b>
                                        </td>
                                    </tr>
                                    <?php if($dadosPI['eqdid'] != $codigoEnquadramentoEmenda): ?>
                                    <tr>
                                        <td>
                                            <b>Edital:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['picedital'] == 't'? 'Sim': 'Não'; ?></span>
                                        </td>
                                    </tr>
                                    <?php endif; ?>
                                    <tr>
                                        <td>
                                            <b>TED:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['picted'] == 't'? 'Sim': 'Não'; ?></span>
                                        </td>
                                    </tr>
                                    <tr class="titulo_espelho">
                                        <td colspan="2">
                                            <b>Emenda</b>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Emenda:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['eqdid'] == $codigoEnquadramentoEmenda? 'Sim': 'Não'; ?></span>
                                        </td>
                                    </tr>
                                    <tr class="titulo_espelho">
                                        <td colspan="2">
                                            <b>Localização do Projeto</b>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Tipo de localização:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['tipo_localizacao']; ?></span>
                                        </td>
                                    </tr>
                                    <?php if($dadosPI['esfid'] == Territorios_Model_Esfera::K_MUNICIPAL): ?>
                                    <tr>
                                        <td>
                                            <b>Localizações:</b>
                                        </td>
                                        <td align="left">
                                            <table id="table_localizacao" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                                <tr class="tr_head">
                                                    <td style="text-align: left;"><b>UF</b></td>
                                                    <td style="text-align: left;"><b>Município</b></td>
                                                </tr>
                                                <?php foreach($aLocalizacao as $localizacao): ?>
                                                    <tr style="height: 30px;" class="tr_localizacao_<?php echo $localizacao['muncod']; ?>" >
                                                        <td style="text-align: left;"><?php echo $localizacao['estuf']; ?></td>
                                                        <td style="text-align: left;"><?php echo $localizacao['mundescricao']; ?></td>
                                                    </tr>
                                                <?php endforeach; ?>
                                            </table>
                                        </td>
                                    </tr>
                                    <?php elseif($dadosPI['esfid'] == Territorios_Model_Esfera::K_ESTADUAL): ?>
                                    <tr>
                                        <td>
                                            <b>localização(ões):</b>
                                        </td>
                                        <td align="left">
                                            <table id="table_localizacao" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                                <tr class="tr_head">
                                                    <td style="text-align: left;"><b>UF</b></td>
                                                    <td style="text-align: left;"><b>Nome</b></td>
                                                </tr>
                                                <?php foreach($aLocalizacao as $localizacao): ?>
                                                <tr style="height: 30px;" class="tr_localizacao_estadual_<?php echo $localizacao['estuf']; ?>" >
                                                    <td style="text-align: left;"><?php echo $localizacao['estuf']; ?></td>
                                                    <td style="text-align: left;"><?php echo $localizacao['estdescricao']; ?></td>
                                                </tr>
                                                <?php endforeach; ?>
                                            </table>
                                        </td>
                                    </tr>
                                    <?php elseif($dadosPI['esfid'] == Territorios_Model_Esfera::K_EXTERIOR): ?>
                                    <tr>
                                        <td>
                                            <b>localização(ões):</b>
                                        </td>
                                        <td align="left">
                                            <table id="table_localizacao" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                                <tr class="tr_head">
                                                    <td style="text-align: left;"><b>País</b></td>
                                                </tr>
                                                <?php foreach($aLocalizacao as $localizacao): ?>
                                                <tr style="height: 30px;" class="tr_localizacao_exterior_<?php echo $localizacao['paiid']; ?>" >
                                                    <td style="text-align: left;"><?php echo $localizacao['paidescricao']; ?></td>
                                                </tr>
                                                <?php endforeach; ?>
                                            </table>
                                        </td>
                                    </tr>
                                    <?php endif; ?>
                                    <?php if($dadosPI['eqdid'] != $codigoEnquadramentoEmenda): ?>
                                    <tr class="titulo_espelho">
                                        <td colspan="2">
                                            <b>Responsáveis do Projeto</b>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td colspan="2">
                                            <table id="table_responsaveis" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                                <tr>
                                                    <td style="text-align: left;"><b>Nome</b></td>
                                                    <td style="text-align: left;"><b>Telefone</b></td>
                                                    <td style="text-align: left;"><b>E-mail</b></td>
                                                </tr>
                                                <?php foreach($aResponsavel as $responsavel){ ?>
                                                    <tr style="height: 30px;" class="tr_responsaveis_<?php echo $responsavel['pirid']; ?>" >
                                                        <td style="text-align: left;"><?php echo $responsavel['usunome']; ?></td>
                                                        <td style="text-align: left;"><?php echo $responsavel['usutelefone']; ?></td>
                                                        <td style="text-align: left;"><?php echo $responsavel['usuemail']; ?></td>
                                                    </tr>
                                                <?php } ?>
                                            </table>
                                        </td>
                                    </tr>
                                    <?php endif; ?>
                                    <tr class="titulo_espelho">
                                        <td colspan="2">
                                            <b>Vinculações Eventuais do Projeto</b>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td colspan="2">
                                            <table cellpadding="5" border="0" width="98%" align="center" id="orcamento" style="border:#c9c9c9 1px solid" class="table table-hover table-bordered table-hover" >
                                                <tr>
                                                    <td>
                                                        <b>Números SNIIC:</b>
                                                    </td>
                                                    <td>
                                                        <b>Número SEI:</b>
                                                    </td>
                                                    <td>
                                                        <b>Número Pronac:</b>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td>
                                                        <table id="table_localizacao" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                                            <?php foreach($aSniic as $sniic){ ?>
                                                                <tr>
                                                                    <td style="text-align: left;"><?php echo $sniic['pissniic']; ?></td>
                                                                </tr>
                                                            <?php } ?>
                                                        </table>
                                                    </td>
                                                    <td>
                                                        <table id="table_localizacao" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                                            <?php foreach($aSei as $sei){ ?>
                                                                <tr>
                                                                    <td style="text-align: left;"><?php echo $sei['psesei']; ?></td>
                                                                </tr>
                                                            <?php } ?>
                                                        </table>
                                                    </td>
                                                    <td>
                                                        <table id="table_localizacao" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                                            <?php foreach($aPronac as $pronac){ ?>
                                                                <tr>
                                                                    <td style="text-align: left;"><?php echo $pronac['pprpronac']; ?></td>
                                                                </tr>
                                                            <?php } ?>
                                                        </table>
                                                    </td>
                                                </tr>
                                            </table>
                                        </td>
                                    </tr>
                                </table>
                                
                                <table cellpadding="5" border="0" width="98%" align="center" id="orcamento" style="border:#c9c9c9 1px solid; page-break-before: always; margin-bottom: 15px" class="table table-hover table-bordered table-hover" >
                                    <tr class="titulo_espelho">
                                        <td colspan="2">
                                            <b>Valores</b>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td colspan="2">
                                            <table cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                                <tr>
                                                    <td style="text-align: left;"></td>
                                                    <td colspan="3" style="text-align: center;">
                                                        <b>Total (Custeio e Capital)</b>
                                                    </td>
                                                </tr>
                                                <?php if($dadosPI['eqdid'] != $codigoEnquadramentoEmenda): ?>
                                                    <tr style="height: 30px;">
                                                        <td style="text-align: left;">Limite Autorizado na Unidade: </td>
                                                        <td colspan="3" id="td_autorizado_sub_unidade" style="text-align: center;" >
                                                            <?php echo number_format($autorizadoSubUnidade, 2, ',', '.') ?>
                                                        </td>
                                                    </tr>
                                                    <tr style="height: 30px;">
                                                        <td style="text-align: left;">
                                                            Limite Disponível na Unidade:
                                                            <input id="VlrSUDisponivel" type="hidden" value="" />
                                                        </td>
                                                        <td colspan="3" id="td_disponivel_sub_unidade" style="text-align: center;" >
                                                            <?php echo number_format($disponivelSubUnidade, 2, ',', '.') ?>
                                                        </td>
                                                    </tr>
                                                <?php endif; ?>
                                                <tr>
                                                    <td style="text-align: left;"></td>
                                                    <td style="text-align: center;"><b>Custeio </b></td>
                                                    <td style="text-align: center;"><b>Capital</b></td>
                                                </tr>
                                                <tr>
                                                    <td style="text-align: left;">Autorizado na Funcional:</td>
                                                    <td style="text-align: center;" id="td_autorizado_funcional_custeio" >0,00</td>
                                                    <td style="text-align: center;" id="td_autorizado_funcional_capital" >0,00</td>
                                                </tr>
                                                <tr>
                                                    <td style="text-align: left;">
                                                        Saldo Disponível na Funcional:
                                                        <input id="VlrFuncionalDisponivelCusteio" type="hidden" value="" />
                                                        <input id="VlrFuncionalDisponivelCapital" type="hidden" value="" />
                                                    </td>
                                                    <td style="text-align: center;" id="td_disponivel_funcional_custeio" >0,00</td>
                                                    <td style="text-align: center;" id="td_disponivel_funcional_capital" >0,00</td>
                                                </tr>
                                                <tr>
                                                    <td id="" style="text-align: left;">Valor do Projeto:</td>
                                                    <td style="text-align: center;">
                                                        <?php echo !empty($dadosPI['picvalorcusteio']) ? number_format($dadosPI['picvalorcusteio'], 2, ',', '.') : ''; ?>
                                                        <input style="display: none;" type="text" id="picvalorcusteio" name="picvalorcusteio" value="<?php echo !empty($dadosPI['picvalorcusteio']) ? number_format($dadosPI['picvalorcusteio'], 2, ',', '.') : ''; ?>" class="CampoEstilo normal form-control obrigatorio" />
                                                        <input type="hidden" id="vlrPiCusteio" value="<?php echo !empty($dadosPI['picvalorcusteio']) ? number_format($dadosPI['picvalorcusteio'], 2, ',', '.') : ''; ?>" />
                                                    </td>
                                                    <td style="text-align: center;">
                                                        <?php echo !empty($dadosPI['picvalorcapital']) ? number_format($dadosPI['picvalorcapital'], 2, ',', '.') : ''; ?>
                                                        <input style="display: none;" type="text" id="picvalorcapital" name="picvalorcapital" value="<?php echo !empty($dadosPI['picvalorcapital']) ? number_format($dadosPI['picvalorcapital'], 2, ',', '.') : ''; ?>" class="CampoEstilo normal form-control obrigatorio" />
                                                        <input type="hidden" id="vlrPiCapital" value="<?php echo !empty($dadosPI['picvalorcapital']) ? number_format($dadosPI['picvalorcapital'], 2, ',', '.') : ''; ?>" />
                                                    </td>
                                                </tr>
                                            </table>
                                        </td>
                                    </tr>
                                    <?php if($dadosPI['eqdid'] != $codigoEnquadramentoEmenda): ?>
                                    <tr class="titulo_espelho">
                                        <td colspan="2">
                                            <b>Cronogramas</b>
                                            <?php
                                                $sql = "SELECT mescod, mesdsc FROM public.meses ORDER BY mescod";
                                                $meses = $db->carregar($sql);

                                                $modelPiCronograma =  new Pi_Cronograma();

                                                $piCronogramasAgrupado = [];
                                                if(!empty($dadosPI['pliid'])){
                                                    $aPiCronograma = $modelPiCronograma->recuperarTodos('*', array('pliid = ' . $dadosPI['pliid']));

                                                    foreach($aPiCronograma as $piCronograma){
                                                        $piCronogramasAgrupado[$piCronograma['mescod']][$piCronograma['crvid']] = $piCronograma;
                                                    }
                                                }

                                                $aCronogramas = $modelPiCronograma->recuperarCronogramas();

                                                $cronogramasAgrupado = [];
                                                foreach($aCronogramas as $cronograma){
                                                    $cronogramasAgrupado[$cronograma['crodsc']][] = $cronograma;
                                                }

                                            ?>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td colspan="2">
                                            <table cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover table-input">
                                                <tr>
                                                    <td rowspan="2" style="text-align: center;">
                                                        <b>Mês</b>
                                                    </td>
                                                    <?php 
                                                        $colunaCronograma = 0;
                                                        foreach($cronogramasAgrupado as $cronograma => $descricaoValores){?>
                                                                <td style="text-align: center;" colspan="<?php echo count($descricaoValores); ?>" class="<?php echo buscarClasseCronograma($colunaCronograma, TRUE); ?>"><b><?php echo $cronograma; ?></b></td>
                                                    <?php
                                                        $colunaCronograma++;
                                                        }
                                                    ?>
                                                </tr>
                                                <tr style="height: 30px;">
                                                    <?php
                                                        $colunaCronograma = 0;
                                                        foreach($cronogramasAgrupado as $cronograma => $descricaoValores){
                                                                foreach($descricaoValores as $cronogramaValor){
                                                        ?>
                                                            <td style="text-align: center;" class="<?php echo buscarClasseCronograma($colunaCronograma, TRUE); ?>"><?php echo $cronogramaValor['crvdsc']; ?></td>
                                                        <?php
                                                                    $colunaCronograma++;
                                                                }
                                                    }
                                                    ?>
                                                </tr>
                                                <?php foreach($meses as $mes){ ?>
                                                    <tr style="height: 30px;">
                                                        <td style="text-align: left;"><?php echo $mes['mesdsc']; ?></td>
                                                        <?php foreach($aCronogramas as $cronogramaValor){
                                                            switch($cronogramaValor['croid']){
                                                                case Cronograma::K_FISICO:
                                                                    $class = 'input_fisico inteiro';
                                                                    break;
                                                                case Cronograma::K_ORCAMENTARIO:
                                                                    $class = 'input_orcamentario';
                                                                    break;
                                                                case Cronograma::K_FINANCEIRO:
                                                                    $class = 'input_financeiro';
                                                                    break;
                                                                default: $class = '';
                                                            }
                                                            
                                                            ?>
                                                        <td class="text-center <?php echo buscarClasseCronograma($cronogramaValor['croid']); ?>">
                                                                <?php
                                                                    $pcrid = $pcrvalor = '';
                                                                    if(isset($piCronogramasAgrupado[$mes['mescod']][$cronogramaValor['crvid']])){
                                                                        $pcrid = $piCronogramasAgrupado[$mes['mescod']][$cronogramaValor['crvid']]['pcrid'];
                                                                        $pcrvalor = $piCronogramasAgrupado[$mes['mescod']][$cronogramaValor['crvid']]['pcrvalor'];

                                                                        if($cronogramaValor['croid'] != Cronograma::K_FISICO){
                                                                            $pcrvalor = number_format($pcrvalor, 2, ',', '.');
                                                                        }
                                                                    }

                                                                    switch ($cronogramaValor['crvid']) {
                                                                        case Cronograma::ORCAMENTO_CUSTEIO:
                                                                            $class .= ' custeio';
                                                                        break;
                                                                        case Cronograma::FINANCEIRO_CUSTEIO:
                                                                            $class .= ' custeio';
                                                                        break;
                                                                        case Cronograma::ORCAMENTO_CAPITAL:
                                                                            $class .= ' capital';
                                                                        break;
                                                                        case Cronograma::FINANCEIRO_CAPITAL:
                                                                            $class .= ' capital';
                                                                        break;
                                                                    }

                                                                ?>
                                                                <input name="cronograma[<?php echo $mes['mescod']; ?>][<?php echo $cronogramaValor['crvid']; ?>][pcrid]" value="<?php echo $pcrid; ?>" type="hidden">
                                                                <input name="cronograma[<?php echo $mes['mescod']; ?>][<?php echo $cronogramaValor['crvid']; ?>][pcrvalor]" value="<?php echo $pcrvalor; ?>" class="CampoEstilo normal form-control obrigatorio <?php echo $class ?>" type="text">
                                                                <?php echo $pcrvalor; ?>
                                                            </td>
                                                        <?php
                                                            
                                                        } ?>
                                                    </tr>
                                                <?php } ?>
                                                <tr style="height: 30px; ">
                                                    <td style="text-align: left;">Total:</td>
                                                    <td style="font-weight: bold; text-align: center;" id="td_total_fisico" class="td_cronograma_fisico">0</td>
                                                    <td style="font-weight: bold; text-align: center;" id="td_total_orcamentario_custeio" class="td_cronograma_orcamentario">R$ 0,00</td>
                                                    <td style="font-weight: bold; text-align: center;" id="td_total_orcamentario_capital" class="td_cronograma_orcamentario">R$ 0,00</td>
                                                    <td style="font-weight: bold; text-align: center;" id="td_total_financeiro_custeio" class="td_cronograma_financeiro">R$ 0,00</td>
                                                    <td style="font-weight: bold; text-align: center;" id="td_total_financeiro_capital" class="td_cronograma_financeiro">R$ 0,00</td>
                                                </tr>
                                            </table>
                                        </td>
                                    </tr>
                                </table>
                                <?php endif; ?>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </form>
</div>

<script type="text/javascript">
    $(document).ready(function() {
        carregarSaldoFuncional();
        atualizarTotalFisico();
        atualizarTotalOrcamentario();
        atualizarTotalFinanceiro();
        
        $('#btnImprimir').click(function(){
            window.print();
        });
        
    });
</script>