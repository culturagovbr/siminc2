<?php
$sql_lista="";
$primeira = 1;
include "planotrabalhoUN/_constantes.php";
include "planotrabalhoUN/_funcoes.php";
$boNaoVePlanoInterno = boNaoVePlanoInterno();

if ($_POST['pesquisa']) {
	$primeira = 2;

	//header('content-type: text/html; charset=ISO-8859-1');
	
	$where = array();
	
	extract($_POST);
	
	// Subação
	if( $sbaid ){
		array_push($where, " su.sbaid =  '".$sbaid."'");
	}
	// Enquadramento
	if( $eqdid ){
		array_push($where, " p.eqdid =  '".$eqdid."'");
	}
	// Nivel
	if( $neeid ){
		array_push($where, " p.neeid =  '".$neeid."'");
	}
	// Categoria
	if( $capid ){
		array_push($where, " p.capid =  '".$capid."'");
	}
	// Codifição da Unidade(livre)
	if( $plilivre ){
		array_push($where, " p.plilivre =  '".$plilivre."'");
	}
	// Modalidade de Ensino
	if( $mdeid ){
		array_push($where, " p.mdeid =  '".$mdeid."'");
	}
	// Titulo
	if( $plititulo ){
		array_push($where, " p.plititulo ilike '%".$plititulo."%'");
	}
	// Descrição
	if( $plidsc ){
		array_push($where, " p.plidsc ilike '%".$plidsc."%'");
	}
	
	
	
	/*$sql = "SELECT '<center><a style=\"cursor:pointer;\" onclick=\"alterarpi(\''||p.pliid||'\');\"><img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\"></a>'|| CASE p.plisituacao 
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
				" . ( is_array($where) && count($where) ? ' AND' . implode(' AND ', $where) : '' ) ."
				$where_ 
				GROUP BY p.pliid, p.plicod,p.plititulo,p.plidata,p.plisituacao 
				ORDER BY p.plidata DESC";
	
	$cabecalho = array("","Código do PI","Título","Valor Previsto(R$)","Situação","Última atualização");
	$db->monta_lista($sql,$cabecalho,50,5,'N','center','');
	
	die();*/
}

// controlador do numero sequencial do gerador
include APPRAIZ."/includes/controlegeradorsequenciapi.inc";

include  APPRAIZ."includes/cabecalho.inc";

// carrega as funções de integração
include "planotrabalhoUN/unidade_atividade_funcoes.php";
// carrega as funções do módulo pde
//include "planotrabalhoUN/_constantes.php";
//include "planotrabalhoUN/_funcoes.php";
include "planotrabalhoUN/_componentes.php";

require_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";
require_once APPRAIZ . "monitora/classes/Pi_PlanoInternoHistorico.class.inc";

$unidade = $db->pegaLinha("SELECT count(1) FROM public.unidade uni WHERE uni.unicod='".$_SESSION['monitora_var']['unicod']."'");
if(!$unidade) die("<script>alert('Unidade não encontrada');window.location = '?modulo=principal/planotrabalhoUN/listaUN&acao=A';</script>");

echo "<br>";

// montando aba
if($_REQUEST["atiid"]){
	echo montarAbasArray(carregardadosplanotrabalhoUN_sub(), "/monitora/monitora.php?modulo=principal/planotrabalhoUN/pesquisa_piUN&acao=A&atiid=".$_REQUEST['atiid'].(($_REQUEST['sbaid'])?"&sbaid=".$_REQUEST['sbaid']:""));
} else {
	echo montarAbasArray(carregardadosplanotrabalhoUN_raiz(), "/monitora/monitora.php?modulo=principal/planotrabalhoUN/pesquisa_piUN&acao=A".(($_REQUEST['sbaid'])?"&sbaid=".$_REQUEST['sbaid']:""));
}

// obtém dados da atividade vinculada à ação
$atividade = retornaTarefaUnidade( $_SESSION['monitora_var']['unicod'], $_SESSION['exercicio'] );

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
$arUnidadeSemModalidae[] = '152020';

$boModalidade = true;
$max_length = 2;
if(in_array($_SESSION['monitora_var']['unicod'],$arUnidadeSemModalidae)){
	$boModalidade = false;
	$max_length = 3;
}

?>
<!-- script type="text/javascript" src="../includes/JQuery/jquery.js"></script>
<script type="text/javascript">jQuery.noConflict(); </script -->	
<script type="text/javascript" src="/includes/prototype.js"></script>
<form method="POST"  name="formulario" id="formulario">
<input type="hidden" name="pesquisa" id="pesquisa" value="1">
<input type="hidden" name="primeira" id="primeira" value="<?php echo $primeira; ?>">
<input type="hidden" name="atiid" id="atiid" value="<?php echo $_RESQUEST['atiid']; ?>">
<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="2" align="center">
	<tr>
		<td>
			<?php 
			echo montar_resumo_atividade( $atividade, $numeracao_relativa = true );
			?>
		</td>
	</tr>
</table>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td width="40%" align='right' class="SubTituloDireita">Subação:</td>
	<td>
	<?php
	 
	$sql = "SELECT sub.sbaid as codigo, sub.sbacod || ' - ' || sub.sbatitulo as descricao FROM monitora.pi_subacao sub 
			INNER JOIN monitora.pi_subacaounidade suu ON suu.sbaid = sub.sbaid 
			WHERE suu.unicod='".$_SESSION['monitora_var']['unicod']."' AND sub.sbastatus='A' AND sub.sbasituacao='A' ORDER BY sub.sbacod";
	
	$habilitado = 'S';

	$db->monta_combo('sbaid', $sql, $habilitado,'Selecione','','','',400, '', 'sbaid', '', $_REQUEST['sbaid']);
	
	?>
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

	$db->monta_combo('eqdid', $sql, 'S', 'Selecione', '', '', '', '240', 'N', 'eqdid'); 
	?>
	</td>
