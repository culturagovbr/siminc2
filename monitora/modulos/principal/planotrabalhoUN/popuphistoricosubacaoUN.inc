<?php
require_once APPRAIZ . "monitora/classes/Pi_Subacao.class.inc";
require_once APPRAIZ . "monitora/classes/Pi_SubacaoHistorico.class.inc";

if($_REQUEST['requisicao'] == 'vincular'){
	
	extract($_POST);
	
	$retorno = false;
	$pi_SubacaoHistorico = new Pi_SubacaoHistorico();
	$pi_SubacaoHistorico->sbaid = $sbaid;
	$pi_SubacaoHistorico->usucpf = $_SESSION['usucpf'];
	$pi_SubacaoHistorico->sahsituacao = $situacao;
	if($sahobs){
		$pi_SubacaoHistorico->sahobs	= $sahobs;		
	}
	$pi_SubacaoHistorico->salvar();
	
	$pi_Subacao = new Pi_Subacao($sbaid);
	$pi_Subacao->sbasituacao = $situacao;
	$pi_Subacao->salvar();
	
	if($pi_SubacaoHistorico->commit()){
		$retorno = true;
	}
	unset($pi_Subacao);
	unset($pi_SubacaoHistorico);
	
	echo $retorno;
	die;
	
}

extract($_GET);

$unicod = $db->pegaUm("select unicod from monitora.pi_subacaounidade where sbaid = $sbaid");

$obPi_Subacao = new Pi_Subacao($sbaid);

if($obPi_Subacao->sbaid){
	$sql_lista = "SELECT
					dtl.ptrid,
					dtl.ptres,
					trim(aca.prgcod||'.'||aca.acacod||'.'||aca.unicod||'.'||aca.loccod||' - '||aca.acadsc) as descricao,
					uni.unidsc,
					SUM(dtl.ptrdotacao) as dotacaoinicial,
					coalesce(cast(SUM(dt.valor) as varchar),'0.00') as detalhamento,
					sum(sad.sadvalor) as valorcadastrado
				FROM planejamento.acao aca
					INNER JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
					INNER JOIN public.unidade uni ON uni.unicod = dtl.unicod
					INNER JOIN monitora.pi_subacaodotacao sad ON dtl.ptrid = sad.ptrid
					LEFT JOIN ( select ptrid,
								sum( sadvalor ) as valor
								from  monitora.pi_subacaodotacao
								group by ptrid ) dt ON dtl.ptrid = dt.ptrid
					WHERE aca.prgano = '".$_SESSION['exercicio']."' 
						  and aca.unicod = '".$unicod."'
					      and aca.acasnrap = false
					      and sad.sbaid = ".$obPi_Subacao->sbaid."
					GROUP BY dtl.ptrid, dtl.ptres,
								trim(aca.prgcod||'.'||aca.acacod||'.'||aca.unicod||'.'||aca.loccod||' - '||aca.acadsc) ,
								uni.unidsc";
	$ptresacao = $db->carregar($sql_lista);
	//ver($sql_lista);
}
$ptresacao = ($ptresacao) ? $ptresacao : array();

?>
<html>
  <head>
    <script type="text/javascript" src="/includes/prototype.js"></script>
    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
  </head>
