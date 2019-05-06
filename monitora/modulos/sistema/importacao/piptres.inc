<?php
/**
 * Arquivo de cabeçalho geral do sistema.
 */
require_once(APPRAIZ . 'includes/cabecalho.inc');
monta_titulo('Importação de PIs + PTRES', '');
?>
<style>
.title td{font-weight:bold}
.light{background-color:#ffffff}
.dark{background-color:#f7f7f7}
.highlight{background-color:#ffffcc !important}
.light td,.dark td,.highlight td,.title td{text-align:center}
.good{color:green}
.bad{color:red;font-weight:bold}
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
<form id="formImportacao" name="formImportacao" method="POST" enctype="multipart/form-data">
    <center>
<?php
if ('save' == $_POST['action']) {
    $total = importaDados($_POST['data']);
    tabulaResultado($total);
} elseif ('import' == $_POST['action']) {
    tabulaDados();
} else {
    formularioImportacao();
}
?>
    </center>
</form>
<script type="text/javascript" language="javascript">
    jQuery('.dark, .light').mouseover(function(){
        jQuery(this).addClass('highlight');
    }).mouseout(function(){
        jQuery(this).removeClass('highlight');
    });
    jQuery('#all').click(function(){
        jQuery('.one').prop('checked', jQuery(this).is(':checked'));
    });
    jQuery('#voltar').click(function(){
        window.href = 'monitora.php?modulo=sistema/importacao/planointerno&acao=A';
    });
</script>
<?php
function formularioImportacao()
{?>
        <table class="tabela" cellspacing="1" cellpadding="3" align="center">
            <input type="hidden" name="action" value="import" />
            <tr>
                <td class="SubTituloDireita">Arquivo de dados (.csv):</td>
                <td>
                    <input type="file" name="datafile" id="datafile" />
                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">&nbsp;</td>
                <td><button class="botao">Verificar Carga</button></td>
            </tr>
        </table>
<script type="text/javascript" language="javascript">
    jQuery('#formImportacao').submit(function(){
        if (!(jQuery('#datafile').val())) {
            alert('A escolha de arquivo de dados é obrigatória.');
            return false;
        }
    });
</script>
<?php
}

function tabulaDados()
{
    $i = 0;
    $campoUO = $_POST['tipo'];
?>
        <table class="listagem" cellspacing="1" cellpadding="3" align="center" width="95%">
            <input type="hidden" name="action" value="save" />
            <input type="hidden" name="verificacao" value="<?php echo $_POST['verificacao']; ?>" />
            <input type="hidden" name="tipo" value="<?php echo $_POST['tipo']; ?>" />
            <?php if (!is_uploaded_file($_FILES['datafile']['tmp_name'])): ?>
            <tr>
                <td style="color:red;text-align:center">O arquivo enviado é inválido.</td>
            </tr>
            <tr>
                <td style="text-align:center"><button class="botao" id="voltar">Voltar</button></td>
            </tr>
            <?php endif; ?>
            <thead>
                <tr class="title">
                    <td><input type="checkbox" id="all" /></td>
                    <td><?php echo strtoupper($_POST['tipo']); ?></td>
                    <td>PTRES</td>
                    <td>PI</td>
                    <td>PI Título</td>
                    <td>Dot. Inicial</td>
                    <td>Dot. Atualizada</td>
                    <td>Subação</td>
                    <td>Categoria da Unidade</td>
                    <td>Enquadramento da Despesa</td>
                    <td>Nível/Etapa de Ensino</td>
                    <td>Categoria de Apropriação</td>
                    <td>Modalidade de Ensino / Tema / Público</td>
                    <td>Ano</td>
                    <td>PI Existente?</td>
                    <td>PTRES Existente?</td>
                    <td>PI + PTRES Existente?</td>
                    <td>Dot. Atualizada Confere?</td>
                </tr>
            </thead>
<?php
    $total['total'] = 0;
    $total['pi_nao_existentes'] = 0;
    $total['ptres_nao_existentes'] = 0;
    $total['pi+ptres_nao_existentes'] = 0;
    $total['dot_atualizada_nao_confere'] = 0;

    foreach (file($_FILES['datafile']['tmp_name']) as $linha) {
        $piStatus = '<span>-</span>';
        $ptresStatus = '<span>-</span>';
        $piptresStatus = '<span>-</span>';
        $dotacaoStatus = '<span>-</span>';

        $linhalimpa = strtoupper(trim($linha));
        $primeiraLetra = substr($linhalimpa, 1, 1);
        $ord = ord($primeiraLetra);
        // -- Se o primeiro caracter da linha não for um número, a linha é de PI
        if (!(($ord >= 48) && ($ord <= 57))) {
            $total['total']++;
            list($_, $pi, $_, $piTitulo, $ptres, $_, $dotInicial, $dotAtualizada) = explode(';', $linhalimpa);
            $_dotInicial = trim(str_replace(array('.', ','), array('', '.'), $dotInicial));
            $_dotAtualizada = trim(str_replace(array('.', ','), array('', '.'), $dotAtualizada));
            if (empty($_dotInicial)) {
                $_dotInicial = '0.00';
            }
            if (empty($_dotAtualizada)) {
                $_dotAtualizada = '0.00';
            }

            if (empty($piTitulo)) {
                $piTitulo = ' - ';
            }
            if ($piExistente = checaPI($pi, $uo)) {
                $piStatus = '<span class="good">Sim</span>';
            } else {
                $piStatus = '<span class="bad">Não</span>';
                $total['pi_nao_existentes']++;
            }
            if ($ptresExistente = checaPtres($ptres)) {
                $ptresStatus = '<span class="good">Sim</span>';
            } else {
                $ptresStatus = '<span class="bad">Não</span>';
                $total['ptres_nao_existentes']++;
            }
            if ($piptresExistente = checaPIPTRES($pi, $ptres, $uo)) {
                $piptresStatus = '<span class="good">Sim</span>';
                if ($_dotAtualizada == $piptresExistente) {
                    $dotacaoStatus = '<span class="good">Sim</span>';
                } else {
                    $dotacaoStatus = '<span class="bad">Não</span><br />(' . $piptresExistente . ')';
                    $total['dot_atualizada_nao_confere']++;
                }
            } else {
                $piptresStatus = '<span class="bad">Não</span>';
                $total['pi+ptres_nao_existentes']++;
            }

            $trClass = (0==$i++%2?'light':'dark');
            $inputInsercao = ' - ';
            if (!$piExistente || !$ptresExistente || !$piptresExistente || $_dotAtualizada != $piptresExistente) {
                $inputInsercao = <<<HTML
                    <input type="checkbox" class="one" name="data[pi][{$i}]" value="{$pi}" />
HTML;
            }
            $codSubacao = substr($pi, 1, 4);
            $codCatUnidade = substr($pi, 8, 2);
            $codNivelEtapa = substr($pi, 5, 1);
            $codCatApropriacao = substr($pi, 6, 2);
            $codModalidade = substr($pi, 10);
            echo <<<HTML
            <tr class="{$trClass}">
                <td>{$inputInsercao}</td>
                <td>{$uo}<input type="hidden" name="data[unicod][{$i}]" value="{$uo}" /></td>
                <td>{$ptres}<input type="hidden" name="data[ptres][{$i}]" value="{$ptres}" /></td>
                <td>{$pi}</td>
                <td>{$piTitulo}<input type="hidden" name="data[pidsc][{$i}]" value="{$piTitulo}" /></td>
                <td>{$dotInicial}<input type="hidden" name="data[doti][{$i}]" value="{$_dotInicial}" /></td>
                <td>{$dotAtualizada}<input type="hidden" name="data[dota][{$i}]" value="{$_dotAtualizada}" /></td>
                <td>{$codSubacao}</td>
                <td>{$codCatUnidade}</td>
                <td>{$primeiraLetra}</td>
                <td>{$codNivelEtapa}</td>
                <td>{$codCatApropriacao}</td>
                <td>{$codModalidade}</td>
                <td>{$_SESSION['exercicio']}</td>
                <td>{$piStatus}</td>
                <td>{$ptresStatus}</td>
                <td>{$piptresStatus}</td>
                <td>{$dotacaoStatus}</td>
            </tr>
HTML;
        } else {
            list($uo) = explode(' ', $linha);
            $uo = trim($uo);
        }
    }
?>
            <tr>
                <td colspan="18">
                    <table class="listagem" width="100%" cellspacing="0" cellpadding="2" border="0" align="center">
                        <tbody>
                            <tr bgcolor="#ffffff">
                                <td><b>Total de Registros: <?php echo ($total['total']); ?></b></td>
                                <td>&nbsp;</td>
                            </tr>
                            <tr bgcolor="#ffffff">
                                <td><b>PIs não Existentes: <?php echo ($total['pi_nao_existentes'] . ' / ' . $total['total']); ?></b></td>
                                <td>&nbsp;</td>
                            </tr>
                            <tr bgcolor="#ffffff">
                                <td><b>PTRES não Existentes: <?php echo ($total['ptres_nao_existentes'] . ' / ' . $total['total']); ?></b></td>
                                <td>&nbsp;</td>
                            </tr>
                            <tr bgcolor="#ffffff">
                                <td><b>PI + PTRES não Existentes: <?php echo $total['pi+ptres_nao_existentes'] . ' / ' . $total['total']; ?></b></td>
                                <td>&nbsp;</td>
                            </tr>
                            <tr bgcolor="#ffffff">
                                <td><b>Dotações para Atualizar: <?php echo $total['dot_atualizada_nao_confere'] . ' / ' . $total['total']; ?></b></td>
                                <td>&nbsp;</td>
                            </tr>
                        </tbody>
                    </table>
                </td>
            </tr>
            <tr style="background-color:#dcdcdc">
                <td colspan="18" style="text-align:center"><button class="botao">Importar</button></td>
            </tr>
        </table>
<?php
}

function checaPI($pi, $uo)
{
    global $db;
    $sqlVerificacao = <<<DML
SELECT 1
  FROM monitora.pi_planointerno pli
--    LEFT JOIN public.unidadegestora udg USING(ungcod)
  WHERE pli.plicod = '%s'
    AND pli.pliano = '%s'
    AND pli.plistatus = 'A'
--    AND COALESCE(pli.unicod, udg.unicod) = '%s'
DML;
    $stmtVerificacao = sprintf($sqlVerificacao, $pi, $_SESSION['exercicio'], $uo);
    return $db->pegaUm($stmtVerificacao);
}

function checaPtres($ptres)
{
    global $db;
    $sqlVerificacao = <<<DML
SELECT 1
  FROM monitora.ptres ptr
  WHERE ptr.ptres = '%s'
    AND ptr.ptrano = '%s'
    AND ptr.ptrstatus = 'A'
DML;
    $stmtVerificacao = sprintf($sqlVerificacao, $ptres, $_SESSION['exercicio']);
    return $db->pegaUm($stmtVerificacao);
}

function checaPIPTRES($pi, $ptres, $uoug)
{
    global $db;
    $sqlVerificacao = <<<DML
SELECT pip.pipvalor
  FROM monitora.pi_planointerno pli
--    LEFT JOIN public.unidadegestora udg USING(ungcod)
    INNER JOIN monitora.pi_planointernoptres pip USING(pliid)
    INNER JOIN monitora.ptres ptr USING(ptrid)
  WHERE pli.plicod = '%s'
    AND ptr.ptres = '%s'
    AND pli.pliano = '%s'
    AND pli.plistatus = 'A'
--    AND coalesce(pli.unicod, udg.unicod) = '%s'
DML;

    $stmtVerificacao = sprintf($sqlVerificacao, $pi, $ptres, $_SESSION['exercicio'], $uoug);
    return $db->pegaUm($stmtVerificacao);
}

function importaDados($dados)
{
    global $db;
    $total['importados'] = $total['total'] = 0;
    $total['pi_importados'] = $total['pi_nao_importados'] = 0;
    $total['pi_ptres_importados'] = $total['pi_ptres_nao_importados'] = 0;
    $total['dot_atualizadas'] = $total['dot_atualizadas'] = 0;

    foreach ($dados['pi'] as $key => $pi) {
        if (!checaPI($pi, $dados['unicod'][$key])) {
            // -- Insere PI
            if (inserePI(
                    $pi,
                    $dados['pidsc'][$key],
                    $dados['unicod'][$key],
                    $dados['ptres'][$key],
                    $dados['dota'][$key],
                    $dados['doti'][$key]
                )) {
                $total['pi_importados']++;
            }else{
                $total['pi_nao_importados']++;
            }
        } elseif (!($dotAtualizada = checaPIPTRES($pi, $dados['ptres'][$key], $dados['unicod'][$key]))) {
            // -- Associação do PI + PTRES
            if (inserePIPTRES($pi, $dados['ptres'][$key], $dados['unicod'][$key], $dados['dota'][$key], $dados['doti'][$key])) {
                $total['pi_ptres_importados']++;
            } else {
                $total['pi_ptres_nao_importados']++;
            }
        } elseif ($dotAtualizada && ($dotAtualizada != $dados['dota'][$key])) {
            if (atualizacaoDotacao($pi, $dados['ptres'][$key], $dados['unicod'][$key], $dados['dota'][$key], $dados['doti'][$key])) {
                $total['dot_atualizadas']++;
            } else {
                $total['dot_atualizadas']++;
            }
        }
    }
    return $total;
}

function inserePI($pi, $pidsc, $unicod, $ptres, $dotAtualizada, $dotInicial)
{
    global $db;
    $ano = $_SESSION['exercicio'];
    $valores = array(
        'pliano' => $ano,
        'unicod' => $unicod,
        'plicod' => $pi,
        'plicodsubacao' => substr($pi, 1, 4),
        'plilivre' => substr($pi, 8, 2),
        'eqdid' => "SELECT eqdid FROM planejamento.enquadramento_despesa WHERE eqdcod = '" . substr($pi, 0, 1) . "' AND eqdano = '{$ano}' AND eqdstatus = 'A'",
        'sbaid' => "SELECT MAX(sbaid) FROM monitora.pi_subacao WHERE sbacod = '" . substr($pi, 1, 4) . "'",
        'neeid' => "SELECT neeid FROM monitora.pi_niveletapaensino WHERE neecod = '" . substr($pi, 5, 1) . "' AND neeano = '{$ano}' AND neestatus = 'A'",
        'capid' => "SELECT capid FROM planejamento.categoria_apropriacao WHERE capcod = '" . substr($pi, 6, 2) . "' AND capano = '{$ano}' AND capstatus = 'A'",
        'mdeid' => "SELECT mdeid FROM planejamento.area_cultural WHERE mdecod = '" . substr($pi, 10) . "' AND mdeano = '{$ano}' AND mdestatus = 'A'",
        'usucpf' => '00000000191',
        'plisituacao' => 'C',
        'plistatus' => 'A'
    );
    if (!empty($pidsc)) {
        $valores['plititulo'] = $valores['plidsc'] = "'{$pidsc}'";
    } else {
        $valores['plititulo'] = "COALESCE((SELECT CASE WHEN TRIM(sbasigla) = '' THEN sbacod ELSE sbasigla END || ' - ' || '{$pi}'
                                             FROM monitora.pi_subacao s
                                             WHERE s.sbacod = '" . substr($pi, 1, 4) . "'
                                               AND s.sbaid = (SELECT MAX(s1.sbaid)
                                                                FROM monitora.pi_subacao s1
                                                                WHERE s1.sbacod = '" . substr($pi, 1, 4) . "')), '{$pi}')";
        $valores['plidsc'] = $valores['plititulo'];
    }
    $insertPI = <<<DML
INSERT INTO monitora.pi_planointerno(pliano, unicod, plicod, plicodsubacao, plilivre, eqdid, sbaid, neeid,
                                     capid, mdeid, usucpf, plisituacao, plistatus, plititulo, plidsc)
  VALUES('%s', '%s', '%s', '%s', '%s', coalesce((%s), null), coalesce((%s), null), coalesce((%s), null),
         coalesce((%s), null), coalesce((%s), null), '%s', '%s', '%s', %s, %s)
  RETURNING pliid
DML;
    $stmtInsert = vsprintf($insertPI, $valores);
    $pliid = $db->pegaUm($stmtInsert);
    associaPIPTRES($pliid, $ptres, $dotAtualizada, $dotInicial);
    if ($db->commit()) {
        return true;
    } else {
        $db->rollback();
        return false;
    }
}

function associaPIPTRES($pliid, $ptres, $dotAtualizada, $dotInicial)
{
    global $db;
    $insert = <<<DML
INSERT INTO monitora.pi_planointernoptres(pliid, ptrid, pipvalor, pipvalorinicial)
  VALUES(%f, (SELECT ptr.ptrid
                FROM monitora.ptres ptr
                WHERE ptr.ptres = '%s'
                  AND ptr.ptrano = '%s'
                  AND ptr.ptrstatus = 'A'), %f, %f)
DML;
    $stmt = sprintf($insert, $pliid, $ptres, $_SESSION['exercicio'], $dotAtualizada, $dotInicial);
    $db->executar($stmt);
}

function inserePIPTRES($pi, $ptres, $unicod, $dotAtualizada, $dotInicial)
{
    global $db;
    $insert = <<<DML
INSERT INTO monitora.pi_planointernoptres(pliid, ptrid, pipvalor, pipvalorinicial)
  VALUES((SELECT MAX(pliid)
            FROM monitora.pi_planointerno pli
--              LEFT JOIN public.unidadegestora udg USING(ungcod)
            WHERE pli.plicod = '%s'
              AND pli.pliano = '%s'
              AND pli.plistatus = 'A'
--              AND COALESCE(pli.unicod, udg.unicod) = '%s'
         ),
         (SELECT MAX(ptr.ptrid)
                FROM monitora.ptres ptr
                WHERE ptr.ptres = '%s'
                  AND ptr.ptrano = '%s'
                  AND ptr.ptrstatus = 'A'),
         %f,
         %f)
DML;
    $stmt = sprintf($insert, $pi, $_SESSION['exercicio'], $unicod, $ptres, $_SESSION['exercicio'], $dotAtualizada, $dotInicial);
    $db->executar($stmt);
    if ($db->commit()) {
        return true;
    } else {
        $db->rollback();
        return false;
    }
}

function atualizacaoDotacao($pi, $ptres, $unicod, $dotAtualizada, $dotInicial)
{
    global $db;
    $update = <<<DML
UPDATE monitora.pi_planointernoptres
  SET pipvalor = %f,
      pipvalorinicial = %f
  WHERE pliid = (SELECT pliid
                   FROM monitora.pi_planointerno pli
--                     LEFT JOIN public.unidadegestora udg USING(ungcod)
                   WHERE pli.plicod = '%s'
                     AND pli.pliano = '%s'
                     AND pli.plistatus = 'A'
--                     AND COALESCE(pli.unicod, udg.unicod) = '%s'
                )
    AND ptrid = (SELECT MAX(ptr.ptrid)
                   FROM monitora.ptres ptr
                   WHERE ptr.ptres = '%s'
                     AND ptr.ptrano = '%s'
                     AND ptr.ptrstatus = 'A')
DML;
    $stmt = sprintf($update, $dotAtualizada, $dotInicial, $pi, $_SESSION['exercicio'], $unicod, $ptres, $_SESSION['exercicio']);
    $db->executar($stmt);
    if ($db->commit()) {
        return true;
    } else {
        $db->rollback();
        return false;
    }
}


function tabulaResultado($total)
{?>
    <table class="listagem" cellspacing="1" cellpadding="3" align="center" width="95%">
        <tr>
            <td colspan="<?php echo $colspan; ?>">
                <table class="listagem" width="100%" cellspacing="0" cellpadding="2" border="0" align="center">
                    <tbody>
                        <tr bgcolor="#ffffff">
                            <td><b>Total de PIs Importados:
                                <?php echo ($total['pi_importados']) . ' / '  . ($total['pi_importados'] + $total['pi_nao_importados']); ?></b></td>
                            <td></td>
                        </tr>
                        <tr bgcolor="#ffffff">
                            <td><b>Total de PI+PTRES Associados:
                                <?php echo ($total['pi_ptres_importados']) . ' / '  . ($total['pi_ptres_importados'] + $total['pi_ptres_nao_importados']); ?></b></td>
                            <td></td>
                        </tr>
                        <tr bgcolor="#ffffff">
                            <td><b>Total de Dotações Atualizadas:
                                <?php echo ($total['dot_atualizadas']) . ' / '  . ($total['dot_atualizadas'] + $total['dot_nao_atualizadas']); ?></b></td>
                            <td></td>
                        </tr>
                    </tbody>
                </table>
            </td>
        </tr>
        <tr style="background-color:#dcdcdc">
            <td colspan="<?php echo $colspan; ?>" style="text-align:center"><button class="botao" id="voltar">Voltar</button></td>
        </tr>
    </table>
<?php
}

