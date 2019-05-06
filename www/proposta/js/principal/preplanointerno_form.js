
/**
 * Comportamentos executados no momento em que a tela está pronta e carregada.
 * 
 * @returns VOID
 */
function initPreplanointernoForm(){

    toggleItem();
    recuperarValoresLimitesPtres();
    
    controlarExibicaoFormularioReduzido();
    controlarExibicaoUnidadeMedidaQuantidade($('#pprid').val());
 
    $('#eqdid').change(function(){
        $('#span-item').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-item&eqdid=' + $(this).val(), function(){
            toggleItem();
        });
        
        // Exibe ou Oculta os campos do formulário de acordo com o enquadramento selecionado
        controlarExibicaoFormularioReduzido();
        
        $('#pprid').val(intProdNaoAplica).trigger("chosen:updated");
        controlarExibicaoUnidadeMedidaQuantidade(intProdNaoAplica);
    });

    $('#eqdid, #suoid').change(function(){
        // Busca opções de funcionais de acordo com a Subunidade e Enquadramento
        $('#span-funcional').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-funcional&eqdid=' + $('#eqdid').val() + '&suoid=' + $('#suoid').val());
        // Busca valores de limites da Subunidade
        recuperarValoresLimitesSubUnidade();
    });

    $('#suoid').change(function(){
        // Busca opções de Metas PNC de acordo com a Subunidade
        $('#span-metapnc').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-metapnc&suoid=' + $('#suoid').val());
    });

    $('#mdeid').change(function(){
        $('#span-segmento').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-segmento&mdeid=' + $(this).val());
    });

    $('body').on('change', '#maiid', function(){
        $('#span-subitem').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-subitem&maiid=' + $(this).val());
    });

    $('body').on('change', '#mpnid', function(){
        $('#span-indicadorpnc').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-indicadorpnc&mpnid=' + $(this).val());
    });
    
    $('#pprid').change(function(){
        controlarExibicaoUnidadeMedidaQuantidade($(this).val());
    });

    $('#oppid').change(function(){
        $('#span-metappa').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-metappa&oppid=' + $('#oppid').val() + '&suoid=' + $('#suoid').val());
        $('#span-iniciativappa').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-iniciativappa&oppid=' + $('#oppid').val());
    });

    $('.valorPlanoInterno').bind('keyup change paste', function(){
        atualizaLimiteDisponivelUnidade();
        criticarValorDoProjeto();
    });
    
    $('body').on('change', '#ptrid', function(){
        $.ajax({
            url: '?modulo=principal/preplanointerno_form&acao=A&req=recuperar-objetivoppa&ptrid=' + $(this).val(),
            success: function(oppid){
                if(oppid){
                    $('#oppid').val(oppid).prop('readonly', 'readonly').trigger("chosen:updated").change();
                } else {
                    $('#oppid').val('').prop('readonly', false).trigger("chosen:updated").change();
                }
            }
        });
        recuperarValoresLimitesPtres();
    });

    $('#esfid').change(function(){
        $('.select-localizacao').hide('slow');
        $('#div-localizacao_' + $('#esfid').val()).show('slow');
    }).change();

    $('#btn-salvar').click(function(){

        if(validarValorDoProjeto() === false){
            swal('Atenção', 'O Limite Disponível na Unidade foi ultrapassado. Favor rever valores preenchidos no Custeio e Capital', 'error');
            return false;
        }

        controlarExibicaoFormularioReduzido();
        controlarExibicaoUnidadeMedidaQuantidade($('#pprid').val());

        $('#formulario').find("button[type='submit']").click();
    });

    $('#importar-pi-btn').on('click', function () {
        var modal = $('#preplanointerno_modal');
        modal.modal();
        modal.find('.modal-body').load('proposta.php?modulo=principal/preplanointerno_form&acao=A&req=proposta_modal', function () {
            $('#preplanointerno_modal').find('table').DataTable({
                responsive: true,
                dom: '<"html5buttons"B>lTfgitp',
                "language": {
                    "url": "/zimec/public/temas/simec/js/plugins/dataTables/Portuguese-Brasil.json"
                }
            });
        });

        modal.on('hide.bs.modal', function () {
            modal.find('.modal-body').html('');
        });
    });

    $(document).on('click', 'a.pi-importer', function () {
        var piID = $(this).data('pi-id');
        importarPIDeAnosAnteriores( piID );
    });
}

