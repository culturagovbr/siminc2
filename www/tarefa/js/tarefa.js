function alteraIcone(id,tarpai,tartarefa, trId, tarAberto, tipo, arFiltros, boCarregaLinkAjax) {
	if(trId == ''){
		trId = id;
	}
	
	if(id == tartarefa){
		var idTemp = "";
	} else {
		var idTemp = "_"+id;
	}

	if(tarpai){
		if(tartarefa == tarpai){
			var tarpaiTemp = "";
		}else if(idTemp == ""){
			var tarpaiTemp = '_'+tarpai+'_';
		} else {
			var tarpaiTemp = '_'+tarpai;
		}		
	} else {
		var tarpaiTemp = '';
	}
	
	
	var img = 'img_'+tartarefa+tarpaiTemp+idTemp;
	var tabela = document.getElementById('tabela_tarefa');
	var i = document.getElementById(img);
	if (tipo == 1) {
		if (tarAberto == 't' && i) { // Se estiver aberto e a imagem setada, mudamos a imagem e mandamos para o banco
			document.getElementById(img).src = "../imagens/menos.gif";
			/*var data = 'tipo=mudatarAberto&tarid=' + id + '&tarAberto=true';
			var aj = new Ajax.Request('ajax.php',  
			{  
				method: 'post',   
				parameters: data,
				asynchronous: false,
				onComplete: function(r){}
			});*/
				montaFilhos('ajax.php', 'tipo=montaFilhos&tarid='+id+'&tarpai='+tarpai+'&tartarefa='+tartarefa+'&trId='+trId+'&arFiltros='+arFiltros, arFiltros, boCarregaLinkAjax);
		} else if (tarAberto == 'f' && i) {
			document.getElementById(img).src = "../imagens/mais.gif";
			/*var data = 'tipo=mudatarAberto&tarid=' + id + '&tarAberto=false';
			var aj = new Ajax.Request('ajax.php',  
			{
				method: 'post',   
				parameters: data,
				asynchronous: false,
				onComplete: function(r){}
			});*/
			
			for(i=0; i < tabela.rows.length; i++) {
				if(tabela.rows[i].id.search(id+"_") >= 0) {
					tabela.rows[i].style.display = "none";
				}
			}
			
			
		}
	} else {
		if (i && i.src.search("mais.gif") > 0) {
			document.getElementById(img).src = "../imagens/menos.gif";
			var data = 'tipo=mudatarAberto&tarid=' + id + '&tarAberto=true';
			/*var aj = new Ajax.Request('ajax.php',  
			{  
				method: 'post',   
				parameters: data,
				asynchronous: false,
				onComplete: function(r){}
			});*/
				montaFilhos('ajax.php', 'tipo=montaFilhos&tarid='+id+'&tarpai='+tarpai+'&tartarefa='+tartarefa+'&trId='+trId+'&arFiltros='+arFiltros, arFiltros, boCarregaLinkAjax);
		} else {
			document.getElementById(img).src = "../imagens/mais.gif";
			/*var data = 'tipo=mudatarAberto&tarid=' + id + '&tarAberto=false';
			var aj = new Ajax.Request('ajax.php',  
			{  
				method: 'post',   
				parameters: data,
				asynchronous: false,
				onComplete: function(r){}
			});*/
			
			for(i=0; i < tabela.rows.length; i++) {
				if(tabela.rows[i].id.search(id+"_") >= 0) {
					tabela.rows[i].style.display = "none";
				}
			}
			
			/**
	       	* MUDAR COR DAS TR
	       	*/
	       	var cor = "#f0f0f0";
			if($('tabela_tarefa').rows.length > 1){
				for (var i = 1; i < $('tabela_tarefa').rows.length; i++) {
					var tr = $('tabela_tarefa').rows[i];
					if(tr.style.display != 'none'){
						if(tr.style.backgroundColor != 'rgb(255, 255, 204)'){
							if(cor == "#fafafa") {
								tr.style.backgroundColor = "#f0f0f0";
								cor = "#f0f0f0";
							} else {
								tr.style.backgroundColor = "#fafafa";
								cor = "#fafafa";
							}
						}					
					}
					
				}
			}
			
		}
	} //fim do else tipo 
}
function alteraIconeArvoreAberta(trId){
	var img = 'img_'+trId;
	var i = document.getElementById(img);
	var tabela = $('tabela_tarefa');
	if(i.src.search("menos.gif") > 0){
		i.src = "../imagens/mais.gif";
		for(i=0; i < tabela.rows.length; i++) {
			if(tabela.rows[i].id.search(trId+"_") >= 0) {
				tabela.rows[i].style.display = "none";
			}
		}
	} else if(i.src.search("mais.gif") > 0){
		i.src = "../imagens/menos.gif";
		for(i=0; i < tabela.rows.length; i++) {
			if(tabela.rows[i].id.search(trId+"_") >= 0) {
				tabela.rows[i].style.display = "";
			}
		}
	}
}


