function ajaxatualizarAsync(params,iddestinatario) {
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: params,
   		async: true,
   		success: function(html){
   			if(iddestinatario!='') {
   				document.getElementById(iddestinatario).innerHTML = html;
   			}
   		}
	});

}

function ajaxatualizar(params,iddestinatario) {
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: params,
   		async: false,
   		success: function(html){
   			if(iddestinatario!='') {
   				document.getElementById(iddestinatario).innerHTML = html;
   			}
   		}
	});

}

function excluirInforme(inpid) {
	var conf = confirm('Deseja excluir este informe?');

	if(conf) {
		ajaxatualizar('requisicao=excluirInforme&inpid='+inpid,'');
		window.location=window.location;
	}
	
}

function importarInformacoesSispacto(tela) {
	var conf = confirm('Deseja importar as informa��es desta tela inseridas no SISPACTO 2013? As informa��es inseridas ser�o sobrescritas');

	if(conf) {
		window.location='sispacto2.php?modulo=principal/universidade/universidade&acao=A&requisicao=importarInformacoesSispacto&tela='+tela;
	}
	
}


function salvarOrientacaoAdm() {
	
	if(jQuery('#oabdesc').val() == '') {
			alert('Orienta��o em branco');
			return false;
	}
	
	jQuery('#formulario_orientacao').submit();
	
}

function inserirInforme(pflcod) {
	var conteudo = prompt('Digite o informe : ');
	if(conteudo) {
		ajaxatualizar('requisicao=inserirInforme&pflcoddestino='+pflcod+'&inpdescricao='+conteudo,'');
		window.location=window.location;
	}
}

function mostrarOrientacaoAdm(abaid) {
	
	if(abaid=='') {
		alert('N�o foi encontrado o menu');
		return false;
	}
	
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: '&requisicao=carregarOrientacaoPorFiltro&abaid='+abaid,
   		async: false,
   		success: function(texto){
   			jQuery('#oabdesc').val(texto);
   			
   		}
	});
	
	jQuery('#abaid').val(abaid);

	jQuery("#modalOrientacaoAdm").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 400,
	                        modal: true,
	                     	close: function(){} 
	                    });

}

function carregarUniversidadesPorUF(estuf) {
	if(estuf) {
		ajaxatualizar('requisicao=carregarUniversidadesPorUF&id=uncid_sim&name=uncid&estuf='+estuf,'td_universidade');
	} else {
		document.getElementById('td_universidade').innerHTML = "Selecione uma UF";
	}
}

function carregarMunicipiosPorUF(estuf) {
	if(estuf) {
		ajaxatualizar('requisicao=carregarMunicipiosPorUF&id=muncod_sim&name=muncod&estuf='+estuf,'td_municipio');
	} else {
		document.getElementById('td_municipio').innerHTML = "Selecione uma UF";
	}
}

function carregarMunicipiosPorUF2(estuf) {
	if(estuf) {
		ajaxatualizar('requisicao=carregarMunicipiosPorUF&id=muncod_nascimento&name=muncod_nascimento&estuf='+estuf,'td_municipio2');
	} else {
		document.getElementById('td_municipio2').innerHTML = "Selecione uma UF";
	}
}

function carregarMunicipiosPorUF3(estuf) {
	if(estuf) {
		ajaxatualizar('requisicao=carregarMunicipiosPorUF&id=muncod_endereco&name=muncod_endereco&estuf='+estuf,'td_municipio3');
	} else {
		document.getElementById('td_municipio3').innerHTML = "Selecione uma UF";
	}
}

function carregarMunicipiosPorUF4(estuf) {
	if(estuf) {
		ajaxatualizar('requisicao=carregarMunicipiosPorUF&onclick=buscarAgencias&id=muncod_agencias&name=muncod_agencias&estuf='+estuf,'td_municipio4');
	} else {
		document.getElementById('td_municipio4').innerHTML = "Selecione uma UF";
	}
}


function carregarMunicipiosPorUF5(estuf) {
	if(estuf) {
		ajaxatualizar('requisicao=carregarMunicipiosPorUF&id=muncod_abrangencia&name=muncod_abrangencia&estuf='+estuf,'td_municipio5');
	} else {
		document.getElementById('td_municipio5').innerHTML = "Selecione uma UF";
	}
}


function carregarCoordenadorLocal() {
	
	var params = '&requisicao=carregarCoordenadorLocal&direcionar=true';
	
	if(document.getElementById('estuf_sim')) {
		params += '&estuf='+document.getElementById('estuf_sim').value;
	}
	
	if(document.getElementById('muncod_sim')) {
		params += '&muncod='+document.getElementById('muncod_sim').value;
	}
	
	window.location = 'sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A'+params;

}

function carregarCoordenadorIES() {
	
	var params = '&requisicao=carregarCoordenadorIES&direcionar=true';
	
	if(document.getElementById('estuf_sim')) {
		params += '&estuf='+document.getElementById('estuf_sim').value;
	}
	
	if(document.getElementById('uncid_sim')) {
		params += '&uncid='+document.getElementById('uncid_sim').value;
	}
	
	window.location = 'sispacto.php?modulo=principal/universidade/universidade&acao=A'+params;

}

