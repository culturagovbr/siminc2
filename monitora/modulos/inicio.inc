<?php
/*
Sistema Simec
Setor responsável: SPO-MEC
Desenvolvedor: Equipe Consultores Simec
Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com), Renan de Lima (renandelima@gmail.com), Renê de Lima (renedelima@gmail.com)
Módulo:inicio.inc
Finalidade: permitir abrir a página inicial so simec
*/

include "planotrabalhoUG/_constantes.php";

function possui_perfil( $pflcods ){

	global $db;

	if ( is_array( $pflcods ) )
	{
		$pflcods = array_map( "intval", $pflcods );
		$pflcods = array_unique( $pflcods );
	}
	else
	{
		$pflcods = array( (integer) $pflcods );
	}
	if ( count( $pflcods ) == 0 )
	{
		return false;
	}
	$sql = "
		select
			count(*)
		from seguranca.perfilusuario
		where
			usucpf = '" . $_SESSION['usucpf'] . "' and
			pflcod in ( " . implode( ",", $pflcods ) . " ) ";
	return $db->pegaUm( $sql ) > 0;
}

$arPerfis = array();
$arPerfis[] = PERFIL_UNIDMONITORAAVALIA;
$arPerfis[] = PERFIL_GESTORUNIDPLANEJAM;
$arPerfis[] = PERFIL_EQUIPAPOIOGESTORUP;
$arPerfis[] = PERFIL_MONITORA_GESTORUNIDORCAMENTO;
$arPerfis[] = PERFIL_MONITORA_EQAGESTORUNIDORCAMENTO;
$arPerfis[] = PERFIL_MONITORA_GESTORUNIOBRIGATORIA;
$arPerfis[] = PERFIL_MONITORA_ESQUIPEGESTORUNIOBRIGATORIA;
$arPerfis[] = PERFIL_MONITORA_GESTORSUBACOES;

$mostraShowModal = false;
if($db->testa_superuser() || possui_perfil($arPerfis)){
	$mostraShowModal = true;
}

if($_GET['boImprimeManualPDF']){
	if($_GET['tipoUnidade'] == 'obrigatorias'){
		$filename= "Manual_Basico-Unidades_Obrigatorias.pdf";
	} else {
		$filename= "Manual_Basico-Demais_Unidades.pdf";		
	}
	
    $caminho = APPRAIZ."www/$filename";
    header("Content-type:application/pdf");
	header("Content-Disposition:attachment;filename=$filename");
	readfile($caminho);
	exit;
    
}

if(!isset($_REQUEST['filtro'])){
    // valores default de formulário alterados (2007-07-02, renan)
    //$_REQUEST['responsabilidade']     = 0;
    //$_REQUEST['responsabilidade_prg'] = 0;
    $_REQUEST['listaprg']             = 'S';
    $_REQUEST['ordemgrupo']           = 'A';
}

// Verificações ///////////////////////////////////////////////////////////////

// caso o usuário possua ações sob sua responsabilidade ele é redirecionado
$cpf = $_SESSION['usucpf'];
if ( !isset( $_SESSION[$cpf.'primeira_vez_entrar_monitora'] ) )
{
	$_SESSION[$cpf.'primeira_vez_entrar_monitora'] = 1;
	$sql = <<<EOT
		select
			acaid
		from usuarioresponsabilidade
		where
			usucpf = '{$_SESSION['usucpf']}' and prsano = '{$_SESSION['exercicio']}' and 
			rpustatus = 'A' and
			acaid is not null
EOT;
	$ids = $db->carregar( $sql );
	$ids = $ids ? $ids : array();
	if ( count( $ids ) )
	{
		$acaids = array();
		foreach ( $ids as $id )
		{
			array_push( $acaids, $id['acaid'] );
		}
		$acaids = implode( ',', $acaids );
		$sql = <<<EOT
			select
				count(*)
			from acao a
				left join unidade b on a.unicod = b.unicod
			where
				a.acaid in ( {$acaids} )
EOT;
		if ( $db->pegaUm( $sql ) )
		{
			$_REQUEST['responsabilidade'] = 1;
			$_REQUEST['listaprg'] = 'N';
			$_REQUEST['ordemgrupo'] = 'U';
		}
	}
}




//força ações sob minha responsaqbilidade ($_REQUEST['responsabilidade'])
if($_REQUEST['responsabilidade']){
	$_REQUEST['listaprg'] = 'N';
	$_REQUEST['ordemgrupo'] = 'A';
}

//força programas sob minha responsaqbilidade ($_REQUEST['responsabilidade'])
if($_REQUEST['responsabilidade_prg']){
	$_REQUEST['listaprg'] = 'S';
	$_REQUEST['ordemgrupo'] = 'A';
}



// Configurações ///////////////////////////////////////////////////////////////

$cor = array(
	'',
	'Verde',
	'Amarelo',
	'Vermelho'
);

