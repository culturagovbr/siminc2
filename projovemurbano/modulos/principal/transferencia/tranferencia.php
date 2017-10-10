<?php

    if ($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
        exit;
    }

    if ($_REQUEST['estuf']){
        $_SESSION['projovemurbano']['estuf'] = $_REQUEST['estuf'];
    } else if ($_REQUEST['muncod']){
        $_SESSION['projovemurbano']['muncod'] = $_REQUEST['muncod'];
    }

    if (!$_SESSION['projovemurbano']['pjuid']) {
        carregarProJovemUrbano();
        //encaminharUltimoAcesso();
    }

    if (!$_SESSION['projovemurbano']['pjuid']) {
        die("<script>
                                alert('Problema encontrado no carregamento. Inicie novamente a navega��o.');
                                window.location='projovemurbano.php?modulo=inicio&acao=C';
                         </script>");
    }

    include_once APPRAIZ . 'includes/cabecalho.inc';
    echo '<br>';

    echo montarAbasArray(montaMenuProJovemUrbano(), $_SERVER['REQUEST_URI']);

    monta_titulo('Projovem Urbano', montaTituloEstMun());

    
    if( $_SESSION['projovemurbano']['ppuid'] == '1' ){
        echo intrucaoAno_2012();
    }elseif( $_SESSION['projovemurbano']['ppuid'] == '2' ){
        echo intrucaoAno_2013();
    } 
?>

<?php
function intrucaoAno_2012(){
    
    if ($_REQUEST['estuf'])
        $strH2 = "Senhor Secret�rio Estadual de Educa��o";
    else if ($_REQUEST['muncod'])
       $strH2 = "Senhor Secret�rio Munic�pal de Educa��o";
?>
    <form id="form" name="form" method="POST">

        <center>
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
                <tr>
                    <td width="90%" align="center">
                        <div style="overflow: auto; height:380px; width: 70%; background-color: rgb(250,250,250); border-width:1px; border-style:solid; text-align:left; padding: 10 10 10 10;" >
                            <h1>Instru��es</h1>
                            <? if ($_SESSION['projovemurbano']['estuf']) : ?>

                                <h2><?php echo $strH2 ?>,</h2>

                                <p>Caso seja do interesse desse estado fazer a ades�o ao Programa Projovem Urbano � necess�rio:</p>


                                <p>De 11 de novembro a 1� de dezembro de 2011:</p>
                                <p>1.	Preencher e validar este Termo de Ades�o no qual est� indicada a meta de atendimento proposta para seu estado;</p>
                                <p>2.	Preencher com sua sugest�o de meta o campo dispon�vel que pode ser maior ou menor daquela j� indicada.</p>
                                <p>A proposta de meta do termo de ades�o, firmado no prazo estabelecido acima, ser� analisada pela SECADI e a meta de seu estado poder� ser ajustada</p>


                                <p>De 05 a 16 de dezembro de 2011:</p>
                                <p>1.	Visualizar o Termo de Ades�o com os ajustes neste sistema;</p>
                                <p>2.	Validar novamente a sugest�o do estado, caso ela tenha sido aprovada pela SECADI;</p>
                                <p>3.	Validar, tamb�m, caso a sugest�o n�o tenha sido aprovada, ou seja, diferente da meta original.</p>


                                <p>De 19 de dezembro de 2011 a 16 de janeiro de 2012:</p>
                                <p>1.	Indicar um Coordenador Geral do Projovem Urbano deste estado, escolhido entre os profissionais do quadro efetivo da Secret�ria de Educa��o ou selecionado e contratado com recursos pr�prios (ver <a href="http://www.fnde.gov.br/index.php/legis-resolucoes" target="_blank">Resolu��o CD/FNDE N� 60, de 9 de novembro de 2011</a>);</p>
                                <p>2.	Solicitar o cadastro do Coordenador Geral  no m�dulo Projovem Urbano do <?php echo SIGLA_SISTEMA; ?> para o preenchimento do Plano de Implementa��o, conforme as instru��es do <?php echo SIGLA_SISTEMA; ?> e as determina��es da Resolu��o n� /2011 � <a href="http://www.fnde.gov.br/index.php/legis-resolucoes" target="_blank">Resolu��o CD/FNDE N� 60, de 9 de novembro de 2011</a>;</p>
                                <p>3.	Analisar e validar o Plano de Implementa��o que dever� estar devidamente preenchido e finalizado para a an�lise da SECADI;</p>
                                <p>4.	Imprimir e assinar o Plano de Implementa��o, ap�s a aprova��o desta secretaria, e envi�-lo para o endere�o:</p>
                                <p>
                                    Secretaria de Educa��o Continuada, Alfabetiza��o e Inclus�o.<br>
                                    Projovem Urbano 2012<br>
                                    Minist�rio da Educa��o<br>
                                    Esplanada do Minist�rio<br>
                                    Bloco L � 2� andar � sala 220<br>
                                    Bras�lia - DF<br>
                                    Cep 70.047-900
                                </p>
                            <? endif; ?>

                            <? if ($_SESSION['projovemurbano']['muncod']) : ?>

                                <h2>Senhor Secret�rio Municipal de Educa��o,</h2>

                                <p>Caso seja do interesse desse munic�pio fazer a ades�o ao Programa Projovem Urbano � necess�rio:</p>


                                <p>De 11 de novembro a 1� de dezembro de 2011:</p>
                                <p>1.	Preencher e validar este Termo de Ades�o no qual est� indicada a meta de atendimento proposta por seu munic�pio;</p>
                                <p>2.	Preencher com sua sugest�o de meta o campo dispon�vel que pode ser maior ou menor daquela j� indicada.</p>
                                <p>A proposta de meta do termo de ades�o, firmado no prazo estabelecido acima, ser� analisada pela SECADI e a meta de seu munic�pio poder� ser ajustada.</p>


                                <p>De 05 a 16 de dezembro de 2011:</p>
                                <p>1.	Visualizar o Termo de Ades�o com os ajustes neste sistema;</p>
                                <p>2.	Validar novamente a sugest�o do munic�pio, caso ela tenha sido aprovada pela SECADI;</p>
                                <p>3.	Validar, tamb�m, caso a sugest�o n�o tenha sido aprovada, ou seja, diferente da meta original.</p>


                                <p>De 19 de dezembro de 2011 a 16 de janeiro de 2012:</p>
                                <p>1.	Indicar um Coordenador Geral do Projovem Urbano deste munic�pio, escolhido entre os profissionais do quadro efetivo da Secret�ria de Educa��o ou selecionado e contratado com recursos pr�prios (<a href="http://www.fnde.gov.br/index.php/legis-resolucoes" target="_blank">Resolu��o CD/FNDE N� 60, de 9 de novembro de 2011</a>);</p>
                                <p>2.	Solicitar o cadastro do Coordenador Geral no m�dulo Projovem Urbano do <?php echo SIGLA_SISTEMA; ?> para o preenchimento do Plano de Implementa��o, conforme as instru��es do <?php echo SIGLA_SISTEMA; ?> e as determina��es da Resolu��o n� /2011 � <a href="http://www.fnde.gov.br/index.php/legis-resolucoes" target="_blank">Resolu��o CD/FNDE N� 60, de 9 de novembro de 2011</a>;</p>
                                <p>3.	Analisar e validar o Plano de Implementa��o que dever� estar devidamente preenchido e finalizado para a an�lise da SECADI;</p>
                                <p>4.	Imprimir e assinar o Plano de Implementa��o, ap�s a aprova��o desta secretaria, e envi�-lo para o endere�o:</p>
                                <p>
                                    Secretaria de Educa��o Continuada, Alfabetiza��o e Inclus�o.<br/>
                                    Projovem Urbano 2012<br/>
                                    Minist�rio da Educa��o<br/>
                                    Esplanada do Minist�rio<br/>
                                    Bloco L � 2� andar � sala 220<br/>
                                    Bras�lia - DF<br/>
                                    Cep 70.047-900
                                <p>
                                <? endif; ?>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloCentro"><input type="button" name="proximo" value="Pr�ximo" onclick="window.location='projovemurbano.php?modulo=principal/identificacao&acao=A';"></td>
                </tr>
            </table>
        </center>
    </form>
<?php
}


function intrucaoAno_2013(){
    
    if ($_REQUEST['estuf'])
        $strH2 = "Senhor Secret�rio Estadual de Educa��o";
    else if ($_REQUEST['muncod'])
        $strH2 = "Senhor Secret�rio Munic�pal de Educa��o";
?>
<form id="form" name="form" method="POST">
	<center>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td width="90%" align="center">
				<div style="overflow: auto; height:380px; width: 70%; background-color: rgb(250,250,250); border-width:1px; border-style:solid; text-align:left; padding: 10 10 10 10;" >
					<h1>Instru��es</h1>
					
					<h2><?php echo $strH2 ?>,</h2>

						<p>A ades�o ao Projovem Urbano para a edi��o 2013 foi pactuada com os Senhores, 
                        	por meio de Termo de Ades�o anexado � mensagem eletr�nica e posteriormente encaminhado a esta DPEJUV/MEC. 
                        	Agora esta ades�o dever� ser formalizada atrav�s do <?php echo SIGLA_SISTEMA; ?> - m�dulo Projovem Urbano.</p>

                       	<p>Para tanto, dever�o ser adotados os seguintes procedimentos:</p>
                                
                       	<p>1. Preencher os campos na aba - Identifica��o com os dados do Senhor(a) Secret�rio(a).</p>
                       	<p>2. Validar/aceitar o Termo de Ades�o no qual est� indicada a meta de atendimento no seu estado/munic�pio.</p>
                       	<p>
                        	3. Imprimir e assinar o Termo de Ades�o e envi�-lo para o seguinte endere�o:
                               Secretaria de Educa��o Continuada, Alfabetiza��o e Inclus�o - SECADI
                               Diretoria de Pol�ticas de Educa��o para a Juventude - DPEJUV
                               Minist�rio da Educa��o
                               Esplanada dos Minist�rios - Bloco L - 2� andar - Sala 220
                               CEP 70.047-900 Bras�lia-DF
                        </p>
					</div>
				</td>
			</tr>
			<tr>
				<td class="SubTituloCentro"><input type="button" name="proximo" value="Pr�ximo" onclick="window.location='projovemurbano.php?modulo=principal/identificacao&acao=A';"></td>
			</tr>
		</table>
	</center>
</form>
<?
}
?>
<? registarUltimoAcesso(); ?>
