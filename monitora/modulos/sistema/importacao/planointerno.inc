<?php
/**
 * Arquivo de cabeçalho geral do sistema.
 */
require_once(APPRAIZ . 'includes/cabecalho.inc');
$subtitulo = '';
if (isset($_POST['tipo'])) {
    if ("ungcod" == $_POST['tipo']) {
        $subtitulo = 'Administração Direta';
    } elseif ("unicod" == $_POST['tipo']) {
        $subtitulo = 'Outras Unidades';
    }
    if (1 == $_POST['verificacao']) {
        $subtitulo .= ' - Considerar UO/UG na verificação de PI';
    } elseif (2 == $_POST['verificacao']) {
        $subtitulo .= ' - Não considerar UO/UG na verificação de PI';
    }
}
monta_titulo('Importação de PIs', $subtitulo);
?>
<style>
.title td{font-weight:bold}
.light{background-color:#ffffff}
.dark{background-color:#f7f7f7}
.highlight{background-color:#ffffcc !important}
.light td,.dark td,.highlight td,.title td{text-align:center}
.sucesso{color:green}
.erro{color:red}
</style>

<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
<form id="formImportacao" name="formImportacao" method="POST" enctype="multipart/form-data">
    <center>
<?php
if ('save' == $_POST['action']) {
    $total = importaDados($_POST['data']);
    tabulaResultado($_POST['data'], $total);
} elseif ('import' == $_POST['action']) {
    tabulaDados();
} else {
?>
        <table class="tabela" cellspacing="1" cellpadding="3" align="center">
            <input type="hidden" name="action" value="import" />
            <tr>
                <td class="SubTituloDireita">Tipo de Unidade:</td>
                <td>
                    <input type="radio" name="tipo" id="tipo1" value="ungcod" required checked />
                    <label for="tipo1" style="cursor:pointer">Administração Direta</label><br />
                    <input type="radio" name="tipo" id="tipo2" value="unicod" required />
                    <label for="tipo2" style="cursor:pointer">Outras Unidades</label>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Considerar UO/UG na verificação?</td>
                <td>
                    <input type="radio" name="verificacao" id="verificacao1" value="1" required checked />
                    <label for="verificacao1" style="cursor:pointer">Sim</label><br />
                    <input type="radio" name="verificacao" id="verificacao2" value="2" required />
                    <label for="verificacao2" style="cursor:pointer">Não</label>
                </td>
            </tr>
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
<?php } ?>
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
function tabulaDados()
{
    $i = 0;
    $campoUO = $_POST['tipo'];
    $colspan = 12;
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
                    <td>UO/UG</td>
                    <td>PI</td>
                    <?php if ('unicod' == $campoUO): $colspan++; ?>
                    <td>PI Título</td>
                    <?php endif; ?>
                    <td>Subação</td>
                    <td>Categoria da Unidade</td>
                    <td>Enquadramento da Despesa</td>
                    <td>Nível/Etapa de Ensino</td>
                    <td>Categoria de Apropriação</td>
                    <td>Modalidade de Ensino / Tema / Público</td>
                    <td>Ano</td>
                    <td>CPF</td>
                    <td>Existente?</td>
                </tr>
            </thead>
<?php
    $total['existentes'] = $total['serao_inseridos'] = 0;
    foreach (file($_FILES['datafile']['tmp_name']) as $linha) {
        $linhalimpa = strtoupper(trim($linha));
        $primeiraLetra = substr($linhalimpa, 0, 1);
        $ord = ord($primeiraLetra);
        // -- Se o primeiro caracter da linha não for um número, a linha é de PI
        if (!(($ord >= 48) && ($ord <= 57))) {
            list($piETitulo) = explode(';', $linhalimpa);
            $piETitulo = explode(' ', $piETitulo);
            // -- O primeiro elemento é o codigo do PI
            $pi = array_shift($piETitulo);
            // -- O Restante é a descrição do PI
            $pititulo = $pititulodsc = implode(' ', $piETitulo);
            if (empty($pititulo)) {
                $pititulodsc = ' - ';
            }
            $existente = checaInsercao($pi, $uo, $campoUO);
            if ($existente) {
                $total['existentes']++;
                $status = '<span style="color:red">Existente</span>';
            } else {
                $total['serao_inseridos']++;
                $status = '<span style="color:green">Não existente</span>';
            }
            $trClass = (0==$i++%2?'light':'dark');
            $piTitulo = '';
            $inputInsercao = ' - ';
            if (!$existente) {
                $inputInsercao = <<<HTML
                    <input type="checkbox" class="one" name="data[pi][{$i}]" value="{$pi}" />
HTML;
            }
            if ('unicod' == $campoUO) {
                $piTitulo = <<<HTML
                <td>{$pititulodsc}<input type="hidden" name="data[pidsc][{$i}]" value="{$pititulo}" /></td>
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
                <td>{$uo}<input type="hidden" name="data[{$campoUO}][{$i}]" value="{$uo}" /></td>
                <td>{$pi}</td>
                {$piTitulo}
                <td>{$codSubacao}</td>
                <td>{$codCatUnidade}</td>
                <td>{$primeiraLetra}</td>
                <td>{$codNivelEtapa}</td>
                <td>{$codCatApropriacao}</td>
                <td>{$codModalidade}</td>
                <td>{$_SESSION['exercicio']}</td>
                <td>000.000.001-91</td>
                <td>{$status}</td>
            </tr>
HTML;
        } else {
            list($uo) = explode(' ', $linha);
            $uo = trim($uo);
        }
    }
?>
            <tr>
                <td colspan="<?php echo $colspan; ?>">
                    <table class="listagem" width="100%" cellspacing="0" cellpadding="2" border="0" align="center">
                        <tbody>
                            <tr bgcolor="#ffffff">
                                <td><b>Total de Registros: <?php echo ($total['existentes'] + $total['serao_inseridos']); ?></b></td>
                                <td></td>
                            </tr>
                            <tr bgcolor="#ffffff">
                                <td><b>Total de Registros Existentes: <?php echo $total['existentes']; ?></b></td>
                                <td></td>
                            </tr>
                            <tr bgcolor="#ffffff">
                                <td><b>Total de Registros para Importação: <?php echo $total['serao_inseridos']; ?></b></td>
                                <td></td>
                            </tr>
                        </tbody>
                    </table>
                </td>
            </tr>
            <tr style="background-color:#dcdcdc">
                <td colspan="<?php echo $colspan; ?>" style="text-align:center"><button class="botao">Importar</button></td>
            </tr>
        </table>
<?php
}

function importaDados(&$dados)
{
    global $db;

    $total['importados'] = $total['nao_importados'] = $total['existentes'] = 0;
    $ano = $_SESSION['exercicio'];
    $campoUO = (isset($dados['ungcod'])?'ungcod':'unicod');

    $sqlInsert = <<<DML
INSERT INTO monitora.pi_planointerno(pliano, {$campoUO}, plicod, plicodsubacao, plilivre, eqdid, sbaid, secid, capid, arceid, usucpf, plisituacao, plistatus, plititulo, plidsc)
  VALUES('%s', '%s', '%s', '%s', '%s', coalesce((%s), null), coalesce((%s), null), coalesce((%s), null), coalesce((%s), null), coalesce((%s), null), '%s', '%s', '%s', %s, %s)
DML;
    if (!isset($dados['pi'])) {
        return $total;
    }
    foreach ($dados['pi'] as $pikey => $picod) {
        $valores = array(
            'pliano' => $ano,
            $campoUO => $dados[$campoUO][$pikey],
            'plicod' => $picod,
            'plicodsubacao' => substr($picod, 1, 4),
            'plilivre' => substr($picod, 8, 2),
            'eqdid' => "SELECT eqdid FROM planejamento.enquadramento_despesa WHERE eqdcod = '" . substr($picod, 0, 1) . "' AND eqdano = '{$ano}' AND eqdstatus = 'A'",
            'sbaid' => "SELECT MAX(sbaid) FROM monitora.pi_subacao WHERE sbacod = '" . substr($picod, 1, 4) . "'",
            'secid' => "SELECT secid FROM planejamento.segmento_cultural WHERE neecod = '" . substr($picod, 5, 1) . "' AND neeano = '{$ano}' AND neestatus = 'A'",
            'capid' => "SELECT capid FROM planejamento.categoria_apropriacao WHERE capcod = '" . substr($picod, 6, 2) . "' AND capano = '{$ano}' AND capstatus = 'A'",
            'arceid' => "SELECT arceid FROM planejamento.area_cultural WHERE arccod = '" . substr($picod, 10) . "' AND arcano = '{$ano}' AND arcstatus = 'A'",
            'usucpf' => '00000000191',
            'plisituacao' => 'C',
            'plistatus' => 'A'
        );
        if (!isset($dados['pidsc'][$pikey]) || empty($dados['pidsc'][$pikey])) {
            $valores['plititulo'] = "COALESCE((SELECT CASE WHEN TRIM(sbasigla) = '' THEN sbacod ELSE sbasigla END || ' - ' || '{$picod}'
                                                 FROM monitora.pi_subacao s
                                                 WHERE s.sbacod = '" . substr($picod, 1, 4) . "'
                                                   AND s.sbaid = (SELECT MAX(s1.sbaid)
                                                                    FROM monitora.pi_subacao s1
                                                                    WHERE s1.sbacod = '" . substr($picod, 1, 4) . "')), '{$picod}')";
            $valores['plidsc'] = "COALESCE((SELECT CASE WHEN TRIM(sbasigla) = '' THEN sbacod ELSE sbasigla END || ' - ' || '{$picod}'
                                              FROM monitora.pi_subacao s
                                              WHERE s.sbacod = '" . substr($picod, 1, 4) . "'
                                                AND s.sbaid = (SELECT MAX(s1.sbaid)
                                                                 FROM monitora.pi_subacao s1
                                                                 WHERE s1.sbacod = '" . substr($picod, 1, 4) . "')), '{$picod}')";
        } else {
            $valores['plititulo'] = $valores['plidsc'] = "'{$dados['pidsc'][$pikey]}'";
        }

        $existente = checaInsercao($valores['plicod'], $valores[$campoUO], $campoUO);
        if (!$existente) { // -- Novo
            $stmtInsert = vsprintf($sqlInsert, $valores);
            $db->executar($stmtInsert);
            if ($db->commit()) {
                // -- Inserido com sucesso
                $dados['result'][$pikey] = '<span class="sucesso">Sucesso</span>';
                $total['importados']++;
            } else {
                // -- Erro
                $dados['result'][$pikey] = '<span class="erro">Erro</span>';
                $total['nao_importados']++;
            }
        } else {
             // -- Já existe
            $dados['result'][$pikey] = '<span>Existente</span>';
            $total['existentes']++;
        }
    }
    return $total;
}

