<?php
//Função para montar os tipos das Caixas
function exibeTipos($pesquisa = false){
		global $db;
		
		$pesquisa != 'false' ? $where = " where tcostatus = 'A' and (tcodescricao ilike '%$pesquisa%' or tconome ilike '%$pesquisa%') " : $where = " where tcostatus = 'A' ";
		$sql = "select * from painel.tipoconteudo $where";
		$tipoCaixa = $db->carregar($sql);
		!$tipoCaixa ? $tipoCaixa = array() : $tipoCaixa = $tipoCaixa; ?>
		<table style="margin-top: 10px" >
		<?php foreach($tipoCaixa as $key =>$tpCx):
		
		$tpCx['tcoimagem'] = !$tpCx['tcoimagem'] || $tpCx['tcoimagem'] == "gadget_padrao.png" ? "" : $tpCx['tcoimagem'];

		if(!file_exists("../imagens/".$tpCx['tcoimagem'])){
			$tpCx['tcoimagem'] = "gadget_padrao.png";
		}
		
		?>
		<tr style="margin-top: 10px" >
			<td valign="top" ><img style="cursor: pointer;border: #000000 solid 1px;" onclick="inserirCaixa(<?=$tpCx['tcoid'];?>,'<?=$tpCx['tconome'];?>');" src="../imagens/<?=$tpCx['tcoimagem'];?>" ></td>
			<td valign="top" ><b><?=$tpCx['tconome'];?></b><br /><?=$tpCx['tcodescricao'];?><br /><div style="width: 150px;display: none;background-color: #FFFACD" id="check_<?=$tpCx['tcoid'];?>" ><img src="../imagens/check.jpg" />Selecionado com sucesso!</div></td>
		</tr>
		<?php if($key != (count($tipoCaixa) -1)): ?>
		<tr><td colspan="2" ><hr></td></tr>
		<?php endif; ?>
		<?php endforeach;
		if(!$tipoCaixa): ?>
			<tr><td colspan="2" >Não existem tipos cadastrados.</td></tr>
		<?php endif; ?>
		</table>
<?php }

function montaCaixa($cxccoluna,$usucpf,$abaid){
		global $db;
		$sql = "select ctuid from painel.conteudousuario where abaid = $abaid and usucpf = '$usucpf'";
		$ctuid = $db->pegaUm($sql);
		$sql = "select * from painel.caixaconteudo where ctuid = $ctuid and cxccoluna = $cxccoluna and cxcstatus = 'A' order by cxclinha";
		$cx_conteudo = $db->carregar($sql);
		
		$cx_conteudo = !$cx_conteudo ? array() : $cx_conteudo;
		
		foreach($cx_conteudo as $cxc):
				
			//Estado da Coluna (Maximizada / Minimizada)
			($cxc['cxcaberta'] == 't')? $estado_cxcid = "max" : $estado_cxcid = "min";
			($estado_cxcid == 'max')? $style_est = "" : $style_est = "none";
			($estado_cxcid == 'max')? $src_img_est = "../imagens/menos.gif" : $src_img_est = "../imagens/mais.gif";
			($cxc['cxcrolagem'] == 'f')? $scroll_cxcid = "false" : $scroll_cxcid = "true";?>
			<div id="colid_<?= $cxc['cxcid'] ?>" class="groupItem">
				<input type="hidden" id="estado_<?= $cxc['cxcid'] ?>" name="estado_<?= $cxc['cxcid'] ?>" value="<?=$estado_cxcid?>"  />
				<input type="hidden" id="scroll_<?= $cxc['cxcid'] ?>" name="scroll_<?= $cxc['cxcid'] ?>" value="<?=$scroll_cxcid?>"  />
				<div style="-moz-user-select: none;" class="itemHeader">
					<div id="titulo_<?=$cxc['cxcid']?>" class="titulo_coluna" > <img style="cursor:pointer" onclick="editarNomecaixa('<?=$cxc['cxcid']?>')"  src="../imagens/editar_nome.gif" border=0 /> <?=$cxc['cxcdescricao']?></div>
					<a href="#" class="closeEl" id="<?=$cxc['cxcid']?>" ><img src="<?=$src_img_est ?>" border=0 /></a>
					<a href="#" class="excluir" ><img onclick="excluirBox('<?=$cxc['cxcid']?>')"  src="../imagens/excluir_2.gif" border=0 /></a>
					<div id="edit_<?=$cxc['cxcid']?>"  class="edit" style="margin-right:8px" >
						<img onclick="exibeEditar('<?=$cxc['cxcid']?>')" src="../imagens/ico_config.gif" border=1 />
					</div>
				</div>
				<div style="display:none"  id="editar_<?=$cxc['cxcid']?>" class="editar" >
					<div style="padding: 3px 3px 3px 3px;" >
						<input type="checkbox" id="autoscroll_<?=$cxc['cxcid']?>" name="autoscroll" value="<?=$cxc['cxcid']?>" onclick="executarEditar('<?=$cxc['cxcid']?>',false)" /> Rolagem
						<span style="cursor:pointer;margin-left:10px;" onclick="editarNomecaixa(<?=$cxc['cxcid']?>)" ><img src="../imagens/editar_nome.gif" /> Editar Nome </span>
						<span style="cursor:pointer;margin-left:10px;" onclick="editarConteudoCaixa(<?=$cxc['cxcid']?>,<?=$cxc['tcoid']?>)" > <img src="../imagens/editar_conteudo_caixa.png" /> Editar Conteudo</span> 
					</div>
				</div>
				<div class="itemContent" style="display:<?=$style_est ?>" id="itemContent_<?=$cxc['cxcid']?>"  >
					<?php if($scroll_cxcid == "true"){ ?>
						<script>
							document.getElementById( 'autoscroll_<?=$cxc['cxcid']?>').checked = true;
							executarEditar(<?=$cxc['cxcid']?>,true);
							document.getElementById( 'itemContent_<?=$cxc['cxcid']?>').style.display = '<?=$style_est?>';
						</script>
					<?php } ?>
					<?php exibeConteudo($cxc['cxcid'],$cxc['tcoid']); ?>
				</div>
			</div>
		<?php endforeach;
}


//Função para montar as Caixas Drag and Drop no Painel de Controle
function addCaixa(){
	global $db;
	?>
<table class="tabela" width="95%" bgcolor="FFFFFF" cellSpacing="1" border=0 cellPadding="3" align="center">
	<tr>
		<td><input type="hidden" name="tcoid" id="tcoid" /></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan="2" style="text-align: center;font-weight: bold" >Tipo da Caixa</td>
	</tr>
	<tr>
		<td colspan="2" >
		
		Pesquisar Tipos: <input class="normal" type="text" id="pesquisa" onkeypress="pesquisaTipos();" onfocus="this.value=''" onblur="if(this.value.length == 0){this.value = 'Digite o tipo'}"  value="Digite o tipo" />
		 <input type="button" style="display:none" id="buttonListaTodosTipos" onclick="listaTipos();" value="Listar Todos" />
		<br/>
		<div id="TipoCaixa">
		<?php exibeTipos(); ?>
		</div>
		</td>
	</tr>
	<tr>
		<td colspan="2" class="SubTituloDireita" style="text-align: center;" ><input type="button" name="cancelar" value="Cancelar" id="button_cancelar" /></td>
	</tr>
</table>	
<?php }

function index($abaid){
	global $db;
	
	if(!$abaid){
		return false;
	}
	
	$sql = "	select 
						ctu.ctuid 
					from 
						painel.conteudousuario ctu
					inner join
						painel.aba aba ON aba.abaid = ctu.abaid 
					where 
						aba.abaid = $abaid 
					and 
						usucpf = '{$_SESSION['usucpf']}'";
	$ctuid = $db->pegaUm($sql);
	
	if(!$ctuid){
		echo "<center>Não existem caixas de conteúdo associadas.</center>";
	}
	elseif($ctuid){
		$sql = "select count(cxcid) from painel.caixaconteudo where ctuid = $ctuid and cxcstatus = 'A'";
		$cxs = $db->pegaUm($sql);
		if(!$cxs)
			echo "<center>Não existem caixas de conteúdo associadas.</center>";
		else{
	?>
		<!-- Início da Coluna 1-->
		<div id="sort1" class="groupWrapper">
		<?php
		$sql = "select count(cxcid) from painel.caixaconteudo where ctuid = $ctuid and cxccoluna = 1 and cxcstatus = 'A'";
		$coluna1 = $db->pegaUm($sql);
		if($coluna1)
		montaCaixa(1,$_SESSION['usucpf'],$abaid)
		?>
		<!-- Fim da Coluna 1-->
		</div>
		
		<!-- Início da Coluna 2-->
		<div id="sort2" class="groupWrapper">
		<?php
		$sql = "select count(cxcid) from painel.caixaconteudo where ctuid = $ctuid and cxccoluna = 2 and cxcstatus = 'A'";
		$coluna2 = $db->pegaUm($sql);
		if($coluna2)
		montaCaixa(2,$_SESSION['usucpf'],$abaid)
		?>
		<!-- Fim da Coluna 2-->
		</div>
		
		<!-- Início da Coluna 3-->
		<div id="sort3" class="groupWrapper">
		<?php
		$sql = "select count(cxcid) from painel.caixaconteudo where ctuid = $ctuid and cxccoluna = 3 and cxcstatus = 'A'";
		$coluna3 = $db->pegaUm($sql);
		if($coluna3)
		montaCaixa(3,$_SESSION['usucpf'],$abaid)
		?>
		<!-- Fim da Coluna 3-->
		</div>	
		<!-- Fim do Drag and Drop  das Caixas-->
<?php		}
	}
}

//Função que monta as abas do painel de controle do usúario
function exibeAbaUsuario($usucpf,$abaDefault,$tipoPainel = null){
	global $db;
	
	//Verifica se está no Painel 1 ou 2
	$tipoPainel = !$tipoPainel || $tipoPainel == 1 ? "" : 2; 
	
	//Se não existir CPF retorna falso
	if(!usucpf){
		return false;
	}
	
	// Verifica se existe um conjunto de abas criado
	$sql = "select 
				aba.abaid,
				aba.abadsc
			from
				painel.aba as aba
			inner join
				painel.conteudousuario ctu ON ctu.abaid = aba.abaid
			where
				ctu.usucpf = '{$usucpf}'
			and
				aba.abastatus = 'A'
			order by
				aba.abaid";
	
	//dbg($sql);
	$abas = $db->carregar($sql);
	//dbg($abas);
	//die;		
	if(!$abas){
		$abaid = addNovaAba("Painel de Controle",true);
		
		/* INÍCIO - Adiciona novas caixas default */
		executaAddCaixa("Árvore de Indicadores",10,$abaid);
		/* FIM - Adiciona novas caixas default */
		
		echo "<script>window.location.href='painel$tipoPainel.php?modulo=principal/painel_controle&acao=A&abaid={$abaid}'</script>";
		
		$menu[0] = array("descricao" => "Painel de Controle", "link"=> "/painel/painel$tipoPainel.php?modulo=principal/painel_controle&acao=A");
	}else{
		$n = 0;
		foreach($abas as $aba){
			$menu[] = array("descricao" => $aba['abadsc'], "link"=> "/painel/painel$tipoPainel.php?modulo=principal/painel_controle&acao=A&abaid={$aba['abaid']}");
			if($n == 0)
				$abaid = $aba['abaid'];
			$n++;
		}
	}
	//Adiciona Nova Aba
	//array_unshift($menu,array("descricao" => "Rede Federal", "link"=> "/painel/painel$tipoPainel.php?modulo=principal/redefederal/rede_federal&acao=A"));
	//array_unshift($menu,array("descricao" => "Mapa de Obras", "link"=> "/painel/painel$tipoPainel.php?modulo=principal/mapa_obras&acao=A"));
	array_unshift($menu,array("descricao" => "Indicadores", "link"=> "/painel/painel$tipoPainel.php?modulo=principal/painel_controle&acao=A"));
	array_push($menu,array("descricao" => "Adicionar Aba [+]", "link"=> "javascript:exibeAddNovaAba()"));
	
	
	if(!$abaDefault){
		echo "<script>window.location.href='painel$tipoPainel.php?modulo=principal/painel_controle&acao=A&abaid={$abaid}'</script>";
	}else{
		if(!$tipoPainel || $tipoPainel == 1)
			echo montarAbasPainel($menu, $_SERVER['REQUEST_URI']);
	}	
}


//Função que monta as abas do Painel de Controle
function montarAbasPainel($itensMenu, $url = false)
{
    $url = $url ? $url : $_SERVER['REQUEST_URI'];

    if (is_array($itensMenu)) {
        $rs = $itensMenu;
    } else {
        global $db;
        $rs = $db->carregar($itensMenu);
    }

    $menu    = '<table width="95%" border="0" cellspacing="0" cellpadding="0" align="center" class="notprint">'
             . '<tr>'
             . '<td>'
             . '<table cellpadding="0" cellspacing="0" align="left">'
             . '<tr>';

    $nlinhas = count($rs) - 1;

    for ($j = 0; $j <= $nlinhas; $j++) {
        extract($rs[$j]);

        if ($url != $link && $j == 0)
            $gifaba = 'aba_nosel_ini.gif';
        elseif ($url == $link && $j == 0)
            $gifaba = 'aba_esq_sel_ini.gif';
        elseif ($gifaba == 'aba_esq_sel_ini.gif' || $gifaba == 'aba_esq_sel.gif')
            $gifaba = 'aba_dir_sel.gif';
        elseif ($url != $link)
            $gifaba = 'aba_nosel.gif';
        elseif ($url == $link)
            $gifaba = 'aba_esq_sel.gif';

        if ($url == $link) {
            $giffundo_aba = 'aba_fundo_sel.gif';
            $cor_fonteaba = '#000055';
            $id = ' id="exibe_edita_aba" ';
            $evento = " ";
        } else {
            $giffundo_aba = 'aba_fundo_nosel.gif';
            $cor_fonteaba = '#4488cc';
            $id = '';
            $evento = "window.location.href='".$link."'";
        }

        $menu .= '<td height="20" valign="top"><img src="../imagens/'.$gifaba.'" width="11" height="20" alt="" border="0"></td>'
               . '<td height="20" align="center" valign="middle" background="../imagens/'.$giffundo_aba.'" style="color:'.$cor_fonteaba.'; padding-left: 10px; padding-right: 10px;cursor:pointer;">';

        
               
        if ($link != $url) {
        	if($link == "javascript:exibeAddNovaAba()"){
	            $menu .= '<span id="exibe_add_aba" onclick="'.$evento.'" >'.$descricao.'</span>';
	            $menu .= '<div id="exibe_escolhe_add_aba" style="display:none;position:absolute;width:115px;height:46px;background-color:#fcfcfc;border:solid 1px #C9C9C9;padding:2px;text-align:left;z-index:1000" >
	            			<div onclick="addNovaAba();" style="cursor:pointer;" ><img style="width:16px;height:16px;" src="../imagens/editar_nome.gif" /> Nova Aba</div> 
	            			<div onclick="addAbaCompartilhada();" style="border-top:solid 1px #C9C9C9;cursor:pointer;position:absolute;z-index:10;width:100%" ><img style="width:13px;height:16px;padding-top:3px;" src="../imagens/grupo.gif" /> Aba Compartilhada</div>
	            			</div></a></li>';
        	}else{
        		$menu .= '<span '.$id.'  onclick="'.$evento.'" >'.$descricao.'</span></li>';
        	}
        } else {
        	if($link == "/painel/painel.php?modulo=principal/painel_controle&acao=A&abaid=paginaInicial" || $link == "/painel/painel.php?modulo=principal/painel_controle&acao=A" || $link == "/painel/painel.php?modulo=principal/mapa_obras&acao=A" || $link == "/painel/painel.php?modulo=principal/redefederal/rede_federal&acao=A"){
        		$menu .= '<span '.$id.' >'.$descricao.' </span>';
            	$menu .= '</td>';
        	}else{
        		$menu .= '<span '.$id.'  onclick="'.$evento.'" >'.$descricao.' <img src="../imagens/seta_baixo.png" > </span>';
            	$menu .= '<div id="abaid_'.str_replace("abaid=","",strstr($link, 'abaid=')).'" style="display:none;position:absolute;width:110px;height:62px;background-color:#fcfcfc;border:solid 1px #C9C9C9;padding:2px;text-align:left;z-index:1000" >
            			<div onclick="alterarNomeAba('.str_replace("abaid=","",strstr($link, 'abaid=')).',\''.$descricao.'\');" style="cursor:pointer;" ><img style="width:16px;height:16px;" src="../imagens/editar_nome.gif" /> Editar Nome</div> 
            			<div onclick="compartilharAba('.str_replace("abaid=","",strstr($link, 'abaid=')).');" style="border-bottom:solid 1px #C9C9C9;border-top:solid 1px #C9C9C9;cursor:pointer;position:absolute;z-index:10;width:100%" ><img style="width:13px;height:16px;padding-top:3px;" src="../imagens/grupo.gif" /> Compartilhar</div>
            			<div onclick="excluirAba('.str_replace("abaid=","",strstr($link, 'abaid=')).');" style="cursor:pointer;" ><img style="width:13px;height:16px;margin-top:26px;clear:both" src="../imagens/excluir.gif" /> Excluir Aba</div>
            			</div>';
            	$menu .= '</td>';
        	}
        	
        }
    }

    if ($gifaba == 'aba_esq_sel_ini.gif' || $gifaba == 'aba_esq_sel.gif')
        $gifaba = 'aba_dir_sel_fim.gif';
    else
        $gifaba = 'aba_nosel_fim.gif';

    $menu .= '<td height="20" valign="top"><img src="../imagens/'.$gifaba.'" width="11" height="20" alt="" border="0"></td></tr></table></td></tr></table>';

    return $menu;
}



//Função que adiciona uma nova aba;
function addNovaAba($abadsc,$return = false){
	global $db;
	
	$sql = "insert into painel.aba (abadsc,abastatus) values ('$abadsc','A') returning abaid";
	$abaid = $db->pegaUm($sql);
	$sql = "insert into painel.conteudousuario (usucpf,abaid) values ('{$_SESSION['usucpf']}',$abaid) returning ctuid";
	$ctuid = $db->pegaUm($sql);
	
	//inicio
	//Seleciona todas as caixas que não possuem abas vinculadas
	$sql = "select 
				cxcid 
			from 
				painel.caixaconteudo cxc
			inner join
				painel.conteudousuario ctu ON cxc.ctuid = ctu.ctuid
			where
				ctu.abaid is null
			and
				ctu.usucpf = '{$_SESSION['usucpf']}'
			and
				cxc.cxcstatus = 'A'";
	$cxcs = $db->carregar($sql);
	if($cxcs){
		foreach($cxcs as $cxc){
			$cx[] = $cxc['cxcid'];
		}
		//Inclui todas as caixas sem aba na aba 'Painel de Controle'
		$sql = "update painel.caixaconteudo set ctuid = $ctuid where cxcid in (".implode(",",$cx).")";
		$db->executar($sql);
	}
	//fim
	
	//Adiciona uma nova caixa de busca
	$cxcid = executaAddCaixa(utf8_decode("Busca de Indicadores"),9,$abaid);
	
	$db->commit();
	
	if($return){
		return $abaid; 
	}else{
		echo $abaid;
	}
	
}


//Função que executa o SQL p/ Add a Caixa
function executaAddCaixa($cxcdescricao,$tcoid,$abaid){
	global $db;
	
	//Atualiza a data de modificação da data
	$sql = "update painel.aba set abadata = now() where abaid = $abaid";
	$db->executar($sql);
		
	//Verifica se já existe registro do usuário
	$sql = "select ctuid from painel.conteudousuario where abaid = $abaid and usucpf = '{$_SESSION['usucpf']}'";
	$ctuid = $db->pegaUm($sql);
	//Insere na tabela caso ñ exista
	if(!$ctuid){
		$sql = "insert into painel.conteudousuario (usucpf,abaid) values ('{$_SESSION['usucpf']}',$abaid) returning ctuid";
		$ctuid = $db->pegaUm($sql);
	}
	//Verifica qual das 3 colunas possui menos Caixas p/ Inserir
	$sql = "select count(cxcid) from painel.caixaconteudo where ctuid = $ctuid and cxccoluna = 1 and cxcstatus = 'A'";
	$coluna[1] = (int)$db->pegaUm($sql);
	$sql = "select count(cxcid) from painel.caixaconteudo where ctuid = $ctuid and cxccoluna = 2 and cxcstatus = 'A'";
	$coluna[2] = (int)$db->pegaUm($sql);
	$sql = "select count(cxcid) from painel.caixaconteudo where ctuid = $ctuid and cxccoluna = 3 and cxcstatus = 'A'";
	$coluna[3] = (int)$db->pegaUm($sql);

	$coluna = array_unique($coluna);
	
	foreach($coluna as $key =>$col){
		if($col == min($coluna))
			$cxccoluna = $key;
	}
	
	$soma = array_sum($coluna);
	
	($soma == 0) ? $cxccoluna = 1 : $cxccoluna = $cxccoluna; 
	$cxccoluna == 0 ? $cxccoluna = 1 : $cxccoluna = $cxccoluna;
		
	//Pega a última cxclinha inserida e incrementa 1;
	$sql = "select max(cxclinha) from painel.caixaconteudo where ctuid = $ctuid and cxccoluna = $cxccoluna and cxcstatus = 'A'";
	$cxclinha = $db->pegaUm($sql);
	
	if($cxclinha == '')
		$cxclinha = 0;
	elseif($cxclinha == '0')
		$cxclinha = 1;
	else
		$cxclinha = $cxclinha + 1;
		
	//Insere a nova Caixa retornando o ID para realizar o focus
	$sql = "insert into painel.caixaconteudo 
	(tcoid,ctuid,cxclinha,cxccoluna,cxcaberta,cxcstatus,cxcrolagem,cxcdescricao) values
	($tcoid,$ctuid,$cxclinha,$cxccoluna,true,'A',false,'$cxcdescricao') returning cxcid";
	$cxcid = $db->pegaUm($sql);
	
	$db->commit();
	
	return $cxcid;
}


function salvaParametros($arrSor1,$arrSor2,$arrSor3,$estados,$scroll,$usucpf){
	global $db;
	
//	dbg($arrSor1);
//	dbg($arrSor2);
//	dbg($arrSor3);
	
	$estados = explode(',',$estados);
	$scroll = explode(',',$scroll);
	
	//dbg($estados);
	//dbg($scroll);
	
	$key = 0;
	if($arrSor1){
		foreach($arrSor1 as $cxclinha => $cx){
			
			$cxcid      = trim(str_replace('colid_','',$cx));
			$cxcaberta  = (($estados[$key] == 'max')? 'TRUE' : 'FALSE');
			$cxcrolagem = (($scroll[$key] == 'true')? 'TRUE' : 'FALSE');
	
			$sql .= "update painel.caixaconteudo set
					cxclinha = $cxclinha,
					cxccoluna = 1,
					cxcaberta = $cxcaberta,
					cxcrolagem = $cxcrolagem
				where cxcid = $cxcid;";
			$key++;
		}
	}
	if($arrSor2){
		foreach($arrSor2 as $cxclinha => $cx){
			
			$cxcid      = trim(str_replace('colid_','',$cx));
			$cxcaberta  = (($estados[$key] == 'max')? 'TRUE' : 'FALSE');
			$cxcrolagem = (($scroll[$key] == 'true')? 'TRUE' : 'FALSE');
	
			$sql .= "update painel.caixaconteudo set
					cxclinha = $cxclinha,
					cxccoluna = 2,
					cxcaberta = $cxcaberta,
					cxcrolagem = $cxcrolagem
				where cxcid = $cxcid;";
			$key++;
		}
	}
	if($arrSor3){
		foreach($arrSor3 as $cxclinha => $cx){
			
			$cxcid      = trim(str_replace('colid_','',$cx));
			$cxcaberta  = (($estados[$key] == 'max')? 'TRUE' : 'FALSE');
			$cxcrolagem = (($scroll[$key] == 'true')? 'TRUE' : 'FALSE');
	
			$sql .= "update painel.caixaconteudo set
					cxclinha = $cxclinha,
					cxccoluna = 3,
					cxcaberta = $cxcaberta,
					cxcrolagem = $cxcrolagem
				where cxcid = $cxcid;";
			
			$key++;
		}
	}
	
	$db->executar($sql);
	$db->commit();
}

function exibeButtons($abaid){
	global $db;
	if(!$abaid || !is_numeric($abaid)){
		return false;
	}
	$sql = "select ctuid from painel.conteudousuario where abaid = $abaid and usucpf = '{$_SESSION['usucpf']}'";
	$ctuid = $db->pegaUm($sql);
	
	if(!$ctuid)
		return 'none';
	elseif($ctuid){
		$sql = "select count(cxcid) from painel.caixaconteudo where ctuid = $ctuid and cxcstatus = 'A'";
		$cxs = $db->pegaUm($sql);
		if(!$cxs)
			return 'none';
		else
			return 'block';
	}
}

function exibeMinhaListaIndicadores($sql,$cxcid = null,$tipoConteudo = null){
	global $db;
			$res = $db->carregar($sql);
			$res = !$res ? array() : $res;
	
			foreach($res as $r){
				$arrIndid[] = $r['indid'];
			}
			
			!$arrIndid? $arrIndid = array() : $arrIndid = $arrIndid;
			
			if(count($arrIndid) != 0){
				
				//Lista as Ações Estratégicas dos Indicadores
				$sql = "select 
							acaid,
							acadsc 
						from 
							painel.acao
						where
							acaid in 	(
										select 
											distinct acaid
										from 
											painel.indicador
										where 
											indid in (".implode(",",$arrIndid).")
										)
						order by
							acadsc";
				$acoes = $db->carregar($sql);	
			}
			
			if(count($acoes) != 0){
				
				echo "<table cellSpacing=0 cellPadding=3 style=\"width:100%\">";
//				echo "<tr bgcolor='#D9D9D9' ><td style=\"font-weight:bold\" >&nbsp;&nbsp;Ações&nbsp;&nbsp;</td><td style=\"font-weight:bold;text-align:center\" >Cód.</td><td style=\"font-weight:bold\"  >Nome do Indicador</td><td style=\"font-weight:bold;text-align:right;\"  >Período</td><td style=\"font-weight:bold;text-align:right;\"  >Quantidade / Valor</td></tr>";
//				echo "</table>";
				### Lista as Ações - início
				foreach($acoes as $acao){
					echo "<tr><td style=\"width:100%;text-align:center;font-weight:bold;background-color:#c9c9c9;font-size:12px;color: rgb(0, 85, 0);\" colspan=6 >{$acao['acadsc']}</td></tr>";
					
					//Lista os novos indicadores atribuidos pelo usuário
					$sql = "select 
								ind.indid,ind.indnome,sec.secdsc 
							from 
								painel.indicador ind
							left join
								painel.secretaria sec ON ind.secid = sec.secid
							where 
								ind.indid in (".implode(",",$arrIndid).")
							and
								ind.acaid = {$acao['acaid']}";
					$indicadores = $db->carregar($sql);
					
					$indicadores = !$indicadores? array() : $indicadores;
					//echo "<table cellSpacing=0 cellPadding=3 style=\"width:100%\">";
					$i = 0;
					foreach($indicadores as $ind){
					
						################# Periodicidade ################# //Pega Periodicidade do Indicador
						$sql = "select 
									perdsc 
								from 
									painel.indicador i
								left join 
									painel.periodicidade p on i.perid = p.perid
								where 
									indid = {$ind['indid']}";
						$perdsc = $db->pegaUm($sql);
								
						################# Diária ################# 
						if($perdsc == "Diária"){ //Se a Periodicidade do Indicador for Diária, busca data e quantidade (sehdtcoleta / sehqtde) da tabela seriehistorica  
							$sql = "select 
									sehid,
									sehqtde,
									(to_char(sehdtcoleta, 'YYYY-MM-DD')) as data 
								from 
									painel.seriehistorica 
								where 
									indid = {$ind['indid']}
								and 
									dpeid is null
								and
									sehstatus <> 'I'
								order by
									data desc
								limit
									1";
						}
						################# Se não for Diária ################# 
						if($perdsc != "Diária"){ //Se a Periodicidade do Indicador não for Diária, busca data (dpedatainicio) da tabela detalheperiodicidade e quantidade (sehdtcoleta) da tabela seriehistorica
							$sql = "select
										seh.sehid,
										seh.sehqtde,
										(to_char(dpe.dpedatainicio, 'YYYY-MM-DD')) as data 
									from 
										painel.seriehistorica  seh
									inner join
										painel.detalheperiodicidade dpe ON seh.dpeid = dpe.dpeid
									where  
										indid = {$ind['indid']}
									and
										seh.dpeid is not null
									and 
										sehstatus <> 'I'
									order by
										data desc
									limit
										1";
						}
								
						$seh = $db->pegaLinha($sql);
										
						################# Comparação do penúltimo valor / data para indicar a seta de tendência (cima / baixo) ################# 
						$seta = "";  //variável seta inicia vazia;
						
						################# Veirifica se existe a Série Histórica p/ comparar #################
						if($seh['sehid']){ // se existe, busca novamente a série histórica, com excessão do último sehid
							
							################# Diária ################# 
							if($perdsc == "Diária"){ //Se a Periodicidade do Indicador for Diária, busca data e quantidade (sehdtcoleta / sehqtde) da tabela seriehistorica  
								$sql = "select 
									sehid,
									sehqtde,
									(to_char(sehdtcoleta, 'YYYY-MM-DD')) as data 
								from 
									painel.seriehistorica 
								where 
									indid = {$ind['indid']}
								and 
									dpeid is null
								and
									sehstatus <> 'I'
								and
									sehid != {$seh['sehid']}
								order by
									data desc
								limit
									1";
							}
							
							################# Se não for Diária ################# 
							if($perdsc != "Diária"){ //Se a Periodicidade do Indicador não for Diária, busca data (dpedatainicio) da tabela detalheperiodicidade e quantidade (sehdtcoleta) da tabela seriehistorica
								$sql = "select
										seh.sehid,
										seh.sehqtde,
										(to_char(dpe.dpedatainicio, 'YYYY-MM-DD')) as data 
									from 
										painel.seriehistorica  seh
									inner join
										painel.detalheperiodicidade dpe ON seh.dpeid = dpe.dpeid
									where  
										indid = {$ind['indid']}
									and
										seh.dpeid is not null
									and 
										sehstatus <> 'I'
									and
										sehid != {$seh['sehid']}
									order by
										data desc
									limit
										1";
							}
									
							$seh2 = $db->pegaLinha($sql);
							
							################# Veirifica qual o tipo de tendência do indicador (Maior melhor, Menor melhor) #################
		
							$sql = "select estid from painel.indicador where indid = {$ind['indid']}";
							$estilo = $db->pegaUm($sql);
							$estilo = !$estilo ? 1 : $estilo;
							 
							################# Veirifica se existe a 2ª Série Histórica p/ comparar #################
							if($seh2['sehid']){ // se existe, faz a comparação para indicar a seta
								
								$valor1 = (float)$seh['sehqtde']; //seh
								$valor2 = (float)$seh2['sehqtde']; //seh2
		
								if($valor1 < $valor2 && $estilo == 1){ // Se o $valor 1 for menor q	o $valor 2 e a tendência for MAIOR MELHOR, a tendência indica queda (seta vermelha p/ baixo)
									$seta = " <img style=\"cursor:pointer\" src=\"../imagens/indicador-vermelha2.png\" align=\"absmiddle\" >";
								}
								if($valor1 < $valor2 && $estilo == 2){ // Se o $valor 1 for menor q	o $valor 2 e a tendência for MENOR MELHOR, a tendência indica aumento (seta verde p/ baixo)
									$seta = " <img style=\"cursor:pointer\" src=\"../imagens/indicador-verde2_para_baixo.png\" align=\"absmiddle\" >";
								}
								if($valor1 > $valor2 && $estilo == 1){ // Se o $valor 1 for maior q	o $valor 2 e a tendência for MAIOR MELHOR, a tendência indica aumento (seta verde p/ cima)
									$seta = " <img style=\"cursor:pointer\"  src=\"../imagens/indicador-verde2.png\" align=\"absmiddle\" >";
								}
								if($valor1 > $valor2 && $estilo == 2){ // Se o $valor 1 for maior q	o $valor 2 e a tendência for MENOR MELHOR, a tendência indica queda (seta vermelha p/ cima)
									$seta = " <img style=\"cursor:pointer\"  src=\"../imagens/indicador-vermelha2_para_cima.png\" align=\"absmiddle\" >";
								}
								if($valor1 > $valor2 && $estilo == 3){ // Se o $valor 1 for maior q	o $valor 2 e a tendência for N/A, a tendência indica aumento (seta cinza p/ cima)
									$seta = " <img style=\"cursor:pointer\"  src=\"../imagens/indicador-cinza.png\" align=\"absmiddle\" >";
								}
								if($valor1 < $valor2 && $estilo == 3){ // Se o $valor 1 for menor q	o $valor 2 e a tendência for N/A, a tendência indica queda (seta cinza p/ baixo)
									$seta = " <img style=\"cursor:pointer\"  src=\"../imagens/indicador-cinza2.png\" align=\"absmiddle\" >";
								}
								
							}
							
							################# Verifica o Tipo de unidade de medida do indicador (umedesc) na tabela unidademeta, p/ exibir no SuperTitle #################
							$sql = "select 
										umedesc 
									from 
										painel.indicador i
									left join 
										painel.unidademeta u on i.umeid = u.umeid
									where 
										indid = {$ind['indid']}";
								
							$umedesc = $db->pegaUm($sql);
							
							################# Verifica o Tipo de unidade de Medição do indicador (unmdesc) na tabela unidademedicao, p/ exibir as casas decimais corretas no sehqtde #################
							$sql = "select 
										unmdesc 
									from 
										painel.indicador i
									left join 
										painel.unidademedicao u on i.unmid = u.unmid
									where 
										indid = {$ind['indid']}";
							
							$unmedsc = $db->pegaUm($sql);
							
							//Verifica o tipo de medição e aplica as regras
							switch($unmedsc){
									case "Número inteiro":
										$seh['sehqtde'] = str_replace(',00','',number_format($seh['sehqtde'],2,',','.'));
										break;
									case "Percentual":
										$seh['sehqtde'] = str_replace(',00','',number_format($seh['sehqtde'],2,',','.'))."%";
										break;
									case "Razão":
										$seh['sehqtde'] = str_replace(',00','',number_format($seh['sehqtde'],2,',','.'));
										break;
									case "Número índice":
										$seh['sehqtde'] = str_replace(',00','',number_format($seh['sehqtde'],2,',','.'));
										break;
									case "Moeda":
										$seh['sehqtde'] = "R$ ".number_format($seh['sehqtde'],2,',','.');
										break;
									default:
										$seh['sehqtde'] = str_replace(',00','',number_format($seh['sehqtde'],2,',','.'));
										break;
								}
						}
		
							################# Verifica a Periodicidade e aplica as regras de exibição de data #################
							switch($perdsc){
								
								case "Diária":
									$tp_data = "d/m/Y";
									break;
								
								case "Mensal":
									$tp_data = "m/Y";
									break;
								
								case "Anual":
									$tp_data = "Y";
									break;
								
								case "Semestral":
									$tp_data = "m/Y";
									break;
								
								case "Trimestral":
									$tp_data = "m/Y";
									break;
								
								case "Semanal":
									$tp_data = "d/m/Y";
									break;
								
								case "Bianual":
									$tp_data = "d/m/Y";
									break;
								
								default:
									$tp_data = "d/m/Y";
									break;
			
							}
						
																		
							################# Pega o Responsável pelo Indicador para exbir no SuperTitle #################
							$responsaveis_ind = "<center><font size=2 ><b>Responsáveis</b></font></center>";
							
							//Coordenador
							$responsaveis_C = $db->carregar("SELECT ent.entnome ,ent.entnumcomercial FROM painel.responsavel resp 
									LEFT JOIN entidade.entidade ent ON ent.entid = resp.entid WHERE indid = {$ind['indid']} and resp.restipo = 'C' ");
							if($responsaveis_C){
								$responsaveis_ind .= "<br /><b>Coordenador:</b>";
								foreach($responsaveis_C as $respon){
									$responsaveis_ind .= "<br /> {$respon['entnome']} - {$respon['entnumcomercial']}";	
								}
							}
							
							//Diretor
							$responsaveis_D = $db->carregar("SELECT ent.entnome ,ent.entnumcomercial FROM painel.responsavel resp 
									LEFT JOIN entidade.entidade ent ON ent.entid = resp.entid WHERE indid = {$ind['indid']} and resp.restipo = 'D' ");
							if($responsaveis_D){
								$responsaveis_ind .= "<br /><b>Diretor:</b>";
								foreach($responsaveis_D as $respon){
									$responsaveis_ind .= "<br /> {$respon['entnome']} - {$respon['entnumcomercial']}";	
								}
							}
							
							//Equipe de Apoio
							$responsaveis_E = $db->carregar("SELECT ent.entnome ,ent.entnumcomercial FROM painel.responsavel resp 
									LEFT JOIN entidade.entidade ent ON ent.entid = resp.entid WHERE indid = {$ind['indid']} and resp.restipo = 'E' ");
							if($responsaveis_E){
								$responsaveis_ind .= "<br /><b>Equipe de Apoio:</b>";
								foreach($responsaveis_E as $respon){
									$responsaveis_ind .= "<br />{$respon['entnome']} - {$respon['entnumcomercial']}";	
								}
							}
							if(!$responsaveis_E && !$responsaveis_C && !$responsaveis_D){
								$responsaveis_ind = "<center><font size=2 ><b>Responsáveis</b></font></center>";
								$responsaveis_ind .= "Não informado";
							}
							
							//Início da Exibição dos elementos na tela
							$cor = $i%2 ? "#FCFCFC" : "#f5f5f5";
							echo "<tr id=\"tr_{$cxcid}_{$ind['indid']}\" bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\" >";
								
							if($tipoConteudo != "Busca Indicadores")
								echo "<td style=\"white-space: nowrap;\" align=\"center\"><img title=\"Remover Indicador do Painel\" onclick=\"removerMeuIndicador($cxcid,{$ind['indid']})\" style=\"cursor:pointer\" src=\"../imagens/excluir.gif\"  /></td>";
							else
								echo "<td style=\"white-space: nowrap;\" align=\"center\"></td>";
								
							echo "<td align=\"center\" ><div style=\"width:100%;text-align:center;color:#0066CC\" >".$ind['indid']."</div></td>
							
							<td><div style=\"cursor:pointer\" onclick=\"exibeDashBoard({$ind['indid']})\" style=\"width:100%;\" >".$ind['indnome']."</div></td>
							
							<td style=\"white-space: nowrap;\" align=\"left\" ><div style=\"margin-left:10px\" >".(!$ind['secdsc']? "<span style=\"color:#AA0000;\" >N/A</span>" : "<span onmouseover=\"this; return escape('$responsaveis_ind');\" >".$ind['secdsc']."</span>")." </div></td>
							
							<td style=\"white-space: nowrap;\" align=\"left\" ><div style=\"margin-left:10px\" >".(!$seh['data']? "<span style=\"color:#AA0000;\" >N/A</span>" : "<span onmouseover=\"this; return escape('$responsaveis_ind');\" >".date($tp_data,strtotime($seh['data']))."</span>")." </div></td>
							
							<td style=\"white-space: nowrap;\" align=\"right\" ><div style=\"width:100%;text-align:right;color:#0066CC\" >".$seh['sehqtde']. " ".$seta."</div></td>
							</tr>";
								
							$i++;
						}
											
				}
				### Lista as Ações - fim
				echo "</table>";
			}
			if(count($acoes) == 0 && $tipoConteudo == "Meus Indicadores"){
				echo "<div style=\"padding:10px;text-align:center;cursor:pointer\" onclick=\"addIndicadoresCaixa({$cxcid})\" >Clique aqui para adicionar indicadores!</div>";
			}
			if(count($acoes) == 0 && $tipoConteudo == "Busca Indicadores"){
				echo "<div style=\"padding:10px;text-align:center;cursor:pointer;color:#990000\" >Não existem registros.</div>";
			}
			
}


function exibeConteudo($cxcid,$tcoid){
	global $db;
	$sql = "select tconome from painel.tipoconteudo where tcoid = $tcoid";
	
	define("TIPO_CONTEUDO_MEUS_INDICADORES",1);
	define("TIPO_CONTEUDO_GRAFICO",2);
	define("TIPO_CONTEUDO_MAPA",3);
	define("TIPO_CONTEUDO_INDICADOR_EIXO",6	);
	define("TIPO_CONTEUDO_INDICADOR_ACAO",7	);
	define("TIPO_CONTEUDO_INDICADOR_SECRETARIA",8 );
	define("TIPO_CONTEUDO_BUSCA_INDICADOR",9 );
	define("TIPO_CONTEUDO_ARVORE_INDICADOR",10 );
	define("TIPO_CONTEUDO_MUNICIPIOS",12 );
	define("TIPO_CONTEUDO_REGIONALIZADORES",13 );
	
	
	switch($tcoid){
		case TIPO_CONTEUDO_MEUS_INDICADORES:
			
			$sql = "select 
						distinct ind.indid 
					from 
						painel.indicadorusuario cxc
					inner join
						painel.indicador ind ON ind.indid = cxc.indid
					inner join
						painel.seriehistorica seh ON seh.indid = ind.indid
					where 
						cxcid = $cxcid and cxc.usucpf = '{$_SESSION['usucpf']}'
					and
						ind.indstatus = 'A'
					and
						seh.sehstatus <> 'I'";
	
		exibeMinhaListaIndicadores($sql,$cxcid,'Meus Indicadores');
				
		break;
		case TIPO_CONTEUDO_GRAFICO:
			
			//Em processo de migração
			$sql = "select conteudo from painel.conteudocaixa where cxcid = $cxcid";
			$dados = $db->pegaUm($sql);
						
			if($dados){
				
				$dado = explode( ";" , unserialize( $dados ) );
				
				foreach($dado as $d){
					
					$dd = explode("=",$d);
					
					$arrDados[ $dd[0]  ] = $dd[1]; 
					
				}
				
				extract($arrDados);
				
				$sql = "select indnome from painel.indicador where indid = $indid";
				$indnome = $db->pegaUm($sql);
				if($tipo != "linha" && $tipo != "barra"){
					$td = explode("_",$tidid);
					$tdiid = $td[1]; 
					$sql = "select tdidsc from painel.detalhetipoindicador where tdiid = $tdiid";
					$tdidsc = $db->pegaUm($sql);
					$exibeNome = "<span onclick=\"exibeDashBoard($indid)\" > <font size=\"2\" >".$indnome ." por " . $tdidsc . ". </font></span> <br />";
				}else{
					$exibeNome = "<span onclick=\"exibeDashBoard($indid)\" > <font size=\"2\" >".$indnome ."</font></span> <br />";
				}?>
				<div style="padding: 5px; cursor: pointer; width: 95%; font-weight: bold; text-align: center;" >
					<?php echo $exibeNome ?>
				</div>
				<div style="text-align: right;font-size: 10px;margin-right:40px;" ><span onclick="maximizaGrafico('geraGrafico.php?<?php echo unserialize( $dados ) ?>');" style="cursor:pointer" >Maximizar</span></div>
				<div style="padding: 5px">
				<div id="grafico_indicador_<?php echo $cxcid ?>"></div>
				<script type="text/javascript">
					<?php //if(TIPO_GRAFICO == TIPO_GRAFICO_HIGHCHARTS):?>
						//geraGraficoHighChartsPorDiv("grafico_indicador_<?php echo $cxcid ?>",<?php echo $indid ?>,"geraGrafico.php?<?php echo unserialize( $dados ) ?>")									
					<?php //elseif(TIPO_GRAFICO == TIPO_GRAFICO_FLASH):?>
						swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_indicador_<?php echo $cxcid ?>", "100%", "200", "9.0.0", "expressInstall.swf", {"data-file":"geraGrafico.php?<?php echo unserialize( $dados ) ?>","loading":"Carregando gráfico..."} );
					<?php //endif; ?>
				</script>
				</div>
				<?php
				if($tipo != "linha" && $tipo != "barra")
					exibeLegendaGrafico($indid,$sehid,$tdiid,$tdiid,$dpeid1,$dpeid2,$estuf,$muncod,$tidid1,$tidid2,$detalhe,$valorDetalhe);?>
					<div style="padding: 5px;"><?php exibeFonterafico($indid); ?></div>	
				<?php
			}else{
				
				$sql = "select 
							tcgid,indid,tpgrafico,tipotidid,valortidid,sehid,dpeid
						from 
							painel.tipoconteudografico 
						where 
							cxcid = $cxcid 
						and
							usucpf = '{$_SESSION['usucpf']}'
						and
							tcgstatus = 'A'";
				$grafico = $db->pegaLinha($sql);
							
				if($grafico){
					$grafico['tpgrafico'] = trim($grafico['tpgrafico']);
					if($grafico['tpgrafico'] == "pizza"){
						$sql = "select indnome,tdidsc{$grafico['tipotidid']},dpedsc from painel.v_detalheindicadorsh where sehid = {$grafico['sehid']} limit 1";
						$indnome = $db->pegaLinha($sql);
						$indnome = "<span onclick=\"exibeDashBoard({$grafico['indid']})\" > <font size=\"2\" >".$indnome['indnome'] ." por " . $indnome['tdidsc'.$grafico['tipotidid']] . ". </font></span> <br /><br /> Período: ";
						$indnome .= "<select id=\"dpeid_{$cxcid}\" onchange=\"mudaPeriodoGrafico({$grafico['tcgid']},$cxcid,{$grafico['indid']},this.value,'tdiid{$grafico['tipotidid']}_{$grafico['valortidid']}','{$grafico['tipotidid']}','{$grafico['valortidid']}');\" >";
						
						//function mudaPeriodoGrafico(tcgid,dpeid,cxcid,indid,sehid,tdiid){
						//Substituição da utilização da view
						$sqlPeriodos = "select distinct 
									seh.sehid,
									dpe.dpedsc
								from 
									painel.seriehistorica seh
								inner join
									painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
								where
									indid = {$grafico['indid']}
								order by
									dpe.dpedsc";
						
						$periodos = $db->carregar($sqlPeriodos);
						
						$periodos = !$periodos ? array() : $periodos;
						
						foreach($periodos as $periodo){
							$indnome .= "<option ". ( $periodo['sehid'] == $grafico['sehid'] ? "selected='selected'" : "" )."  value=\"{$periodo['sehid']}\" >{$periodo['dpedsc']}</option>";
						}
						
						$indnome .= "</select>";
																
					}elseif($grafico['tpgrafico'] == "barra_fatia"){
						$sql = "select indnome,tdidsc{$grafico['tipotidid']},dpedsc from painel.v_detalheindicadorsh where indid = {$grafico['indid']} limit 1";
						$indnome = $db->pegaLinha($sql);
						$indnome = "<span onclick=\"exibeDashBoard({$grafico['indid']})\" > <font size=\"2\" >".$indnome['indnome'] ." por " . $indnome['tdidsc'.$grafico['tipotidid']] . ". </font></span> <br /><br />";
					}else{
					
						$sql = "select indnome from painel.indicador where indid = {$grafico['indid']}";
						$indnome = $db->pegaUm($sql);
						$indnome = "<span onclick=\"exibeDashBoard({$grafico['indid']})\"  ><font size=\"2\" >".$indnome."</font></span>";
					}
									
					echo "<div style=\"cursor:pointer;width:95%;font-weight:bold;text-align:center;padding:5px;\"> $indnome </div>";
					
					if($grafico['tipotidid'] && $grafico['valortidid'] && $grafico['sehid']){
						$tdiid = ";{$grafico['sehid']};tdiid{$grafico['tipotidid']}_{$grafico['valortidid']}";
					}
					
					echo 
						"<script type=\"text/javascript\">
							swfobject.embedSWF(\"/includes/open_flash_chart/open-flash-chart.swf\", \"grafico_{$cxcid}\", \"100%\", \"200\", \"9.0.0\", \"expressInstall.swf\", {\"data-file\":\"geraGrafico.php?tipo={$grafico['indid']};{$grafico['tpgrafico']}$tdiid\",\"loading\":\"Carregando gráfico...\"} );
						</script>
						<div style=\"padding:5px;text-align:center\">
							<div id=\"grafico_{$cxcid}\"></div>
						</div>";
					
					
					//legenda para FATIAS
					if(trim($grafico['tpgrafico']) == "pizza" || trim($grafico['tpgrafico']) == "barra_fatia" || trim($grafico['tpgrafico']) == "barra_comp"){
						echo "<div id=\"{$cxcid}_legenda_grafico\">";
						if(trim($grafico['tpgrafico']) == "pizza")
							exibeLegendaGrafico($grafico['indid'],$grafico['sehid'],$grafico['tipotidid'],$grafico['valortidid']);
						else
							exibeLegendaGrafico($grafico['indid'],false,$grafico['tipotidid'],$grafico['valortidid']);
						echo "</div>";
					}
					echo "<div style=\"padding:5px;\">";
					exibeFonterafico($grafico['indid']);
					echo "</div>";
					
				}else{
					echo "<div style=\"padding:10px;text-align:center;cursor:pointer\" onclick=\"addGrafico({$cxcid})\" >Clique aqui para adicionar um gráfico!</div>";
				}
					
			}
						
		break;
		
		case TIPO_CONTEUDO_INDICADOR_EIXO:
			echo "<div style=\"padding:10px;text-align:center;cursor:pointer\" >Selecione a Área...</div>";
		break;
		
		CASE TIPO_CONTEUDO_BUSCA_INDICADOR:
			exibeFormBuscaIndicador($cxcid);			
		break;
		
		case TIPO_CONTEUDO_ARVORE_INDICADOR:
			exibeArvore($cxcid);
		break;
		
		case TIPO_CONTEUDO_MUNICIPIOS:
			exibeListaMunicipios($cxcid);
		break;
		case TIPO_CONTEUDO_UNIVERSIDADES:
			exibeListaUniversidades($cxcid);
		break;
		case TIPO_CONTEUDO_REGIONALIZADORES:
			exibeListaRegionalizadores($cxcid);
		break;
		default:
			echo "<div style=\"padding:10px;text-align:center;cursor:pointer\" >Em contrução...</div>";
		break;
	}
}

function exibePesquisa($cxcid,$checkbox){
	global $db;?>
	<form method="POST"  name="formulario">
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
			<td colspan=3 bgcolor="#DCDCDC"  align='center' ><b>Selecione o Indicador</b></td>
		</tr>
		<tr>
			<td  align='right' class="SubTituloDireita">Identificador do Indicador:</td>
		    <td><?=campo_texto('indid','N','S','',15,15,'############','','','','',"id='indid'",'',$_POST['indid']);?></td>
		    <td rowspan=7>
		    <?php
			//Lista os indicadores já selecionados
			if($checkbox == "true"){
				echo "<fieldset>
							<legend>Indicadores Selecionados</legend>
								<div style=\"overflow:auto;width:100%;height:120px\" id=\"indicadores_selecionados\" ></div>
						</fieldset>
						<script>listaIndicadoresSelecionados($cxcid,'{$_SESSION['usucpf']}');</script>
						";
				
			} 
			?>
			</td>
		</tr>
		<tr>
			<?php 
				$sql= "SELECT 
					exodsc AS descricao, 
					exoid AS codigo
				FROM 
					painel.eixo
				order by
					descricao
				";
			?>
		    <td align='right' class="SubTituloDireita">Área:</td>
		    <td><?=$db->monta_combo('exoid',$sql,'S','Selecione...','','','','','N',"exoid","",$_POST['exoid']);?></td>
		</tr> 
		<tr>
			<td  align='right' class="SubTituloDireita">Nome Indicador:</td>
		    <td><?=campo_texto('indnome','N','S','',60,100,'','',"","","","id='indnome'","",$_POST['indnome']);?></td>
		</tr>
		
		<tr>
			<?php 
				$sql= "SELECT 
					tpidsc AS descricao, 
					tpiid AS codigo
				FROM 		
					painel.tipoindicador
				order by
					descricao	 
				";
			?>
		    <td align='right' class="SubTituloDireita">Tipo Indicador:</td>
		    <td><?=$db->monta_combo('tpiid',$sql,'S','Selecione...','','','','','N',"tpiid","",$_POST['tpiid']);?></td>
		</tr> 
		<tr>
			<?php 
				$sql= "SELECT 
					secdsc AS descricao, 
					secid AS codigo
				FROM 		
					painel.secretaria 
				order by
					descricao
				";
			?>
		    <td align='right' class="SubTituloDireita">Secretaria:</td>
		    <td><?=$db->monta_combo('secid',$sql,'S','Selecione...',"filtraAcao","",'','','N',"secid","",$_POST['secid']);?></td>
		</tr>
		<tr>
			<?php 
				if($_POST['secid']){
					$sql= "	select 
								acadsc AS descricao, 
								acaid AS codigo
							FROM 		
								painel.acao
							where
								acaid in(
										select 
											distinct acaid
										from
											painel.indicador
										where
											secid = {$_POST['secid']}
									)
							order by
								descricao";
				}else{
					$sql= "	SELECT 
								acadsc AS descricao, 
								acaid AS codigo
							FROM 		
								painel.acao
							order by
								 descricao";
				}
			?>
		    <td align='right' class="SubTituloDireita">Ação:</td>
		    <td id="td_acao" ><?=$db->monta_combo('acaid',$sql,'S','Selecione...','','','','','N',"acaid","",$_POST['acaid']);?></td>
		</tr> 
		<tr style="display: none">
			<?php 
				$sql= "SELECT 
					mapdsc AS descricao, 
					mapid AS codigo
				FROM 
					painel.mapa
				order by
					 descricao 
				";
			?>
		    <td align='right' class="SubTituloDireita">Mapa Estratégico:</td>
		    <td><?=$db->monta_combo('mapid',$sql,'S','Selecione...','','','','','S');?></td>
		</tr> 
		<tr style="display: none">
			<?php 
				$sql= "SELECT 
				temdsc AS descricao, 
				temid AS codigo
				FROM 		
				painel.tema		 
				";
			?>
		    <td align='right' class="SubTituloDireita">Tema Estratégico:</td>
		    <td><?=$db->monta_combo('temid',$sql,'S','Selecione...','','','','','S');?></td>
		</tr>
		<tr>
			<?php 
				$sql= "SELECT 
					regdescricao AS descricao, 
					regid AS codigo
				FROM 
					painel.regionalizacao
				where
					regstatus = 'A'
				order by
					 descricao	 
				";
			?>
		    <td align='right' class="SubTituloDireita">Regionalização:</td>
		    <td><?=$db->monta_combo('regid',$sql,'S','Selecione...','','','','','N',"regid","",$_POST['regid']);?></td>
		</tr> 
		
		<tr bgcolor="#cccccc">
			      <td></td>
			      <td colspan=2>
			  	  	<input type="button" class="botao" name="btassociar" value="Pesquisar" onclick="pesquisar(<?=$cxcid ?>,<?=$checkbox ?>);">
			  	  	<input type="button" class="botao" name="bt_limpar" value="Limpar" onclick="limpar();">
			  	  	<input type="button" class="botao" name="cancelar" value="Cancelar" id="button_cancelar" />
			  	  	
			  	  	<?php if($checkbox == "true"){ ?>
			  	  		<input type="button" class="botao" name="salvar_cima" id="salvar_cima" value="Salvar" />
			  	  	<?php } ?>
			  	  </td>
		 </tr>
		 <tr>
			      <td colspan="3" id="resultado_pesquisa"></td>
		 </tr>
		 
		</table>
	</form>
<?php }


function exibePesquisaMunicipio($cxcid,$checkbox){
	global $db;?>
	<form method="POST"  name="formulario">
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
			<td colspan=3 bgcolor="#DCDCDC"  align='center' ><b>Selecione o(s) Município(s)</b></td>
		</tr>
		<tr>
			<td  align='right' class="SubTituloDireita">Região:</td>
			<?php 
			$sql = "select regcod as codigo, regdescricao as descricao from territorios.regiao order by descricao";
			?>
		    <td><?=$db->monta_combo('regcod',$sql,'S','Selecione...','filtraEstado','','','','N',"regcod","",$_POST['regcod']);?></td>
		    <td rowspan=5>
		    <?php
			//Lista os indicadores já selecionados
			if($checkbox == "true"){
				echo "<fieldset>
							<legend>Municípios Selecionados</legend>
								<div style=\"overflow:auto;width:100%;height:100px\" id=\"municipios_selecionados\" ></div>
						</fieldset>
						<script>listaMunicipiosSelecionados($cxcid,'{$_SESSION['usucpf']}');</script>
						";
				
			} 
			?>
			</td>
		</tr>
		<tr>
		<?php 
				$sql = "select estuf as codigo, estdescricao as descricao from territorios.estado order by estdescricao";
			?>
			<td  align='right' class="SubTituloDireita">Estado:</td>
		    <td id="exibeEstado" ><?=$db->monta_combo('estuf',$sql,'S','Selecione...','','','','','N',"estuf","",$_POST['estuf']);?></td>
		</tr>
		<tr>
			<?php 
				$sql= "SELECT 
					gtmdsc AS descricao, 
					gtmid AS codigo
				FROM 
					territorios.grupotipomunicipio
				order by
					descricao
				";
			?>
		    <td align='right' class="SubTituloDireita">Grupo de Municípios:</td>
		    <td><?=$db->monta_combo('gtmid',$sql,'S','Selecione...','filtraGrupoMunicipios','','','','N',"gtmid","",$_POST['gtmid']);?></td>
		</tr> 
		<tr>
		<?php 
				$sql= "SELECT 
					tpmdsc AS descricao, 
					tpmid AS codigo
				FROM 
					territorios.tipomunicipio
				order by
					descricao
				";
			?>
			<td  align='right' class="SubTituloDireita">Tipos de Municípios:</td>
		    <td id="exibeTipoMunicipio" ><?=$db->monta_combo('tpmid',$sql,'S','Selecione...','','','','','N',"tpmid","",$_POST['tpmid']);?></td>
		</tr>
		<tr>
		<?php 
				$sql= "SELECT 
					tpmdsc AS descricao, 
					tpmid AS codigo
				FROM 
					territorios.tipomunicipio
				order by
					descricao
				";
			?>
			<td  align='right' class="SubTituloDireita">Município:</td>
		    <td><?=campo_texto($municipio,"N","S",'',60,100,"","","","","","id='municipio'","",$_POST['municipio']) ?></td>
		</tr>
		
		<tr bgcolor="#cccccc">
			      <td></td>
			      <td colspan=2>
			  	  	<input type="button" class="botao" name="btassociar" value="Pesquisar" onclick="pesquisarMunicipio(<?=$cxcid ?>,<?=$checkbox ?>);">
			  	  	<input type="button" class="botao" name="bt_limpar" value="Limpar" onclick="limparMunicipios();">
			  	  	<input type="button" class="botao" name="cancelar" value="Cancelar" id="button_cancelar" />
			  	  	
			  	  	<?php if($checkbox == "true"){ ?>
			  	  		<input type="button" class="botao" name="salvar_cima" id="salvar_cima" value="Salvar" />
			  	  	<?php } ?>
			  	  </td>
		 </tr>
		 <tr>
			      <td colspan="3" id="resultado_pesquisa"></td>
		 </tr>
		 
		</table>
	</form>
<?php }

function exibePesquisaRegionalizadores($cxcid,$checkbox){
	global $db;?>
	<form method="POST"  name="formulario">
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
			<td colspan=3 bgcolor="#DCDCDC"  align='center' ><b>Selecione o(s) Regionalizador(es)</b></td>
		</tr>
		<tr id="tr_regid" >
			<?php 
				$sql= "SELECT 
					regdescricao AS descricao, 
					regid AS codigo
				FROM 		
					painel.regionalizacao
				order by
					 descricao		 
				";
			?>
		    <td align='right' class="SubTituloDireita">Regionalização:</td>
		    <td><?=$db->monta_combo('regid',$sql,'S','Selecione...','exibeRegionalizacao','','','','S','regid');?>
		    	<span id="span_selecione" style="color:#990000" >Selecione a Regionalização.</span>
		    	</td>
		</tr>
		<tr id="tr_regcod" style="display:none" >
			<td  align='right' class="SubTituloDireita">Região:</td>
			<?php 
			$sql = "select regcod as codigo, regdescricao as descricao from territorios.regiao order by descricao";
			?>
		    <td><?=$db->monta_combo('regcod',$sql,'S','Selecione...','filtraEstado','','','','N',"regcod","",$_POST['regcod']);?></td>
		</tr>
		<tr id="tr_estuf" style="display:none" >
		<?php 
				$sql = "select estuf as codigo, estdescricao as descricao from territorios.estado order by estdescricao";
			?>
			<td  align='right' class="SubTituloDireita">Estado:</td>
		    <td id="exibeEstado" ><?=$db->monta_combo('estuf',$sql,'S','Selecione...','','','','','N',"estuf","",$_POST['estuf']);?></td>
		</tr>
		<tr id="tr_mundesc" style="display:none">
			<td  align='right' class="SubTituloDireita">Município:</td>
		    <td><?=campo_texto($mundesc,"N","S",'',60,100,"","","","","","id='mundesc'","",$_POST['mundesc']) ?></td>
		</tr>
		<tr id="tr_unidesc" style="display:none">
			<td  align='right' class="SubTituloDireita">Universidade:</td>
		    <td><?=campo_texto($unidesc,"N","S",'',60,100,"","","","","","id='unidesc'","",$_POST['unidesc']) ?></td>
		</tr>
		<tr id="tr_iesdesc" style="display:none">
			<td  align='right' class="SubTituloDireita">IES:</td>
		    <td><?=campo_texto($iesdesc,"N","S",'',60,100,"","","","","","id='iesdesc'","",$_POST['iesdesc']) ?></td>
		</tr>
		<tr id="tr_campusdesc" style="display:none">
			<td  align='right' class="SubTituloDireita">Campus:</td>
		    <td><?=campo_texto($campusdesc,"N","S",'',60,100,"","","","","","id='campusdesc'","",$_POST['campusdesc']) ?></td>
		</tr>
		<tr id="tr_escoladesc" style="display:none">
			<td  align='right' class="SubTituloDireita">Escola:</td>
		    <td><?=campo_texto($escoladesc,"N","S",'',60,100,"","","","","","id='escoladesc'","",$_POST['escoladesc']) ?></td>
		</tr>
		<tr id="tr_iepgdesc" style="display:none">
			<td  align='right' class="SubTituloDireita">IEPG:</td>
		    <td><?=campo_texto($iepgdesc,"N","S",'',60,100,"","","","","","id='iepgdesc'","",$_POST['iepgdesc']) ?></td>
		</tr>
		<tr id="tr_institutodesc" style="display:none">
			<td  align='right' class="SubTituloDireita">IEPG:</td>
		    <td><?=campo_texto($institutodesc,"N","S",'',60,100,"","","","","","id='institutodesc'","",$_POST['institutodesc']) ?></td>
		</tr>
		
		
		<tr bgcolor="#cccccc">
			      <td></td>
			      <td colspan=2>
			  	  	<input type="button" class="botao" name="btassociar" value="Pesquisar" onclick="pesquisarRegionalizadores(<?=$cxcid ?>,<?=$checkbox ?>);">
			  	  	<input type="button" class="botao" name="bt_limpar" value="Limpar" onclick="limparRegionalizadores();">
			  	  	<input type="button" class="botao" name="cancelar" value="Cancelar" id="button_cancelar" />
			  	  	
			  	  	<?php if($checkbox == "true"){ ?>
			  	  		<input type="button" class="botao" name="salvar_cima" id="salvar_cima" value="Salvar" />
			  	  	<?php } ?>
			  	  </td>
		 </tr>
		 <tr>
			      <td colspan="3" id="resultado_pesquisa"></td>
		 </tr>
		 
		</table>
	</form>
<?php }

function pesquisaRegionalizadores($dados){
	global $db;
	extract($dados);
	//dbg($dados);
		
	switch($regid){
		case REGIONALIZACAO_ESCOLA:
			if($regcod) {
				$filtroprocesso[] = "esc.escuf in (select estuf from territorios.estado where regcod = '".$regcod."')";
			}
			if($estuf) {
				$filtroprocesso[] = "esc.escuf = '".$estuf."'";
			}
			if($mundesc) {				
				$filtroprocesso[] = "upper(esc.escmunicipio) like upper('%".utf8_decode($mundesc)."%')";
			}
			if($escoladesc) {
				$filtroprocesso[] = "upper(esc.escdsc) like upper('%".utf8_decode($escoladesc)."%')";
			}
			
			$sql = "select 
						'<input onclick=\"addRegionalizador(this,\'".$cxcid."\',\'".$regid."\')\" type=\"checkbox\" value=\"' || esccodinep ||  '\" id=\"' || esccodinep ||  '\" /> ' || esccodinep || '' as codigo,
						escdsc as descricao,
						escmunicipio as municipio
					from 
						painel.escola esc
					".((count($filtroprocesso) > 0)?"WHERE ".implode(" AND ", $filtroprocesso):"")."
					order by
						descricao,municipio";
			$cabecalho = array("Código","Escola","Município");
		break;
		
		case REGIONALIZACAO_POLO:
			if($regcod) {
				$filtroprocesso[] = "est.escuf in (select estuf from territorios.estado where regcod = '".$regcod."')";
			}
			if($estuf) {
				$filtroprocesso[] = "est.escuf = '".$estuf."'";
			}
			if($mundesc) {				
				$filtroprocesso[] = "upper(pol.muncod) like upper('%".utf8_decode($mundesc)."%')";
			}
			if($escoladesc) {
				$filtroprocesso[] = "upper(pol.poldsc) like upper('%".utf8_decode($poldsc)."%')";
			}
			
			$sql = "select 
						'<input onclick=\"addRegionalizador(this,\'".$cxcid."\',\'".$regid."\')\" type=\"checkbox\" value=\"' || polid ||  '\" id=\"' || polid ||  '\" /> ' || polid || '' as codigo,
						poldsc as descricao,
						mun.mundescricao as municipio
					from 
						painel.polo pol
					left join
						territorios.municipio mun ON pol.muncod = mun.muncod
					".((count($filtroprocesso) > 0)?"WHERE ".implode(" AND ", $filtroprocesso):"")."
					order by
						descricao,municipio";
			$cabecalho = array("Código","Escola","Município");
		break;
		
		case REGIONALIZACAO_IES:
			if($regcod) {				
				$filtroprocesso[] = "ies.iesestuf in (select estuf from territorios.estado where regcod = '".$regcod."')";
			}
			if($estuf) {				
				$filtroprocesso[] = "ies.iesestuf = '".$estuf."'";
			}
			if($mundesc) {				
				$filtroprocesso[] = "upper(ter.mundescricao) like upper('%".utf8_decode($mundesc)."%')";
			}
			if($iesdesc) {				
				$filtroprocesso[] = "upper(ies.iesdsc) like upper('%".utf8_decode($iesdesc)."%')";
			}
			//$filtroprocesso[] = "iesano = ( select max(iesano) from painel.ies )";
			
			$sql = "select 
						'<input onclick=\"addRegionalizador(this,\'".$cxcid."\',\'".$regid."\')\" type=\"checkbox\" value=\"' || iesid ||  '\" id=\"' || iesid ||  '\" /> ' || iesid || '' as codigo,
						iesdsc as descricao,
						ter.mundescricao as municipio
					from 
						painel.ies ies
					left join
						territorios.municipio ter ON ter.muncod = ies.iesmuncod
					".((count($filtroprocesso) > 0)?"WHERE ".implode(" AND ", $filtroprocesso):"")."
					order by
						descricao,municipio";
			$cabecalho = array("Código","IES","Município");
		break;
		
		case REGIONALIZACAO_MUN:
			if($mundesc) {				
				$filtroprocesso[] = "upper(mun.mundescricao) like upper('%".utf8_decode($mundesc)."%')";
			}
			if($regcod) {			
				$filtroprocesso[] = "est.regcod = '".$regcod."'";
			}
						
			$sql = "select 
						'<input onclick=\"addRegionalizador(this,\'".$cxcid."\',\'".$regid."\')\" type=\"checkbox\" value=\"' || muncod ||  '\" id=\"' || muncod ||  '\" /> ' || muncod || '' as codigo,
						mundescricao as descricao,
						est.estuf as estado
					from 
						territorios.municipio mun
					left join
						territorios.estado est ON est.estuf = mun.estuf
					".((count($filtroprocesso) > 0)?"WHERE ".implode(" AND ", $filtroprocesso):"")."
					order by
						descricao,estado";
			$cabecalho = array("Código","Município","UF");
		break;
		
		case REGIONALIZACAO_UF:
			if($regcod) {				
				$filtroprocesso[] = "regcod = '".$regcod."'";
			}
						
			$sql = "select 
						'<input onclick=\"addRegionalizador(this,\'".$cxcid."\',\'".$regid."\')\" type=\"checkbox\" value=\"' || estuf ||  '\" id=\"' || estuf ||  '\" /> ' || estuf || '' as codigo,
						estdescricao as descricao
					from 
						territorios.estado
					".((count($filtroprocesso) > 0)?"WHERE ".implode(" AND ", $filtroprocesso):"")."
					order by
						descricao";
			$cabecalho = array("UF","Estado");
		break;
		
		case REGIONALIZACAO_BRASIL:
			$sql = "select 
						'<input onclick=\"addRegionalizador(this,\'".$cxcid."\',\'".$regid."\')\" type=\"checkbox\" value=\"brasil\" id=\"brasil\" /> 1' as codigo,
						'Brasil' as descricao
					from 
						territorios.estado
					where 1 = 1
					order by
						descricao
					limit 1";
			$cabecalho = array("Código","Pais");
		break;
		
		case REGIONALIZACAO_POSGRADUACAO:
			if($regcod) {				
				$filtroprocesso[] = "iepg.estuf in (select estuf from territorios.estado where regcod = '".$regcod."')";
			}
			if($estuf) {				
				$filtroprocesso[] = "iepg.estuf = '".$estuf."'";
			}
			if($mundesc) {				
				$filtroprocesso[] = "upper(ter.mundescricao) like upper('%".utf8_decode($mundesc)."%')";
			}
			if($iepgdesc) {				
				$filtroprocesso[] = "upper(iepg.iepdsc) like upper('%".utf8_decode($iepgdesc)."%')";
			}
						
			$sql = "select 
						'<input onclick=\"addRegionalizador(this,\'".$cxcid."\',\'".$regid."\')\" type=\"checkbox\" value=\"' || iepid ||  '\" id=\"' || iepid ||  '\" /> ' || iepid || '' as codigo,
						iepdsc as descricao,
						ter.mundescricao as municipio
					from 
						painel.iepg iepg
					left join
						territorios.municipio ter ON ter.muncod = iepg.muncod
					".((count($filtroprocesso) > 0)?"WHERE ".implode(" AND ", $filtroprocesso):"")."
					order by
						descricao,municipio";
			$cabecalho = array("Código","IEPG","Município");
		break;
		
		case REGIONALIZACAO_CAMPUS_SUPERIOR:
			if($regcod) {				
				$filtroprocesso[] = "ende.estuf in (select estuf from territorios.estado where regcod = '".$regcod."')";
			}
			if($estuf) {				
				$filtroprocesso[] = "ende.estuf = '".$estuf."'";
			}
			if($mundesc) {				
				$filtroprocesso[] = "upper(mun.mundescricao) like upper('%".utf8_decode($mundesc)."%')";
			}
			if($campusdesc) {				
				$filtroprocesso[] = "upper(ent.entnome) like upper('%".utf8_decode($campusdesc)."%')";
			}
			
			$sql = "SELECT 
						'<input onclick=\"addRegionalizador(this,\'".$cxcid."\',\'".$regid."\')\" type=\"checkbox\" value=\"' || ent.entid ||  '\" id=\"' || ent.entid ||  '\" /> ' || ent.entid || '' as codigo, 
						UPPER(ent.entnome) as descricao,
						mun.mundescricao as municipio
					FROM 
						entidade.entidade ent
					INNER JOIN
						entidade.funcaoentidade fen ON fen.entid = ent.entid
					INNER JOIN 
						entidade.endereco ende ON ende.entid = ent.entid
					LEFT JOIN 
						territorios.municipio mun ON mun.muncod = ende.muncod
					WHERE
						fen.funid = 18
					".((count($filtroprocesso) > 0)?"AND ".implode(" AND ", $filtroprocesso):"")."
					ORDER BY 
						descricao,municipio";
			$cabecalho = array("Código","Campus","Município");
		break;
		
		case REGIONALIZACAO_HOSPITAL:
			if($regcod) {				
				$filtroprocesso[] = "ende.estuf in (select estuf from territorios.estado where regcod = '".$regcod."')";
			}
			if($estuf) {				
				$filtroprocesso[] = "ende.estuf = '".$estuf."'";
			}
			if($mundesc) {				
				$filtroprocesso[] = "upper(mun.mundescricao) like upper('%".utf8_decode($mundesc)."%')";
			}
			if($campusdesc) {				
				$filtroprocesso[] = "upper(ent.entnome) like upper('%".utf8_decode($campusdesc)."%')";
			}
			
			$sql = "SELECT 
						'<input onclick=\"addRegionalizador(this,\'".$cxcid."\',\'".$regid."\')\" type=\"checkbox\" value=\"' || ent.entid ||  '\" id=\"' || ent.entid ||  '\" /> ' || ent.entid || '' as codigo, 
						UPPER(ent.entnome) as descricao,
						mun.mundescricao as municipio
					FROM 
						entidade.entidade ent
					INNER JOIN
						entidade.funcaoentidade fen ON fen.entid = ent.entid
					INNER JOIN 
						entidade.endereco ende ON ende.entid = ent.entid
					LEFT JOIN 
						territorios.municipio mun ON mun.muncod = ende.muncod
					WHERE
						fen.funid = 16
					".((count($filtroprocesso) > 0)?"AND ".implode(" AND ", $filtroprocesso):"")."
					ORDER BY 
						descricao,municipio";
			$cabecalho = array("Código","Hospital","Município");
		break;
		
		case REGIONALIZACAO_CAMPUS_PROFISSIONAL:
			if($regcod) {				
				$filtroprocesso[] = "ende.estuf in (select estuf from territorios.estado where regcod = '".$regcod."')";
			}
			if($estuf) {				
				$filtroprocesso[] = "ende.estuf = '".$estuf."'";
			}
			if($mundesc) {				
				$filtroprocesso[] = "upper(mun.mundescricao) like upper('%".utf8_decode($mundesc)."%')";
			}
			if($campusdesc) {				
				$filtroprocesso[] = "upper(ent.entnome) like upper('%".utf8_decode($campusdesc)."%')";
			}
			
			$sql = "SELECT 
						'<input onclick=\"addRegionalizador(this,\'".$cxcid."\',\'".$regid."\')\" type=\"checkbox\" value=\"' || ent.entid ||  '\" id=\"' || ent.entid ||  '\" /> ' || ent.entid || '' as codigo, 
						UPPER(ent.entnome) as descricao,
						mun.mundescricao as municipio
					FROM 
						entidade.entidade ent
					INNER JOIN
						entidade.funcaoentidade fen ON fen.entid = ent.entid
					INNER JOIN 
						entidade.endereco ende ON ende.entid = ent.entid
					LEFT JOIN 
						territorios.municipio mun ON mun.muncod = ende.muncod
					WHERE
						fen.funid = 17
					".((count($filtroprocesso) > 0)?"AND ".implode(" AND ", $filtroprocesso):"")."
					ORDER BY 
						descricao,municipio";
			$cabecalho = array("Código","Campus","Município");
		break;
		
		case REGIONALIZACAO_UNIVERSIDADE:
			if($regcod) {				
				$filtroprocesso[] = "ende.estuf in (select estuf from territorios.estado where regcod = '".$regcod."')";
			}
			if($estuf) {				
				$filtroprocesso[] = "ende.estuf = '".$estuf."'";
			}
			if($mundesc) {				
				$filtroprocesso[] = "upper(mun.mundescricao) like upper('%".utf8_decode($mundesc)."%')";
			}
			if($unidesc) {				
				$filtroprocesso[] = "upper(uni.unidsc) like upper('%".utf8_decode($unidesc)."%')";
			}
			
			$sql = "SELECT 
						'<input onclick=\"addRegionalizador(this,\'".$cxcid."\',\'".$regid."\')\" type=\"checkbox\" value=\"' || uni.unicod ||  '\" id=\"' || uni.unicod ||  '\" /> ' || uni.unicod || '' as codigo, 
						UPPER(uni.unidsc) as descricao,
						mun.mundescricao as municipio
					FROM 
						public.unidade uni
					INNER JOIN
						entidade.entidade ent ON ent.entunicod = uni.unicod
					INNER JOIN 
						entidade.endereco ende ON ende.entid = ent.entid
					LEFT JOIN 
						territorios.municipio mun ON mun.muncod = ende.muncod
					WHERE
						uni.orgcod= '". CODIGO_ORGAO_SISTEMA. "'
					AND
						gunid IN (3, 9)
					AND 
						unistatus='A' 
					".((count($filtroprocesso) > 0)?"AND ".implode(" AND ", $filtroprocesso):"")."
					ORDER BY 
						descricao,municipio";
			$cabecalho = array("Código","Universidade","Município");
		break;
		
		case REGIONALIZACAO_INSTITUTO:
			if($regcod) {				
				$filtroprocesso[] = "ende.estuf in (select estuf from territorios.estado where regcod = '".$regcod."')";
			}
			if($estuf) {				
				$filtroprocesso[] = "ende.estuf = '".$estuf."'";
			}
			if($mundesc) {				
				$filtroprocesso[] = "upper(mun.mundescricao) like upper('%".utf8_decode($mundesc)."%')";
			}
			if($institutodesc) {				
				$filtroprocesso[] = "upper(uni.unidsc) like upper('%".utf8_decode($institutodesc)."%')";
			}
			
			$sql = "SELECT 
						'<input onclick=\"addRegionalizador(this,\'".$cxcid."\',\'".$regid."\')\" type=\"checkbox\" value=\"' || uni.unicod ||  '\" id=\"' || uni.unicod ||  '\" /> ' || uni.unicod || '' as codigo, 
						UPPER(uni.unidsc) as descricao,
						mun.mundescricao as municipio
					FROM 
						public.unidade uni
					INNER JOIN
						entidade.entidade ent ON ent.entunicod = uni.unicod
					INNER JOIN 
						entidade.endereco ende ON ende.entid = ent.entid
					LEFT JOIN 
						territorios.municipio mun ON mun.muncod = ende.muncod
					WHERE
						gunid = 2 
					AND 
						unistatus='A' 
					".((count($filtroprocesso) > 0)?"AND ".implode(" AND ", $filtroprocesso):"")."
					ORDER BY 
						descricao,municipio";
			$cabecalho = array("Código","Instituto","Município");
		break;
		
		default:
			echo "<center><span style=\"color:#990000\" >Não existem registros.</span></center>";
		break;
	}
	if($cabecalho)
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%','N');
	
	$sql = "select conteudo from painel.conteudocaixa where cxcid = $cxcid";
	$MeusRegionalizadores = $db->pegaUm($sql);
	if($MeusRegionalizadores){
		$MeusRegionalizadores = unserialize($MeusRegionalizadores);
		if($MeusRegionalizadores[$regid]){
			if(is_array($MeusRegionalizadores[$regid])){
				foreach($MeusRegionalizadores[$regid] as $cod){
					echo "<script>if(document.getElementById('$cod')){document.getElementById('$cod').checked = true}</script>";
				}
			}
		}
		
	}
	
}

function addRegionalizador($cxcid,$regid,$codigo,$acao){
	global $db;
	
	$sql = "select conteudo from painel.conteudocaixa where cxcid = $cxcid";
	$conteudo = $db->pegaUm($sql);
	
	if($conteudo){
		$conteudo = unserialize($conteudo);
		if($conteudo[ $regid ] && $acao == "incluir"){
			array_push($conteudo[ $regid ],$codigo);
		}
		if(!$conteudo[ $regid ] && $acao == "incluir"){
			$conteudo[ $regid ][] = $codigo;
		}
		if($conteudo[ $regid ] && $acao == "excluir"){
			$key = array_search($codigo, $conteudo[ $regid ] );
			unset($conteudo[ $regid ][ $key ]);
		}
		
		$conteudo = serialize($conteudo);
		
		$sql = "update painel.conteudocaixa set conteudo = '$conteudo' where cxcid = $cxcid";
		
	}else{
		$conteudo[ $regid ][] = $codigo;
		
		$conteudo = serialize($conteudo);
		
		$sql = "insert into painel.conteudocaixa (cxcid,conteudo) values ($cxcid,'$conteudo')";
	}
	
	$db->executar($sql);
	$db->commit($sql);
}

function exibePesquisaUniversidade($cxcid,$checkbox){
	global $db;?>
	<form method="POST"  name="formulario">
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
			<td colspan=3 bgcolor="#DCDCDC"  align='center' ><b>Selecione a(s) Universidade(s)</b></td>
		</tr>
		<tr>
			<td  align='right' class="SubTituloDireita">Região:</td>
			<? 
			$sql = "select regcod as codigo, regdescricao as descricao from territorios.regiao order by descricao";
			?>
		    <td><?=$db->monta_combo('regcod',$sql,'S','Selecione...','filtraEstado','','','','N',"regcod","",$_POST['regcod']);?></td>
		    <td rowspan=5>
		    <?
			//Lista os indicadores já selecionados
			if($checkbox == "true"){
				echo "<fieldset style=\"background-color:#f5f5f5\">
							<legend>Universidades Selecionadas</legend>
								<div style=\"overflow:auto;width:100%;height:90px\" id=\"universidades_selecionadas\" ></div>
						</fieldset>
						<script>listaUniversidadesSelecionados($cxcid,'{$_SESSION['usucpf']}');</script>
						";
				
			} 
			?>
			</td>
		</tr>
		<tr>
		<?php 
				$sql = "select estuf as codigo, estdescricao as descricao from territorios.estado order by estdescricao";
			?>
			<td  align='right' class="SubTituloDireita">Estado:</td>
		    <td id="exibeEstado" ><?=$db->monta_combo('estuf',$sql,'S','Selecione...','','','','','N',"estuf","",$_POST['estuf']);?></td>
		</tr>
		<tr style="display:none" >
			<?php 
				$sql= "SELECT 
					gtmdsc AS descricao, 
					gtmid AS codigo
				FROM 
					territorios.grupotipomunicipio
				order by
					descricao
				";
			?>
		    <td align='right' class="SubTituloDireita">Grupo de Municípios:</td>
		    <td><?=$db->monta_combo('gtmid',$sql,'S','Selecione...','filtraGrupoMunicipios','','','','N',"gtmid","",$_POST['gtmid']);?></td>
		</tr> 
		<tr style="display:none" >
		<?php 
				$sql= "SELECT 
					tpmdsc AS descricao, 
					tpmid AS codigo
				FROM 
					territorios.tipomunicipio
				order by
					descricao
				";
			?>
			<td  align='right' class="SubTituloDireita">Tipos de Municípios:</td>
		    <td id="exibeTipoMunicipio" ><?=$db->monta_combo('tpmid',$sql,'S','Selecione...','','','','','N',"tpmid","",$_POST['tpmid']);?></td>
		</tr>
		<tr>
		<?php 
				$sql= "SELECT 
					tpmdsc AS descricao, 
					tpmid AS codigo
				FROM 
					territorios.tipomunicipio
				order by
					descricao
				";
			?>
			<td  align='right' class="SubTituloDireita">Município:</td>
		    <td><?=campo_texto($municipio,"N","S",'',60,100,"","","","","","id='municipio'","",$_POST['municipio']) ?></td>
		</tr>
		<tr>
			<td  align='right' class="SubTituloDireita">Universidade:</td>
		    <td><?=campo_texto($unidsc,"N","S",'',60,100,"","","","","","id='unidsc'","",$_POST['unidsc']) ?></td>
		</tr>
		
		<tr bgcolor="#cccccc">
			      <td></td>
			      <td colspan=2>
			  	  	<input type="button" class="botao" name="btassociar" value="Pesquisar" onclick="pesquisarUniversidades(<?=$cxcid ?>,<?=$checkbox ?>);">
			  	  	<input type="button" class="botao" name="bt_limpar" value="Limpar" onclick="limparUniversidades();">
			  	  	<input type="button" class="botao" name="cancelar" value="Cancelar" id="button_cancelar" />
			  	  	
			  	  	<?php if($checkbox == "true"){ ?>
			  	  		<input type="button" class="botao" name="salvar_cima" id="salvar_cima" value="Salvar" />
			  	  	<?php } ?>
			  	  </td>
		 </tr>
		 <tr>
			      <td colspan="3" id="resultado_pesquisa"></td>
		 </tr>
		 
		</table>
	</form>
<?php }

function pesquisaIndicador($cxcid,$checkbox,$link = null){
	global $db;
	
	//filtros
	if($_REQUEST['indid']) {				
		$filtroprocesso[] = "i.indid=".$_REQUEST['indid']."";
	}
	if($_REQUEST['indnome']) {				
		$filtroprocesso[] = "( 
		upper(i.indnome) ilike upper('%".utf8_decode($_REQUEST['indnome'])."%') OR
		upper(a.acadsc) ilike upper('%".utf8_decode($_REQUEST['indnome'])."%') OR
		upper(s.secdsc) ilike upper('%".utf8_decode($_REQUEST['indnome'])."%')
		)";
	}
	if($_REQUEST['cliid']) {
		$filtroprocesso[] = "i.cliid=".$_REQUEST['cliid']."";
	}
	if($_REQUEST['tpiid']) {
		$filtroprocesso[] = "i.tpiid=".$_REQUEST['tpiid']."";
	}
	if($_REQUEST['secid']) {
		$filtroprocesso[] = "i.secid=".$_REQUEST['secid']."";
	}
	if($_REQUEST['acaid']) {
		$filtroprocesso[] = "i.acaid=".$_REQUEST['acaid']."";
	}
	if($_REQUEST['mapid']) {
		$filtroprocesso[] = "obj.mapid=".$_REQUEST['mapid']."";
	}
	if($_REQUEST['pesid']) {
		$filtroprocesso[] = "obj.pesid=".$_REQUEST['pesid']."";
	}
	if($_REQUEST['temid']) {
		$filtroprocesso[] = "obj.temid=".$_REQUEST['temid']."";
	}
	if($_REQUEST['obeid']) {
		$filtroprocesso[] = "obj.obeid=".$_REQUEST['obeid']."";
	}
	if($_REQUEST['regid']) {
		$filtroprocesso[] = "reg.regid=".$_REQUEST['regid']."";
	}
	if($_REQUEST['exoid']) {
		$filtroprocesso[] = "ex.exoid=".$_REQUEST['exoid']."";
	}
	$filtroprocesso[] = "i.indstatus = 'A'" ;
	$filtroprocesso[] = "i.indpublicado is true" ;
	$filtroprocesso[] = "seh.sehstatus <> 'I'" ;
	
	$sql = "SELECT 
				i.indid,
				i.indnome,
				i.acaid,
				unm.unmdesc,
				i.exoid,
				s.secdsc,
				a.acadsc,
				i.indobjetivo,
				ume.umedesc, 
				i.indcumulativo, 
				i.indcumulativovalor,
				reg.regdescricao 
				FROM  painel.indicador	i
				INNER JOIN  painel.seriehistorica seh ON seh.indid = i.indid
				LEFT JOIN  	painel.mapa m ON m.mapid = m.mapid
				LEFT JOIN  	painel.secretaria s ON i.secid = s.secid
				LEFT JOIN  	painel.acao a ON a.acaid = i.acaid
				LEFT JOIN  	painel.eixo ex ON ex.exoid = i.exoid
				LEFT JOIN  	painel.unidademedicao unm ON unm.unmid = i.unmid 
				LEFT JOIN  	painel.unidademeta ume ON ume.umeid = i.umeid
				LEFT JOIN  	painel.regionalizacao reg ON reg.regid = i.regid
				".((count($filtroprocesso) > 0)?"WHERE ".implode(" AND ", $filtroprocesso):"")."	
				GROUP BY 
				i.indid,i.indnome, i.acaid, i.exoid, s.secdsc, i.indobjetivo, acadsc, unm.unmdesc, ume.umedesc, reg.regdescricao, i.indcumulativo,i.indcumulativovalor";
	
	$dados = $db->carregar($sql);
	
	if(!$dados){
		echo "<div style=\"width:100%;color:#990000;text-align:center\">Não existem registros.</div>";
		return false;
	}
	
	//Se a exibição do checkbox for necessária
	if($checkbox == "true"){
		//Pega os indicadores do usuário 
		$meusIndicadores = pegaMeusIndicadores($cxcid);
	}
	
	foreach($dados as $chave => $val){
		// limpando
		unset($de);
		
		$detalhes = $db->carregar("SELECT * FROM painel.detalhetipoindicador WHERE indid='".$val['indid']."'");
		if($detalhes[0]) {
			foreach($detalhes as $det) {
				$de[] = "Por ".$det['tdidsc'];
			}
		}
		
		$dados_array[$chave] = array(
										 "indicador" => $val['indid'], 
										 "programa" => $val['indnome'],
										 "secretaria" => $val['secdsc'], 
										 "acao" => $val['acadsc'],
										 "indcumulativo" => ( ($val['indcumulativo']=="S") ? "Sim" : ( $val['indcumulativo']=="N" ? "Não" : "Por Ano" )  ),
										 "produto" => $val['umedesc'], 
										 "unidade" => $val['unmdesc'],
										 "regionalizacao" => $val['regdescricao'],
										 "detalhe" => (($de)?implode(" ; ",$de):"N/A"),
										 "descricao" => $val['indobjetivo']);
		
	}
	foreach($dados_array as $arrI){
		$arrInd[] = $arrI['indicador'];
	}
	echo "<table cellSpacing=\"1\" cellPadding=\"3\" width=\"100%\" >";
	
	if($checkbox == "true"){
		$marcarTodos = "<input type=\"hidden\" name=\"arrIndid\" id=\"arrIndid\" value=\"".implode(",",$arrInd)."\" > <input type=\"checkbox\" id=\"marca_todos\" name=\"marca_todos\" onclick=\"marcarTodos({$cxcid},'{$_SESSION['usucpf']}')\" />";	
	}
	
	echo "<tr bgcolor=\"#DCDCDC\" ><td width=\"5%\"> $marcarTodos <b>Cód.</b></td><td><b>Indicador</b></td><td><b>Secretaria</b></td><td><b>Ação</b></td><td><b>Cumulativo</b></td><td><b>Produto</b></td><td><b>Unidade</b></td><td><b>Regionalização</b></td><td><b>Detalhe</b></td><td><b>Descrição</b></td><tr>";
	
	$num = 0;
	foreach($dados_array as $arr){
		
		if($checkbox != "true"){
			if($cxcid == 0)
				$onclick = " onclick=\"exibeDashBoard({$arr['indicador']})\" ";
			else
				$onclick = " onclick=\"selecionaGraficoIndicador({$cxcid},{$arr['indicador']})\" ";
			$ckeck = "";
		}else{
			$onclick = "";
			$ckeck = "<input type=\"checkbox\" ".(in_array($arr['indicador'],$meusIndicadores)? "checked='checked'" : "")." id=\"meu_indicador_{$arr['indicador']}\" name=\"meu_indicador_{$arr['indicador']}\" onclick=\"addMeusIndicadores(this.id,{$arr['indicador']},{$cxcid});listaIndicadoresSelecionados({$cxcid},'{$_SESSION['usucpf']}')\" />";
		}
		
		$cor = ($num%2)? "#F9F9F9" : "";
		echo "<tr style=\"cursor:pointer\" $onclick onmouseout=\"this.bgColor='$cor';\" onmouseover=\"this.bgColor='#ffffcc';\" bgcolor=\"$cor\"><td>$ckeck {$arr['indicador']}</td><td>{$arr['programa']}</td><td>{$arr['secretaria']}</td><td>{$arr['acao']}</td><td>{$arr['indcumulativo']}</td><td>{$arr['produto']}</td><td>{$arr['unidade']}</td><td>{$arr['regionalizacao']}</td><td>{$arr['detalhe']}</td><td>{$arr['descricao']}</td><tr>";
		$num++;
	}
	
	echo "</table>";
}


function pesquisaMunicipio($cxcid,$checkbox){
	global $db;
	
	//filtros
	if($_REQUEST['regcod']) {				
		$filtroprocesso[] = "r.regcod='".$_REQUEST['regcod']."'";
	}
	if($_REQUEST['municipio']) {				
		$filtroprocesso[] = " 
		upper(m.mundescricao) ilike upper('%".utf8_decode($_REQUEST['municipio'])."%')";
	}
	if($_REQUEST['estuf']) {
		$filtroprocesso[] = "e.estuf='".$_REQUEST['estuf']."'";
	}
	if($_REQUEST['gtmid']) {
		$filtroprocesso[] = "g.gtmid='".$_REQUEST['gtmid']."'";
	}
	if($_REQUEST['tpmid']) {
		$filtroprocesso[] = "t.tpmid='".$_REQUEST['tpmid']."'";
	}
	$filtroprocesso[] = "t.tpmstatus = 'A'" ;
	$filtroprocesso[] = "g.gtmstatus = 'A'" ;
	
	$sql = "SELECT 
				m.mundescricao,
				m.muncod,
				e.estdescricao
				FROM  territorios.municipio	m
				LEFT JOIN  territorios.estado e ON e.estuf = m.estuf
				LEFT JOIN  	territorios.regiao r ON r.regcod = e.regcod
				LEFT JOIN  	territorios.muntipomunicipio mt ON mt.muncod = m.muncod
				LEFT JOIN  	territorios.tipomunicipio t ON t.tpmid = mt.tpmid
				LEFT JOIN  	territorios.grupotipomunicipio g ON g.gtmid = t.gtmid
				".((count($filtroprocesso) > 0)?"WHERE ".implode(" AND ", $filtroprocesso):"")."	
			GROUP BY 
				m.mundescricao,m.muncod,e.estdescricao
			ORDER BY 
				e.estdescricao,m.mundescricao";
	
	$dados = $db->carregar($sql);
	
	if(!$dados){
		echo "<div style=\"width:100%;color:#990000;text-align:center\">Não existem registros.</div>";
		return false;
	}
	
	//Se a exibição do checkbox for necessária
	if($checkbox == "true"){
		//Pega os indicadores do usuário
		$sql = "select conteudo from painel.conteudocaixa where cxcid = $cxcid";
		$meusMunicipios = unserialize($db->pegaUm($sql));
		$meusMunicipios = !$meusMunicipios? array() : $meusMunicipios;
	}
	
	foreach($dados as $chave => $val){
		
		$dados_array[$chave] = array(
										 "estado" => $val['estdescricao'], 
										 "municipio" => $val['mundescricao'],
										 "muncod" => $val['muncod']);
		
	}
	foreach($dados_array as $arrM){
		$arrMuncod[] = $arrM['muncod'];
	}
	echo "<table cellSpacing=\"1\" cellPadding=\"3\" width=\"100%\" >";
	
	if($checkbox == "true"){
		$marcarTodos = "<input type=\"hidden\" name=\"arrMuncod\" id=\"arrMuncod\" value=\"".implode(",",$arrMuncod)."\" > <input type=\"checkbox\" id=\"marca_todos\" name=\"marca_todos\" onclick=\"marcarTodosMunicipios({$cxcid},'{$_SESSION['usucpf']}');listaMunicipiosSelecionados({$cxcid},'{$_SESSION['usucpf']}')\" />";	
	}
	
	echo "<tr bgcolor=\"#DCDCDC\" ><td width=\"5%\"> $marcarTodos</td><td><b>Município</b></td><td><b>Estado</b></td><tr>";
	
	$num = 0;
	foreach($dados_array as $arr){
		
		if($checkbox != "true"){
			$onclick = " onclick=\"selecionaGraficoEstado({$cxcid},{$arr['muncod']})\" ";
			$ckeck = "";
		}else{
			$onclick = "";
			$ckeck = "<input type=\"checkbox\" ".(in_array($arr['muncod'],$meusMunicipios)? "checked='checked'" : "")." id=\"meu_muncod_{$arr['muncod']}\" name=\"meu_muncod_{$arr['muncod']}\" onclick=\"addMeusMuncod(this.id,{$arr['muncod']},{$cxcid});listaMunicipiosSelecionados({$cxcid},'{$_SESSION['usucpf']}')\" />";
		}
		
		$cor = ($num%2)? "#F9F9F9" : "";
		echo "<tr style=\"cursor:pointer\" $onclick onmouseout=\"this.bgColor='$cor';\" onmouseover=\"this.bgColor='#ffffcc';\" bgcolor=\"$cor\"><td>$ckeck</td><td>{$arr['municipio']}</td><td>{$arr['estado']}</td><tr>";
		$num++;
	}
	
	echo "</table>";
}

function pesquisaUniversidade($cxcid,$checkbox){
	global $db;
	
	//filtros
	if($_REQUEST['regcod']) {				
		$filtroprocesso[] = " ende.estuf in ( select estuf from territorios.estado where regcod = '".$_REQUEST['regcod']."' ) ";
	}
	if($_REQUEST['municipio']) {				
		$filtroprocesso[] = " 
		ende.muncod in (select muncod from territorios.municipio where upper(mundescricao) ilike upper('%".utf8_decode($_REQUEST['municipio'])."%') ) ";
	}
	if($_REQUEST['unidsc']) {				
		$filtroprocesso[] = " 
		upper(uni.unidsc) ilike upper('%".utf8_decode($_REQUEST['unidsc'])."%') ";
	}
	if($_REQUEST['estuf']) {
		$filtroprocesso[] = " ende.estuf ='".$_REQUEST['estuf']."' ";
	}
	/*
	if($_REQUEST['gtmid']) {
		$filtroprocesso[] = "g.gtmid='".$_REQUEST['gtmid']."'";
	}
	if($_REQUEST['tpmid']) {
		$filtroprocesso[] = "t.tpmid='".$_REQUEST['tpmid']."'";
	}*/
		
	$sql = "SELECT uni.unicod as codigo, UPPER(uni.unidsc) as descricao FROM public.unidade uni 
		  INNER JOIN entidade.entidade ent ON ent.entunicod = uni.unicod 
		  INNER JOIN entidade.endereco ende ON ende.entid = ent.entid 
		  WHERE uni.orgcod= '". CODIGO_ORGAO_SISTEMA. "' AND gunid=3 AND unistatus='A' ".((count($filtroprocesso) > 0)?"AND ".implode(" AND ", $filtroprocesso):"")." ORDER BY descricao";
	//dbg($sql);
	//die;
	/*$sql = "SELECT 
				m.mundescricao,
				m.muncod,
				e.estdescricao
				FROM  territorios.municipio	m
				LEFT JOIN  territorios.estado e ON e.estuf = m.estuf
				LEFT JOIN  	territorios.regiao r ON r.regcod = e.regcod
				LEFT JOIN  	territorios.muntipomunicipio mt ON mt.muncod = m.muncod
				LEFT JOIN  	territorios.tipomunicipio t ON t.tpmid = mt.tpmid
				LEFT JOIN  	territorios.grupotipomunicipio g ON g.gtmid = t.gtmid
				".((count($filtroprocesso) > 0)?"WHERE ".implode(" AND ", $filtroprocesso):"")."	
			GROUP BY 
				m.mundescricao,m.muncod,e.estdescricao
			ORDER BY 
				e.estdescricao,m.mundescricao";
	*/
	$dados = $db->carregar($sql);
	
	if(!$dados){
		echo "<div style=\"width:100%;color:#990000;text-align:center\">Não existem registros.</div>";
		return false;
	}
	
	//Se a exibição do checkbox for necessária
	if($checkbox == "true"){
		//Pega os indicadores do usuário
		$sql = "select conteudo from painel.conteudocaixa where cxcid = $cxcid";
		$meusMunicipios = unserialize($db->pegaUm($sql));
		$meusMunicipios = !$meusMunicipios? array() : $meusMunicipios;
	}
	
	foreach($dados as $chave => $val){
		
		$dados_array[$chave] = array(
										 "codigo" => $val['codigo'], 
										 "descricao" => $val['descricao']);
		
	}
	foreach($dados_array as $arrM){
		$arrMuncod[] = $arrM['codigo'];
	}
	echo "<table cellSpacing=\"1\" cellPadding=\"3\" width=\"100%\" >";
	
	if($checkbox == "true"){
		$marcarTodos = "<input type=\"hidden\" name=\"arrMuncod\" id=\"arrMuncod\" value=\"".implode(",",$arrMuncod)."\" > <input type=\"checkbox\" id=\"marca_todos\" name=\"marca_todos\" onclick=\"marcarTodosMunicipios({$cxcid},'{$_SESSION['usucpf']}');listaUniversidadesSelecionados({$cxcid},'{$_SESSION['usucpf']}')\" />";	
	}
	
	echo "<tr bgcolor=\"#DCDCDC\" ><td width=\"5%\"> $marcarTodos</td><td><b>Código</b></td><td><b>Universidade</b></td><tr>";
	
	$num = 0;
	foreach($dados_array as $arr){
		
		if($checkbox != "true"){
			$onclick = " onclick=\"selecionaGraficoEstado({$cxcid},{$arr['codigo']})\" ";
			$ckeck = "";
		}else{
			$onclick = "";
			$ckeck = "<input type=\"checkbox\" ".(in_array($arr['codigo'],$meusMunicipios)? "checked='checked'" : "")." id=\"meu_muncod_{$arr['codigo']}\" name=\"meu_muncod_{$arr['codigo']}\" onclick=\"addMeusMuncod(this.id,{$arr['codigo']},{$cxcid});listaUniversidadesSelecionados({$cxcid},'{$_SESSION['usucpf']}')\" />";
		}
		
		$cor = ($num%2)? "#F9F9F9" : "";
		echo "<tr style=\"cursor:pointer\" $onclick onmouseout=\"this.bgColor='$cor';\" onmouseover=\"this.bgColor='#ffffcc';\" bgcolor=\"$cor\"><td>$ckeck</td><td>{$arr['codigo']}</td><td>{$arr['descricao']}</td><tr>";
		$num++;
	}
	
	echo "</table>";
}



//Inclui todos os indicadores Marcados
function marcarTodos($cxcid,$usucpf,$arrIndid){
	global $db;
	
	//Atualiza a data de modificação da data
	$sql = "select 
				abaid 
			from 
				painel.conteudousuario con
			inner join
				painel.caixaconteudo cxc ON cxc.ctuid = con.ctuid
			where
				cxcid = $cxcid
			limit 1";
	$abaid = $db->pegaUm($sql);
	if($abaid){
		$sql = "update painel.aba set abadata = now() where abaid = $abaid";
		$db->executar($sql);
	}
	
	//Excluir todos os indicadores p/ não duplicar
	$sql1 = "delete from painel.indicadorusuario where indid in({$arrIndid}) and usucpf = '{$usucpf}' and cxcid = $cxcid";
	$db->executar($sql1);
	
	$arrInd = explode(",",$arrIndid);
	
	foreach($arrInd as $ind){
		$sql2 .= " insert into painel.indicadorusuario (usucpf,indid,cxcid) values ('{$usucpf}',{$ind},{$cxcid}); ";
	}
	$db->executar($sql2);
	$db->commit();
}

//Exclui todos os indicadores Marcados
function desmarcarTodos($cxcid,$usucpf,$arrIndid){
	
	//Atualiza a data de modificação da data
	$sql = "select 
				abaid 
			from 
				painel.conteudousuario con
			inner join
				painel.caixaconteudo cxc ON cxc.ctuid = con.ctuid
			where
				cxcid = $cxcid
			limit 1";
	$abaid = $db->pegaUm($sql);
	if($abaid){
		$sql = "update painel.aba set abadata = now() where abaid = $abaid";
		$db->executar($sql);
	}
	
	global $db;
	//Excluir todos os indicadores p/ não duplicar
	$sql = "delete from painel.indicadorusuario where indid in({$arrIndid}) and usucpf = '{$usucpf}' and cxcid = $cxcid";
	$db->executar($sql);
	$db->commit($sql);
}

//Lista os indicadores já selecionados pelo usuário
function listaIndicadoresSelecionados($cxcid,$usucpf){
	global $db;

	$sql = "select 
				ind.indnome,ind.indid
			from
				painel.indicadorusuario indusu
			inner join
				painel.indicador ind ON indusu.indid = ind.indid
			where
				indusu.cxcid = $cxcid
			and
				indusu.usucpf = '$usucpf'";
	$indUsu = $db->carregar($sql);
	if(!$indUsu){
		echo "Não existem indicadores selecionados.";
		
	}else{
		echo "<table cellSpacing=\"1\" cellPadding=\"3\" width=\"100%\" >";
			echo "<tr bgcolor=\"#DCDCDC\" ><td><b>Cód.</b></td><td><b>Indicador</b></td><tr>";
			$num = 0;
			foreach($indUsu as $arr){
				$cor = ($num%2)? "#F9F9F9" : "";
				echo "<tr style=\"cursor:pointer\" onmouseout=\"this.bgColor='$cor';\" onmouseover=\"this.bgColor='#ffffcc';\" bgcolor=\"$cor\"><td>{$arr['indid']}</td><td>{$arr['indnome']}</td><tr>";
				$num++;
			}
		echo "</table>";
	}
	
}

function listaMunicipiosSelecionados($cxcid,$usucpf){
	global $db;
	
	$sql = "select conteudo from painel.conteudocaixa where cxcid = $cxcid";
	$conteudo = $db->pegaUm($sql);
	$conteudo = !$conteudo ? array() : unserialize($conteudo);
	$sql = "select 
				m.mundescricao,e.estdescricao
			from
				territorios.municipio m
			inner join
				territorios.estado e ON e.estuf = m.estuf
			where
				m.muncod in('".implode("','",$conteudo)."')";
	$muncod = $db->carregar($sql);
	if(!$muncod){
		echo "Não existem municípios selecionados.";
		
	}else{
		echo "<table cellSpacing=\"1\" cellPadding=\"3\" width=\"100%\" >";
			echo "<tr bgcolor=\"#DCDCDC\" ><td><b>Município.</b></td><td><b>Estado</b></td><tr>";
			$num = 0;
			foreach($muncod as $arr){
				$cor = ($num%2)? "#F9F9F9" : "";
				echo "<tr style=\"cursor:pointer\" onmouseout=\"this.bgColor='$cor';\" onmouseover=\"this.bgColor='#ffffcc';\" bgcolor=\"$cor\"><td>{$arr['mundescricao']}</td><td>{$arr['estdescricao']}</td><tr>";
				$num++;
			}
		echo "</table>";
	}
	
}

function listaUniversidadesSelecionados($cxcid,$usucpf){
	global $db;
	
	$sql = "select conteudo from painel.conteudocaixa where cxcid = $cxcid";
	$conteudo = $db->pegaUm($sql);
	$conteudo = !$conteudo ? array() : unserialize($conteudo);
	
	$sql = "SELECT uni.unicod as codigo, UPPER(uni.unidsc) as descricao FROM public.unidade uni 
				  INNER JOIN entidade.entidade ent ON ent.entunicod = uni.unicod 
				  INNER JOIN entidade.endereco ende ON ende.entid = ent.entid 
				  WHERE 
					uni.unicod in ('".implode("' ,'",$conteudo)."')
				AND 
					uni.orgcod= '". CODIGO_ORGAO_SISTEMA. "' AND gunid=3 AND unistatus='A' ORDER BY descricao";
	
	$muncod = $db->carregar($sql);
	if(!$muncod){
		echo "Não existem universidades selecionadas.";
		
	}else{
		echo "<table cellSpacing=\"1\" cellPadding=\"3\" width=\"100%\" >";
			echo "<tr bgcolor=\"#DCDCDC\" ><td><b>Código.</b></td><td><b>Universidade</b></td><tr>";
			$num = 0;
			foreach($muncod as $arr){
				$cor = ($num%2)? "#F9F9F9" : "";
				echo "<tr style=\"cursor:pointer\" onmouseout=\"this.bgColor='$cor';\" onmouseover=\"this.bgColor='#ffffcc';\" bgcolor=\"$cor\"><td>{$arr['codigo']}</td><td>{$arr['descricao']}</td><tr>";
				$num++;
			}
		echo "</table>";
	}
	
}

function selecionaGraficoIndicador($cxcid,$indid){
	global $db;
	
	$sql = "select indnome from painel.indicador where indid = $indid";
	$indnome = $db->pegaUm($sql);
	
	$sql = "select 
				tdiid,
				tdidsc
			from
				painel.detalhetipoindicador
			where
				indid = $indid
			and
				tdistatus = 'A'";
	$det = $db->carregar($sql);
	
	//Carrega o período do indicador
	$sql = "select  
				dpe.dpeid,
				dpe.dpedsc,
				seh.sehid
			from 
				painel.seriehistorica seh
			inner join
				painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
			where
				indid = $indid
			and
				sehstatus <> 'I'
			order by
				dpe.dpedatainicio desc";
	$per = $db->carregar($sql);
	$dpeidFim = $per[0]['dpeid'];
	$count = count( $per ) >= 12 ? 6 : count( $per ) - 1;
	$count = $count <= 0 ? 0 : $count; 
	$dpeidInicio = $per[ $count ]['dpeid'];
	
	/*
	//Pega a última série histórica e fatia pelo detalhamento
	$sql = "select 
				sehid,
				tdidsc1,
				tdiid1,
				tdidsc2,
				tdiid2,
				dpeid,
				dpedsc 
			from 
				painel.v_detalheindicadorsh 
			where 
				indid = $indid 
			order by 
				data desc 
			limit 1";
	$seh = $db->pegaLinha($sql);
	//dbg($sql);
	//dbg($seh);*/
	
	
	?>
	
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td  align='right' class="SubTituloDireita">Indicador:</td>
		    <td><?=$indnome; ?></td>
		</tr>
		<tr>
			<td colspan=2 bgcolor="#DCDCDC"  align='center' ><b>Selecione o tipo de Gráfico</b></td>
		</tr>
		<tr>
			<td colspan=2 align='center' >
			<table cellSpacing="1" cellPadding="3" width="100%">
				<tr>
					<td valign="top">
						<script type="text/javascript">
						swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_barra", "100%", "220", "9.0.0", "expressInstall.swf", {"data-file":"geraGrafico.php?tipo=barra;indid=<?php echo $indid ?>;dpeid1=<?php echo $dpeidInicio ?>;dpeid2=<?php echo $dpeidFim ?>","loading":"Carregando gráfico..."} );
						</script>
						<div onclick="selecionaTipoGrafico(<?=$cxcid;?>,<?=$indid;?>,'barra');" style="font-width:bold;font-size:16px;cursor:pointer;border:solid 1px #000000;text-align:center;padding:5px;background-color: #FFFFFF" >
							<input type="checkbox" name="tipo_grafico_barra" /> Tipo Barra <br />
							<div id="grafico_barra"></div>
						</div>
					</td>
					<td valign="top">
						<script type="text/javascript">
						swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_linha", "100%", "220", "9.0.0", "expressInstall.swf", {"data-file":"geraGrafico.php?tipo=linha;indid=<?php echo $indid ?>;dpeid1=<?php echo $dpeidInicio ?>;dpeid2=<?php echo $dpeidFim ?>","loading":"Carregando gráfico..."} );
						</script>
						<div onclick="selecionaTipoGrafico(<?=$cxcid;?>,<?=$indid;?>,'linha');" style="font-width:bold;font-size:16px;cursor:pointer;border:solid 1px #000000;text-align:center;padding:5px;background-color: #FFFFFF" >
							<input type="checkbox" name="tipo_grafico_linha" /> Tipo Linha <br/>
							<div id="grafico_linha"></div>
						</div>
					</td>
				</tr>
				
				<?php 
				
				if($det[0]['tdiid'] && $det[0]['tdiid'] != ""){
				//Exibe Grafico Barra da Fatia 1?>
				<tr>
					<td valign="top">
							<script type="text/javascript">
							swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_barra_fatia_1", "100%", "220", "9.0.0", "expressInstall.swf", {"data-file":"geraGrafico.php?tipo=barra_fatia;indid=<?php echo $indid ?>;sehid=0;tidid=tdiid1_<? echo $det[0]['tdiid'] ?>;dpeid1=<?php echo $dpeidInicio ?>;dpeid2=<?php echo $dpeidFim ?>","loading":"Carregando gráfico..."} );
							</script>
							<div onclick="selecionaTipoGrafico(<?=$cxcid;?>,<?=$indid;?>,'barra_1_<? echo $det[0]['tdiid'] ?>');" style="font-width:bold;font-size:16px;cursor:pointer;border-top:solid 1px #000000;border-left:solid 1px #000000;border-right:solid 1px #000000;text-align:center;padding:5px;background-color: #FFFFFF" >
								<input type="checkbox" name="grafico_barra_fatia_01" /> Tipo Barra por <? echo $det[0]['tdidsc'] ?><br/>							
								<div id="grafico_barra_fatia_1"></div>
							</div>
							
							<? 
									echo "<div style=\"clear:both;background-color:#FFFFFF;border-left:solid 1px #000000;border-bottom:solid 1px #000000;border-right:solid 1px #000000;padding-bottom:5px;\" >";
									exibeLegendaGrafico($indid,false,1,$det[0]['tdiid']);
									echo "</div>";					
							?>
							
					</td>
					
					<td valign="top">
							<script type="text/javascript">
								swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_pizza_fatia_1", "100%", "220", "9.0.0", "expressInstall.swf", {"data-file":"geraGrafico.php?tipo=pizza;indid=<?php echo $indid ?>;sehid=<?php echo $per[0]['sehid']; ?>;tidid=tdiid1_<? echo $det[0]['tdiid'] ?>","loading":"Carregando gráfico..."} );
							</script>
							<div onclick="selecionaTipoGrafico(<?=$cxcid;?>,<?=$indid;?>,'pizza_1_<? echo $det[0]['tdiid'] ?>');" style="font-width:bold;font-size:16px;cursor:pointer;border-top:solid 1px #000000;border-left:solid 1px #000000;border-right:solid 1px #000000;text-align:center;padding:5px;background-color: #FFFFFF" >
									<input type="checkbox" name="grafico_pizza_fatia_01" /> Tipo Pizza por <? echo $det[0]['tdidsc'] ?> em <? echo $per[0]['dpedsc'] ?><br/>
									<div id="grafico_pizza_fatia_1"></div>
								
							</div>
							<? 
									echo "<div style=\"clear:both;background-color:#FFFFFF;border-left:solid 1px #000000;border-bottom:solid 1px #000000;border-right:solid 1px #000000;padding-bottom:5px;\" >";
									exibeLegendaGrafico($indid,$per[0]['sehid'],1,$det[0]['tdiid']);
									echo "</div>";
							?>
							
					</td>
					</tr>
					
				
				<? }
				if($det[1]['tdiid'] && $det[1]['tdiid'] != ""){
				//Exibe Grafico Barra da Fatia 2?>
				<tr>
					<td valign="top">
							<script type="text/javascript">
							swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_barra_fatia_2", "100%", "220", "9.0.0", "expressInstall.swf", {"data-file":"geraGrafico.php?tipo=barra_fatia;indid=<?php echo $indid ?>;sehid=0;tidid=tdiid2_<? echo $det[1]['tdiid'] ?>;dpeid1=<?php echo $dpeidInicio ?>;dpeid2=<?php echo $dpeidFim ?>","loading":"Carregando gráfico..."} );
							</script>
							<div onclick="selecionaTipoGrafico(<?=$cxcid;?>,<?=$indid;?>,'barra_2_<? echo $det[1]['tdiid'] ?>');" style="font-width:bold;font-size:16px;cursor:pointer;border-top:solid 1px #000000;border-left:solid 1px #000000;border-right:solid 1px #000000;text-align:center;padding:5px;background-color: #FFFFFF" >
								<input type="checkbox" name="grafico_barra_fatia_02" /> Tipo Barra por <? echo $det[1]['tdidsc'] ?><br/>
								<div id="grafico_barra_fatia_2"></div>
							</div>
							
							<? 
									echo "<div style=\"clear:both;background-color:#FFFFFF;border-left:solid 1px #000000;border-bottom:solid 1px #000000;border-right:solid 1px #000000;padding-bottom:5px;\" >";
									exibeLegendaGrafico($indid,false,2,$det[1]['tdiid']);
									echo "</div>";					
							?>
							
					</td>
					
					<td valign="top">
							<script type="text/javascript">
							swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_pizza_fatia_2", "100%", "220", "9.0.0", "expressInstall.swf", {"data-file":"geraGrafico.php?tipo=pizza;indid=<?php echo $indid ?>;sehid=<?php echo $per[0]['sehid']; ?>;tidid=tdiid2_<? echo $det[1]['tdiid'] ?>","loading":"Carregando gráfico..."} );
							</script>
							<div onclick="selecionaTipoGrafico(<?=$cxcid;?>,<?=$indid;?>,'pizza_2_<? echo $det[1]['tdiid'] ?>');" style="font-width:bold;font-size:16px;cursor:pointer;border-top:solid 1px #000000;border-left:solid 1px #000000;border-right:solid 1px #000000;text-align:center;padding:5px;background-color: #FFFFFF" >
								<input type="checkbox" name="grafico_pizza_fatia_02" /> Tipo Pizza por <? echo $det[1]['tdidsc'] ?> em <? echo $per[0]['dpedsc'] ?><br/>
								<div id="grafico_pizza_fatia_2"></div>
							</div>
							
							<? 
									echo "<div style=\"clear:both;background-color:#FFFFFF;border-left:solid 1px #000000;border-bottom:solid 1px #000000;border-right:solid 1px #000000;padding-bottom:5px;\" >";
									exibeLegendaGrafico($indid,$per[0]['sehid'],2,$det[1]['tdiid']);
									echo "</div>";
							?>
							
					</td>
					</tr>
				
				<? } ?>
				
				<?
				if($det[0]['tdiid'] && $det[0]['tdiid'] != ""){
				//Exibe Grafico Barra da Fatia 1
				//geraGrafico.php?tipo=390;barra_comp;24346;tdiid1_141;150;300;null;1;null;todos;todos;todos;todos;;todos
					?>
				<tr>
					<td valign="top" >
							<script type="text/javascript">
							swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_barra_comp_1", "100%", "220", "9.0.0", "expressInstall.swf", {"data-file":"geraGrafico.php?tipo=barra_comp;indid=<?php echo $indid ?>;sehid=0;tidid=tdiid1_<? echo $det[0]['tdiid'] ?>;dpeid1=<?php echo $dpeidInicio ?>;dpeid2=<?php echo $dpeidFim ?>","loading":"Carregando gráfico..."} );
							</script>
							<div onclick="selecionaTipoGrafico(<?=$cxcid;?>,<?=$indid;?>,'barracomp_1_<? echo $det[0]['tdiid'] ?>');" style="font-width:bold;font-size:16px;cursor:pointer;border-right:solid 1px #000000;border-left:solid 1px #000000;border-top:solid 1px #000000;text-align:center;padding:5px;background-color: #FFFFFF" >
								<input type="checkbox" name="grafico_barra_comp_01" /> Tipo Barra Comparativa por <? echo $det[0]['tdidsc'] ?><br/>
								<div id="grafico_barra_comp_1"></div>
							</div>
							
							<? 
									echo "<div style=\"clear:both;background-color:#FFFFFF;border-left:solid 1px #000000;border-bottom:solid 1px #000000;border-right:solid 1px #000000;padding-bottom:5px;\" >";
									exibeLegendaGrafico($indid,false,1,$det[0]['tdiid']);
									echo "</div>";
							?>
					</td>
				
				<?}
				if($det[1]['tdiid'] && $det[1]['tdiid'] != ""){
				//Exibe Grafico Barra da Fatia 2?>
					<td valign="top" >
							<script type="text/javascript">
							swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_barra_comp_2", "100%", "220", "9.0.0", "expressInstall.swf", {"data-file":"geraGrafico.php?tipo=barra_comp;indid=<?php echo $indid ?>;sehid=0;tidid=tdiid2_<? echo $det[1]['tdiid'] ?>;dpeid1=<?php echo $dpeidInicio ?>;dpeid2=<?php echo $dpeidFim ?>","loading":"Carregando gráfico..."} );
							</script>
							<div onclick="selecionaTipoGrafico(<?=$cxcid;?>,<?=$indid;?>,'barracomp_2_<? echo $det[1]['tdiid'] ?>');" style="font-width:bold;font-size:16px;cursor:pointer;border-right:solid 1px #000000;border-left:solid 1px #000000;border-top:solid 1px #000000;text-align:center;padding:5px;background-color: #FFFFFF" >
								<input type="checkbox" name="grafico_barra_comp_02" /> Tipo Barra Comparativa por <? echo $det[1]['tdidsc'] ?><br/>
								<div id="grafico_barra_comp_2"></div>
							</div>
							
							<? 
									echo "<div style=\"clear:both;background-color:#FFFFFF;border-left:solid 1px #000000;border-bottom:solid 1px #000000;border-right:solid 1px #000000;padding-bottom:5px;\" >";
									exibeLegendaGrafico($indid,false,2,$det[1]['tdiid']);
									echo "</div>";							
							?>
					</td>
				</tr>
					
				<? } ?>
			</table>
			</td>
				</tr>
				<tr bgcolor="#cccccc">
					      <td colspan="2" align="center">
					  	  	<input type="button" class="botao" name="cancelar" value="Cancelar" id="button_cancelar" />
					  	  </td>
				 </tr>
		</table>
	
<?php }

function geraGraficoIndicador($cxcid,$indid,$tipo,$dpeid = null,$sehid = null ,$tidid = null){
	global $db;
	
	$sehid = $sehid == "undefined" ? "null" : $sehid;
	if($tidid != "undefined"){
		$dados = explode("_",$tidid);
		$tipotidid = str_replace("tidid","",$dados[0]);
		$valortidid = $dados[1];
	}else{
		$tipotidid = "null";
		$valortidid = "null";
	}
	$dpeid = !$dpeid || $dpeid == "undefined" ? "null" : $dpeid;
	
	$sql = "select tcgid from painel.tipoconteudografico where cxcid = $cxcid and usucpf = '{$_SESSION['usucpf']}' and tcgstatus = 'A'";
	$tcgid =$db->pegaUm($sql);
	if($tcgid){
		 $sql = "update 
		 			painel.tipoconteudografico
		 		set
		 			indid = $indid,
		 			tpgrafico = '$tipo',
		 			sehid = $sehid,
		 			tipotidid = $tipotidid,
		 			valortidid = $valortidid,
		 			dpeid = $dpeid
		 		where
		 			cxcid = $cxcid"; 
	}else{
		$sql = "insert into painel.tipoconteudografico
					(usucpf,cxcid,indid,tpgrafico,tcgstatus,sehid,tipotidid,valortidid,dpeid)
				values
					('{$_SESSION['usucpf']}',$cxcid,$indid,'$tipo','A',$sehid,$tipotidid,$valortidid,$dpeid)"; //adicionar opções de escala, aplicação de cálculos, etc
	}
	$db->executar($sql);
	$db->commit();	
}

function excluirBox($cxcid){
	global $db;
	
	//Atualiza a data de modificação da data
	$sql = "select 
				abaid 
			from 
				painel.conteudousuario con
			inner join
				painel.caixaconteudo cxc ON cxc.ctuid = con.ctuid
			where
				cxcid = $cxcid
			limit 1";
	$abaid = $db->pegaUm($sql);
	if($abaid){
		$sql = "update painel.aba set abadata = now() where abaid = $abaid";
		$db->executar($sql);
	}
	
	$sql = "update painel.caixaconteudo set cxcstatus = 'I' where cxcid = $cxcid";
	$db->executar($sql);
	$db->commit($sql);	
}


/**
 * Função que adiciona / remove o indicador da tabela painel.indicadores_favoritos
 * 
 * @author Juliano Meinen
 * @return false
 * @param string,integer (ação (adiciona / remove), id do indicador)
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 29/06/2009
 */
function AddMeusIndicadores($acao,$indid,$cxcid){
	global $db;
	
	//Atualiza a data de modificação da data
	$sql = "select 
				abaid 
			from 
				painel.conteudousuario con
			inner join
				painel.caixaconteudo cxc ON cxc.ctuid = con.ctuid
			where
				cxcid = $cxcid
			limit 1";
	$abaid = $db->pegaUm($sql);
	if($abaid){
		$sql = "update painel.aba set abadata = now() where abaid = $abaid";
		$db->executar($sql);
	}
	
	if(($acao != "adiciona" && $acao != "remove") || !$indid || !$_SESSION['usucpf']){
		echo "erro";
		return false;
	}else{
		if($acao == "adiciona"){
			$sql = "delete from 
						painel.indicadorusuario
					where
						usucpf = '{$_SESSION['usucpf']}'
					and
						indid = $indid
					and
						cxcid = $cxcid";
			$db->executar($sql);
			$sql = "insert into
						painel.indicadorusuario
					(usucpf,indid,cxcid) values
						('{$_SESSION['usucpf']}',$indid,$cxcid)";
		}else{
			$sql = "delete from 
						painel.indicadorusuario
					where
						usucpf = '{$_SESSION['usucpf']}'
					and
						indid = $indid
					and
						cxcid = $cxcid";
		}
		
		if(!$db->executar($sql)){
			echo "erro";
			return false;
		}else{
			$db->commit($sql);
			echo "ok";
			return true;
		}
		
	}
	
}

function AddMeusMuncod($acao,$cxcid,$muncod){
	global $db;
	
	//Atualiza a data de modificação da data
	$sql = "select 
				abaid 
			from 
				painel.conteudousuario con
			inner join
				painel.caixaconteudo cxc ON cxc.ctuid = con.ctuid
			where
				cxcid = $cxcid
			limit 1";
	$abaid = $db->pegaUm($sql);
	if($abaid){
		$sql = "update painel.aba set abadata = now() where abaid = $abaid";
		$db->executar($sql);
	}
	
	//Atualiza a data de modificação da data
	$sql = "select 
				abaid 
			from 
				painel.conteudousuario con
			inner join
				painel.caixaconteudo cxc ON cxc.ctuid = con.ctuid
			where
				cxcid = $cxcid
			limit 1";
	$abaid = $db->pegaUm($sql);
	if($abaid){
		$sql = "update painel.aba set abadata = now() where abaid = $abaid";
		$db->executar($sql);
	}
	
	$arrMunicipios = explode(",",$muncod);
	
	if(($acao != "adiciona" && $acao != "remove") || !$muncod || !$_SESSION['usucpf']){
		echo "erro";
		return false;
	}else{
		
		//Verifica se exite o conteudo
		$sql = "select conteudo from painel.conteudocaixa where cxcid = $cxcid";
		$conteudo = unserialize($db->pegaUm($sql));
		if($acao == "adiciona"){
			if(is_array($conteudo)){
				
				foreach($arrMunicipios as $muni){
					array_push($conteudo,$muni);
				}
				
				$sql = "update painel.conteudocaixa set conteudo = '".serialize($conteudo)."' where cxcid = $cxcid";
			}else{
				$conteudo = serialize($arrMunicipios);
				$sql = "insert into painel.conteudocaixa (cxcid,conteudo) values($cxcid,'$conteudo')";
			}
		}else{
			foreach($conteudo as $k => $m){
				if(in_array($m,$arrMunicipios))
					unset($conteudo[$k]);
			}
			if(count($conteudo) == 0) $conteudo = array();
			$sql = "update painel.conteudocaixa set conteudo = '".serialize($conteudo)."' where cxcid = $cxcid";
		}
		
		if(!$db->executar($sql)){
			echo "erro";
			return false;
		}else{
			$db->commit($sql);
			echo "ok";
			return true;
		}
		
	}
	
}


/**
 * Função que adiciona / remove o indicador da tabela painel.indicadores_favoritos
 * 
 * @author Juliano Meinen
 * @return array (array com os indicadores do usuário)
 * @param false
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 29/06/2009
 */
function pegaMeusIndicadores($cxcid){
	global $db;
	
	if(!$_SESSION['usucpf']){
		return array();
	}else{
		$sql = "select indid from painel.indicadorusuario where cxcid = $cxcid and usucpf = '{$_SESSION['usucpf']}'";
		$res = $db->carregar($sql);
		
		!$res ? $res = array() : $res = $res;

		foreach($res as $r){
			$arrIndid[] = $r['indid'];
		}
		
		!$arrIndid? $arrIndid = array() : $arrIndid = $arrIndid;
		
		return $arrIndid;
	} 
}

/**
 * Função que exibe a dash board do indicador
 * 
 * @author Juliano Meinen
 * @return null
 * @param integer (indid -> id do indicador)
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 07/07/2009
 */
function exibedashBoard($indid){
	global $db;
	
	$sql = "select
				ind.indnome,
				exo.exodsc,
				sec.secdsc,
				aca.acadsc,
				ind.regid,
				ind.unmid,
				ind.indcumulativo,
				ind.indcumulativovalor,
				ind.indqtdevalor,
				per.perdsc,
				per.perid
			from
				painel.indicador ind
			left join
				painel.eixo exo ON exo.exoid = ind.exoid
			left join
				painel.secretaria sec ON sec.secid = ind.secid
			left join
				painel.acao aca ON aca.acaid = ind.acaid
			left join
				painel.periodicidade per ON per.perid = ind.perid
			where
				ind.indid = $indid";
	$ind = $db->pegaLinha($sql);
	
	$sql = "select 
				tdiid,
				tdidsc
			from
				painel.detalhetipoindicador
			where
				indid = $indid
			and
				tdistatus = 'A'";
	$det = $db->carregar($sql);

	?>
	
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr bgcolor="#f9f9f9">
			      <td colspan="2" align="left">
			  	  	<span style="font-size: 16px;font-weight: bold;"><? echo $indid." - ".$ind['indnome'] ?></span><br />
			  	  	<b><? echo $ind['exodsc'] ?></b> >> <b><? echo $ind['secdsc'] ?></b> >> <b><? echo $ind['acadsc'] ?></b>
			  	  </td>
		 </tr>
		 <tr>
			      <td colspan="2" align="left">
			  	  	<div style="padding:5px;width:99%;border: solid 1px #000000;background-color: #FFFFFF">
				  	  	<fieldset>
				  	  		<legend><span style="cursor:pointer;" onclick="exibeFiltrosPainel(this);" ><img align="absmiddle" title="mais" src="../imagens/mais.gif"/> Filtros</span></legend>
				  	  		<div id="filtros_painel" style="display:none" >
								<input type="hidden" name="ultima_sehid" id="ultima_sehid" value="0" />
								<?php if($ind['indcumulativo'] == "S"){ ?>
									<input type="hidden" name="indcumulativo" id="indcumulativo" value="1" />
								<?php } ?>
								
								<div>
											<div id="opc_periodo" style="margin:5px; float:left;">
											<fieldset style="padding:5px;">
												<legend>Período:</legend>
											
												<? 
												//Carrega o período do indicador
												$sql = "select  
															dpe.dpeid,
															dpe.dpedsc,
															seh.sehid
														from 
															painel.seriehistorica seh
														inner join
															painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
														where
															indid = $indid
														and
															sehstatus <> 'I'
														order by
															dpe.dpedatainicio asc";
												
												/*$sql = "select 
															dpeid,
															dpedsc,
															sehid 
														from 
															painel.v_detalheindicadorsh
														where
															indid = $indid
														and
															sehstatus <> 'I'
														group by
															dpeid,
															dpedsc,
															sehid,
															data
														order by 
															data asc";*/
												$periodos = $db->carregar($sql);
												
												if($periodos){
													
													if(count($periodos) > 12){
														$k1 = count($periodos) - 12;
													}else{
														$k1 = 0;
													}
													?>
													<select name="periodo_inicio" id="periodo_inicio">
													<? foreach($periodos as $k => $periodo){ //inicio do foreach do periodo inicio?>
														<option <? echo $k == $k1 ? "selected=\"selected\"" : "" ?> value="<? echo $periodo['dpeid'] ?>" ><? echo $periodo['dpedsc'] ?></option>
													<? } //fim do foreach do periodo inicio?>
													</select>
													<span id="ate_periodo" >até</span> 
													<select name="periodo_fim" id="periodo_fim">
													<? foreach($periodos as $k => $periodo){ //inicio do foreach do periodo fim ?>
														<option <? echo $k == (count($periodos) - 1)? "selected=\"selected\"" : "" ?> value="<? echo $periodo['dpeid'] ?>" ><? echo $periodo['dpedsc'] ?></option>
													<? } //fim do foreach do periodo fim ?>
													</select>
												<? } ?>
											</fieldset>
											</div>
											<?
											
											if($ind['unmid'] != UNIDADEMEDICAO_PERCENTUAL && $ind['unmid'] != UNIDADEMEDICAO_RAZAO):
											
												$arrPeriodicidade = getPeriodicidadeIndicador($indid);
												$arrPeriodicidade = !$arrPeriodicidade ? array() : $arrPeriodicidade;
												?>
												
												<div id="opc_periodicidade" style="margin:5px;width:100px;float:left;">
												<fieldset style="padding:5px;">
													<legend>Periodicidade:</legend>
													<select id="sel_periodicidade" name="sel_periodicidade" >
														<?php foreach($arrPeriodicidade as $arrPer): ?>
															<?php $arrPerid[] = $arrPer['perid']; ?>
															<option value="<?php echo $arrPer['perid'] ?>" ><?php echo $arrPer['perdsc'] ?></option>
														<?php endforeach; ?>
														<? $arrPerid = !$arrPerid ? array() : $arrPerid ?>
														<?php if(!in_array(PERIODO_ANUAL,$arrPerid)): ?>
															<option value="<?php echo PERIODO_ANUAL ?>" >Anual</option>
														<?php endif; ?>
														
													</select>
												</div>
										<? endif; ?>
											
										<? $sql = "	select 
														unmdesc,
														i.indescala,
														per.perdsc
													from 
														painel.indicador i
													left join 
														painel.unidademedicao u on i.unmid = u.unmid
													left join 
														painel.periodicidade per ON per.perid = i.perid
													where 
														indid = $indid";
								
											$escala = $db->pegaLinha($sql);;
											 
											if(strstr($escala['unmdesc'], 'Número inteiro') && $escala['indescala'] == "t"){?>
											
											<div id="opc_escala" style="margin:5px;float:left;width:150px">
											<fieldset style="padding:5px;">
												<legend>Aplicar escala em:</legend>
												<select id="unidade_inteiro">
													<option selected="selected" value="1">Unidade</option>
													<option value="1000">Milhares</option>
													<option value="1000000">Milhões</option>
													<option value="1000000000">Bilhões</option>
												</select>
											</fieldset>
											</div>
											<? }
											if(strstr($escala['unmdesc'], 'Moeda')){?>
											<div id="opc_escala_moeda" style="margin:5px;float:left;width:150px;">
											<fieldset style="padding:5px;">
												<legend>Aplicar escala em:</legend>
												<select id="unidade_moeda">
													<option selected="selected" value="1">Reais</option>
													<option value="1000">Milhares de Reais</option>
													<option value="1000000">Milhões de Reais</option>
													<option value="1000000000">Bilhões de Reais</option>
												</select>
											</fieldset>
											</div>
											<? }
											if(strstr($escala['unmdesc'], 'Moeda') || ($ind['indqtdevalor'] == 't')){?>
											<div id="opc_escala_moeda" style="margin:5px;float:left;width:100px;">
											<fieldset style="padding:5px;">
												<legend>Aplicar índice:</legend>
												<select id="indice_moeda">
													<option selected="selected" value="null">Selecione...</option>
													<option value="ipca">IPCA Médio</option>
												</select>
											</fieldset>
											</div>
										</div>
									
									
											<? }
											if($ind['regid'] == 7 || $ind['regid'] == 2 || $ind['regid'] == 4 || $ind['regid'] == 5 || $ind['regid'] == 6 || $ind['regid'] == 8 || $ind['regid'] == 9 || $ind['regid'] == 10 || $ind['regid'] == 11 || $ind['regid'] == 12 || 
											   $ind['regid'] == REGIONALIZACAO_POLO || 
											   $ind['regid'] == REGIONALIZACAO_IESCPC){ //Estado?>
										<div style="clear:both">	
											<div id="opc_regiao" style="margin:5px;float:left;width:100px">
											<fieldset style="padding:5px;">
												<legend>Região:</legend>
												<select style="width:90px;" id="regcod" onchange="filtraEstadoDB(this.value)" >
													<option selected="selected" value="todos">Selecione...</option>
													<? 
													$sql = "select 
																regcod, regdescricao
															from
																territorios.regiao
															order by
																regdescricao";
													$regiao = $db->carregar($sql);
													foreach($regiao as $rg){?>
														<option value="<? echo $rg['regcod'] ?>"><? echo $rg['regdescricao'] ?></option>
													<? } ?>
												</select>
											</fieldset>
											</div>
									
											<div id="opc_estado" style="margin:5px;float:left;width:110px;">
											<fieldset style="padding:5px;">
												<legend>Estado:</legend>
												<span id="exibeEstado" >
													<select id="estuf" style="width:100px;" onchange="filtraMunicipio(this.value)" >
														<option selected="selected" value="todos">Selecione...</option>
														<? 
														$sql = "select 
																	estuf, estdescricao
																from
																	territorios.estado
																order by
																	estdescricao";
														$estados = $db->carregar($sql);
														foreach($estados as $uf){?>
															<option value="<? echo $uf['estuf'] ?>"><? echo $uf['estdescricao'] ?></option>
														<? } ?>
													</select>
													</span>
												</fieldset>
											</div>
											<? } if($ind['regid'] == 7 || $ind['regid'] == 2 || 
											        $ind['regid'] == 4 || $ind['regid'] == 5 || $ind['regid'] == 8 || $ind['regid'] == 9 || $ind['regid'] == 10 || $ind['regid'] == 11 || $ind['regid'] == 12 || 
											        $ind['regid'] == REGIONALIZACAO_POLO || 
											        $ind['regid'] == REGIONALIZACAO_IESCPC){ //Município?>
											<div id="opc_grp_mun" style="margin:5px;float:left;width:160px;">
											<fieldset style="padding:5px;">
												<legend>Grupo de Municípios:</legend>
												<span id="exibe_grupo_municipio">
												<select id="gtmid" style="width:150px;" onchange="filtraGrupoMunicipios(this.value)" >
													<option selected="selected" value="todos">Selecione...</option>
												<? 
													$sql = "select 
																gtmid, gtmdsc
															from
																territorios.grupotipomunicipio
															where
																gtmstatus = 'A'
															order by
																gtmdsc";
													$grupoMun = $db->carregar($sql);
													foreach($grupoMun as $gm){?>
														<option value="<? echo $gm['gtmid'] ?>"><? echo $gm['gtmdsc'] ?></option>
													<? } ?>
													</select>
												</span>
											</fieldset>
											</div>
											
											<div id="opc_tpo_mun" style="margin:5px;float:left;width:210px;">
											<fieldset style="padding:5px;">
												<legend>Tipos de Municípios:</legend>
												<span id="exibe_tipo_municipio">
												<select id="tpmid" style="width:200px;">
													<option disabled="disabled" selected="selected" value="todos">Selecione...</option>
												</select>
												</span>
											</fieldset>
											</div>
											
											<div id="opc_mun" style="margin:5px;float:left;width:210px;">
											<fieldset style="padding:5px;">
												<legend>Município:</legend>
												<span id="exibe_municipio">
												<select id="muncod" style="width:200px;">
													<option disabled="disabled" selected="selected" value="todos">Selecione...</option>
												</select>
												</span>
												</fieldset>
											</div>
												<? $sql = "	select
																regdescricao,
																regsqlcombo
															from
																painel.regionalizacao
															where
																regid = {$ind['regid']}
															and
																regsqlcombo is not null";
													$regDados = $db->pegaLinha($sql);
													if(is_array($regDados)): ?>
													<div id="opc_reg" style="margin:5px;float:left;width:210px;">
													<fieldset style="padding:5px;">
														<legend><?=$regDados['regdescricao'] ?>:</legend>
														<span id="exibe_reg">
														<select disabled="disabled" id="regvalue" onclick="alert('Favor Selecionar Estado e Município!')" style="width:200px;">
															<option selected="selected" value="">Selecione...</option>
														</select>
														</span>
														</fieldset>
													</div>
												<? endif; ?>
											<? } ?>
									
									<? if($det[0]['tdiid'] && $det[0]['tdiid'] != ""){ ?>
									<div style="clear:both">
										<? 	$sql = "select tdidsc from painel.detalhetipoindicador where tdiid = {$det[0]['tdiid']}"; 
											$detalhe1 = $db->pegaUm($sql);
										?>
										<div id="opc_det1" style="margin:5px;float:left;width:400px;">
											<fieldset style="padding:5px;">
												<legend><? echo $detalhe1 ?>:</legend>
											<? 
											$sql = "select 
														tidid as codigo,
														tiddsc as descricao
													from 
														painel.detalhetipodadosindicador 
													where 
														tdiid = {$det[0]['tdiid']}
													and
														tidstatus = 'A'"; 
											//$db->monta_combo_multiplo('tidid1',$sql,"S","Selecione","","","",10,300,"","tidid1");
											//combo_popup("tidid1",$sql,"","","","","","S",true,"",5,250,"","","","","",true,"","","",$arrCarregados,"");
											combo_popup( "tidid1", $sql, "", '400x400', 0, array(), '', 'S', true, true );
											//$db->monta_combo('tidid1',$sql,'S','Selecione...','','','','200','N',"tidid1","","");
											?>
										</fieldset>
											</div>
									<? } ?>
									<? if($det[1]['tdiid'] && $det[1]['tdiid'] != ""){ ?>
										
										<? 	$sql = "select tdidsc from painel.detalhetipoindicador where tdiid = {$det[1]['tdiid']}"; 
											$detalhe2 = $db->pegaUm($sql);
										?>
										<div id="opc_det2" style="margin:5px;float:left;width:400px;">
											<fieldset style="padding:5px;">
												<legend><? echo $detalhe2 ?>:</legend>
											<? 
											$sql = "select 
														tidid as codigo,
														tiddsc as descricao
													from 
														painel.detalhetipodadosindicador 
													where 
														tdiid = {$det[1]['tdiid']}
													and
														tidstatus = 'A'"; 
											//$db->monta_combo('tidid2',$sql,'S','Selecione...','','','','200','N',"tidid2","","");
											combo_popup( "tidid2", $sql, "", '400x400', 0, array(), '', 'S', true, true );
											?>
											</fieldset>
											</div>
									</div>
									
									<? } ?>
									
									<?php if($ind['regid'] == REGIONALIZACAO_ZONA || $ind['regid'] == REGIONALIZACAO_SUBPREFEITURA || $ind['regid'] == REGIONALIZACAO_DISTRITO || $ind['regid'] == REGIONALIZACAO_SETOR): ?>
										<div id="opc_zona" style="margin:5px;float:left;">
											<fieldset style="padding:5px;">
												<legend>Zona:</legend>
												<span id="td_zona" >
													<select name="zonid" id="zonid" onchange="filtraSubprefeitura(this.value)" >
														<option selected="selected" value="">Selecione...</option>
														<? 
														$sql = "select 
																	zonid, zondescricao
																from
																	territoriosgeo.zona
																order by
																	zondescricao";
														$zona = $db->carregar($sql);
														foreach($zona as $z){?>
															<option <?php echo ($_POST['zonid'] == $z['zonid'] ? "selected='selected'" : "")?> value="<? echo $z['zonid'] ?>"><? echo $z['zondescricao'] ?></option>
														<? } ?>
													</select>
												</span>
											</fieldset>
										</div>
										
										<div id="opc_subprefeitura" style="margin:5px;float:left;">
											<fieldset style="padding:5px;">
												<legend>Subprefeitura:</legend>
												<span id="td_subprefeitura" >
													<select name="subid" id="subid" <?php echo $_POST['zonid'] ? "" : "disabled='disabled'" ?> onchange="filtraDistrito(this.value)" >
														<option selected="selected" value="">Selecione...</option>
														<? 
														if($_POST['zonid']){
															$sql = "select 
																		subid, subdescricao
																	from
																		territoriosgeo.subprefeitura
																	where
																		zonid = {$_POST['zonid']}
																	order by
																		subdescricao";
															$sub = $db->carregar($sql);
														}else{
															$sub = array();
														}
														foreach($sub as $s){?>
															<option <?php echo ($_POST['subid'] == $s['subid'] ? "selected='selected'" : "")?> value="<? echo $s['subid'] ?>"><? echo $s['subdescricao'] ?></option>
														<? } ?>
													</select>
												</span>
											</fieldset>
										</div>
										
										<div id="opc_distrito" style="margin:5px;float:left;">
											<fieldset style="padding:5px;">
												<legend>Distrito:</legend>
												<span id="td_distrito" >
													<select name="disid" id="disid" <?php echo $_POST['subid'] ? "" : "disabled='disabled'" ?> onchange="filtraSetor(this.value)" >
														<option selected="selected" value="">Selecione...</option>
														<? 
														if($_POST['subid']){
															$sql = "select 
																		disid, disdescricao
																	from
																		territoriosgeo.distrito
																	where
																		subid = {$_POST['subid']}
																	order by
																		disdescricao";
															$dis = $db->carregar($sql);
														}else{
															$dis = array();
														}
														foreach($dis as $d){?>
															<option <?php echo ($_POST['disid'] == $d['disid'] ? "selected='selected'" : "")?> value="<? echo $d['disid'] ?>"><? echo $d['disdescricao'] ?></option>
														<? } ?>
													</select>
												</span>
											</fieldset>
										</div>
										
										<div id="opc_setor" style="margin:5px;float:left;">
											<fieldset style="padding:5px;">
												<legend>Setor:</legend>
												<span id="td_setor" >
													<select name="setid" id="setid" <?php echo $_POST['disid'] ? "" : "disabled='disabled'" ?> >
														<option selected="selected" value="">Selecione...</option>
														<? 
														if($_POST['disid']){
															$sql = "select 
																		setid, setcod
																	from
																		territoriosgeo.setor
																	where
																		disid = {$_POST['disid']}
																	order by
																		setcod";
															$setor = $db->carregar($sql);
														}else{
															$setor = array();
														}
														foreach($setor as $s){?>
															<option <?php echo ($_POST['setid'] == $s['setid'] ? "selected='selected'" : "")?> value="<? echo $s['setid'] ?>"><? echo $s['setcod'] ?></option>
														<? } ?>
													</select>
												</span>
											</fieldset>
										</div>
												
										</div>
									<?php endif; ?>
								<div style="clear:both;margin:5px;">								
								<input style="margin-left:5px;" type="button" name="button_ok" id="button_ok" onclick="mudaTipoGrafico2(<? echo $indid ?>)" value="ok" />
								<input type="hidden" id="regid" name="regid" value="<? echo $ind['regid'] ?>">
								</div>
							</div>
				  	  	</fieldset>
				  	  </div>
			  	  </td>
		 </tr>
		 
		  <tr>
		 	<td colspan="2">
		 	<style>
		 		.div_abas_ajax{width:100%;height:100%;border:solid 1px black;background-color: #FFFFFF}
		 	</style>
		 	<?php 
		 	include(APPRAIZ."/includes/classes/AbaAjax.class.inc");
		 	
		 	$abaAjax = new AbaAjax("abasAjax",false,false,false,false);
		 	
		 	$sql = "select indvispadrao from painel.indicador where indid = $indid";
			$indvispadrao = $db->pegaUm($sql);
			if($indvispadrao){
				$dados = explode(";",$indvispadrao);
				$abaPadrao = $dados[14] && $dados[14] != "null" && $dados[14] != "todos" ? $dados[14] : '';
			}else{
				$abaPadrao = '';
			}
			
			echo '<input type="hidden" name="abaPadrao" id="abaPadrao" value="'.$abaPadrao.'"  />';
		 			 	
		 	if($ind['regid'] == 7 || $ind['regid'] == 2 || $ind['regid'] == 4 || $ind['regid'] == 5 || $ind['regid'] == 6 || $ind['regid'] == 8 || $ind['regid'] == 9 || $ind['regid'] == 10 || $ind['regid'] == 11 || $ind['regid'] == 12 || 
		 	   $ind['regid'] == REGIONALIZACAO_POLO || 
		 	   $ind['regid'] == REGIONALIZACAO_IESCPC){
		 		$arAba = array(
	 		  			array(	 "descricao" => "Gráficos", 
		 	   				 	 "padrao" => (!$abaPadrao || $abaPadrao == '' || $abaPadrao == "graficos" ? true : false) ,
		 		   				 "url" => "painel.php?modulo=principal/painel_controle&acao=A",
		 		   				 "parametro" => "abaAjax=graficos",
	 		  					 "funcao" => "abaPadrao('graficos')"
	 		  			),
	 		  			array(	 "descricao" => "Mapa", 
		 	   				 	 "padrao" => ($abaPadrao == "mapa" ? true : false) ,
		 		   				 "url" => "painel.php?modulo=principal/painel_controle&acao=A",
		 		   				 "parametro" => "abaAjax=mapa",
	 		  					 "funcao" => "abaPadrao('mapa')"
	 		  			),
	 		  			array(	 "descricao" => "Relatório", 
		 	   				 	 "padrao" => ($abaPadrao == "relatorio" ? true : false) ,
		 		   				 "url" => "painel.php?modulo=principal/painel_controle&acao=A",
		 		   				 "parametro" => "abaAjax=relatorio",
	 		  					 "funcao" => "abaPadrao('relatorio')"
	 		  			),
	 		  			array(	 "descricao" => "Série Histórica", 
		 	   				 	 "padrao" => ($abaPadrao == "seriehistorica" ? true : false) ,
		 		   				 "url" => "painel.php?modulo=principal/painel_controle&acao=A",
		 		   				 "parametro" => "abaAjax=seriehistorica",
	 		  					 "funcao" => "abaPadrao('seriehistorica')"
	 		  			),
	 		  			array(	 "descricao" => "Dados", 
		 	   				 	 "padrao" => ($abaPadrao == "detalhes" ? true : false) ,
		 		   				 "url" => "painel.php?modulo=principal/painel_controle&acao=A",
		 		   				 "parametro" => "abaAjax=detalhes",
	 		  					 "funcao" => "abaPadrao('detalhes')"
	 		  			)
	  		   		 );
		 	}
		 	else{
		 		$arAba = array(
	 		  			array(	 "descricao" => "Gráficos", 
		 	   				 	 "padrao" => (!$abaPadrao || $abaPadrao = '' || $abaPadrao == "graficos" ? true : false) ,
		 		   				 "url" => "painel.php?modulo=principal/painel_controle&acao=A",
		 		   				 "parametro" => "abaAjax=graficos"
	 		  			),
	 		  			array(	 "descricao" => "Relatório", 
		 	   				 	 "padrao" => ($abaPadrao == "relatorio" ? true : false) ,
		 		   				 "url" => "painel.php?modulo=principal/painel_controle&acao=A",
		 		   				 "parametro" => "abaAjax=relatorio"
	 		  			),
	 		  			array(	 "descricao" => "Série Histórica", 
		 	   				 	 "padrao" => ($abaPadrao == "seriehistorica" ? true : false) ,
		 		   				 "url" => "painel.php?modulo=principal/painel_controle&acao=A",
		 		   				 "parametro" => "abaAjax=seriehistorica"
	 		  			),
	 		  			array(	 "descricao" => "Dados", 
		 	   				 	 "padrao" => ($abaPadrao == "detalhes" ? true : false) ,
		 		   				 "url" => "painel.php?modulo=principal/painel_controle&acao=A",
		 		   				 "parametro" => "abaAjax=detalhes"
	 		  			)
	  		   		 );
		 	}
		 	
		 	$abaAjax->criaAba($arAba,'div_abas_ajax');
		 	?>
		 	</td>
		 </tr>
		<tr bgcolor="#cccccc">
			      <td colspan="2" align="center">
			  	  	<input type="button" class="botao" onclick="history.back(-1)" name="button_voltar" value="Voltar" id="button_voltar" />
			  	  	<?php 
							$permissoes = verificaPerfilPainel();
							if(!$permissoes['verindicadores']) $permissoes['verindicadores'] = array();
							if($permissoes['verindicadores'] == 'vertodos' || in_array($indid,$permissoes['verindicadores'])){
							?>
								<input style="margin-left:5px;" type="button" name="button_salva_view" id="button_salva_view" onclick="salvaView(<? echo $indid ?>)" value="Definir Visualização Padrão" />
								
					<? } ?>
			  	  	
			  	  </td>
		 </tr>
	</table>
	
<?php }

/**
 * Função que exibe o formulário de busca dos indicadores
 * 
 * @author Juliano Meinen
 * @return null
 * @param integer (indid -> id do indicador)
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 07/07/2009
 */
function exibeFormBuscaIndicador($cxcid){
	global $db;
	
	$sql = "select cxcid from painel.conteudocaixa where cxcid = $cxcid";
	if($db->pegaUm($sql)){
		$sql = "select conteudo from painel.conteudocaixa where cxcid = $cxcid";
		$busca = $db->pegaUm($sql);
		$texto = unserialize($busca);
	}else{
		$sql = "insert into painel.conteudocaixa (cxcid,conteudo) values ($cxcid,'')";
		$db->executar($sql);
	}
	?>
	<div style="padding:3px;">
		<input type="text" id="busca_<?php echo $cxcid ?>" onkeypress="BuscaIndicadorEnter(event,<? echo $cxcid ?>);" name="busca_<? echo $cxcid ?>" value="<? echo $texto ?>" /> <input type="button" onclick="buscarIndicador(<? echo $cxcid ?>);" name="buscar" value="Buscar" /> <input type="button" onclick="limparBusca(<? echo $cxcid ?>);" name="limpar_busca" value="Limpar Busca" />
	</div>
	<div id="resultadoBusca_<?php echo $cxcid ?>" >
	<?php 
	if($texto || $texto != ''){
		 exibeBuscaIndicador($texto);
	}
	?>
	</div>
<?php }

//Grava a busca do usuário
function gravaBuscaIndicador($cxcid,$texto){
	global $db;
	
	$stexto = serialize($texto);
	$sql = "update painel.conteudocaixa set conteudo = '$stexto' where cxcid = $cxcid";
	$db->executar($sql);
	$db->commit($sql);
	
}


function exibeBuscaIndicador($texto){
	global $db;
	
	//Lista os novos indicadores atribuidos pelo usuário
	$sql = "	select distinct
					ind.indid,indnome 
				from 
					painel.indicador ind 
				inner join 
					painel.eixo exo ON exo.exoid = ind.exoid 
				inner join 
					painel.acao aca ON aca.acaid = ind.acaid 
				inner join 
					painel.secretaria sec ON sec.secid = ind.secid 
				inner join 
					painel.seriehistorica dsh ON dsh.indid = ind.indid 
				where 
					ind.indpublicado is true
				and
					ind.indstatus = 'A'
				and (
						ind.indnome ilike '%$texto%' 
					OR 
						exo.exodsc ilike '%$texto%' 
					OR 
						aca.acadsc ilike '%$texto%' 
					OR 
						sec.secdsc ilike '%$texto%'
					OR 
						(ind.indid)::text ilike '%$texto%'
					)";
	//dbg($sql);
	exibeMinhaListaIndicadores($sql,null,'Busca Indicadores');
				
	/*$indicadores = $db->carregar($sql);
	$indicadores = !$indicadores? array() : $indicadores;

	if(count($indicadores) != 0){
		echo "<table cellSpacing=0 cellPadding=3 style=\"width:100%\">";
		echo "<tr bgcolor='#D9D9D9' ><td style=\"font-weight:bold;text-align:center\" >Cód.</td><td style=\"font-weight:bold\"  >Nome do Indicador</td><td style=\"text-align:right;font-weight:bold;white-space: nowrap;\"  >Quantidade / Valor</td></tr>";
		$i = 0;
		foreach($indicadores as $ind){
		################# Periodicidade ################# //Pega Periodicidade do Indicador
				$sql = "select 
							perdsc 
						from 
							painel.indicador i
						left join 
							painel.periodicidade p on i.perid = p.perid
						where 
							indid = {$ind['indid']}";
				$perdsc = $db->pegaUm($sql);
						
				################# Diária ################# 
				if($perdsc == "Diária"){ //Se a Periodicidade do Indicador for Diária, busca data e quantidade (sehdtcoleta / sehqtde) da tabela seriehistorica  
					$sql = "select 
							sehid,
							sehqtde,
							(to_char(sehdtcoleta, 'YYYY-MM-DD')) as data 
						from 
							painel.seriehistorica 
						where 
							indid = {$ind['indid']}
						and 
							dpeid is null
						and
							sehstatus <> 'I'
						order by
							data desc
						limit
							1";
				}
				################# Se não for Diária ################# 
				if($perdsc != "Diária"){ //Se a Periodicidade do Indicador não for Diária, busca data (dpedatainicio) da tabela detalheperiodicidade e quantidade (sehdtcoleta) da tabela seriehistorica
					$sql = "select
								seh.sehid,
								seh.sehqtde,
								(to_char(dpe.dpedatainicio, 'YYYY-MM-DD')) as data 
							from 
								painel.seriehistorica  seh
							inner join
								painel.detalheperiodicidade dpe ON seh.dpeid = dpe.dpeid
							where  
								indid = {$ind['indid']}
							and
								seh.dpeid is not null
							and 
								sehstatus <> 'I'
							order by
								data desc
							limit
								1";
				}
						
				$seh = $db->pegaLinha($sql);
								
				################# Comparação do penúltimo valor / data para indicar a seta de tendência (cima / baixo) ################# 
				$seta = "";  //variável seta inicia vazia;
				
				################# Veirifica se existe a Série Histórica p/ comparar #################
				if($seh['sehid']){ // se existe, busca novamente a série histórica, com excessão do último sehid
					
					################# Diária ################# 
					if($perdsc == "Diária"){ //Se a Periodicidade do Indicador for Diária, busca data e quantidade (sehdtcoleta / sehqtde) da tabela seriehistorica  
						$sql = "select 
							sehid,
							sehqtde,
							(to_char(sehdtcoleta, 'YYYY-MM-DD')) as data 
						from 
							painel.seriehistorica 
						where 
							indid = {$ind['indid']}
						and 
							dpeid is null
						and
							sehstatus <> 'I'
						and
							sehid != {$seh['sehid']}
						order by
							data desc
						limit
							1";
					}
					
					################# Se não for Diária ################# 
					if($perdsc != "Diária"){ //Se a Periodicidade do Indicador não for Diária, busca data (dpedatainicio) da tabela detalheperiodicidade e quantidade (sehdtcoleta) da tabela seriehistorica
						$sql = "select
								seh.sehid,
								seh.sehqtde,
								(to_char(dpe.dpedatainicio, 'YYYY-MM-DD')) as data 
							from 
								painel.seriehistorica  seh
							inner join
								painel.detalheperiodicidade dpe ON seh.dpeid = dpe.dpeid
							where  
								indid = {$ind['indid']}
							and
								seh.dpeid is not null
							and 
								sehstatus <> 'I'
							and
								sehid != {$seh['sehid']}
							order by
								data desc
							limit
								1";
					}
							
					$seh2 = $db->pegaLinha($sql);
					
					################# Veirifica qual o tipo de tendência do indicador (Maior melhor, Menor melhor) #################

					$sql = "select estid from painel.indicador where indid = {$ind['indid']}";
					$estilo = $db->pegaUm($sql);
					$estilo = !$estilo ? 1 : $estilo;
					 
					################# Veirifica se existe a 2ª Série Histórica p/ comparar #################
					if($seh2['sehid']){ // se existe, faz a comparação para indicar a seta
						
						$valor1 = (float)$seh['sehqtde']; //seh
						$valor2 = (float)$seh2['sehqtde']; //seh2

						if($valor1 < $valor2 && $estilo == 1){ // Se o $valor 1 for menor q	o $valor 2 e a tendência for MAIOR MELHOR, a tendência indica queda (seta vermelha p/ baixo)
							$seta = " <img style=\"cursor:pointer\" src=\"../imagens/indicador-vermelha2.png\" align=\"absmiddle\" >";
						}
						if($valor1 < $valor2 && $estilo == 2){ // Se o $valor 1 for menor q	o $valor 2 e a tendência for MENOR MELHOR, a tendência indica aumento (seta verde p/ baixo)
							$seta = " <img style=\"cursor:pointer\" src=\"../imagens/indicador-verde2_para_baixo.png\" align=\"absmiddle\" >";
						}
						if($valor1 > $valor2 && $estilo == 1){ // Se o $valor 1 for maior q	o $valor 2 e a tendência for MAIOR MELHOR, a tendência indica aumento (seta verde p/ cima)
							$seta = " <img style=\"cursor:pointer\"  src=\"../imagens/indicador-verde2.png\" align=\"absmiddle\" >";
						}
						if($valor1 > $valor2 && $estilo == 2){ // Se o $valor 1 for maior q	o $valor 2 e a tendência for MENOR MELHOR, a tendência indica queda (seta vermelha p/ cima)
							$seta = " <img style=\"cursor:pointer\"  src=\"../imagens/indicador-vermelha2_para_cima.png\" align=\"absmiddle\" >";
						}
					}
					
					################# Verifica o Tipo de unidade de medida do indicador (umedesc) na tabela unidademeta, p/ exibir no SuperTitle #################
					$sql = "select 
								umedesc 
							from 
								painel.indicador i
							left join 
								painel.unidademeta u on i.umeid = u.umeid
							where 
								indid = {$ind['indid']}";
						
					$umedesc = $db->pegaUm($sql);
					
					################# Verifica o Tipo de unidade de Medição do indicador (unmdesc) na tabela unidademedicao, p/ exibir as casas decimais corretas no sehqtde #################
					$sql = "select 
								unmdesc 
							from 
								painel.indicador i
							left join 
								painel.unidademedicao u on i.unmid = u.unmid
							where 
								indid = {$ind['indid']}";
					
					$unmedsc = $db->pegaUm($sql);
					
					//Verifica o tipo de medição e aplica as regras
					switch($unmedsc){
							case "Número inteiro":
								$seh['sehqtde'] = str_replace(',00','',number_format($seh['sehqtde'],2,',','.'));
								break;
							case "Percentual":
								$seh['sehqtde'] = str_replace(',00','',number_format($seh['sehqtde'],2,',','.'))."%";
								break;
							case "Razão":
								$seh['sehqtde'] = str_replace(',00','',number_format($seh['sehqtde'],2,',','.'));
								break;
							case "Número índice":
								$seh['sehqtde'] = str_replace(',00','',number_format($seh['sehqtde'],2,',','.'));
								break;
							case "Moeda":
								$seh['sehqtde'] = "R$ ".number_format($seh['sehqtde'],2,',','.');
								break;
							default:
								$seh['sehqtde'] = str_replace(',00','',number_format($seh['sehqtde'],2,',','.'));
								break;
						}
				}
										
					//Início da Exibição dos elementos na tela
					$cor = $i%2 ? "#FCFCFC" : "#f5f5f5";
					echo "<tr id=\"tr_{$cxcid}_{$ind['indid']}\" bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\" >
						<td align=\"center\" ><div style=\"width:100%;text-align:center;color:#0066CC\" >".$ind['indid']."</div></td>
						<td><div style=\"cursor:pointer\" onclick=\"exibeDashBoard({$ind['indid']})\" style=\"width:100%;\" >".$ind['indnome']."</div></td>
						<td style=\"white-space: nowrap;\" align=\"right\" ><div style=\"width:100%;text-align:right;color:#0066CC\" >".$seh['sehqtde']. " ".$seta."</div></td>
						</tr>";
						
					$i++;
				}
				echo "</table>";
			}
			if(count($indicadores) == 0){
				echo "<div style=\"padding:10px;text-align:center;cursor:pointer;color:#990000\" >Não existem registros.</div>";
			}*/
}

function exibeArvoreIndicador($cxcid){
	global $db;
	
	$sql = "select 
				conteudo 
			from 
				painel.conteudocaixa 
			where 
				cxcid = $cxcid";
				
	$arvore = $db->pegaUm($sql);
	
	if(!$arvore){
		echo "<div onclick=\"addArvoreIndicadores($cxcid)\" style=\"padding:10px;text-align:center;cursor:pointer\" >Clique aqui para adicionar a Árvore de Índicadores.</div>";
	}else{
		echo "exibe árvore";
	}
	
}

function addArvoreIndicador($cxcid){
	global $html_agrupador;
	include_once(APPRAIZ.'includes/Agrupador.php');
	
	global $db; ?>
		<form id="formulario" method="post" name="formulario" >
		<input type="hidden" name="cxcid_arvore" id="cxcid_arvore" value="<? echo $cxcid ?>"/>
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr bgcolor="#cccccc">
			    <td colspan="2" align="center">
			  	  	Árvore de Indicadores
				</td>
			</tr>
			<tr>
				<td width="195" class="SubTituloDireita" valign="top">Agrupadores</td>
				<td>
					<?php
					
						$matrizOrigem = array(
										array('codigo' => 'acaid',
											  'descricao' => 'Ação'),
										array('codigo' => 'exoid',
											  'descricao' => 'Área'),
										array('codigo' => 'secid',
											  'descricao' => 'Secretaria'));
						
						//Verifica se existem agrupadores selecionados
						$sql = "select conteudo from painel.conteudocaixa where cxcid = $cxcid";
						
						$arrAgrupadores = $db->pegaUm($sql);
						
						if($arrAgrupadores){
							$arrAgrupadores = unserialize($arrAgrupadores);
							foreach($arrAgrupadores as $agrup){
								switch($agrup){
									case "acaid":
										$desc = "Ação";
									break;
									case "exoid":
										$desc = "Área";
									break;
									case "secid":
										$desc = "Secretaria";
									break;
								}
								$matrizDestino[] = array('codigo' => $agrup,'descricao' => $desc);
							}
						}else{
							$matrizDestino = array();	
						}
						//Retira os agrupadores já existentes
						foreach($matrizOrigem as $k => $mOrigem){
							if(in_array($mOrigem,$matrizDestino)){
								unset($matrizOrigem[$k]);
							}
						}
						$campoAgrupador = new Agrupador( 'formulario' );
						$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matrizOrigem );
						$campoAgrupador->setDestino( 'agrupador', null , $matrizDestino);
						$campoAgrupador->exibir();
						
						?>
				</td>
			</tr>
			<tr bgcolor="#cccccc">
			    <td colspan="2" align="center">
			  	  	<input type="button" onclick="salvarArvore();" value="Salvar" class="botao" name="button_salvar" /> <input type="button" class="botao" name="cancelar" value="Cancelar" id="button_cancelar" />
				</td>
			</tr>
		</table>
		</form>		
		  
<?php }

//Chama a inclusão do conteudo da caixa
function exibeConteudoCaixa($cxcid,$tcoid,$abaid){
	
	define("TIPO_CONTEUDO_MEUS_INDICADORES",1);
	define("TIPO_CONTEUDO_GRAFICO",2);
	define("TIPO_CONTEUDO_MAPA",3);
	define("TIPO_CONTEUDO_INDICADOR_EIXO",6	);
	define("TIPO_CONTEUDO_INDICADOR_ACAO",7	);
	define("TIPO_CONTEUDO_INDICADOR_SECRETARIA",8 );
	define("TIPO_CONTEUDO_BUSCA_INDICADOR",9 );
	define("TIPO_CONTEUDO_ARVORE_INDICADOR",10 );
	
	switch($tcoid){
		
		case TIPO_CONTEUDO_MEUS_INDICADORES :
			exibePesquisa( $cxcid, "true" ); //checkbox = true
		break;
		
		case TIPO_CONTEUDO_GRAFICO :
			exibePesquisa( $cxcid, "false" ); //checkbox = false
		break;
		
		case TIPO_CONTEUDO_ARVORE_INDICADOR :
			addArvoreIndicador( $cxcid );
			//index($abaid);
		break;
		
		default:
			index($abaid);
		break;
		
	}
	
}

//Altera o nome da caixa
function editarNomecaixa($cxcid,$cxcdescricao){
	global $db;
	
	//Atualiza a data de modificação da data
	$sql = "select 
				abaid 
			from 
				painel.conteudousuario con
			inner join
				painel.caixaconteudo cxc ON cxc.ctuid = con.ctuid
			where
				cxcid = $cxcid
			limit 1";
	$abaid = $db->pegaUm($sql);
	if($abaid){
		$sql = "update painel.aba set abadata = now() where abaid = $abaid";
		$db->executar($sql);
	}
	
	$sql = "update painel.caixaconteudo set cxcdescricao = '$cxcdescricao' where cxcid = $cxcid";
	
	$db->executar($sql);
	$db->commit($sql);
	
}

//Adiciona a árvore de acordo com os agrupadores escolhidos
function addArvore($cxcid,$agrupador){
	global $db;
	
	//Atualiza a data de modificação da data
	$sql = "select 
				abaid 
			from 
				painel.conteudousuario con
			inner join
				painel.caixaconteudo cxc ON cxc.ctuid = con.ctuid
			where
				cxcid = $cxcid
			limit 1";
	$abaid = $db->pegaUm($sql);
	if($abaid){
		$sql = "update painel.aba set abadata = now() where abaid = $abaid";
		$db->executar($sql);
	}
	
	$conteudo = serialize($agrupador);
	
	$sql = "select cxcid from painel.conteudocaixa where cxcid = $cxcid";
	$cxcExiste = $db->pegaUm($sql);
	
	if(!$cxcExiste){
		$sql = "insert into 
					painel.conteudocaixa
				(cxcid,conteudo)
				values
					('$cxcid','".$conteudo."')";
	}else{
		$sql = "update painel.conteudocaixa set conteudo = '$conteudo' where cxcid = $cxcid";
	}
	$db->executar($sql);
	$db->commit($sql);
}


function EditaSerieHistoricaGrafico($tcgid,$sehid){
	global $db;
	
	$sql = "update painel.tipoconteudografico set sehid = $sehid where tcgid = $tcgid";
	$db->executar($sql);
	$db->commit($sql);
	
}

function exibeArvore($cxcid){
	global $db;
	
	$sql = "select conteudo from painel.conteudocaixa where cxcid = $cxcid";
	
	$arupadores = unserialize($db->pegaUm($sql));
	
	$arupadores = !$arupadores ? array("exoid","acaid","secid") : $arupadores;

	if(count($arupadores) < 1){
		echo "<div onclick=\"addArvoreIndicadores($cxcid)\" style=\"padding:10px;text-align:center;cursor:pointer\" >Clique aqui para adicionar a Árvore de Índicadores.</div>";
		return false;
	}
	else{
		echo "<div style=\"padding:10px;\" ><a href=\"javascript: d_$cxcid.openAll();\">Abrir Todos</a> &nbsp;|&nbsp; <a
					href=\"javascript: d_$cxcid.closeAll();\">Fechar Todos</a></div>";
		echo "<div id=\"arvore_{$cxcid}\" style=\";width:100%;height:100%;\" ></div>";
	}
	
	foreach($arupadores as $agp){
		switch($agp){
			case "acaid":
				$arrCampos[] = "acadsc"; 
				$arrJoin[] = "painel.acao aca ON ind.acaid = aca.acaid";
				break;
			case "exoid":
				$arrCampos[] = "exodsc"; 
				$arrJoin[] = "painel.eixo exo ON ind.exoid = exo.exoid";
				break;
			case "secid":
				$arrCampos[] = "secdsc"; 
				$arrJoin[] = "painel.secretaria sec ON ind.secid = sec.secid";
				break;
		}
	}
	
	$sql = "select 
				".implode(",",$arrCampos).",indnome
			from
				painel.indicador ind
			left join ".implode(" left join ",$arrJoin)."
			where
				ind.indstatus = 'A'";
	$dados = $db->carregar($sql);
	
	if(!$dados){
		echo "<div onclick=\"addArvoreIndicadores($cxcid)\" style=\"padding:10px;text-align:center;cursor:pointer\" >Clique aqui para adicionar a Árvore de Índicadores.</div>";
		return false;
	}
		
	echo "<script> document.getElementById( 'aguarde' ).style.display = 'none' </script>";
	
	$array[0]["uf"] = "DF"; 
	$array[0]["desc"] = "BRASília"; 
	$array[0]["num"] = "123";
	
	$array[1]["uf"] = "DF"; 
	$array[1]["desc"] = "Tagua"; 
	$array[1]["num"] = "123";
	
	$array[2]["uf"] = "SP"; 
	$array[2]["desc"] = "Paulo"; 
	$array[2]["num"] = "123";
	
	$array[3]["uf"] = "SP"; 
	$array[3]["desc"] = "Campinas"; 
	$array[3]["num"] = "123";
	
	$array[4]["uf"] = "GO"; 
	$array[4]["desc"] = "Val"; 
	$array[4]["num"] = "12";
	 
	$arAgrupadores = array( "uf", "desc" );
	$boFim = false;
	
	?>
	<script type="text/javascript">

		d_<?php echo $cxcid ?> = new dTree('d_<? echo $cxcid ?>');

		d_<? echo $cxcid ?>.config.folderLinks = true;
		d_<? echo $cxcid ?>.config.useIcons = true;
		d_<? echo $cxcid ?>.config.useCookies = true; 
		
		//alert(screen.width);
		var texto = "";

		d_<? echo $cxcid ?>.add(0,-1,'<b>Áreas</b>');
		<?php
		
		$sql = "select * from painel.eixo order by exodsc";
		$eixos = $db->carregar($sql);
		$eixos = $eixos ? $eixos : array();
		
		foreach ( $eixos as $eixo ) :?>
	
			//formação do eixo
		    texto = '<a style="cursor:pointer"  onmouseover="this; return escape(\'<?=$eixo["exodsc"]?>\');"><b><?=strlen($eixo["exodsc"]) > 50 ? substr($eixo["exodsc"],0,50)."..." : $eixo["exodsc"]?></b></a>';
			//imprime eixo
			d_<? echo $cxcid ?>.add(<?=$eixo["exoid"]?>,<?=$estrutura["exodsc"] ? $estrutura["exodsc"] : 0;?>,' '+texto,'','','','../imagens/eixos-mini2.png','../imagens/eixos-mini2.png');
		    <?php
		    //Nível 2
		    $sql = "select distinct
						aca.acadsc,
						aca.acaid
					from
						painel.acao aca
					inner join
						painel.indicador ind ON ind.acaid = aca.acaid
					where
						ind.exoid = {$eixo["exoid"]}
					and
						ind.indpublicado is true
					and
						ind.indstatus = 'A' 
					and
						aca.acastatus = 'A'
					order by
						aca.acadsc";
			$acao = $db->carregar($sql);
			
			!$acao? $acao = array() : $acao = $acao;
			
			foreach($acao as $ac):
				?>
				//formação da ação
			    texto = '<a style="cursor:pointer"  onmouseover="this; return escape(\'<?=$ac["acadsc"]?>\');"><?=strlen($ac["acadsc"]) > 50 ? substr($ac["acadsc"],0,50)."..." : $ac["acadsc"]?></a>';
				//imprime o programa
				d_<? echo $cxcid ?>.add(999<?=$ac["acaid"]?>,<?=$eixo["exoid"]?>,' '+texto,'','','','../includes/dtree/img/folder.gif','');
				<?php
				//Nível 3
				$sql = "select distinct
							ind.indnome,
							ind.indid
						from
							painel.acao aca
						inner join
							painel.indicador ind ON ind.acaid = aca.acaid
						where
							ind.exoid = {$eixo['exoid']}
						and
							ind.indpublicado is true
						and
							aca.acastatus = 'A'
						and
							aca.acaid = {$ac['acaid']}
						and 
							ind.indstatus = 'A'
						order by
							ind.indnome";
				$indicador = $db->carregar($sql);
				!$indicador? $indicador = array() : $indicador = $indicador; 
					foreach($indicador as $ind):

					//Verifica se o Indicador possui Série Histórica
						$sql = "select
									coalesce(sehqtde,0) as sehqtde
								from
									painel.seriehistorica
								where
									indid = {$ind['indid']}";
					
						$sehqtde = $db->pegaUm($sql);
						if($sehqtde){
							$img = "../imagens/odometro.png";
								$link = "<a style=\"cursor:pointer;\" onclick=\"exibeDashBoard(".$ind['indid'].")\" >".(strlen($ind["indnome"]) > 50 ? substr($ind["indnome"],0,50)."..." : $ind["indnome"])."</a>";
						}else{
							$img = "../imagens/odometro_01.png";
							$link = "<a style=\"cursor:pointer;\" onclick=\"alert(\'Série Histórica Não Atribuída.\')\" >".(strlen($ind["indnome"]) > 50 ? substr($ind["indnome"],0,50)."..." : $ind["indnome"])."</a>";
						}
						?>
						//formação do indicador
					    texto = '<span style="cursor:pointer" onmouseover="this; return escape(\'<?=$ind['indnome']?>\');" ><i><?=$link?></i></span>';
						//imprime o programa
						d_<? echo $cxcid ?>.add(99999<?=$ind["indid"]?>,999<?=$ac["acaid"]?>,' '+texto,'','','','<?=$img;?>','');
						<?php
					endforeach;
			endforeach;
		endforeach; ?>
		elemento = document.getElementById( 'arvore_<? echo $cxcid ?>' );
		elemento.innerHTML = d_<? echo $cxcid ?>;

		//-->
</script>
	<?php
}

function atualizaTabelaIndicador($indid,$parametros = null){
	global $db;
	
	$parametros = str_replace("geraGrafico.php?","",$parametros);
	
	$arrPar = explode(";",$parametros);
	
	foreach($arrPar as $dado){
		
		$d = explode("=",$dado);
		
		$arrparametros[ $d[0] ] = $d[1]; 
		
	}
	
	extract($arrparametros);

		if($tidid1 && $tidid1!= "todos" && $tidid1 != "null"){
			if(strstr($tidid1,",")){
				$arrTidid1 = explode(",",$tidid1);
				foreach($arrTidid1 as $td){
					if($td){
						$arrTid1[] = $td;
					}
				}
			}else{
				$arrTid1[] = $tidid1;
			}
			$sqld1 = "select 
						tdiid 
					from 
						painel.detalhetipodadosindicador
					where
						tidid in (".implode(",",$arrTid1).")";
			$tiid1 = $db->pegaUm($sqld1);
			
			$detalhe_1 = array( $tiid1 => array($tidid1));
		}
		if($tidid2 && $tidid2!= "todos" && $tidid2 != "null"){
			if(strstr($tidid2,",")){
				$arrTidid2 = explode(",",$tidid2);
				foreach($arrTidid2 as $td){
					if($td){
						$arrTid2[] = $td;
					}
				}
			}else{
				$arrTid2[] = $tidid2;
			}
			$sqld2 = "select 
						tdiid 
					from 
						painel.detalhetipodadosindicador
					where
						tidid in (".implode(",",$arrTid2).")";
			$tiid2 = $db->pegaUm($sqld2);
			
			$detalhe_2 = array($tiid2 => array($tidid2));
		}
		
		
		if($tiid1 && !$tiid2){
			$arrDetalhe = array($tiid1);
		}elseif(!$tiid1 && $tiid2){
			$arrDetalhe = array($tiid2);
		}elseif($tiid1 && $tiid2){
			$arrDetalhe = array($tiid1,$tiid2);
		}else{
			$arrDetalhe = array("naoexibir");
		}
				
		$dpeid = array($dpeid,$dpeid2);
		$muncod = !$muncod  || $muncod == "todos" || $muncod == "null" ? "" : $muncod;
		$estuf = !$estuf || $estuf == "todos" || $estuf == "null" ? "" : $estuf;
		$regcod = !$regcod || $regcod == "todos" || $regcod == "null" ? "" : $regcod;
		$gtmid = !$gtmid || $gtmid == "todos" || $gtmid == "null" ? "" : $gtmid;
		$tpmid = !$tpmid || $tpmid == "todos" || $tpmid == "null" ? "" : $tpmid;
		
		$arrFiltros = array( 
							"muncod" => $muncod , 
							"estuf" => $estuf, 
							"regcod" => $regcod, 
							"gtmid" => $gtmid, 
							"tpmid" => $tpmid,
							"dpeid" => $dpeid,
							"tidid_1" => $detalhe_1,
							"tidid_2" => $detalhe_2,
							"zonid" => $zonid,
							"subid" => $subid,
							"disid" => $disid,
							"setid" => $setid
							);
							
		/* Início - Filtro por regvalue*/
		//Filtro por regvalue
		if($regvalue && $regvalue != "" && $regvalue != "todos"){
			$sql = "select rgaidentificador,rgafiltro from painel.regagreg where regid = (select regid from painel.indicador where indid = $indid )";
			$campoReg = $db->pegaLinha($sql);
			$campoRegValue = str_replace(array("d."," ","AND","and","='{".$campoReg['rgaidentificador']."}'"),"",$campoReg['rgafiltro']);
			$arrFiltros[$campoRegValue] = $regvalue;
		}
		/* Fim - Filtro por regvalue*/

		$periodo = $periodicidade;
							
		exibeTabelaIndicador($indid,$arrDetalhe,$periodo,array("quantidade","valor"),$arrFiltros, array(1,1), $regionalizador ,null,null,false,true,true);
	
}

function db_exibeSerieHistorica($indid,$parametros = null){
	global $db;
	
	define("REGIONALIZACAO_ESCOLA",2);
	define("REGIONALIZACAO_IES",5);
	define("REGIONALIZACAO_MUN",4);
	define("REGIONALIZACAO_UF",6);
	define("REGIONALIZACAO_BRASIL",1);
	define("REGIONALIZACAO_POSGRADUACAO",7);
	define("REGIONALIZACAO_CAMPUS_ED_SUP",8);
	define("REGIONALIZACAO_CAMPUS_ED_PROF",9);
	define("REGIONALIZACAO_UNIVERSIDADE",10);
	define("REGIONALIZACAO_FED_CIE_TEC",11);
	
	if($parametros){
//		$dados = explode(";",$parametros);
//		$indid = trim($dados[0]);
//		$tipo  = trim($dados[1]);
//		switch($tipo){
//			case "barra":
//				//indid + ";" + tipografico + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + + ";" + indice_moeda;
//				$dpeid = trim($dados[2]);
//				$dpeid2 = trim($dados[3]);
//				$unidade_inteiro = trim($dados[4]);
//				$unidade_moeda = trim($dados[5]);
//				$indice_moeda = trim($dados[6]);
//				$estuf = trim($dados[7]);
//				$muncod = trim($dados[8]);
//				$gtmid = trim($dados[9]);
//				$tpmid = trim($dados[10]);
//				$tidid1 = trim($dados[11]);
//				$tidid2 = trim($dados[12]);
//				$regcod = trim($dados[13]);
//			break;
//			case "linha":
//				//indid + ";" + tipografico + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + + ";" + indice_moeda;
//				$dpeid = trim($dados[2]);
//				$dpeid2 = trim($dados[3]);
//				$unidade_inteiro = trim($dados[4]);
//				$unidade_moeda = trim($dados[5]);
//				$indice_moeda = trim($dados[6]);
//				$estuf = trim($dados[7]);
//				$muncod = trim($dados[8]);
//				$gtmid = trim($dados[9]);
//				$tpmid = trim($dados[10]);
//				$tidid1 = trim($dados[11]);
//				$tidid2 = trim($dados[12]);
//				$regcod = trim($dados[13]);
//			break;
//			case "barra_fatia":
//				// + indid + ";barra_fatia;" + sehid + ";tdiid" + fatia + "_" + idFatia + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + + ";" + indice_moeda;
//				$sehid = trim($dados[2]);
//				$tidid = trim($dados[3]);
//				$dpeid = trim($dados[4]);
//				$dpeid2 = trim($dados[5]);
//				$unidade_inteiro = trim($dados[6]);
//				$unidade_moeda = trim($dados[7]);
//				$indice_moeda = trim($dados[8]);
//				$estuf = trim($dados[9]);
//				$muncod = trim($dados[10]);
//				$gtmid = trim($dados[11]);
//				$tpmid = trim($dados[12]);
//				$tidid1 = trim($dados[13]);
//				$tidid2 = trim($dados[14]);
//				$regcod = trim($dados[15]);
//			break;
//			case "pizza":
//				// + indid + ";barra_fatia;" + sehid + ";tdiid" + fatia + "_" + idFatia + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + + ";" + indice_moeda;
//				$sehid = trim($dados[2]);
//				$tidid = trim($dados[3]);
//				$dpeid = trim($dados[4]);
//				$dpeid2 = trim($dados[5]);
//				$unidade_inteiro = trim($dados[6]);
//				$unidade_moeda = trim($dados[7]);
//				$indice_moeda = trim($dados[8]);
//				$estuf = trim($dados[9]);
//				$muncod = trim($dados[10]);
//				$gtmid = trim($dados[11]);
//				$tpmid = trim($dados[12]);
//				$tidid1 = trim($dados[13]);
//				$tidid2 = trim($dados[14]);
//				$regcod = trim($dados[15]);
//			break;
//			default:
//				$sehid = trim($dados[2]);
//				$tidid = trim($dados[3]);
//				$dpeid = trim($dados[4]);
//				$dpeid2 = trim($dados[5]);
//				$unidade_inteiro = trim($dados[6]);
//				$unidade_moeda = trim($dados[7]);
//				$indice_moeda = trim($dados[8]);
//				$estuf = trim($dados[9]);
//				$muncod = trim($dados[10]);
//				$gtmid = trim($dados[11]);
//				$tpmid = trim($dados[12]);
//				$tidid1 = trim($dados[13]);
//				$tidid2 = trim($dados[14]);
//				$regcod = trim($dados[15]);
//			break;
//		}

		$parametros = "tipo=".$parametros;
	
		$arrPar = explode(";",$parametros);
		
		foreach($arrPar as $dado){
			
			$d = explode("=",$dado);
			
			$arrparametros[ $d[0] ] = $d[1]; 
			
		}
		
		extract($arrparametros);
		
		
	}
		
	/*
	 * Se o indicador for 'percentual' os valores não podem ser somados
	 */
	$sql = "select regid,unmid,indshformula,formulash from painel.indicador where indid = $indid";
	$regid = $db->pegaLinha($sql);
	
	//Filtro por regvalue
	$sql = "select 
				regdescricao, 
				rgaidentificador,
				rgafiltro 
			from 
				painel.regagreg reg1
			inner join
				painel.regionalizacao reg2 ON reg1.regid = reg2.regid 
			where 
				reg1.regid = {$regid['regid']} 
			and 
				regsqlcombo is not null";
	$campoReg = $db->pegaLinha($sql);
	/* Fim - Filtro por regvalue*/
	
	
	if($campoReg && $regid['unmid'] == UNIDADEMEDICAO_RAZAO && !$regvalue){
		die("<center><br />Indicador de razão! Por favor, selecione o(a) {$campoReg['regdescricao']} e tente novamente!</center>");
	}
	
	$arrRegPercent = array(REGIONALIZACAO_MUN,REGIONALIZACAO_UF,REGIONALIZACAO_BRASIL); //array de regionalização que podem exibir porcentagem
	// Se a regionalização do indicador for IES, POS, ESCOLA não exibe Série Histórica
	if($regid['indshformula'] != "t" && $regid['unmid'] == 1 && !in_array($regid['regid'],$arrRegPercent)){
		echo "Não existem informações disponíveis.";
		return false;	
	}
	// Se a regionalização do indicador for MUNICIPAL e não existir municipio selecionado não exibe a Sèrie Histórica
	if($regid['unmid'] == 1 && $regid['regid'] == REGIONALIZACAO_MUN && (!$muncod || $muncod == "todos")){
		echo "Não existem informações disponíveis.";
		return false;	
	}
	// Se a regionalização do indicador for ESTADUAL e não existir estado selecionado não exibe a Sèrie Histórica
	if($regid['unmid'] == 1 && $regid['regid'] == REGIONALIZACAO_UF && (!$estuf || $estuf == "todos")){
		echo "Não existem informações disponíveis.";
		return false;	
	}
	
	//Aplica Escala
	if(($unidade_moeda && $unidade_moeda != "null") || ($unidade_inteiro && $unidade_inteiro != "null")){
		$escala = $unidade_moeda != "null" ? $unidade_moeda : $unidade_inteiro; 
	}else{
		$escala = 1;
	}
	//Filtra pelas datas informadas
	if($dpeid && $dpeid != "null"){
		$whereData = "and dpeid = $dpeid";
	}
	if($dpeid && $dpeid != "null" && $dpeid2 && $dpeid2 != "null"){
		$sql = "select dpedatainicio from painel.detalheperiodicidade where dpeid = $dpeid";
		$data1 = $db->pegaUm($sql);
		$sql = "select dpedatainicio from painel.detalheperiodicidade where dpeid = $dpeid2";
		$data2 = $db->pegaUm($sql);
		$whereData = "and
						data between '{$data1}' and '{$data2}' ";
	}
	//Aplica o filtro de Região / Estado / Município
	if($regcod && $regcod != "todos"){
		$andRegiao = " 	and 
							dshuf in ( select estuf from territorios.estado where regcod = '$regcod' ) ";
	}if($estuf && $estuf != "todos"){
		$andRegiao = " 	and 
							dshuf = '$estuf' ";
	}if($muncod && $muncod != "todos"){
		$andRegiao .= "	and 
							dshcodmunicipio = '$muncod' ";
	}if((!$regcod || $regcod == "todos") && (!$estuf || $estuf == "todos")){
		$andRegiao = " ";
	}
	
	//Aplica o filtro de Detalhe
	if($tidid1 && $tidid1 != "todos"){
		$sql = "select 
					tdinumero 
				from 
					painel.detalhetipoindicador dti 
				inner join
					painel.detalhetipodadosindicador dtdi ON dtdi.tdiid = dti.tdiid 
				where 
					dtdi.tidid = $tidid1";
		$tdinumero = $db->pegaUm($sql);
		$andDetalhe = " and tidid$tdinumero = $tidid1 ";
	}
	if(!$tidid1 || $tidid1 == "todos"){
		$andDetalhe = "";
	}
	if($tidid2 && $tidid2 != "todos"){
		$sql = "select 
					tdinumero 
				from 
					painel.detalhetipoindicador dti 
				inner join
					painel.detalhetipodadosindicador dtdi ON dtdi.tdiid = dti.tdiid 
				where 
					dtdi.tidid = $tidid2";
		$tdinumero2 = $db->pegaUm($sql);
		$andDetalhe .= " and tidid$tdinumero2 = $tidid2 ";
	}
	
	//Aplica filtro de Tipo de Município
	if(!$tpmid || $tpmid == "todos"){
		$andGrupoMunicipio = " ";
	}if($tpmid && $tpmid != "todos"){
		$andGrupoMunicipio = " and
						dshcodmunicipio in 	(
										select muncod from territorios.muntipomunicipio where tpmid = $tpmid
									)";	
	}
	
	/* Início - Filtro por regvalue*/
	//Filtro por regvalue
	if($regvalue && $regvalue != "" && $regvalue != "todos"){
		$sql = "select rgaidentificador,rgafiltro from painel.regagreg where regid = {$regid['regid']}";
		$campoReg = $db->pegaLinha($sql);
		$andRegValue = " and ".str_replace(array("AND","and","{".$campoReg['rgaidentificador']."}"),array("","",$regvalue),$campoReg['rgafiltro'])." ";
	}
	/* Fim - Filtro por regvalue*/
	
	//Aplica filtro de Grupo de Município
	if($gtmid && $gtmid != "todos" && $gtmid != "")
		$andGrupoMunicipio .= " and dshcodmunicipio in (select muncod from territorios.muntipomunicipio where tpmid in (select tpmid from territorios.tipomunicipio where gtmid = $gtmid ) )";

	if($regid['indshformula'] != "t" || !$regid['formulash']){
		$qtde = "sum(coalesce(qtde,0))";
	}else{
		$qtde = trataFormulaPorcentagem($regid['formulash'],"d");
	}
		
	$sql = "select 
					data,
					dpedsc,
					$qtde as qtde,
					sum(coalesce(valor,0)) as valor 
			from
				painel.v_detalheindicadorsh d
			where 
				indid = {$indid}
			$whereData
			$andRegiao
			$andGrupoMunicipio
			$andDetalhe
			$andRegValue 
			and 
				sehstatus <> 'I'
			group by
				data,dpedsc,dpeid
			order by
				data,dpedsc";
	
	$dados = $db->carregar($sql);
	if($dados){
	
		$sql = "select 
					umedesc 
				from 
					painel.indicador i
				left join 
					painel.unidademeta u on i.umeid = u.umeid
				where 
					indid = {$indid}";
		$umedesc = $db->pegaUm($sql);
		($umedesc == "")? $umedesc = "Valor / Qtd." : $umedesc = $umedesc;
		
		$sql = "select 
					unmdesc 
				from 
					painel.indicador i
				left join 
					painel.unidademedicao u on i.unmid = u.unmid
				where 
					indid = {$indid}";
		$unmdesc = $db->pegaUm($sql);
		($unmdesc == "")? $unmdesc = "Razão" : $unmdesc = $unmdesc;
		
		$sql = "select
					indqtdevalor,
					perid
				from
					painel.indicador
				where 
					indid = {$indid}";
		
		$ind = $db->pegaLinha($sql);
		
		if($ind['indqtdevalor'] == 't' && $unmdesc != "Moeda"){
			
			switch($escala){
					case 1000:
						$umedesc = $umedesc." (Milhares)";
						break;
					case 1000000:
						$umedesc = $umedesc." (Milhões)";
						break;
					case 1000000000:
						$umedesc = $umedesc." (Bilhões)";
						break;
					default:
						$umedesc = $umedesc;
						break;
				}
					
			$cabecalho = array("Referência",$umedesc,"Valor");
			
			if( $dados ){
				
				foreach($dados as $chave => $val){	
					
					//Verifica o tipo de medição e aplica as regras
					switch($unmdesc){
						case "Número inteiro":
							$val['qtde'] = $val['qtde'] / $escala;
							$val['qtde'] = str_replace(",0","",number_format( $val['qtde'], 1, ',', '.'))." ";					
							break;
						case "Percentual":
							$val['qtde'] = $val['qtde'] / $escala;
							$val['qtde'] = number_format($val['qtde'],2,'.','')."%";
							break;
						case "Razão":
							$val['qtde'] = $val['qtde'] / $escala;
							$val['qtde'] = number_format($val['qtde'],2,'.','');
							break;
						case "Número índice":
							$val['qtde'] = $val['qtde'] / $escala;
							$val['qtde'] = number_format($val['qtde'],2,'.','');
							break;
						case "Moeda":
							$val['qtde'] = $val['qtde'] / $escala;
							$val['qtde'] = number_format($val['qtde'],2,',','.');
							break;
						default:
							$val['qtde'] = $val['qtde'] / $escala;
							$val['qtde'] = $val['qtde'];
							break;
					}
					
					
					$dados_array[$chave] = array("dpedsc" => $val['dpedsc'],
												 "qtde" => "<div style=\"width:100%;text-align:right;color:#0066CC\" >".$val['qtde']."</div>",
												 "valor" => "<div style=\"width:100%;text-align:right;color:#0066CC\" >".number_format($val['valor'],2,',','.') ."</div>" );
				}
			}
			
		}
		else{
			
			switch($escala){
					case 1000:
						$umedesc = $umedesc." (Milhares)";
						break;
					case 1000000:
						$umedesc = $umedesc." (Milhões)";
						break;
					case 1000000000:
						$umedesc = $umedesc." (Bilhões)";
						break;
					default:
						$umedesc = $umedesc;
						break;
				}
			
			$cabecalho = array("Referência",$umedesc);
			
			if( $dados ){
				
				foreach($dados as $chave => $val){
					
					//Aplica o índice IPCA
					if($indice_moeda && $indice_moeda != "null"){
						$sql = "select ipcindice from painel.ipca where ipcstatus = 'A' and ipcano = '".$val['dpedsc']."'";
						$ipca = $db->pegaUm($sql);
						$ipca = !$ipca ? 1 : $ipca;
					}else{
						$ipca = 1;
						
						
					}
	
					//Verifica o tipo de medição e aplica as regras
					switch($unmdesc){
						case "Número inteiro":
							$val['qtde'] = ($val['qtde'] * $ipca) / $escala;
							$val['qtde'] = str_replace(",0","",number_format( $val['qtde'], 1, ',', '.'))." ";
							break;
						case "Percentual":
							$val['qtde'] = ($val['qtde'] * $ipca) / $escala;
							$val['qtde'] = number_format($val['qtde'],2,'.','')."%";
							break;
						case "Razão":
							$val['qtde'] = ($val['qtde'] * $ipca) / $escala;
							$val['qtde'] = number_format($val['qtde'],2,'.','');
							break;
						case "Número índice":
							$val['qtde'] = ($val['qtde'] * $ipca) / $escala;
							$val['qtde'] = number_format($val['qtde'],2,'.','');
							break;
						case "Moeda":
							$val['qtde'] = ($val['qtde'] * $ipca) / $escala;
							$val['qtde'] = number_format($val['qtde'],2,',','.');
							break;
						default:
							$val['qtde'] = ($val['qtde'] * $ipca) / $escala;
							$val['qtde'] = $val['qtde'];
							break;
					}
					
					$dados_array[$chave] = array("dpedsc" => $val['dpedsc'],
												 "qtde" => "<div style=\"width:100%;text-align:right;color:#0066CC\" >".$val['qtde']."</div>");
				}
			}
		}
		
		$db->monta_lista_array($dados_array, $cabecalho, 500, 20, '', 'center', '');
	}else{
		echo "Não existem registros.";
	}
	
}

function verificaExibeGrafico($indid,$estuf,$muncod,$gtmid = null,$tpmid = null,$tidid1 = null,$tidid2 = null){
	global $db;
	
	$sql = "select regid,unmid,indshformula,formulash from painel.indicador where indid = $indid";
	$dados = $db->pegaLinha($sql);
		
	/* ********* *  INÍCIO - REGRA PARA PROCENTAGEM E ÍNDICE * ********* */
	if($dados['indshformula'] != "t" && ($dados['unmid'] == UNIDADEMEDICAO_PERCENTUAL || $dados['unmid'] == UNIDADEMEDICAO_NUM_INDICE) && $dados['regid'] != REGIONALIZACAO_BRASIL){
		if($dados['regid'] == REGIONALIZACAO_MUN && (!$muncod || $muncod == "todos" || $muncod == "")){
			echo "<center><span style=\"color:#990000\" >Favor selecionar o Municípo.</span></center>";
			die;
		}
		
		if($dados['regid'] == REGIONALIZACAO_UF && (!$estuf || $estuf == "todos" || $estuf == "")){
			echo "<center><span style=\"color:#990000\" >Favor selecionar o Estado.</span></center>";
			die;
		}
		
		echo "true";
		
	}else{
//		ver(123,d);
		echo "true";
	}
	/* ********* *  FIM - REGRA PARA PROCENTAGEM E ÍNDICE * ********* */
	
}

function editarNomeAba($abaid,$abadsc){
	global $db;
	$sql = "update painel.aba set abadsc = '$abadsc', abadata = now() where abaid = $abaid";
	$db->executar($sql);
	$db->commit($sql);
	
}

function excluirAba($abaid){
	global $db;
	$sql = "update painel.aba set abastatus = 'I' where abaid = $abaid";
	$db->executar($sql);
	$db->commit($sql);
	
}
function compartilharAba($abaid){	
	global $db;
	$sql = "update painel.aba set abacompartilhada = 't', abadata = now() where abaid = $abaid";
	$db->executar($sql);
	$db->commit($sql);
	
}

function db_exibeGraficos($indid){
	global $db;
	
	?>
	<div style="padding:2px;">
	<?=exibeFonterafico($indid);?>
	</div>
	<script type="text/javascript">
	swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_indicador", "100%", "90%", "9.0.0", "expressInstall.swf", {"data-file":"geraGrafico.php?tipo=<?php echo $indid ?>;linha","loading":"Carregando gráfico..."} );
	</script>
	<div id="grafico_indicador"></div>
	<div id="legenda_grafico">
	</div>

<?php
	
}
//Exibe a legenda do Gráfico na DashBoard
function exibeLegendaGrafico($indid,$sehid,$tdiid,$valorTdiid,$dpeid_inicio = null,$dpeid_fim = null,$estuf = null,$muncod = null,$tidid1 = null,$tidid2 = null,$detalhe = null,$valorDetalhe = null){
	global $db;
		
	if($valorTdiid){
		$sql = "select 
					tdinumero 
				from 
					painel.detalhetipoindicador dti 
				inner join
					painel.detalhetipodadosindicador dtdi ON dtdi.tdiid = dti.tdiid 
				where 
					dtdi.tdiid = $valorTdiid";
		$tdinumero = $db->pegaUm($sql);
				
		$leg_tidid = "tdiid{$tdinumero}_{$valorTdiid}";
	}
		
	if($leg_tidid && $leg_tidid != ""){
	
		$fatia = explode("_",$leg_tidid);
		
		if($fatia[0] == "tdiid1"){
			$campos = "	tidid1,
						tiddsc1";
			$exibe = "tiddsc1";
			$exibeId = "tidid1";
		}
		if($fatia[0] == "tdiid2"){
			$campos = "	tidid2,
						tiddsc2";
			$exibe = "tiddsc2";
			$exibeId = "tidid2";
		}
		
		$where = "where 
					1 = 1 ";
	}
	
	//Estado
	if($estuf && $estuf != "todos" && $estuf != "null"){
		$sql_estuf = "and dshuf = '$estuf' ";
		if($muncod && $muncod != "todos" && $muncod != "null"){
			$sql_estuf .= " and dshcodmunicipio = '$muncod' ";
		}
	}
	
	//Grafico Pizza
	//Verifica se o indicador é cumulativo
	$sql = "select indcumulativo from painel.indicador where indid = $indid";
	$indcumulativo = $db->pegaUm($sql);

	if($dpeid_inicio && !$dpeid_fim && $indcumulativo == "S"){
		$sql = "select dpedatainicio from painel.detalheperiodicidade where dpeid = $dpeid_inicio";
		$data = $db->pegaUm($sql);
		$slq_dpeid = "and data <= '$data' ";
	}if($dpeid_inicio && !$dpeid_fim && $indcumulativo == "N"){
		$sql = "select dpedatainicio from painel.detalheperiodicidade where dpeid = $dpeid_inicio";
		$data = $db->pegaUm($sql);
		$slq_dpeid = "and data = '$data' ";
	}
	if($sehid && !$dpeid_inicio && $indcumulativo == "S"){
		$sql = "select 
					dpe.dpedatainicio 
				from 
					painel.detalheperiodicidade dpe
				inner join
					painel.seriehistorica seh ON seh.dpeid = dpe.dpeid 
				where 
					seh.sehid = $sehid";
		$data = $db->pegaUm($sql);
		$slq_dpeid = "and data <= '$data' ";
	}if($sehid && !$dpeid_inicio && $indcumulativo == "N"){
		$sql = "select 
					dpe.dpedatainicio 
				from 
					painel.detalheperiodicidade dpe
				inner join
					painel.seriehistorica seh ON seh.dpeid = dpe.dpeid 
				where 
					seh.sehid = $sehid";
		$data = $db->pegaUm($sql);
		$slq_dpeid = "and data = '$data' ";
	}
	
	if($dpeid_inicio && $dpeid_fim){
		$sql = "select dpedatainicio from painel.detalheperiodicidade where dpeid = $dpeid_inicio";
		$data1 = $db->pegaUm($sql);
		$sql = "select dpedatainicio from painel.detalheperiodicidade where dpeid = $dpeid_fim";
		$data2 = $db->pegaUm($sql);
		$slq_dpeid = "and
						data between '{$data1}' and '{$data2}' ";
	}
	
	//Aplica o filtro de Detalhe
	if($tidid1 && $tidid1 != "todos"){
		$sql = "select 
					tdinumero 
				from 
					painel.detalhetipoindicador dti 
				inner join
					painel.detalhetipodadosindicador dtdi ON dtdi.tdiid = dti.tdiid 
				where 
					dtdi.tidid = $tidid1";
		$tdinumero = $db->pegaUm($sql);		
		$slq_tdiid .= " and tidid$tdinumero = $tidid1 ";
	}
	
	if($tidid2 && $tidid2 != "todos"){
		$sql = "select 
					tdinumero 
				from 
					painel.detalhetipoindicador dti 
				inner join
					painel.detalhetipodadosindicador dtdi ON dtdi.tdiid = dti.tdiid 
				where 
					dtdi.tidid = $tidid2";
		$tdinumero2 = $db->pegaUm($sql);
		$slq_tdiid .= " and tidid$tdinumero2 = $tidid2 ";
	}
	
	//Aplica o filtro de regionalizador (POPUP)
	if($detalhe && $detalhe != "" && $detalhe != "undefined" && $valorDetalhe && $valorDetalhe != "" && $valorDetalhe != "undefined"){
		switch($detalhe){
			case "pais":
				$andDetalheRegAgreg = "";
			break;
			case "escola":
				$andDetalheRegAgreg = " and dshcod = '$valorDetalhe' ";
			break;
			case "ies":
				$andDetalheRegAgreg = " and dshcod = '$valorDetalhe' ";
			break;
			case "posgraduacao":
				$andDetalheRegAgreg = " and iepid = '$valorDetalhe' ";
			break;
			case "universidade":
				$andDetalheRegAgreg = " and unicod = '$valorDetalhe' ";
			break;
			case "instituto":
				$andDetalheRegAgreg = " and unicod = '$valorDetalhe' ";
			break;
			case "hospital":
				$andDetalheRegAgreg = " and entid = '$valorDetalhe' ";
			break;
			case "campussuperior":
				$andDetalheRegAgreg = " and entid = '$valorDetalhe' ";
			break;
			case "campusprofissional":
				$andDetalheRegAgreg = " and entid = '$valorDetalhe' ";
			break;
			default:
				$andDetalheRegAgreg = "";
		}
	}
	
	//Pega o detalhe da série histórica e fatia pelo detalhamento
	$leg_sql = "select distinct
				$campos
			from 
				painel.v_detalheindicadorsh 
			$where
			and
				indid = $indid
			and
				sehstatus <> 'I'
			$slq_dpeid
			$slq_sehid
			$sql_estuf
			$slq_tdiid
			$andDetalheRegAgreg
			group by
				$campos
			order by
				$campos";
	
	$leg_detalhe = $db->carregar($leg_sql);
	$leg_detalhe = !$leg_detalhe ? array() : $leg_detalhe;

	/* Início - Lista todos os detalhes */
	if($valorTdiid && $valorTdiid != "todos"){
		$sql = "select distinct 
					tidid
				from
					painel.detalhetipodadosindicador
				where
					tdiid = $valorTdiid
				and
					tidstatus = 'A'
				order by
					tidid";
		$arrDetalhes = $db->carregar($sql);
		
		$arrCores = array('#6495ED','#66CDAA','#990000','#FFD700','#CDC8B1',' #000000','#FF0000','#008B45','#8B008B','#FFE4E1','#0000FF',' #7CFC00','#8B4513','#FF1493','#00FF00','#00008B','#7FFFD4','#8B8B00','#FF6A6A','#8B1A1A','#8B0A50','#828282');
		$n = 0;
		foreach($arrDetalhes as $detalhe){
			$arrCoresDetalhe[$detalhe['tidid']] = $arrCores[$n];
			$n++;
		}
		
	}
	/* Fim - Lista todos os detalhes */

	echo "<div style=\"background-color:#FFFFFF;width:99%;padding-bottom:5px\" >";
	echo "<table width=98% border=0><tr><td>";
	if($leg_detalhe[0]){ 
		foreach($leg_detalhe as $det){
			echo "<div style=\"float:left;margin:10px;\" >";
			echo " <span style=\"width:10px;height:10px;background-color:".$arrCoresDetalhe[ $det[ $exibeId ] ].";color:".$arrCoresDetalhe[ $det[ $exibeId ] ]."\" >&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span> ".$det[$exibe]."";
			echo "</div>";
		}
	}else{
		echo "<center><span style=\"font-size:14px;color:#990000;\" >Não existem registros.</span></center>";
	}
	echo "</td></tr></table>";
	echo "</div>";
}

function exibeFonterafico($indid){
	global $db;
	
	$sql = "select indfontetermo from painel.indicador where indid = $indid";
	$indFonte = $db->pegaUm($sql);
	$indFonte = !$indFonte ? "Não informada" : $indFonte;
	echo "<div style=\"clear:both\" ><b>Fonte:</b> $indFonte</div>";
		
}
function testaDatasDpeid($dpeid1,$dpeid2){
	global $db;
	
	if($dpeid1 && !$dpeid2){
		echo "true";
		return true;
	}
	if($dpeid2 && !$dpeid1){
		echo "false";
		return true;
	}
	
	if($dpeid1){
		$sql = "select 
					dpedatainicio 
				from 
					painel.detalheperiodicidade 
				where 
					dpeid = $dpeid1";
		$dtDpeid1 = $db->pegaUm($sql);
		$dtDpeid1 = $dtDpeid1 ? strtotime($dtDpeid1) : false;
	}
	
	if($dpeid2){
		$sql = "select 
					dpedatainicio 
				from 
					painel.detalheperiodicidade 
				where 
					dpeid = $dpeid2";
		$dtDpeid2 = $db->pegaUm($sql);
		$dtDpeid2 = $dtDpeid2 ? strtotime($dtDpeid2) : false;
	}
	if($dtDpeid2 < $dtDpeid1){
		echo "true";
	}else{
		echo "false";
	}
}


function exibeMapaDashBoard($visualizaMapa,$indid,$sehid =null,$dpeid_inicio = null,$dpeid_fim = null,$regcod = null,$estuf = null, $muncod = null,$gtmid = null, $tpmid = null,$tidid1 = null,$tidid2 = null,$abaPadrao = null){
	global $db;
	
	/* FILTROS DO MAPA */
	if($estuf == "todos"){
		$estuf = "";
	}
	if($estuf) {
		$filtro .= " AND d.dshuf='".$estuf."'";
	}
	if($muncod == "todos"){
		$muncod = "";
	}
	if($muncod) {
		$filtro .= " AND d.dshcodmunicipio='".$muncod."'";
	}
	if($tpmid == "todos"){
		$tpmid = "";
	}
	if($tpmid) {
		$filtro .= " AND d.dshcodmunicipio IN(SELECT muncod FROM territorios.muntipomunicipio WHERE tpmid='".$tpmid."')";
	}
	if($tidid1) {
		$filtro .= " AND d.tidid1='".$tidid1."'";
	}
	if($tidid2) {
		$filtro .= " AND d.tidid2='".$tidid2."'";
	}
	
	$indicadordados = $db->pegaLinha("SELECT ume.umedesc, ind.regid, ind.indcumulativo, ind.indpublicado, ind.unmid 
									  FROM painel.indicador ind
									  LEFT JOIN painel.unidademeta ume ON ume.umeid = ind.umeid  
									  WHERE indid='".$indid."'");
	
	if($dpeid_inicio && !$dpeid_fim){
	
		$sql = "SELECT sehid FROM painel.seriehistorica
				WHERE indid = {$indid} AND dpeid = {$dpeid_inicio} AND sehstatus <> 'I'";

		$arrsehid = $db->carregar($sql);
		
		$arrsehid = !$arrsehid ? array() : $arrsehid;
		
		foreach($arrsehid as $seh){
			
			$arrdpe[] = $seh['sehid'];
			
		}
		
		
	}elseif($dpeid_inicio && $dpeid_fim){

		$sql = "SELECT distinct sehid
				FROM painel.v_detalheindicadorsh
				WHERE indid = {$indid} AND data BETWEEN (
									SELECT dpedatainicio 
									FROM painel.detalheperiodicidade 
									WHERE dpeid = $dpeid_inicio
								  ) 
							  AND (
								  	SELECT dpedatainicio 
									FROM painel.detalheperiodicidade 
									WHERE dpeid = $dpeid_fim
							  	)
				AND
					sehstatus <> 'I'";
		$arrsehid = $db->carregar($sql);
		
		$arrsehid = !$arrsehid ? array() : $arrsehid;
		
		foreach($arrsehid as $seh){
			
			$arrdpe[] = $seh['sehid'];
			
		}
		
	}
	
	if($arrdpe) {
		
		$filtro .= " AND d.sehid IN('".implode("','", $arrdpe)."')";	
	}
	
	
	if($abaPadrao == "mapa"){
			if($visualizaMapa == "true"){ ?>
				<div style="cursor:pointer;padding:3px;font-weight: bold;width:95%;text-align:left" onclick="escondeMapaDashBoard('<?=$indid ?>');"> <img title="Esconder Mapa" style="vertical-align:middle;" src="../imagens/fechar_globo_terrestre.png" />  Esconder Mapa</div>
			<?php }else{ ?>
				<div style="cursor:pointer;padding:3px;font-weight: bold;width:95%;text-align:left" onclick="visualizaMapaDashBoard('<?=$indid ?>');"> <img title="Visualizar Mapa" style="vertical-align:middle;" src="../imagens/globo_terrestre.png" />  Visualizar Mapa</div>
			<?php } 
	}?>
	
	<table width="100%">
	<tr style="display:none" id='linha_mapa_<?php echo $indid; ?>'>
	<?php
	
	if($abaPadrao == "mapa"){
		if($visualizaMapa == "true"){ ?>
			<td colspan=4 width=50%>
			<div id=div_exibe_mapa_<? echo $indid; ?> style=height:300px; ></div>
			</td>
		<?php }else{ ?>
			<td style="display:none">
			<div style="text-align:center;float:left;display:none;" id=div_exibe_mapa_<? echo $indid; ?>></div>
			</td>
		<?php } ?>
		<td colspan=4 width=50% id=info_mapa_<? echo $indid; ?> valign=top></td>
<?php	} ?>
	</tr>
	</table>
	<script>
	exibeGrafico('<? echo $indid; ?>', '<? echo md5_encrypt($filtro); ?>');
	</script>
<?php
}

function filtraMunicipio($estuf){
	global $db;
	
	$sql = "select 
				muncod,mundescricao 
			from
				territorios.municipio
			where
				estuf = '{$estuf}'
			order by
				mundescricao";
	$municipios = $db->carregar($sql);
	$municipios = !$municipios ? array() : $municipios;
	
	?>
	<select name="muncod" id="muncod" style="width:200px;" onchange="filtraRegionalizador(this.value)" >
		<option value="todos">Todos os Municípios...</option>
	<? foreach($municipios as $mun){?>
		<option value="<? echo $mun['muncod'] ?>"><? echo $mun['mundescricao'] ?></option>
	<? } ?>
	</select>
<?php
}

function filtraMunicipioPorTipoGrupo($tpmid,$estuf){
	global $db;
	
	if(!$tpmid){
		$municipios = array();
	}
	elseif($tpmid && (!$estuf || $estuf == "todos")){
		$sql = "select 
					muncod,mundescricao 
				from
					territorios.municipio
				where
					muncod in 	(
									select muncod from territorios.muntipomunicipio where tpmid = $tpmid 
								)
				order by
					mundescricao";
		$municipios = $db->carregar($sql);
		$municipios = !$municipios ? array() : $municipios;
	}
	elseif($tpmid && ($estuf || $estuf != "todos")){
		$sql = "select 
					muncod,mundescricao 
				from
					territorios.municipio
				where
					muncod in 	(
									select muncod from territorios.muntipomunicipio where tpmid = $tpmid and estuf = '$estuf'
								)
				order by
					mundescricao";
		$municipios = $db->carregar($sql);
		$municipios = !$municipios ? array() : $municipios;
	}
	
	$municipios = !$municipios ? array() : $municipios;
	?>
	<select id="muncod" name="muncod" style="width:200px;" onchange="filtraRegionalizador(this.value)" >
		<option value="todos">Selecione...</option>
	<?php foreach($municipios as $mun){?>
		<option value="<? echo $mun['muncod'] ?>"><? echo $mun['mundescricao'] ?></option>
	<?php } ?>
	</select>
<?php }

function filtraRegionalizador($indid,$muncod,$value = null){
	global $db;
	
	$sql = "select regid from painel.indicador where indid = $indid";
	$regid = $db->pegaUm($sql);
	
	$sql = "	select
					regsqlcombo
				from
					painel.regionalizacao
				where
					regid = $regid";
	$sqmCombo = $db->pegaUm($sql);
	$sqmCombo = str_replace("{where}"," and mun.muncod = '$muncod' ",$sqmCombo);	
	$db->monta_combo('regvalue',$sqmCombo,'S','Selecione...','','','','200','N',"regvalue","",$value);
}

function abrirMunicipios($estuf, $muncod = null, $filtro) {
	global $db;
	
	$filtroB64 = $filtro;
	$filtro = base64_decode($filtro);
	
	$sehid = explode("AND",$filtro);
	
	$sql = "select 
				ind.regid,
				ind.indpublicado 
			from 
				painel.indicador ind
			inner join
				painel.seriehistorica seh ON ind.indid = seh.indid
			left join
				painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid 
			left join
				 painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
			where
				{$sehid[1]}";
	$regid = $db->pegaLinha($sql);
		
	switch($regid['regid']){
		case REGIONALIZACAO_ESCOLA:
			if($regid['indpublicado'] == "t")
				$OnClickMais = " <img id=\"img_mais_' || mun.muncod || '\" onclick=\"abrirEscolas(this,\'' || mun.muncod || '\',\'$filtroB64\')\" style=\"cursor:pointer\" title=\"Visualizar Escolas\" src=\"../imagens/mais.gif\" /> ";
			else
				$OnClickMais = "";
			break;
		case REGIONALIZACAO_POLO:
			if($regid['indpublicado'] == "t")
				$OnClickMais = " <img id=\"img_mais_' || mun.muncod || '\" onclick=\"abrirEscolas(this,\'' || mun.muncod || '\',\'$filtroB64\')\" style=\"cursor:pointer\" title=\"Visualizar Escolas\" src=\"../imagens/mais.gif\" /> ";
			else
				$OnClickMais = "";
			break;
		case REGIONALIZACAO_IES:
			if($regid['indpublicado'] == "t")
				$OnClickMais = " <img id=\"img_mais_' || mun.muncod || '\" onclick=\"abrirIES(this,\'' || mun.muncod || '\',\'$filtroB64\')\" style=\"cursor:pointer\" title=\"Visualizar Instituição de Ensino Superior\" src=\"../imagens/mais.gif\" /> ";
			else
				$OnClickMais = "";
			break;
		case REGIONALIZACAO_POSGRADUACAO:
			if($regid['indpublicado'] == "t")
				$OnClickMais = " <img id=\"img_mais_' || mun.muncod || '\" onclick=\"abrirPOS(this,\'' || mun.muncod || '\',\'$filtroB64\')\" style=\"cursor:pointer\" title=\"Visualizar Instituições com Programas de Pós-Graduação\" src=\"../imagens/mais.gif\" /> ";
			else
				$OnClickMais = "";
			break;
		case REGIONALIZACAO_CAMPUS_SUPERIOR:
			if($regid['indpublicado'] == "t")
				$OnClickMais = " <img id=\"img_mais_' || mun.muncod || '\" onclick=\"abrirUNIVCAM(this,\'' || mun.muncod || '\',\'$filtroB64\')\" style=\"cursor:pointer\" title=\"Visualizar Universidade\" src=\"../imagens/mais.gif\" /> ";
			else
				$OnClickMais = "";
			break;
		case REGIONALIZACAO_HOSPITAL:
				//if($regid['indpublicado'] == "t")
					//$OnClickMais = " <img id=\"img_mais_' || mun.muncod || '\" onclick=\"abrirHospital(this,\'' || mun.muncod || '\',\'$filtroB64\')\" style=\"cursor:pointer\" title=\"Visualizar Campus (Educação Superior)\" src=\"../imagens/mais.gif\" /> ";
			break;
		case REGIONALIZACAO_CAMPUS_PROFISSIONAL:
			//if($regid['indpublicado'] == "t")
				//$OnClickMais = " <img id=\"img_mais_' || mun.muncod || '\" onclick=\"abrirCampusProf(this,\'' || mun.muncod || '\',\'$filtroB64\')\" style=\"cursor:pointer\" title=\"Visualizar Campus (Educação Profissional)\" src=\"../imagens/mais.gif\" /> ";
			break;
		case REGIONALIZACAO_UNIVERSIDADE:
			//if($regid['indpublicado'] == "t")
				//$OnClickMais = " <img id=\"img_mais_' || mun.muncod || '\" onclick=\"abrirUniversidade(this,\'' || mun.muncod || '\',\'$filtroB64\')\" style=\"cursor:pointer\" title=\"Visualizar Universidade\" src=\"../imagens/mais.gif\" /> ";
			break;
		case REGIONALIZACAO_INSTITUTO:
			//if($regid['indpublicado'] == "t")
				//$OnClickMais = " <img id=\"img_mais_' || mun.muncod || '\" onclick=\"abrirInstituto(this,\'' || mun.muncod || '\',\'$filtroB64\')\" style=\"cursor:pointer\" title=\"Visualizar Instituto Federal de Educação, Ciência e Tecnologia\" src=\"../imagens/mais.gif\" /> ";
			break;
		default:
			$OnClickMais = "";
	}
	//Verifica se existe a possibilidade de perfurar por mais um regionalizador
	
	
	$municipio = $muncod == "false" || !$muncod ? " " : " AND mun.muncod = '$muncod' ";	
	$indicadordados = $db->pegaLinha("SELECT * FROM painel.indicador WHERE indid='".$_SESSION['indid']."'");
		
	//Não existe totalizador se a unidade for 'percentual'
	if($indicadordados['unmid'] == 1 && $indicadordados['regid'] != REGIONALIZACAO_MUN){
		$qtde = "'' as dshqtde ";
		$soma = "N";
	}elseif($indicadordados['unmid'] == 1 && $indicadordados['regid'] == REGIONALIZACAO_MUN){
		$qtde = "".(($indicadordados['unmid']==UNIDADEMEDICAO_MOEDA)?"trim(to_char(SUM(dshqtde), '999g999g999g999d99'))":"trim(to_char(SUM(dshqtde), '9999999999'))")." as dshqtde";
		$soma = "N";	
	}else{
		$qtde = "".(($indicadordados['unmid']==UNIDADEMEDICAO_MOEDA)?"trim(to_char(SUM(dshqtde), '999g999g999g999d99'))":"trim(to_char(SUM(dshqtde), '9999999999'))")." as dshqtde";
		$soma = "S";
	}
	
	if($regid['indpublicado'] == "t")
		$onclick = "onclick=\"amun(\''||mun.muncod||'\');\"";
	else
		$onclick = " ";
	
	$sql = "SELECT ' $OnClickMais <a style=\"cursor:pointer\" $onclick >'||mun.mundescricao||'</a>', $qtde FROM painel.detalheseriehistorica dsh 
		    INNER JOIN territorios.municipio mun ON mun.muncod = dsh.dshcodmunicipio
			INNER JOIN painel.seriehistorica seh ON seh.sehid = dsh.sehid 
			LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
			WHERE indid='".$_SESSION['indid']."' $filtro AND mun.estuf='".$estuf."' $municipio AND seh.sehstatus!='I' GROUP BY mun.muncod, mun.mundescricao ORDER BY mun.mundescricao";
	$cabec = array("Municípios","");
	$db->monta_lista_simples($sql,$cabec,900,1,$soma,'100%','N');
}

	//////////////////////************** paleativo para painel 3 TEMAS (revisar porteriormente)

//Função que monta as abas do painel de controle do usúario
function exibeAbaUsuario2($usucpf,$abaDefault){
	global $db;
	
	//Se não existir CPF retorna falso
	if(!usucpf){
		return false;
	}
	
	// Verifica se existe um conjunto de abas criado
	$sql = "select 
				aba.abaid,
				aba.abadsc
			from
				painel.aba as aba
			inner join
				painel.conteudousuario ctu ON ctu.abaid = aba.abaid
			where
				ctu.usucpf = '{$usucpf}'
			and
				aba.abastatus = 'A'
			order by
				aba.abaid";
	
	//dbg($sql);
	$abas = $db->carregar($sql);
	//dbg($abas);
	//die;		
	if(!$abas){
		$abaid = addNovaAba("Painel de Controle",true);
		
		/* INÍCIO - Adiciona novas caixas default */
		executaAddCaixa("Árvore de Indicadores",10,$abaid);
		/* FIM - Adiciona novas caixas default */
		
		echo "<script>window.location.href='painel2.php?modulo=principal/painel_controle&acao=A&abaid={$abaid}'</script>";
		$menu[0] = array("descricao" => "Painel de Controle", "link"=> "/painel/painel2.php?modulo=principal/painel_controle&acao=A");
	}else{
		$n = 0;
		foreach($abas as $aba){
			$menu[] = array("descricao" => $aba['abadsc'], "link"=> "/painel/painel.php?modulo=principal/painel_controle&acao=A&abaid={$aba['abaid']}");
			if($n == 0)
				$abaid = $aba['abaid'];
			$n++;
		}
	}
	//Adiciona Nova Aba
	
	array_push($menu,array("descricao" => "Adicionar Aba [+]", "link"=> "javascript:exibeAddNovaAba()"));
	//array_unshift($menu,array("descricao" => "Rede Federal", "link"=> "/painel/painel2.php?modulo=principal/redefederal/rede_federal&acao=A"));
	//array_unshift($menu,array("descricao" => "Mapa de Obras", "link"=> "/painel/painel2.php?modulo=principal/mapa_obras&acao=A"));
	array_unshift($menu,array("descricao" => "Indicadores", "link"=> "/painel/painel2.php?modulo=principal/painel_controle&acao=A"));
	echo montarAbasPainel2($menu, $_SERVER['REQUEST_URI']);
	
}

function montarAbasPainel2($itensMenu, $url = false)
{
	$url = $url ? $url : $_SERVER['REQUEST_URI'];
	$url = str_replace("painel.php", "painel2.php",$url);
	
	if (is_array($itensMenu)) {
        $rs = $itensMenu;
    } else {
        global $db;
        $rs = $db->carregar($itensMenu);
    }

    $menu    = '<div id="menu">'
             . '<ul>';

    $nlinhas = count($rs) - 1;

    for ($j = 0; $j <= $nlinhas; $j++) {
        extract($rs[$j]);
		$link = str_replace("painel.php", "painel2.php",$link);
        if ($url == $link) {
			$classe='class="aba_on"';
            $giffundo_aba = 'aba_fundo_sel.gif';
            $cor_fonteaba = '#000055';
            $id = "id=\"exibe_edita_aba\"";
            $evento = " ";
        } else {
			$classe='';
			$id = "";
            $giffundo_aba = 'aba_fundo_nosel.gif';
            $cor_fonteaba = '#4488cc';
            $evento = "window.location.href='".$link."'";
        }

        

        if ($link != $url) {
            
        	if($link == "javascript:exibeAddNovaAba()"){
        		$menu .= '<li '.$classe.' id="exibe_add_aba" onclick="'.$evento.'" ><a>'.$descricao.'</a>';
        		$menu .= '<div id="exibe_escolhe_add_aba" style="display:none;position:absolute;width:122px;height:46px;background-color:#fcfcfc;border:solid 1px #C9C9C9;padding:2px;text-align:left;z-index:1000" >
            			<div onclick="addNovaAba();" style="cursor:pointer;" ><img style="width:16px;height:16px;" src="../imagens/editar_nome.gif" /> Nova Aba</div> 
            			<div onclick="addAbaCompartilhada();" style="border-top:solid 1px #C9C9C9;cursor:pointer;position:absolute;z-index:10;width:100%" ><img style="width:13px;height:16px;padding-top:3px;" src="../imagens/grupo.gif" /> Aba Compartilhada</div>
            			</div></a></li>';
        	}else
        		$menu .= '<li '.$classe.' '.$id.' onclick="'.$evento.'" ><a>'.$descricao.'</a></li>';
        	
        } else {
        	if($link == "/painel/painel2.php?modulo=principal/painel_controle&acao=A&abaid=paginaInicial" || $link == "/painel/painel2.php?modulo=principal/painel_controle&acao=A" || $link == "/painel/painel2.php?modulo=principal/mapa_obras&acao=A" || $link == "/painel/painel2.php?modulo=principal/redefederal/rede_federal&acao=A"){
        		$menu .= '<li '.$classe.' '.$id.'><a>'.$descricao.'</a></li>'; 
        	}else{        	
	            $menu .= '<li '.$classe.' '.$id.' onclick="'.$evento.'" ><a>'.$descricao.' <img src="../imagens/seta_baixo.png" > ';
	            $menu .= '<div id="abaid_'.str_replace("abaid=","",strstr($link, 'abaid=')).'" style="display:none;position:absolute;width:100px;height:66px;background-color:#fcfcfc;border:solid 1px #C9C9C9;padding:2px;text-align:left;z-index:1000" >
	            			<div onclick="alterarNomeAba('.str_replace("abaid=","",strstr($link, 'abaid=')).',\''.$descricao.'\');" style="cursor:pointer;" ><img style="width:16px;height:16px;" src="../imagens/editar_nome.gif" /> Editar Nome</div> 
	            			<div onclick="compartilharAba('.str_replace("abaid=","",strstr($link, 'abaid=')).');" style="border-bottom:solid 1px #C9C9C9;border-top:solid 1px #C9C9C9;cursor:pointer;position:absolute;z-index:10;width:100%" ><img style="width:13px;height:16px;padding-top:3px;" src="../imagens/grupo.gif" /> Compartilhar</div>
	            			<div onclick="excluirAba('.str_replace("abaid=","",strstr($link, 'abaid=')).');" style="cursor:pointer;" ><img style="width:13px;height:16px;padding-top:28px;" src="../imagens/excluir.gif" /> Excluir Aba</div>
	            			</div></a></li>';
        	}
        }
    }

   $menu .= '</ul></div>';

    return $menu;
}
//////////////////*************

/*
 * Função que lista os Municípios selecionados pelo usuário
 */
function exibeListaMunicipios($cxcid){
	global $db;
	
	$sql = "select conteudo from painel.conteudocaixa where cxcid = $cxcid";
	$conteudo = unserialize($db->pegaUm($sql));
	$conteudo = !$conteudo ? array() : $conteudo;
	if($conteudo){
		$sql = "select 
					est.estdescricao,
					est.estuf
				from 
					territorios.estado est
				inner join
					territorios.municipio mun ON mun.estuf = est.estuf 
				where 
					mun.muncod in ('".implode("' ,'",$conteudo)."')
				group by
					est.estuf,estdescricao 
				order by
					est.estuf,estdescricao";
		$estados = $db->carregar($sql);
	echo '<table cellspacing="0" cellpadding="3" style="width: 100%;" >';
	
	//<tr><td colspan="6" style="width: 100%; text-align: center; font-weight: bold; background-color: rgb(201, 201, 201); font-size: 12px; color: rgb(0, 85, 0);">Brasil Alfabetizado</td></tr>
	foreach($estados as $uf){
		echo '<tr><td style="width: 100%; text-align: center; font-weight: bold; background-color: rgb(201, 201, 201); font-size: 12px; color: rgb(0, 85, 0);">'.$uf['estdescricao'].'</td></tr>';
		$sql = "select 
					mun.mundescricao,
					mun.muncod
				from
					territorios.municipio mun 
				where 
					mun.muncod in ('".implode("' ,'",$conteudo)."')
				and
					mun.estuf = '{$uf['estuf']}'
				order by
					mundescricao";
		$municipios = $db->carregar($sql);
		$num = 0; 
		foreach($municipios as $m){
			$cor = ($num % 2 )? "#f5f5f5" : "#fcfcfc";
			echo "<tr bgcolor=\"$cor\" onmouseout=\"this.bgColor='$cor';\" onmouseover=\"this.bgColor='#ffffcc';\">";
			echo "<td style=\"cursor:pointer\"> <img style=\"margin-right:10px;\" onclick=\"exibeLocalizacaoMapa('{$m['muncod']}')\" title=\"Localizar no Mapa\" src=\"../imagens/globo_terrestre.png\" /><span onclick=\"amun('{$m['muncod']}');\" >".$m['mundescricao']."</span></td>";
			echo "</tr>";
			$num++;
		}
	}
	echo "</table>";
	}else{
		echo "<div style=\"padding:10px;text-align:center;cursor:pointer\" onclick=\"addMunicipiosCaixa({$cxcid})\" >Clique aqui para adicionar municípios!</div>";
	}
}

/*
 * Função que lista os Municípios selecionados pelo usuário
 */
function exibeListaUniversidades($cxcid){
	global $db;
	
	$sql = "select conteudo from painel.conteudocaixa where cxcid = $cxcid";
	$conteudo = unserialize($db->pegaUm($sql));
	$conteudo = !$conteudo ? array() : $conteudo;
	if($conteudo){
		
		$sql = "SELECT uni.unicod as codigo, UPPER(uni.unidsc) as descricao FROM public.unidade uni 
				  INNER JOIN entidade.entidade ent ON ent.entunicod = uni.unicod 
				  INNER JOIN entidade.endereco ende ON ende.entid = ent.entid 
				  WHERE 
					uni.unicod in ('".implode("' ,'",$conteudo)."')
				AND 
					uni.orgcod= '". CODIGO_ORGAO_SISTEMA. "' AND gunid=3 AND unistatus='A' ORDER BY descricao";

		$universidades = $db->carregar($sql);
	echo '<table cellspacing="0" cellpadding="3" style="width: 100%;" >';
	echo "<tr><td style=\"text-align: center; font-weight: bold; background-color: rgb(201, 201, 201); font-size: 12px; color: rgb(0, 85, 0);\">Código</td><td style=\"text-align: center; font-weight: bold; background-color: rgb(201, 201, 201); font-size: 12px; color: rgb(0, 85, 0);\">Universidade</td></tr>";
	//<tr><td colspan="6" style="width: 100%; text-align: center; font-weight: bold; background-color: rgb(201, 201, 201); font-size: 12px; color: rgb(0, 85, 0);">Brasil Alfabetizado</td></tr>
	$num = 0; 
	foreach($universidades as $uni){
		$cor = ($num % 2 )? "#f5f5f5" : "#fcfcfc";
		echo "<tr bgcolor=\"$cor\" onmouseout=\"this.bgColor='$cor';\" onmouseover=\"this.bgColor='#ffffcc';\">";
		echo "<td style=\"cursor:pointer\"><span onclick=\"auni('{$uni['codigo']}');\" >".$uni['codigo']."</span></td>";
		echo "<td style=\"cursor:pointer\"><span onclick=\"auni('{$uni['codigo']}');\" >".$uni['descricao']."</span></td>";
		echo "</tr>";
		$num++;
	}
	echo "</table>";
	}else{
		echo "<div style=\"padding:10px;text-align:center;cursor:pointer\" onclick=\"addUniversidadeCaixa({$cxcid})\" >Clique aqui para adicionar universidades!</div>";
	}
}

function exibeListaRegionalizadores($cxcid){
	global $db;
	
$sql = "select conteudo from painel.conteudocaixa where cxcid = $cxcid";
	$conteudo = unserialize($db->pegaUm($sql));
	$conteudo = !$conteudo ? array() : $conteudo;
	if($conteudo){
		if(is_array($conteudo)){
			foreach($conteudo as $regid => $arrConteudo){
				switch($regid){
					case REGIONALIZACAO_ESCOLA :
						if(is_array($arrConteudo)) {
							$sql = "select 
										'<span style=\"cursor:pointer\" onclick=\"aesc(\'' || esccodinep || '\')\" >' || esccodinep || '</span>' as codigo,
										'<span style=\"cursor:pointer\" onclick=\"aesc(\'' || esccodinep || '\')\" >' || escdsc || '</span>' as descricao
									from 
										painel.escola esc
									where
										esccodinep in ('".implode("','",$arrConteudo)."')
									order by
										descricao";
						} else {
							$sql = array();
						}
						$cabecalho = array("Código","Escola");
						echo "<table cellspacing=\"0\" cellpadding=\"3\" style=\"width: 100%;\" ><tr><td style=\"width: 100%; text-align: center; font-weight: bold; background-color: rgb(201, 201, 201); font-size: 12px; color: rgb(0, 85, 0);\">Escola</td></tr></table>";
						$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%','N');
					break;
					
					case REGIONALIZACAO_POLO:
						if(is_array($arrConteudo)) {
							$sql = "select 
										'<span style=\"cursor:pointer\" onclick=\"apol(\'' || polid || '\')\" >' || polid || '</span>' as codigo,
										'<span style=\"cursor:pointer\" onclick=\"apol(\'' || polid || '\')\" >' || poldsc || '</span>' as descricao
									from 
										painel.polo
									where
										esccodinep in ('".implode("','",$arrConteudo)."')
									order by
										descricao";
						} else {
							$sql = array();
						}
						$cabecalho = array("Código","Pólo");
						echo "<table cellspacing=\"0\" cellpadding=\"3\" style=\"width: 100%;\" ><tr><td style=\"width: 100%; text-align: center; font-weight: bold; background-color: rgb(201, 201, 201); font-size: 12px; color: rgb(0, 85, 0);\">Pólo</td></tr></table>";
						$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%','N');
					break;
					
					case REGIONALIZACAO_IES :
						if(is_array($arrConteudo)) {
							$sql = "select 
										'<span style=\"cursor:pointer\" onclick=\"aies(\'' || iesid || '\')\" >' || iesid || '</span>' as codigo,
										'<span style=\"cursor:pointer\" onclick=\"aies(\'' || iesid || '\')\" >' || iesdsc || '</span>' as descricao
									from 
										painel.ies ies
									where
										iesid in ( '".implode("','",$arrConteudo)."' )
									--and
									--	iesano = ( select max(iesano) from painel.ies )
									order by
										descricao";
						} else {
							$sql = array();
						}
						$cabecalho = array("Código","IES");
						echo "<table cellspacing=\"0\" cellpadding=\"3\" style=\"width: 100%;\" ><tr><td style=\"width: 100%; text-align: center; font-weight: bold; background-color: rgb(201, 201, 201); font-size: 12px; color: rgb(0, 85, 0);\">Instituição de Ensino Superior - IES</td></tr></table>";
						$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%','N');
					break;
					
					case REGIONALIZACAO_MUN :
						if(is_array($arrConteudo)) {
							$sql = "select 
										'<span style=\"cursor:pointer\" onclick=\"amun(\'' || muncod || '\')\" >' || muncod || '</span>' as codigo,
										'<span style=\"cursor:pointer\" onclick=\"amun(\'' || muncod || '\')\" >' || mundescricao || '</span>' as descricao,
										'<span style=\"cursor:pointer\" onclick=\"aest(\'' || est.estuf || '\')\" >' || est.estuf || '</span>' as estado
									from 
										territorios.municipio mun
									left join
										territorios.estado est ON est.estuf = mun.estuf
									where
										muncod in ( '".implode("','",$arrConteudo)."' )
									order by
										descricao,estado";
						} else {
							$sql = array();
						}
						$cabecalho = array("Código","Município","UF");
						echo "<table cellspacing=\"0\" cellpadding=\"3\" style=\"width: 100%;\" ><tr><td style=\"width: 100%; text-align: center; font-weight: bold; background-color: rgb(201, 201, 201); font-size: 12px; color: rgb(0, 85, 0);\">Municipal</td></tr></table>";
						$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%','N');
					break;
					
					case REGIONALIZACAO_UF :
						if(is_array($arrConteudo)) {
						$sql = "select 
									'<span style=\"cursor:pointer\" onclick=\"aest(\'' || estuf || '\')\" >' || estuf || '</span>' as codigo,
									'<span style=\"cursor:pointer\" onclick=\"aest(\'' || estuf || '\')\" >' || estdescricao || '</span>' as descricao
								from 
									territorios.estado
								where
									estuf in ( '".implode("','",$arrConteudo)."' )
								order by
									descricao";
						} else {
							$sql = array();
						}
						$cabecalho = array("UF","Estado");
						echo "<table cellspacing=\"0\" cellpadding=\"3\" style=\"width: 100%;\" ><tr><td style=\"width: 100%; text-align: center; font-weight: bold; background-color: rgb(201, 201, 201); font-size: 12px; color: rgb(0, 85, 0);\">Estado</td></tr></table>";
						$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%','N');
					break;
					
					case REGIONALIZACAO_BRASIL :
						$sql = "select 
									'1' as codigo,
									'Brasil' as descricao
								from 
									territorios.estado
								where 1 = 1
								order by
									descricao
								limit 1";
						$cabecalho = array("Código","Pais");
						echo "<table cellspacing=\"0\" cellpadding=\"3\" style=\"width: 100%;\" ><tr><td style=\"width: 100%; text-align: center; font-weight: bold; background-color: rgb(201, 201, 201); font-size: 12px; color: rgb(0, 85, 0);\">Brasil</td></tr></table>";
						$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%','N');
					break;
					
					case REGIONALIZACAO_POSGRADUACAO :
						if(is_array($arrConteudo)) {
							$sql = "select 
										'<span style=\"cursor:pointer\" onclick=\"aiep(\'' || iepid || '\')\" >' || iepid || '</span>' as codigo,
										'<span style=\"cursor:pointer\" onclick=\"aiep(\'' || iepid || '\')\" >' || iepdsc || '</span>' as descricao
									from 
										painel.iepg iepg
									where
										iepid in ( '".implode("','",$arrConteudo)."' )
									order by
										descricao";
						} else {
							$sql = array();
						}
						$cabecalho = array("Código","IEPG");
						echo "<table cellspacing=\"0\" cellpadding=\"3\" style=\"width: 100%;\" ><tr><td style=\"width: 100%; text-align: center; font-weight: bold; background-color: rgb(201, 201, 201); font-size: 12px; color: rgb(0, 85, 0);\">Instituições com Programas de Pós-Graduação</td></tr></table>";
						$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%','N');
					break;
					
					default:
					break;
					
					case REGIONALIZACAO_CAMPUS_SUPERIOR :
						if(is_array($arrConteudo)) {
							$sql = "SELECT 
										'<span style=\"cursor:pointer\" onclick=\"acampussup(\'' || ent.entid  || '\')\" >' || ent.entid || '</span>' as codigo, 
										'<span style=\"cursor:pointer\" onclick=\"acampussup(\'' || ent.entid  || '\')\" >' || UPPER(ent.entnome) || '</span>' as descricao
									FROM 
										entidade.entidade ent
									INNER JOIN
										entidade.funcaoentidade fen ON fen.entid = ent.entid
									INNER JOIN 
										entidade.endereco ende ON ende.entid = ent.entid
									WHERE
										fen.funid = 18
									AND
										ent.entid in ( '".implode("','",$arrConteudo)."' )
									ORDER BY 
										descricao";
						} else {
							$sql = array();
						}
						$cabecalho = array("Código","Campus");
						
						echo "<table cellspacing=\"0\" cellpadding=\"3\" style=\"width: 100%;\" ><tr><td style=\"width: 100%; text-align: center; font-weight: bold; background-color: rgb(201, 201, 201); font-size: 12px; color: rgb(0, 85, 0);\">Campus (Educação Superior)</td></tr></table>";
						$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%','N');
					break;
					
					case REGIONALIZACAO_HOSPITAL :
						if(is_array($arrConteudo)) {						
							$sql = "SELECT 
										'<span style=\"cursor:pointer\" onclick=\"ahospital(\'' || ent.entid  || '\')\" >' || ent.entid || '</span>' as codigo, 
										'<span style=\"cursor:pointer\" onclick=\"ahospital(\'' || ent.entid  || '\')\" >' || UPPER(ent.entnome) || '</span>' as descricao
									FROM 
										entidade.entidade ent
									INNER JOIN
										entidade.funcaoentidade fen ON fen.entid = ent.entid
									INNER JOIN 
										entidade.endereco ende ON ende.entid = ent.entid
									WHERE
										fen.funid = 16
									AND
										ent.entid in ( '".implode("','",$arrConteudo)."' )
									ORDER BY 
										descricao";
						} else {
							$sql = array();
						}
						$cabecalho = array("Código","Hospital");
						
						echo "<table cellspacing=\"0\" cellpadding=\"3\" style=\"width: 100%;\" ><tr><td style=\"width: 100%; text-align: center; font-weight: bold; background-color: rgb(201, 201, 201); font-size: 12px; color: rgb(0, 85, 0);\">Campus (Educação Superior)</td></tr></table>";
						$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%','N');
					break;
					
					case REGIONALIZACAO_CAMPUS_PROFISSIONAL :
						if(is_array($arrConteudo)) {						
							$sql = "SELECT 
										'<span style=\"cursor:pointer\" onclick=\"acampusprof(\'' || ent.entid  || '\')\" >' || ent.entid || '</span>' as codigo, 
										'<span style=\"cursor:pointer\" onclick=\"acampusprof(\'' || ent.entid  || '\')\" >' || UPPER(ent.entnome) || '</span>' as descricao
									FROM 
										entidade.entidade ent
									INNER JOIN
										entidade.funcaoentidade fen ON fen.entid = ent.entid
									INNER JOIN 
										entidade.endereco ende ON ende.entid = ent.entid
									WHERE
										fen.funid = 17
									AND
										ent.entid in ( '".implode("','",$arrConteudo)."' )
									ORDER BY 
										descricao";
						} else {
							$sql = array();
						}
						$cabecalho = array("Código","Campus");
						
						echo "<table cellspacing=\"0\" cellpadding=\"3\" style=\"width: 100%;\" ><tr><td style=\"width: 100%; text-align: center; font-weight: bold; background-color: rgb(201, 201, 201); font-size: 12px; color: rgb(0, 85, 0);\">Campus (Educação Profissional)</td></tr></table>";
						$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%','N');
					break;
					
					case REGIONALIZACAO_UNIVERSIDADE :
						if(is_array($arrConteudo)) {						
							$sql = "SELECT 
										'<span style=\"cursor:pointer\" onclick=\"auni(\'' || uni.unicod  || '\')\" >' || uni.unicod || '</span>' as codigo, 
										'<span style=\"cursor:pointer\" onclick=\"auni(\'' || uni.unicod  || '\')\" >' || UPPER(uni.unidsc) || '</span>' as descricao
									FROM 
										public.unidade uni
									INNER JOIN
										entidade.entidade ent ON ent.entunicod = uni.unicod
									INNER JOIN 
										entidade.endereco ende ON ende.entid = ent.entid
									WHERE
										uni.orgcod= '". CODIGO_ORGAO_SISTEMA. "'
									AND
										gunid IN (3,9)
									AND 
										unistatus='A' 
									AND
										uni.unicod in ( '".implode("','",$arrConteudo)."' )
									ORDER BY 
										descricao";
						} else {
							$sql = array();
						}
						$cabecalho = array("Código","Universidade");
						echo "<table cellspacing=\"0\" cellpadding=\"3\" style=\"width: 100%;\" ><tr><td style=\"width: 100%; text-align: center; font-weight: bold; background-color: rgb(201, 201, 201); font-size: 12px; color: rgb(0, 85, 0);\">Universidade</td></tr></table>";
						$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%','N');
					break;
					
					case REGIONALIZACAO_INSTITUTO :
						if(is_array($arrConteudo)) {
							$sql = "SELECT 
										'<span style=\"cursor:pointer\" onclick=\"ains(\'' || uni.unicod  || '\')\" >' || uni.unicod || '</span>' as codigo, 
										'<span style=\"cursor:pointer\" onclick=\"ains(\'' || uni.unicod  || '\')\" >' || UPPER(uni.unidsc) || '</span>' as descricao
									FROM 
										public.unidade uni
									INNER JOIN
										entidade.entidade ent ON ent.entunicod = uni.unicod
									INNER JOIN 
										entidade.endereco ende ON ende.entid = ent.entid
									WHERE
										gunid = 2 
									AND 
										unistatus='A' 
									AND
										uni.unicod in ( '".implode("','",$arrConteudo)."' )
									ORDER BY 
										descricao";
						} else {
							$sql = array();
						}
						$cabecalho = array("Código","Instituto");
						echo "<table cellspacing=\"0\" cellpadding=\"3\" style=\"width: 100%;\" ><tr><td style=\"width: 100%; text-align: center; font-weight: bold; background-color: rgb(201, 201, 201); font-size: 12px; color: rgb(0, 85, 0);\">Instituto Federal de Educação, Ciência e Tecnologia</td></tr></table>";
						$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%','N');
					break;
					
					default:
					break;
				}
			}
		}
	}else{
		echo "<div style=\"padding:10px;text-align:center;cursor:pointer\" onclick=\"addRegionalizadoresCaixa({$cxcid})\" >Clique aqui para adicionar regionalizadores!</div>";
	}
}

function filtraEstado($regcod,$filtro = null){
	global $db;
	if(!$regcod || $regcod == "todos"){
		$sql = "select estuf as codigo, estdescricao as descricao from territorios.estado order by descricao";
	}else{
		$sql = "select estuf as codigo, estdescricao as descricao from territorios.estado where regcod = '$regcod' order by descricao";
	}
	
	if($filtro){
		$acao = "filtraMunicipio";
	}else{
		$acao = "";
	}
	
	$db->monta_combo('estuf',$sql,'S','Selecione...',$acao,'','','100','N',"estuf","",$_POST['estuf']);	
}
function filtraGrupoMunicipios($gtmid,$filtro = null){
	global $db;
	if(!$gtmid || $gtmid == "todos"){
		$sql = "select tpmid as codigo, tpmdsc as descricao from territorios.tipomunicipio where tpmstatus = 'A' and (1 = 2) order by descricao";
	}else{
		$sql = "select tpmid as codigo, tpmdsc as descricao from territorios.tipomunicipio where gtmid = '$gtmid' and tpmstatus = 'A' order by descricao";
	}
	
	if($filtro){
		$acao = "filtraMunicipioPorTipoGrupo";
	}else{
		$acao = "";
	}
	
	$db->monta_combo('tpmid',$sql,'S','Selecione...',$acao,'','','200','N',"tpmid","",$_POST['tpmid']);	
}

function addAbaCompartilhada(){
	global $db; ?>
		<table class="tabela" width="95%" bgcolor="FFFFFF" cellSpacing="1" border=0 cellPadding="3" align="center">
	<tr>
		<td><input type="hidden" name="tcoid" id="tcoid" /></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan="2" style="text-align: center;font-weight: bold" >Adicionar Aba Compartilhada</td>
	</tr>
	<tr>
		<td colspan="2" >
		
		Pesquisar Abas: <input style="width:200px;" class="normal" type="text" id="pesquisa" onkeypress="pesquisaAbas();" onfocus="this.value=''" value="" />
		 <input type="button" style="display:none" id="buttonListaTodosTipos" onclick="listaAbas();" value="Listar Todos" />
		<br/>
		<div id="TipoAbas" style="padding-top:20px;width:100%">
		<?php listaAbas();?>
		</div>
		</td>
	</tr>
	<tr>
		<td colspan="2" class="SubTituloDireita" style="text-align: center;" ><input type="button" name="cancelar" value="Cancelar" id="button_cancelar" /></td>
	</tr>
</table>	
<?php }

function pesquisaAbas($pesquisa){
	global $db;
	$sql = "select 
				'<img title=\"Visualizar Aba\" style=\"display:none;cursor:pointer;vertical-align:middle\" onclick=\"visualizaAba('|| aba.abaid || ');\" src=\"../imagens/consultar.gif\" /> <a href=\"javascript:addAbaComp('|| aba.abaid ||')\" >' || aba.abadsc || '</a>' as abadsc,
				usu.usunome,
				aba.abadata
			from
				painel.aba aba
			inner join
				painel.conteudousuario con ON con.abaid = aba.abaid
			inner join
				seguranca.usuario usu ON usu.usucpf = con.usucpf
			where
				abacompartilhada = 't'
			and
				abastatus = 'A'
			and
				(usu.usunome ilike upper('%".utf8_decode($pesquisa)."%')
			or
				upper(aba.abadsc) ilike upper('%".utf8_decode($pesquisa)."%'))
			order by
				aba.abadsc";
	$abas = $db->carregar($sql);
			
	if(!$abas){
		echo "<center><span style=\"color:#990000;\" >Não existem registros.</span></center>";
		return false;
	}
	
	echo "<table cellSpacing=0 cellPadding=3 style=\"width:100%\">";
	echo "<tr bgcolor='#D9D9D9' ><td style=\"font-weight:bold;text-align:center\" >Descrição da Aba</td><td style=\"font-weight:bold\"  >Usuário</td><td style=\"font-weight:bold\"  >Última Atualização</td></tr>";
	foreach($abas as $arrabas){
		echo "<tr><td>{$arrabas['abadsc']}</td><td>{$arrabas['usunome']}</td><td>".date("d/m/Y H:m:s",strtotime($arrabas['abadata']))."</td></tr>";
	}
	echo "</table>";
}

function listaAbas(){
	global $db;
	
	$sql = "select 
				'<img title=\"Visualizar Aba\" style=\"display:none;cursor:pointer;vertical-align:middle\" onclick=\"visualizaAba('|| aba.abaid || ');\" src=\"../imagens/consultar.gif\" /> <a href=\"javascript:addAbaComp('|| aba.abaid ||')\" >' || aba.abadsc || '</a>' as abadsc,
				usu.usunome,
				aba.abadata
			from
				painel.aba aba
			inner join
				painel.conteudousuario con ON con.abaid = aba.abaid
			inner join
				seguranca.usuario usu ON usu.usucpf = con.usucpf
			where
				abacompartilhada = 't'
			and
				abastatus = 'A'
			order by
				aba.abadsc";
	
	$abas = $db->carregar($sql);
	if(!$abas){
		echo "<center><span style=\"color:#990000;\" >Não existem registros.</span></center>";
		return false;
	}
	
	echo "<table cellSpacing=0 cellPadding=3 style=\"width:100%\">";
	echo "<tr bgcolor='#D9D9D9' ><td style=\"font-weight:bold;text-align:center\" >Descrição da Aba</td><td style=\"font-weight:bold\"  >Usuário</td><td style=\"font-weight:bold\"  >Última Atualização</td></tr>";
	foreach($abas as $arrabas){
		echo "<tr><td>{$arrabas['abadsc']}</td><td>{$arrabas['usunome']}</td><td>".date("d/m/Y H:m:s",strtotime($arrabas['abadata']))."</td></tr>";
	}
	echo "</table>";
}

function copiarAba($abaid){
	global $db;
	define("TIPO_CONTEUDO_MEUS_INDICADORES",1);
	define("TIPO_CONTEUDO_GRAFICO",2);
	define("TIPO_CONTEUDO_MAPA",3);
	define("TIPO_CONTEUDO_INDICADOR_EIXO",6	);
	define("TIPO_CONTEUDO_INDICADOR_ACAO",7	);
	define("TIPO_CONTEUDO_INDICADOR_SECRETARIA",8 );
	define("TIPO_CONTEUDO_BUSCA_INDICADOR",9 );
	define("TIPO_CONTEUDO_ARVORE_INDICADOR",10 );
	define("TIPO_CONTEUDO_MUNICIPIOS",12 );
	
	$sql = "select 
				* 
			from 
				painel.caixaconteudo 
			where
				cxcstatus = 'A'
			and
				ctuid in (select ctuid from painel.conteudousuario where abaid = $abaid)";
	
	$conteudo = $db->carregar($sql);
	
	if(!$conteudo){
		echo "false";
		return false;
	}else{
		$sql = "select abadsc from painel.aba where abaid = $abaid";
		$abadsc = $db->pegaUm($sql);
		$abadsc = "Cópia de ".$abadsc;
		$sql = "insert into painel.aba (abadsc,abastatus) values ('$abadsc','A') returning abaid";
		$abaid = $db->pegaUm($sql);
		$sql = "insert into painel.conteudousuario (usucpf,abaid) values ('{$_SESSION['usucpf']}',$abaid) returning ctuid";
		$ctuid = $db->pegaUm($sql);
		foreach($conteudo as $arrConteudo){
			
			$cxcid = executaAddCaixa($arrConteudo['cxcdescricao'],$arrConteudo['tcoid'],$abaid);
			//Atualiza o posicionamento e status da caixa copiada
			$sql = "update 
						painel.caixaconteudo 
					set 
						cxclinha = '{$arrConteudo['cxclinha']}',
						cxccoluna = '{$arrConteudo['cxccoluna']}',
						cxcaberta = '{$arrConteudo['cxcaberta']}',
						cxcrolagem = '{$arrConteudo['cxcrolagem']}'
					where
						cxcid = $cxcid";
			$db->executar($sql);
			
			//Início - Adiciona Meus Indicadores
			if($arrConteudo['tcoid'] == TIPO_CONTEUDO_MEUS_INDICADORES){
				$sql = "select indid from painel.indicadorusuario where cxcid = {$arrConteudo['cxcid']}";
				$indicadores = $db->carregar($sql);
				if($indicadores){
					foreach($indicadores as $arrIndicadores){
						$sql = "insert into 
									painel.indicadorusuario 
								(usucpf,indid,cxcid)
								values
								('{$_SESSION['usucpf']}','{$arrIndicadores['indid']}','$cxcid')";
						$db->executar($sql);
					}
				}
			}//Fim - Adiciona Meus Indicadores
			//Início - Adiciona Gráfico
			elseif($arrConteudo['tcoid'] == TIPO_CONTEUDO_GRAFICO){
				$sql = "select * from painel.tipoconteudografico where cxcid = {$arrConteudo['cxcid']}";
				$grafico = $db->pegaLinha($sql);
				if($grafico){
					$grafico['tipotidid'] = !$grafico['tipotidid'] ? "NULL" : "'{$grafico['tipotidid']}'";
					$grafico['valortidid'] = !$grafico['valortidid'] ? "NULL" : "'{$grafico['valortidid']}'";
					$grafico['sehid'] = !$grafico['sehid'] ? "NULL" : "'{$grafico['sehid']}'";
					$grafico['dpeid'] = !$grafico['dpeid'] ? "NULL" : "'{$grafico['dpeid']}'";
								
					$sql = "insert into 
								painel.tipoconteudografico
							(usucpf,cxcid,indid,tcgstatus,tpgrafico,tipotidid,valortidid,sehid,dpeid)
								values
							('{$_SESSION['usucpf']}',$cxcid,'{$grafico['indid']}','{$grafico['tcgstatus']}','{$grafico['tpgrafico']}',{$grafico['tipotidid']},{$grafico['valortidid']},{$grafico['sehid']},{$grafico['dpeid']})";
					$db->executar($sql);
				}
			}//Fim - Adiciona Gráfico
			//Início - Busca de Indicador
			elseif($arrConteudo['tcoid'] == TIPO_CONTEUDO_BUSCA_INDICADOR){
				$sql = "select conteudo from painel.conteudocaixa where cxcid = {$arrConteudo['cxcid']}";
				$conteudo = $db->pegaUm($sql);
				$sql = "insert into 
							painel.conteudocaixa
						(cxcid,conteudo)
							values
						($cxcid,'$conteudo')";
				$db->executar($sql);
			}//Fim - Busca de Indicador
			//Início - Árvore
			elseif($arrConteudo['tcoid'] == TIPO_CONTEUDO_ARVORE_INDICADOR){
				$sql = "select conteudo from painel.conteudocaixa where cxcid = {$arrConteudo['cxcid']}";
				$conteudo = $db->pegaUm($sql);
				$sql = "insert into 
							painel.conteudocaixa
						(cxcid,conteudo)
							values
						($cxcid,'$conteudo')";
				$db->executar($sql);
			}//Fim - Árvore
			//Início - Municípios
			elseif($arrConteudo['tcoid'] == TIPO_CONTEUDO_MUNICIPIOS){
				$sql = "select conteudo from painel.conteudocaixa where cxcid = {$arrConteudo['cxcid']}";
				$conteudo = $db->pegaUm($sql);
				$sql = "insert into 
							painel.conteudocaixa
						(cxcid,conteudo)
							values
						($cxcid,'$conteudo')";
				$db->executar($sql);
			}//Fim - Municípios
			
		}
		$db->commit();
		echo $abaid;
	}
	
}

function salvaView($indid,$indvispadrao){
	global $db;
	
	$permissoes = verificaPerfilPainel();
	if($permissoes['verindicadores'] == 'vertodos' || in_array($indid,$permissoes['verindicadores'])){
		$sql = "update painel.indicador set indvispadrao = '$indvispadrao' where indid = $indid";
		$db->executar($sql);
		$db->commit($sql);
		echo "ok";
	}else{
		echo "false";
	}
	
}

function exibeFiltroGrafico($cxcid,$indid,$tipo,$tipoTidid = null,$tidid = null){
	global $db;
	
	$sql = "select
				ind.indnome,
				exo.exodsc,
				sec.secdsc,
				aca.acadsc,
				ind.indobjetivo,
				ind.indcumulativo,
				ind.regid,
				unm.unmdesc,
				ind.indformula,
				ind.indtermos,
				ind.indfontetermo,
				ind.indobservacao,
				ind.indvispadrao,
				per.perdsc,
				est.estdsc,
				col.coldsc,
				reg.regdescricao,
				ume.umedesc
			from
				painel.indicador ind
			left join
				painel.eixo exo ON exo.exoid = ind.exoid
			left join
				painel.secretaria sec ON sec.secid = ind.secid
			left join
				painel.acao aca ON aca.acaid = ind.acaid
			left join
				painel.unidademedicao unm ON unm.unmid = ind.unmid
			left join
				painel.periodicidade per ON per.perid = ind.perid
			left join
				painel.unidademeta ume on ind.umeid = ume.umeid
			left join
				painel.estilo est ON est.estid = ind.estid
			left join
				painel.coleta col ON col.colid = ind.colid
			left join
				painel.regionalizacao reg ON reg.regid = ind.regid
			where
				ind.indid = $indid
			limit 1";
	$ind = $db->pegaLinha($sql);
	
	?>
	
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr bgcolor="#f9f9f9">
			      <td colspan="2" align="left">
			  	  	<span style="font-size: 16px;font-weight: bold;"><? echo $indid." - ".$ind['indnome'] ?></span><br />
			  	  	<b><?php echo $ind['exodsc'] ?></b> >> <b><? echo $ind['secdsc'] ?></b> >> <b><? echo $ind['acadsc'] ?></b>
			  	  </td>
		 </tr>
		 <tr>
			      <td colspan="2" align="left">
			  	  	<div style="padding:5px;width:98%;border: solid 1px #000000;background-color: #FFFFFF">
				  	  	<fieldset>
				  	  		<legend><span style="cursor:pointer;" onclick="exibeFiltrosPainel(this);" ><img align="absmiddle"  title="menos" src="../imagens/menos.gif"/> Filtros</span></legend>
				  	  		<div id="filtros_painel" style="display:" >
								<input type="hidden" name="ultima_sehid" id="ultima_sehid" value="0" />
								<?php if($ind['indcumulativo'] == "S"){ ?>
									<input type="hidden" name="indcumulativo" id="indcumulativo" value="1" />
								<?php } ?>
								<div>
								
								<?php
											$sql = "select 
														tdiid,
														tdidsc
													from
														painel.detalhetipoindicador
													where
														indid = $indid
													and
														tdistatus = 'A'";
											$det = $db->carregar($sql);
											
											if($det[0]['tdiid'] && $det[0]['tdiid'] != ""){
												$option_barra .= "<option ".($tipo == "barra_1_{$det[0]['tdiid']}" ? "selected=selected" : "")." value=\"barra_1_{$det[0]['tdiid']}\" >Gráfico em Barra - {$det[0]['tdidsc']}</option>";
												$option_barra .= "<option ".($tipo == "barracomp_1_{$det[0]['tdiid']}" ? "selected=selected" : "")." value=\"barracomp_1_{$det[0]['tdiid']}\" >Gráfico em Barra Comparativa - {$det[0]['tdidsc']}</option>";
												$option_pizza .= "<option ".($tipo == "pizza_1_{$det[0]['tdiid']}" ? "selected=selected" : "")." value=\"pizza_1_{$det[0]['tdiid']}\" >Gráfico em Pizza - {$det[0]['tdidsc']}</option>";
											}
											if($det[1]['tdiid'] && $det[1]['tdiid'] != ""){
												$option_barra .= "<option ".($tipo == "barra_2_{$det[1]['tdiid']}" ? "selected=selected" : "")." value=\"barra_2_{$det[1]['tdiid']}\" >Gráfico em Barra - {$det[1]['tdidsc']}</option>";
												$option_barra .= "<option ".($tipo == "barracomp_2_{$det[1]['tdiid']}" ? "selected=selected" : "")." value=\"barracomp_2_{$det[1]['tdiid']}\" >Gráfico em Barra Comparativa - {$det[1]['tdidsc']}</option>";
												$option_pizza .= "<option ".($tipo == "pizza_2_{$det[1]['tdiid']}" ? "selected=selected" : "")." value=\"pizza_2_{$det[1]['tdiid']}\" >Gráfico em Pizza - {$det[1]['tdidsc']}</option>";
											}
											
											?>
											<div id="opc_tipo_grafico" style="margin:5px;width:250px;float:left;">
												<fieldset style="padding:5px;">
													<legend>Apresentação:</legend>
													<select id="tipo_grafico" onchange="escondePeriodo(this.value);" style="width:220px;" >
															<option <? echo ($tipo == "linha" ? "selected=selected" : "") ?> value="linha" >Gráfico em Linha</option>
															<option <? echo ($tipo == "barra" ? "selected=selected" : "") ?> value="barra" >Gráfico em Barra</option>
															<? echo $option_barra; ?>
															<? echo $option_pizza; ?>
														</select>
													
												</fieldset>
											</div>
								
											<div id="opc_periodo" style="margin:5px; float:left;">
											<fieldset style="padding:5px;">
												<legend>Período:</legend>
											
												<?php
												//Carrega o período do indicador
												$sql = "select  
															dpe.dpeid,
															dpe.dpedsc,
															seh.sehid
														from 
															painel.seriehistorica seh
														inner join
															painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
														where
															indid = $indid
														and
															sehstatus <> 'I'
														order by
															dpe.dpedatainicio asc";
												
												$periodos = $db->carregar($sql);
												if($periodos){
													
													if(count($periodos) > 12){
														$k1 = count($periodos) - 12;
													}else{
														$k1 = 0;
													}
													
													?>
													<select name="periodo_inicio" id="periodo_inicio">
													<?php foreach($periodos as $k => $periodo){ //inicio do foreach do periodo inicio?>
														<option <? echo $k == $k1 ? "selected=\"selected\"" : "" ?> value="<? echo $periodo['dpeid'] ?>" ><? echo $periodo['dpedsc'] ?></option>
													<?php } //fim do foreach do periodo inicio?>
													</select>
													<span id="ate_periodo" >até</span> 
													<select name="periodo_fim" id="periodo_fim">
													<?php foreach($periodos as $k => $periodo){ //inicio do foreach do periodo fim ?>
														<option <? echo $k == (count($periodos) - 1)? "selected=\"selected\"" : "" ?> value="<? echo $periodo['dpeid'] ?>" ><? echo $periodo['dpedsc'] ?></option>
													<?php } //fim do foreach do periodo fim ?>
													</select>
												<?php } ?>
											</fieldset>
											</div>
										
										<?php $sql = "	select 
														unmdesc,
														i.indescala,
														per.perdsc
													from 
														painel.indicador i
													left join 
														painel.unidademedicao u on i.unmid = u.unmid
													left join 
														painel.periodicidade per ON per.perid = i.perid
													where 
														indid = $indid";
								
											$escala = $db->pegaLinha($sql);;
											
											if(strstr($escala['unmdesc'], 'Número inteiro') && $escala['indescala'] == "t"){ ?>
											
											<div id="opc_escala" style="margin:5px;float:left;width:150px">
											<fieldset style="padding:5px;">
												<legend>Aplicar escala em:</legend>
												<select id="unidade_inteiro">
													<option selected="selected" value="1">Unidade</option>
													<option value="1000">Milhares</option>
													<option value="1000000">Milhões</option>
													<option value="1000000000">Bilhões</option>
												</select>
											</fieldset>
											</div>
											<?php }
											if(strstr($escala['unmdesc'], 'Moeda')){ ?>
											<div id="opc_escala_moeda" style="margin:5px;float:left;width:150px;">
											<fieldset style="padding:5px;">
												<legend>Aplicar escala em:</legend>
												<select id="unidade_moeda">
													<option selected="selected" value="1">Reais</option>
													<option value="1000">Milhares de Reais</option>
													<option value="1000000">Milhões de Reais</option>
													<option value="1000000000">Bilhões de Reais</option>
												</select>
											</fieldset>
											</div>
											<?php }
											if(strstr($escala['unmdesc'], 'Moeda') && strstr($escala['perdsc'], 'Anual')){?>
											<div id="opc_escala_moeda" style="margin:5px;float:left;width:100px;">
											<fieldset style="padding:5px;">
												<legend>Aplicar índice:</legend>
												<select id="indice_moeda">
													<option selected="selected" value="null">Selecione...</option>
													<option value="ipca">IPCA Médio</option>
												</select>
											</fieldset>
											</div>
										</div>
									
									
											<?php }
											if($ind['regid'] == 7 || $ind['regid'] == 2 || $ind['regid'] == 4 || $ind['regid'] == 5 || $ind['regid'] == 6 || $ind['regid'] == 8 || $ind['regid'] == 9 || $ind['regid'] == 10 || $ind['regid'] == 11 || $ind['regid'] == 12 || $ind['regid'] == REGIONALIZACAO_POLO){ //Estado?>
										<div style="clear:both">	
											<div id="opc_regiao" style="margin:5px;float:left;width:100px">
											<fieldset style="padding:5px;">
												<legend>Região:</legend>
												<select style="width:90px;" id="regcod" onchange="filtraEstadoDB(this.value)" >
													<option selected="selected" value="todos">Selecione...</option>
													<?php 
													$sql = "select 
																regcod, regdescricao
															from
																territorios.regiao
															order by
																regdescricao";
													$regiao = $db->carregar($sql);
													foreach($regiao as $rg){?>
														<option value="<?php echo $rg['regcod'] ?>"><?php echo $rg['regdescricao'] ?></option>
													<?php } ?>
												</select>
											</fieldset>
											</div>
									
											<div id="opc_estado" style="margin:5px;float:left;width:110px;">
											<fieldset style="padding:5px;">
												<legend>Estado:</legend>
												<span id="exibeEstado" >
													<select id="estuf" style="width:100px;" onchange="filtraMunicipio(this.value)" >
														<option selected="selected" value="todos">Selecione...</option>
														<?php 
														$sql = "select 
																	estuf, estdescricao
																from
																	territorios.estado
																order by
																	estdescricao";
														$estados = $db->carregar($sql);
														foreach($estados as $uf){?>
															<option value="<?php echo $uf['estuf'] ?>"><?php echo $uf['estdescricao'] ?></option>
														<?php } ?>
													</select>
													</span>
												</fieldset>
											</div>
											<?php } if($ind['regid'] == 7 || $ind['regid'] == 2 || $ind['regid'] == 4 || $ind['regid'] == 5 || $ind['regid'] == 8 || $ind['regid'] == 9 || $ind['regid'] == 10 || $ind['regid'] == 11 || $ind['regid'] == 12 || $ind['regid'] == REGIONALIZACAO_POLO){ //Município?>
											<div id="opc_grp_mun" style="margin:5px;float:left;width:160px;">
											<fieldset style="padding:5px;">
												<legend>Grupo de Municípios:</legend>
												<span id="exibe_grupo_municipio">
												<select id="gtmid" style="width:150px;" onchange="filtraGrupoMunicipios(this.value)" >
													<option selected="selected" value="todos">Selecione...</option>
												<?php 
													$sql = "select 
																gtmid, gtmdsc
															from
																territorios.grupotipomunicipio
															where
																gtmstatus = 'A'
															order by
																gtmdsc";
													$grupoMun = $db->carregar($sql);
													foreach($grupoMun as $gm){?>
														<option value="<?php echo $gm['gtmid'] ?>"><?php echo $gm['gtmdsc'] ?></option>
													<?php } ?>
													</select>
												</span>
											</fieldset>
											</div>
											
											<div id="opc_tpo_mun" style="margin:5px;float:left;width:210px;">
											<fieldset style="padding:5px;">
												<legend>Tipos de Municípios:</legend>
												<span id="exibe_tipo_municipio">
												<select id="tpmid" style="width:200px;">
													<option disabled="disabled" selected="selected" value="todos">Selecione...</option>
												</select>
												</span>
											</fieldset>
											</div>
											
											<div id="opc_mun" style="margin:5px;float:left;width:210px;">
											<fieldset style="padding:5px;">
												<legend>Município:</legend>
												<span id="exibe_municipio">
												<select id="muncod" style="width:200px;">
													<option disabled="disabled" selected="selected" value="todos">Selecione...</option>
												</select>
												</span>
												</fieldset>
											</div>
											<?php } ?>
									
									<?php if($det[0]['tdiid'] && $det[0]['tdiid'] != ""){ ?>
									<div style="clear:both">
										<?php 	$sql = "select tdidsc from painel.detalhetipoindicador where tdiid = {$det[0]['tdiid']}"; 
											$detalhe1 = $db->pegaUm($sql);
										?>
										<div id="opc_det1" style="margin:5px;float:left;width:210px;">
											<fieldset style="padding:5px;">
												<legend><?php echo $detalhe1 ?>:</legend>
											<?php 
											$sql = "select 
														tidid as codigo,
														tiddsc as descricao
													from 
														painel.detalhetipodadosindicador 
													where 
														tdiid = {$det[0]['tdiid']}
													and
														tidstatus = 'A'"; 
											$db->monta_combo('tidid1',$sql,'S','Selecione...','','','','200','N',"tidid1","","");
											?>
										</fieldset>
											</div>
									<?php } ?>
									<?php if($det[1]['tdiid'] && $det[1]['tdiid'] != ""){ ?>
										
										<?php 	$sql = "select tdidsc from painel.detalhetipoindicador where tdiid = {$det[1]['tdiid']}"; 
											$detalhe2 = $db->pegaUm($sql);
										?>
										<div id="opc_det2" style="margin:5px;float:left;width:210px;">
											<fieldset style="padding:5px;">
												<legend><?php echo $detalhe2 ?>:</legend>
											<?php 
											$sql = "select 
														tidid as codigo,
														tiddsc as descricao
													from 
														painel.detalhetipodadosindicador 
													where 
														tdiid = {$det[1]['tdiid']}
													and
														tidstatus = 'A'"; 
											$db->monta_combo('tidid2',$sql,'S','Selecione...','','','','200','N',"tidid2","","");
											?>
											</fieldset>
											</div>
									</div>
									
									<?php } ?>
								<div style="clear:both;margin:5px;">								
								<input style="margin-left:5px;" type="button" name="button_visualizar" id="button_visualizar" onclick="vizualizarGrafico('<?php echo $indid ?>')" value="Vizualizar" />
								<input style="margin-left:5px;" type="button" name="button_salvar_grafico" id="button_salvar_grafico" onclick="salvarGrafico('<?php echo $indid ?>','<?php echo $cxcid ?>')" value="Salvar" />
								<input style="margin-left:5px;" type="button" name="button_cancelar" id="button_cancelar" onclick="window.location.reload();" value="Cancelar" />
								</div>
							</div>
				  	  	</fieldset>
				  	  </div>
			  	  </td>
		 </tr>
		 <tr>
		 	<td colspan="2">
		 		<div id="div_graficos" style="width:98%;padding:5px;height:400px;overflow:auto;border: solid 1px #000000;background-color: #FFFFFF">
				  	  	<fieldset style="height:90%" >
				  	  		<legend>Gráficos</legend>
				  	  			<div style="height:100%;overflow: auto;">
				  	  			<div id="grafico_indicador"></div>
								<div id="legenda_grafico">
						  	  	</div>
				  	  		
						</fieldset>
					</div>
		 	</td>
		 </tr>
	</table>
	
<?php }

function salvaGrafico($cxcid,$dados){
	global $db;
	
	$parametros = $dados;
	
	$arrPar = explode(";",$parametros);
	
	foreach($arrPar as $dado){
		
		$d = explode("=",$dado);
		
		$dados[ $d[0] ] = $d[1]; 
		
	}
	
	
	$sql = "select cxcid from painel.conteudocaixa where cxcid = $cxcid";
	$existe = $db->pegaUm($sql);
	if($existe){
		$sql = "update painel.conteudocaixa set conteudo = '".serialize($dados)."' where cxcid = $cxcid";
	}else{
		$sql = "insert into painel.conteudocaixa (cxcid,conteudo) values ($cxcid,'".serialize($dados)."')";
	}
	$db->executar($sql);
	$db->commit($sql);
}

function maximizaGrafico($parametros){
	global $db;
	
	$DefaultParametros = $parametros;
	
	$parametros = str_replace("geraGrafico.php?","",$parametros);;
	
	$arrPar = explode(";",$parametros);
	
	foreach($arrPar as $dado){
		
		$d = explode("=",$dado);
		
		$arrparametros[ $d[0] ] = $d[1]; 
		
	}
	extract($arrparametros);
	$sql = "select indnome from painel.indicador where indid = $indid";
	$indnome = $db->pegaUm($sql);
	if($tipo != "linha" && $tipo != "barra"){
		$d = explode("_",$tidid);
		$tdiid = $d[1];
		$sql = "select tdidsc from painel.detalhetipoindicador where tdiid = {$tdiid}";
		$tdidsc = $db->pegaUm($sql);
		$exibeNome = "<span onclick=\"exibeDashBoard($indid)\" > <font size=\"2\" >".$indnome ." por " . $tdidsc . ". </font></span> <br />";
	}else{
		$exibeNome = "<span onclick=\"exibeDashBoard($indid)\" > <font size=\"2\" >".$indnome ."</font></span> <br />";
	}
	
	?>
	<style>
		table{font-size:12px;font-family:verdana}
	</style>
	<script language="javascript" type="text/javascript" src="/includes/open_flash_chart/swfobject.js"></script>
	<div style="font-size: 12px;font-family: verdana" >
		<fieldset style="padding: 5px;" >
  	  		<legend style="font-size: 12px;font-weight: bold" >Indicador</legend>
  	  			<?php echo $exibeNome ?>
		</fieldset>
	</div>
	<div id="div_graficos" style="font-family: verdana;padding:5px;height:400px;overflow:auto;">	
  	  	<fieldset style="padding: 5px;">
  	  		<legend style="font-size: 12px;font-weight: bold" >Gráfico</legend>
  	  			<div style="width:100%;height:100%;overflow: auto;">
  	  			<div id="grafico_indicador"></div>
				<div id="legenda_grafico">
		  	  	</div>
		</fieldset>
	</div>
	<?php if($tipo != "linha" && $tipo != "barra"){ ?>
		<div style="font-size:12px;font-family:verdana;margin-top:-20px" >
			<fieldset style="padding: 5px;" >
	  	  		<legend style="font-size: 12px;font-weight: bold" >Legenda</legend>
					<?php exibeLegendaGrafico($indid,$sehid,$tdiid,$tdiid,$dpeid1,$dpeid2,$estuf,$muncod,$tidid1,$tidid2,$detalhe,$valorDetalhe); ?>
			</fieldset>
		</div>
	<?php } ?>
	<div style="font-size: 12px;font-family: verdana;padding: 5px;" ><?php exibeFonterafico($indid); ?></div>
	<script>
		swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_indicador", "100%", "350", "9.0.0", "expressInstall.swf", {"data-file":"<?php echo $DefaultParametros ?>","loading":"Carregando gráfico..."} );
	</script>
<?php	
}

function abrirEscola($muncod,$filtro){
	global $db;
	//window.location='detalhamentoIndicador.php?detalhes=escola&dshcod=51010070&indid=1';
	$filtro = base64_decode($filtro);
	
	$sql = "select regid,unmid from painel.indicador where indid = {$_SESSION['indid']}";
	$indicadordados = $db->pegaLinha($sql);
	
	//Não existe totalizador se a unidade for 'percentual'
	if($indicadordados['unmid'] == 1 && $indicadordados['regid'] != REGIONALIZACAO_MUN){
		$qtde = "'' as dshqtde ";
		$soma = "N";
	}elseif($indicadordados['unmid'] == 1 && $indicadordados['regid'] == REGIONALIZACAO_MUN){
		$qtde = "".(($indicadordados['unmid']==UNIDADEMEDICAO_MOEDA)?"trim(to_char(SUM(dshqtde), '999g999g999g999d99'))":"trim(to_char(SUM(dshqtde), '9999999999'))")." as dshqtde";
		$soma = "N";	
	}else{
		$qtde = "".(($indicadordados['unmid']==UNIDADEMEDICAO_MOEDA)?"trim(to_char(SUM(dshqtde), '999g999g999g999d99'))":"trim(to_char(SUM(dshqtde), '9999999999'))")." as dshqtde";
		$soma = "S";
	}
	
	
	$sql = "select 
				'<a href=\"javascript:aesc(\'' || dsh.dshcod || '\')\" >' || esc.escdsc || '</a>', $qtde 
			from 
				painel.escola esc
			inner join
				painel.detalheseriehistorica dsh ON dsh.dshcod = esc.esccodinep
			inner join
				painel.seriehistorica seh ON seh.sehid = dsh.sehid
			inner join 
				painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
			inner join
				territorios.municipio mun ON mun.muncod = dsh.dshcodmunicipio
			where 
				escmuncod = '$muncod'
			$filtro
			group by escdsc,dsh.dshcod";				
	$cabec = array("Escolas","");
	$db->monta_lista_simples($sql,$cabec,900,1,$soma,'100%','N');
}

function abrirIES($muncod,$filtro){
	global $db;
	$filtro = base64_decode($filtro);
	
	$sql = "select regid,unmid from painel.indicador where indid = {$_SESSION['indid']}";
	$indicadordados = $db->pegaLinha($sql);
	
	//Não existe totalizador se a unidade for 'percentual'
	if($indicadordados['unmid'] == 1 && $indicadordados['regid'] != REGIONALIZACAO_MUN){
		$qtde = "'' as dshqtde ";
		$soma = "N";
	}elseif($indicadordados['unmid'] == 1 && $indicadordados['regid'] == REGIONALIZACAO_MUN){
		$qtde = "".(($indicadordados['unmid']==UNIDADEMEDICAO_MOEDA)?"trim(to_char(SUM(dshqtde), '999g999g999g999d99'))":"trim(to_char(SUM(dshqtde), '9999999999'))")." as dshqtde";
		$soma = "N";	
	}else{
		$qtde = "".(($indicadordados['unmid']==UNIDADEMEDICAO_MOEDA)?"trim(to_char(SUM(dshqtde), '999g999g999g999d99'))":"trim(to_char(SUM(dshqtde), '9999999999'))")." as dshqtde";
		$soma = "S";
	}
	
	$sql = "select 
				upper(ies.iesdsc), $qtde 
			from 
				painel.ies ies
			inner join
				painel.detalheseriehistorica dsh ON dsh.dshcod = ies.iesid::text
			inner join
				painel.seriehistorica seh ON seh.sehid = dsh.sehid
			inner join 
				painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
			inner join
				territorios.municipio mun ON mun.muncod = dsh.dshcodmunicipio
			where 
				iesmuncod = '$muncod'
			$filtro
			group by iesdsc";			
	$cabec = array("IES","");
	$db->monta_lista_simples($sql,$cabec,900,1,$soma,'100%','N');
}

function abrirPOS($muncod,$filtro){
	global $db;
	$filtro = base64_decode($filtro);
	
	$sql = "select regid,unmid from painel.indicador where indid = {$_SESSION['indid']}";
	$indicadordados = $db->pegaLinha($sql);
	
	//Não existe totalizador se a unidade for 'percentual'
	if($indicadordados['unmid'] == 1 && $indicadordados['regid'] != REGIONALIZACAO_MUN){
		$qtde = "'' as dshqtde ";
		$soma = "N";
	}elseif($indicadordados['unmid'] == 1 && $indicadordados['regid'] == REGIONALIZACAO_MUN){
		$qtde = "".(($indicadordados['unmid']==UNIDADEMEDICAO_MOEDA)?"trim(to_char(SUM(dshqtde), '999g999g999g999d99'))":"trim(to_char(SUM(dshqtde), '9999999999'))")." as dshqtde";
		$soma = "N";	
	}else{
		$qtde = "".(($indicadordados['unmid']==UNIDADEMEDICAO_MOEDA)?"trim(to_char(SUM(dshqtde), '999g999g999g999d99'))":"trim(to_char(SUM(dshqtde), '9999999999'))")." as dshqtde";
		$soma = "S";
	}
	
	$sql = "select 
				upper(iepg.iepdsc), $qtde 
			from 
				painel.iepg iepg
			inner join
				painel.detalheseriehistorica dsh ON dsh.iepid = iepg.iepid
			inner join
				painel.seriehistorica seh ON seh.sehid = dsh.sehid
			inner join 
				painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
			inner join
				territorios.municipio mun ON mun.muncod = dsh.dshcodmunicipio
			where 
				mun.muncod = '$muncod'
			$filtro
			group by iepdsc";
	//dbg($sql);			
	$cabec = array("IEPG","");
	$db->monta_lista_simples($sql,$cabec,900,1,$soma,'100%','N');
}

function abrirUNIVCAM($muncod,$filtro){
	global $db;
	$filtro = base64_decode($filtro);
	
	$sql = "select regid,unmid from painel.indicador where indid = {$_SESSION['indid']}";
	$indicadordados = $db->pegaLinha($sql);
	
	//Não existe totalizador se a unidade for 'percentual'
	if($indicadordados['unmid'] == 1 && $indicadordados['regid'] != REGIONALIZACAO_MUN){
		$qtde = "'' as dshqtde ";
		$soma = "N";
	}elseif($indicadordados['unmid'] == 1 && $indicadordados['regid'] == REGIONALIZACAO_MUN){
		$qtde = "".(($indicadordados['unmid']==UNIDADEMEDICAO_MOEDA)?"trim(to_char(SUM(dshqtde), '999g999g999g999d99'))":"trim(to_char(SUM(dshqtde), '9999999999'))")." as dshqtde";
		$soma = "N";	
	}else{
		$qtde = "".(($indicadordados['unmid']==UNIDADEMEDICAO_MOEDA)?"trim(to_char(SUM(dshqtde), '999g999g999g999d99'))":"trim(to_char(SUM(dshqtde), '9999999999'))")." as dshqtde";
		$soma = "S";
	}
	
	$sql = "select 
				'<img id=\"img_maiscampus_' || ent2.entid || '\" onclick=\"abrirCAM(this,\'' || mun.muncod || '\', \''||ent2.entid||'\',\'".base64_encode($filtro)."\')\" style=\"cursor:pointer\" title=\"Visualizar campus\" src=\"../imagens/mais.gif\" /> <a href=\"#\" onclick=\"auni(\''||ent2.entunicod||'\');\">'||upper(ent2.entnome)||'</a>', $qtde 
			from 
				entidade.entidade ent 
			inner join 
				entidade.funcaoentidade fen ON fen.entid=ent.entid
			inner join 
				entidade.funentassoc fea ON fea.fueid=fen.fueid
			inner join 
				entidade.entidade ent2 ON ent2.entid=fea.entid 
			inner join 
				entidade.funcaoentidade fen2 ON fen2.entid=ent2.entid
			inner join
				painel.detalheseriehistorica dsh ON dsh.entid = ent.entid
			inner join
				painel.seriehistorica seh ON seh.sehid = dsh.sehid
			inner join 
				painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
			inner join
				territorios.municipio mun ON mun.muncod = dsh.dshcodmunicipio
			where 
				fen.funid='".FUN_CAMPUS."' AND 
				fen2.funid='".FUN_UNIVERSIDADE."' AND
				mun.muncod = '$muncod'
			$filtro
			group by ent2.entid, ent2.entnome, mun.muncod, ent2.entunicod";
	//dbg($sql);			
	$cabec = array("Universidade","");
	$db->monta_lista_simples($sql,$cabec,900,1,$soma,'100%','N');
}

function abrirCAM($muncod,$entid,$filtro){
	global $db;
	$filtro = base64_decode($filtro);
	
	$sql = "select regid,unmid from painel.indicador where indid = {$_SESSION['indid']}";
	$indicadordados = $db->pegaLinha($sql);
	
	//Não existe totalizador se a unidade for 'percentual'
	if($indicadordados['unmid'] == 1 && $indicadordados['regid'] != REGIONALIZACAO_MUN){
		$qtde = "'' as dshqtde ";
		$soma = "N";
	}elseif($indicadordados['unmid'] == 1 && $indicadordados['regid'] == REGIONALIZACAO_MUN){
		$qtde = "".(($indicadordados['unmid']==UNIDADEMEDICAO_MOEDA)?"trim(to_char(SUM(dshqtde), '999g999g999g999d99'))":"trim(to_char(SUM(dshqtde), '9999999999'))")." as dshqtde";
		$soma = "N";	
	}else{
		$qtde = "".(($indicadordados['unmid']==UNIDADEMEDICAO_MOEDA)?"trim(to_char(SUM(dshqtde), '999g999g999g999d99'))":"trim(to_char(SUM(dshqtde), '9999999999'))")." as dshqtde";
		$soma = "S";
	}
	
	$sql = "select 
				'<a href=\"#\" onclick=\"acampussup(\''||ent.entid||'\');\">'||upper(ent.entnome)||'</a>', $qtde 
			from 
				entidade.entidade ent 
			inner join 
				entidade.funcaoentidade fen ON fen.entid=ent.entid
			inner join 
				entidade.funentassoc fea ON fea.fueid=fen.fueid
			inner join 
				entidade.entidade ent2 ON ent2.entid=fea.entid 
			inner join 
				entidade.funcaoentidade fen2 ON fen2.entid=ent2.entid
			inner join
				painel.detalheseriehistorica dsh ON dsh.entid = ent.entid
			inner join
				painel.seriehistorica seh ON seh.sehid = dsh.sehid
			inner join 
				painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
			inner join
				territorios.municipio mun ON mun.muncod = dsh.dshcodmunicipio
			where 
				fen.funid='".FUN_CAMPUS."' AND 
				fen2.funid='".FUN_UNIVERSIDADE."' AND
				mun.muncod = '$muncod' AND ent2.entid='".$entid."'
			$filtro
			group by ent.entid, ent.entnome";
	//dbg($sql);			
	$cabec = array("Campus","");
	$db->monta_lista_simples($sql,$cabec,900,1,$soma,'100%','N');
}



function exibeAbaAjax($abaAjax){
	global $db;
	
	switch($abaAjax){
		
		case "detalhes":
			
			$sql = "select
				ind.indnome,
				exo.exodsc,
				sec.secdsc,
				aca.acadsc,
				ind.indobjetivo,
				ind.indcumulativo,
				ind.regid,
				unm.unmdesc,
				ind.indformula,
				ind.indtermos,
				ind.indfontetermo,
				ind.indobservacao,
				ind.indvispadrao,
				per.perdsc,
				est.estdsc,
				col.coldsc,
				reg.regdescricao,
				ume.umedesc
			from
				painel.indicador ind
			left join
				painel.eixo exo ON exo.exoid = ind.exoid
			left join
				painel.secretaria sec ON sec.secid = ind.secid
			left join
				painel.acao aca ON aca.acaid = ind.acaid
			left join
				painel.unidademedicao unm ON unm.unmid = ind.unmid
			left join
				painel.periodicidade per ON per.perid = ind.perid
			left join
				painel.unidademeta ume on ind.umeid = ume.umeid
			left join
				painel.estilo est ON est.estid = ind.estid
			left join
				painel.coleta col ON col.colid = ind.colid
			left join
				painel.regionalizacao reg ON reg.regid = ind.regid
			where
				ind.indid = {$_SESSION['indid']}
			limit 1";
		$ind = $db->pegaLinha($sql);
		
		$sqlRes = "	select distinct
						entnome,
						coalesce('(' || entnumdddcomercial || ') ' || entnumcomercial,'N/A') as telefone
					from 
						painel.responsavel res
					inner join
						entidade.entidade ent ON res.entid = ent.entid
					where
						res.indid = {$_SESSION['indid']}
					and
						entnome is not null";
			
		 ?>
		 <div style="width:98%;margin:5px;padding:5px;height:480px">
		 <fieldset>
			  	  		<legend>Dados do Indicador</legend>
			  	  		<div style="height:450px;overflow: auto;" >
				  	  		<table  class="tabela" bgcolor="#f5f5f5" width="100%" cellSpacing="1" cellPadding="3" align="center">
								<tr bgcolor="#cccccc">
									    <td colspan="2" align="center">
									  	  	<b>Dados Gerais do Indicador</b>
										</td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Área:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['exodsc'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Secretaria:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['secdsc'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Ação:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['acadsc'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Nome Indicador:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['indnome'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Objetivo Indicador:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['indobjetivo'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Observação:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo !$ind['indobservacao'] ? "N/A" : $ind['indobservacao'];?>
									  	  </td>
								</tr>
								<tr bgcolor="#cccccc">
									    <td colspan="2" align="center">
									  	  	<b>Dados de Exibição</b>
										</td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Unidade de Medição:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['unmdesc'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Unidade de exibição do produto:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['umedesc'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Possui dados cumulativos:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['indcumulativo']=="S" ? "Sim" : ( $ind['indcumulativo']=="N" ? "Não" : "Por Ano" ); ?>
									  	  </td>
								</tr>
								<tr bgcolor="#cccccc">
									    <td colspan="2" align="center">
									  	  	<b>Dados sobre a coleta do indicador</b>
										</td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Fórmula:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['indformula'] = !$ind['indformula'] ? "N/A" : $ind['indformula'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Termos:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['indtermos'] = !$ind['indtermos'] ? "N/A" : $ind['indtermos'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Fonte:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['indfontetermo'] = !$ind['indfontetermo'] ? "N/A" : $ind['indfontetermo'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Periodicidade:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['perdsc'] = !$ind['perdsc'] ? "N/A" : $ind['perdsc'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Estilo:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['estdsc'] = !$ind['estdsc'] ? "N/A" : $ind['estdsc'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Coleta:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['coldsc'] = !$ind['coldsc'] ? "N/A" : $ind['coldsc'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Regionalização:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['regdescricao'] = !$ind['regdescricao'] ? "N/A" : $ind['regdescricao'] ?>
									  	  </td>
								</tr>
								<tr bgcolor="#cccccc">
									    <td colspan="2" align="center">
									  	  	<b>Responsáveis</b>
										</td>
								</tr>
								<tr>
									      <td colspan=2>
									  	  	<?php
									  	  		$cabecalho = array("Responsável","Telefone"); 
									  	  		$db->monta_lista_simples($sqlRes,$cabecalho,50,5,'N','100%','N');?>
									  	  </td>
								</tr>
							</table>
			  	  		</div>
			  	  	</fieldset>
			 </div>
		 <?php
		break;
		
		case "graficos":

  	  		$sql = "select 
						tdiid,
						tdidsc
					from
						painel.detalhetipoindicador
					where
						indid = {$_SESSION['indid']}
					and
						tdistatus = 'A'";
			$det = $db->carregar($sql);
			
			if($det[0]['tdiid'] && $det[0]['tdiid'] != ""){
				$option_barra .= "<option value=\"barra_1_{$det[0]['tdiid']}\" >Gráfico em Barra - {$det[0]['tdidsc']}</option>";
				$option_barra .= "<option value=\"barracomp_1_{$det[0]['tdiid']}\" >Gráfico em Barra Comparativa - {$det[0]['tdidsc']}</option>";
				$option_pizza .= "<option value=\"pizza_1_{$det[0]['tdiid']}\" >Gráfico em Pizza - {$det[0]['tdidsc']}</option>";
			}
			if($det[1]['tdiid'] && $det[1]['tdiid'] != ""){
				$option_barra .= "<option value=\"barra_2_{$det[1]['tdiid']}\" >Gráfico em Barra - {$det[1]['tdidsc']}</option>";
				$option_barra .= "<option value=\"barracomp_2_{$det[1]['tdiid']}\" >Gráfico em Barra Comparativa - {$det[1]['tdidsc']}</option>";
				$option_pizza .= "<option value=\"pizza_2_{$det[1]['tdiid']}\" >Gráfico em Pizza - {$det[1]['tdidsc']}</option>";
			}
			
			?>
			<fieldset style="padding:5px;margin: 5px;">
				<legend>Apresentação:</legend>
				<select id="tipo_grafico" onchange="escondePeriodo(this.value);mudaTipoGrafico2(<?php echo $_SESSION['indid'] ?>);">
						<option <?php echo ($tipo == "linha" ? "selected=selected" : "") ?> value="linha" >Gráfico em Linha</option>
						<option <?php echo ($tipo == "barra" ? "selected=selected" : "") ?> value="barra" >Gráfico em Barra</option>
						<?php echo $option_barra; ?>
						<?php echo $option_pizza; ?>
					</select>
				<span style="margin-left:10px;display:none" id="grafico_linha_financeiro"><input type="radio" onclick="checkFinanceiro(this)" id="finac_qtde" name="finac_tipo" value="qntde" />Quantidade  <input onclick="checkFinanceiro(this)" style="margin-left:10px;" type="radio" id="finac_valor" name="finac_tipo" value="valor" />Valor Monetário R$ <input onclick="checkFinanceiro(this)" style="margin-left:10px;" type="radio" checked="checked" id="finac_ambos" name="finac_tipo" value="ambos" />Ambos </span>
			</fieldset>
			<div id="div_graficos" style="width:98%;margin:5px;padding:5px;overflow:auto;">
		  	  	<fieldset style="height:95%" >
		  	  		<legend>Gráficos</legend>
		  	  			<table cellpadding="2" border="0" align="center" width="100%">
		  	  				<tr>
		  	  				<?php 
		  	  				$sql = "select 
										regid
									from
										painel.indicador
									where
										indid = {$_SESSION['indid']}";
							$regid = $db->pegaUm($sql);
							
							if($regid == REGIONALIZACAO_BRASIL){
								$heightGrafico = "100%";	
								$displayLista = "none";
							}else{
								$heightGrafico = "600px";
								$displayLista = "";
							}
							
							$heightGrafico = "100%";
							
		  	  				?>
		  	  				
		  	  					<td style="width:<?php echo $heightGrafico ?>;height:400px;">
			  	  					<div style="width:100%;height:100%;overflow: auto;">
			  	  					<div style="width:100%;height:100%;" id="grafico_indicador"></div>
									<div id="legenda_grafico"></div>
								</td>
							</tr>
						</table>
				</fieldset>
				<div>
					<fieldset >
			  	  		<legend>Tabela do Indicador</legend>
			  	  		<div id="tabela_indicador_db"></div>
			  	  	</fieldset>
		  	  	</div>
				
			</div>
			<script>
			jQuery(document).ready(function() {
				<?php 
				$sql = "select indvispadrao from painel.indicador where indid = {$_SESSION['indid']}";
				$indvispadrao = $db->pegaUm($sql);
				if($indvispadrao){
					$dados = explode(";",$indvispadrao);
					$tipoGrafico = $dados[0] ? $dados[0] : '';
					?>
					jQuery("#tipo_grafico").val("<?php echo $tipoGrafico ?>");
				<?php } ?>
				mudaTipoGrafico2(<?php echo $_SESSION['indid'] ?>,'div_mapa2');
				
			});
			</script>
			<?php
		break;
		
		case "mapa":
			?>
			  	  	
		  	  	<div id="exibe_mapa" style="margin:5px;height:480px;padding:5px">
		  	  		<div style="margin:5px;overflow:auto;height:465px" >
			  	  		<fieldset>
			  	  			<legend >Mapa</legend>
			  	  				<div id="div_mapa">
			  	  				</div>
			  	  		</fieldset>
			  	  		<script>
								jQuery(document).ready(function() {
									mudaTipoGrafico2(<?php echo $_SESSION['indid'] ?>,'div_mapa')
								});
						</script>
		  	  		</div>
		  	  	</div>
			<?php
		break;
		
		case "relatorio":
			?>
			<div style="margin:5px;height:480px;padding:5px">
		  	  		<div style="margin:5px;overflow:auto;height:465px" >
			  	  		<fieldset>
			  	  			<legend >Relatório</legend>
			  	  				<div>
			  	  				<?php include APPRAIZ . 'painel/modulos/principal/relatorioDashBoard.inc'; ?>
			  	  				</div>
			  	  		</fieldset>
		  	  		</div>
		  	  	</div>
			<?php
		break;
		
		case "seriehistorica":
			?>
			<div id="div_serie_historica" style="width:98%;margin:5px;float:left;padding:5px;height:480px;background-color: #FFFFFF">
		  	  	<fieldset>
		  	  		<legend>Série Histórica</legend>
		  	  		<div style="overflow:auto;height:450px" id="exibe_serie_historica_db">
					</div>
				</fieldset>
			</div>
			<div style="display: none" id="legenda_grafico">
			<script>
			jQuery(document).ready(function() {
				mudaTipoGrafico2(<?php echo $_SESSION['indid'] ?>)
			});
			</script>
			<?php
		break;
		
		default:
			echo "Dados indisponíveis.";
		break;
		
	}
}

function exibePaginaInicial(){
	?>
	<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" class="tabela">
		<tr>
			<td align="center" style="background-color: #DCDCDC"><?=campo_texto('busca','N','S','',60,60,'','','','','',"id='busca' onkeyup='exibeBuscaRegionalizacaoEnter(event)' ",'',$_REQUEST['busca']); ?>
			 	<input onclick="exibeBuscaRegionalizacao();" type="button" name="Buscar" value="Buscar" />
			 	<input onclick="document.getElementById('busca').value='';exibeBuscaRegionalizacao();" type="button" name="Limpar" value="Limpar" />
			 </td>
		</tr>
		<tr>
			<td valign="top" id="td_exibe_resultado_busca" ><?php exibeResultadoBusca(); ?></td>
		</tr>
	</table>
	<?php
}

function exibeResultadoBusca($busca = null){
	global $db;
	
	$sql = "select
				cxpid,
				cxpsql1 as sql,
				cxpcabecalho,
				(CASE WHEN cxp.regid is not null
					THEN
					(select regunidade from painel.regionalizacao r where cxp.regid = r.regid)
				 ELSE
				 	cxpunidade
				 END) as cxpunidade,
				cxpicone
			from
				painel.caixapesquisa cxp
			where
				cxpstatus = 'A'
			order by
				cxpordem";

	$arrDados = $db->carregar($sql);
	
	$i = 0;
	foreach($arrDados as $arrD):
		$td[$i][] = $arrD;
		if($i==2)
			$i=0;
		else
			$i++;
	endforeach;
	
	?>
	<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" border="0" width="100%">
		<tr>
			<td valign="top" width="<?php echo count($arrDados) > 2 ? "33" : (count($arrDados) == 2 ? "50" : "100") ?>%" >
			<?php foreach($td[0] as $arrTd): ?>
				<?=exibeBuscaRegionalizador($arrTd,$busca); ?>
			<?php endforeach; ?>
			</td>
			<?php if( $td[1] ): ?>
				<td valign="top" width="<?php echo count($arrDados) > 2 ? "33" : (count($arrDados) == 2 ? "50" : "100") ?>%" >
				<?php foreach($td[1] as $arrTd): ?>
					<?=exibeBuscaRegionalizador($arrTd,$busca); ?>
			<?php endforeach; ?>
				</td>
			<?php endif; ?>
			<?php if( $td[2] ): ?>
				<td valign="top" width="<?php echo count($arrDados) > 2 ? "33" : (count($arrDados) == 2 ? "50" : "100") ?>%" >
				<?php foreach($td[2] as $arrTd): ?>
					<?=exibeBuscaRegionalizador($arrTd,$busca); ?>
			<?php endforeach; ?>
				</td>
			<?php endif; ?>
			</tr>
	</table>

<?php }

function exibeBuscaRegionalizador($arrPesquisa,$busca = null){
	global $db;
	
	$busca = utf8_decode($busca);
	
	if($busca):
		$sql = str_replace("{busca}",$busca,$arrPesquisa['sql']);
		$cabecalho = explode(",",$arrPesquisa['cxpcabecalho']);
		$dados = $db->carregar($sql);
		if($dados):
			$img = "seta_cima";
			$display= "";
		else:
			$img = "seta_baixo";
			$display = "none";
		endif;
		?>
		<div onclick="exibeResultadoBuscaRegionalizador('<?=$arrPesquisa['cxpid'] ?>')" id="function_regid_<?=$arrPesquisa['cxpid'];?>" style="cursor:pointer;width:100%;border:solid 1px #888888 ;background-color:#EEE9E9; color: black;font-weight: bold;margin-top:10px" ><div style="padding: 5px;"><img id="img_seta_<?=$arrPesquisa['cxpid']?>" src="../imagens/<?=$img?>.png" /> <?=$arrPesquisa['cxpunidade']?></div></div>
		<div id="div_exibe_regid_<?=$arrPesquisa['cxpid'];?>" style="display:<?=$display?> ;width:100%;border-right:solid 1px #888888 ;border-left:solid 1px #888888 ;border-bottom:solid 1px #888888 ;background-color: #FFFFFF; color: black;" ><div style="padding: 5px;">
		<?php 
		$dados = !$dados ? array() : $dados;
		echo "<div style=\"width:100%;text-align:center;cursor:pointer\"><img onclick=\"exibeMapaRegionalizador('".$arrPesquisa['cxpid']."')\"  style=\"vertical-align:middle\" src=\"../painel/images/".$arrPesquisa['cxpicone']."\" /></div>";
		$db->monta_lista_simples($dados,$cabecalho,50,5,'N','100%','N');
		if(count($dados) >= 50):
			echo "<center><span style=\"color:#990000\" >Foram encontrados mais de 50 registros, favor refinar sua busca.</span></center>";
		endif;
	else:
		?>
		<div onclick="exibeResultadoBuscaRegionalizador('<?=$arrPesquisa['cxpid'] ?>')" id="function_regid_<?=$arrPesquisa['cxpid'];?>" style="cursor:pointer;width:100%;border:solid 1px #888888 ;background-color:#EEE9E9; color: black;font-weight: bold;margin-top:10px" ><div style="padding: 5px;"><img id="img_seta_<?=$arrPesquisa['cxpid']?>" src="../imagens/seta_cima.png" /> <?=$arrPesquisa['cxpunidade']?></div></div>
		<div id="div_exibe_regid_<?=$arrPesquisa['cxpid'];?>" style="width:100%;border-right:solid 1px #888888 ;border-left:solid 1px #888888 ;border-bottom:solid 1px #888888 ;background-color: #FFFFFF; color: black;" ><div style="padding: 5px;">
		<?php 
		echo "<div style=\"width:100%;text-align:center;cursor:pointer\"><img onclick=\"exibeMapaRegionalizador('".$arrPesquisa['cxpid']."')\" style=\"vertical-align:middle\" src=\"../painel/images/".$arrPesquisa['cxpicone']."\" /></div>";
	endif;
		echo "</div></div>";
	
}

function exibeMapaRegionalizador($cxpid){
	global $db;
	
	$sql = "select
				cxpid,
				cxpsql2 as sql,
				cxpcabecalho,
				(CASE WHEN cxp.regid is not null
					THEN
					(select regunidade from painel.regionalizacao r where cxp.regid = r.regid)
				 ELSE
				 	cxpunidade
				 END) as cxpunidade,
				cxpicone
			from
				painel.caixapesquisa cxp
			where
				cxpstatus = 'A'
			and
				cxpid = $cxpid";

	$arrDados = $db->pegaLinha($sql);
	
	?>
	<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" class="tabela">
		<tr>
			<td colspan="2" align="center" style="background-color: #DCDCDC">
				<a href="javascript:window.location.reload();" >Busca de Reginalizadores</a> >> <?=$arrDados['cxpunidade']; ?>
			 </td>
		</tr>
		<tr>
			<td valign="top" width="450">
				<fieldset style="background: #ffffff; width: 450px;"  >
					<legend> Mapa </legend>
					
					<div style="width: 100%;" id="containerMapa" >
						<img src="/imagens/mapa_brasil.png" width="444" height="357" border="0" usemap="#mapaBrasil" />
						<map name="mapaBrasil" id="mapaBrasil">
							<area shape="rect" coords="388,15,427,48"   style="cursor:pointer;" onclick="document.getElementById('buscakey').value='';document.getElementById('hidden_letra_inicial').value='';exibeRegionalizador('todas', '<?=$arrDados['cxpid']?>');" title="Brasil"/>							
							<area shape="rect" coords="48,124,74,151"   style="cursor:pointer;" onclick="exibeRegionalizador('AC','<?=$arrDados['cxpid']?>');" title="Acre"/>
							<area shape="rect" coords="364,147,432,161" style="cursor:pointer;" onclick="exibeRegionalizador('AL','<?=$arrDados['cxpid']?>');" title="Alagoas"/>
							<area shape="rect" coords="202,27,233,56"   style="cursor:pointer;" onclick="exibeRegionalizador('AP','<?=$arrDados['cxpid']?>');" title="Amapá"/>
							<area shape="rect" coords="89,76,133,107"   style="cursor:pointer;" onclick="exibeRegionalizador('AM','<?=$arrDados['cxpid']?>');" title="Amazonas"/>
							<area shape="rect" coords="294,155,320,183" style="cursor:pointer;" onclick="exibeRegionalizador('BA','<?=$arrDados['cxpid']?>');" title="Bahia"/>
							<area shape="rect" coords="311,86,341,114"  style="cursor:pointer;" onclick="exibeRegionalizador('CE','<?=$arrDados['cxpid']?>');" title="Ceará"/>
							<area shape="rect" coords="244,171,281,197" style="cursor:pointer;" onclick="exibeRegionalizador('DF','<?=$arrDados['cxpid']?>');" title="Distrito Federal"/>
							<area shape="rect" coords="331,215,369,242" style="cursor:pointer;" onclick="exibeRegionalizador('ES','<?=$arrDados['cxpid']?>');" title="Espírito Santo"/>
							<area shape="rect" coords="217,187,243,218" style="cursor:pointer;" onclick="exibeRegionalizador('GO','<?=$arrDados['cxpid']?>');" title="Goiás"/>
							<area shape="rect" coords="154,155,210,186" style="cursor:pointer;" onclick="exibeRegionalizador('MT','<?=$arrDados['cxpid']?>');" title="Mato Grosso"/>
							<area shape="rect" coords="156,219,202,246" style="cursor:pointer;" onclick="exibeRegionalizador('MS','<?=$arrDados['cxpid']?>');" title="Mato Grosso do Sul"/>
							<area shape="rect" coords="248,80,301,111"  style="cursor:pointer;" onclick="exibeRegionalizador('MA','<?=$arrDados['cxpid']?>');" title="Maranhão"/>
							<area shape="rect" coords="264,206,295,235" style="cursor:pointer;" onclick="exibeRegionalizador('MG','<?=$arrDados['cxpid']?>');" title="Minas Gerais"/>
							<area shape="rect" coords="188,84,217,112"  style="cursor:pointer;" onclick="exibeRegionalizador('PA','<?=$arrDados['cxpid']?>');" title="Pará"/>
							<area shape="rect" coords="368,112,433,130" style="cursor:pointer;" onclick="exibeRegionalizador('PB','<?=$arrDados['cxpid']?>');" title="Paraíba"/>
							<area shape="rect" coords="201,262,231,289" style="cursor:pointer;" onclick="exibeRegionalizador('PR','<?=$arrDados['cxpid']?>');" title="Paraná"/>
							<area shape="rect" coords="369,131,454,147" style="cursor:pointer;" onclick="exibeRegionalizador('PE','<?=$arrDados['cxpid']?>');" title="Pernambuco"/>
							<area shape="rect" coords="285,116,313,146" style="cursor:pointer;" onclick="exibeRegionalizador('PI','<?=$arrDados['cxpid']?>');" title="Piauí"/>
							<area shape="rect" coords="349,83,383,108"  style="cursor:pointer;" onclick="exibeRegionalizador('RN','<?=$arrDados['cxpid']?>');" title="Rio Grande do Norte"/>
							<area shape="rect" coords="189,310,224,337" style="cursor:pointer;" onclick="exibeRegionalizador('RS','<?=$arrDados['cxpid']?>');" title="Rio Grande do Sul"/>
							<area shape="rect" coords="302,250,334,281" style="cursor:pointer;" onclick="exibeRegionalizador('RJ','<?=$arrDados['cxpid']?>');" title="Rio de Janeiro"/>
							<area shape="rect" coords="98,139,141,169"  style="cursor:pointer;" onclick="exibeRegionalizador('RO','<?=$arrDados['cxpid']?>');" title="Rondônia"/>
							<area shape="rect" coords="112,24,147,56"   style="cursor:pointer;" onclick="exibeRegionalizador('RR','<?=$arrDados['cxpid']?>');" title="Roraima"/>
							<area shape="rect" coords="228,293,272,313" style="cursor:pointer;" onclick="exibeRegionalizador('SC','<?=$arrDados['cxpid']?>');" title="Santa Catarina"/>
							<area shape="rect" coords="233,243,268,270" style="cursor:pointer;" onclick="exibeRegionalizador('SP','<?=$arrDados['cxpid']?>');" title="São Paulo"/>
							<area shape="rect" coords="337,161,401,178" style="cursor:pointer;" onclick="exibeRegionalizador('SE','<?=$arrDados['cxpid']?>');" title="Sergipe"/>
							<area shape="rect" coords="227,130,270,163" style="cursor:pointer;" onclick="exibeRegionalizador('TO','<?=$arrDados['cxpid']?>');" title="Tocantins"/>
							<area shape="rect" coords="17,264,85,282"   style="cursor:pointer;" onclick="exibeRegionalizador('Norte','<?=$arrDados['cxpid']?>');" title="Norte" />
							<area shape="rect" coords="16,281,94,296"   style="cursor:pointer;" onclick="exibeRegionalizador('Nordeste','<?=$arrDados['cxpid']?>');" title="Nordeste" />
							<area shape="rect" coords="15,296,112,312"  style="cursor:pointer;" onclick="exibeRegionalizador('Centro-Oeste','<?=$arrDados['cxpid']?>');" title="Centro-Oeste" />
							<area shape="rect" coords="14,312,100,329"  style="cursor:pointer;" onclick="exibeRegionalizador('Sudeste','<?=$arrDados['cxpid']?>');" title="Sudeste" />
							<area shape="rect" coords="13,329,68,344"   style="cursor:pointer;" onclick="exibeRegionalizador('Sul','<?=$arrDados['cxpid']?>')" title="Sul" />
						</map>
					</div>
				</fieldset>
			</td>
			<td valign="top" align="left">
				<fieldset style="background: #ffffff;width: 95%"  >
					<legend> Regionalizadores </legend>
						<div style="width: 100%;">
						<div>
						<?php $src_img = is_file("../painel/images/".$arrDados['cxpicone']) ? "../painel/images/".$arrDados['cxpicone'] : "../painel/images/13.gif" ?>
							<div style="float:left" ><img style="vertical-align: middle" src="<?=$src_img?>" /></div>
							<div style="float:left;padding-top: 13px;padding-left: 5px" >Pesquisar:<br/><input type="text" name="buscakey" id="buscakey" size="40" onkeyup="exibeRegionalizadorKeyUp(this.value,'<?=$arrDados['cxpid']?>');" > </div>
						</div>
						<div style="clear:both">
							<input type="hidden" name="hidden_regiao" id="hidden_regiao" value="todas" >
							<input type="hidden" name="hidden_letra_inicial" id="hidden_letra_inicial" value="" >
							<div id="exibeBuscaRegionalizador">
								<?buscaRegionalizador($arrDados['cxpid'],"todas"); ?>
							</div>
						</div>
						</div>
				</fieldset>
			</td>
		</tr>
	</table>
	<?php
}

function retornaNomeRegionalizador($regid){
	switch($regid){
		case REGIONALIZACAO_ESCOLA:
			return "Escola";
		break;
		case REGIONALIZACAO_POLO:
			return "Pólo";
		break;
		case REGIONALIZACAO_IES:
			return "Instituição de Ensino Superior - IES";
		break;
		case REGIONALIZACAO_MUN:
			return "Município";
		break;
		case REGIONALIZACAO_UF:
			return "Estado";
		break;
		case REGIONALIZACAO_BRASIL:
			return "Brasil";
		break;
		case REGIONALIZACAO_POSGRADUACAO:
			return "Instituição com Programa de Pós-Graduação";
		break;
		case REGIONALIZACAO_CAMPUS_SUPERIOR:
			return "Campus (Educação Superior)";
		break;
		case REGIONALIZACAO_CAMPUS_PROFISSIONAL:
			return "Campus (Educação Profissional)";
		break;
		case REGIONALIZACAO_UNIVERSIDADE:
			return "Instituição Federal de Ensino Superior";
		break;
		case REGIONALIZACAO_INSTITUTO:
			return "Instituto Federal de Educação, Ciência e Tecnologia";
		break;
		case REGIONALIZACAO_HOSPITAL:
			return "Hospitais Universitários";
		break;
		case REGIONALIZACAO_ACAO:
			return "Ações";
		case REGIONALIZACAO_SECRETARIA:
			return "Secretarias";
		break;
		case REGIONALIZACAO_TERRITORIO:
			return "Grupo de Municípios / Mesorregião / Região / Microrregião";
		break;
		default:
			return false;
	}
}

function buscaRegionalizador($cxpid,$regiao,$busca = null,$letraInicial = null){
	global $db;
	
	$sql = "select
				cxpid,
				cxpsql2,
				cxpcabecalho,
				(CASE WHEN cxp.regid is not null
					THEN
					(select regunidade from painel.regionalizacao r where cxp.regid = r.regid)
				 ELSE
				 	cxpunidade
				 END) as cxpunidade,
				cxpicone
			from
				painel.caixapesquisa cxp
			where
				cxpstatus = 'A'
			and
				cxpid = $cxpid";

	$arrDados = $db->pegaLinha($sql);
	
	?>
	<div style="cursor:pointer;width:100%;border:solid 1px #888888 ;background-color:#EEE9E9; color: black;font-weight: bold;margin-top:10px" ><div style="padding: 5px;"><?php echo $regiao != "todas" ? "$regiao - " : "" ?><?=$arrDados['cxpunidade']?></div></div>
	<div style="width:100%;height:302px;overflow:auto;border-right:solid 1px #888888 ;border-left:solid 1px #888888 ;border-bottom:solid 1px #888888 ;background-color: #FFFFFF; color: black;" ><div style="padding: 5px;">
	<?php $arAlfabeto = array( "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
                                    "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" ); 
			foreach($arAlfabeto as $letra){
				if($letraInicial != $letra){?>
			<div onclick="exibeRegionalizadorLetraInicial('<?=$regiao?>','<?=$arrDados['cxpid'] ?>','<?=$letra?>');" style="margin:1px;border:solid 1px black;padding:3px;float:left;width:8px;text-align:center;cursor:pointer;"><?=$letra?></div>
		<?php }else{ ?>
			<div style="margin:1px;border:solid 1px black;padding:3px;float:left;width:8px;text-align:center;font-weight:bold;text-decoration:underline;"><?=$letra ?></div>
		<?php } ?>
	<?php } ?> <div onclick="exibeRegionalizadorLetraInicial('<?=$regiao?>','<?=$arrDados['cxpid'] ?>','');" style="margin:1px;border:solid 1px black;padding:3px;float:left;width:34px;text-align:center;cursor:pointer;<?php echo $letraInicial ? "" : "font-weight:bold;text-decoration:underline"?>">Todos</div>
	<?php
		if($regiao && $regiao != "todas"){
			if($regiao == "Sul"){
				$whereRegiao = "select estuf from territorios.estado where regcod = '4'";	
			}elseif($regiao == "Norte"){
				$whereRegiao = "select estuf from territorios.estado where regcod = '1'";	
			}elseif($regiao == "Nordeste"){
				$whereRegiao = "select estuf from territorios.estado where regcod = '2'";	
			}elseif($regiao == "Centro-Oeste"){
				$whereRegiao = "select estuf from territorios.estado where regcod = '5'";	
			}elseif($regiao == "Sudeste"){
				$whereRegiao = "select estuf from territorios.estado where regcod = '3'";	
			}else{
				$whereRegiao = "'$regiao'";
			}
		}elseif(!$regiao || $regiao == null || $regiao == "todas"){
			$whereRegiao = "select estuf from territorios.estado" ;
		}
		
		$sql = str_replace( array("{busca}","{letra}","{selectregiao}") , array($busca,$letraInicial,$whereRegiao) ,$arrDados['cxpsql2'] );
		$cabecalho = explode(",",$arrDados['cxpcabecalho']);
		$dados = $db->carregar($sql);
		$dados = !$dados ? array() : $dados;
		$db->monta_lista_simples($dados,$cabecalho,50,5,'N','100%','N');
		if(count($dados) >= 50){
			echo "<center><span style=\"color:#990000\" >Foram encontrados mais de 50 registros, favor refinar sua busca.</span></center>";
		}?>
	</div></div>
	<?php
}

function exibeDadosIndicador($indid){
	global $db;
	
	$sql = "select
				ind.indnome,
				exo.exodsc,
				sec.secdsc,
				aca.acadsc,
				aca.acaid,
				ind.indobjetivo,
				ind.indcumulativo,
				ind.regid,
				unm.unmdesc,
				ind.indformula,
				ind.indtermos,
				ind.indfontetermo,
				ind.indobservacao,
				ind.indvispadrao,
				per.perdsc,
				est.estdsc,
				col.coldsc,
				reg.regdescricao,
				ume.umedesc
			from
				painel.indicador ind
			left join
				painel.eixo exo ON exo.exoid = ind.exoid
			left join
				painel.secretaria sec ON sec.secid = ind.secid
			left join
				painel.acao aca ON aca.acaid = ind.acaid
			left join
				painel.unidademedicao unm ON unm.unmid = ind.unmid
			left join
				painel.periodicidade per ON per.perid = ind.perid
			left join
				painel.unidademeta ume on ind.umeid = ume.umeid
			left join
				painel.estilo est ON est.estid = ind.estid
			left join
				painel.coleta col ON col.colid = ind.colid
			left join
				painel.regionalizacao reg ON reg.regid = ind.regid
			where
				ind.indid = $indid
			limit 1";
		$ind = $db->pegaLinha($sql);
		
		$sqlRes = "	select distinct
						entnome,
						coalesce('(' || entnumdddcomercial || ') ' || entnumcomercial,'N/A') as telefone
					from 
						painel.responsavel res
					inner join
						entidade.entidade ent ON res.entid = ent.entid
					where
						res.indid = $indid
					and
						entnome is not null";
			
		 ?>
		 <div style="width:98%;margin:5px;padding:5px;">
		 <fieldset>
			  	  		<legend>Dados do Indicador</legend>
			  	  		<div style="" >
			  	  			<div style="float:right;margin-bottom:10px">
			  	  				<input type="button" value="Fechar" onclick="document.getElementById('tr_<?=$ind['acaid']?>_<?=$indid?>_grafico').style.display = 'none';" />
				  	  		</div>
				  	  		<table  style="clear:both" class="tabela" bgcolor="#f5f5f5" width="100%" cellSpacing="1" cellPadding="3" align="center">
								<tr bgcolor="#cccccc">
									    <td colspan="2" align="center">
									  	  	<b>Dados Gerais do Indicador</b>
										</td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Área:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['exodsc'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Secretaria:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['secdsc'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Ação:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['acadsc'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Nome Indicador:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['indnome'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Objetivo Indicador:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['indobjetivo'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Observação:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo !$ind['indobservacao'] ? "N/A" : $ind['indobservacao'];?>
									  	  </td>
								</tr>
								<tr bgcolor="#cccccc">
									    <td colspan="2" align="center">
									  	  	<b>Dados de Exibição</b>
										</td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Unidade de Medição:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['unmdesc'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Unidade de exibição do produto:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['umedesc'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Possui dados cumulativos:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['indcumulativo']=="S" ? "Sim" : ( $ind['indcumulativo']=="N" ? "Não" : "Por Ano" ); ?>
									  	  </td>
								</tr>
								<tr bgcolor="#cccccc">
									    <td colspan="2" align="center">
									  	  	<b>Dados sobre a coleta do indicador</b>
										</td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Fórmula:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['indformula'] = !$ind['indformula'] ? "N/A" : $ind['indformula'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Termos:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['indtermos'] = !$ind['indtermos'] ? "N/A" : $ind['indtermos'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Fonte:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['indfontetermo'] = !$ind['indfontetermo'] ? "N/A" : $ind['indfontetermo'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Periodicidade:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['perdsc'] = !$ind['perdsc'] ? "N/A" : $ind['perdsc'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Estilo:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['estdsc'] = !$ind['estdsc'] ? "N/A" : $ind['estdsc'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Coleta:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['coldsc'] = !$ind['coldsc'] ? "N/A" : $ind['coldsc'] ?>
									  	  </td>
								</tr>
								<tr>
									      <td width="20%" align="right">
									  	  	<b>Regionalização:</b>
									  	  </td>
									  	  <td align="left">
									  	  	<?php echo $ind['regdescricao'] = !$ind['regdescricao'] ? "N/A" : $ind['regdescricao'] ?>
									  	  </td>
								</tr>
								<tr bgcolor="#cccccc">
									    <td colspan="2" align="center">
									  	  	<b>Responsáveis</b>
										</td>
								</tr>
								<tr>
									      <td colspan=2>
									  	  	<?php
									  	  		$cabecalho = array("Responsável","Telefone"); 
									  	  		$db->monta_lista_simples($sqlRes,$cabecalho,50,5,'N','100%','N');?>
									  	  </td>
								</tr>
							</table>
			  	  		</div>
			  	  	</fieldset>
			 </div>
			 <?php 
}

