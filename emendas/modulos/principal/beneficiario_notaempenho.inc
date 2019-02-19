<?php if(count($mBeneficiarioNotaEmpenho)){ ?>
    <div class="table-responsive">
        <table class="table table-striped table-bordered table-hover" >
            <thead>
            <tr class="text-center">
                <th width="10%">Ações</th>
                <th>Nota de Empenho</th>
            </tr>
            </thead>
            <tbody>
            <?php foreach($mBeneficiarioNotaEmpenho as $dados):?>
                <tr>
                    <td class="text-center">
                        <?php
                        if ($podeEditar){
                        ?>
                            <a title="Alterar" class="editar-nota-empenho" href="?modulo=principal/beneficiario_form&acao=A&req=formulario-nota-empenho&neid=<?= $dados['neid']; ?>"><i class="fa fa-pencil"></i></a>
                            <a title="Excluir" class="excluir-nota-empenho" href="?modulo=principal/beneficiario_form&acao=A&req=excluir-nota-empenho&neid=<?= $dados['neid']; ?>" style="margin-left: 5px;"><i class="fa fa-close"></i></a>
                        <?php
                        }
                        ?>
                    </td>
                    <td><?= $dados['numnotaempenho']; ?></td>
                </tr>
            <?php endforeach; ?>
            </tbody>
        </table>
    </div>
<?php } else {
    echo '<div class="alert alert-danger">Nenhum registro cadastrado</div>';
} ?>

<script>
    $(function(){
        $('.editar-nota-empenho').click(function(){
            jQuery("#div_formulario_nota_empenho").load(jQuery(this).attr('href'));
            return false;
        });

        $('.excluir-nota-empenho').click(function(){
            href = $(this).attr('href');

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
                    $.ajax({
                        url: href,
                        success: function (benid) {
                            jQuery("#div_listagem_nota_empenho").load('?modulo=principal/beneficiario_form&acao=A&req=listar-nota-empenho&benid=' + benid);
                        }
                    });
                });
            return false;
        });
    })
</script>