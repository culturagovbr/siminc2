<?php

include_once 'RestReceitaFederal.php';
include_once 'AdapterReceitaFederalSimec.php';

/**
 * Classe para acesso ao webservice de pessoa jur�dica.
 * 
 * PS: N�o esque�a de ler o leiame.txt
 *
 */
final class PessoaJuridicaClient
{
	/**
	 * Coloca o objeto do cliente do webservice.
	 *
	 * @var SoapClient
	 */
	private $soapClient;

	
	/**
	 * Construtor da classe.
	 *
	 * @param string $wsdl
	 */
	public function __construct($wsdl)
	{

//		try{
//			$this->soapClient = new SoapClient( $wsdl );
//		} catch (Exception $e){
//			exit("N�o est� conectado!");
//		}
		
	}
	
	/**
	 * Retorna dados de pessoa jur�dica pelo CNPJ.
	 *
	 * @param string $cnpj
	 * @return string
	 */
	public function solicitarDadosResumidoPessoaJuridicaPorCnpj( $cnpj )
	{
		return AdapterReceitaFederalSimec::solicitarDadosPessoaJuridicaPorCnpj($cnpj);
	}
	
	/**
	 * Retorna dados completo de pessoa jur�dica por CNPJ.
	 *
	 * @param string $cnpj
	 * @return string
	 */
	public function solicitarDadosPessoaJuridicaPorCnpj( $cnpj )
	{
		return AdapterReceitaFederalSimec::solicitarDadosPessoaJuridicaPorCnpj($cnpj);
	}
	
	/**
	 * Retorna dados de Endere�o da pessoa jur�dica por CNPJ.
	 *
	 * @param string $cnpj
	 * @return string
	 */
	public function solicitarDadosEnderecoPessoaJuridicaPorCnpj( $cnpj )
	{
		return (  $this->soapClient->solicitarDadosEnderecoPessoaJuridicaPorCnpj( $cnpj ) );
	}
	
	/**
	 * Retorna dados de Contato da pessoa jur�dica por CNPJ.
	 *
	 * @param string $cnpj
	 * @return string
	 */
	public function solicitarDadosContatoPessoaJuridicaPorCnpj( $cnpj )
	{
		return (  $this->soapClient->solicitarDadosContatoPessoaJuridicaPorCnpj( $cnpj ) );
	}
	
	/**
	 * Retorna as informa��es do s�cio da pessoa jur�dica.
	 *
	 * @param string $cnpj
	 * @return string
	 */
	public function solicitarDadosSocioPessoaJuridicaPorCnpj( $cnpj )
	{
		return (  $this->soapClient->solicitarDadosSocioPessoaJuridicaPorCnpj( $cnpj ) );
	}
}
