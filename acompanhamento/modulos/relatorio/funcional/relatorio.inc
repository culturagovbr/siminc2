<?php
ini_set("memory_limit","1024M");
set_time_limit(0);

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.simple.class.inc';

// instancia a classe de relatório
$rel = new relatorioSimple();
//ver($_REQUEST['filtro'],d);
$rel->setColunas($_REQUEST['coluna'], $_REQUEST['naoColuna']);
$rel->setFiltros($_REQUEST['filtro']);
$rel->setOrdenacao($_REQUEST['campoOrdenacao']);
$rel->setTipoRelatorio("funcional");

//Lista de colunas, Agrupadores e joins
$col = array();
$col["unosigla"]              = array("campo" => "unosigla",             "alias" => "suo",          "label" => "Unidade",                  "type" => $rel::STRING);
$col["suonome"]               = array("campo" => "suonome",              "alias" => "suo",          "label" => "Subunidade",               "type" => $rel::STRING);
$col["ptres"]                 = array("campo" => "ptres",                "alias" => "ptr",          "label" => "PTRES",                    "type" => $rel::STRING);
$col["funcional"]             = array("campo" => "funcional",            "alias" => "ptr",          "label" => "Funcional",                "type" => $rel::STRING);
$col["ptrdotacao_custeio"]    = array("campo" => "ptrdotacao_custeio",   "alias" => "psu",          "label" => "Dotação Custeio",          "type" => $rel::CURRENCY);
$col["picvalorcusteio"]       = array("campo" => "picvalorcusteio",      "alias" => "pic",          "label" => "Planejado Custeio",        "type" => $rel::CURRENCY);
$col["ptrdotacao_capital"]    = array("campo" => "ptrdotacao_capital",   "alias" => "psu",          "label" => "Dotação Capital",          "type" => $rel::CURRENCY);
$col["picvalorcapital"]       = array("campo" => "picvalorcapital",      "alias" => "pic",          "label" => "Planejado Capital",        "type" => $rel::CURRENCY);
$col["ptrdotacao_total"]      = array("campo" => "ptrdotacao_total",     "alias" => "pit",          "label" => "Dotação Total",            "type" => $rel::CURRENCY);
$col["picvalorcapital"]       = array("campo" => "picvalorcapital_total","alias" => "pit",          "label" => "Planejado Total",          "type" => $rel::CURRENCY);
$col["provisionado"]          = array("campo" => "provisionado",         "alias" => "sec",          "label" => "Provisionado",             "type" => $rel::CURRENCY);
$col["empenhado"]             = array("campo" => "empenhado",            "alias" => "sec",          "label" => "Empenhado",                "type" => $rel::CURRENCY);
$col["liquidado"]             = array("campo" => "liquidado",            "alias" => "sec",          "label" => "Liquidado",                "type" => $rel::CURRENCY);
$col["pago"]                  = array("campo" => "pago",                 "alias" => "sec",          "label" => "Pago",                     "type" => $rel::CURRENCY);

$join = array();
$join[] = array("campo" => "unosigla",                 "join" => array(),                                                                                    "where" => array(" pro.prostatus='A'"));
$join[] = array("campo" => "suonome",                  "join" => array(),                                                                                    "where" => array(" prd.prdstatus='A'"));
$join[] = array("campo" => "ptres",                    "join" => array("JOIN spo.ptressubunidade psu on psu.suoid = suo.suoid",
                                                                       "JOIN monitora.vw_ptres ptr on ptr.ptrid = psu.ptrid AND ptr.ptrano = suo.prsano"),   "where" => array(" prd.prdstatus='A'"));
$join[] = array("campo" => "ptrdotacao_custeio",       "join" => array("JOIN spo.ptressubunidade psu on psu.suoid = suo.suoid"),                             "where" => array(" pro.prostatus='A'"));    
$join[] = array("campo" => "picvalorcusteio",          "join" => array("JOIN spo.ptressubunidade psu on psu.suoid = suo.suoid",
                                                                       "JOIN monitora.vw_ptres ptr on ptr.ptrid = psu.ptrid AND ptr.ptrano = suo.prsano",
                                                                       "LEFT JOIN monitora.pi_planointernoptres ppt on ppt.ptrid = ptr.ptrid",
                                                                       "LEFT JOIN monitora.pi_planointerno pli on(pli.pliid = ppt.pliid AND pli.ungcod = suo.suocod AND pli.unicod = suo.unocod AND plistatus = 'A')", 
                                                                       "LEFT JOIN planacomorc.pi_complemento pic on pic.pliid = pli.pliid"),                  "where" => array(" pro.prostatus='A'"));
