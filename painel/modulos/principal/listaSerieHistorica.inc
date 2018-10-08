<?php
/*
 * VALIDANDO SE EXISTE O ID DO INDICADOR NA SESSÃO
 * É OBRIGATÓRIO A EXISTÊNCIA DESTA VARIAVEL NA SESSÃO
 */
if(!$_SESSION['indid']) {
	echo "<script>
			alert('Não existe indicador selecionado.');
			window.location='?modulo=inicio&acao=C';
		  <script>";
	exit;
}

/* Segurança : Validando os acessos as funções autorizadas */
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('listaSerieHistorica'    =>true,
									 'removerSerieHistorica'  => true,
									 'bloquearSerieHistorica' => true,
									 'exportarsehcsv'         => true,
									 'exibeTelaJustificativa'  => true,
									 'salvarJustificativa' => true,
									 'atualizardatacoleta'    => true,
									 'importarTerritorios'	  => true,
									 'popupImportarTerritorios' => true,
									 'executarFormulaSH' 	  => true);
	break;
}
/* Fim Segurança : Validando os acessos as funções autorizadas */
//listaSerieHistorica();
if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
		exit;
	}
}

// monta cabeçalho
include APPRAIZ . "includes/cabecalho.inc";
echo "<br />";

$db->cria_aba($abacod_tela,$url,'');
monta_titulo("Série Histórica", "&nbsp;");
cabecalhoIndicador($_SESSION['indid']);

$dadosindicador = $db->pegaLinha("SELECT * FROM painel.indicador WHERE indid='".$_SESSION['indid']."'");

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="./js/painel.js"></script>
<script>
function excluirSerieHistorica(sehid) {
	var conf = confirm("Deseja realmente excluir esta série historica?");
	if(conf) {
		ajaxatualizar('requisicao=removerSerieHistorica&sehid='+sehid,'');
		ajaxatualizar('requisicao=listaSerieHistorica','listaSerieHistorica');
	}
}
function alterarBloqueio(sehid, situacao) {
	if(situacao == 'nao') {
		var conf = confirm("Deseja realmente desbloquear a serie historica?");
	} else {
		var conf = confirm("Deseja realmente bloquear a serie historica?");
	}
	if(conf) {
		ajaxatualizar('requisicao=bloquearSerieHistorica&sehid='+sehid+'&situacao='+situacao,'listaSerieHistorica');
		ajaxatualizar('requisicao=listaSerieHistorica','listaSerieHistorica');
	}
}

function exportarsehcsv(sehid) {
	window.location='painel.php?modulo=principal/listaSerieHistorica&acao=A&requisicao=exportarsehcsv&sehid='+sehid;
}

function atualizardatacoleta() {
	
	var pagina = "painel.php?modulo=principal/listaSerieHistorica&acao=A&requisicao=exibeTelaJustificativa";
	
	janela(pagina,"800","400","Justificativa");
	
	/*
	var conf = confirm("Deseja realmente declarar que não existem novas atualizações para este indicador?");
	if(conf) {
		ajaxatualizar('requisicao=atualizardatacoleta','');
		ajaxatualizar('requisicao=listaSerieHistorica','listaSerieHistorica');
		alert("Serie historica atualizada com sucesso");
	}*/
}
</script>
<?php
/*
 * Carregando permissões do perfil
 */
$permissoes = verificaPerfilPainel();

