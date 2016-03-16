/**
 * Sistema SCRUM
 * @package simec
 * @subpackage scrum
 */

/**
 * Move um item para a coluna anterior (sprint -> proximaSprint, proximaSprint -> backlog).
 * @param $item O Item que deve ser movido.
 * @param coluna A coluna de destino do item.
 */
function moveParaColuna($item, coluna) {
    // -- Movendo o postit para a coluna de destino
    $item.hide().insertBefore(jQuery('#'+coluna+' div:eq(0)')).fadeIn(100);
    // -- Disparando o evento de atualiza��o no servidor
    $item.removeClass('green yellow blue').addClass('gray');
    atualizaPosicaoItem($item);
}

/**
 * Verifica se uma coluna pode receber uma determinada quantidade de horas.
 * @param horaID ID do texto contendo o valor que deve ser verificado.
 * @param horasParaAdicionar Quantidade de horas que ser�o adicionadas.
 * @return Se a sprint ou pr�xima sprint conseguem incorporar a qtd adicional de horas.
 */
function cabeNaColuna(horaID, horasParaAdicionar)
{
    var totalHoras = parseInt(jQuery('#totalHoras').text());
    var qtdHorasGastas = parseInt(jQuery('#'+horaID).text());
    return (totalHoras >= (qtdHorasGastas + horasParaAdicionar));
}

/**
 * Adiciona uma quantidade de horas ao contador de horas da sprint ou da pr�xima sprint.
 * @param horaID ID do texto contendo o valor que deve ser atualizado.
 * @param horasParaAdicionar Quantidade de horas que ser�o adicionadas.
 */
function adicionaHoras(horaID, horasParaAdicionar)
{
    var horasGastas = parseInt(jQuery('#'+horaID).text());
    jQuery('#'+horaID).text(horasGastas + horasParaAdicionar);
}

/**
 * Subtrai uma quantidade de horas do contador de horas da sprint ou da pr�xima sprint.
 * @param horaID ID do texto contendo o valor que deve ser atualizado.
 * @param horasParaSubtrair Quantidade de horas que ser�o subtra�das.
 */
function subtrairHoras(horaID, horasParaSubtrair)
{
    var horasGastas = parseInt(jQuery('#'+horaID).text());
    jQuery('#'+horaID).text(horasGastas - horasParaSubtrair);
}

/**
 * Callback utilizada para atualizar a posi��o de um item. Faz uma requisi��o ao servidor
 * informando a nova posi��o do item para que seja persistida.
 * Faz chamadas para realoca��o de items quando uma coluna j� est� com as horas esgotadas.
 */
function atualizaPosicaoItem($item)
{
    var itemID = $item.attr('id');
    var itemHoras = parseInt(jQuery('.hora', $item).text());
    var columnID = $item.parent().attr('id');
    var columnClass = $item.parent('td').attr('class');
    jQuery('#'+itemID+' .loading').css('display', 'block');

    // -- Verificando se tem vaga para o item na nova coluna
    var colunaRemanejamento = {
        'sprint': 'proximasprint',
        'proximasprint': 'backlog'
    };
    // -- Antes de fazer a movimenta��o do item manipulado pelo usu�rio,
    // -- verifica se o item pode ser alocado naquela coluna e, se necess�rio,
    // -- Faz o remanejamento dos �ltimo �tens da coluna.
    switch (columnID) {
        case 'sprint':
            // -- No break
        case 'proximasprint':
            while (!cabeNaColuna(columnID+'Hora', parseInt(jQuery('.hora', $item).text()))) {
                var $ultimoItem = jQuery('#'+columnID+' .postit').last();
                var ultimoItemHoras = parseInt(jQuery('.hora', $ultimoItem).text());
                moveParaColuna($ultimoItem, colunaRemanejamento[columnID]);
                subtrairHoras(columnID+'Hora', ultimoItemHoras);
            }
            break; 
    }
    
//    var arrClass = columnClass.split(' ');
//    var arrClass = arrClass.split('_');
//    console.log(columnClass);
//    console.log(arrClass);
//    return false;
//    var arrClass = columnClass.split('_');
    
    // -- Requisi��o ao servidor de persist�ncia da posi��o do item
    jQuery.ajax({
        type:'POST',
        url:'scrum.php?modulo=principal/priorizacao/update&acao=A',
        data:'entid='+itemID+
             '&column='+columnID+
             '&columnClass='+columnClass+
             '&pos='+$item.index()+
             '&prgid='+jQuery('#prgid').val(),
         // -- Sucesso na requisi��o @TODO Adicionar valida��o do retorno.
        success:function(data){
            console.log(data);
//            switch (columnID){
//                case 'sprint':
//                    jQuery('#'+itemID).addClass('green');
//                    adicionaHoras('sprintHora', itemHoras);
//                    break;
//                case 'proximasprint':
//                    jQuery('#'+itemID).addClass('yellow');
//                    adicionaHoras('proximasprintHora', itemHoras);
//                    break;
//                case 'backlog':
//                    jQuery('#'+itemID).addClass('blue');
//                    break;
//            }
//            jQuery('#'+itemID+' .loading').css('display', 'none');
//            jQuery('#'+itemID).removeClass('gray');
        },
        // -- Erro na requisi��o
        error:function(data){
            alert('N�o foi poss�vel executar sua requisi��o.');
            jQuery('.column').sortable('cancel');
        }
    });
}

