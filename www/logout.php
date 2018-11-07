<?php
    include "config.inc";

    $sisid = $_SESSION['sisid'];
    $aviso = isset($_SESSION['MSG_AVISO']) ? $_SESSION['MSG_AVISO'] : null;

    $_SESSION = array();
    $_SESSION['MSG_AVISO'] = $aviso;

    if($sisid == 48){
        header('Location: /sigest.php');
    } else {
        header('Location: /login.php');
    }

    exit();
?>