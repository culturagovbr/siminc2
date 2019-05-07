<?php
$perfis = pegaPerfilGeral();
//valida cod pi AJAX
if ($_REQUEST['piAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	validaCodPi($_POST['piAjax'], $_POST['pliid']);
	exit;
}

//valida cod pi AJAX
if ($_REQUEST['sbaAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	buscaDadosSubacao($_POST['sbaAjax'],$_POST['capidAjax']);
	exit;
}

// controlador do numero sequencial do gerador
include APPRAIZ."/includes/controlegeradorsequenciapi.inc";

include  APPRAIZ."includes/cabecalho.inc";

// carrega as funções de integração
include "planotrabalhoUN/unidade_atividade_funcoes.php";
// carrega as funções do módulo pde
include "planotrabalhoUN/_constantes.php";
include "planotrabalhoUN/_funcoes.php";
include "planotrabalhoUN/_componentes.php";

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
	$obPi_PlanoInternoHistorico->salvar();
	
	if($obPi_PlanoInternoHistorico->commit()){
		enviaEmailStatusPi($pliid);
		$retorno = true;
	}
	unset($obPi_PlanoInterno);
	unset($obPi_PlanoInternoHistorico);
	
	echo $retorno;
	die;
	
}

$unidade = $db->pegaLinha("SELECT * FROM public.unidade uni WHERE uni.unicod='".$_SESSION['monitora_var']['unicod']."'");
if(!$unidade) die("<script>alert('Unidade não encontrada');window.location = '?modulo=principal/planotrabalhoUN/listaUN&acao=A';</script>");

if(!$_POST['evento'] && $_GET['evento']){
	$_POST['evento'] = $_GET['evento'];
}

if(!$_POST['pliid'] && $_GET['pliid']){
	$_POST['pliid'] = $_GET['pliid'];
}

$boNaoVePlanoInterno = boNaoVePlanoInterno();
$permissao_formulario = (!$boNaoVePlanoInterno) ? 'S' : 'N'; # S habilita e N desabilita o formulário

echo "<br>";
$boSelecionaSubacao = false;
// verifica se ocorre algum evento
if(isset($_POST['evento']) && ($_POST['evento'] != '') ) {
	
	if($_REQUEST['combo_sbaid']) {
		$_REQUEST['sbaid'] = $_REQUEST['combo_sbaid']; 	
	}
	
	switch($_POST['evento']) {
		// atualizar os dados do plano interno
		case "G":
			
			$db->executar("DELETE FROM monitora.pi_planointernoptres WHERE pliid='".$_REQUEST['pliid']."'");
			
			// reinserindo os que ja existiam
			if($_POST['plivalored']) {
				foreach($_POST['plivalored'] as $ptrid => $valor) {
					$valor = str_replace(array(".",","), array("","."), $valor);
					$sql = "INSERT INTO monitora.pi_planointernoptres(
				            pliid, ptrid, pipvalor)
				    		VALUES ('".$_REQUEST['pliid']."', '".$ptrid."', '".$valor."');";
					$db->executar($sql);
				}
			}
			
			// inserindo novos
			if(is_array($_POST['acaid'])) {
				foreach($_POST['acaid'] as $ptres => $ptrid) {
					$valor = $_POST['plivalor'][$ptres][$ptrid] ? $_POST['plivalor'][$ptres][$ptrid] : 'null';		
					$valor = str_replace(array(".",","), array("","."), $valor);
					$sql = "INSERT INTO monitora.pi_planointernoptres(
				            pliid, ptrid, pipvalor)
				    		VALUES ('".$_REQUEST['pliid']."', '".$ptrid."', '".$valor."');";
					
					$db->executar($sql);

					
				}
			}
			
			$plicodorigem = $db->pegaUm("SELECT plicod FROM monitora.pi_planointerno WHERE pliid='".$_REQUEST['pliid']."'");
			 
			$qry = "INSERT INTO monitora.pi_planointernohistorico(
            	 	plicod, pihobs, pihdata, usucpf, pihsituacao, plicodorigem)
    			 	VALUES ('".$_POST['plicod']."', NULL, NOW(), '".$_SESSION['usucpf']."', 'P', '".$plicodorigem."')";
			
			//$db->executar($qry);
			
			$existe_plicod = $db->pegaUm("SELECT pliid FROM monitora.pi_planointerno WHERE plistatus='A' AND plicod='".$plicodnew."'");
			if($existe_plicod) die("<script>alert('O Código do PI ja existe.');window.location='?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A';</script>");
			
			$sql = "UPDATE monitora.pi_planointerno SET plicod='".$_POST['plicod']."', 
													    arceid='".$_POST['arceid']."', 
													    eqdid='".$_POST['eqdid']."', 
													    secid='".$_POST['secid']."', 
													    capid='".$_POST['capid']."', 
													    plilivre='".$_POST['plilivre']."', 
													    plidsc='".$_POST['plidsc']."',  
													    plidata='".date("Y-m-d")."', 
													    plititulo='".$_POST['plititulo']."' WHERE pliid='".$_REQUEST['pliid']."'";
			
			$db->executar($sql);
			
			$db->commit();
			
			die("<script>alert('Dados gravados com sucesso');window.location='monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A';</script>");
			
		break;
		
		// carregar os dados do pi
		case 'A':

			$planointerno = $db->pegaLinha("SELECT * FROM monitora.pi_planointerno pi 
											LEFT JOIN monitora.pi_subacao sb ON sb.sbaid = pi.sbaid 
											WHERE pliid = '".$_POST['pliid']."'");
			if($planointerno) {
				$pliid = $planointerno['pliid'];
				$plititulo = trim($planointerno['plititulo']);
				$plicod = $planointerno['plicod'];
				$plisituacao = $planointerno['plisituacao'];
				$plidsc = $planointerno['plidsc'];
				$_REQUEST['sbaid'] = $planointerno['sbaid'];
				$plititulo_sba = trim($planointerno['sbatitulo']).' - ';
				
				$arceid = $planointerno['arceid'];
				$eqdid = $planointerno['eqdid'];
				$secid = $planointerno['secid'];
				$capid = $planointerno['capid'];
				$plilivre      = $planointerno['plilivre'];
				
				$sql = "SELECT
							pl.pliid,
							ptr.ptres,
							pt.ptrid,
							pt.pipvalor, 
							ptr.acaid,
							trim(ac.prgcod||'.'||ac.acacod||'.'||ac.unicod||'.'||ac.loccod||' - '||ac.acadsc) as descricao,
							ptr.ptrdotacao as dotacaoinicial,
							round(sum( coalesce(dt.valor,0) ),2) as dotacaosubacao,
							coalesce((SELECT SUM(pipvalor) as valor FROM monitora.pi_planointernoptres pt2 inner join monitora.pi_planointerno p ON p.pliid = pt2.pliid WHERE p.sbaid is not null AND pt.ptrid = pt2.ptrid AND p.plistatus='A'),0) as detalhamento
						FROM monitora.pi_planointerno pl
						INNER JOIN monitora.pi_planointernoptres pt ON pt.pliid = pl.pliid 
						LEFT JOIN monitora.ptres ptr ON ptr.ptrid = pt.ptrid 
						LEFT JOIN planejamento.acao ac ON ac.acaid = ptr.acaid
						LEFT JOIN monitora.pi_subacaodotacao sad ON ptr.ptrid = sad.ptrid and sad.sbaid = pl.sbaid
						LEFT JOIN ( select sbaid, ptrid, 
									sum( sadvalor ) as valor
									from  monitora.pi_subacaodotacao
									group by sbaid, ptrid ) dt ON ptr.ptrid = dt.ptrid and dt.sbaid = sad.sbaid
						WHERE
								pl.sbaid is not null AND
								pl.pliid = '".$pliid."' AND
								pl.plistatus='A'
				    	GROUP BY pl.pliid, pt.ptrid, ptr.ptres, pl.plistatus, pt.pipvalor, ac.prgcod, ptr.acaid, ac.acacod, ac.unicod, ac.loccod, ac.acadsc, ptr.ptrdotacao
				    	ORDER BY ptr.ptres";

				$acoespl = $db->carregar($sql);
				
				$boSelecionaSubacao = true;
				
			} else {
				
				die("<script>alert('Plano interno não encontrado');window.location='?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A';</script>");

			}
			
		break;
		// inserir plano interno
		case 'I':
			// verificando a se existe algum plicod no BD
			$existe_plicod = $db->pegaUm("SELECT pliid FROM monitora.pi_planointerno WHERE plistatus='A' AND plicod='".$_POST['plicod']."'");
			if($existe_plicod) {
				die("<script>alert('Não foi possível criar o Plano Interno. Código já existente.');window.location='?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A';</script>");
			}
			
			$sql_I = "INSERT INTO monitora.pi_planointerno ( plicod, 
														  arceid, 
														  eqdid, 
														  secid, 
														  capid, 
														  plilivre, 
														  plidsc, 
														  usucpf, 
														  plidata, 
														  plititulo, 
														  plistatus,
														  plisituacao,
														  sbaid,
														  unicod,
														  pliano ) 
     			  	  VALUES ('".$_POST['plicod']."', 
     			  	  		  '".$_POST['arceid']."', 
     			  	  		  '".$_POST['eqdid']."', 
     			  	  		  '".$_POST['secid']."', 
     			  	  		  '".$_POST['capid']."', 
     			  	  		  upper('".$_POST['plilivre']."'), 
     			  	  		  '".$_POST['plidsc']."', 
     			  	  		  '".$_SESSION['usucpf']."',
     			  	  		  '".date("Y-m-d")."',
     			  	  		  '".$_POST['plititulo']."',
     			  	  		  'A',
     			  	  		  'P',
     			  	  		  '".$_REQUEST['sbaid']."',
     			  	  		  '".$_SESSION['monitora_var']['unicod']."',
     			  	  		  '".$_SESSION['exercicio']."'
     			  	  		  )
     			  	  RETURNING pliid";	
			$pliid = $db->pegaUm($sql_I);
			
			$qry = "INSERT INTO monitora.pi_planointernohistorico(pliid, plicod, pihobs, pihdata, usucpf, pihsituacao, plicodorigem)
    			    VALUES ('".$pliid."','".$_POST['plicod']."', NULL, NOW(), '".$_SESSION['usucpf']."', 'P', '".$_POST['plicod']."')";
			
			$db->executar($qry);
			

			if(is_array($_POST['acaid'])) {
				foreach($_POST['acaid'] as $ptres => $ptrid) {
					$valor = $_POST['plivalor'][$ptres][$ptrid] ? $_POST['plivalor'][$ptres][$ptrid] : 'null';		
					$valor = str_replace(array(".",","), array("","."), $valor);
					$sql = "INSERT INTO monitora.pi_planointernoptres(pliid, ptrid, pipvalor)
				    		VALUES ('".$pliid."', '".$ptrid."', '".$valor."');";
					$db->executar($sql);
					
				}
			}
			
			$db->commit();
			
			if($_REQUEST['atiid']){
				die("<script>alert('Dados salvos com sucesso.');window.location = '?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A&atiid={$_REQUEST['atiid']}';</script>");
			} else {
				die("<script>alert('Dados salvos com sucesso.');window.location = '?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A';</script>");				
			}
			
		break;
		
		case 'E':
			$sql_D = "UPDATE monitora.pi_planointerno SET plistatus = 'I' where pliid = '".$_POST['pliid']."'";
			$db->executar($sql_D);
			$db->commit();
			die("<script>alert('Registro removido com sucesso.');window.location = '?modulo=principal/planotrabalhoUN/pesquisa_piUN&acao=A';</script>");
		break;
	}
}

