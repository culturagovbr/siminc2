<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";
/**
 * Autoload de classes dos M�dulos SPO.
 * @see autoload.php
 */
require_once APPRAIZ . 'spo/autoload.php';

// carrega as fun��es espec�ficas do m�dulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';
$perfis = pegaPerfilGeral();
if (is_array($perfis)) {
    /* Controle de exibi��o do bot�o do Gr�fico do Workflow */
    if (in_array(PFL_ADMINISTRADOR, $perfis) || in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis)) {
        $exibirGraficoWorflow = true;
    }
    /* Controle de exibi��o do de Simular Usu�rio */
    if (in_array(PFL_ADMINISTRADOR, $perfis) || in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis)) {
        $exibirSimular = true;
    }
}


$simec = new Simec_View_Helper();
$_SESSION['sislayoutbootstrap'] = 'zimec';

// -- Export de XLS autom�tico da Listagem
Simec_Listagem::monitorarExport($_SESSION['sisdiretorio']);

//Carrega as fun��es de controle de acesso
include_once "controleAcesso.inc";

/* Inclus�o de Javascript para as funcionalidades da pasta SISTEMA */
if (strpos($_SERVER['REQUEST_URI'], 'modulo=sistema')): ?>
    <script src="/includes/funcoes.js" ></script>
    <?php
endif;
