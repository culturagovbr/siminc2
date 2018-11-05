<?php

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
     * Op��o para retornar o resultado da conex�o como uma string
     * 
     * @var boolean
     */
    private $returnTranfer;
    
    /**
     * Op��o de tempo de espera da resposta da requisi��o
     * 
     * @var integer
     */
    private $timeout;
    
    public function __construct($url = NULL, $post = NULL, $listHeader = NULL, $user = NULL, $password = NULL, $auth = NULL, $returnTranfer = NULL, $timeout = NULL) {
        $this->url = $url;
        $this->post = $post;
        $this->listHeader = $listHeader;
        $this->user = $user;
        $this->password = $password;
        $this->auth = $auth;
        $this->returnTranfer = $returnTranfer;
        $this->timeout = $timeout;
    }
    
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

    public function getReturnTranfer() {
        return $this->returnTranfer;
    }

    public function getTimeout() {
        return $this->timeout;
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

    public function setReturnTranfer($returnTranfer) {
        $this->returnTranfer = $returnTranfer;
        return $this;
    }

    public function setTimeout($timeout) {
        $this->timeout = $timeout;
        return $this;
    }


}
