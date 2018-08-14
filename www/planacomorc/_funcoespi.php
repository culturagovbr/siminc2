<?php

/**
 * Funções de apoio ao gerenciamento de PIs.
 * $Id: _funcoespi.php 99771 2015-07-06 20:42:51Z werteralmeida $
 */
function listaPisManterAt($dados) {
    global $db;
    $obrigatorias = UNIDADES_OBRIGATORIAS;
    /* Filtros */
    $where = '';
    $where .= $dados['pi'] ? " AND pli.plicod ilike '%" . $dados['pi'] . "%' " : "";
    $where .= $dados['unidade'] ? " AND (pli.unicod = '{$dados['unidade']}' OR pli.ungcod IN (select ungcod from public.unidadegestora where unicod = '{$dados['unidade']}') )" : "";
    $where .= $dados['plisituacao'] && $dados['plisituacao'] != 'undefined' ? " AND pli.plisituacao = '" . $dados['plisituacao'] . "' " : "";
    $where .= $dados['apenas_obrigatorias'] ? " AND pli.unicod IN (" . UNIDADES_OBRIGATORIAS . ")" : "";
    //$where .= $dados["ptres"] != 'null' && $dados["ptres"] != '' ? " AND ptres.ptrid in ('" . str_replace(",", "','", $dados['ptres']) . "') " : "";
    if (is_array($_REQUEST["ptres"])) {
        $where .= $_REQUEST["ptres"] != 'null' && !$_REQUEST["ptres"] ? " AND ptres.ptrid in ('" . implode("','", $_REQUEST["ptres"]) . "') " : "";
    } else {
        $where .= $_REQUEST["ptres"] != 'null' && $_REQUEST["ptres"] != '' ? " AND ptres.ptrid in ('" . str_replace(",", "','", $dados['ptres']) . "') " : "";
    }
    $titulodescricaoTmp = removeAcentos(str_replace("-", "", $_REQUEST['titulodescricao']));
    $where .= $dados['titulodescricao'] ? " AND (UPPER(public.removeacento(pli.plititulo)) ilike '%" . $titulodescricaoTmp . "%' OR UPPER(public.removeacento(pli.plidsc)) ilike '%" . $titulodescricaoTmp . "%')" : "";


    if ($dados['apenasEmpenhados'] == 'true') {
        $strColumns = "
            pli.plicod AS codigo,
            uni.unicod || ' - ' || uni.unidsc AS unidsc,
            COALESCE(pemp.total,0) AS empenhado_total
        ";
    } else {
        $strColumns = "
            pli.pliid  AS acoes,
            pli.plicod AS codigo,
            CASE
                WHEN trim(pli.plititulo) IS NOT NULL
                THEN pli.plititulo
                ELSE 'Não Preenchido '
            END                               AS titulo,
            uni.unicod || ' - ' || uni.unidsc AS unidsc,
            CASE
                WHEN pli.plisituacao = 'P'
                THEN ' Pendente '
                WHEN pli.plisituacao = 'A'
                THEN ' Aprovado '
                WHEN pli.plisituacao = 'R'
                THEN ' Revisado '
                WHEN pli.plisituacao = 'C'
                THEN ' Cadastrado no SIAFI '
                WHEN pli.plisituacao = 'E'
                THEN ' Enviado para Revisão '
                WHEN pli.plisituacao = 'S'
                THEN ' Confimado no SIAFI '
                WHEN pli.plisituacao = 'T'
                THEN ' Cadastrado no SIAFI '
                WHEN pli.plisituacao = 'H'
                THEN ' Homologação '
            END                    AS situacao,
            COALESCE(pemp.total,0) AS empenhado_total
        ";
    }


    $sql = "SELECT
        DISTINCT
        {$strColumns}
FROM
    monitora.pi_planointerno pli
LEFT JOIN
    monitora.pi_planointernoptres pip
ON
    pip.pliid = pli.pliid
LEFT JOIN
    siafi.pliempenho pemp
ON
    pli.plicod = pemp.plicod
AND pli.pliano = pemp.exercicio
LEFT JOIN
    monitora.ptres
ON
    pip.ptrid = ptres.ptrid
LEFT JOIN
    public.unidade uni
ON
    uni.unicod = pli.unicod
LEFT JOIN
    public.unidadegestora ung
ON
    ung.ungcod = pli.ungcod
WHERE
    pli.plistatus = 'A'
AND pli.pliano = '{$_SESSION['exercicio']}'
$where
    ORDER BY
    2";

    return $sql;
}

/**
 * Lista de PIs para MANTER
 */
function listaPisManter($dados) {
    global $db;

    $obrigatorias = UNIDADES_OBRIGATORIAS;

    /* Filtros */
    $where = '';
    $where .= $dados['pi'] ? " AND pli.plicod ilike '%" . $dados['pi'] . "%' " : "";
    $where .= $dados['unidade'] ? " AND (pli.unicod = '{$dados['unidade']}' OR pli.ungcod IN (select ungcod from public.unidadegestora where unicod = '{$dados['unidade']}') )" : "";
    $where .= $dados['plisituacao'] && $dados['plisituacao'] != 'undefined' ? " AND pli.plisituacao = '" . $dados['plisituacao'] . "' " : "";
    $where .= $dados['apenas_obrigatorias'] ? " AND (pli.unicod IN ($obrigatorias) OR pli.ungcod IN (select ungcod from public.unidadegestora where unicod IN ($obrigatorias)) )" : "";
    $where .= $dados["ptres"] != 'null' && $dados["ptres"] != '' ? " AND ptres.ptrid in ('" . str_replace(",", "','", $dados['ptres']) . "') " : "";
    $titulodescricaoTmp = removeAcentos(str_replace("-", "", $_REQUEST['titulodescricao']));
    $where .= $dados['titulodescricao'] ? " AND (UPPER(public.removeacento(pli.plititulo)) ilike '%" . $titulodescricaoTmp . "%' OR UPPER(public.removeacento(pli.plidsc)) ilike '%" . $titulodescricaoTmp . "%')" : "";


    /* Caso seja selecionado apenas 1 PTRES, mostrar as colunas dos dados apenas para aquele PTRES */
    $sqlAdicional = "";
    if ($dados["ptres"] != 'null' && $dados["ptres"]) {
        $arPtres = explode(",", $dados['ptres']);
    }

    if ($_REQUEST["ptres"] && count($arPtres) == 1 && $dados['ptres'][0] != "" && $_REQUEST["ptres"] != 'null') {
        $dados = $db->carregar("SELECT ptres FROM monitora.ptres WHERE ptrid = {$_REQUEST["ptres"][0]}");
        $ptres = $dados[0]['ptres'];
        #ver($_REQUEST["ptres"][0],d);
        $sqlAdicional.=", COALESCE((SELECT
                                        pip.pipvalor
                                    FROM
                                        monitora.pi_planointernoptres pip
                                    LEFT JOIN
                                        monitora.ptres
                                    ON
                                        pip.ptrid = ptres.ptrid
                                    WHERE
                                        ptres.ptrano = '{$_SESSION['exercicio']}'
                                    AND pip.pliid= gmb.pliid
                                    AND ptres.ptrid={$_REQUEST["ptres"][0]}),0.00) as dotacao_pip_ptres
                        ,        COALESCE((SELECT
                                    total
                                FROM
                                    siafi.pliptrempenho ppe
                                WHERE
                                    plicod = gmb.codigo
                                    AND ppe.exercicio = '{$_SESSION['exercicio']}'
                                AND ptres = '{$ptres}'),0.00) as empenhado_pi_ptres";
    }
    /* Cabeçalho da Consulta */

    $acoes = " '-' ";
    /* Remove as ações para o perfil Gabinete */
    $acoes = <<<SQL
        gmb.pliid
SQL;

    $params['SELECT'] = <<<SQL
SELECT
    {$acoes}
    AS acoes,

        gmb.codigo as codigo,
        CASE WHEN trim(gmb.titulo) is not null THEN
            gmb.titulo  || '<input type=\"hidden\" id=\"plititulo[' || gmb.pliid || ']\" value=\"' || gmb.codigo || ' - ' || gmb.titulo || '\">'
        ELSE
            'Não Preenchido <input type=\"hidden\" id=\"plititulo[' || gmb.pliid || ']\" value=\" ' || gmb.codigo ||' - Não Preenchido\"/>'
        END as titulo,
        uni.unicod || ' - ' || uni.unidsc as unidsc,
        '<center>' || CASE
        WHEN pli.plisituacao = 'P' THEN ' Pendente '
        WHEN pli.plisituacao = 'A' THEN ' Aprovado '
        WHEN pli.plisituacao = 'R' THEN ' Revisado '
        WHEN pli.plisituacao = 'C' THEN ' Cadastrado no SIAFI '
        WHEN pli.plisituacao = 'E' THEN ' Enviado para Revisão '
        WHEN pli.plisituacao = 'S' THEN ' Confimado no SIAFI '
        WHEN pli.plisituacao = 'T' THEN ' <span style="color:red">Cadastrado no SIAFI</span> '
        WHEN pli.plisituacao = 'H' THEN ' Homologação '
        END || '</center>' AS situacao,
        empenhado AS empenhado_total
        {$sqlAdicional}
SQL;
    $params['where'] = " $where ";
    $sql = retornaConsultaPI($params);

    /* Busca alternativa para recuperar os PIs que possuem Empenho mas não estão cadastrados no SIMEC */
    if ($dados['apenasEmpenhados'] == 'true') {
        $sql = <<<SQL
               SELECT
                plicod,
                'Não informado.' AS unidade,
                total            AS empenhado
            FROM
                siafi.pliempenho pip
            WHERE
                pip.exercicio = '{$_SESSION['exercicio']}'
            AND pip.plicod NOT IN
                (
                    SELECT
                        plicod
                    FROM
                        monitora.pi_planointerno pli
                    WHERE
                        pli.pliano = '{$_SESSION['exercicio']}' )
SQL;
    }

    $cabecalho = array("Ação",
        "Código",
        "Unidade",
        "Empenhado Total do PI (R$)"
    );
    /* Caso seja selecionado apenas 1 PTRES, mostrar as colunas dos dados apenas para aquele PTRES */
    if ($_REQUEST["ptres"][0] && count($_REQUEST["ptres"]) == 1) {
        array_push($cabecalho, "Dotação para este PTRES (R$)", "Empenhado neste PTRES (R$)", "Não Empenhado neste PTRES");
    }

    return $sql;
}

/**
 * Monta a combo de UGs filtrando por UO
 */
function carregarComboUG($unicod, $editavel = 'S', $fnc = 'FALSE') {
    global $db;

    if (in_array(PFL_SUBUNIDADE, pegaPerfilGeral($_SESSION['usucpf']))) {
        $filtroPerfilUG = <<<DML
            AND EXISTS (
                SELECT 1
                FROM planacomorc.usuarioresponsabilidade urp
                WHERE
                    urp.ungcod = suo.suocod
                    AND urp.pflcod = '%s'
                    AND urp.usucpf = '%s'
                    AND urp.rpustatus = 'A')
DML;
        $filtroPerfilUG = sprintf($filtroPerfilUG, PFL_SUBUNIDADE, $_SESSION['usucpf']);
    }

    $sql = <<<DML
            SELECT DISTINCT
                suo.suocod AS codigo,
                suo.suocod || ' - ' || suonome AS descricao
            FROM public.vw_subunidadeorcamentaria suo
            WHERE
                suo.suostatus = 'A'
                AND suo.prsano = '{$_SESSION['exercicio']}'
                AND suo.unocod = '%s' 
                AND suo.unofundo = {$fnc}
                {$filtroPerfilUG}
            ORDER BY
                descricao
DML;

    $stmt = sprintf($sql, $unicod);
//ver($stmt, d);
    $dados = $db->carregar($stmt);
    if (count($dados) && $dados[0]) {
        $infoCombo = 'Selecione';
    } else {
        $dados = array();
        $infoCombo = 'Selecione uma unidade';
    }

    $db->monta_combo('ungcod', $dados, $editavel, $infoCombo, '', null, null, 240, 'N', 'ungcod', null, (isset($ungcod) ? $ungcod : null), null, 'class="form-control chosen-select" style="width=100%;""', null, null);
}

/**
 * Monta a combo de metas PPA
 */
function carregarMetasPPA($oppid, $mppid, $suocod = null) {
    global $db;

    $join = '';
    if($suocod){
        $ret="";
        $suocod = explode(',', $suocod);
        for($i=0;$i<count($suocod);$i++){
            $ret .= "'".$suocod[$i]."',";
        }
        $suocod = substr($ret, 0, strlen($ret)-1);

        $join = "inner join (
                    select smp.mppid 
                    from spo.subunidademetappa smp
                        inner join public.vw_subunidadeorcamentaria suo on suo.suoid = smp.suoid and suo.prsano = '{$_SESSION['exercicio']}'
                    where suo.suocod in ($suocod)
                    union all
                    select mpp.mppid from public.metappa mpp
                            left join spo.subunidademetappa smp on smp.mppid = mpp.mppid
                    where mpp.prsano = '{$_SESSION['exercicio']}'       
                    and smp.mppid is null               
                ) smp on smp.mppid = om.mppid";
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
            AND m.prsano = '{$_SESSION['exercicio']}'
            AND om.oppid = ". (int)$oppid. "
        ORDER BY
            descricao
    ";
//ver($sql, d);
    $db->monta_combo('mppid', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'mppid', null, (isset($mppid)? $mppid: null), null, 'class="form-control chosen-select" style="width=100%;"');
}

/**
 * Monta a combo de iniciativas PPA
 */
function carregarIniciativaPPA($oppid, $ippid, $listaSubunidades=null) {
    global $db;
    $filtroSubUnidades = $listaSubunidades!=null?" and su.suocod =  '$listaSubunidades'":'';
    $sql = "select i.ippid as codigo,
                   i.ippcod || ' - ' || i.ippnome AS descricao,
                   ippcod
              from public.iniciativappa i
              left join spo.subunidadeiniciativappa si
                on i.ippid = si.ippid
              left join public.vw_subunidadeorcamentaria su
                on si.suoid = su.suoid
             where i.prsano = '{$_SESSION['exercicio']}'
               $filtroSubUnidades
               and i.oppid = ". (int)$oppid. " 
               and ippstatus = 'A'
             union
            select i.ippid as codigo,
                   i.ippcod || ' - ' || i.ippnome AS descricao,
                   ippcod
              from public.iniciativappa i
              left join spo.subunidadeiniciativappa si
                on i.ippid = si.ippid
             where si.ippid is null 
               and i.oppid = ". (int)$oppid. " 
               and ippstatus = 'A'
             ORDER BY ippcod";
//ver($sql,d);
    $db->monta_combo('ippid', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'ippid', null, (isset($ippid)? $ippid: null), null, 'class="form-control chosen-select" style="width=100%;"');
}

/**
 * Monta a combo de Metas PNC
 */
function carregarMetaPNC($suocod, $mpnid) {
    $mMetaPnc = new Public_Model_MetaPnc();
    $mMetaPnc->monta_combo($mpnid, null, $mMetaPnc->recuperarSqlCombo(['suocod'=>$suocod]));
}

/**
 * Monta a combo de Indicadores PNC
 */
