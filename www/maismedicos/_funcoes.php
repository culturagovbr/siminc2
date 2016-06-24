<?php 
function comboEstado($estuf = null, $habilitado = "S")
{
	global $db;

	$estuf = $_POST['estuf'] ? $_POST['estuf'] : $estuf;
	
	$sql = "select 
				estuf as codigo,
				estuf as descricao
			from
				territorios.estado
			order by
				estuf";
	$db->monta_combo("estuf", $sql, $habilitado, "Selecione...", 'filtrarMunicipio', '', '', '', 'N','estuf');
}

function comboMunicipio($estuf = null, $habilitado = "S")
{
	global $db;

	$estuf = $_POST['estuf'] ? $_POST['estuf'] : $estuf;
	$muncod = $_POST['muncod'] ? $_POST['muncod'] : $muncod;
	
	if(!$estuf){
		echo "Selecione a UF.";
	}else{
		$sql = "select
					muncod as codigo,
					mundescricao as descricao
				from
					territorios.municipio
				where
					estuf = '$estuf'
				order by
					mundescricao";
		$db->monta_combo("muncod", $sql, $habilitado, "Selecione...", '', '', '', '', 'N','muncod');
	}
}

function textoHTML2($entid)
{
	global $db;
	$sql = "select * from entidade.entidade  ent
	inner join entidade.endereco ende ON ende.entid = ent.entid
	inner join territorios.municipio mun ON mun.muncod = ende.muncod
	where ent.entid = $entid";
	$arrDados = $db->pegaLinha($sql);

	$sql = "select
	usunome as nome
	from
	seguranca.usuario usu
	inner join
	maismedicos.usuarioresponsabilidade ure ON ure.usucpf = usu.usucpf
	where
	ure.entid = $entid
	and
	ure.pflcod = ".PERFIL_REITOR."
	and
				ure.rpustatus = 'A'
			and
				usu.usustatus = 'A'
			order by
				rpudata_inc desc";
	$arrReitor = $db->pegaLinha($sql);
	if(!$arrReitor){
		$sql = "SELECT
		DISTINCT fun.funid, fun.fundsc, ent.entnome as nome, ent.entid
		FROM
		entidade.funcao fun
		LEFT JOIN
		entidade.funcaoentidade fen ON fen.funid = fun.funid
		AND fen.entid IN (SELECT
		fen2.entid
		FROM
		entidade.funentassoc fea2
		LEFT JOIN
		entidade.funcaoentidade fen2 on fea2.fueid = fen2.fueid
		WHERE
		fea2.entid='$entid'
		AND fun.funid = fen2.funid)
		LEFT JOIN
		entidade.entidade ent ON fen.entid = ent.entid
		WHERE
		fun.funid IN('21')";
		$arrReitor = $db->pegaLinha($sql);
	}
	?>
<div style="text-align:justify;">
<center>
<h2>PORTARIA NORMATIVA N� 14, DE 9 DE JULHO DE 2013</h2>
<p><b>MINIST�RIO DA EDUCA��O</b></p>
<p><b>GABINETE DO MINISTRO</b></p>
<p><b>DOU de 10/07/2013 (n� 131, Se��o 1, p�g. 18)</b></p>
</center>
<p>Disp�e sobre os procedimentos de ades�o das institui��es federais de educa��o superior ao Projeto Mais M�dicos e d� outras provid�ncias.</p>
<p>O MINISTRO DE ESTADO DA EDUCA��O no uso da atribui��o que lhe confere o art. 87, inciso II da Constitui��o Federal, e tendo em vista o disposto na Medida Provis�ria n� 621, de 8 de julho de 2013, bem como na Portaria Interministerial MS/MEC n� 1.369, de 8 de julho de 2013, resolve:</p>
<p>Art. 1� - Poder�o aderir ao Projeto Mais M�dicos as institui��es federais de educa��o superior que ofere�am curso de Medicina.</p>
<p>� 1� - As institui��es federais de educa��o superior interessadas em aderir ao Projeto Mais M�dicos dever�o apresentar termo de pr�-ades�o, conforme o modelo do <a href="javascript:abreLinkAnexo(<?php echo $entid ?>)" >Anexo I</a> desta Portaria, no per�odo de 11 a 15 de julho de 2013, ao Minist�rio da Educa��o.</p>
<p>� 2� - As institui��es dever�o indicar, no momento da pr�ades�o, um tutor acad�mico respons�vel pelas atividades e, no m�nimo, tr�s tutores acad�micos para fins de cadastro de reserva, que atendam aos requisitos da Portaria Interministerial MS/MEC n� 1.369, de 8 de julho de 2013 e desta Portaria.</p>
<p>� 3� - As institui��es dever�o cadastrar via sistema SIMEC, no m�dulo rede federal, por meio do endere�o eletr�nico <a href="http://simec.mec.gov.br" target="_blank">http://simec.mec.gov.br</a>, os tutores indicados no termo de pr�-ades�o.</p>
<p>� 4� - No momento da pr�-ades�o as institui��es dever�o indicar a unidade respons�vel pela avalia��o e autoriza��o de pagamento das bolsas de tutoria e supervis�o acad�micas.</p>
<p>Art. 2� - O Minist�rio da Educa��o decidir� sobre a valida��o do termo de pr�-ades�o das institui��es que atenderem aos requisitos previstos no art. 1� desta Portaria, observadas as necessidades do Projeto Mais M�dicos.</p>
<p>Par�grafo �nico - Em caso de manifesta��o de interesse de mais de uma institui��o por unidade da federa��o, ser� dada prefer�ncia �quela sediada na capital, caso persista o empate, ser� selecionada �quela que ofertar curso de Medicina h� mais tempo.</p>
<p>Art. 3� - As institui��es que tiverem seus termos de pr�-ades�o validados pelo Minist�rio da Educa��o dever�o firmar termo de ades�o no prazo m�ximo de 10 (dez) dias ap�s a divulga��o das institui��es selecionadas.</p>
<p>Par�grafo �nico - O termo de ades�o estar� dispon�vel para assinatura das institui��es selecionadas no sistema SIMEC, no m�dulo rede federal, por meio do endere�o eletr�nico <a href="http://simec.mec.gov.br" target="_blank">http://simec.mec.gov.br</a>, e conter�, no m�nimo, as seguintes obriga��es para a institui��o:</p>
<p>I - atuar em coopera��o com os entes federativos, as Coordena��es Estaduais do Projeto e organismos internacionais, no �mbito de sua compet�ncia, para execu��o do Projeto Mais M�dicos;</p>
<p>II - coordenar o acompanhamento acad�mico do Projeto;</p>
<p>III - ratificar a unidade respons�vel pela avalia��o e autoriza��o de pagamento das bolsas de tutoria e supervis�o acad�micas, indicada no termo de pr�-ades�o;</p>
<p>IV - definir mecanismo de avalia��o e autoriza��o de pagamento das bolsas de tutoria e supervis�o;</p>
<p>V - ratificar a indica��o dos tutores acad�micos do Projeto, feita no termo de pr�-ades�o;</p>
<p>VI - definir crit�rios e mecanismo de sele��o de supervisores;</p>
<p>VII - realizar sele��o dos supervisores do Projeto;</p>
<p>VIII - monitorar e acompanhar as atividades dos supervisores e tutores acad�micos no �mbito do Projeto;</p>
<p>IX - ofertar os m�dulos de acolhimento e avalia��o aos m�dicos intercambistas; e</p>
<p>X - ofertar cursos de especializa��o e atividades de pesquisa, ensino e extens�o aos m�dicos participantes.</p>
<p>Art. 4� - Os tutores acad�micos ser�o selecionados pela institui��o entre os docentes da �rea m�dica, preferencialmente vinculados � �rea de sa�de coletiva ou correlata, ou � �rea de cl�nica m�dica.</p>
<p>� 1� - Os tutores acad�micos perceber�o bolsa-tutoria, na forma prevista no termo de ades�o.</p>
<p>� 2� - Os tutores acad�micos ser�o respons�veis pela orienta��o acad�mica e pelo planejamento das atividades do supervisor, trabalhando em parceria com as Coordena��es Estaduais do Projeto, e tendo, no m�nimo, as seguintes atribui��es:</p>
<p>I - coordenar as atividades acad�micas da integra��o ensinoservi�o, atuando em coopera��o com os supervisores e os gestores do SUS;</p>
<p>II - indicar, em plano de trabalho, as atividades a serem executadas pelos m�dicos participantes e supervisores, bem como a metodologia de acompanhamento e avalia��o;</p>
<p>III - monitorar o processo de acompanhamento e avalia��o a ser executado pelos supervisores, garantindo sua continuidade;</p>
<p>IV - integrar as atividades do curso de especializa��o �s atividades de integra��o ensino-servi�o;</p>
<p>V - relatar � institui��o p�blica de ensino superior � qual esteja vinculado a ocorr�ncia de situa��es nas quais seja necess�ria a ado��o de provid�ncia pela institui��o; e</p>
<p>VI - apresentar relat�rios peri�dicos da execu��o de suas atividades no Projeto � institui��o � qual esteja vinculado e � Coordena��o do Projeto.</p>
<p>Art. 5� - Os supervisores ser�o selecionados entre profissionais m�dicos por meio de edital conforme crit�rios e mecanismos estabelecidos pela institui��o aderente e validados pela Coordena��o Estadual do Projeto Mais M�dicos.</p>
<p>� 1� - Os supervisores selecionados perceber�o bolsa, conforme avalia��o e autoriza��o das institui��es aderentes, na forma prevista no termo de ades�o.</p>
<p>� 2� - Os supervisores selecionados ser�o respons�veis pelo acompanhamento e fiscaliza��o das atividades de ensino-servi�o do m�dico participante, em conjunto com o gestor do SUS no Munic�pio, e ter�o, no m�nimo, as seguintes atribui��es:</p>
<p>I - realizar visita peri�dica para acompanhar atividades dos m�dicos participantes;</p>
<p>II - estar dispon�vel para os m�dicos participantes, por meio de telefone e internet;</p>
<p>III - aplicar instrumentos de avalia��o presencialmente; e</p>
<p>IV - acompanhar e fiscalizar, em conjunto com o gestor do SUS, o cumprimento da carga hor�ria de 40 horas semanais prevista pelo Projeto para os m�dicos participantes, por meio de sistema de informa��o disponibilizado pela Coordena��o do Programa.</p>
<p>Art. 6� - Esta Portaria entra em vigor na data de sua publica��o.</p>
<p>ALOIZIO MERCADANTE OLIVA</p>
<?php
}

