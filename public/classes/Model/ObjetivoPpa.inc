<?php
/**
 * Classe de mapeamento da entidade public.objetivoppa
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-05-2017
 * @link     no link
 */


require_once APPRAIZ . 'includes/classes/Modelo.class.inc';


/**
 * Public_Model_Objetivoppa
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Public_Model_ObjetivoPpa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.objetivoppa';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'oppid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'oppid' => null,
        'prsano' => null,
        'oppdsc' => null,
        'oppnome' => null,
        'oppcod' => null,
        'oppstatus' => null,
    );

    public function recuperarSqlCombo($descricao = null, $where = [], $order = '')
    {
        $where['exercicio'] = !empty($where['exercicio']) ? $where['exercicio'] : $_SESSION['exercicio'];

        $sql = "
            SELECT
                oppid AS codigo,
                oppcod || ' - ' || oppnome AS descricao,
                prsano
            FROM public.objetivoppa
            WHERE
                oppstatus = 'A'
                AND prsano = '".(int)$where['exercicio']."'
            ORDER BY
                oppcod
        ";
        return $sql;
      }

    /**
     * Busca o código do objetivo(oppid) por código da funcional.
     *
     * @param integer $ptrid
     */
    public function buscarOppidPorFuncional($ptrid) {
        $sql = "
            SELECT DISTINCT
                o.oppid
            FROM monitora.ptres ptr
                JOIN monitora.acao aca ON ptr.acaid = aca.acaid AND ptr.ptrano = aca.prgano
                JOIN public.objetivoppa o ON o.oppcod = aca.acaobjetivocod AND o.prsano = aca.prgano
            WHERE
                ptrid = ". (int)$ptrid;
        return $this->pegaUm($sql);
    }
    
    public function recuperarSqlComboPorOppcod($where = []){
        $where['exercicio'] = !empty($where['exercicio']) ? $where['exercicio'] : $_SESSION['exercicio'];

        $sql = "
            SELECT
                oppcod AS codigo,
                oppcod || '-' || oppnome AS descricao
            FROM public.objetivoppa
            WHERE
                oppstatus = 'A'
                AND prsano = '{$where['exercicio']}'
            ORDER BY
                oppcod";
        return $sql;
    }

}
