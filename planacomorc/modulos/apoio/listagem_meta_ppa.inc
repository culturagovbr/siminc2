<table class="table table-striped table-bordered table-hover dataTablesPg5" >
    <thead>
    <tr class="text-center">
        <th width="10%">Ações</th>
        <th>Código</th>
        <th>Meta</th>
        <th>Quantificavel</th>
        <th>Quantidade</th>
    </tr>
    </thead>
    <tbody>
    <?php
        if($arrMetappa):
            foreach($arrMetappa as $meta): ?>
            <tr>
                <td class="text-center">
                    <a title="Alterar" href="javascript:alterarCamposMeta(<?= $meta['mppid'] ?>,'<?= $_REQUEST['oppid'] ?>');"><i class="fa fa-pencil"></i></a>
                </td>
                <td><?= $meta['mppcod']; ?></td>
                <td><?= $meta['mppnome']; ?></td>
                <td><?= $meta['mppquantificavel'] == 't'? 'Sim': 'Não'; ?></td>
                <td><?= $meta['mppquantidade']; ?></td>
            </tr>
        <?php
            endforeach;
        endif; ?>
    </tbody>
</table>