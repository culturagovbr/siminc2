<?php

include_once APPRAIZ . "includes/library/simec/Grafico.php";

require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

# Lista de perfis vinculados ao usuário logado
$perfis = pegaPerfilGeral();

if(count(array_intersect($perfis, [PFL_SUBUNIDADE]))){
    # Perfil unidade são redirecionados para a tela de lista de Pré-Pis
    $urlPainel = 'proposta.php?modulo=principal/preplanointerno&acao=A';
    if($_REQUEST['exercicio']){
        $urlPainel .= '&exercicio='. $_REQUEST['exercicio'];
    }
    if($_REQUEST['req']){
        $urlPainel .= '&req='. $_REQUEST['req'];
    }
    if($_REQUEST['pliid']){
        $urlPainel .= '&pliid='. $_REQUEST['pliid'];
    }
    simec_redirecionar($urlPainel);
}

$mProposta = new Proposta_Model_Proposta();
$mPtres = new Monitora_Model_Ptres();
$mPrePlanoInterno = new Proposta_Model_PrePlanoInterno();

# Busca último momento atualizado
$ultimoMomento = $mPtres->buscarUltimoMomentoAtualizado((int)$_SESSION['exercicio']);

$momento = $_REQUEST['momento']? $_REQUEST['momento']: ($ultimoMomento? $ultimoMomento: 1000);

switch ($_REQUEST['req']) {
    case 'gerar-planilha':
        $mProposta->gerarPlanilhaImportacao($momento);
        die;
    case 'carregarGraficoUnidade':
        $mPrePlanoInterno->carregarGrafico(NULL, [], TRUE);
        die;
    case 'carregarGraficoDireta':
        $mPrePlanoInterno->carregarGrafico(NULL, ['unosigla' => 'MINC', 'unidades'=>"suocod not in ('420009', '420008')"]);
        die;
    case 'carregarGraficoCgconCogep':
        $mPrePlanoInterno->carregarGrafico(NULL, ['unosigla' => 'MINC', 'unidades'=>"suocod in ('420009', '420008')"]);
        die;
    case 'importar-siop':
        $mPtres->importarSiop($_SESSION['exercicio'], $momento);
    case 'atualizar-sistema':
        $mPtres->importarSiop($_SESSION['exercicio'], $momento);
}

$quantidadeSiop = $mPtres->recuperarQuantidadesSiop((int)$momento, (int)$_SESSION['exercicio']);
$funcionaisSiop = $mPtres->listarFuncionaisSiop((int)$momento, (int)$_SESSION['exercicio']);
$comparacaoSiop = $mPtres->recuperarComparacaoSiop((int)$momento, (int)$_SESSION['exercicio']);

