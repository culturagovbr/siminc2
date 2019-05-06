<?php
/**
 * Classe de abstração da tabela monitora.pi_planointerno.
 *
 * @version $Id$
 * @see Pi_PlanoInterno
 */

/**
 *
 */
require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 *
 */
class Pi_PlanoInterno extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "monitora.pi_planointerno";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pliid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'pliid' => null,
									  	'arceid' => null,
									  	'eqdid' => null,
									  	'secid' => null,
									  	'capid' => null,
									  	'sbaid' => null,
									  	'obrid' => null,
									  	'plisituacao' => null,
									  	'plititulo' => null,
									  	'plidata' => null,
									  	'plistatus' => null,
									  	'plicodsubacao' => null,
									  	'plicod' => null,
									  	'plilivre' => null,
									  	'plidsc' => null,
									  	'usucpf' => null,
									  	'unicod' => null,
									  	'ungcod' => null,
									  	'pliano' => null,
									  );
}