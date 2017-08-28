
    /**
     * Controla a obrigat�riedade dos campos do formulario de acordo com o enquadramento selecionado.
     *
     * @returns Array
     */
    function definirCamposObrigatorios(){
        var codigoEnquadramento = $('#eqdid').val();
        var listaObrigatorios = ['plititulo', 'plidsc', 'unicod', 'ungcod','eqdid', 'neeid', 'capid', 'mdeid'];

        // Se o formul�rio possui op��es de manuten��o item o sistema define como obrigat�rio o preenchimento dos itens de manuten��o.
        if($('#maiid option').size() > 0){
            listaObrigatorios.push('maiid', 'masid');
        // Se o formulario n�o possui as op��es de manuten��o item o sistema lista como obrigat�rio as op��es Objetivo PPA, Metas PPA, Iniciativa PPA
        } else {
            listaObrigatorios.push('oppid', 'mppid', 'pprid');
            
            // Verifica se o usu�rio escolheu um produto diferente de n�o se aplica para verificar a valida��o do cronograma f�sico.
            if($('#pprid').val() != intProdNaoAplica ){
                listaObrigatorios.push('pumid', 'picquantidade');
            }
        }

        // Se o c�digo for diferente de Finalistico, o sistema n�o define como obrigat�rio o preenchimento das op��es PNC.
        if(codigoEnquadramento == intEnqFinalistico){
            listaObrigatorios.push('mpnid', 'ipnid');
        }

        if($('#picedital').is(':checked')){
            listaObrigatorios.push('mes');
        }

        return listaObrigatorios;
    }

    function submeter(pliid) {
//        var codsubacao = $('#subacao').text();
//        if ('' === codsubacao) {
//            alert('O c�digo da suba��o n�o pode ser deixado em branco.');
//            return false;
//        }

//        O n�mero do PI ser� gerado somente ap�s a aprova��o do PI pela equipe de Coordena��o.
//        var pi = document.getElementById("enquadramento").innerHTML
//            + codsubacao
//            + document.getElementById("nivel").innerHTML
//            + document.getElementById("apropriacao").innerHTML
//            + document.getElementById("codificacao").innerHTML
//            + document.getElementById("modalidade").innerHTML;
//        $("#plicod").val(pi);
//
//        var validado = true;
//        var l = $('#plicodsubacao').val();
//        var sub = l.length;
//        $('plititulo').value = $('prefixotitulo').textContent + $('plititulo').value;

        // Msgs personalizadas de valida��o.
        addMsgCustom = new Array();

        if($('input[name="ptrid"]').size() == 0){
            $('.legend_funcional').addClass('validateRedText');
            addMsgCustom.push('Funcional');
        } else {
            $('.legend_funcional').removeClass('validateRedText');
        }

        // Se n�o for adicionado sniic, acrescenta uma msg ao erro.
    //        if($('[name="lista_sniic[]"]').size() == 0){
    //            addMsgCustom.push('N�meros SNIIC');
    //            $('.legend_sniic').addClass('validateRedText');
    //        } else {
    //            $('.legend_sniic').removeClass('validateRedText');
    //        }
        // Se n�o for adicionado convenio, acrescenta uma msg ao erro.
    //        if($('[name="lista_convenio[]"]').size() == 0){
    //            addMsgCustom.push('N�meros De Conv�nio');
    //            $('.legend_convenio').addClass('validateRedText');
    //        } else {
    //            $('.legend_convenio').removeClass('validateRedText');
    //        }

        // Se n�o for selecionado nenhum tipo de localiza��o, o sistema acrescenta uma mensagem de erro.
        if($('#esfid').val() == ""){
            $('.legend_localizacao').addClass('validateRedText');
            addMsgCustom.push('Localiza��o do Projeto');
        } else {
            switch($('#esfid').val()) {
                // Verifica se a esfera � Estadual/DF.
                case intEsfidEstadualDF:
                    // Verifica se existe n�o foi inserido item na lista de localiza��o municipal.
                    if($('input[name="listaLocalizacaoEstadual[]"]').size() == 0){
                        $('.legend_localizacao').addClass('validateRedText');
                        addMsgCustom.push('Inserir Localiza��o do Projeto Estadual/Distrito Federal');
                    } else {
                        $('.legend_localizacao').removeClass('validateRedText');
                    }
                break;
                // Verifica se a esfera � Exterior.
                case intEsfidExterior:
                    // Verifica se existe n�o foi inserido item na lista de localiza��o no Exterior.
                    if($('input[name="listaLocalizacaoExterior[]"]').size() == 0){
                        $('.legend_localizacao').addClass('validateRedText');
                        addMsgCustom.push('Inserir Localiza��o do Projeto no Exterior');
                    } else {
                        $('.legend_localizacao').removeClass('validateRedText');
                    }
                break;
                // Verifica se a esfera � Municipal.
                case intEsfidMunicipal:
                    // Verifica se existe n�o foi inserido item na lista de localiza��o municipal.
                    if($('input[name="listaLocalizacao[]"]').size() == 0){
                        $('.legend_localizacao').addClass('validateRedText');
                        addMsgCustom.push('Inserir Localiza��o do Projeto Municipal');
                    } else {
                        $('.legend_localizacao').removeClass('validateRedText');
                    }
                break;
                default:
                    $('.legend_localizacao').removeClass('validateRedText');
                break;
            }
        }

        // Se n�o for inserido nenhum Respons�vel, o sistema acrescenta uma mensagem de erro.
        if($('#table_responsaveis input[name="listaResponsaveis[]"]').size() == 0){
            $('.legend_responsaveis').addClass('validateRedText');
            addMsgCustom.push('Respons�veis pelo Projeto');
        } else {
            $('.legend_responsaveis').removeClass('validateRedText');
        }

        // Valida se o usu�rio preencheu Valor do Projeto - Custeio.
        if($('#picvalorcusteio').val() == ""){
            $('#valor_projeto').addClass('validateRedText');
            addMsgCustom.push('Custeio');
        } else {
            $('#valor_projeto').removeClass('validateRedText');
        }

        // Valida se o usu�rio preencheu Valor do Projeto - Capital.
        if($('#picvalorcapital').val() == ""){
            $('#valor_projeto').addClass('validateRedText');
            addMsgCustom.push('Capital');
        } else {
            $('#picvalorcapital').removeClass('validateRedText');
        }

        /*
         *
         * @todo Refatorar esse c�digo de valida��o da parte Custeio e Capital dividindo em fun��es.
         */
        var disponivelUnidade = textToFloat($('#td_disponivel_sub_unidade').text());
        var disponivelFuncional = buscarValorDisponivelFuncional();
        var valorDoProjeto = buscarValorDoProjeto();

        if(valorDoProjeto > disponivelUnidade || valorDoProjeto > disponivelFuncional){
            $('#picvalorcusteio').addClass('validateRedText');
            $('#picvalorcapital').addClass('validateRedText');
            $('#td_valor_projeto').addClass('validateRedText');
            if(valorDoProjeto > disponivelUnidade){
                addMsgCustom.push('Valor do projeto superior ao limite dispon�vel da Sub-Unidade');
            }
            if(valorDoProjeto > disponivelFuncional){
                addMsgCustom.push('Valor do projeto superior ao limite dispon�vel da Funcional');
            }
        } else {
            $('#picvalorcusteio').removeClass('validateRedText');
            $('#picvalorcapital').removeClass('validateRedText');
            $('#td_valor_projeto').removeClass('validateRedText');
        }

        // Verifica se o usu�rio escolheu um enquadramento que n�o tem item de manuten��o para verificar a valida��o do cronograma f�sico.
        if($('#maiid option').size() == 0){

            // Verifica se o usu�rio escolheu um produto diferente de n�o se aplica para verificar a valida��o do cronograma f�sico.
            if($('#pprid').val() != intProdNaoAplica ){

                // Verifica se o cronograma f�sico foi preenchido.
                if(!validarCronogramaFisicoPreenchido()){
                    $('input.input_fisico').addClass('validateRedText');
                    $('#td_total_fisico').addClass('validateRedText');
                    addMsgCustom.push('Cronograma Fis�co');
                } else {
                    $('input.input_fisico').removeClass('validateRedText');
                    $('#td_total_fisico').removeClass('validateRedText');
                }

                // Verifica se o valor do cronograma F�sico � igual ao informado no Produto do PI.
                if(!validarCronogramaFisicoIgualQuantidade()){
                    $('input.input_fisico').addClass('validateRedText');
                    $('#td_total_fisico').addClass('validateRedText');
                    addMsgCustom.push('Soma dos valores do Cronograma Fis�co est� diferente da quantidade informada para o Produto do PI');
                } else {
                    $('input.input_fisico').removeClass('validateRedText');
                    $('#td_total_fisico').removeClass('validateRedText');
                }
            }

        }

        // Verifica se o cronograma or�ament�rio foi preenchido.
        if(!validarCronogramaOrcamentarioPreenchido()){
            $('input.input_orcamentario').addClass('validateRedText');
            $('#td_total_orcamentario_custeio').addClass('validateRedText');
            $('#td_total_orcamentario_capital').addClass('validateRedText');
            addMsgCustom.push('Cronograma Or�ament�rio');
        } else {
            $('input.input_orcamentario').removeClass('validateRedText');
            $('#td_total_orcamentario_custeio').removeClass('validateRedText');
            $('#td_total_orcamentario_capital').removeClass('validateRedText');
        }

        // Verifica se o cronograma financeiro foi preenchido.
        if(!validarCronogramaFinanceiroPreenchido()){
            $('input.input_financeiro').addClass('validateRedText');
            $('#td_total_financeiro').addClass('validateRedText');
            addMsgCustom.push('Cronograma Financeiro');
        } else {
            $('input.input_financeiro').removeClass('validateRedText');
            $('#td_total_financeiro').removeClass('validateRedText');
        }

        // Verifica se o valor do cronograma CUSTEIO � superior ao valor do Projeto.
        if(!validarCronogramaOrcamentarioCusteioIgualValorProjeto()){
            $('.input_orcamentario.custeio').addClass('validateRedText');
            addMsgCustom.push('Soma dos valores de CUSTEIO do Cronograma Or�ament�rio diferente do valor de CUSTEIO do Valor do Projeto');
        } else {
            $('.input_orcamentario.custeio').removeClass('validateRedText');
        }

        // Verifica se o valor do cronograma CAPITAL � superior ao valor do Projeto.
        if(!validarCronogramaOrcamentarioCapitalIgualValorProjeto()){
            $('.input_orcamentario.capital').addClass('validateRedText');
            addMsgCustom.push('Soma dos valores de CAPITAL do Cronograma Or�ament�rio diferente do valor de CAPITAL do Valor do Projeto');
        } else {
            $('.input_orcamentario.capital').removeClass('validateRedText');
        }

        // Verifica se o valor do cronograma CUSTEIO � superior ao valor do Projeto.
        if(!validarCronogramaFinanceiroCusteioIgualValorProjeto()){
            $('.input_financeiro.custeio').addClass('validateRedText');
            addMsgCustom.push('Soma dos valores de CUSTEIO do Cronograma Financeiro diferente do valor de CUSTEIO do Valor do Projeto');
        } else {
            $('.input_financeiro.custeio').removeClass('validateRedText');
        }

        // Verifica se o valor do cronograma CAPITAL � superior ao valor do Projeto.
        if(!validarCronogramaFinanceiroCapitalIgualValorProjeto()){
            $('.input_financeiro.custeio').addClass('validateRedText');
            addMsgCustom.push('Soma dos valores de CAPITAL do Cronograma Financeiro diferente do valor de CAPITAL do Valor do Projeto');
        } else {
            $('.input_financeiro.custeio').removeClass('validateRedText');
        }

        listaObrigatorios = definirCamposObrigatorios();
        validarFormulario(listaObrigatorios, 'formulario', 'validar('+ pliid +')', addMsgCustom);
    }
