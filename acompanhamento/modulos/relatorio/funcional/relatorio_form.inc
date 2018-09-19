<input type="hidden" name="req" id="req" value=""/>
<div class="ibox-title">
    <h5>Relatório de Funcionais</h5><br>
</div>
<div class="ibox-content">
    <?php
    $stSql = "SELECT prsano AS codigo, prsano AS descricao FROM public.vw_subunidadeorcamentaria ORDER BY 1;";
    $preenchidos = null;
    if (!$_REQUEST['filtro']['prsano']) {
        $_REQUEST['filtro']['prsano'] = $preenchidos = $_SESSION['exercicio'];
    } else {
        $preenchidos = $_REQUEST['filtro']['prsano'];
    }
    echo $simec->select('filtro[pliano]', 'Exercício', $preenchidos, $stSql, null, ['input-size' => 9, 'label-size' => 2]);
    ?>
    <label class="col-sm-2 col-md-2 col-lg-2 control-label">Colunas</label>
    <?php
        unset($agrupador, $destino, $origem);
        // Início das colunas
        $agrupador = new AgrupadorBootstrap('filtro', '');
        // Dados padrão de destino (nulo)
        $destino = isset($agrupador2) ? $agrupador2 : array();
        // Dados padrão de origem
        $origem = array(
            array('codigo' => 'unosigla',           'descricao' => 'Unidade'),
            array('codigo' => 'suonome',            'descricao' => 'Subunidade'),
            array('codigo' => 'ptres',              'descricao' => 'PTRES'),
            array('codigo' => 'funcional',          'descricao' => 'Funcional'),
            array('codigo' => 'ptrdotacao_custeio', 'descricao' => 'Dotação Custeio'),
            array('codigo' => 'picvalorcusteio',    'descricao' => 'Planejado Custeio'),
            array('codigo' => 'ptrdotacao_capital', 'descricao' => 'Dotação Capital'),
            array('codigo' => 'picvalorcapital',    'descricao' => 'Planejado Capital'),
            array('codigo' => 'ptrdotacao_total',   'descricao' => 'Dotação Total'), // criar campo na consulta, nao existe total no sql
            array('codigo' => 'picvalorcapital',    'descricao' => 'Planejado Total'), // criar campo na consulta, nao existe total no sql 
            array('codigo' => 'provisionado',       'descricao' => 'Provisionado'),
            array('codigo' => 'empenhado',          'descricao' => 'Empenhado'),
            array('codigo' => 'liquidado',          'descricao' => 'Liquidado'),
            array('codigo' => 'pago',               'descricao' => 'Pago'),
        );
        // exibe agrupador
        $agrupador->setOrigem('naoColuna', null, $origem);
        $agrupador->setDestino('coluna', null, $destino);
        $agrupador->exibir();

        $stSql = "
            SELECT DISTINCT
                suo.unoid AS codigo,
                suo.unocod || ' - ' || unonome AS descricao
            FROM vw_subunidadeorcamentaria suo
            WHERE
		suo.suostatus = 'A'
		AND suo.prsano = '". $_REQUEST['filtro']['prsano']."'
            ORDER BY
                descricao
        ";
        echo $simec->select("filtro[unoid][]", 'Unidade', NULL, $stSql, null, ['input-size' => 9, 'label-size' => 2]);

        $stSql = "
            SELECT DISTINCT
                suo.suoid AS codigo,
                suo.suocod || ' - ' || suo.suonome AS descricao
            FROM public.vw_subunidadeorcamentaria suo
            WHERE
                suo.suostatus = 'A'
                AND suo.prsano= '". $_REQUEST['filtro']['prsano']."'
            ORDER BY
                descricao";
        echo $simec->select("filtro[suoid][]", 'Subunidade', NULL, $stSql, null, ['input-size' => 9, 'label-size' => 2]);

        $stSql = (new Monitora_Model_Acao())->recuperarSqlCombo(['prgcod', 'acacod', 'acaobjetivocod', 'loccod', 'acatitulo']);
        echo $simec->select("filtro[acaid][]", 'Ação', null, $stSql, null, ['input-size' => 9, 'label-size' => 2]);
        
        $stSql = "
            SELECT
                ptr.ptrid codigo,
                coalesce(ptr.prgcod, '') || ' - ' || coalesce(ptr.acacod, '') || ' - ' || coalesce(ptr.loccod, '') || ' - ' || coalesce(ptr.acaobjetivocod, '') || ' - ' || coalesce(ptr.plocod, '') || ' - ' || ptr.acatitulo || ' - ' || ptr.plodsc descricao
            FROM monitora.vw_ptres ptr
                JOIN monitora.acao ON ptr.acaid = acao.acaid
                JOIN public.unidadeorcamentaria ON(
                    acao.unicod = unidadeorcamentaria.unocod
                    AND acao.prgano = unidadeorcamentaria.prsano
                )
            WHERE
                ptr.ptrstatus = 'A'
                AND ptr.ptrano = '". $_REQUEST['filtro']['prsano']."'
            ORDER BY
                descricao
        ";
        echo $simec->select("filtro[ptrid][]", 'Funcional', null, $stSql, null, ['input-size' => 9, 'label-size' => 2]);

        $stSql = "
            SELECT DISTINCT
                irp.irpcod AS codigo,
                irp.irpcod || ' - ' || irp.irpdsc AS descricao
            FROM public.identresultprimario irp
            WHERE
                irp.irpstatus = 'A'
            ORDER BY
                2,1";
        echo $simec->select("filtro[irpcod][]", 'RP', null, $stSql, null, ['input-size' => 9, 'label-size' => 2]);
    ?>
    <div class="text-center">
        <a class="btn-success btn btn-md" href="javascript:exibeRelatorioGeral('exibir');"><i class="fa fa-eye"></i> Visualizar</a>
        <a class="btn btn-primary btn-buscar" href="javascript:exibeRelatorioGeral('xls');"><i class="fa fa-file-excel-o"></i> Exportar XLS</a>
    </div>
</div>