// montando aba
if($_REQUEST["atiid"]){
	echo montarAbasArray(carregardadosplanotrabalhoUN_sub(), "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A&atiid=".$_REQUEST['atiid'].(($_REQUEST['sbaid'])?"&sbaid=".$_REQUEST['sbaid']:""));
} else {
	echo montarAbasArray(carregardadosplanotrabalhoUN_raiz(), "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A".(($_REQUEST['sbaid'])?"&sbaid=".$_REQUEST['sbaid']:""));
}

// montando o menu
//echo montarAbasArray(carregardadosplanotrabalhoUN_raiz(), "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A".(($_REQUEST['sbaid'])?"&sbaid=".$_REQUEST['sbaid']:""));
monta_titulo("Subação/PI",'<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');


// obtém dados da atividade vinculada à ação
$atividade = retornaTarefaUnidade( $_SESSION['monitora_var']['unicod'], $_SESSION['exercicio'] );

if ($_REQUEST["atiid"]) {
	$atividade = atividade_pegar( $_REQUEST["atiid"] );
} else {
	$atividade = atividade_pegar( $atividade["atiid"] );
}

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script>
function titulopi() {
	if(document.getElementById('plititulo_sub').value != '') {
		document.getElementById('plititulo').value = document.getElementById('plititulo_sub').value + document.getElementById('plititulo').value.substr(document.getElementById('plititulo_sub').value.length);
	}
	return true;
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
</script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<form method="POST"  name="formulario" id="formulario">
<input type="hidden" name="evento" id="evento" value="<? echo (($_POST['pliid'])?"G":"I"); ?>">
<input type="hidden" name="plicod"  id="plicod" value="<?=$plicod;?>">
<input type="hidden" name="pliid"  id="pliid" value="<?=$_POST['pliid'];?>">
<input type="hidden" name="sbaidAnterior"  id="sbaidAnterior" value="">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
<td colspan="2">
<?= montar_resumo_atividade( $atividade ) ?>
</td>
</tr>

<tr>
	<td width="40%" align='right' class="SubTituloDireita">Subação:</td>
	<td>
	<? 
	$sql = "SELECT sub.sbaid as codigo, sub.sbacod || ' - ' || sub.sbatitulo as descricao FROM monitora.pi_subacao sub 
			INNER JOIN monitora.pi_subacaounidade suu ON suu.sbaid = sub.sbaid
			WHERE suu.unicod='".$_SESSION['monitora_var']['unicod']."' AND sub.sbastatus='A' AND sub.sbasituacao='A'";
	
	$habilitado = $permissao_formulario;
	if($_REQUEST['atiid']){
		$combo_sbaid = $db->pegaUm("select sbaid from monitora.pi_subacaoatividade where atiid = {$_REQUEST['atiid']}");
		if($combo_sbaid){
			$boSelecionaSubacao = true;
			$habilitado = 'N';
			$planointerno['sbaid'] = $combo_sbaid;
		}
	}
	$db->monta_combo('combo_sbaid', $sql, $habilitado,'Selecione','selecionarsubacao','','',400, '', 'sbaid', '', $_REQUEST['sbaid']); 
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
		    <td align="right"><a href="javascript:detfin('<?=$acpl['ptres']?>')"><? echo number_format($acpl['detalhamento'],2,',','.'); ?></a></td>
		    <td align="right"><? echo number_format(($acpl['dotacaosubacao']-$acpl['detalhamento']),2,',','.'); ?></td>
		    <td align="center"><input type="text" name="plivalored[<? echo $acpl['ptrid']; ?>]" size="28" maxlength="" value="<? echo number_format($acpl['pipvalor'],2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);calculovalorPI();"  class="normal"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this); verificaDisponivel(this,'<? echo $acpl['ptres']; ?>','<? echo number_format($acpl['pipvalor'],2,',','.'); ?>');" style="text-align : right; width:25ex;" title='' /></td>
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
	<tr>
		<td align="right" colspan="7">
		<?php if(!$_POST['pliid'] ) { ?>
			
		<?php } ?>	

		<?php if($_POST['pliid'] && ( $plisituacao == 'P' || $plisituacao == 'E') ) { ?>
			
		<?php } elseif($_POST['pliid'] && ( $plisituacao <> 'P' || $plisituacao <> 'E') ) { ?>	
			
		<?php } ?>

		</td>
	</tr>
	</table>
	</td>
