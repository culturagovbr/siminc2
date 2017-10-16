<?php

    /**
     * Tela de listagem de PI cadastrados no sistema.
     *
     * $Id: listapimanter.inc 102359 2015-09-11 18:26:07Z maykelbraz $
     *
     * @filesource
     */

    $requisicao = $_REQUEST['requisicao'];
    $filtropi = $_REQUEST;
    
    $sqlListaUO = Spo_Model_Unidade::queryCombo((object)array(
        'prsano::INTEGER = '. (int)$_SESSION['exercicio'],
        'suo.unofundo = TRUE'));

    $listaUO = $db->carregar($sqlListaUO);
    
    # Verifica se o usu�rio tem vinculo de Sub-Unidades no seu Perfil
    $listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));
    
    if($requisicao) {
        switch ($requisicao) {
            case 'exibirGrafico':
                include_once APPRAIZ . "planacomorc/modulos/principal/unidade/grafico-pi-dotacao.inc";
                die();
            break;
            case 'carregarComboUG':
                echo montarComboUG((object) array(
                    'ungcod' => $filtropi['ungcod'],
                    'exercicio' => $_SESSION['exercicio'],
                    'unicod' => $_REQUEST['unicod'],
                    'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario
                ));
                die();
            break;
            case 'detalharPiDelegadas':
                include_once APPRAIZ . "planacomorc/modulos/principal/unidade/sub-unidades-delegadas.inc";
                die();
            break;
        }
    }
    
    /**
     * Monta a combo de UGs filtrando por UO
     * 
     * @param $filtros 
     * @return VOID
     */
    function montarComboUG(stdClass $filtros) {
        global $simec;

        return $simec->select(
            'ungcod',
            'Sub-Unidade',
            $filtros->ungcod,
            Public_Model_SubUnidadeOrcamentaria::queryCombo((object) array(
                'exercicio' => $filtros->exercicio,
                'unicod' => $filtros->unicod,
                'listaSubUnidadeUsuario' => $filtros->listaSubUnidadeUsuario)),
            array('multiple' => 'true'));
    }

    /**
     * Cabe�alho padr�o do sistema.
     */
    include APPRAIZ . "includes/cabecalho.inc";
?>

<script src="js/unidade/lista-pi-manter.js"></script>
<style>
/*    .tabela-listagem {
        font-size: 1em !important;
    }*/
