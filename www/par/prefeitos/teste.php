<?php
require_once("dompdf/dompdf_config.inc.php");

$html =
  '<html><body><table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" style="font-family: Calibri">
		<thead>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td><img src="imagem/cabecalho.jpg" width="710px" border="0" ></img></td>
					</tr>
					<tr style="color: white; background-color: #FF8C00; text-align: center; font-size: 16px">
						<td>Munic�pio: '.$arrDadosMunicipio['nome'].' - '.$arrDadosMunicipio['uf'].'</td>
					</tr>
					<tr style="color: white; background-color: #FFA500; text-align: center; font-family: arial; font-weight: bold">
						<td>Principais a��es do Minist�rio da Educa��o</td>
					</tr>
				</table>
			</td>
		</tr>
		</thead>
		<tbody>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td align="justify" style="font-family: Calibri"><b>Sistemas e habilita��o.</b><br/>
							A situa��o cadastral da prefeitura nos sistemas corporativos do FNDE, bem como a habilita��o do munic�pio junto ao �rg�o s�o imprescind�veis para acessar os 
							recursos dos diferentes programas do Minist�rio da Educa��o. A habilita��o objetiva consolidar os documentos legais para efetiva��o das transfer�ncias de 
							recursos pelo FNDE. J� os sistemas informatizados s�o a porta de entrada para cadastramento de projetos, planejamento das a��es educacionais, consultas, 
							monitoramento de informa��es  entre outros.
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
				<tr><td>
					<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
						<tr style="font-family: Calibri">
							<th width="15%">Sistema</th>
							<th width="15%">Situa��o</th>
							<th width="70%">O que fazer</th>
						</tr>
						<tr style="font-family: Calibri; font-size: 12px">
							<td>Habilita��o FNDE / SAPE</td>
							<td>Habilitado, n�o habilitado. Dado din�mico.</td>
							<td>Acessar o site do FNDE e consultar a Resolu��o FNDE n� 10 de 31 de maio de 2012 que prev� os documentos necess�rios para cadastro.
								<a href="#">http://www.fnde.gov.br/fnde/legislacao/</a></td>
						</tr>
						<tr style="font-family: Calibri; font-size: 12px">
							<td>Sistema de Gerenciamento de Ades�o a Registro de Pre�os  - SIGARPWEB</td>
							<td>(Informa��o din�mica  - fonte SIGARP), Senha ativa, Senha expirada, Senha inexistente</td>
							<td>O sistema permite ao munic�pio o acesso  � produtos escolares  padronizados e de qualidade, pela ades�o aos registros de pre�os nacionais, 
								com contrata��o  de  empresas licitadas pelo FNDE. Para acessar entre  pelos m�dulos �Produtos -  Ades�o on line� ou  �Sistemas� em 
								<a href="#">http://www.fnde.gov.br/portaldecompras/</a></td>
						</tr>
					</table>
				</td></tr>
				</table>
			</td>
		</tr>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
				<tr><td>
					<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
						<tr style="font-family: Calibri">
							<th width="30%">Sistema</th>
							<th width="30%">O que �:</th>
							<th width="60%">Como acessar:</th>
						</tr>
						<tr style="font-family: Calibri; font-size: 12px">
							<td>Simec - PAR</td>
							<td>No M�dulo PAR  do Simec o munic�pio elabora o seu Plano de A��es Articuladas. Na edi��o atual, o PAR apresenta as a��es e suba��es  
								para o per�odo de 2011 a 2014.</td>
							<td>Acesse <a href="#">http://simec.mec.gov.br</a><br/>Na tela inicial do Simec, solicitar cadastro, preencher os dados cadastrais e enviar a solicita��o. 
								A senha de acesso � enviada para o e-mail informado no cadastro, desde que o endere�o eletr�nico esteja correto e perten�a � pessoa 
								cadastrada - prefeito(a) ou dirigente municipal de educa��o.</td>
						</tr>
						<tr style="font-family: Calibri; font-size: 12px">
							<td>SIGPC - Contas Online</td>
							<td>Sistema de Gest�o de Presta��o de Contas, regulamentado pela Resolu��o CD/FNDE n� 2/2012.</td>
							<td>Acesse� ��https://www.fnde.gov.br/sigpc ��Na tela inicial do Simec, solicitar cadastro, preencher os dados cadastrais e enviar a solicita��o. 
								A senha de acesso � enviada para o e-mail informado no cadastro, desde que o endere�o eletr�nico esteja correto</td>
						</tr>
						<tr style="font-family: Calibri; font-size: 12px">
							<td>SIOPE</td>
							<td>Sistema de Informa��es sobre Or�amentos P�blicos em Educa��o.</td>
							<td>Acesse http://www.fnde.gov.br/fnde-sistemas/sistema-siope-apresentacao �nesse endere�o o Senhor ir� obter todas as informa��es que o SIOPE 
								disponibiliza para o P�blico e �rg�os de Controle.</td>
						</tr>
						<tr style="font-family: Calibri; font-size: 12px">
							<td>Portal FNDE</td>
							<td>S�tio de Internet com informa��es atualizadas sobre a��es e programas executados pelo FNDE. Disponibiliza acesso a sistemas, legisla��o e 
								listagem dos respons�veis na autarquia.</td>
							<td>Acesse http://fnde.gov.br</td>
						</tr>
					</table>
				</td></tr>
				</table>
			</td>
		</tr>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr style="font-family: Calibri; font-weight: bold; font-size: 18px" align="justify">
						<td>Presta��o de Contas:</td>
					</tr>
					<tr>
						<td>A presta��o de contas tem  a finalidade de comprovar a boa e regular aplica��o dos recursos repassados, bem como o cumprimento do objeto 
							e objetivos do programa e/ou projeto. A partir do exerc�cio de 2011, a presta��o de contas � por meio eletr�nico, utilizando o � �Contas Online� 
							- Sistema de Gest�o de Presta��o de Contas (SiGPC). O respons�vel pela entidade � identificado de acordo com o cadastro feito na base corporativa
							do FNDE. Ap�s a atualiza��o, basta solicitar o reenvio da senha para primeiro acesso, o que pode ser feito junto ao Atendimento Institucional, pelo 
							0800 616161, pelo �Fale Conosco� dispon�vel no s�tio do FNDE ou acessar o endere�o www.fnde.gov.br/sigpc e informar seu CPF e, deixando em branco 
							o campo senha, clicar em �Entrar�, pois esse procedimento automaticamente far� o envio da mensagem com as orienta��es de acesso ao e-mail registrado 
							no FNDE. Vale esclarecer que o cadastro inicial e o envio da presta��o de contas dever� ser realizado pelo gestor (Prefeito).�

						</td>
					</tr>
					<tr>
						<td><table align="center" width="100%" cellspacing="0" cellpadding="3">
								<tr style="background-color: #FF8C00;">
									<td></td>
								</tr>
							</table>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		</tbody>
		<tfoot>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td width="10%"><img src="imagem/brasil.png" height="30px;" alt="" ></img></td>
						<td width="10%" align="left"><img src="imagem/fnde.jpg" height="30px;" alt="" ></img></td>
						<td align="right">'.date("j/n/Y H:i:s").'</td>
					</tr>
				</table>
			</td>
		</tr>
		</tfoot>
	</table></body></html>';

