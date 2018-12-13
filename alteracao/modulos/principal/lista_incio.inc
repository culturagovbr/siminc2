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
                <a href="?modulo=principal/cadastro_alteracoes&acao=C&pedid=<?=$dados['pedid'];?>&tpdid=<?=$tpdid;?>" title="Editar"><span class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></span></a>
                <a href="?modulo=inicio&acao=C&pedid=<?=$dados['pedid'];?>&requisicao=excluir_pedido" title="Apagar"><span class="btn btn-danger btn-sm glyphicon glyphicon-trash"></span></a>
            </td>
            <td><?= $dados['pedid']; ?></td>
            <td><?= $dados['tpacod']; ?></td>
            <td><?= $dados['jannome']; ?></td>
            <td><?= $dados['esddsc']; ?></td>
            <td><?= $dados['suoid'].' - '.$dados['suonome']; ?></td>
            <td><?= $dados['cnc']; ?></td>
            <td><?= $dados['soap']; ?></td>
            <td><?= $dados['pedtitulo']; ?></td>
            <td><?= $dados['dtalteracao'] .' às '. $dados['hralteracao']; ?></td>
        </tr>
    <?php endforeach ?>
    </tbody>
</table>