function montaPai(_tartarefa, arFiltros, boCarregaLinkAjax) {
	$('aguarde').hide();
	var maxRows = $('tabela_tarefa').rows.length;
	if(maxRows > 1){
		for (var i = 1; i < maxRows; i++) {
			$('tabela_tarefa').deleteRow(1);
		}
	}
	/*if((arFiltros == "" || arFiltros == ",,,") && $('cadAcompanhamento') == null){
		tr = $('tabela_tarefa').insertRow(1);
		td1 = tr.insertCell(0);
		td1.colSpan = '6';
		td1.style.textAlign = "center";
		td1.style.color = "#6495ED";
		td1.style.fontWeight = 'bold';
		td1.innerHTML = "� necess�rio escolher um filtro.";
		return false;
	}*/

	var data = 'tipo=montaPai&_tartarefa='+_tartarefa+'&arFiltros='+arFiltros;
	var cor = "#f0f0f0";
	var aj = new Ajax.Request('ajax.php',  
	{  
		method: 'post',
		asynchronous: false,
		parameters: data,
		onLoading: $('aguarde_').show(),
		onLoading: $('tabela_tarefa').setOpacity(0.3),
		onComplete: function(r)
		{
	//	$('teste').update(r.responseText);
		//alert(r.responseText); return false;
			if(r.responseText != ""){
				eval(r.responseText);
				if(arDados.length >= 1){
					for (var j = 0; j < arDados.length; j++) {
						var tarid 					= arDados[j].tarid;
						var tarpai 					= arDados[j].tarpai;
						var tartarefa 				= arDados[j].tartarefa;
			       		var tartitulo 				= arDados[j].tartitulo;
			       		var nomeresponsavel 		= arDados[j].nome;
			       		var boFilho 				= arDados[j].boFilho;
			       		var boAnexo 				= arDados[j].boAnexo;
			       		var boRestricao 			= arDados[j].boRestricao;
			       		var tardataprazoatendimento = arDados[j].tardataprazoatendimento;
			       		var img 					= arDados[j].img;
			       		var barraExecucao			= arDados[j].barraExecucao;
			       		var dataPrazo   			= arDados[j].dataPrazo;
			       		var setorRespon   			= arDados[j].setorrespon;
			       		var codTarefa   			= arDados[j].codTarefa;
			       		var tardepexterna   		= arDados[j].tardepexterna;
			       		var prioridade   			= arDados[j].prioridade;

                		if(tarpai){
                			var st_tarefa_atividade = 'Atividade';
                		} else {
                			var st_tarefa_atividade = 'Tarefa';
                		}
                		
						var tr = $('tabela_tarefa').insertRow($('tabela_tarefa').rows.length);
						tr.id = tarid;
						
						if($('tarid').value == tarid){
							tr.style.background = '#ffffcc';
						} else {
				       		if(cor == "#fafafa") {
								tr.style.backgroundColor = "#f0f0f0";
								cor = "#f0f0f0";
							} else {
								tr.style.backgroundColor = "#fafafa";
								cor = "#fafafa";
							}
						}
						
						td1 = tr.insertCell(0);
                		td1.style.textAlign = "center";
                		
                		if(tarpai == ""){
                			tarpai = tartarefa;
                		}
                		
                		
                		td1.innerHTML = "<img src=\"../imagens/gif_inclui.gif\" onClick=\"window.location.href='tarefa.php?modulo=principal/cadAtividade&acao=A&tarpai="+tarpai+"&tartarefa="+tartarefa+"'\" style=\"border:0; cursor:pointer;\" title=\"Incluir uma atividade a Atividade\">"
                		td1.innerHTML += "&nbsp;<img src=\"../imagens/alterar.gif\" onClick=\"window.location.href='tarefa.php?modulo=principal/cadTarefa&acao=A&tarid="+tarid+"'\" style=\"border:0; cursor:pointer;\" title=\"Alterar "+st_tarefa_atividade+"\">"
						td1.innerHTML += "&nbsp;<img src=\"../imagens/excluir.gif\" style=\"border:0; cursor:pointer;\" title=\"Excluir "+st_tarefa_atividade+"\" onClick=\"excluirTarefaAtividade('"+tarid+"');\" >";
						
						
						if(prioridade == 'U'){
							var imgPrioridade = '<img src=\'../imagens/pd_urgente.JPG\' /> Urgente';
						} else if(prioridade == 'A'){
							var imgPrioridade = '<img src=\'../imagens/pd_alta.JPG\' /> Alta';
						} else {
							var imgPrioridade = '<img src=\'../imagens/pd_normal.JPG\' /> Normal';						
						}
						
						tdPrioridade = tr.insertCell(1);						
						tdPrioridade.innerHTML = imgPrioridade;
						
						/**
						* BLOCO PARA MONTAR IMAGENS ANEXO E RESTRI��O NA LISTA
						*/
						var imgAnexo = "";
						if(boAnexo){
							imgAnexo = "<img src=\"../imagens/anexo.gif\" onClick=\"window.location.href='tarefa.php?modulo=principal/cadDocumento&acao=A&tarid="+tarid+"'\" style=\"border:0; cursor:pointer;\" title=\"Anexo\">";
						}
						var imgRestricao = "";
						if(boRestricao){
							imgRestricao = "<img src=\"../imagens/restricao.png\" onClick=\"window.location.href='tarefa.php?modulo=principal/cadAcompanhamento&acao=A&tarid="+tarid+"&boPadraoRetricao=1';\" style=\"border:0; cursor:pointer;\" title=\"Restri��o\">";
						}
						var imgDepexterna = "";
						if(tardepexterna){
							imgDepexterna = "<img src=\"../imagens/botao_de.png\" title=\"Depend�ncia Externa\">";
						}
						
						/*
						* VERIFICA SE O LINK DEVE SER CARREGADO COM AJAX OU N�O, S� DEVER� USAR AJAX NO cadAcompanhamento
                		*/
                		if(boCarregaLinkAjax){
                			var onclick = "onClick=\"carregaCabecalhoAtendimento("+tarid+", this.parentNode.parentNode);\"";
                		} else {
                			var onclick = "onClick=\"window.location.href='tarefa.php?modulo=principal/cadAcompanhamento&acao=A&tarid="+tarid+"';\"";
                		}
						td2 = tr.insertCell(2);						
                		if( boFilho ) {
							td2.innerHTML = "<a href=\"#\" onclick=\"alteraIcone('"+tarid+"','"+ tarpai+"','"+ tartarefa+"', '"+tr.id+"', '', 2, '"+arFiltros+"' );\">"+
											"<img id=\"img_"+tarid+"\" src=\"../imagens/"+img+"\" border=\"0\"></a>&nbsp;&nbsp;"+imgAnexo+imgRestricao+" <a href=\"#\" title=\""+tartitulo+"\" "+onclick+" ><b>"+codTarefa+' - '+tartitulo+"</b></a> "+imgDepexterna;
						} else {
				            td2.innerHTML = imgAnexo+imgRestricao+" <a href=\"#\" title=\""+tartitulo+"\" "+onclick+" ><b>"+codTarefa+' - '+tartitulo+"</b></a> "+imgDepexterna;
				        }
				        
				        /**
						* RESPONS�VEL
						*/
				        td3 = tr.insertCell(3);
				        td3.style.color = "#1E90FF";
				        td3.style.cursor = "pointer";
						td3.innerHTML = "<span onclick=\"alteraResponsavel('td3_" +tarid + "')\" >"+setorRespon+' - '+nomeresponsavel+"</span>";
						td3.id = 'td3_'+tarid;
												
						/**
						* SITUA��O
						*/
						td4 = tr.insertCell(4);
						td4.style.cursor = "pointer";
						td4.setAttribute( 'align', 'center' );
						td4.id = 'td_'+tarid;
						array = barraExecucao.split('@@');
						td4.innerHTML = "<span onclick=\"posicionaSlider('td_" +tarid + "')\" >"+array[0]+"</span>";
						td4.status = array[1];
						td4.percentual = array[2];
						//carregaBarraExecucao(td4,'tipo=carrega_barra_execucao&tarid='+tarid+'',tarid);
						
						/**
						* PRAZO ATENDIMENTO
						*/
						td5 = tr.insertCell(5);
						td5.style.textAlign = "center";
						td5.style.cursor = "pointer";
						td5.style.color = "#008000";
						td5.title = "Alterar Prazo de Atendimento";
						td5.id = 'dataprazo_'+tarid;
						mostraDataPrazoFormatada(td5, dataPrazo, tarid);
						//carregaDataPrazo(td5,'tipo=carrega_data&tarid='+tarid,tarid);
						
						/**
						* ORDEM
						*/
						td6 = tr.insertCell(6);
						td6.setAttribute( 'align', 'center' );
						var desabilitadoB = "";
						var desabilitadoC = "";
						var linkB = "onclick=\"mudaPosicao('baixo',this.parentNode.parentNode.rowIndex, '"+tarid+"', '"+tarid+"', '', '', '"+arFiltros+"')\"";
						var cursorB = "style=\"cursor: pointer;\""; 
						
						var linkC = "onclick=\"mudaPosicao('cima',this.parentNode.parentNode.rowIndex, '"+tarid+"', '"+tarid+"', '', '', '"+arFiltros+"')\"";
						var cursorC = "style=\"cursor: pointer;\""; 
							
						if(j == 0){
							desabilitadoC = "d";
							linkC = "";
							cursorC = "";
						}
						if(j + 1 == arDados.length){
							desabilitadoB = "d";
							linkB = "";
							cursorB = ""; 
						}
						td6.innerHTML = "&nbsp;<img "+linkB+" "+cursorB+" src=\"../imagens/seta_baixo"+desabilitadoB+".gif\" />";
						td6.innerHTML += "&nbsp;<img "+linkC+" "+cursorC+" src=\"../imagens/seta_cima"+desabilitadoC+".gif\" />";
					}
				}
			} else {
				tr = $('tabela_tarefa').insertRow(1);
				td1 = tr.insertCell(0);
				td1.colSpan = '6';
				td1.style.textAlign = "center";
				td1.style.color = "#FF0000";
				td1.innerHTML = "N�o foi encontrado nenhum registro.";
			}
		}
		
	});
	$('aguarde_').hide();
	$('tabela_tarefa').setOpacity(1);
}

