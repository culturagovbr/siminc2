<?php

$aMetappa = (new Public_Model_MetaPpa)->recuperarTodos('*' , ["prsano = '{$_SESSION['exercicio']}'"]);

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
                    
                    <a class="btn btn-small btn-warning" href="?modulo=apoio/metappa_form&acao=A">Novo</a>
                    
                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                            <tr class="text-center">
                                <th width="10%">A��es</th>
                                <th>C�digo</th>
                                <th>Meta</th>
                                <th>Ano</th>
                                <th>Quantificavel</th>
                                <th>Quantidade</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($aMetappa as $dados){ ?>
                                <tr>
                                    <td class="text-center">
                                        <a title="Alterar" href="?modulo=apoio/metappa_form&acao=A&mppid=<?php echo $dados['mppid']; ?>"><i class="fa fa-pencil"></i></a>
                                    </td>
                                    <td><?php echo $dados['mppcod']; ?></td>
                                    <td><?php echo $dados['mppnome']; ?></td>
                                    <td><?php echo $dados['prsano']; ?></td>
                                    <td><?php echo $dados['mppquantificavel'] == 't'? 'Sim': 'N�o'; ?></td>
                                    <td><?php echo $dados['mppquantidade']; ?></td>
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