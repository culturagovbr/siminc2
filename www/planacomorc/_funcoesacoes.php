<?php

/**
 * Funções de apoio de ações.
 * $Id: _funcoesacoes.php 94533 2015-02-26 20:19:25Z fellipesantos $
 */
/**
 *
 * @global cls_banco $db
 * @param type $dados
 */
require_once (APPRAIZ . 'includes/library/simec/Listagem.php');

function carregarUnidade($dados) {
    global $db;
    if ($dados ['id_acao_programatica'] && $dados ['loccod'] && $dados ['percod']) {
        $orgcod = $db->pegaUm("SELECT orgcod FROM planacomorc.acao WHERE id_acao_programatica='" . $dados ['id_acao_programatica'] . "'");
        $_SESSION ['planacomorc'] ['orgcod'] = $orgcod;
        $al = array(
            "location" => "planacomorc.php?modulo=principal/acoes/gerenciarunidades&acao=A&aba=monitoraracao&id_acao_programatica=" . $dados ['id_acao_programatica'] . "&loccod=" . $dados ['loccod'] . "&percod=" . $dados ['percod']
        );
        alertlocation($al);
    } else {
        // -- Para mover estes parametros para a URL (e manter a nova aba selecionada, altere
        // -- a montagem do link da aba na função "montaAbasSisopAcoes", adicionando os
        // -- demais parametros à URL.
        $_SESSION ['planacomorc'] ['orgcod'] = $dados ['orgcod'];
        $_SESSION ['planacomorc'] ['percod'] = $dados ['percod'];
        $al = array(
            "location" => "planacomorc.php?modulo=principal/acoes/gerenciarunidades&acao=A&aba=listaacoes"
        );
        alertlocation($al);
    }
}

function montaAbasSisopAcoes($abaativa = null) {
    global $db;

    $menu [] = array(
        "id" => 1,
        "descricao" => "Lista Unidades",
        "link" => "/planacomorc/planacomorc.php?modulo=principal/acoes/listaunidades&acao=A"
    );
    $menu [] = array(
        "id" => 2,
        "descricao" => "Ações ({$_SESSION['exercicio']})",
        "link" => "/planacomorc/planacomorc.php?modulo=principal/acoes/gerenciarunidades&acao=A&aba=listaacoes"
    );
    if ($_REQUEST ['aba'] == 'monitoraracao') {
        $menu [] = array(
            "id" => 4,
            "descricao" => "Acompanhar Ação ({$_SESSION['exercicio']})",
            "link" => '/planacomorc/planacomorc.php?modulo=principal/acoes/gerenciarunidades&acao=A&aba=monitoraracao' . "&id_acao_programatica={$_REQUEST['id_acao_programatica']}" . "&loccod={$_REQUEST['loccod']}" . ($_REQUEST ['percod'] ? "&percod={$_REQUEST['percod']}" : '')
        );
    }
    echo montarAbasArray($menu, $abaativa);
}

function montarCabecalhoUnidade($dados) {
    global $db;

    if (isset($dados ['id_acao_programatica']) && $dados ['id_acao_programatica'] != '') {

        $sql = <<<DML
SELECT org.codigo,
       org.descricao,
       COALESCE(mnt.monnome, '-') AS monnome,
       COALESCE(mnt.monemail, '-') AS monemail,
       COALESCE(mnt.monfone, '-') AS monfone
  FROM planacomorc.acao_programatica apr
    inner join planacomorc.orgao org using(id_orgao)
    left join planacomorc.monitorinterno mnt
      ON (org.codigo = mnt.orgcod and mnt.monano = '{$_SESSION['exercicio']}')
  WHERE apr.id_acao_programatica = {$dados['id_acao_programatica']}
DML;
    } else {
        $sql = <<<DML
SELECT o.codigo,
       o.descricao,
       m.monnome,
       m.monfone,
       m.monemail
  FROM planacomorc.orgao o
    LEFT JOIN planacomorc.monitorinterno m
      ON (m.orgcod = o.codigo AND m.monano = '{$_SESSION['exercicio']}')
  WHERE o.codigo = '{$dados['orgcod']}'
DML;
    }

    $unidade = $db->pegaLinha($sql);
    $_SESSION ['planacomorc'] ['orgcod'] = $unidade ['codigo'];

    if ('-' != $unidade ['monfone']) {
        $unidade ['monfone'] = '(' . substr($unidade ['monfone'], 0, 2) . ') ' . substr($unidade ['monfone'], 2, 4) . '-' . substr($unidade ['monfone'], 6);
    }

    $cabecalho = <<<HTML
<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td class="SubTituloDireita" width=17%>Unidade Orçamentária :</td>
        <td colspan=5>{$unidade['codigo']} - {$unidade['descricao']}</td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width=17%>Monitor interno :</td>
        <td>{$unidade['monnome']}</td>
        <td class="SubTituloDireita" width=17%>Fone :</td>
        <td>{$unidade['monfone']}</td>
        <td class="SubTituloDireita" width=14%>E-mail :</td>
        <td>{$unidade['monemail']}</td>
    </tr>
</table>
HTML;
    return $cabecalho;
}

function consultarAcao($dados) {
    global $db;
    /*descricao_abreviada
    prgtitulo
    objsiop_id objenunciado
    orgpai
     */
    $sql = <<<DML
        SELECT
            COALESCE(aca.prgcod ||'.'|| aca.acacod ||' - '|| aca.acatitulo,'--') AS acao,
            COALESCE(aca.acatitulo,'--') AS titulo,
            COALESCE(aca.prgcod ||' - '|| COALESCE(aca.prgdsc,''),'--') AS prg,
            COALESCE(aca.acaobjetivocod ||' - '|| aca.acaobjetivodsc,'--') AS obj,
            COALESCE(aca.unicod ||' - '|| uni.unidsc,'--') AS uo,
            COALESCE(aca.acainiciativacod ||' - '|| aca.acainiciativadsc,'--') AS iniciativa,
            COALESCE(aca.esfcod ||' - '|| COALESCE(aca.esfdsc,''),'--') AS esfera,
            COALESCE(aca.funcod ||' - '|| COALESCE(aca.fundsc,''),'--') AS funcao,
            COALESCE(aca.sfucod ||' - '|| COALESCE(aca.sfundsc,''),'--') AS subfuncao,
            COALESCE(aca.acadescricao,'--') as acadescricao
        FROM planejamento.acao aca
        JOIN public.unidade uni ON aca.unicod = uni.unicod
        WHERE aca.acacod = '{$dados['acacod']}'
            AND aca.unicod = '{$dados['unicod']}'
            AND aca.loccod = '{$dados['loccod']}'
            AND aca.prgcod = '{$dados['prgcod']}'
            AND aca.prgano = '{$_SESSION['exercicio']}'
DML;
    #ver($sql);
    $acao = $db->pegaLinha($sql);
    #ver($acao);

    $html .= '<table class="table table-striped" width="100%" cellSpacing="1" cellPadding="3" align="center">';
    $html .= '<tr><th class="SubTituloDireita" width=25%>Ação:</th><td>' . $acao ['acao'] . '</td></tr>';
    $html .= '<tr><th class="SubTituloDireita" width=25%>Título:</th><td>' . $acao ['titulo'] . '</td></tr>';
    $html .= '<tr><th class="SubTituloDireita" width=25%>Programa:</th><td>' . $acao ['prg'] . '</td></tr>';
    $html .= '<tr><th class="SubTituloDireita" width=25%>Objetivo:</th><td>' . $acao ['obj'] . '</td></tr>';
    $html .= '<tr><th class="SubTituloDireita" width=25%>Iniciativa:</th><td>' . $acao ['iniciativa'] . '</td></tr>';
    $html .= '<tr><th class="SubTituloDireita" width=25%>Órgão Orçamentário Responsável:</th><td>' . $acao ['uo'] . '</td></tr>';
    //$html .= '<tr><th class="SubTituloDireita" width=25%>Unidade Orçamentária Responsável:</th><td>' . $acao ['orgpai'] . '</td></tr>';
    $html .= '<tr><th class="SubTituloDireita" width=25%>Esfera:</th><td>' . $acao ['esfera'] . '</td></tr>';
    $html .= '<tr><th class="SubTituloDireita" width=25%>Função:</th><td>' . $acao ['funcao'] . '</td></tr>';
    $html .= '<tr><th class="SubTituloDireita" width=25%>Subfunção:</th><td>' . $acao ['subfuncao'] . '</td></tr>';
    $html .= '<tr><th class="SubTituloDireita" width=25%>Descrição:</th><td style="text-align:justify;">' . $acao ['acadescricao'] . '</td></tr>';
    $html .= '</table>';
    echo $html;
}

