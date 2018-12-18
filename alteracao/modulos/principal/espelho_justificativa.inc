<?php
    $justificativa = (new Alteracao_Model_Justificativa())->recuperaJustificativa($_REQUEST['pedid']);
?>

<h2 style="color: #1AB394">4. Justificativa
    <a href="javascript:window.location.href='?modulo=principal/cadastro_alteracoes&acao=C&pedid=<?=$_REQUEST['pedid']?>&tpdid=<?=$_REQUEST['tpdid']?>'" title="Editar">
        <span class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></span>
    </a>
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
                        echo $simec->input('jstnecessidade', '1. Necessidade de Alteração', $justificativa->jstnecessidade, null, ['input-size' => 9, 'label-size' => 2]);
                        echo $simec->input('jstfinanciamento', '3. Formas de financiamento de Crédito e adequação da proposta à meta fiscal e ao limite de gastos', $justificativa->jstfinanciamento, null, ['input-size' => 9, 'label-size' => 2]);
                        echo $simec->input('jstmedida', '5. Medida Provisória', $justificativa->jstmedida, null, ['input-size' => 9, 'label-size' => 2]);
                    ?>
                </div>
                <div class="col-lg-6 form-group">
                    <?php
                        echo $simec->input('jstcausa', '2. Causa da Demanda', $justificativa->jstcausa, null, ['input-size' => 9, 'label-size' => 2]);
                        echo $simec->input('jstfontes', '4. Verificação das fontes, dos Identificadores de uso - Iduso e dos identificadores de Resultado Primário - RP', $justificativa->jstfontes, null, ['input-size' => 9, 'label-size' => 2]);
                        echo $simec->input('jstlegislacao', '6. Legislação Específica', $justificativa->jstlegislacao, null, ['input-size' => 9, 'label-size' => 2]);
                    ?>
                </div>
            </div>
            <div class="row">
                <div class="col-lg-12 form-group">
                    <?php
                        echo $simec->input('jstoutros', '7. Outras Informações', $justificativa->jstoutros, null, ['input-size' => 9, 'label-size' => 2]);
                    ?>
                </div>
            </div>
        </div>
    </div>
</div>