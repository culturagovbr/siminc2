 /*
 *
 */
function abrePopLista(nome, titulo)
{
	var popUp = window.open('../geral/popLista.php?nome='+nome+'&titulo='+titulo+'', 'popLista', 'height=500,width=400,scrollbars=yes,top=50,left=200');
	popUp.focus();
}

/*
 * Classe   Calculo
 * O obj cï¿½lculo possibilita operaï¿½ï¿½es aritmï¿½ticas e lï¿½gicas em nï¿½meros no formato monetï¿½rio (###.###,##).
 *
 * @author  Felipe Tarchiani Cerï¿½volo Chiavicatti
 * @since   07/04/2010
 * @exemple /simec/obras/modulos/principal/popUpInserirAditivo.inc
 * @link    http://simec-local/obras/obras.php?modulo=principal/popUpInserirAditivo&acao=A
 */
function Calculo(){
	this.operacao = function (num1, num2, operador, decimal){
		operador = operador ? operador : "+";
		decimal  = decimal ? decimal : 2;

		if ( isNaN( num1 ) == true ){
			num1 = this.converteMonetario(num1); //new Number( replaceAll( replaceAll( num1, ".", "" ), ",", ".") );
		}else{
			num1 = new Number( num1 );
		}

		if ( isNaN( num2 ) == true ){
			num2 = this.converteMonetario(num2); //new Number( replaceAll( replaceAll( num2, ".", "" ), ",", ".") );
		}else{
			num2 = new Number( num2 );
		}

		switch(operador) {
			case '+':
				return (num1 + num2).toFixed(decimal);
				break;
			case '-':
				return (num1 - num2).toFixed(decimal);
				break;
			case '*':
				return (num1 * num2).toFixed(decimal);
				break;
			case '/':
				return (num1 / num2).toFixed(decimal);
				break;
		}
		alert('A funï¿½ï¿½o "operacao" nï¿½o contempla AINDA o operador aritmï¿½tico escolhido.');
	}

	this.comparar = function (num1, num2, operador){
		operador = operador ? operador : '>';

//		num1 = this.converteMonetario(num1); //new Number( replaceAll( replaceAll( num1, ".", "" ), ",", ".") );
//		num2 = this.converteMonetario(num2); //new Number( replaceAll( replaceAll( num2, ".", "" ), ",", ".") );
		if ( isNaN( num1 ) == true ){
			num1 = this.converteMonetario(num1); //new Number( replaceAll( replaceAll( num1, ".", "" ), ",", ".") );
		}else{
			num1 = new Number( num1 );
		}

		if ( isNaN( num2 ) == true ){
			num2 = this.converteMonetario(num2); //new Number( replaceAll( replaceAll( num2, ".", "" ), ",", ".") );
		}else{
			num2 = new Number( num2 );
		}

		switch(operador) {
			case '>':
				return (num1 > num2);
				break;
			case '>=':
				return (num1 >= num2);
				break;
			case '<':
				return (num1 < num2);
				break;
			case '<=':
				return (num1 <= num2);
				break;
			case '=':
				return (num1.valueOf() == num2.valueOf());
				break;
		}
		alert('A funï¿½ï¿½o "comparar" nï¿½o contempla AINDA o operador lï¿½gico escolhido.');

	}

	this.converteMonetario = function (num){
		return new Number( replaceAll( replaceAll( num, ".", "" ), ",", ".") );
	}

	this.retiraZeroEsquerda = function (num){
		num = new String( replaceAll( replaceAll( num, ".", "" ), ",", "") );

		return new Number( num );
	}

	/* Funï¿½ï¿½o para subustituir todos
	* descscricao 	:
	 * author 		: Alexandre Dourado
	 * parametros 	:
	 */
	function replaceAll(str, de, para){
	    var pos = str.indexOf(de);
	    while (pos > -1){
			str = str.replace(de, para);
			pos = str.indexOf(de);
		}
	    return (str);
	}

}

/*
 * Classe   Data
 * O obj Data possibilita operaï¿½ï¿½es aritmï¿½ticas (Adicionar dias) e lï¿½gicas em datas no formato Brasileiro (DD/MM/YYYY).
 *
 * @author  Felipe Tarchiani Cerï¿½volo Chiavicatti
 * @since   07/04/2010
 * @exemple /simec/obras/modulos/principal/popUpInserirAditivo.inc
 * @link    http://simec-local/obras/obras.php?modulo=principal/popUpInserirAditivo&acao=A
 */
function Data(){

	/**
	 * Compara as duas datas,
	 * O terceiro parametro indica o operador lï¿½gico de comparaï¿½ï¿½o, o padrï¿½o ">", Ex. data1 > data2.
	 * retorna o boleano resultante da operaï¿½ï¿½o de comparaï¿½ï¿½o
	*/
	this.comparaData = function (data1, data2, operador){
		var arrData1, arrData2;
		operador = operador ? operador : '>';

		arrData1 = preparaData( data1 );
		arrData2 = preparaData( data2 );

		data1 = new Date(arrData1[2], arrData1[1], arrData1[0]);
		data2 = new Date(arrData2[2], arrData2[1], arrData2[0]);

		switch(operador) {
			case '>':
				return (data1 > data2);
				break;
			case '>=':
				return (data1 >= data2);
				break;
			case '<':
				return (data1 < data2);
				break;
			case '<=':
				return (data1 <= data2);
				break;
			case '=':
				return (data1.getTime() == data2.getTime());
				break;
		}
		alert('A funï¿½ï¿½o "comparaData" nï¿½o contempla AINDA o operador lï¿½gico escolhido.');
	}

	this.dtAddDia = function (data, dias){
		if ( !isNaN(dias) ){
			dias = new Number(dias);
		}else{
			dias = 0;
		}
		arrData    = preparaData( data ); //data.split('/');

		var obDt = new Date(arrData[2], arrData[1], (arrData[0] + dias ));
		return montaData( obDt );
	}

	preparaData = function (data){
		arrData    = data.split('/');
		arrData[0] = new Number( arrData[0] );
		// ï¿½ necessï¿½rio diminuir 1, por que a contagem dos meses vai de 0 a 11
		arrData[1] = (new Number( arrData[1] ) - 1);
		arrData[2] = new Number( arrData[2] );

		return arrData;
	}

	montaData = function (data){
		dia = numComZero(data.getDate().toString(), 2);
		// ï¿½ necessï¿½rio aumentar 1, para a formataï¿½ï¿½o ficar correta, pois os meses vï¿½o de 0 a 11, nï¿½o de 1 a 12.
		mes = new String(data.getMonth() + 1);
		mes = numComZero(mes, 2)

		return dia + "/" + mes + "/" + data.getFullYear();
	}

	numComZero = function (num, quantZero){
		var ini = quantZero - num.length;
		while ( ((quantZero - num.length) >= 0) && (ini > 0) ){
			num = '0' + num;
			quantZero--;
		}
		return num;
	}
}

function extrairScript(texto) {
		//desenvolvido por Skywalker.to, Micox e Pita.
		//http://forum.imasters.uol.com.br/index.php?showtopic=165277
		var ini, pos_src, fim, codigo, fimTag, texto2, codAnt;
		var objScript = null;
		ini = texto.indexOf('<script', 0)
		while (ini!=-1){
			var objScript = document.createElement("script");
			//Busca se tem algum src a partir do inicio do script
			pos_src = texto.indexOf(' src', ini)
			ini = texto.indexOf('>', ini) + 1;
			//Verifica se este e um bloco de script ou include para um arquivo de scripts
			if (pos_src < ini && pos_src >=0){//Se encontrou um "src" dentro da tag script, esta e um include de um arquivo script
				//Marca como sendo o inicio do nome do arquivo para depois do src
				fimTag = ini;
				ini    = pos_src + 4;

				texto2 = texto.substring(ini,fimTag);
				//Procura pelo ponto do nome da extencao do arquivo e marca para depois dele
				fim = texto2.lastIndexOf('.', ini+4)+4;
				//Pega o nome do arquivo
				codigo = texto2.substring(0,fim);

				//Procura pelo ponto do nome da extencao do arquivo e marca para depois dele
				fim = texto.indexOf('.', ini+4)+4;
				//Pega o nome do arquivo
		//		codigo = texto.substring(ini,fim);
				//Elimina do nome do arquivo os caracteres que possam ter sido pegos por engano
				codigo = codigo.replace("=","").replace(" ","").replace("\"","").replace("\"","").replace("\'","").replace("\'","").replace(">","");
				// Adiciona o arquivo de script ao objeto que sera adicionado ao documento
				objScript.src  = codigo;
				objScript.type = "text/javascript";
			}else{
			//Se nao encontrou um "src" dentro da tag script, esta e um bloco de codigo script
				// Procura o final do script
				fim = texto.indexOf('</script', ini);
				// Extrai apenas o script
				codigo = texto.substring(ini,fim);
				// Adiciona o bloco de script ao objeto que sera adicionado ao documento
				objScript.text = codigo;
			}

			//Adiciona o script ao documento
			document.body.appendChild(objScript);

			// Procura a proxima tag de <script>
			ini = texto.indexOf('<script', fim);

			//Limpa o objeto de script
			objScript = null;
		}
	}
/*
 * function divCarregando
 * Cria e insere no nï¿½ de elementos uma DIV transparente com uma imagem de carregando
 * P.S.: Para esconder essa DIV use a funï¿½ï¿½o "divCarregado", apï¿½s a execuï¿½ï¿½o de suas operaï¿½ï¿½es.
 *
 * @author  Felipe Tarchiani Cerï¿½volo Chiavicatti
 * @access  public
 * @since   11/05/2010
 * @param   string|object id - Deve conter o id ou objeto onde se estï¿½ clicando a fim de montar a imagem "carregando"
 							   tomando como referï¿½ncia o elemento encontrado por esse paramentro (nï¿½o obrigatï¿½rio).

 						  texto - No parametro texto pode ser passado o texto que aparecerï¿½ em tela. Nï¿½o ï¿½ obrigatï¿½rio,
 						  		aparecendo o texto "Carregando..." caso nï¿½o passado.
 * @return  void
 * @exemple /simec/obras/modulos/principal/supervisao/listaOS.inc
 * @link    http://simec-local/obras/obras.php?modulo=principal/supervisao/listaOS&acao=A
 */
 function divCarregando(id, texto){
	var d = document;
	var div, span, img, h, w, topImg, elementRefe;
	//var j = jQuery.noConflict();
	id = id ? id : '';
	texto = texto ? texto : 'Carregando...';
	h = d.body.scrollHeight;
	w = d.body.scrollWidth;

	elementRefe = typeof(id) != 'object' ? d.getElementById(id) : id;
	h = h < screen.height ? screen.height : h;

	if (elementRefe){
		topImg = findPosY(elementRefe);
	}else{
		topImg = (h/4);
	}

	if (!jQuery("#temporizador1")[0]){
		div = d.createElement("div");
	}else{
		div = jQuery("#temporizador1");
		jQuery(div).remove(div);
	}

	// Monta Span
	if (jQuery("span", div).length == 0){
		span = d.createElement("span");
	}else{
		span = jQuery("span", div).eq(0);
		jQuery("span", div).remove();
	}

	jQuery(span).attr({
		id : 'spanCarregando'
	})
	.css({
		'position' : 'relative',
		'top'	   : topImg + 'px'
	})
	.append('<center>'+texto+'</center>');

	// Monta Imagem
	if (jQuery("img", span).length == 0){
		img = d.createElement("img");
	}else{
		img = jQuery("img", span).eq(0);
		jQuery("img", span).eq(0).remove();
	}

	jQuery(img).attr({
		src : '/imagens/carregando.gif'
	});
	jQuery("center", span).before(img);

	// Insere span com img na div, e prepara a mesma
	jQuery(div).append(span)
	.attr({
		id : 'temporizador1'
	})
	.css({
		'-moz-opacity' : '0.8',
		'filter' 	   : 'alpha(opacity=80)',
		'background'   : '#ffffff',
		'text-align'   : 'center',
		'position' 	   : 'absolute',
		'top' 		   : '0px',
		'left' 		   : '0px',
		'width'		   : w + 'px',
		'height' 	   : h + 'px',
		'z-index' 	   : '1000'
	});

	// Insere no nï¿½ a div
	jQuery(d.body).append(div);

	return;
	//return div;

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

}

/*
 * function divCarregado
 * Retira a DIV que nubla a tela criada pela funï¿½ï¿½o "divCarregando"
 *
 * @author  Felipe Tarchiani Cerï¿½volo Chiavicatti
 * @access  public
 * @since   11/05/2010
 * @return  void
 * @exemple /simec/obras/modulos/principal/supervisao/listaOS.inc
 * @link    http://simec-local/obras/obras.php?modulo=principal/supervisao/listaOS&acao=A
 */
