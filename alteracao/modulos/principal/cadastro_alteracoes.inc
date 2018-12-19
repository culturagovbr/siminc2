<?php
require_once APPRAIZ . 'includes/workflow.php';

$disabled = ['required'];
$podeEditarAlteracao = TRUE;

$mAlteracaoPedido = new Alteracao_Model_Pedido($_REQUEST['pedid']);
if ($_REQUEST['pedid']){
    $arrPedidos = $mAlteracaoPedido->recuperarPedidoPorId($_REQUEST['pedid']);
    if (!$mAlteracaoPedido->docid){
        $mAlteracaoPedido->docid = $mAlteracaoPedido->pegarDocidPedido($arrPedidos['pedid'], $arrPedidos['tpdid']);
    }
}

if ($_REQUEST['req']){
    switch ($_REQUEST['req']) {
        case 'carrega_pedidos':
            include_once APPRAIZ.'alteracao/modulos/principal/cadastro_pedido.inc';
            die;
        case 'carrega_seleciona_pis':
            $aDadosPiSelecionados = $mAlteracaoPedido->listaPisSelecionados($_REQUEST['pedid']);
            include_once APPRAIZ.'alteracao/modulos/principal/selecionar_pi.inc';
            die;
        case 'carrega_lista_pis_selecionados':
            $aDadosPiSelecionados = $mAlteracaoPedido->listaPisSelecionados($_REQUEST['pedid']);
            include_once APPRAIZ.'alteracao/modulos/principal/lista_pis_selecionados.inc';
            die;
        case 'carrega_lista_pis':
            $filtropi['exercicio'] = $_SESSION['exercicio'];
            $filtropi['usucpf'] = $_SESSION['usucpf'];
            $filtropi['unofundo'] = 'FALSE';
            $filtropi['esdid'] = array(ESD_PI_APROVADO);
            $filtropi['subunidade_pedido'] = $_REQUEST['pedid']? $_REQUEST['pedid']: 0;

            $sql = $mAlteracaoPedido->listarPis((object) $filtropi);
            $aDadosPi = $mAlteracaoPedido->carregar($sql);
            include_once APPRAIZ.'alteracao/modulos/principal/lista_pis.inc';
            die;
        case 'carrega_remanejamento_loa':
            $aDadosPiSelecionados = $mAlteracaoPedido->listaPisSelecionados($_REQUEST['pedid']);
            include_once APPRAIZ.'alteracao/modulos/principal/remanejamento_loa.inc';
            die;
        case 'carrega_justificativa':
            include_once APPRAIZ.'alteracao/modulos/principal/cadastro_justificativa.inc';
            die;
        case 'carrega_dados_credito':
            if ($_REQUEST['pedid']){
                $arrPedidos= $mAlteracaoPedido->recuperarPedidoPorId($_REQUEST['pedid']);
            }
            include_once APPRAIZ.'alteracao/modulos/principal/dados_credito.inc';
            die;
        case 'salvar_pedido':
            $cPedido = new Alteracao_Controller_Pedido();
            $cPedido->salvar($_REQUEST);
            die;
        case 'salvar_campo':
            $cPedido = new Alteracao_Controller_PlanoInternoSelecionado();
            $cPedido->salvarCampo($_REQUEST);
            die;
        case 'adiciona_pi_pedido':
            $cPedido = new Alteracao_Controller_PlanoInternoSelecionado();
            $cPedido->salvar($_REQUEST);
            die;
        case 'remove_pi_pedido':
            $cPedido = new Alteracao_Controller_PlanoInternoSelecionado();
            $cPedido->excluir($_REQUEST);
            die;
        case 'salvar_loa':
            $cPedido = new Alteracao_Controller_RemanejamentoLoa();
            $cPedido->salvar($_REQUEST);
            die;
        case 'salvar_justificativa':
            $cJustificativa = new Alteracao_Controller_Justificativa();
            $cJustificativa->salvar($_REQUEST);
            die;
        case 'remover_loa':
            $cPedido = new Alteracao_Controller_RemanejamentoLoa();
            $cPedido->excluir($_REQUEST['rlid']);
            die;
        case 'seleciona_dados_funcional':
            $mPedido = new Alteracao_Model_RemanejamentoLoa();
            echo json_encode($mPedido->selecionaDadosFuncional($_REQUEST));
            die;
        case 'seleciona_dotacao':
            $mPedido = new Alteracao_Model_RemanejamentoLoa();
            echo json_encode($mPedido->selecionaDotacao($_REQUEST));
            die;
        case 'carrega_espelho':
            include_once APPRAIZ.'alteracao/modulos/principal/espelho_alteracao.inc';
            die;
        case 'espelho-pi':
            include APPRAIZ. 'planacomorc/modulos/principal/unidade/espelho-pi.inc';
            die;
        case 'alterar_estado':
            $cPedido = new Alteracao_Controller_Pedido();
            $cPedido->alterarEstadoAlteracao($_REQUEST['pedid']);
            die;
        default:
            break;
    }
}
include APPRAIZ. "includes/cabecalho.inc";
?>
<script src="js/principal/cadastro-alteracoes.js?v=1"></script>
<style>
    .wizard > .steps > ul > li {
        width: 210px;
    }
</style>
<input name="pedid" id="pedid" value="<?=$arrPedidos['pedid'];?>" type="hidden">
<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-8">
        <h2><?= $titulo_modulo;?></h2>
        <h6><span style="color: #ff0000"> Os campos com * são de preenchimento obrigatório</span></h6>
    </div>
</div>
<div class="divDadosCredito row wrapper border-bottom white-bg page-heading" style="display: none;">
    <div class="col-lg-8">
        <h3>DADOS DO CRÉDITO</h3>
    </div>
</div>
<div class="divDadosCredito row wrapper border-bottom white-bg page-heading" id="divDadosCredito" style="display: none;">
    <?php include_once APPRAIZ.'alteracao/modulos/principal/dados_credito.inc'; ?>
</div>
<div class="row">
    <div class="col-lg-12">
        <div class="ibox-content">
            <div id="wizard">
                <h1>PEDIDO</h1>
                
                <div class="div_pedido">
                    <?php include_once APPRAIZ.'alteracao/modulos/principal/cadastro_pedido.inc'; ?>
                </div>

                <h1>SELECIONAR PI</h1>
                <div class="div_selecionar_pi"></div>

                <h1>REMANEJAMENTO LOA</h1>
                <div class="div_remanejamento_loa"></div>

                <h1>JUSTIFICATIVA</h1>
                <div class="div_justificativa"></div>
                
                <h1>ESPELHO</h1>
                <div class="div_espelho"></div>

            </div>
        </div>
    </div>
    <div class="col-lg-1" style="position: absolute; right: 0; top: 10px;">
        <div class="col-md-1">
            <?php wf_desenhaBarraNavegacao($mAlteracaoPedido->docid, array('pedid' => $mAlteracaoPedido->pedid)); ?>
            <div id="div_comentario_estado_atual" style="display: none;"><?php echo wf_pegarComentarioEstadoAtual($mAlteracaoPedido->docid); ?></div>
        </div>         
    </div>
</div>

<script>
    $(function(){
        initCadastroAlteracoes();
    });
</script>