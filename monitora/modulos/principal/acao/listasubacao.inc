<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   M�dulo:listasubacao.inc
   Finalidade: permitir o controle da lista de suba��es
   */

$modulo=$_REQUEST['modulo'] ;//
if ($_SESSION['prgcod']) $_REQUEST['prgcod'] = $_SESSION['prgcod'];
  if ($_REQUEST['acaid'])
  {
     // achar os atributo de acao que interessam
     $sql= "select acadsc from acao where acaid = ".md5_decrypt($_REQUEST['acaid'],'');
     $RS = $db->record_set($sql);
     $res = $db->carrega_registro($RS,0);

     $sql= "select sao.* , a.acadsc from subacao sao, acao a where sao.saostatus = 'A' and sao.acaid = a.acaid and sao.acaid = '".md5_decrypt($_REQUEST['acaid'],'')."' order by sao.saodsc";

     $RS = $db->record_set($sql);
     $nlinhas = $db->conta_linhas($RS);
  }
include APPRAIZ."includes/cabecalho.inc";

?>

<title>Lista de A��es do Simec</title>

<body>

<? 
abas(4,'monitora.php?modulo=principal/programa/listaprograma','Listar programa','monitora.php?modulo=principal/programa/cadprograma','Atributo',"<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/programa/cadindicador&prgcod=".$_REQUEST['prgcod'],'Indicadores',"<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/acao/listaacao&prgcod=".$_REQUEST['prgcod'],'A��es',"<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/programa/monitoraprg&prgcod=".$_REQUEST['prgcod'],'Acompanhamento')
?>


<div align="center">
<table  width='100%' border="0" cellspacing="0" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;" align='center'>
  <tr align="center">
    <td width="100%"><strong>Lista de Sub-a��es da A��o -> <?=$res['acadsc'].' ('.($nlinhas+1).')'?></strong>
    </td>
  </tr>
</table>
<table  align='center'  width='100%' border="0" cellspacing="0" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
  <tr>
    <td width="100%"><font color=red><strong><?print $_SESSION['msgsistema']; $_SESSION['msgsistema'] = '' ?></strong></font></td>
  </tr>
</table>

<table width='100%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
      <td valign="top" class="title"><strong>C�digo</strong></td>
      <td valign="top" class="title"><strong>Descri��o</strong></td>
      <td valign="top" class="title"><strong>Alterar/Excluir</strong></td>
      <td valign="top" class='title'><a href="simec?modulo=principal/acao/saoadm&acaid=<?= $_REQUEST['acaid']?>><strong>Incluir</a></strong></td>
    </tr>
</thead>
<tbody>
<?
for ($i=0; $i<=$nlinhas;$i++){
  $res = $db->carrega_registro($RS,$i);
  if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
   //print '<tr class="'.$marcado.'"><td><a href=simec?modulo=principal/acao/cadacao&acaid='.$acaid.'>'.$codigo.'-'.$unicod.'-'.$loccod.'</a></td><td>'.$descricao.'<br>Localizador:'.$locdsc.'<br>'.$sacdsc.'</td>';
print '<tr class="'.$marcado.'"><td>'.$saocod.'</td><td>'.$saodsc.'</td>';
   print '<td><a href=simec?modulo=principal/acao/saoadm&acaid='.md5_encrypt($acaid,'').'>Alterar/Excluir</a></td><td></td></tr>';
}
?>
</tbody>
    </table>

          </td>
    </tr>
  </table>
  </center>
</div>
