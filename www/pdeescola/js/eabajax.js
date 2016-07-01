/*** AJAX REQUESTS ***/

/**
 * Fun��o para redirecionar o usu�rio para a tela inicial (Dados da Escola)
 * do Escola Aberta e seta as vari�veis de sess�o de acordo.
 */
function redirecionaEAB(url,data) {
	var aj = new Ajax.Request(  
			url, {  
			 method:'get',   
			 parameters: data,   
			 onComplete: getResponseRedirecionaEAB
			 }  
			);
}

/*** AJAX RESPONSES ***/
function getResponseRedirecionaEAB(oReq) {
	if(oReq.responseText == "eablista_ano_exercicio") {
		location.href = "pdeescola.php?modulo=eabprincipal/eablista_ano_exercicio&acao=A";
	}
	else if(oReq.responseText == "erro") {
		alert("Ocorreu um erro com o redirecionamento para as informa��es da escola.");
		location.href = "pdeescola.php?modulo=inicio&acao=C";
	}
	else {
		location.href = "pdeescola.php?modulo=eabprincipal/dados_escola&acao=A";
	}
}