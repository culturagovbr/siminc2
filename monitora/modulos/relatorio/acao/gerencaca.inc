<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:gerencaca.inc
   Finalidade: permitir a construção de relatorio gerencial de ações
    */

ini_set("memory_limit", "1024M");

if($_POST['requisicao'] == 'montaComboPopAcao'){
	$arPrograma = explode(",", $_POST['arPrograma']);
	$where = "";
	if($arPrograma[0]){
		$where = "and prgcod in ( select prgcod from planejamento.programa where prgid in('". implode("', '", $arPrograma). "' )  )";
	}	
	$sqlCombo = "select distinct
							acacod as codigo,
							acacod ||' - '|| acadsc as descricao 
						from monitora.acao 
						where prgano='".$_SESSION['exercicio']."' 
						and acasnrap = false
						$where			
						order by 2 ";
	
	//echo $sqlCombo; die;
	echo combo_popup( "acacod", $sqlCombo, "Ação(ões)", "600x600", 0, array(), "", "S", false, false, 5, 400 );			
	die;
}


$modulo=$_REQUEST['modulo'] ;//
unset($_SESSION['LINHA']);
unset($_SESSION['title']);
unset($_SESSION['TIT']);
unset($_SESSION['data']);
unset($_SESSION['LIN']);
$_SESSION['LIN']= array();
$_SESSION['TIT']=array();
$rap = $_REQUEST['cbrap'] == 't' ?  't' : 'f';

if ($_REQUEST['relat']=='planilha')
{
	
	unset ($_REQUEST['relat']);
	if ($_REQUEST['planilha']=='3')
	{
    header('Content-type: application/xls');
    header('Content-Disposition: attachment; filename="planilha_simec.xls"');
	}
	else if ($_REQUEST['planilha']=='5')
	{
    header('Content-type: application/csv');
    header('Content-Disposition: attachment; filename="planilha_simec.csv"');		
	}
    for ($i=0;$i<count($_SESSION['cab']);$i++)
    {
	    if ($i<count($_SESSION['cab'])-1) print trim(($_SESSION['cab'][$i])). ' ' . "\t";
	    else print($_SESSION['cab'][$i]);
    }
    print chr(13);
//  if (count($_SESSION['conteudo'][0])>1) print count($_SESSION['conteudo'][0]);
   // else print_r ($_SESSION['conteudo'][10]);

    if (count($_SESSION['conteudo'][0])>1)
    {
    for ($i=0;$i<count($_SESSION['conteudo']);$i++)
    {
        foreach($_SESSION['conteudo'][$i] as $k=>$v)
        {
            ${$k}=$v;
            if ($v == 't') $v='1';
            else if ($v=='f') $v='0';
            if (! is_int($k) and $v <>'') 
            {
               if (is_numeric($v)) $v=number_format($v,2,',','.');//$v=formata_numero($v);
               print $v."\t";
            }
	     }
	     print chr(13);
     }	
   }
   else
   {
    for ($i=0;$i<count($_SESSION['conteudo']);$i++)
    {
        for ($j=0;$j<count($_SESSION['conteudo'][$i]);$j++)
        {
               
        foreach($_SESSION['conteudo'][$i][$j] as $k=>$v)
        {
            ${$k}=$v;
            if ($v == 't') $v='1';
            else if ($v=='f') $v='0';
            if (! is_int($k) and $v <>'') 
            {
               if (is_numeric($v)) $v=number_format($v,2,',','.');//$v=formata_numero($v);
               print $v."\t";
            }
	     }
	     print chr(13);
            }
     }   
   }   
	 exit();
}


