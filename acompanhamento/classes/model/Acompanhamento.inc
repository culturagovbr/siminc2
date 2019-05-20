<?php

/**
 * Classe de mapeamento da entidade acompanhamento.acompanhamento
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 23-01-2018
 * @link     no link
 */

require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Acompanhamento_Model_Acompanhamento
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_Acompanhamento extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.acompanhamento';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'acoid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'prsano' => array('tabela' => 'acompanhamento.programacaoexercicio', 'pk' => 'prsano'),
        'janid' => array('tabela' => 'acompanhamento.janela', 'pk' => 'janid'),
        'pliid' => array('tabela' => 'pi_planointerno', 'pk' => 'pliid'),
        'mppid' => array('tabela' => 'metappa', 'pk' => 'mppid'),
        'claid' => array('tabela' => 'acompanhamento.classificacao', 'pk' => 'claid'),
        'meuid' => array('tabela' => 'planacomorc.meta_unidade', 'pk' => 'meuid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'acoid' => null,
        'meuid' => null,
        'acoquantidade' => null,
        'acodata' => null,
        'acoanalise' => null,
        'claid' => null,
        'acoprovidencias' => null,
        'suoid' => null,
        'mppid' => null,
        'pliid' => null,
        'janid' => null,
        'prsano' => null,
        'acostatus' => null,
    );

    public function recuperarMetasPlanejamentoEstrategico($suoid, $exercicio = null)
    {
        if (!$suoid) return [];
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        $sql = "
            SELECT
                meu.*,
                aco.acoid,
                smu.suoid,
                aco.acoquantidade
            FROM planacomorc.meta_unidade AS meu
            JOIN planacomorc.subunidade_meta_unidade AS smu ON smu.meuid = meu.meuid
                LEFT JOIN acompanhamento.acompanhamento AS aco ON(
                    aco.meuid = meu.meuid
                    AND aco.suoid = smu.suoid
                    AND acostatus = 'A'
                )
            WHERE
                meu.meustatus = 'A'
                AND smu.suoid = '{$suoid}'
            UNION
            SELECT
                meu.*,
                aco.acoid,
                smu.suoid,
                aco.acoquantidade
            FROM planacomorc.meta_unidade AS meu
                LEFT JOIN planacomorc.subunidade_meta_unidade AS smu ON smu.meuid = meu.meuid
                LEFT JOIN acompanhamento.acompanhamento aco ON(
                    aco.meuid = meu.meuid
                    AND aco.suoid = '{$suoid}'
                    AND acostatus = 'A'
                )
            WHERE
                meu.meustatus = 'A'
                AND smu.suoid IS NULL
            ORDER BY
                meunome
        ";
        return $this->carregar($sql);
    }

    public function getMetasPpa($suoid, $exercicio = null)
    {
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        if($suoid){
            $sql = "select mpp.*, aco.acoid, smp.suoid, aco.acoquantidade
                    from public.metappa mpp
                            inner join spo.subunidademetappa smp on smp.mppid = mpp.mppid
                            left join acompanhamento.acompanhamento aco on aco.mppid = mpp.mppid and aco.suoid = smp.suoid and acostatus = 'A'
                    where mpp.prsano = '$exercicio'
                    and mpp.mppstatus = 'A'
                    and smp.suoid = " . (int) $suoid . "
                    union
                    select mpp.*, aco.acoid, smp.suoid, aco.acoquantidade
                    from public.metappa mpp
                            left join spo.subunidademetappa smp on smp.mppid = mpp.mppid
                            left join acompanhamento.acompanhamento aco on aco.mppid = mpp.mppid and aco.suoid = $suoid and acostatus = 'A'
                    where mpp.prsano = '$exercicio'
                    and mpp.mppstatus = 'A'
                    and smp.suoid is null
                    and mpp.mppcod != '0'
                    order by mppcod, mppdsc";

//            ver($sql, d);
            
            return $this->carregar($sql);
        }

        return [];
    }
    
    /**
     * Retorna lista de nomes de colunas que receberão formatação de moeda
     * 
     * @return array
     */
    public static function montarColunasFormatoMoedaRelatorioPPA(){
        $listaColunaMoeda = array(
            'mppquantidade',
            'acoquantidade',
            'previsto',
            'provisionado',
            'empenhado',
            'liquidado',
            'pago',
        );
        return $listaColunaMoeda;
    }

    /**
     * Retorna lista de nomes de colunas do relatório de monitoramento PPA
     * 
     * @return array
     */
    public static function montarColunasRelatorioPPA(){
        $listaCabecalho = array(
            'Código da UO',
            'Nome da UO',
            'Código da Unidade',
            'Nome da Unidade',
            'Janela',
            'Data de início da Janela',
            'Data de encerramento da Janela',
            'Código do Objetivo',
            'Nome do Objetivo',
            'Código da Meta',
            'Nome da Meta',
            'Descrição da Meta',
            'Anualizável',
            'Previsto Anualizável',
            'Meta quantificável',
            'Previsto PPA',
            'Quantidade alcançada',
            'Qtd PIs associados à Meta',
            'Percentual de empenho',
            'Análise Situacional',
            'Dt. Referência',
            'Classificação da Meta',
            'Medidas a serem adotadas',
            'Detalhamento das providências a serem adotadas'
        );
        return $listaCabecalho;
    }
    
    public function consultarRelatorioAcompanhamentoPPA(stdClass $param){
        $where = "";
//        $where .= $param->suoid? "\n AND smp.suoid = ". (int)$param->suoid: '';
        $where .= $param->unicod? "\n AND suo.unocod = '". $param->unicod. "'": '';
        $where .= $param->suocod? "\n AND suo.suocod = '". $param->suocod. "'": '';
        $where .= $param->janid? "\n AND aco.janid = ". (int)$param->janid: '';
        $where .= $param->oppid? "\n AND om.oppid = ". (int)$param->oppid: '';
        $where .= $param->mppid? "\n AND mpp.mppid = ". (int)$param->mppid: '';
        
        $sql = "
            SELECT DISTINCT
                suo.unocod,
                suo.unonome,
                suo.suocod,
                suo.suonome,
                jan.jandsc,
                TO_CHAR(jan.janinicio, 'dd/mm/yyyy') AS janinicio,
                TO_CHAR(jan.janfim, 'dd/mm/yyyy') AS janfim,
                opp.oppcod,
                opp.oppnome,
                mpp.mppcod,
                mpp.mppnome,
                mpp.mppdsc,
                CASE WHEN mpp.mppanualizavel IS TRUE THEN
                    'Sim'
		WHEN mpp.mppanualizavel IS FALSE THEN
                    'Não'
		ELSE
                    NULL
		END AS meta_anualizavel,
		ROUND((mpp.mppquantidade / 4 * ((to_char(now(), 'YYYY')::int % 4) + 1))::NUMERIC, 2) AS mppquantidadeanualizada,
                CASE WHEN mpp.mppquantificavel IS TRUE THEN
                    'Sim'
		WHEN mpp.mppquantificavel IS FALSE THEN
                    'Não'
		ELSE
                    NULL
		END AS meta_quantificavel,
                mpp.mppquantidade AS previsto_ppa,
                aco.acoquantidade AS quantidade_alcancada,
                info_pi.qtd AS qtd_pis,
		ROUND((CASE WHEN info_pi.previsto IS NOT NULL THEN
			(info_pi.empenhado * 100) / info_pi.previsto
		ELSE
			0
		END)::NUMERIC, 2) AS percentual,
		aco.acoanalise,
		TO_CHAR(aco.acodata, 'dd/mm/yyyy') AS acodata,
		cla.cladsc,
		med.meddsc,
		aco.acoprovidencias
            FROM public.metappa mpp
                LEFT JOIN spo.subunidademetappa smp ON smp.mppid = mpp.mppid
                JOIN acompanhamento.acompanhamento aco ON(
                    aco.mppid = mpp.mppid
                    AND acostatus = 'A'
                )
                JOIN public.vw_subunidadeorcamentaria suo ON aco.suoid = suo.suoid
                JOIN public.objetivometappa om ON mpp.mppid = om.mppid -- SELECT * FROM public.objetivometappa
                JOIN public.objetivoppa opp ON(
                    om.oppid = opp.oppid
                    AND opp.oppstatus = 'A'
                    AND opp.prsano = mpp.prsano
                    AND opp.oppcod != '0'
                ) -- SELECT * FROM public.objetivoppa
                LEFT JOIN acompanhamento.janela jan ON aco.janid = jan.janid -- SELECT * FROM acompanhamento.janela -- SELECT * FROM acompanhamento.tipo
                LEFT JOIN acompanhamento.classificacao cla ON aco.claid = cla.claid
                LEFT JOIN acompanhamento.acompanhamentomedida acm ON aco.acoid = acm.acoid
		LEFT JOIN acompanhamento.medida med ON acm.medid = med.medid
		LEFT JOIN(
		    SELECT
			pli.pliano,
			pic.mppid,
			suo.suoid,
			count(pli.pliid) qtd,
			sum(coalesce(pic.picvalorcapital, 0)) picvalorcapital,
			sum(coalesce(pic.picvalorcusteio, 0)) picvalorcusteio,
			sum(coalesce(pic.picvalorcapital, 0) + coalesce(pic.picvalorcusteio, 0)) previsto,
			sum(coalesce(sex.vlrautorizado, 0)) AS provisionado,
			sum(coalesce(sex.vlrempenhado, 0)) AS empenhado,
			sum(coalesce(sex.vlrliquidado, 0)) AS liquidado,
			sum(coalesce(sex.vlrpago, 0)) AS pago
		    FROM monitora.pi_planointerno pli
			JOIN planacomorc.pi_complemento pic ON pic.pliid = pli.pliid
			JOIN public.vw_subunidadeorcamentaria suo ON(
			    suo.unocod = pli.unicod
			    AND suo.suocod = pli.ungcod
			    AND suo.prsano = pli.pliano
			)
			LEFT JOIN (
			    SELECT
				sex.exercicio,
				sex.plicod,
				sum(sex.vlrautorizado) vlrautorizado,
				sum(sex.vlrempenhado) vlrempenhado,
				sum(sex.vlrliquidado) vlrliquidado,
				sum(sex.vlrpago) vlrpago
			    FROM
				spo.siopexecucao sex
			    WHERE
				coalesce(sex.plicod, '') != ''
			    GROUP BY
				sex.exercicio,
				sex.plicod) sex ON sex.plicod = pli.plicod
			    AND sex.exercicio = pli.pliano
			WHERE
			    pli.plistatus = 'A'
		    GROUP BY
			pli.pliano,
			pic.mppid,
			suo.suoid
		) info_pi ON(
                    info_pi.pliano = mpp.prsano
                    AND info_pi.mppid = mpp.mppid
                    AND info_pi.suoid = suo.suoid
		)
            WHERE
                mpp.prsano = '". (int)$param->exercicio. "'
                AND mpp.mppstatus = 'A'
                $where
            ORDER BY
                mppcod,
                mppdsc
        ";
//ver($sql,d);
        $listaResultado = $this->carregar($sql);

        return $listaResultado? $listaResultado: array();
    }

    public function getMetasPpaRelatorio(stdClass $param){
        $where = "";
//        $where .= $param->suoid? "\n AND smp.suoid = ". (int)$param->suoid: '';
        $where .= $param->unicod? "\n AND suo.unocod = '". $param->unicod. "'": '';
        $where .= $param->suocod? "\n AND suo.suocod = '". $param->suocod. "'": '';
        $where .= $param->janid? "\n AND aco.janid = ". (int)$param->janid: '';
        $where .= $param->oppid? "\n AND om.oppid = ". (int)$param->oppid: '';
        $where .= $param->mppid? "\n AND mpp.mppid = ". (int)$param->mppid: '';

        $sql = "
            SELECT DISTINCT
                aco.acoid,
                aco.suoid,
                aco.janid,
                mpp.mppid,
                mpp.mppdsc,
                mpp.mppcod,
                mpp.mppnome,
                mpp.mppquantificavel,
                mpp.mppquantidade,
                mpp.mppanualizavel,
                aco.acoquantidade
            FROM public.metappa mpp
                LEFT JOIN spo.subunidademetappa smp ON smp.mppid = mpp.mppid
                JOIN acompanhamento.acompanhamento aco ON(
                    aco.mppid = mpp.mppid
--                    AND aco.suoid = smp.suoid
                    AND acostatus = 'A'
                )
                JOIN public.vw_subunidadeorcamentaria suo ON aco.suoid = suo.suoid
                JOIN public.objetivometappa om ON mpp.mppid = om.mppid
            WHERE
                mpp.prsano = '". (int)$param->exercicio. "'
                AND mpp.mppstatus = 'A'
                $where
            ORDER BY
                aco.janid,
                mppcod,
                mppdsc
        ";
//ver($sql,d);
        $listaResultado = $this->carregar($sql);

        return $listaResultado? $listaResultado: array();
    }

    public function getMetasPpaComPi($suoid, $janid = null, $exercicio = null)
    {
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        if($suoid){
            $sql = "select  ac.mppid, ac.mppdsc, ac.mppcod, ac.mppnome, ac.mppstatus, ac.prsano, ac.mppquantificavel, ac.mppquantidade, ac.acoid, ac.suoid, ac.acoquantidade,
                            case 
                                when  (select count(1) from spo.subunidademetappa m where m.mppid = ac.mppid) = 0 then true
                                when  (select count(1) from spo.subunidademetappa m where m.mppid = ac.mppid) > 2 then true
                                else false      
                            end as compartilhada,
                            count(pic.pliid) pis, sum(pic.picquantidade) picquantidade
                    from (
                            select mpp.mppid, mpp.mppdsc, mpp.mppcod, mpp.mppnome, mpp.mppstatus, mpp.prsano, mpp.mppquantificavel, mpp.mppquantidade, aco.acoid, smp.suoid, aco.acoquantidade
                            from public.metappa mpp
                                inner join spo.subunidademetappa smp on smp.mppid = mpp.mppid
                                left join acompanhamento.acompanhamento aco on aco.mppid = mpp.mppid and aco.suoid = smp.suoid and acostatus = 'A' and aco.janid = " . (int) $janid . "
                            where mpp.prsano = '$exercicio'
                            and mpp.mppstatus = 'A'
                            and smp.suoid = $suoid
                            union
                            select mpp.mppid, mpp.mppdsc, mpp.mppcod, mpp.mppnome, mpp.mppstatus, mpp.prsano, mpp.mppquantificavel, mpp.mppquantidade, aco.acoid, $suoid, aco.acoquantidade
                            from public.metappa mpp
                                left join spo.subunidademetappa smp on smp.mppid = mpp.mppid
                                left join acompanhamento.acompanhamento aco on aco.mppid = mpp.mppid and aco.suoid = $suoid and acostatus = 'A' and aco.janid = " . (int) $janid . "
                            where mpp.prsano = '$exercicio'
                            and mpp.mppstatus = 'A'
                            and smp.suoid is null
                            and mpp.mppcod != '0'
                    ) ac
                            left join public.vw_subunidadeorcamentaria suo on suo.suoid = ac.suoid
                            left join monitora.pi_planointerno pli on pli.ungcod = suo.suocod and pli.unicod = suo.unocod        
                            left join planacomorc.pi_complemento pic on pic.pliid = pli.pliid and pic.mppid = ac.mppid        
                    group by ac.mppid, ac.mppdsc, ac.mppcod, ac.mppnome, ac.mppstatus, ac.prsano, ac.mppquantificavel, ac.mppquantidade, ac.acoid, ac.suoid, ac.acoquantidade, compartilhada
                    order by mppcod, mppdsc        
                    ;";

//            ver($sql, d);

            return $this->carregar($sql);
        }

        return [];
    }

    public function getIndicadoresPnc(stdClass $filtro)
    {
        $listaIndicadoresPnc = array();
        if($filtro->suoid){
            $sql = "
                SELECT
                    ipn.*,
                    aco.acoid,
                    sic.suoid,
                    aco.acoquantidade,
                    mpn.mpncod::int,
                    mpn.mpndsc
                FROM public.indicadorpnc ipn
                    JOIN public.metapnc mpn ON mpn.mpnid = ipn.mpnid
                    JOIN spo.subunidadeindicadorpnc sic ON sic.ipnid = ipn.ipnid
                    JOIN public.vw_subunidadeorcamentaria suo ON sic.suoid = suo.suoid
                    LEFT JOIN acompanhamento.acompanhamento aco ON(
                        ipn.ipncod = aco.ipncod
                        AND suo.unocod = aco.unocod
                        AND suo.suocod = aco.suocod
                        AND ipn.prsano = aco.prsano
                        AND aco.acostatus = 'A'
                    )
                WHERE
                    ipn.prsano = '". (int)$filtro->exercicio. "'
                    AND ipn.ipnstatus = 'A'
                    AND sic.suoid = ". (int)$filtro->suoid. "
                    AND mpn.mpnid = ". (int)$filtro->mpnid. "
                UNION
                SELECT
                    ipn.*,
                    aco.acoid,
                    sic.suoid,
                    aco.acoquantidade,
                    mpn.mpncod::int,
                    mpn.mpndsc
                FROM public.indicadorpnc ipn
                    JOIN public.metapnc mpn ON mpn.mpnid = ipn.mpnid
                    LEFT JOIN spo.subunidadeindicadorpnc sic ON sic.ipnid = ipn.ipnid
                    LEFT JOIN public.vw_subunidadeorcamentaria suo ON sic.suoid = suo.suoid
                    LEFT JOIN acompanhamento.acompanhamento aco ON(
                        ipn.ipncod = aco.ipncod
                        AND suo.unocod = aco.unocod
                        AND suo.suocod = aco.suocod
                        AND ipn.prsano = aco.prsano
                        AND acostatus = 'A'
                    )
                WHERE
                    ipn.prsano = '". (int)$filtro->exercicio. "'
                    AND ipn.ipnstatus = 'A'
                    AND sic.suoid IS NULL
                    AND ipn.ipncod != '0'
                    AND mpn.mpnid = ". (int)$filtro->mpnid. "
                ORDER BY
                    mpncod,
                    ipncod,
                    ipndsc
            ";

            $listaIndicadoresPnc = $this->carregar($sql);
        }

        return $listaIndicadoresPnc;
    }

    /**
     * Busca Metas PNC
     * 
     * @param stdClass $filtro
     * @return array
     */
    public function getMetasPncComPi(stdClass $filtro){
        include_once APPRAIZ. 'public/classes/Model/SubUnidadeOrcamentaria.inc';
        $listaMetas = array();
        $subunidade = new Public_Model_SubUnidadeOrcamentaria($filtro->suoid);
        $uo = new Public_Model_UnidadeOrcamentaria($subunidade->unoid);
        $sql = "
            SELECT DISTINCT
                ac.prsano,
                ac.suoid,
                mpn.mpncod::int,
                mpn.mpndsc,
                mpn.mpnid,
                count(pic.pliid) pis,
                count(ac.acoid) acompanhamento
            FROM (
                SELECT
                    ipn.ipncod,
                    ipn.mpnid,
                    ipn.ipndsc,
                    ipn.ipnstatus,
                    ipn.prsano,
                    ipn.ipncod,
                    aco.acoid,
                    sic.suoid,
                    suo.unocod,
                    suo.suocod,
                    aco.acoquantidade
                FROM public.indicadorpnc ipn
                    JOIN spo.subunidadeindicadorpnc sic ON sic.ipnid = ipn.ipnid
                    JOIN public.vw_subunidadeorcamentaria suo ON sic.suoid = suo.suoid
                    LEFT JOIN acompanhamento.acompanhamento aco ON(
                        ipn.ipncod = aco.ipncod
                        AND suo.unocod = aco.unocod
                        AND suo.suocod = aco.suocod
                        AND acostatus = 'A'
                        AND aco.janid = ". (int)$filtro->janid. "
                    )
                WHERE
                    ipn.prsano = '". (int)$filtro->exercicio. "'
                    AND ipn.ipnstatus = 'A'
                    AND suo.suoid = ". (int)$filtro->suoid. "
                UNION
                SELECT
                    ipn.ipncod,
                    ipn.mpnid,
                    ipn.ipndsc,
                    ipn.ipnstatus,
                    ipn.prsano,
                    ipn.ipncod,
                    aco.acoid,
                    sic.suoid,
                    '". (int)$uo->unocod. "',
                    '". (int)$subunidade->suocod. "',
                    aco.acoquantidade
                FROM public.indicadorpnc ipn
                    LEFT JOIN spo.subunidadeindicadorpnc sic ON sic.ipnid = ipn.ipnid
                    LEFT JOIN public.vw_subunidadeorcamentaria suo ON sic.suoid = suo.suoid
                    LEFT JOIN acompanhamento.acompanhamento aco ON(
                        ipn.ipncod = aco.ipncod
                        AND sic.suoid = ". (int)$filtro->suoid. "
                        AND acostatus = 'A'
                        AND aco.janid = ". (int)$filtro->janid. "
                    )
                WHERE
                    ipn.prsano = '". (int)$filtro->exercicio. "'
                    AND ipn.ipnstatus = 'A'
                    AND suo.suoid IS NULL
                    AND ipn.ipncod != '0'
            ) ac
            JOIN public.metapnc mpn ON mpn.mpnid = ac.mpnid
            LEFT JOIN public.vw_subunidadeorcamentaria suo ON suo.suoid = ac.suoid
            LEFT JOIN monitora.pi_planointerno pli ON(
                pli.ungcod = suo.suocod
                AND pli.unicod = suo.unocod
            )
            LEFT JOIN planacomorc.pi_complemento pic ON(
                pli.pliid = pic.pliid
                AND ac.mpnid = pic.mpnid
            )
            GROUP BY
                ac.prsano,
                ac.suoid,
                mpn.mpncod,
                mpn.mpndsc,
                mpn.mpnid
            ORDER BY
                mpncod
        ";
//ver($sql, d);
        $listaMetas = $this->carregar($sql);

        return $listaMetas? $listaMetas: array();
    }

    public function getIndicadoresPncComPi($suoid, $exercicio = null)
    {
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        if($suoid){
            $sql = "select ac.ipnid, ac.ipndsc, ac.ipncod, ac.ipndsc, ac.ipnstatus, ac.prsano, ac.acoid, ac.suoid, mpn.mpncod, mpn.mpndsc, count(pic.pliid) pis
                    from (
                        select ipn.ipnid, ipn.mpnid, ipn.ipndsc, ipn.ipnstatus, ipn.prsano, ipn.ipncod, aco.acoid, sic.suoid, aco.acoquantidade
                        from public.indicadorpnc ipn
                            inner join spo.subunidadeindicadorpnc sic on sic.ipnid = ipn.ipnid
                            left join acompanhamento.acompanhamento aco on aco.ipnid = ipn.ipnid and aco.suoid = sic.suoid and acostatus = 'A'
                        where ipn.prsano = '$exercicio'
                        and ipn.ipnstatus = 'A'
                        and sic.suoid = $suoid
                        union
                        select ipn.ipnid, ipn.mpnid, ipn.ipndsc, ipn.ipnstatus, ipn.prsano, ipn.ipncod, aco.acoid, $suoid, aco.acoquantidade
                        from public.indicadorpnc ipn
                            left join spo.subunidadeindicadorpnc sic on sic.ipnid = ipn.ipnid
                            left join acompanhamento.acompanhamento aco on aco.ipnid = ipn.ipnid and aco.suoid = $suoid and acostatus = 'A'
                        where ipn.prsano = '$exercicio'
                        and ipn.ipnstatus = 'A'
                        and sic.suoid is null
                        and ipn.ipncod != '0'
                    ) ac
                        inner join public.metapnc mpn on mpn.mpnid = ac.mpnid
                        left join public.vw_subunidadeorcamentaria suo on suo.suoid = ac.suoid
                        left join monitora.pi_planointerno pli on pli.ungcod = suo.suocod and pli.unicod = suo.unocod        
                        left join planacomorc.pi_complemento pic on pic.pliid = pli.pliid and pic.ipnid = ac.ipnid        
                    group by ac.ipnid, ac.ipndsc, ac.ipncod, ac.ipndsc, ac.ipnstatus, ac.prsano, ac.acoid, ac.suoid, mpn.mpncod, mpn.mpndsc
                    order by mpn.mpncod::int, ac.ipncod, ac.ipndsc       
                    ";

            return $this->carregar($sql);
        }

        return [];
    }

    public function recuperarMedidas()
    {
        if($this->acoid){
            $medidas = (new Acompanhamento_Model_AcompanhamentoMedida())->recuperarTodos('*', ['acoid = ' . $this->acoid]);
            $aRetorno = [];
            foreach($medidas as $medida){
                $aRetorno[$medida['medid']] = $medida['medid'];
            }
            return $aRetorno;
        }

        return [];
    }
    
    public function associarAnexo($dados)
    {
        if($this->acoid){
            $mAcompanhamentoAnexo = new Acompanhamento_Model_AcompanhamentoArquivo();
            $mAcompanhamentoAnexo->excluirVarios("acoid = '$this->acoid'");

            if(isset($dados['listaAnexos']) && is_array($dados['listaAnexos'])){

                $mAcompanhamentoAnexo->acoid = $this->acoid;

                foreach($dados['listaAnexos'] as $arqid){
                    $mAcompanhamentoAnexo->arqid = $arqid;
                    $mAcompanhamentoAnexo->salvar();

                    $mAcompanhamentoAnexo->acaid = null;
                }
            }
        }
    }

}
