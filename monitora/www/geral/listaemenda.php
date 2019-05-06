<?

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();

switch($_REQUEST["sumariogrupo"]) {
	case "unicod": $sumariogrupo = 'unicod'; break;
	case "emdsglpartidoautor": $sumariogrupo = 'emdsglpartidoautor'; break;
	case "foncod": $sumariogrupo = 'foncod'; break;
	case "gndcod": $sumariogrupo = 'gndcod'; break;
	case "acaptres": $sumariogrupo = 'acaptres'; break;
	case "funcprog": default:
		$sumariogrupo = 'funcprog';
	break;
}

$ordem = $_REQUEST['ordem'];
$acaid = str_replace('|',chr(39),$_REQUEST['acaid']);
$tipoacao1 = $_REQUEST['tipoacao1'];
$tipoacao2 = $_REQUEST['tipoacao2'];
$tipoacao3 = $_REQUEST['tipoacao3'];

$codigo = $_REQUEST['codigo'];
//$emdsglpartidoautor
//$emdufautor
//$emdcodtipoautor
//$emdnomeautor

//prepara os arrays para enviar para o abreconteudo
$emdsglpartidoautor = unserialize(get_magic_quotes_gpc() ? stripslashes($_REQUEST["emdsglpartidoautor"]) : $_REQUEST["emdsglpartidoautor"]);
$emdcodtipoautor	= unserialize(get_magic_quotes_gpc() ? stripslashes($_REQUEST["emdcodtipoautor"]) : $_REQUEST["emdsglpartidoautor"]);
$emdufautor			= unserialize(get_magic_quotes_gpc() ? stripslashes($_REQUEST["emdufautor"]) : $_REQUEST["emdsglpartidoautor"]);
$emdnomeautor		= unserialize(get_magic_quotes_gpc() ? stripslashes($_REQUEST["emdnomeautor"]) : $_REQUEST["emdsglpartidoautor"]);
$gndcod				= unserialize(get_magic_quotes_gpc() ? stripslashes($_REQUEST["gndcod"]) : $_REQUEST["gndcod"]);
$mapcod				= $_REQUEST["mapcod"];
$foncod				= $_REQUEST["foncod"];
$acaptres			= $_REQUEST["acaptres"];

$whjoin = array();
if (isset($emdsglpartidoautor) && (@count($emdsglpartidoautor)>=1 && (bool)$emdsglpartidoautor[0]) ) {
	$buffer = implode("', '", $emdsglpartidoautor);
	$whjoin[] = " emdsglpartidoautor IN ('$buffer')";
}
if (isset($emdufautor) && (@count($emdufautor)>=1 && (bool)$emdufautor[0]) ) {
	$buffer = implode("', '", $emdufautor);
	$whjoin[] = " emdufautor IN ('$buffer')";
}
if (isset($emdcodtipoautor) && (@count($emdcodtipoautor)>=1 && (bool)$emdcodtipoautor[0]) ) {
	$buffer = implode("', '", $emdcodtipoautor);
	$whjoin[] = " emdcodtipoautor IN ('$buffer')";
}
if (isset($emdnomeautor) && (@count($emdnomeautor)>=1 && (bool)$emdnomeautor[0]) ) {
	$buffer = implode("', '", $emdnomeautor);
	$whjoin[] = " emdnomeautor IN ('$buffer')";
}
if (isset($gndcod) && (@count($gndcod)>=1 && (bool)$gndcod[0]) ) {
	$buffer = implode("', '", $gndcod);
	$whjoin[] = " gndcod IN ('$buffer')";
}
if (isset($foncod) && $foncod) {
	$whjoin[] = " foncod = '$foncod'";
}
if (isset($mapcod) && $mapcod) {
	$whjoin[] = " mapcod = '$mapcod'";
}
if (isset($acaptres) && $acaptres) {
	$whjoin[] = " a.acaptres = '$acaptres'";
}

if ($acaid)
{
	$whjoin[] = " acaid in ($acaid)";
}
elseif ($ordem == 'L') 
{
	$whjoin[] = " loccod = '$codigo'";
} 
elseif ($ordem == 'U') 
{	
	$whjoin[] = " unicod = $codigo";
}
elseif ($ordem == 'T') 
{
	$whjoin[] = " emdcodautor = $codigo";
}
else
{
	$whjoin[] = " prgcod ||'.'|| acacod = '$codigo'";
}

$whjoin = implode(" AND ", $whjoin);
$sqlEmenda = <<<EOS
	SELECT 
	'<a href="{$_SESSION['sisdiretorio']}.php?modulo=principal/acao/monitoraemenda&acao=C&acaid=' || acaid || '">' || funcprog || '</a>' AS funcprog, 
	acaptres, 
	unicod, 
	emdsglpartidoautor, 
	emdufautor, 
	emdnomeautor, 
	foncod, 
	gndcod, 
	mapcod, 
	saldo, 
	liberado 
	FROM 
	planejamento.acaoautoremenda 
	WHERE 
	$whjoin 
	ORDER BY $sumariogrupo
EOS;

?>
<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#006600;">
<?
$rsgrupo = $db->carregaAgrupado($sqlEmenda, $sumariogrupo);

if (@count($rsgrupo)<=0) {
	print "</table><font color='red'>N�o foram encontrados Registros</font>";
}
else {
		//Relatorio Resumido
		$cabecalho = array('Func. Progr.','Ptres', 'Uniade', 'Partido','UF','Autor','Fonte','GND','Modalidade de Aplica��o','Saldo','Liberado');
		$rsgrupo = $db->carregaAgrupado($sqlEmenda, $sumariogrupo);
	?>
	<tr bgcolor="F7F7F7">
      <td valign="top" width="12" style="padding:2px;"><img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0"></td>
	<td colspan="6" style="border-bottom: 2px solid black;"><?
		$db->monta_lista_agrupado($rsgrupo, $cabecalho, array(0,0,0,0,0,0,0,0,0,1,1), '100%');
	?>
	</td></tr>
	<tr><td colspan="7">&nbsp;</td></tr>
	</table>
	<?
	$db->close();
	exit();
}
?>