<?php

    $estado = new Territorios_Model_Estado();
    $atividade = new Acompanhamento_Model_Atividadepnc((int)$_REQUEST['atvid']);
    if($atividade->atvid){
        $acompanhamentoPnc = new Acompanhamento_Model_Acompanhamentopnc($atividade->acpid);
    } else {
        $acompanhamentoPnc = new Acompanhamento_Model_Acompanhamentopnc($_REQUEST['acpid']);
    }

?>

<form id="formulario-acompanhamento-atividade" name="formulario-acompanhamento-atividade" method="post" action="?modulo=principal/monitorar-pnc&acao=A" class="form-horizontal">
    <input name="req" id="req" type="hidden" value="salvar-acompanhamento-atividade-pnc" />
    <button type="submit" class="hide"></button>
    <input name="atvid" type="hidden" value="<?= $atividade->atvid; ?>">
    <input name="acpid" type="hidden" value="<?= $acompanhamentoPnc->acpid; ?>">

    <?php
        $simec->setPodeEditar(1);
        echo $simec->input('atvtitulo', 'Título da atividade', $atividade->atvtitulo, ['required']);
        echo $simec->input('atvproduto', 'Produto ou serviço resultante', $atividade->atvproduto, ['required']);
        echo $simec->valor('atvprodutoqtd', 'Quantidade', $atividade->atvprodutoqtd, ['required']);
        echo $simec->select('estuf', 'UF', NULL, $estado->recuperarSqlCombo(['estuf', 'estdescricao']));
        echo '<div id="municipio">';
        echo $simec->select('muncod', 'Município', $atividade->muncod, (new Territorios_Model_Municipio())->recuperarSqlCombo(['estuf', 'mundescricao']));
        echo '</div>';
        echo $simec->input('atvfonteinformacao', 'Fonte de verificação da informação', $atividade->atvfonteinformacao, ['required']);
    ?>

</form>

<script>

    $(function(){

        $("#formulario-acompanhamento-atividade").submit(function(e) {
            $.ajax({
                type: "POST",
                url: $(this).attr('action'),
                data: $("#formulario-acompanhamento-atividade").serialize(), // serializes the form's elements.
                dataType: "json",
                success: function(response){
                    $('#div_modal_acompanhamento_atividade').modal('hide');
                    carregarListaAtividadePnc(response.acpid);
                }
            });

            e.preventDefault(); // avoid to execute the actual submit of the form.
        });

        $('#estuf').change(function(){
            $.ajax({
                type: "GET",
                url: '?modulo=principal/monitorar-pnc&acao=A&req=carregar-municipio&estuf=' + $(this).val(),
                dataType: "HTML",
                success: function(response){
                    $('#municipio').html(response);
                }
            });
        });

    });

</script>
