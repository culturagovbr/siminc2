function ajaxatualizar(params,iddestinatario) {
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: params,
			asynchronous: false,
			onComplete: function(resp) {
				if(iddestinatario) {
					document.getElementById(iddestinatario).innerHTML = resp.responseText;
				} 
			},
			onLoading: function(){
				document.getElementById(iddestinatario).innerHTML = 'Carregando...';
			}
		});
}

function validarEnvioCarga(superuser, dataatual) {
	if(!validaData(document.getElementById('agddataprocessamento'))) {
		alert('"Data de processamento" é inválida.');
		return false;
	}

	if(document.getElementById('arquivocsv').value == '') {
		alert('Selecione um arquivo para enviar');
		return false;
	}
	document.getElementById('1click').innerHTML='Processando...';
	document.getElementById('formulario').submit();
}

function verdicionariocarga() {
	window.open('?modulo=principal/cargaporindicador&acao=A&requisicao=verdicionariocarga','Dicionário','scrollbars=yes,height=600,width=600,status=no,toolbar=no,menubar=no,location=no');
}

function efetuarprocessamento(dataproc, dataatual, agdid) {
	if(dataproc == dataatual) {
    	var conf = confirm('A data selecionada é igual a data de hoje. Deseja efetuar o processamento imediatamente?');
    	if(conf) {
    		window.open('./processaragendamento.php?fecharjanela=true&agdid='+agdid,'Agendamento','scrollbars=yes,height=50,width=50,status=no,toolbar=no,menubar=no,location=no');
    	}
    }
}

// inserir um detalhe do indicador
function inserirDetalheTipoIndicador() {
	var tdidsc = prompt("Descrição:");
	if(tdidsc) {
		ajaxatualizar('requisicao=verificarExisteDados','auxiliar');
		// testando se ja possui dados inseridos (para não dar incompatibilidade)
		if(document.getElementById('auxiliar').innerHTML) {
			alert('Não foi possivel inserir um novo detalhe. Existem dados inseridos neste indicador, e a inserção causaria incompatibilidade dos dados.');
			return false;
		}

		ajaxatualizar('requisicao=inserirDetalhesTipoIndicador&tdidsc='+tdidsc,'');
		ajaxatualizar('requisicao=carregarDetalhesTipoIndicador','detalhetipoindicador');
		ajaxatualizar('requisicao=carregarDetalhesTipoIndicador&numeroregistro=true','auxiliar');
		// testando se ja esta no limite
		if(document.getElementById('auxiliar').innerHTML == '2') {
			document.getElementById('inserirTipoIndicador').style.display = 'none';
		}
	}
}

// inserir dados nos detalhes do indicador
function inserirDetalheTipoDadosIndicador(tdiid, referencia) {
	var tiddsc = prompt("Descrição:");
	if(tiddsc) {
		ajaxatualizar('requisicao=verificarExisteDados','auxiliar');
		// testando se ja possui dados inseridos (para não dar incompatibilidade)
		//if(document.getElementById('auxiliar').innerHTML) {
		//	alert('Não foi possivel inserir um novo detalhe. Existem dados inseridos neste indicador, e a inserção causaria incompatibilidade dos dados.');
		//	return false;
		//}
	
		ajaxatualizar('requisicao=inserirDetalhesTipoDadosIndicador&tdiid='+tdiid+'&tiddsc='+tiddsc,'');
		if(document.getElementById('imagem'+tdiid).title == "menos") {
			carregarDetalheTipoDadosIndicador(tdiid, document.getElementById('imagem'+tdiid));
		}
		carregarDetalheTipoDadosIndicador(tdiid, document.getElementById('imagem'+tdiid));
	}
}

function ordenar(tdiid, acao) {
	ajaxatualizar('requisicao=ordenarDetalhesTipoIndicador&tdiid='+tdiid+'&executar='+acao,'');
	ajaxatualizar('requisicao=carregarDetalhesTipoIndicador','detalhetipoindicador');
}

