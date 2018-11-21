<?php

/**
 * Classe para conectar com o Webservice do SIOP atrav�s do componente SoapCurl
 * 
 */
class SiopSoapCurl_Receita_Service_CaptarBaseExterna extends SiopSoapCurl_Service {

    /**
     * Url do servi�o
     * 
     * @var string
     */
    protected $url = WEB_SERVICE_SIOP_URL. 'WSReceita';
    
    /**
     * Documento XML
     * 
     * @var SiopSoapCurl_Receita_Xml_CaptarBaseExterna
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

    public function setXml(SiopSoapCurl_Receita_Xml_CaptarBaseExterna $xml) {
        $this->xml = $xml;
        return $this;
    }

    public function __construct() {
        parent::__construct();
        $this->xml = new SiopSoapCurl_Receita_Xml_CaptarBaseExterna();
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
//    public function request() {
//        $result = parent::request();
//        $listas = new stdClass();
//        $listas->return = new stdClass();
//
//        foreach($result as $nome => $registro){
//            if(strpos($nome, 'DTO')){
//                if(!$listas->return->$nome){
//                    $listas->return->$nome = array();
//                }
//                array_push($listas->return->$nome, $registro);
//            }
//        }
//
//        return $listas;
//    }

}

