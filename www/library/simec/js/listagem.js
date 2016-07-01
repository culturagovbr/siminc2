/**
 * Indica se o arquivo de js da listagem j� est� carregado
 * @returns {undefined}
 * @version: $Id: listagem.js 102252 2015-09-09 19:21:57Z maykelbraz $
 */
function listagem_js_carregado()
{
}

/**
 * Fun��o JS utilizada pela classe Simec_Listagem para tratar a a��o de expandir linhas.
 *
 * @param {string} acao A��o passada como requisi��o para a p�gina de processamento.
 * @param {type} dados Dados passados para executar a requisi��o.
 * @param {integer} id ID da linha que expandida.
 */
function expandirLinha(acao, dados, id)
{
    var id = '#arow-' + id;
    var $parentTR = $(id).closest('tr');
    var numAcao;
    var td_acao;

    // -- Identificando o estado atual do item
    if ($(id + ' span').hasClass('glyphicon-plus')) { // -- N�o est� detalhado, ent�o exibe detalhe
        $.post(
            window.location,
            {requisicao:acao,dados:dados},
            function(html){
                $(id + ' span').removeClass('glyphicon-plus').addClass('glyphicon-minus');
                var numCols = $('td', $parentTR).length;

                numAcao = $parentTR.parents('table').attr('data-qtd-acoes');

                td_acao = '<td colspan="'+ numAcao +'">&nbsp;</td>';

                $parentTR.after('<tr>'+td_acao+'<td colspan="' + numCols + '">' + html + '</td></tr>');
            }
        );
    } else { // -- Item j� est� detalhado, ent�o oculta detalhe
        $parentTR.next().remove();
        $(id + ' span').removeClass('glyphicon-minus').addClass('glyphicon-plus');
    }
}

/**
 * Verifica se o nome de uma fun��o � uma fun��o definida no sistema.
 * @param {string} cb Nome da fun��o callback para verifica��o
 * @returns {Boolean}
 */
function verificaCallback(cb)
{
    return typeof Function == typeof window[cb];
}

/**
 * Atribui os eventos de click para as a��es do tipo Select. Esta a��o funciona<br />
 * basicamente como um um checkbox, mas que permite que seja executada uma fun��o<br />
 * de callback. A assinatura da fun��o de callback deve ser como segue:<br />
 * <pre>function nomeDaCallback(id, estado, extras){}</pre>
 * <ul><li><b>id</b>: Campo de identifica��o da linha - primeira coluna da query, como de costume.</li>
 * <li><b>estado</b>: Indica se o resultado do click ser� o campo marcado, ou desmarcado.</li>
 * <li><b>extras</b>: Reservado. Ser� utilizado para passar informa��es adicionais � fun��o.</li></ul>
 * @returns {undefined}
 */
