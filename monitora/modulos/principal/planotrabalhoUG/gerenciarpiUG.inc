<?php

include "planotrabalhoUG/_constantes.php";
include "planotrabalhoUG/_funcoes.php";

if($_REQUEST['requisicao'] == 'vincular'){
	require_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";
	require_once APPRAIZ . "monitora/classes/Pi_PlanoInternoHistorico.class.inc";

	extract($_POST);
	
	$arPliId = explode(",",$arPliId);
	$retorno = false;
	foreach($arPliId as $pliid){
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
		unset($obPi_PlanoInterno);
		unset($obPi_PlanoInternoHistorico);
	}
	
	echo $retorno;
	die;
	
}

if ( $_REQUEST['submetido'] == 1 ){
	$arFiltro = array();
	$filtroProgUnidade = "";
		
	//ver($_REQUEST,d);
	if( $_REQUEST["plicod"] ){
		array_push($arFiltro, " UPPER(pi.plicod) ilike '%" . strtoupper($_REQUEST['plicod']) . "%' ");
	}
	
	if( $_REQUEST["plititulo"] ){
		array_push($arFiltro, " UPPER(pi.plititulo) ilike '%" . strtoupper($_REQUEST['plititulo']) . "%' ");
	}
	
	if( $_REQUEST["unidade"] ){
		array_push($arFiltro, " u.ungcod = '" . $_REQUEST['unidade'] . "' ");
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
	
	if( $_REQUEST["prog_unidade"] ){
		$filtroProgUnidade = " and ptres.unicod = '" . $_REQUEST['prog_unidade'] . "' ";
	}
	
	if ($_REQUEST["valor"] == "1") {
		array_push($arFiltro, " valor IS NOT NULL ");
		
	}
	
	$_REQUEST["ptres"] = ($_REQUEST["ptres"]) ? $_REQUEST["ptres"] : array();	
	if( $_REQUEST["ptres"][0] ){
		array_push($arFiltro, " ptres.ptrid in ('" . implode( "','", $_REQUEST["ptres"] ) . "') ");
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
monta_titulo( $titulo_modulo, "");

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
					$plititulo = $_POST['plititulo'];
					echo campo_texto( 'plititulo', 'N', 'S', '', 65, 60, '', '', 'left', '', 0);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Unidade</td>
			<td>
				<?php
					$unidade = $_REQUEST['unidade'];
					$sql = "SELECT 
								ungcod AS codigo, CASE WHEN (unicod = '26101') THEN ungcod ELSE unicod END ||' - '|| ungdsc as descricao FROM public.unidadegestora";  
					$db->monta_combo('unidade', $sql, 'S',  'Selecione', '', '', '', '', 'N'); 
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Enquadramento da Despesa:</td>
		    <td>
				<?php
					$enquadramento = $_REQUEST['enquadramento'];
					$sql = "SELECT eqdid as codigo, eqdcod ||' - '|| eqddsc as descricao FROM planejamento.enquadramento_despesa WHERE eqdano = '{$_SESSION['exercicio']}' ORDER BY eqddsc";
					$db->monta_combo('enquadramento', $sql, 'S',  'Selecione', '', '', '', '', 'N'); 
				?>    	
		    </td>
		</tr>
		<tr>
			<td class='SubTituloDireita'>Nível/Etapa de Ensino:</td>
			<td>
				<?php
					$nivel = $_REQUEST['nivel'];
					$sql = "SELECT secid as codigo, neecod ||' - '|| needsc as descricao FROM planejamento.segmento_cultural WHERE neeano = '{$_SESSION['exercicio']}' ORDER BY neecod ";
					$db->monta_combo('nivel', $sql, 'S', 'Selecione', '', '', '', '240', 'N', '');
				?>
			</td>
		</tr>
		<tr>
			<td class='SubTituloDireita'>Categoria de Apropriação:</td>
			<td>
				<?php
					$apropriacao = $_REQUEST['apropriacao'];
					$sql = "SELECT capid as codigo, capcod ||' - '|| capdsc as descricao FROM planejamento.categoria_apropriacao WHERE capano = '{$_SESSION['exercicio']}' ORDER BY capcod";
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
					//$modensino = $modid;
					$db->monta_combo('modalidade', $sql, 'S', 'Selecione', '', '', '', '240', 'N', '');
		    	 ?>
		    </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Programação da Unidade:</td>
		    <td>
				<?php
					$ar = array(
								array('codigo'=>AD,'descricao'=>AD.' - Administração Direta'),
								array('codigo'=>INEP,'descricao'=>INEP.' - Instituto Nacional de Estudos e Pesquisas Educacionais Anísio Teixeira'),
								array('codigo'=>CAPES,'descricao'=>CAPES.' - Coordenação de Aperfeiçoamento de Pessoal de Nível Superior'),
								array('codigo'=>FNDE,'descricao'=>FNDE.' - Fundo Nacional de Desenvolvimento da Educação')
						);
					$prog_unidade = $_REQUEST['prog_unidade'];
					$db->monta_combo('prog_unidade', $ar, 'S', 'Selecione', '', '', '', '240', 'N', '');
		    	 ?>
		    </td>
		</tr>
		<tr>
				<td class="SubTituloDireita" valign="top">PTRES:</td>
			    <td>
					<?
					if($_POST['ptres'][0]) {
						$ptres = $db->carregar("SELECT
													pt.ptrid as codigo,
													'(PTRES:'||pt.ptres||') - '|| aca.unicod ||'.'|| aca.prgcod ||'.'|| aca.acacod ||' - '|| aca.acadsc as descricao
												  FROM planejamento.acao aca 
												  	inner join monitora.ptres pt on pt.acaid=aca.acaid  
												WHERE aca.prgano='".$_SESSION['exercicio']."' and aca.acasnrap = false
												and aca.unicod IN('".AD."','".CAPES."','".INEP."','".FNDE."')
												and pt.ptrid in (".implode(",", $_POST['ptres']).")
												GROUP BY codigo , descricao
												ORDER BY 
													1");
					}
					$sqlComboAcoes = "SELECT
										pt.ptrid as codigo,
										'(PTRES:'||pt.ptres||') - '|| aca.unicod ||'.'|| aca.prgcod ||'.'|| aca.acacod ||' - '|| aca.acadsc as descricao
									  FROM planejamento.acao aca 
									  	inner join monitora.ptres pt on pt.acaid=aca.acaid
									  WHERE aca.prgano='".$_SESSION['exercicio']."' and aca.acasnrap = false
									  	and aca.unicod IN('".AD."','".CAPES."','".INEP."','".FNDE."')
									  GROUP BY codigo , descricao
									  ORDER BY 
										1";
					combo_popup( "ptres", $sqlComboAcoes, "Ações", "600x600", 0, array(), "", "S", false, false, 5, 400 );
					?>    	
			    </td>
			</tr>
		<tr>
			<td class="SubTituloDireita">Situação:</td>
			<td>
				<label><input type="radio" name="plisituacao" id="situacao_qualquer" value="" <?php if($_REQUEST['plisituacao'] == '') echo 'checked'; ?>> Qualquer</label>
				<label><input type="radio" name="plisituacao" id="situacao_enviado" value="A"<?php if($_REQUEST['plisituacao'] == 'A') echo 'checked'; ?>> Aprovado</label>
				<label><input type="radio" name="plisituacao" id="situacao_enviado" value="E" <?php if($_REQUEST['plisituacao'] == 'E') echo 'checked'; ?>> Enviado para Revisão</label>
				<label><input type="radio" name="plisituacao" id="situacao_pendente" value="P" <?php if($_REQUEST['plisituacao'] == 'P') echo 'checked'; ?>> Pendente</label>
				<label><input type="radio" name="plisituacao" id="situacao_siafi" value="C" <?php if($_REQUEST['plisituacao'] == 'C') echo 'checked'; ?>> Cadastrado no SIAFI</label>
				<label><input type="radio" name="plisituacao" id="situacao_revisado" value="R" <?php if($_REQUEST['plisituacao'] == 'R') echo 'checked'; ?>> Revisado</label>
				<label><input type="radio" name="plisituacao" id="situacao_homologado" value="H" <?php if($_REQUEST['plisituacao'] == 'H') echo 'checked'; ?>> Homologado</label>
				<label><input type="radio" name="plisituacao" id="situacao_confirmadosiafi" value="S" <?php if($_REQUEST['plisituacao'] == 'S') echo 'checked'; ?>> Confirmado no SIAFI</label>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Valor:</td>
			<td>
				<label><input type="checkbox" name="valor" id="valor" value="1" <?php if($_REQUEST['valor'] == '1') echo 'checked'; ?>> Maior que zero</label>
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
					<input type="button" value="Pesquisar" style="cursor: pointer;" onclick="enviar()"/>
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
			
				$perfilGestorSubacao = possui_perfil(PERFIL_MONITORA_GESTORSUBACOES);
				$stWhere = "";

				if($perfilGestorSubacao){
					$ug = perfil_unidadegestora();
					$ug2 = implode( "','", $ug );
					$stWhere = "AND ptres.unicod in ('".$ug2."')"; 
				}
				
				if($filtroProgUnidade){
					$ptresPadrao = $filtroProgUnidade;
					$ptresPadrao2 = $filtroProgUnidade;
				} else {
					$ptresPadrao = "and ptres.unicod in ('".AD."','".CAPES."','".INEP."','".FNDE."','".FIES."','".SUPERVISAOMEC."')";
					$ptresPadrao2 = "";					
				}
				
				$sql = "
				SELECT  distinct
							'<center><input class=\"normal\" type=\"checkbox\" id=\"plicod[' || pi.pliid || ']\"  name=\"plicod\" value=\"' || pi.pliid || '\" onclick=\"retorna(\'' || pi.pliid || '\');\"></center>' as acao,
							'<a title=\"Visualizar Dados\" style=\"cursor:pointer;\" onclick=\"mostrahistoricoplanointerno(\'' || pi.pliid || '\');\">' || pi.plicod || '</a>',
							CASE WHEN trim(pi.plititulo) is not null THEN 
								pi.plititulo  || '<input type=\"hidden\" id=\"plititulo[' || pi.pliid || ']\" value=\"' || pi.plicod || ' - ' || pi.plititulo || '\">'
							ELSE 
								'Não Preenchido<input type=\"hidden\" id=\"plititulo[' || pi.pliid || ']\" value=\" ' || pi.plicod ||' - Não Preenchido\"/>' 
							END as titulo,
                            Case when trim(u.ungcod) <> '' then ungdsc else unidsc end as ungdsc,
                            to_char(pi.plidata, 'DD/MM/YYYY') as plidata,
                            CASE WHEN pi.plisituacao = 'P' THEN  ' Pendente '
                                WHEN pi.plisituacao = 'A' THEN ' Aprovado ' 
                                WHEN pi.plisituacao = 'R' THEN ' Revisado ' 
                                WHEN pi.plisituacao = 'C' THEN ' Cadastrado no SIAFI ' 
                                WHEN pi.plisituacao = 'H' THEN ' Homologado ' 
                                WHEN pi.plisituacao = 'E' THEN ' Enviado para Revisão ' 
                                WHEN pi.plisituacao = 'S' THEN ' Confimado no SIAFI '
                            END as situacao,
                            to_char(max(pih.pihdata), 'DD/MM/YYYY') as plidata_alteracao,
                            coalesce(pt.valor,0) as valor    
                        FROM
                            monitora.pi_planointerno pi
                        	INNER JOIN monitora.pi_subacao sa ON sa.sbaid = pi.sbaid
                        	LEFT JOIN monitora.pi_obra o ON o.pliid = pi.pliid
                        	LEFT JOIN obras.obrainfraestrutura obr ON obr.obrid = o.obrid
                        	LEFT JOIN entidade.entidade ent ON ent.entid = obr.entidunidade
                        	LEFT JOIN monitora.pi_planointernoatividade pia ON pia.pliid = pi.pliid
                        	LEFT JOIN monitora.pi_planointernoptres pip ON pip.pliid = pi.pliid
							LEFT JOIN monitora.ptres ptres ON ptres.ptrid = pip.ptrid
                        	LEFT JOIN pde.atividade ati ON ati.atiid = pia.atiid
                        	LEFT JOIN monitora.pi_planointernohistorico pih ON pi.pliid = pih.pliid 
                        	LEFT JOIN public.unidadegestora u ON u.ungcod = pi.ungcod 
                        	LEFT JOIN public.unidade un ON un.unicod = pi.unicod
                        	LEFT JOIN ( select pliid, sum(pipvalor) as valor from monitora.pi_planointernoptres group by pliid ) pt ON pt.pliid = pi.pliid 
                       WHERE 
	                       ( pi.ungcod IS NOT NULL or ( pi.unicod IS NOT NULL $ptresPadrao )) AND 
                       		 pi.plistatus = 'A' " . ( !empty($arFiltro) ? "AND" . implode(" AND ", $arFiltro) : '' ) . "
                       		 AND pi.pliano = '{$_SESSION['exercicio']}'
                       		 $ptresPadrao2
                       		 $stWhere
                       GROUP BY pi.pliid,pi.plicod,pi.plititulo,u.ungcod, u.ungdsc, unidsc,pi.plidata,pi.plisituacao,pi.obrid, pt.valor
                       ORDER BY plidata";

//ver($sql,d);
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
					<input type="button" value="Homologar" onclick="vincular('H')" style="cursor: pointer;"/>
					<input type="button" value="Enviar para Revisão" onclick="vincular('E')" style="cursor: pointer;"/>
				<?php } ?>
				<?php if($_REQUEST["plisituacao"] == 'H'){ ?>
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

	function enviar(){
		var form = document.getElementById("formulario1");
		selectAllOptions( form.ptres );		 		
 		form.submit();

	}

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
		window.open('?modulo=principal/planotrabalhoUG/popuphistoricoplanointernoUG&acao=A&pliid='+pliid,'','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=500');
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
					//alert(r.responseText);
					//$('dibDegug').update(r.responseText);
					//return;
					if(r.responseText){
						alert('Dados gravados com Sucesso.');
						// feito isso por causa da presa.
						document.formulario1.submit();
					}
				}
			}
		);
	 	
 	}

</script>
<div id="dibDegug"></div>