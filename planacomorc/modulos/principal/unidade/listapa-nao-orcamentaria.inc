<?php

    /**
     * Tela de listagem de Plano de Ação Não Orçamentária cadastrados no sistema.
     *
     * $Id: listapa-nao-orcamentaria.inc 102359 2019-03-12 06:56:07Z Douglas Santana $
     *
     * @filesource
     */

    $requisicao = $_REQUEST['requisicao'];
    $filtropa = $_SESSION['filtropa'] = ($_POST? $_POST: $_SESSION['filtropa']);

    $perfis = pegaPerfilGeral();

    # Verifica se o usuário tem vinculo de Sub-Unidades no seu Perfil
    $listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));

    if($requisicao) {
        switch ($requisicao) {
            case 'exibirGrafico':
                include_once APPRAIZ . "planacomorc/modulos/principal/unidade/grafico-pi-dotacao.inc";
                die();
            break;
            case 'carregarComboUG':
                echo montarComboUG((object) array(
                    'ungcod' => $filtropa['ungcod'],
                    'exercicio' => $_SESSION['exercicio'],
                    'unicod' => $_REQUEST['unicod'],
                    'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario
                ));
                die();
            break;
            case 'detalharPiDelegadas':
                include_once APPRAIZ . "planacomorc/modulos/principal/unidade/sub-unidades-delegadas.inc";
                die();
            break;
            case 'limpar':
                $filtropa = $_SESSION['filtropa'] = array();
            break;
            case 'listapimanter-xls':
                # Lista de PAs Não Orçamentários
                $filtropa['exercicio'] = $_SESSION['exercicio'];
                $filtropa['usucpf'] = $_SESSION['usucpf'];
                $filtropa['unofundo'] = 'FALSE';
                $filtropa['enquadramento'] = 'N';

                $mPlanointerno = new Spo_Model_Planointerno();
                $sql = $mPlanointerno->listarNaoOrcamentario((object) $filtropa);
                $listaPa = $mPlanointerno->carregar($sql);

                include_once APPRAIZ. "planacomorc/modulos/principal/unidade/listapa-nao-orcamentaria-xls.inc";
                die;
            break;
        }
    }

    /**
     * Monta a combo de UGs filtrando por UO
     *
     * @param $filtros
     * @return VOID
     */
    function montarComboUG(stdClass $filtros) {
        global $simec;

        return $simec->select(
            'ungcod',
            'SubUnidade',
            $filtros->ungcod,
            Public_Model_SubUnidadeOrcamentaria::queryCombo((object) array(
                'exercicio' => $filtros->exercicio,
                'unicod' => $filtros->unicod,
                'listaSubUnidadeUsuario' => $filtros->listaSubUnidadeUsuario)),
            array('multiple' => 'true'));
    }

    /**
     * Cabeçalho padrão do sistema.
     */
    include APPRAIZ . "includes/cabecalho.inc";
?>

