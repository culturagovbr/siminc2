<?php

$dataInicial = date( "Y-m-d H:i:s" );
header( "Content-Type: text/plain;" );

/*
select * from monitora.referencia
where refsnmonitoramento = true and refdata_limite_avaliacao_aca >= now()
order by refano_ref, refmes_ref
*/

function pegarAcoesSemParecerMes( $ano, $mes )
{
	global $db;
	$ano = (integer) $ano;
	$mes = sprintf( "%02d", $mes );
	$sql = "
		select
			a.acaid,
			a.unicod, uni.unidsc,
			a.prgcod, pro.prgdsc,
			a.acacod, a.acadsc,
			a.loccod, a.sacdsc as locdsc,
			u.usunome, u.usuemail, u.usucpf, u.ususexo
		from monitora.referencia r
			left join monitora.acao a on
				a.acasnrap = false and
				a.prgano = r.refano_ref
			left join monitora.avaliacaoparecer p on
				p.refcod = r.refcod and
				p.acaid = a.acaid and
				p.avpliberada = true and
				p.tpaid = 1
			inner join monitora.usuarioresponsabilidade ur on
				ur.acaid = a.acaid
			inner join seguranca.usuario u on
				u.usucpf = ur.usucpf
			inner join public.unidade uni on
				uni.unicod = a.unicod and
				uni.unitpocod = 'U' and
				uni.unistatus = 'A'
			inner join monitora.programa pro on
				pro.prgcod = a.prgcod and
				pro.prgano = a.prgano and
				pro.prgid = a.prgid
		where
			r.refdata_limite_parecer_aca is null and
			r.refsngrupo = false and
			a.acadscproduto is not null and
			a.acastatus = 'A' and
			p.avpid is null and
			r.refano_ref = '" . $ano . "' and
			r.refmes_ref = '" . $mes . "' and
			ur.rpustatus = 'A' and
			ur.pflcod = 1 and
			u.usustatus = 'A'
	";
	$dados = $db->carregar( $sql );
	return $dados ? $dados : array();
}

function pegarAcoesSemParecer()
{
	// carrega os meses/anos que ainda � poss�vel fazer an�lise
	$sql = "
		select refmes_ref, refano_ref
		from monitora.referencia
		--where refsnmonitoramento = true and refdata_limite_avaliacao_aca >= now()
		order by refano_ref, refmes_ref
	";
	global $db;
	$periodos = $db->carregar( $sql );
	$periodos = $periodos ? $periodos : array();
	
	//dump( $periodos, true );
	
	//$periodos = array( array( "refmes_ref" => "4", "refano_ref" => "2008" ) );
	
	$dados = array();
	foreach ( $periodos as $periodo )
	{
		$mesAtual = (integer) $periodo['refmes_ref'];
		$anoAtual = $periodo['refano_ref'];
		$chaveMesAno = $anoAtual . "-" . $mesAtual;
		// carrega as a��es sem parecer de um determinado m�s/ano
		$dadosMes = pegarAcoesSemParecerMes( $anoAtual, $mesAtual );
		foreach ( $dadosMes as $dadosMesLinha )
		{
			// agrupa a��es por usu�rio
			$usucpf = $dadosMesLinha['usucpf'];
			if ( !array_key_exists( $usucpf, $dados ) )
			{
				$dados[$usucpf]             = array();
				$dados[$usucpf]['usucpf']   = $usucpf;
				$dados[$usucpf]['usunome']  = $dadosMesLinha['usunome'];
				$dados[$usucpf]['usuemail'] = $dadosMesLinha['usuemail'];
				$dados[$usucpf]['ususexo']  = $dadosMesLinha['ususexo'];
				$dados[$usucpf]['periodos'] = array();
			}
			// agrupa por data mes/ano para cada usu�rio
			if ( !array_key_exists( $chaveMesAno, $dados[$usucpf]['periodos'] ) )
			{
				$dados[$usucpf]['periodos'][$chaveMesAno] = array();
			}
			$acao = array(
				"acaid"  => $dadosMesLinha['acaid'],
				"prgcod" => $dadosMesLinha['prgcod'],
				"acacod" => $dadosMesLinha['acacod'],
				"unicod" => $dadosMesLinha['unicod'],
				"loccod" => $dadosMesLinha['loccod'],
				"acadsc" => $dadosMesLinha['acadsc'],
				"locdsc" => $dadosMesLinha['locdsc']
			);
			array_push( $dados[$usucpf]['periodos'][$chaveMesAno], $acao );
		}
	}
	return $dados;
}

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

error_reporting( E_ALL );

$nome_bd     = '';
$servidor_bd = '';
$porta_bd    = '5432';
$usuario_db  = '';
$senha_bd    = '';

$_SESSION['mnuid'] = 1;
$_SESSION['sisid'] = 1;
$_SESSION['usucpforigem'] = "";

$db = new cls_banco();

$pendencias = pegarAcoesSemParecer();

$meses = array(
	"1" => "Janeiro",	"2" => "Favereiro",	"3" => "Mar�o",		"4" => "Abril",
	"5" => "Maio",		"6" => "Junho",		"7" => "Julho",		"8" => "Agosto",
	"9" => "Setembro",	"10" => "Outubro",	"11" => "Novembro",	"12" => "Dezembro"
);

require APPRAIZ . "includes/Email.php";

$enviador = new Email();

$frase =
	"<br/>".
	"Existem a��es que n�o foram avaliadas ou liberados no sistema.<br/>" .
	"Abaixo a lista de a��es (Unidade, Programa, A��o, Localizador) seguidas de seus per�odos pendentes:<br/>";

$dataAtual = date( "d" ) . " de " . $meses[date( "n" )] . " de " . date( "Y" );

