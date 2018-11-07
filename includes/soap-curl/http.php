<?php

include_once 'client.php';

class SoapCurl_Http {
 
    /**
     * Url do serviço onde contem o WSDL
     * 
     * @var string
     */
    private $wsdl;
    
    /**
     * Opção pra definir se o tipo da requisição é POST
     * 
     * @var boolean
     */
    private $post;
    
    /**
     * Itens do Cabeçalho da requisição
     *
     * @var array
     */
    private $listHeader;
    
    /**
     * Usuário
     * 
     * @var string
     */
    private $user;
    
    /**
     * Senha do usuário
     * 
     * @var string
     */
    private $password;
    
    /**
     * Opção método de autenticação
     * 
     * @var integer
     */
    private $auth;
    
    /**
     * Opção de tempo de espera da resposta da requisição
     * 
     * @var integer
     */
    private $timeout;
    
    /**
     * Opção para retornar o resultado da conexão como uma string
     * 
     * @var boolean
     */
    private $return;
    
    public function getWsdl() {
        return $this->wsdl;
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

    public function setWsdl($wsdl) {
        $this->wsdl = $wsdl;
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
     * Manipula as informações da requisição
     * 
     * @param string $wsdl
     * @param boolean $post
     * @param array $listHeader
     * @param string $user
     * @param string $password
     * @param integer $auth
     * @param integer $timeout
     * @param boolean $return
     */
    public function __construct($wsdl = NULL, $post = NULL, $listHeader = NULL, $user = NULL, $password = NULL, $auth = NULL, $timeout = NULL, $return = NULL) {
        $this->wsdl = $wsdl;
        $this->post = $post;
        $this->listHeader = $listHeader;
        $this->user = $user;
        $this->password = $password;
        $this->auth = $auth;
        $this->timeout = $timeout;
        $this->return = $return;
    }
    
    public function configureAll(){
        $this->configureWsdl()
            ->configurePost()
            ->configureListHeader()
            ->configureUser()
            ->configureAuth()
            ->configureTimeout()
            ->configureReturn()
        ;
        return $this;
    }
    
    public function configureWsdl(){
        if($this->wsdl){
            curl_setopt(SoapCurl_Client::$resource, CURLOPT_URL, $this->wsdl);
        }
        return $this;
    }
    
    public function configurePost(){
        if($this->post){
            curl_setopt(SoapCurl_Client::$resource, CURLOPT_POST, $this->post);
        }
        return $this;
    }
    
    public function configureListHeader(){
        if($this->listHeader){
            curl_setopt(SoapCurl_Client::$resource, CURLOPT_HTTPHEADER, $this->listHeader);
        }
        return $this;
    }
    
    public function configureUser(){
        if($this->user){
            curl_setopt(SoapCurl_Client::$resource, CURLOPT_USERPWD, $this->user. ':'. $this->password);
        }
        return $this;
    }
    
    public function configureAuth(){
        if($this->auth){
            curl_setopt(SoapCurl_Client::$resource, CURLOPT_HTTPAUTH, $this->auth);
        }
        return $this;
    }
    
    public function configureTimeout(){
        if($this->timeout){
            curl_setopt(SoapCurl_Client::$resource, CURLOPT_TIMEOUT, $this->timeout);
        }
        return $this;
    }
    
    public function configureReturn(){
        if($this->return){
            curl_setopt(SoapCurl_Client::$resource, CURLOPT_RETURNTRANSFER, $this->return);
        }
        return $this;
    }


}