function carregarIndicadorPNC($mpnid, $ipnid) {
    global $db;

    $sql = "
        SELECT
            ipnid AS codigo,
            ipncod || ' - ' || ipndsc AS descricao
        FROM public.indicadorpnc
        WHERE
            ipnstatus = 'A'
            AND prsano = '{$_SESSION['exercicio']}'
            AND mpnid = ". (int)$mpnid. "
        ORDER BY
            ipncod
    ";

    $db->monta_combo('ipnid', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'ipnid', null, (isset($ipnid)? $ipnid: null), null, 'class="form-control chosen-select" style="width=100%;"');
}

/**
 * Monta a combo de Indicadores PNC
 */
function carregarAderenciaPtFnc($ptaid) {
    global $db;

    $sql = "
        SELECT
            n4.ptaid AS codigo,
            n1.ptaitem || '.' || n2.ptaitem || '.' || n3.ptaitem || '.' || n4.ptaitem || ' ' || n4.ptadsc AS descricao
        FROM spo.planotrabalhoanual AS n1
            JOIN spo.planotrabalhoanual AS n2 ON n1.ptaid = n2.ptapai
            JOIN spo.planotrabalhoanual AS n3 ON n2.ptaid = n3.ptapai
            JOIN spo.planotrabalhoanual AS n4 ON n3.ptaid = n4.ptapai
        WHERE
            n1.ptapai IS NULL
            AND n1.ptaitem = '". PTAID_LINHAS_PROGRAMATICAS. "'
            AND n4.prsano = '{$_SESSION['exercicio']}'
        ORDER BY
            descricao
    ";

    $db->monta_combo('ptaid', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'ptaid', null, (isset($ptaid)? $ptaid: null), null, 'class="form-control chosen-select" style="width=100%;"');
}

/**
 * Monta a combo de Segmento Cultural.
 */
function carregarSegmentoCultural($mdeid, $neeid) {
    global $db;

    $sql = "
        SELECT
            neeid AS codigo,
            needsc AS descricao
        FROM monitora.pi_niveletapaensino
        WHERE
            neeano = '{$_SESSION['exercicio']}'
            AND neestatus = 'A'
            AND mdeid = ". (int)$mdeid. "
        ORDER BY
            descricao
    ";
    $db->monta_combo('neeid', $sql, 'S', 'Selecione', '', null, null, null, 'N', 'neeid', null, (isset($neeid) ? $neeid : null), null, 'class="form-control chosen-select" style="width=100%;"');
}

/**
 * Monta a combo de Manutenção Item
 */
function carregarManutencaoItem($eqdid, $maiid = null) {
    global $db;

    $sql = "SELECT maiid AS codigo, mainome AS descricao
            FROM planacomorc.manutencaoitem
            WHERE prsano = '{$_SESSION['exercicio']}'
            AND maistatus = 'A'
            AND eqdid = ". (int)$eqdid. "
            ORDER BY descricao";
    $dados = $db->carregar($sql);

    if($dados){
        $db->monta_combo('maiid', $dados, 'S', 'Selecione', '', null, null, null, 'N', 'maiid', null, (isset($maiid) ? $maiid : null), null, 'class="form-control chosen-select" style="width=100%;"');
    } else {
        $db->monta_combo('maiid', array(), 'S', 'Selecione', '', null, null, null, 'N', 'maiid', null, (isset($maiid) ? $maiid : null), null, 'class="form-control chosen-select" style="width=100%;"');
    }
}

/**
 * Monta a combo de Manutenção Item
 */
function carregarManutencaoSubItem($maiid, $masid = null) {
    global $db;

    $sql = "
        SELECT
            masid AS codigo,
            masnome AS descricao
        FROM planacomorc.manutencaosubitem
        WHERE
            prsano = '{$_SESSION['exercicio']}'
            AND masstatus = 'A'
            AND maiid = ". (int)$maiid. "
        ORDER BY
            descricao";
//ver($sql,d);
    $db->monta_combo('masid', $sql, 'S', 'Selecione', '', null, null, null, 'N', 'masid', null, (isset($masid) ? $masid : null), null, 'class="form-control chosen-select" style="width=100%;"');
}

/**
 * Retorna os dados de limite autorizado para a Sub-Unidade.
 * 
 * @param stdClass $parametros
 * @return float Limite autorizado da Sub-Unidade.
 */
function carregarLimiteAutorizadoSubUnidade(stdClass $parametros) {
    global $db;

    $sql = "
        SELECT
            lmuvlr
        FROM planacomorc.unidadegestora_limite ul
        WHERE
            ul.lmustatus = 'A'
            AND ul.prsano = '{$_SESSION['exercicio']}'
            AND ul.ungcod = '". $parametros->ungcod. "'";
//ver($sql, d);
    $autorizado = $db->pegaUm($sql);
    return $autorizado;
}

/**
 * Retorna os dados de limite autorizado para a Sub-Unidade.
 * 
 * @param stdClass $parametros
 * @return float Limite autorizado da Sub-Unidade.
 */
function carregarLimiteAutorizadoUnidadeFnc(stdClass $parametros) {
    global $db;

    $sql = "
        SELECT
            lmuvlr
        FROM planacomorc.unidadegestora_limite ul
        WHERE
            ul.lmustatus = 'A'
            AND ul.prsano = '{$parametros->exercicio}'
            AND ul.unoid IS NOT NULL
    ";

    $autorizado = $db->pegaUm($sql);
    return $autorizado;
}

/**
 * Monta o SQL da consulta de funcional para as subunidades.
 * 
 * @param stdClass $filtros "$filtros->exercicio" O Exercício é obrigatório.
 * @return string
 */
function montarSqlBuscarFuncional(stdClass $filtros) {
    $where = NULL;
    
    # Filtros
    $where .= $filtros->prgcod? "\n AND UPPER(ptr.prgcod) LIKE('%". strtoupper($filtros->prgcod). "%')": NULL;
    $where .= $filtros->acacod? "\n AND UPPER(aca.acacod) LIKE('%". strtoupper($filtros->acacod). "%')": NULL;
    $where .= $filtros->unicod? "\n AND aca.unicod = '{$filtros->unicod}'": NULL;
    $where .= $filtros->ungcod? "\n AND uni.suocod = '{$filtros->ungcod}'": NULL;
    $where .= $filtros->buscalivre? "\n AND (TRIM(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) ILIKE('%" . $filtros->buscalivre . "%') OR dtl.ptres ILIKE '%" . $filtros->buscalivre . "%')": NULL;
    $where .= $filtros->ptrid? "\n AND ptr.ptrid = {$filtros->ptrid}": NULL;
    if($filtros->no_ptrid){
        if(is_array($filtros->no_ptrid)){
            $where .= "\n AND ptr.ptrid NOT IN('". join($filtros->no_ptrid, "','"). "')";
        } else {
            $where .= "\n AND ptr.ptrid != ". (int)$filtros->no_ptrid;
        }
    }
    if($filtros->eqdid){
        $where .= " \n AND ptr.irpcod IN(
                SELECT
                    eqrp.irpcod
                FROM monitora.enquadramentorp eqrp
                WHERE
                    eqrp.eqdid = ". (int)$filtros->eqdid. "
            )
        ";
    }
    
    $sql = "
        SELECT
            ptr.ptrid,
            ptr.ptres || '<autorizadocusteio>' || COALESCE(psu.ptrdotacaocusteio, 0.00) || '</autorizadocusteio><autorizadocapital>' || COALESCE(psu.ptrdotacaocapital, 0.00) || '</autorizadocapital>' AS ptres,
            TRIM(aca.prgcod) || '.' || TRIM(aca.acacod) || '.' || TRIM(aca.loccod) || '.' || (CASE WHEN LENGTH(TRIM(aca.acaobjetivocod)) <= 0 THEN '-' ELSE COALESCE(TRIM(aca.acaobjetivocod), '') END) || '.' || COALESCE(TRIM(ptr.plocod)) || ' - ' || aca.acatitulo || CASE WHEN LENGTH(TRIM(ptr.plodsc)) >= 0 THEN ': ' || ptr.plodsc ELSE '' END || ' (RP ' || COALESCE(ptr.irpcod, '') || ')' AS descricao,
            COALESCE(psu.ptrdotacaocusteio, 0.00) + COALESCE(psu.ptrdotacaocapital, 0.00) AS dotacaoatual,
            COALESCE(SUM(det.pipvalor), 0.00) AS det_pi,
            COALESCE(psu.ptrdotacaocusteio, 0.00) - COALESCE(SUM(det.custeio), 0.00) AS nao_det_pi_custeio,
            COALESCE(psu.ptrdotacaocapital, 0.00) - COALESCE(SUM(det.capital), 0.00) AS nao_det_pi_capital,
            COALESCE((pemp.total), 0.00) AS empenhado,
            (COALESCE(psu.ptrdotacaocusteio, 0.00) + COALESCE(psu.ptrdotacaocapital, 0.00)) - COALESCE(pemp.total, 0.00) AS nao_empenhado
        FROM monitora.ptres ptr
            JOIN monitora.acao aca ON(ptr.acaid = aca.acaid)
            JOIN public.vw_subunidadeorcamentaria uni ON(aca.unicod = uni.unocod AND ptr.ptrano = uni.prsano)
            JOIN spo.ptressubunidade psu ON(ptr.ptrid = psu.ptrid AND uni.suoid = psu.suoid)
            LEFT JOIN (
                SELECT
                    pip2.ptrid,
                    ungcod,
                    (SUM(COALESCE(pc.picvalorcusteio, 0.00)) + SUM(COALESCE(pc.picvalorcapital, 0.00))) AS pipvalor,
                    SUM(COALESCE(pc.picvalorcusteio, 0.00)) AS custeio,
                    SUM(COALESCE(pc.picvalorcapital, 0.00)) AS capital
                FROM monitora.pi_planointernoptres pip2
                    JOIN monitora.pi_planointerno USING(pliid)
                    JOIN planacomorc.pi_complemento pc USING(pliid)
                WHERE
                    plistatus = 'A'
                GROUP BY
                    pip2.ptrid,
                    ungcod
            ) det ON(ptr.ptrid = det.ptrid AND uni.suocod = det.ungcod)
            LEFT JOIN (
                SELECT
                    ex.unicod,
                    pi.ungcod,
                    ex.ptres,
                    ex.exercicio,
                    sum(ex.vlrempenhado) AS total
                FROM spo.siopexecucao ex
                    JOIN monitora.pi_planointerno pi ON(ex.plicod = pi.plicod AND ex.exercicio = pi.pliano)
                WHERE
                    ex.exercicio = '{$filtros->exercicio}'
                GROUP BY
                    ex.unicod,
                    pi.ungcod,
                    ex.ptres,
                    ex.exercicio
                ) pemp ON(pemp.ptres = ptr.ptres AND pemp.exercicio = ptr.ptrano AND pemp.unicod = ptr.unicod AND uni.suocod = pemp.ungcod)
        WHERE
            ptr.ptrstatus = 'A'
            AND ptr.ptrano = '{$filtros->exercicio}' $where
        GROUP BY
            ptr.ptrid,
            ptr.ptres,
            psu.ptrdotacaocusteio,
            psu.ptrdotacaocapital,
            aca.prgcod,
            aca.acacod,
            aca.loccod,
            aca.acaobjetivocod,
            ptr.plocod,
            aca.unicod,
            uni.unonome,
            aca.acatitulo,
            pemp.total
        ORDER BY
            ptr.ptres
    ";
    return $sql;
}

/**
 * Monta o SQL da consulta de funcional para as subunidades.
 * 
 * @param stdClass $filtros "$filtros->exercicio" O Exercício é obrigatório.
 * @return string
 */
function montarSqlBuscarFuncionalImportacao(stdClass $filtros) {
    $where = NULL;
    
    # Filtros
    $where .= $filtros->prgcod? "\n AND UPPER(ptr.prgcod) LIKE('%". strtoupper($filtros->prgcod). "%')": NULL;
    $where .= $filtros->acacod? "\n AND UPPER(aca.acacod) LIKE('%". strtoupper($filtros->acacod). "%')": NULL;
    $where .= $filtros->unicod? "\n AND aca.unicod = '{$filtros->unicod}'": NULL;
    $where .= $filtros->ungcod? "\n AND uni.suocod = '{$filtros->ungcod}'": NULL;
    $where .= $filtros->buscalivre? "\n AND (TRIM(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) ILIKE('%" . $filtros->buscalivre . "%') OR dtl.ptres ILIKE '%" . $filtros->buscalivre . "%')": NULL;
    $where .= $filtros->ptrid? "\n AND ptr.ptrid = {$filtros->ptrid}": NULL;
    if($filtros->no_ptrid){
        if(is_array($filtros->no_ptrid)){
            $where .= "\n AND ptr.ptrid NOT IN('". join($filtros->no_ptrid, "','"). "')";
        } else {
            $where .= "\n AND ptr.ptrid != ". (int)$filtros->no_ptrid;
        }
    }
    if($filtros->eqdid){
        $where .= " \n AND ptr.irpcod IN(
                SELECT
                    eqrp.irpcod
                FROM monitora.enquadramentorp eqrp
                WHERE
                    eqrp.eqdid = ". (int)$filtros->eqdid. "
            )
        ";
    }
    
    $sql = "
        SELECT
            ptr.ptrid,
            ptr.ptres,
            TRIM(aca.prgcod) || '.' || TRIM(aca.acacod) || '.' || TRIM(aca.loccod) || '.' || (CASE WHEN LENGTH(TRIM(aca.acaobjetivocod)) <= 0 THEN '-' ELSE COALESCE(TRIM(aca.acaobjetivocod), '') END) || '.' || COALESCE(TRIM(ptr.plocod)) || ' - ' || aca.acatitulo || CASE WHEN LENGTH(TRIM(ptr.plodsc)) >= 0 THEN ': ' || ptr.plodsc ELSE '' END || ' (RP ' || COALESCE(ptr.irpcod, '') || ')' AS descricao,
            COALESCE(psu.ptrdotacaocusteio, 0.00) + COALESCE(psu.ptrdotacaocapital, 0.00) AS dotacaoatual,
            COALESCE(psu.ptrdotacaocusteio, 0.00) AS ptrdotacaocusteio,
            COALESCE(psu.ptrdotacaocapital, 0.00) AS ptrdotacaocapital,
            COALESCE(SUM(det.pipvalor), 0.00) AS det_pi,
            COALESCE(SUM(det.custeio), 0.00) AS det_pi_custeio,
            COALESCE(SUM(det.capital), 0.00) AS det_pi_capital,
            (COALESCE(psu.ptrdotacaocusteio, 0.00) - COALESCE(SUM(det.custeio), 0.00)) + COALESCE(psu.ptrdotacaocapital, 0.00) - COALESCE(SUM(det.capital), 0.00) AS nao_det_pi,
            COALESCE(psu.ptrdotacaocusteio, 0.00) - COALESCE(SUM(det.custeio), 0.00) AS nao_det_pi_custeio,
            COALESCE(psu.ptrdotacaocapital, 0.00) - COALESCE(SUM(det.capital), 0.00) AS nao_det_pi_capital,
            COALESCE((pemp.total), 0.00) AS empenhado,
            (COALESCE(psu.ptrdotacaocusteio, 0.00) + COALESCE(psu.ptrdotacaocapital, 0.00)) - COALESCE(pemp.total, 0.00) AS nao_empenhado
        FROM monitora.ptres ptr
            JOIN monitora.acao aca ON(ptr.acaid = aca.acaid)
            JOIN public.vw_subunidadeorcamentaria uni ON(aca.unicod = uni.unocod AND ptr.ptrano = uni.prsano)
            JOIN spo.ptressubunidade psu ON(ptr.ptrid = psu.ptrid AND uni.suoid = psu.suoid)
        LEFT JOIN (
            SELECT
                pip2.ptrid,
                ungcod,
                (SUM(COALESCE(pc.picvalorcusteio, 0.00)) + SUM(COALESCE(pc.picvalorcapital, 0.00))) AS pipvalor,
                SUM(COALESCE(pc.picvalorcusteio, 0.00)) AS custeio,
                SUM(COALESCE(pc.picvalorcapital, 0.00)) AS capital
            FROM monitora.pi_planointernoptres pip2
                JOIN monitora.pi_planointerno USING(pliid)
                JOIN planacomorc.pi_complemento pc USING(pliid)
            WHERE
                plistatus = 'A'
                AND pliano = '{$filtros->exercicio}'
            GROUP BY
                pip2.ptrid,
                ungcod
        ) det ON(ptr.ptrid = det.ptrid AND uni.suocod = det.ungcod)
        LEFT JOIN (
            SELECT
                ex.unicod,
                pi.ungcod,
                ex.ptres,
                ex.exercicio,
                sum(ex.vlrempenhado) AS total
            FROM spo.siopexecucao ex
                JOIN monitora.pi_planointerno pi ON(ex.plicod = pi.plicod AND ex.exercicio = pi.pliano)
            WHERE
                ex.exercicio = '{$filtros->exercicio}'
            GROUP BY
                ex.unicod,
                pi.ungcod,
                ex.ptres,
                ex.exercicio
            ) pemp ON(pemp.ptres = ptr.ptres AND pemp.exercicio = ptr.ptrano AND pemp.unicod = ptr.unicod AND uni.suocod = pemp.ungcod)
        WHERE
            ptr.ptrstatus = 'A'
            AND ptr.ptrano = '{$filtros->exercicio}' $where
        GROUP BY
            ptr.ptrid,
            ptr.ptres,
            psu.ptrdotacaocusteio,
            psu.ptrdotacaocapital,
            aca.prgcod,
            aca.acacod,
            aca.loccod,
            aca.acaobjetivocod,
            ptr.plocod,
            aca.unicod,
            uni.unonome,
            aca.acatitulo,
            pemp.total
        ORDER BY
            ptr.ptres
    ";
    return $sql;
}

