<?php

//include 'ConsultaFinanceiroUG.inc';
include "planotrabalhoUG/_constantes.php";
include "planotrabalhoUG/_funcoes.php";
include "planotrabalhoUG/_componentes.php";


$ano = $_SESSION['exercicio'];
$possuiPerfilConsultaUnidades = false;

$sql = "SELECT 
			pu.pflcod as perfil
		FROM 
			seguranca.perfilusuario pu 
		INNER JOIN 
			seguranca.perfil p ON p.pflcod = pu.pflcod AND p.sisid = ".$_SESSION["sisid"]." 
		WHERE 
			pu.usucpf = '".$_SESSION["usucpf"]."'";
$perfilUsuario = $db->carregar($sql);

// Verifica se o usuário possui somente perfil de "Consulta Unidade"
if((count($perfilUsuario) == 1) && ($perfilUsuario[0]["perfil"] == 175)) {
	$possuiPerfilConsultaUnidades = true;
}

if ( $_POST['pesquisa'] || $_GET['pesquisa'] ){
	ini_set("memory_limit","250M");
	set_time_limit(0);
	
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	//ver($_POST,d);
	
	/*if($_GET['boPainel']){
		$_POST['agrupador'] 		= array(0 => 'dcoconvenio');
		$_POST['muncod'] 			= array(0 => $_GET['muncod']);
		$_POST['muncod_campo_flag'] = true;
		$_POST['estuf'] 			= array(0 => $_GET['estuf']);
		$_POST['estuf_campo_flag'] = true;
		$_POST['desid'] 			= array(0 => $_GET['desid']);
		
		$sql = "select distinct dfimesanosaldo, dfidatasaldo from painel.dadosfinanceirosconvenios order by dfidatasaldo desc limit 1";
		$_POST['dfimesanosaldo'] = $db->pegaUm($sql);
		
	}*/
	
	$sql   = monta_sql();
	$dados = $db->carregar($sql);
	$agrup = monta_agp();
	$col   = monta_coluna();
	
	$r = new montaRelatorio();
	$r->setAgrupador($agrup, $dados);
	$r->setColuna($col);
	$r->setBrasao($true ? true : false);
	$r->setOverFlowTableHeight(500);
	$r->setTotNivel(true);
	$r->setEspandir(true);
	?>
	<html>
		<head>
			<script type="text/javascript" src="../includes/funcoes.js"></script>
			<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
			<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
			<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
			<script type="text/javascript" src="../includes/remedial.js"></script>
			<script type="text/javascript" src="../includes/superTitle.js"></script>
		</head>
	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
	
	<?php 
	echo $r->getRelatorio();	
	?>
	</body>
	</html>
	<?php 
	die;	
}

