<?php

ini_set( "memory_limit", "1024M" );
set_time_limit(0);

/*

// ACAO ////////////////////////////////////////////////////////////////////////

DROP VIEW elabrev.totais_previstos_dadfis;
DROP VIEW elabrev.totais_previstos_dadfinanc;

ALTER TABLE elabrev.ppaacao ALTER acacodppa TYPE char(7);
ALTER TABLE elabrev.ppaacao ALTER COLUMN acacodppa SET STATISTICS -1;

CREATE OR REPLACE VIEW elabrev.totais_previstos_dadfis AS 
 SELECT a.acacod, sum(e.fisqtdeprevistoano1) AS s_ano1, sum(e.fisqtdeprevistoano2) AS s_ano2, sum(e.fisqtdeprevistoano3) AS s_ano3, sum(e.fisqtdeprevistoano4) AS s_ano4, sum(e.fisqtdeprevistoano5) AS s_ano5, sum(e.fisqtdeprevistoano6) AS s_ano6
   FROM elabrev.dadofisico e
   JOIN elabrev.ppaacao a ON a.acacodppa = e.acacodppa AND a.acaid = e.acaid
  GROUP BY a.acacod
  ORDER BY a.acacod;
ALTER TABLE elabrev.totais_previstos_dadfis OWNER TO dba_admin;

CREATE OR REPLACE VIEW elabrev.totais_previstos_dadfinanc AS 
 SELECT a.acacod, sum(e.finvlrprevistoano1) AS s_ano1, sum(e.finvlrprevistoano2) AS s_ano2, sum(e.finvlrprevistoano3) AS s_ano3, sum(e.finvlrprevistoano4) AS s_ano4, sum(e.finvlrprevistoano5) AS s_ano5, sum(e.finvlrprevistoano6) AS s_ano6
   FROM elabrev.dadofinanceiro e
   JOIN elabrev.ppaacao a ON a.acacodppa = e.acacodppa AND a.acaid = e.acaid
  GROUP BY a.acacod
  ORDER BY a.acacod;
ALTER TABLE elabrev.totais_previstos_dadfinanc OWNER TO postgres;

// LOCALIZADOR /////////////////////////////////////////////////////////////////

ALTER TABLE elabrev.ppalocalizador ALTER acacodppa TYPE char(7);
ALTER TABLE elabrev.ppalocalizador ALTER COLUMN acacodppa SET STATISTICS -1;

ALTER TABLE elabrev.ppalocalizador ALTER loccodppa TYPE char(7);
ALTER TABLE elabrev.ppalocalizador ALTER COLUMN loccodppa SET STATISTICS -1;

ALTER TABLE localizador ALTER locdsc TYPE char(255);
ALTER TABLE localizador ALTER COLUMN locdsc SET STATISTICS -1;

// DADO FISICO /////////////////////////////////////////////////////////////////

ALTER TABLE monitora.dadofisico ALTER loccodppa TYPE char(7);
ALTER TABLE monitora.dadofisico ALTER COLUMN loccodppa SET STATISTICS -1;

ALTER TABLE monitora.dadofisico ALTER acacodppa TYPE char(7);
ALTER TABLE monitora.dadofisico ALTER COLUMN acacodppa SET STATISTICS -1;


*/

/**
 * @author Renê de Lima Barbosa <renebarbosa@mec.gov.br>
 */
class Sigplan{

	/**
	 * @var string
	 */
	const ALTERACAO = 'alteracao';

	/**
	 * @var string
	 */
	const EXCLUSAO = 'exclusao';

	/**
	 * @var string
	 */
	const INSERCAO = 'insercao';

	/**
	 * @var integer
	 */
	private $ano = '';

	/**
	 * @var cls_banco
	 */
	private $bancodedados = null;

