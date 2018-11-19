<?php

/**
 * Classe para conectar com o Webservice do SIOP atrav�s do componente SoapCurl
 * 
 */
class SiopSoapCurl_Quantitativo_Service_ProgramacaoCompleta extends SiopSoapCurl_Service {

    /**
     * Url do servi�o
     * 
     * @var string
     */
    protected $url = WEB_SERVICE_SIOP_URL. 'WSQualitativo';
    
    /**
     * Documento XML
     * 
     * @var SiopSoapCurl_Quantitativo_Xml_ProgramacaoCompleta
     */
    protected $xml;
    
    public function getUrl() {
        return $this->url;
    }

    public function getXml() {
        return $this->xml;
    }

    public function setUrl($url) {
        $this->url = $url;
        return $this;
    }

    public function setXml(SiopSoapCurl_Quantitativo_Xml_ProgramacaoCompleta $xml) {
        $this->xml = $xml;
        return $this;
    }

    public function __construct() {
        parent::__construct();
        $this->xml = new SiopSoapCurl_Quantitativo_Xml_ProgramacaoCompleta();
    }
    
    /**
     * Retorna o XML pra ser enviado no momento da requisi��o
     * 
     * @return string xml
     */
    public function loadXml() {
        $documento = $this->xml->describe();
        
        return $documento;
    }
    
    /**
     * Faz requisi��o ao servi�o e retorna lista de a��es, localizadores e Planos Or�ament�rios(POs)
     * 
     * @return array
     */
    public function request() {
        $result = parent::request();
        $listas = new stdClass();
        $listas->return = new stdClass();

        foreach($result as $nome => $registro){
            if(strpos($nome, 'DTO')){
                if(!$listas->return->$nome){
                    $listas->return->$nome = array();
                }
                array_push($listas->return->$nome, $registro);
            }
        }

        return $listas;
    }

}

