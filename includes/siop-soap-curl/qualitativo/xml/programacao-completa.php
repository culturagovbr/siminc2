<?php

/**
 * Classe para conectar com o Webservice do SIOP atrav�s do componente SoapCurl
 * 
 */
class SiopSoapCurl_Quantitativo_Xml_ProgramacaoCompleta extends SiopSoapCurl_Xml {

    /**
     * Servi�o acessado
     * 
     * @var string
     */
    protected $service = 'obterProgramacaoCompleta';

    /**
     * Ano do exercicio � qual ser� consultado
     * 
     * @var int
     */
    protected $exercicio = 2018;

    /**
     * Opcional, caso n�o informado, o servi�o assume a a��o em seu momento atual.
     * 
     * @var int
     */
    protected $codigoMomento = 3000;
    
    /**
     * Opcional. Data e hora a partir das quais se desejam as respostas.
     * 
     * @var string 
     */
    protected $dataHoraReferencia;

    /**
     * Listas que ser�o retornadas
     * 
     * @var array
     */
    protected $listasRetornadas = array(
        'retornarOrgaos',
        'retornarProgramas',
        'retornarIndicadores',
        'retornarObjetivos',
        'retornarIniciativas',
        'retornarAcoes',
        'retornarLocalizadores',
        'retornarMetas',
        'retornarRegionalizacoes',
        'retornarPlanosOrcamentarios',
        'retornarAgendaSam',
        'retornarMedidasInstitucionaisNormativas'
    );

    public function getService() {
        return $this->service;
    }

    public function getExercicio() {
        return $this->exercicio;
    }

    public function getCodigoMomento() {
        return $this->codigoMomento;
    }

    public function getDataHoraReferencia() {
        return $this->dataHoraReferencia;
    }

    public function getListasRetornadas() {
        return $this->listasRetornadas;
    }

    public function setService($service) {
        $this->service = $service;
        return $this;
    }

    public function setExercicio($exercicio) {
        $this->exercicio = $exercicio;
        return $this;
    }

    public function setCodigoMomento($codigoMomento) {
        $this->codigoMomento = $codigoMomento;
        return $this;
    }

    public function setDataHoraReferencia($dataHoraReferencia) {
        $this->dataHoraReferencia = $dataHoraReferencia;
        return $this;
    }

    public function setListasRetornadas($listasRetornadas) {
        $this->listasRetornadas = $listasRetornadas;
        return $this;
    }
    
    /**
     * Retorna trexo c�digo XML referente a tag do servi�o solicitado
     * 
     * @return string xml
     */
    public function describeService() {
        $xml = "\n        <ns1:". $this->service. '>';
        $xml .= $this->describeCredential();
        $xml .= $this->exercicio? "\n              <exercicio>". (int) $this->exercicio. "</exercicio>": NULL;
        $xml .= $this->codigoMomento? "\n              <codigoMomento>". (int) $this->codigoMomento. "</codigoMomento>": NULL;
        $xml .= $this->describeListasRetornadas();
        $xml .= $this->dataHoraReferencia? "\n              <dataHoraReferencia>". (int) $this->dataHoraReferencia. "</dataHoraReferencia>": NULL;
        $xml .= "\n". '        </ns1:'. $this->service. '>';
        return $xml;
    }
    
    public function describeListasRetornadas() {
        $xml = '';
        if($this->listasRetornadas){
            foreach ($this->listasRetornadas as $lista) {
                $xml .= "\n              <". $lista. '>true</'. $lista. '>';
            }
        }
        return $xml;
    }
    
}