function montaArvoreAberta(_tartarefa, arFiltros, boCarregaLinkAjax) {
	$('aguarde').hide();
	var maxRows = $('tabela_tarefa').rows.length;
	if(maxRows > 1){
		for (var i = 1; i < maxRows; i++) {
			$('tabela_tarefa').deleteRow(1);
		}
	}
	var data = 'tipo=montaArvoreAberta&_tartarefa='+_tartarefa+'&arFiltros='+arFiltros;
	var cor = "#f0f0f0";
	var aj = new Ajax.Request('ajax.php',  
	{  
		method: 'post',
		asynchronous: false,
		parameters: data,
		//onLoading: $('aguarde_').show(),
		onLoading: $('tabela_tarefa').setOpacity(0.3),
		onComplete: function(r)
		{
			//$('teste').update(r.responseText);
			if(r.responseText != ""){
				eval(r.responseText);
				if(arDados.length >= 1){
			       	var idNivel					= new Array();
					for (var j = 0; j < arDados.length; j++) {
						var tarid 					= arDados[j].tarid;
						var tarpai 					= arDados[j].tarpai;
						var tartarefa 				= arDados[j].tartarefa;
			       		var tartitulo 				= arDados[j].tartitulo;
			       		var nomeresponsavel 		= arDados[j].nome;
			       		var boFilho 				= arDados[j].boFilho;
			       		var boAnexo 				= arDados[j].boAnexo;
			       		var boRestricao 			= arDados[j].boRestricao;
			       		var tardataprazoatendimento = arDados[j].tardataprazoatendimento;
			       		var img 					= arDados[j].img;
			       		var tarordem				= arDados[j].tarordem;
			       		var barraExecucao			= arDados[j].barraExecucao;
			       		var dataPrazo   			= arDados[j].dataPrazo;
			       		var boCima					= arDados[j].boCima;
			       		var boBaixo					= arDados[j].boBaixo;
			       		var setorRespon   			= arDados[j].setorrespon;
			       		var codTarefa   			= arDados[j].codTarefa;
			       		var tardepexterna   		= arDados[j].tardepexterna;
			       		var prioridade   			= arDados[j].prioridade;
			       		//var taraberto 				= arDados[j].taraberto;
			       		
			       		if(tarpai){
                			var st_tarefa_atividade = 'Atividade';
                		} else {
                			var st_tarefa_atividade = 'Tarefa';
                		}
			       		
			       		if(tarid == tartarefa){
							var idTemp = "";
						} else {
							var idTemp = "_"+tarid;
						}
						
						if(tartarefa == tarpai){
							var tarpaiTemp = "";
						}
						
						if(tarpai){
							if(tartarefa == tarpai){
								var tarpaiTemp = "";
							}else if(idTemp == ""){
								var tarpaiTemp = '_'+tarpai+'_';
							} else {
								var tarpaiTemp = '_'+tarpai;
							}		
						} else {
							tarpaiTemp = '';
						}
			       		
						var tr = $('tabela_tarefa').insertRow($('tabela_tarefa').rows.length);
						
						var tamanho = tarordem.length;
						var nivel = tamanho / 4;
						
						idNivel[nivel] = tarid;
						var id = '';
						// prepara para forma o id das TR
						for (i=1; i <= nivel; i++){
							id += (i == 1 ? idNivel[i] : '_' + idNivel[i]);
						}
						// setamos o id da tr
						tr.id = id;
						
						// Cor da TR
						if($('tarid').value == tarid){
							tr.style.background = '#ffffcc';
						} else {
				       		if(cor == "#fafafa") {
								tr.style.backgroundColor = "#f0f0f0";
								cor = "#f0f0f0";
							} else {
								tr.style.backgroundColor = "#fafafa";
								cor = "#fafafa";
							}
						}
						
						// Identa��o
			       		var espaco     = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
			       		var espacoTemp = "";
			
			       		for (y = 1; y < nivel; y++) {
			            	espacoTemp = espacoTemp + espaco;
			            }
			            
			            var seta = "";
			            if(espacoTemp){
			            	seta = "<img src=\"../imagens/seta_filho.gif\">";
			            }
			            
			            /**
			            * A��O
			            */
						td1 = tr.insertCell(0);
                		td1.style.textAlign = "center";
                		
                		if(tarpai == ""){
                			tarpai = tartarefa;
                		}
                		
                		td1.innerHTML = "<img src=\"../imagens/gif_inclui.gif\" onClick=\"window.location.href='tarefa.php?modulo=principal/cadAtividade&acao=A&tarpai="+tarid+"&tartarefa="+tartarefa+"'\" style=\"border:0; cursor:pointer;\" title=\"Incluir uma atividade a "+st_tarefa_atividade+"\">"
                		if(st_tarefa_atividade == 'Tarefa'){
                			td1.innerHTML += "&nbsp;<img src=\"../imagens/alterar.gif\" onClick=\"window.location.href='tarefa.php?modulo=principal/cadTarefa&acao=A&tarid="+tarid+"'\" style=\"border:0; cursor:pointer;\" title=\"Alterar "+st_tarefa_atividade+"\">"
                		} else {
                			td1.innerHTML += "&nbsp;<img src=\"../imagens/alterar.gif\" onClick=\"window.location.href='tarefa.php?modulo=principal/cadAtividade&acao=A&tarid="+tarid+"&tartarefa="+tartarefa+"&tarpai="+tarpai+"'\" style=\"border:0; cursor:pointer;\" title=\"Alterar "+st_tarefa_atividade+"\">"
                		}
                		
						td1.innerHTML += "&nbsp;<img src=\"../imagens/excluir.gif\" style=\"border:0; cursor:pointer;\" title=\"Excluir "+st_tarefa_atividade+"\" onClick=\"excluirTarefaAtividade('"+tarid+"');\" >";
						
						if(prioridade == 'U'){
							var imgPrioridade = '<img src=\'../imagens/pd_urgente.JPG\' /> Urgente';
						} else if(prioridade == 'A'){
							var imgPrioridade = '<img src=\'../imagens/pd_alta.JPG\' /> Alta';
						} else {
							var imgPrioridade = '<img src=\'../imagens/pd_normal.JPG\' /> Normal';						
						}
						
						tdPrioridade = tr.insertCell(1);						
						tdPrioridade.innerHTML = imgPrioridade;
						
						var imgAnexo = "";
						if(boAnexo){
							imgAnexo = "<img src=\"../imagens/anexo.gif\" onClick=\"window.location.href='tarefa.php?modulo=principal/cadDocumento&acao=A&tarid="+tarid+"'\" style=\"border:0; cursor:pointer;\" title=\"Anexo\">";
						}
						var imgRestricao = "";
						if(boRestricao){
							imgRestricao = "<img src=\"../imagens/restricao.png\" onclick=\"clicaAba($('td_nome_1'),'ajax.php','tipo=abaRestricao&tarid="+tarid+"')\" style=\"border:0; cursor:pointer;\" title=\"Restri��o\">";
						}
						var imgDepexterna = "";
						if(tardepexterna){
							imgDepexterna = "<img src=\"../imagens/botao_de.png\" title=\"Depend�ncia Externa\">";
						}
						
						/*
						* VERIFICA SE O LINK DEVE SER CARREGADO COM AJAX OU N�O, S� DEVER� USAR AJAX NO cadAcompanhamento
                		*/
                		if(boCarregaLinkAjax){
                			var onclick = "onClick=\"carregaCabecalhoAtendimento("+tarid+", this.parentNode.parentNode); return void(0);\"";
                		} else {
                			var onclick = "onClick=\"window.location.href='tarefa.php?modulo=principal/cadAcompanhamento&acao=A&tarid="+tarid+"';\"";
                		}
						td2 = tr.insertCell(2);						
                		if( boFilho ) {
							td2.innerHTML = espacoTemp+seta+"<a href=\"#\" onclick=\"alteraIconeArvoreAberta('"+tr.id+"');\">"+
											"<img id=\"img_"+tr.id+"\" src=\"../imagens/"+img+"\" border=\"0\"></a>&nbsp;&nbsp;"+imgAnexo+imgRestricao+" <a href=\"javascript: return void(0);\" title=\""+tartitulo+"\" "+onclick+" ><b>"+codTarefa+' - '+tartitulo+"</b></a> "+imgDepexterna;
						} else {
				            td2.innerHTML = espacoTemp+seta+imgAnexo+imgRestricao+" <a href=\"javascript: return void(0);\" title=\""+tartitulo+"\" "+onclick+" ><b>"+codTarefa+' - '+tartitulo+"</b></a> "+imgDepexterna;
				        }
				        
				        /**
						* RESPONS�VEL
						*/
				        td3 = tr.insertCell(3);
				        td3.style.color = "#1E90FF";
				        td3.style.cursor = "pointer";
						td3.innerHTML = "<span onclick=\"alteraResponsavel('td3_" +tarid + "')\" >"+setorRespon+' - '+nomeresponsavel+"</span>";
						td3.id = 'td3_'+tarid;
												
						/**
						* SITUA��O
						*/
						td4 = tr.insertCell(4);
						td4.style.cursor = "pointer";
						td4.setAttribute( 'align', 'center' );
						td4.id = 'td_'+tarid;
						array = barraExecucao.split('@@');
						td4.innerHTML = "<span onclick=\"posicionaSlider('td_" +tarid + "')\" >"+array[0]+"</span>";
						td4.status = array[1];
						td4.percentual = array[2];
						
						/**
						* PRAZO ATENDIMENTO
						*/
						td5 = tr.insertCell(5);
						td5.style.textAlign = "center";
						td5.style.cursor = "pointer";
						td5.style.color = "#008000";
						td5.title = "Alterar Prazo de Atendimento";
						td5.id = 'dataprazo_'+tarid;
						mostraDataPrazoFormatada(td5, dataPrazo, tarid);
						
						/**
						* ORDEM
						*/
						td6 = tr.insertCell(6);
						td6.setAttribute( 'align', 'center' );
						
						var desabilitadoC = "d";
						var linkC = "";
						var cursorC = "";
						
						var desabilitadoB = "d";
						var linkB = "";
						var cursorB = "";
							
						if(boCima){
							desabilitadoC = "";
							var linkC = "onclick=\"mudaPosicao('cima',this.parentNode.parentNode.rowIndex, '"+tarid+"', '"+_tartarefa+"', '', '', '"+arFiltros+"')\"";
							var cursorC = "style=\"cursor: pointer;\""; 
						}
						
						if(boBaixo){
							desabilitadoB = "";
							linkB = "onclick=\"mudaPosicao('baixo',this.parentNode.parentNode.rowIndex, '"+tarid+"', '"+_tartarefa+"', '', '', '"+arFiltros+"')\"";
							cursorB = "style=\"cursor: pointer;\""; 
						}
						
						td6.innerHTML = "&nbsp;<img "+linkB+" "+cursorB+" src=\"../imagens/seta_baixo"+desabilitadoB+".gif\" />";
						td6.innerHTML += "&nbsp;<img "+linkC+" "+cursorC+" src=\"../imagens/seta_cima"+desabilitadoC+".gif\" />";
					}
				}
			} else {
				tr = $('tabela_tarefa').insertRow(1);
				td1 = tr.insertCell(0);
				td1.colSpan = '6';
				td1.style.textAlign = "center";
				td1.style.color = "#FF0000";
				td1.innerHTML = "N�o foi encontrado nenhum registro.";
			}
		}
		
	});
	//$('aguarde_').hide();
	$('tabela_tarefa').setOpacity(1);
}


