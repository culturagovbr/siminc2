<?php
require_once APPRAIZ . 'includes/workflow.php';

$mAlteracaoPedido = new Alteracao_Model_Pedido($_REQUEST['pedid']);
if ($_REQUEST['pedid']){
    if (!$mAlteracaoPedido->docid){
        $mAlteracaoPedido->docid = $mAlteracaoPedido->pegarDocidPedido($_REQUEST['pedid'], $_REQUEST['tpdid']);
    }
}
if ($_REQUEST['req']){
    switch ($_REQUEST['req']) {
        case 'pedidos':
            include_once APPRAIZ.'alteracao/modulos/principal/cadastro_pedido.inc';
        die;
        case 'salvar_pedido':
            $cPedido = new Alteracao_Controller_Pedido();
            $cPedido->salvar($_REQUEST);
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
<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-8">
        <h2><?= $titulo_modulo;?></h2>
    </div>
</div>
<div class="row">
    <div class="col-lg-11">
        <div class="ibox-content">
            <div id="wizard">
                <h1>PEDIDO</h1>
                <div class="div_lista_indicadores_pnc" data-mode="async" data-url="?modulo=principal/cadastro_alteracoes&acao=C&req=pedidos&pedid=<?=$_REQUEST['pedid'];?>">
                    Carregando ...
                </div>

                <h1>SELECIONAR PI</h1>
                <div class="div_lista_atividades_pnc" data-mode="async" data-url="?modulo=principal/cadastro_alteracoes&acao=C&req=pedidos&pedid=<?=$_REQUEST['pedid'];?>">
                    Carregando ...
                </div>

                <h1>REMANEJAMENTO LOA</h1>
                <div class="div_lista_anexos_pnc" data-mode="async" data-url="?modulo=principal/cadastro_alteracoes&acao=C&req=pedidos&pedid=<?=$_REQUEST['pedid'];?>">
                    Carregando ...
                </div>
                <h1>NATUREZA DA RECEITA</h1>
                <div class="div_lista_anexos_pnc" data-mode="async" data-url="?modulo=principal/cadastro_alteracoes&acao=C&req=pedidos&pedid=<?=$_REQUEST['pedid'];?>">
                    Carregando ...
                </div>
                <h1>JUSTIFICATIVA</h1>
                <div class="div_lista_anexos_pnc" data-mode="async" data-url="?modulo=principal/cadastro_alteracoes&acao=C&req=pedidos&pedid=<?=$_REQUEST['pedid'];?>">
                    Carregando ...
                </div>
                <h1>ESPELHO</h1>
                <div class="div_lista_anexos_pnc" data-mode="async" data-url="?modulo=principal/cadastro_alteracoes&acao=C&req=pedidos&pedid=<?=$_REQUEST['pedid'];?>">
                    Carregando ...
                </div>

            </form>  
            </div>
        </div>
    </div>
    <div class="col-lg-1">
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
        enableAllSteps: true,
        //enableFinishButton: exibirBotaoEnviar(),
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
                    salvarPedido();
            }
            return permitirAvancarPasso;
        },
        onFinished: function(event, currentIndex){
            enviarAcompanhamento();
        }
    }); 
});
</script>