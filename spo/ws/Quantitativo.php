<?php
/**
 * Implementa a classe de acesso ao webservice WSQuantitativo da SOF.
 * $Id: Quantitativo.php 87061 2014-09-19 13:25:44Z maykelbraz $
 */

/**
 * Classe base com as configura��es comuns para os webservices da SOF.
 * @see Spo_Ws_Sof
 */
require_once(dirname(__FILE__) . '/Sof.php');

/**
 * Classes de mapeamento de dados para o webservice WSQuantitativo da SOF.
 * @see WSQuantitavoMap.php
 */
require_once(dirname(__FILE__) . '/QuantitativoMap.php');

/**
 * Classe de acesso ao Webserviceo WSQuantitativo da SOF.
 */
class Spo_Ws_Quantitativo extends Spo_Ws_Sof
{
    /**
     * {@inheritdoc}
     */
    protected function loadURL()
    {
        switch ($this->enviroment) {
            case self::PRODUCTION:
                $this->urlWSDL = <<<DML
https://www.siop.gov.br/services/WSQuantitativo?wsdl
DML;
                break;
            case self::STAGING:
                $this->urlWSDL = <<<DML
https://homologacao.siop.planejamento.gov.br/services/WSQuantitativo?wsdl
DML;
                break;
            case self::DEVELOPMENT:
                $this->urlWSDL = <<<DML
https://testews.siop.gov.br/services/WSQuantitativo?wsdl
DML;
        }
        return $this;
    }

    /**
     * {@inheritdoc}
     * @todo Atualizar o QuantitativoMap para usar com o loadClassMap do Sof.
     */
    protected function loadClassMap()
    {
        $classMap = new Simec_SoapClient_ClassMap();
        $classMapClass = new ReflectionClass(get_class($this) . "Map");

        foreach ($classMapClass->getStaticPropertyValue('classmap') as $tipo => $classe) {
            $classMap->add($tipo, $classe);
        }

        return $classMap;
    }

    public function cadastrarProposta(PropostaDTO $proposta)
    {
        $cadastrarProposta = new cadastrarProposta();
        $cadastrarProposta->credencial = $this->credenciais;
        $cadastrarProposta->proposta = $proposta;

        return $this->soapClient->call('cadastrarProposta', array($cadastrarProposta));
    }

