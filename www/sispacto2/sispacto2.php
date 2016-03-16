<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

// carrega as fun��es espec�ficas do m�dulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';


//if((date("w") == 1 || date("w") == 3 || date("w") == 5) && date("H:i") > "01:30" && date("H:i") < "05:00") {

if(date("H:i") > "01:30" && date("H:i") < "05:00") {
	
	include  APPRAIZ."includes/cabecalho.inc";

	echo "<br>";

	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td style="font-size:x-large;" align="center"><img src="../imagens/alerta_sistema.gif" border="0" align="absmiddle"> O SISPACTO 2014 em Manuten��o</td>
	</tr>
	<tr>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td style="font-size:large;" align="center">A manuten��o no SISPACTO 2014 deixar� indispon�vel o sistema nos dias da semana : Segunda, Quarta e Sexta (01:30 �s 05:00)</td>
	</tr>
	</table>
	<?

	include APPRAIZ . "includes/rodape.inc";

} else {


	if ($db->testa_superuser()) {
		$painelCabecalho = array(
				array('titulo' => 'Pagamento de bolsas', 'funcao' => 'montarGraficoPagamentos', 'icon' => 'tasks'),
				array('titulo' => 'Perfil das turmas dos alfabetizadores', 'funcao' => 'montarGraficoPerfilTurmas', 'icon' => 'tasks')
		);
	}

	//Carrega as fun��es de controle de acesso
	include_once "controleAcesso.inc";

}
?>