/**
 * Monta o SQL da consulta de funcional para as subunidades.
 * 
 * @param stdClass $filtros "$filtros->exercicio" O Exercício é obrigatório.
 * @return string
 */
function montarSqlBuscarFuncionalFnc(stdClass $filtros) {
    $where = NULL;
    
    # Filtros
    $where .= $filtros->prgcod? "\n AND UPPER(ptr.prgcod) LIKE('%". strtoupper($filtros->prgcod). "%')": NULL;
    $where .= $filtros->acacod? "\n AND UPPER(aca.acacod) LIKE('%". strtoupper($filtros->acacod). "%')": NULL;
    $where .= $filtros->unicod? "\n AND aca.unicod = '{$filtros->unicod}'": NULL;
    if($filtros->ungcod){
        $where .= "\n AND uni.suocod = '{$filtros->ungcod}'";
        $whereCadastrado = "\n AND pli.ungcod = '{$filtros->ungcod}'";
    }
    $where .= $filtros->buscalivre? "\n AND (TRIM(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) ILIKE('%" . $filtros->buscalivre . "%') OR dtl.ptres ILIKE '%" . $filtros->buscalivre . "%')": NULL;
    if($filtros->no_ptrid){
        if(is_array($filtros->no_ptrid)){
            $where .= "\n AND ptr.ptrid NOT IN('". join($filtros->no_ptrid, "','"). "')";
        } else {
            $where .= "\n AND ptr.ptrid != ". (int)$filtros->no_ptrid;
        }
    }
    if($filtros->eqdid){
        $where .= " \n AND ptr.irpcod IN(
                SELECT
                    eqrp.irpcod
                FROM monitora.enquadramentorp eqrp
                WHERE
                    eqrp.eqdid = ". (int)$filtros->eqdid. "
            )
        ";
    }
    
    $sql = "
        SELECT
            ptr.ptrid,
            ptr.ptres
            || '<autorizadocusteio>' || COALESCE(ptr.ptrdotacaocusteio, 0.00) || '</autorizadocusteio>'
            || '<autorizadocapital>' || COALESCE(ptr.ptrdotacaocapital, 0.00) || '</autorizadocapital>'
            || '<cadastradocusteio>' || COALESCE(cadastrado.custeio, 0.00) || '</cadastradocusteio>'
            || '<cadastradocapital>' || COALESCE(cadastrado.capital, 0.00) || '</cadastradocapital>'
            AS ptres,
            TRIM(aca.prgcod) || '.' || TRIM(aca.acacod) || '.' || TRIM(aca.loccod) || '.' || (CASE WHEN LENGTH(TRIM(aca.acaobjetivocod)) <= 0 THEN '-' ELSE COALESCE(TRIM(aca.acaobjetivocod), '') END) || '.' || COALESCE(TRIM(ptr.plocod)) || ' - ' || aca.acatitulo || CASE WHEN LENGTH(TRIM(ptr.plodsc)) >= 0 THEN ': ' || ptr.plodsc ELSE '' END || ' (RP ' || COALESCE(ptr.irpcod, '') || ')' AS descricao,
            COALESCE(ptr.ptrdotacaocusteio, 0.00) + COALESCE(ptr.ptrdotacaocapital, 0.00) AS dotacaoatual,
            COALESCE(SUM(det.total), 0.00) AS det_pi,
            COALESCE(ptr.ptrdotacaocusteio, 0.00) - COALESCE(SUM(det.custeio), 0.00) AS nao_det_pi_custeio,
            COALESCE(ptr.ptrdotacaocapital, 0.00) - COALESCE(SUM(det.capital), 0.00) AS nao_det_pi_capital,
            COALESCE((pemp.total), 0.00) AS empenhado,
            (COALESCE(ptr.ptrdotacaocusteio, 0.00) + COALESCE(ptr.ptrdotacaocapital, 0.00)) - COALESCE(pemp.total, 0.00) AS nao_empenhado
        FROM monitora.ptres ptr
            JOIN monitora.acao aca ON(ptr.acaid = aca.acaid)
            JOIN public.vw_subunidadeorcamentaria uni ON(aca.unicod = uni.unocod AND ptr.ptrano = uni.prsano)
            JOIN spo.ptressubunidade psu ON(ptr.ptrid = psu.ptrid AND uni.suoid = psu.suoid)
            LEFT JOIN (
                SELECT
                    pip2.ptrid,
                    pli.ungcod,
                    (SUM(COALESCE(pc.picvalorcusteio, 0.00)) + SUM(COALESCE(pc.picvalorcapital, 0.00))) AS total,
                    SUM(COALESCE(pc.picvalorcusteio, 0.00)) AS custeio,
                    SUM(COALESCE(pc.picvalorcapital, 0.00)) AS capital
                FROM monitora.pi_planointernoptres pip2
                    JOIN monitora.pi_planointerno pli USING(pliid)
                    JOIN planacomorc.pi_complemento pc USING(pliid)
                    JOIN workflow.documento wd ON(pli.docid = wd.docid)
                    JOIN workflow.estadodocumento ed ON(wd.esdid = ed.esdid)
                WHERE
                    pli.plistatus = 'A'
                    AND pli.pliano = '{$filtros->exercicio}'
                    AND ed.esdid = ". ESD_FNC_PI_APROVADO. "
                GROUP BY
                    pip2.ptrid,
                    pli.ungcod
            ) det ON(ptr.ptrid = det.ptrid AND uni.suocod = det.ungcod)
            LEFT JOIN (
                SELECT
                    pip2.ptrid,
                    (SUM(COALESCE(pc.picvalorcusteio, 0.00)) + SUM(COALESCE(pc.picvalorcapital, 0.00))) AS total,
                    SUM(COALESCE(pc.picvalorcusteio, 0.00)) AS custeio,
                    SUM(COALESCE(pc.picvalorcapital, 0.00)) AS capital
                FROM monitora.pi_planointernoptres pip2
                    JOIN monitora.pi_planointerno pli USING(pliid)
                    JOIN planacomorc.pi_complemento pc USING(pliid)
                WHERE
                    pli.plistatus = 'A'
                    AND pli.pliano = '{$filtros->exercicio}' {$whereCadastrado}
                GROUP BY
                    pip2.ptrid
            ) cadastrado ON(ptr.ptrid = cadastrado.ptrid)
            LEFT JOIN (
                SELECT
                    ex.unicod,
                    pi.ungcod,
                    ex.ptres,
                    ex.exercicio,
                    sum(ex.vlrempenhado) AS total
                FROM spo.siopexecucao ex
                    JOIN monitora.pi_planointerno pi ON(ex.plicod = pi.plicod AND ex.exercicio = pi.pliano)
                WHERE
                    ex.exercicio = '{$filtros->exercicio}'
                GROUP BY
                    ex.unicod,
                    pi.ungcod,
                    ex.ptres,
                    ex.exercicio
                ) pemp ON(pemp.ptres = ptr.ptres AND pemp.exercicio = ptr.ptrano AND pemp.unicod = ptr.unicod AND uni.suocod = pemp.ungcod)
        WHERE
            ptr.ptrstatus = 'A'
            AND ptr.ptrano = '{$filtros->exercicio}' $where
        GROUP BY
            ptr.ptrid,
            ptr.ptres,
            ptr.ptrdotacaocusteio,
            ptr.ptrdotacaocapital,
            cadastrado.custeio,
            cadastrado.capital,
            aca.prgcod,
            aca.acacod,
            aca.loccod,
            aca.acaobjetivocod,
            ptr.plocod,
            aca.unicod,
            uni.unonome,
            aca.acatitulo,
            pemp.total
        ORDER BY
            ptr.ptres
    ";
//ver($sql,d);
    return $sql;
}

/**
 * Retorna os dados de limites detalhados em PI da Sub-Unidade.
 * 
 * @param stdClass $parametros
 * @return float Limite disponivel da Sub-Unidade.
 */
function carregarLimiteDetalhadoSubUnidade(stdClass $parametros) {
    global $db;

    $sql = "
        SELECT
            SUM(COALESCE(picvalorcusteio, 0) + COALESCE(picvalorcapital, 0)) AS detalhado
        FROM monitora.pi_planointerno pli
            JOIN monitora.pi_planointernoptres pliptr ON(pli.pliid = pliptr.pliid) -- SELECT * FROM monitora.pi_planointernoptres
            JOIN monitora.vw_ptres ptr ON(pliptr.ptrid = ptr.ptrid)
            JOIN public.vw_subunidadeorcamentaria suo ON(
                pli.unicod = suo.unocod
                AND pli.ungcod = suo.suocod
                AND suo.prsano = pli.pliano
                AND suo.unofundo = FALSE
            )
            JOIN planacomorc.pi_complemento pc ON(pli.pliid = pc.pliid)
        WHERE
            pli.plistatus = 'A'
            AND ptr.irpcod != '6'
            AND pli.pliano = '{$_SESSION['exercicio']}'
            AND pli.ungcod = '". $parametros->ungcod. "'
    ";
//ver($sql);
    $disponivel = $db->pegaUm($sql);
    return $disponivel;
}

/**
 * Retorna os dados de limites detalhados em PI da Sub-Unidade.
 * 
 * @param stdClass $parametros
 * @return float Limite disponivel da Sub-Unidade.
 */
function carregarLimiteDetalhadoUnidadeFnc(stdClass $parametros) {
    global $db;

    $sql = "
        SELECT
            suo.unoid,
            COALESCE((SUM(COALESCE(pc.picvalorcusteio, 0.00) + COALESCE(pc.picvalorcapital, 0.00))), 0.00) AS total
        FROM monitora.pi_planointerno pli
            JOIN planacomorc.pi_complemento pc USING(pliid)
            JOIN public.vw_subunidadeorcamentaria suo ON( -- SELECT * FROM public.vw_subunidadeorcamentaria suo
                suo.suostatus = 'A'
                AND pli.unicod = suo.unocod
                AND pli.ungcod = suo.suocod
                AND suo.prsano = pli.pliano
            )
            JOIN workflow.documento wd ON(pli.docid = wd.docid)
            JOIN workflow.estadodocumento ed ON(wd.esdid = ed.esdid)
        WHERE
            unofundo IS TRUE
            AND pli.plistatus = 'A'
            AND pli.pliano = '{$parametros->exercicio}'
            AND ed.esdid = ". ESD_FNC_PI_APROVADO. "
        GROUP BY
            suo.unoid
    ";
    
    $resultado = $db->pegaLinha($sql);
    return $resultado['total'];
}

/**
 *  Carrega as subações de uma UO
 */
function carregarComboSubacao($unicod, $ungcod = null, $retornaSQL = false) {
    global $db;
    $sql = <<<DML
SELECT DISTINCT sba.sbaid AS codigo,
                sba.sbacod || ' - ' || sba.sbatitulo AS descricao
  FROM monitora.pi_subacao sba
    INNER JOIN monitora.pi_subacaounidade sbu USING(sbaid)
    LEFT JOIN public.unidadegestora udg USING(ungcod)
  WHERE COALESCE(sbu.unicod, udg.unicod) = '%s' %s
    AND sba.sbaano = '{$_SESSION['exercicio']}'
    AND sba.sbastatus = 'A'    
  ORDER BY descricao
DML;
    $whereAdicional = '';
    if ($ungcod) {
        $whereAdicional = sprintf(" AND sbu.ungcod = '%s'", $ungcod);
    }
    $stmt = sprintf($sql, $unicod, $whereAdicional);
    if ($retornaSQL) {
        return $stmt;
    }

    $dados = $db->carregar($stmt);
    if (count($dados) && $dados[0]) {
        $infoCombo = 'Selecione';
    } else {
        $dados = array();
        $infoCombo = 'Nenhuma Subação encontrada';
    }
    //ver($stmt);
    //$db->monta_combo('sbaid', $dados, 'S', $infoCombo, 'carregarInfoPI', '', '', '240', 'S', 'sbaid', false);
    $db->monta_combo('sbaid', $dados, 'S', $infoCombo, 'selecionarsubacao', null, null, '240', 'N', 'sbaid', null, '', null, 'class="form-control chosen-select" '
            . 'style="width=100%;""', null, (isset($sbaid) ? $sbaid : null));
}

/**
 *  Carrega as subações de uma UO
 */
function carregarComboSubacaoUO($unicod, $retornaSQL = false) {
    global $db;
    $sql = <<<DML
SELECT DISTINCT sba.sbaid AS codigo,
                sba.sbacod || ' - ' || sba.sbatitulo AS descricao
  FROM monitora.pi_subacao sba
    INNER JOIN monitora.pi_subacaounidade sbu USING(sbaid)
    WHERE sbu.unicod = '%s' %s
    AND sba.sbaano = '{$_SESSION['exercicio']}'
    AND sba.sbastatus = 'A'
  ORDER BY descricao
DML;

    $whereAdicional = '';
    $stmt = sprintf($sql, $unicod, $whereAdicional);
    if ($retornaSQL) {
        return $stmt;
    }

    $dados = $db->carregar($stmt);
    if (count($dados) && $dados[0]) {
        $infoCombo = 'Selecione';
    } else {
        $dados = array();
        $infoCombo = 'Nenhuma Subação encontrada';
    }
    //$db->monta_combo('sbaid', $dados, 'S', $infoCombo, '', '', '', '240', 'S', 'sbaid', false);
    $db->monta_combo('sbaid', $dados, 'S', $infoCombo, 'selecionarsubacao', null, null, '240', 'N', 'sbaid', null, '', null, 'class="form-control chosen-select" style="width=100%;""', null, '');
}

