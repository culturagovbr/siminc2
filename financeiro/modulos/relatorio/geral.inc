<?php

include APPRAIZ. '/includes/Agrupador.php';
include 'ConsultaFinanceiro.inc';

$possuiPerfilConsultaUnidades = false;

$sql = "SELECT 
			pu.pflcod as perfil
		FROM 
			seguranca.perfilusuario pu 
		INNER JOIN 
			seguranca.perfil p ON p.pflcod = pu.pflcod AND p.sisid = ".$_SESSION["sisid"]." 
		WHERE 
			pu.usucpf = '".$_SESSION["usucpf"]."'";

$perfilUsuario = $db->carregar($sql);

// Verifica se o usuário possui somente perfil de "Consulta Unidade"
if((count($perfilUsuario) == 1) && ($perfilUsuario[0]["perfil"] == 175)) {
	$possuiPerfilConsultaUnidades = true;
}

// transforma consulta em pública
if ( $_REQUEST['prtid'] && $_REQUEST['publico'] )
{
	$sql = sprintf(
		"UPDATE public.parametros_tela SET prtpublico = case when prtpublico = true then false else true end WHERE prtid = %d",
		$_REQUEST['prtid']
	);
	$db->executar( $sql );
	$db->commit();
	?>
	<script type="text/javascript">
		location.href = '?modulo=<?= $modulo ?>&acao=R';
	</script>
	<?
}
// FIM transforma consulta em pública

// remove consulta
if ( $_REQUEST['prtid'] && $_REQUEST['excluir'] ) {
	$sql = sprintf(
		"DELETE from public.parametros_tela WHERE prtid = %d",
		$_REQUEST['prtid']
	);
	$db->executar( $sql );
	$db->commit();
	?>
		<script type="text/javascript">
			location.href = '?modulo=<?= $modulo ?>&acao=R';
		</script>
	<?
}
// FIM remove consulta

// remove flag de submissão de formulário
if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] )
{
	unset( $_REQUEST['form'] );
}
// FIM remove flag de submissão de formulário

// exibe consulta
if ( isset( $_REQUEST['form'] ) == true && $_REQUEST['alterar_ano'] == '0' )
{
	if ( $_REQUEST['prtid'] )
	{
		$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid'] );
		$itens = $db->pegaUm( $sql );
		$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
		$_REQUEST = $dados;//array_merge( $_REQUEST, $dados );
		unset( $_REQUEST['salvar'] );
	}
	include 'resultado_geral.inc'; 
	exit();
}

// variáveis que alimentam o formulário
// variáveis dos filtros gerais
$titulo = '';
$ano = '';
$nao_mostra_filtro_impressao = false;
$agrupador = array();
$sumarizador = '';
$publico = '';
$prtid = '';
$excluir = '';
$escala = '1';

// FIM variáveis que alimentam o formulário

// verifica se usuário alterou o ano
$ano = $_SESSION['exercicio'];
if ( $_REQUEST['alterar_ano'] == '1' )
{
	$titulo = $_REQUEST['titulo'];
	$ano = $_REQUEST['ano'];
	if ( $_REQUEST['agrupador'] )
	{
		foreach ( $_REQUEST['agrupador'] as $valorAgrupador )
		{
			array_push( $agrupador, array( 'codigo' => $valorAgrupador, 'descricao' => ConsultaFinanceiroTraducao::pegarTitulo( $valorAgrupador ) ) );
		}
	}
	$sumarizador = $_REQUEST['sumarizador'];
}


// carrega consulta do banco
if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] )
{
	$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = ".$_REQUEST['prtid'] );
	$itens = $db->pegaUm( $sql );
	$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
	extract( $dados );
	$_REQUEST = $dados;//array_merge( $_REQUEST, $dados );
	unset( $_REQUEST['form'] );
	$titulo = $_REQUEST['titulo'];
	$ano = $_REQUEST['ano'];
	//$agrupador = $_REQUEST['agrupador'];
	$agrupador = array();
	if ( $_REQUEST['agrupador'] )
	{
		foreach ( $_REQUEST['agrupador'] as $valorAgrupador )
		{
			array_push( $agrupador, array( 'codigo' => $valorAgrupador, 'descricao' => ConsultaFinanceiroTraducao::pegarTitulo( $valorAgrupador ) ) );
		}
	}
	$sumarizador = $_REQUEST['sumarizador'];
	$escala = $_REQUEST['escala'];
	$nao_mostra_filtro_impressao = $_REQUEST['nao_mostra_filtro_impressao'];
}
// FIM carrega consulta do banco

// variáveis que alimentam o formulário
// variáveis dos combos
// variáveis devem ser alimentadas pelos querys antecedentes aos combos
$orgao = array();
$grupouo = array();
$uo = array();
$ug = array();
$funcao = array();
$subfuncao = array();
$programa = array();
$acacod = array();
$localizador = array();
$fonte = array();
$gnd = array();
$natureza = array();
$usopri = array();
$grf = array();
$iducod = array();
$mapcod = array();
$fontesiafi = array();
$catecon = array();
$elemento = array();
$esfera = array();
$fontesof = array();

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Relatório Módulo Financeiro', 'Relatório Geral' );

