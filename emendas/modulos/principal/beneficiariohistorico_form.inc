<form id="formulario-historico" name="formulario-historico" method="post" class="form-horizontal">
    <input type="hidden" name="req" id="req" value="salvar-historico"/>
    <input name="bobid" id="bobid" type="hidden" value="<?= $mBeneficiarioObservacao->bobid; ?>">
    <input name="benid" type="hidden" value="<?= $mBeneficiarioObservacao->benid; ?>">

    <?php
        echo $simec->data('bobdata', 'Data', $mBeneficiarioObservacao->bobdata, ['required']);
        echo $simec->textarea('bobdsc', 'Observações', $mBeneficiarioObservacao->bobdsc, ['required']);
    ?>

    <div class="form-group">
        <div class="text-center">
            <button class="btn btn-primary" type="button" id="btn-salvar-historico"><i class="fa fa-check"></i>&nbsp;Salvar
            </button>
        </div>
    </div>
</form>


<script type="text/javascript">
    $(document).ready(function () {
        $('#btn-salvar-historico').click(function () {
            options = {
                success: function () {
                    // Carregar a listagem
                    jQuery("#div_listagem_historico").load('?modulo=principal/beneficiario_form&acao=A&req=listar-historico&benid=' + $('#benid').val());

                    // Zerar o formulário
                    jQuery("#div_formulario_historico").load('?modulo=principal/beneficiario_form&acao=A&req=formulario-historico&benid=' + $('#benid').val());
                }
            }

            jQuery("#formulario-historico").ajaxForm(options).submit();
        });
    });
</script>