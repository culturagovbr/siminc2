<?php
/**
 * Fun��es de apoio � altera��o or�ament�ria.
 * $Id: _funcoesalteracaoorcamentaria.php 102362 2015-09-11 19:01:00Z maykelbraz $
 */

/**
 * Fun��es do workflow
 * @see workflow.php
 */
include_once APPRAIZ . 'includes/workflow.php';

/**
 * Classe de abstra��o do webservice de altera��es or�ament�rias da SOF.
 * @see WSAlteracoesOrcamentarias
 */
require_once(APPRAIZ . 'altorc/classes/WSAlteracoesOrcamentarias.php');

/**
 * Carrega os dados de um pedido ou carrega as informa��es necess�rias para criar
 * um novo pedido, conforme a requisi��o feita pelo usu�rio.
 * Um pedido existente � carregado com base no $dados['paoid'], a coleta dos dados
 * de um novo pedido � feita com base em: $dados['mcrid'], $dados['tcrid'] e $dados['unicod'].
 *
 * @todo Considerar mcrid, tcrid, unicod e exercicio como chave prim�ria, se for solicitado
 * um novo registro com essa combina��o, deve-se abrir o anteriormente criado (na segunda etapa).
 *
 * @global cls_banco $db
 * @param array $dados
 * @return array|bool
 */
function dadosDoPedidoAlteracaoOrcamentaria($dados)
{
    global $db;

    if (chaveTemValor($dados, 'paoid')) {
        $query = <<<DML
SELECT mcr.mcrdsc,
       uni.unidsc,
       tcr.tcrcod,
       tcr.tcrdsc,
       uni.unicod,
       tcr.tcrid,
       mcr.mcrid,
       mcr.mcrcod,
       COALESCE(siopid::TEXT, '-') AS siopid,
       rqp.rqpsucesso AS paostatus,
       paoid,
       paodsc,
       pao.docid
  FROM altorc.pedidoalteracaoorcamentaria pao
    INNER JOIN altorc.momentocredito mcr ON(mcr.mcrid = pao.mcrid AND mcr.mcrano = pao.paoano)
    INNER JOIN altorc.tipocredito tcr ON(tcr.tcrid = pao.tcrid AND tcr.tcrano = pao.paoano)
    INNER JOIN public.unidade uni USING(unicod)
    LEFT JOIN altorc.requisicoespedido rqp USING(paoid)
  WHERE paoid = %d
    AND pao.paoano = '%s'
DML;
        $stmt = sprintf($query, $dados['paoid'], $_SESSION['exercicio']);
        $dadosdb = $db->pegaLinha($stmt);
    } else {
        // -- Se n�o foi enviado um PAO ID, tem que verificar o conjunto de chave para
        // -- carregar um pedido que j� esteja criado. Chave: exercicio, momento de cr�dito, tipo de cr�dito, uo.

        if (pedidoExistente($_SESSION['exercicio'], $dados['mcrid'], $dados['tcrid'], $dados['unicod'])) {
            // -- Carrega os dados do pedido chamando novamente "dadosDoPedidoAlteracaoOrcamentaria" com $dados['paoid']
            return true;
        } else {
            $query = <<<DML
SELECT 1, mcr.mcrdsc, mcrcod, '' AS unidsc, '' AS tcrcod, '' AS tcrdsc
  FROM altorc.momentocredito mcr WHERE mcrid = %d
UNION
SELECT 2, '' AS mcrdsc, '' AS mcrcod, uni.unicod || ' - ' || uni.unidsc AS unidsc, '' AS tcrcod, '' AS tcrdsc
  FROM public.unidade uni WHERE unicod = '%s'
UNION
SELECT 3, '' AS mcrdsc, '' AS mcrcod, '' AS unidsc, tcrcod, tcrdsc
  FROM altorc.tipocredito tcr WHERE tcrid = %d
  ORDER BY 1
DML;
            $stmt = sprintf($query, $dados['mcrid'], $dados['unicod'], $dados['tcrid']);
            $dadosdb = $db->carregar($stmt);
            $dadosdb = array(
                'mcrdsc' => $dadosdb[0]['mcrdsc'],
                'unidsc' => $dadosdb[1]['unidsc'],
                'tcrcod' => $dadosdb[2]['tcrcod'],
                'tcrdsc' => $dadosdb[2]['tcrdsc'],
                'unicod' => $dados['unicod'],
                'tcrid' => $dados['tcrid'],
                'mcrid' => $dados['mcrid'],
                'mcrcod' => $dadosdb[0]['mcrcod'],
                'siopid' => '-',
                'paostatus' => 'novo'
            );
        }
    }
    // -- Armazena os dados do pedido na sess�o.
    $_SESSION['altorc']['pedido']['dados'] = $dadosdb;

    return (bool)$dadosdb;
}

function pedidoExistente($exercicio, $mcrid, $tcrid, $unicod)
{
    global $db;
    $query = <<<DML
SELECT paoid
  FROM altorc.pedidoalteracaoorcamentaria pao
  WHERE pao.mcrid = %d
    AND pao.tcrid = %d
    AND pao.unicod = '%s'
    AND pao.paoano = '%d'
    AND pao.paostatus = 'A'
DML;
    $stmt = sprintf($query, $mcrid, $tcrid, $unicod, $exercicio);
    if (!($paoid = $db->pegaUm($stmt))) {
        return false;
    }
    return dadosDoPedidoAlteracaoOrcamentaria(array('paoid' => $paoid));
}

/**
 * Verifica se os dados do pedido est�o gravados na sess�o.
 * @return bool
 */
function dadosAlteracaoOrcamentariaNaSessao()
{
    return isset($_SESSION['altorc'])
           && isset($_SESSION['altorc']['pedido'])
           && isset($_SESSION['altorc']['pedido']['dados'])
           && !empty($_SESSION['altorc']['pedido']['dados']);
}

/**
 * Remove os dados do pedido da sess�o.
 */
function limpaDadosDoPedido()
{
    if (isset($_SESSION['altorc']) && isset($_SESSION['altorc']['pedido'])) {
        unset ($_SESSION['altorc']['pedido']);
    }
}

