<?php
if ( $_POST['pesquisa'] || $_GET['pesquisa'] ){
	ini_set("memory_limit","250M");
	set_time_limit(0);
	
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
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
	
	// Atividade
	if( $f_atividade[0] && $atividade_campo_flag ){
		array_push($where, " atv.atiid in ('" . implode( "','", $f_atividade ) . "') ");
	}
	// Ação
	if( $f_acao[0] && $acao_campo_flag ){
		array_push($where, " ac.acacod in ('" . implode( "','", $f_acao ) . "') ");
	}
	// Categoria Apropriação
	if( $f_cat_apropriacao[0] && $cat_apropriacao_campo_flag ){
		array_push($where, " cap.capid in ('" . implode( "','", $f_cat_apropriacao ) . "') ");
	}
	// Enquadramento Despesa
	if( $f_enq_Despesa[0] && $enq_Despesa_campo_flag ){
		array_push($where, " eqd.eqdid in ('" . implode( "','", $f_enq_Despesa ) . "') ");
	}
	// Localizador
	if( $f_localizador[0] && $localizador_campo_flag ){
		array_push($where, " ac.acaid in ('" . implode( "','", $f_localizador ) . "') ");
	}
	// Nível/Etapa de Ensino
	if( $f_nivel_ensino[0] && $nivel_ensino_campo_flag ){
		array_push($where, " nee.neeid in ('" . implode( "','", $f_nivel_ensino ) . "') ");
	}
	// Modalidade de Ensino
	if( $f_mod_ensino[0] && $mod_ensino_campo_flag ){
		array_push($where, " mde.mdeid in ('" . implode( "','", $f_mod_ensino ) . "') ");
	}
	// PTRES
	if( $f_ptres[0] && $ptres_campo_flag ){
		array_push($where, " ptr.ptres in ('" . implode( "','", $f_ptres ) . "') ");
	}
	// Plano Interno
	if( $f_planointerno[0] && $planointerno_campo_flag ){
		array_push($where, " pl.pliid in ('" . implode( "','", $f_planointerno ) . "') ");
	}
	// Programa
	if( $f_programa[0] && $programa_campo_flag ){
		array_push($where, " ac.prgcod in ('" . implode( "','", $f_programa ) . "') ");
	}
	// Subação
	if( $f_subacao[0] && $subacao_campo_flag ){
		array_push($where, " sa.sbaid in ('" . implode( "','", $f_subacao ) . "') ");
	}
		
	$sql = "SELECT
                                pl.sbaid,
                                sa.sbacod || ' - '|| sa.sbatitulo as subacao,
                                pl.pliid,
                                pl.plicod || ' - '|| pl.plititulo as planointerno,
                                ptr.ptres,
                                pt.ptrid,
                                pt.pipvalor as valorpi,
                                ptr.acaid,
                                ac.acacod || ' - ' || ac.acadsc as acao,
                                ac.prgcod,
                                ac.loccod,
                                sum(ptr.ptrdotacao) as dotacaoautorizada,
                                round(sum( coalesce(sad.sadvalor,0) ),2) as dotacaosubacao,
                                coalesce(cast(SUM(dt2.valorpi) as varchar),'0.00') as detalhamentonopi,
                                sum(empenhado) as empenhado,
                                sum(liquidado) as liquidado,
                                sum(pago) as pago,
                                atv.atidescricao as atividade,
                                cap.capcod || ' - ' || cap.capdsc as categoria,
                                eqd.eqdcod || ' - ' || eqd.eqddsc as enquadramento,
                                nee.neecod || ' - ' || nee.needsc as niveletapa,
                                mde.mdecod || ' - ' || mde.mdedsc as modalidade
                        FROM monitora.pi_planointerno pl
                        INNER JOIN monitora.pi_subacao sa ON sa.sbaid = pl.sbaid
                        INNER JOIN monitora.pi_planointernoptres pt ON pt.pliid = pl.pliid
                        LEFT JOIN monitora.ptres ptr ON ptr.ptrid = pt.ptrid
                        LEFT JOIN monitora.acao ac ON ac.acaid = ptr.acaid
                        LEFT JOIN monitora.pi_subacaodotacao sad ON ptr.ptrid = sad.ptrid and sad.sbaid = pl.sbaid
                        LEFT JOIN monitora.pi_planointernoatividade pia on pia.pliid = pl.pliid
                        LEFT JOIN pde.atividade atv on pia.atiid = atv.atiid
                        LEFT JOIN monitora.pi_categoriaapropriacao cap on pl.capid = cap.capid
                        LEFT JOIN planejamento.enquadramento_despesa eqd on pl.eqdid = eqd.eqdid
                        LEFT JOIN monitora.pi_niveletapaensino nee on pl.neeid = nee.neeid
                        LEFT JOIN monitora.pi_modalidadeensino mde on pl.mdeid = mde.mdeid
                        LEFT JOIN ( select sbaid, ptrid,
                                                sum( sadvalor ) as valor
                                                from  monitora.pi_subacaodotacao
                                                group by sbaid, ptrid ) dt ON ptr.ptrid = dt.ptrid and dt.sbaid = sad.sbaid
                        LEFT JOIN ( select sbaid, ptrid, sum(dtl.valorpi) as valorpi from monitora.v_pi_detalhepiptres dtl group by sbaid, dtl.ptrid ) dt2 ON ptr.ptrid = dt2.ptrid and dt2.sbaid = sad.sbaid  
                        LEFT JOIN ( SELECT plicod, ptres, sum(rofempenhado) as empenhado, sum(rofliquidado_favorecido) as liquidado, sum(rofpago) as pago from financeiro.execucao where rofano ='{$_SESSION['exercicio']}' group by plicod, ptres ) fin ON trim(fin.plicod) = pl.plicod and fin.ptres = ptr.ptres
                        WHERE
                            pl.pliano ='{$_SESSION['exercicio']}' AND
						pl.plistatus='A'
						" . ( is_array($where) && count($where) ? ' AND' . implode(' AND ', $where) : '' ) ."
                GROUP BY 
                	pl.sbaid, 
                	sa.sbacod, 
                	subacao, 
                	pl.pliid, 
                	pl.plicod, 
                	planointerno, 
                	pt.ptrid, 
                	ptr.ptres, 
                	pl.plistatus, 
                	pt.pipvalor, 
                	ac.prgcod, 
                	ptr.acaid, 
                	ac.acacod, 
                	ac.unicod, 
                	ac.loccod, 
                	ac.acadsc, 
                	pia.atiid, 
                	atividade,
                	cap.capid,
                	cap.capcod,
                	categoria,
                	eqd.eqdid,
                	eqd.eqdcod,
                	enquadramento,
                	nee.neeid,
                	nee.neecod,
                	niveletapa,
                	mde.mdeid,
                	mde.mdecod,
                	modalidade
                ORDER BY 
                " . ( is_array($agrupador) && count($agrupador) ? implode(', ', $agrupador) : '' )."
                ";	

		return $sql;
}