$dompdf = new DOMPDF();
$dompdf->load_html($html);
//$dompdf->set_paper('A4');
$dompdf->render();

$dompdf->stream("sample.pdf");




// create some HTML content
//$html = '<h1>Example of HTML text flow</h1>Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem. <em>Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur?</em> <em>Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur, vel illum qui dolorem eum fugiat quo voluptas nulla pariatur?</em><br /><br /><b>A</b> + <b>B</b> = <b>C</b> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>B</i> = <i>A</i> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>A</i> = <i>B</i> -&gt; &nbsp;&nbsp; <b>A</b> + <b>B</b> = <b>C</b> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>B</i> = <i>A</i> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>A</i> = <i>B</i> -&gt; &nbsp;&nbsp; <b>A</b> + <b>B</b> = <b>C</b> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>B</i> = <i>A</i> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>A</i> = <i>B</i> -&gt; &nbsp;&nbsp; <b>A</b> + <b>B</b> = <b>C</b> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>B</i> = <i>A</i> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>A</i> = <i>B</i> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <b>A</b> + <b>B</b> = <b>C</b> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>B</i> = <i>A</i> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>A</i> = <i>B</i> -&gt; &nbsp;&nbsp; <b>A</b> + <b>B</b> = <b>C</b> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>B</i> = <i>A</i> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>A</i> = <i>B</i> -&gt; &nbsp;&nbsp; <b>A</b> + <b>B</b> = <b>C</b> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>B</i> = <i>A</i> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>A</i> = <i>B</i> -&gt; &nbsp;&nbsp; <b>A</b> + <b>B</b> = <b>C</b> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>B</i> = <i>A</i> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>A</i> = <i>B</i><br /><br /><b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u>';

$html = '<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td width="10%"><img src="imagem/brasil.png" height="30px;" alt="" ></img></td>
						<td width="10%" align="left"><img src="imagem/fnde.jpg" height="30px;" alt="" ></img></td>
						<td align="right">'.date("j/n/Y H:i:s").'</td>
					</tr>
				</table>';
