<?php
/**
 * Funções de cadastramento e remanejamento de PI.
 * $Id: _funcoesremanejavalorpi.php 98005 2015-05-29 20:02:53Z werteralmeida $
 */

/**
 *
 * @global type $db
 * @param type $dados
 */
function listaPtresSubacao($dados) {
    global $db;
    /* Unidades Obrigatórias (AD, CAPES, INEP, FNDE, FIES, SUP.MEC, EBSERH */
    $obrigatorias = "'26101', '26291', '26290', '26298', '74902', '73107', '26443'";
    $sql = <<<DML
SELECT '<img src="../imagens/send.png" style="cursor:pointer" title="Remanejar valores" onclick="remanejar('||ptr.ptrid||' , '||sd.sbaid||')">' AS remanejar,
       ptr.ptres,
       trim(aca.prgcod||'.'||aca.acacod||'.'||aca.unicod||'.'||aca.loccod||' - '||aca.acadsc) AS descricao,
       uni.unidsc,
       SUM(DISTINCT ptr.ptrdotacao) AS dotacao,
       COALESCE(CAST(SUM(DISTINCT dt.valor) AS varchar), '0.00') AS orcado_subacao,
       COALESCE(CAST(SUM(dt2.valorpi) AS varchar), '0.00') AS detalhado_pi,
       COALESCE(SUM(ppi.total), 0.00) AS empenhado,
       (SUM(ptr.ptrdotacao) - COALESCE(SUM(dt.valor), 0.00)) AS nao_orcado_subacao
  FROM planejamento.acao aca
    INNER JOIN monitora.ptres ptr ON aca.acaid = ptr.acaid
    INNER JOIN monitora.pi_subacaodotacao sd ON ptr.ptrid = sd.ptrid
    INNER JOIN public.unidade uni ON uni.unicod = ptr.unicod
    LEFT JOIN (SELECT ptrid,
				      SUM(sadvalor) AS valor
				 FROM monitora.pi_subacaodotacao
                 GROUP BY ptrid) dt ON ptr.ptrid = dt.ptrid
    LEFT JOIN (SELECT sbaid,
                      ptrid,
                      SUM(dtl.valorpi) AS valorpi
                 FROM monitora.v_pi_detalhepiptres dtl
                 GROUP BY sbaid, dtl.ptrid) dt2 ON ptr.ptrid = dt2.ptrid AND dt2.sbaid = sd.sbaid
    LEFT JOIN monitora.pi_subacao sba ON sba.sbaid = sd.sbaid
    LEFT JOIN siafi.pliptrempenho ppi ON ppi.ptres = ptr.ptres and SUBSTR(ppi.plicod, 2, 4) = sba.sbacod AND ppi.exercicio = '{$_SESSION['exercicio']}'
  WHERE aca.prgano = '{$_SESSION['exercicio']}'
    AND ptrano = '{$_SESSION['exercicio']}'
    AND aca.acasnrap = false
    AND sd.sbaid = {$dados['sbaid']}
    AND ptr.unicod IN({$obrigatorias})
  GROUP BY ptr.ptrid,
           ptr.ptres,
           aca.prgcod,
           aca.acacod,
           aca.unicod,
           aca.loccod,
           aca.acadsc,
           uni.unidsc,
           sd.sbaid
	ORDER BY 3
DML;
//    ver($sql,d);
    $cabecalho = array(
        "",
        "PTRES",
        "Ação",
        "Unidade Orçamentária",
        "Dotação do PTRES (R$)",
        "Orçado em Subação (R$)",
        "Detalhado em PI (R$)",
        "Empenhado (R$)",
        "Não orçado em Subação (R$)"
    );
    echo "<tr><td>&nbsp;</td><td colspan=5 align=left>";
    $db->monta_lista($sql, $cabecalho, 300, 20, 'S', '', 'S');
    echo "</td></tr>";
}

