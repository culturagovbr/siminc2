<input type="hidden" name="req" id="req" value=""/>
<div class="ibox-title">
    <h5>Relatório de Funcionais</h5><br>
</div>
<div class="ibox-content">
    <?php
        $stSql = "SELECT prsano AS codigo, prsano AS descricao FROM public.vw_subunidadeorcamentaria ORDER BY 1;";
        $preenchidos = NULL;
        if (!$_REQUEST['filtro']['prsano']) {
            $_REQUEST['filtro']['prsano'] = $preenchidos = $_SESSION['exercicio'];
        } else {
            $preenchidos = $_REQUEST['filtro']['prsano'];
        }
        echo $simec->select('filtro[prsano]', 'Exercício', $preenchidos, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
    ?>
    <label class="col-sm-2 col-md-2 col-lg-2 control-label">Colunas</label>
    <?php
        unset($agrupador, $destino, $origem);
        // Início das colunas
        $agrupador = new AgrupadorBootstrap('filtro', '');
        // Dados padrão de destino (nulo)
        $destino = isset($agrupador2) ? $agrupador2 : array();
        // Dados padrão de origem
        $origem = montarColunasRelatorioFuncional();
        
        // exibe agrupador
        $agrupador->setOrigem('naoColuna', NULL, $origem);
        $agrupador->setDestino('coluna', NULL, $destino);
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
        echo $simec->select("filtro[unoid][]", 'Unidade', NULL, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
        
        echo "<div class='div_suoid'>";
        $stSql = "
            SELECT DISTINCT
                suo.suoid AS codigo,
                suo.suocod || ' - ' || suo.suonome AS descricao
            FROM public.vw_subunidadeorcamentaria suo
            WHERE
                suo.suostatus = 'A'
                AND suo.prsano = '". $_REQUEST['filtro']['prsano']."'
            ORDER BY
                descricao";
        echo $simec->select("filtro[suoid][]", 'Subunidade', NULL, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
        echo "</div>";
        
        $stSql = "
            SELECT DISTINCT
                acao.acaid AS codigo,
                prgcod || ' - ' || acacod || ' - ' || acaobjetivocod || ' - ' || loccod || ' - ' || acatitulo AS descricao
            FROM monitora.acao acao
                JOIN public.unidade uni ON uni.unicod = acao.unicod
            WHERE
                acastatus = 'A'
                AND prgano = '". $_REQUEST['filtro']['prsano']."'
        ";
        echo $simec->select("filtro[acaid][]", 'Ação', NULL, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
        
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
        echo $simec->select("filtro[ptrid][]", 'Funcional', NULL, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);

        $stSql = "
            SELECT DISTINCT
                irp.irpcod AS codigo,
                irp.irpcod || ' - ' || irp.irpdsc AS descricao
            FROM public.identresultprimario irp
            WHERE
                irp.irpstatus = 'A'
            ORDER BY
                2,1";
        echo $simec->select("filtro[irpcod][]", 'RP', NULL, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
    ?>
    <div class="text-center">
        <a class="btn-success btn btn-md" href="javascript:exibeRelatorioGeral('exibir');"><i class="fa fa-eye"></i> Visualizar</a>
        <a class="btn btn-primary btn-buscar" href="javascript:exibeRelatorioGeral('xls');"><i class="fa fa-file-excel-o"></i> Exportar XLS</a>
    </div>
</div>