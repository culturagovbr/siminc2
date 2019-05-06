<?php
require_once APPRAIZ . "monitora/www/planotrabalhoUN/_constantes.php";
require_once APPRAIZ . "monitora/www/planotrabalhoUN/_funcoes.php";

boUnidadesObrigatorias();

if($_REQUEST['requisicao'] == 'vincular'){
	require_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";
	require_once APPRAIZ . "monitora/classes/Pi_PlanoInternoHistorico.class.inc";

	extract($_POST);
	
	$arPliId = explode(",",$arPliId);
	$retorno = false;
	$arPliIdSemPtres = array();
	foreach($arPliId as $pliid){
		$sql = "SELECT ptres FROM monitora.pi_planointernoptres pip
							INNER JOIN monitora.ptres p on pip.ptrid = p.ptrid
						WHERE pip.pliid = $pliid";
		$ptres = $db->pegaUm($sql);
		
		if($ptres){
			$obPi_PlanoInterno = new Pi_PlanoInterno($pliid);
			$obPi_PlanoInterno->plisituacao = $situacao;
			$obPi_PlanoInterno->salvar();
			$sql = "SELECT plicod FROM monitora.pi_planointernohistorico WHERE pliid = $pliid ORDER BY pihdata DESC LIMIT 1";
			if(!$plicodOrigem = $db->pegaUm($sql)){
				$plicodOrigem	= $obPi_PlanoInterno->plicod;	
			}
			
			$obPi_PlanoInternoHistorico = new Pi_PlanoInternoHistorico();
			$obPi_PlanoInternoHistorico->pliid 		  = $pliid;
			$obPi_PlanoInternoHistorico->usucpf 	  = $_SESSION['usucpf'];
			//$obPi_PlanoInternoHistorico->pihdata 	  = date('Y-m-d H:i:s');
			$obPi_PlanoInternoHistorico->pihsituacao  = $situacao;
			$obPi_PlanoInternoHistorico->plicod 	  = $obPi_PlanoInterno->plicod;
			$obPi_PlanoInternoHistorico->plicodorigem = $plicodOrigem;
			$obPi_PlanoInternoHistorico->salvar();
			
			if($obPi_PlanoInternoHistorico->commit()){
				enviaEmailStatusPi($pliid);
				$retorno = true;
			}
		} else {
			array_push($arPliIdSemPtres, $pliid);
		}
		unset($obPi_PlanoInterno);
		unset($obPi_PlanoInternoHistorico);
	}
	
	if($arPliIdSemPtres && $arPliIdSemPtres[0]){
		$ptres = implode(', ',$arPliIdSemPtres);
		
		$sql = "SELECT plicod ||' - '||plititulo as planointerno FROM monitora.pi_planointerno WHERE pliid in ($ptres)";
		$arPi = $db->carregar($sql);
		$count = 0;
		foreach($arPi as $pi){
			$arPi[$count]['planointerno'] = utf8_encode($pi['planointerno']);
			$count++;
		}
		echo simec_json_encode($arPi);
		die;		
	}
	
	echo $retorno;
	die;
	
}

if ( $_REQUEST['submetido'] == 1 ){
	$arFiltro = array();
		
	if( $_REQUEST["plicod"] ){
		array_push($arFiltro, " UPPER(pi.plicod) ilike '%" . strtoupper($_REQUEST['plicod']) . "%' ");
	}
	
	if( $_REQUEST["plititulo"] ){
		array_push($arFiltro, " UPPER(pi.plititulo) ilike '%" . strtoupper($_REQUEST['plititulo']) . "%' ");
	}
	
	if( $_REQUEST["unidade"] ){
		array_push($arFiltro, " u.unicod = '" . $_REQUEST['unidade'] . "' ");
	}
	
	if( $_REQUEST["enquadramento"] ){
		array_push($arFiltro, " pi.eqdid = '" . $_REQUEST['enquadramento'] . "' ");
	}

	if( $_REQUEST["nivel"] ){
		array_push($arFiltro, " pi.secid = '" . $_REQUEST['nivel'] . "' ");
	}
	
	if( $_REQUEST["apropriacao"] ){
		array_push($arFiltro, " pi.capid = '" . $_REQUEST['apropriacao'] . "' ");
	}
	
	if( $_REQUEST["modalidade"] ){
		array_push($arFiltro, " pi.arceid = '" . $_REQUEST['modalidade'] . "' ");
	}
	
	if( $_REQUEST["plisituacao"] ){
		array_push($arFiltro, " pi.plisituacao ilike '%" . $_REQUEST['plisituacao'] . "%' ");
	}
	
} 


