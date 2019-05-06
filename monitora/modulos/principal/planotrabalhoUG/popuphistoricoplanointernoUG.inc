<?php

include "planotrabalhoUG/_constantes.php";
include "planotrabalhoUG/_funcoes.php";
require_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";
require_once APPRAIZ . "monitora/classes/Pi_PlanoInternoHistorico.class.inc";

if($_REQUEST['requisicao'] == 'vincular'){
	
	extract($_POST);
	
	$retorno = false;
	
	$obPi_PlanoInterno = new Pi_PlanoInterno($pliid);
	$obPi_PlanoInterno->plisituacao = $situacao;
	$obPi_PlanoInterno->salvar();
	$sql = "SELECT plicod FROM monitora.pi_planointernohistorico WHERE pliid = $pliid ORDER BY pihdata DESC LIMIT 1";
	if(!$plicodOrigem = $db->pegaUm($sql)){
		$plicodOrigem = $obPi_PlanoInterno->plicod;	
	}
	
	$obPi_PlanoInternoHistorico = new Pi_PlanoInternoHistorico();
	$obPi_PlanoInternoHistorico->pliid 		  = $pliid;
	$obPi_PlanoInternoHistorico->usucpf 	  = $_SESSION['usucpf'];
	$obPi_PlanoInternoHistorico->pihsituacao  = $situacao;
	$obPi_PlanoInternoHistorico->plicod 	  = $obPi_PlanoInterno->plicod;
	$obPi_PlanoInternoHistorico->plicodorigem = $plicodOrigem;
	if($pihobs){
		$obPi_PlanoInternoHistorico->pihobs	= trim(utf8_decode($pihobs));
	}
	//ver($obPi_PlanoInternoHistorico,d);
	$obPi_PlanoInternoHistorico->salvar();
	
	if($obPi_PlanoInternoHistorico->commit()){
		enviaEmailStatusPi($pliid);
		//die;
		$retorno = true;
	}
	unset($obPi_PlanoInterno);
	unset($obPi_PlanoInternoHistorico);
	
	echo $retorno;
	die;
	
}

extract($_GET);

