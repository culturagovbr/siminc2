<?php

class soapCurlHttp {
 
    /**
     * Endereço da documentação do serviço
     * 
     * @var string
     */
    private $url;
    
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
    
    public function configureAll($resource){
        $this->configureUrl($resource)
            ->configurePost($resource)
            ->configureListHeader($resource)
            ->configureUser($resource)
            ->configureAuth($resource)
            ->configureTimeout($resource)
            ->configureReturn($resource)
        ;
        return $this;
    }    
    
    public function configureUrl($resource){
        if($this->url){
            curl_setopt($resource, CURLOPT_URL, $this->url);
        }
        return $this;
    }
    
    public function configurePost($resource){
        if($this->post){
            curl_setopt($resource, CURLOPT_POST, $this->post);
        }
        return $this;
    }
    
    public function configureListHeader($resource){
        if($this->listHeader){
            curl_setopt($resource, CURLOPT_HTTPHEADER, $this->listHeader);
        }
        return $this;
    }
    
    public function configureUser($resource){
        if($this->user){
            curl_setopt($resource, CURLOPT_USERPWD, $this->user. ':'. $this->password);
        }
        return $this;
    }
    
    public function configureAuth($resource){
        if($this->auth){
            curl_setopt($resource, CURLOPT_HTTPAUTH, $this->auth);
        }
        return $this;
    }
    
    public function configureTimeout($resource){
        if($this->timeout){
            curl_setopt($resource, CURLOPT_TIMEOUT, $this->timeout);
        }
        return $this;
    }
    
    public function configureReturn($resource){
        if($this->return){
            curl_setopt($resource, CURLOPT_RETURNTRANSFER, $this->return);
        }
        return $this;
    }


}
