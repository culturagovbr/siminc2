<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   M�dulo:associa_resp.inc
   Finalidade: permitir a associa��o de respons�veis por programas
   */

$modulo=$_REQUEST['modulo'] ;//
include APPRAIZ."includes/cabecalho.inc";
if ($_REQUEST['prgid']) $_SESSION['prgid']= $_REQUEST['prgid']; else $_REQUEST['prgid'] = $_SESSION['prgid'];
?>
<title>Associa��o de Respons�veis por Programas do Simec</title>

<? 
abas(2,'monitora.php?modulo=principal/programa/listaprograma&acao=C','Listar programa','monitora.php?modulo=principal/programa/cadprograma&acao=C','Atributo',"<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/programa/cadindicador&acao=C&prgid=".$_REQUEST['prgid'],'Indicadores',"<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/acao/listaacao&acao=C&prgid=".$_REQUEST['prgid'],'A��es',"<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/programa/monitoraprg&acao=A&prgid=".$_REQUEST['prgid'],'Acompanhamento')
?>

<div align="center">
<table  width='100%' border="0" cellspacing="0" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;" align='center'>
  <tr align="center">
    <td width="100%"><strong>Cadastro de Programas - Atributos</strong></td>
     </tr>
</table>
<?

$sql= "select * from programa where prgstatus='A' and prgano = '".$_SESSION['exercicio']."'  order by prgcod";
if ($_POST['procura']) $sql= "select * from programa where prgano = '".$_SESSION['exercicio']."' and prgcod='".$_POST['procura']."'  order by prgcod";
if ($_REQUEST['prgcod']) $sql= "select * from programa where prgano = '".$_SESSION['exercicio']."' and prgcod='".$_REQUEST['prgcod']."'  order by prgcod";
$RS = $db->record_set($sql);

$nlinhas = $db->conta_linhas($RS);
// as linhas abaixo n�o permitem navegar para al�m dos limites
//controla_navegacao($nlinhas);

// fim do controle de navega��o

$res =  $db->carrega_registro($RS,$_POST['registro']);
// a linha abaixo transforma em variáveis todos os campos do array

    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	// encontrar o desafio (macro-objetivo)
 $sql= "select mobdsc from macroobjetivo where mobcod='".$mobcod."'";

    $res = $db->recuperar($sql);

    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar o orgao
    $sql= "select orgdsc from orgao where orgcod='".$orgcod."'";
    $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar a unidade
    $sql= "select unidsc from unidade where unicod='".$unicod."'";
    $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar o tipo de programa
    $sql= "select tprnome from tipoprograma where tprcod='".$tprcod."'";
    $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

?>

