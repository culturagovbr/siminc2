<?php
// controlador do numero sequencial do gerador
include APPRAIZ."/includes/controlegeradorsequenciapi.inc";

include "planotrabalhoUG/_constantes.php";
include "planotrabalhoUG/_funcoes.php";
include "planotrabalhoUG/_componentes.php";

require_once APPRAIZ . "monitora/classes/Pi_Subacao.class.inc";
require_once APPRAIZ . "monitora/classes/Pi_SubacaoDotacao.class.inc";

if($_POST['acaoForm']){
	
	extract($_POST);
	
	$obPi_Subacao = new Pi_Subacao($sbaid);
	$obPi_Subacao->sbadsc = $sbadsc;
	$obPi_Subacao->sbatitulo = trim($sbatitulo);
	$obPi_Subacao->sbasigla = trim($sbasigla);
	$obPi_Subacao->sbaobras = $sbaobras == true ? 't' : 'f';
	$obPi_Subacao->sbaplanotrabalho = $sbaplanotrabalho;
	$obPi_Subacao->sbasituacao = 'A';
	$obPi_Subacao->usucpf = $_SESSION['usucpf'];
	$obPi_Subacao->pieid = $pieid;
	$obPi_Subacao->pigid = $pigid;
	
	if (!$sbaid) {
		$gerador1 = $db->pegaUm("SELECT piecod FROM monitora.pi_executor WHERE pieid ='".$pieid."'");
		$gerador2 = $db->pegaUm("SELECT pigcod FROM monitora.pi_gestor WHERE pigid = '".$pigid."'");
		
		// Nova maneira.
		$identificador = $gerador1.$gerador2;
		$seq = $db->pegaUm("SELECT gspseq FROM public.geradorsequencialpi WHERE gspidentificador = '".$identificador."' ORDER BY gspid DESC");
			
		if($seq) {
			$gspseq = retornaseq(substr($seq, -2));
			$gspseq = str_pad($gspseq, 4, "0", STR_PAD_LEFT);
		} else {
			$gspseq = "0001";
		}
			
		$sql = "INSERT INTO geradorsequencialpi(gspseq, gspidentificador)
	    			VALUES ('".$gspseq."', '".$identificador."');";
		$db->executar($sql);
				
		// No caso da subação retorna apenas os dois últimos campos do código gerado.
		$gerador3 = substr($gspseq, -2);
			
		// Então gera o sbacod para gravar na tabela 'monitora.subacao'.
		$obPi_Subacao->sbacod = $gerador1.$gerador2.$gerador3;
		
	}
	
	$obPi_Subacao->salvar();
	
	if($obPi_Subacao->sbaid){
		$obPi_Subacao->deletaSubacaoDotacao( $obPi_Subacao->sbaid );		
	}
	$obPi_SubacaoDotacao = new Pi_SubacaoDotacao();
	$obPi_SubacaoDotacao->sbaid = $obPi_Subacao->sbaid;
	$obPi_SubacaoDotacao->ptrid = $ptrid;
	$obPi_SubacaoDotacao->sadvalor = $sadvalor;
	$obPi_SubacaoDotacao->salvar($obPi_Subacao->sbaid,$ptrid,$valor);
	
	if($obPi_Subacao->sbaid){
		$db->executar("DELETE FROM monitora.pi_subacaounidade WHERE sbaid = '$obPi_Subacao->sbaid'");
		$db->executar("DELETE FROM monitora.pi_subacaoenquadramento WHERE sbaid = '$obPi_Subacao->sbaid'");
	}
	
	$arUngcod = $ungcod;
	$ungcod = '';
	$unicod = 'null';
	
	foreach($arUngcod as $ungcod){
		$db->executar("INSERT INTO monitora.pi_subacaounidade(sbaid, ungcod, unicod, unitpocod) VALUES ('".$obPi_Subacao->sbaid."', '".$ungcod."', ".$unicod.", 'U');");
	}
	$arEqdid = $eqdid;
	$eqdid = '';
	if($arEqdid[0]) {
		foreach($arEqdid as $eqdid){
			$db->executar("INSERT INTO monitora.pi_subacaoenquadramento(sbaid, eqdid) VALUES ('".$obPi_Subacao->sbaid."', '".$eqdid."');");
		}
	}
	$db->commit();
		
	$_REQUEST['acao'] = 'A';
	$db->sucesso("principal/planotrabalhoUG/cadsubacaoUG","&sbaid=".$obPi_Subacao->sbaid);
	unset($obPi_Subacao);
	unset($obPi_SubacaoDotacao);
	die;
}

