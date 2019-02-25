<?php

    $aPtres = (new Monitora_Model_Ptres)->recuperarTodosApoio((object)array('exercicio' => $_SESSION['exercicio']));

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
                    <a class="btn-small btn btn-success" href="?modulo=apoio/funcionais_form&acao=A">
                        <span class="glyphicon glyphicon-plus"></span>
                        Novo
                    </a>
                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                                <tr class="text-center">
                                    <th width="5%">Ação</th>
                                    <th>PTRES</th>
                                    <th>Unidade Orçamentária</th>
                                    <th>Função</th>
                                    <th>Subfunção</th>
                                    <th>funcional</th>
                                    <th>Ação</th>
                                    <th>RP</th>
                                </tr>
                            </thead>
                            <tbody>
                            <?php
                                if($aPtres):
                                    foreach($aPtres as $funcional): ?>
                                        <tr>
                                            <td class="text-center">
                                                <a title="Alterar" href="?modulo=apoio/funcionais_form&acao=A&ptrid=<?= $funcional['ptrid']; ?>"><i class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></i></a>
                                            </td>
                                            <td><?= $funcional['ptres']; ?></td>
                                            <td><?= $funcional['unonome']; ?></td>
                                            <td><?= $funcional['funcod']; ?></td>
                                            <td><?= $funcional['sfucod']; ?></td>
                                            <td><?= $funcional['funcional']; ?></td>
                                            <td><?= $funcional['acatitulo']; ?></td>
                                            <td><?= $funcional['irpcod']; ?></td>
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