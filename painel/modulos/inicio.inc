<?php
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");


// selecionando o perfil de maior nivel
$sql = "SELECT p.pflcod FROM seguranca.perfil p 
		LEFT JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod 
		WHERE pu.usucpf = '". $_SESSION['usucpf'] ."' and p.pflstatus = 'A' and p.sisid =  '". $_SESSION['sisid'] ."' 
		ORDER BY pflnivel ASC LIMIT 1";

$perfilcod = $db->pegaUm($sql);

header('Location: ?modulo=principal/lista&acao=A');
die;

switch($perfilcod) {
	case PAINEL_PERFIL_ENVIARSMS:
		header('Location: painel.php?modulo=principal/enviarsms&acao=A');
		break;
    case PAINEL_PERFIL_COCKPIT:
        header('Location: /pde/estrategico.php?modulo=principal/alinhamento_estrategico&acao=A');
        break;
	default:
		//include APPRAIZ . '/painel/modulos/principal/painel_controle.inc';
		header('Location: painel.php?modulo=principal/painel_controle&acao=A');
}

?>