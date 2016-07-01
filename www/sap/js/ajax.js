/**
 * Busca endereco completo do cep
 * @name carregarEndereco
 * @param eudcep - cep
 * @return void
 */
function carregarEndereco(cep){
	
	if(cep != '' && cep.length < 10){
		
		alert('Cep inv�lido');
		limparEndereco();
		
	}else if(cep != ''){
	
		new Ajax.Request('ajax.php',{
			method: 'post',
			parameters: {'cep': cep, 'servico': 'carregarEndereco'},
			onComplete: function(transport){
			    
				var resposta = transport.responseText.evalJSON();	
	
				if(resposta.status == 'ok'){
					
					//caso retorne cep
					if(resposta.cep != ''){
						var cep = mascaraglobal('##.###-###', resposta.cep)
						if($('eudcep')){
							$('eudcep').setValue(cep);
						}
						else if($('forcep')){
							$('forcep').setValue(cep);
						}
					}
					
					//caso retorne logradouro
					if(resposta.log != ''){
						if($('eudlog')){
							$('eudlog').setValue(resposta.log);
						}
						else if($('forlog')){
							$('forlog').setValue(resposta.log);
						}
					}
					
					//caso retorne bairro
					if(resposta.bai != ''){
						if($('eudbai')){
							$('eudbai').setValue(resposta.bai);
						}
						else if($('forbai')){
							$('forbai').setValue(resposta.bai);
						}
					}
					
					//caso retorne cidade
					if(resposta.cid != ''){
						if($('eudcid')){
							$('eudcid').setValue(resposta.cid);
						}
						else if($('forcid')){
							$('forcid').setValue(resposta.cid);
						}
					}
						
					//caso retorne uf
					if(resposta.uf != ''){
						if($('euduf')){
							$('euduf').setValue(resposta.uf);
						}
						else if($('foruf')){
							$('foruf').setValue(resposta.uf);
						}
					}
					
					//caso os campos de complemento e n�mero estejam readonly, habilita
					if($('eudcom')){
						if($('eudcom').hasAttribute('readonly')) { 
							$('eudcom').removeAttribute('readonly'); 
					    }
						$('eudcom').setAttribute('class','normal');
					}
					
					if($('eudnum')){
						if($('eudnum').hasAttribute('readonly')) { 
							$('eudnum').removeAttribute('readonly'); 
					    }
						$('eudnum').setAttribute('class','normal');
					}
						
					
				}else{
					
					limparEndereco();
					alert('Endere�o n�o encontrado.');
					
				}
		    
			},
			onFailure: function(){ 
				
				limparEndereco();
				alert('Ocorreu um erro ao buscar o Cep.');
				
			}
		});
	}
	else if(cep == ''){
		limparEndereco();
	}
}

/**
 * Busca os dados da unidade de acordo com seu codigo interno
 * @name carregarUnidade
 * @param co_interno_uorg - C�digo interno de unidade
 * @param boAtualizaLista - Verifica se true -> atualizar a lista de endere�o se false -> n�o atualiza
 * @return void
 */
function carregarUnidade(co_interno_uorg, boAtualizaLista){
	
	if(co_interno_uorg.length < 1){
		
		alert('Erro ao recuperar o c�digo da Unidade.');
		
	}else{
		
		new Ajax.Request('ajax.php',{
			method: 'post',
			parameters: {'co_interno_uorg': co_interno_uorg, 'servico': 'carregarUnidade'},
			onComplete: function(transport){
			    
				var resposta = transport.responseText.evalJSON();
				
				if(resposta.status == 'ok'){
					
					if(resposta.co_interno_uorg)
						$('co_interno_uorg').setValue(resposta.co_interno_uorg);
					if(resposta.no_unidade_org)
						$('no_unidade_org').setValue(resposta.no_unidade_org);
					
					if( boAtualizaLista != 'false' ) atualizaListaEnderecos(resposta.co_interno_uorg);
					
				}else{
					
					limparBusca();
					alert('Unidade n�o encontrada.');
					
				}
		    
			},
			onFailure: function(){ 
				
				limparBusca();
				alert('Ocorreu um erro ao buscar a Unidade.');
				
			}
		});
		
	}
	
}

/**
 * Exibe a lista de endere�os de acordo com a unidade selecionada
 * @name atualizaListaEnderecos
 * @param co_interno_uorg - C�digo interno de unidade
 * @return void
 */
function atualizaListaEnderecos(co_interno_uorg){
	
	new Ajax.Updater('atualizaListaEnderecos', 'ajax.php', {
		method : 'post',
		parameters: {'co_interno_uorg': co_interno_uorg, 'servico': 'atualizaListaEnderecos'},
		evalScript: true	
	});	
	
}

/**
 * Carrega a combo de motivos de estado de conservacao de acordo com o estado conservacao
 * @name montaComboMotivoEstadoConservacao
 * @param ecoid       - C�digo do Estado de Conserva��o
 * @param mecid       - C�digo do Motivo de Estado de Conserva��o
 * @param obrigatorio - Indica se a imagem de obrigatoriedade aparecer�
 * @return void
 */
function montaComboMotivoEstadoConservacao(ecoid,mecid,obrigatorio){

	if(typeof(mecid) ==  undefined)
		mecid = null;
		
	if(typeof(obrigatorio) ==  undefined)
		obrigatorio = null;

	// buscando
	new Ajax.Updater('motivoestadoconservacao', 'ajax.php', {
		method : 'post',
		asynchronous: false,
		parameters : {'ecoid': ecoid, 'mecid': mecid, 'obrigatorio':obrigatorio, 'servico': 'montaComboMotivoEstadoConservacao'}
	});
	
}

