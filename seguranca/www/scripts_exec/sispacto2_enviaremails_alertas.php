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
require_once APPRAIZ . "www/sispacto2/_constantes.php";
require_once APPRAIZ . "www/sispacto2/_funcoes.php";
require_once APPRAIZ . "www/sispacto2/_funcoes_coordenadorlocal.php";

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
		from sispacto2.bolsistaserroagencia s
		inner join sispacto2.identificacaousuario i on i.iuscpf = trim(s.cpf)
		inner join sispacto2.tipoperfil t on t.iusd = i.iusd 
		inner join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.suscod='A' AND us.sisid=181
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
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2014 - Problemas com ag�ncia banc�ria";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>Foi identificado problema com a ag�ncia selecionada no cadastro do SISPACTO 2014. Possivelmente sua ag�ncia foi invalidada ou desativada do programa.</p>
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
 * ENVIANDO EMAIL PARA OS PROFESSORES NA QUAL N�O PREENCHERAM AS TURMAS (1 ATIVIDADE)
 */

$sql1 = "select * from (
		select 
		i.iusd,
		i.iusnome as nome,
		i.iusemailprincipal as email,
		(select count(*) from sispacto2.turmasprofessoresalfabetizadores pa where tpastatus='A' and (coalesce(tpatotalmeninos,0)+coalesce(tpatotalmeninas,0))!=0 and pa.iusd=i.iusd) as totalturmas 
		from sispacto2.identificacaousuario i 
		inner join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.suscod='A' and us.sisid=181
		inner join sispacto2.tipoperfil t on t.iusd = i.iusd 
		inner join sispacto2.folhapagamentouniversidade fpu ON fpu.uncid = i.uncid AND fpu.pflcod = t.pflcod and fpu.rfuparcela=1
		inner join sispacto2.folhapagamento f ON f.fpbid = fpu.fpbid 
		where t.pflcod=1118 and i.iusstatus='A' and i.iusstatus='A' and to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd')
		) foo where totalturmas=0";

$foos = $db->carregar($sql1);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2014 - Preenchimento da Atividade Obrigat�ria (1� Parcela)";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
						   <p>Precisamos da informa��o a respeito das turmas na qual voc� � regente em 2014. Solicitamos que acessem o SISPACTO 2014 e preencham as informa��es solicitadas na aba \"Atividades Obrigat�rias\" => 1� Parcela.</p>
						   <p>Inicialmente foi feito uma pr�-carga das turmas de cada professor (Censo Escolar 2013), ao acessar a aba, o professor dever� validar se as turmas carregadas realmente est�o designadas a ele (caso esteja faltando alguma turma, o professor poder� inserir as turmas n�o cadastradas).</p>
						   <p>Com as turmas validadas/cadastradas, o professor dever� inserir as informa��es solicitadas para cada turma designada.</p>
						   <p>Att.<br>Equipe do PACTO</p>
					   	   <p>[ASSIM QUE FOR PREENCHIDO, ESTE E-MAIL N�O SER� MAIS ENVIADO]</p>";
		

		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Professor Dados das turmas _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}

/*
 * ENVIANDO EMAIL PARA OS PROFESSORES NA QUAL N�O RESPONDERAM AS QUEST�ES VINCULADAS A MATEMATICA (2 ATIVIDADE)
*/

$sql2 = "select * from (
		select 
		i.iusd,
		i.iusnome as nome,
		i.iusemailprincipal as email,

		(SELECT CASE WHEN count(DISTINCT a.tpaid) > 0 THEN count(*)/count(DISTINCT a.tpaid) ELSE 0 END as itens 
		 FROM sispacto2.aprendizagemconhecimentoturma a 
		 INNER JOIN sispacto2.aprendizagemconhecimento c ON c.catid = a.catid
		 INNER JOIN sispacto2.turmasprofessoresalfabetizadores t ON t.tpaid = a.tpaid 
		 WHERE t.tpastatus='A' AND tpaconfirmaregencia=true AND c.cattipo='M' AND t.iusd=i.iusd) as aprendizagemMat

		from sispacto2.identificacaousuario i 
		inner join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.suscod='A' and us.sisid=181
		inner join sispacto2.tipoperfil t on t.iusd = i.iusd 
		inner join sispacto2.folhapagamentouniversidade fpu ON fpu.uncid = i.uncid AND fpu.pflcod = t.pflcod and fpu.rfuparcela=2
		inner join sispacto2.folhapagamento f ON f.fpbid = fpu.fpbid 
 
		where i.iusstatus='A' and t.pflcod=1118 and i.iusstatus='A' and to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd')
		) foo where aprendizagemMat!=17";