<script src="js/unidade/lista-pi-manter.js?v=6"></script>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Pesquisa avançada por Planos de Ação Não Orçamentários</h5>
                </div>
                <div class="ibox-content">
                    <form id="filtropa" name="filtropa" method="POST" role="form" class="form-horizontal">
                        <input name="requisicao" id="requisicao" value="" type="hidden">

                        <?php
                            echo $simec->input('idpa', 'ID PA', $filtropa['idpa'], ['maxlength' => 6]);
                            # Se o usuário tiver Sub-Unidades vinculadas ao seu perfil o sistema não mostra opção de selecionar UO.
                            if (!$listaSubUnidadeUsuario) {
                                echo $simec->select('unicod', 'Unidade', $filtropa['unicod'], Spo_Model_Unidade::queryCombo((object)array(
                                    'prsano::INTEGER = '. (int)$_SESSION['exercicio'],
                                    'suo.unofundo = FALSE')), array('multiple' => 'true'));
                            }
                            echo '<div id=div_ungcod >';
                            echo $simec->select('ungcod', 'SubUnidade', $filtropa['ungcod'], Public_Model_SubUnidadeOrcamentaria::queryCombo((object) array(
                                'unofundo' => 'FALSE',
                                'exercicio' => $_SESSION['exercicio'],
                                'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)), array('multiple' => 'true'));
                            echo '</div>';
                            echo $simec->input('descricao', 'Título/Descrição', $filtropa['descricao'], ['maxlength' => 200]);
                            echo $simec->select('esdid', 'Situação', $filtropa['esdid'], Spo_Model_Unidade::queryComboEstados((object) array('tpdid' => WF_TPDID_PLANEJAMENTO_PI)), array('multiple' => 'true'));
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-warning btn-limpar" type="button">
                                    <span class="glyphicon glyphicon-remove-circle"></span> Limpar
                                </button>
                                <button class="btn btn-primary btn-buscar" type="submit">
                                    <span class="glyphicon glyphicon-search"></span> Buscar
                                </button>
                                <button id="btn-exportar-xls" class="btn btn-primary btn-buscar" type="button">
                                    <i class="fa fa-file-excel-o"></i> Exportar XLS
                                </button>
                                <?php if(!array_intersect($perfis, [PFL_CONSULTA, PFL_CONSULTA_UNIDADE])){ ?>
                                <button class="btn btn-success btn-novo" type="button">
                                    <span class="glyphicon glyphicon-plus"></span>
                                    Novo
                                </button>
                              <?php } ?>
                            </div>
                        </div>

                    </form>
                </div>
            </div>
        </div>
    </div>
</div>

<div class="row col-md-12" style="margin-bottom: 30px;">
    <?php
        # Lista de PAs Não Orçamentários
        $filtropa['exercicio'] = $_SESSION['exercicio'];
        $filtropa['usucpf'] = $_SESSION['usucpf'];
        $filtropa['unofundo'] = 'FALSE';
        $filtropa['enquadramento'] = 'N';

        $mPlanointerno = new Spo_Model_Planointerno();
        $sql = $mPlanointerno->listarNaoOrcamentario((object) $filtropa);
        $planoAcaoNaoOrcamentario = $mPlanointerno->carregar($sql);

    ?>

    <div class="wrapper wrapper-content animated fadeInRight">
        <div class="row">
            <div class="col-md-12">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Lista de Planos de Ação Não Orçamentários</h5>
                    </div>
                    <div class="ibox-content">
                        <div class="table-responsive">
                            <table class="table table-striped table-bordered table-hover dataTables" >
                                <thead>
                                    <tr class="text-center">
                                        <th width="5%">Ações</th>
                                        <th>Id</th>
                                        <th>Unidade</th>
                                        <th>Título</th>
                                        <th>Situação</th>
                                        <th>Quantidade</th>
                                        <th>Unidade de Medida</th>
                                        <th>Produto</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    <?php foreach($planoAcaoNaoOrcamentario as $planoAcao): ?>
                                        <tr>
                                            <td class="text-center" nowrap="nowrap">
                                                <a href="javascript:alterarPi(<?= $planoAcao['pliid']; ?>);" title="Editar">
                                                    <span class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></span>
                                                </a>
                                                <?php if (!array_intersect([PFL_CONSULTA_UNIDADE, PFL_SUBUNIDADE, PFL_CONSULTA], $perfis)) {?>
                                                    <a href="javascript:removerPi(<?= $planoAcao['pliid']; ?>);" title="Apagar">
                                                        <span class="btn btn-danger btn-sm glyphicon glyphicon-trash"></span>
                                                    </a>
                                                <?php } ?>
                                            </td>
                                            <td><?= $planoAcao['id']; ?></td>
                                            <td><?= $planoAcao['sub_unidade'] . ($planoAcao['compartilhada'] == 't'? ' <i style="font-size: 15px; color: #6892ff;" class="fa fa-handshake-o link" title="PA compartilhado com outras Subunidades."></i>\'': NULL); ?></td>
                                            <td><?= $planoAcao['plititulo']; ?></td>
                                            <td><?= $planoAcao['situacao']; ?></td>
                                            <td><?= $planoAcao['quantidade']; ?></td>
                                            <td><?= $planoAcao['unidademedida']; ?></td>
                                            <td><?= $planoAcao['produto']; ?></td>
                                        </tr>
                                    <?php endforeach; ?>
                                </tbody>
                            </table>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">
    sufixoUrl = '';

    $(document).ready(function(){
        initListaPiManter();
    });
</script>
