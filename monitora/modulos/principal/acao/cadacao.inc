<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   M�dulo:cadacao.inc
   Finalidade: permitir o controle do cadastro de acao
   */

$modulo=$_REQUEST['modulo'] ;//

if (!$_REQUEST['prgid'] && $_REQUEST['acaid']){
	$_SESSION['prgid'] = $db->pegaUm("SELECT prgid FROM monitora.acao WHERE acaid = {$_REQUEST['acaid']}");
	$_REQUEST['prgid'] = $_SESSION['prgid'];
}


if ($_REQUEST['prgid']) 
	$_SESSION['prgid'] = $_REQUEST['prgid'];
else if ($_SESSION['prgid']) 
	$_REQUEST['prgid'] = $_SESSION['prgid'];



if ($_REQUEST['acaid']) $_SESSION['acaid'] = $_REQUEST['acaid'];
else $_REQUEST['acaid'] =$_SESSION['acaid'];

$sql = "select a.*, b.prgdsc from acao a, programa b where a.acastatus='A' and b.prgstatus='A' and a.prgid = b.prgid and a.prgano ='" .$_SESSION['exercicio']."' and a.prgid=".(integer)$_SESSION['prgid'];

if ($_REQUEST['acaid'])
{
    $sql = $sql. "  and a.acaid = ".(integer)$_REQUEST['acaid'];
}

// se houver prgid busca ent�o us�-lo
if ($_REQUEST['prgcodbusca'])
{
  $sql = "select a.*, b.prgdsc from monitora.acao a, monitora.programa b where a.acastatus='A' and b.prgstatus='A' and a.prgid = b.prgid and a.prgano ='" .$_SESSION['exercicio']."' and a.prgcod='".$_REQUEST['prgcodbusca']."'";
}
// se houver acacodbusca busca ent�o us�-lo
if ($_REQUEST['acacodbusca'])
{
  $sql = "select a.*, b.prgdsc from acao a, programa b where a.acastatus='A' and b.prgstatus='A' and a.prgid = b.prgid and a.prgano ='" .$_SESSION['exercicio']."' and a.acacod='".str_to_upper($_REQUEST['acacodbusca'])."'";
}

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);

$nl=$nlinhas;
if ($nlinhas >= 0) {


   // as linhas abaixo n�o permitem navegar para al�m dos limites

// fim do controle de navega��o
    $res =  $db->carrega_registro($RS,$_POST['registro']);
// a linha abaixo transforma em variáveis todos os campos do array
    if(is_array($res)){
       foreach($res as $k=>$v) {${$k}=$v;};
            $_SESSION['prgid'] = $prgid;
            $_SESSION['acaid']= $acaid;
            if ($acasnmetanaocumulativa=='t') $tipometa ='N�o cumulativa';
            if ($acasnmetanaocumulativa=='f') $tipometa ='Cumulativa';
            if ($acasnmetanaocumulativa=='') $tipometa ='N�o informado';
       }
	// encontrar o desafio (macro-objetivo)
        $sql= "select prodsc from produto where procod='".$procod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar descrição da unidade de medida
        $sql= "select unmdsc from public.unidademedida where unmcod='".$unmcod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar descrição do tipo de ação
        $sql= "select tacdsc from public.tipoacao where taccod='".$taccod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar a função
       if ($funcod) {
        $sql= "select fundsc from ppafuncao where funcod='".$funcod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        }
        // encontrar a subfunção
       if ($sfucod) {
        $sql= "select sfudsc from ppasubfuncao where sfucod='".$sfucod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
       }
    // encontrar o órgão responsável
        $sql= "select orgdsc from orgao where orgcod='".$orgcod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

    // encontrar o orgao responsável
        $sql= "select orgdsc from orgao where substr(orgcod,0,3)='".substr($unicod,0,2)."'";

        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        
    // encontrar a unidade responsável
        $sql= "select unicod||'-'||unidsc as unidade from unidade where unicod='".$unicod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    // encontrar o produto
        $sql= "select prodsc from produto where procod='".$procod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    // encontrar a meta
        $sql= "select fisqtdeprevistoano as meta from dadofisico where acacod='$acacod' and saccod='$saccod' and regcod='$regcod' and prgcod='$prgcod' and prgano='".$_SESSION['exercicio']."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    // encontrar a esfera
       if ($esfcod) {
        $sql= "select esfdsc from esfera where esfcod='".$esfcod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        }
 } else
 {
   ?>
<script>
    alert('C�digo da A��o inexistente.! \nVoc� pode estar tentando abrir a��o de outro exerc�cio.');
    history.back();
</script>
   <?
   exit();
 }

 if ($_REQUEST['prgid']) $_SESSION['prgid'] = $_REQUEST['prgid'];
