<?php

if($_REQUEST['requisicao']=='validararquivo') {
	ob_clean();
	$db->executar("UPDATE public.arquivo_recuperado SET arqvalidacao=true WHERE arqid='".$_REQUEST['arqid']."'");
	$db->commit();
	die("TRUE");
}



//$escrita = verificaPermiss�oEscritaUsuarioPreObra($_SESSION['usucpf'], $_REQUEST['preid']);

$obPreObra 	= new PreObraControle();
//$db 		= new cls_banco();
 
$preid = ($_SESSION['par']['preid']) ? $_SESSION['par']['preid'] : $_REQUEST['preid'];
$docid = prePegarDocid($preid);
$esdid = prePegarEstadoAtual($docid);

$respSim = $respSim ? $respSim : array();

$arTipoObraDocumentos = $obPreObra->recuperarArTiposObraDocumentos($preid);
$arTipoObraDocumentos = $arTipoObraDocumentos ? $arTipoObraDocumentos : array();

$boDocDominialidade = $obPreObra->verificaDocumentoDominialidade($preid);

if($_REQUEST['download'] == 's'){	
	
	$obPreObra->documentoDownloadAnexo($_GET['arqid']);
	die();
}

$boAtivo = 'N';
$stAtivo = 'disabled="disabled"';
/////////////////////////////////////////////////////////////
if( $esdid ){
	
	$obSubacaoControle2 = new SubacaoControle();
	$obPreObra2 = new PreObra();
	
	if($preid){
		$arDados = $obPreObra2->recuperarPreObra($preid);
		/*
		$sql = "SELECT 
					pre.preid,
					pre.docid,
					pre.presistema,
					pre.preidsistema,
					pre.ptoid,
					pre.preobservacao,
					pre.prelogradouro,
					pre.precomplemento,
					pre.estuf,
					pre.muncod,
					pre.precep,
					pre.prelatitude,
					pre.prelongitude,
					pre.predtinclusao,
					pre.prebairro,
					pre.preano,
					pre.qrpid,
					pre.predescricao,
					pre.prenumero,
					pre.pretipofundacao,
					pre.entcodent,
					pto.ptodescricao,
					pto.ptoprojetofnde,
					mun.mundescricao
				FROM obras.preobra pre
				LEFT JOIN territorios.municipio mun ON mun.muncod = pre.muncod
				INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
				WHERE preid = '{$preid}'
				AND prestatus = 'A'";
		
		$arDados = $db->pegaLinha($sql);
		*/
	}
	
	// Regra passada pelo Daniel - 9/6/11
	if(possuiPerfil(array(PAR_PERFIL_COORDENADOR_GERAL)) && 
	   $esdid == WF_PRONATEC_OBRA_APROVADA && $arDados['ptoprojetofnde'] == 'f') {
		$boAtivo = 'S';
		$stAtivo = '';
	} else {
		$arrPerfil = array(PAR_PERFIL_EQUIPE_MUNICIPAL, PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,PAR_PERFIL_EQUIPE_ESTADUAL,PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,PAR_PERFIL_PREFEITO,PAR_PERFIL_SUPER_USUARIO);
		$arrSituacao = array(WF_PRONATEC_EM_CADASTRAMENTO, WF_PRONATEC_EM_DILIGENCIA);
		
		if( in_array($esdid, $arrSituacao) && possuiPerfil($arrPerfil) ){
			$boAtivo = 'S';
			$stAtivo = '';
		}
	}
}
//dbg('ok');
//dbg($arDados,1);
/////////////////////////////////////////////////////////////

if($_POST['poasituacao'] && $boAtivo == 'S'){
	
	$sql = "UPDATE obras.preobraanexo SET poasituacao = 'A' WHERE preid = {$preid}";
	$db->executar($sql);
	
	$arDados = $_POST['poasituacao'];
	foreach($arDados as $podid){
		$sql = "UPDATE obras.preobraanexo SET poasituacao = 'B' WHERE preid = {$preid} AND podid = {$podid}";
		$db->executar($sql);
	}
	$db->commit();
}

if($_GET['arqidDel'] && $boAtivo == 'S' ){
	$obPreObra->excluirDocumentosPreObra($_GET['arqidDel'],$preid);
}


