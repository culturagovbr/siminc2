<?php
header( 'Content-Type: text/html; charset=ISO-8859-1' );
//header( 'Content-Type: text/html; charset=UTF-8' );

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );


error_reporting( E_ALL ^ E_NOTICE );

ini_set("memory_limit", "1024M");
set_time_limit(0);


$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

// carrega as fun��es gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . "www/sispacto/_constantes.php";
require_once APPRAIZ . "www/sispacto/_funcoes.php";
require_once APPRAIZ . "www/sispacto/_funcoes_coordenadorlocal.php";

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';


// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';


   
// abre conex��o com o servidor de banco de dados
$db = new cls_banco();

/*
$sql = "SELECT i.iusd, uu.unisigla||' - '||uu.uninome as universidade, iusnome, iusemailprincipal, iustermocompromisso, iuscpf FROM sispacto.identificacaousuario i
		INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
		INNER JOIN sispacto.universidadecadastro u ON u.uncid = i.uncid 
		INNER JOIN sispacto.universidade uu ON uu.uniid = u.uniid 
		WHERE t.pflcod='849' AND i.iusstatus='A' AND i.uncid IN(
SELECT uncid from (
SELECT uncid, count(distinct f.fpbid) as n FROM sispacto.folhapagamentouniversidade f 
INNER JOIN sispacto.folhapagamento ff ON ff.fpbid = f.fpbid 
WHERE to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd')
group by uncid
) foo where foo.n=12)";

$identificacaousuario = $db->carregar($sql);

if($identificacaousuario[0]) {
	echo "<table>";
	foreach($identificacaousuario as $iu) {

		unset($texto);

		if($iu['iustermocompromisso']!='t') {
			$texto[] = "- Preenchimento dos dados (inclusive a aceita��o do termo de compromisso) para o recebimento da bolsa n�o foi realizado";
		}

		$preencheu_material = $db->pegaUm("SELECT mapid FROM sispacto.materiaisprofessores WHERE iusd='".$iu['iusd']."'");

		if(!$preencheu_material) {
			$texto[] = "- As informa��es sobre o recebimento dos materiais n�o foram preenchidas";
		}
		
		
		$npreencheu_turmas = $db->pegaUm("SELECT tpaid FROM sispacto.turmasprofessoresalfabetizadores 
										  WHERE tpatotalmeninos is null AND tpastatus='A' AND (tpaconfirmaregencia IS NULL OR tpaconfirmaregencia=true) AND iusd='".$iu['iusd']."'");
		
		if($npreencheu_turmas) {
			$texto[] = "- As informa��es sobre os dados das turmas n�o foram cadastrados";
		}
		
		$npreencheu_conhecimento = $db->pegaUm("SELECT m.tpaid FROM sispacto.turmasprofessoresalfabetizadores m  
										 		LEFT JOIN sispacto.aprendizagemconhecimentoturma aa ON aa.tpaid = m.tpaid
										 		WHERE aa.actid is null and tpaconfirmaregencia=true AND m.tpastatus='A' AND m.iusd='".$iu['iusd']."'");
		
		if($npreencheu_conhecimento) {
			$texto[] = "- As informa��es sobre os dados da aprendizagem das turmas n�o foram cadastrados";
		}
		
		if($texto) {
			
			echo "<tr>";
			echo "<td>".$iu['iuscpf']."</td>";
			echo "<td>".$iu['iusnome']."</td>";
			echo "<td>".$iu['iusemailprincipal']."</td>";
			echo "<td>".implode(";<br>",$texto)."</td>";
			echo "<td>".$iu['universidade']."</td>";
			echo "</tr>";
			
			$count++;
			
			$mensagem = new PHPMailer();
			$mensagem->persistencia = $db;
			
			$mensagem->Host         = "localhost";
			$mensagem->Mailer       = "smtp";
			$mensagem->FromName		= SIGLA_SISTEMA;
			$mensagem->From 		= "noreply@mec.gov.br";
			$mensagem->Subject 		= SIGLA_SISTEMA. " - Finaliza��o do SISPACTO 2013";
			
			$mensagem->AddAddress( $iu['iusemailprincipal'], $iu['iusnome'] );
			
				
			$mensagem->Body = "<p>Prezado(a) {$iu['iusnome']},</p>
<p>No dia <b>21 de mar�o de 2014</b>, o SisPacto 2013 ser� encerrado. Ap�s esta data, n�o ser� mais poss�vel processar informa��es relativas a bolsas de estudo pendentes. Neste sentido, constatamos que Vossa Senhoria n�o realizou as atividades necess�rias ao recebimento de todas as bolsas de estudo, conforme descrito abaixo:</p>

<p>
<b>Atividades pendentes:</b><br><br>
".implode(";<br>",$texto)."
</p>

<p>Caso seja do seu interesse receber as bolsas de estudo n�o encaminhadas para pagamento, solicitamos o m�ximo de empenho para resolu��o das pend�ncias descritas acima at� a data informada (21/03/2014), caso contr�rio, n�o ser� mais poss�vel processar as solicita��es de pagamento. A previs�o de cr�dito em conta das bolsas pendentes de 2013 � Junho/2014.</p>
<p>Em caso de d�vida, sugerimos que entre em contato com o(a) seu(sua) Coordenador(a) local ou com a Institui��o de Ensino Superior (IES) respons�vel pela forma��o. Permanecendo as d�vidas ou problemas, por favor, envie um e-mail para ". $_SESSION['email_sistema']. ".</p> 

<p>Secretaria de Educa��o B�sica<br>
Minist�rio da Educa��o</p>
			";
			
			$mensagem->IsHTML( true );
			$resp = $mensagem->Send();
			
			
		}

	}
	
	echo "</table>";
	echo "<p><b>Professores Alfabetizadores com restri��es: {$count}</b></p>";
}


$sql = "SELECT i.iusd, uu.unisigla||' - '||uu.uninome as universidade, iusnome, iusemailprincipal, iustermocompromisso, iuscpf FROM sispacto.identificacaousuario i
		INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd
		INNER JOIN sispacto.universidadecadastro u ON u.uncid = i.uncid
		INNER JOIN sispacto.universidade uu ON uu.uniid = u.uniid
		WHERE t.pflcod='827' AND i.iusstatus='A' AND i.iuscpf NOT ILIKE 'SIS%' AND i.uncid IN(
SELECT uncid from (
SELECT uncid, count(distinct f.fpbid) as n FROM sispacto.folhapagamentouniversidade f 
INNER JOIN sispacto.folhapagamento ff ON ff.fpbid = f.fpbid 
WHERE to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd')
group by uncid
) foo where foo.n=12)";

$identificacaousuario = $db->carregar($sql);

if($identificacaousuario[0]) {
	echo "<table>";
	foreach($identificacaousuario as $iu) {

		unset($texto);

		if($iu['iustermocompromisso']!='t') {
			$texto[] = "- Preenchimento dos dados (inclusive a aceita��o do termo de compromisso) para o recebimento da bolsa n�o foi realizado";
		}
		
		$sql = "SELECT lpad(fpbmesreferencia::text, 2, '0') as mes, fpbanoreferencia::text as ano, d.esdid FROM sispacto.mensario m 
				INNER JOIN workflow.documento d ON d.docid = m.docid 
				INNER JOIN sispacto.folhapagamento p ON p.fpbid = m.fpbid  
				WHERE m.iusd='".$iu['iusd']."' ORDER BY p.fpbid LIMIT 11";
		
		$m = $db->carregar($sql);
		
		if($m[0]) {
			
			$sql = "SELECT count(*) as x FROM sispacto.turmas t 
					INNER JOIN sispacto.orientadorturma ot ON ot.turid = t.turid 
					WHERE t.iusd='".$iu['iusd']."'";
			
			$nturma = $db->pegaUm($sql);
			
			foreach($m as $mm) {
				if($mm['esdid']==588 && $nturma > 0) {
					$texto[] = "- Voc� n�o finalizou a avalia��o referente a {$mm['mes']}/{$mm['ano']}";
				}
			}
			
		} else {
			$texto[] = "- Voc� n�o realizou nenhuma avalia��o, acesse o sistema e avalie seus Professores Alfabetizadores";
		}



		if($texto) {
				
			echo "<tr>";
			echo "<td>".$iu['iuscpf']."</td>";
			echo "<td>".$iu['iusnome']."</td>";
			echo "<td>".$iu['iusemailprincipal']."</td>";
			echo "<td>".implode(";<br>",$texto)."</td>";
			echo "<td>".$iu['universidade']."</td>";
			echo "</tr>";
				
			$count++;
				
			$mensagem = new PHPMailer();
			$mensagem->persistencia = $db;
				
			$mensagem->Host         = "localhost";
			$mensagem->Mailer       = "smtp";
			$mensagem->FromName		= SIGLA_SISTEMA;
			$mensagem->From 		= "noreply@mec.gov.br";
			$mensagem->Subject 		= SIGLA_SISTEMA. " - Finaliza��o do SISPACTO 2013";
				
			$mensagem->AddAddress( $iu['iusemailprincipal'], $iu['iusnome'] );
				

			$mensagem->Body = "<p>Prezado(a) {$iu['iusnome']},</p>
			<p>No dia <b>21 de mar�o de 2014</b>, o SisPacto 2013 ser� encerrado. Ap�s esta data, n�o ser� mais poss�vel processar informa��es relativas a bolsas de estudo pendentes. Neste sentido, constatamos que Vossa Senhoria n�o realizou as atividades necess�rias ao recebimento de todas as bolsas de estudo, conforme descrito abaixo:</p>

			<p>
			<b>Atividades pendentes:</b><br><br>
			".implode(";<br>",$texto)."
</p>

<p>Caso seja do seu interesse receber as bolsas de estudo n�o encaminhadas para pagamento, solicitamos o m�ximo de empenho para resolu��o das pend�ncias descritas acima at� a data informada (21/03/2014), caso contr�rio, n�o ser� mais poss�vel processar as solicita��es de pagamento. A previs�o de cr�dito em conta das bolsas pendentes de 2013 � Junho/2014.</p>
<p>Em caso de d�vida, sugerimos que entre em contato com o(a) seu(sua) Coordenador(a) local ou com a Institui��o de Ensino Superior (IES) respons�vel pela forma��o. Permanecendo as d�vidas ou problemas, por favor, envie um e-mail para ". $_SESSION['email_sistema']. ".</p>

<p>Secretaria de Educa��o B�sica<br>
Minist�rio da Educa��o</p>
			";
				
			$mensagem->IsHTML( true );
			$resp = $mensagem->Send();
				
				
		}

		}

	echo "</table>";
	echo "<p><b>Orientadores com restri��es: {$count}</b></p>";
}
*/

