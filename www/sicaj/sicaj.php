<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";
/**
 * Autoload de classes dos M�dulos SPO.
 * @see autoload.php
 */
require_once APPRAIZ . 'spo/autoload.php';

// carrega as fun��es espec�ficas do m�dulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

$perfis = pegaPerfilGeral();
/* Controle de exibi��o do de Simular Usu�rio */
if (in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(PERFIL_CGO, $perfis)) {
    $exibirSimular = true;
}

//Carrega as fun��es de controle de acesso
include_once "controleAcesso.inc";
