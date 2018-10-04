<?php
// Perfil
define( "PAINEL_PERFIL_SUPER_USUARIO",	349);
define( "PAINEL_PERFIL_ATUALIZA_SH",	260);
define( "PAINEL_PERFIL_ADM_ACAO",		259);
define( "PAINEL_PERFIL_ADM_EIXO",		258);
define( "PAINEL_PERFIL_CONSULTA",		261);
define("PAINEL_PERFIL_ATENDENTE", 		405);
define("PAINEL_PERFIL_SOLICITANTE",		429);
define("PAINEL_PERFIL_ENCAMINHADOR", 	430);
define("PAINEL_PERFIL_ENVIARSMS", 		423);
define("PAINEL_PERFIL_MAPAS", 			557);
define("PAINEL_PERFIL_COCKPIT",			1218);
define("PAINEL_PERFIL_GESTOR_ACOMPANHAMENTO",	1515);
define("PAINEL_PERFIL_GESTOR_INDICADOR",	1516);


define("FUNCAO_RESPONSAVEL_ASSESSORIA", 1);

define("SEC_SECRETARIAEXECUTIVA", 1);

/* Inicio - Perfil para Publicação do Indicador */
define( "GESTOR_PDE",		271);
define( "EQUIPE_APOIO_GESTOR_PDE",		272);
/* Fim - Inicio - Perfil para Publicação do Indicador */

define("REGIONALIZACAO_ESCOLA",2);
define("REGIONALIZACAO_IES",5);
define("REGIONALIZACAO_MUN",4);
define("REGIONALIZACAO_UF",6);
define("REGIONALIZACAO_BRASIL",1);
define("REGIONALIZACAO_POSGRADUACAO",7);

define("REGIONALIZACAO_CAMPUS_SUPERIOR",8);
define("REGIONALIZACAO_CAMPUS_PROFISSIONAL",9);
define("REGIONALIZACAO_UNIVERSIDADE",10);
define("REGIONALIZACAO_INSTITUTO",11);
define("REGIONALIZACAO_HOSPITAL",12);
define("REGIONALIZACAO_ACAO",1);
define("REGIONALIZACAO_SECRETARIA",999);
define("REGIONALIZACAO_TERRITORIO",998);
define("REGIONALIZACAO_POLO",13);
define("REGIONALIZACAO_IESCPC",14);

define("UNIDADEMEDICAO_PERCENTUAL",  1);
define("UNIDADEMEDICAO_RAZAO", 		 2);
define("UNIDADEMEDICAO_NUM_INTEIRO", 3);
define("UNIDADEMEDICAO_NUM_INDICE",  4);
define("UNIDADEMEDICAO_MOEDA", 		 5);
define("UNIDADEMEDICAO_BOLEANA",	 6);

define("PERIODO_ANUAL", 3);
define("PERIODO_MENSAL", 1);

$_CONFIGS = array('estado'             => array('campo' => 'dshuf'),
				  'municipio'          => array('campo' => 'dshcodmunicipio'),
				  'escola'             => array('campo' => 'dshcod'),
				  'ies'                => array('campo' => 'dshcod'),
				  'iescpc'             => array('campo' => 'iecid'),
				  'campussuperior'     => array('campo' => 'entid'),
				  'campusprofissional' => array('campo' => 'entid'),
				  'hospital' 		   => array('campo' => 'entid'),
				  'universidade' 	   => array('campo' => 'unicod'),
				  'instituto'	 	   => array('campo' => 'unicod'),
				  'posgraduacao'       => array('campo' => 'iepid'),
				  'polo'               => array('campo' => 'polid'));

//referente painel.coleta
define("COLETA_AUTOMATICA", 2);// tipo automatica

define("FUN_RESPONSAVELINDICADOR", 51);
define("FUN_UNIVERSIDADE", 12);
define("FUN_CAMPUS", 18);

//Tipos de Conteudo das Caixas
define("TIPO_CAIXA_CONTEUDO_BUSCA_INDICADOR",9);

//Item do Módulo Acadêmico - Utilizado na Tabela de Indicadores da Rede Federal
define("ITM_VAGAS_SUP",2);
define("ITM_VAGAS_REALIZADO_SUP",28);
define("ITM_INVESTIMENTO_SUP",5);
define("ITM_MAT_OFERTATUAL_PROF", 34);
define("ITM_MAT_PREVISTA_PROF", 11);
define("ITM_INVS_PREVISTO_PROF", 14);
define("ITM_INVS_REALIZADO_PROF", 18);

//Função p/ Entidade
define("FUNCAO_RESP_SECRETARIA", 63);

define("SITSOL_NINICIADO", 1);
define("SITSOL_EMATENDIMENTO", 2);
define("SITSOL_FINALIZADO", 3);
define("SITSOL_ARQUIVADO", 4);

//Tipo de Temas para os Mapas
define("TIPO_MAPA_FORMULA", 6);

//Tipos de Gráficos do Painel
define("TIPO_GRAFICO_FLASH", "flash");
define("TIPO_GRAFICO_HIGHCHARTS", "highcharts");
//define("TIPO_GRAFICO", TIPO_GRAFICO_FLASH);
define("TIPO_GRAFICO", TIPO_GRAFICO_HIGHCHARTS);
?>