/**
 * Carrega a combo de itens de conta cont�bil
 * @name carregarItemContaContabil
 * @param ccbid       - C�digo da conta cont�bil
 * @param obrigatorio - Indica se o campo ser� obrigat�rio
 * @return void
 */
function carregarItemContaContabil(ccbid,obrigatorio){
	
	// buscando
	new Ajax.Updater('itemconta', 'ajax.php', {
		method : 'post',
		parameters : {'ccbid': ccbid, 'obrigatorio':obrigatorio, 'servico': 'carregarItemContaContabil'}
	});
	
}

/**
 * Carrega a combo de itens de conta cont�bil por classe
 * @name carregarItemContaContabilPorClasse
 * @param ccbid - C�digo da conta cont�bil
 * @param clscodclasse - C�digo da Classe
 * @return void
 */
function carregarItemContaContabilPorClasse(ccbid, clscodclasse){
	
	// verificando se ir� buscar tamb�m por classe	
	if($('clscodclasse'))
		clscodclasse = $('clscodclasse').getValue();
	else
		clscodclasse = '';
	
	
	// buscando
	new Ajax.Updater('itemconta', 'ajax.php', {
		method : 'post',
		parameters : {'ccbid': ccbid, 'clscodclasse': clscodclasse, 'servico': 'carregarItemContaContabilPorClasse'}
	});
	
}



/**
 * Carrega a combo de conta cont�bil
 * @name carregarContaContabil
 * @param clscodclasse - C�digo da classe
 * @return void
 */
function carregarContaContabil(clscodclasse){
	
	new Ajax.Updater('contacontabil', 'ajax.php', {
		method : 'post',
		parameters : {'clscodclasse': clscodclasse, 'servico': 'carregarContaContabil'}
	});
	
}

/**
 * Busca os dados da classe de acordo com seu codigo
 * @name carregarClasse
 * @param clscodclasse - C�digo da Classe
 * @return void
 */
function carregarClasse(clscodclasse){
	
	if(clscodclasse.length < 1){
		
		alert('Erro ao recuperar o c�digo da Classe.');
		
	}else{
		
		new Ajax.Request('ajax.php',{
			method: 'post',
			parameters: {'clscodclasse': clscodclasse, 'servico': 'carregarClasse'},
			onComplete: function(transport){
			    
				var resposta = transport.responseText.evalJSON();	
	
				if(resposta.status == 'ok'){
					
					if(resposta.clscodclasse)
						$('clscodclasse').setValue(resposta.clscodclasse);
					if(resposta.clsdescclasse)
						$('clsdescclasse').setValue(resposta.clsdescclasse);
					
					atualizaListaMateriais(resposta.clscodclasse);
					carregarContaContabil(resposta.clscodclasse);
					
				}else{
					
					limparBusca();
					alert('Classe n�o encontrada.');
					
				}
		    
			},
			onFailure: function(){ 
				
				limparBusca();
				alert('Ocorreu um erro ao buscar a Classe.');
				
			}
		});
		
	}
	
}

/**
 * Busca os dados do material de acordo com seu codigo
 * @name carregarMaterial
 * @param matid - C�digo do Material
 * @return void
 */
function carregarMaterial(matid){
	
	if(matid.length < 1){
		
		alert('Erro ao recuperar o c�digo do Material.');
		
	}else{
		
		new Ajax.Request('ajax.php',{
			method: 'post',
			parameters: {'matid': matid, 'servico': 'carregarMaterial'},
			onComplete: function(transport){
			    
				var resposta = transport.responseText.evalJSON();	
	
				if(resposta.status == 'ok'){
					
					if(resposta.matid && $('matid'))
						$('matid').setValue(resposta.matid);
					if(resposta.matdsc && $('matdsc'))
						$('matdsc').setValue(resposta.matdsc);
					if(resposta.ccbid && $('ccbid'))
						$('ccbid').setValue(resposta.ccbid);
					if(resposta.ccbdsc && $('ccbdsc'))
						$('ccbdsc').setValue(resposta.ccbdsc);
					
				}else{
					
					alert('Material n�o encontrado.');
					
				}
		    
			},
			onFailure: function(){ 
				
				alert('Ocorreu um erro ao buscar o Material.');
				
			}
		});
		
	}
	
}

/**
 * Busca os dados do Endereco de unidade administrativa de acordo com seu codigo
 * @name carregarEnderecoUnidade
 * @param eudid - C�digo do Endereco de unidade administrativa
 * @return void
 */
function carregarEnderecoUnidade(eudid){
	
	if(eudid.length < 1){
		
		alert('Erro ao recuperar o c�digo do Endere�o da Unidade.');
		
	}else{
		
		new Ajax.Request('ajax.php',{
			method: 'post',
			parameters: {'eudid': eudid, 'servico': 'carregarEnderecoUnidade'},
			onComplete: function(transport){
			    
				var resposta = transport.responseText.evalJSON();	
	
				if(resposta.status == 'ok'){
					
					if(resposta.eudid)
						$('eudid').setValue(resposta.eudid);
					if(resposta.no_unidade_org)
						$('no_unidade_org').setValue(resposta.no_unidade_org);
					if(resposta.eudlog)
						$('eudlog').setValue(resposta.eudlog);
					if(resposta.eudcom)
						$('eudcom').setValue(resposta.eudcom);
					if(resposta.eudid)
						$('eudnum').setValue(resposta.eudnum);
					
				}else{
					
					alert('Endere�o da Unidade n�o encontrado.');
					
				}
		    
			},
			onFailure: function(){ 
				
				alert('Ocorreu um erro ao buscar o Endere�o da Unidade.');
				
			}
		});
		
	}
	
}

