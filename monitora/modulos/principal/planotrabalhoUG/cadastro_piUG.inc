<?php

//valida cod pi AJAX
if ($_POST['piAjax']) {
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

if ($_REQUEST['carregarComboEnquadramentoPorSubacao']) {
	header('content-type: text/html; charset=ISO-8859-1');
	carregarComboEnquadramentoPorSubacao($_POST['sbaid']);
	exit;
}


// controlador do numero sequencial do gerador
include APPRAIZ."/includes/controlegeradorsequenciapi.inc";

include  APPRAIZ."includes/cabecalho.inc";

// carrega as funções de integração
include "planotrabalhoUG/unidade_atividade_funcoes.php";
// carrega as funções do módulo pde
include "planotrabalhoUG/_constantes.php";
include "planotrabalhoUG/_funcoes.php";
include "planotrabalhoUG/_componentes.php";

require_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";
require_once APPRAIZ . "monitora/classes/Pi_PlanoInternoHistorico.class.inc";

if($_POST['requisicao'] == 'vincular'){
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
	$obPi_PlanoInternoHistorico->pliid        = $pliid;
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

$unidadegestora = $db->pegaLinha("SELECT * FROM public.unidadegestora ung WHERE ung.ungcod='".$_SESSION['monitora_var']['ungcod']."'");
if(!$unidadegestora) die("<script>alert('Unidade não encontrada');window.location = '?modulo=principal/planotrabalhoUG/listaUG&acao=A';</script>");

echo "<br>";

if(!$_POST['evento'] && $_GET['evento']){
	$_POST['evento'] = $_GET['evento'];
}

if(!$_POST['pliid'] && $_GET['pliid']){
	$_POST['pliid'] = $_GET['pliid'];
}

//ver($_POST,$_GET,d);
// verifica se ocorre algum evento
if(isset($_POST['evento']) && ($_POST['evento'] != '') ) {
	
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
					$valor = $_POST['plivalor'][$ptres][$ptrid] ? $_POST['plivalor'][$ptres][$ptrid] : '0,00';		
					$valor = str_replace(array(".",","), array("","."), $valor);
					$sql = "INSERT INTO monitora.pi_planointernoptres(
				            pliid, ptrid, pipvalor)
				    		VALUES ('".$_REQUEST['pliid']."', '".$ptrid."', '".$valor."');";
					$db->executar($sql);

					
				}
			}
			//$plicodorigem = $db->pegaUm("SELECT plicod FROM monitora.pi_planointerno WHERE pliid='".$_REQUEST['pliid']."'");
			 