$foos = $db->carregar($sql2);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2014 - Preenchimento da Atividade Obrigat�ria (2� Parcela)";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>Solicitamos que acessem o SISPACTO 2014 e preencham as informa��es solicitadas na aba \"Atividades Obrigat�rias\" => 2� Parcela.</p>
		<p>Att.<br>Equipe do PACTO</p>
		<p>[ASSIM QUE FOR PREENCHIDO, ESTE E-MAIL N�O SER� MAIS ENVIADO]</p>";


		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Professor Aprendizagem Mat _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}


if(date("w")==6) {

/*
 * ENVIANDO EMAIL PARA OS PROFESSORES QUE INFORMARAM QUE N�O TIVERAM ACESSO AO RESULTADO DA ANA
*/

$sql2 = "SELECT distinct i.iuscpf, i.iusnome nome, i.iusemailprincipal as email FROM sispacto2.impressoesana a 
		 INNER JOIN sispacto2.identificacaousuario i on i.iusd = a.iusd 
		 INNER JOIN sispacto2.tipoperfil t on t.iusd = i.iusd and t.pflcod=1118
		 WHERE i.iusstatus='A' AND imaacessoresultados='N'";

$foos = $db->carregar($sql2);

if($foos[0]) {
foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2014 - Acesso aos resultados da ANA";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>Verificamos que voc� informou no SISPACTO 2014 que n�o teve acesso aos resultados da ANA de sua escola. Por conta do alto n�mero de professores nessa situa��o, disponibilizamos os resultados na ferramenta do SISPACTO 2014.</p>
		<p>Para acessar os resultados, entre em simec.mec.gov.br, digite seu CPF e senha, e clique na aba Resultados ANA 2013. Feito isso, por favor atualize as informa��es na aba Atividades Obrigat�rias => 7� Parcela para n�o recebe mais este e-mail.<p>
		<p>Att.<br>Equipe do PACTO</p>
		<p>[ASSIM QUE FOR PREENCHIDO, ESTE E-MAIL N�O SER� MAIS ENVIADO]</p>";


		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Resultado ANA _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}

}

/*
 * ENVIANDO EMAIL PARA OS PROFESSORES NA QUAL N�O RESPONDERAM AS QUEST�ES VINCULADAS A PORTUGUES (3 ATIVIDADE)
*/

$sql3 = "select * from (
		select
		i.iusd,
		i.iusnome as nome,
		i.iusemailprincipal as email,

		(SELECT CASE WHEN count(DISTINCT a.tpaid) > 0 THEN count(*)/count(DISTINCT a.tpaid) ELSE 0 END as itens
		 FROM sispacto2.aprendizagemconhecimentoturma a
		 INNER JOIN sispacto2.aprendizagemconhecimento c ON c.catid = a.catid
		 INNER JOIN sispacto2.turmasprofessoresalfabetizadores t ON t.tpaid = a.tpaid
		 WHERE t.tpastatus='A' AND tpaconfirmaregencia=true AND c.cattipo='P' AND t.iusd=i.iusd) as aprendizagemPor

		from sispacto2.identificacaousuario i
		inner join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.suscod='A' and us.sisid=181
		inner join sispacto2.tipoperfil t on t.iusd = i.iusd
		inner join sispacto2.folhapagamentouniversidade fpu ON fpu.uncid = i.uncid AND fpu.pflcod = t.pflcod and fpu.rfuparcela=3
		inner join sispacto2.folhapagamento f ON f.fpbid = fpu.fpbid 
		where i.iusstatus='A' and t.pflcod=1118 and i.iusstatus='A' and to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd')
		) foo where aprendizagemPor!=11";

