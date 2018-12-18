
<form id="formulario-medida" name="formulario-medida" method="post" class="form-horizontal">
    <input type="hidden" name="req" id="req" value="salvar-medida" />
    <input name="medid" id="medid" type="hidden" value="<?php echo $mMedida->medid; ?>">
    <input name="claid" id="claid" type="hidden" value="<?php echo $mMedida->claid; ?>">

    <?php
        echo $simec->input('meddsc', 'Medida', $mMedida->meddsc, ['required', 'maxlength' => 200]);
    ?>

    <?php if($podeEditar): ?>
        <div class="form-group">
            <div class="text-center">
                <button class="btn btn-primary" id="btn-salvar-medida"><i class="fa fa-check"></i>&nbsp;Salvar</button>
            </div>
        </div>
    <?php endif; ?>
</form>

<script type="text/javascript">
    $(function(){
        
        $('#btn-salvar-medida').click(function() {

            // Valida a obrigatoriedade do campo.
            if ($('#meddsc').val() !== null && $('#meddsc').val() !== '') {

                options = {
                    success: function () {
                        // Carregar a listagem
                        $("#div_listagem_medida").load('?modulo=apoio/classificacao_form&acao=A&req=listar-medida&claid=' + $('#claid').val());

                        // Zerar o formulário
                        $("#div_formulario_medida").load('?modulo=apoio/classificacao_form&acao=A&req=formulario-medida&claid=' + $('#claid').val());
                    }
                };

                jQuery("#formulario-medida").ajaxForm(options).submit();
                return false;
            }
        });

    });
</script>