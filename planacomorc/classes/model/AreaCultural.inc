<?php
/**
 * Classe de mapeamento da entidade planejamento.area_cultural
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-12-2017
 * @link     no link
 */

require_once APPRAIZ .'includes/classes/Modelo.class.inc';

/**
 * Planacomorc_Model_AreaCultural
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Planacomorc_Model_AreaCultural extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'planejamento.area_cultural';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'arceid',
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
        'arceid' => null,
        'arccod' => null,
        'arcdsc' => null,
        'arcano' => null,
        'arcstatus' => null,
    );

}