function remanejamentoDePI($dados) {
    global $db;
    if ($rmpid = criaTransacao($dados)) {
        // -- Decrementa pi
        criaTransacaoPI($rmpid, $dados['rem_pi'], '-');
        // -- Incrementa pi
        criaTransacaoPI($rmpid, $dados['rem_adc_pi'], '+');
    }

    // -- Aplicando alterações de valores do PI
    if ('E' == $dados['tipotransacao']) {
        incrementaPI($dados['ptrid'], $dados['rem_adc_pi']);
        decrementaPI($dados['ptrid'], $dados['rem_pi']);
    }

    // -- Concluíndo a transação de origem
    if (!empty($dados['rmpidorigem'])) {
        concluiTransacaoOrigem($dados['rmpidorigem']);
    }

    if ($db->commit()) {
        if ('S' == $dados['tipotransacao']) {
            $tipoEvento = 'solRemPI';
        } elseif ('H' == $dados['tipotransacao']) {
            $tipoEvento = 'homRemPI';
        } elseif ('E' == $dados['tipotransacao']) {
            $tipoEvento = 'efeRemPI';
        }

        enviaEmailPI(
            array(
                'tipoEvento' => $tipoEvento,
                'rmpid' => $rmpid
            )
        );

        return true;
    }
    $db->rollback();
    return false;
}

function criaTransacao($dados) {
    global $db;
    $sql = <<<DML
INSERT INTO planacomorc.remanejamentopi(
    tipotransacao,
    ptrid,
    funcprogramatica,
    ptrdotacao,
    ptrempenhado,
    ptrsaldo,
    sbaid,
    sbadotacao,
    sbaempenhado,
    sbasaldo,
    usucpf,
    rmpsaldosubtraido,
    rmpsaldoadicionado,
    rmpidorigem,
    dscalteracao,
    dscjustificativa,
    dscexecucao,
    ptrsaldodetalhadopi
) VALUES ('%s', %d, '%s', %f, %f, %f, %d, %f, %f, %f, '%s', %f, %f, %s, '%s', '%s', '%s', %f)
RETURNING rmpid
DML;
    $stmt = sprintf(
        $sql,
        $dados['tipotransacao'],
        $dados['ptrid'],
        $dados['funcprogramatica'],
        $dados['ptrdotacao'],
        $dados['ptrempenhado'],
        $dados['ptrsaldo'],
        $dados['sbaid'],
        $dados['sbadotacao'],
        $dados['sbaempenhado'],
        $dados['sbasaldo'],
        $_SESSION['usucpf'],
        $dados['rmpsaldosubtraido'],
        $dados['rmpsaldoadicionado'],
        empty($dados['rmpidorigem']) ? 'null' : $dados['rmpidorigem'],
        $dados['dscalteracao'],
        $dados['dscjustificativa'],
        $dados['dscexecucao'],
        $dados['ptrsaldodetalhadopi']
    );
    return $db->pegaUm($stmt);
}

function criaTransacaoPI($rmpid, &$PIs, $tipomovimento) {
    global $db;
    // -- Grava a transação de cada PI
    $sql = <<<DML
INSERT INTO planacomorc.rmpmovimentacao(rmpid, tipomovimento, pliid, vlrmovimento)
  VALUES(%d, '%s', %d, %f)
DML;
    if (is_array($PIs) && count($PIs) > 0) {
        foreach ($PIs as $pliid => &$valor) {
            $valor = str_replace(array('.', ','), array('', '.'), $valor);
            $stmt = sprintf($sql, $rmpid, $tipomovimento, $pliid, $valor);
            $db->executar($stmt);
        }
    }
}

function incrementaPI($ptrid, $PIs) {
    global $db;
    $sqlCheck = <<<DML
SELECT COUNT(1)
  FROM monitora.pi_planointernoptres
  WHERE pliid = %d
    AND ptrid = %d
DML;
    $sqlInsert = <<<DML
INSERT INTO monitora.pi_planointernoptres(pliid, ptrid, pipvalor)
  VALUES(%d, %d, %f)
DML;
    $sqlUpdate = <<<DML
UPDATE monitora.pi_planointernoptres
  SET pipvalor = pipvalor + %f
  WHERE ptrid = %d
    AND pliid = %d
DML;
     if (is_array($PIs) && count($PIs) > 0) {
        foreach ($PIs as $pliid => $valor) {
            $stmtCheck = sprintf($sqlCheck, $pliid, $ptrid);
            if ($db->pegaUm($stmtCheck)) {
                $stmt = sprintf($sqlUpdate, $valor, $ptrid, $pliid);
            } else {
                if (((float) $valor) == 0.00) {
                    continue;
                }
                $stmt = sprintf($sqlInsert, $pliid, $ptrid, $valor);
                /* Sempre que for adicionado um novo PTRES ao PI ele deve voltar para a situação "T" (Atualizar no SIAFI) */
                $db->executar("UPDATE monitora.pi_planointerno SET plisituacao = 'T' WHERE pliid = {$pliid}");
            }
            $db->executar($stmt);
        }
    }
}

