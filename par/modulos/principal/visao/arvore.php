
<?php
/*ini_set("memory_limit","4000M");
set_time_limit(0);*/
?>

<link rel="stylesheet" href="../includes/jquery-treeview/jquery.treeview.css" />
<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
<script type="text/javascript" src="../includes/jquery-treeview/lib/jquery.cookie.js"></script>
<script type="text/javascript" src="../includes/jquery-treeview/jquery.treeview.js"></script>
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
<script type="text/javascript">

<!--
jQuery(document).ready(function(){
	jQuery("#tree").treeview({
		collapsed: true,
		animated: "medium",
		control:"#sidetreecontrol",
		persist: "cookie"

	});

});

var u='/par/par.php?modulo=principal/planoTrabalho&acao=A&titleFor=';

function listarSubacao(sbaid)
{
	var local = "par.php?modulo=principal/subacao&acao=A&sbaid=" + sbaid;
	janela(local,800,600,"Suba��o");
}

function incluirSubacao(aciid, indicador){
	url = 'par.php?modulo=principal/incluirSubacao&acao=A&aciid=' + aciid + '&indid=' + indicador;
	window.open(url, 'popupIncluirSubacao', "height=600,width=800,scrollbars=yes,top=50,left=200" );
}

function incluirSubacaoMun(aciid, indicador){
	url = 'par.php?modulo=principal/incluirSubacao&acao=A&mun=1&aciid=' + aciid + '&indid=' + indicador;
	window.open(url, 'popupIncluirSubacao', "height=600,width=800,scrollbars=yes,top=50,left=200" );
}

function excluirSubacao(sbaid, pontuacao, aciid){
	var qtd;
	if( pontuacao == 3 || pontuacao == 4 ){
		jQuery.ajax({
				type: "POST",
				url: window.location,
				asynchronous: false,
				data: "regraExclusao=1&aciid="+aciid,
				success: function(qtd){
					if( Number(qtd) == Number(1) ){
						if( confirm("A a��o tamb�m ser� excluida.\nTem certeza que deseja continuar?") ){
							jQuery.ajax({
									type: "POST",
									url: window.location,
									data: "excluiSubacaoArvore=1&sbaid="+sbaid,
									success: function(resposta){
										alert(resposta);
									 	window.location.reload();
									 	return false;
								   }
							 });
						}
					} else if( qtd > 1 ) {
						if( confirm("Tem certeza que deseja excluir essa suba��o?") ){
							jQuery.ajax({
									type: "POST",
									url: window.location,
									asynchronous: false,
									data: "excluiSubacaoArvore=1&sbaid="+sbaid,
									success: function(msg){
										alert(msg);
									 	window.location.reload();
								   }
							 });
						 }
					}
				 	//window.location.reload();
			   }
		 });
	} else {
		if( confirm("Tem certeza que deseja excluir essa suba��o?") ){
			jQuery.ajax({
					type: "POST",
					url: window.location,
					asynchronous: false,
					data: "excluiSubacaoArvore=1&sbaid="+sbaid,
					success: function(msg){
						alert(msg);
					 	window.location.reload();
				   }
			 });
		 }
	 }
}

function incluirAcao(ptoid){
	url = 'par.php?modulo=principal/incluirAcao&acao=A&ptoid=' + ptoid;
	window.open(url, 'popupIncluirAcao', "height=600,width=800,scrollbars=yes,top=50,left=200" );
}

function excluirAcao(aciid){
	if( confirm("Tem certeza que deseja excluir essa a��o?") ){
		jQuery.ajax({
			type: "POST",
			url: window.location,
			asynchronous: false,
			data: "excluiAcaoArvore=1&aciid="+aciid,
			success: function(msg){
				alert(msg);
			 	window.location.reload();
		   }
		});
	 }
}

function pesquisaSubacao(){
	var strdimensaopar = jQuery('[name="strdimensaopar"]').val();
	var areapar = jQuery('[name="areapar"]').val();
	var indicadorpar = jQuery('[name="indicadorpar"]').val();
	var acaopar = jQuery('[name="acaopar"]').val();
	
	var url = '';
	if( strdimensaopar != '' ) url = url + '&strdimensaopar='+strdimensaopar;
	if( areapar != '' ) url = url + '&areapar='+areapar;
	if( indicadorpar != '' ) url = url + '&indicadorpar='+indicadorpar;
	if( acaopar != '' ) url = url + '&acaopar='+acaopar;
	
	window.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore'+url; 
}