/*
 * Verifica se o usuário possui o perfil 'Gestor da Unidade de Planejamento' 
 * e possui a unidade do FNDE(26298) associada.
 */
$GESTOR_FNDE = false;

// definindo variaveis
$atiid  = $_POST['atiid'] ? $_POST['atiid'] : $_GET['atiid']; 
$plicod = $_POST['plicod'] ? $_POST['plicod'] : $_GET['plicod']; 

// verifica se ocorre algum evento
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

if($_GET['sbaid']){
	$subacao = $db->pegaUm("SELECT sbaid FROM monitora.pi_subacao WHERE sbaid = {$_GET['sbaid']}");
	if( !$subacao ){
		echo "<script>
				alert('A Subação informada não existe!');
				history.back(-1);
			  </script>";
		die;
	}
}

$obPi_Subacao = new Pi_Subacao($_GET['sbaid']);

if($obPi_Subacao->sbaid){
		$sql = "SELECT
						dtl.ptrid,
						dtl.ptres,
						trim(aca.prgcod||'.'||aca.acacod||'.'||aca.unicod||'.'||aca.loccod||' - '||aca.acadsc) as descricao,
						uni.unidsc,
						SUM(dtl.ptrdotacao) as dotacaoinicial,
						coalesce(cast(SUM(dt.valor) as varchar),'0.00') as detalhamento,
						coalesce(cast(SUM(dt2.valorpi) as varchar),'0.00') as detalhamento2,
						(SUM(dtl.ptrdotacao)-coalesce(SUM(dt.valor),0.00)) as diferenca,
						sum(sd.sadvalor) as valorcadastrado 	
					  FROM monitora.acao aca 
						inner JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
						inner JOIN monitora.pi_subacaodotacao sd ON dtl.ptrid = sd.ptrid
						inner JOIN public.unidade uni ON uni.unicod = dtl.unicod 					  
						LEFT JOIN ( select ptrid,
									sum( sadvalor ) as valor
								from  monitora.pi_subacaodotacao
								group by ptrid ) dt ON dtl.ptrid = dt.ptrid 
						LEFT JOIN ( select sbaid, ptrid, sum(dtl.valorpi) as valorpi from monitora.v_pi_detalhepiptres dtl group by sbaid, dtl.ptrid ) dt2 ON dtl.ptrid = dt2.ptrid and dt2.sbaid = sd.sbaid
						WHERE aca.prgano='".$_SESSION['exercicio']."' and aca.acasnrap = false
						and sd.sbaid = ".$obPi_Subacao->sbaid."
					  	and aca.unicod IN('".AD."','".CAPES."','".INEP."','".FNDE."','".FIES."','".SUPERVISAOMEC."')
					    GROUP BY dtl.ptrid,dtl.ptres,descricao,uni.unidsc
				  		ORDER BY 1
					  ";
	$ptresacao = $db->carregar($sql);
}

monta_titulo( "Cadastro de Subação", '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');
?>

