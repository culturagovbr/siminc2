/** 
 * Arquivo com fun��es utilizadas pelos relat�rios que usam o componente de relat�rios.
 * @see includes/classes/relatorio.class.inc
 */

/**
 * Alterar visibilidade de um campo.
 * Requer que o formul�rio de configura��o do relat�rio seja chamada "formulario".
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
        var div_on = document.getElementById( campo + '_campo_on' );
        var div_off = document.getElementById( campo + '_campo_off' );
        var input = document.getElementById( campo + '_campo_flag' );
        if ( div_on.style.display == 'none' )
        {
                div_on.style.display = 'block';
                div_off.style.display = 'none';
                input.value = '1';
        }
        else
        {
                div_on.style.display = 'none';
                div_off.style.display = 'block';
                input.value = '0';
        }
}
