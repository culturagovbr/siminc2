<div class="col-md-6">
    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5>Vinculações Eventuais do Projeto</h5>
        </div>
        <div class="ibox-content">
            <div class="row">
                <div class="col-md-3" id="div_sniic">
                    Números SNIIC
                    <div class="form-group">
                        <div class="col-md-10" style="padding-right: 3px;">
                            <input type="text" placeholder="Adicione" value="" class="CampoEstilo inteiro normal form-control" title="Número SNIIC" id="input_sniic" maxlength="10" name="input_sniic" />
                        </div>
                        <div class="col-lg-1" style="padding-left: 0;">
                            <input type="button" id="btn_adicionar_sniic" value="+" class="btn btn-info" />
                        </div>
                    </div>
                    <div class="form-group">
                        <div class="col-lg-12">
                            <table id="table_sniic" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                <?php foreach($aSniic as $sniic){ ?>
                                    <tr style="height: 30px;" id="tr_sniic_<?php echo $sniic['pisid']; ?>" >
                                        <td style="text-align: left;"><?php echo $sniic['pissniic']; ?></td>
                                        <td style="text-align: center;">
                                            <input type="hidden" name="lista_sniic[]" value="<?php echo $sniic['pissniic']; ?>" />
                                            <span class="glyphicon glyphicon-trash link btnRemoveSniic" data-sniic="<?php echo $sniic['pisid']; ?>" ></span>
                                        </td>
                                    </tr>
                                <?php } ?>
                            </table>
                        </div>
                    </div>
                </div>
                <div class="col-md-5">
                    Número do Processo
                    <div class="form-group">
                        <div class="col-md-10" style="padding-right: 3px;">
                            <input type="text" placeholder="Adicione" value="" class="CampoEstilo normal form-control" title="Número SEI" id="input_sei" name="input_sei" />
                        </div>
                        <div class="col-lg-1" style="padding-left: 0;">
                            <input type="button" id="btn_adicionar_sei" value="+" class="btn btn-info" />
                        </div>
                    </div>
                    <div class="form-group">
                        <div class="col-lg-12">
                            <table id="table_sei" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                <?php foreach($aSei as $sei){ ?>
                                    <tr style="height: 30px;" id="tr_sei_<?php echo $sei['pseid']; ?>" >
                                        <td style="text-align: left;"><?php echo $sei['psesei']; ?></td>
                                        <td style="text-align: center;">
                                            <input type="hidden" name="lista_sei[]" value="<?php echo $sei['psesei']; ?>" />
                                            <span class="glyphicon glyphicon-trash link btnRemoveSei" data-sei="<?php echo $sei['pseid']; ?>" ></span>
                                        </td>
                                    </tr>
                                <?php } ?>
                            </table>
                        </div>
                    </div>
                </div>
                <div class="col-md-4" id="div_pronac">
                    Número Pronac
                    <div class="form-group">
                        <div class="col-md-10" style="padding-right: 3px;">
                            <input type="text" placeholder="Adicione" value="" class="CampoEstilo normal form-control" title="Número PRONAC" id="input_pronac" name="input_pronac" />
                        </div>
                        <div class="col-lg-1" style="padding-left: 0;">
                            <input type="button" id="btn_adicionar_pronac" value="+" class="btn btn-info" />
                        </div>
                    </div>
                    <div class="form-group">
                        <div class="col-lg-12">
                            <table id="table_pronac" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                <?php foreach($aPronac as $pronac){ ?>
                                    <tr style="height: 30px;" id="tr_pronac_<?php echo $pronac['pprid']; ?>" >
                                        <td style="text-align: left;"><?php echo $pronac['pprpronac']; ?></td>
                                        <td style="text-align: center;">
                                            <input type="hidden" name="lista_pronac[]" value="<?php echo $pronac['pprpronac']; ?>" />
                                            <span class="glyphicon glyphicon-trash link btnRemovePronac" data-pronac="<?php echo $pronac['pprid']; ?>" ></span>
                                        </td>
                                    </tr>
                                <?php } ?>
                            </table>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>