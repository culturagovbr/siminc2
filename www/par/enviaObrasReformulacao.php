<?php

if( $_REQUEST['versao'] != '' ){
	echo '1.10';
	die();
}

set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as fun��es gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . 'includes/workflow.php';
include_once "autoload.php";
include_once "_constantes.php";

session_start();

// abre conex�o com o servidor de banco de dados
$db = new cls_banco();
$_SESSION['usucpf'] = '';

$sql = "SELECT DISTINCT * FROM carga.par_obras_reformulacao_mi_convencional crf
		INNER JOIN obras.preobra pre ON pre.preid = crf.preid
		INNER JOIN workflow.documento doc ON doc.docid = pre.docid
		WHERE preidbkp IS NULL" ;

$arrObras = $db->carregar( $sql );
$arrObras = is_array($arrObras) ? $arrObras : Array();

foreach( $arrObras as $obra ){

	if( $obra['tooid'] == 1 ){
		$esdidDestino = 1486;
	}else{
		$esdidDestino = 1488;
	}

	if( $obra['esdid'] !=  $esdidDestino ){

		$sql = "SELECT
					aedid
				FROM workflow.acaoestadodoc
				WHERE
					esdiddestino = $esdidDestino
					AND esdidorigem = {$obra['esdid']}";

		$aedid = $db->pegaUm($sql);

		if( $aedid == '' ){
			$sql = "INSERT INTO workflow.acaoestadodoc
						(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada,
						esdsncomentario, aedvisivel, aedcodicaonegativa, aedposacao)
					VALUES(
						{$obra['esdid']}, $esdidDestino, 'Enviar para Em reformula��o MI para convencional', 'A', 'Enviada para Em reformula��o MI para convencional', true, false, false, 'wf_pos_refurmulaPreObra_miparaconvencional( preid )' )
					RETURNING
						aedid";

			$aedid = $db->pegaUm($sql);
		}

		$teste = wf_alterarEstado( $obra['docid'], $aedid, 'Tramitado por reformularObra preid = '.$obra['preid'], array( 'docid' => $obra['docid'], 'preid' => $obra['preid'] ) );
		$db->commit();

		if( $teste ){

			$objPreObra = new PreObra( $obra['preid'] );
			$novoPreid = $objPreObra->criarBkp();
			$db->commit();

			$sql = "UPDATE obras.preobra SET ptoid = NULL, preusucpfreformulacao = '', predatareformulacao = now() WHERE preid = {$obra['preid']};";
			$db->executar( $sql );

			$sql = "UPDATE carga.par_obras_reformulacao_mi_convencional SET preidbkp = $novoPreid WHERE preid = {$obra['preid']}";
			$db->executar($sql);
		}
	}
	$db->commit();

	$sql = "SELECT DISTINCT
				ent.entemail
			FROM
				obras.preobra pre
			INNER JOIN par.instrumentounidade 			inu ON (inu.muncod = pre.muncodpar AND pre.tooid = 1) OR (inu.estuf = pre.estufpar AND pre.tooid <> 1)
			INNER JOIN par.instrumentounidadeentidade	iue ON iue.inuid = inu.inuid
			INNER JOIN entidade.entidade				ent ON ent.entnumcpfcnpj = iue.iuecnpj AND ent.entemail IS NOT NULL
			WHERE
				pre.preid = {$obra['preid']}
			ORDER BY
				ent.entemail DESC";

	$entemail = $db->pegaUm( $sql );

	$texto = '
			<html>
				<head>
					<title></title>
				</head>
				<body>
					<table style="width: 100%;">
						<thead>
							<tr>
								<td style="text-align: center;">
									<p><img  src="http://simec.mec.gov.br/imagens/brasao.gif" width="70"/><br/>
									<b>MINIST�RIO DA EDUCA��O</b><br/>
									FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCA��O<br/>
									DIRETORIA DE GEST�O, ARTICULA��O E PROJETOS EDUCACIONAIS<br/>
									COORDENA��O GERAL DE INFRAESTRUTURA EDUCACIONAL<br/>
									SBS Quadra 02 - Bloco F - 14� andar - Edif�cio FNDE - CEP -70070-929<br/>
								</td>
							</tr>
						</thead>
						<tbody>
							<tr>
								<td style="line-height: 15px;">
								</td>
							</tr>
							<tr>
								<td style="line-height: 15px; text-align:justify">
									<p>Prezado (a) Senhor (a) Gestor,</p>

									<p>Informamos que a(s) a��o (�es) referente � constru��o de creche em Metodologias Inovadoras est� (�o) aberta (s) para reformula��o, possibilitando a altera��o do
									projeto para constru��o em metodologia convencional.</p>

									<p>Com a disponibiliza��o de dois novos projetos, que aumentam a capacidade de atendimento das unidades escolares, o munic�pio dever� fazer os ajustes necess�rios
									�s obras, considerando o termo pactuado.</p>

									<p>Desta forma, comunicamos que a partir de agora a a��o encontra-se na situa��o "<b>Em reformula��o MI para Convencional</b>" e o sistema <b>SIMEC - M�dulo PAR
									(Plano Trabalho / �rvore / Lista de Obras)</b> j� se encontra aberto para altera��o, sendo necess�ria a substitui��o dos seguintes documentos t�cnicos nas respectivas abas:</p>


									<p><b>Aba Dados do terreno:</b><br>
									�	<b>Tipo de obra</b> - <i>Projeto 1 ou Projeto 2 Convencional, conforme o caso;</i><br>
									�	<b>Ponto de Refer�ncia</b> - <i>Local conhecido de refer�ncia pr�ximo ao terreno.</i></p>

									<p><b>Aba Documentos anexos</b><br>
									�	<b>Planta de loca��o</b> - <i>planta com o projeto padr�o FNDE da Creche (Tipo 1 ou Tipo 2, conforme o caso) inserido no terreno;</i><br>
									�	<b>Declara��o de Compatibilidade de Funda��o</b> do projeto com o terreno.</p>


									<p>Em caso de Troca de terreno, al�m de alterar o tipo de obra, todas as informa��es (<i>endere�o e relat�rio de vistoria</i>) e documentos anexos<br>
									(<i>fotos, plantas t�cnicas, estudo de demanda e declara��es</i>) tamb�m dever�o ser atualizados, conforme segue:</p>

									<p><b>Aba Dados do terreno:</b><br>
									�	<b>Nome do Terreno;</b><br>
									�	<b>Endere�o Completo</b> - <i>Logradouro, N�mero, Complemento, Ponto de Refer�ncia e Bairro do novo terreno;</i><br>
									�	<b>Coordenadas</b> <i>do novo terreno.</i></p>

									<p><b>Aba Relat�rio de vistoria:</b><br>
									�	Informa��es t�cnicas de infraestrutura existentes do novo terreno.</p>

									<p><b>Aba Cadastro de fotos do terreno:</b><br>
									�	Relat�rio fotogr�fico - <i>fotos do novo terreno, das ruas de acesso, lotes vizinhos, todas com legenda.</i></p>

									<p><b>Abas Planilha Or�ament�ria:</b><br>
									�	<i>N�o � pass�vel de preenchimento, a planilha ser� carregada de acordo com o valor da unidade federativa.</i></p>

									<p><b>Aba Documentos anexos</b><br>
									�	<b>Estudo de Demanda</b> - <i>novo estudo de demanda caso o terreno esteja em outro bairro;</i><br>
									�	<b>Planta de localiza��o</b> - <i>planta indicando a localiza��o do novo terreno na malha urbana do munic�pio;</i><br>
									�	<b>Planta de situa��o</b> - <i>planta indicando as dimens�es totais, lotes vizinhos e ruas de acesso do novo terreno;</i><br>
									�	<b>Planta de loca��o</b> - <i>planta com o projeto padr�o FNDE da Creche (Tipo 1 ou Tipo 2) inserido no novo terreno;</i><br>
									�	<b>Levantamento planialtim�trico</b> - <i>planta com indica��o das curvas de n�vel do novo terreno a cada metro de altura;</i><br>
									�	<b>Declara��o de fornecimento de infraestrutura;</b><br>
									�	<b>Declara��o de Compatibilidade de Funda��o</b> <i>do projeto com o novo terreno;</i><br>
									�	<b>Declara��o de Dominialidade</b>.</p>

									<p>Enquanto o munic�pio aguarda a finaliza��o do procedimento de reformula��o, com a valida��o do Prefeito no Termo de Compromisso reformulado,
									poder� dar in�cio ao processo licitat�rio, baixando a documenta��o t�cnica referente ao projeto escolhido, Tipo 1 ou Tipo 2, do site do FNDE, no link
									<a href="http://www.fnde.gov.br/programas/proinfancia" >http://www.fnde.gov.br/programas/proinfancia</a>, e providenciando a elabora��o dos projetos de implanta��o, bem como adequa��o da planilha or�ament�ria, caso necess�ria.</p>

									<p>Esclarecemos que:</p>

									<p>1.	Qualquer d�vida siga o passo-a-passo do Manual para Reformula��o de Obras Metodologia Inovadora para Metodologia Convencional disponibilizado no portal do FNDE, no link:<br>
									http://www.fnde.gov.br/arquivos/category/130-proinfancia?download=9490:proinfancia-creche-de-tipo-1-e-2-manual-reformulacao-mi-convencional<br>
									2.	Ap�s anexar os novos documentos, o munic�pio deve entrar na aba "Enviar para an�lise" da a��o e clicar no bot�o "<b>enviar para an�lise de reformula��o MI para Convencional</b>".<br>
									3.	Caso a an�lise de engenharia constate a necessidade de corre��o ou complementa��o da proposta, a a��o sair� da situa��o "<b>Em an�lise de reformula��o..."</b> e retornar�
										para a situa��o "<b>Em Dilig�ncia de Reformula��o...</b>" e as pend�ncias do que dever� ser corrigido estar�o descritas na aba "An�lise de engenharia". Neste caso, o proponente deve
										ler as observa��es descritas nos itens marcados com <u>N�o</u>, sanar as pend�ncias e envie para an�lise novamente.<br>
									4.	� responsabilidade do munic�pio monitorar periodicamente o sistema SIMEC m�dulo PAR e verificar se a a��o retornou para a situa��o "Em dilig�ncia de reformula��o". Nesse caso, siga os passos 3 e 2 respectivamente.<br>
									5.	Est� vedado o envio de documentos pelo correio. Todos os documentos referentes a altera��o solicitada dever�o ser substitu�dos no sistema.<br>
									6.	N�o ser�o toleradas modifica��es no projeto padr�o do FNDE.<br>
									7.	O prazo para envio dos documentos para an�lise de reformula��o � <u>7 dias ap�s sua libera��o no SIMEC.</u><br>
									8.	Propostas que permanecerem na situa��o "<b>Em reformula��o</b>" por mais de 60 dias travam o PAR municipal, impedindo a libera��o de recursos e aprova��o de novos pleitos.</p>

									<p>Durante a an�lise por parte do FNDE se for verificado que o novo terreno n�o cumpre as exig�ncias, ser� solicitada a corre��o ou
									complementa��o da documenta��o ou em caso de inviabilidade detectada a apresenta��o de um terceiro terreno. Caso seja constatada a
									inviabilidade t�cnica de implanta��o da unidade em todos os terrenos apresentados, os novos e o original, ser� recomendado o cancelamento
									do Termo de Compromisso e a devolu��o dos recursos.</p>

									<p>Informa��es complementares poder�o ser prestadas pelo endere�o eletr�nico reformulacao.obras@fnde.gov.br.</p>
								</td>
							</tr>
							<tr>
								<td style="padding: 10px 0 0 0;">
									Atenciosamente,
								</td>
							</tr>
							<tr>
								<td style="text-align: center; padding: 10px 0 0 0;">
									<img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="http://simec.mec.gov.br/imagens/obras/assinatura-fabio.png" />
									<br />
									<b>F�bio L�cio de Almeida Cardoso</b><br>
									Coordenador-Geral de Infraestrutura Educacional - CGEST<br>
									Diretoria de Gest�o, Articula��o e Projetos Educacionais - DIGAP<br>
									Fundo Nacional de Desenvolvimento da Educa��o-FNDE<br>
								</td>
							</tr>
						</tbody>
					</table>
				</body>
			</html>';


	$assunto  = "Confirma��o de abertura de Reformula��o de obras de constru��o de creches em metodologias inovadoras para convencional.";

	if( $_SERVER['SERVER_NAME'] == 'simec-d' || $_SERVER['SERVER_NAME'] == 'simec-d.mec.gov.br' ){
		$email = Array($_SESSION['email_sistema']);
	}else{
		$email = Array($entemail);
	}
	// 	$email = Array($_SESSION['email_sistema']);
	if( $entemail ){
// 		enviar_email(array('nome'=>SIGLA_SISTEMA. ' - PAR', 'email'=>'noreply@mec.gov.br'), $email, $assunto, $texto, $cc, $cco );
	}



	echo $obra['preid']." enviada para reformula��o.<br>";
// 	echo "$preid - BKP criado.<br>";
}

