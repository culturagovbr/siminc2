<?php
ini_set("memory_limit", "1024M");
set_time_limit(0);

//Pega o sisid e cria a variável $sisid
$sisid = !$_SESSION['sisid'] ? $_REQUEST['sisid'] : $_SESSION['sisid'];

include APPRAIZ . 'includes/classes/ManterTabela.class.inc';

$tbl = new ManterTabela();

if($schema){
	$_POST['combo_sch'] = $schema;
}
if($table){
	$_POST['combo_tbl'] = $schema.".".$table;
	$_POST['tbls'] = $_POST['combo_tbl'];
	$tbl->saveTableApoio(null,false);
} 

if($_POST['combo_sch']){
	$tbl->setSchemaName($_POST['combo_sch']);
	if($_POST['combo_tbl']){
		$dado = explode(".",$_POST['combo_tbl']);
		$tbl->setTableName($dado[1]);
		
		$pk = $tbl->getPK();
		
		if($_POST['inp_hdn_acao'] == "salvar"){
			$tbl->salvarDados(null,null,null,true,$editPK);
			unset($_POST['inp_hdn_acao']);
		}
		
		if($$pk){
			$_POST['inp_hdn_campo'] = $pk;
			$_POST['inp_hdn_campo_valor'] = $$pk;
			$arrDados = $tbl->popularDados();
		}
		
		if($_POST['inp_hdn_campo'] && $_POST['inp_hdn_campo_valor'] && $_POST['inp_hdn_acao'] == "carregar"){
			$arrDados = $tbl->popularDados();
		}
				
		if($_POST['inp_hdn_campo'] && $_POST['inp_hdn_campo_valor'] && $_POST['inp_hdn_acao'] == "excluir"){
			$tbl->excluirDados();
		}
		
		if($_POST['ajaxMethod']){
			header('content-type: text/html; charset=ISO-8859-1');
			$tbl->$_POST['ajaxMethod']();
			exit;
		}
			
	}
}

if(!$db->testa_superuser()){
	if(!$arrPermission){
		$tbl->setPermission(array("alterar"));	
	}else{
		$tbl->setPermission($arrPermission);
	}
}

// monta cabeçalho 
if($esconderCabecalho){
	
}else{
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';
	
	$titulo = !$titulo ? str_replace("...","",$_SESSION['mnudsc']) : $titulo;
	monta_titulo( $titulo, '&nbsp' );	
}

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript">

var arrLabel = new Array();

function esconde(id){
	$('#' + id ).fadeOut('slow');
}


