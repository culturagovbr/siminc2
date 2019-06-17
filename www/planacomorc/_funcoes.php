<?php
/**
 * Funções de apoio do módulo Planacomorc.
 * $Id: _funcoes.php 102359 2015-09-11 18:26:07Z maykelbraz $
 */
require_once APPRAIZ . 'includes/funcoesspo.php';
/**
 *
 * @param type $dados
 */
function alertlocation($dados) {

    die("<script>
		" . (($dados['alert']) ? "alert('" . $dados['alert'] . "');" : "") . "
		" . (($dados['location']) ? "window.location='" . $dados['location'] . "';" : "") . "
		" . (($dados['javascript']) ? $dados['javascript'] : "") . "
		 </script>");
}

function mascaraglobal($value, $mask) {
    $casasdec = explode(",", $mask);
// Se possui casas decimais
    if ($casasdec[1])
        $value = sprintf("%01." . strlen($casasdec[1]) . "f", $value);

    $value = str_replace(array("."), array(""), $value);
    if (strlen($mask) > 0) {
        $masklen = -1;
        $valuelen = -1;
        while ($masklen >= -strlen($mask)) {
            if (-strlen($value) <= $valuelen) {
                if (substr($mask, $masklen, 1) == "#") {
                    $valueformatado = trim(substr($value, $valuelen, 1)) . $valueformatado;
                    $valuelen--;
                } else {
                    if (trim(substr($value, $valuelen, 1)) != "") {
                        $valueformatado = trim(substr($mask, $masklen, 1)) . $valueformatado;
                    }
                }
            }
            $masklen--;
        }
    }
    return $valueformatado;
}

/**
 * Verifica se o dia atual é posterior à dia limite de tramitação da ação.
 * @return bool
 */
function excedeuDataLimite() {
    $dtHoje = new DateTime();
    $dtLimite = new DateTime('08-08-2013');

    return $dtLimite < $dtHoje;
}

function condicaoEmElaboracao($id_acao_programatica) {
    global $db;

    if ($db->testa_superuser()) {
        return true;
    }

    $sql = "SELECT rpuid
              FROM planacomorc.usuarioresponsabilidade
              WHERE rpustatus='A'
                AND usucpf='" . $_SESSION['usucpf'] . "'
                AND (pflcod='" . PFL_COORDENADORACAO . "' OR pflcod='" . PFL_VALIDADORACAO . "' OR pflcod='" . PFL_VALIDADOR_SUBSTITUTO . "')
                AND id_acao_programatica='" . $id_acao_programatica . "'";
    $possui_permissao_coordenadoracao = $db->pegaUm($sql);
    $excedeuDataLimite = excedeuDataLimite();
    $naoExcedeuDataLimite = !$excedeuDataLimite;
    $sql = "SELECT COUNT(1) AS qtd_tramites_para_validacao
              FROM planacomorc.acompanhamento_acao
                INNER JOIN workflow.documento doc USING(docid)
                INNER JOIN workflow.historicodocumento hsd USING(docid)
                INNER JOIN workflow.acaoestadodoc aed USING(aedid)
              WHERE id_acao_programatica = {$id_acao_programatica}
                AND aed.esdidorigem = " . ESD_EMVALIDACAO;
    $jaHaviaEnviado = $db->pegaUm($sql);
    $sqlEhDaCasa = "SELECT COUNT(1) AS eh_da_casa
                      FROM planacomorc.acao_programatica apr
                        INNER JOIN planacomorc.orgao org USING(id_orgao)
                      WHERE apr.id_acao_programatica = {$id_acao_programatica}
                        AND org.codigo IN('26101', '26298', '26291', '26443', '26290', '74902')";
    $ehDaCasa = $db->pegaUm($sqlEhDaCasa);

    if ($possui_permissao_coordenadoracao && (
            ($naoExcedeuDataLimite || ($excedeuDataLimite && $jaHaviaEnviado)) || $ehDaCasa
            )
    ) {
        return true;
    }

    return "Você não possui autorização para enviar ação ao validador";
}

function condicaoEmValidacao($id_acao_programatica) {
    global $db;

    if ($db->testa_superuser()) {
        return true;
    }

    $possui_permissao_validadoracao = $db->pegaUm("SELECT rpuid
                                                     FROM planacomorc.usuarioresponsabilidade
                                                     WHERE rpustatus='A'
                                                       AND usucpf='" . $_SESSION['usucpf'] . "'
                                                       AND (pflcod='" . PFL_VALIDADORACAO . "' OR pflcod='" . PFL_VALIDADOR_SUBSTITUTO . "')
                                                       AND id_acao_programatica='" . $id_acao_programatica . "'");

    if ($possui_permissao_validadoracao) {
        return true;
    }

    return "Você não possui autorização para encaminhar ação a CMPO";
}

function consultarPermissao($dados) {
    global $db;

    $perfis = pegaPerfilGeral();

    if ($db->testa_superuser() || in_array(PFL_CPMO, $perfis)) {
        return true;
    }

    /* Bloqueia o preenchimento fora do período para os usuário UO */
    if (in_array(array(PFL_VALIDADORACAO, PFL_VALIDADOR_SUBSTITUTO), $perfis)) {
        $sql = " SELECT
            CASE
                WHEN CURRENT_DATE BETWEEN inicio_preenchimento AND fim_preenchimento
                THEN TRUE
                ELSE FALSE
            END AS validade_periodo
        FROM
            planacomorc.periodo_referencia
        WHERE
            id_periodo_referencia = {$dados['id_periodo_referencia']}";
        if ($db->pegaUm($sql)) {
            return false;
        }
    }

    if (in_array(PFL_COORDENADORACAO, $perfis)) {

        $possui_permissao_coordenadoracao = $db->pegaUm("SELECT rpuid
                                                           FROM planacomorc.usuarioresponsabilidade
                                                           WHERE rpustatus='A'
                                                             AND usucpf='" . $_SESSION['usucpf'] . "'
                                                             AND pflcod='" . PFL_COORDENADORACAO . "'
                                                             AND id_acao_programatica='" . $dados['id_acao_programatica'] . "'");
        if ((!$dados['esdid'] || $dados['esdid'] == ESD_EMELABORACAO) && $possui_permissao_coordenadoracao) {
            return true;
        }
    }

    if (in_array(PFL_VALIDADORACAO, $perfis) || in_array(PFL_VALIDADOR_SUBSTITUTO, $perfis)) {
        $possui_permissao_validadoracao = $db->pegaUm(
                "SELECT rpuid
                   FROM planacomorc.usuarioresponsabilidade
                   WHERE rpustatus='A'
                     AND usucpf='" . $_SESSION['usucpf'] . "'
                     AND (pflcod='" . PFL_VALIDADORACAO . "' OR pflcod='" . PFL_VALIDADOR_SUBSTITUTO . "')
                     AND id_acao_programatica='" . $dados['id_acao_programatica'] . "'");

        if (((!$dados['esdid'] || $dados['esdid'] == ESD_EMELABORACAO) || $dados['esdid'] == ESD_EMVALIDACAO) && $possui_permissao_validadoracao) {
            return true;
        }
    }

    return false;
}

function number_format2($number) {
    if (!$number) {
        return 'R$ 0,00';
    }
    return 'R$ ' . number_format($number, 2, ',', '.');
}

/**
 * Consulta a tabela de log em busca de registros para a ação programática.
 * @global cls_banco $db Conexão com a base de dados
 * @param type $idAcaoProgramatica
 */
function exibirLogEnvio($dados) {
    global $db;
    $sql = <<<DML
SELECT TO_CHAR(datacriacao, 'DD/MM/YYYY HH24:MI:SS') AS datacriacao,
       COALESCE(wslmsgretorno, '<center>Não informado</center>') AS descricao
  FROM elabrev.ws_log
  WHERE id_acao_programatica = {$dados['id_acao_programatica']}
  ORDER BY datacriacao DESC
  LIMIT 5
DML;
  $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
  $listagem->addCallbackDeCampo(array('descricao' ), 'alinhaParaEsquerda');
  $listagem->setCabecalho(array('Data', 'Descrição'));
  $listagem->setQuery($sql);
  $listagem->render();
  //$db->monta_lista_simples($sql, array('Data', 'Descrição'), 25, 1300, null, null, null, null, array(20, 80), 300, true);
}

/**
 * Carrega scripts estaticos, js, css, etc...
 * @return String (HTML)
 */
function loadStaticScripts() {

    $files = array(
        'js' => array(
            '/includes/JQuery/jquery-1.9.1/jquery-1.9.1.js',
            '/pnbe/js/jquery.easing.min.js',
            '/pnbe/jquery-ui-1.9.2.custom/js/jquery-ui-1.9.2.custom.min.js',
            '/planacomorc/js/tabelasApoio.js',
            '/planacomorc/js/jquery.livequery.js',
        ),
        'css' => array(
            'http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.9/themes/base/jquery-ui.css',
            '/planacomorc/css/tabelasApoio.css',
        )
    );

    $markup = array(
        'js' => '<script src="%s" type="text/javascript"></script>',
        'css' => '<link rel="stylesheet" type="text/css" href="%s"/>'
    );

    $output = '';
    foreach ($files['js'] as $file) {
        $output.= sprintf($markup['js'], $file);
    }

    foreach ($files['css'] as $file) {
        $output.= sprintf($markup['css'], $file);
    }

    echo $output;
}

/**
 * Is request xmlHttpRequest
 * @return bool
 */
function isAjax() {
    return (!empty($_SERVER['HTTP_X_REQUESTED_WITH']) &&
            strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest');
}

/**
 * Pega os dados do registro e retorna em json
 * @global cls_banco $db
 * @param int $rowID
 * @return Void(0)
 */
function updateEnquadramentoDespesas($rowID) {

    global $db;

    $strSql = 'SELECT * FROM monitora.pi_enquadramentodespesa WHERE eqdid=' . $rowID;
    $rs = $db->carregar($strSql);
    if (!$rs)
        return false;
    $rs = $rs[0];

    foreach ($rs as $k => $v) {
        $rs[$k] = utf8_encode($v);
    }

    header('Content-Type: application/json; charset=utf-8', true, 200);
    echo simec_json_encode($rs);
    exit;
}

/**
 * Pega os dados do registro e retorna em json
 * @global cls_banco $db
 * @param int $rowID
 * @return Void(0)
 */
function updateModalidadeEnsino($rowID) {

    global $db;

    $strSql = 'SELECT * FROM monitora.pi_modalidadeensino WHERE mdeid=' . $rowID;
    $rs = $db->carregar($strSql);
    if (!$rs)
        return false;
    $rs = $rs[0];

    foreach ($rs as $k => $v) {
        $rs[$k] = utf8_encode($v);
    }

    header('Content-Type: application/json; charset=utf-8', true, 200);
    echo simec_json_encode($rs);
    exit;
}

/**
 * Pega os dados do registro e retorna em json
 * @global cls_banco $db
 * @param int $rowID
 * @return Void(0)
 */
function updateNivelEtapaEnsino($rowID) {

    global $db;

    $strSql = 'SELECT * FROM monitora.pi_niveletapaensino WHERE neeid=' . $rowID;
    $rs = $db->carregar($strSql);
    if (!$rs)
        return false;
    $rs = $rs[0];

    foreach ($rs as $k => $v) {
        $rs[$k] = utf8_encode($v);
    }

    header('Content-Type: application/json; charset=utf-8', true, 200);
    echo simec_json_encode($rs);
    exit;
}

/**
 * Apaga um registro da tabela
 * @global cls_banco $db
 * @param int $rowID
 * @return boolean
 */
function deleteRowEnquadramentoDespesas($rowID) {

    global $db;

    $strSql = sprintf('UPDATE monitora.pi_enquadramentodespesa SET eqdstatus=\'I\' WHERE eqdid=%d', (int) $rowID);
    //$db->executar($strSql);
      if ($db->executar( $strSql )) {
        $db->commit ();
      }
    //$result = ($db->commit()) ? 'success' : 'fail';

   // header('Content-Type: application/json; charset=utf-8', true, 200);
    //echo simec_json_encode(array('result' => $result));
    //exit;
}

/**
 * Apaga um registro da tabela
 * @global cls_banco $db
 * @param int $rowID
 * @return boolean
 */
function deleteRowCategoriaApropriacao($rowID) {

    global $db;
    $strSql = sprintf('UPDATE monitora.pi_categoriaapropriacao SET capstatus=\'I\' WHERE capid=%d', (int) $rowID);
    if ($db->executar( $strSql )) {
        $db->commit ();
    }
}

/**
 * Apaga um registro da tabela
 * @global cls_banco $db
 * @param int $rowID
 * @return boolean
 */
function deleteRowModalidadeEnsino($rowID) {

    global $db;

    $strSql = sprintf('UPDATE monitora.pi_modalidadeensino SET mdestatus=\'I\' WHERE mdeid=%d', (int) $rowID);
     if ($db->executar( $strSql )) {
        $db->commit ();
      }
    //$db->executar($strSql);
//    $result = ($db->commit()) ? 'success' : 'fail';
//
//    header('Content-Type: application/json; charset=utf-8', true, 200);
//    echo simec_json_encode(array('result' => $result));
//    exit;
}

/**
 * Apaga um registro da tabela
 * @global cls_banco $db
 * @param int $rowID
 * @return boolean
 */
function deleteRowNivelEtapaEnsino($rowID) {

    global $db;

    $strSql = sprintf('UPDATE monitora.pi_niveletapaensino SET neestatus=\'I\' WHERE neeid=%d', (int) $rowID);
      if ($db->executar( $strSql )) {
        $db->commit ();
      }
      //$db->executar($strSql);
    //$result = ($db->commit()) ? 'success' : 'fail';

    //header('Content-Type: application/json; charset=utf-8', true, 200);
    //echo simec_json_encode(array('result' => $result));
    //exit;
}

/**
 * Salva um registro na base de dados
 * @param array $post
 * @return boolean
 */
function salvaEnquadramentoDespesas(array $post) {

    global $db;
    extract($post);

//insere
    if (empty($post['eqdid'])) {

        $strSqlBase = "INSERT INTO
            monitora.pi_enquadramentodespesa(eqdcod, eqddsc, eqdano, eqdstatus)
            VALUES('%s', '%s', '%s', '%s')";
        $strSql = sprintf($strSqlBase, $eqdcod, $eqddsc, $eqdano, 'A');
        $mensagem = 'inserido';
    } else {
//atualiza
        $strSql = "UPDATE monitora.pi_enquadramentodespesa SET ";
        $eqdid = $post['eqdid'];
        unset($post['eqdid']);

        foreach ($post as $k => $value)
            $strSql.= "{$k} = '{$value}',";

        $strSql = substr($strSql, 0, -1);
        $strSql.= " WHERE eqdid=" . $eqdid;
        $mensagem = 'atualizado';
    }

    $db->executar($strSql);
    $db->commit();

    alertlocation(array(
        'location' => 'planacomorc.php?modulo=sistema/tabelasapoio/cadEnquadramentoDespesas&acao=A'
        , 'alert' => "Registro {$mensagem} com sucesso!"
    ));
}

/**
 * Salva um registro na base de dados
 * @param array $post
 * @return boolean
 */
function salvaModalidadeEnsino(array $post) {

    global $db;
    extract($post);

//insere
    if (empty($post['mdeid'])) {

        $strSqlBase = "INSERT INTO
            monitora.pi_modalidadeensino(mdecod, mdedsc, mdeano, mdestatus)
            VALUES('%s', '%s', '%s', '%s')";
        $strSql = sprintf($strSqlBase, $mdecod, $mdedsc, $mdeano, 'A');
        $mensagem = 'inserido';
    } else {
//atualiza
        $strSql = "UPDATE monitora.pi_modalidadeensino SET ";
        $mdeid = $post['mdeid'];
        unset($post['mdeid']);

        foreach ($post as $k => $value)
            $strSql.= "{$k} = '{$value}',";

        $strSql = substr($strSql, 0, -1);
        $strSql.= " WHERE mdeid=" . $mdeid;
        $mensagem = 'atualizado';
    }

    $db->executar($strSql);
    $db->commit();

    alertlocation(array(
        'location' => 'planacomorc.php?modulo=sistema/tabelasapoio/cadModalidadesEnsino&acao=A'
        , 'alert' => "Registro {$mensagem} com sucesso!"
    ));
}

/**
 * Salva um registro na base de dados
 * @param array $post
 * @return boolean
 */
function salvaNivelEtapaEnsino(array $post) {

    global $db;
    extract($post);

//insere
    if (empty($post['neeid'])) {

        $strSqlBase = "INSERT INTO
            monitora.pi_niveletapaensino(neecod, needsc, neeano, neestatus)
            VALUES('%s', '%s', '%s', '%s')";
        $strSql = sprintf($strSqlBase, $neecod, $needsc, $neeano, 'A');
        $mensagem = 'inserido';
    } else {
//atualiza
        $strSql = "UPDATE monitora.pi_niveletapaensino SET ";
        $neeid = $post['neeid'];
        unset($post['neeid']);

        foreach ($post as $k => $value)
            $strSql.= "{$k} = '{$value}',";

        $strSql = substr($strSql, 0, -1);
        $strSql.= " WHERE neeid=" . $neeid;
        $mensagem = 'atualizado';
    }

    $db->executar($strSql);
    $db->commit();

    alertlocation(array(
        'location' => 'planacomorc.php?modulo=sistema/tabelasapoio/cadNivelEtapaEnsino&acao=A'
        , 'alert' => "Registro {$mensagem} com sucesso!"
    ));
}

function updateGestorSubacao($rowID) {
    global $db;

    $strSql = 'SELECT * FROM monitora.pi_gestor WHERE pigid=' . $rowID;
    $rs = $db->carregar($strSql);
    if (!$rs)
        return false;
    $rs = $rs[0];

    foreach ($rs as $k => $v) {
        $rs[$k] = utf8_encode($v);
    }

    header('Content-Type: application/json; charset=utf-8', true, 200);
    echo simec_json_encode($rs);
    exit;
}

function deleteRowGestorSubacao($rowID) {
    global $db;

    $strSql = sprintf('UPDATE monitora.pi_gestor SET pigstatus=\'I\' WHERE pigid=%d', (int) $rowID);
   // ver($strSql,d);
     if ($db->executar( $strSql )) {
        $db->commit ();
      }
//    $db->executar($strSql);
//    $result = ($db->commit()) ? 'success' : 'fail';
//
//    header('Content-Type: application/json; charset=utf-8', true, 200);
//    echo simec_json_encode(array('result' => $result));
//    exit;
}

function salvaGestorSubacao(array $post) {
    global $db;
    extract($post);

//insere
    if (empty($post['pigid'])) {

        $strSqlBase = "INSERT INTO
            monitora.pi_gestor(pigano, pigstatus, pigcod, pigdsc)
            VALUES('%s', '%s', '%s', '%s')";
        $strSql = sprintf($strSqlBase, $pigano, 'A', $pigcod, $pigdsc);
        $mensagem = 'inserido';
    } else {
//atualiza
        $strSql = "UPDATE monitora.pi_gestor SET ";
        $pigid = $post['pigid'];
        unset($post['pigid']);

        foreach ($post as $k => $value)
            $strSql.= "{$k} = '{$value}',";

        $strSql = substr($strSql, 0, -1);
        $strSql.= " WHERE pigid=" . $pigid;
        $mensagem = 'atualizado';
    }

    $db->executar($strSql);
    $db->commit();

    alertlocation(array(
        'location' => 'planacomorc.php?modulo=sistema/tabelasapoio/cadGestorSubacao&acao=A'
        , 'alert' => "Registro {$mensagem} com sucesso!"
    ));
}

function updateExecutorOrcamentario($rowID) {
    global $db;

    $strSql = 'SELECT * FROM monitora.pi_executor WHERE pieid=' . $rowID;
    $rs = $db->carregar($strSql);
    if (!$rs)
        return false;
    $rs = $rs[0];

    foreach ($rs as $k => $v) {
        $rs[$k] = utf8_encode($v);
    }

    header('Content-Type: application/json; charset=utf-8', true, 200);
    echo simec_json_encode($rs);
    exit;
}

function deleteRowExecutorOrcamentario($rowID) {
    global $db;

    $strSql = sprintf('UPDATE monitora.pi_executor SET piestatus=\'I\' WHERE pieid=%d', (int) $rowID);
     if ($db->executar( $strSql )) {
        $db->commit ();
      }
//    $db->executar($strSql);
//    $result = ($db->commit()) ? 'success' : 'fail';
//
//    header('Content-Type: application/json; charset=utf-8', true, 200);
//    echo simec_json_encode(array('result' => $result));
//    exit;
}

function salvaExecutorOrcamentario(array $post) {
    global $db;
    extract($post);

//insere
    if (empty($post['pieid'])) {

        $strSqlBase = "INSERT INTO
            monitora.pi_executor(pieano, piestatus, piecod, piedsc)
            VALUES('%s', '%s', '%s', '%s')";
        $strSql = sprintf($strSqlBase, $pieano, 'A', $piecod, $piedsc);
        $mensagem = 'inserido';
    } else {
//atualiza
        $strSql = "UPDATE monitora.pi_executor SET ";
        $pieid = $post['pieid'];
        unset($post['pieid']);

        foreach ($post as $k => $value)
            $strSql.= "{$k} = '{$value}',";

        $strSql = substr($strSql, 0, -1);
        $strSql.= " WHERE pieid=" . $pieid;
        $mensagem = 'atualizado';
    }

    $db->executar($strSql);
    $db->commit();

    alertlocation(array(
        'location' => 'planacomorc.php?modulo=sistema/tabelasapoio/cadExecutorOrcamentarioFinanceiro&acao=A'
        , 'alert' => "Registro {$mensagem} com sucesso!"
    ));
}

/**
 * Retorna a consulta para Subação
 */
function retornaConsultaSubacao(array $params, $apenasObrigatorias = "") {
    /* Unidades Obrigatórias (AD, CAPES, INEP, FNDE, FIES, SUP.MEC, EBSERH */
    $obrigatorias = UNIDADES_OBRIGATORIAS;
    $perfis = pegaPerfilGeral();
    if($params['unicod']){
        $whereUnidade = "AND uni.unicod ='".$params['unicod']."'";
    }
    if ($apenasObrigatorias == 'n') {
        $whereObrigatorio = ' AND aca.unicod NOT IN(' . $obrigatorias . ')) AS dotacao,';
        $whereObrigatorio2 = 'AND (ptr.unicod NOT IN(' . $obrigatorias . ')  OR ptr.unicod IS NULL)';
        $whereUnicodNotNull = 'and psu.unicod is not null';

        if (in_array(PFL_GESTAO_ORCAMENTARIA_IFS, $perfis)) {
            $sqlUO = <<<DML
EXISTS (SELECT 1
         FROM planacomorc.usuarioresponsabilidade rpu
         WHERE rpu.usucpf = '%s'
           AND rpu.pflcod = %d
           AND rpu.rpustatus = 'A'
           AND rpu.unicod  = uni.unicod)
DML;
            $wherePerfil[] = $whereUO = sprintf($sqlUO, $_SESSION['usucpf'], PFL_GESTAO_ORCAMENTARIA_IFS);
            $whereUO = " AND {$whereUO}";

        }
    } else {
        $whereObrigatorio = ' AND aca.unicod IN(' . $obrigatorias . ')) AS dotacao,';
        $whereObrigatorio2 = 'AND (ptr.unicod IN(' . $obrigatorias . ')  OR ptr.unicod IS NULL) AND psu.unicod IS NULL';
    }
    /* Cabeçalho do SELECT */
    if (!$params['SELECT']) {
//        //SELECT '<center>' || acoes || CASE WHEN SUM(cast(detalhado_pi AS NUMERIC)) > 0
//                                     THEN '<img src="/imagens/excluir.gif" border="0" title="Excluir" style="cursor:pointer" '
//                                            || 'onclick="alert(''Não é possível remover uma subação que já apresente valores detalhados em PI.'');" />'
//                                   ELSE '<img src="/imagens/excluir.gif" border="0" title="Excluir" style="cursor:pointer" '
//                                          || 'onclick="removerSubacao(' || sbaid || ', ''' || sbacod || ''');" />'
//                              END || '</center>',
        $params['SELECT'] = <<<SQL
SELECT sbaid as acoes,
       codigo,
       sbatitulo,
       unidsc,
       dotacao,
       SUM(CAST(detalhado_pi AS NUMERIC)) AS detalhado_pi,
       SUM(empenhado) AS empenhado,
       SUM(dotacao) - SUM(CAST(detalhado_pi AS NUMERIC)) AS saldo_nao_detalhado,
       SUM(dotacao) - SUM(empenhado) AS saldo_nao_empenhado
SQL;
    }
    /* WHERE */
    if ($params['where']) {
        $where = $params['where'];
    }

    /* Filtro para listar apenas as subações que o usuário logado pode visualizar */
    $filtroListagemUO_UG = retornaFiltroSubacoesUoUg();

// -- Group by
    if ($params['groupby']) {
        if (is_array($params['groupby'])) {
            $params['groupby'] = implode(', ', $params['groupby']);
        }
        $groupby = "GROUP BY {$params['groupby']}";
    } else {
        $groupby = <<<GROUPBY
GROUP BY sbacod,
         codigo,
         sbatitulo,
         unidsc,
         dotacao,
         acoes,
         sbaid
GROUPBY;
    }
    if ($params['orderby']) {
        if (is_array($params['orderby'])) {
            $params['orderby'] = implode(', ', $params['orderby']);
        }
        $orderby = "ORDER BY {$params['orderby']}";
    } else {
        $orderby = "ORDER BY 2";
    }

    $sql = <<<SQL
    {$params['SELECT']}
  FROM (SELECT  sba.sbacod  as codigo,
               sba.sbacod,
               COALESCE(uni.unicod||' - '||uni.unidsc, ' - ') as unidsc,
               sba.sbatitulo,
               sba.sbaid,
                (SELECT COALESCE(SUM(sadvalor),0.00)
                FROM monitora.pi_subacaodotacao sd
                JOIN monitora.ptres pt on pt.ptrid=sd.ptrid
                JOIN monitora.acao aca on aca.acaid = pt.acaid
                WHERE aca.prgano = '{$_SESSION['exercicio']}' AND sd.sbaid = sba.sbaid
              $whereObrigatorio
                   COALESCE(dtpi.valorpi, 0.00) AS detalhado_pi,
               COALESCE(semp.total, 0.00) AS empenhado
          FROM monitora.pi_subacao sba
            LEFT JOIN monitora.pi_subacaodotacao sdt ON sdt.sbaid = sba.sbaid
            LEFT JOIN monitora.ptres ptr ON ptr.ptrid = sdt.ptrid
            LEFT JOIN (SELECT
                            sbaid,
                            SUM(pip.pipvalor) as valorpi
                        FROM
                            monitora.pi_planointerno pli
                        INNER JOIN
                            monitora.pi_planointernoptres pip
                        ON
                            pli.pliid = pip.pliid
                        WHERE
                            pli.pliano = '{$_SESSION['exercicio']}'
                        AND pli.plistatus = 'A'
                        AND pli.plisituacao IN ('A','C','S','T')
                        GROUP BY
                            sbaid) dtpi
              ON dtpi.sbaid = sba.sbaid
            LEFT JOIN siafi.sbaempenho semp on semp.sbacod = sba.sbacod AND semp.exercicio = sba.sbaano
            LEFT JOIN monitora.pi_subacaounidade psu ON    psu.sbaid = sba.sbaid
            LEFT JOIN public.unidade uni on psu.unicod = uni.unicod
          WHERE sba.sbastatus = 'A' {$where}
            AND sba.sbaano = '{$_SESSION['exercicio']}'
            {$whereUnicodNotNull}
            {$whereObrigatorio2}
            {$filtroListagemUO_UG}
            {$whereUO}
            {$whereUnidade}
          GROUP BY 2, 3, sba.sbaid, sba.sbacod, semp.total, sba.sbatitulo, dtpi.valorpi, uni.unicod, uni.unidsc
          ORDER BY sba.sbacod ) AS foo
  {$groupby}
  {$orderby}
SQL;
//ver($sql,d);
    return "$sql";
}

/**
 * Retorna a consulta para Subação modelo novo
 */
function retornaConsultaSubacao_bootstrap(array $params, $apenasObrigatorias = "") {
    /* Unidades Obrigatórias (AD, CAPES, INEP, FNDE, FIES, SUP.MEC, EBSERH */
    $obrigatorias = UNIDADES_OBRIGATORIAS;
    $perfis = pegaPerfilGeral();
    if($params['unicod']){
        $whereUnidade = "AND uni.unicod ='".$params['unicod']."'";
    }
    if ($apenasObrigatorias == 'n') {
        $whereObrigatorio = ' AND aca.unicod NOT IN(' . $obrigatorias . ')) AS dotacao,';
        $whereObrigatorio2 = 'AND (ptr.unicod NOT IN(' . $obrigatorias . ')  OR ptr.unicod IS NULL)';
        $whereUnicodNotNull = 'and psu.unicod is not null';

        if (in_array(PFL_GESTAO_ORCAMENTARIA_IFS, $perfis)) {
            $sqlUO = "
				EXISTS (SELECT 1
         		FROM planacomorc.usuarioresponsabilidade rpu
         		WHERE rpu.usucpf = '%s'
           		AND rpu.pflcod = %d
           		AND rpu.rpustatus = 'A'
           		AND rpu.unicod  = uni.unicod)
			";
            $wherePerfil[] = $whereUO = sprintf($sqlUO, $_SESSION['usucpf'], PFL_GESTAO_ORCAMENTARIA_IFS);
            $whereUO = " AND {$whereUO}";

        }
    } else {
        $whereObrigatorio = ' AND aca.unicod IN('.$obrigatorias.')) AS dotacao,';
        $whereObrigatorio2 = 'AND (ptr.unicod IN('.$obrigatorias.') OR ptr.unicod IS NULL) --AND psu.unicod IS NULL';
        if($params['caixaAzul'] == 'S'){
            $whereObrigatorio3 = "
                AND sba.sbaid IN (
                    SELECT
                        sbaid
                    FROM monitora.pi_subacaounidade
                    WHERE unicod IN(". UNIDADES_OBRIGATORIAS. ")
                        OR ungcod IN (
                            SELECT
                                ungcod
                            FROM public.unidadegestora
                            WHERE unicod IN(". UNIDADES_OBRIGATORIAS. ")
                        )
		)";
        }
    }
    /* Cabeçalho do SELECT */
    if (!$params['SELECT']) {
        $params['SELECT'] = "
            SELECT
                sbaid,
                codigo as cod,
                sbatitulo,
                unidsc,
                dotacao,
                SUM(CAST(detalhado_pi AS NUMERIC)) AS detalhado_pi,
                SUM(empenhado) AS empenhado,
                SUM(dotacao) - SUM(CAST(detalhado_pi AS NUMERIC)) AS saldo_nao_detalhado,
                SUM(dotacao) - SUM(empenhado) AS saldo_nao_empenhado,
                CASE
                    WHEN SUM(CAST(detalhado_pi AS NUMERIC)) > 0
                        THEN 0
                    ELSE 1
                END as delete
        ";
    }
    /* WHERE */
    if ($params['where']) {
        $where = $params['where'];
    }

    /* Filtro para listar apenas as subações que o usuário logado pode visualizar */
    $filtroListagemUO_UG = retornaFiltroSubacoesUoUg();

	// -- Group by
    if ($params['groupby']) {
        if (is_array($params['groupby'])) {
            $params['groupby'] = implode(', ', $params['groupby']);
        }
        $groupby = "GROUP BY {$params['groupby']}";
    } else {
        $groupby = "
            GROUP BY
            --sbacod,
            codigo,
            sbatitulo,
            unidsc,
            dotacao,
            --acoes,
            sbaid
        ";
    }
    if ($params['orderby']) {
        if (is_array($params['orderby'])) {
            $params['orderby'] = implode(', ', $params['orderby']);
        }
        $orderby = "ORDER BY {$params['orderby']}";
    } else {
        $orderby = "ORDER BY 2";
    }

//     '<img src="/imagens/alterar.gif" border="0" title="Alterar" style="cursor:pointer" '
//     || 'onclick="alterarSubacao(' || sba.sbaid || ');" />&nbsp;'::text AS acoes,
//     '<spam class="linkSubacao" onclick="detalheSubacao('|| sba.sbaid ||')">'|| sba.sbacod ||'</spam>' as codigo
    $sql = "
	    {$params['SELECT']}
	  	FROM
	  		(SELECT
				--sba.sbaid  as acoes,
	            --sba.sbacod,
	            sba.sbacod as codigo,
	            sba.sbacod as codigo2,

	            COALESCE(uni.unicod||' - '||uni.unidsc, ' - ') as unidsc,
				sba.sbatitulo,
	            sba.sbaid,
	            (SELECT COALESCE(SUM(sadvalor),0.00)
			FROM monitora.pi_subacaodotacao sd
			JOIN monitora.ptres pt on pt.ptrid=sd.ptrid
	        JOIN monitora.acao aca on aca.acaid = pt.acaid
	        WHERE
	        	aca.prgano = '{$_SESSION['exercicio']}'
	        AND sd.sbaid = sba.sbaid
	        	$whereObrigatorio
	        	COALESCE(dtpi.valorpi, 0.00) AS detalhado_pi,
	        	COALESCE(semp.total, 0.00) AS empenhado
	        FROM monitora.pi_subacao sba
			LEFT JOIN monitora.pi_subacaodotacao sdt ON sdt.sbaid = sba.sbaid
	        LEFT JOIN monitora.ptres ptr ON ptr.ptrid = sdt.ptrid
	        LEFT JOIN
	        	(SELECT
	            	sbaid,
	               	SUM(pip.pipvalor) as valorpi
				FROM
	               	monitora.pi_planointerno pli
	            INNER JOIN
	            	monitora.pi_planointernoptres pip
				ON
	            	pli.pliid = pip.pliid
	            WHERE
	            	pli.pliano = '{$_SESSION['exercicio']}'
				AND pli.plistatus = 'A'
	            AND pli.plisituacao IN ('A','C','S','T')
	            GROUP BY
	            sbaid) dtpi	ON dtpi.sbaid = sba.sbaid
			LEFT JOIN siafi.sbaempenho semp on semp.sbacod = sba.sbacod AND semp.exercicio = sba.sbaano
	        LEFT JOIN monitora.pi_subacaounidade psu ON    psu.sbaid = sba.sbaid
	        LEFT JOIN public.unidade uni on psu.unicod = uni.unicod
	        WHERE sba.sbastatus = 'A' {$where}
	        AND sba.sbaano = '{$_SESSION['exercicio']}'
	            {$whereUnicodNotNull}
	            {$whereObrigatorio2}
	            {$filtroListagemUO_UG}
	            {$whereUO}
	            {$whereUnidade}
                    {$whereObrigatorio3}
	        GROUP BY 2, 3, sba.sbaid, sba.sbacod, semp.total, sba.sbatitulo, dtpi.valorpi, uni.unicod, uni.unidsc
	        ORDER BY sba.sbacod ) AS foo
	  	{$groupby}
	  	{$orderby}
		";
    return "$sql";
}

/*
 * Retorna a consulta para PI
 */

function retornaConsultaPI($params) {
    /* Unidades Obrigatórias (AD, CAPES, INEP, FNDE, FIES, SUP.MEC, EBSERH */
    $obrigatorias = UNIDADES_OBRIGATORIAS;
    $perfis = pegaPerfilGeral();

    /* Variáveis da Consulta */
    $ptrid = $params['v_ptrid'];
    $sbaid = $params['v_sbaid'];

    /* por PTRES */
    $filtroNoUnion = $params['filtroNoUnion'];
    $filtroNoUnionSiafi = $params['filtroNoUnionSiafi'];

    /* Cabeçalho do SELECT */
    if (!$params['SELECT']) {
        $params['SELECT'] = <<<SQL
SELECT
        gmb.codigo,
        gmb.titulo,
        COALESCE(SUM(pip.pipvalor),0.00) as dotacao,
        gmb.empenhado,
        COALESCE(SUM(pip.pipvalor) - empenhado, 0.00) as saldo
SQL;
    }
    /* Filtros */
    if ($params['where']) {
        $where = $params['where'];
    }
    /* Pula se for SU */
    if ($_SESSION['superuser'] != 1) {
        /* Faz filtro para Gabinete */
        if (in_array(PFL_GABINETE, pegaPerfilGeral())) {
            $cpf = $_SESSION['usucpf'];
            $perfil = PFL_GABINETE;
            $sqlFiltro = <<<SQL
                AND sbaid IN (
                    SELECT
                        sbaid
                    FROM
                        monitora.pi_subacaounidade sbu
                    WHERE
                    sbu.ungcod IN
                        (
                            SELECT
                                ungcod
                            FROM
                                public.unidadegestora
                            WHERE
                                ungcod IN
                                           (
                                           SELECT DISTINCT
                                               ungcod
                                           FROM
                                               planacomorc.usuarioresponsabilidade usr
                                           WHERE
                                               usr.usucpf = '{$cpf}'
                                           AND usr.pflcod = {$perfil}  AND usr.rpustatus = 'A') )
                    )

SQL;
            $where .= $sqlFiltro;
        }

        /* FIM filtro para Gabinete */
    } /* FIM do pular se for SU */

    $filtroObrigatorias1 = "AND (pi.unicod IN ({$obrigatorias}) OR pi.ungcod IN (select ungcod from public.unidadegestora where unicod IN ($obrigatorias)) )";
    $filtroObrigatorias1 = "AND ptr.unicod IN ({$obrigatorias})";
    if (in_array(PFL_GESTAO_ORCAMENTARIA_IFS, $perfis)) {
        $filtroObrigatorias1 = "";
        $filtroObrigatorias2 = "";
    }
    $sql = <<<SQL
    {$params['SELECT']}
    FROM (
    SELECT
            descricao.pliid,
            descricao.codigo,
            descricao.titulo,
            COALESCE(SUM(descricao.empenhado), 0.00) as empenhado
    FROM (        -- nosimec
            SELECT
                    pi.pliid,
                    pi.plicod AS codigo,
                   CASE
                     WHEN trim(pi.plititulo) IS NOT NULL
                       THEN pi.plititulo
                     ELSE 'Não Preenchido'
                   END AS titulo,
                    ppe.total    AS empenhado
              FROM monitora.pi_planointerno pi
              LEFT JOIN monitora.pi_subacao sa ON sa.sbaid = pi.sbaid
              LEFT JOIN monitora.pi_planointernoptres pip ON pip.pliid = pi.pliid
              LEFT JOIN monitora.ptres ptr USING(ptrid)
              LEFT JOIN siafi.pliptrempenho ppe ON ppe.plicod = pi.plicod AND ppe.ptres = ptr.ptres AND ppe.exercicio = pliano
              WHERE pi.pliano = '{$_SESSION['exercicio']}'
              {$filtroObrigatorias1}
                    AND pi.plistatus = 'A'
                    {$filtroNoUnion}
            UNION
            -- nosiafi
            SELECT pi.pliid,
                   pi.plicod AS codigo,
                   CASE WHEN trim(pi.plititulo) IS NOT NULL THEN pi.plititulo ELSE 'Não Preenchido' END AS titulo,
                   COALESCE(ppe.total, 0.00) AS empenhado
                  FROM monitora.pi_planointerno pi
                  LEFT JOIN siafi.pliptrempenho ppe ON pi.plicod = ppe.plicod
                  LEFT JOIN monitora.ptres ptr ON ptr.ptres = ppe.ptres AND ppe.exercicio = '{$_SESSION['exercicio']}'
                  WHERE
                      pi.pliano = '{$_SESSION['exercicio']}'
                      {$filtroObrigatorias2}
                      AND pi.plistatus = 'A'
                      {$filtroNoUnionSiafi}
            ) as descricao
            GROUP BY descricao.codigo, descricao.titulo, descricao.pliid
    ) as gmb
LEFT JOIN monitora.pi_planointerno pli on pli.pliid = gmb.pliid
LEFT JOIN monitora.pi_planointernoptres pip ON pip.pliid = gmb.pliid
LEFT JOIN monitora.ptres ON pip.ptrid = ptres.ptrid
LEFT JOIN public.unidade uni ON uni.unicod = pli.unicod
LEFT JOIN public.unidadegestora ung ON ung.ungcod = pli.ungcod
WHERE pli.pliano = '{$_SESSION['exercicio']}'
{$where}
GROUP BY gmb.codigo, gmb.titulo, gmb.pliid, gmb.empenhado, uni.unicod, uni.unidsc, pli.plisituacao, ung.ungabrev, pli.obrid, pli.plicadsiafi
ORDER BY 2
SQL;
//ver($sql,d);
    return $sql;
}

function retornaConsultaPTRESInstituicoes($params, $modelo = true) {
    /* Unidades Obrigatórias (AD, CAPES, INEP, FNDE, FIES, SUP.MEC, EBSERH */
    $obrigatorias = UNIDADES_OBRIGATORIAS;
//ver($params,d);
    if ($params['obrigatorio'] == 'n') {
        $whereObrigatorio = 'AND aca.unicod NOT IN(' . $obrigatorias . ')';
        $perfis = pegaPerfilGeral();

        if (in_array(PFL_GESTAO_ORCAMENTARIA_IFS, $perfis)) {
            $sqlUO = <<<DML
EXISTS (SELECT 1
         FROM planacomorc.usuarioresponsabilidade rpu
         WHERE rpu.usucpf = '%s'
           AND rpu.pflcod = %d
           AND rpu.rpustatus = 'A'
           AND rpu.unicod  = uni.unicod)
DML;
            $wherePerfil[] = $whereUO = sprintf($sqlUO, $_SESSION['usucpf'], PFL_GESTAO_ORCAMENTARIA_IFS);
            $whereUO = " AND {$whereUO}";
        }
    } else {
        $whereObrigatorio = 'AND aca.unicod IN(' . $obrigatorias . ')';
        $whereUO = '';
    }
   // ver($params['obrigatorio'],d);

    /* Cabeçalho do SELECT */
    if (!$params['SELECT']) {
        if ($modelo == 'NOVO') {
            if ($params['obrigatorio'] == 'n') {
                $params['SELECT'] = <<<SQL
SELECT
                         dtl.ptrid as codigo,
                          dtl.ptres,
                         trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acatitulo) AS descricao,
                         uni.unicod || ' - ' || uni.unidsc as unidade,
                         COALESCE(SUM(dtl.ptrdotacao)+0.00, 0.00) AS dotacaoinicial,
                         COALESCE(SUM(dt.valor), 0.00) AS det_subacao,
                         COALESCE(SUM(dt2.valorpi), 0.00) AS det_pi,
                         COALESCE((pemp.total), 0.00) AS empenhado,
                         COALESCE(SUM(dtl.ptrdotacao) - COALESCE(SUM(dt.valor), 0.00), 0.00) AS saldo
SQL;
            } else {
                $params['SELECT'] = <<<SQL
SELECT
                         dtl.ptrid as codigo,
                          dtl.ptres,
                         trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acatitulo) AS descricao,
                         uni.unicod || ' - ' || uni.unidsc as unidade,
                         COALESCE(SUM(dtl.ptrdotacao)+0.00, 0.00) AS dotacaoinicial,
                         COALESCE(SUM(dt.valor), 0.00) AS det_subacao,
                         COALESCE((pemp.total), 0.00) AS empenhado,
                         COALESCE(SUM(dtl.ptrdotacao) - COALESCE(SUM(dt.valor), 0.00), 0.00) AS saldo
SQL;
            }
        } else {
            if ($params['obrigatorio'] == 'n') {
                            $params['SELECT'] = <<<SQL
SELECT
                          dtl.ptres as sbacod,
                         trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acatitulo) AS descricao,
                        uni.unicod || ' - ' || uni.unidsc as unidade,
                         COALESCE(SUM(dtl.ptrdotacao)+0.00, 0.00) AS dotacaoinicial,
                         COALESCE(SUM(dt.valor), 0.00) AS det_subacao,
                         COALESCE((pemp.total), 0.00) AS empenhado,
                         COALESCE(SUM(dtl.ptrdotacao) - COALESCE(SUM(dt.valor), 0.00), 0.00) AS saldo
SQL;
            }else{
                            $params['SELECT'] = <<<SQL
SELECT
                         '<div class=\"linkSubacao\" onclick=\"detalhePtres(\''|| dtl.ptrid ||'\');\">'|| dtl.ptres ||'</div>'as sbacod,
                         trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acatitulo) AS descricao,
                         uni.unicod || ' - ' || uni.unidsc as unidade,
                         COALESCE(SUM(dtl.ptrdotacao)+0.00, 0.00) AS dotacaoinicial,
                         COALESCE(SUM(dt.valor), 0.00) AS det_subacao,
                         COALESCE(SUM(dt2.valorpi), 0.00) AS det_pi,
                         COALESCE((pemp.total), 0.00) AS empenhado,
                         COALESCE(SUM(dtl.ptrdotacao) - COALESCE(SUM(dt.valor), 0.00), 0.00) AS saldo
SQL;
            }
        }
    }
    /* Filtros */
    if ($params['where']) {
        $where = $params['where'];
    }
    if ($params['obrigatorio'] == 'n') {
        $sql = <<<SQL
    {$params['SELECT']}
         FROM monitora.ptres ptr
   INNER JOIN monitora.acao aca on ptr.acaid = aca.acaid
   INNER JOIN public.unidade uni on aca.unicod = uni.unicod
   LEFT JOIN monitora.pi_planointernoptres pip on ptr.ptrid = pip.ptrid
   LEFT JOIN (SELECT ptrid, SUM(sadvalor) AS valor
                FROM monitora.pi_subacaodotacao
                GROUP BY ptrid) dts ON dts.ptrid = ptr.ptrid
   LEFT JOIN (SELECT pip.ptrid, SUM(pipvalor) AS valor
               FROM
                    monitora.pi_planointernoptres pip
                    join monitora.pi_planointerno pli using (pliid)
                WHERE
                   plistatus  = 'A'
                GROUP BY ptrid) dtp ON dtp.ptrid = ptr.ptrid
   LEFT JOIN siafi.uo_ptrempenho pemp ON (pemp.ptres = ptr.ptres AND pemp.exercicio = '2014' AND pemp.unicod = ptr.unicod)
                    WHERE 1=1
AND aca.acasnrap = FALSE
AND aca.prgano='{$_SESSION['exercicio']}'
{$where}
{$whereObrigatorio}
{$whereUO}
AND aca.prgano='{$_SESSION['exercicio']}'
AND ptr.ptrstatus = 'A'
AND aca.unicod NOT IN('26101','26291', '26290', '26298', '26443', '74902', '73107')
GROUP BY ptr.ptrid, ptr.ptres,aca.prgcod, aca.acacod,aca.unicod,aca.loccod,aca.acatitulo,uni.unidsc, ptr.ptrdotacao, pemp.total
ORDER BY ptr.ptres
SQL;

//   }else{
//        $sql = <<<SQL
//    {$params['SELECT']}
//   FROM monitora.acao aca
//                      INNER JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
//                      INNER JOIN public.unidade uni ON uni.unicod = dtl.unicod
//                      LEFT JOIN (SELECT ptrid,
//                                        SUM(sadvalor) AS valor
//                                   FROM monitora.pi_subacaodotacao
//                                   GROUP BY ptrid) dt ON dtl.ptrid = dt.ptrid
//                      LEFT JOIN (SELECT ptrid,
//                                        SUM(dtl.valorpi) AS valorpi
//                                   FROM monitora.v_pi_detalhepiptres dtl
//                                   GROUP BY dtl.ptrid) dt2 ON dtl.ptrid = dt2.ptrid
//                      LEFT JOIN siafi.ptrempenho pemp
//                        ON (pemp.ptres = dtl.ptres AND pemp.exercicio = '{$_SESSION['exercicio']}')
//                      LEFT JOIN (SELECT pliid,
//                                        ptrid,
//                                        SUM(pipvalor) AS valor
//                                   FROM monitora.pi_planointernoptres
//                                   GROUP BY pliid, ptrid) pli ON pli.ptrid = dt.ptrid
//                    WHERE aca.prgano='{$_SESSION['exercicio']}'
//                        AND dtl.ptrano='{$_SESSION['exercicio']}'
//                        {$where}
//                      AND ptrstatus = 'A'
//                      AND aca.acasnrap = FALSE
//                      {$whereObrigatorio}
//                      {$whereUO}
//                GROUP BY dtl.ptrid,dtl.ptres,descricao,uni.unidsc, pemp.total,  uni.unicod ORDER BY 1
//SQL;
//    }

//
//ver($sql,d);
    return $sql;
}
}



/*
 * Retorna a consulta para PI
 */

function retornaConsultaPTRES($params, $modelo = true) {
    /* Unidades Obrigatórias (AD, CAPES, INEP, FNDE, FIES, SUP.MEC, EBSERH */
    $obrigatorias = UNIDADES_OBRIGATORIAS;

    if ($params['obrigatorio'] == 'n') {
        $whereObrigatorio = 'AND aca.unicod NOT IN(' . $obrigatorias . ')';
        $perfis = pegaPerfilGeral();

        if (in_array(PFL_GESTAO_ORCAMENTARIA_IFS, $perfis)) {
            $sqlUO = <<<DML
EXISTS (SELECT 1
         FROM planacomorc.usuarioresponsabilidade rpu
         WHERE rpu.usucpf = '%s'
           AND rpu.pflcod = %d
           AND rpu.rpustatus = 'A'
           AND rpu.unicod  = uni.unicod)
DML;
            $wherePerfil[] = $whereUO = sprintf($sqlUO, $_SESSION['usucpf'], PFL_GESTAO_ORCAMENTARIA_IFS);
            $whereUO = " AND {$whereUO}";
        }
    } else {
        $whereObrigatorio = 'AND aca.unicod IN(' . $obrigatorias . ')';
        $whereUO = '';
    }
   // ver($params['obrigatorio'],d);

    /* Cabeçalho do SELECT */
    if (!$params['SELECT']) {
        if ($modelo == 'NOVO') {
            if ($params['obrigatorio'] == 'n') {
                $params['SELECT'] = <<<SQL
SELECT
                         dtl.ptrid as codigo,
                          dtl.ptres,
                         trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acatitulo) AS descricao,
                         uni.unicod || ' - ' || uni.unidsc as unidade,
                         COALESCE(SUM(dtl.ptrdotacao)+0.00, 0.00) AS dotacaoinicial,
                         COALESCE(SUM(dt.valor), 0.00) AS det_subacao,
                         COALESCE(SUM(dt2.valorpi), 0.00) AS det_pi,
                         COALESCE((pemp.total), 0.00) AS empenhado,
                         COALESCE(SUM(dtl.ptrdotacao) - COALESCE(SUM(dt.valor), 0.00), 0.00) AS saldo
SQL;
            } else {
                $params['SELECT'] = <<<SQL
SELECT
                         dtl.ptrid as codigo,
                          dtl.ptres,
                         trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acatitulo) AS descricao,
                         uni.unicod || ' - ' || uni.unidsc as unidade,
                         COALESCE(SUM(dtl.ptrdotacao)+0.00, 0.00) AS dotacaoinicial,
                         COALESCE(SUM(dt.valor), 0.00) AS det_subacao,
                         COALESCE((pemp.total), 0.00) AS empenhado,
                         COALESCE(SUM(dtl.ptrdotacao) - COALESCE(SUM(dt.valor), 0.00), 0.00) AS saldo
SQL;
            }
        } else {
            if ($params['obrigatorio'] == 'n') {
                            $params['SELECT'] = <<<SQL
SELECT
                          dtl.ptres as sbacod,
                         trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acatitulo) AS descricao,
                        uni.unicod || ' - ' || uni.unidsc as unidade,
                         COALESCE(SUM(dtl.ptrdotacao)+0.00, 0.00) AS dotacaoinicial,
                         COALESCE(SUM(dt.valor), 0.00) AS det_subacao,
                         COALESCE((pemp.total), 0.00) AS empenhado,
                         COALESCE(SUM(dtl.ptrdotacao) - COALESCE(SUM(dt.valor), 0.00), 0.00) AS saldo
SQL;
            }else{
                            $params['SELECT'] = <<<SQL
SELECT
                         '<div class=\"linkSubacao\" onclick=\"detalhePtres(\''|| dtl.ptrid ||'\');\">'|| dtl.ptres ||'</div>'as sbacod,
                         trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acatitulo) AS descricao,
                         uni.unicod || ' - ' || uni.unidsc as unidade,
                         COALESCE(SUM(dtl.ptrdotacao)+0.00, 0.00) AS dotacaoinicial,
                         COALESCE(SUM(dt.valor), 0.00) AS det_subacao,
                         COALESCE(SUM(dt2.valorpi), 0.00) AS det_pi,
                         COALESCE((pemp.total), 0.00) AS empenhado,
                         COALESCE(SUM(dtl.ptrdotacao) - COALESCE(SUM(dt.valor), 0.00), 0.00) AS saldo
SQL;
            }
        }
    }
    /* Filtros */
    if ($params['where']) {
        $where = $params['where'];
    }
    if ($params['obrigatorio'] == 'n') {
        $sql = <<<SQL
    {$params['SELECT']}
   FROM monitora.acao aca
                      INNER JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
                      INNER JOIN public.unidade uni ON uni.unicod = dtl.unicod
                      LEFT JOIN (SELECT ptrid,
                                        SUM(sadvalor) AS valor
                                   FROM monitora.pi_subacaodotacao
                                   GROUP BY ptrid) dt ON dtl.ptrid = dt.ptrid
                      LEFT JOIN (SELECT ptrid,
                                        SUM(dtl.valorpi) AS valorpi
                                   FROM monitora.v_pi_detalhepiptres dtl
                                   WHERE prgano = '{$_SESSION['exercicio']}'
                                   GROUP BY dtl.ptrid) dt2 ON dtl.ptrid = dt2.ptrid
                       LEFT JOIN siafi.uo_ptrempenho pemp
                        ON (pemp.ptres = dtl.ptres AND pemp.exercicio = '{$_SESSION['exercicio']}' AND pemp.unicod = dtl.unicod)
                      LEFT JOIN (SELECT pliid,
                                        ptrid,
                                        SUM(pipvalor) AS valor
                                   FROM monitora.pi_planointernoptres
                                   GROUP BY pliid, ptrid) pli ON pli.ptrid = dt.ptrid
                    WHERE aca.prgano='{$_SESSION['exercicio']}'
                        AND dtl.ptrano='{$_SESSION['exercicio']}'
                        {$where}
                      AND ptrstatus = 'A'
                      AND aca.acasnrap = FALSE
                      {$whereObrigatorio}
                      {$whereUO}
                GROUP BY dtl.ptrid,dtl.ptres,descricao,uni.unidsc, pemp.total,  uni.unicod ORDER BY 1
SQL;
    }else{
        $sql = <<<SQL
    {$params['SELECT']}
   FROM monitora.acao aca
                      INNER JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
                      INNER JOIN public.unidade uni ON uni.unicod = dtl.unicod
                      LEFT JOIN (SELECT ptrid,
                                        SUM(sadvalor) AS valor
                                   FROM monitora.pi_subacaodotacao
                                   GROUP BY ptrid) dt ON dtl.ptrid = dt.ptrid
                      LEFT JOIN (SELECT ptrid,
                                        SUM(dtl.valorpi) AS valorpi
                                   FROM monitora.v_pi_detalhepiptres dtl
                                   WHERE prgano = '{$_SESSION['exercicio']}'
                                   GROUP BY dtl.ptrid) dt2 ON dtl.ptrid = dt2.ptrid
                      LEFT JOIN siafi.ptrempenho pemp
                        ON (pemp.ptres = dtl.ptres AND pemp.exercicio = '{$_SESSION['exercicio']}')
                      LEFT JOIN (SELECT pliid,
                                        ptrid,
                                        SUM(pipvalor) AS valor
                                   FROM monitora.pi_planointernoptres
                                   GROUP BY pliid, ptrid) pli ON pli.ptrid = dt.ptrid
                    WHERE aca.prgano='{$_SESSION['exercicio']}'
                        AND dtl.ptrano='{$_SESSION['exercicio']}'
                        {$where}
                      AND ptrstatus = 'A'
                      AND aca.acasnrap = FALSE
                      {$whereObrigatorio}
                      {$whereUO}
                GROUP BY dtl.ptrid,dtl.ptres,descricao,uni.unidsc, pemp.total,  uni.unicod ORDER BY 1
SQL;
    }

