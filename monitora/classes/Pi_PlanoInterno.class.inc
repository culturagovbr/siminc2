<?php

class Pi_PlanoInterno extends Modelo
{

    const K_TIPO_DETALHE_PTRES = 'PT';
    const K_TIPO_DETALHE_PI = 'PI';
    const K_TIPO_DETALHE_PI_NAO_ORCAMENTARIO = 'PIN';
    const K_TIPO_DETALHE_DELEGADO = 'DL';
    const K_TIPO_DETALHE_DELEGADO_NAO_ORCAMENTARIO = 'DLN';

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "monitora.pi_planointerno";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("pliid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pliid' => null,
        'mdeid' => null,
        'eqdid' => null,
        'neeid' => null,
        'capid' => null,
        'sbaid' => null,
        'obrid' => null,
        'plisituacao' => null,
        'plititulo' => null,
        'plidata' => null,
        'plistatus' => null,
        'plicodsubacao' => null,
        'plicod' => null,
        'plilivre' => null,
        'plidsc' => null,
        'usucpf' => null,
        'unicod' => null,
        'ungcod' => null,
        'pliano' => null,
        'docid' => null,
    );

    public function montarSqlExecucaoOrcamentaria($filtro = []){
        $filtro[] = "suo.suostatus = 'A'";
        $filtro[] = "ptr.irpcod <> '6'";
        $where = $this->montarFiltros($filtro);

        $sql = "
            SELECT
                suo.suoid,
                suo.unoid,
                suo.unocod,
                suo.unosigla,
                suo.unonome,
                suo.suosigla,
                suo.suocod,
                suo.suonome,
                ptr.ptrano,
                ptr.funcional,
                ptr.acatitulo,
                ptr.irpcod,
                ptr.plodsc,
                ptr.ptrid,
                ptr.ptres,
                ptr.ptrdotacaoinicialcapital,
                ptr.ptrdotacaoinicialcusteio,
                lmu.lmuvlr,
                lmu.lmuid,
                psu.psuid,
                psu.ptrdotacaocapital ptrdotacao_capital,
                psu.ptrdotacaocusteio ptrdotacao_custeio,
                sum(COALESCE(pic.picvalorcapital, 0)) picvalorcapital,
                sum(COALESCE(pic.picvalorcusteio, 0)) picvalorcusteio,
                (
                    SELECT
                        COUNT(1)
                    FROM spo.ptressubunidade
                    WHERE
                        ptressubunidade.ptrid = ptr.ptrid
                ) AS compartilhada
            FROM public.vw_subunidadeorcamentaria suo
                JOIN spo.ptressubunidade psu on psu.suoid = suo.suoid
                JOIN monitora.vw_ptres ptr ON(
                    ptr.ptrid = psu.ptrid
                    AND ptr.ptrano = suo.prsano
                    AND ptr.ptrstatus = 'A'
                )
                LEFT JOIN monitora.pi_planointernoptres ppt on ppt.ptrid = ptr.ptrid
                LEFT JOIN monitora.pi_planointerno pli ON(
                    pli.pliid = ppt.pliid
                    AND pli.ungcod = suo.suocod
                    AND pli.unicod = suo.unocod
                    AND plistatus = 'A'
                )
                LEFT JOIN planacomorc.pi_complemento pic on pic.pliid = pli.pliid
                LEFT JOIN planacomorc.unidadegestora_limite lmu on lmu.ungcod = suo.suocod AND lmu.lmustatus = 'A' AND lmu.prsano = suo.prsano
            WHERE
                {$where}
            GROUP BY
                suo.suoid,
                suo.unoid,
                suo.unocod,
                suo.unosigla,
                suo.unonome,
                suo.suocod,
                suo.suonome,
                suo.suosigla,
                ptr.ptrano,
                ptr.funcional,
                ptr.acatitulo,
                ptr.irpcod,
                ptr.plodsc,
                ptr.ptrid,
                ptr.ptres,
                lmu.lmuvlr,
                lmu.lmuid,
                psu.psuid,
                ptrdotacao_capital,
                ptrdotacao_custeio,
                ptr.ptrdotacaoinicialcapital,
                ptr.ptrdotacaoinicialcusteio              
            ORDER BY
                suo.unonome,
                suo.suonome,
                ptr.funcional
        ";
//ver($sql, d);
        return $sql;
    }
    
    public function montarSqlRelFuncionais($filtro = []){
        $filtro[] = "suo.suostatus = 'A'";
        $where = $this->montarFiltros($filtro);
        $sql = "
            SELECT
                suo.suoid,
                suo.unoid,
                suo.unocod,
                suo.unosigla,
                suo.unonome,
                suo.suosigla,
                suo.suocod,
                suo.suonome,
                ptr.ptrano,
                ptr.funcional,
                ptr.acatitulo,
                ptr.irpcod,
                ptr.plodsc,
                ptr.ptrid,
                ptr.ptres,
                lmu.lmuvlr,
                lmu.lmuid,
                psu.psuid,
                COALESCE(psu.ptrdotacaocapital, 0) ptrdotacao_capital,
                COALESCE(psu.ptrdotacaocusteio, 0) ptrdotacao_custeio,
                SUM(COALESCE(pic.picvalorcapital, 0)) picvalorcapital,
                SUM(COALESCE(pic.picvalorcusteio, 0)) picvalorcusteio,
		COALESCE(sec.provisionado, 0.00) AS provisionado,
		COALESCE(sec.empenhado, 0.00) AS empenhado,
		COALESCE(sec.liquidado, 0.00) AS liquidado,
		COALESCE(sec.pago, 0.00) as pago,
                (
                    SELECT
                        COUNT(1)
                    FROM spo.ptressubunidade
                    WHERE
                        ptressubunidade.ptrid = ptr.ptrid
                ) AS compartilhada
            FROM public.vw_subunidadeorcamentaria suo
                JOIN spo.ptressubunidade psu on psu.suoid = suo.suoid
                JOIN monitora.vw_ptres ptr on ptr.ptrid = psu.ptrid AND ptr.ptrano = suo.prsano
                LEFT JOIN monitora.pi_planointernoptres ppt on ppt.ptrid = ptr.ptrid
                LEFT JOIN monitora.pi_planointerno pli on(pli.pliid = ppt.pliid AND pli.ungcod = suo.suocod AND pli.unicod = suo.unocod AND plistatus = 'A')
                LEFT JOIN planacomorc.pi_complemento pic on pic.pliid = pli.pliid
                LEFT JOIN planacomorc.unidadegestora_limite lmu on lmu.ungcod = suo.suocod AND lmu.lmustatus = 'A' AND lmu.prsano = suo.prsano
                LEFT JOIN(
                    SELECT
                        siopexecucao.unicod,
                        siopexecucao.ptres,
                        SUM(COALESCE(siopexecucao.vlrautorizado, 0.00))::NUMERIC AS provisionado,
                        SUM(COALESCE(siopexecucao.vlrempenhado, 0.00))::NUMERIC AS empenhado,
                        SUM(COALESCE(siopexecucao.vlrliquidado, 0.00))::NUMERIC AS liquidado,
                        SUM(COALESCE(siopexecucao.vlrpago, 0.00))::NUMERIC AS pago
                    FROM spo.siopexecucao
                        LEFT JOIN monitora.pi_planointerno ON(siopexecucao.plicod = pi_planointerno.plicod AND siopexecucao.exercicio = pi_planointerno.pliano AND pi_planointerno.plistatus = 'A')
                    WHERE
			siopexecucao.exercicio = '". (int)$_SESSION['exercicio']. "' -- Inserindo o ano direto na subquery por motivo de performance da consulta.
                    GROUP BY
                        siopexecucao.ptres,
                        siopexecucao.unicod
		) sec ON(
                    ptr.ptres = sec.ptres
                    AND suo.unocod = sec.unicod
		)
            WHERE
                {$where}
            GROUP BY
                suo.suoid,
                suo.unoid,
                suo.unocod,
                suo.unosigla,
                suo.unonome,
                suo.suocod,
                suo.suonome,
                suo.suosigla,
                ptr.ptrano,
                ptr.funcional,
                ptr.acatitulo,
                ptr.irpcod,
                ptr.plodsc,
                ptr.ptrid,
                ptr.ptres,
                lmu.lmuvlr,
                lmu.lmuid,
                psu.psuid,
                ptrdotacao_capital,
                ptrdotacao_custeio,
		sec.provisionado,
		sec.empenhado,
		sec.liquidado,
		sec.pago
            ORDER BY
                suo.unosigla,
                suo.unocod,
                suo.suonome,
                suo.suocod,
                ptr.funcional
        ";
        return $sql;
    }

    public function montarSqlRelGerencialFuncionais($filtro = [], $ano){
        $filtro[] = "suo.suostatus = 'A'";
        $where = $this->montarFiltros($filtro);
        $sql = "
            SELECT
                suo.suoid,
                suo.unonome || ' (' || suo.unocod || ')' AS unidade,
                suo.suonome || ' (' || suo.suocod || ')' AS subunidade,
                ptr.ptres,
                ptr.funcional || ' - ' || ptr.acatitulo || ': ' || ptr.plodsc || ' (RP ' || ptr.irpcod || ')' AS funcional,
                COALESCE(psu.ptrdotacaocusteio, 0) custeio_dotacao,
                SUM(COALESCE(pic.picvalorcusteio, 0)) custeio_planejado,
                COALESCE(psu.ptrdotacaocapital, 0) capital_dotacao,
                SUM(COALESCE(pic.picvalorcapital, 0)) capital_planejado,
                COALESCE(psu.ptrdotacaocusteio, 0) + COALESCE(psu.ptrdotacaocapital, 0) AS total_dotacao,
                SUM(COALESCE(pic.picvalorcusteio, 0)) + SUM(COALESCE(pic.picvalorcapital, 0)) AS total_planejado,
		COALESCE(sec.provisionado, 0.00) AS provisionado,
		COALESCE(sec.empenhado, 0.00) AS empenhado,
		COALESCE(sec.liquidado, 0.00) AS liquidado,
		COALESCE(sec.pago, 0.00) AS pago,
                (
                    SELECT
                        COUNT(1)
                    FROM spo.ptressubunidade
                    WHERE
                        ptressubunidade.ptrid = ptr.ptrid
                ) AS compartilhada
            FROM public.vw_subunidadeorcamentaria suo
                JOIN spo.ptressubunidade psu on psu.suoid = suo.suoid
                JOIN monitora.vw_ptres ptr on ptr.ptrid = psu.ptrid AND ptr.ptrano = suo.prsano
                LEFT JOIN monitora.pi_planointernoptres ppt on ppt.ptrid = ptr.ptrid
                LEFT JOIN monitora.pi_planointerno pli on(pli.pliid = ppt.pliid AND pli.ungcod = suo.suocod AND pli.unicod = suo.unocod AND plistatus = 'A')
                LEFT JOIN planacomorc.pi_complemento pic on pic.pliid = pli.pliid
                LEFT JOIN planacomorc.unidadegestora_limite lmu on lmu.ungcod = suo.suocod AND lmu.lmustatus = 'A' AND lmu.prsano = suo.prsano
                LEFT JOIN(
                    SELECT
                        siopexecucao.unicod,
                        pi_planointerno.ungcod,
                        siopexecucao.ptres,
                        SUM(COALESCE(siopexecucao.vlrautorizado, 0.00))::NUMERIC AS provisionado,
                        SUM(COALESCE(siopexecucao.vlrempenhado, 0.00))::NUMERIC AS empenhado,
                        SUM(COALESCE(siopexecucao.vlrliquidado, 0.00))::NUMERIC AS liquidado,
                        SUM(COALESCE(siopexecucao.vlrpago, 0.00))::NUMERIC AS pago
                    FROM spo.siopexecucao
                        LEFT JOIN monitora.pi_planointerno ON(siopexecucao.plicod = pi_planointerno.plicod AND siopexecucao.exercicio = pi_planointerno.pliano AND pi_planointerno.plistatus = 'A')
                    WHERE
                        pi_planointerno.ungcod IS NOT NULL
                        AND siopexecucao.exercicio = '". (int)$ano. "' -- Inserindo o ano direto na subquery por motivo de performance da consulta.
                    GROUP BY
                        siopexecucao.ptres,
                        siopexecucao.unicod,
                        pi_planointerno.ungcod
		) sec ON(ptr.ptres = sec.ptres AND suo.unocod = sec.unicod AND suo.suocod = sec.ungcod)
            WHERE
                {$where}
            GROUP BY
                suo.suoid,
                unidade,
                subunidade,
                ptr.ptrid,
                ptr.ptres,
                ptr.funcional,
                ptr.acatitulo,
                ptr.plodsc,
                ptr.irpcod,
                custeio_dotacao,
                capital_dotacao,
		sec.provisionado,
		sec.empenhado,
		sec.liquidado,
		sec.pago
            ORDER BY
                suo.suoid,
                unidade,
                subunidade,
                ptres,
                funcional
        ";
//ver($sql,d );
        return $sql;
    }
    
    /**
     * Busca lista de PI.
     * @example array( 'suo.unocod' => UNICOD_FNC, 'suo.unocod != '. UNICOD_FNC, 'suo.unocod' => array(UNICOD_FNC, 42101)
     *
     * @param integer $exercicio
     * @param array $aFiltro
     * @return array
     */
    public function recuperarExecucaoOrcamentaria($aFiltro = [])
    {
        $sql = $this->montarSqlExecucaoOrcamentaria($aFiltro);
        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];
        $dadosAgrupados = [];
        foreach ($dados as $dado) {
            $dadosAgrupados[$dado['suocod']]['detalhe'][] = $dado;
            $dadosAgrupados[$dado['suocod']]['dotacaoCapital'] += $dado['ptrdotacao_capital'];
            $dadosAgrupados[$dado['suocod']]['dotacaoCusteio'] += $dado['ptrdotacao_custeio'];
            $dadosAgrupados[$dado['suocod']]['planejadoCapital'] += $dado['picvalorcapital'];
            $dadosAgrupados[$dado['suocod']]['planejadoCusteio'] += $dado['picvalorcusteio'];
            $dadosAgrupados[$dado['suocod']]['totalPlanejado'] = ($dadosAgrupados[$dado['suocod']]['planejadoCapital'] + $dadosAgrupados[$dado['suocod']]['planejadoCusteio']);
        }

