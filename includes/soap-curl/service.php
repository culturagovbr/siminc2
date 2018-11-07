<?php

include_once 'http.php';
include_once 'ssl.php';

/**
 * Classe principal que compoem o componente de comunicação via SOAP.
 * 
 */
class SoapCurl_Service {
    
    /**
     * Conexão com o provedor do serviço
     * 
     * @var resource
     */
    public static $resource;
    
    /**
     * Protocolo de comunicação
     * 
     * @var SoapCurl_Http
     */
    private $http;
    
    /**
     * Protocolo de segurança
     * 
     * @var SoapCurl_Ssl
     */
    private $ssl;
    
    /**
     * Lista de dados para serem enviados na requisição
     * 
     * @var array
     */
    private $listField;
    
    /**
     * Documento xml para envio na requisição
     * 
     * @var string
     */    
    private $xml;

    /**
     * Caminho fisico do arquivo para ser enviado na requisição
     * 
     * @var string
     */
    private $file;
    
    /**
     * Resposta da requisição
     * 
     * @var string
     */
    private $response;

    public function getHttp() {
        return $this->http;
    }

    public function getSsl() {
        return $this->ssl;
    }

    public function getListField() {
        return $this->listField;
    }

    public function getXml() {
        return $this->xml;
    }

    public function getFile() {
        return $this->file;
    }

    public function getResponse() {
        return $this->response;
    }

    public function setHttp(SoapCurl_Http $http) {
        $this->http = $http;
        return $this;
    }

    public function setSsl(SoapCurl_Ssl $ssl) {
        $this->ssl = $ssl;
        return $this;
    }

    public function setListField($listField) {
        $this->listField = $listField;
        return $this;
    }

    public function setXml($xml) {
        $this->xml = $xml;
        return $this;
    }

    public function setFile($file) {
        $this->file = $file;
        return $this;
    }

    public function setResponse($response) {
        $this->response = $response;
        return $this;
    }

    /**
     * Manipula a comunicação via SOAP.
     * 
     * @param SoapCurl_Http $http
     * @param SoapCurl_Ssl $ssl
     * @param array $listField
     * @param string $xml
     * @param string $file
     * @param string $response
     */
    public function __construct(SoapCurl_Http $http = NULL, SoapCurl_Ssl $ssl = NULL, $listField = NULL, $xml = NULL, $file = NULL, $response = NULL) {
        self::$resource = $resource? $resource: curl_init();
        $this->http = $http;
        $this->ssl = $ssl;
        $this->listField = $listField;
        $this->xml = $xml;
        $this->file = $file;
        $this->response = $response;
    }
    
    public function configureHttp(){
        $this->http->configureAll(self::$resource);
        return $this;
    }
    
    public function configureSsl(){
        $this->ssl->configureAll(self::$resource);
        return $this;
    }
    
    public function configureListField(){
        if($this->listField){
            curl_setopt(self::$resource, CURLOPT_POSTFIELDS, $this->listField);
        }
        return $this;
    }
    
    public function configureXml(){
        if($this->xml){
            curl_setopt(self::$resource, CURLOPT_POSTFIELDS, $this->xml);
        }
        return $this;
    }
    
    public function configureFile(){
        if($this->file){
            curl_setopt(self::$resource, CURLOPT_POSTFIELDS, $this->file);
        }
        return $this;
    }
    
    /**
     * Faz requisição de forma segura configurando todos as opções e encerrando a sessão.
     * 
     * @return string
     */
    public function request(){
        $this->configureHttp()
            ->configureSsl()
            ->configureListField()
            ->configureXml()
            ->configureFile()
            ->execute()
            ->close();
        return $this->response;
    }
    
    /**
     * Faz a requisição e retorna a resposta, mas os recursos usados pemanecem em aberto.
     * 
     * @return $this
     */
    public function execute(){
        $this->response = curl_exec(self::$resource);
        return $this;
    }
    
    /**
     * Encerra a sessão da conexão e encerra todos os recursos utilizados
     * 
     * @return $this
     */
    public function close(){
        curl_close(self::$resource);
        return $this;
    }

    /**
     * Ao descarregar o objeto da mémoria encerra a sessão e os recursos
     *
     * @return VOID
     */
    public function __destruct(){
        $this->close();
    }
    
}
