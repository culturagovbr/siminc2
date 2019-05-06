<?
/*
Sistema Simec
Setor responsavel: SPO-MEC
Desenvolvedor: Equipe Consultores Simec
Analista: Gilberto Arruda Cerqueira Xavier
Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Fabrício Mendonça(fabriciomendonca@gmail.com)
Modificacoes: Thiago Mata thiago_mata@yahoo.com.br 9 de novembro de 2006
Módulo:plantrabpje_apresentacao.inc
Finalidade: este arquivo monta o corpo do plano gerencial
*/
?>
<?
//	dbg( $_SESSION, 1 );
?>


<script language="JavaScript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/remedial.js"></script>
<script language="JavaScript" src="../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/superTitle.css">
<script language="JavaScript" src="plantrabpje_funcoes_js.js"></script>
<script language="JavaScript" src="plantrabpje_funcoes_js2.js"></script>
<script language="JavaScript">
	var limiteProjeto = 0<?=$limiteProjeto?>;
	function rep_img(){}
	
</script>
<script language="JavaScript" src="../includes/dateFunctions.js"></script>
<script language="JavaScript">
	var globalDataFimValida = '';
	function atualizaDataPelaDuracao( objInputDataInicio, objInputDataTermino, objInputIntervalo, strQuemMudara ){
		try{
			var intDuracao	= 0 + forceInt( objInputIntervalo.value );
			
			objDataInicio	= strDateToObjDate( objInputDataInicio.value , 'd/m/Y' , '/' );
			objDataTermino	= strDateToObjDate( objInputDataTermino.value , 'd/m/Y' , '/' );
			switch( strQuemMudara )
			{
				case 'objDataInicio':
				{
					// falta a funcao tira dias
					return;
					break;
				}
				case 'objDataTermino':
				{
					objDataTermino	= moreManyDays( objDataInicio , intDuracao );
					objInputDataTermino.value = objDateToStrDate( objDataTermino, 'd/m/Y' , '/' );
					break;
				}
				case 'objIntervalo':
				{
					intNovaDuracao = Math.ceil(( objDataTermino - objDataInicio )/( 60*60*24*1000 ));
					objInputIntervalo.value = intNovaDuracao;
					break;
				}
				default:
				{
					// ninguem será mudado //
					return;
				}
			}
			if( intNovaDuracao < 0 )
			{
				alert( 'Data Término não pode ser Anterior que Data Início.');
				
				objInputDataTermino.value = globalDataFimValida;
				objInputDataTermino.onchange();
			}
			else
			{	
				globalDataFimValida = objInputDataTermino.value;
			}
		}catch( e )
		{
		}
	}
	
</script>
<?
//Verifica se houve erro na alteração da data
if( $erroData )
{
	printf( "<script>alert( '%s' );</script>", $erroData[ 'mensagem' ] );
}
if( $strErro )
{
	printf( "<script>alert( '%s' );</script>", $strErro );
}
?>
<br>
<?
$db->cria_aba($abacod_tela,$url,'');
if($_REQUEST['act2']=='alterar') {
	$sql= "select * from monitora.planotrabalho where ptoid=".$_REQUEST['ptoid'];
	$saida = $db->recuperar($sql,$res);
	if(is_array($saida)){
		foreach($saida as $k=>$v) ${$k}=$v;
		$_REQUEST['ptotipo']=$ptotipo;
	}
 } else {
	$ptocod = $_REQUEST['ptocod'];
	$ptodsc =$_REQUEST['ptodsc'];
	$unmcod = $_REQUEST['unmcod'];
	$ptoprevistoexercicio = $_REQUEST['ptoprevistoexercicio'];
	$ptosnsoma = $_REQUEST['ptosnsoma'];
	$ptosnpercent = $_REQUEST['ptosnpercent'];
	$ptoanofim = $_REQUEST['ptoanofim'];
	$ptoid_antecessor = $_REQUEST[ 'ptoid_antecessor' ];
	$ptodata_ini = $_REQUEST[ 'ptodata_ini' ] ? date( "Y-m-d", gera_timestamp( $_REQUEST[ 'ptodata_ini' ] ) ) : '' ;
	$ptodata_fim = $_REQUEST[ 'ptodata_fim' ] ? date( "Y-m-d", gera_timestamp( $_REQUEST[ 'ptodata_fim' ] ) ) : '' ;
	$_REQUEST[ 'act2' ] = $_REQUEST[ 'ptotipo' ];

}

if ($_REQUEST['ptoid_pai'] or $ptoid_pai)
{
	if ($_REQUEST['ptoid_pai'])  $ptoid_pai= $_REQUEST['ptoid_pai'];
	$sql="select ptosndatafechada as paifechado, to_char(ptodata_ini,'dd/mm/YYYY') as pjedataini,to_char(ptodata_fim,'dd/mm/YYYY') as pjedatafim from monitora.planotrabalho where ptoid=$ptoid_pai";
	$datas=$db->pegalinha($sql);
	$pjeinimt=$datas['pjedataini'];
	$pjefimmt=$datas['pjedatafim'];
	$paifechado=$datas['paifechado'];

}

$titulo_modulo='Estruturar o Plano Gerencial do Projeto Especial';
if ($_REQUEST['ptotipo']=='P' ) $titulo_modulo='Inclusão de Etapa no Plano Gerencial';
if ($_REQUEST['ptotipo']=='M' ) $titulo_modulo='Inclusão de Macro-Etapa no Plano Gerencial';
if ($ptotipo=='P' ) $titulo_modulo='Alteração de Etapa no Plano Gerencial';
if ($ptotipo=='M' ) $titulo_modulo='Alteração de Macro-Etapa no Plano Gerencial';