<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario">
<!-- input type="hidden" name="evento" id="evento" value="<? echo (($_REQUEST['sbaid'])?"G":"I"); ?>" -->
<input type="hidden" name="acaoForm" id="acaoForm" value="1" />
<input type="hidden" name="sbaid" id="sbaid" value="<? echo $obPi_Subacao->sbaid; ?>" />
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td align='right' class="SubTituloDireita">Título:</td>
    <td><? 
    		$sbatitulo = trim($obPi_Subacao->sbatitulo);
			echo campo_texto('sbatitulo','S','S','',76, 200,'','',null,null,null,'',"limitaTitulo();");
			echo "<input type='hidden' name='sbatituloantigo' value='".$sbatitulo."'>"; 
		?>
		
	</td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Título Abreviado / Sigla:</td>
    <td><? 
    		$sbasigla = trim($obPi_Subacao->sbasigla);
			echo campo_texto('sbasigla','S','S','',25,25,'','',null,null,null,''); 
		?>
		
	</td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita" valign="top">Enquadramento da Despesa:</td>
    <td>
		<?
		if( $obPi_Subacao->sbaid ){
			$sql = "SELECT 
						ed.eqdid as codigo, 
						ed.eqdcod ||' - '|| ed.eqddsc as descricao 
					FROM planejamento.enquadramento_despesa ed
						INNER JOIN monitora.pi_subacaoenquadramento sae on ed.eqdid = sae.eqdid
					WHERE sae.sbaid = {$obPi_Subacao->sbaid} ";
			$eqdid = $db->carregar($sql);
	 	}
		
		$sql = "SELECT eqdid as codigo, eqdid ||' - '|| eqdcod ||' - '|| eqddsc as descricao FROM planejamento.enquadramento_despesa WHERE eqdano = '{$_SESSION['exercicio']}' order by eqddsc";
		combo_popup( "eqdid", $sql, "Enquadramento da Despesa", "600x600", 0, array(), "", "S", false, false, 5, 480 );
		?>
		<img src="../imagens/obrig.gif">    	
    </td>
</tr>
<tr>
<td align='right' class="SubTituloDireita">Executor Orçamentário e Financeiro:</td>
    <td>
	<?
	//ver($obPi_Subacao,d);
		if($obPi_Subacao->sbaid && $obPi_Subacao->pieid){
			$sql = "SELECT piecod ||' - '|| piedsc as descricao FROM  monitora.pi_executor WHERE pieid = {$obPi_Subacao->pieid}";
			echo $db->pegaUm($sql);				
		} else {
			$sql = "SELECT pieid as codigo, piecod ||' - '|| piedsc as descricao FROM  monitora.pi_executor order by piecod";  
			$db->monta_combo('pieid', $sql, 'S', 'Selecione', '', '', '', '170', 'S', 'pieid');
		}
	?>    	
    </td>
</tr>
<tr>
<td align='right' class="SubTituloDireita">Gestor da Subação:</td>
    <td>
	<?
		if($obPi_Subacao->sbaid && $obPi_Subacao->pigid){
			$sql = "SELECT pigcod ||' - '|| pigdsc as descricao FROM  monitora.pi_gestor WHERE pigid = {$obPi_Subacao->pigid}";
			echo $db->pegaUm($sql);  
		} else {
			$sql = "SELECT pigid as codigo, pigcod ||' - '|| pigdsc as descricao FROM  monitora.pi_gestor order by pigcod";  
			$db->monta_combo('pigid', $sql, 'S', 'Selecione', '', '', '', '170', 'S', 'pigid'); 
		}
	?>    	
    </td>
</tr>
<? if($obPi_Subacao->sbaid){ ?>
<td align='right' class="SubTituloDireita">Código da subação:</td>
    <td><? echo $obPi_Subacao->sbacod; ?>    	
    </td>
</tr>
<? } ?>
<tr>
	<td align='right' class="SubTituloDireita">Permite Obras?</td>
    <td>
		<input type="checkbox" id="checkPermiteObras" onchange="marcaPermiteObras();" <?= ($obPi_Subacao->sbaobras == "t") ? 'checked="checked"' : '' ?>>
		<input type="hidden" id="sbaobras" name="sbaobras" value="<?= (isset($obPi_Subacao->sbaobras) && $obPi_Subacao->sbaobras=="t") ? "true" : "false" ?>">
    </td>
</tr>

<tr>
	<td class="SubTituloDireita" valign="top">Unidades Orçamentárias / Gestoras:</td>
	<td>
	<?php
	
		if( $obPi_Subacao->sbaid ){
			$sql = "SELECT
					   ung.ungcod as codigo,
					   ung.ungdsc as descricao
					FROM public.unidadegestora ung
					inner join monitora.pi_subacaounidade sau on ung.ungcod = sau.ungcod
					where sau.sbaid = {$obPi_Subacao->sbaid} ";
			$ungcod = $db->carregar($sql);
	 	}
	 	
		$sql = "SELECT
			   	  ung.ungcod as codigo,
		   	   	  ung.ungdsc as descricao
			   FROM public.unidadegestora ung";

		combo_popup( "ungcod", $sql, "Unidades Orçamentárias / Gestoras", "600x600", 0, array(), "", "S", false, false, 5, 480 );
	?>
	<img src="../imagens/obrig.gif">
	</td>
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
		    <td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Detalhado em SubAção</b></td>
		    <td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Detalhado em PI</b></td>
		    <td style="background-color: #C9C9C9; width:100px;" align="center"><b>Dotação Disponível</b></td>
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
			<td align="right" colspan="7"><input type="button" onclick="abrir_lista();" id="btn_selecionar_acaptres" value="Selecionar Ação/PTRES"></td>
		</tr>
        </table>
    </td>
