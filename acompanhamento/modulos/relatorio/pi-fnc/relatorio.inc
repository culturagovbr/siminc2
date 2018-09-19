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
$rel->setTipoRelatorio("pi-fnc");

//Lista de colunas, Agrupadores e joins
$col = array();
$col["esddsc"]                          = array("campo" => "esddsc",                    "alias" => "aca", "label" => "Situação",                        "type" => $rel::NUMERIC);
$col["plicod"]                          = array("campo" => "plicod",                    "alias" => "prd", "label" => "Código PI",                    "type" => $rel::CURRENCY);
$col["unocod"]                          = array("campo" => "unocod",                    "alias" => "prd", "label" => "Código da UO",                 "type" => $rel::CURRENCY);
$col["unonome"]                         = array("campo" => "unonome",                   "alias" => "pro", "label" => "UO",                           "type" => $rel::NUMERIC);
$col["suocod"]                          = array("campo" => "suocod",                    "alias" => "pro", "label" => "Código da Unidade",            "type" => $rel::NUMERIC);
$col["suonome"]                         = array("campo" => "suonome",                   "alias" => "pro", "label" => "Unidade",                         "type" => $rel::STRING);
$col["unodelegada"]                     = array("campo" => "unodelegada",               "alias" => "pro", "label" => "Unidade Delegada",                 "type" => $rel::STRING);
$col["suodelegada"]                     = array("campo" => "suodelegada",               "alias" => "suo", "label" => "Unidade Delegada",                    "type" => $rel::STRING);
$col["eqddsc"]                          = array("campo" => "eqddsc",                    "alias" => "eqd", "label" => "Enquadramento",                       "type" => $rel::STRING);
$col["resultadoprimario"]               = array("campo" => "resultadoprimario",         "alias" => "ptr", "label" => "RP - Resultado Primário",                       "type" => $rel::STRING);
$col["mainome"]                         = array("campo" => "mainome",                   "alias" => "ptr", "label" => "Item de Manutenção",                  "type" => $rel::STRING);
$col["masnome"]                         = array("campo" => "masnome",                   "alias" => "ptr", "label" => "Subitem de Manutenção",                     "type" => $rel::STRING);
$col["prgcod"]                          = array("campo" => "prgcod",                    "alias" => "ptr", "label" => "Código do Programa",                       "type" => $rel::STRING);
$col["prgdsc"]                          = array("campo" => "prgdsc",                    "alias" => "prd", "label" => "Programa",                            "type" => $rel::STRING);
$col["oppcod"]                          = array("campo" => "oppcod",                    "alias" => "idu", "label" => "Código do Objetivo",                    "type" => $rel::STRING);
$col["oppnome"]                         = array("campo" => "oppnome",                   "alias" => "fr",  "label" => "Objetivo",                            "type" => $rel::STRING);
$col["ippcod"]                          = array("campo" => "ippcod",                    "alias" => "prd", "label" => "Código da Iniciativa",                     "type" => $rel::STRING);
$col["ippnome"]                         = array("campo" => "ippnome",                   "alias" => "fr",  "label" => "Iniciativa",                           "type" => $rel::STRING);
$col["acacod"]                          = array("campo" => "acacod",                    "alias" => "fr",  "label" => "Código da Ação",                    "type" => $rel::STRING);
$col["acatitulo"]                       = array("campo" => "acatitulo",                 "alias" => "fr",  "label" => "Ação",                                "type" => $rel::STRING);
$col["loccod"]                          = array("campo" => "loccod",                    "alias" => "fr",  "label" => "Código do Localizador",                    "type" => $rel::STRING);
$col["locdsc"]                          = array("campo" => "locdsc",                    "alias" => "fr",  "label" => "Localizador",                    "type" => $rel::STRING);
$col["plocod"]                          = array("campo" => "plocod",                    "alias" => "fr",  "label" => "Código do PO",                    "type" => $rel::STRING);
$col["po"]                              = array("campo" => "po",                        "alias" => "fr",  "label" => "Plano Orçamentário",                    "type" => $rel::STRING);
$col["ptres"]                           = array("campo" => "ptres",                     "alias" => "fr",  "label" => "PTRES",                    "type" => $rel::STRING);
$col["mdedsc"]                          = array("campo" => "mdedsc",                    "alias" => "fr",  "label" => "Área",                    "type" => $rel::STRING);
$col["mpnnome"]                         = array("campo" => "mpnnome",                   "alias" => "fr",  "label" => "Meta PNC",                    "type" => $rel::STRING);
$col["mppnome"]                         = array("campo" => "mppnome",                   "alias" => "fr",  "label" => "Meta PPA",                    "type" => $rel::STRING);
$col["plititulo"]                       = array("campo" => "plititulo",                 "alias" => "fr",  "label" => "Título",                    "type" => $rel::STRING);
$col["plidsc"]                          = array("campo" => "plidsc",                    "alias" => "fr",  "label" => "Descrição",                    "type" => $rel::STRING);
$col["needsc"]                          = array("campo" => "needsc",                    "alias" => "fr",  "label" => "Segmento Cultural",                    "type" => $rel::STRING);
$col["capdsc"]                          = array("campo" => "capdsc",                    "alias" => "fr",  "label" => "Tipo de Instrumento / Modalidade de Pactuação",                    "type" => $rel::STRING);
$col["pliemenda"]                       = array("campo" => "pliemenda",                 "alias" => "fr",  "label" => "Emenda parlamentar",                    "type" => $rel::STRING);
$col["picedital"]                       = array("campo" => "picedital",                 "alias" => "fr",  "label" => "Forma de execução - Edital",                    "type" => $rel::STRING);
$col["esfdsc"]                          = array("campo" => "esfdsc",                    "alias" => "fr",  "label" => "Localização da ação",                    "type" => $rel::STRING);
$col["pais"]                            = array("campo" => "pais",                      "alias" => "fr",  "label" => "País",                    "type" => $rel::STRING);
$col["estado"]                          = array("campo" => "estado",                    "alias" => "fr",  "label" => "Unidade Federativa",                    "type" => $rel::STRING);
$col["municipio"]                       = array("campo" => "municipio",                 "alias" => "fr",  "label" => "Município",                    "type" => $rel::STRING);
$col["usuario"]                         = array("campo" => "usuario",                   "alias" => "fr",  "label" => "Responsável",                    "type" => $rel::STRING);
$col["ted"]                             = array("campo" => "ted",                       "alias" => "fr",  "label" => "TED",                    "type" => $rel::STRING);