function carregarDadosLocalizador($snaid, $paoid)
{
    global $db;
    $sql = <<<DML
SELECT sna.prodsc,
       sna.unmdsc,
       sna.proquantidade,
       paf.valacrescimo,
       paf.valreducao,
       sna.acacod,
       sna.acadsc,
       sna.prgcod,
       sna.prgdsc,
       sna.loccod,
       COALESCE(sna.locdsc, '?????') AS locdsc,
       esfcod || '.' || unicod || '.' || funcod || '.' || sfucod || '.' || prgcod || '.' || acacod || '.' || loccod AS programatica
  FROM altorc.snapshotacao sna
    LEFT JOIN altorc.pedidoalteracaofisico paf ON sna.snaid = paf.snaid AND paf.paoid = %d
  WHERE sna.snaid = %d
DML;
    $stmt = sprintf($sql, $paoid, $snaid);
    if ($prodata = $db->pegaLinha($stmt)) {
        return $prodata;
    }
    return array();
}

/**
 * Salva os valores f�sicos do localizador, e cria um novo pedido, se necess�rio.
 *
 * @global cls_banco $db
 * @param array $dadospao Dados do pedido de altera��o or�ament�ria.
 * @param int $valAcrescimo Valor do acr�scimento pedido para o f�sico.
 * @param int $valReducao Valor da redu��o pedida para o f�sico.
 * @return boolean
 */
function salvarFisicoDoLocalizador(array &$dadospao, $valAcrescimo, $valReducao)
{
    global $db;

    // -- Limpando a formata��o dos n�meros
    $valAcrescimo = str_replace('.', '', $valAcrescimo);
    $valReducao = str_replace('.', '', $valReducao);

    if (is_null($dadospao['paoid'])) {
        // -- Cria um novo pedido
        list($dadospao['paoid'], $dadospao['paodsc'], $dadospao['docid']) = criaPedidoAlteracaoOrcamentaria($dadospao);
    }

    $sql = <<<DML
UPDATE altorc.pedidoalteracaofisico
  SET valacrescimo = %d,
      valreducao = %d
  WHERE paoid = %d
    AND snaid = %d
  RETURNING pafid
DML;
    $stmt = sprintf($sql, $valAcrescimo, $valReducao, $dadospao['paoid'], $dadospao['snaid']);

    if (!($pafid = $db->pegaUm($stmt))) {
        $sql = <<<DML
INSERT INTO altorc.pedidoalteracaofisico(valacrescimo, valreducao, paoid, snaid)
  VALUES(%d, %d, %d, %d)
DML;
        $stmt = sprintf($sql, $valAcrescimo, $valReducao, $dadospao['paoid'], $dadospao['snaid']);
        $db->executar($stmt);
    }

    if (!$db->commit()) {
        unset($dadospao['paoid'], $dadospao['paodsc'], $dadospao['docid']);
        $db->rollback();
        return false;
    }
    return true;
}

/**
 *
 * @global cls_banco $db
 * @param array $dadosPO
 * @param array $dadospao
 * @return boolean
 */
function salvarFinanceiroDoPO(array $dadosPO, array &$dadospao)
{
    global $db;

    // -- Indica se um novo pedido de ALTERA��O ORCAMENT�RIA foi criado.
    $novoPedido = false;

    if (!isset($dadospao['paoid'])) {
        // -- Cria um novo pedido
        list($dadospao['paoid'], $dadospao['paodsc'], $dadospao['docid']) = criaPedidoAlteracaoOrcamentaria($dadospao);
        $novoPedido = true;
    }

    // -- Removendo a mascara do valor de cancelamento
    $valcancelamento = str_replace(array('.', ','), array('', '.'), $dadosPO['valcancelamento']);
    // -- Valor de suplementa��o e tipo de fonte de recurso
    list($tfrcod, $valsuplementacao) = getTipoFonteRecursoEValSuplementacao(
        $dadosPO['valsuplementacao'],
        $dadosPO['valsuperavit'],
        $dadosPO['valexcesso'],
        $dadosPO['valopcredito'],
        $dadosPO['tfrcod']
    );

    if (!empty($dadosPO['spoid'])) { // -- Atualizando o PO
        $snaid = upsertFinanceiroPO(
            $valsuplementacao,
            $valcancelamento,
            $tfrcod,
            $dadosPO['spoid'],
            $dadospao['paoid'],
            'M'
        );
        $spoid = $dadosPO['spoid'];
    } else {
        // -- Verificando se o PO j� n�o existe
        if (verificaSePOExiste($dadosPO)) {
            // -- $dadospao atribu�dos quando foi criada uma transa��o
            if ($novoPedido) {
                unset($dadospao['paoid'], $dadospao['paodsc'], $dadospao['docid']);
            }
            $db->rollback();
            return false;
        }
        // -- dadosPO['snaid'] � setado quando se esta criando um novo PO
        $snaid = $dadosPO['snaid'];
        // -- Criando uma nova entrada do PO
        $spoid = criarNovoPO($dadosPO);

        // -- Criando a nova entrada de cr�dito
        upsertFinanceiroPO(
            $valsuplementacao,
            $valcancelamento,
            $tfrcod,
            $spoid,
            $dadospao['paoid'],
            'N'
        );
    }

    // -- Verifica e cria, se necess�rio, uma entrada de a��o assoaciada ao PO, no pedido atual.
    criaFisicoVazio($snaid, $dadospao['paoid']);

    if (!$db->commit()) {
        if ($novoPedido) {
            unset($dadospao['paoid'], $dadospao['paodsc'], $dadospao['docid']);
        }
        $db->rollback();
        return false;
    }
    return true;
}

/**
 * Cria uma nova entrada em snapshotplanoorcamentario com os dados do PO criado.
 *
 * @global cls_banco $db
 * @param array $dadosPO Dados do novo PO.
 * @return int|bool O novo spoid ou falha na execu��o.
 */
function criarNovoPO($dadosPO)
{
    global $db;

    $query = <<<DML
INSERT INTO altorc.snapshotplanoorcamentario(
    foncod,
    idoccod,
    idusocod,
    natcod,
    rpleicod,
    rpcod,
    plocod,
    snaid,
    dotatual)
  VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s', %d, 0.00)
  RETURNING spoid
DML;

    $stmt = sprintf(
        $query,
        $dadosPO['foncod'],
        $dadosPO['idoccod'],
        $dadosPO['idusocod'],
        $dadosPO['natcod'],
        $dadosPO['rpleicod'],
        $dadosPO['rpcod'],
        $dadosPO['plocod'],
        $dadosPO['snaid']
    );
    return $db->pegaUm($stmt);
}

/**
 * Com base nos valores submetidos no formul�rio de altera��o de po, identifica o tipo de
 * suplementa��o e retorna o valor formatado e o c�digo do tipo de fonte de recurso.
 *
 * @param string $valsuplementacao O valor submetido no formul�rio para suplementa��o por cancelamento.
 * @param string $valsuperavit O valor submetido no formul�rio para suplementa��o por superavit.
 * @param string $valexcesso O valor submetido no formul�rio para suplementa��o por excesso.
 * @return array Com o codigo do tipo de fonte e valor suplementado sem a m�scara de reais.
 */
