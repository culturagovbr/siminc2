<?php

	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: SPO/MEC
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	 * Programadores: Renan de Lima Barbosa <renandelima@gmail.com>, Renê de Lima Barbosa <renedelima@gmail.com>
	 * Módulo: Usuário
	 * Finalidade: Permitir o controle de cadastro de usuários.
	 * Data de criação: 19/11/2006
	 * Última modificação: 21/11/2006
	 */

	define( 'SENHA_PADRAO', 'siminc123' );

	$status = array(
		'A' => 'Ativo',
		'B' => 'Bloqueado',
		'P' => 'Pendente'
	);
	
	// carrega os dados da conta do usuário
	$sql = sprintf(
		"SELECT u.* FROM seguranca.usuario u WHERE usucpf = '%s'",
		$_REQUEST['usucpf']
	);
	$usuario = $db->pegaLinha( $sql );
	extract( $usuario );
	
	$usuario = (object) $usuario;
	$cpf = formatar_cpf( $usuario->usucpf );
	
	if ( $_REQUEST['formulario'] ) {
		
		// atualiza dados gerais do usuário
		$sql = sprintf(
			"UPDATE seguranca.usuario SET
				usunome = '%s',
				usuemail = '%s',
				usufoneddd = '%s',
				usufonenum = '%s',
				usufuncao = '%s',
				orgcod = '%s',
				unicod = '%s',
				regcod = '%s',
				ungcod = '%s',
				ususexo = '%s'
			WHERE usucpf = '%s'",
			$_REQUEST['usunome'],
			$_REQUEST['usuemail'],
			$_REQUEST['usufoneddd'],
			$_REQUEST['usufonenum'],
			$_REQUEST['usufuncao'],
			$_REQUEST['orgcod'],
			$_REQUEST['unicod'],
			$_REQUEST['regcod'],
			$_REQUEST['ungcod'],
			$_REQUEST['ususexo'],
			$usuario->usucpf
		);
		$db->executar( $sql );

		// altera a senha do usuário com o valor padrão
		if ( $_REQUEST['senha'] ) {
			$sql = sprintf(
				"UPDATE seguranca.usuario SET ususenha = '%s', usuchaveativacao = 'f' WHERE usucpf = '%s'",
				md5_encrypt_senha( SENHA_PADRAO, '' ),
				$usucpf
			);
			$db->executar( $sql );
		}

		// aplica as alterações de status nos sistemas
		foreach ( $_REQUEST['status'] as $sisid => $suscod ) {
			$sql = sprintf(
				"SELECT us.* FROM seguranca.usuario_sistema us WHERE sisid = %d AND usucpf = '%s'",
				$sisid,
				$usuario->usucpf
			);
			$usuariosistema = (object) $db->pegaLinha( $sql );
			if ( !$usuariosistema->sisid ) {
				$sql = sprintf(
					"INSERT INTO seguranca.usuario_sistema ( sisid, usucpf ) VALUES ( %d, '%s' )",
					$sisid,
					$usuario->usucpf
				);
		    	$db->executar( $sql );
			}
			if ( $usuariosistema->suscod != $suscod ) {
				$justificativa = str_replace("'","''",$_REQUEST['justificativa'][$sisid]);
				$db->alterar_status_usuario( $usuario->usucpf, $suscod, $justificativa, $sisid );
				$email_aprovacao = $usuariosistema->suscod == 'P' && $suscod == 'A' ? true : $email_aprovacao;
			}
		}

		// executa rotina para alteração do status geral no sistema
		if ( in_array( 'geral', $configuracao ) ) {
			if ( $usuario->suscod != $_REQUEST['suscod'] ) {
				$db->alterar_status_usuario( $usuario->usucpf, str_replace("'","''",$_REQUEST['suscod'], $_REQUEST['htudsc']) );
				$email_aprovacao = $usuario->suscod == 'P' && $_REQUEST['suscod'] == 'A';
			}
		}

		// envia o email de confirmação caso a conta seja aprovada
		if ( $email_aprovacao ) {
			$remetente = $_SESSION['usuemail'];
			$destinatario = $_REQUEST['usuemail'];
			$assunto = "Aprovação do Cadastro no ".SIGLA_SISTEMA;
			$conteudo = sprintf(
				"%s %s<p>Sua conta está ativa. Sua Senha de acesso é: %s</p>",
				$_REQUEST['ususexo'] == 'M' ? 'Prezado Sr.' : 'Prezada Sra.',
				$_REQUEST['usunome'],
				md5_decrypt_senha( $usuario->ususenha, '' )
			);
			enviar_email( $remetente, $destinatario, $assunto, $conteudo );
		}

		// cadastra os perfils selecionados
		foreach ( $_REQUEST['pflcod'] as $sisid => $perfis ) {
			$sql = sprintf(
				"DELETE FROM seguranca.perfilusuario WHERE usucpf = '%s' AND pflcod IN ( SELECT p.pflcod FROM seguranca.perfil p WHERE p.sisid = %d )",
				$usucpf,
				$sisid
			);
			$db->executar( $sql );
			foreach ( $perfis as $pflcod ) {
				if ( empty( $pflcod ) ) {
					continue;
				}
				$sql = sprintf(
					"INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '%s', %d )",
					$usucpf,
					$pflcod
				);
				$db->executar( $sql );
			}
		}

		$db->commit();
		$parametros = '&usucpf='. $_REQUEST['usucpf'];
		$db->sucesso( $modulo, $parametros );
	}
	
	include APPRAIZ ."includes/cabecalho.inc";
	print '<br>';
	monta_titulo( 'Cadastro de Usuários', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );

