<?php

include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

$oPlanoInterno = new Pi_PlanoInterno();

switch ($_REQUEST['req']) {
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

$aDadosPorAcao = $oPlanoInterno->recuperarDadosFncPorAcao($_SESSION['exercicio']);
$aValoresEstado = $oPlanoInterno->recuperarValoresPorEstado($_SESSION['exercicio']);
$aValoresExecucao = $oPlanoInterno->recuperarValoresPorExecucao($_SESSION['exercicio']);

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
                            $funcionalAtual = '';
                            $aDadosUnidade = $oPlanoInterno->recuperarDadosFncPorUnidade($_SESSION['exercicio']);
                            $aValoresUnidadeEstado = $aDadosUnidade['estado'];
                            $aValoresUnidadeExecucao = $aDadosUnidade['execucao'];

                            foreach($aDadosUnidade['unidade'] as $dados){

                                $style = '';
                                if($funcionalAtual != $dados['funcional']){
                                    $style = 'style="border-top: 2px #acacac solid"';
                                    $funcionalAtual = $dados['funcional'];
                                }
                                ?>
                                <tr class="text-right" <?php echo $style; ?>>
                                    <td style="text-align: left"><?php echo $dados['suocod'] . ' - ' . $dados['suosigla'] . ' - ' . $dados['suonome']; ?></td>
                                        <?php
                                            # VALOR APRESENTADO
                                            $valor = $aValoresUnidadeEstado[$dados['suoid']][ESD_FNC_PI_EM_ANALISE] +
                                                     $aValoresUnidadeEstado[$dados['suoid']][ESD_FNC_PI_BANCO_PROJETOS] +
                                                     $aValoresUnidadeEstado[$dados['suoid']][ESD_FNC_PI_DELIBERACAO_CFNC] +
                                                     $aValoresUnidadeEstado[$dados['suoid']][ESD_FNC_PI_AGUARDANDO_CORRECAO] +
                                                     $aValoresUnidadeEstado[$dados['suoid']][ESD_FNC_PI_SELECIONADO_CFNC] +
                                                     $aValoresUnidadeEstado[$dados['suoid']][ESD_FNC_PI_APROVADO];
                                            $total['APRESENTADO'] += $valor;
                                        ?>
                                    <td>
                                        <?php echo $valor ? number_format($valor, 2, ',', '.') : '-'; ?>
                                    </td>
                                        <?php
                                            # VALOR SELECIONADO
                                            $valor = $aValoresUnidadeEstado[$dados['suoid']][ESD_FNC_PI_SELECIONADO_CFNC] +
                                                     $aValoresUnidadeEstado[$dados['suoid']][ESD_FNC_PI_APROVADO];

                                            $total[ESD_FNC_PI_SELECIONADO_CFNC] += $valor;
                                        ?>
                                    <td>
                                        <?php echo $valor ? number_format($valor, 2, ',', '.') : '-'; ?>
                                    </td>
                                        <?php
                                            # VALOR APRESENTADO
                                            $valor = !empty($aValoresUnidadeEstado[$dados['suoid']][ESD_FNC_PI_APROVADO]) ? $aValoresUnidadeEstado[$dados['suoid']][ESD_FNC_PI_APROVADO] : '0';
                                            $total[ESD_FNC_PI_APROVADO] += $valor;
                                        ?>
                                    <td>
                                        <?php echo $valor ? number_format($valor, 2, ',', '.') : '-'; ?>
                                    </td>
                                    <td> - </td>
                                    <td>
                                        <?php
                                            # VALOR EMPENHADO
                                            $valor = !empty($aValoresUnidadeExecucao[$dados['suoid']]['vlrempenhado']) ? $aValoresUnidadeEstado[$dados['suoid']]['vlrempenhado'] : '0';
                                            $total['vlrempenhado'] += $valor;
                                            echo $valor ? number_format($valor, 2, ',', '.') : '-';
                                        ?>
                                    </td>
                                    <td>
                                        <?php
                                            # VALOR LIQUIDADO
                                            $valor = !empty($aValoresUnidadeExecucao[$dados['suoid']]['vlrliquidado']) ? $aValoresUnidadeEstado[$dados['suoid']]['vlrliquidado'] : '0';
                                            $total['vlrliquidado'] += $valor;
                                            echo $valor ? number_format($valor, 2, ',', '.') : '-';
                                        ?>
                                    </td>
                                    <td>
                                        <?php
                                            # VALOR PAGO
                                            $valor = !empty($aValoresUnidadeExecucao[$dados['suoid']]['vlrpago']) ? $aValoresUnidadeEstado[$dados['suoid']]['vlrpago'] : '0';
                                            $total['vlrpago'] += $valor;
                                            echo $valor ? number_format($valor, 2, ',', '.') : '-';
                                        ?>
                                    </td>
                                </tr>
                            <?php } ?>
                            </tbody>
                            <tfoot>
                                <tr class="text-right">
                                    <th style="text-align: left">TOTAL</th>
                                    <th><?php echo number_format(($total['APRESENTADO']), 2, ',', '.'); ?></th>
                                    <th><?php echo number_format(($total[ESD_FNC_PI_SELECIONADO_CFNC]), 2, ',', '.'); ?></th>
                                    <th><?php echo number_format(($total[ESD_FNC_PI_APROVADO]), 2, ',', '.'); ?></th>
                                    <th> - </th>
                                    <th><?php echo number_format(($total['vlrempenhado']), 2, ',', '.'); ?></th>
                                    <th><?php echo number_format(($total['vlrliquidado']), 2, ',', '.'); ?></th>
                                    <th><?php echo number_format(($total['vlrpago']), 2, ',', '.'); ?></th>
                                </tr>
                            </tfoot>
                        </table>
                    </div>
                </div>
            </div>
        </div>

        <!-- Lista de Funcionais FNC -->

    </div>
</div>

<script>

    $(document).ready(function(){

        $('#btn-atualizar-siop').click(function(){
            atualizarSIOP();
        });

    });

</script>
