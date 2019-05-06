<?php
/**
 * Detalhamento do PI com histórico.
 * $Id: popuphistoricoplanointernoUN.inc 88637 2014-10-17 12:25:33Z werteralmeida $
 */

/**
 */
include "planotrabalhoUN/_constantes.php";
include "planotrabalhoUN/_funcoes.php";
require_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";
require_once APPRAIZ . "monitora/classes/Pi_PlanoInternoHistorico.class.inc";

if ($_REQUEST ['requisicao'] == 'Z') {
	
	extract ( $_POST );
	
	$retorno = false;
	
	$obPi_PlanoInterno = new Pi_PlanoInterno ( $pliid );
	$obPi_PlanoInterno->plisituacao = $situacao;
	$obPi_PlanoInterno->salvar ();
	$sql = "SELECT plicod FROM monitora.pi_planointernohistorico WHERE pliid = $pliid ORDER BY pihdata DESC LIMIT 1";
	if (! $plicodOrigem = $db->pegaUm ( $sql )) {
		$plicodOrigem = $obPi_PlanoInterno->plicod;
	}
	
	$obPi_PlanoInternoHistorico = new Pi_PlanoInternoHistorico ();
	$obPi_PlanoInternoHistorico->pliid = $pliid;
	$obPi_PlanoInternoHistorico->usucpf = $_SESSION ['usucpf'];
	$obPi_PlanoInternoHistorico->pihsituacao = $situacao;
	$obPi_PlanoInternoHistorico->plicod = $obPi_PlanoInterno->plicod;
	$obPi_PlanoInternoHistorico->plicodorigem = $plicodOrigem;
	if ($pihobs) {
		$obPi_PlanoInternoHistorico->pihobs = trim ( utf8_decode ( $pihobs ) );
	}
	$obPi_PlanoInternoHistorico->salvar ();
	
	if ($obPi_PlanoInternoHistorico->commit ()) {
		enviaEmailStatusPi ( $pliid );
		// die;
		$retorno = true;
	}
	unset ( $obPi_PlanoInterno );
	unset ( $obPi_PlanoInternoHistorico );
	
	echo $retorno;
	die ();
}

extract ( $_GET );

$obPi_PlanoInterno = new Pi_PlanoInterno ( $pliid );

if ($obPi_PlanoInterno->pliid) {
	// $obPi_PlanoInterno->pliid =1030;
	$sql = "SELECT
				pl.pliid,
				ptr.ptres,
				pt.ptrid,
				pt.pipvalor,
				ptr.acaid,
				trim(ac.prgcod||'.'||ac.acacod||'.'||ac.unicod||'.'||ac.loccod||' - '||ac.acadsc) as descricao,
				sum(ptr.ptrdotacao) as dotacaoinicial,
				round(sum( coalesce(sad.sadvalor,0) ),2) as dotacaosubacao,
				coalesce((SELECT SUM(pipvalor) as valor FROM monitora.pi_planointernoptres pt2 inner join monitora.pi_planointerno p ON p.pliid = pt2.pliid WHERE pt.ptrid = pt2.ptrid AND p.plistatus='A'),0) as detalhamento
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
					pl.pliid = '" . $obPi_PlanoInterno->pliid . "' AND
					pl.plistatus='A'
	    	GROUP BY pl.pliid, pt.ptrid, ptr.ptres, pl.plistatus, pt.pipvalor, ac.prgcod, ptr.acaid, ac.acacod, ac.unicod, ac.loccod, ac.acadsc
	    	ORDER BY ptr.ptres";
	
	$acoespl = $db->carregar ( $sql );
}
$acoespl = ($acoespl) ? $acoespl : array ();

