<form id="form-meta-ppa" name="form-meta-ppa" method="post" class="form-horizontal" action="?modulo=apoio/objetivoppa_form&acao=A">
    <input type="hidden" name="req" id="req" value="salvar-meta" />
    <input name="mppid" id="mppid" type="hidden" value="<?php echo $mMetappa->mppid; ?>">
    <input name="oppid" id="oppid" type="hidden" value="<?php echo $_REQUEST['oppid']; ?>">
    <?php
        echo $simec->input('mppcod', 'Código', $mMetappa->mppcod, ['required', 'maxlength' => 4, 'class'=> 'upper-text']);
        echo $simec->textarea('mppnome', 'Meta', $mMetappa->mppnome, ['required', 'maxlength' => 400]);
        echo $simec->textarea('mppdsc', 'Descrição', $mMetappa->mppdsc, ['required', 'maxlength' => 1000]);
        echo $simec->radioBoolean('mppquantificavel', 'Possui medida quantificável', $mMetappa->mppquantificavel);
        $mostra=$mMetappa->mppquantificavel=='t'?true:false;
        echo '<div id="div_mppquantidade">';
        echo $simec->radioBoolean('mppanualizavel', 'Anualizável', $mMetappa->mppanualizavel);
        echo $simec->input('mppquantidade', 'Quantidade', $mMetappa->mppquantidade, ['class' => 'inteiro', 'required', 'inteiro']);
        echo '</div>';
    ?>
    <div class="modal-footer">
        <button class="btn btn-primary" type="submit" id="btn-salvar-meta"><i class="fa fa-check"></i>&nbsp;Salvar</button>
        <?php if($mMetappa->mppid){ ?>
            <a class="btn btn-danger link-excluir btn-excluir-meta" data-mppid="<?= $mMetappa->mppid ?>" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
        <?php } ?>
    </div>
</form>
<?php
if ($mostra){
    echo "<script>
            $('#div_mppquantidade').show('slow');
            $('#mppquantidade').attr('required', 'required');
          </script>";
}else{
    echo "<script>
            $('#div_mppquantidade').hide('slow');
            $('#mppquantidade').removeAttr('required');
            $('#mppquantidade').val('');
         </script>";    
}
?>
<script>
    $(document).ready(function(){
        $('#btnNovaMeta').click(function(){
            $('#cadastro-metas').modal();
        });
        toggleQuantidade();
        $('#mppquantificavel').change(function(){
            toggleQuantidade();
        });
        $('.btn-excluir-meta').click(function () {
            var meta = $(this).attr('data-mppid');
            var oppid = $('#oppid').val();
            swal({
                    title: "Atenção!",
                    text: "Deseja realmente excluir o registro?",
                    type: "warning",
                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55",
                    confirmButtonText: "Sim, estou certo!",
                    closeOnConfirm: true
                },
                function(){
                    window.location.href = '?modulo=apoio/objetivoppa_form&acao=A&req=excluir-meta&mppid=' + meta + '&oppid=' + oppid;
                });
        });
    });
</script>
