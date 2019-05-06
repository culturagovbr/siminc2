<?php
if ( $_REQUEST['submetido'] == 1 ){
	$arFiltro = array();
	$join_I = array();
	$join_O = array();
	$join_M = array();
	
	if( $_REQUEST["andsc"] ){
		array_push($arFiltro, " UPPER(anodsc) ilike '%" . strtoupper($_REQUEST['andsc']) . "%' ");
	}
		
	if( $_REQUEST["ansituacao"] ){
		array_push($arFiltro, " anosituacao = '" . $_REQUEST['ansituacao'] . "' ");
	}
	
	if( $_REQUEST["prgid"] ){
		array_push($join_I, " monitora.objetivoiniciativa oi ON oi.ictid = ai.ictid ");
		array_push($join_I, " monitora.objetivo o ON o.objid = oi.objid AND prgid = " . $_REQUEST['prgid']);
		
		array_push($join_O, " monitora.objetivo o ON o.objid = ao.objid AND prgid = " . $_REQUEST['prgid']);
		
		array_push($join_M, " monitora.objetivometa om ON om.metid = am.metid ");
		array_push($join_M, " monitora.objetivo o ON o.objid = om.objid AND prgid = " . $_REQUEST['prgid']);
	}
	
	if( $_REQUEST["acaid"] ){
		array_push($join_I, " monitora.objetivoiniciativa oi1 ON oi1.ictid = ai.ictid ");
		array_push($join_I, " monitora.objetivo o1 ON o1.objid = oi1.objid");
		array_push($join_I, " monitora.acao a1 ON a1.prgid = o1.prgid AND acaid = " . $_REQUEST['acaid']);
		
		array_push($join_O, " monitora.objetivo o1 ON o1.objid = ao.objid");
		array_push($join_O, " monitora.acao a1 ON a1.prgid = o1.prgid AND acaid = " . $_REQUEST['acaid']);
		
		array_push($join_M, " monitora.objetivometa om1 ON om1.metid = am.metid ");
		array_push($join_M, " monitora.objetivo o1 ON o1.objid = om1.objid");
		array_push($join_M, " monitora.acao a1 ON a1.prgid = o1.prgid AND acaid = " . $_REQUEST['acaid']);
	}
	
} 

include APPRAIZ."includes/cabecalho.inc";

echo '<br/>';

$titulo_modulo = "Gerenciar Análises Situacionais";

