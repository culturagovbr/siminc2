function filtraSetorRespon(obj){
	var boSuperUsuario = verificaSeSuperUsuario();
	if(($('unaidsetorresponsavel').value != $('unaidsetororigem').value) && !boSuperUsuario ){
		//select = document.getElementsByName('usucpfresponsavel')[0];
		document.formulario.usucpfresponsavel.options[0].selected = true;
		document.formulario.usucpfresponsavel.disabled = true;
	} else {
		document.formulario.usucpfresponsavel.disabled = false;
		td 	   = document.getElementById('td_usucpfresponsavel2');
		select = document.getElementsByName('usucpfresponsavel')[0];
		
		if (select){
			select.disabled = true;
			select.options[0].text = 'Aguarde...';
			select.options[0].selected = true;
		}	
		
		// Faz uma requisi��o ajax, passando o parametro 'ordid', via POST
		var req = new Ajax.Request('ajax.php', {
								        method:     'post',
								        asynchronous: false,
								        parameters: 'tipo=unaidSetorResp&unaid=' + $('unaidsetorresponsavel').value,
								        onComplete: function (res)
								        {
											//$('teste').innerHTML = res.responseText;
											td.innerHTML = res.responseText;
											//td.style.visibility = 'visible';
								        }
								  });
		
		var usucpfresponsavelAnterior = document.getElementById('usucpfresponsavelAnterior');
		if(usucpfresponsavelAnterior.value != ''){
			var comboUsucpfresponsavel = document.getElementById('usucpfresponsavel');
			for (var i = 0; i < comboUsucpfresponsavel.length; i++) {
				var indiceCombo = comboUsucpfresponsavel.options[i].index;
				var textoCombo = comboUsucpfresponsavel.options[i].text;
				var valorCombo = comboUsucpfresponsavel.options[i].value;
				
				if(valorCombo == usucpfresponsavelAnterior.value){
					comboUsucpfresponsavel.options[i].selected = true;
				}
			}			
		}
		
	   	// Espera 100 milisegundos para dar tempo da fun��o AJAX ser executada.
		//window.setTimeout('alteraComboUsuResp()', 1000);
	}
	
}

/*
function alteraComboUsuResp(){
	var st_usucpfresponsavel = '<?php echo $obTarefa->usucpfresponsavel; ?>';
	if(st_usucpfresponsavel){
		var comboUsucpfresponsavel = document.getElementById('usucpfresponsavel');
		for (var i = 0; i < comboUsucpfresponsavel.length; i++) {
			var indiceCombo = comboUsucpfresponsavel.options[i].index;
			var textoCombo = comboUsucpfresponsavel.options[i].text;
			var valorCombo = comboUsucpfresponsavel.options[i].value;
			
			if(valorCombo == st_usucpfresponsavel){
				comboUsucpfresponsavel.options[i].selected = true;
			}
		}
	}
}
*/

function validaPermissaoSetores(){
	/**
	* VALIDA��O DE SETOR RESPONS�VEL E USU�RIO RESPONS�VEL 
	*/
	var boSuperUsuario = verificaSeSuperUsuario();
	if(!boSuperUsuario){
		document.formulario.unaidsetorresponsavel.disabled   = true;
		document.formulario.usucpfresponsavel.disabled 	     = true;
		document.formulario.sitid.disabled 					 = true;
		document.formulario.tardataprazoatendimento.disabled = true;
		document.formulario.tardepexterna.disabled 			 = true;
		
		var setorUsuarioLogado = recuperaSetorUsuarioLogado();
		var cpfUsuarioLogado   = recuperaCpfUsuarioLogado();
		var boPerfilGerente    = verificaSeGerente();
		
		/**
		 * SETOR RESPONS�VEL
		 */
		if(boPermissaoSetorRespon( boPerfilGerente, setorUsuarioLogado, $('unaidsetororigem').value, $('unaidsetorresponsavel').value )){
			document.formulario.unaidsetorresponsavel.disabled   = false;	
		}
		
		/**
		 * PESSOA RESPONS�VEL
		 */
		if(boPermissaoPessoaRespon( boPerfilGerente, setorUsuarioLogado, $('unaidsetorresponsavel').value )){
			document.formulario.usucpfresponsavel.disabled   = false;	
		}
		
		/**
		 * VALIDA��O DE SITUA��O 
		 */
		if(boPermissaoSituacao( boPerfilGerente, setorUsuarioLogado, $('unaidsetororigem').value, $('unaidsetorresponsavel').value, cpfUsuarioLogado, $('usucpfresponsavelAnterior').value) ){
			$('sitid').disabled  = false;
			document.formulario.sitid.disabled 	= false;	
		}
		
		/**
		 * PRAZO
		 */
		if(boPermissaoPrazo(boPerfilGerente, setorUsuarioLogado, $('unaidsetororigem').value, $('unaidsetorresponsavel').value)){
			document.formulario.tardataprazoatendimento.disabled   = false;
		}
		
		/**
		 * DEPENDENCIA EXTERNA
		 */
		if(boPermissaoDependencia(boPerfilGerente, setorUsuarioLogado, $('unaidsetororigem').value, $('unaidsetorresponsavel').value)){
			document.formulario.tardepexterna.disabled   = false;
		}
	}
}

function validacaoInicial(){
	if($('cadAcompanhamento') != null || ($('cadAtividade') != null || $('cadTarefa') != null && $('tarid').value != '') ){
		validaPermissaoSetores();
		if($('cadAtividade') == null || $('cadTarefa') == null && document.formulario.unaidsetorresponsavel.disabled == false && document.formulario.usucpfresponsavel.disabled == false){
			var usucpfresponsavel = document.getElementsByName('usucpfresponsavel')[0];
			if(usucpfresponsavel.value == ''){
				filtraSetorRespon($('unaidsetorresponsavel'));
			}
		}
	} else {
		filtraSetorRespon($('unaidsetorresponsavel'));
	}
}