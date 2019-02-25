<?php

/**
 * Carrega os dados financeiros do SIOP para a base do SIMEC.
 *
 * Assim que termina de baixar os dados financeiros, o script roda um processamento
 * que coloca os dados na tabela <tt>spo.siopexecucao</tt>. O acompanhamento das p�ginas
 * da execu��o j� baixadas � feito na tabela <tt>spo.siopexecucao_acompanhamento</tt>.
 * Ao final da execu��o, � enviado um e-mail com o resultado do processo.
 *
 * Sequ�ncia de execu��o:<br />
 * <ol><li>Baixa os dados do webservice (WSQuantitativo.consultarExecucaoOrcamentaria);</li>
 * <li>Apaga os dados da tabela wssof.ws_execucaoorcamentaria;</li>
 * <li>Insere os dados retornados pelo webservice na tabela wssof.ws_execucaoorcamentaria;</li>
 * <li>Executa o script de atualiza��o de finaceiros na seguinte tabela: spo.siopexecucao;</li>
 * <li>Envia e-mail com resultado da execu��o.</li></ol>
 *
 * @version $Id: spo_BaixarDadosFinanceirosSIOP.php 101880 2015-08-31 19:50:33Z maykelbraz $
 * @link http://siminc2.cultura.gov.br/seguranca/scripts_exec/spo_atualizarExecucaoOrcamentaria.php URL de execu��o.
 */

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));
//define('BASE_PATH_SIMEC', '/var/www/html/siminc2');
require_once BASE_PATH_SIMEC. '/global/config.inc';
require_once APPRAIZ. 'includes/classes_simec.inc';
include_once APPRAIZ. 'www/planacomorc/_constantes.php';
include_once APPRAIZ. 'planacomorc/classes/controller/ImportaDadosSiop.inc';
include_once APPRAIZ. 'planacomorc/classes/model/ImportaDadosSiop.inc';
require_once(APPRAIZ. 'spo/ws/sof/Quantitativo.php');

try{
    $db = new cls_banco();
    $cImportaDadosSiop = new Planacomorc_Controller_ImportaDadosSiop();
    $cImportaDadosSiop->exercicio = date('Y');
    echo '1)Atualizando, dados! | ';
    $cImportaDadosSiop->AtualizarDados();
    echo '2)Montando email para ser enviado! | ';
    $cImportaDadosSiop->AtualizarDotacao();
    echo '3)Rotina Finalizada!';
} catch (Exception $e){
    # Buscando Destinat�rios
    $mImportaDadosSiop = new Planacomorc_Model_ImportaDadosSiop();
    $listaDestinatarios = $mImportaDadosSiop->RetornaEmailsSuperUsuarios();
    echo '1)Buscando Destinat�rios! | ';
    # Atribuindo mensagem de assunto do e-mail
    $assunto = '[SIMINC2] Erro nas Altera��es de Dota��o';
    
    # Criando mensagem de e-mail
    $corpoEmailV3 = "
        <p style='text-aling: justify;'>
            Prezados,
            <br />
            <br />
            &nbsp;&nbsp;&nbsp;&nbsp; N�o foi poss�vel atualizar as informa��es de execu��o or�ament�ria(Aprovisionado, Empenhado, liquidado e Pago).
            O sistema n�o conseguiu acessar o 
            <a style='color: #0000ff;' href='https://www.siop.planejamento.gov.br' title='Clique aqui para acessar o SIOP e obter mais inform��es'>
                SIOP - Sistema Integrado de Planejamento e Or�amento do Governo Federal
            </a>.
        </p>
    ";
    include_once APPRAIZ. "includes/email-template.php";
    $conteudo = $textoEmailV3;
    echo '2)Criando mensagem de e-mail! | ';

    # Enviando o e-mail com aviso de erro de execu��o
    simec_email('', $listaDestinatarios, $assunto, $conteudo);
    echo '3)Enviando o e-mail com aviso de erro de execu��o! | ';
    echo '4)Rotina Finalizada!';
}
