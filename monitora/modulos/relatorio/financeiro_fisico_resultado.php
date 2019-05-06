<?php


function pegaArrayRequest( $indice )
{
	if ( !isset( $_REQUEST[$indice] ) || $_REQUEST[$indice][0] == "" )
	{
		return array();
	}
	if ( isset( $_REQUEST[$indice . "_campo_flag"] ) && $_REQUEST[$indice . "_campo_flag"] == 0 )
	{
		return array();
	}
	return $_REQUEST[$indice];
}

// pega dados do formulário
$agrupadorMacro = pegaArrayRequest( "agrupadorMacro" );
$agrupadorMicro = pegaArrayRequest( "agrupadorMicro" );
$acacod = pegaArrayRequest( "acacod" );
$prgcod = pegaArrayRequest( "programa" );
$loccod = pegaArrayRequest( "localizador" ); // não utilizado !
$unicod = pegaArrayRequest( "uo" );
$cumulativo = (string) $_REQUEST['cumulativo'];
$produto = (string) $_REQUEST['produto'];
$escalaFin = (integer) $_REQUEST['escala_fin'];

$where = array();

// realiza filtro de ação
$where_acao = array();
foreach ( $acacod as $cod )
{
	array_push( $where_acao, " fis.acacod = '" . $cod . "' " );
}
if ( count( $where_acao ) )
{
	$operador = isset( $_REQUEST["acacod_campo_excludente"] ) ? " not " : "";
	array_push( $where, $operador . "(" . implode( " or ", $where_acao ) . ")" );
}

// realiza filtro de localizador
$where_loc = array();
foreach ( $loccod as $cod )
{
	array_push( $where_loc, " fis.loccod = '" . $cod . "' " );
}


if ( count( $where_loc ) )
{
	$operador = isset( $_REQUEST["localizador_campo_excludente"] ) ? " not " : "";
	array_push( $where, $operador . "(" . implode( " or ", $where_loc ) . ")" );
}


// realiza filtro de programa
$where_prg = array();
foreach ( $prgcod as $cod )
{
	array_push( $where_prg, " fis.prgcod = '" . $cod . "' " );
}


if ( count( $where_prg ) )
{
	$operador = isset( $_REQUEST["programa_campo_excludente"] ) ? " not " : "";
	array_push( $where, $operador . "(" . implode( " or ", $where_prg ) . ")" );
}


// realiza filtro de unidade
$where_uo = array();
foreach ( $unicod as $cod )
{
	array_push( $where_uo, " fis.unicod = '" . $cod . "' " );
}


if ( count( $where_uo ) )
{
	$operador = isset( $_REQUEST["uo_campo_excludente"] ) ? " not " : "";
	array_push( $where, $operador . "(" . implode( " or ", $where_uo ) . ")" );
}


// realiza filtro de cumulativo
if ( $cumulativo != "" )
{
	array_push( $where, " fis.cumulativo = '" . $cumulativo . "' " );
}


// realiza filtro de produto
switch ( $produto )
{
	case "sim":
		array_push( $where, " pro.prodsc != '-' " );
		break;
	case "nao":
		array_push( $where, " pro.prodsc = '-' " );
		break;
}

// serializa filtros para a requisição
if ( count( $where ) )
{
	$where = implode( " and \n ", $where ) . " \n and ";
}
else
{
	$where = "";
}