/**
 * Exibe a lista de materiais de acordo com a classe selecionada
 * @name atualizaListaMateriais
 * @param clscodclasse - C�digo da Classe
 * @return void
 */
function atualizaListaMateriais(clscodclasse){
	
	new Ajax.Updater('atualizaListaMateriais', 'ajax.php', {
		method : 'post',
		parameters: {'clscodclasse': clscodclasse, 'servico': 'atualizaListaMateriais'},
		evalScript: true	
	});	
	
}

/**
 * Carrega a combo de cidades de acordo com a uf
 * @name carregarCidade
 * @param uf - Estado
 * @return void
 */
function carregarCidade(uf){
	
	// buscando
	new Ajax.Updater('cidades', 'ajax.php', {
		method : 'post',
		parameters : {'uf': uf, 'servico': 'carregarCidade'}
	});
	
}



/**
 * Metodo responsavel por fazer a requisi��o ajax que busca dados do respons�vel 
 * atrav�s da matr�cula siape digitada 
 * Caso a unidade vinculada ao respons�vel em quest�o tenha apenas um endere�o associado a ela
 * retorna tamb�m a unidade e os dados do endere�o
 * 
 * @name carregaResponsavel
 * @param integer nu_matricula_siape - matr�cula siape do respons�vel 
 * @author Alysson Rafael
 * @return string
 */
function carregaResponsavel(nu_matricula_siape){
	
	new Ajax.Request('ajax.php',{
		method: 'post',
		parameters: {'nu_matricula_siape': nu_matricula_siape, 'servico': 'carregarResponsavel'},
		onComplete: function(transport){
		    
			//limpa os campos de endere�o e de unidade, se houverem
			if($('unidade')){
				$('unidade').clear();
			}
			
			if($('uendid')){
				$('uendid').clear();
			}
			
			if($('enduf')){
				$('enduf').clear();
			}
			
			if($('endcid')){
				$('endcid').clear();
			}
			
			if($('endcep')){
				$('endcep').clear();
			}
			
			if($('endlog')){
				$('endlog').clear();
			}
			
			if($('enadescricao')){
				$('enadescricao').clear();
			}
			
			if($('easdescricao')){
				$('easdescricao').clear();
			}
			
			var resposta = transport.responseText.evalJSON();	

			if(resposta.status == 'ok'){
				
				//caso traga o nome do respons�vel
				if($('no_servidor') && resposta.no_servidor != ''){
					$('no_servidor').setValue(resposta.no_servidor);
				}
				else if($('no_servidor')){
					$('no_servidor').clear();
				}
				
				//caso traga o nome da unidade
				if($('unidade') && resposta.uorno != ''){
					$('unidade').setValue(resposta.uorno);
				}
				else if($('unidade')){
					$('unidade').clear();
				}
				
				if($('uorco_uorg_lotacao_servidor') && resposta.uorco_uorg_lotacao_servidor != ''){
					$('uorco_uorg_lotacao_servidor').setValue(resposta.uorco_uorg_lotacao_servidor);
				}
				else if($('uorco_uorg_lotacao_servidor')){
					$('uorco_uorg_lotacao_servidor').clear();
				}
				
				if($('uorsg') && resposta.uorsg != ''){
					$('uorsg').setValue(resposta.uorsg);
				}
				else if($('uorsg')){
					$('uorsg').clear();
				}
				
				//caso traga a UF
				if($('enduf') && resposta.enduf != ''){
					$('enduf').setValue(resposta.enduf);
				}
				else if($('enduf')){
					$('enduf').clear();
				}
				
				//caso traga a cidade
				if($('endcid') && resposta.endcid != ''){
					$('endcid').setValue(resposta.endcid);
				}
				else if($('endcid')){
					$('endcid').clear();
				}
				
				//caso traga o CEP
				if($('endcep') && resposta.endcep != ''){
					$('endcep').setValue(resposta.endcep);
				}
				else if($('endcep')){
					$('endcep').clear();
				}
				
				//caso traga o logradouro
				if($('endlog') && resposta.endlog != ''){
					$('endlog').setValue(resposta.endlog);
				}
				else if($('endlog')){
					$('endlog').clear();
				}
				
				//caso traga o andar
				if($('enadescricao') && resposta.enadescricao != ''){
					$('enadescricao').setValue(resposta.enadescricao);
				}
				else if($('enadescricao')){
					$('enadescricao').clear();
				}
				
				//caso traga a sala
				if($('easdescricao') && resposta.easdescricao != ''){
					$('easdescricao').setValue(resposta.easdescricao);
				}
				else if($('easdescricao')){
					$('easdescricao').clear();
				}
				
				//caso traga o campo q corresponde � chave da tabela de bens
				if($('uendid') && resposta.uendid != ''){
					$('uendid').setValue(resposta.uendid);
				}
				else if($('uendid')){
					$('uendid').clear();
				}

				
			}else{
				limparBuscaResponsavel();
				alert('A Matr�cula Informada N�o Foi Encontrada.');
			}
	    
		},
		onFailure: function(){ 
			alert('Ocorreu um erro ao buscar o Respons�vel.');
		}
	});
	
}




