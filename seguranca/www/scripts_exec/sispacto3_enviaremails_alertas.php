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
require_once APPRAIZ . "www/sispacto3/_constantes.php";
require_once APPRAIZ . "www/sispacto3/_funcoes.php";
require_once APPRAIZ . "www/sispacto3/_funcoes_coordenadorlocal.php";

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';


// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

function getmicrotime() {list($usec, $sec) = explode(" ", microtime()); return ((float)$usec + (float)$sec);}

$microtime = getmicrotime();

   
// abre conex��o com o servidor de banco de dados
$db = new cls_banco();

/*
 * ENVIANDO EMAIL PARA OS BOLSISTA QUE SELECIONARAM AG�NCIAS QUE N�O TRABALHAM MAIS COM O MEC
*/

$sql = "select distinct i.iusd, i.iuscpf, i.iusnome as nome, i.iusemailprincipal as email, i.iusagenciasugerida, s.agencia, i.cadastradosgb
		from sispacto3.bolsistaserroagencia s
		inner join sispacto3.identificacaousuario i on i.iuscpf = trim(s.cpf)
		inner join sispacto3.tipoperfil t on t.iusd = i.iusd
		inner join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.suscod='A' AND us.sisid=".SIS_SISPACTO."
		where trim(i.iusagenciasugerida) = trim(s.agencia) and i.iusstatus='A'";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2015 - Problemas com ag�ncia banc�ria";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>Foi identificado problema com a ag�ncia selecionada no cadastro do SISPACTO 2015. Possivelmente sua ag�ncia foi invalidada ou desativada do programa.</p>
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



/*
 * ALERTANDO TODOS OS PERFIS COM ACESSO AO SISPACTO DE PREENCHER O TERMO DE COMPROMISSO
*/

$sql = "select i.iusnome as nome, i.iusemailprincipal as email, u.ususenha as senha from sispacto3.identificacaousuario i
		inner join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.suscod='A' and us.sisid=".SIS_SISPACTO."
		inner join sispacto3.tipoperfil t on t.iusd = i.iusd
		inner join seguranca.perfilusuario pu on i.iuscpf = pu.usucpf and t.pflcod = pu.pflcod
		inner join seguranca.usuario u on u.usucpf = i.iuscpf
		where i.iusstatus='A' and i.iustermocompromisso is null and i.uncid in(

		SELECT u.uncid FROM sispacto3.universidadecadastro u
		INNER JOIN workflow.documento d2 ON d2.docid = u.docidformacaoinicial
		WHERE d2.esdid='".ESD_FECHADO_FORMACAOINICIAL."'

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
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2015 -  Preenchimento dos dados cadastrais";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>Informamos que seu acesso ja esta liberado no SIMEC. Solicitamos que acesse o sistema e preencha os dados solicitados para o recebimento da bolsa.</p>
		<p>Secretaria de Educa��o B�sica<br/>Minist�rio da Educa��o</p>
		<br/><br/>
		<p>ATEN��O � PACTO NACIONAL DE ALFABETIZA��O NA IDADE CERTA</p>
		<p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>
				";

				$mensagem->IsHTML( true );
				$resp = $mensagem->Send();
				echo "Preenchimento termo de compromisso _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}


/*
 * ALERTANDO OS COORDENADORES LOCAIS DE CADASTRAR OS PROFESSORES ALFABETIZADORES
*/

$sql = "select u.iusnome as nome, u.iusemailprincipal as email, us.ususenha as senha from sispacto3.identificacaousuario u
		inner join seguranca.usuario_sistema uss on uss.usucpf = u.iuscpf and uss.suscod='A' and uss.sisid=182
		inner join sispacto3.tipoperfil t on t.iusd = u.iusd
		inner join sispacto3.pactoidadecerta p on p.picid = u.picid
		inner join seguranca.usuario us on us.usucpf = u.iuscpf
		left join workflow.documento d on d.docid = p.docid
		left join workflow.documento d2 on d2.docid = p.docidturma
		where u.iusstatus='A' AND t.pflcod='".PFL_COORDENADORLOCAL."' and (d.esdid=".ESD_ANALISE_COORDENADOR_LOCAL." or d.esdid=".ESD_VALIDADO_COORDENADOR_LOCAL.") and d2.esdid!='".ESD_FECHADO_TURMA."'";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2015 - Cadastramento dos Professores Alfabetizadores";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) Coordenador(a) Local - {$foo['nome']},</p>
		<p>O cadastramento dos Orientadores de Estudo est� em an�lise, por�m esta liberado o cadastramento dos professores alfabetizadores.</p>
		<p>Para cadastrar os professores alfabetizadores, basta acessar a plataforma do SIMEC => SISPACTO 2015. Em seguida ir na aba de \"Turmas\", clicar no orientador e inserir os CPFs na turma deste. Para facilitar o cadastramento, existe a op��o de importar os Professores Alfabetizadores do SISPACTO 2014.</p>
		<p>N�o se esque�a ao final do cadastramento, voc� deve clicar no link \"Concluir composi��o de turma\".</p>
		<p>Secretaria de Educa��o B�sica<br/>Minist�rio da Educa��o</p>
		<br/><br/>
		<p>ATEN��O � PACTO NACIONAL DE ALFABETIZA��O NA IDADE CERTA</p>
		<p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>
				";

		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Cadastramento dos professores _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}