//faz o controle do registro atual para a paginação
$numero = !$_REQUEST['numero'] ? 1 : intval( $_REQUEST['numero'] );
//Registros por página
$perpage = 100;
//Qtde de Paginas que serão mostradas na barrinha
$pages = 50;

include APPRAIZ."includes/cabecalho.inc";

echo '<br/>';

// Cria o título da tela
$titulo_modulo = "Gerenciar Plano Interno";

if($db->testa_superuser()){
	$unidade = "";
} else {
	$unidade = carrega_unidade_titulo();
	
}

monta_titulo( $titulo_modulo, $unidade);

$stWhere = "";
if(!$db->testa_superuser()){
		$arUnicodTemp = perfil_unidade();
		$arUnicodTemp = ($arUnicodTemp) ? $arUnicodTemp : array();
		if($arUnicodTemp[0]){
			$arUnicod = array();
			foreach($arUnicodTemp as $uniid){
				if($uniid['unicod']){
					array_push($arUnicod,$uniid['unicod']);								
				}
			}
			if($arUnicod && $arUnicod[0]){
				$unicodTemp = "'" . implode("', '",$arUnicod) . "'";
				if( $unicodTemp ){
					$stWhere = " AND u.unicod in(" . $unicodTemp . ") ";
				}
			}
		}
}

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="formulario1" name="formulario1" method="post" action="">
	<input type="hidden" name="submetido" id="submetido" value="1"/>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita">Código do PI</td>
			<td>
				<?php 
					$plicod = $_REQUEST['plicod'];
					echo campo_texto( 'plicod', 'N', 'S', '', 20, 15, '', '', 'left', '', 0);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Título</td>
			<td>
				<?php
					$plititulo = $_REQUEST['plititulo'];
					echo campo_texto( 'plititulo', 'N', 'S', '', 65, 60, '', '', 'left', '', 0);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Unidade</td>
			<td>
				<?php
					$unidade = $_REQUEST['unidade'];
					$sql = "SELECT u.unicod as codigo, u.unicod ||' - '|| u.unidsc as descricao FROM public.unidade u WHERE orgcod = '". CODIGO_ORGAO_SISTEMA. "' $stWhere ORDER BY unicod";
					$db->monta_combo('unidade', $sql, 'S',  'Selecione', '', '', '', '', 'N'); 
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Enquadramento da Despesa:</td>
		    <td>
				<?php
					$enquadramento = $_REQUEST['enquadramento'];
					$sql = "SELECT eqdid as codigo, eqdcod ||' - '|| eqddsc as descricao FROM planejamento.enquadramento_despesa WHERE eqdano = '{$_SESSION['exercicio']}' ORDER BY eqdcod";
					$db->monta_combo('enquadramento', $sql, 'S',  'Selecione', '', '', '', '', 'N'); 
				?>    	
		    </td>
		</tr>
		<tr>
			<td class='SubTituloDireita'>Nível/Etapa de Ensino:</td>
			<td>
				<?php
					$nivel = $_REQUEST['nivel'];
					$sql = "SELECT secid as codigo, neecod ||' - '|| needsc as descricao FROM monitora.pi_niveletapaensino WHERE neeano = '{$_SESSION['exercicio']}' ORDER BY neecod ";
					$db->monta_combo('nivel', $sql, 'S', 'Selecione', '', '', '', '240', 'N', '');
				?>
			</td>
		</tr>
		<tr>
			<td class='SubTituloDireita'>Categoria de Apropriação:</td>
			<td>
				<?php
					$apropriacao = $_REQUEST['apropriacao'];
					$sql = "SELECT capid as codigo, capcod ||' - '|| capdsc as descricao FROM planejamento.categoria_apropriacao WHERE capano = '{$_SESSION['exercicio']}' ORDER BY capcod ";
					$db->monta_combo('apropriacao', $sql, 'S', 'Selecione', '', '', '', '340', 'N', '');
				?>
			</td>
		</tr>	
		<tr>
			<td align='right' class="SubTituloDireita">Modalidade de Ensino:</td>
		    <td>
				<?php
					$modalidade = $_REQUEST['modalidade'];
					$sql = "SELECT arceid as codigo, arccod ||' - '|| arcdsc as descricao FROM planejamento.area_cultural WHERE arcano = '{$_SESSION['exercicio']}'";
					$db->monta_combo('modalidade', $sql, 'S', 'Selecione', '', '', '', '240', 'N', '');
		    	 ?>
		    </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Situação</td>
			<td>
				<input type="radio" name="plisituacao" id="situacao_qualquer" value="" <?php if($_REQUEST['plisituacao'] == '') echo 'checked'; ?>> Qualquer
				<input type="radio" name="plisituacao" id="situacao_enviado" value="A"<?php if($_REQUEST['plisituacao'] == 'A') echo 'checked'; ?>> Aprovado
				<input type="radio" name="plisituacao" id="situacao_enviado" value="E" <?php if($_REQUEST['plisituacao'] == 'E') echo 'checked'; ?>> Enviado para Revisão
				<input type="radio" name="plisituacao" id="situacao_pendente" value="P" <?php if($_REQUEST['plisituacao'] == 'P') echo 'checked'; ?>> Pendente
				<input type="radio" name="plisituacao" id="situacao_siafi" value="C" <?php if($_REQUEST['plisituacao'] == 'C') echo 'checked'; ?>> Cadastrado no SIAFI
				<input type="radio" name="plisituacao" id="situacao_revisado" value="R" <?php if($_REQUEST['plisituacao'] == 'R') echo 'checked'; ?>> Revisado
				<input type="radio" name="plisituacao" id="situacao_confirmadosiafi" value="S" <?php if($_REQUEST['plisituacao'] == 'S') echo 'checked'; ?>> Confirmado no SIAFI
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td>
				<?php if ( $_REQUEST['submetido'] == 1 ){ ?>
					<div id="mdtodos"><input type="checkbox" name="chktodos" id="chktodos" onclick="marcatodas()"><a href="#" onclick="marcatodas()">Marcar todos</a></div>
				<?php }?>
			</td>
			<td>
				<div style="float: left;">
					<input type="submit" value="Pesquisar" style="cursor: pointer;"/>
					<input type="button" value="Ver Todos" style="cursor: pointer;" onclick="location.href='?modulo=principal/planointerno/gerenciarpi&acao=A&submetido=1';"/>
				</div>
			</td>
		</tr>
	</table>
