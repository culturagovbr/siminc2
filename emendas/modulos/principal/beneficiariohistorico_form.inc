<form id="formulario-historico" name="formulario-historico" method="post" class="form-horizontal">
    <input type="hidden" name="req" id="req" value="salvar-historico" />
    <input name="behid" id="behid" type="hidden" value="<?php echo $mBeneficiarioHistorico->behid; ?>">
    <input name="benid" type="hidden" value="<?php echo $mBeneficiarioHistorico->benid; ?>">

    <?php
    echo $simec->data('behdata', 'Data', $mBeneficiarioHistorico->behdata);
    echo $simec->textarea('behdsc', 'Hist�rico', $mBeneficiarioHistorico->behdsc);
    ?>

    <div class="form-group">
        <div class="text-center">
            <button class="btn btn-primary" type="button" id="btn-salvar-historico"><i class="fa fa-check"></i>&nbsp;Salvar</button>
        </div>
    </div>
</form>


<script type="text/javascript">
    $(function(){
        $('#btn-salvar-historico').click(function(){
            options = {
                success : function() {
                    // Carregar a listagem
                    jQuery("#div_listagem_historico").load('?modulo=principal/beneficiario_form&acao=A&req=listar-historico&benid='+$('#benid').val());

                    // Zerar o formul�rio
                    jQuery("#div_formulario_historico").load('?modulo=principal/beneficiario_form&acao=A&req=formulario-historico&benid='+$('#benid').val());
                }
            }

            jQuery("#formulario-historico").ajaxForm(options).submit();
        });
    });
</script>