$(function() {

	 $("#aguarde").hide();
	 
	 if($('#tr_msg_sucess')){
	 	window.setTimeout( "esconde('tr_msg_sucess')" , '5000' );
	 }
	 
	 $('#btn_todos').click(function() {
	 	$("[name=txt_busca]").val("");
	 	$("#formulario_tabela_apoio").submit();
	 });
	 
	 $('[class=link fechar]').click(function() {
	 	$('#' + this.parentNode.id ).fadeOut('slow');
	 });
	 
	 $('#btn_add_tbl_fk_depois').click(function() {
	 	esconde('add_table_fk');
	 });
	 
	 $('#btn_add_tbl_fk').click(function() {
	 	if($('input:checked','#add_table_fk').length == 0){
	 		alert('Selecione a(s) tabela(s) para adicionar!');
	 	}else{
	 		var arrValor = new Array();
	 		var n = 0;
	 		$('input:checked','#add_table_fk' ).each(function(){
	 			if(this.id != "chk_marcar_todos"){
	 				arrValor[n] = this.value;
	 				n++;
	 			}
			});
			$.ajax({
			   type: "POST",
			   url: window.location,
			   data: "combo_sch=" + $('[name=combo_sch]').val() + "&combo_tbl=" + $('[name=combo_tbl]').val() + "&ajaxMethod=saveTableApoio&tbls=" + arrValor,
			   success: function(msg){
			   		$('#add_table_fk').html( msg );
			   		window.setTimeout( "esconde('add_table_fk')" , '5000' );
			   		
			   		$.ajax({
					   type: "POST",
					   url: window.location,
					   data: "combo_sch=" + $('[name=combo_sch]').val() + "&combo_tbl=" + $('[name=combo_tbl]').val() + "&ajaxMethod=reloadComboSchema",
					   success: function(msg){
					   		$('#td_combo_sch').html( msg );
					   }
					 });
					 
					 $.ajax({
					   type: "POST",
					   url: window.location,
					   data: "combo_sch=" + $('[name=combo_sch]').val() + "&combo_tbl=" + $('[name=combo_tbl]').val() + "&ajaxMethod=reloadComboTable",
					   success: function(msg){
					   		$('#td_combo_tbl').html( msg );
					   }
					 });
					 
			   }
			 });
	 	}
	 });
	 
	 $('[name=inp_table_description]').keypress(function(event){
	 	if(event.keyCode == 13){
		 		if(!$('[name=inp_table_description]').val()){
		 		alert('Informe a descrição da tabela.');
		 		$('[name=inp_table_description]').focus();
		 	}else{
		 		if( $('[name=inp_table_description]').val() == $('#span_table_description').html() ){
		 			$('#div_table_description').fadeOut('slow');
		 		}else{
			 		$.ajax({
					   type: "POST",
					   url: window.location,
					   data: "combo_sch=" + $('[name=combo_sch]').val() + "&combo_tbl=" + $('[name=combo_tbl]').val() + "&ajaxMethod=saveTableDescription&params=" + $('[name=inp_table_description]').val(),
					   success: function(msg){
					   		$('#span_table_description').html( $('[name=inp_table_description]').val() );
							$('#div_table_description').fadeOut('slow');
							return false;
					   }
					 });
				}
		 	}
	 		return false;
	 	}
	 });
	 
	 $('#img_add_table_description').click(function() {
	 	$('[class~=absolute]').fadeOut('slow');
	 	
	 	if($('#img_add_table_description').attr("src") == "../imagens/icon_campus_2.png"){
	 		$('[name=inp_table_description]').val( $('#span_table_description').html() );
	 	}
	 	
	 	$('#div_table_description').fadeIn('slow');
	 });
	 
	  $('#btn_table_description').click(function() {
	 	if(!$('[name=inp_table_description]').val()){
	 		alert('Informe a descrição da tabela.');
	 		$('[name=inp_table_description]').focus();
	 	}else{
	 		if( $('[name=inp_table_description]').val() == $('#span_table_description').html() ){
	 			$('#div_table_description').fadeOut('slow');
	 		}else{
		 		$.ajax({
				   type: "POST",
				   url: window.location,
				   data: "combo_sch=" + $('[name=combo_sch]').val() + "&combo_tbl=" + $('[name=combo_tbl]').val() + "&ajaxMethod=saveTableDescription&params=" + $('[name=inp_table_description]').val(),
				   success: function(msg){
				   		$('#span_table_description').html( $('[name=inp_table_description]').val() );
						$('#div_table_description').fadeOut('slow');
						$('#img_add_table_description').attr("src","../imagens/icon_campus_2.png");
				   }
				 });
			}
	 	}
	 });
	 
	 $('#btn_salvar').click(function() {
		var erro = 0;
		$("[class~=obrigatorio]").each(function() { 
			if(!this.value || this.value == "Selecione..."){
				erro = 1;
				if(arrLabel['' + this.id + '']){
					alert('Favor preencher o campo: ' + arrLabel['' + this.id + '']);
				}else{
					alert('Favor preencher todos os campos obrigatórios!');
				}
				this.focus();
				return false;
			}
		});
		if(erro == 0){
			$("#inp_hdn_acao").val("salvar");
			$("#formulario_tabela_apoio").submit();
		}
	});
});

