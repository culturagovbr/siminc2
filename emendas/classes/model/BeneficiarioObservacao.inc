<?php
/**
 * Classe de mapeamento da entidade emendas.beneficiario_observacao
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-11-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Emendas_Model_BeneficiarioObservacao
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Emendas_Model_BeneficiarioObservacao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.beneficiario_observacao';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'bobid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'benid' => array('tabela' => 'emendas.beneficiario', 'pk' => 'benid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'bobid' => null,
        'bobdsc' => null,
        'bobdata' => null,
        'usucpf' => null,
        'bobstatus' => null,
        'benid' => null,
    );

    public function montarRecuperarPorUnidade($benid){
        $sql = "
            SELECT DISTINCT
                bh.*
            FROM emendas.beneficiario_observacao bh
                LEFT JOIN emendas.usuarioresponsabilidade ur ON bh.usucpf = ur.usucpf
            WHERE
                bh.bobstatus = 'A'
                AND bh.benid = " . (int)$benid . " 
            ORDER BY
                bobdata desc
        ";

        return $sql;
    }

    public function recuperarPorUnidade($benid){
        $sql = $this->montarRecuperarPorUnidade($benid);
        return $this->carregar($sql);
    }

}
