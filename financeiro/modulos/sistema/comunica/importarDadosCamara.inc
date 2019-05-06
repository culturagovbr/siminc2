<?php

/*
	Sistema Simec
	Setor responsável: SPO-MEC
	Desenvolvedor: Equipe Consultores Simec
	Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
	Programador: Renan de Lima Barbosa (renandelima@gmail.com)
	Módulo: importarDadosCamara.inc
	Finalidade: Importar dados oriundos da Câmara
*/


// inclui classes utilizadas
include APPRAIZ . "includes/CSVFileReader.php";
include APPRAIZ . "includes/PogProgressBar.php";

// inicia barra de progressão
$bar = new PogProgressBar( 'bar' );

set_time_limit( 0 );

?>
<html>
	<head>
		<title>Simec - Ministério da Educação</title>
		<script type="text/javascript">
			self.focus();
		</script>
	</head>
	<body>
		<table align="center" cellpadding="0" cellspacing="20" border="0">
			<tr>
				<td align="center" style="font-family: verdana; font-size: 10pt;">
					Importação Câmara
				</td>
			</tr>
			<tr>
				<td>
					<? $bar->draw(); ?>
				</td>
			</tr>
		</table>
<?php

/**
 * Formata um número para ser utilizado como valor em uma query.
 * 
 * @param string $num
 * @return string
 */
function toNumeric( $num )
{
	if ( !$num )
	{
		return '0';
	}
	return str_replace( ',', '.', $num );
}

$ano = isset( $_REQUEST['ano'] ) ? stripslashes( $_REQUEST['ano'] ) : $db->pega_ano_atual();

// inicia leitor do arquivo csv
$csv = new CSVFileReader();
//$csvFile = APPRAIZ . 'www/temp/' . basename( $_REQUEST['nome'] );
$csvFile = '/tmp/' . basename( $_REQUEST['nome'] );
$csv->setFile( $csvFile );

$erro = '';