monta_titulo($titulo_modulo,'');
?>
<div align="center">
<center>

<?
// verifica se á coordenador de ação
$autoriza = false;
$coordpje=false;
$digit=false;

if ($db->testa_responsavel_projespec($_SESSION['pjeid'])) $coordpje = true;
// verifica se é digitador

if ($db->testa_digitador($_SESSION['pjeid'],'E')) $digit = true;
// verific se á super-usuário
if ($db->testa_superuser())   {
	$coordpje = true;
	$_SESSION[ 'coordpje' ] = true;
	$digit = true;
}

// verifica se o exercício está aberto para estruturar o simec
$sql= "select prsano from planejamento.programacaoexercicio where prsano='".$_SESSION['exercicio']."' and prsstatus='A'";

$registro=$db->recuperar($sql);

/*if (is_array($registro)) $autoriza = true;
else
{
// não está autorizado, então verifica se há alguma autorização especial
$sql= "select ae.aelid from autorizacaoespecial ae where ae.acaid =".$_SESSION['acaid']." and ae.aelstatus ='A' and ae.aeldata_inicio <='".date('Y-m-d')."' and ae.aeldata_fim >='".date('Y-m-d')."' and ae.togcod=8 ";
$registro=$db->recuperar($sql);
if (is_array($registro)) $autoriza = true;
}
*/
if (! $coordpje and ! $digit and ! $visivel) {
    ?>
       <script>
       alert ('Vocá não tem acesso para monitorar o Plano Gerencial neste Projeto Especial!');
       //history.back();
       </script>
    <?
    exit();
}
?>
<?
	/**
	 * Seleciona o somatório das previsões de desembolso do projeto especial em questão
	 */
	$sqlLimite = 
		" select " .
	    " sum( coalesce( dpe.dpevalor, 0 ) ) as DESPESA " .
	    " from monitora.planotrabalho p " .
	    " inner join monitora.plantrabpje pa on pa.ptoid=p.ptoid and pa.pjeid = " . $_SESSION['pjeid'] .
	    " left join monitora.desembolso_projeto dpe on dpe.ptoid = p.ptoid " .
	    " where p.ptostatus='A' and p.ptoid_pai is null";
	
	if( $ptoid )
	{
		$sqlLimite .= " and p.ptoid <> ".$ptoid;
	}
	$totalGasto = $db->pegaUm( $sqlLimite );
	//calcula o valor que ainda pode ser utilizado nas atividades do projeto especial
	$limiteProjeto = $pjevlrano - $totalGasto;
?>
<form method="POST"  name="formulario" id="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name="ptoid" value=<?=$ptoid?>>
<input type='hidden' name='exclui' value=0>
<input type='hidden' name='act'>
<input type='hidden' name='act2'>
<input type='hidden' name='ptotipo' value='<?=$_REQUEST['ptotipo']?>'>
<input type='hidden' name='dtini' value="<?=$pjedataini?>">
<input type='hidden' name='dtfim' value="<?=$pjedatafim?>">
<input type='hidden' name='dtinimt' value="<?=$pjeinimt?>">
<input type='hidden' name='dtfimmt' value="<?=$pjefimmt?>">
<input type='hidden' name='projfechado' value="<?=$projfechado?>">
<input type='hidden' name='paifechado' value="<?=$paifechado?>">
<input type='hidden' name='abrirarvore' value="<?=$_REQUEST['abrirarvore']?>">

<div id="camposDinamicos" style="visibility:hidden;margin:0; padding:0;"></div>


<center>
<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;">
  <tr>
    <td align='right' class="SubTituloDireita">Projeto :</td>
    <td><b><?=$pjecod?>&nbsp;-&nbsp;<?=$pjedsc?></b></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Período do Projeto:</td>
    <td><b><?=$pjedataini?>&nbsp; - &nbsp;<?=$pjedatafim?></b></td>
  </tr>  
  <? if ($pjefimmt or $pjeinimt)
  {?>
      <tr>
    <td align='right' class="SubTituloDireita">Período da Macro-Etapa Agregadora :</td>
    <td><b><?=$pjeinimt?>&nbsp; - &nbsp;<?=$pjefimmt?></b></td>
  </tr>
  <?}?>
  <tr>
    <td align='right' class="SubTituloDireita">Produto :</td>
    <td><b><?=$prodsc?></b></td>
  </tr>
        <tr>
    <td align='right' class="SubTituloDireita">Físico e Financeiro :</td>
    <td><b><?=$pjeprevistoano?>&nbsp;/&nbsp;<?='R$ '.number_format($pjevlrano,2,',','.')?></b></td>
  </tr> 
  <tr>
  <td></td>
  <td>
  <input type="button" name="btequipe" value="Equipe do Projeto" onclick="mostra_equipe()">
  </td>
  </tr>
  <tr>
		<td class="SubTituloDireita">Arquivos vinculados ao projeto:</td>
		<td>
			<? 	
				$sql=sql_vincula_arquivo('pjeid',$_SESSION[ 'pjeid' ]);
				$insere=0; 
				if ($coordpje or $digit) $insere=1;		   
				popup_arquivo( 'arquivo_pje', $sql, 'pjeid',$_SESSION[ 'pjeid' ], $insere, 400, 400 );
			?>
		</td>
	</tr>
		     <tr>
     <td align="right">GRÁFICO DE GANTT
     </td>
     <td>
     <input type="button" class='botao' value='Ver Gráfico' title='Ver Gráfico.' onclick="exibe_grafico('<?=$ptoid?>')">
     </td>
     </tr>