function textoAdesaoHTML2($entid)
{
	global $db;
	$sql = "select * from entidade.entidade  ent
	inner join entidade.endereco ende ON ende.entid = ent.entid
	inner join territorios.municipio mun ON mun.muncod = ende.muncod
	where ent.entid = $entid";
	$arrDados = $db->pegaLinha($sql);

	$sql = "select
	usunome as nome
	from
	seguranca.usuario usu
	inner join
	maismedicos.usuarioresponsabilidade ure ON ure.usucpf = usu.usucpf
	where
	ure.entid = $entid
	and
	ure.pflcod = ".PERFIL_REITOR."
	and
				ure.rpustatus = 'A'
			and
				usu.usustatus = 'A'
			order by
				rpudata_inc desc";
	$arrReitor = $db->pegaLinha($sql);
	if(!$arrReitor){
		$sql = "SELECT
		DISTINCT fun.funid, fun.fundsc, ent.entnome as nome, ent.entid
		FROM
		entidade.funcao fun
		LEFT JOIN
		entidade.funcaoentidade fen ON fen.funid = fun.funid
		AND fen.entid IN (SELECT
		fen2.entid
		FROM
		entidade.funentassoc fea2
		LEFT JOIN
		entidade.funcaoentidade fen2 on fea2.fueid = fen2.fueid
		WHERE
		fea2.entid='$entid'
		AND fun.funid = fen2.funid)
		LEFT JOIN
		entidade.entidade ent ON fen.entid = ent.entid
		WHERE
		fun.funid IN('21')";
		$arrReitor = $db->pegaLinha($sql);
	}
	?>
<div style="text-align:justify;">
<center>
<h2>PORTARIA NORMATIVA N� 17, DE 31 DE JULHO DE 2013</h2>
<p><b>MINIST�RIO DA EDUCA��O</b></p>
<p><b>GABINETE DO MINISTRO</b></p>
</center>
<p>Disp�e sobre os procedimentos de ades�o das institui��es p�blicas estaduais e municipais de educa��o superior e de sa�de; programas de resid�ncia em Medicina de Fam�lia e Comunidade Medicina Preventiva e Social e Cl�nica M�dica; e de escolas de governo em sa�de p�blica ao Programa Mais M�dicos para o Brasil e d� outras provid�ncias.</p>
<p>O MINISTRO DE ESTADO DA EDUCA��O no uso da atribui��o que lhe confere o art. 87, inciso II da Constitui��o Federal, e tendo em vista o disposto na Medida Provis�ria no 621, de 8 de julho de 2013, bem como na Portaria Interministerial MS/MEC no 1.369, de 8 de julho de 2013, resolve:</p>
<p>Art. 1� Poder�o aderir ao Programa Mais M�dicos para o Brasil:</p>
<p>I - as institui��es p�blicas estaduais e municipais de educa��o superior, que ofere�am curso de Medicina gratuitamente;</p>
<p>II - os programas de resid�ncia em Medicina de Fam�lia e Comunidade, de Medicina Preventiva e Social e Cl�nica M�dica que estejam devidamente credenciados pela Comiss�o Nacional de Resid�ncia M�dica (CNRM);</p>
<p>III - as escolas de governo em sa�de p�blica, que possuam no m�nimo um programa resid�ncia m�dica ou de p�s-gradua��o na �rea de sa�de coletiva ou afins; e</p>
<p>IV - as secretarias municipais e estaduais de sa�de que tenham ao menos um programa de resid�ncia m�dica vinculado �s mesmas.</p>
<p>�1� As institui��es, escolas e programas de resid�ncia interessados em aderir ao Programa Mais M�dicos para o Brasil dever�o apresentar termo de pr�-ades�o, conforme o modelo do Anexo I desta Portaria, no per�odo de 05 a 12 de agosto de 2013, ao Minist�rio da Educa��o.</p>
<p>�2� As institui��es, escolas e programas de resid�ncia dever�o indicar, no momento da pr�-ades�o, um tutor acad�mico respons�vel pelas atividades e, no m�ximo, tr�s tutores acad�micos para fins de cadastro de reserva, que atendam aos requisitos da Portaria Interministerial MS/MEC n� 1.369, de 8 de julho de 2013 e desta Portaria.</p>
<p>�3� As institui��es, escolas e programas de resid�ncia dever�o enviar o termo de pr�-ades�o devidamente assinado pela autoridade local respons�vel pela institui��o, escola ou programa de resid�ncia, e digitalizado, at� as 23:59 hs do dia 12/08/2013, para o endere�o <?php echo $_SESSION['email_sistema']; ?>.</p>
<p>�4� As institui��es, escolas e programas de resid�ncia dever�o, no prazo estipulado no par�grafo anterior, enviar atrav�s de postagem pelo correio c�pia impressa e assinada do termo de pr�-ades�o, com aviso de recebimento (AR), para o endere�o Minist�rio da Educa��o, Edif�cio Sede, Bloco L, Esplanada dos Minist�rios, 3� Andar, Sala 303 CEP: 70047-900.</p>
<p>�5� No momento da pr�-ades�o institui��es, escolas e programas de resid�ncia dever�o indicar a unidade respons�vel pela avalia��o e autoriza��o de pagamento das bolsas de tutoria e supervis�o acad�micas.</p>
<p>Art. 2� O Minist�rio da Educa��o decidir� sobre a valida��o do termo de pr�-ades�o das institui��es, escolas e programas de resid�ncia que atenderem aos requisitos previstos no art. 1� desta Portaria, observadas as necessidades do Programa Mais M�dicos para o Brasil.</p>
<p>�1� Ser�o selecionadas institui��es, escolas e programas de resid�ncia apenas nas unidades da federa��o onde n�o houver ades�o de institui��o federal de educa��o superior, nos termos da Portaria Normativa n� 14, de 10 de julho de 2013.</p>
<p>�2� Em caso de manifesta��o de interesse de mais de uma institui��o, escola ou programa de resid�ncia por unidade da federa��o, ser� dada prefer�ncia �quele sediado na capital.</p>
<p>�3� Caso persista o empate, ser� selecionado aquele que ofertar programa de resid�ncia m�dica ou especializa��o na �rea de sa�de coletiva, medicina de fam�lia e comunidade ou �reas afins.</p>
<p>�4� Se ainda persistir o empate, ser� selecionado aquele programa de resid�ncia vinculado a institui��es estaduais e municipais de educa��o superior, de acordo com crit�rios do art. 1�.</p>
<p>�5� As institui��es, escolas e programas de resid�ncia n�o selecionados neste primeiro momento de pr�-ades�o ir�o compor um banco de entidades supervisoras, que poder�o ser mobilizadas a qualquer momento para composi��o do quadro de tutoria do Programa Mais M�dicos para o Brasil.</p>
<p>Art. 3� As institui��es, escolas e programas de resid�ncia que tiverem seus termos de pr�-ades�o validados pelo Minist�rio da Educa��o dever�o firmar termo de ades�o no prazo m�ximo de 10 (dez) dias ap�s a divulga��o das entidades selecionadas.</p>
<p>Par�grafo �nico. O termo de ades�o estar� dispon�vel para assinatura das institui��es, escolas e programas de resid�ncia selecionados por meio de comunica��o via endere�o eletr�nico e expediente de of�cio do MEC a ser enviado e conter�, no m�nimo, as seguintes obriga��es para a entidade:</p>
<p>I - atuar em coopera��o com os entes federativos, as Coordena��es Estaduais do Programa e organismos internacionais, no �mbito de sua compet�ncia, para execu��o do Programa Mais M�dicos para o Brasil;</p></p>
<p>II - coordenar o acompanhamento acad�mico do Programa;</p>
<p>III - ratificar a unidade respons�vel pela avalia��o e autoriza��o de pagamento das bolsas de tutoria e supervis�o acad�micas, indicada no termo de pr�-ades�o;</p>
<p>IV - definir mecanismo de avalia��o e autoriza��o de pagamento das bolsas de tutoria e supervis�o;</p>
<p>V - ratificar a indica��o dos tutores acad�micos do Programa, feita no termo de pr�-ades�o;</p>
<p>VI - definir crit�rios e mecanismo de sele��o de supervisores;</p>
<p>VII - realizar sele��o dos supervisores do Programa;</p>
<p>VIII - monitorar e acompanhar as atividades dos supervisores e tutores acad�micos no �mbito do Programa;</p>
<p>IX - ofertar os m�dulos de acolhimento e avalia��o aos m�dicos intercambistas; e</p>
<p>Art. 4� Os tutores acad�micos ser�o selecionados pela institui��es, escolas e programas de resid�ncia entre os docentes da �rea m�dica, preferencialmente vinculados � �rea de sa�de coletiva ou correlata, � �rea de medicina de fam�lia e comunidade, ou � �rea de cl�nica m�dica.</p>
<p>�1� Os tutores acad�micos perceber�o bolsa-tutoria, na forma prevista no termo de ades�o.</p>
<p>�2� Os tutores acad�micos ser�o respons�veis pela orienta��o acad�mica e pelo planejamento das atividades do supervisor, trabalhando em parceria com as Coordena��es Estaduais do Programa, e tendo, no m�nimo, as seguintes atribui��es:</p>
<p>I - coordenar as atividades acad�micas da integra��o ensinoservi�o, atuando em coopera��o com os supervisores e os gestores do SUS;</p>
<p>II - indicar, em plano de trabalho, as atividades a serem executadas pelos m�dicos participantes e supervisores, bem como a metodologia de acompanhamento e avalia��o;</p>
<p>III - monitorar o processo de acompanhamento e avalia��o a ser executado pelos supervisores, garantindo sua continuidade;</p>
<p>IV - integrar as atividades do curso de especializa��o �s atividades de integra��o ensino-servi�o;</p>
<p>V - relatar � institui��o ou escola � qual esteja vinculado a ocorr�ncia de situa��es nas quais seja necess�ria a ado��o de provid�ncia pela institui��o; e</p>
<p>VI - apresentar relat�rios peri�dicos da execu��o de suas atividades no Programa � institui��o � qual esteja vinculado e � Coordena��o do Programa.</p>
<p>Art. 5� Os supervisores ser�o selecionados entre profissionais m�dicos por meio de edital conforme crit�rios e mecanismos estabelecidos pelas institui��es, escolas e programas de resid�ncia aderente e validados pela Coordena��o Estadual do Programa Mais M�dicos para o Brasil.</p>
<p>�1� Os supervisores selecionados perceber�o bolsa, conforme avalia��o e autoriza��o das institui��es, escolas e programas de resid�ncia aderentes, na forma prevista no termo de ades�o.</p>
<p>�2� Os supervisores selecionados ser�o respons�veis pelo acompanhamento e fiscaliza��o das atividades de ensino-servi�o do m�dico participante, em conjunto com o gestor do SUS no Munic�pio, e ter�o, no m�nimo, as seguintes atribui��es:</p>
<p>I - realizar visita peri�dica para acompanhar atividades dos m�dicos participantes;</p>
<p>II - estar dispon�vel para os m�dicos participantes, por meio de telefone e internet;</p>
<p>III - aplicar instrumentos de avalia��o presencialmente; e</p>
<p>IV - acompanhar e fiscalizar, em conjunto com o gestor do SUS, o cumprimento da carga hor�ria de 40 horas semanais prevista pelo Programa para os m�dicos participantes, por meio de sistema de informa��o disponibilizado pela Coordena��o do Programa.</p>
<p>Art. 6� Os prazos desta Portaria poder�o ser alterados mediante ato do Secret�rio de Educa��o Superior.</p>
<p>Art. 7� Esta Portaria entra em vigor na data de sua publica��o.</p>
<p>ALOIZIO MERCADANTE OLIVA</p>
<?php
}

