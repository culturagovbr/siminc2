<?php

    $listaTipo = (new Alteracao_Model_Tipo())->recuperarTodos();

    include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
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

                    <a class="btn btn-small btn-warning" href="?modulo=apoio/tipo_form&acao=A">Novo</a>

                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                            <tr class="text-center">
                                <th width="10%">Ação</th>
                                <th>Código</th>
                                <th>Descrição</th>
                                <th>Fluxo de trabalho</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php
                            if($listaTipo) :
                                foreach($listaTipo as $dados) :
                                    ?>
                                    <tr>
                                        <td class="text-center" nowrap="nowrap">
                                            <a href="?modulo=apoio/tipo_form&acao=A&tpaid=<?= $dados['tpaid']; ?>" title="Editar" >
                                                <span class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></span>
                                            </a>
                                            <a href="?modulo=apoio/tipo_form&acao=A&req=excluir-tipo&tpaid=<?= $dados['tpaid']; ?>" title="Apagar">
                                                <span class="btn btn-danger btn-sm glyphicon glyphicon-trash"></span>
                                            </a>
                                        </td>
                                        <td><?= $dados['tpacod']; ?></td>
                                        <td><?= $dados['tpadsc']; ?></td>
                                        <td><?= $dados['tpafluxo']; ?></td>
                                    </tr>
                                <?php endforeach;
                            endif ?>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
