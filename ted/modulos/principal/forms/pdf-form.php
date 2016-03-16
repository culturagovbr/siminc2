<?php 

//Declara��o de Objetos
$unidadeGestora = new Ted_Model_UnidadeGestora();
$termoExecDesc = new Ted_Model_TermoExecucaoDescentralizada();
$representanteLegal = new Ted_Model_RepresentanteLegal();
$justificativa = new Ted_Model_Justificativa();
$previsaoOrcamentaria = new Ted_Model_PrevisaoOrcamentaria();

//Capturando proponente
$ungCodProp = $termoExecDesc->capturaProponente();
//Capturando dados da ung proponente
$dadosUngProp = $unidadeGestora->pegaUnidade($ungCodProp);

//capturando dados do representante legal da ung proponente
$representanteProponente = $representanteLegal->recuperaResponsavelUG($ungCodProp);
$responsavel = $representanteLegal->areaTecnicaResponsavel($_GET['ted']);

//Capturando concedente
$ungCodConc = $termoExecDesc->capturaConcedente();
$coordenacaoResponsavel = $representanteLegal->coordenacaoResponsavel($_GET['ted']);

//capturando dados da ung concedente
$dadosUngConc = $unidadeGestora->pegaUnidade($ungCodConc['ungcodconcedente']);
//capturando dados do representante legal da ung concedente
$representanteConcedente = $representanteLegal->recuperaResponsavelUG($ungCodConc['ungcodconcedente']);

//Capturando dados da justificativa
$dadosJustificativa = $justificativa->capturaDadosJustificativa();

//Capturando lista previs�o or�amentaria do termo
$listaPO = $previsaoOrcamentaria->buscaPrevisaoOrcamentariaPDF();

//Capturando prazo para o objeto termo
$prazoObjeto = $previsaoOrcamentaria->capturaPrazoTotalPO();

//Capturando o reitor do atual termo
$reitor = $termoExecDesc->capturaReitor();

//Captura contagem do termo (n�o entendi)
$rsCountSolAlt = $termoExecDesc->capturaContagemTermo();
?>

