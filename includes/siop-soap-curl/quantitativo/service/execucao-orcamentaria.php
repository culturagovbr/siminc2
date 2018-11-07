<?php

include_once APPRAIZ. 'includes/soap-curl/client.php';

/**
 * Classe para conectar com o Webservice do SIOP através do componente SoapCurl
 * 
 */
class SiopSoapCurl_Quantitativo_Service_ExecucaoOrcamentaria extends SiopSoapCurl_Service {
    
    /**
     * Url do serviço
     * 
     * @var string
     */
    private $url = WEB_SERVICE_SIOP_URL. 'WSQuantitativo';

    /**
     * Documento XML
     * 
     * @var SiopSoapCurl_Quantitativo_Xml_ExecucaoOrcamentaria
     */
    private $xml;

    public function getXml() {
        return $this->xml;
    }

    public function setXml(SiopSoapCurl_Quantitativo_Xml_ExecucaoOrcamentaria $xml) {
        $this->xml = $xml;
        return $this;
    }
    
//    public function __construct($url = WEB_SERVICE_SIOP_URL. 'WSQuantitativo', $xml, SoapCurl_Client $client) {
//        $this->url = $url;
//        $this->xml = $xml;
//        $this->client = $client? $client: new SoapCurl_Client();
//    }

}