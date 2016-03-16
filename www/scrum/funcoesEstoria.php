<?php
/**
 * Sistema SCRUM
 * @package simec
 * @subpackage scrum
 */

/**
 * Cadastra um novo subprograma na base de dados do scrum.
 * @global cls_banco $db
 *      Conex�o com a base de dados.
 * @param array $dados
 *      Dados para cadastro. Chaves obrigat�rias: subprgdsc, prgid, sisid.
 */
function salvarEstoria($dados)
{
//    ver($dados,d);
    // -- Valida��o dos parametros
    if (!isset($dados['estdsc']) || !isset($dados['estdsc'])
        || empty($dados['subprgid']) || empty($dados['subprgid'])) {
        return false;
    }

    if (isset($dados['estid']) && !empty($dados['estid']) && !empty($dados['esttitulo'])) { 
        // -- update
        $dml = <<<DML
UPDATE scrum.estoria
  SET estdsc = '%s',
      subprgid = %d,
      esttitulo = '%s'
  WHERE estid = %d
DML;
        $dml = sprintf($dml, $dados['estdsc'], $dados['subprgid'], $dados['esttitulo'] , $dados['estid']);
    } else { // -- insert
        $dml = <<<DML
INSERT INTO scrum.estoria(estdsc, subprgid, esttitulo)
  VALUES('%s', %d, '%s')
DML;
        $dml = sprintf($dml, $dados['estdsc'], $dados['subprgid'], $dados['esttitulo']);
    }
    global $db;
    $db->executar($dml);
    return $db->commit();
}

/**
 * Carrega e retorna os dados de uma est�ria.
 * @global cls_banco $db
 *      Conex�o com a base de dados
 * @param array $dados
 *      Dados da requisi��o para processamento.
 * @return boolean|array
 */
function carregarEstoria($dados)
{
    // -- Valida��o dos par�metros
    if (!isset($dados['estid']) || empty($dados['estid'])) {
        return false;
    }

    $query = <<<DML
SELECT *
  FROM scrum.estoria
    LEFT JOIN scrum.subprg spg USING(subprgid)
  WHERE estid = %d
DML;
    $query = sprintf($query, $dados['estid']);
    global $db;
    return $db->pegaLinha($query);
}

/**
 * Prepara os par�metros para serem inclusos na URL e filtrar a listagem de est�rias.
 * 
 * @param array $dados
 *      Dados para filtragem das est�rias.
 * @return string
 */
function filtrarEstoria($dados)
{
    return criaFiltroURI(
        array(
            'prgid',
            'subprgid',
            'esttitulo',
            'estdsc',
        ),
        $dados
    );
}

/**
 * Lista as est�rias cadastrados com base em filtros de busca.
 * 
 * @global cls_banco $db
 *      Conex�o com a base de dados
 * @param type $dados
 *      Dados de filtragem das est�rias. Par�metros enviados via $_GET.
 */
function listarEstorias($dados)
{
    if( $dados['subprgid'] == 'Selecione um subprograma') $dados['subprgid'] = '';
    
    $where = array();
    if (!empty($dados['prgid'])) {
        $where[] = sprintf("prgid = %d", $dados['prgid']);
    }
    if (!empty($dados['subprgid'])) {
        $where[] = sprintf("subprgid = %d", $dados['subprgid']);
    }
    if (!empty($dados['estdsc'])) {
        $where[] = sprintf("estdsc ILIKE '%%%s%%'", $dados['estdsc']);
    }
    if (!empty($dados['esttitulo'])) {
        $where[] = sprintf("esttitulo ILIKE '%%%s%%'", $dados['esttitulo']);
    }
    if (!empty($where)) {
        $where = 'WHERE ' . implode(' AND ', $where);
    } else {
        $where = '';
    }

    $sql = <<<DML
SELECT '&nbsp;&nbsp;<input type="image" src="../imagens/alterar.gif" onclick="carregarItem('
            || est.estid || ', \'estid\')" class="gui" title="Editar est�ria" />' AS codigo,
       spg.subprgdsc AS subprograma,
       est.esttitulo,
       CASE WHEN LENGTH(est.estdsc) > 150
              THEN SUBSTRING(est.estdsc, 1, 150) || '...'
            ELSE
              est.estdsc
       END AS descricao
  FROM scrum.estoria est
    INNER JOIN scrum.subprg spg USING(subprgid) {$where}
DML;

//    ver($sql , $where,$dados,d);
    
    global $db;
    $db->monta_lista($sql, array('&nbsp', 'Subprograma', 'Est�ria', 'Resumo da est�ria'), 20, 5, false, 'center', 'N');
}

/**
 * Consulta a lista de estorias para inser��o em um select.
 * 
 * @global cls_banco $db
 *      Conex�o com a base de dados
 * @param array $dados
  *      Dados para consulta de estorias.
 * @return type
 */
function jsonEstoria($dados)
{
    $sql = <<<DML
SELECT est.estid AS codigo,
       est.esttitulo AS descricao
  FROM scrum.estoria est
  WHERE est.subprgid = %d
DML;
    $sql = sprintf($sql, $dados['subprgid']);
    global $db;

    $result = $db->carregar($sql);
    foreach ($result as $key => &$item) {
        $item['descricao'] = utf8_encode($item['descricao']);
    }

    return $result;
}