/*** (IN�CIO) MANIPULA ALTERA��O DA DATA DE IN�CIO/T�RMINO ***/
function montaCalendario(td) {
	removeSlider();
	removeComboResponsavel();
	
	var boPerfilGerente = verificaSeGerente();
	var tarid = $(td).id.replace('dataprazo_', '');
	/**
	* VALIDA��O DE DATA PRAZO ATENDIMENTO 
	*/
	var setorUsuarioLogado = recuperaSetorUsuarioLogado();
	var boExibeCalendario = true;
	var req = new Ajax.Request('ajax.php', {
	        method:     'post',
	        parameters: 'tipo=recuperaSetorOrigemSetorResponCpfResponPorTarid&tarid=' + tarid,
	        asynchronous: false,
	        onComplete: function (r)
	        {
	        	//$('teste').update(r.responseText);
				var obTarefa = r.responseText.evalJSON();
				if(!boPermissaoPrazo(boPerfilGerente, setorUsuarioLogado, obTarefa.unaidsetororigem, obTarefa.unaidsetorresponsavel)){
					alert('Usuario sem permiss�o para alterar a Situa��o');
					boExibeCalendario = false;
					return false;
				}
				
				 
	        }
	  });

	if(boExibeCalendario){
		var objInputGeral = document.getElementById('inputGeral');
		if( trim( $('span_data_'+tarid).innerHTML ) != '' ) {
			objInputGeral.value = $('span_data_'+tarid).innerHTML;
		} else {
			objInputGeral.value = '';
		}
		objInputGeral.parent = $(td).id;
		displayCalendar( objInputGeral, 'dd/mm/yyyy', $(td).parentNode.getElementsByTagName("td")[5] );
	}
}

function desmontaCalendario(objInputGeral) {
	if( !objInputGeral || objInputGeral.value == '' )
	{
		return;
	}
	var strSpanId = objInputGeral.parent;
	var id = strSpanId.substr( 'dataprazo_'.length );
	var strDataAntiga = $('span_data_'+id).innerHTML;
	var objSpan = document.getElementById( strSpanId );
	//var strDataAntiga = objSpan.innerHTML;
	if(verificaDataPaiEDataFilha(objInputGeral, id, '')){
		if( strSpanId.indexOf( 'datainicio_' ) == 0 ) {
			var id = strSpanId.substr( 'datainicio_'.length );
			strDataAlterada = 'mondatainicio';
		} else if( strSpanId.indexOf( 'dataprazo_' ) == 0 ) {
			var id = strSpanId.substr( 'dataprazo_'.length );
			strDataAlterada = 'tardataprazoatendimento';
		}
		
		//objSpan.innerHTML = '<img align="absmiddle" src="../imagens/wait.gif"/>';
		alteraData( id , strDataAlterada , objInputGeral.value , strDataAntiga, 'S');
	}
}

