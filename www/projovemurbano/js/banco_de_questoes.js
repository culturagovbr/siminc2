var BancoDeQuestoes = {

	acao 		: null,
	abaMestre 	: null,
	
	init : function(){
		
		BancoDeQuestoes.setAcao( $('#acao').val() );
		BancoDeQuestoes.setAbaMestre( $('#abaMestre').val() );
		BancoDeQuestoes.setAcaoAbaDisciplinas();
		BancoDeQuestoes.marcaAbaComoSelecionada();
		BancoDeQuestoes.marcaPrimeiroFilho();
	},
    
	setAbaMestre : function( valor ){
//		alert(valor);
		BancoDeQuestoes.abaMestre = valor;
		console.log( "abaMestre" + BancoDeQuestoes.abaMestre );
	},
	
	setAcao : function( valor ){
//		alert(valor);
		BancoDeQuestoes.acao = valor;
		console.log( "acao" + BancoDeQuestoes.acao );
	},
	
	setAcaoAbaDisciplinas : function(){
		var listaDisciplinas = new Array();
		listaDisciplinas[0] = "Ci�ncias da Natureza";
		listaDisciplinas[1] = "Ci�ncias Humanas";
		listaDisciplinas[2] = "Ingl�s";
		listaDisciplinas[3] = "L�ngua Portuguesa";
		listaDisciplinas[4] = "Matem�tica";
		listaDisciplinas[5] = "Participa��o Cidad�";
		listaDisciplinas[6] = "Qualifica��o Profissional";
		
		for(var i=0; i < listaDisciplinas.length; i++ ){
			
			// ul .nav-tabs li a [name^=tab]
			
			$.each( $("ul li a"), function(index, value){
				/*console.log($(value).html());
				console.log(listaDisciplinas[i]);
				console.log($(value).html()==listaDisciplinas[i]);*/
				
				//$('a[title^="'+ listaDisciplinas[i] +'"]').click( function(event){
				if( $(value).html() == listaDisciplinas[i] ){
//					console.log('teste');
					jQuery(value).click( function(event){
//						alert('87878');
						event.preventDefault();
						var linkAbaFilha = $(this).attr('href') + "&abaMestre=" + BancoDeQuestoes.abaMestre;
						window.location = linkAbaFilha; 
					} );
				}
			} );
		}
    },

    marcaAbaComoSelecionada : function( ){
    	if( BancoDeQuestoes.abaMestre == 'I' ){
	    	$('a[title="Unidade Formativa I"]').parent().prev().children().attr('src','../imagens/aba_esq_sel.gif');
	    	$('a[title="Unidade Formativa I"]').parent().attr('background','../imagens/aba_fundo_sel.gif');
	    	$('a[title="Unidade Formativa I"]').parent().next().children().attr('src','../imagens/aba_dir_sel_fim.gif');
	    	$('a[title="Unidade Formativa I"]').css('color','#000055').css('text-decoration', 'none');
	    	
    	}else if( BancoDeQuestoes.abaMestre == 'J'){
	    	$('a[title="Unidade Formativa II"]').parent().prev().children().attr('src','../imagens/aba_esq_sel.gif');
	    	$('a[title="Unidade Formativa II"]').parent().attr('background','../imagens/aba_fundo_sel.gif');
	    	$('a[title="Unidade Formativa II"]').parent().next().children().attr('src','../imagens/aba_dir_sel_fim.gif');
	    	$('a[title="Unidade Formativa II"]').css('color','#000055').css('text-decoration', 'none');
	    	
    	}else if( BancoDeQuestoes.abaMestre == 'K'){
	    	$('a[title="Unidade Formativa III"]').parent().prev().children().attr('src','../imagens/aba_esq_sel.gif');
	    	$('a[title="Unidade Formativa III"]').parent().attr('background','../imagens/aba_fundo_sel.gif');
	    	$('a[title="Unidade Formativa III"]').parent().next().children().attr('src','../imagens/aba_dir_sel_fim.gif');
	    	$('a[title="Unidade Formativa III"]').css('color','#000055').css('text-decoration', 'none');
	    	
    	}else if( BancoDeQuestoes.abaMestre == 'Y'){
	    	$('a[title="Unidade Formativa IV"]').parent().prev().children().attr('src','../imagens/aba_esq_sel.gif');
	    	$('a[title="Unidade Formativa IV"]').parent().attr('background','../imagens/aba_fundo_sel.gif');
	    	$('a[title="Unidade Formativa IV"]').parent().next().children().attr('src','../imagens/aba_dir_sel_fim.gif');
	    	$('a[title="Unidade Formativa IV"]').css('color','#000055').css('text-decoration', 'none');
	    	
    	}else if( BancoDeQuestoes.abaMestre == 'T'){
	    	$('a[title="Unidade Formativa V"]').parent().prev().children().attr('src','../imagens/aba_esq_sel.gif');
	    	$('a[title="Unidade Formativa V"]').parent().attr('background','../imagens/aba_fundo_sel.gif');
	    	$('a[title="Unidade Formativa V"]').parent().next().children().attr('src','../imagens/aba_dir_sel_fim.gif');
	    	$('a[title="Unidade Formativa V"]').css('color','#000055').css('text-decoration', 'none');
	    	
    	}else if( BancoDeQuestoes.abaMestre == 'Z'){
	    	$('a[title="Unidade Formativa VI"]').parent().prev().children().attr('src','../imagens/aba_esq_sel.gif');
	    	$('a[title="Unidade Formativa VI"]').parent().attr('background','../imagens/aba_fundo_sel.gif');
	    	$('a[title="Unidade Formativa VI"]').parent().next().children().attr('src','../imagens/aba_dir_sel_fim.gif');
	    	$('a[title="Unidade Formativa VI"]').css('color','#000055').css('text-decoration', 'none');
    	}
    },
    
    marcaPrimeiroFilho : function( ){
    	if( BancoDeQuestoes.acao == 'B' ){
    		$('a[title^="Ci�ncias da Natureza"]').parent().prev().children().attr('src','../imagens/aba_esq_sel.gif');
    		$('a[title^="Ci�ncias da Natureza"]').parent().attr('background','../imagens/aba_fundo_sel.gif');
    		$('a[title^="Ci�ncias da Natureza"]').parent().next().children().attr('src','../imagens/aba_dir_sel_fim.gif');
    		$('a[title^="Ci�ncias da Natureza"]').css('color','#000055').css('text-decoration', 'none');
    	}  
    }
};

/**
 * Submete o formul�rio
 * @name enviaArquivos
 * @return void
 */
function enviaArquivos(  )
{
   var formulario = $('#formulario');
   var arquivo    = document.getElementById('arquivo').value;
   var meuerro 	  = "";
   
   if ( !arquivo ) {
      //Se n�o tenho arquivo, � porque n�o foi selecionado um arquivo no formul�rio.
        meuerro = "Por favor, selecionar o arquivo que deseja anexar para realizar essa a��o.";
        alert( meuerro );
   }else{
		formulario.submit();
   }
   //se estou aqui � porque n�o se pode submeter
   //return 0;
}
	
function excluirAnexo( arqid ){
 	if ( confirm( 'Deseja excluir este Documento?' ) ) {
 		location.href= window.location+'&arqidDel='+arqid;
 	}
}