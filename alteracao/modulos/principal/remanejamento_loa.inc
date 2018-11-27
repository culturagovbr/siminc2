<div class="row">
    <div class="col-lg-12">
        <div style="text-align: center; color: #1AB394;">
            <h3>PLANOS INTERNOS</h3>
        </div>
        <div class="ibox-title">
            <table id="tbListaPisSelecionados" class="table table-striped table-bordered table-hover dataTables" >
                <thead>
                    <tr class="text-center">
                        <th>Funcional</th>
                        <th>Id Planejamento</th>
                        <th>Número do PI</th>
                        <th>Título</th>
                        <th width="11%">Tipo</th>
                        <th>Suplementar</th>
                        <th>Cancelado</th>
                        <th>Suplementar por Excesso</th>
                        <th>Suplement1ar por Superávit</th>
                    </tr>
                </thead>
                <tbody>
                <?php
                foreach($aDadosPiSelecionados as $dados): ?>
                    <tr>
                        <td ><?= $dados['funcional']; ?></td>
                        <td ><?= $dados['pliid']; ?></td>
                        <td ><?= $dados['plicod']; ?></td>
                        <td ><?= $dados['plititulo']; ?></td>
                        <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                            <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                                <tr>
                                    <td style="border: none;">
                                        <input type="hidden" id="picusteio<?=$dados['pliselid'];?>" value="<?= number_format($dados['custeio'],2,',','.'); ?>">
                                        Custeio: R$ <?= number_format($dados['custeio'],2,',','.'); ?>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="border: none; border-top: 1px solid #e7eaec;">
                                        <input type="hidden" id="picapital<?=$dados['pliselid'];?>" value="<?= number_format($dados['capital'],2,',','.'); ?>">
                                        Capital: R$ <?= number_format($dados['capital'],2,',','.'); ?>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="border: none; border-top: 1px solid #e7eaec;">
                                        <input type="hidden" id="picquantidade<?=$dados['pliselid'];?>" value="<?= number_format($dados['picquantidade'],2,',','.'); ?>">
                                        Físico: <?= number_format($dados['picquantidade'],0,',','.'); ?>
                                    </td>
                                </tr>
                            </table>
                        </td>          
                        <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                            <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                                <tr>
                                    <td style="border: none;">
                                        <?=number_format($dados['vlsuplementarcusteio'],2,',','.');?>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="border: none; border-top: 1px solid #e7eaec;">
                                        <?=number_format($dados['vlsuplementarcapital'],2,',','.');?>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="border: none; border-top: 1px solid #e7eaec;">
                                        <?=number_format($dados['plisuplementarquantidade'],2,',','.');?>
                                    </td>
                                </tr>
                            </table>
                        </td>
                        <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                            <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                                <tr>
                                    <td style="border: none;">
                                        <?=number_format($dados['vlcancelarcusteio'],2,',','.');?>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="border: none; border-top: 1px solid #e7eaec;">
                                        <?=number_format($dados['vlcancelarcapital'],2,',','.');?>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="border: none; border-top: 1px solid #e7eaec;">
                                        <?=number_format($dados['plicancelarquantidade'],2,',','.');?>
                                    </td>
                                </tr>                    
                            </table>
                        </td>
                        <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                            <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                                <tr>
                                    <td style="border: none;">
                                        <?=number_format($dados['vlsuplementarexcessocusteio'],2,',','.');?>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="border: none; border-top: 1px solid #e7eaec;">
                                        <?=number_format($dados['vlsuplementarexcessocapital'],2,',','.');?>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="border: none; border-top: 1px solid #e7eaec;">
                                        <?=number_format($dados['plisuplementarexcessoquantidade'],2,',','.');?>
                                    </td>
                                </tr>
                            </table>
                        </td>
                        <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                            <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                                <tr>
                                    <td style="border: none;">
                                        <?=number_format($dados['vlsuplementarsuperavitcusteio'],2,',','.');?>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="border: none; border-top: 1px solid #e7eaec;">
                                        <?=number_format($dados['vlsuplementarsuperavitcapital'],2,',','.');?>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="border: none; border-top: 1px solid #e7eaec;">
                                        <?=number_format($dados['plisuplementarsuperavitquantidade'],2,',','.');?>
                                    </td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                <?php endforeach ?>
                </tbody>
            </table>
        </div>
    </div>
</div>
<!--Início tratamento datatables dinamicamente-->
<script>
    $(document).ready(function(){
        $("#tbListaPisSelecionados").dataTable({
            bPaginate: false,
            responsive: true,
            paging: true,
            pageLength: 25,
            pagingType: "full_numbers",
            "language": {
                "url": "/zimec/public/temas/simec/js/plugins/dataTables/Portuguese-Brasil.json"
            }
        });
        
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
<style>
div.dataTables_paginate ul.pagination {
    margin: 2px 0;
    white-space: nowrap;
}
.dataTables_wrapper .datatables-footer .dataTables_paginate .pagination {
	display: block;
	margin: 0;
}
.pagination {
    display: -webkit-inline-box;
}
.wizard > .content > .body ul {
    list-style: none !important; 
}
</style>
<!--Final tratamento datatables dinamico-->