$col["edital"]                          = array("campo" => "edital",                    "alias" => "fr",  "label" => "Edital",                    "type" => $rel::STRING);
$col["mesdsc"]                          = array("campo" => "mesdsc",                    "alias" => "fr",  "label" => "Previsão Edital",                    "type" => $rel::STRING);
$col["convenio"]                        = array("campo" => "convenio",                  "alias" => "fr",  "label" => "Convênio",                    "type" => $rel::STRING);
$col["sniic"]                           = array("campo" => "sniic",                     "alias" => "fr",  "label" => "SNIIC",                    "type" => $rel::STRING);
$col["sei"]                             = array("campo" => "sei",                       "alias" => "fr",  "label" => "Processo",                    "type" => $rel::STRING);
$col["pronac"]                          = array("campo" => "pronac",                    "alias" => "fr",  "label" => "PRONAC",                    "type" => $rel::STRING);
$col["pprnome"]                         = array("campo" => "pprnome",                   "alias" => "fr",  "label" => "Produto PI",                    "type" => $rel::STRING);
$col["pumnome"]                         = array("campo" => "pumnome",                   "alias" => "fr",  "label" => "Unidade de Medida",                    "type" => $rel::STRING);
$col["picquantidade"]                   = array("campo" => "picquantidade",             "alias" => "fr",  "label" => "Meta Física",                    "type" => $rel::STRING);
$col["valortotal"]                      = array("campo" => "valortotal",                "alias" => "fr",  "label" => "Valor Total",                    "type" => $rel::STRING);
$col["picvalorcusteio"]                 = array("campo" => "picvalorcusteio",           "alias" => "fr",  "label" => "Valor Total Custeio",                    "type" => $rel::STRING);
$col["picvalorcapital"]                 = array("campo" => "picvalorcapital",           "alias" => "fr",  "label" => "Valor Total Capital",                    "type" => $rel::STRING);

