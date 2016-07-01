<?php
/**
 * Arquivo de fun��es para per�odos de refer�ncia.
 * $Id: funcoesperiodoreferencia.php 98390 2015-06-09 18:48:49Z maykelbraz $
 */

/**
 *
 * @global cls_banco $db Conex�o com a base de dados.
 * @param array $dados
 * @return type
 */
function inserirPeriodoReferencia(array $dados)
{
    global $db;

    // -- Valida��es
    if (empty($dados)) {
        return array(
            'sucesso' => false,
            'msg' => 'Os dados do novo per�odo n�o podem ser vazios.'
        );
    }
    if (!chaveTemValor($dados, 'prfdsc')) {
        return array(
            'sucesso' => false,
            'msg' => "O campo 'Descri��o' � obrigat�rio e n�o pode ser vazio."
        );
    } else {
        // -- Escapando aspas simples para o postgres
        $dados['prfdsc'] = str_replace("'", "''", $dados['prfdsc']);
    }
    if (!chaveTemValor($dados, 'prfdatainicio')) {
        return array(
            'sucesso' => false,
            'msg' => "O campo 'Per�odo de validade' � obrigat�rio e n�o pode ser vazio."
        );
    }
    if (!chaveTemValor($dados, 'prfdatafim')) {
        return array(
            'sucesso' => false,
            'msg' => "O campo 'Per�odo de validade' � obrigat�rio e n�o pode ser vazio."
        );
    }
    if (!chaveTemValor($dados, 'prfpreenchimentoinicio')) {
        return array(
            'sucesso' => false,
            'msg' => "O campo 'Per�odo de preenchimento' � obrigat�rio e n�o pode ser vazio."
        );
    }
    if (!chaveTemValor($dados, 'prfpreenchimentofim')) {
        return array(
            'sucesso' => false,
            'msg' => "O campo 'Per�odo de preenchimento' � obrigat�rio e n�o pode ser vazio."
        );
    }
    try {
        $dados['prfdatainicio'] = preparaData($dados['prfdatainicio']);
        $_ = new DateTime($dados['prfdatainicio']);
    } catch (Exception $e) {
        return array(
            'sucesso' => false,
            'msg' => "A data inicial do per�odo � inv�lida."
        );
    }
    try {
        $dados['prfdatafim'] = preparaData($dados['prfdatafim']);
        $_ = new DateTime($dados['prfdatafim']);
    } catch (Exception $e) {
        return array(
            'sucesso' => false,
            'msg' => "A data final do per�odo � inv�lida."
        );
    }
    try {
        $dados['prfpreenchimentoinicio'] = preparaData($dados['prfpreenchimentoinicio']);
        $_ = new DateTime($dados['prfpreenchimentoinicio']);
    } catch (Exception $e) {
        return array(
            'sucesso' => false,
            'msg' => "A data de preenchimento inicial do per�odo � inv�lida."
        );
    }
    try {
        $dados['prfpreenchimentofim'] = preparaData($dados['prfpreenchimentofim']);
        $_ = new DateTime($dados['prfpreenchimentofim']);
    } catch (Exception $e) {
        return array(
            'sucesso' => false,
            'msg' => "A data final do per�odo � inv�lida."
        );
    }

    if (!chaveTemValor($dados, 'prfid')) { // -- Insert
        $sql = <<<DML
INSERT INTO recorc.periodoreferencia(
    prfdsc,
    prfdatainicio,
    prfdatafim,
    exercicio,
    prfpreenchimentoinicio,
    prfpreenchimentofim,
    codcaptacaosiop,
    exerciciosiop
) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', %d, '%s')
DML;
        $stmt = sprintf(
            $sql,
            $dados['prfdsc'],
            $dados['prfdatainicio'],
            $dados['prfdatafim'],
            $_SESSION['exercicio'],
            $dados['prfpreenchimentoinicio'],
            $dados['prfpreenchimentofim'],
            $dados['codcaptacaosiop'],
            $dados['exerciciosiop']
        );
    } else { // -- Update
        $sql = <<<DML
UPDATE recorc.periodoreferencia
  SET prfdsc = '%s',
      prfdatainicio = '%s',
      prfdatafim = '%s',
      prfpreenchimentoinicio = '%s',
      prfpreenchimentofim = '%s',
      codcaptacaosiop = %d,
      exerciciosiop = '%s'
  WHERE prfid = %d
DML;
        $stmt = sprintf(
            $sql,
            $dados['prfdsc'],
            $dados['prfdatainicio'],
            $dados['prfdatafim'],
            $dados['prfpreenchimentoinicio'],
            $dados['prfpreenchimentofim'],
            $dados['codcaptacaosiop'],
            $dados['exerciciosiop'],
            $dados['prfid']
        );
    }

    // -- Executando insert
    $db->executar($stmt);
    if ($db->commit()) {
        return array(
            'sucesso' => true,
            'msg' => 'Sua requisi��o foi executada com sucesso.'

        );
    }

    return array(
        'sucesso' => false,
        'msg' => 'N�o foi poss�vel inserir o novo per�odo.'
    );
}

function deletePeriodoReferencia(array $dados)
{
    global $db;

    // -- Validando dados do formul�rio
    if (!chaveTemValor($dados, 'prfid')) {
        return array(
            'sucesso' => false,
            'msg' => "Nenhum per�odo foi selecionado para exclus�o."
        );
    }

    // -- Inativando o per�odo
    $sql = <<<DML
UPDATE recorc.periodoreferencia
  SET prfstatus = 'I'
  WHERE prfid = %d
DML;
    $stmt = sprintf($sql, $dados['prfid']);

    // -- Executando update
    $db->executar($stmt);
    if ($db->commit()) {
        return array(
            'sucesso' => true,
            'msg' => 'Sua requisi��o foi executada com sucesso.'

        );
    }

    return array(
        'sucesso' => false,
        'msg' => 'N�o foi poss�vel excluir o per�odo selecionado.'
    );
}

function carregarPeriodoReferencia(array $dados)
{
    global $db;

    // -- Validando dados do formul�rio
    if (!chaveTemValor($dados, 'prfid')) {
        return array();
    }

    // -- Consultando dados do periodo
    $sql = <<<DML
SELECT prf.prfid,
       prf.prfdsc,
       to_char(prf.prfdatainicio, 'DD/MM/YYYY') AS prfdatainicio,
       to_char(prf.prfdatafim, 'DD/MM/YYYY') AS prfdatafim,
       to_char(prf.prfpreenchimentoinicio, 'DD/MM/YYYY') AS prfpreenchimentoinicio,
       to_char(prf.prfpreenchimentofim, 'DD/MM/YYYY') AS prfpreenchimentofim,
       codcaptacaosiop,
       exerciciosiop
  FROM recorc.periodoreferencia prf
  WHERE prf.prfid = %d
DML;

    $stmt = sprintf($sql, $dados['prfid']);
    return $db->pegaLinha($stmt);
}
