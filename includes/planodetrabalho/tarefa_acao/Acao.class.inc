<?php
class Acao
{
	/**
	 * Nome da tabela do banco onde ficam armazenadas os projetos
	 *
	 */
	public $TABLE_NAME = 'planejamento.acao';
	
	/**
	 * Identificador do projeto
	 */
	public $ROW_NAME_ID 				= 'acaid';
	
	/**
	 * Codigo do Acao 
	 * 
	 */
	public $ROW_NAME_STR_CODE			= 'acacod';
	
	/**
	 * Coluna de texto que contém o codigo do tipo da acao
	 */
	public $ROW_NAME_STR_TYPE_CODE 			= 'taccod';
	
	/**
	 * Coluna do ano da data que contém a data de início do projeto
	 */
	public $ROW_NAME_DATE_YEAR_START 	= 'acaanoinicio';
	
	/**
	 * Coluna do ano da data que contém a data de término do projeto
	 */
	public $ROW_NAME_DATE_YEAR_END 		= 'acaanotermino';
	
	/**
	 * Coluna do mes da data que contém a data de início do projeto
	 */
	public $ROW_NAME_DATE_MONTH_START 	= 'acamesinicio';
	
	/**
	 * Coluna do mes da data que contém a data de término do projeto
	 */
	public $ROW_NAME_DATE_MONTH_END 	= 'acamestermino';
	
	/**
	 * @see ROW_NAME_ID
	 */
	protected $intId = null;
	
	/**
	 * @var string
	 * @see Acao::ROW_NAME_STR_NAME
	 */
	protected $strNome = null;
	
	/**
	 * @var date
	 * @see Acao::ROW_NAME_DATE_START
	 */
	protected $datInicio = null;
	
	/**
	 * @var date
	 * @see Acao::ROW_NAME_DATE_END
	 */
	protected $datFim = null;
	
	protected $strElementoNome = null;
	
	/**
	 * Array das tarefas que deste projeto
	 */
	protected $arrTarefasDaAcao = null;
	
	/**
	 * Array estatico das projetos já lidas. Evita se instanciar mais
	 * de uma copia do mesmo objeto e recursividade eterna.
	 *
	 * @var Array of Acao
	 */
	static protected $arrInstances = array();
	
	/**
	 * @var object database connection
	 */		
	static protected $objDatabase;

	/**
	 * Se o objeto já teve suas datas alteradas
	 * 
	 * @var bool
	 */
	protected $boolChanged;
	
	protected $strTipoCodigo;
	
	protected $strCodigo = null;
	
	protected $intAnoExercicioCorrente = null;

	// sets e gets //
			
	public function setId( $intValue )
	{
		if( $intValue !== null)
		{
			$this->intId = (integer) $intValue;
		}
		else
		{
			$this->intId = null;
		}
	}
	
	public function getId()
	{
		return $this->intId;
	}
	
	public function setNome( $strNome )
	{
		if( $strNome !== null )
		{
			$this->strNome = $strNome;
		}
		else
		{
			$this->strNome = null;
		}
	}
	
	public function getNome()
	{
		return $this->strNome;
	}
	
	public function setDescricao( $strDescricao )
	{
		if( $strDescricao !== null )
		{
			$this->strDescricao =  (string) $strDescricao;
		}
		else
		{
			$this->strDescricao = null;
		}
	}
	
	public function setDataInicio( $datInicio )
	{
		if( $datInicio !== null )
		{
			$this->datInicio = $datInicio;
		}
		else
		{
			$this->datInicio = null;
		}
	}
	
	public function getDataInicio()
	{
		return $this->datInicio;
	}
	
	public function getDataInicioTimestamp()
	{
		return convertToTimestamp( $this->datInicio );
	}
	
	public function getNovaDataInicio()
	{
		return $this->datNovaDataInicio;
	}
	
	public function setNovaDataInicio( $datInicio )
	{
		if( $datInicio !== null )
		{
			$this->datNovaDataInicio = $datInicio;
		}
		else
		{
			$this->datNovaDataInicio = null;
		}
	}
	
	public function setDataFim( $datFim )
	{
		if( $datFim !== null )
		{
			$this->datFim = $datFim;
		}
		else
		{
			$this->datFim = null;
		}
	}
	
