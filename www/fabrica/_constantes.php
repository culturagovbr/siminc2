<?php
/* 
 * Editado por: Rondomar Fran�a
 * Obs.: quando uma constante for criada no arquivo 'DEV' a mesma dever� ser criada no arquivo 'PRO'.
 */
if( $_SESSION['sisbaselogin'] == 'simec_desenvolvimento'){
	include_once '_constantes_dev.php';
}else{
	include_once '_constantes_pro.php';
}