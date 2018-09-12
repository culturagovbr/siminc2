<?php
if($_GET['novameta']){
	$sql = "select
				secid,
				acaid,
				indid
			from
				painel.indicador
			where
				indid = {$_GET['novameta']}";
	$arrDados = $db->pegaLinha($sql);
}

if($_SESSION['painel']['metid']){
	$_REQUEST['metid'] = $_SESSION['painel']['metid'];
}

if($_POST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	include (APPRAIZ.'www/painel/_funcoes_metas.php');
	$_POST['requisicaoAjax']();
	exit();
}

if($_POST['requisicao']){
	include (APPRAIZ.'www/painel/_funcoes_metas.php');
	$resultado = $_POST['requisicao']();
}

if($_REQUEST['metid'] || $resultado['metid']){
	$metid = $_REQUEST['metid'] ? $_REQUEST['metid'] : $resultado['metid'];
	$sql = "select 
				met.*,
				ind.acaid,
				ind.secid 
			from 
				painel.metaindicador met
			inner join
				painel.indicador ind ON ind.indid = met.indid 
			where 
				metid = $metid";
	$arrDados = $db->pegaLinha($sql);
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

if($arrDados && $arrDados['metid']){
	$_REQUEST['metid'] = $metid;
	$_SESSION['painel']['metid'] = $metid;
	$menu[0] = array("descricao" => "Listagem de Metas do Indicador", 		 "link"=> "/painel/painel.php?modulo=principal/listarMetasIndicador&acao=A");
	$menu[1] = array("descricao" => "Editar Metas do Indicador", "link"=> "/painel/painel.php?modulo=principal/cadastrarMetasIndicador&acao=A");
	$menu[2] = array("descricao" => "Valores de Meta", 		 	 "link"=> "/painel/painel.php?modulo=principal/valorMetasIndicador&acao=A");
	echo montarAbasArray($menu, "/painel/painel.php?modulo=principal/cadastrarMetasIndicador&acao=A");
}else{
	$db->cria_aba($abacod_tela,$url,'');	
}
$titulo = ($_REQUEST['metid'] ? "Editar" : "Cadastrar")." Metas do Indicador";
monta_titulo( $titulo, '&nbsp' );

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../painel/js/metas_indicador.js"></script>
<style>
	.SubtituloTabela{background-color:#cccccc}
	.negrito{font-weight:bold}
	.bold{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.direita{text-align: right;}
	.esquerda{text-align: left;}
</style>

<form name="formulario_metas" id="formulario_metas"  method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubTituloDireita" >Selecione a Secretaria:</td>
			<td id="td_combo_secid" >
				<?php				
				$secid = $arrDados['secid'];
				$sql = "select 
							secid as codigo,
							secdsc as descricao
						from
							painel.secretaria
						where
							secstatus = 'A'";
				$db->monta_combo('secid',$sql,'S','Selecione a Secretaria','filtrarAcao','');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Selecione a Ação:</td>
			<td id="td_combo_acaid" >
				<?php 
				$acaid = $arrDados['acaid'];
				if($secid){
					$habilitado = "S";
					$sql = "select 
								acaid as codigo,
								acadsc as descricao
							from
								painel.acao
							where
								acastatus = 'A'
							and
								acaid in ( select acaid from painel.acaosecretaria where secid = $secid)";
				}else{
					$habilitado = "N";
					$sql = array();
				}
				$db->monta_combo('acaid',$sql,$habilitado,'Selecione a Ação','filtrarIndicador','');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Selecione o Indicador:</td>
			<td id="td_combo_indid" >
				<?php 
				$indid = $arrDados['indid'];
				if($secid && $acaid){
					$habilitado = "S";
					$sql = "select 
								indid as codigo,
								indid || ' - ' || indnome as descricao
							from
								painel.indicador
							where
								indstatus = 'A'
							and
								secid = $secid
							and
								acaid = $acaid";
				}else{
					$habilitado = "N";
					$sql = array();
				}
				$db->monta_combo('indid',$sql,$habilitado,'Selecione o Indicador','filtrarPeriodicidade','');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Selecione a Periodicidade:</td>
			<td id="td_combo_perid" >
				<?php 
				$perid = $arrDados['perid'];
				if($indid){
					$habilitado = "S";
					$sql = "select 
								perid as codigo,
								perdsc as descricao
							from
								painel.periodicidade
							where
								perstatus = 'A'
							and
								pernivel >= (select pernivel from  painel.periodicidade where perid = (select perid from painel.indicador where indid = $indid))
							order by
								pernivel";
				}else{
					$habilitado = "N";
					$sql = array();
				}
				$db->monta_combo('perid',$sql,$habilitado,'Selecione a Periodicidade','verificaMetaAnterior','');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Meta:</td>
			<td>
				<?php $metdesc = $arrDados['metdesc'] ?>
				<?=campo_textarea( 'metdesc', 'S', 'S', '', 80, 4, 1000 );?>
			</td>
		</tr>
		<tr>
			<?php
				$metcumulativa = $arrDados['metcumulativa'];
				$checkValorNaoAc = "";
				$checkValorSimAc = "";
				$metcumulativa == "S" ? $checkValorSimAc = "checked=checked" : (($metcumulativa == "N")? $checkValorNaoAc = "checked=checked" : $checkValorAnoAc = "checked=checked");
				!$metcumulativa ? $checkValorSimAc = "checked=checked" : "";
				!$metcumulativa ? $checkValorNaoAc = "" : "";
				!$metcumulativa ? $checkValorAnoAc = "" : "";
			?>
		    <td align='right' class="SubTituloDireita">Exibir dados acumulados entre os períodos?</td>
		    <td><input type="radio" value="N" <?=$checkValorNaoAc?> name="metcumulativa" id="metcumulativa_n" />Não <input id="metcumulativa_s" type="radio" <?=$checkValorSimAc?> value="S" name="metcumulativa" />Sim <input type="radio" <?=$checkValorAnoAc?> value="A" name="metcumulativa" />Por ano</td>
		</tr>
		<tr>
			<td class="SubtituloTabela" ></td>
			<td class="SubtituloTabela" >
				<input type="hidden" name="requisicao" value="salvarMetas" />
				<input type="hidden" name="metid" value="<?php echo $_REQUEST['metid'] ?>" />
				<input type="button" id="btn_salvar" value="Salvar" onclick="salvarMetas()" />
				<input type="button" id="btn_cancelar" value="Cancelar" onclick="window.location.href='painel.php?modulo=principal/listarMetasIndicador&acao=A'" />
			</td>
		</tr>
	</table>
</form>
<?php if($resultado['msg']): ?>
<script>
	alert('<?php echo $resultado['msg'] ?>');
</script>
<?php endif; ?>