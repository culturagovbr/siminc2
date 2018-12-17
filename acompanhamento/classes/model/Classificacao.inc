<?php
/**
 * Classe de mapeamento da entidade acompanhamento.classificacao
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
 * Acompanhamento_Model_Classificacao
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_Classificacao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.classificacao';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'claid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'tipid' => array('tabela' => 'acompanhamento.tipo', 'pk' => 'tipid')
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'claid' => null,
        'cladsc' => null,
        'clastatus' => null,
        'tipid' => null,
        'clamedida' => null,
    );

}
