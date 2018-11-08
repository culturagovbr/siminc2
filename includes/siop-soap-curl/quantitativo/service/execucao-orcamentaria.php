<?php

include_once APPRAIZ. 'includes/siop-soap-curl/service.php';
include_once APPRAIZ. 'includes/siop-soap-curl/quantitativo/xml/execucao-orcamentaria.php';

/**
 * Classe para conectar com o Webservice do SIOP atrav�s do componente SoapCurl
 * 
 */
class SiopSoapCurl_Quantitativo_Service_ExecucaoOrcamentaria extends SiopSoapCurl_Service {

    /**
     * Url do servi�o
     * 
     * @var string
     */
    protected $url = WEB_SERVICE_SIOP_URL. 'WSQuantitativo';
    
    /**
     * Documento XML
     * 
     * @var SiopSoapCurl_Quantitativo_Xml_ExecucaoOrcamentaria
     */
    protected $xml;
    
    /**
     * Filtro do ano do exercicio
     * 
     * @var int
     */
    protected $ano = 2018;
    
    /**
     * Numero da p�gina do controle de pagina��o
     * 
     * @return int
     */
    protected $pagina = 0;

    public function getUrl() {
        return $this->url;
    }

    public function getXml() {
        return $this->xml;
    }

    public function getAno() {
        return $this->ano;
    }

    public function getPagina() {
        return $this->pagina;
    }

    public function setUrl($url) {
        $this->url = $url;
        return $this;
    }

    public function setXml(SiopSoapCurl_Quantitativo_Xml_ExecucaoOrcamentaria $xml) {
        $this->xml = $xml;
        return $this;
    }

    public function setAno($ano) {
        $this->ano = $ano;
        return $this;
    }

    public function setPagina($pagina) {
        $this->pagina = $pagina;
        return $this;
    }
    
    /**
     * Retorna o XML pra ser enviado no momento da requisi��o
     * 
     * @return string xml
     */
    public function loadXml() {
        $this->xml = new SiopSoapCurl_Quantitativo_Xml_ExecucaoOrcamentaria();
        $this->xml->setListFilter(array('anoExercicio' => $this->ano));
        $this->xml->setPage($this->pagina);
        $documento = $this->xml->describe();
        
        return $documento;
    }
    
    /**
     * Faz requisi��o ao servi�o e retorna a lista de execu��es or�ament�rias(funcionais,
     * Dota��es, PIs, valores Autorizados, Empenhados, Liquidados, Pagos e informa��es complementares)
     * 
     * @return array
     */
    public function request() {
        $result = parent::request();
        if($result && $result->execucoesOrcamentarias){
            $listExecucoesOrcamentarias = (array)$result->execucoesOrcamentarias;
            $listExecucaoOrcamentaria = $listExecucoesOrcamentarias['execucaoOrcamentaria'];
        }

        return $listExecucaoOrcamentaria;
    }

}