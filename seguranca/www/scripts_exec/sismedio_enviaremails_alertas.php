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
require_once APPRAIZ . "www/sismedio/_constantes.php";
require_once APPRAIZ . "www/sismedio/_funcoes.php";

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';


// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

function getmicrotime() {list($usec, $sec) = explode(" ", microtime()); return ((float)$usec + (float)$sec);}

$microtime = getmicrotime();



   
// abre conex��o com o servidor de banco de dados
$db = new cls_banco();

if(date("w")==2) {

	$sql = "select i.iusnome as nome, i.iusemailprincipal as email from sismedio.identificacaousuario i 
			inner join sismedio.tipoperfil t on t.iusd = i.iusd 
			where t.pflcod in(1076,1077) and i.uncid in(
			
			SELECT u.uncid FROM sismedio.universidadecadastro u
			INNER JOIN workflow.documento d ON d.docid = u.docid 
			INNER JOIN workflow.documento d2 ON d2.docid = u.docidturmaformadoresregionais 
			INNER JOIN workflow.documento d3 ON d3.docid = u.docidturmaorientadoresestudo
			WHERE d.esdid='931' AND d2.esdid=1200 AND d3.esdid=1200
			
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
			$mensagem->Subject 		= SIGLA_SISTEMA. " - SISM�dio - LEMBRETE : Aprovar Equipe";
	
			$mensagem->AddAddress( $foo['email'], $foo['nome'] );
	
				
			$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
			<p>Uma vez por semana, a equipe do MEC vai enviar este e-mail para lembra-lo de aprovar a avalia��o feita pelos membros da sua equipe. este procedimento ir� garantir as bolsas de todos os participantes.</p>
			<p>Para aprovar a bolsa, basta acessar a aba Execu��o => Aprovar Equipe, selecione os per�odos de ref�rencia e os perfis, e clique no bot�o Aprovar. Essa atividade pode ser realizar pelo Coordenador Geral ou Adjuntos das universidades (� recomendado que estes fa�am essa atividade periodicamente).</p>
			<p>Secretaria de Educa��o B�sica<br/>Minist�rio da Educa��o</p>
			<br/><br/>
			<p>ATEN��O � Pacto Nacional pelo Fortalecimento do Ensino M�dio</p>
			<p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha</p>
			";
	
			$mensagem->IsHTML( true );
			$resp = $mensagem->Send();
			echo "Lembrete aprovar equipe _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
		}
	}

}

$sql = "select i.iusd, i.iuscpf, i.iusnome as nome, i.iusemailprincipal as email, i.iusagenciasugerida, s.agencia, i.cadastradosgb
		from sismedio.bolsistaserroagencia s
		inner join sismedio.identificacaousuario i on i.iuscpf = trim(s.cpf)
		inner join sismedio.tipoperfil t on t.iusd = i.iusd
		where trim(i.iusagenciasugerida) = trim(s.agencia)";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISMEDIO - Problemas com ag�ncia banc�ria";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>Foi identificado problema com a ag�ncia selecionada no cadastro do SISMEDIO. Possivelmente sua ag�ncia foi invalidada ou desativada do programa.</p>
		<p>Pedimos que acesse o sistema e selecione outra ag�ncia banc�ria(na aba \"Dados\").</p>
		<p>Att.<br>Equipe do SISMEDIO</p>
		<p>[ASSIM QUE FOR ALTERADO, ESTE E-MAIL N�O SER� MAIS ENVIADO]</p>


		<p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha</p>
		";

		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Ag�ncia Banc�ria ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}


$sql = "SELECT i.iusnome as nome, i.iusemailprincipal as email FROM sismedio.identificacaousuario i 
		INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd AND t.pflcod IN(1076, 1078, 1077)
		WHERE i.uncid IN(
		
		SELECT u.uncid FROM sismedio.universidadecadastro u
		INNER JOIN workflow.documento d ON d.docid = u.docid 
		INNER JOIN workflow.documento d2 ON d2.docid = u.docidturmaformadoresregionais 
		INNER JOIN workflow.documento d3 ON d3.docid = u.docidturmaorientadoresestudo
		WHERE d.esdid='931' AND (d2.esdid!=1200 OR d3.esdid!=1200)
		
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
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISM�dio -  Composi��o de turmas";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>O projeto da sua universidade encontra-se validado pelo MEC, por�m existem mais alguns passos para iniciarmos a execu��o do curso, e consequentemente o pagamento das bolsas.</p>
		<p>O primeiro passo � compor as turmas dos Formadores Regionais (Principal => Composi��o de turmas). Para efetuar este procedimento, basta acessar a turma do Formador e adicionar Orientadores de Estudos, e ao final clicar no link \"Concluir composi��o da turma\". Este passo pode ser efetuado pelo Coordenador Geral, Adjunto e Supervisores.</p>
		<p>O segundo passo � compor as turmas dos Orientadores de Estudo. Por padr�o, o sistema faz um pr� carregamento para as escolas que possuem apenas 1 orientador de estudo, por�m os que possuem mais de 1, esses perfis devem aloca-los na devida turma. ao final clicar no link \"Concluir composi��o da turma\".</p>
		<p>Depois desses passos, iniciaremos a Execu��o do programa.</p>
		<p>Secretaria de Educa��o B�sica<br/>Minist�rio da Educa��o</p>
		<br/><br/>
		<p>ATEN��O � Pacto Nacional pelo Fortalecimento do Ensino M�dio</p>
		<p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha</p>
		";

		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Composi��o de turmas _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}