$foos = $db->carregar($sql3);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2014 - Preenchimento da Atividade Obrigat�ria (3� Parcela)";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>Solicitamos que acessem o SISPACTO 2014 e preencham as informa��es solicitadas na aba \"Atividades Obrigat�rias\" => 3� Parcela.</p>
		<p>Att.<br>Equipe do PACTO</p>
		<p>[ASSIM QUE FOR PREENCHIDO, ESTE E-MAIL N�O SER� MAIS ENVIADO]</p>";


		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Professor Aprendizagem Por _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}

/*
 * ENVIANDO EMAIL PARA OS PROFESSORES NA QUAL N�O RESPONDERAM AS QUEST�ES VINCULADAS A RECEBIMENTO DOS MATERIAIS (4 ATIVIDADE)
*/

$sql4 = "select * from (
			select
			i.iusd,
			i.iusnome as nome,
			i.iusemailprincipal as email,
			
			(SELECT mapid FROM sispacto2.materiaisprofessores WHERE iusd=i.iusd) as materiais
			
			from sispacto2.identificacaousuario i 
			inner join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.suscod='A' and us.sisid=181
			inner join sispacto2.tipoperfil t on t.iusd = i.iusd
			inner join sispacto2.folhapagamentouniversidade fpu ON fpu.uncid = i.uncid AND fpu.pflcod = t.pflcod and fpu.rfuparcela=4
			inner join sispacto2.folhapagamento f ON f.fpbid = fpu.fpbid
			where i.iusstatus='A' and t.pflcod=1118 and i.iusstatus='A' and to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd')
			) foo where materiais IS NULL";

$foos = $db->carregar($sql4);

if($foos[0]) {
	foreach($foos as $foo) {
	
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2014 - Preenchimento da Atividade Obrigat�ria (4� Parcela)";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>Solicitamos que acessem o SISPACTO 2014 e preencham as informa��es solicitadas na aba \"Atividades Obrigat�rias\" => 4� Parcela.</p>
		<p>Att.<br>Equipe do PACTO</p>
		<p>[ASSIM QUE FOR PREENCHIDO, ESTE E-MAIL N�O SER� MAIS ENVIADO]</p>";


		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Professor Recebimento Materiais _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}

/*
 * ENVIANDO EMAIL PARA OS PROFESSORES NA QUAL N�O RESPONDERAM AS QUEST�ES VINCULADAS AO USO DOS MATERIAIS (5 ATIVIDADE)
*/

$sql5 = "select * from (
		select
		i.iusd,
		i.iusnome as nome,
		i.iusemailprincipal as email,
			
		(SELECT count(*) as total FROM sispacto2.usomateriaisdidaticos WHERE iusd=i.iusd) as usomateriaisdidaticos
			
		from sispacto2.identificacaousuario i 
		inner join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.suscod='A' and us.sisid=181
		inner join sispacto2.tipoperfil t on t.iusd = i.iusd
		inner join sispacto2.folhapagamentouniversidade fpu ON fpu.uncid = i.uncid AND fpu.pflcod = t.pflcod and fpu.rfuparcela=5
		inner join sispacto2.folhapagamento f ON f.fpbid = fpu.fpbid
		where i.iusstatus='A' and t.pflcod=1118 and i.iusstatus='A' and to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd')
		) foo where usomateriaisdidaticos!=7";

$foos = $db->carregar($sql5);

