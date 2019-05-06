<?php

if($_POST['exporta'] == "true"){
	global $db;
	header('content-type: text/html; charset=ISO-8859-1');
	$cabecalho = array( "Código" , "Unidade Gestora", "Entidade", "Subação", "Título", "Ptres", "Produto", "Unidade Medida", "Ação", "Programa", "Descrição da Ação", "Dotação", "Autorizada", "Empenhado", "Executado");
	
	$sql_financeiro = $_SESSION['monitora']['sql_financeiro'];
	/*
	$sql_financeiro = str_replace("ex.dotacao","replace(cast(ex.dotacao as varchar),'.',',') as v1", $sql_financeiro);
	$sql_financeiro = str_replace("ex.autorizada","replace(cast(ex.autorizada as varchar),'.',',') as v2", $sql_financeiro);
	$sql_financeiro = str_replace("ex.empenhado","replace(cast(ex.empenhado as varchar),'.',',') as v3", $sql_financeiro);
	$sql_financeiro = str_replace("ex.executado","replace(cast(ex.executado as varchar),'.',',') as v4", $sql_financeiro);
	*/
	/*
	$sql_financeiro = str_replace("ex.dotacao"," replace(cast(to_char(ex.dotacao, '99999999999D99')::money as varchar),'R$','') as v1", $sql_financeiro);
	$sql_financeiro = str_replace("ex.autorizada"," replace(cast(to_char(ex.autorizada, '99999999999D99')::money as varchar),'R$','') as v2", $sql_financeiro);
	$sql_financeiro = str_replace("ex.empenhado"," replace(cast(to_char(ex.empenhado, '99999999999D99')::money as varchar),'R$','') as v3", $sql_financeiro);
	$sql_financeiro = str_replace("ex.executado"," replace(cast(to_char(ex.executado, '99999999999D99')::money as varchar),'R$','') as v4", $sql_financeiro);
	*/
	//dbg($sql_financeiro,1);
	$tipo_campo = array();
	for( $i = 0; $i < 15; $i++ )
	{
		if($i>10){
			array_push( $tipo_campo, 'n' );
		}
		else{
			array_push( $tipo_campo, 's' );
		}
		
	}
	//dbg($tipo_campo,1);
	//$db->sql_to_excel( $consulta->montaRequisicao(), 'relorc', '', $tipo_campo );	
	
	$db->sql_to_excel($sql_financeiro, 'relpiplan', $cabecalho, $tipo_campo);
	$exporta = "false";
}



if ( $_POST['requisicao'] == 1 ){
		
	$filtro = array();		
	
	if( $_POST["acaid"] ){
		$aux = implode(",", $_POST['acaid'] );  
		array_push($filtro, " a.acaid in (" . $aux . ") ");
	}	
	if( $_POST["entid"] ){
		$aux = implode(",", $_POST['entid'] );  
		array_push($filtro, " e.entid in (" . $aux . ") ");
	}	
	if( $_POST["exefinan"] ){
		array_push($filtro, " s.exeid = " . $_POST['exefinan'] . " ");
	}		
	if( $_POST["gestorsub"] ){
		array_push($filtro, " s.gstid = " . $_POST['gestorsub'] . " ");
	}		
	if( $_POST["ptres"] ){
		array_push($filtro, " p.ptrid = " . $_POST['ptres'] . " ");
	}
	if( $_POST["prgcod"] ){
		array_push($filtro, " pr.prgcod = '" . $_POST['prgcod'] . "' ");
	}
}

// Criação das combos do PI
define('COMBO_REGRAGERAL', 5);

define("AD"   ,26101);
define("CAPES",26291);
define("INEP" ,26290);
define("FNDE" ,26298);

// Cabeçalho e título
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Relatório de Planejamento', '' );
?>
<html>

