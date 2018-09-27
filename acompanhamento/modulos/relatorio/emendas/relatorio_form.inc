<input type="hidden" name="req" id="req" value=""/>

<div class="ibox-title">
    <h5>Relatório Geral de Emendas</h5><br>
</div>
<div class="ibox-content">
    <?php
        $stSql = "SELECT DISTINCT prsano AS codigo, prsano AS descricao FROM emendas.emenda WHERE emestatus = 'A' ORDER BY 1";

        $preenchidos = null;
        if(!$_REQUEST['filtro']['prsano']) {
            $_REQUEST['filtro']['prsano'] = $preenchidos = $_SESSION['exercicio'];
        } else {
            $preenchidos = $_REQUEST['filtro']['prsano'];
        }
        echo $simec->select('filtro[prsano]', 'Exercício', $preenchidos, $stSql, null, ['input-size' => 9, 'label-size' => 2]);
    ?>
    <label class="col-sm-2 col-md-2 col-lg-2 control-label">Colunas</label>
    <?php
        unset($agrupador, $destino, $origem);
        // Início das colunas
        $agrupador = new AgrupadorBootstrap('filtro', '');
        // Dados padrão de destino (nulo)
        $destino = isset($agrupador2) ? $agrupador2 : array();

        # Dados das colunas do Relatório
        $origem = Emendas_Model_Emenda::montarColunasRelatorioEmendas();

        // exibe agrupador
        $agrupador->setOrigem('naoColuna', null, $origem);
        $agrupador->setDestino('coluna', null, $destino);
        $agrupador->exibir();

        $stSql = "
            SELECT DISTINCT
                suo.suoid AS codigo,
                suo.suocod || ' - ' || suo.suonome AS descricao
            FROM public.vw_subunidadeorcamentaria suo
            WHERE
                suo.prsano = '".$_REQUEST['filtro']['prsano']."'
                AND suo.suostatus = 'A'
            ORDER BY
                descricao
        ";
        echo $simec->select("filtro[suoid][]", 'Subunidade', $suoid, $stSql, null, ['input-size' => 9, 'label-size' => 2]);
        
        $stSql = "
            SELECT DISTINCT
                suo.suoid AS codigo,
                suo.suocod || ' - ' || suo.suonome AS descricao
            FROM public.vw_subunidadeorcamentaria suo
            WHERE
                suo.prsano = '".$_REQUEST['filtro']['prsano']."'
                AND suo.suostatus = 'A'
            ORDER BY
                descricao
        ";
        echo $simec->select("filtro[suoid_delegada][]", 'Subunidade Delegada', $suoid, $stSql, null, ['input-size' => 9, 'label-size' => 2]);

        $stSql = "
            SELECT DISTINCT
                sit.sitid AS codigo,
                sit.sitdsc AS descricao
            FROM emendas.siconvsituacao sit
            WHERE
                sit.sitstatus = 'A'
            ORDER BY
                descricao
        ";
        echo $simec->select("filtro[sitid][]", 'Situação SICONV', NULL, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);

        $stSql = "
            SELECT DISTINCT
                esdid AS codigo,
                esddsc AS descricao
            FROM workflow.estadodocumento
            WHERE
                tpdid = '".WF_TPDID_BENEFICIARIO."'
                AND esdstatus = 'A'
            ORDER BY
                esddsc
        ";
        echo $simec->select("filtro[esdid][]", 'Situação', NULL, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);

        $stSql = "
            SELECT
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
        echo $simec->select("filtro[autid][]", 'Autor', NULL, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);

        $stSql = "
            SELECT DISTINCT
                estuf AS codigo,
                estdescricao || ' - ' || estuf AS descricao
            FROM territorios.estado
            ORDER BY
                descricao
        ";
        echo $simec->select("filtro[estuf][]", 'UF Parlamentar', NULL, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);

        $stSql = "
            SELECT DISTINCT
                parid AS codigo,
                parnome || ' - ' || parsigla AS descricao
            FROM emenda.partido
            WHERE
                parstatus = 'A'
            ORDER BY
                descricao
        ";
        echo $simec->select("filtro[parid][]", 'Partido', NULL, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);

        $stSql = "
            SELECT DISTINCT
                capid AS codigo,
                capdsc AS descricao
            FROM monitora.pi_categoriaapropriacao
            WHERE
                capano = '".$_REQUEST['filtro']['prsano']."'
                AND capstatus = 'A'
            ORDER BY
                descricao
        ";
        echo $simec->select("filtro[capid][]", 'Modalidade de Pactuação', NULL, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);

        # Traz o Todos por default
        $bented = '';
        echo $simec->radio("filtro[bented]", 'TED', $bented, ['t' => 'Sim', 'f' => 'Não', '' => 'Todos'], NULL, ['input-size' => 9, 'label-size' => 2]);

        $stSql = "
            SELECT
                mdeid AS codigo,
                mdedsc AS descricao
            FROM monitora.pi_modalidadeensino
            WHERE
                mdestatus = 'A'
                AND mdeano = '".$_REQUEST['filtro']['prsano']."'
            ORDER BY
                descricao ASC
        ";
        echo $simec->select("filtro[mdeid][]", 'Área Cultural', NULL, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);

        $stSql = "
            SELECT DISTINCT
                aloid AS codigo,
                alodsc AS descricao
            FROM emendas.alteracaoorcamentaria
            WHERE
                alostatus = 'A'
            ORDER BY
                descricao
        ";
        echo $simec->select("filtro[aloid][]", 'Alterações', NULL, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);

        # Traz o Todos por default
        $parecerTecn = '';
        echo $simec->radio("filtro[benparecertecnico]", 'Parecer Técnico', $parecerTecn, ['t' => 'Sim', 'f' => 'Não', '' => 'Todos'], NULL, ['input-size' => 9, 'label-size' => 2]);

        # Traz o Todos por default
        $parecerJur = '';
        echo $simec->radio("filtro[benparecerjuridico]", 'Parecer Jurídico', $parecerJur, ['t' => 'Sim', 'f' => 'Não', '' => 'Todos'], NULL, ['input-size' => 9, 'label-size' => 2]);

        $stSql = "
            SELECT DISTINCT
                impid AS codigo,
                impdsc AS descricao
            FROM emendas.impedimento
            WHERE
                impstatus = 'A'
            ORDER BY
                descricao
            ";
        echo $simec->select("filtro[impid][]", 'Impedimento', NULL, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
    ?>
    <div class="text-center">
        <a class="btn-success btn btn-md" href="javascript:exibeRelatorioGeral('exibir');"><i class="fa fa-eye"></i> Visualizar</a>
        <a class="btn btn-primary btn-buscar" href="javascript:exibeRelatorioGeral('xls');"><i class="fa fa-file-excel-o"></i> Exportar XLS</a>
    </div>
</div>
