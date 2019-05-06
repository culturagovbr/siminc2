<?php
/**
 * Classe de mapeamento da entidade planejamento.segmento_cultural
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-12-2017
 * @link     no link
 */

require_once APPRAIZ .'includes/classes/Modelo.class.inc';

/**
 * Monitora_Model_Pi_niveletapaensino
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Monitora_Model_PiNivelEtapaEnsino extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'planejamento.segmento_cultural';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'secid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'arceid' => array('tabela' => 'pi_modalidadeensino', 'pk' => 'arceid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'secid' => null,
        'needsc' => null,
        'neeano' => null,
        'neestatus' => null,
        'arceid' => null,
        'neecod' => null,
    );

    /**
     * Retorna os Segmentos por Àrea Cultural.
     *
     * @param $arceid
     * @return array
     * @access public
     */
    public function RecuperaSegmentoPorArea($arceid)
    {

        $sql ="SELECT
                    secid,
                    needsc,
                    arceid,
                    neecod
                FROM
                    planejamento.segmento_cultural
                WHERE
                    arceid = $arceid
                    AND neestatus = 'A'
        ";

        return $this->carregar($sql);
    }

}
