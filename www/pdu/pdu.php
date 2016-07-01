<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

// carrega as fun��es espec�ficas do m�dulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

//1100
//Institui��o
//pdu.php?modulo=principal/instituicao&acao=C


//1200
//Indicadores Educacionais
//pdu.php?modulo=principal/indicadores_educacionais&acao=C



/**
 * Classe para realizar listagem.
 */
include_once APPRAIZ . 'includes/library/simec/Crud/Listing.php';

/**
 * Classe de conex�o com o banco de dados.
 */
//include_once APPRAIZ . '/includes/library/simec/ConnectOracle.php';
//$clsOracle = new ConnectOracle();

/**
 * Classe que gera graficos.
 */
include_once APPRAIZ . "includes/library/simec/Grafico.php";
//$clsGrafico = new Grafico();

include_once '_autoload.php';

$nomesRastros = array( 'inicio' => 'In�cio', 'principal' => 'In�cio' , 'instituicao' => 'Institui��o', 'indicadores_educacionais' => 'Indicadores Educacionais' , 'configuracao_guia' => 'Configura��o');

//Carrega as fun��es de controle de acesso
include_once "controleAcesso.inc";


?>

<script>
    $('select.chosen').chosen();
</script>