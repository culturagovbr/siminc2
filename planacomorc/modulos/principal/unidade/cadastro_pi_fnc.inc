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

$sufixoOrigemRequisicao = '_fnc';

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

if($_REQUEST['uploadAnexo']){
    $file = new FilesSimec();
    if($file->setUpload($_REQUEST['arqdescricao'], '', false)){
        echo simec_json_encode(array(
            'arqid' => $file->getIdArquivo(),
            'arqnome' => $_FILES['file']['name'],
            'arqdescricao' => $_REQUEST['arqdescricao']
        ));
        die();
    }else{
        echo simec_json_encode(array(
            'error' => 1,
            'errorMensage' => 'Não foi possível enviar o arquivo!'
        ));
        die();
    }
}
# Requisições
include_once APPRAIZ. "acompanhamento/modulos/principal/monitorar-pi-request.inc";
if ($_REQUEST['carregarComboUG']) {
    echo carregarComboUG($_REQUEST['unicod'], null, $_REQUEST['fnc']);
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
    # Busca limite da Unidade
    $autorizado = carregarLimiteAutorizadoUnidadeFnc(
        (object) array(
            'exercicio' => $_SESSION['exercicio']
    ));
    # Busca limite já detalhado e aprovado na Unidade
    $detalhadoSubUnidade = carregarLimiteDetalhadoUnidadeFnc(
        (object) array(
            'exercicio' => $_SESSION['exercicio']
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

require(APPRAIZ . 'planacomorc/modulos/principal/unidade/cadastropi_requisicoes.inc');

// -- Identificando se o usuário atual faz parte do CGSO/CPMO
$pulaSolicitacao = pulaSolicitacao();

// -- Definições iniciais das variáveis de controle do formulário
$habilitado = 'S';
$obrigatorio = 'N';
$ehSolicitacao = true;
$dadosPI = array();

$perfis = pegaPerfilGeral();

$sqlListaUO = Spo_Model_Unidade::queryCombo((object)array(
    'prsano::INTEGER = '. (int)$_SESSION['exercicio'],
    'suo.unofundo = TRUE'));
$listaUO = $db->carregar($sqlListaUO);

$listaUO = $listaUO ? $listaUO : [0];

if($listaUO){
    $uoFnc = current($listaUO);
}

$codigoUoFnc = $uoFnc['codigo'];

// ******* Ao carregar a listagem de PTRES, sempre filtrar pela UO selecionada, independentemente do Perfil ; *****
/*if (in_array(PFL_GESTAO_ORCAMENTARIA_IFS, $perfis)) {
    $sqlUO = <<<DML
EXISTS (SELECT 1
         FROM planacomorc.usuarioresponsabilidade rpu
         WHERE rpu.usucpf = '%s'
           AND rpu.pflcod = %d
           AND rpu.rpustatus = 'A'
           AND rpu.unicod  = uni.unicod)
DML;
    $where[] = $whereUO = sprintf($sqlUO, $_SESSION['usucpf'], PFL_GESTAO_ORCAMENTARIA_IFS);
    $whereUO = " AND {$whereUO}";
}*/

$listaDeAnexos = $aSniic = $aSei = $aPronac = $aConvenio = $aLocalizacao = $aResponsavel = [];
if (validaRequisicao('pliid')) { // -- Edição de Plano de Ação

    $pliid = $_REQUEST['pliid'];

    // -- Carrega dados do Plano de Ação e libera apenas o titulo e a descrição para edição
    $dadosPI = carregarPI($_REQUEST['pliid']);
    $enquadramento_pi = carregarEnquadramentoPI($_REQUEST['pliid']);
    //ver($dadosPI,d);
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
} elseif (validaRequisicao('scpid')) { // -- Transações de cadastro de Plano de Ação
    // -- Carrega os dados da transação de criação e, de acordo com o tipo:
    // -- E: apenas exibe os dados - read only de uma transação já executada
    // -- S: deixa o formulário aberto para edição e liga as regras de validação - edição de
    // -- informações para confirmar a transação.
    $dadosPI = carregarTransacao($_REQUEST['scpid']);
} elseif ($_REQUEST['replicarProposta']) {
    $dadosPI = (new Pi_PlanoInterno())->recuperarDadosPropostasSiminc($_REQUEST['replicarProposta']);
}
/* Habilita a Edição para Super Usuário */
if ($db->testa_superuser()) {
    $habilitado = 'S';
}

if ($dadosPI['pliid'] && !$dadosPI['docid']) {
    $dadosPI['docid'] = pegarDocidPi($dadosPI['pliid'], WF_TPDID_FNC_PLANEJAMENTO_PI);
}
$estadoAtual = wf_pegarEstadoAtual($dadosPI['docid']);
$codigoEnquadramentoFinalistico = buscarCodigoEnquadramento((int)$_SESSION['exercicio'], 'F');
$codigoEnquadramentoNaoOrcamentario = buscarCodigoEnquadramento((int)$_SESSION['exercicio'], 'N');
$codigoProdutoNaoAplica = buscarCodigoProdutoNaoAplica((int)$_SESSION['exercicio']);
$listaEqdidReduzido = simec_json_encode(buscarListaCodigoEnquadramentoReduzido((int)$_SESSION['exercicio']));

$listaJanelasDeAlteracao = (new Planacomorc_Model_PiJanela)->buscarTodos((object) array(
    'prsano' => $_SESSION['exercicio'],
    'periodoVigente' => TRUE
));
$mPiPlanoInterno = new Pi_PlanoInterno();
$podeEditar = $mPiPlanoInterno->verificarPermissaoEditarFnc($estadoAtual, $perfis, $listaJanelasDeAlteracao);

/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="JavaScript" src="../includes/funcoesspo.js?v=1"></script>
<link rel="stylesheet" href="/library/bootstrap-switch/stylesheets/bootstrap-switch.css">
<script src="/library/bootstrap-switch/js/bootstrap-switch.min.js"></script>
<script src="js/cadastro_pi_funcoes.js?v=10"></script>
<script src="js/cadastro_pi_validacoes.js?v=9"></script>
<script src="js/cadastro_pi_init.js?v=11"></script>
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
    <div class="col-lg-10">
        <h2>
            Cadastro de Plano de Ação do FNC
            <?php if(!empty($dadosPI['plicod'])){ ?>
                <span style="color: red; padding: 5px;" id="span-plicod"><?= strtoupper($dadosPI['plicod']) ?></span>
                <input class="upper-text" maxlength="11" type="text" id="plicod" value="<?= $dadosPI['plicod'] ?> " style="display:none;">
                <a style="margin-left: 10px;" class="fa fa-history" id="modal-historico-pi" title="Histórico de alterações"></a>
            <?php } ?>
        </h2>
        <?php echo $dadosPI['pliid'] ? "<span>Id: {$dadosPI['pliid']}</span>" : ''; ?>
        <?php echo $dadosPI['emenumero'] ? "<br /><span>Emenda: {$dadosPI['emenumero']}</span>": ''; ?>
        <?php echo $dadosPI['benid'] ? "<br /><span>Beneficiário: {$dadosPI['benid']}</span>": ''; ?>
        <?php if(!$mPiPlanoInterno->verificarPermissaoEditarUnidade($perfis, $listaJanelasDeAlteracao)): ?>
            <h3>
                <span style="color: red; padding: 5px;">
                    Não há janelas de período vigente no momento para possibilitar a unidade cadastrar Plano de Ação.
                </span>
            </h3>
        <?php endif; ?>
    </div>
</div>

<?php bootstrapPopup("Histórico de Alterações", 'historico-pi', APPRAIZ. "planacomorc/modulos/principal/unidade/historico_pi_modal.inc", array('fechar'), array('tamanho' => 'lg')); ?>
<?php bootstrapPopup("Registro do último usuário que alterou este pi", 'historico-pi-usuario', APPRAIZ. "planacomorc/modulos/principal/unidade/historico_pi_usuario_modal.inc", array('fechar'), array('tamanho' => 'lg')); ?>

<div class="wrapper wrapper-content animated fadeInRight">

    <form id="formulario" name="formulario" method="post" action="" class="form-horizontal">
        <input type="hidden" name="evento" id="evento" value="S" />
        <input type="hidden" name="pliid" id="pliid" value="<?= $_REQUEST['pliid']; ?>" />
        <input type="hidden" name="tipotransacao" id="tipotransacao" value="E" />
        <input type="hidden" name="scpid" id="scpid" value="<?php echo $_REQUEST['scpid']; ?>" />

        <div class="row">

            <div class="col-md-12">

                <?php if ($_GET['apagar']): ?>
                    <div class="form-group">
                        <div class="col-lg-12">
                            <input type="button" class="btn btn-danger" id="btnApagar" value="APAGAR" />
                        </div>
                    </div>
                <?php endif; ?>
            </div>

            <div class="col-md-6">

                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Dados do Plano de Ação</h5>
                    </div>
                    <div class="ibox-content">

                        <div class="form-group">
                            <div class="col-md-3 text-right">
                                <label class="control-label" for="pijid">Reunião:</label>
                            </div>
                            <div class="col-lg-9">
                                <?php
                                    $janela = end($listaJanelasDeAlteracao);
                                    $sql = (new Planacomorc_Model_PiJanela)->montarSqlBuscarTodosCombo((object) array(
                                        'prsano' => $_SESSION['exercicio'],
//                                        'periodoVigente' => TRUE
                                    ));
                                    $pijid = $dadosPI['pijid']? $dadosPI['pijid']: $janela['pijid'];
                                    $db->monta_combo('pijid', $sql, (in_array(PFL_SUBUNIDADE, $perfis)? 'N': 'S'), 'Selecione', '', null, null, null, 'N', 'pijid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($pijid) ? $pijid : null));
                                ?>
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-md-3 text-right">
                                <label class="control-label" for="plititulo">Título: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                            </div>
                            <div class="col-lg-9">
                                <input type="text" value="<?php
                                if ($dadosPI['plititulo']) {
                                    echo $dadosPI['plititulo'];
                                }
                                ?>" class="CampoEstilo normal form-control obrigatorio" title="" id="plititulo" value="" maxlength="250" size="2" name="plititulo">
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-md-3 text-right">
                                <label class="control-label" for="plidsc">Descrição / Finalidade: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                            </div>
                            <div class="col-lg-9">
                                <textarea class="obrigatorio txareanormal form-control" rows="6" cols="60" name="plidsc" id="plidsc" maxlength="1000"><?php echo $dadosPI['plidsc']? $dadosPI['plidsc']: NULL; ?></textarea>
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-md-3 text-right">
                                <label class="control-label" for="picpublico">Público alvo:</label>
                            </div>
                            <div class="col-lg-9">
                                <textarea class="txareanormal form-control" rows="6" cols="60" name="picpublico" id="picpublico" maxlength="1000"><?php echo $dadosPI['picpublico']? $dadosPI['picpublico']: NULL; ?></textarea>
                            </div>
                        </div>
                    </div>
                </div>
            </div>

            <div class="col-md-5">

                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Identificação</h5>
                    </div>
                    <div class="ibox-content">

                        <div class="form-group">
                            <div class="col-md-3 text-right">
                                <label class="control-label" for="unicod">Unidade: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                            </div>
                            <div class="col-lg-9">
                                <?php
                                $obrigatorias = UNIDADES_OBRIGATORIAS;
                                // -- Se a requisição tiver um scpid, quer dizer que a tela foi aberta para homologar uma
                                // -- solicitação de PI, ou para ver uma transação executada anteriormente. Nos dois casos,
                                // -- o filtro de UOs deve exibir (ao menos) a UO escolhida no momento da criação da transação,
                                // -- portanto, não deve filtrar pela UO do usuário que a está visualizando.
                                // ******* Ao carregar a listagem de PTRES, sempre filtrar pela UO selecionada, independentemente do Perfil ; *****

                                $wherePerfilUO = $filtroPerfilUG = '';
                                if (in_array(PFL_SUBUNIDADE, $perfis)) {

                                    $sql = "SELECT distinct suo.suoid, suo.suocod, suo.unoid, suo.unocod, suo.unofundo
                                            FROM planacomorc.usuarioresponsabilidade ur
                                                    inner join public.vw_subunidadeorcamentaria suo on suo.suocod = ur.ungcod
                                            WHERE suo.suostatus = 'A'
                                            and usucpf = '{$_SESSION['usucpf']}'
                                            AND ur.pflcod = '" . PFL_SUBUNIDADE . "'
                                            and suo.prsano = '{$_SESSION['exercicio']}'
                                            and rpustatus = 'A'
                                            and unofundo = 't'
                                            ORDER BY suo.unocod";

                                    $dadosUsuario = $db->carregar($sql);

                                    $unidadeUsuario = [];
                                    if($dadosUsuario){
                                        foreach ($dadosUsuario as $dadoUsuario){
                                            $unidadeUsuario['unidades'][$dadoUsuario['unocod']] = $dadoUsuario['unocod'];
                                            $unidadeUsuario['subunidades'][$dadoUsuario['unocod']] = $dadoUsuario['unocod'];

                                            // Agrupando por FNC
                                            $unidadeUsuario['unocod'][$dadoUsuario['unofundo']][$dadoUsuario['unocod']] = $dadoUsuario['unocod'];
                                            $unidadeUsuario['suocod'][$dadoUsuario['unofundo']][$dadoUsuario['suocod']] = $dadoUsuario['suocod'];
                                        }

                                        $wherePerfilUO  = " and unocod in ('" . implode("', '", $unidadeUsuario['unidades']) . "')";

                                        if(empty($dadosPI['unicod'])){
                                            $dadosPI['unicod'] = (1 == count($unidadeUsuario['unocod']['t'])) ? current($unidadeUsuario['unocod']['t']) : '';
                                        }
                                        if(empty($dadosPI['ungcod'])){
                                            $dadosPI['ungcod'] = (1 == count($unidadeUsuario['suocod']['t'])) ? current($unidadeUsuario['suocod']['t']) : '';
                                        }
                                    }
                                }

                                $sql = "
                                    SELECT DISTINCT
                                        suo.unocod AS codigo,
                                        suo.unocod || ' - ' || suo.unonome AS descricao
                                    FROM public.vw_subunidadeorcamentaria suo
                                    WHERE
                                        suo.prsano = '{$_SESSION['exercicio']}'
                                        AND suo.suostatus = 'A'
                                        AND suo.unofundo = TRUE
                                        $wherePerfilUO
                                    ORDER BY
                                        descricao
                                ";
                                $editavelUg = ($dadosPI['unicod'] && $dadosPI['pliid']) ? 'N' : 'S';
                                $db->monta_combo('unicod', $sql, $editavelUg, '', null, null, null, null, 'N', 'unicod', null, (isset($dadosPI['unicod'])? $dadosPI['unicod']: current($listaUO)), null, 'class="form-control chosen-select" style="width=100%;"', null, null);
                                ?>
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-md-3 text-right">
                                <label class="control-label" for="ungcod">SubUnidade: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                            </div>
                            <div class="col-lg-9 div_ungcod">
                                <?php
                                $ungcod = $dadosPI['ungcod'];
                                carregarComboUG((isset($dadosPI['unicod'])? $dadosPI['unicod']: $codigoUoFnc), $editavelUg, 'TRUE');
                                ?>
                            </div>
                        </div>

                        <div class="form-group">
                            <div class="col-md-3 text-right">
                                <label class="control-label" for="picted">Delegação:</label>
                            </div>
                            <div class="col-md-1">
                                <input type="checkbox" name="radioDelegacao" id="radioDelegacao" value="t" class="js-switch" <?php echo count($listaSubUnidadesDelegadas) ? 'checked="checked"' : ''; ?> />
                            </div>
                            <div class="col-md-7" id="div_unidades_delegadas" style="margin-left: 15px;">
                                <?php
                                $aSubUnidadeOrcamentaria = simec_preparar_array((new Public_Model_SubUnidadeOrcamentaria())->recuperarTodos("suoid as codigo, unosigla || ' - ' || suonome as descricao", ["prsano = '{$_SESSION['exercicio']}'", "suostatus = 'A'"], 'descricao'));
                                echo $simec->select("delegacao[]", '', $listaSubUnidadesDelegadas, $aSubUnidadeOrcamentaria);
                                ?>
                            </div>
                        </div>

                        <div class="form-group">
                            <div class="col-md-3 text-right">
                                <label class="control-label" for="eqdid">Enquadramento da Despesa: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                            </div>
                            <div class="col-lg-9 carregaEnq">
                                <?php
                                    $sql = "
                                        SELECT
                                            eqdid AS codigo,
                                            eqddsc AS descricao
                                        FROM monitora.pi_enquadramentodespesa
                                        WHERE
                                            eqdstatus = 'A'
                                            AND eqdano = '{$_SESSION['exercicio']}'
                                        ORDER BY
                                            eqddsc
                                    ";
                                    $eqdid = $dadosPI['eqdid']? $dadosPI['eqdid']: $codigoEnquadramentoFinalistico;
                                    $db->monta_combo('eqdid', $sql, 'S', '', '', null, null, null, 'N', 'eqdid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, null);
                                ?>
                            </div>
                        </div>

                        <div class="form-group grupo_manutencao" style="display: none;">
                            <div class="col-md-3 text-right">
                                <label class="control-label" for="maiid">Item: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                            </div>
                            <div class="col-lg-9 div_maiid">
                                <?php carregarManutencaoItem($dadosPI['eqdid'], $dadosPI['maiid']); ?>
                            </div>
                        </div>
                        <div class="form-group grupo_manutencao" style="display: none;">
                            <div class="col-md-3 text-right">
                                <label class="control-label" for="masid">SubItem: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                            </div>
                            <div class="col-lg-9 div_masid">
                                <?php carregarManutencaoSubItem($dadosPI['maiid'], $dadosPI['masid']); ?>
                            </div>
                        </div>

                    </div>
                </div>
            </div>

            <div class="col-md-1">
                <a style="margin-left:0.8em; height:0.8em; padding-bottom:1.2em; font-size:3em;" class="fa fa-history" id="modal-historico-pi-usuario" title="Último usuário que salvou"></a>
                <?php wf_desenhaBarraNavegacao($dadosPI['docid'], array('pliid' => $dadosPI['pliid'])); ?>
                <div id="div_comentario_estado_atual" style="display: none;"><?php echo wf_pegarComentarioEstadoAtual($dadosPI['docid']); ?></div>
            </div>

            <div class="col-sm-12 div_ptres">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Detalhamento Orçamentário <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></h5>
                    </div>
                    <div class="ibox-content">

                        <div class="form-group">
                            <div class="col-lg-12">
                                <table cellpadding="5" border="0" width="98%" align="center" id="orcamento" style="border:#c9c9c9 1px solid" onmouseover="tabindexcampo();"  class="table table-hover table-bordered table-hover" >
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
                                            <b><span id="detPi">Aprovado em PI (R$)</span></b>
                                        </td>
                                        <td colspan="2">
                                            <b>Empenho (R$)</b>
                                        </td>
                                        <td rowspan="2" align="center">
                                            <b>Valor do Projeto</b>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td nowrap><b>Aprovado</b></td>
                                        <td nowrap><b>Disponível</b></td>
                                        <td nowrap><b>Empenhado</b></td>
                                        <td nowrap><b>Não Empenhado</b></td>
                                    </tr>
                                    <?php
                                        if ($dadosPI['pliid'] || $dadosPI['ptrid']) {
                                            $dadosPTRES = buscarPtresFnc((object) array(
                                                'exercicio' => $_SESSION['exercicio'],
                                                'pliid' => $dadosPI['pliid'],
                                                'importar' => ($_REQUEST['replicarProposta']? TRUE: FALSE)
                                            ));
// ver($dadosPTRES);
                                            if($dadosPTRES) {
                                                $valortotalpi = 0;
                                                $cor = 1;

                                                foreach ($dadosPTRES as $ptres):
                                                    $vlrPiDetalhado = strip_tags($ptres['det_pi']);
                                                    $vlrPiDetalhadoCusteio = strip_tags($ptres['det_pi_custeio']);
                                                    $vlrPiDetalhadoCapital = strip_tags($ptres['det_pi_capital']);
                                                    $vlrPtresCusteio = strip_tags($ptres['ptrdotacaocusteio']);
                                                    $vlrPtresCapital = strip_tags($ptres['ptrdotacaocapital']);
                                                    $vlrPtresCadastradoCusteio = strip_tags($ptres['cadastradocusteio']);
                                                    $vlrPtresCadastradoCapital = strip_tags($ptres['cadastradocapital']);
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
                                                    $cor++;
                                                    $valortotalpi = $valortotalpi + $ptres['pipvalor_'];
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
                                <input type="hidden" id="ptresCadastradoCusteio" value="<?php echo $vlrPtresCadastradoCusteio; ?>" />
                                <input type="hidden" id="ptresCadastradoCapital" value="<?php echo $vlrPtresCadastradoCapital; ?>" />
                                <input type="hidden" id="piNaoDetalhadoCusteio" value="<?php echo $vlrPiNaoDetalhadoCusteio; ?>" />
                                <input type="hidden" id="piNaoDetalhadoCapital" value="<?php echo $vlrPiNaoDetalhadoCapital; ?>" />
                                <input type="hidden" id="piNaoDetalhado" value="<?php echo $vlrPiNaoDetalhado; ?>" />
                                
                            </div>
                        </div>
                        <div class="form-group text-right">
                            <div class="col-lg-12">
                                <input type="button" id="btn_selecionar_functional" value="Selecionar Funcional/PTRES" class="btn btn-info" />
                            </div>
                        </div>

                    </div>
                </div>
            </div>

            <div class="col-md-6">

                <div class="ibox float-e-margins div_metas_ppa_pnc">
                    <div class="ibox-title">
                        <h5>Metas </h5>
                    </div>
                    <div class="ibox-content">
                        <div class="form-group">
                            <div class="col-md-3 text-right">
                                <label for="oppid" class=" control-label">Objetivo PPA: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                            </div>
                            <div class="col-lg-9">
                                <?php (new Public_Model_ObjetivoPpa())->monta_combo($dadosPI['oppid']); ?>
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-md-3 text-right">
                                <label for="mppid" class=" control-label">Metas PPA: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                            </div>
                            <div class="col-lg-9 div_mppid">
                                <?php carregarMetasPPA($dadosPI['oppid'], $dadosPI['mppid']); ?>
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-md-3 text-right">
                                <label for="ippid" class="control-label">Iniciativa PPA: </label>
                            </div>
                            <div class="col-lg-9 div_ippid">
                                <?php carregarIniciativaPPA($dadosPI['oppid'], $dadosPI['ippid']); ?>
                            </div>
                        </div>
                        <div class="form-group grupo_pnc">
                            <div class="col-md-3 text-right">
                                <label for="mpnid" class="control-label">Meta PNC: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                            </div>
                            <div class="col-lg-9 div_mpnid">
                                <?php carregarMetaPNC($dadosPI['ungcod'], $dadosPI['mpnid']); ?>
                            </div>
                        </div>
                        <div class="form-group grupo_pnc">
                            <div class="col-md-3 text-right">
                                <label for="ipnid" class="control-label">Indicador PNC: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                            </div>
                            <div class="col-lg-9 div_ipnid">
                                <?php carregarIndicadorPNC($dadosPI['mpnid'], $dadosPI['ipnid']); ?>
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-md-3 text-right">
                                <label for="ptaid" class="control-label">Aderência ao PT do FNC: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                            </div>
                            <div class="col-lg-9">
                                <?php carregarAderenciaPtFnc($dadosPI['ptaid']); ?>
                            </div>
                        </div>
                    </div>
                </div>

                <div class="ibox float-e-margins div_produto_pi">
                    <div class="ibox-title">
                        <h5>Produto PI:</h5>
                    </div>
                    <div class="ibox-content">
                        <div class="form-group">
                            <div class="col-md-3 text-right">
                                <label for="pprid" class="control-label">Produto: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                            </div>
                            <div class="col-lg-9">
                                <?php
                                    $sql = "
                                        SELECT
                                            pprid AS codigo,
                                            pprnome AS descricao
                                        FROM monitora.pi_produto
                                        WHERE
                                            prsano = '{$_SESSION['exercicio']}'
                                            AND pprstatus = 'A'
                                        ORDER BY
                                            descricao
                                    ";
                                    $pprid = $dadosPI['pprid'];
                                    $db->monta_combo('pprid', $sql, 'S', 'Selecione', '', null, null, null, 'N', 'pprid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($pprid)? $pprid: null));
                                ?>
                            </div>
                        </div>
                        <div class="form-group div_unidade_medida">
                            <div class="col-md-3 text-right">
                                <label for="pumid" class="control-label">Unidade de Medida: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                            </div>
                            <div class="col-lg-9">
                                <?php
                                    $sql = "
                                        SELECT
                                            pumid AS codigo,
                                            pumdescricao AS descricao
                                        FROM monitora.pi_unidade_medida
                                        WHERE
                                            prsano = '{$_SESSION['exercicio']}'
                                            AND pumstatus = 'A'
                                        ORDER BY
                                            descricao
                                    ";

                                    $pumid = !empty($dadosPI['pumid'])? $dadosPI['pumid']: '9';
                                    $db->monta_combo('pumid', $sql, 'S', 'Selecione', '', null, null, null, 'N', 'pumid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($pumid) ? $pumid: null));
                                ?>
                            </div>
                        </div>
                        <div class="form-group div_quantidade_produto">
                            <div class="col-md-3 text-right">
                                <label for="picquantidade" class="control-label">Quantidade: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                            </div>
                            <div class="col-lg-9">
                                <input type="text" value="<?php echo !empty($dadosPI['picquantidade']) ? $dadosPI['picquantidade'] : ''; ?>" class="CampoEstilo normal inteiro form-control obrigatorio" title="Quantidade" id="picquantidade" name="picquantidade" maxlength="50" />
                            </div>
                        </div>
                    </div>
                </div>

                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Informações Complementares:</h5>
                    </div>
                    <div class="ibox-content">
                        <div class="form-group" id="div_area_cultural">
                            <div class="col-md-3 text-right">
                                <label for="mdeid" class="control-label">Área Cultural: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                            </div>
                            <div class="col-lg-9">
                                <?php
                                $sql = <<<DML
                                    SELECT
                                        MAX(mdeid) AS codigo,
                                        upper(public.removeacento(mdedsc)) AS descricao
                                    FROM monitora.pi_modalidadeensino
                                    WHERE
                                        mdestatus = 'A'
                                        AND mdeano = '{$_SESSION['exercicio']}' -- Resolver 2014
                                    GROUP BY
                                        descricao
                                    ORDER BY
                                        descricao ASC
DML;

                                $mdeid = $dadosPI['mdeid'];
                                $db->monta_combo('mdeid', $sql, 'S', 'Selecione', '', null, null, null, 'N', 'mdeid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($mdeid) ? $mdeid : null));
                                ?>
                            </div>
                        </div>
                        <div class="form-group" id="div_segmento_cultural">
                            <div class="col-md-3 text-right">
                                <label class="control-label" for="neeid">Segmento Cultural: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                            </div>
                            <div class="col-lg-9 div_neeid">
                                <?php carregarSegmentoCultural($dadosPI['mdeid'], $dadosPI['neeid']); ?>
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-md-3 text-right">
                                <label class="control-label" for="capid">Modalidade de Pactuação:</label>
                            </div>
                            <div class="col-lg-9">
                                <?php
                                $sql = <<<DML
                            SELECT
                                capid AS codigo,
                                capdsc AS descricao
                            FROM monitora.pi_categoriaapropriacao
                            WHERE
                                capano = '{$_SESSION['exercicio']}'
                                AND capstatus = 'A'
                            ORDER BY
                                descricao
DML;
                                $capid = $dadosPI['capid'];
                                $db->monta_combo('capid', $sql, 'S', 'Selecione', '', null, null, null, 'N', 'capid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($capid) ? $capid : null));
                                ?>
                            </div>
                        </div>
                        <div class="panel panel-default" id="div_siconv">
                            <div class="panel-body">
                                Números de Proposta SICONV
                                <div class="form-group">
                                    <div class="col-lg-9">
                                        <input type="text" placeholder="Digite um número e clique em adicionar." value="" class="CampoEstilo normal inteiro form-control" title="Número de Convênio" id="input_convenio"  maxlength="10" name="input_convenio" />
                                    </div>
                                    <div class="col-lg-3">
                                        <input type="button"  id="btn_selecionar_convenio" value="Adicionar" class="btn btn-info" style="width: 100%;"/>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <div class="col-lg-9">
                                        <table id="table_convenio" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                            <?php foreach($aConvenio as $convenio){ ?>
                                                <tr style="height: 30px;" id="tr_convenio_<?php echo $convenio['pcoid']; ?>" >
                                                    <td style="text-align: left;"><?php echo $convenio['pcoconvenio']; ?></td>
                                                    <td style="text-align: center;">
                                                        <input type="hidden" name="lista_convenio[]" value="<?php echo $convenio['pcoconvenio']; ?>" />
                                                        <span class="glyphicon glyphicon-trash link btnRemoveConvenio" data-convenio="<?php echo $convenio['pcoid']; ?>" ></span>
                                                    </td>
                                                </tr>
                                            <?php } ?>
                                        </table>
                                    </div>
                                </div>
                            </div>
                        </div>

                        <div class="form-group">
                            <div class="col-md-3 text-right">
                                <label class="control-label" for="picted">TED:</label>
                            </div>
                            <div class="col-lg-9">
                                <input type="checkbox" name="picted" id="picted" value="t" class="js-switch" <?php echo !empty($dadosPI['picted']) && 't' == $dadosPI['picted'] ? 'checked="checked"' : ''; ?> />
                            </div>
                        </div>

                        <div class="form-group" id="div_botao_edital">
                            <div class="col-md-3 text-right">
                                <label class="control-label" for="picedital">Edital:</label>
                            </div>
                            <div class="col-lg-9">
                                <input type="checkbox" name="picedital" id="picedital" value="t" class="js-switch element_edital" <?php echo !empty($dadosPI['picedital']) && 't' == $dadosPI['picedital'] ? 'checked="checked"' : ''; ?> />
                            </div>
                        </div>

                        <div class="form-group"  id="div_edital">
                            <div class="col-md-3 text-right">
                                <label class="control-label" for="mes">Prev. Lançamento Edital:</label>
                            </div>
                            <div class="col-lg-9">
                                <?php
                                $sql = "
                                                SELECT
                                                    mescod as codigo,
                                                    mesdsc as descricao
                                                FROM public.meses 
                                                ORDER BY
                                                    codigo
                                            ";
                                $mescod = $dadosPI['mescod'];
                                $db->monta_combo('mescod', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'mes', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($mescod) ? $mescod: null));
                                ?>
                            </div>
                        </div>

                        <div class="form-group">
                            <div class="col-md-3 text-right">
                                <label for="picexecucao" class="control-label">% Execução c/ Ente Federado:</label>
                            </div>
                            <div class="col-lg-9">
                                <input type="number" min="0" max="100" maxlength="3" value="<?php echo !empty($dadosPI['picexecucao'])? (int)$dadosPI['picexecucao']: ''; ?>" class="CampoEstilo normal form-control" title="Valor do percentual de execução do projeto." id="picexecucao" name="picexecucao" />
                            </div>
                        </div>
                        
                        <div class="form-group">
                            <div class="col-md-3 text-right">
                                <label for="picpriorizacao" class="control-label">Grau de priorização:</label>
                            </div>
                            <div class="col-lg-9">
                                <input type="text" value="<?php echo !empty($dadosPI['picpriorizacao']) ? $dadosPI['picpriorizacao'] : ''; ?>" class="CampoEstilo normal inteiro form-control" title="Grau de priorização para unidade" id="picpriorizacao" name="picpriorizacao" maxlength="2" />
                            </div>
                        </div>

                    </div>
                </div>

            </div>
            <div class="col-md-6">

                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Localização do Projeto <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></h5>
                    </div>
                    <div class="ibox-content">
                        <div class="form-group">
                            <div class="col-md-3 text-right">
                                <label for="esfid" class="control-label">Tipo de localização:</label>
                            </div>
                            <div class="col-lg-9">
                                <?php
                                $sql = "
                                                SELECT
                                                    esfid AS codigo,
                                                    esfdsc AS descricao
                                                FROM territorios.esfera
                                                ORDER BY
                                                    descricao
                                            ";
                                $esfid = $dadosPI['esfid'];
                                $db->monta_combo('esfid', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'esfid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, null);
                                ?>
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-lg-12">
                                <table id="table_localizacao" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                    <tr class="tr_head">
                                        <td style="text-align: left;"><b>UF</b></td>
                                        <td style="text-align: left;"><b>Município</b></td>
                                        <td style="text-align: center;"></td>
                                    </tr>
                                    <?php if($dadosPI['esfid'] == Territorios_Model_Esfera::K_MUNICIPAL){
                                        foreach($aLocalizacao as $localizacao){ ?>
                                            <tr style="height: 30px;" class="tr_localizacao_<?php echo $localizacao['muncod']; ?>" >
                                                <td style="text-align: left;"><?php echo $localizacao['estuf']; ?></td>
                                                <td style="text-align: left;"><?php echo $localizacao['mundescricao']; ?></td>
                                                <td style="text-align: center;">
                                                    <input type="hidden" value="<?php echo $localizacao['muncod']; ?>" name="listaLocalizacao[]">
                                                    <span class="glyphicon glyphicon-trash btnRemoverLocalizacao link" data-localizacao="<?php echo $localizacao['muncod']; ?>" />
                                                </td>
                                            </tr>
                                        <?php }
                                    } ?>

                                </table>
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-lg-12">
                                <table id="table_localizacao_estadual" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                    <tr class="tr_head">
                                        <td style="text-align: left;"><b>UF</b></td>
                                        <td style="text-align: left;"><b>Nome</b></td>
                                        <td style="text-align: center;"></td>
                                    </tr>
                                    <?php if($dadosPI['esfid'] == Territorios_Model_Esfera::K_ESTADUAL){
                                        foreach($aLocalizacao as $localizacao){ ?>
                                            <tr style="height: 30px;" class="tr_localizacao_estadual_<?php echo $localizacao['estuf']; ?>" >
                                                <td style="text-align: left;"><?php echo $localizacao['estuf']; ?></td>
                                                <td style="text-align: left;"><?php echo $localizacao['estdescricao']; ?></td>
                                                <td style="text-align: center;">
                                                    <input type="hidden" value="<?php echo $localizacao['estuf']; ?>" name="listaLocalizacaoEstadual[]">
                                                    <span class="glyphicon glyphicon-trash btnRemoverLocalizacaoEstadual link" data-localizacao-estadual="<?php echo $localizacao['estuf']; ?>" />
                                                </td>
                                            </tr>
                                        <?php }
                                    } ?>
                                </table>
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-lg-12">
                                <table id="table_localizacao_exterior" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                    <tr class="tr_head">
                                        <td style="text-align: left;"><b>País</b></td>
                                        <td style="text-align: center;"></td>
                                    </tr>
                                    <?php if($dadosPI['esfid'] == Territorios_Model_Esfera::K_EXTERIOR){
                                        foreach($aLocalizacao as $localizacao){ ?>
                                            <tr style="height: 30px;" class="tr_localizacao_exterior_<?php echo $localizacao['paiid']; ?>" >
                                                <td style="text-align: left;"><?php echo $localizacao['paidescricao']; ?></td>
                                                <td style="text-align: center;">
                                                    <input type="hidden" value="<?php echo $localizacao['paiid']; ?>" name="listaLocalizacaoExterior[]">
                                                    <span class="glyphicon glyphicon-trash btnRemoverLocalizacaoExterior link" data-localizacao-exterior="<?php echo $localizacao['paiid']; ?>" />
                                                </td>
                                            </tr>
                                        <?php }
                                    } ?>
                                </table>
                            </div>
                        </div>
                        <div class="form-group text-right">
                            <div class="col-lg-12">
                                <div class="text-right">
                                    <input type="button" id="btn_selecionar_localizacao_estadual" title="Inserir Localização Estadual/Distrito Federal" value="Inserir Localização Estadual/Distrito Federal" class="btn btn-info" />
                                    <input type="button" id="btn_selecionar_localizacao_exterior" title="Inserir Localização no Exterior" value="Inserir Localização no Exterior" class="btn btn-info" />
                                    <input type="button" id="btn_selecionar_localizacao" title="Inserir Localização municipal" value="Inserir Localização municipal" class="btn btn-info" />
                                </div>
                            </div>
                        </div>
                    </div>
                </div>

                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5 class="legend_responsaveis">Responsáveis pelo Projeto <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></h5>
                    </div>
                    <div class="ibox-content">

                        <div class="form-group">
                            <div class="col-lg-12">
                                <table id="table_responsaveis" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                    <tr>
                                        <td style="text-align: left;"><b>Nome</b></td>
                                        <td style="text-align: left;"><b>Telefone</b></td>
                                        <td style="text-align: left;"><b>E-mail</b></td>
                                        <td style="text-align: center;"><b>Ação</b></td>
                                    </tr>
                                    <?php foreach($aResponsavel as $responsavel){ ?>
                                        <tr style="height: 30px;" class="tr_responsaveis_<?php echo $responsavel['pirid']; ?>" >
                                            <td style="text-align: left;"><?php echo $responsavel['usunome']; ?></td>
                                            <td style="text-align: left;"><?php echo $responsavel['telefone']; ?></td>
                                            <td style="text-align: left;"><?php echo $responsavel['usuemail']; ?></td>
                                            <td style="text-align: center;">
                                                <input type="hidden" value="<?php echo $responsavel['usucpf']; ?>" name="listaResponsaveis[]">
                                                <span class="glyphicon glyphicon-trash btnRemoverResponsaveis link" data-responsaveis="<?php echo $responsavel['pirid']; ?>" />
                                            </td>
                                        </tr>
                                    <?php } ?>
                                </table>
                            </div>
                        </div>
                        <div class="form-group text-right">
                            <div class="col-lg-12">
                                <input type="button" id="btn_selecionar_responsaveis" title="Inserir Responsáveis" value="Inserir Responsáveis" class="btn btn-info"/>
                            </div>
                        </div>

                    </div>
                </div>

                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Vinculações Eventuais do Projeto</h5>
                    </div>
                    <div class="ibox-content">
                        <div class="row">
                            <div class="col-md-3">
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
                                            <?php foreach($aSniic as $sniic){ ?>
                                                <tr style="height: 30px;" id="tr_sniic_<?php echo $sniic['pisid']; ?>" >
                                                    <td style="text-align: left;"><?php echo $sniic['pissniic']; ?></td>
                                                    <td style="text-align: center;">
                                                        <input type="hidden" name="lista_sniic[]" value="<?php echo $sniic['pissniic']; ?>" />
                                                        <span class="glyphicon glyphicon-trash link btnRemoveSniic" data-sniic="<?php echo $sniic['pisid']; ?>" ></span>
                                                    </td>
                                                </tr>
                                            <?php } ?>
                                        </table>
                                    </div>
                                </div>
                            </div>
                            <div class="col-md-5">
                                Número do Processo
                                <div class="form-group">
                                    <div class="col-md-10" style="padding-right: 3px;">
                                        <input type="text" placeholder="Adicione" value="" class="CampoEstilo normal form-control" title="Número SEI" id="input_sei" name="input_sei" />
                                    </div>
                                    <div class="col-lg-1" style="padding-left: 0;">
                                        <input type="button" id="btn_adicionar_sei" value="+" class="btn btn-info" />
                                    </div>
                                </div>
                                <div class="form-group">
                                    <div class="col-lg-12">
                                        <table id="table_sei" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                            <?php foreach($aSei as $sei){ ?>
                                                <tr style="height: 30px;" id="tr_sei_<?php echo $sei['pseid']; ?>" >
                                                    <td style="text-align: left;"><?php echo $sei['psesei']; ?></td>
                                                    <td style="text-align: center;">
                                                        <input type="hidden" name="lista_sei[]" value="<?php echo $sei['psesei']; ?>" />
                                                        <span class="glyphicon glyphicon-trash link btnRemoveSei" data-sei="<?php echo $sei['pseid']; ?>" ></span>
                                                    </td>
                                                </tr>
                                            <?php } ?>
                                        </table>
                                    </div>
                                </div>
                            </div>
                            <div class="col-md-4">
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
                                            <?php foreach($aPronac as $pronac){ ?>
                                                <tr style="height: 30px;" id="tr_pronac_<?php echo $pronac['pprid']; ?>" >
                                                    <td style="text-align: left;"><?php echo $pronac['pprpronac']; ?></td>
                                                    <td style="text-align: center;">
                                                        <input type="hidden" name="lista_pronac[]" value="<?php echo $pronac['pprpronac']; ?>" />
                                                        <span class="glyphicon glyphicon-trash link btnRemovePronac" data-pronac="<?php echo $pronac['pprid']; ?>" ></span>
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

            <div class="clearfix"></div>

            <div class="col-md-6 div_custeio_capital">

                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Custeio e Capital</h5>
                    </div>
                    <div class="ibox-content">
                        <div class="form-group">
                            <div class="col-lg-12">
                                <table cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                    <tr>
                                        <td style="text-align: left;"></td>
                                        <td colspan="3" style="text-align: center;">
                                            <b>Total (Custeio e Capital)</b>
                                        </td>
                                    </tr>
                                    <tr style="height: 30px;">
                                        <td style="text-align: left;">Limite Geral FNC: </td>
                                        <td colspan="3" id="td_autorizado_sub_unidade" style="text-align: center;" >0,00</td>
                                    </tr>
                                    <tr style="height: 30px;">
                                        <td style="text-align: left;">
                                            Limite Disponível FNC:
                                            <input id="VlrSUDisponivel" type="hidden" value="" />
                                        </td>
                                        <td colspan="3" id="td_disponivel_sub_unidade" style="text-align: center;" >0,00</td>
                                    </tr>
                                    <tr>
                                        <td style="text-align: left;"></td>
                                        <td style="text-align: center;"><b>Custeio </b></td>
                                        <td style="text-align: center;"><b>Capital</b></td>
                                    </tr>
                                    <tr>
                                        <td style="text-align: left;">Autorizado na Funcional:</td>
                                        <td style="text-align: left;" id="td_autorizado_funcional_custeio" >0,00</td>
                                        <td style="text-align: center;" id="td_autorizado_funcional_capital" >0,00</td>
                                    </tr>
                                    <tr>
                                        <td style="text-align: left;">
                                            Já cadastrado na funcional pela Unidade:
                                            <input id="VlrFuncionalDisponivelCusteio" type="hidden" value="" />
                                            <input id="VlrFuncionalDisponivelCapital" type="hidden" value="" />
                                        </td>
<td style="display: none; text-align: left;" id="td_disponivel_funcional_custeio" >0,00</td>
<td style="display: none; text-align: center;" id="td_disponivel_funcional_capital" >0,00</td>
                                        <td style="text-align: left;" id="td_ja_autorizado_funcional_custeio" >0,00</td>
                                        <td style="text-align: center;" id="td_ja_autorizado_funcional_capital" >0,00</td>
                                    </tr>
                                    <tr>
                                        <td id="" style="text-align: left;">Valor do Projeto: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></td>
                                        <td style="text-align: left;">
                                            <input type="text" id="picvalorcusteio" name="picvalorcusteio" value="<?php echo !empty($dadosPI['picvalorcusteio']) ? number_format($dadosPI['picvalorcusteio'], 2, ',', '.') : ''; ?>" class="CampoEstilo normal form-control obrigatorio" />
                                            <input type="hidden" id="vlrPiCusteio" value="<?php echo !empty($dadosPI['picvalorcusteio']) && !$_REQUEST['replicarProposta']? number_format($dadosPI['picvalorcusteio'], 2, ',', '.') : ''; ?>" />
                                        </td>
                                        <td style="text-align: center;">
                                            <input type="text" id="picvalorcapital" name="picvalorcapital" value="<?php echo !empty($dadosPI['picvalorcapital']) ? number_format($dadosPI['picvalorcapital'], 2, ',', '.') : ''; ?>" class="CampoEstilo normal form-control obrigatorio" />
                                            <input type="hidden" id="vlrPiCapital" value="<?php echo !empty($dadosPI['picvalorcapital']) && !$_REQUEST['replicarProposta']? number_format($dadosPI['picvalorcapital'], 2, ',', '.') : ''; ?>" />
                                        </td>
                                    </tr>
                                </table>
                            </div>
                        </div>
                    </div>
                </div>

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

            <div class="col-md-6">

                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Cronogramas <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></h5>
                    </div>
                    <div class="ibox-content">

                        <div class="form-group">
                            <div class="col-lg-12">
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
                                <table cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover table-input">
                                    <tr>
                                        <td rowspan="2" style="text-align: center; border-right: 1px black solid;">
                                            <b>Mês</b>
                                        </td>
                                        <?php 
                                            $colunaCronograma = 0;
                                            foreach($cronogramasAgrupado as $cronograma => $descricaoValores){ ?>
                                                <td style="text-align: center; border-right: 1px black solid;" colspan="<?php echo count($descricaoValores); ?>" class="<?php echo buscarClasseCronograma($colunaCronograma, TRUE); ?>"><b><?php echo $cronograma; ?></b></td>
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
                                                    $bordaPreta = in_array($cronogramaValor['crvid'], array(Cronograma::K_FISICO, Cronograma::ORCAMENTO_CAPITAL, Cronograma::FINANCEIRO_CAPITAL))? 'border-right: 1px black solid': NULL;
                                            ?>
                                                <td style="text-align: center; <?php echo $bordaPreta; ?>" class="<?php echo buscarClasseCronograma($colunaCronograma, TRUE); ?>"><?php echo $cronogramaValor['crvdsc']; ?></td>
                                            <?php
                                                    $colunaCronograma++;
                                                }
                                        }
                                        ?>
                                    </tr>
                                    <?php foreach($meses as $mes){ ?>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left; border-right: 1px black solid;"><?php echo $mes['mesdsc']; ?></td>
                                            <?php foreach($aCronogramas as $cronogramaValor){
                                                switch($cronogramaValor['croid']){
                                                    case Cronograma::K_FISICO:          $class = 'input_fisico inteiro';        break;
                                                    case Cronograma::K_ORCAMENTARIO:    $class = 'input_orcamentario';  break;
                                                    case Cronograma::K_FINANCEIRO:      $class = 'input_financeiro';    break;
                                                    default: $class = '';
                                                }
                                                $bordaPreta = in_array($cronogramaValor['crvid'], array(Cronograma::K_FISICO, Cronograma::ORCAMENTO_CAPITAL, Cronograma::FINANCEIRO_CAPITAL))? 'border-right: 1px black solid': NULL;
                                            ?>
                                            <td class="<?php echo buscarClasseCronograma($cronogramaValor['croid']); ?>" style="<?php echo $bordaPreta; ?>">
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
                                                </td>
                                            <?php } ?>
                                        </tr>
                                    <?php } ?>
                                    <tr style="height: 30px; ">
                                        <td style="text-align: left; border-right: 1px black solid;">Total:</td>
                                        <td style="font-weight: bold; text-align: center; border-right: 1px black solid;" id="td_total_fisico" class="td_cronograma_fisico">0</td>
                                        <td style="font-weight: bold; text-align: center;" id="td_total_orcamentario_custeio" class="td_cronograma_orcamentario">R$ 0,00</td>
                                        <td style="font-weight: bold; text-align: center; border-right: 1px black solid;" id="td_total_orcamentario_capital" class="td_cronograma_orcamentario">R$ 0,00</td>
                                        <td style="font-weight: bold; text-align: center;" id="td_total_financeiro_custeio" class="td_cronograma_financeiro">R$ 0,00</td>
                                        <td style="font-weight: bold; text-align: center; border-right: 1px black solid;" id="td_total_financeiro_capital" class="td_cronograma_financeiro">R$ 0,00</td>
                                    </tr>
                                </table>
                            </div>
                        </div>

                    </div>
                </div>
            </div>
        </div>

        <?php if (!(in_array(PFL_CONSULTA, $perfis)) && !(in_array(PFL_CONSULTA_UNIDADE, $perfis))) {?>
            <div class="form-group">
                <div class="text-center">
                    <button class="btn btn-primary" type="button" name="btg" onclick="submeter('<?php echo $plicod; ?>//');" <?php echo $disabled; ?>><i class="fa fa-check"></i>&nbsp;Salvar</button>
                    <a href="planacomorc.php?modulo=principal/unidade/listapimanter_fnc&acao=A" class="btn btn-warning" id="btnVoltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                </div>
            </div>
        <?php } ?>
    </form>
</div>

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
                        <form class="dropzone" method="POST" enctype="multipart/form-data" action="planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A&uploadAnexo=1" id="formularioAnexo" name="formularioAnexo">
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
    // Estado do PI FNC
    intEsdidAprovado = "<?php echo ESD_FNC_PI_APROVADO; ?>";
    // Manutenção item
    intMaiid = '<?php echo $dadosPI['maiid']; ?>';
    // Estado do PI
    intEsdid = '<?php echo $estadoAtual['esdid']; ?>';

    listaEqdReduzido = <?php echo $listaEqdidReduzido; ?>;

    // Permissão Admin
    isAdmin = '<?php echo (in_array(PFL_ADMINISTRADOR, $perfis) || in_array(PFL_SUPERUSUARIO, $perfis)); ?>';

    // Permissão para Editar
    podeEditar = '<?php echo $podeEditar; ?>';
    
    fnc = true;
    
    ppiid = '<?php echo isset($_REQUEST['replicarProposta'])? $_REQUEST['replicarProposta']: 0; ?>';

    urlPagina = '?modulo=principal/unidade/cadastro_pi_fnc&acao=A';

    $(document).ready(function() {

        // Ações efetuadas quando a tela de cadastro de Plano de Ação é iniciada.
        initCadastroPi();

    });

</script>