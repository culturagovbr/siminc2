<?php
    $acpid = $_REQUEST['acpid'];
    $acompanhamentopnc = new Acompanhamento_Model_Acompanhamentopnc($acpid);
    $listaDeAnexos = $acompanhamentopnc->buscarAnexos();
?>

<div class="row">
    <div class="col-sm-12">
        <h3>Inclua novos anexos que tenham um papel importante para a verificação dos indicadores e atividades cadastradas.</h3>
    </div>
</div>

<table cellpadding="5" border="0" align="center" class="table_anexos table table-striped table-hover table-bordered table-hover atividades_table">
    <thead>
        <tr>
            <td style="text-align: left;"><b>Nome do arquivo</b></td>
            <td style="text-align: left;"><b>Descrição</b></td>
            <td style="text-align: left;"><b>Ação</b></td>
        </tr>
    </thead>
    <?php if($listaDeAnexos): ?>
        <tbody>
            <?php foreach($listaDeAnexos as $anexo): ?>
                <tr style="height: 30px;" class="tr_anexos_<?php echo $anexo['arqid']; ?>" >
                    <td style="text-align: left;">
                        <a href="#" onclick="javascript:abrirArquivo('<?php echo $anexo['arqid']; ?>'); return false;" ><?php echo $anexo['arqnome']. '.'. $anexo['arqextensao']; ?></a>
                    </td>
                    <td style="text-align: left;">
                        <?php echo $anexo['arqdescricao']; ?>
                    </td>
                    <td style="text-align: left;">
                        <?php if(Acompanhamento_Controller_Acompanhamentopnc::verificarPermissaoEditar($acpid)): ?>
                            <span style="color: red;">
                                <span class="glyphicon red glyphicon-trash excluir_anexo link" title="Excluir o arquivo <?php echo $anexo['arqnome']. '.'. $anexo['arqextensao']; ?>" data-acpid="<?php echo $acpid; ?>" data-arqid="<?php echo $anexo['arqid']; ?>" data-descricao="<?php echo $anexo['arqnome']. '.'. $anexo['arqextensao']; ?>" />
                            </span>
                        <?php endif; ?>
                    </td>
                </tr>
            <?php endforeach; ?>
        </tbody>
    <?php else: ?>
        <tfoot>
            <tr>
                <td colspan="3">Não existem anexos cadastrados pra essa Meta PNC.</td>
            </tr>
        </tfoot>
    <?php endif; ?>
</table>

<?php if(Acompanhamento_Controller_Acompanhamentopnc::verificarPermissaoEditar($acpid)): ?>
    <div>
            <ul style="text-align: right;">
                <button class="btn btn-success inserir_anexo" data-acpid="<?php echo $acpid; ?>" type="button">
                    <i class="fa fa-plus-circle"></i>&nbsp;Inserir Anexos
                </button>
            </ul>
    </div>
<?php endif; ?>

<script>
    $(function(){

        $('#btn_inserir_anexos').click(function(){
            abrirModalUpload();
        });

    });
</script>
