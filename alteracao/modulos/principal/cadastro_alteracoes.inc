<?php

$mAlteracaoPedido = new Alteracao_Model_Pedido($_REQUEST['pedid']);
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
    width: 200px;
}
</style>
<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-8">
        <h2><?= $titulo_modulo;?></h2>
    </div>
</div>

<div class="ibox-content">
    <div id="wizard">
        <h1>PEDIDO</h1>
        <div class="div_lista_indicadores_pnc" data-mode="async" data-url="?modulo=principal/cadastro_alteracoes&acao=C&req=pedidos&pedid=<?=$_REQUEST['pedid'];?>">
            Carregando ...
        </div>

        <h1>SELECIONAR PI</h1>
        <div class="div_lista_atividades_pnc" data-mode="async" data-url="">
            Carregando ...
        </div>

        <h1>REMANEJAMENTO LOA</h1>
        <div class="div_lista_anexos_pnc" data-mode="async" data-url="">
            Carregando ...
        </div>
        <h1>NATUREZA DA RECEITA</h1>
        <div class="div_lista_anexos_pnc" data-mode="async" data-url="">
            Carregando ...
        </div>
        <h1>JUSTIFICATIVA</h1>
        <div class="div_lista_anexos_pnc" data-mode="async" data-url="">
            Carregando ...
        </div>
        <h1>ESPELHO</h1>
        <div class="div_lista_anexos_pnc" data-mode="async" data-url="">
            Carregando ...
        </div>

    </form>
    </div>
</div>

<script>
$(function(){
        
    var wizard = $("#wizard").steps({
        transitionEffect: "slide",
        startIndex: 0,
        width: 50,
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
                case numeroPassoIndicadores:
                    if(newIndex === numeroPassoAtividades){
                        permitirAvancarPasso = verificarPreenchimentoIndicadoresAtual();
                        if(permitirAvancarPasso === false){
                            swal('', 'Por favor, preencha pelo menos os indicadores do ano de '+ $('#chosen-exercicio').val()+ '.', 'warning');
                        }
                    }
                    break;
                case numeroPassoAtividades:
                    if(newIndex === numeroPassoAnexos){
                        permitirAvancarPasso = verificarPreenchimentoAtividade();
                        if(permitirAvancarPasso === false){
                            swal('', 'Por favor, insira pelo menos uma atividade.', 'warning');
                        }
                    }
                    break;
            }
            return permitirAvancarPasso;
        },
        onFinished: function(event, currentIndex){
            enviarAcompanhamento();
        }
    }); 
});
</script>