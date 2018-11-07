<?php

include_once 'interface-service.php';
include_once APPRAIZ. 'includes/soap-curl/client.php';

/**
 * Classe para conectar com o Webservice do SIOP através do componente SoapCurl
 * 
 */
abstract class SiopSoapCurl_Service implements SiopSoapCurl_InterfaceService {
    
    /**
     * Url do serviço
     * 
     * @var string
     */
    protected $url;

    /**
     * Cliente usado pra comunicar com o serviço
     * 
     * @var SoapCurl_Client
     */
    protected $client;
    
    public function getUrl() {
        return $this->url;
    }

    public function getClient() {
        return $this->client;
    }

    public function setUrl($url) {
        $this->url = $url;
        return $this;
    }

    public function setClient(SoapCurl_Client $client) {
        $this->client = $client;
        return $this;
    }
  
    public function __construct() {
        $this->client = new SoapCurl_Client();
    }
    
    protected function mountListHeader(){
        $listHeader = array(
            "Content-type: text/xml;charset=\"utf-8\"",
            "Accept: text/xml",
            "Cache-Control: no-cache",
            "Pragma: no-cache",
            "SOAPAction: ". $this->url,
            "Content-length: ". strlen($this->loadXml())
        );
        
        return $listHeader;
    }
    
    public function configure(){
        # Configurado Protocolo de segurança
        $this->client->getSsl()
            ->setVerifyPeer(FALSE)
            ->setVerifyHost(FALSE)
            ->setVersion('3')
            ->setCertificate(WEB_SERVICE_SIOP_CERTIFICADO)
            ->setPassword('simec')
        ;
        
        # Configurando Requisição
        $this->client->getHttp()
            ->setWsdl($this->url. '?wsdl')
            ->setReturn(TRUE)
            ->setAuth(CURLAUTH_ANY)
            ->setTimeout(180)
            ->setPost(TRUE)
            ->setListHeader($this->mountListHeader())
        ;
        
        # Configurando xml contendo o nome e parametros utilizados pelo serviço especifico
        $this->client
            ->setXml($this->loadXml())->configureAll();
        
        return $this;
    }

    /**
     * Faz requisição ao serviço
     * 
     * @return string
     */
    public function request(){
        $this->client->request();
        return $this->client->getResponse();
    }
    
}