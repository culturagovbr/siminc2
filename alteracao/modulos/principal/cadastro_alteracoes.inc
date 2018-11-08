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
        case 'carrega_pedidos':
            include_once APPRAIZ.'alteracao/modulos/principal/cadastro_pedido.inc';
            die;   
        case 'carrega_justificativa':
            include_once APPRAIZ.'alteracao/modulos/principal/cadastro_justificativa.inc';
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
                <div class="div_lista_indicadores">
                    <?php include_once APPRAIZ.'alteracao/modulos/principal/cadastro_pedido.inc'; ?>
                </div>

                <h1>SELECIONAR PI</h1>
                <div class="div_selecionar_pi"></div>

                <h1>REMANEJAMENTO LOA</h1>
                <div class="div_remanejamento_loa"></div>
                
                <h1>NATUREZA DA RECEITA</h1>
                <div class="div_natureza_receita"></div>
                
                <h1>JUSTIFICATIVA</h1>
                <div class="div_justificativa"></div>
                
                <h1>ESPELHO</h1>
                <div class="div_espelho"></div>

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
                    permitirAvancarPasso = verificaCampos();
                    if (permitirAvancarPasso) {
                        $("#startIndex").val(newIndex);
                        salvarPedido();
                    }
                    break;
                case 1:
                    $("#startIndex").val(newIndex);
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
                    $(".div_lista_atividades").load('?modulo=principal/cadastro_alteracoes&acao=C&req=carrega_pedidos&pedid=<?=$_REQUEST['pedid'];?>');
                    break;
                case 4:
                    $(".div_justificativa").load('?modulo=principal/cadastro_alteracoes&acao=C&req=carrega_justificativa&pedid=<?=$_REQUEST['pedid'];?>');
                    break;
            }
        }
    }); 
});
</script>