$estado = array(
	'NI',
	'Estável',
	'Merece Atenção',
	'Crítico'
);

$situacoes = array(
	'Não Validado',
	'Validado com Concordância',
	'Pendente',
	'Validado com Discordância'
);

$situacoes_letra = array(
	'',
	'c',
	'p',
	'd'
);


// Funções /////////////////////////////////////////////////////////////////////
$sql_aux_a = "";

function get_sql( $entidade, $referencias, $programa, $localizador, $unidade, $acao, $responsabilidade, $responsabilidade_prg ) {
	$referencias = implode( ',', $referencias );
	switch ( $entidade ) {
		default:
		case 'A':
			$condicao_resp = '';
			if ( $_REQUEST['acacod'] || $_REQUEST['unidade'] || $_REQUEST['localizador'] ) {
				if ( $responsabilidade ) {
					$condicao_resp = "
						and a.acaid in (
							select
								acaid
							from monitora.usuarioresponsabilidade
							where
								usucpf = '{$_SESSION['usucpf']}' and
								prsano = '{$_SESSION['exercicio']}' and
								rpustatus = 'A'
						)
					";
				}
				if ( $responsabilidade_prg ) {
					$condicao_resp_prg = "
						and a.prgid in (
							select
								prgid
							from monitora.usuarioresponsabilidade
							where
								usucpf = '{$_SESSION['usucpf']}' and
								prsano = '{$_SESSION['exercicio']}' and
								rpustatus = 'A'
						)
					";
				}
				$tabela = 'monitora.avaliacaostat_aca';
				$sql = sprintf(
					"select 
							a.prgid,
							a.acaid as identificador,
							a.prgcod || '.' || a.acacod || '.' || a.unicod || '.' || a.loccod || ' - ' || a.acadsc as descricao,
							coalesce( uni.uniabrev, uni.unidsc ) as unidade,
							a.sacdsc as localizador,
							av.corcod as cor_avaliacao,
							pa.corcod as cor_parecer,
							pa.avpsituacao as situacao,
							ref.refmes_ref as mes_referencia,
							ref.refano_ref as ano_referencia,
							ref.refcod as referencia
						from acao a
						left join unidade uni on a.unicod = uni.unicod 
						left join referencia ref on
							ref.refdata_limite_avaliacao_aca is not null and
							ref.refstatus = 'A' and
							ref.refcod in ( %s ) 
						left join monitora.gruporeferencia gref on
							gref.refcod = ref.refcod
						left join avaliacaoparecer av on
							a.acaid = av.acaid and
							av.tpaid = 1 and
							av.refcod = ref.refcod and
							av.avpstatus = 'A' and
							av.avpliberada = 't'
						left join avaliacaoparecer pa on
							a.acaid = pa.acaid and
							pa.tpaid = 2 and
							pa.refcod = gref.grefcod and
							pa.avpstatus = 'A' and
							pa.avpliberada = 't'
						left join cor cav on
							av.corcod = cav.corcod
						left join cor cpa on
							pa.corcod = cpa.corcod
						where
							a.prgano = '%s' and
							a.acastatus = 'A' and
							acasnrap = 'f'
							%s %s %s %s %s %s %s
						order by
							a.prgcod,
							a.acacod,
							a.unicod,
							a.loccod,
							a.acaid,
							ref.refmes_ref",
					$referencias,
					$_SESSION['exercicio'],
					$programa ? " and a.prgid = $programa " : '',
					$localizador ? " and a.regcod = '$localizador' " : '',
					$unidade ? " and a.unicod = '$unidade' " : '',
					$acao ? " and a.acacod = '$acao' " : '',
					$condicao_resp,
					$condicao_resp_prg,
					$_SESSION['sql_condicao_tipo']
				);
				return $sql;
			} else {
				if ( $responsabilidade ) {
					$condicao_resp = "
						and a.acacod in (
							select
								a.acacod
							from monitora.usuarioresponsabilidade u
								inner join monitora.acao a on
									a.acaid = u.acaid
							where
								u.usucpf = '{$_SESSION['usucpf']}' and
								u.prsano = '{$_SESSION['exercicio']}' and
								u.rpustatus = 'A'
							group by
								a.acacod
						)
					";
				}
				if ( $responsabilidade_prg ) {
					$condicao_resp_prg = "
						and a.acacod in (
							select
								a0.acacod
							from monitora.usuarioresponsabilidade ur
								inner join monitora.acao a0 on a0.prgid = ur.prgid 
							where
								ur.usucpf = '{$_SESSION['usucpf']}' and
								ur.prsano = '{$_SESSION['exercicio']}' and
								ur.rpustatus = 'A'
							group by
								a0.acacod
						)
					";
				}
				$tabela = 'monitora.avaliacaostat_aca';
				$sql = "
					select
						a.acadsc as descricao,
						a.acacod as identificador,
						a.acacod as codigo,
						v.refcod,
						v.acatotal as total,
						v.faltap as cor0,
						v.total_cor1 as cor1,
						v.total_cor2 as cor2,
						v.total_cor3 as cor3
					from %s v
						inner join monitora.acoesunicas a on
							v.acacod = a.acacod and
							a.prgano = '" . $_SESSION['exercicio'] . "'
						inner join monitora.referencia r on
							v.refcod = r.refcod and
							r.refano_ref = a.prgano
					where
						refsngrupo = 'f' and
						v.refcod in ( %s )
						" . $condicao_resp . "
						" . $condicao_resp_prg . "
						%s
						%s
					order by
						a.acacod,
						a.acadsc,
						r.refmes_ref";
				
			}
			break;
		case 'L':
			if ( $responsabilidade ) {
				$condicao_resp = "
					and rg.regcod in (
						select
							a.regcod
						from monitora.usuarioresponsabilidade u
							inner join monitora.acao a on
								a.acaid = u.acaid
						where
							u.usucpf = '{$_SESSION['usucpf']}' and
							u.prsano = '{$_SESSION['exercicio']}' and
							u.rpustatus = 'A'
						group by
							a.regcod
					)
				";
			}
			if ( $responsabilidade_prg ) {
				$condicao_resp_prg = "
					and rg.regcod in (
						select
							a.regcod
						from monitora.usuarioresponsabilidade u
							inner join monitora.acao a on
								a.prgid = u.prgid
						where
							u.usucpf = '{$_SESSION['usucpf']}' and
							u.prsano = '{$_SESSION['exercicio']}' and
							u.rpustatus = 'A'
						group by
							a.regcod
					)
				";
			}
			$tabela = 'monitora.avaliacaostat_reg';
			$sql =
				"select
					rg.regdsc as descricao,
					rg.regcod as codigo,
					rg.regcod as identificador,
					v.refcod,
					v.acatotal as total,
					v.faltap as cor0,
					v.total_cor1 as cor1,
					v.total_cor2 as cor2,
					v.total_cor3 as cor3
				from %s v
					inner join public.regiao rg on v.regcod = rg.regcod
					inner join monitora.referencia r on v.refcod = r.refcod
				where
					refsngrupo = 'f' and
					v.refcod in ( %s )
					%s
					%s
					$condicao_resp
					$condicao_resp_prg
				order by
					rg.regcod,
					rg.regdsc,
					r.refmes_ref";
			break;
		case 'P':
			if ( $responsabilidade ) {
				$condicao_resp = "
					and p.prgid in (
						select
							a.prgid
						from monitora.usuarioresponsabilidade u
							inner join monitora.acao a on
								a.acaid = u.acaid
						where
							u.usucpf = '{$_SESSION['usucpf']}' and
							u.prsano = '{$_SESSION['exercicio']}' and
							u.rpustatus = 'A'
						group by
							a.prgid
					)
				";
			}
			if ( $responsabilidade_prg ) {
				$condicao_resp_prg = "
					and p.prgid in (
						select
							prgid
						from monitora.usuarioresponsabilidade
						where
							usucpf = '{$_SESSION['usucpf']}' and
							prsano = '{$_SESSION['exercicio']}' and
							rpustatus = 'A'
					)
				";
			}
			$tabela = 'monitora.avaliacaostat';
			$sql =
				"select
					p.prgcod as codigo,
					p.prgdsc as descricao,
					v.prgid as identificador,
					v.acatotal as total,
					v.faltap as cor0,
					v.total_cor1 as cor1,
					v.total_cor2 as cor2,
					v.total_cor3 as cor3, 
					count(distinct(a.acacod)) as total_acoes
				from %s v
					inner join planejamento.programa p on v.prgid = p.prgid
					inner join monitora.referencia r on v.refcod = r.refcod
					inner join monitora.acao a ON a.prgano = p.prgano and a.prgcod = p.prgcod and a.prgid = p.prgid
				where
					a.acasnrap = false and
					p.prgano = '". $_SESSION['exercicio'] ."' and
					p.prgstatus = 'A' and
					refsngrupo = 'f' and
					v.refcod in ( %s )
					%s
					%s
					$condicao_resp
					$condicao_resp_prg
				
				group by p.prgcod, p.prgdsc, v.prgid,v.acatotal, v.faltap, v.total_cor1, v.total_cor2, v.total_cor3, r.refmes_ref
									
				order by
					p.prgcod,
					r.refmes_ref";
			break;
		case 'U':
			if ( $responsabilidade ) {
				$condicao_resp = "
					and v.unicod in (
						select
							a.unicod
						from monitora.usuarioresponsabilidade u
							inner join monitora.acao a on
								a.acaid = u.acaid
						where
							u.usucpf = '{$_SESSION['usucpf']}' and
							u.prsano = '{$_SESSION['exercicio']}' and
							u.rpustatus = 'A'
						group by
							a.unicod
					)
				";
			}
			if ( $responsabilidade_prg ) {
				$condicao_resp_prg = "
					and v.unicod in (
						select
							a.unicod
						from monitora.usuarioresponsabilidade u
							inner join monitora.acao a on
								a.prgid = u.prgid
						where
							u.usucpf = '{$_SESSION['usucpf']}' and
							u.prsano = '{$_SESSION['exercicio']}' and
							u.rpustatus = 'A'
						group by
							a.unicod
					)
				";
			}
			$tabela = 'monitora.avaliacaostat_uni';
			$sql = "
				select
					u.unidsc as descricao,
					u.unicod as codigo,
					u.unicod as identificador,
					v.refcod,
					v.acatotal as total,
					v.faltap as cor0,
					v.total_cor1 as cor1,
					v.total_cor2 as cor2,
					v.total_cor3 as cor3
				from %s v
					inner join public.unidade u on v.unicod = u.unicod and u.unitpocod = 'U' and u.unistatus = 'A'
					inner join monitora.referencia r on v.refcod = r.refcod
				where
					refsngrupo = 'f' and
					v.refcod in ( %s )
					%s
					%s
					" . $condicao_resp . "
					" . $condicao_resp_prg . "
				order by
					u.unicod,
					u.unidsc,
					r.refmes_ref";
			break;
	}
	$condicao = '';
	if ( $programa ) {
		$tabela .= '_p';
		$condicao = ' and v.prgid = ' . $programa . ' ';
	}
//	dbg( sprintf( $sql, $tabela, $referencias, $condicao, $_SESSION['sql_condicao_tipo'] ), 1 );
	return sprintf( $sql, $tabela, $referencias, $condicao, $_SESSION['sql_condicao_tipo'] );
}

