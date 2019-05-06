<?php

// Filtros do relatório
if ( $_REQUEST['requisicao'] == 1 ){
	
	$filtro = array();
	
	
	if( $_REQUEST["acaid"][0] ){
		$aux = implode(",", $_REQUEST['acaid'] );  
		array_push($filtro, " pi.acaid in (" . $aux . ") ");
	}

	if( $_REQUEST["entid"][0] ){
		$aux = implode(",", $_REQUEST['entid'] );  
		array_push($filtro, " e.entid in (" . $aux . ") ");
	}

	if( $_REQUEST["campo3"] ){
		array_push($filtro, " sub.exeid = " . $_REQUEST['campo3'] . " ");
	}
	
	if( $_REQUEST["campo4"] ){
		array_push($filtro, " sub.gstid = " . $_REQUEST['campo4'] . " ");
	}
	
	if( $_REQUEST["campo5"] ){
		array_push($filtro, " sub.eqdid = " . $_REQUEST['campo5'] . " ");
	}
	
	if( $_REQUEST["campo6"] ){
		//$posicao = strpos($_REQUEST["campo6"], '|');
		//$campo6 = substr($_REQUEST["campo6"], 0, $posicao);
		//array_push($filtro, " pl.dpiid = " . $campo6 . " ");
		array_push($filtro, " pi.dpiid = " . $_REQUEST["campo6"] . " ");
	}
	
	if( $_REQUEST["campo7"] ){
		//$posicao = strpos($_REQUEST["campo7"], '|');
		//$campo7 = substr($_REQUEST["campo7"], 0, $posicao);
		//array_push($filtro, " pl.cpiid = " . $campo7 . " ");
		array_push($filtro, " pi.cpiid = " . $_REQUEST["campo7"] . " ");
	}
	
	if( $_REQUEST["plisituacao"] ){
		array_push($filtro, " pi.plisituacao = '" . $_REQUEST['plisituacao'] . "' ");
	}
	
}

// Criação das combos do PI
define('COMBO_REGRAGERAL', 5);

define("AD"   ,26101);
define("CAPES",26291);
define("INEP" ,26290);
define("FNDE" ,26298);

include APPRAIZ."/includes/controlecombobox.inc";

$parametroconstrutor = array();
if(is_array($_REQUEST['dados'])) {
	$parametroconstrutor = array_merge($_REQUEST['dados'], array());
}
$combo = new ControleComboBox($parametroconstrutor);

// Cabeçalho e título
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Relatório de Plano Interno', 'Clique no código para obter maiores informações sobre Plano Interno' );

//faz o controle do registro atual para a paginação
$numero = !$_REQUEST['numero'] ? 1 : intval( $_REQUEST['numero'] );
//Registros por página
$perpage = 70;
//Qtde de Paginas que serão mostradas na barrinha
$pages = 50;

?>

<script>
	function pagina( numero ){
	 	document.formulario.numero.value = numero;
	 	document.formulario.submit();
 	}
 	
 	function filtra_pi(){
 		var form = document.getElementById("formulario");
 		
		selectAllOptions( form.entid );
		selectAllOptions( form.acaid );
		 		
 		form.submit();
 	}
 	
 	function abredadosobs( cod ){
 		window.open('?modulo=relatorio/pi/dadosobs&acao=A&pi=' + cod,'Observação','width=700, height=300');
	}
 	
