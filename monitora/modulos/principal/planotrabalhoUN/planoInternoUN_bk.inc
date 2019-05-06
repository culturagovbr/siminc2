<?php

ini_set( "memory_limit", "1024M" );
set_time_limit(0);

include APPRAIZ. 'includes/classes/relatorio.class.inc';
// carrega as funções de integração
include "planotrabalhoUN/unidade_atividade_funcoes.php";

include APPRAIZ. '/includes/Agrupador.php';

// carrega as funções do módulo pde
include "planotrabalhoUN/_constantes.php";
include "planotrabalhoUN/_funcoes.php";
include "planotrabalhoUN/_componentes.php";


// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// montando o menu
echo montarAbasArray(carregardadosplanotrabalhoUN_sub(), "/monitora/monitora.php?modulo=principal/planotrabalhoUN/planoInternoUN&acao=A&atiid=".$_REQUEST['atiid']);



// obtém dados da atividade vinculada à ação
$atividade = retornaTarefaUnidade( $_SESSION['monitora_var']['unicod'], $_SESSION['exercicio'] );

$db->commit();
if ( $_REQUEST["atiid"] ) {
	$atividade = atividade_pegar( $_REQUEST["atiid"] );
} else {
	$_REQUEST["atiid"] = $atividade["atiid"];
	$atividade = atividade_pegar( $_REQUEST["atiid"] );
}

montar_titulo_projeto( $atividade['atidescricao'] );


//$var = $_REQUEST['visualiza'] == '' ? 'PDE' : $_REQUEST['visualiza'];
//$suprimir = $_REQUEST['suprimir'] == '0' ? 'plano_interno' : '';
?>
<script type="text/javascript">
function exibirRelatorio() {
	var formulario = document.formulario;
	formulario.action='monitora.php?modulo=principal/acao/planoInterno&acao=A';
	formulario.submit();
}

