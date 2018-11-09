<?php

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
    
    /**
     * Resposta da requisição
     * 
     * @var SimpleXMLElement 
     */
    protected $response;

    /**
     * Tag onde existem os resultados da requisição
     * 
     * @var string
     */
    protected $tagReturn = 'return';

    public function getUrl() {
        return $this->url;
    }

    public function getClient() {
        return $this->client;
    }

    public function getResponse() {
        return $this->response;
    }

    public function getTagReturn() {
        return $this->tagReturn;
    }

    public function setUrl($url) {
        $this->url = $url;
        return $this;
    }

    public function setClient(SoapCurl_Client $client) {
        $this->client = $client;
        return $this;
    }

    public function setResponse(SimpleXMLElement $response) {
        $this->response = $response;
        return $this;
    }

    public function setTagReturn($tagReturn) {
        $this->tagReturn = $tagReturn;
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
        $this->convertResponseToObject();
        return $this->response;
    }
    
    /**
     * Converte o resultado da requisição(xml) em objeto
     * 
     * @return SimpleXMLElement
     */
    public function convertResponseToObject() {
        $this->response = new SimpleXMLElement('<'. $this->tagReturn. '></'. $this->tagReturn. '>');
        
        # Verifica se existe resposta na requisição
        if($this->client->getResponse()){
            # Retira tags de cabeçalho do documento XML
            $posTagStartReturn = strpos($this->client->getResponse(), '<'. $this->tagReturn. '>');
            $responseNoHeader = substr($this->client->getResponse(), $posTagStartReturn, strlen($this->client->getResponse()));
            $posTagEndReturn = strpos($responseNoHeader, '</'. $this->tagReturn. '>');
            $responseReturn = substr($responseNoHeader, 0, ((int)$posTagEndReturn + (int)strlen('</'. $this->tagReturn. '>')));

            # Verifica se dentro da tag de retorno do documento XML existem resultados
            if(trim($responseReturn)){
                $this->response = simplexml_load_string($responseReturn);
            }
        }

        return $this->response;
    }

    /**
     * Exibe o documento xml enviado no momento da requisição completo em um arquivo separado pra Baixar/fazer download.
     * 
     * @return VOID
     */    
    public function showXmlRequest(){
        echo $this->loadXml();
        header('Content-Type: application/xml; charset=utf-8');
        header('Expires: 0');
        header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
        header('Content-Disposition: attachment; filename="request.xml"');
        die;
    }

    /**
     * Exibe o documento xml de resposta completo em um arquivo separado pra Baixar/fazer download.
     * 
     * @return VOID
     */
    public function showXmlResponse() {
        echo $this->client->getResponse();
        header('Content-Type: application/xml; charset=utf-8');
        header('Expires: 0');
        header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
        header('Content-Disposition: attachment; filename="response.xml"');
        die;
    }
    
}