// Filtros /////////////////////////////////////////////////////////////////////

$filtro_condicao = array();

// Período
setlocale(LC_TIME, 'pt_BR' );
$mes_limite = $_SESSION['exercicio'] == date( 'Y' ) ? date( 'n' ) : 12;
$mes_selecionado = (integer) ( empty( $_REQUEST['mes'] ) ? $mes_limite : $_REQUEST['mes'] );
$responsabilidade     = !!$_REQUEST['responsabilidade'];
$responsabilidade_prg = !!$_REQUEST['responsabilidade_prg'];
$meses = array();
for( $mes = $mes_selecionado; $mes > 0 && count( $meses ) < 6; $mes-- ) {
	array_push( $meses, sprintf( "'%02d'", $mes ) );
}
$colunas = count( $meses );
$meses = implode( ', ', $meses );

// Ações
array_push( $filtro_condicao, sprintf( " a.acasnrap = '%s' ", isset( $_REQUEST['tipo_acao_emenda'] ) ?  't' : 'f' ) );
array_push( $filtro_condicao, sprintf( " a.acasnemenda = '%s' ", isset( $_REQUEST['tipo_acao_rap'] ) ?  't' : 'f' ) );

$filtro_condicao = implode( ' AND ', $filtro_condicao );

// Programas
$filtro_programa = $_REQUEST['listaprg'] != 'N';