if($foos[0]) {
	foreach($foos as $foo) {
	
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
						$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2014 - Preenchimento da Atividade Obrigat�ria (5� Parcela)";
		
				$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>Solicitamos que acessem o SISPACTO 2014 e preencham as informa��es solicitadas na aba \"Atividades Obrigat�rias\" => 5� Parcela.</p>
		<p>Att.<br>Equipe do PACTO</p>
		<p>[ASSIM QUE FOR PREENCHIDO, ESTE E-MAIL N�O SER� MAIS ENVIADO]</p>";
		
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Professor Uso Materiais _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}


/*
 * ENVIANDO EMAIL PARA OS PROFESSORES NA QUAL N�O RESPONDERAM AS QUEST�ES VINCULADAS AO RELATO DE EXPERIENCIA (6 ATIVIDADE)
*/

$sql6 = "select * from (
			select
			i.iusd,
			i.iusnome as nome,
			i.iusemailprincipal as email,
			
			(SELECT count(*) as total FROM sispacto2.relatoexperiencia WHERE iusd=i.iusd) as relatoexperiencia
			
			from sispacto2.identificacaousuario i 
			inner join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.suscod='A' and us.sisid=181
			inner join sispacto2.tipoperfil t on t.iusd = i.iusd
			inner join sispacto2.folhapagamentouniversidade fpu ON fpu.uncid = i.uncid AND fpu.pflcod = t.pflcod and fpu.rfuparcela=6
			inner join sispacto2.folhapagamento f ON f.fpbid = fpu.fpbid
			where i.iusstatus='A' and t.pflcod=1118 and i.iusstatus='A' and to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd')
			) foo where relatoexperiencia=0";

$foos = $db->carregar($sql6);

if($foos[0]) {
	foreach($foos as $foo) {
	
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2014 - Preenchimento da Atividade Obrigat�ria (6� Parcela)";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
		
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
						<p>Solicitamos que acessem o SISPACTO 2014 e preencham as informa��es solicitadas na aba \"Atividades Obrigat�rias\" => 6� Parcela.</p>
						<p>Att.<br>Equipe do PACTO</p>
						<p>[ASSIM QUE FOR PREENCHIDO, ESTE E-MAIL N�O SER� MAIS ENVIADO]</p>";
		
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Relato exp _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}

/*
 * ENVIANDO EMAIL PARA OS PROFESSORES NA QUAL N�O RESPONDERAM AS QUEST�ES VINCULADAS AS IMPRESSOES DA ANA (7 ATIVIDADE)
*/

$sql7 = "select * from (
select
i.iusd,
i.iusnome as nome,
i.iusemailprincipal as email,

(SELECT count(*) as total FROM sispacto2.impressoesana WHERE iusd=i.iusd) as impressoesana

from sispacto2.identificacaousuario i 
inner join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.suscod='A' and us.sisid=181
inner join sispacto2.tipoperfil t on t.iusd = i.iusd
inner join sispacto2.folhapagamentouniversidade fpu ON fpu.uncid = i.uncid AND fpu.pflcod = t.pflcod and fpu.rfuparcela=7
inner join sispacto2.folhapagamento f ON f.fpbid = fpu.fpbid
where i.iusstatus='A' and t.pflcod=1118 and i.iusstatus='A' and to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd')
) foo where impressoesana=0";

$foos = $db->carregar($sql7);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2014 - Preenchimento da Atividade Obrigat�ria (7� Parcela)";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );


		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>Solicitamos que acessem o SISPACTO 2014 e preencham as informa��es solicitadas na aba \"Atividades Obrigat�rias\" => 7� Parcela.</p>
		<p>Att.<br>Equipe do PACTO</p>
		<p>[ASSIM QUE FOR PREENCHIDO, ESTE E-MAIL N�O SER� MAIS ENVIADO]</p>";


		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Impressoes ana _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}


/*
 * ENVIANDO EMAIL PARA OS PROFESSORES NA QUAL N�O RESPONDERAM AS QUEST�ES VINCULADAS A MOBILIZA��O (8 ATIVIDADE)
*/

$sql8 = "select * from (
select
i.iusd,
i.iusnome as nome,
i.iusemailprincipal as email,

(SELECT count(*) as total FROM sispacto2.questoesdiversasatv8 WHERE iusd=i.iusd) as questoesdiversasatv8

from sispacto2.identificacaousuario i 
inner join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.suscod='A' and us.sisid=181 
inner join sispacto2.tipoperfil t on t.iusd = i.iusd
inner join sispacto2.folhapagamentouniversidade fpu ON fpu.uncid = i.uncid AND fpu.pflcod = t.pflcod and fpu.rfuparcela=8
inner join sispacto2.folhapagamento f ON f.fpbid = fpu.fpbid
where i.iusstatus='A' and t.pflcod=1118 and i.iusstatus='A' and to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd')
) foo where questoesdiversasatv8=0";