if ( $_REQUEST['tipo'] == 'pl' )
{
	$nomeTabelaDestino = ' financeiro.reporcfin ';
	// REMOVE DADOS REDUNDANTES
	//    exemplo: se estiver importando execução de 2005, os que estão
	//    na base desse tipo e desse ano serão removidos para serem
	//    substituídos pelos novos
	$sqlRemocao =
		' delete from ' . $nomeTabelaDestino  .
		' where ' .
			' rofano = \'' . $ano . '\' and ' .
			' coalesce( rofpl, 0 ) != 0 ';
	$db->executar( $sqlRemocao );
	// MONTA QUERY BASE PARA INSERÇÃO DE DADOS
	$sqlBase = <<<EOF
		insert into $nomeTabelaDestino
		(
			orgcod, rofano, loccod,
			prgcod, acacod, sfucod,
			funcod, unicod, esfcod,
			gndcod, foncod, edpcod,
			mapcod, ctecod, iducod,
			rofrp,
			
			rofpl,
			rofpl_cred,
			rofpl_canc,
			acadsc,
			prgdsc
		)
		VALUES
		(
			'%s', '%s', '%s',
			'%s', '%s', '%s',
			'%s', '%s', '%s',
			'%s', '%s', '%s',
			'%s', '%s', '%s',
			'%s',			

			%d,
			%d,
			%d,

			'%s',			
			'%s'

		)
EOF;
	// laço de leitura dos registros a serem importados
	$progress = 0;
	$total = 0;
	while ( $row = $csv->getRow() )
	{
		// verifica a quantidade de campos
		if ( count( $row ) != 21 )
		{
			continue;
		}
		// carrega dados para variáveis
		// códigos

		$orgcod = $row[0];		$rofano = $row[1];		$loccod = $row[2];
		$prgcod = $row[3];		$acacod = $row[4];		$sfucod = $row[5];
		$funcod = $row[6];		$unicod = $row[7];		$esfcod = $row[8];
		$gndcod = $row[9];		$foncod = $row[10];		$edpcod = $row[11];
		$mapcod = $row[12];		$ctecod = $row[13];		$iducod = $row[14];
		$rofrp = $row[15];
		
		$rofpl      = toNumeric( $row[16] );
		$rofpl_cred = toNumeric( $row[17] );
		$rofpl_canc = toNumeric( $row[18] );
		
		$acadsc = $row[19];
		$prgdsc = $row[20];
		
		// monta query para inserção da linha na base de dados
		$sqlInsert = sprintf( $sqlBase,
			// códigos
			$orgcod,	$rofano,	$loccod,
			$prgcod,	$acacod,	$sfucod,
			$funcod,	$unicod,	$esfcod,
			$gndcod,	$foncod,	$edpcod,
			$mapcod,	$ctecod,	$iducod,
			$rofrp,
			
			$rofpl,
			$rofpl_cred,
			$rofpl_canc,
			
			$acadsc,
			$prgdsc
		);
		
		// realiza inserção de dados na base
		//    utiliza ob para capturar possíveis erros de query
		//    esses possíveis erros são exibidos ao final da página
		
		ob_start();
		$db->executar( $sqlInsert );
		$erro = ob_get_clean();

		if ( $erro != '' )
		{
			dbg( $sqlInsert );
			break;
		}
		
		// atualiza barra de progresso
		if ( $csv->getProgress() != $progress )
		{
			$progress = $csv->getProgress();
			$bar->setProgress( $progress );
		}
		
		$total++;
	}
}
else
{
	$nomeTabelaDestino = ' financeiro.reporcfin ';
	// REMOVE DADOS REDUNDANTES
	//    exemplo: se estiver importando execução de 2005, os que estão
	//    na base desse tipo e desse ano serão removidos para serem
	//    substituídos pelos novos
	$sqlRemocao =
		' delete from ' . $nomeTabelaDestino  .
		' where ' .
			' rofano = \'' . $ano . '\' and ' .
			' coalesce( rofpl, 0 ) = 0 ';
	$db->executar( $sqlRemocao );
	// MONTA QUERY BASE PARA INSERÇÃO DE DADOS
	$sqlBase = <<<EOF
		insert into $nomeTabelaDestino
		(
			orgcod, rofano, loccod,
			acacod, prgcod, sfucod,
			funcod, unicod, esfcod,
			gndcod, foncod, edpcod,
			mapcod, iducod, ctecod,
			
			rofdot_ini,							rofautorizado,
			rofcred_adi,						rofcred_sup,
			rofcred_esp,						rofcred_ext,
			rofsoma_dotacao_cancel_remanejada,	rofempenhado,
			rofliquidado_favorecido,			rofpago,
			
			rofrp,						rofrp_nao_proc_inscrito,
			rofrp_nao_proc_pago,		rofrp_nao_proc_cancelado,
			rofrp_nao_proc_a_pagar,		rofrp_nao_proc_transferido,
			rofrp_nao_proc_recebido,	rofrp_proc_inscrito,
			rofrp_proc_a_pagar,			rofrp_nao_proc_insc_outros_exer,
			rofrp_proc_pago,			rofrp_proc_cancelado,
			
			prgdsc,	acadsc
		)
		VALUES
		(
			'%s', '%s', '%s',
			'%s', '%s', '%s',
			'%s', '%s', %s,
			%s,   '%s', '%s',
			'%s', '%s', %s,
			
			'%s', '%s',
			'%s', '%s',
			'%s', '%s',
			'%s', '%s',
			'%s', '%s',
			
			'%s', '%s',
			'%s', '%s',
			'%s', '%s',
			'%s', '%s',
			'%s', '%s',
			'%s', '%s',
			
			'%s', '%s'
		)
EOF;
	
	// REALIZA IMPORTAÇÃO
	
	// laço de leitura dos registros a serem importados
	$progress = 0;
	$total = 0;

	while ( $row = $csv->getRow() )
	{

		// verifica a quantidade de campos
		if ( count( $row ) != 39 )
		{
			continue;
		}
		
		// carrega dados para variáveis
		// códigos
		$orgcod = $row[0];		$rofano = $row[1];		$loccod = $row[2];
		$acacod = $row[3];		$prgcod = $row[4];		$sfucod = $row[5];
		$funcod = $row[6];		$unicod = $row[7];		$esfcod = $row[8];
		$gndcod = $row[9];		$foncod = $row[10];		$edpcod = $row[11];
		$mapcod = $row[12];		$iducod = $row[13];		$ctecod = $row[36];
		// valores gerais
		$rofdot_ini                        = toNumeric( $row[14] );		$rofautorizado = toNumeric( $row[15] );
		$rofcred_adi                       = toNumeric( $row[16] );		$rofcred_sup   = toNumeric( $row[17] );
		$rofcred_esp                       = toNumeric( $row[18] );		$rofcred_ext   = toNumeric( $row[19] );
		$rofsoma_dotacao_cancel_remanejada = toNumeric( $row[20] );		$rofempenhado  = toNumeric( $row[21] );
		$rofliquidado_favorecido           = toNumeric( $row[22] );		$rofpago       = toNumeric( $row[23] );	
		// RPs
		$rofrp                   = $row[24];					$rofrp_nao_proc_inscrito         = toNumeric( $row[25] );
		$rofrp_nao_proc_pago     = toNumeric( $row[26] );		$rofrp_nao_proc_cancelado        = toNumeric( $row[27] );
		$rofrp_nao_proc_a_pagar  = toNumeric( $row[28] );		$rofrp_nao_proc_transferido      = toNumeric( $row[29] );
		$rofrp_nao_proc_recebido = toNumeric( $row[30] );		$rofrp_proc_inscrito             = toNumeric( $row[31] );
		$rofrp_proc_a_pagar      = toNumeric( $row[32] );		$rofrp_nao_proc_insc_outros_exer = toNumeric( $row[33] );
		$rofrp_proc_pago         = toNumeric( $row[34] );		$rofrp_proc_cancelado            = toNumeric( $row[35] );
		// descricoes
		$prgdsc = $row[37];		$acadsc = $row[38];
		
		// correções de dados
		$esfcod .= '0';
		if ( strtoupper( $rofrp ) == 'N' )
		{
			$rofrp = '4';
		}
		
		// monta query para inserção da linha na base de dados
		$sqlInsert = sprintf( $sqlBase,
			// códigos
			$orgcod,	$rofano,	$loccod,
			$acacod,	$prgcod,	$sfucod,
			$funcod,	$unicod,	$esfcod,
			$gndcod,	$foncod,	$edpcod,
			$mapcod,	$iducod,	$ctecod,
			// valores gerais
			$rofdot_ini,						$rofautorizado,
			$rofcred_adi,						$rofcred_sup,
			$rofcred_esp,						$rofcred_ext,
			$rofsoma_dotacao_cancel_remanejada,	$rofempenhado,
			$rofliquidado_favorecido,			$rofpago,
			// RPs
			$rofrp,						$rofrp_nao_proc_inscrito,
			$rofrp_nao_proc_pago,		$rofrp_nao_proc_cancelado,
			$rofrp_nao_proc_a_pagar,	$rofrp_nao_proc_transferido,
			$rofrp_nao_proc_recebido,	$rofrp_proc_inscrito,
			$rofrp_proc_a_pagar,		$rofrp_nao_proc_insc_outros_exer,
			$rofrp_proc_pago,			$rofrp_proc_cancelado,
			
			$prgdsc,	$acadsc
		);

		// realiza inserção de dados na base
		//    utiliza ob para capturar possíveis erros de query
		//    esses possíveis erros são exibidos ao final da página
		ob_start();
		$db->executar( $sqlInsert );
		$erro = ob_get_clean();
		if ( $erro != '' )
		{
			dbg( $sqlInsert );
			break;
		}
		
		// atualiza barra de progresso
		if ( $csv->getProgress() != $progress )
		{
			$progress = $csv->getProgress();
			$bar->setProgress( $progress );
		}
		
		$total++;
	}
}
// realiza correções de nome de ações e programas
$erro = trim( $erro );
if ( $erro == '' && $_REQUEST['atu'] == 'true' )
{
	ob_start();
	$sqlCorrigirAcao = <<<EOF
		update financeiro.reporcfin
		set acadsc = (
			select acadsc
			from monitora.acao a
			where
				reporcfin.acacod = a.acacod and
				a.prgano = reporcfin.rofano and
				a.acasnrap = 'f' and
				a.acastatus = 'A'
			group by acadsc
		)
		where rofano = '$ano'
EOF;
	$sqlCorrigirPrograma = <<<EOF
		update financeiro.reporcfin
		set prgdsc = (
			select prgdsc
			from planejamento.programa p
			where
				reporcfin.prgcod = p.prgcod and
				p.prgano = reporcfin.rofano and
				p.prgstatus = 'A'
			group by prgdsc
		)
		where rofano = '$ano'
EOF;
	$db->executar( $sqlCorrigirAcao );
	$db->executar( $sqlCorrigirPrograma );
	$erro = ob_get_clean();
}

?>
		<!-- RELATÓRIO DA IMPORTAÇÃO -->
		<table align="center" cellpadding="0" cellspacing="20" border="0" width="350">
			<?php if ( $erro == '' ) : ?>
				<tr>
					<td align="center" style="font-family: verdana; font-size: 10pt; color: #209020;">
						Foram importados <?php echo $total; ?> registros.
					</td>
				</tr>
				<?php $db->commit(); ?>
			<?php else : ?>
				<tr>
					<td align="center" style="font-family: verdana; font-size: 10pt; color: #902020;">
						Importação não concluída.<br/>
						Ocorreu um erro ao importar os dados.
					</td>
				</tr>
				<tr>
					<td style="font-family: verdana; font-size: 10pt;">
						<?php echo $erro; ?>
						<?php $db->rollback(); ?>
						<?php $bar->setProgress( 0 ); ?>
					</td>
				</tr>
			<?php endif; ?>
		</table>
	</body>
</html>





