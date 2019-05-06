<html>
<head>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
</head>
<body>
<?php
extract($_GET);

$sql = "SELECT 
	   --aca.acaid,
       coalesce(cast(SUM(dt.sadvalor) as varchar),'0.00') as detalhamento,
      -- dt.sbaid,
       su.sbatitulo
  FROM planejamento.acao aca
    INNER JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
    INNER JOIN monitora.pi_subacaodotacao dt ON dtl.ptrid = dt.ptrid
    INNER JOIN monitora.pi_subacao su ON dt.sbaid = su.sbaid
  WHERE aca.prgano = '{$_SESSION['exercicio']}' 
    --AND aca.acaid = '{$acaid}' 
  GROUP BY aca.acaid, dt.sadvalor, dt.sbaid, su.sbatitulo";


$cabecalho = array ("Detalhamento", "Descrição");
//$db->monta_lista($sql,$cabecalho,60,20,'','','');

$titulo = utf8_encode("Subações Vinculados com a ação");
?>
<br/>
<table border="0">
	<tr>
		<td colspan="2"><b><?php echo $titulo;?></b></td>
	</tr>
	<tr>
		<td colspan="2">_______________________________</td>
	</tr>
	<tr>
		<td><b>Detalhamento</b></td>
		<td><b><?php echo utf8_encode('Descrição');?></b></td>
	</tr>
<?php 
$arDados = $db->carregar($sql);
if ( is_array($arDados) && !empty($arDados) ){
	foreach($arDados as $val){				
		echo "<tr>
				<td><b>{$val['detalhamento']}</b></td>
				<td>".utf8_encode($val['sbatitulo'])."</td>		
			  </tr>";
	}
}

?>
</table>
</body>
</html>
<div id="dibDegug"></div>