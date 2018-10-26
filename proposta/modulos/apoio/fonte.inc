<?php

$listaFonte = (new Public_Model_FonteRecurso())->recuperarTodos('*', ["fonstatus = 'A' "], 'fondsc');

    include APPRAIZ . "includes/cabecalho.inc";

?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
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

                    <a class="btn btn-small btn-warning" href="?modulo=apoio/fonte_form&acao=A">Novo</a>

                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                            <tr class="text-center">
                                <th>Ação</th>
                                <th>Código</th>
                                <th>Descrição</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php
                            if($listaFonte){
                            foreach($listaFonte as $dados){
                                ?>
                                <tr>
                                    <td class="text-center">
                                        <a title="Alterar" href="?modulo=apoio/fonte_form&acao=A&fonid=<?= $dados['fonid']; ?>"><i class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></i></a>
                                    </td>
                                    <td><?php echo $dados['foncod']; ?></td>
                                    <td><?php echo $dados['fondsc']; ?></td>
                                </tr>
                            <?php } } ?>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