</style>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Pesquisa avan�ada por PI do FNC</h5>
                </div>
                <div class="ibox-content">
                    <form id="filtropi" name="filtropi" method="POST" role="form" class="form-horizontal">
                        <input name="" id="filtropi_requisicao" value="" type="hidden">

                        <?php
                            echo $simec->input('plicod', 'C�digo do PI', $filtropi['plicod'], ['maxlength' => 11]);
                            # Se o usu�rio tiver Sub-Unidades vinculadas ao seu perfil o sistema n�o mostra op��o de selecionar UO.
                            if(!$listaSubUnidadeUsuario) {
                                echo $simec->select('unicod', 'Unidade', current($listaUO), $sqlListaUO, array('multiple' => 'true'));
                            }
                            echo '<div id=div_ungcod >';
                            echo $simec->select('ungcod', 'Sub-Unidade', $filtropi['ungcod'], Public_Model_SubUnidadeOrcamentaria::queryCombo((object) array(
                                'unofundo' => 'TRUE',
                                'exercicio' => $_SESSION['exercicio'],
                                'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)), array('multiple' => 'true'));
                            echo '</div>';
                            echo $simec->select('ptres', 'PTRES', $filtropi['ptres'], Spo_Model_Ptres::queryCombo((object) array(
                                'exercicio' => $_SESSION['exercicio'],
                                'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)), array('multiple' => 'true'));
                            echo $simec->input('descricao', 'T�tulo/Descri��o', $filtropi['descricao'], ['maxlength' => 200]);
                            echo $simec->select('eqdid', 'Enquadramento da Despesa', $filtropi['eqdid'], Spo_Model_Ptres::queryComboEnquadramento((object) array('exercicio' => $_SESSION['exercicio'])), array('multiple' => 'true'));
                            echo $simec->radio('pliemenda', 'Emenda', (!$filtropi['pliemenda'] ? '' : $filtropi['pliemenda']), array('t' => 'Sim', 'f' => 'N�o', '' => 'Todos'));
                            echo $simec->select('esdid', 'Situa��o', $filtropi['esdid'], Spo_Model_Unidade::queryComboEstados((object) array()), array('multiple' => 'true'));
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-warning btn-limpar" type="reset">
                                    <span class="glyphicon glyphicon-remove-circle"></span> Limpar
                                </button>
                                <button class="btn btn-primary btn-buscar" type="submit">
                                    <span class="glyphicon glyphicon-search"></span> Buscar
                                </button>
                                <button class="btn btn-success btn-novo-fnc" type="button">
                                    <span class="glyphicon glyphicon-plus"></span>
                                    Novo
                                </button>
                            </div>
                        </div>
                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>
    
<div class="row col-md-12" style="margin-bottom: 30px;">
    <?php

        # Lista de PIs
        $filtropi['exercicio'] = $_SESSION['exercicio'];
        $filtropi['usucpf'] = $_SESSION['usucpf'];
        $filtropi['unofundo'] = 'TRUE';
        $list = new Simec_Listagem();
        $list->setQuery(Spo_Model_Planointerno::listar((object) $filtropi))
            ->turnOnPesquisator()
            ->setFormFiltros('filtropi')
//            ->esconderColunas('unidsc', 'valido')
            ->setCabecalho(array(
                'id', 'C�digo', 'Sub-Unidade', 'T�tulo', 'Funcional', 'Situa��o', 'Or�amento do PI (R$)' => array(
                    'Custeio', 'Capital', 'Empenhado', 'Pago', 'Liquidado'
                )))
            ->addCallbackDeCampo('id', 'exibirLinkEspelho')
//            ->addCallbackDeCampo('unicod', 'alinharUOEsquerda')
//            ->addCallbackDeCampo('plititulo', 'formatarTituloPI')
//            ->addCallbackDeCampo('obrid', 'formatarObrid')
//            ->addCallbackDeCampo('delegadas', 'exibirIconeDelegadas')
//            ->addCallbackDeCampo('cadastramento', 'formatarCadastramento')
            ->addCallbackDeCampo(array('custeio', 'capital', 'total', 'empenhado', 'pago', 'liquidado'), 'mascaraMoeda')
            ->addAcao('edit', 'alterarPiFnc')
//            ->setAcaoComoCondicional('edit', array(
//                array('campo' => 'obrid', 'valor' => 'N/A', 'op' => 'igual'),
//                array('campo' => 'cadastramento', 'valor' => Spo_Model_Planointerno::CADASTRADO_SIAFI, 'op' => 'diferente')
//            ))
            ->addAcao('delete', 'removerPi')
//            ->setAcaoComoCondicional('delete', array(array(
//                'campo' => 'cadastramento', 'valor' => Spo_Model_Planointerno::CADASTRADO_SIAFI, 'op' => 'diferente'
//            )))
//            ->addAcao('chart', 'exibirGrafico')
//            ->addAcao('view', 'exibirEspelhoPi')
//            ->setAcaoComoCondicional('view', array(array(
//                'campo' => 'cadastramento', 'valor' => Spo_Model_Planointerno::CADASTRADO_SIAFI, 'op' => 'diferente'
//            )))
            ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

        # Popup de detalhamento do PI
        bootstrapPopup('Dados do Plano Interno', 'detalhepi', '', array('fechar'), array('tamanho' => 'lg'));

        # Popup com lista de Unidades delegadas
        bootstrapPopup('Dados do Plano Interno', 'detalhePiDelegadas', '', array('fechar'), array('tamanho' => 'lg'));
    ?>
</div>

<script type="text/javascript">
    $(document).ready(function(){
        initListaPiManter();
    });
</script>