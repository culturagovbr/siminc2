<?php
include("fies.php");


?>
<html>
<head>
	<title>FIES - Financiamento Estudantil</title>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
</head>
<body>
<?monta_titulo('Simulador Fies', ''); ?>
<form action="result.php" method="POST">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td colspan="2" style="border-bottom: 1px solid #cccccc;" >
		<center>
			<div class="textoAzul2" align="left" style="font-weight: normal;">
			Este simulador possui um car�ter meramente ilustrativo e visa possibilitar ao estudante interessado no Financiamento Estudantil informa��es aproximadas sobre a sua d�vida futura, bem como o montante de recursos que dever� despender mensalmente para quit�-la. Lembramos que vari�veis como, data da presta��o, valor da semestralidade, data da assinatura do contrato, data da realiza��o dos aditamentos, trazem varia��es nos valores simulados.<br><br>
			Para a informa��o dos valores da taxa de juros, dever� ser indicado no campo correspondente o valor praticado no seu contrato levando em conta as diferentes taxas. Para os contratos firmados at� 2005 inclusive, a taxa de juros � de 9% ao ano.Para os contratos firmados a partir do segundo semestre de 2006 a taxa de juros � de 6,5% ao ano ou 3,5% ao ano exclusivamente para os cursos de licenciatura, pedagogia, normal superior e cursos constantes do Cat�logo de Cursos Superiores de Tecnologia.<br><br> 
			Os valores das presta��es foram calculados considerando os dados informados pelo estudante.
			</div>
		</td>
	</tr>
	<tr>
		<th colspan="2" class="SubTituloCentro" style="background: #DCDCDC;">Formul�rio para Simula��o</th>
	</tr>
	<tr>
		<td class="SubTituloDireita">Processo Seletivo:</td>
		<td width="60%">
			<?php
				$comboArr = array(	
									array(
										"codigo" 	=> "2� semestre de 1999",
										"descricao" => "2� semestre de 1999"
									)
								  );
				for ($i=2000; $i <= 2010; $i++){
					array_push($comboArr, array(
													"codigo"    => "1� semestre de {$i}",
													"descricao" => "1� semestre de {$i}"
												));
												
					array_push($comboArr, array(
													"codigo"    => "2� semestre de {$i}",
													"descricao" => "2� semestre de {$i}"
												));
					
				}
								  
				$db->monta_combo("processoseletivo", $comboArr, 'S', "", '', '', '', '', 'N', '');
		    ?>		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Quantidade de Semestres do Curso:</td>
		<td>
			<?= campo_texto( 'qtdsemcurso', 'N', 'S', '', 8, 10, '############', '', 'left', '', 0, ''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Quantidade de Semestres j� Conclu�dos:</td>
		<td>
			<?= campo_texto( 'qtdsemconc', 'N', 'S', '', 8, 10, '############', '', 'left', '', 0, ''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Percentual de Financiamento:</td>
		<td>
			<?php
				$comboArr = array();
				for ($i=10; $i <= 100; $i++){
					array_push($comboArr, array(
													"codigo"    => "{$i}%",
													"descricao" => "{$i}%"
												));
				}
								  
				$db->monta_combo("percentfinanc", $comboArr, 'S', "", '', '', '', '', 'N', '');
		    ?>		
		</td>
	</tr>
	<tr>
	<tr>
		<td class="SubTituloDireita">Taxa de juros a.a.:<br/><font size="1">(escolher a taxa de juros conforme seu contrato nos valores abaixo)</font></td>
		<td>
			<?php
				$comboArr = array(	
									array(
										"codigo" 	=> "3,5%",
										"descricao" => "3,5%"
									),
									array(
										"codigo" 	=> "6,5%",
										"descricao" => "6,5%"
									),
									array(
										"codigo" 	=> "9%",
										"descricao" => "9%"
									),
								  );
								  
				$db->monta_combo("taxajuros", $comboArr, 'S', "--", '', '', '', '', 'N', '');
		    ?>		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo de estudante:</td>
		<td>
			<?php
				$comboArr = array(	
									array(
										"codigo" 	=> "Bolsista Prouni 50%",
										"descricao" => "Bolsista Prouni 50%"
									),
									array(
										"codigo" 	=> "Bolsista Complementar 25%",
										"descricao" => "Bolsista Complementar 25%"
									),
									array(
										"codigo" 	=> "N�o Bolsista",
										"descricao" => "N�o Bolsista"
									),
								  );
								  
				$db->monta_combo("tipoestudante", $comboArr, 'S', "--", '', '', '', '', 'N', '');
		    ?>		
		</td>
	</tr>
	<tr>
	<tr>
		<td class="SubTituloDireita">Valor da Mensalidade:<br/><font size="1">(Informar o valor da sua mensalidade deduzidos todos os descontos oferecidos pela institui��o de ensino, inclusive os concedidos em virtude de pagamento pontual)</font></td>
		<td>
			<?= campo_texto( 'valmens', 'N', 'S', '', 8, 10, '#.###.###.###,##', '', 'left', '', 0, ''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Prazo de Car�ncia:<br><font size="1">(em meses)</font> </td>
		<td>
			<? $carencia = '06'; ?>	
			<?= campo_texto( 'carencia', 'N', 'N', '', 3, 10, '', '', 'left', '', 0, ''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data da assinatura do contrato:</td>
		<td>
		<?= campo_data2( 'dtasscontrato', 'S', 'S', 'Data da assinatura do contrato', 'S' ); ?>		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Escolha o melhor dia para Vencimento:</td>
		<td>
			<?php
				$comboArr = array(	
									array(
										"codigo" 	=> "05",
										"descricao" => "05"
									),
									array(
										"codigo" 	=> "10",
										"descricao" => "10"
									),
									array(
										"codigo" 	=> "15",
										"descricao" => "15"
									),
									array(
										"codigo" 	=> "20",
										"descricao" => "20"
									),
									array(
										"codigo" 	=> "25",
										"descricao" => "25"
									)
								  );
								  
				$db->monta_combo("diavenc", $comboArr, 'S', "", '', '', '', '', 'N', '');
		    ?>		
		</td>
	</tr>
	<tr bgcolor="#CCCCCC">
	   <td>&nbsp;</td>
	   <td>
	    	<input type="submit" name="btalterar" value="Continuar" onclick="" class="botao">
	    	&nbsp;&nbsp;&nbsp;&nbsp;
	    	<input type="reset" name="btcancelar" value="Limpar" class="botao">
	   </td>
	</tr>      
	<tr>
		<td colspan="2" style="border-top: 1px solid #cccccc;">
		<center>
			<div class="textoAzul2" align="left" style="font-weight: normal;">
Pela lei 11.552/2007, publicada no DOU em 19/11/2007, os contratos de FIES, assinados a partir de 2008, passam a contar com prazo de car�ncia de 6 meses a partir do t�rmino do prazo de utiliza��o bem como o prazo de amortiza��o II passa de at� 1,5 e meia o prazo de utiliza��o para at� 2 vezes o prazo de utiliza��o.<br>
Dessa forma um contrato FIES passa a ter 4 fases distintas:<br><br>

1 - PRAZO DE UTILIZA��O : Prazo contado a partir do primeiro m�s de ingresso no FIES at� o �ltimo m�s do prazo de utiliza��o (considera-se os semestres suspensos ou encerrados sem in�cio de amortiza��o). Nessa fase o estudante paga presta��es de juros a cada 3(tr�s) meses de at� 50,00, nos meses MAR�O, JUNHO, SETEMBRO e DEZEMBRO.<br>
2 - PRAZO DE CAR�NCIA : Fixo em 6 meses imediatamente subseq�entes ao PRAZO DE UTILIZA��O. O prazo de car�ncia � opcional por�m o estudante deve se manifestar caso n�o o queira. Nessa fase as presta��es t�m a mesma regra do PRAZO DE UTILIZA��O.<br>
3 - PRAZO DE AMORTIZA��O I : Fixo em 12 meses imediatamente subseq�entes ao PRAZO DE CAR�NCIA. Nessa fase o estudante paga presta��es mensais cujo valor � exatamente o mesmo repassado mensalmente � IES� INSTITUI��O DE ENSINO SUPERIOR em fun��o do �ltimo aditamento do aluno.<br>
4 - PRAZO DE AMORTIZA��O II : At� 2 vezes o PRAZO DE UTILIZA��O(o simulador adota exatamente 2(duas) vezes). Nessa fase o sistema calcula a presta��o PRICE em fun��o do saldo devedor do contrato no dia da mudan�a para essa fase, em fun��o ainda da taxa de juros e do prazo dessa fase.<br><br>

Durante as fases PRAZO DE UTILIZA��O, PRAZO DE CAR�NCIA e PRAZO DE AMORTIZA��O I, os juros excedentes ao valor da presta��o calculado s�o incorporados ao saldo devedor do contrato no m�s da sua apura��o(c�lculo). 			</div>
		</center>
		</td>
	</tr>
</table>
</form>
</body>
</html>