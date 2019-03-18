<?php
/**
 * Classe de mapeamento da entidade public.iniciativappa
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-05-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Public_Model_Iniciativappa
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Public_Model_IniciativaPpa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.iniciativappa';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'ippid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'oppid' => array('tabela' => 'objetivoppa', 'pk' => 'oppid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'ippid' => null,
        'oppid' => null,
        'ippdsc' => null,
        'ippnome' => null,
        'ippcod' => null,
        'ippstatus' => null,
        'prsano' => null,
    );

    public function recuperarSqlCombo($descricao = null, $where = [], $order = '')
    {
        $where['exercicio'] = !empty($where['exercicio']) ? $where['exercicio'] : $_SESSION['exercicio'];

        $sql = "
            SELECT
                ippid AS codigo,
                ippcod || ' - ' || ippnome AS descricao
            FROM public.iniciativappa
            WHERE
                ippstatus = 'A'
                AND prsano = '{$where['exercicio']}'
                AND oppid = ". (int)$where['oppid']. "
            ORDER BY
                ippcod
        ";
        return $sql;
    }
}
