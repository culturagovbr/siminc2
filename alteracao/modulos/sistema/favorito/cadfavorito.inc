<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:cadfavorito.inc
   Finalidade: permitir o controle de cadastro das url favoritas pelo usuário
   */
$modulo=$_REQUEST['modulo'] ;//
if ($_REQUEST['act'] == 'inserir')
{
  $sql= "select favurl from favorito where favurl='".$_SESSION['favurl']."' and usucpf='".$_SESSION['usucpf']."'";
  $usu = $db->recuperar($sql);
	if (is_array($usu)) {
	   // favorito identico, logo, tem que bloquear
	   ?>
	      <script>
	         alert ('A página já se encontra cadastrada no sistema.');
	         history.back();
	      </script>
	   <?
	     exit();
	   };
	if (! isset($_SESSION['favurl'])) {
	   // está tentando gravar o proprio favorito
	   ?>
	      <script>
	         alert ('Não é possível cadastrar Favoritos.');
	         history.back();
	      </script>
	   <?
	     exit();
	   }
	  if ($_REQUEST['favordem']=='' or $_REQUEST['favordem']==0 ) $_REQUEST['favordem']=1;
   $sql = "insert into favorito (favurl,favdsc,favordem,usucpf) values (".
   "'".$_SESSION['favurl']."',".
   "'".$_REQUEST['favdsc']."',".
   $_REQUEST['favordem'].",".
   "'".$_SESSION['usucpf']."')";
    $saida = $db->executar($sql);
	$db->commit();
	$db->sucesso($modulo);
}

if ($_REQUEST['act']=='alterar') 
{
   // fazer alteração de favorito na base de dados.
   if ($_REQUEST['favordem']=='' or $_REQUEST['favordem']==0 ) $_REQUEST['favordem']=1;
       $sql = "update favorito set favdsc='".$_REQUEST['favdsc']."',favordem=".$_REQUEST['favordem']." where favurl='".$_REQUEST['favurl']. "' and usucpf ='".$_SESSION['usucpf']."'";

    $saida = $db->executar($sql);
	$db->commit();
	$db->sucesso($modulo);

}

if ($_POST['exclui'])
{
    $sql = "delete from favorito where favurl='".$_REQUEST['exclui']. "' and usucpf ='".$_SESSION['usucpf']."'";

    $saida = $db->executar($sql);
	$db->commit();
	$db->sucesso($modulo);
}
?>
<br>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>

<title>Listagem de Responsáveis</title>
<div align="center">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <tr bgcolor="#f0f0f0">
    <td width="100%" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#D4D0C8', gradientType='1')"><label class="TituloTela">Administrar Favoritos</label></td>
  </tr>
</table>

<div align="center">
  <center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
      <td width="100%">Por intermédio desta tela você poderá incluir, alterar ou excluir links à sua relação de Favoritos.<br>Um favorito é um link que permite ir direto à página desejada sem necessitar percorrer o passo a passo da navegação.<br><b>ATENÇÃO!</b> No campo Descrição escreva uma frase que lhe permita identificar a página escolhida. (Exemplo: Monitorar a ação... 4045)<br>Note que o sistema já trás uma proposta de nome em função da página escolhida.<br>Complemente, se desejar, a descrição com mais dados identificadores.<br>O campo ordem não é obrigatório.
      
      </td>
    </tr>
  </table>
  </center>
</div>

<center>

<?
if($_REQUEST['favurl']) {
        $sql= "select * from favorito where favurl='".$_REQUEST['favurl']."' and usucpf='".$_SESSION['usucpf']."'";
        $saida = $db->recuperar($sql,$res);
        if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;
        }
?>
<? } else {
	if (! $_REQUEST['favdsc'] ) $favdsc=$_SESSION['mnudsc'];
	else $favdsc = $_REQUEST['favdsc'];
	$favordem =$_REQUEST['favordem'];

 } 
 
 ?>

<form method="POST"  name="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name="favurl" value=<?=$favurl?>>
<input type='hidden' name='exclui' value=0>
<input type='hidden' name='act' value=<?=$_REQUEST['act']?>>

<center>
<br>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='right' class="SubTituloDireita">Descrição:</td>
	<td>
	<?=campo_texto('favdsc','S','S','',50,200,'','');?>
	</td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Ordem:</td>
	<td><?=campo_texto('favordem','N','S','',3,5,'','');?>
	</td>
      </tr>


<? if   ($_REQUEST["favurl"]) { ?>

<tr bgcolor="#CCCCCC">
   <td></td>
   <td><input type="button" name="btalterar" value="Alterar" onclick="validar_cadastro('A')" class="botao">
   <input type="button" name="btvoltar" value="Cancelar" onclick="history.back();" class="botao"></td>
 </tr>
<? } else { ?>
<tr bgcolor="#CCCCCC">
   <td></td>
   <td><input type="button" name="btinserir" value="Incluir" onclick="validar_cadastro('I')" class="botao"></td>

 </tr>
<? } ?>
    </table>
    <br><br>

<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
      <td valign="top" class="title"><strong>Ação</strong></td>
      <td valign="top" class="title"><strong>Ordem</strong></td>
      <td valign="top" class="title"><strong>Descrição (nome fantasia)</strong></td>
    </tr>
</thead>
<tbody>
<?
$sql= "select favurl as codigo, favdsc as descricao, favordem as ordem from favorito where usucpf='".$_SESSION['usucpf']."' order by favordem";
$RS2 = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS2);
if ($nlinhas >= 0) {
for ($i=0; $i<=$nlinhas;$i++){
  $res = $db->carrega_registro($RS2,$i);
// a linha abaixo transforma em variáveis todos os campos do array
  if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
  print '<tr class="'.$marcado.'"><td><img border="0" src="../imagens/fav.gif" title="Ir para a página escolhida." onclick="irpara_fav('."'".$codigo."'".')">&nbsp;&nbsp;&nbsp;<img border="0" src="../imagens/alterar.gif" title="Alterar descrição." onclick="altera_fav('."'".$codigo."'".')">&nbsp;&nbsp;&nbsp;<img border="0" src="../imagens/excluir.gif" title="Excluir de Favoritos." onclick="excluir_fav('."'".$codigo."'".')"></td><td>'.$ordem.'</td><td>'.$descricao.'</td></tr>';
  
} }
else
{
  print '<tr class="'.$marcado.'"><td></td><td>Não há registros de favoritos</td></tr>';
}
?>
</tbody>
    </table>
    </center>
  </div>
</form>

<script>
  function altera_fav(cod) {
    document.formulario.favurl.value = cod;
     document.formulario.submit();
  }
  function excluir_fav(cod) {
  
    if( window.confirm( "Confirma a exclusão de Favoritos ?") )
    {
	document.formulario.exclui.value = cod;
	document.formulario.submit();
    } else document.formulario.exclui.value = 0;
    
  }

    function irpara_fav(cod) {
  
    	 //alert (cod);
       window.opener.location.href=cod;
       self.close();
           
  }
  
function validar_cadastro(cod) 
{

	if (!validaBranco(document.formulario.favdsc, 'Descrição')) return;

	if (cod == 'I') document.formulario.act.value = 'inserir'; else 		document.formulario.act.value = 'alterar';
   	document.formulario.submit();

}    
</script>
