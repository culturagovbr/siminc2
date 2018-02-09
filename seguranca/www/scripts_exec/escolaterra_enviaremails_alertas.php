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

$sql = "SELECT i.iusnome as nome, i.iusemailprincipal as email, u.ususenha as senha, sum(foo.qtd) as qtd FROM (
SELECT (SELECT i.iusid FROM escolaterra.identificacaousuario i INNER JOIN escolaterra.tipoperfil t ON i.iusid=t.iusid WHERE ii.ufpid=i.ufpid AND t.pflcod=1020) as iusidcoordenadorestadual, count(distinct ii.iusid) as qtd from escolaterra.relatorioacompanhamento r 
INNER JOIN escolaterra.identificacaousuario ii on ii.iusid = r.iusid 
INNER JOIN workflow.documento d on d.docid = r.docid 
WHERE d.esdid=856 and (ii.iusrede is null or ii.muncodatuacao is null)
group by ii.ufpid
) foo 
LEFT JOIN escolaterra.identificacaousuario i ON i.iusid = foo.iusidcoordenadorestadual
LEFT JOIN seguranca.usuario u 				 ON u.usucpf = i.iuscpf 
GROUP BY i.iusnome, i.iusemailprincipal, u.ususenha";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - Forma��o Escola da Terra - Cadastramento dos munic�pios e redes de atua��o dos tutores";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>Um requisito obrigat�rio para o pagamento da bolsa no programa \"Escola da Terra\" � o munic�pio e a rede(estadual ou municipal) de atua��o dos tutores.</p>
		 <p>Para preencher esse requisito, acesse a aba de \"Coordenador estadual - Cadastramento\" e clique em \"Cadastrar Tutores\" e clique no �cone de \"seta\" das colunas \"Munic�pio de atua��o\" e \"Rede\". Selecione as informa��es obrigat�rias e ao final aperte o bot�o 'Salvar'.</p>
		 <p>Equipe Escola da Terra<br/>Minist�rio da Educa��o</p>
		 <br/><br/>
		 <p>ATEN��O � Forma��o Escola da Terra</p>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>";

		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "preencher atuacao _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}


$sql = "SELECT i.iusnome as nome, i.iusemailprincipal as email, u.ususenha as senha, sum(foo.qtd) as qtd FROM (
SELECT (SELECT i.iusid FROM escolaterra.identificacaousuario i INNER JOIN escolaterra.tipoperfil t ON i.iusid=t.iusid WHERE ii.ufpid=i.ufpid AND t.pflcod=1020) as iusidcoordenadorestadual, 1 as qtd from escolaterra.relatorioacompanhamento r 
INNER JOIN escolaterra.identificacaousuario ii on ii.iusid = r.iusid 
INNER JOIN workflow.documento d on d.docid = r.docid 
WHERE d.esdid=855
) foo 
LEFT JOIN escolaterra.identificacaousuario i ON i.iusid = foo.iusidcoordenadorestadual
LEFT JOIN seguranca.usuario u 				 ON u.usucpf = i.iuscpf 
GROUP BY i.iusnome, i.iusemailprincipal, u.ususenha";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - Forma��o Escola da Terra - An�lise dos relat�rios";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>Alguns tutores ja enviaram o relat�rio mensal sobre o andamento dos professores no programa Escola da Terra. Verificamos que voc� possui <b>".$foo['qtd']."</b> para serem analisados dentro os per�odos de refer�ncias do seu Estado</p>
		 <p>Para fazer a an�lise, acesse a aba de \"Coordenador estadual - Execu��o\" e clique em \"Analisar Relat�rio Tutores\" e selecione um per�odo de refer�ncia. Clique no �cone da \"lupa\" para visualizar o relat�rio, em seguida marque a op��o \"Liberar para pagamento\" ou \"Retornar para tutor\" (caso tenha altera��o a ser feita) , ao final aperte o bot�o 'Salvar'.</p>
		 <p>Equipe Escola da Terra<br/>Minist�rio da Educa��o</p>
		 <br/><br/>
		 <p>ATEN��O � Forma��o Escola da Terra</p>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>";

		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "analise relatorio _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}


