<?php

    include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

    $modelPlanoInterno = new Pi_PlanoInterno();

    switch($_REQUEST['req']){
        case 'atualizarSIOP':
            try{
                $cImportaDadosSiop = new Planacomorc_Controller_ImportaDadosSiop();
                $cImportaDadosSiop->AtualizarDados();
                $cImportaDadosSiop->AtualizarDotacao();

                echo simec_json_encode(array(
                    'success' => 1,
                    'message' => 'As informações de execução orçamentária(Aprovisionado, Empenhado, liquidado e Pago) foram Atualizadas com sucesso!'
                ));
            } catch (Exception $e){
                echo simec_json_encode(array(
                    'success' => 0,
                    'message' => 'Não foi possível atualizar as informações de execução orçamentária(Aprovisionado, Empenhado, liquidado e Pago). O sistema não conseguiu acessar o SIOP-Sistema Integrado de Planejamento e Orçamento do Governo Federal. https://www.siop.planejamento.gov.br'
                ));
            }
            die;      
    }
    
    $listaSubunidadeFnc = $modelPlanoInterno->listarSubunidadeFnc((object)array('exercicio' => $_SESSION['exercicio']));

    $aDadosPorAcao = $modelPlanoInterno->recuperarDadosFncPorAcao($_SESSION['exercicio']);
    $aValoresEstado = $modelPlanoInterno->recuperarValoresPorEstado($_SESSION['exercicio']);
    $aValoresExecucao = $modelPlanoInterno->recuperarValoresPorExecucao($_SESSION['exercicio']);
    
    include APPRAIZ . "includes/cabecalho.inc";
