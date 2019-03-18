<?php

    if($_REQUEST['reqpopup'] == 'espelho-pi') {
        include APPRAIZ. 'planacomorc/modulos/principal/unidade/espelho-pi.inc';
        die;
    }

    $podeEditarAlteracao = FALSE;
    include APPRAIZ . "includes/cabecalho_bootstrap_v3_popup.inc";
    include APPRAIZ.'alteracao/modulos/principal/espelho_alteracao.inc';

?>
<script>

    $(document).ready(function(){
        
        $('body').on('click', '.a_espelho', function(){
            var pliid = $(this).attr('data-pi');
            exibirEspelhoPi(pliid);
            return false;
        });

    });
    
    /**
     * Exibe popup com Detalhes do pi. Tela de Espelho de PI.
     *
     * @returns VOID
     */
    function exibirEspelhoPi(pliid){
        window.open(
            window.location.href + '&reqpopup=espelho-pi&pliid='+ pliid,
            'popup_espelho_pi',
            'width=780,height=1000,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
    }

</script>