function aposAlterarDataPrazo( id , strDataAlterada , strNovaData ) {
	var objDate = strDateToObjDate( strNovaData , 'd/m/Y' , '/' );
	var objDataAtual = new Date();
	
	var objDataMaisQuatroDias = new Date();
	objDataMaisQuatroDias.setDate(objDataMaisQuatroDias.getDate() + 4);
	
	/**
	* FEITO ASSIM POR CAUSA DA PRESA
	*/
	if( strDataAlterada == 'tardataprazoatendimento' ){
		strSpanId = 'dataprazo_' + id;
		var objSpan = document.getElementById( strSpanId );
		
	
		var objDataAtualTemp = objDataAtual.getDate() + "/" + (objDataAtual.getMonth() + 1) + "/" + objDataAtual.getFullYear();
		var objDateTemp = objDate.getDate() + "/" + (objDate.getMonth() + 1) + "/" + objDate.getFullYear();
		var objDataMaisQuatroDias = objDataMaisQuatroDias.getDate() + "/" + (objDataMaisQuatroDias.getMonth() + 1) + "/" + objDataMaisQuatroDias.getFullYear();
		
		if( objDate <= objDataAtual ) {
			objSpan.style.color = '#ff2020';
			objSpan.style.fontWeight = 'bold';
		}else if( objDateTemp >= objDataAtualTemp && objDateTemp <= objDataMaisQuatroDias ) {
			objSpan.style.color = '#FFA500';
			objSpan.style.fontWeight = 'bold';
		} else {
			objSpan.style.color = "#008000";
			objSpan.style.fontWeight = '';
		}

		objSpan.innerHTML = "<span id='span_data_"+id+"' onclick=\"montaCalendario('dataprazo_" +id + "')\" >"+strNovaData+"</span>";
		//objSpan.innerHTML = strNovaData;
	}
	
	/**
	* SO CARREGA ATENDIMENTO SE ESTIVER NA TELA DE ATENDIMENTO
	*/
	if($('cadAcompanhamento') != null){
		carregaCabecalhoAtendimento(id, $(strSpanId).parentNode);
	}
}
/*** (FIM) MANIPULA ALTERA��O DA DATA DE IN�CIO/T�RMINO ***/


/*** (IN�CIO) MANIPULA ALTERA��O DA SITUA��O ***/
function posicionaSlider(td) {
	//closeCalendar();
	removeComboResponsavel();
	var boPerfilGerente = verificaSeGerente();
	
	var tarid = $(td).id.replace('td_', '');
	/**
	* VALIDA��O DA SITUA��O 
	*/
	var setorUsuarioLogado = recuperaSetorUsuarioLogado();
	var cpfUsuarioLogado   = recuperaCpfUsuarioLogado();
	var boExibeSituacao = true;
	var req = new Ajax.Request('ajax.php', {
	        method:     'post',
	        parameters: 'tipo=recuperaSetorOrigemSetorResponCpfResponPorTarid&tarid=' + tarid,
	        asynchronous: false,
	        onComplete: function (r)
	        {
				var obTarefa = r.responseText.evalJSON();
				if(!boPermissaoSituacao(boPerfilGerente, setorUsuarioLogado, obTarefa.unaidsetororigem, obTarefa.unaidsetorresponsavel, cpfUsuarioLogado, obTarefa.usucpfresponsavel)){
					alert('Usuario sem permiss�o para alterar a Situa��o');
					boExibeSituacao = false;
					return false;
				}				
	        }
	  });
	
	if(boExibeSituacao){
		var left = getleftPos($(td))+'px';
		var top  = getTopPos($(td))+'px';
		
		var objSlider 		= document.getElementById('sliderDiv');
		var objSliderValor 	= document.getElementById('valorSlider');
		var objSliderStatus = document.getElementById('situacaoSlider');
			
		var intValor 		= $(td).percentual;
		var intSelectValue 	= $(td).status;
		var strIdSpan 		= $(td).id;
		
		objSlider.style.position = "absolute";
		objSlider.style.left = left;
		objSlider.style.top = top;
		objSlider.style.display = "block";
		
		objSliderValor.value = intValor;
		objSliderStatus.value = intSelectValue;	
		objSliderStatus.status = intSelectValue;
		objSliderStatus.id_tarefa = strIdSpan;
		objSliderValor.onchange();
	}
}

function alteraResponsavel(td) {
	try	{
		closeCalendar();
		removeSlider();
	}
	catch(e) {}
	var boPerfilGerente = verificaSeGerente();
	
	var left = getleftPos($(td))+'px';
	var top  = getTopPos($(td))+'px';
	
	var objDivResp = document.getElementById('comboDiv');
	$('idComboResp').value = $(td).id;
	var tarid = $('idComboResp').value.replace('td3_', '');
	
	/**
	* VALIDA��O DE SETOR RESPONS�VEL E USU�RIO RESPONS�VEL 
	*/
	var setorUsuarioLogado = recuperaSetorUsuarioLogado();
	var boExibeComboResp = true;
	var req = new Ajax.Request('ajax.php', {
	        method:     'post',
	        parameters: 'tipo=recuperaSetorOrigemSetorResponCpfResponPorTarid&tarid=' + tarid,
	        asynchronous: false,
	        onComplete: function (r)
	        {
				//$('teste').update(r.responseText);
				var obTarefa = r.responseText.evalJSON();
				if(!boPermissaoPessoaRespon( boPerfilGerente, setorUsuarioLogado, obTarefa.unaidsetorresponsavel )){
					alert('Usuario sem permiss�o para alterar o Respons�vel');
					boExibeComboResp = false;
					return false;	
				}
				
	        }
	  });

	if(boExibeComboResp){
		if(objDivResp.style.display == "block"){
			removeComboResponsavel();
		} else {
			td 	   = document.getElementById('td_usucpfresponsavel');
			var req = new Ajax.Request('ajax.php', {
									        method:     'post',
									        parameters: 'tipo=recuperaResponsavelPorTarid&tarid=' + tarid,
									        asynchronous: false,
									        onComplete: function (r)
									        {
									        	//$('teste').update(r.responseText);
												td.innerHTML = r.responseText;
												//td.style.visibility = 'visible';
									        }
									  });
			objDivResp.style.position = "absolute";
			objDivResp.style.left = left;
			objDivResp.style.top = top;
			objDivResp.style.display = "block";
		}
	}
}


function aposAlterarResponsavel(usucpfresponsavel, boMontaShowModal){
	if(usucpfresponsavel != ''){
		/*if(boMontaShowModal != 'N'){
			var funcaoParametros = 'aposAlterarResponsavel("'+usucpfresponsavel+'", "N" );'
			montaShowModal(funcaoParametros);
			return false;
		}*/
		var idTd = $('idComboResp').value;
		var id = idTd.replace('td3_', '');
		var data = 'tipo=atualizaResponsavel&tarid=' + id +'&usucpfresponsavel='+usucpfresponsavel;
		var aj = new Ajax.Request('ajax.php',  
		{  
			method: 'post',   
			parameters: data,   
			onComplete: function(r)
			{
				//$('teste').update(r.responseText);
				if(r.responseText){
					$(idTd).update("<span onclick=\"alteraResponsavel('td3_" +id + "')\" >"+r.responseText+"</span>");
				} else {
					alert("Erro ao atualizar a Respons�vel.");
				}
			}
		});
		removeComboResponsavel();
		
		/**
		* SO CARREGA ATENDIMENTO SE ESTIVER NA TELA DE ATENDIMENTO
		*/
		if($('cadAcompanhamento') != null){
			carregaCabecalhoAtendimento(id, $(idTd).parentNode);	
		}
	} else {
		removeComboResponsavel();
	}
}

