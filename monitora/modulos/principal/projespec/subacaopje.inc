<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   M�dulo:subacaopje.inc
   Finalidade: permitir o controle de cadastro de uma suba��o para atender a um projeto especial
   */
$modulo=$_REQUEST['modulo'] ;//

if ($_REQUEST['act'] == 'inserir')
{
	require_once( 'plantrabpje_funcoes.inc' );
	$pjeid=$_SESSION['pjeid'];
	$acaid = $_REQUEST['acaid'];
	
    if (! $_REQUEST['ptodescricao'] ) $_REQUEST['ptodescricao']=null;
    if (! $_REQUEST['ptofinalidade'] ) $_REQUEST['ptofinalidade']=null;
    if (! $_REQUEST['ptodetalhamento'] ) $_REQUEST['ptodetalhamento']=null;
    $_REQUEST['ptoid_pai']='null';

        
	// verifica primeiro se j� n�o existe o registro
	$sql = "select acaid from monitora.pjecomponente where pjeid=$pjeid and acaid=$acaid";
	$existe=$db->pegaUm($sql);
	if (! $existe) 
	{
		// ent�o cria
		//  incluir um registro no plano de trabalho.
		// pega alguns dados do projeto
		$sql = "select to_char(pjedataini,'YYYY/mm/dd') as dataini, to_char(pjedatafim,'YYYY/mm/dd') as datafim from monitora.projetoespecial where pjeid=$pjeid";
		$datas= $db->pegaLinha($sql);
		$dtinipje=$datas['dataini'];
		$dtfimpje=$datas['datafim'];
	   
		// pega os dados da a��o para utilizar no plano de trabalho				   
		 $sql = "select acacod as acacod2 ,acadsc,taccod,acamesinicio,acamestermino,acaanotermino,acaanoinicio  from monitora.acao where acaid=$acaid";
		 $res= $db->pegaLinha($sql);
		 if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
		 $_REQUEST['ptosnsoma']='f';
     	 $_REQUEST['ptosnaprovado']='f';
         $dtinicio= $dtinipje;
     	 $dttermino= $dtfimpje;
     	 if ($taccod==1)
     	 {
     	   	// � uma a��o do tipo projeto
     	   	if ($acamesinicio and $acaanoinicio) $dtinicio=$acaanoinicio.'/'.$acamesinicio.'/01';
     	   	else $dtinicio= $dtinipje;
     	   	if ($acamestermino and $acaanotermino) $dtinicio=$acaanotermino.'/'.$acamestermino.'/28';
     	   	else $dttermino= $dtfimpje;     			   	
     	  }
     	  // aqui se compara as datas
     	  
     	       	   
     	  // if ($dtinipje > $dtinicio) $dtinicio=$dtinipje;
     	 // if ($dtfimpje < $dttermino) $dttermino=$dtfimpje;    
     	   
     	   $dtinicio=$dtinipje;
     	   $dttermino=$dtfimpje;
     	   // foi necess�rio fazer este coment�rio acima porque h� caso de a��es com valores de datas inconsistentes.
     	   
     	   if (! $_REQUEST['ptovalorprevisto']) $_REQUEST['ptovalorprevisto']=0;
     	    			   
		   $_REQUEST['ptosnsubacao']='t';  
		 // busca o valor de or�amento da a��o
			 $sql = "select sum( coalesce( rofautorizado, 0 ) ) as dotacao from financeiro.reporcfin where prgcod = '$prgcod' and acacod = '$acacod2' and unicod = '$unicod' and loccod = '$loccod' and rofano = '".$_SESSION['exercicio']."' "; 
             $valor = $db->pegaUm($sql);

             $sql = "select sum( coalesce( ptovalorprevisto, 0 ) ) as planejado from monitora.planotrabalho where acaid=$acaid "; 
             $planejado = $db->pegaUm($sql);

             $disponivel = $valor - $planejado;
             if ($_REQUEST['ptovalorprevisto'] < $disponivel) $_REQUEST['ptovalorprevisto']=$disponivel;

     	     // achar o mais alto id para o c�digo
			$sql = "select ptoordemprov from monitora.planotrabalho where pjeid=$pjeid and ptonivel = 1 and ptostatus='A' order by ptoordemprov desc limit 1";
			$maiorcod=$db->pegaum($sql);
			$novocod = $maiorcod+ 1;
					
		    // achar o mais alto id para o c�digo s� em a��o
			$sql = "select ptoordemprovacao from monitora.planotrabalho where acaid=$acaid and ptonivel = 1  and ptostatus='A'  order by ptoordemprovacao desc limit 1";
			$maiorcodacao=$db->pegaum($sql);
			$novocodacao = $maiorcodacao+ 1;					

     		// achar a ordem na a��o
			$sql = " select ptoordemacao from monitora.planotrabalho where acaid=$acaid order by ptoordemacao desc limit 1";
			$_REQUEST['ptoordemacao']=$db->pegaum($sql);
			if (! $_REQUEST['ptoordemacao'])$_REQUEST['ptoordemacao']=1;
     		// achar a ordem no projeto
			$sql = " select ptoordem from monitora.planotrabalho where pjeid=$pjeid order by ptoordem desc limit 1";
			$_REQUEST['ptoordem']=$db->pegaum($sql);
				
			$sql = "insert into monitora.planotrabalho (
  						  acaid,
						  ptodsc ,
						  ptoprevistoexercicio ,
						  unmcod ,
						  ptosnaprovado,
						  ptodata_ini,
						  ptodata_fim ,
						  ptoordem ,
						  ptodetalhamento,
						  ptodescricao,
						  ptofinalidade ,
						  ptocod ,
						  pjeid ,
						  ptosnsubacao ,
						  ptoordemacao,
						  ptoorigemppa ,
						  ptonivel,
						  ptoordemprov ,
						  procod,
						  ptovalorprevisto, 
						  ptoordemprovacao ) 
						values (".
    				    $acaid.",'".
						$_REQUEST['ptodsc']."',".
						$_REQUEST['ptoprevistoexercicio'].",".    
						$_REQUEST['unmcod'].",'".
						'f'."','".
						$dtinicio."','".
						$dttermino."',".
						$_REQUEST['ptoordem'].",'".
						$_REQUEST['ptodetalhamento']."','".
						$_REQUEST['ptodescricao']."','".
						$_REQUEST['ptofinalidade']."','".
						$novocod."',".
						$pjeid.",'".
						't'."',".
						$_REQUEST['ptoordemacao'].",'".
						't'."',".
						'1'.",".
						$novocod.",".
						$_REQUEST['procod'].",".
						$_REQUEST['ptovalorprevisto'].",".
						$novocodacao.")";

   						$saida = $db->executar($sql); 
                    	$sql =  "Select ptoid from monitora.planotrabalho where oid = ".pg_last_oid($saida);
						$ultimo = $db->pegaUm($sql);
 						
   						$sql_insercao = "insert into monitora.pjecomponente (ptoid, pjeid,acaid ) values ( " . $ultimo . "," . $pjeid . ",$acaid)";	
						$db->executar( $sql_insercao );
   						 $db->commit();
   						// envia e-mail ao coordenador avisando da cria��o da suba��o com c�pia para o gerente do projeto especial
					   // descobre quem � o coordenador
					   $sql = "select a.prgcod,a.acacod,a.unicod,a.loccod,a.acadsc from monitora.acao a where acaid = ".$acaid;
					   $res=$db->pegalinha($sql);
					   if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
					
					   $sql = "select u.usunome as nomecriador,u.ususexo as sexocriador,u.usuemail as emailcriador,u.usufoneddd ||'-'||u.usufonenum as fonecriador from seguranca.usuario u where u.usucpf='".$_SESSION['usucpf']."'";
					   $res=$db->pegalinha($sql);
					   if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;   
					   
					   $sql = "select u.usunome,u.ususexo,u.usuemail from seguranca.usuario u inner join monitora.usuarioresponsabilidade ur on u.usucpf=ur.usucpf and ur.acaid=".$acaid." and ur.pflcod=1 and ur.rpustatus='A' ";
					   $res=$db->pegalinha($sql);
					   if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
					   $assunto = 'Inclus�o de Suba��o para apoiar Projeto Especial.';
					   $sexo = 'Prezado Sr.  ';
					   if ($ususexo == 'F') $sexo = 'Prezada Sra. ';
					   $mensagem = $sexo. str_to_upper($usunome).chr(13)."Reportamos que foi criada uma Suba��o na a��o $prgcod - $acacod2.$unicod.$loccod - $acadsc, que est� sob sua responsabilidade como Coordenador(a). Para que esta Suba��o possa ser utilizada no Projeto Especial, V.Sa. dever�, no cadastro do Plano de Trabalho da a��o, aprovar a sua cria��o.<br> Esta Suba��o foi criada por:$nomecriador <br>E-Mail:$emailcriador<br>Telefone:$fonecriador";
					   $emailpara = $usuemail.','.$emailcriador;
					   $emailpara = $_SESSION['email_sistema'];
					   email($usunome, $emailpara, $assunto, $mensagem);// o email vai com c�pia para quem criou
   						
				   
				}
			
      
     if ($saida)
        {
        ?>
          <script>
                alert ('Opera��o realizada com sucesso!');
                window.opener.location.reload();
        		self.close();
        		window.close();
          </script>
        <?
       // $db->close();
       }

   	//novo_ajusta_ordem();
   //$db->sucesso($modulo);
}


	$ptocod = $_REQUEST['ptocod'];
	$ptodsc =$_REQUEST['ptodsc'];
	$unmcod = $_REQUEST['unmcod'];
    $ptoprevistoexercicio = $_REQUEST['ptoprevistoexercicio'];
    $ptosnsoma = $_REQUEST['ptosnsoma'];
    $ptosnpercent = $_REQUEST['ptosnpercent'];
  //  $ptoanofim = $_REQUEST['ptoanofim'];    

