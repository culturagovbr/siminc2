<?php

if ( isset( $_REQUEST['form'] ) )
{
	include "financeiro_fisico_resultado.php";
	exit();
}

$agrupadorHtml =
<<<EOF
	<table>
		<tr valign="middle">
			<td>
				<select id="{NOME_ORIGEM}" name="{NOME_ORIGEM}[]" multiple="multiple" size="4" style="width: 140px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );" class="combo campoEstilo"></select>
			</td>
			<td>
				<img src="../imagens/rarrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
				<!--
				<img src="../imagens/rarrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
				<img src="../imagens/larrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, ''); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
				-->
				<img src="../imagens/larrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
			</td>
			<td>
				<select id="{NOME_DESTINO}" name="{NOME_DESTINO}[]" multiple="multiple" size="4" style="width: 140px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );" class="combo campoEstilo"></select>
			</td>
			<td>
				<img src="../imagens/uarrow.gif" style="padding: 5px" onClick="subir( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
				<img src="../imagens/darrow.gif" style="padding: 5px" onClick="descer( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
			</td>
		</tr>
	</table>
	<script type="text/javascript" language="javascript">
		limitarQuantidade( document.getElementById( '{NOME_DESTINO}' ), {QUANTIDADE_DESTINO} );
		limitarQuantidade( document.getElementById( '{NOME_ORIGEM}' ), {QUANTIDADE_ORIGEM} );
		{POVOAR_ORIGEM}
		{POVOAR_DESTINO}
		sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );
	</script>
EOF;

include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Relatório Físico/Financeiro', 'Relatório Geral' );

// ano utilizado pelos filtros
$ano = $_SESSION['exercicio'];

