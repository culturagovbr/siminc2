<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:associa_usuario.inc
   Finalidade: permitir a associação de um perfil ao usuario
   */
$modulo=$_REQUEST['modulo'] ;//
if ($_REQUEST['act']=='cadastra' and $_POST['usucpf']<>'') {
  $sql = 'select * from perfil order by pflcod';
  $RS = $db->record_set($sql);
  $nlinhas = $db->conta_linhas($RS);
  if ($nlinhas >= 0)
   {
     for ($i=0; $i<=$nlinhas;$i++)
     {
       $res = $db->carrega_registro($RS,$i);
       // a linha abaixo transforma em variáveis todos os campos do array
       if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
       if ($_POST[$pflcod]<>'')
       {
         // então marcou a opção
         // procurar se já foi marcada antes. se não foi, então inserir.
           $sql = 'select * from perfilusuario where pflcod='.$pflcod." and usucpf='".$_POST['usucpf']."'";
            $RS2 = $db->record_set($sql);
            $nlinhas2 = $db->conta_linhas($RS2);
            if ($nlinhas2 == -1) {
              // incluir
                $sql = 'insert into perfilusuario (pflcod, usucpf) values ('.$pflcod.",'".$_POST['usucpf']."')";
                 $saida = $db->executar($sql);
            }
       } else
       {
         // caso não tenha marcado, procurar se existe no banco. Se existir, então apagar
            $sql = 'select * from perfilusuario where pflcod='.$pflcod." and usucpf='".$_POST['usucpf']."'";
            $RS3 = $db->record_set($sql);
            $nlinhas3 = $db->conta_linhas($RS3);
            if ($nlinhas3 == 0)
            { // excluir
                 $sql = 'delete from perfilusuario where pflcod='.$pflcod."  and usucpf='".$_POST['usucpf']."'";
                 $saida = $db->executar($sql);
            }
       }
     }
     $db -> commit();
     //$db->sucesso($modulo.'&acao=A&usucpf='.$_POST['usucpf']);
	 print '<script language="JavaScript">window.parent.opener.location.reload();self.close();</script>';
   }
}
?>
<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>Perfil do Usuário</title>
<script language="JavaScript" src="includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
</head>
<body LEFTMARGIN="0" TOPMARGIN="0" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">
<table  align="center" width='100%' border="0" cellspacing="0" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;" align='center'>
  <tr>
    <td width="100%"><strong>Associação de Usuário aos Perfis</strong></td>
  </tr>
</table>
<?
    $usucpf =$_REQUEST['usucpf'];
	$sql = "select usucpf, usunome from usuario where usucpf='".$_REQUEST['usucpf']."'";
    $saida = $db->recuperar($sql);
        if(is_array($saida)) 	   foreach($saida as $k=>$v) ${$k}=$v;
	?>
<form method="POST" name="formulario">
<input type=hidden name="act" value=0>
<input type=hidden name='usucpf' value='<?=$usucpf?>'>
<table width='100%' align="center">
<tr><td>Selecione o Perfil Para o Usuário: <strong><?=$usunome?></strong>
</td></tr></table>
<table  width='100%' border="0" cellspacing="0" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;" align='center' >
<?
// listar apenas os perfis que são de nível menor do que o do usuário logado
// isto impede que ele possa cadastrar alguém com um perfil superior ao dele
  $sql = "select p.pflnivel from perfil p inner join perfilusuario pu on pu.pflcod=p.pflcod and pu.usucpf='".$_SESSION['usucpf']."' order by p.pflnivel";
    $RS = $db->record_set($sql);
    $res = $db->carrega_registro($RS,0);

  $sql = "select * from perfil where pflstatus='A' and pflnivel >= ".$res[0]." and sisid=".$_SESSION['sisid']." order by pfldsc";
  $RS = $db->record_set($sql);
  $nlinhas = $db->conta_linhas($RS);
  if ($nlinhas >= 0)
  {
    for ($i=0; $i<=$nlinhas;$i++)
    {
    // monta a matriz de opções
       $res = $db->carrega_registro($RS,$i);
       // a linha abaixo transforma em variáveis todos os campos do array
       if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
       // verifica se já foi marcado
       $sql = "select * from perfilusuario where pflcod=".$pflcod." and usucpf='".$usucpf."'";
       $RSC = $db->record_set($sql);
       $nlinhasC = $db->conta_linhas($RSC);
       if ($nlinhasC == 0) $cb = '  checked ' ; else $cb = '';
       print '<tr><td><input type="checkbox" name="'.$pflcod.'" value="'.$pflcod.'"'.$cb.' >'.$pfldsc.'</td></tr>';
     }
  }
  else
  {
    print '<tr><td></td><td>Não há registros de Perfil</td></tr>';
  }
?>
<tr bgcolor="#cccccc"><td align="right"><input type='button' value='OK!' onclick="validar_cadastro()">

</td></tr>
</table>
</form>
</div>
<script language="JavaScript">
  function validar_cadastro() {
    document.formulario.act.value = 'cadastra';
	document.formulario.submit();
  }
  function submete_perfil(cod){
    if (document.formulario.pflcod.value != '')
    {
        document.formulario.submit();
    }
  }

</script>