function documentoAdesao($uniid,$valida_eletronicamente = false)
{
	global $db;
	$sql = "select
				usu.usucpf,
				usu.usunome,
				usu.ususexo,
				uni.*,
				mun.mundescricao,
				mun.estuf,
				tpu.tpudsc
			from
				maismedicos.universidade uni
			inner join
				maismedicos.tipouniversidade tpu ON tpu.tpuid = uni.tpuid
			inner join
				maismedicos.usuarioresponsabilidade ure ON ure.uniid = uni.uniid
			inner join
				seguranca.usuario usu ON usu.usucpf = ure.usucpf
			inner join
				territorios.municipio mun ON mun.muncod = uni.muncod
			where
				ure.pflcod = '".PERFIL_REITOR."'
			and
				rpustatus = 'A'
			and
				uni.uniid = $uniid";
	$arrDados = $db->pegaLinha($sql);
	//dbg($arrDados);
?>
<div style="text-align:justify;font-size:11px">
<center>
<table width="100%" >
	<tr>
		<td><img src="../imagens/logo_brasil_mais_medicos.png" /></td>
		<td width="50%" ></td>
		<td><img src="../imagens/logo_mais_medicos.png" /></td>
	</tr>
</table>
<div><img src="../imagens/brasao_mais_medicos.JPG" /></div>

<h2>MINIST�RIO DA EDUCA��O</h2>
<h2>SECRETARIA DE EDUCA��O SUPERIOR</h2>
<h2>PROGRAMA MAIS M�DICOS</h2>
<h2>TERMO DE ADES�O AO PROGRAMA MAIS M�DICOS</h2>
</center>

<p>TERMO DE ADES�O E COMPROMISSO QUE ENTRE SI CELEBRAM O MINIST�RIO DA EDUCA��O E <?php echo $arrDados['uninome'] ?><?php echo $arrDados['unisigla'] ? " - ".$arrDados['unisigla'] : "" ?> PARA ADES�O � SUPERVIS�O ACAD�MICA DO PROGRAMA MAIS M�DICOS. O MINIST�RIO DA EDUCA��O, CNPJ  00.394.445/0003-65, neste ato representado por Jesualdo Pereira Farias, Secret�rio de Educa��o Superior, com endere�o na Esplanada dos Minist�rios, Bloco "L", 3� andar, sala 300 - CEP 70.047-900, Bras�lia (DF), e <?php echo $arrDados['uninome'] ?><?php echo $arrDados['unisigla'] ? " - ".$arrDados['unisigla'] : "" ?>, com sede na cidade de <?php echo $arrDados['mundescricao'] ?> - <?php echo $arrDados['estuf'] ?>, inscrita no CNPJ/MF sob o n� <?php echo mascara_global_maismedicos_tela($arrDados['unicnpj'],"##.###.###/####-##") ?>, doravante intitulada INSTITUI��O SUPERVISORA, neste ato representado por seu Reitor(a) <?php echo $arrDados['usunome'] ?>, nos termos da Lei n� 12.871, de 22 de outubro de 2013, da Portaria Interministerial n� 1.369/MS/MEC, de 8 de julho de 2013 e da Portaria Interministerial n� 2.087/MS/MEC, de 1� de setembro de 2011 resolvem celebrar o presente Termo de Ades�o e Compromisso para ades�o �  Supervis�o Acad�mica ao Programa Mais M�dicos, mediante as cl�usulas e condi��es seguintes:</p>

<p>CL�USULA PRIMEIRA - DO OBJETO</p>
<p>O presente Termo de Ades�o tem por objeto viabilizar a tutoria e supervis�o acad�mica a m�dicos formados em institui��es de educa��o superior brasileiras ou com diploma revalidado no Brasil e m�dicos formados em institui��es de educa��o superior estrangeiras, por meio de interc�mbio m�dico internacional inscritos no Projeto Mais M�dicos para o Brasil, nos termos da Lei 12.871/2013 e na Portaria Interministerial MS/MEC n� 1.369/2013.</p>
<p>CL�USULA SEGUNDA - DAS OBRIGA��ES</p>
<p>Para consecu��o do objeto do presente Termo a INSTITUI��O SUPERVISORA dever� efetuar procedimento de ades�o por meio do Sistema Integrado de Monitoramento do Minist�rio da Educa��o (SIMEC), com as credenciais do dirigente m�ximo da INSTITUI��O SUPERVISORA e compromete-se a assumir as seguintes obriga��es:</p>
<p>I - atuar em coopera��o com os entes federativos, as Coordena��es Estaduais do Projeto e institui��es internacionais, no �mbito de sua compet�ncia, para execu��o do Projeto Mais M�dicos para o Brasil;</p>
<p>II - seguir as orienta��es e diretrizes do Minist�rio da Educa��o, bem como receber representantes do mesmo nos espa�os de execu��o da supervis�o;</p>
<p>III - coordenar o acompanhamento acad�mico do Programa Mais M�dicos;</p>
<p>IV - ratificar a indica��o dos tutores acad�micos do Projeto Mais M�dicos para o Brasil;</p>
<p>V - acompanhar os mecanismos de avalia��o e autoriza��o de pagamento das bolsas de tutoria e supervis�o com o n�cleo gestor do Projeto Mais M�dicos para o Brasil;</p>
<p>VI - definir crit�rios e mecanismo de sele��o de supervisores, n�o ferindo regulamenta��o vigente do Projeto Mais M�dicos para o Brasil;</p>
<p>VII - realizar sele��o dos primeiros supervisores do Projeto, no per�odo de 30 (trinta) dias, a contar da data de assinatura do Termo de Ades�o. </p>
<p>VIII - estabelecer calend�rio de fluxo cont�nuo para sele��o de novos supervisores, conforme as necessidades expressas pela Diretoria de Desenvolvimento da Educa��o em Sa�de DDES/SESu/MEC;</p>
<p>IX - monitorar e acompanhar as atividades dos tutores e supervisores acad�micos e m�dicos participantes do Projeto;</p>
<p>X - Acompanhar o pagamento das bolsas de tutores e supervisores acad�micos;</p>
<p>XI - Apoiar a execu��o dos M�dulos de Acolhimento e Avalia��o aos m�dicos intercambistas no local indicado pela Coordena��o Nacional do Projeto;</p>
<p>XII - ofertar atividades de pesquisa, ensino e extens�o aos m�dicos participantes do Projeto Mais M�dicos para o Brasil; e</p>
<p>XIII - As institui��es interessadas tamb�m dever�o cadastrar 2 (dois) Tutores Acad�micos, dentre os profissionais com perfil docente da �rea m�dica, vinculado � mesma, e preferencialmente atuante em alguma das seguintes �reas de conhecimento: Sa�de Coletiva, Medicina de Fam�lia e Comunidade, Cl�nica M�dica, Pediatria, ou �reas afins.</p>
</p>XIV - Um dos tutores ser� ser� cadatrado para fins de cadastro reserva, atendendo aos requisitos da Portaria Interministerial MS/MEC n� 1.369, de 08 de julho de 2013, conforme procedimentos estabelecidos pela Diretoria de Desenvolvimento da Educa��o em Sa�de, da Secretaria de Educa��o Superior do Minist�rio da Educa��o.</p>
<p>CL�USULA TERCEIRA - DOS TUTORES ACAD�MICOS</p>
<p>O Tutor Acad�mico ser� indicado pela INSTITUI��O SUPERVISORA dentre os profissionais com perfil docente da �rea m�dica, vinculado � mesma, e preferencialmente atuante em alguma das seguintes �reas de conhecimento: Sa�de Coletiva, Medicina de Fam�lia e Comunidade, Cl�nica M�dica, Pediatria, ou �reas afins.</p>
<p>SUBCL�USULA 3.1</p>
<p>O Tutor Acad�mico � respons�vel pela orienta��o acad�mica e pelo planejamento das atividades do supervisor, observadas as orienta��es gerais da Diretoria de Desenvolvimento da Educa��o em Sa�de DDES/SESu/MEC.</p>
<p>SUBCL�USULA 3.2</p>
<p>O(s) Tutor(es) do cadastro reserva poder�(�o) ser convocado(s), de acordo com o n�mero de m�dicos selecionados para o Projeto Mais M�dicos para o Brasil, observada a propor��o de supervisores por Tutor definida pela Diretoria de Desenvolvimento da Educa��o em Sa�de DDES/SESu/MEC.</p>
<p>SUBCL�USULA 3.3</p>
<p>As Institui��es Supervisoras dever�o garantir a dispensa dos professores que atuar�o como Tutores Acad�micos, de atividades perante as mesmas, para o desempenho das atividades de tutoria de forma adequada, sem preju�zos de qualquer ordem para os mesmos.</p>
<p>SUBCL�USULA 3.4</p>
<p>As Institui��es Supervisoras dever�o computar a atividades de tutoria em seu plano institucional sem preju�zos para o docente designado.</p>
<p>CL�USULA QUARTA - DA BOLSA-TUTORIA</p>
<p>Para o desenvolvimento de suas atividades o Tutor Acad�mico receber� bolsa-tutoria no valor de R$ 5.000,00 (cinco mil reais) mensais, mediante cumprimento das respectivas atribui��es  durante o prazo de vincula��o ao Projeto Mais M�dicos para o Brasil.</p>
<p>CL�USULA QUINTA - DAS ATRIBUI��ES DO TUTOR ACAD�MICO</p>
<p>O tutor Acad�mico dever� seguir atribui��es estabelecidas na regulamenta��o vigente do Projeto conforme orienta��o da Coordena��o Nacional.</p>
<p>CL�USULA SEXTA - DA SELE��O DE SUPERVISORES</p>
<p>Os supervisores ser�o selecionados pela INSTITUI��O dentre profissionais m�dicos com perfil docente da �rea m�dica, vinculado � mesma, e preferencialmente atuante em alguma das seguintes �reas de conhecimento: Sa�de Coletiva, Medicina de Fam�lia e Comunidade, Cl�nica M�dica, Pediatria, ou �reas afins.</p>
<p>SUBCL�USULA 6.1</p>
<p>Os supervisores selecionados ser�o respons�veis pelo acompanhamento das atividades de integra��o ensino-servi�o do m�dico participante, em conjunto com o gestor do SUS no Munic�pio ou Coordenador de Distrito Sanit�rio Especial Ind�gena;</p>
<p>CL�USULA S�TIMA - DA BOLSA SUPERVIS�O</p>
<p>Os supervisores selecionados perceber�o bolsa no valor de R$ 4.000,00 (quatro mil reais) mensais, mediante cumprimento das atribui��es de supervis�o acad�mica e durante o prazo de vincula��o ao Projeto Mais M�dicos para o Brasil.</p>
<p>CL�USULA OITAVA - DAS ATRIBUI��ES DO SUPERVISOR</p>
<p>O tutor Acad�mico dever� seguir atribui��es estabelecidas na regulamenta��o vigente do Projeto conforme orienta��o da Coordena��o Nacional.</p>
<p>CL�USULA NONA - DA VIG�NCIA</p>
<p>O presente TERMO DE ADES�O ter� vig�ncia de 3 (tr�s) anos, podendo ser prorrogado por igual per�odo, respeitando o tempo de vig�ncia do Projeto Mais M�dicos para o Brasil.</p>
<p>CL�USULA D�CIMA - DISPOSI��ES FINAIS</p>
<p>As INSTITUI��ES SUPERVISORAS com ades�o ao Projeto Mais M�dicos para o Brasil, que manifestarem formalmente sua impossibilidade de atenderem aos determinantes deste Termo, dever�o encaminhar of�cio � DDES/SESu/MEC com o prazo de 30 (trinta) dias de anteced�ncia, para que se proceda seu desligamento perante o sistema SIMEC.</p>
<p>Compete � SESu/MEC decidir sobre eventuais casos omissos.</p>
<p>&nbsp;</p>
<center>
<p>Bras�lia, <?php echo date('d');?> de <?php echo mes_extenso(date('m'))?> de <?php echo date('Y'); ?>.</p>
<p>&nbsp;</p>
<p>___________________________</p>
<p><?php echo $arrDados['usunome'] ?></p>
<p><?php echo retornaNome($arrDados['tpuid'],$arrDados['ususexo']) ?> de <?php echo $arrDados['tpudsc']?></p>
<p><?php echo $arrDados['uninome'] ?><?php echo $arrDados['unisigla'] ? " - ".$arrDados['unisigla'] : "" ?></p>
</center>

<?php if ($valida_eletronicamente): ?>
	<p><b>VALIDA��O ELETR�NICA DO DOCUMENTO</b></p>
	<p><b>Validado por <?php echo $_SESSION['usunome'] ?> - CPF: <?php echo mascara_global_maismedicos_tela($_SESSION['usucpf'],"###.###.###-##") ?> em <?php echo date("d/m/Y H:m:s")?>.</b></p>
<?php endif; ?> 
</center>
<?php 

}

