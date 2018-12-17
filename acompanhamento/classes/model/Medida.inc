<?php
/**
 * Classe de mapeamento da entidade acompanhamento.medida
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 23-01-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Acompanhamento_Model_Medida
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_Medida extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.medida';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'medid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'claid' => array('tabela' => 'acompanhamento.classificacao', 'pk' => 'claid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'medid' => null,
        'meddsc' => null,
        'medstatus' => null,
        'claid' => null,
    );

}
