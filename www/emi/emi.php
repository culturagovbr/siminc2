<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";


// carrega as fun��es espec�ficas do m�dulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';


$emi = new emi();

include "geral/_permissoes.php";

//Carrega as fun��es de controle de acesso
include_once "controleAcesso.inc";
?>
<script src="/emi/geral/js/emi.js"></script>