//			$qry = "INSERT INTO monitora.pi_planointernohistorico(
//            	 	plicod, pihobs, pihdata, usucpf, pihsituacao, plicodorigem)
//    			 	VALUES ('".$_POST['plicod']."', NULL, NOW(), '".$_SESSION['usucpf']."', 'P', '".$plicodorigem."')";
			
			//$db->executar($qry);
			
			$existe_plicod = $db->pegaUm("SELECT pliid FROM monitora.pi_planointerno WHERE plistatus='A' AND pliano ='".$_SESSION['exercicio']."' AND plicod='".$plicodnew."'");
			if($existe_plicod) die("<script>alert('O Código do PI ja existe.');window.location='?modulo=principal/planotrabalhoUG/cadastro_piUG&acao=A';</script>");
			
			$stModalidade = "";
			if($_POST['boModalidade']){
				$stModalidade = "mdeid='".$_POST['mdeid']."',"; 
			}
			
			$sql = "UPDATE monitora.pi_planointerno SET plicod='".$_POST['plicod']."',
                                $stModalidade
                                eqdid='".$_POST['eqdid']."', 
                                neeid='".$_POST['neeid']."', 
                                capid='".$_POST['capid']."', 
                                plilivre='".$_POST['plilivre']."', 
                                plidsc='".$_POST['plidsc']."', 
                                plidata='".date("Y-m-d")."', 
                                plititulo='".$_POST['plititulo']."',
                                sbaid='".$_POST['sbaid']."' WHERE pliid='".$_REQUEST['pliid']."'";
			$db->executar($sql);
			
			$db->commit();
			
			die("<script>alert('Dados gravados com sucesso');window.location='monitora.php?modulo=principal/planotrabalhoUG/cadastro_piUG&acao=A';</script>");
			
		break;
		
		// carregar os dados do pi
		case 'A':
			$planointerno = $db->pegaLinha("SELECT * FROM monitora.pi_planointerno pi LEFT JOIN monitora.pi_subacao sb ON sb.sbaid = pi.sbaid WHERE pliid = '".$_POST['pliid']."'");
			if($planointerno) {
				$pliid = $planointerno['pliid'];
				$plicod = $planointerno['plicod'];
				$plititulo = $planointerno['plititulo'];
				$plisituacao = $planointerno['plisituacao'];
				$plidsc = $planointerno['plidsc'];
				$_REQUEST['sbaid'] = $planointerno['sbaid'];
				$plititulo_sba = trim($planointerno['sbatitulo']).' - ';
				
				$mdeid = $planointerno['mdeid'];
				$eqdid = $planointerno['eqdid'];
				$neeid = $planointerno['neeid'];
				$capid = $planointerno['capid'];
				$plilivre      = $planointerno['plilivre'];
				
				$sql = "SELECT
							pl.pliid,
							ptr.ptres,
							pt.ptrid,
							pt.pipvalor, 
							ptr.acaid,
							trim(ac.prgcod||'.'||ac.acacod||'.'||ac.unicod||'.'||ac.loccod||' - '||ac.acadsc) as descricao,
							sum(ptr.ptrdotacao) as dotacaoinicial,
							round(sum( coalesce(sad.sadvalor,0) ),2) as dotacaosubacao,
							coalesce(cast(SUM(dt2.valorpi) as varchar),'0.00') as detalhamento
						FROM monitora.pi_planointerno pl
						INNER JOIN monitora.pi_planointernoptres pt ON pt.pliid = pl.pliid 
						LEFT JOIN monitora.ptres ptr ON ptr.ptrid = pt.ptrid 
						LEFT JOIN monitora.acao ac ON ac.acaid = ptr.acaid
						LEFT JOIN monitora.pi_subacaodotacao sad ON ptr.ptrid = sad.ptrid and sad.sbaid = pl.sbaid
						LEFT JOIN ( select sbaid, ptrid, sum(dtl.valorpi) as valorpi from monitora.v_pi_detalhepiptres dtl group by sbaid, dtl.ptrid ) dt2 ON ptr.ptrid = dt2.ptrid and dt2.sbaid = sad.sbaid
						WHERE
								pl.pliid = '".$pliid."' AND
								pl.plistatus='A'
				    	GROUP BY pl.pliid, pt.ptrid, ptr.ptres, pl.plistatus, pt.pipvalor, ac.prgcod, ptr.acaid, ac.acacod, ac.unicod, ac.loccod, ac.acadsc
				    	ORDER BY ptr.ptres";
				//ver($sql);
				$acoespl = $db->carregar($sql);
				
				$boSelecionaSubacao = true;
				
				//$permissao_formulario = 'N';
				$disabled = " disabled='disabled' ";
				if($plisituacao == 'P' || $plisituacao == 'R' || $plisituacao == 'E'){
						$permissao_formulario = 'S';
						$disabled = "";
	
					}
				
			} else {
				
				die("<script>alert('Plano interno não encontrado');window.location='?modulo=principal/planotrabalhoUG/cadastro_piUG&acao=A';</script>");

			}
			
		break;
		// inserir plano interno
		case 'I':
