<?php

/**
 * @author Renan de Lima Barbosa <renandelima@gmail.com>
 * @author Renê de Lima Barbosa <renedelima@gmail.com>
 */
class ExceptionHandler {

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
     * @param Exception $oException
     * @return void
     */
    public function display(Exception $oException) {
        $aTrace = $oException->getTrace ();

        array_unshift ( $aTrace, array (
            'function' => '__construct',
            'line' => $oException->getLine (),
            'file' => $oException->getFile (),
            'class' => get_class ( $oException ),
            'object' => null,
            'type' => '::',
            'args' => array (
                $oException->getMessage (),
                $oException->getCode ()
            )
        ) );
        $oTrace = new BackTrace ();
        ob_start();
        print "<pre>";
        print '#> ' . $oException->getFile() . "<br>";
        print_r( $oException->getTraceAsString() );
        $oTrace->explain();
        $sTrace = ob_get_contents();;
        ob_clean();

        ob_start();
        print_r( $_REQUEST );
        $v_requ = ob_get_contents();
        ob_clean();

        ob_start();
        print_r( $_POST );
        $v_post = ob_get_contents ();
        ob_clean();

        ob_start();
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
        print_r( $session );
        $v_session = ob_get_contents();
        ob_clean();

        ob_start();
        print_r( $_SERVER );
        $v_server = ob_get_contents();
        ob_clean();

        $msg_erro = "
	   <fieldset>
			   <legend>" . get_class ( $oException ) . " - " . $_SESSION ['ambiente'] . "</legend>
			   <pre><b>" . $oException->getMessage () . "</b></pre>" . $sTrace . "
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
			   <legend><b>Variaveis de _SERVER</b></legend>
			   <pre>" . $v_server . "</pre>
	   </fieldset>        ";

        return $msg_erro;
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
    function msgErro($msgLog) {
        ?><script>alert('Ocorreu uma falha inesperada e sua operação não foi executada.\nO problema foi enviado automaticamente aos administradores do sistema para análise,\nse necessário entre em contato com o setor responsável.\n');history.back();</script><?php

        exit;
    }

    /**
     * Manipula exceções disparadas durante a execução.
     *
     * @param Exception $oException
     * @return void
     * @see set_exception_handler
     */
    public function handle(Exception $oException) {
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

        $msgLog = $this->display ( $oException );

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
            // enviar um e-mail de aviso quando acontecer algum erro
            $this->enviaEmail ( $msgLog );
            // Mostra Mensagem de erro e encerra o programa
            $this->msgErro ( $msgLog );
        } else {
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
            self::$oHandler = new ExceptionHandler ();
            $cException = array (
                self::$oHandler,
                'handle'
            );
            set_exception_handler ( $cException );
        }
    }
}
