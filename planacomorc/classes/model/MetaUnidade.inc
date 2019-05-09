<?php
/**
 * Classe de mapeamento da entidade planacomorc.meta_unidade
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 22-02-2019
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Planacomorc_Model_MetaUnidade
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU siminc2.cultura.gov.br
 * @version  Release: 
 * @link     no link
 */
class Planacomorc_Model_MetaUnidade extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'planacomorc.meta_unidade';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'meuid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'dimeid' => array('tabela' => 'planacomorc.dimensao_estrategica', 'pk' => 'dimeid'),
        'obeid'  => array('tabela' => 'planacomorc.objetivo_estrategico', 'pk' => 'obeid'),
        'meeid'  => array('tabela' => 'planacomorc.meta_estrategica',     'pk' => 'meeid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'meuid' => null,
        'dimeid' => null,
        'obeid' => null,
        'meeid' => null,
        'meunome' => null,
        'meuquantificavel' => null,
        'meuquantidade' => null,
        'meuprioritario' => null,
        'meucadastro' => null,
        'meustatus' => null,
    );

}
