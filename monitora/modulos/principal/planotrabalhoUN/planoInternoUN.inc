<?php

include APPRAIZ. '/includes/Agrupador.php';
include 'ConsultaFinanceiroUN.inc';

$ano = $_SESSION['exercicio'];

$possuiPerfilConsultaUnidades = false;

//ver($_POST,$_GET);

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



// remove flag de submissão de formulário
if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] )
{
	unset( $_REQUEST['form'] );
}
// FIM remove flag de submissão de formulário

// variáveis que alimentam o formulário
// variáveis dos filtros gerais
$titulo = '';
//$ano = '';
$nao_mostra_filtro_impressao = false;
$agrupador = array();
$sumarizador = '';
$publico = '';
$prtid = '';
$excluir = '';
$escala = '1';

// FIM variáveis que alimentam o formulário

// carrega consulta do banco
if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] )
{
	$oConsultaFinanceiroTraducao = new ConsultaFinanceiroTraducao();
	$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = ".$_REQUEST['prtid'] );
	$itens = $db->pegaUm( $sql );
	$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
	extract( $dados );
	$_REQUEST = $dados;//array_merge( $_REQUEST, $dados );
	unset( $_REQUEST['form'] );
	$titulo = $_REQUEST['titulo'];
	//$ano = $_REQUEST['ano'];
	//$agrupador = $_REQUEST['agrupador'];
	$agrupador = array();
	if ( $_REQUEST['agrupador'] )
	{
		foreach ( $_REQUEST['agrupador'] as $valorAgrupador )
		{
			array_push( $agrupador, array( 'codigo' => $valorAgrupador, 'descricao' => $oConsultaFinanceiroTraducao->pegarTitulo( $valorAgrupador ) ) );
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
$plicod = array();
$sbacod = array();

include "planotrabalhoUN/_constantes.php";
include "planotrabalhoUN/_funcoes.php";
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// montando o menu
echo montarAbasArray(carregardadosplanotrabalhoUN_sub(), "/monitora/monitora.php?modulo=principal/planotrabalhoUN/planoInternoUN&acao=R&atiid=".$_REQUEST['atiid']);

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
		//if ( formulario.alterar_ano.value == '0' && tipo != 'relatorio' )
		//{
			if ( formulario.agrupador.options.length == 0 )
			{
				alert( 'Escolha pelo menos um agrupador.' );
				return;
			}
		//}
		
		if ( formulario.alterar_ano.value == '0' && tipo == 'relatorio' )
		{
			formulario.action = 'monitora.php?modulo=principal/planotrabalhoUN/planoInternoUN&acao=R';
			//window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			//formulario.target = 'relatorio';
		}
		else
		{
			if ( tipo == 'planilha' )
			{
				formulario.action = 'monitora.php?modulo=principal/planotrabalhoUN/planoInternoUN&acao=R&tipoRelatorio=xls';
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
				formulario.action = 'monitora.php?modulo=principal/planotrabalhoUN/planoInternoUN&acao=R&salvar=1';
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
<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="form" value="1"/> <!-- indica envio de formulário -->
	<input type="hidden" name="alterar_ano" value="0"/> <!-- indica se há mudança de ano no formulário -->
	<input type="hidden" name="publico" value="<?= $publico ?>"/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" value="<?= $prtid ?>"/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	<input type="hidden" name="excluir" value="<?= $excluir ?>"/> <!-- indica se foi clicado para excluir o relatório já gravado -->
	<input type="hidden" name="escala" value="<?= $escala ?>"/> 
	<input type="hidden" name="ano" value="<?= $ano ?>"/> 
	<input type="hidden" name="uo" value="<?= $_SESSION['monitora_var']['unicod'] ?>"/>
	<input type="hidden" name="atiid" value="<?= $_REQUEST['atiid'] ?>"/>
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
			<tr>
				<td width="195" class="SubTituloDireita" valign="top">Agrupadores</td>
				<td>
					<?php
						$oConsultaFinanceiroTraducao = new ConsultaFinanceiroTraducao();
						$matriz = $oConsultaFinanceiroTraducao->pegarAgrupadores();
						$campoAgrupador = new Agrupador( 'formulario' );
						$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
						$campoAgrupador->setDestino( 'agrupador', 4 );
						$campoAgrupador->exibir();
					?>
				</td>
			</tr>
	</table>
	
	<!-- OUTROS FILTROS -->
	
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
				<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'programa' );">
					Programa
					<input type="hidden" id="programa_campo_flag" name="programa_campo_flag" value="0"/>
				</td>
				<td>
					<div id="programa_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'programa' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="programa_campo_on" style="display:none;">
						<? $sql_combo = "select p.prgcod as codigo, p.prgcod || ' - ' || p.prgdsc as descricao from financeiro.execucao r inner join planejamento.programa p on r.prgcod = p.prgcod where r.rofano = '" . $ano . "' group by p.prgcod, p.prgdsc order by p.prgcod, p.prgdsc";
						if ( $_REQUEST['programa'] && $_REQUEST['programa'][0] != '' )	
						{
							$sql_carregados = "select p.prgcod as codigo, p.prgcod || ' - ' || p.prgdsc as descricao from financeiro.execucao r inner join planejamento.programa p on r.prgcod = p.prgcod where r.rofano = '" . $ano . "' and p.prgcod in ('".implode("','",$_REQUEST['programa'])."') group by p.prgcod, p.prgdsc ";
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
						<? $sql_combo = "select acao.acacod as codigo, acao.acacod || ' - ' || acao.acadsc as descricao from financeiro.execucao r inner join ( select distinct acacod, acadsc, prgano from planejamento.acao where prgano = '".$ano."' and acasnrap = 'f' group by acacod, acadsc, prgano ) acao on acao.acacod = r.acacod and acao.prgano = r.rofano where r.rofano = '" . $ano . "' group by acao.acacod, acao.acadsc order by acao.acacod, acao.acadsc";
						if ( $_REQUEST['acacod'] && $_REQUEST['acacod'][0] != '' )
						{
							$sql_carregados = "select acao.acacod as codigo, acao.acacod || ' - ' || acao.acadsc as descricao from financeiro.execucao r inner join ( select distinct acacod, acadsc, prgano from planejamento.acao where prgano = '$ano' and acasnrap = 'f' group by acacod, acadsc, prgano ) acao on acao.acacod = r.acacod and acao.prgano = r.rofano where r.rofano = '" . $ano . "' and acao.acacod in ('".implode("','",$_REQUEST['acacod'])."') group by acao.acacod, acao.acadsc ";
							$acacod = $db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'acacod', $sql_combo, 'Selecione a(s) Ação(ões)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $acacod )  { ?>	<script type="text/javascript"> onOffCampo( 'acacod' ); </script> <? } ?>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'plicod' );">
					Plano Interno
					<input type="hidden" id="plicod_campo_flag" name="plicod_campo_flag" value="0"/>
				</td>
				<td>
					<div id="plicod_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'plicod' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="plicod_campo_on" style="display:none;">
						<? $sql_combo = "select plicod as codigo, plicod || ' - ' || plititulo as descricao from monitora.pi_planointerno where pliano = '".$ano."'";
						if ( $_REQUEST['plicod'] && $_REQUEST['plicod'][0] != '' )
						{
							$sql_carregados = "select plicod as codigo, plicod || ' - ' || plititulo as descricao from monitora.pi_planointerno where pliano = '".$ano."' and plicod in ('".implode("','",$_REQUEST['plicod'])."')";
							$plicod = $db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'plicod', $sql_combo, 'Selecione o(s) Plano(s) Interno(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $plicod )  { ?>	<script type="text/javascript"> onOffCampo( 'plicod' ); </script> <? } ?>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'sbacod' );">
					Subação
					<input type="hidden" id="sbacod_campo_flag" name="sbacod_campo_flag" value="0"/>
				</td>
				<td>
					<div id="sbacod_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'sbacod' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="sbacod_campo_on" style="display:none;">
						<? $sql_combo = "select sbacod as codigo, sbacod || ' - ' || sbatitulo as descricao from monitora.pi_subacao";
						if ( $_REQUEST['sbacod'] && $_REQUEST['sbacod'][0] != '' )
						{
							$sql_carregados = "select sbacod as codigo, sbacod || ' - ' || sbatitulo as descricao from monitora.pi_subacao where sbacod in ('".implode("','",$_REQUEST['sbacod'])."')";
							$sbacod = $db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'sbacod', $sql_combo, 'Selecione o(s) Plano(s) Interno(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $sbacod )  { ?>	<script type="text/javascript"> onOffCampo( 'sbacod' ); </script> <? } ?>
			
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
			<tr>
			<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'fontesiafi' );">
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
		</table>
	</div>
	
	<!-- FIM CLASSIFICACAO ECONÔMICA -->
	
	<!-- ----------------------------------------------------------------------- -->
	
	
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
			<tr>
				<td align="center">
					<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:submeterFormulario( 'relatorio' );"/>
					<!-- &nbsp;
					<input type="button" name="Exportar Planilha" value="Exportar Planilha" onclick="javascript:submeterFormulario( 'planilha' );"/>
					&nbsp;
					<input type="button" name="Salvar Consulta" value="Salvar Consulta" onclick="javascript:submeterFormulario( 'salvar' );"/>
					 -->
				</td>
			</tr>
	</table>
	
	<!-- ----------------------------------------------------------------------- -->

</form>
<center>
<table class="tabela">
	<tr>
		<td>
<?php 
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
	//ver($_POST,d);
	include 'resultado_geralUN.inc'; 
	//exit();
}
?>
		</td>
	</tr>
</table>
</center>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script type="text/javascript">
	
	//onOffBloco( 'inst' );
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