foreach ( $pendencias as $itemUsu )
{
	$fraseAcao = "";
	$acaids = array();
	$fraseAcao = "";
	foreach ( $itemUsu['periodos'] as $chave => $acoes )
	{
		$data = explode( "-", $chave );
		$mes = $meses[$data[1]];
		$ano = $data[0];
		$data = $mes . " / " . $ano;
		$frasePeriodo = "<font color=\"#dd3030\">" . $data . "</font><br/>";
		foreach ( $acoes as $acao )
		{
			$frasePeriodo .=
				$acao['prgcod'] . "." .
				$acao['acacod'] . "." .
				$acao['unicod'] . "." .
				$acao['loccod'] . " " .
				$acao['acadsc'] . " " .
				"(" . $acao['locdsc'] . ")<br/>";
			array_push( $acaids, $acao['acaid'] );
		}
		$fraseAcao .= "<br/>" . $frasePeriodo;
	}
	$email    = $itemUsu['usuemail'];
	$cpf      = $itemUsu['usucpf'];
	//$assunto  = "Lembrete de pend�ncias";
	switch ( strtoupper( $itemUsu['ususexo'] ) )
	{
		case 'M':
			$nome_pre = "Coordenador";
			$nome_pos = "Prezado Coordenador";
			break;
		case 'F':
			$nome_pre = "Coordenadora";
			$nome_pos = "Prezada Coordenadora";
			break;
		default:
			$nome_pre = "Coordenador(a)";
			$nome_pos = "Prezado(a) Coordenador(a)";
			break;
	}
	$nome = $itemUsu['usunome'];
	
	global $data;
	
	$assunto = "Atualiza��o de informa��es no SIMEC";
	
	$mensagem = <<<EOT
		<p align="center">
		    <b>
		        MINIST�RIO DA EDUCA��O
		        <br/>
		        SECRETARIA EXECUTIVA
		        <br/>
		        SUBSECRETARIA DE PLANEJAMENTO E OR�AMENTO
		    </b>
		</p>
		<br/><br/>
		<p align="left">
		    Circular Eletr�nica SPO/SE/MEC
		</p>
		<p align="right">
		    Bras�lia, $dataAtual.
		</p>
		<p align="left">
		    DE: Subsecret�rio de Planejamento e Or�amento, Substituto
		    <br/>
		    PARA: $nome, $nome_pre de A��es dos Programas do MEC 
		</p>
		<p align="left">
		    Assunto: $assunto
		</p>
		<p align="left">
		    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
		    $nome_pos de A��o,
		</p>
		<p align="left">
		    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
		    Esta � uma mensagem eletr�nica e ser� enviada automaticamente pelo Sistema de Planejamento, Or�amento e Finan�as (SIMEC), a todos (as) Coordenadores (as) de A��o, deste Minist�rio, no inicio de cada m�s, com o objetivo de lembr�-lo (a) do preenchimento da execu��o f�sica e avalia��o das a��es sob sua responsabilidade, no M�dulo de Monitoramento e Avalia��o do Sistema.
		</p>
		<p align="left">
		    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
		    Caso j� tenha efetuado os devidos registros no SIMEC, por favor, desconsidere a mensagem.
		    Para maiores informa��es entre em contato com a Unidade de Monitoramento e Avalia��o (UMA) da Coordena��o Geral de Planejamento da SPO, via  mensagem eletr�nica para o SIMEC (<a href="http://simec.mec.gov.br">http://simec.mec.gov.br</a>) ou (<a href="mailto:spo_planejamento@mec.gov.br">spo_planejamento@mec.gov.br</a>).
		</p>
		<p align="left">
			$fraseAcao
		</p>
		<p align="left">
		    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
		    Atenciosamente,
		</p>
		<br/><br/>
		<p align="center">
		    <b>
		        Paulo Eduardo Nunes de Moura Rocha
		        <br/>
		        Subsecret�rio de Planejamento e Or�amento
		    </b>
		</p>
EOT;
/*
		"<br/>" .
		//$nome . ",<br/><br/>" .
		$nome . " ( " . $cpf . " ) " . ",<br/><br/>" .
		"Esta � uma mensagem de aviso autom�tica do SIMEC indicando que voc� possui pend�ncias referente ao monitoramento das a��es sob sua responsabilidade.<br/>" .
		$frase .
		$fraseAcao . "<br/>" .
		"Obs.: Caso o preenchimento j� tenha diso efetuado, favor desconsiderar o mesmo. " .
		"Caso ainda o " . $nome_pos . " n�o seja respons�vel por estas a��es favor entrar em contato atrav�s do e-mail abaixo solicitando sua desvincula��o.<br/>" .
		"<br/>" .
		"<font style=\"color: #909090;\">" .
			"--<br/>" .
			"SIMEC - Sistema Integrado de Planejamento Or�amento e Finan�as<br/>" .
			"Minist�rio da Educa��o<br/>" .
			"http://simec.mec.gov.br<br/>" .
			"e-mail: simec@mec.gov.br<br/>" .
		"</font>" .
		"<br/>";
*/
	
	/*
	// TESTE EXIBE MENSAGEM
	echo $mensagem;
	exit();
	*/	
	
	
	// TESTE ENVIA
	//if ( $cpf == "" )
	//{
		$enviador->enviar( array( "" ), $assunto, $mensagem );
	//}
	
	
	// PRODUCAO
	//$enviador->enviar( array( $cpf ), $assunto, $mensagem );
	echo $cpf . "\t" . $email . "\t" . implode( "\t", array_unique( $acaids ) ) . "\n";
}

echo "\n" . $dataInicial . "\n" . date( "Y-m-d H:i:s" ) . "\n";

//$db->commit();
$db->rollback();

?>
