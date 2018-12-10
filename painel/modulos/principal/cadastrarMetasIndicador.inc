<?php
$sql = "select
                        secid,
                        acaid,
                        indid
                from
                        painel.indicador
                where
                        indid = {$_SESSION['indid']}";
$arrDados = $db->pegaLinha($sql);

$mIndicador = new Painel_Model_Indicador();
$listaPerfis = $mIndicador->RetornaPerfil();

if($_SESSION['painel']['metid']){
	$_REQUEST['metid'] = $_SESSION['painel']['metid'];
}

if($_POST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	include (APPRAIZ.'www/painel/_funcoes_metas.php');
        switch ($_POST['requisicaoAjax']) {
            case 'getComboPeriodoPorPerid':
                getComboPeriodoPorPerid($_POST['perid'],false,"",$_SESSION['painel']['metid']);

                break;

            default:
                $_POST['requisicaoAjax']();
                break;
        }
	exit();
}

if($_POST['requisicao']){
	include (APPRAIZ.'www/painel/_funcoes_metas.php');
	$resultado = $_POST['requisicao']();
}
$habilitado='S';
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
//        ver($sql, d);
        if (in_array(PAINEL_PERFIL_GESTOR_INDICADOR, $listaPerfis)){
            $habilitado='N';
        }
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

if($arrDados && $arrDados['metid']){
	$_REQUEST['metid'] = $metid;
	$_SESSION['painel']['metid'] = $metid;

	$menu[0] = array("descricao" => "Lista de Indicadores", "link"=> "/painel/painel.php?modulo=principal/lista&acao=A");
	$menu[1] = array("descricao" => "Cadastro de Indicadores", "link"=> "/painel/painel.php?modulo=principal/cadastro&acao=A");
	$menu[2] = array("descricao" => "Série Histórica", "link"=> "/painel/painel.php?modulo=principal/listaSerieHistorica&acao=A");
	$menu[3] = array("descricao" => "Metas do Indicador", "link"=> "/painel/painel.php?modulo=principal/metaIndicador&acao=A");
	$menu[4] = array("descricao" => "Listagem de Metas do Indicador", "link"=> "/painel/painel.php?modulo=principal/listarMetasIndicador&acao=A");
	$menu[5] = array("descricao" => "Cadastrar Metas do Indicador", "link"=> "/painel/painel.php?modulo=principal/cadastrarMetasIndicador&acao=A");
//	$menu[6] = array("descricao" => "Valores de Meta", "link"=> "/painel/painel.php?modulo=principal/valorMetasIndicador&acao=A");
	$menu[7] = array("descricao" => "Legendas", "link"=> "/painel/painel.php?modulo=principal/cadastroEstrategiaMetas&acao=A");
	$menu[8] = array("descricao" => "Avaliação do Gestor", "link"=> "/painel/painel.php?modulo=principal/avaliacaoGestor&acao=A");
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
    <input type="hidden" name="secid" value="<?=$arrDados['secid']?>">
    <input type="hidden" name="acaid" value="<?=$arrDados['acaid']?>">
    <input type="hidden" name="indid" value="<?=$arrDados['indid']?>">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita" >Selecione a Periodicidade:</td>
			<td id="td_combo_perid" >
				<?php 
				$perid = $arrDados['perid'];
				if($_SESSION['indid']){
					$sql = "select 
								perid as codigo,
								perdsc as descricao
							from
								painel.periodicidade
							where
								perstatus = 'A'
							and
								pernivel >= (select pernivel from  painel.periodicidade where perid = (select perid from painel.indicador where indid = {$_SESSION['indid']}))
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
				<?=campo_textarea( 'metdesc', 'S', $habilitado, '', 80, 4, 1000 );?>
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
                                <?php
                                if ($_REQUEST['metid']){?>
				<input type="button" id="btn_valores" value="Valores de Meta" onclick="javascript:mostrarPopup('painel.php?modulo=principal/valorMetasIndicador&acao=A');" />
                                <?php
                                }
                                ?>
			</td>
		</tr>
	</table>
</form>
<div id="dialog_vincular_temas" title="Valores da Meta"></div>
<?php if($resultado['msg']): ?>
<script>
	alert('<?php echo $resultado['msg'] ?>');
</script>
<?php endif; ?>