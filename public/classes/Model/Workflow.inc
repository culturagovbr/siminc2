<?php
/**
 * Classe de mapeamento da entidade workflow.estadodocumento
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25-10-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Public_Model_Workflow
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Public_Model_Workflow extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'workflow.estadodocumento';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'esdid',
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
        'esdid' => null,
        'tpdid' => null,
        'esdstatus' => null,
        'esdordem' => null,
        'esddsc' => null,
        'constantevirtual' => null,
        'esdprazo' => null,
        'esdatraso' => null,
        'esdatrazo' => null,
        'esdfim' => null,
    );

}
