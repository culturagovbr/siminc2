<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:admexerc.inc
   Finalidade: permitir o controle do exercicio
   */
$modulo=$_REQUEST['modulo'] ;//

$_SESSION['anolimite'] = '2020';
if (($_REQUEST['act'] == 'inserir') or ($_REQUEST['act']=='alterar'))
{
  // verifica o ano
   if ((intval($_REQUEST['prsano']) < 2005)  or (intval($_REQUEST['prsano']) > $_SESSION['anolimite'])) {
   // ano inválido
	   ?>
	      <script>
	         alert ('Ano inválido');
	         history.back();
	      </script>
	   <?
	     exit();   
   }   
}
if ($_REQUEST['act'] == 'inserir')
{
   // fazer inserção de programação simec.
   $sql = "insert into monitora.programacaoexercicio (prsano,prsdata_inicial,prsdata_termino,prsstatus,prsexerccorrente) values (".
   "'".$_REQUEST['prsano']."',".
   "'".$_REQUEST['prsdata_inicial']."',".
   "'".$_REQUEST['prsdata_termino']."',".
    "'".$_REQUEST['prsstatus']."',".
    "'".$_REQUEST['prsexerccorrente']."')";
print $sql;
   $saida = $db->executar($sql);
    $db->commit();
    $db->sucesso($modulo);
}
if ($_REQUEST['act']=='alterar')
{
   if ($_REQUEST['prsexerccorrente']=='t')
   {
   	$sql = "update monitora.programacaoexercicio set prsexerccorrente='f'";
    $saida = $db->executar($sql);
   }
	
   $sql = "update monitora.programacaoexercicio set prsdata_inicial='".
   $_REQUEST['prsdata_inicial'].
   "',  prsdata_termino='".$_REQUEST['prsdata_termino'].
   "',prsstatus='".
   $_REQUEST['prsstatus'].
   "',prsexerccorrente='".
   $_REQUEST['prsexerccorrente'].
   "',prsativo=".
   $_REQUEST['prsativo'].
   ",prsexercicioaberto='".
   $_REQUEST['prsexercicioaberto'].
   "'  where prsano=".$_REQUEST['prsano'];

   $saida = $db->executar($sql);
    $db->commit();
    $db->sucesso($modulo);  
}

include APPRAIZ."includes/cabecalho.inc";
?>
<title>Controle de Exercicio</title>
<script language="JavaScript" src="../includes/calendario.js"></script>
<div align="center">
<?
if($_REQUEST['prsano']) {
        $sql= "select * from monitora.programacaoexercicio where prsano='".$_REQUEST['prsano']."'";
        $saida = $db->recuperar($sql,$res);
        if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;
    }
?>
<? } else { 
	$prsano = $_REQUEST['prsano'];
	$prsdata_inicial =$_REQUEST['prsdata_inicial'];
	$prsdata_termino = $_REQUEST['prsdata_termino'];
	
 }
print '<br>';
$titulo_modulo='Controle de Exercício do Simec';
monta_titulo($titulo_modulo,'');
 ?>