// Agrupamento
$agrupadores = array( 'A', 'U', 'L' );
$filtro_agrupador = in_array( $_REQUEST['ordemgrupo'], $agrupadores ) ? $_REQUEST['ordemgrupo'] : 'A';

// Configurações de Impressão //////////////////////////////////////////////////

$bg = '';
$class = 'notscreen';
$classp = 'notscreen';
if ( $_REQUEST['cor'] == 1 )
{
	$bg = "background-color:%s;";
	$class = 'notprint';
}
if ( $_REQUEST['porcento'] == 1 )
{
	$classp = 'notprint';
}
header( 'Content-Type: text/html; charset=iso-8859-1' );
//ver($_SESSION['exercicio'],d);
?>
<body onload="verificaSeMostraShowModal()">
<?php if( !$_REQUEST['box'] ): ?>
	<?php include APPRAIZ."includes/cabecalho.inc"; ?>
	<?php if($_SESSION['exercicio'] > "2011") die("<script>window.location='monitora.php?modulo=inicio2&acao=C';</script>"); ?>
	<br/>
	<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
	<script type="text/javascript" src="../includes/remedial.js"></script>
	<script type="text/javascript" src="../includes/superTitle.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/listagem2.css">
	<?php
		$db->cria_aba( $abacod_tela, $url, '' );
		$titulo_modulo = "Estatísticas de Avaliação das Ações do MEC";
		monta_titulo( $titulo_modulo, '<div style="margin:3px;" align="right" class="' . $class . '">Legenda: |<span style="background-color:#FFF0EE;"> 0 - 49% </span>|<span style="background-color:#FEFFE6"> 50 - 74% </span>|<span style="background-color:#E9FFEC"> 75 - 99% </span>|<span style="background-color:#E6F7FF"> 100% </span>|</div>' );
	?>
	<form method="post" name="filtro">
		<input type="hidden" name="filtro" value="1"/>
		<table width="95%" border="0" cellspacing="0" cellpadding="2" align="center" bgcolor="#f7f7f7" style="border-top: 1px solid #c0c0c0;">
			<tr>
				<td style="color:#008000;">
					<input type="checkbox" name="responsabilidade" id="responsabilidade" value="1" <?php echo $responsabilidade ? 'checked="checked"' : ''; ?>>
					<label for="responsabilidade">Ações sob minha responsabilidade</label>
					<br/>
					<input type="checkbox" name="responsabilidade_prg" id="responsabilidade_prg" value="1" <?php echo $responsabilidade_prg ? 'checked="checked"' : ''; ?>>
					<label for="responsabilidade_prg">Programas sob minha responsabilidade</label>
				</td>
				<td class="SubTituloDireita">Período:</td>
				<td style="color:#000080;">
					<select name="mes" onchange="document.filtro.submit();">
						<?php
							$sql = "select refmes_ref, refdsc from monitora.referencia where refsngrupo = 'f' and refano_ref = '". $_SESSION['exercicio'] ."' order by refmes_ref";
							$lista_meses = $db->carregar( $sql );
						?>
						<?php foreach( $lista_meses as $mes ): ?>
							<option <?= (integer) $mes['refmes_ref'] == $mes_selecionado ? 'selected="selected"' : '' ?> value="<?= $mes['refmes_ref'] ?>"><?= $mes['refdsc'] ?></option>
						<?php endforeach; ?>
					</select>
				</td>
				<!--
				<td class="SubTituloDireita">Tipo de Ações:</td>
				<td style="color:#000080;">
					<input type="checkbox" name="normal" value="N" <?if ($normal) print 'checked';?>> Normal
					<input type="checkbox" name="tiporap" value="R" <?if ($tiporap) print 'checked';?>> RAP 
					<? if ($db->testa_superuser() or $db->testa_emenda()) {?>
						<input type="checkbox" name="emenda" value="E" <?if ($emenda) print 'checked';?>> Emenda
					<? } else {?>
						<input type="hidden" name="emenda" value="E">
					<?}?>
					<input type="checkbox" name="tipo_acao_rap" id="tipo_acao_rap" value="1" <?= isset( $_REQUEST['tipo_acao_rap'] ) ? 'checked="checked"' : '' ?>>
					<label for="tipo_acao_rap">RAP</label>
					<? if( $db->testa_superuser() or $db->testa_emenda() ): ?>
						<input type="checkbox" name="tipo_acao_emenda" id="tipo_acao_emenda" value="1" <?= isset( $_REQUEST['tipo_acao_emenda'] ) ? 'checked="checked"' : '' ?>>
						<label for="tipo_acao_emenda">Emenda</label>
					<? else: ?>
						<input type="hidden" name="tipo_acao_emenda" value="1">
					<? endif; ?>
				</td>
				-->
				<td class="SubTituloDireita">Programas:</td>
				<td style="color:#008000;">
					<input type="radio" name="listaprg" id="listaprg_S" value="S" <?= $_REQUEST['listaprg'] != 'N' ? 'checked="checked"' : '' ?>>
					<label for="listaprg_S">Mostrar</label>
					<input type="radio" name="listaprg" id="listaprg_N" value="N" <?= $_REQUEST['listaprg'] == 'N' ? 'checked="checked"' : '' ?>>
					<label for="listaprg_N">Suprimir</label>
				</td>
				<td class="SubTituloDireita">Agrupamento:</td>
				<td style="color:#800000;">
					<input type="radio" name="ordemgrupo" id="ordemgrupo_A" value="A" <?= $filtro_agrupador == 'A' ? 'checked="checked"' : '' ?>>
					<label for="ordemgrupo_A">Ação</label>
					<input type="radio" name="ordemgrupo" id="ordemgrupo_U"value="U" <?= $filtro_agrupador == 'U' ? 'checked="checked"' : '' ?>>
					<label for="ordemgrupo_U">Unidade</label>
					<input type="radio" name="ordemgrupo" id="ordemgrupo_L"value="L" <?= $filtro_agrupador == 'L' ? 'checked="checked"' : '' ?>>
					<label for="ordemgrupo_L">Localizador</label>
				</td>
				<td class="SubTituloDireita" style="text-align:center;">
					<input type="Button" style="width:80px;" name="Filtrar" value="Filtrar" onclick="document.filtro.submit();">
				</td>
			</tr>
		</table>
	</form>
