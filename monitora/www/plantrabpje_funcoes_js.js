/**
* Vari�veis globais para manipula��o futura das c�lulas com as datas e redefini��o de eventos da popup do calend�rio
*
*/
var codigoCampo;
var prefixoCampo;
var corCampo
var definirEventos = false;
var arrElementos = new Array();// Array que armazenar� o c�digo, a cor original e a ordem de cada atividade
var ordemElemento;
var isIE;

/**
* Abre popup do calend�rio, cria campo hidden de acordo com o prefixo e o c�digo (ptoid)
*
* @param integer cod
* @param string prfx ('dt_ini' || 'dt_fim')
* @param string ordem
* @param string cor
* @return void
*/
function altera_data( cod, prfx, ordem, cor, naoAbrir )
{
	codigoCampo = cod;
	prefixoCampo = prfx;
	ordemElemento = ordem;
	corCampo = cor;
	nome = prfx + cod;
	if( !naoAbrir )
	{
		show_calendar( ( "formulario." + nome ) );

		if ( navigator.userAgent.indexOf("Safari") > 0 )
		{
			ggWinCal.addEventListener( "unload", onCloseCalendario, false );
		}
		else if( navigator.product == "Gecko" )
		{
			ggWinCal.addEventListener( "unload", onCloseCalendario, false );
		}
		else
		{
			ggWinCal.onclose = new Function( '' );
			ggWinCal.attachEvent( 'onunload', onCloseCalendario );
			isIE = true;
		}
		if( !document.formulario[ nome ] )
		{
			objFormulario = document.getElementById( 'formulario' );

			tag = isIE ? '<input type="hidden" name="' + nome + '" />': "input";

			campo = document.createElement( tag );
			if( !isIE )
			{
				campo.setAttribute( "type", "hidden" );

				campo.setAttribute( "name", nome );
			}
			objFormulario.appendChild( campo );
		}

		definirEventos = true;
	}
	else
	{
		onCloseCalendario();
	}
}

function defineEventos( janela )
{
	if ( navigator.userAgent.indexOf("Safari") > 0 )
	{
		janela.addEventListener( "unload", onCloseCalendario, false );
	}
	else if ( navigator.product == "Gecko")
	{
		janela.addEventListener( "unload", onCloseCalendario, false );
	}
	else
	{
		janela.onclose = new Function( '' );
		janela.attachEvent( 'onunload', onCloseCalendario );
	}
}
/**
* Listener para o evento unload da janela do calend�rio
*
* @return void
*/
function onCloseCalendario( evt )
{
	jaExiste = false;
	for( var i = 0 ; i < arrElementos.length ; i++ )
	{
		if( arrElementos[ i ][ 'codigo' ] == codigoCampo )
		{
			jaExiste = true;
		}
	}
	if( !jaExiste )
	{
		arrElementos.push( { codigo:codigoCampo, cor:corCampo, ordem:ordemElemento } );
	}

	var celula = document.getElementById( prefixoCampo + codigoCampo );
	var campo = document.formulario[ prefixoCampo + codigoCampo ];
	celula.innerHTML = campo.value;
}

function submeterAlteracoes()
{
	if( arrElementos.length > 0 )
	{
		arrElementos.sortObject( 'ordem', 'asc', 1 );
		toString = arrElementos.objectToString( 'codigo', null, '%' );
		if( !document.formulario.arrCod )
		{
			objFormulario = document.getElementById( 'formulario' );

			tag = isIE ? '<input type="hidden" name="arrCod" value="' + toString + '" />': "input";

			campo = document.createElement( tag );
			if( !isIE )
			{
				campo.setAttribute( "type", "hidden" );

				campo.setAttribute( "name", "arrCod" );
				campo.setAttribute( "value", toString );
			}
			objFormulario.appendChild( campo );
		}
		else
		{
			document.formulario.arrCod.value = toString;
		}
		if( !document.formulario.ptocod )
		{

			//Verifica se as datas de in�cio s�o menores que as datas de t�rmino
			erro = 0;
			for( var i = 0 ; i < arrElementos.length ; i++ )
			{
				if( !validaDataMaior( document.formulario[ 'dt_ini' + arrElementos[ i ][ 'codigo' ]  ], document.formulario[ 'dt_fim' + arrElementos[ i ][ 'codigo' ] ] ) )
				{
					document.getElementById( 'dt_ini' + arrElementos[ i ][ 'codigo' ] ).style.color = "#ff0000";
					erro = 1;
				}
				else
				{
					document.getElementById( 'dt_ini' + arrElementos[ i ][ 'codigo' ] ).style.color = arrElementos[ i ][ 'cor' ];
				}
			}
			if( erro )
			{
				alert( 'As datas de in�cio em vermelho s�o posteriores �s datas de t�rmino' );
			}
			else
			{
				document.formulario.submit();
			}
		}
		else
		{
			validar_cadastro( 'A' );
		}
	}
}
