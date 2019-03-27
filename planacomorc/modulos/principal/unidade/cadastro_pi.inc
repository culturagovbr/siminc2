<?php

/**
 * Tela de cadastro de Plano de Ação
 * @see cadastro.inc
 * $Id: cadastro_pi.inc 101265 2015-08-14 13:05:26Z maykelbraz $
 */

/**
 * Funções de apoio ao cadastro de Plano de Ação.
 * @see _funcoespi.php
 */
include_once '_funcoespi.php';

require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "planacomorc/classes/Cronograma.class.inc";
include_once APPRAIZ . "planacomorc/classes/Pi_Sniic.class.inc";
include_once APPRAIZ . "planacomorc/classes/Pi_Localizacao.class.inc";
include_once APPRAIZ . "planacomorc/classes/Pi_Convenio.class.inc";
include_once APPRAIZ . "planacomorc/classes/Pi_Responsavel.class.inc";
include_once APPRAIZ . "planacomorc/classes/Pi_Acao.class.inc";
include_once APPRAIZ . "planacomorc/classes/Pi_Cronograma.class.inc";
include_once APPRAIZ . "planacomorc/classes/Pi_Anexo.class.inc";
include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

require_once APPRAIZ. 'proposta/classes/model/PrePlanoInterno.inc';

if($_REQUEST['uploadAnexo']){
    $_FILES['file']['name'] = utf8_decode($_FILES['file']['name']);
    $_REQUEST['arqdescricao'] = utf8_decode($_REQUEST['arqdescricao']);
    $file = new FilesSimec();
    if($file->setUpload($_REQUEST['arqdescricao'], '', false)){
        echo simec_json_encode(array(
            'arqid' => $file->getIdArquivo(),
            'arqnome' => $_FILES['file']['name'],
            'arqdescricao' => $_REQUEST['arqdescricao']
        ));
        die();
    } else {
        echo simec_json_encode(array(
            'error' => 1,
            'errorMensage' => 'Não foi possível enviar o arquivo!'
        ));
        die();
    }
}

$urlModulo = '?modulo=principal/unidade/cadastro_pi&acao=A&aba=1';

# Requisições
include_once APPRAIZ. "acompanhamento/modulos/principal/monitorar-pi-request.inc";