/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-8">
        <h2>Painel de Acompanhamento de envio de Proposta ao SIOP</h2>
    </div>
    <div class="col-lg-4">
        <div style="padding-top: 10px;" class="text-right">
            <a class="btn btn-danger fa fa-table" href="?modulo=principal/painel_siop&acao=A&req=gerar-planilha"> Gerar Planilha</a>
        </div>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">

        <div class="col-md-5">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Funcionais SIOP</h5>
                </div>
                <div class="ibox-content">
                    <form name='form_importar' id='form_importar' method="POST" action="?modulo=principal/painel_siop&acao=A&req=importar-siop" class="form-horizontal">
                        <?php
                            echo $simec->select(
                                'momento',
                                'Momento',
                                $momento,
                                (new Wssof_Ws_MomentosDto())->recuperarSqlCombo(),
                                ['required']);
                        ?>
                        <div class="form-group">
                            <div class="text-center">
                                <a class="btn btn-warning" id="btn-funcionais-siop"><i class="fa fa-search"></i>&nbsp;Ver Funcionais</a>
                                <button class="btn btn-danger fa fa-download" type="submit" id="btn-importar">&nbsp;Importar</button>
                            </div>
                        </div>
                    </form>
                    <div class="table-responsive">
                        <table class="table table-bordered table-hover" >
                            <thead>
                            <tr class="text-center">
                                <th>Carga</th>
                                <th>Quantidade</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($quantidadeSiop as $dado){ ?>
                                <tr>
                                    <td><?php echo $dado['descricao']; ?></td>
                                    <td align="right"><?php echo $dado['qtd']; ?></td>
                                </tr>
                            <?php } ?>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>

        <div class="col-md-3">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Comparativo SIOP/Propostas SIMINC</h5>
                </div>
                <div class="ibox-content">

                    <div class="table-responsive">
                        <table class="table table-bordered table-hover" >
                            <thead>
                            <tr class="text-center">
                                <?php foreach($comparacaoSiop as $dado){ ?>
                                    <th><?php echo $dado['descricao']; ?></th>
                                <?php } ?>
                            </tr>
                            </thead>
                            <tbody>
                            <tr>
                                <?php foreach($comparacaoSiop as $dado){ ?>
                                    <td  align="right"><?php echo $dado['qtd']; ?></td>
                                <?php } ?>
                            </tr>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>

        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Valores por Unidades</h5>
                </div>
                <div class="ibox-content">
                    <div class="tabs-container tabs">
                        <ul class="nav nav-tabs prodTabs">
                            <li class="active"><a data-toggle="tab" href="#tab-1" data-url="?modulo=principal/painel_siop&acao=A&req=carregarGraficoUnidade"> Gráficos das Unidades</a></li>
                            <li class=""><a data-toggle="tab" href="#tab-2" data-url="?modulo=principal/painel_siop&acao=A&req=carregarGraficoDireta"> Gráficos da Direta</a></li>
                            <li class=""><a data-toggle="tab" href="#tab-3" data-url="?modulo=principal/painel_siop&acao=A&req=carregarGraficoCgconCogep"> Gráficos da CGCON/COGEP</a></li>
                        </ul>
                        <div class="tab-content">
                            <div id="tab-1" class="tab-pane active">
                                <?php $mPrePlanoInterno->carregarGrafico(NULL, [], TRUE); ?>
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

    </div>
</div>

<!-- Modal -->
<div class="modal fade" id="modalFuncionaisSiop" tabindex="-1" role="dialog" aria-labelledby="myModalLabel">
    <div class="modal-dialog modal-lg" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                <h4 class="modal-title" id="myModalLabel">Funcionais do SIOP</h4>
            </div>
            <div class="modal-body">
                <div class="table-responsive">
                    <table class="table table-bordered table-hover" >
                        <thead>
                        <tr class="text-center">
                            <th>Unidade</th>
                            <th>Ação</th>
                            <th>Programa</th>
                            <th>Localizador</th>
                            <th>Objetivo</th>
                            <th>PO</th>
                            <th>Título Ação</th>
                            <th>Título PO</th>
                        </tr>
                        </thead>
                        <tbody>
                        <?php foreach($funcionaisSiop as $dado){ ?>
                            <tr>
                                <td><?php echo $dado['codigoorgao']; ?></td>
                                <td><?php echo $dado['codigoacao']; ?></td>
                                <td><?php echo $dado['codigoprograma']; ?></td>
                                <td><?php echo $dado['codigolocalizador']; ?></td>
                                <td><?php echo $dado['codigoobjetivo']; ?></td>
                                <td><?php echo $dado['planoorcamentario']; ?></td>
                                <td><?php echo $dado['acatitulo']; ?></td>
                                <td><?php echo $dado['plotitulo']; ?></td>
                            </tr>
                        <?php } ?>
                        </tbody>
                    </table>
                </div>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>

<script>

    $(document).ready(function(){

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

        $('#btn-funcionais-siop').click(function(){
            $('#modalFuncionaisSiop').modal();
        });
        
        $('#momento').change(function(){
            window.document.location = '?modulo=principal/painel_siop&acao=A&momento='+ $(this).val();
        });

    });

</script>

