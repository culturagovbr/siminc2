/**
 * Fun��es javascript do m�dulo de proposta or�ament�ria - gest�o da ploa.
 * $Id: gestaoploa.js 82067 2014-06-27 13:07:35Z maykelbraz $
 */

/**
 * Carrega de um financeiro para edi��o do valor. Os dados s�o carregados na popup de
 * financeiro. Ap�s carregar os dados exibe a popup de financeiro para o usu�rio
 * prosseguir com as altera��es necess�rias.
 *
 * @param {Integer} dpaid ID despesa a��o.
 * @param {Integer} ploid ID do plano or�ament�rio.
 * @param {Integer} sbaid ID da suba��o.
 * @param {String} ndpcod C�digo da natureza de despesa.
 * @param {String} foncod C�digo da fonte de recursos.
 * @param {Numeric} dpavalor Valor para a a��o considerando grupo e coluna.
 * @returns {undefined}
 */
function editarFinanceiro(dpaid, ploid, ungcod, sbaid, ndpid, ndpcod, foncod, plfvalor)
{
    $('#ploid').val(ploid);
    $('#sbaid').val(sbaid);
    $('#ungcod').val(ungcod);
    $('#ndpid').val(ndpid);
    $('#ndpcod').val(ndpcod);
    $('#foncod').val(foncod);
    $('#plfvalor').val(plfvalor).blur();
    // -- Atualizando todos os chosen da modal
    $('#modal-financeiro .chosen-select').attr('disabled', true).trigger('chosen:updated');

    // -- Setando DPAID em altera��o e setando o tipo de requisi��o
    $('#dpaid').val(dpaid);
    $('#requisicao').val('alterarFinanceiro');

    $('#modal-financeiro').modal();
}

/**
 *
 * @returns {undefined}
 */
function novoFinanceiro()
{
    var ploid = $('#filtro_ploid').val();
    // -- Validando se um plano or�ament�rio foi escolhido antes da inclus�o de uma nova despesa
    if ('' == ploid) {
        $('#modal-alert .modal-body').text('Antes de prosseguir, voc� deve escolher um Plano or�ament�rio.');
        $('#modal-alert').modal();
        return;
    }

    // -- Limpando todas as sele��es e valores
    $('#modal-financeiro .chosen-select').val('').removeAttr('disabled').trigger('chosen:updated');
    $('#plfvalor').val('');
    // -- Setando o PO selecionado para cria��o do novo item
    $('#ploid').val(ploid).attr('disabled', true).trigger('chosen:updated');
    // -- Limpando DPAID e setando o tipo de requisi��o
    $('#requisicao').val('novoFinanceiro');
    $('#modal-financeiro').modal();
}

/**
 * Verifica se h� limite dispon�vel para a coluna e submete o formul�rio financeiro.
 * @returns {undefined}
 */
function salvarFinanceiro()
{
    var novoValor = $('#plfvalor').val();

    if (checaLimite(novoValor.replace(/\./g, ''))) {
        $('#ploid').removeAttr('disabled');
        $('#formfinanceiro').submit();
    } else {
        $('#modal-alert .modal-body').text('N�o existe saldo dispon�vel para a sua solicita��o.');
        $('#modal-alert').modal();
    }
}

/**
 * Verifica se h� limite suficiente para atender a solicita��o.
 * @param {Numeric} novoValor Valor para verifica��o dentro do saldo
 * @returns {Boolean}
 */
function checaLimite(novoValor)
{
    var saldoColuna = $('#saldocoluna').val();
    return (parseFloat(novoValor) <= parseFloat(saldoColuna));
}

function detalharFinanceiro(dpaid, mtrid)
{
    $.post(
        window.location,
        {requisicao:'detalharFinanceiro', 'dados[dpaid]':dpaid, 'dados[mtrid]':mtrid},
        function(html){
            $('#modal-info .modal-body').html(html);
            $('#modal-info').modal();
        }
    );
}

function excluirFinanceiro(dpaid)
{
    $('#dpaid').val(dpaid);
    $('#requisicao').val('excluirFinanceiro');
    $('#modal-confirm').modal();
}

function imprimirRelatorioQDD()
{
    var winrelatorio = window.open('', 'Imprimir', 'height=600,width=800');
    winrelatorio.document.write('<html><head><title></title>');
    winrelatorio.document.write('</head><body>');
    winrelatorio.document.write($('#relatorio-qdd').html());
    winrelatorio.document.write('</body></html>');
    winrelatorio.print();
    winrelatorio.close();
}

/**
 *
 * @param {type} relatorio
 * @returns {undefined}
 */
function exportarRelatorioQDD(relatorio)
{
    var unicod = $('#unicod').val();
    var ppoid = $('#ppoid').val();
    var exercicio = $('#exercicio_spo').val();

    var $formExportarRelatorio = $('#formexportqdd');
    if ($formExportarRelatorio[0]) {
        $('#export_req').val(relatorio);
        $formExportarRelatorio.submit();
        return;
    }

    var $formExportarRelatorio = $('<form />').attr({
        id: 'formexportqdd',
        method: 'post',
        target: 'formexportqdd'
    });
    $formExportarRelatorio.append($('<input />').attr({name:'requisicao', value:relatorio, id:'export_req'}));
    $formExportarRelatorio.append($('<input />').attr({name:'dados[exercicio]', value:exercicio}));
    $formExportarRelatorio.append($('<input />').attr({name:'dados[ppoid]', value:ppoid}));
    $formExportarRelatorio.append($('<input />').attr({name:'dados[unicod]', value:unicod}));
    $formExportarRelatorio.append($('<input />').attr({name:'exportar', value:true}));
    $formExportarRelatorio.append($('<input />').attr({name:'relat', value:'planilha_lista'}));
    $formExportarRelatorio.append($('<input />').attr({name:'planilha', value:4}));

    $('body').append($formExportarRelatorio);
    $formExportarRelatorio.submit();
}

function relatorioElabrev(requisicao, titulo, exercicio, ppoid)
{
    $.post(
        window.location,
        {
            requisicao:requisicao,
            'dados[unicod]':$('#unicod').val(),
            'dados[ppoid]':ppoid,
            'dados[exercicio]':exercicio
        },
        function (html){
            $('#modal-alert .modal-title').text(titulo);
            $('#modal-alert .modal-body').html(html.trim());
            $('#modal-alert').modal();
        }
    );
}

function relatorioElabrevQddPO(exercicio, ppoid)
{
    relatorioElabrev('showQDDPO', 'Quadro de Detalhamento das Despesas - PO', exercicio, ppoid);
}

function relatorioElabrevQddSubacao(exercicio, ppoid)
{
    relatorioElabrev('showQDDSubacao', 'Quadro de Detalhamento das Despesas - Suba��o', exercicio, ppoid);
}

function relatorioElabrevSintese(exercicio, ppoid)
{
    relatorioElabrev('showSinteseDespesas', 'S�ntese de Despesas da UO', exercicio, ppoid);
}
function pesquisaRapida(id){
	 jQuery.expr[':'].contains = function(a, i, m) {
	        return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
	 };

	 $("#"+id).keyup(function()
	 {
		 $('table.table tbody tr td').removeClass('marcado');
	     $('table.table tbody tr').removeClass('remover');
	     stringPesquisa = $("#"+id).val();
	     if (stringPesquisa) {	    	 
	         $('table.table tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
	         $('table.table tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
	     }
	});
}