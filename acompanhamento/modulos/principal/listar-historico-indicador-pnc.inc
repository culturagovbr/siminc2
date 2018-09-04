<?php

    $idpid = $_REQUEST['idpid'];
    $modelHistoricoIndicadorPnc = new Acompanhamento_Model_Historicoindicadorpnc();
    $modelHistoricoIndicadorPnc->idpid = $idpid;
    $listaHistorico = $modelHistoricoIndicadorPnc->buscarPorIndicador();

?>

<div class="row">
    <div class="col-md-12">
        <div class="ibox float-e-margins">
            <div class="table-responsive">
                <?php if ($listaHistorico): ?>
                    <table class="table table-bordered table-hover">
                        <thead>
                            <tr class="text-center">
                                <th>Data</th>
                                <th>Usuário</th>
                                <th>E-mail</th>
                                <th>Telefone</th>
                                <th>Quantidade Anterior</th>
                                <th>Quantidade Nova</th>
                            </tr>
                        </thead>
                        <tbody>
                        <?php foreach ($listaHistorico as $dados) { ?>
                            <tr>
                                <td><?= $dados['hipdata'] ?></td>
                                <td><?= $dados['usunome'] ?></td>
                                <td><?= $dados['usuemail'] ?></td>
                                <td><?= ($dados['usufoneddd']?'('. $dados['usufoneddd']. ') ': NULL). $dados['usufonenum'] ?></td>
                                <td><?= $dados['hipquantidadeantigo'] ?></td>
                                <td><?= $dados['hipquantidadenovo'] ?></td>
                            </tr>
                        <?php } ?>
                        </tbody>
                    </table>
                <?php else : ?>
                    <div class="alert alert-danger">Não há Histórico de alterações nesse Indicador PNC</div>
                <?php endif; ?>
            </div>
        </div>
    </div>
</div>