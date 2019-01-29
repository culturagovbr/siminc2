
/**
 * Ações efetuadas quando a tela de lista de Alterações Orçamentárias é iniciada.
 *
 */
function initListaAlteracao() {

    $('.btn-limpar').click(function(){
        $('#requisicao').val('limpar');
        $('#filtroalteracao').submit();
    });

    $('#btn-exportar-xls').click(function(){
//            window.open('?modulo=inicio&acao=C&req=lista-inicio-xls&tipo='+ $("#tipo").val());
    });

    $('.tab-tipo').click(function () {
        $("#tpdid").val($(this).attr('tipo'));
        $('#filtroalteracao').submit();
    });

    $('.a_espelho_pedido').click(function(){
        var pedid = $(this).attr('data-pedid');
        exibirEspelhoPedido(pedid);
        return false;
    });

    $('.btn-excluir-pedido').click(function () {
        var pedid = $(this).data('pedid');
        removerPedido(pedid);
    });
}

/**
 * Exibe popup com Detalhes do Pedido. Tela de Espelho de Pedido.
 *
 * @returns VOID
 */
function exibirEspelhoPedido(pedid){
    window.open(
        '?modulo=inicio&acao=C&req=espelho-pedido&pedid='+ pedid,
        'popup_espelho_pedido',
        'width=980,height=1000,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
}

/**
 * Exclui o pedido de Alteração Orçamentária.
 *
 * @param pedid
 */
function removerPedido(pedid) {
    swal({
            title: "Atenção!",
            text: "Deseja realmente excluir o registro ?",
            type: "warning",
            showCancelButton: true,
            confirmButtonColor: "#DD6B55",
            confirmButtonText: "Sim, estou certo!",
            closeOnConfirm: false
        },
        function(isConfirm){

            if (isConfirm){
                window.location.href = 'alteracao.php?modulo=inicio&acao=C&requisicao=excluir_pedido&pedid='+pedid
            }
        });
}