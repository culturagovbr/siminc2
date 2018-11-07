<?php

include_once 'http.php';
include_once 'ssl.php';

/**
 * Classe principal que compoem o componente de comunica��o via SOAP.
 * 
 */
class SoapCurl_Client {
    
    /**
     * Conex�o com o provedor do servi�o
     * 
     * @var resource
     */
    public static $resource;
    
    /**
     * Protocolo de comunica��o
     * 
     * @var SoapCurl_Http
     */
    private $http;
    
    /**
     * Protocolo de seguran�a
     * 
     * @var SoapCurl_Ssl
     */
    private $ssl;
    
    /**
     * Lista de dados para serem enviados na requisi��o
     * 
     * @var array
     */
    private $listField;
    
    /**
     * Documento xml para envio na requisi��o
     * 
     * @var string
     */    
    private $xml;

    /**
     * Caminho fisico do arquivo para ser enviado na requisi��o
     * 
     * @var string
     */
    private $file;
    
    /**
     * Resposta da requisi��o
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
     * Manipula a comunica��o via SOAP.
     * 
     * @param SoapCurl_Http $http
     * @param SoapCurl_Ssl $ssl
     * @param array $listField
     * @param string $xml
     * @param string $file
     * @param string $response
     */
    public function __construct(SoapCurl_Http $http = NULL, SoapCurl_Ssl $ssl = NULL, $listField = NULL, $xml = NULL, $file = NULL, $response = NULL) {
        if(isset(self::$resource)){
            $this->close();
        }
        self::$resource = curl_init();
        $this->http = $http? $http: new SoapCurl_Http();
        $this->ssl = $ssl? $ssl: new SoapCurl_Ssl();
        $this->listField = $listField;
        $this->xml = $xml;
        $this->file = $file;
        $this->response = $response;
    }
    
    public function configureHttp(){
        $this->http->configureAll();
        return $this;
    }
    
    public function configureSsl(){
        $this->ssl->configureAll();
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
    
    public function configureAll() {
        $this->configureSsl()
            ->configureHttp()
            ->configureListField()
            ->configureXml()
            ->configureFile();
        
        return $this;
    }
    
    /**
     * Faz requisi��o de forma segura configurando todos as op��es e encerrando a sess�o.
     * 
     * @return string
     */
    public function request(){
        $this->configureAll()
            ->execute()
            ->close();
        return $this->response;
    }
    
    /**
     * Faz a requisi��o e retorna a resposta, mas os recursos usados pemanecem em aberto.
     * 
     * @return $this
     */
    public function execute(){
        $this->response = curl_exec(self::$resource);
        return $this;
    }
    
    /**
     * Encerra a sess�o da conex�o e encerra todos os recursos utilizados
     * 
     * @return $this
     */
    public function close(){
        curl_close(self::$resource);
        return $this;
    }
    
}