	/**
	 * @var array
	 */
	private $chaves = array(
		'acao'              => array( 'prgano', 'prgcod', 'acacod', 'saccod' ),
		'basegeografica'    => array( 'bsgcod' ),
		'dadofinanceiro'    => array( 'prgano', 'prgcod', 'acacod', 'saccod', 'fppcod', 'regcod' ),
		'dadofisico'        => array( 'prgano', 'prgcod', 'acacod', 'saccod', 'regcod' ),
		'fonteppa'          => array( 'fppcod' ),
		'indicador'         => array( 'prgano', 'prgcod', 'indnum' ),
		'macroobjetivo'     => array( 'mobcod' ),
		'opcaoestrategica'  => array( 'oescod', 'oesdsc' ),
		'orgao'             => array( 'organo', 'orgcod', 'tpocod' ),
		'periodicidade'     => array( 'percod' ),
		'produto'           => array( 'procod' ),
		'programa'          => array( 'prgano', 'prgcod' ),
		'regiao'            => array( 'regcod' ),
		'restricaoacao'     => array( 'prgano', 'prgcod', 'acacod', 'saccod', 'trscod', 'rsinum' ),
		'restricaoprograma' => array( 'prgano', 'prgcod', 'trscod', 'rsinum' ),
		'tipoacao'          => array( 'taccod' ),
		'tipoorgao'         => array( 'tpocod' ),
		'tipoprograma'      => array( 'tprcod' ),
		'tiporestricao'     => array( 'trscod' ),
		'tiposituacao'      => array( 'tpscod' ), // no xml a chave primária é SITCod
		'unidade'           => array( 'unicod', 'unitpocod' ), // o campo UNIAno foi removido da lista para evitar sobrecarga de dados no banco
		'unidademedida'     => array( 'unmcod' ),
	);

	/**
	 * @var array
	 */
	private $historico = array();

	/**
	 * Lista que relaciona entidades xml com as tabelas do sistema.
	 * 
	 * @see self::pegarTabela()
	 * @var array
	 */
	private static $tabelas = array(
		'OpcaoEstrat'       => 'opcaoestrategica',
		'DadoFinanceiroRAP' => 'dadofinanceiro',
		'DadoFisicoRAP'     => 'dadofisico',
		'PPADadoFinanceiro' => 'dadofinanceiro',
		'PPADadoFisico'     => 'dadofisico'
	);

	/**
	 * @param integer $ano
	 * @return void
	 */
	public function __construct( $ano ){
		global $db;
		$this->bancodedados = $db;
		$this->ano          = $ano;
	}

	/**
	 * @param string $tabela
	 * @param array $registro
	 * @return boolean
	 */
	private function atualizar( $tabela, $registro ){
		// monta as atribuições
		$atribuicao = array();
		foreach ( $registro as $campo => $valor ) {
			if ( empty( $valor ) ) {
				continue;
			}
			array_push( $atribuicao, sprintf( " %s = '%s' ", $campo, $valor ) );
		}
		$atribuicao = implode( ',', $atribuicao );
		$condicao = $this->pegarCondicao( $tabela, $registro );
		// monta o comando
		$sql = sprintf(
			"update %s set %s where %s",
			$tabela,
			$atribuicao,
			$condicao
		);
		// efetua a alteração
		if ( !$this->bancodedados->executar( $sql ) ) {
			dbg( $sql, 1 );
			return false;
		}
		return true;
	}

	/**
	 * @param string $tabela
	 * @param array $registro
	 * @return boolean
	 */
	private function existe( $tabela, $registro ){
		// monta o comando
		$sql = sprintf(
			"select count(*) from %s where %s",
			$tabela,
			$this->pegarCondicao( $tabela, $registro )			
		);
		//dbg( $sql, 1 );
		// efetua a verificação
		return $this->bancodedados->pegaUm( $sql ) >= 1;
	}

	/**
	 * Registrar informações a respeito da importação de dados de uma entidade.
	 * 
	 * @return boolean
	 */
	public function gravarRelatorio(){
		// carrega o arquivo de relatório
		$arquivo = APPRAIZ . 'arquivos/SIGPLAN/importacao/'. $this->ano . '/Relatorio.xml';
		$documento = DOMDocument::load( $arquivo );
		if( !$documento ) {
			$documento = new DOMDocument( '1.0', 'utf-8' );
			$tag_relatorio = new DOMElement( 'relatorio' );
			$documento->appendChild( $tag_relatorio );
		} else {
			$tag_relatorio = $documento->getElementsByTagName( 'relatorio' )->item( 0 );
		}
		$tag_importacao = new DOMElement( 'importacao' );
		$tag_relatorio->appendChild( $tag_importacao );
		$tag_importacao->setAttribute( 'data', date( 'Y-m-d H:i:s.0T' ) );
		foreach ( $this->historico as $tabela => $historico ) {
			$tag_tabela = new DOMElement( $tabela );
			$tag_importacao->appendChild( $tag_tabela );
			foreach ( $historico as $operacao => $registros ) {
				$tag_operacao = new DOMElement( $operacao );
				$tag_tabela->appendChild( $tag_operacao );
				$tag_operacao->setAttribute( 'quantidade', count( $historico[$operacao] ) );
				foreach ( $registros as $registro ) {
					if ( !$registro ) {
						continue;
					}
					$tag_registro = new DOMElement( 'registro' );
					$tag_operacao->appendChild( $tag_registro );
					foreach ( $registro as $campo => $valor ) {
						$tag_registro->appendChild( new DOMElement( $campo, utf8_encode( $valor ) ) );
					}
				}
			}
		}
		if( !$documento->save( $arquivo ) ) {
			// TODO: tratar erro
			return false;
		}
		return true;
	}

