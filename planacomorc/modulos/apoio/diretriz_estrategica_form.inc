<form id="formulario" name="formulario" method="post" class="form-horizontal">
    <input type="hidden" name="req" id="req" value="salvar-diretriz-estrategica"/>
    <input name="dieid" id="dieid" type="hidden" value="<?= $mDiretrizEstrategica->dieid; ?>">
    <input name="obeid" id="obeid" type="hidden" value="<?= $_REQUEST['obeid']; ?>">

    <?php
        echo $simec->input('diecod', 'Código', $mDiretrizEstrategica->diecod, ['required', 'class' => 'upper-text', 'maxlength' => 4]);
        echo $simec->input('dienome', 'Nome', $mDiretrizEstrategica->dienome, ['required', 'maxlength' => 125]);
    ?>

    <div class="form-group">
        <div class="text-center">
            <a class="btn btn-primary" id="btn-salvar-diretriz-estrategica">
                <i class="fa fa-check"></i>&nbsp;Salvar
            </a>
        </div>
    </div>
</form>

<script>
    $(function () {

        $('#btn-salvar-diretriz-estrategica').click(function () {
            if ( ($('#diecod').val().trim() !== '') && ( $('#dienome').val().trim() !== '') ) {
                $.ajax({
                    type: 'POST',
                    dataType: 'json',
                    url: '?modulo=apoio/objetivo_meta_diretriz_form&acao=A&req=salvar-diretriz-estrategica',
                    data: {
                        obeid: $('#obeid').val(),
                        dieid: $('#dieid').val(),
                        diecod: $('#diecod').val().toUpperCase(),
                        dienome: $('#dienome').val(),
                    }, success: function(){
                        $('#div_listagem_diretriz_estrategica').load('?modulo=apoio/objetivo_meta_diretriz_form&acao=A&req=listar-diretriz-estrategica&obeid=' + $('#obeid').val());
                        $('#div_formulario_diretriz_estrategica').load('?modulo=apoio/objetivo_meta_diretriz_form&acao=A&req=formulario-diretriz-estrategica&dieid=' + $('#dieid').val());
                    }
                });
            }else{
                return false;
            }

        });

    });
</script>
