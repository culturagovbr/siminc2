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
     * Quando a prote��o de senhas estiver ativa, aqui ser� armazenada a
     * tag que cont�m o password.
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
        }

        if (!is_null($classMap)) {
            $options->merge(array('classmap' => $classMap->asArray()));
        }

        $this->urlWSDL = $urlWSDL;

        // -- Op��es da chamada aos m�todos do webservice
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
     * Ativa a prote��o de password.
     *
     * @param String $passwordTag A tag de password que deve ser ofuscada.
     * @throws Exception Lan�ada se a tag de password informada for vazia.
     */
    public function protectPassword($passwordTag)
    {
        if (empty($passwordTag)) {
            throw new Exception('A tag de password a ser protegida n�o pode ser vazia.');
        }
        $this->protectedPasswordTag = $passwordTag;
    }

    /**
     * Faz a chamado de um m�todo do webservice.<br />
     * Se o log de fun��es estiver ligado, faz a grava��o dos logs da chamada (pr� e p�s chamada).
     *
     * @param string $function O nome do m�todo do websevice que ser� executado.
     * @param array $arguments Lista de par�metros do m�otodo que ser� executado.
     * @return SoapFault|mixed
     */
    public function call($function, array $arguments)
    {
        // -- Informa��es iniciais de log
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

        // -- Finaliza a coleta de log e faz a grava��o
        if ($this->logger) {
            $lastRequest = parent::__getLastRequest();
            // -- Removendo o password do XML de requisi��o, se necess�rio
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
     * @param string $type Tipo do handler de log que ser� criado: db, file, screen.
     * @param array $config Configura��es do handler, verificar em cada tipo.
     * @see Simec_SoapClient_Log_LoggerDb
     */
    public function startLogger($type, array $config)
    {
        $this->logger = Simec_SoapClient_Log::getLogger($type);
        $this->logger->setConfig($config);
    }
}
