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
	include APPRAIZ."/pde/www/_funcoes_mobile.php";
//	$mobile = new Mobile();
?>
<form name="formulario_mobile" method="post">
<div data-theme="a" data-role="page">
	<div data-role="header" data-position="fixed" data-theme="a">
		<div data-role="controlgroup" data-type="horizontal">
			<a data-transition="slidedown" href="../" data-role="button" data-icon="home" class="inicio-rodape">In�cio</a>
			<a href="../profissional" data-role="button" data-icon="arrow-r" data-ajax="false">Educa��o Profissional</a>
		</div>
		<h1>Expans�o da Rede Federal de EPT</h1>
		<div data-role="navbar">
			<ul>
				<li><a class="ui-btn-active" data-theme="a" href="expansao_ept.php"  data-transition="slide">Expans�o da Rede Federal de EPT </a></li>
			</ul>
		</div>
	</div>
	<div data-role="content">
	
	 <ul data-role="listview" data-inset="true">
				<li>
					<a data-transition="flip" href="expansao_ept_Vistoria.php">Vistorias</a>
				 </li>
				 <tr>
  <!-- Tabela Mapa -->
				 <td class="fundo_td" >
                	<div>
                		<img style="float:left" src="../../../imagens/icones/icons/cone.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" ><b><br>Vistoria<br/><span class="subtitulo_box" >Situa��o quanto ao n�vel de preenchimento</span></b></div>
                	</div>
                	<?
					$sql = "select nivelpreenchimento, sum(contador) as total, corpreenchimento from (
								SELECT	CASE WHEN stoid IN (1, 2) THEN
									(CASE	WHEN DATE_PART('days', NOW() - obrdtvistoria) < 45 THEN '1 - Obras atualizadas h� menos de 45 dias atr�s'
										WHEN DATE_PART('days', NOW() - obrdtvistoria) BETWEEN 45 AND 60	THEN '2 - Obras atualizadas entre 45 e 60 dias'
									ELSE '3 - Obras atualizadas h� mais de 60 dias'
									END)
									WHEN stoid = 3 THEN '4 - Obras conclu�das'
									ELSE '5 - N�o se aplica' END as nivelpreenchimento,
									CASE WHEN stoid IN (1, 2) THEN
									(CASE WHEN DATE_PART('days', NOW() - obrdtvistoria) < 45 THEN '#80BC44'
									WHEN DATE_PART('days', NOW() - obrdtvistoria) BETWEEN 45 AND 60 THEN '#FFC211'
									ELSE '#E95646'
									END)
									WHEN stoid = 3 THEN '#2B86EE'
								ELSE '#888888' END as corpreenchimento,
								1 AS CONTADOR
								FROM obras.obrainfraestrutura  
								where obsstatus = 'A'
								and stoid not in (11) 
								and orgid = 2  
								AND prfid = 34 --EXPANS�O RFEPT
							) as foo
							group by nivelpreenchimento, corpreenchimento 
							order by 1";
					$vistoria = $db->carregar($sql,null,3200);
                	?>
                	<div style="clear:both;" >
                			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
		                		<tr>
		                			<td class="center" ><b>Preenchimento</b></td>		                			
		                			<td class="center" ><b>Total</b></td>
		                		</tr>
                			<? if($vistoria[0]) : ?>
                			<? foreach($vistoria as $vis) : ?>
		                		<tr>
		                			<td style="background-color:<?=$vis['corpreenchimento'] ?>;"><?=str_replace(array("1 - ","2 - ","3 - ","4 - ","5 - "),"",$vis['nivelpreenchimento'])?></td>
		                			<td style="background-color:<?=$vis['corpreenchimento'] ?>;" class="numero"><?=$vis['total'] ?></td>
		                		</tr>
		                		<?
								$tottotal += $vis['total'];
		                		?>
                			<? endforeach; ?>
                			<? endif; ?>
		                	<tr>
								<td class="bold"><b>Total</b></td>
								<td class="numero"><b><?php echo number_format($tottotal,0,"",".") ?></b></td>
	                		</tr>
		                	</table>
                		</div>
                </td>
                <!-- Fim Tabela Mapa -->
</tr>
                </tr>
                <li>
					<a data-transition="flip" href="expansao_ept_Situacao.php">Situa��o das Obras</a>
				 </li>
		</ul>
	
	
	</div>
</body>
</html>