else $_REQUEST['prgid'] =$_SESSION['prgid'];
if ($_REQUEST['acaid']) $_SESSION['acaid'] = $_REQUEST['acaid'];
else $_REQUEST['acaid'] =$_SESSION['acaid'];

include  APPRAIZ."includes/cabecalho.inc";		
?>
<br>
<? 
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Atributo da A��o';
monta_titulo($titulo_modulo,'');
?>
<? // inclus�o do menu de setas de navega��o
$nlinhas=$nl;
?> 
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="orgao" value=0>
<input type=hidden name="registro" value=<?=$_POST['registro'];?>>
<input type=hidden name="navega" value=0>
<input type=hidden name="procura" value=0>
<!--
<tr><td align='right' class="SubTituloDireita">Programa</td>
    <td><?=campo_texto('prgcod','S',$habil,'',8,4,'','');?>
	<img border="0" src="imagens/busca.gif"  alt='Procura programa' title='Procura programa' onclick="ProcuraPrograma(this.form)">
    <?='  '.$prgdsc?> 
    </td></tr>-->
	 <tr>
        <td align='right' class="SubTituloDireita" style="width: 20%">Programa:</td>
        <td><b><?=$prgcod?></b> - <?=$prgdsc?><!--<input type="text" name="acacodbusca" size="5" ><img border="0" src="imagens/busca.gif"  alt='Procura a��o' title='Procura a��o' onclick="ProcuraAcao(this.form)">-->
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">A��o:</td>
        <td><b><?=$acacod?></b>.<?=$unicod.'.'.$loccod?><!--<input type="text" name="acacodbusca" size="5" ><img border="0" src="imagens/busca.gif"  alt='Procura a��o' title='Procura a��o' onclick="ProcuraAcao(this.form)">-->
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">T�tulo:</td>
        <td><?=campo_textarea('acadsc','N','N','',100,3,'');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Org�o Or�ament�rio Respons�vel:</td>
        <td><?=campo_texto('orgdsc','N','N','',100,100,'','');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Unidade Or�ament�ria Respons�vel:</td>
        <td><?=campo_texto('unidade','N','N','',100,100,'','');?></td>
      </tr>
      <?  if ($fundsc) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Fun��o:</td>
        <td><?=campo_texto('fundsc','N','N','',100,100,'','');?></td>
      </tr>
      <?}?>
      <? if ($sfudsc) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Subfun��o:</td>
        <td><?=campo_texto('sfudsc','N','N','',100,100,'','');?></td>
      </tr>
      <?}?>
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
        <td align='right' class="SubTituloDireita">Meta (<b><?=$tipometa?>)</b>:</td>
        <td><?=campo_texto('meta','N','N','',100,100,'','');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Unidade de Medida:</td>
        <td><?=campo_texto('unmdsc','N','N','',100,100,'','');?></td>
      </tr>

      <? if ($acadscproduto) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Especifica��o do Produto:</td>
        <td><?=campo_textarea('acadscproduto','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
  <?
    if ($acasnoutrasfontes =='t') {?>
        <tr>
        <td align='right' class="SubTituloDireita">Quanto ao Or�amento:</td>
        <td><input type="hidden" name="taccod" value='3'>N�o or�ament�ria</td>
      </tr>
   <? } else {?>
        <tr>
           <td align='right' class="SubTituloDireita">Quanto ao Or�amento:</td>
           <td><input type="hidden" name="taccod" value='3'>Or�ament�ria</td>
        </tr>
        <tr>
           <td align='right' class="SubTituloDireita">Tipo de Or�amento:</td>
           <td>
	    <?
	         if ($acasnfiscalseguridade=='t') print 'Fiscal&nbsp;&nbsp;&nbsp;&nbsp;';
  	         if ($acasnfiscalseguridade=='f') print 'Seguridade&nbsp;&nbsp;&nbsp;&nbsp;';
  	         if ($acasninvestatais=='t') print 'Investimentos Estatais&nbsp;&nbsp;&nbsp;&nbsp;';
	    ?>
          </td>
      </tr>
      <tr>
	     <td align='right' class="SubTituloDireita">Tipo de a��o:</td>
         <td><input type="hidden" name="tacdsc" ><?=$tacdsc?></td>
      </tr>
      <?if ($taccod==1){?>
        <tr>
	      <td align='right' class="SubTituloDireita">Evolu��o da Situa��o F�sica:</td>
          <td>
             <table border=1>
                <tr>
                   <td width='33%'>Total</td>
                   <td width='33%'>Realizado at� <?=strval($_SESSION['exercicio'])-2?></td>
                   <td width='33%'>Previsto em <?=$_SESSION['exercicio']-1?></td>
                </tr>
                <tr>
                   <td width='33%'><?=$acaqtdcustototal?></td>
                   <td width='33%'><?=$acaqtdateanoanterior?></td>
                   <td width='33%'><?=$acaqtdprevistoanocorrente?></td>
                </tr>
             </table>
          </td>
         </tr>
        <tr>
	      <td align='right' class="SubTituloDireita">Evolu��o da Situa��o Financeira:</td>
          <td>
             <table border=1>
                <tr>
                   <td width='33%'>Total</td>
                   <td width='33%'>Realizado at� <?=strval($_SESSION['exercicio'])-2?></td>
                   <td width='33%'>Previsto em <?=$_SESSION['exercicio']-1?></td>
                </tr>
                <tr>
                   <td width='33%'><?=number_format($acavlrcustototal,2,',','.')?></td>
                   <td width='33%'><?=number_format($acavlrcustoateanoanterior,2,',','.')?></td>
                   <td width='33%'><?=number_format($acavlrprevistoanocorrente,2,',','.')?></td>
                </tr>
             </table>
          </td>
         </tr>
        <tr>
	      <td align='right' class="SubTituloDireita">Justificativa da Repercuss�o Financeira sobre o Custeio da Uni�o:</td>
          <td>
             <?=campo_textarea('acarepercfinanceira','N','N','',100,5,'');?>
          </td>
         </tr>
        <tr>
	      <td align='right' class="SubTituloDireita">Valor Estimado da Repercuss�o Financeira (R$/ano):</td>
          <td>
             <?=number_format($acavlrrepercfinanceira,2,',','.')?>
          </td>
         </tr>
        <tr>
	      <td align='right' class="SubTituloDireita">Dura��o do Projeto:</td>
          <td>
             <table border=1>
                <tr>
                   <td width='50%'>In�cio</td>
                   <td width='50%'>T�rmino</td>
                </tr>
                <tr>
                   <td width='50%'><?=$acamesinicio.'/'.$acaanoinicio?></td>
                   <td width='50%'><?=$acamestermino.'/'.$acaanotermino?></td>
                </tr>
             </table>
          </td>
         </tr>
      <?}?>
      <? if ($esfcod) {?>
        <tr>
	      <td align='right' class="SubTituloDireita">Esfera:</td>
          <td>
             <?=campo_texto('esfdsc','N','N','',50,50,'','');?>
          </td>
         </tr>
       <?}?>
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
	</td>
      </tr>
     <?}?>
      <? if ($acahistorico) {?>
     <tr>
        <td align='right' class="SubTituloDireita">Hist�rico da a��o:</td>
       <td><?=campo_textarea('acahistorico','N','N','',100,5,'');?></td>
      </tr>
     <?}?>
	<?$db -> mostra_resp($_SESSION['acaid'], 'acaid');?>
<? if ($db->testa_uma()) { ?>
  <tr bgcolor="#cccccc">
      <td></td>
   <td><input type="button" class="botao" name="btassociar" value="Associar Respons�veis" onclick="Associa_resp()"></td>
      </tr>
  <?}?>
</form>



		<?php
		
		/*
		
		create table monitora.acaounidadegestora (
			acaid int4 not null,
			ungcod char(6) not null,
			constraint pk_acaounidadegestora primary key ( acaid, ungcod ),
			constraint fk_acaounidadegestora_acaid foreign key ( acaid )
				references monitora.acao ( acaid ) match simple
				on update cascade
				on delete restrict,
			constraint fk_acaounidadegestora_ungcod foreign key ( ungcod )
				references public.unidadegestora ( ungcod ) match simple
				on update cascade
				on delete restrict
		) with oids;
		
		*/
		
		$sql = sprintf(
			"select count( pflcod )
			from seguranca.perfilusuario pu
			where pu.usucpf = '%s' and  pu.pflcod in ( 18, 112, 6 )",
			$_SESSION['usucpf']
		);
		$privilegio_associacao = $db->pegaUm( $sql ) > 0;
		$ungcod_habil = $privilegio_associacao ? 'S' : 'N';
		
		if ( $privilegio_associacao ) {
            
			# grava a associa��o
			if ( $_REQUEST['unidadegestora'] ) {
				$sql = sprintf( "delete from monitora.acaounidadegestora where acaid = %d", $_SESSION['acaid'] );
				$db->executar( $sql );
				if($_REQUEST['ungcod'][0]) {
				foreach ( (array) $_REQUEST['ungcod'] as $ungcod ) 
                {
                    if(count($_REQUEST['ungcod'])>0)
                    {
                        $sql = sprintf(
                            "insert into monitora.acaounidadegestora ( ungcod, acaid ) values ( '%s', %d )",
                            $ungcod,
                            $_SESSION['acaid']);
                        $db->executar( $sql );
                    }
				}
				$db->commit();
			}
			}
		}
		
		$sql = sprintf(
			"select u.ungcod as codigo, u.ungdsc as descricao
			from monitora.acaounidadegestora aug
			inner join public.unidadegestora u on u.ungcod = aug.ungcod
			where aug.acaid = %d",
			$_SESSION['acaid']
		);
		$ungcod = $db->carregar( $sql );
		
		?>
		<form method="post" name="unidadegestora">
			<input type="hidden" name="unidadegestora" value="1"/>
			<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td align='right' class="SubTituloDireita">Unidade Gestora:</td>
					<td>
						<?php
						$sql = sprintf( "select ungcod as codigo, ungdsc as descricao from public.unidadegestora order by ungdsc" );
						combo_popup( "ungcod", $sql, "Unidade Gestora", "400x400", 0, array(), "", $ungcod_habil, false, false, 5, 400 );
						?>
					</td>
				</tr>
				<?php if( $privilegio_associacao ): ?>
					<tr bgcolor="#cccccc">
						<td style="width: 20%;">&nbsp;</td>
						<td>
							<input type="button" class="botao" name="" value="Gravar" onclick="associar_ug();"/>
						</td>
					</tr>
				<?php endif; ?>
			</table>
  	  </form>
    <script>
    	
	    function selectAllOptions(obj) {
			for (var i=0; i<obj.options.length; i++) {
				obj.options[i].selected = true;
			}
		}
		
    	function associar_ug(){
    		selectAllOptions( document.getElementById( 'ungcod' ) );
			document.unidadegestora.submit();
    	}
    </script>

<script>
var WindowObjectReference; /* Declaring a global variable
which will store a reference to the new window to be created */
	function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }
    function VerRegistro(cod) {
	//alert (cod+1);
        document.formulario.navega.value = cod;
	document.formulario.submit();
    }
    function AvaliaAcao(cod) {
	document.formulario.avalia.value = cod;
	document.formulario.submit();
    }
    function ProcuraAcao(form) {
	document.formulario.procura.value = document.formulario.acacodbusca.value;
	document.formulario.submit();
    }
    function ProcuraPrograma(form) {
	document.formulario.submit();
    }
    function Associa_resp()
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/acao/assocrespac&acao=A";
          WindowObjectReference = window.open(e, "Associa��o_de_Respons�veis",
"menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=600,height=400'");
    }
    

</script>

