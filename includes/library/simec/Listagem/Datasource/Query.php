<?php
/**
 * Arquivo de implementa��o do datasource da listagem do tipo Query.
 *
 * $Id: Query.php 102680 2015-09-22 13:11:51Z maykelbraz $
 * @filesource
 * @package Simec\Listagem\Datasource
 */

/**
 * Implementa uma fonte de dados do tipo query.
 *
 * @package Simec\Listagem\Datasource
 */
class Simec_Listagem_Datasource_Query extends Simec_Listagem_Datasource
{
    /**
     * Valor m�ximo para o timeout da query no memcache.
     */
    const MAX_QUERY_TIMEOUT = 2592000;

    /**
     * @var \cls_banco Inst�ncia do banco de dados.
     */
    protected $db;

    /**
     * @var integer Define tempo de armazenamento em cache da query.
     * @see \cls_banco::carregar()
     */
    protected $queryTimeout = null;

    /**
     * Inicializa a base de dados.
     *
     * @global \cls_banco $db
     */
    public function __construct()
    {
        global $db;

        $this->db = $db;
    }

    /**
     * Limpar a string da query.
     *
     * @param string $query Query utilizada para carregar os dados.
     */
    protected function clean(&$query)
    {
        $query = trim($query);
        // -- Removendo ';' no final da query para evitar problemas com o count
        if (';' == substr($query, -1)) {
            $query = substr($query, 0, -1);
        }
    }

    /**
     * Carrega a fonte de dados.
     *
     * Op��es dispon�veis:
     * timout: Timeout de cache da query.
     *
     * @param string $source Query de consulta dos dados.
     * @param array $opcoes Op��es de configura��o extra da fonte de dados.
     * @throws Exception
     */
    public function setSource($source, array $opcoes = array())
    {
        $this->clean($source);

        if (empty($source)) {
            throw new Exception('A query da lista n�o deve ser vazia.');
        }

        $this->source = $source;

        if (array_key_exists('timeout', $opcoes)) {
            if (self::MAX_QUERY_TIMEOUT < $opcoes['timeout']) {
                throw new Exception('O tempo de cache da query n�o deve exceder 2592000.');
            }
            $this->queryTimeout = $opcoes['timeout'];
        }
    }

    /**
     * Retorna a query que originou as linhas do array ($source).
     *
     * @return string
     */
    public function getQuery()
    {
        return $this->getSource();
    }

    /**
     * Retorna o conjunto de registros da p�gina (ou todos os registros).
     *
     * @return array
     */
    public function getDados()
    {
        // -- Todos os dados, sem restri��es
        if ('all' == $this->getPaginaAtual()) {
            $query = $this->source;
        } else { // -- Apenas da p�gina atual
            $offset = $this->offset();
            $query = "{$this->source} OFFSET {$offset} LIMIT {$this->numRegistrosPorPagina}";
        }

        $resultado = $this->db->carregar($query, null, $this->queryTimeout);

        return $resultado?$resultado:array();
    }

    /**
     * Realiza a contagem dos registros atuais.
     *
     * @return int
     * @see Simec_Listagem_Datasource::getTotalRegistros()
     */
    protected function contaRegistros()
    {
        if (is_null($this->db)) {
            throw new Exception('N�o foi informada a conex�o com a base de dados para execu��o da query.');
        }

        $resultado = $this->db->pegaUm("SELECT COUNT(1) AS total FROM({$this->source}) contagem");
        return $resultado?$resultado:0;
    }
}
