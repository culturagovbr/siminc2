<form id="formulario-observacao" name="formulario-observacao" method="post" class="form-horizontal">
    <input type="hidden" name="req" id="req" value="salvar-observacao"/>
    <input name="bobid" id="bobid" type="hidden" value="<?= $mBeneficiarioObservacao->bobid; ?>">
    <input name="benid" type="hidden" value="<?= $mBeneficiarioObservacao->benid; ?>">

    <?php
        echo $simec->data('bobdata', 'Data', $mBeneficiarioObservacao->bobdata, ['required']);
        echo $simec->textarea('bobdsc', 'Observações', $mBeneficiarioObservacao->bobdsc, ['required']);
    ?>

    <div class="form-group">
        <div class="text-center">
            <button class="btn btn-primary" type="button" id="btn-salvar-observacao"><i class="fa fa-check"></i>&nbsp;Salvar
            </button>
        </div>
    </div>
</form>


<script type="text/javascript">
    $(document).ready(function () {
        $('#btn-salvar-observacao').click(function () {
            options = {
                success: function () {
                    // Carregar a listagem
                    jQuery("#div_listagem_observacao").load('?modulo=principal/beneficiario_form&acao=A&req=listar-observacao&benid=' + $('#benid').val());

                    // Zerar o formulário
                    jQuery("#div_formulario_observacao").load('?modulo=principal/beneficiario_form&acao=A&req=formulario-observacao&benid=' + $('#benid').val());
                }
            }

            jQuery("#formulario-observacao").ajaxForm(options).submit();
        });
    });
</script>