if($permissoes['verindicadores'] == 'vertodos') {
	$acesso = true;
} else {
	$acesso = validaAcessoIndicadores($permissoes['verindicadores'], $_SESSION['indid']);
}

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita">
	&nbsp;
	<? if($acesso) : ?>
	<?php if($dadosindicador['indshformula'] == "t"): ?>
		<?php 
			$formulash = $dadosindicador['formulash'];
			echo "<b>Fórmula: </b>".$formulash."<br/>";
			if($formulash){
				$hdn_indid = explode("{Indicador:",str_replace(array(" ","-","+","*"),array("","","",""),$formulash));
				if($hdn_indid){
						foreach($hdn_indid as $i){
							if($i){
								$fim = strpos($i,"}");
								$valor = substr($i,0,$fim);
								$dado = explode("_",$valor);
								if($dado[1]){
									$sql = "select dpeid from painel.detalheperiodicidade where dpedsc = '".$dado[1]."'";
									$dpeid__ = $db->pegaUm($sql);
									if($dpeid__){
										$arrIndFormula[] = $dado[0]."_".$dpeid__;
									}
								}else{
									$arrIndFormula[] = $dado[0];
								}
							}
						}
					}
			}
			if($arrIndFormula){
				$arrIndUsados = array();
				foreach($arrIndFormula as $indF){
					if(strstr($indF,"_") && !in_array($indF,$arrIndUsados)){
						$data = explode("_",$indF);
						$ind_ = $data[0];
						$dpeid_ = $data[1];
						$sql = "select 
									indnome
								from 
									painel.indicador ind
								where 
									indid = '$ind_'";
						$indnomeF = $db->pegaUm($sql);
						$sql = "select 
									dpedsc
								from 
									painel.detalheperiodicidade
								where 
									dpeid = '$dpeid_'";
						$dpedsc = $db->pegaUm($sql);
						echo "<p><b>Indicador: $ind_</b> - $indnomeF de $dpedsc</p>";
						$arrIndUsados[] = $ind_;
					}else{
						if($indF && !in_array($indF,$arrIndUsados)){
							$sql = "select 
									indnome
								from 
									painel.indicador ind
								where 
									indid = '$indF'";
							$indnomeF = $db->pegaUm($sql);
							echo "<p><b>Indicador: $indF</b> - $indnomeF - Padrão</p>";
							$arrIndUsados[] = $indF;
						}
					}
				}
			}
			$sql = "select 
						min(dpedatainicio) as min,
						max(dpedatafim) as max
					from 
						painel.detalheperiodicidade dpe
					inner join
						painel.seriehistorica seh ON seh.dpeid = dpe.dpeid
					where
								
						seh.indid in (".implode(",",$arrIndUsados).");";
			$arrPer = $db->pegaLinha($sql);
			
			$sql = "select
						dpeid as codigo,
						dpedsc as descricao
					from
						painel.detalheperiodicidade
					where
						perid = ".$dadosindicador['perid']."
					and
						dpestatus = 'A'
					and
						dpedatainicio between '{$arrPer['min']}' and '{$arrPer['max']}'
					order by
						dpeanoref,dpeordem";
			
			$db->monta_combo("dpeid",$sql,"S","Selecione o Período...","","");
		?>
		<input type="button" value="Executar Fórmula" name="btn_atualizar" onclick="executarFormulaSH();">
	<?php else: ?>
		<?php if($dadosindicador['regid'] == REGIONALIZACAO_SETOR): ?>
			<input type="button" value="Importar Dados do Setor" name="botao" onclick="popupImportarTerritorios();">
		<?php endif; ?>
			<input type="button" value="Sem novas atualizações" name="botao" onclick="atualizardatacoleta();">
			<input type="button" value="Incluir dados" name="botao" onclick="window.location='?modulo=principal/preenchimentoSerieHistorica&acao=A';">
	<?php endif; ?>
	<? endif; ?>
	
	</td>
</tr>
</table>
<div id="listaSerieHistorica"></div>

<?php
unset($_POST['requisicao']);
if($_POST) {
	foreach($_POST as $indice => $valor) $param[] = "$indice=$valor";
}
?>
<script>
function ordena(ordem, direcao) {document.formlista.requisicao.value='';document.formlista.ordemlista.value=ordem;document.formlista.ordemlistadir.value=direcao;document.formlista.submit();} 
function pagina(numero) {document.formlista.requisicao.value='';document.formlista.numero.value=numero;document.formlista.submit();}

document.observe("dom:loaded", function() {
  // initially hide all containers for tab content
  ajaxatualizar('requisicao=listaSerieHistorica<?=(($param)?"&".implode("&",$param):"") ?>','listaSerieHistorica');
});

function executarFormulaSH()
{
	var dpeid = jQuery("[name='dpeid']").val();
	if(!dpeid){
		alert('Selecione o Período.');
	}else{
		jQuery("[name='btn_atualizar']").attr("disabled",true);
		jQuery("[name='btn_atualizar']").val("Aguarde... Executando");
		jQuery.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicao=executarFormulaSH&dpeid=" + dpeid + "&indid=<?php echo $dadosindicador['indid'] ?>",
		   success: function(msg){
		   		//jQuery('#listaSerieHistorica').html( msg );
		   		jQuery("[name='btn_atualizar']").val("Executar Fórmula");
		   		jQuery("[name='btn_atualizar']").attr("disabled",false);
		   		ajaxatualizar('requisicao=listaSerieHistorica<?=(($param)?"&".implode("&",$param):"") ?>','listaSerieHistorica');
		   }
		 });
	}
}

function popupImportarTerritorios()
{
	var pagina = "painel.php?modulo=principal/listaSerieHistorica&acao=A&requisicao=popupImportarTerritorios&indid=<?php echo $dadosindicador['indid'] ?>";
	
	janela(pagina,"800","400","Importar Dados");
}
</script>
