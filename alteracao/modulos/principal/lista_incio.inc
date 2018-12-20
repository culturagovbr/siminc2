<?php
    $aAlteracao = (new Alteracao_Model_Pedido())->recuperarListagem($filtroalteracao);
?>
<table class="table table-striped table-bordered table-hover dataTables" id="table-principal">
    <thead>
    <tr class="text-center">
        <th width="4%">Ações</th>
        <th>Nº do Pedido</th>
        <th>Tipo de Cr&eacute;dito</th>
        <th>Janela</th>
        <th>Situa&ccedil;&atilde;o</th>
        <th>Unidade</th>
        <th>Cancelado</th>
        <th>Suplementado</th>
        <th width="20%">T&iacute;tulo</th>
        <th>Cadastrado em</th>
    </tr>
    </thead>
    <tbody>
    <?php
    foreach($aAlteracao as $dados): ?>
        <tr>
            <td class="text-center" nowrap="nowrap">
                <?php if ($podeEditarAlteracao === TRUE): ?>
                    <a href="?modulo=principal/cadastro_alteracoes&acao=C&pedid=<?=$dados['pedid'];?>&tpdid=<?=$tpdid;?>" title="Editar"><span class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></span></a>
                    <a href="?modulo=inicio&acao=C&pedid=<?=$dados['pedid'];?>&requisicao=excluir_pedido" title="Apagar"><span class="btn btn-danger btn-sm glyphicon glyphicon-trash"></span></a>
                <?php endif; ?>
                    <a href="#" class="a_espelho_pedido" title="Visualizar Pedido de Alteração" data-pedid="<?=$dados['pedid'];?>"><span class="btn btn-success btn-sm fa fa-search"></span></a>
            </td>
            <td><?= $dados['pedid']; ?></td>
            <td><?= $dados['tpacod']; ?></td>
            <td><?= $dados['jannome']; ?></td>
            <td><?= $dados['esddsc']; ?></td>
            <td><?= $dados['lista_subunidades']; ?></td>
            <td><?= number_format($dados['cancelado'], 0, ',', '.'); ?></td>
            <td><?= number_format($dados['suplementado'], 0, ',', '.'); ?></td>
            <td><?= $dados['pedtitulo']; ?></td>
            <td><?= $dados['dtalteracao'] .' às '. $dados['hralteracao']; ?></td>
        </tr>
    <?php endforeach ?>
    </tbody>
</table>

<script>
    $('body').on('click', '.a_espelho_pedido', function(){
        var pedid = $(this).attr('data-pedid');
        exibirEspelhoPedido(pedid);
        return false;
    });

    /**
     * Exibe popup com Detalhes do Pedido. Tela de Espelho de Pedido.
     *
     * @returns VOID
     */
    function exibirEspelhoPedido(pedid){
        window.open(
            window.location.href + '&req=espelho-pedido&pedid='+ pedid,
            'popup_espelho_pedido',
            'width=980,height=1000,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
    }
</script>