monta_titulo( $titulo_modulo, $unidade);
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="formulario1" name="formulario1" method="post" action="">
	<input type="hidden" name="submetido" id="submetido" value="1"/>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita">Descrição</td>
			<td>
				<?php
					$andsc = $_REQUEST['andsc'];
					echo campo_texto( 'andsc', 'N', 'S', '', 65, 60, '', '', 'left', '', 0);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Situação</td>
			<td>
				<input type="radio" name="ansituacao" id="situacao_qualquer" value=""  <?php if($_REQUEST['ansituacao'] == '')  echo 'checked'; ?> > Qualquer
				<input type="radio" name="ansituacao" id="situacao_enviado"  value="P" <?php if($_REQUEST['ansituacao'] == 'P') echo 'checked'; ?>> Pendente
				<input type="radio" name="ansituacao" id="situacao_enviado"  value="E" <?php if($_REQUEST['ansituacao'] == 'E') echo 'checked'; ?>> Enviado para SPO
				<input type="radio" name="ansituacao" id="situacao_enviado"  value="H" <?php if($_REQUEST['ansituacao'] == 'H') echo 'checked'; ?>> Homologado pela SPO
				<input type="radio" name="ansituacao" id="situacao_enviado"  value="R" <?php if($_REQUEST['ansituacao'] == 'R') echo 'checked'; ?>> Enviado para Revisão
				<input type="radio" name="ansituacao" id="situacao_enviado"  value="A" <?php if($_REQUEST['ansituacao'] == 'A') echo 'checked'; ?>> Aprovado pela SPO
				<input type="radio" name="ansituacao" id="situacao_enviado"  value="S" <?php if($_REQUEST['ansituacao'] == 'S') echo 'checked'; ?>> Enviado para SIOP
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita">Tipo</td>
			<td>
				<select name="tipo" id="tipo">
					<option value="">Selecione</option>
					<option value="I" <?php echo ($_REQUEST['tipo'] == 'I' ? 'selected="selected"' : ''); ?>>Iniciativa</option>
					<option value="O" <?php echo ($_REQUEST['tipo'] == 'O' ? 'selected="selected"' : ''); ?>>Objetivo</option>
					<option value="M" <?php echo ($_REQUEST['tipo'] == 'M' ? 'selected="selected"' : ''); ?>>Meta</option>
				</select>
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita" width="40%">Programa:</td>
				<?php 
					$sql = "SELECT prgid as codigo, prgdsc as descricao 
							FROM planejamento.programa 
							WHERE prgstatus='A' AND prgano='".$_SESSION['exercicio']."'
							ORDER BY prgdsc";
				?>	
			<td>
				<?php 
				echo $db->monta_combo('prgid', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'prgid', true, $_REQUEST['prgid']);
				?>
			</td>
		</tr>
		
		<!--tr>
			<td class="SubTituloDireita" width="40%">Ação:</td>
				<?php 
					/*$sql = "SELECT acaid as codigo, acacod||' - '||acatitulo as descricao 
							FROM monitora.acao 
							WHERE acastatus='A' AND acadsc <> '' AND prgano='".$_SESSION['exercicio']."'
							ORDER BY acadsc";*/
					//dbg($sql);
				?>	
			<td>
				<?php 
				//echo $db->monta_combo('acaid', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'acaid', true, $_REQUEST['acaid']);
				?>
			</td>
		</tr-->
		
		<tr bgcolor="#C0C0C0">
			<td>
				&nbsp;
			</td>
			<td>
				<div style="float: left;">
					<input type="submit" value="Pesquisar" style="cursor: pointer;"/>
				</div>
			</td>
		</tr>
	</table>
</form>


