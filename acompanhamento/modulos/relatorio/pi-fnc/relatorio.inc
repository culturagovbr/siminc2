<?php

    # Incluindo arquivo do Módulo que possui a implementação da busca
    include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";
    $modelPlanoInterno = new Pi_PlanoInterno();

    # Incluindo o arquivo da classe de relatorio
    include_once APPRAIZ . 'includes/relatorio-agrupador/tabela.php';

    # Busca SQL com filtros pra realizar a consulta
    $dto = (object)$_POST['filtro'];
    $dto->fundo = TRUE;
    $dto->exercicio = $_POST['filtro']['prsano'];
    $listaRelatorio = $modelPlanoInterno->listarRelatorioGeralPi($dto);

    # Colunas do relatorio
    $listaTodasColunas = montarColunasRelatorioPI();
    $listaColunasSelecionadas = $_POST['coluna'];
    $listaColunaFormatoMoeda = montarColunasFormatoMoeda();

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
        header('Content-Disposition: attachment; filename="relatorio_gerencial_pi_fnc.xls"');
        die();
    }

    include APPRAIZ. "includes/cabecalho_bootstrap_v3_relatorio.inc";
?>

<div class="row" style="margin-top: 5px;">
    <div class="col-md-12">
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>Relatório de PI-FNC</h5>
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

                    <?= $tabela ?>

                </div>
                <div class="table-responsive" style="overflow-y: auto;">
                    <h5>Filtros do Relatório</h5>
                    <div class="ibox-title"></div>

                    <?php
                    # Configura o formulário pra ter formatação de somente leitura
                    $simec->setPodeEditar(FALSE);

                    if( count($_REQUEST['filtro']['suoid']) > 0 ){

                        $stSql = "SELECT DISTINCT
                                  suo.suoid AS codigo,
                                  suo.suocod || ' - ' || suo.suonome AS descricao,
                                  suo.prsano
                              FROM public.vw_subunidadeorcamentaria suo
                              WHERE
                                  suo.prsano = '".$_REQUEST['filtro']['prsano']."'
                                  AND suo.suostatus = 'A'
                                  AND suo.unofundo IS TRUE
                              ORDER BY
                                  3,2
                    ";
                        echo $simec->select("filtro[suoid][]", 'Subunidade', $_REQUEST['filtro']['suoid'], $stSql, null, ['input-size' => 3, 'label-size' => 1]);
                    }

                    if( count($_REQUEST['filtro']['esdid']) > 0 ){

                        $stSql = "SELECT DISTINCT
                                      esdid AS codigo,
                                      esddsc AS descricao
                                  FROM workflow.estadodocumento
                                  WHERE
                                      tpdid = '".WF_TPDID_PLANEJAMENTO_PI_FNC."'
                                      AND esdstatus = 'A'
                                  ORDER BY
                                      1
                        ";
                        echo $simec->select("filtro[esdid][]", 'Situação', $_REQUEST['filtro']['esdid'], $stSql, null, ['input-size' => 3, 'label-size' => 1]);
                    }

                    if( count($_REQUEST['filtro']['eqdid']) > 0 ){

                        $stSql = "SELECT DISTINCT
                                      eqd.eqdid AS codigo,
                                      eqd.eqddsc AS descricao
                                  FROM monitora.pi_enquadramentodespesa eqd
                                  WHERE eqd.eqdano = '".$_REQUEST['filtro']['prsano']."'
                                      AND eqdstatus = 'A'
                                  ORDER BY
                                      2,1
                        ";
                        echo $simec->select("filtro[eqdid][]", 'Enquadramento', $_REQUEST['filtro']['eqdid'], $stSql, null, ['input-size' => 3, 'label-size' => 1]);
                    }

                    if( $_REQUEST['filtro']['picted'] != '' ) {
                        echo $simec->radio("filtro[picted]", 'TED', $_REQUEST['filtro']['picted'], ['t' => 'Sim', 'f' => 'Não', '' => 'Todos'], null, ['input-size' => 3, 'label-size' => 1]);
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
                        echo $simec->select("filtro[irpcod][]", 'RP', $_REQUEST['filtro']['irpcod'], $stSql, null, ['input-size' => 3, 'label-size' => 1]);
                    }

                    if( count($_REQUEST['filtro']['acaid']) > 0 ) {

                        $stSql = "SELECT DISTINCT
                                  acao.acaid AS codigo,
                                  prgcod || ' - ' ||
                                  acacod || ' - ' ||
                                  COALESCE(acaobjetivocod, '') || ' - ' || 
                                  loccod || ' - ' || 
                                  acatitulo AS descricao
                              FROM monitora.acao acao
                              JOIN public.vw_subunidadeorcamentaria suo ON(
                                  suostatus = 'A'
                                  AND acao.unicod = suo.unocod
                                  AND suo.prsano = prgano
                              )
                              WHERE
                                  acastatus = 'A'
                                  AND prgano = '" . $_REQUEST['filtro']['prsano'] . "'
                              ORDER BY
                                  descricao
                        ";
                        echo $simec->select("filtro[acaid][]", 'Ação', $_REQUEST['filtro']['acaid'], $stSql, null, ['input-size' => 3, 'label-size' => 1]);
                    }

                    if( count($_REQUEST['filtro']['oppnome']) > 0 ) {

                        $stSql = "SELECT DISTINCT
                                  oppid as codigo,
                                  oppcod || ' - ' || oppnome as descricao,
                                  prsano as ano
                              FROM public.objetivoppa
                              WHERE
                                  prsano = '" . $_REQUEST['filtro']['prsano'] . "'
                                  AND oppstatus = 'A'
                              ORDER BY
                                  3
                        ";
                        echo $simec->select("filtro[oppnome][]", 'Objetivo', $_REQUEST['filtro']['oppnome'], $stSql, null, ['input-size' => 3, 'label-size' => 1]);
                    }

                    if( count($_REQUEST['filtro']['mdeid']) > 0 ) {

                        $stSql = "SELECT
                                  mdeid AS codigo,
                                  mdedsc AS descricao
                              FROM monitora.pi_modalidadeensino
                              WHERE
                                  mdestatus = 'A'
                                  AND mdeano = '" . $_REQUEST['filtro']['prsano'] . "'
                              ORDER BY
                                  descricao ASC
                        ";
                        echo $simec->select("filtro[mdeid][]", 'Área Cultural', $_REQUEST['filtro']['mdeid'], $stSql, null, ['input-size' => 3, 'label-size' => 1]);
                    }

                    if( count($_REQUEST['filtro']['capid']) > 0 ) {

                        $stSql = "SELECT DISTINCT
                                  capid AS codigo,
                                  capano as ano,
                                  capdsc AS descricao
                              FROM monitora.pi_categoriaapropriacao
                              WHERE
                                  capano = '" . $_REQUEST['filtro']['prsano'] . "'
                                  AND capstatus = 'A'
                              ORDER BY
                                  3,2
                        ";
                        echo $simec->select("filtro[capid][]", 'Modalidade de Pactuação', $_REQUEST['filtro']['capid'], $stSql, null, ['input-size' => 3, 'label-size' => 1]);
                    }

                    if( count($_REQUEST['filtro']['mescod']) > 0 ) {

                        $stSql = "SELECT
                                  mescod AS codigo,
                                  mesdsc AS descricao
                              FROM public.meses
                              ORDER BY
                                  codigo
                        ";
                        echo $simec->select("filtro[mescod][]", 'Edital', $_REQUEST['filtro']['mescod'], $stSql, null, ['input-size' => 3, 'label-size' => 1]);
                    }

                    if( count($_REQUEST['filtro']['claid']) > 0 ) {

                        $stSql = "SELECT
                                 claid AS codigo,
                                 cladsc AS descricao
                              FROM acompanhamento.classificacao
                              WHERE
                                  tipid = '" . TPDID_PLANO_INTERNO . "'
                                  AND clastatus = 'A'
                              ORDER BY
                                  2,1
                        ";
                        echo $simec->select("filtro[claid][]", 'Classificação', $_REQUEST['filtro']['claid'], $stSql, null, ['input-size' => 3, 'label-size' => 1]);
                    }
                    ?>
                </div>
            </div>
        </div>
    </div>
</div>