/*
$sql = "SELECT distinct  i.iusnome as nome, i.iusemailprincipal as email, us.ususenha as senha, s.logresponse, i.iuscpf
		from sispacto.identificacaousuario i 
		inner join sispacto.tipoperfil t on t.iusd = i.iusd 
		inner join seguranca.perfil p on p.pflcod = t.pflcod 
		left join seguranca.usuario us on us.usucpf = i.iuscpf 
		left join sispacto.universidadecadastro c on c.uncid = i.uncid 
		left join sispacto.universidade u on u.uniid = c.uniid 
		left join sispacto.pactoidadecerta pp on pp.picid = i.picid 
		left join territorios.municipio m on m.muncod = pp.muncod 
		left join territorios.estado es on es.estuf = pp.estuf
		inner join sispacto.logsgb s on s.logcpf = i.iuscpf and s.logservico='gravarDadosBolsista' and s.logerro=true
		where iustermocompromisso=true and cadastradosgb=false 
		and logresponse ilike '%Erro: </return>%' 
		ORDER BY i.iusnome";

$arr = $db->carregar($sql);


if($foos[0]) {
	foreach($foos as $foo) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO - Problemas com CPF na Receita Federal";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
						   <p>Foi identificado um problema com seu CPF na RECEITA FEDERAL. Para confirmar este problema acesse o site oficial da receita federal (http://www.receita.fazenda.gov.br/aplicacoes/atcta/cpf/consultapublica.asp) e confirme se seu CPF consta como REGULAR.</p>
						   <p>A conta no Banco do Brasil s� poder� ser criada se o CPF estiver REGULAR.</p>
						   <p>Att.<br>Equipe do PACTO</p>
						   <p>[CASO SEU NOME CONSTE COMO REGULAR NO SITE DA RECEITA FEDERAL, ENTRE EM CONTATO COM A EQUIPE GESTORA DO PACTO]</p>
		
		
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha</p>
		 ";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "CPF receita ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}





$sql = "select * from (
		select tpaid, count(distinct catid) as x from sispacto.aprendizagemconhecimentoturma group by tpaid
		) foo where foo.x!=11";

$arr = $db->carregar($sql);

if($arr[0]) {
	foreach($arr as $a1) {
		
		$sql = "select i.iusnome as nome, t.tpanometurma, t.tpanomeescola, i.iusemailprincipal as email from sispacto.turmasprofessoresalfabetizadores t 
				inner join sispacto.identificacaousuario i on i.iusd = t.iusd 
				where tpaid=".$a1['tpaid']." and tpastatus='A' AND tpaconfirmaregencia=true";
		
		$arr2 = $db->pegaLinha($sql);
		
		if($arr2) {
		
			$html  = "<p>Prezado ".$arr2['nome']."</p>";
			$html .= "<p>Identificamos que voc� n�o completou o cadastro da ABA Aprendizagem da Turma do SISPACTO, referente a turma : ".$arr2['tpanometurma']." da escola ".$arr2['tpanomeescola'].".</p>";
			$html .= "<p>Deve-se preencher o n�mero de alunos de todos os 11 conhecimentos cadastrados. Seguem abaixo os conhecimentos que n�o foram preenchidos:</p>";
	
			$sql = "select catdsc||'<br>' as cat from sispacto.aprendizagemconhecimento where catid not in( select catid from sispacto.aprendizagemconhecimentoturma where tpaid=".$a1['tpaid'].")";
			$catdsc = $db->carregarColuna($sql);
			
			if($catdsc) {
				$html .= "<p>".implode("",$catdsc)."</p>";
			}
			
			$html .= "<p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha</p>";
			
			$mensagem = new PHPMailer();
			$mensagem->persistencia = $db;
			
			$mensagem->Host         = "localhost";
			$mensagem->Mailer       = "smtp";
			$mensagem->FromName		= SIGLA_SISTEMA;
			$mensagem->From 		= "noreply@mec.gov.br";
			$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO - Faltam informa��es na ABA Aprendizagem da Turma";
			
			$mensagem->AddAddress( $arr2['email'], $arr2['nome'] );
			
				
			$mensagem->Body = $html;
			
			$mensagem->IsHTML( true );
			$resp = $mensagem->Send();
			echo "Aprendizagem incompleta ".$foo['nome']." - ".$foo['email']." : ".$resp;
		
		}
		
		
		
	}
}

$sql = "SELECT DISTINCT  i.iusnome as nome, i.iusemailprincipal as email, us.ususenha as senha, s.logresponse, i.iuscpf

		from sispacto.identificacaousuario i 
		inner join sispacto.tipoperfil t on t.iusd = i.iusd 
		inner join seguranca.perfil p on p.pflcod = t.pflcod 
		left join seguranca.usuario us on us.usucpf = i.iuscpf
		left join sispacto.universidadecadastro c on c.uncid = i.uncid 
		left join sispacto.universidade u on u.uniid = c.uniid 
		left join sispacto.pactoidadecerta pp on pp.picid = i.picid 
		left join territorios.municipio m on m.muncod = pp.muncod 
		left join territorios.estado es on es.estuf = pp.estuf 
		inner join (select max(logid) as logid, logcpf from sispacto.logsgb s where s.logservico='gravarDadosBolsista' and s.logerro=true group by logcpf) foo on foo.logcpf = i.iuscpf
		inner join sispacto.logsgb s on s.logid = foo.logid
		where cadastradosgb=false and iustermocompromisso=true and logresponse ilike '%Erro: 00026:%' ORDER BY i.iusnome;";

$identificacaousuario = $db->carregar($sql);

if($identificacaousuario[0]) {
	foreach($identificacaousuario as $ius) {
		
		$sl = explode("(",$ius['logresponse']);
		$sl = explode(")",$sl[1]);
		
		if(substr(strtoupper($ius['nome']),0,9)!=substr(strtoupper(trim($sl[0])),0,9)) {
			$arrListaP[$ius['iuscpf']] = array('nome' => $ius['nome'],'email' => $ius['email'],'senha' => $ius['senha']);				
		}
		
	}
	
	foreach($arrListaP as $arr) {
		$foos[] = $arr;
	}
}

if($foos[0]) {
	foreach($foos as $foo) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO - Problemas na identifica��o dos nomes";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
						   <p>Identificamos que o nome cadastrado no SisPacto � diferente do registro na Receita Federal, o que impede o recebimento da bolsa de estudo relativa ao Pacto Nacional pela Alfabetiza��o na Idade Certa.</p>
						   <p>Para regularizar a sua situa��o, solicitamos que envie documentos que comprovem a mudan�a de nome para o e-mail ". $_SESSION['email_sistema']. ". Por exemplo: CPF com o nome anterior e CPF atual.</p>

							<p>Secretaria de Educa��o B�sica<br>
							Minist�rio da Educa��o</p>
		
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha</p>
		 ";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Troca de nomes ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}




$sql = "select i.iusd, i.iuscpf, i.iusnome as nome, i.iusemailprincipal as email, i.iusagenciasugerida, s.agencia, i.cadastradosgb from sispacto.bolsistaserroagencia s 
inner join sispacto.identificacaousuario i on i.iuscpf = trim(s.cpf)
inner join sispacto.tipoperfil t on t.iusd = i.iusd 
where i.iusagenciasugerida = s.agencia";

	$foos = $db->carregar($sql);
	
if($foos[0]) {
	foreach($foos as $foo) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO - Problemas com ag�ncia banc�ria";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
						   <p>Foi identificado problema com a ag�ncia selecionada no cadastro do SISPACTO. Possivelmente sua ag�ncia foi invalidada ou desativada do programa.</p>
						   <p>Pedimos que acesse o sistema e selecione outra ag�ncia banc�ria(na aba \"Dados\").</p>
						   <p>Att.<br>Equipe do PACTO</p>
						   <p>[ASSIM QUE FOR ALTERADO, ESTE E-MAIL N�O SER� MAIS ENVIADO]</p>
		
		
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha</p>
		 ";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Ag�ncia Banc�ria ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}
*/


