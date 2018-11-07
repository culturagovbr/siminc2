<?php

include_once 'i-siop-xml.php';
//include_once 'siop-soap-curl.php';

/**
 * Classe para conectar com o Webservice do SIOP atrav�s do componente SoapCurl
 * 
 */
abstract class siopXml implements iSiopXml {

    /**
     * Servi�o acessado
     * 
     * @var string
     */
    private $service;
    
    /**
     * Usu�rio
     * 
     * @var string
     */
    private $user;
    
    /**
     * Senha do usu�rio
     * 
     * @var string
     */
    private $password;
    
    /**
     * Perfil de acesso ao servi�o fornecido junto ao usu�rio e a senha pelo provedor
     * 
     * @var int
     */
    private $profile;
    
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

    public function __construct($service = 'operacao', $user = NULL, $password = NULL, $profile = NULL) {
        $this->service = $service;
        $this->user = $user;
        $this->password = $password;
        $this->profile = $profile;
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

}