<? endif; ?>

	<?php
		if ( $_REQUEST['box'] ) {
			$pai_entidade = array_shift( $_REQUEST['box'] );
			$entidade = $_REQUEST['box'][0];
		} else {
			$entidade = $filtro_programa == true ? 'P' : $filtro_agrupador;
		}
	?>
	<table align="center" width="<?= $_REQUEST['box'] ? '100' : '95' ?>%" border="0" cellpadding="2" cellspacing="0" class="listagem" style="color:#333333">
		<tbody>
			<tr bgcolor="#e7e7e7">
				<td align="center" width="1%">
					&nbsp;
					<!-- <img src="../imagens/mais.gif" border="0" width="9" height="9" align="absmiddle" vspace="3" id="img1" onclick="abre_tudo();"> -->
				</td>
				<td style="width:300px">
					<b>
					<?php
						switch ( $entidade ) {
							case 'A':
								echo 'Ações';
								break;
							case 'U':
								echo 'Unidades';
								break;
							case 'L':
								echo 'Localizadores';
								break;
							default:
							case 'P':
								echo 'Programas';
								break;
						}
					?>
					</b>
				</td>
				<? // if ( $entidade == 'A' && $_REQUEST['box'] ): ?>
				<? if ( $entidade == 'A' && ( $_REQUEST['acacod'] || $_REQUEST['unidade'] || $_REQUEST['localizador'] ) ){?>
					<td style="width: 100px">
						<b><?= $_REQUEST['unidade'] ? 'Localizador' : 'Unidade' ?></b>
					</td>
				<?} else if ($entidade == 'A'){ ?>
					<td style="width: 50px">
						<b>Qtd. Loc.</b>
					</td>				
				<?} else{ ?>
					<td style="width: 50px">
						<b>Qtd. Loc.</b>
					</td>
					<td style="width: 50px">
						<b>Qtd. Ações</b>
					</td>
				<? } ?>
				<?php
					//Montanto titulo do  mostrador visual de status
					$sql = "select * from referencia where refdata_limite_avaliacao_aca is not null and refsngrupo = 'f' and refano_ref='" . $_SESSION['exercicio'] . "' and refmes_ref in ( ". $meses ." ) order by refmes_ref";
					$RS = $db->record_set( $sql );
					$nlinhas = $db->conta_linhas( $RS );
					$colunasref = $nlinhas + 4;
					if ( $nlinhas >= 0 )
					{
						$referencias = array();
						 for ( $i = 0; $i <= $nlinhas; $i++ )
						 {
							$res = $db->carrega_registro( $RS, $i );
						 	$referencias[$i] = $res['refcod'];
							if( is_array( $res ) ) foreach( $res as $k => $v ) ${$k} = $v;
							$cab_mesref[$i] = substr( $refdsc, 0, 3 ) . '/' . $refano_ref;
							print '<td  align="center" style="width:50px;color:#696969;font-size:8px;font-family:Verdana;">' . $cab_mesref[$i] . '</td>';
						}
					}
				?>
			</tr>
			<?php
			
				$sql = get_sql( $entidade, $referencias, $_REQUEST['programa'], $_REQUEST['localizador'], $_REQUEST['unidade'], $_REQUEST['acacod'], $_REQUEST['responsabilidade'], $_REQUEST['responsabilidade_prg'] );
				$j = 0;
				$RS = $db->record_set( $sql );	

				//dump($sql);
									
				$nlinhas = $db->conta_linhas( $RS );
				if ( $nlinhas >= 0 ) {
					for ( $i = 0; $i <= $nlinhas; $i++ )
					{							
						$res = $db->carrega_registro( $RS, $i );
						$total_aca = $res['total_acoes']; 
						
						
						if( is_array( $res ) ) foreach( $res as $k => $v ) ${$k} = trim($v);
						if ( $v_identificador <> $identificador || $v_descricao <> $descricao || $coluna == ( $colunas - 1 ) )
						{ 
							$coluna = 0;
							$id = $j . '-' . $identificador;
							$mes = 0;
							if ( $v_identificador ) {
								echo '</tr><tr><td id="td'. $j . '-' . $v_identificador .'" colspan="'. $colunasref .'" style="padding:0px;display:none;border: 5px red"></tr>';
							}
							?>
							<tr bgcolor="<?= $marcado ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?= $marcado ?>';">
							<?php
							
							$v_identificador = $identificador;
							$v_descricao = $descricao;
							$j = $j + 1;
							$id = $j . '-' . $identificador;
							$marcado = fmod( $j, 2 ) == 0 ? '' : '#F7F7F7';
							
							$box[0] = $entidade;
							if ( $entidade == 'P' ) {
								$box[1] = $filtro_agrupador;
								$box[2] = $box[1] == 'A' ? null : 'A';
							} else {
								$box[1] = 'A';
								$box[2] = null;
							}
							
//							if ( !( $entidade == 'A' && $_REQUEST['box'] ) ) {
							if ( !( $entidade == 'A' && ( $_REQUEST['acacod'] || $_REQUEST['unidade'] || $_REQUEST['localizador'] ) ) ) {
								$programa = $entidade == 'P' ? $identificador : $_REQUEST['programa'];
								$localizador = $entidade == 'L' ? $identificador : $_REQUEST['localizador'];
								$unidade = $entidade == 'U' ? $identificador : $_REQUEST['unidade'];
								$acao = $entidade == 'A' ? $identificador : $_REQUEST['acacod'];
								$onclick_param = array(
									'modulo' => 'inicio',
									'acao' => 'C',
									'programa' => $programa,
									'localizador' => $localizador,
									'unidade' => $unidade,
									'acacod' => $acao,
									'mes' => $mes_selecionado,
									'box' => $box,
									'responsabilidade' => $_REQUEST['responsabilidade'] 
								);
								$onclick = sprintf(
									"abreconteudo( 'monitora.php?%s', '%s' );",
									http_build_query( $onclick_param ),
									$id
								);
								$funcao_abretudo .= " $onclick\n";
							}
							if ( $onclick ) {
								$img = sprintf(
									'<img src="../imagens/mais.gif" style="padding-right: 5px" border="0" width="9" height="9" align="absmiddle" vspace="3" id="img%s" name="+" onclick="%s"/>',
									$id,
									$onclick
								);
							}
							switch ( $entidade ) {
								case 'A':
									if ( $_REQUEST['acacod'] || $_REQUEST['unidade'] || $_REQUEST['localizador'] ) {
										$sql = sprintf(
											"select u.usucpf, u.usunome, u.usuemail, u.usufoneddd, u.usufonenum
											from monitora.usuarioresponsabilidade r
											inner join seguranca.usuario u using ( usucpf )
											where r.acaid = '%s' and r.pflcod = 1 and r.rpustatus = 'A'",
											$identificador
										);
//									
										$coordenador = $db->pegaLinha( $sql );
										if ( $coordenador ) {
											$texto_coordenador = sprintf( '<b>Coordenador</b><br/>%s<br/>%s<br/>%s %s', $coordenador['usunome'], $coordenador['usuemail'], $coordenador['usufoneddd'], $coordenador['usufonenum'] );
										} else {
											$texto_coordenador = 'Coordenador não atribuído.';
										}
										$supertitle = 'onmousemove="SuperTitleOn( this, \''. $texto_coordenador .'\' )" onmouseout="SuperTitleOff( this )"';
										$link = sprintf( '<a href="monitora.php?modulo=principal/acao/cadacao&acao=C&acaid=%s&prgid=%s" %s>%s</a>', $identificador, $prgid, $supertitle, $descricao );
										break;
									}
								case 'U':
								case 'L':
									$link = $codigo .' - '. $descricao;
									break;
								default:
								case 'P':
									$link = sprintf( '<a href="monitora.php?modulo=principal/programa/cadprograma&acao=C&prgid=%s">%s</a>', $identificador, $codigo .' - '. $descricao );									
									break;
							}
							
							if( ! $img )
							{
								$img = '<img src="../imagens/mais.gif" style="visibility: hidden;padding-right: 5px" border="0" width="9" height="9" align="absmiddle" vspace="3" id="img%s" name="+"/>';
							}
							
							if ( $entidade == 'A' && ( $_REQUEST['acacod'] || $_REQUEST['unidade'] || $_REQUEST['localizador'] ) ) {
								$largura = '300';
							} else {
								$largura = '400';
							}
							
							printf(
								'<td  style="text-align: center">&nbsp;%s</td><td id="bolinha" style="width: %spx" ><font color="#003366">%s%s</font></td>',
								$img,
								$largura,
								$_REQUEST['box'] ? '<img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0" style="margin-bottom: -3px; margin-right: 5px">' : '',
								$link
							);
							//totais unidades/localizadores
//							if ( $entidade == 'A' && $_REQUEST['box'] ) {
							if ( $entidade == 'A' && ( $_REQUEST['acacod'] || $_REQUEST['unidade'] || $_REQUEST['localizador'] ) ) {
								printf(
									'<td align="left" style="width: 150px">(%s)</td>',
									$_REQUEST['unidade'] ? $localizador : $unidade
								);
							} else if($entidade == 'A') {
								// totais programas
								printf(
									'<td align="right" style="width: 50px">%s</td>',
									$total 
								);								
								
								$total_acoes = $total_acoes + $total;  
							}else {
								// totais programas
								printf(
									'<td align="right" style="width: 50px">%s</td>',
									$total 
								);
								
								printf(
									'<td align="right" style="width: 50px">%s</td>',
									$total_aca
								);
								$total_acoes = $total_acoes + $total;  
							}
							
						} else {
							$coluna++;
						}
						
//						if ( $entidade == 'A' && $_REQUEST['box'] ) {
						if ( $entidade == 'A' && ( $_REQUEST['acacod'] || $_REQUEST['unidade'] || $_REQUEST['localizador'] ) ) {
							$cor_avaliacao = !$cor_avaliacao ? 0 : $cor_avaliacao;
							$cor_parecer = !$cor_parecer ? 0 : $cor_parecer;
							$situacao = !$situacao ? 0 : $situacao;
							if ( $situacao == 1 ) {
								$cor_parecer = $cor_avaliacao;
							}
							$texto = $mes_referencia .'/'. $ano_referencia .': '. $estado[$cor_avaliacao] .'/'. $estado[$cor_parecer] .' - '. $situacoes[$situacao];
							printf(
								'<td align="center"><a href="monitora.php?modulo=principal/acao/monitoraacao&acao=A&refcod=%s&acaid=%s"><img border="0" src="../imagens/Sin_%s_%s_%s.gif" title="%s"/></a></td>',
								$referencia,
								$identificador,
								$cor[$cor_avaliacao],
								$cor[$cor_parecer],
								$situacoes_letra[$situacao],
								$texto
							);							
						} else {
							$porcentopreenchido = number_format( 100 - ( $cor0 * 100 / $total ), 0, ',', '.' );
							if ( $porcentopreenchido < 50 ) {
								$cor = '#FFF0EE';
							} elseif ( $porcentopreenchido < 75 ) {
								$cor = '#FEFFE6';
							} elseif ( $porcentopreenchido < 100 ) {
								$cor = '#E9FFEC';
							} else {
								$cor = '#E6F7FF';
							}
							?>
							<td align="left" nowrap style="text-align: center;font-size:8px;font-family:Verdana;<?= sprintf( $bg, $cor ) ?>">
								<?= monta_grafico_avaliacao( $cor0, $cor1, $cor2, $cor3, $total, $cab_mesref[$mes], $porcentopreenchido . '%' ) ?>
								<span class="<?= $classp ?>"><br><?= $porcentopreenchido ?>%</span>
							</td>
							<?php
						}
						
						$mes++;
					}
				}
			?>
			</tr>
			<tr>
				<td id="td<?= $j ?>-<?= $v_identificador ?>" colspan="<?= $colunasref ?>" style="padding:0px;display:none;border: 5px red"></td>
			</tr>
			<tr>
				<td>
					<b>TOTAL <?php
						switch ( $entidade ) {
							case 'A':
								echo 'AÇÕES';
								break;
							case 'U':
								echo 'UNIDADES';
								break;
							case 'L':
								echo 'LOCALIZADORES';
								break;
							default:
							case 'P':
								echo 'PROGRAMAS';
								break;
						}
					?>:
					</b>
				</td>
				<td>
					<b><?=$j;?></b>
				</td>
				<td ALIGN=RIGHT>
					<b><?=$total_acoes;?></b>
				</td>
			</tr>
		</tbody>
	</table>
