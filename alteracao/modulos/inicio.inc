<?php

    include_once APPRAIZ. "includes/library/simec/Grafico.php";

    require_once APPRAIZ. 'includes/workflow.php';

    # Lista de perfis vinculados ao usuário logado
    $perfis = pegaPerfilGeral();

    # Filtro por Subunidades do perfil do usuário logado.
    $listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));

    # Filtro de Situação na sessão do usuário
    $filtroalteracao = $_SESSION['filtroalteracao'] = (isset($_REQUEST['esdid'])? $_REQUEST: $_SESSION['filtroalteracao']);
    $requisicao = $_REQUEST['requisicao'];

    $aAlteracao = (new Alteracao_Model_Pedido())->recuperarListagem(array(
        'exercicio' => $_SESSION['exercicio'],
        'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario
    ), $filtroalteracao);
    if($requisicao){
        switch ($requisicao) {
            case 'limpar':
                $filtroalteracao = $_SESSION['filtroalteracao'] = array();
            case 'buscar_pedidos':
                $aAlteracao = (new Alteracao_Model_Pedido())->recuperarListagem($_REQUEST);
            break;
        }
    }
    /**
     * Cabeçalho padrão do sistema.
     * @see cabecalho.inc
     */
    include APPRAIZ. "includes/cabecalho.inc";
?>
<script src="js/principal/alteracao.js?v=0"></script>
<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-8">
        <h2>Painel de Acompanhamento de Pedidos de Alteração</h2>
    </div>
    <div class="col-lg-4">
        <div style="padding-top: 10px;" class="text-right">
            <button id="btn-exportar-xls" class="btn btn-primary btn-buscar" type="button">
                <i class="fa fa-file-excel-o"></i> Exportar XLS
            </button>
        </div>
    </div>
</div>
<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">

                <div class="tabs-container tabs">
                    <ul class="nav nav-tabs prodTabs">
                        <li class="active"><a data-toggle="tab" href="#tab-interno" tipo="Interno">Interno</a></li>
                        <li class=""><a data-toggle="tab" href="#tab-externo" tipo="Externo">Externo</a></li>
                    </ul>
                    <div class="ibox-title">
                        <h5>Pesquisa avançada por Pedidos de Altera&ccedil;&atilde;o</h5>
                    </div>
                    <div class="tab-content">
                        <div id="tab-interno" class="tab-pane active">
                            <?php
                            $tpdid = WF_TPDID_INTERNO;
                            include(APPRAIZ . "alteracao/modulos/principal/filtro_pedido_credito.inc");?>
                        </div>
                        <div id="tab-externo" class="tab-pane">
                            <?php
                            $tpdid = WF_TPDID_EXTERNO;
                            include(APPRAIZ . "alteracao/modulos/principal/filtro_pedido_credito.inc");?>
                        </div>
                    </div>
                </div>



            </div>
        </div>
    </div>
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">
                    <div class="table-responsive">
                        
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                                <tr class="text-center">
                                    <th width="4%">Ações</th>
                                    <th>Nº do Pedido</th>
                                    <th>Tipo de Cr&eacute;dito</th>
                                    <th>Janela</th>
                                    <th>Situa&ccedil;&atilde;o</th>
                                    <th>Unidade</th>
                                    <th>Cancelado</th>
                                    <th>Suplementado</th>
                                    <th width="20%">T&iacute;tulo</th>
                                    <th>Cadastrado em</th>
                                </tr>
                            </thead>
                            <tbody>
                            <?php
                            foreach($aAlteracao as $dados): ?>
                                <tr>
                                    <td class="text-center" nowrap="nowrap">
                                        <a href="?modulo=principal/cadastro_alteracoes&acao=C&pedid=<?=$dados['pedid'];?>&tpdid=<?=$tpdid;?>" title="Editar"><span class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></span></a>
                                        <a href="?modulo=principal/cadastro_alteracoes&acao=C&pedid=<?=$dados['pedid'];?>&req=excluir_pedido" title="Apagar"><span class="btn btn-danger btn-sm glyphicon glyphicon-trash"></span></a>
                                    </td>
                                    <td><?= $dados['pedid']; ?></td>
                                    <td><?= $dados['tpadsc']; ?></td>
                                    <td><?= $dados['jannome']; ?></td>
                                    <td><?= $dados['sit']; ?></td>
                                    <td><?= $dados['suoid'].' - '.$dados['suonome']; ?></td>
                                    <td><?= $dados['cnc']; ?></td>
                                    <td><?= $dados['soap']; ?></td>
                                    <td><?= $dados['pedtitulo']; ?></td>
                                    <td><?= $dados['cad']; ?></td>
                                </tr>
                            <?php endforeach ?>
                            </tbody>
                        </table>

                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<script>

    $(document).ready(function(){
        initListaAlteracao();
    });

</script>
