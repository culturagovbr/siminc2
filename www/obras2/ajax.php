<?php

/**
 * Centraliza as requisi��es ajax do m�dulo.  
 *
 * @author Ren� de Lima Barbosa <renebarbosa@mec.gov.br> 
 * @since 25/05/2007 
 */

// carrega as fun��es gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";


// carrega as fun��es do m�dulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

include_once '_funcoes_obras_par.php';

// abre conex�o com o servidor de banco de dados
$db = new cls_banco();

function fechaDb()
{
    global $db;
    $db->close();
}

register_shutdown_function('fechaDb');

if($_REQUEST['buscasaldoprocesso']){
    exibirSaldoProcesso($_REQUEST['buscasaldoprocesso'], false);
    die;
}

if($_REQUEST['buscasaldofuncionalprogramatica']){
    exibirDadosFuncionalProgramatica($_REQUEST['buscasaldofuncionalprogramatica'], $_REQUEST['ptres']);
    die;
}

if($_REQUEST['detalhar_pendencias_obras']){
    $esfuf  = $_REQUEST['estuf'] && $_REQUEST['estuf'] != 'undefined' ? $_REQUEST['estuf'] : '';
    $muncod = $_REQUEST['muncod'] && $_REQUEST['muncod'] != 'undefined' ? $_REQUEST['muncod'] : '';

    $esfera = $esfuf ? 'E' : 'M';

    exibirAvisoPendencias(null, $muncod, null, $esfuf, $esfera);

    die;
}

if($_REQUEST['montar_painel']){
    exibirGraficoHistoricoWorkflow();
    die;
}

if($_REQUEST['exibirGraficoHistoricoWorkflow']){
    exibirGraficoHistoricoWorkflow();
    die;
}