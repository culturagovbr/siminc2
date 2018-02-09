<?php
/**
 * Carrega os dados financeiros do SIOP para a base do SIMEC.
 *
 * Assim que termina de baixar os dados financeiros, o script roda um processamento
 * que coloca os dados na tabela <tt>spo.siopexecucao</tt>. O acompanhamento das p�ginas
 * da execu��o j� baixadas � feito na tabela <tt>spo.siopexecucao_acompanhamento</tt>.
 * Ao final da execu��o, � enviado um e-mail com o resultado do processo.
 *
 * Sequ�ncia de execu��o:<br />
 * <ol><li>Baixa os dados do webservice (WSQuantitativo.consultarExecucaoOrcamentaria);</li>
 * <li>Apaga os dados da tabela wssof.ws_execucaoorcamentaria;</li>
 * <li>Insere os dados retornados pelo webservice na tabela wssof.ws_execucaoorcamentaria;</li>
 * <li>Executa o script de atualiza��o de finaceiros na seguinte tabela: spo.siopexecucao;</li>
 * <li>Envia e-mail com resultado da execu��o.</li></ol>
 *
 * @version $Id: spo_BaixarDadosFinanceirosSIOP.php 101880 2015-08-31 19:50:33Z maykelbraz $
 * @link http://simec.mec.gov.br/seguranca/scripts_exec/spo_BaixarDadosFinanceirosSIOP.php URL de execu��o.
 */

// -- Setup
ini_set('display_errors', 1);
ini_set('error_reporting', E_ALL & ~E_NOTICE);
set_time_limit(0);
ini_set("memory_limit", "2048M");
define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));
session_start();
$_SESSION['baselogin'] = "simec_espelho_producao";

// -- Includes necess�rios ao processamento
/**
 * Carrega as configura��es gerais do sistema.
 * @see config.inc
 */
require_once BASE_PATH_SIMEC . "/global/config.inc";

/**
 * Carrega as classes do simec.
 * @see classes_simec.inc
 */
require_once APPRAIZ . "includes/classes_simec.inc";

/**
 * Carrega as fun��es b�sicas do simec.
 * @see funcoes.inc
 */
require_once APPRAIZ . "includes/funcoes.inc";

/**
 * Classe de conex�o com o SIOP, servi�o WSQuantitativo.
 * @see Spo_Ws_Sof_Quantitativo
 */
require_once(APPRAIZ . 'spo/ws/sof/Quantitativo.php');

// -- Abrindo conex�o com o banco de dados
$db = new cls_banco();

// -- fun��es
/*
 * Muda a quantidade "m�xima" de p�ginas
 */
function mudarQuantidadePaginas($quantidade) {
    global $db;
    $sql = "DELETE FROM spo.siopexecucao_acompanhamento";
    $db->executar($sql);
    for ($i = 1; $i <= $quantidade; $i++) {
        $sql = "INSERT INTO spo.siopexecucao_acompanhamento (pagina, data) VALUES ({$i}, '1900-01-01');";
        $db->executar($sql);
    }
    $db->commit();
}

