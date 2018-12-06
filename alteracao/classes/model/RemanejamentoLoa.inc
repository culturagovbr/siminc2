<?php
/**
 * Classe de mapeamento da entidade alteracao.remanejamento_loa
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 28-11-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Alteracao_Model_RemanejamentoLoa
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 28-11-2018
 * @link     no link
 */
class Alteracao_Model_RemanejamentoLoa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'alteracao.remanejamento_loa';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'rlid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'pliselid' => array('tabela' => 'alteracao.plano_interno_selecionado', 'pk' => 'pliselid'),
        'ctecod'   => array('tabela' => 'public.categoriaeconomica',           'pk' => 'ctecod'),
        'gndcod'   => array('tabela' => 'public.gnd',                          'pk' => 'gndcod'),
        'mapcod'   => array('tabela' => 'public.modalidadeaplicacao',          'pk' => 'mapcod'),
        'fonid'    => array('tabela' => 'public.fonterecurso',                 'pk' => 'fonid'),
        'idoid'    => array('tabela' => 'public.idoc',                         'pk' => 'idoid'),
        'iduid'    => array('tabela' => 'public.identifuso',                   'pk' => 'iduid'),
        'rpcod'    => array('tabela' => 'planacomorc.resultadoprimario',       'pk' => 'rpcod'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'rlid' => null,
        'pliselid' => null,
        'ctecod' => null,
        'gndcod' => null,
        'mapcod' => null,
        'fonid' => null,
        'idoid' => null,
        'iduid' => null,
        'rpcod' => null,
        'vldotacao' => null,
        'vlsuplementar' => null,
        'vlcancelado' => null,
        'vlsuplementarexcesso' => null,
        'vlsuplementarsuperavit' => null,
    );

    /**
     * Busca array de funcionais selecionadas.
     *
     * @param $pedid
     * @return array|string
     * @since 28-11-2018
     */
    public function recuperaFuncionalSelecionada($pedid)
    {
        $sql = "select distinct 
                    pis.pliselid AS codigo,
                    TRIM(aca.prgcod) || '.' || TRIM(aca.acacod) || '.' || TRIM(aca.loccod) || '.' || (
                        CASE WHEN LENGTH(COALESCE(aca.acaobjetivocod, '-')) <= 0 THEN
                            '-'
                        ELSE
                            COALESCE(aca.acaobjetivocod, '-')
                        END) || '.' || (
                        CASE WHEN LENGTH(COALESCE(ptr.plocod, '-')) <= 0 THEN
                            '-'
                        ELSE
                            COALESCE(ptr.plocod, '-')
                        END) || ' PI: ' || pis.pliid AS descricao
                    from public.vw_subunidadeorcamentaria vs 
                   inner join alteracao.pedido_unidade pu on vs.suoid = pu.suoid and pu.pedid = 3  and vs.suostatus = 'A' and vs.prsano = '2018'
                   inner JOIN monitora.ptres ptr ON (vs.unocod = ptr.unicod AND vs.prsano = ptr.ptrano and ptr.ptrstatus = 'A')
                     inner JOIN monitora.pi_planointernoptres ppt ON ptr.ptrid = ppt.ptrid
                   inner JOIN monitora.acao aca ON ptr.acaid = aca.acaid
                   inner join alteracao.plano_interno_selecionado pis on pu.pedid = pis.pedid
                   where pu.pedid = {$pedid}
         ";

        return $this->carregar($sql);
    }

    /** Retorna combo de Categoria.
     *
     * @return array|string
     * @since 28-11-2018
     */
    public function montaComboCategoria()
    {
        $sql = "SELECT
                    ctecod as codigo,
                    ctecod as descricao
                FROM public.categoriaeconomica
                WHERE
                    ctestatus = 'A'
                ORDER BY
                    descricao
                ";

        return $this->carregar($sql);
    }

    /** Retorna combo de GND.
     *
     * @return array|string
     * @since 28-11-2018
     */
    public function montaComboGND()
    {
        $sql = "SELECT
                    gndcod as codigo,
                    gndcod as descricao
                FROM public.gnd
                WHERE
                    gndstatus = 'A'
                ORDER BY
                    descricao
                ";

        return $this->carregar($sql);
    }

    /** Retorna combo de GND.
     *
     * @return array|string
     * @since 28-11-2018
     */
    public function montaComboModalidade()
    {
        $sql = "SELECT
                    mapcod as codigo,
                    mapcod as descricao
                FROM public.modalidadeaplicacao
                ORDER BY
                    descricao
                ";

        return $this->carregar($sql);
    }

    /** Retorna combo de Fonte.
     *
     * @return array|string
     * @since 28-11-2018
     */
    public function montaComboFonte()
    {
        $sql = "SELECT
                    fonid as codigo,
                    foncod as descricao
                FROM public.fonterecurso
                WHERE
                    fonstatus = 'A'
                ORDER BY
                    descricao
                ";

        return $this->carregar($sql);
    }

    /** Retorna combo de Idoc.
     *
     * @return array|string
     * @since 28-11-2018
     */
    public function montaComboIdoc()
    {
        $sql = "SELECT
                    idoid as codigo,
                    idocod as descricao
                FROM public.idoc
                WHERE
                    idostatus = 'A'
                ORDER BY
                    descricao
                ";

        return $this->carregar($sql);
    }

    /** Retorna combo de Iduso.
     *
     * @return array|string
     * @since 28-11-2018
     */
    public function montaComboIduso()
    {
        $sql = "SELECT
                    iduid as codigo,
                    iducod as descricao
                FROM public.identifuso
                WHERE
                    idustatus = 'A'
                ORDER BY
                    descricao
                ";

        return $this->carregar($sql);
    }

    /** Retorna combo de RP.
     *
     * @return array|string
     * @since 28-11-2018
     */
    public function montaComboRP()
    {
        $sql = "SELECT
                    rpcod as codigo,
                    rpcod as descricao
                FROM planacomorc.resultadoprimario
                ORDER BY
                    descricao
                ";

        return $this->carregar($sql);
    }
    
    public function recuperarTodosPorPedido($pedid)
    {
        $sql = "select rl.rlid,
                       rl.pliselid,
                       rl.ctecod,
                       rl.gndcod,
                       rl.mapcod,
                       rl.fonid,
                       rl.idoid,
                       rl.idoid,
                       rl.iduid,
                       rl.rpcod,
                       rl.vldotacao,
                       rl.vlsuplementar,
                       rl.vlcancelado,
                       rl.vlsuplementarexcesso,
                       rl.vlsuplementarsuperavit,
                       ptr.unicod
                  from alteracao.remanejamento_loa rl
                 inner join alteracao.plano_interno_selecionado pis
                    on rl.pliselid = pis.pliselid
                    JOIN monitora.pi_planointerno pli ON pis.pliid = pli.pliid
                    LEFT JOIN monitora.pi_planointernoptres ppt ON pis.pliid = ppt.pliid
                    LEFT JOIN monitora.ptres ptr ON (ppt.ptrid = ptr.ptrid
                            AND pli.pliano = ptr.ptrano)
                    LEFT JOIN monitora.acao aca ON ptr.acaid = aca.acaid                    
                 where pedid = ".$pedid;
        return $this->carregar($sql);
    }
    
    public function selecionaDadosFuncional($pliselid)
    {
        $sql = "SELECT 
                    ptr.ptrid,
                    ptr.unicod,
                    ptr.irpcod
                FROM
                    alteracao.plano_interno_selecionado pis
                    JOIN monitora.pi_planointerno pli ON pis.pliid = pli.pliid
                    LEFT JOIN monitora.pi_planointernoptres ppt ON pis.pliid = ppt.pliid
                    LEFT JOIN monitora.ptres ptr ON (ppt.ptrid = ptr.ptrid
                            AND pli.pliano = ptr.ptrano)
                    LEFT JOIN monitora.acao aca ON ptr.acaid = aca.acaid
                WHERE
                    pis.pliselid = {$pliselid}
         ";

        return $this->pegaLinha($sql);
    } 
    
    public function selecionaDotacao($dados){
        $sql = "SELECT DISTINCT coalesce(SUM(dotinicialsiafi::NUMERIC),0) AS dotacao_inicial
                  FROM wssof.ws_execucaoorcamentariadto exo
                 WHERE anoexercicio = '".$_SESSION['exercicio']."' -- EXERCICIO
                   AND unidadeorcamentaria = '".$dados['unicod']."' -- UNOCOD
                   AND programa = '".$dados['prgcod']."' -- PRGCOD
                   AND acao = '".$dados['acacod']."' -- ACACOD
                   AND localizador = '".$dados['loccod']."' -- LOCCOD
                   AND planoorcamentario = '".$dados['plocod']."' -- PLOCOD
                   AND SUBSTR(natureza::TEXT, 1, 4) = '".$dados['natureza']."' -- CATEGORIA+GND+MODALIDADE";
        $dados = $this->carregar($sql);
        return $dados[0]['dotacao_inicial'];
    }
}