<form method="POST"  name="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name='act' value=0>
<center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
   <tr>
    <td align='right' class="SubTituloDireita">Exercício:</td>
    <td><?=campo_texto('prsano','','','',4,4,'####','');?></td>
   </tr>
   <tr>
     <td align='right' class="SubTituloDireita">Data de início do Exercício:</td>
     <td><?=campo_data('prsdata_inicial', '','S','','S');?>	</td>
   </tr>
   <tr>
      <td align='right' class="SubTituloDireita">Data Final de Exercício:</td>
      <td><?=campo_data('prsdata_termino', '','S','','S');?></td>
   </tr>
      <?
        // verifica se é coordenador do simec ou super-usuário
  $sql= "select usucpf from perfilusuario where pflcod in (4,6) and usucpf='".$_SESSION['usucpf']."'";
  $registro=$db->recuperar($sql);
  if (is_array($registro)) { ?>
      <input type='hidden' name='coord' value='S'>
      <tr>
        <td align='right' class="SubTituloDireita">Libera para Montagem da Estrutura?</td>
	<? if ($prsstatus == 'A') {?>
        <td><input type="radio" name="prsstatus" value='A' checked>Sim&nbsp;&nbsp;<input type="radio" name="prsstatus" value='I' >Não</td>
       <?;} else {?>
        <td ><input type="radio" name="prsstatus" value='A'>Sim&nbsp;&nbsp;<input type="radio" name="prsstatus" value='I' checked>Não<?=obrigatorio();?></td>
      <?} ?>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Marca como Exercício Corrente?</td>
	<? if ($prsexerccorrente == 't') {?>
        <td><input type="radio" name="prsexerccorrente" value='t' checked>Sim&nbsp;&nbsp;<input type="radio" name="prsexerccorrente" value='f' >Não</td>
       <?;} else {?>
        <td ><input type="radio" name="prsexerccorrente" value='t'>Sim&nbsp;&nbsp;<input type="radio" name="prsexerccorrente" value='f' checked>Não<?=obrigatorio();?></td>
      <?} ?>
      </tr>
      <? }?>

	<tr>
        <td align='right' class="SubTituloDireita">Autoriza Restrição e Providência e Parecer de Alta Gestão?</td>
	<? if ($prsexercicioaberto == 't') {?>
        <td><input type="radio" name="prsexercicioaberto" value='t' checked>Sim&nbsp;&nbsp;<input type="radio" name="prsexercicioaberto" value='f' >Não<?=obrigatorio();?></td>
       <?;} else {?>
        <td ><input type="radio" name="prsexercicioaberto" value='t'>Sim&nbsp;&nbsp;<input type="radio" name="prsexercicioaberto" value='f' checked>Não<?=obrigatorio();?></td>
      <?} ?>
      </tr>      
      
      <tr>
        <td align='right' class="SubTituloDireita">Coloca o sistema em manutenção?</td>
	<? if ($prsativo == '0') {?>
        <td><input type="radio" name="prsativo" value='0' checked>Sim&nbsp;&nbsp;<input type="radio" name="prsativo" value='1' >Não</td>
       <?;} else {?>
        <td ><input type="radio" name="prsativo" value='0'>Sim&nbsp;&nbsp;<input type="radio" name="prsativo" value='1' checked>Não<?=obrigatorio();?></td>
      <?} ?>
      </tr>
      

      
<? if   ($_REQUEST["prsano"]) { ?>
<tr bgcolor="#C0C0C0">
   <td></td>
   <td><input type="button" class="botao" name="btalterar" value="Atualizar" onclick="validar_cadastro('A');"><input type="button" class="botao" name="btcancelar" value="Voltar" onclick="history.back();"></td>
 </tr>
<? } else { ?>
<tr bgcolor="#C0C0C0">
   <td></td>
   <td><input type="button" class="botao" name="btinserir" value="Incluir" onclick="validar_cadastro('I');"></td>
 </tr>
<? } ?>
    </table>
    <br><br></form>
<?
$cabecalho = array('Ação','Ano','Período');
$sql= "select  '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Registro \" onclick=\"altera_prs('||prsano||')\">' as acao,  prsano, to_char(prsdata_inicial,'DD/MM/YYYY') ||'  ---   '||to_char(prsdata_termino,'DD/MM/YYYY') as descricao from monitora.programacaoexercicio order by prsano";
$db->monta_lista($sql,$cabecalho,100,20,'','','');

?>

<script>
	function validar_cadastro(cod)
	{
		if (!validaBranco(document.formulario.prsano, 'Exercicio')) return;
		if (!validaBranco(document.formulario.prsdata_inicial, 'Data Início Exercício')) return;

		if (!validaData(document.formulario.prsdata_inicial))
		{
			alert("Data Início Exercício.");
			document.formulario.prsdata_inicial.focus();
			return;
		}

		if (document.formulario.prsdata_termino.value != "")
		{
            if (!validaData(document.formulario.prsdata_termino))
		    {
				alert("Data Término de Exercício Inválida.");
				document.formulario.prsdata_termino.focus();
				return;
            }
            if (!validaDataMaior(document.formulario.prsdata_inicial , document.formulario.prsdata_termino))
		      {
				alert("Data Término não pode ser Anterior que Data Inicio de Exercício.");
				document.formulario.prsdata_termino.focus();
				return;
		      }
            if (cod == 'I') document.formulario.act.value = 'inserir'; else document.formulario.act.value = 'alterar';

   	  	document.formulario.submit();
	}
	}
  function altera_prs(cod) {
    document.formulario.prsano.value = cod;
     document.formulario.submit();
  }

</script>
