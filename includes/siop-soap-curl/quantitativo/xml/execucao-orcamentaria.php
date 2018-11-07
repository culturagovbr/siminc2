<?php

include_once APPRAIZ. 'includes/siop-soap-curl/xml.php';

/**
 * Classe para conectar com o Webservice do SIOP através do componente SoapCurl
 * 
 */
class SiopSoapCurl_Quantitativo_Xml_ExecucaoOrcamentaria extends SiopSoapCurl_Xml {

    protected $service = 'consultarExecucaoOrcamentaria';

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
    private $listField = array(
        'acao' => 'true',
        'acompanhamentoPO' => 'true',
        'anoExercicio' => 'true',
        'anoReferencia' => 'true',
        'autorizado' => 'true',
        'bloqueadoRemanejamento' => 'true',
        'bloqueadoSOF' => 'true',
        'categoriaEconomica' => 'true',
        'creditoContidoSOF' => 'true',
        'detalheAcompanhamentoPO' => 'true',
        'disponivel' => 'true',
        'dotAtual' => 'true',
        'dotInicialSiafi' => 'true',
        'dotacaoAntecipada' => 'true',
        'dotacaoInicial' => 'true',
        'elementoDespesa' => 'true',
        'empLiquidado' => 'true',
        'empenhadoALiquidar' => 'true',
        'esfera' => 'true',
        'estatalDependente' => 'true',
        'executadoPorInscricaoDeRAP' => 'true',
        'fonte' => 'true',
        'funcao' => 'true',
        'grupoNaturezaDespesa' => 'true',
        'identificadorAcompanhamentoPO' => 'true',
        'idoc' => 'true',
        'iduso' => 'true',
        'indisponivel' => 'true',
        'localizador' => 'true',
        'modalidadeAplicacao' => 'true',
        'natureza' => 'true',
        'numeroptres' => 'true',
        'origem' => 'true',
        'pago' => 'true',
        'planoInterno' => 'true',
        'planoOrcamentario' => 'true',
        'programa' => 'true',
        'projetoLei' => 'true',
        'rapAPagarNaoProcessado' => 'true',
        'rapAPagarProcessado' => 'true',
        'rapCanceladosNaoProcessados' => 'true',
        'rapCanceladosProcessados' => 'true',
        'rapExerciciosAnteriores' => 'true',
        'rapInscritoNaoProcessado' => 'true',
        'rapInscritoProcessado' => 'true',
        'rapNaoProcessadoALiquidar' => 'true',
        'rapNaoProcessadoBloqueado' => 'true',
        'rapNaoProcessadoLiquidadoAPagar' => 'true',
        'rapPagoNaoProcessado' => 'true',
        'rapPagoProcessado' => 'true',
        'resultadoPrimarioAtual' => 'true',
        'resultadoPrimarioLei' => 'true',
        'subElementoDespesa' => 'true',
        'subFuncao' => 'true',
        'tematicaPO' => 'true',
        'tipoApropriacaoPO' => 'true',
        'tipoCredito' => 'true',
        'unidadeGestoraResponsavel' => 'true',
        'unidadeOrcamentaria' => 'true',
    );
    
    /**
     * Pagina atual da consulta
     * 
     * @var int
     */    
    private $page = 1;
    
    /**
     * Quantidade de registros por consulta
     * 
     * @var int
     */
    private $limit = 2000;
    
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