?>
<html>
<head>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
<style type="text/css">
button{cursor:pointer}
</style>
</head>
<br>
<form name="formulario" id="formulario" method="post">
	<table bgcolor="#f5f5f5" align="center" class="tabela">
		<tr>
			<td class="subtitulodireita" colspan="2">
				<center>
					<h3>Dados do plano interno</h3>
				</center>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Enquadramento da Despesa:</td>
			<td><?php
			if ($obPi_PlanoInterno->eqdid) {
				echo $db->pegaUm ( "SELECT eqdcod || ' - ' || eqddsc FROM planejamento.enquadramento_despesa WHERE eqdid='" . $obPi_PlanoInterno->eqdid . "'" );
			}
			
			?>
		</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Subação:</td>
			<td><?php
			if ($obPi_PlanoInterno->sbaid) {
				echo $db->pegaUm ( "SELECT sbacod || ' - ' || sbatitulo FROM monitora.pi_subacao WHERE sbaid='" . $obPi_PlanoInterno->sbaid . "'" );
			}
			?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Nível/Etapa de Ensino:</td>
			<td><?php
			
			echo $db->pegaUm ( "SELECT neecod || ' - ' || needsc FROM monitora.pi_niveletapaensino WHERE neeid='" . $obPi_PlanoInterno->neeid . "'" );
			?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Categoria de Apropriação:</td>
			<td><?php echo $db->pegaUm("SELECT capcod || ' - ' || capdsc FROM planejamento.categoria_apropriacao WHERE capid='".$obPi_PlanoInterno->capid."'"); ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Codifição da Unidade(livre):</td>
			<td><?php echo $obPi_PlanoInterno->plilivre; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Modalidade de Ensino/Tema/Público:</td>
			<td><?php
			if ($obPi_PlanoInterno->mdeid) {
				echo $db->pegaUm ( "SELECT mdecod || ' - ' || mdedsc FROM planejamento.area_cultural WHERE mdeid='" . $obPi_PlanoInterno->mdeid . "'" );
			}
			?>
		</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Título:</td>
			<td><?php echo $obPi_PlanoInterno->plititulo; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Código do PI:</td>
			<td><?php echo $obPi_PlanoInterno->plicod; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Descrição do PI:</td>
			<td><?php echo $obPi_PlanoInterno->plidsc; ?></td>
		</tr>
		</tr>
		<tr>
			<td class="subtitulodireita">Unidade Orçamentária:</td>
			<td><?php
			
			$ug = $db->pegaUm ( "SELECT  unicod || ' - ' || unidsc FROM public.unidade WHERE unicod = '" . $obPi_PlanoInterno->unicod . "'" );
			if ($ug == null || $ug == '') {
				$ugS = $db->pegaUm ( "SELECT  unicod || ' - ' || unidsc FROM public.unidade WHERE unicod = (SELECT unicod FROM public.unidadegestora WHERE ungcod = '" . $obPi_PlanoInterno->ungcod . "')" );
				if ($ugS == null || $ugS == '') {
					$ugS = " - ";
					echo $ugS;
				}
			} else {
				echo $ug;
			}
			?></td>
		</tr>
		</tr>
		<tr>
			<td class="subtitulodireita">Unidade Gestora:</td>
			<td><?php
			
			$ug = $db->pegaUm ( "SELECT ungcod || ' - ' || ungdsc FROM public.unidadegestora WHERE ungcod = '" . $obPi_PlanoInterno->ungcod . "'" );
			if ($ug == null || $ug == '') {
				$ug = " - ";
			}
			echo $ug;
			?></td>
		</tr>
		<tr>
			<td colspan="2">
		<?php
		$sql = <<<DML
SELECT TO_CHAR(pih.pihdata, 'DD/MM/YYYY HH24:MI:SS') AS pihdata,
       '<center>' || CASE WHEN pih.pihsituacao = 'P' THEN ' Pendente '
                          WHEN pih.pihsituacao = 'A' THEN ' Aprovado '
                          WHEN pih.pihsituacao = 'R' THEN ' Revisado '
                          WHEN pih.pihsituacao = 'C' THEN ' Cadastrado no SIAFI '
                          WHEN pih.pihsituacao = 'E' THEN ' Enviado para Revisão '
                          WHEN pih.pihsituacao = 'H' THEN ' Homologado '
                       END || '</center>' AS situacao,
       COALESCE(pih.pihobs, '<center>-</center>') AS pihobs,
       u.usunome
  FROM monitora.pi_planointernohistorico pih
    INNER JOIN seguranca.usuario u ON pih.usucpf = u.usucpf
  WHERE pliid = {$obPi_PlanoInterno->pliid}
  ORDER BY pih.pihdata DESC
