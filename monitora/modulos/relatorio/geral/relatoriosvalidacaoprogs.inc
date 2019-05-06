<?php


include APPRAIZ . "includes/cabecalho.inc";

if ( isset( $_REQUEST['prgid'] ) && $_REQUEST['prgid'])
{
	$prgids = array( $_REQUEST['prgid'] );
	$prgid = $_REQUEST['prgid'];
	$unicod = $_REQUEST['unicod'];
}
else
{
	$prgids = array();
}

//$prgids = array();


// carrega referencias
$sql = "
	select
		refcod,
		refdsc
	from monitora.referencia
	where
		refdata_limite_parecer_aca is not null and
		refsngrupo = 't' and
		refano_ref = '" . $_SESSION['exercicio'] . "' and
		refsnmonitoramento = 't'
	order by refmes_ref
";
$meses = $db->carregar( $sql );
$referencias = array();
foreach ( $meses as $mes )
{
	array_push( $referencias, $mes['refcod'] );
}

// monta titulo da página
echo '<br>';

$db->cria_aba( $abacod_tela,$url,'');
$subtitulo_modulo = '<b>Legenda:</b> 	<img src="../imagens/valida0.gif" border="0" width="15" height="15"/> Não Validado | 
						<img src="../imagens/valida1.gif" border="0" width="15" height="15"/> Validado com Concordância | 
						<img src="../imagens/valida3.gif" border="0" width="15" height="15"/> Validado com Discordância | 
						<img src="../imagens/valida2.gif" border="0" width="15" height="15"/> Pendente <br>Clique no Ícone correspondente à Ação e Trimestre Desejado para Validar as Ações abaixo.';
monta_titulo( $titulo_modulo, $subtitulo_modulo );



?>


<script type="text/javascript">

	function  validar_relatorio(){
		formulario = document.filtro;
		pgrid = formulario.prgid;			
		
		if(pgrid.value == ""){
			
			alert("Escolha um Programa.");
			return;
			
		}else{
			formulario.varaux.value='1';
			formulario.submit();
		}
	}
	
	function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }
    
    function AtualizaUnidade()
    {
    	document.filtro.submit();
    }
    

</script>

<form action="" method="post" name="filtro">
<input type="hidden" name="varaux" value="">

<table class="tabela" cellspacing="1" cellpadding="1" bgcolor="#f5f5f5" border="0" align="center" style="border-top: medium none; border-bottom: medium none;">

<tr>
	<td class="SubTituloDireita" width='40%'>	
	Programa:
	</td>
	
	<td>
	
	<?
	//carrega dados para o combobox de programas 
	$prgs = "
			SELECT
				planejamento.programa.prgid AS codigo,
				planejamento.programa.prgcod || ' - ' || planejamento.programa.prgdsc AS descricao 
			FROM
				planejamento.programa
			WHERE
				planejamento.programa.prgstatus =  'A' AND
				planejamento.programa.prgano =  '" . $_SESSION['exercicio']. "'"
	;	
	$prgid = $_REQUEST["prgid"];
	$db->monta_combo("prgid", $prgs, "S", "Todos os Programas", "AtualizaUnidade", null); 
	?>	
	</td>
</tr>
<?if($prgid != ""){?>
<tr>
	<td class="SubTituloDireita">	
	Unidade Orçamentária (UO):
	</td>
	
	<td>
	
	<?
	$prgs = "
			SELECT distinct
				monitora.acao.unicod AS codigo,
				monitora.acao.unicod || ' - ' || public.unidade.unidsc AS descricao 
			FROM
				monitora.acao,
				public.unidade
			WHERE
				public.unidade.unistatus =  'A' AND
				public.unidade.unicod = monitora.acao.unicod AND
				monitora.acao.prgid =  '" . $prgid . "'"
	;
	$unicod = $_REQUEST["unicod"];
	$db->monta_combo("unicod", $prgs, "S", "Todas as Unidades", null, null); 
	?>	
	</td>
</tr>
<?}?>

<tr>
	<td class="SubTituloDireita">	
	
	</td>
	
	<td>	
	<input type="button" onclick="validar_relatorio();" value="Gerar Relatório" name="filtrar" class="botao"/>

	</td>
</tr>

</table>

</form>

