<?php
//Carrega parametros iniciais do simec
include_once 'controleInicio.inc';

/**
 * Autoload de classes dos M�dulos SPO.
 * @see autoload.php
 */
require_once APPRAIZ . 'spo/autoload.php';

// carrega as fun��es espec�ficas do m�dulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';
include_once '_autoload.inc';

$perfis = pegaPerfilGeral();
/* Controle de exibi��o do de Simular Usu�rio */
/* 1285 - CGSO */
if (is_array($perfis) && (in_array(PERFIL_CGSO, $perfis) || in_array(PERFIL_UG_REPASSADORA, $perfis))) {
    $exibirSimular = true;
}

//Carrega as fun��es de controle de acesso
include_once 'controleAcesso.inc';

$_SESSION['sislayoutbootstrap'] = 't';

if (strpos($_SERVER['REQUEST_URI'], 'modulo=sistema')) : ?>
    <script src="/includes/funcoes.js" ></script>
<?php endif;