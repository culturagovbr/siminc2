<?php
/**
 * Classe de mapeamento da entidade territorios.municipio
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 22-11-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Territorios_Model_Municipio
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Territorios_Model_Municipio extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'territorios.municipio';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'muncod',
        'estuf',
        'muncod',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'miccod' => array('tabela' => 'territorios.microregiao', 'pk' => 'miccod'),
        'mescod' => array('tabela' => 'territorios.mesoregiao', 'pk' => 'mescod'),
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'muncod' => null,
        'estuf' => null,
        'estuf' => null,
        'miccod' => null,
        'mescod' => null,
        'mundescricao' => null,
        'munprocesso' => null,
        'muncodcompleto' => null,
        'munmedlat' => null,
        'munmedlog' => null,
        'munhemis' => null,
        'munaltitude' => null,
        'munmedarea' => null,
        'muncepmenor' => null,
        'muncepmaior' => null,
        'munmedraio' => null,
        'munmerid' => null,
        'muncodsiafi' => null,
        'munpopulacao' => null,
        'munpopulacaohomem' => null,
        'munpopulacaomulher' => null,
        'munpopulacaourbana' => null,
        'munpopulacaorural' => null,
    );

}
