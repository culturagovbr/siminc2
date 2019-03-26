<form id="formulario" name="formulario" method="post" class="form-horizontal">
    <input type="hidden" name="req" id="req" value="salvar-meta-estrategica"/>
    <input name="meeid" id="meeid" type="hidden" value="<?= $mMetaEstrategica->meeid; ?>">
    <input name="obeid" id="obeid" type="hidden" value="<?= $_REQUEST['obeid']; ?>">

    <?php
        echo $simec->input('meecod', 'Código', $mMetaEstrategica->meecod, ['required', 'class' => 'upper-text', 'maxlength' => 4]);
        echo $simec->input('meenome', 'Nome', $mMetaEstrategica->meenome, ['required', 'maxlength' => 125]);
    ?>

    <div class="form-group">
        <div class="text-center">
            <a class="btn btn-primary" id="btn-salvar-meta-estrategica">
                <i class="fa fa-check"></i>&nbsp;Salvar
            </a>
        </div>
    </div>
</form>

<script>
    $(function () {

        $('#btn-salvar-meta-estrategica').click(function () {
            if ( ($('#meecod').val().trim() !== '') && ( $('#meenome').val().trim() !== '') ) {
                $.ajax({
                    type: 'POST',
                    dataType: 'json',
                    url: '?modulo=apoio/objetivo_meta_diretriz_form&acao=A&req=salvar-meta-estrategica',
                    data: {
                        obeid: $('#obeid').val(),
                        meeid: $('#meeid').val(),
                        meecod: $('#meecod').val().toUpperCase(),
                        meenome: $('#meenome').val(),
                    }, success: function(){
                        $('#div_listagem_meta_estrategica').load('?modulo=apoio/objetivo_meta_diretriz_form&acao=A&req=listar-meta-estrategica&obeid=' + $('#obeid').val());
                        $('#div_formulario_meta_estrategica').load('?modulo=apoio/objetivo_meta_diretriz_form&acao=A&req=formulario-meta-estrategica&meeid=' + $('#meeid').val());
                    }
                });
            }else{
                return false;
            }

        });

    });
</script>
