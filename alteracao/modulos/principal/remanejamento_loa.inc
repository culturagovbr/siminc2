<style>
    .wizard > .content {
        overflow: visible;
    }

    .wizard .chosen-container .chosen-results {
        height: 80px;
        overflow-y: auto;
        overflow-x: hidden;
    }

    .wizard .chosen-container .chosen-drop {
        height: 120px;
    }

</style>
<div class="row">
    <div class="col-lg-12">
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>PLANOS INTERNOS</h5>
            </div>
            <div class="ibox-content">
                <?php include APPRAIZ.'alteracao/modulos/principal/lista_pi_loa.inc';?>
            </div>
        </div>
    </div>
</div>
<div class="row">
    <div class="col-lg-12">
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>LOA</h5>
            </div>
            <div class="ibox-content">
                <?php if ($podeEditar): ?>
                    <button class="btn btn-primary" type="button" id="btnloa">
                        <span class="glyphicon glyphicon-plus"></span> Adicionar
                    </button>
                <?php endif;
                    include APPRAIZ.'alteracao/modulos/principal/lista_loa.inc';
                ?>
            </div>
        </div>
    </div>
</div>
<!--Início tratamento datatables dinamicamente-->
<script>
    $(document).ready(function(){

        $(".btnRemovePi").click(function(){
            removePiPedido($("#pedid").val(), $(this).attr('pliselid'));
        });
        
        $(".input_valor").blur(function(){
            salvarCampo($(this).prop('name'), $(this).attr('pliselid'), $(this).val(), $(this).attr('tipo'));
        });
        $('.input_valor').keyup(function(){
            $(this).val(mascaraglobal('###.###.###.###,##', $(this).val()));
        }).keyup();
    });
    
    function salvarCampo(nomeCampo, pliselid, valor, tipo){
        if (valor!=''){
            var vlrCampo=0;
            var vlrcancelado=0;
            var vlrsuplementar=0;
            $('.input_valor').each(function(){
                if ($(this).attr('name')=='vlsuplementarcusteio' || $(this).attr('name')=='vlsuplementarcapital' || 
                    $(this).attr('name')=='vlsuplementarsuperavitcusteio' || $(this).attr('name')=='vlsuplementarsuperavitcapital' ||
                    $(this).attr('name')=='vlsuplementarexcessocusteio' || $(this).attr('name')=='vlsuplementarexcessocapital'){
                    vlrCampo = $(this).val() ? str_replace(['.', ','], ['', '.'], $(this).val()) : 0;
                    vlrsuplementar = parseFloat(vlrsuplementar)+parseFloat(vlrCampo);
                }else if ($(this).attr('name')=='vlcancelarcusteio' || $(this).attr('name')=='vlcancelarcapital'){
                    vlrCampo = $(this).val() ? str_replace(['.', ','], ['', '.'], $(this).val()) : 0;
                    vlrcancelado = parseFloat(vlrcancelado)+parseFloat(vlrCampo);
                }
            });
            vlrCampo = valor ? str_replace(['.', ','], ['', '.'], valor) : 0;
            if (parseFloat(vlrsuplementar)!=parseFloat(vlrcancelado)){
                swal({
                    title: "",
                    text: "Valor cancelado divergente do suplementado!",
                    type: "warning",
                    confirmButtonText: 'Ok',
                    closeOnConfirm: true
                },
                function(isConfirm){
                    if(isConfirm){
                        $.post('?modulo=principal/cadastro_alteracoes&acao=C&req=salvar_campo',
                        {
                            pliselid: pliselid,
                            campo: nomeCampo,
                            valor: vlrCampo
                        },
                            function(retorno){
                                if (retorno){
                                    var url = '?modulo=principal/cadastro_alteracoes&acao=C&req=carrega_lista_pis_selecionados&pedid='+$("#pedid").val();
                                    $('#listaPisSelecionados').load(url);
                                }
                            }
                        );
                    }
               });                
            }else{
                $.post('?modulo=principal/cadastro_alteracoes&acao=C&req=salvar_campo',
                {
                    pliselid: pliselid,
                    campo: nomeCampo,
                    valor: vlrCampo
                },
                    function(retorno){
                        if (retorno){
                            var url = '?modulo=principal/cadastro_alteracoes&acao=C&req=carrega_lista_pis_selecionados&pedid='+$("#pedid").val();
                            $('#listaPisSelecionados').load(url);
                        }
                    }
                );                
            }
        }
    }
</script>