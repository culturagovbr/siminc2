<?

 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:montadadosaca.inc
   Finalidade: permitir a construção de relatorio gerencial de acao em dados gerais
    */
//print $sqlrel;
    $temresposta=1;
//  dbg( $sqlrel , 1 );
    $RS = $db->record_set($sqlrel);    
    $_SESSION['LINHA'] = $db->conta_linhas($RS);
    $_SESSION['title'] = $tit;
    $_SESSION['TITULO']= 'Relatório Gerencial de Ação';
    $_SESSION['TIT'][0][0] = 'Código ';
    $_SESSION['TIT'][0][1] = 'Título';
    $_SESSION['data']= date('(h:m) d/m/Y');
    $_SESSION['LIN'] = array();
    
    print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>'.$titul. '</b></td><td>';
    if ($_SESSION['LINHA'] >=0) print '<input type="button" class="botao" name="btrelat" value ="Gerar PDF" align="left" onclick="imprime_rel(2)">&nbsp;&nbsp;<a href="ar505ptb.exe"><img border="0" width="30" heigth="30" src="../imagens/pdf_adobe.jpg" title=" Você não tem o Adobe Reader? Clique aqui!"></a>';
    if (($_REQUEST['finevo']) and (! $_REQUEST['fisevotot']) and (! $_REQUEST['dadobas'] ) and (! $_REQUEST['avapar'] ) and (! $_REQUEST['fisevo'])and (! $_REQUEST['plantrab'] ) and (! $_REQUEST['respro'] )and (! $_REQUEST['paralt'] )) print '&nbsp;&nbsp;<input type="button" class="botao" name="btcsv" value ="Gerar XLS" align="left" onclick="imprime_rel(3)">&nbsp;&nbsp;<input type="button" class="botao" name="btcsv" value ="Gerar CSV" align="left" onclick="imprime_rel(5)">';
    else 
if (($_REQUEST['fisevotot']) and (! $_REQUEST['finevo']) and (! $_REQUEST['dadobas'] ) and (! $_REQUEST['avapar'] ) and (! $_REQUEST['fisevo'])and (! $_REQUEST['plantrab'] ) and (! $_REQUEST['respro'] )and (! $_REQUEST['paralt'] )) print '&nbsp;&nbsp;<input type="button" class="botao" name="btcsv" value ="Gerar XLS" align="left" onclick="imprime_rel(3)">&nbsp;&nbsp;<input type="button" class="botao" name="btcsv" value ="Gerar CSV" align="left" onclick="imprime_rel(5)">';   
  
    print '</td></tr></table>';
    
