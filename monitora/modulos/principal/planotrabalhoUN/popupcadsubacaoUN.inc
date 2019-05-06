<html>
<head>
<script type="text/javascript" src="../includes/funcoes.js"></script>

<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
<script type="text/javascript" src="../includes/remedial.js"></script>
<script type="text/javascript" src="../includes/superTitle.js"></script>
	

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
</head>
<?php
$where = "";
if($_POST['acaoForm']){

	extract($_POST);

	$arFiltro = array();
		
	if( $prgcod ){
		array_push($arFiltro, " aca.prgcod = '" .$prgcod. "' ");
	}
	
	if( $acacod ){
		array_push($arFiltro, " aca.acacod = '" .$acacod. "' ");
	}
	
	if( $buscalivre ){
		array_push($arFiltro, " (trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) ilike('%".$buscalivre."%') OR dtl.ptres ilike '%".$buscalivre."%') ");
	}
}

$sql_lista = "SELECT
				'<input type=\"checkbox\" id=\"chk_'||dtl.ptrid||'\" onclick=\"resultado(this,\''||dtl.ptrid||'\',\''||dtl.ptres||'\');\">' as checkbox,
				dtl.ptres,
                trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) as descricao,
                SUM(dtl.ptrdotacao)+0.00 as dotacaoinicial,
                
                coalesce(cast(SUM(dt.valor) as varchar),'0.00'),
                
				coalesce(cast(SUM(dt2.valorpi) as varchar),'0.00') as detalhamento2,
                (SUM(dtl.ptrdotacao)-coalesce(SUM(dt.valor),0.00)) as diferenca
              FROM planejamento.acao aca
              	INNER JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
                INNER JOIN public.unidade uni ON uni.unicod = dtl.unicod
                LEFT JOIN ( select ptrid, sum( sadvalor ) as valor from monitora.pi_subacaodotacao group by ptrid ) dt ON dtl.ptrid = dt.ptrid
				LEFT JOIN ( select ptrid, sum(dtl.valorpi) as valorpi from monitora.v_pi_detalhepiptres dtl group by dtl.ptrid ) dt2 ON dtl.ptrid = dt2.ptrid
              WHERE aca.prgano = '".$_SESSION['exercicio']."' 
                    AND aca.unicod = '".$_SESSION['monitora_var']['unicod']."' 
                    AND aca.acasnrap = false
                    " . ( !empty($arFiltro) ? " AND " . implode(" AND ", $arFiltro) : '' ) . "
              GROUP BY dtl.ptrid, dtl.ptres,
                       trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc),
                       aca.acaid, dt.valor
			";

monta_titulo( $db->pegaUm("SELECT unicod || ' - ' || unidsc FROM public.unidade WHERE unicod='".$_SESSION['monitora_var']['unicod']."'"), ""); 

?>
<form action="" method="post" name="formulario" id="formulario" >
<input type="hidden" name="acaoForm" id="acaoForm" value="1" />
<table width="100%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" align="right">Programa:</td>
		<td>
			<?
			$sql = "SELECT p.prgcod as codigo, (p.prgcod || ' - ' || p.prgdsc) as descricao
					FROM planejamento.programa p
					WHERE p.prgano = '".$_SESSION['exercicio']."' ORDER BY prgcod";
			$db->monta_combo('prgcod', $sql, 'S','Selecione','','','',400); 
			?>
		</td>
	</tr>	

	<tr>
		<td class="SubTituloDireita" align="right">Programa:</td>
		<td>
			<?
			$sql = "SELECT p.prgcod as codigo, (p.prgcod || ' - ' || p.prgdsc) as descricao
					FROM planejamento.programa p
					WHERE p.prgano = '".$_SESSION['exercicio']."' ORDER BY prgcod";
			$db->monta_combo('prgcod', $sql, 'S','Selecione','','','',400); 
			?>
		</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" align="right">Ação:</td>
		<td>
			<?
			$sql = "SELECT ac.acacod as codigo, (ac.acacod || ' - ' || ac.acadsc) as descricao
		 			FROM planejamento.acao ac 
		 			WHERE ac.prgano = '".$_SESSION['exercicio']."' AND ac.acastatus = 'A' AND ac.acasnrap = false  
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
			<input type="button" name="botao" value="Todos" onclick="window.location='/monitora/monitora.php?modulo=principal/planotrabalhoUN/popupcadsubacaoUN&acao=A';"/>
			<input type="button" name="close" value="Fechar" onclick="window.close();">	
		</td>
	</tr>
</table>
</form>
<?php
$cabecalho = array ("", "PTRES", "Descrição", "Dotação autorizada", "Detalhado em subações", "Detalhado em PIs", "Saldo disponível" );
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
		celula3.innerHTML = linhaTbl.cells[3].innerHTML;
		
		var celula4 = tabelaorigem.rows[2].insertCell(3);
		celula4.style.textAlign = "right";
		celula4.innerHTML = linhaTbl.cells[4].innerHTML;
		
		var celula5 = tabelaorigem.rows[2].insertCell(4);
		celula5.style.textAlign = "right";
		celula5.innerHTML = linhaTbl.cells[5].innerHTML;
		
		var celula6 = tabelaorigem.rows[2].insertCell(5);
		celula6.style.textAlign = "right";
		celula6.innerHTML = linhaTbl.cells[6].innerHTML;

		
		var celula7 = tabelaorigem.rows[2].insertCell(6);
		celula7.style.textAlign = "center";
		celula7.innerHTML = "<input type=\"text\" name=\"valor["+ptrid+"]\" size=\"28\" maxlength=\"\" value=\"\" onKeyUp=\"this.value=mascaraglobal('###.###.###.###,##',this.value);calculovalorPI();\" onBlur=\"return verificaDisponivel(this,'"+ptrid+"','0');\"  class=\"normal\"  onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" style=\"text-align : right; width:25ex;\" title='' />";
	
	} else {

		var tabelaorigem = window.opener.document.getElementById('orcamento');
		tabelaorigem.deleteRow(window.opener.document.getElementById('ptrid_'+ptrid).rowIndex);
		window.opener.calculovalorPI();

	}

}

</script>
</body>
</html>
<div id="dibDegug"></div>