/**
 * Metodo responsavel por fazer a requisi��o ajax que busca nome do fornecedor 
 * atrav�s do cnpj digitado 
 * 
 * @name carregaFornecedor
 * @param string forcpfcnpj - cnpj do fornecedor
 * @author Alysson Rafael
 * @return string
 */
function carregaFornecedor(forcpfcnpj){
	
	forcpfcnpj = forcpfcnpj.replace('.','');
	forcpfcnpj = forcpfcnpj.replace('.','');
	forcpfcnpj = forcpfcnpj.replace('/','');
	forcpfcnpj = forcpfcnpj.replace('-','');
	
	$('fornomefantasia').clear();
	
	if(forcpfcnpj != ''){
		new Ajax.Request('ajax.php',{
			method: 'post',
			parameters: {'forcpfcnpj': forcpfcnpj, 'servico': 'carregaFornecedor'},
			onComplete: function(transport){
			    
				var resposta = transport.responseText.evalJSON();	

				if(resposta.status == 'ok'){
					if(resposta.forrazaosocial != ''){
						$('fornomefantasia').setValue(resposta.forrazaosocial);
					}
				}else{
					limparBuscaFornecedor();
					alert('Fornecedor n�o encontrado. Fa�a o cadastro do mesmo.');
				}
		    
			},
			onFailure: function(){ 
				
				alert('Ocorreu um erro ao buscar o Fornecedor.');
				
			}
		});
	}
	
	
}

/**
 * Metodo responsavel por fazer a requisi��o ajax que busca os dados do rgp
 * atrav�s do rgpnum digitado
 * @param rgpnum - N�mero do Rgp
 * @param success
 * @param failure
 * @param complete
 * @param create
 * @author Silas Matheus
 * @return void
 */
function carregaDadosRGP(rgpnum, success, failure, create, complete){

	if(rgpnum.length > 0){
		
		new Ajax.Request('ajax.php',{
			method: 'post',
			parameters: {'rgpnum': rgpnum, 'servico': 'carregarDadosRGP'},
			onCreate: eval(create),
			onSuccess: eval(success),
			onFailure: eval(failure),
			onComplete: eval(complete)
		});
		
	}

}

/**
 * Metodo responsavel por fazer a requisi��o ajax que busca dados na receita
 * atrav�s do CNPJ digitado 
 * @name procuraNomeReceita
 * @param int cnpj - numero do CNPJ
 * @author Romualdo da Silva
 * @return void
 */
function procuraNomeReceita(cnpj){
	document.getElementById('aguarde').style.visibility = 'visible';
	document.getElementById('aguarde').style.display = '';
	
	var comp     = new dCNPJ();
	comp.buscarDados( cnpj );
	document.getElementById('bebnomerecebedor').value = comp.dados.no_empresarial_rf;
	
	document.getElementById('aguarde').style.visibility = 'hidden';
	document.getElementById('aguarde').style.display = 'none';
}

/**
 * Busca os dados do Empenho de acordo com seu id
 * @name carregarEmpenho
 * @param empid - Id do Empenho
 * @return void
 */
function carregarEmpenho(empid){
	
	if(empid.length < 1){
		
		alert('Erro ao recuperar o c�digo do Empenho.');
		
	}else{
		
		//verifica se o empenho selecionado ainda possui valor dispon�vel
		var empenhoesgotado = false;
		var benvlrdoc = $('benvlrdoc').getValue();
		new Ajax.Request('ajax.php',{
			method: 'post',
			asynchronous: false,
			parameters: {'empid': empid,'benvlrdoc':benvlrdoc, 'servico': 'verificarEmpenhoDisponivel'},
			onComplete: function(transport){
			    
				var resposta = transport.responseText.evalJSON();	
	
				if(resposta.totalmenteusado != ''){
					empenhoesgotado = true;
					alert('O Valor para Material Permanente do Empenho selecionado j� est� completamente utilizado por outras entradas.');
				}
		    
			},
			onFailure: function(){ 
				
				alert('Ocorreu um erro ao verificar a Disponibilidade do Empenho.');
				
			}
		});
		
		//caso ainda tenha valor dispon�vel
		if(!empenhoesgotado){
			
			//verifica quais processos de entrada de bens
			//est�o utilizando o empenho em quest�o e qual valor ainda tem dispon�vel
			new Ajax.Request('ajax.php',{
				method: 'post',
				parameters: {'empid': empid, 'servico': 'verificarEmpenhoUsado'},
				onComplete: function(transport){
				    
					var resposta = transport.responseText.evalJSON();	
		
					if(resposta.disponivel != ''){
						alert('O Empenho selecionado est� sendo utilizado pelo(s) processo(s) de entrada '+resposta.processos+' e possui R$'+resposta.disponivel+' para Material Permanente dispon�vel para utiliza��o em novos processos de entrada.');
					}
			    
				},
				onFailure: function(){ 
					
					alert('Ocorreu um erro ao verificar o Empenho.');
					
				}
			});
			

			new Ajax.Request('ajax.php',{
				method: 'post',
				parameters: {'empid': empid, 'servico': 'carregarEmpenho'},
				onComplete: function(transport){
				    
					var resposta = transport.responseText.evalJSON();	
		
					if(resposta.status == 'ok'){
						
						if(resposta.empnumero != ''){
							$('empnumero').setValue(resposta.empnumero);
						}
						
						// Se for a TelaConsultaTermos, ignorar os campos abaixo
						if(typeof TelaConsultaTermos == 'undefined'){
							if(resposta.empdata != ''){
								$('empdata').setValue(resposta.empdata);
							}
							if(resposta.empvalorper != ''){
								$('empvalorper').setValue(MascaraMonetario(resposta.empvalorper));
							}
						}

						
						if(resposta.empid != ''){
							$('empid').setValue(resposta.empid);
						}
						
					}else{
						
						alert('Empenho n�o encontrado.');
						
					}
			    
				},
				onFailure: function(){ 
					
					alert('Ocorreu um erro ao buscar o Empenho.');
					
				}
			});
			
		}
		
		
		
	}
	
}