/*

$sql = "SELECT distinct i.iusnome as nome, i.iusemailprincipal as email, us.ususenha as senha 
	FROM sispacto.identificacaousuario i 
	INNER JOIN sispacto.tipoperfil t on t.iusd = i.iusd 
	LEFT JOIN seguranca.usuario us ON us.usucpf = i.iuscpf 
	LEFT JOIN sispacto.materiaisprofessores m ON m.iusd = i.iusd 
	WHERE t.pflcod=849 AND mapid is null";

	$foos = $db->carregar($sql);
	
if($foos[0]) {
	foreach($foos as $foo) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO - Preenchimento de informa��es sobre materiais";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
						   <p>Precisamos da sua informa��o a respeito dos materiais. Solicitamos que acessem o SISPACTO e preencham as informa��es solicitadas na aba \"Materiais\".</p>
						   <p>Uma das informa��es � sobre a cria��o do cantinho da leitura, se for poss�vel, enviar fotos deste local (envio pelo sistema).</p>
						   <p>Att.<br>Equipe do PACTO</p>
						   <p>[ASSIM QUE FOR PREENCHIDO, ESTE E-MAIL N�O SER� MAIS ENVIADO]</p>
		
		
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>
		 ";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Materiais Professores ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}

*/


$sql = "select DISTINCT i.iusnome as nome, i.iusemailprincipal as email, us.ususenha as senha 
		from sispacto.identificacaousuario i 
		inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod=826 
		left join seguranca.usuario us on us.usucpf = i.iuscpf 
		left join sispacto.gestaomobilizacaoperguntas g on g.iusd = i.iusd 
		where gmpid is null";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO - Gest�o e Mobiliza��o";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
						   <p>Precisamos da sua informa��o. Solicitamos que acessem o SISPACTO e preencham as informa��es solicitadas na aba \"Gest�o e Mobiliza��o\".</p>
						   <p>Leia atentamente as instru��es antes de responder.</p>
						   <p>Att.<br>Equipe do PACTO</p>
						   <p>[ASSIM QUE FOR PREENCHIDO, ESTE E-MAIL N�O SER� MAIS ENVIADO]</p>
		
		
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>
		 ";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Gestao mobilizacao ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}

