<?php

    $acompanhamentoIndicadorPnc = new Acompanhamento_Model_Indicadorpnc((int)$_REQUEST['idpid']);
    if($acompanhamentoIndicadorPnc->idpid){
        $acompanhamentoPnc = new Acompanhamento_Model_Acompanhamentopnc($acompanhamentoIndicadorPnc->acpid);
    } else {
        $acompanhamentoPnc = new Acompanhamento_Model_Acompanhamentopnc($_REQUEST['acpid']);
        $acompanhamentoIndicadorPnc->unocod = $_REQUEST['unocod'];
        $acompanhamentoIndicadorPnc->suocod = $_REQUEST['suocod'];
        $acompanhamentoIndicadorPnc->ipncod = $_REQUEST['ipncod'];
        $acompanhamentoIndicadorPnc->prsano = $_REQUEST['prsano'];
    }
    
    # Buscando dados do Indicador PNC.
    $modelIndicadorPnc = new Public_Model_IndicadorPnc();
    $indicadorPnc = $modelIndicadorPnc->buscarPorCodigo((object)array(
        'ipncod' => $acompanhamentoIndicadorPnc->ipncod,
        'prsano' => $acompanhamentoIndicadorPnc->prsano));
    # Caso não exista indicador PNC cadastrado na tabela de apoio o sistema busca a descrição do Indicador do exercicio atual.
    if(!$indicadorPnc->ipnid){
        $indicadorPnc = $modelIndicadorPnc->buscarPorCodigo((object)array(
            'ipncod' => $acompanhamentoIndicadorPnc->ipncod,
            'prsano' => $_SESSION['exercicio']));
    }

?>

<form id="formulario-acompanhamento-indicador" name="formulario-acompanhamento-indicador" method="post" action="?modulo=principal/monitorar-pnc&acao=A" class="form-horizontal">
    <input name="req" id="req" type="hidden" value="salvar-acompanhamento-indicador-pnc" />
    <button type="submit" class="hide"></button>
    <input name="idpid" type="hidden" value="<?= $acompanhamentoIndicadorPnc->idpid; ?>">
    <input name="acpid" type="hidden" value="<?= $acompanhamentoPnc->acpid; ?>">
    <input name="unocod" type="hidden" value="<?= $acompanhamentoIndicadorPnc->unocod; ?>">
    <input name="suocod" type="hidden" value="<?= $acompanhamentoIndicadorPnc->suocod; ?>">
    <input name="ipncod" type="hidden" value="<?= $acompanhamentoIndicadorPnc->ipncod; ?>">
    <input name="prsano" type="hidden" value="<?= $acompanhamentoIndicadorPnc->prsano; ?>">

    <?php

        $simec->setPodeEditar(0);
        echo $simec->input('ipncod', 'Código', $indicadorPnc->ipncod);
        echo $simec->input('ipndsc', 'Indicador', $indicadorPnc->ipndsc);
        echo $simec->input('prsano', 'Ano', $acompanhamentoIndicadorPnc->prsano);
        $simec->setPodeEditar(1);
        echo $simec->input(
            'idpquantidade',
            'Quantidade',
            $acompanhamentoIndicadorPnc->idpid && !$acompanhamentoIndicadorPnc->idpquantidade? 0: $acompanhamentoIndicadorPnc->idpquantidade,
            ['required', 'maxlength' => 9]
        );
        if($acompanhamentoIndicadorPnc->prsano != $_SESSION['exercicio']){
            echo $simec->data('idpdata', 'Data', $acompanhamentoIndicadorPnc->idpdata);
        } else {
            echo $simec->data('idpdata', 'Data', $acompanhamentoIndicadorPnc->idpdata, ['required']);
        }
        # Caso o exercicio seja diferente(Na prática anterior ao ano do exercicio selecionado) o sistema exibe o campo de justificativa
        if($acompanhamentoIndicadorPnc->prsano != $_SESSION['exercicio']){
            echo $simec->textarea(
                'idpjustificativa',
                'Justificativa',
                $acompanhamentoIndicadorPnc->idpjustificativa
            );
        }

    ?>
    
</form>

<script>

    $(function(){

        $("#formulario-acompanhamento-indicador").submit(function(e) {
            $.ajax({
                type: "POST",
                url: $(this).attr('action'),
                data: $("#formulario-acompanhamento-indicador").serialize(), // serializes the form's elements.
                dataType: "json",
                success: function(response){
                    $('#div_modal_acompanhamento_indicador').modal('hide');
                    carregarListaIndicadorPnc(response.acpid);
                }
            });

            e.preventDefault(); // avoid to execute the actual submit of the form.
        });

    });

</script>
