<?php
    
    define('SIGLA_SISTEMA', 'SIMEC-DESENVOLVIMENTO');
    define('DESCRICAO_SISTEMA', 'Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle');
    define('NOME_SISTEMA', SIGLA_SISTEMA. ' - '. DESCRICAO_SISTEMA);
    define('URL_SISTEMA', 'http://localhost:8083/');
    define('EMAIL_SISTEMA_NOREPLY', 'noreply@cultura.gov.br');
    define('IP_CLI_AUTORIZADO', '172.17.0.2');

    global $servidor_bd, $porta_bd, $nome_bd, $usuario_db, $senha_bd, $servidor_bd_siafi, $porta_bd_siafi, $nome_bd_siafi, $usuario_db_siafi, $senha_bd_siafi;

    /**
     * Configuração PHP  
     */
    session_start();
    ini_set("short_open_tag","on");
    ini_set('display_errors', 'on');
    date_default_timezone_set('America/Sao_Paulo');
    header('content-type: text/html; charset=ISO-8859-1');

    defined('AUTHSSD') || define('AUTHSSD', false);
    defined('APPRAIZ') || define('APPRAIZ', '/var/www/simec/');
    defined('DIRFILES') || define('DIRFILES', APPRAIZ. 'arquivos/');
    defined('SISRAIZ') || define('SISRAIZ', APPRAIZ . $_SESSION['sisdiretorio'] . '/');
    defined('MAXONLINETIME') || define('MAXONLINETIME', 3600);
    
    # Indica o ambiente para controlar comportamentos que só devem ser realizados em produção
    define('IS_PRODUCAO', FALSE);
    # Indica se o sistema vai gravar auditoria
    define('EXECUTAR_AUDITORIA', TRUE);

    $_SESSION['ambiente'] = IS_PRODUCAO ? 'Ambiente de Produção' : 'Ambiente de Desenvolvimento';
    $_SESSION['sisbaselogin'] = $_REQUEST['baselogin'] && $_REQUEST['baselogin'] != "teste" ? $_REQUEST['baselogin'] : $_SESSION['sisbaselogin'];
    $_SESSION['baselogin'] = $_REQUEST['baselogin'] ? $_REQUEST['baselogin'] : $_SESSION['baselogin'];

    # Configuração de Banco de Dados
    include_once 'database.php';

    $_SESSION['email_sistema'] = '[PREENCHER]';
    $_SESSION['config_smtp'] = '';

    preg_match('/\/([a-zA-Z]*)\//', $_SERVER['REQUEST_URI'], $sisdiretorio);
    $sisdiretorio = $sisdiretorio[1];

    preg_match('/\/([a-zA-Z]*)\.php/', $_SERVER['REQUEST_URI'], $sisarquivo);
    $sisarquivo = $sisarquivo[1];

    # Código do órgão que está usando o sistema. Ex: 42000 => Ministério da Cultura
    define('CODIGO_ORGAO_SISTEMA', 42000);
    define('NOME_ORGAO_SISTEMA', 'Ministério da Cultura');
    define('SIGLA_ORGAO_SISTEMA', 'MinC');

    # Configuração padrão dos dados de acesso
    $configPadrao = new stdClass();
    $configPadrao->estufPadrao = 'DF';
    $configPadrao->muncodPadrao = '5300108';
    $configPadrao->tpocodPadrao = '1';
    
    $configSiconv = new stdClass();
    $configSiconv->user = '[PREENCHER]';
    $configSiconv->pwd = md5('[PREENCHER]');
    
    # SIOP - Configuração de webservice do SIOP
    # Arquivo de credencial
    define('WEB_SERVICE_SIOP_CERTIFICADO', APPRAIZ. '[PREENCHER]');
    # Produção SIOP
    define('WEB_SERVICE_SIOP_URL', 'https://www.siop.gov.br/services/');
    define('WEB_SERVICE_SIOP_USUARIO', '[PREENCHER]');
    define('WEB_SERVICE_SIOP_SENHA', '[PREENCHER]');
    define('WEB_SERVICE_SIOP_PERFIL', '[PREENCHER]');
    
    # SICONV
    define('WEB_SERVICE_SICONV_URL', 'https://ws.convenios.gov.br/siconv-siconv-interfaceSiconv-1.0/InterfaceSiconvHandlerBeanImpl?wsdl');

    # Configurações do Webservice próprio do Orgão pra gerenciar consultas ao Webservice da Receita Federal
    # Homologação
    define('WS_RF_BASE_URL', '[PREENCHER]');
    define('WS_RF_USER', '[PREENCHER]');
    define('WS_RF_SENHA', '[PREENCHER]');
    
    # Aplicativo de Mapas do Google
    define('WS_GOOGLE_MAPS_URL', 'http://maps.google.com/maps?file=api&amp;v=2&amp;key=[PREENCHER]');

    include_once APPRAIZ. 'includes/simec_funcoes.inc';
    include_once APPRAIZ. 'includes/backtrace/BackTrace.php';
    include_once APPRAIZ. 'includes/backtrace/BackTraceExplain.php';
    include_once APPRAIZ. 'includes/failure/ErrorHandler.php';
    include_once APPRAIZ. 'includes/failure/ExceptionHandler.php';
    include_once APPRAIZ. 'includes/library/simec/SoapClient.php';
    include_once APPRAIZ. 'includes/connection/adapter-connection.php';
    include_once APPRAIZ. 'includes/siop-soap-curl/siop-soap-curl.php';

    function fechar_conexoes(){
        while(  pg_ping()){
            pg_close();
        }
    }

    register_shutdown_function('fechar_conexoes');
    ErrorHandler::start();
    ExceptionHandler::start();
    ob_start();
