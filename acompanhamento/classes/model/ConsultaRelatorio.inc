<?php
/**
 * Classe de mapeamento da entidade acompanhamento.consulta_relatorio
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO<rafael.carneiro@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 11-10-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Acompanhamento_Model_Consulta_Relatorio
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_ConsultaRelatorio extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.consulta_relatorio';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'crid',
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
        'crid' => null,
        'tprel' => null,
        'dtcriacao' => null,
        'usucpf' => null,
        'norelatorio' => null,
    );
    
    public function selecionaConsulta($crid){
        $sql = "select norelatorio, tprel
                  from acompanhamento.consulta_relatorio
                 where crid = $crid";
        return $this->pegaLinha($sql);        
    }
}