DML;
		
		$sql2 = "SELECT plisituacao FROM monitora.pi_planointerno WHERE pliid = $obPi_PlanoInterno->pliid ";
		$ultimaSituacao = $db->pegaUm ( $sql2 );
		
		$cabecalho = array (
				"Data Hora",
				"Situação",
				"Observação",
				"Cadastrado Por" 
		);
		$db->monta_lista_simples ( $sql, $cabecalho, 100, 30, 'N', '100%', 'N' );
		?>
		</td>
		</tr>
		<tr bgcolor="#C0C0C0">
                <? 
                
                ?>	
                    <td align="left" colspan="2"> 
		<?php
		/* Não exibe nenum botão para o perfil Gabinete */
                $perfis = pegaPerfilGeral();
                if (!in_array(1063, $perfis)) {
                    $sql = <<<DML
                    SELECT pli.unicod
                      FROM monitora.pi_planointerno pli
                      WHERE pli.pliid = {$obPi_PlanoInterno->pliid}
DML;

                    $arUOdoPI = $db->carregar ( $sql );
                    $arUOdoPI = ($arUOdoPI) ? $arUOdoPI : array ();

                    $unicodsPI = array ();
                    foreach ( $arUOdoPI as $ptres ) {
                            array_push ( $unicodsPI, $ptres ['unicod'] );
                    }

                    $arUOdoPTRES = array ();
                    if (in_array ( '26101', $unicodsPI )) {
                            $sql = <<<DML
                    SELECT unicod
                      FROM monitora.pi_planointernoptres pip
                        INNER JOIN monitora.ptres p ON pip.ptrid = p.ptrid
                      WHERE pip.pliid = {$obPi_PlanoInterno->pliid}
DML;
                            $arPtres = $db->carregar ( $sql );
                            $arPtres = ($arPtres) ? $arPtres : array ();
                            foreach ( $arPtres as $ptres ) {
                                    array_push ( $arUOdoPTRES, $ptres ['unicod'] );
                            }
                    }

                    if (! $_GET ['sl']) {
                            $perfisUsuario = pegaPerfilGeral ( $_SESSION ['usucpf'], $_REQUEST ['sisid'] );
                            $pflcod = 1044; // -- www/planacomorc/_constantes.php::PFL_GESTAO_ORCAMENTARIA
                            $sql = <<<DML
    SELECT uni.unicod AS codigo
      FROM planacomorc.usuarioresponsabilidade ur
        INNER JOIN public.unidade uni USING(unicod)
      WHERE ur.usucpf = '{$_SESSION['usucpf']}'
        AND ur.pflcod = '{$pflcod}'
        AND ur.rpustatus = 'A'
DML;
                            $usuarioresponsabilidade = $db->carregar ( $sql );
                            $usuarioresponsabilidade = $usuarioresponsabilidade ? $usuarioresponsabilidade : array ();
                            foreach ( $usuarioresponsabilidade as &$ur ) {
                                    $ur = $ur ['codigo'];
                            }
                            switch ($ultimaSituacao) {
                                    case 'P' : // -- Pendente
                                            botaoEnviarRevisao ();
                                            break;
                                    case 'H' : // -- Homologação
                                            botaoEnviarRevisao ();
                                            botaoAprovar ( $unicodsPI, $perfisUsuario, $usuarioresponsabilidade, $pflcod, $arUOdoPTRES );
                                            break;
                                    case 'E' : // -- Enviado para Revisão
                                            botaoTornarPendente ();
                                            break;
                                    case 'A' : // -- Aprovado
                                            botaoCadastrarSIAFI ();
                                            break;
                                    case 'T' : // -- Atualizar no SIAFI - Setado pelo sistema
                                            botaoAtualizarSIAFI ($pliid);
                                            break;
                                    case 'C' ://Cadastrado no SIAFI -- adicionado conforme demanda Cod 240042
                                            botaoEnviarRevisao ();
                                            break;
                            }
                    }
		}
                ?>
		
                    </td>
                
		</tr>
		<tr>
			<td align="center" colspan="2">
			<?php echo $unidade = $db->pegaUm("SELECT unicod || ' - ' || unidsc FROM public.unidade WHERE unicod = '".$_SESSION['monitora_var']['unicod']."'"); ?>
		</td>
		</tr>
		<tr>
			<td colspan="2">
				<table cellpadding="0" border="0" width="98%" align="center"
					id="orcamento"
					style="BORDER-RIGHT: #C9C9C9 1px solid; BORDER-TOP: #C9C9C9 1px solid; BORDER-LEFT: #C9C9C9 1px solid; BORDER-BOTTOM: #C9C9C9 1px solid;"
					onmouseover="tabindexcampo();">
					<tr>
						<td style="background-color: #C9C9C9;" colspan="7" align="center"><b>Detalhamento
								Orçamentário</b></td>
					</tr>
					<tr>
						<td style="background-color: #C9C9C9;" align="center" nowrap><b>PTRES</b><input
							type="hidden" name="pliptres"></td>
						<td style="background-color: #C9C9C9; width: 45%;" align="center"
							nowrap><b>Ação</b></td>
						<td style="background-color: #C9C9C9; width: 100px;"
							align="center" nowrap><b>Dotação Inicial</b></td>
						<td style="background-color: #C9C9C9; width: 100px;"
							align="center" nowrap><b>Dotação Subação</b></td>
						<td style="background-color: #C9C9C9; width: 100px;"
							align="center" nowrap><b>Detalhado no PI</b></td>
						<td style="background-color: #C9C9C9; width: 100px;"
							align="center"><b>Dotação Disponível</b></td>
						<td style="background-color: #C9C9C9;" align="center"><b>Valor
								Previsto(Anual)</b></td>
					</tr>
				<?
				
				if ($acoespl [0]) {
					$valortotalpi = 0;
					$cor = 0;
					foreach ( $acoespl as $acpl ) {
						?>
			        <tr style="height:30px;<? echo (($cor%2)?"":"background-color:#DCDCDC;"); ?>" id="ptres_<? echo $acpl['ptres']; ?>">
						<td align="center"><? echo $acpl['ptres']; ?></td>
						<td align="left"><? echo $acpl['descricao']; ?></td>
						<td align="right"><? echo number_format($acpl['dotacaoinicial'],2,',','.'); ?></td>
						<td align="right"><? echo number_format($acpl['dotacaosubacao'],2,',','.'); ?></td>
						<td align="right"><? echo number_format($acpl['detalhamento'],2,',','.'); ?></td>
						<td align="right"><? echo number_format(($acpl['dotacaoinicial'] - $acpl['detalhamento']),2,',','.'); ?></td>
						<td align="center"><input type="text"
							name="plivalored[<? echo $acpl['ptrid']; ?>]" size="28"
							maxlength=""
							value="<? echo number_format($acpl['pipvalor'],2,',','.'); ?>"
							onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);calculovalorPI();"
							disabled class="disabled" onmouseover="MouseOver(this);"
							onfocus="MouseClick(this);this.select();"
							onmouseout="MouseOut(this);"
							onblur="MouseBlur(this); verificaDisponivel(this,'<? echo $acpl['ptres']; ?>','<? echo number_format($acpl['pipvalor'],2,',','.'); ?>');"
							style="text-align: right; width: 25ex;" title='' /></td>
					</tr>
				<?
						$cor ++;
						$valortotalpi = $valortotalpi + $acpl ['pipvalor'];
					}
				}
				?>
				<tr style="height: 30px;">
						<td align="right" valign="top" colspan="6"><b>TOTAL :</b></td>
						<td align="center" valign="top"><input type="text"
							name="valortotalpi" id="valortotalpi" size="28" maxlength=""
							value="<? echo number_format($valortotalpi,2,',','.'); ?>"
							onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);"
							disabled class="disabled" onmouseover="MouseOver(this);"
							onfocus="MouseClick(this);this.select();"
							onmouseout="MouseOut(this);" onblur="MouseBlur(this);"
							style="text-align: right; width: 25ex;" title='' /></td>
					</tr>
				</table>
			</td>
		</tr>

		</form>
	</table>
	<link rel="stylesheet"
		href="/includes/ModalDialogBox/modal-message.css" type="text/css"
		media="screen" />
	<script type="text/javascript"
		src="../includes/ModalDialogBox/modal-message.js"></script>
	<script type="text/javascript"
		src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
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
				'style="width:75ex;" '+
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
	var alertaDisplay = '<center><div class="titulo_box" >É necessário colocar observação.<br/ >'+campoTextArea+'</div>'
            //+ '<div class="links_box" ><br><input type="button" onclick="vincular(\'E\',1)" value="Gravar" />'
            + '<input type="button" onclick=\'closeMessage(); return false \' value="Cancelar" /></center>';
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

