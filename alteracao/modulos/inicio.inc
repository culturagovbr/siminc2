<?php
    
    $permiteEditar=true; //Flag para não mostrar botões de editar e excluir na listagem de pedidos.
    
    if($_REQUEST['req'] == 'espelho-pedido'){
        include_once APPRAIZ.'alteracao/modulos/principal/popup-espelho-pedido.inc';
        die;
    }

    include_once APPRAIZ. "includes/library/simec/Grafico.php";

    require_once APPRAIZ. 'includes/workflow.php';

    # Lista de perfis vinculados ao usuário logado
    $perfis = pegaPerfilGeral();

    # Filtro por Subunidades do perfil do usuário logado.
    $listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));

    # Filtro de Situação na sessão do usuário
    $filtroalteracao = $_SESSION['filtroalteracao'] = (isset($_REQUEST['esdid']) ? $_REQUEST : $_SESSION['filtroalteracao']);
    $requisicao = $_REQUEST['requisicao'];
    $filtroalteracao['listaSubUnidadeUsuario'] = $listaSubUnidadeUsuario;

    if($requisicao){
        switch ($requisicao) {
            case 'limpar':
                $filtroalteracao = $_SESSION['filtroalteracao'] = array();
            break;
            case 'buscar_pedidos':
                $aAlteracao = (new Alteracao_Model_Pedido())->recuperarListagem($_REQUEST);
            break;
            case 'excluir_pedido':
                $cPedido = new Alteracao_Controller_Pedido();
                $cPedido->excluir($_REQUEST['pedid']);
            die;
        }
    }
    $filtroalteracao['tpdid'] = $_REQUEST['tpdid'] ? $_REQUEST['tpdid'] : WF_TPDID_INTERNO;
    $podeEditarAlteracao = TRUE;

    /**
     * Cabeçalho padrão do sistema.
     * @see cabecalho.inc
     */
    include APPRAIZ. "includes/cabecalho.inc";
?>
<script src="js/principal/alteracao.js?v=1"></script>
<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-8">
        <h2>Painel de Acompanhamento de Pedidos de Alteração</h2>
    </div>
    <div class="col-lg-4">
        <div style="padding-top: 10px;" class="text-right">
            <button id="btn-exportar-xls" class="btn btn-primary btn-buscar" type="button">
                <i class="fa fa-file-excel-o"></i> Exportar XLS
            </button>
        </div>
    </div>
</div>
<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">

                <div class="tabs-container tabs">
                    <ul class="nav nav-tabs prodTabs">
                        <?php
                            if ($_REQUEST['tpdid']==WF_TPDID_INTERNO || !$_REQUEST['tpdid']){
                                $activeInterno = 'active';
                                $activeExterno = '';
                            }else{
                                $activeInterno = '';
                                $activeExterno = 'active';
                            }
                        ?>
                        <li class="<?=$activeInterno?>"><a class="tab-tipo" data-toggle="tab" id="tp_interno" href="#tab-interno" tipo="<?= WF_TPDID_INTERNO ?>">Interno</a></li>
                        <li class="<?=$activeExterno?>"><a class="tab-tipo" data-toggle="tab" id="tp_externo" href="#tab-externo" tipo="<?= WF_TPDID_EXTERNO ?>">Externo</a></li>
                    </ul>
                    <div class="ibox-title">
                        <h5>Pesquisa avançada por Pedidos de Altera&ccedil;&atilde;o</h5>
                    </div>
                    <div class="tab-content">
                        <div id="tab-interno" class="tab-pane <?=$activeInterno?>">
                            <?php
                                if ($_REQUEST['tpdid']==WF_TPDID_INTERNO || !$_REQUEST['tpdid']){
                                    $tpdid = WF_TPDID_INTERNO;
                                    include(APPRAIZ . "alteracao/modulos/principal/filtro_pedido_credito.inc");
                                }
                            ?>
                        </div>
                        
                        <div id="tab-externo" class="tab-pane <?=$activeExterno?>">
                            <?php 
                                if ($_REQUEST['tpdid']==WF_TPDID_EXTERNO){
                                    $tpdid = WF_TPDID_EXTERNO;
                                    include(APPRAIZ . "alteracao/modulos/principal/filtro_pedido_credito.inc");
                                }
                            ?>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">
                    <div class="" id="div-principal">
                        <?php include_once APPRAIZ.'alteracao/modulos/principal/lista_incio.inc'; ?>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<script>

    $(document).ready(function(){
        initListaAlteracao();
    });

</script>
