<div class="ibox-content">
    <div class="table-responsive">
        <table id="tbListaPis" class="table table-striped table-bordered table-hover dataTables" >
            <thead>
                <tr class="text-center">
                    <th width="5%">Ações</th>
                    <th width="5%">Id Planejamento</th>
                    <th width="5%">Id Planejamento</th>
                    <th width="5%">Funcional</th>
                    <th width="5%">Número do PI</th>
                    <th width="50%">Título</th>
                    <th width="15%">Custeio</th>
                    <th width="15%">Capital</th>
                </tr>
            </thead>
            <tbody>
            <?php
            foreach($aDadosPi as $dados): ?>
                <tr>
                    <td align="center"><input type="checkbox" <?= $dados['pliselid']>0?'checked="true"':''; ?> class="check checkbox-success" id="chk<?=$dados['id'];?>" esdid="<?=$dados['esdid'];?>" pliselid="<?=$dados["pliselid"];?>" value="<?=$dados['id'];?>" name="chkPliid"></td>
                    <td align="center"><?= $dados['id']; ?></td>
                    <td align="center"><?= $dados['esddsc']; ?></td>
                    <td align="center"><?= $dados['funcional']; ?></td>
                    <td align="center"><?= $dados['plicod']; ?></td>
                    <td align="left"><?= $dados['plititulo']; ?></td>
                    <td align="right">R$ <?= number_format($dados['custeio'],2,',','.'); ?></td>
                    <td align="right">R$ <?= number_format($dados['capital'],2,',','.'); ?></td>
                </tr>
            <?php endforeach ?>
            </tbody>
        </table>
    </div>
</div>
<!--Início tratamento datatables dinamicamente-->
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
                swal('', 'PI Em Cadastramento!\n Não pode ser selecionado!', 'warning');
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