function monta_sql(){
	
	$where = array();
	
	extract($_POST);
	
	//ver($_POST,d);
	
	// Ano
	if( $ano ){
		array_push($where, " orc.rofano = '2010' ");
	}

	// Ug
	if( $ug ){
		array_push($where, " u.ungcod = '150028' ");
	}
	//atiid
	
	// Programa
	if( $programa[0] && $programa_campo_flag ){
		array_push($where, " orc.prgcod in ('" . implode( "','", $programa ) . "') ");
	}
	
	// Ação
	if( $acacod[0] && $acacod_campo_flag ){
		array_push($where, " orc.acacod in ('" . implode( "','", $acacod ) . "') ");
	}
	
	// PI
	if( $plicod[0] && $plicod_campo_flag ){
		array_push($where, " pli.plicod in ('" . implode( "','", $plicod ) . "') ");
	}
	
	// Subação
	if( $sbacod[0] && $sbacod_campo_flag ){
		array_push($where, " sba.sbacod in ('" . implode( "','", $sbacod ) . "') ");
	}
	
	// Fonte Siafi
	if( $fontesiafi[0] && $fontesiafi_campo_flag ){
		array_push($where, " f.foscod in ('" . implode( "','", $fontesiafi ) . "') ");
	}
	
	// Fonte Sof
	if( $fontesof[0] && $fontesof_campo_flag ){
		array_push($where, " fs.foncod in ('" . implode( "','", $fontesof ) . "') ");
	}
	
	// Fonte de Recurso
	if( $fonte[0] && $fonte_campo_flag ){
		array_push($where, " fr.codigo in ('" . implode( "','", $fonte ) . "') ");
	}
	
	// Categoria Econômica
	if( $catecon[0] && $catecon_campo_flag ){
		array_push($where, " cte.ctecod in ('" . implode( "','", $catecon ) . "') ");
	}
	
	// GND
	if( $gnd[0] && $gnd_campo_flag ){
		array_push($where, " gnd.gndcod in ('" . implode( "','", $gnd ) . "') ");
	}
	
	// Modalidade de Aplicação
	if( $mapcod[0] && $mapcod_campo_flag ){
		array_push($where, " ma.mapcod in ('" . implode( "','", $mapcod ) . "') ");
	}
	
	// Elemento de Despesa
	if( $elemento[0] && $elemento_campo_flag ){
		array_push($where, " ele.edpcod in ('" . implode( "','", $elemento ) . "') ");
	}
	
	// Natureza de Despesa
	if( $natureza[0] && $natureza_campo_flag ){
		array_push($where, " nd.ctecod || nd.gndcod || nd.mapcod || nd.edpcod in ('" . implode( "','", $natureza ) . "') ");
	}
	
	$sql = " select 
				orc.acacod as acacod, 
				orc.acadsc as acadsc, 
				cte.ctecod as ctecod, 
				cte.ctedsc as ctedsc, 
				ele.edpcod as edpcod, 
				ele.edpdsc as edpdsc, 
				frs.foscod as foscod, 
				frs.fosdsc as fosdsc, 
				fs.foncod as foncod, 
				fs.fondsc as fondsc, 
				fr.codigo as fonterecurso, 
				fr.descricao as fonterecursodsc, 
				gnd.gndcod as gndcod, 
				gnd.gnddsc as gnddsc, 
				gf.grfid as grfid, 
				gf.grfdsc as grfdsc, 
				loc.prgcod || '.' || loc.acacod || '.' || loc.unicod || '.' || loc.loccod as funcionalcod, 
				loc.acadsc || ' ' || loc.sacdsc as funcionaldsc, 
				ma.mapcod as mapcod, 
				ma.mapdsc as mapdsc, 
				cast(nd.ctecod as varchar) || nd.gndcod || nd.mapcod || nd.edpcod as natureza, 
				nd.ndpcod as ndpcod, 
				nd.ndpdsc as ndpdsc, 
				pli.plicod as plicod, 
				pli.plititulo as plidsc, 
				orc.prgcod as prgcod, 
				orc.prgdsc as prgdsc, 
				orc.sbecod as sbecod, 
				orc.sbecod as sbepdsc, 
				sba.sbacod as sbacod, 
				sba.sbatitulo as sbadsc, 
				u.ungcod as ungcod, 
				u.ungdsc as ungdsc,  
				sum( coalesce( rofdot_ini, 0 ) ) / 1 as rofdot_ini,  
				sum( coalesce( rofautorizado, 0 ) ) / 1 as rofautorizado,  
				sum( coalesce( rof_valorpropostopi, 0 ) ) / 1 as rof_valorpropostopi,  
				sum( coalesce( rofempenhado, 0 ) ) / 1 as rofempenhado,  
				sum( coalesce( rofliquidado_favorecido, 0 ) ) / 1 as rofliquidado_favorecido,  
				sum( coalesce( rofpago, 0 ) ) / 1 as rofpago    
			from financeiro.execucao orc   
			    inner join public.categoriaeconomica cte on cte.ctecod = orc.ctecod 
			    inner join public.elementodespesa ele on ele.edpcod = orc.edpcod   
			    inner join financeiro.fontesiafi frs on frs.foscod = orc.foncodsiafi  
			    inner join financeiro.fontesof fs on fs.foncod = orc.foncod  
			    inner join financeiro.fonterecursos fr ON fr.codigo = substr( orc.foncod, 2, 2)   
			    inner join gnd on gnd.gndcod = cast(orc.gndcod as integer)    
			    inner join grupofonte gf ON gf.grfid = cast( substring( orc.foncod, 1, 1 ) as integer )  
			    inner join monitora.acao loc on loc.acacod = orc.acacod and loc.prgcod = orc.prgcod and loc.unicod = orc.unicod and loc.loccod = orc.loccod and loc.prgano = orc.rofano and loc.acasnrap = 'f'   
			    inner join modalidadeaplicacao ma on ma.mapcod = orc.mapcod   
			    inner join public.naturezadespesa nd on cast(nd.ctecod as varchar) || nd.gndcod || nd.mapcod || nd.edpcod = cast(orc.ctecod as varchar) || cast(orc.gndcod as varchar) || orc.mapcod || orc.edpcod and nd.sbecod = '00'  
			    inner join monitora.pi_planointerno pli on pli.plicod = orc.plicod and pli.plistatus = 'A' and pli.pliano = '2010'   
			    inner join monitora.pi_subacao sba on sba.sbacod = substring( orc.plicod, 2, 4 ) and sba.sbastatus = 'A'  
			    inner join public.unidadegestora u on u.ungcod = orc.ungcod
			 where " . ( is_array($where) && count($where) ? ' ' . implode(' and ', $where) : '' ) ."
			 group by orc.acacod, orc.acadsc, cte.ctecod, cte.ctedsc, ele.edpcod, ele.edpdsc, frs.foscod, frs.fosdsc, fs.foncod, fs.fondsc, fr.codigo, fr.descricao, gnd.gndcod, gnd.gnddsc, gf.grfid, gf.grfdsc, loc.prgcod || '.' || loc.acacod || '.' || loc.unicod || '.' || loc.loccod, loc.acadsc || ' ' || loc.sacdsc, ma.mapcod, ma.mapdsc, cast(nd.ctecod as varchar) || nd.gndcod || nd.mapcod || nd.edpcod, nd.ndpcod, nd.ndpdsc, pli.plicod, pli.plititulo, orc.prgcod, orc.prgdsc, orc.sbecod, orc.sbecod, sba.sbacod, sba.sbatitulo, u.ungcod, u.ungdsc 
			 order by " . ( is_array($agrupador) && count($agrupador) ? implode(', ', $agrupador) : '' ) ."
			 --limit 10
			    ";
	//ver($sql,d);
	return $sql;
}

