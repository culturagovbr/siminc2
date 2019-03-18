<?php

$listaHistoricoUsuario = (new Planacomorc_Model_HistoricoPiUsuario())->recuperarListagem($_REQUEST['pliid']);

?>

<div class="row">
    <div class="col-md-12">
        <div class="ibox float-e-margins">
            <div class="table-responsive">
                <?php if ($listaHistoricoUsuario) { ?>
                    <table class="table table-bordered table-hover">
                        <thead>
                        <tr class="text-center">
                            <th>Data</th>
                            <th>Usuário</th>
                            <th>Email</th>
                            <th>Telefone</th>
                        </tr>
                        </thead>
                        <tbody>
                        <?php foreach ($listaHistoricoUsuario as $dados) { ?>
                            <tr>
                                <td><?= $dados['hpudata'] ?></td>
                                <td><?= $dados['usunome'] ?></td>
                                <td><?= $dados['usuemail'] ?></td>
                                <td><?= $dados['telefone'] ?></td>
                            </tr>
                        <?php } ?>
                        </tbody>
                    </table>
                <?php } else { ?>
                    <div class="alert alert-danger">Não há Histórico de alterações nesse PA</div>
                <?php } ?>
            </div>
        </div>
    </div>
</div>