$join[] = array("campo" => "ptrdotacao_capital",       "join" => array("JOIN spo.ptressubunidade psu on psu.suoid = suo.suoid"),                              "where" => array(" pro.prostatus='A'"));    
$join[] = array("campo" => "picvalorcapital",          "join" => array("JOIN spo.ptressubunidade psu on psu.suoid = suo.suoid",
                                                                       "JOIN monitora.vw_ptres ptr on ptr.ptrid = psu.ptrid AND ptr.ptrano = suo.prsano",
                                                                       "LEFT JOIN monitora.pi_planointernoptres ppt on ppt.ptrid = ptr.ptrid",
                                                                       "LEFT JOIN monitora.pi_planointerno pli on(pli.pliid = ppt.pliid AND pli.ungcod = suo.suocod AND pli.unicod = suo.unocod AND plistatus = 'A')", 
                                                                       "LEFT JOIN planacomorc.pi_complemento pic on pic.pliid = pli.pliid"),                   "where" => array(" pro.prostatus='A'"));    
$join[] = array("campo" => "ptrdotacao_total",         "join" => array("JOIN spo.ptressubunidade psu on psu.suoid = suo.suoid"),                               "where" => array(" pro.prostatus='A'"));    
$join[] = array("campo" => "picvalorcapital_total",    "join" => array("JOIN monitora.vw_ptres ptr ON pro.ptrid = ptr.ptrid"),                                 "where" => array(" pro.prostatus='A'"));    
$join[] = array("campo" => "provisionado",             "join" => array("JOIN spo.ptressubunidade psu on psu.suoid = suo.suoid",
                                                                       "JOIN monitora.vw_ptres ptr on ptr.ptrid = psu.ptrid AND ptr.ptrano = suo.prsano",
                                                                       "LEFT JOIN(SELECT
                                                                                    siopexecucao.unicod,
                                                                                    pi_planointerno.ungcod,
                                                                                    siopexecucao.ptres,
                                                                                    SUM(COALESCE(siopexecucao.vlrautorizado, 0.00))::NUMERIC AS provisionado,
                                                                                    SUM(COALESCE(siopexecucao.vlrempenhado, 0.00))::NUMERIC AS empenhado,
                                                                                    SUM(COALESCE(siopexecucao.vlrliquidado, 0.00))::NUMERIC AS liquidado,
                                                                                    SUM(COALESCE(siopexecucao.vlrpago, 0.00))::NUMERIC AS pago
                                                                                FROM spo.siopexecucao
                                                                                    LEFT JOIN monitora.pi_planointerno ON(siopexecucao.plicod = pi_planointerno.plicod 
                                                                                    AND siopexecucao.exercicio = pi_planointerno.pliano 
                                                                                    AND pi_planointerno.plistatus = 'A')
                                                                                WHERE
                                                                                    pi_planointerno.ungcod IS NOT NULL
                                                                                    AND siopexecucao.exercicio = '2018' -- Inserindo o ano direto na subquery por motivo de performance da consulta.
                                                                                GROUP BY
                                                                                    siopexecucao.ptres,
                                                                                    siopexecucao.unicod,
                                                                                    pi_planointerno.ungcod
                                                                            ) sec ON(ptr.ptres = sec.ptres 
                                                                            AND suo.unocod = sec.unicod 
                                                                            AND suo.suocod = sec.ungcod)"),                                                    "where" => array(" pro.prostatus='A'"));    
$join[] = array("campo" => "empenhado",                "join" => array("JOIN spo.ptressubunidade psu on psu.suoid = suo.suoid",
                                                                       "JOIN monitora.vw_ptres ptr on ptr.ptrid = psu.ptrid AND ptr.ptrano = suo.prsano",
                                                                       "LEFT JOIN(SELECT
                                                                                    siopexecucao.unicod,
                                                                                    pi_planointerno.ungcod,
                                                                                    siopexecucao.ptres,
                                                                                    SUM(COALESCE(siopexecucao.vlrautorizado, 0.00))::NUMERIC AS provisionado,
                                                                                    SUM(COALESCE(siopexecucao.vlrempenhado, 0.00))::NUMERIC AS empenhado,
                                                                                    SUM(COALESCE(siopexecucao.vlrliquidado, 0.00))::NUMERIC AS liquidado,
                                                                                    SUM(COALESCE(siopexecucao.vlrpago, 0.00))::NUMERIC AS pago
                                                                                FROM spo.siopexecucao
                                                                                    LEFT JOIN monitora.pi_planointerno ON(siopexecucao.plicod = pi_planointerno.plicod 
                                                                                    AND siopexecucao.exercicio = pi_planointerno.pliano 
                                                                                    AND pi_planointerno.plistatus = 'A')
                                                                                WHERE
                                                                                    pi_planointerno.ungcod IS NOT NULL
                                                                                    AND siopexecucao.exercicio = '2018' -- Inserindo o ano direto na subquery por motivo de performance da consulta.
                                                                                GROUP BY
                                                                                    siopexecucao.ptres,
                                                                                    siopexecucao.unicod,
                                                                                    pi_planointerno.ungcod
                                                                            ) sec ON(ptr.ptres = sec.ptres 
                                                                            AND suo.unocod = sec.unicod 
                                                                            AND suo.suocod = sec.ungcod)"),                                                     "where" => array(" prd.prdstatus='A'",
                                                                                                                                                                "pro.prostatus='A'"));    