/* Valida o PI */

function validaCodPi($pi, $pliid = false) {
    global $db;
    $sql = "SELECT plicod FROM monitora.pi_planointerno WHERE plistatus='A' AND date_part('Y', plidata) = '{$_SESSION['exercicio']}' AND plicod = '{$pi}'" . (($pliid) ? " AND pliid != '" . $pliid . "'" : "");
    $plicod = $db->PegaUm($sql);

    if (!$plicod) {
        $retorno = "";
        echo $retorno;
        exit;
    } else {
        $retorno = "pijaexiste";
        echo $retorno;
        $sql = "SELECT p.plicod as plicod, coalesce(p.plititulo,'Não preenchido') as titulo,
                    coalesce(SUM(pp.pipvalor),0) as total,
                    CASE WHEN p.plisituacao = 'P' THEN ' Pendente ' WHEN p.plisituacao = 'C' THEN ' Aprovado '
                         WHEN p.plisituacao = 'H' THEN ' Homologado ' WHEN p.plisituacao = 'V' THEN ' Revisado '
                         WHEN p.plisituacao = 'S' THEN ' Cadastrado no SIAFI ' WHEN p.plisituacao = 'R' THEN ' Enviado para Revisão ' END as situacao,
                    u.usunome ||' por '||to_char(p.plidata, 'dd/mm/YYYY hh24:mi'),
                    COALESCE(a._atinumero||' - '||a.atidescricao, 'Não atribuido')as atividade
                    FROM monitora.pi_planointerno p
                    LEFT JOIN monitora.pi_planointernoptres pp ON  pp.pliid=p.pliid
                    LEFT JOIN seguranca.usuario u ON u.usucpf = p.usucpf
                    LEFT JOIN monitora.pi_planointernoatividade pa on pa.pliid = p.pliid
                    LEFT JOIN pde.atividade a on a.atiid = pa.atiid
                    WHERE p.plicod='" . $plicod . "' AND p.plistatus = 'A'
                    GROUP BY p.plicod,p.plititulo,u.usunome,p.plidata,p.plisituacao,atividade
                    ORDER BY p.plidata DESC";
        $cabecalho = array("Código PI", "Título", "Total PI", "Situação", "Dados inserção", "Atividade");
        $db->monta_lista($sql, $cabecalho, 500, 10, 'N', '', '');
        exit;
    }
}

/* Retorna os dados da Subação */

function buscaDadosSubacao($sbaid, $capid = "", $retornarArray = false) {
    global $db;

    $sql = <<<DML
        SELECT
            psa.sbacod,
            psa.sbasigla,
            psa.sbadsc,
            COALESCE(SUM(psd.sadvalor), 0.00) AS dotacao,
            COALESCE(SUM(dpp2.valorpi), 0.00) AS detalhado_pi,
            COALESCE(SUM(sbe.total), 0.00) AS empenhado
        FROM monitora.pi_subacao psa
        LEFT JOIN monitora.pi_subacaodotacao psd USING(sbaid)
        --LEFT JOIN monitora.ptres ptr USING(ptrid)
        LEFT JOIN (
            SELECT
                dpp.sbaid,
                dpp.ptrid,
                SUM(dpp.valorpi) AS valorpi
            FROM monitora.v_pi_detalhepiptres dpp
            GROUP BY dpp.sbaid,dpp.ptrid)
            dpp2 USING(ptrid, sbaid)
        LEFT JOIN siafi.sbaempenho sbe ON(sbe.sbacod = psa.sbacod AND sbe.exercicio = '{$_SESSION['exercicio']}')
        WHERE psa.sbaid = %d
            AND psa.sbaano = '%s'
            --AND ptr.ptrano='{$_SESSION['exercicio']}'
        GROUP BY psa.sbacod,
            psa.sbasigla,
            psa.sbadsc
DML;
    $stmt = sprintf($sql, $sbaid, $_SESSION['exercicio']);
    $subacao = $db->pegaLinha($stmt);

    $categoria = "";
    if ($capid) {
        $sql = "SELECT capdsc FROM monitora.pi_categoriaapropriacao WHERE capid='" . $capid . "'";
        $categoria = $db->pegaUm($sql);
    }
    $arrRetorno = array('sbacod' => $subacao['sbacod']);
    $arrRetorno['sbasigla'] = $subacao['sbasigla'];
    $arrRetorno['categoria'] = $categoria;
    $arrRetorno['sbadsc'] = $subacao['sbadsc'];
    $arrRetorno['dotacao'] = number_format($subacao['dotacao'], 2, ',', '.');
    $arrRetorno['detalhado_pi'] = number_format($subacao['detalhado_pi'], 2, ',', '.');
    $arrRetorno['empenhado'] = number_format($subacao['empenhado'], 2, ',', '.');

    if ($retornarArray) {
        return $arrRetorno;
    }
    echo implode('!@#', $arrRetorno);
}

function recuperarObjetivoPorPtres($ptrid) {
    global $db;

    $sql = "SELECT o.oppid
                FROM monitora.ptres ptr
                INNER JOIN monitora.acao aca on ptr.acaid = aca.acaid and aca.prgano = ptr.ptrano
                INNER JOIN public.objetivoppa o on o.oppcod = aca.acaobjetivocod and o.prsano = ptr.ptrano
            where ptrid = $ptrid";

    return $db->pegaUm($sql);
}

/* Carregar os enquadramentos para a Subação */

function carregarComboEnquadramentoPorSubacao($sbaid) {
    global $db;

    if ($sbaid) {
        $sql = "SELECT ed.eqdid as codigo, ed.eqdcod ||' - '|| ed.eqddsc as descricao
              FROM monitora.pi_enquadramentodespesa ed
                   INNER JOIN monitora.pi_subacaoenquadramento se on ed.eqdid = se.eqdid
              WHERE ed.eqdano='" . $_SESSION['exercicio'] . "' and ed.eqdstatus='A' and se.sbaid=$sbaid
              ORDER BY ed.eqdcod";
        $arDados = $db->carregar($sql);
        if (!$arDados) {
            $arDados = array();
        }
        //die($db->monta_combo('eqdid', $arDados, 'S', 'Selecione', 'atualizarPrevisaoPI', '', '', '240', 'S', 'eqdid', false));
        die($db->monta_combo('eqdid', $arDados, 'S', 'Selecione', 'atualizarPrevisaoPI', null, null, 240, 'N', 'eqdid', null, (isset($eqdid) ? $eqdid : null), null, 'class="form-control chosen-select" style="width=100%;""', null, null));
    }
}

/**
 * Salva os PI's que foram do FNDE na base do ELABREV
 * para ser usado no Termo de Cooperação Descentralizada
 * @param $unicod
 * @param $plicod
 * @return bool
 */
function _salvarPI_ElabrevTED($unicod, $plicod) {
    global $db;

    if ($unicod != '26298') {
        return false;
    }

    $strSQL = "
        insert into ted.dadosprogramasfnde
            (prgcodfnde, plicod, gescod, tpddoccod, obscod, eventocontabil)
        values('3', '{$plicod}', '61500000000', 'NC', '2', '300300')
    ";

    $db->executar($strSQL);
    $db->commit();
    return true;
}

/**
 * Faz a inserção do PI no banco de dados do monitora.
 *
 * @global cls_banco $db Conexão com a base de dados.
 * @param array $dados Array com os dados da requisição
 * @param bool $comCommit Se deve ou não ser feito um commit após a inserção.
 * @param bool $criarComoAprovado
 *              Indica que, ao executar uma transação do tipo 'E', o status do PI deve ser APROVADO.
 * @return type bool|integer
 */
