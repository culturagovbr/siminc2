<?php
    # Incluindo arquivo do Módulo que possui a implementação da busca
    include_once APPRAIZ . "emendas/classes/model/Emenda.inc";

    # Incluindo o arquivo da classe de relatorio
    include_once APPRAIZ . 'includes/relatorio-agrupador/tabela.php';

    # Busca SQL com filtros pra realizar a consulta
    $filtros = array(
        'eme.emeimpositiva = FALSE',
        "eme.prsano = '" . $_POST['filtro']['prsano'] . "'"
    );

    $dto = (object)$_POST['filtro'];
    $dto->suoid = $dto->suoid? $dto->suoid: $listaSubUnidades;
    $dto->exercicio = $_POST['filtro']['prsano'];
    $modelEmenda = new Emendas_Model_Emenda();
    $sql = $modelEmenda->montarSqlRelatorioGeral($dto);

    # Busca lista pra montar a tabela do relatório
    $listaRelatorio = $db->carregar($sql);

    # Colunas do relatorio
    $listaTodasColunas = Emendas_Model_Emenda::montarColunasRelatorioEmendas();
    $listaColunasSelecionadas = $_POST['coluna'];
    $listaColunaFormatoMoeda = Emendas_Model_Emenda::montarColunasFormatoMoedaRelatorioEmendas();

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

                    if( count($_REQUEST['filtro']['suoid_delegada']) > 0 ) {

                        $stSql = "SELECT DISTINCT
                                      suo.suoid AS codigo,
                                      suo.suocod || ' - ' || suo.suonome AS descricao
                                  FROM public.vw_subunidadeorcamentaria suo
                                  WHERE
                                      suo.prsano = '" . $_REQUEST['filtro']['prsano'] . "'
                                      AND suo.suostatus = 'A'
                                  ORDER BY
                                      descricao
                        ";
                        echo $simec->select("filtro[suoid_delegada][]", 'Subunidade Delegada', $_REQUEST['filtro']['suoid_delegada'], $stSql, null, ['input-size' => 9, 'label-size' => 2]);
                    }

                    if( count($_REQUEST['filtro']['sitid']) > 0 ) {

                        $stSql = "SELECT DISTINCT
                                      sit.sitid AS codigo,
                                      sit.sitdsc AS descricao
                                  FROM emendas.siconvsituacao sit
                                  WHERE
                                      sit.sitstatus = 'A'
                                  ORDER BY
                                      descricao
                        ";
                        echo $simec->select("filtro[sitid][]", 'Situação SICONV', $_REQUEST['filtro']['sitid'], $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
                    }

                    if( count($_REQUEST['filtro']['esdid']) > 0 ) {

                        $stSql = "SELECT DISTINCT
                                      esdid AS codigo,
                                      esddsc AS descricao
                                  FROM workflow.estadodocumento
                                  WHERE
                                      tpdid = '" . WF_TPDID_BENEFICIARIO . "'
                                      AND esdstatus = 'A'
                                  ORDER BY
                                      esddsc
                        ";
                        echo $simec->select("filtro[esdid][]", 'Situação', $_REQUEST['filtro']['esdid'], $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
                    }

                    if( count($_REQUEST['filtro']['autid']) > 0 ) {

                        $stSql = "SELECT
                                      autid AS codigo,
                                      a.autcod || ' - ' || a.autnome || ' - ' || aut.atpdsc AS descricao
                                  FROM emendas.autor a -- SELECT * FROM emendas.autor a
                                      JOIN emendas.autortipo aut ON a.atpid = aut.atpid -- SELECT * FROM emendas.autortipo
                                      JOIN emendas.partido p ON a.parid = p.parid -- SELECT * FROM emendas.partido
                                  WHERE
                                      autstatus = 'A'
                                  ORDER BY
                                      descricao
                        ";
                        echo $simec->select("filtro[autid][]", 'Autor', $_REQUEST['filtro']['autid'], $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
                    }

                    if( count($_REQUEST['filtro']['estuf']) > 0 ) {

                        $stSql = "SELECT DISTINCT
                                  estuf AS codigo,
                                  estdescricao || ' - ' || estuf AS descricao
                              FROM territorios.estado
                              ORDER BY
                                  descricao
                        ";
                        echo $simec->select("filtro[estuf][]", 'UF Parlamentar', $_REQUEST['filtro']['estuf'], $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
                    }

                    if( count($_REQUEST['filtro']['parid']) > 0 ) {

                        $stSql = "SELECT DISTINCT
                                      parid AS codigo,
                                      parnome || ' - ' || parsigla AS descricao
                                  FROM emenda.partido
                                  WHERE
                                      parstatus = 'A'
                                  ORDER BY
                                      descricao
                        ";
                        echo $simec->select("filtro[parid][]", 'Partido', $_REQUEST['filtro']['parid'], $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
                    }

                    if( count($_REQUEST['filtro']['capid']) > 0 ) {

                        $stSql = "SELECT DISTINCT
                                      capid AS codigo,
                                      capdsc AS descricao
                                  FROM monitora.pi_categoriaapropriacao
                                  WHERE
                                      capano = '" . $_REQUEST['filtro']['prsano'] . "'
                                      AND capstatus = 'A'
                                  ORDER BY
                                      descricao
                        ";
                        echo $simec->select("filtro[capid][]", 'Modalidade de Pactuação', $_REQUEST['filtro']['capid'], $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
                    }

                    if( $_REQUEST['filtro']['bented'] != '' ) {

                        echo $simec->radio("filtro[bented]", 'TED', $_REQUEST['filtro']['bented'], ['t' => 'Sim', 'f' => 'Não', '' => 'Todos'], NULL, ['input-size' => 9, 'label-size' => 2]);
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
                        echo $simec->select("filtro[mdeid][]", 'Área Cultural', $_REQUEST['filtro']['mdeid'], $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
                    }

                    if( count($_REQUEST['filtro']['aloid']) > 0 ) {

                        $stSql = "SELECT DISTINCT
                                      aloid AS codigo,
                                      alodsc AS descricao
                                  FROM emendas.alteracaoorcamentaria
                                  WHERE
                                      alostatus = 'A'
                                  ORDER BY
                                      descricao
                        ";
                        echo $simec->select("filtro[aloid][]", 'Alterações', $_REQUEST['filtro']['aloid'], $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
                    }

                    if( $_REQUEST['filtro']['benparecertecnico'] != '' ) {

                        echo $simec->radio("filtro[benparecertecnico]", 'Parecer Técnico', $_REQUEST['filtro']['benparecertecnico'], ['t' => 'Sim', 'f' => 'Não', '' => 'Todos'], NULL, ['input-size' => 9, 'label-size' => 2]);
                    }

                    if( $_REQUEST['filtro']['benparecerjuridico'] != '' ) {
                        echo $simec->radio("filtro[benparecerjuridico]", 'Parecer Jurídico', $_REQUEST['filtro']['benparecerjuridico'], ['t' => 'Sim', 'f' => 'Não', '' => 'Todos'], NULL, ['input-size' => 9, 'label-size' => 2]);
                    }

                    if( count($_REQUEST['filtro']['impid']) > 0 ) {

                        $stSql = "SELECT DISTINCT
                                      impid AS codigo,
                                      impdsc AS descricao
                                  FROM emendas.impedimento
                                  WHERE
                                      impstatus = 'A'
                                  ORDER BY
                                      descricao
                        ";
                        echo $simec->select("filtro[impid][]", 'Impedimento', $_REQUEST['filtro']['impid'], $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
                    }
                    ?>
                </div>
            </div>
        </div>
    </div>
</div>