//
//ver($sql,d);
    return $sql;
}



/**
 * Função utilizada para identificar perfis que pulam a solicitação, seja ela
 * de remanejamento (pi, subação) ou cadastro (pi). Geralmente, ao pular a
 * solicitação, são feitas algumas verificações adicionais para finalizar a
 * transação. Isso se aplica apenas aos perfis GO e CPMO.
 * @see remaneja.inc
 * @see cadastro.inc
 */
function pulaSolicitacao() {
    $arPermitidos = array(PFL_CPMO, PFL_GESTAO_ORCAMENTARIA);
    return (1 == $_SESSION['superuser']) || array_intersect($arPermitidos, pegaPerfilGeral());
}

function pegaUOsResponsabilidade($cpf, $perfil) {
    global $db;

    $sql = <<<SQL
SELECT urb.unicod
  FROM planacomorc.usuarioresponsabilidade urb
  WHERE urb.pflcod = %d
    AND urb.usucpf = '%s'
    AND urb.rpustatus = 'A'
SQL;
    $stmt = sprintf($sql, $perfil, $cpf);
    if ($result = $db->carregar($stmt)) {
        foreach ($result as &$unicod) {
            $unicod = $unicod['unicod'];
        }
        return $result;
    }

    return array();
}

/**
 * Buscar a funcional do Plano Interno.
 * 
 * @global cls_banco $db
 * @param stdClass $filtros
 * @todo Melhorar performance da consulta pois a mesma já sofreu várias manutenções devido a várias mudanças do sistema e precisa ser refatorada.
 * @return array
 */