if ( $_REQUEST['xls'] == '1' )
{
global $escalaFin;
$sqlBusca = "

SELECT acacod,acadsc,prgcod,prodsc,unmdsc,fis_previsto,fis_realizado,
	porc_fisico,
	fin_previsto,
	fin_realizado,
	porc_finaceiro,
	total_previsto,
	CASE prodsc
		WHEN '-' then '-'
		ELSE total_realizado::text
	END

FROM

(
SELECT acacod,acadsc,prgcod,prodsc,unmdsc,fis_previsto,fis_realizado,
	CASE fis_previsto
		WHEN 0 then 0
		ELSE Ceiling(((fis_realizado/fis_previsto)*$escalaFin))
	END as 	porc_fisico,
	fin_previsto::integer ,
	Ceiling(fin_realizado) AS fin_realizado,
	CASE fin_previsto
		WHEN 0 then 0
		ELSE ((fin_realizado/fin_previsto)*$escalaFin)::integer
	END as 	porc_finaceiro,
	CASE fis_previsto
		WHEN 0 then (fin_previsto)::numeric(10,2)
		ELSE ((fin_previsto/fis_previsto)*$escalaFin)::numeric(12,2)
	END as 	total_previsto,
	CASE fis_realizado 
		WHEN 0 then (fin_realizado*$escalaFin)
		ELSE Floor(fin_realizado/fis_realizado)::integer
	END as 	total_realizado


FROM (
select
	fis.acacod,
	trim(fis.acadsc) as acadsc,
	fis.prgcod,
	trim(fis.prgdsc) as prgdsc,
	trim(pro.prodsc) as prodsc,
	trim(unm.unmdsc) as unmdsc,
	sum (Ceiling(fis.previsto)) as fis_previsto,
	sum (Ceiling(fis.realizado)) as fis_realizado,
	sum (fin.finvlratualano)/$escalaFin::integer as fin_previsto,
	sum (fin.finvlrrealizadoano)/$escalaFin::integer as fin_realizado
	from monitora.execucaofisica fis
		inner join monitora.dadofinanceiro fin on fin.acaid = fis.acaid
		inner join public.unidade uni on uni.unicod = fis.unicod
		--inner join public.localizador loc on loc.loccod = fis.loccod
		inner join planejamento.acao aca on aca.acaid = fis.acaid
		inner join public.produto pro on pro.procod = aca.procod
		inner join public.unidademedida unm on unm.unmcod = aca.unmcod
	
	where
		$where
		fis.prgano = '" . ( (integer) $_SESSION["exercicio"] ) . "' and
		uni.unitpocod = 'U' and
		uni.unistatus = 'A'

	GROUP BY
		fis.acacod,
		trim(fis.acadsc),
		fis.prgcod,
		trim(fis.prgdsc),
		trim(pro.prodsc),
		trim(unm.unmdsc)
) AS tabela1
) AS tabela2

";
}
else{
$sqlBusca = "
	select
		-- agrupadores micro
			-- acao
			fis.acacod,
			trim( fis.acadsc ) as acadsc,
			-- localizador
			fis.acacod || '.' || fis.unicod || '.' || fis.loccod as loccod,
			trim( aca.sacdsc ) as locdsc,
		-- agrupadores macros
			-- programa
			fis.prgcod,
			trim( fis.prgdsc ) as prgdsc,
			-- unidade
			fis.unicod,
			trim( uni.unidsc ) as unidsc,
		-- produto e unidade de medida
		pro.prodsc,
		unm.unmdsc,
		-- flag acumulativo
		--fis.cumulativo,
		-- fisico
		fis.previsto as fis_previsto,
		fis.realizado as fis_realizado,
		CASE fis.previsto
			when 0 then 0 else ((fis.realizado/fis.previsto)*100)::integer end as percentFisico,
		-- financeiro
		fin.finvlratualano as fin_previsto,
		fin.finvlrrealizadoano as fin_realizado,
		CASE fin.finvlratualano
			when 0 then 0 else ((fin.finvlrrealizadoano/fin.finvlratualano)*100)::integer end as 	percentFinanceiro,
		CASE fis.previsto
					when 0 then fin.finvlratualano else (fin.finvlratualano/fis.previsto) end as totalFisicoFinanceiro,
		CASE fis.realizado
					when 0 then fin.finvlrrealizadoano else (fin.finvlrrealizadoano/fis.realizado) end as totalFisicoFinanceiro2
	from monitora.execucaofisica fis
		left join monitora.dadofinanceiro fin on fin.acaid = fis.acaid
		left join public.unidade uni on uni.unicod = fis.unicod
		left join planejamento.acao aca on aca.acaid = fis.acaid
		left join public.produto pro on pro.procod = aca.procod
		left join public.unidademedida unm on unm.unmcod = aca.unmcod
	where
		$where
		fis.prgano = '" . ( (integer) $_SESSION["exercicio"] ) . "' and
		uni.unitpocod = 'U' and
		uni.unistatus = 'A'
";
}

if ( $_REQUEST['xls'] == '1' )
{	
	//print $sqlBusca;
	//exit();
	header( 'Content-type: application/xls' );
	header( 'Content-Disposition: attachment; filename="planilha_financeira_fisico.xls"' );
	$db->sql_to_excel( $sqlBusca, 'financeira_fisico', '' );
	exit();
}

