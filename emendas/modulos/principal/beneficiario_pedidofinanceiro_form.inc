<form id="formulario-pedido" name="formulario-pedido" method="post" class="form-horizontal">
    <input type="hidden" name="req" id="req" value="salvar-pedido-financeiro" />
    <input name="pefid" id="pefid" type="hidden" value="<?php echo $mBeneficiarioPedidoFinanceiro->pefid; ?>">
    <div class="row">
        <div class="col-sm-12">
            <div class="form-group ">
                <label for="vlcusteiopedido" class="col-sm-3 col-md-3 col-lg-3 control-label">Vl. Custeio: </label>
                <div class="col-sm-9 col-md-9 col-lg-9 ">
                    <?php
                        if (date('d')>0 && date('d')<11){
                    ?>
                            <input name="vlcusteiopedido" id="vlcusteiopedido" type="text" value="<?=$mBeneficiarioPedidoFinanceiro->vlcusteiopedido;?>" class="form-control  inteiro">
                    <?php
                        }else{ 
                            echo $mBeneficiarioPedidoFinanceiro->vlcusteiopedido;
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
                    <?php
                        if (date('d')>0 && date('d')<11){
                    ?>                    
                            <input name="vlcapitalpedido" id="vlcapitalpedido" type="text" value="<?=$mBeneficiarioPedidoFinanceiro->vlcapitalpedido;?>" class="form-control  inteiro">
                    <?php
                        }else{ 
                            echo $mBeneficiarioPedidoFinanceiro->vlcapitalpedido;
                        }
                    ?>                            
                </div>
            </div>            
        </div>
    </div>
    <div class="row">
        <div class="col-sm-12">          
            <?php
            if (date('d')>0 && date('d')<11){
                echo $simec->select('vinid', 'Vinculação', $mBeneficiarioPedidoFinanceiro->vinid, (new Emendas_Model_Vinculacao())->recuperarSqlCombo());
            }else{
                echo '<div class="form-group ">';
                echo '<label for="vlcapitalpedido" class="col-sm-3 col-md-3 col-lg-3 control-label">Vinculação: </label>';
                echo '<div class="col-sm-9 col-md-9 col-lg-9 ">';
                echo $mBeneficiarioPedidoFinanceiro->vindsc;
                echo '</div>';
                echo '</div>';
            }
            ?>
        </div>
    </div>
    <div class="row">
        <div class="form-group col-sm-12">
            <div class="text-center">
                <button class="btn btn-primary" type="button" id="btn-salvar-pedido"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                <button class="btn btn-primary" type="button" id="btn-historico-pedido"><i class="fa fa-history"></i>&nbsp;Histórico</button>
            </div>
        </div>
    </div>
    <?php bootstrapPopup("Histórico de Pedidos",'historico-pedido',null, array('fechar'), array('tamanho' => 'lg')); ?>        
</form>


<script type="text/javascript">
    $(function(){

        $('#btn-salvar-pedido').click(function(){
            jQuery("#formulario-pedido").submit();
        });
        
        $('#btn-historico-pedido').click(function(){
            $('#historico-pedido .modal-body').load('?modulo=principal/beneficiario_form&acao=A&req=historico-pedido&benid='+ $("#benid").val());
            $('#historico-pedido').modal();
        });        
    });
</script>