/*
 * ALERTANDO TODOS OS PERFIS COM ACESSO AO SISPACTO DE PREENCHER O TERMO DE COMPROMISSO
*/

$sql = "select i.iusnome as nome, i.iusemailprincipal as email, u.ususenha as senha from sismedio.identificacaousuario i
		inner join sismedio.tipoperfil t on t.iusd = i.iusd
		inner join seguranca.perfilusuario pu on i.iuscpf = pu.usucpf and t.pflcod = pu.pflcod
		inner join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.sisid=".SIS_MEDIO." AND us.suscod='A'
		inner join seguranca.usuario u on u.usucpf = i.iuscpf
		where i.iusstatus='A' and i.iustermocompromisso is null and i.uncid in(
		
				SELECT u.uncid FROM sismedio.universidadecadastro u
				INNER JOIN workflow.documento d ON d.docid = u.docid
				WHERE d.esdid='".ESD_VALIDADO_COORDENADOR_IES."'
		
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
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISM�dio -  Preenchimento dos dados cadastrais";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>Informamos que seu acesso ja esta liberado no SIMEC. Solicitamos que acesse o sistema e preencha os dados solicitados para o recebimento da bolsa.</p>
		<p>Secretaria de Educa��o B�sica<br/>Minist�rio da Educa��o</p>
		<br/><br/>
		<p>ATEN��O � Pacto Nacional pelo Fortalecimento do Ensino M�dio</p>
		<p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>
		";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Preenchimento das informa��es _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}

$sql = "SELECT foo.iusnome as nome, foo.iusemailprincipal as email, foo.ususenha as senha, foo.referencia, foo.pfldsc FROM (
SELECT i.uncid, per.pfldsc, i.iusnome, i.iusemailprincipal, usu.ususenha, t.pflcod, rf.rfuparcela ||'� Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as referencia, CASE WHEN count(racid) > 0 THEN 'OK' ELSE 'NOK' END as ap
					FROM sismedio.folhapagamento f 
					INNER JOIN sismedio.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid 
					INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia 
					INNER JOIN sismedio.identificacaousuario i ON i.uncid = rf.uncid 
					INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd  AND t.pflcod IN(1082,1088) AND rf.pflcod = t.pflcod
					INNER JOIN sismedio.pagamentoperfil pp ON pp.pflcod = t.pflcod 
					INNER JOIN seguranca.usuario_sistema us ON us.usucpf = i.iuscpf AND us.suscod='A' AND us.sisid=174 
					INNER JOIN seguranca.perfilusuario pu ON pu.usucpf = i.iuscpf AND pu.pflcod = t.pflcod 
					INNER JOIN seguranca.usuario usu ON usu.usucpf = i.iuscpf 
					INNER JOIN seguranca.perfil per ON per.pflcod = t.pflcod 
					LEFT JOIN sismedio.respostasavaliacaocomplementar mm ON mm.iusdavaliador = i.iusd AND mm.fpbid = f.fpbid
					WHERE f.fpbstatus='A' AND to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd')
GROUP BY i.uncid, per.pfldsc, i.iusnome, i.iusemailprincipal, usu.ususenha, t.pflcod, rf.rfuparcela, m.mesdsc, fpbanoreferencia 
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
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISM�dio - Avalia��o complementar";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>Informamos que seu cadastro ja esta liberado no SIMEC, e � obrigat�rio que voc� preencha o espa�� de avalia��o complementar. Verificamos que voc� n�o fez a avalia��o do per�odo de refer�ncia: <b>".$foo['referencia']."</b></p>
		 <p>Para fazer a avalia��o, acesse o SIMEC e clique em Avalia��o Complementar. Em seguida selecione as op��es referentes e aperte o bot�o 'Salvar'.</p>
		 <p>Secretaria de Educa��o B�sica<br/>Minist�rio da Educa��o</p>
		 <br/><br/>
		 <p>ATEN��O � Pacto Nacional pelo Fortalecimento do Ensino M�dio</p>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>";

		 $mensagem->IsHTML( true );
		 $resp = $mensagem->Send();
		 echo "avalia��o equipe _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}

$sql = "select i.iusnome as nome, i.iusemailprincipal as email, usu.ususenha as senha, fu.rfuparcela ||'� Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as referencia from sismedio.identificacaousuario i 
inner join sismedio.tipoperfil t on t.iusd = i.iusd 
inner join sismedio.folhapagamentouniversidade fu on fu.pflcod = t.pflcod and fu.uncid = i.uncid 
inner join sismedio.folhapagamento f on f.fpbid = fu.fpbid 
inner join seguranca.usuario usu ON usu.usucpf = i.iuscpf
inner join public.meses m on m.mescod::integer = f.fpbmesreferencia
left join sismedio.cadernoatividadesrespostas ca on ca.iusd = i.iusd and fu.fpbid = ca.fpbid and ca.caroeproposatividadecadernoformacao is not null
where t.pflcod in(1082,1088) and i.iusstatus='A' and ca.carid is null";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISM�dio - Avalia��o obrigat�ria";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>O MEC possui interesse em conhecer o trabalho feito pelo professor no Pacto Nacional pelo Fortalecimento do Ensino M�dio. E para isso criamos uma atividade na qual voc� deve informar as atividades realizadas. Verificamos que voc� ainda n�o fez essa atividade no per�odo: <b>".$foo['referencia']."</b></p>
		 <p>Para fazer a atividade, acesse o SIMEC e clique em Avalia��o Obrigat�ria. Em seguida clique nas op��es dispon�veis, insira as atividades e complete o formul�rio.</p>
		 <p>Secretaria de Educa��o B�sica<br/>Minist�rio da Educa��o</p>
		 <br/><br/>
		 <p>ATEN��O � Pacto Nacional pelo Fortalecimento do Ensino M�dio</p>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>";

		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "avalia��o equipe _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}


$sql = "SELECT foo.iusnome as nome, foo.iusemailprincipal as email, foo.ususenha as senha, foo.referencia, foo.pfldsc FROM (
	SELECT i.uncid, per.pfldsc, i.iusnome, i.iusemailprincipal, usu.ususenha, t.pflcod, rf.rfuparcela ||'� Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as referencia, CASE WHEN (dd.esdid NOT IN(951,957) OR dd.esdid IS NULL) THEN 'NOK' ELSE 'OK' END as ap
					FROM sismedio.folhapagamento f 
					INNER JOIN sismedio.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid 
					INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia 
					INNER JOIN sismedio.identificacaousuario i ON i.uncid = rf.uncid 
					INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd  AND t.pflcod IN(1076,1190,1078,1077,1081) AND rf.pflcod = t.pflcod
					INNER JOIN sismedio.pagamentoperfil pp ON pp.pflcod = t.pflcod 
					INNER JOIN seguranca.usuario_sistema us ON us.usucpf = i.iuscpf AND us.suscod='A' AND us.sisid=181 
					INNER JOIN seguranca.perfilusuario pu ON pu.usucpf = i.iuscpf AND pu.pflcod = t.pflcod 
					INNER JOIN seguranca.usuario usu ON usu.usucpf = i.iuscpf 
					INNER JOIN seguranca.perfil per ON per.pflcod = t.pflcod 
					LEFT JOIN sismedio.mensario mm ON mm.iusd = i.iusd AND mm.fpbid = f.fpbid and mm.pflcod = t.pflcod 
					LEFT JOIN workflow.documento dd ON dd.docid = mm.docid 
					WHERE f.fpbstatus='A' AND to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd')
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
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISM�dio - Avalia��o da equipe";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		 <p>Informamos que seu cadastro ja esta liberado no SIMEC, e � fundamental que voc� fa�a avalia��es sobre a equipe do projeto Pacto Nacional pelo Fortalecimento do Ensino M�dio. Verificamos que voc� n�o fez a avalia��o do per�odo de refer�ncia: <b>".$foo['referencia']."</b></p>
		 <p>Para fazer a avalia��o, acesse a aba de Execu��o e clique em Avaliar Equipe. Em seguida selecione as op��es referentes a Frequ�ncia (caso seja obrigat�rio), Atividades Realizadas (caso seja obrigat�rio) e aperte o bot�o 'Salvar'.</p>
		 <p>Em seguida no �cone 'Enviar para an�lise'. Este passo � muito importante para a nota de monitoramento (parte da nota total da avalia��o).</p>
		 <p>Secretaria de Educa��o B�sica<br/>Minist�rio da Educa��o</p>
		 <br/><br/>
		 <p>ATEN��O � Pacto Nacional pelo Fortalecimento do Ensino M�dio</p>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>";

		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "avalia��o equipe _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}

$sql = "UPDATE seguranca.agendamentoscripts SET agstempoexecucao='".round((getmicrotime() - $microtime),2)."' WHERE agsfile='sismedio_enviaremails_alertas.php'";
$db->executar($sql);
$db->commit();


$db->close();

?>