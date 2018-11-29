<?php
/**
 * @author Alexandre Dourado <alexandredourado@gmail.com>
 */

function carregarUsuariosSessao()
{
    global $servidor_bd, $porta_bd, $nome_bd, $usuario_db, $senha_bd;

    /* configurações do relatorio - Memoria limite de 1024 Mbytes */
    ini_set ( "memory_limit", "1024M" );
    set_time_limit ( 0 );
    /* FIM configurações - Memoria limite de 1024 Mbytes */

    $conn_string = "host=$servidor_bd port=$porta_bd dbname=$nome_bd user=$usuario_db password=$senha_bd";
    $dbconn = pg_connect ( $conn_string );

    $result = pg_query ( $dbconn, "SELECT ees.sisid, usu.usuemail FROM seguranca.envioerrosusuarios eeu
                             INNER JOIN seguranca.envioerrosususistema ees ON ees.eeuid = eeu.eeuid
                             INNER JOIN seguranca.usuario usu ON usu.usucpf = eeu.usucpf
                             WHERE eeu.eeustatus='A'" );

    $num = pg_numrows ( $result );

    $aDestinatarios = array();
    for($i = 0; $i < $num; $i ++) {
        $line = pg_fetch_array ( $result, $i, PGSQL_ASSOC );
        $aDestinatarios[$line['sisid']][] = $line['usuemail'];
        $aDestinatarios['todos'][$line['usuemail']] = $line['sisid'];
    }

    @pg_close ( $dbconn );

    return $aDestinatarios;
}

/**
 *
 * @author Renan de Lima Barbosa <renandelima@gmail.com>
 * @author Renê de Lima Barbosa <renedelima@gmail.com>
 */
class ErrorHandler {

    /**
     *
     * @var array
     */
    static $aErrorType = array (
        E_ERROR => 'Error',
        E_WARNING => 'Warning',
        E_PARSE => 'Parsing Error',
        E_NOTICE => 'Notice',
        E_CORE_ERROR => 'Core Error',
        E_CORE_WARNING => 'Core Warning',
        E_COMPILE_ERROR => 'Compile Error',
        E_COMPILE_WARNING => 'Compile Warning',
        E_USER_ERROR => 'User Error',
        E_USER_WARNING => 'User Warning',
        E_USER_NOTICE => 'User Notice',
        E_STRICT => 'Runtime Notice'
    );

    /**
     *
     * @var ErrorHandler
     */
    static $oHandler = null;

    /**
     *
     * @ignore
     *
     */
    private function __construct() {
    }

    /**
     *
     * @param integer $iCode
     * @param string $sMessage
     * @param string $sFile
     * @param integer $iLine
     * @param array $aContext
     * @return void
     */
    public function display($iCode, $sMessage, $sFile, $iLine, array $aContext = array()) {
        $oTrace = new BackTrace ();
        $oTrace->levelDown ( 2 );
        $sTrace = $oTrace->explain ();
        ob_clean ();

        ob_start ();
        print_r ( $_REQUEST );
        $v_requ = ob_get_contents ();
        ob_clean ();

        ob_start ();
        print_r ( $_POST );
        $v_post = ob_get_contents ();
        ob_clean ();

        ob_start ();
        print_r ( $_FILES );
        $v_files = ob_get_contents ();
        ob_clean ();

        ob_start ();
        $session = $_SESSION;
        unset( $session ['desenvolvedores'] );
        unset( $session ['desenvolvedores_'] );
        unset( $session ['acl'] );
        unset( $session ['sistemas'] );
        unset( $session ['abamenu'] );
        unset( $session ['menu'] );
        unset( $session ['dadosusuario'] );
        unset( $session ['indice_sessao_combo_popup'] );
        unset( $session ['manuais'] );
        unset( $session ['perfils'] );
        unset( $session ['tipodocumentos'] );
        unset( $session ['perfil'] );
        unset( $session ['rastro'] );
        unset( $session ['MSG_AVISO'] );
        print_r ( $session );
        $v_session = ob_get_contents ();
        ob_clean ();

        ob_start ();
        print_r ( $_SERVER );
        $v_server = ob_get_contents ();
        ob_clean ();

        $msg_erro = "
	   <fieldset>
			   <legend>" . self::$aErrorType [$iCode] . " - " . $_SESSION ['ambiente'] . "</legend>
			   <pre><b>" . $sMessage . "</b></pre>" . $sTrace . "
	   </fieldset>
	   <fieldset>
			   <legend><b>Variaveis de _REQUEST</b></legend>
			   <pre>" . $v_requ . "</pre>
	   </fieldset>
	   <fieldset>
			   <legend><b>Variaveis de _POST</b></legend>
			   <pre>" . $v_post . "</pre>
	   </fieldset>
	   <fieldset>
			   <legend><b>Variaveis de _SESSION</b></legend>
			   <pre>" . $v_session . "</pre>
	   </fieldset>
	   <fieldset>
			   <legend><b>Variaveis de _FILES</b></legend>
			   <pre>" . $v_files . "</pre>
	   </fieldset>
	   <fieldset>
			   <legend><b>Variaveis de _SERVER</b></legend>
			   <pre>" . $v_server . "</pre>
	   </fieldset>        ";

        return $msg_erro;
    }

    /**
     * Grava Log de erro
     *
     * @param Text $sMessage
     * @return void
     */
    function gravarErro($sMessage) {
        global $db;

        if (is_object ( $db )) {
            $oTrace = new BackTrace ();
            $oTrace->levelDown ( 3 );

            do {
                $info = $oTrace->getCurrent ();
                if ($info ['file'] && 'classes_simec.inc' != substr ( $info ['file'], - 17 )) {
                    break;
                }
            } while ( $oTrace->levelDown () === true );

            // Identificando tipo de Erro
            switch (true) {
                // Diretório cheio
                case (false !== stripos ( $sMessage, 'failed to open stream: Too many open files' )) :
                    $tipo = 'DC';
                    break;
                // Nome de arquivo errado
                case (false !== stripos ( $sMessage, 'failed to open stream: No such file or directory' )) :
                    $tipo = 'AI';
                    break;
                // Banco de dados
                case (false !== stripos ( $sMessage, 'value too long for type' )) :
                case (false !== stripos ( $sMessage, 'invalid input syntax for' )) :
                case (false !== stripos ( $sMessage, 'syntax error at or near' )) :
                case (false !== stripos ( $sMessage, 'missing FROM-clause entry' )) :
                case (false !== stripos ( $sMessage, 'is out of range for type' )) :
                case (false !== stripos ( $sMessage, 'specified more than once' )) :
                case (false !== stripos ( $sMessage, 'does not exist' )) :
                case (false !== stripos ( $sMessage, 'violates foreign key constraint' )) :
                    $tipo = 'DB';
                    break;
                // WebService
                case (false !== stripos ( $sMessage, 'SoapClient' )) :
                    $tipo = 'WS';
                    break;
                // Encoding no banco
                case (false !== stripos ( $sMessage, 'invalid byte sequence for encoding' )) :
                    $tipo = 'EN';
                    break;
                // Erro na Conexão (possivelmente pdeinterativo)
                case (false !== stripos ( $sMessage, 'no pg_hba.conf entry for host' )) :
                case (false !== stripos ( $sMessage, 'Connection timed out Is the server running' )) :
                    $tipo = 'PD';
                    break;
                // Erro de programação (expected array, etc)
                case (false !== stripos ( $sMessage, 'First argument should be an array' )) :
                case (false !== stripos ( $sMessage, 'Invalid argument supplied for foreach' )) :
                    $tipo = 'PR';
                    break;
                // Queda no banco
                case (false !== stripos ( $sMessage, 'no working server connection' )) :
                case (false !== stripos ( $sMessage, 'terminating connection due to' )) :
                case (false !== stripos ( $sMessage, 'deadlock detected' )) :
                    $tipo = 'QB';
                    break;
                // Diversos (detectar aos poucos)
                default :
                    $tipo = 'DV';
                    break;
            }

            $errdata = date ( 'Y-m-d H:i:s' );
            $errlinha = $info ['line'] ? $info ['line'] : "NULL";
            $errarquivo = $info ['file'] ? "'" . substr ( str_replace ( '\\', '/', $info ['file'] ), 0, 300 ) . "'" : "NULL";
            $sisid = $_SESSION ['sisid'] ? $_SESSION ['sisid'] : "NULL";
            $usucpf = $_SESSION ['usucpforigem'] ? "'" . $_SESSION ['usucpforigem'] . "'" : "NULL";

            $errdescricao = str_replace ( "<q>", "\n", $sMessage );
            $errdescricao = substr ( str_replace ( "'", "''", $errdescricao ), 0, 4000 );

            $sql = "INSERT INTO seguranca.erro( sisid, errdata, errtipo, errdescricao, errarquivo, errlinha, usucpf)
                    VALUES ($sisid, '$errdata', '$tipo', '$errdescricao', $errarquivo, $errlinha,  $usucpf);";

            try {
                $db->executar ( $sql );
                $db->commit ();
                $db->close();
            } catch ( Exception $e ) {
                if (isset($db) && is_object ( $db )) {
                    $db->rollback();
                    $db->close();
                }
            }
        }
    }

    /**
     * Grava Log de erro
     *
     * @param Text $sMessage
     * @return void
     */
    function gravarLogArquivo($sMessage) {

        $oTrace = new BackTrace ();
        $oTrace->levelDown ( 3 );

        do {
            $info = $oTrace->getCurrent ();
            if ($info ['file'] && 'classes_simec.inc' != substr ( $info ['file'], - 17 )) {
                break;
            }
        } while ( $oTrace->levelDown () === true );

        $errdata = date ( 'Y-m-d H:i:s' );
        $errlinha = $info ['line'] ? $info ['line'] : "NULL";
        $errarquivo = $info ['file'] ? "'" . substr ( str_replace ( '\\', '/', $info ['file'] ), 0, 300 ) . "'" : "NULL";
        $sisid = $_SESSION ['sisid'] ? $_SESSION ['sisid'] : "NULL";
        $usucpf = $_SESSION ['usucpforigem'] ? "'" . $_SESSION ['usucpforigem'] . "'" : "NULL";

        $nomeArquivo = 'log_erro_' . date("Ymd") . '.log';
        $pathRaiz = APPRAIZ . 'arquivos/log_erro/';

        if(!is_dir($pathRaiz)){
            mkdir($pathRaiz, 0777);
        }


        $arquivo = fopen($pathRaiz . $nomeArquivo, "a+");

        $registroExiste = false;
        while(!feof($arquivo)) {
            // lê uma linha do arquivo
            $linha = fgets($arquivo, 4096);
            if(false !== strpos($linha, ";{$errlinha};{$errarquivo}")){
                $registroExiste = true;
                break;
            }
        }

        if(!$registroExiste){
            $log = "{$errdata};{$errlinha};{$errarquivo};{$sisid};{$usucpf}\r\n";
            fwrite($arquivo, $log);
        }

        fclose($arquivo);

        return $registroExiste;
    }

    /**
     * Grava Log de erro
     *
     * @param Text $sMessage
     * @return void
     */
    function gravarErroMigracao($sMessage, $iCode) {
        global $servidor_bd, $porta_bd, $nome_bd, $usuario_db, $senha_bd;

        $conn_string = "host=$servidor_bd port=$porta_bd dbname=$nome_bd user=$usuario_db password=$senha_bd";
        $dbErro = pg_connect ( $conn_string );

        $oTrace = new BackTrace ();
        $oTrace->levelDown ( 3 );

        do {
            $info = $oTrace->getCurrent ();
            if ($info ['file'] && 'classes_simec.inc' != substr ( $info ['file'], - 17 )) {
                break;
            }
        } while ( $oTrace->levelDown () === true );

        $errdata = date ( 'Y-m-d H:i:s' );
        $errlinha = $info ['line'] ? $info ['line'] : "NULL";
        $errarquivo = $info ['file'] ? "'" . substr ( str_replace ( '\\', '/', $info ['file'] ), 0, 300 ) . "'" : "NULL";
        $sisid = $_SESSION ['sisid'] ? $_SESSION ['sisid'] : "NULL";
        $usucpf = $_SESSION ['usucpforigem'] ? "'" . $_SESSION ['usucpforigem'] . "'" : "NULL";

        $errdescricao = str_replace ( "<q>", "\n", $sMessage );
        $errdescricao = substr ( str_replace ( "'", "''", $errdescricao ), 0, 4000 );

        $sql = "INSERT INTO seguranca.erromigracao( sisid, errdata, errtipo, errdescricao, errarquivo, errlinha, usucpf)
                VALUES ($sisid, '$errdata', '$iCode', '$errdescricao', $errarquivo, $errlinha,  $usucpf);";

        try {
            @pg_query ( $dbErro, $sql );
            @pg_query ( $dbErro, 'commit; ' );
        } catch ( Exception $e ) {
        }

        @pg_close ( $dbErro );
    }

    /**
     * Grava Log de erro
     *
     * @param Text $msgLog
     * @return void
     */
    function grava($msgLog) {
    	$nomeArquivo = 'log_auditoria_' . date("Ymd") . '.log';
    	$pathRaiz = APPRAIZ . 'arquivos/log_auditoria/';
    	
    	if (!is_dir($pathRaiz)) {
    		mkdir($pathRaiz, 0777);
    	}
    	
        $msgLog = str_replace ( "<q>", "\n", $msgLog );
        $msgLog = str_replace ( "'", "''", $msgLog );
        $tabelaAuditoria = 'auditoria_' . date('Y_m');

        $sql = "insert into {$tabelaAuditoria} (auddata, audsql,usucpf,audmsg,audip,mnuid, audtipo) values ('" . date ( "d-m-Y H:i:s" ) . "','" . str_replace ( "'", "''", $_SESSION ['sql'] ) . "','" . $_SESSION ['usucpf'] . "','" . $msgLog . "','" . $_SESSION ['ip'] . "','" . $_SESSION ['mnuid'] . "','X')";
        $sql = utf8_encode( $sql );
        
        $arquivo = fopen($pathRaiz . $nomeArquivo, "a+");
        $log = "{$sql};\r\n";
        fwrite($arquivo, $log);
        fclose($arquivo);
    }

    /**
     * Envia email de erro para administrador
     *
     * @param Text $msgLog
     * @return void
     */
    function enviaEmail($msgLog) {
        $msgLog = str_replace ( "<q>", "<br>", $msgLog );
        $assunto = 'Erro no '. SIGLA_SISTEMA . ' ' . date ( "d-m-Y H:i:s" ) . " - " . $_SESSION ['ambiente'];

        $aDestinatarios = carregarUsuariosSessao();
        $remetente     = array("nome"=>SIGLA_SISTEMA. " - ".strtoupper($_SESSION['sisdiretorio'])." - " . $_SESSION['usunome'] . " - " . $_SESSION['usuorgao'], "email"=>EMAIL_SISTEMA_NOREPLY);
        $destinatarios = !empty($aDestinatarios[$_SESSION['sisid']]) ? $aDestinatarios[$_SESSION['sisid']] : array_keys($aDestinatarios['todos']);

        simec_email($remetente, $destinatarios, $assunto, $msgLog);
    }

    /**
     * Mostra mensagem para o usuário
     *
     * @param Text $msgLog
     * @return void
     */
    function msgErro($iCode = null, $sMessage = null, $sFile = null, $iLine = null, $aContext = array()) {
        ?><script>alert('Ocorreu uma falha inesperada e sua operação não foi executada.\nO problema foi enviado automaticamente aos administradores do sistema para análise,\nse necessário entre em contato com o setor responsável.\n');history.back();</script><?php
        exit ();
    }

    /**
     * Manipula erros disparados durante a execução.
     *
     * Captura todos os erros disparados durante a execução.
     *
     * @param integer $iCode
     * @param string $sMessage
     * @param string $sFile
     * @param integer $iLine
     * @param array $aContext
     * @return void
     * @see set_error_handler
     */
    public function handle($iCode, $sMessage, $sFile, $iLine, array $aContext = array()) {
        global $db;

        // Ignora somente deprecated
        if ($iCode == E_DEPRECATED) {
            if (!IS_PRODUCAO) {
                $this->gravarErroMigracao ($sMessage, $iCode);
            }
            return false;
        }
//ver($iCode, $sMessage, $sFile, $iLine, $aContext, d);
        // Ignorando erros de pg_last_error() e pg_errormessage()
        if(false !== strpos($sMessage, 'pg_last_error():') || false !== strpos($sMessage, 'pg_errormessage():')  || false !== strpos($sMessage, 'session data (memcache)')){
            return false;
        }

        if (IS_PRODUCAO && function_exists ( 'zend_monitor_custom_event' )) {
        	// Verificando se esta em horario de manutenção para ignorar erros de aplicação
        	if ( !(date('Gi') >= 300 && date('Gi') <= 331) ) {
	        	if (!preg_match("/pg_connect()/i", $sMessage) && !preg_match("/pg_query()/i", $sMessage)) {
		        	$aBacktrace = debug_backtrace();
		        	
		        	if ($aBacktrace) {
		        		$aUltimoErro = end($aBacktrace);
		        		$sFile = $aUltimoErro['file'];
		        	}
		        	
		        	ob_start ();
		        	print_r ( $aContext );
		        	$sContexto = ob_get_contents ();
		        	ob_clean ();
		        	ob_flush ();
		        	
		            ob_start ();
		            zend_monitor_set_aggregation_hint($sFile . $iLine);
		            zend_monitor_custom_event ( $sFile, $sMessage, "\r\nMensagem: {$sMessage} \r\nArquivo: {$sFile} {$iLine} \r\n\nContexto: {$sContexto}" );
		            ob_clean ();
		        	ob_flush ();
	        	}
        	}
        }

        // Ignora deprecated e warning de DESENVOLVIMENTO e grava um log para posterior verificação
        if (! isset ( $_SESSION ['usucpf'] )) {
            session_unset ();
            $_SESSION ['MSG_AVISO'] = 'Sua sessão expirou. Efetue login novamente.';
            header ( 'Location: login.php' );
            exit ();
        }

        // cancela a transacao, caso ela exista;
        if (isset ( $_SESSION ['transacao'] )) {
            pg_query ( 'rollback;' );
            unset ( $_SESSION ['transacao'] );
        }

        ob_start();
        $msgLog = $this->display ( $iCode, $sMessage, $sFile, $iLine, $aContext );
        ob_clean();

        /*
         * Trecho que limpa o código HTML com o número excessivo de <span>
         */
        $msgLog = str_replace ( array (
            "<span style=\"color: #007700\"></span>",
            "<span style=\"color: #0000BB\"></span>",
            "<span style=\"color: #DD0000\"></span>",
            "<span style=\"color: #FF8000\"></span>"
        ), array (
            "",
            "",
            "",
            ""
        ), $msgLog );
        /*
         * FIM - Trecho que limpa o código HTML com o número excessivo de <span>
         */
        // gravando no arquivo de log
        if (IS_PRODUCAO) {

            if (false !== strpos($sMessage, 'pg_connect(): ')) {
                $this->msgErro ($iCode, $sMessage, $sFile, $iLine, $aContext);
                exit ();
            }

            @$this->grava ($sMessage);

            $registroExiste = $this->gravarLogArquivo($sMessage);
            if(!$registroExiste){
                @$this->enviaEmail ($msgLog);
            }

            @$this->gravarErro ($sMessage);

            // Não para a aplicação em caso de warning
            if ($iCode == E_WARNING) {
                return false;
            }
            $this->msgErro ($iCode, $sMessage, $sFile, $iLine, $aContext);
        } else {
            $this->gravarErro($sMessage);
            print $msgLog;
        }

        exit ();
    }

    /**
     *
     * @return void
     */
    public static function start() {
        if (self::$oHandler === null) {
            self::$oHandler = new ErrorHandler ();
            $cError = array (
                self::$oHandler,
                'handle'
            );
            set_error_handler ( $cError, E_ALL & ~ E_NOTICE & ~ E_STRICT );
        }
    }
}
