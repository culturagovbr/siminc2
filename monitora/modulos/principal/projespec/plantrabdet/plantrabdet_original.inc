<?php
//error_reporting( E_ALL );
$pjeid = $_SESSION['pjeid'];

define( "APP_PLANO_TRABALHO" , APPRAIZ . "includes/planodetrabalho/tarefa_pt/" );

require_once( APP_PLANO_TRABALHO . 'arquivos_requeridosPT.inc' );
require_once( APP_PLANO_TRABALHO . "../comuns/lista_tarefas_do_projeto.inc" );
?>