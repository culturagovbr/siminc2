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
    } else {
        if($listaSubUnidades){
            $filtros[] = "suo.suoid IN(". join($listaSubUnidades, ','). ")";
        }
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
                    <h2 style="text-align: center; margin-top: 10px !important;"><?= $suonome ?> </h2>
            <?php endif; ?>
            <div class="ibox-content">
                <div class="table-responsive" style="overflow-y: auto;">

                    <?=$tabela?>

                </div>
                <div class="table-responsive" style="overflow-y: auto;">
                    <h5>Filtros do Relatório</h5>
                    <div class="ibox-title"></div>

                    <?php
                    # Configura o formulário pra ter formatação de somente leitura
                    $simec->setPodeEditar(FALSE);

                    if( count($_REQUEST['filtro']['unoid']) > 0 ){

                        $stSql = "SELECT DISTINCT
                                      suo.unoid AS codigo,
                                      suo.unocod || ' - ' || unonome AS descricao
                                  FROM vw_subunidadeorcamentaria suo
                                  WHERE
                                      suo.suostatus = 'A'
                                  ORDER BY
                                      descricao
                        ";
                       echo $simec->select("filtro[unoid][]", 'Unidade', $_REQUEST['filtro']['unoid'], $stSql, NULL, ['input-size' => 3, 'label-size' => 1]);
                    }

                    if( count($_REQUEST['filtro']['suoid']) > 0 ){

                        $stSql = "SELECT DISTINCT
                                      suo.suoid AS codigo,
                                      suo.suocod || ' - ' || suo.suonome AS descricao
                                  FROM public.vw_subunidadeorcamentaria suo
                                  WHERE
                                      suo.suostatus = 'A'
                                      AND suo.prsano = '". $_REQUEST['filtro']['prsano']."'
                                  ORDER BY
                                      descricao
                        ";
                        echo $simec->select("filtro[suoid][]", 'Subunidade', $_REQUEST['filtro']['suoid'], $stSql, NULL, ['input-size' => 3, 'label-size' => 1]);
                    }

                    if( count($_REQUEST['filtro']['acaid']) > 0 ){

                        $stSql = "SELECT DISTINCT
                                      acao.acaid AS codigo,
                                      prgcod || ' - ' || acacod || ' - ' || acaobjetivocod || ' - ' || loccod || ' - ' || acatitulo AS descricao
                                  FROM monitora.acao acao
                                  JOIN public.vw_subunidadeorcamentaria suo ON(
                                        suostatus = 'A'
                                        AND acao.unicod = suo.unocod
                                        AND suo.prsano = prgano
                                  )
                                  WHERE
                                      acastatus = 'A'
                                      AND prgano = '". $_REQUEST['filtro']['prsano']."'
                                  ORDER BY
                                      descricao
                        ";
                        echo $simec->select("filtro[acaid][]", 'Ação', $_REQUEST['filtro']['acaid'], $stSql, NULL, ['input-size' => 3, 'label-size' => 1]);

                    }

                    if( count($_REQUEST['filtro']['ptrid']) > 0 ){

                        $stSql = "SELECT DISTINCT
                                      ptr.ptrid codigo,
                                      COALESCE (ptr.prgcod, '') || ' - ' ||
                                      COALESCE (ptr.acacod, '') || ' - ' ||
                                      COALESCE (ptr.loccod, '') || ' - ' ||
                                      COALESCE (ptr.acaobjetivocod, '') || ' - ' ||
                                      COALESCE (ptr.plocod, '') || ' - ' ||
                                      ptr.acatitulo || ' - ' ||
                                      ptr.plodsc descricao
                                  FROM monitora.vw_ptres ptr
		                          JOIN public.vw_subunidadeorcamentaria suo ON(
                                      suostatus = 'A'
                                      AND ptr.unicod = suo.unocod
                                      AND suo.prsano = ptr.ptrano
		                          )
                                  WHERE
                                      ptr.ptrstatus = 'A'
                                      AND ptr.ptrano = '". $_REQUEST['filtro']['prsano']."'
                                  ORDER BY
                                      descricao
                        ";
                        echo $simec->select("filtro[ptrid][]", 'Funcional', $_REQUEST['filtro']['ptrid'], $stSql, NULL, ['input-size' => 3, 'label-size' => 1]);

                    }

                    if( count($_REQUEST['filtro']['irpcod']) > 0 ){

                        $stSql = "SELECT DISTINCT
                                      irp.irpcod AS codigo,
                                      irp.irpcod || ' - ' || irp.irpdsc AS descricao
                                  FROM public.identresultprimario irp
                                  WHERE
                                      irp.irpstatus = 'A'
                                  ORDER BY
                                      descricao
                        ";
                        echo $simec->select("filtro[irpcod][]", 'RP', $_REQUEST['filtro']['irpcod'], $stSql, NULL, ['input-size' => 3, 'label-size' => 1]);

                    }
                    ?>
                </div>
            </div>
        </div>
    </div>
</div>

