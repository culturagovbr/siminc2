<?php
require_once APPRAIZ . 'includes/workflow.php';

$disabled = ['required'];

$mAlteracaoPedido = new Alteracao_Model_Pedido($_REQUEST['pedid']);
if ($_REQUEST['pedid']){
    $arrPedidos= $mAlteracaoPedido->recuperarPedidoPorId($_REQUEST['pedid']);
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
        case 'carrega_remanejamento_loa':
            $aDadosPiSelecionados = $mAlteracaoPedido->listaPisSelecionados($_REQUEST['pedid']);
            include_once APPRAIZ.'alteracao/modulos/principal/remanejamento_loa.inc';
            die;   
        case 'carrega_lista_pis_selecionados':
            $aDadosPiSelecionados = $mAlteracaoPedido->listaPisSelecionados($_REQUEST['pedid']);
            include_once APPRAIZ.'alteracao/modulos/principal/lista_pis_selecionados.inc';
            die;   
        case 'carrega_lista_pis':
            $filtropi['exercicio'] = $_SESSION['exercicio'];
            $filtropi['usucpf'] = $_SESSION['usucpf'];
            $filtropi['unofundo'] = 'FALSE';
            $filtropi['pedid'] = $_REQUEST['pedid']?$_REQUEST['pedid']:0;
            $filtropi['esdid'] = array(ESD_PI_APROVADO);

            $sql = $mAlteracaoPedido->listarPis((object) $filtropi);
            $aDadosPi = $mAlteracaoPedido->carregar($sql);
            include_once APPRAIZ.'alteracao/modulos/principal/lista_pis.inc';
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
        default:
            break;
    }
}
include APPRAIZ. "includes/cabecalho.inc";
?>

<style>
.wizard > .steps > ul > li {
    width: 210px;
}
</style>
<input name="pedid" id="pedid" value="<?=$arrPedidos['pedid'];?>" type="hidden">
<input name="tpdid" id="tpdid" value="<?=$_REQUEST['tpdid'];?>" type="hidden">
<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-8">
        <h2><?= $titulo_modulo;?></h2>
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
                <div class="div_lista_indicadores">
                    <?php include_once APPRAIZ.'alteracao/modulos/principal/cadastro_pedido.inc'; ?>
                </div>

                <h1>SELECIONAR PI</h1>
                <div class="div_selecionar_pi"></div>

                <h1>REMANEJAMENTO LOA</h1>
                <div class="div_remanejamento_loa"></div>
                
<!--                <h1>NATUREZA DA RECEITA</h1>-->
<!--                <div class="div_natureza_receita"></div>-->
                
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
        
    var wizard = $("#wizard").steps({
        transitionEffect: "slide",
        startIndex: 0,
        width: 50,
        
        //enableFinishButton: exibirBotaoEnviar(),
        <?php
        if (!$_REQUEST['pedid']){
            echo 'enablePagination: false, enableAllSteps: false,';
        }else{
            echo 'enablePagination: true, enableAllSteps: true,';
        }
        ?>
        labels: {
            cancel: "Cancelar",
            current: "Passo atual:",
            pagination: "Paginação",
            next: "Próximo",
            previous: "Anterior",
            finish: "Enviar",
            loading: "Carregando ..."
        },
        onStepChanging: function(event, currentIndex, newIndex){
            var permitirAvancarPasso = true;
            switch(currentIndex) {
                case 0:
                    permitirAvancarPasso = verificaCampos();
                    if (permitirAvancarPasso) {
                        salvarPedido();
                    }
                    break;
                case 4:
                    salvarPedido();
                    break;
            }
            return permitirAvancarPasso;
        },
        onFinished: function(event, currentIndex){
            enviarAcompanhamento();
        },
        onStepChanged: function(event, currentIndex, priorIndex){
            switch(currentIndex){
                case 0:
                    $(".div_lista_atividades").load('?modulo=principal/cadastro_alteracoes&acao=C&req=carrega_pedidos&pedid='+$("#pedid").val()+'&tpdid='+$("#tpdid").val());
                    $(".divDadosCredito").hide('fast');
                    break;
                case 1:
                    $(".div_selecionar_pi").load('?modulo=principal/cadastro_alteracoes&acao=C&req=carrega_seleciona_pis&pedid='+$("#pedid").val()+'&tpdid='+$("#tpdid").val());
                    $(".divDadosCredito").show('fast');
                    break;
                case 2:
                    $(".div_remanejamento_loa").load('?modulo=principal/cadastro_alteracoes&acao=C&req=carrega_remanejamento_loa&pedid='+$("#pedid").val()+'&tpdid='+$("#tpdid").val());
                    $(".divDadosCredito").show('fast');
                    break;
                case 3:
                    $(".div_justificativa").load('?modulo=principal/cadastro_alteracoes&acao=C&req=carrega_justificativa&pedid='+$("#pedid").val()+'&tpdid='+$("#tpdid").val());
                    $(".divDadosCredito").show('fast');
                    break;
                case 4:
                    $(".div_espelho").load('?modulo=principal/cadastro_alteracoes&acao=C&req=carrega_espelho&pedid='+$("#pedid").val()+'&tpdid='+$("#tpdid").val());
                    $(".divDadosCredito").show('fast');
                    break;
            }
        }
    }); 
});
</script>