/*
			if( (integer)$_POST['plivalored'] == 0)
			{
				die("<script>alert('O Valor Previsto informado deverá ser maior que 0(zero).');window.location='?modulo=principal/planotrabalhoUG/cadastro_piUG&acao=A';</script>");
			}
*/
			// verificando a se existe algum plicod no BD
			$existe_plicod = $db->pegaUm("SELECT pliid FROM monitora.pi_planointerno WHERE plistatus='A' AND pliano ='".$_SESSION['exercicio']."' AND plicod='".$_POST['plicod']."'");
			if($existe_plicod) {
				die("<script>alert('Não foi possível criar o Plano Interno. Código já existente.');window.location='?modulo=principal/planotrabalhoUG/cadastro_piUG&acao=A';</script>");
			}
			
			$stModalidade = "";
			if($_POST['boModalidade']){
				$stModalidade = "mdeid,"; 
				$stModalidade2 = "'".$_POST['mdeid']."',"; 
			}
			
			$sql_I = "INSERT INTO monitora.pi_planointerno ( plicod, 
														  $stModalidade 
														  eqdid, 
														  neeid, 
														  capid, 
														  plilivre, 
														  plidsc, 
														  usucpf, 
														  plidata, 
														  plititulo, 
														  plistatus,
														  plisituacao,
														  sbaid,
														  pliano,
														  ungcod ) 
     			  	  VALUES ('".$_POST['plicod']."', 
     			  	  		  $stModalidade2
     			  	  		  '".$_POST['eqdid']."', 
     			  	  		  '".$_POST['neeid']."', 
     			  	  		  '".$_POST['capid']."', 
     			  	  		  upper('".$_POST['plilivre']."'), 
     			  	  		  '".$_POST['plidsc']."', 
     			  	  		  '".$_SESSION['usucpf']."',
     			  	  		  '".date("Y-m-d")."',
     			  	  		  '".$_POST['plititulo']."',
     			  	  		  'A',
     			  	  		  'P',
     			  	  		  '".$_REQUEST['sbaid']."',
     			  	  		  '".$_SESSION['exercicio']."',
     			  	  		  '".$_SESSION['monitora_var']['ungcod']."')
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
			
			die("<script>alert('Dados salvos com sucesso.');window.location = '?modulo=principal/planotrabalhoUG/cadastro_piUG&acao=A';</script>");
			
		break;
		
		case 'E':
						
			$sql_D = "UPDATE monitora.pi_planointerno SET plistatus = 'I' where pliid = '".$_POST['pliid']."'";
			$db->executar($sql_D);
			$db->commit();
			die("<script>alert('Registro removido com sucesso.');window.location = '?modulo=principal/planotrabalhoUG/cadastro_piUG&acao=A';</script>");
			
		break;
	}
}


// montando aba
if($_REQUEST["atiid"]){
	echo montarAbasArray(carregardadosplanotrabalhoUG_sub(), "/monitora/monitora.php?modulo=principal/planotrabalhoUG/cadastro_piUG&acao=A&atiid=".$_REQUEST['atiid'].(($_REQUEST['sbaid'])?"&sbaid=".$_REQUEST['sbaid']:""));
} else {
	echo montarAbasArray(carregardadosplanotrabalhoUG_raiz(), "/monitora/monitora.php?modulo=principal/planotrabalhoUG/cadastro_piUG&acao=A".(($_REQUEST['sbaid'])?"&sbaid=".$_REQUEST['sbaid']:""));
}

// obtém dados da atividade vinculada à ação
$atividade = retornaTarefaUnidade( $_SESSION['monitora_var']['ungcod'], $_SESSION['exercicio'] );

if ($_REQUEST["atiid"]) {
	$atividade = atividade_pegar( $_REQUEST["atiid"] );
} else {
	$atividade = atividade_pegar( $atividade["atiid"] );
}

//monta_titulo("Subação/PI",$atividade['atidescricao'].'  <img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
print '<tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">Subação/PI</label></td></tr>';
print '<tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >'.$atividade['atidescricao'].'</td></tr>';
print '<tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.</td></tr></table>';


$arUnidadeSemModalidae = array();
$arUnidadeSemModalidae[] = '153978';

$boModalidade = true;
$max_length = 2;
if(in_array($_SESSION['monitora_var']['ungcod'],$arUnidadeSemModalidae)){
	$boModalidade = false;
	$max_length = 3;
}

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>

<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />

