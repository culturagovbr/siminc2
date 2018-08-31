<?php

include_once '../planacomorc/_funcoespi.php';
require_once APPRAIZ . 'includes/workflow.php';

$cBeneficiario = new Emendas_Controller_Beneficiario();
$cBeneficiarioObservacao = new Emendas_Controller_BeneficiarioObservacao();

$perfis = pegaPerfilGeral();
$podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;
//$podeEditar = TRUE;
$configPodeEditar = array('pode-editar' => $podeEditar? 'S': 'N');

switch ($_REQUEST['req']) {
    case 'salvar':
        $cBeneficiario->salvar($_REQUEST);
        die;
    case 'salvar-complemento':
        $cBeneficiario->salvar($_REQUEST);
        die;
    case 'excluir':
        $cBeneficiario->excluir($_REQUEST['benid']);
        die;
    case 'formulario-observacao':
        $cBeneficiario->montarFormularioObservacao($_REQUEST['benid'], $_REQUEST['bobid']);
        die;
    case 'listar-observacao':
        $cBeneficiario->montarListagemObservacao($_REQUEST['benid'], $podeEditar);
        die;
    case 'salvar-observacao':
        $cBeneficiarioObservacao->salvar($_POST);
        die;
    case 'excluir-observacao':
        echo $cBeneficiarioObservacao->excluir($_REQUEST['bobid']);
        die;
    case 'formulario-detalhe':
        $cBeneficiario->montarFormularioDetalhe($_REQUEST['emeid'], $_REQUEST['benid'], $_REQUEST['bedid']);
        die;
    case 'listar-detalhe':
        $cBeneficiario->montarListagemDetalhe($_REQUEST['benid'], $podeEditar);
        die;
    case 'salvar-detalhe':
        $cBeneficiarioDetalhe = new Emendas_Controller_BeneficiarioDetalhe();
        $cBeneficiarioDetalhe->salvar($_POST);
        die;
    case 'excluir-detalhe':
        $cBeneficiarioDetalhe = new Emendas_Controller_BeneficiarioDetalhe();
        echo $cBeneficiarioDetalhe->excluir($_REQUEST['bedid']);
        die;
    case 'carregarSegmento':
        $mBeneficiario = new Emendas_Model_Beneficiario($_REQUEST['benid']);
        echo $simec->select('neeid', 'Segmento Cultural <span class="campo-obrigatorio" title="Campo obrigatório pra Gerar PI">*</span>', $mBeneficiario->neeid, (new Monitora_Model_PiNivelEtapaEnsino())->recuperarSqlCombo(null, ["neestatus = 'A'", "mdeid = " . (int)$_REQUEST['mdeid']]));
        die;
    case 'verificarPactuacaoConvenio':
        echo verificarPactuacaoConvenio($_REQUEST['capid']);
        die;
    case 'atualizar-siconv':
        $cBeneficiario->atualizarSiconv($_REQUEST['emeid'], $_REQUEST['benid']);
        die;
    case 'atualizar-programa':
        $cBeneficiario->atualizarPrograma($_REQUEST['emeid'], $_REQUEST['benid']);
        die;
    case 'uploadAnexo':
        $file = new FilesSimec();
        if($file->setUpload($_REQUEST['arqdescricao'], '', false)){
            echo simec_json_encode(array(
                'arqid' => $file->getIdArquivo(),
                'arqnome' => $_FILES['file']['name'],
                'arqdescricao' => $_REQUEST['arqdescricao']
            ));
        }else{
            echo simec_json_encode(array(
                'error' => 1,
                'errorMensage' => 'Não foi possível enviar o arquivo!'
            ));
        }
        die;
    case 'carregarMetasPPA':
        $sql = (new Public_Model_MetaPpa())->recuperarSqlCombo(NULL, array(
            'exercicio' => $_SESSION['exercicio'],
            'suoid' => $_REQUEST['suoid'],
            'oppid' => $_REQUEST['oppid']));
        $listaMetaPpa = $db->carregar($sql);

        if($listaMetaPpa){
            echo simec_json_encode($listaMetaPpa);
        } else {
            echo simec_json_encode(array());
        }
        die;
    case 'carregarIniciativaPPA':
        $sql = (new Public_Model_IniciativaPpa())->recuperarSqlCombo(NULL, array(
            'exercicio' => $_SESSION['exercicio'],
            'oppid' => $_REQUEST['oppid']));
        $listaIniciativaPpa = $db->carregar($sql);
        if($listaIniciativaPpa){
            echo simec_json_encode($listaIniciativaPpa);
        } else {
            echo simec_json_encode(array());
        }
        die;
    case 'carregarIndicadorPNC':
        $sql = (new Public_Model_IndicadorPnc())->recuperarSqlCombo(NULL, array(
            'exercicio' => $_SESSION['exercicio'],
            'mpnid' => $_REQUEST['mpnid']));
        $listaIndicadorPnc = $db->carregar($sql);
        if($listaIndicadorPnc){
            echo simec_json_encode($listaIndicadorPnc);
        } else {
            echo simec_json_encode(array());
        }
        die;
    case 'espelho-pi':
        include APPRAIZ. 'planacomorc/modulos/principal/unidade/espelho-pi.inc';
        die;
    case 'historico-alteracao':
        include APPRAIZ. 'emendas/modulos/principal/beneficiario_historico_modal.inc';
        die;
}
//ver('ok', d);
$mBeneficiario = new Emendas_Model_Beneficiario($_REQUEST['benid']);
$beneficiario = (object)$mBeneficiario->buscarBeneficiario($mBeneficiario->benid);
$convenio = (new Monitora_Model_CategoriaApropriacao())->recuperarTodos('*', ["capstatus = 'A'",  "capdsc = 'Convênio'", "capano = '{$_SESSION['exercicio']}'"]);
$listaSniic = (new Emendas_Model_Sniic())->recuperarTodos('*', array('benid = '. (int)$mBeneficiario->benid));
$listaPronac = (new Emendas_Model_Pronac())->recuperarTodos('*', array('benid = '. (int)$mBeneficiario->benid));
$listaDeAnexos = (new Emendas_Model_BeneficiarioAnexo())->buscarPorBeneficiario((int)$mBeneficiario->benid);