function tabulaResultado($dados, $total)
{
    $i = 0;
    $campoUO = (isset($dados['ungcod'])?'ungcod':'unicod');
?>
        <table class="listagem" cellspacing="1" cellpadding="3" align="center" width="95%">
            <thead>
                <tr class="title">
                    <td>UO/UG</td>
                    <td>PI</td>
                    <td>Subação</td>
                    <td>Categoria da Unidade</td>
                    <td>Enquadramento da Despesa</td>
                    <td>Nível/Etapa de Ensino</td>
                    <td>Categoria de Apropriação</td>
                    <td>Modalidade de Ensino / Tema / Público</td>
                    <td>Ano</td>
                    <td>CPF</td>
                    <td>INSERIDO?</td>
                </tr>
            </thead>
<?php if (isset($dados['pi'])): ?>
<?php foreach ($dados['pi'] as $pikey => $picod): ?>
            <tr class="<?php echo(0==$i++%2?'light':'dark');?>">
                <td><?php echo $dados[$campoUO][$pikey]; ?></td>
                <td><?php echo $picod; ?></td>
                <td><?php echo substr($picod, 1, 4); ?></td>
                <td><?php echo substr($picod, 8, 2); ?></td>
                <td><?php echo substr($picod, 0, 1); ?></td>
                <td><?php echo substr($picod, 5, 1); ?></td>
                <td><?php echo substr($picod, 6, 2); ?></td>
                <td><?php echo substr($picod, 10); ?></td>
                <td><?php echo $_SESSION['exercicio']; ?></td>
                <td><?php echo '000.000.001-91'; ?></td>
                <td><?php echo $dados['result'][$pikey]; ?></td>
            </tr>
<?php endforeach; ?>
<?php else: ?>
            <tr>
                <td colspan="12">
                    <center><span style="color:red">Sem registros para importação</span></center>
                </td>
            </tr>
<?php endif; ?>
            
            <tr>
                <td colspan="12">
                    <table class="listagem" width="100%" cellspacing="0" cellpadding="2" border="0" align="center">
                        <tbody>
                            <tr bgcolor="#ffffff">
                                <td><b>Total de Registros: <?php echo ($total['importados'] + $total['nao_importados'] + $total['existentes']); ?></b></td>
                                <td></td>
                            </tr>
                            <tr bgcolor="#ffffff">
                                <td><b>Total de Registros Importados: <?php echo $total['importados']; ?></b></td>
                                <td></td>
                            </tr>
                            <tr bgcolor="#ffffff">
                                <td><b>Total de Registros Não Importados: <?php echo $total['nao_importados']; ?></b></td>
                                <td></td>
                            </tr>
                            <tr bgcolor="#ffffff">
                                <td><b>Total de Registros Já Existentes: <?php echo $total['existentes']; ?></b></td>
                                <td></td>
                            </tr>
                        </tbody>
                    </table>
                </td>
            </tr>
            <tr style="background-color:#dcdcdc">
                <td colspan="12" style="text-align:center"><button class="botao" id="voltar">Voltar</button></td>
            </tr>
        </table>
<?php
}

function checaInsercao($pi, $uo, $campoUO)
{
    global $db;

    $sqlVerificacao = <<<DML
SELECT 1 AS qtd
  FROM monitora.pi_planointerno
  WHERE plicod = '%s'
    AND pliano = '%s'
    AND plistatus = '%s'
DML;
    if (1 == $_POST['verificacao']) {
        $sqlVerificacao .= <<<DML
    AND {$campoUO} = '%s'
DML;
    }
    $stmtVerificacao = sprintf($sqlVerificacao, $pi, 2013, 'A', $uo);
    return $db->pegaUm($stmtVerificacao);
}
