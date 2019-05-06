<?php
$sbaid = $_GET['sbaid'];
/*
 * Modifica a tabela para cada tipo de chamada
 * (Remanejamento de Subação, Remanejamento de PI ou padrão (Apenas para leitura))
 */
switch ($_REQUEST['tp']) {
    case 'r':
        $cabecalho = array(
            "PTRES",
            "Ação",
            "Unidade",
            "Dotação Atualizada (R$)",
            "Não detalhado em Subação (R$)",
            "Detalhado do PTRES nesta Subação (R$)",
            "Empenhado do PTRES nesta Subação (R$)"
        );
        $compSql = "dtl.ptrid AS acao,";
        break;
    default:
        $cabecalho = array(
            "PTRES",
            "Ação",
            "Unidade",
            "Dotação Atualizada (R$)",
            "Não detalhado em Subação (R$)",
            "Detalhado do PTRES nesta Subação (R$)",
            "Empenhado do PTRES nesta Subação (R$)"
        );

        break;
}
if ($sbaid) {
    $subacao = $db->pegaLinha("SELECT * FROM monitora.pi_subacao WHERE sbaid = {$sbaid}");
    if (!$subacao) {
        echo "<script>
				alert('A Subação informada não existe!');
				history.back(-1);
			  </script>";
        die;
    }
}
?>

<div style="width: 50%; float: left;">
    <table class="table table-hover table-bordered" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <th class="active text-left" >Título:</th>
            <td><?php echo $subacao['sbatitulo']; ?></td>
        </tr>
        <tr>
            <th class="active text-left">Título Abreviado / Sigla:</th>
            <td><?php echo $subacao['sbasigla']; ?></td>
        </tr>
        <tr>
            <th class="active text-left">Enquadramento da Despesa:</th>
            <td>
                <?php
                if ($sbaid) {
                    $sql = <<<DML
						SELECT
							ed.eqdid AS codigo,
       						ed.eqdcod ||' - '|| ed.eqddsc AS descricao
  						FROM planejamento.enquadramento_despesa ed
    					INNER JOIN monitora.pi_subacaoenquadramento sae ON ed.eqdid = sae.eqdid
  						WHERE sae.sbaid = {$sbaid}
    						AND ed.eqdano = '{$_SESSION['exercicio']}'
DML;

                    $eqdid = $db->carregar($sql);
                    if ($eqdid) {
                        foreach ($eqdid as $key => $value) {
                            echo strtoupper("{$value['descricao']}<br/>");
                        }
                    }
                }
                ?>
            </td>
        </tr>
        <tr>
            <th class="active text-left">Código da subação:</th>
            <td><b><? echo $subacao['sbacod'] ?></b></td>
        </tr>
        <tr>
            <th class="active text-left">Permite Obras?</th>
            <td>
                <?= ($obPi_Subacao->sbaobras == "t") ? 'SIM"' : 'NÃO' ?>
            </td>
        </tr>
        <tr>
            <th class="active text-left" style="width:40%;">Unidades Orçamentárias / Gestoras:</th>
            <td>
                <?php
                if ($sbaid) {
                    $sql = <<<DML
						SELECT
							ung.ungcod AS codigo,
       						ung.ungdsc AS descricao
  						FROM public.unidadegestora ung
    					INNER JOIN monitora.pi_subacaounidade sau ON ung.ungcod = sau.ungcod
  						WHERE sau.sbaid = {$sbaid}
DML;
                    $ungcod = $db->carregar($sql);
                    if ($ungcod) {
                        foreach ($ungcod as $key => $value) {
                            echo "({$value['codigo']}) " . strtoupper("{$value['descricao']}<br />");
                        }
                    }
                }
                ?>
            </td>
        </tr>
    </table>
</div>

<div id="graficoDestino" style="width: 50%; float: left;">
    <?php
    /* Monta o HTML do grafico de Execução */
    $obrigatorias = UNIDADES_OBRIGATORIAS;
    $sqlGrafico = "
    	SELECT
            'Dotação Atual'  AS descricao,
            'Total' as categoria,
            SUM(sadvalor) as valor
		FROM monitora.pi_subacaodotacao
        INNER JOIN monitora.ptres mp USING (ptrid)
        WHERE sbaid = {$sbaid} and mp.ptrano = '{$_SESSION['exercicio']}'
        UNION ALL
        SELECT
            'Empenhado'  AS descricao,
            'Total' as categoria,
            vlrempenhado AS valor
        FROM (SELECT
        		SUM(vlrempenhado) AS vlrempenhado
			FROM spo.siopexecucao sex
			WHERE sex.exercicio = '{$_SESSION['exercicio']}'
				AND SUBSTR(plicod, 2, 4) = '{$subacao['sbacod']}'
				AND unicod IN ({$obrigatorias})
			GROUP BY SUBSTR(plicod, 2, 4) ) foo
	UNION ALL
		SELECT
        	'Liquidado'  AS descricao,
            'Total' as categoria,
            vlrliquidado AS valor
		FROM (SELECT
				SUM(vlrliquidado) AS vlrliquidado
            FROM spo.siopexecucao sex
			WHERE sex.exercicio = '{$_SESSION['exercicio']}'
				AND SUBSTR(plicod, 2, 4) = '{$subacao['sbacod']}' AND unicod IN ({$obrigatorias})
			GROUP BY
            SUBSTR(plicod, 2, 4) ) foo
		UNION ALL
        SELECT
        	'Pago'  AS descricao,
        	'Total' as categoria,
            vlrpago AS valor
		FROM (SELECT
				SUM(vlrpago) AS vlrpago
            FROM spo.siopexecucao sex
			WHERE sex.exercicio = '{$_SESSION['exercicio']}'
			AND SUBSTR(plicod, 2, 4) = '{$subacao['sbacod']}' AND unicod IN ({$obrigatorias})
            GROUP BY
            SUBSTR(plicod, 2, 4) ) foo";