if ($_REQUEST['relat']=='planilha_lista')
{
	unset ($_REQUEST['relat']);
	if ($_REQUEST['planilha']=='4')
	{
    header('Content-type: application/xls');
    header('Content-Disposition: attachment; filename="planilha_simec.xls"');
	}
	else if ($_REQUEST['planilha']=='6')
	{
    header('Content-type: application/csv');
    header('Content-Disposition: attachment; filename="planilha_simec.csv"');		
	}
    for ($i=1;$i<count($_SESSION['cab']);$i++)
    {
	    if ($i<count($_SESSION['cab'])-1) print trim(($_SESSION['cab'][$i])). ' ' . "\t";
	    else print($_SESSION['cab'][$i]);
    }
    print chr(13);
    if (count($_SESSION['conteudo'][0])>1)
    {
    for ($i=1;$i<count($_SESSION['conteudo']);$i++)
    {
        foreach($_SESSION['conteudo'][$i] as $k=>$v)
        {
        	if ($k<>'acao'){
            ${$k}=$v;
            if ($v == 't') $v='1';
            else if ($v=='f') $v='0';
            if (! is_int($k) and $v <>'') 
            {
               if (is_numeric($v)) $v=number_format($v,2,',','.');
               print $v."\t";
            }
        	}
	     }
	     print chr(13);
     }	
   }
	 exit();
}




//trata combo_popup para paginação e ordenação por coluna
//campo programa
if(!$_POST['numero'] && !$_POST['ordemlista']){
	if($_POST['prgid'][0]){
		for($i=0; $i<count($_POST['prgid']);  $i++){ 
			$prgidx .= "|" . $_POST['prgid'][$i];
		}
		$_SESSION['prgidaux'] = substr($prgidx, 1);	
	}
	else{
		$_SESSION['prgidaux'] = "";
	}
}
if($_SESSION['prgidaux']){
	$prgidx = str_replace("|","','",$_SESSION['prgidaux']);
	//parametro para query
	$sqlx = "select 
				prgid as CODIGO,
				prgcod ||'-'|| prgdsc as DESCRICAO 
			from programa 
			where prgid in ('".$prgidx."')
			order by prgcod
			"; 
	//$_POST['prgid'] = $db->carregarColuna($sqlx);
	$_REQUEST['prgid'] = $db->carregarColuna($sqlx);
	//preenche combopopup
	$prgid = $db->carregar($sqlx);
}
else{
	if(!is_array($_POST['prgid'])) $_REQUEST['prgid'] = array();
}
												

//campo ação
if(!$_POST['numero'] && !$_POST['ordemlista']){
	
	if($_POST['acacod'][0]){
		for($i=0; $i<count($_POST['acacod']);  $i++){
			$acacodx .= "|" . $_POST['acacod'][$i];
		}
		$_SESSION['acacodaux'] = substr($acacodx, 1);	
	}
	else{
		$_SESSION['acacodaux'] = "";
	}
}
if($_SESSION['acacodaux']){
	$acacodx = str_replace("|","','",$_SESSION['acacodaux']);
	//parametro para query
	$sqlx = "select distinct
				acacod as CODIGO,
				acacod ||' - '|| acadsc as DESCRICAO 
			from acao 
			where acacod in ('".$acacodx."')
			and acasnrap = false
			and prgano = '{$_SESSION['exercicio']}'
			order by 2
			";
	$_REQUEST['acacod'] = $db->carregarColuna($sqlx);
	//preenche combopopup
	$acacod = $db->carregar($sqlx);
}
else{
	if(!is_array($_POST['acacod'])) $_REQUEST['acacod'] = array();
}


//campo inserir ação
if(!$_POST['numero'] && !$_POST['ordemlista']){
	
	if($_POST['inserirAcao'][0]){
		for($i=0; $i<count($_POST['inserirAcao']);  $i++){
			$inserirAcaox .= "|" . $_POST['inserirAcao'][$i];
		}
		$_SESSION['inserirAcaoaux'] = substr($inserirAcaox, 1);	
	}
	else{
		$_SESSION['inserirAcaoaux'] = "";
	}
}
if($_SESSION['inserirAcaoaux']){
	$inserirAcaox = str_replace("|","','",$_SESSION['inserirAcaoaux']);
	//parametro para query
	$sqlx = "select distinct
				acacod as CODIGO,
				acacod ||' - '|| acadsc as DESCRICAO 
			from acao 
			where acacod in ('".$inserirAcaox."')
			and acasnrap = false
			and prgano = '{$_SESSION['exercicio']}'
			order by 2
			";
	
	$_REQUEST['inserirAcao'] = $db->carregarColuna($sqlx);
	//preenche combopopup
	$inserirAcao = $db->carregar($sqlx);
}
else{
	if(!is_array($_POST['inserirAcao'])) $_REQUEST['inserirAcao'] = array();
}




