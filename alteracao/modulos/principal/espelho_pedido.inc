<?php
    $mAlteracaoPedido = new Alteracao_Model_Pedido($_REQUEST['pedid']);
    $arrPedidos = $mAlteracaoPedido->recuperarPedidoPorId($_REQUEST['pedid']);
?>

<h2 style="color: #1AB394">1. Dados do Pedido
    <?php if ($podeEditarAlteracao === TRUE): ?>
        <a href="#" id="btnPedido" title="Editar">
            <span class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></span>
        </a>
    <?php endif; ?>
</h2>
<div class="ibox-content">
    <div class="row">
        <div class="col-lg-6 b-r">
            <div class="form-group row">
                <?php
                    $podeEditar = FALSE;
                    $simec->setPodeEditar($podeEditar);
                    echo $simec->input('pedid', 'Nº Pedido', $arrPedidos['pedid'], null, ['input-size' => 9, 'label-size' => 2]);
                    echo $simec->input('tpaid', 'Tipo de Crédito', $arrPedidos['tpacod'].' - '.$arrPedidos['tpadsc'], null, ['input-size' => 9, 'label-size' => 2]);
                    echo $simec->select('suoid', 'Unidade', $arrSuoid, (new Public_Model_SubUnidadeOrcamentaria())->queryComboChaveSuoid((object)array('exercicio' => $_SESSION['exercicio'], 'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)), null, ['input-size' => 9, 'label-size' => 2]);
                    echo $simec->input('jannome', 'Janela', $arrPedidos['jannome'], null, ['input-size' => 9, 'label-size' => 2]);
                    echo $simec->input('pedtitulo', 'Título', $arrPedidos['pedtitulo'], null, ['input-size' => 9, 'label-size' => 2]);
                ?>
            </div>
        </div>
        <div class="col-lg-6">
            <h4>Cadastrado Por:</h4>
            <hr class="hr-line-solid">
            <?php
                echo $simec->input('usunome', 'Nome', $arrPedidos['usunome'], null, ['input-size' => 9, 'label-size' => 2]);
                echo $simec->input('usufone', 'Telefone', $arrPedidos['usufone'], null, ['input-size' => 9, 'label-size' => 2]);
                echo $simec->input('usuemail', 'E-mail', $arrPedidos['usuemail'], null, ['input-size' => 9, 'label-size' => 2]);
            ?>
        </div>
    </div>
</div>
<script>
$(document).ready(function(){
    $("#btnPedido").click(function(){
        $("#wizard-t-0").click();
    });
});
</script>