function delegateAcaoSelect()
{
	$(document).on('click', '.tabela-listagem span.glyphicon-ok', function(e){
        var id = $(e.target).attr('data-id');
        var callback = $(e.target).attr('data-cb');
        var extraParams = $(e.target).attr('data-exp');

        if (undefined !== extraParams) {
            extraParams = JSON.parse(extraParams.replace(/\'/g, '"'));
        } else {
            extraParams = {};
        }

        if (!verificaCallback(callback)) {
            alert("A fun��o '" + callback + "' n�o est� definida.");
            return;
        }

        window[callback](id, false, extraParams);
        $(e.target).removeClass('glyphicon-ok').addClass('glyphicon-remove').css('color', 'gray');
    }).on('click', '.tabela-listagem span.glyphicon-remove', function(e){
        var id = $(e.target).attr('data-id');
        var callback = $(e.target).attr('data-cb');
        var extraParams = $(e.target).attr('data-exp');

        if (undefined !== extraParams) {
            extraParams = JSON.parse(extraParams.replace(/\'/g, '"'));
        } else {
            extraParams = {};
        }

        if (!verificaCallback(callback)) {
            alert("A fun��o '" + callback + "' n�o est� definida.");
            return;
        }

        window[callback](id, true, extraParams);
        $(e.target).removeClass('glyphicon-remove').addClass('glyphicon-ok').css('color', 'green');
    });
}

if (!verificaCallback('delegatePaginacao')) {
    /**
     * Atribui os eventos de click para todos os paginadores da tela.
     * @returns {undefined}
     */
    delegatePaginacao = function()
    {
        $('body').on('click', '.container-listing li[class="pgd-item"]:not(".disabled")', function(){
            // -- definindo a nova p�gina
            var novaPagina = $(this).attr('data-pagina');
            $('#listagem-p').val(novaPagina);

            // -- Submetendo o formul�rio
            $(this).parents('div').find('form.form-listagem').eq(0).submit();
        });
    };
}

// -- @todo: Colocar para detectar automaticamente por uma classe no formul�rio
if (!verificaCallback('delegateFormFiltroPadrao')) {
    /**
     * Transfere os filtros padr�o da listagem para o formul�rio de controle interno da listagem e os<br />
     * submete junto com os demais dados.
     * @returns {undefined}
     */
    function delegateFormFiltroPadrao()
    {
        $('body').on('submit', 'form.form-listagem', function(){
            var formListagem = $(this);
            var formFiltro = formListagem.attr('data-form-filtros');
            if (!formFiltro) {
                return true;
            }
            // -- Adicionando os campos do formul�rio de filtros ao formul�rio da listagem
            formFiltro = '#' + formFiltro;
            // -- Radios / selects / text / hidden
            $(formFiltro).find('input[type="radio"]:checked, select, input[type="text"],input[type="hidden"]').each(function(){
                var $input = $(this);
                if ($input.attr('name') && '' !== $input.val()) {

// -- @todo: Avaliar impacto desta altera�ao, envio de elementos multiplos do formul�rio como array e n�o lista em formato de string
//                    var valueInput = $(this).val();
//                    if (typeof [] === typeof valueInput) {
//                        for (var x in valueInput) {
//
//                            if (typeof Function === typeof valueInput[x]) {
//                                continue;
//                            }
//
//                            $('<input>').attr({
//                                type:'hidden',
//                                value:valueInput[x],
//                                name:$(this).attr('name') + '[]'
//                            }).appendTo(formListagem);
//                        }
//                    } else {
                        $('<input>').attr({
                            type:'hidden',
                            value:$(this).val(),
                            name:$(this).attr('name')
                        }).appendTo(formListagem);
//                    }
                }
            });
        });
    }
}

function enableWorkflowPopovers()
{
    $('.tabela-listagem a.workflow').click(function(e){
        e.preventDefault();

        // -- Escondendo os popovers ativos
        $('.popover-shown').popover('destroy').removeClass('popover-shown');

        // -- Configurando o popover
        $(this).addClass('popover-shown').popover({
            html: true,
            trigger: 'focus',
            title: 'Estado atual',
            content: function(){
                var action = $(this).attr('data-action'),
                    // -- @todo: Pensar em como fazer sem eval
                    params = eval($(this).attr('data-params'));

                $.post(
                    window.location,
                    {requisicao: action, params: params},
                    function(response){
                        $('.popover-content').empty().html(
                            response.replace(/<br\/>/g, '')
                        );
                    }
                );
            }
        // -- E exibindo
        }).popover('show');
    });
}

function enableDownloadPopovers()
{
    // -- Popover de download de arquivos com v�rias extensoes
    $('.tabela-listagem a.multi-download').popover({
        html: true,
        title: 'Escolha um formato',
        trigger: 'focus',
        content: function(){
            var func = $(this).attr('data-cb'),
                params = $(this).attr('data-params'),
                types = JSON.parse($(this).attr('data-types').replace(/'/g, '"'));

            // -- Processando os tipos dispon�veis para download
            var html = '';
            for (var x in types) {
                // -- Ignorando elementos do array que n�o s�o strings
                if (typeof '' !== typeof types[x]) {
                    continue;
                }

                html += '<button type="button" class="btn btn-success multi-download" data-cb="' + func + '" data-params="' + params + '" data-type="' + types[x] + '">'
                      + '<span class="glyphicon glyphicon-save"></span> ' + types[x].toUpperCase() + '</button>&nbsp;';
            }
            return html;
        }
    });

    // -- A��es dos bot�es de download, por tipo - S�o os bot�es que aparecem dentro da popover
    $('.tabela-listagem').on('click', 'button.multi-download', function(e){
        var params = JSON.parse($(this).attr('data-params').replace(/'/g, '"')),
            type = $(this).attr('data-type'),
            callback = $(this).attr('data-cb');

            console.log(callback);

        window[callback](params, type);
    });
}

/**
 * Recebe um pesquisator e retorna a tabela mais pr�xima dele.
 *
 * @param {htmlObject} Inst�ncia do pesquisator.
 * @returns {jQuery} Tabela de lista mais pr�xima do pesquisator.
 */
function getTabelaDoPesquisator(pesquisator)
{
    return $(pesquisator).closest('nav').next('table.tabela-listagem');
}

/**
 * Ativa o pesquisator relacionando-o � tabela de listagem na sequencia.
 */
function enablePesquisator()
{
    $.expr[':'].contains = function(a, i, m){
        return $(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
    };

    $(document).on('keyup', 'input.busca-listagem', function(){
        var $tbody = getTabelaDoPesquisator(this).children('tbody');

        $('tr td', $tbody).removeClass('listagem-marcado');
        $('tr', $tbody).removeClass('listagem-remover');
        stringPesquisa = $(this).val();
        if (stringPesquisa) {
            $('tr td:contains(' + stringPesquisa + ')', $tbody).addClass('listagem-marcado');
            $('tr:not(:contains(' + stringPesquisa + '))', $tbody).addClass('listagem-remover');
        }
    }).on('focus', 'input.busca-listagem', function(){
        getTabelaDoPesquisator(this).addClass('table-listagem-filtrando');
    }).on('blur', 'input.busca-listagem', function(){
        getTabelaDoPesquisator(this).removeClass('table-listagem-filtrando');
    });

    $('.navbar-pesquisa span').popover(
        {container:'body', placement:'top', trigger:'hover', title:'Ajuda - Pesquisa r&aacute;pida', html:true,
         content:'Exibe apenas as linhas da p&aacute;gina atual<br /> que apresentem o texto digitado.'}
    );
    $('.navbar-listagem button[data-popover!=""]').popover(
        {container:'body', placement:'top', trigger:'hover', html:true}
    );
}

function enableExportXLS()
{
    $('body').on('click', '.navbar-listagem .btn-xls', function(){
        var $form = $('#listagem-form-export');

        if (!$form[0]) {
            $form = $('<form />', {method:'POST', action:window.location.href, id:'listagem-form-export', target:'_blank'});
            $('<input />', {type:'hidden', name:'listagem[requisicao]', value:'exportar-xls'}).appendTo($form);
            $('body').append($form);
        }

        $form.submit();
    });
}

$(document).ready(function(){
    delegateAcaoSelect();
    delegatePaginacao();
    delegateFormFiltroPadrao();
    enableDownloadPopovers();
    enableWorkflowPopovers();
    enablePesquisator();
    enableExportXLS();

    // -- Adicionando o CSS da listagem ao documento
    $('head').append('<link rel="stylesheet" type="text/css" href="/library/simec/css/listagem.css" />');
});

$(function(){
    $('.campo_filtro').keyup(function(e) {
        if(e.which == 13) {
            $(this).eq(0).closest('form').submit();
        }
    });

    $('.campo_ordenacao').click(function() {
        $(this).closest('form').find('.input_ordenacao').val($(this).attr('campo-ordenacao'));
        $(this).closest('form').submit();
    });
});