	/**
	 * @param string $natureza
	 * @param string $arquivo
	 * @return boolean
	 */
	public function importar( $natureza, $arquivo ){
		// carrega o xml
		$arquivo = APPRAIZ . 'arquivos/SIGPLAN/importacao/'. $this->ano . '/' . $natureza . '/' . $arquivo . '.xml';
		if ( !file_exists( $arquivo ) ) {
			// @TODO: manipular erro
 			return false;
		}
		$elemento = simplexml_load_file( $arquivo );
		if ( !$elemento ) {
			// @TODO: manipular erro
			return false;
		}
				
		return $this->importarTabela( $natureza, $elemento );
	}

	/**
	 * @param string $natureza
	 * @param array $elemento
	 * @return boolean
	 */
	private function importarTabela( $natureza, $elemento ){		
		// percorre a árvore xml
		foreach( (array) $elemento as $entidade => $item ) {			
			// verifica se o item é vazio
			if ( empty( $item ) ) {
				continue;
			}
			// verifica se o item é uma lista
			if ( is_array( current( $item ) ) ) {
				$this->importarTabela( $natureza, $item );
				continue;
			}
			// identifica a tabela que corresponde a entidade
			$tabela = $this->pegarTabela( $entidade );
			if ( !$tabela ) {
				continue;
			}
			// efetua a importação dos registros
			foreach ( (array) $item as $registro ) {				
				// filtra os dados do registro
				$registro = (array) $registro;
				$registro = array_combine( array_map( 'strtolower', array_keys( $registro ) ), array_values( $registro ) );
				$registro = array_map( 'utf8_decode', $registro );
				$registro = array_map( 'trim', $registro );
				$registro = array_map( 'addslashes', $registro );
				$metodo = 'importarTabela' . $natureza;
				if ( !$this->$metodo( $tabela, $registro ) ) {
					return false;
				}
			}
		}
		return true;
	}

	/**
	 * @param string $tabela
	 * @param array $registro
	 * @return integer
	 */
	protected function importarTabelaApoio( $tabela, $registro ){
		// burla a incompatibilidade entre o arquivo CargaTipoSituacao.xml e a tabela tiposituacao
		if ( $tabela == 'tiposituacao' ) {
			$registro = array(
				'tpscod' => $registro['sitcod'],
				'tpsdsc' => $registro['sitdsc']
			);
		}
		if( $this->existe( $tabela, $registro ) ) {
			// identifica o campo de descrição
			$campo = '';
			foreach ( array_keys( $registro ) as $campo ) {
				if ( ereg( '^([a-zA-Z]{3})+(dsc|nome)$', $campo ) ) {
					break;
				}
			}
			if ( !$campo ) {
				return false;
			}
			// verifica se a descrição é diferente
			$sql = sprintf(
				"select %s from %s where %s",
				$campo,
				$tabela,
				$this->pegarCondicao( $tabela, $registro )
			);
			$descricao = addslashes( trim( $this->bancodedados->pegaUm( $sql ) ) );
			if ( strcmp( $descricao, $registro[$campo] ) != 0 ) {
				$this->registrar( $tabela, $registro, self::ALTERACAO );
			}
		} else {
			if ( !$this->inserir( $tabela, $registro ) ) {
				return false;  
			}
			$this->registrar( $tabela, $registro, self::INSERCAO );
		}
		return true;
	}