function decrementaPI($ptrid, $PIs) {
    global $db;
    $sql = <<<DML
UPDATE monitora.pi_planointernoptres
  SET pipvalor = pipvalor - %f
  WHERE ptrid = %d
    AND pliid = %d
DML;
     if (is_array($PIs) && count($PIs) > 0) {
         foreach ($PIs as $pliid => $valor) {
            $stmt = sprintf($sql, $valor, $ptrid, $pliid);
            $db->executar($stmt);
        }
    }
}

function concluiTransacaoOrigem($rmpidorigem) {
    global $db;
    $sql = <<<DML
UPDATE planacomorc.remanejamentopi
  SET rmpstprocessado = TRUE
  WHERE rmpid = %d
DML;
    $stmt = sprintf($sql, $rmpidorigem);
    $db->executar($stmt);
}

function apagarTransacao($dados)
{
    global $db;

    // -- Atualizando o status do solicitação
    $sql = <<<DML
UPDATE planacomorc.solicitacaocriacaopi
  SET scpstatus = 'I'
  WHERE scpid = %d
DML;
    $db->executar(sprintf($sql, $dados['scpid']));

    // -- Verificando o resultado do update
    if ((bool)$db->commit()) {
        // -- Notificando dono da solicitação
        $sql = <<<DML
SELECT usunome, usuemail, scptitulo, TO_CHAR(scpdata, 'DD/MM/YYYY') AS scpdata
  FROM seguranca.usuario
    INNER JOIN planacomorc.solicitacaocriacaopi scp USING(usucpf)
  WHERE scp.scpid = %d
DML;
        $usudata = $db->pegaLinha(sprintf($sql, $dados['scpid']));
        enviar_email(
            array( // -- Remetente
                'nome' => 'Planejamento e Acompanhamento Orçamentário',
                'email' => $_SESSION['email_sistema']),
            array( // -- Destinatario
                'nome' => $usudata['usunome'],
                'email' => $usudata['usuemail']
            ),
            'Solicitação de novo PI - Exclusão',
            <<<TEXT
Caro usuário,<br />
<p style="padding-left:25px">Sua solicitação de PI realizada no dia {$usudata['scpdata']}, e identificada como "{$usudata['scptitulo']}" foi excluída.<br />
Para realizar uma nova solicitação, acesse o módulo Planejamento e
Acompanhamento Orçamentário no <a href="http://simec.mec.gov.br">SIMEC</a>.</p>
<p style="font-size:9px">Este é um e-mail automático, favor não responder.</p>
TEXT
        );
        return true;
    }
    return false;
}

/**
 * Cria uma query de filtro para a transação de criação e remanejamento de PIs.
 * @param string $tipoTrasacao 'C': criação de PI
 */
