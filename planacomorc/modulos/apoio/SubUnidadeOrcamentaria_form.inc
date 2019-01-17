<form id="formulario" name="formulario" method="post" class="form-horizontal">
    <input type="hidden" name="req" id="req" value="salvar-sub-unidade" />
    <input name="suoid" id="suoid" type="hidden" value="<?php echo $mSubUnidade->suoid;?>">
    <input name="unoid" id="unoid" type="hidden" value="<?php echo $mSubUnidade->unoid; ?>">

    <?php
    echo $simec->input('suocod', 'Código', $mSubUnidade->suocod, ['required', 'class'=>"inteiro", 'maxlength' => 6]);
    echo $simec->input('suonome', 'Nome', $mSubUnidade->suonome, ['required', 'maxlength' => 200]);
    echo $simec->input('suosigla', 'Sigla', $mSubUnidade->suosigla, ['required', 'maxlength' => 20]);
    echo $simec->radio('suoflaglimiteliberado', 'Limite Liberado', $mSubUnidade->suoflaglimiteliberado, ['t'=>'Sim', 'f'=>'Não']);
    echo $simec->input('suocodigopi', 'Código PI', $mSubUnidade->suocodigopi, ['required', 'maxlength' => 2]);
    ?>

    <div class="form-group">
        <div class="text-center">
            <button class="btn btn-primary" id="#btn-salvar-sub-unidade" type="submit"><i class="fa fa-check"></i>&nbsp;Salvar</button>
        </div>
    </div>
</form>

<script type="text/javascript">
    $(function(){

        $('#btn-salvar-sub-unidade').click(function(){
            options = {
                success: function() {
                    // Carregar a listagem
                    jQuery("#div_listagem_sub_unidade").load('?modulo=apoio/unidadeOrcamentaria_form&acao=A&req=listar-sub-unidade&$unoid='+$('#$unoid').val());

                    // Zerar o formulário
                    jQuery("#div_formulario_sub_unidade").load('?modulo=apoio/unidadeOrcamentaria_form&acao=A&req=formulario-sub-unidade&$unoid='+$('#$unoid').val());
                }
            }

            jQuery("#formulario-sub-unidade").ajaxForm(options).submit();
            return false;
        });

    });
</script>
