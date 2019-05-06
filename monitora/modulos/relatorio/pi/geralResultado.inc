<?php 
//class montaRelatorio{
//	private $db;
//	private $agrupador;
//	private $coluna;
//	private $erro;
//	private $relatorio;
//	private $dados;
//	
//	function __construct(){
//		global $db;
//		$this->db = $db;		
//	}
//	
//	function agrupar(){	
//		try {
//			if ( !is_array($this->dados) )	
//				throw new Exception("O Atributo DADOS não foi setado, ou não está no formato de ARRAY.");		
//			if ( !is_array($this->agrupador) )	
//				throw new Exception("O Atributo AGRUPADOR não foi setado, ou não está no formato de ARRAY.");
//		  	if ( !is_array($this->coluna) )	  
//				throw new Exception("O Atributo COLUNA não foi setado, ou não está no formato de ARRAY.");
//		  
//		  		  
//			  
//		} catch (Exception $e) {
//		  	$this->setErro("<B>Exceção na função de agrupar: </B>".  $e->getMessage(). "\n");
//		}
//	}
//	
//	function setAgrupador($dado = null, $dado1 = null){
//		$this->agrupador = $dado;
//		$this->dados	 = $dado1;
//	}
//	
//	function setColuna($dado = null){
//		$this->coluna = $dado;
//	}
//	
//	function setErro($dado){
//		$this->relatorio = $dado; 
//	}
//
////	function setDados($dado){
////		$this->dados = $dado;
////	}	
//	
//	function getRelatorio(){
//		return $this->relatorio;
//	}
//	
//}
//
//$agrupador = array(
//					"programa",
//					"acao",
//					"localizador",
//					"pi"
//				   );
//$coluna    = array(
//					"dotini",
//					"previsto",
//					"disponivel"
//				   );
//$r = new montaRelatorio();
//$r->setAgrupador($agrupador, $dados); 
//$r->setColuna($coluna);
//$r->agrupar();
//echo $r->getRelatorio();
//die;

$sql   = monta_sql();
//ver($sql, d);
//dbg($_POST,1);
//dbg($sql,1);

$label = monta_label($agpLabel);
$dados = $db->carregar($sql);

?>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<table cellspacing="1" cellpadding="5" border="0" align="center" class="tabela">
<?
if (!$dados){
?>
	<TR style="background:#FFF; color:red;">
		<TD colspan="10" align="center">
			Não há registros para o filtro
		</TD>
	</TR>
</table>

<?
}else{
	# Monta data da ultima atualização
	if (is_array($dados)) {	
		foreach ($dados as $date){
			if ($date['dataatu']){
				$datArr = explode("-",$dados[0][dataatu]);
				$data   = "{$datArr[2]}-{$datArr[1]}-{$datArr[0]}"; 
				break;
			}	
		}		
		unset($datArr);
	}
	
	$total  = count($dados);
	
	$dados = is_array($dados) ? $dados : array();
	
	$agp = $_POST['agrupador'];//array("programa", "acao", "unidgest", "pi");
	$agrupadorr = $agp;
	$dados = agrupar( $dados, $agp );
//	dbg($dados,1);
	echo '	<TR style="background:#D1D1D1;">';
	echo '		<TD valign="top" style="font-weight:bold;">'.$label.'</TD>';
	echo '		<TD align="center" valign="top" nowrap style="font-weight:bold;">Dotação Inicial</TD>';
	echo '		<TD align="center" valign="top" nowrap style="font-weight:bold;">Previsto</TD>';
	echo '		<TD align="center" valign="top" nowrap style="font-weight:bold;">Disponível</TD>';
	echo '	</TR>';
	exibir($dados);
	
	echo '	<TR style="background:#DFDFDF;">
				<TD>&nbsp;&nbsp;&nbsp;&nbsp;<b>Totais:</b></TD>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($totdotini,2,',','.').'</td>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($totprevisto,2,',','.').'</td>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($totdisponivel,2,',','.').'</td>	
			</TR>';	
	
	$data = $data ? "* Dados financeiros atualizados até: $data" : "";		
		
	echo '<TR style="background:#FFFFFF;">
			<TD colspan="10" align="right" style="font-weight:bold; font-size:9px; border-top:2px solid black; border-bottom:2px solid black;"><div style="float:left; font-size:11px;">Total de registros: '.$total.'</div>'.$data.'</TD>
		  </tr>';
	echo "</table>";

}

