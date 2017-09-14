<?php

$aCategoriaApropriacao = (new Monitora_Model_CategoriaApropriacao())->recuperarTodos('*', ["capstatus = 'A'", "capano = '{$_SESSION['exercicio']}'"], 'capdsc');

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

                    <a class="btn btn-small btn-warning" href="?modulo=apoio/modalidade-pactuacao-form&acao=A">Novo</a>

                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                            <tr class="text-center">
                                <th width="10%">A��es</th>
                                <th>C�digo</th>
                                <th>Descri��o</th>
                                <th>SICONV</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($aCategoriaApropriacao as $dados){ ?>
                                <tr>
                                    <td class="text-center">
                                        <a title="Alterar" href="planacomorc.php?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=<?php echo $dados['capid']; ?>"><i class="fa fa-pencil"></i></a>
                                    </td>
                                    <td class="text-center"><?php echo $dados['capcod']; ?></td>
                                    <td><?php echo $dados['capdsc']; ?></td>
                                    <td class="text-center"><?php echo $dados['capsiconv'] == 't' ? 'Sim' : 'N�o'; ?></td>
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