function pesqPlanoInterno () {
	janela = window.open('pde.php?modulo=principal/atividade_/pesqPlanoInterno&acao=A&atiid=<?php echo $_GET[atiid]; ?>', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width='+(screen.width-120)+',height=680' ); janela.focus();
}

function directAcao (acaid){}

function alterAgrupador(){
	var d =  document;
	var f = d.formulario;
	
	selectAllOptions( f.agrupador );

	if(!f.agrupador.value){
		alert('Selecione pelo menos um agrupador!');
		return false;
	}
	f.submit();
}
</script>

<form name="formulario" id="formulario" action="monitora.php?modulo=principal/planotrabalho/planoInterno2&acao=A&atiid=<?=$_REQUEST['atiid']?>" method="post">	
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top">Agrupadores</td>
		<td>
			<?
			$matriz 	   = agrupador();
			$matrizDefault = agrupadorDefault();
			$campoAgrupador = new Agrupador( 'formulario' );
			$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
			$campoAgrupador->setDestino( 'agrupador', null, $matrizDefault);
			$campoAgrupador->exibir();
			?>
		</td>
	</tr>
	<tr style="background-color: #ccc;">
		<td colspan="2" style="text-align: center;">
		<input type="button" name="gerar" value="Gerar" onclick="javascript: alterAgrupador();">
		</td>
	</tr>	
</table>		
</form>
<?php
//$atiordematual = $db->pegaUm("SELECT _atinumero FROM pde.atividade WHERE atiid = '".$_REQUEST['atiid']."'");
//		 		COALESCE( sum(v.rofdotori), 0 ) AS rofdotori,
//		 		COALESCE( sum(v.rofautorizado), 0 ) AS rofautorizado,
//		 		COALESCE( sum(v.rofempenhado), 0 ) AS empenhado,
//		 		COALESCE( sum(v.rofliquidado_favorecido), 0 ) AS rofliquidado_favorecido,
//		 		COALESCE( sum(v.rofpago), 0 ) as rofpago,
/*
$sql1  = "select 
--				esp.espano,
--			    esp.acacod, 
--			    esp.sbaid,
--			    esp.plicod, 
--			    esp.ptres,
--				pl.plivalor,
				COALESCE(pl.plicod || ' - ' || pl.plititulo, 'Não preenchido') AS pi,
			  	a.prgcod || '.' || a.acacod || '.' || a.unicod || '.' || a.loccod || ' - ' || a.acadsc || ' - ' || a.sacdsc AS acao,
			  	sa.sbacod || ' - ' || sa.sbatitulo AS subacao,
			
			    --(
			    --    select    
			    --    	sum(rofdot_pi)
			    --    from    
			    --    	financeiro.execucao a
			    --    where    
			    --    	a.rofano = esp.rofano
			    --    	and    a.acacod = esp.acacod
			    --    	and    a.plicod = esp.plicod
			    --    	and     a.ptres  = esp.ptres
			    --)    as vlrdotacao,
			    (
			        select     
			        	sum(rofempenhado)
			        from    
			        	financeiro.execucao a
			        where    
			        	a.rofano = esp.rofano
			        	and    a.acacod = esp.acacod
			        	and    a.plicod = esp.plicod
			        	and     a.ptres  = esp.ptres
			    )    as vlrempenhado,
			    (
			        select     
			        	sum(rofliquidado_favorecido)
			        from    
			        	financeiro.execucao a
			        where    
			        	a.rofano = esp.rofano
			        	and    a.acacod = esp.acacod
			        	and    a.plicod = esp.plicod
			        	and     a.ptres  = esp.ptres
			    )    as vlrliquidado,
			    (
			        select     
			        	sum(rofpago)
			        from    
			        	financeiro.execucao a
			        where    
			        	a.rofano = esp.rofano
			        	and    a.acacod = esp.acacod
			        	and    a.plicod = esp.plicod
			        	and     a.ptres  = esp.ptres
			    )    as vlrpago
			
			from  
				financeiro.execucao esp
	  		  	INNER JOIN monitora.planointerno pl on pl.plicod = esp.plicod AND
	  		  					       pl.plistatus = 'A'
	  		  	LEFT JOIN monitora.subacao sa ON sa.sbaid = pl.sbaid AND
	  		 					 sa.sbastatus = 'A'			
	  		  	LEFT JOIN planejamento.acao a ON a.acacod = esp.acacod AND
	  		  				     a.acastatus = 'A'				
			where
				esp.rofano = '" . $_SESSION['exercicio'] . "'
			order by 
			    esp.rofano,
			    esp.acacod,
			    esp.plicod,
			    esp.ptres;";
*/

$sql1  = "select 
				COALESCE(p.plicod || ' - ' || p.plititulo, 'Não preenchido') AS pi,
				a.prgcod || '.' || a.acacod || '.' || a.unicod || '.' || a.loccod || ' - ' || a.acadsc || ' - ' || a.sacdsc AS acao,
				s.sbacod || ' - ' || s.sbatitulo AS subacao,
				sum(rofempenhado) as vlrempenhado,
				sum(rofliquidado_favorecido) as vlrliquidado,
				sum(rofpago) as vlrpago
			from
				pde.planointerno p
				inner join monitora.planointernoatividade pia ON pia.pliid = p.pliid
				inner join monitora.subacao s ON s.sbaid = p.sbaid
				inner join planejamento.acao a ON a.acaid = p.acaid
				inner join financeiro.execucao e ON e.plicod = p.plicod and e.acacod = a.acacod
			where 
				a.prgano ='".$_SESSION['exercicio']."' and 
				e.rofano ='".$_SESSION['exercicio']."' and 
				p.pliorigem = 'm' and 
				pia.atiid = '".$_REQUEST['atiid']."'
			group by 
					p.plicod||' - '||p.plititulo,
					a.prgcod || '.' || a.acacod || '.' || a.unicod || '.' || a.loccod || ' - ' || a.acadsc || ' - ' || a.sacdsc,
					s.sbacod || ' - ' || s.sbatitulo";

//dbg($sql1,1);

$dados = $db->carregar($sql1);

//dbg($dados,1);

$agrup = monta_agp();
$col   = monta_coluna();
//dbg($agrup,1);
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setTolizadorLinha(true);
$r->setTotNivel(true);
//$r->setBrasao(true);
echo $r->getRelatorio();

function monta_agp(){
	$agrupador = $_POST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
									//		"atiid",
									//		"acaid",
									//		"plivalor",
									//		"vlrdotacao",
											"vlrempenhado",
											"vlrliquidado",
											"vlrpago",
//											"autorizado_porcentagem",
//											"rofdotori",
//											"autorizado_porcentagem",
//											"totalatividade"	
//											"sempenhado",
//											"agrupador",
//											"sub_itens"
										  )	  
				);
				
					
	
				
									   				
				
									   				
									   				
	if ($agrupador){								   															   				
		foreach ($agrupador as $val): 
			switch ($val) {
			    case 'acao':
					array_push($agp['agrupador'], array(
														"campo" => "acao",
												  		"label" => "Ação")										
										   				);
					continue;
			        break;
			    case 'pi':
					array_push($agp['agrupador'], array(
														"campo" => "pi",
												  		"label" => "Plano Interno")										
										   				);				    	
			    	continue;
			        break;		    	
			    case 'subacao':
				array_push($agp['agrupador'], array(
													"campo" => "subacao",
											  		"label" => "Sub-Ação")										
									   				);					
			    	continue;			
			        break;	
			}
		endforeach;
	}else{
		array_push($agp['agrupador'], array(
											"campo" => "subacao",
									  		"label" => "Sub-Ação")										
							   				);
					   					
		array_push($agp['agrupador'], array(
											"campo" => "pi",
									  		"label" => "Plano Interno")										
							   				);		   				
	}
	
	return $agp;
}