function editarDetalheTipoIndicador(tdiid, referencia) {
	var linha = referencia.parentNode.parentNode.parentNode;
	var tdidsc = prompt("Descrição:", linha.cells[3].innerHTML);
	if(tdidsc) {
		ajaxatualizar('requisicao=atualizarDetalhesTipoIndicador&tdiid='+tdiid+'&tdidsc='+tdidsc,'');
		ajaxatualizar('requisicao=carregarDetalhesTipoIndicador','detalhetipoindicador');
	}
}

function excluirDetalheTipoIndicador(tdiid) {
	var conf = confirm("Deseja realmente excluir este tipo detalhe?");
	if(conf) {
		ajaxatualizar('requisicao=verificarExisteDados','auxiliar');
		// testando se ja possui dados inseridos (para não dar incompatibilidade)
		if(document.getElementById('auxiliar').innerHTML) {
			alert('Não foi possivel excluir detalhe. Existem dados inseridos neste indicador.');
			return false;
		}
	
		ajaxatualizar('requisicao=removerDetalhesTipoIndicador&tdiid='+tdiid,'detalhetipoindicador');
		ajaxatualizar('requisicao=carregarDetalhesTipoIndicador','detalhetipoindicador');
		ajaxatualizar('requisicao=carregarDetalhesTipoIndicador&numeroregistro=true','auxiliar');
		if(document.getElementById('auxiliar').innerHTML != '2') {
			document.getElementById('inserirTipoIndicador').style.display = '';
		}
	}
}
function excluirDetalheTipoDadosIndicador(tidid, tdiid, referencia) {
	var conf = confirm("Deseja realmente excluir este dado do tipo detalhe?");
	if(conf) {
		ajaxatualizar('requisicao=verificarExisteDados','auxiliar');
		// testando se ja possui dados inseridos (para não dar incompatibilidade)
		if(document.getElementById('auxiliar').innerHTML) {
			alert('Não foi possivel excluir detalhe. Existem dados inseridos neste indicador.');
			return false;
		}

		ajaxatualizar('requisicao=removerDetalhesTipoDadosIndicador&tidid='+tidid,'');
		ajaxatualizar('requisicao=carregarDetalhesTipoDadosIndicador&tdiid='+tdiid,referencia.parentNode.parentNode.parentNode.parentNode.parentNode.parentNode.id);
	}
	
}


function editarDetalheTipoDadosIndicador(tidid, tdiid, referencia) {
	var linha = referencia.parentNode.parentNode.parentNode;
	var tiddsc = prompt("Descrição:", linha.cells[1].innerHTML);
	if(tiddsc) {
		ajaxatualizar('requisicao=atualizarDetalhesTipoDadosIndicador&tidid='+tidid+'&tiddsc='+tiddsc,'');
		ajaxatualizar('requisicao=carregarDetalhesTipoDadosIndicador&tdiid='+tdiid,referencia.parentNode.parentNode.parentNode.parentNode.parentNode.parentNode.id);
	}
	
}

function carregarDetalheTipoDadosIndicador(tdiid, referencia) {
	var linha = referencia.parentNode.parentNode.parentNode;
	var tabela = referencia.parentNode.parentNode.parentNode.parentNode;
	switch(referencia.title) {
		case 'mais':
			referencia.title = "menos";
			referencia.src = "../imagens/menos.gif";
			var novalinha = tabela.insertRow(linha.rowIndex);
			var col0 = novalinha.insertCell(0);
			var col1 = novalinha.insertCell(1);
			col0.innerHTML = "&nbsp;";
			col1.colSpan=4;
			col1.id="coluna"+novalinha.rowIndex;
			ajaxatualizar('requisicao=carregarDetalhesTipoDadosIndicador&tdiid='+tdiid,"coluna"+novalinha.rowIndex);
			break;
		case 'menos':
			referencia.title = "mais";
			referencia.src = "../imagens/mais.gif";
			tabela.deleteRow(linha.rowIndex);
			break;
	}
}