function divCarregado(){
	//var j = jQuery.noConflict();
	jQuery('#temporizador1').hide(300, function (){jQuery('#temporizador1').remove();});
}

function pegaRetornoAjax(tagIni, tagFim, html, somenteSeEncontrar){
	somenteSeEncontrar = somenteSeEncontrar ? somenteSeEncontrar : false;
	var tagIniTam = (tagIni ? tagIni.length : 0);
   	var iniReturn = html.indexOf( tagIni );
   	var fimReturn = html.lastIndexOf( tagFim );

   	// Se "somenteSeEncontrar" = true e caso nï¿½o encontre a "tagIni" e "tagFim" retorna false;
   	if ( somenteSeEncontrar == true && (iniReturn < 0 || fimReturn < 0) ){ return false }

   	iniReturn = iniReturn > -1 ? iniReturn : 0;
   	iniReturn = (iniReturn + tagIniTam);
	fimReturn = fimReturn ? fimReturn : html.length;

	return html.substring(iniReturn, fimReturn);
}

function sleep(milliseconds) {
  var start = new Date().getTime();
  for (var i = 0; i < 1e7; i++) {
    if ((new Date().getTime() - start) > milliseconds){
      break;
    }
  }
}


function debug(obj){
       var janela = window.open()
       for(prop in obj){
         janela.document.write(prop + ' = '+ obj[prop]+'<BR>');
       }

 }


/*
   Sistema Sistema Simec
   Setor responsï¿½vel: SPO/MEC
   Analista: Cristiano Cabral
   Programador: Cristiano Cabral (e-mail: cristiano.cabral@gmail.com)
   Mï¿½dulo: valida.js
   Finalidade: Funï¿½ï¿½es de validaï¿½ï¿½o em Javascript
   Data de criaï¿½ï¿½o: 03/08/2005
*/


function emailPara( cpf ){
	e = "/geral/emailPara.php?cpf=" + cpf;
	var j = window.open(e, "emailPara","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=530,height=490");
	j.focus();
}


/**
 * @function validaForm();
 * @descscricao : Valida o formulï¿½rio passado.
 * @author 		: Thiago Tasca Barbosa
 * @data		: 13/05/2009
 * @vesrion     : 1.0
 * @return		: true ou false e um alert com os erros caso exista erros.
 * @tutorial	: Existem 4 parametros:
 *					1ï¿½ parametro: string com nome do formulï¿½rio.
 *					2ï¿½ parametro: string ou array com os campos que deseja que seja validado.  Campos separados pelo caracter # se for passado como string.
 *					3ï¿½ parametro: string ou array com os tipos de dados do parametro anterior. Campos separados pelo caracter # se for passado como string.
 *					4ï¿½ parametro: boleano indica se funï¿½ï¿½o irï¿½ subimeter o formulï¿½rio ou nï¿½o.
 *				  OBS: O parametro tipoCampos pode receber: numero, cpf, valor, data, checkbox, select, funcao, radio, texto
 *					   Sendo que:   se o tipo for numero irï¿½ validar um campo numerico.
 *					   				se o tipo for textarea irï¿½ validar um campo de texto grande.
 *					   				se o tipo for texto irï¿½ verificar se o campos estï¿½ prenchido.
 *									se o tipo for cpf irï¿½ validar se o cpf e valido.
 *									se o tipo for valor irï¿½ validar valores numericos.
 * 									se o tipo for data irï¿½ validar um campo data.
 * 									se o tipo for checkbox pelo menos 1 checkbox terï¿½ que estar marcado.
 *									se o tipo for radio pelo menos 1 radio terï¿½ que estar marcado.
 * 									se o tipo for select irï¿½ validar se o select estï¿½ selecionado.
 *									se o tipo for funcao deve-se passar da seguinte forma funcao:nomeFuncao(paremetros).
 						O tipo funï¿½ï¿½o server para validar um campo com uma funï¿½ï¿½o exclusiva criada pelo desenvolvedor.
 *
 * @exemplo1	: onclick="validaForm('formulario',
 *									  'conta#nacimento#contrapartida#existe#informe#informe2#informe3',
 *									  'numero#data#radio#radio#checkbox#select#select',
 *									  false );"
 *
 *
 * @exemplo2	:function salvarDados(){
 *						var nomeform 		= 'formulario';
 *
 *						var campos 			= new Array();
 *						campos[0] 			= "conta";
 *						campos[1] 			= "agencia";
 *						campos[2] 			= "teste3";
 *						campos[3] 			= "saldoinicio";
 *
 *						var tiposDeCampos 	= new Array();
 *						tiposDeCampos[0] 	= "numero";
 *						tiposDeCampos[1] 	= "data";
 *						tiposDeCampos[2] 	= "funcao:teste(parametro1)";
 *						tiposDeCampos[3] 	= "cpf";
 *
 *						var submeterForm 	= false;
 *						validaForm(nomeform, campos, tiposDeCampos, submeterForm );
 *					}
 *
 * @exemplo3	: function salvarDados(){
 *						var nomeform 		= 'formulario';
 *						var campos 			='conta#agencia#contrapartida#contrapartidaa#teste3#teste2#teste5';
 *						var tiposDeCampos 	= 'numero#data#radio#radio#checkbox#select#select';
 *						var submeterForm 	= true;
 *						validaForm(nomeform, campos, tiposDeCampos, submeterForm );
 *				  }
 *
 * @exemplo4    : function salvarDados(){
 *						var nomeform 		= 'formulario';
 *						var submeterForm 	= true;
 *						campos 				= new Array("anoreferencia","prgplanointerno");
 *						tiposDeCampos 		= new Array("select","texto");
 *
 *						validaForm(nomeform, campos, tiposDeCampos, submeterForm );
 *					}
 */
function validaForm(nomeFormulario, campos, tipoCampos, submeter){
	if(typeof(campos) == "object"){
		var dados 		= campos;
	}else{
		var dados 		= campos.split("#");
	}

	if(typeof(tipoCampos) == "object"){
		var tiposDados 	= tipoCampos;
	}else{
		var tiposDados 	= tipoCampos.split("#");
	}

	var formulario 			= eval('document.'+nomeFormulario);
	var erro 				= new Array();
	var marcadoRadio 		= new Array();
	var erroRadio   		= new Array();
	var campoRadio			= new Array();
	var arerroFuncao		= new Array();
	var existeErro 			= false;
	var checarCheckbox		= false;
	var erroCheckbox		= true;
	var alerta 				= "";
	var contValidaRadio 	= "";
	var erroRadioMostra 	= "";
	var mostraErroCheckbox 	= "";
	var erroFuncao			= "";
	var contErro			= 0;
	var contRadio   		= 0;
	var validaContRadio 	= 0;
	var contRadioCampo 		= 0;
	var erroFuncaoMostra    = '';
	var contfuncao			= 0;
	var strValor			= '';
	for (cont=0; cont<dados.length; cont++){
		for (i=0;i<formulario.elements.length;i++){
			if(formulario.elements[i].name == dados[cont]){
				var strValor = Trim(formulario.elements[i].value);
				if(formulario.elements[i].type == "textarea"){
					if(tiposDados[cont] == "textarea"){
						if(!strValor){
							existeErro = true;
							erro[contErro] = "O campo "+formulario.elements[i].title+" ï¿½ obrigatï¿½rio.";
							contErro++;
						}
					}
				}else if(formulario.elements[i].type == "text"){
					if(tiposDados[cont] == "numero"){
						if(!strValor){
							existeErro = true;
							erro[contErro] = "O campo "+formulario.elements[i].title+" ï¿½ obrigatï¿½rio.";
							contErro++;
						}else if(isNaN(strValor)){
							existeErro = true;
							erro[contErro] = "O campo "+formulario.elements[i].title+" sï¿½ aceita valores numï¿½ricos.";
							contErro++;
						}
					}else if(tiposDados[cont] == "cpf"){
						if(!strValor){
							existeErro = true;
							erro[contErro] = "O campo "+formulario.elements[i].title+" ï¿½ obrigatï¿½rio.";
							contErro++;
						}else if(!validar_cpf(strValor)){
							existeErro = true;
							erro[contErro] = "O CPF do campo "+formulario.elements[i].title+" nï¿½o ï¿½ valido.";
							contErro++;
						}
					}else if(tiposDados[cont] == "texto"){
						if(!strValor){
							existeErro = true;
							erro[contErro] = "O campo "+formulario.elements[i].title+" ï¿½ obrigatï¿½rio.";
							contErro++;
						}
					}
					if(tiposDados[cont] == "valor"){
						var valor = strValor.replace(".", "");
						var valor = valor.replace(",", "");
						if(!strValor){
							existeErro = true;
							erro[contErro] = "O campo "+formulario.elements[i].title+" ï¿½ obrigatï¿½rio.";
							contErro++;
						}else if(isNaN(valor)){
							existeErro = true;
							erro[contErro] = "O campo "+formulario.elements[i].title+"  sï¿½ aceita valores numï¿½ricos.";
							contErro++;
						}
					}
					if(tiposDados[cont] == "data"){
						if(!strValor){
							existeErro = true;
							erro[contErro] = "O campo "+formulario.elements[i].title+" ï¿½ obrigatï¿½rio.";
							contErro++;
						}else if(!validaData(formulario.elements[i])){
							existeErro = true;
							erro[contErro] = "O campo "+formulario.elements[i].title+" nï¿½o e uma data.";
							contErro++;
						}
					}
				}else if(formulario.elements[i].type == "radio"){
					if(tiposDados[cont] == "radio"){
						if(cont != contValidaRadio){
							contValidaRadio 				= cont;
							contRadio++;
							campoRadio[contRadio] 	= formulario.elements[i].title;
							marcadoRadio[contRadio] 	= false;
						}
						if(formulario.elements[i].checked == true ){
							marcadoRadio[contRadio] = true;
						}
					}
				}else if(formulario.elements[i].type == "checkbox"){
					if(tiposDados[cont] == "checkbox"){
						var checarCheckbox = true;
						if (formulario.elements[i].checked == true && erroCheckbox == true ) {
							erroCheckbox = false;
						}else{
							var nomeCheckbox = formulario.elements[i].title;
						}
					}
				}else if(formulario.elements[i].type ==  "select-one"){
					if(tiposDados[cont] == "select"){
						if(!strValor){
							existeErro = true;
							erro[contErro] = "O campo "+formulario.elements[i].title+" ï¿½ obrigatï¿½rio.";
							contErro++;
						}
					}
				}

				var tipo = tiposDados[cont].split(":");
				if(tipo[0] == "funcao" ){
					var funcao = eval(tipo[1]);
					if(funcao == true) {
						var ok = 1;
					}else if(funcao == false) {
						existeErro = true;
					} else {
						existeErro = true;
						arerroFuncao[contfuncao] = funcao;
						contfuncao++;
					}
				}
			}
		}
	}

	if(erroCheckbox && checarCheckbox ){
		existeErro = true;
	}
	for(conta=1; conta<campoRadio.length; conta++){
		if(marcadoRadio[conta] == false){
			existeErro = true;
		}
	}
	if(existeErro){
		if(erroCheckbox && checarCheckbox ){
			mostraErroCheckbox = "ï¿½ obrigatï¿½rio selecionar pelo menos um dado do campo : "+nomeCheckbox;
		}
		for(conta=1; conta<campoRadio.length; conta++){
			if(marcadoRadio[conta] == false){
				erroRadio[contRadioCampo] = "ï¿½ obrigatï¿½rio selecionar pelo menos um dado do campo : "+campoRadio[conta];
				contRadioCampo++;
			}
		}
		for(contaa=0; contaa<erro.length; contaa++){
			alerta += erro[contaa]+" \n";
		}
		for(contn=0; contn<erroRadio.length; contn++){
			erroRadioMostra += erroRadio[contn]+" \n";
		}
		for(contf=0; contf<arerroFuncao.length; contf++){
			erroFuncaoMostra += arerroFuncao[contf]+" \n";
		}

		alerta = alerta+erroRadioMostra+mostraErroCheckbox+erroFuncaoMostra;
		if(alerta){
			alert(alerta);
			return false;
		}else{
			return false;
		}
	}else{
		if(!submeter){
			return true;
		}else{
			formulario.submit();
		}
	}
}

function Trim(str){return str.replace(/^\s+|\s+$/g,"");}

/*
objetivo: mascarar de acordo com a mascara passada
caracteres: # - caracter a ser mascarado
           | - separador de mascaras
modos (exemplos):
mascara simples: "###-####"	                 mascara utilizando a mascara passada
mascara composta: "###-####|####-####"       mascara de acordo com o tamanho (length) do valor passado
mascara dinï¿½mica: "[###.]###,##"             multiplica o valor entre colchetes de acordo com o length do valor para que a mascara seja dinï¿½mica ex: ###.###.###.###,##
utilizar no onkeyup do objeto
ex: onkeyup="this.value = mascara_global('#####-###',this.value);"
tratar o maxlength do objeto na pï¿½gina (a funï¿½ï¿½o nï¿½o trata isso)
*/