<br>    
<form name="formulario" id="formulario" method="post">
<table  bgcolor="#f5f5f5" align="center" class="tabela" >
	<tr>
		<td class = "subtitulodireita" colspan="2">
			<center> 
			<h3>Cadastro Subação</h3>
			</center>
		</td> 
	</tr>
	<tr>
		<td align="center" colspan="2">
			<?php echo $unidade = $db->pegaUm("SELECT unicod || ' - ' || unidsc FROM public.unidade WHERE unicod = '".$unicod."'"); ?>
		</td>
	</tr>
	<tr>
		<td width="250px" class = "subtitulodireita">
			Código:
		</td>
		<td>
			<?php echo $obPi_Subacao->sbacod; ?>
		</td>
	</tr>
	<tr>
		<td class = "subtitulodireita">
			Título:
		</td>
		<td>
			<?php echo $obPi_Subacao->sbatitulo; ?>
		</td>
	</tr>
	<tr>
		<td class = "subtitulodireita">
			Descrição / Funcionalidade:
		</td>
		<td>
			<?php echo $obPi_Subacao->sbadsc; ?>
		</td>
	</tr>
	<tr>
		<td colspan="2">
			<table cellpadding="0" border="0" width="98%"  id="orcamento"  style="BORDER-RIGHT: #C9C9C9 1px solid; BORDER-TOP: #C9C9C9 1px solid; BORDER-LEFT: #C9C9C9 1px solid; BORDER-BOTTOM: #C9C9C9 1px solid;">
				<tr>
				<td style="background-color: #C9C9C9;" colspan="9" align="center"><b>Detalhamento Orçamentário</b></td>
				</tr>
		        <tr>
					<td style="background-color: #C9C9C9;" align="center" nowrap><b>PTRES</b><input type="hidden" name="ptrid"></td>
					<td style="background-color: #C9C9C9; width:45%;" align="center" nowrap><b>Ação</b></td>
				    <td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Dotação Autorizada</b></td>
				    <td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Detalhado na SubAção</b></td>
				    <td style="background-color: #C9C9C9; width:100px;" align="center"><b>Saldo Disponível</b></td>
				    <td style="background-color: #C9C9C9;" align="center"><b>Valor Previsto(Anual)</b></td>
				</tr>
				<? 
				if($ptresacao[0]) {
					$valortotal = 0;
					$cor = 0;
					foreach($ptresacao as $acpl) { 
				?>
		        <tr style="height:30px;<? echo (($cor%2)?"":"background-color:#DCDCDC;"); ?>" id="ptrid_<? echo $acpl['ptrid']; ?>">
					<td align="center"><? echo $acpl['ptres']; ?></td>
					<td align="left"><? echo $acpl['descricao']; ?><input type='hidden' name='ptrid[<? echo $acpl['ptrid']; ?>]' value='<? echo $acpl['ptrid']; ?>'></td>
				    <td align="right"><? echo number_format($acpl['dotacaoinicial'],2,',','.'); ?></td>
				    <td align="right"><? echo number_format($acpl['detalhamento'],2,',','.'); ?></td>
				    <td align="right"><? echo number_format(($acpl['dotacaoinicial']-$acpl['detalhamento']),2,',','.'); ?></td>
				    <td align="center"><input type="text" name="valor[<? echo $acpl['ptrid']; ?>]" size="28" maxlength="" value="<? echo number_format($acpl['valorcadastrado'],2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);calculovalorPI();" disabled  class="disabled" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this); verificaDisponivel(this,'<?=$acpl['ptrid']?>','<?=number_format($acpl['valorcadastrado'],2,',','.')?>');" style="text-align : right; width:25ex;" title='' /></td>
				</tr>
				<? 
						$cor++;
						$valortotal = $valortotal + $acpl['valorcadastrado']; 
					}
				}
				?>
				<tr style="height: 30px;">
					<td align="right" valign="top" colspan="5"><b>TOTAL :</b></td>
					<td align="center" valign="top"><input type="text" name="valortotal" id="valortotal" size="28" maxlength="" value="<? echo number_format($valortotal,2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);" disabled  class="disabled"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : right; width:25ex;" title='' /></td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td>
			<?php

			$sql = "SELECT ptres FROM monitora.pi_subacaodotacao sd
						INNER JOIN monitora.ptres p on sd.ptrid = p.ptrid
					WHERE sd.sbaid = $obPi_Subacao->sbaid AND p.ptrano = '{$_SESSION['exercicio']}'";
//			$sql = "SELECT ptres FROM monitora.pi_subacaodotacao sd
//						INNER JOIN monitora.ptres p on sd.ptrid = p.ptrid
//					WHERE sd.sbaid = 24";
			
			$arPtres = $db->carregar($sql);
			$arPtres = ($arPtres) ? $arPtres : array();			
			
			$nPtres = array();
			$boPtres = false;
			foreach($arPtres as $ptres){
				array_push($nPtres,$ptres['ptres']);
				$boPtres = true;
			}
			
			 
			if(!$boPtres){
				echo "<font color=\"blue\">É necessário vincular ação orçamentária para aprovar subação no exercício de {$_SESSION['exercicio']}.</font>";					
			}
			?>
		</td>
	</tr>
	<tr>
		<td colspan="2">
		<?php 
			$sql = "SELECT  
							to_char(sah.sahdata, 'DD/MM/YYYY HH24:MI:SS') as sahdata,
							CASE WHEN sah.sahsituacao = 'P' THEN ' <font color=\"red\">Pendente de Aprovação</font> '
								 WHEN sah.sahsituacao = 'A' THEN ' <font color=\"green\">Aprovado</font> '
								 WHEN sah.sahsituacao = 'R' THEN ' <font color=\"#3F85FF\">Revisado</font> '
								 WHEN sah.sahsituacao = 'E' THEN ' <font color=\"#EAC117\">Enviado para Revisão</font> ' 
							END as situacao,
							sah.sahobs,
							u.usunome 
					FROM monitora.pi_subacaohistorico sah
						INNER JOIN seguranca.usuario u on sah.usucpf = u.usucpf
					WHERE sbaid = $obPi_Subacao->sbaid 
					ORDER BY sah.sahdata DESC";
			
			$sql2 = "SELECT sbasituacao FROM monitora.pi_subacao WHERE sbaid = $obPi_Subacao->sbaid ";
			$ultimaSituacao = $db->pegaUm($sql2);
			
			$cabecalho = array("Data Hora", "Situação", "Observação", "Cadastrado Por");
			$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%', 'N' );
		?>
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td align="left" colspan="2">
			<?php if($ultimaSituacao == 'P' || $ultimaSituacao == 'R'){ ?>
				<?php if($boPtres){ ?>
					<input type="button" value="Aprovar" onclick="vincular('A')" style="cursor: pointer;"/>
				<?php } ?>
				<input type="button" value="Enviar para Revisão" onclick="montaShowModal()" style="cursor: pointer;"/>
			<?php } ?>
		</td>
	</tr>
	</form>
