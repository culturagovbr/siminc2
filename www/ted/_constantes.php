<?php
/**
 * Constantes do sistema
 * $Id: _constantes.php 101608 2015-08-24 18:29:38Z LucasGomes $
 */
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

/** Relat�rio de cumprimento do objeto aguardando aprova��o do Gestor Or�ament�rio do Proponente **/
define("RCO_AGUARDANDO_ANALISE_GESTOR_ORCAMENTARIO_PROPONENTE", 652);

/** Relat�rio de cumprimento do objeto aguardando aprova��o do Representante Legal do Proponente **/
define("RCO_AGUARDANDO_ANALISE_REPRESENTANTE_LEGAL_PROPONENTE", 655);

/** Relat�rio de cumprimento do objeto aguardando aprova��o da Diretoria **/
define("RCO_AGUARDANDO_ANALISE_DIRETORIA", 1492);

/** Relat�rio de cumprimento do objeto aguardando aprova��o do Secret�rio **/
define("RCO_AGUARDANDO_ANALISE_SECRETARIO", 1493);

/** Relat�rio de cumprimento do objeto aguardando aprova��o do Representante Legal do Concedente **/
define("RCO_AGUARDANDO_ANALISE_REPRESENTANTE_LEGAL_CONCEDENTE", 1495);

/** Relat�rio de cumprimento do objeto aguardando aprova��o da DIGAP FNDE **/
define("RCO_AGUARDANDO_ANALISE_DIGAP", 1494);

/** Em analise pelo Departamento Jur�dico do Proponente **/
define('EM_ANALISE_DPT_JURIDICO_PROPONENTE', 1490);

/** Em an�lise pelo Departamento Jur�dico do Concedente **/
define('EM_ANALISE_DPT_JURIDICO_CONCEDENTE', 1491);

/** Aguardando aprova��o do Secret�rio **/
define('AGUARDANDO_APROVACAO_SECRETARIO_FNDE', 1220);

//Acao workflow
define("AEDID_ARQUIVAR_TERMO", 1608);
define("APROVADO_PELO_REPRESENTANTE_LEGAL_PROPONENTE", 1597);
define("AEDID_GABINETE_SECRETARIA_AUTARQUIA_ENVIOU_COORDENACAO", 1602);
define("AEDID_EM_ANALISE_PELA_COORDENACAO", 1652);

#Perfis
define("PERFIL_PROREITOR_ADM", 1262);
define("PERFIL_COORDENADOR_SEC", 1265);
define("PERFIL_UG_REPASSADORA", 1273);
define("PERFIL_REITOR", 1263);
define("PERFIL_SECRETARIO", 1267);
define("PERFIL_SECRETARIA", 1264);
define("PERFIL_DIRETORIA", 1266);
define("PERFIL_SUBSECRETARIO", 1268);
define('COORDENADOR_SECRETARIA_AUTARQUIA', 1265);
define('PERFIL_DIRETORIA_FNDE', 1270);
define('PERFIL_AREA_TECNICA_FNDE', 1269);
define('PERFIL_GABINETE_SECRETARIA_AUTARQUIA', 1264);
define("PERFIL_ANALISTA_DIGAP", 1269);
define("PERFIL_SUPER_USUARIO", 1233);
define("PERFIL_CGSO", 1285);
define("UO_EQUIPE_TECNICA", 1271);

/**
 * Tramita��o em Lote
**/
define("LOTE_TIPO_DESCENTRALIZACAO", 1);
define("EM_EMISSAO_NOTA_CREDITO", 1034);
define("WF_ACAO_SOL_ALTERACAO", 1620);

#Hints
define("OBJETIVO_HINT", "Favor Preencher com a descri��o do objeto a ser executado, indicando, inclusive, o campus em que se localizar� o objeto. O objeto � o que deve ser fisicamente entregue � sociedade ao final da execu��o do Plano de Trabalho.");
define("JUSTIFICATIVA_HINT", "Favor registrar: Contextualiza��o da obra no campus em que o projeto ser� executado; Motiva��o da obra, isto �, qual o problema que a obra busca sanar e qual a demanda para o projeto. Caso a proposta tenha recursos a serem descentralizados em mais de um exerc�cio, o proponente dever� inserir no campo da justificativa o coment�rio de como o recurso dever� ser distribu�do ao longo dos exerc�cios. Ex.: A constru��o em quest�o dever� ter aporte de recursos distribu�dos em mais de um exerc�cio. Sendo a parcela para 2013 de R$ XX, para 2014 de R$ YY e para 2015 de R$ ZZ.");

#UGs
define("UG_FNDE", 	153173);
define("UG_CGSO", 	152734);
define("UG_CAPES", 	154003);
define("UG_INEP", 	153978);
define("UG_SECADI", 150028);
define("UG_SETEC", 	150016);
define("UG_SEB", 	150019);

#UOs
define("UO_FNDE", 26298);
define("UO_MEC", 26101);

define('MODULO_NAME', 'Termo de Execu��o Descentralizada');

/**
 * Identifica o nome do sistema. Utilizado para armazenar dados na sess�o.
 */
define('MODULO', $_SESSION['sisdiretorio']);