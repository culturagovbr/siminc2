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

$where .= $_REQUEST['sbaid'] ? "AND dt.sbaid = '".$_REQUEST['sbaid']."'" : '';
$where .= $_POST['prgcod'] ? "AND UPPER(dtl.prgcod) LIKE('%".strtoupper($_POST['prgcod'])."%')" : '';
$where .= $_POST['acacod'] ? "AND UPPER(dtl.acacod) LIKE('%".strtoupper($_POST['acacod'])."%')" : '';
$where .= $_POST['buscalivre'] ? "AND (trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) ilike('%".$_POST['buscalivre']."%') OR dtl.ptres ilike '%".$_POST['buscalivre']."%')" : '';


	$sql_lista = "SELECT
                            '<input type=\"checkbox\" id=\"chk_'||dtl.ptrid||'\" onclick=\"resultado(this,\''||dtl.ptrid||'\',\''||dtl.ptres||'\');\">' as checkbox,
                            dtl.ptres,
                            trim(aca.prgcod||'.'||aca.acacod||'.'||aca.unicod||'.'||aca.loccod||' - '||aca.acadsc) as descricao,
                            uni.unidsc,
                            SUM(dtl.ptrdotacao) as dotacaoinicial,
                            coalesce(cast(SUM(dt.valor) as varchar),'0.00') as detalhamento,
                            '<div id=\"detalhamentoPI_'||dtl.ptrid||'\" style=\"color: rgb(0, 102, 204);\" align=\"right\" > ' || Case when trim(to_char( coalesce(SUM(dt2.valorpi),0) , '999G999G999G999D99')) = ',00' then '0,00' else to_char( coalesce(SUM(dt2.valorpi),0) , '999G999G999G999D99') || '</div>' end as detalhamento2,
                            COALESCE(SUM(pte.total),0.00) as empenhado,						
                            (SUM(dtl.ptrdotacao)- coalesce(SUM(dt.valor),0.00)) as diferenca 	
			FROM monitora.acao aca 
			JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid AND dtl.ptrano = '{$_SESSION['exercicio']}'
			JOIN public.unidade uni ON uni.unicod = dtl.unicod 					  
			LEFT JOIN ( select ptrid,
                            sum( sadvalor ) as valor
                            from  monitora.pi_subacaodotacao
                            group by ptrid ) dt ON dtl.ptrid = dt.ptrid 
			LEFT JOIN ( select ptrid, 
                            sum(dtl.valorpi) as valorpi	
                            from monitora.v_pi_detalhepiptres dtl
                            GROUP BY dtl.ptrid ) dt2 ON dtl.ptrid = dt2.ptrid
                        LEFT JOIN siafi.ptrempenho pte ON pte.ptres = dtl.ptres AND pte.exercicio = '{$_SESSION['exercicio']}'
			WHERE aca.prgano='".$_SESSION['exercicio']."' and aca.acasnrap = false  
                            and aca.unicod IN('".AD."','".CAPES."','".INEP."','".FNDE."','".FIES."','".SUPERVISAOMEC."')
					  ";


$sql_lista .= $where ? $where : ''; 
$sql_lista .= " GROUP BY dtl.ptrid,dtl.ptres,checkbox,descricao,uni.unidsc 
				ORDER BY 1;";
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
	<input type="button" name="botao" value="Todos" onclick="window.location='/monitora/monitora.php?modulo=principal/planotrabalhoUG/popcadsubacaoUG&acao=A';"/>
	<input type="button" name="close" value="Fechar" onclick="window.close();">	
	</td>
</tr>		
</table>

</form>
<br>
<?php

$cabecalho = array (
                    "", 
                    "PTRES", 
                    "Ação", 
                    "Unidade Orçamentária", 
                    "Dotação Autorizada (R$)", 
                    "Detalhado em Subação (R$)", 
                    "Detalhado em PI (R$)", 
                    "Empenhado (R$)", 
                    "Saldo não Orçado em Subação (R$)");
$db->monta_lista($sql_lista,$cabecalho,60,20,'','','');

?>
<script type="text/javascript">
/* CARREGANDO OS DADOS DE PTRES */
function Trim(str){return str.replace(/^\s+|\s+$/g,"");}

var tabelaorigem = window.opener.document.getElementById('orcamento');
var idck;
for(i=2;i<tabelaorigem.rows.length-2;i++) {
	
	idck = tabelaorigem.rows[i].id.split("_");
	
	if(document.getElementById("chk_"+idck[1])) {
		document.getElementById("chk_"+idck[1]).checked=true;
		
		//var detalhamentoPI = Trim(document.getElementById("detalhamentoPI_"+idck[1]).innerHTML);
		//detalhamentoPI = parseFloat(replaceAll(replaceAll(detalhamentoPI,".",""),",","."));
		//if(detalhamentoPI > 0){
		//	document.getElementById("chk_"+idck[1]).disabled = 'disabled';
		//}
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
	celula8.style.textAlign = "center";
	celula8.innerHTML = "<input type=\"text\" name=\"valor["+ptrid+"]\" size=\"28\" maxlength=\"\" value=\"\" onKeyUp=\"this.value=mascaraglobal('###.###.###.###,##',this.value);calculovalorPI();\" onBlur=\"return verificaDisponivel(this,'"+ptrid+"','0');\"  class=\"normal\"  onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" style=\"text-align : right; width:25ex;\" title='' />";

	} else {

	var tabelaorigem = window.opener.document.getElementById('orcamento');
	tabelaorigem.deleteRow(window.opener.document.getElementById('ptrid_'+ptrid).rowIndex);
	window.opener.calculovalorPI();

	}

}

</script>
</body>
</html>