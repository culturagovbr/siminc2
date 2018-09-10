<?php
/**
 * Classe de mapeamento da entidade emendas.beneficiariolocalizacao
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-11-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Emendas_Model_BeneficiarioLocalizacao
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Emendas_Model_BeneficiarioLocalizacao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.beneficiariolocalizacao';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'bloid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'muncod' => array('tabela' => 'territorios.municipio', 'pk' => 'muncod'),
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
        'paiid' => array('tabela' => 'territorios.pais', 'pk' => 'paiid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'bloid' => null,
        'benid' => null,
        'paiid' => null,
        'estuf' => null,
        'muncod' => null,
    );

    public function recuperarPorBeneficiario($benid){
        $sql = "
            SELECT
                *
            FROM emendas.beneficiariolocalizacao
            WHERE
                benid = ". (int)$benid;

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];

        $aLocalizacoes = ['paiid'=>[], 'estuf'=>[], 'muncod'=>[]];
        foreach($dados as $dado){
            if($dado['paiid']){ $aLocalizacoes['paiid'][] = $dado['paiid']; }
            if($dado['estuf']){ $aLocalizacoes['estuf'][] = $dado['estuf']; }
            if($dado['muncod']){ $aLocalizacoes['muncod'][] = $dado['muncod']; }
        }

        return $aLocalizacoes;
    }

}//end Class
?>