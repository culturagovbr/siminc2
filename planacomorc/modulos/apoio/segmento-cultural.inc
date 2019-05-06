<?php
$podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;
if($lista): ?>
    <div class="table-responsive" id="lista-segmento">
        <table class="table table-striped table-bordered table-hover" >
            <thead>
            <tr class="text-center">
                <th width="10%">Ações</th>
                <th>Segmento</th>
            </tr>
            </thead>
            <tbody>
            <?php foreach($lista as $dados){?>
                <tr>
                    <td class="text-center">
                        <a title="Alterar" class="editar-segmento" href="javascript:alterarSegmento(<?= $dados['secid'];?>,'<?= $dados['neecod'];?>','<?= $dados['needsc'];?>')"><i class="fa fa-pencil"></i></a>
                        <?php if($podeEditar) { ?>
                            <a title="excluir" name="req" value="excluir-segmento" id="excluir-segmento" href="?modulo=apoio/area_cultural_form&acao=A&req=excluir-segmento&arceid=<?= $dados['arceid'];?>&secid=<?= $dados['secid'];?>" style="margin-left: 5px;"><i class="fa fa-close"></i></a>
                        <?php } ?>
                    </td>
                    <td><?php echo $dados['needsc']; ?></td>
                </tr>
            <?php } ?>
            </tbody>
        </table>
    </div>
<?php else: ?>
    <div class="alert alert-danger">Nenhum registro cadastrado</div>
<?php endif; ?>

<script>

    function alterarSegmento(secid, neecod, needsc){
        $("#secid").val(secid);
        $("#neecod").val(neecod);
        $("#needsc").val(needsc);
    }
</script>