/*

$sql = "SELECT distinct i.iusnome as nome, i.iusemailprincipal as email, us.ususenha as senha 
	FROM sispacto.identificacaousuario i 
	INNER JOIN sispacto.tipoperfil t on t.iusd = i.iusd 
	LEFT JOIN seguranca.usuario us ON us.usucpf = i.iuscpf 
	INNER JOIN sispacto.turmasprofessoresalfabetizadores m ON m.iusd = i.iusd AND m.tpastatus='A'
	LEFT JOIN sispacto.aprendizagemconhecimentoturma aa ON aa.tpaid = m.tpaid
	WHERE t.pflcod=849 AND aa.actid is null and tpaconfirmaregencia=true";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO - Aprendizagem da Turma";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
						   <p>Precisamos novamente da sua informa��o a respeito das turmas na qual voc� � regente. Solicitamos que acessem o SISPACTO e preencham as informa��es solicitadas na aba \"Aprendizagem da Turma\".</p>
						   <p>Voc� dever� responder informa��es sobre conhecimentos adquiridos pelos alunos. Leia atentamente as instru��es antes de responder.</p>
						   <p>Att.<br>Equipe do PACTO</p>
						   <p>[ASSIM QUE FOR PREENCHIDO, ESTE E-MAIL N�O SER� MAIS ENVIADO]</p>
		
		
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>
		 ";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Aprendizagem turmas ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}

*/



/*
$sql = "SELECT distinct i.iusnome as nome, i.iusemailprincipal as email, us.ususenha as senha 
	FROM sispacto.identificacaousuario i 
	INNER JOIN sispacto.tipoperfil t on t.iusd = i.iusd 
	LEFT JOIN seguranca.usuario us ON us.usucpf = i.iuscpf 
	LEFT JOIN sispacto.turmasprofessoresalfabetizadores m ON m.iusd = i.iusd 
	WHERE t.pflcod=849 AND tpatotalmeninos is null AND tpastatus='A' AND (tpaconfirmaregencia IS NULL OR tpaconfirmaregencia=true)";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO - Dados gerais das turmas";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
						   <p>Precisamos novamente da sua informa��o a respeito das turmas na qual voc� � regente. Solicitamos que acessem o SISPACTO e preencham as informa��es solicitadas na aba \"Dados das Turmas\".</p>
						   <p>Inicialmente foi feito uma pr�-carga das turmas de cada professor (Censo Escolar 2013 - vers�o preliminar), ao acessar a aba, o professor dever� validar se as turmas carregadas realmente est�o designadas a ele (caso esteja faltando alguma turma, o professor poder� inserir as turmas n�o cadastradas).</p>
						   <p>Com as turmas validadas/cadastradas, o professor dever� inserir as informa��es solicitadas para cada turma designada.</p>
						   <p>Att.<br>Equipe do PACTO</p>
						   <p>[ASSIM QUE FOR PREENCHIDO, ESTE E-MAIL N�O SER� MAIS ENVIADO]</p>
		
		
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>
		 ";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Materiais Professores ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}
*/


