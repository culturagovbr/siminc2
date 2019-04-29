<div class="div_custeio_capital">
    <div class="ibox-title">
        <h5>Custeio e Capital</h5>
    </div>
    <div class="ibox-content">
        <div class="form-group">
            <div class="col-lg-12">
                <table cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                    <tr>
                        <td style="text-align: left;"></td>
                        <td colspan="3" style="text-align: center;">
                            <b>Total (Custeio e Capital)</b>
                        </td>
                    </tr>
                    <tr style="height: 30px;" class="tr_limite_autorizado_unidade">
                        <td style="text-align: left;">Limite Autorizado na Unidade: </td>
                        <td colspan="3" id="td_autorizado_sub_unidade" style="text-align: center;" >0,00</td>
                    </tr>
                    <tr style="height: 30px;" class="tr_limite_disponivel_unidade">
                        <td style="text-align: left;">
                            Limite Disponível na Unidade:
                            <input id="VlrSUDisponivel" type="hidden" value="" />
                        </td>
                        <td colspan="3" id="td_disponivel_sub_unidade" style="text-align: center;" >0,00</td>
                    </tr>
                    <tr>
                        <td style="text-align: left;"></td>
                        <td style="text-align: center;"><b>Custeio </b></td>
                        <td style="text-align: center;"><b>Capital</b></td>
                    </tr>
                    <tr>
                        <td style="text-align: left;">Autorizado na Funcional:</td>
                        <td style="text-align: left;" id="td_autorizado_funcional_custeio" >0,00</td>
                        <td style="text-align: center;" id="td_autorizado_funcional_capital" >0,00</td>
                    </tr>
                    <tr>
                        <td style="text-align: left;">
                            Saldo Disponível na Funcional:
                            <input id="VlrFuncionalDisponivelCusteio" type="hidden" value="" />
                            <input id="VlrFuncionalDisponivelCapital" type="hidden" value="" />
                        </td>
                        <td style="text-align: left;" id="td_disponivel_funcional_custeio" >0,00</td>
                        <td style="text-align: center;" id="td_disponivel_funcional_capital" >0,00</td>
                    </tr>
                    <tr>
                        <td id="" style="text-align: left;">Valor do Projeto: </td>
                        <td style="text-align: left;">
                            <input type="text" id="picvalorcusteio" name="picvalorcusteio" value="<?php echo !empty($dadosPI['picvalorcusteio']) ? number_format($dadosPI['picvalorcusteio'], 2, ',', '.') : ''; ?>" class="CampoEstilo normal form-control" />
                            <input type="hidden" id="vlrPiCusteio" value="<?php echo !empty($dadosPI['picvalorcusteio']) && !$_REQUEST['replicarProposta']? number_format($dadosPI['picvalorcusteio'], 2, ',', '.') : ''; ?>" />
                        </td>
                        <td style="text-align: center;">
                            <input type="text" id="picvalorcapital" name="picvalorcapital" value="<?php echo !empty($dadosPI['picvalorcapital']) ? number_format($dadosPI['picvalorcapital'], 2, ',', '.') : ''; ?>" class="CampoEstilo normal form-control" />
                            <input type="hidden" id="vlrPiCapital" value="<?php echo !empty($dadosPI['picvalorcapital']) && !$_REQUEST['replicarProposta']? number_format($dadosPI['picvalorcapital'], 2, ',', '.') : ''; ?>" />
                        </td>
                    </tr>
                </table>
            </div>
        </div>
    </div>
</div>