?>
<script type="text/javascript">
	
	function enviarFormulario( excel )
	{
		formulario = document.filtro;
		if ( !excel )
		{
			if ( !formulario.agrupadorMacro.options.length )
			{
				alert( 'Escolhe ao menos um agrupador macro' );
				return;
			}
			if ( !formulario.agrupadorMicro.options.length )
			{
				alert( 'Escolhe ao menos um agrupador micro' );
				return;
			}
		}
		selectAllOptions( formulario.agrupadorMacro );
		selectAllOptions( formulario.agrupadorMicro );
		selectAllOptions( formulario.acacod );
		selectAllOptions( formulario.programa );
		selectAllOptions( formulario.uo );
		
		if ( !excel )
		{
			formulario.xls.value = '0';
			formulario.target = 'resultado';
			var janela = window.open( '', 'resultado', 'width=750,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		}
		else
		{
			formulario.xls.value = '1';
			formulario.target = '_top';
		}
		formulario.submit();
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

</script>
<form action="" method="post" name="filtro" target="resultado">
	<input type="hidden" name="form" value="1"/> <!-- indica envio de formulário -->
	<input type="hidden" name="xls" value="0"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		
		<!-- AGRUPADOR MACRO ----------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">
				Agrupador macro
			</td>
			<td width="80%">
				<?php
					
					// inicia agrupador
					$agrupador = new Agrupador( 'filtro', $agrupadorHtml );
					$destino = array();
					$origem = array(
						'pro' => array(
							'codigo'    => 'pro',
							'descricao' => 'Programa'
						),
						'uo' => array(
							'codigo'    => 'uo',
							'descricao' => 'Unidade Orçamentária'
						)
					);
					
					// move os dados da origem par ao destino de acordo com os campo da variável $agrupadorMacro
					/*
					foreach ( $agrupadorMacro as $campo )
					{
						if ( array_key_exists( $campo, $origem ) )
						{
							array_push( $destino, $origem[$campo] );
							unset( $origem[$campo] );
						}
					}
					*/
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoAgrupadoMacro', null, $origem );
					$agrupador->setDestino( 'agrupadorMacro', null, $destino );
					$agrupador->exibir();
					
				?>
			</td>
		</tr>
		
		<!-- AGRUPADOR MICRO ----------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Agrupador micro
			</td>
			<td>
				<?php
					
					// inicia agrupador
					$agrupador = new Agrupador( 'filtro', $agrupadorHtml );
					$destino = array();
					$origem = array(
						'loc' => array(
							'codigo'    => 'loc',
							'descricao' => 'Localizador'
						),
						'aca' => array(
							'codigo'    => 'aca',
							'descricao' => 'Ação'
						)
					);
					
					// move os dados da origem par ao destino de acordo com os campo da variável $agrupadorMicro
					/*
					foreach ( $agrupadorMicro as $campo )
					{
						if ( array_key_exists( $campo, $origem ) )
						{
							array_push( $destino, $origem[$campo] );
							unset( $origem[$campo] );
						}
					}
					*/
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoAgrupadoMicro', null, $origem );
					$agrupador->setDestino( 'agrupadorMicro', null, $destino );
					$agrupador->exibir();
					
				?>
			</td>
		</tr>
		
		<!-- CUMULATIVO ---------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Cumulativo
			</td>
			<td>
				<select name="cumulativo">
					<option value="" <?php echo $cumulativo == "" ? 'selected="selected"' : '' ?>>
						&nbsp;
					</option>
					<option value="Sim" <?php echo $cumulativo == "Sim" ? 'selected="selected"' : '' ?>>
						Sim
					</option>
					<option value="Não" <?php echo $cumulativo == "Não" ? 'selected="selected"' : '' ?>>
						Não
					</option>
				</select>
			</td>
		</tr>
		
		<!-- PRODUTO ------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				&nbsp;
			</td>
			<td>
				<input type="radio" name="produto" value="sim" id="produto_sim"/>
				<label for="produto_sim">Com produto</label>
				&nbsp;&nbsp;
				<input type="radio" name="produto" value="nao" id="produto_nao"/>
				<label for="produto_nao">Sem produto</label>
				&nbsp;&nbsp;
				<input type="radio" name="produto" value="todos" id="produto_todos" checked="checked"/>
				<label for="produto_todos">Todos</label>
			</td>
		</tr>
		
		<!-- ESCALA FINANCEIRO --------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Escala financeiro
			</td>
			<td>
				<input type="radio" name="escala_fin" value="1" id="escala_fin_1"/>
				<label for="escala_fin_1">R$ 1</label>
				&nbsp;&nbsp;
				<input type="radio" name="escala_fin" value="1000" id="escala_fin_1000" checked="checked"/>
				<label for="escala_fin_1000">R$ 1000</label>
			</td>
		</tr>
		
		<!-- AÇÃO ---------------------------------------------------------- -->
		<?php $possuiSelecionado = count( $acacod ); ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'acacod' );">
				Ação
				<input
					type="hidden"
					id="acacod_campo_flag"
					name="acacod_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="acacod_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'acacod' );"
				><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="acacod_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
						$sql_combo =
							"select
								acao.acacod as codigo,
								acao.acacod || ' - ' || acao.acadsc as descricao
							from monitora.acao acao
								left join monitora.dadofinanceiro fin on fin.acaid = acao.acaid
								left join monitora.dadofisico fis on fis.acaid = acao.acaid
							where
								acao.prgano = '" . $ano . "' and
								(
									fin.acaid is not null or
									fis.acaid is not null
								)
							group by acao.acacod, acao.acadsc
							order by acao.acacod, acao.acadsc"; 
						if ( $possuiSelecionado )
						{
							$sql_carregados =
								"select
									acao.acacod as codigo,
									acao.acacod || ' - ' || acao.acadsc as descricao
								from monitora.acao acao
								where
									acao.acacod in ( '" . implode( "','", $_REQUEST['acacod'] ) . "' )
								group by acao.acacod, acao.acadsc
								order by acao.acacod, acao.acadsc";
							$acacod = $db->carregar( $sql_carregados );
						}
						$acacod_campo_excludente = !!$_REQUEST['acacod_campo_excludente'];
						combo_popup(
							'acacod',						// nome do campo
							$sql_combo,						// sql de captura dos valores
							'Selecione a(s) Ação(ões)',		// label
							'400x400',						// tamanho do popup
							0,								// quantidade máxima de itens
							array(),						// valores fixos
							'',								// mensagem
							'S',							// habilitado
							true,							// habilitar busca por código
							true							// habilitar flag contém
						);
					?>
				</div>
			</td>
		</tr>
		
		<!-- PROGRAMA ------------------------------------------------------ -->
		<?php $possuiSelecionado = count( $prgcod ); ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'programa' );">
				Programa
				<input
					type="hidden"
					id="programa_campo_flag"
					name="programa_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="programa_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'programa' );"
				><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="programa_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
						$sql_combo =
							"select
								p.prgcod as codigo,
								p.prgcod || ' - ' || p.prgdsc as descricao
							from planejamento.programa p 
							where
								prgano = '" . $ano . "'
							group by p.prgcod, p.prgdsc
							order by p.prgcod, p.prgdsc"; 
						if ( $possuiSelecionado )	
						{
							$sql_carregados =
								"select
									p.prgcod as codigo,
									p.prgcod || ' - ' || p.prgdsc as descricao
								from planejamento.programa p 
								where
									prgano = '" . $ano . "' and
									p.prgcod in ( '" . implode( "','", $_REQUEST['programa'] ) . "' )
								group by p.prgcod, p.prgdsc ";
							$programa=$db->carregar( $sql_carregados );
						}
						$programa_campo_excludente = !!$_REQUEST['programa_campo_excludente'];
						combo_popup(
							'programa',						// nome do campo
							$sql_combo,						// sql de captura dos valores
							'Selecione o(s) Programa(s)',	// label
							'400x400',						// tamanho do popup
							0,								// quantidade máxima de itens
							array(),						// valores fixos
							'',								// mensagem
							'S',							// habilitado
							true,							// habilitar busca por código
							true							// habilitar flag contém
						);
					?>
				</div>
			</td>
		</tr>
		
		<!-- UNIDADE ORCAMENTÁRIA ------------------------------------------ -->
		<?php $possuiSelecionado = count( $unicod ); ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="onOffCampo( 'uo' );">
				Unidades Orçamentárias
				<input
					type="hidden"
					id="uo_campo_flag"
					name="uo_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<?php $possuiSelecionado ? "block" : "none" ?>
				<div
					id="uo_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'uo' );"
				><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="uo_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
						$sql_combo =
							"select
								entunicod as codigo,
								entunicod  || ' - ' || entnome as descricao
							from entidade.entidade 
							where entunicod is not null and entunicod like '26%'
							order by entunicod, entnome"; 
						if ( $possuiSelecionado )
						{
							$sql_carregados =
								"select
									entunicod as codigo,
									entunicod || ' - ' || entnome as descricao
								from entidade.entidade
								where
									entunicod is not null and entunicod like '26%' and
									entunicod in ( '" . implode( "','", $_REQUEST['uo'] ) ."' )
								group by entunicod, entnome ";
							$uo = $db->carregar( $sql_carregados );
						}
						$uo_campo_excludente = !!$_REQUEST['uo_campo_excludente'];
						combo_popup(
							'uo',							// nome do campo
							$sql_combo,						// sql de captura dos valores
							'Selecione a(s) Unidade(s)',	// label
							'400x400',						// tamanho do popup
							0,								// quantidade máxima de itens
							array(),						// valores fixos
							'',								// mensagem
							'S',							// habilitado
							true,							// habilitar busca por código
							true							// habilitar flag contém
						);
					?>
				</div>
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita" valign="top">
				&nbsp;
			</td>
			<td class="SubTituloDireita" style="text-align:left;">
				<input
					type="button"
					name="filtrar"
					value="HTML"
					onclick="enviarFormulario( false );"
				/>
				
				<input
					type="button"
					name="filtrar"
					value="Microsoft Excel"
					onclick="enviarFormulario( true );"
				/>
			</td>
		</tr>
		
	</table>
</form>







