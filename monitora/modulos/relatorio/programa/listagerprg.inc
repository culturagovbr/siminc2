<?
   /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   M�dulo:listagerprg.inc
   Finalidade: permitir a constru��o de lista gerencial de programas
    */
  $rest=1;
  $tpsav=1;
  $corav=1;
  $tpspar=1;
  $corpar=1;
  $periodo=1;
  $periodo2=1;

  $from = ' from monitora.programa p ';
  if ($_REQUEST['cbrp']) {
    // restri��o e providencia
    $rest=2;
    $from = ' from monitora.programa p, monitora.restricaoprograma r ';
  }
  if ($_REQUEST['tpscodav'] or $_REQUEST['cbpalt']) {
    $tpsav=2;
    if ($rest==1)   $from = ' from monitora.programa p, monitora.avaliacaoparecer ava ';
    else  $from = ' from monitora.programa p, monitora.restricaoprograma r, monitora.avaliacaoparecer ava  ';
  }
  if ($_REQUEST['corcodav'] or $_REQUEST['cbpalt']) {
    $corav=2;
    $tpsav=2;
    if ($rest==1)
       $from = ' from monitora.programa p, monitora.avaliacaoparecer ava ';
    else
      $from = ' from monitora.programa p, monitora.restricaoprograma r,monitora.avaliacaoparecer ava  ';
  }

  if ($_REQUEST['tpscodpar'] or $_REQUEST['cbpalt']) {
    $tpspar=2;
    if ($rest==1)
    {
      if ($tpsav==1) $from = ' from monitora.programa p, monitora.avaliacaoparecer par ';
      else $from = ' from monitora.programa p, monitora.avaliacaoparecer ava, monitora.avaliacaoparecer par ';
    }
    else  {
      if ($tpsav==1) $from = ' from monitora.programa p, monitora.restricaoprograma r,monitora.avaliacaoparecer par ';
      else $from = ' from monitora.programa p, monitora.restricaoprograma r, monitora.avaliacaoparecer ava, monitora.avaliacaoparecer par ';
    }
  }
  if ($_REQUEST['corcodpar'] or $_REQUEST['cbpalt']) {
    $corpar=2;
    $tpspar=2;
    if ($rest==1)
    {
      if ($tpsav==1) $from = ' from monitora.programa p, monitora.avaliacaoparecer par ';
      else $from = ' from monitora.programa p, monitora.avaliacaoparecer ava, monitora.avaliacaoparecer par ';
    }
    else  {
      if ($tpsav==1) $from = ' from monitora.programa p, monitora.restricaoprograma r,monitora.avaliacaoparecer par ';
      else $from = ' from monitora.programa p, monitora.restricaoprograma r, monitora.avaliacaoparecer ava, monitora.avaliacaoparecer par ';
    }
  }
  // monta relatorio
  if ($_REQUEST['lista']=='L'){
  $i=0;
  $sql = "select distinct '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Ver Cadastro \" onclick=\"alterar_cad(' || ' ''' || p.prgid || ''' ' || ')\">' as acao, p.prgcod as codigo,p.prgdsc as descricao $from where  p.prgstatus='A' and p.prgano='".$_SESSION['exercicio']."'";
} else
{
    $sql = "select distinct p.prgid, p.prgcod as codigo,p.prgdsc as descricao $from where  p.prgstatus='A' and p.prgano='".$_SESSION['exercicio']."'";

}
  // aten��o !!!! na sql acima, o espa�o � importante para n�o causar erro!!!!
  $titul = 'Filtro -';
  $tit = 'Filtro -';
  if ($_REQUEST['orgcod'])
  {
    $sql = $sql." and substr(p.orgcod,0, 3)='".substr($_REQUEST['orgcod'],0,2)."'";
    $sqldsc = "select orgdsc from orgao where orgcod='".$_REQUEST['orgcod']."'";
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Do (a) '.$titulo['orgdsc'];
    $tit.= '| Do (a) '.$titulo['orgdsc'];
  }
  
    if ($_REQUEST['exceto'])
  {
    $org = substr($_SESSION['ittorgao'],0,2);
    $sql = $sql." and substr(p.orgcod,0, 3) <> '$org' ";
    $titul.= '| Exceto o '.$_SESSION['ittabrev'] ;
    $tit.= '| Exceto o '.$_SESSION['ittabrev'];
  }


    if ($_REQUEST['prgid'][0])
  {
    $sql = $sql." and p.prgid in (".implode(",",$_REQUEST['prgid']).")";
    $sqldsc = "select prgdsc from monitora.programa where prgid in (".implode(",",$_REQUEST['prgid']).")";
    $titulo=$db->carregar($sqldsc);
    foreach($titulo as $titulo2){
    	$titul.= '| Programa:'.$titulo2['prgdsc'];
    	$tit.= '| Programa:'.$titulo2['prgdsc'];
    }
  }
  
    if ($_REQUEST['prgdsc'])
  {
    $sql = $sql." and p.prgdsc ilike '%".$_REQUEST['prgdsc']."%'";
    $titul.= '| Programa que contenha:'.$_REQUEST['prgdsc'].' no T�tulo';
    $tit.= '| programa que contenha: '.$_REQUEST['prgdsc'].' no T�tulo';
  }
    if ($_REQUEST['tprcod'])
  {
    $sql = $sql." and p.tprcod ='".$_REQUEST['tprcod']."'";
    $sqldsc = "select tprnome from tipoprograma where tprcod='".$_REQUEST['tprcod']."'";
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Tipo:'.$titulo['tprnome'];
    $tit.= '| Tipo: '.$titulo['tprnome'];
  }
   if ($_REQUEST['prgsntemporario'])
  {
    $sql = $sql." and p.prgsntemporario ='".$_REQUEST['prgsntemporario']."'";
    if ($_REQUEST['prgsntemporario']== 't') {
       $titul.= '| Tempor�rios ';
       $tit.= '| Tempor�rios ';
    } else
    {
       $titul.= '| Cont�nuos';
       $tit.= '| Cont�nuos';
    }
  }

  if ($_REQUEST['cbrp'])
  {
     if ($_REQUEST['cbrp'] == 'N') {
       $sql = $sql." and p.prgid=r.prgid and r.rspsnliberado='t' and r.rspsntempohabil='f' ";
           $titul.= '| Com restri��es ainda sem solu��o';
           $tit.= '| Com restri��es ainda sem solu��o ';
     }
     if ($_REQUEST['cbrp'] == 'S') {
       $sql = $sql." and p.prgid=r.prgid and r.rspsnliberado='t' and r.rspsntempohabil='t' ";
           $titul.= '| Com restri��es solucionadas';
           $tit.= '| Com restri��es solucionadas ';
     }
     if ($_REQUEST['cbrp'] == 'A') {
       $sql = $sql." and p.prgid=r.prgid and r.rspsnliberado='t' ";
           $titul.= '| Com restri��es ';
           $tit.= '| Com restri��es ';
     }

  }
  if ($_REQUEST['cbpalt'])
  {
    $sql = $sql." and ava.avporigem=9 and ava.prgid=p.prgid ";
    $titul.= '| Parecer da Alta Gest�o ';
    $tit.= '| Parecer da Alta Gest�o ';
  }

   if ($_REQUEST['tpscodav'])
  {
    $sql = $sql." and p.prgid=ava.prgid and ava.tpaid=1 and ava.tpscod=".$_REQUEST['tpscodav'];
    $sqldsc = "select tpsdsc from tiposituacao where tpscod=".$_REQUEST['tpscodav'];
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Avalia��o: '.$titulo['tpsdsc'];
    $tit.= '| Avalia��o '.$titulo['tpsdsc'];
    if ($_REQUEST['dataini'] and $_REQUEST['datafim'])
    {
      $periodo=2;
        $sql = $sql." and ava.avpdata >= '".$_REQUEST['dataini']."' and ava.avpdata <= '".$_REQUEST['datafim']."' ";
        $titul.= '| Per�odo: '.$_REQUEST['dataini'].' a '.$_REQUEST['datafim'];
        $tit.= '| Per�odo: '.$_REQUEST['dataini'].' a '.$_REQUEST['datafim'];
    }

  }
   if ($_REQUEST['corcodav'])
  {
    $sql = $sql." and p.prgid=ava.prgid and ava.tpaid=1 and ava.corcod=cor.corcod and cor.corcod=".$_REQUEST['corcodav'];
    $sqldsc = "select corsignificado from cor where corcod=".$_REQUEST['corcodav'];
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Avaliado como : '.$titulo['corsignificado'];
    $tit.= '| Avaliado como : '.$titulo['corsignificado'];
    if ($_REQUEST['dataini'] and $_REQUEST['datafim'] and $periodo==1)
    {
        $sql = $sql." and ava.avpdata >= '".$_REQUEST['dataini']."' and ava.avpdata <= '".$_REQUEST['datafim']."' ";
        $titul.= '| Per�odo: '.$_REQUEST['dataini'].' a '.$_REQUEST['datafim'];
        $tit.= '| Per�odo: '.$_REQUEST['dataini'].' a '.$_REQUEST['datafim'];
    }
  }

   if ($_REQUEST['tpscodpar'])
  {
    $sql = $sql." and p.prgid=par.prgid and par.tpaid=2 and par.tpscod=".$_REQUEST['tpscodpar'];
    $sqldsc = "select tpsdsc from tiposituacao where tpscod=".$_REQUEST['tpscodpar'];
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Parecer : '.$titulo['tpsdsc'];
    $tit.= '| Parecer '.$titulo['tpsdsc'];
    if ($_REQUEST['dataini'] and $_REQUEST['datafim'] )
    {
      $periodo2=2;
        $sql = $sql." and par.avpdata >= '".$_REQUEST['dataini']."' and par.avpdata <= '".$_REQUEST['datafim']."' ";
        $titul.= '| Per�odo: '.$_REQUEST['dataini'].' a '.$_REQUEST['datafim'];
        $tit.= '| Per�odo: '.$_REQUEST['dataini'].' a '.$_REQUEST['datafim'];
    }
  }
   if ($_REQUEST['corcodpar'])
  {
    $sql = $sql." and p.prgid=par.prgid and par.tpaid=2 and par.corcod=cor.corcod and cor.corcod=".$_REQUEST['corcodpar'];
    $sqldsc = "select corsignificado from cor where corcod=".$_REQUEST['corcodpar'];
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Parecer de: '.$titulo['corsignificado'];
    $tit.= '| Parecer de: '.$titulo['corsignificado'];
    if ($_REQUEST['dataini'] and $_REQUEST['datafim'] and $periodo2==1)
    {
        $sql = $sql." and par.avpdata >= '".$_REQUEST['dataini']."' and par.avpdata <= '".$_REQUEST['datafim']."' ";
        $titul.= '| Per�odo: '.$_REQUEST['dataini'].' a '.$_REQUEST['datafim'];
        $tit.= '| Per�odo: '.$_REQUEST['dataini'].' a '.$_REQUEST['datafim'];
    }
  }
  if ($_REQUEST['dadobas']){
    $titul.= '| Dados B�sicos';
    $tit.= '| Dados B�sicos';
  }
  if ($_REQUEST['finevo']){
    $titul.= '| Financeiro Evolu��o';
    $tit.= '| Financeiro Evolu��o';
  }
  if ($_REQUEST['respro']){
    $titul.= '| Restri��es e Provid�ncias';
    $tit.= '| Restri��es e Provid�ncias';
  }
  if ($_REQUEST['avapar']){
    $titul.= '| Avalia��o e Parecer';
    $tit.= '| Avalia��o e Parecer';
  }
    if ($_REQUEST['paralt']){
    $titul.= '| Parecer da Alta Gest�o';
    $tit.= '| Parecer da Alta Gest�o';
  }
    if ($_REQUEST['dataini'] and $_REQUEST['datafim']){
    $titul.= '| Per�odo:'.$_REQUEST['dataini'].' a '.$_REQUEST['datafim'];
    $tit.= '| Per�odo:'.$_REQUEST['dataini'].' a '.$_REQUEST['datafim'];
  }
  $sqlrel = $sql." ))";
  if ($i == 0) unset($sqlrel);
  $sql = $sql."  and p.prgano='".$_SESSION['exercicio']."' order by p.prgdsc";
  $sqlrel=$sql;





