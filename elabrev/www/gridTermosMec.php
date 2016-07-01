<?php

    header("HTTP/1.1 303 See Other");
    header("Location: http://{$_SERVER['HTTP_HOST']}/ted/termo-de-execucao-descentralizada.php");
    die;

    if (IS_LOCAL) {
        $_REQUEST['baselogin'] = "simec_espelho_producao";
    } else {
        $_REQUEST['baselogin'] = "simec";
    }

    require_once "config.inc";

    require_once APPRAIZ . "includes/funcoes.inc";
    require_once APPRAIZ . "includes/classes_simec.inc";

    require_once APPRAIZ . 'elabrev/www/_constantes.php';
    require_once "_funcoes_termoCooperacao.php";

    $_SESSION['sisid'] = 2; # seleciona o sistema de seguran�a
    $_SESSION['usucpf'] = '';
    $_SESSION['usucpforigem'] = '';

    $db = new cls_banco();

    function gerarPDFTermoCoopercao($html) {
        ob_clean();

        $content = http_build_query(array('conteudoHtml' => utf8_encode($html)));

        $context = stream_context_create(array('http' => array('method' => 'POST', 'content' => $content)));

        $contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context);

        header('Content-Type: application/pdf');
        header("Content-Disposition: attachment; filename=termo_cooperacao.pdf");
        echo $contents;
        exit;
    }

    #GERAR O TERMO DE COOPERA��O EM PDF.
    if ($_REQUEST['gerarPDF'] == 'S') {

        $tcpid = $_REQUEST['tcpid'];

        $dadosUG = buscarDadosUndProp($tcpid);
        $dadosObjeto = buscarObjetoTermo($tcpid);
        
        $sqlCountSolAteracao = " select count(*) from workflow.historicodocumento where aedid = ".WF_ACAO_SOL_ALTERACAO." and docid = (select docid from monitora.termocooperacao where tcpid = {$tcpid}) ";
        $rsCountSolAlt = $db->pegaUm($sqlCountSolAteracao);

        $html = '
            <!-- DADOS DO �RG�O OU ENTIDADE PROPONENTE -->
			<!--
            <table id="gerapdf" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">
                <tr id="tr_titulo">
                    <td class="subtitulocentro" >Termo de coopera��o - Gerar PDF</td>
                </tr>
            </table>
        	-->
        	';
        
        $html .= '
				<table id="gerapdf" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">
					<tr id="tr_titulo">
						<td class="subtituloDireita" width="100">N� do Termo:</td>
						<td>&nbsp;'.($rsCountSolAlt>0 ? $tcpid.'.'.$rsCountSolAlt : $tcpid).'</td>
			';
        
        if ( $dadosObjeto['ungcodconcedente'] == UG_FNDE && in_array($dadosObjeto['ungcodpoliticafnde'], array(UG_SECADI, UG_SETEC, UG_SEB)) ){
        	$html .= '
						<td class="subtituloDireita" width="100">N� do Processo:</td>
						<td width="200">&nbsp;'.$dadosObjeto['tcpnumprocessofnde'].'</td>
					';
        }
        
        $html .= '
					</tr>
				</table>
				';

            $html .= '
            <table id="dados_entidade_prop" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">
                <tr>
                    <td height="30" colspan="5" style="background-color: #CFCFCF; text-align:center; font-weight: bold;">DADOS DO �RG�O OU ENTIDADE PROPONENTE</td>
                </tr>
                <tr>
                    <td width="180" style="font-weight: bold;">1. C�d. Und. Gestora</td>
                    <td colspan="2" style="font-weight: bold;">2. C�d. da Gest�o</td>
                    <td colspan="2" style="font-weight: bold;">3. Raz�o Social</td>
                </tr>
                <tr>
                    <td>' . $dadosUG['ungcod'] . '</td>
                    <td colspan="2">' . $dadosUG['gescod'] . '</td>
                    <td colspan="2">' . $dadosUG['ungdsc'] . '</td>
                </tr>
                <tr>
                    <td height="21" colspan="2" style="font-weight: bold;">4. Endere�o</td>
                    <td colspan="2" style="font-weight: bold;">5. Bairro ou Distrito</td>
                    <td style="font-weight: bold;">6. Munic�pio</td>
                </tr>
                <tr>
                    <td colspan="2">' . $dadosUG['ungendereco'] . '</td>
                    <td colspan="2">' . $dadosUG['ungbairro'] . '</td>
                    <td>' . $dadosUG['muncod'] . '</td>
                </tr>
                <tr>
                    <td height="21" style="font-weight: bold;">7. UF</td>
                    <td style="font-weight: bold;">8. CEP</td>
                    <td colspan="2" style="font-weight: bold;">9. Telefone</td>
                    <td style="font-weight: bold;">10. E-Mail</td>
                </tr>
                <tr>
                    <td>' . $dadosUG['estuf'] . '</td>
                    <td>' . $dadosUG['ungcep'] . '</td>
                    <td colspan="2">' . $dadosUG['ungfone'] . '</td>
                    <td>' . $dadosUG['ungemail'] . '</td>
                </tr>
            </table>
        ';

        $dadosProp = recuperarResponsavelProponente($tcpid); //buscaResponsavelProp($tcpid);

        $html .= '
            <!-- REPRESENTANTE LEGAL DO �RG�O OU ENTIDADE PROPONENTE -->

            <table id="dados_proponente" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">
                <tr>
                    <td colspan="6" style="background-color: #CFCFCF; text-align:center; font-weight: bold;">REPRESENTANTE LEGAL DO �RG�O OU ENTIDADE PROPONENTE</td>
                </tr>
                <tr>
                    <td width="214" style="font-weight: bold;">11. CPF </td>
                    <td colspan="5" style="font-weight: bold;">12. Nome do Representante Legal</td>
                </tr>
                <tr>
                    <td>' . $dadosProp['usucpf'] . '</td>
                    <td colspan="5">' . $dadosProp['usunome'] . '</td>
                </tr>
                <tr>
                    <td colspan="3" style="font-weight: bold;">13. Endere�o</td>
                    <td style="font-weight: bold;">14. Bairro ou Distrito</td>
                    <td colspan="2" style="font-weight: bold;">15. Munic�pio</td>
                </tr>
                <tr>
                    <td colspan="3">' . $dadosProp['endereco'] . '</td>
                    <td>' . $dadosProp['bairro'] . '</td>
                    <td colspan="2">' . $dadosProp['municipio'] . '</td>
                </tr>
                <tr>
                    <td style="font-weight: bold;">16. UF</td>
                    <td colspan="2" style="font-weight: bold;">17. CEP</td>
                    <td style="font-weight: bold;">18. Telefone</td>
                    <td colspan="2" style="font-weight: bold;">19. E-Mail</td>
                </tr>
                <tr>
                    <td>' . $dadosProp['estado'] . '</td>
                    <td colspan="2">' . $dadosProp['endcep'] . '</td>
                    <td>' . $dadosProp['fone'] . '</td>
                    <td colspan="2">' . $dadosProp['usuemail'] . '</td>
                </tr>
                <tr>
                    <td style="font-weight: bold;">20. N&ordm; da C�dula da CI</td>
                    <td colspan="2" style="font-weight: bold;">21. �rg�o Expeditor</td>
                    <td colspan="3" style="font-weight: bold;">22. Cargo</td>
                </tr>
                <tr>
                    <td>' . $dadosProp['numeroidentidade'] . '</td>
                    <td colspan="2">' . $dadosProp['entorgaoexpedidor'] . '</td>
                    <td colspan="3">' . $dadosProp['usufuncao'] . '</td>
                </tr>
            </table>
        ';

        $dadoConc = buscarDadosUndConc($tcpid);

        $html .= ' 
            <!-- DADOS DO �RG�O OU ENTIDADE CONCEDENTE -->

            <table id="dados_entidade_conc" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">
                <tr>
                    <td height="30" colspan="5" style="background-color: #CFCFCF; text-align:center; font-weight: bold;">DADOS DO �RG�O OU ENTIDADE CONCEDENTE</td>
                </tr>
                <tr>
                    <td width="180" style="font-weight: bold;">23. C�d. Und. Gestora</td>
                    <td colspan="2" style="font-weight: bold;">24. C�d. da Gest�o</td>
                    <td colspan="2" style="font-weight: bold;">25. Raz�o Social</td>
                </tr>
                <tr>
                    <td>' . $dadoConc['ungcod'] . '</td>
                    <td colspan="2">' . $dadoConc['gescod'] . '</td>
                    <td colspan="2">' . $dadoConc['ungdsc'] . '</td>
                </tr>
                <tr>
                    <td height="21" colspan="2" style="font-weight: bold;">26. Endere�o</td>
                    <td colspan="2" style="font-weight: bold;">27. Bairro ou Distrito</td>
                    <td style="font-weight: bold;">28. Munic�pio</td>
                </tr>	
                <tr>
                    <td colspan="2">' . $dadoConc['ungendereco'] . '</td>
                    <td colspan="2">' . $dadoConc['ungbairro'] . '</td>
                    <td>' . $dadoConc['muncod'] . '</td>
                </tr>
                <tr>
                    <td height="21" style="font-weight: bold;">29. UF</td>
                    <td style="font-weight: bold;">30. CEP</td>
                    <td colspan="2" style="font-weight: bold;">31. Telefone</td>
                    <td style="font-weight: bold;">32. E-Mail</td>
                </tr>
                <tr>	
                    <td>' . $dadoConc['estuf'] . '</td>
                    <td>' . $dadoConc['ungcep'] . '</td>
                    <td colspan="2">' . $dadoConc['ungfone'] . '</td>
                    <td>' . $dadoConc['ungemail'] . '</td>
                </tr>
            </table>
        ';

        $dadosRespConced = buscaResponsavelCons($tcpid);

        $html .= '
            <!-- REPRESENTANTE LEGAL DO �RG�O OU ENTIDADE CONCEDENTE -->

            <table id="dados_concedente" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">
                <tr>
                    <td colspan="6" style="background-color: #CFCFCF; text-align:center; font-weight: bold;">REPRESENTANTE LEGAL DO �RG�O OU ENTIDADE CONCEDENTE</td>
                </tr>
                <tr>
                    <td width="214" style="font-weight: bold;">33. CPF </td>
                    <td colspan="5" style="font-weight: bold;">34. Nome do Representante Legal</td>
                </tr>
                <tr>
                    <td>' . $dadosRespConced['usucpf'] . '</td>
                    <td colspan="5">' . $dadosRespConced['usunome'] . '</td>
                </tr>
                <tr>
                    <td colspan="3" style="font-weight: bold;">35. Endere�o</td>
                    <td style="font-weight: bold;">36. Bairro ou Distrito</td>
                    <td colspan="2" style="font-weight: bold;">37. Munic�pio</td>
                </tr>
                <tr>
                    <td colspan="3">' . $dadosRespConced['endereco'] . '</td>
                    <td>' . $dadosRespConced['bairro'] . '</td>
                    <td colspan="2">' . $dadosRespConced['municipio'] . '</td>
                </tr>
                <tr>
                    <td style="font-weight: bold;">38. UF</td>
                    <td colspan="2" style="font-weight: bold;">39. CEP</td>
                    <td style="font-weight: bold;">40. Telefone</td>
                    <td colspan="2" style="font-weight: bold;">41. E-Mail</td>
                </tr>
                <tr>
                    <td>' . $dadosRespConced['estado'] . '</td>
                    <td colspan="2">' . $dadosRespConced['endcep'] . '</td>
                    <td>' . $dadosRespConced['fone'] . '</td>
                    <td colspan="2">' . $dadosRespConced['usuemail'] . '</td>
                </tr>
                <tr>
                    <td style="font-weight: bold;">42. N&ordm; da C�dula da CI</td>
                    <td colspan="2" style="font-weight: bold;">43. �rg�o Expeditor</td>
                    <td colspan="3" style="font-weight: bold;">44. Cargo</td>
                </tr>
                <tr>
                    <td>' . $dadosRespConced['numeroidentidade'] . '</td>
                    <td colspan="2">' . $dadosRespConced['entorgaoexpedidor'] . '</td>
                    <td colspan="3">' . $dadosRespConced['usufuncao'] . '</td>
                </tr>
            </table>';

        

        $html .= '
            <!-- OBJETO E JUSTIFICATIVA DA DESCENTRALIZA��O DO CR�DITO -->

            <table id="objeto_justificativa" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">
                <tr>
                    <td colspan="2" style="background-color: #CFCFCF; text-align:center; font-weight: bold;">OBJETO E JUSTIFICATIVA DA DESCENTRALIZA��O DO CR�DITO</td>
                </tr>
                <tr>
                    <td colspan="2" style="font-weight: bold;">45. Identifica��o (T�tulo/Objeto da Despesa)</td>
                </tr>
                <tr>
                    <td colspan="2">' . $dadosObjeto['tcpdscobjetoidentificacao'] . '</td>
                </tr>
                <tr>
                    <td colspan="2" style="font-weight: bold;">46. Objetivo</td>
                </tr>
                <tr>
                    <td colspan="2">' . $dadosObjeto['tcpobjetivoobjeto'] . '</td>
                </tr>
                <tr>
                    <td width="350" style="font-weight: bold;">47. UG/Gest�o Repassadora</td>
                    <td width="350" style="font-weight: bold;">48. UG/Gest�o Recebedora</td>
                </tr>
                <tr>
                    <td style="font-weight: bold;">152734 / CGSO-SPO </td>
                    <td>' . $dadosUG['ungcod'] . ' / ' . $dadosUG['ungdsc'] . '</td>
                </tr>
                <tr>
                    <td colspan="2" style="font-weight: bold;">49. Justificativa (Motiva��o/Clientela/Cronograma Fisico)</td>
                </tr>
                <tr>
                    <td colspan="2">' . $dadosObjeto['tcpobjetojustificativa'] . '</td>
                </tr>
                <tr>
                    <td colspan="2">
			';
        
	    if( $dadosObjeto['ungcodconcedente'] == UG_FNDE && in_array($dadosObjeto['ungcodpoliticafnde'], array(UG_SECADI, UG_SETEC, UG_SEB))  ){
			$html .= '
			    	<p>I - Integra este termo, independentemente de transcri��o, o Plano de Trabalho e o Termo de Refer�ncia, cujos dados ali contidos acatam os part�cipes e se comprometem em cumprir, sujeitando-se �s normas da Lei Complementar n� 101/2000, Lei n� 8.666, de 21 de junho de 1993, no que couber, Lei n� 4.320/1964, Lei n� 10.520/2002, Decreto n� 93.872/1986 e o de n� 6.170, de 25 de julho de 2007, Portaria Interministerial no 507, de 24 de novembro de 2011, Portaria Conjunta MP/MF/CGU n� 8, de 7 de novembro de 2012, bem como o disposto na Resolu��o CD/FNDE n� 28/2013.<br/></p>		    	
					<p>
			    	II - constituem obriga��es da CONCEDENTE:<br/>
					a) efetuar a transfer�ncia dos recursos financeiros previstos para a execu��o deste Termo, na forma estabelecida no Cronograma de Desembolso constante do Plano de Trabalho; <br/>
			    	</p>
			    	<p>
					III - constituem obriga��es do GESTOR DO PROGRAMA:<br/>
					a) orientar, supervisionar e cooperar com a implanta��o das a��es objeto deste Termo;<br/>
					b) acompanhar as atividades de execu��o, avaliando os seus resultados e reflexos;<br/>
					c) analisar o relat�rio de cumprimento do objeto do presente Termo;<br/>
		    		</p>
			    	<p> 
					IV - constituem obriga��es da PROPONENTE:<br/>
					a) solicitar ao gestor do projeto senha e login do SIMEC;<br/>
					b) solicitar � UG concedente senha e login do SIGEFWEB, no caso de recursos enviados pelo FNDE;<br/>
					c) promover a execu��o do objeto do Termo na forma e prazos estabelecidos no Plano de Trabalho;<br/>
					d) aplicar os recursos discriminados exclusivamente na consecu��o do objeto deste Termo;<br/>
					e) permitir e facilitar ao �rg�o Concedente o acesso a toda documenta��o, depend�ncias e locais do projeto;<br/>
					f) observar e exigir, na apresenta��o dos servi�os, se couber, o cumprimento das normas espec�ficas que regem a forma de execu��o da a��o a que os cr�ditos estiverem vinculados;<br/>
					g) manter o �rg�o Concedente informado sobre quaisquer eventos que dificultem ou interrompam o curso normal de execu��o do Termo;<br/>
					h) devolver os saldos dos cr�ditos or�ament�rios descentralizados e n�o empenhados, bem como os recursos financeiros n�o utilizados, conforme norma de encerramento do correspondente exerc�cio financeiro;<br/>
					i) emitir o relat�rio descritivo de cumprimento do objeto proposto;<br/>
					j) comprovar o bom e regular emprego dos recursos recebidos, bem como dos resultados alcan�ados;<br/>
					k) assumir todas as obriga��es legais decorrentes de contrata��es necess�rias � execu��o do objeto do termo;<br/>
					l) solicitar ao gestor do projeto , quando for o caso, a prorroga��o do prazo para cumprimento do objeto em at� quinze (15) dias antes do t�rmino previsto no termo de execu��o descentralizada, ficando tal prorroga��o condicionada � aprova��o por aquele;<br/>
					m) a presta��o de contas dos cr�ditos descentralizados devem integrar as contas anuais do �rg�o Proponente a serem apresentadas aos �rg�os de controle interno e externo, conforme normas vigentes;<br/>
					n) apresentar relat�rio de cumprimento do objeto pactuado at� 60 dias ap�s o t�rmino do prazo para cumprimento do objeto estabelecido no Termo.<br/>
			    	</p>';
		}else{
			$html .= '
			    	<p>I - Integra este termo, independentemente de transcri��o, o Plano de Trabalho e o Termo de Refer�ncia, cujos dados ali contidos acatam os part�cipes e se comprometem em cumprir, sujeitando-se �s normas da Lei Complementar n� 101/2000, Lei n� 8.666, de 21 de junho de 1993, no que couber, Lei n� 4.320/1964, Lei n� 10.520/2002, Decreto n� 93.872/1986 e o de n� 6.170, de 25 de julho de 2007, Portaria Interministerial no 507, de 24 de novembro de 2011, Portaria Conjunta MP/MF/CGU n� 8, de 7 de novembro de 2012, bem como o disposto na Resolu��o CD/FNDE n� 28/2013.<br/></p>
			    	<p> 
					II - constituem obriga��es da CONCEDENTE:<br/>
					a) efetuar a transfer�ncia dos recursos financeiros previstos para a execu��o deste Termo, na forma estabelecida no Cronograma de Desembolso constante do Plano de Trabalho;<br/>
					b) orientar, supervisionar e cooperar com a implanta��o das a��es objeto deste Termo;<br/>
					c) acompanhar as atividades de execu��o, avaliando os seus resultados e reflexos;<br/>
					d) analisar o relat�rio de cumprimento do objeto do presente Termo;<br/>
			    	</p>
			    	<p> 
					III - constituem obriga��es da PROPONENTE:<br/>
					a) solicitar ao gestor do projeto senha e login do SIMEC;<br/>
					b) solicitar � UG concedente senha e login do SIGEFWEB, no caso de recursos enviados pelo FNDE;<br/>
					c) promover a execu��o do objeto do Termo na forma e prazos estabelecidos no Plano de Trabalho;<br/>
					d) aplicar os recursos discriminados exclusivamente na consecu��o do objeto deste Termo;<br/>
					e) permitir e facilitar ao �rg�o Concedente o acesso a toda documenta��o, depend�ncias e locais do projeto;<br/>
					f) observar e exigir, na apresenta��o dos servi�os, se couber, o cumprimento das normas espec�ficas que regem a forma de execu��o da a��o a que os cr�ditos estiverem vinculados;<br/>
					g) manter o �rg�o Concedente informado sobre quaisquer eventos que dificultem ou interrompam o curso normal de execu��o do Termo;<br/>
					h) devolver os saldos dos cr�ditos or�ament�rios descentralizados e n�o empenhados, bem como os recursos financeiros n�o utilizados, conforme norma de encerramento do correspondente exerc�cio financeiro;<br/>
					i) emitir o relat�rio descritivo de cumprimento do objeto proposto;<br/>
					j) comprovar o bom e regular emprego dos recursos recebidos, bem como dos resultados alcan�ados;<br/>
					k) assumir todas as obriga��es legais decorrentes de contrata��es necess�rias � execu��o do objeto do termo;<br/>
					l) solicitar ao gestor do projeto , quando for o caso, a prorroga��o do prazo para cumprimento do objeto em at� quinze (15) dias antes do t�rmino previsto no termo de execu��o descentralizada, ficando tal prorroga��o condicionada � aprova��o por aquele;<br/>
					m) a presta��o de contas dos cr�ditos descentralizados devem integrar as contas anuais do �rg�o Proponente a serem apresentadas aos �rg�os de controle interno e externo, conforme normas vigentes;<br/>
					n) apresentar relat�rio de cumprimento do objeto pactuado at� 60 dias ap�s o t�rmino do prazo para cumprimento do objeto estabelecido no Termo.<br/>
			    	</p>';
		}
	
   $html .= '
                    </td>
                </tr>
            </table>
        ';

        $dadosPrevisao = buscarPrevisaoOrca($tcpid);
		$html .= '		
		<table id="previsao_orc" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%" style="font-size:8px;">
		  <tr>
		    <td height="21" colspan="8"style="background-color: #CFCFCF; text-align:center; font-weight: bold;">PREVIS�O OR�AMENT�RIA</td>
		  </tr>
		  <tr>
		  	<td style="font-weight: bold;">50. Ano</td>
		    <td style="font-weight: bold;">51. Programa de Trabalho</td>
		    <td style="font-weight: bold;">52. A��o</td>
		    <td style="font-weight: bold;">53. Plano Interno</td>
		    <td style="font-weight: bold;">54. Descri��o da A��o constante da LOA</td>
		    <td style="font-weight: bold;">55. Nat. da Despesa</td>
		    <td style="font-weight: bold;">56. M�s da Libera��o</td>
		    <td style="font-weight: bold;">57. Valor (em R$ 1,00)</td>
		  </tr>';
	
			if($dadosPrevisao):
				$arAnosPrevisao = array();
				$totalPrevisao = count($dadosPrevisao)-1; 
	  			foreach($dadosPrevisao as $k => $d):
	  			
		  			
	  				if(!in_array($d['proanoreferencia'], $arAnosPrevisao)){
		  				if($subTotalPorAno>0){
		  						$html .= '
									<tr bgcolor="#f0f0f0">
										<td colspan="8" align="right">
											<table>
												<tr>
													<td><b>Subtotal ('.($anoAnterior ? $anoAnterior : 'ano n�o informado').')</b>&nbsp;</td>
													<td align="right" width="110"><b>R$ '.formata_valor($subTotalPorAno).'</b></td>
												</tr>
											</table>
										</td>
									</tr>
								';
		  				}
						array_push($arAnosPrevisao, $d['proanoreferencia']);
	  					$subTotalPorAno = 0;
	  					$anoAnterior = $d['proanoreferencia'];
					}
	  			
					$html .= '		  
				 		  <tr>
				 		  	<td style="font-size:10px;">'.$d['proanoreferencia'].'</td>
						    <td style="font-size:10px;">'.$d['plano_trabalho'].'</font></td>
						    <td style="font-size:10px;">'.$d['acao'].'</td>
						    <td style="font-size:10px;">'.$d['plano_interno'].'</td>
						    <td style="font-size:10px;">'.$d['acao_loa'].'</td>
						    <td style="font-size:10px;">'.$d['nat_despesa'].'</td>
						    <td style="font-size:10px;">'.mes_extenso($d['crdmesliberacao']).'</td>
						    <td style="font-size:10px;" align="right">R$ '.$d['provalor'].'</td>
						  </tr>
						';
					
					$subTotalPorAno = $subTotalPorAno+$d['valor'];
					
					if($totalPrevisao==$k){
						$html .= '
							<tr bgcolor="#f0f0f0">
								<td colspan="8" align="right">
									<table>
										<tr>
											<td><b>Subtotal ('.($anoAnterior ? $anoAnterior : 'ano n�o informado').')</b>&nbsp;</td>
											<td align="right" width="110"><b>R$ '.formata_valor($subTotalPorAno).'</b></td>
										</tr>
									</table>
								</td>
							</tr>
						';
					}
				endforeach;
			endif;

        $sql = "select * from monitora.previsaoorcamentaria where tcpid = " . $tcpid . " order by proid limit 1";
        $rsPOmes = $db->pegaLinha($sql);

        $html .= '
                <tr>                    
                    <td colspan="2"><b>58. Prazo para o cumprimento do objeto</b></td>
                    <td colspan="2">' . ($rsPOmes['crdmesexecucao'] ? $rsPOmes['crdmesexecucao'] . '&nbsp;meses' : '') . '&nbsp;</td>
                    <td style="font-weight: bold;">59. TOTAL</td>
                    <td colspan="3">R$ ' . $d['total'] . '</td>
                </tr>
            </table>
        ';

        $mes = date("m");

        switch ($mes) {
            case 1: $mes = "Janeiro";
                break;
            case 2: $mes = "Fevereiro";
                break;
            case 3: $mes = "Mar�o";
                break;
            case 4: $mes = "Abril";
                break;
            case 5: $mes = "Maio";
                break;
            case 6: $mes = "Junho";
                break;
            case 7: $mes = "Julho";
                break;
            case 8: $mes = "Agosto";
                break;
            case 9: $mes = "Setembro";
                break;
            case 10: $mes = "Outubro";
                break;
            case 11: $mes = "Novembro";
                break;
            case 12: $mes = "Dezembro";
                break;
        }

        $sqlReitor = "
            Select  u.usunome, 
                    u.usucpf, 
                    to_char(h.htddata, 'DD/MM/YYYY') as htddata,
                    to_char(h.htddata, 'HH:II:SS') as hora,
                    g.ungdsc
            From monitora.termocooperacao t
            Join workflow.historicodocumento h on h.docid = t.docid
            Join workflow.acaoestadodoc a on a.aedid = h.aedid
            Join seguranca.usuario u on u.usucpf = h.usucpf
            Left Join unidadegestora g on g.ungcod = t.ungcodconcedente
            Where t.tcpid = " . $tcpid . " and a.esdiddestino = " . EM_ANALISE_DA_SECRETARIA . "
            Order by hstid asc
        ";

        $rsReitor = $db->pegaLinha($sqlReitor);
        $stAnaliseReitor = '';
        if ($rsReitor) {
            $stAnaliseReitor = "Validado e encaminhado pelo reitor {$rsReitor['usunome']} no dia {$rsReitor['htddata']} �s {$rsReitor['hora']} <br/>";
        }
        
        $sqlPresidente = "select * from monitora.termocooperacao where ungcodconcedente = '153173' and tcpid = $tcpid";        
        $rsPresidente = $db->pegaLinha ( $sqlPresidente );
        
        $where = '';
        if($rsPresidente){
        	$where .= " and a.esdiddestino in ( " . EM_EMISSAO_NOTA_CREDITO . ", ". EM_ANALISE_PELA_SPO ." )";
        }else{
        	$where .= " and a.esdiddestino = " . EM_ANALISE_PELA_CGSO . " ";
        }

        $sqlSecretario = "
            Select  u.usunome,
                    u.usucpf,
                    to_char(h.htddata, 'DD/MM/YYYY') as htddata,
                    to_char(h.htddata, 'HH:II:SS') as hora,
                    g.ungdsc
            From monitora.termocooperacao t
            Join workflow.historicodocumento h on h.docid = t.docid
            Join workflow.acaoestadodoc a on a.aedid = h.aedid
            Join seguranca.usuario u on u.usucpf = h.usucpf
            Left Join unidadegestora g on g.ungcod = t.ungcodconcedente
            Where t.tcpid = " . $tcpid . " 
            {$where}
            Order by hstid asc
        ";

        $rsSecretario = $db->pegaLinha($sqlSecretario);
        
        $stAnaliseSecretaria = '';
        if ($rsSecretario) {
        	if($rsPresidente){
        		$stAnaliseSecretaria = "Autorizado pelo(a) presidente(a) {$rsSecretario['usunome']} no dia {$rsSecretario['htddata']} �s {$rsSecretario['hora']}";
        	}
            else{
            	$stAnaliseSecretaria = "Autorizado pelo(a) secret�rio(a) {$rsSecretario['usunome']} no dia {$rsSecretario['htddata']} �s {$rsSecretario['hora']}";
            }
        }

        $html .= '		
            <table id="assinatura" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">		  
                <tr>
                    <td>&nbsp;</td>
                </tr>
                <tr>
                    <td align="center">
                        ' . $stAnaliseReitor . ' ' . $stAnaliseSecretaria . '
                    </td>
                </tr>
                <tr>
                    <td>&nbsp;</td>
                </tr>
                <tr>
                    <!-- <td colspan="2" style="text-align:center; font-weight: bold;">Bras�lia, ' . date("d") . ' de ' . $mes . ' de ' . date("Y") . '</td> -->
                    <td colspan="2" style="text-align:center; font-weight: bold;">'.recuperaDataGeraPdf().'</td>
                </tr>
                <tr>
                    <td>&nbsp;</td>
                </tr>	  		  
            </table>
        ';
        gerarPDFTermoCoopercao($html);
    }#Fim $_post gerarpdf
