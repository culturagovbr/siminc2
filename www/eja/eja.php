<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

// carrega as fun��es espec�ficas do m�dulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

/**
 * @TODO Tratamento para colocar o layout antigo nas telas de sistemas que n�o tem o jquery compativel ainda com o layout novo
 */
#BLOCO DE C�DIGO USADO APENAS PARA O RELAT�RIO. MDIDA EMERGENCIAL PARA SER A ESTRUTURA ENTIGA NO RELAT�RIO.
$arrModulo = explode( '/', $_GET['modulo']);
$modulo = reset($arrModulo);

if(!empty($modulo) && ( $modulo == 'sistema' || $modulo == 'relatorio') ){
    $_SESSION['sislayoutbootstrap'] = false; 
} else {
    $_SESSION['sislayoutbootstrap'] = true; 
}
#FIM DE BLOCO.

//Carrega as fun��es de controle de acesso
include_once "controleAcesso.inc";

?>