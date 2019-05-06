<?php
/*
 * Verifique se estão sendo passados tais parâmetros. Se passados, verificar se estão sendo realmente usados.   
 * 		$pieid = $_REQUEST['pieid']; ---> ao invés de 'campo3'
 * 		$pigid = $_REQUEST['pigid']; ---> ao invés de 'campo4' 
 * 		$eqdid = $_REQUEST['eqdid']; ---> ao invés de 'campo5' 
 *		'campo6' inutilizado previamente
 * 		$capid = $_REQUEST['capid']; ---> ao invés de 'campo7'
 */

// Filtros do relatório
if ( $_REQUEST['requisicao'] == 1 ){
	
	$filtro = array();
	
	
	if( $_REQUEST["acaid"][0] && $_REQUEST["acaid"][0] != 'A' ){
		$aux = implode(",", $_REQUEST['acaid'] );  
		array_push($filtro, " pi.acaid in (" . $aux . ") ");
	}

	if( $_REQUEST["ungcod"][0] && $_REQUEST["ungcod"][0] != 'A' ){
		$aux = implode(",", $_REQUEST['ungcod'] );  
		array_push($filtro, " pi.ungcod in ('" . $aux . "') ");
	}

	if( $_REQUEST["pieid"] ){
		array_push($filtro, " pi.pieid = " . $_REQUEST['pieid'] . " ");
	}
	
	if( $_REQUEST["pigid"] ){
		array_push($filtro, " pi.pigid = " . $_REQUEST['pigid'] . " ");
	}
	
	if( $_REQUEST["eqdid"] ){
		array_push($filtro, " pi.eqdid = " . $_REQUEST['eqdid'] . " ");
	}
	
//	if( $_REQUEST["campo6"] ){
//		//$posicao = strpos($_REQUEST["campo6"], '|');
//		//$campo6 = substr($_REQUEST["campo6"], 0, $posicao);
//		//array_push($filtro, " pl.dpiid = " . $campo6 . " ");
//		array_push($filtro, " pi.dpiid = " . $_REQUEST["campo6"] . " ");
//	}
	
	if( $_REQUEST["capid"] ){
		//$posicao = strpos($_REQUEST["campo7"], '|');
		//$campo7 = substr($_REQUEST["campo7"], 0, $posicao);
		//array_push($filtro, " pl.cpiid = " . $campo7 . " ");
		array_push($filtro, " pi.capid = " . $_REQUEST["capid"] . " ");
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
 		
		selectAllOptions( form.ungcod );
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
				$sqlComboEntidade = "SELECT 
										ungcod AS codigo, 
										CASE WHEN (unicod = '26101') THEN 
												ungcod 
											 ELSE 
											 	unicod 
											 END ||' - '|| ungdsc as descricao 
									 FROM 
									 	public.unidadegestora where ungstatus = 'A' $stWhere ";  
				
				combo_popup( "ungcod", $sqlComboEntidade, "Unidades Orçamentárias", "600x600", 0, array(), "", "S", false, false, 5, 400 );				
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
									1";
				combo_popup( "acaid", $sqlComboAcoes, "Ações", "600x600", 0, array(), "", "S", false, false, 5, 400 );				
				?>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Executor Orçamentário e Financeiro:</td>
			    <td>
			    	
					<?
					$pieid = $_REQUEST['pieid'];
					$sql = "SELECT pieid as codigo, piecod ||' - '|| piedsc as descricao FROM  monitora.pi_executor";
					$db->monta_combo('pieid', $sql, 'S', 'Selecione', '', '', '', '100', 'N', '000000003_0');					
					?>    	
			    </td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Gestor da Subação:</td>
			    <td>
			    	
					<?
					$pigid = $_REQUEST['pigid'];
					$sql = "SELECT pigid as codigo, pigcod ||' - '|| pigdsc as descricao FROM  monitora.pi_gestor";  
					$db->monta_combo('pigid', $sql, 'S', 'Selecione', '', '', '', '100', 'N', '000000003_1');					 
					?>    	
			    </td>
			</tr>
		
		
				<tr>
					<td class='SubTituloDireita'>Enquadramento da Despesa:</td>
					<td>
						<?
						$eqdid = $_REQUEST['eqdid'];
						$sql = "SELECT eqdid as codigo, eqdcod ||' - '|| eqddsc as descricao FROM planejamento.enquadramento_despesa WHERE eqdano = '{$_SESSION['exercicio']}' ORDER BY eqdcod";
						$db->monta_combo('eqdid', $sql, 'S', 'Selecione', '', '', '', '240', 'N', '000000005_0');						 
						?>
					</td>
				</tr>
				<tr>
					<td class='SubTituloDireita'>Nível/Etapa/Modalidade de ensino:</td>
					<td>
						<?
						$sql = "SELECT neeid as codigo, neecod ||' - '|| needsc as descricao FROM monitora.pi_niveletapaensino WHERE neeano = '{$_SESSION['exercicio']}' ORDER BY neecod ";						
						$db->monta_combo('neeid', $sql, 'S', 'Selecione', '', '', '', '240', 'N', '000000005_1');
						?>
					</td>
				</tr>
				<tr>
					<td class='SubTituloDireita'>Categoria de Apropriação:</td>
					<td>
						<?
						$capid = $_REQUEST['capid'];
						$sql = "SELECT capid as codigo, capcod ||' - '|| capdsc as descricao FROM monitora.pi_categoriaapropriacao WHERE capano = '{$_SESSION['exercicio']}' ORDER BY capcod ";
						$db->monta_combo('capid', $sql, 'S', 'Selecione', '', '', '', '340', 'N', '000000005_2');						
						?>
					</td>
				</tr>	
		
		<tr>
			<td class="SubTituloDireita">Situação:</td>
			<td>
				<input type="radio" name="plisituacao" id="plisituacao" value="" <?if($_REQUEST["plisituacao"] == '') echo 'checked';?>> Qualquer
				<input type="radio" name="plisituacao" id="plisituacao" value="P" <?if($_REQUEST["plisituacao"] == 'P') echo 'checked';?>> Pendente de Aprovação
				<input type="radio" name="plisituacao" id="plisituacao" value="A" <?if($_REQUEST["plisituacao"] == 'A') echo 'checked';?>> Aprovado
				<input type="radio" name="plisituacao" id="plisituacao" value="H" <?if($_REQUEST["plisituacao"] == 'H') echo 'checked';?>> Homologado
				<input type="radio" name="plisituacao" id="plisituacao" value="E" <?if($_REQUEST["plisituacao"] == 'E') echo 'checked';?>> Enviado para Revisão
				<input type="radio" name="plisituacao" id="plisituacao" value="R" <?if($_REQUEST["plisituacao"] == 'R') echo 'checked';?>> Revisado
				<input type="radio" name="plisituacao" id="plisituacao" value="C" <?if($_REQUEST["plisituacao"] == 'C') echo 'checked';?>> Cadastrado no SIAFI
				<input type="radio" name="plisituacao" id="plisituacao" value="S" <?if($_REQUEST["plisituacao"] == 'S') echo 'checked';?>> Confirmado no SIAFI
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
	
		$sql = "select distinct plicod as codigo, 
								ptres::varchar(6) as ptres, 
								CASE
									WHEN trim(plititulo) is not null THEN 
										'<a title=\"Visualizar Dados\" style=\"cursor:pointer;\" onclick=\"mostrapi2(\'' || pi.pliid || '\', \'consulta\');\">' || pi.plititulo  || '</a><input type=\"hidden\" id=\"plititulo[' || pi.plicod || ']\" value=\"' || pi.plicod || ' - ' || pi.plititulo || '\">'
									ELSE 
										'<a title=\"Visualizar Dados\" style=\"cursor:pointer;\" onclick=\"mostrapi2(\'' || pi.pliid || '\',  \'consulta\');\">Não Preenchido</a><input type=\"hidden\" id=\"plititulo[' || pi.plicod || ']\" value=\" ' || pi.plicod ||' - Não Preenchido\"/>' 
								END as titulo,
								acao, 
								ungdsc as unidade, 
								statuspi as situacao, 
								valor,  
								to_char(plidata, 'DD/MM/YYYY') as data 
				from
						(
						select	ug.ungcod,
							ungdsc,
							pi.pliid,
							plicod,
							sbacod,
							sbatitulo,
							mdeid,
							eqdid,
							neeid,
							capid,
							sa.pieid,
							sa.pigid,
							sa.sbaid,
							obrid,
							plititulo,
							null as acaid,
							'' as ptres,
							'' as funcional,
							'' as acao,
							0 as valor,
							plidsc,
							plidata,
							plisituacao,
							Case plisituacao
								when 'P' then 'Pendente de Aprovação'
								when 'H' then 'Homologado'
								when 'A' then 'Aprovado'
								when 'E' then 'Enviado para Revisão'
								when 'R' then 'Revisado'
								when 'C' then 'Cadastrado no SIAFI'
								when 'S' then 'Confirmado no SIAFI'
							end as statusPI	
						from 
							monitora.pi_planointerno pi
							inner join monitora.pi_subacao sa ON sa.sbaid = pi.sbaid
							inner join public.unidadegestora ug ON ug.ungcod = pi.ungcod
						where pi.plistatus = 'A' and pliano = '".$_SESSION['exercicio']."' and pi.ungcod is not null and pliid not in ( select pliid from monitora.pi_planointernoptres )
						union all
						select	ug.ungcod,
							ungdsc,
							pi.pliid,
							plicod,
							sbacod,
							sbatitulo,
							mdeid,
							eqdid,
							neeid,
							capid,
							sa.pieid,
							sa.pigid,
							sa.sbaid,
							obrid,
							plititulo,
							ptres.acaid,
							ptres,
							ptres.funcod||'.'||ptres.sfucod||'.'||ptres.prgcod||'.'||ptres.acacod||'.'||ptres.unicod||'.'||ptres.loccod as funcional,
							acadsc as acao,
							pipvalor as valor,
							plidsc,
							plidata,
							plisituacao,
							Case plisituacao
								when 'P' then 'Pendente de Aprovação'
								when 'H' then 'Homologado'
								when 'A' then 'Aprovado'
								when 'E' then 'Enviado para Revisão'
								when 'R' then 'Revisado'
								when 'C' then 'Cadastrado no SIAFI'
								when 'S' then 'Confirmado no SIAFI'
							end as statusPI	
						from 
							monitora.pi_planointerno pi
							inner join monitora.pi_subacao sa ON sa.sbaid = pi.sbaid
							inner join public.unidadegestora ug ON ug.ungcod = pi.ungcod
							inner join monitora.pi_planointernoptres pip ON pip.pliid = pi.pliid
							inner join monitora.ptres ptres ON ptres.ptrid = pip.ptrid 
							inner join monitora.acao a ON a.acaid = ptres.acaid
						where pi.plistatus = 'A' and pliano = '".$_SESSION['exercicio']."' 
						order by plicod
						) as pi 
						" . ( !empty($filtro) ? "where" . implode(" AND ", $filtro) : '' ) . "
						order by plicod ";
/*
		$sql = "SELECT distinct
					pi.plicod as codigo,
					ptr.ptrid as ptres,
					CASE
						WHEN trim(pi.plititulo) is not null THEN 
							'<a title=\"Visualizar Dados\" style=\"cursor:pointer;\" onclick=\"mostrapi2(\'' || pi.plicod || '\', \'consulta\');\">' || pi.plititulo  || '</a><input type=\"hidden\" id=\"plititulo[' || pi.plicod || ']\" value=\"' || pi.plicod || ' - ' || pi.plititulo || '\">'
						ELSE 
							'<a title=\"Visualizar Dados\" style=\"cursor:pointer;\" onclick=\"mostrapi2(\'' || pi.plicod || '\',  \'consulta\');\">Não Preenchido</a><input type=\"hidden\" id=\"plititulo[' || pi.plicod || ']\" value=\" ' || pi.plicod ||' - Não Preenchido\"/>' 
					END as titulo,
					ac.acadsc as acao,
					uni.unidsc as entnome,
					CASE 
						WHEN pi.plisituacao = 'P' THEN ' Pendente '
		 				WHEN pi.plisituacao = 'S' THEN ' Cadastrado no SIAFI '
		 				WHEN pi.plisituacao = 'C' THEN ' Aprovado '
		 				WHEN pi.plisituacao = 'H' THEN ' Homologado '
		 				WHEN pi.plisituacao = 'R' THEN '  <a title=\"Visualizar Dados da Revisão\" style=\"cursor:pointer;\" onclick=\"abredadosobs(\'' || pi.plicod || '\');\" >Enviado para Revisão</a>  ' 
					END as situacao,
					to_char(pi.plidata, 'DD/MM/YYYY') as data
				FROM
					monitora.pi_planointerno pi
				left JOIN
					monitora.pi_subacao sub ON sub.sbaid = pi.sbaid
				left JOIN
					monitora.pi_subacaounidade su ON su.sbaid = sub.sbaid
				left JOIN
					public.unidade uni ON uni.unicod = su.unicod
				inner JOIN 
					monitora.pi_planointernoptres pt ON pt.pliid = pi.pliid 
				left JOIN 
					monitora.ptres ptr ON ptr.ptrid = pt.ptrid 
				left JOIN 
					monitora.acao ac ON ac.acaid = ptr.acaid
				WHERE
					pi.plistatus = 'A' " . ( !empty($filtro) ? "AND" . implode(" AND ", $filtro) : '' ) . "
				ORDER BY
					ac.acadsc ";	
*/	
		
		$cabecalho = array( "Código" , "Ptres", "Título", "Ação", "Unidade", "Situação", "Valor", "Data de Inclusão" );  
		$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
		//dbg($sql,1);

	?>

	<script>
		/**
		 * Abre os dados do PI
		 */
	 
		function mostrapi2( id, tipoacao ){
				var janela = window.open( '?modulo=principal/planotrabalhoUG/popuphistoricoplanointernoUG&acao=A&pliid=' + id + '&tipoacao=' + tipoacao, 
							 			  'Dados do PI','scrollbars=yes, width=800, height=650 ');
				janela.focus();
			}	
	</script>