function mostraPopupEdicao(entid, uiPostIt)
{
    var popwidth = 450;
    jQuery('#dialog-editar-entregavel').dialog({
        width:popwidth,
        modal:true,
        center:true,
        position:[(jQuery(window).width() / 2) - (popwidth / 2) - 7, 250],
        buttons:{
            'Confirmar':function(){
                var objValues = {
                    entid: entid
                  , entdsc: jQuery('#entdsc').val()
                  , enthrsexec: jQuery('#enthrsexec').val()
                  , usucpfresp: jQuery('#usucpfresp').val()
                  , usucpfresp_dsc: jQuery('#usucpfresp_dsc').val()
                };

                if ('' == objValues.entdsc) {
                    alert('O campo "Descri��o" n�o pode ser deixado em branco.');
                    jQuery('#entdsc').focus();
                    return;
                }
                if ('' == objValues.enthrsexec) {
                    alert('O campo "Dura��o da tarefa" n�o pode ser deixado em branco.');
                    jQuery('#enthrsexec').focus();
                    return;
                }
                if ('' == objValues.usucpfresp) {
                    alert('O campo "Respons�vel" n�o pode ser deixado em branco.');
                    jQuery('usucpfresp').focus();
                    return;
                }

                // -- Requisi��o de update da demanda
                updateEntregavelEDemanda(this, objValues);
                updateInterface(uiPostIt, objValues);
            },
            'Cancelar':function(){
                jQuery(this).dialog('close');
            }
        },
        open:function(){
            jQuery('#usucpfresp_dsc').val('Selecione...');
            jQuery('#usucpfresp').attr('value', '');
            jQuery(".ui-dialog-titlebar").prepend('<img src="css/imagens/loading_red2.gif" id="title-loading" />');
            jQuery.ajax({
                type:'POST',
                url:'scrum.php?modulo=principal/escopo/entregavel&acao=A',
                data:'entid='+entid+'&action=jsonResponsavelTempoExecucao',
                success:function(data){
                    jQuery('#title-loading').remove();
                    if (!data['error']) {
                        for (x in data) {
                            jQuery('#'+x).val(data[x]);
                        }
                    }
                    // -- Recalculando a quantidade de caracteres
                    jQuery('#entdsc').keyup();
                }
            });
        },
        close:function(){
            jQuery('#title-loading').remove();
        }
    });
}

/**
 * Atualiza a interface depois de salvar os dados
 * @param {type} uiPostIt
 * @param {type} obj
 * @returns {Boolean}
 */
function updateInterface(uiPostIt, obj) {
    
    var header = $(uiPostIt).find(".body span.estoria").clone();
    $(uiPostIt).find(".header span.hora").html(obj.enthrsexec);
    $(uiPostIt).find(".body div.content").html("").append(header).append(obj.entdsc);
    
    var resp = '';
    if (obj.usucpfresp_dsc.length > 21) {
        for(var i=0; i < 21; i++) {
            resp += obj.usucpfresp_dsc[i];
        }
        resp+='...';
    } else {
        resp = obj.usucpfresp_dsc;
    }
    
    $(uiPostIt).find(".footer .responsavel").html(resp);
    return false;
}

function updateEntregavelEDemanda(btnConfirmar, obj)
{
    //entid, entdsc, enthrsexec, usucpfresp
    jQuery.ajax({
        type:'POST',
        url:'scrum.php?modulo=principal/escopo/entregavel&acao=A',
        data:'entid='+obj.entid+'&entdsc='+obj.entdsc
          + '&enthrsexec='+obj.enthrsexec+'&usucpfresp='+obj.usucpfresp+'&action=jsonUpdateEntregavel',
        success:function(data){
            jQuery('#dialog-editar-entregavel').dialog('close');
        },
        error:function(data){
            jQuery('#dialog-editar-entregavel').dialog('close');
        }
    });
}

// -- Inicializa��o do componente de ordena��o do board.
jQuery(document).ready(function(){
    jQuery('.column').sortable({
        connectWith:'.column', // -- Classe dos containers que podem receber itens
        handle: '.header',
        revert:300,
        placeholder:'gap', // -- Classe do placeholder
        start:function(e,ui){
            var $item = ui.item;
            var itemHoras = parseInt(jQuery('.hora', $item).text());
            var columnID = $item.parent().attr('id');
            subtrairHoras(columnID+'Hora', itemHoras);
            $item.addClass('gray').removeClass('green yellow blue');
        },
        stop:function(e,ui){
            atualizaPosicaoItem(ui.item);
        }
    });
    // -- Exibindo e ocultando toolbar
    jQuery('.body').mouseover(function(){
        jQuery('.toolbar', this).css('display', 'block');
        jQuery(this).height(140);
    }).mouseout(function(){
        jQuery('.toolbar', this).css('display', 'none');
        jQuery(this).height(115);
    });
    // -- Edi��o da hist�ria
    jQuery('.edit-item').click(function(){
        var entid = jQuery(this).parent().attr('id')
          , postIt = $(this).parent().parent().parent();
        mostraPopupEdicao(entid.substr(2), postIt);
    });
    // -- Adicionando carregando ao titulo da dialog
    jQuery('.ui-dialog-title').prepend('<img src="css/imagens/loading.gif">');
});