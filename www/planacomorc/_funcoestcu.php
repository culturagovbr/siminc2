<?php
/**
 * Arquivo de fun��es da funcionalidade relat�rio do TCU.
 *
 * @package SiMEC
 * @subpackage planej-acomp-orcamentario
 * @version $Id: _funcoestcu.php 93248 2015-01-28 18:08:38Z lindalbertofilho $
 */

/**
 * Consulta os dados de um relat�rio completo, ou de um item do relat�rio e retorna um array com estes dados.
 *
 * @global cls_banco $db DBAL do banco de dados.
 * @param int $rlgid Id do relat�rio.
 * @param int $rldid Id do item do relat�rio.
 * @return array|bool
 * @throws Exception Lan�ado quando n�o s�o encontrados dados.
 */
function consultarDadosTCU($rlgid, $rldid = null)
{
    global $db;

    $query = <<<DML
SELECT CASE WHEN rld.rldtipo = 'po' THEN rld.rldcod
            ELSE rlg.acacod
         END AS codigo,
       rlg.unicod,
       rlg.exercicio,
       rld.*
  FROM planacomorc.relgestao rlg
    INNER JOIN planacomorc.relgdados rld USING(rlgid)
  WHERE rlg.rlgid = %d __RLDID__
  ORDER BY rld.rldtipo
DML;

    $params = is_null($rldid)?array($rlgid):array($rlgid, $rldid);
    $query = str_replace(
        '__RLDID__',
        (is_null($rldid)?'':' AND rld.rldid = %d'),
        $query
    );
    $stmt = vsprintf($query, $params);

    if (!$dados = $db->carregar($stmt)) {
        throw new Exception('Dados n�o encontrados.');
    }

    return $dados;
}

/**
 * Recebe um template html de um relat�rio com coringas e os substitu� por valores informados em um conjunto de dados.
 *
 * O campo exercicio-anterior � um campo calculado. Campos num�ricos e monet�rios recebem m�scara.
 * @param string $templateHtml String contendo o html de um relat�rio.
 * @param array $dados Conjunto de dados para substitui��o no template.
 * @return string
 */
function preencheTemplate($templateHtml, array $dados)
{
    $camposNumericos = array(
        'rldmontanteprevisto',
        'rldmontantereprogramado',
        'rldmontanterealizado',
        'rldrapearealizado'
    );
    $camposMonetarios = array(
        'rlddotacaoinicial',
        'rlddotacaofinal',
        'rlddespempenhada',
        'rlddespliquidada',
        'rlddesppaga',
        'rldrapinscprocessado',
        'rldrapinscnaoprocessado',
        'rldrapeaem0101',
        'rldrapeavalorliquidado',
        'rldrapeavalorcancelado'
    );

    // -- Campo calculado exercicio-anterior
    $dados['exercicio-anterior'] = $dados['exercicio'] - 1;

    // -- Processando cada campo e substitu�ndo o valor no template
    foreach ($dados as $campo => $valor) {
        // -- Processando o campo de a��o priorit�ria
        if ('rldacaoprioritaria' == $campo) {
            $rldacaoprioritaria_t = $rldacaoprioritaria_f = '';
            $valor = $valor?$valor:'f';
            ${"rldacaoprioritaria_{$valor}"} = 'X';
            $templateHtml = str_replace(
                array('%rldacaoprioritaria_t%', '%rldacaoprioritaria_f%'),
                array($rldacaoprioritaria_t, $rldacaoprioritaria_f),
                $templateHtml
            );
            continue;
        }

        // -- Processando o campo de tipo de a��o priorit�ria
        if ('rldacaoprioritariatipo' == $campo) {
            $rldacaoprioritariatipo_p = $rldacaoprioritariatipo_b = $rldacaoprioritariatipo_o = '';
            $valor = strtolower($valor);
            if ('t' == $dados['rldacaoprioritaria']) {
                ${"rldacaoprioritariatipo_{$valor}"} = 'X';
            }
            $templateHtml = str_replace(
                array('%rldacaoprioritariatipo_p%', '%rldacaoprioritariatipo_b%', '%rldacaoprioritariatipo_o%'),
                array($rldacaoprioritariatipo_p, $rldacaoprioritariatipo_b, $rldacaoprioritariatipo_o),
                $templateHtml
            );
            continue;
        }

        // -- Formatando valores monet�rios
        if (in_array($campo, $camposMonetarios)) {
            $valor = mascaraMoeda($valor, false);
        }

        // -- Formatando valores num�ricos
        if (in_array($campo, $camposNumericos)) {
            $valor = mascaraNumero($valor);
        }

        // -- Substitu�ndo os curingas por valores
        $templateHtml = str_replace("%{$campo}%", $valor, $templateHtml);
    }

    return $templateHtml;
}