/*
 * ALERTANDO OS COORDENADORES LOCAIS DE CADASTRAR OS ORIENTADORES DE ESTUDO
*/


$sql = "select u.iusnome as nome, u.iusemailprincipal as email, us.ususenha as senha from sispacto3.identificacaousuario u
		inner join seguranca.usuario_sistema uss on uss.usucpf = u.iuscpf and uss.suscod='A' and uss.sisid=182
		inner join sispacto3.tipoperfil t on t.iusd = u.iusd
		inner join sispacto3.pactoidadecerta p on p.picid = u.picid
		inner join seguranca.usuario us on us.usucpf = u.iuscpf
		left join workflow.documento d on d.docid = p.docid
		where u.iusstatus='A' AND t.pflcod='".PFL_COORDENADORLOCAL."' and (d.esdid=".ESD_ELABORACAO_COORDENADOR_LOCAL." or d.esdid is null)";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2015 - Cadastramento dos Orientadores de Estudo";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) Coordenador(a) Local - {$foo['nome']},</p>
		<p>Seu Munic�pio/Estado esta participando do Pacto Nacional pela Alfabetiza��o na Idade Certa - 2015, por�m identificamos que n�o foi conclu�do o cadastramento dos Orientadores de Estudo.</p>
		<p>Para cadastrar os orientadores de estudo, basta acessar a plataforma do SIMEC => SISPACTO 2015. Em seguida ir na aba de \"Definir Orientadores de Estudo\" e inserir os CPFs do perfil. Para facilitar o cadastramento, existe a op��o de importar os Orientadores de Estudo do SISPACTO 2014.</p>
		<p>N�o se esque�a ao final do cadastramento, voc� deve clicar na aba \"Resumo Orientadores de Estudo\" e clicar no link \"Enviar para an�lise\".</p>
		<p>Secretaria de Educa��o B�sica<br/>Minist�rio da Educa��o</p>
		<br/><br/>
		<p>ATEN��O � PACTO NACIONAL DE ALFABETIZA��O NA IDADE CERTA</p>
		<p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>
				";

		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Cadastramento dos orientadores _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}


$sql = "UPDATE seguranca.agendamentoscripts SET agstempoexecucao='".round((getmicrotime() - $microtime),2)."' WHERE agsfile='sispacto3_enviaremails_alertas.php'";
$db->executar($sql);
$db->commit();

$db->close();

?>