function getTipoFonteRecursoEValSuplementacao($valsuplementacao, $valsuperavit, $valexcesso, $valopcredito)
{
    // -- Considera suplementa��o por cancelamento como o tipo de fonte de recurso padr�o
    if ($valsuperavit != '0') {
        $tfrcod = 3;
        $valsuplementacao = str_replace(array('.', ','), array('', '.'), $valsuperavit);
    } elseif ($valexcesso != '0') {
        $tfrcod = 2;
        $valsuplementacao = str_replace(array('.', ','), array('', '.'), $valexcesso);
    } elseif ($valopcredito != '0') {
        $tfrcod = 4;
        $valsuplementacao = str_replace(array('.', ','), array('', '.'), $valopcredito);
    } else {
        $tfrcod = 1;
        $valsuplementacao = str_replace(array('.', ','), array('', '.'), $valsuplementacao);
    }
    return array($tfrcod, $valsuplementacao);
}

/**
 *
 * @global cls_banco $db
 * @param float $valsuplementacao Valor de suplementa��o indicado pelo usu�rio.
 * @param float $valcancelamento Valor de cancelamento indicado pelo usu�rio.
 * @param int $tfrcod Tipo de fonte de recurso do dinheiro suplementado.
 * @param int $spoid Id do snapshot do planoorcamentario.
 * @param int $paoid Id do pedido de altera��o or�ament�ria.
 * @param string $pastip Tipo de associa��o que est� sendo inserida.
 * @return int Id do snapshot da a��o.
 */
function upsertFinanceiroPO($valsuplementacao, $valcancelamento, $tfrcod, $spoid, $paoid, $pastip)
{
    global $db;

    // -- Tenta fazer o update com base no spoid e paoid
    $query = <<<DML
UPDATE altorc.pedidoalteracaofinanceiro
  SET valsuplementacao = %f,
      valcancelamento = %f,
      tfrcod = %d
  WHERE spoid = %d
    AND paoid = %d
  RETURNING pasid
DML;
    $stmt = sprintf($query, $valsuplementacao, $valcancelamento, $tfrcod, $spoid, $paoid);

    // -- Se n�o conseguiu fazer o update, o registro de financeiro n�o existe, ent�o faz a inser��o de um novo
    if (!($pasid = $db->pegaUm($stmt))) {
        $query = <<<DML
INSERT INTO altorc.pedidoalteracaofinanceiro(valsuplementacao, valcancelamento, tfrcod, spoid, paoid, pastip)
  VALUES(%f, %f, %d, %d, %d, '%s')
DML;
        $stmt = sprintf($query, $valsuplementacao, $valcancelamento, $tfrcod, $spoid, $paoid, $pastip);
        $db->executar($stmt);
    }

    // -- Consultando o SNAID
    $query = <<<DML
SELECT snaid
  FROM altorc.snapshotplanoorcamentario
  WHERE spoid = %d
DML;

    $stmt = sprintf($query, $spoid);
    $snaid = $db->pegaUm($stmt);

    return $snaid;
}

/**
 * Verifica se uma configura��o de PO, j� associada a uma a��o, existe na base de dados.
 *
 * @global cls_banco $db
 * @param array $dadosPO Dados do PO.
 * @return bool
 */
function verificaSePOExiste($dadosPO)
{
    global $db;

    $query = <<<DML
SELECT COUNT(1)
  FROM altorc.snapshotplanoorcamentario spo
  WHERE spo.foncod = '%s'
    AND spo.idoccod = '%s'
    AND spo.idusocod = '%s'
    AND spo.natcod = '%s'
    AND spo.rpleicod = '%s'
    AND spo.rpcod = '%s'
    AND spo.plocod = '%s'
    AND spo.snaid = %d
DML;
    $stmt = sprintf(
        $query,
        $dadosPO['foncod'],
        $dadosPO['idoccod'],
        $dadosPO['idusocod'],
        $dadosPO['natcod'],
        $dadosPO['rpleicod'],
        $dadosPO['rpcod'],
        $dadosPO['plocod'],
        $dadosPO['snaid']
    );

    $qtdspo = $db->pegaUm($stmt);
    return ($qtdspo >= 1);
}

/**
 * Verifica se existe um f�sico declarado para uma a��o em um determinado pedido.
 * Se n�o existir, criar uma entrada para aquela a��o, com o f�sico zerado.
 * @global cls_banco $db
 * @param int $snaid Identificador do snapshot da a��o.
 * @param int $paoid Identificador do pedido.
 * @return boolean
 */
function criaFisicoVazio($snaid, $paoid)
{
    global $db;
    $sql = <<<DML
SELECT COUNT(1)
  FROM altorc.pedidoalteracaofisico
  WHERE paoid = %d
    AND snaid = %d
DML;
    $stmt = sprintf($sql, $paoid, $snaid);
    if ('1' != $db->pegaUm($stmt)) {
        // -- Se n�o houver uma entrada de f�sico da a��o
        // -- para o pedido indicado, um novo, e vazio, dever� ser criado.
        $sql = <<<DML
INSERT INTO altorc.pedidoalteracaofisico(valacrescimo, valreducao, paoid, snaid)
  VALUES(0, 0, %d, %d)
DML;
        $stmt = sprintf($sql, $paoid, $snaid);
        return (bool)$db->executar($stmt);
    }

    return true;
}

/**
 * Cria um novo pedido de altera��o orcament�ria.
 *
 * @global cls_banco $db
 * @param array $dadospao Dados do pedido de altera��o or�ament�ria.
 * @return boolean
 */
function criaPedidoAlteracaoOrcamentaria(array $dadospao)
{
    global $db;

    $sql = <<<DML
INSERT INTO altorc.pedidoalteracaoorcamentaria(mcrid, tcrid, unicod, usucpf, paoano, paodsc)
  VALUES(%d, %d, '%s', '%s', '%d', '%s')
  RETURNING paoid, paodsc
DML;
    $paodsc = "{$dadospao['unicod']}/{$dadospao['tcrcod']}/{$_SESSION['exercicio']}.1";
    $stmt = sprintf($sql, $dadospao['mcrid'], $dadospao['tcrid'], $dadospao['unicod'], $_SESSION['usucpf'], $_SESSION['exercicio'], $paodsc);
    if ($dadosdb = $db->pegaLinha($stmt)) {
        $dadosdb['docid'] = criarDocumento($dadosdb['paoid']);
        return array($dadosdb['paoid'], $dadosdb['paodsc'], $dadosdb['docid']);
    }
    return array(false, false);
}