function processarRegistros($anoDeExecucao)
{
    global $db;

    $dml = <<<DML
DELETE
  FROM spo.siopexecucao
  WHERE exercicio = '{$anoDeExecucao}';

INSERT INTO spo.siopexecucao(
   exercicio,
   anoreferencia,
   esfcod,
   unicod,
   funcod,
   sfucod,
   prgcod,
   acacod,
   loccod,
   plocod, -- N�o retornar� nada antes de 2012, 2012 n�o incluso.
   ptres,
   plicod,
   vlrdotacaoinicial,
   vlrdotacaoatual,
   vlrempenhado,
   vlrliquidado,
   vlrpago,
   vlrrapnaoprocessadopago,
   vlrrapprocessadopago,
   vlrrapnaoprocessadoinscritoliquido,
   vlrrapnaoprocessadoliquidadoapagar,
   vlrrapnaoprocessadoliquidadoefetivo
)
SELECT DISTINCT exo.anoexercicio AS exercicio,
                exo.anoreferencia,
                exo.esfera AS esfcod,
                exo.unidadeorcamentaria AS unicod,
                exo.funcao AS funcod,
                exo.subfuncao AS sfucod,
                exo.programa AS prgcod,
                exo.acao AS acacod,
                exo.localizador AS loccod,
                CASE -- Anos anoteriores a 2012 n�o possuem PO, no entanto, o sistema espera o c�digo 0000
                    WHEN exo.anoexercicio <= 2011 THEN COALESCE(exo.planoorcamentario, '0000')
                    ELSE exo.planoorcamentario
                  END AS plocod,
                exo.numeroptres AS ptres,
                exo.planointerno AS plicod,
                CASE WHEN exo.dotacaoinicial <> '' THEN exo.dotacaoinicial::NUMERIC
                     ELSE 0
                  END AS vlrdotacaoinicial ,
                CASE WHEN exo.dotatual <> '' THEN exo.dotatual::NUMERIC
                     ELSE 0
                  END AS vlrdotacaoatual ,
                exo.empliquidado::NUMERIC + exo.empenhadoaliquidar::NUMERIC AS vlrempenhado,
                exo.empliquidado::NUMERIC AS vlrliquidado,
                exo.pago::NUMERIC AS vlrpago,
                -- RAP n�o processado Pago
                -- RAP Processados Pagos
                -- RAP n�o processado Inscrito L�quido = RAP Inscrito N�o Processado + RAP Exerc�cios Anteriores + (RAP Cancelados N�o Processados)
                -- RAP n�o processado Liquidado a pagar
                -- RAP n�o processado Liquidado efetivo = RAP N�o Processado Liquidado A Pagar + RAP Pago N�o Processado
                exo.rappagonaoprocessado::numeric AS vlrrapnaoprocessadopago,
                exo.rappagoprocessado::numeric AS vlrrapprocessadopago,
                exo.rapinscritonaoprocessado::numeric + exo.rapexerciciosanteriores::numeric + (exo.rapcanceladosnaoprocessados::numeric) AS vlrrapnaoprocessadoinscritoliquido,
                exo.rapnaoprocessadoliquidadoapagar::numeric AS vlrrapnaoprocessadoliquidadoapagar,
                exo.rapnaoprocessadoliquidadoapagar::numeric + exo.rappagonaoprocessado::numeric AS vlrrapnaoprocessadoliquidadoefetivo
  FROM wssof.ws_execucaoorcamentariadto_carga exo
  WHERE anoexercicio = '{$anoDeExecucao}';
DML;

    $db->executar($dml);
    $db->commit();
}

function atualizaPaginaAtual($paginaAtual, $qtdRegistros)
{
    global $db;
    $sql = <<<DML
UPDATE spo.siopexecucao_acompanhamento
  SET data = CURRENT_DATE,
      resultado = 'SUCESSO',
      registros = %d
  WHERE pagina = %d
DML;
    $db->executar(sprintf($sql, $qtdRegistros, $paginaAtual));
    $db->commit();
}

function atualizaPaginasRestantes($paginaAtual)
{
    global $db;
    $sql = <<<DML
UPDATE spo.siopexecucao_acompanhamento
  SET data = CURRENT_DATE,
      resultado = 'SUCESSO',
      registros = 0
  WHERE pagina >= %d
DML;
    $db->executar(sprintf($sql, $paginaAtual));
    $db->commit();
}

function insereRegistrosTabelaTemporaria($data)
{
    global $db, $insertDML;

    foreach ($data as $linha) {
        $stmt = vsprintf($insertDML, $linha);
        $db->executar($stmt);
    }
    return (bool)$db->commit();
}

function limparTabelaTemporaria()
{
    global $db;

    $dml = <<<DML
DELETE FROM wssof.ws_execucaoorcamentariadto_carga
DML;
    $db->executar($dml);
    $db->commit();
}

function paginaInicial()
{
    if ($_REQUEST['pagina']) {
        $pagina = $_REQUEST['pagina'];
    } else {
        global $db;

        $query = <<<DML
SELECT MIN(pagina) AS pagina
  FROM spo.siopexecucao_acompanhamento
  WHERE data < CURRENT_DATE
    OR resultado <> 'SUCESSO'
DML;
        $pagina = $db->pegaUm($query);
    }

    return $pagina;
}

// -- Iniciando vari�veis

// -- CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

// -- Filtros
$anoDeExecucao = (int)($_GET['ano']?$_GET['ano']:date('Y')); // -- Ano utilizado pelo script para execu��o
$unidadeOrcamentaria = $_GET['uo']?explode(',', $_GET['uo']):null; // -- Filtro de UO

/*
 * Par�metro para quando for executar uma "carga completa"
 * N�o ser� processado para a tabela [ spo.siopexecucao ] os dados v�o apenas para [ wssof.ws_execucaoorcamentariadto_carga ]
 * Esses dados s�o para Snapshots, por exemplo, de Momento de Cr�dito
 */
if ($_GET['carga_com_fonte_e_nd'] === 'SIM') {
    $executarCargaComFonteND = true;
    mudarQuantidadePaginas(200);
}

// -- Iniciando o armazenamento do buffer de sa�da - incluso no log de execu��o enviado por e-mail
ob_start();

