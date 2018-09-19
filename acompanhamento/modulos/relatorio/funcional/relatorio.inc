<?php

    # Incluindo arquivo do Módulo que possui a implementação da busca
    include_once APPRAIZ. "monitora/classes/Pi_PlanoInterno.class.inc";

    # Incluindo o arquivo da classe de relatorio
    include_once APPRAIZ. 'includes/relatorio-agrupador/tabela.php';

//    # Busca SQL com filtros pra realizar a consulta
//    $sql = montarSqlRelatorioGeralPrePi((object) array(
//        'exercicio' => $_POST['filtro']['prsano'],
//        'suoid' => $_POST['filtro']['suoid'],
//        'irpcod' => $_POST['filtro']['irpcod'],
//        'esdid' => $_POST['filtro']['esdid']
//    ));
    
    # Busca SQL com filtros pra realizar a consulta
    $modelPlanoInterno = new Pi_PlanoInterno();
    $filtros = array(
        'suo.unofundo = FALSE',
        "suo.prsano = '". $_SESSION['exercicio']. "'"
    );
    $sql = $modelPlanoInterno->montarSqlRelFuncionais($filtros);
    
    # Busca lista pra montar a tabela do relatório
    $listaRelatorio = $db->carregar($sql);
    
    # Colunas do relatorio
    $listaTodasColunas = $_SESSION['relatorio_gerencial_funcional']['listaTodasColunas'];
    $listaColunasSelecionadas = $_POST['coluna'];
    $listaColunaFormatoMoeda = [
        'ptrdotacao_capital',
        'ptrdotacao_custeio',
        'picvalorcapital',
        'picvalorcusteio',
        'provisionado',
        'empenhado',
        'liquidado',
        'pago',
    ];

    $relatorio = new relatorio_agrupador_tabela();
    $tabela = $relatorio->setListaTodasColunas($listaTodasColunas)
        ->setListaColunasSelecionadas($listaColunasSelecionadas)
        ->setListaColunaFormatoMoeda($listaColunaFormatoMoeda)
        ->setListaRelatorio($listaRelatorio)
        ->montarTabela()
        ->getTabela();
    
    # Gera o XLS do relatório
    if($_REQUEST['req'] == 'xls'){
        echo $tabela;
        
        header('Content-Type: application/vnd.ms-excel');
        header('Expires: 0');
        header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
        header('Content-Disposition: attachment; filename="relatorio_gerencial_pre_pi.xls"');
        die();
    }

    include APPRAIZ. "includes/cabecalho_bootstrap_v3_popup.inc";
?>

<div class="row" style="margin-top: 5px;">
    <div class="col-md-12">
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>Relatório Geral de Pré-PI</h5>
            </div>
            <div class="ibox-content">
                <div class="table-responsive" style="overflow-y: auto;">

                    <?=$tabela?>

                </div>
            </div>
        </div>
    </div>
</div>

