<?
//Constantes seguranca.perfil
define("PFL_UNIDADE_MONITORAMENTO_AVALIACAO", 18);

function ver_iniciativa($dados) {
	global $db;
	$vl = explode(".",$dados['codigo']); 
	
	$sql = "select i.* from monitora.iniciativaacao ia 
			inner join monitora.iniciativa i on i.ictid = ia.ictid 
			where acacod='".$vl[1]."'";
	
	$dados_ = $db->pegaLinha($sql);
	?>
	<table class="listagem" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Resumo da Iniciativa do Programa</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="25%"><b>C�digo :</b></td>
		<td><?=$dados_['ictcod'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="25%"><b>Iniciativa do Programa :</b></td>
		<td><?=$dados_['ictdsc'] ?></td>
	</tr>
	</table>
	<?
	
}
function ver_acao($dados) {
	global $db;
	
	if ( empty($dados['codigo']) ){
            die("<script type=\"text/javascript\">window.location='?modulo=principal/detalhesppa&acao=A&aba=acao&codigo=" . $_SESSION['acao']['codigo'] . "';</script>");
	}
	
	$_SESSION['acao']['codigo'] = ($dados['codigo'] ? $dados['codigo'] : $_SESSION['acao']['codigo']);
	$dados['codigo'] = $_SESSION['acao']['codigo'];
	
	$sql_acao = "
            SELECT * 
            FROM monitora.acao a
            INNER JOIN monitora.programa p ON a.prgid = p.prgid  
            WHERE a.prgcod||'.'||a.acacod||'.'||a.unicod||'.'||a.loccod = '".$dados['codigo']."'
                AND acasnrap = false AND a.prgano='".$_SESSION['exercicio']."'";
//	echo $sql_acao;
//	die;
	$acao = $db->pegaLinha($sql_acao);
	
	
	
	$_SESSION['acaid'] = $acao['acaid'];
	
	
	$sql_obj_ini = "SELECT  o.objcod, o.objdsc as objetivo, i.ictcod, i.ictdsc as iniciativa
            FROM monitora.iniciativaacao ia
            INNER JOIN monitora.iniciativa  i ON ia.ictid = i.ictid 
            INNER JOIN monitora.objetivoiniciativa oi ON i.ictid = oi.ictid
            INNER JOIN monitora.objetivo o ON o.objid = oi.objid	   
            WHERE ia.acacod = '".$acao['acacod']."'";
	
	
	$obj_ini = $db->pegaLinha($sql_obj_ini);
	
	global $acahistorico, $acatitulo, $acabaselegal, $acadetalhamento, $esfdsc, $acadsc, $orgdsc, $unidade, $fundsc, $sfudsc, $acafinalidade, $acadescricao, $prodsc, $meta, $unmdsc, $tacdsc, $acarepercfinanceira, $tipoinclusao, $tipoacao, $prodbem, $inicioacao, $terminoacao;
        
        if(!$acao){
            echo <<<HTML
            <center style="color:red;">Dados da a��o n�o foram encontrados.</center>
HTML;
            return;
        }
	extract($acao);
	
	$prodbem = $prodsc;
	
	$sql= "SELECT orgdsc FROM orgao WHERE orgcod='".$acao['orgcod']."'";
  	$orgdsc = $db->pegaUm($sql);
  	
	$sql= "SELECT unicod||'-'||unidsc AS unidade FROM unidade WHERE unicod='".$acao['unicod']."'";
  	$unidade = $db->pegaUm($sql);
  	
    if($acao['funcod']) {
        $sql= "select funcod||'-'||fundsc AS fundsc FROM ppafuncao WHERE funcod='".$acao['funcod']."'";
    	$fundsc = $db->pegaUm($sql);
	}
	
	if($acao['sfucod']) {
        $sql= "SELECT sfucod||'-'||sfudsc AS sfudsc FROM ppasubfuncao WHERE sfucod='".$acao['sfucod']."'";
    	$sfudsc = $db->pegaUm($sql);
	}
	
//	if ( $acao['procod'] ){
//		$sql= "SELECT prodsc FROM produto WHERE procod='".$acao['procod']."'";
//	  	$prodbem = $db->pegaUm($sql);
//	}
	if($acao['acasnmetanaocumulativa']=='t') $tipometa ='N�o cumulativa';
	if($acao['acasnmetanaocumulativa']=='f') $tipometa ='Cumulativa';
	if($acao['acasnmetanaocumulativa']=='') $tipometa ='N�o informado';
	
	$sql= "SELECT fisqtdeprevistoano as meta FROM dadofisico WHERE acacod='".$acao['acacod']."' AND saccod='".$acao['saccod']."' AND regcod='".$acao['regcod']."' AND prgcod='".$acao['prgcod']."' AND prgano='".$_SESSION['exercicio']."'";
  	$meta = $db->pegaUm($sql);
	
//    $sql= "SELECT unmdsc FROM public.unidademedida WHERE unmcod='".$acao['unmcod']."'";
//    $unmdsc = $db->pegaUm($sql);
  	
    $sql= "SELECT tacdsc FROM public.tipoacao WHERE taccod='".$acao['taccod']."'";
    $tacdsc = $db->pegaUm($sql);
    
    if ($acao['esfcod']) {
        $sql= "SELECT esfcod||'-'||esfdsc as esfdsc FROM esfera WHERE esfcod='".$acao['esfcod']."'";
  		$esfdsc = $db->pegaUm($sql);
	}

?> 
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	 <tr>
        <td align='right' class="SubTituloDireita" style="width: 20%">Momento da A��o:</td>
        <td><?=$acao['descricaomomento']; ?></td>
      </tr>	
	 <tr>
        <td align='right' class="SubTituloDireita" style="width: 20%">Programa:</td>
        <td><b><?=$acao['prgcod']; ?></b> - <?=$acao['prgdsc'] ?></td>
      </tr>
	 <tr>
        <td align='right' class="SubTituloDireita" style="width: 20%">Objetivo:</td>
        <td><p align="justify"><?=$obj_ini['objcod'] .' - '. $obj_ini['objetivo']; ?></p></td>
      </tr>
	 <tr>
        <td align='right' class="SubTituloDireita" style="width: 20%">Iniciativa:</td>
        <td><p align="justify"><?=$obj_ini['ictcod'] . ' - ' . $obj_ini['iniciativa']; ?></p></td>
      </tr>      
      <tr>
        <td align='right' class="SubTituloDireita">A��o:</td>
        <td><b><?=$acao['acacod']; ?></b>.<?=$acao['unicod'].'.'.$acao['loccod']?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">T�tulo:</td>
        <td><?=campo_textarea('acatitulo','N','N','',100,3,'');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Org�o Or�ament�rio Respons�vel:</td>
        <td><?=campo_texto('orgdsc','N','N','',100,100,'','');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Unidade Or�ament�ria Respons�vel:</td>
        <td><?=campo_texto('unidade','N','N','',100,100,'','');?></td>
      </tr>
      <? if($acao['esfcod']) {?>
        <tr>
	      <td align='right' class="SubTituloDireita">Esfera:</td>
          <td>
             <?=campo_texto('esfdsc','N','N','',50,50,'','');?>
          </td>
         </tr>
       <?}?>      
	  <?  if ($fundsc) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Fun��o:</td>
        <td><?=campo_texto('fundsc','N','N','',50,50,'','');?></td>
      </tr>
      <?}?>
      <? if ($sfudsc) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Subfun��o:</td>
        <td><?=campo_texto('sfudsc','N','N','',50,50,'','');?></td>
      </tr>
      <?}?>
      <?php if($acao['tipoacao']){?>
      <tr>
	     <td align='right' class="SubTituloDireita">Tipo de a��o:</td>
      	 <td><?=campo_texto('tipoacao','N','N','',50,50,'','');?></td>
      </tr>      
      <?php } ?>
      <?php if($tipoinclusao){?>
      <tr>
	     <td align='right' class="SubTituloDireita">Origem:</td>
	     <td><?=campo_texto('tipoinclusao','N','N','',50,50,'','');?></td>
      </tr>      
      <?php } ?>
      <? if ($acafinalidade) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Finalidade:</td>
        <td><?=campo_textarea('acafinalidade','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
      <? if ($acadescricao) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Descri��o:</td>  
        <td><?=campo_textarea('acadescricao','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
    <? if ($prodsc) {?>
       <tr>
        <td align='right' class="SubTituloDireita">Produto (Bem ou Servi�o):</td>
        <td><?=campo_texto('prodsc','N','N','',100,100,'','');?></td>
      </tr>
      <?}?>
       <tr>
        <td align='right' class="SubTituloDireita">Meta:</td>
        <td> <?=number_format($acao['acaqtdprevistoanocorrente'],0,',','.')?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Unidade de Medida:</td>
        <td><?=campo_texto('unmdsc','N','N','',100,100,'','');?></td>
      </tr>

  <?
    if ($acao['acasnoutrasfontes'] =='t') {?>
        <tr>
        <td align='right' class="SubTituloDireita">Quanto ao Or�amento:</td>
        <td>N�o or�ament�ria</td>
      </tr>
   <? } else {?>
        <tr>
           <td align='right' class="SubTituloDireita">Quanto ao Or�amento:</td>
           <td>Or�ament�ria</td>
        </tr>
        <tr>
           <td align='right' class="SubTituloDireita">Tipo de Or�amento:</td>
           <td>
	    <?
	         if ($acao['acasnfiscalseguridade']=='t') print 'Fiscal&nbsp;&nbsp;&nbsp;&nbsp;';
  	         if ($acao['acasnfiscalseguridade']=='f') print 'Seguridade&nbsp;&nbsp;&nbsp;&nbsp;';
  	         if ($acao['acasninvestatais']=='t') print 'Investimentos Estatais&nbsp;&nbsp;&nbsp;&nbsp;';
	    ?>
          </td>
      </tr>
      <?if ($acao['taccod']==1){?>
        <tr>
	      <td align='right' class="SubTituloDireita">Justificativa da Repercuss�o Financeira sobre o Custeio da Uni�o:</td>
          <td>
             <?=campo_textarea('acarepercfinanceira','N','N','',100,5,'');?>
          </td>
         </tr>
        <tr>
	      <td align='right' class="SubTituloDireita">Valor Estimado da Repercuss�o Financeira (R$/ano):</td>
          <td>
             <?=number_format($acao['acaqtdcustototal'],2,',','.')?>
          </td>
         </tr>
      <? } ?>
      <tr>
	     <td align='right' class="SubTituloDireita">Forma de Implementa��o:</td>
         <td><input type="hidden" name="tacdsc" >
         <?
            if ($acasnmedireta=='t') print 'Direta<br>';
            if ($acasnmedesc=='t')   print 'Descentralizada<br>';
		    if ($acasnmelincred=='t') print 'Linha de Cr�dito<br>';
            if ($acasntransfobrigatoria=='t') print 'Transfer�ncia obrigat�ria<br>';
            if ($acasntransfvoluntaria=='t') print 'Transfer�ncia volunt�ria<br>';
            if ($acasntransfoutras=='t') print 'Outras transfer�ncias<br>';
         ?>
         </td>
      </tr>
      <? if ($acadetalhamento) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Detalhamento da Implementa��o:</td>
        <td><?=campo_textarea('acadetalhamento','N','N','',100,3,'');?></td>
      </tr>
      <?}?>
      <?}?>
     
      <?
    if ($acabaselegal ) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Base legal:</td>
        <td><?=campo_textarea('acabaselegal','N','N','',100,3,'');?></td>
      </tr>
     <?}?>
      <? if ($acahistorico) {?>
     <tr>
        <td align='right' class="SubTituloDireita">Hist�rico da a��o:</td>
       <td><?=campo_textarea('acahistorico','N','N','',100,5,'');?></td>
      </tr>
     <?}?>
     <?php if($acao['tipoacao'] == "Projeto"){ ?>
     <tr>
        <td align='right' class="SubTituloDireita">�nicio da a��o:</td>
        <td><?=campo_texto('inicioacao','N','N','',50,50,'','');?></td>
     </tr>
     <tr>
        <td align='right' class="SubTituloDireita">T�rmino da a��o:</td>
       	<td><?=campo_texto('terminoacao','N','N','',50,50,'','');?></td>
     </tr>
     <?}?>          
