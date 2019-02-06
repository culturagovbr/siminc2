<?php
/**
 * Classe de abstração da tabela monitora.pi_complemento.
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
    
    const ORCAMENTO_CUSTEIO = 2;
    const ORCAMENTO_CAPITAL = 3;
    const FINANCEIRO_CUSTEIO = 4;
    const FINANCEIRO_CAPITAL = 5;

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