$obPi_PlanoInterno = new Pi_PlanoInterno($pliid);
if($obPi_PlanoInterno->pliid){
//$obPi_PlanoInterno->pliid =1030;
	$sql = "SELECT
				pl.pliid,
				ptr.ptres,
				pt.ptrid,
				pt.pipvalor, 
				ptr.acaid,
				trim(ac.prgcod||'.'||ac.acacod||'.'||ac.unicod||'.'||ac.loccod||' - '||ac.acadsc) as descricao,
				sum(ptr.ptrdotacao) as dotacaoinicial,
				round(sum( coalesce(sad.sadvalor,0) ),2) as dotacaosubacao,
--				coalesce((SELECT SUM(pipvalor) as valor FROM monitora.pi_planointernoptres pt2 inner join monitora.pi_planointerno p ON p.pliid = pt2.pliid WHERE pt.ptrid = pt2.ptrid AND p.plistatus='A'),0) as detalhamento
				coalesce(cast(SUM(dt2.valorpi) as varchar),'0.00') as detalhamento
			FROM monitora.pi_planointerno pl
			INNER JOIN monitora.pi_planointernoptres pt ON pt.pliid = pl.pliid 
			LEFT JOIN monitora.ptres ptr ON ptr.ptrid = pt.ptrid 
			LEFT JOIN planejamento.acao ac ON ac.acaid = ptr.acaid
			LEFT JOIN monitora.pi_subacaodotacao sad ON ptr.ptrid = sad.ptrid and sad.sbaid = pl.sbaid
			LEFT JOIN ( select sbaid, ptrid, 
						sum( sadvalor ) as valor
						from  monitora.pi_subacaodotacao
						group by sbaid, ptrid ) dt ON ptr.ptrid = dt.ptrid and dt.sbaid = sad.sbaid
			LEFT JOIN ( select sbaid, ptrid, sum(dtl.valorpi) as valorpi from monitora.v_pi_detalhepiptres dtl group by sbaid, dtl.ptrid ) dt2 ON ptr.ptrid = dt2.ptrid and dt2.sbaid = sad.sbaid	
			WHERE
					pl.pliid = '".$obPi_PlanoInterno->pliid."' AND
					pl.plistatus='A'
	    	GROUP BY pl.pliid, pt.ptrid, ptr.ptres, pl.plistatus, pt.pipvalor, ac.prgcod, ptr.acaid, ac.acacod, ac.unicod, ac.loccod, ac.acadsc
	    	ORDER BY ptr.ptres";
	
	$acoespl = $db->carregar($sql);
}
	$acoespl = ($acoespl) ? $acoespl : array();

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
			<h3>Dados do plano interno</h3>
			</center>
		</td> 
	</tr>
	<tr>
		<td align="center" colspan="2">
			<?php
			if($obPi_PlanoInterno->pliid){
			echo $unidade = $db->pegaUm("SELECT u.ungcod || ' - ' || u.ungdsc FROM monitora.pi_planointerno pi LEFT JOIN public.unidadegestora u ON u.ungcod = pi.ungcod WHERE pliid = ".$obPi_PlanoInterno->pliid); 
			}
			?>
		</td>
	</tr>
	<tr>
		<td colspan="2">
			<table cellpadding="0" border="0" width="98%" align="center" id="orcamento"  style="BORDER-RIGHT: #C9C9C9 1px solid; BORDER-TOP: #C9C9C9 1px solid; BORDER-LEFT: #C9C9C9 1px solid; BORDER-BOTTOM: #C9C9C9 1px solid;" onmouseover="tabindexcampo();">
				<tr>
					<td style="background-color: #C9C9C9;" colspan="7" align="center"><b>Detalhamento Orçamentário</b></td>
				</tr>
				<tr>
					<td style="background-color: #C9C9C9;" align="center" nowrap><b>PTRES</b><input type="hidden" name="pliptres"></td>
					<td style="background-color: #C9C9C9; width:45%;" align="center" nowrap><b>Ação</b></td>
					<td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Dotação Autorizada</b></td>
					<td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Dotação Subação</b></td>
					<td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Detalhado em PI</b></td>
					<td style="background-color: #C9C9C9; width:100px;" align="center"><b>Saldo Disponível</b></td>
					<td style="background-color: #C9C9C9;" align="center"><b>Valor Previsto(Anual)</b></td>
				</tr>
				<?
				 
				if($acoespl[0]) {
					$valortotalpi = 0;
					$cor = 0;
					foreach($acoespl as $acpl) { 
				?>
			        <tr style="height:30px;<? echo (($cor%2)?"":"background-color:#DCDCDC;"); ?>" id="ptres_<? echo $acpl['ptres']; ?>">
						<td align="center"><? echo $acpl['ptres']; ?></td>
						<td align="left"><? echo $acpl['descricao']; ?></td>
					    <td align="right"><? echo number_format($acpl['dotacaoinicial'],2,',','.'); ?></td>
					    <td align="right"><? echo number_format($acpl['dotacaosubacao'],2,',','.'); ?></td>
					    <td align="right"><? echo number_format($acpl['detalhamento'],2,',','.'); ?></td>
					    <td align="right"><? echo number_format(($acpl['dotacaosubacao']-$acpl['detalhamento']),2,',','.'); ?></td>
					    <td align="center"><input type="text" name="plivalored[<? echo $acpl['ptrid']; ?>]" size="28" maxlength="" value="<? echo number_format($acpl['pipvalor'],2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);calculovalorPI();"  disabled class="disabled"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this); verificaDisponivel(this,'<? echo $acpl['ptres']; ?>','<? echo number_format($acpl['pipvalor'],2,',','.'); ?>');" style="text-align : right; width:25ex;" title='' /></td>
					</tr>
				<? 
						$cor++;
						$valortotalpi = $valortotalpi + $acpl['pipvalor']; 
					}
				} 
				?>
				<tr style="height: 30px;">
					<td align="right" valign="top" colspan="6"><b>TOTAL :</b></td>
					<td align="center" valign="top"><input type="text" name="valortotalpi" id="valortotalpi" size="28" maxlength="" value="<? echo number_format($valortotalpi,2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);" disabled  class="disabled"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : right; width:25ex;" title='' /></td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
	<tr>
		<td class = "subtitulodireita">Enquadramento da Despesa:</td>
		<td><?php echo (($obPi_PlanoInterno->eqdid)?$db->pegaUm("SELECT eqdcod || ' - ' || eqddsc FROM planejamento.enquadramento_despesa WHERE eqdid='".$obPi_PlanoInterno->eqdid."'"):"Não informado"); ?></td>
	</tr>
	<tr>
		<td class = "subtitulodireita">Subação:</td>
		<td><?php echo $db->pegaUm("SELECT sbacod || ' - ' || sbatitulo FROM monitora.pi_subacao WHERE sbaid='".$obPi_PlanoInterno->sbaid."'"); ?></td>
	</tr>
	<tr>
		<td class = "subtitulodireita">Nível/Etapa de Ensino:</td>
		<td><?php echo $db->pegaUm("SELECT neecod || ' - ' || needsc FROM monitora.pi_niveletapaensino WHERE neeid='".$obPi_PlanoInterno->neeid."'"); ?></td>
	</tr>
	<tr>
		<td class = "subtitulodireita">Categoria de Apropriação:</td>
		<td><?php echo $db->pegaUm("SELECT capcod || ' - ' || capdsc FROM planejamento.categoria_apropriacao WHERE capid='".$obPi_PlanoInterno->capid."'"); ?></td>
	</tr>
	<tr>
		<td class = "subtitulodireita">Codificação da Unidade(livre):</td>
		<td><?php echo $obPi_PlanoInterno->plilivre; ?></td>
	</tr>
	<tr>
		<td class = "subtitulodireita">Modalidade de Ensino/Tema/Público:</td>
		<td><?php echo (($obPi_PlanoInterno->mdeid)?$db->pegaUm("SELECT mdecod || ' - ' || mdedsc FROM monitora.pi_modalidadeensino WHERE mdeid='".$obPi_PlanoInterno->mdeid."'"):"Não informado"); ?></td>
	</tr>
	<tr>
		<td class = "subtitulodireita">Título:</td>
		<td><?php echo $obPi_PlanoInterno->plititulo; ?></td>
	</tr>
	<tr>
		<td class = "subtitulodireita">Código do PI:</td>
		<td><?php echo $obPi_PlanoInterno->plicod; ?></td>
	</tr>
		<tr>
		<td class = "subtitulodireita">Descrição do PI:</td>
		<td><?php echo $obPi_PlanoInterno->plidsc; ?></td>
	</tr>
	<tr>
		<td colspan="2">
		<?php 
			$sql = "SELECT 
					    to_char(pih.pihdata, 'DD/MM/YYYY HH24:MI:SS') as pihdata,
					    CASE WHEN pih.pihsituacao = 'P' THEN  ' Pendente '
					         WHEN pih.pihsituacao = 'A' THEN ' Aprovado ' 
					         WHEN pih.pihsituacao = 'H' THEN ' Homologado ' 
					         WHEN pih.pihsituacao = 'R' THEN ' Revisado ' 
					         WHEN pih.pihsituacao = 'C' THEN ' Cadastrado no SIAFI ' 
					         WHEN pih.pihsituacao = 'E' THEN ' Enviado para Revisão ' 
					   END as situacao,
						pih.pihobs,
						u.usunome,
						unidsc 
					FROM monitora.pi_planointernohistorico pih
						INNER JOIN seguranca.usuario u on pih.usucpf = u.usucpf
						INNER JOIN unidade un on un.unicod = u.unicod
					WHERE pliid = $obPi_PlanoInterno->pliid 
						ORDER BY pih.pihdata DESC";
			
			$sql2 = "SELECT plisituacao FROM monitora.pi_planointerno WHERE pliid = $obPi_PlanoInterno->pliid ";
			$ultimaSituacao = $db->pegaUm($sql2);

			$cabecalho = array("Data Hora", "Situação", "Observação", "Cadastrado Por", "Unidade");
			$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%', 'N' );
		?>
		</td>
	</tr>
	<?php
	
	if(!isset($_GET['tipoacao']) && !$_GET['tipoacao']){ 
	
		$sql = "SELECT ptres FROM monitora.pi_planointernoptres pip
							INNER JOIN monitora.ptres p on pip.ptrid = p.ptrid
						WHERE pip.pliid = {$_GET['pliid']}";
			
		$arPtres = $db->carregar($sql);
		$arPtres = ($arPtres) ? $arPtres : array();			
		
		$nPtres = array();
		$boPtres = false;
		foreach($arPtres as $ptres){
			array_push($nPtres,$ptres['ptres']);
			$boPtres = true;
		}
	
		# Verifica se existe ptres do pi
		$sql = "select p.pliid, pi.ptrid, ptres.unicod
				from monitora.pi_planointerno p
					inner join monitora.pi_planointernoptres pi ON pi.pliid = p.pliid
					inner join monitora.ptres ptres ON ptres.ptrid = pi.ptrid
				where p.pliid = {$_GET['pliid']}";
		$arDados = $db->carregar($sql);
		$arDados = ($arDados) ? $arDados : array();
		$boNaoPtresFNDE = $boPtresFNDE = false;
		if($arDados[0] && count($arDados)){
			foreach($arDados as $dados){
//				if($dados['unicod'] == FNDE || $dados['unicod'] == CAPES || $dados['unicod'] == INEP){
				if($dados['unicod'] == FNDE ){
					$boPtresFNDE = true;
				} else {
					$boNaoPtresFNDE = true;
				}
			}
		}
		
		$perfilGestorSubacao = possui_perfil(PERFIL_MONITORA_GESTORSUBACOES);
		if($perfilGestorSubacao){
				if($boPtresFNDE && !$boNaoPtresFNDE){
	?>
					<tr bgcolor="#C0C0C0">
						<td align="left" colspan="2">
							<?php if($ultimaSituacao == 'P' || $ultimaSituacao == 'R'){ ?>
								<input type="button" value="Homologar" onclick="vincular('H','')" style="cursor: pointer;"/>
								<input type="button" value="Enviar para Revisão" onclick="montaShowModal()" style="cursor: pointer;"/>
							<?php } ?>
							<?php if($ultimaSituacao == 'H'){ ?>
								<?php if($boPtres){ ?>
									<input type="button" value="Aprovar" onclick="vincular('A','')" style="cursor: pointer;"/>
								<?php } ?>
								<input type="button" value="Enviar para Revisão" onclick="montaShowModal()" style="cursor: pointer;"/>
							<?php } ?>
							<?php if($ultimaSituacao == 'A'){ ?>
								<input type="button" value="Cadastrado no SIAFI" onclick="vincular('C')" style="cursor: pointer;"/>
								<input type="button" value="Enviar para Revisão" onclick="montaShowModal()" style="cursor: pointer;"/>
							<?php 
								} 
							?>
							<?php if($ultimaSituacao == 'C' || $ultimaSituacao == 'S'){ ?>
								<input type="button" value="Enviar para Revisão" onclick="montaShowModal()" style="cursor: pointer;"/>
							<?php 
								} 
							?>
							</td>
					</tr>
	<?php
				}
				elseif($boPtresFNDE && $boNaoPtresFNDE){
	?>
					<tr bgcolor="#C0C0C0">
						<td align="left" colspan="2">
							<?php if($ultimaSituacao == 'P' || $ultimaSituacao == 'R'){ ?>
								<input type="button" value="Homologar" onclick="vincular('H','')" style="cursor: pointer;"/>
							<?php } ?>
							<?php if($ultimaSituacao == 'A'){ ?>
								<input type="button" value="Cadastrado no SIAFI" onclick="vincular('C')" style="cursor: pointer;"/>
							<?php 
								} 
							?>
							<?php if($ultimaSituacao == 'C' && !verifica_cadastrosiafi($_GET['pliid']) ) { ?>
								<input type="button" value="Cadastrado no SIAFI" onclick="vincular('C')" style="cursor: pointer;"/>
							<?php 
								} 
							?>
								<input type="button" value="Enviar para Revisão" onclick="montaShowModal()" style="cursor: pointer;"/>
						</td>
					</tr>
	<?php
				}
				elseif($boNaoPtresFNDE){
					
	?>
					<tr bgcolor="#C0C0C0">
						<td align="left" colspan="2">
							<?php if($ultimaSituacao == 'P' || $ultimaSituacao == 'R'){ ?>
								<input type="button" value="Homologar" onclick="vincular('H','')" style="cursor: pointer;"/>
							<?php } ?>
						</td>
					</tr>
	<?php
				}
		} else {
		?>
	<tr bgcolor="#C0C0C0">
		<td align="left" colspan="2">
			<?php if($ultimaSituacao == 'P' || $ultimaSituacao == 'R'){ ?>
				<?php if(!$boPtresFNDE){ ?>
					<input type="button" value="Homologar" onclick="vincular('H','')" style="cursor: pointer;"/>
				<?php } ?>
				<input type="button" value="Enviar para Revisão" onclick="montaShowModal()" style="cursor: pointer;"/>
			<?php } ?>
			<?php if($ultimaSituacao == 'H'){ ?>
				<?php if($boPtres){ ?>
					<?php if($boPtresFNDE && $boNaoPtresFNDE){ ?>
						<input type="button" value="Aprovar" onclick="vincular('A','')" style="cursor: pointer;"/>
					<?php } elseif($boNaoPtresFNDE){ ?>
						<input type="button" value="Aprovar" onclick="vincular('A','')" style="cursor: pointer;"/>
					<?php } ?>
				<?php } ?>
				<input type="button" value="Enviar para Revisão" onclick="montaShowModal()" style="cursor: pointer;"/>
			<?php }
				if($ultimaSituacao == 'A'){
			?>
					<input type="button" value="Cadastrado no SIAFI" onclick="vincular('C')" style="cursor: pointer;"/>
				<input type="button" value="Enviar para Revisão" onclick="montaShowModal()" style="cursor: pointer;"/>
			<?php 
				} 
			?>
				<?php 
				
				if($ultimaSituacao == 'C' && !verifica_cadastrosiafi($_GET['pliid']) ){
			?>
					<input type="button" value="Cadastrado no SIAFI" onclick="vincular('C')" style="cursor: pointer;"/>
			<?php 
				}
				if($ultimaSituacao == 'C' || $ultimaSituacao == 'S'){
			?>
					<input type="button" value="Enviar para Revisão" onclick="montaShowModal()" style="cursor: pointer;"/>
			<?php 
				} 
			?>
		</td>
	</tr>
	<?php } 
	} // fim do if tipoacao = consulta
	?>
	</form>
