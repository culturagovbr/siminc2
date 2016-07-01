<?php
header('Cache-Control: no-cache, must-revalidate');
header('Expires: Mon, 26 Jul 1997 05:00:00 GMT');
header('Content-type: application/json');

include_once "config.inc";
include_once APPRAIZ . 'includes/classes_simec.inc';
include_once APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . 'fabrica/classes/autoload.inc';


$status  = '';
$retorno = '';

try {

    $memorandoRepositorio = new MemorandoRepositorio();
    if ( !$memorandoRepositorio->removeMemorando( $_POST['memo'] ) ) {
        throw new Exception( 'N�o foi poss�vel remover o memorando' );
    }

    $status  = 'SIM';
    $retorno = 'Memorando exclu�do com sucesso';
} catch ( Exception $e ) {
    $status  = 'NAO';
    $retorno = $e->getMessage();
}

echo $status.'|'.$retorno;

