<div class="ibox-title">
    <h5>Anexos</h5>
</div>
<div class="ibox-content">
    <div class="form-group">
        <div class="col-lg-12">
            <table id="table_anexos" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                <tr>
                    <td style="text-align: left;"><b>Nome do arquivo</b></td>
                    <td style="text-align: left;"><b>Descrição</b></td>
                    <td style="text-align: center;"><b>Ação</b></td>
                </tr>
                <?php foreach($listaDeAnexos as $anexo){ ?>
                    <tr style="height: 30px;" class="tr_anexos_<?php echo $anexo['arqid']; ?>" >
                        <td style="text-align: left;"><a href="#" onclick="javascript:abrirArquivo('<?php echo $anexo['arqid']; ?>'); return false;" ><?php echo $anexo['arqnome']. '.'. $anexo['arqextensao']; ?></a></td>
                        <td style="text-align: left;"><?php echo $anexo['arqdescricao']; ?></td>
                        <td style="text-align: center;">
                            <input type="hidden" value="<?php echo $anexo['arqid']; ?>" name="listaAnexos[]">
                            <span class="glyphicon glyphicon-trash btnRemoverAnexos link" title="Excluir o arquivo <?php echo $anexo['arqnome']. '.'. $anexo['arqextensao']; ?>" data-anexos="<?php echo $anexo['arqid']; ?>" />
                        </td>
                    </tr>
                <?php } ?>
            </table>
        </div>
    </div>
    <div class="form-group text-right">
        <div class="col-lg-12">
            <input type="button" id="btn_inserir_anexos" title="Inserir Anexos" value="Inserir Anexos" class="btn btn-info"/>
        </div>
    </div>
</div>