</table>
<!-- LINKS MODAL -->
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script>
function vincular(situacao, boPihobs){
	if(boPihobs){
		var pihobs = $('pihobs1').value;
		if(pihobs == ''){
			alert('O campo da descrição é obrigatório');
			return false;
		} else {
			closeMessage();
		}
	} else {
		var pihobs = "";
	}

	var pliid = '<?php echo $obPi_PlanoInterno->pliid; ?>';
	
 	var url = window.location.href;
	var parametros = "requisicao=vincular&pliid="+pliid+'&situacao='+situacao+'&pihobs='+pihobs;
	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(r) {
				if(r.responseText){
					//$('dibDebug').update(r.responseText);
					//return false;
					
					alert('Dados gravados com Sucesso.');
					// feito isso por causa da presa.
					window.opener.document.formulario1.submit();
					window.close();
					
				}
			}
		}
	);
}

//coloca tabindex no campo valor
function tabindexcampo(){
	var x = document.getElementsByTagName("input");
	var y = 1;
	for(i=0;i<x.length;i++) {
		if(x[i].type=="text"){
			if(x[i].name.substr(0,8) == 'plivalor'){
				x[i].tabIndex=y;
				y++;
			}
		}
	}
}

//messageObj = new DHTML_modalMessage();	// We only create one object of this class
//messageObj.setShadowOffset(5);	// Large shadow

