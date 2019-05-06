<?php
include "planotrabalhoUG/_constantes.php";
include "planotrabalhoUG/_funcoes.php";
include "planotrabalhoUG/_componentes.php";

/*
 * Verifica se o usuário possui o perfil 'Gestor da Unidade de Planejamento' 
 * e possui a unidade do FNDE(26298) associada.
 */
$GESTOR_FNDE = false;

$sql = "SELECT
			trim(ur.unicod) as unicod
		FROM
			monitora.usuarioresponsabilidade ur
		WHERE
			ur.usucpf = '".$_SESSION["usucpf"]."' AND
			ur.pflcod = 112 AND
			ur.rpustatus = 'A'";

$unidades = $db->carregar($sql);
if($unidades) {
	for($i=0; $i<count($unidades); $i++) {
		if($unidades[$i]["unicod"] == "26298") {
			$GESTOR_FNDE = true;
		}
	}
}

/*
 * Constantes das unidades que utilizam regras especificas para o gerador
 * Inclusão de Unidade Gestora e Unidade Responsável na geração do número 
 */

// controlador do numero sequencial do gerador
include APPRAIZ."/includes/controlegeradorsequenciapi.inc";

/*
 * SCRIPT que controle as regras de combobox
 */
include APPRAIZ."/includes/controlecombobox.inc";
if(is_array($_REQUEST['dados'])) {
	$parametroconstrutor = array_merge($_REQUEST['dados'], array());
}
$combo = new ControleComboBox($parametroconstrutor);

/*
 * Constantes das opções da combobox
 */
define('COMBO_REGRAGERAL', 5);
define('COMBO_REGRAPDEEPPA', 3);
/*
 * Constantes das opções da combobox
 */

/*
 * FIM SCRIPT que controle as regras de combobox
 */

// carrega as funções de integração
include "planotrabalhoUG/unidade_atividade_funcoes.php";

include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

monta_titulo( "Gerenciar Subação", "");


if($_GET['del_sbaid']){
	$pliid = $db->pegaUm("select pliid from monitora.pi_planointerno where plistatus = 'A' and sbaid = {$_GET['del_sbaid']}");
	if($pliid){
		die("<script>alert('Subação não pode ser removida, pois existem Planos Internos para esta subação.');history.back(-1);</script>");	
	} else {
		$sql_D = "UPDATE monitora.pi_subacao SET sbastatus = 'I' where sbaid = '".$_GET['del_sbaid']."'";
		$db->executar($sql_D);
		$db->commit();
		die("<script>alert('Registro removido com sucesso.');window.location = '?modulo=principal/planotrabalhoUG/gerenciarsubacaoUG&acao=A';</script>");		
	}	
}

?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td align='right' class="SubTituloDireita">Código:</td>
    <td><?
    	$sbacod = $_REQUEST['sbacod'];
    	echo campo_texto('sbacod','N','S','',6,4,'',''); 
    	?>
    </td>
</tr>

<tr>
	<td align='right' class="SubTituloDireita">Título:</td>
    <td><? 
    	$sbatitulo = $_REQUEST['sbatitulo'];
    	echo campo_texto('sbatitulo','N','S','',50,255,'',''); 
    	?>
    </td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Enquadramento da Despesa:</td>
    <td>
    	
		<?
		$eqdid = $_REQUEST['eqdid'];
		$sql = "SELECT eqdid as codigo, eqdcod ||' - '|| eqddsc as descricao FROM planejamento.enquadramento_despesa WHERE eqdano = '{$_SESSION['exercicio']}' order by eqddsc";
		$db->monta_combo('eqdid', $sql, 'S',  'Selecione', '', '', '', '', 'N'); 
		?>    	
    </td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Executor Orçamentário e Financeiro:</td>
    <td>
		<?
		$pieid = $_REQUEST['pieid'];
		$sql = "SELECT pieid as codigo, piecod ||' - '|| piedsc as descricao FROM  monitora.pi_executor";  
		$db->monta_combo('pieid', $sql, 'S', 'Selecione', '', '', '', '100', 'N', 'pieid'); 
		?>    	
    </td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Gestor da Subação:</td>
    <td>
		<?
		$pigid = $_REQUEST['pigid'];
		$sql = "SELECT pigid as codigo, pigcod ||' - '|| pigdsc as descricao FROM  monitora.pi_gestor";  
		$db->monta_combo('pigid', $sql, 'S', 'Selecione', '', '', '', '100', 'N', 'pigid'); 
		?>    	
    </td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Permite Obras?</td>
    <td>
    	<input type="radio" name="sbaobra" value="todos" <? echo ((!$_REQUEST["sbaobra"]||$_REQUEST["sbaobra"]=="todos")?"checked":""); ?>> Todos <input type="radio" name="sbaobra" value="sim" <? echo (($_REQUEST["sbaobra"]=="sim")?"checked":""); ?>> Sim <input type="radio" name="sbaobra" value="nao" <? echo (($_REQUEST["sbaobra"]=="nao")?"checked":""); ?>> Não
    </td>
