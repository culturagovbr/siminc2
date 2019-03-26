<?php
/**
 * Classe de mapeamento da entidade planacomorc.diretriz_estrategica
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
 * Planacomorc_Model_DiretrizEstrategica
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU siminc2.cultura.gov.br
 * @version  Release:
 * @link     no link
 */
class Planacomorc_Model_DiretrizEstrategica extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'planacomorc.diretriz_estrategica';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'dieid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'prsano' => array('tabela' => 'planacomorc.programacaoexercicio', 'pk' => 'prsano'),
        'obeid' => array('tabela' => 'planacomorc.objetivo_estrategico', 'pk' => 'obeid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'dieid' => null,
        'diecod' => null,
        'dienome' => null,
        'diecadastro' => null,
        'prsano' => null,
        'diestatus' => null,
        'obeid' => null,
    );

}
