<?php

    # Incluindo arquivo do Módulo que possui a implementação da busca
    include_once APPRAIZ. "monitora/classes/Pi_PlanoInterno.class.inc";

    # Incluindo o arquivo da classe de relatorio
    include_once APPRAIZ. 'includes/relatorio-agrupador/tabela.php';

    # Busca SQL com filtros pra realizar a consulta
    $filtros = array(
        'suo.unofundo = FALSE',
        "suo.prsano = '". $_POST['filtro']['prsano']. "'"
    );
    # Filtros opcionais
    if($_POST['filtro']['unoid']){
        $filtros[] = "suo.unoid IN(". join($_POST['filtro']['unoid'], ','). ")";
    }
    if($_POST['filtro']['suoid']){
        $filtros[] = "suo.suoid IN(". join($_POST['filtro']['suoid'], ','). ")";
    }
    if($_POST['filtro']['acaid']){
        $filtros[] = "ptr.acaid IN(". join($_POST['filtro']['acaid'], ','). ")";
    }
    if($_POST['filtro']['ptrid']){
        $filtros[] = "ptr.ptrid IN(". join($_POST['filtro']['ptrid'], ','). ")";
    }
    if($_POST['filtro']['irpcod']){
        $filtros[] = "ptr.irpcod::INTEGER IN(". join($_POST['filtro']['irpcod'], ','). ")";
    }
    $modelPlanoInterno = new Pi_PlanoInterno();
//    ver($_POST, d);
    $sql = $modelPlanoInterno->montarSqlRelGerencialFuncionais($filtros, $_POST['filtro']['prsano']);

    # Busca lista pra montar a tabela do relatório
    $listaRelatorio = $db->carregar($sql);

    # Colunas do relatorio
    $listaTodasColunas = montarColunasRelatorioFuncional();
    $listaColunasSelecionadas = $_POST['coluna'];
    $listaColunaFormatoMoeda = [
        'custeio_dotacao',
        'custeio_planejado',
        'capital_dotacao',
        'capital_planejado',
        'total_dotacao',
        'total_planejado',
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

    include APPRAIZ. "includes/cabecalho_bootstrap_v3_relatorio.inc";
?>

<div class="row" style="margin-top: 5px;">
    <div class="col-md-12">
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>Relatório de Funcionais</h5>
            </div>
            <?php
                if(count($_POST['filtro']['suoid']) === 1):

            $sql = "SELECT DISTINCT
                        suo.suonome AS descricao
                    FROM public.vw_subunidadeorcamentaria suo
                    WHERE
                        suo.suostatus = 'A'
                        AND suo.prsano = '". $_REQUEST['filtro']['prsano']."'
                        AND suo.suoid = '".$_POST['filtro']['suoid'][0]."'
                    ORDER BY
                        descricao
            ";
            $suonome = $db->pegaUm($sql); ?>
                <h2 style="text-align: center"><?= $suonome ?> </h2>
            <?php endif; ?>
            <div class="ibox-content">
                <div class="table-responsive" style="overflow-y: auto;">

                    <?=$tabela?>

                </div>
                <div class="table-responsive" style="overflow-y: auto;">

                    <?php
                        $stSql = "
                            SELECT DISTINCT
                                suo.unoid AS codigo,
                                suo.unocod || ' - ' || unonome AS descricao
                            FROM vw_subunidadeorcamentaria suo
                            WHERE
                                suo.suostatus = 'A'
                            ORDER BY
                                descricao
                        ";
                       echo $simec->select("filtro[unoid][]", 'Unidade', NULL, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
                    ?>

                </div>
            </div>
        </div>
    </div>
</div>