//-->
</script>
<?php
// ver($_SESSION['par']['inuid']);
if(!$_SESSION['par']['inuid']) {
	
	die("<script>
			alert('Problemas com vari�veis. Clique a navega��o novamente.');
			window.location='par.php?modulo=inicio&acao=C';
		 </script>");

}

include_once APPRAIZ . "includes/workflow.php";

$boVerArvore = true;

$itrid = $_SESSION['par']['itrid'];
$inuid = $_SESSION['par']['inuid'];

if( $itrid == 1 ){ //estadual
	$qrpid = pegaQrpid( $inuid, QUESTOES_PONTUAIS_QUEID, $_SESSION['par']['estuf'] );
} else { //municipal
	$qrpid = pegaQrpid( $inuid, QUESTOES_PONTUAIS_QUEID, $_SESSION['par']['estuf'], $_SESSION['par']['muncod'] );
}

if( !possuiPerfil(array(PAR_PERFIL_SUPER_USUARIO, 
					    PAR_PERFIL_ANALISE_PI,
					    PAR_PERFIL_ADMINISTRADOR,
					    PAR_PERFIL_CONSULTA,
					    PAR_PERFIL_CONSULTA_ESTADUAL,
                        PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL,
					    PAR_PERFIL_CONSULTA_MUNICIPAL,
                        PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL
					   ) 
				  )){
	$oDadosUnidade = new DadosUnidade();
	//$oDadosUnidade->verificaPreenchimentoAbas($_SESSION['par']['inuid'], $_SESSION['par']['muncod'], $_SESSION['par']['estuf'], $_SESSION['par']['itrid']);
	unset($oDadosUnidade);

	if(!verificaQuestionarioQuestoes($qrpid)){
		$boVerArvore = false;
		echo "<script>
				alert('Favor preencher todo o question�rio de quest�es pontuais!');
				window.location='par.php?modulo=principal/questoesPontuais&acao=A'
	  		</script>";
		exit;
	}
					
}

$oPontuacao = new Pontuacao();
$arrPerfil = pegaPerfilGeral();
$arrPerfil = is_array($arrPerfil) ? $arrPerfil : Array();
$docid = parPegarDocidParaEntidade($_SESSION['par']['inuid']);
$estadoAtual = wf_pegarEstadoAtual( $docid );
$grandesCapitais = recuperaGrandesMunicipios($_SESSION['par']['inuid']);

// MONTA OS ARRAYS PARA CRIAR A �RVORE.
if($boVerArvore){
	$arDimensao 	= array();
	$arArea 		= array();
	$arIndicador 	= array();
	$arAcao 		= array();
	$indid 			= array();
	$ptoid 			= NULL;
	$aciid 			= NULL;
	$arDados 		= ($arDados) ? $arDados : array();

	foreach( $arDados as $resultado ){
		$sbaid 		= NULL;	
		$subDesc 	= NULL;	
		$subOrdem 	= NULL;	
		if(!in_array($resultado["indid"],$indid) ){ // SE MUDAR O INDICADOR LIMPA PONTUA��O A��O E SUBA��O
			$ptoid 		= NULL;	
			$aciid 		= NULL;
			$acidesc 	= NULL;
		}
		if($resultado["ptoid"] !== NULL){ // SE PONTUA��O EXISTE CARREGA
			$ptoid = $resultado["ptoid"];
		}
		if( !is_null($resultado["aciid"]) ){ // SE A��O EXISTE CARREGA
			$aciid = $resultado["aciid"];
			$acidesc =  $resultado["acidsc"];
		}
		if( !is_null($resultado["sbaid"]) ){ // SE A��O EXISTE CARREGA
			$sbaid = $resultado["sbaid"];
			$sbdid = $resultado["sbdid"];
			$subDesc =  $resultado["sbadsc"];
			$subOrdem = $resultado["sbaordem"];
		}
		// ARRAY DIMENS�O
		$arDimensao[$resultado["dimid"]] 						= $resultado["dimcod"].". ".$resultado["dimdsc"];
		// ARRAY AREA
		$arArea[$resultado["dimid"]][$resultado["areid"]] 		= $resultado["arecod"].". ".$resultado["aredsc"];
		// ARRAY INDICADOR
		$arIndicador[$resultado["areid"]][$resultado["indid"]] 	= array( 'descricao'=> $resultado["indcod"].". ".$resultado["inddsc"], 'ptoid'=> $ptoid);
		// ARRAY A��O

		if($aciid){ 
			if( in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) 
				|| in_array(PAR_PERFIL_ADM_OBRAS,$arrPerfil) 
				|| in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil) 
				|| in_array(PAR_PERFIL_EQUIPE_TECNICA,$arrPerfil) 
				|| in_array(PAR_PERFIL_ANALISTA_MERITOS,$arrPerfil) 
				|| in_array(PAR_PERFIL_CONSULTA,$arrPerfil) 
				|| in_array(PAR_PERFIL_CONSULTA_MUNICIPAL,$arrPerfil)
                || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL,$arrPerfil)
				|| ((in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrPerfil) && $estadoAtual['esdid'] != WF_DIAGNOSTICO)) 
				|| ((in_array(PAR_PERFIL_PREFEITO, $arrPerfil) && $estadoAtual['esdid'] != WF_DIAGNOSTICO)) 
				|| ((in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrPerfil) && $estadoAtual['esdid'] != WF_DIAGNOSTICO))
				|| ( in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$arrPerfil) && $estadoAtual['esdid'] != WF_DIAGNOSTICO)
				|| ( in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$arrPerfil) && $estadoAtual['esdid'] != WF_DIAGNOSTICO)
				|| in_array(PAR_PERFIL_ORGAO_DE_CONTROLE,$arrPerfil)
			)
			{ 
				$arAcao[$resultado["indid"]][$aciid] =  $acidesc;

				if( !( in_array(PAR_PERFIL_CONSULTA,$arrPerfil) || in_array(PAR_PERFIL_CONSULTA_MUNICIPAL,$arrPerfil) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL,$arrPerfil) )
					|| ( 
						in_array(PAR_PERFIL_EQUIPE_ESTADUAL,$arrPerfil) 
						|| in_array(PAR_PERFIL_EQUIPE_MUNICIPAL,$arrPerfil) 
						|| in_array(PAR_PERFIL_PREFEITO,$arrPerfil) 
						|| in_array(PAR_PERFIL_EQUIPE_TECNICA,$arrPerfil) 
						|| in_array(PAR_PERFIL_EQUIPE_FINANCEIRA,$arrPerfil)
						|| in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$arrPerfil)
						|| in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$arrPerfil)
					) 
				){
					$boEmpenhoAcao = $db->pegaUm("select count(es.eobid) from par.empenhosubacao es
													inner join par.subacao s on s.sbaid = es.sbaid and eobstatus = 'A'
													inner join par.acao a on a.aciid = s.aciid
												where a.aciid = {$aciid}");
					$boEmpenhoAcao = ((int)$boEmpenhoAcao > 0 ? true : false);
					//exclui a��o
					if( !in_array(PAR_PERFIL_ORGAO_DE_CONTROLE,$arrPerfil) && !$boEmpenhoAcao )
						$btnAcoes[$resultado["indid"]][$aciid] = '<img border="0" onclick="excluirAcao('.$aciid.');" src="../imagens/excluir.gif" align="absmiddle" style="cursor:pointer;"  title="Excluir A��o" />';
				}
			}
		}
		// ARRAY SUBA��O
		if($sbaid){
			
			$boEmpenho = $db->pegaUm("select count(eobid) from par.empenhosubacao where sbaid = $sbaid and eobstatus = 'A'");
			$boEmpenho = ($boEmpenho == 0 ? true : false);
			
			if( in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) 
				|| in_array(PAR_PERFIL_ADM_OBRAS,$arrPerfil) 
				|| in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil) 
				|| in_array(PAR_PERFIL_EQUIPE_TECNICA,$arrPerfil) 
				|| in_array(PAR_PERFIL_ANALISTA_MERITOS,$arrPerfil) 
				|| in_array(PAR_PERFIL_CONSULTA,$arrPerfil) 
				|| in_array(PAR_PERFIL_CONSULTA_MUNICIPAL,$arrPerfil)
                || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL,$arrPerfil)
				|| (( (in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrPerfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrPerfil)) && $estadoAtual['esdid'] != WF_DIAGNOSTICO)) 
				|| ((in_array(PAR_PERFIL_PREFEITO, $arrPerfil) && $estadoAtual['esdid'] != WF_DIAGNOSTICO)) 
				|| (( (in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrPerfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrPerfil)) && $estadoAtual['esdid'] != WF_DIAGNOSTICO))
				|| in_array(PAR_PERFIL_ORGAO_DE_CONTROLE,$arrPerfil) )
			{
				$arSubacao[$resultado["aciid"]][$sbaid] = array( 'descricao'=> $subOrdem.". ".$subDesc, 'sbdid'=> $sbdid);
				if( $estadoAtual['esdid'] == WF_ELABORACAO && $boEmpenho ){
					if( in_array(PAR_PERFIL_EQUIPE_ESTADUAL,$arrPerfil) 
						|| in_array(PAR_PERFIL_EQUIPE_MUNICIPAL,$arrPerfil) 
						|| in_array(PAR_PERFIL_PREFEITO,$arrPerfil) 
						|| in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil) 
						|| in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) 
						// || in_array(PAR_PERFIL_ORGAO_DE_CONTROLE,$arrPerfil) 
						){
						//exclui suba��o
						if( $resultado['sbaextraordinaria'] == '' ){
							$btnSubacoes[$resultado["aciid"]][$sbaid] = '<img border="0" onclick="excluirSubacao('.$sbaid.', '.$resultado["crtpontuacao"].', '.$resultado["aciid"].');" src="../imagens/excluir.gif" align="absmiddle" style="cursor:pointer;"  title="Excluir Suba��o" />';
						}
					}
				} elseif( $estadoAtual['esdid'] == WF_ANALISE && $boEmpenho ){
					if( in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil) 
						|| in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) 
						// || in_array(PAR_PERFIL_ORGAO_DE_CONTROLE,$arrPerfil) 
						){
						//exclui suba��o
						if( $resultado['sbaextraordinaria'] == '' ){
							$btnSubacoes[$resultado["aciid"]][$sbaid] = '<img border="0" onclick="excluirSubacao('.$sbaid.', '.$resultado["crtpontuacao"].', '.$resultado["aciid"].');" src="../imagens/excluir.gif" align="absmiddle" style="cursor:pointer;"  title="Excluir Suba��o" />';
						}
					}
				} elseif( !$boEmpenho ) {
					$btnSubacoes[$resultado["aciid"]][$sbaid] = '<img border="0" src="../imagens/transferencia-empenho.gif" align="absmiddle" style="cursor:pointer;"  title="Suba��o possui registro de empenho. N�o pode ser exclu�da. " />';
				} 
			}
		}	
		$indid[] = $resultado["indid"];
		//ver($arSubacao,d);		
	}