function monta_agp(){

	$agrupador = (array) $_POST['agrupador'];
	
	$agp = array(
				 "agrupador" 	  => array(),
				 "agrupadoColuna" => array(
											"rofdot_ini",
											"rofautorizado",
											"rof_valorpropostopi",
											"rofempenhado",
											"rofliquidado_favorecido",
											"rofpago"
										  )
				);
	
	foreach( $agrupador as $val ):
		switch ($val) {
		    case 'acacod':
				array_push($agp['agrupador'], array(
													"campo" => "acacod",
											  		"label" => "Ação"
												   )										
						  );				
		    continue;
		    break;
		    case 'ctecod':
				array_push($agp['agrupador'], array(
													"campo" => "ctecod",
											  		"label" => "Categoria Econômica"
												   )										
						  );				
		    continue;
		    break;
		    case 'edpcod':
				array_push($agp['agrupador'], array(
													"campo" => "edpcod",
											  		"label" => "Elemento"
												   )										
						  );				
		    continue;
		    break;
		    case 'foscod':
				array_push($agp['agrupador'], array(
													"campo" => "foscod",
											  		"label" => "Fonte Detalhada"
												   )										
						  );				
		    continue;
		    break;
		    case 'foncod':
				array_push($agp['agrupador'], array(
													"campo" => "foncod",
											  		"label" => "Fonte SOF"
												   )										
						  );				
		    continue;
		    break;		
		    case 'fonterecurso':
				array_push($agp['agrupador'], array(
													"campo" => "fonterecurso",
											  		"label" => "Fonte de Recursos"
												   )										
						  );
			continue;
		    break;				
		    case 'gndcod':
				array_push($agp['agrupador'], array(
													"campo" => "gndcod",
											  		"label" => "GND"
												   )										
						  );
			continue;
		    break;				
		    case 'grfid':
				array_push($agp['agrupador'], array(
													"campo" => "grfid",
											  		"label" => "Grupo de Fonte"
												   )										
						  );
			continue;
		    break;				
		    case 'funcionalcod':
				array_push($agp['agrupador'], array(
													"campo" => "funcionalcod",
											  		"label" => "Localizador"
												   )										
						  );
			continue;
		    break;				
		    case 'mapcod':
				array_push($agp['agrupador'], array(
													"campo" => "mapcod",
											  		"label" => "Modalidade de Aplicação"
												   )										
						  );
			continue;
		    break;				
		    case 'ndpcod':
				array_push($agp['agrupador'], array(
													"campo" => "ndpcod",
											  		"label" => "Natureza de Despesa"
												   )										
						  );
			continue;
		    break;				
		    case 'plicod':
				array_push($agp['agrupador'], array(
													"campo" => "plicod",
											  		"label" => "Plano Interno"
												   )
						  );
			continue;
		    break;				
		    case 'prgcod':
				array_push($agp['agrupador'], array(
													"campo" => "prgcod",
											  		"label" => "Programa"
												   )
						  );
			continue;
		    break;				
		    case 'sbecod':
				array_push($agp['agrupador'], array(
													"campo" => "sbecod",
											  		"label" => "Sub-Elemento de Despesa"
												   )
						  );
			continue;
		    break;				
		    case 'sbacod':
				array_push($agp['agrupador'], array(
													"campo" => "sbacod",
											  		"label" => "Subação"
												   )										
						  );
			continue;
		    break;				
		    case 'ungcod':
				array_push($agp['agrupador'], array(
													"campo" => "ungcod",
											  		"label" => "Unidade Gestora"
												   )										
						  );				
		    continue;
		    break;		    		    
		}       
	endforeach;
	
	return $agp;
}