function buscarPTRES(stdClass $filtros) {
    global $db;

    # Filtros.
    $where .= $filtros->pliid? " AND pip.pliid = $filtros->pliid ": NULL;
    $where .= $filtros->ptrid? " AND ptr.ptrid = $filtros->ptrid ": NULL;
    
    # Configuração da consulta pra atender a funcionalidade de Importacao do SIMINC1.
    $colunaPipValor = $filtros->importar? "0 AS pipvalor": "pip.pipvalor AS pipvalor";
    $colunaAgrupadaPipValor = $filtros->importar? NULL: "pip.pipvalor,";

    $sql = <<<SQL
        SELECT
            ptr.ptrid,
            ptr.ptres,
            TRIM(aca.prgcod) || '.' || TRIM(aca.acacod) || '.' || TRIM(aca.loccod) || '.' || (CASE WHEN LENGTH(TRIM(aca.acaobjetivocod)) <= 0 THEN '-' ELSE TRIM(coalesce(aca.acaobjetivocod, '')) END) || '.' || TRIM(ptr.plocod) || ' - ' || aca.acatitulo || CASE WHEN LENGTH(TRIM(ptr.plodsc)) >= 0 THEN ': ' || ptr.plodsc ELSE '' END || ' (RP ' || COALESCE(ptr.irpcod, '') || ')' AS descricao,
            aca.unicod || ' - ' || uni.unonome as unidsc,
            (COALESCE(psu.ptrdotacaocusteio, 0.00) + COALESCE(psu.ptrdotacaocapital, 0.00)) AS dotacaoatual,
            COALESCE(psu.ptrdotacaocusteio, 0.00) AS ptrdotacaocusteio,
            COALESCE(psu.ptrdotacaocapital, 0.00) AS ptrdotacaocapital,
            COALESCE(SUM(dtp.valor), 0.00) AS det_pi,
	    COALESCE(SUM(dtp.custeio), 0.00) AS det_pi_custeio,
	    COALESCE(SUM(dtp.capital), 0.00) AS det_pi_capital,
            ((COALESCE(psu.ptrdotacaocusteio, 0.00) + COALESCE(psu.ptrdotacaocapital, 0.00)) - COALESCE(SUM(dtp.valor), 0.00)) AS nao_det_pi,
            (COALESCE(psu.ptrdotacaocusteio, 0.00) - COALESCE(SUM(dtp.custeio), 0.00)) AS nao_det_pi_custeio,
            (COALESCE(psu.ptrdotacaocapital, 0.00) - COALESCE(SUM(dtp.capital), 0.00)) AS nao_det_pi_capital,
            COALESCE((pemp.total), 0.00) AS empenhado,
            (COALESCE(psu.ptrdotacaocusteio, 0.00) + COALESCE(psu.ptrdotacaocapital, 0.00)) - COALESCE(pemp.total, 0.00) AS nao_empenhado,
            $colunaPipValor
        FROM monitora.ptres ptr
	    JOIN monitora.pi_planointernoptres pip ON(ptr.ptrid = pip.ptrid)
	    JOIN monitora.pi_planointerno pi ON(pip.pliid = pi.pliid)
            JOIN monitora.acao aca ON(ptr.acaid = aca.acaid)
            JOIN public.vw_subunidadeorcamentaria uni ON(aca.unicod = uni.unocod AND uni.suocod = pi.ungcod AND uni.prsano = aca.prgano) -- SELECT * FROM public.vw_subunidadeorcamentaria
            LEFT JOIN spo.ptressubunidade psu ON(ptr.ptrid = psu.ptrid AND uni.suoid = psu.suoid)
            LEFT JOIN (
                SELECT
                    pip.ptrid,
                    pli.ungcod,
                    SUM(COALESCE(picvalorcusteio, 0.00) + COALESCE(picvalorcapital, 0.00)) AS valor,
                    SUM(COALESCE(picvalorcusteio, 0.00)) AS custeio,
                    SUM(COALESCE(picvalorcapital, 0.00)) AS capital
                FROM monitora.pi_planointernoptres pip
                    JOIN monitora.pi_planointerno pli USING(pliid)
                    JOIN planacomorc.pi_complemento pc USING(pliid)
                WHERE
                    plistatus = 'A'
                    AND pliano = '{$filtros->exercicio}'
                GROUP BY
                    ptrid,
                    pli.ungcod
            ) dtp ON(dtp.ptrid = ptr.ptrid AND uni.suocod = dtp.ungcod)
            LEFT JOIN (
                SELECT
                    ex.unicod,
                    pi.ungcod,
                    ex.ptres,
                    ex.exercicio,
                    sum(ex.vlrempenhado) AS total
                FROM spo.siopexecucao ex
                    JOIN monitora.pi_planointerno pi ON(ex.plicod = pi.plicod AND ex.exercicio = pi.pliano)
                WHERE
                    ex.exercicio = '{$filtros->exercicio}'
                GROUP BY
                    ex.unicod,
                    pi.ungcod,
                    ex.ptres,
                    ex.exercicio
                ) pemp ON(pemp.ptres = ptr.ptres AND pemp.exercicio = ptr.ptrano AND pemp.unicod = ptr.unicod AND uni.suocod = pemp.ungcod)
        WHERE  aca.prgano = '{$filtros->exercicio}'
            AND ptr.ptrstatus = 'A'
            $where
        GROUP BY
            ptr.ptrid,
            ptr.ptres,
            psu.ptrdotacaocusteio,
            psu.ptrdotacaocapital,
            aca.prgcod,
            aca.acaobjetivocod,
            aca.acacod,
            aca.unicod,
            aca.loccod,
            ptr.plocod,
            aca.acatitulo,
            uni.unonome,
            $colunaAgrupadaPipValor
            pemp.total
        ORDER BY
            ptr.ptres
SQL;

//ver($sql, d);
    $result = is_array($result) ? $result : Array();
//ver($sql,d);
    $result = $db->carregar($sql);
    if(is_array($result)) {
        $result = formatarValoresFuncional($result);
    }
    return $result;
}

