<?php

include_once 'soap-curl.php';

class soapCurlHttp {
 
    /**
     * Endere�o da documenta��o do servi�o
     * 
     * @var string
     */
    private $url;
    
    /**
     * Op��o pra definir se o tipo da requisi��o � POST
     * 
     * @var boolean
     */
    private $post;
    
    /**
     * Itens do Cabe�alho da requisi��o
     *
     * @var array
     */
    private $listHeader;
    
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
     * Op��o m�todo de autentica��o
     * 
     * @var integer
     */
    private $auth;
    
    /**
     * Op��o de tempo de espera da resposta da requisi��o
     * 
     * @var integer
     */
    private $timeout;
    
    /**
     * Op��o para retornar o resultado da conex�o como uma string
     * 
     * @var boolean
     */
    private $return;
    
    public function getUrl() {
        return $this->url;
    }

    public function getPost() {
        return $this->post;
    }

    public function getListHeader() {
        return $this->listHeader;
    }

    public function getUser() {
        return $this->user;
    }

    public function getPassword() {
        return $this->password;
    }

    public function getAuth() {
        return $this->auth;
    }

    public function getTimeout() {
        return $this->timeout;
    }

    public function getReturn() {
        return $this->return;
    }

    public function setUrl($url) {
        $this->url = $url;
        return $this;
    }

    public function setPost($post) {
        $this->post = $post;
        return $this;
    }

    public function setListHeader($listHeader) {
        $this->listHeader = $listHeader;
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

    public function setAuth($auth) {
        $this->auth = $auth;
        return $this;
    }

    public function setTimeout($timeout) {
        $this->timeout = $timeout;
        return $this;
    }

    public function setReturn($return) {
        $this->return = $return;
        return $this;
    }    
    
    /**
     * Manipula as informa��es da requisi��o
     * 
     * @param string $url
     * @param boolean $post
     * @param array $listHeader
     * @param string $user
     * @param string $password
     * @param integer $auth
     * @param integer $timeout
     * @param boolean $return
     */
    public function __construct($url = NULL, $post = NULL, $listHeader = NULL, $user = NULL, $password = NULL, $auth = NULL, $timeout = NULL, $return = NULL) {
        $this->url = $url;
        $this->post = $post;
        $this->listHeader = $listHeader;
        $this->user = $user;
        $this->password = $password;
        $this->auth = $auth;
        $this->timeout = $timeout;
        $this->return = $return;
    }
    
    public function configureAll(){
        $this->configureUrl()
            ->configurePost()
            ->configureListHeader()
            ->configureUser()
            ->configureAuth()
            ->configureTimeout()
            ->configureReturn()
        ;
        return $this;
    }    
    
    public function configureUrl(){
        if($this->url){
            curl_setopt(soapCurl::$resource, CURLOPT_URL, $this->url);
        }
        return $this;
    }
    
    public function configurePost(){
        if($this->post){
            curl_setopt(soapCurl::$resource, CURLOPT_POST, $this->post);
        }
        return $this;
    }
    
    public function configureListHeader(){
        if($this->listHeader){
            curl_setopt(soapCurl::$resource, CURLOPT_HTTPHEADER, $this->listHeader);
        }
        return $this;
    }
    
    public function configureUser(){
        if($this->user){
            curl_setopt(soapCurl::$resource, CURLOPT_USERPWD, $this->user. ':'. $this->password);
        }
        return $this;
    }
    
    public function configureAuth(){
        if($this->auth){
            curl_setopt(soapCurl::$resource, CURLOPT_HTTPAUTH, $this->auth);
        }
        return $this;
    }
    
    public function configureTimeout(){
        if($this->timeout){
            curl_setopt(soapCurl::$resource, CURLOPT_TIMEOUT, $this->timeout);
        }
        return $this;
    }
    
    public function configureReturn(){
        if($this->return){
            curl_setopt(soapCurl::$resource, CURLOPT_RETURNTRANSFER, $this->return);
        }
        return $this;
    }


}
