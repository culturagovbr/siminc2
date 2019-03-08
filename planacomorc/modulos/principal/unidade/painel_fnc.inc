<?php

    include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";
    require_once APPRAIZ . 'includes/php-excel/PHPExcel.php';

    $modelPlanoInterno = new Pi_PlanoInterno();

    switch($_REQUEST['req']){
        case 'exportar_xls_unidade':
            $modelPlanoInterno->exportarXlsUnidade((int)$_SESSION['exercicio']);
            die;
        case 'exportar_xls_acao':
            $modelPlanoInterno->exportarXlsAcao((int)$_SESSION['exercicio']);
            die;
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
    
    # Busca Subunidades do FNC
    $listaSubunidadeFnc = $modelPlanoInterno->listarSubunidadeFnc((object)array('exercicio' => $_SESSION['exercicio']));
    
    # Busca Funcionais do FNC
    $listaFuncionais = $modelPlanoInterno->listarFuncionaisFnc((object)array('exercicio' => $_SESSION['exercicio']));
    
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
            <a class="btn btn-success" href="planacomorc.php?modulo=inicio&acao=C" title="Painel de Acompanhamento">
                <i class="fa fa-area-chart"></i> Painel
            </a>
            <a class="btn btn-danger" href="planacomorc.php?modulo=principal/unidade/listapimanter_fnc&acao=A" title="Listagem de PA do Fundo Nacional de Cultura">
                <i class="fa fa-list"></i> Lista de PA - FNC
            </a>
            <?php if(!array_intersect($perfis, [PFL_CONSULTA, PFL_CONSULTA_UNIDADE])): ?>
                <a class="btn btn-warning" href="planacomorc.php?modulo=principal/unidade/cadastro_pi_fnc&acao=A" title="Novo cadastro de PA do Fundo Nacional de Cultura">
                    <i class="fa fa-plus-circle"></i> Criar PA - FNC
                </a>
            <?php endif; ?>
            <a class="btn btn-primary" href="planacomorc.php?modulo=relatorio/geral&acao=A&fundo=t" title="Relatório Geral em Planilha apenas dos PAs do Fundo Nacional de Cultura">
                <i class="fa fa-file-excel-o"></i> Relatório Geral
            </a>
        </div>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">

        <div class="col-md-12">

            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5 class="col-lg-3">Acompanhamento Geral FNC - Por Unidade</h5>
                    <a class="btn btn-primary" href="planacomorc.php?modulo=principal/unidade/painel_fnc&acao=A&req=exportar_xls_unidade"><i class="fa fa-file-excel-o"></i> Exportar XLS</a>
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

                                if($listaSubunidadeFnc):
                                    foreach($listaSubunidadeFnc as $subUnidadeFnc):?>
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
                                    <?php endforeach; ?>
                                <?php endif; ?>
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
                    <a class="btn btn-primary" href="planacomorc.php?modulo=principal/unidade/painel_fnc&acao=A&req=exportar_xls_acao"><i class="fa fa-file-excel-o"></i> Exportar XLS</a>
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
                                    <th>Empenhado</th>
                                    <th>Liquidado</th>
                                    <th>Pago</th>
                                </tr>
                            </thead>
                            <tbody>
                            <?php
                                $totalDotacaoCusteio = 0;
                                $totalDotacaoCapital = 0;
                                # Previsto/Planejado/Aberto em PI
                                $totalApresentadoCusteio = 0;
                                $totalApresentadoCapital = 0;
                                $totalSelecionadoCusteio = 0;
                                $totalSelecionadoCapital = 0;
                                $totalAprovadoCusteio = 0;
                                $totalAprovadoCapital = 0;
                                # Execução Orçamentária
                                $totalProvisionadoCusteio = 0;
                                $totalProvisionadoCapital = 0;
                                $totalEmpenhadoCusteio = 0;
                                $totalEmpenhadoCapital = 0;
                                $totalLiquidadoCusteio = 0;
                                $totalLiquidadoCapital = 0;
                                $totalPagoCusteio = 0;
                                $totalPagoCapital = 0;
                                # Formatação da lista
                                $funcionalAnterior = NULL;


                                if($listaFuncionais):
                                    foreach($listaFuncionais as $funcional):
                                        $style = NULL;
                                        if($funcionalAnterior != $funcional['funcional']){
                                            $style = 'style="border-top: 2px #acacac solid"';
                                            $funcionalAnterior = $funcional['funcional'];
                                        }
                                    ?>
                                    <tr class="text-right" <?php echo $style; ?>>
                                        <td style="text-align: left">
                                            <?php echo $funcional['funcional']. ' ('. $funcional['tipo']. ') - '. $funcional['acatitulo']. ': '. $funcional['plodsc']. ' (RP '. $funcional['irpcod']. ')'; ?>
                                        </td>
                                        <td>
                                            <?php echo number_format($funcional['dotacao'], 2, ',', '.');

                                                if($funcional['tipo'] == 'CUSTEIO'){
                                                    $totalDotacaoCusteio += $funcional['dotacao'];
                                                } else {
                                                    $totalDotacaoCapital += $funcional['dotacao'];
                                                }

                                                # Valor da coluna apresentado
                                                $apresentado = $modelPlanoInterno->buscarTotalFnc((object) array(
                                                    'tipo' => $funcional['tipo'],
                                                    'exercicio' => $_SESSION['exercicio'],
                                                    'ptrid' => $funcional['ptrid'],
                                                    'esdid' => array(
                                                        ESD_FNC_PI_EM_ANALISE,
                                                        ESD_FNC_PI_BANCO_PROJETOS,
                                                        ESD_FNC_PI_DELIBERACAO_CFNC,
                                                        ESD_FNC_PI_AGUARDANDO_CORRECAO,
                                                        ESD_FNC_PI_SELECIONADO_CFNC,
                                                        ESD_FNC_PI_APROVADO
                                                    )
                                                ));
                                                if($funcional['tipo'] == 'CUSTEIO'){
                                                    $totalApresentadoCusteio += $apresentado;
                                                } else {
                                                    $totalApresentadoCusteio += $apresentado;
                                                }
                                                $disponivel = $funcional['dotacao'] - $apresentado;
                                            ?>
                                        </td>
                                        <td style="color: #<?php echo $modelPlanoInterno->getCorPainelPorValorDisponivel((int)$disponivel); ?>;">
                                            <?php echo $apresentado? number_format($apresentado, 2, ',', '.'): '-';

                                                # Valor da coluna selecionado
                                                $selecionado = $modelPlanoInterno->buscarTotalFnc((object) array(
                                                    'tipo' => $funcional['tipo'],
                                                    'exercicio' => $_SESSION['exercicio'],
                                                    'ptrid' => $funcional['ptrid'],
                                                    'esdid' => array(
                                                        ESD_FNC_PI_SELECIONADO_CFNC,
                                                        ESD_FNC_PI_APROVADO
                                                    )
                                                ));
                                                if($funcional['tipo'] == 'CUSTEIO'){
                                                    $totalSelecionadoCusteio += $selecionado;
                                                } else {
                                                    $totalSelecionadoCapital += $selecionado;
                                                }
                                                $disponivel = $funcional['dotacao'] - $selecionado;
                                            ?>
                                        </td>
                                        <td style="color: #<?php echo $modelPlanoInterno->getCorPainelPorValorDisponivel((int)$disponivel); ?>;">
                                            <?php echo $selecionado? number_format($selecionado, 2, ',', '.'): '-';

                                               # Valor da coluna aprovado
                                                $aprovado = $modelPlanoInterno->buscarTotalFnc((object) array(
                                                    'tipo' => $funcional['tipo'],
                                                    'exercicio' => $_SESSION['exercicio'],
                                                    'ptrid' => $funcional['ptrid'],
                                                    'esdid' => array(
                                                        ESD_FNC_PI_APROVADO
                                                    )
                                                ));
                                                if($funcional['tipo'] == 'CUSTEIO'){
                                                    $totalAprovadoCusteio += $aprovado;
                                                } else {
                                                    $totalAprovadoCapital += $aprovado;
                                                }
                                                $disponivel = $funcional['dotacao'] - $aprovado;
                                            ?>
                                        </td>
                                        <td style="color: #<?php echo $modelPlanoInterno->getCorPainelPorValorDisponivel((int)$disponivel); ?>;">
                                            <?php echo $aprovado? number_format($aprovado, 2, ',', '.'): '-';

                                                # Valor da coluna Provisionado
                                                $provisionado = $modelPlanoInterno->buscarTotalExecucaoFnc((object) array(
                                                    'tipo' => 'provisionado',
                                                    'exercicio' => $_SESSION['exercicio'],
                                                    'ptrid' => $funcional['ptrid'],
                                                    'gnd' => $funcional['tipo']
                                                ));

                                                if($funcional['tipo'] == 'CUSTEIO'){
                                                    $totalProvisionadoCusteio += $provisionado;
                                                } else {
                                                    $totalProvisionadoCapital += $provisionado;
                                                }
                                                $disponivel = $funcional['dotacao'] - $provisionado;
                                            ?>
                                        </td>
                                        <td style="color: #<?php echo $modelPlanoInterno->getCorPainelPorValorDisponivel((int)$disponivel); ?>;">
                                            <?php echo $provisionado? number_format($provisionado, 2, ',', '.'): '-';

                                                # Valor da coluna Empenhado
                                                $empenhado = $modelPlanoInterno->buscarTotalExecucaoFnc((object) array(
                                                    'tipo' => 'empenhado',
                                                    'exercicio' => $_SESSION['exercicio'],
                                                    'ptrid' => $funcional['ptrid'],
                                                    'gnd' => $funcional['tipo']
                                                ));

                                                if($funcional['tipo'] == 'CUSTEIO'){
                                                    $totalEmpenhadoCusteio += $empenhado;
                                                } else {
                                                    $totalEmpenhadoCapital += $empenhado;
                                                }
                                                $disponivel = $funcional['dotacao'] - $empenhado;
                                            ?>
                                        </td>
                                        <td style="color: #<?php echo $modelPlanoInterno->getCorPainelPorValorDisponivel((int)$disponivel); ?>;">
                                            <?php echo $empenhado? number_format($empenhado, 2, ',', '.'): '-';

                                                # Valor da coluna Liquidado
                                                $liquidado = $modelPlanoInterno->buscarTotalExecucaoFnc((object) array(
                                                    'tipo' => 'liquidado',
                                                    'exercicio' => $_SESSION['exercicio'],
                                                    'ptrid' => $funcional['ptrid'],
                                                    'gnd' => $funcional['tipo']
                                                ));

                                                if($funcional['tipo'] == 'CUSTEIO'){
                                                    $totalLiquidadoCusteio += $liquidado;
                                                } else {
                                                    $totalLiquidadoCapital += $liquidado;
                                                }
                                                $disponivel = $funcional['dotacao'] - $liquidado;
                                            ?>
                                        </td>
                                        <td style="color: #<?php echo $modelPlanoInterno->getCorPainelPorValorDisponivel((int)$disponivel); ?>;">
                                            <?php echo $liquidado? number_format($liquidado, 2, ',', '.'): '-';

                                                # Valor da coluna Pago
                                                $pago = $modelPlanoInterno->buscarTotalExecucaoFnc((object) array(
                                                    'tipo' => 'pago',
                                                    'exercicio' => $_SESSION['exercicio'],
                                                    'ptrid' => $funcional['ptrid'],
                                                    'gnd' => $funcional['tipo']
                                                ));

                                                if($funcional['tipo'] == 'CUSTEIO'){
                                                    $totalPagoCusteio += $pago;
                                                } else {
                                                    $totalPagoCapital += $pago;
                                                }
                                                $disponivel = $funcional['dotacao'] - $pago;
                                            ?>
                                        </td>
                                        <td style="color: #<?php echo $modelPlanoInterno->getCorPainelPorValorDisponivel((int)$disponivel); ?>;">
                                            <?php echo $pago? number_format($pago, 2, ',', '.'): '-'; ?>
                                        </td>
                                    </tr>
                                <?php endforeach; ?>
                            <?php endif; ?>
                            </tbody>
                            <tfoot>
                                <tr class="text-right" style="border-top: 2px #acacac solid">
                                    <td style="text-align: left">TOTAL CUSTEIO</td>
                                    <td><?php echo number_format($totalDotacaoCusteio, 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($totalApresentadoCusteio, 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($totalSelecionadoCusteio, 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($totalAprovadoCusteio, 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($totalProvisionadoCusteio, 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($totalEmpenhadoCusteio, 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($totalLiquidadoCusteio, 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($totalPagoCusteio, 2, ',', '.'); ?></td>
                                </tr>
                                <tr class="text-right">
                                    <td style="text-align: left">TOTAL CAPITAL</td>
                                    <td><?php echo number_format($totalDotacaoCapital, 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($totalApresentadoCapital, 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($totalSelecionadoCapital, 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($totalAprovadoCapital, 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($totalProvisionadoCapital, 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($totalEmpenhadoCapital, 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($totalLiquidadoCapital, 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($totalPagoCapital, 2, ',', '.'); ?></td>
                                </tr>
                                <tr class="text-right">
                                    <th style="text-align: left">TOTAL</th>
                                    <th><?php echo number_format($totalDotacaoCusteio + $totalDotacaoCapital, 2, ',', '.'); ?></th>
                                    <th><?php echo number_format($totalApresentadoCusteio + $totalApresentadoCapital, 2, ',', '.'); ?></th>
                                    <th><?php echo number_format($totalSelecionadoCusteio + $totalSelecionadoCapital, 2, ',', '.'); ?></th>
                                    <th><?php echo number_format($totalAprovadoCusteio + $totalAprovadoCapital, 2, ',', '.'); ?></th>
                                    <th><?php echo number_format($totalProvisionadoCusteio + $totalProvisionadoCapital, 2, ',', '.'); ?></th>
                                    <th><?php echo number_format($totalEmpenhadoCusteio + $totalEmpenhadoCapital, 2, ',', '.'); ?></th>
                                    <th><?php echo number_format($totalLiquidadoCusteio + $totalLiquidadoCapital, 2, ',', '.'); ?></th>
                                    <th><?php echo number_format($totalPagoCusteio + $totalPagoCapital, 2, ',', '.'); ?></th>
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