//dump($sqlrel);
if ($_REQUEST['relat']=='r')
{   
  unset($sqlrel);  
  include (APPRAIZ.$_SESSION['sisdiretorio']."/modulos/relatorio/acao/listageraca.inc");
}

include APPRAIZ."includes/cabecalho.inc";
?>
<br>
<div class="notprint">
<?
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Relatório Gerencial de Ação<br>';
monta_titulo($titulo_modulo,'');
?>
<div align="center">
<center>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">


//var arrPrograma = new Array();

function teste(obj){
	jQuery('#divComboAcao').hide();
	jQuery('#divComboAcaoVazio').show();
}

jQuery.noConflict();

jQuery(document).ready(function() {

	jQuery('#inserirAcao').dblclick(function() {
		
		var arPrograma = new Array();

		if(jQuery('#prgid').children().length && jQuery('#prgid option')[0].value){
			jQuery('#prgid option').each(function(){
				arPrograma.push(jQuery(this).val());
			});
		}
			
		var param = new Array();
		param.push({name : 'requisicao', value : 'montaComboPopAcao'}, 
				   {name : 'arPrograma', value : arPrograma}
		);
		jQuery.ajax({
			type	 : "POST",
			url		 : "monitora.php?modulo=relatorio/acao/gerencaca&acao=R",
			data	 : param,
			async    : false,
			//dataType : 'json',
			success	 : function(data){
							jQuery('#divComboAcao').html(data);
							jQuery('#divComboAcaoVazio').hide();
							jQuery('#divComboAcao').show();
							jQuery('#acacod').dblclick();
							//alert(data)
					   }
			 });
		return false;
	});
	
	
	
});


</script>
<form method="POST"  name="formulario">
	<input type='hidden' name="modulo" value="<?=$modulo?>">
	<input type='hidden' name="relat" >
	<input type='hidden' name="planilha" >

	<center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
<tr><td colspan=2 align='center'><b>Escolha Listagem ou Dados Gerais. Se escolher Dados Gerais, marque pelo menos um dos dados. <br>Escolha os itens desejados (Filtros). <br>Pressione o botão "Gerar Relatório"</b></td></tr>
<tr><td colspan=2 align='center'><img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.</td></tr>
<tr><td colspan=2><hr></td></tr>
<tr valign='top'><td ><input type="radio" value="L" name="lista" onclick="mostra_div('L')"
<?if ($_REQUEST['lista']=='L') print '   checked'?>><b>Listagem</b></td><td><input type="radio" value="G" name="lista" onclick="mostra_div('D')" <?if ($_REQUEST['lista']=='G') print '   checked'?>><b>Dados gerais</b><br>
<div style='display:none' id='dadger'>
<input type='checkbox' name='dadobas' value='d' <?if ($_REQUEST['dadobas']) print '   checked'?>>Dados Básicos
&nbsp;&nbsp;
<input type='checkbox' name='avapar' value='avp' <?if ($_REQUEST['avapar']) print '   checked'?>>Avaliação
&nbsp;&nbsp;
<input type='checkbox' name='finevo' value='fin' <?if ($_REQUEST['finevo']) print '   checked'?>>Execução Financeira
<br>
<input type='checkbox' name='fisevo' value='fis' <?if ($_REQUEST['fisevo']) print '   checked'?>>Execução Física
&nbsp;&nbsp;
<input type='checkbox' name='fisevotot' value='fistot' <?if ($_REQUEST['fisevotot']) print '   checked'?>>Execução Física (Totais)<br>
<input type='checkbox' name='plantrab' value='plant' <?if ($_REQUEST['plantrab']) print '   checked'?>>Plano de Trabalho
&nbsp;&nbsp;
<input type='checkbox' name='respro' value='res' <?if ($_REQUEST['respro']) print '   checked'?>>Restrições e Providências
<input type='checkbox' name='paralt' value='pag' <?if ($_REQUEST['paralt']) print '   checked'?>>Parecer da Alta Gestão&nbsp;&nbsp;
</div>
<?
if ($_REQUEST['lista']=='G')
{
  ?>
<script>
   document.getElementById("dadger").style.visibility = "visible";
   document.getElementById("dadger").style.display = "";
</script>
<?}?>
</td></tr>
<tr><td colspan=2><hr></td></tr>
<tr> <td align='right' class="SubTituloDireita" valign="top">Programa:</td>
        <td>
		     <?
		     	
		     	/*
		     	$sql = "select prgid as CODIGO,prgcod ||'-'||prgdsc as DESCRICAO from programa where prgano='".$_SESSION['exercicio']."' order by prgcod ";
		      	$prgid = $_REQUEST['prgid'];
			  	$db->monta_combo("prgid",$sql,'S',"Todos os Programas",'atualizaComboAcao','');
				*/
		     
				$sqlCombo = "select 
										prgid as CODIGO,
										prgcod ||'-'||prgdsc as DESCRICAO 
									from programa 
									where prgano='".$_SESSION['exercicio']."' 
									order by prgcod";
			
				combo_popup( "prgid", $sqlCombo, "Programa(s)", "600x600", 0, array(), "", "S", false, false, 5, 400, null, null, false, null, null, true, false, 'teste(this)');
		     
			 ?>
		</td>