#ver($sqlGrafico,d);
	//$dados = $db->carregar($sqlGrafico);
    $grafico = new Grafico(null, false);
    echo $grafico->setTipo(Grafico::K_TIPO_COLUNA)->setId('graficoSubacao')->setTitulo('Execução Orçamentária')->gerarGrafico($sqlGrafico);
    ?>
</div>

<div style="clear:both" />
<div class="">
    <div class="page-header" style="margin-bottom:5px;">
        <h4>Descrição / Finalidade da Subação</h4>
    </div>
    <p style="text-align: justify;line-height: 150%;font-size: 15px;"><?= ($subacao['sbadsc'])?></p>
</div>


<script>jQuery("#graficoSubacao").appendTo("#graficoDestino");</script>
<script>
    function remaneja(ptrid) {
        window.location.href = "planacomorc.php?modulo=principal/remanejavalorsubacao/remaneja&acao=A&ptrid=" + ptrid;
    }
</script>
<div style="clear:both" />

<?php
$sql = <<<DML
    SELECT
       {$compSql}
       dtl.ptres,
       trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acatitulo) AS descricao,
       uni.unicod || ' - ' || uni.unidsc,
       COALESCE(SUM(dtl.ptrdotacao),0.00) AS dotacaoinicial,
       COALESCE((SUM(dtl.ptrdotacao) - COALESCE(SUM(dt.valor), 0.00)),0.00) AS nao_det_subacao,
       COALESCE(SUM(sd.sadvalor), 0.00) AS dot_ptres_subacao,
       COALESCE(emp_sba_ptres.empenho, 0.00) AS emp_ptres_subacao
  FROM monitora.acao aca
    INNER JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
    INNER JOIN monitora.pi_subacaodotacao sd ON dtl.ptrid = sd.ptrid
    INNER JOIN public.unidade uni ON uni.unicod = dtl.unicod
    LEFT JOIN (SELECT ptrid,
                      SUM(sadvalor) AS valor
                 FROM monitora.pi_subacaodotacao
                 GROUP BY ptrid) dt ON dtl.ptrid = dt.ptrid
    LEFT JOIN (SELECT sbaid,
                      ptrid,
                      SUM(dtl.valorpi) AS valorpi
                 FROM monitora.v_pi_detalhepiptres dtl
                 GROUP BY sbaid,
                          dtl.ptrid) dt2 ON (dtl.ptrid = dt2.ptrid AND dt2.sbaid = sd.sbaid)
    LEFT JOIN (
        SELECT
            SUBSTR(plicod, 2, 4) AS sbacod,
            ptres,
            SUM(total) AS empenho
        FROM
            siafi.pliptrempenho
         WHERE SUBSTR(plicod, 2, 4) = '{$subacao['sbacod']}' AND exercicio = '{$_SESSION['exercicio']}'
        GROUP BY
            SUBSTR(plicod, 2, 4),
            ptres
        ) emp_sba_ptres ON emp_sba_ptres.ptres = dtl.ptres
  WHERE aca.prgano = '{$_SESSION['exercicio']}'
    AND dtl.ptrano='{$_SESSION['exercicio']}'
    AND aca.acasnrap = FALSE
    AND sd.sbaid = {$sbaid}
  GROUP BY dtl.ptrid,
           dtl.ptres,
           descricao,
           uni.unicod,
           uni.unidsc,
           emp_sba_ptres.empenho
  ORDER BY dtl.ptrid
DML;

$list = new Simec_Listagem();
$list->addCallbackDeCampo(array('dotacaoinicial', 'nao_det_subacao', 'emp_ptres_subacao', 'dot_ptres_subacao'), 'mascaraMoeda');
$list->setCabecalho($cabecalho);
if ($_REQUEST['tp'] == 'r') {
    $list->addAcao('send2', 'remaneja');
}
$list->setQuery($sql);
$list->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA,array('dotacaoinicial', 'nao_det_subacao', 'emp_ptres_subacao', 'dot_ptres_subacao'));
$list->setTitulo('Orçamento da Subação');
$list->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);

exit();
