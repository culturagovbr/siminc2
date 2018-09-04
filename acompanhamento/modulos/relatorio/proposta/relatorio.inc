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
$rel->setTipoRelatorio("proposta");

//Lista de colunas, Agrupadores e joins
$col = array();
$col["locquantidadeproposta"]    = array("campo" => "locquantidadeproposta",    "alias" => "aca", "label" => "Quantidade Localizador",   "type" => $rel::NUMERIC);
$col["prdvalor"]                 = array("campo" => "prdvalor",                 "alias" => "prd", "label" => "Valor",                    "type" => $rel::CURRENCY);
$col["prdvalorexpansao"]         = array("campo" => "prdvalorexpansao",         "alias" => "prd", "label" => "Valor Expansão",           "type" => $rel::CURRENCY);
$col["proquantidade"]            = array("campo" => "proquantidade",            "alias" => "pro", "label" => "Quantidade PO",            "type" => $rel::NUMERIC);
$col["proquantidadeexpansao"]    = array("campo" => "proquantidadeexpansao",    "alias" => "pro", "label" => "Quantidade Expansão PO",   "type" => $rel::NUMERIC);
$col["projustificativa"]         = array("campo" => "projustificativa",         "alias" => "pro", "label" => "Justificativa",            "type" => $rel::STRING);
$col["projustificativaexpansao"] = array("campo" => "projustificativaexpansao", "alias" => "pro", "label" => "Justificativa Expansão",   "type" => $rel::STRING);
$col["suonome"]                  = array("campo" => "suonome",                  "alias" => "suo", "label" => "Subunidade",               "type" => $rel::STRING);
$col["eqddsc"]                   = array("campo" => "eqddsc",                   "alias" => "eqd", "label" => "Enquadramento da Despesa", "type" => $rel::STRING);
$col["irpcod"]                   = array("campo" => "irpcod",                   "alias" => "ptr", "label" => "RP",                       "type" => $rel::STRING);
$col["funcional"]                = array("campo" => "funcional",                "alias" => "ptr", "label" => "Funcional",                "type" => $rel::STRING);
$col["acatitulo"]                = array("campo" => "acatitulo",                "alias" => "ptr", "label" => "Ação",                     "type" => $rel::STRING);
$col["plodsc"]                   = array("campo" => "plodsc",                   "alias" => "ptr", "label" => "PO",                       "type" => $rel::STRING);
$col["ndpid"]                    = array("campo" => "ndpid",                    "alias" => "prd", "label" => "Natureza de Despesa",      "type" => $rel::STRING);
$col["iducod"]                   = array("campo" => "iducod",                   "alias" => "idu", "label" => "IDUSO",                    "type" => $rel::STRING);
$col["foncod"]                   = array("campo" => "foncod",                   "alias" => "fr",  "label" => "Fonte",                    "type" => $rel::STRING);
$col["idoid"]                    = array("campo" => "idoid",                    "alias" => "prd", "label" => "IDOC",                     "type" => $rel::STRING);
$col["proid"]                    = array("campo" => "proid",                    "alias" => "pro", "label" => "ID Proposta",              "type"	=> $rel::STRING);
$col["prsano"]                   = array("campo" => "prsano",                   "alias" => "pro", "label" => "Ano",                      "type"	=> $rel::STRING);
$col["suoid"]                    = array("campo" => "suoid",                    "alias" => "suo", "label" => "SUOID",                    "type"	=> $rel::STRING);
$col["eqdid"]                    = array("campo" => "eqdid",                    "alias" => "eqd", "label" => "Enquadramento ID",         "type"	=> $rel::STRING);
$col["irpcod"]                   = array("campo" => "irpcod",                   "alias" => "ptr", "label" => "RP ID",                    "type"	=> $rel::STRING);
$col["tpdid"]                    = array("campo" => "tpdid",                    "alias" => "eqd", "label" => "Tipo de Detalhamento",     "type"	=> $rel::STRING);

$join = array();
$join[] = array("campo" => "locquantidadeproposta",    "join" => array("JOIN monitora.vw_ptres ptr ON pro.ptrid = ptr.ptrid",
                                                                       "JOIN monitora.acao aca ON ptr.acaid = aca.acaid"),                    "where" => array(" pro.prostatus='A'"));
