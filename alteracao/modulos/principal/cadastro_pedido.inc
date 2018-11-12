<div class="ibox-content">
    <form id="filtroalteracao" name="filtroalteracao" method="POST" role="form" action="?modulo=principal/cadastro_alteracoes&acao=C" class="form-horizontal">
        <input name="req" id="req" value="" type="hidden">
        <div class="row">
            <div class="col-lg-8">
                <?= $simec->select('suoid', 'Unidade', $mAlteracaoPedido->suoid, (new Public_Model_SubUnidadeOrcamentaria())->queryComboChaveSuoid((object)array('exercicio' => $_SESSION['exercicio'], 'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)), ['required'], ['input-size' => 9, 'label-size' => 2]); ?>
            </div>
            <div class="col-lg-3">
                <?= $simec->select('tpaid', 'Tipo', $mAlteracaoPedido->tpaid, (new Alteracao_Model_Tipo())->recuperarSqlCombo('tpacod'), ['required'],  ['input-size' => 8, 'label-size' => 3]); ?>
            </div>
        </div>

        <div class="row">
            <div class="col-lg-8">
                <?= $simec->input('pedtitulo',  'Título', $mAlteracaoPedido->pedtitulo, ['required'], ['input-size' => 9, 'label-size' => 2]); ?>
            </div>
            <div class="col-lg-3">
                <?= $simec->select('janid', 'Janela', $mAlteracaoPedido->janid, (new Alteracao_Model_Janela())->recuperarSqlCombo(), ['required'],  ['input-size' => 8, 'label-size' => 3]); ?>
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
    $(function(){
        $("#btnSalvarPedido").click(function(){
            salvarPedido(true);
        });
    });

    function salvarPedido(showAlert){
        $.post('?modulo=principal/cadastro_alteracoes&acao=C',
            {
                req: 'salvar_pedido',
                pedid: $("#pedid").val(),
                suoid: $("#suoid").val(),
                pedtitulo: $("#pedtitulo").val(),
                tpaid: $("#tpaid").val(),
                janid: $("#janid").val(),
                tpdid: $("#tpdid").val()
            }, function (result) {
                result = JSON.parse(result);
                if (result['result']) {
                    $("#pedid").val(result['pedid']);
                    $("#divDadosCredito").load('?modulo=principal/cadastro_alteracoes&acao=C&req=carrega_dados_credito&pedid='+result['pedid']+'&tpdid=<?=$_REQUEST['tpdid'];?>');
                    if (showAlert) {
                        swal({
                            title: "",
                            text: "Registro salvo com sucesso!",
                            type: "success",
                            confirmButtonText: 'OK'
                        });
                    }
                } else {
                    swal('', 'Erro ao salvar registro!', 'error');
                }
            });
    }

    function verificaCampos(){
        var retorno = true;
        $("#filtroalteracao input,select").each(function(){
            if ($(this).attr('required') && $(this).val()==''){
                retorno = false;
            }
        });
        return retorno;
    }
</script>