<?php

include_once 'siop-xml.php';

/**
 * Classe para conectar com o Webservice do SIOP através do componente SoapCurl
 * 
 */
class siopXmlExecucaoOrcamentaria extends siopXml {

    /**
     * Lista de filtros
     * 
     * @var array
     */
    private $listFilter;
    
    /**
     * Lista de campos que serão retornados
     * 
     * @var array
     */
    private $listField;

    /**
     * Pagina atual da consulta
     * 
     * @var int
     */    
    private $page;
    
    /**
     * Quantidade de registros por consulta
     * 
     * @var int
     */
    private $limit;
    
    public function getListFilter() {
        return $this->listFilter;
    }

    public function getListField() {
        return $this->listField;
    }

    public function getPage() {
        return $this->page;
    }

    public function getLimit() {
        return $this->limit;
    }

    public function setListFilter($listFilter) {
        $this->listFilter = $listFilter;
        return $this;
    }

    public function setListField($listField) {
        $this->listField = $listField;
        return $this;
    }

    public function setPage($page) {
        $this->page = $page;
        return $this;
    }

    public function setLimit($limit) {
        $this->limit = $limit;
        return $this;
    }

    public function __construct($service = 'operacao', $user = NULL, $password = NULL, $profile = NULL, $listFilter = NULL, $listField = NULL, $page = 1, $limit = 2000) {
        $this->service = $service;
        $this->user = $user;
        $this->password = $password;
        $this->profile = $profile;
        $this->listFilter = $listFilter;
        $this->listField = $listField;
        $this->page = $page;
        $this->limit = $limit;
    }
    
    public function describeService() {
        $xml = "\n        <ns1:". $this->service. '>';
        $xml .= $this->describeCredential();
        $xml .= $this->describeListFilter();
        $xml .= $this->describeListField();
        $xml .= $this->describePagination();
        $xml .= "\n". '        </ns1:'. $this->service. '>';
        return $xml;
    }
    
    public function describeListFilter() {
        $xml = "\n              <filtro>";
        if($this->listFilter){
            foreach ($this->listFilter as $filter => $value) {
                $xml .= "\n                  <". $filter. '>'. $value. '</'. $filter. '>';
            }
        }
        $xml .= "\n              </filtro>";
        return $xml;
    }
    
    public function describeListField() {
        $xml = "\n              <selecaoRetorno>";
        if($this->listField){
            foreach ($this->listField as $field => $value) {
                $xml .= "\n                  <". $field. '>'. $value. '</'. $field. '>';
            }
        }
        $xml .= "\n              </selecaoRetorno>";
        return $xml;
    }
    
    public function describePagination() {
        $xml = "\n              <paginacao>";
        $xml .= "\n                  <pagina>". (int)$this->page. '</pagina>';
        $xml .= "\n                  <registrosPorPagina>". (int)$this->limit. '</registrosPorPagina>';
        $xml .= "\n              </paginacao>";

        return $xml;
    }
    
}