/*
if(date("d")==28) {

	$sql = "select i.iusnome as nome, i.iusemailprincipal as email, u.ususenha as senha from sispacto.identificacaousuario i 
inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod=826 
left join seguranca.usuario u on u.usucpf = i.iuscpf
where picid in(
select picid from sispacto.pactoidadecerta where estuf in('SP','MG','RS','DF','SC','MT','AM','PA','PB','ES','MS','RO','AC') or muncod in('3304557','3550308','1302603','2304400','2927408','4106902','3106200','2611606')
)";
	
	$foos = $db->carregar($sql);
	
	if($foos[0]) {
		foreach($foos as $foo) {
			
			$mensagem = new PHPMailer();
			$mensagem->persistencia = $db;
			
			$mensagem->Host         = "localhost";
			$mensagem->Mailer       = "smtp";
			$mensagem->FromName		= SIGLA_SISTEMA;
			$mensagem->From 		= "noreply@mec.gov.br";
			$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO - Redes com mais de 1(um) Coordenador Local";
			
			$mensagem->AddAddress( $foo['email'], $foo['nome'] );
			
				
			$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
							   <p>Devido ao n�mero elevado de participantes, algumas redes de ensino passaram a possuir, no �mbito do PACTO, mais de 1 (um) coordenador local.</p>
							   <p>Caso este seja o caso da sua rede, pedimos que sejam adotados alguns crit�rios de trabalho, a fim de n�o comprometer o pagamento das bolsas:</p>
							   <p>1. Cada coordenador deve avaliar mensalmente pelo menos um orientador de estudo, sob pena de comprometer a libera��o da sua bolsa, uma vez que para o MEC este coordenador n�o ter� realizado uma das atividades obrigat�rias.<br>
								  2. O n�mero de  orientadores de estudo a ser avaliado deve ser dividido de forma igualit�ria entre os coordenadores locais da rede municipal ou estadual.<br>
								  3. Esta divis�o de trabalho entre os coordenadores deve contemplar n�o somente a avalia��o, mas tamb�m as demais tarefas que cabem aos coordenadores, por exemplo: gerenciar senhas, auxiliar o orientador de estudos, acompanhar a forma��o e a condu��o das atividades pelo orientador, etc.<br>
								  4. O SISPACTO n�o define quais orientadores de estudo ser�o avaliados por quais coordenadores. Esta divis�o deve ser feita e respeitada dentro da organiza��o da equipe do PACTO na rede.<br>
								  5. Quando um orientador for avaliado ele desaparece da lista para avalia��o, e n�o poder� ser reavaliado por outro coordenador da rede.<br>
								  6. Ao final da avalia��o do seu grupo de orientadores, o coordenador deve sempre enviar para an�lise a avalia��o feita.</p>
							   <p>Salientamos que caso um coordenador local avalie todos os orientadores de estudo os demais coordenadores ficar�o, de forma irrevers�vel, impossibilitados de receber a bolsa referente ao per�odo avaliado. Caso um dos coordenadores da rede n�o fa�a nenhuma avalia��o no per�odo de refer�ncia, ele ficar� impossibilitado de receber a bolsa referente ao per�odo avaliado.</p>
							   <p>Att.<br>Equipe do PACTO</p>
			
			
			 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>
			 ";
			
			$mensagem->IsHTML( true );
			$resp = $mensagem->Send();
			echo "Aviso Coordenador Local + de 1 ".$foo['nome']." - ".$foo['email']." : ".$resp;
		}
	}
	
}
*/

/*
if(date("d")==26) {

	$sql = "SELECT  
			i.iusnome as nome,
			i.iusemailprincipal as email,
			u.ususenha as senha
			FROM sispacto.identificacaousuario i
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd  
			LEFT JOIN seguranca.usuario u ON u.usucpf = i.iuscpf   
			WHERE i.iusstatus='A' AND t.pflcod!='".PFL_PROFESSORALFABETIZADOR."'";
	
	$foos = $db->carregar($sql);
	
	if($foos[0]) {
		foreach($foos as $foo) {
			
			$mensagem = new PHPMailer();
			$mensagem->persistencia = $db;
			
			$mensagem->Host         = "localhost";
			$mensagem->Mailer       = "smtp";
			$mensagem->FromName		= SIGLA_SISTEMA;
			$mensagem->From 		= "noreply@mec.gov.br";
			$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO - Comunicado sobre reavalia��o";
			
			$mensagem->AddAddress( $foo['email'], $foo['nome'] );
			
				
			$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
			 <p>Para  reavaliar um usu�rio voc� deve clicar no �cone , adicionar uma justificativa e reavaliar o bolsista no box que aparecer� abaixo do seu nome. Ao final salve o procedimento.</p> 
			 <p>A ferramenta de reavalia��o pode ser utilizada em dois casos:</p> 
			 <p>
			 a. para reavaliar bolsista com nota inferior a 7,0 a qualquer tempo, mesmo que n�o seja poss�vel tramitar a avalia��o pois seu estado atual j� � \"aprovado\".<br>
			 b. para avaliar pela primeira vez bolsista que entrou tardiamente no programa do PACTO e ainda n�o foi avaliado, mesmo que n�o seja poss�vel tramitar a avalia��o pois seu estado atual j� � \"aprovado\".<br><br/>
			 Caso voc� tenha substitu�do um participante anterior que ocupava este perfil, voc� poder� utilizar a ferramenta de reavalia��o normalmente. No entanto, caso n�o seja poss�vel tramitar a avalia��o (aparecer o aviso \"nenhuma a��o dispon�vel\")  isso ocorrer� porque seu estado j� est� como \"aprovado\" . Isso significa que o m�s que quest�o foi tramitado pelo ocupante anterior do perfil. Ap�s salvar a altera��o da nota, nenhuma a��o ser�, portanto, necess�ria.
			 </p>
			
			 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>
			 ";
			
			$mensagem->IsHTML( true );
			$resp = $mensagem->Send();
			echo "Aviso reavalia��o_ ".$foo['nome']." - ".$foo['email']." : ".$resp;
		}
	}

}
*/