function salvarPI($dados, $comCommit = true, $criarComoAprovado = false) {
    global $db, $obrigatorias_array;

    $unicod = $dados['unicod'] ? $dados['unicod'] : $dados['unicod_disable'];

    /* Apenas para PI de Unidades */
    $cadastroSIAF = 'false';
    if ($dados['plicadsiafi'] == 'T') {
        $cadastroSIAF = 'true';
    }
    $plicodC = strtoupper($dados['plicod']);

    $sql = <<<DML
        SELECT
            *
        FROM monitora.pi_planointerno
        WHERE
            plicod = '{$plicodC}'
            AND pliano = '{$_SESSION['exercicio']}'
            AND unicod = '{$unicod}'
            AND plistatus = 'A'
DML;
    
    # Soma valores preenchidos pelo usuário na parte de Capital e Custeio do PI
    $totalValor = str_replace(array('.', ','), array('', '.'), $dados['picvalorcusteio']) + str_replace(array('.', ','), array('', '.'), $dados['picvalorcapital']);
    $totalValorTemplate = number_format($totalValor, 2, ',', '.');


    $dados['mdeid'] = $dados['mdeid'] ? $dados['mdeid'] : 'null';
    $dados['neeid'] = $dados['neeid'] ? $dados['neeid'] : 'null';
    $dados['capid'] = $dados['capid'] ? $dados['capid'] : 'null';

    $plicod = null; //$db->PegaUm($sql);
    if (empty($dados['pliid'])) {
        if ($dados['plicodsubacao']) {
            $subacao = strtoupper($dados['plicodsubacao']);
        } else {
            $subacao = strtoupper(substr($plicod, 1, 4));
        }
        if (!$plicod) {
            $plicod = strtoupper($dados['plicod']);
            $plicod = str_replace(' ', '', $plicod);
            $sql = <<<DML
                INSERT INTO monitora.pi_planointerno(
                    mdeid,
                    eqdid,
                    neeid,
                    capid,
                    sbaid,
                    plititulo,
                    plicodsubacao,
                    plicod,
                    plilivre,
                    plidsc,
                    usucpf,
                    unicod,
                    ungcod,
                    pliano,
                    plisituacao,
                    plicadsiafi
                ) VALUES (%s, %d, %s, %s, %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s','%s')
                RETURNING
                    pliid;
DML;
            
            $stmt = sprintf(
                    $sql, $dados['mdeid'], $dados['eqdid'], $dados['neeid'], $dados['capid'], $dados['sbaid'], str_replace(array("'"), ' ', $dados['plititulo']), $subacao, $plicod, $dados['plilivre'], str_replace(array("'"), ' ', $dados['plidsc']), $_SESSION['usucpf'], $unicod, $dados['ungcod'], $_SESSION['exercicio'], ($criarComoAprovado ? 'A' : 'H'), $cadastroSIAF);
            $pliid = $db->pegaUm($stmt);
            
            //Grava usuário que salvou por último
            gravarUsuarioAlteracao($pliid);
            
            // Grava informações complementares
            salvarPiComplemento($pliid, $dados);

            if($dados['ptrid']){
                // -- Associando o pi aos ptres
                associarPIePTRES($pliid, NULL, array($dados['ptrid'] => $totalValorTemplate));
            }

            /**
             * Inserindo as novas associações PI/Enquadramento
             * 
             * @todo Verificar a necessidade desse método. Se ele não for necessário ao MINC, ele deverá ser excluído.
             */
            associarPIeEnquadramento($pliid, $dados['m_eqdid']);
        } else {
            echo '<script>alert(\'PI: ' . $plicodC . ' já cadastrado para esta unidade.\');location.href= \'planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A\'</script>';
            die();
        }
    } else {


        $mPiPlanoInterno = new Pi_PlanoInterno($dados['pliid']);
        $perfis = pegaPerfilGeral();
        $estadoAtual = wf_pegarEstadoAtual($mPiPlanoInterno->docid);


        $podeEditar = $mPiPlanoInterno->verificarPermissaoEditar($estadoAtual, $perfis);

        if($podeEditar){

        $sql = <<<DML
            UPDATE monitora.pi_planointerno SET
                plititulo = '%s',
                plidsc = '%s',
                mdeid = %s,
                eqdid = %s,
                neeid = %s,
                capid = %s,
                plicadsiafi = {$cadastroSIAF}
            WHERE
                pliid = %d
DML;
        $stmt = sprintf($sql,
            trim($dados['plititulo']),
            trim($dados['plidsc']),
            $dados['mdeid'],
            $dados['eqdid'],
            $dados['neeid'],
            $dados['capid'],
            $dados['pliid']);
        $db->executar($stmt);

        /**
         * Inserindo as novas associações PI/Enquadramento
         * 
         * @todo Verificar a necessidade desse método. Se ele não for necessário ao MINC, ele deverá ser excluído.
         */
        desassociarPIeEnquadramento($dados['pliid']);

        // -- Apagando os ptres já associados, para uma posterior re-inserção
        if ($dados['obrigatoria'] != '0') {
            desassociarPIePTRES($dados['pliid']);
        }
        //carregando array com ptres cadastrados para comparar com o array que foi desmarcado.
        $arrPtrids[] = $db->carregarColuna("SELECT ptrid FROM monitora.pi_planointernoptres WHERE pliid = " . $dados['pliid']);
        if (count($dados['ids_apagar']) > 0) {
            foreach ($arrPtrids as $arrPtrid) {
                $arrPtrid = $arrPtrid;
            }
            foreach ($dados['ids_apagar'] as $ptrid) {
                if (in_array($ptrid, $arrPtrid)) {
                    removerPTRESdoPI($dados['pliid'], $ptrid);
                }
            }
        }
        if ($dados['pliid']) {
            $pliidFinal = $dados['pliid'];
        } else {
            $pliidFinal = $pliid;
        }

        // Grava informações complementares
        $pliid = $dados['pliid'];
        
        //Grava usuário que salvou por último
        gravarUsuarioAlteracao($pliid);
        
        salvarPiComplemento($pliid, $dados);

        if($dados['ptrid']){
            // Inserindo as novas associações PI/PTRES
            associarPIePTRES($pliidFinal, NULL, array($dados['ptrid'] => $totalValorTemplate));
        }

        /**
         * Inserindo as novas associações PI/Enquadramento
         * 
         * @todo Verificar a necessidade desse método. Se ele não for necessário ao MINC, ele deverá ser excluído.
         */
        associarPIeEnquadramento($pliidFinal, $dados['m_eqdid']);

        } else {
            $pliid = $dados['pliid'];

            associarConvenio($pliid, $dados);
            associarSniic($pliid, $dados);
            associarSei($pliid, $dados);
            associarPronac($pliid, $dados);
        }
    }

    //Salva PI na base do Elabrev para Termos de Cooperação pertencentes ao FNDE
    _salvarPI_ElabrevTED($unicod, $dados['plicod']);

    if ($comCommit) {
        $db->commit();
    }

    return $pliid;
}

function gravarUsuarioAlteracao($pliid){
    $historicoPiUsuario = new Planacomorc_Model_HistoricoPiUsuario();
    $historicoPiUsuario->pliid = $pliid;
    $historicoPiUsuario->usucpf = $_SESSION['usucpf'];
    $historicoPiUsuario->salvar();
}

function salvarPiComplemento($pliid, $dados)
{
    include_once APPRAIZ . "planacomorc/classes/Pi_Complemento.class.inc";

    # Fix - Corrigindo formato dos dados de valores orçámentários
    $dados['picvalorcusteio'] = $dados['picvalorcusteio']? str_replace(array('.', ','), array('', '.'), $dados['picvalorcusteio']): NULL;
    $dados['picvalorcapital'] = $dados['picvalorcapital']? str_replace(array('.', ','), array('', '.'), $dados['picvalorcapital']): NULL;
    
    $modelPiComplemento = new Pi_Complemento($pliid);
    $modelPiComplemento->popularDadosObjeto($dados);
    $modelPiComplemento->pliid = $pliid;
    $modelPiComplemento->mpnid = $dados['mpnid'] ? $dados['mpnid'] : null;
    $modelPiComplemento->ipnid = $dados['ipnid'] ? $dados['ipnid'] : null;
    $modelPiComplemento->mescod = $dados['mescod'] ? $dados['mescod'] : null;
    $modelPiComplemento->maiid = $dados['maiid'] ? $dados['maiid'] : null;
    $modelPiComplemento->masid = $dados['masid'] ? $dados['masid'] : null;
    $modelPiComplemento->pijid = $dados['pijid'] ? $dados['pijid'] : null;
    $modelPiComplemento->ptaid = $dados['ptaid'] ? $dados['ptaid'] : null;
    $modelPiComplemento->picpublico = str_replace(array("'"), ' ', $dados['picpublico']);
    $modelPiComplemento->picexecucao = $dados['picexecucao']? desformata_valor($dados['picexecucao']): null;
    $modelPiComplemento->picted = $dados['picted'] == 't' ? 't' : 'f';
    $modelPiComplemento->picedital = $dados['picedital'] == 't' ? 't' : 'f';

    $modelPiComplemento->salvar(NULL, NULL, array('ptaid', 'pijid', 'oppid', 'mppid', 'ippid', 'pprid', 'pumid', 'picpriorizacao', 'picquantidade', 'picpublico', 'picexecucao', 'picvalorcusteio', 'picvalorcapital'));

    associarConvenio($pliid, $dados);
    associarSniic($pliid, $dados);
    associarSei($pliid, $dados);
    associarPronac($pliid, $dados);
    associarLocalizacao($pliid, $dados);
    associarResponsavel($pliid, $dados);
    associarAnexos($pliid, $dados);
    associarCronograma($pliid, $dados);
    associarDelegacao($pliid, $dados);
}

function associarDelegacao($pliid, $dados)
{
    $mDelegacao = new Planacomorc_Model_PiDelegacao();
    $mDelegacao->excluirVarios("pliid = '$pliid'");

    if(isset($dados['delegacao']) && is_array($dados['delegacao'])){

        $mDelegacao->pliid = $pliid;
        foreach($dados['delegacao'] as $suoid){
            if ($suoid!=''){
                $mDelegacao->suoid = $suoid;
                $mDelegacao->salvar();

                $mDelegacao->pdeid = null;
            }
        }
    }
}

function associarAcao($pliid, $dados)
{
    include_once APPRAIZ . "planacomorc/classes/Pi_Acao.class.inc";

    // Vinculando Ações
    $modelPiAcao = new Pi_Acao();

    $modelPiAcao->excluirVarios("pliid = $pliid");
    if(isset($dados['acaid']) && is_array($dados['acaid'])){

        $modelPiAcao->pliid = $pliid;

        foreach($dados['acaid'] as $acaid){
            $modelPiAcao->acaid = $acaid;
            $modelPiAcao->salvar();
            $modelPiAcao->piaid = null;
        }
    }
}

function associarConvenio($pliid, $dados)
{
    include_once APPRAIZ . "planacomorc/classes/Pi_Convenio.class.inc";

    // Vinculando Ações
    $modelPiConvenio= new Pi_Convenio();
    $modelPiConvenio->excluirVarios("pliid = $pliid");

    if(isset($dados['lista_convenio']) && is_array($dados['lista_convenio'])){

        $modelPiConvenio->pliid = $pliid;

        foreach($dados['lista_convenio'] as $pcoconvenio){
            $modelPiConvenio->pcoconvenio = $pcoconvenio;
            $modelPiConvenio->salvar();
            $modelPiConvenio->pcoid = null;
        }
    }
}

function associarSniic($pliid, $dados)
{
    include_once APPRAIZ . "planacomorc/classes/Pi_Sniic.class.inc";

    // Vinculando Ações
    $modelPiSniic= new Pi_Sniic();
    $modelPiSniic->excluirVarios("pliid = $pliid");

    if(isset($dados['lista_sniic']) && is_array($dados['lista_sniic'])){

        $modelPiSniic->pliid = $pliid;

        foreach($dados['lista_sniic'] as $pissniic){
            $modelPiSniic->pissniic = $pissniic;
            $modelPiSniic->salvar();
            $modelPiSniic->pisid = null;
        }
    }
}

function associarSei($pliid, $dados)
{
    // Vinculando Ações
    $modelPiSei= new Planacomorc_Model_PiSei();
    $modelPiSei->excluirVarios("pliid = $pliid");

    if(isset($dados['lista_sei']) && is_array($dados['lista_sei'])){

        $modelPiSei->pliid = $pliid;

        foreach($dados['lista_sei'] as $psesei){
            $modelPiSei->psesei = $psesei;
            $modelPiSei->salvar();
            $modelPiSei->pseid = null;
        }
    }
}

function associarPronac($pliid, $dados)
{
    // Vinculando Ações
    $modelPiPronac= new Planacomorc_Model_PiPronac();
    $modelPiPronac->excluirVarios("pliid = $pliid");

    if(isset($dados['lista_pronac']) && is_array($dados['lista_pronac'])){

        $modelPiPronac->pliid = $pliid;

        foreach($dados['lista_pronac'] as $pprpronac){
            $modelPiPronac->pprpronac = $pprpronac;
            $modelPiPronac->salvar();
            $modelPiPronac->pprid = null;
        }
    }
}

function associarResponsavel($pliid, $dados)
{
    include_once APPRAIZ . "planacomorc/classes/Pi_Responsavel.class.inc";

    // Vinculando Responsáveis
    $modelPiResponsavel= new Pi_Responsavel();

    $modelPiResponsavel->excluirVarios("pliid = $pliid");
    if(isset($dados['listaResponsaveis']) && is_array($dados['listaResponsaveis'])){

        $modelPiResponsavel->pliid = $pliid;

        foreach($dados['listaResponsaveis'] as $usucpf){
            $modelPiResponsavel->usucpf = $usucpf;
            $modelPiResponsavel->salvar();
            $modelPiResponsavel->pirid = null;
        }
    }
}

function associarAnexos($pliid, $dados)
{
    include_once APPRAIZ . "planacomorc/classes/Pi_Anexo.class.inc";
    
    $modelPiAnexo = new Pi_Anexo();
    
    # Excluindo vinculo de anexos
    $modelPiAnexo->excluirVarios("pliid = $pliid");
    
    if(isset($dados['listaAnexos']) && is_array($dados['listaAnexos'])){
        # Vinculando Anexos
        $modelPiAnexo->pliid = $pliid;
        foreach($dados['listaAnexos'] as $arqid){
            $modelPiAnexo->arqid = $arqid;
            $modelPiAnexo->salvar();
            $modelPiAnexo->piaid = null;
        }
    }
}

function associarLocalizacao($pliid, $dados)
{
    include_once APPRAIZ . "planacomorc/classes/Pi_Localizacao.class.inc";

    // Vinculando Ações
    $modelPiLocalizacao= new Pi_Localizacao();

    $modelPiLocalizacao->excluirVarios("pliid = $pliid");

    $dadosLocalizacao = [];

    # Estadual
    if(isset($dados['listaLocalizacaoEstadual']) && is_array($dados['listaLocalizacaoEstadual'])){
        $dadosLocalizacao = $dados['listaLocalizacaoEstadual'];
    # Municipal
    } elseif(isset($dados['listaLocalizacao']) && is_array($dados['listaLocalizacao'])){
        $dadosLocalizacao = $dados['listaLocalizacao'];
    # Exterior
    }elseif(isset($dados['listaLocalizacaoExterior']) && is_array($dados['listaLocalizacaoExterior'])){
        $dadosLocalizacao = $dados['listaLocalizacaoExterior'];
    }

    $modelPiLocalizacao->pliid = $pliid;
    foreach($dadosLocalizacao as $localizacao){
        switch($dados['esfid']){
            case Territorios_Model_Esfera::K_EXTERIOR:  $modelPiLocalizacao->paiid = $localizacao;  break;
            case Territorios_Model_Esfera::K_ESTADUAL:  $modelPiLocalizacao->estuf = $localizacao;  break;
            case Territorios_Model_Esfera::K_MUNICIPAL: $modelPiLocalizacao->muncod = $localizacao; break;
        }

        $modelPiLocalizacao->salvar();
        $modelPiLocalizacao->pilid = null;
    }
}


function associarCronograma($pliid, $dados)
{       
    include_once APPRAIZ . "planacomorc/classes/Pi_Cronograma.class.inc";

    // Vinculando Ações

    if(isset($dados['cronograma']) && is_array($dados['cronograma'])){
        foreach($dados['cronograma'] as $mescod => $cronogramaValor){
            foreach($cronogramaValor as $crvid => $pcrvalor){
                
                $modelPiCronograma = new Pi_Cronograma($pcrvalor['pcrid']);
                if($pcrvalor['pcrvalor']){
                    $modelPiCronograma->pliid = $pliid;
                    $modelPiCronograma->mescod = $mescod;
                    $modelPiCronograma->crvid = $crvid;
                    $modelPiCronograma->pcrvalor = $pcrvalor['pcrvalor'] ? desformata_valor($pcrvalor['pcrvalor']) : null;
                    
                    $modelPiCronograma->salvar();
                    unset($modelPiCronograma);
                } elseif($pcrvalor['pcrid']){
                    $modelPiCronograma->excluir($pcrvalor['pcrid']);
                }
            }
        }
    }
}

function associarPIePTRES($pliid, $pliNovosPTRES, $pliPTRESAssociados) {
    //ver($pliid,$pliNovosPTRES, $pliPTRESAssociados,d);
    global $db;

    $sql = <<<DML
INSERT INTO monitora.pi_planointernoptres(pliid, ptrid, pipvalor)
  VALUES(%d, %d, %f)
DML;
    // -- Inserindo dotações selecionadas agora
    if ($pliNovosPTRES) {
        foreach ($pliNovosPTRES as $valor) {
            $stmt = sprintf(
                    $sql, $pliid, key($valor), str_replace(array('.', ','), array('', '.'), current($valor))
            );
            //ver($stmt,d);
            $db->executar($stmt);
        }
    }

    // -- Inserindo dotações selecionadas previamente
    if ($pliPTRESAssociados) {
        foreach ($pliPTRESAssociados as $PTRES => $valor) {
            $stmt = sprintf(
                    $sql, $pliid, $PTRES, str_replace(array('.', ','), array('', '.'), $valor)
            );
            $db->executar($stmt);
        }
    }
}

function associarPIeEnquadramento($pliid, $enquadramento) {
    //ver($pliid,$pliNovosPTRES, $pliPTRESAssociados,d);
    global $db;
    if (sizeof($enquadramento) == 1 && $enquadramento[0] == '')
        return;
    $sql = <<<DML
INSERT INTO spo.planointernometapne(pliid, mpneid)
  VALUES
DML;
    // -- Inserindo dotações selecionadas agora
    if ($enquadramento && count($enquadramento) > 0) {
        foreach ($enquadramento as $valor) {
            if ($valor == '') {
                continue;
            }
            $sql .="($pliid,$valor),";
        }
        $db->carregar(substr($sql, 0, -1));
    }
}

function removerPTRESdoPI($pliid, $ptrid) {
    global $db;
    $sql = <<<DML
DELETE
  FROM monitora.pi_planointernoptres
  WHERE pliid = %d AND ptrid =%d
DML;
    $stmt = sprintf($sql, $pliid, $ptrid);
    //exit($stmt);
    return $db->executar($stmt);
}

function desassociarPIePTRES($pliid) {
    global $db;
    $sql = <<<DML
DELETE
  FROM monitora.pi_planointernoptres
  WHERE pliid = %d
DML;
    $stmt = sprintf($sql, $pliid);
    $db->executar($stmt);
}

function desassociarPIeEnquadramento($pliid) {
    global $db;
    $sql = <<<DML
DELETE
  FROM spo.planointernometapne
  WHERE pliid = %d
DML;
    $stmt = sprintf($sql, $pliid);
    $db->executar($stmt);
}

/**
 * Busca os PTRES associados a um PI
 * Query utilizada também em: simec/monitora/modulos/principal/planotrabalhoUG/listarProgramaUG.inc
 * @global cls_banco $db Conexão com a base de dados.
 * @param integer $pliid
 * @param integer $sbaid
 * @return bol|array
 *
 * @global cls_banco $db
 * @param integer $pliid
 * @param integer $sbaid
 * @param type $obrigatorio
 * @return array|bol
 */
function buscarPTRESdoPI($pliid, $sbaid, $obrigatorio = '') {
    global $db;

    $params['SELECT'] = <<<SQL
SELECT dtl.ptrid,
       dtl.ptres,
       trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acatitulo) AS descricao,
       uni.unidsc,
       COALESCE(SUM(dtl.ptrdotacao), 0.00) AS dotacaoatual,
       COALESCE(SUM(dt.valor), 0.00) AS det_subacao,
       -- dotacaoinicial - det_subacao
       (COALESCE(SUM(dtl.ptrdotacao), 0.00) - COALESCE(SUM(dt.valor), 0.00)) AS nao_det_subacao,
       COALESCE(SUM(dt2.valorpi), 0.00) AS det_pi,
       -- det_subacao - det_pi
       (COALESCE(SUM(dt.valor), 0.00) - COALESCE(SUM(dt2.valorpi), 0.00)) AS nao_det_pi,
       COALESCE((pemp.total), 0.00) AS empenhado,
       COALESCE(SUM(dtl.ptrdotacao), 0.00) - COALESCE(pemp.total, 0.00) AS nao_empenhado,
       (SELECT pipvalor FROM monitora.pi_planointernoptres WHERE ptrid = dtl.ptrid AND pliid = {$pliid}) as pipvalor
SQL;
    /* Filtros */
    if ($obrigatorio == 'N') {
        $params['obrigatorio'] = 'n';
    }
    $where .= $sbaid ? " AND dt.ptrid IN (SELECT ptrid FROM monitora.pi_subacaodotacao WHERE sbaid = '" . $sbaid . "')" : '';
    $where .= $pliid ? " AND pli.pliid = $pliid " : "";

    /* Parametros para montar a consulta */
    $params['where'] = $where;
    $sql = retornaConsultaPTRES($params);
    $result = is_array($result) ? $result : Array();

    $result = $db->carregar($sql);
    if (is_array($result)) {
        foreach ($result as $key => $_) {
            $result[$key]['dotacaoatual'] = mascaraMoeda($result[$key]['dotacaoatual']);
            $result[$key]['det_subacao'] = mascaraMoeda($result[$key]['det_subacao']);
            $result[$key]['nao_det_subacao'] = mascaraMoeda($result[$key]['nao_det_subacao']);
            $result[$key]['det_pi'] = mascaraMoeda($result[$key]['det_pi']);
            $result[$key]['nao_det_pi'] = mascaraMoeda($result[$key]['nao_det_pi']);
            $result[$key]['empenhado'] = mascaraMoeda($result[$key]['empenhado']);
            $result[$key]['nao_empenhado'] = mascaraMoeda($result[$key]['nao_empenhado']);
            $result[$key]['pipvalor_'] = $result[$key]['pipvalor']; // -- Não formatado - para soma na interface
            $result[$key]['pipvalor'] = number_format($result[$key]['pipvalor'], 2, ',', '.');
        }
    }
    return $result;
}