$titulo_modulo='Criar uma Suba��o para atender a um Projeto Especial';

monta_titulo($titulo_modulo,'');
?>
<div align="center">
<center>

<?
$sql="select pe.pjecod, pe.pjedsc,to_char(pe.pjedataini,'dd/mm/YYYY') as pjedataini,to_char(pe.pjedatafim,'dd/mm/YYYY') as pjedatafim,p.prodsc,pe.pjeprevistoano,pe.pjevlrano from monitora.projetoespecial pe inner join produto p on p.procod=pe.procod where pjeid=".md5_decrypt($_REQUEST['cod'],'');
$res=$db->pegalinha($sql);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;


  // verifica se � Gerente de Projeto Especial
  $autoriza = false;
  $coordaca=false;
  $digit=false;
  if ($db->testa_responsavel_projespec()) $coordaca = true;
  if ($db->testa_superuser())   $coordaca = true;

// verifica se o exerc�cio est� aberto para estruturar o simec
  $sql= "select prsano from monitora.programacaoexercicio where prsano='".$_SESSION['exercicio']."' and prsstatus='A'";

  $registro=$db->pegaUm($sql);

  if ($registro) $autoriza = true;
  if (! $coordaca ) {
    ?>
       <script>
          alert ('Voc� n�o tem acesso para Criar Suba��o!');
          history.back();
       </script>
    <?
    exit();
  }
