<?php
/**
 * Classe de mapeamento da entidade acompanhamento.tipo
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
 * Acompanhamento_Model_Tipo
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_Tipo extends Modelo
{
    const K_TIPO_META_PPA       = 1;
    const K_TIPO_INDICADOR_PNC  = 2;
    const K_TIPO_PI             = 3;

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.tipo';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'tipid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'tipid' => null,
        'tipdsc' => null,
        'tipstatus' => null,
        'usucpf' => null,
        'tipdata' => null,
    );

}