/**
 * Buscar a funcional do Plano Interno FNC.
 * 
 * @global cls_banco $db
 * @param stdClass $filtros
 * @todo Melhorar performance da consulta pois a mesma já sofreu várias manutenções devido a várias mudanças do sistema e precisa ser refatorada.
 * @return array
 */
function buscarPtresFnc(stdClass $filtros) {
    global $db;

    # Filtros.
    $where .= $filtros->pliid? " AND pip.pliid = $filtros->pliid ": NULL;
    $where .= $filtros->ptrid? " AND ptr.ptrid = $filtros->ptrid ": NULL;
    
    # Configuração da consulta pra atender a funcionalidade de Importacao do SIMINC1.
    $colunaPipValor = $filtros->importar? "0 AS pipvalor": "pip.pipvalor AS pipvalor";
    $colunaAgrupadaPipValor = $filtros->importar? NULL: "pip.pipvalor,";

    $sql = "
        SELECT
            ptr.ptrid,
            ptr.ptres,
            TRIM(aca.prgcod)
            || '.' || TRIM(aca.acacod)
            || '.' || TRIM(aca.loccod)
            || '.' || (CASE WHEN LENGTH(TRIM(aca.acaobjetivocod)) <= 0 OR aca.acaobjetivocod IS NULL THEN '-' ELSE TRIM(aca.acaobjetivocod) END)
            || '.' || TRIM(ptr.plocod)
            || ' - ' || aca.acatitulo
            || CASE WHEN LENGTH(TRIM(ptr.plodsc)) >= 0 THEN ': ' || ptr.plodsc ELSE '' END
            || ' (RP ' || COALESCE(ptr.irpcod, '') || ')'
            AS descricao,
            aca.unicod || ' - ' || uni.unonome as unidsc,
            (COALESCE(ptr.ptrdotacaocusteio, 0.00) + COALESCE(ptr.ptrdotacaocapital, 0.00)) AS dotacaoatual,
            COALESCE(cadastrado.custeio, 0.00) AS cadastradocusteio,
            COALESCE(cadastrado.capital, 0.00) AS cadastradocapital,
            COALESCE(ptr.ptrdotacaocusteio, 0.00) AS ptrdotacaocusteio,
            COALESCE(ptr.ptrdotacaocapital, 0.00) AS ptrdotacaocapital,
            COALESCE(SUM(dtp.valor), 0.00) AS det_pi,
	    COALESCE(SUM(dtp.custeio), 0.00) AS det_pi_custeio,
	    COALESCE(SUM(dtp.capital), 0.00) AS det_pi_capital,
            ((COALESCE(ptr.ptrdotacaocusteio, 0.00) + COALESCE(ptr.ptrdotacaocapital, 0.00)) - COALESCE(SUM(dtp.valor), 0.00)) AS nao_det_pi,
            (COALESCE(ptr.ptrdotacaocusteio, 0.00) - COALESCE(SUM(dtp.custeio), 0.00)) AS nao_det_pi_custeio,
            (COALESCE(ptr.ptrdotacaocapital, 0.00) - COALESCE(SUM(dtp.capital), 0.00)) AS nao_det_pi_capital,
            COALESCE((pemp.total), 0.00) AS empenhado,
            (COALESCE(ptr.ptrdotacaocusteio, 0.00) + COALESCE(ptr.ptrdotacaocapital, 0.00)) - COALESCE(pemp.total, 0.00) AS nao_empenhado,
            $colunaPipValor
        FROM monitora.ptres ptr
	    JOIN monitora.pi_planointernoptres pip ON(ptr.ptrid = pip.ptrid)
	    JOIN monitora.pi_planointerno pi ON(pip.pliid = pi.pliid)
            JOIN monitora.acao aca ON(ptr.acaid = aca.acaid)
            JOIN public.vw_subunidadeorcamentaria uni ON(aca.unicod = uni.unocod AND uni.suocod = pi.ungcod AND uni.prsano = aca.prgano) -- SELECT * FROM public.vw_subunidadeorcamentaria
            JOIN spo.ptressubunidade psu ON(ptr.ptrid = psu.ptrid AND uni.suoid = psu.suoid)
            LEFT JOIN (
                SELECT
                    pip.ptrid,
                    pli.ungcod,
                    SUM(COALESCE(picvalorcusteio, 0.00) + COALESCE(picvalorcapital, 0.00)) AS valor,
                    SUM(COALESCE(picvalorcusteio, 0.00)) AS custeio,
                    SUM(COALESCE(picvalorcapital, 0.00)) AS capital
                FROM monitora.pi_planointernoptres pip
                    JOIN monitora.pi_planointerno pli USING(pliid)
                    JOIN planacomorc.pi_complemento pc USING(pliid)
                    JOIN workflow.documento wd ON(pli.docid = wd.docid)
                    JOIN workflow.estadodocumento ed ON(wd.esdid = ed.esdid)
                WHERE
                    pli.plistatus = 'A'
                    AND pli.pliano = '{$filtros->exercicio}'
                    AND ed.esdid = ". ESD_FNC_PI_APROVADO. "
                GROUP BY
                    ptrid,
                    pli.ungcod
            ) dtp ON(dtp.ptrid = ptr.ptrid AND uni.suocod = dtp.ungcod)
            LEFT JOIN (
                SELECT
                    pip2.ptrid,
                    pli.ungcod,
                    (SUM(COALESCE(pc.picvalorcusteio, 0.00)) + SUM(COALESCE(pc.picvalorcapital, 0.00))) AS total,
                    SUM(COALESCE(pc.picvalorcusteio, 0.00)) AS custeio,
                    SUM(COALESCE(pc.picvalorcapital, 0.00)) AS capital
                FROM monitora.pi_planointernoptres pip2
                    JOIN monitora.pi_planointerno pli USING(pliid)
                    JOIN planacomorc.pi_complemento pc USING(pliid)
                WHERE
                    pli.plistatus = 'A'
                    AND pli.pliano = '{$filtros->exercicio}'
                GROUP BY
                    pip2.ptrid,
                    pli.ungcod
            ) cadastrado ON(ptr.ptrid = cadastrado.ptrid AND pi.ungcod = cadastrado.ungcod)
            LEFT JOIN (
                SELECT
                    ex.unicod,
                    pi.ungcod,
                    ex.ptres,
                    ex.exercicio,
                    sum(ex.vlrempenhado) AS total
                FROM spo.siopexecucao ex
                    JOIN monitora.pi_planointerno pi ON(ex.plicod = pi.plicod AND ex.exercicio = pi.pliano)
                WHERE
                    ex.exercicio = '{$filtros->exercicio}'
                GROUP BY
                    ex.unicod,
                    pi.ungcod,
                    ex.ptres,
                    ex.exercicio
            ) pemp ON(pemp.ptres = ptr.ptres AND pemp.exercicio = ptr.ptrano AND pemp.unicod = ptr.unicod AND uni.suocod = pemp.ungcod)
        WHERE
            ptr.ptrstatus = 'A'
            AND aca.prgano = '{$filtros->exercicio}'
            $where
        GROUP BY
            ptr.ptrid,
            ptr.ptres,
            ptr.ptrdotacaocusteio,
            ptr.ptrdotacaocapital,
            cadastradocusteio,
            cadastradocapital,
            aca.prgcod,
            aca.acaobjetivocod,
            aca.acacod,
            aca.unicod,
            aca.loccod,
            ptr.plocod,
            aca.acatitulo,
            uni.unonome,
            $colunaAgrupadaPipValor
            pemp.total
        ORDER BY
            ptr.ptres
    ";

//ver($sql,d);
    $result = $db->carregar($sql);
    if(is_array($result)) {
        $result = formatarValoresFuncional($result);
    }
    return $result;
}

