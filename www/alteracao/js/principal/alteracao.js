
/**
 * A��es efetuadas quando a tela de lista de Altera��es Or�ament�rias � iniciada.
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

        $.ajax({
            type: 'POST',
            url: '?modulo=inicio&acao=C&requisicao=lista-pedidos&tpdid='+ $(this).attr('tipo') +'&dados='+ $('#filtroalteracao').serialize(),
            success: function(dados){
                $('#div-principal').html(dados);
            }
        });

    });

}
