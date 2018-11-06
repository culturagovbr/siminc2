<?
// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

# Verifica se a sessão não expirou, se tiver expirada envia pra tela de login.
controlarAcessoSemAutenticacao();

$db = new cls_banco();

$modulo=$_REQUEST['modulo'] ;//
$usuCPF = $_REQUEST['usuID'];

if ($_REQUEST['email']){
if (ereg_replace("<[^>]*>","",$_REQUEST['email']) == '')
{
	   ?>
	      <script>
	         alert ('O texto do e-mail não pode estar vazio.');
	         history.back();
	      </script>
	   <?
	     exit();
}
else
{
  // envia email
  $sql="select usunome,usuemail from usuario where usucpf='".$_REQUEST['usucpf']."'";
  $RSu = $db->record_set($sql);
  $resu =  $db->carrega_registro($RSu,0);
  if(is_array($resu)) foreach($resu as $k=>$v) ${$k}=$v;
  $assunto = $_REQUEST['assunto'];
  $mensagem = $_REQUEST['email'];
  $corpoEmailV3 = $mensagem;
  include APPRAIZ . "includes/email-template.php";
//ver($textoEmailV3, d);
  $emailSession = $_SESSION['usuemail'];
  $paraUsu = $_REQUEST['usunome'];
  $cc=$_REQUEST['cc'];
  $cco=$_REQUEST['cco'];  
  enviar_email_usuario($emailSession, $paraUsu, $assunto, $textoEmailV3, $cc, $cco);
  ?>
      <script>
         alert('Email enviado com sucesso. Esta janela será fechada.')
         window.close();
      </script>
  <?
  exit();

}
}

$sqlEmail = "
SELECT u.usuemail, CASE WHEN u.entid is not null THEN e.entnome ELSE u.orgao END as orgaonome FROM seguranca.usuario u
LEFT JOIN entidade.entidade e ON u.entid = e.entid
WHERE u.usucpf = '$usuCPF'";
$sqlUsu  = $db->carregar( $sqlEmail );
$sqlUsu = $sqlUsu ? $sqlUsu : array();
foreach ( $sqlUsu as $item ) { $emailUsu = $item["usuemail"]; $usuOrgao = $item["orgaonome"];}

$sql="select u.usunome,e.entnome from usuario u left join entidade.entidade e on u.entid = e.entid where u.usucpf='".$_REQUEST['cpf']."'";
$RSu = $db->record_set($sql);
$resu =  $db->carrega_registro($RSu,0);
if(is_array($resu)) foreach($resu as $k=>$v) ${$k}=$v;

?>
<html>
<head>
<title>Envio de Email</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="../includes/funcoes.js"></script>

</head>
<body bgcolor="#ffffff" leftmargin="0" rightmargin="0" topmargin="0" bottommargin="0" marginheight="0" marginwidth="0">
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="cpf" value="<?=$_REQUEST['cpf']?>">

    <center>
    <table width='100%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
     <tr>
	 <td colspan="2" align="Center" bgcolor="#dedede">Enviar Email</td>
	 </tr>
	  <tr>
        <td align="right" class="subtitulodireita">Para:</td> 
        <td><?=campo_texto('usunome','N','N','',70,100, '', '','','','','','',$emailUsu,'');?></td>
     </tr>
	  <tr>
        <td align="right" class="subtitulodireita">Órgão:</td> 
        <td><?=campo_texto('orgdsc','N','N','',70,100, '', '','','','','','',$usuOrgao,'');?></td>
     </tr>
	  <tr>
        <td align="right" class="subtitulodireita">Cc:</td> 
        <td><?=campo_texto('cc','N','S','',70,100,'','');?></td>
     </tr>
     <tr>
        <td align="right" class="subtitulodireita">Cco:</td> 
        <td><?=campo_texto('cco','N','S','',70,100,'','');?></td>
     </tr>     
	  <tr>
        <td align="right" class="subtitulodireita">Assunto:</td> 
        <td><?=campo_texto('assunto','S','S','',70,100,'','');?></td>
     </tr>
	  <tr>
        <td align="right" class="subtitulodireita">ATENÇÃO:</td> 
        <td><b><font color="red">EVITE COPIAR TEXTOS FORMATADOS DO WORD PORQUE SE O DESTINATÁRIO UTILIZAR O OUTLOOK, A MENSAGEM PODE FICAR CONFUSA E ININTELIGÍVEL!</font></td>
     </tr> 
     <tr>    
        <? $email= '';?>
        <td colspan=2>
        <?=campo_textarea('email','N','S','',132,20,'');?>
        </td>
     </tr>
	 <tr>
	 <td colspan="2" align="right" class="subtitulodireita"><input type='button' class="botao" value='Enviar E-mail' onclick="envia_email()">&nbsp;&nbsp;&nbsp;<input type='button' class="botao" value='Fechar' onclick="fechar_janela()"></td>
	 </tr>
  </table>
</form>
<script language="JavaScript" src="../includes/tiny_mce.js"></script> 
<script type="text/javascript">
	// Editor de textos
	tinyMCE.init({
		mode : "textareas",
		theme : "advanced",
		plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
		theme_advanced_buttons1 : "undo,redo,separator,bold,italic,underline,forecolor,backcolor,fontsizeselect,separator,justifyleft,justifycenter,justifyright, justifyfull, separator, outdent,indent, separator, bullist, code",
		theme_advanced_buttons2 : "",
		theme_advanced_buttons3 : "",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
		language : "pt_br",
		entity_encoding : "raw"
	});
	function fechar_janela() {
				window.close();
			}
			
	function envia_email() {
		if ( !validaBranco( document.formulario.assunto, 'Assunto' ) ) {
			return;
		}
		document.formulario.submit();
	}
</script>
</body>
</html>
