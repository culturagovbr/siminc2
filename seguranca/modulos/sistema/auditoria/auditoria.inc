<?
//Recupera tudo que foi passado por REQUEST e instancia as variáveis correspondentes
foreach($_REQUEST as $k=>$v) ${$k}=$v;
//Montando a cláusula WHERE
if ($usucpf) $wh .= " and u.usucpf like '%".$usucpf."%' ";
if ($usunome) $wh .= " and u.usunome like '%".str_to_upper($usunome)."%' ";
if ($audtipo) $wh .= " and a.audtipo = '".$audtipo."' ";
if ($audtabela) $wh .= " and a.audtabela like '%".$audtabela."%' ";
if ($audsql) $wh .= " and a.audsql like '%".$audsql."%' ";
if ($sisid) $wh .= " and m.sisid =".$sisid;
if ($auddata_ini and $auddata_fim) $wh .=  " and a.auddata<='".formata_data_sql($auddata_fim)." 23:59:59' and a.auddata>='".formata_data_sql($auddata_ini)." 00:00:00' ";
if ($_SESSION['sisid']) $wh.= " and a.sisid=".$_SESSION['sisid'];
if ($wh) $wh = " where ".substr($wh,4,strlen($wh)); else $wh = "";

include APPRAIZ."includes/cabecalho.inc";
?>
<br>
<?
$titul = 'Auditoria Interna do '.SIGLA_SISTEMA;
monta_titulo($titulo_modulo,$titul);
?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <form name="formulario" method="post">
  <tr>
    <td align='right' class="SubTituloDireita">CPF:</td>
    <td><?=campo_texto('usucpf','','','',16,14,'###########','');?></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Nome:</td>
    <td><?=campo_texto('usunome','','','',50,50,'','');?></td>
  </tr>
  <tr>
     <td align='right' class="SubTituloDireita">Sistema:</td>
     <td>
     <?
       $sisid = $_REQUEST['sisid'];
  		$sql="SELECT sisid as codigo, sisdsc as descricao FROM sistema where sisstatus = 'A' order by sisdsc";
		$db->monta_combo("sisid",$sql,'S',"Selecione o Sistema",'','');
	?>
    </td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Operação:</td>
    <td><select name="audtipo" class="CampoEstilo">
								<option value="">Todas</option>
								<option value="I">Inclusão</option>
								<option value="U">Alteração</option>
								<option value="D">Exclusão</option>
								<option value="X">Erros</option>
		</select></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Tabela:</td>
    <td><?=campo_texto('audtabela','','','',30,50,'','');?></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Expressão na Query:</td>
    <td><?=campo_texto('audsql','','','',30,50,'','');?></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Data:</td>
    <td><?=campo_data('auddata_ini', '','S','','N');?> até <?=campo_data('auddata_fim', '','S','','N');?></td>
  </tr>
  <tr bgcolor="#C0C0C0">
    <td ></td>
    <td><input type='button' class="botao" name='consultar' value='Consultar' onclick="Envia();"</td>
  </tr>
</form>
</table>
<?



if ($wh){
$sql = "select a.usucpf as CPF, u.usunome as NOME, a.audtipo as operacao, a.audtabela as tabela, '<text>'||a.audsql||'</text>' as query, m.mnucod||' - '||m.mnutransacao as transacao, to_char(a.auddata,'DD/MM/YYYY HH24:MI:SS') as data from auditoria a inner join usuario u on a.usucpf=u.usucpf inner join menu m on a.mnuid=m.mnuid ".$wh." order by auddata desc";




$db->monta_lista($sql,$cabecalho,50,20,'','','');
}
?>
<script language="JavaScript">
function Envia()
{
document.formulario.submit();
}
</script>
