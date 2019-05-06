<?php

ini_set("memory_limit","250M");
set_time_limit(0);

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );

$diretorio = IS_PRODUCAO ?
	"/var/www/simec/arquivos/SIGPLAN/exportacao/" :
	"/var/www/simec/arquivos/SIGPLAN/exportacao/";

/**
 * @param string $nome
 * @param string $sql
 * @return DOMDocument
 */
function sigplan_criarDocumento( $sql, $nome ){
	global $db;
	# cria documento
	$documento = new DOMDocument( "1.0", "utf-8" );
	$registros = $documento->appendChild( new DOMElement( "ArrayOf{$nome}" ) );
	$registros->setAttribute( "xmlns", "http://www.sigplan.gov.br/xml/" );
	# captura os registros
	$retorno = $db->executar( $sql, false );
	$quantidade = $db->conta_linhas( $retorno );

	if ( $quantidade >= 0 ) {
		# coloca os registro no documento
		for( $i = 0; $i <= $quantidade; $i++ ){
			$registro = $db->carrega_registro( $retorno, $i );
			$acao = $registros->appendChild( new DOMElement( $nome ) );
			foreach ( $registro as $atributo => $valor ) {

				if ( is_null( $valor ) || is_integer( $atributo ) ) {
					continue;
				}

				$valor = $valor == 't' ? 1 : $valor;
				$valor = $valor == 'f' ? 0 : $valor;
				# TODO: verificar a situação dos registros com saccod vazio
				if ( $atributo == "saccod" && $valor == "    " ) {
					continue;
				}
				$valor = str_replace( "'", "&apos;", utf8_encode( simec_htmlspecialchars( $valor ) ) );
				$acao->appendChild( new DOMElement( $atributo, $valor ) );
			}
		}
	} else {
		throw new Exception( "Não há dados para gravar no arquivo {$nome}." );
	}
	return $documento;
}

/**
 * @param string $sql
 * @param string $nome
 * @param string $arquivo
 * @return boolean
 */
function sigplan_gravarDocumento( $sql, $nome, $arquivo ){
	global $diretorio;
	$documento = sigplan_criarDocumento( $sql, $nome );
	$caminho = $diretorio . $arquivo . date( '-d-m-y' ) . ".xml";
	if ( $documento instanceof DOMDocument ) {
		return $documento->save( $caminho ) > 0;
	} else {
		file_put_contents( $caminho, null );
	}
}

function sigplan_prepararAcoes(){
	global $db;
	$avaliacao = array();
$sql = "select
			a.acaid,
			r.refdsc,
			r.refmes_ref,
			ap.avptexto,
			ts.tpsdsc
		from monitora.acao a
			inner join monitora.referencia r on
				r.refano_ref = a.prgano
			inner join monitora.avaliacaoparecer ap on
				ap.acaid = a.acaid
				and ap.refcod = r.refcod
			inner join public.tiposituacao ts on
				ts.tpscod = ap.tpscod
		where
			( a.unicod like '26%' or a.unicod like '74%' ) and
			ap.avpliberada = true and
			ap.tpaid = 1 and
			( ap.avptexto is not null and ap.avptexto != '' ) and
			a.prgano = '".$_SESSION['exercicio']."'
		order by
			r.refmes_ref";
		
	$rs = $db->record_set( $sql );
	$total = $db->conta_linhas( $rs );
	for ( $i = 0; $i <= $total; $i++ ) {
		# formata os campos
		$registro = $db->carrega_registro( $rs, $i );
		$acaid    = (integer) $registro["acaid"];
		$refmes   = (integer) $registro["refmes_ref"];
		$refdsc   = trim( strip_tags( $registro["refdsc"] ) );
		$avptexto = trim( strip_tags( $registro["avptexto"] ) );
		$tpsdsc   = trim( strip_tags( $registro["tpsdsc"] ) );
		# corrige o texto
		$texto = "{$refdsc}\n Situação: {$tpsdsc}\n{$avptexto}";
		$texto = str_replace( "'", "\\'", $texto );
		$texto = str_replace( chr( 147 ), "\"", $texto );
		$texto = str_replace( chr( 148 ), "\"", $texto );
		$texto = str_replace( chr( 150 ), "-", $texto );
		# empilha o texto
		if ( !is_array( $avaliacao[$acaid] ) ) {
			$avaliacao[$acaid] = array();
		}
		$avaliacao[$acaid][$refmes] = $texto;
		# libera memória
		unset( $registro, $acaid, $refmes, $refdsc, $avptexto, $tpsdsc, $texto );
	}
	$sql = "update monitora.acao
			set acadsccomentexecucao = '%s', acadscsituacaoatual = '%s'
			where acaid = %d";
	foreach ( $avaliacao as $acaid => $textos ) {
		# prepara o sql
		$execucao = implode( "\n", $textos );
		$situacao = end( $textos );
		$sql_acao = sprintf( $sql, $execucao, $situacao, $acaid );
		if( !$db->executar( $sql_acao ) ){
			throw new Exception( "Erro ao preparar os dados das ações para exportação." ); 
		}
		# libera memória
		unset( $execucao, $situacao, $sql_acao );
	}
}

