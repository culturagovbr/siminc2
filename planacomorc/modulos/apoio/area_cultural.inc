<?php

$listaAreaCultural = (new Monitora_Model_PiModalidadeEnsino())->recuperarTodos('*', ["mdestatus = 'A'", "mdeano = '". $_SESSION['exercicio']. "'"], 'mdedsc');

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
                    <a class="btn-small btn btn-success" href="?modulo=apoio/area_cultural_form&acao=A">
                        <span class="glyphicon glyphicon-plus"></span>
                        Novo
                    </a>
                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                                
                            <tr class="text-center">
                                <th width="5%">A��o</th>
                                <th>C�digo PI</th>
                                <th>Descri��o</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php
                                if($listaAreaCultural):
                                    foreach($listaAreaCultural as $dados):?>
                                        <tr>
                                            <td class="text-center">
                                                <a title="Alterar" href="planacomorc.php?modulo=apoio/area_cultural_form&acao=A&mdeid=<?php echo $dados['mdeid']; ?>"><i class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></i></a>
                                            </td>
                                            <td><?php echo $dados['mdecod']; ?></td>
                                            <td><?php echo $dados['mdedsc']; ?></td>
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