function criticarValorDoProjeto(){
    if(validarValorDoProjeto() === false){
        swal(
            'Atenção',
            'O Limite Disponível na Unidade foi ultrapassado. Favor rever valores preenchidos no Custeio e Capital',
            'error');
    }
}

/**
 * Verifica se o valor do projeto não ultrapassa o limite disponivel na unidade.
 * 
 * @returns {boolean}
 */
function validarValorDoProjeto(){
    var limiteDisponivelMaiorOuIgual = true;
    
    // Busca valor total disponivel informado pela base de dados sem o valor do próprio PI em edição.
    var limiteDisponivelBd = $('#input_bd_disponivel_unidade_bd').val()? str_replace(['.', ','], ['', '.'], $('#input_bd_disponivel_unidade_bd').val()): 0;
    
    // Busca valor da soma dos dados preenchidos do usuário em tempo real de custeio e capital.
    var totalCusteioCapital = somarCampos('valorPlanoInterno');
    
    // Calcula o valor disponivel menos o valor digitado em custeio e capital pelo usuário
    var disponivelSubunidade = parseFloat(limiteDisponivelBd) - parseFloat(totalCusteioCapital);
    
    if(disponivelSubunidade < 0){
        limiteDisponivelMaiorOuIgual = false;
    }
    
    return limiteDisponivelMaiorOuIgual;
}

/**
 * Atualiza em tempo de execução o valor disponível na unidade deduzindo ou 
 * somando com o valor que o usuário está preenchendo em custeio e capital.
 * 
 * @returns VOID
 */
function atualizaLimiteDisponivelUnidade(){
    // Busca valor total disponivel informado pela base de dados sem o valor do próprio PI em edição.
    var limiteDisponivelBd = $('#input_bd_disponivel_unidade_bd').val()? str_replace(['.', ','], ['', '.'], $('#input_bd_disponivel_unidade_bd').val()): 0;
    // Busca valor da soma dos dados preenchidos do usuário em tempo real de custeio e capital.
    var totalCusteioCapital = somarCampos('valorPlanoInterno');
    // Calcula o valor disponivel menos o valor digitado em custeio e capital pelo usuário
    var disponivelSubunidade = parseFloat(limiteDisponivelBd) - parseFloat(totalCusteioCapital);
    
    // Exibe na tela o valor disponivel na Unidade já deduzindo com o valor que o usuário preencheu do Projeto
    $('#td_disponivel_sub_unidade').html(number_format(disponivelSubunidade, 0, ',', '.'));
}

function toggleItem(){
    if($('#maiid option').size() > 1){
        $('#span-item, #span-subitem').show();
    } else {
        $('#span-item, #span-subitem').hide();
    }
}

/**
 * Busca valores de limites da Subunidade
 * 
 * @returns VOID
 */
function recuperarValoresLimitesSubUnidade(){
    $.ajax({
        url: '?modulo=principal/preplanointerno_form&acao=A&req=recuperar-limite'+
        '&suoid='+ $('#suoid').val()+
        '&eqdid='+ $('#eqdid').val()+
        '&pliid='+ $('#pliid').val(),
        dataType: 'json',
        success: function(dados){
            $('#td_autorizado_sub_unidade').html(number_format(parseFloat(dados.limite_unidade), 0, ',', '.'));
            $('#input_bd_disponivel_unidade_bd').val(parseInt(dados.disponivel_unidade));
            atualizaLimiteDisponivelUnidade();
        }
    });
}

/**
 * Busca valores de limites da funcional
 * 
 * @returns void
 */
function recuperarValoresLimitesPtres(){
    $.ajax({
        url: '?modulo=principal/preplanointerno_form&acao=A&req=recuperar-valores-ptres&ptrid=' + $('#ptrid').val(),
        dataType: 'json',
        success: function(dados){
            $('#td_disponivel_funcional_custeio').html(number_format(parseFloat(dados.custeioptres), 0, ',', '.'));
            $('#td_disponivel_funcional_capital').html(number_format(parseFloat(dados.capitalptres), 0, ',', '.'));
        }
    });
}

