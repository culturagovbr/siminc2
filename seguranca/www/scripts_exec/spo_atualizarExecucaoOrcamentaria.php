<?php

/**
 * Carrega os dados financeiros do SIOP para a base do SIMEC.
 *
 * Assim que termina de baixar os dados financeiros, o script roda um processamento
 * que coloca os dados na tabela <tt>spo.siopexecucao</tt>. O acompanhamento das páginas
 * da execução já baixadas é feito na tabela <tt>spo.siopexecucao_acompanhamento</tt>.
 * Ao final da execução, é enviado um e-mail com o resultado do processo.
 *
 * Sequência de execução:<br />
 * <ol><li>Baixa os dados do webservice (WSQuantitativo.consultarExecucaoOrcamentaria);</li>
 * <li>Apaga os dados da tabela wssof.ws_execucaoorcamentaria;</li>
 * <li>Insere os dados retornados pelo webservice na tabela wssof.ws_execucaoorcamentaria;</li>
 * <li>Executa o script de atualização de finaceiros na seguinte tabela: spo.siopexecucao;</li>
 * <li>Envia e-mail com resultado da execução.</li></ol>
 *
 * @version $Id: spo_BaixarDadosFinanceirosSIOP.php 101880 2015-08-31 19:50:33Z maykelbraz $
 * @link http://siminc2.cultura.gov.br/seguranca/scripts_exec/spo_atualizarExecucaoOrcamentaria.php URL de execução.
 */

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));
//define('BASE_PATH_SIMEC', '/var/www/html/siminc2');
require_once BASE_PATH_SIMEC. '/global/config.inc';
require_once APPRAIZ. 'includes/classes_simec.inc';
include_once APPRAIZ. 'www/planacomorc/_constantes.php';
include_once APPRAIZ. 'planacomorc/classes/controller/ImportaDadosSiop.inc';
include_once APPRAIZ. 'planacomorc/classes/model/ImportaDadosSiop.inc';
require_once(APPRAIZ. 'spo/ws/sof/Quantitativo.php');
$db = new cls_banco();
$cImportaDadosSiop = new Planacomorc_Controller_ImportaDadosSiop();
echo '1)Atualizando, dados! | ';
$cImportaDadosSiop->AtualizarDados();
echo '2)Montando email para ser enviado! | ';
$cImportaDadosSiop->AtualizarDotacao();
echo '3)Rotina Finalizada! | ';