?>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>Atribuir A��es</title>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
</head>

<form method="POST"  name="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name="ptoid" value="<?=$ptoid?>">
<input type='hidden' name='exclui' value="0">
<input type='hidden' name='acao' value="<?=$_REQUEST['acao']?>">
<input type='hidden' name='act'>
<input type='hidden' name='act2'>
<input type='hidden' name='ptotipo' value='<?=$_REQUEST['ptotipo']?>'>
<input type='hidden' name='dtini' value="<?=$pjedataini?>">
<input type='hidden' name='dtfim' value="<?=$pjedatafim?>">
<input type='hidden' name='cod' value="<?=$_REQUEST['cod']?>">


<center>
<?
	$sql_acoes = "select distinct acaid as codigo, acacod||' - '||unicod||' - '||loccod||' - '||acadsc as descricao from monitora.acao where prgano='".$_SESSION["exercicio"]."' and acasnrap='f'  and acaid in (select acaid from monitora.usuarioresponsabilidade where pflcod=1 and acaid is not null ) and acaid not in (select acaid from monitora.planotrabalho where ptosntodaacao='t' )";


?>
<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;">
  <tr>
    <td align='right' class="SubTituloDireita">Projeto :</td>
    <td><b><?=$pjecod?>&nbsp;-&nbsp;<?=$pjedsc?></b></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Per�odo :</td>
    <td><b><?=$pjedataini?>&nbsp; - &nbsp;<?=$pjedatafim?></b></td>
  </tr>  
  <tr>
    <td align='right' class="SubTituloDireita">Produto :</td>
    <td><b><?=$prodsc?></b></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita" >A��o (clique na imagem):</td>
    <td>
    <? hidden_popup( 'acaid', $sql_acoes, 'Acoes', '450x600','ATEN��O! S� ser�o listadas as a��es com Coordenador e que ainda tenham saldo a comprometer!' ); ?>
    </td>
  </tr>
