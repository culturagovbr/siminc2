<div class="ibox-content">
    <form id="justificativa" name="justificativa" method="POST" role="form" action="?modulo=principal/cadastro_alteracoes&acao=C" class="form-horizontal">
        <input name="req" id="req" value="" type="hidden">
        <input name="jtfid" id="jtfid" value="<?=$mAlteracaoPedido->jtfid;?>" type="hidden">

        <div class="row">
            <div class="col-lg-6">
                <?= $simec->textarea('x', 'Necessidade de Alteração', $mAcao->acanomeproduto, ['required'], ['cols'=>30, 'rows'=>3]); ?>
            </div>
            <div class="col-lg-6">
                <?= $simec->textarea('x', 'Formas de financiamento de crédito e adequação da proposta à meta fiscal e ao limite de gastos', $mAcao->acanomeproduto, ['required'], ['cols'=>30, 'rows'=>3]); ?>
            </div>
        </div>

        <div class="row">
            <div class="col-lg-6">
                <?= $simec->textarea('x', 'Verificação das fontes, dos identificadores de uso - iduso e dos identificadores de resultado primário - RP', $mAcao->acanomeproduto, ['required'], ['cols'=>30, 'rows'=>3]); ?>
            </div>
            <div class="col-lg-6">
                <?= $simec->textarea('x', 'Causa da Demanda', $mAcao->acanomeproduto, ['required'], ['cols'=>30, 'rows'=>3]); ?>
            </div>
        </div>

        <div class="row">
            <div class="col-lg-6">
                <?= $simec->textarea('x', 'Medida provisória', $mAcao->acanomeproduto, ['required'], ['cols'=>30, 'rows'=>3]); ?>
            </div>
            <div class="col-lg-6">
                <?= $simec->textarea('x', 'Legislação Específica', $mAcao->acanomeproduto, ['required'], ['cols'=>30, 'rows'=>3]); ?>
            </div>
        </div>

        <div class="row">
            <div class="col-lg-12">
                <?= $simec->textarea('x', 'Outras informações', $mAcao->acanomeproduto, ['required'], ['input-size' => 10, 'label-size' => 2, 'cols' => 30, 'rows' => 3]); ?>
            </div>
        </div>


        <div class="form-group">
            <div class="text-center">
                <button class="btn btn-primary btn-buscar" type="button" id="btnSalvarPedido">
                    <span class="glyphicon glyphicon-saved"></span> Salvar
                </button>
            </div>
        </div>

    </form>
</div>
<script>
    // $(function(){
    //     $("#btnSalvarPedido").click(function(){
    //         salvarPedido(true);
    //     });
    // });
    //
    // function salvarPedido(showAlert){
    //     $.post('?modulo=principal/cadastro_alteracoes&acao=C',
    //     {
    //         req: 'salvar_pedido',
    //         pedid: $("#pedid").val(),
    //         suoid: $("#suoid").val(),
    //         pedtitulo: $("#pedtitulo").val(),
    //         tpaid: $("#tpaid").val(),
    //         janid: $("#janid").val()
    //     },function(result){
    //         result = JSON.parse(result);
    //         if (result['result']){
    //             if (showAlert){
    //                 swal('','Registro salvo com sucesso!', 'success');
    //             }
    //         }else{
    //             swal('','Erro ao salvar registro!', 'error');
    //         }
    //     });
    // }
</script>