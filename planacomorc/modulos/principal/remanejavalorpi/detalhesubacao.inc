<?php
$sbaid = $_GET['sbaid'];
if ($sbaid) {
    $subacao = $db->pegaLinha("SELECT * FROM monitora.pi_subacao WHERE sbaid = {$sbaid}");
    if (!$subacao) {
        echo "<script>
				alert('A Subação informada não existe!');
				history.back(-1);
			  </script>";
        die;
    }

    $sql = <<<DML
SELECT dtl.ptrid,
       dtl.ptres,
       trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acadsc) AS descricao,
       uni.unidsc,
       SUM(dtl.ptrdotacao) AS dotacaoinicial,
       COALESCE(CAST(SUM(dt.valor) AS VARCHAR), '0.00') AS detalhamento,
       COALESCE(CAST(SUM(dt2.valorpi) AS VARCHAR), '0.00') AS detalhamento2,
       (SUM(dtl.ptrdotacao) - COALESCE(SUM(dt.valor), 0.00)) AS diferenca,
       SUM(sd.sadvalor) AS valorcadastrado,
       pte.total
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
    LEFT JOIN siafi.ptrempenho pte ON (pte.ptres = dtl.ptres AND pte.exercicio = aca.prgano)
  WHERE aca.prgano = '{$_SESSION['exercicio']}'
    AND dtl.ptrano='{$_SESSION['exercicio']}'
    AND aca.acasnrap = FALSE
    AND sd.sbaid = {$sbaid}
  GROUP BY dtl.ptrid,
           dtl.ptres,
           descricao,
           uni.unidsc,
           pte.total
  ORDER BY dtl.ptrid
DML;
//ver($sql, d);
    $ptresacao = $db->carregar($sql);
}
?>
<script>
    function abrPtres(prtid)
    {
        if (confirm('Deseja acessar outro PTRES? Todas as informações não salvas do Remanejamento atual serão perdidas.')) {
            window.close();
            window.opener.location.href = 'planacomorc.php?modulo=principal/remanejavalorpi/remaneja&acao=A&ptrid=' + prtid + '&sbaid=<?php echo $sbaid; ?>';
        }
    }
</script>

<style>
    *{font-family:Arial,verdana;font-size:11px}
    .SubTituloDireita{width:300px;font-weight:bold;text-align:right;vertical-align:top}
</style>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td class="SubTituloDireita">Título:</td>
        <td><?php echo $subacao['sbatitulo']; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Título Abreviado / Sigla:</td>
        <td><?php echo $subacao['sbasigla']; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Enquadramento da Despesa:</td>
        <td>
            <?php
            if ($sbaid) {
                $sql = <<<DML
SELECT ed.eqdid as codigo,
       ed.eqdcod ||' - '|| ed.eqddsc AS descricao
  FROM planejamento.enquadramento_despesa ed
    INNER JOIN monitora.pi_subacaoenquadramento sae ON ed.eqdid = sae.eqdid
  WHERE sae.sbaid = {$sbaid}
    AND ed.eqdano = '{$_SESSION['exercicio']}'
DML;
                #ver($sql,d);
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
        <td class="SubTituloDireita">Código da subação:</td>
        <td><b><? echo $subacao['sbacod'] ?></b></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Permite Obras?</td>
        <td>
            <?= ($obPi_Subacao->sbaobras == "t") ? 'SIM"' : 'NÃO' ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Unidades Orçamentárias / Gestoras:</td>
        <td>
            <?php
            if ($sbaid) {
                $sql = <<<DML
SELECT ung.ungcod AS codigo,
       ung.ungdsc AS descricao
  FROM public.unidadegestora ung
    INNER JOIN monitora.pi_subacaounidade sau ON ung.ungcod = sau.ungcod
  WHERE sau.sbaid = {$sbaid}
DML;
                $ungcod = $db->carregar($sql);
                if ($ungcod) {
                    foreach ($ungcod as $key => $value) {
                        echo strtoupper("{$value['descricao']}<br />");
                    }
                }
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Descrição / Finalidade:</td>
        <td><?php echo strtoupper($subacao['sbadsc']); ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Ações:</td>
        <td>
            <table cellpadding="3" border="0" width="98%" id="orcamento" style="border:#c9c9c9 1px solid">
                <tr style="background-color: #C9C9C9;">
                    <td colspan="8" align="center"><b>Detalhamento Orçamentário</b></td>
                </tr>
                <tr style="background-color:#c9c9c9;text-align:center;font-weight:bold">
                    <td nowrap>PTRES<input type="hidden" name="ptrid"></td>
                    <td style="width:45%" nowrap>Ação</td>
                    <td style="width:100px" nowrap>Dotação Autorizada</td>
                    <td style="width:100px" nowrap>Detalhado em<br />SubAção</td>
                    <td style="width:100px" nowrap>Detalhado em PI</td>
                    <td style="width:100px" nowrap>Empenhado total do PTRES</td>
                    <td style="width:100px">Não detalhado em PI</td>
                    <td>Detalhado nessa Subação</td>
                </tr>
                <?
                if ($ptresacao[0]) {
                    $valortotal = 0;
                    $cor = 0;
                    foreach ($ptresacao as $acpl) {
                        ?>
                        <tr style="height:30px;<? echo (($cor % 2) ? "" : "background-color:#dcdcdc;"); ?>;text-align:right" id="ptrid_<? echo $acpl['ptrid']; ?>">
                            <td align="center"><a href="javascript:abrPtres('<?= $acpl['ptrid'] ?>')"><? echo $acpl['ptres']; ?></a></td>
                            <td align="left"><? echo $acpl['descricao']; ?><input type='hidden' name='ptrid[<? echo $acpl['ptrid']; ?>]' value='<? echo $acpl['ptrid']; ?>'></td>
                            <td>R$ <? echo number_format($acpl['dotacaoinicial'], 2, ',', '.'); ?></td>
                            <td>R$ <? echo number_format($acpl['detalhamento'], 2, ',', '.'); ?></td>
                            <td>R$ <? echo number_format($acpl['detalhamento2'], 2, ',', '.'); ?></td>
                            <td>R$ <?php echo number_format($acpl['total']); ?></td>
                            <td>R$ <? echo number_format(($acpl['dotacaoinicial'] - $acpl['detalhamento']), 2, ',', '.'); ?></td>
                            <td>R$ <? echo number_format($acpl['valorcadastrado'], 2, ',', '.'); ?></td>
                        </tr>
                        <?
                        $cor++;
                        $valortotal = $valortotal + $acpl['valorcadastrado'];
                    }
                }
                ?>
                <tr style="height:30px;vertical-align:top;text-align:right">
                    <td colspan="7"><b>TOTAL :</b></td>
                    <td>R$ <? echo number_format($valortotal, 2, ',', '.'); ?></td>
                </tr>
            </table>
        </td>
    </tr>
</table>

