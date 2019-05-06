<? 
 
define("AD"   ,26101);
define("CAPES",26291);
define("INEP" ,26290);
define("FNDE" ,26298);
define("FIES" ,74902);
define("SUPERVISAOMEC" ,73107);

$atiid = $_POST[atiid] ? $_POST[atiid] : $_GET[atiid]; 
$prgcod = $_POST['prgcod'];
$acacod = $_POST['acacod'];
$sbaid = $_REQUEST['sbaid'];
//$ppiidFiltro = "";
$prgano = $_SESSION['exercicio'];

// verificando a unidade
//$unidade = $db->pegaLinha("SELECT ent.entunicod, ent.entungcod, ent.entnome FROM entidade.entidade ent 
			   	   	   //WHERE ent.entid='".$_SESSION['monitora_var']['entid']."'");
// verificando se existe uma unidade
//if(!$unidade) {
//	echo "<script>alert('Não existe unidade');window.location = '?modulo=principal/planotrabalho/plano&acao=A';</script>";
//	exit;
//}
// verificando se é undidade ou unidade gestora


$where .= $_REQUEST['sbaid'] ? "AND dt.sbaid = '".$_REQUEST['sbaid']."'" : '';
$where .= $_POST['prgcod'] ? "AND UPPER(dtl.prgcod) LIKE('%".strtoupper($_POST['prgcod'])."%')" : '';
$where .= $_POST['acacod'] ? "AND UPPER(dtl.acacod) LIKE('%".strtoupper($_POST['acacod'])."%')" : '';
$where .= $_POST['buscalivre'] ? "AND (trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) ilike('%".$_POST['buscalivre']."%') OR dtl.ptres ilike '%".$_POST['buscalivre']."%')" : '';