function redimencionarBodyData() {
	if(document.getElementById('bodydata')) {
		var myHeight;
		if( typeof( window.innerWidth ) == 'number' ) {
		   	//Non-IE
		   	myHeight = window.innerHeight;
		   	document.getElementById('bodydata').style.height = myHeight - 535; 
		} else if( document.documentElement && ( document.documentElement.clientWidth || document.documentElement.clientHeight ) ) {
		   	//IE 6+ in 'standards compliant mode'
		   	myHeight = document.documentElement.clientHeight;
		} else if( document.body && ( document.body.clientWidth || document.body.clientHeight ) ) {
		   	//IE 4 compatible
		   	myHeight = document.body.clientHeight;
		   	document.getElementById('bodydata').style.height = myHeight - 700;
		}
	}
}

function executar_periodoestado(valor) {
	if(valor) {
		document.getElementById('estuf').disabled = false;
		document.getElementById('estuf').name = 'estuf';
		select_innerHTML(document.getElementById('estuf'), "<option value=''>Carregando...</option>");
		var myAjax = new Ajax.Request(
			window.location.href,
			{
				method: 'post',
				parameters: 'requisicao=carregarComboUFPorPeriodo&dpeid='+valor+'&consulta='+document.getElementById('spanestuf').innerHTML,
				asynchronous: false,
				onComplete: function(resp) {
					select_innerHTML(document.getElementById('estuf'), resp.responseText);
				}
			});

	} else  {
		document.getElementById('estuf').disabled = true;
		document.getElementById('estuf').name = 'estuf_disabled';
	}
}

function executar_estadomunicipio(valor) {
	var evento = document.getElementById('muncod').onchange;
	document.getElementById('muncod').disabled = false;
	select_innerHTML(document.getElementById('muncod'), "<option value=''>Carregando...</option>");
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: 'requisicao=carregarComboMunicipioPorUF&dpeid='+document.getElementById('dpeid').value+'&estuf='+valor+'&consulta='+document.getElementById('spanmuncod').innerHTML,
			asynchronous: false,
			onComplete: function(resp) {
				select_innerHTML(document.getElementById('muncod'), resp.responseText);
			}
		});
	//ajaxatualizar('requisicao=&estuf='+valor,'tdmuncod');
	document.getElementById('muncod').onchange = evento;
}
// montar grid por escola (montar+redimensionar+totalizar)
function montargrid_brasil() {
	document.getElementById('gridinfo').innerHTML='<p align="center">Carregando...</p>';
        var nroAnoReferencia = document.getElementById('nroAnoReferencia').value;
	ajaxatualizar('requisicao=carregarGridBrasil&nroAnoReferencia='+nroAnoReferencia,'gridinfo');
	somarTodasColunas();
}
// montar grid por escola (montar+redimensionar+totalizar)
function montargrid_polo(valor) {
	document.getElementById('gridinfo').innerHTML='<p align="center">Carregando...</p>';
	ajaxatualizar('requisicao=carregarGridPolo&dpeid='+document.getElementById('dpeid').value+'&muncod='+valor,'gridinfo');
	redimencionarBodyData();
	somarTodasColunas();
}
// montar grid por escola (montar+redimensionar+totalizar)
function montargrid_escola(valor) {
	document.getElementById('gridinfo').innerHTML='<p align="center">Carregando...</p>';
	ajaxatualizar('requisicao=carregarGridEscola&dpeid='+document.getElementById('dpeid').value+'&muncod='+valor,'gridinfo');
	redimencionarBodyData();
	somarTodasColunas();
}
// montar grid por posgraducao (montar+redimensionar+totalizar)
function montargrid_posgraducao(valor) {
	document.getElementById('gridinfo').innerHTML='<p align="center">Carregando...</p>';
	ajaxatualizar('requisicao=carregarGridPosgraducao&dpeid='+document.getElementById('dpeid').value+'&muncod='+valor,'gridinfo');
	redimencionarBodyData();
	somarTodasColunas();
}

