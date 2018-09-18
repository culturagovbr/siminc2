<?php
/**
 * Implementa a classe de acesso ao webservice WSQuantitativo da SOF.
 * $Id: Quantitativo.php 101880 2015-08-31 19:50:33Z maykelbraz $
 */

/**
 * Classe base com as configurações comuns para os webservices da SOF.
 * @see Spo_Ws_Sof
 */
require_once(dirname(__FILE__) . '/../Sof.php');

/**
 * Classes de mapeamento de dados para o webservice WSQuantitativo da SOF.
 * @see WSQuantitavoMap.php
 */
require_once(dirname(__FILE__) . '/QuantitativoMap.php');

/**
 * Classe de acesso ao Webserviceo WSQuantitativo da SOF.
 */
class Spo_Ws_Sof_Quantitativo extends Spo_Ws_Sof
{
    /**
     * Número de registros retornados por página.
     */
    const REGISTROS_POR_PAGINA = 2000;

    /**
     * {@inheritdoc}
     */
    protected function loadURL()
    {
        $this->urlWSDL = WEB_SERVICE_SIOP_URL. 'WSQuantitativo?wsdl';
        return $this;
    }

    /**
     * {@inheritdoc}
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

    /**
     * Checks if an argument list matches against a valid argument type list
     * @param array $arguments The argument list to check
     * @param array $validParameters A list of valid argument types
     * @return boolean true if arguments match against validParameters
     * @throws Exception invalid function signature message
     */
    public function _checkArguments($arguments, $validParameters) {
    	$variables = "";
    	foreach ($arguments as $arg) {
    		$type = gettype($arg);
    		if ($type == "object") {
    			$type = get_class($arg);
    		}
    		$variables .= "(".$type.")";
    	}
    	if (!in_array($variables, $validParameters)) {
    		throw new Exception("Invalid parameter types: ".str_replace(")(", ", ", $variables));
    	}
    	return true;
    }

