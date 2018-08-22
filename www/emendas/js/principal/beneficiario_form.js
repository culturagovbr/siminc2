
/**
 * Ações a serem realizadas quando a tela de beneficiario for carregada.
 * 
 * @returns VOID
 */
function beneficiario_form_init(){

    $('#modal-historico-beneficiario').click(function(){
        $('#historico-beneficiario .modal-body').load(/*urlPagina+'&req=historico-beneficiario&pliid='+ $('#pliid').val()*/);
        $('#historico-beneficiario').modal();
    });

    $('.coluna-parecer').click(function(){
        // Inverte título com conteúdo
        var title = $(this).attr('title');
        var html = $(this).html();

        $(this).attr('title', html);
        $(this).html(title);
    });

    toggleMotivo();
    $('input[name="benimpedimento"]').change(function(){
        toggleMotivo();
    });

    toggleDelegacao();
    $('#radioDelegacao').change(function(){
        toggleDelegacao();
    });

    $('#esfid').change(function(){
        $('.select-localizacao').hide('slow');
        $('#div-localizacao_' + $('#esfid').val()).show('slow');
    }).change();

    $('#bennumeroprocesso').mask('99999.999999/9999-99');

    $('#mdeid').change(function(){
        $('#span-segmento').load('?modulo=principal/beneficiario_form&acao=A&req=carregarSegmento&mdeid=' + $(this).val());
    });

    $('#capid').change(function(){

        $.ajax({
            url: 'emendas.php?modulo=principal/beneficiario_form&acao=A&req=verificarPactuacaoConvenio&capid='+$('#capid').val(),
            success: function($retorno){
                if($retorno){
                    $('.dados-siconv').show('slow');
                } else {
                    $('.dados-siconv').hide('slow');
                }
            }
        });

    }).change();

    // Evento ao mudar opção de Objetivos PPA
    $('#oppid').change(function(){
        var delegacao = null;
        if($('#radioDelegacao').is(':checked')){
            delegacao = $('#delegacao').val();
        }
        carregarMetasPPA($(this).val(), null, $('#suoid').val(), delegacao, null);
        carregarIniciativaPPA($(this).val());
    });
    
    $('#delegacao').change(function(){    
        var mppid = $('#mppid').val();
        carregarMetasPPA($('#oppid').val(), null, $('#suoid').val(), $(this).val(), mppid);
    });
    
    $('#suoid').change(function(){
        var mppid = $('#mppid').val();
        var delegacao = null;
        if($('#radioDelegacao').is(':checked')){
            delegacao = $('#delegacao').val();
        }        
        carregarMetasPPA($('#oppid').val(), null, $(this).val(), delegacao, mppid);
    });
//    
    // Evento ao mudar opção de Metas PNC
    $('#mpnid').change(function(){
        carregarIndicadorPNC($(this).val());
    });

    $('#btn_inserir_anexos').click(function(){
        abrirModalUpload();
    });

    // Evento de terminar de carregar arquivos
    Dropzone.options.formularioAnexo = {
        init: function() {

            this.on("success", function(file, response){
                var jsonResponse = $.parseJSON(response);
                inserirNovoAnexo(jsonResponse);
            });

            this.on("queuecomplete", function(file){

                // Armazena o objeto Dropzone para chamar métodos
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
                    // fecha modal após a seleção
                    $('#modal_upload').modal('hide');
                });
            });
        }
    };

    $('#table_anexos').on('click', '.btnRemoverAnexos', function(){
        var id = $(this).attr('data-anexos');
        $('.tr_anexos_'+ id).remove();
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

    $('.a_espelho').click(function(){
        var pliid = $(this).attr('data-pi');
        exibirEspelhoPi(pliid);
        return false;
    });

    toggleModadelidadeObrigatorio();

    $('#bented').change(function () {
        toggleModadelidadeObrigatorio();
    });

}

function toggleMotivo(){
    if($('#benimpedimento_1').is(':checked')){
        $('.div_motivo').show('slow');
    } else {
        $('#impid').val('').trigger("chosen:updated");
        $('.div_motivo').hide('slow');
    }
}

function toggleDelegacao(){
    if($('#radioDelegacao').is(':checked')){
        $('#div_unidades_delegadas').show('slow');
    } else {
        $('#div_unidades_delegadas').hide('slow');
        $('#delegacao').val('').trigger("chosen:updated");
    }
}

/**
 * Carrega novo conteúdo para o select de Metas PPA via requisição ajax.
 *
 * @param {integer} oppid
 * @param {integer} mppid
 * @param {integer} suoid
 * @param {array} suoid
 * @returns {VOID}
 */
function carregarMetasPPA(oppid, mppid, suoid, delegacao, mppid) {
    var requisicao = '?modulo=principal/beneficiario_form&acao=A';
    if (delegacao!==null){
        delegacao.push(suoid);
    }else{
        delegacao = suoid;
    }
    carregarSelectPorJson(
        requisicao,
        '#mppid',
        'codigo',
        'descricao',
        {
            'req': 'carregarMetasPPA',
            'oppid': oppid,
            'suoid': delegacao
        },
        'Selecione',
        mppid,
        function(){
            $(".chosen-select").chosen();
            $('#mppid').val(mppid);
        }
    );
}

/**
 * Carrega novo conteúdo para o select de Metas PPA via requisição ajax.
 *
 * @param {integer} codigo
 * @returns {VOID}
 */
function carregarIniciativaPPA(codigo){
    var requisicao = '?modulo=principal/beneficiario_form&acao=A';

    carregarSelectPorJson(
        requisicao,
        '#ippid',
        'codigo',
        'descricao',
        {
            'req': 'carregarIniciativaPPA',
            'oppid': codigo
        },
        'Selecione',
        '',
        function(){
            $(".chosen-select").chosen();
        }
    );
}

/**
 * Carrega novo conteúdo para o select de Indicadores PNC via requisição ajax.
 *
 * @param {integer} codigo
 * @returns {VOID}
 */
function carregarIndicadorPNC(codigo) {
    var requisicao = '?modulo=principal/beneficiario_form&acao=A';

    carregarSelectPorJson(
        requisicao,
        '#ipnid',
        'codigo',
        'descricao',
        {
            'req': 'carregarIndicadorPNC',
            'mpnid': codigo
        },
        'Selecione',
        '',
        function(){
            $(".chosen-select").chosen();
        }
    );
}

function abrirModalUpload() {
    $('.modal_dialog_upload').show();
    $('#modal_upload').modal();
    $('#modal_upload .chosen-container').css('width', '100%');
}

function inserirNovoAnexo(json){
    var trHtml = '<tr style="height: 30px;" class="tr_anexos_'+ json.arqid +'" >'
        + '                    <td style="text-align: left;"><a href="#" onclick="javascript:abrirArquivo('+ json.arqid+ '); return false;" >'+ json.arqnome +'</a></td>'
        + '                    <td style="text-align: left;">'+ json.arqdescricao +'</td>'
        + '                    <td style="text-align: center;">'
        + '                         <input type="hidden" value="'+ json.arqid +'" name="listaAnexos[]">'
        + '                         <span class="glyphicon glyphicon-trash btnRemoverAnexos link" title="Excluir o arquivo '+ json.arqnome+ '" data-anexos="'+ json.arqid +'" >'
        + '                    </td>'
        + '                </tr>'
    ;
    $('#table_anexos').append(trHtml);
}

/**
 * Exibe popup com Detalhes do pi. Tela de Espelho de PI.
 *
 * @returns VOID
 */
function exibirEspelhoPi(pliid){
    window.open(
        '?modulo=principal/beneficiario_form&acao=A&req=espelho-pi&pliid='+ pliid,
        'popup_espelho_pi',
        'width=780,height=1000,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
}

function toggleModadelidadeObrigatorio () {
    if($('#bented').is(':checked')) {
        $('#capid').removeAttr('required');
        $('#capid').parent().parent().find('label .campo-obrigatorio').hide()
    } else {
        $('#capid').attr('required', 'required');
        $('#capid').parent().parent().find('label .campo-obrigatorio').show()
    }
}
