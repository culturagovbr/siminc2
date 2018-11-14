<?php

/**
 * Classe para conectar com o Webservice do SIOP através do componente SoapCurl
 * 
 */
class SiopSoapCurl_Quantitativo_Xml_ExecucaoOrcamentaria extends SiopSoapCurl_Xml {

    /**
     * Serviço acessado
     * 
     * @var string
     */
    protected $service = 'consultarExecucaoOrcamentaria';

    /**
     * Filtros de consulta, permite a passagem de uma lista de ações.
     * 
     * @var array
     */
    private $acoes;
    
    /**
     * Filtros de consulta por anoExercicio.
     * 
     * @var int
     */
    private $anoExercicio;

    /**
     * Filtros de consulta por anoReferencia.
     * 
     * @var int
     */
    private $anoReferencia;

    /**
     * Filtros de consulta, permite a passagem de uma lista de Planos Orçamentários.
     * 
     * @var array
     */
    private $planosOrcamentarios;
    
    /**
     * Contém um valor booleano opcional para cada elemento da classificação que deva ser retornado pela operação.
     * 
     * @var array
     */
    private $selecaoRetorno = array(
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
     * Caso esteja querendo usar paginação na consulta, o número da página deverá ser informado.
     * 
     * @var int
     */
    private $pagina;
    
    /**
     * Caso esteja queira usar paginação na consulta, o número de registros por página deverá ser informado.
     * 
     * @var int
     */
    private $registrosPorPagina;
    
    public function getService() {
        return $this->service;
    }

    public function getAcoes() {
        return $this->acoes;
    }

    public function getAnoExercicio() {
        return $this->anoExercicio;
    }

    public function getAnoReferencia() {
        return $this->anoReferencia;
    }

    public function getPlanosOrcamentarios() {
        return $this->planosOrcamentarios;
    }

    public function getSelecaoRetorno() {
        return $this->selecaoRetorno;
    }

    public function getPagina() {
        return $this->pagina;
    }

    public function getRegistrosPorPagina() {
        return $this->registrosPorPagina;
    }

    public function setService($service) {
        $this->service = $service;
        return $this;
    }

    public function setAcoes($acoes) {
        $this->acoes = $acoes;
        return $this;
    }

    public function setAnoExercicio($anoExercicio) {
        $this->anoExercicio = $anoExercicio;
        return $this;
    }

    public function setAnoReferencia($anoReferencia) {
        $this->anoReferencia = $anoReferencia;
        return $this;
    }

    public function setPlanosOrcamentarios($planosOrcamentarios) {
        $this->planosOrcamentarios = $planosOrcamentarios;
        return $this;
    }

    public function setSelecaoRetorno($selecaoRetorno) {
        $this->selecaoRetorno = $selecaoRetorno;
        return $this;
    }

    public function setPagina($pagina) {
        $this->pagina = $pagina;
        return $this;
    }

    public function setRegistrosPorPagina($registrosPorPagina) {
        $this->registrosPorPagina = $registrosPorPagina;
        return $this;
    }
    
    /**
     * Retorna trexo código XML referente a tag do serviço solicitado
     * 
     * @return string xml
     */
    public function describeService() {
        $xml = "\n        <ns1:". $this->service. '>';
        $xml .= $this->describeCredential();
        $xml .= $this->describeFiltro();
        $xml .= $this->describeSelecaoRetorno();
        $xml .= $this->describePaginacao();
        $xml .= "\n". '        </ns1:'. $this->service. '>';
        return $xml;
    }
    
    public function describeFiltro() {
        $xml = "\n              <filtro>";
        $xml .= $this->describeAcoes();
        $xml .= $this->anoExercicio? "\n                  <anoExercicio>". $this->anoExercicio. '</anoExercicio>': NULL;
        $xml .= $this->anoReferencia? "\n                  <anoReferencia>". $this->anoReferencia. '</anoReferencia>': NULL;
        $xml .= $this->describePlanosOrcamentarios();
        $xml .= "\n              </filtro>";
        return $xml;
    }
    
    public function describeAcoes() {
        $xml = NULL;
        if($this->acoes){
            $xml .= "\n                  <acoes>";
            foreach($this->acoes as $acao) {
                $xml .= "\n                     <acao>". $acao. '</acao>';
            }
            $xml .= "\n                  </acoes>";
        }
        return $xml;
    }
    
    public function describePlanosOrcamentarios() {
        $xml = NULL;
        if($this->planosOrcamentarios){
            $xml .= "\n                  <planosOrcamentarios>";
            foreach($this->planosOrcamentarios as $planoOrcamentario) {
                $xml .= "\n                     <planoOrcamentario>". $planoOrcamentario. '</planoOrcamentario>';
            }
            $xml .= "\n                  </planosOrcamentarios>";
        }
        return $xml;
    }
    
    public function describeSelecaoRetorno() {
        $xml = "\n              <selecaoRetorno>";
        if($this->selecaoRetorno){
            foreach ($this->selecaoRetorno as $selecao => $retorno) {
                $xml .= "\n                  <". $selecao. '>'. $retorno. '</'. $selecao. '>';
            }
        }
        $xml .= "\n              </selecaoRetorno>";
        return $xml;
    }
    
    public function describePaginacao() {
        if($this->pagina || $this->registrosPorPagina){
            $xml = "\n              <paginacao>";
            $xml .= "\n                  <pagina>". (int)$this->pagina. '</pagina>';
            $xml .= "\n                  <registrosPorPagina>". (int)$this->registrosPorPagina. '</registrosPorPagina>';
            $xml .= "\n              </paginacao>";
        }
        
        return $xml;
    }
    
}