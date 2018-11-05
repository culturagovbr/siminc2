<?php
/**
 * $Id: SoapClient.php 98323 2015-06-08 19:29:06Z OrionMesquita $
 */

/**
 * @see Options.php
 */
require_once(dirname(__FILE__) . "/SoapClient/Options.php");

/**
 * @see ClassMap.php
 */
require_once(dirname(__FILE__) . "/SoapClient/ClassMap.php");

/**
 * @see Log.php
 */
require_once(dirname(__FILE__) . '/SoapClient/Log.php');


/**
 * @todo Chamadas de log
 */
class Simec_SoapClient extends SoapClient {

    private $callOptions;

    private $inputHeaders = null;

    private $outputHeaders;

    private $logger;

    protected $urlWSDL;

    /**
     * Quando a proteção de senhas estiver ativa, aqui será armazenada a
     * tag que contém o password.
     * @var String
     */
    protected $protectedPasswordTag;

    public function __construct($urlWSDL, Simec_SoapClient_Options $options = null, Simec_SoapClient_ClassMap $classMap = null)
    {
        if (is_null($options)) {
            $options = new Simec_SoapClient_Options();
            $options->add('trace', 1);
            $options->add('exceptions', true);
            $options->add('cache_wsdl', WSDL_CACHE_NONE);
            $options->add('ssl_method', SOAP_SSL_METHOD_SSLv23);
        }

            $options->add('stream_context', stream_context_create(array(
                "ssl" => array(
                    "verify_peer" => FALSE
            ))));

        if (!is_null($classMap)) {
            $options->merge(array('classmap' => $classMap->asArray()));
        }

        $this->urlWSDL = $urlWSDL;

        // -- Opções da chamada aos métodos do webservice
        $this->callOptions = new Simec_SoapClient_Options();

        // -- Chamada ao webservice
        parent::__construct($urlWSDL, $options->asArray());
    }

    public function setCallOption($elem, $key)
    {
        $this->callOptions->add($elem, $key);
        return $this;
    }

    public function setCallOptions(Array $elems)
    {
        $this->callOptions = $elems;
        return $this;
    }

    public function setInputHeaders(SoapHeader $headers)
    {
        $this->inputHeaders = $headers;
        parent::__setSoapHeaders($headers);
        return $this;
    }

    public function getOutputHeaders()
    {
        return $this->outputHeaders;
    }

    /**
     * Ativa a proteção de password.
     *
     * @param String $passwordTag A tag de password que deve ser ofuscada.
     * @throws Exception Lançada se a tag de password informada for vazia.
     */
    public function protectPassword($passwordTag)
    {
        if (empty($passwordTag)) {
            throw new Exception('A tag de password a ser protegida não pode ser vazia.');
        }
        $this->protectedPasswordTag = $passwordTag;
    }

    /**
     * Faz a chamado de um método do webservice.<br />
     * Se o log de funções estiver ligado, faz a gravação dos logs da chamada (pré e pós chamada).
     *
     * @param string $function O nome do método do websevice que será executado.
     * @param array $arguments Lista de parâmetros do méotodo que será executado.
     * @return SoapFault|mixed
     */
    public function call($function, array $arguments)
    {
        // -- Informações iniciais de log
        $isError = 'FALSE';
        if ($this->logger) {
            $requestDate = new DateTime();
        }

        try {
            $response = $this->__soapCall($function, $arguments);
            $responseDate = new DateTime();
        } catch (Exception $sf) {
            $isError = 'TRUE';
            $responseDate = new DateTime();
            $response = $sf;
        }

        // -- Finaliza a coleta de log e faz a gravação
        if ($this->logger) {
            $lastRequest = parent::__getLastRequest();
            // -- Removendo o password do XML de requisição, se necessário
            if (isset($this->protectedPasswordTag)) {
                $lastRequest = preg_replace(
                    "|>.+</{$this->protectedPasswordTag}>|",
                    ">Senha removida</{$this->protectedPasswordTag}>",
                    $lastRequest
                );
            }

            $this->logger->writeLog(
                array(
                    'requestContent' => $lastRequest,
                    'requestHeader' => parent::__getLastRequestHeaders(),
                    'requestTimestamp' => $requestDate->format('Y-m-d H:i:s'),
                    'responseContent' => str_replace("'", "''", parent::__getLastResponse()),
                    'responseHeader' => str_replace("'", "''", parent::__getLastResponseHeaders()),
                    'responseTimestamp' => $responseDate->format('Y-m-d H:i:s'),
                    'url' => $this->urlWSDL,
                    'method' => $function,
                    'ehErro' => $isError
                )
            );
        }

        return $response;
    }

    /**
     * Instancia o log de chamadas.
     *
     * @param string $type Tipo do handler de log que será criado: db, file, screen.
     * @param array $config Configurações do handler, verificar em cada tipo.
     * @see Simec_SoapClient_Log_LoggerDb
     */
    public function startLogger($type, array $config)
    {
        $this->logger = Simec_SoapClient_Log::getLogger($type);
        $this->logger->setConfig($config);
    }
}
