<?php
function carregarIniciativas($dados) {
	global $db;
	$sql = "SELECT 			
				'<img src=\"../imagens/mais.gif\" id=\"iniciativa_'||i.ictid||'\" title=\"mais\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"carregarAcoes(\''||i.ictid||'\',this);\">' as mais, 
				ictcod as codigo, 
				ictdsc as descricao,
				i.ictid,
				count(a.acacod) as qtde 
			FROM 
				monitora.iniciativa i 
			INNER JOIN 
				monitora.objetivoiniciativa o ON o.ictid = i.ictid 
			left JOIN 
				monitora.iniciativaacao ia ON ia.ictid =  i.ictid
			left JOIN
				monitora.acao a ON a.acacod=ia.acacod
			WHERE 
				o.objid='".$dados['objid']."' AND 
				ictstatus='A'
				--and prgano='".$_SESSION['exercicio']."'
			group by i.ictcod,i.ictdsc,i.ictid 
			ORDER BY ictdsc";
	//ver($sql,1);
	$dados_ = $db->carregar($sql);
	
	$arrPerfil = pegaPerfilGeral();
	
	$arrPerfilSituacional = Array(PERFIL_MONIT_VALIDADOR,PERFIL_MONIT_COORDENADOR_ACAO);
	
	?>
	<script type="text/javascript">
	function gerenciarAnaliseIniciativa(ictid) {
//		var j = window.open('monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseSituacionalIniciativa&objid=<?=$dados['objid'] ?>&ictid='+ictid,'AnaliseObjetivosIniciativa','scrollbars=no,height=500,width=700,status=no,toolbar=no,menubar=no,location=no');
		var j = window.open('monitora.php?modulo=principal/telaprograma&acao=A&requisicao=listarAnaliseSituacionalIniciativa&objid='+<?=$dados['objid'] ?>+'&ictid='+ictid,'AnaliseObjetivosIniciativa','scrollbars=no,height=500,width=700,status=no,toolbar=no,menubar=no,location=no');
		j.focus();
	}
	
	function gerenciarAnaliseGlobalIniciativa(ictid)
	{
		var j = window.open('monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseGlobalIniciativa&ictid='+ictid,'Análise Global da Iniciativa','scrollbars=no,height=500,width=700,status=no,toolbar=no,menubar=no,location=no');
		j.focus();
	}
	</script>
	<table width="100%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem" style="color:333333;">
	<? if($dados_[0]) : ?>
	<tr>
		<td class="SubTituloCentro" width="2%">&nbsp;</td>
		<td class="SubTituloCentro">Iniciativas do Programa</td>
	</tr>
	<? foreach($dados_ as $key => $d) : ?>
	<? if($key%2==0) {$cor="#F7F7F7";} else {$cor="";}	?>
	<tr bgcolor="<?=$cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$cor ?>';">
		<td width="2%"><?=$d['mais'] ?></td>
		<td> 
			<img width="12" height="13" border="0" style="margin-bottom: -3px; margin-right: 5px" alt="" src="../imagens/seta_filho.gif"> 
			<?php if($d['qtde'] == 1): ?>
					<img src="../imagens/principal.gif" style="cursor:pointer;" onclick="gerenciarAnaliseIniciativa(<?=$d['ictid'] ?>);" title="Inserir análise situacional">
					<img src="../imagens/alteracao.gif" style="cursor:pointer;" onclick="gerenciarAnaliseGlobalIniciativa(<?=$d['ictid'] ?>);" title="Análise Global da Iniciativa">
			<?php endif; ?>
			<?=$d['codigo'] ?> - <?=$d['descricao'] ?>
		</td>
	</tr>
	<? endforeach; ?>
	<? else : ?>
	<tr><td class="SubTituloEsquerda">Não existem iniciativas do Programa</td></tr>
	<? endif; ?>
	</table>
	
	<?php
	
}