<script src="/includes/prototype.js"></script>
<body>
<form id="formulario" name="formulario" method="post" action="">
	<input type=hidden name="numero" value="1">
	<input type=hidden name="requisicao" id="requisicao" value="0">
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"	align="center">
		<tr>
			<td  bgcolor="#CCCCCC" colspan="2"><b>Filtros do Relatório</b></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Unidades Orçamentárias:</td>
			<td>
			<?php
			$sqlComboEntidade = "SELECT ent.entid as codigo,
							   			CASE fen.funid
							   			WHEN 34 THEN entungcod ||' - '|| entnome 
							   					ELSE entunicod ||' - '|| entnome 
							   					END as descricao
							   FROM entidade.entidade ent 
							   LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid  
							   WHERE entunicod IN('".AD."','".CAPES."','".INEP."','".FNDE."') and fen.funid <> 33 and ent.entid NOT IN ('391293','391294') ORDER BY entunicod";
		
			combo_popup( "entid", $sqlComboEntidade, "Unidades Orçamentárias", "600x600", 0, array(), "", "S", false, false, 5, 400 );
			?>
			</td>
		</tr>			
		<tr>
			<td class="SubTituloDireita" valign="top">Ações:</td>
			<td>
			<?php
			$sqlComboAcoes = "SELECT
								aca.acaid as codigo,
								'(PTRES:'||pt.ptres||') - '|| aca.unicod ||'.'|| aca.prgcod ||'.'|| aca.acacod ||' - '|| aca.acadsc as descricao
							  FROM monitora.acao aca 
							  	inner join monitora.ptres pt on pt.acaid=aca.acaid
							  WHERE aca.prgano='".$_SESSION['exercicio']."' and aca.acasnrap = false
							  	and aca.unicod IN('".AD."','".CAPES."','".INEP."','".FNDE."')
							  GROUP BY codigo , descricao
							  ORDER BY 
								1 ";
			combo_popup( "acaid", $sqlComboAcoes, "Ações", "600x600", 0, array(), "", "S", false, false, 5, 400 );
			?>
			</td>
		</tr>		
		<tr>
			<td align='right' class="SubTituloDireita">Executor Orçamentário e Financeiro:</td>
		    <td>
		    	
				<?
				//$campo3 = $_REQUEST['campo3'];
				$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao FROM public.combodadostabela WHERE ctbid = 3 order by cdtcod";  
				$db->monta_combo('exeFinan', $sql, 'S', 'Selecione', '', '', '', '100', 'N', 'exeFinan'); 
				?>    	
		    </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Gestor da Subação:</td>
		    <td>
				<?
				$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao FROM public.combodadostabela WHERE ctbid = 4 order by cdtcod";  
				$db->monta_combo('gestorSub', $sql, 'S', 'Selecione', '', '', '', '100', 'N', 'gestorSub'); 
				?>    	
		    </td>
		</tr>
		<tr>
			<td class='SubTituloDireita'>PTRES:</td>
			<td>
				<?
				$sql = "select ptrid as codigo, ptres as descricao 
						from monitora.ptres
						where ptrano = '".$_SESSION['exercicio']."' 
						 and ptres != ''
						order by ptres";

				$db->monta_combo('ptres', $sql, 'S', 'Selecione', '', '', '', '140', 'N', 'ptres'); 
				?>
			</td>
		</tr>
		<tr>
			<td class='SubTituloDireita'>Programa:</td>
			<td>
				<?
				$sql = "SELECT prgcod as codigo, 
						   prgdsc as descricao 
						FROM planejamento.programa
						WHERE prgano = '".$_SESSION['exercicio']."'
						order by descricao";

				$db->monta_combo('prgcod', $sql, 'S', 'Selecione', '', '', '', '240', 'N', 'prgcod'); 
				?>
			</td>
		</tr>
		<tr>
			<th colspan="2">
				<input type="button" value="Pesquisar" onclick="pesquisa();" style="cursor: pointer;"/>
			</th>
		</tr>	
	</table>
	<input type="hidden" id="exporta" name="exporta" value="<?=$exporta; ?>">
</form>