function carregarEnderecoPorCEP_coordenadorLocal(cep) {
	jQuery.ajax({
   		type: "POST",
   		url: '/geral/consultadadosentidade.php',
   		data: 'requisicao=pegarenderecoPorCEP&endcep='+cep,
   		async: false,
   		success: function(resposta){
			var endereco = resposta.split("||");
			if(endereco[3] && endereco[4]) {
				jQuery('#ienlogradouro').val(endereco[0]);
				jQuery('#ienbairro').val(endereco[1]);
				jQuery('#estuf_endereco').val(endereco[3]);
				jQuery('#td_municipio3').html(endereco[2]+' <input type="hidden" id="muncod_endereco" name="muncod_endereco" value="'+endereco[4]+'">');
			} else {
				alert('CEP inexistente na base dos Correios');
				limparEnderecoPorCEP_coordenadorLocal();
			}
		}
	});
}

function carregarEnderecoPorCEP_dirigente(cep) {
	jQuery.ajax({
   		type: "POST",
   		url: '/geral/consultadadosentidade.php',
   		data: 'requisicao=pegarenderecoPorCEP&endcep='+cep,
   		async: false,
   		success: function(resposta){
			var endereco = resposta.split("||");
			if(endereco[3] && endereco[4]) {
				jQuery('#unilogradouro').val(endereco[0]);
				jQuery('#unibairro').val(endereco[1]);
				jQuery('#uniuf').val(endereco[3]);
				jQuery('#td_municipio_dirigente').html(endereco[2]+' <input type="hidden" id="muncod_endereco" name="muncod_endereco" value="'+endereco[4]+'">');
			} else {
				alert('CEP inexistente na base dos Correios');
				limparEnderecoPorCEP_dirigente();
			}
		}
	});
}

function limparEnderecoPorCEP_coordenadorLocal() {
	jQuery('#iencep').val('');
	jQuery('#ienlogradouro').val('');
	jQuery('#ienbairro').val('');
	jQuery('#estuf_endereco').val('');
	jQuery('#td_municipio3').html('Digite CEP');
}

function limparEnderecoPorCEP_dirigente() {
	jQuery('#unicep').val('');
	jQuery('#unilogradouro').val('');
	jQuery('#unibairro').val('');
	jQuery('#uniuf').val('');
	jQuery('#td_municipio_dirigente').html('Digite CEP');
}

function removerDocumentoDesignacao(iuaid) {
	var conf = confirm('Deseja realmente excluir este anexo?');
	
	if(conf) {
		divCarregando();
		window.location=window.location+'&requisicao=removerDocumentoDesignacao&iuaid='+iuaid;
	}
}

function abrirSubatividade(atiid, obj) {

	if(obj.title=='menos') {
		document.getElementById('atiid_'+atiid).style.display = 'none';
		obj.title='mais';
		obj.src='../imagens/mais.gif';
	} else {
		document.getElementById('atiid_'+atiid).style.display = '';
		obj.title='menos';
		obj.src='../imagens/menos.gif';
	}

}

function abrirTurmaOutros(turid, obj) {
	var params='';
	if(document.getElementById('formacaoinicial')) {
		params += '&formacaoinicial='+document.getElementById('formacaoinicial').value;
	}

	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.title    = "menos";
		obj.src      = "../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex);
		var ncol     = nlinha.insertCell(0);
		ncol.colSpan = 8;
		ncol.id      = 'tur_coluna_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=carregarAlunosTurmaOutros&consulta=true&turid='+turid+params,ncol.id);
	} else {
		obj.title    = "mais";
		obj.src      = "../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex);
	}


}

function abrirTurma(turid, obj) {
	var params='';
	if(document.getElementById('formacaoinicial')) {
		params += '&formacaoinicial='+document.getElementById('formacaoinicial').value;
	}

	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.title    = "menos";
		obj.src      = "../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex);
		var ncol     = nlinha.insertCell(0);
		ncol.colSpan = 8;
		ncol.id      = 'tur_coluna_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=carregarAlunosTurma&consulta=true&turid='+turid+params,ncol.id);
	} else {
		obj.title    = "mais";
		obj.src      = "../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex);
	}


}

function selecionarPeriodoReferencia(fpbid) {
	divCarregando();
	window.location=replaceAll(window.location,'#','')+'&fpbid='+fpbid;
}

function carregarMunicipioMateriais(campo,opcao,uncid) {
	window.open(window.location+'&requisicao=verMunicipioMateriais&uncid='+uncid+'&campo='+campo+'&opcao='+opcao,'imagem','width=800,height=600,resizable=yes');
}

function detalharPorcentagemPerfil(fpbid, uncid, obj) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	if(obj.title=="mais") {
		divCarregando();
		obj.title    = "menos";
		obj.src      = "../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex+1);
		var ncol     = nlinha.insertCell(0);
		ncol.colSpan = 7;
		ncol.id      = 'dtl2_coluna_'+(nlinha.rowIndex+1);
		ajaxatualizar('requisicao=exibirPorcentagemPagamentoPerfil&uncid='+uncid+'&fpbid='+fpbid,ncol.id);
		divCarregado();
	} else {
		obj.title    = "mais";
		obj.src      = "../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex+1);
	}
}



function detalharProfessoresAusentes(uncid, obj) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.title    = "menos";
		obj.src      = "../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex);
		var ncol     = nlinha.insertCell(0);
		ncol.colSpan = 3;
		ncol.id      = 'dtl_coluna_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=professoresAlfabetizadoresAusentes&uncid='+uncid,ncol.id);
	} else {
		obj.title    = "mais";
		obj.src      = "../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex);
	}
}