if ($_SESSION['LINHA'] >= 0)
{
     unset($_SESSION['rsdados']);
     unset($_SESSION['rsdadosresp']);
     unset($_SESSION['rsfinevo']);
     unset($_SESSION['rsavapar']);
     unset($_SESSION['rsrespro']);
     unset($_SESSION['rsparalt']);
     unset($_SESSION['rsfisevo']);
     unset($_SESSION['rsplantrab']);
     unset($_SESSION['rspta']);
     unset($_SESSION['rspto']);
     unset($_SESSION['rspto2']);
     unset($_SESSION['soma']);
     unset($_SESSION['soma2']);
     unset($_SESSION['soma2t']);
     unset($_SESSION['rsfisevotot']);
     

   for ($i=0; $i<=$_SESSION['LINHA'];$i++)
   {
     $res = $db->carrega_registro($RS,$i);     
     // a linha abaixo transforma em variáveis todos os campos do array
     if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     $programa=$prgid;
     $acaoid=$acaid;
    // print 'acao===='.$acaoid.'<br>';
     $acacod = $codigo;
     $acadsc = $descricao;
     $sqlp = 'select p.prgcod as coprg, p.prgdsc from planejamento.programa p left join monitora.acao a on a.prgid=p.prgid where a.acaid='.$acaid;
     $RSp = $db->record_set($sqlp);     
     $resp = $db->carrega_registro($RSp,0);     
     if(is_array($resp)) foreach($resp as $k=>$v) ${$k}=$v;
	 // primeira parte do relatório, dados básicos
     if ($_REQUEST['dadobas'])
     {          $temresposta=1;     
       include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/relatorio/acao/dadosbasaca.inc";
     }
     if ($_REQUEST['finevo']){
          // Financeiro Evolução
          $temresposta=1;
	//$cabecalho = array('Unidade','Dotação Autorizada','Provisão Concedida','Provisão Recebida','Destaque Concedido','Destaque Recebido','Valor Empenhado','Valor Liquidado','Valor Pago');
	$cabecalho = array('Unidade','Dotação Inicial','Dotação ( Lei + Créditos )','Empenhado','Liquidado','Pago');
    $sql = "select distinct 
    						a.acacod || '.'||a.unicod ||'.'||a.acadsc||'<br>'||u.unidsc, 
    						sum( coalesce( r.rofdot_ini, 0 ) ) as dotacaoinicial, 
    						sum( coalesce( r.rofautorizado, 0 ) ) as dotacao, 
    						sum( coalesce( r.rofempenhado, 0 ) ) as empenhado, 
    						sum( coalesce( r.rofliquidado_favorecido, 0 ) ) as liquidado, 
    						sum( coalesce( r.rofpago, 0 ) ) as pago 
    		from financeiro.execucao r
				inner join monitora.acao a ON a.prgcod = r.prgcod and a.acacod = r.acacod and a.unicod = r.unicod and a.loccod = r.loccod and a.prgano = r.rofano
				inner join public.unidade u ON u.unicod = r.unicod 
    		where 
    			 r.prgcod = '".$coprg."' 
    			 and r.acacod = '".$acacod."' 
    			 and r.unicod = '".$unicod."' 
    			 and r.rofano ='".$_SESSION['exercicio']."'
    			 and a.prgano = '".$_SESSION['exercicio']."' 
    		group by 1";
    
    $sqlfinevo = "select distinct 
    						a.acacod || '.'||a.unicod as codigo,
    						a.acadsc,u.unidsc as objeto, 
    						sum( coalesce( r.rofdot_ini, 0 ) ) as dotacaoinicial, 
    						sum( coalesce( r.rofautorizado, 0 ) ) as dotacao, 
    						sum( coalesce( r.rofempenhado, 0 ) ) as empenhado, 
    						sum( coalesce( r.rofliquidado_favorecido, 0 ) ) as liquidado, 
    						sum( coalesce( r.rofpago, 0 ) ) as pago 
    				from financeiro.execucao r
						inner join monitora.acao a ON a.prgcod = r.prgcod and a.acacod = r.acacod and a.unicod = r.unicod and a.loccod = r.loccod and a.prgano = r.rofano
						inner join public.unidade u ON u.unicod = r.unicod 
    				where r.prgcod = '".$coprg."' 
    					  and r.acacod = '".$acacod."' 
    					  and r.unicod = '".$unicod."' 
    					  and r.rofano ='".$_SESSION['exercicio']."' 
    					  and a.prgano = '".$_SESSION['exercicio']."' 
    				group by 1,2,3";
    //dbg($sql);
    //SQL`s usados anteriormente:
    //$sql = "Select a.acacod || '.'||a.unicod ||'.'||a.acadsc||'<br>'||s.no_unidade_orcamentaria, s.dotacao_disponivel, s.provisao_concedida, s.provisao_recebida, s.destaque_concedido, s.destaque_recebido, s.empenhos_emitidos, s.empenhos_liquidados, s.valor_pago from  siof_simec s, acao a where s.co_programa='".$coprg."' and s.co_acao='".$acacod."'  and s.sfsstatus='A' and s.co_unidade_orcamentaria='".$unicod."' and a.acacod=s.co_acao and to_char(s.dt_referencia,'YYYY') ='".$_SESSION['exercicio']."' and a.acaid=".$acaoid." order by 1";
    //$sqlfinevo = "Select a.acacod || '.'||a.unicod as codigo ,a.acadsc,s.no_unidade_orcamentaria as objeto, s.dotacao_disponivel, s.provisao_concedida, s.provisao_recebida, s.destaque_concedido, s.destaque_recebido, s.empenhos_emitidos, s.empenhos_liquidados, s.valor_pago from  siof_simec s, acao a where s.co_programa='".$coprg."' and s.co_acao='".$acacod."'  and s.co_unidade_orcamentaria='".$unicod."' and s.sfsstatus='A' and a.acacod=s.co_acao and to_char(s.dt_referencia,'YYYY') ='".$_SESSION['exercicio']."' and a.acaid=".$acaoid." order by 1";
    
    $RSfin = $db->record_set($sqlfinevo);    
    $nlinhas = $db->conta_linhas($RSfin);
            
    if ($nlinhas >=0)
    {        
    $_SESSION['rsfinevo'][] = $db->carrega_tudo($RSfin);
    $_SESSION['cab']= $cabecalho;     
    $RSfin2a = $db->record_set($sql);
    $_SESSION['conteudo'][] =$db->carrega_tudo($RSfin2a);         
    $db->monta_lista($sql,$cabecalho,300,20,'S','','');
    }
  }
     if ($_REQUEST['respro'])
     {
       // restrições e providências
       $sql= "select a.acacod ||'.'|| a.unicod as codigo,a.acadsc, r.rsadscrestricao as restricao, r.rsadscprovidencia as providencia,  to_char(r.rsadatainclusao,'DD/MM/YYYY') as datainc, u.usunome, o.orgdsc as orgao, t.trsdsc from restricaoacao r, acao a, usuario u, orgao o , tiporestricao t where r.acaid =".$acaoid." and r.acaid=a.acaid  and r.rsasnliberado='t' and r.usucpf=u.usucpf and o.orgcod = u.orgcod and t.trscod=r.trscod";
       if ($_REQUEST['cbrp'] == 'N') $sql = $sql." and r.rsasntempohabil='f' ";
       if ($_REQUEST['cbrp'] == 'S') $sql = $sql." and r.rsasntempohabil='t' ";
       $RSr = $db->record_set($sql);
       $_SESSION['rsrespro'][] = $db->carrega_tudo($RSr);
       $nlinhas = $db->conta_linhas($RSr);
       if ($nlinhas >=0)
       {
          $temresposta=1;
           print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>3. RESTRIÇÕES E PROVIDÊNCIAS-- '.$acacod.'-'.$acadsc.'</b></td></tr></table>';
           print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
           print "<tr><td width='50%' valign='top'> <font color='#FF5B3C'><b><h2>Restrições </h2></font></td><td width='50%' valign='top'><font color='#FF5B3C'><b><h2> Providências </h2></b></font></td></tr>";
           for ($irp=0; $irp<=$nlinhas;$irp++)
           {
              $res = $db->carrega_registro($RSr,$irp);
              if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
              print "<tr><td> ".$restricao." </td><td> ".$providencia." </td></tr>";
              print "<tr><td> Tipo de restrição ".$trsdsc." </td><td></td></tr>";
              print "<tr><td>Data de inclusão: ".$datainc." </td><td> Relator: ".$usunome." / $orgao </td></tr>";
              print "<tr><td colspan=2><hr> </td></tr>";
           }
       }

     }
     if ($_REQUEST['paralt'])
     {
       // parecer da alta gestão

  $sql= "select a.acacod,a.acadsc, ap.avptexto as parecer, to_char(ap.avpdata,'DD/MM/YYYY') as datainc, u.usunome, o.orgdsc as orgao from avaliacaoparecer ap, usuario u, orgao o, acao a where ap.acaid =".$acaoid." and ap.acaid=a.acaid and ap.avporigem=9 and ap.usucpf=u.usucpf and o.orgcod = u.orgcod";
  
  $RSr = $db->record_set($sql);
    $_SESSION['rsparalt'][] = $db->carrega_tudo($RSr);
  $nlinhas = $db->conta_linhas($RSr);

  if ($nlinhas >=0)
  {
      $temresposta=1;
 print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>3. PARECER DA ALTA GESTÃO -- '.$acacod.'-'.$acadsc.'</b></td></tr></table>';
  print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
      for ($irp=0; $irp<=$nlinhas;$irp++)
      {
        $res = $db->carrega_registro($RSr,$irp);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        print "<tr><td><b>Parecer: </b> ".$parecer." </td></tr>";
        print "<tr><td><b>Data de inclusão: </b>".$datainc."</td></tr>";
        print "<tr><td><b>Relator: </b>".$usunome." / $orgao </td></tr>";
        print "<tr><td ><hr> </td></tr>";
      }
  }

  print '</table>';
   }
     if ($_REQUEST['avapar'])
   {
    //$sql = "select ref.refdsc, av.avptexto as avaliacao, av.avpliberada as avlib,u.usunome as avusu, to_char(av.avpdata,'DD/MM/YYYY HH:MM') as avdata, ca.corimgav as corav, ca.corsignificado as avcordsc, tsa.tpsdsc as avtpsdsc, tsp.tpsdsc as partpsdsc , pa.avpliberada as parlib, pa.avptexto as parecer,u2.usunome as parusu, to_char(av.avpdata,'DD/MM/YYYY HH:MM') as pardata,  cp.corimgpar as corpar , cp.corsignificado as pacordsc, o.orgdsc as orgav, o2.orgdsc as orgpar,tsa.tpsdsc as patpsdsc, a.acacod, a.acadsc, a.unicod, a.loccod from referencia ref left join avaliacaoparecer av on av.refcod=ref.refcod and av.tpaid=1 and av.acaid =".$acaoid."  left join cor ca on av.corcod = ca.corcod left join tiposituacao tsa on tsa.tpscod = av.tpscod  left join avaliacaoparecer pa on pa.refcod=ref.refcod and pa.tpaid=2 and pa.acaid = ".$acaoid."   left join usuario u2 on pa.usucpf = u2.usucpf left join usuario u on u.usucpf = av.usucpf left join orgao o on o.orgcod = u.orgcod left join orgao o2 on o2.orgcod=u2.orgcod left join cor cp on pa.corcod = cp.corcod  left join tiposituacao tsp on tsp.tpscod = pa.tpscod left join acao a on a.acaid=av.acaid where ref.refdata_limite_parecer_aca is not null and ref.refdata_limite_avaliacao_aca is not null and ref.refsnmonitoramento='t' and ref.refano_ref='".$_SESSION['exercicio']."'";
    $sql = "select distinct a.acaid,a.acacod, a.acadsc, a.unicod, a.loccod, avp.avptexto as avaliacao,to_char(avp.avpdata,'DD/MM/YYYY HH:MM') as avdata,avp.avpliberada as avlib,r.refdsc, c.corimgav as corav, t.tpsdsc as avtpsdsc, u.usunome as avusu,c.corsignificado as avcordsc,o.orgdsc as orgav ,r.refano_ref, r.refmes_ref from acao a left join avaliacaoparecer avp on a.acaid=avp.acaid left join referencia r on r.refcod=avp.refcod and r.refsnmonitoramento='t' and r.refano_ref='".$_SESSION['exercicio']."' left join cor c on c.corcod=avp.corcod left join tiposituacao t on t.tpscod=avp.tpscod left join usuario u on u.usucpf=avp.usucpf left join orgao o on o.orgcod=u.orgcod where a.acaid=".$acaoid." ";

    
    if ($_REQUEST['dataini'] and $_REQUEST['datafim'])
    {
        $sql.= " and r.refmes_ref >= '".substr($_REQUEST['dataini'],3,2)."' and r.refmes_ref <= '".substr($_REQUEST['datafim'],3,2)."' ";
    }
	
	$sql .= " order by r.refano_ref, r.refmes_ref ";

    $RSavp = $db->record_set($sql);
    $_SESSION['rsavapar'][] = $db->carrega_tudo($RSavp);
    $nlinhasM = $db->conta_linhas($RSavp);

  if ($nlinhasM >=0)
  {
      for ($iap=0; $iap<=$nlinhasM;$iap++)
      {
        $res = $db->carrega_registro($RSavp,$iap);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        
        if ($db->testa_coordenador($acaoid,'A')) $avlib='t';
        
        if ($parecer <> '' or $avaliacao<>'' ) {
           $temresposta=1;
print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>3. Avaliação -- '.$acacod.'.'.$unicod.'.'.$loccod.'-'.$acadsc.'</b></td></tr></table>';
  print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td>Referência:'.$refdsc.'</td></tr><tr>';
      if ($avaliacao <> '' and $avlib=='t')
      print "<td width='50%' valign='top'> <font color='#FF5B3C'><b><h2>Avaliação </h2></font></td>";
       if ($parecer <> '' and $parlib=='t') print "<td width='50%' valign='top'><font color='#FF5B3C'><b><h2> Parecer </h2></b></font></td>";
       if (($parecer <> '' or $avaliacao<>'') and ($avlib=='t' or $parlib=='t') ) print '</tr>';
       
        if (($parecer <> '' or $avaliacao<>'') and ($avlib=='t' or $parlib=='t') ) print '<tr>';
        if ($avaliacao<>'' and $avlib=='t')
        print "<td valign='top'> ".$avaliacao." </td>";
        if ($parecer <> '' and $parlib=='t')
        print "<td valign='top'> ".$parecer." </td>";
        if (($parecer <> '' or $avaliacao<>'') and ($avlib=='t' or $parlib=='t') ) print '</tr><tr>';

        if ($avaliacao<>'' and $avlib=='t')
        print "<td><b>Emitida por:</b> ".$avusu." / $orgav </td>";
        if ($parecer <> '' and $parlib=='t')
        print "<td><b>Emitido por:</b> ".$parusu." / $orgpar </td>";
        if (($parecer <> '' or $avaliacao<>'') and ($avlib=='t' or $parlib=='t') ) print '</tr><tr>';

        if ($avaliacao<>'' and $avlib=='t')
        print "<td><b>Situação:</b> ".$avtpsdsc."&nbsp;&nbsp;<img border='0' src='../imagens/$corav' title='$avcordsc'></td>";
        if ($parecer <> '' and $parlib=='t')
        print "<td><b>Situação:</b> ".$partpsdsc."&nbsp;&nbsp;<img border='0' src='../imagens/$corpar' title='$pacordsc'></td>";
        if (($parecer <> '' or $avaliacao<>'') and ($avlib=='t' or $parlib=='t') ) print '</tr><tr>';
        
        if ($avaliacao<>'' and $avlib=='t')
        print "<td><b>Data de avaliação:</b> ".$avdata." </td>";
        if ($parecer <> '' and $parlib=='t')
        print "<td><b>Data do Parecer:</b> ".$pardata."</td>";
        if (($parecer <> '' or $avaliacao<>'') and ($avlib=='t' or $parlib=='t')) print '</tr><tr><td colspan=2><hr> </td></tr>';
    }
      }
        print '</table>';
  }

   }
   
    if ($_REQUEST['fisevo']){
           // Físico Evolução
       $temresposta=1;
       include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/relatorio/acao/dadosfisevo.inc";
    
     }
   
    if ($_REQUEST['fisevotot']){
           // Físico Evolução Totais
       $temresposta=1;

       if (! $acumulaacao)  $acumulaacao = $acaoid;
       else $acumulaacao .= ','.$acaoid;
 
     }     
    if ($_REQUEST['plantrab']){
           // plano de trabalho
       $temresposta=1;
       include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/relatorio/acao/plantrabevo.inc";    
     }     
   
   }
   if ($_REQUEST['fisevotot']){
       	       $cabecalho = array('Ação','Unidade','Região','Produto','Tipo','Unid. Medida','Realizado','Meta','Percent');
	if (! $_REQUEST['dataini']) $refcod1='01'; else $refcod1=substr($_REQUEST['dataini'],3,2);
	if (! $_REQUEST['datafim']) $refcod2='12'; else $refcod2=substr($_REQUEST['datafim'],3,2);	
	
     $sql = "select a.prgcod||'.'||a.acacod||'.'||a.unicod||'.'||a.loccod||' - '||a.acadsc as codigo, u.unidsc, r.regdsc, pro.prodsc, 
		case when acasnmetanaocumulativa='t' then 'Não Cumulativa' else 'Cumulativa' end as tipometa,
		unm.unmdsc, 
Coalesce(
  (case when a.acasnmetanaocumulativa='t' then 
	(select max(exp.exprealizado) 
             from execucaopto exp where exp.acaid=a.acaid 
                and refcod in(select refcod from referencia where refmes_ref>='$refcod1' and refmes_ref<='$refcod2' and refano_ref='".$_SESSION['exercicio']."')) 
        else 
            (select sum(exp.exprealizado) from execucaopto exp where exp.acaid=a.acaid 
               and refcod in(select refcod from referencia where refmes_ref>='$refcod1' and refmes_ref<='$refcod2'  and refano_ref='".$_SESSION['exercicio']."')) end),'0') as executado,
   d.fisqtdeprevistoano as meta,    
Coalesce(
  (case when a.acasnmetanaocumulativa='t' then 
	(select max(exp.exprealizado)/d.fisqtdeprevistoano/0.01  
             from execucaopto exp where exp.acaid=a.acaid 
                and refcod in(select refcod from referencia where refmes_ref>='$refcod1' and refmes_ref<='$refcod2' and refano_ref='".$_SESSION['exercicio']."')) 
        else 
            (select sum(exp.exprealizado)/d.fisqtdeprevistoano/0.01  from execucaopto exp where exp.acaid=a.acaid 
               and refcod in(select refcod from referencia where refmes_ref>='$refcod1' and refmes_ref<='$refcod2'  and refano_ref='".$_SESSION['exercicio']."')) end),'0') as percent
		from acao a
		inner join unidade u on a.unicod=u.unicod
		inner join regiao r on a.regcod=r.regcod
		inner join produto pro on a.procod=pro.procod and prodsc <> '-'
		inner join unidademedida unm on a.unmcod=unm.unmcod
		inner join dadofisico d on a.acaid=d.acaid
		where a.acasnrap='".$rap."' and a.prgano='".$_SESSION['exercicio']."' and a.acaid in($acumulaacao) order by 1";
     $_SESSION['cab']= $cabecalho;
     $RStot= $db->record_set($sql);
      $_SESSION['rsfisevotot'][] = $db->carrega_tudo($RStot);   
      $_SESSION['conteudo'] =$db->carrega_tudo($RStot);
      $db->monta_lista($sql,$cabecalho,100,20,false,'','');  
       }
   if ($temresposta){
       print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>'.$titul. '</b></td><td>';
       print '<input type="button" class="botao" name="btrelat" value ="Gerar PDF" align="left" onclick="imprime_rel(2)">&nbsp;&nbsp;<a href="ar505ptb.exe"><img border="0" width="30" heigth="30" src="../imagens/pdf_adobe.jpg" title=" Você não tem o Adobe Reader? Clique aqui!"></a>';
            if (($_REQUEST['finevo']) and (! $_REQUEST['fisevotot']) and (! $_REQUEST['dadobas'] ) and (! $_REQUEST['avapar'] ) and (! $_REQUEST['fisevo'])and (! $_REQUEST['plantrab'] ) and (! $_REQUEST['respro'] )and (! $_REQUEST['paralt'] )) print '&nbsp;&nbsp;<input type="button" class="botao" name="btcsv" value ="Gerar XLS" align="left" onclick="imprime_rel(3)">&nbsp;&nbsp;<input type="button" class="botao" name="btcsv" value ="Gerar CSV" align="left" onclick="imprime_rel(5)">';
    else 
if (($_REQUEST['fisevotot']) and (! $_REQUEST['finevo']) and (! $_REQUEST['dadobas'] ) and (! $_REQUEST['avapar'] ) and (! $_REQUEST['fisevo'])and (! $_REQUEST['plantrab'] ) and (! $_REQUEST['respro'] )and (! $_REQUEST['paralt'] )) print '&nbsp;&nbsp;<input type="button" class="botao" name="btcsv" value ="Gerar XLS" align="left" onclick="imprime_rel(3)">&nbsp;&nbsp;<input type="button" class="botao" name="btcsv" value ="Gerar CSV" align="left" onclick="imprime_rel(5)">'; 
       print '</td></tr></table>';
   } else {
   print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td>'.$titul. '</td><td>';
       print '<b>Nenhum registro atendeu aos critérios de filtro.';
       print '</td></tr></table>';
       }
}
else {
   print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td>'.$titul. '</td><td>';
       print '<b>Nenhum registro atendeu aos critérios de filtro.';
       print '</td></tr></table>';
       }
?>