</table>
<? 
}


function ver_metas($dados) {
	global $db;
//	dbg($dados, d);
	?>
	<script>
	function gerenciarMetas(objid,metid) {
		window.open('monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarMetas&atualizartodapaginapai=true&objid='+objid+'&metid='+metid,'Metas','scrollbars=yes,height=400,width=750,status=no,toolbar=no,menubar=no,location=no');
	}
	
	function gerenciarAnaliseMeta(metid) {
//		var j = window.open('monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseSituacionalMeta&objid=<?=$dados['objid'] ?>&metid='+metid,'AnaliseObjetivosMeta','scrollbars=no,height=500,width=700,status=no,toolbar=no,menubar=no,location=no');
		var j = window.open('monitora.php?modulo=principal/telaprograma&acao=A&requisicao=listarAnaliseSituacionalMeta&objid=<?=$dados['objid'] ?>&metid='+metid,'AnaliseObjetivosMeta','scrollbars=yes,height=500,width=750,status=no,toolbar=no,menubar=no,location=no');
		j.focus();
	}
	
	function gerenciarAnaliseGlobalMeta(metid){
		var j = window.open('monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseGlobalMeta&metid='+metid,'Analise Global Meta','scrollbars=yes,height=500,width=750,status=no,toolbar=no,menubar=no,location=no');
		j.focus();
	}
	
	function visualizaIndicadoresMeta(metid){
		window.location.href = 'monitora.php?modulo=principal/indicadorMeta&acao=A&prgcod=<?php echo $_REQUEST['prgcod'] ?>&objid=<?php echo $_REQUEST['objid'] ?>&metid='+metid;
	}
	
	</script>
	<?
	
	$cabecalho = array("<b>A��es</b>", "<b>Meta</b>", "<b>Periodicidade</b>");
	
	$perfis = pegaPerfilGeral();
	
	if( $db->testa_superuser() || in_array(PERFIL_MONIT_CPMO, $perfis) ){
		$campo = "'<div style=\"white-space:nowrap;\">
						<img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"gerenciarMetas(\'".$dados['objid']."\',\''||m.metid||'\');\" title=\"Editar Meta\">
				   		<img src=../imagens/alteracao.gif style=cursor:pointer; onclick=\"gerenciarAnaliseGlobalMeta('||m.metid||');\" title=\"An�lise Global da Meta\">
				   		<img src=../imagens/principal.gif style=cursor:pointer; onclick=\"gerenciarAnaliseMeta('||m.metid||');\" title=\"Inserir an�lise situacional\">  '
				   		|| (case when count(distinct mi.indid ) > 0 then '<img src=../imagens/migra.gif style=cursor:pointer; onclick=\"visualizaIndicadoresMeta('||m.metid||');\" title=\"Metas dos Indicadores\">' else '' end) || '
				   </div>' as acao,";
	}elseif( in_array(PERFIL_MONIT_AVALIADOR_OBJETIVO, $perfis) || in_array(PERFIL_MONIT_AVALIADOR_META, $perfis) || in_array(PERFIL_MONIT_CONSULTA, $perfis) || in_array(PERFIL_MONIT_AVALIADOR_INICIATIVA, $perfis)){
		$campo = "'<div style=\"white-space:nowrap;\">
						<img src=../imagens/alteracao.gif style=cursor:pointer; onclick=\"gerenciarAnaliseGlobalMeta('||m.metid||');\" title=\"An�lise Global da Meta\"> 
				   		<img src=../imagens/principal.gif style=cursor:pointer; onclick=\"gerenciarAnaliseMeta('||m.metid||');\" title=\"Inserir an�lise situacional\">' 
				   		|| (case when count(distinct mi.indid ) > 0 then '<img src=../imagens/migra.gif style=cursor:pointer; onclick=\"visualizaIndicadoresMeta('||m.metid||');\" title=\"Metas dos Indicadores\">' else '' end) || '
				   </div>' as acao,";
	}elseif( in_array(PERFIL_MONIT_COORDENADOR_ACAO, $perfis) || in_array(PERFIL_MONIT_VALIDADOR, $perfis) ){
		$campo = "'<div style=\"white-space:nowrap;\">
						<img src=../imagens/alteracao.gif style=cursor:pointer; onclick=\"gerenciarAnaliseGlobalMeta('||m.metid||');\" title=\"An�lise Global da Meta\">'
						|| (case when count(distinct mi.indid ) > 0 then '<img src=../imagens/migra.gif style=cursor:pointer; onclick=\"visualizaIndicadoresMeta('||m.metid||');\" title=\"Metas dos Indicadores\">' else '' end) || '
				</div>' as acao,";			
	}else{
		$campo = "'' as acao,";
	}		
	
	$sql = "SELECT {$campo} metdsc, perdsc FROM monitora.meta m 
			INNER JOIN painel.periodicidade p ON p.perid = m.perid
			INNER JOIN monitora.objetivometa o ON o.metid = m.metid
			left join monitora.metaindicador mi ON mi.metid = m.metid 
			WHERE o.objid='".$dados['objid']."'
			group by metdsc,perdsc,m.metid";

	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','95%','center');
}