$foos = $db->carregar($sql8);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2014 - Preenchimento da Atividade Obrigat�ria (8� Parcela)";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );


		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>Solicitamos que acessem o SISPACTO 2014 e preencham as informa��es solicitadas na aba \"Atividades Obrigat�rias\" => 8� Parcela.</p>
		<p>Att.<br>Equipe do PACTO</p>
		<p>[ASSIM QUE FOR PREENCHIDO, ESTE E-MAIL N�O SER� MAIS ENVIADO]</p>";


		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "questoes diversas _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}

/*
 * ENVIANDO EMAIL PARA OS PROFESSORES NA QUAL N�O RESPONDERAM AS QUEST�ES VINCULADAS A PORTUGUES (9 ATIVIDADE)
*/

$sql9 = "select * from (
select
i.iusd,
i.iusnome as nome,
i.iusemailprincipal as email,

(SELECT CASE WHEN count(DISTINCT a.tpaid) > 0 THEN count(*)/count(DISTINCT a.tpaid) ELSE 0 END as itens
FROM sispacto2.aprendizagemconhecimentoturma2 a
INNER JOIN sispacto2.aprendizagemconhecimento c ON c.catid = a.catid
INNER JOIN sispacto2.turmasprofessoresalfabetizadores t ON t.tpaid = a.tpaid
WHERE t.tpastatus='A' AND tpaconfirmaregencia=true AND c.cattipo='P' AND t.iusd=i.iusd) as aprendizagemPor

from sispacto2.identificacaousuario i
inner join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.suscod='A' and us.sisid=181
inner join sispacto2.tipoperfil t on t.iusd = i.iusd
inner join sispacto2.folhapagamentouniversidade fpu ON fpu.uncid = i.uncid AND fpu.pflcod = t.pflcod and fpu.rfuparcela=9
inner join sispacto2.folhapagamento f ON f.fpbid = fpu.fpbid
where i.iusstatus='A' and t.pflcod=1118 and i.iusstatus='A' and to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd')
) foo where aprendizagemPor!=11";

$foos = $db->carregar($sql9);

if($foos[0]) {
	foreach($foos as $foo) {
	
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2014 - Preenchimento da Atividade Obrigat�ria (9� Parcela)";
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>Solicitamos que acessem o SISPACTO 2014 e preencham as informa��es solicitadas na aba \"Atividades Obrigat�rias\" => 9� Parcela (Portugu�s).</p>
		<p>Att.<br>Equipe do PACTO</p>
		<p>[ASSIM QUE FOR PREENCHIDO, ESTE E-MAIL N�O SER� MAIS ENVIADO]</p>";
		
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Professor Aprendizagem 2 Por _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}

/*
 * ENVIANDO EMAIL PARA OS PROFESSORES NA QUAL N�O RESPONDERAM AS QUEST�ES VINCULADAS A MATEMATICA (9 ATIVIDADE)
*/

$sql10 = "select * from (
select
i.iusd,
i.iusnome as nome,
i.iusemailprincipal as email,

(SELECT CASE WHEN count(DISTINCT a.tpaid) > 0 THEN count(*)/count(DISTINCT a.tpaid) ELSE 0 END as itens
FROM sispacto2.aprendizagemconhecimentoturma2 a
INNER JOIN sispacto2.aprendizagemconhecimento c ON c.catid = a.catid
INNER JOIN sispacto2.turmasprofessoresalfabetizadores t ON t.tpaid = a.tpaid
WHERE t.tpastatus='A' AND tpaconfirmaregencia=true AND c.cattipo='M' AND t.iusd=i.iusd) as aprendizagemMat

from sispacto2.identificacaousuario i
inner join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.suscod='A' and us.sisid=181
inner join sispacto2.tipoperfil t on t.iusd = i.iusd
inner join sispacto2.folhapagamentouniversidade fpu ON fpu.uncid = i.uncid AND fpu.pflcod = t.pflcod and fpu.rfuparcela=9
inner join sispacto2.folhapagamento f ON f.fpbid = fpu.fpbid
where i.iusstatus='A' and t.pflcod=1118 and i.iusstatus='A' and to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd')
) foo where aprendizagemMat!=17";

