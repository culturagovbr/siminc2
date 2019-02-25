<?php                                        
    $mEtapas = (new Planacomorc_Model_Etapas())->recuperarTodos("etaid, etadsc, pliid, etastatus, to_char(etadata, 'DD/MM/YYYY') as etadata", array('pliid = '.$_REQUEST['pliid'], "etastatus = 'A'"));
    if(count($mEtapas)){ 
        $podeEditar = $mPiPlanoInterno->verificarPermissaoEditar($estadoAtual, $perfis);
?>
    <div id="lista_etapas" class="table-responsive">
        <table class="table table-striped table-bordered table-hover"  id="tabela_etapas">
            <thead>
            <tr class="text-center">
                <th width="10%">Ações</th>
                <th>Descrição</th>
                <th>Data</th>
            </tr>
            </thead>
            <tbody id="teste">
            <?php $i=1;
            foreach($mEtapas as $dados):?>
                <tr id="<?=$i?>">
                    <td class="text-center">
                            <a title="Excluir" href="javascript:return false;" style="margin-left: 5px;"><i data-id="<?=$i?>" class="excluir-etapa fa fa-close"></i></a>
                        <?php
                        $i++;
                        ?>
                    </td>
                    <td><input type="hidden" name="etadsc[]" value="<?= $dados['etadsc']; ?>"><?= $dados['etadsc']; ?></td>
                    <td><input type="hidden" name="etadata[]" value="<?= $dados['etadata']; ?>"><?= $dados['etadata']; ?></td>
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
        $('.editar-etapa').click(function(){
            jQuery("#div_listagem_etapas").load(jQuery(this).attr('href'));
            return false;
        });

        $('.excluir-etapa').click(function(){
            var id = $(this).data("id");
            $("#"+id).remove();
            if ($("#tabela_etapas tbody tr").length==0){
                $("#div_listagem_etapas").html('<div class="alert alert-danger">Nenhum registro cadastrado</div>');
            }
        });
    })
</script>