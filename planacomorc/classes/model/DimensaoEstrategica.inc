<?php
/**
 * Classe de mapeamento da entidade planacomorc.dimensao_estrategica
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25-03-2019
 * @link     no link
 */


require_once APPRAIZ . 'includes/classes/Modelo.class.inc';


/**
 * Planacomorc_Model_DimensaoEstrategica
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU siminc2.cultura.gov.br
 * @version  Release:
 * @link     no link
 */
class Planacomorc_Model_DimensaoEstrategica extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'planacomorc.dimensao_estrategica';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'dimeid',
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
        'dimeid' => null,
        'dimenome' => null,
        'dimecadastro' => null,
        'dimestatus' => null,
    );

}