$foos = $db->carregar($sql10);

if($foos[0]) {
foreach($foos as $foo) {

$mensagem = new PHPMailer();
$mensagem->persistencia = $db;

$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= SIGLA_SISTEMA;
$mensagem->From 		= "noreply@mec.gov.br";
$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2014 - Preenchimento da Atividade Obrigat�ria (9� Parcela)";

$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>Solicitamos que acessem o SISPACTO 2014 e preencham as informa��es solicitadas na aba \"Atividades Obrigat�rias\" => 9� Parcela (Matem�tica).</p>
		<p>Att.<br>Equipe do PACTO</p>
		<p>[ASSIM QUE FOR PREENCHIDO, ESTE E-MAIL N�O SER� MAIS ENVIADO]</p>";


		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Professor Aprendizagem 2 Mat _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}



/*
 * ENVIANDO EMAIL PARA TODOS OS COORDENADORES IES QUE EST�O COM PROJETO VALIDADO E N�O CONCLUIRAM
 */ 

$sql = "SELECT
		COALESCE((SELECT iusnome FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t ON i.iusd=t.iusd WHERE i.uncid=u.uncid AND t.pflcod=".PFL_COORDENADORIES."),'Coordenador Local n�o cadastrado') as nome,
		COALESCE((SELECT iusemailprincipal FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t ON i.iusd=t.iusd WHERE i.uncid=u.uncid AND t.pflcod=".PFL_COORDENADORIES."),'Coordenador Local n�o cadastrado') as email
		FROM sispacto2.universidadecadastro u
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
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2014 - Preenchimento do Registro de Frequ�ncia aberto da Forma��o Inicial";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) Coordenador(a) da IES - {$foo['nome']},</p>
		<p>Identificamos que o MEC aprovou seu projeto do PACTO.</p>
		<p>A pr�xima tarefa � sinalizar os presentes/ausentes do curso de Forma��o Inicial. Caso tenha comparecido no curso Forma��o Inicial algum Orientador enviado pelo Munic�pio para substituir, � necess�rio cadastrar alguns dados pessoais para que o Munic�pio autorize a substitui��o.</p>
		<p>Para regularizar a situa��o, orientamos que acesse o SisPacto e, na �ltima tela (�Forma��o Inicial�), informe os ausentes/presentes e clique no bot�o �Salvar�. Para inserir substituto clique em 'Inserir Orientadore Substituto'.</p>
		<p>Ao final, n�o esque�a de clicar em \"Concluir registro de frequ�ncia\".</p>
		<p>Secretaria de Educa��o B�sica<br/>Minist�rio da Educa��o</p>
		<br/><br/>
		<p>ATEN��O � PACTO NACIONAL DE ALFABETIZA��O NA IDADE CERTA</p>
		<p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha</p>
				";

				$mensagem->IsHTML( true );
				$resp = $mensagem->Send();
				echo "Forma��o Inicial pelo Coordenador IES _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}


/*
 * ALERTANDO TODOS OS PERFIS COM ACESSO AO SISPACTO DE PREENCHER O TERMO DE COMPROMISSO
*/

$sql = "select i.iusnome as nome, i.iusemailprincipal as email, u.ususenha as senha from sispacto2.identificacaousuario i
		inner join sispacto2.tipoperfil t on t.iusd = i.iusd and t.pflcod=".PFL_COORDENADORIES."
		inner join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.suscod='A' and us.sisid=181
		inner join seguranca.usuario u on u.usucpf = i.iuscpf
		inner join sispacto2.universidadecadastro uc on uc.uncid = i.uncid
		inner join workflow.documento d ON d.docid = uc.docid
		inner join workflow.documento d2 ON d2.docid = uc.docidformacaoinicial
		inner join workflow.documento d3 ON d3.docid = uc.docidturma
		where i.iusstatus='A' and i.iusstatus='A' and d.esdid='".ESD_VALIDADO_COORDENADOR_IES."' and d2.esdid='".ESD_FECHADO_FORMACAOINICIAL."' and d3.esdid!='".ESD_FECHADO_TURMA."'";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2014 -  Preenchimento das Tumas (outros)";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		 <p>Diferentemente do SISPACTO 2013, no SISPACTO 2014 teremos a defini��o da equipe na qual os supervisores e coordenadores locais dever�o avaliar no decorrer do curso. Essas equipe ser�o agrupadas em turmas e dever�o ser cadastradas pelo coordenador geral da IES.</p>
		 <p>As turmas dos supervisores da IES ser�o compostas por Formadores da IES, Formadores da L�ngua Portuguesa e Coordenadores Locais, enquanto as turmas dos coordenadores locais ser�o os Orientadores de Estudo.</p>
		 <p>Por padr�o o sistema ir� criar as turmas dos coordenadores locais automaticamente (para aqueles que possuem apenas 1 coordenador local por munic�pio). Caso tenha 2 ou mais coordenadores locais, estes dever�o dividir os Orientadores de Estudo em turmas.</p>
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
		
		

/*
 * ALERTANDO TODOS OS PERFIS COM ACESSO AO SISPACTO DE PREENCHER O TERMO DE COMPROMISSO
*/

$sql = "select i.iusnome as nome, i.iusemailprincipal as email, u.ususenha as senha from sispacto2.identificacaousuario i 
		inner join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.suscod='A' and us.sisid=".SIS_SISPACTO."
		inner join sispacto2.tipoperfil t on t.iusd = i.iusd 
		inner join seguranca.perfilusuario pu on i.iuscpf = pu.usucpf and t.pflcod = pu.pflcod 
		inner join seguranca.usuario u on u.usucpf = i.iuscpf 
		where i.iusstatus='A' and i.iustermocompromisso is null and i.uncid in(
		
		SELECT u.uncid FROM sispacto2.universidadecadastro u 
		INNER JOIN workflow.documento d ON d.docid = u.docid 
		INNER JOIN workflow.documento d2 ON d2.docid = u.docidformacaoinicial 
		WHERE d.esdid='".ESD_VALIDADO_COORDENADOR_IES."' AND d2.esdid='".ESD_FECHADO_FORMACAOINICIAL."'

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
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2014 -  Preenchimento dos dados cadastrais";

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
		echo "Cadastramento dos orientadores _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}


/*
 * ALERTANDO OS COORDENADORES LOCAIS DE CADASTRAR OS ORIENTADORES DE ESTUDO
*/


$sql = "select u.iusnome as nome, u.iusemailprincipal as email, us.ususenha as senha from sispacto2.identificacaousuario u 
		inner join seguranca.usuario_sistema uss on uss.usucpf = u.iuscpf and uss.suscod='A' and uss.sisid=181
		inner join sispacto2.tipoperfil t on t.iusd = u.iusd 
		inner join sispacto2.pactoidadecerta p on p.picid = u.picid 
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
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2014 - Cadastramento dos Orientadores de Estudo";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) Coordenador(a) Local - {$foo['nome']},</p>
		<p>Seu Munic�pio/Estado esta participando do Pacto Nacional pela Alfabetiza��o na Idade Certa - 2014, por�m identificamos que n�o foi conclu�do o cadastramento dos Orientadores de Estudo.</p>
		<p>Para cadastrar os orientadores de estudo, basta acessar a plataforma do SIMEC => SISPACTO 2014. Em seguida ir na aba de \"Definir Orientadores de Estudo\" e inserir os CPFs do perfil. Para facilitar o cadastramento, existe a op��o de importar os Orientadores de Estudo do SISPACTO 2013.</p>
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


/*
 * ALERTANDO OS COORDENADORES LOCAIS DE CADASTRAR OS PROFESSORES ALFABETIZADORES
 */

$sql = "select u.iusnome as nome, u.iusemailprincipal as email, us.ususenha as senha from sispacto2.identificacaousuario u 
		inner join seguranca.usuario_sistema uss on uss.usucpf = u.iuscpf and uss.suscod='A' and uss.sisid=181
		inner join sispacto2.tipoperfil t on t.iusd = u.iusd 
		inner join sispacto2.pactoidadecerta p on p.picid = u.picid 
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
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2014 - Cadastramento dos Professores Alfabetizadores";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) Coordenador(a) Local - {$foo['nome']},</p>
		<p>O cadastramento dos Orientadores de Estudo est� em an�lise, por�m esta liberado o cadastramento dos professores alfabetizadores.</p>
		<p>Para cadastrar os professores alfabetizadores, basta acessar a plataforma do SIMEC => SISPACTO 2014. Em seguida ir na aba de \"Turmas\", clicar no orientador e inserir os CPFs na turma deste. Para facilitar o cadastramento, existe a op��o de importar os Professores Alfabetizadores do SISPACTO 2013.</p>
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



$sql = "SELECT foo.iusnome as nome, foo.iusemailprincipal as email, foo.ususenha as senha, foo.referencia, foo.pfldsc FROM (
	SELECT i.uncid, per.pfldsc, i.iusnome, i.iusemailprincipal, usu.ususenha, t.pflcod, rf.rfuparcela ||'� Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as referencia, CASE WHEN (dd.esdid NOT IN(".ESD_APROVADO_MENSARIO.",".ESD_ENVIADO_MENSARIO.") OR dd.esdid IS NULL) THEN 'NOK' ELSE 'OK' END as ap
					FROM sispacto2.folhapagamento f 
					INNER JOIN sispacto2.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid 
					INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia 
					INNER JOIN sispacto2.identificacaousuario i ON i.uncid = rf.uncid 
					INNER JOIN seguranca.usuario_sistema uss on uss.usucpf = i.iuscpf and uss.suscod='A' and uss.sisid=181
					INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd  AND t.pflcod IN(1119,1120,1117,1131,1130,1129) AND rf.pflcod = t.pflcod
					INNER JOIN sispacto2.pagamentoperfil pp ON pp.pflcod = t.pflcod 
					INNER JOIN seguranca.perfilusuario pu ON pu.usucpf = i.iuscpf AND pu.pflcod = t.pflcod 
					INNER JOIN seguranca.usuario usu ON usu.usucpf = i.iuscpf 
					INNER JOIN seguranca.perfil per ON per.pflcod = t.pflcod 
					LEFT JOIN sispacto2.mensario mm ON mm.iusd = i.iusd AND mm.fpbid = f.fpbid and mm.pflcod = t.pflcod 
					LEFT JOIN sispacto2.pagamentobolsista pg ON pg.tpeid = t.tpeid AND pg.fpbid = f.fpbid
					LEFT JOIN workflow.documento dd ON dd.docid = mm.docid 
					WHERE pg.pboid IS NULL AND i.iusstatus='A' AND f.fpbstatus='A' AND to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd') AND rf.rfuparcela <= pp.plpmaximobolsas
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
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO 2014 - Avalia��o da equipe";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		 <p>Informamos que seu cadastro ja esta liberado no SIMEC, e � fundamental que voc� fa�a avalia��es sobre membros do projeto PACTO Idade Certa. Verificamos que voc� n�o fez a avalia��o do per�odo de refer�ncia: <b>".$foo['referencia']."</b></p>
		 <p>Para fazer a avalia��o, acesse a aba de Execu��o e clique em Avaliar Equipe. Em seguida selecione as op��es referentes a Frequ�ncia (caso seja obrigat�rio), Atividades Realizadas (caso seja obrigat�rio) e aperte o bot�o 'Salvar'.</p>
		 <p>Em seguida no �cone 'Enviar para an�lise'. Este passo � muito importante para a nota de monitoramento (parte da nota total da avalia��o).</p>
		 <p>Secretaria de Educa��o B�sica<br/>Minist�rio da Educa��o</p>
		 <br/><br/>
		 <p>ATEN��O � PACTO NACIONAL DE ALFABETIZA��O NA IDADE CERTA</p>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>";

		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Cadastramento dos professores _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}

$sql = "UPDATE seguranca.agendamentoscripts SET agstempoexecucao='".round((getmicrotime() - $microtime),2)."' WHERE agsfile='sispacto2_enviaremails_alertas.php'";
$db->executar($sql);
$db->commit();

$db->close();

?>