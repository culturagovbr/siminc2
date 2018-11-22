<?php

/**
 * Classe para conectar com o Webservice do SIOP através do componente SoapCurl
 * 
 */
class SiopSoapCurl_Receita_Service_CaptarBaseExterna extends SiopSoapCurl_Service {

    /**
     * Url do serviço
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
     * Retorna o XML pra ser enviado no momento da requisição
     * 
     * @return string xml
     */
    public function loadXml() {
        $documento = $this->xml->describe();
        
        return $documento;
    }
    
    /**
     * Faz requisição ao serviço e retorna lista de ações, localizadores e Planos Orçamentários(POs)
     * 
     * @return array
     */
    public function request() {
        $responseXml = parent::request();

        if(strtolower($responseXml->sucesso) == 'false'){
            $response = new stdClass();
            $response->mensagensErro = utf8_decode($responseXml->mensagensErro);
            $response->sucesso = FALSE;
            $response->captacoesBaseExterna = array();
            foreach($responseXml->captacoesBaseExterna->captacaoBaseExterna as $element => $captacaoBaseExternaXml) {
                $captacaoBaseExterna = new stdClass();
                $captacaoBaseExterna->codigoCaptacaoBaseExterna = current($captacaoBaseExternaXml->codigoCaptacaoBaseExterna);
                $captacaoBaseExterna->descricao = utf8_decode($captacaoBaseExternaXml->descricao);
                $captacaoBaseExterna->detalhesBaseExterna = $captacaoBaseExternaXml->detalhesBaseExterna;
                $response->captacoesBaseExterna[] = $captacaoBaseExterna;
            }
        } else {
            $response = $responseXml;
        }

        return $response;
    }

}