function selecionaTabelaApoio(tabela){
	if(tabela){
		$("#formulario_tabela_apoio").attr("action","");
		$("#inp_hdn_salvar").val("0");
		$("#formulario_tabela_apoio").submit();
	}
}

function selecionaSchema(schema){
	if(schema){
		$("#formulario_tabela_apoio").attr("action","");
		$("#inp_hdn_salvar").val("0");
		$("#combo_tbl").val("");
		$("#formulario_tabela_apoio").submit();
	}
}

function editarTabela(campo,valor){
	if(campo && valor){
		$("#formulario_tabela_apoio").attr("action","");
		$("#inp_hdn_acao").val("carregar");
		$("#inp_hdn_campo").val(campo);
		$("#inp_hdn_campo_valor").val(valor);
		$("#inp_hdn_salvar").val("0");
		$("#formulario_tabela_apoio").submit();
	}
}

function excluirTabela(campo,valor){
	if(campo && valor && confirm("Deseja realmente excluir o registro?")){
		$("#formulario_tabela_apoio").attr("action","");
		$("#inp_hdn_acao").val("excluir");
		$("#inp_hdn_campo").val(campo);
		$("#inp_hdn_campo_valor").val(valor);
		$("#inp_hdn_salvar").val("0");
		$("#formulario_tabela_apoio").submit();
	}
}

function addTableColumnDesc(schema,table){
	if(schema && table){
		$('[class~=absolute]').fadeOut('slow');
		$('#div_desc_' + schema + "_" + table).fadeIn('slow');
	}else{
		alert('É necessário \'Schema\' e \'Tabela\' para essa operação!');
	}
}

function addListColumnTable(schema,table){
	if(schema && table){
		$('[class~=absolute]').fadeOut('slow');
		$('#div_list_' + schema + "_" + table).fadeIn('slow');
	}else{
		alert('É necessário \'Schema\' e \'Tabela\' para essa operação!');
	}
}

function salvarCampoDescTabela(column,schema,table,value){
	if(schema && table && $("#inp_desc_" + schema + "_" + table ).val()){
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: "valueColumnTable=" + value + "&columnTable=" + column + "&combo_sch=" + $('[name=combo_sch]').val() + "&combo_tbl=" + $('[name=combo_tbl]').val() + "&ajaxMethod=saveColumnTableDescription&schema=" + schema + "&table=" + table + "&column=" + $("#inp_desc_" + schema + "_" + table ).val(),
		   success: function(msg){
		   		if(column){
		   			$('#td_' + column ).html(msg);
		   		}
		   }
		 });
	}else{
		alert('É necessário informar a coluna da tabela!');
	}
}

function salvarCampoListaTabela(schema,table){
	if(schema && table && $("#inp_list_" + schema + "_" + table ).val()){
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: "txt_busca=" + $('[name=txt_busca]').val() + "&combo_sch=" + $('[name=combo_sch]').val() + "&combo_tbl=" + $('[name=combo_tbl]').val() + "&ajaxMethod=saveListColumnTable&column=" + $("#inp_list_" + schema + "_" + table ).val(),
		   success: function(msg){
		   		$("#div_list_" + schema + "_" + table ).fadeOut('slow');
		   		$('#img_list_' + schema + '_' + table ).attr("src","../imagens/alterar.gif");
		   		$("#divLista").html(msg);
		   }
		 });
	}else{
		alert('É necessário informar a(s) coluna(s) da tabela!');
	}
}

function editCommentColumn(column){
	
	$('[class~=absolute]').fadeOut('slow');
	$('#div_column_description_' + column).fadeIn('slow');
}

