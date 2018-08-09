function Conteudo (){

	var d = document;
	var controleSpancao;
	var indNivel = 0;
	// Atributo guarda os elementos da tabela.
	var elemento = new Array();								
	var maxHeight=false;
	
	this.debug = function (obj){  
	       var janela = window.open()
	       for(prop in obj){
	         janela.document.write(prop + ' = '+ obj[prop]+'<BR>');
	       }
	   
	 }

	function findPosY(obj){
		var curtop = 0;
	    if(obj.offsetParent)
	        while(1)
	        {
	          curtop += obj.offsetTop;
	          if(!obj.offsetParent)
	            break;
	          obj = obj.offsetParent;
	        }
	    else if(obj.y)
	        curtop += obj.y;
	    return curtop;
	}

	this.setMaxHeight = function ( height ){
		maxHeight = height;
	}
	
	this.conteudoCarregado = function (){
		d.getElementById('temporizador').style.display = 'none';
	}
	
	this.retornaElemento = function (){
		return elemento;
	}
	
	this.conteudoCarregando = function (id){
		var div;
		var h;
		var w;
		var topImg;
		
		h = d.body.scrollHeight;
		w = d.body.scrollWidth;

		elementRefe = d.getElementById(id);
		h = h < screen.height ? screen.height : h;
		
		if (elementRefe){
			topImg = findPosY(elementRefe);
		}else{
			topImg = (h/4);
		}

		if (!d.getElementById('temporizador')){
			div = d.createElement("div");
			div.setAttribute('id', 'temporizador');
		}else{
			div = d.getElementById('temporizador');		
		}

		// Monta Imagem

		if (div.getElementsByTagName('img').length == 0){
			img = d.createElement("img");
		}else{
			img = div.getElementsByTagName('img')[0];
		}

		img.setAttribute('src', '/imagens/carregando.gif');
		img.style.cssText = 'position:relative; top:' + topImg + 'px;';
		div.appendChild(img);
		
		d.body.appendChild(div);
		d.getElementById('temporizador').style.cssText = '-moz-opacity:0.8; filter: alpha(opacity=80); background:#ffffff; text-align:center; position:absolute; top:0px; left:0px; width:' + w + 'px; height:' + h + 'px; z-index:1000;';
		return true;
	}	

	this.carregaElemento = function (arrParent, id, visivel, profundidade)
							{
								try{							
									var elementoNivel = new Array();
									var ind;
									
									// Verifica a profundidade 
									if (profundidade == 0){
										// Cria o elemento de primeiro n?vel
										elemento[indNivel] 			   = new Array();
										elemento[indNivel]['id'] 	   = id;
										elemento[indNivel]['visivel']  = visivel;
										elemento[indNivel]['elemento'] = new Array();
										
										indNivel++;								
									}else{
										elementoNivel = elemento;
										for (i=0; i < arrParent.length; i++){
											for(a=0; a < elementoNivel.length; a++){
												if (arrParent[i].id == elementoNivel[a].id){
													elementoNivel = elementoNivel[a]['elemento'];
													a = 0;
													break;
												}
											}
										}
										ind = elementoNivel.length == 0 ? 0 : elementoNivel.length;

										elementoNivel[ind] = new Array();
										elementoNivel[ind]['id'] 	   = id;
										elementoNivel[ind]['visivel']  = visivel;
										elementoNivel[ind]['elemento'] = new Array();
									}	
								}catch(err){
									alert('erro no mapeamento da tabela!');
								}
								return;
							}

	this.controle = function(id, idPaiTx, imgId)
					{
						imgObj = d.getElementById(imgId);
						if( !imgObj ){
							alert(imgId);
						}
						//this.conteudoCarregando(id);
						var elementoId;	
						var visibilidade   = '';
						var visibilidadeId = '';
						var idPai 		   = new Array();
						
						// Transforma string com pais em array.		
						if ((idPaiTx.indexOf(':') > -1)){
							idPai = idPaiTx.split(':');	
						}else if (idPaiTx){
							idPai.push(idPaiTx);
						}
						elementoId = elemento;						
						// Verifica se o elemento est? no primeiro n?vel.
						if (idPaiTx != id){
							// Desce em n?veis, at? chegar no elemento.
							for (i=0; i < idPai.length; i++){
								for(a=0; a < elementoId.length; a++){
									//alert(elementoId[a].id);
									if (idPai[i] == elementoId[a].id){
										elementoId = elementoId[a]['elemento'];
										a = 0;
										break;
									}
								}
							}
							elementoId = elementoId;
						}

						for (z=0; z < elementoId.length; z++){
							if (elementoId[z].id == id){
								elementoId = elementoId[z];
								break;
							}
						}
						
						visibilidadeId = elementoId.visivel;
						
//						console.log(imgId);
//						console.log(visibilidadeId);
//						console.log(imgObj.className);
						
						// Modifica a imagem e seta o atributo de visibilidade.
						if (visibilidadeId == 'S'){
							imgObj.className   = 'btn btn-primary btn-xs glyphicon glyphicon-plus';
//							imgObj.src 		   = '/imagens/mais.gif';
							imgObj.title 	   = 'Clique para expandir';		
							elementoId.visivel = 'N';
						}else{
							imgObj.className   = 'btn btn-primary btn-xs glyphicon glyphicon-minus';
//							imgObj.src 	 	   = '/imagens/menos.gif';
							imgObj.title 	   = 'Clique para minimizar';		
							elementoId.visivel = 'S';		
						}
						// Faz maximiza??o ou minimiza??o dos elementos
						controleSpancao(elementoId, elementoId.visivel);
						this.conteudoCarregado();
					}
					
	controleSpancao = function (elementoId, visibilidade)
					  {
						var elementoAtual;
						var elementoAtualHtml;
						var display;
												
						// Seta ao atributo o valor de maximiza??o ou minimiza??o 
						display = visibilidade == 'N' ? 'none' : ( navigator.appName.indexOf('Explorer') > -1 ? 'block' : 'table-row');
						
						// Varre nos elementos filhos	
						for (var i in elementoId.elemento){						
							elementoAtual = elementoId.elemento[i];
							if (!elementoAtual.id){
								continue;
							}
							elementoAtualHtml 				= d.getElementById(elementoAtual.id);
							elementoAtualHtml.style.display = display;
							
							// Varre os elementos filhos, aplicando a recursividade.
							for (var a in elementoAtual.elemento){	
								if (!elementoAtual.elemento[a].id){
									continue;
								}
								
								if ( elementoId.elemento[i].visivel == 'S' ){
									elementoAtualHtml 				= d.getElementById(elementoAtual.elemento[a].id);
									elementoAtualHtml.style.display = display;
								}
									  
								if (elementoAtual.elemento[a].visivel == 'S' && ((visibilidade == 'N' && elementoId.elemento[i].visivel == 'S') || (visibilidade == 'S' && elementoId.elemento[i].visivel == 'S')) ){
									controleSpancao(elementoAtual.elemento[a], visibilidade);									
								}
							}
						} 
						return;							
					  }

	this.ajustarBarraDeRolagem = function (){
		jQuery('[id*=divListaRelatorio_]').each(function (){
			var divExterna		= this;
			var table 			= jQuery('table:first', this);
			var heightCabecalho = table.find('thead').height();
			
			if ( jQuery(this).find('#divCabecalho').length == 0 ){
				var tableClone 		= table.clone().css('margin-bottom', '0px');
				var divCabecalho	= jQuery('<div></div>').attr('id', 'divCabecalho').append( tableClone.clone() );
					divCabecalho.find('tbody, tfoot').remove();
					
				var divCorpo		= jQuery('<div></div>').attr('id', 'divCorpo').append( tableClone.clone().css({'margin-top':'-' + heightCabecalho + 'px'}) ).css({'overflow-y':'scroll', 'max-height': maxHeight + 'px'});
					divCorpo.find('tfoot').remove();
					
				var divRodape		= jQuery('<div></div>').attr('id', 'divRodape').append( tableClone.clone().css({'margin-top':'-' + heightCabecalho + 'px'}) ).css({'overflow-y':'hidden', 'height':'auto'});
					divRodape.find('tbody').remove();
			
				jQuery('table:first', this).remove();
				jQuery(this).append(divCabecalho, divCorpo, divRodape);
			}

			table = jQuery('#divCorpo table', divExterna);
			table.find('tbody tr:first td').each(function (i, o){
				var width = jQuery(o).width();
				
				if ( jQuery(o).is(':last-child') ){
					jQuery('#divCabecalho table:first thead td', divExterna).eq(i).width( (width + 16) );
					jQuery('#divCorpo table:first thead td', divExterna).eq(i).width( width );
					jQuery('#divRodape table:first thead td', divExterna).eq(i).width( (width + 16) );
					jQuery('#divRodape table:first tfoot tr:first td', divExterna).eq(i).css('padding-right', '25px');
				}else{
					jQuery('#divCabecalho table:first thead td', divExterna).eq(i).width( width );
					jQuery('#divCorpo table:first thead td', divExterna).eq(i).width( width );
					jQuery('#divRodape table:first thead td', divExterna).eq(i).width( width );
				}			
			});
		});		
	}
}
/*
function Conteudo (){

	var d = document;
	var controleSpancao;
	// Atributo guarda os elementos da tabela.
	elemento = new Array();								

	this.carregaElemento = function (arrParent, id, visivel, profundidade)
							{
								try{							
									var txObj    = '';
									var tx		 = '';
									var txObjNew = '';
									var ponto    = '';
									
									// Verifica a profundidade 
									if (profundidade == 0){
										// Cria o elemento de primeiro n?vel
										txObj = 'elemento[\'' + id + '\'] = new Array();' + 
												'elemento[\'' + id + '\'][\'id\'] 		= \'' + id + '\';' + 
												'elemento[\'' + id + '\'][\'visivel\']  = \'' + visivel + '\';' +
												'elemento[\'' + id + '\'][\'elemento\'] = new Array();';
									}else{
										tx += 'elemento[\'' + arrParent[0].id + '\']';
										// Desce os n?veis, para adicionar o novo elemento
										for (i=1; i < arrParent.length; i++){
											idParent 	= arrParent[i].id;
											idProParent = arrParent[i+1] ? arrParent[i+1].id : 1; 
											tx += '[\'elemento\'][\'' + idParent + '\']';
										}
										// Adiciona o novo elemento
										txObj += tx + '[\'elemento\'][\'' + id + '\'] = new Array();' + 
												 tx + '[\'elemento\'][\'' + id + '\'][\'id\'] 		 = \'' + id + '\';' + 
												 tx + '[\'elemento\'][\'' + id + '\'][\'visivel\']  = \'' + visivel + '\';' +
												 tx + '[\'elemento\'][\'' + id + '\'][\'elemento\'] = new Array();';
										
									}	
	
									// Executa o texto JS
									eval(txObj);	
								}catch(err){
									alert('erro no mapeamento da tabela!');
								}								
								return;
							}

	this.controle = function(id, idPaiTx, imgObj)
					{						
						var elementoId;	
						var visibilidade   = '';
						var visibilidadeId = '';
						var idPai 		   = new Array();
						
						// Transforma string com pais em array.		
						if ((idPaiTx.indexOf(':') > -1)){
							idPai = idPaiTx.split(':');	
						}else if (idPaiTx){
							idPai.push(idPaiTx);
						}	
						// Verifica se o elemento est? no primeiro n?vel.
						if (idPaiTx != id){
							// Desce em n?veis, at? chegar no elemento.
							for(i=0; i<idPai.length; i++){
								elementoId = elementoId ? elementoId.elemento[idPai[i]] : elemento[idPai[i]];
							}
							elementoId = elementoId.elemento[id];
						}else{
							elementoId = elemento[id];		
						}
						
						visibilidadeId = elementoId.visivel;
						
						// Modifica a imagem e seta o atributo de visibilidade.
						if (visibilidadeId == 'S'){
							imgObj.src = '/imagens/mais.gif';		
							elementoId.visivel = 'N';
						}else{
							imgObj.src = '/imagens/menos.gif';
							elementoId.visivel = 'S';		
						}
						// Faz maximiza??o ou minimiza??o dos elementos
						controleSpancao(elementoId, elementoId.visivel);
					}
					
	controleSpancao = function (elementoId, visibilidade)
					  {
						var elementoFilho;
						var elementoAtual;
						var display;
						
						// Seta ao atributo o valor de maximiza??o ou minimiza??o 
						display = visibilidade == 'N' ? 'none' : ( navigator.appName.indexOf('Explorer') > -1 ? 'block' : 'table-row');
						
						// Varre nos elementos filhos	
						for (var i in elementoId.elemento){
							elementoAtual = elementoId.elemento[i];
							if (!elementoAtual.id){
								continue;
							}
							elementoFilho = d.getElementById(elementoId.elemento[i].id);
							if (elementoFilho){
								elementoFilho.style.display = display;
								// Varre os elementos filhos, aplicando a recursividade. 
								for (var a in elementoAtual.elemento){
									if (!d.getElementById(elementoAtual.elemento[a].id))
										continue;
										
									if (visibilidade == 'N' && elementoAtual.visivel == 'S'){
										controleSpancao(elementoAtual, visibilidade);
									}else if (visibilidade == 'S' && elementoId.elemento[i].visivel == 'S'){
										controleSpancao(elementoAtual, visibilidade);											
									}
								}	
							}
						} 
						return;							
					  }
	
}
*/