?>
<form method="post" name="formulario">
	<input type="hidden" name="formulario" value="1"/>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita">CPF:</td>
			<td><?= campo_texto( 'cpf', 'N', 'N', '', 19, 14, '###.###.###-##', '' ); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Nome:</td>
			<td><?= campo_texto( 'usunome', 'S', 'S', '', 50, 50, '', '' ); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Sexo:</td>
			<td>
				<input id="sexo_masculino" type="radio" name="ususexo" value="M" <?=($ususexo=='M'?"CHECKED":"")?> <?= $cpf_cadastrado ? 'disabled="disabled"' : '' ?>/>
				<label for="sexo_masculino">Masculino</label>
				<input id="sexo_feminino" type="radio" name="ususexo" value="F" <?=($ususexo=='F'?"CHECKED":"")?> <?= $cpf_cadastrado ? 'disabled="disabled"' : '' ?>/>
				<label for="sexo_feminino">Feminino</label>
				<?= obrigatorio(); ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Unidade Federal:</td>
			<td>
				<?php
					$sql = "SELECT regcod AS codigo, regcod||' - '||descricaouf AS descricao FROM uf WHERE codigoibgeuf IS NOT NULL ORDER BY 2";
					$db->monta_combo( 'regcod', $sql, 'S', '&nbsp;', '', '' );
				?>
				<?= obrigatorio(); ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Órgão:</td>
			<td>
				<?php
				$sql = "SELECT orgcod AS codigo, orgcod||' - '||orgdsc AS descricao FROM public.orgao WHERE organo = '". $_SESSION['exercicio'] ."' order by orgdsc";

					$db->monta_combo( 'orgcod', $sql, 'S', '&nbsp;', 'listar_unidades_orcamentarias', '' );
				?>
				<?= obrigatorio(); ?>
			</td>
		</tr>
		<tr bgcolor="#F2F2F2">
			<td align = 'right' class="subtitulodireita">Unidade Orçamentária:</td>
			<td>
				<div id="unicod_on" style="display:none;">
					<select id="unicod" name="unicod" onchange="listar_unidades_gestoras( this.value )" class="CampoEstilo"></select>
					<?= obrigatorio(); ?>
				</div>
				<div id="unicod_off" style="color:#909090;">O órgão selecionado não possui unidades orçamentárias.</div>
			</td>
		</tr>
		<tr bgcolor="#F2F2F2">
			<td align = 'right' class="subtitulodireita">Unidade Gestora:</td>
			<td>
				<div id="ungcod_on" style="display:none;">
					<select id="ungcod" name="ungcod" class="CampoEstilo"></select>
					<?= obrigatorio(); ?>
				</div>
				<div id="ungcod_off" style="color:#909090;">A unidade Orçamentária selecionada não possui unidades gestoras.</div>
			</td>
		</tr>
		<tr>
			<td align='right' class="subtitulodireita">Telefone (DDD) + Telefone:</td>
			<td>
				<?= campo_texto( 'usufoneddd', 'S', 'S', '', 3, 2, '##', '' ); ?>
				<?= campo_texto( 'usufonenum', 'S', 'S', '', 18, 15, '###-####|####-####', '' ); ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="subtitulodireita">E-Mail:</td>
			<td>
				<?= campo_texto( 'usuemail', 'S', 'S', '', 50, 100, '', '' ); ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="subtitulodireita">Função/Cargo:</td>
			<td>
				<?= campo_texto( 'usufuncao', 'S', 'S', '', 50, 100, '', '' ); ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="subtitulodireita">Senha:</td>
			<td>
				<input id="senha" type="checkbox" name="senha"/>
				<label for="senha">Alterar a senha do usuário para a senha padrão: <b><?php echo SENHA_PADRAO; ?></b>.</label>
			</td>
		</tr>
		<?php if( in_array( 'geral', $configuracao ) ): ?>
			<tr>
				<td align='right' class="SubTituloDireita">Status Geral:</td>
				<td>
					<input id="status_ativo" type="radio" name="suscod" value="A" onchange="alterar_status_geral();" <?= $suscod == 'A' ? 'checked="checked"' : "" ?>/>
					<label for="status_ativo">Ativo</label>
					<input id="status_pendente" type="radio" name="suscod" value="P" onchange="alterar_status_geral();" <?= $suscod == 'P' ? 'checked="checked"' : "" ?>/>
					<label for="status_pendente">Pendente</label>
					<input id="status_bloqueado" type="radio" name="suscod" value="B" onchange="alterar_status_geral();" <?= $suscod == 'B' ? 'checked="checked"' : "" ?>/>
					<label for="status_bloqueado">Bloqueado</label>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="javascript: exibir_ocultar_historico('historico_geral');"><img src="/imagens/mais.gif" style="border: 0"/> histórico</a>
					<div id="historico_geral" style="width: 500px; display: none">
						<p>
						<?php
							$cabecalho = array(
								"Data",
								"Status",
								"Descrição",
							);
							$sql = sprintf(
								"SELECT to_char( hu.htudata, 'dd/mm/YYYY' ) as data, hu.suscod, hu.htudsc FROM seguranca.historicousuario hu WHERE usucpf = '%s' AND sisid IS NULL ORDER BY hu.htudata DESC",
								$usucpf
							);
							$db->monta_lista_simples( $sql, $cabecalho, 25, 0 );
						?>
						</p>
					</div>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Justificativa:</td>
				<td>
					<div id="justificativa_on" style="display: none;">
						<?= campo_textarea( 'htudsc', 'N', 'S', '', 100, 3, '' ); ?>
					</div>
					<div id="justificativa_off" style="display: block; color:#909090;">
						Status não alterado.
					</div>
				</td>
			</tr>
		<?php else: ?>
			<tr>
				<td align='right' class="SubTituloDireita">Status Geral:</td>
				<td><?= $status[$suscod] ?></td>
			</tr>
		<?php endif; ?>
		<?php
			$sistemas = array();
			
			if( in_array( 'seguranca', $configuracao ) ) {
				$sisid = 4;
				include 'cadastro_usuario_seguranca.php';
			}
			if( in_array( 'financeiro', $configuracao ) ) {
				$sisid = 7;
				include 'cadastro_usuario_seguranca.php';
			}
			
			if( in_array( 'monitoramento', $configuracao ) ) {
				$sisid = 1;
				include 'cadastro_usuario_monitoramento.php';
			}
			if( in_array( 'projetos_especiais', $configuracao ) ) {
				$sisid = 6;
				include 'cadastro_usuario_monitoramento.php';
			}
			
			if( in_array( 'proposta', $configuracao ) ) {
				$sisid = 2;
				include 'cadastro_usuario_elaboracao.php';
			}
			if( in_array( 'elaboracao', $configuracao ) ) {
				$sisid = 5;
				include 'cadastro_usuario_elaboracao.php';
			}
			if( in_array( 'ifes', $configuracao ) ) {
				$sisid = 8;
				include 'cadastro_usuario_ifes.php';
			}
			if( in_array( 'pde', $configuracao ) ) {
				$sisid = 10;
				include 'cadastro_usuario_pde.php';
			}
		?>
		<tr bgcolor="#C0C0C0">
			<td width="15%">&nbsp;</td>
			<td>
				<input type="button" class="botao" name="btalterar" value="Enviar" onclick="enviar_formulario();">
				<input type="button" class="botao" name="btvoltar" value="Voltar" onclick="voltar();">
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">

	<?php
		$sql =
		"select " .
			" orgcod, " .
			" unicod, " .
			" unicod || ' - ' || unidsc as unidsc " .
		" from unidade " .
		" where " .
			" unistatus = 'A' and " .
			" unitpocod = 'U' " .
		" order by orgcod, unidsc ";
	?>
	var lista_uo = new Array();
	<? $ultimo_cod = null; ?>
	<? foreach ( $db->carregar( $sql ) as $unidade ) :?>
		<? if ( $ultimo_cod != $unidade['orgcod'] ) : ?>
			lista_uo['<?= $unidade['orgcod'] ?>'] = new Array();
			<? $ultimo_cod = $unidade['orgcod']; ?>
		<? endif; ?>
		lista_uo['<?= $unidade['orgcod'] ?>'].push( new Array( '<?= $unidade['unicod'] ?>', '<?= addslashes( trim( $unidade['unidsc'] ) ) ?>' ) );
	<? endforeach; ?>
	
	<?php
		$sql = "SELECT unicod, ungcod, ungcod||' - '||ungdsc as ungdsc FROM unidadegestora WHERE ungstatus = 'A' AND unitpocod = 'U' ORDER BY unicod, ungdsc";
	?>
	var lista_ug = new Array();
	<? $ultimo_cod = null; ?>
	<? foreach ( $db->carregar( $sql ) as $unidade ) :?>
		<? if ( $ultimo_cod != $unidade['unicod'] ) : ?>
			lista_ug['<?= $unidade['unicod'] ?>'] = new Array();
			<? $ultimo_cod = $unidade['unicod']; ?>
		<? endif; ?>
		lista_ug['<?= $unidade['unicod'] ?>'].push( new Array( '<?= $unidade['ungcod'] ?>', '<?= addslashes( trim( $unidade['ungdsc'] ) ) ?>' ) );
	<? endforeach; ?>
	
	var status_geral_alterado = false;
	function alterar_status_geral(){
		var antigo = '<?= $suscod ?>';
		var novo = antigo;
		if ( document.formulario.suscod[0].checked )
		{
			novo = document.formulario.suscod[0].value;
		}
		else if ( document.formulario.suscod[1].checked )
		{
			novo = document.formulario.suscod[1].value;
		}
		else if ( document.formulario.suscod[2].checked )
		{
			novo = document.formulario.suscod[2].value;
		}
		var justificativa = document.getElementById( 'justificativa_on' );
		var vazia = document.getElementById( 'justificativa_off' );
		status_geral_alterado = antigo != novo;
		if ( status_geral_alterado ) {
			justificativa.style.display = 'block';
			vazia.style.display = 'none';
			
		} else {
			justificativa.style.display = 'none';
			vazia.style.display = 'block';
		}
	}
	
	var antigo = new Array();
	var status_sistema_alterado = new Array();
	<?php foreach( $sistemas as $sistema ): ?>
		antigo['<?= $sistema->sisid ?>'] = '<?= $sistema->usuariosistema->suscod ?>';
		status_sistema_alterado[<?= $sistema->sisid ?>] = false;
	<?php endforeach; ?>

	function alterar_status_sistema( sisid ){
		var ativo = document.getElementById( 'status_ativo_' + sisid );
		if ( ativo.checked ) {
			novo = ativo.value;
		}
		var pendente = document.getElementById( 'status_pendente_' + sisid );
		if ( pendente.checked ) {
			novo = pendente.value;
		}
		var bloqueado = document.getElementById( 'status_bloqueado_' + sisid );
		if ( bloqueado.checked ) {
			novo = bloqueado.value;
		}
		var justificativa = document.getElementById( 'justificativa_on_' + sisid );
		var vazia = document.getElementById( 'justificativa_off_' + sisid );
		status_sistema_alterado[sisid] = antigo[sisid] != novo;
		if ( status_sistema_alterado[sisid] ) {
			justificativa.style.display = 'block';
			vazia.style.display = 'none';
		} else {
			justificativa.style.display = 'none';
			vazia.style.display = 'block';
		}
	}
	
	function enviar_formulario(){
		if ( validar_formulario() ) {
			prepara_formulario();
			document.formulario.submit();
		}
	}

	var validar_uo = false;
	function listar_unidades_orcamentarias( orgcod )
	{
		var campo_select = document.getElementById( 'unicod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );
		
		var div_on = document.getElementById( 'unicod_on' );
		var div_off = document.getElementById( 'unicod_off' );
		if ( !lista_uo[orgcod] )
		{
			validar_uo = false;
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			listar_unidades_gestoras( '' );
			return;
		}
		validar_uo = true;
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		var j = lista_uo[orgcod].length;
		for ( var i = 0; i < j; i++ )
		{
			campo_select.options[campo_select.options.length] = new Option( lista_uo[orgcod][i][1], lista_uo[orgcod][i][0], false, lista_uo[orgcod][i][0] == '<?= $unicod ?>' );
		}
		if ( navigator.appName == 'Microsoft Internet Explorer' ) {
			for ( i=0; i< campo_select.length; i++ )
			{
				if ( campo_select.options(i).value == '<?= $unicod ?>' ) {
					campo_select.options(i).selected = true;
				}
			}
		}
	}

	var validar_ug = false;
	function listar_unidades_gestoras( unicod )
	{
		var campo_select = document.getElementById( 'ungcod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );
		var div_on = document.getElementById( 'ungcod_on' );
		var div_off = document.getElementById( 'ungcod_off' );
		if ( !lista_ug[unicod] )
		{
			validar_ug = false;
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			return;
		}
		validar_ug = true;
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		var j = lista_ug[unicod].length;
		for ( var i = 0; i < j; i++ )
		{
			campo_select.options[campo_select.options.length] = new Option( lista_ug[unicod][i][1], lista_ug[unicod][i][0], false, lista_ug[unicod][i][0] == '<?= $ungcod ?>' );
		}
		if ( navigator.appName == 'Microsoft Internet Explorer' ) {
			for ( i=0; i < campo_select.length; i++ )
			{
				if ( campo_select.options(i).value == '<?= $ungcod ?>' ) {
					campo_select.options(i).selected = true;
				}
			}
		}
	}

	function validar_formulario(){
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos:';
		if ( document.formulario.usunome.value == '' ) {
			mensagem += '\nNome';
			validacao = false;
		}
		if ( !validar_radio( document.formulario.ususexo, 'Sexo' ) ) {
			mensagem += '\nSexo';
			validacao = false;
		}
		if ( document.formulario.regcod.value == '' ) {
			mensagem += '\nUnidade Federal';
			validacao = false;
		}
		if ( document.formulario.orgcod.value == '' ) {
			mensagem += '\nÓrgão';
			validacao = false;
		} else if ( validar_uo == true && document.formulario.unicod.value == '' ) {
			mensagem += '\nUnidade Orçamentária';
			validacao = false;
		} else if ( validar_ug == true && document.formulario.ungcod.value == '' ) {
				mensagem += '\nUnidade Gestora';
				validacao = false;
		}
		if ( document.formulario.usufoneddd.value == '' || document.formulario.usufonenum.value == '' ) {
			mensagem += '\nTelefone';
			validacao = false;
		}
		if ( !validaEmail( document.formulario.usuemail.value ) ) {
			mensagem += '\nE-mail';
			validacao = false;
		}
		if ( document.formulario.usufuncao.value == '' ) {
			mensagem += '\nFunção';
			validacao = false;
		}
		
		// verifica a alteração de status
		status = true;
		var ativo_geral = document.getElementById( 'status_ativo' );
		if ( status_geral_alterado && document.formulario.htudsc.value == '' && ativo_geral.checked == false ) {
			status = false;
		}
		/*
		for ( var sisid in status_sistema_alterado ) {
			if( typeof( status_sistema_alterado[sisid] ) == 'boolean' ) {
				var ativo = document.getElementById( 'status_ativo_' + sisid );
				if ( status_sistema_alterado[sisid] == true && document.formulario.elements['justificativa['+ sisid +']'].value == '' && ativo.checked == false ) {
					status = false;
				}
			}
		}
		*/
		if ( !status ) {
			mensagem += '\nJustificativa';
			validacao = false;
		}
		
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}

	function voltar(){
		window.location.href = '?modulo=sistema/usuario/consusuario&acao=<?= $_REQUEST['acao'] ?>';
	}

	<?php if ( $orgcod ): ?>
		listar_unidades_orcamentarias( <?= $orgcod ?> );
	<?php endif; ?>

	<?php if (  $orgcod == CODIGO_ORGAO_SISTEMA AND $unicod == '26101' ): ?>
		listar_unidades_gestoras( <?= $unicod ?> );
	<?php endif; ?>

	function exibir_ocultar_historico( id ){
		div = document.getElementById( id );
		if ( div.style.display == 'none' ) {
			div.style.display = 'block';
		} else {
			div.style.display = 'none'
		}
	}

</script>