function preencheObjeto(&$objeto, $dados)
{
    // -- Pegando as propriedades da classe para verifica��o
    $propriedades = array_keys(get_class_vars(get_class($objeto)));
    foreach ($dados as $campo => $valor) {
        if (in_array($campo, $propriedades)) {
            $objeto->$campo = $valor;
        }
    }
}

function pedidoAlteracaoOrcamentariaAsObject($paoid)
{
    global $db;
    $query = <<<DML
SELECT pao.siopid AS "identificadorUnico",
       pao.paoano AS exercicio,
       mcr.mcrcod AS "codigoMomento",
       '' AS "codigoClassificacaoAlteracao",
       tcr.tcrcod AS "codigoTipoAlteracao",
       pao.paodsc AS descricao,
       '26000' AS "codigoOrgao",
       pao.paoid,
       mcr.mcrid
  FROM altorc.pedidoalteracaoorcamentaria pao
    INNER JOIN altorc.momentocredito mcr USING(mcrid)
    INNER JOIN altorc.tipocredito tcr USING(tcrid)
  WHERE pao.paoid = %d
DML;
    $stmt = sprintf($query, $paoid);
    $dadospedido = $db->pegaLinha($stmt);
    $pedido = new PedidoAlteracaoDTO();
    preencheObjeto($pedido, $dadospedido);
    $pedido->fisicosPedidoAlteracao = array();
    fisicosPedidoAlteracaoAsList($pedido->fisicosPedidoAlteracao, $dadospedido['paoid']);
    $pedido->respostasJustificativa = array();
    justiticativasPedidoAlteracaoAsList($pedido->respostasJustificativa, $dadospedido['paoid'], $dadospedido['mcrid']);

    return $pedido;
}

function justiticativasPedidoAlteracaoAsList(&$justificativas, $paoid, $mcrid)
{
    global $db;
    $query = <<<DML
SELECT prp.prptexto AS resposta,
       pgm.pgmcod AS "codigoPergunta"
  FROM altorc.perguntasrespostaspedido prp
    INNER JOIN altorc.perguntasmomento pgm USING(pgmid)
  WHERE prp.paoid = %d
--    AND pgm.mcrid = %d
DML;
    $stmt = sprintf($query, $paoid, $mcrid);
    if ($dadosjust = $db->carregar($stmt)) {
        foreach ($dadosjust as $dados) {
            $resposta = new RespostaJustificativaDTO();
            preencheObjeto($resposta, $dados);
            $justificativas[] = $resposta;
        }
    }
}

function fisicosPedidoAlteracaoAsList(array &$fisicos, $paoid)
{
    global $db;
    $query = <<<DML
SELECT snaexercicio AS exercicio,
       esfcod AS "codigoEsfera",
       unicod AS "codigoUO",
       funcod AS "codigoFuncao",
       sfucod AS "codigoSubFuncao",
       prgcod AS "codigoPrograma",
       acacod AS "codigoAcao",
       loccod AS "codigoLocalizador",
       tilcod AS "codigoTipoInclusaoLocalizador",
       tiacod AS "codigoTipoInclusaoAcao",
       paf.valacrescimo AS "quantidadeAcrescimo",
       paf.valreducao AS "quantidadeReducao",
       snaid
  FROM altorc.snapshotacao sna
    INNER JOIN altorc.pedidoalteracaofisico paf USING(snaid)
  WHERE paf.paoid = %d
DML;
    $stmt = sprintf($query, $paoid);

    if ($dadosfis = $db->carregar($stmt)) {
        foreach ($dadosfis as $dados) {
            $fisico = new FisicoPedidoAlteracaoDTO();
            preencheObjeto($fisico, $dados);
            $fisico->listaFinanceiroPedidoAlteracaoDTO = array();
            financeirosPedidoAlteracaoAsList($fisico->listaFinanceiroPedidoAlteracaoDTO, $dados['snaid'], $paoid);
            $fisicos[] = $fisico;
        }
    }
}

function financeirosPedidoAlteracaoAsList(array &$financeiros, $snaid, $paoid)
{
    global $db;
    $query = <<<DML
SELECT foncod AS "codigoFonte",
       idoccod AS "codigoIdOC",
       idusocod AS "codigoIdUso",
       natcod AS "codigoNatureza",
       rpcod AS "codigoRP",
       rpleicod AS "codigoRPLei",
       plocod AS "planoOrcamentario",
       pas.tfrcod AS "codigoTipoFonteRecurso",
       pas.valsuplementacao AS "valorSuplementacao",
       pas.valcancelamento AS "valorCancelamento"
  FROM altorc.snapshotplanoorcamentario spo
    INNER JOIN altorc.pedidoalteracaofinanceiro pas USING(spoid)
  WHERE spo.snaid = %d
    AND pas.paoid = %d
DML;
    $stmt = sprintf($query, $snaid, $paoid);
    if ($dadosfin = $db->carregar($stmt)) {
        foreach ($dadosfin as $dados) {
            $financeiro = new FinanceiroPedidoAlteracaoDTO();
            if ('0.00' == $dados['valorSuplementacao']) {
                $dados['valorSuplementacao'] = 0;
            }
            if ('0.00' == $dados['valorCancelamento']) {
                $dados['valorCancelamento'] = 0;
            }
            preencheObjeto($financeiro, $dados);
            $financeiros[] = $financeiro;
        }
    }
}

function criarNovaRequisicao($paoid, $sucesso)
{
    global $db;

    $sucesso = $sucesso?'TRUE':'FALSE';
    $query = <<<DML
INSERT INTO altorc.requisicoespedido(paoid, rqpsucesso)
  VALUES(%d, %s)
  RETURNING rqpid
DML;
    $stmt = sprintf($query, $paoid, $sucesso);
    $rpqid = $db->pegaUm($stmt);

    $query = <<<DML
UPDATE altorc.pedidoalteracaoorcamentaria
  SET rqpid = %d
  WHERE paoid = %d
DML;
    $stmt = sprintf($query, $rpqid, $paoid);
    $db->executar($stmt);

    return $rpqid;
}

function armazenaMensagensErroRequisicao($rqpid, $msgs)
{
    global $db;
    if (!is_array($msgs)) {
        $msgs = array($msgs);
    }

    $query = <<<DML
INSERT INTO altorc.requisicoeserros(rqemensagem, rqpid)
  VALUES('%s', %d)
DML;
    foreach ($msgs as $msg) {
        $msg = str_replace("'", "''", $msg);
        $stmt = sprintf($query, $msg, $rqpid);
        $db->executar($stmt);
    }
}