function agrupar( $dadosOriginais, $campoCod, $campoDsc, $somar )
{
	$dadosNovos = array();
	$somar = (boolean) $somar;
	foreach ( $dadosOriginais as $dados )
	{
		if ( array_key_exists( "filhos", $dados ) )
		{
			$dados["filhos"] = agrupar(
				$dados["filhos"],
				$campoCod,
				$campoDsc,
				$somar
			);
			$dadosNovos[$dados['cod']] = $dados;
			continue;
		}
		$cod = $dados[$campoCod];
		if ( !array_key_exists( $cod, $dadosNovos ) )
		{
			$dadosNovos[$cod] = array(
				"cod"           => $cod,
				"dsc"           => $dados[$campoDsc],
				"prodsc"        => $dados["prodsc"],
				"unmdsc"        => $dados["unmdsc"],
				"soma"          => $somar,
				"fis_previsto"  => $somar ? 0 : null,
				"fis_realizado" => $somar ? 0 : null,
				"fin_previsto"  => $somar ? 0 : null,
				"fin_realizado" => $somar ? 0 : null,
				"filhos"        => array()
			);
		}
		array_push( $dadosNovos[$cod]['filhos'], $dados );
		if ( $somar )
		{
			$dadosNovos[$cod]['fis_previsto']  += $dados['fis_previsto'];
			$dadosNovos[$cod]['fis_realizado'] += $dados['fis_realizado'];
			$dadosNovos[$cod]['fin_previsto']  += $dados['fin_previsto'];
			$dadosNovos[$cod]['fin_realizado'] += $dados['fin_realizado'];
		}
	}
	return $dadosNovos;
}

// realiza ordenação
$agrupadores = array_merge( $agrupadorMacro, $agrupadorMicro );
$orderby = array();
foreach ( $agrupadores as $campo )
{
	switch ( $campo )
	{
		case "pro":
			array_push( $orderby, "fis.prgcod" );
			break;
		case "uo":
			array_push( $orderby, "uni.unicod" );
			break;
		case "aca":
			array_push( $orderby, "fis.acacod" );
			break;
		case "loc":
			array_push( $orderby, "fis.acacod || fis.unicod || fis.loccod" );
			break;
	}
}
if ( count( $orderby ) )
{
	$sqlBusca .= " order by " . implode( ",", $orderby );
}


// realiza consulta no banco
$dados = $db->carregar( $sqlBusca );
$dados = $dados ? $dados : array();

// realiza agrupamento
foreach ( $agrupadores as $campo )
{
	switch ( $campo )
	{
		case "pro":
			$dados = agrupar( $dados, "prgcod", "prgdsc", false );
			break;
		case "uo":
			$dados = agrupar( $dados, "unicod", "unidsc", false );
			break;
		case "aca":
			$dados = agrupar( $dados, "acacod", "acadsc", true );
			break;
		case "loc":
			$dados = agrupar( $dados, "loccod", "locdsc", true );
			break;
	}
}

