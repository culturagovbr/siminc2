<?php
/**
 * Classe de mapeamento da entidade planacomorc.meta_estrategica
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25-03-2019
 * @link     no link
 */


require_once APPRAIZ . 'includes/classes/Modelo.class.inc';


/**
 * Planacomorc_Model_MetaEstrategica
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU siminc2.cultura.gov.br
 * @version  Release:
 * @link     no link
 */
class Planacomorc_Model_MetaEstrategica extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'planacomorc.meta_estrategica';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'meeid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'prsano' => array('tabela' => 'planacomorc.programacaoexercicio', 'pk' => 'prsano'),
        'obeid' => array('tabela' => 'planacomorc.objetivo_estrategico', 'pk' => 'obeid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'meeid' => null,
        'obeid' => null,
        'meecod' => null,
        'meenome' => null,
        'meecadastro' => null,
        'prsano' => null,
        'meestatus' => null,
    );

    /**
     * Recupera SQL de Metas Estratégicas.
     * @param $obeid
     * @param $suocod
     * @return array|string
     */
    public function recuperarSqlMeta($obeid, $suocod)
    {
        $sql = "
            SELECT DISTINCT
                mee.meeid AS codigo,
                mee.meecod || ' - ' || mee.meenome AS descricao
            FROM planacomorc.subunidademeta_estrategica AS sme
                LEFT JOIN planacomorc.meta_estrategica AS mee ON mee.meeid = sme.meeid
                LEFT JOIN public.vw_subunidadeorcamentaria AS suo ON suo.suoid = sme.suoid
            WHERE
                mee.meestatus = 'A'
                AND mee.obeid = '".(int)$obeid."'
                AND suo.suocod = '".(int)$suocod."'
            UNION ALL
            SELECT DISTINCT
                mee.meeid AS codigo,
                mee.meecod || ' - ' || mee.meenome AS descricao
            FROM planacomorc.meta_estrategica AS mee
                LEFT JOIN planacomorc.subunidademeta_estrategica AS sme ON mee.meeid = sme.meeid
            WHERE
                mee.meestatus = 'A'
                AND mee.obeid = '".(int)$obeid."'
                AND sme.suoid IS NULL
            ORDER BY
                descricao
        ";

        return $sql;
    }
}
