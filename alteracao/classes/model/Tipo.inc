<?php
/**
 * Classe de mapeamento da entidade alteracao.tipo
 *
 * @category Class Tipo de Alteracao Orcamentaria
 * @package  A1
 * @author   Douglasx <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 24-10-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Alteracao_Model_Tipo
 *
 * @category Class Tipo de Alteracao Orcamentaria
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 24-10-2018
 * @link     no link
 */
class Alteracao_Model_Tipo extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'alteracao.tipo';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'tpaid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(

    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'tpaid' => null,
        'tpacod'  => null,
        'tpadsc'  => null,
        'tpaobs'  => null,
        'tpastatus'  => null,
        'tpafluxo'  => null,
    );

    /**
     * Consulta se o código inserido existe e retorna a primeira coluna do primeiro registro.
     *
     * @return array
     */
    public function carregarPorCodigo()
    {
        $sql = "SELECT
                    tpaid,
                    tpacod
                FROM alteracao.tipo
                WHERE
                    tpastatus = 'A'
                    AND tpacod = '" . $this->tpacod . "'
                    AND tpaid <> '" . (int)$this->tpaid . "'
        ";

        return $this->pegaLinha($sql);
    }

    /**
     * Recupera todas os registos.
     *
     * @return array
     */
    public function recuperarTodos()
    {
        $sql = "SELECT DISTINCT
                    tpaid,
                    tpacod,
                    tpadsc,
                    tpaobs,
                    CASE tpafluxo
                        WHEN 'I' THEN 'Interno'
                        ELSE 'Externo'
                    END AS tpafluxo
                FROM alteracao.tipo
                WHERE
                    tpastatus = 'A'
                ORDER BY
                    tpadsc
        ";

        return $this->carregar($sql);
    }

}
