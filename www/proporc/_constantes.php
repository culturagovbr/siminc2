<?php
/**
 * Constantes do sistema
 * $Id: _constantes.php 97332 2015-05-14 19:03:59Z lindalbertofilho $
 */

/**
 * Identifica o nome do sistema. Utilizado para armazenar dados na sess�o.
 */
define('MODULO', $_SESSION['sisdiretorio']);

/**
 * C�digo do tipo de documento usado no workflow da proposta or�ament�ria.
 */
define('TPDOC_PROPOSTA_ORCAMENTARIA', 188);

/**
 * C�digo do tipo de documento usado no workflow da proposta or�ament�ria / prelimites pessoal.
 */
define('TPDOC_PRELIMITES_PESSOAL', 224);

/**
 * Estado da proposta assim que ela � criada e ainda n�o foi tramitado.
 */
define('ESDOC_EM_PREENCHIMENTO', 1195);
/**
 * Estado assumido pela documenta��o qdo a proposta foi enviado para an�lise SPO.
 */
define('ESDOC_ANALISE_SPO', 1196);
/**
 * Estado assumido qdo a proposta precisa de corre��es.
 */
define('ESDOC_ACERTOS_UO', 1197);
/**
 * Estado assumido qdo a proposta foi enviado para a SOF atrav�s do webservice.
 */
define('ESDOC_ENVIADO_SOF', 1198);

/**
 * Perfis utilizado pelas UOs
 */
define('PFL_UO_EQUIPE_TECNICA', 1222);
/**
 * Perfis utilizados pelas UGs da UO 26101
 */
define('PFL_AD_EQUIPE_TECNICA', 1230);
/**
 * Perfil utilizado internamente no mec.
 */
define('PFL_CGO_EQUIPE_ORCAMENTARIA', 1221);
/**
 * Perfil de superusu�rio
 */
define('PFL_ADMINISTRADOR', 1213);
/**
 * Transi��o de An�lise SPO para Envio � SOF.
 */
define('AESDID_SPO_PARA_SIOP', 2732);
/**
 * Id do M�dulo.
 */
define('SISID', 191);
/**
 * Estado do Workflow Pr�-Limites: Em Preenchimento.
 */
define('ESTADO_PRELIMITE_EM_PREENCHIMENTO', 1500);
/**
 * Estado do Workflow Pr�-Limites: An�lise SPO.
 */
define('ESTADO_PRELIMITE_ANALISE_SPO', 1501);
/**
 * Estado do Workflow Pr�-Limites: Ajustes SPO.
 */
define('ESTADO_PRELIMITE_AJUSTES_UO', 1502);
/**
 * Estado do Workflow Pr�-Limites: Conclu�do.
 */
define('ESTADO_PRELIMITE_CONCLUIDO', 1503);