$join[] = array("campo" => "prdvalor",                 "join" => array("LEFT JOIN proposta.propostadetalhe prd ON prd.proid = pro.proid"),    "where" => array(" prd.prdstatus='A'",
                                                                                                                                                               " pro.prostatus='A'"));
$join[] = array("campo" => "prdvalorexpansao",         "join" => array("LEFT JOIN proposta.propostadetalhe prd ON prd.proid = pro.proid"),    "where" => array(" prd.prdstatus='A'",
                                                                                                                                                               " pro.prostatus='A'"));
$join[] = array("campo" => "proquantidade",            "join" => array(),                                                                     "where" => array(" pro.prostatus='A'"));
$join[] = array("campo" => "proquantidadeexpansao",    "join" => array(),                                                                     "where" => array(" pro.prostatus='A'"));
$join[] = array("campo" => "projustificativa",         "join" => array(),                                                                     "where" => array(" pro.prostatus='A'"));
$join[] = array("campo" => "projustificativaexpansao", "join" => array(),                                                                     "where" => array(" pro.prostatus='A'"));    
$join[] = array("campo" => "proid",                    "join" => array(),                                                                     "where" => array(" pro.prostatus='A'"));    
$join[] = array("campo" => "suonome",                  "join" => array("JOIN public.vw_subunidadeorcamentaria suo ON suo.suoid = pro.suoid"), "where" => array(" pro.prostatus='A'"));    
$join[] = array("campo" => "eqddsc",                   "join" => array("JOIN monitora.pi_enquadramentodespesa eqd ON eqd.eqdid = pro.eqdid"), "where" => array(" pro.prostatus='A'"));    
$join[] = array("campo" => "tpdid",                    "join" => array("JOIN monitora.pi_enquadramentodespesa eqd ON eqd.eqdid = pro.eqdid"), "where" => array(" pro.prostatus='A'"));    
$join[] = array("campo" => "irpcod",                   "join" => array("JOIN monitora.vw_ptres ptr ON pro.ptrid = ptr.ptrid"),                "where" => array(" pro.prostatus='A'"));    
$join[] = array("campo" => "funcional",                "join" => array("JOIN monitora.vw_ptres ptr ON pro.ptrid = ptr.ptrid"),                "where" => array(" pro.prostatus='A'"));    
$join[] = array("campo" => "acatitulo",                "join" => array("JOIN monitora.vw_ptres ptr ON pro.ptrid = ptr.ptrid"),                "where" => array(" pro.prostatus='A'"));    
$join[] = array("campo" => "ndpid",                    "join" => array("LEFT JOIN proposta.propostadetalhe prd ON prd.proid = pro.proid"),    "where" => array(" prd.prdstatus='A'",
                                                                                                                                                               " pro.prostatus='A'"));    
$join[] = array("campo" => "iducod",                   "join" => array("LEFT JOIN proposta.propostadetalhe prd ON prd.proid = pro.proid",
                                                                       "LEFT JOIN public.identifuso idu on prd.iduid = idu.iduid"),           "where" => array(" prd.prdstatus='A'",
                                                                                                                                                               " pro.prostatus='A'"));    
$join[] = array("campo" => "foncod",                   "join" => array("LEFT JOIN proposta.propostadetalhe prd ON prd.proid = pro.proid",
                                                                       "LEFT JOIN public.fonterecurso fr on prd.fonid = fr.fonid"),           "where" => array(" pro.prostatus='A'")); 
$join[] = array("campo" => "plodsc",                   "join" => array("JOIN monitora.vw_ptres ptr ON pro.ptrid = ptr.ptrid"),                "where" => array(" pro.prostatus='A'"));    
$join[] = array("campo" => "idoid",                    "join" => array("LEFT JOIN proposta.propostadetalhe prd ON prd.proid = pro.proid"),    "where" => array(" prd.prdstatus='A'",
                                                                                                                                                               " pro.prostatus='A'"));  
$join[] = array("campo" => "suoid",                    "join" => array("JOIN public.vw_subunidadeorcamentaria suo ON suo.suoid = pro.suoid"), "where" => array(" pro.prostatus='A'"));  
$join[] = array("campo" => "eqdid",                    "join" => array("JOIN monitora.pi_enquadramentodespesa eqd ON eqd.eqdid = pro.eqdid"), "where" => array(" pro.prostatus='A'"));    
$rel->setTabelaBase('proposta.proposta pro');
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