<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   M�dulo:relmonitoraprg.inc
   Finalidade: permitir a constru��o de relatorio completo de monitoramento de programa
    */
$modulo=$_REQUEST['modulo'] ;//
if ($_REQUEST['prgid']) $_SESSION['prgid']= $_REQUEST['prgid']; else $_REQUEST['prgid'] = $_SESSION['prgid'];
unset($_SESSION['LINHA']);
unset($_SESSION['title']);
unset($_SESSION['TIT']);
unset($_SESSION['data']);
unset($_SESSION['LIN']);
unset($sql2);
$nlinhasM = -1;
$_SESSION['LIN']= array();
$_SESSION['TIT']=array();

if ($_REQUEST['relat']=='r')
{
   // monta relatorio
  $i=0;

  $sql = "select ref.refdsc, av.avptexto as avaliacao, av.avpliberada as avlib,av.usucpf as avusu, to_char(av.avpdata,'DD/MM/YYYY HH:MM') as avdata, ca.corsignificado as avcordsc, tsa.tpsdsc as avtpsdsc, tsp.tpsdsc as partpsdsc , pa.avpliberada as parlib, pa.avptexto as parecer,pa.usucpf as parusu, to_char(av.avpdata,'DD/MM/YYYY HH:MM') as pardata,  cp.corsignificado as pacordsc, tsa.tpsdsc as patpsdsc from referencia ref left join avaliacaoparecer av on av.refcod=ref.refcod and av.tpaid=1 and av.prgid =".$_SESSION['prgid']."  left join cor ca on av.corcod = ca.corcod left join tiposituacao tsa on tsa.tpscod = av.tpscod  left join avaliacaoparecer pa on pa.refcod=ref.refcod and pa.tpaid=2 and pa.prgid = ".$_SESSION['prgid']."   left join cor cp on pa.corcod = cp.corcod  left join tiposituacao tsp on tsp.tpscod = pa.tpscod  where ref.refdata_limite_parecer_aca is not null and ref.refsnmonitoramento='t' and ref.refano_ref=".$_SESSION['exercicio'] ;

  $titul = 'Relat�rio de Monitoramento de Programa: ';
  $tit = 'Relat�rio de Monitoramento de Programa: ';
  if ($_REQUEST['refcod'])
  {
    $sql = $sql." and ref.refcod=".$_REQUEST['refcod'];
    $sqldsc = "select refdsc from referencia where refcod=".$_REQUEST['refcod'];
    $titulo=$db->recuperar($sqldsc);
    $titul2= $titul.'| dentro da refer�ncia '.$titulo['refdsc'];
    $tit2 = $tit . '| do '.$titulo['refdsc'];
  }
  if ($i == 0) unset($sqlrel);
  $sql1 = $sql ."  order by ref.refano_ref, ref.refmes_ref";
  $sqlrel=$sql1;
}

if ($_REQUEST['prgid']) $sql2= "select prgdsc, prgcod, prgid from programa where prgano = '".$_SESSION['exercicio']."' and prgid=".$_REQUEST['prgid']."  order by prgcod";
if ($_POST['procura']) $sql2= "select prgdsc, prgcod, prgid from programa where prgano = '".$_SESSION['exercicio']."' and prgcod='".$_POST['procura']."'  order by prgcod";

