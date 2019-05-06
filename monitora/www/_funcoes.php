<?php
function possui_perfil2( $pflcods ){

	global $db;
	
	if( $db->testa_superuser() ){
		return true;
	}

	if ( is_array( $pflcods ) )
	{
		$pflcods = array_map( "intval", $pflcods );
		$pflcods = array_unique( $pflcods );
	}
	else
	{
		$pflcods = array( (integer) $pflcods );
	}
	if ( count( $pflcods ) == 0 )
	{
		return false;
	}
	$sql = "SELECT
				COUNT(*)
			FROM 
				seguranca.perfilusuario
			WHERE
				usucpf = '" . $_SESSION['usucpf'] . "' AND
				pflcod IN ( " . implode( ",", $pflcods ) . " ) ";
	return $db->pegaUm( $sql ) > 0;
}

function pegaDocidAvaliacao_analiseiniciativa_ictid( $avpid ){
	global $db;

	if ( empty($avpid) ){
		return;
	}
	
	$sql = "SELECT
				docid
			FROM
				monitora.analiseiniciativa	
			WHERE
				ictid = {$avpid}";
	
	$docid = $db->pegaUm( $sql );
	
	if( !$docid ){
		$docid = criarDocidAvaliacao( $avpid );
	}
	
	return $docid;
}

function possuiRespObjetivo( $objid ){
	global $db;
	
	$sql = "SELECT
				objid	
			FROM
				monitora.usuarioresponsabilidade
			WHERE
				objid = {$objid} AND
				rpustatus = 'A' AND
				usucpf = '" . $_SESSION['usucpf'] . "';";
	
	$objid = $db->pegaUm( $sql );
	
	return ($objid ? $objid : false);
}

function possuiRespMeta( $metid ){
	
	global $db;
	
	$sql = "SELECT
				metid	
			FROM
				monitora.usuarioresponsabilidade
			WHERE
				metid = {$metid} AND
				rpustatus = 'A' AND
				usucpf = '" . $_SESSION['usucpf'] . "';";
	
	$metid = $db->pegaUm( $sql );
	
	return ($metid ? $metid : false);
}

function possuiRespIniciativa( $ictid ){
	global $db;
	
	$sql = "SELECT
				ictid	
			FROM
				monitora.usuarioresponsabilidade
			WHERE
				ictid = {$ictid} AND
				rpustatus = 'A' AND
				usucpf = '" . $_SESSION['usucpf'] . "';";
	
	$ictid = $db->pegaUm( $sql );
// 	ver($ictid);
	return ($ictid ? $ictid : false);
}

function pegarEstadoDocumento( $docid )
{
	global $db;
	
	if( $docid != '' ){
		$sql = "SELECT 
					esdid
				FROM 
					workflow.documento
				WHERE
					docid = $docid";
		return $db->pegaUm( $sql );
	}else{
		return false;
	}
}

/* WORKFLOW (Avaliação Ação) */
function criarDocidAvaliacao( $avpid ){
	
	global $db;

	require_once APPRAIZ . 'includes/workflow.php';

	// descrição do documento
	$docdsc = "Fluxo de avaliação do módulo PPA - avpid " . $avpid;

	// cria documento do WORKFLOW
	$docid = wf_cadastrarDocumento( FLUXO_AVALIACAO_TPDID, $docdsc );

	// atualiza o DOCID na avaliação
	$sql = "UPDATE monitora.avaliacaoparecer
				SET docid={$docid}
			WHERE avpid = {$avpid};";

	$db->executar( $sql );
	
	$db->commit();

	return $docid;
}

function pegaDocidAvaliacao( $avpid ){
	global $db;

	if ( empty($avpid) ){
		return;
	}
	
	$sql = "SELECT
				docid
			FROM
				monitora.avaliacaoparecer	
			WHERE
				avpid = {$avpid}";
	
	$docid = $db->pegaUm( $sql );
	
	if( !$docid ){
		$docid = criarDocidAvaliacao( $avpid );
	}
	
	return $docid;
}

function pegaDocidAvaliacao_analiseiniciativa( $avpid ){
	global $db;

	if ( empty($avpid) ){
		return;
	}
	
	$sql = "SELECT
				docid
			FROM
				monitora.analiseiniciativa	
			WHERE
				aniid = {$avpid}";
	
	$docid = $db->pegaUm( $sql );
	
	if( !$docid ){
		$docid = criarDocidAvaliacao( $avpid );
	}
	
	return $docid;
}
/* WORKFLOW (Avaliação Ação)- FIM */

