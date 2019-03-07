<form id="formulario" name="formulario" method="post" class="form-horizontal">
    <input type="hidden" name="req" id="req" value="salvar-subitem" />
    <input name="masid" id="masid" type="hidden" value="<?php echo $mSubItem->masid;?>">
    <input name="maiid" id="maiid" type="hidden" value="<?php echo $mSubItem->maiid; ?>">
    <input name="prsano" id="prsano" type="hidden" value="<?php echo $mSubItem->prsano; ?>">

    <?php
        echo $simec->input('masnome', 'Nome', $mSubItem->masnome, ['required','maxlength' => 200]);
        echo $simec->input('masdescricao', 'Descrição', $mSubItem->masdescricao, ['maxlength' => 500]);
    ?>

    <div class="form-group">
        <div class="text-center">
            <button class="btn btn-primary" id="#btn-salvar-subitem" type="submit"><i class="fa fa-check"></i>&nbsp;Salvar</button>
        </div>
    </div>
</form>

<script type="text/javascript">
    $(function(){

        $('#btn-salvar-subitem').click(function(){
            options = {
                success: function() {
                    // Carregar a listagem
                    jQuery("#div_listagem_subitem").load('?modulo=apoio/item-manutencao-form&acao=A&req=listar-subitem&$maiid='+$('#maiid').val());

                    // Zerar o formulário
                    jQuery("#div_formulario_subitem").load('?modulo=apoio/item-manutencao-form&acao=A&req=formulario-subitem&$maiid='+$('#maiid').val());
                }
            }

            jQuery("#formulario-subitem").ajaxForm(options).submit();
            return false;
        });

    });
</script>
