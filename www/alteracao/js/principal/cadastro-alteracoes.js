
/**
 * Ações efetuadas quando a tela de Cadastro de Alterações Orçamentárias é iniciada.
 *
 */
function initCadastroAlteracoes() {

    var permitirTodosPassos = $('#pedid').val()? true: false;
    var permitirBotaoFinalizar = verificarEstadoEdicao($('#esdid').val());
    var permitirEditar = permiteEditar($('#esdid').val());

    var wizard = $("#wizard").steps({
        transitionEffect: "slide",
        startIndex: 0,
        width: 50,
        enablePagination: permitirTodosPassos,
        enableAllSteps: permitirTodosPassos,
        enableFinishButton: permitirBotaoFinalizar,
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
                        if(permitirEditar == true){
                            salvarPedido();
                        }
                    }
                    break;
                case 4:
                    if(permitirEditar == true){
                        salvarPedido();
                    }
                    break;
            }
            return permitirAvancarPasso;
        },
        onFinished: function(event, currentIndex){
            alterarEstadoAlteracao();
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
 * Permite Editar somente quando o estado do Pedido estiver em 'Em cadastramento' ou em 'Aguardando Correção'.
 *
 * @param esdid -> Estado do documento
 * @returns {boolean}
 */
function permiteEditar(esdid) {

    if( esdid == ESD_EM_CADASTRAMENTO_INTERNO ||
        esdid == ESD_EM_CADASTRAMENTO_EXTERNO ||
        esdid == ESD_AGURADANDO_CORRECAO_INTERNO ||
        esdid == ESD_AGURADANDO_CORRECAO_EXTERNO
    ){
        return true;
    }else{
        return false;
    }
}

/**
 * Verifica o Estado do Documento Atual para permitir visualizar o botão de 'Enviar'.
 *
 * @param esdid -> Estado documento.
 */
function verificarEstadoEdicao(esdid) {

    if( esdid == ESD_EM_CADASTRAMENTO_INTERNO ||
        esdid == ESD_EM_CADASTRAMENTO_EXTERNO ||
        esdid == ESD_AGURADANDO_CORRECAO_INTERNO ||
        esdid == ESD_AGURADANDO_CORRECAO_EXTERNO
    ){
        return true;
    }else{
        return false;
    }
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

function alterarEstadoAlteracao(){
    $.post('?modulo=principal/cadastro_alteracoes&acao=C',
    {
        req: 'alterar_estado',
        pedid: $("#pedid").val()
    }, function (result) {
        console.log(result);
        result = JSON.parse(result);
        if (result){
            swal({
                title: "",
                text: "Registro salvo com sucesso!",
                type: "success",
                confirmButtonText: 'OK'
            }, function(isConfirm){
                if (isConfirm){
                    window.location.href='?modulo=inicio&acao=C';
                }
            });            
        }else{
            swal('', 'Erro ao realizar operação!', 'warning');
        }
    });
}