/**
 * Formata os principais dados monetarios da consulta de funcionais.
 * 
 * @param array $result
 */
function formatarValoresFuncional($result){
    foreach($result as $key => $_){
        $result[$key]['dotacaoatual'] = mascaraMoeda($result[$key]['dotacaoatual'], false);
        $result[$key]['det_pi'] = mascaraMoeda($result[$key]['det_pi'], false);
        $result[$key]['det_pi_custeio'] = mascaraMoeda($result[$key]['det_pi_custeio'], false);
        $result[$key]['det_pi_capital'] = mascaraMoeda($result[$key]['det_pi_capital'], false);
        $result[$key]['ptrdotacaocusteio'] = mascaraMoeda($result[$key]['ptrdotacaocusteio'], false);
        $result[$key]['ptrdotacaocapital'] = mascaraMoeda($result[$key]['ptrdotacaocapital'], false);
        $result[$key]['cadastradocusteio'] = mascaraMoeda($result[$key]['cadastradocusteio'], false);
        $result[$key]['cadastradocapital'] = mascaraMoeda($result[$key]['cadastradocapital'], false);
        $result[$key]['nao_det_pi'] = number_format($result[$key]['nao_det_pi'], 2, ',', '.');
        $result[$key]['nao_det_pi_custeio'] = mascaraMoeda($result[$key]['nao_det_pi_custeio'], false);
        $result[$key]['nao_det_pi_capital'] = mascaraMoeda($result[$key]['nao_det_pi_capital'], false);
        $result[$key]['empenhado'] = mascaraMoeda($result[$key]['empenhado'], false);
        $result[$key]['nao_empenhado'] = mascaraMoeda($result[$key]['nao_empenhado'], false);
        # Não formatado - para soma na interface
        $result[$key]['pipvalor_'] = $result[$key]['pipvalor'];
        $result[$key]['pipvalor'] = number_format($result[$key]['pipvalor'], 2, ',', '.');
    }

    return $result;
}

function buscarPTRESdoPIInstituicoes($pliid, $sbaid, $ptrid) {
    global $db;

    #  ver($sbaid);
    /* Para o caso de um PI sem Subação */
    $campoNaoDetPi = " (COALESCE(ptr.ptrdotacao, 0.00) - COALESCE(SUM(dtp.valor), 0.00)) AS nao_det_pi, ";
    if ($sbaid && $sbaid != 'null' && $sbaid != '' && $sbaid != '0') {
        $filtroSubacao = " AND sbaid = {$sbaid}";
        $campoNaoDetPi = " (COALESCE(SUM(dts.valor), 0.00) - COALESCE(SUM(dtp.valor), 0.00)) AS nao_det_pi, ";
    }

    /* Filtros */
    $where .= $sbaid ? " AND dtp.ptrid IN (SELECT ptrid FROM monitora.pi_subacaodotacao WHERE sbaid = '" . $sbaid . "')" : '';
    $where .= $pliid ? " AND pip.pliid = $pliid " : "";
    $where .= $ptrid ? " AND ptr.ptrid = $ptrid " : "";

    $valorSelect = $pliid ? "(SELECT pipvalor FROM monitora.pi_planointernoptres JOIN monitora.pi_planointerno USING(pliid) WHERE ptrid = ptr.ptrid AND pliid = {$pliid} {$filtroSubacao} ) as pipvalor" : '0 as valor';

    $sql = <<<SQL
        SELECT
            ptr.ptrid,
            ptr.ptres,
            TRIM(aca.prgcod) || '.' || TRIM(aca.acacod) || '.' || TRIM(aca.loccod) || '.' || (CASE WHEN LENGTH(TRIM(aca.acaobjetivocod)) <= 0 THEN '-' ELSE TRIM(aca.acaobjetivocod) END) || '.' || TRIM(ptr.plocod) || ' - ' || aca.acatitulo AS descricao,
            aca.unicod || ' - ' || uni.unonome as unidsc,
            COALESCE(ptr.ptrdotacao, 0.00) AS dotacaoatual,
            COALESCE(SUM(dts.valor), 0.00) AS det_subacao,
            (COALESCE(SUM(ptr.ptrdotacao), 0.00) - COALESCE(SUM(dts.valor), 0.00)) AS nao_det_subacao,
            COALESCE(SUM(dtp.valor), 0.00) AS det_pi,
            {$campoNaoDetPi}
            COALESCE((pemp.total), 0.00) AS empenhado,
            COALESCE(SUM(ptr.ptrdotacao), 0.00) - COALESCE(pemp.total, 0.00) AS nao_empenhado,
            $valorSelect
        FROM monitora.ptres ptr
            JOIN monitora.acao aca on ptr.acaid = aca.acaid
            JOIN public.unidadeorcamentaria uni on aca.unicod = uni.unocod AND uni.prsano = aca.prgano
            LEFT JOIN monitora.pi_planointernoptres pip on ptr.ptrid = pip.ptrid
            LEFT JOIN (
                SELECT
                    ptrid,
                    SUM(sadvalor) AS valor
                FROM monitora.pi_subacaodotacao
                WHERE
                    1=1
                    {$filtroSubacao}
                GROUP BY ptrid) dts ON dts.ptrid = ptr.ptrid
            LEFT JOIN (
                SELECT
                    pip.ptrid,
                    SUM(pipvalor) AS valor
                FROM monitora.pi_planointernoptres pip
                    JOIN monitora.pi_planointerno pli USING(pliid)
                WHERE
                    plistatus  = 'A'
                    {$filtroSubacao}
                GROUP BY
                    ptrid) dtp ON dtp.ptrid = ptr.ptrid
            LEFT JOIN siafi.uo_ptrempenho pemp ON(pemp.ptres = ptr.ptres AND pemp.exercicio = ptr.ptrano AND pemp.unicod = ptr.unicod)
        WHERE
            aca.acasnrap = FALSE
            AND aca.prgano = '{$_SESSION['exercicio']}'
            AND ptr.ptrstatus = 'A'
            $where
        GROUP BY
            ptr.ptrid,
            ptr.ptres,
            aca.prgcod,
            aca.acaobjetivocod,
            aca.acacod,
            aca.unicod,
            aca.loccod,
            aca.acatitulo,
            uni.unonome,
            ptr.ptrdotacao,
            pemp.total
        ORDER BY
            ptr.ptres
SQL;
//    $result = is_array($result) ? $result : Array();
//ver($sql,d);
//    $result = $db->carregar($sql);
//    if (is_array($result)) {
//        foreach ($result as $key => $_) {
//            $result[$key]['dotacaoatual'] = mascaraMoeda($result[$key]['dotacaoatual'], false);
//            $result[$key]['det_subacao'] = mascaraMoeda($result[$key]['det_subacao'], false);
//            $result[$key]['nao_det_subacao'] = mascaraMoeda($result[$key]['nao_det_subacao'], false);
//            $result[$key]['det_pi'] = mascaraMoeda($result[$key]['det_pi'], false);
//            $result[$key]['nao_det_pi'] = mascaraMoeda($result[$key]['nao_det_pi'], false);
//            $result[$key]['empenhado'] = mascaraMoeda($result[$key]['empenhado'], false);
//            $result[$key]['nao_empenhado'] = mascaraMoeda($result[$key]['nao_empenhado'], false);
//            $result[$key]['pipvalor_'] = $result[$key]['pipvalor']; // -- Não formatado - para soma na interface
//            $result[$key]['pipvalor'] = number_format($result[$key]['pipvalor'], 2, ',', '.');
//        }
//    }
//    return $result;
    $result = $db->carregar($sql);
    if(is_array($result)) {
        $result = formatarValoresFuncional($result);
    }
    return $result;
    
}

function buscarUmPTRES(stdClass $objFiltros) {
    global $db;
//ver($objFiltros,d);
    /* Filtros */
    $where .= $objFiltros->pliid ? " AND ptrpli.pliid = ". (int)$objFiltros->pliid: NULL;
    $where .= $objFiltros->ptrid ? " AND ptr.ptrid = ". (int)$objFiltros->ptrid: NULL;

    $sql = "
        SELECT
            ptr.ptrid,
            ptr.ptres,
            TRIM(aca.prgcod) || '.' || TRIM(aca.acacod) || '.' || TRIM(aca.loccod) || '.' || (CASE WHEN LENGTH(TRIM(aca.acaobjetivocod)) <= 0 THEN '-' ELSE TRIM(aca.acaobjetivocod) END) || '.' || TRIM(ptr.plocod) || ' - ' || aca.acatitulo AS descricao,
            aca.unicod,
            uni.unidsc as unidsc,
            aca.prgcod,
	    prog.prgdsc,
	    aca.acacod,
	    aca.acatitulo,
            aca.loccod,
	    loc.locdsc,
            ptr.plocod,
	    po.plotitulo
        FROM monitora.ptres ptr
            JOIN monitora.acao aca on ptr.acaid = aca.acaid
            JOIN public.unidade uni on aca.unicod = uni.unicod
            LEFT JOIN public.localizador loc ON aca.loccod = loc.loccod
            LEFT JOIN monitora.programa prog ON aca.prgcod = prog.prgcod
            LEFT JOIN monitora.planoorcamentario po ON(
                aca.acacod = po.acacod
                AND aca.prgcod = po.prgcod
                AND aca.unicod = po.unicod
                AND ptr.plocod = po.plocodigo)
            LEFT JOIN monitora.pi_planointernoptres ptrpli ON ptr.ptrid = ptrpli.ptrid
        WHERE
            ptr.ptrstatus = 'A'
            AND aca.prgano = '$objFiltros->exercicio'
            $where
        GROUP BY
            ptr.ptrid,
            ptr.ptres,
            aca.prgcod,
            aca.acaobjetivocod,
            aca.acacod,
            aca.unicod,
            aca.loccod,
            aca.acatitulo,
            uni.unidsc,
            loc.locdsc,
            prog.prgdsc,
            po.plotitulo
    ";
//ver($sql, d);
    $result = $db->pegaLinha($sql);

    return $result;
}

function enviarEmailAprovacao($pliid){
    
    include_once APPRAIZ . "planacomorc/classes/Pi_Responsavel.class.inc";
    
    global $db;
    
    $acao = "Enviado para Aprovação";
    
    # Buscar dados do PI para o corpo do e-mail
    $pi = carregarPI($pliid);
    
    $ptres = buscarUmPTRES((object) array(
        'pliid' => $pi['pliid'],
        'exercicio' => $_SESSION['exercicio']
    ));

    $usuario = wf_pegarUltimoUsuarioModificacao($pi['docid']);

    # $textoEmail
    include APPRAIZ. "planacomorc/modulos/principal/unidade/email.inc";

    $listaUsuariosPlanejamento = buscarUsuarioPerfilPlanejamento((object) array(
        'sisid' => SISID_PLANEJAMENTO,
        'pflcod' => PFL_ADMINISTRADOR));
    
    $listaUsuariosResponsaveisDoPi = (new Pi_Responsavel())->recuperarPorPlanoInterno($pliid);
    
    $listaDestinatario = array_merge(
        $listaUsuariosPlanejamento? $listaUsuariosPlanejamento: array(),
        $listaUsuariosResponsaveisDoPi);

//ver(
//array(
//# Remetente
//'nome' => SIGLA_SISTEMA. ' - SPOA - Planejamento Orçamentário',
//'email' => $_SESSION['email_sistema']
//),
//array(
//# Destinatario
//'email' => $listaResponsaveis
//),
//'PI - '. ($pi['plicod']? $pi['plicod']: $pi['pliid']). ' - '. $acao, # Titulo do e-mail
//$textoEmail,
//d);
    if($listaDestinatario){
        # Envia E-mail para o SOLICITANTE    
        enviar_email(
            array(
                # Remetente
                'nome' => SIGLA_SISTEMA. ' - SPOA - Planejamento Orçamentário',
                'email' => $_SESSION['email_sistema']
            ),
            $listaDestinatario,
            'PI - '. ($pi['plicod']? $pi['plicod']: $pi['pliid']). ' - '. $acao, # Titulo do e-mail
            $textoEmail
        );
    }
    
    return true;
}

function enviarEmailCorrecao($pliid){
    
    include_once APPRAIZ . "planacomorc/classes/Pi_Responsavel.class.inc";
    
    global $db;
    
    $acao = "Enviado para Correção";
    
    # Buscar dados do PI para o corpo do e-mail
    $pi = carregarPI($pliid);
    
    $ptres = buscarUmPTRES((object) array(
        'pliid' => $pi['pliid'],
        'exercicio' => $_SESSION['exercicio']
    ));

    $usuario = wf_pegarUltimoUsuarioModificacao($pi['docid']);
    
    $textoDevolucao = wf_pegarComentarioEstadoAtual($pi['docid']);

    # $textoEmail
    include APPRAIZ. "planacomorc/modulos/principal/unidade/email.inc";

    $listaUsuariosUnidadeDoPi = buscarUsuarioPerfilPlanejamento((object) array(
        'sisid' => SISID_PLANEJAMENTO,
        'pflcod' => PFL_SUBUNIDADE,
        'ungcod' => $pi['ungcod']));

    $listaUsuariosResponsaveisDoPi = (new Pi_Responsavel())->recuperarPorPlanoInterno($pliid);
    
    $listaDestinatario = array_merge(
        $listaUsuariosUnidadeDoPi? $listaUsuariosUnidadeDoPi: array(),
        $listaUsuariosResponsaveisDoPi);

//ver(
//array(
//# Remetente
//'nome' => SIGLA_SISTEMA. ' - SPOA - Planejamento Orçamentário',
//'email' => $_SESSION['email_sistema']
//),
//array(
//# Destinatario
//'email' => $listaResponsaveis
//),
//'PI - '. ($pi['plicod']? $pi['plicod']: $pi['pliid']). ' - '. $acao, # Titulo do e-mail
//$textoEmail,
//d);
    if($listaDestinatario){
        # Envia E-mail para o SOLICITANTE
        enviar_email(
            array(
                # Remetente
                'nome' => SIGLA_SISTEMA. ' - SPOA - Planejamento Orçamentário',
                'email' => $_SESSION['email_sistema']
            ),
            $listaDestinatario,
            'PI - '. ($pi['plicod']? $pi['plicod']: $pi['pliid']). ' - '. $acao, # Titulo do e-mail
            $textoEmail
        );
    }
    
    return true;
}