function gerenciarMonitorInterno($dados) {
    global $db;

    $sql = "SELECT moncpf, monnome, SUBSTR(monfone,1,2) as monfoneddd, SUBSTR(monfone,3) as monfone, monemail
            FROM planacomorc.monitorinterno WHERE orgcod='" . $dados ['orgcod'] . "' AND monano='" . $_SESSION ['exercicio'] . "'";
    $monitorinterno = $db->pegaLinha($sql);

    if ($monitorinterno)
        extract($monitorinterno);
	?>
    <form class="form-horizontal" method="post" id="frm_monitorinterno" name="frm_monitorinterno" action="planacomorc.php?modulo=principal/acoes/listaunidades&acao=A">
    	<input type=hidden name=requisicao value=gravarMonitorInterno>
    	<input type="hidden" name="orgcod" id="orgcod" value="<?=$dados['orgcod']?>">
		<section class="form-group">
			<label class="control-label col-md-2" for="moncpf">CPF</label>
			<section class="col-md-10">
				<?php //echo campo_texto('moncpf', "S", "S", "CPF", 16, 14, "###.###.###-##", "", '', '', 0, 'id="moncpf"', '',
// 						mascaraglobal($moncpf, "###.###.###-##"),
						//(this.value.length==14)?carregaUsuario(): this.value);?>
				<?php inputTexto('moncpf', mascaraglobal($moncpf, "###.###.###-##"), 'moncpf', 14,false,array('masc'=>'###.###.###-##','evtblur'=> 'carregaUsuario()')); ?>
			</section>
		</section>
		<section class="form-group">
			<label class="control-label col-md-2" for="monnome">Nome</label>
			<section class="col-md-10">
				<?php //echo campo_texto('monnome', "S", "N", "Nome", 45, 180, "", "", '', '', 0, 'id="monnome"', '', $monnome);?>
				<?php inputTexto('monnome', $monnome, 'monnome', 180,false);?>
			</section>
		</section>
    	<section class="form-group">
    		<label class="control-label col-md-2" for="telefone" for="monfoneddd">Telefone</label>
    		<section class="col-md-10">
    			<?php //echo campo_texto('monfoneddd', "N", "S", "Telefone", 2, 3, "##", "", '', '', 0, 'id="monfoneddd"', '', $monfoneddd) . " "
 					//	. campo_texto('monfone', "S", "S", "Telefone", 9, 10, "########", "", '', '', 0, 'id="monfone"', '', $monfone);?>
    			<?php inputTexto('monfoneddd', $monfoneddd, 'monfoneddd', 3,false,array('size'=>2,'masc'=>'##'))?>
    			<script>
    				$('#monfoneddd').attr('style','width:50px;float:left;');
					$('#monfone').attr('style','width:200px;margin-left:55px;');
    			</script>
    			<?php inputTexto('monfone', $monfone, 'monfone', 10,false,array('masc'=>'########'))?>
    		</section>
    	</section>
    	<section class="form-group">
    		<label class="control-label col-md-2" for="monemail">Email</label>
    		<section class="col-md-10">
    			<?php //echo campo_texto('monemail', "S", "S", "Email", 45, 50, "", "", '', '', 0, 'id="monemail"', '', $monemail);?>
    			<?php inputTexto('monemail', $monemail, 'monemail', 50,false);?>
    		</section>
    	</section>
    </form>
    <?php
}

function gravarMonitorInterno($dados) {
    global $db;

    $sql = "SELECT monid FROM planacomorc.monitorinterno WHERE orgcod='" . $dados ['orgcod'] . "' AND monano='" . $_SESSION ['exercicio'] . "'";
    $monid = $db->pegaUm($sql);

    if ($monid) {
        $sql = "UPDATE planacomorc.monitorinterno
				SET moncpf='" . str_replace(array(
                    ".",
                    "-"
                        ), array(
                    "",
                    ""
                        ), $dados ['moncpf']) . "',
					monnome='" . $dados ['monnome'] . "',
					monfone='" . $dados ['monfone'] . "',
					monemail='" . $dados ['monemail'] . "'
				WHERE monid='" . $monid . "'";

        $db->executar($sql);
    } else {
        $sql = "INSERT INTO planacomorc.monitorinterno(
	            moncpf, monnome, monfone, monemail, orgcod, monano)
			    VALUES ('" . str_replace(array(
                    ".",
                    "-"
                        ), array(
                    "",
                    ""
                        ), $dados ['moncpf']) . "', '" . $dados ['monnome'] . "', '" . $dados ['monfone'] . "', '" . $dados ['monemail'] . "', '" . $dados ['orgcod'] . "', '" . $_SESSION ['exercicio'] . "');";

        $db->executar($sql);
    }

    $db->commit();
}

