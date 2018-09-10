
    /**
     * Incia evento pra gerar link de espelho
     * 
     * @returns VOID
     */
    function initLinkEspelho(){
        
        $('.a_espelho').click(function(){
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
            '?modulo=inicio&acao=C&req=espelho-pi&pliid='+ pliid,
            'popup_espelho_pi',
            'width=780,height=1000,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
    }