<?if ($_REQUEST['acaid']) {
   $sql = "select distinct pfl.pfldsc as descricao,pfl.pflsncumulativo as mostra, usu.usunome || ' ('|| org.orgdsc ||')' as usuario from perfil pfl left join usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.acaid = ".$_REQUEST['acaid']." and rpu.rpustatus='A' left join usuario usu on usu.usucpf=rpu.usucpf left join orgao org on org.orgcod = usu.orgcod where  pfl.pflstatus='A' and pfl.pflresponsabilidade in ('A','X','D') order by pfl.pfldsc";


$rs = @$db->carregar( $sql );
if (  $rs && count($rs) > 0 )
	{
	 $i=0;
	 foreach ( $rs as $linha )
		{
		 foreach($linha as $k=>$v) ${$k}=$v;
            if ((!$mostra or $mostra=='f') and $usuario ){
	        $linha = "<tr><td align='right' class='SubTituloDireita'>".$descricao.":</td><td>".$usuario."</td></tr>";
            print $linha;
            }		 
		 $i++;
		}
	}   
}   
?>
 <tr>
        <td align='right' class="SubTituloDireita" colspan="2" style="height:1px;padding:0px;"></td>
      </tr>

      <tr>
        <td align='right' class="SubTituloDireita">T�tulo:</td>
	<td>
	<?=campo_texto('ptodsc','S','S','',80,80,'','','','Entre com o t�tulo da suba��o, ou seja, como ela ser� conhecida.');?>
	</td>
      </tr>

      <tr bgcolor="#F2F2F2">
        <td align='right' class="SubTituloDireita">Unidade de Medida:</td>
        <td >
	<?
	  $sql = "select unmcod as CODIGO,unmdsc as DESCRICAO from unidademedida where unmstatus='A' order by unmdsc ";
	  $db->monta_combo("unmcod",$sql,'S',"Selecione a Unidade de Medida",'','','Escolha uma das unidades de medida. Campo N�O obrigat�rio',400);
	?>
	</td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Produto:</td>
	    <td><?

	    	  $sql = "select procod as CODIGO,prodsc as DESCRICAO from produto where prostatus='A' order by prodsc ";
	  $db->monta_combo("procod",$sql,'S',"Selecione o Produto",'','','Escolha um dos produtos dispon�veis. Campo N�O obrigat�rio!',400);
	    ?></td>
      </tr>

      <tr>
        <td align='right' class="SubTituloDireita">Meta:</td>
	    <td><?=campo_texto('ptoprevistoexercicio','S','S','',16,14,'###########','','','Entre com a meta que se pretende alcan�ar. N�o esque�a que a meta deve estar associada ao produto');?></td>
      </tr>
      
      <tr>
        <td align='right' class="SubTituloDireita">Valor previsto (R$):</td>
	    <td><?=campo_texto('ptovalorprevisto','N','S','',16,14,'#########.##','','','Entre com o or�amento que dever� ser comprometido na a��o.');?></td>
      </tr>      

      <tr>
        <td align='right' class="SubTituloDireita">Finalidade:</td>
	    <td><?=campo_textarea('ptofinalidade','N','S','',80,5,'','','','Expressa o objetivo a ser alcan�ado pela suba��o, ou seja, por que esta suba��o � desenvolvida. Deve sempre iniciar com verbo no infinitivo. Por exemplo, para o t�tulo -Atendimento � Popula��o com Medicamentos para Tratamento dos Portadores da S�ndrome da Imunodefici�ncia Adquirida - Aids e das Doen�as Sexualmente Transmiss�veis - DST- a finalidade poderia ser -Proporcionar � popula��o acesso aos medicamentos para tratamento dos portadores da S�ndrome da Imunodefici�ncia Adquirida - AIDS e das Doen�as Sexualmente Transmiss�veis - DST, visando � recupera��o da sa�de e � interrup��o do ciclo da doen�a.');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Descri��o:</td>
	    <td><?=campo_textarea('ptodescricao','N','S','',80,5,'','','','Expressa, de forma sucinta, o que � efetivamente feito no �mbito da suba��o, seu escopo e delimita��es. Por exemplo, para o t�tulo -Atendimento � Popula��o com Medicamentos para Tratamento dos Portadores da S�ndrome da Imunodefici�ncia Adquirida - Aids e das Doen�as Sexualmente Transmiss�veis - DST- poderia ter-se a descri��o -Aquisi��o, acondicionamento, conserva��o, transporte e distribui��o de medicamentos para o tratamento ambulatorial e domiciliar dos casos positivos da doen�a, al�m de: manuten��o das empilhadeiras hidr�ulicas; servi�os de operador das empilhadeiras: loca��o de c�maras frigor�ficas; transporte de cargas e encomendas; despesas com o despachante aduaneiro, bem como as despesas com o agente financeiro-.');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Detalhamento da Forma de Implementa��o:</td>
	    <td><?=campo_textarea('ptodetalhamento','N','S','',80,5,'','','','Indica a forma de implementa��o da suba��o, descrevendo todas as etapas do processo at� a entrega do produto, inclusive as desenvolvidas por parceiros. Deve tamb�m ser classificada segundo os conceitos abaixo:<br>l.1. Direta: suba��o executada diretamente ou sob contrata��o pela unidade respons�vel, sem que ocorra transfer�ncia de recursos para outros entes da federa��o (Estados, Distrito Federal e Munic�pios). <br>l.2. Descentralizada: Atividades ou projetos, na �rea de compet�ncia da Uni�o, executados por outro ente da federa��o (Estado, Munic�pio ou Distrito Federal), com recursos repassados pela Uni�o. <br>l.3. Transfer�ncia, que se subdivide em:<br>Obrigat�ria: Opera��o especial que transfere recursos, por determina��o constitucional ou legal aos Estados, Distrito Federal e Munic�pios. <br>Volunt�ria: Opera��o especial em que ocorre a entrega de recursos correntes ou de capital a outro ente da Federa��o, a t�tulo de coopera��o, aux�lio ou assist�ncia financeira, que n�o decorra de determina��o constitucional ou legal ou se destine ao Sistema �nico de Sa�de. Estas transfer�ncias destinam-se a execu��o de a��es em �reas de compet�ncia dos Estados, Distrito Federal e Munic�pios. <br>Outras: Opera��o especial que transfere recursos a entidades privadas sem fins lucrativos, organiza��es n�o-governamentais e outras institui��es. <br>l.4. Linha de cr�dito: Suba��o realizada mediante empr�stimo de recursos aos benefici�rios da a��o.');?></td>
      </tr>