</tr>
<tr>
	<td class='SubTituloDireita'>Enquadramento da Despesa:</td>
	<td>
	<?
	$sql = "SELECT eqdid as codigo, eqdcod ||' - '|| eqddsc as descricao
		    FROM planejamento.enquadramento_despesa 
		    WHERE eqdano='".$_SESSION['exercicio']."' and eqdstatus='A' 
		    ORDER BY eqdcod";
	
	$db->monta_combo('eqdid', $sql, $permissao_formulario, 'Selecione', 'atualizarPrevisaoPI', '', '', '240', 'S', 'eqdid'); 
	?>
	</td>
</tr>
<tr>
	<td class='SubTituloDireita'>Nível/Etapa de Ensino:</td>
	<td>
		<?
		$sql = "SELECT secid as codigo, neecod ||' - '|| needsc as descricao
			    FROM planejamento.segmento_cultural 
			    WHERE neeano='".$_SESSION['exercicio']."' and neestatus='A' 
			    ORDER BY neecod";
		
		$db->monta_combo('secid', $sql, $permissao_formulario, 'Selecione', 'atualizarPrevisaoPI', '', '', '240', 'S', 'secid');
		?>
	</td>
</tr>
<tr>
	<td class='SubTituloDireita'>Categoria de Apropriação:</td>
	<td>
		<?
		$sql = "SELECT capid as codigo, capcod ||' - '|| capdsc as descricao
			    FROM planejamento.categoria_apropriacao 
			    WHERE capano='".$_SESSION['exercicio']."' and capstatus='A' 
			    ORDER BY capcod";
		
		$db->monta_combo('capid', $sql, $permissao_formulario, 'Selecione', 'atualizarPrevisaoPI', '', '', '340', 'S', 'capid');
		?>
	</td>