$join[] = array("campo" => "liquidado",                "join" => array("JOIN spo.ptressubunidade psu on psu.suoid = suo.suoid",
                                                                       "JOIN monitora.vw_ptres ptr on ptr.ptrid = psu.ptrid AND ptr.ptrano = suo.prsano",
                                                                       "LEFT JOIN(SELECT
                                                                                    siopexecucao.unicod,
                                                                                    pi_planointerno.ungcod,
                                                                                    siopexecucao.ptres,
                                                                                    SUM(COALESCE(siopexecucao.vlrautorizado, 0.00))::NUMERIC AS provisionado,
                                                                                    SUM(COALESCE(siopexecucao.vlrempenhado, 0.00))::NUMERIC AS empenhado,
                                                                                    SUM(COALESCE(siopexecucao.vlrliquidado, 0.00))::NUMERIC AS liquidado,
                                                                                    SUM(COALESCE(siopexecucao.vlrpago, 0.00))::NUMERIC AS pago
                                                                                FROM spo.siopexecucao
                                                                                    LEFT JOIN monitora.pi_planointerno ON(siopexecucao.plicod = pi_planointerno.plicod 
                                                                                    AND siopexecucao.exercicio = pi_planointerno.pliano 
                                                                                    AND pi_planointerno.plistatus = 'A')
                                                                                WHERE
                                                                                    pi_planointerno.ungcod IS NOT NULL
                                                                                    AND siopexecucao.exercicio = '2018' -- Inserindo o ano direto na subquery por motivo de performance da consulta.
                                                                                GROUP BY
                                                                                    siopexecucao.ptres,
                                                                                    siopexecucao.unicod,
                                                                                    pi_planointerno.ungcod
                                                                            ) sec ON(ptr.ptres = sec.ptres 
                                                                            AND suo.unocod = sec.unicod 
                                                                            AND suo.suocod = sec.ungcod)"),                                                     "where" => array(" prd.prdstatus='A'",
                                                                                                                                                                "pro.prostatus='A'"));    
$join[] = array("campo" => "pago",                      "join" => array("JOIN spo.ptressubunidade psu on psu.suoid = suo.suoid",
                                                                       "JOIN monitora.vw_ptres ptr on ptr.ptrid = psu.ptrid AND ptr.ptrano = suo.prsano",
                                                                       "LEFT JOIN(SELECT
                                                                                    siopexecucao.unicod,
                                                                                    pi_planointerno.ungcod,
                                                                                    siopexecucao.ptres,
                                                                                    SUM(COALESCE(siopexecucao.vlrautorizado, 0.00))::NUMERIC AS provisionado,
                                                                                    SUM(COALESCE(siopexecucao.vlrempenhado, 0.00))::NUMERIC AS empenhado,
                                                                                    SUM(COALESCE(siopexecucao.vlrliquidado, 0.00))::NUMERIC AS liquidado,
                                                                                    SUM(COALESCE(siopexecucao.vlrpago, 0.00))::NUMERIC AS pago
                                                                                FROM spo.siopexecucao
                                                                                    LEFT JOIN monitora.pi_planointerno ON(siopexecucao.plicod = pi_planointerno.plicod 
                                                                                    AND siopexecucao.exercicio = pi_planointerno.pliano 
                                                                                    AND pi_planointerno.plistatus = 'A')
                                                                                WHERE
                                                                                    pi_planointerno.ungcod IS NOT NULL
                                                                                    AND siopexecucao.exercicio = '2018' -- Inserindo o ano direto na subquery por motivo de performance da consulta.
                                                                                GROUP BY
                                                                                    siopexecucao.ptres,
                                                                                    siopexecucao.unicod,
                                                                                    pi_planointerno.ungcod
                                                                            ) sec ON(ptr.ptres = sec.ptres 
                                                                            AND suo.unocod = sec.unicod 
                                                                            AND suo.suocod = sec.ungcod)"),                                                         "where" => array(" pro.prostatus='A'"));
$rel->setTabelaBase('public.vw_subunidadeorcamentaria suo');
$rel->setListaColunas($col);
$rel->setListaJoins($join);
$rel->db=$db;
$rel->MontaSelect();
// Gera o XLS do relatório
if ( $_REQUEST['req'] == 'xls' ){
    ob_clean();
    $nomeDoArquivoXls = 'relatorio';
    echo $rel->getRelatorioXls();
    die();
}
include APPRAIZ . "includes/cabecalho_bootstrap_v3_popup.inc";
$where = '';
?>
<?php echo $rel->getRelatorio(); ?>
</div>
</div>
</body>
</html>