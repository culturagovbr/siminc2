
    /**
     * Ações efetuadas quando a tela de lista de PI é iniciada.
     * 
     * @returns VOID
     */
    function initListaPiManter(){
        
        $('[data-toggle="popover"]').popover({placement:'top', trigger:'hover'});
        
        $('.abrir-obra').click(function(){
            abrirObra($(this).attr('data-obrid'));
        });
        
        $('.abrir-ted').click(function(){
            abrirTED($(this).attr('data-tcpid'));
        });
        
        $('.btn-limpar').click(function(){
            $('#requisicao').val('limpar');
            $('#filtropi').submit();
        });
        
        $('#btn-exportar-xls').click(function(){
            $('#requisicao').val('listapimanter-xls');
            $('#filtropi').submit();
            $('#requisicao').val('');
        });
        
        $('.btn-novo').click(function(){
            window.document.location.href = 'planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A';
        });
        
        $('.btn-novo-fnc').click(function(){
            window.document.location.href = 'planacomorc.php?modulo=principal/unidade/cadastro_pi_fnc&acao=A';
        });
        
        $('.a_listar_delegadas').click(function(){
            var pliid = $(this).attr('data-pi');
            abrirPopupDelegadas(pliid);
            return false;
        });
        
        $('.a_espelho').click(function(){
            var pliid = $(this).attr('data-pi');
            exibirEspelhoPi(pliid);
            return false;
        });
        
        // Evento ao mudar opção de UO
        $('#unicod').change(function(){
            carregarUG($(this).val());
        });

    }
    
    /**
     * Carrega novo conteúdo para a opções de Sub-Unidade via requisição ajax.
     * 
     */
    function carregarUG(unicod) {
        $.post('?modulo=principal/unidade/listapimanter&acao=A&requisicao=carregarComboUG', {unicod: $('#unicod').val()}, function(response) {
            $('#div_ungcod').remove('slow');
            $('#div_ungcod').html(response);
            $(".chosen-select").chosen();
        });
    }
    
    /**
     * Abre popup com lista de Sub-Unidades delegadas.
     * 
     * @param integer pliid
     * @returns VOID 
     */
    function abrirPopupDelegadas(pliid) {
       $('#detalhePiDelegadas .modal-body').empty();
       $.post("planacomorc.php?modulo=principal/unidade/listapimanter&acao=A&requisicao=detalharPiDelegadas&pliid=" + pliid, function(html) {
           $('#detalhePiDelegadas .modal-body').html(html);
           $('#detalhePiDelegadas').modal();
       });
    }

    function onFiltropiNovo()
    {
        window.location.assign('planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A');
    }

    function alterarPi(pliid)
    {
        window.location.assign('planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A&pliid=' + pliid);
    }
    
    function alterarPiFnc(pliid)
    {
        window.location.assign('planacomorc.php?modulo=principal/unidade/cadastro_pi_fnc&acao=A&pliid=' + pliid);
    }

    function removerPi(pliid)
    {
        bootbox.confirm('Tem certeza que deseja apagar o PI?', function(confirmacao){
            if (confirmacao) {
                window.location.assign('planacomorc.php?modulo=principal/unidade/cadastro_pi'+ sufixoUrl+ '&acao=A&apagar=true&pliid=' + pliid);
            }
        });
    }

    /**
     * Exibe popup com Detalhes do pi e Gráfico.
     * 
     * @returns VOID
     */
    function exibirGrafico(pliid) {
       $('#detalhepi .modal-body').empty();
       $.post(
            "planacomorc.php?modulo=principal/unidade/listapimanter&acao=A&requisicao=exibirGrafico&pliid=" + pliid,
            function(html) {
                $('#detalhepi .modal-body').html(html);
                $('#detalhepi').modal();
       });
    }
    
    function initLinkEspelhoPi(){
        $('body').on('click', '.a_espelho', function(){
            var pliid = $(this).attr('data-pi');
            exibirEspelhoPi(pliid);
            return false;
        });
    }
    
    /**
     * Exibe popup com Detalhes do pi. Tela de Espelho de PI.
     * 
     * @returns VOID
     */
    function exibirEspelhoPi(pliid){
        window.open(
            'planacomorc.php?modulo=principal/unidade/espelho-pi&acao=A&acao=A&pliid='+ pliid,
            'popup_espelho_pi',
            'width=780,height=1000,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
    }

    /*
     * Abre o TED  em outra janela, no módulo de origem
     */
    function abrirTED(ted)
    {
        window.open('http://simec.mec.gov.br/ted/ted.php?modulo=principal/termoexecucaodescentralizada/previsao&acao=A&ted=' + ted);
    }

    /**
     * Abre a OBRA em outra janela, no módulo de origem
     */
    function abrirObra(obrid)
    {
        window.open('http://simec.mec.gov.br/obras/obras.php?modulo=principal/cadastro_pi&acao=A&obrid=' + obrid);
    }