/**
 * Importa dados de PI da Capitação executiva(Módulo Planejamento Orçamentário) 
 * para o formulário de pré-pi.
 * 
 * @param integer piID
 * @returns VOID
 */
function importarPIDeAnosAnteriores(piID) {
    $('#preplanointerno_modal').modal('hide');
    console.log('Foobar', piID);

    $.ajax({
        url: '?modulo=principal/preplanointerno_form&acao=A&req=importar-pi&pliid=' + piID,
        success: function(oppid){
            /*if(oppid){
                $('#oppid').val(oppid).prop('readonly', 'readonly').trigger("chosen:updated").change();
            } else {
                $('#oppid').val('').prop('readonly', false).trigger("chosen:updated").change();
            }*/
        }
    });
    recuperarValoresLimitesPtres();
}

/**
 * Busca opções de Metas PPA e Iniciativas PPA de acordo com a Subunidade e Objetivo
 * 
 * @todo separar responsabilidades, sendo cada responsabilidade pra um método
 * @returns {undefined}
 */
function recuperarMetasEIniciativaPPA() {
    $('#span-metappa').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-metappa&oppid=' + $('#oppid').val() + '&suoid=' + $('#suoid').val());
    $('#span-iniciativappa').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-iniciativappa&oppid=' + $('#oppid').val());
}

/**
 * Verifica se o formulário é reduzido ou completo.
 * 
 * @returns boolean retorna true se o formulário for reduzido.
 */
function verificarFormularioReduzido(){
    var resultado = false;
    if($.inArray($('#eqdid').val(), listaEqdReduzido) >= 0){
        resultado = true;
    }

    return resultado;
}

/**
 * Controla e formata a exibição do formulario conforme a opção de enquadramento
 * está configurada pra ser reduzida ou não.
 * 
 * @returns {void}
 */
function controlarExibicaoFormularioReduzido(){
    if(verificarFormularioReduzido()){
        // Oculta campos
        $('.div_metas').hide();
        $('#span-area').hide();
        $('#span-segmento').hide();
        
        // Retira obrigatoriedade e Apaga os valores dos campos ocultados
        $('#oppid').attr('required', false); $('#oppid').val('').trigger("chosen:updated");
        $('#mppid').attr('required', false); $('#mppid').val('').trigger("chosen:updated");
        $('#ippid').val('').trigger("chosen:updated");
        $('#mpnid').attr('required', false); $('#mpnid').val('').trigger("chosen:updated");
        $('#ipnid').attr('required', false); $('#ipnid').val('').trigger("chosen:updated");
        // Area
        $('#mdeid').attr('required', false); $('#mdeid').val('').trigger("chosen:updated");
        // Segmento
        $('#secid').attr('required', false); $('#secid').val('').trigger("chosen:updated");
    } else {
        // Exibe campos
        $('.div_metas').show();
        $('#span-area').show();
        $('#span-segmento').show();
        
        // Coloca obrigatoriedade
        $('#oppid').attr('required', 'required');
        $('#mppid').attr('required', 'required');
        $('#mpnid').attr('required', 'required');
        $('#ipnid').attr('required', 'required');
        //'required'
        $('#mdeid').attr('required', 'required');
        // Seg'required'
        $('#secid').attr('required', 'required');
    }
}

/**
 * Formata a tela para exibir ou não as opções Unidade de Medida, Quantidade quando o usuário
 * selecionar "Não se aplica".
 * 
 * @returns VOID
 */
function controlarExibicaoUnidadeMedidaQuantidade(codigo){
    if(codigo == intProdNaoAplica){
        // Oculta campos
        $('#span_unidade_medida').hide();
        $('#span_quantidade').hide();
        
        // Retira obrigatoriedade e Apaga o preenchimento dos campos ocultados
        $('#pumid').attr('required', false); $('#pumid').val('').trigger("chosen:updated");
        $('#pliquantidade').attr('required', false); $('#pliquantidade').val('');
    } else {
        // Exibe os campos
        $('#span_unidade_medida').show();
        $('#span_quantidade').show();
        
        // Retira obrigatoriedade e Apaga o preenchimento dos campos ocultados
        $('#pumid').attr('required', 'required');
        $('#pliquantidade').attr('required', 'required');
    }
}

