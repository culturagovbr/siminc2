<?php

$listaPrograma = (new Monitora_Model_Programa)->buscarTodos((object)array('exercicio' => $_SESSION['exercicio']));;

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
                    <a class="btn-small btn btn-success" href="?modulo=apoio/programa_form&acao=A">
                        <span class="glyphicon glyphicon-plus"></span>
                        Novo
                    </a>
                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                                <tr class="text-center">
                                    <th width="10%">Ações</th>
                                    <th>Código</th>
                                    <th>Descrição</th>
                                </tr>
                            </thead>
                            <tbody>
                            <?php if ($listaPrograma) {
                                foreach ($listaPrograma as $dados) { ?>
                                    <tr>
                                        <td class="text-center">
                                            <a title="Alterar"
                                               href="?modulo=apoio/programa_form&acao=A&prgid=<?= $dados['prgid']; ?>">
                                                <i class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></i>
                                            </a>
                                        </td>
                                        <td><?= $dados['prgcod']; ?></td>
                                        <td><?= $dados['prgdsc']; ?></td>
                                    </tr>
                                <?php }
                            } ?>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>