/**
 *
 * @global cls_banco $db
 * @param type $dados
 * @return type
 * @todo Ao executar uma transação direta, o PI deve ter seu status atualizado para aprovado.
 */
function salvarSolicitacaoPI($dados) {
    global $db;
//ver($dados,d);
    // -- Solicitações com esta configuração, implicam em uma transação
    // -- direta (uma transação de execução, sem uma transação de solicitação).
    // -- Este tipo de transação deve atender às seguintes condições:
    // -- 1) O usuário deve ser superusuário, OU
    // -- 2) Os PTRESs selecionados devem ser da mesma UO do usuário.
    // -- Se nem 1) ou 2) forem atendidas, a transação deverá ser modificada
    // -- para transação de solicitação
    $podeCriarTransacaoDireta = false;
    if (empty($dados['scpid']) && ('E' == $dados['tipotransacao'])) {
//ver($dados,$dados['plivalor'],d);
        // if (!($podeCriarTransacaoDireta = podeCriarTransacaoDireta($dados['plivalor']))) {
//            $dados['tipotransacao'] = 'S';
//        }
    }

    $sql = <<<DML
INSERT INTO planacomorc.solicitacaocriacaopi(
    scpano,
    scptitulo,
    scpdsc,
    scpcod,
    unicod,
    ungcod,
    sbaid,
    scpdotacaosubacao,
    scpdetalhadopisubacao,
    scpempenhadosubacao,
    eqdid,
    neeid,
    capid,
    scplivre,
    mdeid,
    usucpf,
    scpidorigem,
    tipotransacao
) VALUES(%s, %s, '%s', %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, '%s', %s, '%s') RETURNING scpid
DML;

    $dados['plititulo'] = str_replace("'", "''", trim($dados['plititulo']));
    $dados['scpdotacaosubacao'] = formataFloat($dados['scpdotacaosubacao']);
    $dados['scpdetalhadopisubacao'] = formataFloat($dados['scpdetalhadopisubacao']);
    $dados['scpempenhadosubacao'] = formataFloat($dados['scpempenhadosubacao']);

    $stmt = sprintf(
            $sql, $_SESSION['exercicio'], empty($dados['plititulo']) ? 'null' : "'{$dados['plititulo']}'", trim(str_replace("'", "''", $dados['plidsc'])), empty($dados['plicod']) ? 'null' : "'{$dados['plicod']}'", empty($dados['unicod']) ? 'null' : "'{$dados['unicod']}'", empty($dados['ungcod']) ? 'null' : "'{$dados['ungcod']}'", empty($dados['sbaid']) ? 'null' : $dados['sbaid'], empty($dados['scpdotacaosubacao']) ? 'null' : "'{$dados['scpdotacaosubacao']}'", empty($dados['scpdetalhadopisubacao']) ? 'null' : "'{$dados['scpdetalhadopisubacao']}'", empty($dados['scpempenhadosubacao']) ? 'null' : "'{$dados['scpempenhadosubacao']}'", empty($dados['eqdid']) ? 'null' : $dados['eqdid'], empty($dados['neeid']) ? 'null' : $dados['neeid'], empty($dados['capid']) ? 'null' : $dados['capid'], empty($dados['plilivre']) ? 'null' : "'{$dados['plilivre']}'", empty($dados['mdeid']) ? 'null' : $dados['mdeid'], $_SESSION['usucpf'], empty($dados['scpid']) ? 'null' : $dados['scpid'], empty($dados['tipotransacao']) ? 'S' : $dados['tipotransacao']
    );

    if ($scpid = $db->pegaUm($stmt)) {
        // -- Insere as dotações selecionadas e solicitadas
        solicitacaoPIePTRES($scpid, $dados['plivalor'], $dados['plivalored']);
    }

    if ($dados['scpid']) {
        concluirSolicitacaoOrigem($dados['scpid']);
    }

    // -- Criar o PI
    if ('E' == $dados['tipotransacao']) {
        $pliid = salvarPI($dados, $comCommit = false, $podeCriarTransacaoDireta);
    }

    $success = $db->commit();
#ver($success,d);
    if ($success) {
        if ('S' == $dados['tipotransacao']) { // -- Manda e-mail de solicitação de PI
            enviaEmailPI(
                    array(
                        'tipoEvento' => 'solCadPI',
                        'scpid' => $scpid
                    )
            );
        } elseif ('E' == $dados['tipotransacao']) { // -- Manda e-mail de homologação de PI
            enviaEmailPI(
                    array(
                        'tipoEvento' => 'homCadPI',
                        'scpid' => ($dados['scpid'] ? $dados['scpid'] : $scpid),
                        'pliid' => $pliid
                    )
            );

            // -- Quer dizer que o PI também foi criado automaticamente, então tem que notificar
            // -- Sobre a aprovação deste PI
            if ($podeCriarTransacaoDireta) {
                enviaEmailPI(
                        array(
                            'tipoEvento' => 'aprCadPi',
                            'pliid' => $pliid
                        )
                );
            }
        }
    }

    return $success;
}

/**
 * Verifica se o usuário que fez a solicitação pode fazer uma transação direta.
 *
 * @global cls_banco $db Conexão com a base de dados.
 * @param array $PTRESParaInsercao Lista de PTRESs que serão incluídos no PI.
 * @return boolean
 */
function podeCriarTransacaoDireta(array $PTRESParaInsercao) {
    //ver($PTRESParaInsercao,d);
    global $db;

    // -- Se for super usuário pode criar transação direta
    if ($_SESSION['superuser']) {
        return true;
    }

    // -- Se não for do perfil GO, não pode criar transação direta
    if (!in_array(PFL_GESTAO_ORCAMENTARIA, pegaPerfilGeral())) {
        return false;
    }

    // -- Contando que o usuário faça parte do grupo PFL_GESTAO_ORCAMENTARIA,
    // -- devemos verificar se todos os PTRES são pertencentes à mesma UG dele.
    $listaPTRES = array();
    foreach ($PTRESParaInsercao as $ptres => $_) {
        $listaPTRES[] = $ptres;
    }
    $query = <<<DML
SELECT usr.unicod
  FROM planacomorc.usuarioresponsabilidade usr
  WHERE usr.usucpf = '%s'
    AND usr.pflcod = %d
    AND usr.rpustatus = 'A'
    AND EXISTS(SELECT 1
                 FROM monitora.ptres ptr
                 WHERE ptr.ptres IN (%s)
                   AND ptr.unicod = usr.unicod)
DML;
    // -- Existe alguma UO do PTRES que não esteja na lista de UOs do usuário?
    $query = <<<DML
SELECT ptr.unicod
  FROM monitora.ptres ptr
  WHERE ptr.ptres IN (%s)
EXCEPT
SELECT usr.unicod
  FROM planacomorc.usuarioresponsabilidade usr
  WHERE usr.usucpf = '%s'
    AND usr.pflcod = %d
    AND usr.rpustatus = 'A'
DML;
    $stmt = sprintf(
            $query, "'" . implode("', '", $listaPTRES) . "'", $_SESSION['usucpf'], PFL_GESTAO_ORCAMENTARIA
    );

    $uos = (bool) $db->carregar($stmt);
    // -- Verificando a lista de UOs dos PTRESs EXCLUÍNDO as UOs do usuário
    if ($uos) { // -- Se restou alguma UO, ela não está associada ao Gestor, então não deve ser uma transação direta
        return false;
    } else { // -- Se não restou nenhuma UO, então pode ser criada uma transação direta
        return true;
    }
}

function solicitacaoPIePTRES($pliid, $pliNovosPTRES, $pliPTRESAssociados) {
    global $db;
    $sql = <<<DML
INSERT INTO planacomorc.solicitacaopidotacao(scpid, ptrid, spdvalorsolicitado)
  VALUES(%d, %d, %f)
DML;
    // -- Inserindo dotações selecionadas agora
    if ($pliNovosPTRES) {
        foreach ($pliNovosPTRES as $valor) {
            $stmt = sprintf($sql, $pliid, key($valor), formataFloat(current($valor)));
            $db->executar($stmt);
        }
    }

    // -- Inserindo dotações selecionadas previamente
    if ($pliPTRESAssociados) {
        foreach ($pliPTRESAssociados as $PTRES => $valor) {
            $stmt = sprintf(
                    $sql, $pliid, $PTRES, str_replace(array('.', ','), array('', '.'), $valor)
            );
            $db->executar($stmt);
        }
    }
}

function concluirSolicitacaoOrigem($scpid) {
    global $db;

    $sql = <<<DML
UPDATE planacomorc.solicitacaocriacaopi
  SET scpprocessado = TRUE
  WHERE scpid = %d
DML;
    $stmt = sprintf($sql, $scpid);
    $db->executar($stmt);
}

function buscarPTRESdaSolicitacao($scpid, $sbaid) {
    global $db;

    $sql = <<<DML
SELECT dtl.ptrid,
       dtl.ptres,
       trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' ||aca.acadsc) AS descricao,
       COALESCE(SUM(dtl.ptrdotacao), 0.00) AS dotacaoatual,
       COALESCE(SUM(dt.valor), 0.00) AS det_subacao,
       -- dotacaoinicial - det_subacao
       (COALESCE(SUM(dtl.ptrdotacao), 0.00) - COALESCE(SUM(dt.valor), 0.00)) AS nao_det_subacao,
       COALESCE(SUM(dt2.valorpi), 0.00) AS det_pi,
       -- det_subacao - det_pi
       (COALESCE(SUM(dt.valor), 0.00) - COALESCE(SUM(dt2.valorpi), 0.00)) AS nao_det_pi,
       COALESCE((pemp.total), 0.00) AS empenhado,
       COALESCE(SUM(dtl.ptrdotacao), 0.00) - COALESCE(pemp.total, 0.00) AS nao_empenhado,
       sdp.spdvalorsolicitado AS pipvalor
  FROM planacomorc.solicitacaocriacaopi scp
    LEFT JOIN planacomorc.solicitacaopidotacao sdp USING(scpid)
    LEFT JOIN monitora.ptres dtl USING(ptrid)
    left join monitora.acao aca USING(acaid)
    LEFT JOIN monitora.pi_subacaodotacao sd USING(ptrid)
    LEFT JOIN monitora.pi_subacao a ON a.sbaid = sd.sbaid
    LEFT JOIN (SELECT sbaid,
                      ptrid,
                      SUM(sadvalor) AS valor
                 FROM monitora.pi_subacaodotacao
                 GROUP BY sbaid,
                          ptrid) dt ON dtl.ptrid = dt.ptrid AND dt.sbaid = a.sbaid
    LEFT JOIN (SELECT sbaid,
                      ptrid,
                      SUM(dtl.valorpi) AS valorpi
                 FROM monitora.v_pi_detalhepiptres dtl
                 GROUP BY sbaid,
                          dtl.ptrid) dt2 ON dtl.ptrid = dt2.ptrid AND dt2.sbaid = sd.sbaid
    LEFT JOIN siafi.ptrempenho pemp
      ON (pemp.ptres = dtl.ptres AND pemp.exercicio = '{$_SESSION['exercicio']}')
  WHERE scp.scpid = %d
    AND aca.prgano = '%s'
    AND aca.acasnrap = FALSE
    AND scp.sbaid = a.sbaid
  GROUP BY dtl.ptrid,
           dtl.ptres,
           aca.prgcod,
           aca.acacod,
           aca.unicod,
           aca.loccod,
           aca.acadsc,
           sdp.spdvalorsolicitado,
           pemp.total
DML;
    $stmt = sprintf($sql, $scpid, $_SESSION['exercicio']);
    $result = $db->carregar($stmt);
    // -- formatando valores monetarios
    $result = is_array($result) ? $result : Array();
    foreach ($result as $key => $_) {
        $result[$key]['dotacaoatual'] = mascaraMoeda($result[$key]['dotacaoatual']);
        $result[$key]['det_subacao'] = mascaraMoeda($result[$key]['det_subacao']);
        $result[$key]['nao_det_subacao'] = mascaraMoeda($result[$key]['nao_det_subacao']);
        $result[$key]['det_pi'] = mascaraMoeda($result[$key]['det_pi']);
        $result[$key]['nao_det_pi'] = mascaraMoeda($result[$key]['nao_det_pi']);
        $result[$key]['empenhado'] = mascaraMoeda($result[$key]['empenhado']);
        $result[$key]['nao_empenhado'] = mascaraMoeda($result[$key]['nao_empenhado']);
        $result[$key]['pipvalor_'] = $result[$key]['pipvalor']; // -- Não formatado - para soma na interface
        $result[$key]['pipvalor'] = mascaraMoeda($result[$key]['pipvalor']);
    }

    return $result;
}

function formataFloat($num) {
    return str_replace(array('.', ','), array('', '.'), $num);
}

function carregarTransacao($scpid) {
    global $db;

    $sql = <<<DML
SELECT scp.scpid,
       scp.mdeid,
       mde.mdecod,
       scp.eqdid,
       eqd.eqdcod,
       scp.neeid,
       nee.neecod,
       scp.capid,
       cap.capcod,
       scp.sbaid,
       TRIM(scp.scptitulo) AS plititulo,
       scp.scpdsc AS plidsc,
       TRIM(scp.scplivre) AS plilivre,
       scp.unicod,
       scp.ungcod,
       sba.sbasigla || ' - ' AS sbasigla,
       sba.sbacod,
       scp.scpdotacaosubacao AS dotacao,
       scp.scpdetalhadopisubacao AS detalhado_pi,
       scp.scpempenhadosubacao AS empenhado,
       scp.tipotransacao
  FROM planacomorc.solicitacaocriacaopi scp
    LEFT JOIN monitora.pi_subacao sba
      ON (scp.sbaid = sba.sbaid AND scp.scpano = sba.sbaano)
    LEFT JOIN monitora.pi_enquadramentodespesa eqd
      ON (scp.eqdid = eqd.eqdid AND scp.scpano = eqd.eqdano)
    LEFT JOIN monitora.pi_niveletapaensino nee
      ON (scp.neeid = nee.neeid AND scp.scpano = nee.neeano)
    LEFT JOIN monitora.pi_categoriaapropriacao cap
      ON (scp.capid = cap.capid AND scp.scpano = cap.capano)
    LEFT JOIN monitora.pi_modalidadeensino mde
      ON (scp.mdeid = mde.mdeid AND scp.scpano = mde.mdeano)
  WHERE scpid = %d
DML;
    $stmt = sprintf($sql, $scpid);
    $return = $db->pegaLinha($stmt);

    // -- Formatando dados monetarios
    $return['dotacao'] = number_format($return['dotacao'], 2, ',', '.');
    $return['detalhado_pi'] = number_format($return['detalhado_pi'], 2, ',', '.');
    $return['empenhado'] = number_format($return['empenhado'], 2, ',', '.');

    return $return;
}