	/**
	 * Service Call: cadastrarAcompanhamentoOrcamentario
	 * Parameter options:
	 * (cadastrarAcompanhamentoOrcamentario) cadastrarAcompanhamentoOrcamentario
	 * @logger true
	 * @name Cadastrar acompanhamento orçamentario
	 * @param mixed,... See function description for parameter options
	 * @return cadastrarAcompanhamentoOrcamentarioResponse
	 * @throws Exception invalid function signature message
	 */
	public function cadastrarAcompanhamentoOrcamentario($mixed = null) {
		$validParameters = array(
			"(cadastrarAcompanhamentoOrcamentario)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		return $this->getSoapClient()->call("cadastrarAcompanhamentoOrcamentario", $args);
	}

	/**
	 * Service Call: cadastrarProposta
	 * Parameter options:
	 * (cadastrarProposta) cadastrarProposta
	 * @param mixed,... See function description for parameter options
	 * @return cadastrarPropostaResponse
	 * @throws Exception invalid function signature message
	 */
	public function cadastrarProposta(PropostaDTO $proposta = null) {
		$validParameters = array(
			"(PropostaDTO)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);

		$cadastrarProposta = new CadastrarProposta();
		$cadastrarProposta->credencial = $this->credenciais;
		$cadastrarProposta->proposta = $proposta;

		return $this->getSoapClient()->call("cadastrarProposta", array($cadastrarProposta));
	}

	/**
	 * Service Call: consultarAcompanhamentoFisicoFinanceiro
	 * Parameter options:
	 * (consultarAcompanhamentoFisicoFinanceiro) consultarAcompanhamentoFisicoFinanceiro
	 * @param mixed,... See function description for parameter options
	 * @return consultarAcompanhamentoFisicoFinanceiroResponse
	 * @throws Exception invalid function signature message
	 */
	public function consultarAcompanhamentoFisicoFinanceiro(ConsultarAcompanhamentoFisicoFinanceiro $mixed = null) {
		$validParameters = array(
			"(ConsultarAcompanhamentoFisicoFinanceiro)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
                $mixed->credencial = $this->credenciais;
		return $this->getSoapClient()->call("consultarAcompanhamentoFisicoFinanceiro", $args);
	}

	/**
	 * Service Call: consultarAcompanhamentoOrcamentario
	 * Parameter options:
	 * (consultarAcompanhamentoOrcamentario) consultarAcompanhamentoOrcamentario
	 * @param mixed,... See function description for parameter options
	 * @return consultarAcompanhamentoOrcamentarioResponse
	 * @throws Exception invalid function signature message
	 */
	public function consultarAcompanhamentoOrcamentario($mixed = null) {
		$validParameters = array(
			"(consultarAcompanhamentoOrcamentario)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		return $this->getSoapClient()->call("consultarAcompanhamentoOrcamentario", $args);
	}

	/**
	 * Service Call: consultarEmendasLocalizador
	 * Parameter options:
	 * (consultarEmendasLocalizador) consultarEmendasLocalizador
	 * @param mixed,... See function description for parameter options
	 * @return consultarEmendasLocalizadorResponse
	 * @throws Exception invalid function signature message
	 */
	public function consultarEmendasLocalizador($mixed = null) {
            $validParameters = array(
                "(consultarEmendasLocalizador)",
            );
            $args = func_get_args();
            $this->_checkArguments($args, $validParameters);
            return $this->getSoapClient()->call("consultarEmendasLocalizador", $args);
	}

	/**
	 * Faz a chamada do método WSQuantitativo::consultarExecucaoOrcamentaria, que apresenta<br />
	 * informações sobre a execução orçamentária em um determinado período.<br />
	 * <pre>
	 * $filtro = array('anoReferencia' => 2014, 'acoes' => array('20RJ', '20RK'));
	 * $retorno = array('dotAtual', 'acao', 'planoOrcamentario');
	 * $ws = new Spo_Ws_Sof_Quantitativo('', Spo_Ws_Sof_Quantitativo::STAGING);
	 * $ws->consultarExecucaoOrcamentaria($filtro, $retorno);
	 * </pre>
	 * A página inicial é a nº 1, o número padrão de registros por página é de 30k registros.
         * @logger true
	 * @name Consultar execução orçamentária
	 * @param array $filtro Lista com pares nome do filtro e valor/lista de valores para selecionar dados da consulta.
	 * @param array $selRetorno Lista com o nome dos campos que devem ser retornados na consulta.
	 * @param int $pagina Número da página da consulta, no caso de uma consulta pagina.
	 * @param bool $retornaArray Retorna o resultado da consulta como array - o retorno contém apenas os campos indicados em $selecaoRetorno.
	 * @return SoapFault|consultarExecucaoOrcamentariaResponse|array
	 * @throws Exception Lança uma exceção se um filtro ou retorno não estiver definido.
	 * @see filtroExecucaoOrcamentariaDTO
	 * @see selecaoRetornoExecucaoOrcamentariaDTO
	 */
	function consultarExecucaoOrcamentaria(array $filtro, $selRetorno = null, $pagina = 1, $retornaArray = false)
	{
            // -- Filtros da consulta
            $filtroExecucaoOrcamentaria = new filtroExecucaoOrcamentariaDTO();
            foreach ($filtro as $campo => $valor) {
                if (!property_exists($filtroExecucaoOrcamentaria, $campo)) {
                    throw new Exception("O filtro '{$campo}' não é válido para o método WSQuantitativo::consultarExecucaoOrcamentaria.");
                }

                $filtroExecucaoOrcamentaria->$campo = $valor;
            }

            // -- Retorno da consulta
            $selecaoRetornoExecucaoOrcamentaria = new selecaoRetornoExecucaoOrcamentariaDTO();
            $selRetorno = $selRetorno ? $selRetorno : array_keys(get_class_vars('SelecaoRetornoExecucaoOrcamentariaDTO'));
            foreach ($selRetorno as $ret) {
                if (!property_exists($selecaoRetornoExecucaoOrcamentaria, $ret)) {
                    throw new Exception("O retorno '{$ret}' não é válido para o método WSQuantitaivo::consultarExecucaoOrcamentaria.");
                }
                $selecaoRetornoExecucaoOrcamentaria->$ret = true;
            }

            // -- Execução da consulta
            $consultarExecucaoOrcamentaria = new consultarExecucaoOrcamentaria();
            $consultarExecucaoOrcamentaria->credencial = $this->credenciais;
            $consultarExecucaoOrcamentaria->filtro = $filtroExecucaoOrcamentaria;
            $consultarExecucaoOrcamentaria->selecaoRetorno = $selecaoRetornoExecucaoOrcamentaria;

            // -- Controle de paginação das consultas
            $consultarExecucaoOrcamentaria->paginacao = new paginacaoDTO();
            $consultarExecucaoOrcamentaria->paginacao->pagina = $pagina;
            $consultarExecucaoOrcamentaria->paginacao->registrosPorPagina = self::REGISTROS_POR_PAGINA;

            $consultarExecucaoOrcamentariaResponse = $this->getSoapClient()->call(
                'consultarExecucaoOrcamentaria',
                array($consultarExecucaoOrcamentaria)
            );

            if ($retornaArray) {
                if (!$consultarExecucaoOrcamentariaResponse instanceof consultarExecucaoOrcamentariaResponse) {
                    throw new Exception('Instância de ' . get_class($consultarExecucaoOrcamentariaResponse) . ' não pode ser convertida para array.');
                }
                return $this->execucaoOrcamentariaComoArray($consultarExecucaoOrcamentariaResponse, $selRetorno);
            } else {
                return $consultarExecucaoOrcamentariaResponse;
            }
	}

	/**
	 * Service Call: consultarExecucaoOrcamentariaEstataisMensal
	 * Parameter options:
	 * (consultarExecucaoOrcamentariaEstataisMensal) consultarExecucaoOrcamentariaEstataisMensal
	 * @param mixed,... See function description for parameter options
	 * @return consultarExecucaoOrcamentariaEstataisMensalResponse
	 * @throws Exception invalid function signature message
	 */
	public function consultarExecucaoOrcamentariaEstataisMensal($mixed = null) {
		$validParameters = array(
			"(consultarExecucaoOrcamentariaEstataisMensal)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		return $this->getSoapClient()->call("consultarExecucaoOrcamentariaEstataisMensal", $args);
	}

	/**
	 * Service Call: consultarExecucaoOrcamentariaMensal
	 * Parameter options:
	 * (consultarExecucaoOrcamentariaMensal) consultarExecucaoOrcamentariaMensal
	 * @param mixed,... See function description for parameter options
	 * @return consultarExecucaoOrcamentariaMensalResponse
	 * @throws Exception invalid function signature message
	 */
	public function consultarExecucaoOrcamentariaMensal($mixed = null) {
		$validParameters = array(
			"(consultarExecucaoOrcamentariaMensal)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		return $this->getSoapClient()->call("consultarExecucaoOrcamentariaMensal", $args);
	}

	/**
	 * Service Call: consultarProposta
	 * Parameter options:
	 * (consultarProposta) consultarProposta
	 * @logger true
	 * @name Consultar proposta
	 * @param mixed,... See function description for parameter options
	 * @return consultarPropostaResponse
	 * @throws Exception invalid function signature message
	 */
	public function consultarProposta(ConsultarProposta $consultarProposta = null) {
		$validParameters = array(
			"(ConsultarProposta)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		$consultarProposta->credencial = $this->credenciais;
		return $this->getSoapClient()->call("consultarProposta", $args);
	}

	/**
	 * Service Call: excluirProposta
	 * Parameter options:
	 * (excluirProposta) excluirProposta
	 * @param mixed,... See function description for parameter options
	 * @return excluirPropostaResponse
	 * @throws Exception invalid function signature message
	 */
	public function excluirProposta($mixed = null) {
		$validParameters = array(
			"(excluirProposta)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		return $this->getSoapClient()->call("excluirProposta", $args);
	}

	/**
	 * Service Call: obterAcoesDisponiveisAcompanhamentoOrcamentario
	 * Parameter options:
	 * (obterAcoesDisponiveisAcompanhamentoOrcamentario) obterAcoesDisponiveisAcompanhamentoOrcamentario
	 * @logger true
	 * @name Ações para acompanhamento
	 * @param mixed,... See function description for parameter options
	 * @return obterAcoesDisponiveisAcompanhamentoOrcamentarioResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterAcoesDisponiveisAcompanhamentoOrcamentario(ObterAcoesDisponiveisAcompanhamentoOrcamentario $obterAcoesDisponiveisAcompanhamentoOrcamentario = null) {
		$validParameters = array(
			"(ObterAcoesDisponiveisAcompanhamentoOrcamentario)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		$obterAcoesDisponiveisAcompanhamentoOrcamentario->credencial = $this->credenciais;
		return $this->getSoapClient()->call("obterAcoesDisponiveisAcompanhamentoOrcamentario", $args);
	}

	/**
	 * Service Call: obterDatasCargaSIAFI
	 * Parameter options:
	 * (obterDatasCargaSIAFI) obterDatasCargaSIAFI
	 * @param mixed,... See function description for parameter options
	 * @return obterDatasCargaSIAFIResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterDatasCargaSIAFI($mixed = null) {
		$validParameters = array(
			"(obterDatasCargaSIAFI)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		return $this->getSoapClient()->call("obterDatasCargaSIAFI", $args);
	}

	/**
	 * Service Call: obterExecucaoOrcamentariaSam
	 * Parameter options:
	 * (obterExecucaoOrcamentariaSam) obterExecucaoOrcamentariaSam
	 * @param mixed,... See function description for parameter options
	 * @return obterExecucaoOrcamentariaSamResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterExecucaoOrcamentariaSam($mixed = null) {
		$validParameters = array(
			"(obterExecucaoOrcamentariaSam)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		return $this->getSoapClient()->call("obterExecucaoOrcamentariaSam", $args);
	}

	/**
	 * Service Call: obterInformacaoCaptacaoPLOA
	 * Parameter options:
	 * (obterInformacaoCaptacaoPLOA) obterInformacaoCaptacaoPLOA
	 * @param mixed,... See function description for parameter options
	 * @return obterInformacaoCaptacaoPLOAResponse
	 * @throws Exception invalid function signature message
     *
	 * @logger true
	 * @name Obter Informações de Captação da PLOA
	 */
	public function obterInformacaoCaptacaoPLOA($mixed = null)
    {
		$validParameters = array(
			"(ObterInformacaoCaptacaoPLOA)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
        $mixed->credencial = $this->credenciais;
        
		return $this->getSoapClient()->call("obterInformacaoCaptacaoPLOA", $args);
	}

	/**
	 * Service Call: obterProgramacaoCompletaQuantitativo
	 * Parameter options:
	 * (obterProgramacaoCompletaQuantitativo) obterProgramacaoCompletaQuantitativo
	 * @logger true
	 * @name Programação Completa Quantitativo
	 * @param mixed,... See function description for parameter options
	 * @return obterProgramacaoCompletaQuantitativoResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterProgramacaoCompletaQuantitativo(ObterProgramacaoCompletaQuantitativo $mixed = null) {
		$validParameters = array(
			"(ObterProgramacaoCompletaQuantitativo)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		$mixed->credencial = $this->credenciais;
		return $this->getSoapClient()->call("obterProgramacaoCompletaQuantitativo", $args);
	}

	/**
	 * Service Call: obterTabelasApoioQuantitativo
	 * Parameter options:
	 * (obterTabelasApoioQuantitativo) obterTabelasApoioQuantitativo
	 * @param mixed,... See function description for parameter options
	 * @return obterTabelasApoioQuantitativoResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterTabelasApoioQuantitativo($mixed = null) {
		$validParameters = array(
			"(obterTabelasApoioQuantitativo)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		return $this->getSoapClient()->call("obterTabelasApoioQuantitativo", $args);
	}

	/**
	 * Função de processamento do retorna de consultarExecucaoOrcamentaria. Transforma o conjunto de<br />
	 * objetos de retorno em um array com os atributos definidos em $selRetorno.
	 * @param consultarExecucaoOrcamentariaResponse $execOrc Resultado da chamada de consultarExecucaoOrcamentaria.
	 * @param array $selRetorno Campos que deverão ser processados para retorno.
	 * @return array
	 */
	protected function execucaoOrcamentariaComoArray(consultarExecucaoOrcamentariaResponse $execOrc, array $selRetorno)
	{
		$execOrc = $execOrc->return->execucoesOrcamentarias->execucaoOrcamentaria;
		if (empty($execOrc)) {
			return array();
		}

		// -- Processamento de um único retorno
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