<?
if (! $_REQUEST['ptotipo'] and ! $_REQUEST['act']=='inserir' and ($coordpje or $digit) )
{?>
    <tr><td align='right' class="SubTituloDireita">Incluir novo item:</td><td>
    <?
   if (projetoaberto()) {?>
     <input type="button" class='botao' value='Etapa' title='Inclusão de uma etapa no projeto.' onclick="insere_pt('P')"><input type="button"  class='botao' name ='M' value='Macro-Etapa' title='Caracteriza uma Macro-Etapa que poderá conter etapas subordinadas.' onclick="insere_pt('M')">
     <? } else {?>
     <font color="red">O Projeto está Concluído ou Cancelado</font>
     <?}?>
     
     </td></tr>

<? } else if( $_REQUEST[ 'act2' ] ){
	$showForm = true;
	$_SESSION[ 'showForm' ] = true;
	unset( $_REQUEST[ 'arrCod' ] );
	?>
	 <tr>
        <td align='right' class="SubTituloDireita" colspan="2" style="height:1px;padding:0px;"></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Código:</td>
	<td>
	<? if (! $ptocod) $habil='S' ;else $habil= 'S'?>
	<?=campo_texto('ptocod','S',$habil,'',13,11,'','','','Entre com o código de sua escolha com até 11 caracteres.');?>
	</td>
      </tr>

    <tr>
        <td align='right' class="SubTituloDireita">Macro-Etapa Agregadora:</td>
	    <td>
	    <script type="text/javascript">
	    arrMacroEtapas = new Array();
		</script>
	    <?
	    
	    if ($_REQUEST['act2']=='M' or $_REQUEST['act2']=='P')
	    $sql =
	    " select " .
	    " p.ptoid as CODIGO, p.ptodsc as DESCRICAO, " .
	    " to_char( ptodata_ini, 'dd/mm/yyyy' ) as ptodata_ini, " .
	    " to_char( ptodata_fim, 'dd/mm/yyyy' ) as ptodata_fim, " .
	    " ptosndatafechada, " .
	    " sum( coalesce( dpe.dpevalor, 0 ) ) as DESPESA " .
	    " from monitora.planotrabalho p " .
	    " inner join monitora.plantrabpje pa on pa.ptoid=p.ptoid and pa.pjeid = " . $_SESSION['pjeid'] .
	    " left join monitora.desembolso_projeto dpe on dpe.ptoid = p.ptoid " .
	    " where p.ptostatus='A' and p.ptotipo='M' " .
	    " group by CODIGO, DESCRICAO, ptodata_ini, ptodata_fim, ptosndatafechada " .
	    " order by ptodsc ";
	    else
	    $sql =
	    " select " .
	    " p.ptoid as CODIGO,p.ptodsc as DESCRICAO, " .
	    " to_char( ptodata_ini, 'dd/mm/yyyy' ) as ptodata_ini, " .
	    " to_char( ptodata_fim, 'dd/mm/yyyy' ) as ptodata_fim, " .
	    " ptosndatafechada, " .
	    " sum( coalesce( dpe.dpevalor, 0 ) ) as DESPESA " .
	    " from monitora.planotrabalho p " .
	    " inner join monitora.plantrabpje pa on pa.ptoid=p.ptoid and pa.pjeid=".$_SESSION['pjeid'] .
	    " left join monitora.desembolso_projeto dpe on dpe.ptoid = p.ptoid " .
	    " where p.ptostatus='A' and p.ptotipo='M' and p.ptoid <> $ptoid and ( p.ptoid_pai <> $ptoid or p.ptoid_pai is null )" .
	    " group by CODIGO, DESCRICAO, ptodata_ini, ptodata_fim, ptosndatafechada " .
	    " order by ptodsc ";

	    $rsCriarObj = $db->carregar( $sql );
	    $db->monta_combo("ptoid_pai",$sql,'S',"Selecione a Macro-Etapa agregadora",'VerificaSaldo','','Se você deseja que esta atividade fique subordinada a uma Macro-etapa, a aqui.',400);
	    //$db->monta_combo("ptoid_pai",$sql,'S',"Selecione a Macro-Etapa agregadora",'chama_macroetapa('."'".$_REQUEST['act2']."'".')','','Se você deseja que esta atividade fique subordinada a uma Macro-etapa, a aqui.',400);
	    ?>
	    <script type="text/javascript">
	    <?
	    //Cria os objetos javascript para verificação das datas da Macro-Etapa agregadora
	    if( $rsCriarObj !== FALSE )
	    {
	    	foreach( $rsCriarObj as $linha )
	    	{
	    		$total = pega_saldo_total( $linha[ 'codigo' ] );
	    		?>

			    	arrMacroEtapas.push( { ptoid:"<?=$linha[ 'codigo' ]?>", dataini:"<?=$linha[ 'ptodata_ini' ]?>", datafim:"<?=$linha[ 'ptodata_fim' ]?>", datafechada:"<?=$linha[ 'ptosndatafechada' ]?>", somaDespesa:<?=$total?> } );

		    	<? 
	    	} 
	    }
	    //Fim da criação dos objetos javascript ?>
		</script>
		</td>
      </tr>
      
      <tr>
        <td align='right' class="SubTituloDireita">Atividade antecessora?:</td>
	<td>
	<script type="text/javascript">
	arrAntecessores = new Array();
	</script>
	<?
	/*
	if ($_REQUEST['act2']=='M' or $_REQUEST['act2']=='P')
	$sql = "select p.ptoid, p.ptoordem as CODIGO,p.ptoordem || ' - ' || p.ptodsc as DESCRICAO, to_char( ptodata_ini, 'dd/mm/yyyy' ) as ptodata_ini, to_char( ptodata_fim, 'dd/mm/yyyy' ) as ptodata_fim, ptosndatafechada,procod from monitora.planotrabalho p inner join monitora.plantrabpje pa on pa.ptoid=p.ptoid and pa.pjeid=".$_SESSION['pjeid']." where ptostatus='A' order by ptoordem";
	else
	$sql = "select p.ptoid, p.ptoordem as CODIGO,p.ptoordem || ' - ' || p.ptodsc as DESCRICAO, to_char( ptodata_ini, 'dd/mm/yyyy' ) as ptodata_ini, to_char( ptodata_fim, 'dd/mm/yyyy' ) as ptodata_fim, ptosndatafechada,procod from monitora.planotrabalho p inner join monitora.plantrabpje pa on pa.ptoid=p.ptoid and pa.pjeid=".$_SESSION['pjeid']." where ptostatus='A' and p.ptoordem <> $ptoordem order by ptoordem";
	*/
	
	if ($_REQUEST['act2']=='M' or $_REQUEST['act2']=='P')
	$sql = "select p.ptoid, p.ptoordem as CODIGO,p.ptoordem || ' - ' || p.ptodsc as DESCRICAO, to_char( ptodata_ini, 'dd/mm/yyyy' ) as ptodata_ini, to_char( ptodata_fim, 'dd/mm/yyyy' ) as ptodata_fim, ptosndatafechada from monitora.planotrabalho p inner join monitora.plantrabpje pa on pa.ptoid=p.ptoid and pa.pjeid=".$_SESSION['pjeid']." where ptostatus='A' order by ptoordem";
	else
	$sql = "select p.ptoid, p.ptoordem as CODIGO,p.ptoordem || ' - ' || p.ptodsc as DESCRICAO, to_char( ptodata_ini, 'dd/mm/yyyy' ) as ptodata_ini, to_char( ptodata_fim, 'dd/mm/yyyy' ) as ptodata_fim, ptosndatafechada from monitora.planotrabalho p inner join monitora.plantrabpje pa on pa.ptoid=p.ptoid and pa.pjeid=".$_SESSION['pjeid']." where ptostatus='A' and p.ptoordem <> $ptoordem order by ptoordem";
	$rsCriarObj = $db->carregar( $sql );
	$db->monta_combo("ptoid_antecessor",$sql,'S',"Selecione a Atividade antecessora",'','','Se você deseja que esta atividade seja antecedida por uma outra específica.',400);
	    ?>
	    <script type="text/javascript">
	    <?
	    //Cria os objetos javascript para verificação das datas da Macro-Etapa agregadora
	    foreach( $rsCriarObj as $linha )
	    {
	    	?>

	    	arrAntecessores.push( { ptoid:"<?=$linha[ 'ptoid' ]?>" ,ptoordem:"<?=$linha[ 'codigo' ]?>", dataini:"<?=$linha[ 'ptodata_ini' ]?>", datafim:"<?=$linha[ 'ptodata_fim' ]?>", datafechada:"<?=$linha[ 'ptosndatafechada' ]?>" } );


	    	<? } //Fim da criação dos objetos javascript ?>
		</script>
	</td>
      </tr>      
      <tr>
        <td align='right' class="SubTituloDireita">Título:</td>
		<td>
			<?=campo_texto('ptodsc','S','S','',77,80,'','','','Entre com o título (ou nome) que esta atividade será conhecida.');?>
		</td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Descrição:</td>
        <td><?=campo_textarea('ptodescricao','S','S','',80,3,'','','',' Campo utilizado para descrever o motivo da criação da atividade.');?></td>
      </tr>  
      <tr>
        <td align='right' class="SubTituloDireita">Antecedência de aviso:<br>(Padrão: 7 dias)</td>
	<td>
	<?=campo_texto('ptoavisoantecedencia','N','S','',4,2,'','','','O sistema irá avisá-lo n dias antes do início da atividade. Este aviso será visual e por e-mail.');?>
	</td>
      </tr>      
      <tr>
        <td align='right' class="SubTituloDireita">Data Início:</td>
        <td>
        
		<input class="normal" type="text"
		name="ptodata_ini" 
		onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);" 
		onmouseover="MouseOver(this);" 
		onchange="atualizaDataPelaDuracao( document.formulario.ptodata_ini , document.formulario.ptodata_fim, document.formulario.duracao , 'objDataTermino' )" 
		onkeyup="this.value=mascaraglobal('##/##/####',this.value);" 
		value="<?=  formata_data( $ptodata_ini )  ?>" 
		maxlength="10" 
		style="text-align: right;" size="12"/>
		        
		<a href="javascript:show_calendar('formulario.ptodata_ini');">
			<img src="../imagens/calendario.gif" width="16" height="15" border="0" align="absmiddle" alt="">
		</a>
				
	</td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Duração:</td>
        <td>
		<input type="text" 
		name="duracao"
		onchange="atualizaDataPelaDuracao( document.formulario.ptodata_ini , document.formulario.ptodata_fim, document.formulario.duracao , 'objDataTermino' )" size="3" maxlength="3" class="normal" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"/> dias
	</td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Data Término:</td>
        <td>
        
		<input 
		name="ptodata_fim"
		class="normal" type="text" 
		onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="globalDataFimValida=this.value;MouseClick(this);" 
		onmouseover="MouseOver(this);" 
		onchange="atualizaDataPelaDuracao( document.formulario.ptodata_ini , document.formulario.ptodata_fim, document.formulario.duracao , 'objIntervalo' )" 
		onkeyup="this.value=mascaraglobal('##/##/####',this.value);if( this.value.length == 10 ){ this.onchange() }" 
		value="<?= formata_data( $ptodata_fim ) ?>" 
		maxlength="10" 
		style="text-align: right;" size="12"/>
		
		<a href="javascript:show_calendar('formulario.ptodata_fim');">
			<img src="../imagens/calendario.gif" width="16" height="15" border="0" align="absmiddle" alt="">
		</a>		

	</td>
      </tr>
       <tr>
        <td align='right' class="SubTituloDireita">Congela as datas?</td>
        <td>
            <input type="radio" name="ptosndatafechada" value="t" <?=($ptosndatafechada=='t'?"CHECKED":"")?>>  Sim
                &nbsp;<input type="radio" name="ptosndatafechada" value="f" <?=($ptosndatafechada=='f'?"CHECKED":"")?>> Não
         </td>
       </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Produto:</td>
	    <td><?
	    $sql = "select procod as CODIGO,prodsc as DESCRICAO from produto where prostatus='A' order by prodsc ";
	    $db->monta_combo("procod",$sql,'S',"Selecione o Produto",'','','',400);
	    ?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Meta:</td>
	<td><?=campo_texto('ptoprevistoexercicio','S','S','',16,14,'#########','');?>
	</td>
      </tr>
	        <tr>      
      <tr bgcolor="#F2F2F2">
        <td align='right' class="SubTituloDireita">Unidade de Medida:</td>
        <td >
	<?
	$sql = "select unmcod as CODIGO,unmdsc as DESCRICAO from unidademedida where unmstatus='A' order by unmdsc ";
	$db->monta_combo("unmcod",$sql,'S',"Selecione a Unidade de Medida",'','','Entre com a Unidade de Medida da atividade. Campo obrigatório!',400,'S');
	?>
	</td>
      </tr>
 <tr >
        <td align='right' class="SubTituloDireita">Previsão de Desembolso:</td>
        <td >
	<?//(Valores em reais inteiros)campo_texto('ptovlrprevisto','N','S','',20,18,'###############','','rigth','Orçamento que se pretende alocar a esta Atividade e que deverá ser acompanhado neste sistema. Tenha em mente que o SIAFI não terá condições de acompanhar os seus lançamentos.');?> 
		<?
		$sql = $ptoid ? "select to_char( dpedata, 'dd/mm/yyyy' ) as data, dpevalor as valor from monitora.desembolso_projeto where ptoid=".$ptoid : false;
		$ptovlrprevisto = $sql ? $db->carregar( $sql ) : '';
		//$limite = calcula_limite( $pjeid, $ptoid, $pjevlrano );
		combo_desembolso( 'ptovlrprevisto', 'Selecione as previsões de desembolso', '400x400' , $limite , $maximo_itens  );
		?>
		</td>
      </tr>       

       <tr>
        <td align='right' class="SubTituloDireita">É cumulativo?</td>
        <td>
            <input type="radio" name="ptosnsoma" value="t" <?=($ptosnsoma=='t'?"CHECKED":"")?>>  Sim
                &nbsp;<input type="radio" name="ptosnsoma" value="f" <?=($ptosnsoma=='f'?"CHECKED":"")?>> Não
         </td>
       </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Responsável:</td>
	    <td><? $sql = "select distinct u.usucpf as CODIGO,u.usucpf ||'-'||u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum as DESCRICAO, u.usunome from seguranca.usuario u where u.usucpf in (select pu.usucpf from seguranca.perfilusuario pu where pu.pflcod=51) or u.usucpf in (select us.usucpf from seguranca.usuario_sistema us where us.pflcod=51) and u.usustatus='A' order by usunome ";


	$db->monta_combo("usucpf",$sql,'S',"Selecione o Responsável",'','','',400);
	    ?></td>
      </tr>       

              <tr>
        <td align='right' class="SubTituloDireita">Só o responsável pode editar?</td>
        <td>
            <input type="radio" name="ptosntemdono" value="t" <?=($ptosntemdono=='t'?"CHECKED":"")?>>  Sim
                &nbsp;<input type="radio" name="ptosntemdono" value="f" <?=($ptosntemdono=='f'?"CHECKED":"")?>> Não
         </td>
       </tr>
