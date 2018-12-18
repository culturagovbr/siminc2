<?php
if(!$_SESSION['painel']['metid']){
	header("Location: painel.php?modulo=principal/listarMetasIndicador&acao=A");
}
include (APPRAIZ.'www/painel/_funcoes_metas.php');

$mIndicador = new Painel_Model_Indicador();
$listaPerfis = $mIndicador->RetornaPerfil();

if($_REQUEST['requisicaoAjax']){
    header('content-type: text/html; charset=ISO-8859-1');
    switch ($_REQUEST['requisicaoAjax']) {
        case "getComboPeriodoPorPerid":
            if(!$_REQUEST['perid'] && $_REQUEST['dpeid']){
                    $perid = $db->pegaUm("select perid from painel.detalheperiodicidade where dpeid = {$_REQUEST['dpeid']} and dpeanoref= '".$_SESSION['exercicio']."'");
            }else{
                    $perid = $_REQUEST['perid'];
            }
            if($perid){
                    getComboPeriodoPorPerid($perid,false,$_REQUEST['dmiid'],$_SESSION['painel']['metid']);
            }else{
                    echo "Selecione o período.";
            }

            break;
        case "salvarValorMeta":
            $retorno = salvarValorMeta();
            echo json_encode($retorno);
            die;
        case "listarValorMeta":
            echo listarValorMetas($_SESSION['painel']['metid'], $listaPerfis);
            die;
        default:
            $_REQUEST['requisicaoAjax']();
            break;
    }
}

if($_REQUEST['requisicao']){
	$resultado = $_REQUEST['requisicao']();
}

$sql = "select 
			ind.indid,
			ind.indnome,
			ind.indqtdevalor,
			met.perid,
			ume.umedesc
		from 
			painel.metaindicador met
		inner join
			painel.indicador ind ON ind.indid = met.indid
		inner join
			painel.unidademeta ume ON ind.umeid = ume.umeid 
		where 
			metid = {$_SESSION['painel']['metid']}";
//                        ver($sql, d);
$arrDados = $db->pegaLinha($sql);

// monta cabeçalho 
//include APPRAIZ . 'includes/cabecalho.inc';
//print '<br/>';
//$menu[0] = array("descricao" => "Lista de Indicadores", "link"=> "/painel/painel.php?modulo=principal/lista&acao=A");
//$menu[1] = array("descricao" => "Cadastro de Indicadores", "link"=> "/painel/painel.php?modulo=principal/cadastro&acao=A");
//$menu[2] = array("descricao" => "Série Histórica", "link"=> "/painel/painel.php?modulo=principal/listarMetasIndicador&acao=A");
//$menu[3] = array("descricao" => "Metas do Indicador", "link"=> "/painel/painel.php?modulo=principal/metaIndicador&acao=A");
//$menu[4] = array("descricao" => "Listagem de Metas do Indicador", "link"=> "/painel/painel.php?modulo=principal/listarMetasIndicador&acao=A");
//$menu[5] = array("descricao" => "Editar Metas do Indicador", "link"=> "/painel/painel.php?modulo=principal/cadastrarMetasIndicador&acao=A");
//$menu[6] = array("descricao" => "Valores de Meta", "link"=> "/painel/painel.php?modulo=principal/valorMetasIndicador&acao=A");
//$menu[7] = array("descricao" => "Estratégia de Metas", "link"=> "/painel/painel.php?modulo=principal/cadastroEstrategiaMetas&acao=A");
//$menu[8] = array("descricao" => "Avaliação do Gestor", "link"=> "/painel/painel.php?modulo=principal/avaliacaoGestor&acao=A");
//echo montarAbasArray($menu, "/painel/painel.php?modulo=principal/valorMetasIndicador&acao=A");

$titulo = "Valores de Meta";
//monta_titulo( $titulo, $arrDados['indid']." - ".$arrDados['indnome'] );

?>
<!--    <link href="../library/sliding_panel/css/slide.css" rel="stylesheet" media="screen">
    <script src="../library/jquery/jquery-1.11.1.min.js" type="text/javascript" charset="ISO-8895-1"></script>
    <script src="../library/jquery/jquery-ui-1.10.3/jquery-ui.min.js"></script>
    <script src="../library/chosen-1.0.0/chosen.jquery.js"></script>
            <link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
        <link href="/zimec/public/temas/simec/fonts/awesome/font-awesome.css?v=1.0" rel="stylesheet">
        <script src="../includes/JQuery/jquery-1.5.1.min.js" type="text/javascript"></script>
        <script src="../library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
        <script src="../library/chosen-1.0.0/docsupport/prism.js" type="text/javascript"></script>        
        <link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet" media="screen" >
        		<script language="JavaScript" src="../includes/funcoes.js"></script>
        <link href="/library/simec/css/css_reset.css" rel="stylesheet">
        <link href="/library/simec/css/barra_brasil.css" rel="stylesheet">
		<link href="../includes/Estilo.css" rel="stylesheet" type="text/css"/>
		<link href='../includes/listagem.css' rel='stylesheet' type='text/css'/>-->
