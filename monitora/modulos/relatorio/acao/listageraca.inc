<?
   /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:listageraca.inc
   Finalidade: permitir a construção de lista gerencial de acao
    */
// monta relatorio
     unset($_SESSION['conteudo']);
     unset($_SESSION['cab']); 
  $sql='';
  $sql1='';
  $ok=0;
  $titul = 'Filtro -';
  $tit = 'Filtro -';
  $from = ' from monitora.acao a ';

  $where = "  where  a.acasnrap = '$rap' and a.acastatus='A' and a.prgano='".$_SESSION['exercicio']."'";
  
  $sql .= " inner join planejamento.programa p on a.prgid = p.prgid inner join unidade un on un.unicod = a.unicod inner join orgao o on  o.orgcod=un.orgcod ";
 if ($_REQUEST['lista']=='L'){
    $sql1 = "select distinct '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Ver Cadastro \" onclick=\"alterar_cad(' || '\'' || a.acaid || '\',\'' || a.prgid || '\'' || ')\">' as acao, a.prgcod || '.' || a.acacod ||'.'||a.unicod ||'.' ||a.loccod as codigo,a.acadsc as descricao ";
} else
{
     $sql1 = "select distinct a.acaid, a.acacod as codigo,a.acadsc as descricao, a.unicod, a.loccod  ";
}

  if ($_REQUEST['orgcod'])
  {// escolheu o órgão
    
    $sql .= " inner join unidade u on u.unicod=a.unicod inner join orgao o2 on u.orgcod=o2.orgcod and u.orgcod='".$_REQUEST['orgcod']."'";
    $sqldsc = "select orgdsc from orgao where orgcod='".$_REQUEST['orgcod']."'";
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Do (a) '.$titulo['orgdsc'];
    $tit.= '| Do (a) '.$titulo['orgdsc'];
  }

  if ($_REQUEST['exceto'])
  {
    $org = substr($_SESSION['ittorgao'],0,2);
    $ok=1;
    //$org = '26';
    $sql.= " inner join unidade unn2 on unn2.unicod=a.unicod and substr(unn2.orgcod,0, 3) <> '$org' ";
    $where.= " and cast(substr(a.unicod,0, 3) as integer) not in ($org) ";
    $titul.= '| Exceto o '.$_SESSION['ittabrev'];
    $tit.= '| Exceto o '.$_SESSION['ittabrev'];
  }

  if ($_REQUEST['unicod'])
  {
    $ok=1;
    $where.= " and  a.unicod='".$_REQUEST['unicod']."'";
    $sqldsc = "select unidsc from unidade where unicod='".$_REQUEST['unicod']."'";
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Unidade Orçamentária: '.$titulo['unidsc'];
    $tit.= '| Unidade Orçamentária: '.$titulo['unidsc'];
  }

  if ($_REQUEST['ungcod'])
  {
        $ok=1;
    $sql .= " inner join unidade unn on unn.unicod = a.unicod inner join unidadegestora ug on ug.unicod=unn.unicod and ug.ungcod='".$_REQUEST['ungcod']."'";
    $sqldsc = "select ungdsc from unidadegestora where ungcod='".$_REQUEST['ungcod']."'";
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Unidade Gestora: '.$titulo['ungdsc'];
    $tit.= '| Unidade Gestora: '.$titulo['ungdsc'];
  }
  
  
  if ($_REQUEST['gunid'])
  {
        $ok=1;
    $sql .= " inner join unidade un2 on un2.unicod = a.unicod inner join grupounidade gu on gu.gunid=un2.gunid and gu.gunid=".$_REQUEST['gunid'];
    $sqldsc = "select gundsc from grupounidade where gunid=".$_REQUEST['gunid'];
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Unidade tipo: '.$titulo['gundsc'];
    $tit.= '| Unidade tipo: '.$titulo['gundsc'];
  }

   
  if ($_REQUEST['cbrp']) {
    // restrição e providencia
    $ok=1;
    $sql1 .= ', rp.rsasntempohabil ';
    $sql .= " inner join restricaoacao rp on rp.acaid=a.acaid and rp.rsasnliberado='t' ";
    if ($_REQUEST['cbrp'] == 'N') {
       $sql .= "  and rp.rsasntempohabil='f' ";
       $titul.= '| Com restrições ainda sem solução';
       $tit.= '| Com restrições ainda sem solução ';
    }
    if ($_REQUEST['cbrp'] == 'S') {
       $sql = $sql."  and rp.rsasntempohabil='t' ";
           $titul.= '| Com restrições solucionadas';
           $tit.= '| Com restrições solucionadas ';
     }
     if ($_REQUEST['cbrp'] == 'A') {
       //$sql = $sql." and rp.rsasnliberado='t' ";
           $titul.= '| Com restrições ';
           $tit.= '| Com restrições ';
     }
  }

  if ($_REQUEST['prgid'][0])
  {
    $ok=1;
    $where .= " and a.prgid in (".implode(",",$_REQUEST['prgid']).")";
    $sqldsc = "select prgdsc from programa where prgid in (".implode(",",$_REQUEST['prgid']).")";
    $titulo=$db->carregar($sqldsc);
    foreach($titulo as $titulo2){
	    $titul.= '| Programa:'.$titulo2['prgdsc'];
	    $tit.= '| Programa:'.$titulo2['prgdsc'];
    }
  }

  if ($_REQUEST['acacod'][0])
  {
    $ok=1;
    $where .= " and a.acacod in ('".implode("','",$_REQUEST['acacod'])."')";
    $sqldsc = "select acadsc from acao where acacod in ('".implode("','",$_REQUEST['acacod'])."') limit 1";
    $titulo=$db->recuperar($sqldsc);
   	$titul.= '| Ação:'.$titulo['acadsc'];
   	$tit.= '| Ação:'.$titulo['acadsc'];
  }
  

   if ($_REQUEST['acadsc'])
  {
    $ok=1;
    $where .= " and a.acadsc ilike '%".$_REQUEST['acadsc']."%'";
    $titul.= '| Contenha: '.$_REQUEST['acadsc'].' no Título';
    $tit.= '| Contenha: '.$_REQUEST['acadsc'].' no Título';
  }

   if ($_REQUEST['acacodbusca'])
  {    $ok=1;
    unset ($listaacao);
    $codigos = explode(";", $_REQUEST['acacodbusca']);
    for ($c=0;$c<=count($codigos)-1;$c++)
    { if ($c==count($codigos)-1) $listaacao.= "'".strtoupper($codigos[$c])."'";
      else
      $listaacao.= "'".trim(strtoupper($codigos[$c]))."',";}

      $where .= " and a.acacod in ($listaacao) ";
      $titul.= '| Códigos: '.$_REQUEST['acacodbusca'];
      $tit.= '| Códigos: '.$_REQUEST['acacodbusca'];
  }

  if ($_REQUEST['cbrap'])
  {
    $ok=1;
    $where .=" and a.acasnrap='".$_REQUEST['cbrap']."'";
    if ($_REQUEST['cbrap']=='f'){
    $titul.= '| Não é RAP ';
    $tit.= '| Não é RAP ';
    } else
    if ($_REQUEST['cbrap']=='t'){
        $titul.= '| RAP ';
        $tit.= '| RAP ';
    }
  }
  
  if ($_REQUEST['cbemenda'])
  {
    $ok=1;
    $where .=" and a.acasnemenda='".$_REQUEST['cbemenda']."'";
    if ($_REQUEST['cbemenda']=='f'){
    $titul.= '| Não é Emenda ';
    $tit.= '| Não é Emenda ';
    } else
    if ($_REQUEST['cbemenda']=='t'){
        $titul.= '| Emenda ';
        $tit.= '| Emenda ';
    }
  }
  
  if ($_REQUEST['procod'])
  {
    $ok=1;
    $where .=" and a.procod='".$_REQUEST['procod']."'";
    $sqldsc = "select prodsc from produto where procod='".$_REQUEST['procod']."'";
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Produto: '.$titulo['prodsc'];
    $tit.= '| Produto: '.$titulo['prodsc'];
  }
  
  if ($_REQUEST['dataproj'])
  {
    $ok=1;
    $where .=" and a.acamesprevisaoconclusao <='".substr($_REQUEST['dataproj'],3,2)."' and a.acaanoprevisaoconclusao <='".substr($_REQUEST['dataproj'],6,4)."'";

    $titul.= '| Término previsto para até: '.substr($_REQUEST['dataproj'],3,2).'/'.substr($_REQUEST['dataproj'],6,4);
    $tit.= '| Término previsto para até '.substr($_REQUEST['dataproj'],3,2).'/'.substr($_REQUEST['dataproj'],6,4);
  }
  
  
  if ($_REQUEST['unmcod'])
  {
    $ok=1;
    $where .=" and a.unmcod='".$_REQUEST['unmcod']."'";
    $sqldsc = "select unmdsc from unidademedida where unmcod='".$_REQUEST['unmcod']."'";
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Unidade de medida: '.$titulo['unmdsc'];
    $tit.= '| Unidade de medida: '.$titulo['unmdsc'];
  }
    if ($_REQUEST['taccod'])
  {
    $ok=1;
    $where .=" and a.taccod ='".$_REQUEST['taccod']."'";
    $sqldsc = "select tacdsc from tipoacao where taccod='".$_REQUEST['taccod']."'";
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Tipo '.$titulo['tacdsc'];
    $tit.= '| Tipo: '.$titulo['tacdsc'];
  }
    if ($_REQUEST['regcod'])
  {
    $ok=1;
    $where .=" and a.regcod ='".$_REQUEST['regcod']."'";
    $sqldsc = "select regdsc from regiao where regcod='".$_REQUEST['regcod']."'";
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Região '.$titulo['regdsc'];
    $tit.= '| Região: '.$titulo['regdsc'];
  }


  if ($_REQUEST['cbpalt'])
  {    $ok=1;
    $sql .= " inner join avaliacaoparecer ava2 on ava2.avporigem=9 and ava2.acaid=a.acaid ";
    $titul.= '| Parecer da Alta Gestão ';
    $tit.= '| Parecer da Alta Gestão ';
  }
  
