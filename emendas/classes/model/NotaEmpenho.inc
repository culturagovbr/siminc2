<?php
/**
 * Classe de mapeamento da entidade emendas.nota_empenho
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO <rafael.carneiro@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 14-02-2019
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Emendas_Model_NotaEmpenho
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Emendas_Model_NotaEmpenho extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.nota_empenho';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'neid',
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
        'neid' => null,
        'benid' => null,
        'numnotaempenho' => 'null',
        'nestatus' => 'null',
    );

    public function montarRecuperarPorBeneficiario($benid){
        $sql = "
            SELECT ne.*
            FROM emendas.nota_empenho ne
            WHERE ne.benid = " . (int)$benid . "
              AND ne.nestatus = 'A'";

        return $this->carregar($sql);
    }

}
