<?php
$aExercicio = (new Spo_Model_Programacaoexercicio($_SESSION['sisdiretorio']))->buscarTodos();

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
                    <a class="btn-small btn btn-success" href="?modulo=sistema/exercicio/admexerc_form&acao=A">
                        <span class="glyphicon glyphicon-plus"></span>
                        Novo
                    </a>
                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                            <tr class="text-center">
                                <th width="5%">Ação</th>
                                <th>Ano</th>
                                <th>Início</th>
                                <th>Fim</th>
                                <th>Corrente</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($aExercicio as $exercicio): ?>
                                <tr>
                                    <td class="text-center">
                                        <a title="Alterar" href="?modulo=sistema/exercicio/admexerc_form&acao=A&prsano=<?= $exercicio['prsano']; ?>">
                                            <i class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></i>
                                        </a>
                                    </td>
                                    <td><?= $exercicio['prsano']; ?></td>
                                    <td><?= formata_data($exercicio['prsdata_inicial']); ?></td>
                                    <td><?= formata_data($exercicio['prsdata_termino']); ?></td>
                                    <td><?= $exercicio['prsexerccorrente']; ?></td>
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