?>
<script type="text/javascript">

	/**
	 * Alterar visibilidade de um bloco.
	 * 
	 * @param string indica o bloco a ser mostrado/escondido
	 * @return void
	 */
	function onOffBloco( bloco )
	{
		var div_on = document.getElementById( bloco + '_div_filtros_on' );
		var div_off = document.getElementById( bloco + '_div_filtros_off' );
		var img = document.getElementById( bloco + '_img' );
		var input = document.getElementById( bloco + '_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '0';
			img.src = '/imagens/menos.gif';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '1';
			img.src = '/imagens/mais.gif';
		}
	}
	
	/**
	 * Alterar visibilidade de um campo.
	 * 
	 * @param string indica o campo a ser mostrado/escondido
	 * @return void
	 */
	function onOffCampo( campo )
	{
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}
	
	/**
	 * Realiza submissão de formulário. Caso o exercício (ano) tenha sido
	 * alterado a submissão é realizada para a própria página, caso contrário
	 * para uma nova janela.
	 * 
	 * @return void
	 */
	function submeterFormulario( tipo )
	{
		var formulario = document.formulario;
		var nomerel = '';
		var qtd = 0;
		<? $qtdrel = 0; ?>
		prepara_formulario();
		selectAllOptions( formulario.agrupador );
		
		// verifica se foi escolhido algum agrupador
		if ( formulario.alterar_ano.value == '0' && tipo != 'relatorio' )
		{
			if ( formulario.agrupador.options.length == 0 )
			{
				alert( 'Escolha pelo menos um agrupador.' );
				return;
			}
		}
		
		if ( formulario.alterar_ano.value == '0' && tipo == 'relatorio' )
		{
			formulario.action = 'financeiro.php?modulo=relatorio/geral&acao=R';
			window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';
		}
		else
		{
			
			
			
			if ( tipo == 'planilha' )
			{
				formulario.action = 'financeiro.php?modulo=relatorio/geral&acao=R&tipoRelatorio=xls';
			}
			else if ( tipo == 'salvar' ) 
			{
			
			
				if ( document.formulario.titulo.value == '' ) 
				{
					alert( 'É necessário informar a descrição do relatório!' );
					document.formulario.titulo.focus();
					return;
				}
				var nomesExistentes = new Array();
				<?php
					$sqlNomesConsulta = "select prtdsc from public.parametros_tela";
					$nomesExistentes = $db->carregar( $sqlNomesConsulta );
					if ( $nomesExistentes )
					{
						foreach ( $nomesExistentes as $linhaNome )
						{
							print "nomesExistentes[nomesExistentes.length] = '" . str_replace( "'", "\'", $linhaNome['prtdsc'] ) . "';";
						}
					}
				?>
				var confirma = true;
				var i, j = nomesExistentes.length;
				for ( i = 0; i < j; i++ )
				{
					if ( nomesExistentes[i] == document.formulario.titulo.value )
					{
						confirma = confirm( 'Deseja alterar a consulta já existente?' );
						break;
					}
				}
				if ( !confirma )
				{
					return;
				}
				formulario.action = 'financeiro.php?modulo=relatorio/geral&acao=R&salvar=1';
			}
			else
			{
				formulario.action = '';
			}
			formulario.target = '_top';
		}
		formulario.submit();
	}
	
	function alterarAno()
	{
		var formulario = document.formulario;
		formulario.alterar_ano.value = '1';
		submeterFormulario('relatorio');
	}
	