function retornaMesMaisMedicos($mes){

	switch ($mes){
		case '01' :
			return 'Janeiro';
			break;
		case '02' :
			return 'Fevereiro';
			break;
		case '03' :
			return 'Mar�o';
			break;
		case '04' :
			return 'Abril';
			break;
		case '05' :
			return 'Maio';
			break;
		case '06' :
			return 'Junho';
			break;
		case '07' :
			return 'Julho';
			break;
		case '08' :
			return 'Agosto';
			break;
		case '09' :
			return 'Setembro';
			break;
		case '10' :
			return 'Outubro';
			break;
		case '11' :
			return 'Novembro';
			break;
		case '12' :
			return 'Dezembro';
			break;
	}
}

function recuperaArquivo($arqid)
{
	global $db;
	$sql = "select * from public.arquivo where arqid = $arqid";
	return $db->pegaLinha($sql);
}

function mascara_global_maismedicos_tela($string,$mascara)
{
	$string = str_replace(" ","",$string);
	for($i=0;$i<strlen($string);$i++)
	{
		$mascara[strpos($mascara,"#")] = $string[$i];
	}
	return $mascara;
}

function comboBanco($bncid = null,$habilitado = "S")
{
	global $db;

	$bncid = $_POST['bncid'] ? $_POST['bncid'] : $bncid;

	$sql = "select
				bncid as codigo,
				bncdsc as descricao
			from
				maismedicos.banco
			where
				bncstatus = 'A'
			order by
				bncdsc";
	$db->monta_combo("bncid", $sql, $habilitado, "Selecione...", '', '', '', '', 'S','bncid');
}