function sigplan_prepararProgramas(){}

function sigplan_prepararDadosFisicos(){
	global $db;
$sql = "select
			df.acaid,
			a.acasnmetanaocumulativa,
			e.exprealizado,
			r.refmes_ref
		from monitora.dadofisico df
		inner join monitora.acao a on
			a.acaid = df.acaid and a.prgano = df.prgano and a.acasnrap = false
		inner join monitora.execucaopto e on
			e.acaid = df.acaid
		inner join monitora.referencia r on
			r.refcod = e.refcod and r.refmes_ref::integer <= 12
		where df.prgano = '".$_SESSION["exercicio"]."' and ( a.unicod like '26%' or a.unicod like '74%' ) 
			order by df.acaid, r.refmes_ref";
	
	$acoes = $db->carregaAgrupado( $sql, "acaid" );
	foreach ( $acoes as $acaid => $acao ) {
		$realizado_01 = $realizado_02 = $realizado_03 = $realizado_04 =
		$realizado_05 = $realizado_06 = $realizado_07 = $realizado_08 =
		$realizado_09 = $realizado_10 = $realizado_11 = $realizado_12 = 0;
		$realizado_ano = 0;
		$fisico = array();
		foreach ( $acao as $registro ) {
			$variavel = 'realizado_' . $registro["refmes_ref"];
			${$variavel} = (integer) $registro["exprealizado"];
			if ( $registro["acasnmetanaocumulativa"] == "t" ) {
				if ( $realizado_ano < ${$variavel} ) {
					$realizado_ano = ${$variavel};
				}
			} else {
				$realizado_ano += ${$variavel};
			}
		}
		$sql = sprintf(
			"update monitora.dadofisico set
				FISQtdeRealizado1 = %d, FISQtdeRealizado2 = %d, FISQtdeRealizado3 = %d,
				FISQtdeRealizado4 = %d, FISQtdeRealizado5 = %d, FISQtdeRealizado6 = %d,
				FISQtdeRealizado7 = %d, FISQtdeRealizado8 = %d, FISQtdeRealizado9 = %d,
				FISQtdeRealizado10 = %d, FISQtdeRealizado11 = %d, FISQtdeRealizado12 = %d,
				FISQtdeRealizadoano = %d
			where acaid = %d",
			$realizado_01, $realizado_02, $realizado_03,
			$realizado_04, $realizado_05, $realizado_06,
			$realizado_07, $realizado_08, $realizado_09,
			$realizado_10, $realizado_11, $realizado_12,
			$realizado_ano,
			$acaid
		);
		if( !$db->executar( $sql ) ){
			throw new Exception( "Erro ao preparar os dados financeiros para exportação." ); 
		}
	}
	unset( $acoes );
}

