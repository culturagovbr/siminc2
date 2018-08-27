<?php if(count($aHistorico)){ ?>
    <div class="table-responsive">
        <table class="table table-striped table-bordered table-hover bedidTables" >
            <thead>
            <tr class="text-center">
                <th>Data</th>
                <th>Usuário</th>
                <th>E-mail</th>
                <th>Telefone</th>
            </tr>
            </thead>
            <tbody>
            <?php foreach($aHistorico as $dados):?>
                <tr>
                    <td><?= $dados['hisdata']; ?></td>
                    <td><?= $dados['usunome']; ?></td>
                    <td><?= $dados['usuemail']; ?></td>
                    <td><?= $dados['telefone']; ?></td>
                </tr>
            <?php endforeach; ?>
            </tbody>
        </table>
    </div>
<?php } else {
    echo '<div class="alert alert-danger">Nenhum histórico vinculado</div>';
} ?>