function saveColumnComment(column){
	$('[class~=absolute]').fadeOut('slow');
	if(column && $("[name=inp_column_description_" + column + "]").val()){
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: "columnComment=" + $("[name=inp_column_description_" + column + "]").val() + "&column=" + column + "&combo_sch=" + $('[name=combo_sch]').val() + "&combo_tbl=" + $('[name=combo_tbl]').val() + "&ajaxMethod=saveColumnComment",
		   success: function(msg){
		   		$('#img_add_column_description_' + column ).attr("src","../imagens/icon_campus_2.png");
		   		
		   		var valor = $('#inp_column_description_' + column).val();
				var posFim = valor.search("</label>");
				if(posFim > 0){
					valor = valor.substr(0,posFim);
					valor = valor.replace("<label>", "")
					$('#name_column_' + column).html(valor);
				}
		   		
		   }
		 });
	}else{
		alert('É necessário informar a coluna da tabela!');
	}
}

function fechar(obj){
	 	$('#' + obj.parentNode.id ).fadeOut('slow');
}

function marcarTodas(obj,local){
	if(obj.checked == true){
		$("input[type=checkbox]","#" + local ).each(function(){
			if(this.id != obj.id)
			this.checked = true;
		});
	}else{
		$("input[type=checkbox]","#" + local ).each(function(){
			if(this.id != obj.id)
			this.checked = false;
		});
	}
}

</script>

