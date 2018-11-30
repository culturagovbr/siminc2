<?php
/**
 * Classe de mapeamento da entidade alteracao.justificativa
 *
 * @category Class Justificativa de Alteracao Orcamentaria
 * @package  A1
 * @author   Douglasx <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-11-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Alteracao_Model_Justificativa
 *
 * @category Class Justificativa de Alteracao Orcamentaria
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-11-2018
 * @link     no link
 */
class Alteracao_Model_Justificativa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'alteracao.justificativa';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'jstid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'pedid' => array('tabela' => 'alteracao.pedido', 'pk' => 'pedid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'jstid' => null,
        'pedid' => null,
        'jstnecessidade' => null,
        'jstcausa' => null,
        'jstfinanciamento' => null,
        'jstfontes' => null,
        'jstmedida' => null,
        'jstlegislacao' => null,
        'jstoutros' => null,
    );

    /**
     * Busca a Justificativa pelo número do Pedido de Alteração Orçamentária.
     * @param $pedid
     * @return object
     * @since 30-11-2018
     */
    public function recuperaJustificativa($pedid)
    {
        $sql = "SELECT
                    jstid,
                    jstnecessidade,
                    jstcausa,
                    jstfinanciamento,
                    jstfontes,
                    jstmedida,
                    jstlegislacao,
                    jstoutros
                FROM alteracao.justificativa
                WHERE
                    pedid = {$pedid}
        ";

        return (object) $this->pegaLinha($sql);
    }

}