</script>
<script src="../includes/wz_tooltip.js"></script>
<script src="/includes/prototype.js"></script>
<form id="formulario" name="formulario" method="post" action="">
	<input type=hidden name="numero" value="1">
	<input type=hidden name="requisicao" value="1">
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
								  FROM planejamento.acao aca 
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

		<? //echo $combo->pegarComboHTML(COMBO_REGRAGERAL); ?>
		
			<tr>
				<td align='right' class="SubTituloDireita">Executor Orçamentário e Financeiro:</td>
			    <td>
			    	
					<?
					//$campo3 = $_REQUEST['campo3'];
					$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao FROM public.combodadostabela WHERE ctbid = 3 order by cdtcod";  
					$db->monta_combo('campo3', $sql, 'S', 'Selecione', '', '', '', '100', 'N', '000000003_0'); 
					?>    	
			    </td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Gestor da Subação:</td>
			    <td>
			    	
					<?
					//$campo4 = $_REQUEST['campo4'];
					$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao FROM public.combodadostabela WHERE ctbid = 4 order by cdtcod";  
					$db->monta_combo('campo4', $sql, 'S', 'Selecione', '', '', '', '100', 'N', '000000003_1'); 
					?>    	
			    </td>
			</tr>
		
		
				<tr>
					<td class='SubTituloDireita'>Enquadramento da Despesa:</td>
					<td>
						<?
						//$campo5 = $_REQUEST['campo5'];
						$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
							    FROM public.combodadostabela where ctbid=5 and cdtstatus='A' order by cdtcod";
						
						$db->monta_combo('campo5', $sql, 'S', 'Selecione', '', '', '', '240', 'N', '000000005_0'); 
						?>
					</td>
				</tr>
				<tr>
					<td class='SubTituloDireita'>Nível/Etapa/Modalidade de ensino:</td>
					<td>
						<?
						//$campo6 = $_REQUEST['campo6'];
						$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
							    FROM public.combodadostabela where ctbid=6 and cdtstatus='A' order by cdtcod";
						
						$db->monta_combo('campo6', $sql, 'S', 'Selecione', '', '', '', '240', 'N', '000000005_1');
						?>
					</td>
				</tr>
				<tr>
					<td class='SubTituloDireita'>Categoria de Apropriação:</td>
					<td>
						<?
						//$campo7 = $_REQUEST['campo7'];
						$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
							    FROM public.combodadostabela where ctbid=7 and cdtstatus='A' order by cdtcod";
						
						$db->monta_combo('campo7', $sql, 'S', 'Selecione', '', '', '', '340', 'N', '000000005_2');
						?>
					</td>
				</tr>	
		
		<tr>
			<td class="SubTituloDireita">Situação:</td>
			<td>
				<input type="radio" name="plisituacao" id="plisituacao" value="" /> Qualquer
				<input type="radio" name="plisituacao" id="plisituacao" value="C" /> Aprovado
				<input type="radio" name="plisituacao" id="plisituacao" value="H" /> Homologado
				<input type="radio" name="plisituacao" id="plisituacao" value="R" /> Enviado para Revisão
				<input type="radio" name="plisituacao" id="plisituacao" value="S" /> Cadastrado no SIAFI
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td></td>
			<td>
				<input type="button" value="Pesquisar" onclick="filtra_pi();" style="cursor: pointer;"/>
				<input type="button" value="Ver Todos" onclick="location.href='?modulo=relatorio/pi/relatorio_unidade&acao=A';" style="cursor: pointer;"/>
			</td>
		</tr>	
	</table>
	
</form>

	<?php
		
		$sql = "";