<link href="/library/jquery/jquery-ui-1.10.3/themes/custom-theme/jquery-ui-1.10.3.custom.min.css" rel="stylesheet">
<script language="javascript" type="text/javascript" src="/includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../painel/js/metas_indicador.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<style>
	.SubtituloTabela{background-color:#cccccc}
	.negrito{font-weight:bold}
	.bold{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.direita{text-align: right;}
	.esquerda{text-align: left;}
	.link{cursor: pointer;}
	.hidden{display:none}
</style>
<?php $formatoinput = pegarFormatoInput($arrDados['indid']);
	  $formatoinput['campovalor']['mascara'] = !$formatoinput['campovalor']['mascara'] ? "###.###.###.###.###,##" : $formatoinput['campovalor']['mascara']; ?>
<script>
function editarValorMeta(perid,dmiid,dpeid, dmiobs,dmiqtde,dmivalor)
{ 
	$("[name=dmiid]").val(dmiid);
	$("[name=perid]").val(perid);
        $("[name=dmiobs]").val(dmiobs);
	executarAjax("requisicaoAjax=getComboPeriodoPorPerid&perid="+perid+"&dpeid=" + dpeid + "&dmiid=" + dmiid,$("#combo_dpeid"));
	$("[name=dmiqtde]").val(mascaraglobal('[<?php echo $formatoinput['mascara'] ?>]',dmiqtde));
	if($("[name=dmivalor]") && dmivalor){
		$("[name=dmivalor]").val(mascaraglobal('<?php echo $formatoinput['campovalor']['mascara'] ?>',dmivalor));
	}else{
		$("[name=dmivalor]").val("");
	}
}
function alteraPeriodo(perid)
{
	executarAjax("requisicaoAjax=getComboPeriodoPorPerid&perid="+perid,$("#combo_dpeid"));
}
</script>
<form name="formulario_valor_metas" id="formulario_valor_metas"  method="post" action="" >
	<input type="hidden" id="metid" name="metid" value="<?php echo $_SESSION['painel']['metid'] ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubTituloDireita" >Tipo de Período:</td>
			<td id="combo_perid">
				<?php $sql = "	select
									perid as codigo,
									perdsc as descricao
								from
									painel.periodicidade
								where
									pernivel >= (select pernivel from painel.periodicidade where perid = {$arrDados['perid']})"; 
					$db->monta_combo('perid',$sql,"S",'Selecione o Período','alteraPeriodo','','','','S','','',$arrDados['perid']);
				?>
			</td>
		</tr>
		<tr>
			<td width="25%" class="SubTituloDireita" >Período:</td>
			<td id="combo_dpeid"><?php getComboPeriodoPorPerid($arrDados['perid'],true,"",$_SESSION['painel']['metid']) ?></td>
		</tr>
		<tr>
			<td width="25%" class="SubTituloDireita">Previsto:</td>
			<td><?php echo campo_texto('dmiqtde','S',"S",'',20,20,$formatoinput['mascara'],'');?></td>
		</tr>
		<tr>
			<td width="25%" class="SubTituloDireita" >Observação:</td>
			<td><?php echo campo_texto('dmiobs','S',"S",'',120,120,'','');?></td>
		</tr>                
		<?php if($arrDados['indqtdevalor'] == "t"): ?>
			<tr>
				<td width="25%" class="SubTituloDireita" >Valor:</td>
				<td><?=campo_texto('dmivalor','N',"S",'',20,20,$formatoinput['campovalor']['mascara'],'');?></td>
			</tr>
		<?php endif; ?>
		<tr>
			<td width="25%" class="SubtituloTabela" ></td>
			<td class="SubtituloTabela" >
				<input type="hidden" id="dmiid" name="dmiid" value="" />
				<input type="hidden" name="requisicao" value="salvarValorMeta" />
			</td>
		</tr>
	</table>
</form>
<div id="lista_valor_meta">
<?php listarValorMetas($_SESSION['painel']['metid'], $listaPerfis); ?>
</div>
<?php if($resultado['msg']): ?>
<script>
	alert('<?php echo $resultado['msg'] ?>');
</script>
<?php endif; ?>