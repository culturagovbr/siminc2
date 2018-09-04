<?php

include_once APPRAIZ . "includes/library/simec/Grafico.php";

require_once APPRAIZ . 'includes/workflow.php';

# Lista de perfis vinculados ao usuário logado
$perfis = pegaPerfilGeral();

# Filtro por Subunidades do perfil do usuário logado.
$listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));

# Filtro de Situação na sessão do usuário
$filtroPainelProposta = $_SESSION['filtroPainelProposta'] = (isset($_REQUEST['esdid'])? $_REQUEST: $_SESSION['filtroPainelProposta']);

if(count(array_intersect($perfis, [PFL_SUBUNIDADE])) && !$listaSubUnidadeUsuario){
    # Perfil unidade s<E3>o redirecionados para a tela de lista de Pr<E9>-Pis
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


$mPrePlanoInterno = new Proposta_Model_PrePlanoInterno();

# Filtros padrões da consulta
$filtros = array(
    "suo.prsano = '". $_SESSION['exercicio']. "'",
    'suo.unofundo = FALSE',
);

//ver($listaSubUnidadeUsuario);
if($listaSubUnidadeUsuario){
    $filtros[] = "suo.suocod IN('". join("','", $listaSubUnidadeUsuario). "')";
}
# Define se a tela vai mostrar os dados Custeio e Capital normal ou de Custeio e Capital Expansão
$tipo = $_REQUEST['tipo']?$_REQUEST['tipo']:'Normal';

switch ($_REQUEST['req']) {
    case 'lista-inicio-xls':
        $sql = $mPrePlanoInterno->montarSqlExecucaoOrcamentaria($filtros, $tipo, $filtroPainelProposta['esdid']);
        $listaFuncional = $db->carregar($sql);
        include_once APPRAIZ. "proposta/modulos/lista-inicio-xls.inc";
    case 'carregarGraficoUnidade':
        $mPrePlanoInterno->carregarGrafico($_REQUEST['tipo'], null, true, $filtroPainelProposta['esdid']);
    die;
    case 'carregarGraficoDireta':
        $mPrePlanoInterno->carregarGrafico($_REQUEST['tipo'], ['unosigla' => 'MINC', 'unidades' => "suocod NOT IN('". SUOCOD_CGCON. "', '". SUOCOD_COGEP. "')"], true, $filtroPainelProposta['esdid']);
    die;
    case 'carregarGraficoCgconCogep':
        $mPrePlanoInterno->carregarGrafico($_REQUEST['tipo'], ['unosigla' => 'MINC', 'unidades' => "suocod IN('". SUOCOD_CGCON. "', '". SUOCOD_COGEP. "')"], true, $filtroPainelProposta['esdid']);
    die;
    case 'trocaAbaGrafico':
        include_once(APPRAIZ . "proposta/modulos/principal/montaGrafico".$_REQUEST['tipo'].".inc");
    die;        
    case 'carregarListaAcompanhamentoPis':
        $aPropostas = $mPrePlanoInterno->recuperarExecucaoOrcamentaria($filtros, $tipo, $filtroPainelProposta['esdid']);
        include_once(APPRAIZ . "proposta/modulos/principal/listaAcompanhamentoPI.inc");
    die;
}

$aPropostas = $mPrePlanoInterno->recuperarExecucaoOrcamentaria($filtros, $tipo, $filtroPainelProposta['esdid']);

/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

?>
<style>
.gray-bg {
    background-color: #ffffff;
}
.wrapper.wrapper-content.animated.fadeInRight {
    background: #e5e5e5;
}
</style>
<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-8">
        <h2>Painel de Acompanhamento
            <?php
                echo $simec->radio(
                    'esdid',
                    null,
                    ($filtroPainelProposta['esdid']? $filtroPainelProposta['esdid']: ''),
                    array(
                        '' => "Todos",
                        ESD_PREPI_APROVADO => "Aprovados"
                    )
                );
            ?>
        </h2>
    </div>
    <div class="col-lg-4">
        <div style="padding-top: 10px;" class="text-right">
            <?php if(!array_intersect($perfis, [PFL_CONSULTA])){ ?>
                <button id="btn-exportar-xls" class="btn btn-primary btn-buscar" type="button">
                    <i class="fa fa-file-excel-o"></i> Exportar XLS
                </button>
            <?php }?>
        </div>
    </div>
</div>
<input type="hidden" id="tipo" value="Normal">
<div class="tabs-container tabs">
    <ul class="nav nav-tabs prodTabs">
        <li class="active"><a data-toggle="tab" href="#tab-normal" tipo="Normal" data-url="?modulo=inicio&acao=C&req=trocaAbaGrafico&tipo=Normal"> Normal</a></li>
        <li class=""><a data-toggle="tab" href="#tab-expansao" tipo="Expansao" data-url="?modulo=inicio&acao=C&req=trocaAbaGrafico&tipo=Expansao"> Expansão</a></li>
    </ul>
    <div class="tab-content">
        <div id="tab-normal" class="tab-pane active">
            <?php include_once(APPRAIZ . "proposta/modulos/principal/montaGraficoNormal.inc");?>                      
        </div>
        <div id="tab-expansao" class="tab-pane">
            <?php include_once(APPRAIZ . "proposta/modulos/principal/montaGraficoExpansao.inc");?>   
        </div>
    </div>
</div>

<div class="clearfix" style="margin-top: 10px;">&nbsp;</div>

<div id="listaAcompanhamentoPis">
    <?php include_once(APPRAIZ . "proposta/modulos/principal/listaAcompanhamentoPI.inc");?>
</div>

<script>
    $(document).ready(function(){

        $('.tabs').on('click', '.tablink, .prodTabs a',function (e) {
            e.preventDefault();
            var url = $(this).attr("data-url");
            var tipo = $(this).attr("tipo");
            $("#tipo").val(tipo);
            $('#listaAcompanhamentoPis').load('?modulo=inicio&acao=C&req=carregarListaAcompanhamentoPis&tipo='+tipo);
            if (typeof url !== "undefined") {
                var pane = $(this), href = this.hash;

                // ajax load from data-url
                $(href).load(url, function(result){
                    pane.tab('show');
                });
            } else {
                $(this).tab('show');
            }
        });
        
        $("#btn-exportar-xls").click(function(){
            window.open('?modulo=inicio&acao=C&req=lista-inicio-xls&tipo='+ $("#tipo").val());
        });
        
        $("input[name='esdid']").change(function(){
            window.document.location = window.document.location+ '&esdid='+ $(this).val();
        });
        
    });

</script>