/*
		$sql = "SELECT distinct
					pi.plicod as codigo,
					CASE WHEN trim(pi.plititulo) is not null THEN '<a title=\"Visualizar Dados\" style=\"cursor:pointer;\" onclick=\"mostradadospi(\'' || pi.plicod || '\', \'consulta\');\">' || pi.plititulo  || '</a><input type=\"hidden\" id=\"plititulo[' || pi.plicod || ']\" value=\"' || pi.plicod || ' - ' || pi.plititulo || '\">'
						 ELSE '<a title=\"Visualizar Dados\" style=\"cursor:pointer;\" onclick=\"mostradadospi(\'' || pi.plicod || '\',  \'consulta\');\">Não Preenchido</a><input type=\"hidden\" id=\"plititulo[' || pi.plicod || ']\" value=\" ' || pi.plicod ||' - Não Preenchido\"/>' END as titulo,
					ma.acadsc as acao,
					CASE WHEN pi.plisituacao = 'P' THEN ' Pendente '
						 WHEN pi.plisituacao = 'C' THEN ' Aprovado '
						 WHEN pi.plisituacao = 'H' THEN ' Homologado '
						 WHEN pi.plisituacao = 'R' THEN ' <a title=\"Visualizar Dados da Revisão\" style=\"cursor:pointer;\" onclick=\"abredadosobs(\'' || pi.plicod || '\');\" >Enviado para Revisão</a> ' 
					END as situacao,
					to_char(pi.plidata, 'DD/MM/YYYY') as data
				FROM
					monitora.planointerno pi
				INNER JOIN
					monitora.subacao sub ON sub.sbaid = pi.sbaid
				INNER JOIN
					planejamento.acao ma ON ma.acaid = pi.acaid
				INNER JOIN
					unidade u ON u.unicod = ma.unicod
				INNER JOIN
					monitora.usuarioresponsabilidade mu ON ma.unicod = mu.unicod 
				WHERE
					--mu.usucpf = '' AND
					pi.plistatus = 'A' " . ( !empty($filtro) ? "AND" . implode(" AND ", $filtro) : '' ) . "
				--GROUP BY
					--pi.plicod, pi.plititulo, u.unidsc, ma.acadsc, pi.plisituacao, pi.plidata
				--ORDER BY
					--ma.acadsc
					";
*/
		$sql = "SELECT distinct
					pi.plicod as codigo,
					pi.pliptres as ptres,
					CASE WHEN trim(pi.plititulo) is not null THEN '<a title=\"Visualizar Dados\" style=\"cursor:pointer;\" onclick=\"mostrapi2(\'' || pi.plicod || '\', \'consulta\');\">' || pi.plititulo  || '</a><input type=\"hidden\" id=\"plititulo[' || pi.plicod || ']\" value=\"' || pi.plicod || ' - ' || pi.plititulo || '\">'
						 ELSE '<a title=\"Visualizar Dados\" style=\"cursor:pointer;\" onclick=\"mostrapi2(\'' || pi.plicod || '\',  \'consulta\');\">Não Preenchido</a><input type=\"hidden\" id=\"plititulo[' || pi.plicod || ']\" value=\" ' || pi.plicod ||' - Não Preenchido\"/>' END as titulo,
					ma.acadsc as acao,
					e.entnome as entnome,
					CASE WHEN pi.plisituacao = 'P' THEN ' Pendente '
						 WHEN pi.plisituacao = 'S' THEN ' Cadastrado no SIAFI '
						 WHEN pi.plisituacao = 'C' THEN ' Aprovado '
						 WHEN pi.plisituacao = 'H' THEN ' Homologado '
						 WHEN pi.plisituacao = 'R' THEN ' <a title=\"Visualizar Dados da Revisão\" style=\"cursor:pointer;\" onclick=\"abredadosobs(\'' || pi.plicod || '\');\" >Enviado para Revisão</a> ' 
					END as situacao,
					to_char(pi.plidata, 'DD/MM/YYYY') as data
				FROM
					monitora.planointerno pi
				left JOIN
					monitora.subacao sub ON sub.sbaid = pi.sbaid
				left JOIN
					monitora.subacaounidade su ON su.sbaid = sub.sbaid
				left JOIN
					entidade.entidade e ON e.entid = su.entid
				left JOIN
					planejamento.acao ma ON ma.acaid = pi.acaid
				WHERE
					pi.plistatus = 'A' " . ( !empty($filtro) ? "AND" . implode(" AND ", $filtro) : '' ) . "
				ORDER BY
					ma.acadsc
					";
		
		$cabecalho = array( "Código" , "Ptres", "Título", "Ação", "Unidade","Situação","Data de Inclusão");
		$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
		//dbg($sql,1);
		// Dados da Paginação
		/*
		$RS = $db->carregar($sql);
		$nlinhas = count($RS);
		$nl = !$RS ? 0 : $nlinhas;
		$reg_fim = (($numero + $perpage) > $nlinhas) ? $nlinhas : ($numero + $perpage -1);
		$total_reg = $nlinhas;
		
		if ( $RS ){
			echo '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">'
				 . '<thead><tr>'
				 . 		'<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Código</strong></label>'
				 . 		'<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Ptres</strong></label>'
				 . 		'<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Título</strong></label>'
				 . 		'<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Ação</strong></label>'
				 . 		'<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Unidade</strong></label>'
				 . 		'<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Situação</strong></label>'
				 . 		'<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Data de Inclusão</strong></label>'
				 . '</tr> </thead>';
			
			for( $i = 0; $i < count($RS); $i++ ){
				
				$cor = ($i % 2) ? '' : '#F7F7F7';
				
				echo '<tr bgcolor="'.$cor.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$cor.'\';">
						 <td align="center">
							<a title="Visualizar Dados do PI" style="cursor:pointer;" onclick="mostradadospi(\'' . $RS[$i]['codigo'] . '\',  \'consulta\');">' .$RS[$i]['codigo']. '</a>
						 </td>
						 <td>'.$RS[$i]['ptres'].'</td>
						 <td>'.$RS[$i]['titulo'].'</td>
						 <td>'. $RS[$i]['acao'] .'</td>
						 <td>'. $RS[$i]['entnome'] .'</td>
						 <td align="center">'.$RS[$i]['situacao'].'</td>
						 <td align="center">'.$RS[$i]['data'].'</td>
					  </tr>';	
			}
			
			echo '	<tr>
						<td colspan="7" align="right" bgcolor="#dcdcdc" style="padding:1px;">';
							?> <?php include APPRAIZ . "includes/paginacao.inc"; ?> <?php
			echo '		</td>
				 	 </tr>
				 </table>';
			
		}else{
			echo '<table width="95%;" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
					<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>
				  </table>';
		}
		*/
	?>

	<script>
		/**
		 * Abre os dados do PI
		 */
		function mostrapi2( id, tipoacao ){
				var janela = window.open( '?modulo=principal/planointerno/dados_pi&acao=A&plicod=' + id + '&tipoacao=' + tipoacao, 
							 			  'Dados do PI','scrollbars=yes, width=800, height=650 ');
				janela.focus();
			}	
	</script>