<?

if ($coordpje or $digit)
{
	if  ($_REQUEST["ptoid"]) {
		if (! $ptosnaprovado or $ptosnaprovado=='f' or $ptosnaprovado=='')
		{
			// print 'autoriza='.$autoriza;

			if ($digit or $coordpje)
			{  // ainda não está aprovado e sou digitador ou coordenador de ação
  ?>
         <tr bgcolor="#CCCCCC">
         <td></td>
         <td><input type="button" name="btalterar" value="Alterar" onclick="validar_cadastro('A')" class="botao">
         <input type="button" name="btvoltar" value="Cancelar" onclick="history.back();" class="botao"></td>
         </tr>
    <? }
    if ($coordpje)
    {
    	// ainda não está aprovado e sou coordenador de ação
       ?>
         <tr bgcolor="#CCCCCC">
         <td></td>
         <td><input type="button" name="btaprovar" value="Aprovar a Atividade" onclick="aprova_ativ(<?=$ptoid?>)" class="botao"><input type="button" name="btvoltar" value="Cancelar" onclick="history.back();" class="botao"></td>
         </tr>
       <?
    }
		}
		else
		{
			if ($coordpje)
			{
				// ainda não está aprovado e sou coordenador de ação
       ?>
         <tr bgcolor="#CCCCCC">
         <td></td>
         <td><input type="button" name="btaprovar" value="Retornar a atividade para edição" onclick="aprova_retorno(<?=$ptoid?>)" class="botao"><input type="button" name="btvoltar" value="Cancelar" onclick="history.back();" class="botao"></td>
         </tr>
       <?
			} else
			{
       ?>
         <tr bgcolor="#CCCCCC"><td></td>
         <td><b>A Atividade já foi aprovada pelo Coordenador de Ação e não pode ser editada.</b></td>
         </tr>
       <?
			}

		}
	}
	else {
?>
<tr bgcolor="#CCCCCC">
   <td></td>
   <td><input type="button" name="btinserir" value="Incluir" onclick="validar_cadastro('I')" class="botao"><input type="button" name="btvoltar" value="Cancelar" onclick="history.back();" class="botao"></td>

 </tr>
<?}

} else
print '<tr><td align="center"><b></b></td></tr>';} ?>
  <tr ><td align="right">Legenda:</td>
  <td><b>M - Macro-Etapa -----  E - Etapa ----- <img border='0' width=8 heigth=11 src='../imagens/ppa.gif' title='Macro-etapa proveniente do PPA'> - Proveniente do PPA</b>
  <?