function enviarEmailAprovado($pliid){
    
    include_once APPRAIZ . "planacomorc/classes/Pi_Responsavel.class.inc";
    
    global $db;
    
    $acao = "Aprovado";
    
    # Buscar dados do PI para o corpo do e-mail
    $pi = carregarPI($pliid);
    
    $ptres = buscarUmPTRES((object) array(
        'pliid' => $pi['pliid'],
        'exercicio' => $_SESSION['exercicio']
    ));

    $usuario = wf_pegarUltimoUsuarioModificacao($pi['docid']);

    # $textoEmail
    include APPRAIZ. "planacomorc/modulos/principal/unidade/email.inc";

    $listaUsuariosPlanejamento = buscarUsuarioPerfilPlanejamento((object) array(
        'sisid' => SISID_PLANEJAMENTO,
        'pflcod' => PFL_ADMINISTRADOR));
    
    $listaUsuariosUnidadeDoPi = buscarUsuarioPerfilPlanejamento((object) array(
        'sisid' => SISID_PLANEJAMENTO,
        'pflcod' => PFL_SUBUNIDADE,
        'ungcod' => $pi['ungcod']));

    $listaUsuariosResponsaveisDoPi = (new Pi_Responsavel())->recuperarPorPlanoInterno($pliid);
    
    $listaDestinatarioUsuariosTecnicos = array_merge(
        $listaUsuariosPlanejamento? $listaUsuariosPlanejamento: array(),
        $listaUsuariosUnidadeDoPi? $listaUsuariosUnidadeDoPi: array());
    
    $listaDestinatario = array_merge($listaDestinatarioUsuariosTecnicos, $listaUsuariosResponsaveisDoPi);

//ver(
//array(
//# Remetente
//'nome' => SIGLA_SISTEMA. ' - SPOA - Planejamento Orçamentário',
//'email' => $_SESSION['email_sistema']
//),
//array(
//# Destinatario
//'email' => $listaDestinatario
//),
//'PI - '. ($pi['plicod']? $pi['plicod']: $pi['pliid']). ' - '. $acao, # Titulo do e-mail
//$textoEmail,
//d);
    if($listaDestinatario){
        # Envia E-mail para o SOLICITANTE
        enviar_email(
            array(
                # Remetente
                'nome' => SIGLA_SISTEMA. ' - SPOA - Planejamento Orçamentário',
                'email' => $_SESSION['email_sistema']
            ),
            $listaDestinatario,
            'PI - '. ($pi['plicod']? $pi['plicod']: $pi['pliid']). ' - '. $acao, # Titulo do e-mail
            $textoEmail
        );
    }
    return true;
}

/*
 *  Enviar e-mail para os envolvidos no processo de Remanejamento ou
 *  Cadastro de PI
 *  @params tipoEvento[
 *                     solCadPI:
 *                     Depende de: scpid;
 *
 *                     solRemPI:
 *                     Depende de: rmpid;
 *
 *                     homCadPI:
 *                     Depende de: scpid, pliid;
 *
 *                     homRemPI:
 *                     Depende de: rmpid;
 *
 *                     aprCadPi:
 *                     Depende de: pliid;
 *
 *                     efeRemPI:
 *                     Depende de: rmpid(da efetivação);
 *          ];
 */

function enviaEmailPI($params) {
    $pflGestorOrcamentario = PFL_GESTAO_ORCAMENTARIA;
    $pflGabinete = PFL_GABINETE;
    global $db;

    /* Seleciona o tipo do Evento */
    switch ($params['tipoEvento']) {
        /*
         * SOLICITAÇÃO DE CADASTRO DE PI
         */
        case 'solCadPI':

            $sql = <<<DML
SELECT usunome, usuemail, scptitulo, TO_CHAR(scpdata, 'DD/MM/YYYY') AS scpdata, scp.unicod
  FROM seguranca.usuario
    INNER JOIN planacomorc.solicitacaocriacaopi scp USING(usucpf)
  WHERE scp.scpid = %d
DML;
            $usudata = $db->pegaLinha(sprintf($sql, $params['scpid']));

            $tituloEmail = 'Solicitação de novo PI';
            $textoEmail = <<<TEXT
Caro(a) usuário(a) ,<br />
<p style="padding-left:25px">

Foi realizada a solicitação do cadastro de um PI pela Unidade Orçamentária: {$usudata['unicod']}, com o título: {$usudata['scptitulo']} em {$usudata['scpdata']}.

<br/> <br/>
    Para maiores informações, acesse o módulo Planejamento e Acompanhamento Orçamentário no <a href="http://simec.mec.gov.br">SIMEC</a>.</p>
<p style="font-size:9px">Este é um e-mail automático, favor não responder.</p>
TEXT;

            /* Envia E-mail para o SOLICITANTE */
            enviar_email(
                    array(// -- Remetente
                'nome' => 'Planejamento e Acompanhamento Orçamentário',
                'email' => $_SESSION['email_sistema']), array(// -- Destinatario
                'nome' => $usudata['usunome'],
                'email' => $usudata['usuemail']
                    ), $tituloEmail, $textoEmail
            );
            /* Envia E-mail para o Gestor Orçamentário Responsável */

            $sql = <<<SQL
SELECT DISTINCT
    usu.usunome,
    usu.usuemail
FROM
    seguranca.usuario usu
INNER JOIN
    planacomorc.usuarioresponsabilidade ur
USING
    (usucpf)
WHERE
    ur.pflcod = {$pflGestorOrcamentario}
    AND ur.unicod = '{$usudata['unicod']}'
    AND ur.rpustatus = 'A'
SQL;

            if ($result = $db->carregar($sql)) {
                foreach ($result as $usudata) {
                    enviar_email(
                            array(
// -- Remetente
                        'nome' => 'Planejamento e Acompanhamento Orçamentário',
                        'email' => $_SESSION['email_sistema']), array(
// -- Destinatario
                        'nome' => $usudata['usunome'],
                        'email' => $usudata['usuemail']
                            ), $tituloEmail, $textoEmail
                    );
                }
            }
            break;

        /*
         * SOLICITAÇÃO DE REMANEJAMENTO DE PI
         */
        case 'solRemPI':

            $sql = <<<DML
SELECT usunome, usuemail, funcprogramatica, ur.unicod, ur.ungcod
  FROM seguranca.usuario
    INNER JOIN planacomorc.remanejamentopi rmp USING(usucpf)
INNER JOIN
    planacomorc.usuarioresponsabilidade ur
USING (usucpf)
   WHERE ur.pflcod IN({$pflGabinete},{$pflGestorOrcamentario})
  AND rmp.rmpid = %d
   LIMIT 1
DML;

            $usudata = $db->pegaLinha(sprintf($sql, $params['rmpid']));

            if (!$usudata['unicod']) {
                $usudata['unicod'] = $db->pegaUm("SELECT unicod from public.unidadegestora WHERE ungcod = '{$usudata['ungcod']}'");
            }

            $tituloEmail = 'Solicitação de Remanejamento de PI';
            $textoEmail = <<<TEXT
Caro(a) usuário(a) ,<br />
<p style="padding-left:25px">

Foi realizada a solicitação de remanejamento de um PI pela Unidade Orçamentária: {$usudata['unicod']}, envolvendo a ação: {$usudata['funcprogramatica']} .

<br/> <br/>
    Para maiores informações, acesse o módulo Planejamento e Acompanhamento Orçamentário no <a href="http://simec.mec.gov.br">SIMEC</a>.</p>
<p style="font-size:9px">Este é um e-mail automático, favor não responder.</p>
TEXT;

            /* Envia E-mail para o SOLICITANTE */
            enviar_email(
                    array(// -- Remetente
                'nome' => 'Planejamento e Acompanhamento Orçamentário',
                'email' => $_SESSION['email_sistema']), array(// -- Destinatario
                'nome' => $usudata['usunome'],
                'email' => $usudata['usuemail']
                    ), $tituloEmail, $textoEmail
            );

            /* Envia E-mail para o Gestor Orçamentário Responsável */
            $sql = <<<SQL
SELECT DISTINCT
    usu.usunome,
    usu.usuemail
FROM
    seguranca.usuario usu
INNER JOIN
    planacomorc.usuarioresponsabilidade ur
USING
    (usucpf)
WHERE
    ur.pflcod = {$pflGestorOrcamentario}
    AND ur.unicod = '{$usudata['unicod']}'
    AND ur.rpustatus = 'A'
SQL;
            # ver($sql,d);
            if ($result = $db->carregar($sql)) {
                foreach ($result as $usudata) {
                    enviar_email(
                            array(
// -- Remetente
                        'nome' => 'Planejamento e Acompanhamento Orçamentário',
                        'email' => $_SESSION['email_sistema']), array(
// -- Destinatario
                        'nome' => $usudata['usunome'],
                        'email' => $usudata['usuemail']
                            ), $tituloEmail, $textoEmail
                    );
                }
            }
            break;

        /*
         * HOMOLOGAÇÃO DE CADSTRO DE PI
         */
        case 'homCadPI':

            $sql = <<<DML
SELECT usunome, usuemail, scptitulo, TO_CHAR(scpdata, 'DD/MM/YYYY') AS scpdata, scp.unicod
  FROM seguranca.usuario
    INNER JOIN planacomorc.solicitacaocriacaopi scp USING(usucpf)
  WHERE scp.scpid = %d
DML;
            $usudata = $db->pegaLinha(sprintf($sql, $params['scpid']));

            $tituloEmail = 'Homologação de Cadastro de PI';
            $textoEmail = <<<TEXT
Caro(a) usuário(a) ,<br />
<p style="padding-left:25px">

Foi realizada a homologação do cadastro de um PI,  solicitado pela Unidade Orçamentária: {$usudata['unicod']}, com o título: {$usudata['scptitulo']}.

<br/> <br/>
    Para maiores informações, acesse o módulo Planejamento e Acompanhamento Orçamentário no <a href="http://simec.mec.gov.br">SIMEC</a>.</p>
<p style="font-size:9px">Este é um e-mail automático, favor não responder.</p>
TEXT;

            /* Envia E-mail para o SOLICITANTE */
            enviar_email(
                    array(// -- Remetente
                'nome' => 'Planejamento e Acompanhamento Orçamentário',
                'email' => $_SESSION['email_sistema']), array(// -- Destinatario
                'nome' => $usudata['usunome'],
                'email' => $usudata['usuemail']
                    ), $tituloEmail, $textoEmail
            );

            /* Envia E-mail para o Gestor Orçamentário Responsável */
            $sql = <<<SQL
SELECT DISTINCT
    usu.usunome,
    usu.usuemail
FROM
    seguranca.usuario usu
INNER JOIN
    planacomorc.usuarioresponsabilidade ur
USING
    (usucpf)
WHERE
    ur.pflcod = {$pflGestorOrcamentario}
    AND ur.unicod = '{$usudata['unicod']}'
    AND ur.rpustatus = 'A'
SQL;
            if ($result = $db->carregar($sql)) {
                foreach ($result as $usudata) {
                    enviar_email(
                            array(
// -- Remetente
                        'nome' => 'Planejamento e Acompanhamento Orçamentário',
                        'email' => $_SESSION['email_sistema']), array(
// -- Destinatario
                        'nome' => $usudata['usunome'],
                        'email' => $usudata['usuemail']
                            ), $tituloEmail, $textoEmail
                    );
                }
            }
            break;
        /*
         * HOMOLOGAÇÃO DE REMANEJAMENTO DE PI
         */
        case 'homRemPI':

            $sql = <<<DML
SELECT usunome, usuemail, funcprogramatica, ur.unicod
  FROM seguranca.usuario
    INNER JOIN planacomorc.remanejamentopi rmp USING(usucpf)
INNER JOIN
    planacomorc.usuarioresponsabilidade ur
USING (usucpf)
   WHERE ur.pflcod IN({$pflGabinete},{$pflGestorOrcamentario})
  AND rmp.rmpid = %d
   LIMIT 1
DML;
            $usudata = $db->pegaLinha(sprintf($sql, $params['rmpid']));

            $tituloEmail = 'Homologação de Remanejamento de Valor de PI';
            $textoEmail = <<<TEXT
Caro(a) usuário(a) ,<br />
<p style="padding-left:25px">

Foi realizada a homologação do remanejamento de um Valor de PI, solicitado pela Unidade Orçamentária: {$usudata['unicod']}, envolvendo a ação: {$usudata['funcprogramatica']} .

<br/> <br/>
    Para maiores informações, acesse o módulo Planejamento e Acompanhamento Orçamentário no <a href="http://simec.mec.gov.br">SIMEC</a>.</p>
<p style="font-size:9px">Este é um e-mail automático, favor não responder.</p>
TEXT;

            /* Envia E-mail para o SOLICITANTE */
            enviar_email(
                    array(// -- Remetente
                'nome' => 'Planejamento e Acompanhamento Orçamentário',
                'email' => $_SESSION['email_sistema']), array(// -- Destinatario
                'nome' => $usudata['usunome'],
                'email' => $usudata['usuemail']
                    ), $tituloEmail, $textoEmail
            );

            /* Envia E-mail para o Gestor Orçamentário Responsável */
            $sql = <<<SQL
SELECT DISTINCT
    usu.usunome,
    usu.usuemail
FROM
    seguranca.usuario usu
INNER JOIN
    planacomorc.usuarioresponsabilidade ur
USING
    (usucpf)
WHERE
    ur.pflcod = {$pflGestorOrcamentario}
    AND ur.unicod = '{$usudata['unicod']}'
    AND ur.rpustatus = 'A'
SQL;
            if ($result = $db->carregar($sql)) {
                foreach ($result as $usudata) {
                    enviar_email(
                            array(
// -- Remetente
                        'nome' => 'Planejamento e Acompanhamento Orçamentário',
                        'email' => $_SESSION['email_sistema']), array(
// -- Destinatario
                        'nome' => $usudata['usunome'],
                        'email' => $usudata['usuemail']
                            ), $tituloEmail, $textoEmail
                    );
                }
            }
            break;

        default:
            break;
    }
}

/**
 * Pega as UOs dos usuários com perfis PFL_GESTAO_ORCAMENTARIA e/ou PFL_GABINETE.
 *
 * @global cls_banco $db Conexão com o banco.
 * @param number $pflcod Código do perfil do usuário.
 * @return array Lista de UOs associados ao perfil/usuário.
 */
function pegaUOsPerfil($pflcod) {
    global $db;

    switch ($pflcod) {
        case PFL_GESTAO_ORCAMENTARIA:
        case PFL_GABINETE:
            $sql = <<<DML
SELECT COALESCE(usr.unicod, ung.unicod) AS unicod
  FROM planacomorc.usuarioresponsabilidade usr
    LEFT JOIN public.unidadegestora ung USING(ungcod)
  WHERE usr.usucpf = '%s'
    AND usr.pflcod = %d
    AND usr.rpustatus = 'A'
DML;
            break;
        default:
            return array();
    }
    $stmt = sprintf($sql, $_SESSION['usucpf'], $pflcod);
    $result = $db->carregar($stmt);
    if (!$result) {
        return array();
    }
    foreach ($result as &$unicod) {
        $unicod = $unicod['unicod'];
    }
    return $result;
}

