<?php
	if (isset($_POST['submit'])) {
		require_once("../connector.php");
		require("../debug.php");
		header("Content-Type: text/html; charset=utf-8");
		ob_start();
		
		try {
			msgOutput("TESTE DE USUARIO - BUSCAR INFORMACOES DE PERMISSOES DO SISTEMA");
			$SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
			
			msgOutput("Conectando...");
			if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
				$SSDWs->useProductionSSDServices();
				msgOutput("Servidor de PRODUCAO conectado. WSDL baixada.");
			} else {
				$SSDWs->useHomologationSSDServices();
				msgOutput("Servidor de homologacao conectado. WSDL baixada.");
			}
			
			msgOutput("buscando informacoes de permissao");
			
			//$userId = (integer) $_POST['userId'];
			
			//$permissionId = $_POST['permissionId'];
			
			$resposta = $SSDWs->getSystemPermissionsInfo();
			msgOutput("Informacoes retornadas");
			
			/*
			echo "<pre>";
			var_dump($resposta);
			echo "</pre>";
			*/		
			
		} catch (Exception $e) {
			$erro = $e->getMessage();
			echo $erro;
			exit();
		}
?>

	<h3>TESTE DE USUARIO - BUSCAR INFORMACOES DE PERMISSOES DO SISTEMA</h3>
	
	<?php foreach ( $resposta as $eachUserPermissionId ): ?>
	
	<ul>
		<li><b>Id da permiss&atilde;o: </b><?php $eachUserPermissionId-> getId() ?> </b></li>
		<li><b>Tipo de pessoa: </b><?php echo $eachUserPermissionId->getPersonType() ?></li>
		<li><b>Status: </b><?php echo $eachUserPermissionId->getRegisterStatus() ?></li>
		<li><b>Justificativa de altera&ccedil;&atilde;o: </b><?php echo $eachUserPermissionId->getChangeJustify() ?></li>
		<li><b>Usu&aacute;rio que efetuou a altera&ccedil;&atilde;o: </b><?php echo $eachUserPermissionId->getDefaultPermission() ?></li>
		
		<li><b>Descri&ccedil;&atilde;o do perfil: </b><?php echo $eachUserPermissionId->getDescription() ?></li>
		
		<li><b>Sigla do perfil: </b><?php echo $eachUserPermissionId->getSgProfile() ?></li>
	</ul>
	
	<?php endforeach; ?>

<a href="javascript:history.back()">Voltar</a> | 
<?
	} else {
?>
	<h3>TESTE DE USUARIO - BUSCAR INFORMACOES DE PERMISSOES DO SISTEMAS</h3>
	<form method="POST">
		<!-- <label>ID do Usu&aacute;rio:</label> <input type="text" name="userId" value="1404"/><br /> -->
		<!-- <label>ID da Permiss&atilde;o:</label> <input type="text" name="permissionId" value="15"/><br /> -->
		<input type="submit" value="Consultar" name="submit">
	</form>
<?php
	}
?>
<a href="../index.php">Menu Principal</a>