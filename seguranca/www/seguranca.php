<?php
include_once "controleInicio.inc";

include_once '_funcoes.php';

include_once APPRAIZ . "includes/classes/Modelo.class.inc";

if($_SESSION['usucpforigem'] != ''){
    verificaUsuarioReceita($_SESSION['usucpforigem']);
}

include_once "controleAcesso.inc";
?>
