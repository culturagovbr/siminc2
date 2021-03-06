<?php

if( $_SESSION['sisbaselogin'] == 'simec_desenvolvimento' ){
	
	/*** Fluxo de avalia��o da A��o - IN�CIO ***/
	define("FLUXO_AVALIACAO_TPDID", 86);
	/*** Fluxo de avalia��o da A��o - FIM ***/
	
	/*** Fluxo de avalia��o da An�lise (Objetivo/Meta/Iniciativa) - IN�CIO ***/
	define("FLUXO_ANALISE_TPDID", 90);
	/*** Fluxo de avalia��o da An�lise - FIM ***/
	
	define("PFL_COORDENADOR_ACAO", 1);
	
	#PERFIS PPA - MONITORAMENTO.
	
	define("PERFIL_MONIT_COORDENADOR_ACAO", 1);
	define("PERFIL_MONIT_GERENTE_PROGRAMA", 2);
	define("PERFIL_MONIT_GERENTE_EXECUTIVO", 3);
	define("PERFIL_MONIT_COORDENADOR_UNIDADE_MONITORAMENTO", 4);
	define("PERFIL_MONIT_ADM_SISTEMA", 5);
	define("PERFIL_MONIT_SUPER_USUARIO", 6);
	define("PERFIL_MONIT_CONSULTA", 7);
	define("PERFIL_MONIT_EQUIPE_APOIO_COORDENADORES", 8);
	
	define("PERFIL_MONIT_ALTA_GESTAO", 11);
	define("PERFIL_MONIT_UNIDADE_MONITORAMENTO_AVALIACAO", 18);
	define("PERFIL_MONIT_MONITOR_INTERNO", 20);
	define("PERFIL_MONIT_EMENDAS", 21);
	define("PERFIL_MONIT_EQUIPE_APOIO_GERENTES", 22);
	define("PERFIL_MONIT_AUDITOR_INTERNO", 77);
	
	define("PERFIL_MONIT_GESTOR_UNIDADE_PLANEJAMENTO", 112);
	define("PERFIL_MONIT_EQUIPE_APOIO_GESTOR_GESTOR_UNIDADE", 113);
	
	define("PERFIL_MONIT_GESTOR_UNIDADE_ORCAMENTO", 410);
	define("PERFIL_MONIT_EQUIPE_APOIO_GESTOR_UNIDADE_ORCAMENTO", 411);
	define("PERFIL_MONIT_GESTOR_UNIDADE_OBRIGATORIA", 412);
	define("PERFIL_MONIT_EQUIPE_APOIO_GESTOR_UNIDADE_OBRIGATORIA", 413);
	define("PERFIL_MONIT_GESTOR_SUBACAO", 414);
	
	define("PERFIL_MONIT_CPMO", 837);
	define("PERFIL_MONIT_AVALIADOR_META", 838);
	define("PERFIL_MONIT_AVALIADOR_OBJETIVO", 839);
	define("PERFIL_MONIT_AVALIADOR_INICIATIVA", 840);
	define("PERFIL_MONIT_VALIDADOR", 870);
	
	
	//Estado Documento Workflow
	define("WK_FINALIZADO", 580);
	
	define("WF_ACAO_EM_ANALISE_PELA_CPMO",579);
	define("WF_ACAO_EM_CADASTRAMENTO",578);
	define("WF_ACAO_EM_CORRECAO",631);
	define("WF_ACAO_EM_VALIDACAO_DA_UNIDADE",630);
	define("WF_ACAO_FINALIZADA",580);
	
	//
	//Estado Documento Workflow
	define("WK_FINALIZADO_ANALISE", 590);
	
	//Estado Documento p/ Relat�rio
	define("WF_ACAO_EM_CADASTRAMENTO_RELATORIO"			,617);
	define("WF_ACAO_EM_VALIDACAO_DA_UNIDADE_RELATORIO"	,618);
	define("WF_ACAO_EM_ANALISE_PELA_CPMO_RELATORIO"		,619);
	define("WF_ACAO_FINALIZADA_RELATORIO"				,620);
	
	define("REFERENCIA_PRIMEIRO_2012", 262);
	define("REFERENCIA_SEGUNDO_2012", 263);
	
}else{
	/*** Fluxo de avalia��o da A��o - IN�CIO ***/
	define("FLUXO_AVALIACAO_TPDID", 94);
	
	define("WK_FINALIZADO", 623);
	
	define("WF_ACAO_EM_CADASTRAMENTO"			,620);
	define("WF_ACAO_EM_VALIDACAO_DA_UNIDADE"	,621);
	define("WF_ACAO_EM_ANALISE_PELA_CPMO"		,622);
	define("WF_ACAO_FINALIZADA"					,623);
	/*** Fluxo de avalia��o da A��o - FIM ***/
	
	/*** Fluxo de avalia��o da An�lise (Objetivo/Meta/Iniciativa) - IN�CIO ***/
	define("FLUXO_ANALISE_TPDID", 95);
	
	define("WK_OBJ_EM_CADASTRAMENTO",		621);
	define("WK_OBJ_EM_ANALISE_PELA_CPMO",	622);
	define("WK_OBJ_FINALIZADA",				623);
	/*** Fluxo de avalia��o da An�lise - FIM ***/
	
	define("PFL_COORDENADOR_ACAO", 1);
	
	
	#PERFIS PPA - MONITORAMENTO.
	
	define("PERFIL_MONIT_COORDENADOR_ACAO", 1);
	define("PERFIL_MONIT_GERENTE_PROGRAMA", 2);
	define("PERFIL_MONIT_GERENTE_EXECUTIVO", 3);
	define("PERFIL_MONIT_COORDENADOR_UNIDADE_MONITORAMENTO", 4);
	define("PERFIL_MONIT_ADM_SISTEMA", 5);
	define("PERFIL_MONIT_SUPER_USUARIO", 6);
	define("PERFIL_MONIT_CONSULTA", 7);
	define("PERFIL_MONIT_EQUIPE_APOIO_COORDENADORES", 8);
	
	define("PERFIL_MONIT_ALTA_GESTAO", 11);
	define("PERFIL_MONIT_UNIDADE_MONITORAMENTO_AVALIACAO", 18);
	define("PERFIL_MONIT_MONITOR_INTERNO", 20);
	define("PERFIL_MONIT_EMENDAS", 21);
	define("PERFIL_MONIT_EQUIPE_APOIO_GERENTES", 22);
	define("PERFIL_MONIT_AUDITOR_INTERNO", 77);
	
	define("PERFIL_MONIT_GESTOR_UNIDADE_PLANEJAMENTO", 112);
	define("PERFIL_MONIT_EQUIPE_APOIO_GESTOR_GESTOR_UNIDADE", 113);
	
	define("PERFIL_MONIT_GESTOR_UNIDADE_ORCAMENTO", 410);
	define("PERFIL_MONIT_EQUIPE_APOIO_GESTOR_UNIDADE_ORCAMENTO", 411);
	define("PERFIL_MONIT_GESTOR_UNIDADE_OBRIGATORIA", 412);
	define("PERFIL_MONIT_EQUIPE_APOIO_GESTOR_UNIDADE_OBRIGATORIA", 413);
	define("PERFIL_MONIT_GESTOR_SUBACAO", 414);
	
	define("PERFIL_MONIT_CPMO", 854);
	define("PERFIL_MONIT_AVALIADOR_META", 855);
	define("PERFIL_MONIT_AVALIADOR_OBJETIVO", 856);
	define("PERFIL_MONIT_AVALIADOR_INICIATIVA", 857);
	define("PERFIL_MONIT_VALIDADOR", 858);
	
	//
	//Estado Documento Workflow
	define("WK_FINALIZADO_ANALISE", 619);
	
	//Estado Documento p/ Relat�rio
	define("WF_ACAO_EM_CADASTRAMENTO_RELATORIO"			,617);
	define("WF_ACAO_EM_VALIDACAO_DA_UNIDADE_RELATORIO"	,618);
	define("WF_ACAO_EM_ANALISE_PELA_CPMO_RELATORIO"		,619);
	define("WF_ACAO_FINALIZADA_RELATORIO"				,620);
	
	
	define("REFERENCIA_PRIMEIRO_2012", 262);
	define("REFERENCIA_SEGUNDO_2012", 263);
}

?>