<?php
/**
 * Classe de mapeamento da entidade planejamento.cronograma_plano_interno.
 *
 * @version $Id$
 * @see Pi_PlanoInterno
 */

/**
 *
 */
require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 *
 */
class Pi_Cronograma extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "planejamento.cronograma_plano_interno";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pcrid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
        'pcrid' => null,
        'pliid' => null,
        'mescod' => null,
        'pcrvalor' => null,
        'crvid' => null,
    );

    public function recuperarCronogramas()
    {
        $sql = "
            SELECT
                * 
            FROM planacomorc.cronograma_valor cv
                JOIN planacomorc.cronograma c on c.croid = cv.croid
            ORDER BY
                crvid
        ";

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }
}