$sql = "select i.iusnome as nome, i.iusemailprincipal as email, usu.ususenha as senha,  p.rfuparcela ||'� Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as referencia from escolaterra.identificacaousuario i 
inner join escolaterra.tipoperfil t on t.iusid = i.iusid 
inner join escolaterra.periodoreferenciauf p on p.ufpid = i.ufpid 
inner join escolaterra.periodoreferencia f on f.fpbid = p.fpbid 
inner join public.meses m on m.mescod::integer = f.fpbmesreferencia 
inner join seguranca.usuario usu ON usu.usucpf = i.iuscpf
left join escolaterra.relatorioacompanhamento r on r.iusid = i.iusid and r.fpbid = f.fpbid 
left join workflow.documento d on d.docid = r.docid 
where t.pflcod=1021 and  to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd') and (r.racid is null or d.esdid=854)
";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - Forma��o Escola da Terra - Avalia��o da equipe";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>� fundamental que voc� fa�a o conjuneto de avalia��es sobre os professores do projeto Forma��o Escola da Terra. Verificamos que voc� n�o fez a avalia��o do per�odo de refer�ncia: <b>".$foo['referencia']."</b></p>
		 <p>Para fazer a avalia��o, acesse a aba de \"Tutor - Execu��o\" e clique em \"Acompanhar Professores\". Clique no �cones (Tempo Universidade e Tempo Escola Comunidade) e responda as informa��es solicitadas, ao final aperte o bot�o 'Salvar' (Lembre-se que nesta tela o tutor insere informa��es sobre os professores).</p>
		 <p>Em seguida clique na aba \"Relat�rio de Acompanhamento\" e preencha as informa��es solicitadas (neste caso ser�o informa��es sobre o trabalho do tutor). Ao final cliqueno no �cone 'Enviar para an�lise'. Este passo � muito importante para o tramite da bolsa.</p>
		 <p>Equipe Escola da Terra<br/>Minist�rio da Educa��o</p>
		 <br/><br/>
		 <p>ATEN��O � Forma��o Escola da Terra</p>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>";

		 $mensagem->IsHTML( true );
		 $resp = $mensagem->Send();
		 echo "execu��o _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}


$sql = "SELECT i.iusnome as nome, i.iusemailprincipal as email, u.ususenha as senha
		FROM (
		SELECT u.ufpid,
			   u.estuf,
			   (SELECT i.iusid FROM escolaterra.identificacaousuario i INNER JOIN escolaterra.tipoperfil t ON i.iusid=t.iusid WHERE i.ufpid=u.ufpid AND t.pflcod=1020) as iusidcoordenadorestadual
		FROM escolaterra.ufparticipantes u 
		WHERE ufpstatus='A'
		) foo 
		LEFT JOIN escolaterra.identificacaousuario i ON i.iusid = foo.iusidcoordenadorestadual
		LEFT JOIN seguranca.usuario u 				 ON u.usucpf = i.iuscpf 
		LEFT JOIN escolaterra.turmas t               ON t.iusid = i.iusid 
		LEFT JOIN workflow.documento d               ON d.docid = t.docid 
		LEFT JOIN workflow.estadodocumento e         ON e.esdid = d.esdid 
		WHERE d.esdid='851'";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - Escola da terra - Cadastramento de tutores";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado ({$foo['nome']})</p>,
						   <p>O sistema se encontra dispon�vel para o cadastramento dos tutores. Acesse a plataforma \"http://simec.mec.gov.br\", entre com seus dados de acesso e cadastre os tutores que participam do programa.</p>
						   <p>Ao final do cadastramento (quando todos estiverem cadastrados), clique em \"Enviar para an�lise\", para sinalizar que o cadastramento foi finalizado.</p>
				       	   <p>Atenciosamente,<br>Equipe Escola da Terra</p>
						    <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>";

		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Escola da terra cadastramento _ ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}

$sql = "select i.iusnome as nome, i.iusemailprincipal as email, u.ususenha as senha from escolaterra.identificacaousuario i 
inner join escolaterra.tipoperfil t on t.iusid = i.iusid 
inner join seguranca.usuario u on u.usucpf = i.iuscpf 
where i.iusstatus='A' and i.iustermocompromisso is null and t.pflcod=1021 and ufpid in(

SELECT foo.ufpid
		FROM (
		SELECT u.ufpid,
			   u.estuf,
			   (SELECT i.iusid FROM escolaterra.identificacaousuario i INNER JOIN escolaterra.tipoperfil t ON i.iusid=t.iusid WHERE i.ufpid=u.ufpid AND t.pflcod=1020) as iusidcoordenadorestadual
		FROM escolaterra.ufparticipantes u 
		WHERE ufpstatus='A'
		) foo 
		LEFT JOIN escolaterra.identificacaousuario i ON i.iusid = foo.iusidcoordenadorestadual
		LEFT JOIN escolaterra.turmas t               ON t.iusid = i.iusid 
		LEFT JOIN workflow.documento d               ON d.docid = t.docid 
		LEFT JOIN workflow.estadodocumento e         ON e.esdid = d.esdid 
		WHERE (d.esdid='853' or d.esdid is null)



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
		$mensagem->Subject 		= SIGLA_SISTEMA. " - Escola da terra - Preenchimento dos dados pessoais";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		 <p>Informamos que seu acesso ja esta liberado no SIMEC. Solicitamos que acesse o sistema e preencha os dados solicitados para o recebimento da bolsa.</p>
		 <p>Secretaria de Educa��o B�sica<br/>Minist�rio da Educa��o</p>
		 <br/><br/>
		 <p>Equipe Escola da Terra</p>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Escola da terra preenchimento dados pessoais _ ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}

