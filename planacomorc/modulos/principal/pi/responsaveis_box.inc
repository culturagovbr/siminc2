<div id="div_responsaveis">
    <div class="ibox-title">
        <h5 class="legend_responsaveis">Responsáveis pelo Projeto</h5>
    </div>
    <div class="ibox-content">
        <div class="form-group">
            <div class="col-lg-12">
                <table id="table_responsaveis" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                    <tr>
                        <td style="text-align: left;"><b>Nome</b></td>
                        <td style="text-align: left;"><b>Telefone</b></td>
                        <td style="text-align: left;"><b>E-mail</b></td>
                        <td style="text-align: center;"><b>Ação</b></td>
                    </tr>
                    <?php foreach($aResponsavel as $responsavel){ ?>
                        <tr style="height: 30px;" class="tr_responsaveis_<?php echo $responsavel['pirid']; ?>" >
                            <td style="text-align: left;"><?php echo $responsavel['usunome']; ?></td>
                            <td style="text-align: left;"><?php echo $responsavel['telefone']; ?></td>
                            <td style="text-align: left;"><?php echo $responsavel['usuemail']; ?></td>
                            <td style="text-align: center;">
                                <input type="hidden" value="<?php echo $responsavel['usucpf']; ?>" name="listaResponsaveis[]">
                                <span class="glyphicon glyphicon-trash btnRemoverResponsaveis link" data-responsaveis="<?php echo $responsavel['pirid']; ?>" />
                            </td>
                        </tr>
                    <?php } ?>
                </table>
            </div>
        </div>
        <div class="form-group text-right">
            <div class="col-lg-12">
                <input type="button" id="btn_selecionar_responsaveis" title="Inserir Responsáveis" value="Inserir Responsáveis" class="btn btn-info"/>
            </div>
        </div>
    </div>
</div>
