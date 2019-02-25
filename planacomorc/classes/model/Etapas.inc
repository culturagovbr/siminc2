<?php
/**
 * Classe de mapeamento da entidade planacomorc.etapas
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 22-02-2019
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Planacomorc_Model_Etapas
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU siminc2.cultura.gov.br
 * @version  Release: 
 * @link     no link
 */
class Planacomorc_Model_Etapas extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'planacomorc.etapas';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'etaid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'pliid',
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'etaid' => null,
        'etadsc' => null,
        'etadata' => null,
        'pliid' => null,
        'etastatus' => null,

    );

    public function excluirPorPliid($pliid){
        $sql = "update ".$this->stNomeTabela." set etastatus = 'I' where pliid = '".$pliid."'";
        $this->executar($sql);
    }
}
