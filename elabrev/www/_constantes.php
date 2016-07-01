<?php

if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){	
	
	#SERVIDOR LOCAL
	
	#TIPO DE DOCUMENTO
	define("WF_TIPOS_DE_DOCUMENTOS", 84);
	
	#TIPO DE DOCUEMNTO PROJETO ESPANADA SUSTENT�VEL - ALUNOS.  DESENVOLVIMENTO.
	define("WF_PROJETO_ESPLANADA_SUSTENTAVEL_ALUNOS", 99);
	
	#TIPO DE DOCUEMNTO PROJETO ESPANADA SUSTENT�VEL. DESENVOLVIMENTO.
	define("WF_PROJETO_ESPLANADA_SUSTENTAVEL", 100);
	
	#ESTADO DOCUMENTO PROJETO ESPANADA SUSTENT�VEL. DESENVOLVIMENTO.
	define("WF_ES_EM_CADASTRAMENTO", 645);
	define("WF_ES_EM_VALIDACAO_DIRETOR", 646);
	define("WF_ES_ENCAMINHAR_MEC", 647);
	
	#ESTADO DOCUMENTO PROJETO ESPANADA SUSTENT�VEL - POR ALUNOS. DESENVOLVIMENTO.
	define("WF_ES_EM_CADASTRAMENTO_ALUNOS", 639);
	define("WF_ES_EM_VALIDACAO_PROREITOR_ALUNOS", 640);
	define("WF_ES_ENCAMINHAR_MEC_ALUNOS", 641);
	
	### DESCENTRALIZACAO ###
	define("WF_TPDID_DESCENTRALIZACAO", 84);
        
	// Estados workflow
	define("EM_CADASTRAMENTO", 559);
	define("EM_APROVACAO_DA_REITORIA", 560);
	define("EM_ANALISE_DA_SECRETARIA", 562);
	define("EM_ANALISE_OU_PENDENTE", 563);//analise da diretoria
	define("AGUARDANDO_APROVACAO_SECRETARIO", 564);
	define("EM_ANALISE_PELA_CGSO", 565);
	define("EM_ANALISE_PELA_SPO", 566);
	define("EM_DESCENTRALIZACAO", 567);
	define("EM_EMISSAO_NOTA_CREDITO", 1034);	
	define("EM_EXECUCAO", 568);
	define("TERMO_FINALIZADO", 569);
	define("EM_DILIGENCIA", 648);
	define("AGUARDANDO_APROVACAO_DIRETORIA", 642);
	define("ALTERAR_TERMO_COOPERACAO", 643);
	define("EM_ANALISE_COORDENACAO", 649);
	define("TERMO_ARQUIVADO", 654);
	
	define("AGUARDANDO_APROVACAO_GESTOR_PROPONENTE", 631);	
	define("AGUARDANDO_DISPONIBILIDADE_ORCAMENTARIA", 654);
	define("RELATORIO_OBJ_AGUARDANDO_APROV_GESTOR", 652);
	define("RELATORIO_OBJ_AGUARDANDO_APROV_REITORIA", 655);
	define("RELATORIO_OBJ_AGUARDANDO_ANALISE_COORD", 656);
	
	
	//Acao workflow
	define("AEDID_ARQUIVAR_TERMO", 1544);
	### FIM DESCENTRALIZACAO ###
	
	#Perfis
	define("PERFIL_PROREITOR_ADM", 872);
	define("PERFIL_DIRETOR_ADMIM", 876);
	define("PERFIL_COORDENADOR_SEC", 875);
	define("PERFIL_CGSO", 826);
	define("PERFIL_REITOR", 825);
	define("PERFIL_SECRETARIO", 824);
	define("PERFIL_SECRETARIA", 822);
	define("PERFIL_DIRETORIA", 823);
	define("PERFIL_SUBSECRETARIO", 827);
    define('PERFIL_ADMINISTRADOR_PROPOSTAS', 1011);
    define('PERFIL_GABINETE_SECRETARIA_AUTARQUIA', 859);

	/*
	 * Tramita��o em Lote
	 * */
	
	define("LOTE_TIPO_DESCENTRALIZACAO", 1);
	
}else{
	
	#SERVIDOR DE PRODU��O
	
	#TIPO DE DOCUMENTO
	define("WF_TIPOS_DE_DOCUMENTOS", 97);
	
	#TIPO DE DOCUEMNTO PROJETO ESPANADA SUSTENT�VEL - ALUNOS. PRODU��O.
	define("WF_PROJETO_ESPLANADA_SUSTENTAVEL_ALUNOS", 92);

	#TIPO DE DOCUEMNTO PROJETO ESPANADA SUSTENT�VEL. PRODU��O.
	define("WF_PROJETO_ESPLANADA_SUSTENTAVEL", 91);

	#ESTADO DOCUMENTO PROJETO ESPANADA SUSTENT�VEL. PRODU��O.
	define("WF_ES_EM_CADASTRAMENTO", 604);
	define("WF_ES_EM_VALIDACAO_DIRETOR", 605);
	define("WF_ES_ENCAMINHAR_MEC", 606);
	
	#ESTADO DOCUMENTO PROJETO ESPANADA SUSTENT�VEL - POR ALUNOS. PRODU��O.
	define("WF_ES_EM_CADASTRAMENTO_ALUNOS", 607);
	define("WF_ES_EM_VALIDACAO_PROREITOR_ALUNOS", 608);
	define("WF_ES_ENCAMINHAR_MEC_ALUNOS", 609);
	
	### DESCENTRALIZACAO ###
	
	define("WF_TPDID_DESCENTRALIZACAO", 97);

        /** Termo em cadastramento */
	define("EM_CADASTRAMENTO", 631);
        
        /** Termo aguardando aprova��o do Representante Legal do Proponente */
	define("EM_APROVACAO_DA_REITORIA", 632);
        
        /** Em distribui��o pelo gabinete da secretaria/autarquia */
	define("EM_ANALISE_DA_SECRETARIA", 633);
        
        /** analise da diretoria - \Em analise pela coordena��o */
	define("EM_ANALISE_OU_PENDENTE", 634);
        
        /** Termo aguardando aprova��o pelo  Representante Legal do Concedente */
	define("AGUARDANDO_APROVACAO_SECRETARIO", 635);
        
        /** Termo em an�lise pela UG repassadora */
	define("EM_ANALISE_PELA_CGSO", 636); 
        
        /** Termo em An�lise pelo Gestor Or�ament�rio do Concedente */
	define("EM_ANALISE_PELA_SPO", 637);
        
        /** Termo aprovado, aguardando execu��o da descentraliza��o */
	define("EM_DESCENTRALIZACAO", 638);
        
        /** Termo em Execu��o */
	define("EM_EXECUCAO", 639);
        
        /** Termo finalizado */
	define("TERMO_FINALIZADO", 640);
        
        /** Termo com solicita��o de altera��o */
	define("ALTERAR_TERMO_COOPERACAO", 641);
        
        /** Aguardando aprova��o pela diretoria */
	define("AGUARDANDO_APROVACAO_DIRETORIA", 642);
        
        /** Em Dilig�ncia */
	define("EM_DILIGENCIA", 643);
        
        /** Em an�lise da coordena��o */
	define("EM_ANALISE_COORDENACAO", 644);
        
        /** Arquivado */
	define("TERMO_ARQUIVADO", 647);
	
        /** Termo aguardando disponibilidade or�ament�ria */
	define("AGUARDANDO_DISPONIBILIDADE_ORCAMENTARIA", 654);
        
        /** Termo em analise pela DIGAP */
	define("RELATORIO_OBJ_AGUARDANDO_APROV_GESTOR", 652);
        
        /** Termo aguardando aprova��o do Gestor Or�ament�rio do Proponente */
	define("TERMO_AGUARDANDO_APROVACAO_GESTOR_PROP", 653);
        
        /** Relat�rio de cumprimento do objeto aguardando aprova��o do Representante Legal do Proponente */
	define("RELATORIO_OBJ_AGUARDANDO_APROV_REITORIA", 655);
        
        /** Relat�rio de cumprimento do objeto em an�lise pela coordena��o */
	define("RELATORIO_OBJ_AGUARDANDO_ANALISE_COORD", 656);
        
        /** Termo em Dilig�ncia do Relat�rio de cumprimento */
	define("TERMO_EM_DILIGENCIA_RELATORIO", 660);

    /** Termo em an�lise or�ament�ria no FNDE **/
    define("TERMO_EM_ANALISE_ORCAMENTARIA_FNDE", 662);

    /** Termo aguardando valida��o da diretoria no FNDE **/
    define("TERMO_AGUARDANDO_VALIDACAO_DIRETORIA_FNDE", 888);

	//Acao workflow
	define("AEDID_ARQUIVAR_TERMO", 1608);
	
	#Perfis
	define("PERFIL_PROREITOR_ADM", 852);
	define("PERFIL_DIRETOR_ADMIM", 851);
	define("PERFIL_COORDENADOR_SEC", 866);
	define("PERFIL_CGSO", 862); //UG Repassadora
	define("PERFIL_REITOR", 864);
	define("PERFIL_SECRETARIO", 865);
    define("PERFIL_REPRESENTATE_LEGAL_CONCEDENTE", 865);
	define("PERFIL_SECRETARIA", 859);
	define("PERFIL_DIRETORIA", 860);
	define("PERFIL_SUBSECRETARIO", 863);
    define('PERFIL_ADMINISTRADOR_PROPOSTAS', 1011);
    define('PERFIL_DIRETORIA_FNDE', 1052);
    define('PERFIL_AREA_TECNICA_FNDE', 871);
    define('PERFIL_GABINETE_SECRETARIA_AUTARQUIA', 859);
    define('PERFIL_SUPER_USUARIO', 23);

	/*
	 * Tramita��o em Lote
	* */
	
	define("LOTE_TIPO_DESCENTRALIZACAO", 1);
}