?>
<script src="js/painel.js?v=1"></script>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-4">
        <h2 style="color: red;">Painel de Acompanhamento FNC</h2>
    </div>
    <div class="col-lg-8">
        <div style="padding-top: 10px;" class="text-right">
            <?php if((in_array(PFL_ADMINISTRADOR, $perfis)) || (in_array(PFL_SUPERUSUARIO, $perfis))): ?>
                <a id="btn-atualizar-siop" class="btn btn-success">
                    <i class="fa fa-cloud-download"></i> Atualizar SIOP
                </a>
            <?php endif; ?>
            <a class="btn btn-success" href="planacomorc.php?modulo=inicio&acao=C"><i class="fa fa-area-chart"></i> Painel</a>
            <a class="btn btn-danger" href="planacomorc.php?modulo=principal/unidade/listapimanter_fnc&acao=A"><i class="fa fa-list"></i> Lista de PI - FNC</a>
            <?php if(!array_intersect($perfis, [PFL_CONSULTA, PFL_CONSULTA_UNIDADE])): ?>
                <a class="btn btn-warning" href="planacomorc.php?modulo=principal/unidade/cadastro_pi_fnc&acao=A"><i class="fa fa-plus-circle"></i> Criar PI - FNC</a>
            <?php endif; ?>
            <a class="btn btn-primary" href="planacomorc.php?modulo=relatorio/geral&acao=A&fundo=t"><i class="fa fa-file-excel-o"></i> Relatório Geral</a>
        </div>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">

        <div class="col-md-12">

            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5 class="col-lg-3">Acompanhamento Geral FNC - Por Unidade</h5>
                    <a class="btn btn-primary" href="planacomorc.php?modulo=inicio&acao=C&req=exportar_xls_unidade"><i class="fa fa-file-excel-o"></i> Exportar XLS</a>
                </div>
                <div class="ibox-content">
                    <div class="table-responsive">
                        <table class="table table-bordered table-hover" >
                            <thead>
                            <tr class="text-center">
                                <th>Unidade</th>
                                <th>Apresentado</th>
                                <th>Selecionado</th>
                                <th>Aprovado</th>
                                <th>Provisionado</th>
                                <th>Empenhado</th>
                                <th>Liquidado</th>
                                <th>Pago</th>
                            </tr>
                            </thead>
                            <tbody>
                                <?php

                                $totalApresentado = 0;
                                $totalSelecionado = 0;
                                $totalAprovado = 0;
                                $totalProvisionado = 0;
                                $totalEmpenhado = 0;
                                $totalLiquidado = 0;
                                $totalPago = 0;
                                foreach($listaSubunidadeFnc as $subUnidadeFnc){
                                ?>
                                    <tr class="text-right">
                                        <td style="text-align: left">
                                            <?php echo $subUnidadeFnc['suocod']. ' - '. $subUnidadeFnc['suosigla']. ' - '. $subUnidadeFnc['suonome']; ?>
                                        </td>
                                        <td>
                                            <?php
                                                # Valor da coluna apresentado
                                                $apresentado = $modelPlanoInterno->buscarTotalFnc((object) array(
                                                    'exercicio' => $_SESSION['exercicio'],
                                                    'suoid' => $subUnidadeFnc['suoid'],
                                                    'esdid' => array(
                                                        ESD_FNC_PI_EM_ANALISE,
                                                        ESD_FNC_PI_BANCO_PROJETOS,
                                                        ESD_FNC_PI_DELIBERACAO_CFNC,
                                                        ESD_FNC_PI_AGUARDANDO_CORRECAO,
                                                        ESD_FNC_PI_SELECIONADO_CFNC,
                                                        ESD_FNC_PI_APROVADO
                                                    )
                                                ));
                                                $totalApresentado += $apresentado;
                                                echo $apresentado? number_format($apresentado, 2, ',', '.'): '-';
                                            ?>
                                        </td>
                                        <td>
                                            <?php
                                                # Valor da coluna selecionado
                                                $selecionado = $modelPlanoInterno->buscarTotalFnc((object) array(
                                                    'exercicio' => $_SESSION['exercicio'],
                                                    'suoid' => $subUnidadeFnc['suoid'],
                                                    'esdid' => array(
                                                        ESD_FNC_PI_SELECIONADO_CFNC,
                                                        ESD_FNC_PI_APROVADO
                                                    )
                                                ));
                                                $totalSelecionado += $selecionado;
                                                echo $selecionado? number_format($selecionado, 2, ',', '.'): '-';
                                            ?>
                                        </td>
                                        <td>
                                            <?php
                                                # Valor da coluna Aprovado
                                                $aprovado = $modelPlanoInterno->buscarTotalFnc((object) array(
                                                    'exercicio' => $_SESSION['exercicio'],
                                                    'suoid' => $subUnidadeFnc['suoid'],
                                                    'esdid' => array(ESD_FNC_PI_APROVADO)
                                                ));
                                                $totalAprovado += $aprovado;
                                                echo $aprovado? number_format($aprovado, 2, ',', '.'): '-';
                                            ?>
                                        </td>
                                        <td>
                                            <?php
                                                # Valor da coluna Provisionado
                                                $provisionado = $modelPlanoInterno->buscarTotalExecucaoFnc((object) array(
                                                    'tipo' => 'provisionado',
                                                    'exercicio' => $_SESSION['exercicio'],
                                                    'suoid' => $subUnidadeFnc['suoid'],
                                                ));
                                                $totalProvisionado += $provisionado;
                                                echo $provisionado? number_format($provisionado, 2, ',', '.'): '-';
                                            ?>
                                        </td>
                                        <td>
                                            <?php
                                                # Valor da coluna Empenhado
                                                $empenhado = $modelPlanoInterno->buscarTotalExecucaoFnc((object) array(
                                                    'tipo' => 'empenhado',
                                                    'exercicio' => $_SESSION['exercicio'],
                                                    'suoid' => $subUnidadeFnc['suoid'],
                                                ));
                                                $totalEmpenhado += $empenhado;
                                                echo $empenhado? number_format($empenhado, 2, ',', '.'): '-';
                                            ?>
                                        </td>
                                        <td>
                                            <?php
                                                # Valor da coluna Liquidado
                                                $liquidado = $modelPlanoInterno->buscarTotalExecucaoFnc((object) array(
                                                    'tipo' => 'liquidado',
                                                    'exercicio' => $_SESSION['exercicio'],
                                                    'suoid' => $subUnidadeFnc['suoid'],
                                                ));
                                                $totalLiquidado += $liquidado;
                                                echo $liquidado? number_format($liquidado, 2, ',', '.'): '-';
                                            ?>
                                        </td>
                                        <td>
                                            <?php
                                                # Valor da coluna Empenhado
                                                $pago = $modelPlanoInterno->buscarTotalExecucaoFnc((object) array(
                                                    'tipo' => 'pago',
                                                    'exercicio' => $_SESSION['exercicio'],
                                                    'suoid' => $subUnidadeFnc['suoid'],
                                                ));
                                                $totalPago += $pago;
                                                echo $pago? number_format($pago, 2, ',', '.'): '-';
                                            ?>
                                        </td>
                                    </tr>
                                <?php } ?>
                            </tbody>
                            <tfoot>
                                <tr class="text-right">
                                    <th style="text-align: right">TOTAL</th>
                                    <th><?php echo number_format($totalApresentado, 2, ',', '.'); ?></th>
                                    <th><?php echo number_format($totalSelecionado, 2, ',', '.'); ?></th>
                                    <th><?php echo number_format($totalAprovado, 2, ',', '.'); ?></th>
                                    <th><?php echo number_format($totalProvisionado, 2, ',', '.'); ?></th>
                                    <th><?php echo number_format($totalEmpenhado, 2, ',', '.'); ?></th>
                                    <th><?php echo number_format($totalLiquidado, 2, ',', '.'); ?></th>
                                    <th><?php echo number_format($totalPago, 2, ',', '.'); ?></th>
                                </tr>
                            </tfoot>
                        </table>
                    </div>
                </div>
            </div>
        </div>

        <!-- Lista de Funcionais FNC -->
        <div class="col-md-12">

            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5 class="col-lg-3">Acompanhamento Geral FNC - Por Ação</h5>
                    <a class="btn btn-primary" href="planacomorc.php?modulo=inicio&acao=C&req=exportar_xls_acao"><i class="fa fa-file-excel-o"></i> Exportar XLS</a>
                </div>
                <div class="ibox-content">
                    <div class="table-responsive">
                        <table class="table table-bordered table-hover" >
                            <thead>
                                <tr class="text-center">
                                    <th>Ação</th>
                                    <th>Dotação</th>
                                    <th>Apresentado</th>
                                    <th>Selecionado</th>
                                    <th>Aprovado</th>
                                    <th>Provisionado</th>
                                    <th>Limite</th>
                                    <th>Empenhado</th>
                                    <th>Liquidado</th>
                                    <th>Pago</th>
                                </tr>
                            </thead>
                            <tbody>
                            <?php
                                $total = null;
                                $funcionalAtual = '';
                                foreach($aDadosPorAcao as $dados){

                                    $total['dotacao'][$dados['tipo']] += $dados['dotacao'];

                                    $style = '';
                                    if($funcionalAtual != $dados['funcional']){
                                        $style = 'style="border-top: 2px #acacac solid"';
                                        $funcionalAtual = $dados['funcional'];
                                    }
                            ?>
                                <tr class="text-right" <?php echo $style; ?>>
                                    <td style="text-align: left"><?php echo $dados['funcional'] . ' (' . $dados['tipo'] . ') - ' . $dados['acatitulo'] . ': ' . $dados['plodsc'] . ' (RP ' . $dados['irpcod'] . ')'; ?></td>

                                    <td><?php echo number_format($dados['dotacao'], 2, ',', '.'); ?></td>
                                    <?php
                                        # VALOR APRESENTADO
                                        $valor = $aValoresEstado[$dados['ptrid']][ESD_FNC_PI_EM_ANALISE][$dados['tipo']] +
                                                 $aValoresEstado[$dados['ptrid']][ESD_FNC_PI_BANCO_PROJETOS][$dados['tipo']] +
                                                 $aValoresEstado[$dados['ptrid']][ESD_FNC_PI_DELIBERACAO_CFNC][$dados['tipo']] +
                                                 $aValoresEstado[$dados['ptrid']][ESD_FNC_PI_AGUARDANDO_CORRECAO][$dados['tipo']] +
                                                 $aValoresEstado[$dados['ptrid']][ESD_FNC_PI_SELECIONADO_CFNC][$dados['tipo']] +
                                                 $aValoresEstado[$dados['ptrid']][ESD_FNC_PI_APROVADO][$dados['tipo']];
                                        $total['APRESENTADO'][$dados['tipo']] += $valor;
                                        $disponivel = $dados['dotacao'] - $valor;
                                    ?>
                                    <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>;">
                                        <?php echo $valor ? number_format($valor, 2, ',', '.') : '-'; ?>
                                    </td>
                                    <?php
                                        # VALOR SELECIONADO
                                        $valor = $aValoresEstado[$dados['ptrid']][ESD_FNC_PI_SELECIONADO_CFNC][$dados['tipo']] +
                                                 $aValoresEstado[$dados['ptrid']][ESD_FNC_PI_APROVADO][$dados['tipo']];

                                        $total[ESD_FNC_PI_SELECIONADO_CFNC][$dados['tipo']] += $valor;
                                        $disponivel = $dados['dotacao'] - $valor;
                                    ?>
                                    <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>;">
                                        <?php echo $valor ? number_format($valor, 2, ',', '.') : '-'; ?>
                                    </td>
                                    <?php
                                        # VALOR APROVADO
                                        $valor = !empty($aValoresEstado[$dados['ptrid']][ESD_FNC_PI_APROVADO][$dados['tipo']]) ? $aValoresEstado[$dados['ptrid']][ESD_FNC_PI_APROVADO][$dados['tipo']] : '0';
                                        $total[ESD_FNC_PI_APROVADO][$dados['tipo']] += $valor;
                                        $disponivel = $dados['dotacao'] - $valor;
                                    ?>
                                    <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>;">
                                        <?php echo $valor ? number_format($valor, 2, ',', '.') : '-'; ?>
                                    </td>
                                    <td> - </td>
                                    <td> - </td>
                                    <td>
                                        <?php
                                            # VALOR EMPENHADO
                                            $valor = !empty($aValoresExecucao[$dados['ptrid']][$dados['tipo']]['vlrempenhado']) ? $aValoresEstado[$dados['ptrid']][$dados['tipo']]['vlrempenhado'] : '0';
                                            $total['vlrempenhado'][$dados['tipo']] += $valor;
                                            echo $valor ? number_format($valor, 2, ',', '.') : '-';
                                        ?>
                                    </td>
                                    <td>
                                        <?php
                                            # VALOR LIQUIDADO
                                            $valor = !empty($aValoresExecucao[$dados['ptrid']][$dados['tipo']]['vlrliquidado']) ? $aValoresEstado[$dados['ptrid']][$dados['tipo']]['vlrliquidado'] : '0';
                                            $total['vlrliquidado'][$dados['tipo']] += $valor;
                                            echo $valor ? number_format($valor, 2, ',', '.') : '-';
                                        ?>
                                    </td>
                                    <td>
                                        <?php
                                            # VALOR PAGO
                                            $valor = !empty($aValoresExecucao[$dados['ptrid']][$dados['tipo']]['vlrpago']) ? $aValoresEstado[$dados['ptrid']][$dados['tipo']]['vlrpago'] : '0';
                                            $total['vlrpago'][$dados['tipo']] += $valor;
                                            echo $valor ? number_format($valor, 2, ',', '.') : '-';
                                        ?>
                                    </td>
                                </tr>
                            <?php } ?>
                            </tbody>
                            <tfoot>
                                <tr class="text-right" style="border-top: 2px #acacac solid">
                                    <td style="text-align: left">TOTAL CUSTEIO</td>
                                    <td><?php echo number_format($total['dotacao']['CUSTEIO'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($total['APRESENTADO']['CUSTEIO'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($total[ESD_FNC_PI_SELECIONADO_CFNC]['CUSTEIO'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($total[ESD_FNC_PI_APROVADO]['CUSTEIO'], 2, ',', '.'); ?></td>
                                    <td> - </td>
                                    <td> - </td>
                                    <td><?php echo number_format($total['vlrempenhado']['CUSTEIO'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($total['vlrliquidado']['CUSTEIO'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($total['vlrpago']['CUSTEIO'], 2, ',', '.'); ?></td>
                                </tr>
                                <tr class="text-right">
                                    <td style="text-align: left">TOTAL CAPITAL</td>
                                    <td><?php echo number_format($total['dotacao']['CAPITAL'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($total['APRESENTADO']['CAPITAL'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($total[ESD_FNC_PI_SELECIONADO_CFNC]['CAPITAL'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($total[ESD_FNC_PI_APROVADO]['CAPITAL'], 2, ',', '.'); ?></td>
                                    <td> - </td>
                                    <td> - </td>
                                    <td><?php echo number_format($total['vlrempenhado']['CAPITAL'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($total['vlrliquidado']['CAPITAL'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($total['vlrpago']['CAPITAL'], 2, ',', '.'); ?></td>
                                </tr>
                                <tr class="text-right">
                                    <th style="text-align: left">TOTAL</th>
                                    <th><?php echo number_format(array_sum($total['dotacao']), 2, ',', '.'); ?></th>
                                    <th><?php echo number_format(array_sum($total['APRESENTADO']), 2, ',', '.'); ?></th>
                                    <th><?php echo number_format(array_sum($total[ESD_FNC_PI_SELECIONADO_CFNC]), 2, ',', '.'); ?></th>
                                    <th><?php echo number_format(array_sum($total[ESD_FNC_PI_APROVADO]), 2, ',', '.'); ?></th>
                                    <th> - </th>
                                    <th> - </th>
                                    <th><?php echo number_format(array_sum($total['vlrempenhado']), 2, ',', '.'); ?></th>
                                    <th><?php echo number_format(array_sum($total['vlrliquidado']), 2, ',', '.'); ?></th>
                                    <th><?php echo number_format(array_sum($total['vlrpago']), 2, ',', '.'); ?></th>
                                </tr>
                            </tfoot>
                        </table>
                    </div>
                </div>
            </div>
        </div>
        
    </div>
</div>

<script>

    $(document).ready(function(){

        $('#btn-atualizar-siop').click(function(){
            atualizarSIOP();
        });

    });

</script>