</tr>
<?//if ($_REQUEST["prgid"]) {?>
<tr> <td align='right' class="SubTituloDireita" valign="top">Ação:</td>
        <td valign="top">
        	<div id="divComboAcaoVazio" style="display: block">
        		<?php
					combo_popup( "inserirAcao", $sqlCombo, "Programa(s)", "600x600", 0, array(), "", "S", false, false, 5, 400);
        		?>
			</div>
        	<div id="divComboAcao" style="display: none">
			</div>
		</td>
</tr>
<?//}?>
      <tr>
        <td align='right' class="SubTituloDireita">Orgão:</td>
        <td >
	<?
    $orgcod =$_REQUEST["orgcod"];   
    $sql = "select orgcod as CODIGO,orgdsc as DESCRICAO from orgao where organo='".$_SESSION['exercicio']."' order by orgdsc ";
	  //$db->monta_combo("orgcod",$sql,$habil,"Todos os Órgãos",'atualizaComboUnidade','');
	   $db->monta_combo("orgcod",$sql,'S',"Todos os Órgãos",'atualizaComboUnidade','');
	?><br> <input type="checkbox" name="exceto" value='1' title='Filtrar todos os órgãos com exceção do MEC'><b>Exceto o <?=$_SESSION['ittabrev']?></b></td>
      </tr>
	<?
	  if ($_REQUEST["orgcod"]) {
   $unicod =$_REQUEST['unicod'];
        ?>
      <tr bgcolor="#F2F2F2">
        <td align='right' class="SubTituloDireita">Unidade Orçamentária (UO):</td>
         <td >
         <?

$sql = "select unicod as CODIGO,unicod||' - '||unidsc as DESCRICAO from unidade where unistatus='A' and unitpocod='U' and orgcod ='".$_REQUEST["orgcod"]."' order by unidsc ";
$db->monta_combo("unicod",$sql,'S',"Todas as Unidades",'atualizaComboUnidade','');
?>
	</td>
      </tr>
<?}?>
	  <?
	      $org = substr($_SESSION['ittorgao'],0,2);
	  if ($unicod == $org.'101' and $orgcod==$_SESSION['ittorgao']) {?>
      <tr bgcolor="#F2F2F2">
        <td align = 'right' class="subtitulodireita">Unidade Gestora (UG):</td>
         <td >
	<?
	  $sql = "select ungcod as CODIGO,ungcod||' - '||ungdsc as DESCRICAO from unidadegestora where ungstatus='A' and unitpocod='U' and unicod ='".$unicod."' order by ungdsc ";
	  $db->monta_combo("ungcod",$sql,'S',"Todas as Unidades Gestoras",'','');
	   print obrigatorio();
	?>
	</td>
      </tr>

	  <?}
	?>
      <tr bgcolor="#F2F2F2">
        <td align='right' class="SubTituloDireita">Grupo de Unidade:</td>
         <td >
         <?
         $sql = "select gunid as CODIGO,gundsc as DESCRICAO from grupounidade where gunstatus='A' order by gundsc ";