if ($_REQUEST['carregarMetasEstrategicas']) {
    echo $simec->select('meeid', 'Meta Estratégica', $dadosPI['meeid'], (new Planacomorc_Model_MetaEstrategica())->recuperarSqlCombo(['meecod', 'meenome'], ["obeid = '".$_REQUEST['obeid']."'"], 'meecod'));
    die();
}
if ($_REQUEST['carregarDiretrizEstrategicas']) {
    echo $simec->select('dieid', 'Diretriz Estratégica', $dadosPI['dieid'], (new Planacomorc_Model_DiretrizEstrategica())->recuperarSqlCombo(['diecod', 'dienome'], ["obeid = '".$_REQUEST['obeid']."'"], 'diecod'));
    die();
}
if ($_REQUEST['carregarComboUG']) {
    echo carregarComboUG($_REQUEST['unicod']);
    die();
}
if ($_REQUEST['carregarMetasPPA']) {
    carregarMetasPPA((int)$_REQUEST['oppid'], null, $_REQUEST['suocod']);
    die();
}
if ($_REQUEST['carregarMetaPNC']) {
    carregarMetaPNC($_REQUEST['suocod'], $_REQUEST['mpnid']);
    die();
}
if ($_REQUEST['carregarIndicadorPNC']) {
    carregarIndicadorPNC((int)$_REQUEST['mpnid'], null);
    die();
}
if ($_REQUEST['carregarSegmentoCultural']) {
    carregarSegmentoCultural((int)$_REQUEST['mdeid'], null);
    die();
}
if ($_REQUEST['carregarManutencaoItem']) {
    carregarManutencaoItem((int)$_REQUEST['eqdid'], $_REQUEST['maiid']);
    die();
}
if ($_REQUEST['carregarManutencaoSubItem']) {
    carregarManutencaoSubItem((int)$_REQUEST['maiid'], null);
    die();
}
if ($_REQUEST['carregarLimitesUnidade']) {
    $autorizado = carregarLimiteAutorizadoSubUnidade(
        (object) array(
            'ungcod' => $_REQUEST['ungcod']
    ));
    $detalhadoSubUnidade = carregarLimiteDetalhadoSubUnidade(
        (object) array(
            'ungcod' => $_REQUEST['ungcod']
    ));
    $disponivel = ($autorizado - $detalhadoSubUnidade);
    echo json_encode(
        array(
            'autorizado' => number_format($autorizado, 2, ',', '.'),
            'disponivel' => number_format($disponivel, 2, ',', '.')
    ));
    die();
}
if ($_REQUEST['carregarIniciativaPPA']) {
    carregarIniciativaPPA((int)$_REQUEST['oppid'], null, $_REQUEST['suoid']);
    die();
}
if ($_REQUEST['carregarComboSubacaoFederais']) {
    carregarComboSubacaoUO($_REQUEST['unicod']);
    die();
}
if ($_REQUEST['sbaAjax']) {
    echo buscaDadosSubacao($_REQUEST['sbaAjax'], $_REQUEST['capidAjax']);
    die();
}
if ($_REQUEST['recuperarObjetivoPorPtres']) {
    echo recuperarObjetivoPorPtres($_REQUEST['ptrid']);
    die();
}
if ($_REQUEST['verificarPactuacaoConvenio']) {
    echo verificarPactuacaoConvenio($_REQUEST['capid']);
    die();
}
if ($_REQUEST['alterarCodigoPi']) {
    $resposta = simec_json_encode(alterarCodigoPi($_REQUEST));
    echo $resposta;
    die();
}

/**
 * Processa as requições enviadas a esta página.
 * @see cadastropi_requisicoes.inc
 */
require(APPRAIZ. 'planacomorc/modulos/principal/unidade/cadastropi_requisicoes.inc');

// -- Identificando se o usuário atual faz parte do CGSO/CPMO
$pulaSolicitacao = pulaSolicitacao();

// -- Definições iniciais das variáveis de controle do formulário
$habilitado = 'S';
$obrigatorio = 'N';
$ehSolicitacao = true;
$dadosPI = array();

$perfis = pegaPerfilGeral();

$listaDeAnexos = $aSniic = $aSei = $aPronac = $aConvenio = $aLocalizacao = $aResponsavel = [];
if (validaRequisicao('pliid')) { // -- Edição de PI

    $pliid = $_REQUEST['pliid'];

    // -- Carrega dados do PI e libera apenas o titulo e a descrição para edição
    $dadosPI = carregarPI($_REQUEST['pliid']);
    $enquadramento_pi = carregarEnquadramentoPI($_REQUEST['pliid']);
//    ver($dadosPI,d);
    $habilitado = 'N';
    $ehSolicitacao = false;
    $obrigatorio = 'S';

    $aSniic = (new Pi_Sniic())->recuperarTodos('*', array('pliid = ' . $pliid));
    $aSei = (new Planacomorc_Model_PiSei())->recuperarTodos('*', array('pliid = ' . $pliid));
    $aPronac = (new Planacomorc_Model_PiPronac())->recuperarTodos('*', array('pliid = ' . $pliid));
    $aConvenio = (new Pi_Convenio())->recuperarTodos('*', array('pliid = ' . $pliid));
    $aLocalizacao = (new Pi_Localizacao())->recuperarPorPlanoInterno($pliid, $dadosPI['esfid']);
    $aResponsavel = (new Pi_Responsavel())->recuperarPorPlanoInterno($pliid);
    $listaDeAnexos = (new Pi_Anexo())->recuperarPorPlanoInterno($pliid);
    $listaSubUnidadesDelegadas = (new Planacomorc_Model_PiDelegacao())->recuperarIdPorPlanoInterno($pliid, $_SESSION['exercicio']);

    // -- Requisição de apagar transação - Não exibe os botões do rodapé, e nem valida o formulário
    if (validaRequisicao('apagar')) {
        $tipoTransacao = '-';
    }
} elseif (validaRequisicao('scpid')) { // -- Transações de cadastro de PI
    // -- Carrega os dados da transação de criação e, de acordo com o tipo:
    // -- E: apenas exibe os dados - read only de uma transação já executada
    // -- S: deixa o formulário aberto para edição e liga as regras de validação - edição de
    // -- informações para confirmar a transação.
    $dadosPI = carregarTransacao($_REQUEST['scpid']);
}
elseif($_REQUEST['replicarProposta'])
{
    $idPrePi = $_REQUEST['replicarProposta'];
    $modelPrePi = new Proposta_Model_PrePlanoInterno((int)$idPrePi);
    $dadosPI = $modelPrePi->adaptarPiPropostaPlanejamento();
    $aLocalizacao = $modelPrePi->adaptarLocalizacaoPiPropostaPlanejamento();
}
/* Habilita a Edição para Super Usuário */
if ($db->testa_superuser()) {
    $habilitado = 'S';
}