<form method="POST" name="formulario">
    <input type="hidden" name="evento" id="evento" value="<? echo (($_POST['pliid']) ? "G" : "I"); ?>">
    <input type="hidden" name="plicod" id="plicod" value="<?= $plicod; ?>">
    <input type="hidden" name="pliid" id="pliid" value="<?= $_POST['pliid']; ?>">
    <input type="hidden" name="boModalidade" id="boModalidade" value="<? echo $boModalidade; ?>">
    <input type="hidden" name="sbaidAnterior"  id="sbaidAnterior" value="">
    
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td width="40%" align='right' class="SubTituloDireita">Subação:</td>
            <td>
                <?php
                    $sql = "SELECT  sub.sbaid AS codigo, sub.sbacod || ' - ' || sub.sbatitulo AS descricao
                        FROM 
                            monitora.pi_subacao sub 
                        INNER JOIN 
                            monitora.pi_subacaounidade suu ON (suu.sbaid = sub.sbaid)
                        WHERE 
                            suu.ungcod IS NOT NULL AND 
                            sub.sbastatus='A' AND 
                            sub.sbasituacao='A'
                        ORDER BY sub.sbacod";
                    
                    $habilitado = 'S';
                    if ($_REQUEST['atiid']) {
                        $sbaid = $db->pegaUm("select sbaid from monitora.pi_subacaoatividade where atiid = {$_REQUEST['atiid']}");
                        if ($sbaid) {
                            $habilitado = 'N';
                            $boSelecionaSubacao = true;
                            $planointerno['sbaid'] = $sbaid;
                        }
                    }
                    
                    $db->monta_combo('sbaid', $sql, $habilitado, 'Selecione', 'selecionarsubacao', '', '', 400, '', 'sbaid', '', $_REQUEST['sbaid']);
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
                <?php
                    if ($acoespl[0]) {
                        $valortotalpi = 0;
                        $cor = 0;
                        foreach ($acoespl as $acpl) {
                ?>
                            <tr style="height:30px;<? echo (($cor % 2) ? "" : "background-color:#DCDCDC;"); ?>" id="ptres_<? echo $acpl['ptres']; ?>">
                                <td align="center"><? echo $acpl['ptres']; ?></td>
                                <td align="left"><? echo $acpl['descricao']; ?></td>
                                <td align="right"><? echo number_format($acpl['dotacaoinicial'], 2, ',', '.'); ?></td>
                                <td align="right"><? echo number_format($acpl['dotacaosubacao'], 2, ',', '.'); ?></td>
                                <td align="right"><a href="javascript:detfin('<?= $acpl['ptres'] ?>')"><? echo number_format($acpl['detalhamento'], 2, ',', '.'); ?></a></td>
                                <td align="right"><? echo number_format(($acpl['dotacaosubacao'] - $acpl['detalhamento']), 2, ',', '.'); ?></td>
                                <td align="center"><input type="text" name="plivalored[<? echo $acpl['ptrid']; ?>]" id="plivalored[<? echo $acpl['ptrid']; ?>]" size="28" maxlength="" value="<? echo number_format($acpl['pipvalor'], 2, ',', '.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);calculovalorPI();"  class="normal"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this); verificaDisponivel(this,'<? echo $acpl['ptres']; ?>','<? echo number_format($acpl['pipvalor'], 2, ',', '.'); ?>');" style="text-align : right; width:25ex;" title='' /></td>
                            </tr>
                <?php
                            $cor++;
                            $valortotalpi = $valortotalpi + $acpl['pipvalor'];
                        }
                    }
                ?>
                    <tr style="height: 30px;">
                        <td align="right" valign="top" colspan="6"><b>TOTAL :</b></td>
                        <td align="center" valign="top"><input type="text" name="valortotalpi" id="valortotalpi" size="28" maxlength="" value="<? echo number_format($valortotalpi, 2, ',', '.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);" disabled  class="disabled"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : right; width:25ex;" title='' /></td>
                    </tr>
                    <tr>
                        <td align="right" colspan="7">
                            <input type="button" onclick="abrir_lista();" id="btn_selecionar_acaptres" value="Selecionar Ação/PTRES" <?php echo $disabled; ?>>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr>
            <td class='SubTituloDireita'>Enquadramento da Despesa:</td>
            <td id="comboEnquadramento">
                <?php
                    $sql = "
                        SELECT eqdid as codigo, eqdcod ||' - '|| eqddsc as descricao
                        FROM planejamento.enquadramento_despesa 
                        WHERE eqdano='" . $_SESSION['exercicio'] . "' and eqdstatus='A' 
                        ORDER BY eqdcod
                    ";
                    $db->monta_combo('eqdid', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', '', '', '240', 'S', 'eqdid');
                ?>
            </td>
        </tr>
        <tr>
            <td class='SubTituloDireita'>Nível/Etapa de Ensino:</td>
            <td>
                <?php
                    $sql = "
                        SELECT neeid as codigo, neecod ||' - '|| needsc as descricao
                        FROM monitora.pi_niveletapaensino 
                        WHERE neeano='" . $_SESSION['exercicio'] . "' and neestatus='A' 
                        ORDER BY neecod
                    ";
                    $db->monta_combo('neeid', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', '', '', '240', 'S', 'neeid');
                ?>
            </td>
        </tr>
        <tr>
            <td class='SubTituloDireita'>Categoria de Apropriação:</td>
            <td>
                <?php
                    $sql = "
                        SELECT capid as codigo, capcod ||' - '|| capdsc as descricao
                        FROM monitora.pi_categoriaapropriacao 
                        WHERE capano='" . $_SESSION['exercicio'] . "' and capstatus='A' 
                        ORDER BY capcod
                    ";
                    $db->monta_combo('capid', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', '', '', '340', 'S', 'capid');
                ?>
            </td>
        </tr>	
        <tr>
            <td align='right' class="SubTituloDireita">Codificação da Unidade(livre):</td>
            <td><? echo campo_texto('plilivre', 'S', 'S', '', 3, $max_length, '', '', null, null, null, 'id="idCodificacao"', 'atualizarCodigoLivre(); atualizarPrevisaoPI();'); ?></td>
        </tr>
            <?php if ($boModalidade) { ?>
        <tr>
            <td align='right' class="SubTituloDireita">Modalidade de Ensino / Tema / Público:</td>
                <td>
                    <?php
                        $sql = "
                            SELECT mdeid as codigo, mdecod ||' - '|| mdedsc as descricao
                            FROM monitora.pi_modalidadeensino 
                            WHERE mdeano='" . $_SESSION['exercicio'] . "' and mdestatus='A' 
                            ORDER BY mdecod
                        ";
                        $db->monta_combo('mdeid', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', '', '', '240', 'S', 'mdeid');
                    ?>
                </td>
        </tr>
            <?php } ?>
        <tr>
            <td align='right' class="SubTituloDireita">Título:</td>
            <td>
                <input type="hidden" name="plititulo_sub" id="plititulo_sub" value="<? echo $plititulo_sba; ?>">
                <?php
                    //echo campo_texto('plititulo', 'S', 'S', '', 50, 550, '', '', '', '', 0, 'id="plititulo" onKeyUp="titulopi();"');
                    echo campo_texto('plititulo', 'S', 'S', '', 50, 550, '', '', '', '', 0, 'id="plititulo"');
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" valign="top">Descrição / Finalidade:</td>
            <td><? echo campo_textarea('plidsc', 'S', 'S', '', 60, 2, 1000); ?></td>
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
                                    <td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="enquadramento"><? echo substr($planointerno['plicod'], 0, 1); ?></span></td>
                                    <td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" colspan="3"><span id="subacao">XXXX</span></td>
                                    <td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center"><span id="nivel"><? echo substr($planointerno['plicod'], 5, 1); ?></span></td>
                                    <td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="apropriacao"><? echo substr($planointerno['plicod'], 6, 2); ?></span></td>
                                    <td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="codificacao"><? echo substr($planointerno['plicod'], 8, 2); ?></span></td>		        			        			  
                                    <td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="modalidade"><? echo ($boModalidade) ? substr($planointerno['plicod'], 10, 1) : ''; ?></span></td>
                                </tr>
                                <tr>
                                    <td colspan="7" align="center"><b>Código PI / SIAFI: 
                                            <span id="enquadramento_i"><? echo substr($planointerno['plicod'], 0, 1); ?></span><span id="subacao_i">XXXX</span><span id="nivel_i"><? echo substr($planointerno['plicod'], 5, 1); ?></span><span id="apropriacao_i"><? echo substr($planointerno['plicod'], 6, 2); ?></span><span id="codificacao_i"><? echo substr($planointerno['plicod'], 8, 2); ?></span><span id="modalidade_i"><? echo substr($planointerno['plicod'], 10, 1); ?></span></b></td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                </table>

            </td>
        </tr>

        <tr bgcolor="#cccccc">
            <td colspan="2" align="right">
            <?php if ($_POST['pliid'] && $plisituacao == 'E') { ?>
                <input type="button" value="Enviar para Homologação" onclick="vincular('R', '<?php echo $_POST['pliid']; ?>')" style="cursor: pointer;"/>
            <?php } ?>
                <input type="button" class="botao" name="btg" value="Salvar" onclick="submeter('<?= $plicod ?>');" <?php echo $disabled; ?>>
                <input type="button" class="botao" name="btn" value="Novo" onclick="window.location='?modulo=principal/planotrabalhoUG/cadastro_piUG&acao=A';"></td>
        </tr>

        <!-- tr>
            <td colspan="2" class="SubTituloCentro">Lista de PIs - <? echo $db->pegaUm("SELECT UPPER(ungdsc) FROM public.unidadegestora WHERE ungcod='" . $_SESSION['monitora_var']['ungcod'] . "'"); ?></td>
        </tr -->
    </table>
</form>

<script type="text/javascript">
    
function Trim(str){ 
    return str.replace(/^\s+|\s+$/g,"");
}

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
						//window.location.reload();
						document.formulario.submit();
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

        var req = new Ajax.Request(
                window.location.href, {
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
        carregarComboEnquadramentoPorSubacao(sbaid);
        document.getElementById("sbaidAnterior").value = sbaid;
        
    }else{
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
    if( validar(pliid) ){
        if( validado ) {
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
			else if(parseFloat(replaceAll(replaceAll(tabela.rows[i].cells[6].firstChild.value,".",""),",",".")) <= 0){
				msg+="Valor do PTRES: '"+tabela.rows[i].cells[0].innerHTML+"' informado deverá ser maior que 0(zero)\n";
			}
		}
	}

	if(document.formulario.plititulo.value == '') {
		msg+="O preenchimento do campo Título é obrigatório.\n";
	}

	if(document.formulario.boModalidade.value){
		if(document.formulario.plilivre.value.length != 2){
			msg+="O preenchimento do campo Codifição da Unidade é obrigatório e igual a 2(dois dígitos).\n";
		}
	}

	if(document.formulario.plidsc.value == ''){
		msg+="O preenchimento do campo Descrição é obrigatório.\n";
	}
	
	// testa se foi selecionada o enquadramento
	if(document.getElementById("eqdid").value == ''){
		msg+="O preenchimento do campo Enquadramento da Despesa é obrigatório.\n";
	}
	if(document.getElementById("neeid").value == ''){
		msg+="O preenchimento do campo Nível/Etapa de Ensino é obrigatório.\n";
	}
	if(document.getElementById("capid").value == ''){
		msg+="O preenchimento do campo Categoria de Apropriação é obrigatório.\n";
	}
				
	if(msg != "") {
		alert(msg);
		return false;
	
	}else{
		 var pi = (
				 document.getElementById("enquadramento").innerHTML +
		 		 document.getElementById("subacao").innerHTML + 
		 		 document.getElementById("nivel").innerHTML +
		 		 document.getElementById("apropriacao").innerHTML +
		 		 document.getElementById("codificacao").innerHTML + 
		 		 document.getElementById("modalidade").innerHTML
		 );

		// limpa os espacos
		while( pi.search(' ') > -1 ){
			pi = pi.replace(' ','');
		}
		 
		 if( validapi(pi) ) {
		 	document.getElementById("plicod").value = pi; 
		 	return true;
		 } else {
		 	return false;
		 }
	}	
}

function valida2(pi) {

	if( pi.indexOf('pijaexiste') != -1 ){
		while( pi.substr(0,1) == ' ' ){
			pi = pi.substr(1,pi.length);
		}
	}else{
		while( pi.search(' ') > -1 ){
                        pi = pi.replace(' ','');
                }
	}
	
	 if(!pi || pi == ''){
		return true;
	 }
	 else{
	 	if(pi.substr(0,10) == 'pijaexiste'){
	 		pi = pi.replace('pijaexiste',"");
		 	var alertaDisplay = '<div class="titulo_box" >Atenção!</div><div class="texto_box" >Plano Interno já existe!</div><div class="conteudo_box" >Veja abaixo os dados o Plano Interno cadastrado :</div><div class="conteudo_box" >' + pi + '</div><div class="links_box" ><input type="button" onclick=\'closeMessage();return false\' value="Cancelar" /></center>';
		 	displayStaticMessage(alertaDisplay,false);
	 		return false;
	 	}
	 	var alertaDisplay = '<div class="titulo_box" >Atenção!</div><div class="texto_box" >Já existe(m) PI(s) criado(s) com esta estrutura. Veja abaixo a relação dos PI(s) encontrados:</div><div class="conteudo_box" >' + pi + '</div><div class="links_box" >Deseja realmente criar?<br><center><input type="button" onclick=\'document.formulario.submit();\' value="Confirmar" /> <input type="button" onclick=\'closeMessage();return false\' value="Cancelar" /></center>';
	 	displayStaticMessage(alertaDisplay,false);
	 	return false;
	 }
	
}

function validapi(pi) {
    var retorno = true
      , ajaxSetting = {
        method: 'post',
        parameters: 'piAjax=' + pi + '&pliid=' + document.getElementById('pliid').value,
        asynchronous: false,
        onComplete: function (res) {
            //$('divTeste').update(res.responseText);
            x = res.responseText;
            retorno = valida2(x);
        }
      }
      , req = new Ajax.Request(window.location.href, ajaxSetting);

    return retorno;	
}

function abrir_lista() {
	if(document.getElementById("sbaid").value == '') {
		alert("Selecione uma subação");
		return false;
	}
	janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUG/listarProgramaUG&acao=A&sbaid='+document.getElementById("sbaid").value, 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width='+(screen.width-120)+',height=680' ); janela.focus();
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
	} else {
		document.getElementById("enquadramento").innerHTML = ''; 
		document.getElementById("enquadramento_i").innerHTML = '';
	}
	
	if(document.getElementById('neeid').value) {
		var nivel = document.getElementById('neeid').options[document.getElementById('neeid').selectedIndex].text.split(" - ");
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

function carregarComboEnquadramentoPorSubacao(sbaid){
	var req = new Ajax.Request(window.location.href, {
        method:     'post',
        parameters: '&carregarComboEnquadramentoPorSubacao=1&sbaid='+sbaid,
        asynchronous: false,
        onComplete: function (res) {
        	if(res.responseText){
        		$('comboEnquadramento').update(res.responseText);
	        }
        }
   });
   atualizarPrevisaoPI();
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
        }else{
            var req = new Ajax.Request(window.location.href, {
                method:     'post',
                parameters: '&sbaAjax=' + document.getElementById('sbaid').value+'&capidAjax='+document.getElementById('capid').value,
                asynchronous: false,
                onComplete: function (res) {
                    //alert(res.responseText);
                    //$('divTeste').update(res.responseText);
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
    }else{
        var req = new Ajax.Request(window.location.href, {
            method:     'post',
            parameters: '&sbaAjax=' + document.getElementById('sbaid').value+'&capidAjax='+document.getElementById('capid').value,
            asynchronous: false,
            onComplete: function (res) {
                //alert(res.responseText);
                //$('divTeste').update(res.responseText);
                if(res.responseText){
                    document.getElementById("plititulo").value = "";
                    document.getElementById("plititulo_sub").value = "";
                    var dados = res.responseText;
                    dados = dados.split("!@#");
                    var apropriacao = dados[2];
                    var plititulo = Trim(dados[1]) + ' - ' + Trim(document.getElementById("plititulo").value);
                    document.getElementById("plititulo").value = Trim(plititulo.substr(0,45))+' ';
                    document.getElementById("plititulo_sub").value = Trim(plititulo.substr(0,45))+' ';
                }
            }
        });
    }
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
	janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUG/detalhafinanceiro_piUG&acao=A&ptres='+ptres, 'janela2', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width='+(screen.width-420)+',height=280' ); janela.focus();
}

function selecionarprograma(sba) {
	document.formulario.evento.value = "SUBACAO";
	document.formulario.submit();
}

messageObj = new DHTML_modalMessage();	// We only create one object of this class
messageObj.setShadowOffset(5);	// Large shadow


function displayMessage(url) {	
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

function closeMessage() {
    messageObj.close();	
}

function carregarsubacaocodigo(sbaid,boPiTitulo) {
    if (sbaid) {
        var configRequest = {
            method: 'post',
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
          },
          req = new Ajax.Request(window.location.href, configRequest);
    }
}


var boSelecionaSubacao = '<?php echo $boSelecionaSubacao; ?>';
if (boSelecionaSubacao) {
    var atiid = '<?php echo $_GET['atiid']; ?>'
      , boPiTitulo = (atiid) ? true : false
      , sbaid = '<?php echo $planointerno['sbaid']; ?>';
    
    carregarsubacaocodigo(sbaid, boPiTitulo);
}

</script>
<div id="divTeste"></div>