function monta_coluna(){
	$coluna    = array(					
						/*array(
							  "campo" 	 => "plivalor",
					   		  "label" 	 => "Valor Previsto"
						),
						array(
							  "campo" => "vlrdotacao",
					   		  "label" => "Dotação Original"
						),	
//						array(
//							  "campo" => "rofautorizado",
//					   		  "label" => "Lei + Créditos <BR>(Autorizado)"	
//						),*/
						array(
							  "campo" 	 => "vlrempenhado",
					   		  "label" 	 => "Empenhado"
						),
						array(
							  "campo" 	 => "vlrliquidado",
					   		  "label" 	 => "Liquidado"
						),	
						array(
							  "campo" => "vlrpago",
					   		  "label" => "Pago"
						)				
//						array(
//							  "campo" => "autorizado_porcentagem",
//					   		  "label" => " 	% do Pago<BR> s/ <BR>Autorizado"
//						),				
//						array(
//							  "campo" 	 => "plivalor",
//					   		  "label" 	 => "Valor Previsto"	
////					   		  "html"  => "{plivalor} - {plivalor}",
////							  "type"  => "numeric",	
////							  "eval"  => true	
//						),				
//						array(
//							  "campo" => "pago",
//					   		  "label" => "Pago"
//						)				
						
					  );
				  	
	return $coluna;			  	
}

function agrupador(){
	$agp = is_array($_REQUEST['agrupador']) ? $_REQUEST['agrupador'] : array(); 
	$array = array();

	if ($agp){	
		if ( !in_array('acao', $agp) ){
			array_push($array, array(
			 						 'codigo' => 'acao',
							  		 'descricao' => 'Ação'
									)
					  );	
		}
		
		if ( !in_array('subacao', $agp) ){
			array_push($array, array(
									  'codigo' => 'subacao',
									  'descricao' => 'Sub-Ação'
								 	 )
					  );	
		}		

		if ( !in_array('pi', $agp) ){
			array_push($array, array(
									  'codigo' => 'pi',
									  'descricao' => 'Plano Interno'
									 )
					  );	
		}		
		
	}else{
		array_push($array, array(
	 						 'codigo' => 'acao',
					  		 'descricao' => 'Ação'
							)
			  );
	}
	return $array;
}

function agrupadorDefault(){
	$agp = is_array($_REQUEST['agrupador']) ? $_REQUEST['agrupador'] : array(); 
	$array = array();
	
	if ($agp){	
		if ( in_array('acao', $agp) ){
			array_push($array, array(
			 						 'codigo' => 'acao',
							  		 'descricao' => 'Ação'
									)
					  );	
		}
		
		if ( in_array('subacao', $agp) ){
			array_push($array, array(
									  'codigo' => 'subacao',
									  'descricao' => 'Sub-Ação'
								 	 )
					  );	
		}		

		if ( in_array('pi', $agp) ){
			array_push($array, array(
									  'codigo' => 'pi',
									  'descricao' => 'Plano Interno'
									 )
					  );	
		}		
		
	}else{
		array_push($array, array(
	 						 'codigo' => 'subacao',
					  		 'descricao' => 'Sub-Ação'
							)
			  );
		array_push($array, array(
	 						 'codigo' => 'pi',
					  		 'descricao' => 'Plano Interno'
							)
			  );			  
	}

	return $array;				
}
?>
