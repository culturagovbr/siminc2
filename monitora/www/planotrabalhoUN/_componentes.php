<?php


/**
 * @param integer $atividade C�digo da atividade por onde a listagem ser� iniciada
 * @param integer $profundidade Quantidade m�xima de n�veis a ser exibido
 * @param integer $situacao Mostrar apenas as atividades com as situa��es indicadas
 * @param integer $usuario Exibir somente as atividades sob responsabilidade do usu�rio indicado
 * @param integer $diretorio
 * @param integer $numeracao_relativa
 * @param integer $perfis Especifica quais perfis o par�metro $usuario deve fazer refer�ncia 
 */
function arvore( $atividade, $profundidade = 0, $situacao = array(), $usuario = null , $diretorio = null, $numeracao_relativa = false, $perfis = array() ){
	global $db;
	$atividade    = (integer) $atividade;
	$profundidade = (integer) $profundidade;
	$situacao     = (array) $situacao;
	$usuario      = (string) $usuario;
	$diretorio    = (string) ( $diretorio ? $diretorio : $_SESSION['sisdiretorio'] );
	$projeto      = atividade_pegar_projeto( $atividade );
	ob_start();
	?>
<!-- BIBLIOTECAS -->
<script
	language="javascript" type="text/javascript"
	src="../includes/blendtrans.js"></script>
<script
	language="javascript" type="text/javascript"
	src="../includes/JsLibrary/_start.js"></script>
<script
	language="javascript" type="text/javascript"
	src="../includes/JsLibrary/slider/slider.js"></script>
<script
	language="javascript" type="text/javascript"
	src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script
	language="javascript" type="text/javascript"
	src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script
	language="javascript" type="text/javascript"
	src="../includes/JsLibrary/tags/superTitle.js"></script>
	<?php echo arvore_biblioteca( $atividade, $profundidade, $situacao, $usuario , $diretorio, $numeracao_relativa, $perfis ); ?>

<link
	href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"
	type="text/css" rel="stylesheet"></link>
<link
	href="../includes/JsLibrary/slider/slider.css" type="text/css"
	rel="stylesheet"></link>

<!-- BLOCOS INTERNOS DE HTML -->
<div id="sliderDiv"
	style="z-index: 1000; width: 200px; *height: 97px; left: 2px; top: 2px; visibility: visible; display: none;">
<div class="monthYearPicker" style="left: 167px; width: 35px;"
	id="minuteDropDown"></div>
<div class="topBar" id="topBar" style="top: 150px;"><img
	onclick="removeSlider()"
	onmouseover="this.src = '../includes/JsLibrary/slider/images/close_over.gif'"
	onmouseout="this.src = '../includes/JsLibrary/slider/images/close.gif'"
	src="../includes/JsLibrary/slider/images/close.gif"
	style="position: absolute; right: 2px;" /></div>
<div>
<table cellspacing="1" width="100%">
	<tr>
		<td style="background-color: #E2EBED">Situa��o:</td>
		<td style="text-align: left"><select onchange="alteraStatus(this)"
			id="situacaoSlider"
			style="border:1px;border-style:solid;border-color:black; width: 100%; font-size: 10px;">
			<option value='1'>N�o Iniciado</option>
			<option value='2'>Em Andamento</option>
			<option value='3'>Suspenso</option>
			<option value='4'>Cancelado</option>
			<option value='5'>Conclu�do</option>
		</select></td>
	</tr>
	<tr>
		<td style="background-color: #E2EBED">Andamento:</td>
		<td><span id="slider_target"
			style="width: 40px; position: relative; *left: -41px; top: 5px; height: 3px;"></span>
		&nbsp; <input type="text" name="valor" id="valorSlider" value="200"
			style="font-size: 10px; width: 30px" readonly />%</td>
		<script>
							form_widget_amount_slider('slider_target',document.getElementById( "valorSlider" ),70,0,100,"arredonda(document.getElementById( 'valorSlider' ))");
						</script>
	</tr>
	<tr>
		<td colspan="2" style="background-color: #E2EBED"><input type="button"
			onclick="slicerSubmit()" value="Ok"
			style="height:18px; font-size: 10px; " /></td>
	</tr>
</table>
</div>
</div>

<!-- BARRA DE FERRAMENTAS PARA IMPORTA��O E EXPORTA��O -->
	<?php if( !$usuario ): 
		if(!boPerfilSomenteLeitura()){
	?>
<table style="width:100%; border:0; padding:5px;">
	<tr>
		<td style="width:80%; text-align:left;" valign="top">
		<?php if( true || projeto_verificar_responsabilidade( $projeto, $usuario ) || ( $atividade && atividade_verificar_responsabilidade( $atividade, $usuario ) ) ): ?>
		<span title="nova atividade"
			onclick="cadastrar_atividade( <?= $atividade ? $atividade : $projeto ?> );"
			style="cursor: pointer;"> <img align="absmiddle"
			src="/imagens/gif_inclui.gif" /> Cadastrar Atividade </span>
		&nbsp;&nbsp;
		<?
		if($_REQUEST['atiid']) $sbaid = $db->pegaUm("SELECT sbaid FROM monitora.pi_subacaoatividade WHERE atiid='".$_REQUEST['atiid']."'");
		if(!$sbaid) {
		?>
		|&nbsp;&nbsp;<span style="cursor:pointer;" onclick="janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/listarsubacoesatividadeUN&acao=A&atiid=<?= $atividade ?>', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=400' ); janela.focus();"><img src="../imagens/valida2.gif" border="0" align="top" title="Inserir suba��o"/> Inserir Suba��o</span>
		<?
		}
		?>

		<?php endif; ?>
		&nbsp;&nbsp;|&nbsp;&nbsp;
		<span
			title="importar atividade do Microsoft Project"
			onclick="exibir_importar_msproject();" style="cursor: pointer;">
		Importar </span> &nbsp;&nbsp;|&nbsp;&nbsp; <span
			title="exportar para o Microsoft Project"
			onclick="microsoft_project_exportar( false );"
			style="cursor: pointer;"> Exportar Filhos </span>
		&nbsp;&nbsp;|&nbsp;&nbsp; <span
			title="exportar para o Microsoft Project"
			onclick="microsoft_project_exportar( true );"
			style="cursor: pointer;"> Exportar Tudo </span>
			
			
		<div id="formulario_msproject" style="display: none;"><br />
		<form action="" method="post" name="microsoftproject"
			enctype="multipart/form-data"><input type="hidden"
			name="microsoftprojectacao" value="" /> <input type="hidden"
			name="microsoftprojectacaotime" value="" /> <input type="hidden"
			name="microsoftprojectincluipai" value="0" />
		<div id="botoes_msproject_importar" style="display: none;"><img
			align="absmiddle" src="/imagens/msproject.gif" /> Microsoft Project
		&nbsp;&nbsp;&nbsp; <input type="file" name="arquivo" /> <input
			type="button" name="importar" value="Importar"
			onclick="microsoft_project_importar();" /></div>
		</form>
		</div>
		<iframe name="microsoftprojectiframe" src="" style="display: none;"></iframe>


		</td>
		<td style="width:50%; text-align:right;" valign="top"><img
			align="absmiddle" src="../imagens/graph.gif"
			onclick="window.open( '/<?= $diretorio ?>/gantt.php?atiid=<?= $atividade ? $atividade : $projeto ?>', '_blank', 'width=640,height=500,scrollbars=yes,scrolling=yes,resizebled=yes');"
			style="cursor: pointer;" title="exibir gr�fico" /></td>
	</tr>
</table>
<?php } else { ?>
<table style="width:100%; border:0; padding:5px;">
	<tr>
		<td style="width:80%; text-align:left;" valign="top">
		<?php if( true || projeto_verificar_responsabilidade( $projeto, $usuario ) || ( $atividade && atividade_verificar_responsabilidade( $atividade, $usuario ) ) ): ?>
		
		<?
		if($_REQUEST['atiid']) $sbaid = $db->pegaUm("SELECT sbaid FROM monitora.pi_subacaoatividade WHERE atiid='".$_REQUEST['atiid']."'");
		
		?>

		<?php endif; ?>
		&nbsp;&nbsp;
		<span
			title="importar atividade do Microsoft Project"
			onclick="exibir_importar_msproject();" style="cursor: pointer;">
		Importar </span> &nbsp;&nbsp;|&nbsp;&nbsp; <span
			title="exportar para o Microsoft Project"
			onclick="microsoft_project_exportar( false );"
			style="cursor: pointer;"> Exportar Filhos </span>
		&nbsp;&nbsp;|&nbsp;&nbsp; <span
			title="exportar para o Microsoft Project"
			onclick="microsoft_project_exportar( true );"
			style="cursor: pointer;"> Exportar Tudo </span>
			
			
		<div id="formulario_msproject" style="display: none;"><br />
		<form action="" method="post" name="microsoftproject"
			enctype="multipart/form-data"><input type="hidden"
			name="microsoftprojectacao" value="" /> <input type="hidden"
			name="microsoftprojectacaotime" value="" /> <input type="hidden"
			name="microsoftprojectincluipai" value="0" />
		<div id="botoes_msproject_importar" style="display: none;"><img
			align="absmiddle" src="/imagens/msproject.gif" /> Microsoft Project
		&nbsp;&nbsp;&nbsp; <input type="file" name="arquivo" /> <input
			type="button" name="importar" value="Importar"
			onclick="microsoft_project_importar();" /></div>
		</form>
		</div>
		<iframe name="microsoftprojectiframe" src="" style="display: none;"></iframe>


		</td>
		<td style="width:50%; text-align:right;" valign="top"><img
			align="absmiddle" src="../imagens/graph.gif"
			onclick="window.open( '/<?= $diretorio ?>/gantt.php?atiid=<?= $atividade ? $atividade : $projeto ?>', '_blank', 'width=640,height=500,scrollbars=yes,scrolling=yes,resizebled=yes');"
			style="cursor: pointer;" title="exibir gr�fico" /></td>
	</tr>
</table>
<?php
	} 
endif; 
?>

<!-- CORPO DA �RVORE -->
<style rel="stylesheet" type="text/css">
			
			.TitleClass {
				background-color: #ffffcc;
				border: 1px solid #707070;
				color: #252525;
				font-size: 11px;
				font-weight: normal;
				padding: 3px 5px 3px 5px;
			}
			
			.ativo * { xcursor: auto; }
			.ativo a { xcursor: pointer; }
			.inativo * { cursor: wait !important; }
			.inativo tr { background-color: #f8f8f8 !important; }
			.inativo img { cursor: wait !important; }
			.inativo a { cursor: wait !important; color: #909090; }
			.inativo a:hover { text-decoration: none; color: #909090; }
		</style>
<div id="arvore" class="ativo">
<table class="tabela" style="width:100%;" cellpadding="3">
	<tbody>
		<td
			style="text-align:center; cursor:wait; padding:15px; background-color:#fafafa; color:#404040; font-weight:bold; font-size: 10px;">
		<img align="absmiddle" src="../imagens/wait.gif" /> carregando</td>
	</tbody>
</table>
</div>
<script language="javascript" type="text/javascript">
			// for�a o carregamento inicial da �rvore
			recarregar_arvore();
		</script>

		<?php
		return ob_get_clean();
}

function arvore_biblioteca( $atividade, $profundidade = null, $situacao = null, $usuario = null  , $diretorio = null, $numeracao_relativa = false, $perfis = array() ) {
	$diretorio = (string) ( $diretorio ? $diretorio : $_SESSION['sisdiretorio'] );
	ob_start();
	?>
<!-- CONFIGURA��ES GERAIS -->
<script>
		
		var cima     = 'cima';
		var baixo    = 'baixo';
		var esquerda = 'esquerda';
		var direita  = 'direita';
		
		var URL_AJAX = 'http://<?= $_SERVER['SERVER_NAME'] ?>/<?= $diretorio ?>/ajaxUN.php'
			+ '?atividade=<?= $atividade ?>'
			+ '&profundidade=<?= $profundidade ?>'
			+ '<? foreach( $situacao as $item ) echo '&situacao['.$item.']='. $item; ?>'
			+ '&usuario=<?= $usuario ?>'
			+ '&numeracao_relativa=<?= (integer) $numeracao_relativa ?>'
			+ '<? foreach( $perfis as $item ) echo '&perfil['.$item.']='. $item; ?>';
		
		var requisicao_acao   = null;
		var requisicao_arvore = null;
		
		function bloquear_arvore(){
			var arvore = document.getElementById( 'arvore' );
			arvore.className = 'inativo';
			opacity( 'arvore', 100, 45, 500 );
		}
		
		function desbloquear_arvore(){
			var arvore = document.getElementById( 'arvore' );
			arvore.className = 'ativo';
			opacity( 'arvore', 45, 100, 500 );
		}
		
		function montar_arvore(){
			try {
				if ( requisicao_arvore.readyState == 4 ) {
					if ( requisicao_arvore.responseText != '' ) {
						var arvore = document.getElementById( 'arvore' );
						arvore.innerHTML = requisicao_arvore.responseText;
					}
					requisicao_arvore = null;
					desbloquear_arvore();
				}
			}
			catch( e ) {}
		}
		
		function recarregar_arvore(){
			requisicao_arvore = window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
			requisicao_arvore.open( 'GET', URL_AJAX + '&evento=arvore_recarregar', true );
			requisicao_arvore.onreadystatechange = montar_arvore;
			requisicao_arvore.send( null );
		}
		
		window.bloquear_arvore   = bloquear_arvore;
		window.recarregar_arvore = recarregar_arvore;
		
	</script>

<!-- MANIPULA AS A��ES -->
<script language="javascript" type="text/javascript">
		
		function capturar_resposta() {
			try {
				if ( requisicao_acao.readyState == 1 ) {
					bloquear_arvore();
				} else if ( requisicao_acao.readyState == 4 ) {
					if( requisicao_acao.responseText != '' ) {
						alert( requisicao_acao.responseText );
					}
					recarregar_arvore();
					requisicao_acao = null;
				}
			}
			catch( e ) {}
		}
		
		function cadastrar_atividade( atividade ){
			if ( requisicao_acao != null ) {
				return;
			}
			var titulo = window.prompt( 'T�tulo da atividade:', 'Nova Atividade' );
			if ( titulo ) {
				requisicao_acao = window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
				requisicao_acao.open( 'GET', URL_AJAX + '&evento=arvore_inserir&atiidpai=' + atividade + '&atidescricao=' + escape( titulo ), true );
				requisicao_acao.onreadystatechange = capturar_resposta;
				requisicao_acao.send( null );
			}
		}
		
		function excluir_atividade( atividade ){
			if ( requisicao_acao != null ) {
				return;
			}
			var link = document.getElementById( 'link' + atividade );
			if ( !link )
			{
				return;
			}
			titulo = link.innerHTML;
			if ( !confirm( 'Deseja excluir a atividade \''+ titulo +'\'?' ) ) {
				return;
			}
			requisicao_acao = window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
			requisicao_acao.open( 'GET', URL_AJAX + '&evento=arvore_excluir&atiid=' + atividade, true );
			requisicao_acao.onreadystatechange = capturar_resposta;
			requisicao_acao.send( null );
		}
		
		function mudar_ordem_atividade( origem, destino ){
			if ( requisicao_acao != null ) {
				return;
			}
			requisicao_acao = window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
			requisicao_acao.open( 'GET', URL_AJAX + '&evento=arvore_mudar_ordem&origem=' + origem + '&destino=' + destino, true );
			requisicao_acao.onreadystatechange = capturar_resposta;
			requisicao_acao.send( null );
		}
		
		function mudar_pai_atividade( atividade, pai ){
			if ( requisicao_acao != null ) {
				return;
			}
			requisicao_acao = window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
			requisicao_acao.open( 'GET', URL_AJAX + '&evento=arvore_mudar_pai&atiid=' + atividade + '&pai=' + pai, true );
			requisicao_acao.onreadystatechange = capturar_resposta;
			requisicao_acao.send( null );
		}
		
		function mudar_irma_atividade( atividade, irma ){
			if ( requisicao_acao != null ) {
				return;
			}
			requisicao_acao = window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
			requisicao_acao.open( 'GET', URL_AJAX + '&evento=arvore_mudar_irma&atiid=' + atividade + '&irma=' + irma, true );
			requisicao_acao.onreadystatechange = capturar_resposta;
			requisicao_acao.send( null );
		}
		
		function enviar_email( cpf ){
			var nome_janela = 'janela_enviar_emai_' + cpf;
			window.open(
				'/geral/envia_email.php?cpf=' + cpf,
				nome_janela,
				'width=650,height=557,scrollbars=yes,scrolling=yes,resizebled=yes'
			);
		}
		
	</script>

<!-- CONTROLA A��ES DO MICROSOFT PROJECT -->
<script language="javascript" type="text/javascript">
		
		function microsoft_project_limpar_formulario()
		{
			document.microsoftproject.reset();
		}
		window.microsoft_project_limpar_formulario = microsoft_project_limpar_formulario;
		
		function microsoft_project_importar()
		{
			document.microsoftproject.microsoftprojectacao.value = 'importar';
			document.microsoftproject.microsoftprojectacaotime.value = (new Date()) + '';
			if ( !document.microsoftproject.arquivo.value )
			{
				alert( 'Para importar � necess�rio enviar um arquivo.' );
				return;
			}
			document.microsoftproject.target = 'microsoftprojectiframe';
			microsoft_project_submeter();
		}
		
		function microsoft_project_exportar( incluirPai )
		{
			document.microsoftproject.microsoftprojectacao.value = 'exportar';
			document.microsoftproject.microsoftprojectincluipai.value = incluirPai ? '1' : '0';
			document.microsoftproject.microsoftprojectacaotime.value = (new Date()) + '';
			document.microsoftproject.target = 'microsoftprojectiframe';
			microsoft_project_submeter();
		}
		
		function microsoft_project_submeter()
		{
			var formulario = document.microsoftproject;
			formulario.action = '/<?= $diretorio ?>/microsoftproject.php?atiid=<?= $atividade ? $atividade : $projeto ?>';
			formulario.submit(); 
		}
		
		function exibir_importar_msproject()
		{
			var div_form = document.getElementById( 'formulario_msproject' );
			var div_importar = document.getElementById( 'botoes_msproject_importar' );
			//var div_exportar = document.getElementById( 'botoes_msproject_exportar' );
			if ( div_form.style.display == 'block' && div_importar.style.display == 'block' )
			{
				div_form.style.display = 'none';
				return;
			}
			div_form.style.display = 'block';
			div_importar.style.display = 'block';
			//div_exportar.style.display = 'none';
		}
		
	</script>

<!-- CONTROLA A EXIBI��O DOS ITENS -->
<script language="javascript" type="text/javascript">
		
		function mostrar_esconder_item( sNumero, sDisplay ){
			// captura linha e imagem de mostra/esconder
			var oRow   = document.getElementById( 'atividade_' + sNumero );
			var oImage = document.getElementById( 'imagem_' + sNumero );
			if ( !oRow ) {
				return false;
			}
			// mantem seus filhos escondidos
			if ( oImage && oImage.src.indexOf( 'menos' ) > -1 ) {
				linhas = oRow.parentNode.rows;
				for ( var i=0; linhas[i]; i++ ) {
					linha = linhas[i];
					if ( linha.getAttribute( 'id' ).indexOf( 'atividade_' + sNumero + '.' ) == 0 ) {
						if ( linha.getAttribute( 'id' ).substr( 10 + sNumero.length + 1 ).indexOf( '.' ) >= 0 ) {
							continue;
						}
						mostrar_esconder_item( linha.getAttribute( 'id' ).substr( 10 ), sDisplay );
					}
				}
			}
			// atualiza exibi��o ou ocultamento do item atual
			oRow.style.display = sDisplay;
			return true;
		}
		
		function mostrar_esconder( sNumero, iId ){
			// captura linha e imagem de mostra/esconder
			var oRow   = document.getElementById( 'atividade_' + sNumero );
			var oImage = document.getElementById( 'imagem_' + sNumero );
			if ( !oRow || !oImage ) {
				return false;
			}
			// verifica se � para exibir ou ocultar itens
			var sDisplay  = '';
			var sSrc      = '';
			if ( oImage.src.indexOf( 'menos' ) > 0 ) {
				sSrc     = '../imagens/mais.gif';
				sDisplay = 'none';
				oImage.parentNode.parentNode.className = 'linhaFechada';
			} else {
				sSrc     = '../imagens/menos.gif';
				sDisplay = document.all ? 'block' : 'table-row';
				oImage.parentNode.parentNode.className = 'linhaAberta';
			}
			// atualiza imagem
			oImage.src = sSrc;
			
			linhas = oRow.parentNode.rows;
			for ( var i=0; linhas[i]; i++ ) {
				var linha = linhas[i];
				if ( linha.getAttribute( 'id' ).indexOf( 'atividade_' + sNumero + '.' ) == 0 ) {
					if ( linha.getAttribute( 'id' ).substr( 10 + sNumero.length + 1 ).indexOf( '.' ) >= 0 ) {
						continue;
					}
					mostrar_esconder_item( linha.getAttribute( 'id' ).substr( 10 ), sDisplay );
				}
			}
			gravar_mostrar_esconder( sDisplay == 'none' ? 'arvore_ocultar' : 'arvore_exibir', iId );
			return true;
		}
		
		function gravar_mostrar_esconder( sOpcao, iId ){
			var teste = window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
			teste.open( 'GET', URL_AJAX + '&evento=' + sOpcao + '&atiid=' + iId, true );
			teste.send( null );
			teste = null;
		}
		
		/**
		 * 1. faz o input receber o innerHTML do elemento
		 * 2. seta o atributo parent do input com o id do elemento
		 * 3. mostra o calendario do input
		 */
		function montaCalendario( objSpan )
		{
			removeSlider();
			var objInputGeral = document.getElementById( 'inputGeral' );	
			if( trim( objSpan.innerHTML ) != '' )
			{
				objInputGeral.value = trim( objSpan.innerHTML );
			}
			else
			{
				objInputGeral.value = '';
			}
			objInputGeral.setAttribute( "parent" , objSpan.id );
			displayCalendar( objInputGeral ,'dd/mm/yyyy', objSpan.parentNode.getElementsByTagName( "td" )[4] )
		}
	
		/**
		 * 1. captura o id do elemento pelo atributo parent
		 * 2. captura o elemento parent pelo id
		 * 3. altera o innerHTML do parent pelo novo valor do calendario
		 * 4. envia o novo valor ao servidor
		 */
		function desmontaCalendario( objInputGeral )
		{
			if( !objInputGeral || objInputGeral.value == '' )
			{
				return;
			}
			var strSpanId = objInputGeral.getAttribute( "parent" );
			var objSpan = document.getElementById( strSpanId );
			var strDataAntiga = objSpan.innerHTML;
			
			if( strSpanId.indexOf( 'data_inicio_atividade_' ) == 0 )
			{
				var idAtividade = strSpanId.substr( 'data_inicio_atividade_'.length );
				strDataAlterada = 'atidatainicio';
			}
			else if( strSpanId.indexOf( 'data_fim_atividade_' ) == 0 )
			{
				var idAtividade = strSpanId.substr( 'data_fim_atividade_'.length );
				strDataAlterada = 'atidatafim';
			}
			else
			{
				var idAtividade = strSpanId.substr( 'data_conclusao_atividade_'.length );
				strDataAlterada = 'atidataconclusao';
			}
			
			objSpan.innerHTML = '<img align="absmiddle" src="../imagens/wait.gif"/>';
			alteraDataAtividade( idAtividade , strDataAlterada , objInputGeral.value , strDataAntiga);
		}
		
		function recebeConfirmacaoDataAlterada( idAtividade , strDataAlterada , strNovaData, strDataAntiga ){
			try {
				if ( requisicao_data.readyState == 1 ) {
				} else if ( requisicao_data.readyState == 4 ) {
					if( requisicao_data.responseText != '' ) {
						alert( requisicao_data.responseText );
						aposAlterarDataAtividade( idAtividade , strDataAlterada , strDataAntiga );
					} else {
						aposAlterarDataAtividade( idAtividade , strDataAlterada , strNovaData );
					}
					requisicao_data = null;
				}
			}
			catch( e ) {}
		}
		
		function alteraDataAtividade( idAtividade , strDataAlterada , strNovaData, strDataAntiga )
		{
			requisicao_data = window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
			requisicao_data.open( 'GET', URL_AJAX
				+ '&evento=arvore_alterar_atividade'
				+ '&atiid=' + idAtividade
				+ '&campo=' + strDataAlterada
				+ '&valor=' + strNovaData,
				true
			);
			requisicao_data.onreadystatechange = function retorno() { recebeConfirmacaoDataAlterada( idAtividade, strDataAlterada, strNovaData, strDataAntiga ); };
			requisicao_data.send( null );
		}
		
		function aposAlterarDataAtividade( idAtividade , strDataAlterada , strNovaData )
		{
			var objDate = strDateToObjDate( strNovaData , 'd/m/Y' , '/' );
			var objToday = new Date();
			
			switch( strDataAlterada )
			{
				case 'atidatainicio':
				{
					strSpanId = 'data_inicio_atividade_' + idAtividade;
					document.getElementById( strSpanId ).innerHTML = strNovaData;		
					break;
				}
				case 'atidataconclusao':
				{
					strSpanId = 'data_conclusao_atividade_' + idAtividade;
					document.getElementById( strSpanId ).innerHTML = strNovaData;		
					break;
				}
				case 'atidatafim':
				{
					strSpanId = 'data_fim_atividade_' + idAtividade;
					var objSpan = document.getElementById( strSpanId );
					if( objDate > objToday )
					{
						objSpan.style.color = 'green';
						objSpan.style.fontWeight = 'normal';
					}
					else
					{	
						objSpan.style.color = 'red';
						objSpan.style.fontWeight = 'bold';
					}
					
					objSpan.innerHTML = strNovaData;		
					break;
				}
			}
		}
		
	</script>
<input
	type="hidden" id="inputGeral" value="" readonly="readonly"
	onchange="desmontaCalendario( this )" />

<!-- MANIPULA ALTERA��O DA SITUA��O -->
<script>
		function arredonda( objInput )
		{
			if( objInput.value % 10 != 0 )objInput.value -= objInput.value % 10;
			
			var objSliderStatus = document.getElementById( 'situacaoSlider' );
					
			var intOriginalStatus = objSliderStatus.getAttribute( 'status' );
			
			switch( '' + objInput.value )
			{
				case '100':
				{
					objSliderStatus.value = 5;
					break;
				}
				case '0':
				{
					switch( '' + objSliderStatus.value )
					{
						case '5':
						{
							switch( intOriginalStatus )
							{
								case '5':
								{
									objSliderStatus.value = 2;
								}
								default:
								{
									objSliderStatus.value = intOriginalStatus;
									break;
								}
							}
							break;
						}
					}
					break;
				}
				default:
				{
					switch( '' + objSliderStatus.value )
					{
						case '5':
						case '1':
						{
							if( ( intOriginalStatus == 5 ) || ( intOriginalStatus == 1 ) )
							{ 
								objSliderStatus.value = 2;
							}
							else
							{
								objSliderStatus.value = intOriginalStatus;
							}
							break;
						}
						default:
						{
							break;
						}
					}
					break;
				}
			}
		}
		
		function alteraStatus( objSliderStatus )
		{
			var objSliderValor = document.getElementById( 'valorSlider' );
			 
			switch( '' + objSliderStatus.value )
			{
				case '1':
				{
					objSliderValor.value = 0;
					break;
				}
				case '2':
				case '3':
				case '4':
				{
					switch( '' + objSliderValor.value )
					{
						case '100':
						{
							objSliderValor.value = 90;
							break;
						}
						default:
						{
							break;
						}
					}
					break;
				}
				case '5':
				{
					objSliderValor.value = 100;
					break;
				}
				default:
				{
					break;
				}
			}
			
			objSliderValor.onchange();
		}
		
		function posicionaSlider( objSpan )
		{
			try
			{
				closeCalendar();
			}
			catch( e )
			{
			}
			
			var objSlider = document.getElementById( 'sliderDiv' );
			var objSliderValor = document.getElementById( 'valorSlider' );
			var objSliderStatus = document.getElementById( 'situacaoSlider' );
			
			var intValor		= objSpan.getAttribute( "percentual" );
			var intSelectValue	= objSpan.getAttribute( "status" );
			var strIdSpan		= objSpan.id;
			
			objSlider.style.position = "absolute";
			objSlider.style.left = getleftPos(objSpan) + 'px';
			objSlider.style.top = getTopPos(objSpan) + 'px';	
			objSlider.style.display = "block";
			
			objSliderValor.value = intValor;
			objSliderStatus.value = intSelectValue;
			objSliderStatus.setAttribute( "status" , intSelectValue );
			objSliderStatus.setAttribute( "id_tarefa" , strIdSpan );
			objSliderValor.onchange();
		}
		
		function removeSlider( )
		{
			var objSlider = document.getElementById( 'sliderDiv' );
			objSlider.style.display = "none";
		}
		
		function slicerSubmit()
		{
			var objSliderValor	= document.getElementById( 'valorSlider' );
			var objSliderStatus	= document.getElementById( 'situacaoSlider' );
			var strIdSpan		= objSliderStatus.getAttribute( 'id_tarefa' );
			var objSpan			= document.getElementById( strIdSpan );
			
			var strStatus		= document.getElementById( "situacaoSlider" ).options[ objSliderStatus.value - 1 ].innerHTML;
			var intPercentual	= objSliderValor.value;
			
			atualizaBarraStatus( strIdSpan , strStatus , objSliderStatus.value  , intPercentual )
			removeSlider();
		}
		
		function recebeConfirmacaoStatus( intBarraStatusId, strStatus, intStatus, intPercentual ){
			try {
				if ( requisicao_status.readyState == 1 ) {
				} else if ( requisicao_status.readyState == 4 ) {
					if( requisicao_status.responseText != '' ) {
						alert( requisicao_status.responseText );
					} else {
						var idAtividade = intBarraStatusId.replace( 'situacao_atividade_', '' );
						requisicao_execucao = window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
						requisicao_execucao.open( 'GET', URL_AJAX
							+ '&evento=arvore_alterar_atividade'
							+ '&atiid=' + idAtividade
							+ '&campo=atiporcentoexec'
							+ '&valor=' + intPercentual,
							true
						);
						requisicao_execucao.onreadystatechange = function retorno() { recebeConfirmacaoExecucao( intBarraStatusId, strStatus, intStatus, intPercentual ); };
						requisicao_execucao.send( null );
					}
					requisicao_status = null;
				}
			}
			catch( e ) {}
		}
		
		function recebeConfirmacaoExecucao( intBarraStatusId , strStatus , intStatus, intPercentual ){
			try {
				if ( requisicao_execucao.readyState == 1 ) {
				} else if ( requisicao_execucao.readyState == 4 ) {
					if( requisicao_execucao.responseText != '' ) {
						alert( requisicao_execucao.responseText );
					} else {
						aposAtualizarBarraStatus( intBarraStatusId , strStatus , intStatus, intPercentual );
					}
					requisicao_execucao = null;
				}
			}
			catch( e ) {}
		}
		
		var requisicao_status = null;
		var requisicao_execucao = null;
		
		function atualizaBarraStatus( intBarraStatusId , strStatus , intStatus, intPercentual )
		{
			var idAtividade = intBarraStatusId.replace( 'situacao_atividade_', '' );
			requisicao_status = window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
			requisicao_status.open( 'GET', URL_AJAX
				+ '&evento=arvore_alterar_atividade'
				+ '&atiid=' + idAtividade
				+ '&campo=esaid'
				+ '&valor=' + intStatus,
				true
			);
			requisicao_status.onreadystatechange = function retorno() { recebeConfirmacaoStatus( intBarraStatusId, strStatus, intStatus, intPercentual ); };
			requisicao_status.send( null );
		}
		
		function aposAtualizarBarraStatus( intBarraStatusId , strStatus , intStatus, intPercentual )
		{
			
			if( window.arrSituacoes == undefined )
			{
				var arrSituacoes 	= Array();
			
				var arrSituacao		= new Object();
				arrSituacao.status	= 'Cancelado';
				arrSituacao.texto	= '#aa2020';
				arrSituacao.barra	= '#cc3333';
				arrSituacao.sombra	= '#ffe7e7';
				arrSituacoes[<?= STATUS_CANCELADO  ?>] = arrSituacao;
				
				var arrSituacao		= new Object();
				arrSituacao.texto	= '#2020aa';
				arrSituacao.barra	= '#3333cc';
				arrSituacao.sombra	= '#d4e7ff';
				arrSituacoes[<?= STATUS_CONCLUIDO ?>] = arrSituacao;
				
				var arrSituacao		= new Object();
				arrSituacao.texto	= '#209020';
				arrSituacao.barra	= '#339933';
				arrSituacao.sombra	= '#dcffdc';
				arrSituacoes[<?= STATUS_EM_ANDAMENTO ?>] = arrSituacao;
				
				var arrSituacao		= new Object();
				arrSituacao.texto	= '#aa9020';
				arrSituacao.barra	= '#bba131';
				arrSituacao.sombra	= '#feffbf';
				arrSituacoes[<?= STATUS_SUSPENSO ?>] = arrSituacao;
				
				var arrSituacao		= new Object();
				arrSituacao.texto	= '#909090';
				arrSituacao.barra	= '#bbbbbb';
				arrSituacao.sombra	= '#efefef';
				arrSituacoes[<?= STATUS_NAO_INICIADO ?>] = arrSituacao;
	
				
				window.arrSituacoes = arrSituacoes;
			}
					
			arrSituacaoAtual = window.arrSituacoes[ intStatus ];
			
			var strNewSpanInnerHTML = '' +
			'<span style="color: '+ arrSituacaoAtual.texto + ';font-size: 10px;">' + strStatus + '</span>' +
			'<div style="text-align: left; margin-left: 5px; padding: 1px 0 1px 0; ' + 
			'height: 6px; max-height: 6px; width: 75px; border: 1px solid #888888; ' +
			'background-color: ' + arrSituacaoAtual.sombra  + ';" title="' + intPercentual + '%">' +
				'<div style="font-size:4px;width: ' + intPercentual + '%; height: 6px; max-height: 6px; background-color: ' + arrSituacaoAtual.barra + ';">' +
				'</div>' + 
			'</div>';
			
			var objSpan = document.getElementById( intBarraStatusId );
			
			objSpan.setAttribute( "status" , intStatus  );
			objSpan.setAttribute( "percentual" , intPercentual );
	
			objSpan.innerHTML = strNewSpanInnerHTML;
	
		} 
	</script>

	<?php
	return ob_get_clean();
}

function arvore_corpo( $lista, $diretorio = null, $subatividade = false, $numeracao_relativa = false ) {
	if ( empty( $lista ) ) {
		ob_start();
		?>
<table class="tabela" style="width:100%;" cellpadding="3">
	<tbody>
		<td
			style="text-align:center; padding:15px; background-color:#fafafa; color:#404040; font-weight:bold; font-size: 10px;">
		N�o h� atividades.</td>
	</tbody>
</table>
		<?php
		return ob_get_clean();
}

$diretorio = (string) ( $diretorio ? $diretorio : $_SESSION['sisdiretorio'] );

$numeracao_relativa = (boolean) $numeracao_relativa;

// calcula a profundidade inicial, base pro c�lculo da profundidade relativa
$profundidade_inicial = $lista[0]['profundidade'];

// inicializa a pilha de permiss�o
$pilha_permissao = array();
//$permissao_projeto = projeto_verificar_responsabilidade( PROJETO );
$permissao_projeto = true;

// identifica lista de itens ignorados na verifica��o de exibi��o
$numero = explode( '.', substr( $lista[0]['numero'], 0, strrpos( $lista[0]['numero'], '.' ) ) );
$ignorados = array();
for ( $i = count( $numero ); $i > 0; $i-- ) {
	array_push( $ignorados, implode( '.', array_slice( $numero, 0, $i ) ) );
}

ob_start();
?>
<table class="tabela" style="width:100%;" cellpadding="3">
<colgroup><col width="80"/><col width="50"/><col/><col width="80"/><col width="70"/><col width="70"/><col width="50" /></colgroup>
<thead>
<tr style="background-color: #e0e0e0">
<td style="font-weight:bold; text-align:center;">Comandos</td><td style="font-weight:bold; text-align:center;">N�vel</td>
<td style="font-weight:bold; text-align:center;">T�tulo</td><td style="font-weight:bold; text-align:center;">Situa��o</td>
<td style="font-weight:bold; text-align:center;">In�cio</td><td style="font-weight:bold; text-align:center;">T�rmino</td>
<td style="font-weight:bold; text-align:center;">Ordem</td>
</tr>
</thead>
<?php
global $db;
foreach( $lista as $indice => $atividade ) {
	
	// corrige o nome pra evitar problemas de codifica��o
	$atividade['atidescricao'] = str_replace( "'", "", str_replace( '"', "", $atividade['atidescricao'] ) );
	
	// verifica se tem filhos
	$profundidade = $atividade['profundidade'] - $profundidade_inicial;
	$profundidade_proximo = -1;
	if ( array_key_exists( (integer) $indice + 1, $lista ) ) {
		$profundidade_proximo = $lista[$indice + 1]['profundidade'] - $profundidade_inicial;
	}
	
	//Verifica se a Atividade tem PIs cadastrados
	$sql = "SELECT count(p.pliid)
		from monitora.pi_planointerno p
			inner join monitora.pi_planointernoatividade pa on pa.pliid = p.pliid
			inner join pde.atividade a on a.atiid = pa.atiid
		where 
			a.atiid = ".$atividade['atiid']." AND
			p.plistatus = 'A'";
	$pi_cadastrado = $db->pegaUm($sql);

	$tem_filhos           = $atividade['filhos'] > 0;
	$filho_esta_na_tabela = $tem_filhos && $profundidade < $profundidade_proximo;

	// verifica se deve ser exibido
	if ( $expandir ) {
		$visivel = true;
	} else {
		$visivel = $indice == 0 || arvore_verificar_exibicao_item( $atividade['numero'], $ignorados );
	}
	// verifica permiss�o em alguns n�veis
	if ( $permissao_projeto ) {
		$permissao     = true;
		$permissao_pai = true;
		$permissao_avo = true;
	} else {
		$permissao     = false;
		$permissao_pai = false;
		$permissao_avo = false;
		$pilha_permissao = array_slice( $pilha_permissao, 0, $profundidade );
		// permiss�o no pai e av�
		if ( in_array( true, array_slice( $pilha_permissao, 0, -1 ) ) ) {
			$permissao_pai = true;
			$permissao_avo = true;
		} else if( $pilha_permissao[$profundidade-1] ) {
			$permissao_pai = true;
		}
		// permiss�o no n�vel atual
		if( $permissao_pai ){
			$pilha_permissao[$profundidade] = true;
		} else {
			$pilha_permissao[$profundidade] = atividade_verificar_responsabilidade( $atividade['atiid'], $_SESSION['usucpf'] );
		}
		$permissao = $pilha_permissao[$profundidade];
		# quando � subatividade verifica a permiss�o no pai de forma diferente
		if ( $atividade['atiidpai'] != $atividade['_atiprojeto'] && $profundidade == 0 ) {
			$permissao_pai = atividade_verificar_responsabilidade( $atividade['atiidpai'], $_SESSION['usucpf'] );
		}
	}

	// seleciona a cor da linha
	$cor = $cor == '#fafafa' ? '#f0f0f0' : '#fafafa';

	// monta texto de descri��o
	if( $atividade['usucpf'] ) {
		$descricao = sprintf(
		'<br/>%s<br>%s<br>(%s) %s<br>',
		//'<b>%s</b><br/>%s<br>%s<br>(%s) %s<br>',
		//$atividade['atidescricao'],
		$atividade['usunome'],
		$atividade['usuemail'],
		$atividade['usufoneddd'],
		$atividade['usufonenum']
		);
	} else {
		//$descricao = sprintf( '<b>%s</b>', $atividade['atidescricao'] );
		$descricao= '';
	}

	$primeiro    = true;
	$id_anterior = null;
	for( $i=$indice-1; isset($lista[$i]); $i-- ) {
		if ( $lista[$i]['profundidade'] < $atividade['profundidade'] ) {
			break;
		}
		if ( $lista[$i]['profundidade'] == $atividade['profundidade'] ) {
			$primeiro    = false;
			$id_anterior = $lista[$i]['atiid'];
			break;
		}
	}

	$ultimo     = true;
	$id_proximo = null;
	for( $i=$indice+1; isset($lista[$i]); $i++ ) {
		if ( $lista[$i]['profundidade'] == $atividade['profundidade'] ) {
			$ultimo     = false;
			$id_proximo = $lista[$i]['atiid'];
			break;
		}
	}
	
	
$status = $db->pegaUm("SELECT sub.sbasituacao FROM monitora.pi_subacaoatividade sba
				  	   LEFT JOIN monitora.pi_subacao sub ON sub.sbaid=sba.sbaid  
			 	  	   WHERE atiid = '".$atividade['atiid']."' AND sbaatividade=true");

if(!$status) {
	
	$sb = $db->pegaUm("SELECT sba.sbaid FROM monitora.pi_subacaoatividade sba
				 	  	   WHERE atiid = '".$atividade['atiid']."' AND sbaatividade=false");
	
	if($sb) $status = 'Z';
	else $status = 'X';
	
}
//ver($atividade['filhos']);
# Comentando a pedido do Henrique, sem sabe as regras que o Dourado fez.
/*if($atividade['filhos'] > 0) {
	$parametrosubatv='&opc=cri&opcnone=true';
} else {
	$parametrosubatv='&opc=vin';
}*/

$sbaid = $db->pegaUm("SELECT sbaid FROM monitora.pi_subacaoatividade WHERE atiid='".$atividade['atiidpai']."'");

if($sbaid) {
	$status = 'K';
}

$pliid = $db->pegaUm("SELECT pliid FROM monitora.pi_planointernoatividade WHERE atiid='".$atividade['atiid']."'");

$boPerfilSomenteLeitura = boPerfilSomenteLeitura();
arConfiguracoesPerfis($arPerfilUnidadeOrcamento, $arPerfilUnidadePlanejamento);

?><tr id="atividade_<?= $atividade['numero'] ?>" style="background-color: <?= $cor ?>;<?= $visivel ? '' : 'display:none' ?>;" onmouseout="this.style.backgroundColor='<?= $cor ?>';" onmouseover="this.style.backgroundColor='#ffffcc';">
<td style="text-align:center;" nowrap>
<?php if(!$boPerfilSomenteLeitura){ ?>
<a href="?modulo=principal/planotrabalho/atividade&acao=A&atiid=<?= $atividade['atiid'] ?>"><img align="absmiddle" src="../imagens/alterar.gif" style="border: 0;" title="Informa��es Gerais"/></a>
<?php } else {?>
<img align="absmiddle" src="../imagens/alterar_01.gif" style="border: 0;" title="Informa��es Gerais"/>
<?php } ?>
<?php /*echo $status;*/ ?>
<?php if( (!$boPerfilSomenteLeitura) && ($permissao && ($status=='Z' || $status=='A' || $status=='X' || $status=='K' || $status=='P' || !$status)) ): ?><img align="absmiddle" border="0" src="../imagens/gif_inclui.gif" onclick="cadastrar_atividade( <?= $atividade['atiid'] ?> )" title="Cadastrar Subtividade" style="cursor:pointer;"/><?php else: ?><img align="absmiddle" src="../imagens/gif_inclui_d.gif"/><?php endif; ?> 
<?php if( (!$boPerfilSomenteLeitura) && ($permissao && !$tem_filhos && ($pi_cadastrado == 0))): ?><img align="absmiddle" border="0" src="../imagens/excluir.gif" onclick="excluir_atividade( <?= $atividade['atiid'] ?> )" title="Excluir Atividade" style="cursor:pointer;"/><?php else : ?><img align="absmiddle" src="../imagens/excluir_01.gif"/><?php endif; ?><a href="/<?= $diretorio ?>/gantt.php?atiid=<?= $atividade['atiid'] ?>" target="_blank"><img align="absmiddle" border="0" src="../imagens/lupa_grafico.gif" title="Exibir Gr�fico" style="cursor:pointer; margin-left:5px;" /></a>
<?php
$boNaoVePlanoInterno = boNaoVePlanoInterno();
//ver($status);
switch($status) {
	case 'K':
		?>
		<a style="cursor:pointer;" onclick="janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/vincula_subacaoatividadeUN&acao=A&atiid=<?= $atividade['atiid'] ?>', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=400' ); janela.focus();"><img src="../imagens/money.gif" border="0" align="top" title="Plano Interno"/></a>
		<?
		break;
	case 'Z':
		?>
			<?php
			if($db->testa_superuser()){
			?>
				<a style="cursor:pointer;" onclick="janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/vincula_subacaoatividadeUN&acao=A&atiid=<?= $atividade['atiid'] ?>', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=400' ); janela.focus();"><img src="../imagens/money.gif" border="0" align="top" title="Plano Interno"/></a>
				<a style="cursor:pointer;" onclick="janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/listarsubacoesatividadeUN&acao=A&atiid=<?= $atividade['atiid'] ?>&opc=vin&opcnone=true', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=400' ); janela.focus();"><img src="../imagens/valida4.gif" border="0" align="top" title="Inserir suba��o"/></a>
			<?php 
			} elseif( possui_perfil($arPerfilUnidadeOrcamento) && !possui_perfil($arPerfilUnidadePlanejamento)) {
				if(!$boPerfilSomenteLeitura): ?>
					<a style="cursor:pointer;" onclick="janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/vincula_subacaoatividadeUN&acao=A&atiid=<?= $atividade['atiid'] ?>', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=400' ); janela.focus();"><img src="../imagens/money.gif" border="0" align="top" title="Plano Interno"/></a>
					<a style="cursor:pointer;" onclick="janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/listarsubacoesatividadeUN&acao=A&atiid=<?= $atividade['atiid'] ?>&opc=vin&opcnone=true', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=400' ); janela.focus();"><img src="../imagens/valida4.gif" border="0" align="top" title="Inserir suba��o"/></a>
				<?php else: ?>
					<a style="cursor:pointer;" onclick="janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/vincula_subacaoatividadeUN&acao=A&atiid=<?= $atividade['atiid'] ?>', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=400' ); janela.focus();"><img src="../imagens/money.gif" border="0" align="top" title="Plano Interno"/></a>
				<?php endif; 
			} else {
				if(!$boNaoVePlanoInterno): ?>
					<a style="cursor:pointer;" onclick="janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/vincula_subacaoatividadeUN&acao=A&atiid=<?= $atividade['atiid'] ?>', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=400' ); janela.focus();"><img src="../imagens/money.gif" border="0" align="top" title="Plano Interno"/></a>
					<a style="cursor:pointer;" onclick="janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/listarsubacoesatividadeUN&acao=A&atiid=<?= $atividade['atiid'] ?>&opc=vin&opcnone=true', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=400' ); janela.focus();"><img src="../imagens/valida4.gif" border="0" align="top" title="Inserir suba��o"/></a>
				<?php else: ?>
					<img src="../imagens/money_01.gif" border="0" align="top" title="Plano Interno"/>
					<a style="cursor:pointer;" onclick="janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/listarsubacoesatividadeUN&acao=A&atiid=<?= $atividade['atiid'] ?>&opc=vin&opcnone=true', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=400' ); janela.focus();"><img src="../imagens/valida4.gif" border="0" align="top" title="Inserir suba��o"/></a>
				<?php endif; 
			}
			?>
		<?
		break;
	case 'A':
		?>
			<?php
			if($db->testa_superuser()){
			?>
				<a style="cursor:pointer;" onclick="janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/vincula_subacaoatividadeUN&acao=A&atiid=<?= $atividade['atiid'] ?>', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=400' ); janela.focus();"><img src="../imagens/money.gif" border="0" align="top" title="Plano Interno"/></a>
				<a style="cursor:pointer;" onclick="janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/listarsubacoesatividadeUN&acao=A&atiid=<?= $atividade['atiid'] ?>&opc=vin&opcnone=true', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=400' ); janela.focus();"><img src="../imagens/valida4.gif" border="0" align="top" title="Inserir suba��o"/></a>
			<?php 
			} elseif( possui_perfil($arPerfilUnidadeOrcamento) && !possui_perfil($arPerfilUnidadePlanejamento)) {
				if(!$boPerfilSomenteLeitura): ?>
					<a style="cursor:pointer;" onclick="janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/vincula_subacaoatividadeUN&acao=A&atiid=<?= $atividade['atiid'] ?>', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=400' ); janela.focus();"><img src="../imagens/money.gif" border="0" align="top" title="Plano Interno"/></a>
					<a style="cursor:pointer;" onclick="janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/listarsubacoesatividadeUN&acao=A&atiid=<?= $atividade['atiid'] ?>&opc=vin&opcnone=true', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=400' ); janela.focus();"><img src="../imagens/valida4.gif" border="0" align="top" title="Inserir suba��o"/></a>
				<?php else: ?>
					<a style="cursor:pointer;" onclick="janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/vincula_subacaoatividadeUN&acao=A&atiid=<?= $atividade['atiid'] ?>', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=400' ); janela.focus();"><img src="../imagens/money.gif" border="0" align="top" title="Plano Interno"/></a>
				<?php endif; 
			} elseif( !possui_perfil($arPerfilUnidadeOrcamento) && possui_perfil($arPerfilUnidadePlanejamento)) {
				if(!$boNaoVePlanoInterno): ?>
					<a style="cursor:pointer;" onclick="janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/vincula_subacaoatividadeUN&acao=A&atiid=<?= $atividade['atiid'] ?>', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=400' ); janela.focus();"><img src="../imagens/money.gif" border="0" align="top" title="Plano Interno"/></a>
					<img src="../imagens/valida1.gif" border="0" align="top" title="Inserir suba��o"/>
				<?php else: ?>
					<img src="../imagens/money_01.gif" border="0" align="top" title="Plano Interno"/>
					<img src="../imagens/valida1.gif" border="0" align="top" title="Inserir suba��o"/>
				<?php endif; 
			}
			?>
		<?
		break;
	case 'P':
		?>
		<img src="../imagens/valida5.gif" border="0" align="top" title="Suba��o em aprova��o"/>
		<?
		break;
	case 'X':
		?>
		<?php if($db->testa_superuser()) { ?>
				<a style="cursor:pointer;" onclick="janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/listarsubacoesatividadeUN&acao=A&atiid=<?= $atividade['atiid'] ?><?= $parametrosubatv ?>', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=400' ); janela.focus();"><img src="../imagens/valida2.gif" border="0" align="top" title="Inserir suba��o"/></a>
		<?php } else if( possui_perfil($arPerfilUnidadeOrcamento) ){ 
					if(!$boPerfilSomenteLeitura){ ?>
						<a style="cursor:pointer;" onclick="janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/listarsubacoesatividadeUN&acao=A&atiid=<?= $atividade['atiid'] ?><?= $parametrosubatv ?>', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=400' ); janela.focus();"><img src="../imagens/valida2.gif" border="0" align="top" title="Inserir suba��o"/></a>
					<?php
					}
					?>
		<?php }
		
		break;
}

?>
</td>
<td style="text-align:center;"><?php
//ver($atividade);
//ver(usuario_possui_perfil( PERFIL_ASSESSOR ));
if( $permissao_avo && $profundidade > 0 ): ?><img align="absmiddle" onclick="mudar_irma_atividade( <?= $atividade['atiid'] ?>, <?= $atividade['atiidpai'] ?> );" title="Recuar" src="../imagens/recuo_e.gif" style="cursor:pointer;"/><?php else : ?><img align="absmiddle" src="../imagens/recuo_e_d.gif"/><?php endif; ?> 
<?php if( $permissao_pai && !$primeiro ): ?><img align="absmiddle" onclick="mudar_pai_atividade( <?= $atividade['atiid'] ?>, <?= $id_anterior ?> );" title="Avan�ar" src="../imagens/recuo_d.gif" style="cursor:pointer;" /><?php else : ?><img align="absmiddle" src="../imagens/recuo_d_d.gif"/><?php endif; ?>
</td>
<td style="padding-left: <?= $profundidade * 20 ?>px; <?= $tem_filhos ? 'font-weight:bold;' : '' ?>">
<div style=" overflow:hidden; width:100%; height:13px;" scroll="no"><?php
if( $profundidade > 0 ): ?><img align="absmiddle" src="../imagens/seta_filho.gif" /><?php endif; ?><?php
if( $filho_esta_na_tabela && !$expandir ): ?> <img src="../imagens/<?= arvore_verificar_exibicao_filhos( $atividade['numero'] ) ? 'mais' : 'menos' ?>.gif" id="imagem_<?= $atividade['numero'] ?>" onclick="mostrar_esconder( '<?= $atividade['numero'] ?>', '<?= $atividade['atiid'] ?>' );" style="cursor:pointer;" /><?php endif; ?><?php
if( $atividade['qtdrestricoes'] > 0 ): ?> <a href="?modulo=principal/planotrabalhoUN/controle&acao=A&atiid=<?= $atividade['atiid'] ?>"><img src="../imagens/restricao.png" border="0" align="absmiddle" title="<?= $atividade['qtdrestricoes'] ?> <?= $atividade['qtdrestricoes'] == 1 ? 'restri��o' : 'restri��es' ?>"/></a><?php endif; ?><?php
if( $atividade['qtdanexos'] > 0 ): ?> <a href="?modulo=principal/planotrabalhoUN/instrumento&acao=A&atiid=<?= $atividade['atiid'] ?>"><img src="../imagens/clipe.gif" border="0" align="absmiddle" title="<?= $atividade['qtdanexos'] ?> <?= $atividade['qtdanexos'] == 1 ? 'anexo' : 'anexos' ?>"/></a><?php endif;
if( $pliid ): ?><a href="?modulo=principal/planotrabalhoUN/pesquisa_piUN&acao=A&boPiTitulo=1&atiid=<?= $atividade['atiid'] ?>"><img src="../imagens/cifrao.jpg" border="0" align="absmiddle" title="<?php echo $db->pegaUm("SELECT count(1) as count FROM monitora.pi_planointerno p INNER JOIN monitora.pi_planointernoatividade pia ON p.pliid = pia.pliid WHERE p.plistatus = 'A' AND pia.atiid = {$atividade['atiid']}"); ?> PI(s)"/></a><?php endif;
if( ($atividade['graid'] == 1) && (usuario_possui_perfil( PERFIL_ASSESSOR ))): ?> <img src="../imagens/star.gif" border="0" align="top" title="Estrat�gica"/><?php endif; ?>
<span style="margin: 0 5px 0 0; <?= $permissao ? '' : 'color:#909090;' ?>"><?php
	$numero = $atividade['numero'];
	if ( $numeracao_relativa = true ) {
		$numero = implode( ".", array_slice( explode( ".", $numero ), 1 ) );
	}
	?> <?= $numero ?></span><a id="link<?= $atividade['atiid'] ?>" href="?modulo=principal/planotrabalhoUN/subatividadesUN&acao=A&atiid=<?= $atividade['atiid'] ?>" onmouseover="window.SuperTitleOn( this , '<b>' + this.innerHTML + '</b><?= $descricao ?>' )" onmouseout="window.SuperTitleOff( this )" style="<?= (!$boPerfilSomenteLeitura) && ($permissao)  ? '' : 'color:#909090;' ?>"><?= $atividade['atidescricao'] ?></a>
</div>
<?php if( $_SESSION["sisid"] == 10 && ( $atividade["_atiprofundidade"] == 1 || $atividade["_atiprofundidade"] == 2 ) ): ?>
<td>&nbsp;</td>
<td>&nbsp;</td>
<td>&nbsp;</td>
<?php else: ?>
</td>
<?php $onclick = $permissao ? 'onclick="posicionaSlider( this );"' : ''; ?>
<td <?= $onclick ?> id="situacao_atividade_<?= $atividade['atiid'] ?>" percentual="<?= (integer) $atividade[ 'atiporcentoexec' ]?>" status="<?= (integer) $atividade['esaid'] ?>" style="text-align:center;">
<?= montar_barra_execucao( $atividade, $permissao ) ?>
</td>
<?php $onclick = $permissao ? 'onclick="montaCalendario( this );"' : ''; ?>
<td <?= $onclick ?> <?= $permissao ? 'title="Alterar data de in�cio"' : '' ?> id="data_inicio_atividade_<?= $atividade['atiid'] ?>" style="text-align:center; color: <?= $permissao ? '#008000' : '#909090' ?>;">
<?= formata_data( $atividade['atidatainicio'] ) ?>
</td>
<?php if( $atividade['esaid'] == STATUS_CONCLUIDO ): ?>
<td <?= $onclick ?> <?= $permissao ? 'title="Alterar data de t�rmino"' : '' ?> id="data_conclusao_atividade_<?= $atividade['atiid'] ?>" style="text-align:center; color: #103090;">
<?= formata_data( $atividade['atidataconclusao'] ) ?>
</td><?php
else:
?><td <?= $onclick ?> id="data_fim_atividade_<?= $atividade['atiid'] ?>" style="text-align:center; <?= $permissao ? ( strtotime( $atividade['atidatafim'] ) < time() ? 'color:#ff2020;font-weight:bold;' : 'color:#008000;' ) : 'color:#909090;' ?>;">
<?=
formata_data( $atividade['atidatafim'] )
?></td>
<?php endif; ?>
<?php endif; ?>
<td style="text-align:center;"><?php
if( $permissao_pai && !$primeiro ):
?><img align="absmiddle" onclick="mudar_ordem_atividade( '<?= $atividade['atiid'] ?>', '<?= $id_anterior ?>' );" title="Mover para cima" src="../imagens/seta_cima.gif" style="cursor:pointer; border:0;"/><?php else: ?><img align="absmiddle" src="../imagens/seta_cimad.gif"/><?php endif; ?> 
<?php if( $permissao_pai && !$ultimo ): ?><img align="absmiddle" onclick="mudar_ordem_atividade( '<?= $atividade['atiid'] ?>', '<?= $id_proximo ?>' );" title="Mover para baixo" src="../imagens/seta_baixo.gif" style="cursor:pointer; border:0;"/><?php else: ?><img align="absmiddle" src="../imagens/seta_baixod.gif"/><?php
endif;
?></td></tr><?php } ?>
</table>
	<?php
	return ob_get_clean();
}

/**
 * Monta e retorna um um formul�rio para consulta de atividades.
 *
 * @return string
 */
function montar_formulario_pesquisa( $foco = false ){

	$formulario = sprintf(
	'<form action="/%s/%s.php?modulo=principal/atividade_/pesquisa&acao=A" method="post" name="pesquisa">
			<div style="width:100%%; text-align: right; margin-bottom: 5px; font-size: 90%%;">
				<input type="text" name="filtro" value="%s" class="CampoEstilo" onblur="MouseBlur( this );" onmouseout="MouseOut( this );" onfocus="MouseClick( this );" onmouseover="MouseOver( this );"/>
				&nbsp;
				<input type="button" name="botao" value="Pesquisar" onclick="pesquisar();"/>
				<div style="margin-top: 6px;">
					<label for="buscar_por_descricao">pesquisar tamb�m nas descri��es</label>
					<input %s type="checkbox" name="buscar_por_descricao" value="1" id="buscar_por_descricao"/>
				</div>
			</div>
		</form>
		<script language="javascript" type="text/javascript">
			function pesquisar(){
				if ( document.pesquisa.filtro.value != "" ) {
					document.pesquisa.submit();
				}
				document.pesquisa.filtro.focus();
			}
		</script>',
	$_SESSION['sisdiretorio'],
	$_SESSION['sisarquivo'],
	$_REQUEST['filtro'],
	$_REQUEST['buscar_por_descricao'] ? 'checked="checked"' : ''
	);
	if ( $foco ) {
		$formulario .=
		'<script language="javascript" type="text/javascript">
				document.pesquisa.filtro.focus();
			</script>';
	}
	return $formulario;
}

/**
 * @return string
 */
function montar_rastro_atividade( $numero, $destacar = true, $numeracao_relativa = false, $retira_link = false ){
	
	$lista = atividade_pegar_rastro( $numero );
	$rastro = array();
	foreach ( $lista as $indice => $item ) {
		if ( $numeracao_relativa = true ) {
			$item['numero'] = implode( ".", array_slice( explode( ".", $item['numero'] ), 1 ) );
		}
		
		if ( count( $lista ) == $indice + 1 ) {
			$htm = sprintf(
			'<p style="margin: 0 0 5px %dpx;font-weight:bold;font-size:120%%;">%s %s %s</a></p>',
			$indice * 20,
			$indice != 0 ? '<img src="../imagens/seta_filho.gif" align="absmiddle" border="0">&nbsp;' : '',
			$item['numero'],
			$item['atidescricao']
			);
		} else {
			if($retira_link) {
				$htm = sprintf(
				'<p style="margin: 0 0 5px %dpx;">%s %s %s</p>',
				$indice * 20,
				$indice != 0 ? '<img src="../imagens/seta_filho.gif" align="absmiddle" border="0">&nbsp;' : '',
				$item['numero'],
				$item['atidescricao']
				);
			} else { 
				$htm = sprintf(
				'<p style="margin: 0 0 5px %dpx;">%s<a href="?modulo=%s&acao=%s&atiid=%d">%s %s</a></p>',
				$indice * 20,
				$indice != 0 ? '<img src="../imagens/seta_filho.gif" align="absmiddle" border="0">&nbsp;' : '',
				$_REQUEST['modulo'],
				$_REQUEST['acao'],
				$item['atiid'],
				$item['numero'],
				$item['atidescricao']
				);
			}
		}
		array_push( $rastro, $htm );
	}
	return sprintf( '<div style="margin: 5px">%s</div>', implode( '', $rastro ) );
}

/**
 * Monta e retorna um resumo em html da indicadas indicada.
 *
 * @return string
 */
function montar_resumo_atividade( $atividade, $numeracao_relativa = false, $retira_link = false ) {
	return "<table border=\"0\" cellpading=\"0\" cellspacing=\"0\" width=\"100%\">
			<tr>
				<td valign=\"top\">" . montar_rastro_atividade( $atividade['numero'], $destacar = true, $numeracao_relativa, $retira_link ) . "</td>
			</tr>
			</table>
			<hr size=\"1\" noshade=\"noshade\" color=\"#dddddd\" style=\"margin:15px 0 15px 0;\"/>";
}

function montar_barra_execucao( $atividade, $cor = true ){
	if ( !$cor ) {
		$atividade['esaid'] = STATUS_NAO_INICIADO;
	}
	switch ( $atividade['esaid'] ) {
		case STATUS_CANCELADO:
			$cor_texto = '#aa2020';
			$cor_barra = '#cc3333';
			$cor_sombra = '#ffe7e7';
			break;
		case STATUS_CONCLUIDO:
			$cor_texto = '#2020aa';
			$cor_barra = '#3333cc';
			$cor_sombra = '#d4e7ff';
			break;
		case STATUS_EM_ANDAMENTO:
			$cor_texto = '#209020';
			$cor_barra = '#339933';
			$cor_sombra = '#dcffdc';
			break;
		case STATUS_SUSPENSO:
			$cor_texto = '#aa9020';
			$cor_barra = '#bba131';
			$cor_sombra = '#feffbf';
			break;
		default:
		case STATUS_NAO_INICIADO:
			$cor_texto = '#909090';
			$cor_barra = '#bbbbbb';
			$cor_sombra = '#efefef';
			break;
	}
	return sprintf(
	'<span style="color: %s;font-size: 10px;">%s</span>' .
	'<div style="text-align: left; margin-left: 5px; padding: 1px 0 1px 0; height: 6px; max-height: 6px; width: 75px; border: 1px solid #888888; background-color: %s;" title="%d%%">' .
	'<div style="font-size:4px;width: %d%%; height: 6px; max-height: 6px; background-color: %s;">' .
	'</div>'.
	'</div>',
	$cor_texto,
	$atividade['esadescricao'],
	$cor_sombra,
	$atividade['atiporcentoexec'],
	$atividade['atiporcentoexec'],
	$cor_barra
	);
}

function montar_titulo_projeto( $atividade = '' ){
	global $titulo_modulo;
	global $db;
	// pega o projeto
	$sql = sprintf( "select atiid, atidescricao from pde.atividade where atiid = %d", $_SESSION['projeto'] );
	$projeto = $db->pegaLinha( $sql );
	// monta t�tulo e subt�tulo
	if ( $_SESSION["sisid"] == 1 ) {
		$titulo = $projeto['atidescricao'];
	} else{
		$titulo = sprintf( '<a href="?modulo=principal/projeto&acao=A&atiid=%d">%s</a>', $projeto['atiid'], $projeto['atidescricao'] );
	}
	$subtitulo = $atividade ? $atividade : $titulo_modulo;
	// exibe o cabe�alho
	monta_titulo( $titulo, $subtitulo );
}

?>