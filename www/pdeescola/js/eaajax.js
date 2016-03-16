/*** AJAX REQUESTS ***/

/**
 * Fun��o para redirecionar o usu�rio para a tela inicial (Dados da Escola)
 * do Mais Educa��o e seta as vari�veis de sess�o de acordo.
 */
function redirecionaEA(url,data) {
	var aj = new Ajax.Request(  
			url, {  
			 method:'get',   
			 parameters: data,   
			 onComplete: getResponseRedirecionaEA
			 }  
			);
}

/*** AJAX RESPONSES ***/
function getResponseRedirecionaEA(oReq) {
	//alert(oReq.responseText);
	if(oReq.responseText == "ealista_ano_exercicio") {
		location.href = "pdeescola.php?modulo=eaprincipal/ealista_ano_exercicio&acao=A";
	}
	else if(oReq.responseText == "erro") {
		alert("Ocorreu um erro com o redirecionamento para as informa��es da escola.");
		location.href = "pdeescola.php?modulo=inicio&acao=C";
	}
	else if(oReq.responseText == "erro2") {
		alert("Escola inativa ou inexistente. Procure o Gestor do sistema!");
		location.href = "pdeescola.php?modulo=inicio&acao=C";
	}
	else {
		location.href = "pdeescola.php?modulo=eaprincipal/dados_escola&acao=A";
	}
}