function carregarObjetivos($dados) {
	global $db;
	
	$sql = "SELECT	'<img src=\"../imagens/mais.gif\" id=\"objetivo_'||objid||'\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"carregarIniciativas(\''||objid||'\',this);\">' as mais, 
					objid, 
					prgcod, 
					objcod as codigo, 
					objdsc as descricao 
			From monitora.objetivo
			WHERE prgano='".$_SESSION['exercicio']."' AND prgcod='".$dados['prgcod']."' AND objstatus='A' 
			ORDER BY objdsc
	";
		
	/*CASO O USUÁRIO POSSA VER SOMENTE O OBJETOS VINCULADOS AO SEU PERFIL. - USAR O JOIN E O WHERE ABAIXOS.
		Join monitora.usuarioresponsabilidade u on u.objid = o.objid
		Where prgano='".$_SESSION['exercicio']."' and prgcod='".$dados['prgcod']."' and (objstatus='A' and u.rpustatus = 'A') and u.usucpf = '".$_SESSION['usucpf']."'
	*/
//	ver($sql,0);
	$dados_ = $db->carregar($sql);
//	ver($sql, d);
	?>
	<table width="100%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem" style="color:333333;">
		<? if($dados_[0]) : ?>
			<tr>
				<td class="SubTituloCentro" width="2%">&nbsp;</td>
				<td class="SubTituloCentro">Objetivos</td>
			</tr>
			<? foreach($dados_ as $key => $d) : ?>
			<? if($key%2==0) {$cor="#F7F7F7";} else {$cor="";}	?>
			<tr bgcolor="<?=$cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$cor ?>';">
				<td width="2%"><?=$d['mais'] ?></td>
				<td>
					<img width="12" height="13" border="0" style="margin-bottom: -3px; margin-right: 5px" alt="" src="../imagens/seta_filho.gif"> 
					<a href="monitora.php?modulo=principal/detalhesppa&acao=O&prgcod=<?=$d['prgcod'] ?>&objid=<?=$d['objid'] ?>"><?=$d['codigo'] ?> - <?=$d['descricao'] ?> </a>
				</td>
			</tr>
		<? endforeach; ?>
	<? else : ?>
	<?
	$sql = "select prgcod||'.'||acacod||'.'||unicod||'.'||loccod as codigo,acatitulo as descricao from monitora.acao where prgano ='".$_SESSION['exercicio']."' and (prodsc is not null or prodsc <> '') and acasnrap = false and prgcod = '".$dados['prgcod']."'";
//	ver($sql, d);
	$dados_ = $db->carregar($sql);
	?>
	<? if($dados_[0]) : ?>
	<tr><td class="SubTituloCentro" width="2%">&nbsp;</td><td class="SubTituloCentro">Ações</td></tr>
	<? foreach($dados_ as $key => $d) : ?>
	<? if($key%2==0) {$cor="#F7F7F7";} else {$cor="";}	?>
	<tr bgcolor="<?=$cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$cor ?>';">
		<td width="2%">&nbsp;</td>
		<td>
			<img width="12" height="13" border="0" style="margin-bottom: -3px; margin-right: 5px" alt="" src="../imagens/seta_filho.gif">
			 <a href="monitora.php?modulo=principal/detalhesppa&acao=A&codigo=<?=$d['codigo'] ?>"><?=$d['codigo'] ?> - <?=$d['descricao'] ?></a>
		</td>
	</tr>
	<? endforeach; ?>
	<? else : ?>
	<tr><td class="SubTituloEsquerda">Não existem ações</td></tr>
	<? endif; ?>
	<? endif; ?>
	</table>
	<?php
}

function carregarAcoes($dados) {
	
	global $db;
	
	if($dados['loccod']) {
		$where = "loccod='".$dados['loccod']."' and prgano='".$_SESSION['exercicio']."' and acasnrap = false";
		
		$sql = "SELECT '<img src=\"../imagens/mais.gif\" id=\"acao_'||acacod||'\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"carregarAcoesDetalhes(\''||acacod||'\',\'\',\'".$dados['loccod']."\',this);\">' as mais, 
					   a.acacod||' - '||a.acatitulo as descricao,
					   (select count(*) from monitora.acao where acacod = a.acacod and {$where} and acastatus='A') as qtdloc 
					   from monitora.acao a 
						where {$where} and acastatus='A' and (a.prodsc is not null or a.prodsc <> '') 
						GROUP BY acacod, acatitulo ORDER BY acacod";
		
	}
	if($dados['unicod']) {
		$where = "unicod='".$dados['unicod']."' and prgano='".$_SESSION['exercicio']."' and acasnrap = false";
		
		$sql = "SELECT '<img src=\"../imagens/mais.gif\" id=\"acao_'||acacod||'\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"carregarAcoesDetalhes(\''||acacod||'\',\'".$dados['unicod']."\',\'\',this);\">' as mais, 
					   --acacod||' - '||acadsc as descricao,
					   a.acacod||' - '|| a.acatitulo as descricao,
					   (select count(*) from monitora.acao where acacod = a.acacod and {$where} and acastatus='A') as qtdloc 
					   from monitora.acao a 
					   where {$where} and acastatus='A' and (a.prodsc is not null or a.prodsc <> '') 
						GROUP BY acacod, acatitulo  ORDER BY acacod";
		
	}
	if($dados['ictid']) {
		$where = "ictid='".$dados['ictid']."'";
		
		$sql = "SELECT '<img src=\"../imagens/mais.gif\" id=\"acao_'||a.acacod||'\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"carregarAcoesDetalhes(\''||a.acacod||'\',\'\',\'\',this);\">' as mais, 
					   --acacod||' - '||a.acadsc as descricao,
					   a.acacod||' - '||a.acatitulo as descricao,
					   count(a.acaid) as qtdloc
					   --(select count(*) from monitora.acao where acacod=i.acacod and prgano='".$_SESSION['exercicio']."' and acasnrap = false and acastatus='A') as qtdloc 
					   from monitora.iniciativaacao i 
					   inner join monitora.acao a ON a.acacod=i.acacod
					   WHERE {$where} AND icastatus='A'
					   and prgano='".$_SESSION['exercicio']."' 
					   and acasnrap = false 
					   and acastatus='A' and (a.acatitulo <> '' and a.acatitulo is not null)
					   group by a.acacod,a.acatitulo
					   ORDER BY a.acacod";
		
	}
//	ver($sql,0);
	$dados_ = $db->carregar($sql);
	?>
	<table width="100%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem" style="color:333333;">
	<? if($dados_[0]) : ?>
	<tr><td class="SubTituloCentro" width="2%">&nbsp;</td><td class="SubTituloCentro">Ações</td><td class="SubTituloCentro">Qtd Loc.</td></tr>
	<? foreach($dados_ as $key => $d) : ?>
	<? if($key%2==0) {$cor="#F7F7F7";} else {$cor="";}	?>
	<tr bgcolor="<?=$cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$cor ?>';"><td width="2%"><?=$d['mais'] ?></td><td><img width="12" height="13" border="0" style="margin-bottom: -3px; margin-right: 5px" alt="" src="../imagens/seta_filho.gif"> <?=$d['descricao'] ?></td><td><?=$d['qtdloc'] ?></td></tr>
	<? endforeach; ?>
	<? else : ?>
	<tr><td class="SubTituloEsquerda">Não existem ações</td></tr>
	<? endif; ?>
	</table>
	<?php
	
}

function carregarAcoesDetalhes($dados) {
	global $db;
	
	$sql = "select 
				prgcod||'.'||acacod||'.'||a.unicod||'.'||loccod as codigo, 
				--acadsc as descricao,
				acatitulo as descricao, 
				unidsc as unidade 
			from monitora.acao a 
			left join public.unidade u on u.unicod = a.unicod 
			where prgano ='".$_SESSION['exercicio']."' 
			and acasnrap = false 
			and acacod = '".$dados['acacod']."'".(($dados['unicod'])?" 
			and a.unicod='".$dados['unicod']."'":"").(($dados['loccod'])?" 
			and a.loccod='".$dados['loccod']."'":"")." and (prodsc <> '' or prodsc is not null)
			order by prgcod, acacod, a.unicod, loccod
			";
//ver($sql);
	$dados_ = $db->carregar($sql);
	?>
	<table width="100%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem" style="color:333333;">
	<? if($dados_[0]) : ?>
	<tr><td class="SubTituloCentro" width="2%">&nbsp;</td><td class="SubTituloCentro">Ações</td><td class="SubTituloCentro">Unidade</td></tr>
	<? foreach($dados_ as $key => $d) : ?>
	<? if($key%2==0) {$cor="#F7F7F7";} else {$cor="";}	?>
	<tr bgcolor="<?=$cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$cor ?>';">
		<td width="2%">&nbsp;</td>
		<td>
			<img width="12" height="13" border="0" style="margin-bottom: -3px; margin-right: 5px" alt="" src="../imagens/seta_filho.gif"> 
				<a href="monitora.php?modulo=principal/detalhesppa&acao=A&codigo=<?=$d['codigo'] ?>"><?=$d['codigo'] ?> - <?=$d['descricao'] ?></a>
		</td>
		<td><?=$d['unidade'] ?></td>
	</tr>
	<? endforeach; ?>
	<? else : ?>
	<tr><td class="SubTituloEsquerda">Não existem ações</td></tr>
	<? endif; ?>
	</table>
	<?php
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ."includes/cabecalho.inc";
echo "<br/>";

if($_SESSION['exercicio']<"2012") die("<script>window.location='monitora.php?modulo=inicio&acao=C';</script>");

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
jQuery.noConflict();

function carregarObjetivos(prgcod, obj) {
	divCarregando();
	var linha = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.src = '../imagens/menos.gif';
		obj.title = 'menos';
		var linhan = tabela.insertRow(linha.rowIndex+1);
		var cell0 = linhan.insertCell(0);
		cell0.innerHTML = '&nbsp';
		var cell1 = linhan.insertCell(1);
		cell1.id = 'id_io_'+prgcod+'_'+linhan.rowIndex;
		cell1.innerHTML = 'Carregando...';
	
		jQuery.ajax({
			type: "POST",
			url: "monitora.php?modulo=inicio2&acao=C",
			data: "requisicao=carregarObjetivos&prgcod="+prgcod,
			async: false,
			success: function(html){jQuery('#'+cell1.id).html(html);}
		});

	} else {
		obj.src = '../imagens/mais.gif';
		obj.title = 'mais';
		tabela.deleteRow(linha.rowIndex+1);
	}
	divCarregado();
}

function carregarIniciativas(objid, obj) {
	divCarregando();
	var linha = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.src = '../imagens/menos.gif';
		obj.title = 'menos';
		var linhan = tabela.insertRow(linha.rowIndex+1);
		var cell0 = linhan.insertCell(0);
		cell0.innerHTML = '&nbsp';
		var cell1 = linhan.insertCell(1);
		cell1.colSpan = 2;
		cell1.id = 'id_mi_'+objid+'_'+linhan.rowIndex;
		cell1.innerHTML = 'Carregando...';
	
		jQuery.ajax({
			type: "POST",
			url: "monitora.php?modulo=inicio2&acao=C",
			data: "requisicao=carregarIniciativas&objid="+objid,
			async: false,
			success: function(html){jQuery('#'+cell1.id).html(html);}
		});

	} else {
		obj.src = '../imagens/mais.gif';
		obj.title = 'mais';
		tabela.deleteRow(linha.rowIndex+1);
	}
	divCarregado();
}

function carregarAcoes(ictid, obj) {
	divCarregando();
	var linha = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.src = '../imagens/menos.gif';
		obj.title = 'menos';
		var linhan = tabela.insertRow(linha.rowIndex+1);
		var cell0 = linhan.insertCell(0);
		cell0.innerHTML = '&nbsp';
		var cell1 = linhan.insertCell(1);
		cell1.colSpan = 2;
		cell1.id = 'id_aa_'+ictid+'_'+linhan.rowIndex;
		cell1.innerHTML = 'Carregando...';
	
		jQuery.ajax({
			type: "POST",
			url: "monitora.php?modulo=inicio2&acao=C",
			data: "requisicao=carregarAcoes&ictid="+ictid,
			async: false,
			success: function(html){jQuery('#'+cell1.id).html(html);}
		});

	} else {
		obj.src = '../imagens/mais.gif';
		obj.title = 'mais';
		tabela.deleteRow(linha.rowIndex+1);
	}
	divCarregado();
}

function carregarAcoesUnidade(unicod, obj) {
	divCarregando();
	var linha = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.src = '../imagens/menos.gif';
		obj.title = 'menos';
		var linhan = tabela.insertRow(linha.rowIndex+1);
		var cell0 = linhan.insertCell(0);
		cell0.innerHTML = '&nbsp';
		var cell1 = linhan.insertCell(1);
		cell1.colSpan = 2;
		cell1.id = 'id_uu_'+unicod+'_'+linhan.rowIndex;
		cell1.innerHTML = 'Carregando...';
	
		jQuery.ajax({
			type: "POST",
			url: "monitora.php?modulo=inicio2&acao=C",
			data: "requisicao=carregarAcoes&unicod="+unicod,
			async: false,
			success: function(html){jQuery('#'+cell1.id).html(html);}
		});

	} else {
		obj.src = '../imagens/mais.gif';
		obj.title = 'mais';
		tabela.deleteRow(linha.rowIndex+1);
	}
	divCarregado();
}

function carregarAcoesLocalizador(loccod, obj) {
	divCarregando();
	var linha = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.src = '../imagens/menos.gif';
		obj.title = 'menos';
		var linhan = tabela.insertRow(linha.rowIndex+1);
		var cell0 = linhan.insertCell(0);
		cell0.innerHTML = '&nbsp';
		var cell1 = linhan.insertCell(1);
		cell1.colSpan = 2;
		cell1.id = 'id_ll_'+loccod+'_'+linhan.rowIndex;
		cell1.innerHTML = 'Carregando...';
	
		jQuery.ajax({
			type: "POST",
			url: "monitora.php?modulo=inicio2&acao=C",
			data: "requisicao=carregarAcoes&loccod="+loccod,
			async: false,
			success: function(html){jQuery('#'+cell1.id).html(html);}
		});

	} else {
		obj.src = '../imagens/mais.gif';
		obj.title = 'mais';
		tabela.deleteRow(linha.rowIndex+1);
	}
	divCarregado();
}

