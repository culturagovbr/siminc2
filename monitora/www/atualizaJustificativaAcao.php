<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

//$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . 'includes/workflow.php';

echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';

if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "SELECT DISTINCT
			a.prgcod||'.'||a.acacod||'.'||a.unicod||'.'||a.loccod||' - '||a.acatitulo as acao,
			a.unicod||' - '||uni.unidsc as unidade,
		    char_length(avp.avptexto) as tamanhoJustificativa,
		    avp.avptexto as justificativa,
		    case when avp.avptextoold is null then avp.avptexto else avp.avptextoold end as textoold,
		    avp.avpid
		FROM
			planejamento.acao a
			inner join monitora.avaliacaoparecer avp on avp.acaid = a.acaid
			left join monitora.referencia ref on ref.refcod = avp.refcod
			left join monitora.execucaopto exp on exp.acaid = a.acaid and ref.refcod = exp.refcod
			inner join workflow.documento doc on doc.docid = avp.docid
			inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
			left join public.unidade uni on uni.unicod = a.unicod
		WHERE
			ref.refcod = avp.refcod 
		    and ref.refdata_limite_avaliacao_aca IS NOT NULL
		    and ref.refsnmonitoramento = true
                    and ref.refano_ref = '2012'
                    /*and a.acacod = '0048'
                    and a.unicod = '26101'
                    and a.loccod = '0054'*/
                  and a.prgano = '2012' 
                  and a.acasnrap = false
                  and esd.esdid in (620)
                  and (a.prodsc <> '' or a.prodsc is not null)
                order by char_length(avp.avptexto) desc";

$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();
//ver($arrDados);
$totalAnterior = 0;
$totalAtual = 0;

$totalGeralAnterior = 0;
$totalGeralAtual = 0;
$totalGeral = 0;

foreach ($arrDados as $key => $v) {
	
	$key % 2 ? $cor = "#dedfde" : $cor = "";
	
	$avaliacao = strip_tags( htmlspecialchars_decode($v['justificativa'], ENT_QUOTES) );
	$avaliacao = html_entity_decode($avaliacao);
	
	$arrTrans = array("&ndash;" => "-", "&nbsp;" => "");
	$avaliacao = strtr($avaliacao, $arrTrans);
	$avaliacao = str_ireplace("'", "", $avaliacao);
	$avaliacao = ($avaliacao ? "'".$avaliacao."'": 'null');
	
	$avaliacao = preg_replace('/^\s*$/m', '', $avaliacao);
	      	
	$sql = "UPDATE monitora.avaliacaoparecer SET
				avptexto = ".trim($avaliacao).",
				avptextoold = '".simec_htmlspecialchars($v['textoold'], ENT_QUOTES)."'
			WHERE avpid = {$v['avpid']}";
	
	$db->executar( $sql );
	
	$totalAtual = $db->pegaUm("select char_length(avptexto) from monitora.avaliacaoparecer where avpid = {$v['avpid']}");
	$totalAnterior = $db->pegaUm("select char_length(avptextoold) from monitora.avaliacaoparecer where avpid = {$v['avpid']}");
	
	$html .= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
					<td>'.$v['acao'].'</td>
					<td>'.$v['unidade'].'</td>
					<td style="text-align: center; color: rgb(0, 102, 204);">'.$totalAnterior.'</td>
					<td style="text-align: center; color: rgb(0, 102, 204);">'.$totalAtual.'</td>
				</tr>';
	$totalGeralAnterior += $totalAnterior;
	$totalGeralAtual 	+= $totalAtual;
	$totalGeral++;
	$totalAnterior = 0;
	$totalAtual = 0;
}

$db->commit();
//echo $html;
//echo 'Dados Atualizado com sucesso.';
?>
<table align="center" class="listagem" style="width: 100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
	<tr>
		<th colspan="4">DADOS ATUALIZADO COM SUCESSO</th>
	</tr>
	<tr>
		<td align="Center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
		<td align="Center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Unidade</strong></td>
		<td align="Center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Tamanho Anterior</strong></td>
		<td align="Center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Tamanho Atualizado</strong></td>
	</tr>
	<?=$html; ?>
	<tr>
		<td colspan="2" style="text-align: right;"><b>Total Geral:</b></td>
		<td style="text-align: center; color: rgb(0, 102, 204);"><?=$totalGeralAnterior; ?></td>
		<td style="text-align: center; color: rgb(0, 102, 204);"><?=$totalGeralAtual; ?></td>
	</tr>
</table>
<table class="listagem" cellspacing="0" cellpadding="2" border="0" align="center" width="100%">
	<tbody>
	<tr bgcolor="#ffffff">
		<td><b>Total de Registros: <?=$totalGeral; ?></b></td>
	</tr>
	</tbody>
</table>