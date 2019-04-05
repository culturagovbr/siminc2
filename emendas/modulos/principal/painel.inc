<?php

# Busca perfis do usuário logado
$perfis = pegaPerfilGeral();

# Caso seja o perfil de SubUnidade o sistema redireciona para a tela de lista de emendas.
if(count(array_intersect($perfis, [PFL_SUBUNIDADE]))){
    # Todos os perfis são redirecionados para a nova tela de Painel
    $urlPainel = 'emendas.php?modulo=inicio&acao=C';
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

$mEmendas = new Emendas_Model_Emenda();

include_once APPRAIZ . "includes/library/simec/Grafico.php";
include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-4">
        <h2>Painel de Acompanhamento</h2>
    </div>
    <div class="col-lg-8">
        <div style="padding-top: 10px;" class="text-right">
            <a class="btn btn-danger" href="?modulo=inicio&acao=C">
                <i class="fa fa-list"></i> Lista de Emendas
            </a>
        </div>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Valores por GND</h5>
                </div>
                <div class="ibox-content">
                    <?php $mEmendas->gerarGraficoGnd($_SESSION['exercicio'], 1); ?>
                </div>
            </div>
        </div>
        <div class="col-md-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Valores por Gênero</h5>
                </div>
                <div class="ibox-content">
                    <?php $mEmendas->gerarGraficoSexo($_SESSION['exercicio'], 1); ?>
                </div>
            </div>
        </div>
        <div class="col-md-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Valores por GND</h5>
                </div>
                <div class="ibox-content">
                    <?php $mEmendas->gerarGraficoGnd($_SESSION['exercicio']); ?>
                </div>
            </div>
        </div>
        <div class="col-md-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Valores por Gênero</h5>
                </div>
                <div class="ibox-content">
                    <?php $mEmendas->gerarGraficoSexo($_SESSION['exercicio']); ?>
                </div>
            </div>
        </div>
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Valores por Unidade Federativa</h5>
                </div>
                <div class="ibox-content">
                    <?php $mEmendas->gerarGraficoUf($_SESSION['exercicio']); ?>
                </div>
            </div>
        </div>
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Valores por Partido</h5>
                </div>
                <div class="ibox-content">
                    <?php $mEmendas->gerarGraficoPartido($_SESSION['exercicio']); ?>
                </div>
            </div>
        </div>
    </div>
</div>