// verificando se é UNIDADE ORÇAMENTARIA(UO) ou UNIDADE GESTORA(UG)
/*
switch($_SESSION['monitora_var']['tipo']) {
	case 'un':
	$unidsc    = $unidade['entunicod']." - ".$unidade['unidsc'];
	$unicod    = $unidade['entunicod'];
	
	$sql_lista = "SELECT 
				'<input type=\"checkbox\" id=\"chk_'||dtl.ptres||'\" onclick=\"resultado(this,\''||dtl.acaid||'\',\''||dtl.ptres||'\');\">' as checkbox,
				dtl.ptres,
				trim(dtl.prgcod||'.'||dtl.acacod||'.'||dtl.unicod||'.'||dtl.loccod||' - '||dtl.acadsc) as descricao,
				ent.entnome,
				SUM(dtl.ptrdotacao) as dotacaoinicial,
				'<a href=\"javascript:detfin(\''|| dtl.ptres ||'\')\">' || coalesce(cast(SUM(dtl.valorpi) as varchar),'0.00') || '</a>' as detalhamento,
				(SUM(dtl.ptrdotacao)-coalesce(SUM(dtl.valorpi),0)) as diferenca 
				FROM monitora.v_detalhepiptres dtl 
				LEFT JOIN entidade.entidade ent ON ent.entunicod = dtl.unicod AND ent.entungcod IS NULL 
				inner JOIN planejamento.programapiacao ppa ON ppa.acaid = dtl.acaid
				WHERE
					  dtl.unicod = '".$unicod."' AND
					  dtl.prgano = '".$prgano."'  
					  --AND ppa.ppiid = '".$_REQUEST['ppiid']."'
				";
	break;
	case 'ug':
	$unicod       = $unidade['entunicod'];
	$entungcod    = $unidade['entungcod'];
	
	$sql_lista = "SELECT
					'<input type=\"checkbox\" id=\"chk_'||dtl.ptres||'\" onclick=\"resultado(this,\''||dtl.acaid||'\',\''||dtl.ptres||'\');\">' as checkbox,
					dtl.ptres,
					trim(dtl.prgcod||'.'||dtl.acacod||'.'||dtl.unicod||'.'||dtl.loccod||' - '||dtl.acadsc) as descricao,
					ent.entnome,
					SUM(dtl.ptrdotacao) as dotacaoinicial,
					'<a href=\"javascript:detfin(\''|| dtl.ptres ||'\')\">' || coalesce(cast(SUM(dtl.valorpi) as varchar),'0.00') || '</a>' as detalhamento,
					(SUM(dtl.ptrdotacao)-coalesce(SUM(dtl.valorpi),0)) as diferenca 
					FROM monitora.acaounidadegestora aug 
					LEFT JOIN monitora.v_detalhepiptres dtl ON aug.acaid = dtl.acaid 
					LEFT JOIN entidade.entidade ent ON ent.entunicod = dtl.unicod AND ent.entungcod IS NULL 
					inner JOIN planejamento.programapiacao ppa ON ppa.acaid = dtl.acaid
					WHERE
						aug.ungcod = '".$entungcod."' AND
						dtl.prgano = '".$prgano."'  
					    --AND ppa.ppiid = '".$_REQUEST['ppiid']."'
					";
	
	break;
}
*/
	$sql_lista = "SELECT
						'<input type=\"checkbox\" id=\"chk_'||dtl.ptrid||'\" onclick=\"resultado(this,\''||dtl.ptrid||'\',\''||dtl.ptres||'\');\">' as checkbox,
						dtl.ptres,
						trim(aca.prgcod||'.'||aca.acacod||'.'||aca.unicod||'.'||aca.loccod||' - '||aca.acadsc) as descricao,
						ent.entnome,
						SUM(dtl.ptrdotacao) as dotacaoinicial,
						coalesce(cast(SUM(dt.valor) as varchar),'0.00') as detalhamento,
						(SUM(dtl.ptrdotacao)-coalesce(SUM(dt.valor),0.00)) as diferenca,	
						coalesce(rofautorizado, 0.00) as rofautorizado ,
						coalesce(rof_credrecebidodireta, 0.00) as rof_credrecebidodireta ,
						coalesce(rof_credrecebidoindireta, 0.00) as rof_credrecebidoindireta							 
					  FROM monitora.acao aca 
						inner JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid 
						inner JOIN entidade.entidade ent ON ent.entunicod = dtl.unicod AND ent.entungcod IS NULL					  
						LEFT JOIN ( select ptrid,
									sum( sadvalor ) as valor
								from  monitora.subacaodotacao
								group by ptrid ) dt ON dtl.ptrid = dt.ptrid
						LEFT JOIN ( select 
										ptrid,
										e.funcod,
										e.sfucod,
										e.prgcod, 	
										e.acacod,
										e.unicod,
										e.loccod,
										sum(coalesce(rofdot_ini ,0.00) +  coalesce(rofpl_credadicionais ,0.00)) as rofautorizado ,
										sum(coalesce(rof_credrecebidodireta ,0.00)) as rof_credrecebidodireta ,
										sum(coalesce(rof_credrecebidoindireta ,0.00)) as rof_credrecebidoindireta  
									from 
									financeiro.execucao e 
									inner join monitora.ptres p ON p.ptres = e.ptres
									where p.ptrano = '".$_SESSION['exercicio']."' and e.prgcod <> '' and rofano = '".$_SESSION['exercicio']."' and ( rof_credrecebidodireta <> 0 or rof_credrecebidoindireta <> 0 )
									group by 										ptrid,
										e.funcod,
										e.sfucod,
										e.prgcod, 	
										e.acacod,
										e.unicod,
										e.loccod
									 ) cred ON cred.ptrid = dtl.ptrid
						WHERE aca.prgano='".$_SESSION['exercicio']."' and aca.acasnrap = false
					  	and aca.unicod IN('".AD."','".CAPES."','".INEP."','".FNDE."','".FIES."','".SUPERVISAOMEC."')
					  ";


$sql_lista .= $where ? $where : ''; 
$sql_lista .= " GROUP BY dtl.ptres,checkbox,descricao,ent.entnome , rofautorizado ,
						rof_credrecebidodireta ,
						rof_credrecebidoindireta 
				ORDER BY 1;";

