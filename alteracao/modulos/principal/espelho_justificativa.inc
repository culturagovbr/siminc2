<?php
    $justificativa = (new Alteracao_Model_Justificativa())->recuperaJustificativa($_REQUEST['pedid']);
?>

<h2 style="color: #1AB394">4. Justificativa
    <?php if ($podeEditarAlteracao === TRUE): ?>
        <a href="#" id="btnJustificativa" title="Editar">
            <span class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></span>
        </a>
    <?php endif; ?>
</h2>
<div class="tabs-container tabs">
    <div class="tab-content">
        <div class="ibox-content">
            <div class="row">
                <div class="col-lg-6 form-group">
                    <?php
                        $podeEditar = FALSE;
                        $simec->setPodeEditar($podeEditar);
                        $simec->setFormTipo(Simec_View_Helper::K_FORM_TIPO_VERTICAL);
                        echo $simec->input('jstnecessidade', '1. Necessidade de Altera��o', $justificativa->jstnecessidade, null, ['input-size' => 9, 'label-size' => 2]);
                        echo $simec->input('jstfinanciamento', '3. Formas de financiamento de Cr�dito e adequa��o da proposta � meta fiscal e ao limite de gastos', $justificativa->jstfinanciamento, null, ['input-size' => 9, 'label-size' => 2]);
                        echo $simec->input('jstmedida', '5. Medida Provis�ria', $justificativa->jstmedida, null, ['input-size' => 9, 'label-size' => 2]);
                    ?>
                </div>
                <div class="col-lg-6 form-group">
                    <?php
                        echo $simec->input('jstcausa', '2. Causa da Demanda', $justificativa->jstcausa, null, ['input-size' => 9, 'label-size' => 2]);
                        echo $simec->input('jstfontes', '4. Verifica��o das fontes, dos Identificadores de uso - Iduso e dos identificadores de Resultado Prim�rio - RP', $justificativa->jstfontes, null, ['input-size' => 9, 'label-size' => 2]);
                        echo $simec->input('jstlegislacao', '6. Legisla��o Espec�fica', $justificativa->jstlegislacao, null, ['input-size' => 9, 'label-size' => 2]);
                    ?>
                </div>
            </div>
            <div class="row">
                <div class="col-lg-12 form-group">
                    <?php
                        echo $simec->input('jstoutros', '7. Outras Informa��es', $justificativa->jstoutros, null, ['input-size' => 9, 'label-size' => 2]);
                    ?>
                </div>
            </div>
        </div>
    </div>
</div>
<script>
$(document).ready(function(){
    $("#btnJustificativa").click(function(){
        $("#wizard-t-3").click();
    });
});
</script>