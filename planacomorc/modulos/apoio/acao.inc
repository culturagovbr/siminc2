<?php

$listaAcoes = (new Planacomorc_Model_Acao)->recuperarDados();

include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-12">
        <h2><?php echo $titulo_modulo; ?></h2>
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
                    <a class="btn-small btn btn-success" href="?modulo=apoio/acao_form&acao=A">
                        <span class="glyphicon glyphicon-plus"></span>
                        Novo
                    </a>
                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                            <tr class="text-center">
                                <th style="width 5%">Ação</th>
                                <th>Cod. UO</th>
                                <th>Função</th>
                                <th>Subfunção</th>
                                <th>Programa</th>
                                <th>Ação</th>
                                <th>Localizador</th>
                                <th>Objetivo</th>
                                <th>Unidade Orçamentária</th>
                                <th>Nome</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php
                                if($listaAcoes):
                                    foreach($listaAcoes as $acao): ?>
                                        <tr>
                                            <td class="text-center">
                                                <a title="Alterar" href="?modulo=apoio/acao_form&acao=A&acaid=<?php echo $acao['acaid']; ?>"><i class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></i></a>
                                            </td>
                                            <td><?php echo $acao['unicod']; ?></td>
                                            <td><?php echo $acao['funcod']; ?></td>
                                            <td><?php echo $acao['sfucod']; ?></td>
                                            <td><?php echo $acao['prgcod']; ?></td>
                                            <td><?php echo $acao['acacod']; ?></td>
                                            <td><?php echo $acao['loccod']; ?></td>
                                            <td><?php echo $acao['acaobjetivocod']? $acao['acaobjetivocod']: '-'; ?></td>
                                            <td><?php echo $acao['unidade']; ?></td>
                                            <td><?php echo $acao['acatitulo']; ?></td>
                                        </tr>
                                    <?php endforeach; ?>
                                <?php endif; ?>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