function listaUnidadesAcompanhamento($dados) {
    global $db;

    $where = "";
    $wh = array();
    $perfis = pegaPerfilGeral();

    if (1 != $_SESSION['superuser']) {
        if (in_array(PFL_COORDENADORACAO, $perfis)) {
            $whpfl[] = "org.codigo IN(SELECT o.codigo as orgcod
                                        FROM planacomorc.usuarioresponsabilidade u
                                          INNER JOIN planacomorc.acao_programatica a ON a.id_acao_programatica = u.id_acao_programatica
                                          INNER JOIN planacomorc.orgao o ON o.id_orgao = a.id_orgao
                                        WHERE rpustatus='A'
                                          AND usucpf='" . $_SESSION['usucpf'] . "'
                                          AND pflcod='" . PFL_COORDENADORACAO . "')";
        }

        if (in_array(PFL_VALIDADORACAO, $perfis)) {
            $whpfl[] = "org.id_orgao IN (SELECT a.id_orgao
                                           FROM planacomorc.usuarioresponsabilidade u
                                             INNER JOIN planacomorc.acao_programatica a ON a.id_acao_programatica = u.id_acao_programatica
                                           WHERE rpustatus='A'
                                             AND usucpf='" . $_SESSION['usucpf'] . "'
                                             AND pflcod='" . PFL_VALIDADORACAO . "')";
        }

        if (in_array(PFL_VALIDADOR_SUBSTITUTO, $perfis)) {
            $whpfl [] = "org.id_orgao IN (SELECT a.id_orgao
                                           FROM planacomorc.usuarioresponsabilidade u
                                             INNER JOIN planacomorc.acao_programatica a ON a.id_acao_programatica = u.id_acao_programatica
                                           WHERE rpustatus='A'
                                             AND usucpf='" . $_SESSION['usucpf'] . "'
                                             AND pflcod='" . PFL_VALIDADOR_SUBSTITUTO . "')";
        }

        if (in_array(PFL_CONSULTA, $perfis)) {
            $parcial = <<<PARTIAL_DML
    EXISTS (SELECT 1
              FROM planacomorc.usuarioresponsabilidade rpu
              WHERE rpu.id_acao_programatica = acp.id_acao_programatica
                AND rpu.id_periodo_referencia = aca.id_periodo_referencia
                AND rpu.usucpf = '{$_SESSION['usucpf']}'
                AND rpu.pflcod = %d
                AND rpu.rpustatus = 'A')
PARTIAL_DML;
            $whpfl[] = sprintf($parcial, PFL_CONSULTA);
        }
        if ($whpfl) {
            $wh[] = "(" . implode(" OR ", $whpfl) . ")";
        }
    }

    $sql = <<<PARTIAL_DML
SELECT DISTINCT org.codigo AS unicod,
       org.codigo||' ' as codigo,
       org.descricao,
PARTIAL_DML;
    if (1 == $_SESSION ['superuser'] || in_array(PFL_CPMO, pegaPerfilGeral($_SESSION ['usucpf']))) {
        $sql .= <<<PARTIAL_DML
       COALESCE(to_char(mti.moncpf::numeric, '000"."000"."000"-"00" - "') || mti.monnome, 'Não cadastrado') AS monitor
PARTIAL_DML;
    } else {
        $sql .= <<<PARTIAL_DML
       COALESCE(to_char(mti.moncpf::numeric, '000"."000"."000"-"00" - "') || mti.monnome, 'Não cadastrado') AS monitor
PARTIAL_DML;
    }

    if (is_array($wh) && count($wh) > 0) {
        $where = 'AND ' . implode(' AND ', $wh);
    }

    $sql .= <<<PARTIAL_DML
  FROM planacomorc.orgao org
    LEFT JOIN planacomorc.acao_programatica acp USING(id_orgao)
    LEFT JOIN planacomorc.acompanhamento_acao aca ON(
        aca.id_acao_programatica = acp.id_acao_programatica
        AND aca.id_periodo_referencia = {$_REQUEST['percod']}
    )
    LEFT JOIN planacomorc.monitorinterno mti ON(mti.orgcod = org.codigo AND mti.monano = '{$_SESSION['exercicio']}')
  WHERE org.tipo = 'U'
  AND aca.id_periodo_referencia = {$_REQUEST['percod']}
  {$where}
  ORDER BY org.codigo ||' ',
           org.descricao
PARTIAL_DML;
    if ($_REQUEST['percod']) {
        $cabecalho = array(
            "Código",
            "Unidade Orçamentária",
            "Monitor interno"
        );
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->setCabecalho($cabecalho)
            ->setQuery($sql)
            ->addAcao('edit', array('func'=>'gerenciarUnidadeAcao','titulo' => 'Alterar Unidade'))
            ->addAcao('user', array('func'=>'gerenciarMonitorInterno','titulo' => 'Alterar Dados do Monitor'))
            ->addCallbackDeCampo(array('descricao', 'monitor'), 'alinhaParaEsquerda')
            ->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS)
            ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    } else {
        echo <<<HTML
<div class="alert alert-warning text-center col-md-6 col-md-offset-3">Período não selecionado.</div>
HTML;
    }
}

