<?php

include_once APPRAIZ. 'includes/soap-curl/soap-curl.php';

/**
 * Classe para conectar com o Webservice do SIOP através do componente SoapCurl
 * 
 */
class siopSoapCurl {

    /**
     * Protocolo de comunicação
     * 
     * @var soapCurl
     */
    private $soapCurl;

    /**
     * Protocolo de comunicação
     * 
     * @var xmlSiop
     */
    private $xml;
    
    public function getSoapCurl() {
        return $this->soapCurl;
    }

    public function getXml() {
        return $this->xml;
    }

    public function setSoapCurl(soapCurl $soapCurl) {
        $this->soapCurl = $soapCurl;
        return $this;
    }

    public function setXml(xmlSiop $xml) {
        $this->xml = $xml;
        return $this;
    }

    public function __construct(soapCurl $soapCurl, xmlSiop $xml) {
        $this->soapCurl = $soapCurl? $soapCurl: new soapclient();
        $this->xml = $xml;
    }

    public function configure(){
//        $this->soapCurl
    }

}