# Selecionando Aba
$aba = $_REQUEST['aba'];
$classTab0 = empty($aba)? 'active': NULL;
$classTab1 = $aba? 'active': NULL;

if(!$mBeneficiario->emeid){ $mBeneficiario->emeid = $_REQUEST['emeid']; }
$mEmenda = new Emendas_Model_Emenda($mBeneficiario->emeid);

$estadoAtual = $mBeneficiario->gerarDocid(WF_TPDID_BENEFICIARIO, 'Beneficiário');

include APPRAIZ . "includes/cabecalho.inc";
?>
<script language="JavaScript" src="../includes/funcoesspo.js?v=1"></script>
<style>
    .select-localizacao {
        display: none;
    }

    .coluna-parecer {
        cursor: pointer;
    }
    .labelTextLeft {
        text-align: left !important;
    }

</style>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-4">
        <h2><?php echo $titulo_modulo; ?></h2>
        <?php echo $mBeneficiario->pliid? "<span class='link a_espelho' title='Exibir dados do Plano Interno' data-pi='{$mBeneficiario->pliid}' style='color: red;'>Id do PI: {$mBeneficiario->pliid}</span>": NULL; ?>
    </div>
    <div class="col-lg-8 dados-siconv">
        <div style="padding-top: 10px;" class="text-right">
            <a class="btn btn-info" href="?modulo=principal/beneficiario_form&acao=A&req=atualizar-siconv&emeid=<?php echo $mBeneficiario->emeid; ?>&benid=<?php echo $mBeneficiario->benid; ?>"><i class="fa fa-refresh"></i> Atualizar dados SICONV</a>
        </div>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">

        <div class="tabs-container">
            <ul class="nav nav-tabs">
                <li class="<?php echo $classTab0; ?>">
                    <a data-toggle="tab" href="#tab-0">Cadastro Inicial</a>
                </li>
                <li class="<?php echo $classTab1; ?>">
                    <a data-toggle="tab" href="#tab-1">Cadastro complementar do PI</a>
                </li>
            </ul>
            <div class="tab-content">
                <div id="tab-0" class="tab-pane <?php echo $classTab0; ?>">
                    <div class="panel-body">

                        <form id="formulario-beneficiario" name="formulario-beneficiario" method="post" class="form-horizontal">

                            <div class="col-md-6">
                                <div class="ibox float-e-margins">
                                    <div class="ibox-title">
                                        <h5>Dados Gerais</h5>
                                        <a style="font-size: 2.4em; margin-left: 10px;" class="fa fa-history" id="modal-historico-beneficiario" data-benid="<?= $mBeneficiario->benid; ?>" title="Histórico de alterações"></a>
                                    </div>
                                    <div class="ibox-content">
                                        <input type="hidden" name="req" id="req" value="salvar" />
                                        <input name="benid" id="benid" type="hidden" value="<?php echo $mBeneficiario->benid; ?>">
                                        <input name="emeid" id="emeid" type="hidden" value="<?php echo $mBeneficiario->emeid; ?>">

                                        <?php
                                            $sql = (new Public_Model_SubUnidadeOrcamentaria())->recuperarSqlCombo(['suocod', 'suonome'], ["unoid = ". (int)$mEmenda->unoid], 'suonome');
                                            $options = simec_preparar_array($db->carregar($sql));
                                            $mBeneficiario->suoid = (!empty($mBeneficiario->suoid) && count($options) == 1) ? key($options) : $mBeneficiario->suoid;
                                            echo $simec->select('suoid', 'SubUnidade', $mBeneficiario->suoid, $options, ['required'], $configPodeEditar);

                                            $listaSubUnidadesDelegadas = $mBeneficiario->recuperarDelegacao();
                                        ?>

                                        <div class="form-group">
                                            <div class="col-md-3 text-right">
                                                <label class="control-label" for="radioDelegacao">Delegação:</label>
                                            </div>
                                            <div class="col-md-1">
                                                <input <?php echo $podeEditar? NULL: 'disabled="disabled"'; ?> type="checkbox" name="radioDelegacao" id="radioDelegacao" value="t" class="js-switch" <?php echo count($listaSubUnidadesDelegadas) ? 'checked="checked"' : ''; ?> />
                                            </div>
                                            <div class="col-md-7" id="div_unidades_delegadas" style="margin-left: 15px;">
                                            <?php
                                                $aSubUnidadeOrcamentaria = simec_preparar_array((new Public_Model_SubUnidadeOrcamentaria())->recuperarTodos("suoid as codigo, unosigla || ' - ' || suonome as descricao", ["prsano = '{$_SESSION['exercicio']}'", "suostatus = 'A'"], 'descricao'));
                                                echo $simec->select("delegacao[]", '', $listaSubUnidadesDelegadas, $aSubUnidadeOrcamentaria, [], $configPodeEditar);
                                            ?>
                                            </div>
                                        </div>

                                        <?php
                                            echo $simec->select('proid', 'Proponente', $mBeneficiario->proid, (new Emendas_Model_Proponente())->recuperarSqlCombo(["replace(to_char(procnpj::numeric, '00:000:000/0000-00'), ':', '.')", 'pronome'], null,'pronome'), ['required'], $configPodeEditar);

                                            echo $simec->select('esfid', 'Localização <span class="campo-obrigatorio" title="Campo obrigatório pra Gerar PI">*</span>', $mBeneficiario->esfid, (new Territorios_Model_Esfera())->recuperarSqlCombo());
                                            $aLocalizacoes = (new Emendas_Model_BeneficiarioLocalizacao())->recuperarPorBeneficiario($mBeneficiario->benid);
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

                                        <?php
                                            echo $simec->input('bennumeroprocesso', 'Número do Processo', $mBeneficiario->bennumeroprocesso, ['maxlength' => 50]);
                                            echo $simec->data('beninicio', 'Data de Início da Vigência', $mBeneficiario->beninicio);
                                            echo $simec->select('capid', 'Modalidade de Pactuação', $mBeneficiario->capid, (new Monitora_Model_CategoriaApropriacao())->recuperarSqlCombo(null, ["capstatus = 'A'"]), ['required']);
                                            echo $simec->radioBoolean('bented', 'TED', $mBeneficiario->bented);
                                        ?>
                                    </div>
                                </div>
                            </div>

                            <div class="col-md-5">

                                <div class="ibox float-e-margins">
                                    <div class="ibox-title">
                                        <h5>Informações Complementares</h5>
                                    </div>
                                    <div class="ibox-content">
                                        <?php
                                            echo $simec->select('mdeid', 'Área Cultural <span class="campo-obrigatorio" title="Campo obrigatório pra Gerar PI">*</span>', $mBeneficiario->mdeid, (new Monitora_Model_PiModalidadeEnsino())->recuperarSqlCombo(null, ["mdestatus = 'A'"]));
                                            echo '<span id="span-segmento">' . $simec->select('neeid', 'Segmento Cultural <span class="campo-obrigatorio" title="Campo obrigatório pra Gerar PI">*</span>', $mBeneficiario->neeid, (new Monitora_Model_PiNivelEtapaEnsino())->recuperarSqlCombo(null, ["neestatus = 'A'", "mdeid = " . (int)$mBeneficiario->mdeid])) . '</span>';
                                            echo $simec->select("alteracao[]", 'Alterações', $mBeneficiario->recuperarAlteracao(), (new Emendas_Model_AlteracaoOrcamentaria())->recuperarSqlCombo(), [], $configPodeEditar);
                                            # Setar os campos para "não" por padrão.
                                            $benparecertecnico = $mBeneficiario->benparecertecnico ? $mBeneficiario->benparecertecnico : 0;
                                            $benparecerjuridico = $mBeneficiario->benparecerjuridico ? $mBeneficiario->benparecerjuridico : 0;
                                            echo $simec->radio('benparecertecnico', 'Parecer Técnico', $benparecertecnico, $mBeneficiario->recuperarTiposParecer());
                                            echo $simec->radio('benparecerjuridico', 'Parecer Jurídico', $benparecerjuridico, $mBeneficiario->recuperarTiposParecer());
                                        ?>
                                        <div class="form-group ">
                                            <label for="benconveniado" class="col-sm-3 control-label">Conveniado: </label>
                                            <div class="col-md-2 ">
                                                <input type="checkbox" name="benconveniado" id="benconveniado" value="t" class="js-switch" <?php echo 't' == $mBeneficiario->benconveniado ? 'checked="checked"' : ''; ?> />
                                            </div>
                                        </div>
                                        <div class="form-group ">
                                            <label class="col-sm-3 control-label">Empenhado: </label>
                                            <label class="col-md-3 control-label labelTextLeft">R$ <?php echo number_format($beneficiario->empenhado, 2, ',', '.'); ?></label>
                                            <label class="col-sm-2 control-label">Pago: </label>
                                            <label class="col-md-3 control-label labelTextLeft">R$ <?php echo number_format($beneficiario->pago, 2, ',', '.'); ?></label>
                                        </div>
                                        <?php
                                            $benimpedimento = $mBeneficiario->benimpedimento ? $mBeneficiario->benimpedimento : 0;
                                        ?>
                                        <div id="benimpedimento-group">
                                        <?php
                                            echo $simec->radio('benimpedimento', 'Impedimento', $benimpedimento, $mBeneficiario->recuperarTiposParecer());
                                        ?>
                                        </div>
                                        <div class="form-group ">
                                            <label for="benimpedimento" class="col-sm-3 control-label div_motivo">Motivo: </label>
                                            <div class="col-md-6 div_motivo">
                                                <?php echo $simec->select('impid', null, $mBeneficiario->impid, (new Emendas_Model_Impedimento())->recuperarSqlCombo(['impcod', 'impdsc'], ["impstatus = 'A'"], 'impcod'));; ?>
                                            </div>
                                        </div>

                                        <div class="div_motivo">
                                            <?php echo $simec->textarea('benmotivoimpedimento', 'Justificativa', $mBeneficiario->benmotivoimpedimento); ?>
                                        </div>
                                    </div>
                                </div>
                            </div>

                            <div class="col-md-1">
                                <?php
                                  if($mBeneficiario->suoid){
                                    wf_desenhaBarraNavegacao($mBeneficiario->docid, array('benid' => $mBeneficiario->benid));
                                  }
                                ?>
                            </div>

                            <div class="col-md-12">
                                <div class="form-group">
                                    <div class="text-center">
                                        <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                        <a href="?modulo=principal/emenda&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                        <a href="?modulo=principal/emenda_form&acao=A&emeid=<?php echo $mBeneficiario->emeid; ?>" class="btn btn-success" id="btn-emenda" type="button"><i class="fa fa-file"></i>&nbsp;Ver Emenda</a>
                                        <?php if($mBeneficiario->benid && $podeEditar){ ?>
                                            <a href="?modulo=principal/beneficiario_form&acao=A&req=excluir&benid=<?php echo $mBeneficiario->benid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                        <?php } ?>
                                    </div>
                                </div>
                            </div>
                        </form>

                        <div class="col-md-4">
                            <div class="ibox float-e-margins">
                                <div class="ibox-title">
                                    <h5>Observações</h5>
                                </div>
                                <div class="ibox-content">
                                    <div id="div_formulario_observacao">
                                        <?php if($mBeneficiario->benid){
                                          $cBeneficiario->montarFormularioObservacao($mBeneficiario->benid);
                                        } ?>
                                    </div>
                                    <div id="div_listagem_observacao">
                                        <?php
                                        # Busca perfis do usuário logado
                                        $cBeneficiario->montarListagemObservacao($mBeneficiario->benid, $podeEditar);
                                         ?>
                                    </div>
                                </div>
                            </div>
                        </div>

                        <div class="col-md-8">
                            <div class="ibox float-e-margins">
                                <div class="ibox-title">
                                        <h5> Dados Financeiros -
                                            <?php
                                                if($beneficiario->funcional){
                                                    echo "<b>PTRES</b>: " .$beneficiario->ptres . "&nbsp; <b>Funcional</b>:" . $beneficiario->funcional ;
                                                }
                                            ?>
                                            <span class="campo-obrigatorio" title="Campo obrigatório">*</span>
                                        </h5>
                                </div>
                                <div class="ibox-content">
                                    <div id="div_formulario_detalhe">
                                        <?php if($podeEditar && $mBeneficiario->benid){
                                            $cBeneficiario->montarFormularioDetalhe($mBeneficiario->emeid, $mBeneficiario->benid, null);
                                        } ?>
                                    </div>
                                    <div id="div_listagem_detalhe">
                                        <?php $cBeneficiario->montarListagemDetalhe($mBeneficiario->benid, $podeEditar); ?>
                                    </div>
                                </div>
                            </div>
                        </div>

                        <div class="col-md-12 dados-siconv">
                            <div class="ibox float-e-margins">
                                <div class="ibox-title">
                                        <h5>Dados do SICONV</h5>
                                </div>
                                <div class="ibox-content">
                                    <?php $cBeneficiario->montarListagemSiconv($mBeneficiario->benid); ?>
                                </div>
                            </div>
                        </div>

                        <div class="clearfix"></div>
                    </div>
                </div>
                <div id="tab-1" class="tab-pane <?php echo $classTab1; ?>">
                    <div class="panel-body">

                        <form id="formulario-beneficiario-complemento" name="formulario-beneficiario-complemento" method="post" class="form-horizontal">

                            <div class="col-md-6">
                                <div class="ibox float-e-margins">
                                    <div class="ibox-title">
                                        <h5>Dados Gerais</h5>
                                    </div>
                                    <div class="ibox-content">
                                        <input type="hidden" name="req" id="req" value="salvar-complemento" />
                                        <input name="benid" id="benid" type="hidden" value="<?php echo $mBeneficiario->benid; ?>">
                                        <input name="emeid" id="emeid" type="hidden" value="<?php echo $mBeneficiario->emeid; ?>">
                                        <input name="aba" id="aba" type="hidden" value="<?php echo $aba; ?>">

                                        <?php
                                            echo $simec->input('plititulo', 'Título do PI/Projeto <span class="campo-obrigatorio" title="Campo obrigatório pra Gerar PI">*</span>', $mBeneficiario->plititulo, ['maxlength' => 230]);

                                            $descricaoDinamica = $mBeneficiario->benpropostaobjeto? $mBeneficiario->benpropostaobjeto: NULL;
                                            echo $simec->textarea('plidsc', 'Descrição / Finalidade <span class="campo-obrigatorio" title="Campo obrigatório pra Gerar PI">*</span>', $mBeneficiario->plidsc? $mBeneficiario->plidsc: $descricaoDinamica, ['maxlength' => 1000]);
//condicaoGerarPi($benid);
                                        ?>

                                        <div class="clearfix"></div>
                                    </div>
                                </div>
                            </div>

                            <div class="col-md-5">
                                <div class="ibox float-e-margins">
                                    <div class="ibox-title">
                                        <h5>Metas</h5>
                                    </div>
                                    <div class="ibox-content">
                                        <?php
                                            if($mBeneficiario->oppid){
                                                $oppidDinamico = $mBeneficiario->oppid;
                                            } else {
                                                $oppidDinamico = (new Public_Model_ObjetivoPpa())->buscarOppidPorFuncional($mEmenda->ptrid);
                                            }
                                            echo $simec->select('oppid', 'Objetivo PPA', $oppidDinamico, (new Public_Model_ObjetivoPpa())->recuperarSqlCombo(null, ['exercicio' => $_SESSION['exercicio']]));
                                            echo $simec->select('mppid', 'Metas PPA', $mBeneficiario->mppid, (new Public_Model_MetaPpa())->recuperarSqlCombo(null, ['exercicio' => $_SESSION['exercicio'], 'suoid' => $mBeneficiario->suoid, 'oppid' => $oppidDinamico]));
                                            echo $simec->select('ippid', 'Iniciativa PPA', $mBeneficiario->ippid, (new Public_Model_IniciativaPpa())->recuperarSqlCombo(null, ['exercicio' => $_SESSION['exercicio'], 'oppid' => $oppidDinamico]));
                                            echo $simec->select('mpnid', 'Meta PNC', $mBeneficiario->mpnid, (new Public_Model_MetaPnc())->recuperarSqlCombo(null, ['exercicio' => $_SESSION['exercicio'], 'suoid' => $mBeneficiario->suoid]));
                                            echo $simec->select('ipnid', 'Indicador PNC', $mBeneficiario->ipnid, (new Public_Model_IndicadorPnc())->recuperarSqlCombo(null, ['exercicio' => $_SESSION['exercicio'], 'mpnid' => $mBeneficiario->mpnid]));
                                        ?>
                                    </div>
                                </div>

                            </div>

                            <div class="col-md-1">
                                <?php wf_desenhaBarraNavegacao($mBeneficiario->docid, array('benid' => $mBeneficiario->benid)); ?>
                            </div>

                            <div class="col-md-6">
                                <div class="ibox float-e-margins">
                                    <div class="ibox-title">
                                        <h5>Vinculações Eventuais do Projeto</h5>
                                    </div>
                                    <div class="ibox-content">
                                        <div class="row">
                                            <div class="col-md-5">
                                                Números SNIIC
                                                <div class="form-group">
                                                    <div class="col-md-10" style="padding-right: 3px;">
                                                        <input type="text" placeholder="Adicione" value="" class="CampoEstilo inteiro normal form-control" title="Número SNIIC" id="input_sniic" maxlength="10" name="input_sniic" />
                                                    </div>
                                                    <div class="col-lg-1" style="padding-left: 0;">
                                                        <input type="button" id="btn_adicionar_sniic" value="+" class="btn btn-info" />
                                                    </div>
                                                </div>
                                                <div class="form-group">
                                                    <div class="col-lg-12">
                                                        <table id="table_sniic" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                                            <?php foreach($listaSniic as $sniic){ ?>
                                                                <tr style="height: 30px;" id="tr_sniic_<?php echo $sniic['sniid']; ?>" >
                                                                    <td style="text-align: left;"><?php echo $sniic['sninumero']; ?></td>
                                                                    <td style="text-align: center;">
                                                                        <input type="hidden" name="lista_sniic[]" value="<?php echo $sniic['sninumero']; ?>" />
                                                                        <span class="glyphicon glyphicon-trash link btnRemoveSniic" data-sniic="<?php echo $sniic['sniid']; ?>" ></span>
                                                                    </td>
                                                                </tr>
                                                            <?php } ?>
                                                        </table>
                                                    </div>
                                                </div>
                                            </div>
                                            <div class="col-md-7">
                                                Número Pronac
                                                <div class="form-group">
                                                    <div class="col-md-10" style="padding-right: 3px;">
                                                        <input type="text" placeholder="Adicione" value="" class="CampoEstilo normal form-control" title="Número PRONAC" id="input_pronac" name="input_pronac" />
                                                    </div>
                                                    <div class="col-lg-1" style="padding-left: 0;">
                                                        <input type="button" id="btn_adicionar_pronac" value="+" class="btn btn-info" />
                                                    </div>
                                                </div>
                                                <div class="form-group">
                                                    <div class="col-lg-12">
                                                        <table id="table_pronac" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                                            <?php foreach($listaPronac as $pronac){ ?>
                                                                <tr style="height: 30px;" id="tr_pronac_<?php echo $pronac['proid']; ?>" >
                                                                    <td style="text-align: left;"><?php echo $pronac['pronumero']; ?></td>
                                                                    <td style="text-align: center;">
                                                                        <input type="hidden" name="lista_pronac[]" value="<?php echo $pronac['pronumero']; ?>" />
                                                                        <span class="glyphicon glyphicon-trash link btnRemovePronac" data-pronac="<?php echo $pronac['proid']; ?>" ></span>
                                                                    </td>
                                                                </tr>
                                                            <?php } ?>
                                                        </table>
                                                    </div>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                </div>

                            </div>

                            <div class="col-md-5">
                                <div class="ibox float-e-margins">
                                    <div class="ibox-title">
                                        <h5>Produto PI</h5>
                                    </div>
                                    <div class="ibox-content">
                                        <?php
                                            echo $simec->select('pprid', 'Produto <span class="campo-obrigatorio" title="Campo obrigatório pra Gerar PI">*</span>', $mBeneficiario->pprid, (new Monitora_Model_PiProduto())->recuperarSqlCombo(null, ["pprstatus = 'A'"]));
                                            echo $simec->select('pumid', 'Unidade de Medida <span class="campo-obrigatorio" title="Campo obrigatório pra Gerar PI">*</span>', $mBeneficiario->pumid, (new Monitora_Model_PiUnidadeMedida())->recuperarSqlCombo(null, ["pumstatus = 'A'"]));
                                            echo $simec->input('picquantidade', 'Quantidade <span class="campo-obrigatorio" title="Campo obrigatório pra Gerar PI">*</span>', $mBeneficiario->picquantidade, ['class'=>'inteiro']);
                                        ?>

                                        <div class="clearfix"></div>
                                    </div>
                                </div>
                            </div>

                            <div class="col-md-6">
                                <div class="ibox float-e-margins">
                                    <div class="ibox-title">
                                        <h5>Anexos</h5>
                                    </div>
                                    <div class="ibox-content">
                                        <div class="form-group">
                                            <div class="col-lg-12">
                                                <table id="table_anexos" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                                    <tr>
                                                        <td style="text-align: left;"><b>Nome do arquivo</b></td>
                                                        <td style="text-align: left;"><b>Descrição</b></td>
                                                        <td style="text-align: center;"><b>Ação</b></td>
                                                    </tr>
                                                    <?php foreach($listaDeAnexos as $anexo){ ?>
                                                        <tr style="height: 30px;" class="tr_anexos_<?php echo $anexo['arqid']; ?>" >
                                                            <td style="text-align: left;"><a href="#" onclick="javascript:abrirArquivo('<?php echo $anexo['arqid']; ?>'); return false;" ><?php echo $anexo['arqnome']. '.'. $anexo['arqextensao']; ?></a></td>
                                                            <td style="text-align: left;"><?php echo $anexo['arqdescricao']; ?></td>
                                                            <td style="text-align: center;">
                                                                <input type="hidden" value="<?php echo $anexo['arqid']; ?>" name="listaAnexos[]">
                                                                <span class="glyphicon glyphicon-trash btnRemoverAnexos link" title="Excluir o arquivo <?php echo $anexo['arqnome']. '.'. $anexo['arqextensao']; ?>" data-anexos="<?php echo $anexo['arqid']; ?>" />
                                                            </td>
                                                        </tr>
                                                    <?php } ?>
                                                </table>
                                            </div>
                                        </div>
                                        <div class="form-group text-right">
                                            <div class="col-lg-12">
                                                <input type="button" id="btn_inserir_anexos" title="Inserir Anexos" value="Inserir Anexos" class="btn btn-info"/>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            </div>

                            <div class="col-md-12">
                                <div class="form-group">
                                    <div class="text-center">
                                        <button class="btn btn-primary" type="submit">
                                            <i class="fa fa-check"></i>&nbsp;Salvar
                                        </button>
                                        <a href="?modulo=principal/emenda&acao=A" class="btn btn-warning" id="btn-voltar" type="button">
                                            <i class="fa fa-arrow-left"></i>&nbsp;Voltar
                                        </a>
                                        <a href="?modulo=principal/emenda_form&acao=A&emeid=<?php echo $mBeneficiario->emeid; ?>" class="btn btn-success" id="btn-voltar" type="button">
                                            <i class="fa fa-file"></i>&nbsp;Ver Emenda
                                        </a>
                                    </div>
                                </div>
                            </div>

                        </form>

                    </div>
                </div>
            </div>
        </div>

    </div>
</div>

<div class="row">
    <div class="col-md-12">
        <div id="modal_upload" class="modal fade">
            <div class="modal_dialog_upload">
                <div class="modal-content">
                    <div class="modal-header">
                        <button class="close" aria-hidden="true" data-dismiss="modal" type="button">Ã—</button>
                        <h4 class="modal-title">Anexar arquivos(Documentos)</h4>
                    </div>
                    <div class="modal-body">
                        <form class="dropzone" method="POST" enctype="multipart/form-data" action="emendas.php?modulo=principal/beneficiario_form&acao=A&benid=<?php echo $mBeneficiario->benid; ?>&aba=1&req=uploadAnexo" id="formularioAnexo" name="formularioAnexo">
                            <div class="form-group">
                                <label for="arqdescricao" class="col-lg-2 control-label">Descrição:</label>
                                <div class="col-lg-10">
                                    <input type="text" value="" class="CampoEstilo normal form-control" placeholder="Insira a descrição do arquivo." title="Descrição" id="arqdescricao" name="arqdescricao" maxlength="255" size="2">
                                </div>
                            </div>
                            <div class="fallback">
                                <input name="file" type="file" multiple />
                            </div>
                        </form>
                    </div>
                    <div class="modal-footer">
                        <button class="btn btn-default" data-dismiss="modal" type="button">Fechar</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<?php bootstrapPopup("Histórico de Alterações",'historico-beneficiario',null, array('fechar'), array('tamanho' => 'lg')); ?>

<script src="js/principal/beneficiario_form.js?v=5"></script>

<script>
    $(function(){
        beneficiario_form_init();
    });
</script>
