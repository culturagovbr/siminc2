<script>
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
</script>
<?
/*
 Sistema Simec
 Setor responsável: SPO-MEC
 Desenvolvedor: Equipe Consultores Simec
 Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
 Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
 Módulo:gerencusu.inc
 Finalidade: permitir a construção de relatorio gerenciais de usuarios
 */
$modulo=$_REQUEST['modulo'] ;//
unset($_SESSION['LINHA']);
unset($_SESSION['title']);
unset($_SESSION['TIT']);
unset($_SESSION['data']);
unset($_SESSION['LIN']);
$_SESSION['LIN']= array();
$_SESSION['TIT']=array();
$habil = 'S';

$ano = $_SESSION['exercicio'];

$grupounidade = "";
$responsabilidade = "";
$programa = "";

if ($_REQUEST['relat']=='r')
{
	$statusUsuario = ($_POST['statusUsuario'] ) ? $_POST['statusUsuario'] : 'A';
	
	$ok=0;
	$sql1 = "select distinct u.usucpf as codigo,un.unicod,case when u.ungcod <> '' then u.usunome ||' / '|| u.usufoneddd||'-'||u.usufonenum||'/ '||o.orgdsc ||' / '||un.unidsc ||' / '|| ung.ungdsc ||' - '|| u.usufuncao when u.unicod <> '' then u.usunome ||' / '||u.usufoneddd||'-'||u.usufonenum|| '/ '||o.orgdsc ||' / '||un.unidsc ||' - '|| u.usufuncao else u.usunome ||' / '||u.usufoneddd||'-'||u.usufonenum||'/ '|| o.orgdsc ||' - '||u.usufuncao end as descricao, un.unidsc from seguranca.usuario u inner join seguranca.usuario_sistema us on us.usucpf=u.usucpf and us.sisid=".$_SESSION['sisid'];
	$where = "  where u.suscod = '". $statusUsuario ."' ";

	$sql = ' left join orgao o on o.orgcod = u.orgcod left join unidade un on un.unicod =u.unicod  left join unidadegestora ung on ung.ungcod=u.ungcod ';
	$titul = 'Você buscou os Usuários que: ';
	$tit = 'Relatório de usuários: ';

	$sql.=" inner join usuarioresponsabilidade ur on ur.usucpf=u.usucpf and ur.pflcod in ('".implode("','",$_REQUEST['pflcod'])."') and ur.prsano = '".$ano."'"; 
	
	/*UF e Municipio*/
	if ($_REQUEST['regcod_disable']) $where.= " and u.regcod = '".$_REQUEST['regcod_disable']."'";
	if ($_REQUEST['muncod']) $where.= " and u.muncod = '".$_REQUEST['muncod']."'";

	if ($_REQUEST['data_ini'])
	{
		$where .=" and u.usudatainc >= '".$_REQUEST['data_ini']."' ";
		$titul.= '| tenham se cadastrado após '.$_REQUEST['data_ini'];
		$tit.= '| tenham se cadastrado após '.$_REQUEST['data_ini'];
	}

	if ($_REQUEST['data_fim'])
	{
		$where .=" and u.usudatainc <= '".$_REQUEST['data_fim']."' ";
		$titul.= '| tenham se cadastrado até '.$_REQUEST['data_fim'];
		$tit.= '| tenham se cadastrado até '.$_REQUEST['data_fim'];
	}

	if ($_REQUEST['orgcod'])
	{
		$sql .=" left join orgao on o.orgcod=u.orgcod and o.orgcod='".$_REQUEST['orgcod']."'";
		$sqldsc = "select orgdsc from orgao where orgcod='".$_REQUEST['orgcod']."'";
		$titulo=$db->recuperar($sqldsc);
		$titul.= '| pertençam a(ao) '.$titulo['orgdsc'];
		$tit.= '| do '.$titulo['orgdsc'];
	}
	if ($_REQUEST['unicod'])
	{
		$sql .= " inner join unidade und on und.unicod=u.unicod and und.unicod='".$_REQUEST['unicod']."'";
		$sqldsc = "select unidsc from unidade where unicod='".$_REQUEST['unicod']."'";
		$titulo=$db->recuperar($sqldsc);
		$titul.= ' e '.$titulo['unidsc'];
		$tit.= ' e '.$titulo['unidsc'];
	}

	if ($_REQUEST['gunid'] && $_REQUEST['gunid'][0])
	{
		$sql .= " inner join unidade un2 on un2.unicod = u.unicod inner join grupounidade gu on gu.gunid=un2.gunid and gu.gunid in ('".implode("','",$_REQUEST['gunid'])."')";
		$sqldsc = "select gundsc from grupounidade where gunid in ('".implode("','",$_REQUEST['gunid'])."')";
		$titulo=$db->recuperar($sqldsc);
		$titul.= '| Unidade tipo: '.$titulo['gundsc'];
		$tit.= '| Unidade tipo: '.$titulo['gundsc'];
	}

	if ($_REQUEST['ungcod'])
	{
		$sql .= " inner join unidadegestora ung2 on ung2.ungcod=u.ungcod and ung2.ungcod ='".$_REQUEST['ungcod']."'";
		$sqldsc = "select ungdsc from unidadegestora where ungcod='".$_REQUEST['ungcod']."'";
		$titulo=$db->recuperar($sqldsc);
		$titul.= ' e '.$titulo['ungdsc'];
		$tit.= ' e '.$titulo['ungdsc'];
	}
	if ($_REQUEST['usunome'])
	{
		$where .= " and u.usunome ilike '%".$_REQUEST['usunome']."%'";
		$titul.= '| contenham '.$_REQUEST['usunome'].' no Nome';
		$tit.= '| que contenham '.$_REQUEST['usunome'].' no Nome';
	}

	if ($_REQUEST['usucpf'])
	{
		$where .= " and u.usucpf ilike '%".ereg_replace ("[-.]", "", $_REQUEST['usucpf'])."%'";
		$titul.= '| tenha '.$_REQUEST['usucpf']." no CPF";
		$tit.= '| tenha '.$_REQUEST['usucpf'].' no CPF';
	}
	if ($_REQUEST['pflcod'] && $_REQUEST['pflcod'][0])
	{
		$sql .= " inner join perfilusuario pu on u.usucpf=pu.usucpf and pu.pflcod in ('".implode("','",$_REQUEST['pflcod'])."')";
		$sqldsc = "select pfldsc from perfil where pflcod in ('".implode("','",$_REQUEST['pflcod'])."')";
		$titulo=$db->recuperar($sqldsc);
		$titul.= '| Tenham responsabilidade de  '.$titulo['pfldsc'];
		$tit.= '| Tenham responsabilidade de '.$titulo['pfldsc'];
	}
	if ($_REQUEST['exceto'])
	{
		$org = substr($_SESSION['ittorgao'],0,2);
		$where .= " and substr(u.orgcod,0, 3) not in ($org) ";
		$titul.= '| Exceto o '.$_SESSION['ittabrev'];
		$tit.= '| Exceto o '.$_SESSION['ittabrev'];
	}
	$perfis_ano = Array(2,3,20,22,1,8);
	if ($_REQUEST['pflcod'] && $_REQUEST['pflcod'][0] && in_array($_REQUEST['pflcod'],$perfis_ano) )
	{
		// verifico se a responsabilidade está associada a programa
		$sql.=" inner join usuarioresponsabilidade ur on ur.usucpf=u.usucpf and ur.pflcod in ('".implode("','",$_REQUEST['pflcod'])."') and ur.prsano = '".$ano."'";
	}
	
	if ($_REQUEST['pflcod'] && $_REQUEST['pflcod'][0] && $_REQUEST['prgid'][0])
	{
		$sqldsc = "select prgdsc from programa where prgid in ('".implode("','",$_REQUEST['prgid'])."')";
		$titulo=$db->recuperar($sqldsc);
		$titul.= '| Programa:'.$titulo['prgdsc'];
		$tit.= '| Programa:'.$titulo['prgdsc'];
		$prog='ok';
		if (! $_REQUEST['acaid'])
		{
			// verifico se a responsabilidade está associada a programa
			$sql.=" inner join acao a on ur.acaid=a.acaid inner join programa p on p.prgid = a.prgid and p.prgcod in ('".implode("','",$_REQUEST['prgid'])."')";
		}
		else
		{
			$sqldsc = "select acadsc from acao where acaid=".$_REQUEST['acaid'];
			$titulo=$db->recuperar($sqldsc);
			$titul.= '| Ação:'.$titulo['acadsc'];
			$tit.= '| Ação:'.$titulo['acadsc'];
			$acao='ok';
			$sql.=" inner join acao a on ur.acaid=a.acaid and a.acaid=".$_REQUEST['acaid']."  inner join programa p on p.prgid = a.prgid and p.prgid in ('".implode("','",$_REQUEST['prgid'])."')";
		}
	}

	if (!$_REQUEST['pflcod'][0] && $_REQUEST['prgid'][0]) {
		if ($prog<>'ok'){
			$sqldsc = "select prgdsc from programa where prgid in ('".implode("','",$_REQUEST['prgid'])."')";
			$titulo=$db->recuperar($sqldsc);
			$titul.= '| Programa:'.$titulo['prgdsc'];
			$tit.= '| Programa:'.$titulo['prgdsc'];
		}
		if (! $_REQUEST['acaid']){
			// verifico se a responsabilidade está associada a programa
			$sql.=" inner join acao a on ur.acaid=a.acaid inner join programa p on p.prgid = a.prgid and p.prgid in ('".implode("','",$_REQUEST['prgid'])."')";
		} else {
			if ($acao<>'ok'){
				$sqldsc = "select acadsc from acao where acaid=".$_REQUEST['acaid'];
			    $titulo=$db->recuperar($sqldsc);
			    $titul.= '| Ação:'.$titulo['acadsc'];
			    $tit.= '| Ação:'.$titulo['acadsc'];
			}
			$sql.=" inner join acao a on ur.acaid=a.acaid and a.acaid=".$_REQUEST['acaid']."  
					inner join programa p on p.prgid = a.prgid and p.prgid in ('".implode("','",$_REQUEST['prgid'])."')";
		}
	}
	if ($_REQUEST['cbbgu']) {
		
		if ($_REQUEST['pflcod'] && $_REQUEST['pflcod'][0] && in_array($_REQUEST['pflcod'],$perfis_ano) ) 
		{
		// verifico se a ação é bgu
		$sql.=" inner join acao ac on urr.acaid=ac.acaid and ac.acasnbgu='".$_REQUEST['cbbgu']."' ";
		}
		else
		{
		// verifico se a ação é bgu
		$sql.=" inner join usuarioresponsabilidade urr on urr.usucpf=u.usucpf and urr.prsano = '".$ano."'   
				inner join acao ac on urr.acaid=ac.acaid and ac.acasnbgu='".$_REQUEST['cbbgu']."' ";
		}

		if ($_REQUEST['cbbgu']=='t') {
			$titul.= '| Ação BGU';
			$tit.= '| Ação BGU';
	  	} else {
	  		$titul.= '| Ação NÃO BGU';
	  		$tit.= '| Ação NÃO BGU';
	  	}
	}
	
	if ($_REQUEST['cbest']) {
		
		if ($_REQUEST['pflcod'] && $_REQUEST['pflcod'][0] && in_array($_REQUEST['pflcod'],$perfis_ano) ) 
		{
			// verifico se a ação é bgu
		$sql.=" inner join acao acc on urrr.acaid=acc.acaid and acc.acasnestrategica='".$_REQUEST['cbest']."' ";
		}
		else
		{
			// verifico se a ação é bgu
		$sql.=" inner join usuarioresponsabilidade urrr on urrr.usucpf=u.usucpf and ur.prsano = '".$ano."'   
				inner join acao acc on urrr.acaid=acc.acaid and acc.acasnestrategica='".$_REQUEST['cbest']."' ";
		}
		if ($_REQUEST['cbest']=='t') {
			$titul.= '| Ação Estratégica';
			$tit.= '| Ação Estratégica';
	  	} else {
	  		$titul.= '| Ação NÃO Estratégica';
	  		$tit.= '| Ação NÃO Estratégica';
	  	}
	}
	$sqlrel=$sql1.$sql.$where.'  order by un.unicod,un.unidsc,codigo';
	//dbg($sqlrel);
}

