<table id="tbListaPisSelecionados" class="table table-striped table-bordered table-hover dataTables" >
    <thead>
        <tr class="text-center">
            <th>Ações</th>
            <th>Funcional</th>
            <th>Id Planejamento</th>
            <th>Título</th>
            <th width="11%">Tipo</th>
            <th width="11%">Saldo a Empenhar</th>
            <th>Suplementar</th>
            <th>Cancelar</th>
            <th>Suplementar por Excesso</th>
            <th>Suplementar por Superávit</th>
            <th>Resultado Provável</th>
        </tr>
    </thead>
    <tbody>
    <?php
    foreach($aDadosPiSelecionados as $dados): ?>
        <tr>
            <td style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;" align="center">
                <button class="btn btn-danger btnRemovePi" type="button" title="Excluir pedido" pliselid="<?=$dados['pliselid'];?>" name="btnSelecionarPi">
                    <span class="glyphicon glyphicon-trash"></span>
            </td>
            <td ><?= $dados['funcional']; ?></td>
            <td ><?= $dados['pliid']; ?></td>
            <td >
                <?= $dados['plititulo']; ?><br>
                <b>Unidade:</b> <?= $dados['unonome']; ?><br>
                <b>Produto:</b> <?= $dados['pprnome']; ?>
            </td>
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
            <td style="vertical-align: middle;">
                <?php
                    $empenhado = $dados['custeio']+$dados['capital']-$dados['empenhado'];
                    $empenhado = number_format($empenhado,2,',','.');
                    echo "R$ ".$empenhado;
                ?>
                <input type="hidden" id="empenhado<?=$dados['pliselid'];?>" value="<?=$empenhado;?>">
            </td>
            
            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                    <tr>
                        <td style="border: none;">
                            <input type="text" size="10" class="input_valor" name="vlsuplementarcusteio" tipo="custeio" pliselid="<?=$dados['pliselid'];?>" id="vlsuplementarcusteio<?=$dados['pliselid'];?>" value="<?=$dados['vlsuplementarcusteio'];?>">
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <input type="text" size="10" class="input_valor" name="vlsuplementarcapital" tipo="capital" pliselid="<?=$dados['pliselid'];?>" id="vlsuplementarcapital<?=$dados['pliselid'];?>" value="<?=$dados['vlsuplementarcapital'];?>">
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <input type="text" size="10" class="input_valor" name="plisuplementarquantidade" tipo="fisico" pliselid="<?=$dados['pliselid'];?>" id="plisuplementarquantidade<?=$dados['pliselid'];?>" value="<?=$dados['plisuplementarquantidade'];?>">
                        </td>
                    </tr>
                </table>
            </td>
            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                    <tr>
                        <td style="border: none;">
                            <input type="text" size="10" class="input_valor" name="vlcancelarcusteio" tipo="custeio" pliselid="<?=$dados['pliselid'];?>" id="vlcancelarcusteio<?=$dados['pliselid'];?>" value="<?=$dados['vlcancelarcusteio'];?>">
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <input type="text" size="10" class="input_valor" name="vlcancelarcapital" tipo="capital" pliselid="<?=$dados['pliselid'];?>" id="vlcancelarcapital<?=$dados['pliselid'];?>" value="<?=$dados['vlcancelarcapital'];?>">
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <input type="text" size="10" class="input_valor" name="plicancelarquantidade" tipo="fisico" pliselid="<?=$dados['pliselid'];?>" id="plicancelarquantidade<?=$dados['pliselid'];?>" value="<?=$dados['plicancelarquantidade'];?>">
                        </td>
                    </tr>                    
                </table>
            </td>
            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                    <tr>
                        <td style="border: none;">
                            <input type="text" size="10" class="input_valor" name="vlsuplementarexcessocusteio" tipo="custeio" pliselid="<?=$dados['pliselid'];?>" id="vlsuplementarexcessocusteio<?=$dados['pliselid'];?>" value="<?=$dados['vlsuplementarexcessocusteio'];?>">
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <input type="text" size="10" class="input_valor" name="vlsuplementarexcessocapital" tipo="capital" pliselid="<?=$dados['pliselid'];?>" id="vlsuplementarexcessocapital<?=$dados['pliselid'];?>" value="<?=$dados['vlsuplementarexcessocapital'];?>">
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <input type="text" size="10" class="input_valor" name="plisuplementarexcessoquantidade" tipo="fisico" pliselid="<?=$dados['pliselid'];?>" id="plisuplementarexcessoquantidade<?=$dados['pliselid'];?>" value="<?=$dados['plisuplementarexcessoquantidade'];?>">
                        </td>
                    </tr>
                </table>
            </td>
            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                    <tr>
                        <td style="border: none;">
                            <input type="text" size="10" class="input_valor" name="vlsuplementarsuperavitcusteio" tipo="custeio" pliselid="<?=$dados['pliselid'];?>" id="vlsuplementarsuperavitcusteio<?=$dados['pliselid'];?>" value="<?=$dados['vlsuplementarsuperavitcusteio'];?>">
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <input type="text" size="10" class="input_valor" name="vlsuplementarsuperavitcusteio" tipo="capital" pliselid="<?=$dados['pliselid'];?>" id="vlsuplementarsuperavitcapital<?=$dados['pliselid'];?>" value="<?=$dados['vlsuplementarsuperavitcapital'];?>">
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <input type="text" size="10" class="input_valor" name="plisuplementarsuperavitquantidade" tipo="fisico" pliselid="<?=$dados['pliselid'];?>" id="plisuplementarsuperavitquantidade<?=$dados['pliselid'];?>" value="<?=$dados['plisuplementarsuperavitquantidade'];?>">
                        </td>
                    </tr>
                </table>
            </td>
            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                    <tr>
                        <td style="border: none;">
                            <input type="text" size="10" class="input_valor" name="vldotacaocusteio" tipo="custeio" pliselid="<?=$dados['pliselid'];?>" id="vldotacaocusteio<?=$dados['pliselid'];?>" value="<?=$dados['vldotacaocusteio'];?>">
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <input type="text" size="10" class="input_valor" name="vldotacaocapital" tipo="capital" pliselid="<?=$dados['pliselid'];?>" id="vldotacaocapital<?=$dados['pliselid'];?>" value="<?=$dados['vldotacaocapital'];?>">
                        </td>
                    </tr>
                    <tr>
                        <?php
                        if ($dados['vlrprovavel']<0){
                            $color='red';
                        }else{
                            $color='';
                        }
                        ?>
                        <td style="color: <?=$color;?>; border: none; border-top: 1px solid #e7eaec;" id="fisico<?=$dados['pliselid'];?>">
                            <?=$dados['vlrprovavel'];?>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
    <?php endforeach ?>
    </tbody>
</table>
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
            var vlrCancelado = $("#"+nomeCampo+pliselid).val() ? str_replace(['.', ','], ['', '.'], $("#"+nomeCampo+pliselid).val()) : 0;
            var vlrEmpenhado = $("#empenhado"+pliselid).val() ? str_replace(['.', ','], ['', '.'], $("#empenhado"+pliselid).val()) : 0;
            if (parseFloat(vlrCancelado)>parseFloat(vlrEmpenhado)){
                swal({
                    title: "",
                    text: "Valor cancelado maior que o do PI",
                    type: "warning",
                    confirmButtonText: 'Ok',
                    closeOnConfirm: true
                },
                function(isConfirm){
                    if(isConfirm){
                        var url = '?modulo=principal/cadastro_alteracoes&acao=C&req=carrega_lista_pis_selecionados&pedid='+$("#pedid").val();
                        $('#listaPisSelecionados').load(url);
                    }
               });
               return;
            }
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