function mascaraglobal(mascara, valor){

        var mascara_utilizar;
        var mascara_limpa;
        var temp;
        var i;
        var j;
        var caracter;
        var separador;
        var dif;
        var validar;
        var mult;
        var ret;
        var tam;
        var tvalor;
        var valorm;
        var masct;
        tvalor = "";
        ret = "";
        caracter = "#";
        separador = "|";
        mascara_utilizar = "";
        valor = trim(valor);
        if (valor == "")return valor;
        temp = mascara.split(separador);
        dif = 1000;

        valorm = valor;
        //tirando mascara do valor jï¿½ existente
        for (i=0;i<valor.length;i++){
                if (!isNaN(valor.substr(i,1))){
                        tvalor = tvalor + valor.substr(i,1);
                }
        }
        valor = tvalor;

        //formatar mascara dinamica
        for (i = 0; i<temp.length;i++){
                mult = "";
                validar = 0;
                for (j=0;j<temp[i].length;j++){
                        if (temp[i].substr(j,1) == "]"){
                                temp[i] = temp[i].substr(j+1);
                                break;
                        }
                        if (validar == 1)mult = mult + temp[i].substr(j,1);
                        if (temp[i].substr(j,1) == "[")validar = 1;
                }
                for (j=0;j<valor.length;j++){
                        temp[i] = mult + temp[i];
                }
        }


        //verificar qual mascara utilizar
        if (temp.length == 1){
                mascara_utilizar = temp[0];
                mascara_limpa = "";
                for (j=0;j<mascara_utilizar.length;j++){
                        if (mascara_utilizar.substr(j,1) == caracter){
                                mascara_limpa = mascara_limpa + caracter;
                        }
                }
                tam = mascara_limpa.length;
        }else{
                //limpar caracteres diferente do caracter da mï¿½scara
                for (i=0;i<temp.length;i++){
                        mascara_limpa = "";
                        for (j=0;j<temp[i].length;j++){
                                if (temp[i].substr(j,1) == caracter){
                                        mascara_limpa = mascara_limpa + caracter;
                                }
                        }

                        if (valor.length > mascara_limpa.length){
                                if (dif > (valor.length - mascara_limpa.length)){
                                        dif = valor.length - mascara_limpa.length;
                                        mascara_utilizar = temp[i];
                                        tam = mascara_limpa.length;
                                }
                        }else if (valor.length < mascara_limpa.length){
                                if (dif > (mascara_limpa.length - valor.length)){
                                        dif = mascara_limpa.length - valor.length;
                                        mascara_utilizar = temp[i];
                                        tam = mascara_limpa.length;
                                }
                        }else{
                                mascara_utilizar = temp[i];
                                tam = mascara_limpa.length;
                                break;
                        }
                }
        }

        //validar tamanho da mascara de acordo com o tamanho do valor
        if (valor.length > tam){
                valor = valor.substr(0,tam);
        }else if (valor.length < tam){
                masct = "";
                j = valor.length;
                for (i = mascara_utilizar.length-1;i>=0;i--){
                        if (j == 0) break;
                        if (mascara_utilizar.substr(i,1) == caracter){
                                j--;
                        }
                        masct = mascara_utilizar.substr(i,1) + masct;
                }
                mascara_utilizar = masct;
        }

        //mascarar
        j = mascara_utilizar.length -1;
        for (i = valor.length - 1;i>=0;i--){
                if (mascara_utilizar.substr(j,1) != caracter){
                        ret = mascara_utilizar.substr(j,1) + ret;
                        j--;
                }
                ret = valor.substr(i,1) + ret;
                j--;
        }
        return ret;
}

/**
 * Autor: Luciano F. Ribeiro
 * Abre a janela de interaï¿½ï¿½o do select_popup. Instruï¿½ï¿½es no arquivo funcoes.inc
 * @param string nome
 * @param integer height
 * @param integer width
 * @return void
 */
function select_popup_abre_janela( nome, height, width ){
    var campo_select = document.getElementById( nome );

    for ( var i = 0; i < campo_select.options.length; i++ ){
        campo_select.options[i].selected = false;
    }
    //window.open( '../geral/combopopup.php?nome=' + nome, nome, "height=" + height +  ",width=" + width +  ",scrollbars=yes,top=50,left=200" );
    a = window.open( '../geral/selectpopup.php?nome=' + nome, 'Selectpopup', "height=" + height +  ",width=" + width +  ",scrollbars=yes,top=200,left=450" );
    a.focus();
}

/**
 * Abre a janela de interaï¿½ï¿½o do combo_popup. Veja restante dos
 * comentï¿½rio do arquivo www/geral/combopopup.php
 *
 * @param string nome
 * @param integer height
 * @param integer width
 * @param string funcao
 * @return void
 */
function combo_popup_abre_janela( nome, height, width, funcao )
{
	var campo_select = document.getElementById( nome );
	for ( var i = 0; i < campo_select.options.length; i++ )
	{
		campo_select.options[i].selected = false;
	}

	if(funcao != false)
		funcao = '&funcao=' + funcao;
	else
		funcao = '';

	//window.open( '../geral/combopopup.php?nome=' + nome, nome, "height=" + height +  ",width=" + width +  ",scrollbars=yes,top=50,left=200" );
	a = window.open( '../geral/combopopup.php?nome=' + nome + funcao, 'Combopopup', "height=" + height +  ",width=" + width +  ",scrollbars=yes,top=50,left=200" );
	a.focus();
}

function combo_popup_alterar_campo_busca( campo_select )
{
	var campo_busca_id = 'combopopup_campo_busca_' + campo_select.id;
	var campo_busca = document.getElementById( campo_busca_id );
	if ( !campo_busca )
	{
		return;
	}
	var selecionados = 0
	var opcao = null;
	for ( var i = 0; i < campo_select.options.length; i++ )
	{
		if ( campo_select.options[i].selected )
		{
			selecionados++;
			opcao = campo_select.options[i]
		}
	}
	if ( selecionados != 1 )
	{
		return;
	}
	campo_busca.value = opcao.value;
}

function combo_popup_keypress_buscar_codigo( event, nome, valor )
{
	if ( event.keyCode == 13 )
	{
		combo_popup_buscar_codigo( nome, valor );
	}
}

function combo_popup_recebe_buscar_codigo( input, nome )
{
	httpRequest = input.httpRequest;
	if ( httpRequest.readyState == 4 )
	{
		if ( httpRequest.status == 200 )
		{
			var dados = httpRequest.responseText.split( "\n" );
			if ( dados.length == 2 )
			{
				combo_popup_adicionar_item( nome, dados[0], dados[1], true );
				input.value = '';
			}
			else
			{
				alert( 'Nenhum registro encontrado com o cï¿½digo indicado.' );
			}
			input.focus();
		}
	}
}

function combo_popup_codigo_selecionado( nome, cod )
{
	var campo_select = document.getElementById( nome );
	var j = campo_select.length;
	for( var i = 0; i < j; i++ )
	{
		if ( cod == campo_select.options[i].value )
		{
			return true;
		}
	}
	return false;
}

function add_texto_2_areatexto(texto, areatexto){
	if (!(texto == '' || areatexto == '') ){

	}
}

function combo_popup_buscar_codigo( nome, cod )
{
	if ( !cod )
	{
		return;
	}
	var input = document.getElementById( 'combopopup_campo_busca_' + nome );
	if ( combo_popup_codigo_selecionado( nome, cod ) == true )
	{
		input.value = '';
		input.focus();
		return;
	}
	var input = document.getElementById( nome );
	var maximo = input.getAttribute( 'maximo' ) - 0;
	if ( maximo > 0 ) {
		var quantidade = input.options.length;
		if ( quantidade > 1 || input.options[0].value != '' ) {
			if ( quantidade >= maximo ) {
				alert( 'A quantidade mpaxima de itens que podem ser selecionados ï¿½ ' + maximo );
				return;
			}
		}
	}
	/*
	for ( i = input.options.length - 1; i >= 0; i-- )
	{
		if ( input.options[i].selected && input.options[i].value == cod )
		{
			alert( 'O item jï¿½ estï¿½ presenta lista' );
			return;
		}
	}
	*/
	input.httpRequest = window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject( 'Microsoft.XmlHttp' );
	input.httpRequest.onreadystatechange = function(){ combo_popup_recebe_buscar_codigo( input, nome ); };
	input.httpRequest.open( 'GET', '../geral/combopopup.php?nome=' + escape( nome ) + '&pegar_dados_item=1&codigo_busca=' + escape( cod ), true );
	input.httpRequest.send( null );
}

/**
 * Abre a janela de interaï¿½ï¿½o do texto_popup. Veja restante dos
 * comentï¿½rio do arquivo www/geral/textopopup.php
 *
 * @param string nome
 * @param integer height
 * @param integer width
 * @return void
 */
function texto_popup_abre_janela( nome, height, width, param )
{
	window.open( '../geral/textopopup.php?nome=' + nome + param, nome, "height=" + height +  ",width=" + width +  ",scrollbars=yes,top=50,left=200" );
}

/**
 * Adiciona um item ao combo popup.
 *
 * @param string nome_combo
 * @param string codigo
 * @param string descricao
 * @param boolean ordenar
 * @return void
 */
function combo_popup_adicionar_item( nome_combo, codigo, descricao, ordenar, naoChamarEvento )
{
	var campo_select = document.getElementById( nome_combo );
	if ( campo_select.options[0].value == '' )
	{
		campo_select.options[0] = null;
	}
	campo_select.options[campo_select.options.length] = new Option( descricao, codigo, false, false );

	var evento = campo_select.getAttribute( 'onpush' );
	if ( evento && !naoChamarEvento )
	{
		eval( evento );
	}

	if ( ordenar == true )
	{
		sortSelect( campo_select );
	}
}

/**
 * Remove um item do combo popup.
 *
 * @param string nome_combo
 * @param string codigo
 * @param boolean ordenar
 * @return void
 */
function combo_popup_remover_item( nome_combo, codigo, ordenar, naoChamarEvento )
{
	var campo_select = document.getElementById( nome_combo );
	var removeu = false;
	for( var i = 0; i <= campo_select.length-1; i++ )
	{
		if ( codigo == campo_select.options[i].value )
		{
			campo_select.options[i] = null;
			removeu = true;
		}
	}
	if ( removeu && !naoChamarEvento )
	{
		var evento = campo_select.getAttribute( 'onpop' );
		if ( evento )
		{
			eval( evento );
		}
	}
	if ( campo_select.options.length == 0 )
	{
		campo_select.options[0] = new Option( 'Duplo clique para selecionar da lista', '', false, false );
	}
	if ( ordenar == true )
	{
		sortSelect( campo_select );
	}
}

/**
 * Remove um item do combo popup.
 *
 * @param string nome_combo
 * @param string codigo
 * @param boolean ordenar
 * @return void
 */
function combo_popup_remover_item_radio( nome_combo, codigo, ordenar, naoChamarEvento )
{
	var campo_select = document.getElementById( nome_combo );
	var removeu = false;
	for( var i = 0; i <= campo_select.length-1; i++ )
	{
		campo_select.options[i] = null;
		removeu = true;
	}
	if ( removeu && !naoChamarEvento )
	{
		var evento = campo_select.getAttribute( 'onpop' );
		if ( evento )
		{
			eval( evento );
		}
	}
	if ( campo_select.options.length == 0 )
	{
		campo_select.options[0] = new Option( 'Duplo clique para selecionar da lista', '', false, false );
	}
	if ( ordenar == true )
	{
		sortSelect( campo_select );
	}
}

/**
 * Adiciona uma lista de itens ao combo popup.
 *
 * @param string nome_combo
 * @param string[][] lista
 * @return void
 */
function combo_popup_adicionar_itens( nome_combo, lista )
{
	var j = lista.length;
	for ( var i =0; i < j; i ++ )
	{
		combo_popup_adicionar_item( nome_combo, lista[i][0], lista[i][1], false, true );
	}
	var campo_select = document.getElementById( nome_combo );
	var evento = campo_select.getAttribute( 'onpush' );
	if ( evento )
	{
		eval( evento );
	}
	sortSelect( campo_select );
}

/**
 * Remove uma lista de itens ao combo popup.
 *
 * @param string nome_combo
 * @param string[][] lista
 * @return void
 */
function combo_popup_remover_itens( nome_combo, lista )
{
	var j = lista.length;
	for ( var i =0; i < j; i ++ )
	{
		combo_popup_remover_item( nome_combo, lista[i][0], false, true, true );
	}
	var campo_select = document.getElementById( nome_combo );
	var evento = campo_select.getAttribute( 'onpush' );
	//alert( evento );
	if ( evento )
	{
		eval( evento );
	}
	sortSelect( campo_select );
}