function salvarJustificativas(array $justificativas, $paoid, $usucpf, $usunome)
{
    global $db;

    $queryUPD = <<<DML
UPDATE altorc.perguntasrespostaspedido
  SET prptexto = '%s'
  WHERE paoid = %d
    AND pgmid = %d
  RETURNING prpid
DML;
    $queryINS = <<<DML
INSERT INTO altorc.perguntasrespostaspedido(paoid, pgmid, prptexto)
  VALUES(%d, %d, '%s')
DML;
    // -- Adicionando dados na �ltima pergunta do question�rio
    end($justificativas);
    $ultimaChave = key($justificativas);
    $padraoUsuario = "/\|$/";
    if (!preg_match($padraoUsuario, $justificativas[$ultimaChave])) {
        $justificativas[$ultimaChave] .= "\n|{$usucpf} - {$usunome}|";
    }

    foreach ($justificativas as $pgmid => $prptexto) {
        $prptexto = str_replace(array("\\'", '\\"', "'"), array("'", '"', "''"), $prptexto);
        $stmt = sprintf($queryUPD, $prptexto, $paoid, $pgmid);
        if (!$db->pegaUm($stmt)) {
            $stmt = sprintf($queryINS, $paoid, $pgmid, $prptexto);
            $db->executar($stmt);
        }
    }

    if (!$db->commit()) {
        $db->rollback();
        return false;
    }
    return true;
}

/**
 * Caso o documento n�o estaja criado cria um novo
 *
 * @param string $capid
 * @return integer
 */
function criarDocumento($paoid) {
    global $db;

    $docid = pegarDocid($paoid);

    if (!$docid) {
        // recupera o tipo do documento
        $tpdid = TPDOC_PEDIDO_ALTERACAO_ORCAMENTARIA;
        // descri��o do documento
        $docdsc = "Pedido de altera��o or�ament�ria N� " . $paoid;
        // cria documento do WORKFLOW
        $docid = wf_cadastrarDocumento($tpdid, $docdsc);
        // atualiza o plano de trabalho
        $sql = "UPDATE altorc.pedidoalteracaoorcamentaria SET docid = " . $docid . " WHERE paoid = " . $paoid;
        $db->executar($sql);
    }

    return $docid;
}

/**
 * Pega o id do documento do plano de trabalho
 *
 * @param integer $paoid
 * @return integer
 */
function pegarDocid($paoid) {
    global $db;
    $sql = "SELECT docid FROM altorc.pedidoalteracaoorcamentaria WHERE paoid = " . $paoid;
    return $db->pegaUm($sql);
}

/**
 * Pega o estado atual do workflow
 *
 * @param integer $paoid
 * @return integer
 */
function pegarEstadoAtual($paoid) {
    global $db;
    $docid = pegarDocid($paoid);
    if ($docid) {
        $sql = "SELECT ed.esdid
                FROM workflow.documento d
                    JOIN workflow.estadodocumento ed on ed.esdid = d.esdid
                WHERE d.docid = " . $docid;
        $estado = (integer) $db->pegaUm($sql);
        return $estado;
    }
    return false;
}

function atualizarSIOPIDPedido($paoid, $siopid)
{
    global $db;

    $sql = <<<DML
UPDATE altorc.pedidoalteracaoorcamentaria
  SET siopid = %d
  WHERE paoid = %d
DML;
    $stmt = sprintf($sql, $siopid, $paoid);
    return (bool)$db->executar($stmt);
}

/**
 *
 * @global c
 * @global cls_banco $db
 * @param int $rqpidls_banco $db
 * @param int $rqpidAtualiza o status da requisi��o para sucesso.
 * @param int $rqpid Id da requisi��o.
 */
function atualizaRequisicaoComoSucesso($rqpid)
{
    global $db;

    $sql = <<<DML
UPDATE altorc.requisicoespedido
  SET rqpsucesso = TRUE
  WHERE rqpid = %d
DML;
    $stmt = sprintf($sql, $rqpid);
    $db->executar($stmt);
}

function processaVerificacoes($rqpid, $verificacoes)
{
    global $db;
    $quantidadeErros = 0;

    $sql = <<<DML
INSERT INTO altorc.requisicoeserros(rqemensagem, rqpid, rqetipo)
  VALUES('%s', %d, '%s')
DML;
    foreach ($verificacoes->verificacao AS $verificacao) {
        $txtRegra = str_replace("'", "''", $verificacao->regra);

        if ('Janela de trabalho do pedido est� aberta?' == $txtRegra) {
            continue;
        }

        $msg = <<<HTML
<p style="text-align:left">{$txtRegra}</p>
HTML;
        if ($verificacao->passou) {
            if ($verificacao->snInformativa) {
                $rqetipo = 'A';
            } else {
                $rqetipo = 'S';
            }
        } else {
            $rqetipo = 'E';
        }

        if (!empty($verificacao->detalhes)) {
            if (is_array($verificacao->detalhes->detalhe)) {
                foreach ($verificacao->detalhes->detalhe as $detalhe) {
                    $txt = str_replace("'", "''", $detalhe);
                    $msg .= <<<HTML
<blockquote style="text-align:left">{$txt}</blockquote>
HTML;
                }
            } else {
                    $txt = str_replace("'", "''", $verificacao->detalhes->detalhe);
                    $msg .= <<<HTML
<blockquote style="text-align:left">{$txt}</blockquote>
HTML;
            }
        }

        $stmt = sprintf($sql, $msg, $rqpid, $rqetipo);
        $db->executar($stmt);

        if ('E' == $rqetipo) {
            $quantidadeErros++;
        }
    }

    return $quantidadeErros;
}

function suplementacoes($paoid)
{
    global $db;

    $_dados = array(
        'supcancelamento' => 0,
        'cancelamento' => 0,
        'supexcesso' => 0,
        'supsuperavit' => 0,
        'supopcredito' => 0,
        'diferenca' => 0,
        'suptotal' => 0
    );

    if (empty($paoid)) {
        return $_dados;
    }

    $query = <<<DML
SELECT SUM(COALESCE(valsuplementacao, 0.00)) AS valsuplementacao,
       SUM(COALESCE(valcancelamento, 0.00)) AS valcancelamento,
       tfrcod
  FROM altorc.pedidoalteracaofinanceiro paf
  WHERE paf.paoid = %d
  GROUP BY tfrcod
DML;
    $stmt = sprintf($query, $paoid);
    if ($dadosfinanceiro = $db->carregar($stmt)) {
        foreach ($dadosfinanceiro as $dado) {
            $_dados['suptotal'] += (float)$dado['valsuplementacao'];
            $_dados['cancelamento'] += (float)$dado['valcancelamento'];
            switch ($dado['tfrcod']) {
                case '2':
                    $_dados['supexcesso'] += (float)$dado['valsuplementacao'];
                    break;
                case '3':
                    $_dados['supsuperavit'] += (float)$dado['valsuplementacao'];
                    break;
                case '4':
                    $_dados['supopcredito'] += (float)$dado['valsuplementacao'];
                    break;
                default: //case '1':
                    $_dados['supcancelamento'] += (float)$dado['valsuplementacao'];
            }
        }
        $_dados['diferenca'] = $_dados['cancelamento'] - $_dados['suptotal'];
        foreach ($_dados as &$dado) {
            $dado = number_format($dado, 2, ',', '.');
        }
    }
    return $_dados;
}