        return $dadosAgrupados;
    }

    /**
     * Monta consulta de valores totais de execução orçamentária(provisionado, emepenhado, liquidado ou pago) do FNC de acordo com Ano e Subunidade.
     * 
     * @param stdClass $params
     * @return string
     */
    public function montarSqlBuscarTotalExecucaoFnc(stdClass $params){
        # Tipo de retorno
        switch($params->tipo){
            case 'provisionado':
                $campo = 'vlrautorizado';
                break;
            case 'empenhado':
                $campo = 'vlrempenhado';
                break;
            case 'liquidado':
                $campo = 'vlrliquidado';
                break;
            case 'pago':
                $campo = 'vlrpago';
                break;
            default:
                $campo = 'vlrpago';
                break;
        }
        
        # Filtros
        $criterio = $params->exercicio? "\n AND suo.prsano = '". (int)$params->exercicio. "'": NULL;
        $criterio .= $params->suoid? "\n AND suo.suoid = ". (int)$params->suoid: NULL;
        $criterio .= $params->ptrid? "\n AND ptr.ptrid = ". (int)$params->ptrid: NULL;
        if(strtolower($params->gnd) == 'custeio'){
            $criterio .= "\n AND sec.gndcod IN('0', '1', '2', '3')";
        } else if(strtolower($params->gnd) == 'capital'){
            $criterio .= "\n AND sec.gndcod IN('4', '5', '6')";
        }
        
        $sql = "
            SELECT
		sum(COALESCE(sec.$campo, 0))::DECIMAL AS total
            FROM spo.siopexecucao sec
                JOIN monitora.ptres ptr ON(
                    sec.ptres = ptr.ptres
                    AND sec.exercicio = ptr.ptrano
                )
                JOIN monitora.pi_planointernoptres ppt ON ptr.ptrid = ppt.ptrid
                JOIN monitora.pi_planointerno pli ON ppt.pliid = pli.pliid
                JOIN public.vw_subunidadeorcamentaria suo ON(
                    pli.plicod = sec.plicod
                    AND pli.unicod = suo.unocod
                    AND pli.ungcod = suo.suocod
                    AND pli.pliano = suo.prsano
                )
            WHERE
                pli.plistatus = 'A'
                AND suo.unofundo IS TRUE
                AND ptr.irpcod <> '6'
                AND suo.suocod NOT IN('". SUOCOD_ASCOM. "','". SUOCOD_ANCINE. "')
                $criterio
        ";

        return $sql;
    }
    
    /**
     * Busca valor total de execução orçamentária(provisionado, emepenhado, liquidado ou pago) do FNC de acordo com Ano e Subunidade.
     * 
     * @param stdClass $params
     * @return array
     */
    public function buscarTotalExecucaoFnc(stdClass $params){
        $sql = $this->montarSqlBuscarTotalExecucaoFnc($params);
        $total = $this->pegaUm($sql);
//if($params->ptrid) ver($sql);
//if($params->ptrid) ver($total);
        return $total? $total: 0;
    }
    
    /**
     * Monta consulta de valores totais do FNC de acordo com Ano, Subunidade e Situação do Workflow.
     * 
     * @param stdClass $params
     * @return string
     */
    public function montarSqlBuscarTotalFnc(stdClass $params){
        # Campo de valor
        switch(strtolower($params->tipo)) {
            case 'custeio':
                $campo = 'SUM(coalesce(pic.picvalorcusteio, 0))';
            break;
            case 'capital':
                $campo = 'SUM(coalesce(pic.picvalorcapital, 0))';
            break;
            default:
                $campo = 'SUM((coalesce(pic.picvalorcapital, 0) + coalesce(pic.picvalorcusteio, 0)))';
            break;
        }
        
        # Filtros
        $criterio = $params->exercicio? "\n AND pli.pliano = '". (int)$params->exercicio. "'": NULL;
        $criterio .= $params->suoid? "\n AND suo.suoid = ". (int)$params->suoid: NULL;
        $criterio .= $params->ptrid? "\n AND ptr.ptrid = ". (int)$params->ptrid: NULL;
        $criterio .= $params->esdid? "\n AND doc.esdid IN(". join(',', $params->esdid). ")": NULL;
        
        $sql = "
            SELECT
                $campo AS total
            FROM monitora.pi_planointerno pli
                JOIN planacomorc.pi_complemento pic ON pic.pliid = pli.pliid
                JOIN public.vw_subunidadeorcamentaria suo ON(
                    suo.suocod = pli.ungcod
                    AND suo.unocod = pli.unicod
                    AND suo.prsano = pli.pliano)
                JOIN workflow.documento doc ON doc.docid = pli.docid
                JOIN monitora.pi_planointernoptres pip ON pip.pliid = pli.pliid
                JOIN monitora.vw_ptres ptr ON(pip.ptrid = ptr.ptrid)
            WHERE
                pli.plistatus = 'A'
                AND suo.unofundo IS TRUE
                AND ptr.irpcod <> '6'
                $criterio
        ";
        
        return $sql;
    }
    
    /**
     * Busca valores totais do FNC de acordo com Ano, Subunidade e Situação do Workflow.
     * 
     * @param stdClass $params
     * @return array
     */
    public function buscarTotalFnc(stdClass $params){
        $sql = $this->montarSqlBuscarTotalFnc($params);
        $total = $this->pegaUm($sql);
        return $total? $total: 0;
    }
    
    /**
     * Monta consulta a Subunidades do FNC
     * 
     * @param stdClass $params
     * @return string
     */
    public function montarSqlListarSubunidadeFnc(stdClass $params){
        # Filtros
        $criterio = $params->exercicio? "\n AND suo.prsano = '". (int)$params->exercicio. "'": NULL;
        
        $sql = "
            SELECT DISTINCT
                suo.unoid,
                suo.unocod,
                suo.unonome,
                suo.unosigla,
                suo.suoid,
                suo.suocod,
                suo.suosigla,
                suo.suonome,
                CASE WHEN suo.suocod IN(
                    '". SUOCOD_FBN. "',
                    '". SUOCOD_FCRB. "',
                    '". SUOCOD_FCP. "',
                    '". SUOCOD_FUNARTE. "',
                    '". SUOCOD_IBRAM. "',
                    '". SUOCOD_IPHAN. "') THEN
                    1
                ELSE
                    2
                END ordem
            FROM public.vw_subunidadeorcamentaria suo
                JOIN spo.ptressubunidade psu ON psu.suoid = suo.suoid
            WHERE
                suo.suostatus = 'A'
                AND suo.unofundo IS TRUE
                AND suo.suocod NOT IN('". SUOCOD_ASCOM. "','". SUOCOD_ANCINE. "')
                $criterio
            ORDER BY
                ordem,
                suo.suonome
        ";
        return $sql;
    }
    
    /**
     * Consulta as Subunidades do FNC
     * 
     * @param stdClass $params
     * @return array
     */
    public function listarSubunidadeFnc(stdClass $params){
        $sql = $this->montarSqlListarSubunidadeFnc($params);
        $listaSubunidadeFnc = $this->carregar($sql);
        return $listaSubunidadeFnc? $listaSubunidadeFnc: array();
    }
    
    public function montarSqlListarFuncionaisFncTipo(stdClass $params){
        $valorTipo = $params->tipo == 'custeio'? 'CUSTEIO': 'CAPITAL';
        $campo = $params->tipo == 'custeio'? 'ptrdotacaocusteio': 'ptrdotacaocapital';
        
        # Filtros
        $criterio = $params->exercicio? "\n AND suo.prsano = '". (int)$params->exercicio. "'": NULL;
        
        $sql = "
            SELECT DISTINCT
                '{$valorTipo}' AS tipo,
                suo.unoid,
                suo.unocod,
                suo.unosigla,
                suo.unonome,
                lmu.lmuvlr,
                lmu.lmuid,
                psu.ptrid,
                vptr.funcional,
                vptr.acatitulo,
                vptr.irpcod,
                vptr.plodsc,
                ptr.{$campo} AS dotacao
            FROM public.vw_subunidadeorcamentaria suo
                JOIN spo.ptressubunidade psu ON psu.suoid = suo.suoid
                JOIN monitora.vw_ptres vptr ON(
                    vptr.ptrid = psu.ptrid
                    AND vptr.ptrano = suo.prsano
                )
                JOIN monitora.ptres ptr ON ptr.ptrid = vptr.ptrid
                LEFT JOIN planacomorc.unidadegestora_limite lmu ON(
                    lmu.unoid = suo.unoid
                    AND lmu.lmustatus = 'A'
                    AND lmu.prsano = suo.prsano
                )
            WHERE
                suo.suostatus = 'A'
                AND vptr.irpcod <> '6'
                AND suo.unofundo IS TRUE
                AND suo.suocod NOT IN(
                    '". SUOCOD_ANCINE. "',
                    '". SUOCOD_ASCOM. "'
                )
                $criterio
        ";

        return $sql;
    }
    
    /**
     * Monta consulta a Funcionais do FNC
     * 
     * @param stdClass $params
     * @return string
     */
    public function montarSqlListarFuncionaisFnc(stdClass $params){
        $params->tipo = 'custeio';
        $sql = $this->montarSqlListarFuncionaisFncTipo($params).
            "\n UNION \n";
        $params->tipo = 'capital';
        $sql .= $this->montarSqlListarFuncionaisFncTipo($params).
            "
            ORDER BY
                funcional ASC,
                tipo DESC
            "
        ;

        return $sql;
    }
    
    /**
     * Consulta as Funcionais do FNC
     * 
     * @param stdClass $params
     * @return array
     */
    public function listarFuncionaisFnc(stdClass $params){
        $sql = $this->montarSqlListarFuncionaisFnc($params);
        $listaFuncionaisFnc = $this->carregar($sql);
        return $listaFuncionaisFnc? $listaFuncionaisFnc: array();
    }

    public function recuperarExecucaoOrcamentariaFnc()
    {
        $sql = "
            SELECT DISTINCT
                suo.unoid,
                suo.unocod,
                suo.unosigla,
                suo.unonome,
                lmu.lmuvlr,
                lmu.lmuid,
                ptr.ptrdotacaocapital ptrdotacao_capital,
                ptr.ptrdotacaocusteio ptrdotacao_custeio,
                psu.ptrid,
                vptr.funcional,
                vptr.acatitulo,
                vptr.irpcod,
                vptr.plodsc
            FROM public.vw_subunidadeorcamentaria suo
                JOIN spo.ptressubunidade psu ON psu.suoid = suo.suoid
                JOIN monitora.vw_ptres vptr ON(
                    vptr.ptrid = psu.ptrid
                    AND vptr.ptrano = suo.prsano
                )
                JOIN monitora.ptres ptr ON ptr.ptrid = vptr.ptrid
                LEFT JOIN planacomorc.unidadegestora_limite lmu ON(
                    lmu.unoid = suo.unoid
                    AND lmu.lmustatus = 'A'
                    AND lmu.prsano = suo.prsano
                )
            WHERE
                suo.suostatus = 'A'
                AND suo.unofundo IS TRUE
                AND suo.suocod NOT IN ('203003', '420017')
                AND suo.prsano = '{$_SESSION['exercicio']}'
                AND ptr.irpcod <> '6'
            ORDER BY
                suo.unonome
        ";

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];
        $dadosAgrupados = [];
        foreach ($dados as $dado) {
            $dadosAgrupados[$dado['unocod']]['detalhe'][] = $dado;
            $dadosAgrupados[$dado['unocod']]['dotacaoCapital'] += $dado['ptrdotacao_capital'];
            $dadosAgrupados[$dado['unocod']]['dotacaoCusteio'] += $dado['ptrdotacao_custeio'];
        }

        return $dadosAgrupados;
    }

    public static function montarFiltroExecucaoOrcamentariaUo(stdClass $filtro) {
        # Filtros
        $where = "
            suo.prsano = '". (int)$filtro->exercicio. "'
            AND suo.unofundo IS FALSE";
        
        # Caso o exercicio seja superior a 2018(ano da unificação de Òrgãos), o sistema busca todas as vinculadas do MC
        if($filtro->exercicio > 2018){
            $where .= "\n AND suo.unocod IN(
                '". UNOCOD_ANCINE. "',
                '". UNOCOD_FSA. "',
                '". UNOCOD_FBN. "',
                '". UNOCOD_FCP. "',
                '". UNOCOD_FCRB. "',
                '". UNOCOD_FUNARTE. "',
                '". UNOCOD_IBRAM. "',
                '". UNOCOD_IPHAN. "',
                '". UNOCOD_RSFNC. "',
                '". UNOCOD_AGLO. "'
            )
            ";
        } else {
            # Caso o exercicio seja anterior a 2019(ano da unificação de Òrgãos), o sistema busca todas as vinculadas do MINC(Cultura)
            $where .= "\n AND suo.unocod <> '". (int)UNICOD_MINC. "'";
        }
        
        return $where;
    }
    
    /**
     * Busca Unidades com estatistica de orçamento e valores de pagamento.
     *
     * @param stdClass $filtro exercicio é o atributo obrigatório!
     * @return array
     */
    public function consultarExecucaoOrcamentariaUo(stdClass $filtro)
    {
        # Filtros
        $where = self::montarFiltroExecucaoOrcamentariaUo($filtro);
        
        $sql = "
            SELECT DISTINCT
                1 AS ordem,
                'Dotação' AS descricao,
                suo.unosigla AS categoria,
                TRUNC(SUM(COALESCE(psu.ptrdotacaocusteio, 0)::DECIMAL) + SUM(COALESCE(psu.ptrdotacaocapital, 0)::DECIMAL), 0) AS valor
            FROM public.vw_subunidadeorcamentaria suo
                LEFT JOIN spo.ptressubunidade psu ON psu.suoid = suo.suoid
                LEFT JOIN monitora.vw_ptres ptr ON(
                    ptr.ptrid = psu.ptrid
                    AND ptr.ptrano = suo.prsano
                )
            WHERE
                $where
                AND suo.suostatus = 'A'
                AND ptr.irpcod <> '6'
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                2 AS ordem,
                'Limite' AS descricao,
                suo.unosigla AS categoria,
                TRUNC(SUM(COALESCE(lmu.lmuvlr, 0)::DECIMAL), 0) AS valor
            FROM public.vw_subunidadeorcamentaria suo
                LEFT JOIN planacomorc.unidadegestora_limite lmu ON(
                    suo.suocod = lmu.ungcod
                    AND suo.prsano = lmu.prsano
                )
            WHERE
                $where
                AND suo.suostatus = 'A'
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                3 AS ordem,
                'Planejado' AS descricao,
                suo.unosigla AS categoria,
                TRUNC(SUM(COALESCE(pic.picvalorcapital, 0) + COALESCE(pic.picvalorcusteio, 0))::DECIMAL, 0) AS valor
            FROM public.vw_subunidadeorcamentaria suo
                LEFT JOIN monitora.pi_planointerno pli ON(
                    pli.unicod = suo.unocod
                    AND pli.ungcod = suo.suocod
                    AND pli.pliano = suo.prsano
                    AND pli.plistatus = 'A'
                )
                LEFT JOIN emendas.beneficiario ben ON(pli.pliid = ben.pliid)
                LEFT JOIN planacomorc.pi_complemento pic ON pic.pliid = pli.pliid
            WHERE
                $where
                AND suo.suostatus = 'A'
                AND pli.plistatus = 'A'
                AND ben.benid IS NULL
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                3.5 AS ordem,
                'Provisionado' AS descricao,
                suo.unosigla AS categoria,
                TRUNC(SUM(COALESCE(sec.vlrautorizado, 0))::DECIMAL, 0) AS valor
            FROM public.vw_subunidadeorcamentaria suo
                LEFT JOIN monitora.pi_planointerno pli ON(
                    pli.unicod = suo.unocod
                    AND pli.ungcod = suo.suocod
                    AND pli.pliano = suo.prsano
                    AND pli.plistatus = 'A'
                )
                LEFT JOIN emendas.beneficiario ben ON(pli.pliid = ben.pliid)
                LEFT JOIN monitora.pi_planointernoptres ppt ON ppt.pliid = pli.pliid
                LEFT JOIN monitora.ptres ptr ON ptr.ptrid = ppt.ptrid
                LEFT JOIN spo.siopexecucao sec ON(
                    sec.exercicio = ptr.ptrano
                    AND sec.plicod = pli.plicod
                    AND sec.exercicio = ptr.ptrano
                )
            WHERE
                $where
                AND suo.suostatus = 'A'
                AND ben.benid IS NULL
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                4 AS ordem,
                'Empenhado' AS descricao,
                suo.unosigla AS categoria,
                TRUNC(SUM(COALESCE(sec.vlrempenhado, 0))::DECIMAL, 0) AS valor
            FROM public.vw_subunidadeorcamentaria suo
                LEFT JOIN monitora.pi_planointerno pli ON(
                    pli.unicod = suo.unocod
                    AND pli.ungcod = suo.suocod
                    AND pli.pliano = suo.prsano
                    AND pli.plistatus = 'A'
                )
                LEFT JOIN emendas.beneficiario ben ON(pli.pliid = ben.pliid)
                LEFT JOIN monitora.pi_planointernoptres ppt ON ppt.pliid = pli.pliid
                LEFT JOIN monitora.ptres ptr ON ptr.ptrid = ppt.ptrid
                LEFT JOIN spo.siopexecucao sec ON(
                    sec.exercicio = ptr.ptrano
                    AND sec.plicod = pli.plicod
                    AND sec.exercicio = ptr.ptrano
                )
            WHERE
                $where
                AND suo.suostatus = 'A'
                AND ben.benid IS NULL
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                6 AS ordem,
                'Pago' AS descricao,
                suo.unosigla AS categoria,
                TRUNC(SUM(COALESCE(sec.vlrpago, 0))::DECIMAL, 0) AS valor
            FROM public.vw_subunidadeorcamentaria suo
                LEFT JOIN monitora.pi_planointerno pli ON(pli.unicod = suo.unocod AND pli.ungcod = suo.suocod AND pli.pliano = suo.prsano AND pli.plistatus = 'A')
                LEFT JOIN emendas.beneficiario ben ON(pli.pliid = ben.pliid)
                LEFT JOIN monitora.pi_planointernoptres ppt ON ppt.pliid = pli.pliid
                LEFT JOIN monitora.ptres ptr ON ptr.ptrid = ppt.ptrid
                LEFT JOIN spo.siopexecucao sec ON(
                    sec.exercicio = ptr.ptrano
                    AND sec.plicod = pli.plicod
                    AND sec.exercicio = ptr.ptrano
                )
            WHERE
                $where
                AND suo.suostatus = 'A'
                AND ben.benid IS NULL
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                5 AS ordem,
                'Liquidado' AS descricao,
                suo.unosigla AS categoria,
                TRUNC(SUM(COALESCE(sec.vlrliquidado, 0))::DECIMAL, 0) AS valor
            FROM public.vw_subunidadeorcamentaria suo
                LEFT JOIN monitora.pi_planointerno pli ON(
                    pli.unicod = suo.unocod
                    AND pli.ungcod = suo.suocod
                    AND pli.pliano = suo.prsano
                    AND pli.plistatus = 'A')
                LEFT JOIN emendas.beneficiario ben ON(pli.pliid = ben.pliid)
                LEFT JOIN monitora.pi_planointernoptres ppt ON ppt.pliid = pli.pliid
                LEFT JOIN monitora.ptres ptr ON ptr.ptrid = ppt.ptrid
                LEFT JOIN spo.siopexecucao sec ON(
                    sec.exercicio = ptr.ptrano
                    AND sec.plicod = pli.plicod
                    AND sec.exercicio = ptr.ptrano
                )
            WHERE
                $where
                AND suo.suostatus = 'A'
                AND ben.benid IS NULL
            GROUP BY
                ordem,
                descricao,
                categoria
            ORDER BY
                ordem,
                categoria
        ";

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }

    public static function montarFiltroExecucaoOrcamentariaSubunidade(stdClass $filtro) {
        # Filtros
        $where = "
            suo.prsano = '". (int)$filtro->exercicio. "'
            AND suo.unofundo IS FALSE";
        
        $where .= $filtro->unosigla? "\n AND suo.unocod = '". (int)$filtro->unocod. "'": NULL;

        if($filtro->listaSubunidade){
            $where .= "\n AND suo.suocod IN('". join("', '", $filtro->listaSubunidade) ."')";
        }
        
        return $where;
    }
    
    /**
     * Busca Subunidades por UO com estatistica de orçamento e valores de pagamento.
     *
     * @param stdClass $filtro
     * @return array
     */
    public function consultarExecucaoOrcamentariaSubunidade(stdClass $filtro)
    {
        # Filtros
        $where = self::montarFiltroExecucaoOrcamentariaSubunidade($filtro);
        
        $sql = "
            SELECT DISTINCT
                1 AS ordem,
                'Dotação' AS descricao,
                suo.suosigla AS categoria,
                (SUM(COALESCE(psu.ptrdotacaocusteio, 0)::DECIMAL) + SUM(COALESCE(psu.ptrdotacaocapital, 0)::DECIMAL)) AS valor
            FROM public.vw_subunidadeorcamentaria suo
                LEFT JOIN spo.ptressubunidade psu ON psu.suoid = suo.suoid
                LEFT JOIN monitora.vw_ptres ptr ON(
                    ptr.ptrid = psu.ptrid
                    AND ptr.ptrano = suo.prsano
                )
            WHERE
                $where
                AND suo.suostatus = 'A'
                AND ptr.irpcod <> '6'
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                2 AS ordem,
                'Limite' AS descricao,
                suo.suosigla AS categoria,
                SUM(COALESCE (lmu.lmuvlr, 0)::DECIMAL) AS valor
            FROM public.vw_subunidadeorcamentaria suo
                LEFT JOIN planacomorc.unidadegestora_limite lmu ON(
                    suo.suocod = lmu.ungcod
                    AND suo.prsano = lmu.prsano
                    AND lmu.lmustatus = 'A'
                )
            WHERE
                $where
                AND suo.suostatus = 'A'
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                3 AS ordem,
                'Planejado' AS descricao,
                suo.suosigla AS categoria,
                SUM(COALESCE(pic.picvalorcapital, 0) + COALESCE(pic.picvalorcusteio, 0))::DECIMAL AS valor
            FROM public.vw_subunidadeorcamentaria suo
                LEFT JOIN monitora.pi_planointerno pli ON(
                    pli.ungcod = suo.suocod
                    AND pli.pliano = suo.prsano
                    AND pli.plistatus = 'A'
                )
                LEFT JOIN emendas.beneficiario ben ON(pli.pliid = ben.pliid)
                LEFT JOIN planacomorc.pi_complemento pic ON pic.pliid = pli.pliid
            WHERE
                $where
                AND suo.suostatus = 'A'
                AND ben.benid IS NULL
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                3.5 AS ordem,
                'Provisionado' AS descricao,
                suo.suosigla AS categoria,
                SUM(COALESCE(sec.vlrautorizado, 0))::DECIMAL AS valor
            FROM public.vw_subunidadeorcamentaria suo
                LEFT JOIN monitora.pi_planointerno pli ON(
                    pli.ungcod = suo.suocod
                    AND pli.pliano = suo.prsano
                    AND pli.plistatus = 'A'
                )
                LEFT JOIN emendas.beneficiario ben ON(pli.pliid = ben.pliid)
                LEFT JOIN monitora.pi_planointernoptres ppt ON ppt.pliid = pli.pliid
                LEFT JOIN monitora.ptres ptr ON ptr.ptrid = ppt.ptrid
                LEFT JOIN spo.siopexecucao sec ON(
                    sec.exercicio = ptr.ptrano
                    AND sec.plicod = pli.plicod
                    AND sec.exercicio = ptr.ptrano
                )
            WHERE
                $where
                AND suo.suostatus = 'A'
                AND ben.benid IS NULL
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                4 AS ordem,
                'Empenhado' AS descricao,
                suo.suosigla AS categoria,
                SUM(COALESCE(sec.vlrempenhado, 0))::DECIMAL AS valor
            FROM public.vw_subunidadeorcamentaria suo
                LEFT JOIN monitora.pi_planointerno pli ON(
                    pli.ungcod = suo.suocod
                    AND pli.pliano = suo.prsano
                    AND pli.plistatus = 'A')
                LEFT JOIN emendas.beneficiario ben ON(pli.pliid = ben.pliid)
                LEFT JOIN monitora.pi_planointernoptres ppt ON ppt.pliid = pli.pliid
                LEFT JOIN monitora.ptres ptr ON ptr.ptrid = ppt.ptrid
                LEFT JOIN spo.siopexecucao sec ON(
                    sec.exercicio = ptr.ptrano
                    AND sec.plicod = pli.plicod
                    AND sec.exercicio = ptr.ptrano
                )
            WHERE
                $where
                AND suo.suostatus = 'A'
                AND ben.benid IS NULL
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                6 AS ordem,
                'Pago' AS descricao,
                suo.suosigla AS categoria,
                SUM(COALESCE(sec.vlrpago, 0))::DECIMAL AS valor
            FROM public.vw_subunidadeorcamentaria suo
                LEFT JOIN monitora.pi_planointerno pli ON(
                    pli.ungcod = suo.suocod
                    AND pli.pliano = suo.prsano
                    AND pli.plistatus = 'A'
                )
                LEFT JOIN emendas.beneficiario ben ON(pli.pliid = ben.pliid)
                LEFT JOIN monitora.pi_planointernoptres ppt ON ppt.pliid = pli.pliid
                LEFT JOIN monitora.ptres ptr ON ptr.ptrid = ppt.ptrid
                LEFT JOIN spo.siopexecucao sec ON(
                    sec.exercicio = ptr.ptrano
                    AND sec.plicod = pli.plicod
                    AND sec.exercicio = ptr.ptrano
                )
            WHERE
                $where
                AND suo.suostatus = 'A'
                AND ben.benid IS NULL
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                5 AS ordem,
                'Liquidado' AS descricao,
                suo.suosigla AS categoria,
                SUM(COALESCE(sec.vlrliquidado, 0))::DECIMAL AS valor
            FROM public.vw_subunidadeorcamentaria suo
                LEFT JOIN monitora.pi_planointerno pli ON(
                    pli.ungcod = suo.suocod
                    AND pli.pliano = suo.prsano
                    AND pli.plistatus = 'A')
                LEFT JOIN emendas.beneficiario ben ON(pli.pliid = ben.pliid)
                LEFT JOIN monitora.pi_planointernoptres ppt ON ppt.pliid = pli.pliid
                LEFT JOIN monitora.ptres ptr ON ptr.ptrid = ppt.ptrid
                LEFT JOIN spo.siopexecucao sec ON(
                    sec.exercicio = ptr.ptrano
                    AND sec.plicod = pli.plicod
                    AND sec.exercicio = ptr.ptrano
                )
            WHERE
                $where
                AND suo.suostatus = 'A'
                AND ben.benid IS NULL
            GROUP BY
                ordem,
                descricao,
                categoria
            ORDER BY
                ordem,
                categoria
        ";

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }

    public function recuperarEstatisticaUnidade($exercicio, $aFiltro = [])
    {
        $sql = "select uno.unosigla descricao, sum(e.autorizado::decimal) valor
                from wssof.ws_execucaoorcamentariadto e
                        inner join planointerno.unidadeorcamentaria uno on unocod = e.unidadeorcamentaria AND uno.prsano = '$exercicio'
                where e.planointerno is not null
                AND e.anoexercicio = '$exercicio'
                AND e.anoreferencia = '$exercicio'
                group by uno.unosigla
                order by descricao";

        // Carregando dados da base de dados do SIMINC antigo
        $dados = adapterConnection::siminc1()->carregar($sql);
        return $dados ? $dados : [];
    }

    public function recuperarEstatisticaPiUnidade($exercicio, $aFiltro = [])
    {
        $sql = "select uno.unosigla descricao, count(distinct e.planointerno) valor
                from wssof.ws_execucaoorcamentariadto e
                        inner join planointerno.unidadeorcamentaria uno on unocod = e.unidadeorcamentaria AND uno.prsano = '$exercicio'
                where e.planointerno is not null
                AND e.anoexercicio = '$exercicio'
                AND e.anoreferencia = '$exercicio'
                group by uno.unosigla
                order by descricao;";

        // Carregando dados da base de dados do SIMINC antigo
        $dados = adapterConnection::siminc1()->carregar($sql);
        return $dados ? $dados : [];
    }

    /**
     * Verifica se o Código de PI inserido está sendo usado.
     *
     * @param $plicod
     * @return array
     */
    public function consultaCodigoPi($plicod)
    {
        $plicod = strtoupper($plicod);

        $sql = "SELECT
                    p.pliid
                FROM
                    monitora.pi_planointerno AS p
                    JOIN workflow.documento AS d ON p.docid = d.docid
                WHERE
                    plicod = '" . $plicod . "'
                    AND (plistatus = 'A'
                    OR (plistatus = 'I'
                    AND esdid = '" . ESD_PI_CANCELADO . "'))
        ";
        return $this->pegaUm($sql);
    }

    public function alterarCodigoPi($plicod)
    {
        $codigoAntigo = $this->plicod;
        $this->plicod = $plicod;
        $this->salvar();
        $mHistoricoPi = new Planacomorc_Model_HistoricoPi();
        $mHistoricoPi->pliid = $this->pliid;
        $mHistoricoPi->usucpf = $_SESSION['usucpf'];
        $mHistoricoPi->hpidscantigo = $codigoAntigo;
        $mHistoricoPi->hpidscnovo = $plicod;
        $mHistoricoPi->salvar();
        $this->commit();

        return $this;
    }

    public function verificarPermissaoEditar($estadoAtual, $perfis)
    {
        switch ($estadoAtual['esdid']) {
            case ESD_PI_APROVADO:
            case ESD_PI_CANCELADO:
            case ESD_PI_AGUARDANDO_APROVACAO:
                return (bool)count(array_intersect($perfis, [PFL_ADMINISTRADOR, PFL_SUPERUSUARIO, PFL_PLANEJAMENTO]));
        }

        return true;
    }

    /**
     * Casol não exista Janela cadastrada para o periodo vigente o sistema não permite as unidades fazerem inserção ou alteração no PI.
     *
     * @param array $perfis
     * @param array $listaJanelasDeAlteracao
     * @return boolean
     */
    public function verificarPermissaoEditarUnidade($perfis, $listaJanelasDeAlteracao){
        if(in_array(PFL_SUBUNIDADE, $perfis) && !$listaJanelasDeAlteracao){
            return false;
        }

        return true;
    }

    public function verificarPermissaoEditarFnc($estadoAtual, $perfis, $listaJanelasDeAlteracao)
    {
        # Casol não exista Janela cadastrada para o periodo vigente o sistema não permite as unidades fazerem inserção ou alteração no PI.
        if(!self::verificarPermissaoEditarUnidade($perfis, $listaJanelasDeAlteracao)){
            return false;
        }
        if(array_intersect($perfis, [PFL_CONSULTA, PFL_CONSULTA_UNIDADE])){
          return false;
        }
        switch ($estadoAtual['esdid']) {
            case ESD_FNC_PI_APROVADO:
            case ESD_FNC_PI_BANCO_PROJETOS:
            case ESD_FNC_PI_DELIBERACAO_CFNC:
            case ESD_FNC_PI_EM_ANALISE:
            case ESD_FNC_PI_SELECIONADO_CFNC:
                return (bool)count(array_intersect($perfis, [PFL_ADMINISTRADOR, PFL_SUPERUSUARIO, PFL_PLANEJAMENTO]));
        }

        return true;
    }

    /**
     * Monta consulta do Relatório Geral de PI e PI FNC
     * 
     * @param stdClass $dto
     * @return string
     */
    public function montarSqlRelatorioGeral(stdClass $dto){
        $where .= $dto->exercicio? "\n AND pli.pliano = '". (int)$dto->exercicio. "'": NULL;
        $where .= $dto->fundo === TRUE? "\n AND suo.unofundo IS TRUE": NULL;
        $where .= $dto->fundo === FALSE? "\n AND suo.unofundo IS FALSE": NULL;
        $where .= $dto->listaSubunidade? "\n AND suo.suocod IN('". join("','", $dto->listaSubunidade). "') ": NULL;
        $where .= $dto->suoid? "\n AND suo.suoid IN(".join($dto->suoid, ','). ")": NULL;
        $where .= $dto->esdid? "\n AND doc.esdid IN(".join($dto->esdid, ','). ")": NULL;
        $where .= $dto->eqdid? "\n AND pli.eqdid IN(".join($dto->eqdid, ','). ")": NULL;
        $where .= $dto->picted == 't'? "\n AND pic.picted IS TRUE": NULL;
        $where .= $dto->picted == 'f'? "\n AND pic.picted IS FALSE": NULL;
        $where .= $dto->irpcod? "\n AND ptr.irpcod::INTEGER IN(".join($dto->irpcod, ','). ")": NULL;
        $where .= $dto->oppid? "\n AND pic.oppid IN(".join($dto->oppid, ','). ")": NULL;
        $where .= $dto->mdeid? "\n AND pli.mdeid IN(".join($dto->mdeid, ','). ")": NULL;
        $where .= $dto->capid? "\n AND pli.capid IN(".join($dto->capid, ','). ")": NULL;
        $where .= $dto->mescod? "\n AND pic.mescod IN('".join($dto->mescod, "','"). "')": NULL;
        $where .= $dto->claid? "\n AND cl.claid IN(".join($dto->claid, ','). ")": NULL;
        $where .= $dto->acaid? "\n AND ptr.acaid IN(".join($dto->acaid, ','). ")": NULL;

        $sql = "
            SELECT
                pli.pliid,
                esd.esddsc,
                pli.plicod,
                suo.unocod,
                suo.unonome,
                suo.suocod,
                suo.suonome,
                (SELECT ARRAY_TO_STRING(ARRAY(
                    SELECT DISTINCT
                        s.unonome
                    FROM planacomorc.pi_delegacao d
                        JOIN public.vw_subunidadeorcamentaria s ON s.suoid = d.suoid
                    WHERE
                        d.pliid = pli.pliid
                ), '; ')) unodelegada,

                (SELECT ARRAY_TO_STRING(ARRAY(
                    SELECT DISTINCT
                        s.suonome
                    FROM planacomorc.pi_delegacao d
                        JOIN public.vw_subunidadeorcamentaria s ON s.suoid = d.suoid
                    WHERE
                        d.pliid = pli.pliid
                ), '; ')) suodelegada,

                eqd.eqddsc, rp.rpcod || ' - ' || rp.redsc as resultadoprimario, mai.mainome, mas.masnome, ptr.prgcod, ptr.prgdsc, opp.oppcod, opp.oppnome, ipp.ippcod, ipp.ippnome,
                ptr.acacod, ptr.acatitulo, ptr.loccod, ptr.locdsc, ptr.plocod, plodsc po, ptr.ptres,
                mde.mdedsc, mpn.mpncod || ' - ' || mpn.mpnnome mpnnome, mpp.mppcod || ' - ' || mpp.mppnome mppnome,
                pli.plititulo, pli.plidsc, nee.needsc, cap.capdsc,
                CASE WHEN rp.rpcod = 6 THEN 'Sim' ELSE 'Não' END AS pliemenda,
                CASE WHEN pic.picedital = TRUE THEN 'Sim' ELSE 'Não' END AS picedital,
                esf.esfdsc,
                CASE WHEN pic.esfid IN(1, 2, 3) THEN
                    'Brasil'
                ELSE
                    pic.esfid::char
                END AS pais,
                CASE WHEN pic.esfid = 2 THEN
                    (SELECT ARRAY_TO_STRING(array(
                            SELECT DISTINCT e.estdescricao
                            FROM planacomorc.pi_localizacao l
                                    JOIN territorios.estado e ON e.estuf = l.estuf
                            WHERE l.pliid = pli.pliid
                    ), '; '))
                WHEN pic.esfid = 3 THEN
                    (SELECT ARRAY_TO_STRING(array(
                            SELECT DISTINCT e.estdescricao
                            FROM planacomorc.pi_localizacao l
                                    JOIN territorios.municipio m ON m.muncod = l.muncod
                                    JOIN territorios.estado e ON e.estuf = m.estuf
                            WHERE l.pliid = pli.pliid
                    ), '; '))
                ELSE ''
                END estado,

                (SELECT ARRAY_TO_STRING(array(
                        SELECT DISTINCT m.mundescricao
                        FROM planacomorc.pi_localizacao l
                                JOIN territorios.municipio m ON m.muncod = l.muncod
                                JOIN territorios.estado e ON e.estuf = m.estuf
                        WHERE l.pliid = pli.pliid
                ), '; ')) municipio,

                meu.meunome,
                obe.obenome,
                dim.dimenome,
                mee.meenome,
                meu.meuquantidade,
                CASE WHEN meuprioritario = TRUE THEN 'Sim' ELSE 'Não' END AS meuprioritario,

                (SELECT ARRAY_TO_STRING(array(
                        SELECT DISTINCT usu.usunome || ' - ' || replace(to_char(usu.usucpf::numeric, '000:000:000-00'), ':', '.') || ' (' || usu.usufoneddd || ' ' || usu.usufonenum || ' - ' || usu.usuemail || ')'
                        FROM planacomorc.pi_responsavel pir
                                JOIN seguranca.usuario usu ON usu.usucpf = pir.usucpf
                        WHERE pir.pliid = pli.pliid
                ), '; ')) usuario,
                CASE WHEN pic.picted THEN 'Sim' ELSE 'Não' END AS ted,
                CASE WHEN pic.picedital THEN 'Sim' ELSE 'Não' END AS edital,
                mes.mesdsc,

                (SELECT ARRAY_TO_STRING(array(
                       SELECT DISTINCT pco.pcoconvenio
                        FROM planacomorc.pi_convenio pco
                        WHERE pco.pliid = pli.pliid
                ), '; ')) convenio,

                (SELECT ARRAY_TO_STRING(array(
                       SELECT DISTINCT pis.pissniic
                        FROM planacomorc.pi_sniic pis
                        WHERE pis.pliid = pli.pliid
                ), '; ')) sniic,

                (SELECT ARRAY_TO_STRING(array(
                       SELECT DISTINCT pse.psesei
                        FROM planacomorc.pi_sei pse
                        WHERE pse.pliid = pli.pliid
                ), '; ')) sei,


                (SELECT ARRAY_TO_STRING(array(
                       SELECT DISTINCT ppr.pprpronac
                        FROM planacomorc.pi_pronac ppr
                        WHERE ppr.pliid = pli.pliid
                ), '; ')) pronac,

                ppr.pprnome, pum.pumnome, picquantidade,

                COALESCE(pic.picvalorcusteio, 0) + COALESCE(pic.picvalorcapital, 0) AS valortotal,
                COALESCE(pic.picvalorcusteio, 0) AS picvalorcusteio,
                COALESCE(pic.picvalorcapital, 0) AS picvalorcapital,

                -- Cronograma Físico
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 1 AND mescod = '01' AND pcr.pliid = pli.pliid LIMIT 1) fisico_1,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 1 AND mescod = '02' AND pcr.pliid = pli.pliid LIMIT 1) fisico_2,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 1 AND mescod = '03' AND pcr.pliid = pli.pliid LIMIT 1) fisico_3,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 1 AND mescod = '04' AND pcr.pliid = pli.pliid LIMIT 1) fisico_4,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 1 AND mescod = '05' AND pcr.pliid = pli.pliid LIMIT 1) fisico_5,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 1 AND mescod = '06' AND pcr.pliid = pli.pliid LIMIT 1) fisico_6,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 1 AND mescod = '07' AND pcr.pliid = pli.pliid LIMIT 1) fisico_7,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 1 AND mescod = '08' AND pcr.pliid = pli.pliid LIMIT 1) fisico_8,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 1 AND mescod = '09' AND pcr.pliid = pli.pliid LIMIT 1) fisico_9,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 1 AND mescod = '10' AND pcr.pliid = pli.pliid LIMIT 1) fisico_10,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 1 AND mescod = '11' AND pcr.pliid = pli.pliid LIMIT 1) fisico_11,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 1 AND mescod = '12' AND pcr.pliid = pli.pliid LIMIT 1) fisico_12,

                -- Cronograma Financeiro Capital
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 5 AND mescod = '01' AND pcr.pliid = pli.pliid  LIMIT 1) fin_capital_1,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 5 AND mescod = '02' AND pcr.pliid = pli.pliid  LIMIT 1) fin_capital_2,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 5 AND mescod = '03' AND pcr.pliid = pli.pliid  LIMIT 1) fin_capital_3,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 5 AND mescod = '04' AND pcr.pliid = pli.pliid  LIMIT 1) fin_capital_4,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 5 AND mescod = '05' AND pcr.pliid = pli.pliid  LIMIT 1) fin_capital_5,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 5 AND mescod = '06' AND pcr.pliid = pli.pliid  LIMIT 1) fin_capital_6,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 5 AND mescod = '07' AND pcr.pliid = pli.pliid  LIMIT 1) fin_capital_7,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 5 AND mescod = '08' AND pcr.pliid = pli.pliid  LIMIT 1) fin_capital_8,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 5 AND mescod = '09' AND pcr.pliid = pli.pliid  LIMIT 1) fin_capital_9,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 5 AND mescod = '10' AND pcr.pliid = pli.pliid  LIMIT 1) fin_capital_10,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 5 AND mescod = '11' AND pcr.pliid = pli.pliid  LIMIT 1) fin_capital_11,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 5 AND mescod = '12' AND pcr.pliid = pli.pliid  LIMIT 1) fin_capital_12,

                -- Cronograma Financeiro Custeio
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 4 AND mescod = '01' AND pcr.pliid = pli.pliid LIMIT 1) fin_custeio_1,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 4 AND mescod = '02' AND pcr.pliid = pli.pliid LIMIT 1) fin_custeio_2,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 4 AND mescod = '03' AND pcr.pliid = pli.pliid LIMIT 1) fin_custeio_3,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 4 AND mescod = '04' AND pcr.pliid = pli.pliid LIMIT 1) fin_custeio_4,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 4 AND mescod = '05' AND pcr.pliid = pli.pliid LIMIT 1) fin_custeio_5,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 4 AND mescod = '06' AND pcr.pliid = pli.pliid LIMIT 1) fin_custeio_6,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 4 AND mescod = '07' AND pcr.pliid = pli.pliid LIMIT 1) fin_custeio_7,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 4 AND mescod = '08' AND pcr.pliid = pli.pliid LIMIT 1) fin_custeio_8,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 4 AND mescod = '09' AND pcr.pliid = pli.pliid LIMIT 1) fin_custeio_9,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 4 AND mescod = '10' AND pcr.pliid = pli.pliid LIMIT 1) fin_custeio_10,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 4 AND mescod = '11' AND pcr.pliid = pli.pliid LIMIT 1) fin_custeio_11,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 4 AND mescod = '12' AND pcr.pliid = pli.pliid LIMIT 1) fin_custeio_12,

                -- Cronograma Orçamentário Capital
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 3 AND mescod = '01' AND pcr.pliid = pli.pliid LIMIT 1) orc_capital_1,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 3 AND mescod = '02' AND pcr.pliid = pli.pliid LIMIT 1) orc_capital_2,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 3 AND mescod = '03' AND pcr.pliid = pli.pliid LIMIT 1) orc_capital_3,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 3 AND mescod = '04' AND pcr.pliid = pli.pliid LIMIT 1) orc_capital_4,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 3 AND mescod = '05' AND pcr.pliid = pli.pliid LIMIT 1) orc_capital_5,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 3 AND mescod = '06' AND pcr.pliid = pli.pliid LIMIT 1) orc_capital_6,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 3 AND mescod = '07' AND pcr.pliid = pli.pliid LIMIT 1) orc_capital_7,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 3 AND mescod = '08' AND pcr.pliid = pli.pliid LIMIT 1) orc_capital_8,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 3 AND mescod = '09' AND pcr.pliid = pli.pliid LIMIT 1) orc_capital_9,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 3 AND mescod = '10' AND pcr.pliid = pli.pliid LIMIT 1) orc_capital_10,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 3 AND mescod = '11' AND pcr.pliid = pli.pliid LIMIT 1) orc_capital_11,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 3 AND mescod = '12' AND pcr.pliid = pli.pliid LIMIT 1) orc_capital_12,

                -- Cronograma Orçamentário Custeio
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 2 AND mescod = '01' AND pcr.pliid = pli.pliid LIMIT 1) orc_custeio_1,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 2 AND mescod = '02' AND pcr.pliid = pli.pliid LIMIT 1) orc_custeio_2,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 2 AND mescod = '03' AND pcr.pliid = pli.pliid LIMIT 1) orc_custeio_3,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 2 AND mescod = '04' AND pcr.pliid = pli.pliid LIMIT 1) orc_custeio_4,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 2 AND mescod = '05' AND pcr.pliid = pli.pliid LIMIT 1) orc_custeio_5,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 2 AND mescod = '06' AND pcr.pliid = pli.pliid LIMIT 1) orc_custeio_6,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 2 AND mescod = '07' AND pcr.pliid = pli.pliid LIMIT 1) orc_custeio_7,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 2 AND mescod = '08' AND pcr.pliid = pli.pliid LIMIT 1) orc_custeio_8,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 2 AND mescod = '09' AND pcr.pliid = pli.pliid LIMIT 1) orc_custeio_9,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 2 AND mescod = '10' AND pcr.pliid = pli.pliid LIMIT 1) orc_custeio_10,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 2 AND mescod = '11' AND pcr.pliid = pli.pliid LIMIT 1) orc_custeio_11,
                (SELECT pcrvalor FROM planacomorc.pi_cronograma pcr WHERE crvid = 2 AND mescod = '12' AND pcr.pliid = pli.pliid LIMIT 1) orc_custeio_12,

                vlrautorizado, vlrempenhado, vlrliquidado, vlrpago,

		-- DADOS DO MONITORAMENTO
		ac.acoquantidade AS executado,
		ac.acoanalise AS analise,
		cl.cladsc AS classificacao,
		LTRIM(REPLACE(REPLACE(REPLACE(ARRAY(
                    SELECT DISTINCT
                        ' ' || meddsc
                    FROM acompanhamento.medida md
                        JOIN acompanhamento.acompanhamentomedida amd ON md.medid = amd.medid
                    WHERE
                        amd.acoid = ac.acoid
		)::TEXT, '{', ''), '}', ''), '\"', '')) AS medidas,
		ac.acoprovidencias AS providencias
            FROM monitora.pi_planointerno pli
                JOIN planacomorc.pi_complemento pic ON pic.pliid = pli.pliid
                JOIN public.vw_subunidadeorcamentaria suo ON(
                    suo.suocod = pli.ungcod 
                    AND suo.unocod = pli.unicod 
                    AND suo.prsano = pli.pliano
                )
                JOIN monitora.pi_enquadramentodespesa eqd ON eqd.eqdid = pli.eqdid
                LEFT JOIN monitora.enquadramentorp AS erp ON(eqd.eqdid = erp.eqdid)
                LEFT JOIN planacomorc.resultadoprimario AS rp ON(erp.irpcod::INTEGER = rp.rpcod)
                LEFT JOIN planacomorc.manutencaoitem mai ON mai.maiid = pic.maiid
                LEFT JOIN planacomorc.manutencaosubitem mas ON mas.masid = pic.masid
                LEFT JOIN monitora.pi_planointernoptres ppt ON ppt.pliid = pli.pliid
                LEFT JOIN monitora.vw_ptres ptr ON(
                    ptr.ptrid = ppt.ptrid
                    AND ptr.ptrano = pli.pliano
                )
                LEFT JOIN public.objetivoppa opp ON opp.oppid = pic.oppid
                LEFT JOIN public.iniciativappa ipp ON ipp.ippid = pic.ippid
                LEFT JOIN monitora.pi_modalidadeensino mde ON mde.mdeid = pli.mdeid
                LEFT JOIN monitora.pi_niveletapaensino nee ON nee.neeid = pli.neeid
                LEFT JOIN public.metappa mpp ON mpp.mppid = pic.mppid
                LEFT JOIN public.metapnc mpn ON mpn.mpnid = pic.mpnid
                LEFT JOIN monitora.pi_categoriaapropriacao cap ON cap.capid = pli.capid
                LEFT JOIN monitora.pi_produto ppr ON ppr.pprid = pic.pprid
                LEFT JOIN monitora.pi_unidade_medida pum ON pum.pumid = pic.pumid
                LEFT JOIN planacomorc.meta_unidade meu ON(pic.meuid = meu.meuid)
                LEFT JOIN planacomorc.objetivo_estrategico obe ON(meu.obeid = obe.obeid)
                LEFT JOIN planacomorc.dimensao_estrategica dim ON(meu.dimeid = dim.dimeid)
                LEFT JOIN planacomorc.meta_estrategica mee ON(meu.meeid = mee.meeid)
                LEFT JOIN workflow.documento doc ON doc.docid = pli.docid
                LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                LEFT JOIN territorios.esfera esf ON esf.esfid = pic.esfid
                LEFT JOIN public.meses mes ON mes.mescod = pic.mescod
                LEFT JOIN (
                    SELECT
                        sex.exercicio,
                        sex.plicod,
                        sum(sex.vlrautorizado) vlrautorizado,
                        sum(sex.vlrempenhado) vlrempenhado,
                        sum(sex.vlrliquidado) vlrliquidado,
                        sum(sex.vlrpago) vlrpago
                    FROM spo.siopexecucao sex
                    WHERE
                        COALESCE(sex.plicod, '') != ''
                    GROUP BY
                        sex.exercicio,
                        sex.plicod
                ) sex ON(
                    sex.plicod = pli.plicod
                    AND sex.exercicio = pli.pliano
                )
		-- DADOS DO MONITORAMENTO
		LEFT JOIN acompanhamento.acompanhamento ac ON pli.pliid = ac.pliid
		LEFT JOIN acompanhamento.classificacao cl ON ac.claid = cl.claid
            WHERE
                pli.plistatus = 'A'
                $where
        ";

        return $sql;
    }

    /**
     * Lista consulta do Relatório Geral de PI e PI FNC
     * 
     * @param stdClass $dto
     * @return array
     */
    public function listarRelatorioGeralPi(stdClass $dto)
    {
        $sql = $this->montarSqlRelatorioGeral($dto);

        $dados = $this->carregar($sql);
        return $dados? $dados: [];
    }

    public function getCorPainelPorValorDisponivel($valorDisponivel)
    {
        if ($valorDisponivel > 0) return '0000FF';

        if ($valorDisponivel < 0) return 'FF0000';

        return '008000';
    }

    public function recuperarPiPorSubunidade($suoid)
    {
        $dadosPi = $this->recuperarDadosDetalhe((object) array('suoid' => $suoid, 'tipo' => self::K_TIPO_DETALHE_PI));
        $dadosDelegado = $this->recuperarDadosDetalhe((object) array('suoid' => $suoid, 'tipo' => self::K_TIPO_DETALHE_DELEGADO));

        $dadosPiNaoOrcamentario = $this->recuperarDadosDetalhe((object) array('suoid' => $suoid, 'tipo' => self::K_TIPO_DETALHE_PI_NAO_ORCAMENTARIO));
        $dadosDelegadoNaoOrcamentario = $this->recuperarDadosDetalhe((object) array('suoid' => $suoid, 'tipo' => self::K_TIPO_DETALHE_DELEGADO_NAO_ORCAMENTARIO));

        include_once APPRAIZ . 'planacomorc/modulos/principal/unidade/detalhe-subunidade.inc';
    }

    public function recuperarPiPorFuncional(stdClass $parametros)
    {
        $dados = $this->recuperarDadosDetalhe((object) array('suoid' => $parametros->suoid, 'ptrid' => $parametros->ptrid, 'tipo' => self::K_TIPO_DETALHE_PTRES));
        $totaisFuncional = $this->buscarTotaisFuncional((object) array('exercicio' => $_SESSION['exercicio'], 'ptrid' => $parametros->ptrid));
        include_once APPRAIZ . 'planacomorc/modulos/principal/unidade/detalhe-ptres.inc';
    }

    public function recuperarDetalhesValoresGrafico($graficoSubunidade = TRUE, $tipoValor, $sigla, $percentualPlanejamento=false)
    {
        if($graficoSubunidade){
            $filtro = (object)array('suosigla' => $sigla);
        } else {
            $filtro = (object)array('unosigla' => $sigla);
        }

        switch($tipoValor){
            case 'Dot':
                $sql = self::montarSqlGraficoPizzaRp($filtro);
                break;
            case 'Pla':
                $sql = self::montarSqlGraficoPizzaEnquadramento($filtro);
                break;
        }

        $dados = $this->carregar($sql);
        if($dados){
            $grafico = new Grafico(Grafico::K_TIPO_PIZZA, false);
            $grafico->gerarGrafico($dados, $percentualPlanejamento);
        } else {
            echo "<script>$('#detalhe-grafico').modal('hide'); swal({title: 'Atenção', text: 'Nenhum dado encontrado.', type: 'warning', html: true});</script>";
        }
    }

    public static function montarSqlGraficoPizzaRp(stdClass $filtro){
        $where = '';
        $where .= $filtro->suosigla? "\n AND suo.suosigla = '$filtro->suosigla'": NULL;
        $where .= $filtro->unosigla? "\n AND suo.unosigla = '$filtro->unosigla'": NULL;

        $sql = "
            SELECT
                'RP ' || ptr.irpcod descricao,
                SUM(COALESCE(ptr.ptrdotacaocapital, 0) + COALESCE(ptr.ptrdotacaocusteio, 0)) valor
            FROM monitora.vw_ptres ptr
                JOIN spo.ptressubunidade psu ON ptr.ptrid = psu.ptrid
                JOIN public.vw_subunidadeorcamentaria suo ON psu.suoid = suo.suoid AND ptr.ptrano = suo.prsano
            WHERE
                ptr.ptrano = '{$_SESSION['exercicio']}' $where
            GROUP BY
                ptr.irpcod
        ";

        return $sql;
    }

    public static function montarSqlGraficoPizzaEnquadramento(stdClass $filtro){
        $where = '';
        $where .= $filtro->suosigla? "\n AND suo.suosigla = '$filtro->suosigla'": NULL;
        $where .= $filtro->unosigla? "\n AND suo.unosigla = '$filtro->unosigla'": NULL;
        $sql = "
            SELECT
                eqd.eqddsc descricao,
                SUM(COALESCE(pic.picvalorcapital, 0) + COALESCE(pic.picvalorcusteio, 0)) valor
            FROM public.vw_subunidadeorcamentaria suo
                JOIN spo.ptressubunidade psu ON psu.suoid = suo.suoid
                JOIN monitora.vw_ptres ptr ON ptr.ptrid = psu.ptrid AND ptr.ptrano = suo.prsano
                JOIN monitora.pi_planointernoptres ppt ON ppt.ptrid = ptr.ptrid
                JOIN monitora.pi_planointerno pli ON(pli.pliid = ppt.pliid AND pli.ungcod = suo.suocod AND pli.unicod = suo.unocod AND pli.plistatus = 'A')
                JOIN planacomorc.pi_complemento pic ON pic.pliid = pli.pliid
                JOIN monitora.pi_enquadramentodespesa eqd ON eqd.eqdid = pli.eqdid
            WHERE
                ptr.ptrano = '{$_SESSION['exercicio']}' $where
            GROUP BY
                eqd.eqddsc
        ";

        return $sql;
    }

    public function recuperarDadosDetalhe(stdClass $parametros)
    {
        $where = '';
        switch ($parametros->tipo) {
            case self::K_TIPO_DETALHE_PTRES:
                $where .= $parametros->ptrid? " AND ptr.ptrid = '$parametros->ptrid'": NULL;
                $where .= $parametros->suoid? " AND suo.suoid = '$parametros->suoid'": NULL;
                break;
            case self::K_TIPO_DETALHE_PI:
                $where .= " AND eqd.eqdcod != 'N'";
                $where .= " AND suo.suoid = '$parametros->suoid'";
                break;
            case self::K_TIPO_DETALHE_PI_NAO_ORCAMENTARIO:
                $where .= " AND eqd.eqdcod = 'N'";
                $where .= " AND suo.suoid = '$parametros->suoid'";
                break;
            case self::K_TIPO_DETALHE_DELEGADO:
                $where .= " AND eqd.eqdcod != 'N'";
                $where .= " AND pde.suoid = '$parametros->suoid'";
                break;
            case self::K_TIPO_DETALHE_DELEGADO_NAO_ORCAMENTARIO:
                $where .= " AND eqd.eqdcod = 'N'";
                $where .= " AND pde.suoid = '$parametros->suoid'";
                break;
        }

        $sql = "
            SELECT
                pli.pliid,
                pli.plititulo,
                pli.plicod,
                pli.pliemenda,
                pip.pprnome,
                pic.picquantidade,
                pic.picted,
                pic.picedital,
                pic.picvalorcapital,
                pic.picvalorcusteio,
                SUM(COALESCE(ex.vlrautorizado,0)) AS autorizado,
                SUM(COALESCE(ex.vlrempenhado,0)) AS empenhado,
                SUM(COALESCE(ex.vlrliquidado,0)) AS liquidado,
                SUM(COALESCE(ex.vlrpago,0)) AS pago,
                suo.suocod,
                suo.suonome,
                suo.unocod,
                suo.unonome,
                suo.unosigla,
                ptr.funcional,
                ptr.acatitulo,
                ptr.ptrdotacao,
                eqd.eqddsc,
                rp.irpcod
            FROM monitora.pi_planointerno pli
                JOIN planacomorc.pi_complemento pic ON pic.pliid = pli.pliid
                LEFT JOIN planacomorc.pi_delegacao pde ON pde.pliid = pli.pliid
                JOIN public.vw_subunidadeorcamentaria suo ON suo.suocod = pli.ungcod
                    AND suo.unocod = pli.unicod
                    AND suo.prsano = pli.pliano
                LEFT JOIN monitora.pi_planointernoptres ppt ON ppt.pliid = pli.pliid
                LEFT JOIN monitora.vw_ptres ptr ON ptr.ptrid = ppt.ptrid
                    AND ptr.ptrano = pli.pliano
                LEFT JOIN monitora.pi_enquadramentodespesa eqd ON eqd.eqdid = pli.eqdid
                LEFT JOIN monitora.enquadramentorp AS rp ON(eqd.eqdid = rp.eqdid)
                LEFT JOIN spo.siopexecucao ex ON(
                    ex.exercicio = ptr.ptrano
                    AND ex.plicod = pli.plicod
                )
                LEFT JOIN monitora.pi_produto AS pip ON pic.pprid = pip.pprid
            WHERE
                pli.plistatus = 'A'
                AND (ptr.irpcod <> '6' OR ptr.ptrid IS NULL)
                $where
            GROUP BY
                pli.pliid,
                pli.plititulo,
                pli.plicod,
                pli.pliemenda,
                pip.pprnome,
                pic.picquantidade,
                pic.picted,
                pic.picedital,
                pic.picvalorcapital,
                pic.picvalorcusteio,
                suo.suocod,
                suo.suonome,
                suo.unocod,
                suo.unonome,
                suo.unosigla,
                ptr.funcional,
                ptr.acatitulo,
                ptr.ptrdotacao,
                eqd.eqddsc,
                rp.irpcod
            ";

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }
    
    public function buscarTotaisFuncional(stdClass $parametros)
    {
        $where = '';
        $where .= $parametros->ptrid? " \n AND ptr.ptrid = '$parametros->ptrid'": NULL;

        $sql = "
            SELECT DISTINCT
                ptr.ptrid,
                ptr.funcional,
                ptr.ptrdotacaocusteio AS custeio,
                ptr.ptrdotacaocapital AS capital,
                sec.empenhado,
                sec.liquidado,
                sec.pago
            FROM monitora.vw_ptres ptr
                LEFT JOIN(
                    SELECT
                        siopexecucao.unicod,
                        siopexecucao.ptres,
                        SUM(COALESCE(siopexecucao.vlrautorizado, 0.00))::NUMERIC AS provisionado,
                        SUM(COALESCE(siopexecucao.vlrempenhado, 0.00))::NUMERIC AS empenhado,
                        SUM(COALESCE(siopexecucao.vlrliquidado, 0.00))::NUMERIC AS liquidado,
                        SUM(COALESCE(siopexecucao.vlrpago, 0.00))::NUMERIC AS pago
                    FROM spo.siopexecucao
                    WHERE
                        siopexecucao.exercicio = '". (int)$parametros->exercicio. "' -- Inserindo o ano direto na subquery por motivo de performance da consulta.
                    GROUP BY
                        siopexecucao.ptres,
                        siopexecucao.unicod
                ) sec ON(ptr.ptres = sec.ptres)
            WHERE
                ptr.ptrano = '". (int)$parametros->exercicio. "'
                $where
            ";

        $resultado = $this->pegaLinha($sql);
        return $resultado;
    }

    public function recuperarPis($filtro, $unoFundo = 'false')
    {
        if (is_array($filtro['eqdid']) && count($filtro['eqdid'])) {
            $where = ' and pli.eqdid in (' . implode(',', $filtro['eqdid']) . ')';
        }
        if (is_array($filtro['suoid']) && count($filtro['suoid'])) {
            $where .= " and suo.suocod in('" . implode("','", $filtro['suoid']) . "')";
        }

        $sql = "
            SELECT
                pli.docid, doc.esdid, pli.pliid, pli.plicod, pli.plidsc, pli.plititulo, ptr.acatitulo, ptr.funcional,
                doc.esdid, ptr.ptres, ptr.ptrid, ptr.prgcod, ptr.acacod, ptr.loccod, ptr.plocod, ptr.plodsc, ptr.irpcod,
                suo.unocod, suo.unosigla, suo.unonome, suo.suocod, suo.suosigla, suo.suonome, suo.unofundo
            FROM monitora.pi_planointerno AS pli
                JOIN workflow.documento AS doc ON doc.docid = pli.docid
                LEFT JOIN monitora.pi_planointernoptres AS pip ON pli.pliid = pip.pliid
                LEFT JOIN monitora.vw_ptres AS ptr ON pip.ptrid = ptr.ptrid
                JOIN public.vw_subunidadeorcamentaria AS suo ON(
                    suo.suocod = pli.ungcod
                    AND suo.unocod = pli.unicod
                    AND suo.prsano = pli.pliano
                )
                JOIN monitora.pi_enquadramentodespesa AS eqd ON pli.eqdid = eqd.eqdid
            WHERE
                pli.pliano = '{$filtro['exercicio']}'
                AND (pli.plistatus = 'A' OR (pli.plistatus = 'I' AND doc.esdid = 1772))
                AND suo.unofundo = " . $unoFundo . "
                AND doc.esdid = '{$filtro['esdid']}'
            $where
        ";

        return $this->carregar($sql);
    }

    public function tramitarLote($aedid, $aDocid, $cmddsc = null)
    {
        $cmddsc = $cmddsc ? $cmddsc : 'Tramitado em lote';
        if(is_array($aDocid)){
            foreach($aDocid as $pliid => $docid){
                wf_alterarEstado($docid, $aedid, $cmddsc, array('pliid' => $pliid));
            }
        }

    }

    public function recuperarResumoPorMetaESubunidade(stdClass $dto)
    {
        $exercicio = (int)$_SESSION['exercicio'];

        $where = '';
        $dto->mppid ? $where .= "\n  AND pic.mppid = '" . (int)$dto->mppid . "'" : NULL;
        $dto->meuid ? $where .= "\n  AND pic.meuid = '" . (int)$dto->meuid . "'" : NULL;
        $dto->suoid ? $where .= "\n  AND suo.suoid = '" . (int)$dto->suoid . "'" : NULL;

        $sql = "
            SELECT
                COUNT(pli.pliid) qtd,
                SUM(COALESCE(pic.picvalorcapital, 0)) picvalorcapital,
                SUM(COALESCE(pic.picvalorcusteio, 0)) picvalorcusteio,
                SUM(COALESCE(pic.picvalorcapital, 0) + coalesce(pic.picvalorcusteio, 0)) previsto,
                SUM(COALESCE(sex.vlrautorizado, 0)) AS provisionado,
                SUM(COALESCE(sex.vlrempenhado, 0)) AS empenhado,
                SUM(COALESCE(sex.vlrliquidado, 0)) AS liquidado,
                SUM(COALESCE(sex.vlrpago, 0)) AS pago
            FROM monitora.pi_planointerno pli
                LEFT JOIN planacomorc.pi_complemento pic ON pic.pliid = pli.pliid
                JOIN public.vw_subunidadeorcamentaria suo ON(
                    suo.unocod = pli.unicod
                    AND suo.suocod = pli.ungcod
                    AND suo.prsano = pli.pliano
                )
                LEFT JOIN (
                    SELECT
                        sex.plicod,
                        SUM(sex.vlrautorizado) vlrautorizado,
                        SUM(sex.vlrempenhado) vlrempenhado,
                        SUM(sex.vlrliquidado) vlrliquidado,
                        SUM(sex.vlrpago) vlrpago
                    FROM spo.siopexecucao sex
                    WHERE
                        sex.plicod IS NOT NULL
                        AND sex.exercicio = '" . $exercicio . "'
                    GROUP BY
                        sex.plicod
                ) sex ON(
                    sex.plicod = pli.plicod
                )
            WHERE
                pli.plistatus = 'A'
                AND pli.pliano = '" . $exercicio . "'
                {$where}
        ";

        return $this->pegaLinha($sql);
    }

    public function recuperarValoresPiMetaPncESubunidade($mpnid, $suoid, $exercicio = null)
    {
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        $sql = "
            SELECT
                count(*) qtd,
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
                    AND pli.pliano = '$exercicio'
                    AND mpnid = " . (int)$mpnid . "
                    AND suo.suoid = " . (int)$suoid;

        return $this->pegaLinha($sql);
    }

    public function recuperarPiPorMetaESubunidade(stdClass $params, $exercicio = null)
    {
        $exercicio = (int)$exercicio ? (int)$exercicio : (int)$_SESSION['exercicio'];
        $suoid = (int)$params->suoid;

        if ($params->tipo === 'meta-unidade') {
            $params->meuid  = (int)$params->meuid;
            $where = "\n  AND pic.meuid = '{$params->meuid}'";
        } else if ($params->tipo === 'meta-ppa') {
            $params->mppid  = (int)$params->mppid;
            $where = "\n  AND pic.mppid = '{$params->mppid}'";
        }

        $sql = "
            SELECT
                pli.pliid,
                pli.plicod,
                pli.plititulo,
                COALESCE(pic.picvalorcapital, 0) picvalorcapital,
                COALESCE(pic.picvalorcusteio, 0) picvalorcusteio,
                COALESCE(pic.picvalorcapital, 0) + COALESCE(pic.picvalorcusteio, 0) previsto,
                COALESCE(sex.vlrautorizado, 0) AS autorizado,
                COALESCE(sex.vlrempenhado, 0) AS empenhado,
                COALESCE(sex.vlrliquidado, 0) AS liquidado,
                COALESCE(sex.vlrpago, 0) AS pago
            FROM monitora.pi_planointerno pli
                JOIN planacomorc.pi_complemento pic ON pic.pliid = pli.pliid
                JOIN public.vw_subunidadeorcamentaria suo ON suo.unocod = pli.unicod
                    AND suo.suocod = pli.ungcod
                    AND suo.prsano = pli.pliano
                LEFT JOIN (
                    SELECT
                        sex.exercicio,
                        sex.plicod,
                        SUM(sex.vlrautorizado) vlrautorizado,
                        SUM(sex.vlrempenhado) vlrempenhado,
                        SUM(sex.vlrliquidado) vlrliquidado,
                        SUM(sex.vlrpago) vlrpago
                    FROM
                        spo.siopexecucao sex
                    WHERE
                        COALESCE(sex.plicod, '') != ''
                    GROUP BY
                        sex.exercicio,
                        sex.plicod
                ) sex ON(
                    sex.plicod = pli.plicod
                    AND sex.exercicio = pli.pliano
                )
            WHERE
                pli.plistatus = 'A'
                AND pli.pliano = '". $exercicio. "'
                {$where}
                AND suo.suoid = '". $suoid. "'
        ";

        return $this->carregar($sql);
    }

    public function recuperarPiPorIndicadorESubunidade($ipnid, $suoid, $exercicio = null){
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        $sql = "
            SELECT
                pli.pliid,
                pli.plicod,
                pli.plititulo,
                coalesce(pic.picvalorcapital, 0) picvalorcapital,
                coalesce(pic.picvalorcusteio, 0) picvalorcusteio,
                coalesce(pic.picvalorcapital, 0) + coalesce(pic.picvalorcusteio, 0) previsto,
                coalesce(sex.vlrautorizado, 0) AS autorizado,
                coalesce(sex.vlrempenhado, 0) AS empenhado,
                coalesce(sex.vlrliquidado, 0) AS liquidado,
                coalesce(sex.vlrpago, 0) AS pago
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
                        sex.plicod
                ) sex ON(sex.plicod = pli.plicod AND sex.exercicio = pli.pliano)
                WHERE
                    pli.plistatus = 'A'
                    AND pli.pliano = '$exercicio'
                    AND pic.ipnid = ". (int)$ipnid. "
                    AND suo.suoid = ". (int)$suoid;

        return $this->carregar($sql);
    }
    
    public function recuperarPiPorMetaPncESubunidade($mpnid, $suoid, $exercicio = null){
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        $sql = "
            SELECT
                pli.pliid,
                pli.plicod,
                pli.plititulo,
                coalesce(pic.picvalorcapital, 0) picvalorcapital,
                coalesce(pic.picvalorcusteio, 0) picvalorcusteio,
                coalesce(pic.picvalorcapital, 0) + coalesce(pic.picvalorcusteio, 0) previsto,
                coalesce(sex.vlrautorizado, 0) AS autorizado,
                coalesce(sex.vlrempenhado, 0) AS empenhado,
                coalesce(sex.vlrliquidado, 0) AS liquidado,
                coalesce(sex.vlrpago, 0) AS pago
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
                        sex.plicod
                ) sex ON(sex.plicod = pli.plicod AND sex.exercicio = pli.pliano)
                WHERE
                    pli.plistatus = 'A'
                    AND pli.pliano = '$exercicio'
                    AND pic.mpnid = ". (int)$mpnid. "
                    AND suo.suoid = ". (int)$suoid;

        return $this->carregar($sql);
    }

    public function recuperarPorUnidade(stdClass $filtros = NULL)
    {
        $where = $filtros->naoOrcamentaria === TRUE? "\n AND eqd.eqdcod = '". Monitora_Model_PiEnquadramentoDespesa::EQDCOD_NAO_ORCAMENTARIA ."'": "AND eqd.eqdcod != '". Monitora_Model_PiEnquadramentoDespesa::EQDCOD_NAO_ORCAMENTARIA ."'";

        $sql = "
            SELECT
                pli.pliid,
                pli.plititulo,
                pli.plicod,
                coalesce(pic.picvalorcapital, 0) + coalesce(pic.picvalorcusteio, 0) valor,
                pic.picquantidade,
                aco.acoid,
                aco.acoquantidade,
                cla.cladsc,
                suo.unofundo,
                pic.pprid,
                pic.pumid,
                ppr.pprnome,
                pum.pumnome,
                ptr.funcional,
                ptr.plocod,
                ptr.plodsc,
                ptr.acatitulo,
                opp.oppid,
                opp.oppcod,
                opp.oppdsc,
                opp.oppnome
            FROM monitora.pi_planointerno pli
                JOIN public.vw_subunidadeorcamentaria suo ON(
                    suo.unocod = pli.unicod
                    AND suo.suocod = pli.ungcod
                    AND suo.prsano = pli.pliano
                )
                LEFT JOIN monitora.pi_enquadramentodespesa eqd ON(
                    pli.eqdid = eqd.eqdid
                    AND pli.pliano = eqd.eqdano
                )
                JOIN planacomorc.pi_complemento pic ON pic.pliid = pli.pliid
                LEFT JOIN monitora.pi_planointernoptres ppt ON ppt.pliid = pli.pliid
                LEFT JOIN monitora.vw_ptres ptr ON(
                    ptr.ptrid = ppt.ptrid
                    AND ptr.ptrano = pli.pliano
                )
                LEFT JOIN monitora.pi_produto ppr ON ppr.pprid = pic.pprid
                LEFT JOIN monitora.pi_unidade_medida pum ON pum.pumid = pic.pumid
                LEFT JOIN public.objetivoppa opp ON opp.oppid = pic.oppid
                LEFT JOIN acompanhamento.acompanhamento aco ON(
                    aco.acostatus = 'A'
                    AND aco.pliid = pli.pliid
                )
                LEFT JOIN acompanhamento.classificacao cla ON aco.claid = cla.claid
            WHERE
                pli.plistatus = 'A'
                AND pli.pliano = '". (int)$filtros->exercicio. "'
                AND pli.ungcod = (
                    SELECT
                        suocod
                    FROM public.vw_subunidadeorcamentaria
                    WHERE
                        suoid = ". (int)$filtros->suoid. "
                )
                $where
            ORDER BY
                suo.unofundo,
                pum.pumnome,
                pli.plititulo
        ";
//ver($sql);
        return $this->carregar($sql);
    }

    public function recuperarQuantidadePrevista($mes = null)
    {
        $mes = $mes ? $mes : date('m');
        $sql = "select sum(pcrvalor)
                from planacomorc.pi_cronograma pcr
                where crvid = 1
                AND mescod::int <= $mes
                AND pcr.pliid = " . (int)$this->pliid;

        return $this->pegaUm($sql);
    }

    public function recuperarUltimoHistoricoAcompanhamento($mes = null)
    {
        $sql = "select his.*, usu.usunome, to_char(hisdata, 'DD/MM/YYYY HH24:MI:SS') hisdata
                from acompanhamento.historico his
                        inner join acompanhamento.acompanhamento aco on aco.acoid = his.acoid
                        inner join seguranca.usuario usu on usu.usucpf = his.usucpf
                where aco.pliid = " . (int)$this->pliid . "
                order by his.hisdata desc";

        return $this->pegaLinha($sql);
    }

    public function recuperarDados(array $filtro = [], $order = null)
    {
        $filtro['pliano'] = $filtro['exercicio'] ? $filtro['exercicio'] : $_SESSION['exercicio'];
        $order = $order ? $order : ['pli.suonome', 'pli.suonome', 'pli.funcional'];

        if($this->pliid){
            $where[] = ' pli.pliid = ' . (int) $this->pliid;
        }

        foreach($filtro as $campo => $valor){

            if(is_array($valor) && count($valor)){
                $where[] = " pli.{$campo} in '" . implode("', '", $valor) . "' ";
            } elseif(!empty($valor)) {
                $where[] = " pli.{$campo} = '$valor' ";
            }
        }

        $where = ' where ' . implode(' and ', $where);
        $order = ' order by ' . implode(', ', $order);

        $sql = "
            SELECT
                pliid, plicod, plititulo, plidsc, pliano, picquantidade, picvalorcapital,
                picvalorcusteio, previsto, autorizado, empenhado, liquidado,
                pago, acoid, acoquantidade, acodata, esddsc, unoid, unocod, unonome,
                unosigla, suoid, suocod, suonome, suosigla, eqddsc, eqdcod, resultadoprimario,
                mainome, masnome, prgcod, prgdsc, oppcod, oppnome, ippcod, ippnome,
                acacod, acatitulo, loccod, locdsc, plocod, po, ptres, mdedsc,
                mpnnome, mppnome, needsc, capdsc, ptrid, funcional, acaid, pliemenda,
                picedital, pumnome, pprid, pprnome, emenda, edital, esfdsc, mppid,
                mpnid, oppid, pumid, ipnid, ippid
            FROM monitora.vw_planointerno pli
            $where
            $order
        ";

        return $this->carregar($sql);
    }

    public function exportarXlsUnidade($exercicio){
        # Criando Planilha "Financeiro"
        $PHPExcel = new PHPExcel();
        $planilhaFinanceiro = $PHPExcel->getActiveSheet();
        $planilhaFinanceiro->setTitle("Acompanhamento FNC-Por Unidade");

        # Criando cabeçalhos
        $listaColunasCabecalho = [
            'Unidade',
            'Apresentado',
            'Selecionado',
            'Aprovado',
            'Provisionado',
            'Empenhado',
            'Liquidado',
            'Pago',
        ];
        $linha = 1;
        foreach($listaColunasCabecalho as $numeroColuna => $nome){
            $planilhaFinanceiro->setCellValueByColumnAndRow($numeroColuna, $linha, utf8_encode($nome));
        }
        
        # Busca Subunidades do FNC
        $listaSubunidadeFnc = $this->listarSubunidadeFnc((object)array('exercicio' => $_SESSION['exercicio']));
        $totalApresentado = 0;
        $totalSelecionado = 0;
        $totalAprovado = 0;
        $totalProvisionado = 0;
        $totalEmpenhado = 0;
        $totalLiquidado = 0;
        $totalPago = 0;

        foreach($listaSubunidadeFnc as $subUnidadeFnc){
            $linha++;
            $coluna = 0;
            $nome = $subUnidadeFnc['unocod']. ' - '. $subUnidadeFnc['suosigla']. ' - '. $subUnidadeFnc['suonome'];
            $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, utf8_encode($nome));
            
            # Valor da coluna apresentado
            $apresentado = $this->buscarTotalFnc((object) array(
                'exercicio' => $_SESSION['exercicio'],
                'suoid' => $subUnidadeFnc['suoid'],
                'esdid' => array(
                    ESD_FNC_PI_EM_ANALISE,
                    ESD_FNC_PI_BANCO_PROJETOS,
                    ESD_FNC_PI_DELIBERACAO_CFNC,
                    ESD_FNC_PI_AGUARDANDO_CORRECAO,
                    ESD_FNC_PI_SELECIONADO_CFNC,
                    ESD_FNC_PI_APROVADO
                )
            ));
            $totalApresentado += $apresentado;
            $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $apresentado);

            # Valor da coluna selecionado
            $selecionado = $this->buscarTotalFnc((object) array(
                'exercicio' => $_SESSION['exercicio'],
                'suoid' => $subUnidadeFnc['suoid'],
                'esdid' => array(
                    ESD_FNC_PI_SELECIONADO_CFNC,
                    ESD_FNC_PI_APROVADO
                )
            ));
            $totalSelecionado += $selecionado;
            $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $selecionado);
            
            # Valor da coluna Aprovado
            $aprovado = $this->buscarTotalFnc((object) array(
                'exercicio' => $_SESSION['exercicio'],
                'suoid' => $subUnidadeFnc['suoid'],
                'esdid' => array(ESD_FNC_PI_APROVADO)
            ));
            $totalAprovado += $aprovado;
            $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $aprovado);
            
            # Valor da coluna Provisionado
            $provisionado = $this->buscarTotalExecucaoFnc((object) array(
                'tipo' => 'provisionado',
                'exercicio' => $_SESSION['exercicio'],
                'suoid' => $subUnidadeFnc['suoid'],
            ));
            $totalProvisionado += $provisionado;
            $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $provisionado);
            
            # Valor da coluna Empenhado
            $empenhado = $this->buscarTotalExecucaoFnc((object) array(
                'tipo' => 'empenhado',
                'exercicio' => $_SESSION['exercicio'],
                'suoid' => $subUnidadeFnc['suoid'],
            ));
            $totalEmpenhado += $empenhado;
            $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $empenhado);
            
            # Valor da coluna Liquidado
            $liquidado = $this->buscarTotalExecucaoFnc((object) array(
                'tipo' => 'liquidado',
                'exercicio' => $_SESSION['exercicio'],
                'suoid' => $subUnidadeFnc['suoid'],
            ));
            $totalLiquidado += $liquidado;
            $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $liquidado);
            
            # Valor da coluna Empenhado
            $pago = $this->buscarTotalExecucaoFnc((object) array(
                'tipo' => 'pago',
                'exercicio' => $_SESSION['exercicio'],
                'suoid' => $subUnidadeFnc['suoid'],
            ));
            $totalPago += $pago;
            $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $pago);
        }

        $coluna = 0;
        $linha++;
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, 'TOTAL');
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalApresentado);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalSelecionado);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalAprovado);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalProvisionado);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalEmpenhado);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalLiquidado);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalPago);

        # Indicação da criação do ficheiro
        $objWriter = PHPExcel_IOFactory::createWriter($PHPExcel, 'Excel5');

        # Encaminhar o ficheiro resultante para abrir no browser ou fazer download
        header('Content-Type: application/vnd.ms-excel');
        header('Content-Disposition: attachment;filename="painel-fnc-unidade.xls"');
        header('Cache-Control: max-age=0');
        $objWriter->save('php://output');
    }

    public function exportarXlsAcao($exercicio){
        # Criando Planilha "Financeiro"
        $PHPExcel = new PHPExcel();
        $planilhaFinanceiro = $PHPExcel->getActiveSheet();
        $planilhaFinanceiro->setTitle("Acompanhamento FNC-Por Ação");

        # Criando cabeçalhos
        $listaColunasCabecalho = [
            'Ação',
            'Dotação',
            'Apresentado',
            'Selecionado',
            'Aprovado',
            'Provisionado',
            'Empenhado',
            'Liquidado',
            'Pago',
        ];
        $linha=1;
        $coluna = 0;
        foreach($listaColunasCabecalho as $numeroColuna => $nome){
            $planilhaFinanceiro->setCellValueByColumnAndRow($numeroColuna, $linha, utf8_encode($nome));
        }

        # Busca Funcionais do FNC
        $listaFuncionais = $this->listarFuncionaisFnc((object)array('exercicio' => $_SESSION['exercicio']));
        
        $totalDotacaoCusteio = 0;
        $totalDotacaoCapital = 0;
        # Previsto/Planejado/Aberto em PI
        $totalApresentadoCusteio = 0;
        $totalApresentadoCapital = 0;
        $totalSelecionadoCusteio = 0;
        $totalSelecionadoCapital = 0;
        $totalAprovadoCusteio = 0;
        $totalAprovadoCapital = 0;
        # Execução Orçamentária
        $totalProvisionadoCusteio = 0;
        $totalProvisionadoCapital = 0;
        $totalEmpenhadoCusteio = 0;
        $totalEmpenhadoCapital = 0;
        $totalLiquidadoCusteio = 0;
        $totalLiquidadoCapital = 0;
        $totalPagoCusteio = 0;
        $totalPagoCapital = 0;
        # Formatação da lista
        $funcionalAnterior = NULL;
        foreach($listaFuncionais as $funcional){
            $linha++;
            $coluna = 0;
            $nome = $funcional['funcional']. ' ('. $funcional['tipo']. ') - '. $funcional['acatitulo']. ': '. $funcional['plodsc']. ' (RP '. $funcional['irpcod']. ')';
            $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, utf8_encode($nome));
            
            if($funcional['tipo'] == 'CUSTEIO'){
                $totalDotacaoCusteio += $funcional['dotacao'];
            } else {
                $totalDotacaoCapital += $funcional['dotacao'];
            }
            $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $funcional['dotacao']);
            
            # Valor da coluna apresentado
            $apresentado = $this->buscarTotalFnc((object) array(
                'tipo' => $funcional['tipo'],
                'exercicio' => $_SESSION['exercicio'],
                'ptrid' => $funcional['ptrid'],
                'esdid' => array(
                    ESD_FNC_PI_EM_ANALISE,
                    ESD_FNC_PI_BANCO_PROJETOS,
                    ESD_FNC_PI_DELIBERACAO_CFNC,
                    ESD_FNC_PI_AGUARDANDO_CORRECAO,
                    ESD_FNC_PI_SELECIONADO_CFNC,
                    ESD_FNC_PI_APROVADO
                )
            ));
            if($funcional['tipo'] == 'CUSTEIO'){
                $totalApresentadoCusteio += $apresentado;
            } else {
                $totalApresentadoCusteio += $apresentado;
            }
            $disponivel = $funcional['dotacao'] - $apresentado;
            $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $apresentado);
            $planilhaFinanceiro->getStyle(PHPExcel_Cell::stringFromColumnIndex($coluna-1) . $linha)->applyFromArray(array(
                'font' => array(
                    'color' => array('rgb' => $this->getCorPainelPorValorDisponivel((int)$disponivel))
                )
            ));
            
            # Valor da coluna selecionado
            $selecionado = $this->buscarTotalFnc((object) array(
                'tipo' => $funcional['tipo'],
                'exercicio' => $_SESSION['exercicio'],
                'ptrid' => $funcional['ptrid'],
                'esdid' => array(
                    ESD_FNC_PI_SELECIONADO_CFNC,
                    ESD_FNC_PI_APROVADO
                )
            ));
            if($funcional['tipo'] == 'CUSTEIO'){
                $totalSelecionadoCusteio += $selecionado;
            } else {
                $totalSelecionadoCapital += $selecionado;
            }
            $disponivel = $funcional['dotacao'] - $selecionado;
            $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $selecionado);
            $planilhaFinanceiro->getStyle(PHPExcel_Cell::stringFromColumnIndex($coluna-1) . $linha)->applyFromArray(array(
                'font' => array(
                    'color' => array('rgb' => $this->getCorPainelPorValorDisponivel((int)$disponivel))
                )
            ));
            
            # Valor da coluna aprovado
            $aprovado = $this->buscarTotalFnc((object) array(
                'tipo' => $funcional['tipo'],
                'exercicio' => $_SESSION['exercicio'],
                'ptrid' => $funcional['ptrid'],
                'esdid' => array(
                    ESD_FNC_PI_APROVADO
                )
            ));
            if($funcional['tipo'] == 'CUSTEIO'){
                $totalAprovadoCusteio += $aprovado;
            } else {
                $totalAprovadoCapital += $aprovado;
            }
            $disponivel = $funcional['dotacao'] - $aprovado;
            $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $aprovado);
            $planilhaFinanceiro->getStyle(PHPExcel_Cell::stringFromColumnIndex($coluna-1) . $linha)->applyFromArray(array(
                'font' => array(
                    'color' => array('rgb' => $this->getCorPainelPorValorDisponivel((int)$disponivel))
                )
            ));
            
            # Valor da coluna Provisionado
            $provisionado = $this->buscarTotalExecucaoFnc((object) array(
                'tipo' => 'provisionado',
                'exercicio' => $_SESSION['exercicio'],
                'ptrid' => $funcional['ptrid'],
                'gnd' => $funcional['tipo']
            ));
            if($funcional['tipo'] == 'CUSTEIO'){
                $totalProvisionadoCusteio += $provisionado;
            } else {
                $totalProvisionadoCapital += $provisionado;
            }
            $disponivel = $funcional['dotacao'] - $provisionado;
            $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $provisionado);
            $planilhaFinanceiro->getStyle(PHPExcel_Cell::stringFromColumnIndex($coluna-1) . $linha)->applyFromArray(array(
                'font' => array(
                    'color' => array('rgb' => $this->getCorPainelPorValorDisponivel((int)$disponivel))
                )
            ));
            
            # Valor da coluna Empenhado
            $empenhado = $this->buscarTotalExecucaoFnc((object) array(
                'tipo' => 'empenhado',
                'exercicio' => $_SESSION['exercicio'],
                'ptrid' => $funcional['ptrid'],
                'gnd' => $funcional['tipo']
            ));
            if($funcional['tipo'] == 'CUSTEIO'){
                $totalEmpenhadoCusteio += $empenhado;
            } else {
                $totalEmpenhadoCapital += $empenhado;
            }
            $disponivel = $funcional['dotacao'] - $empenhado;
            $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $empenhado);
            $planilhaFinanceiro->getStyle(PHPExcel_Cell::stringFromColumnIndex($coluna-1) . $linha)->applyFromArray(array(
                'font' => array(
                    'color' => array('rgb' => $this->getCorPainelPorValorDisponivel((int)$disponivel))
                )
            ));
            
            # Valor da coluna Liquidado
            $liquidado = $this->buscarTotalExecucaoFnc((object) array(
                'tipo' => 'liquidado',
                'exercicio' => $_SESSION['exercicio'],
                'ptrid' => $funcional['ptrid'],
                'gnd' => $funcional['tipo']
            ));

            if($funcional['tipo'] == 'CUSTEIO'){
                $totalLiquidadoCusteio += $liquidado;
            } else {
                $totalLiquidadoCapital += $liquidado;
            }
            $disponivel = $funcional['dotacao'] - $liquidado;
            $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $liquidado);
            $planilhaFinanceiro->getStyle(PHPExcel_Cell::stringFromColumnIndex($coluna-1) . $linha)->applyFromArray(array(
                'font' => array(
                    'color' => array('rgb' => $this->getCorPainelPorValorDisponivel((int)$disponivel))
                )
            ));
            
            # Valor da coluna Pago
            $pago = $this->buscarTotalExecucaoFnc((object) array(
                'tipo' => 'pago',
                'exercicio' => $_SESSION['exercicio'],
                'ptrid' => $funcional['ptrid'],
                'gnd' => $funcional['tipo']
            ));
            if($funcional['tipo'] == 'CUSTEIO'){
                $totalPagoCusteio += $pago;
            } else {
                $totalPagoCapital += $pago;
            }
            $disponivel = $funcional['dotacao'] - $pago;
            $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $pago);
            $planilhaFinanceiro->getStyle(PHPExcel_Cell::stringFromColumnIndex($coluna-1) . $linha)->applyFromArray(array(
                'font' => array(
                    'color' => array('rgb' => $this->getCorPainelPorValorDisponivel((int)$disponivel))
                )
            ));
        }
        
        $coluna=0;
        $linha++;
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, "TOTAL CUSTEIO");
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalDotacaoCusteio);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalApresentadoCusteio);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalSelecionadoCusteio);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalAprovadoCusteio);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalProvisionadoCusteio);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalEmpenhadoCusteio);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalLiquidadoCusteio);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalPagoCusteio);

        $linha++;
        $coluna=0;
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, "TOTAL CAPITAL");
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalDotacaoCapital);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalApresentadoCapital);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalSelecionadoCapital);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalAprovadoCapital);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalProvisionadoCapital);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalEmpenhadoCapital);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalLiquidadoCapital);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalPagoCapital);

        $linha++;
        $coluna=0;
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, "TOTAL");
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalDotacaoCusteio + $totalDotacaoCapital);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalApresentadoCusteio + $totalApresentadoCapital);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalSelecionadoCusteio + $totalSelecionadoCapital);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalAprovadoCusteio + $totalAprovadoCapital);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalProvisionadoCusteio + $totalProvisionadoCapital);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalEmpenhadoCusteio + $totalEmpenhadoCapital);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalLiquidadoCusteio + $totalLiquidadoCapital);
        $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++, $linha, $totalPagoCusteio + $totalPagoCapital);
        
        # Indicação da criação do ficheiro
        $objWriter = PHPExcel_IOFactory::createWriter($PHPExcel, 'Excel5');

        # Encaminhar o ficheiro resultante para abrir no browser ou fazer download
        header('Content-Type: application/vnd.ms-excel');
        header('Content-Disposition: attachment;filename="painel-fnc-acao.xls"');
        header('Cache-Control: max-age=0');
        $objWriter->save('php://output');
    }
    
    /**
     * Consulta os dados de pedido de Alteração Orçamentária do PI.
     * 
     * @param stdClass $params
     * @return stdClass
     */
    public function consultarPedidoAlteracaoEfetivado(stdClass $params){
        $where = $params->pliid? "\n AND plise.pliid = ". (int)$params->pliid: NULL;
        $sql = "
            SELECT
                ped.pedid,
                ped.pedtitulo,
                tpd.tpddsc,
                COALESCE(plise.vldotacaofisico, 0) AS resultado_provavel_fisico,
                COALESCE(plise.vldotacaocusteio, 0) AS resultado_provavel_custeio,
                COALESCE(plise.vldotacaocapital, 0) AS resultado_provavel_capital,
                COALESCE((
                    SELECT
                        SUM(COALESCE(pcrvalor, 0)) AS total
                    FROM planacomorc.pi_cronograma crv
                    WHERE
                        crv.crvid = ". (int)Cronograma::K_FISICO. "
                        AND crv.pliid = plise.pliid
                ), 0) AS total_fisico,
                COALESCE((
                    SELECT
                        SUM(COALESCE(pcrvalor, 0)) AS total
                    FROM planacomorc.pi_cronograma crv
                    WHERE
                        crv.crvid = ". (int)Cronograma::ORCAMENTO_CUSTEIO. "
                        AND crv.pliid = plise.pliid
                ), 0) AS total_orcamentario_custeio,
                COALESCE((
                    SELECT
                        SUM(COALESCE(pcrvalor, 0)) AS total
                    FROM planacomorc.pi_cronograma crv
                    WHERE
                        crv.crvid = ". (int)Cronograma::ORCAMENTO_CAPITAL. "
                        AND crv.pliid = plise.pliid
                ), 0) AS total_orcamentario_capital,
                COALESCE((
                    SELECT
                        SUM(COALESCE(pcrvalor, 0)) AS total
                    FROM planacomorc.pi_cronograma crv
                    WHERE
                        crv.crvid = ". (int)Cronograma::FINANCEIRO_CUSTEIO. "
                        AND crv.pliid = plise.pliid
                ), 0) AS total_financeiro_custeio,
                COALESCE((
                    SELECT
                        SUM(COALESCE(pcrvalor, 0)) AS total
                    FROM planacomorc.pi_cronograma crv
                    WHERE
                        crv.crvid = ". (int)Cronograma::FINANCEIRO_CAPITAL. "
                        AND crv.pliid = plise.pliid
                ), 0) AS total_financeiro_capital,
                (
                    SELECT
                        htd.htddata
                    FROM workflow.historicodocumento htd
                    WHERE
                        doc.docid = htd.docid
                    ORDER BY
                        htd.htddata DESC
                    LIMIT 1
                ) AS quando_fez
            FROM alteracao.pedido ped
                JOIN workflow.documento doc ON(ped.docid = doc.docid)
                JOIN workflow.estadodocumento esd ON(doc.esdid = esd.esdid)
                JOIN workflow.tipodocumento tpd ON(esd.tpdid = tpd.tpdid)
                JOIN alteracao.plano_interno_selecionado plise ON(ped.pedid = plise.pedid and plise.pliselstatus = 'A')
            WHERE
                ped.pedstatus = 'A'
                AND doc.esdid IN(". (int)ESDID_ALTERACAO_EFETIVADO_INTERNO. ",". (int)ESDID_ALTERACAO_EFETIVADO_EXTERNO. ")
                $where
            ORDER BY
                quando_fez DESC
        ";
//ver($sql, d);
        $pedido = $this->pegaLinha($sql);
        return $pedido? (object)$pedido: new stdClass();
    }
}