<?php if ( $_REQUEST['submetido'] == 1 ){ ?>
<form id="formulario" name="formulario" method="post" action="">
	<center>
		<div id="valida" style="overflow:auto; width:95%; border-bottom:2px solid #cccccc;">
			<?php 
			
				function substituiValoresFiltro($arvalores, $valor)
				{
					for($i=0;$i<count($arvalores);$i++) {
						 $arvalores[$i] = str_replace("ano", $valor, $arvalores[$i]) ;
					}
					return $arvalores;
				}
			
				switch (true){
					case $_POST['tipo'] == 'I':
							$arFiltroAux = substituiValoresFiltro($arFiltro, "ani");
							$sql = "SELECT 
							'<a title=\"Visualizar Dados\" style=\"cursor:pointer;\" onclick=\"mostraAnalise(\'' || aniid || '\',\'I\');\">' || anidsc || '</a>', 
							to_char (anidtinclusao, 'dd/mm/yyyy hh:mm:ss') ,
										
							case 
							when OBJSTATUS = 'P'  THEN 'PENDENTE'  
							when OBJSTATUS = 'E' THEN 'ENVIADO PARA SPO' 
							WHEN OBJSTATUS = 'H' THEN 'HOMOLOGADO' 
							when OBJSTATUS = 'R' THEN 'REVISÃO'
							when OBJSTATUS = 'A'  THEN 'APROVADO' 
							WHEN OBJSTATUS = 'S' THEN 'ENVIADO PARA SIOP'
							END, 'INICIATIVA' AS TIPO
															
							FROM MONITORA.ANALISEINICIATIVA ai "
							 .
							(!empty($join_I) ? " JOIN " . implode(" JOIN ", $join_I) : '') .  
							( !empty($arFiltroAux) ? " AND " . implode(" AND ", $arFiltroAux) : '' ).
							" WHERE ai.anistatus = 'A' ";
						break;
					case $_POST['tipo'] == 'O':
							$sql = "SELECT 
							'<a title=\"Visualizar Dados\" style=\"cursor:pointer;\" onclick=\"mostraAnalise(\'' || ao.objid || '\',\'O\');\">' || anodsc || '</a>', 
							to_char (anodtinclusao, 'dd/mm/yyyy hh:mm:ss') , 
							
							case 
							when OBJSTATUS = 'P'  THEN 'PENDENTE'  
							when OBJSTATUS = 'E' THEN 'ENVIADO PARA SPO' 
							WHEN OBJSTATUS = 'H' THEN 'HOMOLOGADO' 
							when OBJSTATUS = 'R' THEN 'REVISÃO'
							when OBJSTATUS =  'A'  THEN 'APROVADO' 
							WHEN OBJSTATUS = 'S' THEN 'ENVIADO PARA SIOP'
							END , 'OBJETIVO' AS TIPO
							
							FROM MONITORA.ANALISEOBJETIVO ao "
							 . 
							(!empty($join_O) ? " JOIN " . implode(" JOIN ", $join_O) : '') .
							( !empty($arFiltro) ? " AND " . implode(" AND ", $arFiltro) : '' ).
							" WHERE ao.anostatus = 'A' ";
						break;
					case $_POST['tipo'] == 'M':
							$arFiltroAux = substituiValoresFiltro($arFiltro, "anm");
							$sql = "SELECT 
							'<a title=\"Visualizar Dados\" style=\"cursor:pointer;\" onclick=\"mostraAnalise(\'' || am.metid || '\',\'M\');\">' || anmdsc || '</a>', 
							to_char (anmdtinclusao, 'dd/mm/yyyy hh:mm:ss') ,
			
							case 
							when OBJSTATUS = 'P'  THEN 'PENDENTE'  
							when OBJSTATUS = 'E' THEN 'ENVIADO PARA SPO' 
							WHEN OBJSTATUS = 'H' THEN 'HOMOLOGADO' 
							when OBJSTATUS = 'R' THEN 'REVISÃO'
							when OBJSTATUS =  'A'  THEN 'APROVADO' 
							WHEN OBJSTATUS = 'S' THEN 'ENVIADO PARA SIOP'
							END, 'META' AS TIPO
							
							FROM MONITORA.ANALISEMETA am" 
							 . 
							(!empty($join_M) ? " JOIN " . implode(" JOIN ", $join_M) : '') .
							( !empty($arFiltroAux) ? " AND " . implode(" AND ", $arFiltroAux) : '' ).
							" WHERE am.anmstatus = 'A' ";
						break;
					default:
						$sql = "SELECT 
						'<a title=\"Visualizar Dados\" style=\"cursor:pointer;\" onclick=\"mostraAnalise(\'' || ao.anoid || '\',\'O\',\'' || ao.objid || '\');\">' || anodsc || '</a>', 
						to_char (anodtinclusao, 'dd/mm/yyyy hh:mm:ss') , 
						
						case 
						when OBJSTATUS = 'P'  THEN 'PENDENTE'  
						when OBJSTATUS = 'E' THEN 'ENVIADO PARA SPO' 
						WHEN OBJSTATUS = 'H' THEN 'HOMOLOGADO' 
						when OBJSTATUS = 'R' THEN 'REVISÃO'
						when OBJSTATUS =  'A'  THEN 'APROVADO' 
						WHEN OBJSTATUS = 'S' THEN 'ENVIADO PARA SIOP'
						END , 'OBJETIVO' AS TIPO
						
						FROM MONITORA.ANALISEOBJETIVO ao"
						 . 
						(!empty($join_O) ? " JOIN " . implode(" JOIN ", $join_O) : '') .
						( !empty($arFiltro) ? " AND " . implode(" AND ", $arFiltro) : '' ).
						" WHERE ao.anostatus = 'A'";
						
						$arFiltroAux = substituiValoresFiltro($arFiltro, "ani");
						$sql .= " union SELECT
						'<a title=\"Visualizar Dados\" style=\"cursor:pointer;\" onclick=\"mostraAnalise(\'' || aniid || '\',\'I\',\'' || ai.ictid || '\');\">' || anidsc || '</a>', 
						to_char (anidtinclusao, 'dd/mm/yyyy hh:mm:ss') ,
									
						case 
						when OBJSTATUS = 'P'  THEN 'PENDENTE'  
						when OBJSTATUS = 'E' THEN 'ENVIADO PARA SPO' 
						WHEN OBJSTATUS = 'H' THEN 'HOMOLOGADO' 
						when OBJSTATUS = 'R' THEN 'REVISÃO'
						when OBJSTATUS =  'A'  THEN 'APROVADO' 
						WHEN OBJSTATUS = 'S' THEN 'ENVIADO PARA SIOP'
						END, 'INICIATIVA' AS TIPO
														
						FROM MONITORA.ANALISEINICIATIVA ai"
						 . 
						(!empty($join_I) ? " JOIN " . implode(" JOIN ", $join_I) : '') .
						( !empty($arFiltroAux) ? " AND " . implode(" AND ", $arFiltroAux) : '' ).
						" WHERE ai.anistatus = 'A' ";
						
						$arFiltroAux = substituiValoresFiltro($arFiltro, "anm");
						$sql .= " union SELECT 
						'<a title=\"Visualizar Dados\" style=\"cursor:pointer;\" onclick=\"mostraAnalise(\'' || am.anmid || '\',\'M\',\'' || am.metid || '\');\">' || anmdsc || '</a>', 
						to_char (anmdtinclusao, 'dd/mm/yyyy hh:mm:ss') ,
		
						case 
						when OBJSTATUS = 'P'  THEN 'PENDENTE'  
						when OBJSTATUS = 'E' THEN 'ENVIADO PARA SPO' 
						WHEN OBJSTATUS = 'H' THEN 'HOMOLOGADO' 
						when OBJSTATUS = 'R' THEN 'REVISÃO'
						when OBJSTATUS =  'A'  THEN 'APROVADO' 
						WHEN OBJSTATUS = 'S' THEN 'ENVIADO PARA SIOP'
						END, 'META' AS TIPO
						
						FROM MONITORA.ANALISEMETA am"
						 . 
						(!empty($join_M) ? " JOIN " . implode(" JOIN ", $join_M) : '') .
						( !empty($arFiltroAux) ? " AND " . implode(" AND ", $arFiltroAux) : '' ).
						" WHERE am.anmstatus = 'A' ";
				}		
				
				$sql.= " order by 1 ";
				
				//dbg($sql,1);
				
				$cabecalho = array("Descrição", "Data Inclusão", "Situação", "Tipo");
				$db->monta_lista_simples( $sql, $cabecalho, 100, 50, 'N', '100%', 'N' );
			?>
		</div>
	</center>
</form>
<?}?>

<script>
	function pagina( numero ){
	 	document.formulario.numero.value = numero;
	 	document.formulario.submit();
 	}

	function mostraAnalise( id, tipo , id2 ){
		switch (true){
			case tipo == 'I':
				window.open('?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseSituacionalIniciativa&aniid=' + id + '&ictid=' + id2 ,'','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=500');
				break;
			case tipo == 'O':
				window.open('?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseSituacionalObjetivo&anoid=' + id + '&objid=' + id2 ,'','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=500');
				break;
			case tipo == 'M':
				window.open('?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseSituacionalMeta&anmid=' + id + '&metid=' + id2 ,'','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=500');
				break;
		}
//		window.open('?modulo=principal/popupAnaliseSituacional&acao=A&anid='+anid+'&tipo='+tipo,'','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=500');
 	}
</script>
<div id="dibDegug"></div>