if (! $_REQUEST['cbsaval'])
{
    if ($_REQUEST['tpscodav'])
  {
    $ok=1;
    $sql .= " inner join avaliacaoparecer ava on a.acaid=ava.acaid and ava.tpaid=1 and ava.tpscod=".$_REQUEST['tpscodav'];
    $sqldsc = "select tpsdsc from tiposituacao where tpscod=".$_REQUEST['tpscodav'];
    $titulo= $db->recuperar($sqldsc);
    $titul.= '| Avaliação: '.$titulo['tpsdsc'];
    $tit.= '| Avaliação '.$titulo['tpsdsc'];
  }
   if ($_REQUEST['corcodav'])
  {
    $ok=1;
    $sql .= " inner join avaliacaoparecer ava4 on ava4.corcod='".$_REQUEST['corcodav']."' and ava4.acaid =a.acaid ";
   $sqldsc = "select corsignificado from cor where corcod=".$_REQUEST['corcodav'];
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Avaliado como : '.$titulo['corsignificado'];
    $tit.= '| Avaliado como : '.$titulo['corsignificado'];
  }
  if ($_REQUEST['dataini'] and $_REQUEST['datafim'])
  {
    $ok=1;
    $mesini= substr($_REQUEST['dataini'],3,2);
    $mesfim= substr($_REQUEST['datafim'],3,2);
    $anoini= substr($_REQUEST['dataini'],6,4);
    $anofim= substr($_REQUEST['datafim'],6,4);

    $sqlref = " select refcod from referencia where refsngrupo='f' and refmes_ref >= '$mesini' and refmes_ref <= '$mesfim' and refano_ref >= '$anoini' and refano_ref <= '$anofim'";
    $RSref = $db->record_set($sqlref);
    $nlin = $db->conta_linhas($RSref);
    if ($nlin>=0){
        $sql .= " inner join avaliacaoparecer ava3 on ava3.acaid=a.acaid inner join referencia ref1 on ref1.refcod=ava3.refcod and ref1.refcod in (";
        $titul.= '| Avaliação no período: '.$_REQUEST['dataini'].' a '.$_REQUEST['datafim'];
        $tit.= '| Avaliação no período: '.$_REQUEST['dataini'].' a '.$_REQUEST['datafim'];
        for ($i=0;$i<=$nlin;$i++)
        {
           $resref = $db->carrega_registro($RSref,$i);
           if ($i <> $nlin)
           $sql .= $resref['refcod'].",";
           else $sql .= $resref['refcod'];
        }
        $sql .= ') ';
     }
       
  }
}
else
{
    $ok=1;
    //$where .= " and a.acaid not in (select avap.acaid from avaliacaoparecer avap where a.acaid=avap.acaid and avap.avpliberada='t' and avap.tpaid=1) and a.acaid in (select usur.acaid from usuarioresponsabilidade usur where a.acaid=usur.acaid and usur.pflcod=1)";
    //$titul.= '| Sem avaliação: '.$titulo['tpsdsc'];
    //$tit.= '| Sem avaliação '.$titulo['tpsdsc'];
}