function desenha( $dados, $profundidade = 0 )
{
	global $escalaFin;
	static $cor = "";
	foreach ( $dados as $item )
	{
		if ( !isset( $item['filhos'] ) )
		{
			return;
		}
		if ( is_int( $item['fis_previsto'] ) )
		{
			$fis_prev = number_format( $item['fis_previsto'], 0, ",", "." );
			$fis_real = number_format( $item['fis_realizado'], 0, ",", "." );
			$fin_prev = number_format( $item['fin_previsto'] / $escalaFin, 0, ",", "." );
			$fin_real = number_format( $item['fin_realizado'] / $escalaFin, 0, ",", "." );
			$fis_porcento = "0";
			if ( $item['fis_previsto'] != 0 && $item['fis_realizado'] != 0 )
			{
				$fis_porcento = number_format(
					( $item['fis_realizado'] / $item['fis_previsto'] ) * 100,
					0, ",", "."
				);
			}
			$fin_porcento = "0";
			if ( $item['fin_previsto'] != 0 && $item['fin_realizado'] != 0 )
			{
				$fin_porcento = number_format(
					( $item['fin_realizado'] / $item['fin_previsto'] ) * 100,
					0, ",", "."
				);
			}
			$custo_previsto = "0,00";
			$custo_realizado = "0,00";
			if ( $item['fis_previsto'] != 0 && $item['fin_previsto'] != 0 )
			{
				$custo_previsto = number_format(
					( $item['fin_previsto'] / $item['fis_previsto'] ),
					2, ",", "."
				);
			}
			if ( $item['fis_realizado'] != 0 && $item['fin_realizado'] != 0 )
			{
				$custo_realizado = number_format(
					( $item['fin_realizado'] / $item['fis_realizado'] ),
					2, ",", "."
				);
			}
			if ( $item['fis_realizado'] == 0 )
			{
				$custo_realizado =  number_format( $item['fin_realizado'], 2, ",", "." );
				if ( $item['fin_realizado'] != 0 )
				{
					$custo_realizado = "<span style=\"color: #900000;\">" . $custo_realizado . "</span>";
				}
			}
			$pro_dsc = strtolower( $item["prodsc"] );
			$unm_dsc = strtolower( $item["unmdsc"] );
			if ( $pro_dsc == "-" )
			{
				$fis_prev = "-";
				$fis_real = "-";
				$fis_porcento = "-";
				$custo_previsto = "-";
				$custo_realizado = "-";
			}
			else
			{
				$pro_dsc = str_replace( "Ç", "ç", $pro_dsc );
				$pro_dsc = str_replace( "Ã", "ã", $pro_dsc );
				$pro_dsc = str_replace( "Õ", "õ", $pro_dsc );
				$pro_dsc = str_replace( "É", "é", $pro_dsc );
				$pro_dsc = str_replace( "/", "/ ", $pro_dsc );
				$unm_dsc = str_replace( "Ç", "ç", $unm_dsc );
				$unm_dsc = str_replace( "Ã", "ã", $unm_dsc );
				$unm_dsc = str_replace( "Õ", "õ", $unm_dsc );
				$unm_dsc = str_replace( "É", "é", $unm_dsc );
				$unm_dsc = str_replace( "/", "/ ", $unm_dsc );
			}
			$cor     = $cor == "" ? "#f7f7f7" : "";
			echo
				"<tr bgcolor=\"" . $cor . "\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='" . $cor . "';\">" .
					"<td style=\"padding-left: " . ( $profundidade * 5 ) . "px;\">" .
						$item['cod'] . " - " . $item['dsc'] .
					"</td>" .
					"<td style=\"width:80px;\">" . $pro_dsc . "</td>" .
					"<td style=\"width:51px;\">" . $unm_dsc . "</td>" .
					"<td style=\"text-align:right;width:45px\">" . $fis_prev . "</td>" .
					"<td style=\"text-align:right;width:40px\">" . $fis_real . "</td>" .
					"<td style=\"text-align:right;width:33px\">" . $fis_porcento . "</td>" .
					"<td style=\"text-align:right;width:59px\">" . $fin_prev . "</td>" .
					"<td style=\"text-align:right;width:52px\">" . $fin_real . "</td>" .
					"<td style=\"text-align:right;width:25px\">" . $fin_porcento . "</td>" .
					"<td style=\"text-align:right;width:59px\">" . $custo_previsto . "</td>" .
					"<td style=\"text-align:right;width:63px\">" . $custo_realizado . "</td>" .
				"</tr>";
		}
		else
		{
			$cor = "#e0e0e0";
			echo
				"<tr bgcolor=\"" . $cor . "\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='" . $cor . "';\">" .
					"<td style=\"padding-left: " . ( $profundidade * 5 ) . "px;\" colspan=\"11\">" .
						$item['cod'] . " - " . $item['dsc'] .
					"</td>" .
				"</tr>";
		}
		desenha( $item['filhos'], $profundidade + 1 );
	}
}


function cabecalhoBrasao()
{
	global $db;
	global $consulta;
	global $escalaFin;
	?>
	<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">
		<tr bgcolor="#ffffff">
			<td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>
			<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
				<?php echo NOME_SISTEMA; ?><br/>
				Acompanhamento da Execução Orçamentária<br/>
				MEC / SE - Secretaria Executiva <br />
				SPO - Subsecretaria de Planejamento e Orçamento
			</td>
			<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
				Impresso por: <b><?= $_SESSION['usunome'] ?></b><br/>
				Hora da Impressão: <?= date( 'd/m/Y - H:i:s' ) ?><br/>
				Financeiro - EM R$ <?php echo $escalaFin; ?>,00<br/>
			</td>
		</tr>
	</table>
	<?
}

