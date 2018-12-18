
/**
 * Ações efetuadas quando a tela de Cadastro de Alterações Orçamentárias é iniciada.
 *
 */
function initCadastroAlteracoes() {

    permitirTodosPassos = $('#pedid').val()? true: false;

    var wizard = $("#wizard").steps({
        transitionEffect: "slide",
        startIndex: 0,
        width: 50,
        enablePagination: permitirTodosPassos,
        enableAllSteps: permitirTodosPassos,
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

    $('body').on('click', '.a_espelho', function(){
        var pliid = $(this).attr('data-pi');
        exibirEspelhoPi(pliid);
        return false;
    });

}

/**
 * Exibe popup com Detalhes do pi. Tela de Espelho de PI.
 *
 * @returns VOID
 */
function exibirEspelhoPi(pliid){
    window.open(
        '?modulo=principal/cadastro_alteracoes&acao=C&req=espelho-pi&pliid='+ pliid,
        'popup_espelho_pi',
        'width=780,height=1000,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
}
