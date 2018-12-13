<?php
    $disabled = 'disabled';
?>
<h2 style="color: #1AB394">1. Dados do Pedido
    <a href="javascript:window.location.href='?modulo=principal/cadastro_alteracoes&acao=C&pedid=<?=$_REQUEST['pedid']?>&tpdid=<?=$_REQUEST['tpdid']?>'" tittle="Editar">
        <span class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></span>
    </a>
</h2>

<div class="ibox-content">
    <form id="filtroalteracao" name="filtroalteracao" method="POST" role="form" action="?modulo=principal/cadastro_alteracoes&acao=C" class="form-horizontal">
        <input name="req" id="req" value="" type="hidden">
        <div class="row">
            <div class="col-lg-8">
                <?php
                $suoid = array();
                if ($_REQUEST['pedid']){
                    $suoid = (new Alteracao_Model_PedidoUnidade())->carregarPorPedido($_REQUEST['pedid']);
                    foreach($suoid as $value){
                        $arrSuoid[]=$value['suoid'];
                    }
                }
                echo $simec->select('suoid[]', 'Unidade', $arrSuoid, (new Public_Model_SubUnidadeOrcamentaria())->queryComboChaveSuoid((object)array('exercicio' => $_SESSION['exercicio'], 'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)), ['', $disabled], ['input-size' => 9, 'label-size' => 2]);
                ?>
            </div>
            <div class="col-lg-3">
                <?= $simec->select('tpaid', 'Tipo', $mAlteracaoPedido->tpaid, (new Alteracao_Model_Tipo())->recuperarSqlCombo('tpacod'), ['', $disabled],  ['input-size' => 8, 'label-size' => 3]); ?>
            </div>
        </div>

        <div class="row">
            <div class="col-lg-8">
                <?= $simec->input('pedtitulo',  'Título', $mAlteracaoPedido->pedtitulo, ['', $disabled], ['input-size' => 9, 'label-size' => 2]); ?>
            </div>
            <div class="col-lg-3">
                <?= $simec->select('janid', 'Janela', $mAlteracaoPedido->janid, (new Alteracao_Model_Janela())->recuperarSqlCombo(), ['', $disabled],  ['input-size' => 8, 'label-size' => 3]); ?>
            </div>
        </div>
    </form>
</div>