<?php
/**
 * Classe de mapeamento da entidade acompanhamento.consulta_relatorio_filtros
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
 * Acompanhamento_Model_Consulta_Relatorio_Filtros
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_ConsultaRelatorioFiltros extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.consulta_relatorio_filtros';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'crfid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'crid' => array('tabela' => 'acompanhamento.consulta_relatorio', 'pk' => 'crid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'crfid' => null,
        'crid' => null,
        'dscampo' => null,
    );

    public function selecionaFiltros($crid){
        $sql = "select dscampo from acompanhamento.consulta_relatorio_filtros where crid = $crid";
        return $this->carregar($sql);
    }
    
    public function removeFiltros($crid){
        $sql = "delete from acompanhamento.consulta_relatorio_filtros where crid = $crid";
        $this->executar($sql);
    }
}