function cabecalhoUniversidade($uniid)
{
	global $db;
	$sql = "select * from maismedicos.universidade uni left join territorios.municipio mun ON mun.muncod = uni.muncod where uni.uniid = $uniid";
	$arrDados = $db->pegaLinha($sql); ?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="subtituloDireita" width='25%' >Institui��o Supervisora</td>
			<td>
				<?php echo $arrDados['uninome'] ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" >CNPJ</td>
			<td>
				<?php echo formatar_cnpj($arrDados['unicnpj']) ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" >Localiza��o</td>
			<td>
				<?php echo $arrDados['mundescricao'] ?> - <?php echo $arrDados['estuf'] ?>
			</td>
		</tr>
	</table>
	<?php 
	
}

function importarDadosAcademico()
{
	global $db;
	
	$sql = "select * from academico.maismedicos"; //Espelho de Produ��o
	
	$arrDados = $db->carregar($sql);
	
	foreach($arrDados as $arrD){
		$arrCamposNaoInseridos = array();
		foreach($arrD as $chave => $valor){
			$arrCamposNaoInseridos[] = "msmid";
			//$arrCamposNaoInseridos[] = "arqidtermo"; //S� funciona no mesmo ambiente
			//$arrCamposNaoInseridos[] = "arqid"; //S� funciona no mesmo ambiente
			if(!in_array($chave,$arrCamposNaoInseridos)){				
				if($valor){
					$arrCampos[] = $chave;
					$arrValores[] = "'$valor'";
				}
			}
		}
		$sqlI.="insert into maismedicos.maismedicos (".implode(",",$arrCampos).") values (".implode(",",$arrValores).");<br/>";
		$arrCampos = array();
		$arrValores = array();
	}
	dbg($sqlI,1);
	
	//Tutores
	$sql = "select
			ent.entid as entidade,
			*
		from
			academico.maismedicosresponsabilidade tut
		inner join
			entidade.entidade ent ON ent.entid = tut.entidresponsavel
		left join
			entidade.endereco ende ON ende.entid = ent.entid";
	
	$arrDados = $db->carregar($sql);
	
	foreach($arrDados as $arrD){
	
		$arrCampos['entid'] = $arrD['entidunidade'];
		$arrCampos['tuttipo'] = "T";
		$arrCampos['tutcpf'] = $arrD['entnumcpfcnpj'];
		$arrCampos['tutnome'] = $arrD['entnome'];
		$arrCampos['tutdatanascimento'] = $arrD['entdatanasc'] ? $arrD['entdatanasc'] : null;
		$arrCampos['tutemail'] = $arrD['entemail'];
		$arrCampos['tuttelefone'] = "(".trim($arrD['entnumdddresidencial']).")".$arrD['entnumresidencial'];
		$arrCampos['tutcep'] = $arrD['endcep'];
		$arrCampos['tutlogradouro'] = $arrD['endlog'];
		$arrCampos['tutnumero'] = $arrD['endnum'];
		$arrCampos['tutcomplemento'] = $arrD['endcom'];
		$arrCampos['tutbairro'] = $arrD['endbai'];
		$arrCampos['estuf'] = $arrD['estuf'];
		$arrCampos['muncod'] = $arrD['muncod'];
	
		foreach($arrCampos as $chave => $valor){
			$arrColunas[] = $chave;
			if($valor){
				$arrValores[] = "'$valor'";
			}else{
				$arrValores[] = "null";
			}
		}
	
		$sqlI2.="insert into maismedicos.tutor (".implode(",",$arrColunas).") values (".implode(",",$arrValores).");<br/>";
		$arrCampos = array();
		$arrColunas = array();
		$arrValores = array();
	}
	
	$sql = "select distinct
			ure.*
		from
			academico.maismedicosresponsabilidade tut
		inner join
			entidade.entidade ent ON ent.entid = tut.entidresponsavel
		inner join
			academico.usuarioresponsabilidade ure ON ure.entid = tut.entidunidade
		where
			rpustatus = 'A'
		and
			pflcod = 526";
	
	$arrDados = $db->carregar($sql);
	
	
	foreach($arrDados as $arrD){
	
		$arrCampos['pflcod'] = "947";
		$arrCampos['usucpf'] = $arrD['usucpf'];
		$arrCampos['entid'] = $arrD['entid'];
	
		foreach($arrCampos as $chave => $valor){
			$arrColunas[] = $chave;
			if($valor){
				$arrValores[] = "'$valor'";
			}else{
				$arrValores[] = "null";
			}
		}
	
		$sqlI3.="insert into maismedicos.usuarioresponsabilidade (".implode(",",$arrColunas).") values (".implode(",",$arrValores).");
		insert into seguranca.usuario_sistema (usucpf,sisid,suscod,susstatus,pflcod) values ('{$arrD['usucpf']}','168','A','A','947');
		insert into seguranca.perfilusuario (usucpf,pflcod) values ('{$arrD['usucpf']}','947'); <br/>";
		$arrCampos = array();
		$arrColunas = array();
		$arrValores = array();
	}
		dbg($sqlI3);
	
	die;
}

