<?php

$aJanela = (new Alteracao_Model_Janela)->recuperarTodos('*' , ["prsano = '{$_SESSION['exercicio']}'", "janstatus = 'A'"],'jannome');

include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-12">
        <h2><?= $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">

                    <a class="btn-small btn btn-success" href="?modulo=apoio/janela_form&acao=A">
                        <span class="glyphicon glyphicon-plus"></span>
                        Novo
                    </a>

                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                            <tr class="text-center">
                                <th width="5%">Ação</th>
                                <th>Janela</th>
                                <th>Início</th>
                                <th>Fim</th>
                                <th>Ano</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($aJanela as $dados): ?>
                                <tr>
                                    <td class="text-center" nowrap="nowrap">
                                        <a href="?modulo=apoio/janela_form&acao=A&janid=<?= $dados['janid']; ?>" title="Editar" >
                                            <span class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></span>
                                        </a>
                                    </td>
                                    <td><?= $dados['jannome']; ?></td>
                                    <td><?= formata_data($dados['janinicio']); ?></td>
                                    <td><?= formata_data($dados['janfim']); ?></td>
                                    <td><?= $dados['prsano']; ?></td>
                                </tr>
                            <?php endforeach; ?>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