</form>


<?php if ( $_REQUEST['submetido'] == 1 ){ ?>
<form id="formulario" name="formulario" method="post" action="">
	<input type=hidden name="numero" value="1">
	
	<!-- Para manter os valores do formulario de pesquisa quando usar a paginação -->
	<input type=hidden name="submetido" 	value="1">
	<input type=hidden name="unidade"		value="<?=$_REQUEST["unidade"]?>">
	<input type=hidden name="plicod" 		value="<?=$_REQUEST["plicod"]?>">
	<input type=hidden name="plititulo" 	value="<?=$_REQUEST["plititulo"]?>">
	<input type=hidden name="enquadramento" value="<?=$_REQUEST["enquadramento"]?>">
	<input type=hidden name="nivel"			value="<?=$_REQUEST["nivel"]?>">
	<input type=hidden name="apropriacao" 	value="<?=$_REQUEST["apropriacao"]?>">
	<input type=hidden name="modalidade" 	value="<?=$_REQUEST["modalidade"]?>">
	<input type=hidden name="plisituacao" 	value="<?=$_REQUEST["plisituacao"]?>">
	
	<center>
		<div id="valida" style="overflow:auto; width:95%; height:230px; border-bottom:2px solid #cccccc;">
			<?php 
				$sql = "
				SELECT  distinct
							'<center><input class=\"normal\" type=\"checkbox\" id=\"plicod[' || pi.pliid || ']\"  name=\"plicod\" value=\"' || pi.pliid || '\" onclick=\"retorna(\'' || pi.pliid || '\');\"></center>' as acao,
							'<a title=\"Visualizar Dados\" style=\"cursor:pointer;\" onclick=\"mostrahistoricoplanointerno(\'' || pi.pliid || '\');\">' || pi.plicod || '</a>',
							CASE WHEN trim(pi.plititulo) is not null THEN 
								pi.plititulo  || '<input type=\"hidden\" id=\"plititulo[' || pi.pliid || ']\" value=\"' || pi.plicod || ' - ' || pi.plititulo || '\">'
							ELSE 
								'Não Preenchido<input type=\"hidden\" id=\"plititulo[' || pi.pliid || ']\" value=\" ' || pi.plicod ||' - Não Preenchido\"/>' 
							END as titulo,
                            unidsc,
                            to_char(pi.plidata, 'DD/MM/YYYY') as plidata,
                            CASE WHEN pi.plisituacao = 'P' THEN  ' Pendente '
                                WHEN pi.plisituacao = 'A' THEN ' Aprovado ' 
                                WHEN pi.plisituacao = 'R' THEN ' Revisado ' 
                                WHEN pi.plisituacao = 'C' THEN ' Cadastrado no SIAFI ' 
                                WHEN pi.plisituacao = 'E' THEN ' Enviado para Revisão ' 
                                WHEN pi.plisituacao = 'S' THEN ' Confimado no SIAFI ' 
                            END as situacao,
                            to_char(max(pih.pihdata), 'DD/MM/YYYY') as plidata_alteracao,
                           coalesce(pt.valor,0) as valor
                        FROM
                            monitora.pi_planointerno pi
                        	LEFT JOIN monitora.pi_subacao sa ON sa.sbaid = pi.sbaid                    	
                        	LEFT JOIN monitora.pi_obra o ON o.pliid = pi.pliid
                        	LEFT JOIN obras.obrainfraestrutura obr ON obr.obrid = o.obrid
                        	LEFT JOIN entidade.entidade ent ON ent.entid = obr.entidunidade
                        	LEFT JOIN monitora.pi_planointernoatividade pia ON pia.pliid = pi.pliid
                        	LEFT JOIN pde.atividade ati ON ati.atiid = pia.atiid
                        	LEFT JOIN entidade.entidade e ON e.entid = ati.entid
                        	LEFT JOIN monitora.pi_planointernohistorico pih ON pi.pliid = pih.pliid
                        	LEFT JOIN monitora.pi_subacaounidade sbu ON sbu.sbaid = sa.sbaid
                        	LEFT JOIN public.unidade u ON u.unicod = pi.unicod
                        	LEFT JOIN ( select pliid, sum(pipvalor) as valor from monitora.pi_planointernoptres group by pliid ) pt ON pt.pliid = pi.pliid 
                       WHERE pi.plistatus = 'A' " . ( !empty($arFiltro) ? "AND" . implode(" AND ", $arFiltro) : '' ) . " 
                       AND u.orgcod = '". CODIGO_ORGAO_SISTEMA. "'
                       AND pi.pliano = '{$_SESSION['exercicio']}'
                       $stWhere
                       GROUP BY pi.pliid,pi.plicod,pi.plititulo,u.unidsc,pi.plidata,pi.plisituacao,pi.obrid,ent.entnome, pt.valor
                       ORDER BY plidata";
				//ver($sql);
				$cabecalho = array("Ação", "Código", "Título", "Descrição", "Data", "Situação", "Data alteração", "Valor");
				$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%', 'N' );
				
				$arDados = $db->carregar($sql);
				$arDados = ($arDados) ? $arDados : array();
				$count = 1;
				$situacao_anterior = "";
				$boPodeMarcarTodos = 1;
				foreach($arDados as $dados){
					if($dados['situacao'] != $situacao_anterior && $count != 1){
						$boPodeMarcarTodos = 2;
						break;
					}
					$situacao_anterior = $dados['situacao'];
					$count++;
				}
				$nlinhas = count($arDados);
				$nl = !$arDados ? 0 : $nlinhas;
				$reg_fim = (($numero + $perpage) > $nlinhas) ? $nlinhas : ($numero + $perpage -1);
				$total_reg = $nlinhas;
			?>
		</div>
	</center>
	<input type=hidden name="boPodeMarcarTodos" id="boPodeMarcarTodos" value="<?php echo $boPodeMarcarTodos;?>">
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"	align="center">
		<tr>
			<td colspan="2">
				<select multiple size="6" name="selectpliid[]" id="selectpliid" style="width:100%;" class="CampoEstilo" onchange="moveto(this);">
					<option value="">-- Selecione as Subações na Listagem Acima --</option>
				</select>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td align="left">
			<?php if($_REQUEST["plisituacao"] == 'P' || $_REQUEST["plisituacao"] == 'R'){ ?>
				<input type="button" value="Aprovar" onclick="vincular('A')" style="cursor: pointer;"/>
				<input type="button" value="Enviar para Revisão" onclick="vincular('E')" style="cursor: pointer;"/>
			<?php } ?>
			</td>
		</tr>
		<tr>
			<td colspan="3" align="right" bgcolor="#dcdcdc" style="padding:1px;">
				<?php include APPRAIZ . "includes/paginacao.inc"; ?>
			</td>
		</tr>
	</table>
</form>

<?}?>

