<?php

$listaTipo = (new Acompanhamento_Model_Tipo())->recuperarTodos( '*', ["tipstatus ='A'"], 'tipdsc');

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
                        <table class="table table-striped table-bordered table-hover dataTables">
                            <thead>
                            <tr class="text-center">
                                <th style=" width: 10%">Ações</th>
                                <th>Descrição</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php if ($listaTipo):
                                foreach ($listaTipo as $dados): ?>
                                    <tr>
                                        <td class="text-center">
                                            <a title="Alterar" href="?modulo=apoio/tipo_form&acao=A&tipid=<?= $dados['tipid']; ?>"><i class="fa fa-pencil"></i></a>
                                        </td>
                                        <td><?= $dados['tipdsc'] ?></td>
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