$db->monta_combo("gunid",$sql,'S',"Todos os Grupos",'','');
?>
	</td>
      </tr>	
<tr><td align='right' class="SubTituloDireita">Somente Código (separe com ponto e vírgula):</td>
    <td><?=campo_texto('acacodbusca','N','S','Entre com os códigos separados por ponto e vírgula.Exemplo:09hk;6336',100,100,'','');?></td>
</tr>
<tr><td align='right' class="SubTituloDireita">Título (ou parte):</td>
    <td><?=campo_texto('acadsc','N','S','Entre com uma parte do Título (qualquer parte)',100,100,'','');?></td>
</tr>
<tr>
<td align='right' class="SubTituloDireita">Produto:</td>
        <td>	<?$sql = "select procod as CODIGO,prodsc as DESCRICAO from produto where prostatus='A' order by prodsc ";
	  $db->monta_combo("procod",$sql,'S',"Todos os Produtos",'','');
	?></td>
</tr>
<tr> <td align='right' class="SubTituloDireita">Unidade de Medida:</td>
        <td>	<?$sql = "select unmcod as CODIGO,unmdsc as DESCRICAO from unidademedida where unmstatus='A' order by unmdsc ";
	  $db->monta_combo("unmcod",$sql,'S',"Todas as Unidades",'','');
	?></td>
</tr>
<tr><td align='right' class="SubTituloDireita">Tipo:</td>
    <td><?$sql = "select taccod as CODIGO,tacdsc as DESCRICAO from tipoacao where tacstatus='A' order by tacdsc ";
	  $db->monta_combo("taccod",$sql,'S',"Todos os Tipos",'','');
	?></td>
</tr>
<tr><td align='right' class="SubTituloDireita">Região:</td>
    <td><?$sql = "select regcod as CODIGO,regdsc as DESCRICAO from regiao where regstatus='A' order by regdsc ";
	  $db->monta_combo("regcod",$sql,'S',"Todos as Regiões",'','');
	?></td>
</tr>
<tr><td colspan=2><hr></td></tr>
<tr><td align='right' class="SubTituloDireita">RAP:</td>
    <td><input type='radio' value='f' name='cbrap'>Não<input type='radio' value='t' name='cbrap'>Sim</td>
</tr>
<tr><td align='right' class="SubTituloDireita">Emenda:</td>
    <td><input type='radio' value='f' name='cbemenda'>Não<input type='radio' value='t' name='cbemenda'>Sim</td>
</tr>
<tr><td colspan=2><hr></td></tr>
<tr><td align='right' class="SubTituloDireita">Data de fim do projeto:</td>
        <td>
		<?=campo_data('dataproj', 'N','S','','S');?> Só é testada para Ação tipo Projeto </td>
</tr>
<tr><td colspan=2><hr></td></tr>
<tr><td align='right' class="SubTituloDireita">Restrições:</td>
    <td> <input type="radio" name="cbrp" value='N'>Ainda sem solução <input type="radio" name="cbrp" value='S'>Solucionadas  <input type="radio" name="cbrp" value='A'>Ambas</td>
</tr>
<tr><td colspan=2><hr></td></tr>
<tr><td align='right' class="SubTituloDireita">Com Parecer da Alta Gestão:</td>
    <td><input type="checkbox" name="cbpalt" value='REST'></td>
</tr>
</tr>
<tr><td colspan=2><hr></td></tr>
<tr><td></td><td><b>Avaliação</b></td></tr>
<tr><td align='right' class="SubTituloDireita">Sem Avaliação:</td>
    <td><input type="checkbox" name="cbsaval" value='saval'></td>