function removeComboResponsavel(){
	$('idComboResp').value = "";
	$('comboDiv').style.display = "none";
}

function removeSlider() {
	var objSlider = document.getElementById('sliderDiv');
	objSlider.style.display = "none";
}

function slicerSubmit() {
	var objSliderValor	= document.getElementById( 'valorSlider' );
	var objSliderStatus	= document.getElementById( 'situacaoSlider' );
	var strIdSpan		= objSliderStatus.id_tarefa;
	var objSpan			= document.getElementById( strIdSpan );
	
	var strStatus		= document.getElementById( "situacaoSlider" ).options[ objSliderStatus.value - 1 ].innerHTML;
	var intPercentual	= objSliderValor.value;
	
	atualizaBarraStatus( strIdSpan , strStatus , objSliderStatus.value  , intPercentual, 'S' )
	removeSlider();
}

function aposAtualizarBarraStatus(intBarraStatusId, strStatus, intStatus, intPercentual) {
	if(window.arrSituacoes == undefined)
	{
		var arrSituacoes 	= Array();
		
		// Status: 'N�o iniciado'
		var arrSituacao		= new Object();
		arrSituacao.texto	= '#909090';
		arrSituacao.barra	= '#bbbbbb';
		arrSituacao.sombra	= '#efefef';
		arrSituacoes[1] 	= arrSituacao;
		
		// Status: 'Em andamento'
		var arrSituacao		= new Object();
		arrSituacao.texto	= '#209020';
		arrSituacao.barra	= '#339933';
		arrSituacao.sombra	= '#dcffdc';
		arrSituacoes[2] 	= arrSituacao;
		
		// Status: 'Suspenso'
		var arrSituacao		= new Object();
		arrSituacao.texto	= '#aa9020';
		arrSituacao.barra	= '#bba131';
		arrSituacao.sombra	= '#feffbf';
		arrSituacoes[3] 	= arrSituacao;
		
		// Status: 'Cancelado'
		var arrSituacao		= new Object();
		arrSituacao.texto	= '#aa2020';
		arrSituacao.barra	= '#cc3333';
		arrSituacao.sombra	= '#ffe7e7';
		arrSituacoes[4] 	= arrSituacao;
		
		// Status: 'Conclu�do'
		var arrSituacao		= new Object();
		arrSituacao.texto	= '#2020aa';
		arrSituacao.barra	= '#3333cc';
		arrSituacao.sombra	= '#d4e7ff';
		arrSituacoes[5] 	= arrSituacao;
		
		window.arrSituacoes = arrSituacoes;
	}
			
	arrSituacaoAtual = window.arrSituacoes[ intStatus ];
	
	var strNewSpanInnerHTML = '' +
	'<span style="color: '+ arrSituacaoAtual.texto + ';font-size: 10px;">' + strStatus + '</span>' +
	'<div style="text-align: left; margin-left: 5px; padding: 1px 0 1px 0; ' + 
	'height: 6px; max-height: 6px; width: 75px; border: 1px solid #888888; ' +
	'background-color: ' + arrSituacaoAtual.sombra  + ';" title="' + intPercentual + '%">' +
		'<div style="font-size:4px;width: ' + intPercentual + '%; height: 6px; max-height: 6px; background-color: ' + arrSituacaoAtual.barra + ';">' +
		'</div>' + 
	'</div>';
	
	var objSpan = document.getElementById( intBarraStatusId );
	
	objSpan.status = intStatus;
	objSpan.percentual = intPercentual;

	objSpan.innerHTML = strNewSpanInnerHTML;
	
	/**
	* SO CARREGA ATENDIMENTO SE ESTIVER NA TELA DE ATENDIMENTO
	*/
	if($('cadAcompanhamento').value){
		var id = intBarraStatusId.replace('td_', '');
		carregaCabecalhoAtendimento(id, $(intBarraStatusId).parentNode);
	}
}

function alteraStatus(objSliderStatus) {
	var objSliderValor = document.getElementById('valorSlider');
	 
	switch('' + objSliderStatus.value)
	{
		case '1':
		{
			objSliderValor.value = 0;
			break;
		}
		case '2':
		case '3':
		case '4':
		{
			switch( '' + objSliderValor.value )
			{
				case '100':
				{
					objSliderValor.value = 90;
					break;
				}
				default:
				{
					break;
				}
			}
			break;
		}
		case '5':
		{
			objSliderValor.value = 100;
			break;
		}
		default:
		{
			break;
		}
	}
	
	objSliderValor.onchange();
}
		
function arredonda(objInput) {
	if( objInput.value % 10 != 0 )objInput.value -= objInput.value % 10;
	
	var objSliderStatus = document.getElementById('situacaoSlider');
			
	var intOriginalStatus = objSliderStatus.status;
	
	switch( '' + objInput.value )
	{
		case '100':
		{
			objSliderStatus.value = 5;
			break;
		}
		case '0':
		{
			switch( '' + objSliderStatus.value )
			{
				case '5':
				{
					switch( intOriginalStatus )
					{
						case '5':
						{
							objSliderStatus.value = 2;
						}
						default:
						{
							objSliderStatus.value = intOriginalStatus;
							break;
						}
					}
					break;
				}
			}
			break;
		}
		default:
		{
			switch( '' + objSliderStatus.value )
			{
				case '5':
				case '1':
				{
					if( ( intOriginalStatus == 5 ) || ( intOriginalStatus == 1 ) )
					{ 
						objSliderStatus.value = 2;
					}
					else
					{
						objSliderStatus.value = intOriginalStatus;
					}
					break;
				}
				default:
				{
					break;
				}
			}
			break;
		}
	}
}
/*** (FIM) MANIPULA ALTERA��O DA SITUA��O ***/

