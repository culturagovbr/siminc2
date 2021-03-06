<div class="ibox-content">
    <div class="table-responsive">
        <table id="tbListaPis" class="table table-striped table-bordered table-hover dataTables" >
            <thead>
                <tr class="text-center">
                    <th width="5%">A��es</th>
                    <th width="25%">Unidade</th>
                    <th width="5%">Id</th>
                    <th width="5%">Funcional</th>
                    <th width="5%">C�digo</th>
                    <th width="25%">T�tulo</th>
                    <th width="15%">Custeio</th>
                    <th width="15%">Capital</th>
                </tr>
            </thead>
            <tbody>
            <?php
            foreach($aDadosPi as $dados): ?>
                <tr>
                    <td align="center">
                        <input type="checkbox" <?= $dados['pliselid']>0?'checked="true" title="Excluir PI"':'title="Selecionar PI"'; ?> class="check checkbox-success link" id="chk<?=$dados['id'];?>" esdid="<?=$dados['esdid'];?>" pliselid="<?=$dados["pliselid"];?>" value="<?=$dados['id'];?>" name="chkPliid" />
                    </td>
                    <td align="left">
                        <?= $dados['sub_unidade']; ?>
                    </td>
                    <td align="center">
                        <a href="#" title="Exibir detalhes do Plano Interno(Espelho)" class="a_espelho" data-pi="<?=$dados['id']?>"><?= $dados['id']; ?></a>
                    </td>
                    <td align="center">
                        <a href="#" title="Exibir detalhes do Plano Interno(Espelho)" class="a_espelho" data-pi="<?=$dados['id']?>"><?= $dados['funcional']; ?></a>
                    </td>
                    <td align="center">
                        <a href="#" title="Exibir detalhes do Plano Interno(Espelho)" class="a_espelho" data-pi="<?=$dados['id']?>"><?= $dados['plicod']; ?></a>
                    </td>
                    <td align="left">
                        <a href="#" title="Exibir detalhes do Plano Interno(Espelho)" class="a_espelho" data-pi="<?=$dados['id']?>"><?= $dados['plititulo']; ?></a>
                    </td>
                    <td align="right">R$ <?= number_format($dados['custeio'],0,',','.'); ?></td>
                    <td align="right">R$ <?= number_format($dados['capital'],0,',','.'); ?></td>
                </tr>
            <?php endforeach ?>
            </tbody>
        </table>
    </div>
</div>
<!--In�cio tratamento datatables dinamicamente-->
<script>
    $(document).ready(function(){
        $("#tbListaPis").dataTable({
            bPaginate: false,
            responsive: true,
            paging: true,
            pageLength: 25,
            "language": {
                "url": "/zimec/public/temas/simec/js/plugins/dataTables/Portuguese-Brasil.json"
            }
        });

        $(".check").click(function(){
            if ($(this).attr('esdid')==1769 || $(this).attr('esdid')==1774){
                swal('', 'PI Em Cadastramento!\n N�o pode ser selecionado!', 'warning');
                return false;
            }
            if ($(this).prop("checked")){
                adicionaPiPedido($(this).attr('value'), $("#pedid").val());
            }else{
                removePiPedido($("#pedid").val(), $(this).attr('pliselid'));
            }
        });
    });
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