/**
 * Gerencia a exporta��o do relat�rio, seja em XLS ou PDF.
 *
 * @param string $formato Formato de expota��o do relat�rio. Valores v�lidos "pdf" e "xls".
 * @param integer $rlgid ID do relat�rio.
 * @param integer $rldid ID do componente do relat�rio (acao, po ou acaoRap).
 * @throws Exception Lan�ada caso a fun��o receba um tipo inv�lido de sa�da do relat�rio.
 */
function exportarRelatorio($formato, $rlgid, $rldid = null)
{
    if (($formato != 'xls') && ($formato != 'pdf')) {
        throw new Exception('Extens�o desconhecida. Apenas "pdf" e "xls" s�o extens�es v�lidas.');
    }

    // -- Carregando os dados do relat�rio
    $dados = consultarDadosTCU($rlgid, $rldid);

    // -- Processa as linhas de dados e carrega o template para substitui��o
    $relatorioHtml = array();

    foreach ($dados as $dadosRelatorio) {
        $templateHtml = file_get_contents(
            APPRAIZ . "planacomorc/modulos/principal/relatoriogestao/{$dadosRelatorio['rldtipo']}/html-{$formato}.php"
        );

        $relatorioHtml[] = preencheTemplate($templateHtml, $dadosRelatorio)
            . relatorioQuestionario($dadosRelatorio['rldid']);
    }

    // -- Constru�ndo o arquivo completo e adicionando o CSS
    $relatorioHtml = file_get_contents(
        APPRAIZ . "planacomorc/modulos/principal/relatoriogestao/css-relatorio.php"
    ) . implode('<hr class="quadro-tcu" />', $relatorioHtml);

    // -- Gerando a sa�da do relat�rio para exporta��o

    $call = "html2" . ucfirst($formato);
    $call($relatorioHtml);
}

/**
 * Imprime um conte�do em formato Xls, trocando os readers de resposta da requisi��o.
 * @param string $content Conte�do para convers�o em Xls.
 */
function html2Xls($content)
{
	header("Content-Type:   application/vnd.ms-excel; charset=utf-8");
	header("Content-type:   application/x-msexcel; charset=utf-8");
	header("Content-Disposition: attachment; filename=relatorio-tcu.xls");
	header("Expires: 0");
	header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
	header("Cache-Control: private", false);

    echo $content;
    die();
}

/**
 * Imprime um conte�do em formato Pdf, trocando os readers de resposta da requisi��o.
 * @param string $content Conte�do para convers�o em Pdf.
 */
function html2Pdf($content)
{
    // -- Preparando a requisi��o ao webservice de convers�o de HTML para PDF do MEC.
    $content = http_build_query(
        array ('conteudoHtml' => utf8_encode($content))
    );

    $context = stream_context_create(
        array(
            'http' => array(
                'method' => 'POST',
                'content' => $content
            )
        )
    );

    // -- Fazendo a requisi��o de convers�o
    $contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context);

    header('Content-Type: application/pdf');
    header("Content-Disposition: attachment; filename=relatorio-tcu.pdf");
    echo $contents;
    exit();
}