	public function getDataFim()
	{
		return $this->datFim;
	}
	
	public function getDataFimTimestamp()
	{
		return convertToTimestamp( $this->datFim );
	}
	
	public function setAnoExercicio( $intAnoExercicio )
	{
		$this->intAnoExercicioCorrente = $intAnoExercicio;
	}
	
	public function getAnoExercicio()
	{
		return $this->intAnoExercicioCorrente;
	}
	
	public function setElementoNome( $strNome )
	{
		$this->strElementoNome = $strNome;
	}
	
	public function getElementoNome()
	{
		return $this->strElementoNome;
	}
	
	public function getElementoNomePlural()
	{
		return $this->strElementoNome . 's';
	}
	/**
	 * Retorna o tamanho da duração da projeto em timestamp
	 *
	 * @return integer
	 */
	public function getDuracaoTimestamp()
	{
		return $this->getDataFimTimestamp() - $this->getDataInicioTimestamp();
	}
	
	public function setCodigo( $strCodigo )
	{
		$this->strCodigo = $strCodigo;
	}
	
	public function getCodigo()
	{
		return $this->strCodigo;
	}
	
	public function setTipoCodigo( $strTipoCodigo )
	{
		$this->strTipoCodigo = $strTipoCodigo;
	}
	
	public function getTipoCodigo()
	{
		return $this->strTipoCodigo;
	}
	
	// fim do setters and getters //
	
	/**
	 * Construtor dos projetos
	 */
	public function __construct()
	{
		global $db;
		self::$objDatabase = $db;
	}
	
	/**
	 * Retorna a Acao ao receber o id da mesma.
	 *
	 * @param integer $intId
	 * @return Acao
	 */
	public static function getAcaoPeloId( $intId )
	{
		if( isset( Acao::$arrInstances[ $intId ] ) )
		{
			$objAcao = Acao::$arrInstances[ $intId ];
		}
		else
		{
			$objAcao = new self();
			$objAcao->setId( $intId );
			$objAcao->emerge();
			Acao::$arrInstances[ $intId ] = $objAcao;
		}
		return $objAcao;
	}
	
	/**
	 * Retorna se a tarefa foi alterada ou não
	 */
	public function getDatasAlteradas()
	{
		return false;
	}
		
	public function getMensagemdeErroAcaoNaoComporta()
	{
		return	'Não foi possível alterar os datas da acao pois o período não comporta as alterações necessárias.' .
			'';			
	}
	
	public function getMensagemErroAcaoFechada()
	{
		return	'Não foi possível alterar os datas projeto pois a data do mesmo é fechada.' . 
			$this->getDataInicio() . ':' . $this->getDataFim() . 
			' - ' . 
			$this->getNovaDataInicio() . ':' . $this->getNovaDataFim() .
			'';			
	}

	public function getArrTarefasDaAcao()
	{
		if( $this->arrTarefasDaAcao === null )
		{
			 $this->arrTarefasDaAcao = orderArrayOfObjectsByMethod(  TarefaAcao::getArrTarefasPelaAcao( $this )  , 'getDataInicioTimestamp' );
		}
		return $this->arrTarefasDaAcao;
	}
	
	/**
	 * 
	 * Verifica se a Acao esta valida e pode ser salva
	 *
	 */
	public function valida()
	{
		throw new Exception( 'Metodo nao implementado' );
	}
	
	/**
	 * Função responsável pela administração das datas do projeto se 
	 * adequarem corretamente e automaticamente às alterações feitas
	 * nas tarefas.
	 * 
	 * Este método não deve ser diretamente chamado quando o projeto
	 * for alterado, mas sim durante as alterações das tarefas para
	 * conferir se as datas estão compatíveis e manter o processo de
	 * cascata.
	 */
	public function verificaPossibilidadeDeAlterar()
	{
		// uma acao jamais tem suas datas alteradads por processos automaticos //
		
		if( $this->getTipoCodigo() != 1 )
		{
			return;
		}
		
		$intLimiteDataInicio	= $this->getDataInicioTimestamp();
		$intLimiteDataFim		= $this->getDataFimTimestamp();
		$arrTarefasDaAcao 	= $this->getArrTarefasDaAcao();
		
		foreach( $arrTarefasDaAcao as $objTarefaDaAcao )
		{
			$intTarefaInicioData	= $objTarefaDaAcao->getNovaDataInicioTimestamp();
			$intTarefaFimData		= $objTarefaDaAcao->getNovaDataFimTimestamp();

			if( $intTarefaInicioData < $intLimiteDataInicio )
			{
				$intLimiteDataInicio = $intTarefaInicioData;
			}
			if( $intTarefaFimData > $intLimiteDataFim )
			{
				$intLimiteDataFim = $intTarefaFimData;
			}
		}
		if( ( $this->getDataInicio() > $intLimiteDataInicio ) || ( $this->getDataFim() < $intLimiteDataFim ) )
		{
			throw new Exception( 'Erro ao alterar a Acao. As Datas limites da Acao nao podem ser alteradas.' );
		}
	}
	
