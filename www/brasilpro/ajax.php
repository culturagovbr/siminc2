<?php

ob_start();

/**
 * Centraliza as requisi��es ajax do m�dulo.  
 *
 * @author Ren� de Lima Barbosa <renebarbosa@mec.gov.br> 
 * @since 01/11/2007
 */

function erro( $codigo, $mensagem, $arquivo, $linha ){
	echo "Ocorreu um erro. Por favor tente mais tarde.";
	exit();
}

function excecao( Exception $excecao ){
	echo "Ocorreu um erro. Por favor tente mais tarde.";
	exit();
}

// captura controladamente eventuais erros
set_error_handler( 'erro', E_USER_ERROR );
set_exception_handler( 'excecao' );

// indica ao navegador o tipo de sa�da
header( 'Content-type: text/plain' );
header( 'Cache-Control: no-store, no-cache' );

// carrega as fun��es gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// atualiza a��o do usu�rio no sistema
include APPRAIZ . "includes/registraracesso.php";

// carrega as fun��es do m�dulo
include '_constantes.php';
include '_funcoes.php';
include '_componentes.php';

// abre conex�o com o servidor de banco de dados
$db = new cls_banco();

function fechaDb()
{
    global $db;
    $db->close();
}

register_shutdown_function('fechaDb');

switch ( $_REQUEST['evento'] )
{

	case "alterar_status_subacao":
		
		// captura dados
		$sbaid = (integer) $_REQUEST['sbaid'];
		$ssuid = (integer) $_REQUEST['ssuid'];
		$ssuid = $ssuid ? $ssuid : " null ";
		
		$sql = "
			 update cte.subacaoindicador
			 set
				 ssuid = " . $ssuid . "
			 where
				 sbaid = " . $sbaid . "
		";
		ob_end_clean();
		if ( $db->executar( $sql ) )
		{
			echo "sucesso";
			$db->commit();
		}
		else
		{
			echo"falha";
			$db->rollback();
		}
		exit();

	default:
		echo '';
		exit();

}

?>