$(document).ready(function() {
	
	mostraPreExigida();
	
	$('#addArquivo').click(function(){
		
		var qtd = $('#arquivos tr').length-1;
		
		if( $('#'+qtd).attr('name') != '' ){
			$('.linha').each(function(){
				qtd = parseInt($(this).attr('name'))+1;
			});
		}
		
		var html =  '<tr class="linha" id="arq'+qtd+'" name="'+qtd+'">'+
						'<td style="border-bottom: 1px solid #cccccc;">'+
							'<input type="text" class=" normal" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" value="" maxlength="255" size="80" name="arqdsc['+qtd+']" style="text-align:left;">'+
						'</td>'+
						'<td style="border-bottom: 1px solid #cccccc;">'+
							'<input type="file" name="arq'+qtd+'"/>'+
						'</td>'+
						'<td style="border-bottom: 1px solid #cccccc;">'+
							'<center>'+
								'<img src="../imagens/excluir.gif" title="Excluir" class="excluirarq" name="arq'+qtd+'" />'+
							'</center>'+
						'</td>'+
					'</tr>'
		$('#bordainferior').before(html);
	});
	
	$('.excluirarq').live('click',function(){
		
		if($(this).attr('id')!=''){
			if(confirm('Deseja excluir o arquivo?')){
				var arq = $(this).attr('name');
				jQuery.ajax({
					type: "POST",
					url: window.location,
					data: "req=excluirArquivo&arcid="+$(this).attr('id'),
					async: false,
					success: function(msg){ 
						if(msg){
							$('#'+arq).remove();
						}else{
							alert('Arquivo n�o p�de ser removido. Contate o Administrador.');
						}
					}
				});
			}
		}else{
			if(confirm('Deseja excluir o arquivo?')){
				$('#'+$(this).attr('name')).remove();
			}
		}
	});
	
	$('#voltar').click(function(){
		window.location = 'catalogocurso.php?modulo=inicio&acao=C';
	});
	
	$('#proximo').click(function(){
		window.location = 'catalogocurso.php?modulo=principal/cadOrganizacaoCurso&acao=A';
	});
	
	$('#salvarC').click(function(){
		$('#link').val('proximo');
		$('#salvar').click();
	});
	
	$('#salvar').click(function(){
		
		$(this).attr('disabled',true);
		
		//Valida Campo obrigat�rio
		var vazio;
		var erro;
		var test = false;
		erro = true;
		$('input[name$="redid[]"]').each(function(){
			test = $(this).attr('checked');
			if(test){
				erro = false;
			}
		});
		if(erro){
			alert('Campo obrigat�rio.');
			$('input[name$="redid[]"]').focus();
			$(this).removeAttr('disabled');
			return false;
		}
		test = false;
		erro = true;
		$('[name="modid[]"]').each(function(){
			test = $(this).attr('checked');
			if(test){
				erro = false;
			}
		});
		if(erro){
			alert('Campo obrigat�rio.');
			$('input[name$="redid[]"]').focus();
			$(this).removeAttr('disabled');
			return false;
		}
		$('.obrigatorio').each(function(){
			if($(this).val() == ''){
				vazio = $(this);
				erro = true;
				return false;
			}
		});
		$('#cod_etapa_ensino option').each(function(){
			if($(this).val()==''){
				erro = true;
				vazio = $('#cod_etapa_ensino');
			}
		});
		
		if(erro){
			alert('Campo obrigat�rio.');
			vazio.focus();
			$(this).removeAttr('disabled');
			return false;
		}
		
		// Valida Carga Hor�ria
		var curchmim  = parseInt($('#curchmim').val());
		var curchmax  = parseInt($('#curchmax').val());
		if(mim>max){
			alert('Carga hor�ria M�NIMA deve ser MENOR ou IGUAL a carga hor�ria M�XIMA.');
			$('#curchpremim').focus();
			$(this).removeAttr('disabled');
			return false;
		}
		
		var modid = $('#modid').val();
		var limMsg = '';
		
		var ncuid = $('#ncuid').val();

		switch(ncuid) {
			case '1'://Extens�o
				if( (curchmim < 10 || curchmax >= 180) ){
					limMsg = 'Carga hor�ria deve ser MAIOR ou IGUAL a 10 HORAS e  MENOR que 180 HORAS.';
				}
				break;
			case '2'://Aperfei�oamento
				if( (curchmim < 180 || curchmax >= 360) ){
					limMsg = 'Carga hor�ria deve ser MAIOR ou IGUAL a 180 HORAS e MENOR que 360 HORAS.';
				}
				break;
			case '3'://Especializa��o
				if( (curchmim < 360 || curchmax >= 1000) ){
					limMsg = 'Carga hor�ria deve ser MAIOR ou IGUAL a 360 HORAS e MENOR que 1000 HORAS.';
				}
				break;
		}
		
		if( limMsg != '' ){
			alert(limMsg);
			$(this).removeAttr('disabled');
			return false;
		}
		
		//Valida Numero de estudantes por turma
		var mim    = parseInt($('#curnumestudanteminpre').val());
		var ideal  = parseInt($('#curnumestudanteidealpre').val());
		var max    = parseInt($('#curnumestudantemaxpre').val());
		
		if( ((mim>ideal)||(mim>max) || (ideal>max))&&modid!='3' ){
			alert('Numero de estudantes por turma inv�lido. (Dica: M�nimo < Ideal < M�ximo)');
			$(this).removeAttr('disabled');
			return false;
		}
		
		mim    = parseInt($('#curnumestudantemindist').val());
		ideal  = parseInt($('#curnumestudanteidealdist').val());
		max    = parseInt($('#curnumestudantemaxdist').val());
		
		if( (mim>ideal)||(mim>max) || (ideal>max)&&modid!='1' ){
			alert('Numero de estudantes por turma inv�lido. (Dica: M�nimo < Ideal < M�ximo)');
			$(this).removeAttr('disabled');
			return false;
		}
		
		//Validacao Vid�ncia do Curso
		var inicio = $('#curinicio').val();
		var fim    = $('#curfim').val();
		var obData = new Data();
		
		if( inicio != '' && fim != '' ){
			if( obData.comparaData( inicio, fim, '>' ) ){
				alert('Periodo inv�lido.');
				$('#curinicio').focus();
				$(this).removeAttr('disabled');
				return false;
			}
		}
		
		var antcurchmim  = parseInt($('#antcurchmim').val());
		var antcurchmax  = parseInt($('#antcurchmax').val());
		
		if( antcurchmim > 0 || antcurchmax > 0 ){
			var alterado = false;
			var msg = '';
			if( antcurchmim != curchmim ){
				msg = '- Valor M�NIMO de carga hor�ria est� sendo alterado.\n';
				alterado = true;
			}
			if( antcurchmax != curchmax ){
				msg += '- Valor M�XIMO de carga hor�ria est� sendo alterado.\n';
				alterado = true;
			}
			if(alterado){
				if(!confirm(msg+'Salvar esses valores implica na exclus�o dos itens de Organiza��o de curso. Deseja continuar?')){
					$(this).attr('disabled',false);
					return false;
				}
			}
		}
		$('#req').val('salvarCatalogo');
		selectAllOptions( document.getElementById('cod_etapa_ensino') );
		$('#frmCatalogo').submit();
	});
	
	$('#curchmim').keyup(function(){
		mostraPreExigida();
	});
	
	$('#curchmax').keyup(function(){
		mostraPreExigida();
	});
	
	$('#curpercpremim').keyup(function(){
		var val = $(this).val();
		if(val<1){
			$(this).val('0');
		}
		if(val>100){
			$(this).val('100');
		}
	});
	
	$('#curpercpremax').keyup(function(){
		var val = $(this).val();
		if(val<1){
			$(this).val('0');
		}
		if(val>100){
			$(this).val('100');
		}
	});
	
	$('[name="modid[]"]').click(function(){
		
		mostraPreExigida();
	});
	
	$('#btoHistorico').click(function(){
		if($('#historico').css('display')=='none'){
			$('#historico').show();
		}else{
			$('#historico').hide();
		}
	});
	
});

function forceFocus( obj ){
	$('#'+obj).focus();
	if(!$('#'+obj).is(":focus")){
		forceFocus( obj );
	}
}

function mostraPreExigida( ){
	
	var teste = true;
	
	$('[name="modid[]"]').each(function(){
		if( $(this).attr('checked') && $(this).val()!='1' ){
			teste = false;
		}
	});
	if( teste ){
		$('#preexigida').hide();
		$('#curpercpremim').removeClass('obrigatorio');
		$('#curpercpremax').removeClass('obrigatorio');
	}else{
		$('#preexigida').show();
		$('#curpercpremim').addClass('obrigatorio');
		$('#curpercpremax').addClass('obrigatorio');
	}
}

function abreArquivo( arqid ){
	
	window.open( 'catalogocurso.php?modulo=principal/cadCatalogo&acao=A&req=abreArquivo&arqid='+arqid, 'guia', 'width=40,height=40,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	
}