?>

    <link type="text/css" rel="stylesheet" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/calendario.js"></script>
    <script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

    <script type="text/javascript">

        $(document).ready(function() {

            //$('.gerarPDF').live('click',function(){
            $('.gerarPDF').click(function() {
                var tcpid = $(this).attr('id');

                window.open('gridTermosMec.php?gerarPDF=S&tcpid=' + tcpid, 'relatorio', 'width=900, height=600, status=1, menubar=1, toolbar=0, scrollbars=1, resizable=1');
            });
        });

    </script>

<?php
    monta_titulo('Listagem dos termos de Execu��o Descentralizado', '');
?>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

    <form method="post" name="formulario" id="formulario">
        <input type="hidden" id="pesquisa" name="pesquisa" value="1" />

        <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
            <tbody>
                <tr>
                    <td class="SubTituloDireita" width="25%">N�mero Termo: </td>
                    <td>
                        <?php
                            echo campo_texto('tcpid', 'N', 'S', '', 35, 20, '#######', '');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Sigla - Unidade Gestora Proponente: </td>
                    <td>
                        <?php
                            echo campo_texto('ungcodproponente', 'N', 'S', '', 35, 20, '', '');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Sigla - Unidade Gestora Concedente: </td>
                    <td>
                        <?php
                            echo campo_texto('ungcodconcedente', 'N', 'S', '', 35, 20, '', '');
                        ?>
                    </td>
                </tr>			
                <tr>
                    <td class="SubTituloDireita">Data da inclusao do termo: </td>
                    <td>
                        <?php
                            echo campo_data2('docdatainclusao', 'N', 'S', '', 'DD/MM/YYYY', '', '', null, '', '', 'docdatainclusao');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" colspan="2" align="center" style="text-align:center"> 
                        <input type="submit" name="btnPesquisar" id="btnPesquisar" value="Pesquisar" />
                        <input type="submit" name="btnVisualizarTodos" id="btnVisualizarTodos" value="Visualizar Todos " />  
                    </td>
                </tr>
            </tbody>
        </table>
    </form>
    