function preencherComZero($valor,$tamanho)
{
	$tamanho_string = strlen($valor);
	if($tamanho_string < $tamanho){
		$i = $tamanho_string;
		for($i;$i<$tamanho;$i++){
			$valor = "0".$valor;
		}
	}
	return $valor;
}

function preencherComEspacoVazio($valor,$tamanho,$direita = false)
{
	$tamanho_string = strlen($valor);
	if($tamanho_string < $tamanho){
		$i = $tamanho_string;
		for($i;$i<$tamanho;$i++){
			if($direita){
				$valor = $valor." ";
			}else{
				$valor = " ".$valor;
			}			
		}
	}
	return $valor;
}

function modulo11($num, $base=9, $r=0)
{
	/**
	 *   Autor:
	 *           Pablo Costa <hide@address.com>
	 *
	 *
	 *   Entrada:
	 *     $num: string num�rica para a qual se deseja calcularo digito verificador;
	 *     $base: valor maximo de multiplicacao [2-$base]
	 *     $r: quando especificado um devolve somente o resto
	 *
	 *   Sa�da:
	 *     Retorna o Digito verificador.
	 *
	 */
	$soma = 0;
	$fator = 2;

	/* Separacao dos numeros */
	for ($i = strlen($num); $i > 0; $i--) {
		// pega cada numero isoladamente
		$numeros[$i] = substr($num,$i-1,1);
		// Efetua multiplicacao do numero pelo falor
		$parcial[$i] = $numeros[$i] * $fator;
		// Soma dos digitos
		$soma += $parcial[$i];
		if ($fator == $base) {
			// restaura fator de multiplicacao para 2
			$fator = 1;
		}
		$fator++;
	}

	/* Calculo do modulo 11 */
	if ($r == 0) {
		$soma *= 10;
		$digito = $soma % 11;
		if ($digito == 10) {
			$digito = 0;
		}
		return $digito;
	} elseif ($r == 1){
		$resto = $soma % 11;
		return $resto;
	}
}

function removeCaracteres($string,$arrCaracteres = array())
{
	return str_replace($arrCaracteres,"",$string);
}

function removeAcentosRemessa($str){
	$str = trim($str);
	$str = strtr($str,"��������������������������������������������������������������!@#%&*()[]{}+=?",
			"YuAAAAAAACEEEEIIIIDNOOOOOOUUUUYsaaaaaaaceeeeiiiionoooooouuuuyy_______________");
	$str = str_replace("..","",str_replace("/","",str_replace("\\","",str_replace("\$","",$str))));
	return $str;
}

