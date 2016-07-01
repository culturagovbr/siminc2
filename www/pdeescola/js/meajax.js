/*** AJAX REQUESTS ***/
function carregaComboAtividade(url,data) {
	var aj = new Ajax.Request(  
	url, {  
	 method:'get',   
	 parameters: data,   
	 onComplete: getResponseCarregaComboAtividade
	 }  
	);  
}

/**
 * Fun��o para redirecionar o usu�rio para a tela inicial (Diretor)
 * do Mais Educa��o e seta as vari�veis de sess�o de acordo.
 */
function redirecionaME(url,data) {
	var aj = new Ajax.Request(  
			url, {  
			 method:'get',   
			 parameters: data,   
			 onComplete: getResponseRedirecionaME
			 }  
			);
}

/**
 * Fun��o que verifica se o valor informado para o Alunado
 * Participante est� correto. Este valor deve ser menor ou
 * igual ao valor referente no censo.
 */
function verificaAlunadoCenso(url,data) {
	var aj = new Ajax.Request(  
			url, {
			 asynchronous: false,  
			 method:'get',   
			 parameters: data,   
			 onComplete: getResponseVerificaAlunadoCenso
			 }
			);
}

function testaRequisitosPST(url,data) {
	var aj = new Ajax.Request(  
			url, {
			 asynchronous: false,  
			 method:'get',   
			 parameters: data,   
			 onComplete: getResponseTestaRequisitosPST
			 }
			);
}

function aderirPST(url,data) {
	var selMacroCampo = document.getElementById('macrocampo');
	
	selMacroCampo.disabled = true;

	var aj = new Ajax.Request(  
			url, {
			 asynchronous: false,  
			 method:'get',   
			 parameters: data,   
			 onComplete: getResponseAderirPST
			 }
			);
}

function naoAderirPST(url,data) {
	var aj = new Ajax.Request(  
			url, {
			 asynchronous: false,  
			 method:'get',   
			 parameters: data,   
			 onComplete: getResponseNaoAderirPST
			 }
			);
}

function desfazerEscolhaAdesaoAjax(url,data) {
	var aj = new Ajax.Request(  
			url, {
			 asynchronous: false,  
			 method:'get',   
			 parameters: data,   
			 onComplete: getResponseDesfazerEscolhaAdesaoAjax
			 }
			);
}

/*** AJAX RESPONSES ***/
function getResponseCarregaComboAtividade(oReq) {
	var divAtividade = document.getElementById('combo_atividade');
	
	divAtividade.innerHTML = oReq.responseText;
}

function getResponseRedirecionaME(oReq) {
	if(oReq.responseText == "melista_ano_exercicio") {
		location.href = "pdeescola.php?modulo=meprincipal/melista_ano_exercicio&acao=A";
	}
	else {
		location.href = "pdeescola.php?modulo=meprincipal/dados_escola&acao=A";
	}
}

function getResponseVerificaAlunadoCenso(oReq) {
	var arrayRetorno = oReq.responseText.split('@');
	
	if(arrayRetorno[0] == "erro") {
		alert("O Alunado informado n�o pode ser maior que o valor referente do Censo.");
		$('libera_validacao_alunado').value = "0";
		$('alunado_participante_' + arrayRetorno[1]).select();
	}
}

function getResponseTestaRequisitosPST(oReq) {
	var requisitosPST = document.getElementById('requisitos_pst'); 
	
	if(oReq.responseText == 'ERRO') {
		alert("Modalidade de Ensino n�o encontrada. Refa�a o procedimento.");
		window.location='pdeescola.php?modulo=melista&acao=E&requisicao=cadastra';
		return false;
	}
	
	if(oReq.responseText == 'true')
		requisitosPST.value = "1";
	else
		requisitosPST.value = "0";
}

function getResponseAderirPST(oReq) {
	var selMacroCampo 	= document.getElementById('macrocampo');
	var segundoTempo = document.getElementById('segundo_tempo');
	var idMacroCampo, mtapst;

	alert("Agora voc� dever� selecionar mais uma atividade constante na lista deste Macrocampo. \n\nIsto garantir� o envio de recursos para o ressarcimento do monitor, bem como para a aquisi��o do Kit espec�fico. \n\nLembramos que este monitor tamb�m ficar� respons�vel pelo desenvolvimento das atividades do Programa Segundo Tempo.");
	
	idMacroCampo = selMacroCampo.options[selMacroCampo.selectedIndex].value;
	
	segundoTempo.value 	= "S";
	mtaatividadepst 	= 'true';

	//document.getElementById('bt_desfazer_adesao').style.display = "block";	
	carregaComboAtividade('meajax.php', 'tipo=carrega_atividades&id='+idMacroCampo+'&mtaatividadepst='+mtaatividadepst+'&aderiu=1');
}

function getResponseNaoAderirPST(oReq) {
	var selMacroCampo 	= document.getElementById('macrocampo');
	var segundoTempo = document.getElementById('segundo_tempo');
	var idMacroCampo, mtapst;
	
	idMacroCampo = selMacroCampo.options[selMacroCampo.selectedIndex].value;
	
	segundoTempo.value 	= "N";
	mtaatividadepst 	= 'false';
	
	//document.getElementById('bt_fazer_adesao').style.display = "block";
	carregaComboAtividade('meajax.php', 'tipo=carrega_atividades&id='+idMacroCampo+'&mtaatividadepst='+mtaatividadepst+'');
}

function getResponseDesfazerEscolhaAdesaoAjax(oReq) {
	if(oReq.responseText=='true') {
		alert("Dados gravados com sucesso.");
		location.href = "pdeescola.php?modulo=meprincipal/atividades_ano_atual&acao=A";
	} else {
		alert("Erro ao registrar a informa��o. Entre em contato com o Administrador do sistema.");
	}
}