/*** INICIO MANIPULA COLUNAS DA TABELA ***/
function mudaPosicao(tipo,rowIndex, id, tartarefa, trId, tarAberto, arFiltros){
	var tabela = document.getElementById('tabela_tarefa');
	maxRows = tabela.rows.length - 1;
	
	/**
	* VALIDA��O DA ORDEM
	*/
	var boPerfilGerente = verificaSeGerente();
	var setorUsuarioLogado = recuperaSetorUsuarioLogado();
	var boMudaPosicao = true;
	var req = new Ajax.Request('ajax.php', {
	        method:     'post',
	        parameters: 'tipo=recuperaSetorOrigemSetorResponCpfResponPorTarid&tarid='+id,
	        asynchronous: false,
	        onComplete: function (r)
	        {
	        	//$('teste').update(r.responseText);
				var obTarefa = r.responseText.evalJSON();
				if(!boPermissaoOrdem(boPerfilGerente, setorUsuarioLogado, obTarefa.unaidsetororigem, obTarefa.unaidsetorresponsavel)){
					alert('Usuario sem permiss�o para mudar a Posi��o da Tarefa / Atividade');
					boMudaPosicao = false;
					return false;
				}
				
				 
	        }
	  });

	if(boMudaPosicao){
		if(tipo == "baixo" && rowIndex != maxRows){
			var tr1 =  tabela.rows[rowIndex];
			var tr2 =  tabela.rows[rowIndex + 1];
			
			//var tr1_html =  tabela.rows[rowIndex].innerHTML;
			//var tr2_html =  tabela.rows[rowIndex + 1].innerHTML;
			
			//Pegando id 1
			var id1 = tr1.id;
			//Pegando id 2
			var id2 = tr2.id;
			
			/**
			* ARRAY DE IDs (SEPARADO COM _)
			*/
			var arId1 = "";
			var arId2 = "";
			
			/**
			* COUNT DOS ARRAY DE IDs
			*/
			var countArId1 = 0;
			var countArId2 = 0;
			
			arId1 = id1.split('_');
			countArId1 = arId1.length - 1;
			id1 = arId1[countArId1];
		
			arId2 = id2.split('_');
			countArId2 = arId2.length - 1;
			id2 = arId2[countArId2];
			
			var i=1;
			while (countArId1 != countArId2) {
				tr2 =  tabela.rows[rowIndex + i];
				id2 = tr2.id;
				arId2 = id2.split('_');
				countArId2 = arId2.length - 1;
				id2 = arId2[countArId2];
			  	i++;
			}
			
			var data = 'tipo=mudaPosicaoAjax&tarid1=' + id1 + '&tarid2=' + id2;
			var aj = new Ajax.Request('ajax.php',  
			{  
				method: 'post',   
				parameters: data,
				asynchronous: false,
				onLoading: $('tabela_tarefa').setOpacity(0.5),
				onComplete: function(r)
				{	
					//$('teste').update(r.responseText);
					if($('cadAcompanhamento') != null){
						montaArvoreAberta(tartarefa,arFiltros,1);
					} else {
						montaPai('',arFiltros);
					}
					
				}
			});
			$('tabela_tarefa').setOpacity(1);
			
		}
		if(tipo == "cima"  && rowIndex != 1){
			var tr1 =  tabela.rows[rowIndex];
			var tr2 =  tabela.rows[rowIndex - 1];
			
			//var tr1_html =  tabela.rows[rowIndex].innerHTML;
			//var tr2_html =  tabela.rows[rowIndex - 1].innerHTML;
			
			//Pegando id 1
			var id1 = tr1.id;
			//Pegando id 2
			var id2 = tr2.id;
			
			/**
			* ARRAY DE IDs (SEPARADO COM _)
			*/
			var arId1 = "";
			var arId2 = "";
			
			/**
			* COUNT DOS ARRAY DE IDs
			*/
			var countArId1 = 0;
			var countArId2 = 0;
			
			arId1 = id1.split('_');
			countArId1 = arId1.length - 1;
			id1 = arId1[countArId1];
		
			arId2 = id2.split('_');
			countArId2 = arId2.length - 1;
			id2 = arId2[countArId2];
			
			var i=1;
			while (countArId1 != countArId2) {
				tr2 =  tabela.rows[rowIndex - i];
				id2 = tr2.id;
				arId2 = id2.split('_');
				countArId2 = arId2.length - 1;
				id2 = arId2[countArId2];
			  	i++;
			}
			
			var data = 'tipo=mudaPosicaoAjax&tarid1=' + id1 + '&tarid2=' + id2;
			var aj = new Ajax.Request('ajax.php',  
			{  
				method: 'post',   
				parameters: data,
				asynchronous: false,
				onComplete: function(r)
				{
					if($('cadAcompanhamento') != null){
						montaArvoreAberta(tartarefa,arFiltros,1);
					} else {
						montaPai('',arFiltros);
					}
				}
			});
		}
	}
}
/*** FIM MANIPULA COLUNAS DA TABELA ***/


/*** INICIO SHOW MODAL ***/
var countModal = 1;

function montaShowModal(funcaoParametros) {
	//var funcaoTemp  = funcaoParametros.slice(0,22);
	var funcaoTemp = funcaoParametros.slice(0,10);
	
	var boGravaCheck  = "";
	var checkBoxEmail = ""
	
	//if (funcaoTemp == "aposAlterarResponsavel" || funcaoTemp2 == "alteraData"){
	if (funcaoTemp == "alteraData"){
		boGravaCheck = 1;
		checkBoxEmail = "<input type=\"checkbox\" id=\"checkEmail\" title=\"Enviar Email\" name=\"checkEmail\" value=\"1\" align=\"bottom\"> Desejar enviar email para o respons�vel? ";
	}
	
	var campoTextArea = '<form id="form" name="form"><div class="notprint">'+
			'<textarea class="txareaclsMouseOver" id="acodsc2'+countModal+'" name="acodsc2'+countModal+'" cols="80" rows="8" title="Mensagem" '+ 
				'onmouseover="MouseOver( this );" '+
				'onfocus="MouseClick( this );" '+
				'onmouseout="MouseOut( this );" '+
				'onblur="MouseBlur( this ); '+
				'textCounter( this.form.acodsc2'+countModal+', this.form.no_acodsc2, 1500);" '+ 
				'style="width: 80ex;" '+
				'onkeydown="textCounter( this.form.acodsc2'+countModal+', this.form.no_acodsc2, 1500 );" '+ 
				'onkeyup="textCounter( this.form.acodsc2'+countModal+', this.form.no_acodsc2, 1500);">'+
			'</textarea><br> '+
			'<input readonly="readonly" style="border-left: 3px solid rgb(136, 136, 136); text-align: right; color: rgb(128, 128, 128);" '+ 
				'name="no_acodsc2" size="6" maxlength="6" value="1500" '+
				'class="CampoEstilo" type="text"> '+
			'<font size="1" color="red" face="Verdana"> m�ximo de caracteres</font> '+
		'</div><div id="print_acodsc2" class="notscreen" style="text-align: left;"></div>'+
		checkBoxEmail+
		'</form>';
	var alertaDisplay = '<center><div class="titulo_box" >� necess�rio Justificar a altera��o.<br/ >'+campoTextArea+'</div><div class="links_box" ><br><input type="button" onclick=\'gravaMensagemSessao('+boGravaCheck+'); '+funcaoParametros+' closeMessage(); \' value="Gravar" /> <input type="button" onclick=\'closeMessage(); return false \' value="Cancelar" /></center>';
	displayStaticMessage(alertaDisplay,false);
	return false;
}

function displayStaticMessage(messageContent,cssClass) {
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	messageObj.setHtmlContent(messageContent);
	messageObj.setSize(420,215);
	messageObj.setCssClassMessageBox(cssClass);
	messageObj.setSource(false);	// no html source since we want to use a static message here.
	messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
	messageObj.display();
}

function closeMessage() {
	messageObj.close();	
}

function gravaMensagemSessao(boGravaCheck){
	var boEnviarEmailRespon = false;
	if(boGravaCheck){
		if($F('checkEmail') != null){
			boEnviarEmailRespon = true;
		}
	}

	var mensagem = $F('acodsc2'+countModal);
	countModal++;
	var data = 'tipo=gravaMensagemSessao&mensagem='+mensagem+'&boEnviarEmailRespon='+boEnviarEmailRespon;
			var aj = new Ajax.Request('ajax.php',  
			{  
				method: 'post',   
				parameters: data,   
				onComplete: function(r){
					//$('teste').update(r.responseText);
				}
			});
}
/*** FIM SHOW MODAL ***/