function monta_agp(){

	$agrupador = (array) $_POST['agrupador'];
	
	$agp = array(
				 "agrupador" 	  => array(),
				 "agrupadoColuna" => array(
											"dotacaoautorizada",
											"valorpi",
											"dotacaosubacao",
											"empenhado",
											"liquidado",
											"pago"/*,
											"disponivel" */
											
										  ) 	
				);
				
	foreach( $agrupador as $val ):
		switch ($val) {
		    case 'ac.prgcod':
				array_push($agp['agrupador'], array(
													"campo" => "prgcod",
											  		"label" => "Programa"
												   )										
						  );				
		    continue;
		    break;
		    case 'ac.acacod':
				array_push($agp['agrupador'], array(
													"campo" => "acao",
											  		"label" => "Ação"
												   )										
						  );				
		    continue;
		    break;
		    case 'ungcod':
				array_push($agp['agrupador'], array(
													"campo" => "ungcod",
											  		"label" => "Unidade"
												   )										
						  );				
		    continue;
		    break;
		    case 'localizador':
				array_push($agp['agrupador'], array(
													"campo" => "localizador",
											  		"label" => "Localizador"
												   )										
						  );				
		    continue;
		    break;
		    case 'ptres':
				array_push($agp['agrupador'], array(
													"campo" => "ptres",
											  		"label" => "PTRES"
												   )										
						  );				
		    continue;
		    break;		
		    case 'pl.plicod':
				array_push($agp['agrupador'], array(
													"campo" => "planointerno",
											  		"label" => "Plano Interno"
												   )										
						  );				
		    continue;
		    break;		    		    
		    case 'eqd.eqdcod':
				array_push($agp['agrupador'], array(
													"campo" => "enquadramento",
											  		"label" => "Enquadramento da Despesa"
												   )										
						  );				
		    continue;
		    break;		    		    
		    case 'cap.capcod':
				array_push($agp['agrupador'], array(
													"campo" => "categoria",
											  		"label" => "Categoria de Apropriação"
												   )										
						  );				
		    continue;
		    break;		    		    
		    case 'nee.neecod':
				array_push($agp['agrupador'], array(
													"campo" => "niveletapa",
											  		"label" => "Nível/Etapa de ensino"
												   )										
						  );				
		    continue;
		    break;		    		    
		    case 'mde.mdecod':
				array_push($agp['agrupador'], array(
													"campo" => "modalidade",
											  		"label" => "Modalidade de ensino"
												   )										
						  );				
		    continue;
		    break;		    		    
		    case 'sa.sbacod':
				array_push($agp['agrupador'], array(
													"campo" => "subacao",
											  		"label" => "Subação"
												   )										
						  );				
		    continue;
		    break;		    		    
		    case 'pia.atiid':
				array_push($agp['agrupador'], array(
													"campo" => "atividade",
											  		"label" => "Atividade"
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
					  "campo" => "dotacaoautorizada",
			   		  "label" => "Dot. Autorizada<br>R$"
				),
				array(
					  "campo" => "dotacaosubacao",
			   		  "label" => "Valor Proposto Subação<br>R$"
				),
				array(
					  "campo" => "valorpi",
			   		  "label" => "Valor Proposto PI<br>R$"
				),
				array(
					  "campo" => "empenhado",
			   		  "label" => "Despesa Empenhada<br>R$"
				),
				array(
					  "campo" => "liquidado",
			   		  "label" => "Despesa Liquidada<br>R$"
				),
				array(
					  "campo" => "pago",
			   		  "label" => "Valores Pagos<br>R$"
				)/*,
				array(
					  "campo" => "disponivel",
			   		  "label" => "Disponível"
				)*/			
			  );
return $coluna;				  
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";

$titulo_modulo = "Relatório Geral de Plano Interno.";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
	function onOffCampo( campo ){
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		
		if ( div_on.style.display == 'none' ){
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}else{
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
		
		if($('atividade_campo_flag').value == "1"){
			selectAllOptions( form.f_atividade );
		}
		
		if($('acao_campo_flag').value == "1"){
			selectAllOptions( form.f_acao );
		}
		
		if($('cat_apropriacao_campo_flag').value == "1"){
			selectAllOptions( form.f_cat_apropriacao );
		}
		
		if($('enq_Despesa_campo_flag').value == "1"){
			selectAllOptions( form.f_enq_Despesa );
		}
		
		if($('localizador_campo_flag').value == "1"){
			selectAllOptions( form.f_localizador );
		}
		
		if($('nivel_ensino_campo_flag').value == "1"){
			selectAllOptions( form.f_nivel_ensino );
		}
		
		if($('mod_ensino_campo_flag').value == "1"){
			selectAllOptions( form.f_mod_ensino );
		}
		
		if($('ptres_campo_flag').value == "1"){
			selectAllOptions( form.f_ptres );
		}
		
		if($('planointerno_campo_flag').value == "1"){
			selectAllOptions( form.f_planointerno );
		}
		
		if($('programa_campo_flag').value == "1"){
			selectAllOptions( form.f_programa );
		}
		
		if($('subacao_campo_flag').value == "1"){
			selectAllOptions( form.f_subacao );
		}
		
		selectAllOptions( form.agrupador );
		form.target = 'resultadoRelatorioPI';

		var janela = window.open( '', 'resultadoRelatorioPI', 'fullscreen=1,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		
		janela.focus();
		form.submit();
	}
	
</script>

<form name="formulario" id="formulario" action="" method="post">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<input type="hidden" name="pesquisa" value="1"/>
	<tr>
		<td class="SubTituloDireita">Agrupadores</td>
		<td>
			<?php
				// Início dos agrupadores
				$agrupador = new Agrupador('formulario','');
				
				// Dados padrão de origem
				$origem = array(
					array('codigo' => 'ac.prgcod',
						  'descricao' => 'Programa'),
					array('codigo' => 'ac.acacod',
						  'descricao' => 'Ação'),
					//array('codigo' => 'unidorc',
						  //'descricao' => 'Unidade Orçamentária'),
					array('codigo' => 'localizador',
						  'descricao' => 'Localizador'),
					array('codigo' => 'ptres',
						  'descricao' => 'PTRES'),
					array('codigo' => 'pl.plicod',
						  'descricao' => 'Plano Interno'),
					array('codigo' => 'eqd.eqdcod',
						  'descricao' => 'Enquadramento da Despesa'),
					array('codigo' => 'cap.capcod',
						  'descricao' => 'Categoria de Apropriação'),
					array('codigo' => 'nee.neecod',
						  'descricao' => 'Nível/Etapa de ensino'),
					array('codigo' => 'mde.mdecod',
						  'descricao' => 'Modalidade de ensino'),
					array('codigo' => 'sa.sbacod',
						  'descricao' => 'Subação'),
					array('codigo' => 'pia.atiid',
						  'descricao' => 'Atividade')
				);
				
				// exibe agrupador
				$agrupador->setOrigem( 'naoAgrupador', null, $origem );
				$agrupador->setDestino( 'agrupador', null);
				$agrupador->exibir();
				
			?>
		</td>
	</tr>
	<?php
		$sql = "select distinct dprcodprograma as codigo, 
					dprprograma as descricao 
				from painel.dadosprograma
				order by descricao";
	?>
	<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'atividade' );">
				Atividade
				<input
					type="hidden"
					id="atividade_campo_flag"
					name="atividade_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="atividade_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'atividade' );"
				><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="atividade_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
						$sql_combo = "
									SELECT 
								  		a.atiid as codigo,
									  	a.atiid || ' - ' || a.atidescricao as descricao
								   	FROM 
									  	pde.atividade a 
									LEFT JOIN monitora.pi_planointernoatividade pa ON (a.atiid = pa.atiid) 
									LEFT JOIN monitora.pi_planointerno pi ON (pa.pliid = pi.pliid)
									WHERE 
										pi.pliano = '".$_SESSION['exercicio']."'
									GROUP BY
									  	a.atiid, 
									  	a.atidescricao";

						combo_popup(
							'f_atividade',			// nome do campo
							$sql_combo,						// sql de captura dos valores
							'Selecione a(s) Atividade(es)',	// label
							'400x400',						// tamanho do popup
							0,								// quantidade máxima de itens
							array(),						// valores fixos
							'',								// mensagem
							'S',							// habilitado
							true,							// habilitar busca por código
							true							// habilitar flag contém
						);
					?>
				</div>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'acao' );">
				Ação
				<input
					type="hidden"
					id="acao_campo_flag"
					name="acao_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="acao_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'acao' );"
				><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="acao_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
						$sql_combo ="SELECT
										acacod as codigo,
										acacod || ' - ' || acadsc as descricao									
									 FROM 
										monitora.acao a
									 WHERE
										a.acasnrap = false AND 
										a.prgano = '".$_SESSION['exercicio']."' AND
										a.acastatus = 'A'
									 GROUP BY
										acacod,
										acadsc
									 ORDER BY
										acacod";
						combo_popup(
							'f_acao',						// nome do campo
							$sql_combo,						// sql de captura dos valores
							'Selecione o(s) Açao(s)',		// label
							'400x400',						// tamanho do popup
							0,								// quantidade máxima de itens
							array(),						// valores fixos
							'',								// mensagem
							'S',							// habilitado
							true,							// habilitar busca por código
							true							// habilitar flag contém
						);
					?>
				</div>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'cat_apropriacao' );">
				Categoria de Apropriação
				<input
					type="hidden"
					id="cat_apropriacao_campo_flag"
					name="cat_apropriacao_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="cat_apropriacao_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'cat_apropriacao' );"
				><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="cat_apropriacao_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
						$sql_combo = "SELECT capid as codigo, capcod ||' - '|| capdsc as descricao FROM monitora.pi_categoriaapropriacao WHERE capano = '{$_SESSION['exercicio']}' ORDER BY capcod";

						combo_popup(
							'f_cat_apropriacao',			// nome do campo
							$sql_combo,						// sql de captura dos valores
							'Selecione a(s) Categoria(s) de Apropriação (es)',	// label
							'400x400',						// tamanho do popup
							0,								// quantidade máxima de itens
							array(),						// valores fixos
							'',								// mensagem
							'S',							// habilitado
							true,							// habilitar busca por código
							true							// habilitar flag contém
						);
					?>
				</div>
			</td>
		</tr>	
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'enq_Despesa' );">
				Enquadramento da Despesa
				<input
					type="hidden"
					id="enq_Despesa_campo_flag"
					name="enq_Despesa_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="enq_Despesa_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'enq_Despesa' );"
				><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="enq_Despesa_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
						$sql = "SELECT eqdid as codigo, eqdcod ||' - '|| eqddsc as descricao FROM planejamento.enquadramento_despesa WHERE eqdano = '{$_SESSION['exercicio']}' ORDER BY eqdcod";

						combo_popup(
							'f_enq_Despesa',			// nome do campo
							$sql_combo,						// sql de captura dos valores
							'Selecione o(s) Enquadramento(s) de Despesa(s)',	// label
							'400x400',						// tamanho do popup
							0,								// quantidade máxima de itens
							array(),						// valores fixos
							'',								// mensagem
							'S',							// habilitado
							true,							// habilitar busca por código
							true							// habilitar flag contém
						);
					?>
				</div>
			</td>
		</tr>	
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'localizador' );">
				Localizador
				<input
					type="hidden"
					id="localizador_campo_flag"
					name="localizador_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="localizador_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'localizador' );"
				><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="localizador_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
						$sql_combo ="SELECT acaid as codigo, 
										a.prgcod || ' - ' || acacod || ' - ' || unicod || ' - ' || loccod || ' - ' || acadsc || ' ( ' || sacdsc || ' ) ' as descricao
									FROM monitora.acao a
									WHERE
										a.acasnrap = false and a.prgano = '".$_SESSION['exercicio']."'
									order by descricao";

						combo_popup(
							'f_localizador',			// nome do campo
							$sql_combo,						// sql de captura dos valores
							'Selecione o(s) Localizador(es)',	// label
							'400x400',						// tamanho do popup
							0,								// quantidade máxima de itens
							array(),						// valores fixos
							'',								// mensagem
							'S',							// habilitado
							true,							// habilitar busca por código
							true							// habilitar flag contém
						);
					?>
				</div>
			</td>
		</tr>	
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'nivel_ensino' );">
				Nível/Etapa de Ensino
				<input
					type="hidden"
					id="nivel_ensino_campo_flag"
					name="nivel_ensino_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="nivel_ensino_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'nivel_ensino' );"
				><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="nivel_ensino_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
					$sql_combo = "SELECT neeid as codigo, neecod ||' - '|| needsc as descricao FROM monitora.pi_niveletapaensino WHERE neeano = '{$_SESSION['exercicio']}' ORDER BY neecod ";

					combo_popup(
						'f_nivel_ensino',			// nome do campo
						$sql_combo,						// sql de captura dos valores
						'Selecione o(s) Nível/Etapa de Ensino(s)',	// label
						'400x400',						// tamanho do popup
						0,								// quantidade máxima de itens
						array(),						// valores fixos
						'',								// mensagem
						'S',							// habilitado
						true,							// habilitar busca por código
						true							// habilitar flag contém
					);
					?>
				</div>
			</td>
		</tr>	
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'mod_ensino' );">
				Modalidade de Ensino
				<input
					type="hidden"
					id="mod_ensino_campo_flag"
					name="mod_ensino_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="mod_ensino_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'mod_ensino' );"
				><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="mod_ensino_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
					$sql_combo = "SELECT mdeid as codigo, mdecod ||' - '|| mdedsc as descricao FROM monitora.pi_modalidadeensino WHERE mdeano = '{$_SESSION['exercicio']}' order by mdecod";
					
					combo_popup(
						'f_mod_ensino',			// nome do campo
						$sql_combo,						// sql de captura dos valores
						'Selecione a(s) Modalidade de Ensino(s)',	// label
						'400x400',						// tamanho do popup
						0,								// quantidade máxima de itens
						array(),						// valores fixos
						'',								// mensagem
						'S',							// habilitado
						true,							// habilitar busca por código
						true							// habilitar flag contém
					);
					?>
				</div>
			</td>
		</tr>	
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'ptres' );">
				PTRES
				<input
					type="hidden"
					id="ptres_campo_flag"
					name="ptres_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="ptres_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'ptres' );"
				><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="ptres_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
						$sql_combo ="select
										ptres as codigo,
										ptres as descricao									
									 from 
										monitora.ptres 
									 where
										ptrano = '".$_SESSION['exercicio']."' and
										ptres != ''
									 group by
										ptres";
						
						combo_popup(
							'f_ptres',				// nome do campo
							$sql_combo,						// sql de captura dos valores
							'Selecione o(s) PTRES',			// label
							'400x400',						// tamanho do popup
							0,								// quantidade máxima de itens
							array(),						// valores fixos
							'',								// mensagem
							'S',							// habilitado
							true,							// habilitar busca por código
							true							// habilitar flag contém
						);
					?>
				</div>
			</td>
		</tr>		
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'planointerno' );">
				Plano Interno
				<input
					type="hidden"
					id="planointerno_campo_flag"
					name="planointerno_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="planointerno_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'planointerno' );"
				><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="planointerno_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
						$sql_combo ="select
										plicod as codigo,
										plicod || ' - ' || plititulo as descricao									
									 from 
										monitora.v_pi_detalhepiptres d
									 where
										d.prgano = '".$_SESSION['exercicio']."' and
										d.plicod is not null
									 group by
										plicod,
										plititulo";
						
						combo_popup(
							'f_planointerno',			// nome do campo
							$sql_combo,						// sql de captura dos valores
							'Selecione o(s) Plano Interno(s)',	// label
							'400x400',						// tamanho do popup
							0,								// quantidade máxima de itens
							array(),						// valores fixos
							'',								// mensagem
							'S',							// habilitado
							true,							// habilitar busca por código
							true							// habilitar flag contém
						);
					?>
				</div>
			</td>
		</tr>		
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'programa' );">
				Programa
				<input
					type="hidden"
					id="programa_campo_flag"
					name="programa_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="programa_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'programa' );"
				><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="programa_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
						$sql_combo ="SELECT
										prgcod as codigo,
										prgcod || ' - ' || prgdsc as descricao
									 FROM 
										planejamento.programa
									 WHERE
										prgano = '".$_SESSION['exercicio']."'
									 GROUP BY 
										prgcod,
										prgdsc";
						
						combo_popup(
							'f_programa',						// nome do campo
							$sql_combo,						// sql de captura dos valores
							'Selecione o(s) Programa(s)',	// label
							'400x400',						// tamanho do popup
							0,								// quantidade máxima de itens
							array(),						// valores fixos
							'',								// mensagem
							'S',							// habilitado
							true,							// habilitar busca por código
							true							// habilitar flag contém
						);
					?>
				</div>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'subacao' );">
				Subação
				<input
					type="hidden"
					id="subacao_campo_flag"
					name="subacao_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="subacao_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'subacao' );"
				><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="subacao_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
						$sql_combo ="SELECT 
									  s.sbaid as codigo,
									  s.sbaid || ' - ' || s.sbacod as descricao
									FROM 
									  monitora.subacao s inner join monitora.pi_subacaodotacao sd
									  on (s.sbaid = sd.sbaid) inner join monitora.ptres p
									  on (sd.ptrid = p.ptrid)
									WHERE p.ptrano = '".$_SESSION['exercicio']."'
										AND sbastatus = 'A'
									group by
									  s.sbaid, 
									  s.sbacod";

						combo_popup(
							'f_subacao',			// nome do campo
							$sql_combo,						// sql de captura dos valores
							'Selecione a(s) Subação(es)',	// label
							'400x400',						// tamanho do popup
							0,								// quantidade máxima de itens
							array(),						// valores fixos
							'',								// mensagem
							'S',							// habilitado
							true,							// habilitar busca por código
							true							// habilitar flag contém
						);
					?>
				</div>
			</td>
		</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="2" style="text-align: center;">
			<input type="button" value="Pesquisar" onclick="geraPopRelatorio();" style="cursor: pointer;"/>
		</td>
	</tr>
</table>
</form>