</tr>	
<tr>
	<td align='right' class="SubTituloDireita">Codifição da Unidade(livre):</td>
	<td><? echo campo_texto('plilivre','S',$permissao_formulario,'',3,2,'','',null,null,null,'id="idCodificacao"','atualizarCodigoLivre(); atualizarPrevisaoPI();'); ?></td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Modalidade de Ensino / Tema / Público:</td>
    <td>
	<?
	$sql = "SELECT arceid as codigo, arccod ||' - '|| arcdsc as descricao
		    FROM planejamento.area_cultural 
		    WHERE arcano='".$_SESSION['exercicio']."' and arcstatus='A' 
		    ORDER BY arccod";
	
	$db->monta_combo('arceid', $sql, $permissao_formulario, 'Selecione', 'atualizarPrevisaoPI', '', '', '240', 'S', 'arceid');
 	?>
    </td>
</tr>

<tr>
	<td align='right' class="SubTituloDireita">Título:</td>
    <td>
    <input type="hidden" name="plititulo_sub" id="plititulo_sub" value="<? echo $plititulo_sba; ?>">
    <?
	echo campo_texto('plititulo','S',$permissao_formulario,'',50,45,'','',null,null,null,'id="plititulo" onKeyUp="titulopi();"');  
   	?>
    </td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita" valign="top">Descrição / Finalidade:</td>
    <td><? echo campo_textarea( 'plidsc', 'S', $permissao_formulario, '', 60, 2, 250 ); ?></td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Previsão PI:</td>
    <td align="left">
    
        <table style="background-color: #C9C9C9" cellpadding="" border="0" width="126px" >
        		<tr>
	        		<td>
	        		<table cellpadding="0" border="0" width="98%" >
		        		<tr>
		        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Enquadramento</b></td>
		        		<td align="center" colspan="3"><b>Subação</b></td>
		        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Nível</b></td>
		        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Apropriação</b></td>
		        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Codificação</b></td>
		        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Modalidade</b></td>
		        		</tr>
		        		<tr>
		        			<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="enquadramento"><? echo substr($planointerno['plicod'],0,1); ?></span></td>
		        			<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" colspan="3"><span id="subacao">XXXX</span></td>
							<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center"><span id="nivel"><? echo substr($planointerno['plicod'],5,1); ?></span></td>
							<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="apropriacao"><? echo substr($planointerno['plicod'],6,2); ?></span></td>
							<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="codificacao"><? echo substr($planointerno['plicod'],8,2); ?></span></td>		        			        			  
							<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="modalidade"><? echo substr($planointerno['plicod'],10,1); ?></span></td>
		        		</tr>
		        		<tr>
		        			<td colspan="7" align="center"><b>Código PI / SIAFI:
		        			<span id="enquadramento_i"><?php echo substr($planointerno['plicod'],0,1); ?></span><span id="subacao_i">XXXX</span><span id="nivel_i"><? echo substr($planointerno['plicod'],5,1); ?></span><span id="apropriacao_i"><? echo substr($planointerno['plicod'],6,2); ?></span><span id="codificacao_i"><? echo substr($planointerno['plicod'],8,2); ?></span><span id="modalidade_i"><? echo substr($planointerno['plicod'],10,1); ?></span></b></td>
		        		</tr>
		        	</table>	        		
	        		</td>
        		</tr>
        </table>
        
	</td>