// montar grid por ies (montar+redimensionar+totalizar)
function montargrid_ies(valor) {
	document.getElementById('gridinfo').innerHTML='<p align="center">Carregando...</p>';
	ajaxatualizar('requisicao=carregarGridIES&dpeid='+document.getElementById('dpeid').value+'&muncod='+valor,'gridinfo');
	redimencionarBodyData();
	somarTodasColunas();
}

// montar grid por ies (montar+redimensionar+totalizar)
function montargrid_iescpc(valor) {
	document.getElementById('gridinfo').innerHTML='<p align="center">Carregando...</p>';
	ajaxatualizar('requisicao=carregarGridIESCPC&dpeid='+document.getElementById('dpeid').value+'&muncod='+valor,'gridinfo');
	redimencionarBodyData();
	somarTodasColunas();
}

function montargrid_instituto(valor) {
	document.getElementById('gridinfo').innerHTML='<p align="center">Carregando...</p>';
	ajaxatualizar('requisicao=carregarGridInstituto&dpeid='+document.getElementById('dpeid').value+'&muncod='+valor,'gridinfo');
	redimencionarBodyData();
	somarTodasColunas();

}

function montargrid_hospital(valor) {
	document.getElementById('gridinfo').innerHTML='<p align="center">Carregando...</p>';
	ajaxatualizar('requisicao=carregarGridHospital&dpeid='+document.getElementById('dpeid').value+'&muncod='+valor,'gridinfo');
	redimencionarBodyData();
	somarTodasColunas();

}


function montargrid_universidade(valor) {
	document.getElementById('gridinfo').innerHTML='<p align="center">Carregando...</p>';
	ajaxatualizar('requisicao=carregarGridUniversidade&dpeid='+document.getElementById('dpeid').value+'&muncod='+valor,'gridinfo');
	redimencionarBodyData();
	somarTodasColunas();

}

function montargrid_campus_profissional(valor) {
	document.getElementById('gridinfo').innerHTML='<p align="center">Carregando...</p>';
	ajaxatualizar('requisicao=carregarGridCampusProfissional&dpeid='+document.getElementById('dpeid').value+'&muncod='+valor,'gridinfo');
	redimencionarBodyData();
	somarTodasColunas();

}

function montargrid_campus_superior(valor) {
	document.getElementById('gridinfo').innerHTML='<p align="center">Carregando...</p>';
	ajaxatualizar('requisicao=carregarGridCampusSuperior&dpeid='+document.getElementById('dpeid').value+'&muncod='+valor,'gridinfo');
	redimencionarBodyData();
	somarTodasColunas();

}
// montar grid por municipio (montar+redimensionar+totalizar)
function montargrid_municipio(valor) {
	document.getElementById('gridinfo').innerHTML='<p align="center">Carregando...</p>';
	if(document.getElementById('dpeid').value) {
		ajaxatualizar('requisicao=carregarGridMunicipio&dpeid='+document.getElementById('dpeid').value+'&estuf='+valor,'gridinfo');
		redimencionarBodyData();
		somarTodasColunas();
	} else {
		document.getElementById('gridinfo').innerHTML = "";
	}	
}
// montar grid por estado (montar+redimensionar+totalizar)
function montargrid_estado(valor) {
	document.getElementById('gridinfo').innerHTML='<p align="center">Carregando...</p>';
	if(document.getElementById('dpeid').value) {
		ajaxatualizar('requisicao=carregarGridEstado&dpeid='+document.getElementById('dpeid').value,'gridinfo');
		redimencionarBodyData();
		somarTodasColunas();
	} else {
		document.getElementById('gridinfo').innerHTML = "";
	}
}

