<?php
require_once APPRAIZ . "monitora/www/planotrabalhoUN/_constantes.php";
require_once APPRAIZ . "monitora/www/planotrabalhoUN/_funcoes.php";

$_SESSION['monitora_var']['boMostraAbaListaUnidadeUN'] = true;

// estas unidades não podem ser gerenciadas como unidades indiretas
$unidades_excluidas = array(AD => 'Administração Direta',
							INEP => 'Instituto Nacional de Estudos e Pesquisas Educacionais Anísio Teixeira - INEP',
							CAPES => 'Fundação Coordenação de Aperfeiçoamento de Pessoal de Nível Superior - CAPES',
							FNDE => 'Fundo Nacional de Desenvolvimento da Educação');


boUnidadesObrigatorias();
							
$filtro[] = "uni.unicod NOT IN ('".implode("','",array_keys($unidades_excluidas))."')";
$filtro[] = "uni.orgcod = '". CODIGO_ORGAO_SISTEMA. "'";
$filtro[] = "uni.unistatus = 'A'";

// verifica se é possui uma busca por codigo
if($_POST['unicod']) {
	$filtro[] = "uni.unicod='".$_POST['unicod']."'";
}
// verifica se possui uma busca po nome da unidade
if($_POST['unidsc']) {
	$filtro[] = "uni.unidsc ilike '%".$_POST['unidsc']."%'";
}

$stWhere = "";
if(!$db->testa_superuser()){
	$arUnicodTemp = perfil_unidade();
	$arUnicodTemp = ($arUnicodTemp) ? $arUnicodTemp : array();
	if($arUnicodTemp[0]){
		$arUnicod = array();
		foreach($arUnicodTemp as $uniid){
			if($uniid['unicod']){
				array_push($arUnicod,$uniid['unicod']);								
			}
		}
		if($arUnicod && $arUnicod[0]){
			$unicodTemp = "'" . implode("', '",$arUnicod) . "'";
			if( $unicodTemp ){
				$stWhere = " AND uni.unicod in(" . $unicodTemp . ") ";
			}
		}
	}
}

$sql_lista = "SELECT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'monitora.php?modulo=principal/planotrabalhoUN/subatividadesUN&acao=A&unicod='||uni.unicod||'\';\"></center>' as acoes, 
				uni.unicod, 
				uni.unidsc 
		FROM public.unidade uni 
			INNER JOIN ( select distinct unicod from planejamento.acao where prgano = '".$_SESSION['exercicio']."'  and acasnrap = false ) a ON a.unicod = uni.unicod
		".(($filtro)?" WHERE ".implode(" AND ", $filtro):"")." 
		$stWhere
		ORDER BY uni.unicod
		";

$arDados = $db->carregar($sql_lista);
if(count($arDados) == 1 && !$_POST['pesquisa']){
	$unicod = $arDados[0]['unicod'];
	$_SESSION['monitora_var']['boMostraAbaListaUnidadeUN'] = false;
	echo "<script>window.location='monitora.php?modulo=principal/planotrabalhoUN/subatividadesUN&acao=A&boUmaUninade=1&unicod=$unicod'</script>";
	exit;
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';
monta_titulo( 'Gerenciar', 'Gerenciamento nas Unidades Indiretas' );
?>
<form method="post" action="monitora.php?modulo=principal/planotrabalhoUN/listaUN&acao=A">
<input type="hidden" name="pesquisa" value="1">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class='SubTituloDireita'>Código :</td>
	<td><?php
		$unicod = $_POST['unicod']; 
		echo campo_texto('unicod', 'N', 'S', 'Código da unidade', 30, 150, "", "", '', '', 0, '' ); 
		?>
	</td>
</tr>
<tr>
	<td class='SubTituloDireita'>Unidade :</td>
	<td><?php
		$unidsc = $_POST['unidsc'];
		echo campo_texto('unidsc', 'N', 'S', 'Nome da unidade', 67, 150, "", "", '', '', 0, '' ); 
		?>
	</td>
</tr>
<tr>
	<td class='SubTituloCentro' colspan=2><input type="submit" name="buscar" value="Buscar"> <input type="button" name="todos" value="Todos" onclick="window.location='monitora.php?modulo=principal/planotrabalhoUN/listaUN&acao=A';"></td>
</tr>
</table>
</form>
<?php
$cabecalho = array("&nbsp", "Código", "Unidade");
$db->monta_lista($sql_lista,$cabecalho,100,5,'N','center',$par2);
?>