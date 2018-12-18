<?php

if( $_REQUEST['requisicao'] == 'carregaremail' ){
	$usuemail = $db->pegaUm("select usuemail from seguranca.usuario where usucpf = '{$_REQUEST['usucpf']}'");
	echo $usuemail;
	exit();
}
if( $_REQUEST['requisicao'] == 'salvar' ){
	$_POST['purpastaorigem'] = str_ireplace('\\\\', '\\', $_POST['purpastaorigem']);
	$_POST['purpastaorigem'] = str_ireplace('\\', '/', $_POST['purpastaorigem']);
	$_POST['purpastadestino'] = str_ireplace('\\\\', '\\', $_POST['purpastadestino']);
	$_POST['purpastadestino'] = str_ireplace('\\', '/', $_POST['purpastadestino']);
	extract($_POST);
	
	if( $purid ){
		$sql = "UPDATE seguranca.publicacaoresponsavel SET 
				  usucpf = '$usucpf',
				  puremail = '$puremail',
				  purpastaorigem = '$purpastaorigem',
				  purpastadestino = '$purpastadestino'				 
				WHERE 
				  purid = $purid";
		$db->executar($sql);
	} else {
		$sql = "INSERT INTO seguranca.publicacaoresponsavel(usucpf, puremail, purpastaorigem, purpastadestino) 
				VALUES ('$usucpf', '$puremail', '$purpastaorigem', '$purpastadestino')";
		$db->executar($sql);
	}
	$db->commit();
	$db->sucesso('sistema/tabapoio/responsavelPublicar');
	exit();
}

if( $_REQUEST['requisicao'] == 'alterar' ){
	$sql = "SELECT purid, usucpf, puremail, purpastaorigem, purpastadestino
			FROM seguranca.publicacaoresponsavel WHERE purid = ".$_REQUEST['purid'];
	$arPublicador = $db->pegaLinha($sql);
	extract($arPublicador);
}

if( $_REQUEST['requisicao'] == 'excluir' ){
	$sql = "UPDATE seguranca.publicacaoresponsavel SET purstatus = 'I' WHERE purid = ".$_REQUEST['purid'];
	$db->executar($sql);
	$db->commit();
	$db->sucesso('sistema/tabapoio/responsavelPublicar');
	exit();
}

include  APPRAIZ."includes/cabecalho.inc";
?>
<form id="formulario" name="formulario" action="#" method="post" enctype="multipart/form-data" >
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="purid" id="purid" value="<?=$purid; ?>">
	
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="subtitulocentro" colspan="2">Responsável Pela Publicação de Arquivos(SVN)</td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="30%">Publicador:</td>
			<td width="70%"><?
			$sql = "SELECT DISTINCT
				    u.usucpf as codigo,
				    u.usunome as descricao
				FROM
				    seguranca.usuario u
				    inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
				    inner join seguranca.perfilusuario pu on pu.usucpf = u.usucpf
				    inner join seguranca.perfil per on per.pflcod = pu.pflcod
				WHERE
				    us.sisid = 1
				    and per.pfldsc ilike '%super%'
				    and per.sisid = 1
				    order by u.usunome";
			echo $db->monta_combo("usucpf", $sql, 'S','-- Selecione o Publicador --','carregarEmail', '', '',350,'S','usucpf', false, $usucpf, 'Publicador');?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Email:</td>
			<td><?
				echo campo_texto( 'puremail', 'S', 'S', 'Email', 90, 500, '', '','','','','id="puremail"'); ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Pasta SVN Desenvolvimento:</td>
			<td><?
				echo campo_texto( 'purpastaorigem', 'S', 'S', 'Pasta SVN Desenvolvimento', 90, 500, '', '','','','','id="purpastaorigem"'); ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Pasta SVN Produção:</td>
			<td><?
				echo campo_texto( 'purpastadestino', 'S', 'S', 'Pasta SVN Produção', 90, 500, '', '','','','','id="purpastadestino"'); ?></td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td colspan="2" align="center">
				<input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" style="cursor: pointer;" onclick="salvarPublicador();">
			</td>
		</tr>
	</table>
</form>
<?
$sql = "SELECT 
			'<center>
    			<img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\" style=\"cursor:pointer;\" onclick=\"alterarPublicador('|| pr.purid ||');\">&nbsp;
    			<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"excluirPublicador('|| pr.purid ||');\">
    		</center>' as acao,
			pr.usucpf,
			usu.usunome,
			pr.puremail,
			pr.purpastaorigem,
			pr.purpastadestino
		FROM
			seguranca.publicacaoresponsavel pr
			inner join seguranca.usuario usu on usu.usucpf = pr.usucpf
		WHERE
			purstatus = 'A'
		order by usunome";
?>
<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="subtitulocentro">Lista de Publicador</td>
	</tr>
	<tr>
		<td>
	<?
		$cabecalho = array( "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Ações&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;", "CPF", "Publicador", "Email", "Pasta Desenvolvimento", "Pasta Produção");
		/*$db->monta_lista_simples($sql, $cabecalho, 100000, 1, 'N', '100%', 'N', false, false, false, true);*/
		?></td>
	</tr>
</table>
<script type="text/javascript">
function salvarPublicador(){

	var erro = false;
	jQuery('.obrigatorio').each(function(){
		if(	jQuery(this).val() == '' || jQuery(this).val() == '0' ){
			alert('O campo '+jQuery(this).attr('title')+' é de preenchimento obrigatório');
			var nome = jQuery(this).attr('name');
			jQuery('[name="'+nome+'"]').focus();
			erro = true;			
			return false;
		}
	});
	
	if( erro ){
		return false;
	}

	jQuery('#requisicao').val('salvar');
	jQuery('#formulario').submit();
}

function carregarEmail(usucpf){
	jQuery.ajax({
		type: "POST",
		url: "seguranca.php?modulo=sistema/tabapoio/responsavelPublicar&acao=A",
		data: "requisicao=carregaremail&usucpf="+usucpf,
		async: false,
		success: function(msg){
			jQuery('[name="puremail"]').val(msg);
		}
	});
}

function alterarPublicador(purid){
	window.location.href = 'seguranca.php?modulo=sistema/tabapoio/responsavelPublicar&acao=A&requisicao=alterar&purid='+purid;
}
function excluirPublicador(purid){
	window.location.href = 'seguranca.php?modulo=sistema/tabapoio/responsavelPublicar&acao=A&requisicao=excluir&purid='+purid;
}
</script>