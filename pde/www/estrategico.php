<?php
if (!empty($_REQUEST['layout']) && $_REQUEST['layout'] == 'novo'){
    //Carrega parametros iniciais do simec
    include_once "controleInicio.inc";

    include_once APPRAIZ . "includes/classes/Modelo.class.inc";
    include_once APPRAIZ . "includes/classes/Controle.class.inc";
    include_once APPRAIZ . "includes/classes/Visao.class.inc";
    include_once APPRAIZ . "includes/library/simec/Listagem.php";

    // carrega as fun��es espec�ficas do m�dulo
    include_once '_constantes.php';
    include_once '_funcoes.php';
    include_once '_componentes.php';
    include_once 'autoload.php';

    include_once APPRAIZ . 'includes/library/simec/view/Helper.php';

    initAutoload();

    $simec = new Simec_View_Helper();

    $_SESSION['sislayoutbootstrap'] = 'zimec';
    $_SESSION['exercicio'] = "";
    $_SESSION["sisexercicio"] = "";
    //Carrega as fun��es de controle de acesso
    include_once "controleAcesso.inc";  
    
}else{
    //Carrega parametros iniciais do simec
    include_once "controleInicio.inc";

    // carrega as fun��es espec�ficas do m�dulo
    include_once '_constantes.php';
    include_once '_funcoes_estrategico.php';
    include_once '_componentes_estrategico.php';

    //Carrega as fun��es de controle de acesso
    include_once "controleAcesso.inc";
    
    $_SESSION['sislayoutbootstrap'] = "";
}
?>