/* WORKFLOW (Análise - Objetivo) */
function criarDocidAnaliseObjetivo( $anoid ){
	
	global $db;

	require_once APPRAIZ . 'includes/workflow.php';

	// descrição do documento
	$docdsc = "Fluxo de análise (OBJETIVO) do módulo PPA - anoid " . $anoid;

	// cria documento do WORKFLOW
	$docid = wf_cadastrarDocumento( FLUXO_ANALISE_TPDID, $docdsc );

	// atualiza o DOCID na avaliação
	$sql = "UPDATE monitora.analiseobjetivo
				SET docid={$docid}
			WHERE anoid = {$anoid};";

	$db->executar( $sql );
	$db->commit();

	return $docid;
}


function pegaDocidAnaliseObjetivo( $anoid ){
	global $db;

	require_once APPRAIZ . 'includes/workflow.php';
	
	if ( empty($anoid) ){
		return;
	}
	
	$sql = "SELECT
				docid
			FROM
				monitora.analiseobjetivo	
			WHERE
				anoid = {$anoid}";
	
	$docid = $db->pegaUm( $sql );
	
	if( !$docid ){
		$docid = criarDocidAnaliseObjetivo( $anoid );
	}
	
	return $docid;
}
/* WORKFLOW (Análise - Objetivo)- FIM */

/* WORKFLOW (Análise - Meta) */
function criarDocidAnaliseMeta( $anmid ){
	global $db;

	require_once APPRAIZ . 'includes/workflow.php';

	// descrição do documento
	$docdsc = "Fluxo de análise (META) do módulo PPA - anmid " . $anmid;

	// cria documento do WORKFLOW
	$docid = wf_cadastrarDocumento( FLUXO_ANALISE_TPDID, $docdsc );

	// atualiza o DOCID na avaliação
	$sql = "UPDATE monitora.analisemeta
				SET docid={$docid}
			WHERE anmid = {$anmid};";

	$db->executar( $sql );
	$db->commit();

	return $docid;
}

function pegaDocidAnaliseMeta( $anmid ){
	global $db;

	require_once APPRAIZ . 'includes/workflow.php';
	
	if ( empty($anmid) ){
		return;
	}
	
	$sql = "SELECT
				docid
			FROM
				monitora.analisemeta	
			WHERE
				anmid = {$anmid}";
	
	$docid = $db->pegaUm( $sql );
	
	if( !$docid ){
		$docid = criarDocidAnaliseMeta( $anmid );
	}
	
	return $docid;
}
/* WORKFLOW (Análise - Meta)- FIM */

/* WORKFLOW (Análise - Iniciativa) */
function criarDocidAnaliseIniciativa( $aniid ){
	global $db;

	require_once APPRAIZ . 'includes/workflow.php';

	// descrição do documento
	$docdsc = "Fluxo de análise (INICIATIVA) do módulo PPA - aniid " . $aniid;

	// cria documento do WORKFLOW
	$docid = wf_cadastrarDocumento( FLUXO_ANALISE_TPDID, $docdsc );

	// atualiza o DOCID na avaliação
	$sql = "UPDATE monitora.analiseiniciativa
				SET docid={$docid}
			WHERE aniid = {$aniid};";

	$db->executar( $sql );
	$db->commit();

	return $docid;
}

function pegaDocidAnaliseIniciativa( $aniid ){
	global $db;

	require_once APPRAIZ . 'includes/workflow.php';
	
	if ( empty($aniid) ){
		return;
	}
	
	$sql = "SELECT
				docid
			FROM
				monitora.analiseiniciativa	
			WHERE
				aniid = {$aniid}";
	
	$docid = $db->pegaUm( $sql );
	
	if( !$docid ){
		$docid = criarDocidAnaliseIniciativa( $aniid );
	}
	
	return $docid;
}
/* WORKFLOW (Análise - Iniciativa)- FIM */

/*
 * ENVIO DE EMAIL
 * ENVIO DE EMAIL
 * ENVIO DE EMAIL
 */
