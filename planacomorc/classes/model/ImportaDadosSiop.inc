<?php
/**
 * Classe de mapeamento da entidade planacomorc.historico_pi
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO <rafael.carneiro@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 06-07-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


class Planacomorc_Model_ImportaDadosSiop extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = '';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        '',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        '',
        '',
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array();
    
    public function excluiDadosWsOrcamentario($exercicio){
        $sql = "DELETE FROM wssof.ws_execucaoorcamentariadto WHERE anoexercicio = ". (int)$exercicio;
        $this->executar($sql);
    }
    
    public function excluiDadosSiopExecucao($exercicio){
        $sql = "DELETE FROM spo.siopexecucao WHERE exercicio = '". (int)$exercicio. "'";
        $this->executar($sql);
    }
    
    public function insereDadosSiopExecucao($exercicio){
        $sql = "
            INSERT INTO spo.siopexecucao(
                exercicio,
                esfcod,
                unicod,
                funcod,
                sfucod,
                prgcod,
                acacod,
                loccod,
                plocod,
                ptres,
                plicod,
                vlrdotacaoinicial,
                vlrdotacaoatual,
                vlrempenhado,
                vlrliquidado,
                vlrpago,
                vlrautorizado
            )
            SELECT
                exo.anoexercicio AS exercicio,
                exo.esfera AS esfcod,
                exo.unidadeorcamentaria AS unicod,
                exo.funcao AS funcod,
                exo.subfuncao AS sfucod,
                exo.programa AS prgcod,
                exo.acao AS acacod,
                exo.localizador AS loccod,
                exo.planoorcamentario AS plocod,
                exo.numeroptres AS ptres,
                exo.planointerno AS plicod,
                CASE WHEN exo.dotacaoinicial <> '' THEN
                    exo.dotacaoinicial::NUMERIC
                ELSE
                    0
                END AS vlrdotacaoinicial,
                CASE WHEN exo.dotatual <> '' THEN
                    exo.dotatual::NUMERIC
                ELSE
                    0
                END AS vlrdotacaoatual,
                exo.empliquidado::NUMERIC + exo.empenhadoaliquidar::NUMERIC AS vlrempenhado,
                exo.empliquidado::NUMERIC AS vlrliquidado,
                exo.pago::NUMERIC AS vlrpago,
                exo.autorizado::NUMERIC AS vlrautorizado
            FROM wssof.ws_execucaoorcamentariadto exo
            WHERE
                anoexercicio = '".(int)$exercicio. "'";

        $this->executar($sql);
    }
    
    public function RetornaFuncionais($exercicio){
        $sql = "
            SELECT
                exe.programa,
                exe.acao,
                exe.planoorcamentario,
                exe.unidadeorcamentaria,
                exe.anoexercicio,
                exe.anoreferencia,
                exe.categoriaeconomica,
                exe.programa || '.' || exe.acao || '.' || exe.planoorcamentario || '.' || exe.unidadeorcamentaria AS funcional,
                sum(exe.dotatual::numeric) AS dotatual,
                sum(exe.dotinicialsiafi::numeric) AS dotinicialsiafi,
                sum(exe.dotacaoantecipada::numeric) AS dotacaoantecipada,
                sum(exe.dotacaoinicial::numeric) AS dotacaoinicial
            FROM wssof.ws_execucaoorcamentariadto AS exe
            WHERE
                exe.anoexercicio = '".(int)$exercicio. "'
                AND exe.anoreferencia = '".(int)$exercicio. "'
            GROUP BY
                exe.acao,
                exe.planoorcamentario,
                exe.unidadeorcamentaria,
                exe.programa,
                exe.anoexercicio,
                exe.anoreferencia,
                exe.categoriaeconomica
            ORDER BY
                funcional
        ";

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];        
    }
    
    public function RetornaDadosSiminc($exercicio){
        $sql = "
            SELECT
                psu.psuid,
                ptr.ptrid,
                ptr.unicod,
                psu.suoid,
                coalesce(psu.ptrdotacaocapital, 0) AS ptrdotacaocapital,
                coalesce(psu.ptrdotacaocusteio, 0) AS ptrdotacaocusteio,
                funcional AS funcionalptres,
                ptr.prgcod || '.' || ptr.acacod || '.' || ptr.plocod || '.' || ptr.unicod AS funcional,
                suo.suonome,
                suo.unosigla,
                suo.suosigla
            FROM spo.ptressubunidade AS psu
                JOIN monitora.vw_ptres AS ptr ON ptr.ptrid = psu.ptrid
                JOIN public.vw_subunidadeorcamentaria AS suo ON suo.suoid = psu.suoid
            WHERE
                ptr.ptrano = '". (int)$exercicio. "'
                AND suo.unofundo = 'f'
                AND ptr.irpcod != '6'
            ORDER BY
                ptr.unicod,
                funcional
        ";
        //ver($sql, d);
        $dados = $this->carregar($sql);
        return $dados ? $dados : [];        
    }
    
    public function RetornaDadosProvisionados($exercicio){
        $sql = "
            SELECT
                pli.plicod,
                pli.pliid,
                pli.suosigla,
                plititulo,
                pli.unosigla,
                pli.funcional,
                pli.previsto::numeric,
                pli.autorizado::numeric,
                pli.empenhado::numeric,
                pli.liquidado::numeric,
                pli.pago::numeric pago
            FROM monitora.vw_planointerno pli
                JOIN monitora.vw_ptres ptr ON pli.ptrid = ptr.ptrid
            WHERE
                pli.pliano = '$exercicio'
                AND pli.autorizado > pli.previsto
                AND ptr.irpcod <> '6'
            ORDER BY
                pli.unosigla,
                pli.suosigla,
                pli.funcional
        ";

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];        
    }
    
    public function RetornaDivergenciaPlanejado($exercicio){
        $sql = "
            SELECT
                ptrid,
                ptres,
                funcional,
                subunidade,
                empenhado,
                liquidado,
                pago
            FROM(
                SELECT
                    funcionais.ptrid,
                    funcionais.ptres,
                    funcionais.funcional,
                    funcionais.subunidade,
                    COALESCE(sec_geral.empenhado, 0.00) - COALESCE(funcionais.empenhado, 0.00) AS empenhado,
                    COALESCE(sec_geral.liquidado, 0.00) - COALESCE(funcionais.liquidado, 0.00) AS liquidado,
                    COALESCE(sec_geral.pago, 0.00) - COALESCE(funcionais.pago, 0.00) AS pago
                FROM(
                    SELECT
                        agrupado.ptrid,
                        agrupado.ptres,
                        agrupado.funcional,
                        CASE WHEN (
                            SELECT
                                COUNT(1)
                            FROM spo.ptressubunidade
                            WHERE
                                ptressubunidade.ptrid = agrupado.ptrid
                        ) > 1 THEN
                            'Várias'
                        ELSE
                           (
                            SELECT
                                suo.unosigla || ' - ' || suo.suosigla AS subunidade
                            FROM spo.ptressubunidade
                                JOIN public.vw_subunidadeorcamentaria suo ON ptressubunidade.suoid = suo.suoid
                            WHERE
                                ptressubunidade.ptrid = agrupado.ptrid
                                LIMIT 1
                            )
                        END AS subunidade,
                        SUM(agrupado.empenhado) AS empenhado,
                        SUM(agrupado.liquidado) AS liquidado,
                        SUM(agrupado.pago) AS pago
                    FROM (
                        SELECT
                            ptr.ptrid,
                            ptr.ptres,
                            ptr.funcional,
                            COALESCE(sec.empenhado, 0.00) AS empenhado,
                            COALESCE(sec.liquidado, 0.00) AS liquidado,
                            COALESCE(sec.pago, 0.00) as pago
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
                                    LEFT JOIN monitora.pi_planointerno ON(
                                        siopexecucao.plicod = pi_planointerno.plicod
                                        AND siopexecucao.exercicio = pi_planointerno.pliano
                                        AND pi_planointerno.plistatus = 'A')
                                WHERE
                                    pi_planointerno.ungcod IS NOT NULL
                                    AND siopexecucao.exercicio = '$exercicio'
                                GROUP BY
                                    siopexecucao.ptres,
                                    siopexecucao.unicod,
                                    pi_planointerno.ungcod
                            ) sec ON(ptr.ptres = sec.ptres AND suo.unocod = sec.unicod AND suo.suocod = sec.ungcod)
                        WHERE
                            suo.prsano = '$exercicio'
                            AND suo.unofundo = FALSE
                            AND suo.suostatus = 'A'
                            AND ptr.irpcod <> '6'
                        GROUP BY
                            ptr.ptrid,
                            ptr.ptres,
                            ptr.funcional,
                            sec.empenhado,
                            sec.liquidado,
                            sec.pago
                    ) agrupado
                    GROUP BY
                        agrupado.ptrid,
                        agrupado.ptres,
                        agrupado.funcional
                ) AS funcionais
                    LEFT JOIN(
                        SELECT
                            siopexecucao.ptres,
                            SUM(COALESCE(siopexecucao.vlrautorizado, 0.00))::NUMERIC AS provisionado,
                            SUM(COALESCE(siopexecucao.vlrempenhado, 0.00))::NUMERIC AS empenhado,
                            SUM(COALESCE(siopexecucao.vlrliquidado, 0.00))::NUMERIC AS liquidado,
                            SUM(COALESCE(siopexecucao.vlrpago, 0.00))::NUMERIC AS pago
                        FROM spo.siopexecucao
                        WHERE
                            siopexecucao.exercicio = '$exercicio'
                        GROUP BY
                            siopexecucao.ptres
                    ) sec_geral ON(funcionais.ptres = sec_geral.ptres)
                WHERE
                    sec_geral.empenhado - funcionais.empenhado > 0
                    OR sec_geral.liquidado - funcionais.liquidado > 0
                    OR sec_geral.pago - funcionais.pago > 0

                UNION

                SELECT DISTINCT
                    ptr_divergente.ptrid,
                    ptr_divergente.ptres,
                    ptr_divergente.funcional,
                    CASE WHEN (
                        SELECT
                            COUNT(1)
                        FROM spo.ptressubunidade
                        WHERE
                            ptressubunidade.ptrid = ptr_divergente.ptrid
                    ) > 1 THEN
                            'Várias'
                    ELSE
                       (
                        SELECT
                            suo.unosigla || ' - ' || suo.suosigla AS subunidade
                        FROM spo.ptressubunidade
                            JOIN public.vw_subunidadeorcamentaria suo ON ptressubunidade.suoid = suo.suoid
                        WHERE
                            ptressubunidade.ptrid = ptr_divergente.ptrid
                            LIMIT 1
                        )
                    END AS subunidade,
                    sec_total.empenhado - (
                        SELECT
                            SUM(COALESCE(siopexecucao.vlrempenhado, 0.00))::NUMERIC AS empenhado
                        FROM spo.siopexecucao
                            JOIN monitora.vw_planointerno pli ON(
                                siopexecucao.plicod = pli.plicod
                                AND siopexecucao.exercicio = pli.pliano)
                        WHERE
                            siopexecucao.exercicio = '$exercicio'
                            AND siopexecucao.ptres = ptr_divergente.ptres
                            AND siopexecucao.plicod = pi_correto.plicod -- varios
                            AND pli.unocod = pi_correto.unocod
                            AND pli.suocod = pi_correto.suocod
                    ) AS empenhado,
                    sec_total.liquidado - (
                        SELECT
                            SUM(COALESCE(siopexecucao.vlrliquidado, 0.00))::NUMERIC AS empenhado
                        FROM spo.siopexecucao
                            JOIN monitora.vw_planointerno pli ON(
                                siopexecucao.plicod = pli.plicod
                                AND siopexecucao.exercicio = pli.pliano)
                        WHERE
                            siopexecucao.exercicio = '$exercicio'
                            AND siopexecucao.ptres = ptr_divergente.ptres
                            AND siopexecucao.plicod = pi_correto.plicod -- varios
                            AND pli.unocod = pi_correto.unocod
                            AND pli.suocod = pi_correto.suocod
                    ) AS liquidado,
                    sec_total.pago - (
                        SELECT
                            SUM(COALESCE(siopexecucao.vlrpago, 0.00))::NUMERIC AS empenhado
                        FROM spo.siopexecucao
                            JOIN monitora.vw_planointerno pli ON(
                                siopexecucao.plicod = pli.plicod
                                AND siopexecucao.exercicio = pli.pliano)
                        WHERE
                            siopexecucao.exercicio = '$exercicio'
                            AND siopexecucao.ptres = ptr_divergente.ptres
                            AND siopexecucao.plicod = pi_correto.plicod -- varios
                            AND pli.unocod = pi_correto.unocod
                            AND pli.suocod = pi_correto.suocod
                    ) AS pago
                FROM spo.siopexecucao pi_siop -- SELECT * FROM spo.siopexecucao
                    JOIN (
                        SELECT
                            pli.plicod,
                            pli.ptres
                        FROM monitora.vw_planointerno pli
                            JOIN public.vw_subunidadeorcamentaria suo ON(pli.suoid = suo.suoid)
                            JOIN monitora.vw_ptres ptr ON(pli.ptrid = ptr.ptrid)
                        WHERE
                            pli.pliano = '$exercicio'
                            AND suo.unofundo IS FALSE
                            AND ptr.irpcod != '6'
                            AND(pli.plicod IS NOT NULL AND pli.plicod != '')
                    ) pi_siminc2 ON(pi_siop.plicod = pi_siminc2.plicod)
                    JOIN monitora.vw_ptres ptr_divergente ON(pi_siop.ptres = ptr_divergente.ptres AND ptr_divergente.ptrano = '$exercicio') -- SELECT * FROM monitora.vw_ptres
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
                            siopexecucao.exercicio = '$exercicio' -- Inserindo o ano direto na subquery por motivo de performance da consulta.
                        GROUP BY
                            siopexecucao.ptres,
                            siopexecucao.unicod
                    ) sec_total ON(ptr_divergente.ptres = sec_total.ptres)
                    LEFT JOIN (
                        SELECT
                            suo.unocod,
                            suo.suocod,
                            pli.plicod,
                            pli.ptres
                        FROM monitora.vw_planointerno pli
                            JOIN public.vw_subunidadeorcamentaria suo ON(pli.suoid = suo.suoid)
                            JOIN monitora.vw_ptres ptr ON(pli.ptrid = ptr.ptrid)
                        WHERE
                            pli.pliano = '$exercicio'
                            AND suo.unofundo IS FALSE
                            AND ptr.irpcod != '6'
                            AND(pli.plicod IS NOT NULL AND pli.plicod != '')
                    ) pi_correto ON(pi_siop.plicod = pi_siminc2.plicod AND pi_siop.ptres = pi_correto.ptres)
                WHERE
                    pi_siop.exercicio = '$exercicio'
                    AND (
                        pi_siop.vlrempenhado > 0
                        OR pi_siop.vlrliquidado > 0
                        OR pi_siop.vlrpago > 0
                    )
                    -- Filtrar os PIs do SIOP que tem código de pi e ptres diferente do código do pi e ptres do SIMINC2
                    AND pi_siop.ptres != pi_siminc2.ptres
            ) consulta
            ORDER BY
                funcional
        ";
        //ver($sql, d);
        $listaResultado = $this->carregar($sql);
        return $listaResultado? $listaResultado: [];        
    }
    
    public function RetornaEmailsSuperUsuarios(){
        $sql = "
            SELECT DISTINCT
                usu.usucpf,
                usu.usunome,
                usu.usuemail
            FROM seguranca.perfilusuario pu
                JOIN seguranca.usuario usu ON(
                    usu.usucpf = pu.usucpf
                    AND usu.suscod = 'A'
                )
                JOIN seguranca.usuario_sistema us ON(
                    us.usucpf = usu.usucpf
                    AND us.suscod = 'A'
                    AND us.sisid = ". (int)SISID_PLANEJAMENTO. "
                )
            WHERE
                pu.pflcod IN (". (int)PFL_ADMINISTRADOR. ", ". (int)PFL_SUPERUSUARIO. ")
            ORDER BY
                usuemail
        ";
//echo $sql;
        return $this->carregar($sql);        
    }

}//end Class
?>