function ver_indicadores($dados) {
	global $db;
	
	?>
	<script>
	function gerenciarIndicadores(prgcod,idpid) {
		window.open('monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarIndicadores&atualizartodapaginapai=true&prgcod='+prgcod+'&idpid='+idpid,'Indicadores','scrollbars=no,height=400,width=600,status=no,toolbar=no,menubar=no,location=no');
	}
	</script>
	<?
	
	$cabecalho = array("<b>Indicador</b>","<b>Unidade Medida</b>","<b>Periodicidade</b>");
	
	$perfis = pegaPerfilGeral();
	
	if(in_array(PFL_UNIDADE_MONITORAMENTO_AVALIACAO,$perfis) || $db->testa_superuser()) {
		$campo = "'<img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"gerenciarIndicadores(\'".$dados['prgcod']."\',\''||p.idpid||'\');\">' as acao,";
		$cabecalho = array("&nbsp;","<b>Indicador</b>","<b>Unidade Medida</b>","<b>Periodicidade</b>");
	}
	
	$sql = "SELECT {$campo} idpdsc, umidsc, perdsc from monitora.indicadorprograma p  
			INNER JOIN monitora.unidademedidaindicador u ON u.umiid = p.umiid 
			INNER JOIN painel.periodicidade e ON e.perid = p.perid
			WHERE p.prgcod='".$dados['prgcod']."' AND idpstatus='A' AND prgano='".$_SESSION['exercicio']."'";
	
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','95%','center');
	
}


function ver_programa($dados) {
	
	global $db;
	
	global $prgobs, $prgdscestrategia, $prgvlrprograma, $prganotermino, $prgmestermino, $prganoinicio, $prgmesinicio, $prgsntemporario, $tprnome, $prgsnestruturante, $prgsnestruturante, $mobdsc, $prgdscobjetivoassoc, $prgdscjustificativa, $prgdscpublicoalvo, $prgdscobjetivo, $prgdscproblema, $prgcod, $orgdsc, $unidsc, $prgdsc;
	
	$sql = "SELECT * from monitora.programa p WHERE p.prgcod='".$dados['prgcod']."' AND p.prgano='".$_SESSION['exercicio']."'";
	$dados_ = $db->pegaLinha($sql);
	
	if($dados_) :
	
	extract($dados_);
	
	// encontrar o desafio (macro-objetivo)
   $sql= "select mobdsc from macroobjetivo where mobcod='".$dados_['mobcod']."'";
   $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar o orgao
    $sql= "select orgdsc from orgao where orgcod='".$dados_['orgcod']."'";
    $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar a unidade
    $sql= "select unidsc from unidade where unicod='".$dados_['unicod']."'";
    $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar o tipo de programa
    $sql= "select tprnome from tipoprograma where tprcod='".$dados_['tprcod']."'";
    $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	
	?>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='right' class="subtitulodireita" width="20%">C�digo:</td>
        <td><?=campo_texto('prgcod','N','N','',8,100,'','');?></td>
      </tr>

      <tr>
        <td align='right' class="SubTituloDireita">Org�o respons�vel:</td>
        <td><?=campo_texto('orgdsc','N','N','',100,100,'','');?></td>
      </tr>
      <? if ($unidsc) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Unidade respons�vel:</td>
         <td><?=campo_texto('unidsc','N','N','',100,100,'','');?></td>
      </tr>
      <?}?>
      <tr>
        <td align='right' class="SubTituloDireita">T�tulo:</td>
        <td><?=campo_texto('prgdsc','N','N','',100,100,'','');?></td>
      </tr>
      <? if ($prgdscproblema) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Problema:</td>
        <td><?=campo_textarea('prgdscproblema','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
      <? if ($prgdscobjetivo) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Objetivo do Programa:</td>
        <td><?=campo_textarea('prgdscobjetivo','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
      <? if ($prgdscpublicoalvo) {?>
      <tr>
        <td align='right' class="SubTituloDireita">P�blico-Alvo:</td>
        <td><?=campo_textarea('prgdscpublicoalvo','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
      <? if ($prgdscjustificativa) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Justificativa:</td>
        <td><?=campo_textarea('prgdscjustificativa','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
      <? if ($prgdscobjetivoassoc) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Objetivo setorial associado:</td>
        <td><?=campo_textarea('prgdscobjetivoassoc','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
      <? if ($mobdsc) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Desafio associado:</td>
        <td><?=campo_textarea('mobdsc','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
      <? if ($prgsnestruturante) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Programa estruturante?</td>
        <td>
	<?
	   if ($prgsnestruturante =='t') {print '<input type="radio" value="1" name="prgsnestruturante" checked>Sim &nbsp;&nbsp; <input type="radio" name="prgsnestruturante" value="0" checked>N�o';}
	   else {print '<input type="radio" value="0" name="prgsnestruturante"> Sim &nbsp;&nbsp; <input type="radio" name="prgsnestruturante" value="1"  checked>N�o';}
      ?>
      </tr>
      <?}?>
      <tr>
        <td align='right' class="SubTituloDireita">Tipo:</td>
        <td><?=campo_texto('tprnome','N','N','',30,30,'','');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Horizonte temporal:</td>
        <td>
	<?
	   if ($prgsntemporario=='t')
	   {
	      print '<input type="radio" value="0" name="prgsntemporario"> Cont�nuo &nbsp;&nbsp; <input type="radio" name="prgsntemporario" value="1" checked>Tempor�rio';
	   }
	   else
	   {
	      print '<input type="radio" value="1" name="prgsntemporario" checked> Cont�nuo &nbsp;&nbsp; <input type="radio" name="prgsntemporario" value="0" >Tempor�rio';
	   }
	?>
      </tr>
	<?
	   if ($prgsntemporario=='t')
	   {?>
      <tr>
        <td align='right' class="SubTituloDireita">Data In�cio/T�rmino:</td>
        <td><?=campo_texto('prgmesinicio','N','N','',6,2,'','');?> /
	        <?=campo_texto('prganoinicio','N','N','',6,4,'','');?>
	<? if ($prgsntemporario) {
	?>
	  <br>(Se tempor�rio) <?=campo_texto('prgmestermino','N','N','',6,2,'','');?> /
      <?=campo_texto('prganotermino','N','N','',6,4,'','');?>
	  <?}?>
	  </td>
      </tr>
      <?}?>
	<? if ($prgsntemporario=='t') {	?>
       <tr>
        <td align='right' class="SubTituloDireita">Valor Global Estimado <br>do Programa (R$) (se tempor�rio)</td>
        <td>
            <?=campo_texto('prgvlrprograma','N','N','',10,10,'','');?>
	  </td>
      </tr>
	  <?}?>
	<? if ($prgdscestrategia) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Estrat�gia de implementa��o:</td>
        <td><?=campo_textarea('prgdscestrategia','N','N','',100,5,'');?></td>
      </tr>
	  <?}?>
	<?	  if ($prgobs) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Observa��o:</td>
        <td><?=campo_textarea('prgobs','N','N','',100,5,'');?></td>
      </tr>
	  <?}?>
    </table> 
	<?
	else :
	?>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td class="SubTituloEsquerda">N�o foi encontrado o programa com c�digo <b><?=$dados['prgcod'] ?></b> no ano <b><?=$_SESSION['exercicio'] ?></b></td>
      </tr>
	</table>
	<?
	endif;
	
}

function testaAvaliadorObjetivo( $objid ){
	
	global $db;
	
	if( possui_perfil2( Array(PERFIL_MONIT_AVALIADOR_OBJETIVO) ) ){
		
		$sql = "SELECT
					true
				FROM
					monitora.usuarioresponsabilidade
				WHERE
					objid = $objid AND
					rpustatus = 'A' AND
					usucpf = '{$_SESSION['usucpf']}'";
		
		$teste = $db->pegaUm($sql);
		
		return ($teste==''?false:true);
	}else{
		return false;
	}
}

function ver_objetivo( $dados ) {
	global $db;
	
	$sql = "SELECT * from monitora.objetivo WHERE objid='".$dados['objid']."' AND objstatus='A'";
	$dados_ = $db->pegaLinha($sql);
	
	$disabledEdita 			= 'disabled="disabled"';
	$disabledSituacional 	= 'disabled="disabled"';
	$disabledGlobal 		= 'disabled="disabled"';
	
	if( possui_perfil2( Array(PERFIL_MONIT_CPMO) ) ){
		$disabledEdita = "onclick=\"gerenciarObjetivos('{$dados['prgcod']}','{$dados['objid']}')\"";
	}
	if( ( testaAvaliadorObjetivo( $dados['objid'] ) && possui_perfil2( Array(PERFIL_MONIT_AVALIADOR_OBJETIVO) ) ) 
		|| $db->testa_superuser()
		|| possui_perfil2( Array( PERFIL_MONIT_CPMO ) ) ){
		$disabledSituacional = "onclick=\"gerenciarAnaliseObjetivo('{$dados['prgcod']}','{$dados['objid']}')\"";
	}
	if( ( 
			( testaAvaliadorObjetivo( $dados['objid'] ) && possui_perfil2( Array(PERFIL_MONIT_AVALIADOR_OBJETIVO) ) ) ||
		  	possui_perfil2( Array( PERFIL_MONIT_COORDENADOR_ACAO, PERFIL_MONIT_VALIDADOR, PERFIL_MONIT_CPMO ) )
	   	) || $db->testa_superuser() ){
		$disabledGlobal = "onclick=\"gerenciarAnaliseGlobalObjetivo('{$dados['prgcod']}','{$dados['objid']}')\"";
	}
	
	?>
	<script>
	function gerenciarObjetivos(prgcod,objid) {
		window.open('monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarObjetivos&atualizartodapaginapai=true&prgcod='+prgcod+'&objid='+objid,'Objetivos','scrollbars=no,height=400,width=600,status=no,toolbar=no,menubar=no,location=no');
	}
	function gerenciarAnaliseObjetivo(prgcod,objid) {
//		var j = window.open('monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseSituacionalObjetivo&prgcod='+prgcod+'&objid='+objid,'AnaliseObjetivos','scrollbars=no,height=500,width=700,status=no,toolbar=no,menubar=no,location=no');
		var j = window.open('monitora.php?modulo=principal/telaprograma&acao=A&requisicao=listarAnaliseSituacionalObjetivo&prgcod='+prgcod+'&objid='+objid,'AnaliseObjetivos','scrollbars=yes,height=500,width=700,status=no,toolbar=no,menubar=no,location=no');
		j.focus();
	}
	function gerenciarAnaliseGlobalObjetivo(prgcod,objid) {
		var j = window.open('monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseGlobalObjetivo&prgcod='+prgcod+'&objid='+objid,'An�lise Global de Objetivo','scrollbars=yes,height=500,width=700,status=no,toolbar=no,menubar=no,location=no');
		j.focus();
	}
	</script>
	<table class="listagem" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">Resumo do Objetivo</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"><b>C�digo :</b></td>
			<td><?=$dados_['objcod'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"><b>Objetivo :</b></td>
			<td><?=$dados_['objdsc'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"><b>Caracteriza��o :</b></td>
			<td><?=$dados_['objcaracterizacao'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2">
				<input type="button" name="editar" 	value="Editar Objetivo" 	<?=$disabledEdita ?>>
				<input type="button" name="analise" value="An�lise Situacional" <?=$disabledSituacional ?>>
				<input type="button" name="analise" value="An�lise Global" 		<?=$disabledGlobal ?>>
			</td>
		</tr>
	</table>
	<?
	
}

function ver_cadastroanexos($dados) {
	global $db;
	?>
	<script>
		
	function gravarAnexoPrograma() {
		if(document.getElementById('arquivo').value=='') {
			alert('Escolha o anexo');
			return false;
		}
		
		if(document.getElementById('taaid').value=='') {
			alert('Selecione um tipo de anexo');
			return false;
		}

		jQuery('#formarquivo').submit();
	}
	
	function donwload(arqid) {
		window.location='monitora.php?modulo=principal/detalhesppa&acao=A&arqid='+arqid;
	}
	
	function excluirAnexoPrograma(dcpid) {
		var conf = confirm('Deseja realmente excluir o arquivo?');
		if(conf) {
			window.location='monitora.php?modulo=principal/detalhesppa&acao=P&aba=cadastroanexos&prgcod=<?=$dados['prgcod'] ?>&requisicao=excluirAnexoPrograma&dcpid='+dcpid;
		}
	}
		
	</script>
	<form name="formarquivo" id="formarquivo" action="" enctype="multipart/form-data" method="post">
	<input type="hidden" name="requisicao" value="gravarAnexoPrograma">
	<table class="listagem" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">Cadastro de Anexo do Programa</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"><b>Arquivo :</b></td>
			<td><input type="file" name="arquivo" id="arquivo"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"><b>Tipo :</b></td>
			<td><?
			$sql = "SELECT taaid as codigo, taadescricao as descricao FROM pde.tipoanexoatividade WHERE taalegal = true ORDER BY taadescricao";
			$db->monta_combo('taaid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'taaid');
			?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"><b>Descri��o :</b></td>
			<td><? echo campo_textarea( 'arqdescricao', 'S', 'S', '', '70', '4', '255'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2"><input type="button" name="gravar" value="Gravar" onclick="gravarAnexoPrograma();"></td>
		</tr>
	</table>
	</form>
	<?
	
	$sql = "select '<center><img src=../imagens/anexo.gif style=cursor:pointer; onclick=\"donwload('||a.arqid||');\"> <img src=../imagens/excluir.gif style=cursor:pointer onclick=\"excluirAnexoPrograma('||p.dcpid||');\"></center>' as acao, a.arqnome || '.' || a.arqextensao as arquivo, u.usunome as nome from monitora.documentoprograma p
	 		inner join public.arquivo a on a.arqid = p.arqid 
	 		inner join seguranca.usuario u on u.usucpf = p.usucpf
			where prgid in(select prgid from monitora.programa where prgcod='".$dados['prgcod']."' and prgano='".$_SESSION['exercicio']."')";
	$cabecalho = array("&nbsp;","<b>Arquivo</b>","<b>Inserido por</b>");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','95%','center');
	
	
}

function gravarAnexoPrograma($dados) {
	global $db;
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	$campos = array("prgid" => $db->pegaUm("select prgid from monitora.programa where prgcod='".$dados['prgcod']."' and prgano='".$_SESSION['exercicio']."'"),"taaid" => $dados['taaid'],"usucpf" => $_SESSION['usucpf']);
	
	$file = new FilesSimec("documentoprograma", $campos, "monitora");
	
	$arqdescricao = $dados['arqdescricao'];
	
	if( $file->setUpload($arqdescricao, "arquivo", true ) ) {

		echo "<script>alert('Gravado com sucesso');window.location='monitora.php?modulo=principal/detalhesppa&acao=P&aba=cadastroanexos&prgcod=".$dados['prgcod']."';</script>";
		
	} else {
		
		echo "<script>alert('Problemas para gravar o arquivo');window.location='monitora.php?modulo=principal/detalhesppa&acao=P&aba=cadastroanexos&prgcod=".$dados['prgcod']."';</script>";
		
	}
	
}

function excluirAnexoPrograma($dados) {
	global $db;
	$sql = "delete from monitora.documentoprograma where dcpid='".$dados['dcpid']."'";
	$db->executar($sql);
	$db->commit();
	echo "<script>alert('Exclu�do com sucesso');window.location='monitora.php?modulo=principal/detalhesppa&acao=P&aba=cadastroanexos&prgcod=".$dados['prgcod']."';</script>";
	
}


if($_REQUEST['arqid']) {
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec("documentoprograma", array(), "monitora");
	$file->getDownloadArquivo($_REQUEST['arqid']);
}


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ."includes/cabecalho.inc";
echo "<br/>";


if($_SESSION['exercicio']<"2012") die("<script>window.location='monitora.php?modulo=inicio&acao=C';</script>");

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
jQuery.noConflict();
</script>
<?

switch($_REQUEST['acao']) {
	
	case 'P':
		monta_titulo( "Detalhes - Programas", "");
		$menu = array(0 => array("id" => 1, "descricao" => "Todos os Programas", "link" => "/monitora/monitora.php?modulo=inicio2&acao=C"),
					  1 => array("id" => 2, "descricao" => "Programa",    		 "link" => "/monitora/monitora.php?modulo=principal/detalhesppa&acao=P&aba=programa&prgcod=".$_REQUEST['prgcod']),
					  2 => array("id" => 3, "descricao" => "Indicadores", 		 "link" => "/monitora/monitora.php?modulo=principal/detalhesppa&acao=P&aba=indicadores&prgcod=".$_REQUEST['prgcod']),
					  3 => array("id" => 4, "descricao" => "Cadastro de anexos", "link" => "/monitora/monitora.php?modulo=principal/detalhesppa&acao=P&aba=cadastroanexos&prgcod=".$_REQUEST['prgcod'])
				  	  );
				  	  
		if(!$_REQUEST['aba']) $_REQUEST['aba']="programa";
		break;
	case 'O':
		monta_titulo( "Detalhes - Objetivo", "");
		$menu = array(0 => array("id" => 1, "descricao" => "Todos os Programas", "link" => "/monitora/monitora.php?modulo=inicio2&acao=C"),
					  1 => array("id" => 2, "descricao" => "Programa",    		 "link" => "/monitora/monitora.php?modulo=principal/detalhesppa&acao=O&aba=programa&prgcod=".$_REQUEST['prgcod']."&objid=".$_REQUEST['objid']),
					  2 => array("id" => 3, "descricao" => "Indicadores", 		 "link" => "/monitora/monitora.php?modulo=principal/detalhesppa&acao=O&aba=indicadores&prgcod=".$_REQUEST['prgcod']."&objid=".$_REQUEST['objid']),
					  3 => array("id" => 4, "descricao" => "Objetivo", 		 	 "link" => "/monitora/monitora.php?modulo=principal/detalhesppa&acao=O&aba=objetivo&prgcod=".$_REQUEST['prgcod']."&objid=".$_REQUEST['objid']),
					  4 => array("id" => 5, "descricao" => "Metas", 		 	 "link" => "/monitora/monitora.php?modulo=principal/detalhesppa&acao=O&aba=metas&prgcod=".$_REQUEST['prgcod']."&objid=".$_REQUEST['objid']),
				  	  );
				  	  
		if(!$_REQUEST['aba']) $_REQUEST['aba']="objetivo";
		break;
		
	case 'A':
		monta_titulo( "Detalhes - A��es", "");
		$menu = array(0 => array("id" => 1, "descricao" => "Todos os Programas", "link" => "/monitora/monitora.php?modulo=inicio2&acao=C"),
					  1 => array("id" => 2, "descricao" => "A��o",    		 	 "link" => "/monitora/monitora.php?modulo=principal/detalhesppa&acao=A&aba=acao&codigo=".$_REQUEST['codigo']),
// 					  2 => array("id" => 3, "descricao" => "Iniciativa", 		 "link" => "/monitora/monitora.php?modulo=principal/detalhesppa&acao=A&aba=iniciativa&codigo=".$_REQUEST['codigo']),
					  2 => array("id" => 3, "descricao" => "Monitorar A��o", 	 "link" => "/monitora/monitora.php?modulo=principal/acao/monitoraacao&acao=A"),
					  3 => array("id" => 4, "descricao" => "Coment�rios", 	 "link" => "/monitora/monitora.php?modulo=principal/acao/comentarioacao&acao=A")
				  	  );
				  	  
		if(!$_REQUEST['aba']) $_REQUEST['aba']="acao";
		break;
}

echo "<br>";

echo montarAbasArray($menu, "/monitora/monitora.php?modulo=principal/detalhesppa&acao=".$_REQUEST['acao']."&aba=".(($_REQUEST['aba'])?$_REQUEST['aba']:"programa").(($_REQUEST['prgcod'])?"&prgcod=".$_REQUEST['prgcod']:"").(($_REQUEST['objid'])?"&objid=".$_REQUEST['objid']:"").(($_REQUEST['codigo'])?"&codigo=".$_REQUEST['codigo']:""));

$_REQUEST['aba']="ver_".$_REQUEST['aba'];
$_REQUEST['aba']($_REQUEST);

?>