function carregarPI($pliid) {
    global $db;

    $sql = <<<DML
        SELECT DISTINCT
            pli.pliid,
            pli.mdeid,
            mde.mdecod,
            pli.eqdid,
            eqd.eqdcod,
            pli.neeid,
            nee.neecod,
            pli.capid,
            cap.capcod,
            pli.sbaid,
            pli.plititulo,
            pli.plicodsubacao,
            pli.plicod,
            pli.plilivre,
            pli.plidsc,
            pli.unicod,
            pli.ungcod,
            pli.pliano,
            pli.plicadsiafi,
            pli.docid,
            to_char(pli.plidata, 'dd/mm/YYYY') as plidata,
            pc.*,
            CASE plisituacao
                WHEN 'A' THEN 'Aprovado'
                WHEN 'E' THEN 'Enviado para revisao'
                WHEN 'P' THEN 'Pendente'
                WHEN 'C' THEN 'Cadastrado no SIAFI'
                WHEN 'R' THEN 'Revisado'
                WHEN 'H' THEN 'Homologado'
                WHEN 'T' THEN '<span style="color:red">Cadastrado no SIAFI</span>'
            ELSE 'Tendencioso'
            END AS plisituacao,
            sba.sbaid,
            sba.sbasigla || ' - ' AS sbasigla,
            sba.sbacod,
            ben.benid,
            em.emenumero
        FROM monitora.pi_planointerno pli
            LEFT JOIN emendas.beneficiario ben ON(pli.pliid = ben.pliid)
            LEFT JOIN emendas.emenda em ON(ben.emeid = em.emeid)
            LEFT JOIN planacomorc.pi_complemento pc on pc.pliid = pli.pliid
            LEFT JOIN monitora.pi_subacao sba ON (pli.sbaid = sba.sbaid AND pli.pliano = sba.sbaano)
            LEFT JOIN monitora.pi_enquadramentodespesa eqd ON (pli.eqdid = eqd.eqdid AND pli.pliano = eqd.eqdano)
            LEFT JOIN monitora.pi_niveletapaensino nee ON (pli.neeid = nee.neeid AND pli.pliano = nee.neeano)
            LEFT JOIN monitora.pi_categoriaapropriacao cap ON (pli.capid = cap.capid AND pli.pliano = cap.capano)
            LEFT JOIN monitora.pi_modalidadeensino mde ON (pli.mdeid = mde.mdeid) --ON (pli.mdeid = mde.mdeid AND pli.pliano = mde.mdeano)
        WHERE
            pli.pliid = %d
DML;
    $stmt = sprintf($sql, $pliid);
//ver($stmt);
    return $db->pegaLinha($stmt);
}


function carregarPiComDetalhes(stdclass $filtros) {
    global $db;

    $sql = <<<DML
        SELECT
            pli.pliid,
            pli.mdeid,
            suo.unonome || '(' || suo.unosigla || ')' AS unidade,
            suodel.unosigla || ' - ' || suodel.suonome || '' AS sub_unidade,
            mde.mdecod,
            pli.eqdid,
            eqd.eqddsc,
            opp.oppcod || ' - ' || opp.oppnome AS objetivo,
            m.mppcod || ' - ' || m.mppnome AS meta,
            i.ippcod || ' - ' || i.ippnome AS iniciativa,
            mpn.mpncod || ' - ' || mpn.mpnnome AS meta_pnc,
            ipn.ipncod || ' - ' || ipn.ipndsc AS iniciativa_pnc,
            ptr.prgcod || ' - ' || ptr.prgdsc AS programa,
            ptr.acacod || ' - ' || ptr.acatitulo AS acao,
            ptr.loccod || ' - ' || ptr.locdsc AS localizador,
            ptr.plocod || ' - ' || ptr.plodsc AS po,
            ptr.ptres AS ptres,
            pprnome AS produto,
            pum.pumdescricao AS unidade_medida,
            cap.capdsc AS modalidade_pactuacao,
            esf.esfdsc AS tipo_localizacao,
            pli.pliemenda,
            eqd.eqdcod,
            pli.neeid,
            nee.neecod,
            pli.capid,
            cap.capcod,
            pli.sbaid,
            pli.plititulo,
            pli.plicodsubacao,
            pli.plicod,
            pli.plilivre,
            pli.plidsc,
            pli.unicod,
            pli.ungcod,
            pli.pliano,
            pli.plicadsiafi,
            pli.docid,
            to_char(pli.plidata, 'dd/mm/YYYY') as plidata,
            pc.*,
            CASE plisituacao
                WHEN 'A' THEN 'Aprovado'
                WHEN 'E' THEN 'Enviado para revisao'
                WHEN 'P' THEN 'Pendente'
                WHEN 'C' THEN 'Cadastrado no SIAFI'
                WHEN 'R' THEN 'Revisado'
                WHEN 'H' THEN 'Homologado'
                WHEN 'T' THEN '<span style="color:red">Cadastrado no SIAFI</span>'
            ELSE 'Tendencioso'
            END AS plisituacao
        FROM monitora.pi_planointerno pli
            LEFT JOIN monitora.pi_planointernoptres pip ON (pli.pliid = pip.pliid)
            LEFT JOIN monitora.vw_ptres ptr ON (pip.ptrid = ptr.ptrid)
            LEFT JOIN planacomorc.pi_complemento pc ON (pc.pliid = pli.pliid)
            LEFT JOIN monitora.pi_enquadramentodespesa eqd ON (pli.eqdid = eqd.eqdid AND pli.pliano = eqd.eqdano)
            LEFT JOIN monitora.pi_niveletapaensino nee ON (pli.neeid = nee.neeid AND pli.pliano = nee.neeano)
            LEFT JOIN monitora.pi_categoriaapropriacao cap ON (pli.capid = cap.capid AND pli.pliano = cap.capano)
            LEFT JOIN monitora.pi_modalidadeensino mde ON (pli.mdeid = mde.mdeid)
            LEFT JOIN public.vw_subunidadeorcamentaria suo ON (suo.suocod = pli.ungcod AND suo.prsano = pli.pliano)
            LEFT JOIN public.objetivoppa opp ON (pc.oppid = opp.oppid)
            LEFT JOIN public.metappa m ON (pc.mppid = m.mppid AND m.prsano = pli.pliano)
            LEFT JOIN public.iniciativappa i ON (pc.ippid = i.ippid AND i.prsano = pli.pliano)
            LEFT JOIN public.metapnc mpn ON (pc.mpnid = mpn.mpnid)
            LEFT JOIN public.indicadorpnc ipn ON (pc.ipnid = ipn.ipnid)
            LEFT JOIN monitora.pi_produto ppr ON (pc.pprid = ppr.pprid AND ppr.prsano = pli.pliano)
            LEFT JOIN monitora.pi_unidade_medida pum ON (pc.pumid = pum.pumid)
            LEFT JOIN territorios.esfera esf ON (pc.esfid = esf.esfid)
            LEFT JOIN planacomorc.pi_delegacao del ON (del.pliid = pli.pliid)
            LEFT JOIN public.vw_subunidadeorcamentaria suodel ON (suodel.suoid = del.suoid AND suodel.prsano = pli.pliano)
        WHERE
            pli.pliid = %d
DML;
    $stmt = sprintf($sql, $filtros->pliid);
//ver($stmt);
    return $db->pegaLinha($stmt);
}

function carregarEnquadramentoPI($pliid) {
    global $db;

    $sql = <<<DML
    SELECT
        mpneid as codigo
    FROM spo.planointernometapne
  WHERE pliid = %d
DML;
    $stmt = sprintf($sql, $pliid);
    $dados = $db->carregar($stmt);
    $new = array();
    if ($dados) {
        foreach ($dados as $key => $v) {
            $new[] = $v['codigo'];
        }
        $dados = $new;
    }

    return $dados;
}

/**
 * Busca nome da classe de acordo com o número da coluna do cronograma.
 * 
 * @param integer $numeroColuna
 * @return string
 */
function buscarClasseCronograma($numeroColuna, $cabecalho = FALSE){
    if($cabecalho === TRUE){
        $numeroColuna++;
    }
    $classTdCronograma = '';
    switch ($numeroColuna) {
        case Cronograma::K_FISICO:
            $classTdCronograma = 'td_cronograma_fisico';
        break;
        case Cronograma::K_ORCAMENTARIO:
            $classTdCronograma = 'td_cronograma_orcamentario';
        break;
        case Cronograma::K_FINANCEIRO:
            $classTdCronograma = 'td_cronograma_financeiro';
        break;
        default:
            $classTdCronograma = 'td_cronograma_financeiro';
        break;
    }
    return $classTdCronograma;
}

/**
 * Busca enquadramento por ano do exercício e código.
 * 
 * @global cls_banco $db
 * @param integer $exercicio
 * @param string $codigo
 * @return integer
 */
function buscarCodigoEnquadramento($exercicio, $codigo) {
    global $db;

    $sql = "
        SELECT
            eqdid AS codigoFinalistico
        FROM monitora.pi_enquadramentodespesa
        WHERE
            eqdstatus = 'A'
            AND eqdano = '". (int)$exercicio. "'
            AND eqdcod = '". pg_escape_string($codigo). "'
    ";
    
    $codigoFinalistico = $db->pegaUm($sql);
    return $codigoFinalistico;
}

function inativarPI($dados) {
    global $db;
    $sql = <<<DML
UPDATE monitora.pi_planointerno
  SET plistatus = 'I'
  WHERE pliid = %d
DML;
    $stmt = sprintf($sql, $dados['pliid']);
    $db->executar($stmt);
    return $db->commit();
}

/**
 * Verifica se um campo da requisição foi definido e se tem valor.
 * @param string $campo Nome do campo para verificação no $_REQUEST.
 * @return bool
 */
function validaRequisicao($campo) {
    return (isset($_REQUEST[$campo]) && !empty($_REQUEST[$campo]));
}

/**
 * Constroi a query que preenche o filtro de UO de acordo com a sessão do usuário.
 * Funciona para os dois perfis (GESTAO_ORC e GABIN), retornando a instrução SQL.
 * @param bool $semFiltroPerfil Indica se o filtro de perfil deve ser aplicado.
 * @return string
 */
function consultaUOs($semFiltroPerfil = false) {
    if (!$semFiltroPerfil) {
        if (in_array(PFL_GABINETE, pegaPerfilGeral($_SESSION['usucpf']))) {
            $filtroPerfilUO = <<<SQL
 AND EXISTS (SELECT 1
               FROM planacomorc.usuarioresponsabilidade urp
                 INNER JOIN public.unidadegestora ung USING(ungcod)
               WHERE ung.unicod = uni.unicod
                 AND urp.pflcod = %d
                 AND urp.usucpf = '%s'
                 AND urp.rpustatus = 'A')
SQL;
            $filtroPerfilUO = sprintf($filtroPerfilUO, PFL_GABINETE, $_SESSION['usucpf']);
        } elseif (in_array(PFL_GESTAO_ORCAMENTARIA, pegaPerfilGeral())) {
            $filtroPerfilUO = <<<SQL
  AND EXISTS (SELECT 1
                FROM planacomorc.usuarioresponsabilidade urp
                WHERE uni.unicod = urp.unicod
                  AND urp.pflcod = %d
                  AND urp.usucpf = '%s'
                  AND urp.rpustatus = 'A')
SQL;
            $filtroPerfilUO = sprintf($filtroPerfilUO, PFL_GESTAO_ORCAMENTARIA, $_SESSION['usucpf']);
        } elseif (in_array(PFL_NAO_OBRIGATORIAS, pegaPerfilGeral())) {
            $filtroPerfilUO .= <<<SQL
  AND uni.unicod NOT IN(%s)
SQL;
            $filtroPerfilUO = sprintf($filtroPerfilUO, UNIDADES_OBRIGATORIAS);
        }
    }


    $sql = "
SELECT uni.unicod AS codigo,
       uni.unicod || ' - ' || uni.unidsc AS descricao
  FROM public.unidade uni
  WHERE uni.unistatus = 'A'
    AND orgcod = '". CODIGO_ORGAO_SISTEMA. "'
     {$filtroPerfilUO}
  ORDER BY uni.unicod
";
    return sprintf($sql, UNIDADES_OBRIGATORIAS);
}

function vincular($dados) {
    global $db;

    $pliid = $dados['pliid'];
    $plisituacao = $dados['situacao'];

    // -- Atualizando a situação do PIs
    $sql = <<<DML
UPDATE monitora.pi_planointerno
  SET plisituacao = '%s'
  WHERE pliid = %d
DML;
    $stmt = sprintf($sql, $plisituacao, $pliid);
    $db->executar($stmt);

    // -- Gravando o histórico da atualização
    $sql = <<<DML
INSERT INTO monitora.pi_planointernohistorico(
    pliid,
    plicod,
    pihobs,
    pihdata,
    usucpf,
    pihsituacao,
    plicodorigem)
SELECT pli.pliid, pli.plicod, NULL, NOW(), '%s', '%s', pli.plicod
  FROM monitora.pi_planointerno pli
  WHERE pli.pliid = %d
DML;
    $stmt = sprintf($sql, $_SESSION['usucpf'], $plisituacao, $pliid);

    $db->executar($stmt);

    if (!$db->commit()) {
        $db->rollback();
        return;
    }

    // -- Notificando os gestores
    if ('A' == $plisituacao) {
        enviaEmailPI(
                array(
                    'tipoEvento' => 'aprCadPi',
                    'pliid' => $pliid
                )
        );
    }

    echo 'Ok!';
}

function piAtualizadoSiafi($dados) {
    global $db;
    $sql = <<<DML
UPDATE monitora.pi_planointerno
  SET plisituacao = 'C'
  WHERE pliid = %d
DML;
    $stmt = sprintf($sql, (int) $dados['pliid']);
    $db->executar($stmt);
    return $db->commit();
}

/**
 * Lista de PIs das UOS para MANTER
 */
function listaPisUoManter($dados) {
    global $db;
    $perfis = pegaPerfilGeral();

    $obrigatorias = UNIDADES_OBRIGATORIAS;
    /* Filtros */
    $where = '';
    $where .= $dados['pi'] ? " AND pli.plicod ilike '%" . $_REQUEST['pi'] . "%' " : "";
    $where .= $dados['unicod'] ? " AND (pli.unicod = '{$dados['unicod']}' OR pli.ungcod IN (select ungcod from public.unidadegestora where unicod = '{$dados['unicod']}') )" : "";
    $where .= $_REQUEST["ptres"][0] ? " AND ptres.ptrid in ('" . implode("','", $_REQUEST["ptres"]) . "') " : "";
    $titulodescricaoTmp = removeAcentos(str_replace("-", "", $_REQUEST['titulodescricao']));
    $where .= $dados['titulodescricao'] ? " AND (UPPER(public.removeacento(pli.plititulo)) ilike '%" . $titulodescricaoTmp . "%' OR UPPER(public.removeacento(pli.plidsc)) ilike '%" . $titulodescricaoTmp . "%')" : "";

    /* Filtrando apenas os PIs das UOs */
    if (in_array(PFL_GESTAO_ORCAMENTARIA_IFS, $perfis)) {
        $sqlUO = <<<DML
EXISTS (SELECT 1
         FROM planacomorc.usuarioresponsabilidade rpu
         WHERE rpu.usucpf = '%s'
           AND rpu.pflcod = %d
           AND rpu.rpustatus = 'A'
           AND rpu.unicod  = uni.unicod)
DML;
        $whereUO = sprintf($sqlUO, $_SESSION['usucpf'], PFL_GESTAO_ORCAMENTARIA_IFS);
        $whereUO = " AND {$whereUO}";
        $where .= $whereUO;
    }

    /* Apenas as UOs não Obrigatórias */
    $where .= " AND uni.unicod NOT IN ($obrigatorias) ";

    /* Caso seja selecionado apenas 1 PTRES, mostrar as colunas dos dados apenas para aquele PTRES */
    $sqlAdicional = "";
    if ($_REQUEST["ptres"][0] && count($_REQUEST["ptres"]) == 1) {
        $dados = $db->carregar("SELECT ptres FROM monitora.ptres WHERE ptrid = {$_REQUEST["ptres"][0]}");
        $ptres = $dados[0]['ptres'];
        #ver($_REQUEST["ptres"][0],d);
        $sqlAdicional.=", COALESCE((SELECT
                                        pip.pipvalor
                                    FROM
                                        monitora.pi_planointernoptres pip
                                    LEFT JOIN
                                        monitora.ptres
                                    ON
                                        pip.ptrid = ptres.ptrid
                                    WHERE
                                        ptres.ptrano = '{$_SESSION['exercicio']}'
                                    AND pip.pliid= gmb.pliid
                                    AND ptres.ptrid={$_REQUEST["ptres"][0]}),0.00) as dotacao_pip_ptres
                        ,        COALESCE((SELECT
                                    total
                                FROM
                                    siafi.pliptrempenho ppe
                                WHERE
                                    plicod = gmb.codigo
                                    AND ppe.exercicio = '{$_SESSION['exercicio']}'
                                AND ptres = '{$ptres}'),0.00) as empenhado_pi_ptres
                        ,COALESCE((SELECT
                                        pip.pipvalor
                                    FROM
                                        monitora.pi_planointernoptres pip
                                    LEFT JOIN
                                        monitora.ptres
                                    ON
                                        pip.ptrid = ptres.ptrid
                                    WHERE
                                        ptres.ptrano = '{$_SESSION['exercicio']}'
                                    AND pip.pliid= gmb.pliid
                                    AND ptres.ptrid={$_REQUEST["ptres"][0]}),0.00) -
                                COALESCE((SELECT
                                    total
                                FROM
                                    siafi.pliptrempenho ppe
                                WHERE
                                    plicod = gmb.codigo
                                    AND ppe.exercicio = '{$_SESSION['exercicio']}'
                                AND ptres = '{$ptres}'),0.00) AS nao_empenhado_tpi_ptres";
    }
    /* Cabeçalho da Consulta */

    $acoes = " '-' ";
    $acoes = <<<SQL
        gmb.pliid