?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
		<title>Acompanhamento da Execução Orçamentária</title>
		<style type="text/css">
			
			@media print {.notprint { display: none }}

			@media screen {
				.notscreen { display: none;  }
				.div_rolagem{ overflow-x: auto; overflow-y: auto; width:19.5cm;height:350px;}
				.topo { position: absolute; top: 0px; margin: 0; padding: 5px; position: fixed; background-color: #ffffff;}
			}

			*{margin:0; padding:0; border:none; font-size:8px;font-family:Arial;}
			.alignRight{text-align:right !important;}
			.alignCenter{ text-align:center !important;}
			.alignLeft{text-align:left !important;}
			.bold{font-weight:bold !important;}
			.italic{font-style:italic !important;}
			.noPadding{padding:0;}
			.titulo{width:52px;}
			.tituloagrup{font-size:9px;}
			.titulolinha{font-size:9px;}
			
			#tabelaTitulos tr td, #tabelaTitulos tr th{border:2px solid black;border-left:none; border-right:none;}
			#orgao{margin:3px 0 0 0;}
			#orgao tr td{border:1px solid black;border-left:none;border-right:none;font-size:11px;}
			
			div.filtro { page-break-after: always; text-align: center; }
			
			table{width:19cm;border-collapse:collapse;}
			th, td{font-weight:normal;padding:4px;vertical-align:top;}
			thead{display:table-header-group;}
			table, tr{page-break-inside:avoid;}
			a{text-decoration:none;color:#3030aa;}
			a:hover{text-decoration:underline;color:#aa3030;}
			span.topo { position: absolute; top: 3px; margin: 0; padding: 5px; position: fixed; background-color: #f0f0f0; border: 1px solid #909090; cursor:pointer; }
			span.topo:hover { background-color: #d0d0d0; }
			
		</style>
	</head>
	<body>

		<div id="aguarde" style="background-color:#ffffff;position:absolute;color:#000033;top:50%;left:30%;border:2px solid #cccccc; width:300px;">
			<center style="font-size:12px;"><br><img src="../imagens/wait.gif" border="0" align="absmiddle"> Aguarde! Gerando Relatório...<br><br></center>
		</div>
		<script type="text/javascript">
			self.focus();
		</script>
		<table>
			<thead>
				<tr>
					<th class="noPadding" align="left">
						<?php cabecalhoBrasao(); ?>
						<table border="0" id="tabelaTitulos" align="left">
							<thead>
								<tr>
									<th rowspan="2" align="center">&nbsp;</th>
									<th rowspan="2" align="center" style="width:80px">Produto</th>
									<th rowspan="2" align="center" style="width:51px">Unidade de Medida</th>
									<th colspan="3" align="center">Físico</th>
									<th colspan="3" align="center">Financeiro</th>
									<th colspan="2" align="center" valign="bottom">Custo Unitário</th>
								</tr>
								<tr>
									<th align="center" style="width:45px">Prev.<br/>(A)</th>
									<th align="center" style="width:40px">Real.<br/>(B)</th>
									<th align="center" style="width:33px">%<br/>(B/A)</th>
									<th align="center" style="width:59px">Prev.<br/>(C)</th>
									<th align="center" style="width:52px">Real.<br/>(D)</th>
									<th align="center" style="width:25px">%<br/>(D/C)</th>
									<th align="center" style="width:59px">Prev.<br/>(C/A)</th>
									<th align="center" style="width:59px">Real.<br/>(D/B)</th>
								</tr>
							</thead>
						</table>
					</th>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td>
						<?php if ( count( $dados ) ) : ?>
							<div class="div_rolagem">
								<table border="0">
									<?php desenha( $dados ); ?>
								</table>
							</div>
						<?php else : ?>
							Nenhum item encontrado
						<?php endif; ?>
					</td>
				</tr>
			</tbody>
		</table>
		<script type="text/javascript" language="javascript">
			document.getElementById( 'aguarde' ).style.visibility = 'hidden';
			document.getElementById('aguarde').style.display = 'none';
		</script>
	</body>
</html>
