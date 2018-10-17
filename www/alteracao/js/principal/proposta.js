
/**
 * Ações efetuadas quando a tela de lista de Proposta é iniciada.
 *
 */
function initListaProposta() {

    $('.btn-limpar').click(function () {
        $('#requisicao').val('limpar');
        $('#filtroprop').submit();
    });

    $('.btn-novo').click(function () {
        window.document.location.href = '?modulo=principal/proposta_form&acao=A';
    });

    $('body').on('change', '#suoid', function(){
        carregarFuncional();
    });
    
    $('body').on('change', '#eqdid', function(){
        carregarFuncional();
    });
}
function carregarFuncional() {
//    $.ajax({
//        url: '?modulo=principal/proposta_form&acao=A&req=carregar-funcional-filtros&eqdid=' + $('#eqdid').val() + '&suoid=' + $('#suoid').val(),
//        dataType: 'json',
//        success: function(dados){
//            console.log(dados);
//            $('#span-funcional').html(dados);
//        }
//    });
    $('#span-funcional').load('?modulo=principal/proposta_form&acao=A&req=carregar-funcional-filtros&eqdid=' + $('#eqdid').val() + '&suoid=' + $('#suoid').val());
}