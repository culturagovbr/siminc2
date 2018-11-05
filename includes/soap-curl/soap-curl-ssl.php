<?php

class soapCurlSsl {

    /**
     * Senha usada para gerar o certificado
     * 
     * @var string
     */
    private $password;
    
    /**
     * Caminho do certificado
     * 
     * @var string
     */
    private $certificate;
    
    /**
     * Opção de versão do SSL
     * 
     * @var integer
     */
    private $version;
    
    /**
     * Opção verify peer
     * 
     * @var boolean
     */
    private $verifyPeer;
    
    /**
     * Opção verify host
     * 
     * @var boolean
     */
    private $verifyHost;
    
    public function getPassword() {
        return $this->password;
    }

    public function getCertificate() {
        return $this->certificate;
    }

    public function getVersion() {
        return $this->version;
    }

    public function getVerifyPeer() {
        return $this->verifyPeer;
    }

    public function getVerifyHost() {
        return $this->verifyHost;
    }

    public function setPassword($password) {
        $this->password = $password;
        return $this;
    }

    public function setCertificate($certificate) {
        $this->certificate = $certificate;
        return $this;
    }

    public function setVersion($version) {
        $this->version = $version;
        return $this;
    }

    public function setVerifyPeer($verifyPeer) {
        $this->verifyPeer = $verifyPeer;
        return $this;
    }

    public function setVerifyHost($verifyHost) {
        $this->verifyHost = $verifyHost;
        return $this;
    }
    
    public function __construct($password = NULL, $certificate = NULL, $version = NULL, $verifyPeer = NULL, $verifyHost = NULL) {
        $this->password = $password;
        $this->certificate = $certificate;
        $this->version = $version;
        $this->verifyPeer = $verifyPeer;
        $this->verifyHost = $verifyHost;
    }
    
    public function configurePassword($resource){
        if($this->password){
            curl_setopt($resource, CURLOPT_SSLCERTPASSWD, $this->password);
        }
        return $this;
    }
    
    public function configureCertificate($resource){
        if($this->certificate){
            curl_setopt($resource, CURLOPT_SSLCERT, $this->certificate);
        }
        return $this;
    }
    
    public function configureVersion($resource){
        if($this->version){
            curl_setopt($resource, CURLOPT_SSLVERSION, $this->version);
        }
        return $this;
    }
    
    public function configureVerifyPeer($resource){
        if($this->verifyPeer){
            curl_setopt($resource, CURLOPT_SSL_VERIFYPEER, $this->verifyPeer);
        }
        return $this;
    }
    
    public function configureVerifyHost($resource){
        if($this->verifyHost){
            curl_setopt($resource, CURLOPT_SSL_VERIFYHOST, $this->verifyHost);
        }
        return $this;
    }

}

