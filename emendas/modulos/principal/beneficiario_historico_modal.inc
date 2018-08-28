<?php

$listaHistorico = (new Emendas_Model_BeneficiarioHistorico())->recuperarAlteracoes($_REQUEST['benid']);

?>

<div class="row">
    <div class="col-md-12">
        <div class="ibox float-e-margins">
            <div class="table-responsive">
                <?php if ($listaHistorico) { ?>
                    <table class="table table-bordered table-hover">
                        <thead>
                        <tr class="text-center">
                            <th>Data</th>
                            <th>Usuário</th>
                            <th>E-mail</th>
                            <th>Telefone</th>
                        </tr>
                        </thead>
                        <tbody>
                        <?php foreach ($listaHistorico as $dados) : ?>
                            <tr>
                                <td><?= $dados['behdata'] ?></td>
                                <td><?= $dados['usunome'] ?></td>
                                <td><?= $dados['usuemail'] ?></td>
                                <td><?= $dados['telefone'] ?></td>
                            </tr>
                        <?php endforeach; ?>
                        </tbody>
                    </table>
                <?php } else { ?>
                    <div class="alert alert-danger">Não há Histórico de alterações para este Beneficiário</div>
                <?php } ?>
            </div>
        </div>
    </div>
</div>