/*** INICIO SHOW MODAL ***/
var countModal = 1;

function montaShowModal() {

	var campoTextArea = '<form id="form" name="form"><div class="notprint">'+
			'<textarea class="txareaclsMouseOver" id="pihobs'+countModal+'" name="pihobs'+countModal+'" cols="80" rows="8" title="Mensagem" '+ 
				'onmouseover="MouseOver( this );" '+
				'onfocus="MouseClick( this );" '+
				'onmouseout="MouseOut( this );" '+
				'onblur="MouseBlur( this ); '+
				'textCounter( this.form.pihobs'+countModal+', this.form.no_pihobs, 500);" '+ 
				'style="width: 80ex;" '+
				'onkeydown="textCounter( this.form.pihobs'+countModal+', this.form.no_pihobs, 500 );" '+ 
				'onkeyup="textCounter( this.form.pihobs'+countModal+', this.form.no_pihobs, 500);">'+
			'</textarea><br> '+
			'<input readonly="readonly" style="border-left: 3px solid rgb(136, 136, 136); text-align: right; color: rgb(128, 128, 128);" '+ 
				'name="no_pihobs" size="6" maxlength="6" value="500" '+
				'class="CampoEstilo" type="text"> '+
			'<font size="1" color="red" face="Verdana"> máximo de caracteres</font> '+
		'</div><div id="print_pihobs" class="notscreen" style="text-align: left;"></div>'+
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
<div id="dibDebug"></div>