//MONTA A �RVORE
$strdimensaopar = $_REQUEST['strdimensaopar'];
$areapar = $_REQUEST['areapar'];
$indicadorpar = $_REQUEST['indicadorpar'];
$acaopar = $_REQUEST['acaopar'];
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border: none;">
	<tr>
		<td>
		<b>Localizador da Suba��o:</b><br>
			<?=campo_texto('strdimensaopar', 'N', 'S', '', 1, 3, '[#]', '', '', '', '', '', "autoTab('strdimensaopar','areapar');", $strdimensaopar, "this.value=mascaraglobal('[#]',this.value);"); ?>
			<?=campo_texto('areapar', 'N', 'S', '', 1, 3, '[#]', '', '', '', '', '', "autoTab('areapar','indicadorpar');", $areapar, "this.value=mascaraglobal('[#]',this.value);"); ?>
			<?=campo_texto('indicadorpar', 'N', 'S', '', 1, 3, '[#]', '', '', '', '', '', "autoTab('indicadorpar','acaopar');", $indicadorpar, "this.value=mascaraglobal('[#]',this.value);"); ?>
			<?=campo_texto('acaopar', 'N', 'S', '', 1, 3, '[#]', '', '', '', '', '', "autoTab('acaopar','btnPesq');", $acaopar, "this.value=mascaraglobal('[#]',this.value);"); ?>
			<input type="button" name="btnPesq" id="btnPesq" value="Localizar" onclick="pesquisaSubacao();">
			<input type="button" name="btnTodos" id="btnTodos" value="Todas" onclick="window.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore';"><br><br>
		</td>
	</tr>
	<tr>
		<td width="100%" valign="top" style="background: none repeat scroll 0% 0%;" id="_arvore" >
			<div id="sidetree">
				<div id="sidetreecontrol">
					<span> <a href="?#">Fechar Todos</a> | <a href="?#">Abrir Todos</a> <br /><br /> 
						   <img src="../includes/jquery-treeview/images/base.gif" align="top" />Diagn�stico PAR 
					</span> 
				</div>
				<ul id="tree" class="filetree treeview-famfamfam">
					<li><span class="file"><a href="par.php?modulo=principal/orgaoEducacao&acao=A"><strong>Dados da Unidade</strong></a></span></li>
					<li><span class="file"><a href="par.php?modulo=principal/listaObrasParUnidade&acao=A"><strong>Lista de Obras</strong></a></span></li>
                    <?php if (
                        in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrPerfil) ||
                        in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrPerfil) ||
                        in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrPerfil) ||
                        in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrPerfil) ||
                        in_array(PAR_PERFIL_PREFEITO, $arrPerfil) ||
                        in_array(PAR_PERFIL_SUPER_USUARIO, $arrPerfil)){ ?>
                    <li><span class="file"><a href="par.php?modulo=principal/graficoPne&acao=A" target="_self"><strong>Metas do PNE</strong></a></span></li>
                    <?php } ?>
					<li><span><a href="par.php?modulo=principal/questoesPontuais&acao=A"><img alt="Quest�es Pontuais" src="../includes/jquery-treeview/images/question.gif" align="top" border="0" /><strong>Quest�es Pontuais</strong></a></span></li>
					<?php if($_SESSION['par']['itrid'] == 1){ // Se for Estados ?>
					<li><span><a href="#" onclick="dadosDemograficosuf('<?php echo $_SESSION['par']['estuf'];?>');" ><img alt="Indicadores Demogr�ficos e Educacionais" src="../imagens/menu/bt_menu_relatorio.png" align="top" border="0" /><strong> Indicadores Demogr�ficos e Educacionais</strong></a></span></li>
					<?php }else{ // Se for munic�pio ?>
					<li><span class><a href="#" onclick="dadosDemograficosMun(<?php echo $_SESSION['par']['muncod'];?>);" ><img alt="Indicadores Demogr�ficos e Educacionais" src="../imagens/menu/bt_menu_relatorio.png" align="top" border="0" /><strong> Indicadores Demogr�ficos e Educacionais</strong></a></span></li>
					<?php } ?>
					<li><span class="file"><a href="par.php?modulo=principal/escolas&acao=A"><strong>Escolas Atendidas</strong></a></span></li>
					<!-- INDICADORES QUALITATIVOS -->
					<?php 
					if( !(($arrPerfil[0] == PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO || $arrPerfil[0] == PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO_BRASIL_PRO) && !$arrPerfil[1]) ){ ?>
						
					<li><span><img alt="Indicadores Qualitativos" src="../includes/jquery-treeview/images/globe.gif" align="top" /><strong>Indicadores Qualitativos</strong></span>
						<ul>
						<!-- DIMENS�O -->
						<?php foreach ($arDimensao as $idDimensao => $dadosDimensao){ ?>
							<li><span class="folder"><a href="#" title=""><?php echo $dadosDimensao; ?></a></span>
								<ul>
								<!-- �REA -->
								<?php foreach($arArea[$idDimensao] as $idArea => $strArea){ ?>
									<li><span class="folder"><a href="#" title=""><?php echo $strArea; ?></a></span>
										<ul>
										<!-- INDICADOR -->
										<?php foreach($arIndicador[$idArea] as $idIndicador => $arrIndicador){ 
												$class = "";
												if($arrIndicador['ptoid'] !== NULL){ $class = "class=\"check\""; }
										?>
											<li>
												<span <?php echo $class;?>>
													<a href="par.php?modulo=principal/pontuacao&acao=A&indid=<?php echo $idIndicador;?>" title="<?php echo $indicador ?>">
														<?php echo $arrIndicador['descricao'] ?>
													</a>
												</span>
												<?php
													//Quem visualiza a��o e suba��o
													// ver(PAR_PERFIL_ORGAO_DE_CONTROLE,$arrPerfil,d);
													if( in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) 
														|| in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil) 
														|| in_array(PAR_PERFIL_ADM_OBRAS,$arrPerfil) 
														|| in_array(PAR_PERFIL_MANUTENCAO_TABELAS_APOIO,$arrPerfil)
														|| in_array(PAR_PERFIL_CONSULTA,$arrPerfil) 
														|| in_array(PAR_PERFIL_CONSULTA_MUNICIPAL,$arrPerfil)
                                                        || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL,$arrPerfil)
														|| in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrPerfil)
														|| in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrPerfil) 
														|| in_array(PAR_PERFIL_PREFEITO, $arrPerfil) 
														|| in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrPerfil) 
														|| in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$arrPerfil)
														|| in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$arrPerfil)
														|| in_array(PAR_PERFIL_ANALISTA_MERITOS,$arrPerfil)
														|| in_array(PAR_PERFIL_ORGAO_DE_CONTROLE,$arrPerfil)
														){ 
													//	if(in_array($_SESSION['par']['inuid'], $municipiosAbertoParaElaboracao) || in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil)){
														if(is_array($arAcao[$idIndicador])){

															if( $_SESSION['par']['itrid'] == 1 || $grandesCapitais ){
																if( in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) 
																	|| in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil) 
																	|| ((in_array(PAR_PERFIL_PREFEITO, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO)) 
																	|| ((in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO)) 
																	|| ((in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO))
																	|| ((in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO))
																	|| ((in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO))
																	)
																{ ?>
																	<!--<ul><li><span onclick="incluirAcao(<?=$arrIndicador['ptoid']; ?>)"><img border="0" src="../imagens/gif_inclui.gif" align="absmiddle" style="cursor:pointer;" title="Incluir A��o" /><a> Incluir A��o </a></span></li>-->
													<?php } ?><ul><?php
															} else {  ?><ul><?php } ?>
														<?php foreach($arAcao[$idIndicador] as $aciid => $strAcao){ ?>
															<li> <span><?php echo $btnAcoes[$idIndicador][$aciid]; ?><a href="par.php?modulo=principal/parAcao&acao=A&aciid=<?php echo $aciid ?>&indid=<?php echo $idIndicador;  ?>" > <?php echo $strAcao; ?></a></span>
																<?php
																if( ($estadoAtual['esdid'] != WF_DIAGNOSTICO) || (in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil)) ){
																	if(is_array($arSubacao[$aciid])){ ?>
																		<ul>
																		<?php if( $_SESSION['par']['itrid'] == 1 ){ //estadual ?>
																			<?php if( in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) 
																					|| in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil) 
																					|| ((in_array(PAR_PERFIL_PREFEITO, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO)) 
																					|| ((in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO)) 
																					|| ((in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO))
																					|| ((in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO))
																					|| ((in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO))
																					)
																				{ 
																					if( !in_array(PAR_PERFIL_ORGAO_DE_CONTROLE,$arrPerfil) ){?>
																						<li> <span onclick="incluirSubacao(<?=$aciid;?>, <?=$idIndicador; ?>)"><img border="0" src="../imagens/gif_inclui.gif" align="absmiddle" style="cursor:pointer;" title="Incluir Suba��o" /><a> Incluir Suba��o </a></span>
																					<?php } ?>
																				<?php } 
																			} else { //municipal?>
																				<?php if( in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) 
																						|| in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil) 
																						|| ((in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO)) 
																						|| ((in_array(PAR_PERFIL_PREFEITO, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO)) 
																						|| ((in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO))
																						|| ((in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO))
																						|| ((in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO))
																						|| (in_array(PAR_PERFIL_ORGAO_DE_CONTROLE,$arrPerfil))
																						)
																					{
																						if( !in_array(PAR_PERFIL_ORGAO_DE_CONTROLE,$arrPerfil) ){?>
																							<li> <span onclick="incluirSubacaoMun(<?=$aciid;?>, <?=$idIndicador; ?>)"><img border="0" src="../imagens/gif_inclui.gif" align="absmiddle" style="cursor:pointer;" title="Incluir Suba��o" /><a> Incluir Suba��o </a></span>
																						<?php }
																					} 
																				} ?>
																		<?php foreach($arSubacao[$aciid] as $sbaid => $strSubacao){
																			$class = "";
																			//if($strSubacao['sbdid'] !== '0'){ $class = "class=\"check\""; } ?>
																			<li><span <?php echo $class;?>><?php echo $btnSubacoes[$aciid][$sbaid]; ?><a onmouseover="SuperTitleAjax(u+'<?php echo $sbaid; ?>',this)" onmouseout="SuperTitleOff(this);" href="javascript:listarSubacao('<?php echo $sbaid; ?>')" > <?php echo $strSubacao['descricao']; ?></a></span></li>
																		<?php } // FIM SUBA��O ?>
																			
																		</ul>
																<?php } else { ?>
																	<?php if( $_SESSION['par']['itrid'] == 1 ){ ?>
																	<?php if( in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) 
																				|| in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil) 
																				|| ((in_array(PAR_PERFIL_PREFEITO, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO)) 
																				|| ((in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO)) 
																				|| ((in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO))
																				|| ((in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO))
																				|| ((in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO))
																				|| (in_array(PAR_PERFIL_ORGAO_DE_CONTROLE,$arrPerfil))
																				)
																			{ ?>
																		<ul><li> <span onclick="incluirSubacao(<?=$aciid;?>, <?=$idIndicador; ?>)"><img border="0" src="../imagens/gif_inclui.gif" align="absmiddle" style="cursor:pointer;" title="Incluir Suba��o" /><a> Incluir Suba��o </a></span></li></ul>
																		<?php } } else { ?>
																			<?php if( in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) 
																				|| in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil) 
																				|| ((in_array(PAR_PERFIL_PREFEITO, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO)) 
																				|| ((in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO)) 
																				|| ((in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO))
																				|| ((in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO))
																				|| ((in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO))
																				|| (in_array(PAR_PERFIL_ORGAO_DE_CONTROLE,$arrPerfil))
																				)
																			{ ?>
																				<ul><li> <span onclick="incluirSubacaoMun(<?=$aciid;?>, <?=$idIndicador; ?>)"><img border="0" src="../imagens/gif_inclui.gif" align="absmiddle" style="cursor:pointer;" title="Incluir Suba��o" /><a> Incluir Suba��o </a></span></li></ul>
																		<?php }} ?>
																<?php }}?>
															
															</li>
														<?php } // FIM DA A��O ?>
													</ul>
												
												<?php  } else { //SE N�O VIER A��O DO GUIA!?>
															<?php if( $_SESSION['par']['itrid'] == 1 ){ //ESTADO?>
															<?php if( in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) 
																	|| in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil) 
																	|| ((in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO))
																	|| ((in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO))
																	)
																	{ ?>
																<ul><li><span onclick="incluirAcao(<?=$arrIndicador['ptoid']; ?>)"><img border="0" src="../imagens/gif_inclui.gif" align="absmiddle" style="cursor:pointer;" title="Incluir A��o" /><a> Incluir A��o </a></span></li></ul>
												<?php 		} } 
														}
												//	}
												} ?>
											
											</li>
										<?php } // FIM INDICADOR  ?>
										</ul>
									</li>
								<?php } // FIM �REA ?>
								</ul>
							</li>
						<?php } // FIM FOREACH DIMENS�O ?>
						</ul>
					</li>
				<?
				}
				if( $_SESSION['par']['itrid'] == 1 && ( 
						in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) || 
						in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) ||
						in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil) ||
						in_array(PAR_PERFIL_EQUIPE_ESTADUAL,$arrPerfil) ||
						in_array(PAR_PERFIL_EQUIPE_FINANCEIRA,$arrPerfil) ||
						in_array(PAR_PERFIL_EQUIPE_TECNICA,$arrPerfil) ||
						in_array(PAR_PERFIL_ENGENHEIRO_FNDE,$arrPerfil) ||
						in_array(PAR_PERFIL_COORDENADOR_GERAL,$arrPerfil) ||
						in_array(PAR_PERFIL_COORDENADOR_TECNICO,$arrPerfil) ||
						in_array(PAR_PERFIL_CONSULTA,$arrPerfil) ||
						in_array(PAR_PERFIL_CONSULTA_ESTADUAL,$arrPerfil) ||
                        in_array(PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL,$arrPerfil) ||
						in_array(PAR_PERFIL_MANUTENCAO_TABELAS_APOIO,$arrPerfil) ||
						in_array(PAR_PERFIL_PAGADOR,$arrPerfil) ||
						in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$arrPerfil) ||
						in_array(PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO,$arrPerfil) ||
						in_array(PAR_PERFIL_ANALISTA_MERITOS,$arrPerfil) ||
						in_array(PAR_PERFIL_EMPENHADOR,$arrPerfil) ||
						in_array(PAR_PERFIL_GERADOR_DOCUMENTO,$arrPerfil) ||
						in_array(PAR_PERFIL_ALTA_GESTAO_MEC,$arrPerfil) ||
						in_array(PAR_PERFIL_UNIVERSIDADE_ESTADUAL,$arrPerfil) ||
						in_array(PAR_PERFIL_SECRETARIO_ESTADUAL_EDUCACAO,$arrPerfil) ||
						in_array(PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO,$arrPerfil) ||
						in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO_BRASIL_PRO,$arrPerfil) ||
						in_array(PAR_PERFIL_ADM_OBRAS,$arrPerfil)
					 )){ // BRASIL PRO
					$obDim 			= new Dimensao();
					$arDimensao 	= array();
					$arArea 		= array();
					$arIndicador 	= array();
					$arAcao 		= array();
					$indid 			= array();
					$ptoid 			= NULL;
					$aciid 			= NULL;
					$arDados 		= $obDim->lista('array', 3, $whereExterno, $whereInterno, $InnerInterno);
					$arDados = ($arDados ? $arDados : array());
					foreach( $arDados as $resultado ){
						$sbaid 		= NULL;	
						$subDesc 	= NULL;	
						$subOrdem 	= NULL;	
						if(!in_array($resultado["indid"],$indid) ){ // SE MUDAR O INDICADOR LIMPA PONTUA��O A��O E SUBA��O
							$ptoid 		= NULL;	
							$aciid 		= NULL;
							$acidesc 	= NULL;
						}
						if($resultado["ptoid"] !== NULL){ // SE PONTUA��O EXISTE CARREGA
							$ptoid = $resultado["ptoid"];
						}
						if( !is_null($resultado["aciid"]) ){ // SE A��O EXISTE CARREGA
							$aciid = $resultado["aciid"];
							$acidesc =  $resultado["acidsc"];
						}
						if( !is_null($resultado["sbaid"]) ){ // SE A��O EXISTE CARREGA
							$sbaid = $resultado["sbaid"];
							$sbdid = $resultado["sbdid"];
							$subDesc =  $resultado["sbadsc"];
							$subOrdem = $resultado["sbaordem"];
						}
						// ARRAY DIMENS�O
						$arDimensao[$resultado["dimid"]] 						= $resultado["dimcod"].". ".$resultado["dimdsc"];
						// ARRAY AREA
						$arArea[$resultado["dimid"]][$resultado["areid"]] 		= $resultado["arecod"].". ".$resultado["aredsc"];
						// ARRAY INDICADOR
						$arIndicador[$resultado["areid"]][$resultado["indid"]] 	= array( 'descricao'=> $resultado["indcod"].". ".$resultado["inddsc"], 'ptoid'=> $ptoid);
						// ARRAY A��O
				
						if($aciid){ 
							if( in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) 
								|| in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil) 
								|| in_array(PAR_PERFIL_EQUIPE_TECNICA,$arrPerfil) 
								|| in_array(PAR_PERFIL_ANALISTA_MERITOS,$arrPerfil) 
								|| in_array(PAR_PERFIL_CONSULTA,$arrPerfil) 
								|| in_array(PAR_PERFIL_CONSULTA_MUNICIPAL,$arrPerfil)
                                || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL,$arrPerfil)
								|| in_array(PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO,$arrPerfil) 
								|| in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrPerfil)
								|| in_array(PAR_PERFIL_PREFEITO, $arrPerfil) 
								|| in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrPerfil)
								|| in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$arrPerfil) 
								|| in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$arrPerfil)
								|| in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO_BRASIL_PRO,$arrPerfil)
							)
							{ 
								$arAcao[$resultado["indid"]][$aciid] =  $acidesc;
								if( !( in_array(PAR_PERFIL_CONSULTA,$arrPerfil) || in_array(PAR_PERFIL_CONSULTA_MUNICIPAL,$arrPerfil) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL,$arrPerfil) )
								&& ( in_array(PAR_PERFIL_EQUIPE_ESTADUAL,$arrPerfil) 
								|| in_array(PAR_PERFIL_EQUIPE_MUNICIPAL,$arrPerfil) 
								|| in_array(PAR_PERFIL_PREFEITO,$arrPerfil) 
								|| in_array(PAR_PERFIL_EQUIPE_TECNICA,$arrPerfil) 
								|| in_array(PAR_PERFIL_EQUIPE_FINANCEIRA,$arrPerfil)
								|| in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$arrPerfil)
								||  in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$arrPerfil)
								) ){
									//exclui a��o
									$btnAcoes[$resultado["indid"]][$aciid] = '<img border="0" onclick="excluirAcao('.$aciid.');" src="../imagens/excluir.gif" align="absmiddle" style="cursor:pointer;"  title="Excluir A��o" />';
								}
							}
						}
						// ARRAY SUBA��O
						if($sbaid){
							if( in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) 
								|| in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil) 
								|| in_array(PAR_PERFIL_EQUIPE_TECNICA,$arrPerfil) 
								|| in_array(PAR_PERFIL_ANALISTA_MERITOS,$arrPerfil) 
								|| in_array(PAR_PERFIL_CONSULTA,$arrPerfil) 
								|| in_array(PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO,$arrPerfil) 
								|| in_array(PAR_PERFIL_CONSULTA_MUNICIPAL,$arrPerfil)
                                || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL,$arrPerfil)
								|| (in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrPerfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrPerfil)) 
								|| (in_array(PAR_PERFIL_PREFEITO, $arrPerfil) ) 
								|| (in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO_BRASIL_PRO, $arrPerfil) ) 
								|| (in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrPerfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrPerfil)))
							{
								$arSubacao[$resultado["aciid"]][$sbaid] = array( 'descricao'=> $subOrdem.". ".$subDesc, 'sbdid'=> $sbdid);
								if( $estadoAtual['esdid'] == WF_ELABORACAO ){
									if( in_array(PAR_PERFIL_EQUIPE_ESTADUAL,$arrPerfil) 
										|| in_array(PAR_PERFIL_EQUIPE_MUNICIPAL,$arrPerfil) 
										|| in_array(PAR_PERFIL_PREFEITO,$arrPerfil) 
										|| in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil) 
										|| in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) ){
										//exclui suba��o
										if( $resultado['sbaextraordinaria'] == '' ){
											$btnSubacoes[$resultado["aciid"]][$sbaid] = '<img border="0" onclick="excluirSubacao('.$sbaid.', '.$resultado["crtpontuacao"].', '.$resultado["aciid"].');" src="../imagens/excluir.gif" align="absmiddle" style="cursor:pointer;"  title="Excluir Suba��o" />';
										}
									}
								} elseif( $estadoAtual['esdid'] == WF_ANALISE ){
									if( in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil) 
										|| in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) ){
										//exclui suba��o
										if( $resultado['sbaextraordinaria'] == '' ){
											$btnSubacoes[$resultado["aciid"]][$sbaid] = '<img border="0" onclick="excluirSubacao('.$sbaid.', '.$resultado["crtpontuacao"].', '.$resultado["aciid"].');" src="../imagens/excluir.gif" align="absmiddle" style="cursor:pointer;"  title="Excluir Suba��o" />';
										}
									}
								}
							}
						}	
						$indid[] = $resultado["indid"];
						//ver($arSubacao);		
					}
	//			}
				$arqid = $db->pegaUm( "SELECT arqid FROM par.protocolo WHERE terid = 1 AND inuid = ".$_SESSION['par']['inuid'] );
				if( $arqid ){ // Brasil Pr� em an�lise
					$BPAnalise = false;
				}else{
					$BPAnalise = true;
				}
				?>
				<li><span><img alt="Indicadores Qualitativos Brasil Profissionalizado" src="../includes/jquery-treeview/images/globe.gif" align="top" /><strong>Indicadores Qualitativos - Educa��o Profissional</strong></span>
						<ul>
						<!-- DIMENS�O -->
						<?php foreach ($arDimensao as $idDimensao => $dadosDimensao){ ?>
							<li><span class="folder"><a href="#" title=""><?php echo $dadosDimensao; ?></a></span>
								<ul>
								<!-- �REA -->
								<?php foreach($arArea[$idDimensao] as $idArea => $strArea){ ?>
									<li><span class="folder"><a href="#" title=""><?php echo $strArea; ?></a></span>
										<ul>
										<!-- INDICADOR -->
										<?php foreach($arIndicador[$idArea] as $idIndicador => $arrIndicador){  
												$class = "";
												if($arrIndicador['ptoid'] !== NULL){ $class = "class=\"check\""; }
										?>
											<li><span <?php echo $class;?>><a href="par.php?modulo=principal/pontuacao&acao=A&indid=<?php echo $idIndicador;?>" title="<?php echo $indicador ?>"><?php echo $arrIndicador['descricao'] ?></a></span>
												<?php  
													//Quem visualiza a��o e suba��o
													if( in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) 
														|| in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil) 
														|| in_array(PAR_PERFIL_MANUTENCAO_TABELAS_APOIO,$arrPerfil)
														|| in_array(PAR_PERFIL_CONSULTA,$arrPerfil) 
														|| in_array(PAR_PERFIL_CONSULTA_MUNICIPAL,$arrPerfil)
                                                        || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL,$arrPerfil)
                                                        || in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrPerfil)
														|| in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrPerfil) 
														|| in_array(PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO, $arrPerfil) 
														|| in_array(PAR_PERFIL_PREFEITO, $arrPerfil) 
														|| in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrPerfil) 
														|| in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$arrPerfil)
														|| in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$arrPerfil)
														|| in_array(PAR_PERFIL_ANALISTA_MERITOS,$arrPerfil)
														|| in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO_BRASIL_PRO,$arrPerfil)
														){ 
													//	if(in_array($_SESSION['par']['inuid'], $municipiosAbertoParaElaboracao) || in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil)){
														if(is_array($arAcao[$idIndicador])){
																
															if( $_SESSION['par']['itrid'] == 1 || $grandesCapitais ){
																if( in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) 
																			|| in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil) 
																			|| ((in_array(PAR_PERFIL_PREFEITO, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO)) 
																			|| ((in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO)) 
																			|| ((in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO))
																			|| ((in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO))
																			|| ((in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO))
																			|| ((in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO_BRASIL_PRO, $arrPerfil) && $estadoAtual['esdid'] == WF_ELABORACAO))
																			)
																		{ //ver('teste2'); ?>
																	<!--<ul><li><span onclick="incluirAcao(<?=$arrIndicador['ptoid']; ?>)"><img border="0" src="../imagens/gif_inclui.gif" align="absmiddle" style="cursor:pointer;" title="Incluir A��o" /><a> Incluir A��o </a></span></li>-->
													<?php } ?><ul><?php
															} else {  ?><ul><?php } ?>
														<?php foreach($arAcao[$idIndicador] as $aciid => $strAcao){  ?>
															<li> <span><?php echo $btnAcoes[$idIndicador][$aciid]; ?><a href="par.php?modulo=principal/parAcao&acao=A&aciid=<?php echo $aciid ?>&indid=<?php echo $idIndicador;  ?>" > <?php echo $strAcao; ?></a></span>
																<?php  
																	if(is_array($arSubacao[$aciid])){ ?>
																		<ul>
																		<?php if( $_SESSION['par']['itrid'] == 1 ){ //estadual ?>
																			<?php if( in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) 
																					|| in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil) 
																					|| ((in_array(PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO, $arrPerfil)) && $BPAnalise)
																					|| ((in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO_BRASIL_PRO, $arrPerfil)) && $BPAnalise)
																					
																					)
																				{ ?>
																					<li> <span onclick="incluirSubacao(<?=$aciid;?>, <?=$idIndicador; ?>)"><img border="0" src="../imagens/gif_inclui.gif" align="absmiddle" style="cursor:pointer;" title="Incluir Suba��o" /><a> Incluir Suba��o </a></span>
																			<?php } }  ?>
																		<?php foreach($arSubacao[$aciid] as $sbaid => $strSubacao){ 
																			$class = "";
																			//if($strSubacao['sbdid'] !== '0'){ $class = "class=\"check\""; } ?>
																			<li><span <?php echo $class;?>><?php echo $btnSubacoes[$aciid][$sbaid]; ?><a onmouseover="SuperTitleAjax(u+'<?php echo $sbaid; ?>',this)" onmouseout="SuperTitleOff(this);" href="javascript:listarSubacao('<?php echo $sbaid; ?>')" > <?php echo $strSubacao['descricao']; ?></a></span></li>
																		<?php } // FIM SUBA��O ?>
																			
																		</ul>
																<?php }?>
															
															</li>
														<?php } // FIM DA A��O ?>
													</ul>
												
												<?php  } 
												} ?>
											
											</li>
										<?php } // FIM INDICADOR  ?>
										</ul>
									</li>
								<?php } // FIM �REA ?>
								</ul>
							</li>
						<?php } // FIM FOREACH DIMENS�O ?>
						</ul>
					</li>
				</ul>
			<?php } // FIM DE CEAR� ?> 
		</td>
	</tr>
</table>
<?php } ?>