<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="orgao" value=0>
<input type=hidden name="registro" value=<?=$_POST['registro'];?>>
<input type=hidden name="navega" value=0>
<input type=hidden name="procura" value=0>
<? // inclusão do menu de setas de navegação
//include "includes/setas_navegacao.inc";?> 

    <center>
    <table width='100%' align='center' border="0" cellspacing="0" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
      <tr>
        <td align='right'>C�digo:</td>
        <td><input type="text" name="prgcod" size="5" value='<?=$prgcod?>'>
	&nbsp;&nbsp;<img border="0" src="../imagens/busca.gif"  alt='Procura programa' title='Procura programa' onclick="ProcuraPrograma(this.form)"> 	
      </tr>
      <tr>
        <td align='right'>Org�o respons�vel:</td>
        <td><input type="text" name="orgdsc" size="60" value='<?=$orgdsc?>'></td>
      </tr>
      <tr>
        <td align='right'>Unidade respons�vel:</td>
         <td>
	<input type="text" name="unidsc" size="60" value='<?=$unidsc?>'></td>
      </tr>
      <tr>
        <td align='right'>Denomina��o:</td>
        <td><input type="text" name="prgdsc" size="60" value='<?=$prgdsc?>'></td>
      </tr>
      <tr>
        <td align='right'>Problema:</td>
        <td>
	<textarea rows="2" name="prgdscproblema" cols="70"><?=$prgdscproblema?></textarea></td>
      </tr>
      <tr>
        <td align='right'>Objetivo do Programa:</td>
        <td>
	<textarea rows="2" name="prgdscobjetivo" cols="70"><?=$prgdscobjetivo?></textarea></td>
      </tr>
      <tr>
        <td align='right'>P�blico-Alvo:</td>
        <td>
	<textarea rows="2" name="prgdscpublicoalvo" cols="70"><?=$prgdscpublicoalvo?></textarea></td>
      </tr>
      <tr>
        <td align='right'>Justificativa:</td>
        <td>
	<textarea rows="2" name="prgdscjustificativa" cols="70"><?=$prgdscjustificativa?></textarea></td>
      </tr>
      <tr>
        <td align='right'>Objetivo setorial associado:</td>
        <td>
	<textarea rows="2" name="prgdscobjetivoassoc" cols="70"><?=$prgdscobjetivoassoc?></textarea></td>
      </tr>
      <tr>
        <td align='right'>Desafio associado:</td>
        <td>
	<textarea rows="2" name="mobdsc" cols="70"><?=$mobdsc?></textarea></td>
      </tr>
      <tr>
        <td align='right'>Programa estruturante?</td>
        <td>
	<?
	   //if ($prgsnestruturante) {
	     // print '<input type="radio" value="1" name="prgsnestruturante" checked>Sim &nbsp;&nbsp; <input type="radio" name="prgsnestruturante" value="0" checked>Não';
	  // }
	  // else
	  // {
	     // print '<input type="radio" value="0" name="prgsnestruturante"> Sim &nbsp;&nbsp; <input type="radio" name="prgsnestruturante" value="1"  checked>Não';
	  // }
	  print 'Dado n�o informado pelo SIGPLAN';
	?>
      </tr>
      <tr>
        <td align='right'>Tipo:</td>
        <td><input type="text" name="tprnome" size="60" value='<?=$tprnome?>'></td>
      </tr>
      <tr>
        <td align='right'>Horizonte temporal:</td>
        <td>
	<?
	   if ($prgsntemporario=='t') 
	   {
	      print '<input type="radio" value="0" name="prgsntemporario"> Cont�nuo &nbsp;&nbsp; <input type="radio" name="prgsntemporario" value="1" checked>Tempor�rio';
	   }
	   else
	   {
	      print '<input type="radio" value="1" name="prgsntemporario" checked> Cont�nuo &nbsp;&nbsp; <input type="radio" name="prgsntemporario" value="0" >Tempor�rio';
	   }
	?>
      </tr>
      <tr>
        <td align='right'>Data In�cio/T�rmino:</td>
        <td><input type="text" name="prgmesinicio" size="2" value='<?=$prgmesinicio?>'>
	<input type="text" name="prganoinicio" size="4" value='<?=$prganoinicio?>'>
	<? if ($prgsntemporario) {
	?>
	  <br>(Se tempor�rio) <input type="text" name="prgmestermino" size="2" value='<?=$prgmestermino?>'><input type="text" name="prganotermino" size="4" value='<?=$prganotermino?>'>
	  <?}?>

	  </td>
      </tr>
       <tr>
        <td align='right'>Valor Global Estimado <br>do Programa (R$) (se tempor�rio)</td>
        <td>
	<? if ($prgsntemporario) {
	?>
            <input type="text" name="prgvlrprograma" size="10" value='<?=$prgvlrprograma?>'>
	  <?}?>
	  </td>
      </tr>
      <tr>
        <td align='right'>Estrat�gia de implementa��o:</td>
        <td>
	<textarea rows="2" name="prgdscestrategia" cols="70"><?=$prgdscestrategia?></textarea>
    </td>
      </tr>
      <tr>
        <td align='right'>Observa��o:</td>
        <td>
	<textarea rows="2" name="prgobs" cols="70"><?=$prgobs?></textarea></td>
      </tr>
    </table> 
    </center>
      </td>
    </tr>
  </table>
  </center>
</div>
  
<? // inclusão do menu de setas de navegação
//include "includes/setas_navegacao.inc";?> 
</form>
<script>
    function VerRegistro(cod) {
	//alert (cod+1);
        document.formulario.navega.value = cod;
	document.formulario.submit();
    }
    function ProcuraPrograma(form) {
	document.formulario.procura.value = document.formulario.prgcod.value;
	document.formulario.submit();
    }
</script>
</body>
</html>