</body>
	<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script>
/*** INICIO SHOW MODAL ***/

var countModal = 1;

function montaShowModal() {
	var campoTextArea = ''+
		'<p align=\'justify\'>Informamos que o Sistema Integrado de Planejamento, Orçamentação e Custos, constante deste módulo, encontra-se disponível para a geração dos Planos de Trabalho, das Subações Orçamentárias e seus respectivos Planos Internos (PI).'+
		'<br /><br />As Unidades Orçamentárias 26.101 - Administração Direta; 26.290 - Instituto Nacional de Estudos e Pesquisas Educacionais Anísio Teixeira - INEP; 26.291 - Fundação Coordenação de Aperfeiçoamento de Pessoal de Nível Superior - CAPES; e, 26.298 - Fundo Nacional de Desenvolvimento da Educação - FNDE deverão seguir os seguintes passos: <a href=\'#\' onclick=\'redirecionaPDF("obrigatorias"); closeMessage();\'>clique aqui</a>.'+
		'<br /><br />As demais Unidades Orçamentárias vinculadas ao Ministério da Educação - 26.000 deverão seguir os seguintes passos: <a href=\'#\' onclick=\'redirecionaPDF("outras"); closeMessage();\'>clique aqui</a>.</p>';
	var alertaDisplay = '<div class="titulo_box" >Senhores Usuários,<br/ >'+campoTextArea+'</div><div class="links_box" ><center><input type="button" onclick=\'closeMessage(); return false \' value="Fechar" /></center>';
	displayStaticMessage(alertaDisplay,false);
	return false;
}

function displayStaticMessage(messageContent,cssClass) {
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	messageObj.setHtmlContent(messageContent);
	messageObj.setSize(520,300);
	messageObj.setCssClassMessageBox(cssClass);
	messageObj.setSource(false);	// no html source since we want to use a static message here.
	messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
	messageObj.display();
}

function closeMessage() {
	messageObj.close();	
}

function redirecionaPDF(tipo){
	window.location.href = 'monitora.php?modulo=inicio&acao=C&tipoUnidade='+tipo+'&boImprimeManualPDF=1';
}


function verificaSeMostraShowModal(){
	var mostraShowModal = '<?php echo $mostraShowModal; ?>';
	if(mostraShowModal){
		montaShowModal();	
	}
}

</script>