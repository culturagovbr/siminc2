<?php

    # Incluindo arquivo do Módulo que possui a implementação da busca
    include_once APPRAIZ . "www/proposta/_funcoes.php";

    # Incluindo o arquivo da classe de relatorio
    include_once APPRAIZ . 'includes/relatorio-agrupador/tabela.php';

    # Busca SQL com filtros pra realizar a consulta
    $filtro = array(
        'prsano'            => $_REQUEST['filtro']['prsano'],
        'suoid'             => $_REQUEST['filtro']['suoid']? $_REQUEST['filtro']['suoid']: $listaSubUnidades,
        'eqdid'             => $_REQUEST['filtro']['eqdid'],
        'irpcod'            => $_REQUEST['filtro']['irpcod'],
        'tpdid'             => $_REQUEST['filtro']['tpdid']
    );
    
    $sql = montarSqlRelatorioGeralProposta((object) $filtro);

    # Busca lista pra montar a tabela do relatório
    $listaRelatorio = $db->carregar($sql);

    # Colunas do relatorio
    $listaTodasColunas = montarColunasRelatorioProposta();
    $listaColunasSelecionadas = $_REQUEST['coluna'];
    $listaColunaFormatoMoeda = montarColunasFormatoMoedaRelatorioProposta();

    $relatorio = new relatorio_agrupador_tabela();
    $tabela = $relatorio->setListaTodasColunas($listaTodasColunas)
            ->setListaColunasSelecionadas($listaColunasSelecionadas)
            ->setListaColunaFormatoMoeda($listaColunaFormatoMoeda)
            ->setListaRelatorio($listaRelatorio)
            ->montarTabela()
            ->getTabela();

    # Gera o XLS do relatório
    if ($_REQUEST['req'] == 'xls') {
        echo $tabela;

        header('Content-Type: application/vnd.ms-excel');
        header('Expires: 0');
        header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
        header('Content-Disposition: attachment; filename="relatorio_gerencial_pre_pi.xls"');
        die();
    }

    include APPRAIZ. "includes/cabecalho_bootstrap_v3_relatorio.inc";
?>

<div class="row" style="margin-top: 0px;">
    <div class="col-md-12">
        <div class="ibox float-e-margins">
            <?php
            if(count($_REQUEST['filtro']['suoid']) === 1):

                $sql = "SELECT DISTINCT
                            suo.suonome AS descricao
                        FROM public.vw_subunidadeorcamentaria suo
                        WHERE
                            suo.suostatus = 'A'
                            AND suo.prsano = '". $_REQUEST['filtro']['prsano']."'
                            AND suo.suoid = '".$_REQUEST['filtro']['suoid'][0]."'
                        ORDER BY
                            descricao
                ";

                $suonome = $db->pegaUm($sql); ?>

                <h2 style="text-align: center; margin-top: 10px !important;"><?= $suonome ?></h2>
            <?php endif; ?>
            <div class="ibox-content">
                <div class="table-responsive" style="overflow-y: auto;">

                    <?= $tabela ?>

                </div>
                <div class="table-responsive" style="overflow-y: auto;">
                    <h5>Filtros do Relatório</h5>
                    <div class="ibox-title"></div>

                    <?php
                    # Configura o formulário pra ter formatação de somente leitura
                    $simec->setPodeEditar(FALSE);

                    if( count($_REQUEST['filtro']['suoid']) > 0 ) {

                        $stSql = "SELECT DISTINCT
                                      suo.suoid AS codigo,
                                      suo.suocod || ' - ' || suo.suonome AS descricao,
                                      suo.prsano
                                  FROM public.vw_subunidadeorcamentaria suo
                                  WHERE
                                      suo.prsano = '" . $_REQUEST['filtro']['prsano'] . "'
                                      AND suo.suostatus = 'A'
                                  ORDER BY
                                      3,2
                        ";
                        echo $simec->select("filtro[suoid][]", 'Subunidade', $_REQUEST['filtro']['suoid'], $stSql, null, ['input-size' => 9, 'label-size' => 2]);
                    }

                    if( count($_REQUEST['filtro']['eqdid']) > 0 ) {

                        $stSql = "SELECT DISTINCT 
                                  eqd.eqdid AS codigo,
                                  eqd.eqddsc AS descricao
                              FROM planejamento.enquadramento_despesa eqd
                              WHERE 
                                  eqd.eqdano = '" . $_REQUEST['filtro']['prsano'] . "'
                                  AND eqdstatus = 'A'
                              ORDER BY
                                  2,1
                        ";
                        echo $simec->select("filtro[eqdid][]", 'Enquadramento', $_REQUEST['filtro']['eqdid'], $stSql, null, ['input-size' => 9, 'label-size' => 2]);
                    }

                    if( count($_REQUEST['filtro']['irpcod']) > 0 ) {

                        $stSql = "SELECT DISTINCT
                                  irp.irpcod AS codigo,
                                  irp.irpcod || ' - ' || irp.irpdsc AS descricao
                              FROM public.identresultprimario irp
                              WHERE
                                  irp.irpstatus = 'A'
                              ORDER BY
                                  2,1
                        ";
                        echo $simec->select("filtro[irpcod][]", 'RP', $_REQUEST['filtro']['irpcod'], $stSql, null, ['input-size' => 9, 'label-size' => 2]);
                    }

                    if( count($_REQUEST['filtro']['tpdid']) > 0 ) {

                        $stSql = "SELECT DISTINCT
                                  tpd.tpdid AS codigo,
                                  tpd.tpdcod || ' - ' || tpd.tpdnome AS descricao
                              FROM proposta.tipodetalhamento tpd
                              WHERE
                                  tpd.tpdstatus = 'A'
                              ORDER BY
                                  2,1
                        ";
                        echo $simec->select("filtro[tpdid][]", 'Tipo de Detalhamento', $_REQUEST['filtro']['tpdid'], $stSql, null, ['input-size' => 9, 'label-size' => 2]);
                    }
                    ?>
                </div>
            </div>
        </div>
    </div>
</div>
