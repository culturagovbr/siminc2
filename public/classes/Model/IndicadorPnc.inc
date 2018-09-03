<?php
/**
 * Classe de mapeamento da entidade public.indicadorpnc
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
 * Public_Model_Indicadorpnc
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Public_Model_IndicadorPnc extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.indicadorpnc';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'ipnid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'mpnid' => array('tabela' => 'metapnc', 'pk' => 'mpnid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'ipnid' => null,
        'mpnid' => null,
        'ipndsc' => null,
        'ipnstatus' => null,
        'prsano' => null,
        'ipncod' => null,
    );
    
    /**
     * Busca por código do indicador(ipncod) e ano.
     * 
     * @param stdClass $filtros
     * @return sdtClass
     */
    public function buscarPorCodigo(stdClass $filtros){
        # Filtros
        $where = '';
        $where .= $filtros->ipncod? "\n AND ipncod = '". $filtros->ipncod. "'": NULL;
        $where .= $filtros->prsano? "\n AND prsano = '". $filtros->prsano. "'": NULL;
        
        $sql = "
            SELECT
                ipnid,
                mpnid,
                ipndsc,
                ipnstatus,
                prsano,
                ipncod
            FROM public.indicadorpnc
            WHERE
                ipnstatus = 'A'
                $where
        ";
        $resultado = $this->pegaLinha($sql);
        $indicador = $resultado? (object)$resultado: new stdClass();
        return $indicador;
    }
    
    public function recuperarSqlCombo($descricao = null, $where = [], $order = '')
    {
        $where['exercicio'] = !empty($where['exercicio']) ? $where['exercicio'] : $_SESSION['exercicio'];

        $sql = "
            SELECT
                ipnid AS codigo,
                ipncod || ' - ' || ipndsc AS descricao
            FROM public.indicadorpnc
            WHERE
                ipnstatus = 'A'
                AND prsano = '{$where['exercicio']}'
                AND mpnid = ". (int)$where['mpnid']. "
            ORDER BY
                ipncod
        ";
//ver($sql,d);
        return $sql;
    }

    public function recuperarIndicadores()
    {
        $sql = "select * 
                from public.indicadorpnc ipn
                        inner join public.metapnc mpn on mpn.mpnid = ipn.mpnid
                where ipn.ipnstatus = 'A'        
                and ipn.prsano = '{$_SESSION['exercicio']}'
                order by mpn.mpncod::int, ipn.ipncod";

        return $this->carregar($sql);
    }

    public function recuperarInformacoes($exercicio = null)
    {
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        $sql = "select  opm.oppid, opp.oppdsc, opp.oppcod, mpp.mppid, mpp.mppcod, mpp.mppnome, mpp.mppdsc, mpp.mppquantificavel, mpp.mppquantidade, 
                        -- Cálculo para verificar quantidade anualizada: (valor previsto dividido por 4 e vezes o ano do monitoramento)
                        mpp.mppquantidade / 4 * ((to_char(now(), 'YYYY')::int % 4)+1) mppquantidadeanualizada
                from public.objetivometappa opm
                        inner join public.objetivoppa opp on opp.oppid = opm.oppid
                        inner join public.metappa mpp on mpp.mppid = opm.mppid
                where opm.mppid = " . (int) $this->ipnid . "
                and opp.prsano = '$exercicio'";
ver($sql, d);
        return $this->pegaLinha($sql);
    }
}//end Class
?>