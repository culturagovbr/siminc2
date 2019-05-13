
    /**
     * Formata a tela para não exibir Unidade, quantidade e cronograma físico quando o usuário
     * selecionar "Não se aplica" ou opções de mesmo sentido.
     * 
     * @returns VOID
     */
    function formatarTelaProdutoNaoAplica(codigo){
        if(codigo == intProdNaoAplica){
            $('.div_unidade_medida').hide('slow');
            $('#pumid').val('').trigger("chosen:updated");
            $('.div_quantidade_produto').hide('slow');
            $('#picquantidade').val('');
            // Oculta as colunas e campos do Cronograma Físico
            $('.td_cronograma_fisico').hide('slow');
            // Apaga os dados do cronograma Físico
            $('.input_fisico').val('');
        } else {
            $('.div_unidade_medida').show();
            $('.div_quantidade_produto').show();
            // Exibe as colunas e campos do Cronograma Físico
            $('.td_cronograma_fisico').show();
        }
    }

    /**
     * Atualiza o Titulo do PI conforme a opção selecionada em Manutenção Item.
     * 
     * @returns VOID
     */
    function atualizarTitulo(){
        var titulo = $('#maiid :checked').text();
        if(titulo != '' && titulo != 'Selecione'){
            $('#plititulo').val(titulo);
        }
    }
    
    /**
     * Atualiza a descrição do PI conforme a opção selecionada em Manutenção Subitem.
     * 
     * @returns VOID
     */
    function atualizarDescricao(){
        var descricao = $('#masid :checked').text();
        if(descricao != '' && descricao != 'Selecione'){
            $('#plidsc').val(descricao);
        }
    }

   /**
    * Valida informações de valores do Projeto(Capital e Custeio), Fisicos,
    * Orçamentários e Financeiros e caso estejam com inconformidades mudam a cor pra vermelho.
    *
    * @returns VOID
    */
    function mudarCorValoresProjetosFisicoOrcamentarioFinanceiro(){
        mudarCorCronogramaFisico();
        mudarCorCronogramaOrcamentarioCusteio();
        mudarCorCronogramaOrcamentarioCapital();
        mudarCorCronogramaFinanceiroCusteio();
        mudarCorCronogramaFinanceiroCapital();
    }

    /**
     * Retira cor dos elementos marcados pela validação do formulário quando a validação for satisfeita(Quando o elemento for preenchido).
     *
     * @returns VOID
     */
    function initTirarCorValidacao(){
        var listaiten = definirCamposObrigatorios();
        listaiten.push('mpnid', 'ipnid');

        $.each(listaiten , function(i , id){
            $('#formulario').on('change', '#' + id, function(){
                tirarCorVermelha(id);
            });
        });
    }

    /**
     * Tira a Cor Vermelha da validação do label do elemento e da tag pai do elemento.
     *
     * @param string id
     * @returns VOID
     */
    function tirarCorVermelha(id){
        $item = $('#' + id);
        $($item).parent().parent().removeClass('has-error');
        $('label[for="'+ $item.attr('id') +'"]').removeClass('has-error');
    }

    /**
     * Exibe orientações ao usuário para preencher corretamente o Cronograma.
     *
     * @returns VOID
     */
    function avisarCronogramaFisico(){
        if(!validarCronogramaFisicoInferiorQuantidade()){
            alert('<p style="text-align: justify;">&nbsp; &nbsp; Não será possível salvar o formulário com a soma de todos os valores preenchidos nos meses do cronograma físico superior ao valor informado na Quantidade do Produto do PI.<br />&nbsp; &nbsp; <span style="color: #ff0000;">Por favor, diminua os valores informados na coluna do cronograma físico ou aumente a Quantidade no Produto do PI.</span></p>');
        }
    }

    /**
     * Exibe orientações ao usuário para preencher corretamente o Cronograma.
     *
     * @returns VOID
     */
    function avisarCronogramaOrcamentarioSuperiorCusteio(){
        if(!validarCronogramaOrcamentarioSuperiorValorProjetoCusteio()){
            alert('<p style="text-align: justify;">&nbsp; &nbsp; Não será possível salvar o formulário com a soma de todos os valores de CUSTEIO preenchidos nos meses do cronograma orçamentário superior ao valor informado no campo de CUSTEIO do Valor do Projeto.<br />&nbsp; &nbsp; <span style="color: #ff0000;">Por favor, diminua os valores informados nos campos de CUSTEIO do cronograma orçamentário ou aumente o valor de CUSTEIO do Valor do Projeto.</span></p>');
        }
    }
    
   /**
    * Valida se o valor preenchido no cronograma orçamentário é inferior ou igual ao valor declarado em custeio para o valor do projeto.
    * 
    * @return boolean
    */
    function validarCronogramaOrcamentarioSuperiorValorProjetoCusteio(){
        var resultado = false;
        var valorCusteio = textToFloat($('#picvalorcusteio').val());
        var valorTotalCusteioCronograma = buscarTotalCusteioCronogramaOrcamentario();
        
        if(valorTotalCusteioCronograma <= valorCusteio){
            resultado = true;
        }
        
        return resultado;
    }
    
    /**
     * Exibe orientações ao usuário para preencher corretamente o Cronograma.
     *
     * @returns VOID
     */
    function avisarCronogramaOrcamentarioSuperiorCapital(){
        if(!validarCronogramaOrcamentarioSuperiorValorProjetoCapital()){
            alert('<p style="text-align: justify;">&nbsp; &nbsp; Não será possível salvar o formulário com a soma de todos os valores de CAPITAL preenchidos nos meses do cronograma orçamentário superior ao valor informado no campo de CAPITAL do Valor do Projeto.<br />&nbsp; &nbsp; <span style="color: #ff0000;">Por favor, diminua os valores informados nos campos de CAPITAL do cronograma orçamentário ou aumente o valor de CAPITAL do Valor do Projeto.</span></p>');
        }
    }
    
   /**
    * Valida se o valor preenchido no cronograma orçamentário é inferior ou igual ao valor declarado em custeio para o valor do projeto.
    * 
    * @return boolean
    */
    function validarCronogramaOrcamentarioSuperiorValorProjetoCapital(){
        var resultado = false;
        var valorCapital = textToFloat($('#picvalorcapital').val());
        var valorTotalCapitalCronograma = buscarTotalCapitalCronogramaOrcamentario();
        
        if(valorTotalCapitalCronograma <= valorCapital){
            resultado = true;
        }
        
        return resultado;
    }
    
    /**
     * Exibe orientações ao usuário para preencher corretamente o Cronograma.
     *
     * @returns VOID
     */
    function avisarCronogramaFinanceiroSuperiorCapital(){
        if(!validarCronogramaFinanceiroSuperiorValorProjetoCapital()){
            alert('<p style="text-align: justify;">&nbsp; &nbsp; Não será possível salvar o formulário com a soma de todos os valores de CAPITAL preenchidos nos meses do cronograma financeiro superior ao valor informado no campo de CAPITAL do Valor do Projeto.<br />&nbsp; &nbsp; <span style="color: #ff0000;">Por favor, diminua os valores informados nos campos de CAPITAL do cronograma financeiro ou aumente o valor de CAPITAL do Valor do Projeto.</span></p>');
        }
    }
    
   /**
    * Valida se o valor preenchido no cronograma orçamentário é inferior ou igual ao valor declarado em custeio para o valor do projeto.
    * 
    * @return boolean
    */
    function validarCronogramaFinanceiroSuperiorValorProjetoCapital(){
        var resultado = false;
        var valorCapital = textToFloat($('#picvalorcapital').val());
        var valorTotalCapitalCronograma = buscarTotalCapitalCronogramaFinanceiro();
        
        if(valorTotalCapitalCronograma <= valorCapital){
            resultado = true;
        }
        
        return resultado;
    }
    
    /**
     * Exibe orientações ao usuário para preencher corretamente o Cronograma.
     *
     * @returns VOID
     */
    function avisarCronogramaFinanceiroSuperiorCusteio(){
        if(!validarCronogramaFinanceiroSuperiorValorProjetoCusteio()){
            alert('<p style="text-align: justify;">&nbsp; &nbsp; Não será possível salvar o formulário com a soma de todos os valores de CUSTEIO preenchidos nos meses do cronograma financeiro superior ao valor informado no campo de CUSTEIO do Valor do Projeto.<br />&nbsp; &nbsp; <span style="color: #ff0000;">Por favor, diminua os valores informados nos campos de CUSTEIO do cronograma financeiro ou aumente o valor de CUSTEIO do Valor do Projeto.</span></p>');
        }
    }
    
   /**
    * Valida se o valor preenchido no cronograma orçamentário é inferior ou igual ao valor declarado em custeio para o valor do projeto.
    * 
    * @return boolean
    */
    function validarCronogramaFinanceiroSuperiorValorProjetoCusteio(){
        var resultado = false;
        var valorCusteio = textToFloat($('#picvalorcusteio').val());
        var valorTotalCusteioCronograma = buscarTotalCusteioCronogramaFinanceiro();
        
        if(valorTotalCusteioCronograma <= valorCusteio){
            resultado = true;
        }
        
        return resultado;
    }

    /**
     * Muda a cor dos valores do cronograma Físico se o valor exceder o valor do projeto.
     *
     * @returns VOID
     */
    function mudarCorCronogramaFisico(){
        if(validarCronogramaFisicoInferiorQuantidade()){
            $('input.input_fisico').removeClass('validateRedText');
            $('#td_total_fisico').removeClass('validateRedText');
        } else {
            $('input.input_fisico').addClass('validateRedText');
            $('#td_total_fisico').addClass('validateRedText');
        }
    }

    /**
     * Muda a cor dos valores das colunas de Custeio do cronograma orçamentário se o valor exceder o valor do projeto.
     *
     * @returns VOID
     */
    function mudarCorCronogramaOrcamentarioCusteio(){
        if(validarCronogramaOrcamentarioInferiorValorProjetoCusteio()){
            $('.input_orcamentario.custeio').removeClass('validateRedText');
            $('#td_total_orcamentario_custeio').removeClass('validateRedText');
        } else {
            $('.input_orcamentario.custeio').addClass('validateRedText');
            $('#td_total_orcamentario_custeio').addClass('validateRedText');
        }
    }
    
    /**
     * Muda a cor dos valores das colunas de Capital do cronograma orçamentário se o valor exceder o valor do projeto.
     *
     * @returns VOID
     */
    function mudarCorCronogramaOrcamentarioCapital(){
        if(validarCronogramaOrcamentarioInferiorValorProjetoCapital()){
            $('.input_orcamentario.capital').removeClass('validateRedText');
            $('#td_total_orcamentario_capital').removeClass('validateRedText');
        } else {
            $('.input_orcamentario.capital').addClass('validateRedText');
            $('#td_total_orcamentario_capital').addClass('validateRedText');
        }
    }

    /**
     * Muda a cor dos valores do cronograma financeiro se o valor exceder o valor do projeto.
     *
     * @returns VOID
     */
    function mudarCorCronogramaFinanceiroCusteio(){
        if(validarCronogramaFinanceiroInferiorValorProjetoCusteio()){
            $('.input_financeiro.custeio').removeClass('validateRedText');
            $('#td_total_financeiro_custeio').removeClass('validateRedText');
        } else {
            $('.input_financeiro.custeio').addClass('validateRedText');
            $('#td_total_financeiro_custeio').addClass('validateRedText');
        }
    }
    
    /**
     * Muda a cor dos valores do cronograma financeiro se o valor exceder o valor do projeto.
     *
     * @returns VOID
     */
    function mudarCorCronogramaFinanceiroCapital(){
        if(validarCronogramaFinanceiroInferiorValorProjetoCapital()){
            $('.input_financeiro.capital').removeClass('validateRedText');
            $('#td_total_financeiro_capital').removeClass('validateRedText');
        } else {
            $('.input_financeiro.capital').addClass('validateRedText');
            $('#td_total_financeiro_capital').addClass('validateRedText');
        }
    }

    /**
     * Verifica se o cronograma está preenchido.
     *
     * @returns Boolean
     */
    function validarCronogramaFisicoPreenchido(){
        var valido = false;
        var totalFisico = buscarTotalCronogramaFisico();

        if(totalFisico > 0){
            valido = true;
        }

        return valido;
    }

    /**
     * Verifica se o cronograma está preenchido com valores superiores a Quantidade do Produto do PI.
     *
     * @returns Boolean
     */
    function validarCronogramaFisicoInferiorQuantidade(){
        var valido = false;
        var quantidade = textToFloat($('#picquantidade').val());
        var totalFisico = buscarTotalCronogramaFisico();
        if(totalFisico <= quantidade){
            valido = true;
        }

        return valido;
    }

    /**
     * Verifica se a coluna de custeio do cronograma está preenchido com valores inferiores ao valor do projeto.
     *
     * @returns Boolean
     */
    function validarCronogramaOrcamentarioInferiorValorProjetoCusteio(){
        var valido = false;
        var valorDoProjeto = textToFloat($('#picvalorcusteio').val());
        var valorTotalOrcamentario = buscarTotalCusteioCronogramaOrcamentario();

        if(valorTotalOrcamentario <= valorDoProjeto){
            valido = true;
        }

        return valido;
    }
    
    /**
     * Verifica se a coluna de capital do cronograma está preenchido com valores inferiores ao valor do projeto.
     *
     * @returns Boolean
     */
    function validarCronogramaOrcamentarioInferiorValorProjetoCapital(){
        var valido = false;
        var valorDoProjeto = textToFloat($('#picvalorcapital').val());
        var valorTotalOrcamentario = buscarTotalCapitalCronogramaOrcamentario();

        if(valorTotalOrcamentario <= valorDoProjeto){
            valido = true;
        }

        return valido;
    }

    /**
     * Verifica se a coluna de custeio do cronograma está preenchido com valores inferiores ao valor do projeto.
     *
     * @returns Boolean
     */
    function validarCronogramaFinanceiroInferiorValorProjetoCusteio(){
        var valido = false;
        var valorDoProjeto = textToFloat($('#picvalorcusteio').val());
        var valorTotalFinanceiro = buscarTotalCusteioCronogramaFinanceiro();

        if(valorTotalFinanceiro <= valorDoProjeto){
            valido = true;
        }

        return valido;
    }
    
    /**
     * Verifica se a coluna de capital do cronograma está preenchido com valores inferiores ao valor do projeto.
     *
     * @returns Boolean
     */
    function validarCronogramaFinanceiroInferiorValorProjetoCapital(){
        var valido = false;
        var valorDoProjeto = textToFloat($('#picvalorcapital').val());
        var valorTotalFinanceiro = buscarTotalCapitalCronogramaFinanceiro();

        if(valorTotalFinanceiro <= valorDoProjeto){
            valido = true;
        }

        return valido;
    }

    /**
     * Verifica se o cronograma está preenchido.
     *
     * @returns Boolean
     */
    function validarCronogramaOrcamentarioPreenchido(){
        var valido = false;
        var custeio = buscarTotalCusteioCronogramaOrcamentario();
        var capital = buscarTotalCapitalCronogramaOrcamentario();
        
        valorTotalOrcamentario = custeio + capital;

        if(valorTotalOrcamentario > 0){
            valido = true;
        }

        return valido;
    }

    /**
     * Verifica se o cronograma está preenchido.
     *
     * @returns Boolean
     */
    function validarCronogramaFinanceiroPreenchido(){
        var valido = false;
        var custeio = buscarTotalCusteioCronogramaFinanceiro();
        var capital = buscarTotalCapitalCronogramaFinanceiro();
        
        valorTotalFinanceiro = custeio + capital;

        if(valorTotalFinanceiro > 0){
            valido = true;
        }

        return valido;
    }

    /**
     * Busca o total informado de todos os meses no cronograma físico.
     *
     * @returns float
     */
    function buscarTotalCronogramaFisico(){
        var totalFisico = 0;
        $('input.input_fisico').each(function(data, key){
            var valorMesFisico = textToFloat($(this).val());
            totalFisico += valorMesFisico;
        });

        return totalFisico;
    }

    /**
     * Busca o total de custeio informado de todos os meses no cronograma orcamentario.
     *
     * @returns float
     */
    function buscarTotalCusteioCronogramaOrcamentario(){
        var totalOrcamentario = 0;
        $('input.input_orcamentario.custeio').each(function(data, key){
            var valorMesOrcamento = textToFloat($(this).val());
            totalOrcamentario += valorMesOrcamento;
        });

        return totalOrcamentario;
    }
    
    /**
     * Busca o total de capital informado de todos os meses no cronograma orcamentario.
     *
     * @returns float
     */
    function buscarTotalCapitalCronogramaOrcamentario(){
        var totalOrcamentario = 0;
        $('input.input_orcamentario.capital').each(function(data, key){
            var valorMesOrcamento = textToFloat($(this).val());
            totalOrcamentario += valorMesOrcamento;
        });

        return totalOrcamentario;
    }

    /**
     * Busca o total de custeio informado de todos os meses no cronograma financeiro.
     *
     * @returns float
     */
    function buscarTotalCusteioCronogramaFinanceiro(){
        var totalFinanceiro = 0;
        $('input.input_financeiro.custeio').each(function(data, key){
            var valorMesFinanceiro = textToFloat($(this).val());
            totalFinanceiro += valorMesFinanceiro;
        });

        return totalFinanceiro;
    }
    
    /**
     * Busca o total de capital informado de todos os meses no cronograma financeiro.
     *
     * @returns float
     */
    function buscarTotalCapitalCronogramaFinanceiro(){
        var totalFinanceiro = 0;
        $('input.input_financeiro.capital').each(function(data, key){
            var valorMesFinanceiro = textToFloat($(this).val());
            totalFinanceiro += valorMesFinanceiro;
        });

        return totalFinanceiro;
    }

    /**
     * Calcula e exibi na tela o valor disponivel na Sub-Unidade.
     *
     * @returns VOID
     */
    function atualizarValorLimiteDisponivelUnidade() {
        // Valor que veio da Base de Dados.
        var valorBaseLimiteDisponivelUnidade = textToFloat($('#VlrSUDisponivel').val());
        // Valor que veio da Base de Dados.
        var valorBaseProjeto = buscarValorBaseDoProjeto();
        // Valor atualizado em tempo de execução da tela durante o ato de cadastro.
        var valorProjeto = buscarValorDoProjeto();

//        if(valorProjeto < valorBaseProjeto){
//            valorDiferenca = (valorBaseProjeto - valorProjeto);
//        } else {
            valorDiferenca = (valorProjeto - valorBaseProjeto);
//        }

        var fltValorLimiteDisponivelUnidade = (valorBaseLimiteDisponivelUnidade - valorDiferenca);
        $('#td_disponivel_sub_unidade').html(number_format(fltValorLimiteDisponivelUnidade.toFixed(2), 2, ',', '.'));
    }

    /**
     * Calcula e exibi na tela o valor disponivel na funcional de custeio.
     *
     * @returns VOID
     */
    function atualizarValorLimiteDisponivelFuncionalCusteio() {
        // Valor que veio da Base de Dados.
        var valorBaseLimiteDisponivel = textToFloat($('#VlrFuncionalDisponivelCusteio').val());
        // Valor que veio da Base de Dados.
        var valorBaseLimiteProjetoCusteio = textToFloat($('#vlrPiCusteio').val());
        // Valor atualizado em tempo de execução da tela durante o ato de cadastro.
        var valorProjetoCusteio = textToFloat($('#picvalorcusteio').val());

        if(valorProjetoCusteio == 0){
            valorCusteio = valorBaseLimiteProjetoCusteio;
            var fltValorLimiteDisponivelFuncional = (valorBaseLimiteDisponivel + valorCusteio);
        } else if(valorProjetoCusteio < valorBaseLimiteProjetoCusteio){
            valorCusteio = (valorBaseLimiteProjetoCusteio - valorProjetoCusteio);
            var fltValorLimiteDisponivelFuncional = (valorBaseLimiteDisponivel - valorCusteio);
        } else {
            valorCusteio = (valorProjetoCusteio - valorBaseLimiteProjetoCusteio);
            var fltValorLimiteDisponivelFuncional = (valorBaseLimiteDisponivel - valorCusteio);
        }

        $('#td_disponivel_funcional_custeio').html(number_format(fltValorLimiteDisponivelFuncional.toFixed(2), 2, ',', '.'));
    }

    /**
     * Calcula e exibi na tela o valor disponivel na funcional de capital.
     *
     * @returns VOID
     */
    function atualizarValorLimiteDisponivelFuncionalCapital() {
        // Valor que veio da Base de Dados.
        var valorBaseLimiteDisponivel = textToFloat($('#VlrFuncionalDisponivelCapital').val());
        // Valor que veio da Base de Dados.
        var valorBaseLimiteProjetoCapital = textToFloat($('#vlrPiCapital').val());
        // Valor atualizado em tempo de execução da tela durante o ato de cadastro.
        var valorProjetoCapital = textToFloat($('#picvalorcapital').val());

        if(valorProjetoCapital == 0){
            valorCapital = valorBaseLimiteProjetoCapital;
            var fltValorLimiteDisponivelFuncional = (valorBaseLimiteDisponivel + valorCapital);
        } else if(valorProjetoCapital < valorBaseLimiteProjetoCapital){
            valorCapital = (valorBaseLimiteProjetoCapital - valorProjetoCapital);
            var fltValorLimiteDisponivelFuncional = (valorBaseLimiteDisponivel - valorCapital);
        } else {
            valorCapital = (valorProjetoCapital - valorBaseLimiteProjetoCapital);
            var fltValorLimiteDisponivelFuncional = (valorBaseLimiteDisponivel - valorCapital);
        }

        $('#td_disponivel_funcional_capital').html(number_format(fltValorLimiteDisponivelFuncional.toFixed(2), 2, ',', '.'));
    }
    
    /**
     * Calcula e exibi na tela o valor já cadastrado na funcional de custeio.
     *
     * @returns VOID
     */
    function atualizarValorJaCadastradoFuncionalCusteio() {
        // Valor que veio da Base de Dados.
        var valorBaseCadastradoFuncional = textToFloat($('#ptresCadastradoCusteio').val());
        // Valor que veio da Base de Dados.
        var valorBaseProjetoCusteio = textToFloat($('#vlrPiCusteio').val());
        // Valor atualizado em tempo de execução da tela durante o ato de cadastro.
        var valorProjetoCusteio = textToFloat($('#picvalorcusteio').val());

        if(valorProjetoCusteio === 0){
            valorCusteio = -valorBaseProjetoCusteio;
        }
        else if(valorProjetoCusteio < valorBaseProjetoCusteio){
            valorCusteio = (valorBaseProjetoCusteio - valorProjetoCusteio);
        } else {
            valorCusteio = (valorProjetoCusteio - valorBaseProjetoCusteio);
        }
        var fltValorLimiteDisponivelFuncional = (valorBaseCadastradoFuncional + valorCusteio);
        $('#td_ja_autorizado_funcional_custeio').html(number_format(fltValorLimiteDisponivelFuncional.toFixed(2), 2, ',', '.'));
    }
    
    /**
     * Calcula e exibi na tela o valor já cadastrado na funcional de capital.
     *
     * @returns VOID
     */
    function atualizarValorJaCadastradoFuncionalCapital() {
        // Valor que veio da Base de Dados.
        var valorBaseCadastradoFuncional = textToFloat($('#ptresCadastradoCapital').val());
        // Valor que veio da Base de Dados.
        var valorBaseProjetoCapital = textToFloat($('#vlrPiCapital').val());
        // Valor atualizado em tempo de execução da tela durante o ato de cadastro.
        var valorProjetoCapital = textToFloat($('#picvalorcapital').val());

        if(valorProjetoCapital === 0){
            valorCapital = -valorBaseProjetoCapital;
        } else if(valorProjetoCapital < valorBaseProjetoCapital){
            valorCapital = (valorBaseProjetoCapital - valorProjetoCapital);
        } else {
            valorCapital = (valorProjetoCapital - valorBaseProjetoCapital);
        }
        var fltValorLimiteDisponivelFuncional = (valorBaseCadastradoFuncional + valorCapital);
        $('#td_ja_autorizado_funcional_capital').html(number_format(fltValorLimiteDisponivelFuncional.toFixed(2), 2, ',', '.'));
    }

    /**
     * Calcula e exibi na tela o valor detalhado da funcional.
     *
     * @returns VOID
     */
    function atualizarValorDetalhado() {
        // Valor que veio da Base de Dados.
        var fltValorBaseDoProjeto = buscarValorBaseDoProjeto();
        // Valor atualizado em tempo de execução da tela durante o ato de cadastro.
        var fltValorDoProjeto = buscarValorDoProjeto();
        // Valor que veio da Base de Dados.
        var fltValorBaseDetalhado = textToFloat($('#piDetalhado').val());
        
        var fltValorDetalhadoAtual = (fltValorBaseDetalhado - fltValorBaseDoProjeto) + fltValorDoProjeto;
        $('#td_pi_detalhado').html(number_format(fltValorDetalhadoAtual.toFixed(2), 2, ',', '.'));
    }

    /**
     * Calcula e exibi na tela o valor não detalhado da funcional.
     *
     * @returns VOID
     */
    function atualizarValorNaoDetalhado() {
        // Valor que veio da Base de Dados.
        var fltValorBaseDoProjeto = buscarValorBaseDoProjeto();
        // Valor atualizado em tempo de execução da tela durante o ato de cadastro.
        var fltValorDoProjeto = buscarValorDoProjeto();
        // Valor que veio da Base de Dados.
        var fltValorBaseNaoDetalhado = textToFloat($('#piNaoDetalhado').val());
        
        var fltValorNaoDetalhadoAtual = (fltValorBaseNaoDetalhado - (fltValorDoProjeto - fltValorBaseDoProjeto));
        $('#td_pi_nao_detalhado').html(number_format(fltValorNaoDetalhadoAtual.toFixed(2), 2, ',', '.'));
    }

    /**
     * Calcula o valor da funcional.
     *
     * @returns VOID
     */
    function atualizarValorDoProjeto() {
        var valorCusteio = textToFloat($('#picvalorcusteio').val());
        var valorCapital = textToFloat($('#picvalorcapital').val());
        var total = valorCusteio + valorCapital;
        $('#td_valor_projeto').html(number_format(total.toFixed(2), 2, ',', '.'));
    }

    /**
     * Exibe o valor total do cronograma Físico na tela.
     *
     * @returns VOID
     */
    function atualizarTotalFisico() {
        var total = buscarTotalCronogramaFisico();
        $('#td_total_fisico').html( total.toFixed(0) );
    }

    /**
     * Exibe o valor total do cronograma Orçamentário na tela.
     *
     * @returns VOID
     */
    function atualizarTotalOrcamentario() {
        var custeio = buscarTotalCusteioCronogramaOrcamentario();
        var capital = buscarTotalCapitalCronogramaOrcamentario();
        $('#td_total_orcamentario_custeio').html('R$ '+ number_format(custeio.toFixed(2), 2, ',', '.'));
        $('#td_total_orcamentario_capital').html('R$ '+ number_format(capital.toFixed(2), 2, ',', '.'));
    }

    /**
     * Exibe o valor total do cronograma Financeiro na tela.
     *
     * @returns VOID
     */
    function atualizarTotalFinanceiro() {
        var custeio = buscarTotalCusteioCronogramaFinanceiro();
        var capital = buscarTotalCapitalCronogramaFinanceiro();
        $('#td_total_financeiro_custeio').html('R$ '+ number_format(custeio.toFixed(2), 2, ',', '.'));
        $('#td_total_financeiro_capital').html('R$ '+ number_format(capital.toFixed(2), 2, ',', '.'));
    }

    /**
     * Soma o valor de capital e custeio e retorna o valor total do projeto.
     *
     * @returns float Valor do projeto
     */
    function buscarValorDoProjeto(){
        var valorCusteio = textToFloat($('#picvalorcusteio').val());
        var valorCapital = textToFloat($('#picvalorcapital').val());
        var valorDoProjeto = valorCusteio + valorCapital;

        return valorDoProjeto;
    }
    
    /**
     * Busca o valor de Custeio do projeto.
     *
     * @returns float Valor do projeto
     */
    function buscarValorDoProjetoCusteio(){
        var valor = textToFloat($('#picvalorcusteio').val());
        return valor;
    }
    
    /**
     * Busca o valor de capital do projeto.
     *
     * @returns float Valor do projeto
     */
    function buscarValorDoProjetoCapital(){
        var valor = textToFloat($('#picvalorcapital').val());
        return valor;
    }
    
    /**
     * Busca o valor disponivel na Sub-Unidade.
     *
     * @returns float Valor do projeto
     */
    function buscarValorDisponivelSubUnidade(){
        var valor = textToFloat($('#td_disponivel_sub_unidade').text());
        return valor;
    }
    
    /**
     * Busca o valor disponivel na funcional para custeio.
     *
     * @returns float Valor do projeto
     */
    function buscarValorDisponivelFuncionalCusteio(){
        var valor = textToFloat($('#td_disponivel_funcional_custeio').text());
        return valor;
    }
    
    /**
     * Busca o valor disponivel na funcional para Capital.
     *
     * @returns float Valor do projeto
     */
    function buscarValorDisponivelFuncionalCapital(){
        var valor = textToFloat($('#td_disponivel_funcional_capital').text());
        return valor;
    }
    
    /**
     * Valida se o valor do Projeto declarado pelo usuário é maior do que o valor
     * autorizado da funcional para Custeio.
     * 
     * @returns boolean
     */
    function validarValorProjetoMenorAutorizadoFuncionalCusteio(){
        if(buscarValorDoProjetoCusteio() <= buscarValorAutorizadoFuncionalCusteio()){
            resultado = true;
        } else {
            resultado = false;
        }
        
        return resultado;
    }
    
    /**
     * Valida se o valor do Projeto declarado pelo usuário é maior do que o valor
     * autorizado da funcional para Capital.
     * 
     * @returns boolean
     */
    function validarValorProjetoMenorAutorizadoFuncionalCapital(){
        if(buscarValorDoProjetoCapital() <= buscarValorAutorizadoFuncionalCapital()){
            resultado = true;
        } else {
            resultado = false;
        }
        
        return resultado;
    }

    /**
     * Soma o valor de capital e custeio e retorna o valor base total do projeto.
     *
     * @returns float Valor do projeto
     */
    function buscarValorBaseDoProjeto(){
        var valorCusteio = textToFloat($('#vlrPiCusteio').val());
        var valorCapital = textToFloat($('#vlrPiCapital').val());
        var valorBaseDoProjeto = valorCusteio + valorCapital;

        return valorBaseDoProjeto;
    }

    /**
     * Soma o valor de capital e custeio e retorna o valor total disponivel.
     * @todo considerar valor inicial base pra dimininuir antes de somar ao valor disponivel custeio e capita.
     * @returns float Valor
     */
    function buscarValorDisponivelFuncional(){
        var valorCusteio = textToFloat($('#td_disponivel_funcional_custeio').text());
        var valorCapital = textToFloat($('#td_disponivel_funcional_capital').text());
        var disponivelFuncional = valorCusteio + valorCapital;

        return disponivelFuncional;
    }

    /**
     * Soma o valor de capital e custeio e retorna o valor total disponivel.
     *
     * @returns float Valor
     */
    function buscarValorAutorizadoFuncional(){
        var valorCusteio = textToFloat($('#td_autorizado_funcional_custeio').text());
        var valorCapital = textToFloat($('#td_autorizado_funcional_capital').text());
        var autorizadoFuncional = valorCusteio + valorCapital;

        return autorizadoFuncional;
    }

    /**
     * Busca o valor de custeio autorizado na funcional no formato de numero 
     * flutuante sem mascara.
     * 
     * @returns {Number}
     */
    function buscarValorAutorizadoFuncionalCusteio(){
        var valor = textToFloat($('#td_autorizado_funcional_custeio').text());
        return valor;
    }
    
    /**
     * Busca o valor de custeio autorizado na funcional no formato de numero 
     * flutuante sem mascara.
     * 
     * @returns {Number}
     */
    function buscarValorAutorizadoFuncionalCapital(){
        var valor = textToFloat($('#td_autorizado_funcional_capital').text());
        return valor;
    }

    /**
     * 
     * @todo refatorar valores mocados
     */
    function atualizarSaldoFuncional(){
        /**
         * implements in popupptres o import desses dados ao selecionar a funcional.
         */
        $('#td_autorizado_funcional_custeio').text($('#ptresCusteio').val());
        $('#td_autorizado_funcional_capital').text($('#ptresCapital').val());

        $('#td_disponivel_funcional_custeio').text($('#piNaoDetalhadoCusteio').val());
        $('#td_disponivel_funcional_capital').text($('#piNaoDetalhadoCapital').val());
        
        // Caso de FNC o sistema atualiza os valores de já cadastrado na funcional.
        $('#td_ja_autorizado_funcional_custeio').text($('#ptresCadastradoCusteio').val());
        $('#td_ja_autorizado_funcional_capital').text($('#ptresCadastradoCapital').val());
    }
    
    /**
     * 
     * @todo refatorar valores mocados
     */
    function carregarSaldoFuncional(){
        $('#td_autorizado_funcional_custeio').text($('#ptresCusteio').val());
        $('#td_autorizado_funcional_capital').text($('#ptresCapital').val());

        $('#td_disponivel_funcional_custeio').text($('#piNaoDetalhadoCusteio').val());
        $('#td_disponivel_funcional_capital').text($('#piNaoDetalhadoCapital').val());

        $('#VlrFuncionalDisponivelCapital').val($('#piNaoDetalhadoCapital').val());
        $('#VlrFuncionalDisponivelCusteio').val($('#piNaoDetalhadoCusteio').val());
        
        // Caso de FNC o sistema atualiza os valores de já cadastrado na funcional.
        $('#td_ja_autorizado_funcional_custeio').text($('#ptresCadastradoCusteio').val());
        $('#td_ja_autorizado_funcional_capital').text($('#ptresCadastradoCapital').val());
    }

    /**
     * Muda a cor do valor de projeto capital e custeio quando o valor disponvel
     * da sub-unidade ou funcional for ultrapassado.
     *
     * @returns VOID
     */
    function mudarCorValorProjeto(){
        var disponivelUnidade = textToFloat($('#td_disponivel_sub_unidade').text());
        var disponivelFuncionalCusteio = textToFloat($('#td_disponivel_funcional_custeio').text());
        var disponivelFuncionalCapital = textToFloat($('#td_disponivel_funcional_capital').text());
        
        if(verificarFormularioEmenda()){
            if(disponivelFuncionalCusteio < 0){
                $('#picvalorcusteio').addClass('validateRedText');
            } else {
                $('#picvalorcusteio').removeClass('validateRedText');
            }

            if(disponivelFuncionalCapital < 0){
                $('#picvalorcapital').addClass('validateRedText');
            } else {
                $('#picvalorcapital').removeClass('validateRedText');
            }
        } else {
            if(disponivelUnidade < 0 || disponivelFuncionalCusteio < 0){
                $('#picvalorcusteio').addClass('validateRedText');
            } else {
                $('#picvalorcusteio').removeClass('validateRedText');
            }

            if(disponivelUnidade < 0 || disponivelFuncionalCapital < 0){
                $('#picvalorcapital').addClass('validateRedText');
            } else {
                $('#picvalorcapital').removeClass('validateRedText');
            }
        }
    }

    /**
     * Controla as opções do formulario conforme a opção de tipo de localização selecionada.
     *
     * @param int esfid Código da esfera preenchido pelo usuário
     * @return VOID
     */
    function controlarTipoLocalizacao(esfid){
        $('#btn_selecionar_localizacao').hide();
        $('#btn_selecionar_localizacao_estadual').hide();
        $('#btn_selecionar_localizacao_exterior').hide();
        $('#table_localizacao tr').hide();
        $('#table_localizacao_estadual tr').hide();
        $('#table_localizacao_exterior tr').hide();

        switch (esfid) {
            // Verifica se a esfera é Estadual/DF.
            case intEsfidEstadualDF:
                $('#table_localizacao_estadual tr').show('slow');
                $('#btn_selecionar_localizacao_estadual').show('slow');

                $('#table_localizacao tr').not('tr.tr_head').remove();
                $('#table_localizacao_exterior tr').not('tr.tr_head').remove();
            break;
            // Verifica se a esfera é Exterior.
            case intEsfidExterior:
                $('#table_localizacao_exterior tr').show('slow');
                $('#btn_selecionar_localizacao_exterior').show('slow');
                $('#table_localizacao tr').not('tr.tr_head').remove();
                $('#table_localizacao_estadual tr').not('tr.tr_head').remove();
            break;
            // Verifica se a esfera é Federal.
            case intEsfidFederalBrasil:
                $('#btn_selecionar_localizacao').hide('slow');
                $('#table_localizacao tr').hide('slow');
                $('#table_localizacao tr').not('tr.tr_head').remove();
                $('#table_localizacao_estadual tr').not('tr.tr_head').remove();
                $('#table_localizacao_exterior tr').not('tr.tr_head').remove();
            break;
            // Verifica se a esfera é Municipal.
            case intEsfidMunicipal:
                $('#table_localizacao tr').show('slow');
                $('#btn_selecionar_localizacao').show('slow');
                $('#table_localizacao_estadual tr').not('tr.tr_head').remove();
                $('#table_localizacao_exterior tr').not('tr.tr_head').remove();
            break;
        }
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
     * Verifica se o formulário é Não Orçamentário.
     * 
     * @returns boolean retorna true se o formulário for Não Orçamentário.
     */
    function verificarFormularioNaoOrcamentario(){
        var resultado = false;
        if($('#eqdid').val() == intEnqNaoOrcamentario){
            resultado = true;
        }

        return resultado;
    }
    
    /**
     * Verifica se o formulário é de Emenda.
     * 
     * @returns boolean retorna true se o formulário for Não Orçamentário.
     */
    function verificarFormularioEmenda(){
        var resultado = false;
        if($('#eqdid').val() == intEnqEmenda){
            resultado = true;
        }
        
        return resultado;
    }

    function abrirModalResponsaveis() {
        if($('#modal_responsaveis .modal-body div').size() <= 1){
            $.post('planacomorc.php?modulo=principal/unidade/pi-responsaveis&acao=A&ungcod='+ $("#ungcod").val(),
                function(response) {
                    $('#modal_responsaveis .modal-body').html(response);
                    $('#modal_responsaveis').modal();
                    $('#modal_responsaveis .chosen-select').chosen();
                    $('#modal_responsaveis .chosen-container').css('width', '100%');
            });
        } else {
            $('#formulario_responsaveis input[name=ungcod]').val($("#ungcod").val());
            $('#modal_responsaveis').modal();
            $('#btnPopupResponsaveisPesquisar').click();
        }
    }

    function abrirModalLocalizacao() {
        // Verifica se o modal terá que carregar a tela.
        if($('#modal_localizacao .modal-body div').size() <= 1){
            $.post('planacomorc.php?modulo=principal/unidade/pi-localizacao&acao=A', function(response) {
                $('#modal_localizacao .modal-body').html(response);
                $('#modal_localizacao').modal();
                $('#modal_localizacao .chosen-select').chosen();
                $('#modal_localizacao .chosen-container').css('width', '100%');
            });
        } else {
            $('#modal_localizacao').modal();
            $('#btnPopupLocalizacaoPesquisar').click();
        }
    }

    function abrirModalLocalizacaoEstadual() {
        // Verifica se o modal terá que carregar a tela.
        if($('#modal_localizacao_estadual .modal-body div').size() <= 1){
            $.post('planacomorc.php?modulo=principal/unidade/pi-localizacao-estadual&acao=A', function(response) {
                $('#modal_localizacao_estadual .modal-body').html(response);
                $('#modal_localizacao_estadual').modal();
                $('#modal_localizacao_estadual .chosen-select').chosen();
                $('#modal_localizacao_estadual .chosen-container').css('width', '100%');
            });
        } else {
            $('#modal_localizacao_estadual').modal();
            $('#btnPopupLocalizacaoEstadualPesquisar').click();
        }
    }

    function abrirModalLocalizacaoExterior() {
        // Verifica se o modal terá que carregar a tela.
        if($('#modal_localizacao_exterior .modal-body div').size() <= 1){
            $.post('planacomorc.php?modulo=principal/unidade/pi-localizacao-exterior&acao=A', function(response) {
                $('#modal_localizacao_exterior .modal-body').html(response);
                $('#modal_localizacao_exterior').modal();
                $('#modal_localizacao_exterior .chosen-select').chosen();
                $('#modal_localizacao_exterior .chosen-container').css('width', '100%');
            });
        } else {
            $('#modal_localizacao_exterior').modal();
            $('#btnPopupLocalizacaoExteriorPesquisar').click();
        }
    }

    function abrirModalUpload() {
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
    * Controla ações para quando o botão edital estiver marcado ou desmarcado.
    */
    function controlarEdital(opcao){
        if(opcao === true){
            $('#div_edital').show('slow');
        } else {
            $('#div_edital').hide('slow');
        }
    }


    /**
     * Controla a exibição do formulario se o enquadramento for não orçamentário.
     *
     * @param integer codigo Código selecionado pelo usuário.
     * @returns VOID
     */
    function mudarFormularioNaoOrcamentario(codigo){
        // Se o código for Não Orçamentário, o sistema não exibe as opções PTRES(Funcional), Valor do Projeto, Cronograma Orçamentário e Financeiro.
        if(verificarFormularioNaoOrcamentario()){

            // Oculta o botão de Criar Plano de Ação por Proposta.
            $('.div-proposta-pa').hide('slow');
            // Oculta a aba de Alterações Orçamentárias.
            $('.aba-alteracoes').hide('slow');
            // Oculta a opções PTRES(Funcional).
            $('.div_ptres').hide('slow');
            // Exibe as Etapas
            $('.div_etapas_pi').show('slow');
            // Oculta o quadro de Custeio e Capital com a opção de Valor do Projeto.
            $('.div_custeio_capital').hide('slow');
            // Oculta as colunas e campos do Cronograma Orçamentário.
            $('.td_cronograma_orcamentario').hide('slow');
            // Oculta as colunas e campos do Cronograma Financeiro.
            $('.td_cronograma_financeiro').hide('slow');
            // Oculta os campos de Meta PNC e Indicador PNC.
            $('.grupo_pnc').hide('slow');
            // Oculta o campo de Area Cultural.
            $('#div_area_cultural').hide('slow');
            // Oculta o campo de Segmento Cultural.
            $('#div_segmento_cultural').hide('slow');
            // Oculta o campo de TED.
            $('#div_ted').hide('slow');
            // Oculta o campo de Números SNIIC.
            $('#div_sniic').hide('slow');
            // Oculta o campo de Número Pronac.
            $('#div_pronac').hide('slow');
            // Oculta a combo de Modalidade de Pactuação.
            $('#div_pactuacao').hide('slow');
            // Exibe o campo Recursos Necessários
            $(".recursos_necessarios").show('slow'); 
        } else {
            // Exibe a opções PTRES(Funcional).
            $('.div_ptres').show('slow');
            // Exibe o quadro de Custeio e Capital com a opção de Valor do Projeto.
            $('.div_custeio_capital').show('slow');
            // Exibe as colunas e campos do Cronograma Orçamentário.
            $('.td_cronograma_orcamentario').show('slow');
            // Exibe as colunas e campos do Cronograma Financeiro.
            $('.td_cronograma_financeiro').show('slow');
            // Oculta as Etapas
            $('.div_etapas_pi').hide('slow');
            // Exibe o campo de Area Cultural.
            $('#div_area_cultural').show('slow');
            // Exibe o campo de Segmento Cultural.
            $('#div_segmento_cultural').show('slow');
            // Exibe o campo de TED.
            $('#div_ted').show('slow');            
            // Exibe a combo de Modalidade de Pactuação.
            $('#div_pactuacao').show('slow');
            // Oculta o campo Recursos Necessários
            $(".recursos_necessarios").hide('slow');
            // Oculta o botão de Criar Plano de Ação por Proposta.
            $('.div-proposta-pa').show('slow');
            // Oculta a aba de Alterações Orçamentárias.
            $('.aba-alteracoes').show('slow');
        }
    }
    
    /**
     * Controla a exibição do formulario se o enquadramento for Emenda.
     *
     * @param integer codigo Código selecionado pelo usuário.
     * @returns VOID
     */
    function mudarFormularioEmenda(codigo){
        if(fnc === false){
            /*
             * Se o código for Emenda, o sistema não exibe as opções Responsáveis pelo Projeto,
             * edital e Cronogramas.
             */
            if(verificarFormularioEmenda()){
                // Oculta a opções Responsáveis pelo Projeto.
                $('#div_responsaveis').hide('slow');
                // Oculta a opção Edital.
                $('#div_botao_edital').hide('slow');
                // Oculta as colunas e campos do Cronograma.
                $('#div_cronogramas').hide('slow');
                // Ocultando Limites da unidade
                $('.tr_limite_autorizado_unidade').hide('slow');
                $('.tr_limite_disponivel_unidade').hide('slow');
            } else {
                // Exibe a opções Responsáveis pelo Projeto.
                $('#div_responsaveis').show('slow');
                // Exibe a opção Edital.
                $('#div_botao_edital').show('slow');
                // Exibe as colunas e campos do Cronograma.
                $('#div_cronogramas').show('slow');
                // Exibindo Limites da unidade
                $('.tr_limite_autorizado_unidade').show('slow');
                $('.tr_limite_disponivel_unidade').show('slow');
            }
        }
    }

    /**
     * Carrega novo conteúdo para a opções de Sub-Unidade via requisição ajax.
     */
    function carregarUG(unicod) {
        $.post(urlPagina+ '&carregarComboUG=ok&unicod='+ unicod+ '&fnc='+ (fnc? 'TRUE': 'FALSE'), function(response) {
            $('#ungcod').remove('slow');
            $('.div_ungcod').html(response);
            $(".chosen-select").chosen();
        });
    }

    /**
     * Carrega novo conteúdo para o select de Metas Estratégicas via requisição ajax.
     */
    function carregarMetasEstrategicas(obeid, meeid, suocod) {

        $.post(urlPagina + '&carregarMetasEstrategicas=ok&obeid=' + obeid + '&suocod=' +suocod,
            function (response) {
                $('#meeid').remove();
                $('.div_meeid').html(response);
                $('#meeid').val(meeid);
                $(".chosen-select").chosen();
            });
    }

    /**
     * Carrega novo conteúdo para o select de Diretriz Estratégica via requisição ajax.
     */
    function carregarDiretrizEstrategica(obeid, dieid) {

        $.post(urlPagina + '&carregarDiretrizEstrategicas=ok&obeid=' + obeid,
            function (response) {
                $('#dieid').remove();
                $('.div_dieid').html(response);
                $('#dieid').val(dieid);
                $(".chosen-select").chosen();
            });
    }

    /**
     * Carrega novo conteúdo para o select de Metas PPA via requisição ajax.
     */
    function carregarMetasPPA(oppid, mppid, suocod, delegacao, mppid) {
        if (delegacao!==null){
            delegacao.push(suocod);
        }else{
            delegacao = suocod;
        }
        $.post(urlPagina+ '&carregarMetasPPA=ok&oppid=' + oppid + '&suocod=' + delegacao, 
        function(response) {
            $('#mppid').remove();
            $('.div_mppid').html(response);
            $('#mppid').val(mppid);            
            $(".chosen-select").chosen();
        });
    }

    /**
     * Carrega limites da Sub-Unidade via requisição ajax.
     */
    function carregarLimitesUnidade(codigo) {
        if(!verificarFormularioEmenda()){
            $.ajax({
                url: urlPagina+ '&carregarLimitesUnidade=ok',
                type: "post",
                data: {'ungcod': codigo},
                dataType: 'json',
                success: function(data){
                    $('#td_autorizado_sub_unidade').text(data.autorizado);
                    $('#VlrSUDisponivel').val(data.disponivel);

                    atualizarValorLimiteDisponivelUnidade();
                    mudarCorValorProjeto();
                }
            });
        }
    }

    /**
     * Carrega as opções de Meta Unidade de acordo com as metas vinculadas a unidade ou que não tenham vinculo algum.
     * @param ungcod
     */
    function carregarMetaUnidade(ungcod) {

        if (ungcod !== null && ungcod !== '') {
            $.post(urlPagina+ '&carregarComboUnidade=ok&ungcod='+ ungcod, function(response) {
                $('#meuid').remove();
                $('.div_meeid').html(response);
                $('.chosen-select').chosen();
            });
        }
    }

    /**
     * Carrega novo conteúdo para o select de metas PNC via requisição ajax.
     */
    function carregarMetaPNC(codigo) {
        $.post(urlPagina+ '&carregarMetaPNC=ok&suocod=' + codigo, function(response) {
            $('#mpnid').remove();
            $('.div_mpnid').html(response);
            $(".chosen-select").chosen();
        });
    }

    /**
     * Carrega novo conteúdo para o select de Indicadores PNC via requisição ajax.
     */
    function carregarIndicadorPNC(codigo) {
        $.post(urlPagina+ '&carregarIndicadorPNC=ok&mpnid=' + codigo, function(response) {
            $('#ipnid').remove();
            $('.div_ipnid').html(response);
            $(".chosen-select").chosen();
        });
    }

    /**
     * Carrega novo conteúdo para o select de Segmento Cultural via requisição ajax.
     */
    function carregarSegmentoCultural(codigo) {
        $.post(urlPagina+ '&carregarSegmentoCultural=ok&mdeid=' + codigo, function(response) {
            $('#neeid').remove();
            $('.div_neeid').html(response);
            $(".chosen-select").chosen();
        });
    }

    /**
     * Carrega novo conteúdo para o select de Manutenção Item via requisição ajax.
     */
    function carregarManutencaoItem(codigo) {
        $.post(urlPagina+ '&carregarManutencaoItem=ok&eqdid=' + codigo + '&maiid=' + intMaiid, function(response) {
            $('#maiid').remove();
            $('#masid option').not('option[value=""]').remove();
            $('#masid').val('').trigger("chosen:updated");
            $('.div_maiid').html(response);
            formatarTelaEnquadramentoComManutencaoItem();
        });
    }
    
    /**
     * Se existir itens de manutenção exibe as opções de manutenção item e sub-item e bloqueia os campos de titulo e descrição.
     * 
     * @returns VOID
     */
    function exibirOpcoesItemManutencao(){
        var manutencaoItem = $('#maiid :checked').text();
//        var descricao = $('#masid :checked').text();
        
        if($('#maiid option').not('option[value=""]').size() > 0){
            $('.grupo_manutencao').show('slow');
            // Desabilita a opção de modificar os campos de Título e Descrição.
            if($('#pliid').val() == ""){
                $('[name=plititulo]').val('');
                $('[name=plidsc]').val('');
            }
            $('[name=plititulo]').attr('readonly', 'readonly');
            $('[name=plidsc]').attr('readonly', 'readonly');
        } else {
            $('.grupo_manutencao').hide('slow');
            // Habilita a opção de modificar os campos de Título e Descrição.
            $('[name=plititulo]').removeAttr('readonly');
            $('[name=plidsc]').removeAttr('readonly');
        }
    }
    
    function formatarTelaEnquadramentoComManutencaoItem(){
        // Verifica se o usuário já estiver preenchido o enquadramento(Caso de formulário de cadastro de novo PI).
        if($('#eqdid').val() != ""){
            
            // Se o for formulário é reduzido.
            if(verificarFormularioReduzido()){

                // Se existir itens de manutenção exibe as opções de manutenção item e sub-item e bloqueia os campos de titulo e descrição.
                exibirOpcoesItemManutencao();
                
                // Oculta os campos de metas PPA e PNC
                $('.div_metas_ppa_pnc').hide('slow');
                $('#div_area_cultural').hide('slow');
                $('#div_segmento_cultural').hide('slow');
                $('#div_botao_edital').hide('slow');
                // Marca a opção produto com o valor padrão "Não se aplica" se não tiver valor marcado ainda.
                if($('#pprid').val() === ""){
                    $('#pprid').val(intProdNaoAplica).trigger("chosen:updated");
                    formatarTelaProdutoNaoAplica(intProdNaoAplica);
                }
                
                $('#mdeid').val('').trigger("chosen:updated");
                $('#neeid').val('').trigger("chosen:updated");
                $('#oppid').val('').trigger("chosen:updated");
                $('#mppid').val('').trigger("chosen:updated");
                $('#ippid').val('').trigger("chosen:updated");
                $('#mpnid').val('').trigger("chosen:updated");
                $('#ipnid').val('').trigger("chosen:updated");
            } else {
                // Se existir itens de manutenção exibe as opções de manutenção item e sub-item e bloqueia os campos de titulo e descrição.
                exibirOpcoesItemManutencao();
                
                // Exibe os campos de metas PPA e PNC
                $('.div_metas_ppa_pnc').show('slow');
                $('#div_botao_edital').show('slow');

                // Esconde as opções de Area Cultural e de Segmento quando o item é Não Orçamentária
                if(!verificarFormularioNaoOrcamentario()) {
                    $('#div_area_cultural').show('slow');
                    $('#div_segmento_cultural').show('slow');
                } else if (verificarFormularioNaoOrcamentario()) {
                    $('.div_metas_ppa_pnc').hide('fast');
                }
            }
        }
    }

    /**
     * Carrega novo conteúdo para o select de Manutenção SubItem via requisição ajax.
     */
    function carregarManutencaoSubItem(codigo) {
        $.post(urlPagina+ '&carregarManutencaoSubItem=ok&maiid=' + codigo, function(response) {
            $('#masid').remove();
            $('.div_masid').html(response);
            $(".chosen-select").chosen();
        });
    }

    /**
     * Carrega novo conteúdo para o select de Metas PPA via requisição ajax.
     */
    function carregarIniciativaPPA(codigo, suoid) {
        $.post(urlPagina+ '&carregarIniciativaPPA=ok&oppid=' + codigo + '&suoid=' + suoid, function(response) {
            $('#ippid').remove();
            $('.div_ippid').html(response);
            $(".chosen-select").chosen();
        });
    }

    function Trim(str)
    {
        return str.replace(/^\s+|\s+$/g, "");
    }

    //coloca tabindex no campo valor
    function tabindexcampo() {
        var x = document.getElementsByTagName("input");
        var y = 1;
        for (i = 0; i < x.length; i++) {
            if (x[i].type == "text") {
                if (x[i].name.substr(0, 8) == 'plivalor') {
                    x[i].tabIndex = y;
                    y++;
                }
            }
        }
    }

    /**
     * Verifica se o valor do cronograma Físico é igual ao informado no Produto do PI.
     * 
     * @returns {Boolean}
     */
    function validarCronogramaFisicoIgualQuantidade(){
        var resultado = false;
        var qtdCronograma = buscarTotalCronogramaFisico();
        var qtdProdutoPi = textToFloat($('#picquantidade').val());
        
        if(qtdCronograma == qtdProdutoPi){
            resultado = true;
        }
        
        return resultado;
    }
    
    /**
     * Verifica se o valor do cronograma Orcamentario é igual ao informado no valor do projeto.
     * 
     * @returns {Boolean}
     */
    function validarCronogramaOrcamentarioCusteioIgualValorProjeto(){
        var resultado = false;
        var vlrCronograma = buscarTotalCusteioCronogramaOrcamentario();
        var vlorProjeto = textToFloat($('#picvalorcusteio').val());

        if(number_format(vlrCronograma, 2, '.', '') == number_format(vlorProjeto, 2, '.', '')){
            resultado = true;
        }
        
        return resultado;
    }
    
    /**
     * Verifica se o valor do cronograma Orcamentario é igual ao informado no valor do projeto.
     * 
     * @returns {Boolean}
     */
    function validarCronogramaOrcamentarioCapitalIgualValorProjeto(){
        var resultado = false;
        var vlrCronograma = buscarTotalCapitalCronogramaOrcamentario();
        var vlorProjeto = textToFloat($('#picvalorcapital').val());

        if(number_format(vlrCronograma, 2, '.', '') == number_format(vlorProjeto, 2, '.', '')){
            resultado = true;
        }
        
        return resultado;
    }

    /**
     * Verifica se o valor do cronograma Financeiro é igual ao informado no valor do projeto.
     * 
     * @returns {Boolean}
     */
    function validarCronogramaFinanceiroCusteioIgualValorProjeto(){
        var resultado = false;
        var vlrCronograma = buscarTotalCusteioCronogramaFinanceiro();
        var vlorProjeto = textToFloat($('#picvalorcusteio').val());

        if(number_format(vlrCronograma, 2, '.', '') == number_format(vlorProjeto, 2, '.', '')){
            resultado = true;
        }
        
        return resultado;
    }

    /**
     * Verifica se o valor do cronograma Financeiro é igual ao informado no valor do projeto.
     * 
     * @returns {Boolean}
     */
    function validarCronogramaFinanceiroCapitalIgualValorProjeto(){
        var resultado = false;
        var vlrCronograma = buscarTotalCapitalCronogramaFinanceiro()
        var vlorProjeto = textToFloat($('#picvalorcapital').val());

        if(number_format(vlrCronograma, 2, '.', '') == number_format(vlorProjeto, 2, '.', '')){
            resultado = true;
        }
        
        return resultado;
    }

    /* Função para subustituir todos */
    function replaceAll(str, de, para) {
        if(typeof str != 'undefined'){
            var pos = str.indexOf(de);
            while (pos > -1) {
                str = str.replace(de, para);
                pos = str.indexOf(de);
            }
        } else {
            str = '';
        }
        return (str);
    }

    function mostrarPopupPtres() {
        var urlPopup = 'planacomorc.php?modulo=principal/unidade/popupptres&acao=A';
        if(fnc === true){
            urlPopup += '&fnc=1';
        }
        
        if($("#ungcod").val() == ""){
            alert('<p style="text-align: justify;">&nbsp; &nbsp; Não será possível selecionar uma funcional sem informar a Unidade.<br />&nbsp; &nbsp; <span style="color: #ff0000;">Por favor, selecione uma unidade e tente novamente.</span></p>');
        } else if($('#eqdid').val() == ""){
            alert('<p style="text-align: justify;">&nbsp; &nbsp; Não será possível selecionar uma funcional sem informar o Enquadramento da Despesa.<br />&nbsp; &nbsp; <span style="color: #ff0000;">Por favor, selecione um Enquadramento da Despesa e tente novamente.</span></p>');
        } else {
            // Verifica se o modal terá que recarregar a tela.
            if($('#modal_ptres .modal-body div').size() <= 1){
                $.post(urlPopup+ '&obrigatorio=n&unicod='+ $("#unicod").val()+ '&ungcod='+ $("#ungcod").val()+ '&no_ptrid='+ $('input[name=ptrid]').val()+ '&eqdid='+ $('#eqdid').val(), function(response) {
                    $('#modal_ptres .modal-body').html(response);
                    $('#modal_ptres').modal();
                    $('#modal_ptres .chosen-select').chosen();
                    $('#modal_ptres .chosen-container').css('width', '100%');
                });
            } else {
                $('#formularioPopup input[name=unicod]').val($("#unicod").val());
                $('#formularioPopup input[name=ungcod]').val($("#ungcod").val());
                $('#formularioPopup input[name=eqdid]').val($('#eqdid').val());
                $('#formularioPopup input[name=no_ptrid]').val($('input[name=ptrid]').val());
                $('#modal_ptres').modal();
                $('#btnPopupPtresPesquisar').click();
            }
        }
    }

    function visualizarRegistro(ptrid) {
        url = 'planacomorc.php?modulo=principal/unidade/popupptres&acao=A&detalhe=ok&ptrid=' + ptrid;
        $.post(url, function(html) {
            $('#modal_detalhes_ptres .modal-body').html(html);
            $('#modal_detalhes_ptres').modal();
        });
    }

    function toggleDelegacao(){
        if($('#radioDelegacao').is(':checked')){
            $('#div_unidades_delegadas').show('slow');
        } else {
            $('#div_unidades_delegadas').hide('slow');
            $("#delegacao").val('').trigger('chosen:updated');
        }
    }

    /**
     * Caso o formulário seja de edição e exista objetivo cadastrado o sistema 
     * impede que o usuário mude o objetivo.
     * 
     * @returns VOID
     */
    function initTravarObjetivo(){
        // Verifica se o formulário é de edição de PI.
        if($('#pliid').val() !== ""){
            // Verifica se existe objetivo salvo.
            if($('#oppid').val() !== ""){
                $('#oppid').prop('disabled', true).trigger("chosen:updated");
            }
        }
    }
