<?php
    # Permite que insira somente nos 10 primeiros dias do mês corrente.
    $condicaoDiaMes = date('d')>0 && date('d')<11;
?>
<form id="formulario-pedido" name="formulario-pedido" method="post" class="form-horizontal">
    <input type="hidden" name="req" id="req" value="salvar-pedido-financeiro" />
    <input name="pefid" id="pefid" type="hidden" value="<?= $PedidoFinanceiro->pefid ?>">
    <div class="row">
        <div class="col-sm-12">
            <div class="form-group ">
                <label for="vlcusteiopedido" class="col-sm-3 col-md-3 col-lg-3 control-label">Vl. Custeio: </label>
                <div class="col-sm-9 col-md-9 col-lg-9 ">
                    <?php if ($condicaoDiaMes) { ?>
                            <input name="vlcusteiopedido" id="vlcusteiopedido" type="text" value="<?= $PedidoFinanceiro->vlcusteiopedido; ?>"
                                   class="form-control input_valor_financeiro">
                    <?php
                        } else {
                            echo number_format($PedidoFinanceiro->vlcusteiopedido,0,',', '.' );
                        }
                    ?>
                </div>
            </div>
        </div>
    </div>
    <div class="row">    
        <div class="col-sm-12">
            <div class="form-group ">
                <label for="vlcapitalpedido" class="col-sm-3 col-md-3 col-lg-3 control-label">Vl. Capital: </label>
                <div class="col-sm-9 col-md-9 col-lg-9 ">
                    <?php if ($condicaoDiaMes) { ?>
                            <input name="vlcapitalpedido" id="vlcapitalpedido" type="text" value="<?= $PedidoFinanceiro->vlcapitalpedido; ?>"
                                   class="form-control input_valor_financeiro">
                    <?php
                        } else {
                            echo number_format($PedidoFinanceiro->vlcapitalpedido,0,',', '.' );
                        }
                    ?>
                </div>
            </div>            
        </div>
    </div>
    <div class="row">
        <div class="col-sm-12">
            <?php
                if ($condicaoDiaMes) {
                    echo $simec->select('vinid', 'Vinculação', $PedidoFinanceiro->vinid, (new Emendas_Model_Vinculacao())->recuperarSqlCombo());
                } else {
                    echo '<div class="form-group ">';
                    echo '<label for="vlcapitalpedido" class="col-sm-3 col-md-3 col-lg-3 control-label">Vinculação: </label>';
                    echo '<div class="col-sm-9 col-md-9 col-lg-9 ">';
                    echo $PedidoFinanceiro->vindsc;
                    echo '</div>';
                    echo '</div>';
                }
            ?>
        </div>
    </div>
    <?php if ($condicaoDiaMes): ?>
    <div class="row">
        <div class="form-group col-sm-12">
            <div class="text-center">
                <button class="btn btn-primary" type="button" id="btn-salvar-financeiro"><i class="fa fa-check"></i>&nbsp;Salvar</button>
            </div>
        </div>
    </div>
    <?php endif;
        bootstrapPopup("Histórico de Pedidos",'historico-financeiro',null, array('fechar'), array('tamanho' => 'lg'));
    ?>
</form>
