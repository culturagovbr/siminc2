
/**
 * Atualiza os dados de execução orçamentária
 * 
 * @returns VOID Retorna janela modal com aviso pra o usuário
 */
function atualizarSIOP(){
    divCarregando();
    $.ajax({
        type: 'POST',
        url: window.location.href,
        data: 'req=atualizarSIOP',
        dataType: 'json',
        success: function(resp){
            if(resp.success){
                swal({
                        title: '',
                        text: resp.message,
                        type: 'success',
                        showCancelButton: false,
                        confirmButtonText: 'OK',
                        closeOnConfirm: true
                    },
                    function(){
                        window.location.href = window.location.href.replace('&req=atualizar-siop-ajax', '');
                    });
            } else {
                swal({
                        title: 'Erro!',
                        text: resp.message,
                        type: 'error',
                        showCancelButton: false,
                        confirmButtonText: 'OK',
                        closeOnConfirm: true
                    },
                    function(){
                        window.location.href = window.location.href.replace('&req=atualizar-siop-ajax', '');
                    });
            }
        }
    });
}

/**
 * Atualiza os dados de execução orçamentária da própria base do Siminc
 * 
 * @returns VOID Retorna janela modal com aviso pra o usuário
 */
function atualizarSiminc(){
    divCarregando();
    $.ajax({
        type: 'POST',
        url: window.location.href,
        data: 'req=atualizarSIMINC',
        dataType: 'json',
        success: function(resp){
            if(resp.success){
                swal({
                        title: '',
                        text: resp.message,
                        type: 'success',
                        showCancelButton: false,
                        confirmButtonText: 'OK',
                        closeOnConfirm: true
                    },
                    function(){
                        window.location.href = window.location.href;
                    });
            } else {
                swal({
                        title: 'Erro!',
                        text: resp.message,
                        type: 'error',
                        showCancelButton: false,
                        confirmButtonText: 'OK',
                        closeOnConfirm: true
                    },
                    function(){
                        window.location.href = window.location.href;
                    });
            }
        }
    });
}

/**
 * Exibe o modal com grafico detalhado da UO e Subunidade
 * 
 * @param {int} tipoGraficoSubunidade
 * @param {string} tipoValor
 * @param {string} sigla
 * @returns VOID Exibe janela modal com o grafico
 */
function exibirModalDetalheGrafico(tipoGraficoSubunidade, tipoValor, sigla){
    var urlBase = 'planacomorc.php?modulo=inicio&acao=C';
    if(tipoGraficoSubunidade === 1){
        var parametroTipoGrafico = '&req=detalhe-grafico-subunidade';
    } else {
        var parametroTipoGrafico = '&req=detalhe-grafico-uo';
    }
    
    // Dotação ou Planejado
    if(tipoValor.substr(0, 3) === "Dot" || tipoValor.substr(0, 3) === "Pla"){
        $('#detalhe-grafico .modal-body').load(urlBase+ '&tipo_valor='+ tipoValor.substr(0, 3)+ '&sigla='+ sigla+ parametroTipoGrafico);
        $('#detalhe-grafico').modal();
    }
}