// -- P�gina a ser consultada
$pagina = paginaInicial();

// -- Execu��o
if ($pagina) {

    // -- Apagando os registros armazenados em wssof.ws_execucaofinanceira - tabela tempor�ria
    if (1 == $pagina) {
        limparTabelaTemporaria();
    }

    // -- Inicializando o controle de fluxo
    $resultadoExecucao = 'FALHA';
    $continuar = true;
    $erro = false;

    // -- Filtro do m�todo de execu��o or�ament�ria
    $filtro = array('anoExercicio' => $anoDeExecucao);
    if (!empty($unidadeOrcamentaria)) {
        $filtro['unidadesOrcamentarias'] = $unidadeOrcamentaria;
    }

    // -- Retorno do m�todo de execu��o or�ament�ria
    $retorno = array(
        'anoExercicio',
        'anoReferencia',
        'programa',
        'acao',
        'localizador',
        'unidadeOrcamentaria',
        'planoInterno',
        'numeroptres',
        'esfera',
        'funcao',
        'subFuncao',
        'dotacaoInicial',
        'dotAtual',
        'empLiquidado',
        'empenhadoALiquidar',
        'pago',
        'rapInscritoProcessado',
        'rapExerciciosAnteriores',
        'rapCanceladosNaoProcessados',
        'rapCanceladosProcessados',
        'rapAPagarNaoProcessado',
        'rapAPagarProcessado',
        'rapPagoNaoProcessado',
        'rapPagoProcessado',
        'rapInscritoNaoProcessado',
        'rapNaoProcessadoBloqueado',
        'rapNaoProcessadoALiquidar',
        'rapNaoProcessadoLiquidadoAPagar',
        'planoOrcamentario'
    );
    /*
     * Mudando o XML da chamada, caso seja carga com Fonte e ND
     */
    if ($executarCargaComFonteND) {
        array_push($retorno, 'fonte');
        array_push($retorno, 'natureza');
    }

    // -- Criando o insert com base no retorno
    $campos = implode(', ', $retorno);
    $placeholders = implode(',', array_fill(0, count($retorno), "'%s'"));
    $insertDML = <<<DML
INSERT INTO wssof.ws_execucaoorcamentariadto_carga({$campos})
  VALUES({$placeholders})
DML;
    unset($campos, $placeholders);

    // -- Inicializando a conex�o com o webservice
    $ws = new Spo_Ws_Sof_Quantitativo('spo', Spo_Ws_Sof_Quantitativo::PRODUCTION);

    do {
        // -- Consultando os dados no WS
        $wsdata = $ws->consultarExecucaoOrcamentaria($filtro, $retorno, $pagina, true);

        // -- Inserindo os novos registros
        if (!$wsdata || !insereRegistrosTabelaTemporaria($wsdata)) {
            $continuar = false;
            $erro = true;
            break; // -- Termina o loop
        }

        // -- Atualizando no banco a p�gina que foi baixada
        atualizaPaginaAtual($pagina++, count($wsdata));

        if (count($wsdata) < 2000) {
            $continuar = false;
            break;
        }
    } while ($continuar);

    if (!$erro) {
        // -- Se tudo correu bem, atualiza as p�ginas restantes no banco de dados
        atualizaPaginasRestantes($pagina);
        /*
         * Processa apenas para a carga di�ria, para a carga completa n�o
         */
        if (!$executarCargaComFonteND) {
            // -- Processando os registros baixados e inserindo em spo.siopexecucao
            processarRegistros($anoDeExecucao);
        } else {
            /*
             * Volta a quantidade m�xima de p�ginas ao "normal"
             */
           mudarQuantidadePaginas(30);
        }
        $resultadoExecucao = 'SUCESSO';
    } else {
        $resultadoExecucao = 'ERRO';
    }
} else {
    $resultadoExecucao = 'NADA PARA ATUALIZAR';
}

//echo '<pre>';
//$data = var_export($_SERVER);
//echo '</pre>';
//ver($content);
//$content = ob_get_contents();

//enviar_email(
//    '',
//    array(
//        array('usuemail' => $_SESSION['email_sistema'], 'usunome' => SIGLA_SISTEMA)
//    ),
//    'Carga SIOP - ' . date('d/m/Y') . ' - ' . $resultadoExecucao,
//    "Execu��o da carga do SIAFI.\nResultado: {$resultadoExecucao}<br />"
//    . "Servidor: {$_SERVER['SERVER_NAME']}<br />"
//    . "Arquivo: " . __FILE__ . "<br />"
//    . "Detalhes: <pre>{$data}</pre>"
//    . "Conte�do: <pre>{$content}</pre>"
//);

//ob_end_flush();
//$db->close();

