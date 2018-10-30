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
                                    <th width="10%">Ações</th>
                                    <th>Unidade Orçamentária</th>
                                    <th>Cód. Ação</th>
                                    <th>Ação</th>
                                    <th>PTRES</th>
                                    <th>funcional</th>
                                    <th>Cod. PO</th>
                                    <th>PO</th>
                                    <th>RP</th>
                                </tr>
                            </thead>
                            <tbody>
                            <?php foreach($aPtres as $dados){ ?>
                                <tr>
                                    <td class="text-center">
                                        <a title="Alterar" href="?modulo=apoio/funcionais_form&acao=A&ptrid=<?php echo $dados['ptrid']; ?>"><i class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></i></a>
                                    </td>
                                    <td><?php echo $dados['unidade_orcamentaria']; ?></td>
                                    <td><?php echo $dados['acacod']; ?></td>
                                    <td><?php echo $dados['acatitulo']; ?></td>
                                    <td><?php echo $dados['ptres']; ?></td>
                                    <td><?php echo $dados['funcional']; ?></td>
                                    <td><?php echo $dados['plocod']; ?></td>
                                    <td><?php echo $dados['plodsc']; ?></td>
                                    <td><?php echo $dados['irpcod']; ?></td>
                                </tr>
                            <?php } ?>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>