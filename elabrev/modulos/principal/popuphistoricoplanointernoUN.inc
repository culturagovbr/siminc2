<?php
include APPRAIZ . "monitora/www/planotrabalhoUN/_constantes.php";
include APPRAIZ . "monitora/www/planotrabalhoUN/_funcoes.php";
require_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";
require_once APPRAIZ . "monitora/classes/Pi_PlanoInternoHistorico.class.inc";

extract($_GET);

$obPi_PlanoInterno = new Pi_PlanoInterno($pliid);

if($obPi_PlanoInterno->pliid){
//$obPi_PlanoInterno->pliid =1030; 
	$sql = "SELECT
				pl.pliid,
				ptr.ptres,
				pt.ptrid,
				pt.pipvalor, 
				ptr.acaid,
				trim(ac.prgcod||'.'||ac.acacod||'.'||ac.unicod||'.'||ac.loccod||' - '||ac.acadsc) as descricao,
				sum(ptr.ptrdotacao) as dotacaoinicial,
				round(sum( coalesce(sad.sadvalor,0) ),2) as dotacaosubacao,
				coalesce((SELECT SUM(pipvalor) as valor FROM monitora.pi_planointernoptres pt2 inner join monitora.pi_planointerno p ON p.pliid = pt2.pliid WHERE pt.ptrid = pt2.ptrid AND p.plistatus='A'),0) as detalhamento
			FROM monitora.pi_planointerno pl
			INNER JOIN monitora.pi_planointernoptres pt ON pt.pliid = pl.pliid 
			LEFT JOIN monitora.ptres ptr ON ptr.ptrid = pt.ptrid 
			LEFT JOIN monitora.acao ac ON ac.acaid = ptr.acaid
			LEFT JOIN monitora.pi_subacaodotacao sad ON ptr.ptrid = sad.ptrid and sad.sbaid = pl.sbaid
			LEFT JOIN ( select sbaid, ptrid, 
						sum( sadvalor ) as valor
						from  monitora.pi_subacaodotacao
						group by sbaid, ptrid ) dt ON ptr.ptrid = dt.ptrid and dt.sbaid = sad.sbaid
			WHERE
					pl.pliid = '".$obPi_PlanoInterno->pliid."' AND
					pl.plistatus='A'
	    	GROUP BY pl.pliid, pt.ptrid, ptr.ptres, pl.plistatus, pt.pipvalor, ac.prgcod, ptr.acaid, ac.acacod, ac.unicod, ac.loccod, ac.acadsc
	    	ORDER BY ptr.ptres";
	
	$acoespl = $db->carregar($sql);
}
	$acoespl = ($acoespl) ? $acoespl : array();

?>
<html>
  <head>
    <script type="text/javascript" src="/includes/prototype.js"></script>
    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
  </head>
