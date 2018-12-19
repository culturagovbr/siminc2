
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

}