<style>
	.SubtituloTabela{background-color:#cccccc}
	.negrito{font-weight:bold}
	.bold{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.direita{text-align: right;}
	.esquerda{text-align: left;}
	.msg_erro{color:#990000}
	.link{cursor: pointer}
	.mini{width:12px;height:12px}
	.sucess_msg{color: blue;}
	.img_middle{vertical-align:middle}
	.hidden{display:none}
	.absolute{position:absolute;padding-top:5px;padding-bottom:5px;padding-left:5px;margin-top:5px;margin-left:50px;border:solid 1px black;background-color: #FFFFFF}
	.fechar{position:relative;right:-5px;top:-26px;}
	.img{background-color:#FFFFFF}
	.red{color:#990000}
</style>

<form name="formulario_tabela_apoio" id="formulario_tabela_apoio"  method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<?php if($tbl->getMessage()): ?>
			<tr id="tr_msg_sucess" class="center SubtituloTabela" >
				<td colspan="2">
					<?= $tbl->getMessage() ?>
				</td>
			</tr>
		<?php endif;
            if($html):
			    echo $html;
            endif;
		    if(!$schema): ?>
			<tr>
				<td width="25%" class="SubTituloDireita" >Selecione o schema:</td>
				<td id="td_combo_sch" >
<!--					--><?php /*$combo_sch = $_POST['combo_sch']; $db->monta_combo('combo_sch',$tbl->getShemasDB(),'S','Selecione o Schema','selecionaSchema','');*/?>
				</td>
			</tr>
		<?php endif;
		    if(!$table || !$schema):
                if($_POST['combo_sch']): ?>
			<tr>
				<td width="25%" class="SubTituloDireita" >Selecione a tabela:</td>
				<td id="td_combo_tbl" >
					<?php $combo_tbl = $_POST['combo_tbl']; $db->monta_combo('combo_tbl',$tbl->getShemaTablesPorSisid(),'S','Selecione a Tabela','selecionaTabelaApoio','','','','','combo_tbl');?>
				</td>
			</tr>
			<?php endif;
		    endif;
		    if($_POST['combo_tbl']):
                $arrColunas = $tbl->getTableColumns();
                if(!$table || !$schema): ?>
				<tr>
					<td colspan="2" class="SubTituloEsquerda" >Colunas da tabela '<?php echo $tbl->getTableName(); ?>' do schema '<?php echo $tbl->getSchemaName(); ?>'</td>
				</tr>
				<tr>
					<?php $description = $tbl->getDescriptionTable(); ?>
					<td class="SubtituloTabela bold direita" >
						Descrição da Tabela:
						<?php if(in_array("comentar_tabela",$tbl->getPermission())): ?>
							<?php echo !$description ? " <img class=\"mini link img_middle\" id=\"img_add_table_description\" src=\"../imagens/icon_campus_2_off.png\" title=\"Clique aqui para adicionar uma descrição.\" />" : " <img class=\"link img_middle mini\" id=\"img_add_table_description\" src=\"../imagens/icon_campus_2.png\" title=\"Clique aqui para editar a descrição.\" />"?>
							<div id="div_table_description" class="hidden absolute" >
									<?php $top = 92; ?>
									<?php echo campo_textarea("inp_table_description","N","S",'',"80","5",'','','','','','',''); ?>
								<input type="button" id="btn_table_description" name="btn_table_description" value="OK" />
								<img class="link fechar" style="top:-<?php echo $top ?>px" title="Fechar" src="../imagens/sair.gif" />
							</div>
						<?php endif; ?>
					</td>
					<td class="SubtituloTabela bold" >
						<span id="span_table_description"><?php echo !$description ? "Descrição não atribuída." : $description ?></span>
					</td>
				</tr>
			<?php endif; ?>
			<?php if(is_array($arrColunas)):
				foreach($arrColunas as $coluna):?>
					<?php $showPK = $editPK ? true : $showPK; ?>
					<?php $arrColu[$coluna['coluna']] = "
					<tr ". ((strstr($coluna['restricao'],"PRIMARY KEY") && !$showPK) || (strstr($coluna['coluna'],"status") && !$showStatus) ? "class='hidden'" : "") ." >
					<td width=\"25%\" class=\"SubtituloDireita\" >";
						if($coluna['comentario']):
								if(strstr($coluna['comentario'],"<label>") && strstr($coluna['comentario'],"</label>")):
									$coluna['label'] = null;
									$coluna['label'] = $tbl->getLabel($coluna['comentario']);
								endif;
						endif;
						$arrColu[$coluna['coluna']] .= "<span id=\"name_column_".$coluna['coluna']."\">
							". ($coluna['label'] ? $coluna['label'] : $coluna['coluna'])."
						</span>
						<script type=\"text/javascript\" >arrLabel['".$coluna['coluna']."'] = '".($coluna['label'] ? $coluna['label'] : $coluna['coluna'])."'</script>";
						if( in_array("comentar_coluna",$tbl->getPermission() ) ):
							if($coluna['comentario']):
								$arrColu[$coluna['coluna']] .= "<img title=\"Clique aqui para editar a descrição da coluna.\" src=\"../imagens/icon_campus_2.png\" onclick=\"editCommentColumn('".$coluna['coluna']."')\" id=\"img_add_column_description_".$coluna['coluna']."\" class=\"link img_middle mini\">";
							else:
								$arrColu[$coluna['coluna']] .= "<img title=\"Clique aqui para editar a descrição da coluna.\" src=\"../imagens/icon_campus_2_off.png\" onclick=\"editCommentColumn('".$coluna['coluna']."')\" id=\"img_add_column_description_".$coluna['coluna']."\" class=\"link img_middle mini\">";
							endif;
							$arrColu[$coluna['coluna']] .= "<div id=\"div_column_description_".$coluna['coluna']."\" class=\"hidden absolute\" >";
							$top = 92;
							$arrColu[$coluna['coluna']] .= campo_textarea("inp_column_description_".$coluna['coluna'],"N","S",'',"80","5",'','','','','','',$coluna['comentario']);
							$arrColu[$coluna['coluna']] .= "<input type=\"button\" id=\"btn_column_description_".$coluna['coluna']."\" onclick=\"saveColumnComment('".$coluna['coluna']."')\" name=\"btn_column_description_".$coluna['coluna']."\" value=\"OK\" />
								<img class=\"link fechar\" style=\"top:-".$top."px\" title=\"Fechar\" src=\"../imagens/sair.gif\" />
							</div>";
						endif;
						$arrColu[$coluna['coluna']] .= "</td>
						<td id=\"td_".$coluna['coluna']."\" >";
							if(!$arrColuna[$coluna['coluna']]){
								$arrColu[$coluna['coluna']] .= $tbl->getInputColumn($coluna,( $arrDados[$coluna['coluna']] != "" ? $arrDados[$coluna['coluna']] : null ) , $editPK, $arrAttr[$coluna['coluna']]);
							}else{
								$arrColu[$coluna['coluna']] .= $arrColuna[$coluna['coluna']];
								if($arrDados[$coluna['coluna']]){
									echo "<script>$(function(){ $(\"[name={$coluna['coluna']}]\").val(\"{$arrDados[$coluna['coluna']]}\")  });</script>";
								}
							}
						$arrColu[$coluna['coluna']] .= "</td>
						</tr>";
						
				?>
				<?php endforeach;?>
				
				<?php echo $arrColu[$tbl->getPK()] ?>
				
				<?php if($arrOrdemColunas){
					foreach($arrOrdemColunas as $chave => $coluna){
						if($coluna != $tbl->getPK()){
							if(!is_array($coluna)){
								if($arrColu[$coluna]){
									echo $arrColu[$coluna];
								}
							}else{?>
								<tr>
									<td class="SubtituloTabela bold" colspan="2" ><?php echo $chave ?></td>
									</td>
								</tr>
								<?php foreach($coluna as $col){
									if($col != $tbl->getPK()){
										if($arrColu[$col]){
											echo $arrColu[$col];
										}
									}
								}
							}
						}
					}
				}else{
					foreach($arrColu as $chave => $coluna){
						if($chave != $tbl->getPK()){
							echo $coluna;
						}
					}
				}
				?>
				
				<input type="hidden" id="inp_hdn_salvar" name="inp_hdn_salvar" value="1" />
			<?php endif; ?>
			<?php if( (in_array("inserir",$tbl->getPermission()) && (!$_POST[$tbl->getPK()] || $_POST[$tbl->getPK()] == "") ) || (in_array("alterar",$tbl->getPermission()) && $arrDados[$tbl->getPK()]) ): ?>
				<tr>
					<td class="SubtituloTabela" ></td>
					<td class="SubtituloTabela" >
						<input type="button" id="btn_salvar" value="Salvar" />
					</td>
				</tr>
			<?php endif; ?>
		<?php endif; ?>
	</table>
	<input type="hidden" id="inp_hdn_acao" name="inp_hdn_acao" value="" />
	<input type="hidden" id="inp_hdn_campo" name="inp_hdn_campo" value="" />
	<input type="hidden" id="inp_hdn_campo_valor" name="inp_hdn_campo_valor" value="" />
	<input type="hidden" id="inp_hdn_schema" name="inp_hdn_schema" value="<?php echo $schema ?>" />
	<input type="hidden" id="inp_hdn_tabela" name="inp_hdn_tabela" value="<?php echo $table ?>" />
<?php if($arrColunas && !$$pk): ?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloTabela bold" colspan="2" >
				Filtrar registros:
				<?php if(in_array("campo_lista",$tbl->getPermission())): ?>
				<?php
					$arrColumns = $tbl->getTableColumns(null,null,null,null,false);
					if(is_array($arrColumns)):
						foreach($arrColumns as $column):
							$arrCol[] = array("codigo" => $column['coluna'] ,"descricao" => $column['coluna']);
						endforeach;
					endif;
					$arrCampos = $tbl->getListColumnTableDescription();
					$arrCampos = $arrCampos ? explode(",",$arrCampos) : null;
					if(is_array($arrCol)){
						$combo = $tbl->monta_combo_multiplo("inp_list_".$tbl->getSchemaName()."_".$tbl->getTableName(),$arrCol,"S","",'','','',4,'200px',$arrCampos,"inp_list_".$tbl->getSchemaName()."_".$tbl->getTableName(),true);
					}
					echo !$arrCampos ? " <img class=\"link img_middle img\" id=\"img_list_".$tbl->getSchemaName()."_".$tbl->getTableName()."\" src=\"../imagens/gif_inclui.gif\" onclick=\"addListColumnTable('{$tbl->getSchemaName()}','{$tbl->getTableName()}')\" title=\"Clique aqui para adicionar o(s) campo(s) para a lista da tabela.\" />" : " <img class=\"link img_middle img\" id=\"img_list_".$tbl->getSchemaName()."_".$tbl->getTableName()."\"  onclick=\"addListColumnTable('{$tbl->getSchemaName()}','{$tbl->getTableName()}')\" src=\"../imagens/alterar.gif\" style=\"background-color:#FFFFFF\" title=\"Clique aqui para alterar o(s) campo(s) da lista da tabela.\" />";
					echo '<div id="div_list_'.$tbl->getSchemaName().'_'.$tbl->getTableName().'" class="hidden absolute" >
								'.$combo.'
								<input type="button" onclick="salvarCampoListaTabela(\''.$tbl->getSchemaName().'\',\''.$tbl->getTableName().'\')" name="btn_list_'.$tbl->getSchemaName().'_'.$tbl->getTableName().'" value="OK" />
								<img class="link fechar" title="Fechar" src="../imagens/sair.gif" />
							</div>';
				?>
				<?php endif; ?>
			</td>
		</tr>
		<tr>
			<td width="25%" class="SubTituloDireita" >Busca:</td>
			<td>
				<?php echo campo_texto('txt_busca','N','S','Busca',60,200,'','','','','','','',$_POST['txt_busca']); ?>
				<input type="submit" name="btn_buscar" id="btn_buscar" value="OK" />
				<?php if($_POST['txt_busca']): ?>
					<input type="button" name="btn_todos" id="btn_todos" value="Mostrar Todos" />
				<?endif; ?>
			</td>
		</tr>
	</table>
</form>
<div id="divLista">
<?php !$$pk ? $tbl->montaListaTabela($arrWhere) : "" ?>
</div>
<?php else: ?>
</form>
<?php endif;?>

<?php if(in_array("inserir_tabela_apoio",$tbl->getPermission())): ?>
	<?php $arrTblFK = $tbl->getTableFK();?>
	<?php if($arrTblFK && is_array($arrTblFK)): ?>
	<br />
	<div id="add_table_fk">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloTabela bold" >As seguintes tabelas não estão disponíveis para edição neste módulo. Deseja adicioná-las? <input type="button" value="Não" id="btn_add_tbl_fk_depois" name="btn_add_tbl_fk_depois" /> </td>
		</tr>
		<tr>
			<td class="SubtituloTabela bold" ><input type="checkbox" title="Marcar / Desmarcar Todos" onclick="marcarTodas(this,'add_table_fk')" id="chk_marcar_todos" value="" /> Tabelas</td>
		</tr>
			<?php $n = 0 ?>
			<?php foreach($arrTblFK as $tabela): ?>
				<?php $cor =  ($n % 2) == 0 ? "#f7f7f7" : "#FFFFFF" ?>
				<tr bgcolor='<?php echo $cor ?>' ><td>
				<input type="checkbox" name="tbl[]" value="<?php echo $tabela ?>" />
				<?php echo $tabela ?>
				</td></tr>
				<?php $n++; ?>
			<?php endforeach; ?>
		<tr>
			<td class="SubtituloTabela bold" > <input type="button" value="Adicionar" name="btn_add_tbl_fk" id="btn_add_tbl_fk" /> </td>
		</tr>
	</table>
	</div>
	<?php endif; ?>
<?php endif; ?>
<?php $errors = $hideWarning ? false : true ?>
<?php if($errors && $tbl->getError()): ?>
	<br />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloTabela bold" >
				Erros Encontrados
			</td>
		</tr>
		<tr>
			<td style="background-color:#FFFFFF" >
				<?php echo $tbl->getError(); ?>
			</td>
		</tr>
	</table>
<?php endif; ?>