if ($dadosPI['pliid'] && !$dadosPI['docid']) {
    $dadosPI['docid'] = pegarDocidPi($dadosPI['pliid'], WF_TPDID_PLANEJAMENTO_PI);
}
$estadoAtual = wf_pegarEstadoAtual($dadosPI['docid']);
$codigoEnquadramentoFinalistico = buscarCodigoEnquadramento((int)$_SESSION['exercicio'], 'F');
$codigoEnquadramentoNaoOrcamentario = buscarCodigoEnquadramento((int)$_SESSION['exercicio'], 'N');
$codigoEnquadramentoEmenda = buscarCodigoEnquadramento((int)$_SESSION['exercicio'], 'E');
$codigoProdutoNaoAplica = buscarCodigoProdutoNaoAplica((int)$_SESSION['exercicio']);
$listaEqdidReduzido = simec_json_encode(buscarListaCodigoEnquadramentoReduzido((int)$_SESSION['exercicio']));

$mPiPlanoInterno = new Pi_PlanoInterno();
$podeEditar = $mPiPlanoInterno->verificarPermissaoEditar($estadoAtual, $perfis);
if ($_REQUEST['req']){
    switch ($_REQUEST['req']) {
        case 'espelho-pedido':
            include_once APPRAIZ.'alteracao/modulos/principal/popup-espelho-pedido.inc';
            die;
        case 'listar-etapas':
            include_once APPRAIZ.'planacomorc/modulos/principal/unidade/lista_etapas.inc';
            die;
        case 'salvar-etapa':
            $cEtapas = new Planacomorc_Controller_Etapas();
            echo $cEtapas->salvar($_REQUEST);
            die;
        case 'excluir-etapa':
            $cEtapas = new Planacomorc_Controller_Etapas();
            echo $cEtapas->excluir($_REQUEST['etaid']);
            die;            
        default:
            break;
    }
}
# Selecionando Aba pelo parametro do URL da pagina
$aba = $_REQUEST['aba'];
$classTab0 = empty($aba)? 'active': NULL;
$classTab1 = $aba? 'active': NULL;
$classTab2 = $aba? 'active': NULL;

/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

?>
<script language="JavaScript" src="../includes/funcoes.js?v=9"></script>
<script language="JavaScript" src="../includes/funcoesspo.js?v=9"></script>
<link rel="stylesheet" href="/library/bootstrap-switch/stylesheets/bootstrap-switch.css?v=9">
<script src="/library/bootstrap-switch/js/bootstrap-switch.min.js?v=9"></script>
<script src="js/cadastro_pi_funcoes.js?v=19"></script>
<script src="js/cadastro_pi_validacoes.js?v=19"></script>
<script src="js/cadastro_pi_init.js?v=19"></script>
<style>
    autorizadocapital {
        display: none;
    }
    
    autorizadocusteio {
        display: none;
    }