function sigplan_prepararDadosFinanceiros(){}

function sigplan_prepararProvidenciaProgramas(){}

function sigplan_prepararProvidenciaAcoes(){}

function sigplan_prepararValidacaoAcoes(){}

if ( $_REQUEST["formulario"] ) {
	try {
		
		$_REQUEST["documento"] = (array) $_REQUEST["documento"];
		
		# CRIA ARQUIVO DE AÇÕES
		if ( in_array( "acao", $_REQUEST["documento"] ) ) {
			sigplan_prepararAcoes();
		$sql = "select
					a.prgano as \"PRGAno\",
					a.prgcod as \"PRGCod\",
					a.acacod as \"ACACod\",
					a.saccod as \"SACCod\",
					a.loccod as \"LOCCod\",
					a.procod as \"PROCod\",
					a.unicod as \"UNICod\",
					a.sitcodestagio as \"SITCodEstagio\",
					a.sitcodandamento as \"SITCodAndamento\",
					a.sitcodcronograma as \"SITCodCronograma\",
					a.acapercexecucao as \"ACAPercExecucao\",
					a.acadsccomentarios as \"ACADscComentarios\",
					a.acasnmetanaocumulativa as \"ACASNMetaNaoCumulativa\",
					a.acadsccomentsituacao as \"ACADscComentSituacao\",
					a.acadscsituacaoatual as \"ACADscSituacaoAtual\",
					a.acadscresultadosespobt as \"ACADscResultadosEspObt\",
					a.acamesprevisaoconclusao as \"ACAMesPrevisaoConclusao\",
					a.acaanoprevisaoconclusao as \"ACAAnoPrevisaoConclusao\",
					a.acadsccomentexecucao as \"ACADscComentExecucao\",
					a.acadsccomentexecucaorap as \"ACADscComentExecucaoRap\"
				from monitora.acao a
				where acasnrap = false and ( a.unicod like '26%' or a.unicod like '74%' ) and prgano = '".$_SESSION["exercicio"]."'";
				
			$documento = sigplan_gravarDocumento( $sql, "Acao", "RetornaAcao" );
		}
		
		# CRIA ARQUIVO DE PROGRAMAS
		if ( in_array( "programa", $_REQUEST["documento"] ) ) {
			sigplan_prepararProgramas();
		$sql = "select
					prgano as \"PRGAno\",
					prgcod as \"PRGCod\",
					prgurl as \"PRGUrl\",
					prgdsccontexto as \"PRGDscContexto\",
					to_char( prgdataatucontexto, 'YYYY-MM-DDT' ) || to_char( prgdataatucontexto, 'HH24:MI:SS' ) as \"PRGDataAtuContexto\",
					sitcodestagio as \"SITCodEstagio\",
					sitcodandamento as \"SITCodAndamento\",
					sitcodcronograma as \"SITCodCronograma\",
					prgpercexecucao as \"PRGPercExecucao\",
					prgdsccomentsituacao as \"PRGDscComentSituacao\",
					to_char( prgdataatusituacao, 'YYYY-MM-DDT' ) || to_char( prgdataatusituacao, 'HH24:MI:SS' )  as \"PRGDataAtuSituacao\",
					prgdscsituacaoatual as \"PRGDscSituacaoAtual\",
					prgdscresultadosespobt as \"PRGDscResultadosEspObt\",
					to_char( prgdataatusituacaoatual, 'YYYY-MM-DDT' ) || to_char( prgdataatusituacaoatual, 'HH24:MI:SS' )  as \"PRGDataAtuSituacaoAtual\"
				from planejamento.programa
				where prgano = '".$_SESSION["exercicio"]."' and orgcod = '". CODIGO_ORGAO_SISTEMA. "'";
				
			$documento = sigplan_gravarDocumento( $sql, "Programa", "RetornaPrograma" );
		}
		
		# CRIA ARQUIVO DE DADOS FÍSICOS
		if ( in_array( "fisico", $_REQUEST["documento"] ) ) {
			sigplan_prepararDadosFisicos();
/*		$sql = "select
					d.PRGAno as \"PRGAno\",
					d.PRGCod as \"PRGCod\",
					d.ACACod as \"ACACod\",
					d.SACCod as \"SACCod\",
					d.REGCod as \"REGCod\",
					d.FISQtdeCronInicial1 as \"FISQtdeCronInicial1\",
					d.FISQtdeCronInicial2 as \"FISQtdeCronInicial2\",
					d.FISQtdeCronInicial3 as \"FISQtdeCronInicial3\",
					d.FISQtdeCronInicial4 as \"FISQtdeCronInicial4\",
					d.FISQtdeCronInicial5 as \"FISQtdeCronInicial5\",
					d.FISQtdeCronInicial6 as \"FISQtdeCronInicial6\",
					d.FISQtdeCronInicial7 as \"FISQtdeCronInicial7\",
					d.FISQtdeCronInicial8 as \"FISQtdeCronInicial8\",
					d.FISQtdeCronInicial9 as \"FISQtdeCronInicial9\",
					d.FISQtdeCronInicial10 as \"FISQtdeCronInicial10\",
					d.FISQtdeCronInicial11 as \"FISQtdeCronInicial11\",
					d.FISQtdeCronInicial12 as \"FISQtdeCronInicial12\",
					d.FISQtdeCronogramado1 as \"FISQtdeCronogramado1\",
					d.FISQtdeCronogramado2 as \"FISQtdeCronogramado2\",
					d.FISQtdeCronogramado3 as \"FISQtdeCronogramado3\",
					d.FISQtdeCronogramado4 as \"FISQtdeCronogramado4\",
					d.FISQtdeCronogramado5 as \"FISQtdeCronogramado5\",
					d.FISQtdeCronogramado6 as \"FISQtdeCronogramado6\",
					d.FISQtdeCronogramado7 as \"FISQtdeCronogramado7\",
					d.FISQtdeCronogramado8 as \"FISQtdeCronogramado8\",
					d.FISQtdeCronogramado9 as \"FISQtdeCronogramado9\",
					d.FISQtdeCronogramado10 as \"FISQtdeCronogramado10\",
					d.FISQtdeCronogramado11 as \"FISQtdeCronogramado11\",
					d.FISQtdeCronogramado12 as \"FISQtdeCronogramado12\",
					d.FISQtdeRealizado1 as \"FISQtdeRealizado1\",
					d.FISQtdeRealizado2 as \"FISQtdeRealizado2\",
					d.FISQtdeRealizado3  as \"FISQtdeRealizado3\",
					d.FISQtdeRealizado4  as \"FISQtdeRealizado4\",
					d.FISQtdeRealizado5  as \"FISQtdeRealizado5\",
					d.FISQtdeRealizado6  as \"FISQtdeRealizado6\",
					d.FISQtdeRealizado7  as \"FISQtdeRealizado7\",
					d.FISQtdeRealizado8  as \"FISQtdeRealizado8\",
					d.FISQtdeRealizado9  as \"FISQtdeRealizado9\",
					d.FISQtdeRealizado10  as \"FISQtdeRealizado10\",
					d.FISQtdeRealizado11  as \"FISQtdeRealizado11\",
					d.FISQtdeRealizado12  as \"FISQtdeRealizado12\",
					d.FISQtdeprevistoano as \"FISQtdePrevistoAno\",
					d.FISQtdeCronInicialano as \"FISQtdeCronInicialAno\",
					d.FISQtdeatualano as \"FISQtdeAtualAno\",
					d.FISQtdeCronogramadoAno as \"FISQtdeCronogramadoAno\",
					d.FISQtdeRealizadoAno as \"FISQtdeRealizadoAno\",
					d.FISDscComentExecucao as \"FISDscComentExecucao\"
				from monitora.dadofisico d
				inner join monitora.acao a on
						a.acaid = d.acaid and a.acasnrap = false
				where ( a.unicod like '26%' or a.unicod like '74%' ) and d.prgano = '".$_SESSION["exercicio"]."'";
*/		$sql = "select					d.PRGAno as \"PRGAno\",					d.PRGCod as \"PRGCod\",					d.ACACod as \"ACACod\",					d.SACCod as \"SACCod\",					d.REGCod as \"REGCod\",					d.FISQtdeRealizado1 as \"FISQtdeRealizado1\",					d.FISQtdeRealizado2 as \"FISQtdeRealizado2\",					d.FISQtdeRealizado3  as \"FISQtdeRealizado3\",					d.FISQtdeRealizado4  as \"FISQtdeRealizado4\",					d.FISQtdeRealizado5  as \"FISQtdeRealizado5\",					d.FISQtdeRealizado6  as \"FISQtdeRealizado6\",					d.FISQtdeRealizado7  as \"FISQtdeRealizado7\",					d.FISQtdeRealizado8  as \"FISQtdeRealizado8\",					d.FISQtdeRealizado9  as \"FISQtdeRealizado9\",					d.FISQtdeRealizado10  as \"FISQtdeRealizado10\",					d.FISQtdeRealizado11  as \"FISQtdeRealizado11\",					d.FISQtdeRealizado12  as \"FISQtdeRealizado12\",					d.FISQtdeprevistoano as \"FISQtdePrevistoAno\",					d.FISQtdeCronInicialano as \"FISQtdeCronInicialAno\",					d.FISQtdeatualano as \"FISQtdeAtualAno\",					d.FISQtdeCronogramadoAno as \"FISQtdeCronogramadoAno\",					d.FISQtdeRealizadoAno as \"FISQtdeRealizadoAno\",					d.FISDscComentExecucao as \"FISDscComentExecucao\"				from monitora.dadofisico d				inner join monitora.acao a on						a.acaid = d.acaid and a.acasnrap = false				where ( a.unicod like '26%' or a.unicod like '74%' ) and d.prgano = '".$_SESSION["exercicio"]."'";		
//		dbg($sql,1);	
		$documento = sigplan_gravarDocumento( $sql, "DadoFisico", "RetornaDadoFisico" );
		}
		
		# CRIA ARQUIVO DE DADOS FINANCEIROS
		# TODO: o código antigo não criava arquivo com dados financeiros
		if ( in_array( "financeiro", $_REQUEST["documento"] ) ) {
//			sigplan_prepararDadosFinanceiros();
//			$sql = sprintf( "", $_SESSION["exercicio"] );
//			$documento = sigplan_gravarDocumento( $sql, "", "" );
		}
		
		# CRIA ARQUIVO DE RESTRIÇÕES DOS PROGRAMAS
		if ( in_array( "providencia_programa", $_REQUEST["documento"] ) ) {
			sigplan_prepararProvidenciaProgramas();
		$sql = "select
					p.PRGAno as \"PRGAno\",
					p.PRGCod as \"PRGCod\",
					rp.RSPNum as \"RSINum\",
					rp.TRSCod as \"TRSCod\",
					case when rp.TPICod is null then 2 end as \"TPICod\",
					case when rp.CMPCod is null then 0 end as \"CMPCod\",
					to_char(rp.RSPDataInclusao,'YYYY-MM-DDT') || to_char(rp.RSPDataInclusao,'HH24:MI:SS') as \"RSPDataInclusao\",
					rp.RSPDscRestricao as \"RSPDscRestricao\",
					rp.RSPDscProvidencia as \"RSPDscProvidencia\",
					to_char(rp.RSPDataSuperacao,'YYYY-MM-DDT') || to_char(rp.RSPDataSuperacao,'HH24:MI:SS') as \"RSPDataSuperacao\",
					rp.RSPSNRelatSituacao as \"RSPSNRelatSituacao\",
					rp.RSPSNTempoHabil as \"RSPSNTempoHabil\",
					rp.RSPObsMonitor as \"RSPObsMonitor\",
					rp.RSPObsControle as \"RSPObsControle\"
				from monitora.restricaoprograma rp
				inner join planejamento.programa p on
					p.prgid = rp.prgid and p.prgano = rp.prgano
				where rp.prgano = '".$_SESSION["exercicio"]."' and rp.rspsnliberado = true
				order by p.prgano, p.prgcod";

				$documento = sigplan_gravarDocumento( $sql, "RestricaoPrograma", "RetornaRestricaoPrograma" );
		}
		
		# CRIA ARQUIVO DE RESTRIÇÕES DAS AÇÕES
		if ( in_array( "providencia_acao", $_REQUEST["documento"] ) ) {
			sigplan_prepararProvidenciaAcoes();
		$sql = "select
					a.PRGAno as \"PRGAno\",
					a.PRGCod as \"PRGCod\",
					a.ACACod as \"ACACod\",
					a.SACCod as \"SACCod\",
					r.TRSCod as \"TRSCod\",
					r.RSANum as \"RSINum\",
					case when r.TPICod is null then 2 end as \"TPICod\",
					case when r.CMPCod is null then 0 end as \"CMPCod\",
					to_char(r.RSADataInclusao,'YYYY-MM-DDT') || to_char(r.RSADataInclusao,'HH24:MI:SS') as \"RSADataInclusao\",
					r.RSADscRestricao as \"RSADscRestricao\",
					r.RSADscProvidencia as \"RSADscProvidencia\",
					to_char(r.RSADataSuperacao,'YYYY-MM-DDT') || to_char(r.RSADataSuperacao,'HH24:MI:SS') as \"RSADataSuperacao\",
					r.RSASNRelatSituacao as \"RSASNRelatSituacao\",
					r.RSASNTempoHabil as \"RSASNTempoHabil\",
					r.RSAObsMonitor as \"RSAObsMonitor\",
					r.RSAObsControle as \"RSAObsControle\"
				from monitora.restricaoacao r
				inner join monitora.acao a on a.acaid = r.acaid and a.prgano = '".$_SESSION["exercicio"]."'
				where ( a.unicod like '26%' or a.unicod like '74%' ) and r.rsasnliberado = true
				order by a.acacod,a.unicod,a.loccod,a.saccod";
				
			$documento = sigplan_gravarDocumento( $sql, "RestricaoAcao", "RetornaRestricaoAcao" );
		}
		
		# CRIA ARQUIVO DE VALIDAÇÃO DAS AÇÕES
		# TODO: case when a.prgano <> '."''".' then '." ''". ' end as "ObservacoesGerente"
		if ( in_array( "validacao_acao", $_REQUEST["documento"] ) ) {
			sigplan_prepararValidacaoAcoes();
		$sql = "select
					a.PRGAno as \"PRGAno\",
					a.PRGCod as \"PRGCod\",
					a.ACACod as \"ACACod\",
					a.SACCod as \"SACCod\",
					substr(r.refdsc,1,1) as \"Trimestre\",
					substr(c.corsignificadopar,1,1) as \"SituacaoValidacao\",
					'' as \"ObservacoesGerente\",
					avp.avptexto as \"JustificativaDiscordanciaGerente\"
				from monitora.acao a
				inner join monitora.avaliacaoparecer avp on
					a.acaid = avp.acaid and avp.avpliberada = true and avp.tpaid = 2
				inner join public.cor c on
					c.corcod = avp.corcod
				inner join monitora.referencia r on
					r.refcod = avp.refcod
				where ( a.unicod like '26%' or a.unicod like '74%' ) and a.prgano = '".$_SESSION["exercicio"]."'
				order by a.prgano, a.prgcod, a.acacod, a.unicod, a.loccod, a.saccod";
		
			$documento = sigplan_gravarDocumento( $sql, "ValidacaoTrimestral", "RetornaValidacaoAcao" );
		}
	} catch ( Exception $erro ) {
		$db->rollback();
		?>
		<html>
			<head>
				<script type="text/javascript">
				alert( '<?= $erro->getMessage(); ?>' );
				location.href = "?modulo=<?= $_REQUEST["modulo"] ?>&acao=<?= $_REQUEST["acao"] ?>";
				</script>
			</head>
			<body/>
		</html>
		<?
		exit();
	}
	$db->commit();
	$_REQUEST["acao"] = "I";
	$db->sucesso( $_REQUEST["modulo"] );
}