	/**
	 * @param string $tabela
	 * @param array $registro
	 * @return integer
	 */
	protected function importarTabelaPrincipal( $tabela, $registro ){
		static $prgid = array();
		static $acaid = array();
		//dbg($tabela);
		if ( !$this->existe( $tabela, $registro ) ) {
			//dbg($tabela, 1);			
			switch ( $tabela ) {				
				case 'monitora.indicador':
				case 'monitora.restricaoprograma':
				case 'monitora.acao':
					// identifica o prgid
					if ( !isset( $prgid[$registro['prgcod']] ) ) {
						return false;
					}
					$registro['prgid'] = $prgid[$registro['prgcod']];
					break;
				case 'monitora.restricaoacao':
				case 'monitora.dadofinanceiro':
				case 'monitora.dadofisico':
					// identifica o acaid
					if ( !isset( $acaid[$registro['prgcod']][$registro['acacod']][$registro['saccod']] ) ) {
						return false;
					}
					$registro['acaid'] = $acaid[$registro['prgcod']][$registro['acacod']][$registro['saccod']];
					
					break;
				default:
					break;
			}
			$id = $this->inserir( $tabela, $registro );
			if ( !$id ) {
				return false;
			}
			$this->registrar( $tabela, $registro, self::INSERCAO );
			switch ( $tabela ) {
				// atualiza a lista de prgid
				case 'planejamento.programa':
					$sql = sprintf( "select prgid, prgcod from programa where oid = '%s'", $id );
					$programa = $this->bancodedados->pegaLinha( $sql );
					$prgid[$programa['prgcod']] = $programa['prgid'];
					break;
				// atualiza a lista de acaid
				case 'monitora.acao':
					$sql = sprintf( "select acaid, prgcod, acacod, saccod from acao where oid = '%s'", $id );
					$acao = $this->bancodedados->pegaLinha( $sql );
					$acaid[$acao['prgcod']][$acao['acacod']][$acao['saccod']] = $acao['acaid'];
					break;
				default:
					break;
			}
		}
		return true;
	}

	/**
	 * @param string $tabela
	 * @param array $registro
	 * @return integer
	 */
	protected function importarTabelaQualitativa( $tabela, $registro ){
		static $ppa = array( 'dadofinanceiro', 'dadofisico' );
		static $apoio = array( 'ppafonte', 'ppafuncao', 'ppaorgao', 'ppasubfuncao' );
		$registro['prsano'] = $this->ano; # é necessário indicar o ano
		// identifica se o registro deve ser manipulado como tabela de apoio
		if ( in_array( $tabela, $apoio ) ) {
			return $this->importarTabelaApoio( $tabela, $registro );
		}
		// identifica os registros ppa
		// código alterado para atender à realidade da tabela ppacao sem ter que alterar toda a estrutura do banco por cauisa da aplicação
		if ($tabela == 'ppaacao')
		{
			
			$sql = " select acaid from elabrev.ppaacao where prgcod='".$registro['prgcod']."' and prsano='".$registro['prsano']."' and acacod='".$registro['acacod']."' and unicod='".$registro['unicod']."'";
    		if (! $this->bancodedados->pegaUm($sql))
    		{
    			//dbg('elabrev.ppaacao',1);
    			if( !$this->inserir( 'elabrev.ppaacao', $registro ) ) {
					dbg($registro,1);
					return false;
				}
    		}
		} 
		else if ( ereg( '^(ppa)+([a-z]{0,})', $tabela, $resultado ) || in_array( $tabela, $ppa ) ) 
		{
			// insere o registro ppa
			$tabela_ppa = 'elabrev.' . $tabela; # as tabelas ppa ficam no esquema elabrev

			if ( !$this->existe( $tabela_ppa, $registro ) ) {
				if( !$this->inserir( $tabela_ppa, $registro ) ) {
					dbg($registro,1);
					return false;
				}
				$this->registrar( $tabela_ppa, $registro, self::INSERCAO );
			}
//			CÓDIGO SUPRIMIDO PRA QUE A IMPORTAÇÃO PARCIAL FUNCIONE
//
//			// prepara o registro para atualização na tabela principal (que faz referência à tabela ppa)
//			unset( $registro['prsano'] ); # não é necessário indicar o ano
//			if ( !in_array( $tabela, $ppa ) ) {
//				$tabela = $resultado[2]; # retira o prefixo ppa
//			}
//			switch ( $tabela ) {
//				case 'dadofinanceiro':
//					unset( $registro['acacodppa'], $registro['loccodppa'], $registro['natcod'], $registro['tdpcod'] );
//					break;
//				case 'dadofisico':
//					unset( $registro['fisqtdeprevistoano1'], $registro['fisqtdeprevistoano2'], $registro['fisqtdeprevistoano3'], $registro['fisqtdeprevistoano4'], $registro['fisqtdeprevistoano5'], $registro['fisqtdeprevistoano6'], $registro['fisqtdeprevistoano7'], $registro['fisqtdeprevistoano8'], $registro['fisqtdeprevistoano9'], $registro['fisqtdeprevistoano10'], $registro['fisqtdeprevistoano11'], $registro['fisqtdeprevistoano12'], $registro['fissnmetanaocumulativa'] );
//					break;
//				case 'indicador':
//					unset( $registro['umicod'], $registro['indobs'] );
//					break;
//				case 'localizador':
//					$registro = array(
//						'loccod' => $registro['loccod'],
//						'locdsc' => $registro['locdsc']
//					);
//					break;
//				default:
//					break;
//			}
		}
//		CÓDIGO SUPRIMIDO PRA QUE A IMPORTAÇÃO PARCIAL FUNCIONE
//
//		$registro['prgano'] = $this->ano;
//		unset( $registro['unicod'] );
//		if ( !$this->atualizar( $tabela, $registro ) ) {
//			return false;
//		}
//		$this->registrar( $tabela, null, self::ALTERACAO );
		return true;
	}