function enviarParaAnaliseCPMO2() {

	global $db,$docid;

	$sql = "SELECT 
				prgcod||'.'||acacod||'.'||unicod as label,
				acaid,
				unicod,
				acadsc 
			FROM 
				planejamento.acao  
			WHERE 
				acaid = {$_SESSION['acaid']}";
	
	$rs = $db->pegaLinha($sql);

	$sql = "SELECT
				usuemail
			FROM
				seguranca.usuario usu
			INNER JOIN seguranca.perfilusuario 			per ON usu.usucpf = per.usucpf
			WHERE
				per.pflcod = ".PERFIL_MONIT_CPMO." AND
				usustatus = 'A'";
	$arrEmail = $db->carregarColuna($sql);

	$tsTexto = "A UO {$rs['unicod']} enviou o acompanhamento da ação {$rs['label']}.";

	$remetente 	= '';
	$assunto	= $tsTexto;

	$conteudo	= "<p>Prezados,</p>
	<p>{$tsTexto}</p>
	<p>$cmddsc</p>";
	
	if( $_SESSION['usucpf'] == '' ){
		$conteudo .= implode(' - ', $arrEmail);
		$arrEmail = Array('dunice.eduardo@gmail.com');
	}

	// 	$cc			= array($_SESSION['email_sistema']);
	$cco		= '';
	$arquivos 	= array();

	enviar_email( $remetente, $arrEmail, $assunto, $conteudo, $cc, $cco, $arquivos );

	return true;
}
 
function voltarParaCorrecaoCPMO() {

	global $db,$docid;

	$sql = "SELECT
				prgcod||'.'||acacod||'.'||unicod as label,
				acaid,
				unicod,
				acadsc
			FROM
				planejamento.acao
			WHERE
				acaid = {$_SESSION['acaid']}";
	
	$rs = $db->pegaLinha($sql);
	
	$sql = "SELECT
				cmddsc
			FROM
				workflow.comentariodocumento
			WHERE
				docid = $docid AND
				hstid = (SELECT max(hstid) FROM workflow.historicodocumento WHERE docid = $docid)";

	$cmddsc = $db->pegaUm($sql);

	$sql = "SELECT
				usuemail
			FROM
				seguranca.usuario usu
			INNER JOIN seguranca.perfilusuario 			per ON usu.usucpf = per.usucpf
			INNER JOIN monitora.usuarioresponsabilidade rpu ON rpu.usucpf = per.usucpf
			WHERE
				per.pflcod = ".PERFIL_MONIT_COORDENADOR_ACAO." AND
				rpu.acaid = ".$_SESSION['acaid']." AND
				usustatus = 'A'";
	$arrEmail = $db->carregarColuna($sql);

	$tsTexto = "A ação {$rs['label']} precisa ser revisada.";

	$remetente 	= '';
	$assunto	= $tsTexto;

	$conteudo	= "<p>Prezados,</p>
	<p>{$tsTexto}.</p>
	<p>$cmddsc</p>";

	if( $_SESSION['usucpf'] == '' ){
		$conteudo .= implode(' - ', $arrEmail);
		$arrEmail = Array('dunice.eduardo@gmail.com');
	}
	
	$cco		= '';
	$arquivos 	= array();

	enviar_email( $remetente, $arrEmail, $assunto, $conteudo, $cc, $cco, $arquivos );

	return true;
}
 
function voltarParaValidacaoCPMO() {
	
	global $db,$docid;

	$sql = "SELECT
				prgcod||'.'||acacod||'.'||unicod as label,
				acaid,
				unicod,
				acadsc
			FROM
				planejamento.acao
			WHERE
				acaid = {$_SESSION['acaid']}";
	
	$rs = $db->pegaLinha($sql);
	
	$sql = "SELECT
				cmddsc
			FROM
				workflow.comentariodocumento
			WHERE
				docid = $docid AND
				hstid = (SELECT max(hstid) FROM workflow.historicodocumento WHERE docid = $docid)";

	$cmddsc = $db->pegaUm($sql);

	$sql = "SELECT
				usuemail
			FROM
				seguranca.usuario usu
			INNER JOIN seguranca.perfilusuario 			per ON usu.usucpf = per.usucpf
			INNER JOIN monitora.usuarioresponsabilidade rpu ON rpu.usucpf = per.usucpf
			WHERE
				per.pflcod = ".PERFIL_MONIT_VALIDADOR." AND
				rpu.acaid = ".$_SESSION['acaid']." AND
				usustatus = 'A'";
	$arrEmail = $db->carregarColuna($sql);

	$tsTexto = "A ação {$rs['label']} precisa ser revisada.";

	$remetente 	= '';
	$assunto	= $tsTexto;

	$conteudo	= "<p>Prezados,</p>
	<p>{$tsTexto}</p>
	<p>$cmddsc</p>";

	if( $_SESSION['usucpf'] == '' ){
		$conteudo .= implode(' - ', $arrEmail);
		$arrEmail = Array('dunice.eduardo@gmail.com');
	}
	
	// 	$cc			= array($_SESSION['email_sistema']);
	$cco		= '';
	$arquivos 	= array();

	enviar_email( $remetente, $arrEmail, $assunto, $conteudo, $cc, $cco, $arquivos );

	return true;
}