function relatorioQuestionario($rldid)
{
    global $db;
    $sql = <<<DML
SELECT ptcq.tqtid,
       ptcq.qtdexercicio,
       ptcq.tqtdsc AS questionario,
       ptcp.tqpid,
       ptcp.tqppergunta AS pergunta,
       ptcr.tqrid,
       ptcr.tqrresposta AS resposta
  FROM planacomorc.tcuquestionario ptcq
    INNER JOIN planacomorc.tcuquestpergunta ptcp USING(tqtid)
    LEFT JOIN planacomorc.tcuquestresposta ptcr ON (ptcp.tqpid = ptcr.tqpid AND rldid = %d)
  WHERE ptcq.tqtestado = 'A'
    AND ptcp.tqpestado = 'A'
    AND qtdexercicio = '%s'
DML;
    $stmt = sprintf($sql, $rldid, $_SESSION['exercicio']);

    $html = '';
    if ($questionario = $db->carregar($stmt)) {

        $html = <<<HTML
<div class="quadro-tcu">
    <br />
    <table border="1">
        <thead>
            <tr style="text-align:center;font-weight:bold">
                <th colspan="7">{$questionario[0]['questionario']}</th>
            </tr>
        </thead>
        <tbody>
HTML;

        $i = 1;
        foreach ($questionario as $questao){
            $html .= <<<HTML
            <tr><td colspan="7" style="font-weight:bold">{$i}) {$questao['pergunta']}</td></tr>
            <tr><td colspan="7"><span style="font-weight:bold">Resposta: </span>{$questao['resposta']}</td></tr>
HTML;
            $i++;
        }
        $html .= <<<HTML
        </tbody>
    </table>
</div>
HTML;
    }

    return $html;
}

/**
 * Insere ou Atualiza as respostas relacionadas a um question�rio de uma A��o e de seus Planos or�ament�rios
 * @param unknown $dados
 */
function trabalhaQuestionarioRelatorioGestao($dados)
{
    global $db;
    $multipleQuery = '';
    foreach($dados['tqpid'] as $key => $value){
        $sql = "SELECT tqrid FROM planacomorc.tcuquestresposta WHERE tqpid = $key AND rldid = {$dados['rldid']};";

        $resultado = $db->pegaUm($sql);
        if ($resultado) {
            $array = array('tqrresposta'=>$value,'tqpid'=>$key,'tqrid'=>$resultado,'rldid'=>$dados['rldid']);
            $multipleQuery .= atualizaResposta($array);
        } else {
            $array = array('tqrresposta'=>$value,'tqpid'=>$key,'rldid'=>$dados['rldid']);
            $multipleQuery .= insereResposta($array);
        }
    }
    $db->executar($multipleQuery);
    return $db->commit();
}

function insereResposta($dados){
    $sql = "
    INSERT
    INTO planacomorc.tcuquestresposta(
        tqrresposta,
        tqrusuario,
        tqpid,
        rldid)
    VALUES(
        '{$dados['tqrresposta']}',
        '{$_SESSION['usucpf']}',
        {$dados['tqpid']},
        {$dados['rldid']}
    );";
    return $sql;
}

function atualizaResposta($dados){
    $sql = "
    UPDATE planacomorc.tcuquestresposta
        SET
        tqrresposta = '{$dados['tqrresposta']}',
        tqrusuario = '{$_SESSION['usucpf']}'
    WHERE tqpid = {$dados['tqpid']}
        AND tqrid = {$dados['tqrid']}
        AND rldid = {$dados['rldid']};
    ";
    return $sql;
}

function carregarQuestionarioTcu($rldid)
{
    global $db;

    $query = <<<DML
SELECT ptcq.tqtid,
       ptcq.qtdexercicio,
       ptcq.tqtdsc AS questionario,
       ptcp.tqpid,
       ptcp.tqppergunta AS pergunta,
       ptcr.tqrid,
       ptcr.tqrresposta AS resposta
  FROM planacomorc.tcuquestionario ptcq
    INNER JOIN planacomorc.tcuquestpergunta ptcp USING(tqtid)
    LEFT JOIN planacomorc.tcuquestresposta ptcr ON (ptcp.tqpid = ptcr.tqpid AND rldid = %d)
  WHERE ptcq.tqtestado = 'A'
    AND ptcp.tqpestado = 'A'
    AND qtdexercicio = '%s'
DML;
    $stmt = sprintf($query, $rldid, $_SESSION['exercicio']);
    return $db->carregar($stmt);
}

function formatarAcaTipo($acatipo)
{
    switch ($acatipo) {
        case 'N':
            return '<span class="label label-success">Prevista na LOA</span>';
            // no break
        case 'R':
            return '<span class="label label-primary">N�o prevista na LOA</span>';
            // no break
    }
}

