<?php

	function erro(){
		global $db;
		$db->commit();
		$_SESSION = array();
		$_SESSION['MSG_AVISO'] = func_get_args();
		header( "Location: ". $_SERVER['PHP_SELF'] );
		exit();
	}

	// carrega as bibliotecas internas do sistema
	include "config.inc";
	require APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";

	// abre conex�o com o servidor de banco de dados
	$db = new cls_banco();

	// executa a rotina de recupera��o de senha quando o formul�rio for submetido
	if ( $_POST['formulario'] ) {
		
		include_once("connector.php");
		$SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
			
		if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
			$SSDWs->useProductionSSDServices();
		} else {
			$SSDWs->useHomologationSSDServices();
		}

		$cpfOrCnpj = str_replace(array(".","-"),array("",""), $_POST['usucpf']);
		$respostaDadosUsu = $SSDWs->getUserInfoByCPFOrCNPJ($cpfOrCnpj);
		if($respostaDadosUsu instanceof stdClass) {
			$resposta = $SSDWs->recoveryUserPasswordByCPFOrCNPJ($cpfOrCnpj);
			/*
			 * Testando se o retorno � um n�mero, caso sim, mostra o n�mero de ativa��o
			 * caso n�o, mostra o erro
			 */
			if(is_numeric($resposta)) {
				// Mostrar o codigo de ativa��o
				session_unset();
				$_SESSION['MSG_AVISO'] = "Em breve voc� receber� uma mensagem por e-mail(".$respostaDadosUsu->email.") contendo um link de ativa��o da conta. Abra o link � preencha com seu CPF e o C�DIGO DE ATIVA��O : ".$resposta;
			} else {
				session_unset();
				$_SESSION['MSG_AVISO'] = $resposta['erro'];
			}
			header('location: login.php');
    		exit;
			
		} else {
			// Efetuando logout
			session_unset();
			$_SESSION['MSG_AVISO'] = $respostaDadosUsu['erro'];
			header('location: login.php');
    		exit;
		}
	}
?>
<html>
	<head>
		<title>Simec - Minist�rio da Educa��o</title>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<style type=text/css>
			form {
				margin: 0px;
			}
		</style>
	</head>
	<body bgcolor=#ffffff vlink=#666666 bottommargin="0" topmargin="0" marginheight="0" marginwidth="0" rightmargin="0" leftmargin="0">
		<?php include "cabecalho.php"; ?>
		<br/>
		<?php
			$mensagens = '<p style="align: center; color: red; font-size: 12px">'. implode( '<br/>', (array) $_SESSION['MSG_AVISO'] ) . '</p>';
			$_SESSION['MSG_AVISO'] = null;
			$titulo_modulo = 'Recupera��o de Senha';
			$subtitulo_modulo = 'Digite seu CPF e pressione o bot�o "Lembrar Senha".<br/>O Sistema enviar� um e-mail para voc� contendo uma nova senha de acesso.<br/>'. obrigatorio() .' Indica Campo Obrigat�rio.'. $mensagens;
			monta_titulo( $titulo_modulo, $subtitulo_modulo );
		?>
		<form method="POST" name="formulario">
			<input type=hidden name="formulario" value="1"/>
			<input type=hidden name="modulo" value="./inclusao_usuario.php"/>
			<table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;">
				<tr bgcolor="#F2F2F2">
					<td align = 'right' class="subtitulodireita" width="150px">CPF:</td>
					<td>
						<input type="text" name="usucpf" value="" size="20" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" class="normal" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);">
						<?= obrigatorio(); ?>
					</td>
			 	</tr>
				<tr bgcolor="#C0C0C0">
					<td>&nbsp;</td>
					<td>
						<input type="button" name="btinserir" value="Lembrar Senha" onclick="enviar_formulario()"/>
						&nbsp;&nbsp;&nbsp;
						<input type="Button" value="Voltar" onclick="location.href='./login.php'"/>
					</td>
				</tr>
			</table>
		</form>
		<br/>
		<?php include "./rodape.php"; ?>
	</body>
</html>
<script language="javascript">

	document.formulario.usucpf.focus();

	function enviar_formulario() {
		if ( validar_formulario() ) {
			document.formulario.submit();
		}
	}

	function validar_formulario() {
		var validacao = true;
		var mensagem = '';
		if ( !validar_cpf( document.formulario.usucpf.value ) ) {
			mensagem += '\nO cpf informado n�o � v�lido.';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}

</script>