/**
 * Busca os dados do Fornecedor de acordo com seu documento
 * @name carregarFornecedor
 * @param forcpfcnpj - Documento do fornecedor
 * @return void
 */
function carregarFornecedor(forcpfcnpj){
	
	if(forcpfcnpj.length < 1){
		
		alert('Erro ao recuperar o documento do Fornecedor.');
		
	}else{
		
		new Ajax.Request('ajax.php',{
			method: 'post',
			parameters: {'forcpfcnpj': forcpfcnpj, 'servico': 'carregarFornecedor'},
			onComplete: function(transport){
			    
				var resposta = transport.responseText.evalJSON();	
	
				if(resposta.status == 'ok'){
					
					if(resposta.forcpfcnpj != ''){
						$('forcpfcnpj').setValue(resposta.forcpfcnpj);
					}
					else{
						$('forcpfcnpj').clear();
					}
					if(resposta.forrazaosocial != ''){
						$('fornomefantasia').setValue(resposta.forrazaosocial);
					}
					else{
						$('fornomefantasia').clear();
					}
				}else{
					alert('Fornecedor n�o encontrado.');
				}
		    
			},
			onFailure: function(){ 
				alert('Ocorreu um erro ao buscar o Fornecedor.');
			}
		});
		
	}
	
}

/**
 * Busca os dados do Processo de acordo com seu numero
 * @name carregarProcesso
 * @author Silas Matheus
 * @param numprocesso - Numero do processo Sidoc
 * @return void
 */
function carregarProcesso(numprocesso){
	
	if(numprocesso.length < 10){
		
		if(numprocesso.length > 0)
			alert('O processo informado n�o foi encontrado.');
		
		if($('bennumproc'))
			$('bennumproc').setValue('');
		if($('bebnumprocesso'))
			$('bebnumprocesso').setValue('');
		if($('bendtproc'))
			$('bendtproc').setValue('');
		if($('bebdataprocesso'))
			$('bebdataprocesso').setValue('');
		
	}else{
	
		// buscando
		new Ajax.Request('ajax.php',{
			method: 'post',
			parameters: {'numprocesso': numprocesso, 'servico': 'carregarProcesso'},
			onComplete: function(transport){
			    
				var resposta = transport.responseText.evalJSON();	
				
				if(resposta.status == 'ok'){
					
					if($('bendtproc'))
						$('bendtproc').setValue(resposta.data);
					if($('bebdataprocesso'))
						$('bebdataprocesso').setValue(resposta.data);
					
				}else{
					
					if($('bennumproc'))
						$('bennumproc').setValue('');
					if($('bebnumprocesso'))
						$('bebnumprocesso').setValue('');
					if($('bendtproc'))
						$('bendtproc').setValue('');
					if($('bebdataprocesso'))
						$('bebdataprocesso').setValue('');
					alert('O processo '+numprocesso+' n�o est� cadastrado no SIDOC.');
					
				}
		    
			},
			onFailure: function(){ 
				
				alert('N�o foi poss�vel validar o n�mero do processo informado pois ocorreu um erro na integra��o com o SIDOC. Por favor, entre em contato com o setor respons�vel ou tente novamente mais tarde.');
				
			}
		});
	
	}
	
}

/**
 * Busca os andares de acordo com o endere�o
 * @name carregarAndar
 * @author Alysson Rafael
 * @param endid - Id do endere�o
 * @return void
 */
function carregarAndar(endid){
	
	// buscando
	new Ajax.Updater('andar', 'ajax.php', {
		method : 'post',
		parameters : {'endid': endid, 'servico': 'carregarAndar'}
	});
	
}

/**
 * Busca as salas de acordo com o andar
 * @name carregarSala
 * @author Alysson Rafael
 * @param enaid - Id do andar
 * @return void
 */
function carregarSala(enaid){
	
	// buscando
	new Ajax.Updater('sala', 'ajax.php', {
		method : 'post',
		parameters : {'enaid': enaid, 'servico': 'carregarSala'}
	});
	
}


/**
 * Busca os dados do endere�o pelo id da tabela uorgendereco
 * @name carregaDadosEndereco
 * @author Alysson Rafael
 * @param uendid - Id do endere�o
 * @return void
 */