</table>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script>
//function vincular(situacao){
function vincular(situacao, boSahobs){
	if(boSahobs){
		var sahobs = $('sahobs1').value;
		if(sahobs == ''){
			alert('O campo da descrição é obrigatório');
			return false;
		} else {
			closeMessage();
		}
	} else {
		var sahobs = "";
	}
	
	var sbaid = '<?php echo $obPi_Subacao->sbaid; ?>';
 	var url = window.location.href;
	var parametros = "requisicao=vincular&sbaid="+sbaid+'&situacao='+situacao+'&sahobs='+sahobs;
	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(r) {
				//alert(r.responseText);
				//$('dibDegug').update(r.responseText);
				//return;
				if(r.responseText){
					alert('Dados gravados com Sucesso.');
					// feito isso por causa da presa.
					//window.opener.location.replace(window.opener.location);
					window.opener.document.formulario.submit();
					//window.opener.location.reload(); 
					window.close();
				}
			}
		}
	);
}

/*** INICIO SHOW MODAL ***/
var countModal = 1;

function montaShowModal() {
	var campoTextArea = '<form id="form" name="form"><div class="notprint">'+
			'<textarea class="txareaclsMouseOver" id="sahobs'+countModal+'" name="sahobs'+countModal+'" cols="80" rows="8" title="Mensagem" '+ 
				'onmouseover="MouseOver( this );" '+
				'onfocus="MouseClick( this );" '+
				'onmouseout="MouseOut( this );" '+
				'onblur="MouseBlur( this ); '+
				'textCounter( this.form.sahobs'+countModal+', this.form.no_sahobs, 500);" '+ 
				'style="width: 80ex;" '+
				'onkeydown="textCounter( this.form.sahobs'+countModal+', this.form.no_sahobs, 500 );" '+ 
				'onkeyup="textCounter( this.form.sahobs'+countModal+', this.form.no_sahobs, 500);">'+
			'</textarea><br> '+
			'<input readonly="readonly" style="border-left: 3px solid rgb(136, 136, 136); text-align: right; color: rgb(128, 128, 128);" '+ 
				'name="no_sahobs" size="6" maxlength="6" value="500" '+
				'class="CampoEstilo" type="text"> '+
			'<font size="1" color="red" face="Verdana"> máximo de caracteres</font> '+
		'</div><div id="print_sahobs" class="notscreen" style="text-align: left;"></div>'+
		//checkBoxEmail+
		'</form>';
	var alertaDisplay = '<center><div class="titulo_box" >É necessário colocar observação.<br/ >'+campoTextArea+'</div><div class="links_box" ><br><input type="button" onclick="vincular(\'E\',1)" value="Gravar" /> <input type="button" onclick=\'closeMessage(); return false \' value="Cancelar" /></center>';
	displayStaticMessage(alertaDisplay,false);
	return false;
}

function displayStaticMessage(messageContent,cssClass) {
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	messageObj.setHtmlContent(messageContent);
	messageObj.setSize(420,215);
	messageObj.setCssClassMessageBox(cssClass);
	messageObj.setSource(false);	// no html source since we want to use a static message here.
	messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
	messageObj.display();
}

function closeMessage() {
	messageObj.close();	
}

</script>
<div id="dibDegug"></div>