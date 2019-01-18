<?php if($lista): ?>
    <div class="table-responsive">
        <table class="table table-striped table-bordered table-hover" >
            <thead>
            <tr class="text-center">
                <th width="10%">Ações</th>
                <th>Sub-Unidade Orçamentaria</th>
                <th>Sigla</th>
                <th>Cód. PI</th>
            </tr>
            </thead>
            <tbody>
            <?php foreach($lista as $dados){?>
                <tr>
                    <td class="text-center">
                        <a title="Alterar" class="editar-sub-unidade" href="#" data-suoid="<?= $dados['suoid']; ?>">
                            <i class="fa fa-pencil"></i>
                        </a>
                        <?php if($podeEditar): ?>
                          <a title="excluir" class="excluir-sub-unidade" href="#" data-suoid="<?= $dados['suoid']; ?>" data-suocod="<?= $dados['suocod']; ?>" data-suonome="<?= $dados['suonome']; ?>" style="margin-left: 5px;">
                              <i class="fa fa-close"></i>
                          </a>
                        <?php endif; ?>
                    </td>
                    <td><?php echo $dados['suonome']; ?></td>
                    <td><?php echo $dados['suosigla']; ?></td>
                    <td><?php echo $dados['suocodigopi']; ?></td>
                </tr>
            <?php } ?>
            </tbody>
        </table>
    </div>
<?php else: ?>
    <div class="alert alert-danger">Nenhum registro cadastrado</div>
<?php endif; ?>

<script>
    $(function(){

        $('.editar-sub-unidade').click(function(){
            var suoid = $(this).attr('data-suoid');
            $("#div_formulario_sub_unidade").load('?modulo=apoio/unidadeOrcamentaria_form&acao=A&req=formulario-sub-unidade&suoid='+ suoid);
            return false;
        });

        $('.excluir-sub-unidade').click(function(){
            suoid = $(this).attr('data-suoid');
            suocod = $(this).attr('data-suocod');
            suonome = $(this).attr('data-suonome');

            swal({
                title: 'Atenção!',
                text: 'Deseja realmente excluir a Subunidade '+ suocod+ '-'+ suonome+ '?',
                type: 'warning',
                showCancelButton: true,
                confirmButtonColor: '#DD6B55',
                confirmButtonText: 'Sim, estou certo!',
                closeOnConfirm: true
                },
                function(){
                    $.ajax({
                        type: 'DELETE',
                        url: '?modulo=apoio/unidadeOrcamentaria_form&acao=A&req=excluir-sub-unidade&suoid='+ suoid,
                        dataType: 'json',
                        success: function(response) {
                            if(response.result == 'success'){
                                // Carregar a listagem
                                $("#div_listagem_sub_unidade").load('?modulo=apoio/unidadeOrcamentaria_form&acao=A&req=listar-sub-unidade&unoid='+ $('#unoid').val());
                            } else {
                                swal('', response.message, response.result); // 'warning', 'error'
                            }
                        }
                    });
                });
            return false;
        });
    })
</script>
