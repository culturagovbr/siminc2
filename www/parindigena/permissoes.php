<?php

include_once APPRAIZ . "www/obras/_funcoes.php";

$somenteLeitura = "S";

// Verifica perfil do usu�rio e seta as permiss�o pertecentes a cada um deles.
if(possuiPerfil(174) || possuiPerfil(157) || possuiPerfil(161) || possuiPerfil(162) || possuiPerfil(158)) {
	if(!possuiPerfil(160) || !possuiPerfil(155)) {
		$somenteLeitura = "N";
	}
}

?>