</tr>

<tr bgcolor="#cccccc">
	<td colspan="2" align="right">
	<?php if($_POST['pliid'] && $plisituacao == 'E'){ ?>
		
	<?php } ?>
	
	<?php 
		//if(!$boNaoVePlanoInterno && ( $plisituacao == 'P' || $plisituacao == 'E') ) { 
        if (in_array(PERFIL_MONIT_SUPER_USUARIO, $perfis)){?>
	<?php } ?>
	</td>
	
</tr>
<!-- tr>
	<td colspan="2" class="SubTituloCentro">Lista de PIs - <? echo $db->pegaUm("SELECT UPPER(unidsc) FROM public.unidade WHERE unicod='".$_SESSION['monitora_var']['unicod']."'"); ?></td>
</tr -->
</table>
</form>
<?php 
/*
if($_REQUEST["atiid"]){
	$inner = "INNER JOIN monitora.pi_planointernoatividade pia ON p.pliid = pia.pliid";
	$where = "AND pia.atiid = {$_REQUEST["atiid"]} ";
}

if(!$boNaoVePlanoInterno) {
	$btExluir = "<a style=\"cursor:pointer;\" onclick=\"removerpi(\''||p.pliid||'\');\"><img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\"></a>";
} else {
	$btExluir = "<a><img src=\"/imagens/excluir_01.gif \" border=0 title=\"Excluir\"></a>";
}

$sql = "SELECT '<center><a style=\"cursor:pointer;\" onclick=\"alterarpi(\''||p.pliid||'\');\"><img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\"></a>'|| CASE p.plisituacao 
				WHEN 'P' THEN ' $btExluir ' 
				WHEN 'E' THEN ' $btExluir ' 
				WHEN 'R' THEN ' $btExluir ' 
				ELSE ''
				END ||'</center>' as acao,
			p.plicod as plicod,
			coalesce(p.plititulo,'Não preenchido') as titulo,
			coalesce(SUM(pt.pipvalor),0) as total,
			CASE WHEN p.plisituacao = 'P' THEN ' <font color=\"red\">Pendente</font> '
				 WHEN p.plisituacao = 'A' THEN ' <font color=\"green\">Aprovado</font> '
				 WHEN p.plisituacao = 'H' THEN ' <font color=\"blue\">Homologado</font> '
				 WHEN p.plisituacao = 'R' THEN	' <font color=\"#3F85FF\">Revisado</font> '
				 WHEN p.plisituacao = 'C' THEN	' <font color=\"#AF7817\">Cadastrado no SIAFI</font> '
				 WHEN p.plisituacao = 'E' THEN	' <font color=\"#EAC117\">Enviado para Revisão</font> '
				 WHEN p.plisituacao = 'S' THEN ' Confimado no SIAFI ' 
			END as situacao,
			(SELECT usunome ||' por '||to_char(pihdata, 'dd/mm/YYYY hh24:mi') FROM monitora.pi_planointernohistorico p1 LEFT JOIN seguranca.usuario u1 ON u1.usucpf = p1.usucpf WHERE p1.pliid=p.pliid ORDER BY p1.pihdata DESC LIMIT 1) as hst
			FROM monitora.pi_planointerno p 
			LEFT JOIN monitora.pi_planointernoptres pt ON pt.pliid = p.pliid 
			LEFT JOIN monitora.pi_subacaounidade su ON su.sbaid = p.sbaid
			$inner
			WHERE 
			su.unicod = '".$_SESSION['monitora_var']['unicod']."' AND
			p.plistatus = 'A' AND
			p.pliano = '{$_SESSION['exercicio']}'
			$where 
			GROUP BY p.pliid, p.plicod,p.plititulo,p.plidata,p.plisituacao 
			ORDER BY p.plidata DESC";

$cabecalho = array("","Código do PI","Título","Valor Previsto(R$)","Situação","Última atualização");
$db->monta_lista($sql,$cabecalho,50,5,'N','center','');
*/
?>
<script type="text/javascript">
function Trim(str){return str.replace(/^\s+|\s+$/g,"");}

