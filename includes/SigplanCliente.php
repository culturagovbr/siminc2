<?php

require_once( 'CurlHTTPClient.php' );

class SigplanCliente
{
	/**
	 * URL base para requisi��o de servi�os do ambiente de homologa��o do
	 * Sigplan.
	 * 
	 * @var string
	 */
	const URL_HOMOLOGACAO = 'http://homsigplan.serpro.gov.br/infrasig/INFRASIG.ASMX/';
	
	/**
	 * URL base para requisi��o de servi�os do ambiente de produ��o do Sigplan.
	 * 
	 * @var string
	 */
	const URL_PRODUCAO = 'http://www.sigplan.gov.br/infrasig/sigtoinfra.asmx/';
	
	/**
	 * Senha do Sigplan.
	 * 
	 * @var string
	 */
	protected $senha = '';
	
	/**
	 * Servi�os existentes do Sigplan.
	 * 
	 * @var string[]
	 */
	private static $servicos = array(
		'recebePrograma',
		'recebeIndicador',
		'recebeRestricaoPrograma',
		'recebeAcao',
		'recebeRestricaoAcao',
		'recebeDadoFisico',
		'recebeDadoFinanceiro',
		'recebeDadoFisicoRAP',
		'recebeDadoFinanceiroRAP',
		'recebeValidacaoTrimestral',
		'geracaoPorUO',
	);
	
	/**
	 * Armazena �ltimo erro ocorrido durante uma execu��o de servi�o.
	 * 
	 * @var string
	 */
	protected $ultimoErro = '';
	
	/**
	 * URL base utilizado pela inst�ncia.
	 * 
	 * @var string
	 */
	protected $url = '';
	
	/**
	 * Usuario do Sigplan.
	 * 
	 * @var string
	 */
	protected $usuario = '';
	
	/**
	 * Inicia cliente utilizando servidor de homologa��o.
	 * 
	 * @param string
	 * @param string
	 * @return void
	 */
	public function __construct( $usuario, $senha )
	{
		$this->usuario = (string) $usuario;
		$this->senha = (string) $senha;
		$this->usarHomologacao();
	}
	
	/**
	 * Executa um servi�o Sigplan.
	 * 
	 * @param string $servico
	 * @param string $dados
	 * @return mixed
	 */
	public function executar( $servico, $arrDados )
	{
		$this->ultimoErro = '';
		
		//verifica se servi�o existe
		if ( in_array( $servico,  self::$servicos ) == false )
		{
			$this->ultimoErro = 'Servi�o ' . $servico . ' n�o existe.';
			throw new Exception( $this->ultimoErro );
			return null;
		}
		
		
		$arrPostData = array('usuario' => $this->usuario, 'senha' => $this->senha );
		$arrPostData = array_merge( $arrPostData , $arrDados );
		
		$strUrl = $this->url . $servico ;
		
		dbg( $arrPostData );
		dbg( $strUrl );
		
		$objCurl = new Curl_HTTP_Client( true );
		$html_data = $objCurl->send_post_data( $strUrl , $arrPostData);

		return $this->interpretarResultado( utf8_decode(  $html_data ) );
	}
	
	/**
	 * Interpreta o resultado de uma execu��o de servi�o.
	 * 
	 * @param string $resultado
	 * @return mixed
	 */
	protected function interpretarResultado( $resultado )
	{
		return $resultado;
	}
	
	/**
	 * Retorna �ltimo erro ocorrido. Caso n�o exista erro um texto vazio �
	 * retornado.
	 * 
	 * @return string
	 */
	public function ultimoErro()
	{
		return $this->ultimoErro;
	}
	
	/**
	 * Altera o cliente, que passa a utilizar o servidor de homologa��o do
	 * Sigplan.
	 * 
	 * @return void
	 */
	public function usarHomologacao()
	{
		$this->url = self::URL_HOMOLOGACAO;
	}
	
	/**
	 * Altera o cliente, que passa a utilizar o servidor de produ��o do Sigplan.
	 * 
	 * @return void
	 */
	public function usarProducao()
	{
		$this->url = self::URL_PRODUCAO;
	}
	
}

?>