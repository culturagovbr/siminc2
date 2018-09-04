<?php

/**
 * Classe de mapeamento da entidade acompanhamento.atividade_pnc
 */
require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Acompanhamento_Model_Atividadepnc
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_Atividadepnc extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.atividade_pnc';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'atvid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'acpid' => array('tabela' => 'acompanhamento.acompanhamento_pnc', 'pk' => 'acpid'),
        'muncod' => array('tabela' => 'territorios.municipio', 'pk' => 'muncod')
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'atvid' => null,
        'acpid' => null,
        'atvtitulo' => null,
        'atvproduto' => null,
        'atvprodutoqtd' => null,
        'muncod' => null,
        'atvfonteinformacao' => null,
        'atvdatacadastro' => null,
        'atvstatus' => null
    );

}//end Class
?>