// ENVIANDO EMAIL PARA OS COORDENADORES LOCAIS QUE NAO PREENCHERAM MATERIAIS 

$sql = "SELECT  
		i.iusnome as nome,
		i.iusemailprincipal as email,
		u.ususenha as senha
		FROM sispacto.pactoidadecerta p 
		INNER JOIN sispacto.identificacaousuario i ON i.picid = p.picid 
		INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_COORDENADORLOCAL." 
		INNER JOIN workflow.documento d ON d.docid = p.docid 
		LEFT JOIN seguranca.usuario u ON u.usucpf = i.iuscpf
		WHERE p.picid NOT IN(SELECT picid FROM sispacto.materiais) AND d.esdid='563' AND i.iusstatus='A'";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO - Preenchimento do recebimento de Materiais";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		 <p>Estamos coletando informa��es sobre o recebimento do materiais no munic�pios, e verificamos que seu munic�pio ainda n�o preencheu este question�rio.</p>
		 <p>Solicitamos sua participa��o para ajudar o MEC gerenciar os materiais. Por favor acesse o ambiente, fa�a o login, e na aba de Materiais preencha as informa��es.</p>
		 <p>Secretaria de Educa��o B�sica<br/>Minist�rio da Educa��o</p>
		 <br/><br/>
		 <p>ATEN��O � PACTO NACIONAL DE ALFABETIZA��O NA IDADE CERTA</p>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>
		 ";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Materiais _ ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}


// ENVIANDO EMAIL PARA OS COORDENADORES IES QUE POSSUEM PENDENCIAS NO APROVAR

$sql = "SELECT iusnome as nome, iusemailprincipal as email, (SELECT ususenha FROM seguranca.usuario WHERE usucpf=i.iuscpf) as senha FROM sispacto.identificacaousuario i 
INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
WHERE t.pflcod IN(832,846)";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO - Aprova��o da equipe";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		 <p>Informamos que voc� (Coordenador Geral da IES) possui avalia��es pendentes de aprova��o para que seja criado o registro de pagamento.</p>
		 <p>Para fazer a aprova��o, acesse a aba de Execu��o e clique em Aprovar Equipe. Em seguida selecione um per�odo de refer�ncia e o perfil. Na tela ser� exibida todos os usu�rios, e somente vir� marcado os usu�rios que atenderam os crit�rios de aprova��o. Nesta fase voc� pode desmarcar algum nome que voc� n�o queira aprovar no momento, e aprova-lo posteriormente.</p>
		 <p>Em seguida clique no bot�o Aprovar. Com isso, voc� este usu�rio ter� o registro de pagamento criado. Esta op��o de Aprovar Equipe esta dispon�vel para o Coordenador Geral da IES e o Coordenador Adjunto da IES.</p>
		 <p>Ap�s a aprova��o, � necess�rio acessar a aba de pagamentos (somente o Coordenador Geral da IES) e autorizar os pagamentos.</p>
		 <p>Secretaria de Educa��o B�sica<br/>Minist�rio da Educa��o</p>
		 <br/><br/>
		 <p>ATEN��O � PACTO NACIONAL DE ALFABETIZA��O NA IDADE CERTA</p>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>
		 ";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Aprovar Equipe _ ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}

// ENVIANDO EMAIL PARA TODOS QUE N�O REALIZARAM AVALIA��ES
/*
$sql = "SELECT foo.iusnome as nome, foo.iusemailprincipal as email, foo.ususenha as senha, foo.referencia FROM (
		SELECT i.iusnome, i.iusemailprincipal, usu.ususenha, t.pflcod, rf.rfuparcela ||'� Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as referencia, CASE WHEN (dd.esdid NOT IN(657,601) OR dd.esdid IS NULL) THEN 'NOK' ELSE 'OK' END as ap
					FROM sispacto.folhapagamento f 
					INNER JOIN sispacto.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid 
					INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia 
					INNER JOIN sispacto.identificacaousuario i ON i.uncid = rf.uncid 
					INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd  AND t.pflcod IN(826,827,832,848,847,846) 
					INNER JOIN sispacto.pagamentoperfil pp ON pp.pflcod = t.pflcod 
					INNER JOIN seguranca.usuario_sistema us ON us.usucpf = i.iuscpf AND us.suscod='A' AND us.sisid=142 
					INNER JOIN seguranca.perfilusuario pu ON pu.usucpf = i.iuscpf AND pu.pflcod = t.pflcod 
					INNER JOIN seguranca.usuario usu ON usu.usucpf = i.iuscpf 
					LEFT JOIN sispacto.mensario mm ON mm.iusd = i.iusd AND mm.fpbid = f.fpbid 
					LEFT JOIN workflow.documento dd ON dd.docid = mm.docid 
					WHERE f.fpbstatus='A' AND to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd') AND rf.rfuparcela <= pp.plpmaximobolsas
		) foo WHERE foo.ap='NOK'";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO - Avalia��o da equipe";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		 <p>Informamos que seu cadastro ja esta liberado no SIMEC, e � fundamental que voc� fa�a avalia��es sobre membros do projeto PACTO Idade Certa. Verificamos que voc� n�o fez a avalia��o do per�odo de refer�ncia: <b>".$foo['referencia']."</b></p>
		 <p>Para fazer a avalia��o, acesse a aba de Execu��o e clique em Avaliar Equipe. Em seguida selecione as op��es referentes a Frequ�ncia (caso seja obrigat�rio), Atividades Realizadas (caso seja obrigat�rio) e aperte o bot�o 'Salvar'.</p>
		 <p>Em seguida no �cone 'Enviar para an�lise'. Este passo � muito importante para a nota de monitoramento (parte da nota total da avalia��o).</p>
		 <p>Secretaria de Educa��o B�sica<br/>Minist�rio da Educa��o</p>
		 <br/><br/>
		 <p>ATEN��O � PACTO NACIONAL DE ALFABETIZA��O NA IDADE CERTA</p>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>
		 ";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Avalia��o Geral _ ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}
*/

