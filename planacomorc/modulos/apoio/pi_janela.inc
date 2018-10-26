<?php

$listaJanelas = (new Planacomorc_Model_PiJanela)->buscarTodos((object) array("prsano" => $_SESSION['exercicio']));
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
                    
                    <a class="btn btn-small btn-warning" href="?modulo=apoio/pi_janela_form&acao=A">Novo</a>
                    
                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                                <tr class="text-center">
                                    <th width="10%">Ações</th>
                                    <th>Reunião</th>
                                    <th>Início</th>
                                    <th>Fim</th>
                                    <th>Usuário</th>
                                </tr>
                            </thead>
                            <tbody>
                                <?php foreach($listaJanelas as $dados){ ?>
                                    <tr>
                                        <td class="text-center">
                                            <a title="Alterar" href="?modulo=apoio/pi_janela_form&acao=A&pijid=<?php echo $dados['pijid']; ?>">
                                                <i class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></i>
                                            </a>
                                        </td>
                                        <td><?php echo $dados['pijdsc']; ?></td>
                                        <td><?php echo $dados['pijinicio']; ?></td>
                                        <td><?php echo $dados['pijfim']; ?></td>
                                        <td><?php echo $dados['usunome']; ?></td>
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