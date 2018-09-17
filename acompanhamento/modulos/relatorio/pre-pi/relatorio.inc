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
    $col["pliid"]                     = array("campo" => "pli.pliid",                 "alias", "label" => "Id Pré-PI",                  "type" => $rel::NUMERIC);
    $col["plititulo"]                 = array("campo" => "plititulo",                 "alias", "label" => "Título",                     "type" => $rel::STRING);
    $col["plidsc"]                    = array("campo" => "plidsc",                    "alias", "label" => "Descrição/Finalidade",       "type" => $rel::STRING);
    $col["subunidade"]                = array("campo" => "suo.unosigla || ' - ' || suo.suonome subunidade",                "alias", "label" => "Subunidade",                 "type" => $rel::STRING);
    $col["eqddsc"]                    = array("campo" => "eqddsc",                    "alias", "label" => "Enquadramento da Despesa",   "type" => $rel::STRING);
    $col["irpcod"]                    = array("campo" => "irpcod",                    "alias", "label" => "RP",                         "type" => $rel::NUMERIC);
    $col["mainome"]                   = array("campo" => "mainome",                   "alias", "label" => "Item",                       "type" => $rel::STRING);
    $col["masnome"]                   = array("campo" => "masnome",                   "alias", "label" => "Subitem",                    "type" => $rel::STRING);
    $col["funcional"]                 = array("campo" => "funcional",                 "alias", "label" => "Funcional",                  "type" => $rel::CURRENCY);
    $col["acatitulo"]                 = array("campo" => "acatitulo",                 "alias", "label" => "Ação",                       "type" => $rel::STRING);
    $col["plodsc"]                    = array("campo" => "plodsc",                    "alias", "label" => "PO",                         "type" => $rel::STRING);
    $col["esddsc"]                    = array("campo" => "esddsc",                    "alias", "label" => "Situação",                   "type" => $rel::STRING);
    $col["pprnome"]                   = array("campo" => "pprnome",                   "alias", "label" => "Produto",                    "type" => $rel::STRING);
    $col["pumdescricao"]              = array("campo" => "pumdescricao",              "alias", "label" => "Unidade de Medida",          "type" => $rel::STRING);
    $col["pliquantidade"]             = array("campo" => "pliquantidade",             "alias", "label" => "Quantidade",                 "type" => $rel::NUMERIC);
    $col["oppcod"]                    = array("campo" => "oppcod",                    "alias", "label" => "Código Objetivo PPA",        "type" => $rel::NUMERIC);
    $col["oppdsc"]                    = array("campo" => "oppdsc",                    "alias", "label" => "Objetivo PPA",               "type" => $rel::STRING);
    $col["mppcod"]                    = array("campo" => "mppcod",                    "alias", "label" => "Código Metas PPA",           "type" => $rel::STRING);
    $col["mppnome"]                   = array("campo" => "mppnome",                   "alias", "label" => "Metas PPA",                  "type" => $rel::STRING);
    $col["ippcod"]                    = array("campo" => "ippcod",                    "alias", "label" => "Código Iniciativa PPA",      "type" => $rel::STRING);
    $col["ippnome"]                   = array("campo" => "ippnome",                   "alias", "label" => "Iniciativa PPA",             "type" => $rel::STRING);
    $col["mpncod"]                    = array("campo" => "mpncod",                    "alias", "label" => "Código Meta PNC",            "type" => $rel::NUMERIC);
    $col["mpnnome"]                   = array("campo" => "mpnnome",                   "alias", "label" => "Meta PNC",                   "type" => $rel::STRING);
    $col["ipncod"]                    = array("campo" => "ipncod",                    "alias", "label" => "Código Indicador PNC",       "type" => $rel::STRING);
    $col["ipndsc"]                    = array("campo" => "ipndsc",                    "alias", "label" => "Indicador PNC",              "type" => $rel::STRING);
    $col["mdedsc"]                    = array("campo" => "mdedsc",                    "alias", "label" => "Área Cultural",              "type" => $rel::STRING);
    $col["needsc"]                    = array("campo" => "needsc",                    "alias", "label" => "Segmento Cultural",          "type" => $rel::STRING);
    $col["esfdsc"]                    = array("campo" => "esfdsc",                    "alias", "label" => "Localização",                "type" => $rel::STRING);
    $col["paidescricao"]              = array("campo" => "paidescricao",              "alias", "label" => "País",                       "type" => $rel::STRING);
    $col["estuf"]                     = array("campo" => "estuf",                     "alias", "label" => "UF",                         "type" => $rel::STRING);
    $col["estdescricao"]              = array("campo" => "estdescricao",              "alias", "label" => "Estado",                     "type" => $rel::STRING);
    $col["munestuf"]                  = array("campo" => "munestuf",                  "alias", "label" => "UF do Município",            "type" => $rel::STRING);
    $col["mundescricao"]              = array("campo" => "mundescricao",              "alias", "label" => "Município",                  "type" => $rel::STRING);
    $col["plivalorcusteio"]           = array("campo" => "plivalorcusteio",           "alias", "label" => "Custeio",                    "type" => $rel::CURRENCY);
    $col["plivalorcapital"]           = array("campo" => "plivalorcapital",           "alias", "label" => "Capital",                    "type" => $rel::CURRENCY);
    $col["pliquantidadeadicional"]    = array("campo" => "pliquantidadeadicional",    "alias", "label" => "Adicional de Quantidade",    "type" => $rel::CURRENCY);
    $col["plivalorcusteioadicional"]  = array("campo" => "plivalorcusteioadicional",  "alias", "label" => "Adicional de Valor Custeio", "type" => $rel::CURRENCY);
    $col["plivalorcapitaladicional"]  = array("campo" => "plivalorcapitaladicional",  "alias", "label" => "Adicional de Valor Capital", "type" => $rel::CURRENCY);
    $col["plijustificativaadicional"] = array("campo" => "plijustificativaadicional", "alias", "label" => "Justificativa do Adicional",  "type" => $rel::STRING);

