<?php

    include_once APPRAIZ. "includes/library/simec/Grafico.php";

    require_once APPRAIZ. 'includes/workflow.php';

    # Lista de perfis vinculados ao usuário logado
    $perfis = pegaPerfilGeral();

    # Filtro por Subunidades do perfil do usuário logado.
    $listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));

    # Filtro de Situação na sessão do usuário
    $filtroalteracao = $_SESSION['filtroalteracao'] = (isset($_REQUEST['esdid'])? $_REQUEST: $_SESSION['filtroalteracao']);
    $requisicao = $_REQUEST['filtroalteracao'];


    $aAlteracao = (new Alteracao_Model_Alteracao())->recuperarListagem((object) array(
        'exercicio' => $_SESSION['exercicio'],
        'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario
    ), $filtroalteracao);

    if($requisicao){
        switch ($requisicao) {
            case 'limpar':
                $filtroalteracao = $_SESSION['filtroalteracao'] = array();
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
<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Pesquisa avançada por Pedidos de Cr&eacute;dito</h5>
                </div>
                <div class="ibox-content">
                    <form id="filtroalteracao" name="filtroalteracao" method="POST" role="form" class="form-horizontal">
                        <input name="requisicao" id="requisicao" value="" type="hidden">

                        <div class="row">
                            <div class="col-lg-3">
                                <?= $simec->input('altid',  'Nº do Pedido', $filtroalteracao['altid'], ['maxlength' => 12, 'class' => 'inteiro'], ['input-size' => 9, 'label-size' => 2]); ?>
                            </div>
                            <div class="col-lg-3">
                                <?= $simec->select('sitid', 'Situação', $filtroalteracao['sitid'], (new Monitora_Model_PiEnquadramentoDespesa())->recuperarSqlCombo(), null,  ['input-size' => 9, 'label-size' => 2]); ?>
                            </div>
                            <div class="col-lg-6">
                                <?= $simec->select('suoid[]', 'Unidades', $filtroalteracao['suoid'], (new Public_Model_SubUnidadeOrcamentaria())->queryComboChaveSuoid((object)array('exercicio' => $_SESSION['exercicio'], 'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)), null, ['input-size' => 9, 'label-size' => 2]); ?>
                            </div>
                        </div>

                        <div class="row">
                            <div class="col-lg-3">
                                <?= $simec->select('janid', 'Tipo', $filtroalteracao['janid'], (new Monitora_Model_PiEnquadramentoDespesa())->recuperarSqlCombo(), null,  ['input-size' => 9, 'label-size' => 2]); ?>
                            </div>
                            <div class="col-lg-3">
                                <?= $simec->select('janid', 'Janela', $filtroalteracao['janid'], (new Monitora_Model_PiEnquadramentoDespesa())->recuperarSqlCombo(), null,  ['input-size' => 9, 'label-size' => 2]); ?>
                            </div>
                            <div class="col-lg-6">
                                <?= $simec->input('altid',  'Título', $filtroalteracao['altid'], NULL, ['input-size' => 9, 'label-size' => 2]); ?>
                            </div>
                        </div>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-warning btn-limpar" type="button">
                                    <span class="glyphicon glyphicon-remove-circle"></span> Limpar
                                </button>
                                <button class="btn btn-primary btn-buscar" type="submit">
                                    <span class="glyphicon glyphicon-search"></span> Buscar
                                </button>
                                <button class="btn btn-success btn-novo" type="button">
                                    <span class="glyphicon glyphicon-plus"></span>
                                    Novo
                                </button>
                            </div>
                        </div>

                    </form>
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
                                    <th>Cadatrado em</th>
                                </tr>
                            </thead>
                            <tbody>
                            <?php
                            foreach($aAlteracao as $dados){ ?>
                                <tr>
                                    <td class="text-center" nowrap="nowrap">
                                        <a href="#" title="Editar"><span class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></span></a>
                                            <a href="#" title="Apagar"><span class="btn btn-danger btn-sm glyphicon glyphicon-trash"></span></a>
                                    </td>
                                    <td><?= $dados['ped']; ?></td>
                                    <td><?= $dados['tpc']; ?></td>
                                    <td><?= $dados['jan']; ?></td>
                                    <td><?= $dados['sit']; ?></td>
                                    <td><?= $dados['suoid']; ?></td>
                                    <td><?= $dados['cnc']; ?></td>
                                    <td><?= $dados['soap']; ?></td>
                                    <td><?= $dados['tit']; ?></td>
                                    <td><?= $dados['cad']; ?></td>
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

<script>

    $(document).ready(function(){
        initListaAlteracao();
    });

</script>
