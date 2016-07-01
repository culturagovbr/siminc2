/**
 * Exibe as op��es da fun��o selecionada
 * @param {string} tipo ID da div de op��es que deve ser exibida.
 * @returns {undefined}
 */
function mostraCombo(tipo){
  // -- Escondendo as op��es de filtro e/ou chamadas
  $('.chamadaWs').each(function() {
    $(this).next().next().hide();
  });
  // -- Esconde todos os filtros
  $('.filtroWs').hide();
  // -- Exibindo op��es da fun��o selecionada
  $('#'+tipo).show();
  // -- Exibindo os filtros relacionados � fun��o escolhida
  $('.'+tipo).show();
}

/**
 *  Fun��o mostra combo para o componente de comboPopup().
 *  @param obj Radio button (de fun��o) que recebeu o click.
 */
function mostraCombo2(obj)
{
    $(obj).closest('tr').next().toggle()
                               .children('.SubTituloDireita').click();
}

/**
 * Rola a tela para poder visualizar o campo indicado em referencia.
 * @param {string} referencia Elemento utilizado como referencia para a rolagem da tela.
 */
function rolaTela(referencia) {
  $('html, body').animate({scrollTop: $('#'+referencia)
    .offset().top - 100}, 500);
}

/**
 * Faz a valida��o b�sica do formul�rio e executa a valida��o de cada p�gina, se
 * estiver tudo ok, exibe a popup de autentica��o do WS.
 * @returns {Boolean}
 */
function solicitarExecucao() {
  var docSelecionado = false;
  // -- Verificando se ao menos uma chamada ao webService foi solicitada.
  $('.chamadaWs').each(function(){
    if (this.checked) {
      docSelecionado = true;
      return;
    }
  });

  if (!docSelecionado) { return alert('Selecione uma fun��o.'); }

  // -- Valida��o particular da chamada para cada pagina. A fun��o "validacaoAdicional"
  // -- deve ser implementada na p�gina que precisa de valida��es adicionais.
  if ((typeof(validacaoAdicional) === 'function') && (true !== validacaoAdicional())) {
    return false;
  }

  // -- Exibindo div de autentica��o no WS
  $('#div_auth').show();

  // -- Rolando a p�gina para o formul�rio de autentica��o
  rolaTela('formulario');
}

/**
 * Verfica os campos obrigat�rios do ws (user, pass, momento) e submete o formul�rio.
 * @returns {unresolved}
 */
function enviaSolicitacao() {
  if (!$('#wsusuario').attr('value')) { return alert('Favor informar o usu�rio!'); }
  if (!$('#wssenha').attr('value')) { return alert('Favor informar a senha!'); }
  if (('function' != typeof(ignorarMomento)) && $('#codigomomento')[0]
          && !$('#codigomomento').attr('value')) {
    return alert('Favor informar o C�digo do Momento!');
  }
  selectAll();
  $('#requisicao').val('enviasolicitacao');
  $('#formulario').submit();
}

/**
 * Exibe os filtros relacionados ao id selecionado.
 * @param {string} opcaoID 
 * @returns {undefined}
 */
function mostraFiltros(opcaoID) {
  // -- Esconde todos os filtros
  $('.filtroWs').hide();
  // -- Exibindo op��es de filtro conforme id do elem. selecionado
  // -- os filtros associados ao elemento tem em sua lista de classes o id
  // -- deste elemento
  $('.' + opcaoID).show();
}

/**
 * Marca e desmarca um conjunto de checkboxes de acordo com o id de checkbox.
 * @param {htmlObject} input
 * @param {id} tipo
 * @returns {undefined}
 */
function marcarTodos(input, tipo) {
  if ($(input).attr('checked')) {
    $('.check_'+tipo).attr('checked', true);
  } else {
    $('.check_'+tipo).attr('checked', false);
  }
}

/**
 * Seleciona todos os itens conforme o radio de fun��o selecionado.
 */
function selectAll()
{
    var documento = $('input[type=radio]:checked').attr('value');
    $('#acaid_'+documento+' option').attr('selected', 'selected');
}