function monta_sql(){
	global $agpLabel;
	
	$select   = array();
	$from	  = array();
	$group	  = array();
	$agpLabel = array();
	
	$agp = (array) $_POST['agrupador'];

	foreach ($agp as $val): 
		switch ($val) {
		    case 'programa':
				array_push($select,   " v.prgcod || ' - ' || pr.prgdsc AS programa ");
				array_push($from, 	  " INNER JOIN planejamento.programa pr ON pr.prgcod = v.prgcod and pr.prgano = v.prgano ");
				array_push($group, 	  " v.prgcod, prgdsc ");											
				array_push($agpLabel, " Programa ");				
		    	continue;
		        break;
		    case 'acao':
				array_push($select, " acacod || ' - ' || acadsc AS acao ");
				array_push($group, 	  " acacod, acadsc ");
				array_push($agpLabel, " Ação ");				
		    	continue;
		        break;		    	
		    case 'unidorc':
				array_push($select, " e.entunicod || ' - ' || e.entnome AS unidorc ");
				array_push($from, " INNER JOIN entidade.entidade e ON e.entunicod = v.unicod 
									INNER JOIN entidade.funcaoentidade fen ON fen.entid = e.entid AND fen.funid NOT IN (31,32,34) ");
				array_push($group, 	  " e.entunicod, e.entnome ");
				array_push($agpLabel, " Unidade Orçamentária ");				
		    	continue;			
		        break;		    	
		    case 'localizador':
				array_push($select, " v.prgcod || ' - ' || acacod || ' - ' || unicod || ' - ' || loccod || ' - ' || acadsc || ' ( ' || sacdsc || ' ) ' AS localizador ");
				array_push($group, 	  " v.prgcod, acacod, unicod, loccod, acadsc, sacdsc ");
				array_push($agpLabel, " Localizador ");				
		    	continue;
		        break;		    	
		    case 'ptres':
				array_push($select, " ptres AS ptres ");
				array_push($group, 	  " ptres ");
				array_push($agpLabel, " PTRES ");				
		    	continue;
		        break;		    	
		    case 'pi':
				array_push($select, " COALESCE(v.plicod,' ')||' - '||coalesce(v.plititulo,'Não Informado') AS pi ");
				array_push($group, 	  " v.plicod, v.plititulo ");
				array_push($agpLabel, " Plano Interno ");				
		    	continue;
		        break;	
		    case 'eqdid':
				array_push($select, " COALESCE(cdt1.cdtcod || ' - ' || cdt1.cdtdsc,'Não Informado') AS eqdid ");
				array_push($from, 	  " LEFT JOIN public.combodadostabela cdt1 ON cdt1.cdtid = v.eqdid ");
				array_push($group, 	  " cdt1.cdtcod, cdt1.cdtdsc ");
				array_push($agpLabel, " Enquadramento da Despesa ");				
		    	continue;
		        break;	
		    case 'dpiid':
				array_push($select, " COALESCE(cdt2.cdtcod || ' - ' || cdt2.cdtdsc,'Não Informado') AS dpiid ");
				array_push($from, 	  " LEFT JOIN public.combodadostabela cdt2 ON cdt2.cdtid = v.dpiid ");
				array_push($group, 	  " cdt2.cdtcod, cdt2.cdtdsc ");
		    	array_push($agpLabel, " Nível/Etapa/Modalidade de ensino ");				
		    	continue;
		        break;	
		    case 'cpiid':
				array_push($select, " COALESCE(cdt3.cdtcod || ' - ' || cdt3.cdtdsc,'Não Informado') AS cpiid ");
				array_push($from, 	  " LEFT JOIN public.combodadostabela cdt3 ON cdt3.cdtid = v.cpiid ");
				array_push($group, 	  " cdt3.cdtcod, cdt3.cdtdsc ");
		    	array_push($agpLabel, " Categoria de Apropriação ");				
		    	continue;
		        break;
		   case 'sbaid':
                array_push($select, " COALESCE(sa.sbacod || ' - ' || sa.sbatitulo,'Não Informado') AS sbaid ");
                array_push($from, " LEFT JOIN monitora.subacao sa ON sa.sbaid = pi.sbaid ");
                array_push($group, " sa.sbacod, sa.sbatitulo ");
                array_push($agpLabel, " Subação ");                                
                continue;
                break;
		   case 'atiid':
		   		array_push($select, " COALESCE(ati._atinumero || ' - ' || ati.atidescricao, 'Não Informado') AS atiid ");
		   		array_push($from,   " LEFT JOIN monitora.planointerno pi on (v.plicod = pi.plicod) ");
             	array_push($from,   " LEFT JOIN monitora.planointernoatividade pia ON (pi.pliid = pia.pliid) ");
			 	array_push($from,   " LEFT JOIN pde.atividade ati ON (pia.atiid = ati.atiid) ");
				array_push($group, 	  " ati._atinumero, ati.atidescricao ");
		    	array_push($agpLabel, " Atividade ");
		    	continue;
		        break;
		}
	endforeach;

	reset($agp);  
	$order = current($agp)." ASC ";
	
	
	$sql = "SELECT
			sum(coalesce(ptrdotacao,0)) AS dotini,
			sum(coalesce(valorpi,0)) as previsto,
			CASE 
				WHEN coalesce(ptrdotacao,0) = 0 THEN - sum(coalesce(valorpi,0))
				ELSE round(sum(coalesce(ptrdotacao,0)) - sum(coalesce(valorpi,0)),2) 
			END AS disponivel,
			".(implode(' , ', $select))."
		FROM
			monitora.v_detalhepiptres v
			".(implode(' ', $from))."
		WHERE 
			v.prgano='".$_SESSION['exercicio']."'
			".sqlFiltro()."		
		GROUP BY
			ptrdotacao, 
			".(implode(' , ', $group))."
		ORDER BY
			".$order;
	return $sql;
}

function sqlFiltro(){
	$where = array();
	
	$pi  = (array) $_POST['f_planointerno'];
	$prg = (array) $_POST['f_programa'];
	$ac	 = (array) $_POST['f_acao'];
	$uo  = (array) $_POST['f_uo'];
	$ptr = (array) $_POST['f_ptres'];
	$sub = (array) $_POST['f_subacao'];
	$atv = (array) $_POST['f_atividade'];
	$cap = (array) $_POST['f_cat_apropriacao'];
	$enq_Despesa = (array) $_POST['f_enq_Despesa'];
	$localizador = (array) $_POST['f_localizador'];
	$mod_ensino = (array) $_POST['f_mod_ensino'];

	$flag_pi  = $_POST['f_planointerno_campo_excludente'];
	$flag_prg = $_POST['f_programa_campo_excludente'];
	$flag_ac  = $_POST['f_acao_campo_excludente'];
	$flag_uo  = $_POST['f_uo_campo_excludente'];
	$flag_ptr = $_POST['f_ptres_campo_excludente'];
	$flag_sub = $_POST['f_subacao_campo_excludente'];
	$flag_cap = $_POST['f_cat_apropriacao_campo_excludente'];
	$flag_enq_Despesa = $_POST['f_enq_Despesa_apropriacao_campo_excludente'];
	$flag_localizador = $_POST['f_localizador_apropriacao_campo_excludente'];
	$flag_mod_ensino = $_POST['f_mod_ensino_apropriacao_campo_excludente'];
	
	$t_pi  = $_POST['pi_campo_flag'];
	$t_prg = $_POST['programa_campo_flag'];
	$t_ac  = $_POST['acao_campo_flag'];
	$t_uo  = $_POST['uo_campo_flag'];
	$t_ptr = $_POST['ptr_campo_flag'];
	$t_sub = $_POST['subacao_campo_flag'];
	$t_cap = $_POST['cat_apropriacao_campo_flag'];
	$t_enq_Despesa = $_POST['enq_Despesa_campo_flag'];
	$t_localizador = $_POST['localizador_campo_flag'];
	$t_mod_ensino = $_POST['mod_ensino_campo_flag'];

	if ($pi[0] && $t_pi){
		$where[] = " v.plicod ".(!$flag_pi ? ' IN ' : ' NOT IN ')."('".implode("','",$pi)."') ";
	}

	if ($prg[0] && $t_prg){
		$where[] = " v.prgcod ".(!$flag_prg ? ' IN ' : ' NOT IN ')."('".implode("','",$prg)."') ";
	}

	if ($ac[0] && $t_ac){
		$where[] = " v.acacod ".(!$flag_ac ? ' IN ' : ' NOT IN ')."('".implode("','",$ac)."') ";
	}
	
	if ($uo[0] && $t_uo){
		$where[] = " v.unicod ".(!$flag_uo ? ' IN ' : ' NOT IN ')."('".implode("','",$uo)."') ";
	}

	if ($ptr[0] && $t_ptr){
		$where[] = " v.ptres ".(!$flag_ptr ? ' IN ' : ' NOT IN ')."('".implode("','",$ptr)."') ";
	}
	
	if ($sub[0] && $t_sub){
		$where[] = " sa.sbaid ".(!$flag_sub ? ' IN ' : ' NOT IN ')."('".implode("','",$sub)."') ";
	}
	if ($atv[0] && $t_atv){
		$where[] = " ati.atiid ".(!$flag_atv ? ' IN ' : ' NOT IN ')."('".implode("','",$atv)."') ";
	}
	if ($cap[0] && $t_cap){
		$where[] = " cdt3.cdtcod ".(!$flag_cap ? ' IN ' : ' NOT IN ')."('".implode("','",$cap)."') ";
	}
	if ($enq_Despesa[0] && $t_enq_Despesa){
		$where[] = " cdt1.cdtid ".(!$flag_enq_Despesa ? ' IN ' : ' NOT IN ')."('".implode("','",$enq_Despesa)."') ";
	}
	if ($localizador[0] && $t_localizador){
		$where[] = " acaid ".(!$flag_localizador ? ' IN ' : ' NOT IN ')."('".implode("','",$localizador)."') ";
	}
	if ($mod_ensino[0] && $t_mod_ensino){
		$where[] = " cdt2.cdtid ".(!$flag_mod_ensino ? ' IN ' : ' NOT IN ')."('".implode("','",$mod_ensino)."') ";
	}
	
	return $where ? " AND (".implode(' AND ', $where).")" : '';
}


function monta_label($agrupadores){
	foreach($agrupadores as $val):
		$nivel++;
		
		$label .= $nivel == 1 ? 
								"<div>{$val}</div>" 
							  : 
							  	"<div style=\"padding-left:".(20*($nivel-1))."px;\">
							  		<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\"/> {$val} 
							  	</div>";		 
	endforeach;
	
	return $label;
}


function agrupar( $lista, $agrupadores ) {
	global $agrupadorr;
	
	$existeProximo = count( $agrupadores ) > 0; 
	if ( $existeProximo == false )
	{
		return array();
	}
	$campo = array_shift( $agrupadores );
	$novo = array();
	foreach ( $lista as $item ):
	
		$chave = $item[$campo];
		if ( array_key_exists( $chave, $novo ) == false ){			
			$novo[$chave] = array(
				"dotini" 	 => 0,
				"previsto"   => 0,
				"disponivel" => 0,
				"sdotini" 	 => 0,
				"sprevisto"   => 0,
				"sdisponivel" => 0,			
				"agrupador"  => $campo,
				"sub_itens"  => array()
			);
		}
		

			$novo[$chave]['dotini']     += $item['dotini'];
			$novo[$chave]['previsto']   += $item['previsto'];
			$novo[$chave]['disponivel'] += $item['disponivel'];				
			
//			if ($campo == 'pi'){
			$novo[$chave]['sdotini']     += $item['dotini'];
			$novo[$chave]['sprevisto']   += $item['previsto'];
			$novo[$chave]['sdisponivel'] += $item['disponivel'];
//			}
		if ( $existeProximo )
			array_push( $novo[$chave]["sub_itens"], $item );

	endforeach;
	if ( $existeProximo ):
		foreach ( $novo as $chave => $dados )
			$novo[$chave]["sub_itens"] = agrupar( $novo[$chave]["sub_itens"], $agrupadores );		
	endif;
	return $novo;
}
			
			
function exibir( $lista, $profundidade = 0 ){
	global $agrupadorr,$totdotini,$totprevisto,
		   $totdisponivel;	
		   
	if ( count( $lista ) == 0 ){		
		return;
	}	
	$contCor = 0;
	foreach ( $lista as $chave => $dados ):
		$agrupador  	 = (string) $dados["agrupador"];
		$dotini 		 = (float) $dados["dotini"];
		$previsto		 = (float) $dados["previsto"];
		$disponivel		 = (float) $dados['disponivel'];

		$sdotini 	 = (string) $dados['sdotini'];	
		$sprevisto   = (string) $dados['sprevisto'];	
		$sdisponivel = (string) $dados['sdisponivel'];	

		if ($profundidade == 0)
			echo '<tr bgcolor="#CCCCCC" onmouseout="this.bgColor=\'#CCCCCC\';" onmouseover="this.bgColor=\'#ffffcc\';">';
		elseif ($profundidade == 1)
			echo '<tr bgcolor="#DDDDDD" onmouseout="this.bgColor=\'#DDDDDD\';" onmouseover="this.bgColor=\'#ffffcc\';">'; 
		elseif ($profundidade == 2)
			echo '<tr bgcolor="#E9E9E9" onmouseout="this.bgColor=\'#E9E9E9\';" onmouseover="this.bgColor=\'#ffffcc\';">'; 		
		elseif ($profundidade == 3)
			echo '<tr bgcolor="#f1f1f1" onmouseout="this.bgColor=\'#f1f1f1\';" onmouseover="this.bgColor=\'#ffffcc\';">'; 		
		else
			echo '<tr bgcolor="#f9f9f9" onmouseout="this.bgColor=\'#f9f9f9\';" onmouseover="this.bgColor=\'#ffffcc\';">';
?>
			<td style="padding-left:<?= $profundidade * 20 ?>px; font-size:12px;">
				<?php if ( $profundidade > 0 ): ?><img src="../imagens/seta_filho.gif" align="absmiddle"/><?php else: echo '&nbsp;'; endif;
						echo "<b>".$chave."</b>";
				?>		
			</td>
<?php 
		$cl = 'rgb(0, 102, 204);';
		
		if ( $agrupador == current($agrupadorr) ): 
			$totdotini 	   += $sdotini;
			$totprevisto   += $sprevisto;
			$totdisponivel += $sdisponivel;
		endif;
?>		
			<td align="right" style="color: <?=$cl?>;"><?=($agrupador == 'pi' || $agrupador == 'eqdid' || $agrupador == 'cpiid' || $agrupador == 'dpiid' || $agrupador == 'ppiid') ? '-' : number_format($sdotini,2,',','.');?></td>
			<td align="right" style="color: <?=$cl?>;"><?=number_format(($agrupador == 'pi' ? $previsto : $sprevisto ),2,',','.');?></td>
			<td align="right" style="color: <?=$cl?>;"><?=($agrupador == 'pi' || $agrupador == 'eqdid' || $agrupador == 'cpiid' || $agrupador == 'dpiid' || $agrupador == 'ppiid') ? '-' : number_format($sdisponivel,2,',','.'); ?></td>	
		</tr>
<?		
		exibir( $dados["sub_itens"], $profundidade + 1 );
		$contCor++;	
	endforeach;
}

?>