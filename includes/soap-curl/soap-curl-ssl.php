<?php

include_once 'soap-curl.php';

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
     * Op��o de vers�o do SSL
     * 
     * @var integer
     */
    private $version;
    
    /**
     * Op��o verify peer
     * 
     * @var boolean
     */
    private $verifyPeer;
    
    /**
     * Op��o verify host
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
    
    /**
     * Manipula o protocolo de seguran�a
     * 
     * @param string $password
     * @param string $certificate
     * @param integer $version
     * @param boolean $verifyPeer
     * @param boolean $verifyHost
     */
    public function __construct($password = NULL, $certificate = NULL, $version = NULL, $verifyPeer = NULL, $verifyHost = NULL) {
        $this->password = $password;
        $this->certificate = $certificate;
        $this->version = $version;
        $this->verifyPeer = $verifyPeer;
        $this->verifyHost = $verifyHost;
    }
    
    public function configureAll(){
        $this->configurePassword()
            ->configureCertificate()
            ->configureVersion()
            ->configureVerifyPeer()
            ->configureVerifyHost()
        ;
        return $this;
    }
    
    public function configurePassword(){
        if($this->password){
            curl_setopt(soapCurl::$resource, CURLOPT_SSLCERTPASSWD, $this->password);
        }
        return $this;
    }
    
    public function configureCertificate(){
        if($this->certificate){
            curl_setopt(soapCurl::$resource, CURLOPT_SSLCERT, $this->certificate);
        }
        return $this;
    }
    
    public function configureVersion(){
        if($this->version){
            curl_setopt(soapCurl::$resource, CURLOPT_SSLVERSION, $this->version);
        }
        return $this;
    }
    
    public function configureVerifyPeer(){
        if($this->verifyPeer){
            curl_setopt(soapCurl::$resource, CURLOPT_SSL_VERIFYPEER, $this->verifyPeer);
        }
        return $this;
    }
    
    public function configureVerifyHost(){
        if($this->verifyHost){
            curl_setopt(soapCurl::$resource, CURLOPT_SSL_VERIFYHOST, $this->verifyHost);
        }
        return $this;
    }

}