//if($esdid == WF_TIPO_EM_ANALISE_FNDE){
//	$boAnalise = true;
//}
?>
<script language="javascript" type="text/javascript">

	var boAtivo = '<?=$boAtivo?>';

	jQuery(document).ready(function(){

		jQuery('.navegar').click(function(){

			if(this.value == 'Anterior'){
				aba = 'Foto';
			}

			if(this.value == 'Pr�ximo'){
				aba = 'analise';
			}

			document.location.href = 'par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba='+aba+'&preid='+<?php echo $preid ?>;
		});

		jQuery('.excluir').click(function(){

			if(!confirm('Deseja deletar este documento?')){
				return false;
			}
			document.location.href = 'par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba=documento&preid='+<?php echo $preid ?>+'&arqidDel='+this.id;
		});

		jQuery("#formulario").validate({
			ignoreTitle: true,
			rules: {
				podid: "required",
				arquivo: "required",
				poadescricao: "required"
			}
		});

		jQuery(".modelo").click(function(){

			var arDados = this.id.split("_");
			
			if(arDados[0] == 8 || arDados[0] == 9){
				return window.open('documentos/modeloDados.php?modelo='+arDados[0]+'&preid='+arDados[1], 
						   'modelo', 
						   "height=600,width=950,scrollbars=yes,top=50,left=200" );
			}else{
				return window.open('documentos/modelo.php?modelo='+arDados[0]+'&preid='+arDados[1], 
						   'modelo', 
						   "height=600,width=950,scrollbars=yes,top=50,left=200" );
			}
		});		

		jQuery(".anexo").click(function(){
			document.location.href = 'par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba=documento&preid='+<?php echo $preid ?>+'&download=s&arqid='+this.id;
		});
	});
	 
	function excluirAnexo( arqid ){
		var preid = jQuery('#preid').val();
		if( boAtivo == 'S' ){
	 		if ( confirm( 'Deseja excluir o Documento?' ) ) {
	 			location.href= window.location+'&arqidDel='+arqid+'&preid='+preid;
	 		}
		}
 	}

 	function popup_arquivo(podid, preid, sisid)
 	{
 	 	if(podid == 'disabled'){
			alert("Documento j� analisado e bloqueado pelo FNDE.");
			return false;
 	 	}
 	 	
 		return window.open('geral/upload_arquivo.php?podid='+podid+'&preid='+preid+'&sisid='+sisid, 
				   'anexo', 
				   "height=300,width=450,scrollbars=yes,top=50,left=200" );
 	}

 	function popup_ajuda(link)
 	{
 		return window.open('geral/popup_ajuda.php?link='+link, 
				   'ajuda', 
				   "height=300,width=450,scrollbars=yes,top=50,left=200" );
 	}
 	
 	function validarFoto(arqid){
		if(confirm('Deseja realmente validar esta foto ?')){
			jQuery.ajax({
		   		type: "POST",
		   		url: window.location.href,
		   		data: "requisicao=validararquivo&arqid="+arqid,
		   		success: function(msg){
		   			if(msg=="TRUE") {
		   				alert("Arquivo validado com sucesso");
		   				window.location=window.location;
		   			} else {
		   				alert("Arquivo n�o validado com sucesso");
		   				window.location=window.location;
		   			}
		   		}
		 		});
			
		}
	}
 
</script>
<?php 
$preidTx = $_SESSION['par']['preid'] ? '&preid='.$_SESSION['par']['preid'] : '';
$lnkabas = "par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba=Documento".$preidTx;

echo carregaAbasPronatec($lnkabas);
monta_titulo( 'Documentos Anexos', ''  );
?>
<?php echo cabecalho();?>

<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript">
				messageObj = new DHTML_modalMessage();	// We only create one object of this class
				messageObj.setShadowOffset(5);	// Large shadow
				
				function displayMessage(url) {
					messageObj.setSource(url);
					messageObj.setCssClassMessageBox(false);
					messageObj.setSize(690,400);
					messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
					messageObj.display();
				}
				function displayStaticMessage(messageContent,cssClass) {
					messageObj.setHtmlContent(messageContent);
					messageObj.setSize(600,150);
					messageObj.setCssClassMessageBox(cssClass);
					messageObj.setSource(false);	// no html source since we want to use a static message here.
					messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
					messageObj.display();
				}
				function closeMessage() {
					messageObj.close();	
				}
