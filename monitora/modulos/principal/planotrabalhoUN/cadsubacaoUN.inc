<?php 
require_once APPRAIZ . "monitora/classes/Pi_Subacao.class.inc";
require_once APPRAIZ . "monitora/classes/Pi_SubacaoDotacao.class.inc";
require_once APPRAIZ . "monitora/classes/Pi_SubacaoHistorico.class.inc";
require_once APPRAIZ . "monitora/www/planotrabalhoUN/_constantes.php";
require_once APPRAIZ . "monitora/www/planotrabalhoUN/_funcoes.php";
include_once APPRAIZ . "monitora/www/planotrabalhoUN/unidade_atividade_funcoes.php";
$perfis = pegaPerfilGeral();

if($_REQUEST['requisicao'] == 'vincular'){
	
	extract($_POST);
	
	$retorno = false;
	$pi_SubacaoHistorico = new Pi_SubacaoHistorico();
	$pi_SubacaoHistorico->sbaid = $sbaid;
	$pi_SubacaoHistorico->usucpf = $_SESSION['usucpf'];
	$pi_SubacaoHistorico->sahsituacao = $situacao;
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

if($_POST['requisicao'] == 'existeCod'){
	extract($_POST);
	if($sbaid) {
		$existe_sbaid = $db->pegaUm("SELECT sub.sbaid FROM monitora.pi_subacao sub
									 LEFT JOIN monitora.pi_subacaounidade sub2 ON sub.sbaid = sub2.sbaid 
									 WHERE sub.sbaid != '".$sbaid."' AND sub.sbacod = '".$sbacod."' AND sub2.unicod = '".$_SESSION['monitora_var']['unicod']."'");
		$existe_sbaplanotrabalho = $db->pegaUm("SELECT sbaplanotrabalho FROM monitora.pi_subacao WHERE sbaid = '".$sbaid."' AND sbastatus = 'A'");
		$existe_sbaplanotrabalho = ($existe_sbaplanotrabalho == 't') ? true : false; 
	} else {
		$existe_sbaid = $db->pegaUm("SELECT sub.sbaid FROM monitora.pi_subacao sub
									 LEFT JOIN monitora.pi_subacaounidade sub2 ON sub.sbaid = sub2.sbaid 
									 WHERE sub.sbacod = '".$sbacod."' AND sub2.unicod = '".$_SESSION['monitora_var']['unicod']."' AND  sbastatus = 'A'");
	}

	if($existe_sbaid) {
		echo 1;
		die;
	}
	
	echo 0; die;
}

if($_POST['acaoForm']){
	extract($_POST);
	if($sbaid) {
		$existe_sbaplanotrabalho = $db->pegaUm("SELECT sbaplanotrabalho FROM monitora.pi_subacao WHERE sbaid = '".$sbaid."' AND sbastatus = 'A'");
		$existe_sbaplanotrabalho = ($existe_sbaplanotrabalho == 't') ? true : false;		
	}
	
	$msg = "";
	if($sbaplanotrabalho && !$existe_sbaplanotrabalho){
		// obtém dados da atividade vinculada à ação
		$atividade = retornaTarefaUnidade( $_SESSION['monitora_var']['unicod'], $_SESSION['exercicio'] );
		$subacao = array("subatv"    => true,
						 "sbaid"     => $sbaid,
						 "sbatitulo" => $sbatitulo,
						 "sbacod"    => $sbacod,
						 "sbaobras"  => $sbaobras,
						 "sbadsc"    => $sbadsc);
		
		if ( ! $sbaid = atividade_inserir( $atividade['atiid'], $titulo, $subacao, false ) ) {
			die("<script>alert('Ocorreu um erro, por favor tente mais tarde.');window.location='monitora.php?modulo=principal/planotrabalhoUN/cadsubacaoUN&acao=A';</script>");
		}
		// atualiza projeto da sessão
		$sql = sprintf( "select _atiprojeto from pde.atividade where atiid = %d", $atividade['atiid'] );
		$_SESSION['projeto'] = (integer) $db->pegaUm( $sql );
		// atualiza dados da árvore
		atividade_calcular_dados( $atividade['atiid'] );
	} elseif(!$sbaplanotrabalho && $existe_sbaplanotrabalho){
		$atividade 			  = retornaTarefaUnidade( $_SESSION['monitora_var']['unicod'], $_SESSION['exercicio'] );
		
		$atiid = $db->pegaUm("select a.atiid from pde.atividade a
								inner join monitora.pi_subacaoatividade sa on a.atiid = sa.atiid
							  where a.atistatus = 'A' AND a.unicod = '{$_SESSION['monitora_var']['unicod']}' AND atiidpai = {$atividade['atiid']} AND sa.sbaid = $sbaid");
		
		$existeAtividadeFilha = $db->pegaUm("select count(1) from pde.atividade where atiidpai = {$atiid} AND atistatus = 'A'");
		if($existeAtividadeFilha){
			$msg = 'O campo Migrar Plano de Trabalho não pode ser alterado pois existem Atividades relacionadas.\n\n';
		}
		
		$existePiVinculado = $db->pegaUm("select count(1) from monitora.pi_planointerno where sbaid = {$sbaid} AND plistatus = 'A'");
		if($existePiVinculado){
			$msg .= 'O campo Migrar Plano de Trabalho não pode ser alterado pois existem Planos Interno relacionadas.';
		}
		
		if(!$msg){
			$sql = "UPDATE pde.atividade SET atistatus='I' WHERE atiid='".$atiid."'";
			$db->executar($sql);

			$obPi_Subacao = new Pi_Subacao($sbaid);
			$obPi_Subacao->deletaSubacaoAtividade( $sbaid );
			$obPi_Subacao->sbadsc = $sbadsc;
			$obPi_Subacao->sbatitulo = $sbatitulo;
			$obPi_Subacao->sbacod = $sbacod;
			$obPi_Subacao->sbaobras = $sbaobras == true ? 't' : 'f';
			$obPi_Subacao->sbaplanotrabalho = 'false';
			$obPi_Subacao->usucpf = $_SESSION['usucpf'];
			$obPi_Subacao->sbaobras = $sbaobras;
			$obPi_Subacao->salvar();
		} else {
			$obPi_Subacao = new Pi_Subacao($sbaid);
			$obPi_Subacao->sbadsc = $sbadsc;
			$obPi_Subacao->sbatitulo = $sbatitulo;
			$obPi_Subacao->sbacod = $sbacod;
			$obPi_Subacao->sbaobras = $sbaobras == true ? 't' : 'f';
			$obPi_Subacao->usucpf = $_SESSION['usucpf'];
			$obPi_Subacao->sbaobras = $sbaobras;
			$obPi_Subacao->sbaplanotrabalho = 'false';
			$obPi_Subacao->salvar();
		}
	} else {
		$obPi_Subacao = new Pi_Subacao($sbaid);
		$obPi_Subacao->sbadsc = $sbadsc;
		$obPi_Subacao->sbatitulo = $sbatitulo;
		$obPi_Subacao->sbacod = $sbacod;
		$obPi_Subacao->sbaobras = $sbaobras == true ? 't' : 'f';
		$obPi_Subacao->sbaplanotrabalho = $sbaplanotrabalho;
		$obPi_Subacao->usucpf = $_SESSION['usucpf'];
		$obPi_Subacao->sbaobras = $sbaobras;
		//ver($obPi_Subacao,d);
		$obPi_Subacao->salvar();
		$sbaid = $obPi_Subacao->sbaid;
	}
	
	if($sbaid){
		if($sbaplanotrabalho){
			$obPi_Subacao = new Pi_Subacao($sbaid);
			$obPi_Subacao->sbaplanotrabalho = $sbaplanotrabalho;
			$obPi_Subacao->salvar();
		}
		$obPi_Subacao->deletaSubacaoDotacao( $sbaid );		
	}
	$obPi_SubacaoDotacao = new Pi_SubacaoDotacao();
	$obPi_SubacaoDotacao->sbaid = $sbaid;
	$obPi_SubacaoDotacao->ptrid = $ptrid;
	$obPi_SubacaoDotacao->sadvalor = $sadvalor;
	$obPi_SubacaoDotacao->salvar($sbaid,$ptrid,$valor);
	
	if($sbaid){
		$db->executar("DELETE FROM monitora.pi_subacaounidade WHERE sbaid = $sbaid");
	}
	
	$db->executar("INSERT INTO monitora.pi_subacaounidade(sbaid, unicod, unitpocod) VALUES ('".$sbaid."', '".$_SESSION['monitora_var']['unicod']."', 'U');");
	$db->commit();

	if($msg){
		echo "<script>alert('$msg');</script>";
	}
	
	$_REQUEST['acao'] = 'A';
	$db->sucesso("principal/planotrabalhoUN/cadsubacaoUN","&sbaid=".$sbaid);
	unset($obPi_Subacao);
	unset($obPi_SubacaoDotacao);
	die;
}

include APPRAIZ."includes/cabecalho.inc";

echo '<br/>';

echo montarAbasArray(carregardadosplanotrabalhoUN_raiz(), "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadsubacaoUN&acao=A");

// Cria o título da tela
$titulo_modulo = "Cadastro de Subação";
monta_titulo( $titulo_modulo, "");

if($_GET['sbaid']){
	$subacao = $db->pegaUm("SELECT sbaid FROM monitora.pi_subacao WHERE sbaid = {$_GET['sbaid']} AND sbastatus = 'A'");
	if( !$subacao ){
		echo "<script>
				alert('A Subação informada não existe!');
				history.back(-1);
			  </script>";
		die;
	}
}

if($_GET['sbaidExcluir']){
	$existePiVinculado = $db->pegaUm("select count(1) from monitora.pi_planointerno where sbaid = {$_GET['sbaidExcluir']} AND plistatus = 'A'");
	if($existePiVinculado){
		echo "<script>
				alert('Subação não pode ser excluida, pois existe pi vinculado para essa subação.');
				history.back(-1);
			  </script>";
		die;
	} else {
		$obPi_Subacao = new Pi_Subacao($_GET['sbaidExcluir']);
		$obPi_Subacao->deletaSubacaoUnidade( $_GET['sbaidExcluir'] );
		$obPi_Subacao->deletaSubacaoDotacao( $_GET['sbaidExcluir'] );
		$obPi_Subacao->deletaSubacaoAtividade( $_GET['sbaidExcluir'] );
		$obPi_Subacao->deletaSubacaoHistorico( $_GET['sbaidExcluir'] );
		//$obPi_Subacao->excluir($_GET['sbaidExcluir']);
		$obPi_Subacao->sbastatus = 'I';
		$obPi_Subacao->salvar();	
		$obPi_Subacao->commit();
		unset($_GET['sbaidExcluir']);
		unset($obPi_Subacao);
		$_REQUEST['acao'] = 'A';
		$db->sucesso("principal/planotrabalhoUN/cadsubacaoUN");		
	}
	die;
}

$obPi_Subacao = new Pi_Subacao($_GET['sbaid']);

if($obPi_Subacao->sbaid){
	$sql_lista = "SELECT
					dtl.ptrid,
					dtl.ptres,
					trim(aca.prgcod||'.'||aca.acacod||'.'||aca.unicod||'.'||aca.loccod||' - '||aca.acadsc) as descricao,
					uni.unidsc,
					SUM(dtl.ptrdotacao) as dotacaoinicial,
					coalesce(cast(SUM(dt.valor) as varchar),'0.00') as detalhamento,
					coalesce(cast(SUM(dt2.valorpi) as varchar),'0.00') as detalhamento2,
					sum(sad.sadvalor) as valorcadastrado 
				FROM planejamento.acao aca
					INNER JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
					INNER JOIN public.unidade uni ON uni.unicod = dtl.unicod
					INNER JOIN monitora.pi_subacaodotacao sad ON dtl.ptrid = sad.ptrid
					LEFT JOIN ( select ptrid,
								sum( sadvalor ) as valor
								from  monitora.pi_subacaodotacao
								group by ptrid ) dt ON dtl.ptrid = dt.ptrid 
					LEFT JOIN ( select sbaid, ptrid, sum(dtl.valorpi) as valorpi from monitora.v_pi_detalhepiptres dtl group by sbaid, dtl.ptrid ) dt2 ON dtl.ptrid = dt2.ptrid and dt2.sbaid = sad.sbaid
					WHERE aca.prgano = '".$_SESSION['exercicio']."' 
						  and aca.unicod = '".$_SESSION['monitora_var']['unicod']."'
					      and aca.acasnrap = false
					      and sad.sbaid = ".$obPi_Subacao->sbaid."
					GROUP BY dtl.ptrid, dtl.ptres,
								trim(aca.prgcod||'.'||aca.acacod||'.'||aca.unicod||'.'||aca.loccod||' - '||aca.acadsc) ,
								uni.unidsc";
	//ver($sql_lista);
	$ptresacao = $db->carregar($sql_lista);
}
$ptresacao = ($ptresacao) ? $ptresacao : array();

?>
<form method="post" name="formulario" id="formulario" action="">
<input type="hidden" name="acaoForm" id="acaoForm" value="1" />
<input type="hidden" name="sbaid" id="sbaid" value="<? echo $obPi_Subacao->sbaid; ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita">Título:</td>
			<td>
			<?php
				$sbatitulo = $obPi_Subacao->sbatitulo; 
				echo campo_texto( 'sbatitulo', 'S', $habilitado, 'Título', 30, 255, '', '','','','','id="sbatitulo"'); 
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Código:</td>
			<td>
			<?php
			 	$sbacod = $obPi_Subacao->sbacod;
				echo campo_texto( 'sbacod', 'S', $habilitado, 'Código', 10, 4, '', '','','','','id="sbacod"', "this.value=this.value.toUpperCase();"); 
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Unidade Orçamentaria:</td>
			<td><?php echo $db->pegaUm("SELECT unidsc FROM public.unidade WHERE unicod='".$_SESSION['monitora_var']['unicod']."'"); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Descrição / Funcionalidade:</td>
			<td>
				<?php 
					$sbadsc = $obPi_Subacao->sbadsc;
					echo campo_textarea( 'sbadsc', 'N', 'S', 'Descrição ', 65 , 5, 500, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Descrição' ); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Permitir Obras?</td>
			<td><input type="checkbox" name="sbaobras" value="1" <?= $obPi_Subacao->sbaobras == 't' ? 'checked="checked"' : '' ?> /></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Migrar para o Plano de Trabalho?</td>
			<td><input type="checkbox" name="sbaplanotrabalho" value="1" <?= $obPi_Subacao->sbaplanotrabalho == 't' ? 'checked="checked"' : '' ?> /></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Ações:</td>
		    <td >
		        <table cellpadding="0" border="0" width="98%"  id="orcamento"  style="BORDER-RIGHT: #C9C9C9 1px solid; BORDER-TOP: #C9C9C9 1px solid; BORDER-LEFT: #C9C9C9 1px solid; BORDER-BOTTOM: #C9C9C9 1px solid;">
				<tr>
				<td style="background-color: #C9C9C9;" colspan="7" align="center"><b>Detalhamento Orçamentário</b></td>
				</tr>
		        <tr>
				    <td style="background-color: #C9C9C9;" align="center" nowrap><b>PTRES</b><input type="hidden" name="ptrid"></td>
					<td style="background-color: #C9C9C9; width:45%;" align="center" nowrap><b>Ação</b></td>
				    <td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Dotação Autorizada</b></td>
				    <td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Detalhado em Subação</b></td>
				    <td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Detalhado em PI</b></td>
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
				    <td align="right"><a href="javascript:detfin('<?=$acpl['ptrid']?>')"><? echo number_format($acpl['detalhamento'],2,',','.'); ?></a></td>
				    <td align="right"><? echo number_format($acpl['detalhamento2'],2,',','.'); ?></td>
				    <td align="right"><? echo number_format(($acpl['dotacaoinicial']-$acpl['detalhamento']),2,',','.'); ?></td>
				    <td align="center"><input type="text" name="valor[<? echo $acpl['ptrid']; ?>]" size="28" maxlength="" value="<? echo number_format($acpl['valorcadastrado'],2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);calculovalorPI();"  class="normal"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this); verificaDisponivel(this,'<?=$acpl['ptrid']?>','<?=number_format($acpl['valorcadastrado'],2,',','.')?>');" style="text-align : right; width:25ex;" title='' /></td>
				</tr>
				<? 
						$cor++;
						$valortotal = $valortotal + $acpl['valorcadastrado']; 
					}
				} 
				?>
		        <tr style="height: 30px;">
					<td align="right" valign="top" colspan="6"><b>TOTAL :</b></td>
					<td align="center" valign="top"><input type="text" name="valortotal" id="valortotal" size="28" maxlength="" value="<? echo number_format($valortotal,2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);" disabled  class="disabled"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : right; width:25ex;" title='' /></td>
				</tr>
		        <tr>
					
				</tr>
		        </table>
		    </td>
		</tr>
	</table>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-top:none">
		<tr>
			<td class="SubTituloDireita" colspan="2" style="text-align:center">
			<?php if($obPi_Subacao->sbaid && $obPi_Subacao->sbasituacao == 'E'){ ?>
				
			<?php } ?>
			<?php //if(!boPerfilSomenteLeitura()) { 
                        if (in_array(PERFIL_MONIT_SUPER_USUARIO, $perfis)){?>
				
			<?php }
                        if (in_array(PERFIL_MONIT_SUPER_USUARIO, $perfis)){?>
				
                        <?php } ?></td>
		</tr>
	</table>
</form>
<?php
if(boPerfilSubacao()) {
	$btExluir = "<a style=\"margin: 0 -5px 0 5px;\" style=\"cursor:hand\" href=\"#\" onclick=\"excluirSubacao(' || s.sbaid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>";
} else {
	$btExluir = "<a style=\"margin: 0 -5px 0 5px;\" style=\"cursor:hand\" href=\"#\" \"><img src=\"/imagens/excluir_01.gif\" border=0 title=\"Excluir\"></a></center>";	
}

$sql = "select
			'<center><a href=\"monitora.php?modulo=principal/planotrabalhoUN/cadsubacaoUN&acao=A&sbaid='|| s.sbaid ||'\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\"></a>
			$btExluir' as acao,	
			'<center>'||sbacod||'</center>', 
			sbatitulo,
			to_char(sbadata, 'DD/MM/YYYY HH24:MI:SS') as sbadata, 
			CASE WHEN sbasituacao = 'P' THEN ' <font color=\"red\">Pendente de Aprovação</font> '
				 WHEN sbasituacao = 'A' THEN ' <font color=\"green\">Aprovado</font> '
				 WHEN sbasituacao = 'R' THEN ' <font color=\"#3F85FF\">Revisado</font> '
				 WHEN sbasituacao = 'E' THEN ' <font color=\"#EAC117\">Enviado para Revisão</font> ' 
			END as situacao
		from monitora.pi_subacao s
		left join monitora.pi_subacaounidade su ON su.sbaid = s.sbaid 
		where su.unicod='".$_SESSION['monitora_var']['unicod']."'";

	$cabecalho = array("Ação","Código","Título","Data","Situação");
	$db->monta_lista($sql,$cabecalho,200,10,'N','center','');
echo "<br />";
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script>
function enviaForm(pliid) {
	if(validar()) {
//		selectAllOptions( document.formulario.eqdid );
//		selectAllOptions( document.formulario.entid );
//		selectAllOptions( document.formulario.niveletapa );
//		selectAllOptions( document.formulario.categoria );
//		selectAllOptions( document.formulario.modalidade );
		document.formulario.submit();
	}
}
function abrir_lista(){
	var janela = window.open('?modulo=principal/planotrabalhoUN/popupcadsubacaoUN&acao=A', 'blank', 'height=600,width=900,status=no,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	janela.focus();
}

function validar(){	
	var msg = "";

//	var tabela = document.getElementById('orcamento');
//	
//	if(tabela.rows.length != 4) {
//		for(i=2;i<(tabela.rows.length-2);i++) {
//			alert(tabela.rows[i].cells[6].innerHTML+'\n'+tabela.rows[i].cells[6].firstChild.value);
//			if(!tabela.rows[i].cells[6].firstChild.value) {
//				//alert(tabela.rows[i].cells[0].innerHTML)
//				msg+="Valor do PTRES: '"+tabela.rows[i].cells[0].innerHTML+"' é obrigatório.\n";
//			}
//		}
//	}

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
	

	if(document.formulario.sbatitulo.value == '') {
		msg+="O preenchimento do campo Título é obrigatório.\n";
	}
	
	var sbacod = document.getElementById('sbacod');
	if(sbacod.value.length < 4){
		msg+='O Código deve ter 4 caracteres.\n';
	}

	var url = window.location.href;
	var parametros = "requisicao=existeCod&sbaid="+document.getElementById('sbaid').value+'&sbacod='+sbacod.value;
	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(r) {
				//alert(r.responseText);
				//$('dibDegug').update(r.responseText);
				if(r.responseText == 1){
					msg += 'Código da subação encontra-se cadastrado.';
				}
			}
		}
	);

	if(msg != ""){
		alert(msg);
		return false;
	}else return true;	
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
		document.getElementById('valortotal').value = tot.toFixed(2);
	} else {
		document.getElementById('valortotal').value = Arredonda(tot,2);
	}
	document.getElementById('valortotal').onkeyup();
}