<?
global $db;
	//ver($filtro, d);
	// Filtros do relatório
	if ( $_POST['requisicao'] == 1 ){	

	$sql_montado = "";
	$sql_montado = "SELECT DISTINCT 
						entunicod::character(5), 
						entungcod::character(6), 
						entnome::text, 
						s.sbacod::character(4), 
						s.sbatitulo::character(255),
						p.ptres::character(6), 
						prod.prodsc,
						um.unmdsc,
						cast(p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.loccod as text) as acao, 
						pr.prgdsc,
						a.acadsc,
						ex.dotacao, 
						ex.autorizada, 
						ex.empenhado, 
						ex.executado
					FROM
					monitora.subacao s 
					INNER JOIN monitora.subacaounidade sau 
					  ON (sau.sbaid = s.sbaid)
					INNER JOIN monitora.subacaodotacao sad 
					  ON (sad.sbaid = s.sbaid)
					INNER JOIN monitora.ptres p 
					  ON (p.ptrid = sad.ptrid)
					INNER JOIN monitora.acao a 
					  ON (a.acaid = p.acaid)
					INNER JOIN public.unidademedida um 
					  ON (um.unmcod = a.unmcod)
					INNER JOIN public.produto prod 
					  ON (prod.procod = a.procod)
					INNER JOIN planejamento.programa pr 
					  ON (pr.prgcod = a.prgcod)
					INNER JOIN ( SELECT TRIM(ptres)::character(6) as ptres, 
					   				sum(rofdot_ini) as dotacao, 
					                sum(  rofpl + rofpl_credadicionais ) as autorizada,
									sum(rofempenhado) as empenhado, 
					                sum(rofpago) as executado 
								 FROM financeiro.execucao
								 WHERE rofano = '".$_SESSION['exercicio']."'
								 GROUP BY TRIM(ptres) ) ex 
					  ON ex.ptres = p.ptres 
					INNER JOIN entidade.entidade e 
					  ON (e.entid = sau.entid)
					WHERE p.ptrano = '".$_SESSION['exercicio']."' " . ( !empty($filtro) ? "AND" . implode(" AND ", $filtro) : '' ) . "
					ORDER BY sbacod ";
	//dbg($sql_montado,1);
	$_SESSION['monitora']['sql_financeiro'] = $sql_montado;	
	$cabecalho = array( "Código" , "Unidade Gestora", "Entidade", "Subação", "Título", "Ptres", "Produto", "Unidade Medida", "Ação", "Programa", "Descrição da Ação", "Dotação", "Autorizada", "Empenhado", "Executado");
	//$db->monta_lista( $sql_montado, $cabecalho, 50, 10, 'N', '', '', 'formFinanceiro');
	
	
	print('	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"	align="center">
		<tr>
			<th style="text-align: left;"><input type="button" value="Exportar para Excel" onclick="exportar();" style="cursor: pointer;"/></th>
		</tr>
	</table>');
	$db->monta_lista($sql_montado, $cabecalho, 50, 4, 'N','Center','');
	
	}
?>

<div id="listaExporta" style="display: none">

</div>
<div id="lista"></div>
<script>		
	function exportar(){
		$('exporta').value = "true";
		document.formulario.submit();
	}
	function pesquisa(){
		var achou = "";
		var form = document.getElementById("formulario");
 		
	 	$('exporta').value = "false";
	 	
	 	if(form.acaid.options[0].value != "")
			selectAllOptions( form.acaid );
		if(form.entid.options[0].value != "")
	 		selectAllOptions( form.entid );
	
		$('requisicao').value = "1";
		
	 	/*$('loader-container').show();
	 	var req = new Ajax.Request('monitora.php?modulo=relatorio/pi/relatorio_financeiro&acao=A', {
		        method:     'post',
		        parameters: '&pesquisaAjax=true&'+formulario.serialize(),
		        asynchronous: false,
		        onComplete: function (res){
		        	achou = res.responseText.indexOf( "Não foram encontrados Registros." );
		        	if(achou == -1){
		        		$('listaExporta').style.display = '';
		        	}else{
		        		$('listaExporta').style.display = 'none';
		        	}
					$('lista').innerHTML = res.responseText;
		        }
		  });
	 	$('loader-container').hide();*/
	 	
	 	form.submit();
 	}
	</script>
</body>
</html>