 <?
  /*
   sistema simec
   setor respons�vel: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   m�dulo:parecer_prg.inc
   finalidade: permitir o parecer de programa
   */
   if ($refdata_limite_parecer_prg >= date('Y-m-d')) $habil = 'S';
   else {
     // n�o est� autorizado na data
     // ent�o testa se tem alguma autoriza��o especial
     $sql= "select ae.aelid from autorizacaoespecial ae where ae.prgid =".$_SESSION['prgid']." and ae.aelstatus ='A' and ae.aeldata_inicio <='".date('Y-m-d')."' and ae.aeldata_fim >='".date('Y-m-d')."' and ae.togcod=2 and ae.refcod=".$refcod;
    $registro=$db->recuperar($sql);
    if (is_array($registro)) $habil = 'S'; else
       $habil = 'N';
   }
   // a variavel HABIL indica se esta dentro da data
   $mostra_par = true;
   //testo se n�o for o titular, ent�o, verifico se est� liberado para o p�blico
   if (!$autorizado_par and ! $autorizado_dig)
   {
      if (! $parlib or $parlib=='f') {$parecer = '<h2><center>N�o liberado</center></h2>';$mostra_par = false;}
   }
   else
   	{
         if ((! $parlib or $parlib=='f') and $habil == 'N')
         {
           $parecer = '<h2><center>Prazo para libera��o expirado</center></h2>';
           $mostra_par = false;}
    }
   if (! $parid) $parid=0;
   $parec ='parecer'.'_'.$refcod.'_'.$parid.'_'.$i;
   ${$parec} = $parecer;

  ?>
  <table border="0" cellspacing="1" cellpadding="3" align="center" bgcolor="#F5F5F5" style="width:100%;color:#808080;">
<?
   if ((! $autorizado_par and ! $autorizado_dig) or $habil=='N' or $parlib=='t')
   {
    ?>
       <tr>
          <td colspan='4'><DIV class="treeview" style="OVERFLOW:AUTO; WIDTH:100%; HEIGHT:173px; BORDER:2px SOLID #ECECEC; background-color: White;"><? if ($mostra_par==true) {?><div align="right" style="OVERFLOW:AUTO;"><img src="../imagens/preview.gif" border="0" align="absmiddle"> <a href="javascript:visualiza('<?=md5_encrypt($parid,'')?>')">Ver em tela cheia.</a></div><?}?><?=$parecer ?></div>
          
		  </td></tr>
  <? }
     else
     {
       // ent�o � o titular ou um digitador e pode digitar
       ?>
	   <tr>
	      <td colspan="4" align="center"><?=campo_textarea("$parec",'N',$habil,'',65,13,'')?></td></tr>
     <?}?>
   <?
     // segundo n�vel da tabela, caso j� tenha sido liberada, apresenta os dados de data
     // situa��o e cor
     if ($parlib=='t')
     {
             ?>
       <tr><td align="right" class="subtitulodireita">Situa��o:</td><td><img src="../imagens/<?=trim(strtolower($corimgpar))?>" border="0" title="<?=trim($parcordsc)?>"> <?=$partpsdsc?></td><td align="right" class="subtitulodireita">Data:</td><td><?=$pardata?></td></tr>
       <?
	    $sql="select u.usunome,u.usuemail,u.usufoneddd,u.usufonenum,o.orgdsc from usuario u left join orgao o on u.orgcod = o.orgcod where u.usucpf='".$parusu."'";
        $RSu = $db->record_set($sql);
        $resu =  $db->carrega_registro($RSu,0);
        if(is_array($resu)) foreach($resu as $k=>$v) ${$k}=$v;
        print '<tr><td align="right" class="subtitulodireita">Resp.:</td><td>';
		if ($parusu<>$_SESSION['usucpf']) 
		{ // n�o manda e-mail para ele mesmo
		print '<img src="../imagens/email.gif" title="Envia e-mail ao Gestor" border="0" onclick="envia_email(\''.$parusu.'\');"> ';
		}
		print $usunome.'</td><td align="right" class="subtitulodireita">Tel.:</td><td>'.$usufonenum.'</td></tr>';
        print '<tr><td align="right" class="subtitulodireita">�rg�o:</td><td>'.$orgdsc.'</td></tr>';
     }
     else
     {
       // apresenta a situa��o e a escolha de cor
       if ($mostra_par==true){
       print '<tr><td align="right" class="subtitulodireita">Situa��o:</td><td>';
	       // pode fazer parecer
	      $sql = "select tpscod as CODIGO,tpsdsc as descricao from tiposituacao where tpsstatus='A' order by tpsdsc ";
	      $lista = $db->carregar($sql);
		  print "<select name='tpscodpar".$i."' class='CampoEstilo'>";
		  print "<option value=''>Escolha uma situa��o</option>";
		  for ($n=0;$n<count($lista);$n++) {
	        if ($partps == $lista[$n]['codigo']){
				print "<option value='".$lista[$n]['codigo']."' selected>".$lista[$n]['descricao']."</option>";
	        } else {
				print "<option value='".$lista[$n]['codigo']."'>".$lista[$n]['descricao']."</option>";
	             }
	      }
	      print '</select></td></tr>';
	      print '<tr><td align="right" class="subtitulodireita">Cor:</td><td>';
		  $sql = "select corcod as CODIGO,corsignificado as desc , corimgpar as imagem from cor where corstatus='A' order by corcod ";
		  $RSradio = $db->record_set($sql);
	      $nlinhasradio = $db->conta_linhas($RSradio);
	      for ($j=0; $j<=$nlinhasradio;$j++)
	      {
	        $res2 =  $db->carrega_registro($RSradio,$j);
	        if(is_array($res2)) foreach($res2 as $k=>$v) ${$k}=$v;
	        if ($codigo == $parcor) $ck = ' checked '; else $ck ='';
	        print "<input type='radio' value=".$codigo.$ck." name='corcodpar".$i."'  title='".$desc."'>";
	        print "<img src='../imagens/".trim($imagem)."' title='".$desc."'>";
          }
          print "</td></tr>";
          }
      }
    // quarto n�vel da tabela, bot�es de a��o
    // haver� tr�s tipos de bot�o: gravar, liberar, bloquear
    // se for digitador ou titular e habil = s e n�o liberado, mostra o bot�o gravar
    if (($autorizado_par or $autorizado_dig) and $habil=='S')
    {
      print '<tr><td align="right" class="subtitulodireita">A��es:</td><td>';
      if (! $parlib or $parlib=='f' or $parid==0)
      {
         print '<input type="button" class="botao" name="btgrava" value= "Gravar" onclick="grava_par('.$refcod.','.$parid.','.$i.')">&nbsp;&nbsp;&nbsp;';
      }
      // se for o titular e  ainda est� dentro do prazo, pode liberar ou bloquear para nova edi��o
      if ($autorizado_par)
      {
         if ((! $parlib or $parlib=='f') and $parid > 0)
         {
            print '&nbsp;&nbsp;&nbsp;<input type="button" class="botao" name="btlibera" value= "Liberar" onclick="libera_par('.$refcod.','.$parid.','.$i.')">&nbsp;&nbsp;&nbsp;';
         }
         else if ($parid > 0)
         {
            print '&nbsp;&nbsp;&nbsp;<input type="button" class="botao" name="btlibera" value= "Bloquear parecer liberado" onclick="bloquea_par('.$refcod.','.$parid.','.$i.')">&nbsp;&nbsp;&nbsp;';
         }
      }
    }
    print '</td></tr>';
   ?>
   

   </table>