</tr>
<tr><td align="right" class="subtitulodireita">Situação da Avaliação:</td>
<td>
<?    $sql = "select tpscod as CODIGO,tpsdsc as descricao from tiposituacao where tpsstatus='A' order by tpsdsc ";
	  $db->monta_combo("tpscodav",$sql,'S',"Todas as situações",'','');
	  ?>
</td></tr>
<tr><td align="right" class="subtitulodireita">Cor da avaliação:</td>
<td><?
	$sql = "select corcod as CODIGO,corsignificado as desc , corimgav as imagem from cor where corstatus='A' order by corcod desc ";
	$RSradio = $db->record_set($sql);
	$nlinhasradio = $db->conta_linhas($RSradio);
	for ($j=0; $j<=$nlinhasradio;$j++)
	{
	    $res2 =  $db->carrega_registro($RSradio,$j);
	    if(is_array($res2)) foreach($res2 as $k=>$v) ${$k}=$v;
        print "<input type='radio' value='".$codigo."' name='corcodav'  title='".$desc."'>";
	    print "<img src='../imagens/".trim($imagem)."' title='".$desc."'>";
   }
?>
</td></tr>
<tr><td colspan=2><hr></td></tr>
<tr><td></td><td><b>Validação</b></td></tr>
<tr><td align='right' class="SubTituloDireita">Não validado:</td>
    <td><input type="checkbox" name="cbsvalid" value='cbs'></td>
</tr>

<tr><td align='right' class="SubTituloDireita">Validação:</td>
    <td> <input type="radio" name="cbval" value='1'>Concordância <input type="radio" name="cbval" value='2'>Pendência  <input type="radio" name="cbval" value='3'>Discordância</td>
</tr>


<tr><td colspan=2><hr></td></tr>
<tr><td align="right" class="subtitulodireita">Periodo:</td>
<td>Início <?=campo_data('dataini', 'N','S','','S');?> &nbsp;&nbsp;Fim <?=campo_data('datafim', 'N','S','','S');?> (Obs: O Período só é considerado para efeito de monitoramento)

</td></tr>

<tr bgcolor="#CCCCCC">
   <td></td>   <td align='center'><input type="button" name="btrelat" value="Gerar Relatório" onclick="validar_relatorio()" class="botao"></td>
</tr>
</table>
</center>
</div>
</form>
</div>
<?

if ($sqlrel){
//print $sqlrel;
  if ($_REQUEST['lista']=='L')
     include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/relatorio/acao/montageraca.inc";
  else
     include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/relatorio/acao/montadadosaca.inc";
}
?>

