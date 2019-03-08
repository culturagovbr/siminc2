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
            case 'listapimanter-xls':
                # Lista de PAs
                $filtropi['exercicio'] = $_SESSION['exercicio'];
                $filtropi['usucpf'] = $_SESSION['usucpf'];
                $filtropi['unofundo'] = 'FALSE';

                $mPlanointerno = new Spo_Model_Planointerno();
                $sql = $mPlanointerno->listar((object) $filtropi);
                $listaPi = $mPlanointerno->carregar($sql);

                include_once APPRAIZ. "planacomorc/modulos/principal/unidade/listapimanter-xls.inc";
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

<script src="js/unidade/lista-pi-manter.js?v=4"></script>
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
                    <h5>Pesquisa avançada por Planos de Ação</h5>
                </div>
                <div class="ibox-content">
                    <form id="filtropi" name="filtropi" method="POST" role="form" class="form-horizontal">
                        <input name="requisicao" id="requisicao" value="" type="hidden">

                        <?php
                            echo $simec->input('plicod', 'Código PI ou ID PA', $filtropi['plicod'], ['maxlength' => 11]);
                            # Se o usuário tiver Sub-Unidades vinculadas ao seu perfil o sistema não mostra opção de selecionar UO.
                            if (!$listaSubUnidadeUsuario) {
                                echo $simec->select('unicod', 'Unidade', $filtropi['unicod'], Spo_Model_Unidade::queryCombo((object)array(
                                    'prsano::INTEGER = '. (int)$_SESSION['exercicio'],
                                    'suo.unofundo = FALSE')), array('multiple' => 'true'));
                            }
                            echo '<div id=div_ungcod >';
                            echo $simec->select('ungcod', 'SubUnidade', $filtropi['ungcod'], Public_Model_SubUnidadeOrcamentaria::queryCombo((object) array(
                                'unofundo' => 'FALSE',
                                'exercicio' => $_SESSION['exercicio'],
                                'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)), array('multiple' => 'true'));
                            echo '</div>';
                            echo $simec->select('ptres', 'PTRES', $filtropi['ptres'], Spo_Model_Ptres::queryCombo((object) array(
                                'exercicio' => $_SESSION['exercicio'],
                                'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)), array('multiple' => 'true'));
                            echo $simec->input('descricao', 'Título/Descrição', $filtropi['descricao'], ['maxlength' => 200]);
                            echo $simec->select('eqdid', 'Enquadramento da Despesa', $filtropi['eqdid'], Spo_Model_Ptres::queryComboEnquadramento((object) array('exercicio' => $_SESSION['exercicio'])), array('multiple' => 'true'));
                            echo $simec->radio('pliemenda', 'Emenda', (!$filtropi['pliemenda'] ? '' : $filtropi['pliemenda']), array('t' => 'Sim', 'f' => 'Não', '' => 'Todos'));
                            echo $simec->input('benid', 'ID Beneficiário', $filtropi['benid'], ['maxlength' => 200]);
                            echo $simec->select('esdid', 'Situação', $filtropi['esdid'], Spo_Model_Unidade::queryComboEstados((object) array('tpdid' => WF_TPDID_PLANEJAMENTO_PI)), array('multiple' => 'true'));
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
        # Lista de PAs
        $filtropi['exercicio'] = $_SESSION['exercicio'];
        $filtropi['usucpf'] = $_SESSION['usucpf'];
        $filtropi['unofundo'] = 'FALSE';

        $mPlanointerno = new Spo_Model_Planointerno();
        $sql = $mPlanointerno->listar((object) $filtropi);
        $aDados = $mPlanointerno->carregar($sql);

    ?>

    <div class="ibox float-e-margins">
    	<div class="ibox-title">
    		<h5>Lista de Planos de Ação</h5>
    	</div>
    	<div class="ibox-content">
            <div class="table-responsive">
                <table class="table table-striped table-bordered table-hover dataTables" >
                    <tbody>
                    <?php
                    $total['custeio'] = $total['capital'] = $total['empenhado'] = $total['pago'] = $total['liquidado'] = 0;
                    foreach($aDados as $dados){
                        if($dados['plistatus'] == 'A'){
                            $total['custeio'] += $dados['custeio'];
                            $total['capital'] += $dados['capital'];
                            $total['autorizado'] += $dados['autorizado'];
                            $total['empenhado'] += $dados['empenhado'];
                            $total['pago'] += $dados['pago'];
                            $total['liquidado'] += $dados['liquidado'];
                        }
                        ?>
                        <tr>
                            <td class="text-center" nowrap="nowrap">
                                <a href="javascript:alterarPi(<?php echo $dados['pliid']; ?>);" title="Editar"><span class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></span></a>
                              <?php if (!array_intersect([PFL_CONSULTA_UNIDADE, PFL_SUBUNIDADE, PFL_CONSULTA], $perfis)) {?>
                                <a href="javascript:removerPi(<?php echo $dados['pliid']; ?>);" title="Apagar"><span class="btn btn-danger btn-sm glyphicon glyphicon-trash"></span></a>
                             <?php } ?>
                            </td>
                            <td><?php echo $dados['id']; ?></td>
                            <td><?php echo $dados['codigo_pi']; ?></td>
                            <td><?php echo $dados['sub_unidade']; ?></td>
                            <td><?php echo $dados['plititulo']; ?></td>
                            <td><?php echo $dados['funcional']; ?></td>
                            <td><?php echo $dados['situacao']; ?></td>

                            <?php
                                # Fomatações para ficar mais visível os valores de Custeio e Capital.
                                $custeioformat = number_format($dados['custeio'],2,',','.');
                                $capitalformat = number_format($dados['capital'],2,',','.');
                            ?>

                            <td title="Valor Custeio: <?= $custeioformat ."\n\n"?>Valor Capital: <?= $capitalformat ?>" class="text-right">
                                <?= number_format(($dados['custeio'] + $dados['capital']), 2, ',', '.'); ?>
                            </td>
                            <th class="text-right"><?php echo marcarVermelhoValorSuperior($dados['autorizado'], $dados['custeio']+ $dados['capital'], 2); ?></th>
                            <th class="text-right"><?php echo marcarVermelhoValorSuperior($dados['empenhado'], $dados['custeio']+ $dados['capital'], 2); ?></th>
                            <th class="text-right"><?php echo marcarVermelhoValorSuperior($dados['liquidado'], $dados['custeio']+ $dados['capital'], 2); ?></th>
                            <th class="text-right"><?php echo marcarVermelhoValorSuperior($dados['pago'], $dados['custeio']+ $dados['capital'], 2); ?></th>
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
                        <th colspan="7">Orçamento do PA (R$)</th>
                    </tr>
                    <tr class="text-center">
                        <th>Total</th>
                        <th>Provisionado</th>
                        <th>Empenhado</th>
                        <th>Liquidado</th>
                        <th>Pago</th>
                    </tr>
                    <tr>
                        <th colspan="7">TOTAL</th>
                        <?php
                            $totalCusteioFormat = number_format($total['custeio'],2,',','.');
                            $totalCapitalFormat = number_format($total['capital'],2,',','.');
                        ?>
                        <th title="Valor Total Custeio: <?= $totalCusteioFormat."\n\n"?>Valor Total Capital: <?= $totalCapitalFormat."\n\n"?>" class="text-right">
                            <?= number_format(($total['custeio'] + $total['capital']), 2, ',', '.'); ?>
                        </th>
                        <th class="text-right"><?php echo marcarVermelhoValorSuperior($total['autorizado'], $total['custeio']+ $total['capital'], 2); ?></th>
                        <th class="text-right"><?php echo marcarVermelhoValorSuperior($total['empenhado'], $total['custeio']+ $total['capital'], 2); ?></th>
                        <th class="text-right"><?php echo marcarVermelhoValorSuperior($total['liquidado'], $total['custeio']+ $total['capital'], 2); ?></th>
                        <th class="text-right"><?php echo marcarVermelhoValorSuperior($total['pago'], $total['custeio']+ $total['capital'], 2); ?></th>
                    </tr>
                    </thead>
                    <tfoot>
                    <tr>
                        <th colspan="7">TOTAL</th>
                        <th title="Valor Total Custeio: <?= $totalCusteioFormat."\n\n"?>Valor Total Capital: <?= $totalCapitalFormat."\n\n"?>" class="text-right">
                            <?= number_format(($total['custeio'] + $total['capital']), 2, ',', '.'); ?>
                        </th>
                        <th class="text-right"><?php echo marcarVermelhoValorSuperior($total['autorizado'], $total['custeio']+ $total['capital'], 2); ?></th>
                        <th class="text-right"><?php echo marcarVermelhoValorSuperior($total['empenhado'], $total['custeio']+ $total['capital'], 2); ?></th>
                        <th class="text-right"><?php echo marcarVermelhoValorSuperior($total['liquidado'], $total['custeio']+ $total['capital'], 2); ?></th>
                        <th class="text-right"><?php echo marcarVermelhoValorSuperior($total['pago'], $total['custeio']+ $total['capital'], 2); ?></th>
                    </tr>
                    </tfoot>
                </table>
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