</script>
<form action="" method="post" name="formulario">
	
	<input type="hidden" name="form" value="1"/> <!-- indica envio de formulário -->
	<input type="hidden" name="alterar_ano" value="0"/> <!-- indica se há mudança de ano no formulário -->
	<input type="hidden" name="publico" value="<?= $publico ?>"/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" value="<?= $prtid ?>"/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	<input type="hidden" name="excluir" value="<?= $excluir ?>"/> <!-- indica se foi clicado para excluir o relatório já gravado -->

	<!-- ----------------------------------------------------------------------- -->
	
	<!-- INSTITUCIONAL -->
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
			<tr>
				<td class="SubTituloDireita" valign="top">Título</td>
				<td>
					<?= campo_texto( 'titulo', 'N', 'S', '', 78, 100, '', '' ); ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top">Exercício</td>
				<td>
					<?
						//$sqlAno = 'select rofano as codigo, rofano as descricao from financeiro.execucao group by rofano order by rofano desc';
						//$db->monta_combo( 'ano', $sqlAno, 'S', '', 'alterarAno', '' );
					?>
					<select name="ano">
						<option value="2010" selected="selected">2010</option>
						<option value="2009">2009</option>
						<option value="2008">2008</option>
						<option value="2007">2007</option>
						<option value="2006">2006</option>
						<option value="2005">2005</option>
					</select>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top">Escala</td>
				<td>
					<select name="escala" class="CampoEstilo">
						<option value="1" <?= $escala == '1' ? 'selected="selected"' : '' ; ?>>R$ 1 (Reais)</option>
						<option value="1000" <?= $escala == '1000' ? 'selected="selected"' : '' ; ?>>R$ 1.000 (Milhares de Reais)</option>
					</select>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top">Filtro</td>
				<td>
					<input type="checkbox" name="nao_mostra_filtro_impressao" id="nao_mostra_filtro_impressao" value="1" <?= $nao_mostra_filtro_impressao ? 'checked="checked"' : '' ; ?>/>
					<label for="nao_mostra_filtro_impressao">Não mostrar filtros</label>
				</td>
			</tr>
			<tr>
				<td width="195" class="SubTituloDireita" valign="top">Agrupadores</td>
				<td>
					<?php
						$matriz = ConsultaFinanceiroTraducao::pegarAgrupadores();
						$campoAgrupador = new Agrupador( 'formulario' );
						$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
						$campoAgrupador->setDestino( 'agrupador', 4 );
						$campoAgrupador->exibir();
					?>
				</td>
			</tr>
	</table>
	
	<!-- OUTROS FILTROS -->
	
	<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
		<tr>
			<td onclick="javascript:onOffBloco( 'outros' );">
				<img border="0" src="/imagens/mais.gif" id="outros_img"/>&nbsp;
				Relatórios Gerenciais
				<input type="hidden" id="outros_flag" name="outros_flag" value="0" />
			</td>
		</tr>
	</table>
	<div id="outros_div_filtros_off">
		<!--
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td><span style="color:#a0a0a0;padding:0 30px;">nenhum filtro</span></td>
			</tr>
		</table>
		-->
	</div>

	<div id="outros_div_filtros_on" style="display:none;">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
				<tr>
					<td width="195" class="SubTituloDireita" valign="top">Relatórios:</td>
					<?php
						$sql = sprintf(
							"SELECT Case when prtpublico = true and usucpf = '%s' then '<img border=\"0\" src=\"../imagens/usuario.gif\" title=\" Despublicar \" onclick=\"tornar_publico(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' else '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' end as acao, '<a href=\"javascript: carregar_consulta(' || prtid || ');\">' || prtdsc || '</a>' as descricao FROM public.parametros_tela WHERE mnuid = %d AND prtpublico = TRUE",
							$_SESSION['usucpf'],
							$_SESSION['mnuid'],
							$_SESSION['usucpf']
						);
						/*
						$sql = sprintf(
							"SELECT 'abc' as acao, prtdsc FROM public.parametros_tela WHERE mnuid = %d AND prtpublico = TRUE",
							$_SESSION['mnuid']
						);
*/
						$cabecalho = array(
							'Ação',
							'Nome'
						);
					?>
					<td><?php $db->monta_lista_simples( $sql, null, 50, 50, null, null, null ); ?>
					</td>
				</tr>
		</table>
	</div>
						<script language="javascript">	//alert( document.formulario.agrupador_combo.value );	</script>

	<!-- FIM OUTROS FILTROS -->
	
	<!-- MINHAS CONSULTAS -->
	
	<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
		<tr>
			<td onclick="javascript:onOffBloco( 'minhasconsultas' );">
				<img border="0" src="/imagens/mais.gif" id="minhasconsultas_img"/>&nbsp;
				Minhas Consultas
				<input type="hidden" id="minhasconsultas_flag" name="minhasconsultas_flag" value="0" />
			</td>
		</tr>
	</table>
	<div id="minhasconsultas_div_filtros_off">
		<!--
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td><span style="color:#a0a0a0;padding:0 30px;">nenhum filtro</span></td>
			</tr>
		</table>
		-->
	</div>
	<div id="minhasconsultas_div_filtros_on" style="display:none;">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
				<tr>
					<td width="195" class="SubTituloDireita" valign="top">Consultas:</td>
					<?php
						$sql = sprintf(
							"SELECT Case when prtpublico = false then '<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' else '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' end as acao, '<a href=\"javascript: carregar_consulta(' || prtid || ')\">' || prtdsc || '</a>' as descricao FROM public.parametros_tela WHERE mnuid = %d AND usucpf = '%s'",
							$_SESSION['mnuid'],
							$_SESSION['usucpf']
						);
						$cabecalho = array(
							'Ação',
							'Nome'
						);
					?>
					<td><?php $db->monta_lista_simples( $sql, null, 50, 50, null, null, null ); ?></td>
				</tr>
		</table>
	</div>
	
	<!-- FIM MINHAS CONSULTAS -->
	
	<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
		<tr>
			<td onclick="javascript:onOffBloco( 'inst' );">
				<img border="0" src="/imagens/mais.gif" id="inst_img"/>&nbsp;
				Institucional
				<input type="hidden" id="inst_flag" name="inst_flag" value="0" />
			</td>
		</tr>
	</table>
	<div id="inst_div_filtros_off">
		<!--
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td><span style="color:#a0a0a0;padding:0 30px;">nenhum filtro</span></td>
			</tr>
		</table>
		-->
	</div>
	<div id="inst_div_filtros_on" style="display:none;">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<!--
			<tr>
				<td class="SubTituloDireita" style="text-align:center;">A</td>
				<td class="SubTituloDireita" style="text-align:center;">S</td>
				<td colspan="2">&nbsp;</td>
			</tr>
			-->
			<!--
			<tr>
				<td width="195" class="SubTituloDireita" valign="top">
					Órgão
					<input type="hidden" id="orgao_campo_flag" name="orgao_campo_flag" value="1"/>
				</td>
				<td>
					<div id="orgao_campo_off" style="color:#a0a0a0;">Ministério da Educação</div>
					<div id="orgao_campo_on" style="display:none;">
						<input type="hidden" name="orgao[]" value="<?php echo CODIGO_ORGAO_SISTEMA; ?>"/>
						<? $sql_combo = "select o.orgcod as codigo, o.orgcod || ' - ' || o.orgdsc as descricao from financeiro.execucao r inner join public.orgao o on r.orgcod = o.orgcod where r.rofano = '" . $ano . "' group by o.orgcod, o.orgdsc order by o.orgcod, o.orgdsc"; 
						if ( $_REQUEST['orgao'] && $_REQUEST['orgao'][0] != '' )
						{
							$sql_carregados = "select o.orgcod as codigo, o.orgcod || ' - ' || o.orgdsc as descricao from financeiro.execucao r inner join public.orgao o on r.orgcod = o.orgcod where r.rofano = '" . $ano . "' and o.orgcod in ('".implode("','",$_REQUEST['orgao'])."') group by o.orgcod, o.orgdsc ";
							$orgao=$db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'orgao', $sql_combo, 'Selecione o(s) Orgão(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $orgao )  { ?>	<script type="text/javascript"> onOffCampo( 'orgao' ); </script> <? } ?>
			-->
			<tr>
				<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'grupouo' );">
					Grupo de UO
					<input type="hidden" id="grupouo_campo_flag" name="grupouo_campo_flag" value="0"/>
				</td>
				<td>
					<div id="grupouo_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'grupouo' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="grupouo_campo_on" style="display:none;">
						<? $sql_combo = "select u.gunid as codigo, u.gunid || ' - ' || g.gundsc as descricao from financeiro.execucao r inner join public.unidade u on r.unicod = u.unicod inner join public.grupounidade g using ( gunid ) where r.rofano = '" . $ano . "' group by u.gunid, g.gundsc order by u.gunid, g.gundsc"; 
						if ( $_REQUEST['grupouo'] && $_REQUEST['grupouo'][0] != '' )
						{
							$sql_carregados = "select u.gunid as codigo, u.gunid || ' - ' || g.gundsc as descricao from financeiro.execucao r inner join public.unidade u on r.unicod = u.unicod inner join public.grupounidade g using ( gunid ) where r.rofano = '" . $ano . "' and u.gunid in ('".implode("','",$_REQUEST['grupouo'])."') group by u.gunid, g.gundsc";
							$grupouo = $db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'grupouo', $sql_combo, 'Selecione o(s) Grupo(s) de Unidade(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $grupouo )  { ?>	<script type="text/javascript"> onOffCampo( 'grupouo' ); </script> <? } ?>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'uo' );">
					Unidades Orçamentárias
					<input type="hidden" id="uo_campo_flag" name="uo_campo_flag" value="0"/>
				</td>
				<td>
					<div id="uo_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'uo' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="uo_campo_on" style="display:none;">
						<?
						if ( $_REQUEST['uo'] && $_REQUEST['uo'][0] != '' )
						{
							$sql_carregados = "select u.unicod as codigo, u.unicod || ' - ' || u.unidsc as descricao from financeiro.execucao r inner join public.unidade u on r.unicod = u.unicod where r.rofano = '" . $ano . "' and u.unicod in ('".implode("','",$_REQUEST['uo'])."') group by u.unicod, u.unidsc ";
							$uo=$db->carregar( $sql_carregados );
						}
						
					 	if($possuiPerfilConsultaUnidades == true) {
							$qry = "SELECT
										u.unicod as codigo										
									FROM
										financeiro.usuarioresponsabilidade ur
									INNER JOIN
										public.unidade u ON u.unicod = ur.unicod
									WHERE
										ur.usucpf = '".$_SESSION["usucpf"]."' AND
										ur.pflcod = ".$perfilUsuario[0]["perfil"]." AND
										ur.rpustatus = 'A'";
							
							$varCodUnids = $db->carregar($qry);
							
							$unids = "";
							for($i=0; $i<count($varCodUnids); $i++) {
								if($i==0)
									$unids .= "'".$varCodUnids[$i]['codigo']."'";
								else
									$unids .= ",'".$varCodUnids[$i]['codigo']."'";
							}
							
							$sql_combo = "SELECT unicod as codigo, unicod || ' - ' || unidsc as descricao FROM public.unidade WHERE unicod in (".$unids.")";							
					 	}
					 	else {
					 		$sql_combo = "select u.unicod as codigo, u.unicod || ' - ' || u.unidsc as descricao from financeiro.execucao r inner join public.unidade u on r.unicod = u.unicod where r.rofano = '" . $ano . "' group by u.unicod, u.unidsc order by u.unicod, u.unidsc";							
					 	}
					 	
					 	combo_popup( 'uo', $sql_combo, 'Selecione a(s) Unidade(s)', '400x400', 0, array(), '', 'S', true, true );
						?>
					</div>
				</td>
			</tr>
			<? if ( $uo )  { ?>	<script type="text/javascript"> onOffCampo( 'uo' ); </script> <? } ?>
			<!--
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'ug' );">
					Unidades Gestora
					<input type="hidden" id="ug_campo_flag" name="ug_campo_flag" value="0"/>
				</td>
				<td>
					<div id="ug_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'ug' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="ug_campo_on" style="display:none;">
						<? $sql_combo = "select ug.ungcod as codigo, ug.ungcod || ' - ' || ug.ungdsc as descricao from financeiro.reporcfin r inner join public.unidadegestora ug on r.unicod = ug.unicod where r.rofano = '" . $ano . "' group by ug.ungcod, ug.ungdsc order by ug.ungcod, ug.ungdsc"; 
						if ( $_REQUEST['ug'] && $_REQUEST['ug'][0] != '' )
						{
							$sql_carregados = "select ug.ungcod as codigo, ug.ungcod || ' - ' || ug.ungdsc as descricao from financeiro.execucao r inner join public.unidadegestora ug on r.unicod = ug.unicod where r.rofano = '" . $ano . "' and ug.ungcod in ('".implode("','",$_REQUEST['ug'])."') group by ug.ungcod, ug.ungdsc ";
							$ug=$db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'ug', $sql_combo, 'Selecione a(s) Unidade(s) Gestora(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $ug )  { ?>	<script type="text/javascript"> onOffCampo( 'ug' ); </script> <? } ?>
			-->
		</table>
	</div>
	
	<!-- FIM INSTITUCIONAL -->
	
	<!-- ----------------------------------------------------------------------- -->
	
	<!-- FUNCIONAL -->
	
	<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
		<tr>
			<td onclick="javascript:onOffBloco( 'func' );">
				<img border="0" src="/imagens/mais.gif" id="func_img"/>&nbsp;
				Funcional
				<input type="hidden" id="func_flag" name="inst_flag" value="0" />
			</td>
		</tr>
	</table>
	<div id="func_div_filtros_off">
		<!--
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td><span style="color:#a0a0a0;padding:0 30px;">nenhum filtro</span></td>
			</tr>
		</table>
		-->
	</div>
	<div id="func_div_filtros_on" style="display:none;">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<!--
			<tr>
				<td class="SubTituloDireita" style="text-align:center;">A</td>
				<td class="SubTituloDireita" style="text-align:center;">S</td>
				<td colspan="2">&nbsp;</td>
			</tr>
			-->
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'esfera' );">
					Esfera
					<input type="hidden" id="esfera_campo_flag" name="esfera_campo_flag" value="0"/>
				</td>
				<td>
					<div id="esfera_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'esfera' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="esfera_campo_on" style="display:none;">
						<? $sql_combo = "select esf.esfcod as codigo, esf.esfcod || ' - ' || esf.esfdsc as descricao from financeiro.execucao r inner join public.esfera esf on esf.esfcod = r.esfcod where r.rofano = '" . $ano . "' group by esf.esfcod, esf.esfdsc order by esf.esfcod, esf.esfdsc"; 
						if ( $_REQUEST['esfera'] && $_REQUEST['esfera'][0] != '' )
						{
							$sql_carregados = "select esf.esfcod as codigo, esf.esfcod || ' - ' || esf.esfdsc as descricao from financeiro.execucao r inner join public.esfera esf on esf.esfcod = r.esfcod where r.rofano = '" . $ano . "' and esf.esfcod in ('".implode("','",$_REQUEST['esfera'])."') group by esf.esfcod, esf.esfdsc order by esf.esfcod, esf.esfdsc";
							$esfera = $db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'esfera', $sql_combo, 'Selecione a(s) Esfera(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $esfera )  { ?>	<script type="text/javascript"> onOffCampo( 'esfera' ); </script> <? } ?>
			<tr>
				<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'funcao' );">
					Função
					<input type="hidden" id="funcao_campo_flag" name="funcao_campo_flag" value="0"/>
				</td>
				<td>
					<div id="funcao_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'funcao' );" ><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="funcao_campo_on" style="display:none;">
						<? $sql_combo = "select 
											f.funcod as codigo, 
											f.funcod || ' - ' || f.fundsc as descricao  
										from 
											financeiro.execucao r inner join public.ppafuncao f on r.funcod = f.funcod  
										where r.rofano = '" . $ano . "' 
										group by 
											codigo, 
											descricao 
										order by 
											codigo, 
											descricao";
						 
						if ( $_REQUEST['funcao'] && $_REQUEST['funcao'][0] != '' )
						{
							$sql_carregados = "select f.it_co_funcao as codigo, f.it_co_funcao || ' - ' || f.it_no_funcao as descricao from financeiro.execucao r inner join siafi.funcao f on r.funcod = f.it_co_funcao where r.rofano = '" . $ano . "' and f.it_co_funcao in ('".implode("','",$_REQUEST['funcao'])."') group by f.it_co_funcao, f.it_no_funcao";
							$funcao=$db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'funcao', $sql_combo, 'Selecione a(s) Função(ões)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $funcao )  { ?>	<script type="text/javascript"> onOffCampo( 'funcao' ); </script> <? } ?>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'subfuncao' );">
					Sub-Função
					<input type="hidden" id="subfuncao_campo_flag" name="subfuncao_campo_flag" value="0"/>
				</td>
				<td>
					<div id="subfuncao_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'subfuncao' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="subfuncao_campo_on" style="display:none;">
						<? $sql_combo = "select sf.sfucod as codigo, sf.sfucod || ' - ' || sf.sfudsc as descricao from financeiro.execucao r inner join ppasubfuncao sf on r.sfucod = sf.sfucod where r.rofano = '" . $ano . "' group by sf.sfucod, sf.sfudsc order by sf.sfucod, sf.sfudsc"; 
						if ( $_REQUEST['subfuncao'] && $_REQUEST['subfuncao'][0] != '' )
						{
							$sql_carregados = "select sf.sfucod as codigo, sf.sfucod || ' - ' || sf.sfudsc as descricao from financeiro.execucao r inner join ppasubfuncao sf on r.sfucod = sf.sfucod where r.rofano = '" . $ano . "' and sf.sfucod in ('".implode("','",$_REQUEST['subfuncao'])."') group by sf.sfucod, sf.sfudsc ";
							$subfuncao=$db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'subfuncao', $sql_combo, 'Selecione a(s) Sub-Função(ões)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $subfuncao )  { ?>	<script type="text/javascript"> onOffCampo( 'subfuncao' ); </script> <? } ?>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'programa' );">
					Programa
					<input type="hidden" id="programa_campo_flag" name="programa_campo_flag" value="0"/>
				</td>
				<td>
					<div id="programa_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'programa' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="programa_campo_on" style="display:none;">
						<? $sql_combo = "select p.prgcod as codigo, p.prgcod || ' - ' || p.prgdsc as descricao from financeiro.execucao r inner join planejamento.programa p on r.prgcod = p.prgcod where r.rofano = '" . $ano . "' group by p.prgcod, p.prgdsc order by p.prgcod, p.prgdsc";
						if ( $_REQUEST['programa'] && $_REQUEST['programa'][0] != '' )	
						{
							$sql_carregados = "select p.prgcod as codigo, p.prgcod || ' - ' || p.prgdsc as descricao from financeiro.execucao r inner join monitora.programa p on r.prgcod = p.prgcod where r.rofano = '" . $ano . "' and p.prgcod in ('".implode("','",$_REQUEST['programa'])."') group by p.prgcod, p.prgdsc ";
							$programa=$db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'programa', $sql_combo, 'Selecione o(s) Programa(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $programa )  { ?>	<script type="text/javascript"> onOffCampo( 'programa' ); </script> <? } ?>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'acacod' );">
					Ação
					<input type="hidden" id="acacod_campo_flag" name="acacod_campo_flag" value="0"/>
				</td>
				<td>
					<div id="acacod_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'acacod' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="acacod_campo_on" style="display:none;">
						<? $sql_combo = "select acao.acacod as codigo, acao.acacod || ' - ' || acao.acadsc as descricao from financeiro.execucao r inner join ( select distinct acacod, acadsc, prgano from monitora.acao where prgano = '".$ano."' and acasnrap = 'f' group by acacod, acadsc, prgano ) acao on acao.acacod = r.acacod and acao.prgano = r.rofano where r.rofano = '" . $ano . "' group by acao.acacod, acao.acadsc order by acao.acacod, acao.acadsc"; 
						if ( $_REQUEST['acacod'] && $_REQUEST['acacod'][0] != '' )
						{
							$sql_carregados = "select acao.acacod as codigo, acao.acacod || ' - ' || acao.acadsc as descricao from financeiro.execucao r inner join ( select distinct acacod, acadsc, prgano from monitora.acao where prgano = '$ano' and acasnrap = 'f' group by acacod, acadsc, prgano ) acao on acao.acacod = r.acacod and acao.prgano = r.rofano where r.rofano = '" . $ano . "' and acao.acacod in ('".implode("','",$_REQUEST['acacod'])."') group by acao.acacod, acao.acadsc ";
							$acacod = $db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'acacod', $sql_combo, 'Selecione a(s) Ação(ões)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $acacod )  { ?>	<script type="text/javascript"> onOffCampo( 'acacod' ); </script> <? } ?>
			<!--
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'localizador' );">
					Localizador
					<input type="hidden" id="localizador_campo_flag" name="localizador_campo_flag" value="0"/>
				</td>
				<td>
					<div id="localizador_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'localizador' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="localizador_campo_on" style="display:none;">
						<? $sql_combo = "select a.prgcod||'.'||a.acacod||'.'||a.unicod||'.'||a.loccod as codigo, a.prgcod||'.'||a.acacod||'.'||a.unicod||'.'||a.loccod||' - '||a.acadsc||' '||a.sacdsc as descricao from monitora.acao a where a.prgano = '" . $ano . "' order by a.prgcod||'.'||a.acacod||'.'||a.unicod||'.'||a.loccod"; 
						if ( $_REQUEST['localizador'] )
						{
							$sql_carregados = "select a.prgcod||'.'||a.acacod||'.'||a.unicod||'.'||a.loccod as codigo, a.acadsc||' '||a.sacdsc as descricao from monitora.acao a where a.prgano = '" . $ano . "' and 1 in ('".implode("','",$_REQUEST['localizador'])."') order by a.prgcod||'.'||a.acacod||'.'||a.unicod||'.'||a.loccod";
							$localizador=$db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'localizador', $sql_combo, 'Selecione o(s) Localizador(es)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $localizador )  { ?>	<script type="text/javascript"> onOffCampo( 'localizador' ); </script> <? } ?>
			-->
		</table>
	</div>
	
	<!-- FIM FUNCIONAL -->
	
	<!-- ----------------------------------------------------------------------- -->
	
	<!-- CLASSIFICACAO ECONÔMICA -->
	
	<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
		<tr>
			<td onclick="javascript:onOffBloco( 'econ' );">
				<img border="0" src="/imagens/mais.gif" id="econ_img"/>&nbsp;
				Classificação Econômica
				<input type="hidden" id="econ_flag" name="econ_flag" value="0" />
			</td>
		</tr>
	</table>
	<div id="econ_div_filtros_off">
		<!--
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td><span style="color:#a0a0a0;padding:0 30px;">nenhum filtro</span></td>
			</tr>
		</table>
		-->
	</div>
	<div id="econ_div_filtros_on" style="display:none;">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<!--
			<tr>
				<td class="SubTituloDireita" style="text-align:center;">A</td>
				<td class="SubTituloDireita" style="text-align:center;">S</td>
				<td colspan="2">&nbsp;</td>
			</tr>
			-->
			<tr>
				<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'grf' );">
					Grupo Fonte
					<input type="hidden" id="grf_campo_flag" name="grf_campo_flag" value="0"/>
				</td>
				<td>
					<div id="grf_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'grf' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="grf_campo_on" style="display:none;">
						<? $sql_combo = "select gf.grfid as codigo, gf.grfid || ' - ' || gf.grfdsc as descricao from financeiro.execucao f inner join public.grupofonte gf ON gf.grfid = f.grfid group by gf.grfid, gf.grfdsc order by gf.grfid, gf.grfdsc";
						if ( $_REQUEST['grf'] && $_REQUEST['grf'][0] != '' )
						{
							$sql_carregados = "select gf.grfid as codigo, gf.grfid || ' - ' || gf.grfdsc as descricao from financeiro.execucao f inner join public.grupofonte gf ON gf.grfid = to_number(subString(f.foncod,1,1), '99') where gf.grfid in (".implode(",",$_REQUEST['grf']).") group by gf.grfid, gf.grfdsc order by gf.grfid, gf.grfdsc"; 
							$fonte=$db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'grf', $sql_combo, 'Selecione o(s) Grupo(s) de Fonte', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $grf )  { ?>	<script type="text/javascript"> onOffCampo( 'grf' ); </script> <? } ?>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'iducod' );">
					Identificador de Uso
					<input type="hidden" id="iducod_campo_flag" name="iducod_campo_flag" value="0"/>
				</td>
				<td>
					<div id="iducod_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'iducod' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="iducod_campo_on" style="display:none;">
						<? $sql_combo = "select iu.iducod as codigo, iu.iducod || ' - ' || iu.idudsc as descricao from financeiro.execucao r inner join identifuso iu ON iu.iducod = r.iducod where r.rofano = '" . $ano . "' group by iu.iducod, iu.idudsc order by iu.iducod, iu.idudsc "; 
						if ( $_REQUEST['iducod'] && $_REQUEST['iducod'][0] != '' )
						{
							$sql_carregados = "select iu.iducod as codigo, iu.iducod || ' - ' || iu.idudsc as descricao from financeiro.execucao r inner join identifuso iu ON iu.iducod = r.iducod where r.rofano = '" . $ano . "' and iu.iducod in ('".implode("','",$_REQUEST['iducod'])."') group by iu.iducod, iu.idudsc ";
							$iducod=$db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'iducod', $sql_combo, 'Selecione o(s) Identificador(es) de Uso', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $iducod )  { ?>	<script type="text/javascript"> onOffCampo( 'iducod' ); </script> <? } ?>
			<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'fontesiafi' );">
				Fonte Detalhada
				<input type="hidden" id="fontesiafi_campo_flag" name="fontesiafi_campo_flag" value="0"/>
			</td>
			<td>
				<div id="fontesiafi_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'fontesiafi' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="fontesiafi_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT
										f.foscod AS codigo,
										f.foscod || ' - ' || f.fosdsc AS descricao 
									FROM
										financeiro.execucao r 
									INNER JOIN financeiro.fontesiafi f ON f.foscod = r.foncodsiafi 
									--WHERE
									--	r.rofano = '" . $ano . "' 
									GROUP BY 
										f.foscod, 
										f.fosdsc 
									ORDER BY
										f.foscod, 
										f.fosdsc"; 
					
						if ( $_REQUEST['fontesiafi'] && $_REQUEST['fontesiafi'][0] != '' )
						{
							/*$sql_carregados = "SELECT
												foscod AS codigo,
												foscod || ' - ' || fosdsc AS descricao
											   FROM
												financeiro.fontesiafi
											   WHERE
											   	foscod in ('".implode("','", $_REQUEST['fontesiafi'])."')";*/
							$sql_carregados = "SELECT
													f.foscod AS codigo,
													f.foscod || ' - ' || f.fosdsc AS descricao 
												FROM
													financeiro.execucao r 
												INNER JOIN financeiro.fontesiafi f ON f.foscod = r.foncodsiafi 
												WHERE
													f.foscod in ('".implode("','", $_REQUEST['fontesiafi'])."')' 
												GROUP BY 
													f.foscod, 
													f.fosdsc 
												ORDER BY
													f.foscod, 
													f.fosdsc";
							
							$fontesiafi = $db->carregar( $sql_carregados );
						}
						
						combo_popup( 'fontesiafi', $sql_combo, 'Selecione a(s) Fonte(s)', '400x400', 0, array(), '', 'S', true, true);
					?>
				</div>
			</td>
		</tr>
		<tr>
			
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'fontesof' );">
				Fonte SOF
				<input type="hidden" id="fontesof_campo_flag" name="fontesof_campo_flag" value="0"/>
			</td>
			<td>
				<div id="fontesof_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'fontesof' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="fontesof_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT
										f.foncod AS codigo,
										f.foncod || ' - ' || f.fondsc AS descricao 
									FROM
										financeiro.execucao r 
									
									INNER JOIN financeiro.fontesof f on r.foncod = f.foncod  
									--WHERE
									--	r.rofano = '" . $ano . "' 
									GROUP BY 
										f.foncod, 
										f.fondsc 
									ORDER BY
										f.foncod, 
										f.fondsc"; 
					
						if ( $_REQUEST['fontesof'] && $_REQUEST['fontesof'][0] != '' )
						{
							/*$sql_carregados = "SELECT
												foncod AS codigo,
												foncod || ' - ' || fondsc AS descricao
											   FROM
												financeiro.fontesof
											   WHERE
											   	foncod in ('".implode("','", $_REQUEST['fontesof'])."')";*/
							
							$sql_carregados = "SELECT
													f.foncod AS codigo,
													f.foncod || ' - ' || f.fondsc AS descricao 
												FROM
													financeiro.execucao r 
												
												INNER JOIN financeiro.fontesof f on r.foncod = f.foncod  
												WHERE
													f.foncod in ('".implode("','", $_REQUEST['fontesof'])."') 
												GROUP BY 
													f.foncod, 
													f.fondsc 
												ORDER BY
													f.foncod, 
													f.fondsc";
							$fontesof = $db->carregar( $sql_carregados );
						}
						
						combo_popup( 'fontesof', $sql_combo, 'Selecione a(s) Fonte(s)', '400x400', 0, array(), '', 'S', true, true);
					?>
				</div>
			</td>
		</tr>
		<? if ( $fontesof )  { ?> <script type="text/javascript"> onOffCampo( 'fontesof' ); </script> <? } ?>
		<? if ( $fontesiafi )  { ?> <script type="text/javascript"> onOffCampo( 'fontesiafi' ); </script> <? } ?>
			<tr>
				<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'fonte' );">
					Fonte de Recurso
					<input type="hidden" id="fonte_campo_flag" name="fonte_campo_flag" value="0"/>
				</td>
				<td>
					<div id="fonte_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'fonte' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="fonte_campo_on" style="display:none;">
						<?  
						$sql_combo = "SELECT
										f.codigo AS codigo,
										f.codigo || ' - ' || f.descricao AS descricao 
									FROM
										financeiro.execucao r 
									INNER JOIN financeiro.fonterecursos f ON f.codigo = substr( r.foncod, 2, 2) 
									--WHERE
									--	r.rofano = '" . $ano . "' 
									GROUP BY 
										f.codigo, 
										f.descricao 
									ORDER BY
										f.codigo, 
										f.descricao";
															
						if ( $_REQUEST['fonte'] && $_REQUEST['fonte'][0] != '' )
						{
							//$sql_carregados = "select fr.foncod as codigo, fr.foncod || ' - ' || fr.fondsc as descricao from financeiro.execucao r inner join public.fonterecurso fr on r.foncod = fr.foncod where r.rofano = '" . $ano . "' and fr.foncod in ('".implode("','",$_REQUEST['fonte'])."') group by fr.foncod, fr.fondsc order by fr.foncod, fr.fondsc ";
							$sql_carregados = "SELECT
													f.codigo AS codigo,
													f.codigo || ' - ' || f.descricao AS descricao 
												FROM
													financeiro.execucao r 
												INNER JOIN financeiro.fonterecursos f ON f.codigo = substr( r.foncod, 2, 2) 
												WHERE
													f.codigo in ('".implode("','",$_REQUEST['fonte'])."') 
												GROUP BY 
													f.codigo, 
													f.descricao 
												ORDER BY
													f.codigo, 
													f.descricao";
							
							$fonte=$db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'fonte', $sql_combo, 'Selecione a(s) Fonte(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $fonte )  { ?>	<script type="text/javascript"> onOffCampo( 'fonte' ); </script> <? } ?>
			
		
			
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'catecon' );">
					Categoria Econômica
					<input type="hidden" id="catecon_campo_flag" name="catecon_campo_flag" value="0"/>
				</td>
				<td>
					<div id="catecon_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'catecon' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="catecon_campo_on" style="display:none;">
						<? $sql_combo = "select cte.ctecod as codigo, cte.ctecod || ' - ' || cte.ctedsc as descricao from financeiro.execucao r inner join public.categoriaeconomica cte on cte.ctecod = r.ctecod where r.rofano = '" . $ano . "' group by cte.ctecod, cte.ctedsc order by cte.ctecod, cte.ctedsc"; 
						if ( $_REQUEST['catecon'] && $_REQUEST['catecon'][0] != '' )
						{
							$sql_carregados = "select cte.ctecod as codigo, cte.ctecod || ' - ' || cte.ctedsc as descricao from financeiro.execucao r inner join public.categoriaeconomica cte on cte.ctecod = r.ctecod where r.rofano = '" . $ano . "' and cte.ctecod in ( '" . implode( "','", $_REQUEST['catecon'] ) . "' ) group by cte.ctecod, cte.ctedsc order by cte.ctecod, cte.ctedsc";
							$catecon = $db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'catecon', $sql_combo, 'Selecione o(s) GND(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $catecon )  { ?>	<script type="text/javascript"> onOffCampo( 'catecon' ); </script> <? } ?>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'gnd' );">
					GND
					<input type="hidden" id="gnd_campo_flag" name="gnd_campo_flag" value="0"/>
				</td>
				<td>
					<div id="gnd_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'gnd' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="gnd_campo_on" style="display:none;">
						<? $sql_combo = "select g.gndcod as codigo, g.gndcod || ' - ' || g.gnddsc as descricao from financeiro.execucao r inner join public.gnd g on r.gndcod = g.gndcod where r.rofano = '" . $ano . "' group by g.gndcod, g.gnddsc order by g.gndcod, g.gnddsc"; 
						if ( $_REQUEST['gnd'] && $_REQUEST['gnd'][0] != '' )
						{
							$sql_carregados = "select g.gndcod as codigo, g.gndcod || ' - ' || g.gnddsc as descricao from financeiro.execucao r inner join public.gnd g on r.gndcod = g.gndcod where r.rofano = '" . $ano . "' and g.gndcod in ( '" . implode( "','", $_REQUEST['gnd'] ) . "' ) group by g.gndcod, g.gnddsc ";
							$gnd = $db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'gnd', $sql_combo, 'Selecione o(s) GND(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $gnd )  { ?>	<script type="text/javascript"> onOffCampo( 'gnd' ); </script> <? } ?>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'mapcod' );">
					Modalidade de Aplicação
					<input type="hidden" id="mapcod_campo_flag" name="mapcod_campo_flag" value="0"/>
				</td>
				<td>
					<div id="mapcod_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'mapcod' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="mapcod_campo_on" style="display:none;">
						<? $sql_combo = "select ma.mapcod as codigo, ma.mapcod || ' - ' || ma.mapdsc as descricao from financeiro.execucao r inner join modalidadeaplicacao ma ON ma.mapcod = r.mapcod where r.rofano = '" . $ano . "' group by ma.mapcod, ma.mapdsc order by ma.mapcod, ma.mapdsc "; 
						if ( $_REQUEST['mapcod'] && $_REQUEST['mapcod'][0] != '' )
						{
							$sql_carregados = "select ma.mapcod as codigo, ma.mapcod || ' - ' || ma.mapdsc as descricao from financeiro.execucao r inner join modalidadeaplicacao ma ON ma.mapcod = r.mapcod where r.rofano = '" . $ano . "' and ma.mapcod in ('".implode("','",$_REQUEST['mapcod'])."') group by ma.mapcod, ma.mapdsc ";
							$mapcod=$db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'mapcod', $sql_combo, 'Selecione a(s) Modalidade(s) de Aplicação', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $mapcod )  { ?>	<script type="text/javascript"> onOffCampo( 'mapcod' ); </script> <? } ?>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'elemento' );">
					Elemento de Despesa
					<input type="hidden" id="elemento_campo_flag" name="elemento_campo_flag" value="0"/>
				</td>
				<td>
					<div id="elemento_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'elemento' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="elemento_campo_on" style="display:none;">
						<? $sql_combo = "select ele.edpcod as codigo, ele.edpcod || ' - ' || ele.edpdsc as descricao from financeiro.execucao r inner join public.elementodespesa ele on ele.edpcod = r.edpcod where r.rofano = '" . $ano . "' group by ele.edpcod, ele.edpdsc order by ele.edpcod, ele.edpdsc"; 
						if ( $_REQUEST['elemento'] && $_REQUEST['elemento'][0] != '' )
						{
							$sql_carregados = "select ele.edpcod as codigo, ele.edpcod || ' - ' || ele.edpdsc as descricao from financeiro.execucao r inner join public.elementodespesa ele on ele.edpcod = r.edpcod where r.rofano = '" . $ano . "' and ele.edpcod in ('".implode("','",$_REQUEST['elemento'])."') group by ele.edpcod, ele.edpdsc order by ele.edpcod, ele.edpdsc";
							$elemento=$db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'elemento', $sql_combo, 'Selecione o(s) Elemento(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $elemento )  { ?>	<script type="text/javascript"> onOffCampo( 'elemento' ); </script> <? } ?>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'natureza' );">
					Natureza de Despesa
					<input type="hidden" id="natureza_campo_flag" name="natureza_campo_flag" value="0"/>
				</td>
				<td>
					<div id="natureza_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'natureza' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="natureza_campo_on" style="display:none;">
						<? $sql_combo = "select cast(nd.ctecod as varchar) || cast(nd.gndcod as varchar) || nd.mapcod || nd.edpcod as codigo, nd.ctecod || '.' || nd.gndcod || '.' || nd.mapcod || '.' || nd.edpcod || ' - ' || nd.ndpdsc as descricao from financeiro.execucao r inner join public.naturezadespesa nd on nd.ctecod = r.ctecod and nd.gndcod = r.gndcod and nd.mapcod = r.mapcod and nd.edpcod = r.edpcod where r.rofano = '" . $ano . "' group by nd.ctecod, nd.gndcod, nd.mapcod, nd.edpcod, nd.ndpdsc order by nd.ctecod, nd.gndcod, nd.mapcod, nd.edpcod, nd.ndpdsc"; 
						if ( $_REQUEST['natureza'] && $_REQUEST['natureza'][0] != '' )
						{
							$sql_carregados = "select cast(nd.ctecod as varchar) || cast(nd.gndcod as varchar) || nd.mapcod || nd.edpcod as codigo, nd.ctecod || '.' || nd.gndcod || '.' || nd.mapcod || '.' || nd.edpcod || ' - ' || nd.ndpdsc as descricao from financeiro.execucao r inner join public.naturezadespesa nd on nd.ctecod = r.ctecod and nd.gndcod = r.gndcod and nd.mapcod = r.mapcod and nd.edpcod = r.edpcod where r.rofano = '" . $ano . "' and nd.ctecod || nd.gndcod || nd.mapcod || nd.edpcod in ('".implode("','",$_REQUEST['natureza'])."') group by nd.ctecod, nd.gndcod, nd.mapcod, nd.edpcod, nd.ndpdsc order by nd.ctecod, nd.gndcod, nd.mapcod, nd.edpcod, nd.ndpdsc";
							$natureza=$db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'natureza', $sql_combo, 'Selecione a(s) Natureza(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $natureza )  { ?>	<script type="text/javascript"> onOffCampo( 'natureza' ); </script> <? } ?>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'usopri' );">
					Ident. Result. Primário
					<input type="hidden" id="usopri_campo_flag" name="usopri_campo_flag" value="0"/>
				</td>
				<td>
					<div id="usopri_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'usopri' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="usopri_campo_on" style="display:none;">
						<? $sql_combo = "select irp.irpcod as codigo, irp.irpcod || ' - ' || irp.irpdsc as descricao from financeiro.execucao r inner join identresultprimario irp ON irp.irpcod = r.irpcod where r.rofano = '" . $ano . "' group by irp.irpcod, irp.irpdsc order by irp.irpcod, irp.irpdsc"; 
						if ( $_REQUEST['usopri'] && $_REQUEST['usopri'][0] != '' )
						{
							$sql_carregados = "select irp.irpcod as codigo, irp.irpcod || ' - ' || irp.irpdsc as descricao from financeiro.execucao r inner join identresultprimario irp ON irp.irpcod = r.rofrp where r.rofano = '" . $ano . "' and irp.irpcod in ('".implode("','",$_REQUEST['usopri'])."') group by irp.irpcod, irp.irpdsc ";
							$usopri=$db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'usopri', $sql_combo, 'Selecione o(s) Ident. Result. Primário(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $usopri )  { ?>	<script type="text/javascript"> onOffCampo( 'usopri' ); </script> <? } ?>
		</table>
	</div>
	
	<!-- FIM CLASSIFICACAO ECONÔMICA -->
	
	<!-- ----------------------------------------------------------------------- -->
	
	
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
			<tr>
				<td align="center">
					<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:submeterFormulario( 'relatorio' );"/>
					&nbsp;
					<input type="button" name="Exportar Planilha" value="Exportar Planilha" onclick="javascript:submeterFormulario( 'planilha' );"/>
					&nbsp;
					<input type="button" name="Salvar Consulta" value="Salvar Consulta" onclick="javascript:submeterFormulario( 'salvar' );"/>
				</td>
			</tr>
	</table>
	
	<!-- ----------------------------------------------------------------------- -->

</form>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script type="text/javascript">
	
	onOffBloco( 'inst' );
	onOffBloco( 'func' );
	onOffBloco( 'econ' );
	//onOffBloco( 'outros' );
	//onOffBloco( 'minhasconsultas' );

	function tornar_publico( prtid )
	{
//		location.href = '?modulo=<?//= $modulo ?>&acao=R&prtid='+ prtid + '&publico=1';
		document.formulario.publico.value = '1';
		document.formulario.prtid.value = prtid;
		document.formulario.submit();
	}
	
	function excluir_relatorio( prtid )
	{
		document.formulario.excluir.value = '1';
		document.formulario.prtid.value = prtid;
		document.formulario.target = '_self';
		document.formulario.submit();
	}
	
	function carregar_consulta( prtid )
	{
		document.formulario.carregar.value = '1';
		document.formulario.prtid.value = prtid;
		document.formulario.target = '_self';
//		submeterFormulario('salvar');
//		alert( submeterFormulario('salvar') );
		document.formulario.submit();
	}
	
	function carregar_relatorio( prtid )
	{
		document.formulario.prtid.value = prtid;
		document.formulario.alterar_ano.value = '0';
		submeterFormulario( 'relatorio' );
	}

</script>