</script>

<form action="" method="post">
	<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
		<table width="95%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
			<thead>
				<tr>
					<td valign="top" width="40" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>Item</b></td>
					<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>Descri��o</b></td>
					<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>Anexo(s)</b></td>
					<?php if($boAnalise): ?>
						<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>Bloquear</b></td>
					<?php endif; ?>				
					<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>A��es</b></td>
				</tr>
			</thead>
			<tbody>
				<?php $x = 0; ?>
				<?php foreach($arTipoObraDocumentos as $tipos): ?>
					<?php
					$cor = ($x % 2) ? "#F7F7F7" : "white"; 
					?>
					<tr bgcolor="<?php echo $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor ?>';">
						<td align="center"><?php echo $x+1 //$tipos['codigo'] ?></td>			
						<td><?php echo $tipos['descricao'] ?></td>
						<td align="right" width="350px">
							<table width="100%" border="0" cellspacing="0" cellpadding="0">
								<tr>
									<td>
										<?php
										$trav = true;
	
										if( ($esdid == WF_PRONATEC_EM_CADASTRAMENTO) && possuiPerfil($arrPerfil) ){
											$trav = false;
										}
										if(possuiPerfil(array(PAR_PERFIL_COORDENADOR_GERAL)) && 
										   $esdid == WF_PRONATEC_OBRA_APROVADA && $arDados['ptoprojetofnde'] == 'f') {
											$trav = false;
										}
										if( ($esdid == WF_PRONATEC_EM_DILIGENCIA) && possuiPerfil($arrPerfil) ){
											switch ($tipos['codigo']) {
											    case "3":
											        $trav = in_array(QUESTAO_ESTUDO_DEM,$respSim);
											        $txtAjuda = "Anexar o Estudo de Demanda elaborado, devidamente assinado, justificando e apresentando a necessidade de implanta��o da obra solicitada, focado na regi�o de abrang�ncia, informando o n�mero de crian�as a serem atendidas na faixa et�ria pleiteada e apresentando as fontes de pesquisa (ex. levantamento da Secretaria de Sa�de sobre vacina��o infantil).";
											        break;
											    case "1":
											        $trav = in_array(QUESTAO_PLANILHA_LOCALIZACAO,$respSim);
											        $txtAjuda = "Anexar arquivo de imagem da Planta de Localiza��o, destacando o terreno selecionado na malha urbana, indicando o endere�o, apresentando seu entorno, os acessos ao lote, as refer�ncias naturais e constru�das pr�ximas, localizando o terreno proposto dentro do munic�pio. ";
											        break;
											    case "4":
											        $trav = in_array(QUESTAO_PLANILHA_SIT,$respSim);
											        $txtAjuda = "Anexar arquivo de imagem da Planta de Situa��o, indicando as dimens�es do terreno e apresentando os lotes e vias lim�trofes.";
											        break;
											    case "6":
											        $trav = in_array(QUESTAO_PLANILHA_LOCACAO,$respSim);
											        $txtAjuda = "Anexar arquivo de imagem da Planta de Loca��o da obra solicitada no terreno proposto. � necess�rio: 1. Apresentar a Planta Baixa da obra solicitada (disponibilizada no site do FNDE), indicando as dimens�es do terreno e as dimens�es gerais da edifica��o, bem como os afastamentos entre esta e os limites do terreno. Cabe ressaltar que o afastamento das edifica��es em rela��o aos limites do terreno dever� obedecer �s determina��es do Plano Diretor local e demais leis vigentes; 2. Locar a entrada das redes de abastecimento de energia, �gua e esgoto, sempre respeitando as orienta��es t�cnicas definidas pelas concession�rias locais. Em caso de utiliza��o de fossa s�ptica, sumidouro, cisterna ou outro elemento que tenha como fun��o suprir defici�ncia em redes existentes, tais instrumentos dever�o ser tamb�m locados; 3. Defini��o das cotas de n�vel de cada edifica��o existente, bem como dos acessos e �reas verdes em rela��o � via de acesso ou �s cal�adas lim�trofes; 4. Apresentar norte magn�tico e ventos dominantes; 5. Representar em planta o escoamento de �guas pluviais, muros de conten��o e qualquer outro tipo de obra que n�o conste em projeto-padr�o, mas que seja imprescind�vel ao funcionamento adequado da escola.";
											        break;
											    case "5":
											        $trav = in_array(QUESTAO_PLANIALTIMETRICO,$respSim);
											        $txtAjuda = "Encaminhar o Levantamento Planialtim�trico do terreno proposto, apresentando as curvas de n�vel cotadas a cada metro de desn�vel.";
											        break;
											    case "9":
											        $trav = in_array(QUESTAO_DOMINIALLIDADE,$respSim);
											        $txtAjuda = "Gerar a declara��o padr�o de dominialidade, disponibilizada no sistema, preenchendo os dados solicitados; imprimir o documento gerado que deve ser assinado pelo prefeito ou represente legal do munic�pio e anexar em campo pr�prio o arquivo de imagem do documento ou, caso j� tenha-se dispon�vel, da Certid�o de Matr�cula do Im�vel.";
											        break;
											    case "7":
											        $trav = in_array(QUESTAO_FORNECIMENTO_INFRA,$respSim);
											        $txtAjuda = "Gerar a declara��o padr�o de responsabilidade pelo fornecimento e manuten��o dos servi�os de abastecimento de �gua, energia el�trica, esgotamento sanit�rio e pela coleta de lixo para o terreno proposto para edifica��o do objeto pleiteado, al�m da execu��o dos servi�os de terraplanagem, caso sejam necess�rios. Preencher os dados solicitados; imprimir o documento gerado que deve ser assinado pelo prefeito ou represente legal do munic�pio e anexar em campo pr�prio o arquivo de imagem do documento.";
											        break;
											    case "8":
											        $trav = in_array(QUESTAO_COMPATIBILIDADE_FUNDACAO,$respSim);
											        $txtAjuda = "Gerar a declara��o padr�o de adequa��o da funda��o, disponibilizada no sistema, preenchendo os dados solicitados; imprimir o documento gerado que deve ser assinado por profissional competente e anexar em campo pr�prio o arquivo de imagem do documento. No caso de verificar-se inadequa��o do solo ao tipo de funda��o proposta, o munic�pio dever� apresentar novo projeto executivo de funda��o contendo, inclusive, a Anota��o de Responsabilidade T�cnica - ART.";
											        break;
											}
										}


										$arAnexos = $obPreObra->recuperarDocumentosAnexoPorPodid($tipos['codigo'], $preid);
										$arAnexos = $arAnexos ? $arAnexos : array();
										$Anexar   = true;
										echo '<table width=100% border="0" cellspacing="0" cellpadding="0">';
										foreach($arAnexos as $anexo){
											$excluir = '';
											$subtituir = true;
											$restricao = '';
											if($tipos['situacao'] == 'B' || $boAtivo == 'N' || $trav){
//												echo '<img src="../imagens/excluir_01.gif" align="absmiddle" title="Excluir documento" style="padding-right:5px;padding-bottom:5px;" />';
											}else{
												if( $esdid == WF_PRONATEC_EM_DILIGENCIA ){
													$subtituir = true;
												}
												$excluir = '<img class="excluir" id="'.$anexo['codigo'].'" src="../imagens/excluir.gif" align="absmiddle" title="Excluir documento" style="cursor:pointer;padding-right:5px;padding-bottom:5px;" />';
											}
											$arqvalidacao = $db->pegaUm("SELECT arqvalidacao FROM public.arquivo_recuperado WHERE arqid='".$anexo['codigo']."'");
											if(!is_file(APPRAIZ."arquivos/obras/".floor($anexo['codigo']/1000)."/".$anexo['codigo'])) {
												if( $esdid == WF_PRONATEC_EM_CADASTRAMENTO ){
													$Anexar   = false;
												}
												if( $subtituir ){
													$restricao = "<img src=../imagens/restricao.png align=absmiddle border=0> <input type=button name=b value=Substituir onclick=\"displayMessage(window.location.href+'&requisicao=telaSubirArquivo&arqid=".$anexo['codigo']."',false);\">";
												}else{
													$restricao = "";
												}
												$alerta = 'background-color: red;';
											}elseif($arqvalidacao=="f") {
												if( possuiPerfil(Array(PAR_PERFIL_COORDENADOR_GERAL,PAR_PERFIL_SUPER_USUARIO)) ){
													$restricao = "<br /> <img src=../imagens/restricao.png align=absmiddle border=0> <input type=button name=b value=Validar onclick=\"validarFoto('".$anexo['codigo']."');\">";
												}
												$restricao .= "<br /><img src=../imagens/restricao.png align=absmiddle border=0> <input type=button name=b value=Substituir onclick=\"displayMessage(window.location.href+'&requisicao=telaSubirArquivo&arqid=".$anexo['codigo']."',false);\">";
											} else {
												$restricao = "&nbsp;";
												$alerta = '';
											}
											
											echo '<tr><td width=60%>'.$excluir.' <a title="Baixar arquivo" href="javascript:void(0)" id="'.$anexo['codigo'].'" class="anexo" style="'.$alerta.'">'.delimitador($anexo['descricao'], 20).'.'.$anexo['extensao'].'</a></td><td>'.$restricao.'</td></tr>';
										}
										echo '</table>';

										?>
									</td>
									<td width="70px;" align="center">
									<?php if( $Anexar ){?>
										<input type="button" value="Anexar" onclick="popup_arquivo('<?php echo ($tipos['situacao'] == 'B') ? "disabled" : $tipos['codigo'] ?>', '<?php echo $preid ?>', '<?php echo SIS_OBRAS ?>')" <?php echo $stAtivo ?> <?= $trav ? 'disabled="disabled"' : ''; ?>/>
									<?php }?>
									</td>
								</tr>						
							</table>															
						</td>
						<?php if($boAnalise): ?>
							<td width="80px" align="center">
								<input type="checkbox" name="poasituacao[]" value="<?php echo $tipos['codigo'] ?>" <?php echo ($tipos['situacao'] == 'B') ? "checked" : "" ?>>
							</td>
						<?php endif; ?>
						<td align="right" width="80">
							<?php if($tipos['anexo']): ?>
								<img alt="Cont�m anexo(s)" title="Cont�m anexo(s)" src="../imagens/clipe.gif">
							<?php endif; ?>
							<?php if(!empty($tipos['podmodelo'])): ?>
								<?php if(!$boDocDominialidade && $tipos['codigo'] == 7): ?>
									<a href="javascript:alert('Deve ser gerada a declara��o de dominialidade primeiro.')" >
										<img border="0" alt="Visualizar o modelo" title="Visualizar o modelo" src="../imagens/consultar.gif" style="cursor:pointer;">
									</a>
								<?php else: ?>
									<img class="modelo" id="<?php echo $tipos['codigo']."_".$preid ?>" alt="Visualizar o modelo" title="Visualizar o modelo" src="../imagens/consultar.gif" style="cursor:pointer;">
								<?php endif; ?>
							<?php endif; ?>
							<?php if(!empty($tipos['podajuda'])): ?>
								<a href="<?php echo str_replace('par/','',$tipos['podajuda']) ?>" target="_blank">
									<img alt="Orienta��es" title="Orienta��es" src="../imagens/IconeAjuda.gif" border="0" style="cursor:pointer;">
								</a>
							<?php endif; ?>
							<?php if(!$trav && count($respSim)): ?>
								<?php
								$imgAjuda = "<img alt=\"{$txtAjuda}\" title=\"{$txtAjuda}\" src=\"/imagens/ajuda.gif\">";
								echo $imgAjuda; 
								?>
							<?php endif; ?>
						</td>
					</tr>
					<?php $x++ ?>
				<?php endforeach; ?>
			</tbody>
	</table>
	<table width="95%" align="center" bgcolor="#DCDCDC">
		<tr>
			<td align="left">
				<input class="navegar" type="button" value="Anterior" />
			</td>
			<td align="center">
				<?php if($boAtivo == 'S'): ?>
					<input type="submit" value="Salvar" />
				<?php endif; ?>
			</td>
			<td align="right">
				<input class="navegar" type="button" value="Pr�ximo" />
			</td>
		</tr>
	</table>
</form>