function resultado_soma_acoes($vaeid, $ano) {
    global $db;

    $sql = "SELECT DISTINCT acacod FROM planacomorc.vinculacaoestrategicaacoes WHERE vaeid = {$vaeid}";
    $acoes = $db->carregar($sql);
    if ($acoes) {
        $sqlFinanceira = "SELECT * FROM dblink(
                            'dbname= hostaddr= user= password= port=',
                            'SELECT sum(Empenho), sum(Pagamento), sum(rp_processado_pago) rp_processado_pago, sum(rp_nao_processado_pago_ate_2012), sum(rp_nao_processado_pago_apos_2012) FROM (";
        foreach ($acoes as $acaoatual) {

            $unidadesdaacao = "SELECT DISTINCT unicod FROM planacomorc.vinculacaoestrategicaacoes WHERE acacod = '{$acaoatual['acacod']}' and vaeid = {$vaeid}";
            $sqlunidades = $db->carregar($unidadesdaacao);

            $unidades = '';
            foreach ($sqlunidades as $unidadeatual) {
                $unidades .= "''{$unidadeatual['unicod']}'',";
            }
            $unidades = substr($unidades, 0, -1);


            $sqlFinanceira.= "SELECT
    acacod      AS cod_agrupador1,
    acadsc      AS dsc_agrupador1,
    unicod      AS cod_agrupador2,
    SUM(valor1) AS empenho,
    SUM(valor2) AS pagamento,
    SUM(valor3) AS rp_processado_pago,
    SUM(valor4) AS rp_nao_processado_pago_ate_2012,
    SUM(valor5) AS rp_nao_processado_pago_apos_2012
FROM
    (
        SELECT
            sld.acacod,
            aca.acadsc,
            sld.unicod,
            CASE
                WHEN sld.sldcontacontabil IN (''292130100'',
                                              ''292130201'',
                                              ''292130202'',
                                              ''292130203'',
                                              ''292130301'')
                THEN
                    CASE
                        WHEN sld.ungcod=''154004''
                        THEN (sld.sldvalor)*2.3274
                        ELSE (sld.sldvalor)
                    END
                ELSE 0
            END AS valor1,
            CASE
                WHEN sld.sldcontacontabil IN (''292130301'',
                                              ''292410403'')
                THEN
                    CASE
                        WHEN sld.ungcod=''154004''
                        THEN (sld.sldvalor)*2.3274
                        ELSE (sld.sldvalor)
                    END
                ELSE 0
            END AS valor2,
            CASE
                WHEN sld.sldcontacontabil IN (''295210201'',
                                              ''295210202'')
                THEN
                    CASE
                        WHEN sld.ungcod=''154004''
                        THEN (sld.sldvalor)*2.3274
                        ELSE (sld.sldvalor)
                    END
                ELSE 0
            END AS valor3,
            CASE
                WHEN sld.sldcontacontabil IN (''295110300'')
                THEN
                    CASE
                        WHEN sld.ungcod=''154004''
                        THEN (sld.sldvalor)*2.3274
                        ELSE (sld.sldvalor)
                    END
                ELSE 0
            END AS valor4,
            CASE
                WHEN sld.sldcontacontabil IN (''295110301'',
                                              ''295110302'')
                THEN
                    CASE
                        WHEN sld.ungcod=''154004''
                        THEN (sld.sldvalor)*2.3274
                        ELSE (sld.sldvalor)
                    END
                ELSE 0
            END AS valor5
        FROM
            dw.saldo$ano sld
        LEFT JOIN
            dw.acao aca
        ON
            aca.acacod = sld.acacod

                                    WHERE sld.unicod in ($unidades) AND sld.acacod in (''{$acaoatual['acacod']}'') AND sld.sldcontacontabil in (''292130100'',''292130201'',''292130202'',''292130203'',''292130301'',''292130301'',''292410403'',''295110300'',
                                    ''295110301'',            ''295110302''           ,''295210201'')) as foo
                                 WHERE

                                 valor1 <> 0
OR  valor2 <> 0
OR  valor3 <> 0
OR  valor4 <> 0
OR  valor5 <> 0
GROUP BY
    acacod,
    acadsc,
    unicod

                                  union ";
        }
        $sqlFinanceira = substr($sqlFinanceira, 0, -6);
        $sqlFinanceira.= ") buscafinanceira;'  )
                        AS aca
                        (
                            Empenho NUMERIC(15,2),
                            Pagamento NUMERIC(15,2),
                            rp_processado_pago NUMERIC(15,2),
                rp_nao_processado_pago_ate_2012 NUMERIC(15,2),
                rp_nao_processado_pago_apos_2012 NUMERIC(15,2)

                        )";

        $table = array();
        $rs = $db->carregar($sqlFinanceira);
        if ($rs) {

            $rapnprocessadopago = ($ano < 2013) ? $rs[0]["rp_nao_processado_pago_ate_2012"] : $rs[0]["rp_nao_processado_pago_apos_2012"];

            $rs[0]['RapNPPago'] = $rapnprocessadopago;

            $table['html'] = '<table class="tabela table table-striped table-bordered table-hover" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">';
            $table['html'].= '<tr>';
            $table['html'].= '<td><span class="red">Despesas Empenhadas:</span> <span class="bold">' . number_format($rs[0]['empenho'], 2, ',', '.') . '</span></td>';
            $table['html'].= '<td><span class="red">Valores Pagos:</span> <span class="bold">' . number_format($rs[0]['pagamento'], 2, ',', '.') . '</span></td>';
            $table['html'].= '<td><span class="red">RAP não-Processados Pagos:</span> <span class="bold">' . number_format($rs[0]['rp_processado_pago'], 2, ',', '.') . '</span></td>';
            $table['html'].= '<td><span class="red">RAP Processados Pagos:</span> <span class="bold">' . number_format($rapnprocessadopago, 2, ',', '.') . '</span></td>';
            $table['html'].= '</tr>';
            $table['html'].= '</table>';

            $table['total'] = current($rs);
        } else {
            $table['html'] = '';
        }
    } else {
        $table['html'] = '';
    }

    return $table;
}
function resultado_soma_acoes_old($vaeid) {

    global $db;

    $sql = "SELECT DISTINCT acacod FROM planacomorc.vinculacaoestrategicaacoes WHERE vaeid = {$vaeid}";
    $acoes = $db->carregar($sql);
    if ($acoes) {
        $sqlFinanceira = "SELECT * FROM dblink(
                            'dbname= hostaddr= user= password= port=',
                            'SELECT sum(Empenho), sum(Pagamento), sum(RapNPPago), sum(rp_processado_pago) FROM (";
        foreach ($acoes as $acaoatual) {

            $unidadesdaacao = "SELECT DISTINCT unicod FROM planacomorc.vinculacaoestrategicaacoes WHERE acacod = '{$acaoatual['acacod']}' and vaeid = {$vaeid}";
            $sqlunidades = $db->carregar($unidadesdaacao);

            $unidades = '';
            foreach ($sqlunidades as $unidadeatual) {
                $unidades .= "''{$unidadeatual['unicod']}'',";
            }
            $unidades = substr($unidades, 0, -1);


            $sqlFinanceira.= "SELECT
                                    unicod,
                                    acacod,
                                    sum(valor1) AS Empenho,sum(valor2) AS Pagamento,sum(valor3) AS RapNPPago,sum(valor4) AS rp_processado_pago
                                    FROM
                                    (SELECT
                                       sld.unicod,
                                       sld.acacod,
                                       uni.unidsc,
                                       CASE WHEN sld.sldcontacontabil in (''292130100'',''292130201'',''292130202'',''292130203'',''292130301'') THEN
                         CASE WHEN sld.ungcod=''154004'' then (sld.sldvalor)*2.3262 ELSE (sld.sldvalor) END
                        ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in (''292130301'',''292410403'') THEN
                         CASE WHEN sld.ungcod=''154004'' then (sld.sldvalor)*2.3262 ELSE (sld.sldvalor) END
                        ELSE 0 END AS valor2,CASE WHEN sld.sldcontacontabil in (''295110300'') THEN
                         CASE WHEN sld.ungcod=''154004'' then (sld.sldvalor)*2.3262 ELSE (sld.sldvalor) END
                        ELSE 0 END AS valor3,CASE WHEN sld.sldcontacontabil in (''295210201'') THEN
                         CASE WHEN sld.ungcod=''154004'' then (sld.sldvalor)*2.3262 ELSE (sld.sldvalor) END
                        ELSE 0 END AS valor4
                               FROM
                                    dw.saldo2013 sld
                                    LEFT JOIN dw.uo uni ON uni.unicod = sld.unicod
                                    WHERE sld.unicod in ($unidades) AND sld.acacod in (''{$acaoatual['acacod']}'') AND sld.sldcontacontabil in (''292130100'',''292130201'',''292130202'',''292130203'',''292130301'',''292130301'',''292410403'',''295110300''
                                    ,''295210201'')) as foo
                                 WHERE
                                 valor1 <> 0 OR valor2 <> 0 OR valor3 <> 0 OR valor4 <> 0
                                 GROUP BY
                                 unicod,
                                 acacod,
                                 unidsc union ";
        }
        $sqlFinanceira = substr($sqlFinanceira, 0, -6);
        $sqlFinanceira.= ") buscafinanceira;'  )
                        AS aca
                        (
                            Empenho NUMERIC(15,2),
                            Pagamento NUMERIC(15,2),
                            RapNPPago NUMERIC(15,2),
                            rp_processado_pago NUMERIC(15,2)
                        )";

        $table = array();

        $rs = $db->carregar($sqlFinanceira);
        if ($rs) {
            $table['html'] = '<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">';
            $table['html'].= '<tr>';
            $table['html'].= '<td><span class="red">Despesas Empenhadas:</span> <span class="bold">' . number_format($rs[0]['empenho'], 2, ',', '.') . '</span></td>';
            $table['html'].= '<td><span class="red">Valores Pagos:</span> <span class="bold">' . number_format($rs[0]['pagamento'], 2, ',', '.') . '</span></td>';
            $table['html'].= '<td><span class="red">RAP não-Processados Pagos:</span> <span class="bold">' . number_format($rs[0]['RapNPPago'], 2, ',', '.') . '</span></td>';
            $table['html'].= '<td><span class="red">RAP Processados Pagos :</span> <span class="bold">' . number_format($rs[0]['rp_processado_pago'], 2, ',', '.') . '</span></td>';
            $table['html'].= '</tr>';
            $table['html'].= '</table>';

            $table['total'] = current($rs);
        } else {
            $table['html'] = '';
        }
    } else {
        $table['html'] = '';
    }

    return $table;
}

function carrega_soma_subacoes($vaeid, $ano) {
    global $db;
//Unidades
    $rsUnicod = $db->carregar("select distinct unicod from planacomorc.vinculacaoestrategicasubacoes where vaeid = {$vaeid}");
    if ($rsUnicod) {
        $i = 0;
        foreach ($rsUnicod as $row) {
            $unicods[$i] = $row['unicod'];
            $i++;
        }
    }

//Subacoes
    $rsSubacoes = $db->carregar("select distinct sbacod from planacomorc.vinculacaoestrategicasubacoes where vaeid = {$vaeid}");
    if ($rsSubacoes) {
        $i = 0;
        foreach ($rsSubacoes as $row) {
            $subacaoes[$i] = $row['sbacod'];
            $i++;
        }
    }

    if (!empty($unicods) && !empty($subacaoes)) {

        $rs = calcula_execucao_vinculcacao($ano, $acoes, $unicods, $subacaoes, $pis, $ptres);

        $table = array();
        if ($rs) {
            criaTabelinhaExecucao($rs, $table);
            $table['total'] = current($rs);
        } else {
            $table['html'] = '';
        }
    } else {
        $table['html'] = '';
    }
    return $table;
}

function carrega_soma_subacoes_old($vaeid) {
    global $db;

    $rsUnicod = $db->carregar("select distinct unicod from planacomorc.vinculacaoestrategicasubacoes where vaeid = {$vaeid}");
//Unidades
    if ($rsUnicod) {
        $unicods = '';
        foreach ($rsUnicod as $unicod) {
            $unicods.= "''{$unicod['unicod']}'',";
        }
        $unicods = substr($unicods, 0, -1);
    }

//Subações
    $rsSubacoes = $db->carregar("select distinct sbacod from planacomorc.vinculacaoestrategicasubacoes where vaeid = {$vaeid}");
    if ($rsSubacoes) {
        $subacoes = '';
        foreach ($rsSubacoes as $acao) {
            $subacoes.="''{$acao['sbacod']}'',";
        }
        $subacoes = substr($subacoes, 0, -1);
    }


    if ($unicods && $subacoes) {

        $strSQL = "SELECT * FROM dblink(
            'dbname= hostaddr= user= password= port=',
            'SELECT
                sum(valor1) AS empenho,sum(valor2) AS pagamento,sum(valor3) AS RapNPPago,sum(valor4) AS rp_processado_pago
                FROM
                (SELECT
                   sac.sbacod,
                   sac.sbatitulo,
                   CASE WHEN sld.sldcontacontabil in (''292130100'',''292130201'',''292130202'',''292130203'',''292130301'') THEN
                   CASE WHEN sld.ungcod=''154004'' then (sld.sldvalor)*2.3262 ELSE (sld.sldvalor) END
                   ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in (''292130301'',''292410403'') THEN
                   CASE WHEN sld.ungcod=''154004'' then (sld.sldvalor)*2.3262 ELSE (sld.sldvalor) END
                   ELSE 0 END AS valor2,CASE WHEN sld.sldcontacontabil in (''295110300'') THEN
                   CASE WHEN sld.ungcod=''154004'' then (sld.sldvalor)*2.3262 ELSE (sld.sldvalor) END
                   ELSE 0 END AS valor3,CASE WHEN sld.sldcontacontabil in (''295210201'') THEN
                   CASE WHEN sld.ungcod=''154004'' then (sld.sldvalor)*2.3262 ELSE (sld.sldvalor) END
                   ELSE 0 END AS valor4
                FROM
                   dw.saldo2013 sld
                   INNER JOIN financeiro.subacao sac ON sac.sbastatus = ''A'' AND sac.sbacod = substr(sld.plicod, 2, 4)
                   --WHERE sld.unicod in ({$unicods}) AND substr(sld.plicod, 2, 4) in ({$subacoes}) AND sld.sldcontacontabil in (''292130100'',''292130201'',''292130202'',''292130203'',''292130301'',''292130301'',''292410403'',''295110300'',''295210201'')) as foo
                   WHERE sld.unicod in (''26101'',''26298'',''26290'',''26291'',''26443'') AND substr(sld.plicod, 2, 4) in ({$subacoes}) AND sld.sldcontacontabil in (''292130100'',''292130201'',''292130202'',''292130203'',''292130301'',''292130301'',''292410403'',''295110300'',''295210201'')) as foo
                WHERE
                valor1 <> 0 OR valor2 <> 0 OR valor3 <> 0 OR valor4 <> 0;')
            AS sba
            (
                empenho NUMERIC(15,2),
                pagamento NUMERIC(15,2),
                RapNPPago NUMERIC(15,2),
                rp_processado_pago NUMERIC(15,2)
            )";

        $table = array();
        $rs = $db->carregar($strSQL);

        if ($rs) {
            $table['html'] = '<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">';
            $table['html'].= '<tr>';
            $table['html'].= '<td><span class="red">Despesas Empenhadas:</span> <span class="bold">' . number_format($rs[0]['empenho'], 2, ',', '.') . '</span></td>';
            $table['html'].= '<td><span class="red">Valores Pagos:</span> <span class="bold">' . number_format($rs[0]['pagamento'], 2, ',', '.') . '</span></td>';
            $table['html'].= '<td><span class="red">RAP não-Processados Pagos:</span> <span class="bold">' . number_format($rs[0]['RapNPPago'], 2, ',', '.') . '</span></td>';
            $table['html'].= '<td><span class="red">RAP Processados Pagos:</span> <span class="bold">' . number_format($rs[0]['rp_processado_pago'], 2, ',', '.') . '</span></td>';
            $table['html'].= '</tr>';
            $table['html'].= '</table>';

            $table['total'] = current($rs);
        } else {
            $table['html'] = '';
        }
    } else {
        $table['html'] = '';
    }

    return $table;
}

function carrega_soma_ptres($vaeid, $ano) {
    global $db;
    $rs_ptres = $db->carregar("SELECT ptres FROM planacomorc.vinculacaoestrategicapos WHERE vaeid={$vaeid}");

    if ($rs_ptres) {
        $i = 0;
        foreach ($rs_ptres as $row) {
            $ptres[$i] = $row['ptres'];
            $i++;
        }
        /// ################ Só falta passar o ano CERTO
        $rs = calcula_execucao_vinculcacao($ano, $acoes, $unidades, $subacaoes, $pis, $ptres);

        $table = array();
        if ($rs) {
            criaTabelinhaExecucao($rs, $table);
            $table['total'] = current($rs);
        } else {
            $table['html'] = '';
        }
    } else {
        $table['html'] = '';
    }
    return $table;
}

function carrega_soma_pi($vaeid, $ano) {
    global $db;
    $sqlFindPi = $db->carregar("SELECT plicod FROM planacomorc.vinculcaoestrategicapis WHERE vaeid={$vaeid}");

    if ($sqlFindPi) {
        $i = 0;
        foreach ($sqlFindPi as $row) {
            $pis[$i] = $row['plicod'];
            $i++;
        }
        /// ################ Só falta passar o ano CERTO
        $rs = calcula_execucao_vinculcacao($ano, $acoes, $unidades, $subacaoes, $pis, $ptres);
        $table = array();

        if ($rs) {
            criaTabelinhaExecucao($rs, $table);
            $table['total'] = current($rs);
        } else {
            $table['html'] = '';
        }
    } else {
        $table['html'] = '';
    }
    return $table;
}

function criaTabelinhaExecucao(&$rs, &$table) {
    $table['html'] = '<table class="tabela table table-striped table-bordered table-hover" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">';
    $table['html'].= '<tr>';
    $table['html'].= '<td><span class="red">Despesas Empenhadas:</span> <span class="bold">' . number_format($rs[0]['empenho'], 2, ',', '.') . '</span></td>';
    $table['html'].= '<td><span class="red">Valores Pagos:</span> <span class="bold">' . number_format($rs[0]['pagamento'], 2, ',', '.') . '</span></td>';
    $table['html'].= '<td><span class="red">RAP Processados Pagos:</span> <span class="bold">' . number_format($rs[0]['rp_processado_pago'], 2, ',', '.') . '</span></td>';
    $table['html'].= '<td><span class="red">RAP não-Processados Pagos:</span> <span class="bold">' . number_format($rs[0]['RapNPPago'], 2, ',', '.') . '</span></td>';
    $table['html'].= '</tr>';
    $table['html'].= '</table>';
}