function atribuirResponsavel($rldid, $usucpf)
{
    global $db;

    $msg = 'N�o foi poss�vel executar a associa��o do respons�vel.';

    // -- Limpando responsabilidades anteriores
    $sql = <<<DML
UPDATE planacomorc.usuarioresponsabilidade
  SET rpustatus = 'I'
  WHERE rldid = %d
DML;
    $stmt = sprintf($sql, $rldid);
    if (!$db->executar($stmt)) {
        throw new Exception($msg);
    }

    if (!empty($usucpf)) {
        // -- Atribu�ndo a nova responsabilidade
        $sql = <<<DML
INSERT INTO planacomorc.usuarioresponsabilidade(pflcod, usucpf, rldid)
  VALUES(%d, '%s', %d)
DML;
        $stmt = sprintf($sql, PFL_RELATORIO_TCU, $usucpf, $rldid);

        if (!$db->executar($stmt)) {
            throw new Exception($msg);
        }
    }
    $db->commit();
}

function consultarPermissoes($rlgid)
{
    global $db;
    $sql = <<<DML
SELECT COUNT(1) AS qtd,
       rld.rldtipo
  FROM planacomorc.relgdados rld
    INNER JOIN planacomorc.usuarioresponsabilidade rpu ON(rld.rldid = rpu.rldid AND rpu.rpustatus = 'A')
  WHERE rld.rlgid = %d
    AND rpu.usucpf = '%s'
  GROUP BY rld.rldtipo
DML;
    $stmt = sprintf($sql, $rlgid, $_SESSION['usucpf']);

    if (!$dados = $db->carregar($stmt)) {
        return array();
    }

    $retorno = array();
    foreach ($dados as $linha) {
        $retorno[] = $linha['rldtipo'];
    }
    return $retorno;
}

function filtroAcao($perfis, $acacod = null, $unicod = null)
{
// -- Queries da consulta de a��es - select
$whereAcao = '';
if (in_array(PFL_RELATORIO_TCU, $perfis)) {
    $whereAcao = <<<DML
AND EXISTS (SELECT 1
              FROM planacomorc.relgdados rld
                INNER JOIN planacomorc.relgestao rlg USING(rlgid)
                INNER JOIN planacomorc.usuarioresponsabilidade rpu USING(rldid)
              WHERE rlg.acacod = aca.acacod
                AND rpu.usucpf = '%s'
                AND rpu.pflcod = '%s'
                AND rpu.rpustatus = 'A')
DML;
    $whereAcao = sprintf($whereAcao, $_SESSION['usucpf'], PFL_RELATORIO_TCU);
}

$strSQL = <<<HTML
SELECT DISTINCT aca.acacod AS codigo,
                aca.unicod || '.' || aca.acacod ||' - '|| aca.acatitulo AS descricao
  FROM monitora.acao aca
  WHERE aca.prgano = '%s'
    AND aca.acastatus = 'A'
    %where-filtro-unicod%
    {$whereAcao}
  ORDER BY 2
HTML;

    if (empty($unicod)) {
        $strSQL = str_replace('%where-filtro-unicod%', "AND aca.unicod IN('26101', '26298')", $strSQL);
    } else {
        $strSQL = str_replace('%where-filtro-unicod%', "AND aca.unicod = '{$unicod}'", $strSQL);
    }
    $stmtAcao = sprintf($strSQL, $_SESSION['exercicio']);
    inputCombo('acacod', $stmtAcao, $acacod, 'acacod');
}

function formatarStatusIcone($esdid)
{
    switch ($esdid) {
        case ESDID_TCU_EM_PREENCHIMENTO:
            return '<span class="glyphicon glyphicon-minus" style="color:#f0ad4e"></span>';
        case ESDID_TCU_ANALISE_SPO:
        case ESDID_TCU_ACERTOS_UO:
            return '<span class="glyphicon glyphicon-transfer" style="color:#428bca"></span>';
        case ESDID_TCU_CONCLUIDO:
            return '<span class="glyphicon glyphicon-check" style="color:#5cb85c"></span>';
        default:
            return $esdid;
    }
}

function deletarRelatorioGestao($rlgid){
    global $db;
    
    $sql = <<<DML
        DELETE FROM planacomorc.tcuquestresposta 
            WHERE rldid IN (
                SELECT rldid FROM planacomorc.relgdados WHERE rlgid = {$rlgid}
            )
DML;
    $db->executar($sql);
    $sql = <<<DML
        DELETE FROM planacomorc.relgdados WHERE rlgid = {$rlgid}
DML;
    
    $db->executar($sql);
    $sql = <<<DML
        DELETE FROM planacomorc.relgestao WHERE rlgid = {$rlgid}
DML;
    $db->executar($sql);
    $db->commit();
}