</style>

<?php controlarAvisoPedidoAlteracao($dadosPI['pliid']); ?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-5">
        <h2><?php
            echo $titulo_modulo;
            if(!empty($dadosPI['plicod'])){ ?>
                <span style="color: red; padding: 5px;" id="span-plicod"><?= strtoupper($dadosPI['plicod']) ?></span>
                <input class="upper-text" maxlength="11" type="text" id="plicod" value="<?= $dadosPI['plicod'] ?>" style="display:none;">
                <a style="margin-left: 10px;" class="fa fa-history" id="modal-historico-pi" title="Histórico de alterações"></a>
            <?php } ?>
        </h2>
        <?php echo $dadosPI['pliid'] ? "<span>Id: {$dadosPI['pliid']}</span>" : ''; ?>
        <?php echo $dadosPI['emenumero'] ? "<br /><span>Emenda: {$dadosPI['emenumero']}</span>": ''; ?>
        <?php echo $dadosPI['benid'] ? "<br /><span>Beneficiário: {$dadosPI['benid']}</span>": ''; ?>
    </div>
    <div class="col-lg-7 div-proposta-pa">
        <div style="padding-top: 10px;" class="text-right">
            <a class="btn btn-danger" href="planacomorc.php?modulo=principal/unidade/listar_propostas_pi&acao=A" title="Criar um Plano de Ação a partir de uma proposta">
                <i class="fa fa-plus-circle"></i> Criar PA por Proposta
            </a>
        </div>
    </div>
</div>

