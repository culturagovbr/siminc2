
    /**
     * A��es efetuadas quando a tela de cadastro de PI � iniciada.
     * 
     * @returns VOID
     */
    function initCadastroPi(){

        $('#btnVoltar').click(function () {
            window.history.back(-1);
        });

        $('#modal-historico-pi').click(function(){
            $('#historico-pi .modal-body').load(urlPagina+'&req=historico-pi&pliid='+ $('#pliid').val());
            $('#historico-pi').modal();
        });
        
        $('#modal-historico-pi-usuario').click(function(){
            $('#historico-pi-usuario .modal-body').load(urlPagina+'&req=historico-pi-usuario&pliid='+ $('#pliid').val());
            $('#historico-pi-usuario').modal();
        });

        $('#btnApagar').click(function(){
            window.location.href = urlPagina+ '&confirmar-apagar=1&pliid='+ $('#pliid').val();
        });
        
        var strComentarioEstadoAtual = $('#div_comentario_estado_atual').html();

        if(strComentarioEstadoAtual != ""){
            setTimeout(function() {
                toastr.options = {
                    closeButton: true,
                    progressBar: true,
                    showMethod: 'slideDown',
                    timeOut: false
                };
                toastr.success(strComentarioEstadoAtual, 'Enviado para corre��o');
            }, 1300);
        }

        $('#input_sei').mask('99999.999999/9999-99');

        var tipoTransacao = $('tipotransacao').value;
        if ('-' == tipoTransacao) {
            // -- Desabilita o restante do formul�rio
            $('plititulo').disable();
            $('plidsc').disable();
            $('btn_selecionar_acaptres').disable();
        }

        // Retira cor dos elementos marcados pela valida��o do formul�rio quando a valida��o for satisfeita(Quando o elemento for preenchido).
        initTirarCorValidacao();


        toggleDelegacao();
        $('#radioDelegacao').change(function(){
            toggleDelegacao();
        });

        // Evento ao mudar op��o de UO
        $('#unicod').change(function(){
            carregarUG($(this).val());
        });

        $('div.div_ungcod').on('change', '#ungcod', function(){
            carregarLimitesUnidade($(this).val());
            carregarMetaPNC($(this).val());
            carregarMetaUnidade($(this).val());
        });
        
        $('#orcamento').on('click', '.btnVisualizarDetalhes', function(){
            visualizarRegistro($(this).attr('ptrid'));
        });

        // Calcula valores Autorizado e Disponivel da Subunidade no quadro de Custeio e Capital
        carregarLimitesUnidade($('#ungcod').val());
        // Calcula valores Autorizado e Disponivel da funcional no quadro de Custeio e Capital
        carregarSaldoFuncional();

        // Evento ao mudar op��o de enquadramento
        $('#eqdid').change(function(){
            mudarFormularioNaoOrcamentario($(this).val());
            mudarFormularioEmenda($(this).val());
            carregarManutencaoItem($(this).val());
        });

        // Evento ao carregar a tela
        mudarFormularioNaoOrcamentario($('#eqdid').val());
        formatarTelaEnquadramentoComManutencaoItem();

        // Evento ao mudar op��o de Objetivos PPA
        $('#oppid').change(function(){
            var delegacao = null;
            if($('#radioDelegacao').is(':checked')){
                delegacao = $('#delegacao').val();
            }
            carregarMetasPPA($(this).val(), null, $('#ungcod').val(), delegacao, null);
            carregarIniciativaPPA($(this).val(), $('#ungcod').val());
        });

        $('#delegacao').change(function(){
            var mppid = $('#mppid').val();
            carregarMetasPPA($('#oppid').val(), null, $('#ungcod').val(), $(this).val(), mppid);
        });

        $('#ungcod').change(function(){
            var mppid = $('#mppid').val();
            var delegacao = null;
            if($('#radioDelegacao').is(':checked')){
                delegacao = $('#delegacao').val();
            }        
            carregarMetasPPA($('#oppid').val(), null, $(this).val(), delegacao, mppid);
        });
        // Evento ao mudar op��o de Metas PNC
        $('div.div_mpnid').on('change', '#mpnid', function(){
            carregarIndicadorPNC($(this).val());
        });

        // Evento ao mudar op��o de �rea Cultural
        $('#mdeid').change(function(){
            carregarSegmentoCultural($(this).val());
        });

        // Evento ao mudar op��o de Manuten��o SubItem
        $('body').on('change', '#maiid', function(){
            carregarManutencaoSubItem($(this).val());
            atualizarTitulo();
        });
        
        // Evento ao mudar op��o de Manuten��o SubItem
        $('body').on('change', '#masid', function(){
            atualizarDescricao();
        });

        $('#btn_adicionar_sniic').click(function(){
             var trHtml =
                '<tr style="height: 30px;" id="tr_sniic_' + $('#input_sniic').val()+ '" >'
                        + '<td style="text-align: left;">' + $('#input_sniic').val() + '</td>'
                        + '<td style="text-align: center;">'
                            + '<input type="hidden" name="lista_sniic[]" value="' + $('#input_sniic').val() + '" />'
                            + '<span class="glyphicon glyphicon-trash link btnRemoveSniic" data-sniic="' + $('#input_sniic').val()+ '" ></span>'
                        + '</td>'
                + '</tr>';
            $('#table_sniic').append(trHtml);
            $('#input_sniic').val('');
        });

        $('#table_sniic').on('click', '.btnRemoveSniic', function(){
            var sniic = $(this).attr('data-sniic');
            $('#tr_sniic_'+ sniic).remove();
        });

        $('#input_sniic').keypress(function(e){
            if(e.which == 13) {
                $('#btn_adicionar_sniic').click();
            }
        });

        $('#btn_adicionar_sei').click(function(){
            var nuSei = $('#input_sei').val();
            nuSei = nuSei.replace(/[^\d]+/g,'');
             var trHtml =
                '<tr style="height: 30px;" id="tr_sei_' + nuSei+ '" >'
                        + '<td style="text-align: left;">' + $('#input_sei').val() + '</td>'
                        + '<td style="text-align: center;">'
                            + '<input type="hidden" name="lista_sei[]" value="' + $('#input_sei').val() + '" />'
                            + '<span class="glyphicon glyphicon-trash link btnRemoveSei" data-sei="' + nuSei+ '" ></span>'
                        + '</td>'
                + '</tr>';
            $('#table_sei').append(trHtml);
            $('#input_sei').val('');
        });

        $('#table_sei').on('click', '.btnRemoveSei', function(){
            var sei = $(this).attr('data-sei');
            $('#tr_sei_'+ sei).remove();
        });

        $('#input_sei').keypress(function(e){
            if(e.which == 13) {
                $('#btn_adicionar_sei').click();
            }
        });

        $('#btn_adicionar_pronac').click(function(){
             var trHtml =
                '<tr style="height: 30px;" id="tr_pronac_' + $('#input_pronac').val()+ '" >'
                        + '<td style="text-align: left;">' + $('#input_pronac').val() + '</td>'
                        + '<td style="text-align: center;">'
                            + '<input type="hidden" name="lista_pronac[]" value="' + $('#input_pronac').val() + '" />'
                            + '<span class="glyphicon glyphicon-trash link btnRemovePronac" data-pronac="' + $('#input_pronac').val()+ '" ></span>'
                        + '</td>'
                + '</tr>';
            $('#table_pronac').append(trHtml);
            $('#input_pronac').val('');
        });

        $('#table_pronac').on('click', '.btnRemovePronac', function(){
            var pronac = $(this).attr('data-pronac');
            $('#tr_pronac_'+ pronac).remove();
        });

        $('#input_pronac').keypress(function(e){
            if(e.which == 13) {
                $('#btn_adicionar_pronac').click();
            }
        });

        $('#btn_selecionar_convenio').click(function(){
            var trHtml =
                '<tr style="height: 30px;" id="tr_convenio_' + $('#input_convenio').val()+ '" >'
                + '<td style="text-align: left;">' + $('#input_convenio').val() + '</td>'
                + '<td style="text-align: center;">'
                + '<input type="hidden" name="lista_convenio[]" value="' + $('#input_convenio').val() + '" />'
                + '<span class="glyphicon glyphicon-trash link btnRemoveConvenio" data-convenio="' + $('#input_convenio').val()+ '" ></span>'
                + '</td>'
                + '</tr>';
            $('#table_convenio').append(trHtml);
            $('#input_convenio').val('');
        });

        $('#table_convenio').on('click', '.btnRemoveConvenio', function(){
            var convenio = $(this).attr('data-convenio');
            $('#tr_convenio_'+ convenio).remove();
        });

        $('#input_convenio').keypress(function(e){
            if(e.which == 13) {
                $('#btn_selecionar_convenio').click();
            }
        });

        $('#picedital').change(function() {
            controlarEdital($('#picedital').is(':checked'));
        });

        $('#table_localizacao').on('click', '.btnRemoverLocalizacao', function(){
            var id = $(this).attr('data-localizacao');
            $('.tr_localizacao_'+ id).remove();
        });

        $('#table_localizacao_estadual').on('click', '.btnRemoverLocalizacaoEstadual', function(){
            var id = $(this).attr('data-localizacao-estadual');
            $('.tr_localizacao_estadual_'+ id).remove();
        });

        $('#table_localizacao_exterior').on('click', '.btnRemoverLocalizacaoExterior', function(){
            var id = $(this).attr('data-localizacao-exterior');
            $('.tr_localizacao_exterior_'+ id).remove();
        });

        $('#table_responsaveis').on('click', '.btnRemoverResponsaveis', function(){
            var cpf = $(this).attr('data-responsaveis');
            $('.tr_responsaveis_'+ cpf).remove();
        });
        
        $('#table_anexos').on('click', '.btnRemoverAnexos', function(){
            var id = $(this).attr('data-anexos');
            $('.tr_anexos_'+ id).remove();
        });

        // Evento ao carregar a tela
        controlarEdital($('#picedital').is(':checked'));
        mudarFormularioEmenda($('#eqdid').val());

        $('#btn_selecionar_functional').click(function(){
            mostrarPopupPtres();
        });

        $('#btn_selecionar_responsaveis').click(function(){
            abrirModalResponsaveis();
        });

        $('#btn_selecionar_localizacao').click(function(){
            abrirModalLocalizacao();
        });

        $('#btn_selecionar_localizacao_estadual').click(function(){
            abrirModalLocalizacaoEstadual();
        });

        $('#btn_selecionar_localizacao_exterior').click(function(){
            abrirModalLocalizacaoExterior();
        });
        
        $('#btn_inserir_anexos').click(function(){
            abrirModalUpload();
        });
        
        $('#btnSalvarAnexo').click(function(){
            $('#formularioAnexo').submit();
        });

        // Evento de terminar de carregar arquivos
        Dropzone.options.formularioAnexoPlanacomorc = {
            init: function() {
                
                this.on("success", function(file, response){
                    var jsonResponse = $.parseJSON(response);
                    inserirNovoAnexo(jsonResponse);
//                    console.log(jsonResponse.arqid);
//                    console.log(jsonResponse.arqnome);
//                    console.log(jsonResponse.arqdescricao);
                });

                this.on("queuecomplete", function(file){
                    // Armazena o objeto Dropzone para chamar m�todos
                    objFormularioAnexo = this;
                    // Chama mensagem de sucesso
                    swal({
                      title: "",
                      text: "Arquivos salvos com sucesso!",
                      timer: 2000,
                      showConfirmButton: false,
                      type: "success"
                    }, function(){
                        // Fecha o swal alert
                        swal.close();
                        // limpa campo de upload
                        objFormularioAnexo.removeAllFiles();
                        // fecha modal ap�s a sele��o
                        $('#modal_upload').modal('hide');
                    });
                });
            }
        };

        $('#esfid').change(function(){
            controlarTipoLocalizacao($(this).val());
        });

        $('#picvalorcusteio').keyup(function(){
            this.value = mascaraglobal('###.###.###.###,##', this.value);
            atualizarValorDoProjeto();
            atualizarValorDetalhado();
            atualizarValorNaoDetalhado();
            if(fnc === false){
                atualizarValorLimiteDisponivelUnidade();
                atualizarValorLimiteDisponivelFuncionalCusteio();
                mudarCorValorProjeto();
            } else {
                if(intEsdid == intEsdidAprovado){
                    atualizarValorLimiteDisponivelUnidade();
                }
                atualizarValorJaCadastradoFuncionalCusteio();
            }
        });

        $('#picvalorcapital').keyup(function(){
            this.value = mascaraglobal('###.###.###.###,##', this.value);
            atualizarValorDoProjeto();
            atualizarValorDetalhado();
            atualizarValorNaoDetalhado();
            if(fnc === false){
                atualizarValorLimiteDisponivelUnidade();
                atualizarValorLimiteDisponivelFuncionalCapital();
                mudarCorValorProjeto();
            } else {
                if(intEsdid == intEsdidAprovado){
                    atualizarValorLimiteDisponivelUnidade();
                }
                atualizarValorJaCadastradoFuncionalCapital();
            }
        });

        $('#picquantidade').keyup(function(){
            mudarCorCronogramaFisico();
            avisarCronogramaFisico();
        });

        $('.input_fisico').keyup(function(){
            mudarCorCronogramaFisico();
            avisarCronogramaFisico();
            atualizarTotalFisico();
        });

        $('#picexecucao').keyup(function(){
            if(parseInt(this.value) > 100){
                this.value = '';
            }
        });
        
        $('.input_orcamentario').keyup(function(){
            this.value = mascaraglobal('###.###.###.###,##', this.value);
            atualizarTotalOrcamentario();
        });
        
        $('.input_orcamentario.custeio').keyup(function(){
            mudarCorCronogramaOrcamentarioCusteio();
        });
        
        $('.input_orcamentario.capital').keyup(function(){
            mudarCorCronogramaOrcamentarioCapital();
        });
        
        $('.input_financeiro.custeio').keyup(function(){
            mudarCorCronogramaFinanceiroCusteio();
        });
        
        $('.input_financeiro.capital').keyup(function(){
            mudarCorCronogramaFinanceiroCapital();
        });

        $('.input_financeiro').keyup(function(){
            this.value = mascaraglobal('###.###.###.###,##', this.value);
            atualizarTotalFinanceiro();
        });

        if(isAdmin){
            // Evento ao mudar clicar no c�digo do PI
            $('#span-plicod').click(function(){
                var codPi = $('#span-plicod').html();
                $('#span-plicod').hide();
                $('#plicod').show().focus();
            });
        }

        $('#pprid').change(function(){
            formatarTelaProdutoNaoAplica($(this).val());
        });

        // Evento ao alterar o valor do c�digo do PI
        $('#plicod').change(function(){
            var plicod = $(this).val();

            $.ajax({
                url: urlPagina+ '&alterarCodigoPi=ok&pliid='+ $('#pliid').val()+ '&plicod='+ $('#plicod').val(),
                dataType: 'json',
                success: function(response){
                    swal({
                        title: response.title,
                        text: response.text,
                        type: response.type
                    });
                    if(response.plicod){
                        $('#span-plicod').html(response.plicod);
                    }
                }
            });
        });

        // Evento ao mudar sair do campo de c�digo do PI
        $('#plicod').blur(function(){
            $('#plicod').hide();
            $('#span-plicod').show();
        });

        $('#capid').change(function(){

            $.ajax({
                url: urlPagina+ '&verificarPactuacaoConvenio=ok&capid='+$('#capid').val(),
                success: function($retorno){
                    if($retorno){
                        $('#div_siconv').show('slow');
                    } else {
                        $('#div_siconv').hide('slow');
                    }
                }
            });

        }).change();

        controlarTipoLocalizacao($('#esfid').val());

        if(!podeEditar){
            // Desabilitando todos os inputs, textareas e selects
            $('#formulario input, #formulario textarea, #formulario select').prop('disabled', true);
            setTimeout($('#formulario select').prop('disabled', true).trigger("chosen:updated"), 3000);

            // Habilitando campos espec�ficos que poder�o ser alterados a qualquer momento
            $('#btn_adicionar_sniic, #input_sniic, #btn_adicionar_sei, #input_sei, ' +
              '#btn_adicionar_pronac, #input_pronac, #btn_selecionar_convenio, #input_convenio, ' +
              '#evento, #pliid, [name="lista_sniic[]"], [name="lista_sei[]"], [name="lista_pronac[]"], [name="lista_convenio[]"]').prop('disabled', false);
        }

        atualizarTotalFisico();
        atualizarTotalOrcamentario();
        atualizarTotalFinanceiro();
        
        mudarCorValoresProjetosFisicoOrcamentarioFinanceiro();
        
        formatarTelaProdutoNaoAplica($('#pprid').val());
        
        // Caso o formul�rio seja de edi��o e exista objetivo cadastrado o sistema impede que o usu�rio mude o objetivo.
        initTravarObjetivo();
        
        // Efetua calculos financeiros de limites para o caso de pi por replica de proposta.
        if(ppiid > 0){
            atualizarValorDoProjeto();
            atualizarValorDetalhado();
            atualizarValorNaoDetalhado();
            atualizarValorLimiteDisponivelFuncionalCapital();
            atualizarValorLimiteDisponivelFuncionalCusteio();
            mudarCorValorProjeto();
            
            atualizarTitulo();
            atualizarDescricao();
        }

        if(fnc === true && $("#ungcod option[value!='']").size() == 0){
            swal({
                title: 'Aten��o',
                text: 'Voc� n�o possui v�nculos a unidades do FNC. Favor, entrar em contato com administrador do sistema para providenciar a vincula��o do seu perfil a alguma unidade.',
                type: 'warning',
                html: true
            }, function(){
                window.location.href = '/planacomorc/planacomorc.php?modulo=principal/unidade/painel&acao=A';
            });
        }
        
        $('.a_espelho_pedido').click(function(){
            var pedid = $(this).attr('data-pedid');
            exibirEspelhoPedido(pedid);
            return false;
        }); 
        
        $('#btn-salvar-etapa').click(function () {
            if (verificaFormEtapas()) {

                var dados;
                if ($("#lista_etapas").length == 0) {
                    dados = '<div id="lista_etapas" class="table-responsive">';
                    dados += '<table class="table table-striped table-bordered table-hover" id="tabela_etapas">';
                    dados += '<thead>';
                    dados += '<tr class="text-center">';
                    dados += '<th width="10%">A��es</th>';
                    dados += '<th>Descri��o</th>';
                    dados += '<th>Data</th>';
                    dados += '</tr>';
                    dados += '</thead>';
                    dados += '<tbody id="teste">';
                    dados += '</tbody>';
                    dados += '</table></div>';
                    $("#div_listagem_etapas").html(dados);

                }
                var indice = $("#tabela_etapas tbody tr").length+1;
                dados = "<tr id='"+indice+"'>";
                dados += '<td class="text-center">';
                dados += '<a title="Excluir" href="javascript:return false;" style="margin-left: 5px;"><i title="Excluir" data-id="'+indice+'"  class="excluir-etapa fa fa-close"></i></a>';
                dados += '</td>';
                dados += '<td><input type="hidden" name="etadsc[]" value="'+$("#etadsci").val()+'">'+$("#etadsci").val()+'</td>';
                dados += '<td><input type="hidden" name="etadata[]" value="'+$("#etadatai").val()+'">'+$("#etadatai").val()+'</td>';
                dados += '</tr>';
                $("#teste").append(dados);
                $('.excluir-etapa').click(function(){
                    var id = $(this).data("id");
                    $("#"+id).remove();
                    if ($("#tabela_etapas tbody tr").length==0){
                        $("#div_listagem_etapas").html('<div class="alert alert-danger">Nenhum registro cadastrado</div>');
                    }
                });
            }else{
                alert("Favor, Preencher os campos!");
            }
        });

        $('div.div_meeid').on('change', '#meuid', function(){
            carregarPlanejamentoEstrategico($(this).val());
        });
    }

/**
 * Exibe popup com Detalhes do Pedido. Tela de Espelho de Pedido.
 *
 * @returns VOID
 */
function exibirEspelhoPedido(pedid){
    window.open(
        window.location.href + '&req=espelho-pedido&pedid='+ pedid,
        'popup_espelho_pedido',
        'width=980,height=1000,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
}

/**
 * Retorna falso caso n�o seja preenchido os campos de Descri��o e Data do box de Etapas.
 *
 * @returns {boolean}
 */
function verificaFormEtapas() {
    var dsc = $("#etadatai").val();
    var data = $("#etadatai").val();

    if( dsc !== '' && dsc !== null ){
        return true;
    }else if( data !== '' && data !== null ){
        return true;
    }

    return false;
}