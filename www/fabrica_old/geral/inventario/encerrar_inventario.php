<?php

header( 'content-type: text/html; charset=iso-8859-1;' );
include "config.inc";
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/autoload.inc';


$inventario     = new InventarioRepositorio();
$coInventario   = (int) $_REQUEST['co_inventario'];
$status         = false;
$msg            = '';
try {
    
    if( empty($coInventario) )
    {
        throw new Exception('N�o foi possivel encontrar o invent�rio');
    }
    
    $inventario->setAtributos( array(
        'co_inventario'         => $coInventario,
        'usucpfencerramento'    => $_SESSION['usucpf'],
        'dt_encerramento'       => date('Y-m-d'),
    ));
    
    if( !$inventario->alterar() ) {
        throw new Exception('N�o foi possivel encerrar o invent�rio');
    }
    
    $status = true;
    $msg    = 'Invent�rio encerrado com sucesso';
    
    $inventario->commit();
    
} catch ( Exception $e  ) {
    $status = false;
    $msg    = $e->getMessage();
    
}

echo simec_json_encode(array(
    'status'    => $status,
    'msg'       => utf8_encode($msg)
));