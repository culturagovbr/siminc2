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
        $arrColunas = montarColunasRelatorioFuncional();
        if ($retConsultaRelatorio['colunas']){
            foreach($arrColunas as $arrColuna){
                foreach($retConsultaRelatorio['colunas'] as $coluna){
                    if ($coluna['dscampo']==$arrColuna['codigo']){
                        $destino[] = $arrColuna;
                    }
                }
            }
            foreach($arrColunas as $arrColuna){
                $achou=false;
                foreach($retConsultaRelatorio['colunas'] as $coluna){
                    if ($coluna['dscampo']==$arrColuna['codigo']){
                        $achou=true;
                    }
                }
                if (!$achou){
                    $origem[] = $arrColuna;
                }
            }        
        }else{
            $destino = isset($agrupador2) ? $agrupador2 : array();
            $origem = $arrColunas;
        }
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
        echo $simec->select("filtro[unoid][]", 'Unidade', $unoid, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
        
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
        echo $simec->select("filtro[suoid][]", 'Subunidade', $suoid, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
        echo "</div>";
        
        echo "<div class='div_acaid'>";
        $stSql = "
            SELECT DISTINCT
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
        echo $simec->select("filtro[acaid][]", 'Ação', $acaid, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
        echo "</div>";
        
        echo "<div class='div_ptrid'>";
        $stSql = "
            SELECT DISTINCT
                ptr.ptrid codigo,
                coalesce(ptr.prgcod, '') || ' - ' || coalesce(ptr.acacod, '') || ' - ' || coalesce(ptr.loccod, '') || ' - ' || coalesce(ptr.acaobjetivocod, '') || ' - ' || coalesce(ptr.plocod, '') || ' - ' || ptr.acatitulo || ' - ' || ptr.plodsc descricao
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
        echo $simec->select("filtro[ptrid][]", 'Funcional', $ptrid, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
        echo "</div>";

        $stSql = "
            SELECT DISTINCT
                irp.irpcod AS codigo,
                irp.irpcod || ' - ' || irp.irpdsc AS descricao
            FROM public.identresultprimario irp
            WHERE
                irp.irpstatus = 'A'
            ORDER BY
                descricao";
        echo $simec->select("filtro[irpcod][]", 'RP', $irpcod, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
    ?>
</div>