function carregaDadosEndereco(uendid){
	
	if(uendid != ''){
		
		new Ajax.Request('ajax.php',{
			method: 'post',
			parameters: {'uendid': uendid, 'servico': 'carregaDadosEndereco'},
			onComplete: function(transport){
			    
				var resposta = transport.responseText.evalJSON();	
	
				if(resposta.status == 'ok'){
					
					//seta o cep
					if($('endcep') && resposta.endcep != ''){
						$('endcep').setValue(resposta.endcep);
					}
					else if($('endcep')){
						$('endcep').clear();
					}
					
					//seta a uf
					if($('enduf') && resposta.enduf != ''){
						$('enduf').setValue(resposta.enduf);
					}
					else if($('enduf')){
						$('enduf').clear();
					}
					
					//seta a cidade
					if($('endcid') && resposta.endcid != ''){
						$('endcid').setValue(resposta.endcid);
					}
					else if($('endcid')){
						$('endcid').clear();
					}
					
					//seta o bairro
					if($('endbairro') && resposta.endbairro != ''){
						$('endbairro').setValue(resposta.endbairro);
					}
					else if($('endbairro')){
						$('endbairro').clear();
					}
					
					//seta o logradouro
					if($('endlog') && resposta.endlog != ''){
						$('endlog').setValue(resposta.endlog);
					}
					else if($('endlog')){
						$('endlog').clear();
					}
					
					//seta o complemento
					if($('endcom') && resposta.endcom != ''){
						$('endcom').setValue(resposta.endcom);
					}
					else if($('endcom')){
						$('endcom').clear();
					}
					
					//seta o n�mero
					if($('endnum') && resposta.endnum != ''){
						$('endnum').setValue(resposta.endnum);
					}
					else if($('endnum')){
						$('endnum').clear();
					}
					
					//seta a unidade
					if($('unidade') && resposta.uorno != ''){
						$('unidade').setValue(resposta.uorno);
					}
					else if($('unidade')){
						$('unidade').clear();
					}
					
					if($('uorco_uorg_lotacao_servidor') && resposta.uorco_uorg_lotacao_servidor != ''){
						$('uorco_uorg_lotacao_servidor').setValue(resposta.uorco_uorg_lotacao_servidor);
					}
					else if($('uorco_uorg_lotacao_servidor')){
						$('uorco_uorg_lotacao_servidor').clear();
					}
					
					
					//seta o campo q define a rela��o unidade_endereco
					if($('uendid') && resposta.uendid != ''){
						$('uendid').setValue(resposta.uendid);
					}
					else if($('uendid')){
						$('uendid').clear();
					}
					
					//seta o andar
					if($('enadescricao') && resposta.enadescricao != ''){
						$('enadescricao').setValue(resposta.enadescricao);
					}
					else if($('enadescricao')){
						$('enadescricao').clear();
					}
					
					//seta a sala
					if($('easdescricao') && resposta.easdescricao != ''){
						$('easdescricao').setValue(resposta.easdescricao);
					}
					else if($('easdescricao')){
						$('easdescricao').clear();
					}
					
				}else{
					alert('Endere�o n�o encontrado.');
				}
		    
			},
			onFailure: function(){ 
				alert('Ocorreu um erro ao buscar o Endere�o.');
			}
		});
		
	}
	
}






function carregaDadosEnderecoPorEndereco(endid){
	
	if(endid != ''){
		
		new Ajax.Request('ajax.php',{
			method: 'post',
			parameters: {'endid': endid, 'servico': 'carregaDadosEnderecoPorEndereco'},
			onComplete: function(transport){
			    
				var resposta = transport.responseText.evalJSON();	
	
				if(resposta.status == 'ok'){
					
					//seta o cep
					if($('endcep') && resposta.endcep != ''){
						$('endcep').setValue(resposta.endcep);
					}
					else if($('endcep')){
						$('endcep').clear();
					}
					
					//seta a uf
					if($('enduf') && resposta.enduf != ''){
						$('enduf').setValue(resposta.enduf);
					}
					else if($('enduf')){
						$('enduf').clear();
					}
					
					//seta a cidade
					if($('endcid') && resposta.endcid != ''){
						$('endcid').setValue(resposta.endcid);
					}
					else if($('endcid')){
						$('endcid').clear();
					}
					
					//seta o bairro
					if($('endbairro') && resposta.endbairro != ''){
						$('endbairro').setValue(resposta.endbairro);
					}
					else if($('endbairro')){
						$('endbairro').clear();
					}
					
					//seta o logradouro
					if($('endlog') && resposta.endlog != ''){
						$('endlog').setValue(resposta.endlog);
					}
					else if($('endlog')){
						$('endlog').clear();
					}
					
					//seta o complemento
					if($('endcom') && resposta.endcom != ''){
						$('endcom').setValue(resposta.endcom);
					}
					else if($('endcom')){
						$('endcom').clear();
					}
					
					//seta o n�mero
					if($('endnum') && resposta.endnum != ''){
						$('endnum').setValue(resposta.endnum);
					}
					else if($('endnum')){
						$('endnum').clear();
					}
					
					//seta o andar
					if($('enadescricao') && resposta.enadescricao != ''){
						$('enadescricao').setValue(resposta.enadescricao);
					}
					else if($('enadescricao')){
						$('enadescricao').clear();
					}
					
					//seta a sala
					if($('easdescricao') && resposta.easdescricao != ''){
						$('easdescricao').setValue(resposta.easdescricao);
					}
					else if($('easdescricao')){
						$('easdescricao').clear();
					}
					
				}else{
					alert('Endere�o n�o encontrado.');
				}
		    
			},
			onFailure: function(){ 
				alert('Ocorreu um erro ao buscar o Endere�o.');
			}
		});
		
	}
	
}






/**
 * Busca os dados do endere�o pelo id da tabela uorgendereco
 * @name carregaDadosEnderecoMovimentacaoLocalidade
 * @author Alysson Rafael
 * @param uendid - Id do endere�o
 * @return void
 */