function gravarAcompanhamentoAcao($dados) {
    global $db;
    $id_acompanhamento_acao = $db->pegaUm("
        SELECT
            id_acompanhamento_acao
        FROM planacomorc.acompanhamento_acao
        WHERE id_periodo_referencia='{$dados ['percod']}'
            AND id_acao_programatica='{$dados ['id_acao_programatica']}'
            AND id_localizador='{$dados ['id_localizador']}'");
    /* Verifica o Workflow ou cadastra um Novo */
    $docid = pegaDocid($id_acompanhamento_acao);

    if (!$docid) {
        $docid = wf_cadastrarDocumento(FLUXO_MONITORAMENTOACAO, 'Fluxo : ID ' . $dados ['id_acao_programatica'] . ' / LOCCOD ' . $dados ['loccod'] . ' (' . $dados ['id_localizador'] . ') PERCOD ' . $dados ['percod']);
    }
    if (!$id_acompanhamento_acao) {
        $sql = "
            INSERT INTO planacomorc.acompanhamento_acao(
                id_acao_programatica,
                id_periodo_referencia,
                reprogramado_fisico,
                reprogramado_financeiro,
                executado_fisico,
                data_apuracao,
                data_criacao,
                executado_rap_fisico,
                id_localizador,
                docid,
                instante_alteracao,
                cpf_alteracao)
            VALUES ('{$dados ['id_acao_programatica']}',
                '{$dados ['percod']}',
                '{$dados ['acoreprogramadofisico']}',
                '" . str_replace(array(".",","), array("","."),$dados ['acoreprogramadofinanceiro']) . "',
                '{$dados ['acoexecutadofisico']}',
                '" . formata_data_sql($dados ['acodataapuracao']) . "',
                NOW(),
                '{$dados ['acoexecutadorapfisico']}',
                '{$dados ['id_localizador']}',
                '{$docid}',
                NOW(),
                '{$_SESSION ['usucpf']}') RETURNING id_acompanhamento_acao;";
        $id_acompanhamento_acao = $db->pegaUm($sql);
        $db->commit();
    } else {
        // se já existir um acompanhamento_acao fazer update
        if ($dados ['acoreprogramadofisico'] == '') {
            $dados ['acoreprogramadofisico'] = 0;
        }
        if ($dados ['acoexecutadofisico'] == '') {
            $dados ['acoexecutadofisico'] = 0;
        }
        if ($dados ['reprogramado_fisico'] == '') {
            $dados ['reprogramado_fisico'] = 0;
        }
        if ($dados ['reprogramado_financeiro'] == '') {
            $dados ['reprogramado_financeiro'] = 0;
        }

        $sql = "
            UPDATE planacomorc.acompanhamento_acao
            SET
                reprogramado_fisico='" . $dados ['acoreprogramadofisico'] . "',
                reprogramado_financeiro='" . str_replace(array(".",","), array("","."),$dados ['acoreprogramadofinanceiro']) . "',
                executado_fisico='{$dados ['acoexecutadofisico']}',
                data_apuracao='". formata_data_sql($dados ['acodataapuracao']) ."',
                executado_rap_fisico='{$dados ['acoexecutadorapfisico']}',
                instante_alteracao=NOW(),
                cpf_alteracao='{$_SESSION ['usucpf']}',
                docid = {$docid}
            WHERE id_acompanhamento_acao = '{$id_acompanhamento_acao}'";

        $db->executar($sql);
        $db->commit();
    }

    if (isset($dados ['realizadoloa'])) {
        foreach ($dados ['realizadoloa'] as $plocod => $realizadoloa) {
            if ($realizadoloa != '') {
                $realizadoloa = str_replace('.', '', $realizadoloa);
                // recuperando id_plano e id_po para alimentar tabela acompanhamento_po com o valor do campo "Realizado"
                $arrDados = $db->pegaLinha("
                    SELECT
                        id_plano_orcamentario, id_po_programatica
                    FROM planacomorc.snapshot_dotacao_po_programatica as sdpp
                    INNER JOIN planacomorc.localizador_programatica as lp ON lp.id_localizador_programatica = sdpp.id_localizador_programatica
                    INNER JOIN planacomorc.plano_orcamentario po ON po.id_acao_programatica = lp.id_acao_programatica AND sdpp.plocod = po.codigo
                    WHERE id_periodo_referencia = '{$dados ['percod']}'
                        AND po.id_acao_programatica = '{$dados ['id_acao_programatica']}'
                        AND lp.id_localizador = '{$dados ['id_localizador']}'
                        AND sdpp.id_localizador_programatica = '{$dados ['id_localizador_programatica']}'
                        AND po.codigo = '{$plocod}'");

                $id_acompanhamento_po = $db->pegaUm("
                    SELECT
                        id_acompanhamento_po
                    FROM planacomorc.acompanhamento_po
                    WHERE id_localizador_programatica = '{$dados ['id_localizador_programatica']}'
                        AND id_plano_orcamentario = '{$arrDados ['id_plano_orcamentario']}'
                        AND id_po_programatica = '{$arrDados ['id_po_programatica']}'");

                /**
                 * @todo Considera-se que carga dada na tabela planacomorc.snapshot_dotacao_po_programatica terá vinculo direto com plano_orcamentario
                 * através do po.codigo e sdpp.plocod, caso contrário não salvará dados, corretamente, na tabela acompanhamento_po
                 */
                if ((empty($id_acompanhamento_po)) && (!empty($arrDados))) { // insert acompanhamento_po
                    $sqlAcompanhamentoPo = "
                        INSERT INTO planacomorc.acompanhamento_po
                            (id_localizador_programatica,id_plano_orcamentario,realizado_loa,id_po_programatica,
                            id_periodo_referencia,plocod)
                        VALUES({$dados ['id_localizador_programatica']},{$arrDados ['id_plano_orcamentario']},
                            '{$realizadoloa}',{$arrDados ['id_po_programatica']},{$dados ['percod']},'{$plocod}'
                            )";
                } elseif ((!empty($id_acompanhamento_po)) && (!empty($arrDados))) { // realizando update
                    $sqlAcompanhamentoPo = "
                        UPDATE planacomorc.acompanhamento_po SET
                            realizado_loa = '{$realizadoloa}'
                        WHERE id_acompanhamento_po = '{$id_acompanhamento_po}'
                            AND id_periodo_referencia = '{$dados ['percod']}'
                            AND plocod = '{$plocod}'";
                }
                $db->executar($sqlAcompanhamentoPo);
            }
        }
    }

    $sql = "DELETE FROM planacomorc.monquestionarioresposta WHERE id_acompanhamento_acao = '$id_acompanhamento_acao'";
    $db->executar($sql);

    if ($dados ['idperguntas']) {
        foreach ($dados ['idperguntas'] as $mqpid) {
            $sql = "
                INSERT INTO planacomorc.monquestionarioresposta(
                    id_acompanhamento_acao, mqpid, mqrresposta, dtcriacao, usucpf)
                VALUES ('{$id_acompanhamento_acao}', '{$mqpid}', '{$dados ['resposta_' . $mqpid]}',
                    NOW(), '{$_SESSION ['usucpf']}');";
            $db->executar($sql);
        }
    }

    $db->commit();

    $al = array(
        "alert" => "Acompanhamento gravado com sucesso",
        "location" => "planacomorc.php?modulo=principal/acoes/gerenciarunidades&acao=A&aba=monitoraracao&id_acao_programatica=" . $dados ['id_acao_programatica'] . "&loccod=" . $dados ['loccod'] . "&percod=" . $dados ['percod']
    );
    alertlocation($al);
}

/**
 * Função de listagem de <UOs> para a central de Acompanhamento.
 * Essa mesma querie
 * é utilizada na listagem de localizadores utilizando agrupadores diferentes.
 *
 * @global cls_banco $db Conexão com a base de dados.
 * @param array $dados
 *        	$_REQUEST
 * @see detalharDiagnosticoMonitoramentoAcao
 */
function diagnosticoMonitoramentoAcao($dados) {
    global $db;

    if ($_POST ['percod']) {
        $dados ['percod'] = $_POST ['percod'];
        $id_periodo_referencia = $dados ['percod'];
    } else {
        $sqlPer = <<<DML
            SELECT
                id_periodo_referencia AS codigo,
                titulo || ' : ' || to_char(inicio_validade,'DD/MM/YYYY') ||' a ' || to_char(fim_validade,'DD/MM/YYYY') as descricao
            FROM planacomorc.periodo_referencia p
            WHERE id_exercicio = '{$_SESSION ['exercicio']}'
            ORDER BY id_periodo_referencia desc limit 1
DML;
        $pers = $db->pegaLinha($sqlPer);
        $id_periodo_referencia = $pers ['codigo'];
    }
    // id_periodo_referencia = $dados['percod'] ? $dados['percod'] : 0;
    // ######## validação específica para perfil Validador Ação e Validador Substituto ###################
    // definindo váriavel $wh
    $wh = array();
    // carregando perfil logado
    $perfis = pegaPerfilGeral();

    // montando WHERE caso o usuário logado tenha perfil de Validador Ação ou Validador Substituto
    if (in_array(PFL_VALIDADORACAO, $perfis) || in_array(PFL_VALIDADOR_SUBSTITUTO, $perfis)) {
        $pflcod = implode(" ,", $perfis);
        $whpfl [] = <<<DML
            org.id_orgao IN
                (SELECT pap.id_orgao
                FROM planacomorc.usuarioresponsabilidade pu
                INNER JOIN planacomorc.acao_programatica pap ON pap.id_acao_programatica = pu.id_acao_programatica
                INNER JOIN planacomorc.orgao as po ON po.id_orgao = pap.id_orgao
                WHERE rpustatus='A'
                    AND usucpf='{$_SESSION ['usucpf']}'
                    AND pflcod in ({$pflcod})
                )
DML;
    }

    // tratando condição para add na query
    if ($whpfl) {
        $wh [] = "(" . implode(" OR ", $whpfl) . ")";
    }
    // ####### fim validação ##############################################################################

    /* Filtra apenas ações já diligenciadas no Workflow */
    if (isset($dados ['diligenciadas']) && $dados ['diligenciadas'] == 'true') {
        $filtroDiligenciadas = <<<DML
            AND doc.esdid IN (749,750)
            AND doc.docid IN (
                SELECT DISTINCT docid
                FROM workflow.historicodocumento hsd
                INNER JOIN workflow.acaoestadodoc aed USING (aedid)
                WHERE esdidorigem IN (753,752,751)
            )
DML;
    }

    $wh = (($wh) ? " AND " . implode(" AND ", $wh) : "");
    $sql = <<<DML
        SELECT cnt.unicod AS acao,
            {$id_periodo_referencia} AS periodo,
            'this' as this,
            cnt.unicod || ' - ' || cnt.unidsc AS descricao,
            SUM(naoiniciado) AS naoiniciado,
            SUM(emelaboracao) AS emelaboracao,
            SUM(emvalidacao) AS emvalidacao,
            SUM(emaprovacao) AS emaprovacao,
            SUM(finalizado) AS finalizado,
            SUM(enviadosiop) AS enviadosiop
        FROM
            (SELECT DISTINCT
                aca.codigo AS acacod,
                org.codigo AS unicod,
                org.descricao AS unidsc,
                lpr.loccod,
                CASE WHEN doc.esdid IS NULL THEN 1 ELSE 0 END AS naoiniciado,
                CASE WHEN doc.esdid = 749 THEN 1 ELSE 0 END AS emelaboracao,
                CASE WHEN doc.esdid = 750 THEN 1 ELSE 0 END AS emvalidacao,
                CASE WHEN doc.esdid = 751 THEN 1 ELSE 0 END AS emaprovacao,
                CASE WHEN doc.esdid = 752 THEN 1 ELSE 0 END AS enviadosiop,
                CASE WHEN doc.esdid = 753 THEN 1 ELSE 0 END AS finalizado,
                esdid
            FROM planacomorc.acao aca
            JOIN planacomorc.acao_programatica apr ON apr.id_acao = aca.id_acao
            JOIN planacomorc.dados_acao_exercicio dae ON dae.id_acao = aca.id_acao AND dae.id_exercicio=apr.id_exercicio
            INNER JOIN planacomorc.localizador_programatica lpr USING (id_acao_programatica)
            LEFT JOIN planacomorc.quantitativo_sof qsf ON (qsf.loccod = lpr.loccod AND qsf.id_acao_programatica = apr.id_acao_programatica)
            JOIN planacomorc.programa pro ON pro.id_programa = aca.id_programa
            JOIN planacomorc.orgao org ON org.id_orgao = apr.id_orgao
            LEFT JOIN planacomorc.acompanhamento_acao aac ON (aac.id_localizador = lpr.id_localizador AND aac.id_acao_programatica = lpr.id_acao_programatica)
            LEFT JOIN workflow.documento doc ON(doc.docid = aac.docid)
            WHERE qsf.quantidade_fisico != 0
                AND dae.id_produto IS NOT NULL
                AND apr.id_exercicio = {$_SESSION['exercicio']}
                AND aac.id_periodo_referencia = {$id_periodo_referencia}
                {$filtroDiligenciadas}
                {$wh}
            ) cnt
        GROUP BY cnt.unicod, cnt.unidsc
        ORDER BY cnt.unicod, cnt.unidsc
DML;
//    ver($sql/*,d*/);

    $cabecalho = array(
        "Unidade",
        "Não iniciado",
        "Em preenchimento",
        "Em validação",
        "Em aprovação",
        "Finalizado",
        "Enviado para SIOP"
    );

    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
    $listagem->setCabecalho($cabecalho);
    $listagem->setQuery($sql);
    $listagem->esconderColuna(array('this','periodo'));
    $listagem->addAcao('plus', array('func'=>'detalharDiagnosticoMonitoramentoAcao','extra-params'=>array('percod'=>'periodo','obj'=>'this')));
    $listagem->addCallbackDeCampo(array('descricao' ), 'alinhaParaEsquerda');
    $listagem->turnOnPesquisator();
    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('naoiniciado',
        'emelaboracao',
        'emvalidacao',
        'emaprovacao',
        'enviadosiop',
        'finalizado'));
    $listagem->render();
}

/**
 * Função de listagem de <localizadores> para a central de Acompanhamento
 * (Detalhamento da listagem de UOs).
 * Essa mesma querie é utilizada na listagem
 * de UOs utilizando agrupadores diferentes.
 *
 * @global cls_banco $db Conexão com a base de dados.
 * @param array $dados
 *        	$_REQUEST
 * @see diagnosticoMonitoramentoAcao
 */
function detalharDiagnosticoMonitoramentoAcao($dados) {
    global $db;
	//ver($dados['dados']);
    /* Filtra apenas ações já diligenciadas no Workflow */
    if (isset($dados ['diligenciadas']) && $dados ['diligenciadas'] == 'true') {
        $filtroDiligenciadas = <<<DML
         AND doc.esdid IN (749,750)
         AND doc.docid IN
            (SELECT DISTINCT
                docid
            FROM workflow.historicodocumento hsd
            INNER JOIN workflow.acaoestadodoc aed USING (aedid)
            WHERE esdidorigem IN (753,752,751) )
DML;
    }

    $id_periodo_referencia = $dados['dados'] [1] ? $dados['dados'] [1] : 0;
    $sql = <<<DML
        SELECT cnt.id_acao_programatica as acao,
            cnt.loccod as loccod,
            cnt.acacod||'.'||cnt.loccod||' - '||cnt.locdescricao AS descricao,
            SUM(naoiniciado) AS naoiniciado,
            SUM(emelaboracao) AS emelaboracao,
            SUM(emvalidacao) AS emvalidacao,
            SUM(emaprovacao) AS emaprovacao,
            SUM(enviadosiop) AS enviadosiop,
            SUM(finalizado) AS finalizado
        FROM
            (SELECT DISTINCT
                apr.id_acao_programatica,
                aca.codigo AS acacod,
                lpr.loccod,
                lpr.locdescricao,
                CASE WHEN doc.esdid IS NULL THEN 1 ELSE 0 END AS naoiniciado,
                CASE WHEN doc.esdid = 749 THEN 1 ELSE 0 END AS emelaboracao,
                CASE WHEN doc.esdid = 750 THEN 1 ELSE 0 END AS emvalidacao,
                CASE WHEN doc.esdid = 751 THEN 1 ELSE 0 END AS emaprovacao,
                CASE WHEN doc.esdid = 752 THEN 1 ELSE 0 END AS enviadosiop,
                CASE WHEN doc.esdid = 753 THEN 1 ELSE 0 END AS finalizado,
                esdid
            FROM planacomorc.acao aca
            JOIN planacomorc.acao_programatica apr ON apr.id_acao = aca.id_acao
            JOIN planacomorc.dados_acao_exercicio dae ON dae.id_acao = aca.id_acao AND dae.id_exercicio=apr.id_exercicio
            INNER JOIN planacomorc.localizador_programatica lpr USING (id_acao_programatica)
            LEFT JOIN planacomorc.quantitativo_sof qsf ON (qsf.loccod = lpr.loccod AND qsf.id_acao_programatica = apr.id_acao_programatica)
            JOIN planacomorc.programa pro ON pro.id_programa = aca.id_programa
            JOIN planacomorc.orgao org ON org.id_orgao = apr.id_orgao
            LEFT JOIN planacomorc.acompanhamento_acao aac ON (aac.id_localizador = lpr.id_localizador AND aac.id_acao_programatica = lpr.id_acao_programatica)
            LEFT JOIN workflow.documento doc ON(doc.docid = aac.docid)
            WHERE qsf.quantidade_fisico != 0
                AND dae.id_produto IS NOT NULL
                AND apr.id_exercicio = {$_SESSION['exercicio']}
                AND aac.id_periodo_referencia = {$id_periodo_referencia}
                AND org.codigo = '{$dados['dados'][0]}'
                {$filtroDiligenciadas}
                /*AND lpr.id_localizador_programatica IN
                    (select distinct id_localizador_programatica
                    from planacomorc.snapshot_dotacao_localizador_programatica
                    where snapshot_dotacao_localizador_programatica.id_periodo_referencia = {$id_periodo_referencia})*/
            ) cnt
        GROUP BY cnt.id_acao_programatica, cnt.acacod, cnt.loccod, cnt.locdescricao
        ORDER BY cnt.id_acao_programatica, cnt.acacod, cnt.loccod, cnt.locdescricao
DML;

    $cabecalho = array(
    	"Descrição",
        "Não iniciado",
        "Em preenchimento",
        "Em validação",
        "Em Aprovação",
        "Finalizado",
        "Enviado para SIOP"
    );

    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
    $listagem->esconderColuna('loccod');
    $listagem->addAcao('view', array('func'=>'monitorarAcao','extra-params'=>array('loccod')));
    $listagem->setQuery($sql);
    $listagem->setCabecalho($cabecalho);
    $listagem->addCallbackDeCampo(array('descricao' ), 'alinhaParaEsquerda');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA,array('naoiniciado','emelaboracao','emvalidacao','emaprovacao','enviadosiop','finalizado'));
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    //$db->monta_lista_simples($sql, $cabecalho, 400, 5, 'S', '100%', 'N', false, $arrHeighTds, $heightTBody);
}

function enviarEmailAcaoValidador($acaidentificadorunicosiop, $idacomacao) { // -- A
    global $db;
    $sql = <<<DML
SELECT aca.codigo
  FROM planacomorc.acao_programatica apr
    INNER JOIN planacomorc.acao aca USING(id_acao)
  WHERE id_acao_programatica = {$acaidentificadorunicosiop}
DML;
    $msgValidador = "O coordenador da ação " . $db->pegaUm($sql) . " enviou uma análise para ser validada.";
    // -- Recuperando a mensagem digitada ao rejeitar.
    $sq3 = "SELECT cmddsc
              FROM planacomorc.acompanhamento_acao a
                INNER JOIN workflow.documento d USING(docid)
                INNER JOIN workflow.comentariodocumento c USING(docid)
              WHERE id_acompanhamento_acao='" . $idacomacao . "'";
    $msgRetorno = $db->pegaUm($sq3);
    if ($msgRetorno) {
        $msgValidador .= '<br /><br />Observações<br />' . $msgRetorno;
    }

    $sql = "SELECT u.usunome, u.usuemail
              FROM planacomorc.usuarioresponsabilidade ur
                INNER JOIN seguranca.usuario u ON u.usucpf = ur.usucpf
              WHERE id_acao_programatica='" . $acaidentificadorunicosiop . "'
                AND ur.pflcod='" . PFL_VALIDADORACAO . "'
                AND rpustatus='A'";
    $usrs = $db->carregar($sql);

    if ($usrs [0]) {
        foreach ($usrs as $us) {
            $arDest = array();
            $arDest [] = $us ['usuemail'];
            enviar_email(array(
                'nome' => 'Planejamento e Acompanhamento Orçamentário',
                'email' => $_SESSION['email_sistema']
                    ), $arDest, 'Análise pendente', $msgValidador);
        }
    }
    return true;
}

function enviarEmailAcaoCPMO($acaidentificadorunicosiop) { // -- B
    global $db;

    $sql = <<<DML
SELECT org.codigo AS org_codigo, aca.codigo AS aca_codigo
  FROM planacomorc.acao_programatica apr
    INNER JOIN planacomorc.orgao org USING(id_orgao)
    INNER JOIN planacomorc.acao aca USING(id_acao)
  WHERE id_acao_programatica = {$acaidentificadorunicosiop}
DML;
    $data = $db->pegaLinha($sql);
    $msgCPMO = "A UO {$data['org_codigo']} enviou o acompanhamento da ação {$data['aca_codigo']}.";

    $sql = "SELECT u.usunome, u.usuemail
              FROM seguranca.usuario u
                INNER JOIN seguranca.perfilusuario p ON p.usucpf = u.usucpf
              WHERE p.pflcod='" . PFL_CPMO . "'";

    $usrs = $db->carregar($sql);

    if ($usrs [0]) {
        foreach ($usrs as $us) {
            $arDest = array();
            $arDest [] = $us ['usuemail'];
            enviar_email(array(
                'nome' => 'Planejamento e Acompanhamento Orçamentário',
                'email' => $_SESSION['email_sistema']
                    ), $arDest, 'Acompanhamento de ação', $msgCPMO);
        }
    }
    return true;
}

function enviarEmailAcaoCoordenador($acaidentificadorunicosiop, $idacomacao) { // -- C
    global $db;

    $sql = <<<DML
SELECT aca.codigo
  FROM planacomorc.acao_programatica apr
    INNER JOIN planacomorc.acao aca USING(id_acao)
  WHERE id_acao_programatica = {$acaidentificadorunicosiop}
DML;
    $msgRevisao = "A ação " . $db->pegaUm($sql) . " precisa ser revisada.";
    // -- Recuperando a mensagem digitada ao rejeitar.
    $sq3 = "SELECT cmddsc
              FROM planacomorc.acompanhamento_acao a
                INNER JOIN workflow.documento d USING(docid)
                INNER JOIN workflow.comentariodocumento c USING(docid)
              WHERE id_acompanhamento_acao='" . $idacomacao . "'";
    $msgRetorno = $db->pegaUm($sq3);
    if ($msgRetorno) {
        $msgRevisao .= '<br /><br />Observações<br />' . $msgRetorno;
    }

    $sql = "SELECT u.usunome, u.usuemail
              FROM planacomorc.usuarioresponsabilidade ur
                INNER JOIN seguranca.usuario u ON u.usucpf = ur.usucpf
              WHERE id_acao_programatica='" . $acaidentificadorunicosiop . "'
                AND ur.pflcod='" . PFL_COORDENADORACAO . "' AND rpustatus='A'";
    $usrs = $db->carregar($sql);

    if ($usrs [0]) {
        foreach ($usrs as $us) {
            $arDest = array();
            $arDest [] = $us ['usuemail'];
            enviar_email(array(
                'nome' => 'Planejamento e Acompanhamento Orçamentário',
                'email' => $_SESSION['email_sistema']
                    ), $arDest, 'Revisão pendente', $msgRevisao);
        }
    }
    return true;
}

function enviarEmailAcaoCPMORetorno($acaidentificadorunicosiop) { // -- B
    global $db;

    $sql = <<<DML
SELECT aca.codigo
  FROM planacomorc.acao_programatica apr
    INNER JOIN planacomorc.acao aca USING(id_acao)
  WHERE id_acao_programatica = {$acaidentificadorunicosiop}
DML;
    $msgRevisao = "A ação " . $db->pegaUm($sql) . " precisa ser revisada.";
    // -- Recuperando a mensagem digitada ao rejeitar.

    $sql = "SELECT u.usunome, u.usuemail
              FROM planacomorc.usuarioresponsabilidade ur
                INNER JOIN seguranca.usuario u ON u.usucpf = ur.usucpf
              WHERE id_acao_programatica='" . $acaidentificadorunicosiop . "'
                AND ur.pflcod='" . PFL_VALIDADORACAO . "' AND rpustatus='A'";
    $usrs = $db->carregar($sql);

    if ($usrs [0]) {
        foreach ($usrs as $us) {
            $arDest = array();
            $arDest [] = $us ['usuemail'];
            enviar_email(array(
                'nome' => 'Planejamento e Acompanhamento Orçamentário',
                'email' => $_SESSION['email_sistema']
                    ), $arDest, 'Revisão pendente', $msgRevisao);
        }
    }
    return true;
}

function enviarEmailAcaoSIOPRetorno($acaidentificadorunicosiop) { // -- B
    global $db;

    $sql = <<<DML
SELECT aca.codigo
  FROM planacomorc.acao_programatica apr
    INNER JOIN planacomorc.acao aca USING(id_acao)
  WHERE id_acao_programatica = {$acaidentificadorunicosiop}
DML;
    $msgRevisao = "A ação " . $db->pegaUm($sql) . " precisa ser revisada.";
    // -- Recuperando a mensagem digitada ao rejeitar.

    $sql = "SELECT u.usunome, u.usuemail
              FROM planacomorc.usuarioresponsabilidade ur
                INNER JOIN seguranca.usuario u ON u.usucpf = ur.usucpf
              WHERE id_acao_programatica='" . $acaidentificadorunicosiop . "'
                AND ur.pflcod='" . PFL_CPMO . "' AND rpustatus='A'";
    $usrs = $db->carregar($sql);

    if ($usrs [0]) {
        foreach ($usrs as $us) {
            $arDest = array();
            $arDest [] = $us ['usuemail'];
            enviar_email(array(
                'nome' => 'Planejamento e Acompanhamento Orçamentário',
                'email' => $_SESSION['email_sistema']
                    ), $arDest, 'Revisão pendente', $msgRevisao);
        }
    }
    return true;
}

/* Verifica o DOCID para a Subação */

function pegaDocid($idAcompanhamentoAcao) {
    global $db;
    $idAcompanhamentoAcao = (integer) $idAcompanhamentoAcao;
    $sql = "SELECT
			 docid
			FROM
			 planacomorc.acompanhamento_acao
			WHERE
			 id_acompanhamento_acao  = " . $idAcompanhamentoAcao;
    return (integer) $db->pegaUm($sql);
}

/**
 * Função de listagem de <UOs> para a central de Acompanhamento.
 * Essa mesma querie
 * é utilizada na listagem de localizadores utilizando agrupadores diferentes.
 *
 * @global cls_banco $db Conexão com a base de dados.
 * @param array $dados
 *        	$_REQUEST
 * @see detalharDiagnosticoMonitoramentoAcao
 */
function acompanhamentoMonitoramentoSubacao($dados) {
    global $db;
    if ($_POST ['percod']) {
        $dados ['percod'] = $_POST ['percod'];
        $id_periodo_referencia = $dados ['percod'];
    } else {
        $sqlPer = "SELECT id_periodo_referencia AS codigo,
                   titulo || ' : ' || to_char(inicio_validade,'DD/MM/YYYY') ||' a ' || to_char(fim_validade,'DD/MM/YYYY') as descricao
    		FROM planacomorc.periodo_referencia p
    		WHERE id_exercicio = '" . $_SESSION ['exercicio'] . "'
    		ORDER BY id_periodo_referencia desc limit 1";

        $pers = $db->pegaUm($sqlPer);
        $id_periodo_referencia = $pers ['codigo'] ? $pers ['codigo'] : 0;
    }
    // detalharDiagnosticoMonitoramentoAcao
    $id_periodo_referencia = $dados ['percod'] ? $dados ['percod'] : 0;

    $sql = " SELECT
				' '||cnt.aca_codigo||' - '||cnt.titulo AS descricao,
				SUM(nao_iniciado) AS naoiniciado,
				SUM(em_elaboracao) AS emelaboracao,
				SUM(em_validacao) AS emvalidacao,
				SUM(em_aprovacao) AS emaprovacao,
				SUM(enviado_siop) AS enviadosiop,
				SUM(finalizado) AS finalizado
			FROM (
					SELECT  apr.id_subacao,
						   	s.codigo AS aca_codigo,
						   	--lpr.loccod,
						   	s.titulo as titulo,
							CASE WHEN esdid IS NULL THEN SUM(1) ELSE 0 END AS nao_iniciado,
               				CASE WHEN esdid = " . ESD_EMELABORACAO . " THEN SUM(1) ELSE 0 END AS em_elaboracao,
	               			CASE WHEN esdid = " . ESD_EMVALIDACAO . " THEN SUM(1) ELSE 0 END AS em_validacao,
	               			CASE WHEN esdid = " . ESD_EMAPROVACAO . " THEN SUM(1) ELSE 0 END AS em_aprovacao,
	               			CASE WHEN esdid = " . ESD_ENVIADOSIOP . " THEN SUM(1) ELSE 0 END AS enviado_siop,
	               			CASE WHEN esdid = " . ESD_FINALIZADO . " THEN SUM(1) ELSE 0 END AS finalizado
					FROM planacomorc.snapshot_dotacao_subacao apr
					INNER JOIN planacomorc.subacao s USING(id_subacao)
					LEFT JOIN planacomorc.acompanhamento_subacao aco  ON (apr.id_subacao = aco.id_subacao AND aco.id_periodo_referencia = 2 )
					LEFT JOIN workflow.documento doc ON doc.docid = aco.docid
					WHERE s.id_exercicio = {$_SESSION['exercicio']}
					--AND org.tipo = 'U'
					--AND org.codigo = '{$dados['orgcod']}'
					GROUP BY doc.esdid, apr.id_subacao, s.codigo, s.titulo
				 ) cnt
			GROUP BY cnt.id_subacao, cnt.aca_codigo, cnt.titulo
			ORDER BY cnt.id_subacao, cnt.aca_codigo, cnt.titulo";

    $cabecalho = array(
        "Unidade",
        "Não iniciado",
        "Em preenchimento",
        "Em validação",
        "Em Aprovação",
        "Finalizado",
        "Enviado para SIOP"
    );
    $heightTBody = 300;
    $arrHeighTds = array(
        '51',
        '8',
        '8',
        '8',
        '8',
        '8',
        '8',
        '1'
    );
    $db->monta_lista_simples($sql, $cabecalho, 400, 5, 'S', '100%', 'N', false, $arrHeighTds, $heightTBody);
}

function recuperaCoordenadorValidador($cpf,$pflcod) {
    global $db;
    $sql = <<<DML
        SELECT
            1
        FROM
        seguranca.perfil p
        INNER JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod
            AND pu.usucpf = '{$cpf}'
            AND sisid=157
            AND p.pflcod = {$pflcod}
        WHERE p.pflstatus='A'
DML;
    if($db->pegaUm($sql))
        $permissao = true ;
    else
        $permisao = false;


    $sql = <<<DML
        SELECT
            usunome as nome,
            usuemail as email,
            usufoneddd as ddd,
            usufonenum as fone
        FROM seguranca.usuario usu
        WHERE usu.usucpf = '{$cpf}'
        LIMIT 1
DML;
    $dadosCPF = $db->pegaLinha($sql);
    $dadosCPF = array_merge($dadosCPF,array('permissao'=>$permissao));
    foreach ($dadosCPF as $k => $v) {
        $dadosCPF[$k] = utf8_encode($v);
    }
    return (simec_json_encode($dadosCPF));
}

function coordenadorValidadorAcao($dados){
    global $db;
    $perfil = pegaPerfilGeral();
    if(!(in_array(PFL_CPMO, $perfil) || in_array(PFL_SUPERUSUARIO, $perfil))){
?>
<section class="alert alert-danger text-center">
    Seu perfil não possui permissão para efetuar tal ação.
</section>
<script>
    $('#modal-confirm .btn-primary').hide();
</script>
<?
    die();
    }
    if($dados['tipo'] == 'coordenador'){
        $sql = "
            SELECT DISTINCT
                us.usunome AS nome,
                us.usucpf AS cpf,
                us.usufoneddd AS foneddd,
                us.usufonenum AS fone,
                usuemail AS email
            FROM planacomorc.usuarioresponsabilidade ur
            INNER JOIN seguranca.usuario us ON us.usucpf = ur.usucpf
            INNER JOIN seguranca.perfil pe ON pe.pflcod = ur.pflcod
            WHERE ur.pflcod = '" . PFL_COORDENADORACAO . "'
                AND id_acao_programatica = '{$dados['id_acao_programatica']}'
                AND id_periodo_referencia = {$dados['percod']}
                AND rpustatus = 'A'
        ";
        $responsavel = $db->pegaLinha($sql);
        $responsavel['pflcod'] = PFL_COORDENADORACAO;
    }else if($dados['tipo'] == 'validador'){
        $sql = "
            SELECT DISTINCT
                us.usunome AS nome,
                us.usucpf AS cpf,
                us.usufoneddd AS foneddd,
                us.usufonenum AS fone,
                usuemail AS email
            FROM planacomorc.usuarioresponsabilidade ur
            INNER JOIN seguranca.usuario us ON us.usucpf = ur.usucpf
            INNER JOIN seguranca.perfil pe ON pe.pflcod = ur.pflcod
            WHERE ur.pflcod = '" . PFL_VALIDADORACAO . "'
                AND id_acao_programatica = '{$dados['id_acao_programatica']}'
                AND id_periodo_referencia = {$dados['percod']}
                AND rpustatus = 'A'
        ";
        $responsavel = $db->pegaLinha($sql);
        $responsavel['pflcod'] = PFL_VALIDADORACAO;
    }
    ?>

<form class="form-horizontal" method="post" id="frm_coordenador" name="frm_coordenador" action="planacomorc.php?modulo=principal/acoes/gerenciarunidades&acao=A">
    <input type=hidden name=requisicao value=gravarResponsabilidadeAcao>
    <input type=hidden name=id_acao_programatica id=id_acao_programatica value=<?=$dados['id_acao_programatica']?>>
    <input type=hidden name=id_periodo_referencia id=id_periodo_referencia value=<?=$dados['percod']?>>
    <input type=hidden name=pflcod id=pflcod value=<?=$responsavel['pflcod']?>>

    <section id="mensagem_coord"></section>
    <section class="form-group">
        <label class="control-label col-md-2" for="tipo">Perfil Responsável:</label>
        <section class="col-md-10">
            <p class="form-control-static"><span class="label label-info"><?= $dados['tipo'] == 'coordenador' ? 'Coordenador': 'Validador'?></span></p>
        </section>
    </section>
    <section class="form-group">
        <label class="control-label col-md-2" for="cpf">CPF</label>
        <section class="col-md-10">
            <?php inputTexto('cpf', mascaraglobal($responsavel['cpf'], "###.###.###-##"), 'cpf', 14,false,array('masc'=>'###.###.###-##','evtblur'=> "carregaCoordenadorValidador('{$responsavel['pflcod']}')")); ?>
        </section>
    </section>
    <section class="form-group">
        <label class="control-label col-md-2" for="monnome">Nome</label>
        <section class="col-md-10">
            <?php inputTexto('nome', $responsavel['nome'], 'nome', 180,false);?>
        </section>
    </section>
    <section class="form-group">
        <label class="control-label col-md-2" for="telefone" for="monfoneddd">Telefone</label>
        <section class="col-md-10">
            <?php inputTexto('foneddd', $responsavel['foneddd'], 'foneddd', 3,false,array('size'=>2,'masc'=>'##'))?>
            <?php inputTexto('fone', $responsavel['fone'], 'fone', 10,false,array('masc'=>'########'))?>
        </section>
    </section>
    <section class="form-group">
        <label class="control-label col-md-2" for="monemail">Email</label>
        <section class="col-md-10">
            <?php inputTexto('email', $responsavel['email'], 'email', 50,false);?>
        </section>
    </section>
</form>
<script>
    $('#foneddd').attr('disabled','true');
    $('#fone').attr('disabled','true');
    $('#nome').attr('disabled','true');
    $('#email').attr('disabled','true');
    $('#foneddd').attr('style','width:50px;float:left;');
    $('#fone').attr('style','width:200px;margin-left:55px;');
</script>
<?
}

function gravarResponsabilidadeAcao($dados) {
    global $db;
    $sql = "
        UPDATE planacomorc.usuarioresponsabilidade
            SET rpustatus='I'
        WHERE id_acao_programatica='{$dados['id_acao_programatica']}'
            AND pflcod='{$dados['pflcod']}'
            AND id_periodo_referencia = {$dados['id_periodo_referencia']}
    ";
    $db->executar($sql);
    $dados['cpf'] = str_replace('.', '', $dados['cpf']);
    $dados['cpf'] = str_replace('-', '', $dados['cpf']);
    $sql = "
        INSERT INTO planacomorc.usuarioresponsabilidade(
            pflcod,
            usucpf,
            rpustatus,
            rpudata_inc,
            id_acao_programatica,
            id_periodo_referencia)
        VALUES
            ('{$dados['pflcod']}',
            '{$dados['cpf']}', 'A', NOW(),
            '{$dados['id_acao_programatica']}',
            {$dados['id_periodo_referencia']});
    ";

    $db->executar($sql);

    if($db->commit()){
        return utf8_encode(simec_json_encode(array('resultado'=>true,'mensagem'=>'Responsável pela Ação atualizado com sucesso')));
    }else {
        return utf8_encode(simec_json_encode(array('resultado'=>false,'mensagem'=>'Falha ao atualizar Responsável pela Ação')));
    }
}


