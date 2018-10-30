<?php

$listaAutor = (new Emendas_Model_Autor)->buscarTodos((object)array());

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
                    <a class="btn-small btn btn-success" href="?modulo=apoio/autor_form&acao=A">
                        <span class="glyphicon glyphicon-plus"></span>
                        Novo
                    </a>
                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                                <tr class="text-center">
                                    <th width="10%">Ações</th>
                                    <th>Código</th>
                                    <th>Nome</th>
                                    <th>Sexo</th>
                                    <th>Tipo</th>
                                    <th>Partido</th>
                                    <th>E-mail</th>
                                    <th>UF</th>
                                </tr>
                            </thead>
                            <tbody>
                                <?php foreach($listaAutor as $dados){ ?>
                                    <tr>
                                        <td class="text-center">
                                            <a title="Alterar" href="?modulo=apoio/autor_form&acao=A&autid=<?php echo $dados['autid']; ?>"><i class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></i></a>
                                        </td>
                                        <td><?php echo $dados['autcod']; ?></td>
                                        <td><?php echo $dados['autnome']; ?></td>
                                        <td><?php echo retornarDescricaoSexo($dados['autsexo']); ?></td>
                                        <td><?php echo $dados['atpdsc']; ?></td>
                                        <td><?php echo $dados['parsigla']; ?></td>
                                        <td><?php echo $dados['autemail']; ?></td>
                                        <td><?php echo $dados['estuf']; ?></td>
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