<br>    
<form name="formulario" id="formulario" method="post">
<table  bgcolor="#f5f5f5" align="center" class="tabela" >
	<tr>
		<td class = "subtitulodireita" colspan="2">
			<center> 
			<h3>Dados do plano interno</h3>
			</center>
		</td> 
	</tr>
	<tr>
		<td align="center" colspan="2">
			<?php echo $unidade = $db->pegaUm("SELECT unicod || ' - ' || unidsc FROM public.unidade WHERE unicod = '".$_SESSION['monitora_var']['unicod']."'"); ?>
		</td>
	</tr>
	<tr>
		<td colspan="2">
			<table cellpadding="0" border="0" width="98%" align="center" id="orcamento"  style="BORDER-RIGHT: #C9C9C9 1px solid; BORDER-TOP: #C9C9C9 1px solid; BORDER-LEFT: #C9C9C9 1px solid; BORDER-BOTTOM: #C9C9C9 1px solid;" onmouseover="tabindexcampo();">
				<tr>
					<td style="background-color: #C9C9C9;" colspan="7" align="center"><b>Detalhamento Orçamentário</b></td>
				</tr>
				<tr>
					<td style="background-color: #C9C9C9;" align="center" nowrap><b>PTRES</b><input type="hidden" name="pliptres"></td>
					<td style="background-color: #C9C9C9; width:45%;" align="center" nowrap><b>Ação</b></td>
					<td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Dotação Inicial</b></td>
					<td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Dotação Subação</b></td>
					<td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Detalhado no PI</b></td>
					<td style="background-color: #C9C9C9; width:100px;" align="center"><b>Dotação Disponível</b></td>
					<td style="background-color: #C9C9C9;" align="center"><b>Valor Previsto(Anual)</b></td>
				</tr>
				<?
				 
				if($acoespl[0]) {
					$valortotalpi = 0;
					$cor = 0;
					foreach($acoespl as $acpl) { 
				?>
			        <tr style="height:30px;<? echo (($cor%2)?"":"background-color:#DCDCDC;"); ?>" id="ptres_<? echo $acpl['ptres']; ?>">
						<td align="center"><? echo $acpl['ptres']; ?></td>
						<td align="left"><? echo $acpl['descricao']; ?></td>
					    <td align="right"><? echo number_format($acpl['dotacaoinicial'],2,',','.'); ?></td>
					    <td align="right"><? echo number_format($acpl['dotacaosubacao'],2,',','.'); ?></td>
					    <td align="right"><? echo number_format($acpl['detalhamento'],2,',','.'); ?></td>
					    <td align="right"><? if ( $acpl['dotacaosubacao'] > 0 ) { echo number_format(($acpl['dotacaosubacao']-$acpl['detalhamento']),2,',','.'); } else { echo number_format(($acpl['dotacaoinicial']-$acpl['detalhamento']),2,',','.'); }?></td>
					    <td align="center"><input type="text" name="plivalored[<? echo $acpl['ptrid']; ?>]" size="28" maxlength="" value="<? echo number_format($acpl['pipvalor'],2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);calculovalorPI();"  disabled class="disabled"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this); verificaDisponivel(this,'<? echo $acpl['ptres']; ?>','<? echo number_format($acpl['pipvalor'],2,',','.'); ?>');" style="text-align : right; width:25ex;" title='' /></td>
					</tr>
				<? 
						$cor++;
						$valortotalpi = $valortotalpi + $acpl['pipvalor']; 
					}
				} 
				?>
				<tr style="height: 30px;">
					<td align="right" valign="top" colspan="6"><b>TOTAL :</b></td>
					<td align="center" valign="top"><input type="text" name="valortotalpi" id="valortotalpi" size="28" maxlength="" value="<? echo number_format($valortotalpi,2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);" disabled  class="disabled"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : right; width:25ex;" title='' /></td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class = "subtitulodireita">Enquadramento da Despesa:</td>
		<td><?php 
				if($obPi_PlanoInterno->eqdid){
					echo $db->pegaUm("SELECT eqdcod || ' - ' || eqddsc FROM planejamento.enquadramento_despesa WHERE eqdid='".$obPi_PlanoInterno->eqdid."'");
				}

			?>
		</td>
	</tr>
	<tr>
		<td class = "subtitulodireita">Subação:</td>
		<td><?php 
			if($obPi_PlanoInterno->sbaid){
				echo $db->pegaUm("SELECT sbacod || ' - ' || sbatitulo FROM monitora.pi_subacao WHERE sbaid='".$obPi_PlanoInterno->sbaid."'"); 
			}
			?></td>
	</tr>
	<tr>
		<td class = "subtitulodireita">Nível/Etapa de Ensino:</td>
		<td><?php echo $db->pegaUm("SELECT neecod || ' - ' || needsc FROM monitora.pi_niveletapaensino WHERE neeid='".$obPi_PlanoInterno->neeid."'"); 
