<?php 
include "planotrabalhoUG/_funcoes.php";
include "planotrabalhoUG/_componentes.php";

if($_POST['imprimeWord']){
	
	if(!$_POST['aSbaid']){
		die;
	}
	$aSbaid = explode(",", $_POST['aSbaid']);
	$aSbaid = ($aSbaid) ? $aSbaid : array();
	//header("Content-type: application/vnd.ms-word");
	/*header("Content-type: application/msword");
	header("Content-type: application/force-download");
	header("Content-Disposition: attachment; filename=Subacao.doc");
	header("Pragma: no-cache");*/
	
	$sTexto = "";
	if(count($aSbaid)){
		foreach($aSbaid as $sbaid){
			$sql = "SELECT s.sbacod, s.sbadsc, s.sbatitulo FROM monitora.pi_subacao s where s.sbaid = ". $sbaid;
			$aSubacao = $db->pegaLinha( $sql );
			$aSubacao = ($aSubacao) ? $aSubacao : array();
			
			if(count($aSubacao)){
				$sTexto .= "
				<div>
					<table cellSpacing=\"0\" cellPadding=\"0\" align=\"center\" border=\"1\" width=\"95%\" >
						<tr>
							<td colspan=\"2\" align=\"center\">Subação: <b>{$aSubacao['sbatitulo']} ({$aSubacao['sbacod']})</b></td>
						<tr>
					
				";
				# Enquadramento
				$sql = "SELECT 
							ed.eqdcod as enquadramento
		 				FROM monitora.pi_subacaoenquadramento sae
		 					inner join planejamento.enquadramento_despesa ed on ed.eqdid = sae.eqdid
						WHERE sae.sbaid = ". $sbaid;
				$aEnquadramento = $db->carregar( $sql );
				$aEnquadramento = ($aEnquadramento) ? $aEnquadramento : array();
				$sTexto .= "<tr>";
				$sTexto .= "<td align=\"center\" width=\"250px\">"; 
				$sTexto .= "Enquadramento da despesa<br />";
				foreach($aEnquadramento as $enquadramento){
					$sTexto .= "<b>".$enquadramento['enquadramento']."</b><br />";
					
				}
				$sTexto .= "</td>"; 
				
				# Unidade Gestora
				$sql = "SELECT 
							ug.ungabrev as unidadegestora
		 				FROM monitora.pi_subacaounidade su
		 					inner join public.unidadegestora ug on su.ungcod = ug.ungcod
						WHERE su.sbaid = ". $sbaid;
				$aUnidadeGestora = $db->carregar( $sql );
				$aUnidadeGestora = ($aUnidadeGestora) ? $aUnidadeGestora : array();
				$sTexto .= "<td align=\"center\">"; 
				$sTexto .= "Unidade Gestora<br />";
				foreach($aUnidadeGestora as $unidadeGestora){
					$sTexto .= "<b>".$unidadeGestora['unidadegestora']."</b><br />";
				}
				$sTexto .= "</td>"; 
				$sTexto .= "</tr>";
				
				# Programa Ação
				$sql = "select 
						    distinct
						        trim('('||dtl.ptres||') '||aca.prgcod||'.'||aca.acacod||' - '||aca.acadsc) as descricao,
						        sum(sd.sadvalor) as valorcadastrado
								FROM monitora.acao aca 
						        inner JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
								inner JOIN monitora.pi_subacaodotacao sd ON dtl.ptrid = sd.ptrid
								inner join monitora.pi_subacao s ON s.sbaid = sd.sbaid
								--inner join monitora.pi_subacaounidade sau ON sau.sbaid = s.sbaid
						where 	aca.prgano='".date("Y")."' and aca.acasnrap = false
						        and s.sbaid = ". $sbaid."
						group by descricao 
						       ";
				//ver($sql,d);
				$aProgramaAcao = $db->carregar( $sql );
				$aProgramaAcao = ($aProgramaAcao) ? $aProgramaAcao : array();
				$sTexto .= "<tr>";
				$sTexto .= "<td align=\"center\">ORIGEM<br />(Programa - Ação)</td>"; 
				$sTexto .= "<td align=\"left\">"; 
				foreach($aProgramaAcao as $programaAcao){
					$sTexto .= "&nbsp;".$programaAcao['descricao'].' - R$ '.number_format($programaAcao['valorcadastrado'],2,',','.')." <br />";
				}
				$sTexto .= "</td>"; 
				$sTexto .= "</tr>";
				
				$sTexto .= "<tr>";
				$sTexto .= "<td align=\"center\">Descrição</td>"; 
				$sTexto .= "<td align=\"left\">&nbsp;{$aSubacao['sbadsc']}</td>";
				$sTexto .= "</tr>"; 
				
				$sTexto .= "</table></div>";
				$sTexto .= "<br /><br />";
			}
			
		}
	}
	//echo $sTexto;
	//exit;
	
	//echo "<html>".html_entity_decode($sTexto)."</html>";
	echo "<html><head>
		<style type=\"text/css\">
			html { margin: 10px 2px 10px 2px; }
			body { text-align: center; }
			table { border 0; }
			td{ font-size: 8pt; font-family: verdana; }
			td.italico { font-style: italic; }
			td.negrito { font-weight: bold; }
			td.sublinhado { text-decoration: underline; }
			td.bordaDireita { border-right: 1px solid #808080; }
			td.bordaCima{ border-top: 1px solid #000000; }
			td.bordaPontilhadaCima{ border-top: 2px dotted #909090; }
			td.bordaBaixo{ border-bottom: 1px solid #0000; }
			td.fundo{ background-color: #f3f3f3; }
			td.fundoEscuro{ background-color: #dadada; }
			@media print {.imprimir { display: none }}
			span.imprimir { position: absolute; top: 3px; margin: 0; padding: 5px; position: fixed; background-color: #f0f0f0; border: 1px solid #909090; cursor:pointer; }
			span.imprimir:hover { background-color: #d0d0d0; }
			div { margin:0 auto; padding: 0; text-align:left; page-break-after: always; width: 95% }
		</style>
		<title>Ministério da Educação - Subações</title>
		<script type=\"text/javascript\">
			self.focus();
		</script>
	</head>".$sTexto."</html>";
	/*echo "<html><head>
		<style type=\"text/css\">
			html { margin: 10px 2px 10px 2px; }
			body { text-align: center; }
			table { border 0; }
			td{ font-size: 8pt; font-family: verdana; }
			td.italico { font-style: italic; }
			td.negrito { font-weight: bold; }
			td.sublinhado { text-decoration: underline; }
			td.bordaDireita { border-right: 1px solid #808080; }
			td.bordaCima{ border-top: 1px solid #000000; }
			td.bordaPontilhadaCima{ border-top: 2px dotted #909090; }
			td.bordaBaixo{ border-bottom: 1px solid #0000; }
			td.fundo{ background-color: #f3f3f3; }
			td.fundoEscuro{ background-color: #dadada; }
			@media print {.imprimir { display: none }}
			span.imprimir { position: absolute; top: 3px; margin: 0; padding: 5px; position: fixed; background-color: #f0f0f0; border: 1px solid #909090; cursor:pointer; }
			span.imprimir:hover { background-color: #d0d0d0; }
			div { margin:0 auto; padding: 0; text-align:left; page-break-after: always; width: 95% }
		</style>
		<title>Ministério da Educação - Subações</title>
		<script type=\"text/javascript\">
			self.focus();
		</script>
		<script language=\"javascript\" type=\"text/javascript\" src=\"../includes/tiny_mce.js\"></script>
		<script language=\"JavaScript\">
		//Editor de textos
		tinyMCE.init({
			theme : \"advanced\",
			mode: \"specific_textareas\",
			editor_selector : \"text_editor_simple\",
			plugins : \"table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen\",
			theme_advanced_buttons1 : \"undo,redo,separator,link,bold,italic,underline,forecolor,backcolor,separator,justifyleft,justifycenter,justifyright, justifyfull, separator, outdent,indent, separator, bullist\",
			theme_advanced_buttons2 : \"\",
			theme_advanced_buttons3 : \"\",
			theme_advanced_toolbar_location : \"top\",
			theme_advanced_toolbar_align : \"left\",
			extended_valid_elements : \"a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]\",
			language : \"pt_br\",
			width : \"95%\",
			entity_encoding : \"raw\"
			});
		</script>
	</head><textarea name=\"$atidetalhamento\" rows=\"50\" cols=\"80\" class=\"text_editor_simple\">".$sTexto."</textarea></html>";
	*/
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

monta_titulo( "Gerenciar Subação", "");
?>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<form method="POST" name="formulario" id="formulario">
<input type="hidden" name="aSbaid" id="aSbaid" value="">
<input type="hidden" name="imprimeWord" id="imprimeWord" value="">
<input type="hidden" name="pesquisa" id="pesquisa" value="1">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td align='right' class="SubTituloDireita">Código:</td>
	    <td><?php
	    	$sbacod = $_POST['sbacod'];
	    	echo campo_texto('sbacod','N','S','',6,4,'',''); 
	    	?>
	    </td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Título:</td>
	    <td><?php 
	    	$sbatitulo = $_POST['sbatitulo'];
	    	echo campo_texto('sbatitulo','N','S','',50,255,'',''); 
	    	?>
	    </td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Enquadramento da Despesa:</td>
	    <td>
	    	
			<?
			$eqdid = $_REQUEST['eqdid'];
			$sql = "SELECT eqdid as codigo, eqdcod ||' - '|| eqddsc as descricao FROM planejamento.enquadramento_despesa WHERE eqdano = '{$_SESSION['exercicio']}' order by eqdcod";
			$db->monta_combo('eqdid', $sql, 'S',  'Selecione', '', '', '', '', 'N'); 
			?>    	
	    </td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Executor Orçamentário e Financeiro:</td>
	    <td>
			<?
			$pieid = $_REQUEST['pieid'];
			$sql = "SELECT pieid as codigo, piecod ||' - '|| piedsc as descricao FROM  monitora.pi_executor";  
			$db->monta_combo('pieid', $sql, 'S', 'Selecione', '', '', '', '100', 'N', 'pieid'); 
			?>    	
	    </td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Gestor da Subação:</td>
	    <td>
			<?
			$pigid = $_REQUEST['pigid'];
			$sql = "SELECT pigid as codigo, pigcod ||' - '|| pigdsc as descricao FROM  monitora.pi_gestor";  
			$db->monta_combo('pigid', $sql, 'S', 'Selecione', '', '', '', '100', 'N', 'pigid'); 
			?>    	
	    </td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Permite Obras?</td>
	    <td>
	    	<input type="radio" name="sbaobra" value="todos" <? echo ((!$_REQUEST["sbaobra"]||$_REQUEST["sbaobra"]=="todos")?"checked":""); ?>> Todos <input type="radio" name="sbaobra" value="sim" <? echo (($_REQUEST["sbaobra"]=="sim")?"checked":""); ?>> Sim <input type="radio" name="sbaobra" value="nao" <? echo (($_REQUEST["sbaobra"]=="nao")?"checked":""); ?>> Não
	    </td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Ações:</td>
	    <td>
			<?php
			if($_POST['acaid'][0] && !$_POST['ordemlista']) {
				$acaid = $db->carregar("SELECT
											aca.acaid as codigo,
											'(PTRES:'||pt.ptres||') - '|| aca.unicod ||'.'|| aca.prgcod ||'.'|| aca.acacod ||' - '|| aca.acadsc as descricao
										  FROM monitora.acao aca 
										  	inner join monitora.ptres pt on pt.acaid=aca.acaid  
										WHERE aca.prgano='".$_SESSION['exercicio']."' and aca.acasnrap = false
										and aca.unicod IN('".AD."','".CAPES."','".INEP."','".FNDE."')
										and aca.acaid in (".implode(",", $_POST['acaid']).")
										GROUP BY codigo , descricao
										ORDER BY 
											1");
			}
			$sqlComboAcoes = "SELECT
								aca.acaid as codigo,
								'(PTRES:'||pt.ptres||') - '|| aca.unicod ||'.'|| aca.prgcod ||'.'|| aca.acacod ||' - '|| aca.acadsc as descricao
							  FROM monitora.acao aca 
							  	inner join monitora.ptres pt on pt.acaid=aca.acaid
							  WHERE aca.prgano='".$_SESSION['exercicio']."' and aca.acasnrap = false
							  	and aca.unicod IN('".AD."','".CAPES."','".INEP."','".FNDE."')
							  GROUP BY codigo , descricao
							  ORDER BY 
								1";
			combo_popup( "acaid", $sqlComboAcoes, "Ações", "600x600", 0, array(), "", "S", false, false, 5, 400 );
			?>    	
	    </td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Unidades Orçamentárias / Gestoras:</td>
		<td>
		<?php
		if($_POST['unicod'][0] && !$_POST['ordemlista']){
				$unicod = $db->carregar("SELECT ent.entunicod as codigo,
									 		   CASE fen.funid
											   WHEN 34 THEN ent.entungcod ||' - '|| ent.entnome	ELSE ent.entunicod ||' - '|| ent.entnome END as descricao
										FROM entidade.entidade ent 
										LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid  
										WHERE entunicod in ('".implode("', '", $_POST['unicod'])."')");
		}
		else{
			$unicod = "";
		}
		$sqlComboEntidade = "SELECT
						   ent.entunicod as codigo,
						   CASE fen.funid
						   	when 34 then 
						   		entungcod ||' - '|| entnome 
						   	else 
						   		entunicod ||' - '|| entnome 
						   	end as descricao
						   FROM entidade.entidade ent 
						   LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
						   WHERE entunicod IN('".AD."','".CAPES."','".INEP."','".FNDE."','".FIES."') and fen.funid <> 33 and ent.entid not in(391293,391294) ORDER BY entunicod";
	
		combo_popup( "unicod", $sqlComboEntidade, "Unidades Orçamentárias", "600x600", 0, array(), "", "S", false, false, 5, 400 );
		?>
		</td>
	</tr>
	<tr bgcolor="#cccccc">
		<td></td>
		<td><input type="button" class="botao" name="pesquisar" value="Pesquisar" id="pesquisar"></td>
	</tr>
	</table>
</form>
<?php 

if($_POST['pesquisa']){
	$where = " WHERE sba.sbastatus = 'A' ";
	$where .= $_POST['sbatitulo'] ? " AND sba.sbatitulo ilike '%".$_POST['sbatitulo']."%' " : '';
	$where .= $_POST['sbacod'] ? " AND upper(sba.sbacod) like '%".strtoupper($_POST['sbacod'])."%' " : '';
	$where .= $_POST['exeid'] ? " AND sba.exeid = ".$_POST['exeid']." " : '';
	$where .= $_POST['gstid'] ? " AND sba.gstid = ".$_POST['gstid']." " : '';
	$where .= $_POST['eqdid'] ? " AND eq.eqdid = ".$_POST['eqdid']." " : '';
	$where .= $_POST['pieid'] ? " AND exe.pieid = ".$_POST['pieid']." " : '';
	$where .= $_POST['pigid'] ? " AND gst.pigid = ".$_POST['pigid']." " : '';
	
	if($_REQUEST['acaid'][0]) {
		$acoes = implode(",", $_REQUEST['acaid'] );
		$where .= " AND ptr.acaid in (".$acoes.") ";
	}
	
	$where .= ($_REQUEST['sbaobra']!="todos" && $_REQUEST['sbaobra']!="") ? " AND sba.sbaobras = ".(($_REQUEST['sbaobra']=="sim")?"true":"false")." " : '';
	if($_POST['unicod'][0] && !$_POST['ordemlista']) {
		$where .= " AND pe.unicod in ('".implode("', '", $_POST['unicod'])."')";
	}
	
	$sql = "SELECT '<center><input name=\"aSubacao\" id=\"subacao_'||sba.sbaid||'\" value=\"'||sba.sbaid||'\" type=\"checkbox\" class=\"teste\"></center>' as acao, 
				sba.sbacod,
				sba.sbatitulo,
				exe.piecod ||' - '||exe.piedsc as executor,
				gst.pigcod ||' - '||gst.pigdsc as gestor,
				u.usunome ||' - '||to_char(sba.sbadata, 'dd/mm/YYYY hh24:mi') as resp
				FROM monitora.pi_subacao sba
				LEFT JOIN seguranca.usuario u ON u.usucpf = sba.usucpf
				LEFT JOIN monitora.pi_subacaoenquadramento eq ON eq.sbaid = sba.sbaid 
				LEFT JOIN monitora.pi_executor exe ON exe.pieid = sba.pieid
				LEFT JOIN monitora.pi_gestor gst ON gst.pigid = sba.pigid
				LEFT JOIN monitora.pi_subacaounidade pe ON pe.sbaid = sba.sbaid
				INNER JOIN public.unidadegestora ung on ung.ungcod = pe.ungcod
				LEFT JOIN monitora.pi_subacaodotacao sdt ON sdt.sbaid = sba.sbaid
				LEFT JOIN monitora.ptres ptr ON ptr.ptrid = sdt.ptrid
				{$where}
				GROUP BY sba.sbaid,sba.sbacod,sba.sbatitulo,executor,gestor,resp
				ORDER BY sba.sbacod DESC
				";
				//ver($sql);
	$cabecalho = array("","Código","Título","Executor Orçamentário","Gestor da Subação","Atualizado por / Data");
	$db->monta_lista($sql,$cabecalho,50,5,'N','95%',$par2);
}

?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr bgcolor="#cccccc">
		<td width="90px"><input type="checkbox" id="checktodos"> Marcar todos </div></td>
		<td><input type="button" class="botao" id="botaoImprimeDoc" name="bta" value="Imprimir Doc"></td>
	</tr>
</table>

<script type="text/javascript"><!--

$('#pesquisar').click(function () {
    $('#imprimeWord').val(''); 
    $('#aSbaid').val(''); 
	$('#formulario').submit();
});

$('#botaoImprimeDoc').click(function () {
	var aSbaid = [];
	
	$('input[type=checkbox]:checked').each(function() {
		if($(this).val() != 'on'){ // Gato para não pegar o botão marca todos
			aSbaid.push($(this).val());
		}
    });

    if(aSbaid.length == 0){
		alert('É necessário selecionar ao menos uma subação.');
		return false;
    }
    
    $('#pesquisa').val(''); 
    $('#imprimeWord').val(1); 
    $('#aSbaid').val(aSbaid); 
    
	//alert($('#aSbaid').val());
	$('#formulario').submit();

    //var url = '<?php /*echo str_replace('/simec/','',$_SERVER['REQUEST_URI']);*/ ?>';
//    $.ajax({
//		   type		: "POST",
//		   url		: url,
//		   data		: "imprimeWord=1&aSbaid="+aSbaid,
//		   beforeSend: function(xhr) {
		   		/*
		        xhr.setRequestHeader('Content-Type', 'application/msword');
		        xhr.setRequestHeader('Content-Type', 'application/force-download');
		        xhr.setRequestHeader('Content-Disposition', 'attachment; filename=Subacao.doc');
		        xhr.setRequestHeader('Pragma: no-cache');
		        xhr.setRequestHeader('Pragma', '');
		        */
//		        }, 
//		   success	: function(msg){
//						$('#divTeste').html(msg);
//    				}
//		 });

});

$('#checktodos').click(function () {
	$('input[type=checkbox]').each(function() {
		if($('#checktodos').attr("checked") == true){
			if($(this).attr("checked") == false){
				$(this).attr("checked",true);
			}
		} else {
			$(this).attr("checked",false);
			$('#aSbaid').val('');
		}
    });
});

$('#imprimeWord').val(''); 
$('#aSbaid').val('');

--></script>
<div id="divTeste"></div>