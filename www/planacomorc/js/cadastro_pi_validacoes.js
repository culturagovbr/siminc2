
    function submeter(pliid) {

        // Msgs personalizadas de valida��o.
        addMsgCustom = new Array();

        if(!verificarFormularioNaoOrcamentario()){
            if($('input[name="ptrid"]').size() == 0){
                $('.legend_funcional').addClass('validateRedText');
                addMsgCustom.push('PTRES(Funcional)');
            } else {
                $('.legend_funcional').removeClass('validateRedText');
            }
        }

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

        if(!verificarFormularioNaoOrcamentario()){
            // Valida se o usu�rio preencheu ou o valor de capital ou de custeio(Valor do Projeto).
            if(buscarValorDoProjeto() <= 0){
                $('#valor_projeto').addClass('validateRedText');
                addMsgCustom.push('Valor do Projeto(Custeio ou Capital)');
            } else {
                $('#valor_projeto').removeClass('validateRedText');
            }
            
            // Valida se o valor do projeto pra Custeio n�o foi superior ao valor disponivel de Custeio da funcional.
            // implements here...
            // Valida se o valor do projeto pra Custeio n�o foi superior ao valor disponivel de Custeio da funcional.
            // implements here...
        }

        /*
         *
         * @todo Refatorar esse c�digo de valida��o da parte Custeio e Capital dividindo em fun��es.
         */
        var disponivelUnidade = textToFloat($('#td_disponivel_sub_unidade').text());
        var disponivelFuncionalCusteio = textToFloat($('#td_disponivel_funcional_custeio').text());
        var disponivelFuncionalCapital = textToFloat($('#td_disponivel_funcional_capital').text());
        
        
        // Se o usu�rio estiver abaixando o valor em rela��o ao valor salvo na base de dados o sistema n�o valida se o valor ultrapassou o limite permitindo ajuste entre os PIs.
        if(!(buscarValorBaseDoProjeto() > buscarValorDoProjeto())){
            // Verifica se o valor do projeto � superior ao limite dispon�vel da Subunidade
            if(disponivelUnidade < 0){
                $('#picvalorcusteio').addClass('validateRedText');
                $('#picvalorcapital').addClass('validateRedText');
                if(disponivelUnidade < 0){
                    if(fnc === true){
                        addMsgCustom.push('Valor do projeto superior ao limite dispon�vel do FNC');
                    } else {
                        addMsgCustom.push('Valor do projeto superior ao limite dispon�vel da Unidade');
                    }
                }
            } else if(disponivelFuncionalCusteio < 0 || disponivelFuncionalCapital < 0) {
                if(disponivelFuncionalCusteio < 0){
                    $('#picvalorcusteio').addClass('validateRedText');
                    addMsgCustom.push('Valor de Custeio do projeto superior ao limite dispon�vel da Funcional');
                }

                if(disponivelFuncionalCapital < 0){
                    $('#picvalorcapital').addClass('validateRedText');
                    addMsgCustom.push('Valor de Capital do projeto superior ao limite dispon�vel da Funcional');
                }
            } else {
                $('#picvalorcusteio').removeClass('validateRedText');
                $('#picvalorcapital').removeClass('validateRedText');
            }
        }

        // Verifica se o usu�rio escolheu um enquadramento que � do tipo formulario reduzido para verificar a valida��o do cronograma f�sico.
        if(!verificarFormularioReduzido()){

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

        if(!verificarFormularioNaoOrcamentario()){
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
        }

        listaObrigatorios = definirCamposObrigatorios();
        validarFormulario(listaObrigatorios, 'formulario', 'validar('+ pliid +')', addMsgCustom);
    }

    /**
     * Controla a obrigat�riedade dos campos do formulario de acordo com o enquadramento selecionado.
     *
     * @returns Array
     */
    function definirCamposObrigatorios(){
        var codigoEnquadramento = $('#eqdid').val();
        var listaObrigatorios = ['plititulo', 'plidsc', 'unicod', 'ungcod','eqdid', 'capid'];

        // Verifica se o formul�rio � reduzido ou completo.
        if(verificarFormularioReduzido()){
            // Se o formul�rio possui op��es de manuten��o item o sistema define como obrigat�rio o preenchimento dos itens de manuten��o.
            if($('#maiid option').not('option[value=""]').size() > 0){
                listaObrigatorios.push('maiid', 'masid');
            // Se o formulario n�o possui as op��es de manuten��o item o sistema lista como obrigat�rio as op��es Objetivo PPA, Metas PPA, Iniciativa PPA
            }
        } else {
            listaObrigatorios.push('oppid', 'mppid', 'pprid', 'mdeid', 'neeid', 'mpnid', 'ipnid');

            // Verifica se o usu�rio escolheu um produto diferente de n�o se aplica para verificar a valida��o do cronograma f�sico.
            if($('#pprid').val() != intProdNaoAplica ){
                listaObrigatorios.push('pumid', 'picquantidade');
            }
        }

        if($('#picedital').is(':checked')){
            listaObrigatorios.push('mes');
        }

        return listaObrigatorios;
    }

