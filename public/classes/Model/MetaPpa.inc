<?php
/**
 * Classe de mapeamento da entidade public.metappa
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
 * Public_Model_Metappa
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Public_Model_MetaPpa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.metappa';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'mppid',
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
        'mppid' => null,
        'mppdsc' => null,
        'mppcod' => null,
        'mppnome' => null,
        'mppstatus' => null,
        'prsano' => null,
        'mppquantificavel' => null,
        'mppquantidade' => null,
        'mppanualizavel' => null,
    );
    
    public function recuperarSqlCombo($descricao = null, $where = [], $order = '')
    {
        $where['exercicio'] = !empty($where['exercicio']) ? $where['exercicio'] : $_SESSION['exercicio'];

        $join = '';
        if($where['suoid']){
            $suoid = $where['suoid'];
            if (is_array($where['suoid'])){
                $suoid = '';
                for ($i=0;$i<count($where['suoid']);$i++){
                    if ($where['suoid'][$i]!=''){
                        $suoid .= $where['suoid'][$i].', ';
                    }
                }
                $suoid = substr($suoid, 0, strlen($suoid)-2);
            }
            $join = "
                JOIN (
                    SELECT
                        smp.mppid 
                    FROM spo.subunidademetappa smp
                        JOIN public.vw_subunidadeorcamentaria suo ON suo.suoid = smp.suoid AND suo.prsano = '{$where['exercicio']}'
                    WHERE
                        suo.suoid in ({$suoid}) 
                UNION ALL
                    SELECT
                        mpp.mppid
                    FROM public.metappa mpp
                        LEFT JOIN spo.subunidademetappa smp ON smp.mppid = mpp.mppid
                    WHERE
                        mpp.prsano = '{$where['exercicio']}'       
                        AND smp.mppid is null
                ) smp ON smp.mppid = om.mppid
            ";
        }

        $sql = "
            SELECT DISTINCT
                m.mppid AS codigo,
                m.mppcod || ' - ' || m.mppnome AS descricao
            FROM public.metappa m
                JOIN public.objetivometappa om ON m.mppid = om.mppid
            $join
            WHERE
                m.mppstatus = 'A'
                AND m.prsano = '{$where['exercicio']}'
                AND om.oppid = ". (int)$where['oppid']. "
            ORDER BY
                descricao
        ";
        
        return $sql;
    }

    public function recuperarInformacoes($exercicio = null)
    {
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        $sql = "
            SELECT
                opm.oppid,
                opp.oppdsc,
                opp.oppcod,
                mpp.mppid,
                mpp.mppcod,
                mpp.mppnome,
                mpp.mppdsc,
                mpp.mppquantificavel,
                mpp.mppquantidade,
                mpp.mppanualizavel,
                CASE WHEN (SELECT count(1) FROM spo.subunidademetappa m WHERE m.mppid = mpp.mppid) = 0 THEN
                    TRUE
                WHEN (SELECT count(1) FROM spo.subunidademetappa m WHERE m.mppid = mpp.mppid) > 2 THEN
                    TRUE
                ELSE
                    FALSE
                END AS compartilhada,
                -- Cálculo para verificar quantidade anualizada: (valor previsto dividido por 4 e vezes o ano do monitoramento)
                mpp.mppquantidade / 4 * ((to_char(now(), 'YYYY')::int % 4) + 1) mppquantidadeanualizada
            FROM public.objetivometappa opm
                JOIN public.objetivoppa opp ON opp.oppid = opm.oppid
                JOIN public.metappa mpp ON mpp.mppid = opm.mppid
            WHERE
                opm.mppid = ". (int)$this->mppid. "
                AND opp.prsano = '". (int)$exercicio. "'";

        return $this->pegaLinha($sql);
    }

}//end Class
?>