//dbg($sql_lista,1);


//$teste = $db->carregar($sql_lista);

//dbg($teste,1);

/*
 * Carregando o código da unidade
 */
/*
if($_SESSION['monitora_var']['entid']) {
	
	$sql = "SELECT entunicod, entnome FROM entidade.entidade WHERE entid = '".$_SESSION['monitora_var']['entid']."'";
	$dados = $db->pegaLinha($sql);
	$unicod = $dados['entunicod'];
	$entnome = $dados['entnome'];
	
} else {
	echo "<script>alert('Unidade não encontrada.');window.close();</script>";exit;
}

if(!$unicod) {
	echo "<script>alert('Unidade não encontrada');window.close();</script>";exit;
}
*/
?>
<html>
<head>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?php monta_titulo($entnome, '&nbsp'); ?>
<form action="" method="post" name="formulario">

<table width="100%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" align="right">Programa:</td>
	<td>
	<?
	$sql = "SELECT p.prgcod as codigo, (p.prgcod || ' - ' || p.prgdsc) as descricao
			FROM planejamento.programa p
			WHERE p.prgano = '".$prgano."' ORDER BY prgcod";
	$db->monta_combo('prgcod', $sql, 'S','Selecione','','','',400); 
	?>
	</td>
</tr>	
<tr>
	<td class="SubTituloDireita" align="right">Ação:</td>
	<td>
	<?
	$sql = "SELECT ac.acacod as codigo, (ac.acacod || ' - ' || ac.acadsc) as descricao
 			FROM monitora.acao ac 
 			WHERE ac.prgano = '".$prgano."' AND ac.acastatus = 'A' AND ac.acasnrap = false  
 			GROUP BY ac.acacod, ac.acadsc 
 			ORDER BY ac.acacod";
	$db->monta_combo('acacod', $sql, 'S', 'Selecione','','','',400); 
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" align="right">Buscar:</td>
	<td>
	<? echo campo_texto('buscalivre', "N", "S", "", 67, 150, "", "", '', '', 0, '' ); ?>
	</td>
</tr>
<tr style="background-color: #cccccc">
	<td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
	<td>
	<input type="submit" name="botao" value="Pesquisar"/>
	<input type="button" name="botao" value="Todos" onclick="window.location='/monitora/monitora.php?modulo=principal/planotrabalho/listarProgramaSubAcao&acao=A';"/>
	<input type="button" name="close" value="Fechar" onclick="window.close();">	
	</td>
</tr>		
</table>

</form>
<br>
<?php

$cabecalho = array ("", "PTRES", "Ação", "Unidade Orçamentária", "Dotação inicial", "Detalhado na SubAção", "Dotação Disponível", "Crédito Autorizado", "Crédito Recebido Direto", "Crédito Recebido Indireto" );
$db->monta_lista($sql_lista,$cabecalho,60,20,'','','');

?>
<script type="text/javascript">
/* CARREGANDO OS DADOS DE PTRES */
var tabelaorigem = window.opener.document.getElementById('orcamento');
var idck;
for(i=2;i<tabelaorigem.rows.length-2;i++) {
	
	idck = tabelaorigem.rows[i].id.split("_");

	if(document.getElementById("chk_"+idck[1])) {
		document.getElementById("chk_"+idck[1]).checked=true;
	}
}
/* FIM CARREGANDO OS DADOS DE PTRES */