<?php bootstrapPopup("Histórico de Alterações", 'historico-pi', APPRAIZ. "planacomorc/modulos/principal/unidade/historico_pi_modal.inc", array('fechar'), array('tamanho' => 'lg')); ?>
<?php bootstrapPopup("Registro do último usuário que alterou este PA", 'historico-pi-usuario', APPRAIZ. "planacomorc/modulos/principal/unidade/historico_pi_usuario_modal.inc", array('fechar'), array('tamanho' => 'lg')); ?>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">

        <div class="tabs-container col-lg-12">
            <ul class="nav nav-tabs">
                <li class="<?php echo $classTab0; ?>">
                    <a data-toggle="tab" href="#tab-0">Plano de Ação( Projeto )</a>
                </li>
                <?php if( (int)$estadoAtual['esdid'] === (int)ESD_PI_APROVADO): ?>
                    <li class="<?php echo $classTab1; ?>">
                        <a data-toggle="tab" href="#tab-1">Monitoramento</a>
                    </li>
                    <li class="<?= $classTab2; ?> aba-alteracoes">
                        <a data-toggle="tab" href="#tab-2">Alterações Orçamentárias</a>
                    </li>
                <?php endif; ?>
            </ul>
            <div class="tab-content">
                <div id="tab-0" class="tab-pane <?php echo $classTab0; ?>">
                    <div class="panel-body">

                        <form id="formulario" name="formulario" method="post" action="" class="form-horizontal">
                            <input type="hidden" name="evento" id="evento" value="S" />
                            <input type="hidden" name="pliid" id="pliid" value="<?= $_REQUEST['pliid']; ?>" />
                            <input type="hidden" name="tipotransacao" id="tipotransacao" value="E" />
                            <input type="hidden" name="scpid" id="scpid" value="<?php echo $_REQUEST['scpid']; ?>" />

                            <div class="col-md-12">
                                <?php if($_GET['apagar']): ?>
                                    <div class="form-group">
                                        <div class="col-lg-12">
                                            <input type="button" class="btn btn-danger" id="btnApagar" value="APAGAR" />
                                        </div>
                                    </div>
                                <?php endif; ?>
                            </div>
                            <div class="row">
                                <?php include_once APPRAIZ.'planacomorc/modulos/principal/unidade/cadastro_pi_identificacao.inc';?>
                            </div>
                            <div class="row">
                                <?php include_once APPRAIZ.'planacomorc/modulos/principal/unidade/cadastro_pi_detalhamento.inc';?>
                            </div>
                            <div class="row">
                                <?php include_once APPRAIZ.'planacomorc/modulos/principal/unidade/cadastro_pi_metas_produto.inc';?>
                            </div>
                            <div class="row">
                                <?php include_once APPRAIZ.'planacomorc/modulos/principal/unidade/cadastro_pi_informacoes_etapas.inc';?>
                            </div>
                            <div class="row">                            
                                <?php include_once APPRAIZ.'planacomorc/modulos/principal/unidade/cadastro_pi_localizacao_responsavel.inc';?>
                            </div>
                            <div class="row">
                                <?php include_once APPRAIZ.'planacomorc/modulos/principal/unidade/cadastro_pi_vinculacoes.inc';?>
                            </div>
                            <div class="clearfix"></div>
                            <div class="row">
                                <?php include_once APPRAIZ.'planacomorc/modulos/principal/unidade/cadastro_pi_custeio_anexos_cronograma.inc';?>
                            </div>
                        </form>
                    <?php if (!(array_intersect([PFL_CONSULTA,PFL_CONSULTA_UNIDADE], $perfis))) {?>
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="button" name="btg" onclick="submeter('<?php echo $plicod; ?>//');" <?php echo $disabled; ?>><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="planacomorc.php?modulo=principal/unidade/listapimanter&acao=A" class="btn btn-warning" id="btnVoltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                            </div>
                        </div>
                    <?php }?>
                    </div>
                </div>

                <?php if( (int)$estadoAtual['esdid'] === (int)ESD_PI_APROVADO): ?>
                    <div id="tab-1" class="tab-pane <?php echo $classTab1; ?>">
                        <div class="panel-body">

                            <?php include_once APPRAIZ. "acompanhamento/modulos/principal/monitorar-pi-cad.inc"; ?>

                        </div>
                    </div>
                    <div id="tab-2" class="tab-pane <?php echo $classTab1; ?>">
                        <div class="panel-body">

                            <div class="row">
                                <div class="col-md-12">
                                    <div class="ibox float-e-margins">
                                        <div class="ibox-title">
                                            <h5>Lista de Pedidos de Alterações Orçamentárias</h5>
                                        </div>
                                        <div class="ibox-content">
                                            <div id="div-principal">
                                                <?php
                                                    $filtroalteracao = ['pliid' => $dadosPI['pliid']];
                                                    $permiteEditar = false;
                                                    include_once APPRAIZ.'alteracao/modulos/principal/lista_incio.inc';
                                                ?>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            </div>

                        </div>
                    </div>
                <?php endif; ?>

            </div>
        </div>

    </div> <!-- div row -->
</div> <!-- div principal -->