?>
<style>
	label { cursor: pointer; }
</style>
<form method="POST" name="formulario">
	<input type="hidden" name="formulario" value="1"/>
	<table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
		<tr>
			<td style="padding: 10px;">
				<p style="width: 50%;">
					O envio de dados para o SIGPLAN deverá ocorrer quantas vezes o Administrador julgar conveniente.
					Ao pressionar o botão "Gerar Arquivo", o sistema irá gerar um conjunto de arquivos no formato XML e disponibilizá-los para o SIGPLAN.
				</p>
			</td>
		</tr>
		<tr>
			<td style="padding: 0 20px 20px 20px;">
				<label>
					<input id="acao" type="checkbox" name="documento[]" value="acao" onchange="selecionarOpcao( this );"/>
					Ações
				</label><br/>
				<label>
					<input id="programa"type="checkbox" name="documento[]" value="programa" onchange="selecionarOpcao( this );"/>
					Programas
				</label><br/>
				<label>
					<input id="fisico" type="checkbox" name="documento[]" value="fisico" onchange="selecionarOpcao( this );"/>
					Dados Físicos
				</label><br/>
				<!--
				<label style="color: #aaaaaa; cursor: default;">
					<input id="financeiro" type="checkbox" name="documento[]" value="financeiro" onchange="selecionarOpcao( this );" disabled="disabled"/>
					Dados Financeiros
				</label><br/>
				-->
				<label>
					<input id="providencia_programa" type="checkbox" name="documento[]" value="providencia_programa" onchange="selecionarOpcao( this );"/>
					Restrições e Providências de Programa
				</label><br/>
				<label >
					<input id="providencia_acao" type="checkbox" name="documento[]" value="providencia_acao" onchange="selecionarOpcao( this );"/>
					Restrições e Providências de Ação
				</label><br/>
				<label>
					<input id="validacao_acao" type="checkbox" name="documento[]" value="validacao_acao" onchange="selecionarOpcao( this );"/>
					Validação de Ação
				</label><br/>
				&nbsp;<br/>
				<label>
					<input id="todos" type="checkbox" name="" value="acao" onclick="selecionarTodos( this );"/>
					<b>Todos</b>
				</label>
				<p><input type="button" class="botao" value="Gerar Arquivos" onclick="enviar();"></p>
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">

	function selecionarTodos( todos ){
		document.getElementById( 'acao' ).checked = todos.checked;
		document.getElementById( 'programa' ).checked = todos.checked;
		document.getElementById( 'fisico' ).checked = todos.checked;
//		document.getElementById( 'financeiro' ).checked = todos.checked;
		document.getElementById( 'providencia_programa' ).checked = todos.checked;
		document.getElementById( 'providencia_acao' ).checked = todos.checked;
		document.getElementById( 'validacao_acao' ).checked = todos.checked;
	}

	function selecionarOpcao( opcao ){
		if ( !opcao.checked ) {
			document.getElementById( 'todos' ).checked = false;
		} else if(
			document.getElementById( 'acao' ).checked == true &&
			document.getElementById( 'programa' ).checked == true &&
			document.getElementById( 'fisico' ).checked == true &&
//			document.getElementById( 'financeiro' ).checked == true &&
			document.getElementById( 'providencia_programa' ).checked == true &&
			document.getElementById( 'providencia_acao' ).checked == true &&
			document.getElementById( 'validacao_acao' ).checked == true
		) {
			document.getElementById( 'todos' ).checked = true;
		}
	}

	function enviar(){
		document.formulario.submit();
	}

</script>