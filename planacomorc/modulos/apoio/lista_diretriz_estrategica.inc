<?php if($listaDiretrizEstrategica): ?>
    <div class="table-responsive">
        <table class="table table-striped table-bordered table-hover" >
            <thead>
            <tr class="text-center">
                <th width="10%">Ações</th>
                <th>Código</th>
                <th>Nome</th>
            </tr>
            </thead>
            <tbody>
            <?php foreach($listaDiretrizEstrategica as $diretriz): ?>
                <tr>
                    <td class="text-center">
                        <a title="Alterar" class="editar-diretriz-estrategica" href="#" data-dieid="<?= $diretriz['dieid']; ?>">
                            <i class="fa fa-pencil"></i>
                        </a>
                        <?php if($podeEditar): ?>
                            <a title="excluir" class="excluir-diretriz-estrategica" href="#" data-dieid="<?= $diretriz['dieid']; ?>" data-diecod="<?= $diretriz['diecod']; ?>" data-dienome="<?= $diretriz['dienome']; ?>" style="margin-left: 5px;">
                                <i class="fa fa-close"></i>
                            </a>
                        <?php endif; ?>
                    </td>
                    <td align="center"><?= $diretriz['diecod']; ?></td>
                    <td><?= $diretriz['dienome']; ?></td>
                </tr>
            <?php endforeach; ?>
            </tbody>
        </table>
    </div>
<?php else: ?>
    <div class="alert alert-danger">Nenhum registro cadastrado</div>
<?php endif; ?>

<script>
    $(function(){

        $('.editar-diretriz-estrategica').click(function(){
            var dieid = $(this).attr('data-dieid');
            $("#div_formulario_diretriz_estrategica").load('?modulo=apoio/objetivo_meta_diretriz_form&acao=A&req=formulario-diretriz-estrategica&dieid='+ dieid);
            return false;
        });

        $('.excluir-diretriz-estrategica').click(function(){
            dieid = $(this).attr('data-dieid');
            diecod = $(this).attr('data-diecod');
            dienome = $(this).attr('data-dienome');

            swal({
                    title: 'Atenção!',
                    text: 'Deseja realmente excluir a Meta ' + diecod + ' - ' + dienome + '?',
                    type: 'warning',
                    showCancelButton: true,
                    confirmButtonColor: '#DD6B55',
                    confirmButtonText: 'Sim, estou certo!',
                    closeOnConfirm: true
                },
                function(){
                    $.ajax({
                        type: 'DELETE',
                        url: '?modulo=apoio/objetivo_meta_diretriz_form&acao=A&req=excluir-diretriz-estrategica&dieid='+ dieid,
                        dataType: 'json',
                        success: function() {
                            $("#div_listagem_diretriz_estrategica").load('?modulo=apoio/objetivo_meta_diretriz_form&acao=A&req=listar-diretriz-estrategica&obeid='+ $('#obeid').val());
                        }
                    });
                });
            return false;
        });
    })
</script>