function verificaTransmissaoLoteDataAtual()
{
	global $db;
	$sql = "select count(*) from maismedicos.remessacabecalho where to_char(dt_hora,'DD/MM/YYYY') = to_char(now(),'DD/MM/YYYY')";
	return ($db->pegaUm($sql)+1);
}

function verificaTransmissaoLote()
{
	global $db;
	$sql = "select count(*) from maismedicos.remessacabecalho";
	return ($db->pegaUm($sql)+1);
}

function retornaNome($tpuid,$sexo = "M")
{
	switch($tpuid)
	{
		case TPUID_UNIVERSIDADE;
			$nome = "Reitor".($sexo == "F" ? "a" : "");
			break;
		case TPUID_INSTITUICAO;
			$nome = "Dirigente M�xim".($sexo == "F" ? "a" : "o");
			break;
		case TPUID_ESCOLA;
			$nome = "Dirigente M�xim".($sexo == "F" ? "a" : "o");
			break;
		case TPUID_PROGRAMA;
			$nome = "Supervisor".($sexo == "F" ? "a" : "");
			break;
		case TPUID_COMISSAO;
			$nome = "Presidente";
			break;
		default:
			$nome = "Reitor".($sexo == "F" ? "a" : "");
	}
	return $nome;
}

function filtrarDemandaMaisMedicos()
{
	global $db;
	
	if($_POST['uf_campo_flag'] == "1" && $_POST['uf'][0]){
		$arrWhere[] = "est.estuf in ('".implode("','",$_POST['uf'])."')";
	}
	if($_POST['uniid_campo_flag'] == "1" && $_POST['uniid'][0]){
		$arrWhere[] = "uni.uniid in ('".implode("','",$_POST['uniid'])."')";
	}
	if($_POST['tpmid_campo_flag'] == "1" && $_POST['tpmid'][0]){
		$arrWhere[] = "tpm.tpmid in ('".implode("','",$_POST['tpmid'])."')";
	}
	if($_POST['muncod_campo_flag'] == "1" && $_POST['muncod'][0]){
		$arrWhere[] = "mun.muncod in ('".implode("','",$_POST['muncod'])."')";
	}
	
	$sql = "select 
				est.estuf,
				est.estdescricao as estado,
				tpm.tpmid,
				tpm.tpmdsc as regiaosaude,
				mun.mundescricao as municipio,
				mun.muncod as ibge,
				count(mdcid) as qtde_medicos,
				uni.uniid,
				case when uni.unisigla is not null
					then uni.unisigla || ' - ' || uni.uninome
					else uni.uninome
				end as uninome,
				(select count(distinct tut.tutid) from maismedicos.tutor tut where tut.uniid = uni.uniid and tut.tuttipo = 'T' and tut.tutstatus = 'A' and tut.tutvalidade is true) as qtde_tutores,
				(select count(distinct sup.tutid) from maismedicos.tutor sup where sup.uniid = uni.uniid and sup.tuttipo = 'S' and sup.tutstatus = 'A' and sup.tutvalidade is true) as qtde_supervisores
			from
				maismedicos.universidademunicipio unm
			inner join
				maismedicos.universidade uni ON uni.uniid = unm.uniid
			inner join
				territorios.municipio mun ON mun.muncod = unm.muncod
			inner join
				territorios.estado est ON est.estuf = mun.estuf
			inner join
				territoriosgeo.muntipomunicipio mtm ON mtm.muncod = mun.muncod
			inner join
				territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid
			inner join
				maismedicos.medico med ON med.muncod = mun.muncod
			where
				tpm.gtmid = 1
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			group by
				est.estuf,est.estdescricao,tpm.tpmid,tpm.tpmdsc,mun.muncod,mun.mundescricao,uni.uninome,uni.unisigla,uni.uniid
			order by
				est.estuf,tpm.tpmdsc,mun.mundescricao";
	
	$arrDados = $db->carregar($sql);
	if($arrDados){
		foreach($arrDados as $dado)
		{
			$arrEstado[$dado['estuf']]['estuf'] = $dado['estuf'];
			$arrEstado[$dado['estuf']]['estado'] = $dado['estado'];
			$arrEstado[$dado['estuf']]['regiaoes_saude'][$dado['tpmid']]['regiaosaude'] = $dado['regiaosaude'];
			$arrEstado[$dado['estuf']]['regiaoes_saude'][$dado['tpmid']]['total_medicos'] += $dado['qtde_medicos'];
			$arrEstado[$dado['estuf']]['municipios'][$dado['ibge']] = $dado['municipio'];
			$arrEstado[$dado['estuf']]['universidades'][$dado['uniid']]['uninome'] = $dado['uninome'];
			$arrEstado[$dado['estuf']]['universidades'][$dado['uniid']]['qtde_tutores'] = $dado['qtde_tutores'];
			$arrEstado[$dado['estuf']]['universidades'][$dado['uniid']]['qtde_supervisores'] = $dado['qtde_supervisores'];
			$arrEstado[$dado['estuf']]['universidades'][$dado['uniid']]['qtde_medicos'] += $dado['qtde_medicos'];
			$arrEstado[$dado['estuf']]['municipios_por_regiao_saude'][$dado['tpmid']][$dado['ibge']]['municipio'] = $dado['municipio'];
			$arrEstado[$dado['estuf']]['municipios_por_regiao_saude'][$dado['tpmid']][$dado['ibge']]['qtde_medicos'] = $dado['qtde_medicos'];
			$arrEstado[$dado['estuf']]['total_medicos'] += $dado['qtde_medicos'];
		}

		foreach($arrEstado as $estuf => $arrD){

			$supervisores = recuperaTotalSupervisoresPorUf($arrD['estuf']);
			$medicos = $arrD['total_medicos'];
			
			$proporcao1 = $_POST['proporcao1'] ? $_POST['proporcao1'] : 1;
			$proporcao2 = $_POST['proporcao2'] ? $_POST['proporcao2'] : 10;
			/*
			 * Existem 7 supervisores
			* Existem 91 m�dicos
			*
			* F�rmula 1 : para cada 98 m�dico, deve haver 1 , portanto, sobram 6 supervisores
			* F�rmula 2 : para cada 10 m�dico, deve haver 1 m�dico, portanto, faltam 3 supervisores
			*/
			$deficit = $proporcao2*$supervisores;
			$deficit = $medicos - $deficit;
			$deficit = $deficit/$proporcao2;
			if($deficit < 0){
				$deficit = $deficit * (-1);
				$deficit = floor($deficit);
				$sinal = "+";
			}elseif($deficit == 0){
				$sinal = "";
			}else{
				$sinal = "-";
				$deficit = ceil($deficit);
			}
			$style_siplay = "";
			if($sinal == "-"){
				if(strlen($_POST['deficil_inicio']) > 0 && !$_POST['deficil_fim']){
					if($deficit == $_POST['deficil_inicio']){
						$style_siplay = "";
					}else{
						$style_siplay = "none";
					}
				}
				if(strlen($_POST['deficil_inicio'])> 0 && $_POST['deficil_fim']){
					if($_POST['deficil_inicio'] <= $deficit && $_POST['deficil_fim'] >= $deficit){
						$style_siplay = "";
					}else{
						$style_siplay = "none";
					}
				}
				if(strlen($_POST['excedente_inicio'])> 0 && strlen($_POST['deficil_inicio']) == 0){
					$style_siplay = "none";
				}
			}else{
				if(strlen($_POST['excedente_inicio']) && !$_POST['excedente_fim']){
					if($deficit == $_POST['excedente_inicio']){
						$style_siplay = "";
					}else{
						$style_siplay = "none";
					}
				}
				if(strlen($_POST['excedente_inicio'])> 0 && $_POST['excedente_fim']){
					if($_POST['excedente_inicio'] <= $deficit && $_POST['excedente_fim'] >= $deficit){
						$style_siplay = "";
					}else{
						$style_siplay = "none";
					}
				}
				if(strlen($_POST['deficil_inicio'])> 0 && strlen($_POST['excedente_inicio']) == 0 ){
					$style_siplay = "none";
				}
			}
			 ?>
			 <?php if($style_siplay == ""): ?>
				<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
					<tr>
						<td colspan="2" bgcolor="#DCDCDC" align="center">
							<b><?php echo $arrD['estuf'] ?> - <?php echo $arrD['estado'] ?></b> 
						</td>
					</tr>
					<tr>
						<td width="25%" class="subtituloDireita" >Qtde. Tutores Validados:</td>
						<td><?php echo number_format(recuperaTotalTutoresPorUf($arrD['estuf']),0,",",".") ?></td>
					</tr>
						<td class="subtituloDireita" >Qtde. Supervisores Validados:</td>
						<td><?php echo number_format($supervisores,0,",",".") ?></td>
					</tr>
					</tr>
						<td class="subtituloDireita" >Qtde. de M�dicos Ativos na �rea de Atua��o:</td>
						<td><?php echo number_format($medicos,0,",",".") ?></td>
					</tr>
					</tr>
						<td class="subtituloDireita" ><?php echo $sinal == "-" || $deficit == "0" ? "D�ficit" : "Excedente"?> de Supervisores na �rea de Atua��o:</td>
						<td>
							<?php echo ($deficit == "0" ? "" : $sinal).$deficit ?>
						</td>
					</tr>
					<?php foreach($arrEstado[$arrD['estuf']]['universidades'] as $uniid => $uni): ?>
						<?php 
						$deficit_uni = $proporcao2*$uni['qtde_supervisores'];
						$deficit_uni = $uni['qtde_medicos'] - $deficit_uni;
						$deficit_uni = $deficit_uni/$proporcao2;
						if($deficit_uni < 0){
							$deficit_uni = $deficit_uni * (-1);
							$deficit_uni = floor($deficit_uni);
							$sinal_uni = "+";
						}elseif($deficit_uni == 0){
							$sinal_uni = "";
						}else{
							$sinal_uni = "-";
							$deficit_uni = ceil($deficit_uni);
						}
						?>
						<tr bgcolor="#d5d5d5">
							<td class="subtituloDireita" ><b>Insitui��o Supervisora:</b></td>
							<td><?php echo $uni['uninome'] ?></td>
						</tr>
						<tr>
							<td width="25%" class="subtituloDireita" >Qtde. Tutores Validados:</td>
							<td><?php echo number_format($uni['qtde_tutores'],0,",",".") ?></td>
						</tr>
							<td class="subtituloDireita" >Qtde. Supervisores Validados:</td>
							<td><?php echo number_format($uni['qtde_supervisores'],0,",",".") ?></td>
						</tr>
						</tr>
							<td class="subtituloDireita" >Qtde. de M�dicos Ativos na �rea de Atua��o:</td>
							<td><?php echo number_format($uni['qtde_medicos'],0,",",".") ?></td>
						</tr>
						</tr>
							<td class="subtituloDireita" ><?php echo $sinal_uni == "-" || $deficit_uni == "0" ? "D�ficit" : "Excedente"?> de Supervisores na �rea de Atua��o:</td>
							<td>
								<?php echo ($deficit_uni == "0" ? "" : $sinal_uni).$deficit_uni ?>
							</td>
						</tr>
					<?php endforeach; ?>
				</table>
				<table class="listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" width="95%" >
					<tr>
						<th>Regi�o da Sa�de</th>
						<th>IBGE</th>
						<th>Munic�pio</th>
						<th>Qtde. M�dicos</th>
					</tr>
					<?php if($arrEstado[$arrD['estuf']]['regiaoes_saude']): ?>
						<?php $n=0; foreach($arrEstado[$arrD['estuf']]['regiaoes_saude'] as $tpmid => $arrReg): ?>
							<?php $cor = $n%2 ? "#fff" : ""?>
							<tr onmouseout="this.bgColor='<?php echo $cor ?>';" onmouseover="this.bgColor='#ffffcc';" bgcolor="<?php echo $cor ?>" >
								<td><img class="link" src="../imagens/mais.gif" id="img_reg_<?php echo $tpmid ?>" onclick="abreMunicipios('<?php echo $tpmid ?>')"  /> <?php echo $arrReg['regiaosaude'] ?></td>
								<td>-</td>
								<td><?php echo number_format(count($arrEstado[$arrD['estuf']]['municipios_por_regiao_saude'][$tpmid]),0,",",".").(count($arrEstado[$arrD['estuf']]['municipios']) != 1 ? " munic�pios" : " munic�pio") ?></td>
								<td class="number" ><?php echo number_format($arrReg['total_medicos'],0,",",".") ?></td>
							</tr>
							<?php if($arrEstado[$arrD['estuf']]['municipios_por_regiao_saude'][$tpmid]): ?>
								<?php $y=0;foreach($arrEstado[$arrD['estuf']]['municipios_por_regiao_saude'][$tpmid] as $ibge => $arrMun): ?>
									<?php $cor = $y%2 ? "#f9f9f9" : "#f3f3f3" ?>
									<tr onmouseout="this.bgColor='<?php echo $cor ?>';" onmouseover="this.bgColor='#ffffcc';" bgcolor="<?php echo $cor ?>" class="tr_mun_<?php echo $tpmid ?>" style="display:none"  >
										<td><img style="margin-left:30px" src="../imagens/seta_filho.gif" /></td>
										<td><?php echo $ibge ?></td>
										<td><?php echo $arrMun['municipio'] ?></td>
										<td class="number" ><?php echo number_format($arrMun['qtde_medicos'],0,",",".") ?></td>
									</tr>
								<?php $y++;endforeach; ?>
							<?php endif; ?>
						<?php $n++;endforeach; ?>
							<tr bgcolor="#CCCCCC" >
								<td class="bold" >Total</td>
								<td class="bold" >-</td>
								<td class="bold" ><?php echo number_format(count($arrEstado[$arrD['estuf']]['municipios']),0,",",".").(count($arrEstado[$arrD['estuf']]['municipios']) != 1 ? " munic�pios" : " munic�pio") ?></td>
								<td class="bold number" ><?php echo number_format($arrEstado[$arrD['estuf']]['total_medicos'],0,",",".") ?></td>
							</tr>
					<?php else: ?>
						<tr>
							<td colspan="4" >N�o existem registros.</td>
						</tr>
					<?php endif; ?>
				</table> <br/>
			<?php endif; ?>
			<?php
		}
	}
}


function recuperaTotalMedicosPorUf($estuf)
{
	global $db;
	$sql = "select 
				count(mdcid)
			from
				maismedicos.medico med 
			inner join
				territorios.municipio mun on mun.muncod = med.muncod
			where
				mun.estuf = '$estuf'";
	return $db->pegaUm($sql);
}
?>