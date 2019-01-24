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
                <?php if(!in_array($dados['esdid'], array(ESDID_ALTERACAO_EFETIVADO_INTERNO, ESDID_ALTERACAO_EFETIVADO_EXTERNO))): ?>
                    <a href="?modulo=principal/cadastro_alteracoes&acao=C&pedid=<?=$dados['pedid'];?>&tpdid=<?=$tpdid;?>" title="Editar"><span class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></span></a>
                    <a class="btn-excluir-pedido" data-pedid="<?= $dados['pedid']; ?>" title="Apagar"><span class="btn btn-danger btn-sm glyphicon glyphicon-trash"></span></a>
                <?php endif; ?>
                    <a href="#" class="a_espelho_pedido" title="Visualizar Pedido de Alteração" data-pedid="<?=$dados['pedid'];?>"><span class="btn btn-success btn-sm glyphicon glyphicon-search"></span></a>
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
    <?php endforeach; ?>
    </tbody>
</table>