<tr bgcolor="#CCCCCC">
   <td></td>
   <td><input type="button" name="btinserir" value="Incluir" onclick="validar_cadastro('I')" class="botao"><input type="button" name="btvoltar" value="Cancelar" onclick="window.close();" class="botao"></td>

 </tr>
 </table>
    </center>
  </div>
</form>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script> 
<script>
  function altera_pto(cod) {
    document.formulario.ptoid.value = cod;
    document.formulario.submit();
  }
   function cancela() {
    document.formulario.ptoid.value = '';
    document.formulario.submit();
  }
  
  function excluir_pto(cod,dsc) {
  
    if( window.confirm( "Confirma a exclus�o da Atividade "+dsc+" ?") )
    {
	document.formulario.exclui.value = cod;
	document.formulario.submit();
    } else document.formulario.exclui.value = 0;
    
  }
  
  function aprova_ativ(cod) {
    document.formulario.ptoid.value = cod;
	document.formulario.act.value = 'aprov';
	document.formulario.submit();

  }

  function aprova_retorno(cod) {
    document.formulario.ptoid.value = cod;
	document.formulario.act.value = 'retorno';
	document.formulario.submit();

  }
  
  function insere_pt(cod) {
   	document.formulario.act2.value = cod;
   	document.formulario.ptotipo.value = cod;
	document.formulario.submit();

  }

function validar_cadastro(cod) 
{
	if (! document.formulario.acaid.value)
	{
		alert ('Escolha a a��o onde a suba��o ser� criada!');
		return;
	}

	if (!validaBranco(document.formulario.ptodsc, 'T�tulo')) return;

   if (!validaBranco(document.formulario.unmcod, 'Unidade de Medida!')) return;
	if (!validaBranco(document.formulario.ptoprevistoexercicio, 'Previsto no Exerc�cio!')) return;
	
	document.formulario.act.value = 'inserir';
   if( window.confirm( "ATEN��O!\n A cria��o de uma suba��o n�o permite voltar atr�s.\n O sistema ir� criar a suba��o e enviar e-mail para o Coordenador informando de sua cria��o. \n Somente ap�s o Coordenador ter aprovado a cria��o � que a suba��o estar� dispon�vel para ser associada ao seu Projeto Especial. \n Esta associa��o n�o � autom�tica e exigir� que voc� atualize o seu Projeto. \n Quando o Coordenador aprovar a suba��o voc� receber� um e-mail avisando. \n Caso esteja certo clique OK ou Cancelar para corrigir.") )
    {

   	    document.formulario.submit();
    } else return;

}

</script>
