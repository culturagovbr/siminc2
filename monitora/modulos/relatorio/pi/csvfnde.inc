<?php

switch($_REQUEST['buscar']) {
	case '1':
		include "csvfnde_resultado.inc";
		exit;
	case '2':
		if($_POST['filano'])
			$_SESSION['rehuf_var']['filano']=$_POST['filano'];
		if($_POST['entid'])
			$_SESSION['rehuf_var']['hospitalentid']=$_POST['entid'];
			
		if($_REQUEST['agrupador'] && !$_REQUEST['gitid']) {
			echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>
				  <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
			
			foreach($_REQUEST['agrupador'] as $tblid) {
				// Pegando dados da TABELA (pai de toda a estrutura)
				$tabela = $db->pegaLinha("SELECT tabtid, tabtdsc, tabanoini, tabanofim FROM rehuf.tabela WHERE tabtid ='".$tblid."'");
				echo "<table class=\"tabela\" align=\"center\" bgcolor=\"#f5f5f5\" cellspacing=\"1\" cellpadding=\"3\">";
				echo "<tr><td class='SubTituloCentro' colspan='2'>".$tabela['tabtdsc']."</td></tr>";
				$grupoitem = (array) $db->carregar("SELECT * FROM rehuf.grupoitem WHERE tabtid = '". $tabela['tabtid'] ."' ORDER BY gitordem");
				foreach($grupoitem as $grp) {
					echo "<tr><td class='SubTituloDireita' width='50%'>".$grp['gitdsc']."</td><td><form action='' method='post' name='formulario'><input type='hidden' name='buscar' value='2'><input type='hidden' name='agrupador[]' value='".$tblid."'><input type='hidden' name='gitid' value='".$grp['gitid']."'><input type='submit' value='Clique aqui'></form></td></tr>";
				}
				echo "</table>";

			}
			exit;			
		}
		include "relatoriogeralexcel_resultado.inc";
		exit;
	case '3':
		include "relatoriogeralagrupado_resultado.inc";
		exit;
		
}

include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "PPA - Monitoramento e Avaliação", "Relatório CSV" );

$_SESSION['rehuf_var']['filano']=array();

?>
<script type="text/javascript">
function exibirRelatorio() {
	// submete formulario
	formulario.target = 'relatorioppamonitora';
	var janela = window.open( '', 'relatorioppamonitora', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
	formulario.submit();
	janela.focus();
}
</script>

<form action="" method="post" name="formulario">
	<input type="hidden" name="buscar" value="1"/>
	
	
	<!-- new -->

	<input type="hidden" name="submetido" id="submetido" value="1"/>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita">Código do PI</td>
			<td>
				<?php 
					$plicod = $_REQUEST['plicod'];
					echo campo_texto( 'plicod', 'N', 'S', '', 20, 15, '', '', 'left', '', 0);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Título</td>
			<td>
				<?php
					$plititulo = $_REQUEST['plititulo'];
					echo campo_texto( 'plititulo', 'N', 'S', '', 65, 60, '', '', 'left', '', 0);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Unidade</td>
			<td>
				<?php
					$unidade = $_REQUEST['unidade'];
					$sql = "SELECT u.unicod as codigo, u.unicod ||' - '|| u.unidsc as descricao FROM public.unidade u WHERE orgcod = '". CODIGO_ORGAO_SISTEMA. "' $stWhere ORDER BY unicod";
					$db->monta_combo('unidade', $sql, 'S',  'Selecione', '', '', '', '', 'N'); 
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Enquadramento da Despesa:</td>
		    <td>
				<?php
					$enquadramento = $_REQUEST['enquadramento'];
					$sql = "SELECT eqdid as codigo, eqdcod ||' - '|| eqddsc as descricao FROM planejamento.enquadramento_despesa WHERE eqdano = '{$_SESSION['exercicio']}' ORDER BY eqdcod";
					$db->monta_combo('enquadramento', $sql, 'S',  'Selecione', '', '', '', '', 'N'); 
				?>    	
		    </td>
		</tr>
		
		<tr>
			<td class='SubTituloDireita'>Nível/Etapa de Ensino:</td>
			<td>
				<?php
					$nivel = $_REQUEST['nivel'];
					$sql = "SELECT neeid as codigo, neecod ||' - '|| needsc as descricao FROM monitora.pi_niveletapaensino WHERE neeano = '{$_SESSION['exercicio']}' ORDER BY neecod ";
					$db->monta_combo('nivel', $sql, 'S', 'Selecione', '', '', '', '240', 'N', '');
				?>
			</td>
		</tr>
		<tr>
			<td class='SubTituloDireita'>Categoria de Apropriação:</td>
			<td>
				<?php
					$apropriacao = $_REQUEST['apropriacao'];
					$sql = "SELECT capid as codigo, capcod ||' - '|| capdsc as descricao FROM planejamento.categoria_apropriacao WHERE capano = '{$_SESSION['exercicio']}' ORDER BY capcod ";
					$db->monta_combo('apropriacao', $sql, 'S', 'Selecione', '', '', '', '340', 'N', '');
				?>
			</td>
		</tr>	
		<tr>
			<td align='right' class="SubTituloDireita">Modalidade de Ensino:</td>
		    <td>
				<?php
					$modalidade = $_REQUEST['modalidade'];
					$sql = "SELECT mdeid as codigo, mdecod ||' - '|| mdedsc as descricao FROM planejamento.area_cultural WHERE mdeano = '{$_SESSION['exercicio']}'";
					$db->monta_combo('modalidade', $sql, 'S', 'Selecione', '', '', '', '240', 'N', '');
		    	 ?>
		    </td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita">Situação</td>
			<td>
				<input type="radio" name="plisituacao" id="situacao_qualquer" value="" <?php if($_REQUEST['plisituacao'] == '') echo 'checked'; ?>> Qualquer
				<input type="radio" name="plisituacao" id="situacao_enviado" value="A"<?php if($_REQUEST['plisituacao'] == 'A') echo 'checked'; ?>> Aprovado
				<input type="radio" name="plisituacao" id="situacao_enviado" value="E" <?php if($_REQUEST['plisituacao'] == 'E') echo 'checked'; ?>> Enviado para Revisão
				<input type="radio" name="plisituacao" id="situacao_pendente" value="P" <?php if($_REQUEST['plisituacao'] == 'P') echo 'checked'; ?>> Pendente
				<input type="radio" name="plisituacao" id="situacao_siafi" value="C" <?php if($_REQUEST['plisituacao'] == 'C') echo 'checked'; ?>> Cadastrado no SIAFI
				<input type="radio" name="plisituacao" id="situacao_revisado" value="R" <?php if($_REQUEST['plisituacao'] == 'R') echo 'checked'; ?>> Revisado
				<input type="radio" name="plisituacao" id="situacao_confirmadosiafi" value="S" <?php if($_REQUEST['plisituacao'] == 'S') echo 'checked'; ?>> Confirmado no SIAFI
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td>
			</td>
			<td>
				<div style="float: left;">
				<!-- <input type="submit" value="Pesquisar" style="cursor: pointer;"/>  -->	
					<input type="button" name="filtrar" value="Visualizar" onclick="exibirRelatorio();"/>
				</div>
			</td>
		</tr>
	</table>
	
	<!-- end new -->