function enviarPedido($paoid, $exercicio)
{
    global $db;

    /**
     * Helper de exibi��o de alertas entre requisi��es.
     * (Necess�rio por causa da tramita��o do workflow)
     * @see Simec_Helper_FlashMessage
     */
    require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";

    $ws = new WSAlteracoesOrcamentarias();

    // -- Preparando o pedido para envio
    $pedido = pedidoAlteracaoOrcamentariaAsObject($paoid);
    $retorno = $ws->cadastrarPedidoAlteracao($pedido);
    $retorno = $retorno->return;

    // -- (log de) Transa��o iniciada
    $rqpid = criarNovaRequisicao($paoid, $retorno->return->sucesso);

    $fm = new Simec_Helper_FlashMessage('altorc/pedido');

    // -- Link para aba de resumo, utilizada durante a exibi��o de erros
    $link = '<a href="altorc.php?modulo=principal/pedido/inicio&acao=A&dados[paoid]=' . $paoid
        . '&target=resumo">Resumo/Tr�mite</a>';
    $click = <<<HTML
 Ou clique no �cone <span class="glyphicon glyphicon-thumbs-down" style="color:black"></span> do pedido.
HTML;

    // -- Verificando se houve sucesso na requisicao
    if ($retorno->sucesso) {
        $siopid = $retorno->registros->identificadorUnico;
        $fm->addMensagem('Seu pedido foi cadastrado no SIOP com sucesso.');
        if (atualizarSIOPIDPedido($paoid, $siopid)) {
            $retorno = $ws->verificarPedidoAlteracao($siopid, $exercicio);
            $retorno = $retorno->return;

            /* Armazendando os Avisos, mesmo quando retornar sucesso */
            armazenaMensagensErroRequisicao($rqpid, $retorno->mensagensErro);

            // -- Metodo n�o definido
            if ('SoapFault' == get_class($retorno)) {
                $msg = 'N�o foi poss�vel verificar seu pedido no SIOP (chamada inexistente).';
                $fm->addMensagem($msg, Simec_Helper_FlashMessage::ERRO);
                armazenaMensagensErroRequisicao($rqpid, $msg);
            // -- Mensagem de erro generica
            } elseif (!$retorno->sucesso) {
                if (empty($retorno->verificacoes)) {
                    $msg = "N�o foi poss�vel verificar seu pedido no SIOP. Verifique a aba '{$link}' para maiores detalhes.{$click}";
                    armazenaMensagensErroRequisicao($rqpid, $retorno->mensagensErro);
                    $fm->addMensagem($msg, Simec_Helper_FlashMessage::ERRO);
                } else {
                    // -- H� uma das mensagens de erro que s�o ignoradas, logo, precisamos verificar se todas as outras
                    // -- mensagens foram de sucesso, se todas forem de sucesso, marcamos a requisi��o como sucesso.
                    $qtdErros = processaVerificacoes($rqpid, $retorno->verificacoes);
                    if ($qtdErros > 0) { // -- Apresentou erros
                        $fm->addMensagem(
                            "Seu pedido foi verificado no SIOP e apresenta pend�ncias. Verifique a aba '{$link}' para maiores detalhes.{$click}",
                            Simec_Helper_FlashMessage::ERRO
                        );
                    } else { // -- Todas as verifica��es foram de sucesso
                         /* Armazendando os Avisos, mesmo quando retornar sucesso */
                        $retornoSiop = $ws->verificarPedidoAlteracao($siopid, $exercicio);
                        $retornoSiop = $retornoSiop->return;
                        armazenaMensagensErroRequisicao($rqpid, $retornoSiop->mensagensErro);

                        $fm->addMensagem('Seu pedido foi verificado no SIOP com sucesso e n�o apresenta pend�ncias.');
                        atualizaRequisicaoComoSucesso($rqpid);
                    }
                }
            } elseif ($retorno->sucesso) { // -- Conseguiu verificar com sucesso o pedido
                $fm->addMensagem('Seu pedido foi verificado no SIOP com sucesso e n�o apresenta pend�ncias.');
                atualizaRequisicaoComoSucesso($rqpid);
            }
        }
    } else {
        armazenaMensagensErroRequisicao($rqpid, $retorno->mensagensErro);
        $msg = <<<DML
N�o foi poss�vel enviar o pedido. Verifique as mensagens de comunica��o na aba '{$link}'.{$click}
DML;
        $fm->addMensagem($msg, Simec_Helper_FlashMessage::ERRO);
    }

    if (!$db->commit()) {
        $db->rollback();
        return false;
    }

    return true;
}

function podeEnviarPedido($permissao)
{
    return $permissao;
}

function excluirCredito($dados)
{
    global $db;
    $sql = <<<DML
DELETE FROM altorc.pedidoalteracaofinanceiro
  WHERE pasid = %d
DML;
    $stmt = sprintf($sql, $dados['pasid']);
    $db->executar($stmt);

    $sql = <<<DML
DELETE FROM altorc.snapshotplanoorcamentario
  WHERE spoid = %d
    AND NOT EXISTS (SELECT 1
                      FROM altorc.pedidoalteracaofinanceiro pas
                      WHERE pas.spoid = snapshotplanoorcamentario.spoid)
DML;
    $stmt = sprintf($sql, $dados['spoid']);
    $db->executar($stmt);

    if (!$db->commit()) {
        $db->rollback();
        return false;
    }
    return true;
}

function limparCredito($dados)
{
    global $db;
    $sql = <<<DML
DELETE FROM altorc.pedidoalteracaofinanceiro
  WHERE pasid = %d
    AND spoid = %d
DML;

    $stmt = sprintf($sql, $dados['pasid'], $dados['spoid']);
    $db->executar($stmt);
    if (!$db->commit()) {
        $db->rollback();
        return false;
    }
    return true;
}