SQL;

    $params['SELECT'] = <<<SQL
SELECT
    {$acoes}
    AS acoes,

        gmb.codigo ||' ',
        CASE WHEN trim(gmb.titulo) is not null THEN
            gmb.titulo  || '<input type=\"hidden\" id=\"plititulo[' || gmb.pliid || ']\" value=\"' || gmb.codigo || ' - ' || gmb.titulo || '\">'
        ELSE
            'Não Preenchido <input type=\"hidden\" id=\"plititulo[' || gmb.pliid || ']\" value=\" ' || gmb.codigo ||' - Não Preenchido\"/>'
        END as titulo,
        COALESCE(uni.unidsc, ung.ungabrev) as unidsc,
        CASE WHEN obrid IS NULL THEN '-' ELSE 'SIM' END as obras,
        CASE WHEN pli.plicadsiafi = 't' THEN 'Sim' ELSE 'Não' END as cadastroSIAF,
        COALESCE((SELECT
                                        SUM(pip.pipvalor)
                                    FROM
                                        monitora.pi_planointernoptres pip
                                    LEFT JOIN
                                        monitora.ptres
                                    ON
                                        pip.ptrid = ptres.ptrid
                                    WHERE
                                        ptres.ptrano = '{$_SESSION['exercicio']}'
                                    AND pip.pliid= gmb.pliid
                                    ),0.00) as dotacao_total,
        empenhado AS empenhado_total,
        COALESCE((SELECT
                                        SUM(pip.pipvalor)
                                    FROM
                                        monitora.pi_planointernoptres pip
                                    LEFT JOIN
                                        monitora.ptres
                                    ON
                                        pip.ptrid = ptres.ptrid
                                    WHERE
                                        ptres.ptrano = '{$_SESSION['exercicio']}'
                                    AND pip.pliid= gmb.pliid
                                    ),0.00) - empenhado AS nao_empenhado_total
        {$sqlAdicional}
SQL;
    $params['where'] = " $where ";
    $sql = retornaConsultaPI($params);

    /* Busca alternativa para recuperar os PIs que possuem Empenho mas não estão cadastrados no SIMEC
     * POR UO */
    if ($dados['apenasEmpenhadosUo'] == 'true') {
        $where = $dados['unicod'] ? " AND sld.unicod = '{$dados['unicod']}' " : "";
        #ver($where,d);
        $sql = <<<SQL
SELECT
    '-' as acao,
    plicod ,
    'Não Informado.' as titulo,
    unicod || ' - ' || uni.unidsc as unidsc,
    'Não Informado.' as obras,
    0 as orcamento,
    total as empenhado,
    0 as nao_empenhado
FROM
    dblink
    (
    'dbname= hostaddr= user= password= port='
    ,
    '
    SELECT
    sld.plicod,
    sld.unicod,
    SUM(
    CASE
    WHEN sld.sldcontacontabil IN (''292130100'',
    ''292130201'',
    ''292130202'',
    ''292130203'',
    ''292130301'')
    THEN
    CASE
    WHEN sld.ungcod=''154004''
    THEN (sld.sldvalor)*2.2088
    ELSE (sld.sldvalor)
    END
    ELSE 0
    END ) AS total
    FROM
    dw.saldo{$_SESSION['exercicio']} sld
    WHERE
    sld.sldcontacontabil IN (''292130100'',
    ''292130201'',
    ''292130202'',
    ''292130203'',
    ''292130301'')
    AND plicod IS NOT NULL
    AND plicod <> ''''
    AND LENGTH(sld.plicod) = 11
    AND sld.unicod NOT IN (''26101'', ''26298'', ''26291'', ''26443'', ''26290'', ''74902'')
    AND  SUBSTR(sld.unicod , 1, 2)  = ''26''
    $where
    GROUP BY
    sld.plicod, sld.unicod
    ORDER BY   sld.unicod  ;
    '
        ) AS pisiafi ( plicod VARCHAR(15), unicod VARCHAR(5), total NUMERIC(15,2) )
   left join public.unidade uni USING(unicod)
   WHERE
        plicod NOT IN
        (
            SELECT
                plicod
            FROM
                monitora.pi_planointerno pli
            WHERE
                pli.pliano = '{$_SESSION['exercicio']}')

SQL;
    }

    $cabecalho = array("Ação",
        "Código",
        "Título",
        "Unidade",
        "Obras",
        "Cadastrado SIAF",
        "Orçamento Total do PI (R$)",
        "Empenhado Total do PI (R$)",
        "Não Empenhado Total do PI (R$)");
    /* Caso seja selecionado apenas 1 PTRES, mostrar as colunas dos dados apenas para aquele PTRES */
    if ($_REQUEST["ptres"][0] && count($_REQUEST["ptres"]) == 1) {
        array_push($cabecalho, "Dotação para este PTRES (R$)", "Empenhado neste PTRES (R$)", "Não Empenhado neste PTRES");
    }
    $db->monta_lista_ordenaGROUPBY($sql, $cabecalho, 200, 5, 'S', 'center');
}

function botaoEnviarRevisao() {
    ?>
    <button type="button" class="btn btn-primary" style="font-weight:bold" onclick="modalRevisao()">&nbsp;&nbsp;Revisão&nbsp;</button>
    <br style="clear:both" />
    <br style="clear:both" />
    <button type="button" class="btn btn-primary" style="font-weight:bold" onclick="trocarSituacao('C')">Atualizado<br />no SIAFI</button>
    <?php
}

function botaoTornarPendente() {
    ?>
    <button type="button" class="btn btn-primary" style="font-weight:bold" onclick="trocarSituacao('P')">Tornar<br />Pendente</button>
    <?php
}

/**
 * Exibe o bot?o de aprovar de acordo com as permiss?es do usu?rio.
 *
 * @param array $unicodPI UO de quem solicitou o PI
 * @param array $perfis
 * @param array $unicodsResponsabilidade
 * @param type $pflCode
 * @param type $arUOdoPTRES
 */
function botaoAprovar(array $unicodPI, array $perfis, array $unicodsResponsabilidade, $pflCode, $arUOdoPTRES) {
    $podeAprovar = false;
    if (1 == $_SESSION ['superuser']) {
        $podeAprovar = true;
    } elseif (in_array($pflCode, $perfis)) { // -- Verifica se ? o perfil que pode aprovar PIs
        if (1 == count($unicodPI) && in_array(array_pop($unicodPI), $unicodsResponsabilidade)) {
            // -- Se a ?NICA UO do PI estiver dentro do conjunto UO-RESPONSABILIDADE, deixa aprovar o PI
            $podeAprovar = true;
        }
    }

    if ($podeAprovar) {
        ?>
        <br style="clear:both" />
        <br style="clear:both" />
        <button type="button" class="btn btn-primary" style="font-weight:bold" onclick="trocarSituacao('A')">&nbsp;&nbsp;Aprovar&nbsp;&nbsp;</button>
        <?php
    }
}

function botaoCadastrarSIAFI($pliid) {
    ?>
    <button type="button" class="btn btn-primary" style="font-weight:bold" onclick="trocarSituacao('C')">Cadastrar<br />no SIAFI</button>
    <?php
}

function botaoAtualizarSIAFI($pliid) {
    ?>
    <button type="button" class="btn btn-primary" style="color:#FFBF00;font-weight:bold" onclick="trocarSituacao('T')">Cadastrar<br />no SIAFI</button>
    <br style="clear:both" />
    <br style="clear:both" />
    <button type="button" class="btn btn-primary" style="font-weight:bold" onclick="trocarSituacao('C')">Atualizado<br />no SIAFI</button>
    <?php
}

function pegarDocidPi($pliid, $tipoFluxo)
{
    global $db;
    $sql = "select docid from monitora.pi_planointerno where pliid = {$pliid}";
    $docid = $db->pegaUm($sql);
    if (!$docid) {
        $docid = wf_cadastrarDocumento($tipoFluxo, "PI {$pliid}");

        $db->executar("UPDATE monitora.pi_planointerno SET docid = $docid where pliid = {$pliid}");
        $db->commit();
    }

    return $docid;
}

function posAcaoAprovarPi($pliid)
{
    global $db;

    $dadosPI = $db->pegaLinha("select * from monitora.pi_planointerno where pliid = $pliid");

    if(!$dadosPI['plicod']){
        $codigos = gerarCodigosPi($pliid);
        $sql = "update monitora.pi_planointerno set 
                    plicod = '{$codigos['plicod']}', 
                    plilivre = '{$codigos['plilivre']}', 
                    plicodsubacao = '{$codigos['plicodsubacao']}'
                where pliid = $pliid";

        $db->executar($sql);
        $db->commit();
    }

    # Busca docid do Beneficiario caso seja PI originado de beneficiario de Emenda
    $beneficiario = buscarBeneficiarioPi($pliid);
    if($beneficiario){
        # Altera a situação do beneficiario pra PI Aprovado.
        wf_alterarEstado($beneficiario['docid'], AED_EMENDAS_APROVAR_PI, 'PI Aprovado no Planejamento', array('benid' => $beneficiario['benid']));
    }
    
    enviarEmailAprovado($pliid);

    return true;
}

/**
 * Busca beneficiario que gerou o Plano Interno(PI)
 * 
 * @global cls_banco $db
 * @param integer $pliid
 * @return array/boolean
 */
function buscarBeneficiarioPi($pliid) {
    global $db;
    
    $sql = "SELECT benid, docid FROM emendas.beneficiario WHERE pliid = ". (int)$pliid;
    $beneficiario = $db->pegaLinha($sql);
    
    return $beneficiario;
}

function posAcaoCancelarPi($pliid){
    global $db;

    $sql = "
        UPDATE monitora.pi_planointerno SET
            plistatus = 'I'
        WHERE
            pliid = ". (int)$pliid;

    $db->executar($sql);
    $db->commit();

    return true;
}

function posAcaoReabrirPi($pliid){
    global $db;

    $sql = "
        UPDATE monitora.pi_planointerno SET
            plistatus = 'A'
        WHERE
            pliid = ". (int)$pliid;

    $db->executar($sql);
    $db->commit();

    return true;
}

function gerarCodigosPi($pliid)
{
    global $db;

    /********************************************************************************************************
    *        Regra para formação do Código do PI está em /docs/planacomorc/Estrutura_Codigo_PI.xlsx         *
    ********************************************************************************************************/

    $sql = "
        SELECT
            SUBSTR(pliano, 3, 2) ||
            CASE WHEN pi.pliemenda = TRUE THEN
                'E'
            ELSE
                eqd.eqdcod
            END ||
            LPAD(pi.pliid::TEXT, 5, '0') ||
            suo.suocodigopi ||
            CASE WHEN pic.picted = TRUE THEN
                'T'
            ELSE
                cap.capcod
            END AS plicod,
            LPAD(pi.pliid::TEXT, 4, '0') plicodsubacao,
            SUBSTR(pliano, 3, 2) plilivre
        FROM
            monitora.pi_planointerno pi
            JOIN planacomorc.pi_complemento pic ON pic.pliid = pi.pliid
            JOIN monitora.pi_enquadramentodespesa eqd ON eqd.eqdid = pi.eqdid
            JOIN public.vw_subunidadeorcamentaria suo ON suo.suocod = pi.ungcod
                AND suo.unocod = pi.unicod
                AND suo.prsano = pi.pliano
            LEFT JOIN monitora.pi_categoriaapropriacao cap ON cap.capid = pi.capid
        WHERE
             pi.pliid = ". (int)$pliid;
//ver($sql, d);
    return $db->pegaLinha($sql);
}

function verificarPactuacaoConvenio($capid)
{
    if(!$capid){
        return false;
    }

    global $db;

    $sql = "SELECT count(*) FROM monitora.pi_categoriaapropriacao
            WHERE capano = '{$_SESSION['exercicio']}'
            AND capstatus = 'A'
            AND capid = $capid
            AND capsiconv = 't'";
    
    return $db->pegaUm($sql);


}

/**
 * Verifica se o PI é do tipo FNC ou não.
 * 
 * @global cls_banco $db
 * @param integer $pliid
 * @return boolean Retorna TRUE caso o PI seja do tipo FNC
 */
function verificarPiFnc($pliid){
    global $db;
    $sql = "
        SELECT
            suo.unofundo
        FROM monitora.pi_planointerno pli
            JOIN public.vw_subunidadeorcamentaria suo ON(
                suo.suostatus = 'A'
                AND pli.unicod = suo.unocod
                AND pli.ungcod = suo.suocod
                AND suo.prsano = pli.pliano
            )
        WHERE
            pli.pliid = ". (int)$pliid;
    $unofundo = $db->pegaUm($sql);
//ver($unofundo, d);
    return $unofundo == 't'? TRUE: FALSE;
}

/**
 * Altera código do PI
 *
 * @param array $request
 * @return array
 */
function alterarCodigoPi($request){

    $resposta = array();
    # Mudando o Código de PI novo para maiuscula
    $request['plicod'] = trim(strtoupper($request['plicod']));
    $mPlanoInterno = new Pi_PlanoInterno($request['pliid']);
    $pliid = $mPlanoInterno->consultaCodigoPi($request['plicod']);
    if($pliid) {
        $resposta['title'] = 'Atenção';
        $resposta['text'] = 'O código de PI '. $request['plicod'] .' já está sendo usado!';
        $resposta['type'] = 'warning';
        $resposta['plicod'] = NULL;
    } else {
        $mPlanoInterno->alterarCodigoPi($request['plicod']);
        $resposta['title'] = NULL;
        $resposta['text'] = 'O código de PI foi alterado com sucesso!';
        $resposta['type'] = 'success';
        $resposta['plicod'] = $mPlanoInterno->plicod;
    }

    return $resposta;
}