define("EM_EMISSAO_NOTA_CREDITO", 1034);
define("WF_ACAO_SOL_ALTERACAO", 1620);

#PERFIL
define("PERFIL_CONSULTA_ESPLANADA_SUSTENTAVEL", 867);
define("PERFIL_ANALISTA_DIGAP", 871);
define("PERFIL_SUPER_USUARIO", 23);
define("CGO_COORDENADOR_ORCAMENTO", 50);
define("CGO_EQUIPE_ORCAMENTARIA", 52);
define("UO_COORDENADOR_EQUIPE_TECNICA", 53);
define("UO_EQUIPE_TECNICA", 54);
define("MEC_CONSULTA_ORCAMENTO_GERAL", 55);
define("UO_CONSULTA_ORCAMENTO", 57);
define("AUDITOR_EXTERNO", 76);
define("AUDITOR_INTERNO", 388);

#Hints
define("OBJETIVO_HINT", "Favor Preencher com a descri��o do objeto a ser executado, indicando, inclusive, o campus em que se localizar� o objeto. O objeto � o que deve ser fisicamente entregue � sociedade ao final da execu��o do Plano de Trabalho.");
define("JUSTIFICATIVA_HINT", "Favor registrar: Contextualiza��o da obra no campus em que o projeto ser� executado; Motiva��o da obra, isto �, qual o problema que a obra busca sanar e qual a demanda para o projeto. Caso a proposta tenha recursos a serem descentralizados em mais de um exerc�cio, o proponente dever� inserir no campo da justificativa o coment�rio de como o recurso dever� ser distribu�do ao longo dos exerc�cios. Ex.: A constru��o em quest�o dever� ter aporte de recursos distribu�dos em mais de um exerc�cio. Sendo a parcela para 2013 de R$ XX, para 2014 de R$ YY e para 2015 de R$ ZZ.");

#UGs
define("UG_FNDE", 		153173);
define("UG_CGSO", 		152734);
define("UG_CAPES", 		154003);
define("UG_INEP", 		153978);
define("UG_SECADI", 	150028);
define("UG_SETEC", 		150016);
define("UG_SEB", 		150019);

#UOs
define("UO_FNDE", 		26298);

define('MODULO_NAME', 'Termo de Execu��o Descentralizada');

?>