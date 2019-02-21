<?php
            
include_once APPRAIZ . "includes/library/simec/Grafico.php";

include_once '_funcoespi.php';

require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

# Cores dos Graficos
$colors = "'#FFD700', '#FF0000', '#55BF3B', '#eeaaee', '#00BFFF', '#aaeeee', '#7798BF', '#DDDF0D', '#7CCD7C', '#DF5353', '#008000', '#CD0000', '#FF4500', '#ff0066', '#4B0082', '#808000', '#800000', '#2F4F4F', '#006400', '#FFA500'";

# Verifica se o usuário tem vinculo de Sub-Unidades no seu Perfil
$listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));

$prsano = $_REQUEST['exercicio']?$_REQUEST['exercicio']:$_SESSION['exercicio'];
$oPlanoInterno = new Pi_PlanoInterno();

switch ($_REQUEST['req']) {
    case 'detalhe-subunidade':
        $oPlanoInterno->recuperarPiPorSubunidade($_REQUEST['suoid']);
        die;
    case 'espelho-pi':
        include APPRAIZ. 'planacomorc/modulos/principal/unidade/espelho-pi.inc';
        die;
    case 'detalhe-funcional':
        $oPlanoInterno->recuperarPiPorFuncional((object) array('suoid' => $_REQUEST['suoid'], 'ptrid' => $_REQUEST['ptrid']));
        die;
    case 'detalhe-grafico-subunidade':
        $oPlanoInterno->recuperarDetalhesValoresGrafico(TRUE, $_REQUEST['tipo_valor'], $_REQUEST['sigla'], true);
        die;
    case 'detalhe-grafico-uo':
        $oPlanoInterno->recuperarDetalhesValoresGrafico(FALSE, $_REQUEST['tipo_valor'], $_REQUEST['sigla']);
        die;
    case 'carregarGraficoUnidade':
        carregarGraficoUnidade($colors, true);
        die;
    case 'carregarGraficoDireta':
        carregarGraficoDireta($colors, true);
        die;
    case 'carregarGraficoCgconCogep':
        carregarGraficoCgconCogep($colors, true);
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

$filtros = array(
    "suo.prsano = '". $prsano. "'",
    'suo.unofundo = FALSE',
);
# Filtro por Subunidades do perfil do usuário logado.
if($listaSubUnidadeUsuario){
    $filtros[] = "suo.suocod IN('". join("','", $listaSubUnidadeUsuario). "')";
}

$aPropostas = $oPlanoInterno->recuperarExecucaoOrcamentaria($filtros);

/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

?>
<script src="js/unidade/lista-pi-manter.js?v=5"></script>
<script src="js/painel.js?v=2"></script>
<style>
    .detalhe-subunidade, .detalhe-funcional{
        cursor: pointer;
    }

    .title-disponivel{
        cursor: help;
    }

    .linha_subunidade {
        background-color: rgba(241, 246, 215, 0.3) !important;
        font-weight: bold;
    }
</style>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-4">
        <h2>Painel de Acompanhamento</h2>
    </div>
    <div class="col-lg-8">
        <div style="padding-top: 10px;" class="text-right">
            <?php if((in_array(PFL_ADMINISTRADOR, $perfis)) || (in_array(PFL_SUPERUSUARIO, $perfis))) { ?>
            <a id="btn-atualizar-siop" class="btn btn-success">
                <i class="fa fa-cloud-download"></i> Atualizar SIOP
            </a>
            <?php } ?>
            <a class="btn btn-success" href="planacomorc.php?modulo=principal/unidade/painel_fnc&acao=A">
                <i class="fa fa-area-chart"></i> Painel FNC
            </a>
            <a class="btn btn-danger" href="planacomorc.php?modulo=principal/unidade/listapimanter&acao=A">
                <i class="fa fa-list"></i> Lista de PI
            </a>
            <?php if(!array_intersect($perfis, [PFL_CONSULTA, PFL_CONSULTA_UNIDADE])){ ?>
                <a class="btn btn-warning" href="planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A">
                    <i class="fa fa-plus-circle"></i> Criar PI
                </a>
            <?php }?>
            <a class="btn btn-primary" href="planacomorc.php?modulo=relatorio/geral&acao=A&fundo=f">
                <i class="fa fa-file-excel-o"></i> Relatório Geral
            </a>
        </div>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">

        <?php if (!$listaSubUnidadeUsuario): ?>
            <div class="col-md-12">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Valores por Unidades</h5>
                    </div>
                    <div class="ibox-content">
                        <div class="tabs-container tabs">
                            <ul class="nav nav-tabs prodTabs">
                                <li class="active"><a data-toggle="tab" href="#tab-1" data-url="?modulo=inicio&acao=C&req=carregarGraficoUnidade"> Gráficos das Unidades</a></li>
                                <li class=""><a data-toggle="tab" href="#tab-2" data-url="?modulo=inicio&acao=C&req=carregarGraficoDireta"> Gráficos da Direta</a></li>
                                <li class=""><a data-toggle="tab" href="#tab-3" data-url="?modulo=inicio&acao=C&req=carregarGraficoCgconCogep"> Gráficos da CGCON/COGEP</a></li>
                            </ul>
                            <div class="tab-content">
                                <div id="tab-1" class="tab-pane active">
                                    <!--O grafico 1 será carregado aqui via ajax-->
                                    <?php carregarGraficoUnidade($colors, true); ?>
                                </div>
                                <div id="tab-2" class="tab-pane">
                                    <!--O grafico 2 será carregado aqui via ajax-->
                                </div>
                                <div id="tab-3" class="tab-pane">
                                    <!--O grafico 3 será carregado aqui via ajax-->
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        <?php else: ?>
            <div class="col-md-12">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Valores por Unidades</h5>
                    </div>
                    <div class="ibox-content">
                        <div class="tabs-container">
                            <div class="tab-content">
                                <div class="panel-body">
                                    <?php
                                    $codigoUnidadesWhere = "'". join($listaSubUnidadeUsuario, "','"). "'";
                                    $estatistica = $oPlanoInterno->recuperarEstatisticaPagamentoDetalhe((object) array(
                                        'exercicio' => $_SESSION['exercicio'],
                                        'unofundo' => 'FALSE',
                                        'unidades' => "suocod IN(". $codigoUnidadesWhere. ")"));
                                    $grafico = new Grafico(Grafico::K_TIPO_COLUNA, false);
                                    $grafico
                                        ->setWidth('85%')
                                        ->setColors($colors)
                                        ->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)
                                        ->setEvent(array('click' => "exibirModalDetalheGrafico(1, event.point.series.name, event.point.category);"))
                                        ->gerarGrafico($estatistica);
                                    ?>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        <?php endif; ?>

        <div class="col-md-12">

            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Acompanhamento de Funcionais</h5>
                </div>
                <div class="ibox-content">
                    <div class="table-responsive">
                        <table class="table table-bordered table-hover" >
                            <thead>
                            <tr class="text-center">
                                <th rowspan="2" style="border-right: 1px black solid;">
                                    <i class="fa fa-plus detalhar-todos link"></i>&nbsp;&nbsp;Unidade
                                </th>
                                <th colspan="2" style="border-right: 1px black solid;">Custeio</th>
                                <th colspan="2" style="border-right: 1px black solid;">Capital</th>
                                <th colspan="2" style="border-right: 1px black solid;">Total</th>
                                <th rowspan="2" style="border-right: 1px black solid;">Limite</th>
                            </tr>
                            <tr class="text-center">
                                <th>Dotação</th>
                                <th style="border-right: 1px black solid;">Planejado</th>
                                <th>Dotação</th>
                                <th style="border-right: 1px black solid;">Planejado</th>
                                <th>Dotação</th>
                                <th style="border-right: 1px black solid;">Planejado</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($aPropostas as $dados){
                                $dado = $dados['detalhe'][0];
                                ?>
                                <tr class="linha_subunidade">
                                    <td style="border-right: 1px black solid;">
                                        <i class="fa fa-plus detalhar-linha link" data-suoid="<?php echo $dado['suoid']; ?>"></i>&nbsp;&nbsp;
                                        <span class="detalhe-subunidade" data-suoid="<?php echo $dado['suoid']; ?>" title="Visualizar PIs dessa Unidade">
                                            <?php echo $dado['unosigla'] . ' - ' . $dado['suonome'] . ' (' . $dado['suocod'] . ')'; ?>
                                        </span>
                                    </td>

                                    <td class="text-right"><?php echo number_format($dados['dotacaoCusteio'], 0, ',', '.'); ?></td>
                                    <?php $disponivel = $dados['dotacaoCusteio'] - $dados['planejadoCusteio']; ?>
                                    <td style="color: #<?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                        <?php echo number_format($dados['planejadoCusteio'], 0, ',', '.'); ?>
                                    </td>

                                    <td class="text-right"><?php echo number_format($dados['dotacaoCapital'], 0, ',', '.'); ?></td>
                                    <?php $disponivel = $dados['dotacaoCapital'] - $dados['planejadoCapital']; ?>
                                    <td style="color: #<?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                        <?php echo number_format($dados['planejadoCapital'], 0, ',', '.'); ?>
                                    </td>

                                    <td class="text-right"><?php echo number_format(($dados['dotacaoCusteio'] + $dados['dotacaoCapital']), 0, ',', '.'); ?></td>
                                    <?php $disponivel = ($dados['dotacaoCusteio'] + $dados['dotacaoCapital']) - $dados['totalPlanejado']; ?>
                                    <td style="color: #<?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                        <?php echo number_format($dados['totalPlanejado'], 0, ',', '.'); ?>
                                    </td>

                                    <?php $disponivel = $dado['lmuvlr'] - $dados['totalPlanejado']; ?>
                                    <td style="color: #<?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                        <?php echo number_format($dado['lmuvlr'], 0, ',', '.'); ?>
                                    </td>
                                </tr>
                                <?php foreach($dados['detalhe'] as $dado){ ?>
                                    <tr class="tr_<?php echo $dado['suoid']; ?> tr_detalhe" style="display: none;">
                                        <td style="padding-left: 30px; border-right: 1px black solid;" >
                                            <span class="detalhe-funcional" data-ptrid="<?php echo $dado['ptrid']; ?>" data-suoid="<?php echo $dado['suoid']; ?>" title="Visualizar lista de PIs com essa funcional">
                                                <?php echo $dado['funcional'] . ' - ' . $dado['acatitulo'] . ': ' . $dado['plodsc'] . ' (RP ' . $dado['irpcod'] . ') (PTRES:'.$dado['ptres'].')'; ?>
                                            </span>
                                            <?php if($dado['compartilhada'] > 1): ?>
                                                &nbsp;<i class="fa fa-handshake-o link" title="Funcional Compartilhada com outras Subunidades."></i>
                                            <?php endif; ?>
                                        </td>

                                        <td class="text-right"><?php echo number_format($dado['ptrdotacao_custeio'], 0, ',', '.'); ?></td>
                                        <?php $disponivel = $dado['ptrdotacao_custeio'] - $dado['picvalorcusteio']; ?>
                                        <td style="color: #<?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                            <?php echo number_format($dado['picvalorcusteio'], 0, ',', '.'); ?>
                                        </td>

                                        <td class="text-right"><?php echo number_format($dado['ptrdotacao_capital'], 0, ',', '.'); ?></td>
                                        <?php $disponivel = $dado['ptrdotacao_capital'] - $dado['picvalorcapital']; ?>
                                        <td style="color: #<?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                            <?php echo number_format($dado['picvalorcapital'], 0, ',', '.'); ?>
                                        </td>

                                        <td class="text-right"><?php echo number_format(($dado['ptrdotacao_custeio'] + $dado['ptrdotacao_capital']), 0, ',', '.'); ?></td>
                                        <?php $disponivel = ($dado['ptrdotacao_custeio'] + $dado['ptrdotacao_capital']) - ($dado['picvalorcusteio'] + $dado['picvalorcapital']); ?>
                                        <td style="color: #<?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                            <?php echo number_format(($dado['picvalorcusteio'] + $dado['picvalorcapital']), 0, ',', '.'); ?>
                                        </td>
                                        <td class="text-center" style="border-right: 1px black solid;"> - </td>
                                    </tr>
                                <?php } ?>
                            <?php } ?>
                            </tbody>
                        </table>
                    </div>

                </div>
            </div>
        </div>
    </div>
