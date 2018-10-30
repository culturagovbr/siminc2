<?php include APPRAIZ . "includes/cabecalho.inc"; ?>

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
                    <a class="btn-small btn btn-success" href="?modulo=apoio/item-manutencao-form&acao=A">
                        <span class="glyphicon glyphicon-plus"></span>
                        Novo
                    </a>
                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables">
                            <thead>
                            <tr class="text-center">
                                <th>Ação</th>
                                <th>Nome</th>
                                <th>Descrição</th>
                                <th>Enquadramento</th>
                            </tr>
                            </thead>
                            <tbody>
							<?php
                            /**
                             * @todo Colocar a consulta na model, usando a prática do MVC.
                             */
							$sql = "
                                SELECT DISTINCT
                                    maiid AS codigo,
                                    mainome AS nome,
                                    maidescricao AS descricao,
                                    eqddsc AS enquadramento
                                FROM planacomorc.manutencaoitem
                                    LEFT JOIN monitora.pi_enquadramentodespesa eqd ON(manutencaoitem.eqdid = eqd.eqdid)
                                WHERE
                                    prsano = '{$_SESSION['exercicio']}'
                                    AND maistatus = 'A'
                                ORDER BY
                                    nome
                            ";
//							ver($sql,d);
							$manutencao_itens = $db->carregar($sql);

							if ($manutencao_itens):
								foreach ($manutencao_itens as $manutencao_item): ?>
                                    <tr>
                                        <td class="text-center">
                                            <a title="Alterar" href="planacomorc.php?modulo=apoio/item-manutencao-form&acao=A&maiid=<?php echo $manutencao_item['codigo']; ?>"><i class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></i></a>
                                        </td>
                                        <td><?php echo $manutencao_item['nome']; ?></td>
                                        <td><?php echo $manutencao_item['descricao']; ?></td>
                                        <td><?php echo $manutencao_item['enquadramento']; ?></td>
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