    /**
     * Faz a chamada do m�todo WSQuantitativo::consultarExecucaoOrcamentaria, que apresenta<br />
     * informa��es sobre a execu��o or�ament�ria em um determinado per�odo.<br />
     * <pre>
     * $filtro = array('anoReferencia' => 2014, 'acoes' => array('20RJ', '20RK'));
     * $retorno = array('dotAtual', 'acao', 'planoOrcamentario');
     * $ws = new Spo_Ws_Quantitativo('', Spo_Ws_Quantitativo::STAGING);
     * $ws->consultarExecucaoOrcamentaria($filtro, $retorno);
     * </pre>
     * A p�gina inicial � a n� 1, o n�mero padr�o de registros por p�gina � de 30k registros.
     * @param array $filtro Lista com pares nome do filtro e valor/lista de valores para selecionar dados da consulta.
     * @param array $selRetorno Lista com o nome dos campos que devem ser retornados na consulta.
     * @param int $pagina N�mero da p�gina da consulta, no caso de uma consulta pagina.
     * @param bool $retornaArray Retorna o resultado da consulta como array - o retorno cont�m apenas os campos indicados em $selecaoRetorno.
     * @return SoapFault|consultarExecucaoOrcamentariaResponse|array
     * @throws Exception Lan�a uma exce��o se um filtro ou retorno n�o estiver definido.
     * @see filtroExecucaoOrcamentariaDTO
     * @see selecaoRetornoExecucaoOrcamentariaDTO
     */
    function consultarExecucaoOrcamentaria(array $filtro, array $selRetorno, $pagina = null, $retornaArray = false)
    {
        // -- Filtros da consulta
        $filtroExecucaoOrcamentaria = new filtroExecucaoOrcamentariaDTO();
        foreach ($filtro as $campo => $valor) {
            if (!property_exists($filtroExecucaoOrcamentaria, $campo)) {
                throw new Exception("O filtro '{$campo}' n�o � v�lido para o m�todo WSQuantitativo::consultarExecucaoOrcamentaria.");
            }
            // -- Verifica se existe uma classe para detalhar o atributo, exemplo: acoes
            if (!class_exists($campo)) {
                $filtroExecucaoOrcamentaria->$campo = $valor;
                continue;
            }
            $filtroExecucaoOrcamentaria->$campo = new $campo();
            $prop = key(get_class_vars($campo));

            // -- Se vier apenas um valor, usa esse valor diretamente
            if (is_scalar($valor)) {
                $filtroExecucaoOrcamentaria->$campo->$prop = $valor;
                continue;
            }

            // -- Se vier um array com apenas um valor, extrai este valor do array e o usa diretamente
            if (1 == count($valor)) {
                $filtroExecucaoOrcamentaria->$campo->$prop = current($valor);
                continue;
            }

            // -- Se vier um array com v�rios elementos, utiliza esta lista diretamente
            $filtroExecucaoOrcamentaria->$campo->$prop = $valor;
        }

        // -- Retorno da consulta
        $selecaoRetornoExecucaoOrcamentaria = new selecaoRetornoExecucaoOrcamentariaDTO();
        foreach ($selRetorno as $ret) {
            if (!property_exists($selecaoRetornoExecucaoOrcamentaria, $ret)) {
                throw new Exception("O retorno '{$ret}' n�o � v�lido para o m�todo WSQuantitaivo::consultarExecucaoOrcamentaria.");
            }
            $selecaoRetornoExecucaoOrcamentaria->$ret = true;
        }

        // -- Execu��o da consulta
        $consultarExecucaoOrcamentaria = new consultarExecucaoOrcamentaria();
        $consultarExecucaoOrcamentaria->credencial = $this->credenciais;
        $consultarExecucaoOrcamentaria->filtro = $filtroExecucaoOrcamentaria;
        $consultarExecucaoOrcamentaria->selecaoRetorno = $selecaoRetornoExecucaoOrcamentaria;
        if (!is_null($pagina)) {
            $consultarExecucaoOrcamentaria->paginacao;
        }

        $consultarExecucaoOrcamentariaResponse = $this->soapClient->call(
            'consultarExecucaoOrcamentaria',
            array($consultarExecucaoOrcamentaria)
        );

        if ($retornaArray) {
            if (!$consultarExecucaoOrcamentariaResponse instanceof consultarExecucaoOrcamentariaResponse) {
                throw new Exception('Inst�ncia de ' . get_class($consultarExecucaoOrcamentariaResponse) . ' n�o pode ser convertida para array.');
            }
            return $this->execucaoOrcamentariaComoArray($consultarExecucaoOrcamentariaResponse, $selRetorno);
        } else {
            return $consultarExecucaoOrcamentariaResponse;
        }
    }

    /**
     * Fun��o de processamento do retorna de consultarExecucaoOrcamentaria. Transforma o conjunto de<br />
     * objetos de retorno em um array com os atributos definidos em $selRetorno.
     * @param consultarExecucaoOrcamentariaResponse $execOrc Resultado da chamada de consultarExecucaoOrcamentaria.
     * @param array $selRetorno Campos que dever�o ser processados para retorno.
     * @return array
     */
    protected function execucaoOrcamentariaComoArray(consultarExecucaoOrcamentariaResponse $execOrc, array $selRetorno)
    {
        $execOrc = $execOrc->return->execucoesOrcamentarias->execucaoOrcamentaria;
        if (empty($execOrc)) {
            return array();
        }

        // -- Processamento de um �nico retorno
        if (is_object($execOrc)) {
            $ar = array();
            foreach ($selRetorno as $ret) {
                $ar[$ret] = $execOrc->$ret;
            }
            return array($ar);
        }

        // -- Processamento de um retorno composto
        $retornoExec = array();
        foreach ($execOrc as $exec) {
            $ar = array();
            foreach ($selRetorno as $ret) {
                $ar[$ret] = $exec->$ret;
            }
            $retornoExec[] = $ar;
        }

        return $retornoExec;
    }
}
