 <?

   /*
   sistema simec
   setor respons�vel: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   m�dulo:avalia_aca.inc
   finalidade: permitir a avalia��o de acao
   */

	if ( $refdata_limite_avaliacao_aca >= date( 'Y-m-d' ) )
	{
		$habil = 'S';
	}
	else
	{
		// n�o est� autorizado na data
		// ent�o testa se tem alguma autoriza��o especial
		$sql= "select ae.aelid from autorizacaoespecial ae where ae.acaid =".$_SESSION['acaid']." and ae.aelstatus ='A' and ae.aeldata_inicio <='".date('Y-m-d')."' and ae.aeldata_fim >='".date('Y-m-d')."' and ae.togcod in (3,4) and ae.refcod=".$refcod;
		$registro=$db->recuperar($sql);
		if ( is_array( $registro ) ) $habil = 'S';
		else $habil = 'N';
	}
	$habil = 'S';
	// a variavel HABIL indica se esta dentro da data
	$mostra_av = true;

	//testo se n�o for o titular, ent�o, verifico se est� liberado para o p�blico
	if ( !$autorizado_av and !$autorizado_dig )
	{
		if ( !$avlib or $avlib == 'f' )
		{
			$avaliacao = '<h2><center>N�o liberada</center></h2>';
			$mostra_av = false;
		}
	}
	else
	{
		if ( ( ! $avlib or $avlib == 'f' ) and $habil == 'N' )
		{
			$avaliacao = '<h2><center>Prazo para libera��o expirado</center></h2>';
			$mostra_av = false;
		}
	}
	if ( !$avaid ) $avaid = 0;
	$aval = 'avaliacao' . '_' . $refcod . '_' . $avaid . '_' . $i;
	${$aval} = $avaliacao;

	
?>
<table border="0" cellspacing="1" cellpadding="1" align="center" bgcolor="#F5F5F5" style="width:100%;color:#808080;">