	/**
	 * @param string $tabela
	 * @param array $registro
	 * @return integer
	 */
	private function inserir( $tabela, $registro ){
		$sql = sprintf(
			"insert into %s ( %s ) values ( '%s' )",
			$tabela,
			implode( ",", array_keys( (array) $registro ) ),
			implode( "','", $registro )
		);
		//dbg($sql,1);
		//if( !$cont ) { $cont = 1; }
		//echo $cont++."<br>";
		//flush();
		//ob_flush();
		
		$resultado = $this->bancodedados->executar( $sql );
		if ( !$resultado ) {
			dbg( $sql, 1 );
			return false;
		}
		return pg_last_oid( $resultado );
	}

	/**
	 * @param string $tabela
	 * @param array $registro
	 * @return string
	 */
	private function pegarCondicao( $tabela, $registro ){
		$tabela = explode( '.', $tabela );
		$esquema = $tabela[0];
		$tabela = $tabela[1];
		if ( !array_key_exists( $tabela, $this->chaves ) ) {
			//if ( ereg( '^+(\.)+', $tabela, $resultado ) ) {
			//	
			//}
			$sql = sprintf(
				"select k.column_name as campo from information_schema.table_constraints t
				inner join information_schema.key_column_usage k on t.constraint_name = k.constraint_name  
				where t.constraint_type = 'PRIMARY KEY' and t.table_name = '%s' %s
				order by k.ordinal_position",
				$tabela,
				$esquema ? " and t.table_schema = '$esquema' " : null
			);
			//dbg($sql, 1);
			$this->chaves[$tabela] = array();
			foreach ( $this->bancodedados->carregar( $sql ) as $linha ) {
				array_push( $this->chaves[$tabela], $linha['campo'] );
			}
			$this->chaves[$tabela] = array_unique( $this->chaves[$tabela] );
		}
		$condicao = array();
		foreach ( $this->chaves[$tabela] as $chave ) {
			if ( array_key_exists( $chave, $registro ) ) {
				array_push( $condicao, sprintf( " %s = '%s' ", $chave, $registro[$chave] ) );
			}
		}
		if ( count( $condicao ) == 0 ) {
			return ' 1 = 1 ';
		}
		return implode( ' and ', $condicao );
	}

	/**
	 * Obtém o nome da tabela que corresponde à entidade indicada.
	 * 
	 * @param string $entidade
	 * @return string
	 * @see self::$tabelas
	 */
	private function pegarTabela( $entidade ){
		$tabela = strtolower( array_key_exists( $entidade, self::$tabelas ) ? self::$tabelas[$entidade] : $entidade );
						
		$sql = sprintf(
			"select schemaname from pg_tables where tablename = '%s' and schemaname in ('seguranca','monitora','public') ",
			$tabela
		);	
			
		$schema = $this->bancodedados->pegaUm( $sql );
				
		if ( !$schema ) {
			return '';
		}		
		
		return $schema.'.'.$tabela; 
	}

	/**
	 * @param string $tabela
	 * @param array $registro
	 * @param string $operacao
	 * @return void
	 */
	private function registrar( $tabela, $registro, $operacao = null ){
		//dbg($operacao,1);
		if ( $operacao == null ) {
			return;
		}
		// permite apenas que os registros alterados sejam gravados do arquivo de log
		if ( $operacao != self::ALTERACAO ) {
			$registro = null;
		}
		$this->historico[$tabela][$operacao][] = $registro;
	}

}

?>