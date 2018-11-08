<?php

include_once 'interface-xml.php';

/**
 * Classe para conectar com o Webservice do SIOP através do componente SoapCurl
 * 
 */
abstract class SiopSoapCurl_Xml implements SiopSoapCurl_InterfaceXml {

    /**
     * Serviço acessado
     * 
     * @var string
     */
    protected $service = 'operacao';
    
    /**
     * Usuário
     * 
     * @var string
     */
    protected $user = WEB_SERVICE_SIOP_USUARIO;
    
    /**
     * Senha do usuário
     * 
     * @var string
     */
    protected $password = WEB_SERVICE_SIOP_SENHA;
    
    /**
     * Perfil de acesso ao serviço fornecido junto ao usuário e a senha pelo provedor
     * 
     * @var int
     */
    protected $profile = WEB_SERVICE_SIOP_PERFIL;
    
    public function getService() {
        return $this->service;
    }

    public function getUser() {
        return $this->user;
    }

    public function getPassword() {
        return $this->password;
    }

    public function getProfile() {
        return $this->profile;
    }

    public function setService($service) {
        $this->service = $service;
        return $this;
    }

    public function setUser($user) {
        $this->user = $user;
        return $this;
    }

    public function setPassword($password) {
        $this->password = $password;
        return $this;
    }

    public function setProfile($profile) {
        $this->profile = $profile;
        return $this;
    }
    
    public function describeCredential() {
        $xml = "\n              <credencial>
                  <perfil>". $this->profile. '</perfil>
                  <senha>'. $this->password. '</senha>
                  <usuario>'. $this->user. '</usuario>
              </credencial>';

        return $xml;
    }
    
    public function describe() {
        $xml = '<?xml version="1.0" encoding="UTF-8"?>'.
            "\n". '<env:Envelope xmlns:env="http://www.w3.org/2003/05/soap-envelope" xmlns:ns1="http://servicoweb.siop.sof.planejamento.gov.br/">'.
            "\n   <env:Body>";
        $xml .= $this->describeService();
        $xml .= "\n   </env:Body>".
            "\n</env:Envelope>";
        return $xml;
    }
    
    public function __toString() {
        $xml = $this->describe();
        return $xml;
    }
    
    /**
     * Exibe o documento xml completo em um arquivo separado pra Baixar/fazer download.
     * 
     * @return VOID
     */
    public function show() {
        echo $this->describe();
        header('Content-Type: application/xml; charset=utf-8');
        header('Expires: 0');
        header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
        header('Content-Disposition: attachment; filename="'. $this->service. '-request.xml"');
        die;
    }
    
    

}