?></td>
	</tr>
	<tr>
		<td class = "subtitulodireita">Categoria de Apropriação:</td>
		<td><?php echo $db->pegaUm("SELECT capcod || ' - ' || capdsc FROM monitora.pi_categoriaapropriacao WHERE capid='".$obPi_PlanoInterno->capid."'"); ?></td>
	</tr>
	<tr>
		<td class = "subtitulodireita">Codifição da Unidade(livre):</td>
		<td><?php echo $obPi_PlanoInterno->plilivre; ?></td>
	</tr>
	<tr>
		<td class = "subtitulodireita">Modalidade de Ensino/Tema/Público:</td>
		<td><?php
		 		if($obPi_PlanoInterno->mdeid){
					echo $db->pegaUm("SELECT mdecod || ' - ' || mdedsc FROM monitora.pi_modalidadeensino WHERE mdeid='".$obPi_PlanoInterno->mdeid."'"); 
		 		}
			?>
		</td>
	</tr>
	<tr>
		<td class = "subtitulodireita">Título:</td>
		<td><?php echo $obPi_PlanoInterno->plititulo; ?></td>
	</tr>
	<tr>
		<td class = "subtitulodireita">Código do PI:</td>
		<td><?php echo $obPi_PlanoInterno->plicod; ?></td>
	</tr>
	<tr>
		<td class = "subtitulodireita">Descrição do PI:</td>
		<td><?php echo $obPi_PlanoInterno->plidsc; ?></td>
	</tr>
	<tr>
		<td colspan="2">
		<?php 
			$sql = "SELECT 
					    to_char(pih.pihdata, 'DD/MM/YYYY HH24:MI:SS') as pihdata,
					    CASE WHEN pih.pihsituacao = 'P' THEN  ' Pendente '
					         WHEN pih.pihsituacao = 'A' THEN ' Aprovado ' 
					         WHEN pih.pihsituacao = 'R' THEN ' Revisado ' 
					         WHEN pih.pihsituacao = 'C' THEN ' Cadastrado no SIAFI ' 
					         WHEN pih.pihsituacao = 'E' THEN ' Enviado para Revisão ' 
					   END as situacao,
						pih.pihobs,
						u.usunome 
					FROM monitora.pi_planointernohistorico pih
						INNER JOIN seguranca.usuario u on pih.usucpf = u.usucpf
					WHERE pliid = $obPi_PlanoInterno->pliid 
						ORDER BY pih.pihdata DESC";
			
			
			$sql2 = "SELECT plisituacao FROM monitora.pi_planointerno WHERE pliid = $obPi_PlanoInterno->pliid ";
			$ultimaSituacao = $db->pegaUm($sql2);

			$cabecalho = array("Data Hora", "Situação", "Observação", "Cadastrado Por");
			$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%', 'N' );
		?>
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td align="left" colspan="2">
		<?php 
			$sql = "SELECT ptres FROM monitora.pi_planointernoptres pip
							INNER JOIN monitora.ptres p on pip.ptrid = p.ptrid
						WHERE pip.pliid = $obPi_PlanoInterno->pliid";
			
//			$sql = "SELECT ptres FROM monitora.pi_subacaodotacao sd
//						INNER JOIN monitora.ptres p on sd.ptrid = p.ptrid
//					WHERE sd.sbaid = 24";
			
			$arPtres = $db->carregar($sql);
			$arPtres = ($arPtres) ? $arPtres : array();			
			
			$nPtres = array();
			$boPtres = false;
			foreach($arPtres as $ptres){
				array_push($nPtres,$ptres['ptres']);
				$boPtres = true;
			}?>
			<input type="button" value="Fechar" onclick="window.close()" style="cursor: pointer;"/>
		</td>
	</tr>
	</form>
</table>
<script>


//coloca tabindex no campo valor
function tabindexcampo(){
	var x = document.getElementsByTagName("input");
	var y = 1;
	for(i=0;i<x.length;i++) {
		if(x[i].type=="text"){
			if(x[i].name.substr(0,8) == 'plivalor'){
				x[i].tabIndex=y;
				y++;
			}
		}
	}
}


</script>
<div id="dibDebug"></div>