function limparFuncional($dados)
{
    global $db;

    // -- Apagando primeiro os POs associados aquela funcional
    $sql = <<<DML
DELETE
  FROM altorc.pedidoalteracaofinanceiro
  WHERE paoid = %d
    AND EXISTS (SELECT 1
                  FROM altorc.snapshotplanoorcamentario spo
                  WHERE spo.spoid = pedidoalteracaofinanceiro.spoid
                    AND spo.snaid = %d)
DML;
    $stmt = sprintf($sql, $dados['paoid'], $dados['snaid']);
    if (!$db->executar($stmt)) {
        $db->rollback();
        return false;
    }

    $sql = <<<DML
DELETE
  FROM altorc.pedidoalteracaofisico
  WHERE paoid = %d
    AND snaid = %d
DML;
    $stmt = sprintf($sql, $dados['paoid'], $dados['snaid']);
    $db->executar($stmt);

    if (!$db->commit()) {
        $db->rollback();
        return false;
    }

    return true;
}

function comunicarCriadorPedidoDeRetorno($paoid)
{
	global $db;
    $sql = <<<DML
SELECT pao.paodsc,
       usu.usunome,
       usu.usucpf,
       com.cmddsc,
       usu.usuemail
  FROM altorc.pedidoalteracaoorcamentaria pao
    INNER JOIN seguranca.usuario usu USING(usucpf)
    INNER JOIN workflow.documento doc USING(docid)
    INNER JOIN workflow.comentariodocumento com USING(docid)
  WHERE pao.paoid = %d
DML;
    $stmt = sprintf($sql, $paoid);

    $dadospedido = $db->pegaLinha($stmt);
    if (!$dadospedido) {
        return true;
    }

    $msg = <<<HTML
<p>O pedido de altera��o or�ament�ria "{$dadospedido['paodsc']}" foi retornado para corre��o com a seguinte observa��o:</p>
<blockquote>
{$dadospedido['cmddsc']}
</blockquote>
<p>Por favor, acesse o m�dulo "SPO - Altera��es Or�ament�rias", no <a href="http://simec.mec.gov.br">SIMEC</a>, e fa�a as corre��es necess�rias.</p>
HTML;

    enviar_email(
        array(
            'nome' => 'Or�amento - Altera��es Or�ament�rias',
            'email' => 'spo.orcamento@mec.gov.br'
        ),
        array($dadospedido['usuemail']),
        'Corre��o pendente',
        $msg
    );

	return true;
}

function podeCriarPedido()
{
    global $db;

    if (1 == $_SESSION['superuser']) {
        return true;
    }

    $perfis = pegaPerfilGeral();
    if (in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis)) {
        return true;
    }

    $sql = <<<DML
SELECT MIN(mcrid) AS mcrid
  FROM altorc.momentocredito mcr
  WHERE EXISTS (SELECT 1 -- Encontrando o momento de credito atual, com base na data de referencia
                  FROM altorc.momentocredito mcr2
                  WHERE mcr.mcrid = mcr2.mcrid
                    AND now()::DATE BETWEEN mcr2.mcrrefinicio AND mcr2.mcrreffim)
    -- Verificando se o momento de credito atual ainda esta com a data de inclusao aberta
    AND NOW()::DATE BETWEEN mcr.mcrincinicio AND mcr.mcrincfim
    AND mcr.mcrstatus = 'A'
DML;

    $mcrid = $db->pegaUm($sql);
    if ($mcrid) {
        return true;
    }

    return false;
}

function podeAlterarPedido($paoid)
{
    global $db;

    if (1 == $_SESSION['superuser']) {
        return true;
    }

    $perfis = pegaPerfilGeral();
    if (in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis)) {
        return true;
    }

    // -- Um novo pedido est� sendo criado, se ainda est� em fase de cria��o, pode alterar
    if (empty($paoid)) {
        return true;
    }

    $sql = <<<DML
SELECT MIN(mcrid) AS mcrid
  FROM altorc.momentocredito mcr
  WHERE EXISTS (SELECT 1 -- Encontrando o momento de credito atual, com base na data de referencia
                  FROM altorc.momentocredito mcr2
                  WHERE mcr.mcrid = mcr2.mcrid
                    AND now()::DATE BETWEEN mcr2.mcrrefinicio::DATE AND mcr2.mcrreffim::DATE)
    -- Verificando se o momento de credito atual ainda esta com a data de alteracao aberta
    AND NOW()::DATE BETWEEN mcr.mcraltinicio::DATE AND mcr.mcraltfim::DATE
    AND mcr.mcrstatus = 'A'
DML;

    $mcrid = $db->pegaUm($sql);
    if ($mcrid) {
        $esdid = pegarEstadoAtual($paoid);
        if ($esdid == STDOC_ANALISE_SPO || $esdid == STDOC_CADASTRAR_SIOP) {
            return false;
        }
        return true;
    }
    return false;
}

/**
 * Retorna o momento de cr�dito atual, ou o �ltimo momento que aconteceu dentro do exercicio.
 *
 * @global cls_banco $db Abstra��o da base de dados.
 * @param array $dados Filtros do formul�rio, se tiver um MCRID definido, tem prefer�ncia sobre as consultas.
 * @param string $exercicio O Exercicio atualmente selecionado.
 * @param bool $apenasOAtual Indica que s� deve retornar o momento ATUAL, retornando vazio se n�o houver nenhum aberto no momento.
 * @return array
 */
function momentoDeCreditoAtual($exercicio, $dados, $apenasOAtual = false)
{
    global $db;
    // -- Momento de cr�dito do filtro
    if (isset($dados['mcrid']) && !empty($dados['mcrid'])) {
        return array($dados['mcrid']);
    }

    // -- Momento de cr�dito ativo no banco, considerando o exercicio - range de datas
    $sql = <<<DML
SELECT mcr.mcrid,
       mcr.mcrtipocancelamento
  FROM altorc.momentocredito mcr
  WHERE NOW() BETWEEN mcr.mcrrefinicio AND mcr.mcrreffim
    AND mcr.mcrano = '%s'
    AND mcr.mcrstatus = 'A'
DML;
    $stmt = sprintf($sql, $exercicio);
    if ($dadosmcr = $db->pegaLinha($stmt)) {
        return array($dadosmcr['mcrid'], $dadosmcr['mcrtipocancelamento']);
    }

    // -- Se chegar aqui, e nenhum momento atual foi encontrado acima, retorna vazio, pois todos
    // -- os momentos do EXERCICIO est�o inativos.
    if ($apenasOAtual) {
        return array(null, null);
    }

    // -- �ltimo momento de cr�dito ativo no banco, considerando o exercicio - range de datas
    $sql = <<<DML
SELECT mcr.mcrid,
       mcr.mcrtipocancelamento
   FROM altorc.momentocredito mcr
   WHERE mcr.mcrano = '%s'
     AND mcr.mcrstatus = 'A'
   GROUP BY mcr.mcrid,
            mcr.mcrtipocancelamento
   ORDER BY MIN(now() - mcrreffim) ASC
   LIMIT 1
DML;
    $stmt = sprintf($sql, $exercicio);
    if ($dadosmcr = $db->pegaLinha($stmt)) {
        return array($dadosmcr['mcrid'], $dadosmcr['mcrtipocancelamento']);
    }

    return array(null, null);
}