$col["fisico_1"]                        = array("campo" => "fisico_1",                  "alias" => "fr",  "label" => "Cronograma Físico / Capital Janeiro",                     "type" => $rel::STRING);
$col["fisico_2"]                        = array("campo" => "fisico_2",                  "alias" => "fr",  "label" => "Cronograma Físico / Capital Fevereiro",                   "type" => $rel::STRING);
$col["fisico_3"]                        = array("campo" => "fisico_3",                  "alias" => "fr",  "label" => "Cronograma Físico / Capital Março",                       "type" => $rel::STRING);
$col["fisico_4"]                        = array("campo" => "fisico_4",                  "alias" => "fr",  "label" => "Cronograma Físico / Capital Abril",                       "type" => $rel::STRING);
$col["fisico_5"]                        = array("campo" => "fisico_5",                  "alias" => "fr",  "label" => "Cronograma Físico / Capital Maio",                        "type" => $rel::STRING);
$col["fisico_6"]                        = array("campo" => "fisico_6",                  "alias" => "fr",  "label" => "Cronograma Físico / Capital Junho",                       "type" => $rel::STRING);
$col["fisico_7"]                        = array("campo" => "fisico_7",                  "alias" => "fr",  "label" => "Cronograma Físico / Capital Julho",                       "type" => $rel::STRING);
$col["fisico_8"]                        = array("campo" => "fisico_8",                  "alias" => "fr",  "label" => "Cronograma Físico / Capital Agosto",                    "type" => $rel::STRING);
$col["fisico_9"]                        = array("campo" => "fisico_9",                  "alias" => "fr",  "label" => "Cronograma Físico / Capital Setembro",                    "type" => $rel::STRING);
$col["fisico_10"]                       = array("campo" => "fisico_10",                 "alias" => "fr",  "label" => "Cronograma Físico / Capital Outubro",                    "type" => $rel::STRING);
$col["fisico_11"]                       = array("campo" => "fisico_11",                 "alias" => "fr",  "label" => "Cronograma Físico / Capital Novembro",                    "type" => $rel::STRING);
$col["fisico_12"]                       = array("campo" => "fisico_12",                 "alias" => "fr",  "label" => "Cronograma Físico / Capital Dezembro",                    "type" => $rel::STRING);

$col["fin_capital_1"]                   = array("campo" => "fin_capital_1",             "alias" => "fr",  "label" => "Cronograma Financeiro / Capital Janeiro",                    "type" => $rel::STRING);
$col["fin_capital_2"]                   = array("campo" => "fin_capital_2",             "alias" => "fr",  "label" => "Cronograma Financeiro / Capital Fevereiro",                    "type" => $rel::STRING);
$col["fin_capital_3"]                   = array("campo" => "fin_capital_3",             "alias" => "fr",  "label" => "Cronograma Financeiro / Capital Março",                    "type" => $rel::STRING);
$col["fin_capital_4"]                   = array("campo" => "fin_capital_4",             "alias" => "fr",  "label" => "Cronograma Financeiro / Capital Abril",                    "type" => $rel::STRING);
$col["fin_capital_5"]                   = array("campo" => "fin_capital_5",             "alias" => "fr",  "label" => "Cronograma Financeiro / Capital Maio",                    "type" => $rel::STRING);
$col["fin_capital_6"]                   = array("campo" => "fin_capital_6",             "alias" => "fr",  "label" => "Cronograma Financeiro / Capital Junho",                    "type" => $rel::STRING);
$col["fin_capital_7"]                   = array("campo" => "fin_capital_7",             "alias" => "fr",  "label" => "Cronograma Financeiro / Capital Julho",                    "type" => $rel::STRING);
$col["fin_capital_8"]                   = array("campo" => "fin_capital_8",             "alias" => "fr",  "label" => "Cronograma Financeiro / Capital Agosto",                    "type" => $rel::STRING);
$col["fin_capital_9"]                   = array("campo" => "fin_capital_9",             "alias" => "fr",  "label" => "Cronograma Financeiro / Capital Setembro",                    "type" => $rel::STRING);
$col["fin_capital_10"]                  = array("campo" => "fin_capital_10",            "alias" => "fr",  "label" => "Cronograma Financeiro / Capital Outubro",                    "type" => $rel::STRING);
$col["fin_capital_11"]                  = array("campo" => "fin_capital_11",            "alias" => "fr",  "label" => "Cronograma Financeiro / Capital Novembro",                    "type" => $rel::STRING);
$col["fin_capital_12"]                  = array("campo" => "fin_capital_12",            "alias" => "fr",  "label" => "Cronograma Financeiro / Capital Dezembro",                    "type" => $rel::STRING);

