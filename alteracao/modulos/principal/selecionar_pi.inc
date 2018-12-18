<div id="divAlerta" class="alert alert-danger alert-dismissable">
    <button aria-hidden="true" data-dismiss="alert" class="close" type="button">×</button>
    <span id="divAlertaContent">Teste de alerta</span>
</div>

<div class="row">
    <div class="col-lg-12">
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <button class="btn btn-primary" type="button" id="btnSelecionarPi" name="btnSelecionarPi">
                    <span class="glyphicon glyphicon-plus"></span> Selecionar PI
                </button>
            </div>
        </div>
    </div>
</div>

<div class="row">
    <div class="col-lg-12">
        <div class="ibox float-e-margins">
            <div class="ibox-title" id="listaPisSelecionados">
                <?php include_once APPRAIZ.'alteracao/modulos/principal/lista_pis_selecionados.inc';?>
            </div>
        </div>
    </div>
</div>

<!--
    Início da Modal para listar pis disponíveis
-->
<div class="modal fade" id="seleciona_pi" tabindex="-1" role="dialog" aria-labelledby="myModalLabel">
    <div class="modal-dialog modal-lg" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" id="btnSelecionarPi" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                <h4 class="modal-title" id="myModalLabel">Selecionar PI</h4>
            </div>
            <div class="modal-body">
                
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>
<!--
Final da modal de listar pis disponíveis
-->

<script>
    $(document).ready(function(){
        $("#divAlerta").hide();
        $("#btnSelecionarPi").click(function(){
            var url = '?modulo=principal/cadastro_alteracoes&acao=C&req=carrega_lista_pis&pedid='+$("#pedid").val();
            $('#seleciona_pi .modal-body').load(url);
            $("#seleciona_pi").modal();
        });
    });

    function adicionaPiPedido(pliid, pedid){
        $.post('?modulo=principal/cadastro_alteracoes&acao=C&req=adiciona_pi_pedido',
        {
            pedid: pedid,
            pliid: pliid
        },
            function(retorno){
                if (retorno){
                    var url = '?modulo=principal/cadastro_alteracoes&acao=C&req=carrega_lista_pis_selecionados&pedid='+pedid;
                    $('#listaPisSelecionados').load(url);
                    url = '?modulo=principal/cadastro_alteracoes&acao=C&req=carrega_lista_pis&pedid='+$("#pedid").val();
                    $('#seleciona_pi .modal-body').load(url);                    
                }
            }
        );
    }
    
    function removePiPedido(pedid, pliselid){
        $.post('?modulo=principal/cadastro_alteracoes&acao=C&req=remove_pi_pedido',
        {
            pliselid: pliselid,
            pedid: pedid
        },
            function(retorno){
                retorno = JSON.parse(retorno);
                if (retorno.return){
                    var url = '?modulo=principal/cadastro_alteracoes&acao=C&req=carrega_lista_pis_selecionados&pedid='+pedid;
                    $('#listaPisSelecionados').load(url);
                }else{
                    swal('Atenção!', retorno.msg, 'warning');
                }
            }
        );        
    }    
</script>