if (! $_REQUEST['abrirarvore']) { ?>
  <input type="button" name="btabrirarvore" value="Abrir árvore de atividades " onclick="abrir_arvore('1')" class="botao">
  <?} else {
  	?>
  	  <input type="button" name="btabrirarvore" value="Fechar árvore de atividades " onclick="abrir_arvore('0')" class="botao">
  	<?}?>
  
  </td>
         </tr>
    </table>
<?
$sql = "select p.ptoid,ptoid_pai,ptotipo, p.ptoordem,p.ptocod, case when p.ptotipo='M' then 'M' when p.ptotipo='P' then 'E' end as tipo, p.ptoorigemppa, ptodsc,to_char(ptodata_ini,'dd/mm/yyyy') as inicio,to_char(ptodata_fim,'dd/mm/yyyy') as termino  from monitora.planotrabalho p where p.ptostatus='A' and p.ptoid in (select ptoid from monitora.plantrabpje where pjeid=".$_SESSION['pjeid'].") order by p.ptoordem";

// ordeno pelas datas de inicio e fim e pelo tipo
$rs = @$db->carregar( $sql );

	?>

<table style="width:754px;" align='center' border="0" cellspacing="0" cellpadding="0" class="listagem">
<thead>
    <tr>
      <td class="title" colspan="2" style="width:66px; padding:3px;"><strong>Ordem</strong></td>
      <td class="title" style="width:24px;padding:3px;"><strong>Tipo</strong></td>
      <td class="title" style="width:354px;padding:3px;" ><strong>Descrição </strong></td>
      <td class="title" style="width:55px;padding:3px;"><strong>Situalção</strong></td>
      <td class="title" style="width:55px;padding:3px;" ><strong>Início</strong></td>  
      <td class="title"  ><strong>Término</strong></td> 
      <?
      if ($coordpje) {?>
            <td class="title" ><strong>Aprov</strong></td> 
      <?}?>
   </tr>