if ($_REQUEST['relat']=='planilha_lista')
{
	unset ($_REQUEST['relat']);
	header('Content-type: application/xls');
	header('Content-Disposition: attachment; filename="planilha_simec.xls"');
	for ($i=1;$i<count($_SESSION['cab']);$i++)
	{
		if ($i<count($_SESSION['cab'])-1) print trim(($_SESSION['cab'][$i])). ' ' . "\t";
		else print($_SESSION['cab'][$i]);
	}
	print chr(13);
	if (count($_SESSION['conteudo'][0])>1)
	{
		for ($i=1;$i<count($_SESSION['conteudo']);$i++)
		{
			foreach($_SESSION['conteudo'][$i] as $k=>$v)
			{
				if ($k<>'acao'){
					${$k}=$v;
					if ($v == 't') $v='1';
					else if ($v=='f') $v='0';
					if (! is_int($k) and $v <>'')
					{
						if (is_numeric($v)) $v=formatar_cpf($v);
						print $v."\t";
					}
				}
			}
			print chr(13);
		}
	}
	exit();
}

if(isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'listar_mun'){	
	$sql = "SELECT muncod, mundescricao as mundsc 
		FROM territorios.municipio 
		WHERE estuf = '".$_REQUEST['estuf']."' ORDER BY mundsc";
	$dados = $db->carregar($sql);
	
	$enviar = '';
	if($dados) $dados = $dados; else $dados = array();
			$enviar .= "<option value=\"\"> Todos </option> \n";
	foreach ($dados as $data) {
		$enviar .= "<option value= ".$data['muncod'].">  ".simec_htmlentities($data['mundsc'])." </option> \n";
	}
			
	die($enviar);
}

