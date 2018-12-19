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
        'pedid'    => array('tabela' => 'alteracao.pedido',                    'pk' => 'pedid'),
        'ptrid'    => array('tabela' => 'monitora.ptres',                      'pk' => 'ptrid'),
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
        'pedid' => null,
        'ptrid' => null,
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
     * Busca array de funcionais selecionadas das Subunidades vinculadas ao pedido de alteração.
     *
     * @param int $pedid
     * @param int $exercicio Ano
     * @return array|string Lista com Funcionais
     * @since 28-11-2018
     */
    public function recuperaFuncionalSelecionada($pedid, $exercicio)
    {
        $sql = "
            SELECT
                ptr.ptrid AS codigo,
                ptr.funcional AS descricao
            FROM public.vw_subunidadeorcamentaria suo
                JOIN spo.ptressubunidade psu ON psu.suoid = suo.suoid
                JOIN monitora.vw_ptres ptr ON(
                    ptr.ptrid = psu.ptrid
                    AND ptr.ptrano = suo.prsano
		)
                JOIN alteracao.pedido_unidade pu ON(
                    suo.suoid = pu.suoid
                )
            WHERE
                suo.prsano = '". (int)$exercicio. "'
		AND suo.unofundo = FALSE
		AND suo.suostatus = 'A'
		AND pu.pedid = ". (int)$pedid. "
            GROUP BY
                ptr.ptrid,
                ptr.funcional
            ORDER BY
                ptr.funcional,
                ptr.ptrid
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
                    ctecod AS codigo,
                    ctecod AS descricao
                FROM public.categoriaeconomica
                WHERE
                    ctestatus = 'A'
                    AND ctecod in(3, 4)
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
        $sql = "
            SELECT DISTINCT
                rl.rlid,
                rl.ptrid,
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
                ptr.unicod,
                TRIM(aca.prgcod) || '.' || TRIM(aca.acacod) || '.' || TRIM(aca.loccod) || '.' || (CASE WHEN LENGTH(COALESCE(aca.acaobjetivocod, '-')) <= 0 THEN '-' ELSE COALESCE(aca.acaobjetivocod, '-') END) || '.' || (CASE WHEN LENGTH(COALESCE(ptr.plocod, '-')) <= 0 THEN '-' ELSE COALESCE(ptr.plocod, '-') END) AS funcional
            FROM alteracao.remanejamento_loa rl
                LEFT JOIN monitora.ptres ptr on rl.ptrid = ptr.ptrid
                LEFT JOIN monitora.acao aca on ptr.acaid = aca.acaid
                left join public.vw_subunidadeorcamentaria vs on ptr.unicod = vs.unocod
            WHERE
                vs.suoid IN(SELECT suoid FROM alteracao.pedido_unidade WHERE pedid = ". (int)$pedid. ")
                AND pedid = ". (int)$pedid. "
            ORDER BY
                1
        ";
        return $this->carregar($sql);
    }
    
    public function selecionaDadosFuncional($dados)
    {
        $sql = "SELECT 
                    ptr.ptrid,
                    ptr.unicod,
                    ptr.irpcod,
                    pis.pliselid
                FROM monitora.ptres ptr 
                LEFT JOIN monitora.pi_planointernoptres ppt ON ppt.ptrid = ptr.ptrid
                left join alteracao.plano_interno_selecionado pis on ppt.pliid = pis.pliid and pis.pedid = {$dados['pedid']}
               WHERE ptr.ptrid = {$dados['ptrid']}
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
