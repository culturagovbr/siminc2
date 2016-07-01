<?php 
$preid = $_REQUEST['preid'] ? $_REQUEST['preid'] : $_SESSION['par']['preid'];

$titu = $_SESSION['par']['itrid'] == 2 ? 'Munic�pio' : 'Estado';

if( $_REQUEST['obrid'] ){
	
	$endereco = $_REQUEST['endlog'].' - '.$_REQUEST['endnum'].', '.$_REQUEST['endcom'].', '.$_REQUEST['endbai'];
?>
<table>
	<tr>
		<td>
			Nome da Obra: 
		</td>
		<td>
			<?=$_REQUEST['obrdesc'] ?>
		</td>
	</tr>
	<tr>
		<td>
			Programa/Fonte:
		</td>
		<td>
			<?=$_REQUEST['plititulo'] ?>
		</td>
	</tr>
	<tr>
		<td>
			Tipo de Obra:
		</td>
		<td>
			<?=$_REQUEST['tobadesc'] ?>
		</td>
	</tr>
	<tr>
		<td>
			Valor da Obra:
		</td>
		<td>
			<?=number_format($_REQUEST['obrvalorprevisto'],2,',','.'); ?>
		</td>
	</tr>
	<tr>
		<td>	
			Endere�o completo:
			</br>CEP:
		</td>
		<td>
			<?=$endereco ?></br> <?=$_REQUEST['endcep'] ?>
		</td>
	</tr>
	<tr>
		<td>
			Situa��o:
		</td>
		<td>
			<?=$_REQUEST['situacao'] ?>
		</td>
	</tr>
</table>

<?php 
	exit;
}

if($preid){
	$lnkabas = "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=listaObras&preid=".$preid;
}else{
	$lnkabas = "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A";
}

echo carregaAbasProInfancia($lnkabas, $preid, $descricaoItem ); 
monta_titulo( 'Obras no '.$titu, $obraDescricao  );

echo cabecalho();

if($_SESSION['par']['muncod']){

//	$sql = "SELECT
//				'<div border=\"0\" onclick=\"redireciona(' || oi.obrid || ')\" 
//				onmouseover=\"SuperTitleAjax(\'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=listaObras&preid={$preid}&obrid=' || oi.obrid ||
//				'&obrdesc=' || coalesce(oi.obrdesc,'n�o informado') ||
//				'&plititulo=' || coalesce(plititulo,'n�o informado') ||
//				'&tobadesc=' || coalesce(tobadesc,'n�o informado') ||
//				'&obrvalorprevisto=' || coalesce(obrvalorprevisto,0) ||
//				'&endcep=' || coalesce(endcep,'n�o informado') ||
//				'&endlog=' || coalesce(endlog,'n�o informado') ||
//				'&endnum=' || coalesce(endnum,'n�o informado') ||
//				'&endcom=' || coalesce(endcom,'n�o informado') ||
//				'&endbai=' || coalesce(endbai,'n�o informado') ||
//				'&situacao=' || 
//				CASE WHEN iexsitdominialimovelregulariza = true
//					THEN 'Regularizado'
//					ELSE 'N�o Regularizado'
//				END || 
//				'\')\" 
//				onmouseout=\"SuperTitleOff( this );\" >' || obrdesc || '</div>' as descricao
//			FROM
//				obra/*s.obra*/infraestrutura oi
//			INNER JOIN entidade.endereco e ON e.endid = oi.endid AND e.muncod = '{$_SESSION['par']['muncod']}'
//			LEFT JOIN monitora.pi_obra pio ON pio.obrid = oi.obrid
//			LEFT JOIN monitora.pi_planointerno pipi ON pio.pliid = pipi.pliid 
//			LEFT JOIN obras.tipoobra tpo ON tpo.tobaid = oi.tobraid
//			LEFT JOIN obras.infraestrutura iex ON iex.iexid = oi.iexid
//			WHERE oi.prfid = 41";
			
	$sql = "SELECT
				'<div border=\"0\" onclick=\"redireciona(' || oi.obrid || ')\" 
				onmouseover=\"SuperTitleAjax(\'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=listaObras&preid={$preid}&obrid=' || oi.obrid ||
				'&obrdesc=' || coalesce(oi.obrnome,'n�o informado') ||
				'&plititulo=' || coalesce(plititulo,'n�o informado') ||
				'&tobadesc=' || coalesce(tobdesc,'n�o informado') ||
				'&obrvalorprevisto=' || coalesce(obrvalorprevisto,0) ||
				'&endcep=' || coalesce(endcep,'n�o informado') ||
				'&endlog=' || coalesce(endlog,'n�o informado') ||
				'&endnum=' || coalesce(endnum,'n�o informado') ||
				'&endcom=' || coalesce(endcom,'n�o informado') ||
				'&endbai=' || coalesce(endbai,'n�o informado') ||
				'&situacao=' || 
				CASE WHEN iexsitdominialimovelregulariza = true
					THEN 'Regularizado'
					ELSE 'N�o Regularizado'
				END || 
				'\')\" 
				onmouseout=\"SuperTitleOff( this );\" >' || obrnome || '</div>' as descricao
			FROM
				obras2.obras oi
			INNER JOIN obras2.empreendimento 	emp  ON emp.empid = oi.empid
			INNER JOIN entidade.endereco 		e    ON e.endid   = oi.endid AND e.muncod = '{$_SESSION['par']['muncod']}'
			LEFT  JOIN monitora.pi_obra 		pio  ON pio.obrid = oi.obrid
			LEFT  JOIN monitora.pi_planointerno pipi ON pio.pliid = pipi.pliid 
			LEFT  JOIN obras2.tipoobra 			tob  ON tob.tobid = oi.tobid
			LEFT  JOIN obras2.infraestrutura 	iex  ON iex.iexid = oi.iexid					
			WHERE emp.prfid = 41";
	
	$arrObras = $db->carregar($sql);
}
?>
<script type="text/javascript" src="../../includes/remedial.js"></script>
<script type="text/javascript" src="../../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">	
	<?php if(!$_SESSION['par']['muncod'] || !$arrObras){ ?>
		<tr>
			<td align="center">
			N�o existem obras no <?=$titu ?>.
			</td>
		</tr>
	<?php }else{ ?>
		<tr>
			<td align="center">
			<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="color: rgb(51, 51, 51);">
			<thead>
				<tr>
					<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title">Nome da Obra
					</td>
				</tr> 
			</thead>
			</table>
			<?php $arrCabecalho = array(""); ?>
			<?php $db->monta_lista_simples($sql,$arrCabecalho,100,20 )?>
			</td>
		</tr>
	<?php } ?>
</table>
<script>

function redireciona( obrid ){

	window.opener.location = '/obras/obras.php?modulo=principal/cadastro&acao=A&obrid='+obrid;
	return false;
}
</script>