function carregaDadosEnderecoMovimentacaoLocalidade(uendid,atualNova){
	
	if(uendid != ''){
		
		new Ajax.Request('ajax.php',{
			method: 'post',
			parameters: {'uendid': uendid, 'servico': 'carregaDadosEndereco'},
			onComplete: function(transport){
			    
				var resposta = transport.responseText.evalJSON();	
	
				if(resposta.status == 'ok'){
					
					//seta o cep
					if(atualNova == 'atual' && $('endcepatual') && resposta.endcep != ''){
						$('endcepatual').setValue(resposta.endcep);
					}
					else if(atualNova == 'nova' && $('endcepnova') && resposta.endcep != ''){
						$('endcepnova').setValue(resposta.endcep);
					}
					
										
					//seta a uf
					if(atualNova == 'atual' && $('endufatual') && resposta.enduf != ''){
						$('endufatual').setValue(resposta.enduf);
					}
					else if(atualNova == 'nova' && $('endufnova') && resposta.enduf != ''){
						$('endufnova').setValue(resposta.enduf);
					}
					
					
					//seta a cidade
					if(atualNova == 'atual' && $('endcidatual') && resposta.endcid != ''){
						$('endcidatual').setValue(resposta.endcid);
					}
					else if(atualNova == 'nova' && $('endcidnova') && resposta.endcid != ''){
						$('endcidnova').setValue(resposta.endcid);
					}

					
					//seta o logradouro
					if(atualNova == 'atual' && $('endlogatual') && resposta.endlog != ''){
						$('endlogatual').setValue(resposta.endlog);
					}
					else if(atualNova == 'nova' && $('endlognova') && resposta.endlog != ''){
						$('endlognova').setValue(resposta.endlog);
					}

					
					//seta a unidade
					if(atualNova == 'atual' && $('unidadeatual') && resposta.uorno != ''){
						$('unidadeatual').setValue(resposta.uorno);
					}
					else if(atualNova == 'nova' && $('unidadenova') && resposta.uorno != ''){
						$('unidadenova').setValue(resposta.uorno);
					}

					
					//seta o campo q define a rela��o unidade_endereco
					if(atualNova == 'atual' && $('uendidatual') && resposta.uendid != ''){
						$('uendidatual').setValue(resposta.uendid);
					}
					else if(atualNova == 'nova' && $('uendidnova') && resposta.uendid != ''){
						$('uendidnova').setValue(resposta.uendid);
					}

					
					//seta o andar
					if(atualNova == 'atual' && $('enadescricaoatual') && resposta.enadescricao != ''){
						$('enadescricaoatual').setValue(resposta.enadescricao);
					}
					else if(atualNova == 'nova' && $('enadescricaonova') && resposta.enadescricao != ''){
						$('enadescricaonova').setValue(resposta.enadescricao);
					}

					
					//seta a sala
					if(atualNova == 'atual' && $('easdescricaoatual') && resposta.easdescricao != ''){
						$('easdescricaoatual').setValue(resposta.easdescricao);
					}
					else if(atualNova == 'nova' && $('easdescricaonova') && resposta.easdescricao != ''){
						$('easdescricaonova').setValue(resposta.easdescricao);
					}
					
					if(atualNova == 'atual'){
						$('requisicao').setValue('salvar');
						$('formularioCadastro').submit();
					}

					
				}
				else{
					alert('Endere�o n�o encontrado.');
				}
		    
			},
			onFailure: function(){ 
				alert('Ocorreu um erro ao buscar o Endere�o.');
			}
		});
		
	}
	
}










/**
 * Busca o nome da unidade pela chave prim�ria
 * @name setaNomeUnidade
 * @author Alysson Rafael
 * @param uorco_uorg_lotacao_servidor - Chave de unidade organizacional
 * @return void
 */
function setaNomeUnidade(uorco_uorg_lotacao_servidor){
	if(uorco_uorg_lotacao_servidor != ''){
		new Ajax.Request('ajax.php',{
			method: 'post',
			parameters: {'uorco_uorg_lotacao_servidor': uorco_uorg_lotacao_servidor, 'servico': 'carregarNomeUnidade'},
			onComplete: function(transport){
			    
				var resposta = transport.responseText.evalJSON();	
	
				if(resposta.status == 'ok'){
					
					if($('unidade') && resposta.uorno != ''){
						$('unidade').setValue(resposta.uorno);
					}
					else if($('unidade')){
						$('unidade').clear();
					}
					
					if($('uendid') && resposta.uendid != ''){
						$('uendid').setValue(resposta.uendid);
					}
					else if($('uendid')){
						$('uendid').clear();
					}
					
				}else{
					alert('Unidade n�o encontrada.');
				}
		    
			},
			onFailure: function(){ 
				alert('Ocorreu um erro ao buscar a Unidade.');
			}
		});
	}
	else{
		
		if($('unidade')){
			$('unidade').clear();
		}
		
		if($('uendid')){
			$('uendid').clear();
		}
	}
}





/**
 * M�todo respons�vel por carregar o nome do respons�vel e atualizar a combo de unidades
 * 
 * @name carregaResponsavelUnidade
 * @param string nu_matricula_siape - matr�cula siape do respons�vel 
 * @author Alysson Rafael
 * @return void
 */
