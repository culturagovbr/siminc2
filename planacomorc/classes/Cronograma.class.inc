<?php
/**
 * Classe de abstra��o da tabela monitora.pi_complemento.
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
class Cronograma extends Modelo{

    const K_FISICO          = 1;
    const K_ORCAMENTARIO    = 2;
    const K_FINANCEIRO      = 3;


    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "planacomorc.cronograma";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "croid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
                                        'croid' => null,
                                        'crodsc' => null,
    );
}