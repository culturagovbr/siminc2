<?php

include_once "soap-curl-http.php";
include_once "soap-curl-ssl.php";

class soapCurl {
    
    /**
     * Conexão com o provedor do serviço
     * 
     * @var resource
     */
    private $resource;
    
    /**
     * Protocolo de comunicação
     * 
     * @var soapCurlHttp
     */
    private $http;
    
    /**
     * Protocolo de segurança
     * 
     * @var soapCurlSsl
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

    public function getResource() {
        return $this->resource;
    }

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

    public function setResource($resource) {
        $this->resource = $resource;
        return $this;
    }

    public function setHttp(soapCurlHttp $http) {
        $this->http = $http;
        return $this;
    }

    public function setSsl(soapCurlSsl $ssl) {
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
    
    public function getResponse() {
        return $this->response;
    }

    public function setResponse($response) {
        $this->response = $response;
        return $this;
    }

    public function __construct($resource = NULL, soapCurlHttp $http = NULL, soapCurlSsl $ssl = NULL, $listField = NULL, $xml = NULL, $file = NULL, $response = NULL) {
        $this->resource = $resource;
        $this->http = $http;
        $this->ssl = $ssl;
        $this->listField = $listField;
        $this->xml = $xml;
        $this->file = $file;
        $this->response = $response;
    }
    
    /**
     * Faz requisição de forma segura encerrando a sessão corretamente e retorna a resposta.
     * 
     * @return string
     */
    public function request(){
        $this->execute()
            ->close();
        return $this->response;
    }
    
    /**
     * Faz a requisição e retorna a resposta, mas os recursos usados pemanecem em aberto.
     * 
     * @return $this
     */
    public function execute(){
        $this->response = curl_exec($this->resource);
        return $this;
    }
    
    /**
     * Encerra a sessão da conexão e encerra todos os recursos utilizados
     * 
     * @return $this
     */
    public function close(){
        curl_close($this->resource);
        return $this;
    }

}