<script>
function validar_relatorio()
{
	if (!validaRadio(document.formulario.lista,'Listagem ou Dados Gerais')) return;
    // se existir data de fim do projeto, então ela deve ser válida
    if (document.formulario.dataproj.value !='')
    {
       if (!validaData(document.formulario.dataproj))
       {
     	  alert("Data de fim do projeto inválida.");
		  document.formulario.dataproj.focus();
		  return;
       }
    }
	// se existir data de inicio, então ela deve ser válida
    if (document.formulario.dataini.value !='')
    {
      if (!validaData(document.formulario.dataini))
      {
     	alert("Data de início inválida.");
		document.formulario.dataini.focus();
		return;
      }
    }

    // se existir data de fim, então ela deve ser válida
    if (document.formulario.datafim.value !='')
    {
      if (!validaData(document.formulario.datafim))
      {
     	alert("Data de fim inválida.");
		document.formulario.datafim.focus();
		return;
      }
     }
    // se não existir data de inicio, então não pode existir data de fim
    if (document.formulario.dataini.value == '')
    {
      if (document.formulario.datafim.value != '')
      {
     	alert("Se existe data de fim, deve existir data de início.");
		document.formulario.dataini.focus();
		return;
      }
    }
    // se existir data de inicio, então tem que existir data de fim
    if (document.formulario.dataini.value !='')
    {
      if (document.formulario.datafim.value == '')
      {
     	alert("Se existe data de inicio, deve existir data de fim.");
		document.formulario.datafim.focus();
		return;
      }
    }
    
    // se não existir data de fim, então não pode existir data de inicio
    if (document.formulario.datafim.value =='')
    {
      if (document.formulario.dataini.value != '')
      {
     	alert("Se existe tdata de fim, deve existir data de inicio.");
		document.formulario.dataini.focus();
		return;
      }
    }
    // a data fim tem que ser posterior a data inicio
     if (!validaDataMaior(document.formulario.dataini , document.formulario.datafim))
	  {
		alert("Data de Fim não pode ser Anterior que Data Inicio.");
		document.formulario.datafim.focus();
		return;
      }
    
      //Deve ser informado um códido de ação
	  var objacacod = document.getElementById("acacod");
	  var objprgid = document.getElementById("prgid");
	  selectAllOptions(formulario.acacod);
	  selectAllOptions(formulario.prgid);
	  
	  if(objacacod.value == "" && objprgid.value == "") {
			alert("Deve ser informado ao menos um programa ou uma ação.");
			return;			
	  }
	  
	  /*      
      if(document.formulario.acaid){
      	 if (document.formulario.acaid.value == '' && document.formulario.acacodbusca.value == '')
		  {
			alert("Deve ser informado ao menos um código de ação.");
			document.formulario.acacodbusca.focus();
			return;
	      }
      }
    */
    

	if (document.formulario.lista[1].checked)
    {
       if (! document.formulario.plantrab.checked && ! document.formulario.dadobas.checked && ! document.formulario.avapar.checked && ! document.formulario.finevo.checked && ! document.formulario.fisevo.checked && ! document.formulario.fisevotot.checked && ! document.formulario.respro.checked && ! document.formulario.paralt.checked ) {
       	alert ('Preencher pelo menos um dos Dados Gerais'); return;
       }
    }
    
    document.formulario.relat.value='r';
 
   	document.formulario.submit();
   	
}


function alterar_cad(cod,cod2)
{
   	location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/acao/cadacao&acao=C&acaid="+cod+"&prgid="+cod2;

}

function imprime_rel(cod)
{
		     //Abre popup em branco
   	 janela = window.open('about:blank',"relatorio","menubar=no,location=no,open=yes,resizable=yes,scrollbars=yes,status=yes,width=600,height=400'");
     if (cod == 1) {
   	   //Abre relatório pdf no popup
   	 janela.location = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_geral1&acao=R";
     }
     if (cod== 2) {
           janela.location = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_gerenaca&acao=R";
    }
    if (cod>2) {
         	if (cod ==3) document.formulario.relat.value='planilha';
          	if (cod ==4) document.formulario.relat.value='planilha_lista'; 
          	if (cod ==5) document.formulario.relat.value='planilha';
          	if (cod ==6) document.formulario.relat.value='planilha_lista';             	
          	document.formulario.planilha.value=cod;         	
                document.formulario.submit();
    }
}

  function atualizaComboUnidade(cod)
  {
	 if (cod > 0) {
		 	selectAllOptions( document.formulario.prgid );
		 	selectAllOptions( document.formulario.inserirAcao );
		 	selectAllOptions( document.formulario.acacod );
		 	
		 	//document.formulario.submit();
	 	}
    }
  function atualizaComboAcao(cod)
  {
	 if (cod > 0) {
		 selectAllOptions( document.formulario.prgid );
		 selectAllOptions( document.formulario.inserirAcao );
		 selectAllOptions( document.formulario.acacod );
		 	
		// document.formulario.submit();
	 }
    }
    
  function mostra_div(cod)
  {
    if (cod == 'L')
    {
	document.getElementById("dadger").style.visibility = "hidden";
	document.getElementById("dadger").style.display = "none";
     }
    if (cod == 'D')
    {
		document.getElementById("dadger").style.visibility = "visible";
		document.getElementById("dadger").style.display = "";
     }
  }

</script>
