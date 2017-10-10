<?php 
$termoExecDesc = new Ted_Model_TermoExecucaoDescentralizada();
$ungCodConc = $termoExecDesc->capturaConcedente();
?>
<script type="text/javascript">
$(function(){
    $("[for='tcptipoemenda-S'], [for='tcptipoemenda-N']").attr("class", "checkbox-inline");
})
</script>
<form class="well form-horizontal"
      name="<?=$this->element->getName(); ?>"
      id="<?=$this->element->getId(); ?>"
      action="<?= $this->element->getAction(); ?>"
      method="<?= $this->element->getMethod(); ?>"
      role="form">
	
	<?= $this->element->tcpid; ?>
	<?= $this->element->justid; ?>

    <?php if ($this->element->tipoemenda) : ?>
        <div class="form-group">
            <label class="control-label col-md-2" for="tipoemenda">� do tipo Emenda?</label>
            <div class="col-md-10">
                <?= $this->element->tipoemenda; ?>
            </div>
        </div>

        <div class="form-group div-emenda">
            <label class="control-label col-md-2" for="emeid">Emenda:</label>
            <div class="col-md-10 select-emenda">
                <?= $this->element->emeid; ?>
            </div>
        </div>
    <?php endif; ?>

    <div class="form-group">
        <label class="control-label col-md-2" for="identificacao">Identifica��o (T�tulo / Objeto da despesa):</label>
        <div class="col-md-10">        	
            <?= $this->element->identificacao; ?>
            <div id="counter-identificacao" class=""></div>
        </div>
    </div>

   	<div id="fndeblocked" class="form-group">
    	<label class="control-label col-md-2" for="objetivo">Objetivo:</label>
    	<div class="col-md-10">
    		<?= $this->element->objetivo;?>
            <div id="counter-objetivo" class=""></div>
    	</div>
    </div>

    <div class="form-group ">        
        <label class="control-label col-md-2" for="ugrepassadora">UG/Gest�o Repassadora:</label>
        <div class="col-md-10">
            <?= $this->element->ugrepassadora; ?>
	        </div>
	</div>

	<div class="form-group">        
		<label class="control-label col-md-2" for="ugrecebedora">UG/Gest�o Recebedora:</label>
	    <div class="col-md-10">
	    	<?= $this->element->ugrecebedora; ?>
	    </div>
	</div>
	<div class="form-group">        
		<label class="control-label col-md-2" for="justificativa">Justificativa (Motiva��o / Clientela / Cronograma f�sico):</label>
	    <div class="col-md-10">
	    	<?= $this->element->justificativa; ?>
            <div id="counter-justificativa" class=""></div>
	    </div>
	</div>
	<div class="form-group">        
		<label class="control-label col-md-2" for="endereco">Rela��es entre as Partes:</label>        
	    <div class="col-md-10">
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
								a) solicitar ao gestor do projeto senha e login do <?php echo SIGLA_SISTEMA; ?>;<br/>
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
								a) solicitar ao gestor do projeto senha e login do <?php echo SIGLA_SISTEMA; ?>;<br/>
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
		</div>
	</div>	    
    <hr />
    <div class="form-group">
    	<div class="col-md-offset-2">
    		<button type="button" class="btn btn-warning" name="cancel" id="cancel">Cancelar</button>
    		<button type="submit" class="btn btn-primary" name="submit" id="submit">Gravar</button>
    		<button type="submit" class="btn btn-success" name="submitcontinue" id="submitcontinue">Gravar e Continuar</button>    			
    	</div>
    </div>
    
</form>