$join = array();

    $join[] = array("campo" => "pliid",                     "join" => array());
    $join[] = array("campo" => "plititulo",                 "join" => array());
    $join[] = array("campo" => "plidsc",                    "join" => array());
    $join[] = array("campo" => "subunidade",                "join" => array("JOIN public.vw_subunidadeorcamentaria suo ON suo.suoid = pli.suoid"), "where" => array(" pli.prsano = '2019'"));
    $join[] = array("campo" => "eqddsc",                    "join" => array("JOIN monitora.pi_enquadramentodespesa eqd ON eqd.eqdid = pli.eqdid"), "where" => array());
    $join[] = array("campo" => "irpcod",                    "join" => array("JOIN monitora.vw_ptres ptr ON pli.ptrid = ptr.ptrid"), "where" => array());
    $join[] = array("campo" => "mainome",                   "join" => array("LEFT JOIN planacomorc.manutencaoitem mai ON pli.maiid = mai.maiid"), "where" => array());
    $join[] = array("campo" => "masnome",                   "join" => array("LEFT JOIN planacomorc.manutencaosubitem mas ON pli.masid = mas.masid"), "where" => array());
    $join[] = array("campo" => "funcional",                 "join" => array("JOIN monitora.vw_ptres ptr ON pli.ptrid = ptr.ptrid"), "where" => array());
    $join[] = array("campo" => "acatitulo",                 "join" => array("JOIN monitora.vw_ptres ptr ON pli.ptrid = ptr.ptrid"), "where" => array());
    $join[] = array("campo" => "plodsc",                    "join" => array("JOIN monitora.vw_ptres ptr ON pli.ptrid = ptr.ptrid"), "where" => array());
    $join[] = array("campo" => "esddsc",                    "join" => array("LEFT JOIN workflow.documento doc ON doc.docid = pli.docid",
                                                                            "LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid"), "where" => array());
    $join[] = array("campo" => "pprnome",                   "join" => array("LEFT JOIN monitora.pi_produto ppr ON pli.pprid = ppr.pprid"), "where" => array());
    $join[] = array("campo" => "pumdescricao",              "join" => array("LEFT JOIN monitora.pi_unidade_medida pum ON pli.pumid = pum.pumid"), "where" => array());
    $join[] = array("campo" => "pliquantidade",             "join" => array(),  "where" => array("pro.prostatus='A'"));
    $join[] = array("campo" => "oppcod",                    "join" => array("LEFT JOIN public.objetivoppa opp ON pli.oppid = opp.oppid"), "where" => array());
    $join[] = array("campo" => "oppdsc",                    "join" => array("LEFT JOIN public.objetivoppa opp ON pli.oppid = opp.oppid"), "where" => array());
    $join[] = array("campo" => "mppcod",                    "join" => array("LEFT JOIN public.metappa mpp ON pli.mppid = mpp.mppid"), "where" => array());
    $join[] = array("campo" => "mppnome",                   "join" => array("LEFT JOIN public.metappa mpp ON pli.mppid = mpp.mppid"), "where" => array());
    $join[] = array("campo" => "ippcod",                    "join" => array("LEFT JOIN public.iniciativappa ipp ON pli.ippid = ipp.ippid"), "where" => array());
    $join[] = array("campo" => "ippnome",                   "join" => array("LEFT JOIN public.iniciativappa ipp ON pli.ippid = ipp.ippid"), "where" => array());
    $join[] = array("campo" => "ipncod",                    "join" => array("LEFT JOIN public.indicadorpnc ipn ON pli.ipnid = ipn.ipnid"), "where" => array());
    $join[] = array("campo" => "ipndsc",                    "join" => array("LEFT JOIN public.indicadorpnc ipn ON pli.ipnid = ipn.ipnid"), "where" => array());
    $join[] = array("campo" => "mpncod",                    "join" => array("LEFT JOIN public.metapnc mpn ON pli.mpnid = mpn.mpnid"), "where" => array());
    $join[] = array("campo" => "mpnnome",                   "join" => array("LEFT JOIN public.iniciativappa ipp ON pli.ippid = ipp.ippid"), "where" => array());
    $join[] = array("campo" => "mdedsc",                    "join" => array("LEFT JOIN monitora.pi_modalidadeensino mde ON pli.mdeid = mde.mdeid"), "where" => array());
    $join[] = array("campo" => "needsc",                    "join" => array("LEFT JOIN monitora.pi_niveletapaensino nee ON pli.neeid = nee.neeid"), "where" => array());
    $join[] = array("campo" => "esfdsc",                    "join" => array("LEFT JOIN territorios.esfera esf ON pli.esfid = esf.esfid"), "where" => array());
    $join[] = array("campo" => "paidescricao",              "join" => array(
                                                                            "LEFT JOIN proposta.preplanointernolocalizacao plo ON pli.pliid = plo.pliid",
                                                                            "LEFT JOIN territorios.pais pai ON plo.paiid = pai.paiid"), "where" => array());

    $join[] = array("campo" => "estuf",                     "join" => array("LEFT JOIN proposta.preplanointernolocalizacao plo ON pli.pliid = plo.pliid",
                                                                            "LEFT JOIN territorios.estado est ON plo.estuf = est.estuf"), "where" => array());

    $join[] = array("campo" => "estdescricao",              "join" => array("LEFT JOIN proposta.preplanointernolocalizacao plo ON pli.pliid = plo.pliid",
                                                                            "LEFT JOIN territorios.estado est ON plo.estuf = est.estuf"), "where" => array());

    $join[] = array("campo" => "munestuf",                  "join" => array("LEFT JOIN proposta.preplanointernolocalizacao plo ON pli.pliid = plo.pliid",
                                                                            "LEFT JOIN territorios.municipio mun ON plo.muncod = mun.muncod"), "where" => array());

    $join[] = array("campo" => "mundescricao",              "join" => array("LEFT JOIN proposta.preplanointernolocalizacao plo ON pli.pliid = plo.pliid",
                                                                            "LEFT JOIN territorios.municipio mun ON plo.muncod = mun.muncod"), "where" => array());

    $join[] = array("campo" => "plivalorcusteio",           "join" => array(), "where" => array("pro.prostatus='A'"));
    $join[] = array("campo" => "plivalorcapital",           "join" => array(), "where" => array("pro.prostatus='A'"));
    $join[] = array("campo" => "pliquantidadeadicional",    "join" => array(), "where" => array("pro.prostatus='A'"));
    $join[] = array("campo" => "plivalorcusteioadicional",  "join" => array(), "where" => array("pro.prostatus='A'"));
    $join[] = array("campo" => "plivalorcapitaladicional",  "join" => array(), "where" => array("pro.prostatus='A'"));
    $join[] = array("campo" => "plijustificativaadicional", "join" => array(), "where" => array("pro.prostatus='A'"));

$rel->setTabelaBase('proposta.preplanointerno pli');
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

 echo $rel->getRelatorio(); ?>

</div>
</div>
</body>
</html>