function carregaResponsavelUnidade(nu_matricula_siape){
	
	new Ajax.Request('ajax.php',{
		method: 'post',
		parameters: {'nu_matricula_siape': nu_matricula_siape, 'servico': 'carregaResponsavelUnidade'},
		onComplete: function(transport){
		    
			var resposta = transport.responseText.evalJSON();	

			if(resposta.status == 'ok'){
				
				//caso traga o nome do respons�vel
				if($('no_servidor') && resposta.no_servidor != ''){
					$('no_servidor').setValue(resposta.no_servidor);
				}
				else if($('no_servidor')){
					$('no_servidor').clear();
				}
				
				
				//chama o updater para atualizar a combo de unidades
				if(resposta.co_uorg_lotacao_servidor != ''){
					new Ajax.Updater('unidades', 'ajax.php', {
						method : 'post',
						parameters : {'co_uorg_lotacao_servidor': resposta.co_uorg_lotacao_servidor, 'servico': 'filtrarUnidades'}
					});
					$('uorno').clear();
				}
				
				
			}else{
				limparBuscaResponsavel();
				alert('A Matr�cula Informada N�o Foi Encontrada.');
				
				//chama o updater aqui para que a combo de unidades volte ao estado inicial
				new Ajax.Updater('unidades', 'ajax.php', {
					method : 'post',
					parameters : {'co_uorg_lotacao_servidor': resposta.co_uorg_lotacao_servidor, 'servico': 'filtrarUnidades'}
				});
				
				$('uorno').clear();
			}
	    
		},
		onFailure: function(){ 
			alert('Ocorreu um erro ao buscar o Respons�vel.');
		}
	});
	
}

/**
 * Busca o responsavel para movimentacao
 * @param nu_matricula_siape
 * @param success
 * @param failure
 * @param complete
 * @param create
 * @author Silas Matheus
 * @return void
 */
function carregaResponsavelMovimentacao(nu_matricula_siape, success, failure, create, complete){
	
	
		
		new Ajax.Request('ajax.php',{
			method: 'post',
			onCreate: eval(create),
			parameters: {'nu_matricula_siape': nu_matricula_siape, 'servico': 'carregarResponsavel'},
			onSuccess: eval(success),
			onFailure: eval(failure),
			onComplete: eval(complete)
		});
		
	

}

  
/**
 * M�todo respons�vel por carregar a sigla do tipo de entrada 
 * 
 * @name recuperaSiglaTipoEntrada
 * @param int tipoEntrada - id do tipo de entrada
 * @author Alysson Rafael
 * @return void
 */
function recuperaSiglaTipoEntrada(tipoEntrada){
	
	$('tesslg').clear();
	
	if(tipoEntrada != ''){
		
		new Ajax.Request('ajax.php',{
			method: 'post',
			asynchronous: false,
			parameters: {'tipoEntrada': tipoEntrada, 'servico': 'carregarSiglaTipoEntrada'},
			onComplete: function(transport){
			    
				var resposta = transport.responseText.evalJSON();	
	
				if(resposta.status == 'ok'){
					
					$('tesslg').setValue(resposta.sigla);
					
				}
		    
			},
			onFailure: function(){ 
				alert('Ocorreu um erro ao buscar o Tipo de Entrada.');
			}
		});
		
	}
}



function buscarEmpenho(empnumero){
	if(empnumero != ''){
		
		new Ajax.Request('ajax.php',{
			method: 'post',
			asynchronous: false,
			parameters: {'empnumero': empnumero, 'servico': 'buscarEmpenho'},
			onComplete: function(transport){
			    
				var resposta = transport.responseText.evalJSON();	
				
				if(resposta.status == 'ok'){
					carregarEmpenho(resposta.empid);
				}
				else{
					alert('O n�mero informado n�o foi encontrado.');
					$('empid').clear();
					$('empdata').clear();
					$('empvalorper').clear();
				}
	
				/*if(resposta.status == 'ok'){
					
					$('empid').setValue(resposta.empid);
					$('empdata').setValue(resposta.empdata);
					$('empvalorper').setValue(resposta.empvalorper);
					
				}
				else{
					alert('O n�mero informado n�o foi encontrado.');
					$('empid').clear();
					$('empdata').clear();
					$('empvalorper').clear();
				}*/
		    
			},
			onFailure: function(){ 
				alert('Ocorreu um erro ao buscar o Empenho.');
			}
		});
		
	}
}




function verificaExisteMaterialVinculado(benid,benitdoc){
	if(benid != ''){
		new Ajax.Request('ajax.php',{
			method: 'post',
			asynchronous: false,
			parameters: {'benid': benid,'benitdoc':benitdoc, 'servico': 'verificaExisteMaterialVinculado'},
			onComplete: function(transport){
			    
				var resposta = transport.responseText.evalJSON();	
				$('qtdmaterialcadastrada').setValue(resposta.tot);
	
				if(resposta.status != 'ok'){
					
				    alert('A quantidade de materiais cadastrados na tela Itens do Processo est� diferente da quantidade informada no campo Qtd. de Itens do Documento da tela Dados de Entrada. Corrija esta inconsist�ncia para continuar com o cadastramento.');
					
				}
		    
			},
			onFailure: function(){ 
				alert('Ocorreu um erro ao buscar a qtd de Material.');
			}
		});
	}
}



function verificaTotalValorVinculado(benid,benvlrdoc){
	if(benid != ''){
		new Ajax.Request('ajax.php',{
			method: 'post',
			asynchronous: false,
			parameters: {'benid': benid,'benvlrdoc':benvlrdoc, 'servico': 'verificaTotalValorVinculado'},
			onComplete: function(transport){
			    
				var resposta = transport.responseText.evalJSON();	
				$('valortotalbate').setValue(resposta.eIgual);
	
				if(resposta.status != 'ok'){
					
					alert('O soma dos valores totais dos materiais cadastrados n�o bate com o valor total do documento. Corrija esta inconsist�ncia para continuar com o cadastramento.');
					
				}
		    
			},
			onFailure: function(){ 
				alert('Ocorreu um erro ao buscar o valor total.');
			}
		});
	}
}