if ($sql2){
$RS = $db->record_set($sql2);
$nlinhasM = $db->conta_linhas($RS);
}
if ($nlinhasM < 0 and $sql2 <> ''){
?>
<script language="JavaScript">
alert('Programa "<?=$_REQUEST['procura'];?>" n�o encontrado!\n\nTente Novamente.');
history.back();
</script>
<?} else {
if ($nlinhasM >= 0)
{
   $res =  $db->carrega_registro($RS,0);
   if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
   $_SESSION['prgid']= $prgid;

if (! $_REQUEST['refcod'])
{
  $sql = "select refcod from referencia where refsnmonitoramento='t' and refano_ref='".$_SESSION['exercicio']."' order by refano_ref desc,refmes_ref desc";
$RS3 = $db->record_set($sql);
$res =  $db->carrega_registro($RS3,0);
$_REQUEST['refcod']= $res[0];
}
}

include APPRAIZ."includes/cabecalho.inc";
?>
<br>
<?
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Relat�rio de Monitoramento de Programas';
monta_titulo($titulo_modulo,'');
?>
<div align="center">
<center>

<form method="POST"  name="formulario" onsubmit="ProcuraPrograma();">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name="relat" >
<input type=hidden name="procura" value=0>

<center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
<tr>
        <td align='right' class="subtitulodireita" width="20%">C�digo:</td>
        <td><?=campo_texto('prgcod','N','S','',8,100,'','');?>
	&nbsp;&nbsp;<img border="0" src="../imagens/busca.gif"  alt='procura programa' title='procura programa' onclick="ProcuraPrograma()">
      </tr>
<tr><td align='right' class="SubTituloDireita">T�tulo:</td>
    <td><?=campo_texto('prgdsc','N','N','',100,100,'','');?></td>
</tr>
<?
if ($_REQUEST['refcod']) $refcod=$_REQUEST['refcod'];
if ($nlinhasM >= 0 ){

   $sql = "select pfl.pfldsc as descricao,pfl.pflsncumulativo as mostra, usu.usunome || '('|| org.orgdsc ||')' as usuario from perfil pfl left join usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.prgid = ".$_SESSION['prgid']." and rpu.rpustatus='A' left join usuario usu on usu.usucpf=rpu.usucpf left join orgao org on org.orgcod = usu.orgcod where  pfl.pflstatus='A' and pfl.pflresponsabilidade in ('P','X','D') order by pfl.pfldsc";
    $RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em vari�veis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	        $linha = "<tr><td align='right' class='SubTituloDireita'>".$descricao.":</td><td>".$usuario."</td></tr>";
            print $linha;
        }
     }
?>

<tr><td colspan=2><hr></td></tr>
<tr><td align='right' class="SubTituloDireita">Per�odo de Refer�ncia:</td>
<td>
<?
$sql2 = "select refcod as CODIGO,refdsc as DESCRICAO from referencia where refdata_limite_parecer_aca is not null and refsnmonitoramento='t'   and refano_ref='".$_SESSION['exercicio']."'   order by refano_ref,refmes_ref desc";
$db->monta_combo("refcod",$sql2,'S',"Todos os per�odos de Refer�ncia",'','');

?>

</td></tr>
<tr bgcolor="#CCCCCC">
   <td></td><td align='center'><input type="button" name="btrelat" value="Gerar Relat�rio" onclick="validar_relatorio()" class="botao"></td>
</tr>
<?}?>
</table>
</center>
</div>
</form>
<?