<script>

	function retorna(objeto){
		var campoSelect = document.formulario.selectpliid;
		var checado = document.getElementById('plicod['+objeto+']');
		var titulo  = document.getElementById('plititulo['+objeto+']');
		
		tamanho = campoSelect.options.length;
		if ( campoSelect.options[0].value == ''){ tamanho--; }
		if ( checado.checked == true ){
			campoSelect.options[tamanho] = new Option( titulo.value, checado.value, false, false );
			sortSelect(campoSelect);
		}
		else {
			for( var i=0; i<=campoSelect.length-1; i++ ){
				if ( checado.value == campoSelect.options[i].value ){
					campoSelect.options[i] = null;
				}
			}
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('-- Selecione os Planos Internos na Listagem Acima --', '', false, false);}
			sortSelect(campoSelect);
		}
	}
	
	function moveto(obj) {
		document.getElementById('plicod[' + obj.value + ']').focus();
	}
	
	function marcatodas(){
		var boPodeMarcarTodos = $('boPodeMarcarTodos').value;

		if($('situacao_qualquer').checked == true || boPodeMarcarTodos == 2){
			alert('Favor filtrar por alguma situação');
			$('chktodos').checked = false;
			return false;
		}
		
		document.getElementById('mdtodos').innerHTML = '<input type="checkbox" name="chktodos" id="chktodos" onclick="desmarcatodas()"><a href="#" onclick="desmarcatodas()">Desmarcar todos</a>';
		document.getElementById('chktodos').checked = true;
		
		var plicod = document.getElementsByName( 'plicod' );
		
		for( var i=0; i <= plicod.length-1; i++ ){
			if ( plicod[i].checked == false && plicod[i].value != "" ){
				plicod[i].checked = true;
				retorna(plicod[i].value);
			}
		}
	}
	
	function desmarcatodas(){
		
		document.getElementById('mdtodos').innerHTML = '<input type="checkbox" name="chktodos" id="chktodos" onclick="marcatodas()"> <a href="#" onclick="marcatodas()">Marcar todos</a>';
		document.getElementById('chktodos').checked = false;
		
		var plicod = document.getElementsByName( 'plicod' );
		
		for( var i=0; i <= plicod.length-1; i++ ){
			if ( plicod[i].checked == true ){
				plicod[i].checked = false;
				retorna(plicod[i].value);
			}
		}
	}

	function pagina( numero ){
	 	document.formulario.numero.value = numero;
	 	document.formulario.submit();
 	}

	function mostrahistoricoplanointerno( pliid ){
		window.open('?modulo=principal/planotrabalhoUN/popuphistoricoplanointernoUN&acao=A&pliid='+pliid,'','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=500');
 	}
 	
	function vincular(situacao){
		var pliid = document.getElementsByName( 'plicod' );

		var arPliId = new Array();
		for( var i=0; i <= pliid.length-1; i++ ){
			if ( pliid[i].checked == true ){
				arPliId.push(pliid[i].value);
			}
		}

	 	var url = window.location.href;
		var parametros = "requisicao=vincular&arPliId="+arPliId+'&situacao='+situacao ;
		var myAjax = new Ajax.Request(
			url,
			{
				method: 'post',
				parameters: parametros,
				asynchronous: false,
				onComplete: function(r) {
					//$('dibDegug').update(r.responseText);
					if(r.responseText == 1){
						alert('Dados gravados com Sucesso.');
						// feito isso por causa da presa.
						document.formulario1.submit();
					} else if(r.responseText){
						var arDados = eval('(' + r.responseText + ')');
						if(arDados.length){
							var planointerno_sem_ptres = "";
							for (var j = 0; j < arDados.length; j++) {
								planointerno_sem_ptres += arDados[j].planointerno+'\n';
							}
							planointerno_sem_ptres = "Os seguintes PIs não poderam ser 'aprovados / enviados para revisão'\npois não exitem PTRES vinculados.\n\n"+planointerno_sem_ptres;
							alert(planointerno_sem_ptres);
							return false;
						}
					}
				}
			}
		);
	 	
 	}

</script>
<div id="dibDegug"></div>