<?php
    $where = array();

    extract($_POST);

    $stJoin = '';

    if ($_POST['tcpid']) {
        $where[] = "tcp.tcpid = {$tcpid}";
    }

    if ($_POST['ungcodproponente']) {
        $where[] = "ung_p.ungabrev ilike ('%" . $_POST['ungcodproponente'] . "%')";
    }

    if ($_POST['ungcodconcedente']) {
        $where[] = "ung_c.ungabrev ilike ('%" . $_POST['ungcodconcedente'] . "%')";
    }

    if ($_POST['docdatainclusao']) {
        $where[] = "cast(doc.docdatainclusao as date) = '" . formataDataBanco( $_POST['docdatainclusao'] ) . "'";
    }

    $sqlCountSolAteracao = " (select count(*) from workflow.historicodocumento hst where hst.aedid = ".WF_ACAO_SOL_ALTERACAO." and hst.docid = tcp.docid) ";
    
    $sql = "
        Select	distinct '<img border=\"0\" id=\"' || tcp.tcpid || '\" class=\"gerarPDF\" title=\"Gerar PDF\" style=\"cursor: pointer;\" src=\"/imagens/acrobat.gif\">' as acao,			
                'Termo: '|| tcp.tcpid || case when {$sqlCountSolAteracao} > 0 then '.' || {$sqlCountSolAteracao}::varchar else '' end as decricao,
                coalesce(ung_p.ungabrev,' - ') as ung_propon,
                coalesce(ung_c.ungabrev,' - ') as ung_conced,
                to_char(doc.docdatainclusao, 'DD/MM/YYYY') as docdatainclusao,
                'R$ ' || trim(to_char(sum(prev.provalor), '999G999G999G999G999G999G999D99')) as provalor,
                esd.esddsc as esddsc
        From monitora.termocooperacao tcp

        Left Join elabrev.coordenacao coo on coo.cooid = tcp.cooid

        Left Join public.unidadegestora ung_p on ung_p.ungcod = tcp.ungcodproponente
        Left Join public.unidadegestora ung_c on ung_c.ungcod = tcp.ungcodconcedente

        Join monitora.previsaoorcamentaria prev on prev.tcpid = tcp.tcpid

        Left Join workflow.documento doc  ON doc.docid = tcp.docid
        Join workflow.estadodocumento esd  ON esd.esdid = doc.esdid and esd.esdid = " . EM_EXECUCAO . "	

        " . ( $where ? 'WHERE ' . implode(' AND ', $where) : '' ) . "

        Group By tcp.tcpid, ung_p.ungabrev, ung_c.ungabrev, doc.docdatainclusao, esd.esddsc, coo.coodsc, tcp.docid

        Order by 2 desc
            ";

//     ver($sql, d);
    $cabecalho = array("Gerar PDF", "Termo", "Unidade Gestora Proponente", "Unidade Gestora Concedente", "Data da Inclus�o", "Previs�o Or�amentaria - Valor", "Situa��o Documento");

    $align = Array('center', 'left', 'left', 'left', 'center', 'right', 'center');

    $db->monta_lista($sql, $cabecalho, 50, 5, 'N', 'center', '', '', '', $align);
?>