function somarTodasColunas() {
	for(var i=0;i<document.getElementById('linhatotais').cells.length;i++) {
		for(var j=0;j < document.getElementById('linhatotais').cells[i].childNodes.length;j++) {
			if(document.getElementById('linhatotais').cells[i].childNodes[j].type == "text") {
				somarcoluna(document.getElementById('linhatotais').cells[i].childNodes[j]);
			}
		
		}
	}
}

/* Função para subustituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}

function somarcoluna(obj) {
	var tabela = obj.parentNode.parentNode.parentNode.parentNode;
	var coluna = obj.parentNode.cellIndex;
	var totalquantidade = 0;
	var totalvalor = 0;
	var definicaomascaraqtde = false;
	var definicaomascaravalor = false;
	for(var j=0;j<(tabela.rows.length-2);j++) {
		if(tabela.rows[j].cells[coluna]) {
			var isquantidade = true;
			for(var k=0;k<tabela.rows[j].cells[coluna].childNodes.length;k++) {
				if(tabela.rows[j].cells[coluna].childNodes[k].type == "text") {
					if(tabela.rows[j].cells[coluna].childNodes[k].value) {
						if(isquantidade) {
							if(definicaomascaraqtde == false) {
								definicaomascaraqtde = true;
								if(tabela.rows[j].cells[coluna].childNodes[k].value.indexOf(',') == -1) {
									var casasqtde=0;							
								} else {
									var casasqtde=2;
								}
							}

							totalquantidade += parseFloat(replaceAll(replaceAll(tabela.rows[j].cells[coluna].childNodes[k].value,".",""),",","."));
							isquantidade = false;	
						} else {
							if(definicaomascaravalor == false) {
								definicaomascaravalor = true;
								if(tabela.rows[j].cells[coluna].childNodes[k].value.indexOf(',') == -1) {
									var casasvalor=0;							
								} else {
									var casasvalor=2;
								}
							}
						
							totalvalor += parseFloat(replaceAll(replaceAll(tabela.rows[j].cells[coluna].childNodes[k].value,".",""),",","."));
						}
					}
				}
			}
		}
	}
	if(totalquantidade) {
		if(document.getElementById('linhatotais').cells[coluna].childNodes.length == 1) {
			document.getElementById('linhatotais').cells[coluna].childNodes[0].value=totalquantidade.toFixed(casasqtde);
			document.getElementById('linhatotais').cells[coluna].childNodes[0].onkeyup();
		} else {
			document.getElementById('linhatotais').cells[coluna].childNodes[2].value=totalquantidade.toFixed(casasqtde);
			document.getElementById('linhatotais').cells[coluna].childNodes[2].onkeyup();
		}
	}
	if(totalvalor) {
		document.getElementById('linhatotais').cells[coluna].childNodes[7].value=totalvalor.toFixed(casasvalor);
		document.getElementById('linhatotais').cells[coluna].childNodes[7].onkeyup();
	}
}

function excluirAgendamento(agdid) {
	var conf = confirm('Deseja realmente excluir o agendamento?');
	if(conf) {
		ajaxatualizar('requisicao=excluirAgendamento&agdid='+agdid,'');
		ajaxatualizar('requisicao=carregaragendamento','listaagendamento');
	}
}

function select_innerHTML(objeto,innerHTML){
/******
* select_innerHTML - corrige o bug do InnerHTML em selects no IE
* Veja o problema em: http://support.microsoft.com/default.aspx?scid=kb;en-us;276228
* Versão: 2.1 - 04/09/2007
* Autor: Micox - Náiron José C. Guimarães - micoxjcg@yahoo.com.br
* @objeto(tipo HTMLobject): o select a ser alterado
* @innerHTML(tipo string): o novo valor do innerHTML
*******/
    objeto.innerHTML = ""
    var selTemp = document.createElement("micoxselect")
    var opt;
    selTemp.id="micoxselect1"
    document.body.appendChild(selTemp)
    selTemp = document.getElementById("micoxselect1")
    selTemp.style.display="none"
    if(innerHTML.indexOf("<option")<0){//se não é option eu converto
        innerHTML = "<option>" + innerHTML + "</option>"
    }
    innerHTML = innerHTML.replace(/<option/g,"<span").replace(/<\/option/g,"</span")
    selTemp.innerHTML = innerHTML
      
    
    for(var i=0;i<selTemp.childNodes.length;i++){
  var spantemp = selTemp.childNodes[i];
  
        if(spantemp.tagName){     
            opt = document.createElement("OPTION")
    
   if(document.all){ //IE
    objeto.add(opt)
   }else{
    objeto.appendChild(opt)
   }       
    
   //getting attributes
   for(var j=0; j<spantemp.attributes.length ; j++){
    var attrName = spantemp.attributes[j].nodeName;
    var attrVal = spantemp.attributes[j].nodeValue;
    if(attrVal){
     try{
      opt.setAttribute(attrName,attrVal);
      opt.setAttributeNode(spantemp.attributes[j].cloneNode(true));
     }catch(e){}
    }
   }
   //getting styles
   if(spantemp.style){
    for(var y in spantemp.style){
     try{opt.style[y] = spantemp.style[y];}catch(e){}
    }
   }
   //value and text
   opt.value = spantemp.getAttribute("value")
   opt.text = spantemp.innerHTML
   //IE
   opt.selected = spantemp.getAttribute('selected');
   opt.className = spantemp.className;
  } 
 }    
 document.body.removeChild(selTemp)
 selTemp = null
}

