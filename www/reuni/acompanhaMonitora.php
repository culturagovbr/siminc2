<?php

header( 'Content-type: text/html; charset=iso-8859-1' );
// captura entrada
$vl0     = $_REQUEST['vl0'];
$vl1     = $_REQUEST['vl1'];
$vl2     = $_REQUEST['vl2'];
$vl3     = $_REQUEST['vl3'];
$vltotal = $_REQUEST['vltotal'];
$naplicaveis = $_REQUEST['na'];

$strTitle = 'Monitoradas(s) / Preenchida(s)';

if($vltotal == 0){
	
	$vl0p = 100;
	$vl0p = 0;
	$vl0p = 0;
	$vl0p = 0;	
}else{
	$vl0p = number_format( $vl0 * 100 / $vltotal, 0, '.', ',' );
	$vl1p = number_format( $vl1 * 100 / $vltotal, 0, '.', ',' );
	$vl2p = number_format( $vl2 * 100 / $vltotal, 0, '.', ',' );
	$vl3p = number_format( $vl3 * 100 / $vltotal, 0, '.', ',' );	
}

?>
<b><?= $naplicaveis ?> - N�o Aplic�veis</b><br/>
<b><?= $vltotal ?> - Aplicaveis</b>
<br/>
<table border="0">
	<tr>
		<td>&nbsp;&nbsp;&nbsp;&nbsp;</td>
		<td><?= $vl1 ?></td>		
		<td>
			<img src="../../imagens/cor1.gif" style="height:7;width:<?= $vl1p ?>;border:1px solid #888888;"> Est�vel (<?= $vl1p ?>%)
		</td>
	</tr>
	<tr>
	<td>&nbsp;&nbsp;&nbsp;&nbsp;</td>
		<td><?= $vl2 ?></td>
		<td>
			<img src="../../imagens/cor2.gif" style="height:7;width:<?= $vl2p ?>;border:1px solid #888888;"> Merece Aten��o (<?= $vl2p ?>%)
		</td>
	</tr>
	<tr>
	<td>&nbsp;&nbsp;&nbsp;&nbsp;</td>
		<td><?= $vl3 ?></td>
		<td>
			<img src="../../imagens/cor3.gif" style="height:7;width:<?= $vl3p ?>;border:1px solid #888888;"> Cr�tico (<?= $vl3p ?>%)
		</td>
	</tr>
	<tr>
	<td>&nbsp;&nbsp;&nbsp;&nbsp;</td>
		<td><?= $vl0 ?></td>
		<td>
			<img src="../../imagens/cor0.gif" style="height:7;width:<?= $vl0p ?>;border:1px solid #888888;"> N�o Avaliada (<?= $vl0p ?>%)
		</td>
	</tr>
</table>