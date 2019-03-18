<?
//Recupera tudo que foi passado por REQUEST e instancia as variáveis correspondentes
foreach($_REQUEST as $k=>$v) ${$k}=$v;
if($mnuhelp) {

 $texto = stripslashes($mnuhelp);
 $texto = str_replace("'","''",$texto);
$sql = "update menu set mnuhelp='".str_replace("'","''",$texto)."' where mnuid=".$mnuid;

$saida = $db->executar($sql);
    $db->commit();
	?><script>
              alert('Operação realizada com sucesso');
			  location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=ajuda/ajuda&acao=C&mnuid=<?=$mnuid?>";
     </script>
	 <?exit();
}
$sql= "select * from menu where mnuid=".$mnuid;

$saida = $db->recuperar($sql,$res);
if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;}
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">

<html>
<head>
	<title>Ajuda do <?php echo SIGLA_SISTEMA; ?></title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script language="JavaScript">
self.focus();
//Editor de textos
tinyMCE.init({
	mode : "textareas",
		theme : "advanced",
		plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
		theme_advanced_buttons1_add_before : "save,newdocument,separator",
		theme_advanced_buttons1_add : "fontselect,fontsizeselect",
		theme_advanced_buttons2_add : "separator,insertdate,inserttime,preview,zoom,separator,forecolor,backcolor",
		theme_advanced_buttons2_add_before: "cut,copy,paste,pastetext,pasteword,separator,search,replace,separator",
		theme_advanced_buttons3_add_before : "tablecontrols,separator",
		theme_advanced_buttons3_add : "emotions,iespell,flash,advhr,separator,print,separator,ltr,rtl,separator,fullscreen",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		theme_advanced_path_location : "bottom",
		plugin_insertdate_dateFormat : "%d-%m-%Y",
	    plugin_insertdate_timeFormat : "%H:%M:%S",
		extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
		external_link_list_url : "example_link_list.js",
		external_image_list_url : "example_image_list.js",
		flash_external_list_url : "example_flash_list.js",
		file_browser_callback : "fileBrowserCallBack",
		//language : "pt_br",
	entity_encoding : "raw",
	width : "100%"
	});

function fileBrowserCallBack(field_name, url, type, win) {
	// Do custom browser logic
	win.document.forms[0].elements[field_name].value = 'my browser value';
}

</script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
</head>
<body LEFTMARGIN=0 TOPMARGIN=0 MARGINWIDTH="0" MARGINHEIGHT="0" style="background-color: #ffffcc;">
<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notprint">

  <tr bgcolor="#FFEDA4">
	<td nowrap align="left"><a href="javascript:janela('fale_conosco.php?uc=<?=$_SESSION['usucpf'];?>',550,600)" style="color:#000000;margin-right:10px;margin-bottom:5px;">Fale Conosco</a></td>
<td nowrap align="right">
	<a href="javascript:janela('../abre_manual.php',300,150)" style="color:#000000;margin-right:10px;margin-bottom:5px;"><b>Manual</b></a>
  </tr>


  <tr bgcolor="#FFEDA4"> 
    <td height="25" valign="middle"><font face="Verdana, Arial, Helvetica, sans-serif" size="2"><b><IMG height=21 src="../imagens/IconeAjuda.gif" width=25 align=absBottom>&nbsp;<?=$mnudsc?></b></font></td>
	<td align="right" height="25"><?print '<font color="#0000cc">Objetivo:</font> '.$mnutransacao;?></td>
	</tr>
	<tr>
	<td colspan="2" style="padding:10px;">
		<?
		//Verifica se é superusuario
		$sql= "select usucpf from perfilusuario pu inner join perfil p on p.pflcod=pu.pflcod where p.pflsuperuser='t' and pu.usucpf = '".$_SESSION['usucpf']."'";
  $registro=$db->recuperar($sql);
		if (is_array($registro)){?>
		Digite abaixo o texto da ajuda para o módulo:<br>
		<form method="post" name="formulario">
		<input type="Hidden" name="mnuid" value="<?=$mnuid?>">
		<textarea name="mnuhelp" rows="30"><?=$mnuhelp;?></textarea>
		<br><div align="right">
		<input type="Button" name="Cadastrar" value="Cadastrar Ajuda" onclick="cadastra();">&nbsp;&nbsp;&nbsp;<input type="Button" name="Fechar" value="Fechar" onclick="self.close();">&nbsp;&nbsp;&nbsp;
		</div>
		</form>
	 
<script language="JavaScript">
//Redimensiona a janela no caso de ser superusuario
top.resizeTo(700,550);


function cadastra()
{
document.formulario.submit();
}
</script>
<?} else {
	   //Verifica se existe ajuda cadastrada
	   if ($mnuhelp) print $mnuhelp; else print '<div align="center"><font color="#ff0000">Ajuda não cadastrada!</font></div>';
}?>
	</td>
   </tr>
</table>
</body>
</html>






