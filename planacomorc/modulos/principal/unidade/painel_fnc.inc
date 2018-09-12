<?php

include_once APPRAIZ . "includes/library/simec/Grafico.php";

include_once '_funcoespi.php';

require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

# Verifica se o usuário tem vinculo de Sub-Unidades no seu Perfil
$listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));

$oPlanoInterno = new Pi_PlanoInterno();

switch ($_REQUEST['req']) {
    case 'detalhe-subunidade':
        $oPlanoInterno->recuperarPiPorSubunidade($_REQUEST['suoid'], 'fnc');
        die;
    case 'detalhe-funcional':
        $oPlanoInterno->recuperarPiPorFuncional($_REQUEST['ptrid'], 'fnc');
        die;
    case 'atualizarSIOP':
        $cImportaDadosSiop = new Planacomorc_Controller_ImportaDadosSiop();
        $cImportaDadosSiop->AtualizarDados();
        $cImportaDadosSiop->AtualizarDotacao();
        die;        
}

$filtros = array(
    "suo.prsano = '". $_SESSION['exercicio']. "'",
    'suo.unofundo = TRUE',
);
# Filtro por Subunidades do perfil do usuário logado.
if($listaSubUnidadeUsuario){
    $filtros[] = "suo.suocod IN('". join("','", $listaSubUnidadeUsuario). "')";
}
$aDadosPorAcao = $oPlanoInterno->recuperarDadosFncPorAcao($_SESSION['exercicio']);
$aValoresEstado = $oPlanoInterno->recuperarValoresPorEstado($_SESSION['exercicio']);
$aValoresExecucao = $oPlanoInterno->recuperarValoresPorExecucao($_SESSION['exercicio']);

include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-4">
        <h2 style="color: red;">Painel de Acompanhamento FNC</h2>
    </div>
    <div class="col-lg-8">
        <div style="padding-top: 10px;" class="text-right">
            <?php if((in_array(PFL_ADMINISTRADOR, $perfis)) || (in_array(PFL_SUPERUSUARIO, $perfis))) { ?>
            <a class="btn btn-success" href="javascript:AtualizarSIOP();">
                <i class="fa fa-cloud-download"></i> Atualizar SIOP
            </a>
            <?php } ?>
            <a class="btn btn-success" href="planacomorc.php?modulo=inicio&acao=C"><i class="fa fa-area-chart"></i> Painel</a>
            <a class="btn btn-danger" href="planacomorc.php?modulo=principal/unidade/listapimanter_fnc&acao=A"><i class="fa fa-list"></i> Lista de PI - FNC</a>
            <?php if(!array_intersect($perfis, [PFL_CONSULTA, PFL_CONSULTA_UNIDADE])){ ?>
            <a class="btn btn-warning" href="planacomorc.php?modulo=principal/unidade/cadastro_pi_fnc&acao=A"><i class="fa fa-plus-circle"></i> Criar PI - FNC</a>
          <? }?>
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

    function AtualizarSIOP(){
        divCarregando();
        $.post("planacomorc.php?modulo=inicio&acao=C&req=atualizarSIOP",
            function(data){
                 divCarregado();
                 if(data == ''){
                    swal({
                        title: "",
                        text: "Dados Atualizados!",
                        type: "success",
                        showCancelButton: false,
                        confirmButtonText: "OK",
                        closeOnConfirm: true
                    },
                    function(){
                        window.location.href = "planacomorc.php?modulo=principal/unidade/painel_fnc&acao=A";
                    });
                }else{
                    swal('Erro!', 'Houve algum erro na importação de dados do SIOP!', 'warning');
                }
            }
        );
    }

</script>