// ENVIANDO EMAIL PARA TODOS PROFESSORES ALFABETIZAODRES QUE N�O PREENCHERAM OS DADOS
/*
$sql = "select i.iusnome as nome, i.iusemailprincipal as email, uu.ususenha as senha from sispacto.identificacaousuario i 
		inner join sispacto.tipoperfil t on t.iusd = i.iusd 
		inner join seguranca.perfilusuario pp on pp.pflcod = t.pflcod and i.iuscpf = pp.usucpf 
		inner join seguranca.usuario_sistema us on us.usucpf=i.iuscpf and us.sisid=".SIS_SISPACTO." and us.suscod='A' 
		inner join seguranca.usuario uu on uu.usucpf = i.iuscpf and uu.suscod='A'
		where t.pflcod=".PFL_PROFESSORALFABETIZADOR." AND i.iustermocompromisso != true";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO - Preenchimento das dados cadastrais";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) Professor Alfabetizador - {$foo['nome']},</p>
		 <p>Informamos que seu cadastro ja esta liberado no SIMEC, e � obrigat�rio o preenchimento dos dados para o recebimento da bolsa.</p>
		 <p>Alem do preenchimento dos dados cadastrais (Aba de Dados Professor Alfabetizador), voc� tem um ferramenta para avaliar algum quesitos do curso (Orientador de Estudo, Conte�do, etc), a fim de melhorar sua qualidade (Esta atividade n�o � obrigat�ria, por�m todos os coordenadores das universidades ter�o acesso a sua avalia��o).</p>
		 <p>Existe ainda uma aba de Acompanhamento, aonde voc� obter� informa��e sobre as avalia��es e pagamento de bolsas.</p>
		 <p>Secretaria de Educa��o B�sica<br/>Minist�rio da Educa��o</p>
		 <br/><br/>
		 <p>ATEN��O � PACTO NACIONAL DE ALFABETIZA��O NA IDADE CERTA</p>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>
		 ";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Ativa��o dos Professores Alfabetizaodres _ ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}
*/

// ENVIANDO EMAIL PARA TODOS OS MUNIC�PIOS QUE EST�O APTOS A CADASTRAREM AS TURMAS

$sql = "SELECT distinct foo.nome, foo.email, foo.senha, foo.esdid_formacao FROM (
		SELECT 
		i.iusnome as nome,
		i.iusemailprincipal as email,
		e.esdid,
		dc.esdid as esdid_formacao,
		d2.esdid as esdid_turmas,
		u.ususenha as senha
		FROM sispacto.pactoidadecerta p 
		INNER JOIN sispacto.identificacaousuario i ON i.picid = p.picid 
		INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_COORDENADORLOCAL." 
		LEFT JOIN seguranca.usuario u ON u.usucpf = i.iuscpf 
		INNER JOIN territorios.municipio m ON m.muncod = p.muncod
		LEFT JOIN workflow.documento d ON d.docid = p.docid  
		LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid
		LEFT JOIN workflow.documento d2 ON d2.docid = p.docidturma  
		LEFT JOIN workflow.estadodocumento e2 ON e2.esdid = d2.esdid
		INNER JOIN sispacto.abrangencia ab ON ab.muncod = m.muncod 
		INNER JOIN sispacto.estruturacurso es ON es.ecuid = ab.ecuid 
		INNER JOIN sispacto.universidadecadastro un ON un.uncid = es.uncid 
		INNER JOIN workflow.documento dc ON dc.docid = un.docidformacaoinicial 
		) foo 
		WHERE foo.esdid=563 and foo.esdid_formacao=611 and (foo.esdid_turmas IS NULL OR foo.esdid_turmas!=630)";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO - Valida��o dos substitutos/Cadastramento de turmas pelo Coordenador Local";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) Coordenador(a) do Pacto - {$foo['nome']},</p>
		 <p>Informamos que a Universidade informou no sistema os Orientadores de Estudo Ausentes/Presentes e fizeram o lan�amento dos substitutos para aprova��o.</p>
		 <p>Para regularizar a situa��o, orientamos que acesse o SisPacto e, na tela (�Definir Orientadores de Estudo�), autorize a substitui��o dos Orientadores que compare�eram na Forma��o Inicial, em seguida termine o cadastro dos orientadores que foram substitu�dos na tela �Definir Orientadores de Estudo�. A seguir, clique em �Turmas� e cadastre os Professores vinculados a cada Orientador de Estudo.</p>
		 <p>Ap�s finalizar o cadastramento dos Professores Alfabetizadores clique em �Concluir composi��o de turma�, este processo finaliza o cadastramento dos professores.</p>
		 <p>Secretaria de Educa��o B�sica<br/>Minist�rio da Educa��o</p>
		 <br/><br/>
		 <p>ATEN��O � PACTO NACIONAL DE ALFABETIZA��O NA IDADE CERTA</p>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>
		 ";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Valida��o dos substitutos/Cadastramento de turmas pelo Coordenador Local _ ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}

// ENVIANDO EMAIL PARA TODOS OS COORDENADORES IES QUE EST�O COM PROJETO VALIDADO E N�O CONCLUIRAM

