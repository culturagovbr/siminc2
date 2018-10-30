<?php include APPRAIZ . "includes/cabecalho.inc";  ?>

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
                    <a class="btn-small btn btn-success" href="?modulo=apoio/produtos_pi_form&acao=A">
                        <span class="glyphicon glyphicon-plus"></span>
                        Novo
                    </a>
                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                            <tr class="text-center">
                                <th>Ação</th>
                                <th>Nome</th>
                                <th>Descrição</th>
                                <th>Status</th>
                            </tr>
                            </thead>
                            <tbody>

                            <?php
                            /**
                             * @todo Colocar a consulta na model, usando a prática do MVC e padronizando com o sistema.
                             */
							$sql = "
                                SELECT
                                    pprid AS codigo,
                                    pprnome AS nome,
                                    pprdescricao AS descricao,
                                    pprstatus AS status
                                FROM monitora.pi_produto
                                WHERE
                                    prsano = '{$_SESSION['exercicio']}'
                                ORDER BY
                                    descricao
                            ";
                            $produtos = $db->carregar($sql);

                            if($produtos):
                            foreach($produtos as $produto): ?>
                                <tr>
                                    <td class="text-center">
                                        <a title="Alterar" href="planacomorc.php?modulo=apoio/produtos_pi_form&acao=A&pprid=<?php echo $produto['codigo']; ?>"><i class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></i></a>
                                        <!--<a style="color: red;" title="Excluir" href="planacomorc.php?modulo=apoio/produtos_pi_form&acao=A&req=excluir&pprid=<?php /*echo $produto['codigo']; */?>"><i class="fa fa-trash"></i></a>-->
                                    </td>
                                    <td><?php echo $produto['nome']; ?></td>
                                    <td><?php echo $produto['descricao']; ?></td>
                                    <td><?php echo $produto['status'] == 'A' ? 'Ativo' : 'Inativo'; ?></td>
                                </tr>
                            <?php endforeach;
                            endif; ?>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