if ($sqlrel){
//print $sqlrel;

    print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>'.$titul. '</b></td><td>';
    print '<input type="button" class="botao" name="btrelat" value ="Gerar PDF" align="left" onclick="imprime_rel()">';
    print '</td></tr></table>';
    // primeira parte do relat�rio : dados b�sicos
    print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>1. DADOS B�SICOS</b></td></tr></table>';
     $sql= 'select p.*,o.orgdsc,tp.tprnome from programa p left join orgao o on o.orgcod=p.orgcod left join tipoprograma tp on tp.tprcod=p.tprcod where p.prgid ='.$_SESSION['prgid'];
     $RSr = $db->record_set($sql);
     $res = $db->carrega_registro($RSr,0);
     if(is_array($res))
       foreach($res as $k=>$v) { ${$k}=$v; if (${$k}== '') ${$k} == 'Dado n�o informado pelo SIGPLAN';}

     ?>
     <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='right' class="subtitulodireita" width="20%">C�digo:</td>
        <td><?=campo_texto('prgcod','N','S','',8,100,'','');?></td>
      </tr>
</form>
      <tr>
        <td align='right' class="SubTituloDireita">Org�o respons�vel:</td>
        <td><?=campo_texto('orgdsc','N','N','',100,100,'','');?></td>
      </tr>
      <?if ($unidsc){?>
      <tr>
        <td align='right' class="SubTituloDireita">Unidade respons�vel:</td>
         <td><?=campo_texto('unidsc','N','N','',100,100,'','');?></td>
      </tr>
      <?}?>
      <tr>
        <td align='right' class="SubTituloDireita">Denomina��o:</td>
        <td><?=campo_texto('prgdsc','N','N','',100,100,'','');?></td>
      </tr>
      <?if ($prgdscproblema){?>
      <tr>
        <td align='right' class="SubTituloDireita">Problema:</td>
        <td><?=campo_textarea('prgdscproblema','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
      <?if ($prgdscobjetivo){?>
      <tr>
        <td align='right' class="SubTituloDireita">Objetivo do Programa:</td>
        <td><?=campo_textarea('prgdscobjetivo','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
      <tr>
        <td align='right' class="SubTituloDireita">P�blico-Alvo:</td>
        <td><?=campo_texto('prgdscpublicoalvo','N','N','',100,100,'','');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Justificativa:</td>
        <td><?=campo_textarea('prgdscjustificativa','N','N','',100,8,'');?></td>
      </tr>
      <?if ($prgdscobjetivoassoc){?>
      <tr>
        <td align='right' class="SubTituloDireita">Objetivo setorial associado:</td>
        <td><?=campo_textarea('prgdscobjetivoassoc','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
      <?if ($mobdsc){?>
      <tr>
        <td align='right' class="SubTituloDireita">Desafio associado:</td>
        <td><?=campo_textarea('mobdsc','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
      <tr>
        <td align='right' class="SubTituloDireita">Programa estruturante?</td>
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
        <td align='right' class="SubTituloDireita">Tipo:</td>
        <td><?=campo_texto('tprnome','N','N','',30,30,'','');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Horizonte temporal:</td>
        <td>
	<?
	   if ($prgsntemporario=='f')    print 'Cont�nuo </td>';
	   else    print 'Tempor�rio </td>';
	?>
      </tr>
	<?
	   if ($prgsntemporario=='t'){?>
      <tr>
        <td align='right' class="SubTituloDireita">Data In�cio/T�rmino:</td>
        <td><?=campo_texto('prgmesinicio','N','N','',6,2,'','');?> /
	        <?=campo_texto('prganoinicio','N','N','',6,4,'','');?>
	<? if ($prgsntemporario=='t') {
	?>
	  <br>(Se tempor�rio) <?=campo_texto('prgmestermino','N','N','',6,2,'','');?> /
      <?=campo_texto('prganotermino','N','N','',6,4,'','');?>
	  <?}?>
	  </td>
      </tr>

       <tr>
        <td align='right' class="SubTituloDireita">Valor Global Estimado <br>do Programa (R$) (se tempor�rio)</td>
        <td>
	<? if ($prgsntemporario=='t') {
	?>
            <?=campo_texto('prgvlrprograma','N','N','',10,10,'','');?>
	  <?}?>
	  </td>
      </tr>
      <?}?>
      <tr>
        <td align='right' class="SubTituloDireita">Estrat�gia de implementa��o:</td>
        <td><?=campo_textarea('prgdscestrategia','N','N','',100,5,'');?></td>
    </td>
      </tr>
      <?if ($prgobs){?>
      <tr>
        <td align='right' class="SubTituloDireita">Observa��o:</td>
        <td><?=campo_textarea('prgobs','N','N','',100,5,'');?></td>
      </tr>
	  <?}?>
<?
   $sql = "select pfl.pflcod as codigo, pfl.pfldsc as descricao,pfl.pflsncumulativo,rpu.usucpf, rpu.prgid, usu.usunome || '('|| org.orgdsc ||')' as usuario from perfil pfl left join usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.prgid = ".$_SESSION['prgid']." and rpu.rpustatus='A' left join usuario usu on usu.usucpf=rpu.usucpf left join orgao org on org.orgcod = usu.orgcod where pfl.pflstatus='A' and pfl.pflresponsabilidade in ('P','X','D') order by pfl.pfldsc";
    $RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em vari�veis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	        $linha = "<tr><td align='right' class='SubTituloDireita'><b>".$descricao.":</b></td><td>".$usuario."</td></tr>";
            print $linha;

        }
     }

?>
    </table>
<?
    // segunda parte do relat�rio : Financeiro Evolu��o
    print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>2. FINANCEIRO EVOLU��O</b></td></tr></table>';
$cabecalho = array('T�tulo da A��o','Dota��o Autorizada','Valor Empenhado','Valor Liquidado','Valor Pago');
$sql = "Select '<font color=#006600><b>'||co_acao||'</b></font> - '||no_projeto_atividade, sum(dotacao_disponivel) as disponivel, sum(empenhos_emitidos) as emitidos, sum(empenhos_liquidados) as liquidados, sum(valor_pago) as pago from  siof_simec where co_programa='".$prgcod."' group by co_acao, no_projeto_atividade order by 1";
$db->monta_lista($sql,$cabecalho,300,20,'S','','');


// restri��es e provid�ncias
 print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>3. RESTRI��ES E PROVID�NCIAS</b></td></tr></table>';
  $sql= "select r.rspdscrestricao as restricao, r.rspdscprovidencia as providencia, r.rspdatasuperacao, u.usunome, o.orgdsc from restricaoprograma r, usuario u, orgao o where r.prgid =".$_SESSION['prgid']." and r.rspsnliberado='t' and r.usucpf=u.usucpf and o.orgcod = u.orgcod";
  $RSr = $db->record_set($sql);
  $nlinhas = $db->conta_linhas($RSr);
  print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
  if ($nlinhas >=0)
  {
      print "<tr><td valign='top'> <font color='#FF5B3C'><b><h2>Restri��es </h2></font></td><td valign='top'><font color='#FF5B3C'><b><h2> Provid�ncias </h2></b></font></td></tr>";
      for ($i=0; $i<=$nlinhas;$i++)
      {
        $res = $db->carrega_registro($RSr,$i);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        print "<tr><td> ".$restricao." </td><td> ".$providencia." </td></tr>";
      }
  }
  else print "<tr><td><h3><font color='#FF5B3C'> N�o h� restri��es cadastradas</font></h3> </td><td></td></tr>";
  print '</table>';

// avalia��es e pareceres
    $RS = $db->record_set($sqlrel);
    $_SESSION['LINHA'] = $db->conta_linhas($RS);
    $_SESSION['title'] = $tit;
    $_SESSION['TIT'][0][0] = 'C�digo ';
    $_SESSION['TIT'][0][1] = 'T�tulo';
    $_SESSION['data']= date('(h:m) d/m/Y');
    $_SESSION['LIN'] = array();

 print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>4. AVALIA��ES E PARECERES</b></td></tr></table>';
  print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
    for ($i=0; $i<=$_SESSION['LINHA'];$i++)
    {
      $res = $db->carrega_registro($RS,$i);
      // a linha abaixo transforma em vari�veis todos os campos do array
      if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
      if ($refdsc <> $refer) {
           print "<tr><td colspan=2> <hr> </td></tr>";
           print "<tr><td colspan=2><font color='#FF5B3C'><b><h3> Per�odo de Refer�ncia ".$refdsc."</font></b></h3> </td></tr>";
           print "<tr><td >Avalia��o </td><td >Parecer </td></tr><tr>";
           $refer =$refdsc;
      }
      if ($avpliberada == 'f') $avaliacao='Avalia��o n�o liberada';
      print "<td>".$avaliacao." </td>"."<td>".$parecer." </td></tr>";
      /*if ($tpaid == 1)
      {
        // � uma avalia��o
        if ($avpliberada == 't'){
           if ($avaliacao) print "<tr><td> Avalia��o ".$avaliacao." </td></tr>";
           else print "<tr><td> Avalia��o n�o cadastrada </td></tr>";
      } else print "<tr><td><h2> Avalia��o n�o liberada</h2> </td></tr>";
      } else
      {
        // � um parecer
        if ($avpliberada == 't'){
          if ($parecer) print "<tr><td> Parecer ".$parecer." </td></tr>";
          else print "<tr><td> Parecer n�o cadastrado </td></tr>";
        } else print "<tr><td><h2> Parecer n�o liberado</h2> </td></tr>";
        
      }*/
      

    }
    
    //

    if ($_SESSION['LINHA'] >= 0) {
        for ($i=0; $i<=$_SESSION['LINHA'];$i++){
            $res = $db->carrega_registro($RS,$i);
            // a linha abaixo transforma em vari�veis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
 	       $_SESSION['LIN'][$i][0] = $codigo;
		   $_SESSION['LIN'][$i][1] = $descricao;
        }
     }
}
}

?>

<script>
function validar_relatorio()
{
    document.formulario.relat.value='r';
   	document.formulario.submit();
}
function alterar_cad(cod)
{
   	location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/programa/cadprograma&acao=C&prgid="+cod;

}

function imprime_rel()
{
    <?
       $browser = get_browser();
       $saiu = false;
       if (is_array($browser)){
       foreach ($browser as $name => $value) {
           if ($value == 'IE') {
           $saiu = true;
          }
       }}
       if ($saiu){?>
           location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_geral1&acao=R";
       <?} else { ?>
           e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_geral1&acao=R";
          window.open(e, "relatorio","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=400'");

      <? } ?>
}
function ProcuraPrograma() {
	document.formulario.procura.value = document.formulario.prgcod.value;
	document.formulario.submit();
    }
</script>

