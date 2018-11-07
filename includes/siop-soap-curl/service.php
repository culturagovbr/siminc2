<?php

include_once APPRAIZ. 'includes/soap-curl/client.php';

/**
 * Classe para conectar com o Webservice do SIOP atrav�s do componente SoapCurl
 * 
 */
abstract class SiopSoapCurl_Service {
    
    /**
     * Url do servi�o
     * 
     * @var string
     */
    protected $url;

    /**
     * Documento XML
     * 
     * @var string
     */
    protected $xml;
    
    /**
     * Cliente usado pra comunicar com o servi�o
     * 
     * @var SoapCurl_Client
     */
    protected $client;
    
    public function getUrl() {
        return $this->url;
    }

    public function getXml() {
        return $this->xml;
    }

    public function getClient() {
        return $this->client;
    }

    public function setUrl($url) {
        $this->url = $url;
        return $this;
    }

    public function setXml($xml) {
        $this->xml = $xml;
        return $this;
    }

    public function setClient(SoapCurl_Client $client) {
        $this->client = $client;
        return $this;
    }

    public function __construct($url, $xml, SoapCurl_Client $client) {
        $this->url = $url;
        $this->xml = $xml;
        $this->client = $client? $client: new SoapCurl_Client();
    }

    protected function mountListHeader(){
        $listHeader = array(
            "Content-type: text/xml;charset=\"utf-8\"",
            "Accept: text/xml",
            "Cache-Control: no-cache",
            "Pragma: no-cache",
            "SOAPAction: ". $this->url,
            "Content-length: ". strlen($this->xml)
        );
        
        return $listHeader;
    }
    
    protected function configure(){
        # Configurado Protocolo de seguran�a
        $this->client->getSsl()
            ->setVerifyPeer(FALSE)
            ->setVerifyHost(FALSE)
            ->setVersion(3)
            ->setCertificate(WEB_SERVICE_SIOP_CERTIFICADO)
            ->setPassword('simec')
        ;
        
        # Configurando Requisi��o
        $this->client->getHttp()
            ->setWsdl($this->url. '?wsdl')
            ->setReturn(TRUE)
            ->setUser(WEB_SERVICE_SIOP_USUARIO)
            ->setPassword(WEB_SERVICE_SIOP_SENHA)
            ->setAuth(CURLAUTH_ANY)
            ->setTimeout(180)
            ->setPost(TRUE)
            ->setListHeader($this->mountListHeader())
        ;
        
        # Configurando xml contendo o nome e parametros utilizados pelo servi�o especifico
        $this->client
            ->setXml($this->xml);
    }

}