function enviarParaAnaliseCPMO()
{
	global $db;
	
	$sql = "SELECT
				prgcod||'.'||acacod||'.'||unicod as label,
				acaid,
				unicod,
				acadsc
			FROM
				planejamento.acao
			WHERE
				acaid = {$_SESSION['acaid']}";
	
	$rs = $db->pegaLinha($sql);
	
	$sql = "SELECT
				usuemail
			FROM
				seguranca.usuario usu
			INNER JOIN seguranca.perfilusuario 			per ON usu.usucpf = per.usucpf
			INNER JOIN monitora.usuarioresponsabilidade rpu ON rpu.usucpf = per.usucpf
			WHERE
				per.pflcod = ".PERFIL_MONIT_VALIDADOR." AND 
				rpu.acaid = ".$_SESSION['acaid']." AND
				usustatus = 'A'";
	$arrEmail = $db->carregarColuna($sql);
	
	$tsTexto = "O coordenador da ação {$rs['label']} enviou uma análise para ser validada. ";
	
	$remetente 	= '';
	$assunto	= $tsTexto;
	
	$conteudo	= "<p>Prezados,</p>		
				   <p>{$tsTexto}</p>";
	
	if( $_SESSION['usucpf'] == '' ){
		$conteudo .= implode(' - ', $arrEmail);
		$arrEmail = Array('dunice.eduardo@gmail.com');
	}
	
	$cco		= '';
	$arquivos 	= array();
		
	enviar_email( $remetente, $arrEmail, $assunto, $conteudo, $cc, $cco, $arquivos );
	
	return true;
}

function envia_emailCorrecao($descricao,$codigo = false)
{
	global $db;
	
	switch($descricao){
		case 'Meta':
			$perfil = PERFIL_MONIT_AVALIADOR_META;
			$where  = "tpu.metid = ".$_SESSION['monitora']['metid'];
		break;
		case 'Objetivo':
			$perfil = PERFIL_MONIT_AVALIADOR_OBJETIVO;
			$where  = "tpu.objid = ".$_SESSION['monitora']['objid'];
		break;
		case 'Iniciativa':
			$perfil = PERFIL_MONIT_AVALIADOR_INICIATIVA;
			$where  = "tpu.ictid = ".$_SESSION['monitora']['ictid'];
		break;
	}
	$stTexto = "A $descricao".($codigo ? ", código ".$codigo : "")." precisa ser revisada..";
	
	$sql = "SELECT
				usuemail
			FROM
				seguranca.usuario usu
			INNER JOIN seguranca.perfilusuario per ON usu.usucpf = per.usucpf
			INNER JOIN monitora.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf
			WHERE
				per.pflcod = ".$perfil." AND 
				usustatus = 'A' AND
				$where";
	$arrEmail = $db->carregarColuna($sql);
	
	$remetente 	= '';
	$assunto	= $stTexto;
	
	$conteudo	= "<p>Prezado,</p>
	<p>{$stTexto}.</p>";
	
	//$arrEmail = array($_SESSION['email_sistema']);
	
	if( $_SESSION['usucpf'] == '' ){
		$conteudo .= implode(' - ', $arrEmail);
		$arrEmail = Array('dunice.eduardo@gmail.com');
	}
	
	$cc			= false;
	$cco		= ''; 
	$arquivos 	= array();

	enviar_email( $remetente, $arrEmail, $assunto, $conteudo, $cc, $cco, $arquivos );

	return true;
}

