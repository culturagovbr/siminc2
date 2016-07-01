<?php
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

echo carregaAbasProInfancia("par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=questionario&preid=".$_REQUEST['preid'], $_REQUEST['preid'], $descricaoItem);

monta_titulo('QUESTION�RIO', 'Preencha o question�rio');

$preid = $_REQUEST['preid'];
$qrpid = pegaQrpidPAC( $preid, 43 );

$docid = prePegarDocid($preid);
$esdid = prePegarEstadoAtual($docid);

$perfil = pegaPerfilGeral();

$habilitado = 'N';
$travaCorrecao = true;

$obSubacaoControle = new SubacaoControle();
$obPreObra = new PreObra();

if($preid){
	$arDados = $obSubacaoControle->recuperarPreObra($preid);
}

//ver($esdid, is_array($respSim), d);
if($esdid) {
	$arrLivres = Array(
					WF_TIPO_EM_CADASTRAMENTO, 
					WF_TIPO_EM_REFORMULACAO, 
					WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL, 
					WF_TIPO_EM_DILIGENCIA_REFORMULACAO_MI_PARA_CONVENCIONAL
				);
	if( is_array($respSim) && !in_array($esdid, $arrLivres) ){
		$travaCorrecao = (in_array(QUESTAO_DOCUMENTO1, $respSim) && in_array(QUESTAO_DOCUMENTO2, $respSim));
	}elseif( in_array($esdid, $arrLivres) ){
		$travaCorrecao = false;
	}
	
	#Regras de acesso: Passada por Thiago em 24/05/2012 - PERFIL CONSULTA, APENAS VISUALIZA��O.
	#Regras de acesso: Modifica��o na estrutura do c�gido para melhoria e adequa��o as regras estabelecidas.
	if(in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) && $esdid == WF_TIPO_OBRA_APROVADA && $arDados['ptoprojetofnde'] == 'f') {
		$habilitado = 'S';
	}else{
		if(
				(
						$esdid == WF_TIPO_EM_CORRECAO ||
						$esdid == WF_TIPO_EM_CADASTRAMENTO || 
						$esdid == WF_TIPO_EM_REFORMULACAO ||
						$esdid == WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL ||
						$esdid == WF_TIPO_EM_DILIGENCIA_REFORMULACAO_MI_PARA_CONVENCIONAL
				) &&
				(
						in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ||
						in_array(PAR_PERFIL_PREFEITO, $perfil) ||
						in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) ||
						in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) ||
						in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) ||
						in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil)
				)
		){
			if($travaCorrecao){
				$habilitado = 'N';
			}else{
				$habilitado = 'S';
			}
		}
	}
}

// nova situa��o, se o preobra for uma reformula��o ou se tiver mais de 0% de execu��o da obra... desabilitar
if($arDados['preidpai'] || (float)$arDados['percexec'] > 0) {
	$habilitado = 'N';
	$travaCorrecao = true;
}

	# C�digo refeito em 22/10/2012. Regra para libera��o da tela para (cadastramento e atera��o) dos perfil abaixos listados nas seguintes situa��es tamb�m listadas abaixo. 
	# Foi inserido os perfis Estaduais e a situa��o em Dilig�ncia.
	$perfil = pegaArrayPerfil($_SESSION['usucpf']);	
	$arrEsdid = Array(
					WF_TIPO_EM_CORRECAO,
					WF_TIPO_EM_CADASTRAMENTO,
					WF_TIPO_EM_REFORMULACAO,
					WF_TIPO_EM_ANALISE_DILIGENCIA,
					WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL,
					WF_TIPO_EM_DILIGENCIA_REFORMULACAO_MI_PARA_CONVENCIONAL
				);
	if(	in_array($esdid, $arrEsdid) &&
		(
			in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) ||
			in_array(PAR_PERFIL_PREFEITO, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $perfil) ||
			in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
			in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil)
		)
	){
		$habilitado = 'S';
		$travaCorrecao = false;
	}
/*
 * REGRA TEMPORARIA 02/05/2012
 * SOLICITADO PELO DANIEL AREAS
 * LIBERA EDI��O PARA
 * MUNICIPIO COM OBRA
 * EM REFORMULA��O
 * 
 * MUNICIPIOS: SORRISO/MT
 */
//if(in_array($_SESSION['par']['muncod'], array(5107925)) && in_array($esdid, array(WF_TIPO_EM_REFORMULACAO)) ){
//	$habilitado = 'S';
//	$travaCorrecao = false;
//}
?>
<script language="JavaScript">

</script>
<?php echo cabecalho(); ?>
<?php if($habilitado == 'S' && count($respSim)): ?>
	<?php
	$txtAjuda = "� necess�rio o preenchimento completo e apresenta��o das informa��es complementares solicitadas no Relat�rio de Vistoria do Terreno disponibilizado no sistema.";
	$imgAjuda = "<img alt=\"{$txtAjuda}\" title=\"{$txtAjuda}\" src=\"/imagens/ajuda.gif\">"; 
	?>
	<table align="center" class="Tabela" cellpadding="2" cellspacing="1">
		<tr>
			<td width="100" style="text-align: right;" class="SubTituloDireita">Ajuda:</td>
			<td width="90%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
				<?php echo $imgAjuda ?>
			</td>
		</tr>
	</table>
<?php endif; ?>
<table bgcolor="#f5f5f5" align="center" class="tabela" >
	<tr>
		<td>
		<fieldset style="width: 94%; background: #fff;"  >
			<legend>Question�rio</legend>
			<?php
				$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'tamDivPx' => 250, 'habilitado' => $habilitado ) );
			?>
		</fieldset>
	</tr>
</table>