function combo_popup_remove_selecionados( event, nome_combo )
{
	if( window.event ) // IE
	{
		var keynum = event.keyCode
	}
	else if( event.which ) // Netscape/Firefox/Opera
	{
		var keynum = event.which
	}
	if ( keynum != 46 )
	{
		return;
	}
	var campo_select = document.getElementById( nome_combo );
	for( var i = 0; i <= campo_select.length-1; )
	{
		if ( campo_select.options[i].selected )
		{
			combo_popup_remover_item( nome_combo, campo_select.options[i].value, false, true );
		}
		else
		{
			i++;
		}
	}
	var evento = campo_select.getAttribute( 'onpop' );

	//alert( evento );
	if ( evento )
	{
		eval( evento );
	}
	sortSelect( campo_select );
}

/**
 * Seleciona todos os itens do campo select.
 *
 * @param object campo_select
 * @return void
 */
function selectAllOptions( campo_select )
{
	if ( !campo_select )
	{
		return;
	}
	var j = campo_select.options.length;
	for ( var i = 0; i < j; i++ )
	{
		campo_select.options[i].selected = true;
	}
}


/**
 * Deseleciona todos os itens do campo select.
 *
 * @param object campo_select
 * @return void
 */
function DeselectAllOptions( campo_select )
{
	if ( !campo_select )
	{
		return;
	}
	var j = campo_select.options.length;
	for ( var i = 0; i < j; i++ )
	{
		campo_select.options[i].selected = false;
	}
}

/**
 * Varre formulï¿½rio em busca de campos que precisam de tratamento
 * especial. O campo do tipo 'combo_popup' tem seus itens todos
 * selecionados.
 *
 * @return void
 */
function prepara_formulario()
{
	var quantidade = document.forms.length;
	var quantidade_elementos = 0;
	var elemento = null;
	var j = 0;
	for ( var i = 0; i < quantidade; i++  )
	{
		quantidade_elementos = document.forms[i].elements.length;
		for ( j = 0; j < quantidade_elementos; j++ )
		{
			elemento = document.forms[i].elements[j];
			if ( elemento.getAttribute( 'tipo' ) == 'combo_popup' )
			{
				selectAllOptions( elemento );
			}
		}
	}
}

/**
* Abre popup dos arquivos relacionados ao projeto especial de acordo com a acao (L - listar, I - inserir)
*
* @param string acao
* @param string nome
* @param integer width
* @param integer height
*/
function popup_arquivo( acao, nome, width, height )
{
	window.open( '../geral/popup_arquivo.php?acao=' + acao + '&nome=' + nome, '', "height=" + height +  ",width=" + width +  ",scrollbars=yes,top=50,left=200" );
}

//tirar os espaï¿½os das extremidades do valor passado (utilizada pela mascaraglobal)
function trim(valor){
	valor+='';
        for (i=0;i<valor.length;i++){
                if(valor.substr(i,1) != " "){
                        valor = valor.substr(i);
                        break;
                }
                if (i == valor.length-1){
                        valor = "";
                }
        }
        for (i=valor.length-1;i>=0;i--){
                if(valor.substr(i,1) != " "){
                        valor = valor.substr(0,i+1);
                        break;
                }
        }
        return valor;
}


function validaRadio(campo,label) {
   var radiogroup = campo;
   var itemchecked = false;
   for(var j = 0 ; j < radiogroup.length ; ++j) {
    if(radiogroup[j].checked) {
	 itemchecked = true;
	 break;
	}
   }
   if(!itemchecked) {
    alert("Escolha uma opï¿½ï¿½o para o Campo: "+label+".");
    if(campo[0].focus)
     campo[0].focus();
	return false;
   }
 return true;
}

function validar_radio( campo,label ) {
	var radiogroup = campo;
	var itemchecked = false;
	for( var j = 0 ; j < radiogroup.length ; ++j ) {
		if( radiogroup[j].checked ) {
			itemchecked = true;
			break;
		}
	}
	return itemchecked;
}

function validaEmail(Email) {
        var s = new String(Email);

        // { } ( ) < > [ ] | \ / ' '
        if ((s.indexOf("{")>=0) || (s.indexOf("}")>=0) || (s.indexOf("(")>=0) || (s.indexOf(")")>=0) || (s.indexOf("<")>=0) || (s.indexOf(">")>=0) || (s.indexOf("[")>=0) || (s.indexOf("]")>=0) || (s.indexOf("|")>=0) || (s.indexOf("\\")>=0) || (s.indexOf("/")>=0) || (s.indexOf(" ")>=0) )
        {
        	return false;
        }
        if (vogalAcentuada(Email))
        {
        	return false;
        }
        // & * $ % ? ! ^ ~ ` ' "
        if ((s.indexOf("&")>=0) || (s.indexOf("*")>=0) || (s.indexOf("$")>=0) || (s.indexOf("%")>=0) || (s.indexOf("?")>=0) || (s.indexOf("!")>=0) || (s.indexOf("^")>=0) || (s.indexOf("~")>=0) || (s.indexOf("`")>=0) || (s.indexOf("'")>=0) )
        {
        	return false;
        }
        // , ; : = #
        if ((s.indexOf(",")>=0) || (s.indexOf(";")>=0) || (s.indexOf(":")>=0) || (s.indexOf("=")>=0) || (s.indexOf("#")>=0) )
        {
        	return false;
        }
        // procura se existe apenas um @
        if ( (s.indexOf("@") < 0) || (s.indexOf("@") != s.lastIndexOf("@")) )
        {
        	return false;
        }
        // verifica se tem pelo menos um ponto ap\u00f3s o @
        if (s.lastIndexOf(".") < s.indexOf("@"))
        {
        	return false;
        }
        // verifica se nao termina com um ponto
        if (s.substr(s.length-1,s.length) == ".")
        {
        	return false;
        }
        return true;
}

//verifica se tem vogais acentuadas
function vogalAcentuada(s) {
        ls = s.toLowerCase();
        if ((ls.indexOf("\u00e1")>=0) || (ls.indexOf("\u00e0")>=0) || (ls.indexOf("\u00e3")>=0) || (ls.indexOf("\u00e2")>=0) || (ls.indexOf("\u00e9")>=0) || (ls.indexOf("\u00ed")>=0) || (ls.indexOf("\u00f3")>=0) || (ls.indexOf("\u00f5")>=0) || (ls.indexOf("\u00f4")>=0) || (ls.indexOf("\u00fa")>=0) || (ls.indexOf("\u00fc")>=0))
                return true;
}


/**
 * Verifica campos nï¿½o preenchidos.
 *
 * O campo ï¿½ dito nï¿½o preenchido caso somente o caracter ' '. Verifica
 * tambï¿½m o campo do tipo select, neste caso deve haver pelo menos um
 * item selecionado e nenhum deles deve possuir valor vazio.
 *
 * @param campo
 * @param label
 * @return void
 */