function monta_coluna(){

	$coluna = array(
				array(
					  "campo" => "rofdot_ini",
			   		  "label" => "Dotação Inicial<br/>(A)",
				),
				array(
					  "campo" => "rofautorizado",
			   		  "label" => "Dotação Autorizada<br/>(B)",
				),
				array(
					  "campo" => "rof_valorpropostopi",
			   		  "label" => "Valor Proposto<br/>(C)"
				),
				array(
					  "campo" => "rofempenhado",
			   		  "label" => "Despesa Empenhada<br/>(D)"
				),
				array(
					  "campo" => "rofliquidado_favorecido",
			   		  "label" => "Despesa Liquidada<br/>(E)"
				),
				array(
					  "campo" => "rofpago",
			   		  "label" => "Valores Pagos<br/>(F)",
				)
			  );
return $coluna;				  
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$atividade = atividade_pegar( $_REQUEST['atiid'] );

// montando o menu
echo montarAbasArray(carregardadosplanotrabalhoUG_sub(), "/monitora/monitora.php?modulo=principal/planotrabalhoUG/planoInternoUG&acao=A&atiid=".$_REQUEST['atiid']);

monta_titulo( 'Relatório Módulo Financeiro', 'Relatório Geral' );

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">

	/**
	 * Alterar visibilidade de um bloco.
	 * 
	 * @param string indica o bloco a ser mostrado/escondido
	 * @return void
	 */
	function onOffBloco( bloco )
	{
		var div_on = document.getElementById( bloco + '_div_filtros_on' );
		var div_off = document.getElementById( bloco + '_div_filtros_off' );
		var img = document.getElementById( bloco + '_img' );
		var input = document.getElementById( bloco + '_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '0';
			img.src = '/imagens/menos.gif';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '1';
			img.src = '/imagens/mais.gif';
		}
	}
	
	/**
	 * Alterar visibilidade de um campo.
	 * 
	 * @param string indica o campo a ser mostrado/escondido
	 * @return void
	 */
	function onOffCampo( campo )
	{
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}
	
	function geraPopRelatorio(){
		
		var form = $('formulario');
		var agrupador  = document.getElementById( 'agrupador' );

		if ( !agrupador.options.length ){
			alert( 'Favor selecionar ao menos um item para agrupar o resultado.' );
			return false;
		}
		
		if($('programa_campo_flag').value == "1"){
			selectAllOptions( form.programa );
		}
		
		if($('acacod_campo_flag').value == "1"){
			selectAllOptions( form.acacod );
		}
		
		if($('plicod_campo_flag').value == "1"){
			selectAllOptions( form.plicod );
		}
		
		if($('sbacod_campo_flag').value == "1"){
			selectAllOptions( form.sbacod );
		}
		
		if($('fontesiafi_campo_flag').value == "1"){
			selectAllOptions( form.fontesiafi );
		}
		
		if($('fontesof_campo_flag').value == "1"){
			selectAllOptions( form.fontesof );
		}
		
		if($('fonte_campo_flag').value == "1"){
			selectAllOptions( form.fonte );
		}
		
		if($('catecon_campo_flag').value == "1"){
			selectAllOptions( form.catecon );
		}
		
		if($('gnd_campo_flag').value == "1"){
			selectAllOptions( form.gnd );
		}
		
		if($('mapcod_campo_flag').value == "1"){
			selectAllOptions( form.mapcod );
		}
		
		if($('elemento_campo_flag').value == "1"){
			selectAllOptions( form.elemento );
		}
		
		if($('natureza_campo_flag').value == "1"){
			selectAllOptions( form.natureza );
		}
		
		selectAllOptions( form.agrupador );
		form.target = 'resultadoRelatorioPlanoInternoUG';

		var janela = window.open( '', 'resultadoRelatorioPlanoInternoUG', 'fullscreen=1,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		
		janela.focus();
		form.submit();
	}
	
	function alterarAno()
	{
		var formulario = document.formulario;
		formulario.alterar_ano.value = '1';
		submeterFormulario('relatorio');
	}
	
</script>
<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="pesquisa" value="1"/> <!-- indica envio de formulário -->
	<input type="hidden" name="alterar_ano" value="0"/> <!-- indica se há mudança de ano no formulário -->
	<input type="hidden" name="ano" value="<?= $ano ?>"/> 
	<input type="hidden" name="ug" value="<?= $_SESSION['monitora_var']['ungcod'] ?>"/> 
	<input type="hidden" name="atiid" value="<?= $_REQUEST['atiid'] ?>"/> 
	
	<!-- INSTITUCIONAL -->
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
			<tr>
				<td width="195" colspan="2" class="SubTituloDireita" valign="top"><?= montar_resumo_atividade( $atividade ) ?></td>
				
			<tr>
				<td width="195" class="SubTituloDireita" valign="top">Agrupadores</td>
				<td>
					<?php
						// Início dos agrupadores
						$agrupador = new Agrupador('formulario','');
						
						// Dados padrão de origem
						$origem = array(
							array(
								'codigo'    => 'acacod',
								'descricao' => 'Ação'
							),
							array(
								'codigo'    => 'ctecod',
								'descricao' => 'Categoria Econômica'
							),
							array(
								'codigo'    => 'edpcod',
								'descricao' => 'Elemento'
							),
							array(
								'codigo'    => 'foscod',
								'descricao' => 'Fonte Detalhada'
							),
							array(
								'codigo'    => 'foncod',
								'descricao' => 'Fonte SOF'
							),
							array(
								'codigo'    => 'fonterecurso',
								'descricao' => 'Fonte de Recursos'
							),
							array(
								'codigo'    => 'gndcod',
								'descricao' => 'GND'
							),
							array(
								'codigo'    => 'grfid',
								'descricao' => 'Grupo de Fonte'
							),
							array(
								'codigo'    => 'funcionalcod',
								'descricao' => 'Localizador'
							),
							array(
								'codigo'    => 'mapcod',
								'descricao' => 'Modalidade de Aplicação'
							),
							array(
								'codigo'    => 'ndpcod',
								'descricao' => 'Natureza de Despesa'
							),
							array(
								'codigo'    => 'plicod',
								'descricao' => 'Plano Interno'
							),
							array(
								'codigo'    => 'prgcod',
								'descricao' => 'Programa'
							),
							array(
								'codigo'    => 'sbecod',
								'descricao' => 'Sub-Elemento de Despesa'
							),
							array(
								'codigo'    => 'sbacod',
								'descricao' => 'Subação'
							),
							array(
								'codigo'    => 'ungcod',
								'descricao' => 'Unidade Gestora'
							),
						);
						
						// exibe agrupador
						$agrupador->setOrigem( 'naoAgrupador', null, $origem );
						$agrupador->setDestino( 'agrupador', null);
						$agrupador->exibir();
					?>
				</td>
			</tr>
	</table>
	
	<!-- OUTROS FILTROS -->
	
	<!-- FUNCIONAL -->
	
	<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
		<tr>
			<td onclick="javascript:onOffBloco( 'func' );">
				<img border="0" src="/imagens/mais.gif" id="func_img"/>&nbsp;
				Funcional
				<input type="hidden" id="func_flag" name="inst_flag" value="0" />
			</td>
		</tr>
	</table>
	<div id="func_div_filtros_off">
		<!--
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td><span style="color:#a0a0a0;padding:0 30px;">nenhum filtro</span></td>
			</tr>
		</table>
		-->
	</div>
	<div id="func_div_filtros_on" style="display:none;">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<!--
			<tr>
				<td class="SubTituloDireita" style="text-align:center;">A</td>
				<td class="SubTituloDireita" style="text-align:center;">S</td>
				<td colspan="2">&nbsp;</td>
			</tr>
			-->
			
			<tr>
				<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'programa' );">
					Programa
					<input type="hidden" id="programa_campo_flag" name="programa_campo_flag" value="0"/>
				</td>
				<td>
					<div id="programa_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'programa' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="programa_campo_on" style="display:none;">
						<? $sql_combo = "select p.prgcod as codigo, p.prgcod || ' - ' || p.prgdsc as descricao from financeiro.execucao r inner join planejamento.programa p on r.prgcod = p.prgcod where r.rofano = '" . $ano . "' group by p.prgcod, p.prgdsc order by p.prgcod, p.prgdsc";
						if ( $_REQUEST['programa'] && $_REQUEST['programa'][0] != '' )	
						{
							$sql_carregados = "select p.prgcod as codigo, p.prgcod || ' - ' || p.prgdsc as descricao from financeiro.execucao r inner join planejamento.programa p on r.prgcod = p.prgcod where r.rofano = '" . $ano . "' and p.prgcod in ('".implode("','",$_REQUEST['programa'])."') group by p.prgcod, p.prgdsc ";
							$programa=$db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'programa', $sql_combo, 'Selecione o(s) Programa(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $programa )  { ?>	<script type="text/javascript"> onOffCampo( 'programa' ); </script> <? } ?>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'acacod' );">
					Ação
					<input type="hidden" id="acacod_campo_flag" name="acacod_campo_flag" value="0"/>
				</td>
				<td>
					<div id="acacod_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'acacod' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="acacod_campo_on" style="display:none;">
						<? $sql_combo = "select acao.acacod as codigo, acao.acacod || ' - ' || acao.acadsc as descricao from financeiro.execucao r inner join ( select distinct acacod, acadsc, prgano from monitora.acao where prgano = '".$ano."' and acasnrap = 'f' group by acacod, acadsc, prgano ) acao on acao.acacod = r.acacod and acao.prgano = r.rofano where r.rofano = '" . $ano . "' group by acao.acacod, acao.acadsc order by acao.acacod, acao.acadsc"; 
						if ( $_REQUEST['acacod'] && $_REQUEST['acacod'][0] != '' )
						{
							$sql_carregados = "select acao.acacod as codigo, acao.acacod || ' - ' || acao.acadsc as descricao from financeiro.execucao r inner join ( select distinct acacod, acadsc, prgano from monitora.acao where prgano = '$ano' and acasnrap = 'f' group by acacod, acadsc, prgano ) acao on acao.acacod = r.acacod and acao.prgano = r.rofano where r.rofano = '" . $ano . "' and acao.acacod in ('".implode("','",$_REQUEST['acacod'])."') group by acao.acacod, acao.acadsc ";
							$acacod = $db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'acacod', $sql_combo, 'Selecione a(s) Ação(ões)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $acacod )  { ?>	<script type="text/javascript"> onOffCampo( 'acacod' ); </script> <? } ?>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'plicod' );">
					Plano Interno
					<input type="hidden" id="plicod_campo_flag" name="plicod_campo_flag" value="0"/>
				</td>
				<td>
					<div id="plicod_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'plicod' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="plicod_campo_on" style="display:none;">
						<? $sql_combo = "select plicod as codigo, plicod || ' - ' || plititulo as descricao from monitora.pi_planointerno where pliano = '".$ano."'";
						if ( $_REQUEST['plicod'] && $_REQUEST['plicod'][0] != '' )
						{
							$sql_carregados = "select plicod as codigo, plicod || ' - ' || plititulo as descricao from monitora.pi_planointerno where pliano = '".$ano."' and plicod in ('".implode("','",$_REQUEST['plicod'])."')";
							$plicod = $db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'plicod', $sql_combo, 'Selecione o(s) Plano(s) Interno(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? 
			
				if ( $plicod )  { ?>	<script type="text/javascript"> onOffCampo( 'plicod' ); </script> <? } 
			
					/* Colocado o filtro abaixo para verificar se a atividade é uma subação, caso afiramtivo, coloca no filtro somente esta subação */
					if ( $_REQUEST["atiid"] ) {
					$sb = $db->pegaUm("SELECT sba.sbaid FROM monitora.pi_subacaoatividade sba
								 	  	   WHERE atiid = '".$_REQUEST["atiid"]."' AND sbaatividade=false");
					}

					if($sb) {
						$codsubacao = $db->pegaUm("SELECT sbacod FROM monitora.pi_subacao WHERE sbaid = ".$sb);
						$wh2 = 'where sbaid = '.$sb;
						$_REQUEST['sbacod'][0] = ($codsubacao);
					}
					else {
						$wh2 = '';
					}
			
			?>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'sbacod' );">
					Subação
					<input type="hidden" id="sbacod_campo_flag" name="sbacod_campo_flag" value="0"/>
				</td>
				<td>
					<div id="sbacod_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'sbacod' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="sbacod_campo_on" style="display:none;">
						<? $sql_combo = "select sbacod as codigo, sbacod || ' - ' || sbatitulo as descricao from monitora.pi_subacao {$wh2}";
						if ( $_REQUEST['sbacod'] && $_REQUEST['sbacod'][0] != '' )
						{
							$sql_carregados = "select sbacod as codigo, sbacod || ' - ' || sbatitulo as descricao from monitora.pi_subacao where sbacod in ('".implode("','",$_REQUEST['sbacod'])."')";
							$sbacod = $db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'sbacod', $sql_combo, 'Selecione o(s) Plano(s) Interno(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $sbacod )  { ?>	<script type="text/javascript"> onOffCampo( 'sbacod' ); </script> <? } ?>
			
		</table>
	</div>
	
	<!-- FIM FUNCIONAL -->
	
	<!-- ----------------------------------------------------------------------- -->
	
	<!-- CLASSIFICACAO ECONÔMICA -->
	
	<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
		<tr>
			<td onclick="javascript:onOffBloco( 'econ' );">
				<img border="0" src="/imagens/mais.gif" id="econ_img"/>&nbsp;
				Classificação Econômica
				<input type="hidden" id="econ_flag" name="econ_flag" value="0" />
			</td>
		</tr>
	</table>
	<div id="econ_div_filtros_off">
		<!--
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td><span style="color:#a0a0a0;padding:0 30px;">nenhum filtro</span></td>
			</tr>
		</table>
		-->
	</div>
	<div id="econ_div_filtros_on" style="display:none;">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
			<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'fontesiafi' );">
				Fonte Detalhada
				<input type="hidden" id="fontesiafi_campo_flag" name="fontesiafi_campo_flag" value="0"/>
			</td>
			<td>
				<div id="fontesiafi_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'fontesiafi' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="fontesiafi_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT
										f.foscod AS codigo,
										f.foscod || ' - ' || f.fosdsc AS descricao 
									FROM
										financeiro.execucao r 
									INNER JOIN financeiro.fontesiafi f ON f.foscod = r.foncodsiafi 
									--WHERE
									--	r.rofano = '" . $ano . "' 
									GROUP BY 
										f.foscod, 
										f.fosdsc 
									ORDER BY
										f.foscod, 
										f.fosdsc"; 
					
						if ( $_REQUEST['fontesiafi'] && $_REQUEST['fontesiafi'][0] != '' )
						{
							/*$sql_carregados = "SELECT
												foscod AS codigo,
												foscod || ' - ' || fosdsc AS descricao
											   FROM
												financeiro.fontesiafi
											   WHERE
											   	foscod in ('".implode("','", $_REQUEST['fontesiafi'])."')";*/
							$sql_carregados = "SELECT
													f.foscod AS codigo,
													f.foscod || ' - ' || f.fosdsc AS descricao 
												FROM
													financeiro.execucao r 
												INNER JOIN financeiro.fontesiafi f ON f.foscod = r.foncodsiafi 
												WHERE
													f.foscod in ('".implode("','", $_REQUEST['fontesiafi'])."')' 
												GROUP BY 
													f.foscod, 
													f.fosdsc 
												ORDER BY
													f.foscod, 
													f.fosdsc";
							
							$fontesiafi = $db->carregar( $sql_carregados );
						}
						
						combo_popup( 'fontesiafi', $sql_combo, 'Selecione a(s) Fonte(s)', '400x400', 0, array(), '', 'S', true, true);
					?>
				</div>
			</td>
		</tr>
		<tr>
			
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'fontesof' );">
				Fonte SOF
				<input type="hidden" id="fontesof_campo_flag" name="fontesof_campo_flag" value="0"/>
			</td>
			<td>
				<div id="fontesof_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'fontesof' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="fontesof_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT
										f.foncod AS codigo,
										f.foncod || ' - ' || f.fondsc AS descricao 
									FROM
										financeiro.execucao r 
									
									INNER JOIN financeiro.fontesof f on r.foncod = f.foncod  
									--WHERE
									--	r.rofano = '" . $ano . "' 
									GROUP BY 
										f.foncod, 
										f.fondsc 
									ORDER BY
										f.foncod, 
										f.fondsc"; 
					
						if ( $_REQUEST['fontesof'] && $_REQUEST['fontesof'][0] != '' )
						{
							/*$sql_carregados = "SELECT
												foncod AS codigo,
												foncod || ' - ' || fondsc AS descricao
											   FROM
												financeiro.fontesof
											   WHERE
											   	foncod in ('".implode("','", $_REQUEST['fontesof'])."')";*/
							
							$sql_carregados = "SELECT
													f.foncod AS codigo,
													f.foncod || ' - ' || f.fondsc AS descricao 
												FROM
													financeiro.execucao r 
												
												INNER JOIN financeiro.fontesof f on r.foncod = f.foncod  
												WHERE
													f.foncod in ('".implode("','", $_REQUEST['fontesof'])."') 
												GROUP BY 
													f.foncod, 
													f.fondsc 
												ORDER BY
													f.foncod, 
													f.fondsc";
							$fontesof = $db->carregar( $sql_carregados );
						}
						
						combo_popup( 'fontesof', $sql_combo, 'Selecione a(s) Fonte(s)', '400x400', 0, array(), '', 'S', true, true);
					?>
				</div>
			</td>
		</tr>
		<? if ( $fontesof )  { ?> <script type="text/javascript"> onOffCampo( 'fontesof' ); </script> <? } ?>
		<? if ( $fontesiafi )  { ?> <script type="text/javascript"> onOffCampo( 'fontesiafi' ); </script> <? } ?>
			<tr>
				<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'fonte' );">
					Fonte de Recurso
					<input type="hidden" id="fonte_campo_flag" name="fonte_campo_flag" value="0"/>
				</td>
				<td>
					<div id="fonte_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'fonte' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="fonte_campo_on" style="display:none;">
						<?  
						$sql_combo = "SELECT
										f.codigo AS codigo,
										f.codigo || ' - ' || f.descricao AS descricao 
									FROM
										financeiro.execucao r 
									INNER JOIN financeiro.fonterecursos f ON f.codigo = substr( r.foncod, 2, 2) 
									--WHERE
									--	r.rofano = '" . $ano . "' 
									GROUP BY 
										f.codigo, 
										f.descricao 
									ORDER BY
										f.codigo, 
										f.descricao";
															
						if ( $_REQUEST['fonte'] && $_REQUEST['fonte'][0] != '' )
						{
							//$sql_carregados = "select fr.foncod as codigo, fr.foncod || ' - ' || fr.fondsc as descricao from financeiro.execucao r inner join public.fonterecurso fr on r.foncod = fr.foncod where r.rofano = '" . $ano . "' and fr.foncod in ('".implode("','",$_REQUEST['fonte'])."') group by fr.foncod, fr.fondsc order by fr.foncod, fr.fondsc ";
							$sql_carregados = "SELECT
													f.codigo AS codigo,
													f.codigo || ' - ' || f.descricao AS descricao 
												FROM
													financeiro.execucao r 
												INNER JOIN financeiro.fonterecursos f ON f.codigo = substr( r.foncod, 2, 2) 
												WHERE
													f.codigo in ('".implode("','",$_REQUEST['fonte'])."') 
												GROUP BY 
													f.codigo, 
													f.descricao 
												ORDER BY
													f.codigo, 
													f.descricao";
							
							$fonte=$db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'fonte', $sql_combo, 'Selecione a(s) Fonte(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $fonte )  { ?>	<script type="text/javascript"> onOffCampo( 'fonte' ); </script> <? } ?>
			
		
			
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'catecon' );">
					Categoria Econômica
					<input type="hidden" id="catecon_campo_flag" name="catecon_campo_flag" value="0"/>
				</td>
				<td>
					<div id="catecon_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'catecon' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="catecon_campo_on" style="display:none;">
						<? $sql_combo = "select cte.ctecod as codigo, cte.ctecod || ' - ' || cte.ctedsc as descricao from financeiro.execucao r inner join public.categoriaeconomica cte on cte.ctecod = r.ctecod where r.rofano = '" . $ano . "' group by cte.ctecod, cte.ctedsc order by cte.ctecod, cte.ctedsc"; 
						if ( $_REQUEST['catecon'] && $_REQUEST['catecon'][0] != '' )
						{
							$sql_carregados = "select cte.ctecod as codigo, cte.ctecod || ' - ' || cte.ctedsc as descricao from financeiro.execucao r inner join public.categoriaeconomica cte on cte.ctecod = r.ctecod where r.rofano = '" . $ano . "' and cte.ctecod in ( '" . implode( "','", $_REQUEST['catecon'] ) . "' ) group by cte.ctecod, cte.ctedsc order by cte.ctecod, cte.ctedsc";
							$catecon = $db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'catecon', $sql_combo, 'Selecione o(s) GND(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $catecon )  { ?>	<script type="text/javascript"> onOffCampo( 'catecon' ); </script> <? } ?>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'gnd' );">
					GND
					<input type="hidden" id="gnd_campo_flag" name="gnd_campo_flag" value="0"/>
				</td>
				<td>
					<div id="gnd_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'gnd' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="gnd_campo_on" style="display:none;">
						<? $sql_combo = "select g.gndcod as codigo, g.gndcod || ' - ' || g.gnddsc as descricao from financeiro.execucao r inner join public.gnd g on r.gndcod = g.gndcod where r.rofano = '" . $ano . "' group by g.gndcod, g.gnddsc order by g.gndcod, g.gnddsc"; 
						if ( $_REQUEST['gnd'] && $_REQUEST['gnd'][0] != '' )
						{
							$sql_carregados = "select g.gndcod as codigo, g.gndcod || ' - ' || g.gnddsc as descricao from financeiro.execucao r inner join public.gnd g on r.gndcod = g.gndcod where r.rofano = '" . $ano . "' and g.gndcod in ( '" . implode( "','", $_REQUEST['gnd'] ) . "' ) group by g.gndcod, g.gnddsc ";
							$gnd = $db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'gnd', $sql_combo, 'Selecione o(s) GND(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $gnd )  { ?>	<script type="text/javascript"> onOffCampo( 'gnd' ); </script> <? } ?>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'mapcod' );">
					Modalidade de Aplicação
					<input type="hidden" id="mapcod_campo_flag" name="mapcod_campo_flag" value="0"/>
				</td>
				<td>
					<div id="mapcod_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'mapcod' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="mapcod_campo_on" style="display:none;">
						<? $sql_combo = "select ma.mapcod as codigo, ma.mapcod || ' - ' || ma.mapdsc as descricao from financeiro.execucao r inner join modalidadeaplicacao ma ON ma.mapcod = r.mapcod where r.rofano = '" . $ano . "' group by ma.mapcod, ma.mapdsc order by ma.mapcod, ma.mapdsc "; 
						if ( $_REQUEST['mapcod'] && $_REQUEST['mapcod'][0] != '' )
						{
							$sql_carregados = "select ma.mapcod as codigo, ma.mapcod || ' - ' || ma.mapdsc as descricao from financeiro.execucao r inner join modalidadeaplicacao ma ON ma.mapcod = r.mapcod where r.rofano = '" . $ano . "' and ma.mapcod in ('".implode("','",$_REQUEST['mapcod'])."') group by ma.mapcod, ma.mapdsc ";
							$mapcod=$db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'mapcod', $sql_combo, 'Selecione a(s) Modalidade(s) de Aplicação', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $mapcod )  { ?>	<script type="text/javascript"> onOffCampo( 'mapcod' ); </script> <? } ?>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'elemento' );">
					Elemento de Despesa
					<input type="hidden" id="elemento_campo_flag" name="elemento_campo_flag" value="0"/>
				</td>
				<td>
					<div id="elemento_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'elemento' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="elemento_campo_on" style="display:none;">
						<? $sql_combo = "select ele.edpcod as codigo, ele.edpcod || ' - ' || ele.edpdsc as descricao from financeiro.execucao r inner join public.elementodespesa ele on ele.edpcod = r.edpcod where r.rofano = '" . $ano . "' group by ele.edpcod, ele.edpdsc order by ele.edpcod, ele.edpdsc"; 
						if ( $_REQUEST['elemento'] && $_REQUEST['elemento'][0] != '' )
						{
							$sql_carregados = "select ele.edpcod as codigo, ele.edpcod || ' - ' || ele.edpdsc as descricao from financeiro.execucao r inner join public.elementodespesa ele on ele.edpcod = r.edpcod where r.rofano = '" . $ano . "' and ele.edpcod in ('".implode("','",$_REQUEST['elemento'])."') group by ele.edpcod, ele.edpdsc order by ele.edpcod, ele.edpdsc";
							$elemento=$db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'elemento', $sql_combo, 'Selecione o(s) Elemento(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $elemento )  { ?>	<script type="text/javascript"> onOffCampo( 'elemento' ); </script> <? } ?>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'natureza' );">
					Natureza de Despesa
					<input type="hidden" id="natureza_campo_flag" name="natureza_campo_flag" value="0"/>
				</td>
				<td>
					<div id="natureza_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'natureza' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="natureza_campo_on" style="display:none;">
						<? $sql_combo = "select cast(nd.ctecod as varchar) || cast(nd.gndcod as varchar) || nd.mapcod || nd.edpcod as codigo, nd.ctecod || '.' || nd.gndcod || '.' || nd.mapcod || '.' || nd.edpcod || ' - ' || nd.ndpdsc as descricao from financeiro.execucao r inner join public.naturezadespesa nd on nd.ctecod = r.ctecod and nd.gndcod = r.gndcod and nd.mapcod = r.mapcod and nd.edpcod = r.edpcod where r.rofano = '" . $ano . "' group by nd.ctecod, nd.gndcod, nd.mapcod, nd.edpcod, nd.ndpdsc order by nd.ctecod, nd.gndcod, nd.mapcod, nd.edpcod, nd.ndpdsc"; 
						if ( $_REQUEST['natureza'] && $_REQUEST['natureza'][0] != '' )
						{
							$sql_carregados = "select cast(nd.ctecod as varchar) || cast(nd.gndcod as varchar) || nd.mapcod || nd.edpcod as codigo, nd.ctecod || '.' || nd.gndcod || '.' || nd.mapcod || '.' || nd.edpcod || ' - ' || nd.ndpdsc as descricao from financeiro.execucao r inner join public.naturezadespesa nd on nd.ctecod = r.ctecod and nd.gndcod = r.gndcod and nd.mapcod = r.mapcod and nd.edpcod = r.edpcod where r.rofano = '" . $ano . "' and nd.ctecod || nd.gndcod || nd.mapcod || nd.edpcod in ('".implode("','",$_REQUEST['natureza'])."') group by nd.ctecod, nd.gndcod, nd.mapcod, nd.edpcod, nd.ndpdsc order by nd.ctecod, nd.gndcod, nd.mapcod, nd.edpcod, nd.ndpdsc";
							$natureza=$db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'natureza', $sql_combo, 'Selecione a(s) Natureza(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $natureza )  { ?>	<script type="text/javascript"> onOffCampo( 'natureza' ); </script> <? } ?>
		</table>
	</div>
	
	<!-- FIM CLASSIFICACAO ECONÔMICA -->
	
	<!-- ----------------------------------------------------------------------- -->
	
	
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
			<tr>
				<td align="center">
					<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="geraPopRelatorio();"/>
				</td>
			</tr>
	</table>
	
	<!-- ----------------------------------------------------------------------- -->

</form>
<center>
<script type="text/javascript">
	onOffBloco( 'func' );
	onOffBloco( 'econ' );
</script>