<?php

/**
 * Classe de mapeamento da entidade acompanhamento.acompanhamentopncarquivo
 */

require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Acompanhamento_Model_Acompanhamentopncarquivo
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_Acompanhamentopncarquivo extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.acompanhamentopncarquivo';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'apaid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'acpid' => array('tabela' => 'acompanhamento.acompanhamento_pnc', 'pk' => 'acpid'),
        'arqid' => array('tabela' => 'public.arquivo', 'pk' => 'arqid')
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'apaid' => null,
        'acpid' => null,
        'arqid' => null
    );

}
