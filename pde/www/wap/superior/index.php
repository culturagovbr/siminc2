<?php
// carrega as fun��es gerais
include_once "config.inc";
include_once "../../_constantes.php";
include ("../../../../includes/funcoes.inc");
include ("../../../../includes/classes_simec.inc");

// abre conex�o com o servidor de banco de dados
$db = new cls_banco();

?>
<!DOCTYPE html> 
<html> 
	<head> 
	<title><?php echo SIGLA_SISTEMA; ?> - Mobile</title> 
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<link rel="stylesheet" href="/includes/mobile-simec/SIMEC.min.css" />
	<link rel="stylesheet" href="http://code.jquery.com/mobile/1.3.0-rc.1/jquery.mobile.structure-1.3.0-rc.1.min.css" /> 
	<script src="http://code.jquery.com/jquery-1.8.3.min.js"></script> 
	<script src="http://code.jquery.com/mobile/1.3.0-rc.1/jquery.mobile-1.3.0-rc.1.min.js"></script> 
</head> 
<body >
<?php 
	include APPRAIZ."includes/classes/Mobile.class.inc";
//	include APPRAIZ."/pde/www/_funcoes_mobile.php";
//	$mobile = new Mobile();
?>
<form name="formulario_mobile" method="post">
<div data-theme="a" data-role="page">
	<div data-role="header" data-position="fixed" data-theme="a">
		<div data-role="controlgroup" data-type="horizontal">
			<a data-transition="slidedown" href="../" data-role="button" data-icon="home" class="inicio-rodape">In�cio</a>
		</div>
		<h1>Educa��o Superior</h1>
		<div data-role="navbar">
			<ul>
				<li><a data-theme="a" href="" data-transition="slide" >Expans�o da Educa��o Superior </a></li>
			</ul>
		</div>
	</div>
	<div data-role="content">
	
	 <ul data-role="listview" data-inset="true">
				<li>
					<a data-transition="flip" href="superior_Vistoria.php">Vistorias</a>
				 </li>
				<li>
					<a data-transition="flip" href="superior_Situacao.php">Situa��o das Obras</a>
				 </li>
				 </ul>
	</div>
			 

</div>
</body>
</html>