<?
   if ((! $autorizado_av and ! $autorizado_dig) or $habil=='N' or $avlib=='t')
   {
    ?>
       <tr>
          <td colspan='4'><DIV class="treeview" style="OVERFLOW:AUTO; WIDTH:100%; HEIGHT:150px; BORDER:2px SOLID #ECECEC; background-color: White;">
          	<? if ($mostra_av==true) {?>
          		<div align="right">
          			<img src="../imagens/preview.gif" border="0" align="absmiddle">
          				<a href="javascript:visualiza('<?=md5_encrypt($avaid,'')?>')">Ver em tela cheia.</a>
          		</div>
          	<?}?>
          	
          	<?=$avaliacao ?></div>
		  </td></tr>
  <? }
     else
     { 
       // ent�o � o titular ou um digitador e pode digitar
       ?>
       <tr>
	      <td colspan="2" align="center">
	      	<?php 
	      	//xx($aval);
	      	?>
	      	<script type="text/javascript">
	      		//listaTextArea[listaTextArea.length] = '<?= $aval ?>';
	      	</script>
	      	<?
	      	$avaliacao = strip_tags( htmlspecialchars_decode($avaliacao, ENT_QUOTES) );
	      	$avaliacao = html_entity_decode($avaliacao);
	      	
	      	$arrTrans = array("&ndash;" => "-", "&nbsp;" => "");
	      	
	      	$avaliacao = strtr($avaliacao, $arrTrans);
	      	
	      	if( $_REQUEST['refsnmonitoramento'] == 'f'){
	      		$edita = "N";
	      	}else{
	      		$edita = "S";
	      	}
	      	?>
	     	<?=campo_textarea( $aval, 'N', $edita, '', '100', 15, '5000', '', '', '', '', '', $avaliacao ); ?>
	     	
	     	<input type=hidden id="campoDesc" name="campoDesc" value=<?=$aval ?>>  
	     	<input type=hidden id="texto_justificativa" name="texto_justificativa" >
	      </td>
	   </tr>
     <?}?>
 </table>
 
 <td valign="top">
 
  <table border="0" cellspacing="1" cellpadding="3" align="center" bgcolor="#F5F5F5" style="width:100%;color:#808080;">
   <?
     // segundo n�vel da tabela, caso j� tenha sido liberada, apresenta os dados de data
     // situa��o e cor
     if ($avlib=='t')
     {
       print '<tr><td align="right" class="subtitulodireita">Produto da A��o:</td><td>'.$prodsc.'<input type="Hidden" name="prodsc'.$i.'" value="'.$prodsc.'"></td></tr>';
	   print '<tr><td align="right" class="subtitulodireita">Unidade de Medida:</td><td>'.$unmdsc.'</td></tr>';
	   ${"exprealizado".$i} = $exprealizado;
	   if (in($procod,array('0','3')))
	   print '<tr><td align="right" class="subtitulodireita">Realizado no Per�odo:</td><td>-</td></tr>';
	   else
	   print '<tr><td align="right" class="subtitulodireita">Realizado no Per�odo:</td><td>'.campo_texto('exprealizado'.$i,'N','N','',10,10,'##########','','right','Informe a quantidade executada no per�odo de refer�ncia para esta A��o.').'</td></tr>';?>
       <tr>
       <!--  
       <td align="right" class="subtitulodireita">Situa��o:</td><td><img src="../imagens/<?=trim(strtolower($corimgav))?>" border="0" title="<?=trim($avcordsc)?>"> <?=$avtpsdsc?></td>
       -->
       <td align="right" class="subtitulodireita">Data:</td><td><?=$avdata?></td>
       </tr>
       <?
	    $sql="select u.usunome,u.usuemail,u.usufoneddd,u.usufonenum,o.orgdsc from usuario u left join orgao o on u.orgcod = o.orgcod where u.usucpf='".$avusu."'";
        $RSu = $db->record_set($sql);
        $resu =  $db->carrega_registro($RSu,0);
        if(is_array($resu)) foreach($resu as $k=>$v) ${$k}=$v;
        print '<tr><td align="right" class="subtitulodireita">Resp.:</td><td>';
		if ($avusu<>$_SESSION['usucpf']) 
		{ // n�o manda e-mail para ele mesmo
		print '<img src="../imagens/email.gif" title="Envia e-mail ao Gestor" border="0" onclick="envia_email(\''.$avusu.'\');"> ';
		}
		print $usunome.'</td><td align="right" class="subtitulodireita">Tel.:</td><td>'.$usufonenum.'</td></tr>';
        print '<tr><td align="right" class="subtitulodireita">�rg�o:</td><td>'.$orgdsc.'</td></tr>';
     }
     else
     { 
       // apresenta a situa��o e a escolha de cor
       if ($mostra_av==true){
	   print '<tr><td align="right" class="subtitulodireita">Produto da A��o:</td><td>'.$prodsc.'<input type="Hidden" name="prodsc'.$i.'" value="'.$prodsc.'"></td></tr>';
	   print '<tr><td align="right" class="subtitulodireita">Unidade de Medida:</td><td>'.$unmdsc.'</td></tr>';
	   ${"exprealizado".$i} = $exprealizado;
	   ${"avpdtapuracao".$i} = $avpdtapuracao;
	    if (in($procod,array('0','3')))
	   print '<tr><td align="right" class="subtitulodireita">Realizado no Per�odo:</td><td>-<input type="Hidden" name="exprealizado'.$i.'" value="-"></td></tr>';
		else
		include_once APPRAIZ ."includes/workflow.php";
			$docid = pegaDocidAvaliacao( $avpid );
			$arrEstado = wf_pegarEstadoAtual($docid);
			if(WK_FINALIZADO == $arrEstado['esdid']){
			   print '<tr><td align="right" class="subtitulodireita">Realizado no Per�odo:</td><td>'.campo_texto('exprealizado'.$i,'S',( $permPreenchimento2 ? 'S' : 'N'),'',10,10,'##########','','right','Informe a quantidade executada no per�odo de refer�ncia para esta A��o.').'</td></tr>';
			   print '<tr><td align="right" class="subtitulodireita">Data de Apura��o:</td><td>'.campo_data2( 'avpdtapuracao'.$i, 'N', ( $permPreenchimento2 ? 'S' : 'N'), '', 'S' ).'</td></tr>';
			}else{
			   print '<tr><td align="right" class="subtitulodireita">Realizado no Per�odo:</td><td>'.campo_texto('exprealizado'.$i,'S',( $permPreenchimento ? 'S' : 'N'),'',10,10,'##########','','right','Informe a quantidade executada no per�odo de refer�ncia para esta A��o.').'</td></tr>';
			   print '<tr><td align="right" class="subtitulodireita">Data de Apura��o:</td><td>'.campo_data2( 'avpdtapuracao'.$i, 'N', ( $permPreenchimento ? 'S' : 'N'), '', 'S' ).'</td></tr>';
			}
//       print '<tr><td align="right" class="subtitulodireita">Situa��o:</td><td>';
//	       // pode fazer avalia��o
//	      $sql = "select tpscod as CODIGO,tpsdsc as descricao from tiposituacao where tpsstatus='A' order by tpsdsc ";
//		  ${"tpscodav".$i}=$avtps;
//		  $db->monta_combo("tpscodav".$i,$sql,'S',"Escolha uma situa��o",'','','Informe a situa��o da A��o no per�odo de refer�ncia.');
//		  print '</td></tr>';
	      print '<tr><td align="right" class="subtitulodireita">Cor:</td><td>';
		  $sql = "select corcod as CODIGO,corsignificado as desc , corimgav as imagem from cor where corstatus='A' and corcod>0 order by corcod ";
		  $RSradio = $db->record_set($sql);
	      $nlinhasradio = $db->conta_linhas($RSradio);
	      for ($j=0; $j<=$nlinhasradio;$j++)
	      {
	        $res2 =  $db->carrega_registro($RSradio,$j);
	        if(is_array($res2)) foreach($res2 as $k=>$v) ${$k}=$v;
	        if ($codigo == $avcor) $ck = ' checked '; else $ck ='';
	        print "<input type='radio' value=".$codigo.$ck." name='corcodav".$i."'  title='".$desc."'>";
	        print "<img src='../imagens/".trim($imagem)."' title='".$desc."'>";
          }
          print "</td></tr>";
          }
      }
      //dbg($refcod.','.$avaid.','.$i,0);
    // quarto n�vel da tabela, bot�es de a��o
    // haver� tr�s tipos de bot�o: gravar, liberar, bloquear
    // se for digitador ou titular e habil = s e n�o liberado, mostra o bot�o gravar
    
    if (($autorizado_av or $autorizado_dig) and $habil=='S')
    {
      print '<tr><td align="right" class="subtitulodireita">A��es:</td><td colspan="4">';
      if (! $avlib or $avlib=='f' or $avaid==0) {
		if ($_REQUEST['refcod'] != 'x'){
			$docid = pegaDocidAvaliacao( $avpid );
			$arrEstado = wf_pegarEstadoAtual($docid);
			if(WK_FINALIZADO == $arrEstado['esdid']){
	      		print '<input type="button" '. ($permPreenchimento ? '' : ' disabled="disabled" ') .' class="botao" name="btgrava" value= "Gravar" onclick="grava_av('.$refcod.','.$avaid.','.$i.')" disabled="disabled">&nbsp;&nbsp;&nbsp;';
			}elseif($edita == "S"){
	      		print '<input type="button" '. ($permPreenchimento ? '' : ' disabled="disabled" ') .' class="botao" name="btgrava" value= "Gravar" onclick="grava_av('.$refcod.','.$avaid.','.$i.')">&nbsp;&nbsp;&nbsp;';
			}else{
	      		print '&nbsp;&nbsp;&nbsp;';
			}
	      		print '<input type="button" name="btl_voltar" value="Voltar" onclick="location.href=\'monitora.php?modulo=principal/detalhesppa&acao=A&aba=acao&codigo='.$_REQUEST['codigo'].'\'">';
      	}
      }
      // se for o titular e  ainda est� dentro do prazo, pode liberar ou bloquear para nova edi��o
      if ($autorizado_av)
      {
         if ((!$avlib or $avlib=='f') and $avaid > 0)
         {
//            print '&nbsp;&nbsp;&nbsp;<input type="button" class="botao" name="btlibera" value= "Liberar" onclick="libera_av('.$refcod.','.$avaid.','.$i.')">&nbsp;&nbsp;&nbsp;';
         }
         else if ($avaid > 0 and $acasnbgu=='f')
         {
            print '&nbsp;&nbsp;&nbsp;<input type="button" class="botao" name="btlibera" value= "Corrigir Avalia��o Liberada" onclick="bloquea_av('.$refcod.','.$avaid.','.$i.')">&nbsp;&nbsp;&nbsp;';
         }
      }
    }
    print '</td></tr>';
   ?>
   </table>
   

