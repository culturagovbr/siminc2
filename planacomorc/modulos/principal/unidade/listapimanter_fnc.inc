<?php

    /**
     * Tela de listagem de PI cadastrados no sistema.
     *
     * $Id: listapimanter.inc 102359 2015-09-11 18:26:07Z maykelbraz $
     *
     * @filesource
     */

    $requisicao = $_REQUEST['requisicao'];
    $filtropi = $_SESSION['filtropi'] = ($_POST? $_POST: $_SESSION['filtropi']);

    $perfis = pegaPerfilGeral();

    $sqlListaUO = Spo_Model_Unidade::queryCombo((object)array(
        'prsano::INTEGER = '. (int)$_SESSION['exercicio'],
        'suo.unofundo = TRUE'));

    $listaUO = $db->carregar($sqlListaUO);

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
                    'ungcod' => $filtropi['ungcod'],
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
                $filtropi = $_SESSION['filtropi'] = array();
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
            'Unidade',
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

<script src="js/unidade/lista-pi-manter.js?v=2"></script>
<style>
/*    .tabela-listagem {
        font-size: 1em !important;
    }*/
</style>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Pesquisa avançada por PI do FNC</h5>
                </div>
                <div class="ibox-content">
                    <form id="filtropi" name="filtropi" method="POST" role="form" class="form-horizontal">
                        <input name="requisicao" id="requisicao" value="" type="hidden">

                        <?php
                            echo $simec->input('plicod', 'Código ou ID do PI', $filtropi['plicod'], ['maxlength' => 11]);
                            # Se o usuário tiver Sub-Unidades vinculadas ao seu perfil o sistema não mostra opção de selecionar UO.
                            if(!$listaSubUnidadeUsuario) {
                                echo $simec->select('unicod', 'Unidade', current($listaUO), $sqlListaUO, array('multiple' => 'true'));
                            }
                            echo '<div id=div_ungcod >';
                            echo $simec->select('ungcod', 'SubUnidade', $filtropi['ungcod'], Public_Model_SubUnidadeOrcamentaria::queryCombo((object) array(
                                'unofundo' => 'TRUE',
                                'exercicio' => $_SESSION['exercicio'],
                                'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)), array('multiple' => 'true'));
                            echo '</div>';
                            echo $simec->select('ptres', 'PTRES', $filtropi['ptres'], Spo_Model_Ptres::queryCombo((object) array(
                                'exercicio' => $_SESSION['exercicio'],
                                'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)), array('multiple' => 'true'));
                            echo $simec->input('descricao', 'Título/Descrição', $filtropi['descricao'], ['maxlength' => 200]);
                            echo $simec->select('eqdid', 'Enquadramento da Despesa', $filtropi['eqdid'], Spo_Model_Ptres::queryComboEnquadramento((object) array('exercicio' => $_SESSION['exercicio'])), array('multiple' => 'true'));
                            echo $simec->radio('pliemenda', 'Emenda', (!$filtropi['pliemenda'] ? '' : $filtropi['pliemenda']), array('t' => 'Sim', 'f' => 'Não', '' => 'Todos'));
                            echo $simec->select('esdid', 'Situação', $filtropi['esdid'], Spo_Model_Unidade::queryComboEstados((object) array('tpdid' => WF_TPDID_FNC_PLANEJAMENTO_PI)), array('multiple' => 'true'));
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-warning btn-limpar" type="button">
                                    <span class="glyphicon glyphicon-remove-circle"></span> Limpar
                                </button>
                                <button class="btn btn-primary btn-buscar" type="submit">
                                    <span class="glyphicon glyphicon-search"></span> Buscar
                                </button>
                                <?php if(!array_intersect($perfis, [PFL_CONSULTA, PFL_CONSULTA_UNIDADE])){ ?>
                                <button class="btn btn-success btn-novo-fnc" type="button">
                                    <span class="glyphicon glyphicon-plus"></span>
                                    Novo
                                </button>
                              <?php }?>
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

        # Lista de PIs
        $filtropi['exercicio'] = $_SESSION['exercicio'];
        $filtropi['usucpf'] = $_SESSION['usucpf'];
        $filtropi['unofundo'] = 'TRUE';

        $mPlanointerno = new Spo_Model_Planointerno();
        $sql = $mPlanointerno->listar((object) $filtropi);
        $aDados = $mPlanointerno->carregar($sql);
    ?>

    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5>Lista de PIs</h5>
        </div>
        <div class="ibox-content">
            <div class="table-responsive">
                <table class="table table-striped table-bordered table-hover dataTables" >
                    <tbody>
                    <?php
                    $total['custeio'] = $total['capital'] = $total['empenhado'] = $total['pago'] = $total['liquidado'] = 0;
                    foreach($aDados as $dados){
                        $total['custeio'] += $dados['custeio'];
                        $total['capital'] += $dados['capital'];
                        $total['autorizado'] += $dados['autorizado'];
                        $total['empenhado'] += $dados['empenhado'];
                        $total['pago'] += $dados['pago'];
                        $total['liquidado'] += $dados['liquidado'];
                        ?>
                        <tr>
                            <td class="text-center" nowrap="nowrap">
                                <a href="javascript:alterarPiFnc(<?php echo $dados['pliid']; ?>);" title="Editar"><span class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></span></a>
                                <?php if(!array_intersect($perfis, [PFL_CONSULTA, PFL_CONSULTA_UNIDADE])){ ?>
                                    <a href="javascript:removerPi(<?php echo $dados['pliid']; ?>);" title="Apagar"><span class="btn btn-danger btn-sm glyphicon glyphicon-trash"></span></a>
                                <?php } ?>
                            </td>
                            <td><?php echo $dados['id']; ?></td>
                            <td><?php echo $dados['codigo_pi']; ?></td>
                            <td><?php echo $dados['sub_unidade']; ?></td>
                            <td><?php echo $dados['plititulo']; ?></td>
                            <td><?php echo $dados['funcional']; ?></td>
                            <td><?php echo $dados['situacao']; ?></td>
                            <td class="text-right"><?php echo number_format($dados['custeio'], 2, ',', '.'); ?></td>
                            <td class="text-right"><?php echo number_format($dados['capital'], 2, ',', '.'); ?></td>
                            <td class="text-right"><?php echo number_format(($dados['custeio'] + $dados['capital']), 2, ',', '.'); ?></td>
                            <td class="text-right"><?php echo number_format($dados['autorizado'], 2, ',', '.'); ?></td>
                            <td class="text-right"><?php echo number_format($dados['empenhado'], 2, ',', '.'); ?></td>
                            <td class="text-right"><?php echo number_format($dados['liquidado'], 2, ',', '.'); ?></td>
                            <td class="text-right"><?php echo number_format($dados['pago'], 2, ',', '.'); ?></td>
                        </tr>
                    <?php } ?>
                    </tbody>
                    <thead>
                    <tr class="text-center">
                        <th rowspan="2" width="5%">Ações</th>
                        <th rowspan="2">Id</th>
                        <th rowspan="2">Código</th>
                        <th rowspan="2">Unidade</th>
                        <th rowspan="2">Título</th>
                        <th rowspan="2">Funcional</th>
                        <th rowspan="2">Situação</th>
                        <th colspan="7">Orçamento do PI (R$)</th>
                    </tr>
                    <tr class="text-center">
                        <th>Custeio</th>
                        <th>Capital</th>
                        <th>Total</th>
                        <th>Provisionado</th>
                        <th>Empenhado</th>
                        <th>Liquidado</th>
                        <th>Pago</th>
                    </tr>
                    <tr>
                        <th colspan="7">TOTAL</th>
                        <th class="text-right"><?php echo number_format($total['custeio'], 2, ',', '.'); ?></th>
                        <th class="text-right"><?php echo number_format($total['capital'], 2, ',', '.'); ?></th>
                        <th class="text-right"><?php echo number_format(($total['custeio'] + $total['capital']), 2, ',', '.'); ?></th>
                        <th class="text-right"><?php echo number_format($total['autorizado'], 2, ',', '.'); ?></th>
                        <th class="text-right"><?php echo number_format($total['empenhado'], 2, ',', '.'); ?></th>
                        <th class="text-right"><?php echo number_format($total['liquidado'], 2, ',', '.'); ?></th>
                        <th class="text-right"><?php echo number_format($total['pago'], 2, ',', '.'); ?></th>
                    </tr>
                    </thead>
                    <tfoot>
                    <tr>
                        <th colspan="7">TOTAL</th>
                        <th class="text-right"><?php echo number_format($total['custeio'], 2, ',', '.'); ?></th>
                        <th class="text-right"><?php echo number_format($total['capital'], 2, ',', '.'); ?></th>
                        <th class="text-right"><?php echo number_format(($total['custeio'] + $total['capital']), 2, ',', '.'); ?></th>
                        <th class="text-right"><?php echo number_format($total['autorizado'], 2, ',', '.'); ?></th>
                        <th class="text-right"><?php echo number_format($total['empenhado'], 2, ',', '.'); ?></th>
                        <th class="text-right"><?php echo number_format($total['liquidado'], 2, ',', '.'); ?></th>
                        <th class="text-right"><?php echo number_format($total['pago'], 2, ',', '.'); ?></th>
                    </tr>
                    </tfoot>
                </table>
            </div>

        </div>
    </div>
</div>

<script type="text/javascript">
    sufixoUrl = '_fnc';

    $(document).ready(function(){
        initListaPiManter();
    });
</script>
