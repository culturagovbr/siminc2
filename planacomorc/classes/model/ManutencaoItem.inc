<?php
/**
 * Classe de mapeamento da entidade planejamento.manutencaoitem
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <teste@teste.com.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 09-04-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Planacomorc_Model_Manutencaoitem
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Planacomorc_Model_ManutencaoItem extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'planejamento.manutencao_item';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'maiid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'eqdid' => array('tabela' => 'pi_enquadramentodespesa', 'pk' => 'eqdid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'maiid' => null,
        'prsano' => null,
        'mainome' => null,
        'maidescricao' => null,
        'maistatus' => null,
        'eqdid' => null,
    );

    /**
     * Busca a lista com todos os itens de manutenção.
     *
     * @return array|string
     */
    public function recuperarLista()
    {
        global $db;

        $sql = "
            SELECT DISTINCT
                maiid AS codigo,
                mainome AS nome,
                maidescricao AS descricao,
                eqddsc AS enquadramento
            FROM planejamento.manutencao_item mai
                LEFT JOIN planejamento.enquadramento_despesa eqd ON(mai.eqdid = eqd.eqdid)
            WHERE
                prsano = '{$_SESSION['exercicio']}'
                AND maistatus = 'A'
            ORDER BY
                nome
        ";

        return $db->carregar($sql);
    }
}
