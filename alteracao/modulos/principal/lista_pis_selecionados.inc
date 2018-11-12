<table id="tbListaPisSelecionados" class="table table-striped table-bordered table-hover dataTables" >
    <thead>
        <tr class="text-center">
            <th>Ações</th>
            <th>Funcional</th>
            <th>Id Planejamento</th>
            <th>Título</th>
            <th width="11%">Tipo</th>
            <th>Saldo a Empenhar</th>
            <th>Suplementar</th>
            <th>Cancelar</th>
            <th>Suplementar por Excesso</th>
            <th>Suplementar por Superávit</th>
            <th>Dotação Provável</th>
        </tr>
    </thead>
    <tbody>
    <?php
    foreach($aDadosPiSelecionados as $dados): ?>
        <tr>
            <td style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;" align="center">
                <button class="btn btn-danger btnRemovePi" type="button" pliselid="<?=$dados['pliselid'];?>" name="btnSelecionarPi">
                    <span class="glyphicon glyphicon-trash"></span>
            </td>
            <td ><?= $dados['funcional']; ?></td>
            <td ><?= $dados['pliid']; ?></td>
            <td ><?= $dados['plititulo']; ?></td>
            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                    <tr>
                        <td style="border: none;">Custeio: R$ <?= number_format($dados['custeio'],2,',','.'); ?></td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">Capital: R$ <?= number_format($dados['capital'],2,',','.'); ?></td>
                    </tr>
                </table>
            </td>
            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                    <tr>
                        <td style="border: none;">
                            <input type="text" size="10" class="input_valor salvar-dotacao-fnc" name="vlsaldoempenhocusteio" pliselid="<?=$dados['pliselid'];?>" id="vlsaldoempenharcusteio<?=$dados['pliid'];?>" value="<?=$dados['vlsaldoempenhocusteio'];?>">
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <input type="text" size="10" class="input_valor" name="vlsaldoempenhocapital" pliselid="<?=$dados['pliselid'];?>" id="vlsaldoempenhocapital<?=$dados['pliid'];?>" value="<?=$dados['vlsaldoempenhocapital'];?>">
                        </td>
                    </tr>
                </table>
            </td>
            
            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                    <tr>
                        <td style="border: none;">
                            <input type="text" size="10" class="input_valor" name="vlsuplementarcusteio" pliselid="<?=$dados['pliselid'];?>" id="vlsuplementarcusteio<?=$dados['pliid'];?>" value="<?=$dados['vlsuplementarcusteio'];?>">
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <input type="text" size="10" class="input_valor" name="vlsuplementarcapital" pliselid="<?=$dados['pliselid'];?>" id="vlsuplementarcapital<?=$dados['pliid'];?>" value="<?=$dados['vlsuplementarcapital'];?>">
                        </td>
                    </tr>
                </table>
            </td>
            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                    <tr>
                        <td style="border: none;">
                            <input type="text" size="10" class="input_valor" name="vlcancelarcusteio" pliselid="<?=$dados['pliselid'];?>" id="vlcancelarcusteio<?=$dados['pliid'];?>" value="<?=$dados['vlcancelarcusteio'];?>">
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <input type="text" size="10" class="input_valor" name="vlcancelarcapital" pliselid="<?=$dados['pliselid'];?>" id="vlcancelarcapital<?=$dados['pliid'];?>" value="<?=$dados['vlcancelarcapital'];?>">
                        </td>
                    </tr>
                </table>
            </td>
            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                    <tr>
                        <td style="border: none;">
                            <input type="text" size="10" class="input_valor" name="vlsuplementarexcessocusteio" pliselid="<?=$dados['pliselid'];?>" id="vlsuplementarexcessocusteio<?=$dados['pliid'];?>" value="<?=$dados['vlsuplementarexcessocusteio'];?>">
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <input type="text" size="10" class="input_valor" name="vlsuplementarexcessocapital" pliselid="<?=$dados['pliselid'];?>" id="vlsuplementarexcessocapital<?=$dados['pliid'];?>" value="<?=$dados['vlsuplementarexcessocapital'];?>">
                        </td>
                    </tr>
                </table>
            </td>
            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                    <tr>
                        <td style="border: none;">
                            <input type="text" size="10" class="input_valor" name="vlsuplementarsuperavitcusteio" pliselid="<?=$dados['pliselid'];?>" id="vlsuplementarsuperavitcusteio<?=$dados['pliid'];?>" value="<?=$dados['vlsuplementarsuperavitcusteio'];?>">
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <input type="text" size="10" class="input_valor" name="vlsuplementarsuperavitcusteio" pliselid="<?=$dados['pliselid'];?>" id="vlsuplementarsuperavitcapital<?=$dados['pliid'];?>" value="<?=$dados['vlsuplementarsuperavitcapital'];?>">
                        </td>
                    </tr>
                </table>
            </td>
            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                    <tr>
                        <td style="border: none;">
                            <input type="text" size="10" class="input_valor" name="vldotacaocusteio" pliselid="<?=$dados['pliselid'];?>" id="vldotacaocusteio<?=$dados['pliid'];?>" value="<?=$dados['vldotacaocusteio'];?>">
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <input type="text" size="10" class="input_valor" name="vldotacaocapital" pliselid="<?=$dados['pliselid'];?>" id="vldotacaocapital<?=$dados['pliid'];?>" value="<?=$dados['vldotacaocapital'];?>">
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
            salvarCampo($(this).prop('name'), $(this).attr('pliselid'), $(this).val());
        });
    });
    
    function salvarCampo(nomeCampo, pliselid, valor){
        $.post('?modulo=principal/cadastro_alteracoes&acao=C&req=salvar_campo',
        {
            pliselid: pliselid,
            campo: nomeCampo,
            valor: valor
        },
            function(retorno){
                if (retorno){
                    var url = '?modulo=principal/cadastro_alteracoes&acao=C&req=carrega_lista_pis_selecionados&pedid='+$("#pedid").val();
                    $('#listaPisSelecionados').load(url);
                }
            }
        );         
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