function filtroUsuarioResponsabilidade($tipoTransacao)
{
    global $db;

        // -- Identificando o perfil do usuário
    $perfisDoUsuario = pegaPerfilGeral();
    
    // -- Não há restrições para superusuário
    if (1 == $_SESSION['superuser'] || in_array(PFL_CGP_GESTAO, $perfisDoUsuario)) {
        return '';
    }

    // -- Se ele não for GO ou Gabinete, ele não (deveria) tem permissão de listagem
    if (!in_array(PFL_GESTAO_ORCAMENTARIA, $perfisDoUsuario)
        && !in_array(PFL_GABINETE, $perfisDoUsuario)) {
        return 'AND FALSE';
    }

    // -- Preparando os perfis para consultar as responsabilidades do usuário
    if (count($perfisDoUsuario) > 1) {
        $perfis = implode(', ', $perfisDoUsuario);
        $sqlPerfil = <<<DML
AND usr.pflcod IN({$perfis})
DML;
    } else {
        $perfil = current($perfisDoUsuario);
        $sqlPerfil = <<<DML
AND usr.pflcod = {$perfil}
DML;
    }

    // -- Consultando as responsabilidades do USUARIO, se ele for
    // -- da 26101, recebe privilégios de homologação e remanejamento diferenciados.
    $sqlResp = <<<DML
SELECT COALESCE(usr.unicod, ung.unicod) AS unicod
  FROM planacomorc.usuarioresponsabilidade usr
    LEFT JOIN public.unidadegestora ung USING(ungcod)
  WHERE usr.usucpf = '%s'
    %s
    AND rpustatus = 'A'
    AND usr.unicod IS NOT NULL
DML;
    $stmtResp = sprintf($sqlResp, $_SESSION['usucpf'], $sqlPerfil);
    $uos = $db->carregar($stmtResp);

    // -- Se não retornar nenhuma responsabilidade, bloqueie todos as solicitacoes
    if (!$uos) {
        return 'AND FALSE';
    } else {
        foreach ($uos as &$uo) {
            $uo = $uo['unicod'];
        }
    }

    // -- Enviando para tratamento conforme tipo de transação
    switch ($tipoTransacao) {
        case TRANSACAO_CRIACAO_PI:
            return filtroCriacaoPI($uos);
            break;
        case TRANSACAO_REMANEAMENTO_PI:
            return filtroRemanejamentoPI($uos);
            break;
    }
}

function filtroCriacaoPI($uos)
{
    $uosString = implode(', ', $uos);
    //tratando caso que array traga valores vazios, elimando-os para não dar erro no select.
    if(is_array($uosString)){
        foreach($uosString as $key => $value)
        {
            if(empty($value))
            {
                unset($uosString[$key]);
            }
        }
    }

    if (!in_array('26101', $uos)) {
        $sqlBase = <<<DML
    AND EXISTS (SELECT 1
                  FROM planacomorc.solicitacaopidotacao spd
                    INNER JOIN monitora.ptres ptr USING(ptrid)
                  WHERE spd.scpid = scp.scpid
                    AND %s)
    AND NOT EXISTS (SELECT 1
                      FROM planacomorc.solicitacaopidotacao spd
                        INNER JOIN monitora.ptres ptr USING(ptrid)
                      WHERE spd.scpid = scp.scpid
                        AND ptr.unicod = '26101')
    AND scp.unicod != '26101'
DML;
        $stmtBase = sprintf($sqlBase, "ptr.unicod::INTEGER IN({$uosString})");
    } else {
        $sqlBase = <<<DML
    AND (EXISTS (SELECT 1
                   FROM planacomorc.solicitacaopidotacao spd
                     INNER JOIN monitora.ptres ptr USING(ptrid)
                   WHERE spd.scpid = scp.scpid
                     AND %s) OR scp.unicod = '26101')
DML;
    }
    $stmtBase = sprintf($sqlBase, "ptr.unicod::INTEGER IN({$uosString})");
    return $stmtBase;
}

function filtroRemanejamentoPI()
{
    
}


function excluiTransacaoRmp($rmpid){
    global $db;
    if(is_array($rmpid)){
        $rmpid = $rmpid['rmpid'] ;
    }    
    if(trim($rmpid) == ''){
        echo 'false';
        die();
    }
$sql = <<<DML
DELETE
  FROM planacomorc.rmpmovimentacao
  WHERE rmpid = %d
DML;
    $stmt = sprintf($sql, $rmpid);
    try{
        $db->executar($stmt);
    } catch (Exception $ex) {
        echo $ex->getMessage();
        die();
    }
    $sql = <<<DML
DELETE
  FROM planacomorc.remanejamentopi
  WHERE rmpid = %d
DML;
    $stmt = sprintf($sql, $rmpid);
    try{
        $db->executar($stmt);
        $db->commit();        
        echo 'true';
    } catch (Exception $ex) {
        echo 'false';
//        echo 'false'.$ex->getMessage();        
    }    
}