function carregarAcoesDetalhes(acacod, unicod, loccod, obj) {
	divCarregando();
	
	var linha = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.src = '../imagens/menos.gif';
		obj.title = 'menos';
		var linhan = tabela.insertRow(linha.rowIndex+1);
		var cell0 = linhan.insertCell(0);
		cell0.innerHTML = '&nbsp';
		var cell1 = linhan.insertCell(1);
		cell1.colSpan = 2;
		cell1.id = 'id_ac_'+acacod+'_'+linhan.rowIndex;
		cell1.innerHTML = 'Carregando...';
	
		jQuery.ajax({
			type: "POST",
			url: "monitora.php?modulo=inicio2&acao=C",
			data: "requisicao=carregarAcoesDetalhes&loccod="+loccod+"&unicod="+unicod+"&acacod="+acacod,
			async: false,
			success: function(html){jQuery('#'+cell1.id).html(html);}
		});

	} else {
		obj.src = '../imagens/mais.gif';
		obj.title = 'mais';
		tabela.deleteRow(linha.rowIndex+1);
	}
	divCarregado();
}

</script>
<?php

monta_titulo( "PPA (2012-2015)", "");

$filtro_agrupador = (($_REQUEST['ordemgrupo'])?$_REQUEST['ordemgrupo']:"P");
//		dbg ($filtro_agrupador,1);
switch($filtro_agrupador) {
	
	case 'P':
		$sql = "SELECT '<img src=\"../imagens/mais.gif\" id=\"programa_'||p.prgcod||'\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"carregarObjetivos(\''||p.prgcod||'\',this);\">' as mais, 
					   p.prgcod as codigo, 
					   p.prgcod || ' - ' || p.prgdsc as descricao,
					   count(acaid) as qtdloc ,
					   count(distinct acacod) as qtdacao
				from planejamento.programa p 
				inner join monitora.acao a ON a.prgid = p.prgid 
				WHERE p.prgano='".$_SESSION['exercicio']."' and acasnrap = false and trim(prodsc) != '' and p.prgcod not in ('1073','1449') 
				group by p.prgcod, p.prgdsc 
				ORDER BY p.prgcod, p.prgdsc";
		

		break;
		
	case 'A':
		
		$sql = "SELECT 
					'<img src=\"../imagens/mais.gif\" id=\"acao_'||a.acacod||'\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"carregarAcoesDetalhes(\''||a.acacod||'\', \'\', \'\', this);\">' as mais, 
				   	a.acacod as codigo, 
				   	a.acacod || ' - ' || a.acatitulo as descricao,
				   	count(acaid) as qtdloc ,
				   	count(distinct acacod) as qtdacao
				FROM 
					planejamento.programa p 
				INNER JOIN monitora.acao a ON a.prgid = p.prgid 
				WHERE p.prgano='".$_SESSION['exercicio']."' and acasnrap = false and trim(prodsc) != '' and p.prgcod not in ('1073','1449') 
				GROUP BY a.acacod, a.acatitulo 
				ORDER BY a.acacod, a.acatitulo";
		
		break;
		
	case 'U':
		
		$sql = "SELECT '<img src=\"../imagens/mais.gif\" id=\"unidade_'||u.unicod||'\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"carregarAcoesUnidade(\''||u.unicod||'\',this);\">' as mais, 
					   u.unicod as codigo, 
					   u.unicod || ' - ' || u.unidsc as descricao,
					   count(acaid) as qtdloc ,
					   count(distinct acacod) as qtdacao
				from planejamento.programa p 
				inner join monitora.acao a ON a.prgid = p.prgid 
				inner join public.unidade u ON u.unicod = a.unicod
				WHERE p.prgano='".$_SESSION['exercicio']."' and acasnrap = false and trim(prodsc) != '' and p.prgcod not in ('1073','1449') 
				group by u.unicod, u.unidsc 
				ORDER BY u.unicod, u.unidsc";
		
		break;
		
	case 'L':
		
		$sql = "SELECT '<img src=\"../imagens/mais.gif\" id=\"localizador_'||l.loccod||'\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"carregarAcoesLocalizador(\''||l.loccod||'\',this);\">' as mais, 
					   l.loccod as codigo, 
					   l.loccod || ' - ' || l.locdsc as descricao,
					   count(acaid) as qtdloc ,
					   count(distinct acacod) as qtdacao
				from planejamento.programa p 
				inner join monitora.acao a ON a.prgid = p.prgid 
				inner join public.localizador l ON l.loccod = a.loccod
				WHERE p.prgano='".$_SESSION['exercicio']."' and acasnrap = false and trim(prodsc) != '' and p.prgcod not in ('1073','1449') 
				group by l.loccod, l.locdsc 
				ORDER BY l.loccod, l.locdsc";
		
		break;
	
}

$dados = $db->carregar($sql);

?>
<form method="post" name="filtro">
	<input type="hidden" name="filtro" value="1"/>
	<table width="95%" border="0" cellspacing="0" cellpadding="2" align="center" bgcolor="#f7f7f7" style="border-top: 1px solid #c0c0c0;">
		<tr>
			<td class="SubTituloDireita">Agrupamento:</td>
			<td style="color:#800000;">
				<input type="radio" name="ordemgrupo" id="ordemgrupo_P" value="P" <?= $filtro_agrupador == 'P' ? 'checked="checked"' : '' ?>>
				<label for="ordemgrupo_P">Programa</label>
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
<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem" style="color:333333;">
<tr><td class="SubTituloCentro" width="2%">&nbsp;</td><td class="SubTituloCentro">Programas</td><td class="SubTituloCentro">Qtd. Loc.</td><td class="SubTituloCentro">Qtd. Ações</td></tr>
			<? if($dados[0]) : ?>
			<? foreach($dados as $key => $d) : ?>
			<? if($key%2==0) {$cor="#F7F7F7";} else {$cor="";}	?>
			<tr bgcolor="<?=$cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$cor ?>';">
				<td width="2%"><?=$d['mais'] ?></td>
				<td>
					<a href="monitora.php?modulo=principal/detalhesppa&acao=P&prgcod=<?=$d['codigo'] ?>"><?=$d['descricao'] ?></a>
				</td>
				<td align="right"><?=$d['qtdloc'] ?></td>
				<td align="right"><?=$d['qtdacao'] ?></td>
			</tr>
			<? endforeach; ?>
			<? endif; ?>
</table>

<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>

<script>
/*** INICIO SHOW MODAL ***/

var countModal = 1;

function montaShowModal() {
	var campoTextArea = ''+
		'<p align=\'justify\'>Informamos que o Módulo PPA – Monitoramento e Avaliação está sendo reformulado.'+
		'<br /><br />A partir de 2013, o acompanhamento das ações orçamentárias será viabilizado por meio do módulo <b> <font color=red>SPO - Planejamento e Acompanhamento Orçamentário </font></b>.'+
		'<br /><br />O Sistema está fuincionando apenas para <b>Consultas</b></p> <br/><br/>';
	var alertaDisplay = '<div class="titulo_box" >Prezados,<br/ >'+campoTextArea+'</div><div class="links_box" ><center><input type="button" onclick=\'closeMessage(); return false \' value="Fechar" /></center>';
	displayStaticMessage(alertaDisplay,false);
	return false;
}

function displayStaticMessage(messageContent,cssClass) {
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	messageObj.setHtmlContent(messageContent);
	messageObj.setSize(520,180);
	messageObj.setCssClassMessageBox(cssClass);
	messageObj.setSource(false);	// no html source since we want to use a static message here.
	messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
	messageObj.display();
}

function closeMessage() {
	messageObj.close();	
}

montaShowModal();

</script>
