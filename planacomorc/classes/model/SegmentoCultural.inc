<?php
/**
 * Classe de mapeamento da entidade planejamento.segmento_cultural
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
 * Monitora_Model_Pi_niveletapaensino
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Planacomorc_Model_SegmentoCultural extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'planejamento.segmento_cultural';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'secid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'arceid' => array('tabela' => 'segmento_cultural', 'pk' => 'arceid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'secid' => null,
        'arceid' => null,
        'secdsc' => null,
        'secano' => null,
        'seccod' => null,
        'secstatus' => null,
    );

}
