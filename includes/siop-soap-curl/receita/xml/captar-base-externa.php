<?php

/**
 * Classe para conectar com o Webservice do SIOP através do componente SoapCurl
 * 
 */
class SiopSoapCurl_Receita_Xml_CaptarBaseExterna extends SiopSoapCurl_Xml {

    /**
     * Serviço acessado
     * 
     * @var string
     */
    protected $service = 'captarBaseExterna';

    protected $codigoCaptacaoBaseExterna;
    protected $codigoNaturezaReceita;
    protected $codigoUnidadeRecolhedora;
    protected $subNatureza;
    protected $justificativa;
    protected $metodologia;
    protected $memoriaDeCalculo;
    protected $exercicio;
    protected $valor;

    public function getService() {
        return $this->service;
    }

    public function getCodigoCaptacaoBaseExterna() {
        return $this->codigoCaptacaoBaseExterna;
    }

    public function getCodigoNaturezaReceita() {
        return $this->codigoNaturezaReceita;
    }

    public function getCodigoUnidadeRecolhedora() {
        return $this->codigoUnidadeRecolhedora;
    }

    public function getSubNatureza() {
        return $this->subNatureza;
    }

    public function getJustificativa() {
        return $this->justificativa;
    }

    public function getMetodologia() {
        return $this->metodologia;
    }

    public function getMemoriaDeCalculo() {
        return $this->memoriaDeCalculo;
    }

    public function getExercicio() {
        return $this->exercicio;
    }

    public function getValor() {
        return $this->valor;
    }

    public function setService($service) {
        $this->service = $service;
        return $this;
    }

    public function setCodigoCaptacaoBaseExterna($codigoCaptacaoBaseExterna) {
        $this->codigoCaptacaoBaseExterna = $codigoCaptacaoBaseExterna;
        return $this;
    }

    public function setCodigoNaturezaReceita($codigoNaturezaReceita) {
        $this->codigoNaturezaReceita = $codigoNaturezaReceita;
        return $this;
    }

    public function setCodigoUnidadeRecolhedora($codigoUnidadeRecolhedora) {
        $this->codigoUnidadeRecolhedora = $codigoUnidadeRecolhedora;
        return $this;
    }

    public function setSubNatureza($subNatureza) {
        $this->subNatureza = $subNatureza;
        return $this;
    }

    public function setJustificativa($justificativa) {
        $this->justificativa = $justificativa;
        return $this;
    }

    public function setMetodologia($metodologia) {
        $this->metodologia = $metodologia;
        return $this;
    }

    public function setMemoriaDeCalculo($memoriaDeCalculo) {
        $this->memoriaDeCalculo = $memoriaDeCalculo;
        return $this;
    }

    public function setExercicio($exercicio) {
        $this->exercicio = $exercicio;
        return $this;
    }

    public function setValor($valor) {
        $this->valor = $valor;
        return $this;
    }

    public function describe() {
        $xml = '<?xml version="1.0" encoding="UTF-8"?>'.
            "\n". '<soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:ser="http://servicoweb.siop.sof.planejamento.gov.br/">'.
            "\n". '<soapenv:Header/>'.
            "\n   <soapenv:Body>";
        $xml .= $this->describeService();
        $xml .= "\n   </soapenv:Body>".
            "\n</soapenv:Envelope>";
        return $xml;
    }
    
    /**
     * Retorna trexo código XML referente a tag do serviço solicitado
     * 
     * @return string xml
     */
    public function describeService() {
        $xml = "\n        <ser:". $this->service. '>';
        $xml .= $this->describeCredential();
        $xml .= "\n        <captacaoBaseExterna>";
        $xml .= $this->codigoCaptacaoBaseExterna? "\n              <codigoCaptacaoBaseExterna>". (int) $this->codigoCaptacaoBaseExterna. "</codigoCaptacaoBaseExterna>": NULL;
        $xml .= "\n        <detalhesBaseExterna>";
        $xml .= "\n        <detalheBaseExterna>";
        $xml .= $this->codigoNaturezaReceita? "\n              <codigoNaturezaReceita>". (int) $this->codigoNaturezaReceita. "</codigoNaturezaReceita>": NULL;
        $xml .= $this->codigoUnidadeRecolhedora? "\n              <codigoUnidadeRecolhedora>". (int) $this->codigoUnidadeRecolhedora. "</codigoUnidadeRecolhedora>": NULL;
        $xml .= $this->subNatureza? "\n              <subNatureza>". (int) $this->subNatureza. "</subNatureza>": NULL;
        $xml .= $this->justificativa? "\n              <justificativa>". utf8_encode($this->justificativa) . "</justificativa>": NULL;
        $xml .= $this->metodologia? "\n              <metodologia>". utf8_encode($this->metodologia). "</metodologia>": NULL;
        $xml .= $this->memoriaDeCalculo? "\n              <memoriaDeCalculo>". utf8_encode($this->memoriaDeCalculo). "</memoriaDeCalculo>": NULL;
        $xml .= "\n        <valoresBaseExterna>";
        $xml .= "\n        <valorBaseExterna>";
        $xml .= $this->exercicio? "\n              <exercicio>". (int) $this->exercicio. "</exercicio>": NULL;
        $xml .= $this->valor? "\n              <valor>". (int) $this->valor. "</valor>": NULL;
        $xml .= "\n        </valorBaseExterna>";
        $xml .= "\n        </valoresBaseExterna>";
        $xml .= "\n        </detalheBaseExterna>";
        $xml .= "\n        </detalhesBaseExterna>";
        $xml .= "\n        </captacaoBaseExterna>";
        $xml .= "\n". '        </ser:'. $this->service. '>';
        return $xml;
    }
    
}