function vincular(situacao, pliid){
	if(pliid){
	 	var url = window.location.href;
		var parametros = "requisicao=vincular&pliid="+pliid+'&situacao='+situacao ;
		var myAjax = new Ajax.Request(
			url,
			{
				method: 'post',
				parameters: parametros,
				asynchronous: false,
				onComplete: function(r) {
					if(r.responseText){
						alert('Dados gravados com Sucesso.');
						// feito isso por causa da presa.
						document.formulario.submit();
						//window.location.reload();
					}
				}
			}
		);
	}
}

function selecionarsubacao(sbaid) {
	var sbaidAnterior = document.getElementById("sbaidAnterior").value;
	if(!sbaidAnterior){
		document.getElementById("sbaidAnterior").value = sbaid;
	}
	
	if(sbaid) {
		document.getElementById("subacao").innerHTML   = "XXXX";
		document.getElementById("subacao_i").innerHTML = "XXXX";
		
		for(var i=(document.getElementById("orcamento").rows.length-3); i > 1 ;i--) {
			document.getElementById("orcamento").deleteRow(i);
		}
	
		var req = new Ajax.Request(window.location.href, {
								        method:     'post',
								        parameters: '&sbaAjax=' + sbaid,
								        asynchronous: false,
								        onComplete: function (res) {

											if(sbaid != sbaidAnterior){
												document.getElementById("plititulo").value = "";
												document.getElementById("plititulo_sub").value = "";
											}
	        
	    						        	var dados = res.responseText;
	    						        	dados = dados.split("!@#");
											document.getElementById("subacao").innerHTML   = dados[0];
											document.getElementById("subacao_i").innerHTML = dados[0];
											var plititulo = Trim(dados[1]) + ' - ' + Trim(document.getElementById("plititulo").value);
											document.getElementById("plititulo").value = Trim(plititulo.substr(0,45));
											document.getElementById("plititulo_sub").value = Trim(plititulo.substr(0,45));
	
								        }
								  });
		atualizaTituloCodigoLivre();
		document.getElementById("sbaidAnterior").value = sbaid;
	} else {
		document.getElementById("subacao").innerHTML   = "XXXX";
		document.getElementById("subacao_i").innerHTML = "XXXX";
		document.getElementById("plititulo").value     = "";
		
		for(var i=(document.getElementById("orcamento").rows.length-3); i > 1 ;i--) {
			document.getElementById("orcamento").deleteRow(i);
		}
	}
}


function submeter(pliid){
	var validado = true;
	if(validar(pliid)){
		if(validado) {
			document.formulario.submit();
		}
	}

}

function submeterComSituacao(pliid,situacao) {
	var validado = true;
	
	if(!validar()){
		return false;
	}
	
	if(validado) {
		document.formulario.plisituacao.value=situacao;
		document.formulario.submit();
	}
}


function removerpi(pliid){
	var conf = confirm("Você realmente deseja excluir este PI?");	
	if(conf) {
		document.getElementById('evento').value = 'E';
		document.getElementById('pliid').value = pliid;
		document.formulario.submit();	
	}
}

function alterarpi(pliid){
	document.getElementById('evento').value = 'A';
	document.getElementById('pliid').value = pliid;
	document.formulario.submit();
}