$sql = "SELECT
		COALESCE((SELECT iusnome FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.uncid=u.uncid AND t.pflcod=832),'Coordenador Local n�o cadastrado') as nome,
		COALESCE((SELECT iusemailprincipal FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.uncid=u.uncid AND t.pflcod=832),'Coordenador Local n�o cadastrado') as email
		FROM sispacto.universidadecadastro u 
		INNER JOIN workflow.documento d1 ON d1.docid = u.docid 
		INNER JOIN  workflow.documento d2 ON d2.docid = u.docidformacaoinicial
		WHERE d1.esdid=".ESD_VALIDADO_COORDENADOR_IES." AND d2.esdid=".ESD_ABERTO_FORMACAOINICIAL;

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO - Preenchimento do Registro de Frequ�ncia aberto da Forma��o Inicial";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) Coordenador(a) da IES - {$foo['nome']},</p>
		 <p>Identificamos que o MEC aprovou seu projeto do PACTO.</p>
		 <p>A pr�xima tarefa � sinalizar os presentes/ausentes do curso de Forma��o Inicial. Caso tenha comparecido no curso Forma��o Inicial algum Orientador enviado pelo Munic�pio para substituir, � necess�rio cadastrar alguns dados pessoais para que o Munic�pio autorize a substitui��o.</p>
		 <p>Para regularizar a situa��o, orientamos que acesse o SisPacto e, na �ltima tela (�Forma��o Inicial�), informe os ausentes/presentes e clique no bot�o �Salvar�. Para inserir substituto clique em 'Inserir Orientadore Substituto'.</p>
		 <p>Caso tenha alguma pend�ncia no projeto, reajuste o projeto e tente novamente</p>
		 <p>Secretaria de Educa��o B�sica<br/>Minist�rio da Educa��o</p>
		 <br/><br/>
		 <p>ATEN��O � PACTO NACIONAL DE ALFABETIZA��O NA IDADE CERTA</p>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>
		 ";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Forma��o Inicial pelo Coordenador IES _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}


// ENVIANDO EMAIL PARA TODOS OS COORDENADORES IES QUE N�O ENVIARAM O PROJETO

$sql = "SELECT foo.nome, foo.email, foo.senha FROM (
		SELECT 
		COALESCE((SELECT iusnome FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.uncid=u.uncid AND t.pflcod=832),'Coordenador Local n�o cadastrado') as nome,
		COALESCE((SELECT iusemailprincipal FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.uncid=u.uncid AND t.pflcod=832),'Coordenador Local n�o cadastrado') as email,
		e.esdid,
		COALESCE((SELECT us.ususenha FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd INNER JOIN seguranca.usuario us ON us.usucpf = i.iuscpf WHERE i.uncid=u.uncid AND t.pflcod=832),'') as senha
		FROM sispacto.universidadecadastro u 
		LEFT JOIN workflow.documento d ON d.docid = u.docid  
		LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid) foo 
		WHERE foo.esdid=577";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO - Em elabora��o pelo Coordenador IES";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) Coordenador(a) da IES - {$foo['nome']},</p>
		 <p>Identificamos que o sua universidade n�o concluiu o projeto do PACTO.</p>
		 <p>Para regularizar a situa��o, orientamos que acesse o SisPacto e, na �ltima tela (�Visualiza��o do Projeto�) clique no bot�o de a��o �Enviar para an�lise do MEC�. Este procedimento � fundamental para concluir o processo de cadastramento.</p>
		 <p>Caso tenha alguma pend�ncia no projeto, reajuste o projeto e tente novamente</p>
		 <p>Secretaria de Educa��o B�sica<br/>Minist�rio da Educa��o</p>
		 <br/><br/>
		 <p>ATEN��O � PACTO NACIONAL DE ALFABETIZA��O NA IDADE CERTA</p>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>
		 ";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Em elabora��o pelo Coordenador IES _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}

// ENVIANDO EMAIL PARA TODOS OS MUNIC�PIOS QUE EST�O COM ORIENTADORES DE ESTUDO EM CADASTRAMENTO

$sql = "SELECT foo.nome, foo.email, foo.senha FROM (
		SELECT 
		i.iusnome as nome,
		i.iusemailprincipal as email,
		e.esdid,
		u.ususenha as senha
		FROM sispacto.pactoidadecerta p 
		INNER JOIN sispacto.identificacaousuario i ON i.picid = p.picid 
		INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_COORDENADORLOCAL." 
		LEFT JOIN seguranca.usuario u ON u.usucpf = i.iuscpf 
		INNER JOIN territorios.municipio m ON m.muncod = p.muncod
		LEFT JOIN workflow.documento d ON d.docid = p.docid  
		LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid) foo 
		WHERE foo.esdid=561";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO - Em elabora��o pelo Coordenador Local";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) Coordenador(a) do Pacto - {$foo['nome']},</p>
		 <p>Identificamos que o seu munic�pio iniciou o cadastramento dos Orientadores de Estudo no SisPacto, todavia, os cadastros n�o foram submetidos � an�lise da Institui��o de Ensino Superior (IES) respons�vel pela forma��o, o que impossibilitar� o registro e futura matr�cula desses profissionais.</p>
		 <p>Para regularizar a situa��o, orientamos que acesse o SisPacto e, na �ltima tela (�Resumo Orientadores de Estudo�) clique no bot�o de a��o �Enviar para an�lise da IES�. Este procedimento � fundamental para concluir o processo de cadastramento.</p>
		 <p>Secretaria de Educa��o B�sica<br/>Minist�rio da Educa��o</p>
		 <br/><br/>
		 <p>ATEN��O � PACTO NACIONAL DE ALFABETIZA��O NA IDADE CERTA</p>
		 <p>Avisamos aos Estados e Munic�pios do PACTO que dia 15 de Fevereiro encerra-se o prazo para troca de Orientadores de Estudos do PACTO.</p> 
		 <p>Assim, a partir dessa data, o Sispacto estar� fechado para a execu��o da a��o: \"Efetuar troca de Orientadores de Estudo\"</p>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>
		 ";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Em elabora��o pelo Coordenador Local _ ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}


?>