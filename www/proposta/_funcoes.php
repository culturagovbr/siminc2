<?php

/**
 * Monta consulta do relatório geral de Propostas
 * 
 * @param stdClass $filtros
 * @return string
 */
function montarSqlRelatorioGeralProposta(stdClass $filtros){
    $sql = "
        SELECT
            pro.proid,
            suo.unosigla || ' - ' || suo.suonome subunidade,
            eqd.eqddsc,
	    ptr.irpcod,
            ptr.funcional,
            ptr.acatitulo,
            ptr.plodsc,
            aca.locquantidadeproposta,
            pro.proquantidade,
            pro.proquantidadeexpansao,
            pro.projustificativa,
	    pro.projustificativaexpansao,
	    ndp.ndpcod,
	    idu.iducod,
	    fr.foncod,
	    prd.idoid,
	    prd.prdvalor,
	    prd.prdvalorexpansao
        FROM proposta.proposta pro
            JOIN monitora.vw_ptres ptr ON pro.ptrid = ptr.ptrid
	    JOIN monitora.acao aca ON ptr.acaid = aca.acaid
            JOIN public.vw_subunidadeorcamentaria suo ON suo.suoid = pro.suoid
            JOIN monitora.pi_enquadramentodespesa eqd ON eqd.eqdid = pro.eqdid
            LEFT JOIN proposta.propostadetalhe prd ON(
		prd.proid = pro.proid
		AND prd.prdstatus = 'A'
	    )
            LEFT JOIN public.naturezadespesa ndp ON(prd.ndpid = ndp.ndpid)
            left join public.fonterecurso fr on prd.fonid = fr.fonid
            left join public.identifuso idu on prd.iduid = idu.iduid
        WHERE
            pro.prsano = '". (int)$filtros->exercicio. "'
            AND prostatus = 'A'
        ORDER BY
            pro.proid,
            subunidade,
            eqd.eqddsc,
	    ptr.irpcod,
            ptr.funcional,
            ptr.acatitulo,
            ptr.plodsc
    ";
    return $sql;
}

/**
 * Monta consulta do relatório geral de Pre-PIs
 * 
 * @param stdClass $filtros
 * @return string
 */
function montarSqlRelatorioGeralPrePi(stdClass $filtros){
    $sql = "
        SELECT
            pli.pliid,
            pli.plititulo,
            pli.plidsc,
	    suo.unosigla || ' - ' || suo.suonome subunidade,
	    eqd.eqddsc,
            ptr.irpcod,
	    mai.mainome,
	    mas.masnome,
            ptr.funcional,
            ptr.acatitulo,
            ptr.plodsc,
            esd.esddsc,
            ppr.pprnome,
            pum.pumdescricao,
            pli.pliquantidade,
            opp.oppcod,
            opp.oppdsc,
            mpp.mppcod,
            mpp.mppnome,
            ipp.ippcod,
            ipp.ippnome,
            mpn.mpncod,
            mpn.mpnnome,
            ipn.ipncod,
            ipn.ipndsc,
            -- Area cultural
            mde.mdedsc,
            -- Segmento Cultural
            nee.needsc,
            -- Localização
            esf.esfdsc,
            -- Pais
            pai.paidescricao,
            -- Estado
            est.estuf,
            est.estdescricao,
            -- Municipio
            mun.estuf AS munestuf,
            mun.mundescricao,
            pli.plivalorcusteio,
            pli.plivalorcapital,
            pli.pliquantidadeadicional,
            pli.plivalorcusteioadicional,
            pli.plivalorcapitaladicional,
            pli.plijustificativaadicional
        FROM proposta.preplanointerno pli
            JOIN monitora.vw_ptres ptr ON pli.ptrid = ptr.ptrid
            JOIN public.vw_subunidadeorcamentaria suo ON suo.suoid = pli.suoid
            JOIN monitora.pi_enquadramentodespesa eqd ON eqd.eqdid = pli.eqdid
            LEFT JOIN workflow.documento doc ON doc.docid = pli.docid
            LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
            LEFT JOIN planacomorc.manutencaoitem mai ON pli.maiid = mai.maiid
            LEFT JOIN planacomorc.manutencaosubitem mas ON pli.masid = mas.masid
            LEFT JOIN monitora.pi_produto ppr ON pli.pprid = ppr.pprid
            LEFT JOIN monitora.pi_unidade_medida pum ON pli.pumid = pum.pumid
            LEFT JOIN public.objetivoppa opp ON pli.oppid = opp.oppid
            LEFT JOIN public.metappa mpp ON pli.mppid = mpp.mppid
            LEFT JOIN public.iniciativappa ipp ON pli.ippid = ipp.ippid
            LEFT JOIN public.metapnc mpn ON pli.mpnid = mpn.mpnid
            LEFT JOIN public.indicadorpnc ipn ON pli.ipnid = ipn.ipnid
            LEFT JOIN monitora.pi_modalidadeensino mde ON pli.mdeid = mde.mdeid
            LEFT JOIN monitora.pi_niveletapaensino nee ON pli.neeid = nee.neeid
            LEFT JOIN territorios.esfera esf ON pli.esfid = esf.esfid
            LEFT JOIN proposta.preplanointernolocalizacao plo ON pli.pliid = plo.pliid
            LEFT JOIN territorios.pais pai ON plo.paiid = pai.paiid
            LEFT JOIN territorios.estado est ON plo.estuf = est.estuf
            LEFT JOIN territorios.municipio mun ON plo.muncod = mun.muncod
        WHERE
            pli.prsano = '". (int)$filtros->exercicio. "'
            AND plistatus = 'A'
    ";
    return $sql;
}

/**
 * Retorna o nome da classe css que exibe as cores verde, azul ou
 * vermelho de acordo com a situação de utilização do recurso financeiro disponível.
 * 
 * @param int $valorDisponivel Calculo do Valor de limite menos o valor utilizado.
 * @return string Nome da classe css que exibe a cor.
 */
function controlarCorPorValorDisponivel($valorDisponivel)
{
    $resultado = 'green';
    if($valorDisponivel > 0){
        $resultado = 'blue';
    }

    if($valorDisponivel < 0){
        $resultado = 'red';
    }

    return $resultado;
}