function apagarPedido($dados, $exercicio, $usucpf)
{
    global $db;

    $fm = new Simec_Helper_FlashMessage('altorc/pedido');

    // -- Registro j� foi enviado para o SIOP? Tem SIOP ID?
    $sql = <<<DML
SELECT pao.siopid,
       pao.unicod
  FROM altorc.pedidoalteracaoorcamentaria pao
  WHERE pao.paoid = %d
DML;
    $stmt = sprintf($sql, $dados['paoid']);
    $dadosdb = $db->pegaLinha($stmt);

    if (in_array(PFL_UO_EQUIPE_TECNICA, pegaPerfilGeral($usucpf))
        && !in_array($dadosdb['unicod'], pegaResposabilidade($usucpf, PFL_UO_EQUIPE_TECNICA, 'unicod', null, true))) {
        $fm->addMensagem('Voc� n�o tem permiss�o para apagar este pedido.', Simec_Helper_FlashMessage::ERRO);
        return false;
    }

    $apagouNoSIOP = false;

    if ($dadosdb['siopid']) {
        // -- Apaga no SIOP
        $wsAltOrc = new WSAlteracoesOrcamentarias();
        $retorno = $wsAltOrc->excluirPedidoAlteracao($dadosdb['siopid'], $exercicio);

        // -- (log de) Transa��o iniciada
        $rqpid = criarNovaRequisicao($dados['paoid'], $retorno->return->sucesso);
        $retorno = $retorno->return;

        if ($retorno->sucesso) {
            $apagouNoSIOP = true;
            atualizaRequisicaoComoSucesso($rqpid);
        } else {

            // -- Link para aba de resumo, utilizada durante a exibi��o de erros
            $link = '<a href="altorc.php?modulo=principal/pedido/inicio&acao=A&dados[paoid]=' . $dados['paoid']
                  . '&target=resumo">Resumo/Tr�mite</a>';

            $msg = "N�o foi poss�vel apagar seu pedido no SIOP. Verifique a aba '{$link}' para maiores detalhes.";
            armazenaMensagensErroRequisicao($rqpid, $retorno->mensagensErro);
            $db->commit();
            $fm->addMensagem($msg, Simec_Helper_FlashMessage::ERRO);
            return false;
        }
    }

    // -- Apaga o registro na base - exclus�o l�gica
    $dml = <<<DML
UPDATE altorc.pedidoalteracaoorcamentaria
  SET paostatus = NULL
  WHERE paoid = %d
DML;
    $stmt = sprintf($dml, $dados['paoid']);
    if (!$db->executar($stmt)) {
        $db->rollback();
        if (!empty($siopid) && $apagouNoSIOP) {
            $msg = 'O pedido foi apagado no SIOP, mas n�o p�de ser apagado no SIMEC.';
        } else {
            $msg = 'N�o foi poss�vel apagar o pedido solicitado.';
        }
        $fm->addMensagem($msg, Simec_Helper_FlashMessage::ERRO);
        return false;
    }
    $db->commit();

    $fm->addMensagem('Seu pedido foi apagado com sucesso.');
    return true;
}

function alterarTipoCredito($paoid,$tcrid)
{

    global $db;
    $fm = new Simec_Helper_FlashMessage('altorc/pedido');
    if(!$paoid || !$tcrid){
        $fm->addMensagem('N�mero do Pedido ou Tipo de Cr�dito vazio.',Simec_Helper_FlashMessage::ERRO);
        return false;
    }
    $dml = <<<DML
        SELECT
            pao.tcrid AS tcrid,
            pao.paodsc AS paodsc,
            tcr.tcrcod AS tcrcod,
            (SELECT tcrcod FROM altorc.tipocredito WHERE tcrid = %d) AS tcrcod_novo,
            pao.mcrid,
            pao.unicod,
            pao.paoano,
            pao.paostatus
        FROM altorc.pedidoalteracaoorcamentaria pao
        INNER JOIN altorc.tipocredito tcr ON tcr.tcrid = pao.tcrid
        WHERE paoid = %d
DML;

    $stmt = sprintf($dml,$tcrid,$paoid);
    $dados = $db->pegaLinha($stmt);
    if(!$dados){
        $fm->addMensagem('N�o foi possivel encontrar os dados do pedido.',Simec_Helper_FlashMessage::ERRO);
        return false;
    }
    if($dados['tcrid'] == $tcrid){
        $fm->addMensagem('Tipo de cr�dito selecionado � o mesmo j� cadastrado para o pedido.',Simec_Helper_FlashMessage::ERRO);
        return false;
    }

    $split = split("/", $dados['paodsc']);
    $split[1] = $dados['tcrcod_novo'];

    $paodsc = implode('/',$split);

    $dml = <<<DML
        UPDATE altorc.pedidoalteracaoorcamentaria
        SET tcrid = %d,
        paodsc = '%s'
        WHERE paoid = %d
            AND NOT EXISTS(
                SELECT 1
                FROM altorc.pedidoalteracaoorcamentaria pao
                WHERE pao.mcrid = %d
                    AND pao.tcrid = %d
                    AND pao.unicod = '%s'
                    AND pao.paoano = '%s'
                    AND pao.paostatus = '%s'
                )
        RETURNING tcrid;
DML;

    $stmt = sprintf($dml,$tcrid,$paodsc, $paoid, $dados['mcrid'],$tcrid,$dados['unicod'],$dados['paoano'],$dados['paostatus']);

    if (!($result = $db->pegaUm($stmt))) {
        $msg = 'Falha ao alterar Pedido. J� existe um pedido cadastrado ativo com este tipo de cr�dito para essa unidade e exerc�cio.';
        $fm->addMensagem($msg, Simec_Helper_FlashMessage::ERRO);
        return false;
    }

    $db->commit();
    return true;
}
