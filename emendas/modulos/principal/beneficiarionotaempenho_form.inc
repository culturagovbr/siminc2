<form id="formulario-nota-empenho" name="formulario-nota-empenho" method="post" class="form-horizontal">
    <input type="hidden" name="req" id="req" value="salvar-nota-empenho"/>
    <input name="neid" id="neid" type="hidden" value="<?= $mBeneficiarioNotaEmpenho->neid; ?>">
    <input name="benid" type="hidden" value="<?= $mBeneficiarioNotaEmpenho->benid; ?>">

    <?php
        echo $simec->input('numnotaempenho', 'Nº Nota de Empenho', $mBeneficiarioNotaEmpenho->numnotaempenho, ['required']);
    ?>

    <div class="form-group">
        <div class="text-center">
            <button class="btn btn-primary" type="button" id="btn-salvar-nota-empenho"><i class="fa fa-check"></i>&nbsp;Salvar
            </button>
        </div>
    </div>
</form>


<script type="text/javascript">
    $(document).ready(function () {
        $('#btn-salvar-nota-empenho').click(function () {
            var nota = $('#numnotaempenho').val();

            if (nota != '') {

                options = {
                    success: function () {
                        // Carregar a listagem
                        jQuery("#div_listagem_nota_empenho").load('?modulo=principal/beneficiario_form&acao=A&req=listar-nota-empenho&benid=' + $('#benid').val());
                        // Zerar o formulário
                        jQuery("#div_formulario_nota_empenho").load('?modulo=principal/beneficiario_form&acao=A&req=formulario-nota-empenho&benid=' + $('#benid').val());
                    }
                };

                jQuery("#formulario-nota-empenho").ajaxForm(options).submit();
                return true;
            }
            swal('Atenção!', 'Verificar a obrigatoriedade dos campos acima!', 'warning');
            return false;
        });
    });
</script>