function atualizadoSiafi(pliid){
    var url = window.location.href;
    var parametros = "&requisicao=piAtualizadoSiafi&pliid="+pliid;
    jQuery.ajax({
        url: url+parametros,
        async: false,
        success: function(html) {
            alert('Dados atualizados com Sucesso.\n'+html);
        }
    });
    //Reload na página
    window.location = url;
}

</script>
	<div id="dibDebug"></div>

<?php
function botaoEnviarRevisao() {
	?>
<?php
}
function botaoTornarPendente() {
	?>
<?php
}

/**
 * Exibe o botão de aprovar de acordo com as permissões do usuário.
 *
 * @param array $unicodPI
 *        	UO de quem solicitou o PI
 * @param array $perfis        	
 * @param array $unicodsResponsabilidade        	
 * @param type $pflCode        	
 * @param type $arUOdoPTRES        	
 */
function botaoAprovar(array $unicodPI, array $perfis, array $unicodsResponsabilidade, $pflCode, $arUOdoPTRES) {
	$podeAprovar = false;
	if (1 == $_SESSION ['superuser']) {
		$podeAprovar = true;
	} elseif (in_array ( $pflCode, $perfis )) { // -- Verifica se é o perfil que pode aprovar PIs
		if (1 == count ( $unicodPI ) && in_array ( array_pop ( $unicodPI ), $unicodsResponsabilidade )) {
			// -- Se a ÚNICA UO do PI estiver dentro do conjunto UO-RESPONSABILIDADE, deixa aprovar o PI
			$podeAprovar = true;
		}
	}
	
	if ($podeAprovar) {
		?>

    <?php
	}
}
function botaoCadastrarSIAFI() {
	?>
<?php
}
function botaoAtualizarSIAFI($pliid) {
	?>
<?php
}

/**
 * Remove a UO 26101 de uma lista de UOs.
 *
 * @param array $uos
 *        	Lista de UOs alterado por referência.
 */
function remove26101(&$uos) {
	function check($uo) {
		return $uo != '26101';
	}
	
	$uos = array_filter ( $uos, check );
}