<div class="row">
    <div class="col-md-12">
        <div id="modal-ptres" class="modal fade">
            <div class="modal-dialog-ptres">
                <div class="modal-content">
                    <div class="modal-header">
                        <button class="close" aria-hidden="true" data-dismiss="modal" type="button">×</button>
                        <h4 class="modal-title">Seleção de PTRES</h4>
                    </div>
                    <div class="modal-body">
                        <p></p>
                    </div>
                    <div class="modal-footer">
                        <button class="btn btn-default" data-dismiss="modal" type="button">Fechar</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<div class="row">
    <div class="col-md-12">
        <div id="modal_responsaveis" class="modal fade">
            <div class="modal_dialog_responsaveis">
                <div class="modal-content">
                    <div class="modal-header">
                        <button class="close" aria-hidden="true" data-dismiss="modal" type="button">×</button>
                        <h4 class="modal-title">Seleção de Responsáveis</h4>
                    </div>
                    <div class="modal-body">
                        <p></p>
                    </div>
                    <div class="modal-footer">
                        <button class="btn btn-default" data-dismiss="modal" type="button">Fechar</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<div class="row">
    <div class="col-md-12">
        <div id="modal_localizacao" class="modal fade">
            <div class="modal_dialog_localizacao">
                <div class="modal-content">
                    <div class="modal-header">
                        <button class="close" aria-hidden="true" data-dismiss="modal" type="button">×</button>
                        <h4 class="modal-title">Seleção de Localizações Municipais</h4>
                    </div>
                    <div class="modal-body">
                        <p></p>
                    </div>
                    <div class="modal-footer">
                        <button class="btn btn-default" data-dismiss="modal" type="button">Fechar</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<div class="row">
    <div class="col-md-12">
        <div id="modal_localizacao_estadual" class="modal fade">
            <div class="modal_dialog_localizacao_estadual">
                <div class="modal-content">
                    <div class="modal-header">
                        <button class="close" aria-hidden="true" data-dismiss="modal" type="button">×</button>
                        <h4 class="modal-title">Seleção de Localizações Estaduais</h4>
                    </div>
                    <div class="modal-body">
                        <p></p>
                    </div>
                    <div class="modal-footer">
                        <button class="btn btn-default" data-dismiss="modal" type="button">Fechar</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<div class="row">
    <div class="col-md-12">
        <div id="modal_localizacao_exterior" class="modal fade">
            <div class="modal_dialog_localizacao_exterior">
                <div class="modal-content">
                    <div class="modal-header">
                        <button class="close" aria-hidden="true" data-dismiss="modal" type="button">×</button>
                        <h4 class="modal-title">Seleção de Localizações no Exterior</h4>
                    </div>
                    <div class="modal-body">
                        <p></p>
                    </div>
                    <div class="modal-footer">
                        <button class="btn btn-default" data-dismiss="modal" type="button">Fechar</button>
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
                        <button class="close" aria-hidden="true" data-dismiss="modal" type="button">×</button>
                        <h4 class="modal-title">Anexar arquivos(Documentos)</h4>
                    </div>
                    <div class="modal-body">
                        <form class="dropzone" method="POST" enctype="multipart/form-data" action="planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A&uploadAnexo=1" id="formularioAnexoPlanacomorc" name="formularioAnexoPlanacomorc">
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

<script type="text/javascript">
    // Enquadramentos
    intEnqFinalistico = "<?php echo $codigoEnquadramentoFinalistico; ?>";
    intEnqNaoOrcamentario = "<?php echo $codigoEnquadramentoNaoOrcamentario; ?>";
    intEnqEmenda = "<?php echo $codigoEnquadramentoEmenda; ?>";
    intProdNaoAplica = "<?php echo $codigoProdutoNaoAplica; ?>";
    // Localizações
    intEsfidEstadualDF = "<?php echo ESFERA_ESTADUAL_DISTRITO_FEDERAL; ?>";
    intEsfidExterior = "<?php echo ESFERA_EXTERIOR; ?>";
    intEsfidFederalBrasil = "<?php echo ESFERA_FEDERAL_BRASIL; ?>";
    intEsfidMunicipal = "<?php echo ESFERA_MUNICIPAL; ?>";
    // Manutenção item
    intMaiid = '<?php echo $dadosPI['maiid']; ?>';

    listaEqdReduzido = <?php echo $listaEqdidReduzido; ?>;

    // Permissão Admin
    isAdmin = '<?php echo (in_array(PFL_ADMINISTRADOR, $perfis) || in_array(PFL_SUPERUSUARIO, $perfis)); ?>';

    // Permissão para Editar
    podeEditar = '<?php echo $podeEditar; ?>';
    
    fnc = false;
    
    urlPagina = '?modulo=principal/unidade/cadastro_pi&acao=A';
    
    ppiid = '<?php echo isset($_REQUEST['replicarProposta'])? $_REQUEST['replicarProposta']: 0; ?>';

    $(document).ready(function() {

        // Ações efetuadas quando a tela de cadastro de PI é iniciada.
        initCadastroPi();

    });

</script>