</thead>
<tbody>
<?
if (  $rs && count($rs) > 0 )
{
	$i=0;
	foreach ( $rs as $linha )
	{
		foreach($linha as $k=>$v) ${$k}=$v;
		if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
		$nivel = $db->busca_pai($ptoid,0);
		// exibe status
		$sqlStatus = "select t.tpsdsc as status, t.tpscor as cor from public.tiposituacao t inner join monitora.execucaopje e on e.tpscod = t.tpscod where e.ptoid=".$ptoid." order by e.expdata desc limit 1";

		$rsStatus = @$db->recuperar( $sqlStatus );
		$status = $rsStatus[ "status" ] ? $rsStatus[ "status" ] : "S/ avaliação";
		$cor = $rsStatus[ "cor" ] ? $rsStatus[ "cor" ] : "black";
		$sqlAlt =
		" select " .
		" pt.ptodsc, " .
		" pt.ptoid, pt.ptosnaprovado," .
		" epobs.observacao, " .
		" pt.ptoprevistoexercicio as previsto, " .
		" sum( ep.exprealizado ) as realizado, " .
		" sum( ep.expfinanceiro ) as gasto, " .
		" ( ( sum( ep.exprealizado ) / pt.ptoprevistoexercicio ) * 100 ) as porcentagem " .
		" from monitora.planotrabalho pt " .
		" inner join monitora.execucaopje ep using ( ptoid ) " .
		" left join ( " .
		" select expobs as observacao, ptoid from monitora.execucaopje where ptoid = " . $ptoid . " order by expdata desc limit 1 " .
		" ) epobs using ( ptoid ) " .
		" where ptoid = " . $ptoid .
		" group by pt.ptodsc, pt.ptoid, pt.ptosnaprovado,epobs.observacao, pt.ptoprevistoexercicio";
		
		$dadosAlt = $db->recuperar( $sqlAlt );
		$txtAlt =
		"Previsto: " . formata_valor( $dadosAlt['previsto'], 0 ) . "<br/>" .
		"Executado: " . formata_valor( $dadosAlt['realizado'], 0 ) . "<br/>" .
		"Gasto: R$ " . formata_valor( $dadosAlt['gasto'], 0 ) . "<br/>" .
		"Percentual: " . formata_valor( $dadosAlt['porcentagem'], 2 ) . "%";
		if ( $dadosAlt['observacao'] )
		{
			$txtAlt .= "<br/><br/>" . $dadosAlt['observacao'];
		}
		$status = '<font color="'. $cor . '">' . $status . '</font>';
//		$status = '<span onmouseover="return escape(\'' . $txtAlt .'\')">' . $status . '</span>';
		$status = '<span onmouseover="SuperTitleOn( this , \'' .  simec_htmlentities( $txtAlt ) . '\')" onmouseout="SuperTitleOff( this )" >' . $status . '</span>';
		// FIM exibe status

		// para cada registro devo verificar se ele é uma etapa ou macro -etapa
		// se for uma macro etapa, coloco e
		if ($ptoorigemppa=='t')
		{
			$ppa="&nbsp;&nbsp;<img border='0' width=8 heigth=11 src='../imagens/ppa.gif' title='Macro-etapa proveniente do PPA'>";
		} else
		$ppa='';
		if ($ptotipo=='M' and $nivel ==0 )
		{

    	?>
       <tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';" >
          <td style="width:45px; text-align:left; padding:3px;"><img border="0" src="../imagens/alterar.gif" title="Alterar a atividade." onclick="altera_pto('<?=$ptoid?>')">&nbsp;&nbsp;<img border="0" src="../imagens/excluir.gif" title="Excluir a atividade." onclick="excluir_pto('<?=$ptoid?>','<?=$ptocod?>')">
          </td>
          <td style="width:24px; padding:3px;text-align:right;"><?=$ptoordem?>       </td>
          <td style="width:34px; padding:3px;"><b><?= $tipo .$ppa?></b></td>
		  <td  style="width:399px;padding:3px;" onclick="abreconteudo('geral/listamacroetapa.php?ptoid=<?=$ptoid?>','<?=$ptoid?>')"><img src="../imagens/mais.gif" name="+" border="0" id="img<?=$ptoid?>">&nbsp;&nbsp;<b><?=$ptodsc?></b>		  </td>
		  <td style="width:65px; padding:3px;"><?= $status ?></td>
	  <?
	  if( !$showForm && $coordpje )
	  {
	  ?>
     	<td style="width:55px; padding:3px;" onclick="altera_data('<?=$ptoid?>', 'dt_ini', '<?=$ptoordem ?>', '#000000')">		      	
      	<?//verifica a existencia de erro na alteração da data
			if( $erroData  && existe_no_array( $arrCodigos, $ptoid ) )
			{

		      	?>
		      	<span id="dt_ini<?=$ptoid?>" <? if( $erroData[ 'ptoid' ] == $ptoid )echo 'style="color:#ff0000;"' ?>><?= $_REQUEST[ 'dt_ini'.$ptoid ] ?></span>
		      	<input type="hidden" name="dt_ini<?=$ptoid?>" value="<?= $_REQUEST[ 'dt_ini'.$ptoid ] ?>" />
		      	<script type="text/javascript">altera_data('<?=$ptoid?>', 'dt_ini', '<?=$ptoordem ?>', '#000000', 1 );</script>
		      	<?
		      	?>
			<?
			}
			else 
			{ 
			?>
		      	<span id="dt_ini<?=$ptoid?>"><?= $inicio ?></span>
		      	<input type="hidden" name="dt_ini<?=$ptoid?>" value="<?= $inicio?>" />
			<? 
			}//Fim da verificação de erro na alteração da data 
			?>		      	
		</td>
		<td style="width:55px; padding:3px;" onclick="altera_data('<?=$ptoid?>', 'dt_fim', '<?=$ptoordem ?>', '#000000')">
			<?
			if( $erroData  && existe_no_array( $arrCodigos, $ptoid ) )
			{
	      	?>
				<span id="dt_fim<?=$ptoid?>" <? if( $erroData[ 'ptoid' ] == $ptoid )echo 'style="color:#ff0000;"' ?>><?= $_REQUEST[ 'dt_fim'.$ptoid ] ?></span>
				<input type="hidden" name="dt_fim<?=$ptoid?>" value="<?= $_REQUEST[ 'dt_fim'.$ptoid ] ?>" />
				<script type="text/javascript">altera_data('<?=$ptoid?>', 'dt_fim', '<?=$ptoordem ?>', '#000000', 1 );</script>
			<?
			} 
			else 
			{ 
	      	?>
				<span id="dt_fim<?=$ptoid?>"><?= $termino ?></span>
				<input type="hidden" name="dt_fim<?=$ptoid?>" value="<?= $termino?>" />
			<? 
	      	 } //Fim da verificação de erro na alteração da data ?>	
          </td>
          <? 
			if ($coordpje) 
			{ 
				$ok=0;
				$sql = "select ptosnaprovado from monitora.planotrabalho where ptoid='$ptoid' and pjeid=$pjeid ";
				$ok=$db->pegaUm($sql);
				?>
					<td >
						<input type="checkbox" name="aprovpto[]" value='<?=
							$ptoid."'";
							if ($ok=='t') {print " checked";}
							?>
						>
					</td>
		<?	} ?>
		<?
	  	}
		else 
		{
		?>
			<td style=" padding:3px;"><?= $inicio ?></td>
		  	<td style="width:55px; padding:3px;"><?= $termino?> </td>
		<? 
			if ($coordpje)
			{ 
          		$ok=0;
          		$sql = "select ptosnaprovado from monitora.planotrabalho where ptoid='$ptoid' and pjeid=$pjeid ";
        		$ok=$db->pegaUm($sql);
	          	?>
    	            <td >
    	            	<input type="checkbox" name="aprovpto[]" value='<?=$ptoid."' ";
			         	if ($ok=='t') {print " checked";}?>>
					</td>
		<?	} ?> 
	<?	}	?>
	</tr>
	<tr bgcolor="<?=$marcado?>"> 
		<td colspan="8" id="td<?=$ptoid?>"></td>
		<? //verificação se a atividade é pai da atividade com erro de alteração de data
		if( $erroData[ "ptoid_pai" ] == $ptoid or $_REQUEST['abrirarvore']==1)
		{
		?>
			<script type="text/javascript">
				abreconteudo('geral/listamacroetapa.php?ptoid=<?=$ptoid?>','<?=$ptoid?>');
			</script>
		<?
		}//Fim verificação se a atividade é pai da atividade com erro de alteração de data
		?>
	</tr>
	<?} else if ($nivel==1){
        	?>
     <tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';" >
        <td style="width:45px; text-align:left; padding:3px;"><img border="0" src="../imagens/alterar.gif" title="Alterar a atividade." onclick="altera_pto('<?=$ptoid?>')">&nbsp;&nbsp;<img border="0" src="../imagens/excluir.gif" title="Excluir a atividade." onclick="excluir_pto('<?=$ptoid?>','<?=$ptocod?>')">
        </td>
        <td style="width:24px;padding:3px;text-align:right;"><?=$ptoordem?> </td>
	    <td align=left style="width:34px; padding:3px;"><?=$tipo.$ppa?></td>
        <td style="width:399px;padding:3px;"><?=$ptodsc?>     </td>
        <td style="width:66px;padding:3px;"><?=$status?></td>
      	<?
      	if( !$showForm  && $coordpje ){
		  ?>
        <td style=" padding:3px;" onclick="altera_data('<?=$ptoid?>', 'dt_ini', '<?=$ptoordem ?>', '#000000')">
        <?//verifica a existencia de erro na alteração da data
		  if( $erroData  && existe_no_array( $arrCodigos, $ptoid ) )
		  {

		      	?>
		      	<span id="dt_ini<?=$ptoid?>" <? if( $erroData[ 'ptoid' ] == $ptoid )echo 'style="color:#ff0000;"' ?>><?= $_REQUEST[ 'dt_ini'.$ptoid ] ?></span>
		      	<input type="hidden" name="dt_ini<?=$ptoid?>" value="<?= $_REQUEST[ 'dt_ini'.$ptoid ] ?>" />
		      	<script type="text/javascript">altera_data('<?=$ptoid?>', 'dt_ini', '<?=$ptoordem ?>', '#000000', 1 );</script>
		      	<?
		      	?>
		      	<? } else { ?>
		      	<span id="dt_ini<?=$ptoid?>"><?= $inicio ?></span>
		      	<input type="hidden" name="dt_ini<?=$ptoid?>" value="<?= $inicio?>" />
		      	<? } //Fim da verificação de erro na alteração da data?>	
      	</td>
        <td style="width:55px; padding:3px;" onclick="altera_data('<?=$ptoid?>', 'dt_fim', '<?=$ptoordem ?>', '#000000')">
        <? //verifica a existencia de erro na alteração da data
		      	if( $erroData && existe_no_array( $arrCodigos, $ptoid ) )
		      	{

		      	?>
		      	<span id="dt_fim<?=$ptoid?>" <? if( $erroData[ 'ptoid' ] == $ptoid )echo 'style="color:#ff0000;"' ?>><?= $_REQUEST[ 'dt_fim'.$ptoid ] ?></span>
		      	<input type="hidden" name="dt_fim<?=$ptoid?>" value="<?= $_REQUEST[ 'dt_fim'.$ptoid ] ?>" />
		      	<script type="text/javascript">altera_data('<?=$ptoid?>', 'dt_fim', '<?=$ptoordem ?>', '#000000', 1 );</script>
		      	<?
		      	?>
		      	<? } else { ?>
		      	<span id="dt_fim<?=$ptoid?>"><?= $termino ?></span>
		      	<input type="hidden" name="dt_fim<?=$ptoid?>" value="<?= $termino?>" />
		      	<? } //Fim da verificação de erro na alteração da data ?>	
        </td>
        	<? 
            if ($coordpje) 
            { 
          		$ok=0;
          		$sql = "select ptosnaprovado from monitora.planotrabalho where ptoid='$ptoid' and pjeid=$pjeid ";
        		$ok=$db->pegaUm($sql);
	          	?>
    	            <td >
    	            	<input type="checkbox" name="aprovpto[]" value='<?=$ptoid."' ";
				         if ($ok=='t') {print " checked";}?>>
					</td>
		<?	} ?> 
		<?
      	} 
      	else 
      	{
		?>
		<td style=" padding:3px;"><?= $inicio ?></td>
		<td style=" padding:3px;"><?= $termino?> </td>
			<? 
            if ($coordpje)
			{ 
				$ok=0;
				$sql = "select ptosnaprovado from monitora.planotrabalho where ptoid='$ptoid' and pjeid=$pjeid ";
        		$ok=$db->pegaUm($sql);
			?>
               	<td >
               		<input type="checkbox" name="aprovpto[]" value='<?=$ptoid."' ";
		        		if ($ok=='t') {print " checked";}?>>
				</td>
		<?	}	?> 
<?	}	?>  
	 </tr>
	 <tr bgcolor="<?=$marcado?>">
		 
		 <td colspan="7" id="td<?=$ptoid?>"></td>
	 </tr>
	        <?}
	        $i++;}
		?>
	<?
	if( !$showForm && $coordpje && projetoaberto()){
	 ?>
	 <tr>
	 	<td colspan="5"></td>
	 	<td colspan="2"><input type="button" class="botao" onclick="submeterAlteracoes();" value="Salvar Alterações"></td>
	 <?}?>
	 	 	<?
	 	if( $coordpje ){
	 		?>
	 		<td colspan="2"><input type="button" class="botao" onclick="submete_aprov();" value="OK"></td>
	 		<?
	 	}
?>
	 </tr>
<?}?>
    
</tbody>
</table>
    </center>
  </div>
</form>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script> 
<script>
  function mostra_equipe()
  {
  	 e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/verequipe&acao=A";
     window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=800,height=300'");
  }
</script>
<script language="JavaScript">
atualizaDataPelaDuracao( document.formulario.ptodata_ini , document.formulario.ptodata_fim, document.formulario.duracao , 'objIntervalo' );
</script>
 