$sql = "select i.iusnome as nome, i.iusemailprincipal as email, u.ususenha as senha from escolaterra.identificacaousuario i 
inner join escolaterra.tipoperfil t on t.iusid = i.iusid 
inner join seguranca.usuario u on u.usucpf = i.iuscpf 
left join escolaterra.turmas tt               ON tt.iusid = i.iusid 
left join workflow.documento d               ON d.docid = tt.docid 
left join workflow.estadodocumento e         ON e.esdid = d.esdid 
where i.iusstatus='A' and (d.esdid='851' or d.esdid is null) and t.pflcod=1021 and ufpid in(

SELECT foo.ufpid
		FROM (
		SELECT u.ufpid,
			   u.estuf,
			   (SELECT i.iusid FROM escolaterra.identificacaousuario i INNER JOIN escolaterra.tipoperfil t ON i.iusid=t.iusid WHERE i.ufpid=u.ufpid AND t.pflcod=1020) as iusidcoordenadorestadual
		FROM escolaterra.ufparticipantes u 
		WHERE ufpstatus='A'
		) foo 
		LEFT JOIN escolaterra.identificacaousuario i ON i.iusid = foo.iusidcoordenadorestadual
		LEFT JOIN escolaterra.turmas t               ON t.iusid = i.iusid 
		LEFT JOIN workflow.documento d               ON d.docid = t.docid 
		LEFT JOIN workflow.estadodocumento e         ON e.esdid = d.esdid 
		WHERE d.esdid='853'



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
		$mensagem->Subject 		= SIGLA_SISTEMA. " - Escola da terra - Cadastramento dos professores cursistas";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado ({$foo['nome']})</p>,
						   <p>O sistema se encontra dispon�vel para o cadastramento dos professores cursistas. Acesse a plataforma \"http://simec.mec.gov.br\", entre com seus dados de acesso e cadastre os professores que participam do programa.</p>
						   <p>Ao final do cadastramento (quando todos estiverem cadastrados), clique em \"Enviar para an�lise\", para sinalizar que o cadastramento foi finalizado.</p>
				       	   <p>Atenciosamente,<br>Equipe Escola da Terra</p>
						    <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>";
		

		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Escola da terra cadastramento professores _ ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}


$sql = "select i2.iusnome as nome, i2.iusemailprincipal as email, u.ususenha as senha, count(*) as total from escolaterra.identificacaousuario i 
inner join escolaterra.tipoperfil t on t.iusid = i.iusid 
inner join escolaterra.turmas tu on tu.iusid = i.iusid 
inner join workflow.documento d on d.docid = tu.docid 
inner join escolaterra.turmaidusuario ot on ot.iusid = i.iusid 
inner join escolaterra.turmas tu2 on tu2.turid = ot.turid 
inner join escolaterra.identificacaousuario i2 on i2.iusid = tu2.iusid 
inner join seguranca.usuario u on u.usucpf = i2.iuscpf
where t.pflcod=1021 and i.iusstatus='A' and d.esdid=852 
group by i2.iusnome, i2.iusemailprincipal, u.ususenha";


$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - Escola da terra - Valida��o do cadastramento dos professores cursistas";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado ({$foo['nome']})</p>,
		<p>Existem <b>{$foo['total']}</b> tutores da sua rede que ja finalizaram o cadastramento dos professores cursistas e aguardam uma valida��o do coordenador estadual. Para faze-la acesse a plataforma \"http://simec.mec.gov.br\", entre com seus dados de acesso e clique na aba \"Validar Cadastramento dos Professores\".</p>
		<p>Nesta tela, mostrar� a lista de todos os tutores e a situa��o do cadastramento de cada um. Verifique os nomes dos professores e clique em \"Validado\" ou \"Devolver para ajustes\". Ao final clique no bot�o \"Salvar\"</p>
		<p>Atenciosamente,<br>Equipe Escola da Terra</p>
		<p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"N�o cadastrada")."</p>";


				$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Escola da terra valida��o professores _ ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}


?>