</div>

<?php
    // -- popup de detalhamento do PI
    bootstrapPopup("PI's da Unidade", 'detalhe-subunidade', '', array('fechar'), array('tamanho' => 'lg'));
    bootstrapPopup("PI's da Funcional na Unidade", 'detalhe-funcional', '', array('fechar'), array('tamanho' => 'lg'));
    bootstrapPopup("Detalhe da Unidade", 'detalhe-grafico', '', array('fechar'), array('tamanho' => 'lg'));
?>

<script>
    $(document).ready(function(){

        initLinkEspelhoPi();

        $('#btn-atualizar-siop').click(function(){
            atualizarSIOP();
        });

        $('.tabs').on('click', '.tablink, .prodTabs a',function (e) {
            e.preventDefault();
            var url = $(this).attr("data-url");

            if (typeof url !== "undefined") {
                var pane = $(this), href = this.hash;

                // ajax load from data-url
                $(href).empty().load(url, function(result){
                    pane.tab('show');
                });
            } else {
                $(this).tab('show');
            }
        });
        
        $('.detalhe-subunidade').click(function(){
            $('#detalhe-subunidade .modal-body').load('planacomorc.php?modulo=inicio&acao=C&req=detalhe-subunidade&suoid='+ $(this).data('suoid'));
            $('#detalhe-subunidade').modal();
        });

        $('.detalhe-funcional').click(function(){
            $('#detalhe-funcional .modal-body').load('planacomorc.php?modulo=inicio&acao=C&req=detalhe-funcional&ptrid='+ $(this).data('ptrid')+ '&suoid='+ $(this).data('suoid'));
            $('#detalhe-funcional').modal();
        });

        $('.detalhar-linha').click(function(){

            var suoid = $(this).data('suoid');

            if($(this).hasClass('fa-minus')){
                $(this).removeClass('fa-minus').addClass('fa-plus');
                $('.tr_' + suoid).hide();
            } else {
                $(this).removeClass('fa-plus').addClass('fa-minus');
                $('.tr_' + suoid).show('slow');
            }
        });

        $('.detalhar-todos').click(function(){

            if($(this).hasClass('fa-minus')){
                $(this).removeClass('fa-minus').addClass('fa-plus');
                $('.detalhar-linha').removeClass('fa-minus').addClass('fa-plus');
                $('.tr_detalhe').hide();
            } else {
                $(this).removeClass('fa-plus').addClass('fa-minus');
                $('.detalhar-linha').removeClass('fa-plus').addClass('fa-minus');
                $('.tr_detalhe').show('slow');
            }
        });
    });

</script>