	/**
	 * Verifica a possibilidade de alterar as tarefas que esta contém.
	 * 
	 * Verifica a possibilidade de alterar as tarefas que esta contém atualizando 
	 * suas datas e mantendo a consistencia dos dados. Recebe as novas datas
	 * de inico e termino desta.
	 * 
	 * @param integer $intContainerDataInicio 
	 * @param integer $intContainerDataFim
	 */
	protected function verificaPossibilidadeDeAlterarAsQueContenho( $intContainerDataInicio , $intContainerDataFim )
	{
		throw new Exception( 'Metodo nao implementado!' );
	}
	
	/**
	 * Recebe os dados da tarefa da persistencia e os preenche na
	 * entidade.
	 *
	 */
	protected function emerge()
	{
		$strSql =	' SELECT ' .
						'*' . 
					' FROM ' .
						$this->TABLE_NAME .
					' WHERE ' .
						$this->ROW_NAME_ID . ' = ' . escape( $this->getId() ) .
					' LIMIT 1 ' .
					'';
//		dbg( $strSql );
		$objSaida = self::$objDatabase->carregar( $strSql );
		if( sizeof( $objSaida ) == 1 )
		{
			$objSaida = $objSaida[ 0 ];
						
			$this->setId(					unescape( @$objSaida[ $this->ROW_NAME_ID ]					, 'integer' ) );
			$this->setNome(					unescape( @$objSaida[ $this->ROW_NAME_STR_NAME ] 			, 'string' ) );
			$this->setCodigo(				unescape( @$objSaida[ $this->ROW_NAME_STR_CODE ]			, 'string') );
			$this->setTipoCodigo(			unescape( @$objSaida[ $this->ROW_NAME_STR_TYPE_CODE ]		, 'string') );
			
			$intAnoInicio	= 				unescape( @$objSaida[ $this->ROW_NAME_DATE_YEAR_START ]		, 'integer');
			$intMesInicio	= 				unescape( @$objSaida[ $this->ROW_NAME_DATE_MONTH_START ]	, 'integer');
			$intAnoFim		= 				unescape( @$objSaida[ $this->ROW_NAME_DATE_YEAR_END ]		, 'integer');
			$intMesFim		= 				unescape( @$objSaida[ $this->ROW_NAME_DATE_MONTH_END ]		, 'integer');

			$this->setAnoExercicio( $_SESSION[ 'exercicio' ] );
			
			if( $this->getTipoCodigo() == 1 )
			{
				$this->setDataInicio(	'01' . '/' . $intMesInicio	. '/' . $intAnoInicio );
				$strDataFimSeguinte = 	'01' . '/' . $intMesFim		. '/' . $intAnoFim;
				$this->setDataFim( date( 'd/m/Y') , convertToTimestamp( $strDataFimSeguinte ) - 1 );
				$this->setElementoNome( 'Etapa' );
			}
			else
			{
				$this->setDataInicio( '01' . '/' . '01' . '/' . $this->getAnoExercicio() );
				$this->setDataFim( '31' . '/' . '12' . '/'. $this->getAnoExercicio() );	
				$this->setElementoNome( 'Fase' );
			}
		}
		else
		{
			throw new Exception( 'Acao ( ' . $this->getId() . ' ) nao encontrada ') ;
		}
	}
	
	protected function insert()
	{
		throw new Exception( 'Medoto nao implementado' );
	}
				
	public function criaTarefa()
	{
		return TarefaAcao::criaTarefa();
	}
}
?>