function validaBranco( campo, label )
{
	var i = 0;
	var teste_campo = "false"; //variavel para teste de espacos em branco
	// campo instanceof HTMLSelectElement  dï¿½ problema no Microsoft Internet Explorer
	if ( campo.tagName == 'SELECT' )
	{
		var tamanho_select = campo.options.length;
		var possui_selecionado = false;
		var possui_vazio = false;
		for ( i = 0; i < tamanho_select; i++ )
		{
			if ( campo.options[i].selected )
			{
				possui_selecionado = true;
				if ( campo.options[i].value == '' )
				{
					possui_vazio = true;
				}
			}
		}
		if ( possui_selecionado && !possui_vazio )
		{
			return true;
		}
		alert( "Campo obrigat\u00f3rio: " + label + "." );
		return false;
	}
	else
	{
		var tamanho_campo = campo.value.length;
		if ( tamanho_campo != 0 )
		{
			for ( i = 0; i < tamanho_campo; i++ )
			{
				if ( campo.value.charAt( i ) != " " )
				{
					teste_campo = "true"; // existe caracter diferente de branco
				}
			}
			if ( teste_campo == "false" ) // todos os caracteres digitados sï¿½o brancos
			{
				alert( "Campo obrigat\u00f3rio: " + label + "." );
				campo.focus();
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			alert( "Campo obrigat\u00f3rio: " + label + "." );
			campo.focus();
			return false;
		}
	}
}




function janela(pagina,TW,TH,dest) {
		var j = window.open(pagina, dest, "toolbar=0,location=0,directories=0,status=0,menubar=0,scrollbars=yes"+",width="+TW+",height="+TH);
		j.focus();
 }

function FiltraCampo(codigo) {
    var s = "";

        tam = codigo.length;
        for (i = 0; i < tam ; i++) {
                if (codigo.substring(i,i + 1) == "0" ||
                   codigo.substring(i,i + 1) == "1" ||
            codigo.substring(i,i + 1) == "2" ||
            codigo.substring(i,i + 1) == "3" ||
            codigo.substring(i,i + 1) == "4" ||
            codigo.substring(i,i + 1) == "5" ||
            codigo.substring(i,i + 1) == "6" ||
            codigo.substring(i,i + 1) == "7" ||
            codigo.substring(i,i + 1) == "8" ||
            codigo.substring(i,i + 1) == "9"  )
                                 s = s + codigo.substring(i,i + 1);
        }
        return s;
}



function DvCpfOk(e) {
    var dv = false;
    controle = "";
    s = FiltraCampo(e.value);
    tam = s.length;
    if ( tam == 11 ) {
        dv_cpf = s.substring(tam-2,tam);
        for ( i = 0; i < 2; i++ ) {
            soma = 0;
            for ( j = 0; j < 9; j++ )
                soma += s.substring(j,j+1)*(10+i-j);
            if ( i == 1 ) soma += digito * 2;
            digito = (soma * 10) % 11;
            if ( digito == 10 ) digito = 0;
            controle += digito;
        }
        if ( controle == dv_cpf )
            dv = true;
    }
	if ( s == "11111111111" ) dv = false;
	if ( s == "22222222222" ) dv = false;
 	if ( s == "33333333333" ) dv = false;
	if ( s == "44444444444" ) dv = false;
	if ( s == "55555555555" ) dv = false;
	if ( s == "66666666666" ) dv = false;
	if ( s == "77777777777" ) dv = false;
	if ( s == "88888888888" ) dv = false;
	if ( s == "99999999999" ) dv = false;
	if ( s == "00000000000" ) dv = false;
	if ( s == "00000000191" ) dv = false;
	if ( s == "22525837541" ) dv = false;
	if ( s == "23111478203" ) dv = false;
	if ( s == "28075746708" ) dv = false;
	if ( s == "91087662826" ) dv = false;

	if ( ! dv && tam > 0) {
         mensagem = "           Erro de digita\347\343o:\n";
         mensagem+= "          ===============\n\n";
         mensagem+= " O CPF: " + e.value + " n\343o: existe!\n";
         //mensagem+= " O DV: " + controle + "\n";
         alert(mensagem);
     }
    return dv;
}

function validaData(dataform)
{	//Funcionalidade:	Valida a Data retornando True se for uma Data
        //					vï¿½lida e False se nï¿½o for.
        //					Antes de se usar esta funï¿½ï¿½o deve-se garantir que os parï¿½metros
        //					passados sejam numï¿½ricos e inteiros.
        // PARï¿½METROS:
        //		Dia = Dia da Data(caracteres numericos),
        //		Mes = Mes da Data(caracteres numericos),
        //		Ano = Ano da Data(caracteres numericos)

        //alert(dia +"/"+ mes +"/"+ ano);
		var dia = dataform.value.substring(0,2);
		var mes = dataform.value.substring(3,5);
		var ano = dataform.value.substring(6,10);
        var v_dia;
        var v_mes;
        var v_ano;

        if (!validaInteiro(dia)){
                return (false);
        }
        if (!validaInteiro(mes)){
                return (false);
        }
        if (!validaInteiro(ano)){
                return (false);
        }

        v_dia = dia;
        v_mes = mes;
        v_ano = ano;

        if (v_dia.length < 2)
        {
                return(false);
        }

        if (v_mes.length < 2)
        {
                return(false);
        }

        if (v_ano.length < 4)
        {
                return(false);
        }

        if (((v_ano < 1900) || (v_ano > 2079)) && (v_ano.length != 0))
        {
                return(false);
        }

        if (v_dia > 31 || v_dia < 1)
        {
                return(false);
        }

        if (v_mes > 12 || v_mes < 1)
        {
                return(false);
        }

        if (v_dia == "31")
        {
                if ((v_mes == "04") || (v_mes == "06") || (v_mes == "09") || (v_mes == "11"))
                {
                        return(false);
                }
        }

        //Validaï¿½ï¿½o de Ano Bissexto
        if (v_mes == "02")
        {
                if (!(v_ano%4))
                {
                        if (v_dia > 29)
                        {
                                return(false);
                        }
                }
                else if (v_dia > 28)
                {
                        return(false);
                }
        }

        //o -if- abaixo testa se algum campo foi preenchido e outro deixado em branco deixando a data incompleta

        if (((v_dia != "") || (v_mes != "") || (v_ano != "")) && ((v_dia == "") || (v_mes == "") || (v_ano == "")))
        {
                return(false);
        }

        return(true);
}



function validaInteiro(parametro)  //FUNCAO PARA VALIDACAO DE Nï¿½MEROS INTEIROS, E ESPAï¿½OS EM BRANCO
{
        if (parametro.length != 0)
        {

                teste_ponto = "false";
                tamanho_parametro = parametro.length;

                if (isNaN(parametro)) //valor digitado nï¿½o ï¿½ numï¿½rico
                {
                        return false;
                }
                else //valor digitado ï¿½ um numï¿½rico vï¿½lido
                {

                        for (k = 0; k < tamanho_parametro; k++)
                        {if ((parametro.charAt(k) == '.') || (parametro.charAt(k) == '-') || (parametro.charAt(k) == '+'))
                                {
                                        teste_ponto = "true"; /*existe caracter ponto*/
                                }
                        }

                        if (teste_ponto == "true") //encontrou caracter ponto(numero real)
                        {
                                return false;
                        }
                        else
                        {
                                return true;
                        }
                }
        }
        else
        {
                return true;
        }

}

/* Funï¿½ï¿½o que permite somente a digitaï¿½ï¿½o de nï¿½meros. */

function somenteNumeros(e) {
	if(window.event) {
    	/* Para o IE, 'e.keyCode' ou 'window.event.keyCode' podem ser usados. */
        key = e.keyCode;
    }
    else if(e.which) {
    	/* Netscape */
        key = e.which;
    }
    if(key!=8 || key < 48 || key > 57) return (((key > 47) && (key < 58)) || (key==8) || (key==9));
    {
    	return true;
    }
}

function somenteLetras(e) {
	if(window.event) {
    	/* Para o IE, 'e.keyCode' ou 'window.event.keyCode' podem ser usados. */
        key = e.keyCode;
    }
    else if(e.which) {
    	/* Netscape */
        key = e.which;
    }
    if(key!=8 || key < 65 || key > 122) return (((key > 65) && (key < 122)) || (key==8));
    {
    	return true;
    }
}

// Retorna true se a dataIni maior que a dataFin
function validaDataMaior(dataIni, dataFin)  {

	if ((dataIni.length != 0 && dataIni.length < 10) || (dataFin.length != 0 && dataFin.length < 10))
		return(true);
	else
		if ((dataIni.length != 0 && !validaData(dataIni)) || (dataFin.length != 0 && !validaData(dataFin)))
			return(true);

	dataIni = dataIni.value.substr(6,4) + dataIni.value.substr(3,2) + dataIni.value.substr(0,2);
	dataFin = dataFin.value.substr(6,4) + dataFin.value.substr(3,2) + dataFin.value.substr(0,2);

	if (dataIni <= dataFin)
		return(true);

	return(false);
}

function validaDataMaiorQueHoje(data)  {

		hoje = new Date();
        dia = hoje.getDate();
        if (dia < 10) dia = "0"+dia;

        mes = hoje.getMonth()+1;

        if (mes < 10) mes = "0"+mes;

        ano=hoje.getFullYear();

        diadehoje = ano + '' + mes +''+ dia;
        // alert (diadehoje)  ;
	data = data.value.substr(6,4) + data.value.substr(3,2) + data.value.substr(0,2);
	//alert(data);
	//alert(data + '\n'+diadehoje);
	if (data >= diadehoje)
		return(true);
	else return(false);
}

/**
 * substitui todas as ocorrencias de um string por outro
 * replace all the ocurrence of some string to another
 *
 *@param  string strText
 *@param  string strFinder
 *@param  string strReplacer
 *@return bool
 */
function replaceAll (strText , strFinder, strReplacer)
{
	strText += "";
	var strSpecials = /(\.|\*|\^|\?|\&|\$|\+|\-|\#|\!|\(|\)|\[|\]|\{|\}|\|)/gi; // :D
	strFinder = strFinder.replace(strSpecials, "\\$1")

	var objRe = new RegExp(strFinder, "gi");
	return strText.replace(objRe, strReplacer);
}

//Funï¿½ï¿½o para limitar o tamanho do Textarea
function textCounter(field, countfield, maxlimit) {
	if ( !field || !field.value )
	{
		countfield.value = maxlimit;
		return;
	}
	if (field.value.length > maxlimit)
	field.value = field.value.substring(0, maxlimit);
	else
	countfield.value = maxlimit - field.value.length;
}

// Inicio funï¿½ï¿½es para preencher conteï¿½dos dinamicamente (sem refresh)
var xmlhttp
var idobjeto

abreconteudo = function (url, idobj) {
	var jsNode = document.getElementById('td'+idobj);
	if (document.getElementById('img'+idobj).name=='+')
	{
		try
		{
		strContent = document.getElementById('td'+idobj).innerHTML;
		}
		catch( e )
		{
			throw new Error( 'objeto nao encontrado ' + 'td' + idobj );
		}
		strContent = replaceAll( strContent , "\t" , " ");
		strContent = replaceAll( strContent , String.fromCharCode( 10 ) , " ");
		strContent = trim( strContent );

		if ( strContent == '' || strContent == ' ' ) {

		  	document.getElementById('td'+idobj).style.visibility = "visible";
		  	document.getElementById('td'+idobj).style.display  = "";
			document.getElementById('td'+idobj).innerHTML='<div align=center><img src="../imagens/wait.gif" border="0" align="middle"> <font color=blue>Carregando Dados...</font></div>'

			jsNode._xmlHttp = window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject("Microsoft.XmlHttp");
			jsNode._xmlHttp.open("GET", url, true);
			jsNode._xmlHttp.onreadystatechange = new Function("func_onload(\"" + idobj + "\")");

			// call in new thread to allow ui to update
			window.setTimeout("func_ontimeout(\"" + idobj + "\")", 300);

			document.getElementById('img'+idobj).name='-';
			document.getElementById('img'+idobj).src = document.getElementById('img'+idobj).src.replace('mais.gif', 'menos.gif');
		} else {
			document.getElementById('img'+idobj).name='-';
			document.getElementById('img'+idobj).src = document.getElementById('img'+idobj).src.replace('mais.gif', 'menos.gif');
			document.getElementById('td'+idobj).style.visibility = "visible";
			document.getElementById('td'+idobj).style.display  = "";
		}
	}
	else
	{
		document.getElementById('img'+idobj).name='+';
		document.getElementById('img'+idobj).src = document.getElementById('img'+idobj).src.replace('menos.gif', 'mais.gif');
		document.getElementById('td'+idobj).style.visibility = "hidden";
		document.getElementById('td'+idobj).style.display = "none";
	}


};

func_ontimeout = function (idobj) {var jsNode = document.getElementById('td'+idobj);
									contLoadQueue.add(jsNode);};

func_onload = function (idobj) {
	var jsNode = document.getElementById('td'+idobj);

	if (jsNode._xmlHttp.readyState == 4) {

		if (jsNode._xmlHttp.status==200)
		{
			jsNode.innerHTML='';
			var resp = jsNode._xmlHttp.responseText;
			var jsCode = '';
			if(resp.indexOf('<JSCode>')!=-1) {
				jsCode = resp.substring( ( resp.indexOf('<JSCode>') + 8 ), resp.indexOf('</JSCode>') );
				resp = resp.replace("/<JSCode>(?:\n|\r|.)*?<\/JSCode>/gm", "");
			}
			jsNode.innerHTML=resp;
			if (jsCode != '') {
				try	{
					eval( jsCode );
				}
				catch( e ) {
					alert( e );
				}
			}
		}
		else
		{
			alert("Erro! Nï¿½o foi possï¿½vel Recuperar os Dados!");
			document.getElementById('td'+idobjeto).innerHTML='';
		}

		if (jsNode._xmlHttp.dispose)
			jsNode._xmlHttp.dispose();
		jsNode._xmlHttp = null;
	}
};

var contLoadQueue = {add: function (jsNode) {jsNode._xmlHttp.send(null);}}


// Fim funï¿½ï¿½es para preencher conteï¿½dos dinamicamente (sem refresh)

/////////////////////////////
//Eventos
//Funï¿½ï¿½es para controlar eventos de campos de formulï¿½rio
////////////////////////////
function MouseOver(objeto)
{
	if (objeto.type == "text" || objeto.type == "password"){
		if(objeto.className != 'clsMouseFocus'){
				objeto.className = objeto.className.replace("normal", "clsMouseOver");
		}

	}else if(objeto.type == "textarea"){
		if(objeto.className != 'txareaclsMouseFocus'){
				objeto.className = objeto.className.replace("txareanormal", "txareaclsMouseOver");
		}
	}
	return true;
}

function MouseOut(objeto)
{
	if (objeto.type == "text" || objeto.type == "password")
	{
		if(objeto.className != 'clsMouseFocus'){
					objeto.className = objeto.className.replace("clsMouseOver", "normal");
			}

	}else if(objeto.type == "textarea"){
		if(objeto.className != 'txareaclsMouseFocus'){
				objeto.className = objeto.className.replace("txareaclsMouseOver", "txareanormal");
		}
	}
	return true;
}


function MouseClick(objeto){
	if (objeto.type == "text" || objeto.type == "password"){
		objeto.className = objeto.className.replace("clsMouseOver", "clsMouseFocus");
		objeto.className = objeto.className.replace("normal", "clsMouseFocus");
	}else if(objeto.type == "textarea"){
		objeto.className = objeto.className.replace("txareanormal", "txareaclsMouseFocus");
		objeto.className = objeto.className.replace("txareaclsMouseOver", "txareaclsMouseFocus");
	}
}


function MouseBlur( objeto )
{
	if ( objeto.type == "text" || objeto.type == "textarea" || objeto.type == "password" )
	{
		if ( objeto.type == "textarea")
		{
			objeto.className = objeto.className.replace("txareaclsMouseOver", "txareanormal");
			objeto.className = objeto.className.replace("txareaclsMouseFocus", "txareanormal");
		}
		else
		{
			objeto.className = objeto.className.replace("clsMouseOver", "normal");
			objeto.className = objeto.className.replace("clsMouseFocus", "normal");
		}
	}
}

function sortSelect(obj) {
		var o = new Array();
		if (obj.options==null) { return; }
		for (var i=0; i<obj.options.length; i++) {
			o[o.length] = new Option( obj.options[i].text, obj.options[i].value, obj.options[i].defaultSelected, obj.options[i].selected) ;
			}
		if (o.length==0) { return; }
		o = o.sort(
			function(a,b) {
				if ((a.text+"") < (b.text+"")) { return -1; }
				if ((a.text+"") > (b.text+"")) { return 1; }
				return 0;
				}
			);

		for (var i=0; i<o.length; i++) {
			obj.options[i] = new Option(o[i].text, o[i].value, o[i].defaultSelected, o[i].selected);
			}
}






/* ----------------------------------------------------------------------------
---------------------------------------------------------------------------- */
function Right(pcString,pnTamanho)
{
	var lcRight
	lcRigth = pcString.substr(pcString.length - pnTamanho, pnTamanho)
	return lcRigth
}

/* ----------------------------------------------------------------------------
---------------------------------------------------------------------------- */
function Left(pcString,pnTamanho)
{
	var lcRigth
	lcRigth = pcString.substr(0, pnTamanho)
	return lcRigth
}

/* ----------------------------------------------------------------------------
---------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
---------------------------------------------------------------------------- */
// mï¿½scara para valores monetï¿½rios. Usar passando como paramentro uma string ele retorna
// um string com a formataï¿½ï¿½o padrï¿½o de valores com duas casa decimais.

function MascaraMonetario( objeto )
{
	var j = 1;
	var valorObjeto = objeto.split(".");
	var lcDecimal;


	if(valorObjeto.length > 1) //Possui parte decimal
	{
		lcDecimal = valorObjeto[1];
		lcDecimal = Left(lcDecimal + "00",2);
		lcDecimal = Right("00" + lcDecimal,2);
	}
	else
	{
		lcDecimal = "00";
	}

	var obj = valorObjeto[0] + lcDecimal;

	if ( obj.length > 2 ) obj = obj.substr(0,obj.length - 2) + "," + obj.substr(obj.length - 2,2);
	if ( obj.length > 6 )
		 for (i = obj.length - 1; i >= 0; i--) {
		 	j = obj.length - i;
		 	if (j > 3 && (j + 1)%4 == 0) obj = obj.substr(0,i + 1) + "." + obj.substr(i + 1,obj.length - 1);
		 }
	return obj;

}

/////////////////////////////
//Fim Eventos
//////////////////////////////



//////////////////////////////
// Prototypes
//////////////////////////////

/**
  *	Mï¿½todo adicionado ao objeto Array do javascript para ordenaï¿½ï¿½o de array de objetos
  *
  * @param string campo
  * @param string sentido
  * @param integer isNumeric
  * @return void
  */
  Array.prototype.sortObject = function( campo, sentido, isNumeric )
  {
  	var cmpString = sentido != 'dsc' ? 'jCampo > iCampo' : 'jCampo < iCampo';
	for( var i = 0 ; i < this.length ; i++ )
	{
		for( var j = 0 ; j < this.length ; j++ )
		{
			jCampo = isNumeric ? parseFloat( this[ j ][ campo ] ) : this[ j ][ campo ];
			iCampo = isNumeric ? parseFloat( this[ i ][ campo ] ) : this[ i ][ campo ];
			if( eval( cmpString ) )
			{
				temp = this[ i ];
				this[ i ] = this[ j ];
				this[ j ] = temp;
			}
		}
	}
  }
  /**
  * Mï¿½todo adicionado ao objeto Array do javascript para conversï¿½o de um array de objetos em string
  * Se campo for vazio entï¿½o ele gerarï¿½ um array contendo todos os campos do objeto no registro
  * separador de campo padrï¿½o '#'
  * separador de registro padrï¿½o ','
  *
  * @param string campo
  * @param string separadorReg
  * @param string separadorCampo
  * @return string
  */
  Array.prototype.objectToString = function( campo, separadorReg, separadorCampo )
  {
  	var string = '';
  	sepReg = separadorReg ? separadorReg : ',';
  	sepCampo = separadorCampo ? separadorCampo : '#';
	for( var i = 0 ; i < this.length ; i++ )
	{
		if( campo )
		{
			string += this[ i ][ campo ];
		}
		else
		{
			for( var j in this[ i ] )
			{
				string += this[ i ][ j ] + sepCampo;
			}
			string = string.substr( 0, string.length - 1 );//Eliminar o ï¿½ltimo separador de campo
		}
		string += sepReg;
	}
	string = string.substr( 0, string.length - 1 );//Elimina o ï¿½ltimo separador de registro
  	return string;
  }

function validar_cpf( cpf ){
    var dv = false;
    controle = "";
    s = FiltraCampo( cpf );
    tam = s.length;
    if ( tam == 11 ) {
        dv_cpf = s.substring( tam-2, tam );
        for ( i = 0; i < 2; i++ ) {
            soma = 0;
            for ( j = 0; j < 9; j++ ) {
                soma += s.substring( j, j + 1 ) * ( 10 + i - j );
            }
            if ( i == 1 ) {
            	soma += digito * 2;
            }
            digito = ( soma * 10 ) % 11;
            if ( digito == 10 ) {
            	digito = 0;
            }
            controle += digito;
        }
        if ( controle == dv_cpf ) {
            dv = true;
        }
    }
	if ( s == "11111111111" ) dv = false;
	if ( s == "22222222222" ) dv = false;
 	if ( s == "33333333333" ) dv = false;
	if ( s == "44444444444" ) dv = false;
	if ( s == "55555555555" ) dv = false;
	if ( s == "66666666666" ) dv = false;
	if ( s == "77777777777" ) dv = false;
	if ( s == "88888888888" ) dv = false;
	if ( s == "99999999999" ) dv = false;
	if ( s == "00000000000" ) dv = false;
	if ( s == "12345678909" ) dv = false;
	if ( s == "00000000191" ) dv = false;
	if ( s == "22525837541" ) dv = false;
	if ( s == "23111478203" ) dv = false;
	if ( s == "28075746708" ) dv = false;
	if ( s == "91087662826" ) dv = false;
	return dv;
}




































/**
 * Carrega os municipios ao modificar um combo de UFs
 *
 * @param string Sigla da UF (2 caracteres)
 * @param Object Select onde serï¿½o carregados os municï¿½pios da UF selecionada
 * @return void
 */
function carregarMunicipiosPorUf(regcod, munContainer, endid)
{
    if (!regcod || regcod.length != 2)
        return false;

    var ajax         = window.XMLHttpRequest ? new XMLHttpRequest                    : new window.ActiveXObject( 'Microsoft.XmlHttp' );
    var munContainer = munContainer          ? document.getElementById(munContainer) : document.getElementById('muncod_' + endid);

    while(munContainer.options.length)
        munContainer.options[0] = null;

    munContainer.options[0] = new Option("Selecione um municï¿½pio", "0", false, true);

    ajax.onreadystatechange = function()
    {
        munContainer.setAttribute("disabled", "disabled");

        if (ajax.readyState != 4)
            return;

        eval(ajax.responseText);

        for (var i = 0; i < listaMunicipios[regcod].length; i++) {
            munContainer.options[munContainer.options.length] = new Option(listaMunicipios[regcod][i][1],
                                                                           listaMunicipios[regcod][i][0],
                                                                           false,
                                                                           listaMunicipios[regcod][i][0] == munContainer.value );
        }

        for (i = 0; i < munContainer.options.length; i++)
            munContainer.options[i].selected = munContainer.options[i].value == munContainer.value;

        munContainer.removeAttribute("disabled");
    };

    //ajax.open('POST', 'seguranca.php?modulo=sistema/tabapoio/listamunicipio&acao=A&ajax=true&regcod=' + regcod, false);
    ajax.open('POST', '/geral/dne.php?opt=municipio&regcod=' + regcod, false);
    ajax.send(null);
}


/**
 *
 */
function getEnderecoPeloCEP(el)
{
    var endcep = document.getElementById('endcep_' + el);

    if (endcep.value == '') {
        limparDadosDNE(el);
        return false;
    }

    document.getElementById('endnum_' + el).select();
    var ajax = window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject( 'Microsoft.XmlHttp' );

    ajax.onreadystatechange = function()
    {
        if (ajax.readyState != 4)
            return;

        eval(ajax.responseText);

        if (!DNE['co_logradouro'] || DNE['co_logradouro'] == '') {
            alert('CEP nï¿½o encontrado!');
            endcep.value = '';
            endcep.select();
            return false;
        }

        var endlog   = document.getElementById('endlog_' + el);
        endlog.value = DNE['tipo_logradouro'] + ' ' + DNE['no_logradouro'];

        var endcom   = document.getElementById('endcom_' + el);
        endcom.value = DNE['no_complemento_logradouro'];

        var endbai   = document.getElementById('endbai_' + el);
        endbai.value = DNE['no_bairro'];

        var estuf    = document.getElementById('estuf_' + el);
        estuf.value  = DNE['sg_uf'];

        carregarMunicipiosPorUf(estuf.value, 'muncod_' + el);

        var muncod   = document.getElementById('muncod_' + el);
        var endnum   = document.getElementById('endnum_' + el);

        /**
         * @cat Burlar diferenï¿½a nas tabelas do IBGE e DNE
         */
        if (DNE['sg_uf'] == 'DF') {
            muncod.options[1].selected = true;
        } else {
            for (i = 0; i < muncod.options.length; i++) {
                if (muncod.options[i].innerHTML == DNE['co_municipio']) {
                    muncod.options[i].selected = true;
                }
            }
        }

        //endlog.setAttribute('disabled', 'disabled');
        //endbai.setAttribute('disabled', 'disabled');
        //estuf.setAttribute ('disabled', 'disabled');
        muncod.setAttribute('disabled', 'disabled');
    };

    //ajax.open('POST', 'seguranca.php?modulo=sistema/geral/dne&acao=B&endcep=' + endcep.value, true);
    ajax.open('POST', '/geral/dne.php?opt=dne&endcep=' + endcep.value, true);
    ajax.send(null);
}


/**
 *
 */
function limparDadosDNE(el)
{
    if (el == '') {
        var endlog   = document.getElementById('endlog_' + el);
        endlog.value = '';

        var endcom   = document.getElementById('endcom_' + el);
        endcom.value = '';

        var endbai   = document.getElementById('endbai_' + el);
        endbai.value = '';

        var estuf    = document.getElementById('estuf_' + el);
        estuf.options[0].selected  = true;

        var muncod   = document.getElementById('muncod_' + el);
        muncod.options[0].selected = true;

        var endnum   = document.getElementById('endnum_' + el);
        endnum.value = '';

        //endlog.removeAttribute('disabled');
        //endbai.removeAttribute('disabled');
        //muncod.removeAttribute('disabled');
        //estuf .removeAttribute('disabled');
    }
}

var ALL = 0;

/**
 *
 */
function validarFrm(frm, fields)
{
    var inputList  = document.getElementsByTagName('input');
    var labelList  = document.getElementsByTagName('label');
    var selectList = document.getElementsByTagName('select');

    var erros      = new Array();
    var z          = 0;

    for (var z = 0; z < fields.length; z++) {
        for (var i = 0; i < inputList.length; i++) {
            for (var j = 0; j < labelList.length; j++) {
                if (labelList[j].getAttribute('for') == inputList[i].getAttribute('id')) {
                    inputList[i].setAttribute('label', labelList[j].innerHTML.replace(':', ''));
                }
            }

            if (!/hidden/.test(inputList[i].getAttribute('type'))) {
                if (inputList[i].value == '' && (fields == ALL || fields[z] == inputList[i].getAttribute('name'))) {
                    erros.push('O campo ' + inputList[i].getAttribute('label') + ' ï¿½ obrigatï¿½rio!');
                }
            }
        }

        for (var i = 0; i < selectList.length; i++) {
            for (var j = 0; j < labelList.length; j++) {
                if (labelList[j].getAttribute('for') == selectList[i].getAttribute('id')) {
                    selectList[i].setAttribute('label', labelList[j].innerHTML.replace(':', ''));
                }
            }

            if (selectList[i].value == '' && (fields == ALL || fields[z] == selectList[i].getAttribute('name'))) {
                erros.push('O campo ' + selectList[i].getAttribute('label') + ' ï¿½ obrigatï¿½rio!');
            }
        }
    }


    if (erros.length > 0) {
        if (erros.length == 1)
            alert('Nï¿½o foi possï¿½vel salvar os dados.\n'
                 +'O seguinte erro foi encontrado:\n\n' + erros.join('\n'));
        else
            alert('Nï¿½o foi possï¿½vel salvar os dados.\n'
                 +'Os seguintes erros foram encontrados:\n\n' + erros.join('\n'));

        return false;
    }

    frm.submit();

    window.location.href.reload();

    //var tr = frm.parentNode;
    //tr.parentNode.removeChild(tr);
}


/**
 *
 */
function excluirEndereco(endid)
{
    if (!confirm('Deseja excluir o registro?')) {
        return false;
    }

    var ajax = window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject('Microsoft.XmlHttp');

    ajax.onreadystatechange = function()
    {
        if (ajax.readyState != 4)
            return;

        if (ajax.responseText == '') {
        } else {
            alert(ajax.responseText);
        }
    };

    ajax.open('POST', '/geral/dne.php?opt=excluirEndereco&endid=' + endid, false);
    ajax.send(null);

    return false;
}


function alterarEndereco(endid, container)
{
    var ajax = window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject('Microsoft.XmlHttp');

    ajax.onreadystatechange = function()
    {
        if (ajax.readyState != 4)
            return;

        if (ajax.responseText == '') {
        } else {
            alert(ajax.responseText);
        }
    };

    var entid     = document.getElementById('entid_'     + endid).value;
    var endcep    = document.getElementById('endcep_'    + endid).value;
    var endlog    = document.getElementById('endlog_'    + endid).value;
    var endcom    = document.getElementById('endcom_'    + endid).value;
    var endbai    = document.getElementById('endbai_'    + endid).value;
    var muncod    = document.getElementById('muncod_'    + endid).value;
    var estuf     = document.getElementById('estuf_'     + endid).value;
    var endnum    = document.getElementById('endnum_'    + endid).value;

    var request   = 'endid='  + endid  + '&' +
                    'entid='  + entid  + '&' +
                    'endcep=' + endcep + '&' +
                    'endlog=' + endlog + '&' +
                    'endcom=' + endcom + '&' +
                    'endbai=' + endbai + '&' +
                    'muncod=' + muncod + '&' +
                    'estuf='  + estuf  + '&' +
                    'endnum=' + endnum + '&' ;

    ajax.open('POST', '/geral/dne.php?opt=endereco&' + request, false);
    ajax.send(null);

    //alert(request);

    return false;
}


/**
 *
 */
function cancelarInsercaoEndereco(frm)
{
    var tr = frm.parentNode;
    tr.parentNode.removeChild(tr);
}


/**
 *
 */
function validarCpfCnpj(e)
{
    if (e.value.length != 14 && e.value.length != 18) {
        alert('O CPF ou CNPJ informado nï¿½o ï¿½ vï¿½lido!');
        return false;
    }

    var ajax = window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject( 'Microsoft.XmlHttp' );
    //var munContainer = munContainer          ? document.getElementById(munContainer) : document.getElementById('muncod');

    ajax.onreadystatechange = function()
    {
        e.setAttribute("disabled", "disabled");

        if (ajax.readyState != 4)
            return;

        //eval(ajax.responseText);

        e.removeAttribute("disabled");
    };

    //ajax.open('POST', 'seguranca.php?modulo=sistema/tabapoio/listamunicipio&acao=A&ajax=true&regcod=' + regcod, false);

    return true;
}


/**
 *
 */
function aplicarMascaraCpfCnpj(e)
{
    var el = e;

    if (el.value.length <= 14) {
        el.value = mascaraglobal("###.###.###-##", el.value);
    } else {
        el.value = mascaraglobal("##.###.###/####-##", el.value);
    }

    return el;
}


if (!this._windows)
    this._windows = [];

/**
 *
 */

this.onclose = this.onunload = function(e)
{
    try {
        for (var i = 0; i < this._windows.length; i++) {
//            if (typeof(this._windows[i].close) == 'function')
//                if (this._windows[i]._closeWindows == false)
//                    continue;
//                else
					if (this._windows[i] != this.notclose)
                    	this._windows[i].close();
        }
    } catch (e) {
    }
};

if (this.opener) {
    this.onload = function(e)
    {
        if (!this.opener._windows)
            this.opener._windows = [];

        this.opener._windows.push(this);
    };
}



function validarCnpj(cnpj) {
	cnpj = cnpj.replace(/[^0-9]/ig, '');
    if (cnpj.length != 14)
        return false;

    if (cnpj.length != 14)
        return false;

    var dv      = false;
    var cnpj_dv = cnpj.substr( 12, 2 );

    var digito  = 0;
    var controle = '';

    for (var i = 0; i < 2; i++ ) {
        var soma = 0;
        for ( var j = 0; j < 12; j++ )
            soma += cnpj.substr(j, 1) * ((11 + i - j) % 8 + 2);

        if ( i == 1 )
            soma += digito * 2;

        digito = 11 - soma  % 11;

        if ( digito > 9 )
            digito = 0;

        controle += digito + '';
    }

    return controle == cnpj_dv;
}

/*
*	Funï¿½ï¿½o que marca todos os checkboxes.
*	idComponente: id do input checkbox usado para selecionar todos.
*	nomeChecks: nome dos checkboxes a serem marcados/desmarcados(geralmente ï¿½ um nome de array. Ex: nome[]);
*/
function marcarTodosChecks( idComponente, nomeChecks ){

	var obMarcarTodos= document.getElementById( idComponente );
	var arCampos = document.getElementsByName( nomeChecks );

	for( i = 0; i<arCampos.length; i++ )
		arCampos[i].checked = obMarcarTodos.checked;

}

function windowOpen(url,name,options){window.open(url,name,options).focus();}
function fecharJanela(){if(window.opener){window.opener.location.reload();}window.close();}

/**
 * FunÃ§Ã£o que envia uma requisiÃ§Ãµes em ajax para a prÃ³pria pÃ¡gina
 * para carregar uma unidade gestora
 *
 * unicod: ID da unidade selecionada
 */
function ajax_unidade_gestora ( unicod ){
	var url = location.href + '&ajax=3&unicod=' + unicod;
	jQuery.post( url, function(html) {
		jQuery( "#unidade_gestora" ).html( html );
		jQuery( "#unidade_gestora").find('select').chosen({ width: '100%' });
		jQuery( "#unidade_gestora").find('select').trigger("chosen:updated");
	});
}

/**
 * FunÃ§Ã£o que envia uma requisiÃ§Ãµes em ajax para a prÃ³pria pÃ¡gina
 * para carregar uma unidade orï¿½amentï¿½ria
 *
 * entid: ID do OrgÃ£o selecionado
 */
function ajax_carrega_unidade( entid ){
	var url = location.href + '&ajax=2&entid=' + entid;
	jQuery.post( url, function(html) {
		jQuery( "#unidade" ).html( html );
		jQuery( "#unidade").find('select').chosen({ width: '100%' });
		jQuery( "#unidade").find('select').trigger("chosen:updated");
	});
}

/**
 * Funï¿½ï¿½o que evia uma requisiï¿½ï¿½o em ajax para a prï¿½pria pï¿½gina
 * para carregar uma unidade orgï¿½o
 *
 * tpocod: ID do tipo de orgï¿½o selecionado
 */
function ajax_carrega_orgao( tpocod ){
	if ( tpocod ) {

		if ( ($('muncod').value == '') && ($('regcod').value == '') ){

			var url = location.href + '&ajax=1&tpocod=' + tpocod;

		}else {

			var url = location.href + '&ajax=1&tpocod=' + tpocod
									+ '&muncod=' + $('muncod').value
									+ '&regcod=' + $('regcod').value;

		}

		if ( $('unicod') ){

			$('unicod').value = '';
			ajax_unidade_gestora( $('unicod').value );

		}

		if ( $('entid') ){

			$('entid').value = '';
			ajax_carrega_unidade( $('entid').value );

		}

		if ( $('orgao') ){

			ajax_carrega_unidade( '' );

		}

		var myAjax = new Ajax.Updater(
			"spanOrgao",
			url,
			{
				method: 'get',
				asynchronous: false
		});
	}
}




/**
 * Abre a janela de interaï¿½ï¿½o do combo_popup. Veja restante dos
 * comentï¿½rio do arquivo www/geral/combopopup.php
 *
 * @param string nome
 * @param integer height
 * @param integer width
 * @return void
 */
function campo_popup_abre_janela( nome, height, width )
{
//	var campo_select = document.getElementById( nome );
//	for ( var i = 0; i < campo_select.options.length; i++ )
//	{
//		campo_select.options[i].selected = false;
//	}
	//window.open( '../geral/combopopup.php?nome=' + nome, nome, "height=" + height +  ",width=" + width +  ",scrollbars=yes,top=50,left=200" );
	a = window.open( '../geral/campopopup.php?nome='+nome, 'Campopopup', "height=" + height +  ",width=" + width +  ",scrollbars=yes,top=50,left=200" );
	a.focus();
}

function selecionarprograma(sba) {
	document.formulario.evento.value = "SUBACAO";
	document.formulario.submit();
}

function campo_popup_adicionar_item( nome_campo, codigo, descricao){
	var campo = document.getElementById( nome_campo );

	if (campo.tagName == 'INPUT'){
		var campoDsc = document.getElementById( nome_campo+'_dsc' );

		campo.value    = codigo;
		campoDsc.value = descricao;
	}else{
		for (i=0; i<campo.options.length; i++){
			if (campo.options[i].value == codigo){
				campo.options[i].selected = true;
				break;
			}
		}
	}
//	var campo 	 = document.getElementById( nome_campo );
//	var campoDsc = document.getElementById( nome_campo+'_dsc' );
//
//	campo.value    = codigo;
//	campoDsc.value = codigo+' - '+descricao;
}

/**
 * Remove um item do combo popup.
 *
 * @param string nome_combo
 * @param string codigo
 * @param boolean ordenar
 * @return void
 */
function campo_popup_remover_item( nome_campo)
{
	var campo = document.getElementById( nome_campo );
	var campoDsc = document.getElementById( nome_campo+'_dsc' );

	campo.value    = '';
	campoDsc.value = 'Selecione...';
}

/**
 * Abre os dados do PI
 */
function mostradadospi( id, tipoacao ){
		var janela = window.open( '?modulo=principal/planointerno/dados_pi&acao=A&plicod=' + id + '&tipoacao=' + tipoacao,
					 			  'Dados do PI','scrollbars=yes, width=800, height=650 ');
		janela.focus();
	}

	function enviarEmail_usu(pagina,largura,altura,usuemail){

		//pega a resoluï¿½ï¿½o do visitante
		w = screen.width;
		h = screen.height;

		//divide a resoluï¿½ï¿½o por 2, obtendo o centro do monitor
		meio_w = w/2;
		meio_h = h/2;

		//diminui o valor da metade da resoluï¿½ï¿½o pelo tamanho da janela, fazendo com q ela fique centralizada
		altura2 = altura/2;
		largura2 = largura/2;
		meio1 = meio_h-altura2;
		meio2 = meio_w-largura2;

		window.open(pagina + usuemail,'','height=' + altura + ', width=' + largura + ', top='+meio1+', left='+meio2+'');
	}

function mudaPosicao(idTabela, tipo, rowIndex, table, atributoOrdem, id){

	var tabela = window.document.getElementById(idTabela);
	maxRows = tabela.rows.length - 1;

	if(tipo == "baixo"){
		var tr1 =  tabela.rows[rowIndex].innerHTML;
		var tr2 =  tabela.rows[rowIndex + 1].innerHTML;

		//Pegando id 1
		var ini1 = tr1.indexOf("ordem_");
		var fim1 = tr1.indexOf("_ordem");
		var id1 = tr1.substring(ini1,fim1);
		id1 = id1.replace(/ordem_/, "")

		//Pegando id 2
		var ini2 = tr2.indexOf("ordem_");
		var fim2 = tr2.indexOf("_ordem");
		var id2 = tr2.substring(ini2,fim2);
		id2 = id2.replace(/ordem_/, "");
	}
	//alert(rowIndex);
	if(tipo == "cima"){
		var tr1 =  tabela.rows[rowIndex].innerHTML;
		var tr2 =  tabela.rows[rowIndex - 1].innerHTML;

		//Pegando id 1
		var ini1 = tr1.indexOf("ordem_");
		var fim1 = tr1.indexOf("_ordem");
		var id1 = tr1.substring(ini1,fim1);
		id1 = id1.replace(/ordem_/, "")

		//Pegando id 2
		var ini2 = tr2.indexOf("ordem_");
		var fim2 = tr2.indexOf("_ordem");
		var id2 = tr2.substring(ini2,fim2);
		id2 = id2.replace(/ordem_/, "");
	}
	window.document.formlista.ordemId1.value = id1;
	window.document.formlista.ordemId2.value = id2;
	window.document.formlista.ordemTable.value = table;
	window.document.formlista.ordemAtributo.value = atributoOrdem;
	window.document.formlista.ordemId.value = id;
	window.document.formlista.ordemlista.value = "";

	window.document.formlista.submit();
}

function VerificaData(campo,valor) {
	var date=valor;
	var ardt=new Array;
	var ExpReg=new RegExp("(0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[012])/[12][0-9]{3}");
	ardt=date.split("/");
	erro=false;
	if ( date.search(ExpReg)==-1){
		erro = true;
		}
	else if (((ardt[1]==4)||(ardt[1]==6)||(ardt[1]==9)||(ardt[1]==11))&&(ardt[0]>30))
		erro = true;
	else if ( ardt[1]==2) {
		if ((ardt[0]>28)&&((ardt[2]%4)!=0))
			erro = true;
		if ((ardt[0]>29)&&((ardt[2]%4)==0))
			erro = true;
	}
	if (erro && valor) {
		alert("\"" + valor + "\" nï¿½o ï¿½ uma data vï¿½lida!!!");
		campo.focus();
		campo.value = "";
		return false;
	}
	return true;
}

function validando_data(objeto) {
	// validando a data
	var data_regex = /^((0[1-9]|[12]\d)\/(0[1-9]|1[0-2])|30\/(0[13-9]|1[0-2])|31\/(0[13578]|1[02]))\/\d{4}$/;
	if( objeto.value && ( !data_regex.test(objeto.value) || !validaData(objeto) ) ){
		alert('Data invï¿½lida!');
		objeto.value = '';
		objeto.focus();
		return false;
	}else{
		return true;
	}
}

function HoraMask(formName, fieldName, event){
    if (event.keyCode)
		var key = event.keyCode;
    else
	    var key = event.which;
    vr = document.forms[formName].elements[fieldName].value;
	vr = vr.replace(':','');
	size = vr.length + 1;

	//8=backspace, 9=tab, 46=delete, 127=delete, 37-40=arrow keys
	if (key!=9 && key!=8 && key!=46 && (key<37 || key>40))
	{
	   if (size == 3)
	      document.forms[formName].elements[fieldName].value = vr.substr(0, size-1) + ':' + vr.substr(size-1, size);
	   if (size > 3 && size < 5 )
	      document.forms[formName].elements[fieldName].value = vr.substr(0, size-2) + ':' + vr.substr(size-2, size);
	}
	return true;
}

function HoraValidationRoundMore(formName, fieldName, event){
	value = document.forms[formName].elements[fieldName].value;
	if(value == '')
		return;

	var reValue = /^\d{2}\:\d{2}$/;
	var flag = reValue.test(value);
	if(!flag)
	{
		alert("Valor invï¿½lido! O formato do Horï¿½rio deve ser hh:mm");
		//document.forms[formName].elements[fieldName].value = '';
		setTimeout("document.forms['" + formName + "'].elements['" + fieldName + "'].focus()",50);
		return;
	}
	var hora = value.substring(0,value.indexOf(":"));
	var minuto = value.substring(value.indexOf(":")+1,value.length);
	var hh = parseInt(hora);
	var mm = parseInt(minuto);
	if(hh > 23)
	{
		if(mm > 59)
		{
			alert("Valor invï¿½lido! O valor da Hora deve ser no mï¿½ximo 23 e o valor do Minuto deve ser no mï¿½ximo 59");
			//document.forms[formName].elements[fieldName].value = '';
			setTimeout("document.forms['" + formName + "'].elements['" + fieldName + "'].focus()",50);
			return;
		}
		else
		{
			alert("Valor invï¿½lido! O valor da Hora deve ser no mï¿½ximo 23 ");
			//document.forms[formName].elements[fieldName].value = '';
			setTimeout("document.forms['" + formName + "'].elements['" + fieldName + "'].focus()",50);
			return;
		}
	}
	if(mm > 59)
	{
		alert("Valor invï¿½lido! O valor do Minuto deve ser no mï¿½ximo 59");
		//document.forms[formName].elements[fieldName].value = '';
		setTimeout("document.forms['" + formName + "'].elements['" + fieldName + "'].focus()",50);
		return;
	}
	if( (mm % 5) > 0 ){
		var temp = value.substring(value.indexOf(":")+1,value.length);
		if (temp < "10"){
			temp = value.substring(value.indexOf(":")+2,value.length);
		}
		var mmTemp = parseInt(temp);
		//mmTemp = mmTemp + (5 - (mmTemp % 5));

		var parteAlta = value.substring(0,value.indexOf(":")+1);
		parteAlta = parteAlta.replace(":","");
		if(parteAlta == 23 && mmTemp == 60)
		{
		    alert("O horï¿½rio mï¿½ximo permitido ï¿½ 23:55!");
		    document.forms[formName].elements[fieldName].value = '';
		    return;
		}
		else if(mmTemp == 60 && parteAlta < 23 )
		{
		    mmTemp = "0";
		    parteAlta = parseInt(parteAlta) + 1;

		}
		parteAlta += ":"

		if (mmTemp < 10){
			parteAlta = parteAlta.concat("0");
			parteAlta = parteAlta.concat(mmTemp.toString());
		}
		else{
			parteAlta = parteAlta.concat(mmTemp.toString());
		}
		document.forms[formName].elements[fieldName].value = parteAlta;
	}
	return;
}

/*
	Funï¿½ï¿½o que, quando atingido seu maxlength, o focus o campo informado
	formName   = Nome do Formulï¿½rio
	size       = maxlength do campo
	fieldName  = Nome do Campo
	campoFocus = Nome do campo que receberï¿½ o focus.
*/

function pulaCampo(formName, size, fieldName, campoFocus, event){
	if (event.keyCode)
		var key = event.keyCode;
    else
	    var key = event.which;

	//8=backspace, 9=tab, 46=delete, 127=delete, 37-40=arrow keys
	if (key!=9 && key!=8 && key!=46 && (key<37 || key>40)){
		var vr = document.forms[formName].elements[fieldName].value;
		var tamanho = vr.length;
		if( tamanho == size ){
			document.forms[formName].elements[campoFocus].focus();
		}
	}
}

function naoSomenteNumeros(formName, fieldName, labelCampo){
	if(typeof(fieldName) == "object"){
		var dados 		= fieldName;
	}else{
		var dados 		= fieldName.split("#");
	}

	if(typeof(labelCampo) == "object"){
		var labelCampo 	= labelCampo;
	}else{
		var labelCampo 	= labelCampo.split("#");
	}

	var formulario 			= eval('document.'+formName);
	var erro 				= new Array();
	var existeErro 			= false;
	var alerta 				= "";
	var contErro			= 0;

	for (cont=0; cont<dados.length; cont++){
		for (i=0;i<formulario.elements.length;i++){
			if(formulario.elements[i].name == dados[cont]){
				if( Number(formulario.elements[i].value) ){
					existeErro = true;
					erro[contErro] = 'O campo "'+labelCampo[cont]+'" nï¿½o aceita somente nï¿½meros!';
					contErro++;
				}
			}
		}
	}
	for(contaa=0; contaa<erro.length; contaa++){
		alerta += erro[contaa]+" \n";
	}
	if(alerta){
		alert(alerta);
		return false;
	}else{
		return true;
	}
}

function abrirRegras(mnuid){
        $('#popup_anexo_documentos_modulo').modal();
//        $('#popup_anexo_documentos_modulo').modal();
//        $('#modal_upload .chosen-container').css('width', '100%');    
//	window.open(
//		'../geral/regra_tela.php?mnuid='+mnuid,
//		'usuariosonline',
//		'height=500,width=800,scrollbars=yes,top=50,left=200'
//	);
}

function abrirPopUpPublicarArquivo(usucpf, sisid){

	window.open(
		'../geral/listaPublicarArquivos.php',
		'publicararquivo',
		'toolbar=no,location=no,status=yes,menubar=no,fullscreen=yes,scrollbars=yes,resizable=yes'
	);
}

function truncarDecimal( valor, quantidade, separador )
{
	valor = ''+valor+'';
	quantidade = quantidade ? quantidade : 2;
	separador = separador ? separador : ',';

	arValor = valor.split(separador);
	if(arValor[1].length == quantidade){
		return valor;
	}else{
		arValor[1] = arValor[1].substr(0,quantidade);
		valor = arValor[0]+separador+arValor[1];
		return valor;
	}
}

function delimitadorTexto(texto, qtd)
{
	qtd = qtd ? qtd : 150;
	total = texto.lenght;
	if(total == qtd){
		return texto;
	}else{
		return texto.substr(0, qtd)+'...';
	}
}



/**
 * Funcao feita para validar numero de caracteres e retornar false no caso de mais caracteres que o permitido
 */
function validaNumeroCaracteresTextareas( arrayCampos, numeroCaracteres ){
	var retorno = true;

	for ( var i = 0; arrayCampos.length > i ; i++ ){

		jQuery('[name^='+arrayCampos[i]+']').each(function(){
			if( jQuery(this).val().length > numeroCaracteres ) {
				retorno = false;
			}
		});

	};

	return retorno;
}

/*
 * funï¿½ï¿½o similar com a in_array() do php
 * */
function in_array(needle, haystack) {
    var length = haystack.length;
    for(var i = 0; i < length; i++) {
        if(haystack[i] == needle) return true;
    }
    return false;
}

/**
 * Faz a mudanï¿½a de focus automatica de um elemento para o outro apï¿½s o usuario digitar um valor
 *
 * @param string nameCurrentElement atributo "name" do elemento atual que perderï¿½ o foco
 * @param string nameNextElement atributo "name" do elemento que receberï¿½ o foco
 * @returns VOID
 */
function autoTab(nameCurrentElement, nameNextElement){
    // Busca tamanho do elemento
    var NumChar = jQuery('input[name='+nameCurrentElement+']').val().length;
    // Espera meio segundo para desencadear a prï¿½xima aï¿½ï¿½o
    setTimeout( function(){
        // Verifica se existe mais de 1 caractere no elemento atual
        if(NumChar >= 1){
            // Muda o foco para o prï¿½ximo elemento do formulario
            jQuery('input[name='+nameNextElement+']').focus();
        }
    }, 500);
}


function removeAcento(strToReplace) {
	str_acento= "ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½";
	str_sem_acento = "aaaaaeeeeiiiiooooouuuucAAAAAEEEEIIIIOOOOOUUUUC";
	var nova="";

	for (var i = 0; i < strToReplace.length; i++) {
		if (str_acento.indexOf(strToReplace.charAt(i)) != -1) {
			nova+=str_sem_acento.substr(str_acento.search(strToReplace.substr(i,1)),1);
		} else {
			nova+=strToReplace.substr(i,1);
		}
	}

	return nova;
}

/**
 * Máscaras ER
 * Mascaras compativeis com dispositivos mobile e versões recentes de navegadores
 * @example <input type="text" onkeyup="mascara(this, mcpf);" />
 * @example $('#usucpf').keyup(function(){ mascara(this, mcpf); });
 * 
 * @param object o Objeto dom HTML
 * @param string f Nome da função
 * @returns {undefined}
 */
function mascara(o,f){
    v_obj=o
    v_fun=f
    setTimeout("execmascara()",1)
}
function execmascara(){
    v_obj.value=v_fun(v_obj.value)
}
function mcep(v){
    v=v.replace(/\D/g,"")                    //Remove tudo o que não é dígito
    v=v.replace(/^(\d{5})(\d)/,"$1-$2")         //Esse é tão fácil que não merece explicações
    return v
}
function mtel(v){
    v=v.replace(/\D/g,"");             //Remove tudo o que não é dígito
    v=v.replace(/^(\d{2})(\d)/g,"($1) $2"); //Coloca parênteses em volta dos dois primeiros dígitos
    v=v.replace(/(\d)(\d{4})$/,"$1-$2");    //Coloca hífen entre o quarto e o quinto dígitos
    return v;
}
function cnpj(v){
    v=v.replace(/\D/g,"")                           //Remove tudo o que não é dígito
    v=v.replace(/^(\d{2})(\d)/,"$1.$2")             //Coloca ponto entre o segundo e o terceiro dígitos
    v=v.replace(/^(\d{2})\.(\d{3})(\d)/,"$1.$2.$3") //Coloca ponto entre o quinto e o sexto dígitos
    v=v.replace(/\.(\d{3})(\d)/,".$1/$2")           //Coloca uma barra entre o oitavo e o nono dígitos
    v=v.replace(/(\d{4})(\d)/,"$1-$2")              //Coloca um hífen depois do bloco de quatro dígitos
    return v
}
function mcpf(v){
    v=v.replace(/\D/g,"")                    //Remove tudo o que não é dígito
    v=v.replace(/(\d{3})(\d)/,"$1.$2")       //Coloca um ponto entre o terceiro e o quarto dígitos
    v=v.replace(/(\d{3})(\d)/,"$1.$2")       //Coloca um ponto entre o terceiro e o quarto dígitos
                                             //de novo (para o segundo bloco de números)
    v=v.replace(/(\d{3})(\d{1,2})$/,"$1-$2") //Coloca um hífen entre o terceiro e o quarto dígitos
    return v
}
function mdata(v){
    v=v.replace(/\D/g,"");                    //Remove tudo o que não é dígito
    v=v.replace(/(\d{2})(\d)/,"$1/$2");
    v=v.replace(/(\d{2})(\d)/,"$1/$2");

    v=v.replace(/(\d{2})(\d{2})$/,"$1$2");
    return v;
}
function mtempo(v){
    v=v.replace(/\D/g,"");                    //Remove tudo o que não é dígito
    v=v.replace(/(\d{1})(\d{2})(\d{2})/,"$1:$2.$3");
    return v;
}
function mhora(v){
    v=v.replace(/\D/g,"");                    //Remove tudo o que não é dígito
    v=v.replace(/(\d{2})(\d)/,"$1h$2");
    return v;
}
function mrg(v){
    v=v.replace(/\D/g,"");                                      //Remove tudo o que não é dígito
        v=v.replace(/(\d)(\d{7})$/,"$1.$2");    //Coloca o . antes dos últimos 3 dígitos, e antes do verificador
        v=v.replace(/(\d)(\d{4})$/,"$1.$2");    //Coloca o . antes dos últimos 3 dígitos, e antes do verificador
        v=v.replace(/(\d)(\d)$/,"$1-$2");               //Coloca o - antes do último dígito
    return v;
}
function mnum(v){
    v=v.replace(/\D/g,"");                                      //Remove tudo o que não é dígito
    return v;
}
function mvalor(v){
    v=v.replace(/\D/g,"");//Remove tudo o que não é dígito
    v=v.replace(/(\d)(\d{8})$/,"$1.$2");//coloca o ponto dos milhões
    v=v.replace(/(\d)(\d{5})$/,"$1.$2");//coloca o ponto dos milhares

    v=v.replace(/(\d)(\d{2})$/,"$1,$2");//coloca a virgula antes dos 2 últimos dígitos
    return v;
}

/**
 * Abre um arquivo cadastrado no sistema de comunicados.
 * @param {int} arqid O ID do arquivo no sistema.
 */
function abrirArquivo(arqid) {
    var uri = window.location.href;
    uri = uri.replace(/\?.+/g, '?modulo=principal/arquivo/visualizar&acao=A&download=S&arqid=' + arqid);
    window.location.href = uri;
}

/**
 * Transforma o valor de texto pra flutuante pra efetuar operações matematicas.
 *
 * @param string text
 * @return float numero
 */
 function textToFloat(text){
     var numero = 0;
     text = replaceAll(text, '.', '');
     text = replaceAll(text, ',', '.');
     if(!isNaN(parseFloat(text))){
         numero = parseFloat(text);
     }

     return numero;
 }
 
 