function retornaFiltroSubacoesUoUg() {

    /* Retorna o Perfis do CPF logado */
    $cpf = $_SESSION['usucpf'];

    /* Faz filtro para Gestor Orçamentário */
    if (in_array(PFL_GESTAO_ORCAMENTARIA, pegaPerfilGeral())) {
        $perfil = PFL_GESTAO_ORCAMENTARIA;
        $sql = <<<SQL
                AND sba.sbaid IN (
                    SELECT
                        sbaid
                    FROM
                        monitora.pi_subacaounidade sbu
                    WHERE
                        sbu.unicod IN
                                       (
                                       SELECT DISTINCT
                                           unicod
                                       FROM
                                           planacomorc.usuarioresponsabilidade usr
                                       WHERE
                                           usr.usucpf = '{$cpf}'
                                       AND usr.pflcod = {$perfil}  AND usr.rpustatus = 'A')
                    OR  sbu.ungcod IN
                        (
                            SELECT
                                ungcod
                            FROM
                                public.unidadegestora
                            WHERE
                                unicod IN
                                           (
                                           SELECT DISTINCT
                                               unicod
                                           FROM
                                               planacomorc.usuarioresponsabilidade usr
                                           WHERE
                                               usr.usucpf = '{$cpf}'
                                           AND usr.pflcod = {$perfil}  AND usr.rpustatus = 'A') )
                    )

SQL;

        return $sql;
    }
    /* FIM filtro para Gestor Orçamentário */

    /* Faz filtro para Gabinete */
    if (in_array(PFL_GABINETE, pegaPerfilGeral())) {
        $perfil = PFL_GABINETE;
        $sql = <<<SQL
                AND sba.sbaid IN (
                    SELECT
                        sbaid
                    FROM
                        monitora.pi_subacaounidade sbu
                    WHERE
                    sbu.ungcod IN
                        (
                            SELECT
                                ungcod
                            FROM
                                public.unidadegestora
                            WHERE
                                ungcod IN
                                           (
                                           SELECT DISTINCT
                                               ungcod
                                           FROM
                                               planacomorc.usuarioresponsabilidade usr
                                           WHERE
                                               usr.usucpf = '{$cpf}'
                                           AND usr.pflcod = {$perfil}  AND usr.rpustatus = 'A') )
                    )

SQL;
        return $sql;
    }
    /* FIM filtro para Gabinete */
}

function retornaFiltroEfetivarRemanejamentoSubacoesUoUg() {

    $uoug = pegaUOsPerfil(PFL_GESTAO_ORCAMENTARIA);

    /* Ignora filtros para S.U. */
    if (!in_array(PFL_SUPERUSUARIO, pegaPerfilGeral()) || !in_array(PFL_CPMO, pegaPerfilGeral())) {
        /* Caso o usuario não tenha UO atribuida ao perfil */
        if (count($uoug) != 0) {
            /* Caso o usuário logado seja G.O da 26.101 */
            if (in_array("26101", $uoug)) {
                $sql = <<<PARCIAL_DML
    AND EXISTS (SELECT 1
                  FROM planacomorc.rmsmovimentacao rmm
                    INNER JOIN monitora.pi_subacaounidade sbu USING(sbaid)
                    LEFT JOIN public.unidadegestora ung USING(ungcod)
                  WHERE rms.rmsid = rmm.rmsid
                    AND COALESCE(sbu.unicod, ung.unicod)::numeric IN(%s))
PARCIAL_DML;
            } else {
                $sql = <<<PARCIAL_DML
    AND EXISTS (SELECT 1
                  FROM planacomorc.rmsmovimentacao rmm
                    INNER JOIN monitora.pi_subacaounidade sbu USING(sbaid)
                    LEFT JOIN public.unidadegestora ung USING(ungcod)
                  WHERE rms.rmsid = rmm.rmsid
                    AND COALESCE(sbu.unicod, ung.unicod)::numeric IN(%s))
    AND NOT EXISTS (SELECT 1
                      FROM planacomorc.rmsmovimentacao rmm
                        INNER JOIN monitora.pi_subacaounidade sbu USING(sbaid)
                        LEFT JOIN public.unidadegestora ung USING(ungcod)
                      WHERE rms.rmsid = rmm.rmsid
                        AND COALESCE(sbu.unicod, ung.unicod) = '26101')
PARCIAL_DML;
            }
            $sql = sprintf($sql, implode(', ', $uoug));
        }//fim validacao usuario sem UO atribuida ao perfil
    }//fim if pegaPerfilGeral
    return $sql;
}

function calcula_execucao_vinculcacao($ano, $acoes, $unidades, $subacaoes, $pis, $ptres) {
    global $db;
    $filtros = "";

    if (!empty($acoes)) {
        $filtros = " and sld.acacod IN ( ''" . implode("'',''", $acoes) . "'') ";
    }
    if (!empty($unidades)) {
        $filtros .= " and sld.unicod IN ( ''" . implode("'',''", $unidades) . "'') ";
    }
    if (!empty($subacaoes)) {
        $filtros .= " and substr(sld.plicod, 2, 4) IN ( ''" . implode("'',''", $subacaoes) . "'') ";
    }
    if (!empty($pis)) {
        $filtros .= " and sld.plicod IN ( ''" . implode("'',''", $pis) . "'') ";
    }
    if (!empty($ptres)) {
        $filtros .= " and sld.ptres IN ( ''" . implode("'',''", $ptres) . "'') ";
    }


    if (!empty($filtros)) {
        $strSQL = "SELECT * FROM dblink(
            'dbname= hostaddr= user= password= port=',
            'Select
	SUM(empenhado) AS empenhado,
    SUM(pago) AS pago,
    SUM(rp_processado_pago) AS rp_processado_pago,
    SUM(rp_nao_processado_pago_ate_2012) AS rp_nao_processado_pago_ate_2012,
    SUM(rp_nao_processado_pago_apos_2012) AS rp_nao_processado_pago_apos_2012
from
(

SELECT
    acacod      AS cod_agrupador1,
    acadsc      AS dsc_agrupador1,
    unicod      AS cod_agrupador2,
    SUM(valor1) AS empenhado,
    SUM(valor2) AS pago,
    SUM(valor3) AS rp_processado_pago,
    SUM(valor4) AS rp_nao_processado_pago_ate_2012,
    SUM(valor5) AS rp_nao_processado_pago_apos_2012
FROM
    (
        SELECT
            sld.acacod,
            aca.acadsc,
            sld.unicod,
            CASE
                WHEN sld.sldcontacontabil IN (''292130100'',
                                              ''292130201'',
                                              ''292130202'',
                                              ''292130203'',
                                              ''292130301'')
                THEN
                    CASE
                        WHEN sld.ungcod=''154004''
                        THEN (sld.sldvalor)*2.3274
                        ELSE (sld.sldvalor)
                    END
                ELSE 0
            END AS valor1,
            CASE
                WHEN sld.sldcontacontabil IN (''292130301'',
                                              ''292410403'')
                THEN
                    CASE
                        WHEN sld.ungcod=''154004''
                        THEN (sld.sldvalor)*2.3274
                        ELSE (sld.sldvalor)
                    END
                ELSE 0
            END AS valor2,
            CASE
                WHEN sld.sldcontacontabil IN (''295210201'',
                                              ''295210202'')
                THEN
                    CASE
                        WHEN sld.ungcod=''154004''
                        THEN (sld.sldvalor)*2.3274
                        ELSE (sld.sldvalor)
                    END
                ELSE 0
            END AS valor3,
            CASE
                WHEN sld.sldcontacontabil IN (''295110300'')
                THEN
                    CASE
                        WHEN sld.ungcod=''154004''
                        THEN (sld.sldvalor)*2.3274
                        ELSE (sld.sldvalor)
                    END
                ELSE 0
            END AS valor4,
            CASE
                WHEN sld.sldcontacontabil IN (''295110301'',
                                              ''295110302'')
                THEN
                    CASE
                        WHEN sld.ungcod=''154004''
                        THEN (sld.sldvalor)*2.3274
                        ELSE (sld.sldvalor)
                    END
                ELSE 0
            END AS valor5
        FROM
            dw.saldo$ano sld
        LEFT JOIN
            dw.acao aca
        ON
            aca.acacod = sld.acacod
        WHERE true
        $filtros

        AND sld.sldcontacontabil IN (''292130100'',
                                     ''292130201'',
                                     ''292130202'',
                                     ''292130203'',
                                     ''292130301'',
                                     ''292130301'',
                                     ''292410403'',
                                     ''295210201'',
                                     ''295210202'',
                                     ''295110300'',
                                     ''295110301'',
                                     ''295110302'')) AS foo
WHERE
    valor1 <> 0
OR  valor2 <> 0
OR  valor3 <> 0
OR  valor4 <> 0
OR  valor5 <> 0
GROUP BY
    acacod,
    acadsc,
    unicod
ORDER BY
    acacod,
    acadsc,
    unicod

) x; ' )
            AS sba
            (
                empenho NUMERIC(15,2),
                pagamento NUMERIC(15,2),
                rp_processado_pago NUMERIC(15,2),
                rp_nao_processado_pago_ate_2012 NUMERIC(15,2),
                rp_nao_processado_pago_apos_2012 NUMERIC(15,2)
            )";


/* Nova consulta com dados do SIOP */
//        $filtros = "";
//            if (!empty($acoes)) {
//                $filtros = " and sex.acacod IN ( '" . implode("','", $acoes) . "') ";
//            }
//            if (!empty($unidades)) {
//                $filtros .= " and sex.unicod IN ( '" . implode("','", $unidades) . "') ";
//            }
//            if (!empty($subacaoes)) {
//                $filtros .= " and substr(sex.plicod, 2, 4) IN ( '" . implode("','", $subacaoes) . "') ";
//            }
//            if (!empty($pis)) {
//                $filtros .= " and sex.plicod IN ( '" . implode("','", $pis) . "') ";
//            }
//            if (!empty($ptres)) {
//                $filtros .= " and sex.ptres IN ( '" . implode("','", $ptres) . "') ";
//            }
//        if (!empty($filtros)) {
//            $strSQL = "
//            SELECT
//                SUM(sex.vlrempenhado) AS Empenho ,
//                SUM(sex.vlrpago)      AS Pagamento,
//                0                     AS rp_processado_pago,
//                0                     AS rp_nao_processado_pago_ate_2012,
//                0                     AS rp_nao_processado_pago_apos_2012
//            FROM
//                spo.siopexecucao sex
//            WHERE
//            sex.exercicio = '{$ano}'
//            {$filtros} ";
        #ver($sqlFinanceira,$acoesFiltro, d);
/* FIM Nova consulta com dados do SIOP */

        $rs = $db->carregar($strSQL);

        if ($rs) {
            if ($ano < 2013)
                $rs[0]["RapNPPago"] = $rs[0]["rp_nao_processado_pago_ate_2012"];
            else
                $rs[0]["RapNPPago"] = $rs[0]["rp_nao_processado_pago_apos_2012"];
            //print_r($rs);
            return $rs;
        }
    }
}



/**
 * Formata de vermelho e coloca entre parenteses um valor negativo.
 *
 * @param string $valor Valor a ser formatado.
 * @return string
 */
function formatarValor($valor) {
    if (false === strpos($valor, '-')) {
        return $valor;
    }
    return '<span style="color:red">(' . $valor . ')</span>';
}

/**
 * Pega o estado atual do workflow
 *
 * @param integer $id_acomp_acao
 * @return integer
 */
function pegarEstadoAtual($id_acomp_acao) {
    global $db;
    $docid = pegarDocid($id_acomp_acao);
    if ($docid) {
        $sql = "SELECT ed.esdid
                FROM workflow.documento d
                    JOIN  workflow.estadodocumento ed on ed.esdid = d.esdid
                WHERE d.docid = " . $docid;
        $estado = (integer) $db->pegaUm($sql);
        return $estado;
    }
    return false;
}


/**
 * Pega o id do documento acompanhamento
 *
 * @param integer $capid
 * @return integer
 */
function pegarDocid($id_acomp_acao) {
    global $db;
    $sql = "Select	docid
			FROM planacomorc.acompanhamento_acao
			WHERE id_acompanhamento_acao = ".$id_acomp_acao;
    return $db->pegaUm($sql);
}

function recuperaUltimoPeriodoReferencia(){
    global $db;
    $sql = "
        SELECT
            id_periodo_referencia AS codigo
        FROM planacomorc.periodo_referencia p
        WHERE id_exercicio = '".$_SESSION['exercicio']."'
        ORDER BY id_periodo_referencia desc
        LIMIT 1
    ";
    return $db->pegaUm($sql);
}

function alinharUOEsquerda($unicod, $dados)
{
    return alinharEsquerda("{$unicod} - {$dados['unidsc']}");
}

function formatarTituloPI($plititulo)
{
    if ('N/A' == $plititulo) {
        return <<<HTML
<span class="label label-danger">não preenchido</span>
HTML;
    }
    return alinharEsquerda($plititulo);
}

function exibirIconeDelegadas($pliid){
    $strIcone = "";
    if($pliid){
        $strIcone = "<a href='#' title='Visualizar Sub-Unidades Delegadas' class='a_listar_delegadas' data-pi='". (int)$pliid. "'><span class='btn btn-primary btn-sm fa fa-handshake-o'></span></a>";
    }
    
    return $strIcone;
}

function formatarObrid($obrid)
{
    if ('N/A' == $obrid) {
        return '-';
    }
    return <<<HTML
<span class="label label-info abrir-obra" style="cursor:pointer"
      data-obrid="{$obrid}">{$obrid}</span>
HTML;
}

function formatarTcpid($tcpid)
{
    if ('[null]' == $tcpid) {
        return '-';
    }

    $tcpid = json_decode($tcpid);
    $html = '';
    foreach ($tcpid as $id) {
        $html .= <<<HTML
<span class="label label-info abrir-ted" style="cursor:pointer"
      data-tcpid="{$id}">{$id}</span>
HTML;
    }

    return $html;
}

function formatarCadastramento($cadastramento, $dados)
{
    $template = <<<HTML
<span class="label label-%s">%s</span>
HTML;
    switch ($cadastramento) {
        case Spo_Model_Planointerno::CADASTRADO_SIAFI_SIMEC:
            $cor = 'success';
            $texto = SIGLA_SISTEMA. '/SIAFI';
            break;
        case Spo_Model_Planointerno::CADASTRADO_SIMEC:
            $cor = 'default';
            $texto = SIGLA_SISTEMA;
            break;
        case Spo_Model_Planointerno::CADASTRADO_SIAFI:
            $cor = 'warning';
            $texto = 'SIAFI';

            if ('v' !== $dados['valido']) {
                return sprintf($template, $cor, $texto)
                    . <<<HTML
&nbsp<span class="glyphicon glyphicon-exclamation-sign"
           data-toggle="popover"
           title="O código deste PI é inválido"
           style="color:#d9534f;cursor:pointer"></span>
HTML;
            }
            break;
        case 4:
        default:
            $cor = 'danger';
            $texto = 'N/A';
    }
    return sprintf($template, $cor, $texto);
}

function exibirLinkEspelho($pliid){
    $strIcone = "";
    if($pliid){
        $strIcone = "<a href='#' title='Exibir detalhes do Plano Interno(Espelho)' class='a_espelho' data-pi='". (int)$pliid. "'>". (int) $pliid. "</a>";
    }

    return $strIcone;
}

/**
 * Exibe o grafico de UOs vinculadas.
 * 
 * @param string $colors
 * @param boolean $percentualPlanejamento
 */
function montarGraficoVinculadas($colors, $percentualPlanejamento = FALSE){
    $oPlanoInterno = new Pi_PlanoInterno();
    echo '<div class="panel-body">';
    $estatistica = $oPlanoInterno->consultarExecucaoOrcamentariaUo((object) array(
        'exercicio' => (int)$_SESSION['exercicio']
    ));
//ver($estatistica, d);
    $grafico = new Grafico(Grafico::K_TIPO_COLUNA, FALSE);
    $grafico
        ->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)
        ->setColors($colors)
        ->setEvent(array('click' => "exibirModalDetalheGrafico(0, event.point.series.name, event.point.category);"))
        ->gerarGrafico($estatistica, $percentualPlanejamento);
    echo '</div>';
}

/**
 * Exibe o grafico de Subunidades.
 * 
 * @param string $colors
 * @param boolean $percentualPlanejamento
 * @param array $listaSubunidade
 */
function montarGraficoDireta($colors, $percentualPlanejamento = FALSE, $listaSubunidade = array()){
    $oPlanoInterno = new Pi_PlanoInterno();
    echo '<div class="panel-body">';
    $estatistica = $oPlanoInterno->consultarExecucaoOrcamentariaSubunidade((object) array(
        'exercicio' => $_SESSION['exercicio'],
        # Caso o exercicio seja superior a 2018(ano da unificação de Òrgãos), o sistema busca todas as vinculadas do MC
        'unocod' => (int)$_SESSION['exercicio'] > 2018? UNICOD_MC: UNICOD_MINC,
        'listaSubunidade' => $listaSubunidade
    ));
    $grafico = new Grafico(Grafico::K_TIPO_COLUNA, FALSE);
    $grafico
        ->setWidth('85%')
        ->setColors($colors)
        ->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)
        ->setEvent(array('click' => "exibirModalDetalheGrafico(1, event.point.series.name, event.point.category);"))
        ->gerarGrafico($estatistica, $percentualPlanejamento);
    echo '</div>';
}