/* Função para subustituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}

function validar(x){	

	var msg = "";
	
	var tabela = document.getElementById('orcamento');
	// validando se existe ação selecionado/ valor
	if(tabela.rows.length == 4) {
		msg+="A escolha das ações é obrigatório.\n";
	} else {
		for(i=2;i<(tabela.rows.length-2);i++) {
			if(!tabela.rows[i].cells[6].firstChild.value) {
				msg+="Valor do PTRES: '"+tabela.rows[i].cells[0].innerHTML+"' é obrigatório.\n";
			}
		}
	}

	if(document.formulario.plititulo.value == '') {
		msg+="O preenchimento do campo Título é obrigatório.\n";
	}

	if(document.formulario.plilivre.value.length != 2){
		msg+="O preenchimento do campo Codifição da Unidade é obrigatório e igual a 2(dois dígitos).\n";
	}
	
	if(document.formulario.plidsc.value == ''){
		msg+="O preenchimento do campo Descrição / Finalidade é obrigatório.\n";
	}

	if(document.getElementById("sbaid").value == ''){
		msg+="A subação é obrigatório.\n";
	}
	
	// testa se foi selecionada o enquadramento
	if(document.getElementById("eqdid").value == ''){
		msg+="O preenchimento do campo Enquadramento da Despesa é obrigatório.\n";
	}
	if(document.getElementById("secid").value == ''){
		msg+="O preenchimento do campo Nível/Etapa de Ensino é obrigatório.\n";
	}
	if(document.getElementById("capid").value == ''){
		msg+="O preenchimento do campo Categoria de Apropriação é obrigatório.\n";
	}
	
	if(msg != "") {
		alert(msg);
		return false;
	
	}else{
		 var pi = (document.getElementById("enquadramento").innerHTML + 
		 		  document.getElementById("subacao").innerHTML + 
		 		  document.getElementById("nivel").innerHTML +
		 		  document.getElementById("apropriacao").innerHTML +
		 		  document.getElementById("codificacao").innerHTML + 
		 		  document.getElementById("modalidade").innerHTML);
		 
		 if(validapi(pi)) {
		 	document.getElementById("plicod").value = pi; 
		 	return true;
		 } else {
		 	return false;
		 }
		 
	
	}	
}

function valida2(pi) {
	 if(!pi){
		return true;
	 }
	 else{
	 	if(pi.substr(0,10) == 'pijaexiste'){
	 		pi = pi.replace("pijaexiste","");
		 	var alertaDisplay = '<div class="titulo_box" >Atenção!</div><div class="texto_box" >Plano Interno já existe!</div><div class="conteudo_box" >Veja abaixo os dados o Plano Interno cadastrado :</div><div class="conteudo_box" >' + pi + '</div><div class="links_box" ><input type="button" onclick=\'closeMessage();return false\' value="Cancelar" /></center>';
		 	displayStaticMessage(alertaDisplay,false);
	 		return false;
	 	}
	 	var alertaDisplay = '<div class="titulo_box" >Atenção!</div><div class="texto_box" >Já existe(m) PI(s) criado(s) com esta estrutura. Veja abaixo a relação dos PI(s) encontrados:</div><div class="conteudo_box" >' + pi + '</div><div class="links_box" >Deseja realmente criar?<br><center><input type="button" onclick=\'closeMessage();return false\' value="Cancelar" /></center>';
	 	displayStaticMessage(alertaDisplay,false);
	 	return false;
	 }
	
}

function validapi(pi) {
	var retorno = true;
	
	var req = new Ajax.Request(window.location.href, {
							        method:     'post',
							        parameters: '&piAjax=' + pi + '&pliid=' + document.getElementById('pliid').value,
							        asynchronous: false,
							        onComplete: function (res) {
    						        	x = res.responseText;
    									retorno = valida2(x);
							        }
							  });

	return retorno;	
}

function abrir_lista() {
	if(document.getElementById("sbaid").value == '') {
		alert("Selecione uma subação");
		return false;
	}
	janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/listarProgramaUN&acao=A&sbaid='+document.getElementById("sbaid").value, 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width='+(screen.width-120)+',height=680' ); janela.focus();
}


function atualizarPrevisaoPI(){
	if(document.getElementById('capid').value) {
		var apropriacao = document.getElementById('capid').options[document.getElementById('capid').selectedIndex].text.split(" - ");
		document.getElementById("apropriacao").innerHTML = apropriacao[0]; 
		document.getElementById("apropriacao_i").innerHTML = apropriacao[0];
	}
	if(document.getElementById('eqdid').value) {
		var enquadramento = document.getElementById('eqdid').options[document.getElementById('eqdid').selectedIndex].text.split(" - ");
		document.getElementById("enquadramento").innerHTML = enquadramento[0]; 
		document.getElementById("enquadramento_i").innerHTML = enquadramento[0];
	}
	if(document.getElementById('secid').value) {
		var nivel = document.getElementById('secid').options[document.getElementById('secid').selectedIndex].text.split(" - ");
		document.getElementById("nivel").innerHTML = nivel[0]; 
		document.getElementById("nivel_i").innerHTML = nivel[0];
	}
	if(document.getElementById('mdeid').value) {
		var modalidade = document.getElementById('mdeid').options[document.getElementById('mdeid').selectedIndex].text.split(" - ");
		document.getElementById("modalidade").innerHTML = modalidade[0]; 
		document.getElementById("modalidade_i").innerHTML = modalidade[0];
	}
	
	document.getElementById("idCodificacao").value = document.getElementById("idCodificacao").value.toUpperCase();
	document.getElementById("codificacao").innerHTML = document.getElementById("idCodificacao").value;
	document.getElementById("codificacao_i").innerHTML = document.getElementById("idCodificacao").value;

	atualizaTituloCodigoLivre();
	
}


function atualizarCodigoLivre() {
	document.getElementById("idCodificacao").value = document.getElementById("idCodificacao").value.toUpperCase();
	document.getElementById("codificacao").innerHTML = document.getElementById("idCodificacao").value;
	atualizaTituloCodigoLivre();
}

function atualizaTituloCodigoLivre(){
	if(document.getElementById("idCodificacao").value == '00'){
		var msg = "";
		if(!document.getElementById('sbaid').value){
			msg += 'Favor escolha uma Subação.\n';
		}

		if(!document.getElementById('capid').value) {
			msg += 'Favor escolha uma Categoria de Apropriação.\n';
		}

		if(msg){
			alert(msg);
			return false;
		} else {
				var req = new Ajax.Request(window.location.href, {
			        method:     'post',
			        parameters: '&sbaAjax=' + document.getElementById('sbaid').value+'&capidAjax='+document.getElementById('capid').value,
			        asynchronous: false,
			        onComplete: function (res) {
			        	if(res.responseText){
				        	document.getElementById("plititulo").value = "";
							document.getElementById("plititulo_sub").value = "";
				        	var dados = res.responseText;
				        	dados = dados.split("!@#");
				        	var apropriacao = dados[2];
							var plititulo = Trim(dados[1]) + ' - ' + apropriacao + Trim(document.getElementById("plititulo").value);
							document.getElementById("plititulo").value = Trim(plititulo.substr(0,45))+' ';
							document.getElementById("plititulo_sub").value = Trim(plititulo.substr(0,45))+' ';
				        }
	
			        }
			   });
		}
	}

//	if(document.getElementById('plilivre').value) {
//		var codificacao = document.getElementById("idCodificacao").value;
//		document.getElementById("codificacao").innerHTML = codlivre; 
//		document.getElementById("codificacao_i").innerHTML = codlivre;
//	}
	
} 

function calculovalorPI() {
	var tabela = document.getElementById('orcamento');
	var tot = 0;
	for(i=2;i<tabela.rows.length-2;i++) {
		if(tabela.rows[i].cells[6].firstChild.value != "") {
			tot = tot + parseFloat(replaceAll(replaceAll(tabela.rows[i].cells[6].firstChild.value,".",""),",","."));
		}
	}

	var c = tot.toString();
	if(c.indexOf('.') == -1) {
		document.getElementById('valortotalpi').value = tot.toFixed(2);
	} else {
		document.getElementById('valortotalpi').value = Arredonda(tot,2);
	}
	document.getElementById('valortotalpi').onkeyup();
}

function Arredonda( valor , casas ){
   var novo = Math.round( valor * Math.pow( 10 , casas ) ) / Math.pow( 10 , casas );
   var c = novo.toString();
   if(c.indexOf('.') == -1) {
	   	alert(novo);
   		return novo;
   } else {
   		return novo.toFixed(casas);
   }
}

function verificaDisponivel(campo, ptres, vlold) {
	var linha = document.getElementById('ptres_'+ptres);
	valordisp = parseFloat(replaceAll(replaceAll(linha.cells[5].innerHTML,".",""),",","."));
	valoratual = parseFloat(replaceAll(replaceAll(campo.value,".",""),",","."));
	if(valoratual>(valordisp+parseFloat(replaceAll(replaceAll(vlold,".",""),",",".")))) {		
		alert('Valor não pode ser maior do que o Disponível');
		campo.value = vlold;
		calculovalorPI();
	}
}

function detfin(ptres){
	janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/detalhafinanceiro_piUN&acao=A&ptres='+ptres, 'janela2', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width='+(screen.width-420)+',height=280' ); janela.focus();
}

function selecionarprograma(sba) {
	document.formulario.evento.value = "SUBACAO";
	document.formulario.submit();
}

messageObj = new DHTML_modalMessage();	// We only create one object of this class
messageObj.setShadowOffset(5);	// Large shadow


function displayMessage(url)
{
	
	messageObj.setSource(url);

	messageObj.setCssClassMessageBox(false);
	messageObj.setSize(400,200);
	messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
	messageObj.display();
}

function displayStaticMessage(messageContent,cssClass) {
	messageObj.setHtmlContent(messageContent);
	messageObj.setSize(600,500);
	messageObj.setCssClassMessageBox(cssClass);
	messageObj.setSource(false);	// no html source since we want to use a static message here.

	messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
	messageObj.display();
	
	
}

function closeMessage()
{
	messageObj.close();	
}

function carregarsubacaocodigo(sbaid,boPiTitulo) {
	if(sbaid) {
		var req = new Ajax.Request(window.location.href, {
								        method:     'post',
								        parameters: '&sbaAjax=' + sbaid,
								        asynchronous: false,
								        onComplete: function (res) {
	    						        	var dados = res.responseText;
	    						        	dados = dados.split("!@#");
											document.getElementById("subacao").innerHTML   = dados[0];
											document.getElementById("subacao_i").innerHTML = dados[0];

											if(boPiTitulo){
												var plititulo = Trim(dados[1]) + ' - ' + Trim(document.getElementById("plititulo").value);
												document.getElementById("plititulo").value = Trim(plititulo.substr(0,45));
												document.getElementById("plititulo_sub").value = Trim(plititulo.substr(0,45));
											}
								        }
								  });
							  
	}
}


var boSelecionaSubacao = '<?php echo $boSelecionaSubacao; ?>';
if(boSelecionaSubacao){
	var atiid = '<?php echo $_GET['atiid']; ?>';
	var boPiTitulo = false;
	if(atiid){
		boPiTitulo = true;
	}
	var sbaid = '<?php echo $planointerno['sbaid']; ?>';
	carregarsubacaocodigo(sbaid,boPiTitulo);
}

</script>