include APPRAIZ."includes/cabecalho.inc";
?>
<script	language="JavaScript" src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<br>
<?
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Relatório Gerencial de Usuários';
monta_titulo($titulo_modulo,'');
?>
<div align="center">
<center>

<form method="POST" name="formulario"><input type='hidden' name="modulo" value="<?=$modulo?>"> <input type='hidden' name="relat">
<center>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
	<tr>
		<td colspan=2 align='center'><b>Escolha abaixo os itens desejados e
		pressione o botão "Gerar Relatório"</b></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Orgão:</td>
		<td>
			<?
				$orgcod =$_REQUEST["orgcod"];
				$sql = "select distinct orgcod as codigo, orgdsc as descricao from orgao  where organo = '".$_SESSION['exercicio']."'  order by orgdsc ";
				$db->monta_combo("orgcod",$sql,$habil,"Todos os Órgãos",'atualizaComboUnidade','');?>
			<br>
			<input type="checkbox" name="exceto" value='1' title='Filtrar todos os órgãos com exceção do MEC'><b>Exceto o <?=$_SESSION['ittabrev']?></b>
		</td>
	</tr>
	<?	  if ($orgcod) {  ?>
	<tr bgcolor="#F2F2F2">
		<td align='right' class="SubTituloDireita">Unidade:</td>
		<td>
			<?
		  	$unicod =$_REQUEST["unicod"];
		  	$sql = "select unicod as codigo, unidsc as descricao from unidade where unistatus='A' and unitpocod='U' and orgcod ='".$_REQUEST["orgcod"]."' order by unidsc ";
		  	$db->monta_combo("unicod",$sql,$habil,"Todas as Unidades",'atualizaComboUnidade','');
			?>
		</td>
	</tr>
	<?}?>
	<?
	if ($unicod == '26101' and $orgcod== CODIGO_ORGAO_SISTEMA) {?>
	<tr bgcolor="#F2F2F2">
		<td align='right' class="subtitulodireita">Unidade Gestora:</td>
		<td>
			<?
			$ungcod = $_REQUEST['ungcod'];
			$sql = "select ungcod as codigo,ungcod||' - '||ungdsc as descricao from unidadegestora where ungstatus='A' and unitpocod='U' and unicod ='".$unicod."' order by ungdsc ";
			$db->monta_combo("ungcod",$sql,'S',"Todas as Unidades Gestoras",'','');
			?>
		</td>
	</tr>
	<?}?>
	<tr>
		<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'grupounidade' );">Grupo de Unidade:
			<input type="hidden" id="grupounidade_campo_flag" name="grupounidade_campo_flag" value="0"/>
		</td>
		<td>
			<div id="grupounidade_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'grupounidade' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
			<div id="grupounidade_campo_on" style="display:none;">
				<? $sql_combo = "select gunid as codigo,gundsc as descricao from grupounidade where gunstatus='A' order by gundsc "; 
				if ( $_REQUEST['gunid'] && $_REQUEST['gunid'][0] != '' )	
				{
					$sql_carregados = "select gunid as codigo,gundsc as descricao from grupounidade where gunstatus='A' and gunid in ('".implode("','",$_REQUEST['gunid'])."') order by gundsc ";
					$grupounidade=$db->carregar( $sql_carregados );
				}
				?>
				<? combo_popup( 'gunid', $sql_combo, 'Selecione o(s) Grupo(s) de Unidade(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
			</div>
		</td>
	</tr>
	<?php if ( $grupounidade )  { ?>	<script type="text/javascript"> onOffCampo( 'grupounidade' ); </script> <? } ?>
	<tr>
		<td align='right' class="SubTituloDireita">Nome (ou parte):</td>
		<td><?=campo_texto('usunome','N','S','',100,100,'','');?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">CPF:</td>
		<td><?=campo_texto('usucpf','','','',16,14,'###.###.###-##','');?></td>
	</tr>
	<tr>
		<td colspan=2><hr></td>
	</tr>
	<tr bgcolor="#F2F2F2">
		<td align='right' class="SubTituloDireita">Unidade Federativa:</td>
		<td>
			<?php
				$sql = "SELECT regcod AS codigo, regcod||' - '||descricaouf AS descricao FROM uf WHERE codigoibgeuf IS NOT NULL ORDER BY 2";
				$db->monta_combo("regcod",$sql,"","&nbsp;",'listar_municipios','');
			?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Município:</td>
		<td>
			<div id="muncod_on" style="display:none;">
				<select id="muncod" name="muncod" onchange="" class="CampoEstilo" <?php //$editavel == 'S' ? "" : 'disabled="disabled"'?> ></select>
			</div>
			<div id="muncod_off" style="color:#909090;">Selecione uma Unidade Federativa.</div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'responsabilidade' );">Responsabilidade:
			<input type="hidden" id="responsabilidade_campo_flag" name="responsabilidade_campo_flag" value="0"/>
		</td>
		<td>
			<div id="responsabilidade_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'responsabilidade' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
			<div id="responsabilidade_campo_on" style="display:none;">
				<? $sql_combo = "select pflcod as codigo,pfldsc as descricao from perfil where sisid=".$_SESSION['sisid']." order by pfldsc "; 
				if ( $_REQUEST['pflcod'] && $_REQUEST['pflcod'][0] != '' )	
				{
					$sql_carregados = "select pflcod as codigo,pfldsc as descricao from perfil where sisid=".$_SESSION['sisid']." and pflcod in ('".implode("','",$_REQUEST['pflcod'])."') order by pfldsc ";
					$responsabilidade=$db->carregar( $sql_carregados );
				}
				?>
				<? combo_popup( 'pflcod', $sql_combo, 'Selecione a(s) Responsabilidade(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
			</div>
		</td>
	</tr>
	<?php if ( $responsabilidade )  { ?>	<script type="text/javascript"> onOffCampo( 'responsabilidade' ); </script> <? } ?>
	<tr>
		<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'programa' );">Programa
			<input type="hidden" id="programa_campo_flag" name="programa_campo_flag" value="0"/>
		</td>
		<td>
			<div id="programa_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'programa' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
			<div id="programa_campo_on" style="display:none;">
				<? $sql_combo = "select p.prgcod as codigo, p.prgcod || ' - ' || p.prgdsc as descricao from financeiro.execucao r inner join planejamento.programa p on r.prgcod = p.prgcod where r.rofano = '" . $ano . "' group by p.prgcod, p.prgdsc order by p.prgcod, p.prgdsc";
				if ( $_REQUEST['prgid'] && $_REQUEST['prgid'][0] != '' )	
				{
					$sql_carregados = "select p.prgcod as codigo, p.prgcod || ' - ' || p.prgdsc as descricao from financeiro.execucao r inner join planejamento.programa p on r.prgcod = p.prgcod where r.rofano = '" . $ano . "' and p.prgcod in ('".implode("','",$_REQUEST['prgid'])."') group by p.prgcod, p.prgdsc ";
					$programa=$db->carregar( $sql_carregados );
				}
				?>
				<? combo_popup( 'prgid', $sql_combo, 'Selecione o(s) Programa(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
			</div>
		</td>
	</tr>
	<?php if ( $programa )  { ?>	<script type="text/javascript"> onOffCampo( 'programa' ); </script> <? } ?>
	<tr>
	<tr>
		<td colspan=2><hr></td>
	</tr>
	<?php if ($_REQUEST["prgid"] && $_REQUEST["prgid"][0]) { ?>
	<tr>
		<td align='right' class="SubTituloDireita">Ação:</td>
		<td>
			<?
			$acaid = $_REQUEST['acaid'];
			$sql = "select acaid as codigo,acacod ||'.'||unicod||'-'||acadsc as descricao from acao where acasnrap='f' and prgid in ('".implode("','",$_REQUEST['prgid'])."') order by acacod ||'.'||unicod||'-'||acadsc ";
			$db->monta_combo("acaid",$sql,'S',"Todas as ações",'','');?>
		</td>
	</tr>
	<?php }?>
	<tr>
		<td align='right' class="SubTituloDireita">Status do Usuário:</td>
		<td>
			<select class="CampoEstilo" id="statusUsuario" name="statusUsuario">
				<?php 
					$sql = "select suscod, susdsc from seguranca.statususuario";	
					$coStatus = $db->carregar( $sql );
			
					foreach( $coStatus as $arStatus ){ 
					
						$selected = $arStatus["suscod"] == $_REQUEST["statusUsuario"] ? 'selected="selected" ' : "" ?>
				
						<option <?= $selected; ?> value="<?php echo $arStatus["suscod"] ?>"><?php echo $arStatus["susdsc"] ?></option>
				<?php 
					} 
				?>
					<option value="0">Todos</option>
			</select>
		</td>
	</tr>
	<tr>
		<td colspan=2><hr></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">BGU:</td>
		<td><input type='radio' value='f' name='cbbgu'>Não<input type='radio' value='t' name='cbbgu'>Sim</td>
	</tr>
	<tr>
		<td colspan=2><hr></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Estratégica:</td>
		<td><input type='radio' value='f' name='cbest'>Não<input type='radio' value='t' name='cbest'>Sim</td>
	</tr>
	<tr>
		<td colspan=2><hr></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Período de Cadastramento:</td>
		<td>De <?=campo_data('data_ini', 'N','S','','S');?> a <?=campo_data('data_fim', 'N','S','','S');?></td>
	</tr>
	<tr bgcolor="#CCCCCC">
		<td></td>
		<td align='center'><input type="button" name="btrelat" value="Gerar Relatório" onclick="validar_relatorio()" class="botao"></td>
	</tr>
</table>
</center>
</form>
</center>
</div>
	<?
	if ($sqlrel){
		
		unset($_SESSION['rsdados']);
		$RS = $db->record_set($sqlrel);
		$_SESSION['rsdados'] = $db->carrega_tudo($RS);
		$_SESSION['LINHA'] = $db->conta_linhas($RS);
		$_SESSION['title'] = $tit.    '       (Total='.($_SESSION['LINHA']+1).')';

		$_SESSION['data']= date('(h:m) d/m/Y');
		$_SESSION['LIN'] = array();
		print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>'.$titul. '       (Total='.($_SESSION['LINHA']+1).')</b></td><td>';
		if ($_SESSION['LINHA'] >=0) print '<input type="button" class="botao" name="btrelat" value ="Gerar PDF" align="left" onclick="imprime_rel()">&nbsp;&nbsp;<a href="ar505ptb.exe"><img border="0" width="30" heigth="30" src="../imagens/pdf_adobe.jpg" title=" Você não tem o Adobe Reader? Clique aqui!"></a>';
		//   print '&nbsp;&nbsp;<input type="button" class="botao" name="btcsv" value ="Gerar Planilha" align="left" onclick="imprime_rel(4)">';
		print '</td></tr></table>';
?>
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
	<thead>
		<tr>
			<td valign="top" class="title"><strong>Ação</strong></td>
			<td valign="top" class="title"><strong>CPF</strong></td>
			<td valign="top" class="title"><strong>Nome / Órgão</strong></td>
			<td valign="top" class="title"><strong>Unidade</strong></td>
			<td valign="top" class="title"><strong>Responsabilidades</strong></td>
		</tr>
	</thead>
	<tbody>
	<?
	//
	if ($_SESSION['LINHA'] >= 0) {
		$_SESSION['cab']=array('Ação','CPF ','Nome','Unidade','Responsabilidade');
		for ($i=0; $i<=$_SESSION['LINHA'];$i++){
			$res = $db->carrega_registro($RS,$i);
			$_SESSION['conteudo'][]= $res;
			// a linha abaixo transforma em variáveis todos os campos do array
			if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
			if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
			print '<tr class="'.$marcado.'"><td><img border="0" src="../imagens/alterar.gif" title="Consultar cadastro." onclick="consulta_cad(\''.$codigo.'\')"></td><td>'.formatar_cpf($codigo)."</td><td>$descricao</td><td>$unicod - $unidsc</td><td>";
			$sql = "select distinct p.pfldsc || case when u.prgid > 0 then ' -   Programa - ' || pg.prgcod ||'-'|| pg.prgdsc when u.acaid > 0 then ' -   Ação - '||a.prgcod||'-'|| a.acacod||'.'||a.unicod ||'-'||a.acadsc when u.pjeid is null and u.acaid is null and u.prgid is null then '' end as resp from perfilusuario pu inner join perfil p on pu.pflcod = p.pflcod left join usuarioresponsabilidade u on u.pflcod=p.pflcod and u.rpustatus='A' and u.usucpf='$codigo' left join programa pg on pg.prgid=u.prgid left join acao a on a.acaid = u.acaid and a.prgano='".$_SESSION['exercicio']."' where pu.usucpf='$codigo' and p.sisid=".$_SESSION['sisid'];
  

			$RS2 = $db->record_set($sql);
			//$_SESSION['conteudo'][]= $db->carrega_tudo($RS2);
			$nlinhas = $db->conta_linhas($RS2);
			for ($j=0;$j<=$nlinhas;$j++)
			{
				$res2 = $db->carrega_registro($RS2,$j);
				$_SESSION['conteudo']['responsabilidade'][]=  $res2;
				if(is_array($res2)) foreach($res2 as $k=>$v) ${$k}=$v;
				if ($resp ) {//($resp <> '-'){
					print $pfldsc.'   -   '.$resp.'<br>';
					$_SESSION['LIN'][$codigo][] = $resp;}
			}
			print '</td></tr>';
		}
	}

	print '</tbody>';
	}
?>
<script>
function validar_relatorio()
{
    document.formulario.relat.value='r';

    selectAllOptions( $('pflcod') );
    selectAllOptions( $('gunid') );
    selectAllOptions( $('prgid') );
    
   	document.formulario.submit();
}
function consulta_cad(cod)
{
   	location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/usuario/cadusuario&acao=C&usucpf="+cod;
}
function atualizaComboUnidade(cod)
  {
	 if (cod > 0) {document.formulario.submit()};
    }
     function atualizaComboAcao(cod)
  {
	 if (cod > 0) {document.formulario.submit()};
    }
function imprime_rel(cod)
{
	
	     //Abre popup em branco
   	 janela = window.open('about:blank',"relatorio","menubar=no,location=no,open=yes,resizable=yes,scrollbars=yes,status=yes,width=600,height=400'");

   	   //Abre relatório pdf no popup
       janela.location ="<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_gerenusu&acao=R";
	
}

function listar_municipios( estuf )
{
	validar_mun = true;    
    var div_on = document.getElementById( 'muncod_on' );
	var div_off = document.getElementById( 'muncod_off' );        
	div_on.style.display = 'block';
	div_off.style.display = 'none';
			
    return new Ajax.Updater('muncod', '<?=$_SESSION['sisdiretorio'] ?>.php?modulo=relatorio/usuario/gerencusu&acao=<?=$_REQUEST['acao'] ?>',
     {
     
        method: 'post',
        parameters: '&servico=listar_mun&estuf=' + estuf,
        onComplete: function(res)
        {	
        // alert(res.responseText);                   
        }
    });
}
</script>