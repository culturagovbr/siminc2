<?php
global $arrPermissoes;

/************* ADMINISTRADOR **********************/
// GERAL
$arrPermissoes[EXEC_PERFIL_ADMINISTRADOR]["principal/administrarProcesso"]["geral"] = true;

/************* TECNICO EM EMPENHO **********************/
// GERAL
$arrPermissoes[EXEC_PERFIL_TECNICO_EMPENHO]["principal/listaDeProcessos"]["geral"] 		= false;
$arrPermissoes[EXEC_PERFIL_TECNICO_EMPENHO]["principal/administrarProcesso"]["geral"] 	= false;
$arrPermissoes[EXEC_PERFIL_TECNICO_EMPENHO]["principal/empenhoPagamento"]["geral"]		= true;
$arrPermissoes[EXEC_PERFIL_TECNICO_EMPENHO]["principal/gerarDocumentos"]["geral"]		= false;

// fun��o para retornar se o usu�rio tem acesso(true/false) � partir dos par�metros
function permissoesPerfil($perfil, $pagina, $categoria) {
	global $arrPermissoes;
	return $arrPermissoes[$perfil][$pagina][$categoria];	
}

?>