function verificaDisponivel(campo, ptrid, vlold) {
	var linha = document.getElementById('ptrid_'+ptrid);

	valordisp = parseFloat(replaceAll(replaceAll(linha.cells[5].innerHTML,".",""),",","."));
	valoratual = parseFloat(replaceAll(replaceAll(campo.value,".",""),",","."));

	if(valoratual>(valordisp+parseFloat(replaceAll(replaceAll(vlold,".",""),",",".")))) {
		alert('Valor não pode ser maior do que o Disponível');
		campo.value = vlold;
		calculovalorPI();
	}
}

function excluirSubacao(sbaid){
	if(confirm('Deseja excluir este registro')){
		window.location.href = "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadsubacaoUN&acao=A&sbaidExcluir="+sbaid;
		return true;
	} else {
		return false;
	}
}

function vincular(situacao){
	var sbaid = '<?php echo $obPi_Subacao->sbaid; ?>';
 	var url = window.location.href;
	var parametros = "requisicao=vincular&sbaid="+sbaid+'&situacao='+situacao ;
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
					window.location.reload();
				}
			}
		}
	);
}

function detfin(ptrid){
	var sbaid = '<?php echo $obPi_Subacao->sbaid;?>';
	if(sbaid){
		janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/detalhafinanceiro_subacaoUN&acao=A&ptrid='+ptrid+'&sbaid='+sbaid, 'janela2', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width='+(screen.width-420)+',height=280' ); janela.focus();
	}
}
</script>
<div id="dibDegug"></div>