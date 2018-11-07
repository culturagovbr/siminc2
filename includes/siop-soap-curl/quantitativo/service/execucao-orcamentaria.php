<?php

include_once APPRAIZ. 'includes/siop-soap-curl/service.php';
include_once APPRAIZ. 'includes/siop-soap-curl/quantitativo/xml/execucao-orcamentaria.php';

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
    protected $url = WEB_SERVICE_SIOP_URL. 'WSQuantitativo';

    /**
     * Filtro do ano do exercicio
     * 
     * @var int
     */
    protected $ano;
    
    /**
     * Documento XML
     * 
     * @var SiopSoapCurl_Quantitativo_Xml_ExecucaoOrcamentaria
     */
    protected $xml;
    
    public function getUrl() {
        return $this->url;
    }

    public function getAno() {
        return $this->ano;
    }

    public function getXml() {
        return $this->xml;
    }

    public function setUrl($url) {
        $this->url = $url;
        return $this;
    }

    public function setAno($ano) {
        $this->ano = $ano;
        return $this;
    }

    public function setXml(SiopSoapCurl_Quantitativo_Xml_ExecucaoOrcamentaria $xml) {
        $this->xml = $xml;
        return $this;
    }

    public function loadXml() {
        $this->xml = new SiopSoapCurl_Quantitativo_Xml_ExecucaoOrcamentaria();
        $this->xml->setListFilter(array('anoExercicio' => $this->ano));
        $documento = $this->xml->describe();
        
        return $documento;
    }

}