</tr>
<tr>
	<td class="SubTituloDireita" valign="top">Ações:</td>
    <td>
		<?
		if($_POST['acaid'][0] && !$_POST['ordemlista']) {
			$acaid = $db->carregar("SELECT
										aca.acaid as codigo,
										'(PTRES:'||pt.ptres||') - '|| aca.unicod ||'.'|| aca.prgcod ||'.'|| aca.acacod ||' - '|| aca.acadsc as descricao
									  FROM monitora.acao aca 
									  	inner join monitora.ptres pt on pt.acaid=aca.acaid  
									WHERE aca.prgano='".$_SESSION['exercicio']."' and aca.acasnrap = false
									and aca.unicod IN('".AD."','".CAPES."','".INEP."','".FNDE."')
									and aca.acaid in (".implode(",", $_POST['acaid']).")
									GROUP BY codigo , descricao
									ORDER BY 
										1");
		}
		$sqlComboAcoes = "SELECT
							aca.acaid as codigo,
							'(PTRES:'||pt.ptres||') - '|| aca.unicod ||'.'|| aca.prgcod ||'.'|| aca.acacod ||' - '|| aca.acadsc as descricao
						  FROM monitora.acao aca 
						  	inner join monitora.ptres pt on pt.acaid=aca.acaid
						  WHERE aca.prgano='".$_SESSION['exercicio']."' and aca.acasnrap = false
						  	and aca.unicod IN('".AD."','".CAPES."','".INEP."','".FNDE."')
						  GROUP BY codigo , descricao
						  ORDER BY 
							1";
		combo_popup( "acaid", $sqlComboAcoes, "Ações", "600x600", 0, array(), "", "S", false, false, 5, 400 );
		?>    	
    </td>
</tr>
<tr>
	<td class="SubTituloDireita" valign="top">Unidades Orçamentárias / Gestoras:</td>
	<td>
	<?php
	if($_POST['unicod'][0] && !$_POST['ordemlista']){
			$unicod = $db->carregar("SELECT
									   ungcod as codigo,
									   Case when unicod = '26101' then ungcod || ' - ' || ungdsc else unicod || ' - ' || ungdsc end as descricao
								   FROM public.unidadegestora 
								   where ungcod in ('".implode("', '", $_POST['unicod'])."')");
	}
	else{
		$unicod = "";
	}
	$sqlComboEntidade = "SELECT
							   ungcod as codigo,
							   Case when unicod = '26101' then ungcod || ' - ' || ungdsc else unicod || ' - ' || ungdsc end as descricao
						   FROM public.unidadegestora 
						   ORDER BY ungcod ";

	combo_popup( "unicod", $sqlComboEntidade, "Unidades Orçamentárias", "600x600", 0, array(), "", "S", false, false, 5, 400 );
	?>
	</td>
</tr>
<tr bgcolor="#cccccc">
	      <td></td>
	  	  <td>
	  	  <input type="button" class="botao" name="bta" value="Pesquisar" onclick="submeter();">
	  	  <input type="button" class="botao" name="bt" value="Nova Subação" onclick="window.location='?modulo=principal/planotrabalhoUG/cadsubacaoUG&acao=A';"></td>
	  	  
	      </tr>
</table>
</form>
<?
if($_POST){
	$where = " WHERE sba.sbastatus = 'A' ";
	$where .= $_REQUEST['sbatitulo'] ? " AND sba.sbatitulo ilike '%".$_REQUEST['sbatitulo']."%' " : '';
	$where .= $_REQUEST['sbacod'] ? " AND upper(sba.sbacod) like '%".strtoupper($_REQUEST['sbacod'])."%' " : '';
	$where .= $_REQUEST['exeid'] ? " AND sba.exeid = ".$_REQUEST['exeid']." " : '';
	$where .= $_REQUEST['gstid'] ? " AND sba.gstid = ".$_REQUEST['gstid']." " : '';
	$where .= $_REQUEST['eqdid'] ? " AND eq.eqdid = ".$_REQUEST['eqdid']." " : '';
	$where .= $_REQUEST['pieid'] ? " AND exe.pieid = ".$_REQUEST['pieid']." " : '';
	$where .= $_REQUEST['pigid'] ? " AND gst.pigid = ".$_REQUEST['pigid']." " : '';
	
	if($_REQUEST['acaid'][0]) {
		$acoes = implode(",", $_REQUEST['acaid'] );
		$where .= " AND ptr.acaid in (".$acoes.") ";
	}
	
	$where .= ($_REQUEST['sbaobra']!="todos" && $_REQUEST['sbaobra']!="") ? " AND sba.sbaobras = ".(($_REQUEST['sbaobra']=="sim")?"true":"false")." " : '';
	if($_POST['unicod'][0] && !$_POST['ordemlista']) {
		$where .= " AND pe.ungcod in ('".implode("', '", $_POST['unicod'])."')";
	}
	$sql = "SELECT '<center><a style=\"cursor:pointer;\" onclick=\"alterarsba(\''||sba.sbaid||'\');\"><img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\"></a><a  style=\"cursor:pointer;\" onclick=\"removersba(\''||sba.sbaid||'\');\"><img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\"></a></center>' as acao, 
				sba.sbacod,
				sba.sbatitulo,
				exe.piecod ||' - '||exe.piedsc as executor,
				gst.pigcod ||' - '||gst.pigdsc as gestor,
				u.usunome ||' - '||to_char(sba.sbadata, 'dd/mm/YYYY hh24:mi') as resp
				FROM monitora.pi_subacao sba
				LEFT JOIN seguranca.usuario u ON u.usucpf = sba.usucpf
				LEFT JOIN monitora.pi_subacaoenquadramento eq ON eq.sbaid = sba.sbaid 
				LEFT JOIN monitora.pi_executor exe ON exe.pieid = sba.pieid
				LEFT JOIN monitora.pi_gestor gst ON gst.pigid = sba.pigid
				LEFT JOIN monitora.pi_subacaounidade pe ON pe.sbaid = sba.sbaid
				INNER JOIN public.unidadegestora ung on ung.ungcod = pe.ungcod
				LEFT JOIN monitora.pi_subacaodotacao sdt ON sdt.sbaid = sba.sbaid
				LEFT JOIN monitora.ptres ptr ON ptr.ptrid = sdt.ptrid
				{$where}
				GROUP BY sba.sbaid,sba.sbacod,sba.sbatitulo,executor,gestor,resp
				ORDER BY sba.sbacod DESC";
				
	$cabecalho = array("","Código","Título","Executor Orçamentário","Gestor da Subação","Atualizado por / Data");
	$db->monta_lista($sql,$cabecalho,50,5,'N','95%',$par2);
}
?>
<script type="text/javascript">
function submeter(){
	if(document.formulario.acaid.options[0].value != "")
		selectAllOptions( document.formulario.acaid );
	if(document.formulario.unicod.options[0].value != "")
 		selectAllOptions( document.formulario.unicod );

	document.formulario.submit();	
}
function removersba(sbaid){
	var conf = confirm("Você realmente deseja excluir esta Subação?");	
	if(conf) {
		location.href="monitora.php?modulo=principal/planotrabalhoUG/gerenciarsubacaoUG&acao=A&del_sbaid="+sbaid;	
	}
}
function alterarsba(sbaid){
	location.href="monitora.php?modulo=principal/planotrabalhoUG/cadsubacaoUG&acao=A&sbaid="+sbaid;
}
</script>