$col["fin_custeio_1"]                   = array("campo" => "fin_custeio_1",              "alias" => "fr",  "label" => "Cronograma Financeiro / Custeio Janeiro",                    "type" => $rel::STRING);
$col["fin_custeio_2"]                   = array("campo" => "fin_custeio_2",              "alias" => "fr",  "label" => "Cronograma Financeiro / Custeio Fevereiro",                    "type" => $rel::STRING);
$col["fin_custeio_3"]                   = array("campo" => "fin_custeio_3",              "alias" => "fr",  "label" => "Cronograma Financeiro / Custeio Março",                    "type" => $rel::STRING);
$col["fin_custeio_4"]                   = array("campo" => "fin_custeio_4",              "alias" => "fr",  "label" => "Cronograma Financeiro / Custeio Abril",                    "type" => $rel::STRING);
$col["fin_custeio_5"]                   = array("campo" => "fin_custeio_5",              "alias" => "fr",  "label" => "Cronograma Financeiro / Custeio Maio",                    "type" => $rel::STRING);
$col["fin_custeio_6"]                   = array("campo" => "fin_custeio_6",              "alias" => "fr",  "label" => "Cronograma Financeiro / Custeio Junho",                    "type" => $rel::STRING);
$col["fin_custeio_7"]                   = array("campo" => "fin_custeio_7",              "alias" => "fr",  "label" => "Cronograma Financeiro / Custeio Julho",                    "type" => $rel::STRING);
$col["fin_custeio_8"]                   = array("campo" => "fin_custeio_8",              "alias" => "fr",  "label" => "Cronograma Financeiro / Custeio Agosto",                    "type" => $rel::STRING);
$col["fin_custeio_9"]                   = array("campo" => "fin_custeio_9",              "alias" => "fr",  "label" => "Cronograma Financeiro / Custeio Setembro",                    "type" => $rel::STRING);
$col["fin_custeio_10"]                  = array("campo" => "fin_custeio_10",             "alias" => "fr",  "label" => "Cronograma Financeiro / Custeio Outubro",                    "type" => $rel::STRING);
$col["fin_custeio_11"]                  = array("campo" => "fin_custeio_11",             "alias" => "fr",  "label" => "Cronograma Financeiro / Custeio Novembro",                    "type" => $rel::STRING);
$col["fin_custeio_12"]                  = array("campo" => "fin_custeio_12",             "alias" => "fr",  "label" => "Cronograma Financeiro / Custeio Dezembro",                    "type" => $rel::STRING);

$col["orc_capital_1"]                   = array("campo" => "orc_capital_1",              "alias" => "fr",  "label" => "Cronograma Orçamentaria / Capital Janeiro",                    "type" => $rel::STRING);
$col["orc_capital_2"]                   = array("campo" => "orc_capital_2",              "alias" => "fr",  "label" => "Cronograma Orçamentaria / Capital Fevereiro",                    "type" => $rel::STRING);
$col["orc_capital_3"]                   = array("campo" => "orc_capital_3",              "alias" => "fr",  "label" => "Cronograma Orçamentaria / Capital Março",                    "type" => $rel::STRING);
$col["orc_capital_4"]                   = array("campo" => "orc_capital_4",              "alias" => "fr",  "label" => "Cronograma Orçamentaria / Capital Abril",                    "type" => $rel::STRING);
$col["orc_capital_5"]                   = array("campo" => "orc_capital_5",              "alias" => "fr",  "label" => "Cronograma Orçamentaria / Capital Maio",                    "type" => $rel::STRING);
$col["orc_capital_6"]                   = array("campo" => "orc_capital_6",              "alias" => "fr",  "label" => "Cronograma Orçamentaria / Capital Junho",                    "type" => $rel::STRING);
$col["orc_capital_7"]                   = array("campo" => "orc_capital_7",              "alias" => "fr",  "label" => "Cronograma Orçamentaria / Capital Julho",                    "type" => $rel::STRING);
$col["orc_capital_8"]                   = array("campo" => "orc_capital_8",              "alias" => "fr",  "label" => "Cronograma Orçamentaria / Capital Agosto",                    "type" => $rel::STRING);
$col["orc_capital_9"]                   = array("campo" => "orc_capital_9",              "alias" => "fr",  "label" => "Cronograma Orçamentaria / Capital Setembro",                    "type" => $rel::STRING);
$col["orc_capital_10"]                  = array("campo" => "orc_capital_10",             "alias" => "fr",  "label" => "Cronograma Orçamentaria / Capital Outubro",                    "type" => $rel::STRING);
$col["orc_capital_11"]                  = array("campo" => "orc_capital_11",             "alias" => "fr",  "label" => "Cronograma Orçamentaria / Capital Novembro",                    "type" => $rel::STRING);
$col["orc_capital_12"]                  = array("campo" => "orc_capital_12",             "alias" => "fr",  "label" => "Cronograma Orçamentaria / Capital Dezembro",                  "type" => $rel::STRING);