</tr>

<tr>
    <td align='right' class="SubTituloDireita" valign=top>Descrição / Finalidade:</td>
    <td><?php
    		$sbadsc = $obPi_Subacao->sbadsc;
    		echo campo_textarea( 'sbadsc', 'S', 'S', '', 80, 5, 1000 );
    	?>
    </td>
</tr>
<tr bgcolor="#cccccc">
	      <td></td>
	  	  <td>
	  	  <input type="button" class="botao" name="btg" value="Gravar" onclick="submeter('<?=$plicod?>');">
	  	  <? if($_REQUEST['sbaid']) { ?>
	  	  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	  	  <input type="button" class="botao" name="btassociar2" value="Novo" onclick="validabotao('N');">
	  	  <?}?>
	  	  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	  	  <input type="button" class="botao" name="btassociar3" value="Voltar" onclick="validabotao('V');"></td>
	  	  
	      </tr>
</table>
</form>
<script type="text/javascript">
function submeter(pliid) {
	if(validar()) {
		selectAllOptions( document.formulario.eqdid );
		selectAllOptions( document.formulario.ungcod );
		document.formulario.submit();
	}
}

function marcaPermiteObras() {
	var chPermiteObras = document.getElementById("checkPermiteObras");
	var hSbaObra = document.getElementById("sbaobras");
	
	if(chPermiteObras.checked)
		hSbaObra.value = "true";
	else
		hSbaObra.value = "false";
}

function removersba(sbaid){
	var conf = confirm("Você realmente deseja excluir este Programa?");	
	if(conf) {
		document.getElementById('evento').value = 'E';
		document.getElementById('sbaid').value = sbaid;
		document.formulario.submit();	
	}
}

function alterarsba(sbaid){
	document.getElementById('evento').value = 'A';
	document.getElementById('sbaid').value = sbaid;
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

function validar(){	
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

	if(document.formulario.sbatitulo.value == '') {
		msg+="O preenchimento do campo Título é obrigatório.\n";
	}
	if(document.formulario.sbasigla.value == '') {
		msg+="O preenchimento do campo Sigla é obrigatório.\n";
	}

	var eqdid = document.getElementById('eqdid');
	if( eqdid[0].value == '' ){
		msg += 'Você deve selecionar no mínimo Enquadramento da Despesa.\n';
	}

	
	var ungcod 	 = document.getElementById('ungcod');
	if( ungcod[0].value == '' ){
		msg += 'Você deve selecionar no mínimo Unidades Orçamentárias / Gestoras.\n';
	}
	

	if(msg != ""){
		alert(msg);
		return false;
	} else {
		return true;
	}	
}


function abrir_lista() {
	var janela = window.open( '?modulo=principal/planotrabalhoUG/popupcadsubacaoUG&acao=A', 'blank', 'height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
	janela.focus();
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
function validabotao(bot) {
	form = document.formulario;
	if(bot=='N'){
		window.location='?modulo=principal/planotrabalhoUG/cadsubacaoUG&acao=A';
	}
	if(bot=='V'){
		window.location='?modulo=principal/planotrabalhoUG/gerenciarsubacaoUG&acao=A';
	}
}
function limitaTitulo() {
	tit = document.formulario.sbatitulo.value;
	tot_tit = tit.length;
	if(tot_tit > 200){
		alert('O campo Título possui o limite de 200 caracteres.');
		document.formulario.btg.focus();
	}
}

function detfin(ptrid){
	var sbaid = '<?php echo $obPi_Subacao->sbaid;?>';
	if(sbaid){
		janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUG/detalhafinanceiro_subacaoUG&acao=A&ptrid='+ptrid+'&sbaid='+sbaid, 'janela2', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width='+(screen.width-420)+',height=280' ); janela.focus();
	}
}
</script>