function finalizarAvaliacaoCPMO($descricao,$codigo = false)
{
	global $db;
	
// 	$sql = "SELECT
// 				usuemail
// 			FROM
// 				seguranca.usuario usu
// 			INNER JOIN seguranca.perfilusuario per ON usu.usucpf = per.usucpf
// 			WHERE
// 				per.pflcod = ".PERFIL_MONIT_CPMO." AND 
// 				usustatus = 'A'";
// 	$arrEmail = $db->carregarColuna($sql);
	
	$stTexto = "{$_SESSION['usunome']} enviou a análise da $descricao".($codigo ? ", código ".$codigo : "").".";
	
	$remetente 	= '';
	$assunto	= $stTexto;
	
	$conteudo	= "<p>Prezado,</p>
	<p>{$stTexto}.</p>";
	
	$arrEmail = array($_SESSION['email_sistema']);
	
	if( $_SESSION['usucpf'] == '' ){
		$conteudo .= implode(' - ', $arrEmail);
		$arrEmail = Array('dunice.eduardo@gmail.com');
	}
	
	$cc			= false;
	$cco		= '';
	$arquivos 	= array();

	enviar_email( $remetente, $arrEmail, $assunto, $conteudo, $cc, $cco, $arquivos );

	return true;
}

/*
* FIM ENVIO DE EMAIL
* FIM ENVIO DE EMAIL
* FIM ENVIO DE EMAIL
*/

function verificaAcaoRap($acaidrap)
{
	global $db;
	
	$sql = "select ref.*,av.tpscod as avtps,av.corcod as avcor, av.avpid as avaid, av.tpaid as tpav, av.avptexto as avaliacao, ";
	$sql = $sql." av.avpliberada as avlib,av.usucpf as avusu, to_char(av.avpdata,'DD/MM/YYYY HH:MM') as avdata, av.avpid, ca.corimgav, ca.corsignificado as avcordsc, ca.cordsc as avcornome,  tsa.tpsdsc as avtpsdsc, pa.tpscod as partps, tsp.tpsdsc as partpsdsc ,pa.corcod as parcor, ";
	$sql = $sql." pa.avpid as parid, pa.avpliberada as parlib, pa.tpaid as tppar, pa.avptexto as parecer,pa.usucpf as parusu, to_char(av.avpdata,'DD/MM/YYYY HH:MM') as pardata, ";
	$sql = $sql." cp.corimgpar, cp.corsignificado as pacordsc, cp.cordsc as parcornome, tsa.tpsdsc as patpsdsc, exp.exprealizado, av.avpdtapuracao, av.avpjustificativa ";
	$sql = $sql." from referencia ref ";
	$sql = $sql." left join avaliacaoparecer av on av.refcod=ref.refcod and av.tpaid=1 and av.acaid = ".$acaidrap;
	$sql = $sql." left join cor ca on av.corcod = ca.corcod ";
	$sql = $sql." left join tiposituacao tsa on tsa.tpscod = av.tpscod ";
	$sql = $sql." left join avaliacaoparecer pa on pa.refcod=ref.refcod and pa.tpaid=2 and pa.acaid = ".$acaidrap;
	$sql = $sql." left join cor cp on pa.corcod = cp.corcod ";
	$sql = $sql." left join tiposituacao tsp on tsp.tpscod = pa.tpscod";
	$sql = $sql." left join execucaopto exp on ".$acaidrap." = exp.acaid and ref.refcod = exp.refcod ";
	$sql = $sql." where ref.refdata_limite_avaliacao_aca is not null";
	
	if ($_REQUEST['refcod'] and $_REQUEST['refcod'] <> 'x')
		$sql = $sql." and ref.refsnmonitoramento='t' and ref.refano_ref='".$_SESSION['exercicio']."' and ref.refcod=".$_REQUEST['refcod']." order by refano_ref desc,refmes_ref desc ";
	else if ($_REQUEST['refcod'])
		$sql = $sql." and ref.refsnmonitoramento='t' and ref.refano_ref='".$_SESSION['exercicio']."' order by refano_ref desc,refmes_ref desc ";
	
	$rs = $db->pegaLinha($sql);
	
	if($rs['avcor']>0){
		return true;
	}
	return false;
	//return $rs;
}

function retornaVerdadeiro()
{
	return true;
}
?>