$sql = "SELECT DISTINCT preid FROM carga.par_obras_reformulacao_mi_convencional";

$arrPreid = $db->carregarColuna( $sql );
$arrPreid = is_array($arrPreid) ? $arrPreid : Array();

foreach( $arrPreid as $preid ){


	$sql = "SELECT
				obrid,
				doc.esdid,
				doc.docid
			FROM
				obras2.obras obr
			INNER JOIN workflow.documento doc ON doc.docid = obr.docid
			WHERE
				preid = $preid
				AND obrstatus = 'A'";

	$arObra = $db->pegaLinha( $sql );

	if( $arObra['obrid'] && $arObra['esdid'] != 768 ){

		$sql = "SELECT
					aedid
				FROM workflow.acaoestadodoc
				WHERE
					esdiddestino = 768
					AND esdidorigem = {$arObra['esdid']}";

		$aedid = $db->pegaUm($sql);

		if( $aedid == '' ){
			$sql = "INSERT INTO workflow.acaoestadodoc
						(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada,
						esdsncomentario, aedvisivel, aedcodicaonegativa)
					VALUES
						({$arObra['esdid']}, 768, 'Enviar para reformula��o', 'A', 'Enviada para reformula��o',
						true, false, false )
					RETURNING
						aedid";

				$aedid = $db->pegaUm($sql);
		}

		$teste = wf_alterarEstado( $arObra['docid'], $aedid, 'Tramitado por reformularObra preid = '.$preid, array( 'docid' => $arObra['docid'] ) );
		$db->commit();
		if( !$teste ){
			return false;
		}
	}
}
?>