</tr>
<tr>
	<td class='SubTituloDireita'>Nível/Etapa de Ensino:</td>
	<td>
		<?
		$sql = "SELECT neeid as codigo, neecod ||' - '|| needsc as descricao
			    FROM monitora.pi_niveletapaensino 
			    WHERE neeano='".$_SESSION['exercicio']."' and neestatus='A' 
			    ORDER BY neecod";
		
		$db->monta_combo('neeid', $sql, 'S', 'Selecione', '', '', '', '240', 'N', 'neeid');
		?>
	</td>
</tr>
<tr>
	<td class='SubTituloDireita'>Categoria de Apropriação:</td>
	<td>
		<?
		$sql = "SELECT capid as codigo, capcod ||' - '|| capdsc as descricao
			    FROM monitora.pi_categoriaapropriacao 
			    WHERE capano='".$_SESSION['exercicio']."' and capstatus='A' 
			    ORDER BY capcod";
		
		$db->monta_combo('capid', $sql, 'S', 'Selecione', '', '', '', '340', 'N', 'capid');
		?>
	</td>
</tr>	
<tr>
	<td align='right' class="SubTituloDireita">Codifição da Unidade(livre):</td>
	<td><? echo campo_texto('plilivre','N','S','',3,$max_length,'','',null,null,null,'id="idCodificacao"',''); ?></td>
</tr>
<?php if($boModalidade){ ?>
<tr>
	<td align='right' class="SubTituloDireita">Modalidade de Ensino / Tema / Público:</td>
    <td>
	<?
	$sql = "SELECT mdeid as codigo, mdecod ||' - '|| mdedsc as descricao
		    FROM monitora.pi_modalidadeensino 
		    WHERE mdeano='".$_SESSION['exercicio']."' and mdestatus='A' 
		    ORDER BY mdecod";
	
	$db->monta_combo('mdeid', $sql, 'S', 'Selecione', '', '', '', '240', 'N', 'mdeid');
 	?>
    </td>
</tr>
<?php } ?>
<tr>
	<td align='right' class="SubTituloDireita">Título:</td>
    <td>
    <input type="hidden" name="plititulo_sub" id="plititulo_sub" value="<? echo $plititulo_sba; ?>">
    <?
	echo campo_texto('plititulo','N','S','',50,45,'','',null,null,null,'id="plititulo"');  
   	?>
    </td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita" valign="top">Descrição / Finalidade:</td>
    <td><? echo campo_textarea( 'plidsc', 'N', 'S', '', 60, 2, 250 ); ?></td>
</tr>
<tr bgcolor="#cccccc">
	<td></td>
	<td align="left">
		<input type="button" class="botao" name="btg" value="Pesquisar" onclick="pesquisar('','');">
	</td>
</tr>
</table>
</form>
<?php 

if(!$boNaoVePlanoInterno) {
	$btExluir = "<a style=\"cursor:pointer;\" onclick=\"removerpi(\''||p.pliid||'\');\"><img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\"></a>";
} else {
	$btExluir = "<a><img src=\"/imagens/excluir_01.gif \" border=0 title=\"Excluir\"></a>";
}
	
if($_REQUEST['atiid']){
	$inner = "INNER JOIN monitora.pi_planointernoatividade pia ON p.pliid = pia.pliid";
	$where_ = "AND pia.atiid = {$_REQUEST['atiid']} ";
}

$sql = "SELECT '<center><a style=\"cursor:pointer;\" onclick=\"alterarpi(\''||p.pliid||'\');\"><img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\"></a>'|| CASE p.plisituacao 
					WHEN 'P' THEN ' $btExluir ' 
					WHEN 'E' THEN ' $btExluir '  
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
				" . ( is_array($where) && count($where) ? ' AND' . implode(' AND ', $where) : '' ) ."
				$where_ 
				GROUP BY p.pliid, p.plicod,p.plititulo,p.plidata,p.plisituacao 
				ORDER BY p.plidata DESC";
	
	$cabecalho = array("","Código do PI","Título","Valor Previsto(R$)","Situação","Última atualização");
	$db->monta_lista($sql,$cabecalho,50,5,'N','center','');
?>
<div id="divBusca"></div>
<script type="text/javascript">

function pesquisar(pesquisa,atiid){

	document.formulario.submit();
	
	/*if(pesquisa){
		var data = "&pesquisa="+pesquisa+"&atiid="+atiid;
	} else {
		var data = $('formulario').serialize(true);
	}

	var req = new Ajax.Request(window.location.href, {
	    method:     'post',
	    parameters: data,
	    asynchronous: false,
	    onComplete: function (res) {
	    	if(res.responseText){
		    	$('divBusca').update(res.responseText);
		    	//return;
	        	//var dados = res.responseText;
	        }
	
	    }
	});	
	*/
}

function alterarpi(pliid){
	//alert(pliid);
	window.location.href = '?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A&pliid='+pliid+'&evento=A';
}

function removerpi(pliid){
	var conf = confirm("Você realmente deseja excluir este PI?");	
	if(conf) {
		window.location.href = '?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A&pliid='+pliid+'&evento=E';
	}
}

var atiid = '<?php echo $_GET['atiid']; ?>';


//pesquisar(1,atiid);
// Presa...
if(document.formulario.primeira == 1){
	pesquisar(1,atiid);
}

</script>