<?php if( $_REQUEST['varaux'] ): ?>
	<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
		<tbody>
			<?php foreach( $prgids as $prgid ): ?>
				<?php
					
					$var='';
					if($unicod) $var = " a.unicod = '%s' and ";
						
					$sql = sprintf(
						"select
							a.prgid,
							a.prgcod,
							p.prgdsc,
							a.acaid,
							a.prgcod || '.' || a.acacod || '.' || a.unicod || '.' || a.loccod as codigo,
					 		a.acadsc,
							a.sacdsc,
							a.unicod,
							case
								when cor.corcod is null then 0
								else cor.corcod end as parecer,
							case
								when cor.corsignificadopar is null then 'Não Validado'
								else cor.corsignificadopar end as situacao,
							ref.refcod,
							ref.refmes_ref as mes,
							ref.refano_ref as ano,
							usu.usunome,
							pa.usucpf,
							pa.avpdata
						from monitora.acao a
							left join monitora.referencia ref on
								ref.refdata_limite_parecer_aca is not null and
								ref.refsngrupo = true and
								ref.refstatus = 'A' and
								ref.refcod in ( %s ) 
							left join monitora.avaliacaoparecer pa on
								a.acaid = pa.acaid and
								pa.tpaid = 2 and
								pa.refcod = ref.refcod and
								pa.avpstatus = 'A'
							left join seguranca.usuario usu on
								 pa.usucpf = usu.usucpf
							left join cor on
								pa.avpsituacao = cor.corcod
							inner join planejamento.programa p on
								p.prgid = a.prgid
						where
							a.prgano = '%s' and
							a.acastatus = 'A' and
							acasnrap = 'f' and
							a.prgid = %d and
							$var
							ref.refdata_limite_parecer_aca is not null and
							ref.refsnmonitoramento = 't'
						order by
							a.prgcod,
							a.acacod,
							a.unicod,
							a.loccod,
							a.acaid,
							ref.refmes_ref
						",
							implode( ',', $referencias ),
							$_SESSION['exercicio'],
							$prgid,
							$unicod
					);
					

					
					//echo $sql;
					//dump($sql, true);
					
					$acoes = array();
					$registros = $db->carregar( $sql );
					if ( $registros )
					{
						foreach( $registros as $registro )
						{
							if ( !array_key_exists( $registro['acaid'], $acoes ) )
							{
								$acoes[$registro['acaid']] = array(
									'prgid'   => $registro['prgid'],
									'acaid'   => $registro['acaid'],
									'codigo'  => $registro['codigo'],
									'acadsc'  => $registro['acadsc'],
									'sacdsc'  => $registro['sacdsc'],
									'unicod'  => $registro['unicod'],
									'prgcod'  => $registro['prgcod'],
									'prgdsc'  => $registro['prgdsc'],
									'usunome'  => $registro['usunome'],
									'usucpf'  => $registro['usucpf'],
									'avpdata'  => $registro['avpdata'],
									'situacoes' => array()
								);
							}
							$situacao = array(
								'avaliacao' => $registro['avaliacao'] ? $registro['avaliacao'] : 0,
								'parecer'   => $registro['parecer'] ? $registro['parecer'] : 0,
								'situacao'  => $registro['situacao'] ? $registro['situacao'] : 0,
								'refcod'    => $registro['refcod'],
								'mes'       => $registro['mes'],
								'ano'       => $registro['ano']
							);
							if ( $situacao['situacao'] == 1 )
							{
								$situacao['cor_parecer'] = $situacao['cor_avaliacao'];
							}
							$acoes[$registro['acaid']]['situacoes'][] = $situacao;
						}
						$acoes = array_values( $acoes );
					}
				?>
				
				<tr style="background-color: #e7e7e7;">
					<td colspan="10">
						<table  style="width:100%;"  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
								<?=@$db -> cabecalho_programa($prgid);?>
								<?//=@$db -> cabecalho_programa($acoes[0]['prgid']);?>
						</table>
					</td>
				</tr>
				<tr style="background-color: #e7e7e7;">
					<td align="center">Código</td>
					<td align="center">Descrição</td>
					<td align="center">Localizador</td>
					<td align="center">Usuário</td>
					<td align="center">Perfil</td>
					<td align="center">Data da Validação</td>
					<?php $mesdsc = array(); ?>
					<?php foreach( $meses as $mes ): ?>
						<td align="center"><?= $mes['refdsc'] ?></td>
						<?php $mesdsc[$mes['refcod']] = $mes['refdsc']; ?>
					<?php endforeach; ?>
				</tr>
				<?php foreach( $acoes as $indice => $acao ): ?>
					<tr style="background-color: <?= $indice % 2 ? '#ffffff' : '#f5f5f5' ?>">
						<td style="text-align:center; padding-left: 15px;">
							<a href="?modulo=principal/acao/cadacao&acao=C&acaid=<?= $acao['acaid'] ?>&prgid=<?= $acao['prgid'] ?>">
								<?= $acao['codigo'] ?>
							</a>
						</td>
						<td><?= $acao['acadsc'] ?></td>
						<td><?= $acao['sacdsc'] ?></td>
						<td><?= $acao['usunome'] ?></td>
						<td><?
							if($acao['usucpf']){
								$p_sql = "select b.pfldsc from seguranca.perfilusuario a, seguranca.perfil b where a.pflcod=b.pflcod and b.pflstatus='A' and b.sisid='".$_SESSION['sisid']."' and a.usucpf ='".$acao['usucpf']."'";
								$p_registros = $db->carregar( $p_sql );
								if ( $p_registros )
								{	
									$ct = 0;
									foreach( $p_registros as $p_registro )
									{
										if($ct>0) echo ";<br>";
										echo $p_registro['pfldsc'];
										$ct++;
									}
								}
							}								 
							?>
						</td>
						<td>
							<? if($acao['avpdata']){
									echo strftime("%d/%m/%Y",strtotime($acao['avpdata']));
								} 
							?>
						</td>
						<?php foreach( $acao['situacoes'] as $situacao ): ?>
							<td style="text-align:center; width:50px;">
								<a href="monitora.php?modulo=principal/programa/monitoraprg&acao=A&refcod=<?= $situacao['refcod'] ?>&unicod=<?= $acao['unicod']?>&prgid=<?= $acao['prgid'] ?>">
									<img
										src="../imagens/valida<?= $situacao['parecer'] ?>.gif"
										title="<?= $mesdsc[$situacao['refcod']] . " - " . $situacao['situacao'] ?>"
										align="absmiddle"
										border="0"
										width="15"
										height="15"
									/>
								</a>
							</td>
						<?php endforeach; ?>
					</tr>
				<?php endforeach; ?>
			<?php endforeach; ?>
		</tbody>
	</table>
<?php endif; ?>	
<?php if(!$registros && $_REQUEST['varaux']): ?>
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="10" class="listagem">
	<tbody>
		<td style="text-align: center; background-color: #ffffff">
			<p>Nenhum registro encontrado.</p>
		</td>
	</tbody>
</table>
<?php endif; ?>