function validarsolicitacao(btn) {

	if(document.getElementById('solprazodata').value == "") {
		alert('Data do prazo de resposta é obrigatória');
		return false;
	}
	
	if(document.getElementById('solprazohora').value == "") {
		alert('Hora do prazo de resposta é obrigatória');
		return false;
	}
	
	if(document.getElementById('soldesc').value == "") {
		alert('Descrição é obrigatória');
		return false;
	}
	
	document.getElementById('formulario').submit();
	
	btn.disabled=true
}

function validarencaminhamento(btn) {
	if(document.getElementById('pessoas').value == "") {
		alert('Selecione destinatarios');
		return false;
	}

	if(document.getElementById('encdesc').value == "") {
		alert('Mensagem é obrigatória');
		return false;
	}
	
	if(document.getElementById('encprazodata').value == "") {
		alert('Data do prazo do encaminhamento é obrigatória');
		return false;
	}
	
	if(document.getElementById('encprazohora').value == "") {
		alert('Hora do prazo do encaminhamento é obrigatória');
		return false;
	}
	

	document.getElementById('formulario').submit();
	btn.disabled=true
}

function validarresposta(btn) {

	if(document.getElementById('rsptxtresposta').value == "") {
		alert('Resposta é obrigatória');
		return false;
	}

	document.getElementById('formulario').submit();
	btn.disabled=true
}

function inserirNovosArquivos() {
	var tabela = document.getElementById('anexos');
	for(i=0;i<1;i++) {
		var line = tabela.insertRow((tabela.rows.length-2));
		var cell = line.insertCell(0);
		cell.innerHTML = "<input type=\"file\" name=\"arquivo[]\">";
		var cell1 = line.insertCell(1);
		cell1.innerHTML = "Nome do arquivo : <input class=\"normal\" type=\"text\" name=\"arquivonome[]\">";
		var cell2 = line.insertCell(2);
		cell2.innerHTML = "<img src=\"../imagens/excluir.gif\" onclick=\"document.getElementById('anexos').deleteRow(this.parentNode.parentNode.rowIndex);\">";
	}
}

function arquivarsolicitacao(solid) {
	var conf = confirm('Deseja realmente arquivar esta solicitação?');
	
	if(conf) {
		window.location='painel.php?modulo=principal/solicitacoes/solicitacao&acao=A&requisicao=arquivarsolicitacao&solid='+solid;
	}
	
}

