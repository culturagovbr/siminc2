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
$mPtres = new Planacomorc_Model_Ptres();
$mPrePlanoInterno = new Proposta_Model_PrePlanoInterno();

# Busca último momento atualizado
$ultimoMomento = $mPtres->buscarUltimoMomentoAtualizado((int)$_SESSION['exercicio']);

$momento = $_REQUEST['momento']? $_REQUEST['momento']: ($ultimoMomento? $ultimoMomento: 1000);

$mProposta->gerarPlanilhaImportacao($momento);
die;
