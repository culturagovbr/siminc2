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
    );

}
