<?php if($listaMetasEstrategicas): ?>
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
            <?php foreach($listaMetasEstrategicas as $meta): ?>
                <tr>
                    <td class="text-center">
                        <a title="Alterar" class="editar-meta-estrategica" href="#" data-meeid="<?= $meta['meeid']; ?>">
                            <i class="fa fa-pencil"></i>
                        </a>
                        <?php if($podeEditar): ?>
                          <a title="excluir" class="excluir-meta-estrategica" href="#" data-meeid="<?= $meta['meeid']; ?>" data-meecod="<?= $meta['meecod']; ?>" data-meenome="<?= $meta['meenome']; ?>" style="margin-left: 5px;">
                              <i class="fa fa-close"></i>
                          </a>
                        <?php endif; ?>
                    </td>
                    <td align="center"><?= $meta['meecod']; ?></td>
                    <td><?= $meta['meenome']; ?></td>
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

        $('.editar-meta-estrategica').click(function(){
            var meeid = $(this).attr('data-meeid');
            $("#div_formulario_meta_estrategica").load('?modulo=apoio/objetivo_meta_diretriz_form&acao=A&req=formulario-meta-estrategica&meeid='+ meeid);
            return false;
        });

        $('.excluir-meta-estrategica').click(function(){
            meeid = $(this).attr('data-meeid');
            meecod = $(this).attr('data-meecod');
            meenome = $(this).attr('data-meenome');

            swal({
                title: 'Atenção!',
                text: 'Deseja realmente excluir a Meta ' + meecod + ' - ' + meenome + '?',
                type: 'warning',
                showCancelButton: true,
                confirmButtonColor: '#DD6B55',
                confirmButtonText: 'Sim, estou certo!',
                closeOnConfirm: true
                },
                function(){
                    $.ajax({
                        type: 'DELETE',
                        url: '?modulo=apoio/objetivo_meta_diretriz_form&acao=A&req=excluir-meta-estrategica&meeid='+ meeid,
                        dataType: 'json',
                        success: function() {
                            $("#div_listagem_meta_estrategica").load('?modulo=apoio/objetivo_meta_diretriz_form&acao=A&req=listar-meta-estrategica&obeid='+ $('#obeid').val());
                        }
                    });
                });
            return false;
        });
    })
</script>