function resultado(dados, ptrid, ptres){


	if(!ptres) {
		alert('Não existe PTRES. Entre em contato com o administrador do sistema.');
		return false;
	}


	if(dados.checked) {

	var linhaTbl = dados.parentNode.parentNode;
	var tabelaorigem = window.opener.document.getElementById('orcamento');
	if(eval(tabelaorigem.rows.length%2)) {
		var cor = "";
	} else {
		var cor = "#DCDCDC";
	}
	var linha = tabelaorigem.insertRow(2);
	linha.id = "ptrid_"+ptrid;
	linha.style.backgroundColor = cor;
	linha.style.height = '30px';
	
	// setando o ptres
	var celula1 = tabelaorigem.rows[2].insertCell(0);
	celula1.style.textAlign = "center";
	celula1.innerHTML = ptres;
	
	var celula2 = tabelaorigem.rows[2].insertCell(1);
	celula2.style.textAlign = "left";
	celula2.innerHTML = linhaTbl.cells[2].innerHTML+"<input type='hidden' name='ptrid["+ptrid+"]' value='"+ptrid+"'>";
	
	var celula3 = tabelaorigem.rows[2].insertCell(2);
	celula3.style.textAlign = "right";
	celula3.innerHTML = linhaTbl.cells[4].innerHTML;
	
	var celula4 = tabelaorigem.rows[2].insertCell(3);
	celula4.style.textAlign = "right";
	celula4.innerHTML = linhaTbl.cells[5].innerHTML;
	
	var celula5 = tabelaorigem.rows[2].insertCell(4);
	celula5.style.textAlign = "right";
	celula5.innerHTML = linhaTbl.cells[6].innerHTML;

	var celula6 = tabelaorigem.rows[2].insertCell(5);
	celula6.style.textAlign = "right";
	celula6.innerHTML = linhaTbl.cells[7].innerHTML;

	var celula7 = tabelaorigem.rows[2].insertCell(6);
	celula7.style.textAlign = "right";
	celula7.innerHTML = linhaTbl.cells[8].innerHTML;

	var celula8 = tabelaorigem.rows[2].insertCell(7);
	celula8.style.textAlign = "right";
	celula8.innerHTML = linhaTbl.cells[9].innerHTML;

	var celula9 = tabelaorigem.rows[2].insertCell(8);
	celula9.style.textAlign = "center";
	celula9.innerHTML = "<input type=\"text\" name=\"valor["+ptrid+"]\" size=\"28\" maxlength=\"\" value=\"\" onKeyUp=\"this.value=mascaraglobal('###.###.###.###,##',this.value);calculovalorPI();\" onBlur=\"return verificaDisponivel(this,'"+ptrid+"','0');\"  class=\"normal\"  onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" style=\"text-align : right; width:25ex;\" title='' />";
	/*
	switch(ptres) {
		case '001703':
			var unidgest = window.opener.document.getElementById('000000003_0');
			var unidresp = window.opener.document.getElementById('000000003_1');
			unidgest.parentNode.parentNode.style.display = 'none';
			unidresp.parentNode.parentNode.style.display = 'none';
			window.opener.document.getElementById('geradorcompleto').style.display = '';
			window.opener.document.getElementById('regraespecial1').options[1] = new Option("2773","2773");
			window.opener.document.getElementById('regraespecial1').options[2] = new Option("2774","2774");
			window.opener.document.getElementById('btn_selecionar_acaptres').disabled = true;
			window.close();
			break;
		case '001704':
			var unidgest = window.opener.document.getElementById('000000003_0');
			var unidresp = window.opener.document.getElementById('000000003_1');
			unidgest.parentNode.parentNode.style.display = 'none';
			unidresp.parentNode.parentNode.style.display = 'none';
			window.opener.document.getElementById('geradorcompleto').style.display = '';
			window.opener.document.getElementById('regraespecial1').options[1] = new Option("2339","2339");
			window.opener.document.getElementById('regraespecial1').options[2] = new Option("2783","2783");
			window.opener.document.getElementById('regraespecial1').options[3] = new Option("2778","2778");
			window.opener.document.getElementById('btn_selecionar_acaptres').disabled = true;
			window.close();
			break;
	}
	*/
	} else {

	var tabelaorigem = window.opener.document.getElementById('orcamento');
	tabelaorigem.deleteRow(window.opener.document.getElementById('ptrid_'+ptrid).rowIndex);
	window.opener.calculovalorPI();

	}

}


</script>
</body>
</html>