if (! $_REQUEST['cbsvalid'])
{
    if ($_REQUEST['cbval'])
  {
    $ok=1;
    $sql .= " inner join avaliacaoparecer avav on a.acaid=avav.acaid and avav.tpaid=2 and avav.corcod=".$_REQUEST['cbval'];
    $sqldsc = "select corsignificadopar from cor where corcod=".$_REQUEST['cbval'];
    $titulo= $db->recuperar($sqldsc);
    $titul.= '| Validado como: '.$titulo['corsignificadopar'];
    $tit.= '| validado como '.$titulo['corsignificadopar'];
  }   
}
else
{
    $ok=1;
    //$where .= " and a.acaid not in (select avap2.acaid from avaliacaoparecer avap2 where a.acaid=avap2.acaid and avap2.avpliberada='t' and avap2.tpaid=2) and a.acaid in (select usur2.acaid from usuarioresponsabilidade usur2 where a.acaid=usur2.acaid and usur2.pflcod=2)";
	$where .= " and a.acaid not in (select avap2.acaid from avaliacaoparecer avap2 where a.acaid=avap2.acaid and avap2.avpliberada='t' and avap2.tpaid=2) ";    
    $titul.= '| Sem validação: ';//.$titulo['tpsdsc'];
    $tit.= '| Sem validação ';//.$titulo['tpsdsc'];
}

  if ($_REQUEST['dadobas']){
    $titul.= '| Dados Básicos';
    $tit.= '| Dados Básicos';
  }
  if ($_REQUEST['finevo']){  
    $titul.= '| Financeiro Evolução';
    $tit.= '| Financeiro Evolução';
  }
  if ($_REQUEST['respro']){
    $titul.= '| Restrições e Providências';
    $tit.= '| Restrições e Providências';
  }
  if ($_REQUEST['avapar']){
    $titul.= '| Avaliação';
    $tit.= '| Avaliação';
  }
    if ($_REQUEST['paralt']){
    $titul.= '| Parecer da Alta Gestão';
    $tit.= '| Parecer da Alta Gestão';
  }
  if ($_REQUEST['fisevotot']){
    $titul.= '| Evolução Física (Totais)';
    $tit.= '| Evolução Física (Totais)';
  }  
  if ($_REQUEST['fisevo']){
    $titul.= '| Evolução Física';
    $tit.= '| Evolução Física';
  }   
 
 unset($sqlrel);
 
 if($ok) {
 	$sql1 .= '' .$from. '  ';
   	$sqlrel=$sql1.$sql.$where.'  order by codigo,a.acadsc';
 }

?>