function filtroPesquisa(_tartarefa, boCadAcompanhamento){
	if(boCadAcompanhamento){
		montaArvoreAberta(_tartarefa,'',1);
	} else {
		var filtrotarid      			= $('filtrotarid').value;
		var filtrotartitulo      		= $('filtrotartitulo').value;
		var filtrosidoc					= $('filtrosidoc').value;
		var filtrounaidsetororigem      = $('filtrounaidsetororigem').value;
		var filtrounaidsetorresponsavel = $('filtrounaidsetorresponsavel').value;
		var filtrousucpfresponsavel 	= $('filtrousucpfresponsavel').value;
		var filtroprazoini 				= $('filtroprazoini').value;
		var filtroprazofim 				= $('filtroprazofim').value;
		
		var marcados = '';
		var filtrosituacao = document.getElementsByTagName("INPUT");  
		for (i = 0; i < filtrosituacao.length; i++) {
			var situacao = filtrosituacao[i];  
		    if (situacao.type == "checkbox" && situacao.checked) {
		    	marcados += situacao.value+'. '; 
		   	}  
		}
		
		var arFiltros=new Array();
		arFiltros[0] = filtrotarid;
		arFiltros[1] = filtrotartitulo;
		arFiltros[2] = filtrounaidsetororigem;
		arFiltros[3] = filtrounaidsetorresponsavel;
		arFiltros[4] = filtrousucpfresponsavel;
		arFiltros[5] = filtrosidoc;
		arFiltros[6] = marcados;
		arFiltros[7] = filtroprazoini;
		arFiltros[8] = filtroprazofim;
		
		var data = $('formulario').serialize(true);

		var ajax = new Ajax.Request('ajax.php', {
			        method:     'post',
			        parameters: data,
			        onComplete: function (res)
			        {
						//$('teste').innerHTML = res.responseText;
			        }
			  });
		montaPai(_tartarefa,arFiltros,'');
	}
	
}

function montaListaAnexo(_tartarefa) {
	$('aguarde').hide();
	var data = 'tipo=montaListaAnexo&_tartarefa='+_tartarefa;
	var cor = "#f0f0f0";
	var aj = new Ajax.Request('ajax.php',  
	{
		method: 'post',
		asynchronous: false,
		parameters: data,
		//onLoading: $('aguarde_').show(),
		//onLoading: $('tabela_anexo').setOpacity(0.3),
		onComplete: function(r)
		{
			$('div_listaDocumento').update(r.responseText)
			//$('teste').update(r.responseText)
			return false;
		}
		
	});
	//$('aguarde_').hide();
	//$('tabela_anexo').setOpacity(1);
}

///////////////////////////// VALIDA��ES ATENDIMENTO ////////////////////////////////////////
/**
* SETOR RESPONS�VEL
*/
function boPermissaoSetorRespon(boPerfilGerente, setorUsuarioLogado, setororigem, setorresponsavel){
	if(verificaSeSuperUsuario()){
		return true;
	}
	if(boPerfilGerente == 'S' && (setorUsuarioLogado == setororigem || setorUsuarioLogado == setorresponsavel) ){
		return true;
	}
	return false;
}

/**
* PESSOA RESPONS�VEL
*/
function boPermissaoPessoaRespon(boPerfilGerente, setorUsuarioLogado, setorresponsavel){
	if(verificaSeSuperUsuario()){
		return true;
	}
	if(boPerfilGerente == 'S' && (setorUsuarioLogado == setorresponsavel) ){
		return true;
	}
	return false;
}

/**
* VALIDA��O DE SITUA��O
*/
function boPermissaoSituacao(boPerfilGerente, setorUsuarioLogado, setororigem, setorresponsavel, cpfUsuarioLogado, cpfResponAnterior){
	if(verificaSeSuperUsuario()){
		return true;
	}
	if((boPerfilGerente == 'S' && setorUsuarioLogado == setororigem) || (boPerfilGerente == 'S' &&  setorUsuarioLogado == setorresponsavel) || (boPerfilGerente == 'N' &&  cpfUsuarioLogado == cpfResponAnterior)){
		return true;
	}
	return false;
}

/**
* PRAZO
*/
function boPermissaoPrazo(boPerfilGerente, setorUsuarioLogado, setororigem, setorresponsavel){
	if(verificaSeSuperUsuario()){
		return true;
	}
	if(boPerfilGerente == 'S' && (setorUsuarioLogado == setororigem || setorUsuarioLogado == setorresponsavel) ){
		return true;
	}
	return false;
}

/**
* ORDEM
*/
function boPermissaoOrdem(boPerfilGerente, setorUsuarioLogado, setororigem, setorresponsavel){
	if(verificaSeSuperUsuario()){
		return true;
	}
	if(boPerfilGerente == 'S' && (setorUsuarioLogado == setororigem || setorUsuarioLogado == setorresponsavel) ){
		return true;
	}
	return false;
}

/**
* PESSOA DEPENDENCIA EXTERNA
*/
function boPermissaoDependencia(boPerfilGerente, setorUsuarioLogado, setororigem, setorresponsavel){
	if(verificaSeSuperUsuario()){
		return true;
	}
	if((boPerfilGerente == 'S' && setorUsuarioLogado == setororigem) || setorUsuarioLogado == setorresponsavel){
		return true;
	}
	return false;
}
///////////////////////////// VALIDA��ES ATENDIMENTO ////////////////////////////////////////
function verificaDataPaiEDataFilha(objData, tarid, boCadastro){
	var dataAntiga = objData.value;
	/**
	* VERIFICA SE � ATIVIDADE OU TAREFA / RECUPERA DATA DA TAREFA PAI
	*/
	
	if(!tarid) {
		//alert comentado por, ainda, n�o ter achado a l�gica dele... 
		//alert('Tarefa n�o enviada.');
		return false;
	}
	
	var data = 'tipo=verificaDataPaiEDataFilha&tarid='+tarid+'&dataPrazoAtual='+objData.value;
	var boAtividade = false; 
	var dataPai = "";
	var boDataFilhaMaior = false;
	var dataprazoAnterior = '';
	var aj = new Ajax.Request('ajax.php',  
	{  
		method: 'post',
		parameters: data,
		asynchronous: false,
		onLoading: $('aguarde_').show(),
		onComplete: function(r){
			//$('teste').update(r.responseText);
			var obTarefa = r.responseText.evalJSON();
			dataPai     = obTarefa.tardataprazoatendimento;
			boAtividade = obTarefa.boAtividade;
			boDataFilhaMaior = obTarefa.boDataFilhaMaior;
			dataprazoAnterior = obTarefa.dataprazoAnterior
		}
	});
	
	$('aguarde_').hide();
	if(boAtividade){
		var data1 = objData.value;
		var data2 = dataPai;
		data1 = parseInt( data1.split( "/" )[2].toString() + data1.split( "/" )[1].toString() + data1.split( "/" )[0].toString() );
		data2 = parseInt( data2.split( "/" )[2].toString() + data2.split( "/" )[1].toString() + data2.split( "/" )[0].toString() );
		
		if ( data1 > data2 ) {
		    alert('A Data do Prazo de Atendimento n�o pode ser maior que a Data do Prazo de Atendimento da Tarefa Pai');
		    if(boCadastro){
		    	objData.value = dataprazoAnterior;
		    	objData.focus();
		    }
			return false;
		}
	}
	if(boDataFilhaMaior){
	    alert('Existem atividades vinculadas a esta Tarefa / Atividade com data superior a '+objData.value);
	    if(boCadastro){
	    	objData.value = dataprazoAnterior;
	    	objData.focus();
	    }
		return false;			
	}
	return true;
}