$col["orc_custeio_1"]                   = array("campo" => "orc_custeio_1",              "alias" => "fr",  "label" => "Cronograma Orçamentaria / Custeio Janeiro",                    "type" => $rel::STRING);
$col["orc_custeio_2"]                   = array("campo" => "orc_custeio_2",              "alias" => "fr",  "label" => "Cronograma Orçamentaria / Custeio Fevereiro",                    "type" => $rel::STRING);
$col["orc_custeio_3"]                   = array("campo" => "orc_custeio_3",              "alias" => "fr",  "label" => "Cronograma Orçamentaria / Custeio Março",                    "type" => $rel::STRING);
$col["orc_custeio_4"]                   = array("campo" => "orc_custeio_4",              "alias" => "fr",  "label" => "Cronograma Orçamentaria / Custeio Abril",                    "type" => $rel::STRING);
$col["orc_custeio_5"]                   = array("campo" => "orc_custeio_5",              "alias" => "fr",  "label" => "Cronograma Orçamentaria / Custeio Maio",                    "type" => $rel::STRING);
$col["orc_custeio_6"]                   = array("campo" => "orc_custeio_6",              "alias" => "fr",  "label" => "Cronograma Orçamentaria / Custeio Junho",                    "type" => $rel::STRING);
$col["orc_custeio_7"]                   = array("campo" => "orc_custeio_7",              "alias" => "fr",  "label" => "Cronograma Orçamentaria / Custeio Julho",                    "type" => $rel::STRING);
$col["orc_custeio_8"]                   = array("campo" => "orc_custeio_8",              "alias" => "fr",  "label" => "Cronograma Orçamentaria / Custeio Agosto",                    "type" => $rel::STRING);
$col["orc_custeio_9"]                   = array("campo" => "orc_custeio_9",              "alias" => "fr",  "label" => "Cronograma Orçamentaria / Custeio Setembro",                    "type" => $rel::STRING);
$col["orc_custeio_10"]                  = array("campo" => "orc_custeio_10",             "alias" => "fr",  "label" => "Cronograma Orçamentaria / Custeio Outubro",                    "type" => $rel::STRING);
$col["orc_custeio_11"]                  = array("campo" => "orc_custeio_11",             "alias" => "fr",  "label" => "Cronograma Orçamentaria / Custeio Novembro",                    "type" => $rel::STRING);
$col["orc_custeio_12"]                  = array("campo" => "orc_custeio_12",             "alias" => "fr",  "label" => "Cronograma Orçamentaria / Custeio Dezembro",                  "type" => $rel::STRING);

$col["vlrautorizado"]                   = array("campo" => "vlrautorizado",              "alias" => "pro", "label" => "Valor Provisionado",                      "type"	=> $rel::STRING);
$col["vlrempenhado"]                    = array("campo" => "vlrempenhado",               "alias" => "pro", "label" => "Valor Empenhado",                      "type"	=> $rel::STRING);
$col["vlrliquido"]                      = array("campo" => "vlrliquido",                 "alias" => "pro", "label" => "Valor Liquidado",                      "type"	=> $rel::STRING);
$col["vlrpago"]                         = array("campo" => "vlrpago",                    "alias" => "pro", "label" => "Valor Pago",                      "type"	=> $rel::STRING);
$col["executado"]                       = array("campo" => "executado",                  "alias" => "pro", "label" => "Executado",                      "type"	=> $rel::STRING);
$col["analise"]                         = array("campo" => "analise",                    "alias" => "pro", "label" => "Análise Situacional",                      "type"	=> $rel::STRING);
$col["classificacao"]                   = array("campo" => "classificacao",              "alias" => "pro", "label" => "Classificação",                      "type"	=> $rel::STRING);
$col["medidas"]                         = array("campo" => "medidas",                    "alias" => "pro", "label" => "Medidas a serem adotadas",                      "type"	=> $rel::STRING);
$col["providencias"]                    = array("campo" => "providencias",               "alias" => "pro", "label" => "Detalhamento das providências a serem adotadas",                      "type"	=> $rel::STRING);

$col["proid"]                           = array("campo" => "proid",                      "alias" => "pro", "label" => "ID Planejamento",              "type"	=> $rel::STRING);
$col["prsano"]                          = array("campo" => "prsano",                     "alias" => "pro", "label" => "Ano",                      "type"	=> $rel::STRING);
$col["suoid"]                           = array("campo" => "suoid",                      "alias" => "suo", "label" => "SUOID",                    "type"	=> $rel::STRING);
$col["eqdid"]                           = array("campo" => "eqdid",                      "alias" => "eqd", "label" => "Enquadramento ID",         "type"	=> $rel::STRING);
$col["irpcod"]                          = array("campo" => "irpcod",                     "alias" => "ptr", "label" => "RP ID",                    "type"	=> $rel::STRING);
$col["tpdid"]                           = array("campo" => "tpdid",                      "alias" => "eqd", "label" => "Tipo de Detalhamento",     "type"	=> $rel::STRING);

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