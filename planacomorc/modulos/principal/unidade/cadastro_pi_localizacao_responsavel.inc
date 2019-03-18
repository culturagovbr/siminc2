<div class="col-md-6">

    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5>Localização do Projeto <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></h5>
        </div>
        <div class="ibox-content">
            <div class="form-group">
                <div class="col-md-3 text-right">
                    <label for="esfid" class="control-label">Tipo de localização:</label>
                </div>
                <div class="col-lg-9">
                    <?php
                    $sql = "
                                    SELECT
                                        esfid AS codigo,
                                        esfdsc AS descricao
                                    FROM territorios.esfera
                                    ORDER BY
                                        descricao
                                ";
                    $esfid = $dadosPI['esfid'];
                    $db->monta_combo('esfid', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'esfid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, null);
                    ?>
                </div>
            </div>
            <div class="form-group">
                <div class="col-lg-12">
                    <table id="table_localizacao" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                        <tr class="tr_head">
                            <td style="text-align: left;"><b>UF</b></td>
                            <td style="text-align: left;"><b>Município</b></td>
                            <td style="text-align: center;"></td>
                        </tr>
                        <?php if($dadosPI['esfid'] == Territorios_Model_Esfera::K_MUNICIPAL){
                            foreach($aLocalizacao as $localizacao){ ?>
                                <tr style="height: 30px;" class="tr_localizacao_<?php echo $localizacao['muncod']; ?>" >
                                    <td style="text-align: left;"><?php echo $localizacao['estuf']; ?></td>
                                    <td style="text-align: left;"><?php echo $localizacao['mundescricao']; ?></td>
                                    <td style="text-align: center;">
                                        <input type="hidden" value="<?php echo $localizacao['muncod']; ?>" name="listaLocalizacao[]">
                                        <span class="glyphicon glyphicon-trash btnRemoverLocalizacao link" data-localizacao="<?php echo $localizacao['muncod']; ?>" />
                                    </td>
                                </tr>
                            <?php }
                        } ?>

                    </table>
                </div>
            </div>
            <div class="form-group">
                <div class="col-lg-12">
                    <table id="table_localizacao_estadual" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                        <tr class="tr_head">
                            <td style="text-align: left;"><b>UF</b></td>
                            <td style="text-align: left;"><b>Nome</b></td>
                            <td style="text-align: center;"></td>
                        </tr>
                        <?php if($dadosPI['esfid'] == Territorios_Model_Esfera::K_ESTADUAL){
                            foreach($aLocalizacao as $localizacao){ ?>
                                <tr style="height: 30px;" class="tr_localizacao_estadual_<?php echo $localizacao['estuf']; ?>" >
                                    <td style="text-align: left;"><?php echo $localizacao['estuf']; ?></td>
                                    <td style="text-align: left;"><?php echo $localizacao['estdescricao']; ?></td>
                                    <td style="text-align: center;">
                                        <input type="hidden" value="<?php echo $localizacao['estuf']; ?>" name="listaLocalizacaoEstadual[]">
                                        <span class="glyphicon glyphicon-trash btnRemoverLocalizacaoEstadual link" data-localizacao-estadual="<?php echo $localizacao['estuf']; ?>" />
                                    </td>
                                </tr>
                            <?php }
                        } ?>
                    </table>
                </div>
            </div>
            <div class="form-group">
                <div class="col-lg-12">
                    <table id="table_localizacao_exterior" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                        <tr class="tr_head">
                            <td style="text-align: left;"><b>País</b></td>
                            <td style="text-align: center;"></td>
                        </tr>
                        <?php if($dadosPI['esfid'] == Territorios_Model_Esfera::K_EXTERIOR){
                            foreach($aLocalizacao as $localizacao){ ?>
                                <tr style="height: 30px;" class="tr_localizacao_exterior_<?php echo $localizacao['paiid']; ?>" >
                                    <td style="text-align: left;"><?php echo $localizacao['paidescricao']; ?></td>
                                    <td style="text-align: center;">
                                        <input type="hidden" value="<?php echo $localizacao['paiid']; ?>" name="listaLocalizacaoExterior[]">
                                        <span class="glyphicon glyphicon-trash btnRemoverLocalizacaoExterior link" data-localizacao-exterior="<?php echo $localizacao['paiid']; ?>" />
                                    </td>
                                </tr>
                            <?php }
                        } ?>
                    </table>
                </div>
            </div>
            <div class="form-group text-right">
                <div class="col-lg-12">
                    <div class="text-right">
                        <input type="button" id="btn_selecionar_localizacao_estadual" title="Inserir Localização Estadual/Distrito Federal" value="Inserir Localização Estadual/Distrito Federal" class="btn btn-info" />
                        <input type="button" id="btn_selecionar_localizacao_exterior" title="Inserir Localização no Exterior" value="Inserir Localização no Exterior" class="btn btn-info" />
                        <input type="button" id="btn_selecionar_localizacao" title="Inserir Localização municipal" value="Inserir Localização municipal" class="btn btn-info" />
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<div class="col-md-6">
    <div class="ibox float-e-margins" id="div_responsaveis">
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
</div>