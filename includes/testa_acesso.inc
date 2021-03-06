<?
/*
 * Sistema Simec
 * Setor respons�vel: SPO-MEC
 * Desenvolvedor: Equipe Consultores Simec
 * Analista: Cristiano Cabral (cristiano.cabral@gmail.com), Gilberto Arruda Cerqueira Xavier
 * Programador: Cristiano Cabral (cristiano.cabral@gmail.com), Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
 * M�dulo:testa_acesso.inc
 * Finalidade: permitir o acesso �s p�ginas do sistema
 */

// Verifica a sess�o
if (! isset ( $_SESSION ['usucpf'] ) || ! isset ( $_SESSION ['sisid'] )) {
	session_unset ();
	$_SESSION ['link_tentativa'] = $_SERVER ['HTTP_HOST'] . $_SERVER ['REQUEST_URI'];
	$_SESSION ['MSG_AVISO'] = 'Autentica��o inv�lida. Efetue login novamente.';
	header ( 'Location: ../login.php' );
	exit ();
}

$url = str_replace ( '/' . $_SESSION ['sisdiretorio'] . '/', '', $_SERVER ['REQUEST_URI'] );

if (! strpos ( $url, 'favorito' )) {
	$_SESSION ['favurl'] = $url;
}

$posicao = strpos ( $url, '&acao=' );
$url = substr ( $url, 0, $posicao + 7 );

function recuperarAcessoRapido($mnuidpai = null, &$aAcessoRapido, $dadosMenu = ''){
    global $db;
    $where = $mnuidpai ? "and  mnuidpai = $mnuidpai" : 'and mnuidpai is null ';

    $sql = "select mnu.mnuid, mnu.mnudsc, trim(mnu.mnulink) as mnulink, mnuidpai
            from seguranca.perfilusuario pfu, seguranca.perfil pfl, seguranca.menu mnu, seguranca.perfilmenu pfm  
            where pfu.pflcod = pfl.pflcod 
            and mnu.mnuid = pfm.mnuid 
            and pfm.pflcod = pfl.pflcod 
            and pfu.usucpf = '{$_SESSION ['usucpf']}' 
            and pfl.sisid = {$_SESSION ['sisid']}
            $where
            order by mnu.mnudsc";

    $dados = $db->carregar($sql);

    if($dados){
        foreach ($dados as $dado) {
            $dado['name'] = $dadosMenu ? $dadosMenu['name'] . ' - ' . $dado['mnudsc'] : $dado['mnudsc'];
            recuperarAcessoRapido($dado['mnuid'], $aAcessoRapido, $dado);
        }
    } else {
        $aAcessoRapido[] = $dadosMenu;
    }
}

if (!isset($_SESSION['acl'][$_SESSION['sisid']]))
{
	// buscar na tabela de menu um registro cujo link seja igual � url
	$sql = 'select mnu.mnuid, mnu.mnudsc, mnu.abacod, mnu.mnutransacao, pfu.usucpf, pfu.pflcod, pfl.pfldsc, trim(mnu.mnulink) as mnulink ';
	$sql = $sql . ' from seguranca.perfilusuario pfu, seguranca.perfil pfl, seguranca.menu mnu, seguranca.perfilmenu pfm ';
	$sql = $sql . " where pfu.pflcod = pfl.pflcod and mnu.mnuid = pfm.mnuid and pfm.pflcod = pfl.pflcod and pfu.usucpf= '" . $_SESSION ['usucpf'] . "' and pfl.sisid=" . $_SESSION ['sisid'];

	$saida = $db->carregar($sql);

	$acls = array();
	if( is_array( $saida )) {
		foreach ($saida as $menu)
		{
			$acls[$menu['mnulink']] = $menu;
		}
	}

	$_SESSION['acl'][$_SESSION['sisid']] = $acls;

    $aAcessoRapido = [];
    recuperarAcessoRapido(null, $aAcessoRapido);
	$_SESSION['acesso-rapido'][$_SESSION['sisid']] = json_encode(simec_utf8_encode_recursive($aAcessoRapido));

}

$saida = $_SESSION['acl'][$_SESSION ['sisid']][$url];

if (!is_array($saida)) {
	header("Location: ../acesso_negado.php");
} else {
	// utilizado nos titulos de p�ginas
	$abacod_tela = $saida['abacod'];
	$titulo_modulo = $saida['mnutransacao'];

	if (!strpos($url, 'favorito')) {
		$_SESSION ['mnudsc'] = $saida['mnudsc'] . '...';
	}

	// utilizado pela migalha de pao no cabe�alho e pela fun��o de auditoria
	$modulo_atual = $saida['mnuid'];
	$_SESSION['mnuid'] = $saida['mnuid'];
}