<section class="col-md-12">
	<form class="form-horizontal" enctype="multipart/form-data" name="<?= $this->element->getName(); ?>" id="<?= $this->element->getId(); ?>" action="<?= $this->element->getAction(); ?>" method="<?= $this->element->getMethod(); ?>" role="form">
		<?= $this->element->tcpid; ?>
		<section class="well">
			<table class="col-md-12 table-condensed table-hover table-responsive">
				<tr>
					<th class="col-md-2 text-right" style="background-color: #BBB;">N� do Termo:</td>
					<td class="col-md-6">&nbsp; <?= ($rsCountSolAlt > 0 ? $_GET['ted'].'.'. $rsCountSolAlt : $_GET['ted'])?></td>
					<?php if ($ungCodConc['ungcodconcedente'] == UG_FNDE && in_array ($ungCodConc['ungcodpoliticafnde'], array (UG_SECADI,UG_SETEC,UG_SEB))) { ?>
					<th class="col-md-2 text-right" style="background-color: #BBB;">N� do Processo:</th>
					<td class="col-md-4">&nbsp; <?= $ungCodConc['tcpnumprocessofnde']?></td>
					<?php } else { ?>
					<th class="col-md-2 text-right"></th>
					<td class="col-md-2"></td>
					<?php } ?>
				</tr>
			</table>
		</section>
		<br>			
	    <table class="col-md-12 table-condensed table-bordered table-hover table-responsive">
	    	<thead>
	    		<tr class="well" >
	    			<th  colspan="4" class="text-center">DADOS DO �RG�O OU ENTIDADE PROPONENTE</th>
	    		</tr>
	    	</thead>
	    	<tbody>
	    		<tr>
	    			<th>C�d. Und. Gestora</th>
	    			<th>C�d. da Gest�o</th>
	    			<th>CNPJ</th>
	    			<th>Raz�o Social</th>
	    		</tr>
	    		<tr>
	    			<td><?= $dadosUngProp['ungcod'];?></td>
	    			<td><?= $dadosUngProp['gescod']; ?></td>
	    			<td><?= formatar_cnpj($dadosUngProp['ungcnpj']); ?></td>
	    			<td><?= $dadosUngProp['descricao']; ?></td>
	    		</tr>
	    		<tr>
	    			<th colspan="2">Endere�o</th>
	    			<th>Bairro ou Distrito</th>	
	    			<th>Munic�pio</th>
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= $dadosUngProp['ungendereco']; ?></td>
	    			<td><?= $dadosUngProp['ungbairro']; ?></td>
	    			<td><?= $dadosUngProp['municipio']; ?></td>
	    		</tr>
	    		<tr>
	    			<th>UF</th>
	    			<th>CEP</th>
	    			<th>Telefone</th>
	    			<th>E-Mail</th>
	    		</tr>
	    		<tr>
	    			<td><?= $dadosUngProp['estuf']; ?></td>
	    			<td><?= $dadosUngProp['ungcep']; ?></td>
	    			<td><?= $dadosUngProp['ungfone']; ?></td>
	    			<td><?= $dadosUngProp['ungemail']; ?></td>
	    		</tr>	    		
	    	</tbody>	    		    
	    </table>		
	    <br>
		<table class="col-md-12 table-condensed table-bordered table-hover table-responsive">
	    	<thead>
	    		<tr class="well" >
	    			<th  colspan="4" class="text-center">REPRESENTANTE LEGAL DO ORG�O OU ENTIDADE PROPONENTE</th>
	    		</tr>
	    	</thead>
	    	<tbody>
	    		<tr>
	    			<th colspan="2">CPF</th>
	    			<th colspan="2">Nome do Representante Legal</th>	    				    			
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= formatar_cpf($representanteProponente['usucpf']); ?></td>
	    			<td colspan="2"><?= $representanteProponente['usunome']; ?></td>	    				    			
	    		</tr>
	    		<tr>
	    			<th colspan="2">Endere�o</th>
	    			<th>Bairro ou Distrito</th>	
	    			<th>Munic�pio</th>
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= $representanteProponente['endereco']; ?></td>
	    			<td><?= $representanteProponente['bairro']; ?></td>
	    			<td><?= $representanteProponente['municipio']; ?></td>
	    		</tr>
	    		<tr>
	    			<th>UF</th>
	    			<th>CEP</th>
	    			<th>Telefone</th>
	    			<th>E-Mail</th>
	    		</tr>
	    		<tr>
	    			<td><?= $representanteProponente['estado']; ?></td>
	    			<td><?= $representanteProponente['endcep']; ?></td>
	    			<td><?= $representanteProponente['fone']; ?></td>
	    			<td><?= $representanteProponente['usuemail']; ?></td>
	    		</tr>	  
	    		<tr>
	    			<th colspan="2">N� da C�dula da CI</th>
	    			<th>�rg�o Expeditor</th>	
	    			<th>Cargo</th>
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= $representanteProponente['numeroidentidade']; ?></td>
	    			<td><?= $representanteProponente['entorgaoexpedidor']; ?></td>
	    			<td><?= $representanteProponente['usufuncao']; ?></td>	    			
	    		</tr>
                <?php if ($responsavel): ?>
                <tr>
                    <th colspan="3">�rea T�cnica Respons�vel</th>
                    <th>CPF</th>
                </tr>
                <tr>
                    <td colspan="3"><?= $responsavel['usunome'] ?></td>
                    <td><?= formatar_cpf($responsavel['usucpf']) ?></td>
                </tr>
                <?php endif; ?>
	    	</tbody>	    		    
	    </table>		
	    <br>
	    <table class="col-md-12 table-condensed table-bordered table-hover table-responsive">
	    	<thead>
	    		<tr class="well" >
	    			<th  colspan="4" class="text-center">DADOS DO �RG�O OU ENTIDADE CONCEDENTE</th>
	    		</tr>
	    	</thead>
	    	<tbody>
	    		<tr>
	    			<th>C�d. Und. Gestora</th>
	    			<th>C�d. da Gest�o</th>
	    			<th>CNPJ</th>
	    			<th>Raz�o Social</th>
	    		</tr>
	    		<tr>
	    			<td><?= $dadosUngConc['ungcod'];?></td>
	    			<td><?= $dadosUngConc['gescod']; ?></td>
	    			<td><?= formatar_cnpj($dadosUngConc['ungcnpj']); ?></td>
	    			<td><?= $dadosUngConc['descricao']; ?></td>
	    		</tr>
	    		<tr>
	    			<th colspan="2">Endere�o</th>
	    			<th>Bairro ou Distrito</th>	
	    			<th>Munic�pio</th>
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= $dadosUngConc['ungendereco']; ?></td>
	    			<td><?= $dadosUngConc['ungbairro']; ?></td>
	    			<td><?= $dadosUngConc['municipio']; ?></td>
	    		</tr>
	    		<tr>
	    			<th>UF</th>
	    			<th>CEP</th>
	    			<th>Telefone</th>
	    			<th>E-Mail</th>
	    		</tr>
	    		<tr>
	    			<td><?= $dadosUngConc['estuf']; ?></td>
	    			<td><?= $dadosUngConc['ungcep']; ?></td>
	    			<td><?= $dadosUngConc['ungfone']; ?></td>
	    			<td><?= $dadosUngConc['ungemail']; ?></td>
	    		</tr>	    		
	    	</tbody>	    		    
	    </table>		
	    <br>
	    <table class="col-md-12 table-condensed table-bordered table-hover table-responsive">
	    	<thead>
	    		<tr class="well" >
	    			<th  colspan="4" class="text-center">REPRESENTANTE LEGAL DO ORG�O OU ENTIDADE CONCEDENTE</th>
	    		</tr>
	    	</thead>
	    	<tbody>
	    		<tr>
	    			<th colspan="2">CPF</th>
	    			<th colspan="2">Nome do Representante Legal</th>	    				    			
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= formatar_cpf($representanteConcedente['usucpf']);?></td>
	    			<td colspan="2"><?= $representanteConcedente['usunome']; ?></td>	    				    			
	    		</tr>
	    		<tr>
	    			<th colspan="2">Endere�o</th>
	    			<th>Bairro ou Distrito</th>	
	    			<th>Munic�pio</th>
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= $representanteConcedente['endereco']; ?></td>
	    			<td><?= $representanteConcedente['bairro']; ?></td>
	    			<td><?= $representanteConcedente['municipio']; ?></td>
	    		</tr>
	    		<tr>
	    			<th>UF</th>
	    			<th>CEP</th>
	    			<th>Telefone</th>
	    			<th>E-Mail</th>
	    		</tr>
	    		<tr>
	    			<td><?= $representanteConcedente['estado']; ?></td>
	    			<td><?= $representanteConcedente['endcep']; ?></td>
	    			<td><?= $representanteConcedente['fone']; ?></td>
	    			<td><?= $representanteConcedente['usuemail']; ?></td>
	    		</tr>	  
	    		<tr>
	    			<th colspan="2">N� da C�dula da CI</th>
	    			<th>�rg�o Expeditor</th>	
	    			<th>Cargo</th>
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= $representanteConcedente['numeroidentidade']; ?></td>
	    			<td><?= $representanteConcedente['entorgaoexpedidor']; ?></td>
	    			<td><?= $representanteConcedente['usufuncao']; ?></td>	    			
	    		</tr>
                <?php if ($coordenacaoResponsavel): ?>
                    <tr>
                        <th colspan="2">Coordena��o Respons�vel</th>
                        <th>CPF</th>
                    </tr>
                    <tr>
                        <td colspan="2"><?= $coordenacaoResponsavel['usunome']; ?></td>
                        <td><?= formatar_cpf($coordenacaoResponsavel['usucpf']); ?></td>
                    </tr>
                <?php endif; ?>
	    	</tbody>
	    </table>		
	    <br>		
	    <table class="col-md-12 table-condensed table-bordered table-hover table-responsive">
	    	<thead>
	    		<tr class="well" >
	    			<th  colspan="2" class="text-center">OBJETO E JUSTIFICATIVA DA DESCENTRALIZA��O DO CR�DITO</th>
	    		</tr>
	    	</thead>
	    	<tbody>
	    		<tr>
	    			<th colspan="2">Identifica��o (T�tulo/Objeto da Despesa)</th>	    				    				    			
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= $dadosJustificativa['identificacao'];?></td>
	    		</tr>
	    		<tr>
	    			<th colspan="2">Objetivo</th>	    			
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= $dadosJustificativa['objetivo']; ?></td>
	    		</tr>
	    		<tr>
	    			<th>UG/Gest�o Repassadora</th>
	    			<th>UG/Gest�o Recebedora</th>	    				    			
	    		</tr>
	    		<tr>
	    			<td><?= $dadosJustificativa['ugrepassadora']; ?></td>
	    			<td><?= $dadosJustificativa['ugrecebedora']; ?></td>
	    		</tr>	  
	    		<tr>
	    			<th colspan="2">Justificativa (Motiva��o/Clientela/Cronograma F�sico)</th>	    				    			
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= $dadosJustificativa['justificativa']; ?></td>
	    		</tr>  	
	    		<tr>
	    			<td colspan="2">
	    			<?php 
	    				if($ungCodConc['ungcodconcedente'] == UG_FNDE 
							&& in_array($ungCodConc['ungcodpoliticafnde'], array(UG_SECADI,UG_SETEC,UG_SEB)))
						{
					?>
							<p>
								I - Integra este termo, independentemente de transcri��o, o Plano de Trabalho e o Termo de Refer�ncia, cujos dados ali contidos acatam os part�cipes e se comprometem em cumprir, sujeitando-se �s normas da Lei Complementar n� 101/2000, Lei n� 8.666, de 21 de junho de 1993, no que couber, Lei n� 4.320/1964, Lei n� 10.520/2002, Decreto n� 93.872/1986 e o de n� 6.170, de 25 de julho de 2007, Portaria Interministerial no 507, de 24 de novembro de 2011, Portaria Conjunta MP/MF/CGU n� 8, de 7 de novembro de 2012, bem como o disposto na Resolu��o CD/FNDE n� 28/2013.<br/>
							</p>
							<br>		    	
							<p>
		    					II - constituem obriga��es da CONCEDENTE:<br/>
								a) efetuar a transfer�ncia dos recursos financeiros previstos para a execu��o deste Termo, na forma estabelecida no Cronograma de Desembolso constante do Plano de Trabalho; <br/>
		    				</p>
		    				<br>
		    				<p>
								III - constituem obriga��es do GESTOR DO PROGRAMA:<br/>
								a) orientar, supervisionar e cooperar com a implanta��o das a��es objeto deste Termo;<br/>
								b) acompanhar as atividades de execu��o, avaliando os seus resultados e reflexos;<br/>
								c) analisar o relat�rio de cumprimento do objeto do presente Termo;<br/>
	    					</p>
	    					<br>
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
					    	</p>
						<?php 
						}else
						{
						?>							
	    					<p>
	    						I - Integra este termo, independentemente de transcri��o, o Plano de Trabalho e o Termo de Refer�ncia, cujos dados ali contidos acatam os part�cipes e se comprometem em cumprir, sujeitando-se �s normas da Lei Complementar n� 101/2000, Lei n� 8.666, de 21 de junho de 1993, no que couber, Lei n� 4.320/1964, Lei n� 10.520/2002, Decreto n� 93.872/1986 e o de n� 6.170, de 25 de julho de 2007, Portaria Interministerial no 507, de 24 de novembro de 2011, Portaria Conjunta MP/MF/CGU n� 8, de 7 de novembro de 2012, bem como o disposto na Resolu��o CD/FNDE n� 28/2013.<br/>
	    					</p>
	    					<br>
					    	<p> 
								II - constituem obriga��es da CONCEDENTE:<br/>
								a) efetuar a transfer�ncia dos recursos financeiros previstos para a execu��o deste Termo, na forma estabelecida no Cronograma de Desembolso constante do Plano de Trabalho;<br/>
								b) orientar, supervisionar e cooperar com a implanta��o das a��es objeto deste Termo;<br/>
								c) acompanhar as atividades de execu��o, avaliando os seus resultados e reflexos;<br/>
								d) analisar o relat�rio de cumprimento do objeto do presente Termo;<br/>
					    	</p>
					    	<br>
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
					    	</p>
	    			<?php 
						}
	    			?>
	    			</td>
	    		</tr>	
	    	</tbody>	    		    
	    </table>	
	    <br>
	    
	    <table class="col-md-12 table-condensed table-bordered table-hover table-responsive">
	    	<thead>
	    		<tr class="well" >
	    			<th  colspan="8" class="text-center">PREVIS�O OR�AMENT�RIA</th>
	    		</tr>
	    		<tr style="font-size:12px;">
	    			<th>Ano</th>
	    			<th>Programa de Trabalho</th>
	    			<th>A��o</th>
	    			<th>Plano Interno</th>
	    			<th>Descri��o da A��o constante da LOA</th>
	    			<th>Natureza da Despesa</th>
	    			<th>M�s da Libera��o</th>
	    			<th>Valor (em R$ 1,00)</th>
	    		</tr>
	    	</thead>
	    	<tbody style="font-size:11px;">
	    	<?php 
	    	if($listaPO)
			{
				$arNotaCredito = array ();
				$totalPrevisao = count($listaPO) - 1;
                $total = 0;
	    		foreach ($listaPO as $k => $po)
				{
                    $total += $po['valor'];

					if (!in_array($po['notacredito'], $arNotaCredito ))
					{
						if ($subTotal > 0)
						{
							echo '
									<tr bgcolor="#f0f0f0">
										<td colspan="8" align="right">
											<table width="550">
												<tr>
													<td align="left"><b>Nota de Cr�dito(' . ($ncAnterior ? $ncAnterior : 'N�o informado') . ')</b>&nbsp;</td>
													<td align="right"><b>Subtotal</b>&nbsp;</td>
													<td align="right" width="110"><b>R$ ' . formata_valor ( $subTotal ) . '</b></td>
												</tr>
											</table>
										</td>
									</tr>
								';
						}
						array_push ($arNotaCredito, $po['notacredito'] );
						$subTotal = 0;
						$ncAnterior = $po['notacredito'];
					}
					echo 
					'					
					<tr>
						<td>'.$po['proanoreferencia'] . '</td>
						<td>'.$po['plano_trabalho'] . '</td>
						<td>'.$po['acao'] . '</td>
						<td>'.$po['plano_interno'] . '</td>
						<td>'.$po['acao_loa'] . '</td>
						<td>'.$po['nat_despesa'] . '</td>
						<td>'.Ted_Utils_Model::mes_extenso($po['crdmesliberacao']).'</td>
						<td style="text-align:right;">R$ '.$po['provalor'].'</td>
					</tr>
					';
					$subTotal = $subTotal + $po['valor'];
					
					if ($totalPrevisao == $k)
					{
						echo'
							<tr bgcolor="#f0f0f0">
								<td colspan="8" align="right">
									<table width="550">
										<tr>
											<td align="left"><b>Nota de Cr�dito(' . ($ncAnterior ? $ncAnterior : 'ano n�o informado') . ')</b>&nbsp;</td>
											<td align="right"><b>Subtotal</b>&nbsp;</td>
											<td align="right" width="110"><b>R$ ' . formata_valor ( $subTotal ) . '</b></td>
										</tr>
									</table>
								</td>
							</tr>
						';
					}
				}    	
				
				echo '
				<tr>
		  			<th style="font-size:12px;" colspan="3">Prazo para o cumprimento do objeto</th>
		  			<td style="font-size:12px;" colspan="1"><b>'.($prazoObjeto['crdmesexecucao'] ? $prazoObjeto['crdmesexecucao'] . '&nbsp;meses' : '').'</b>&nbsp;</td>
		    		<th style="font-size:12px;" colspan="2" class="text-right">TOTAL</th>
		    		<td style="font-size:12px;" colspan="2" align="right"><b>'.number_format2($total).'</b></td>
				</tr>
				';
			}			    		    	
	    	?>	    		  		
	    	</tbody>	    		    
	    </table>	
	    <br>
	    <section class="well">
	    <!-- ASSINATURA DO TERMO -->
		    <?php 
		    
		    //Guarda o estado atual do termo
		    $estadoTermoAtual = $termoExecDesc->pegarEstadoAtualTermo();
		    
		    //mostra assinatura somente quando termo � aprovado pelo representante legal do proponente
		    $mostraAssinaturaReitor = (!in_array($estadoTermoAtual, array(EM_CADASTRAMENTO, TERMO_AGUARDANDO_APROVACAO_GESTOR_PROP, EM_APROVACAO_DA_REITORIA)));

		    if ($reitor && $mostraAssinaturaReitor) {
		    	$stAnaliseReitor = "Autorizado pelo(a) {$reitor['usunome']} no dia {$reitor['htddata']} �s {$reitor['hora']} <br/>";
		    }

		    //Captura presidente e secret�rio do termo
		    $presSec = $termoExecDesc->capturaSecretarioTermo();
		    $rsPresidente = $presSec[0];
		    $rsSecretario = $presSec[1];
		    
		    
		    $stAnaliseSecretaria = '';
		    
		    $arrayPull = array(
		    		EM_CADASTRAMENTO,
		    		TERMO_AGUARDANDO_APROVACAO_GESTOR_PROP,
		    		EM_APROVACAO_DA_REITORIA,
		    		EM_ANALISE_OU_PENDENTE,
		    		AGUARDANDO_APROVACAO_DIRETORIA,
		    		AGUARDANDO_APROVACAO_SECRETARIO,
		    		TERMO_EM_ANALISE_ORCAMENTARIA_FNDE,
		    		AGUARDANDO_APROVACAO_DIRETORIA,
		    );
		    
		    /**
		     * mostra assinatura somente quando termo �
		     * autorizado pelo representante legal do concedente
		     * ou
		     * encaminhado para valida��o da diretoria do FNDE
		    */
		    $mostraAssConcedente = (!in_array($estadoTermoAtual, $arrayPull));
		    
		    if ($rsSecretario && $mostraAssConcedente) {
		    	if ($rsPresidente) {
		    		$stAnaliseSecretaria = "Autorizado pelo(a) presidente(a) {$rsSecretario['usunome']} no dia {$rsSecretario['htddata']} �s {$rsSecretario['hora']}";
		    	} else {
		    		$stAnaliseSecretaria = "Autorizado pelo(a) secret�rio(a) {$rsSecretario['usunome']} no dia {$rsSecretario['htddata']} �s {$rsSecretario['hora']}";
		    	}
		    }
		    
		    echo '
				<table id="assinatura" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
				  <tr>
				    <td>&nbsp;</td>
				  </tr>
				  <tr>
				    <td align="center">
				    	' . $stAnaliseReitor . '
				    	' . $stAnaliseSecretaria . '
				    </td>
				  </tr>
				  <tr>
				    <td>&nbsp;</td>
				  </tr>
				  <tr>
		    		<td colspan="2" style="text-align:center; font-weight: bold;">'.Ted_Utils_Model::recuperaDataGeraPdf().'</td>
				  </tr>
				  <tr>
				    <td>&nbsp;</td>
				  </tr>
				</table>
	    	';
		    ?>	    
	    </section>	    	   
		<hr>
		<div class="well form-group">    	
			<div class=" col-md-10">			
				<button type="submit" class="btn btn-success" name="requisicao" value="gerarPDF" id="submit"><span class="glyphicon glyphicon-list-alt"></span> Gerar PDF</button>					    			
			</div>
		</div>		
	</form>
</section>