<?php

class ConsultaFinanceiroTraducao
{
	// TRADUÇÃO CÓDIGO
	/* 'nome' => array(
			'cod_campo' => '',
			'cod_alias' => '',
			'dsc_campo' => '',
			'dsc_alias' => '',
			'titulo' 	=> '',
			'tabela' 	=> '',
			'join' 		=> '',
			'join_dep' 	=> array()
	 ) */
	public $traducao = array();
	
	public function __construct(){
		$this->traducao = array(
							'ano'	=> array(
								'cod_campo' => 'orc.rofano',
								'cod_alias' => 'rofano',
								'dsc_campo' => 'orc.rofano',
								'dsc_alias' => 'rofano',
								'titulo'    => 'Ano',
								'tabela'    => 'financeiro.execucao',
								'join'		=> '',
								'join_dep'	=> array()
							),
							'uo'	=> array(
								'cod_campo' => 'u.unicod',
								'cod_alias' => 'unicod',
								'dsc_campo' => 'u.unidsc',
								'dsc_alias' => 'unidsc',
								'titulo'    => 'Unidade Orçamentária',
								'tabela'    => 'public.unidade',
								'join'		=> ' inner join unidade u on u.unicod = orc.unicod ',
								'join_dep'	=> array()
							),
							/*'ug'	=> array(
								'cod_campo' => 'orc.ungcod',
								'cod_alias' => 'ungcod',
								'dsc_campo' => 'orc.ungdsc',
								'dsc_alias' => 'ungdsc',
								'titulo'    => 'Unidade Gestora',
								'tabela'    => 'financeiro.execucao',
								'join'		=> '',
								'join_dep'	=> array()
							),
							*/
							'programa'	=> array(
								'cod_campo' => 'orc.prgcod',
								'cod_alias' => 'prgcod',
								'dsc_campo' => 'orc.prgdsc',
								'dsc_alias' => 'prgdsc',
								'titulo'    => 'Programa',
								'tabela'    => 'financeiro.execucao',
								'join'		=> '',
								'join_dep'	=> array()
							),
							'acacod'	=> array(
								'cod_campo' => 'orc.acacod',
								'cod_alias' => 'acacod',
								'dsc_campo' => 'orc.acadsc',
								'dsc_alias' => 'acadsc',
								'titulo'    => 'Ação',
								'tabela'    => 'financeiro.execucao',
								'join'		=> '',
								'join_dep'	=> array()
							),
							'fonte'	=> array(
								'cod_campo' => 'fr.codigo',
								'cod_alias' => 'codigo',
								'dsc_campo' => 'fr.descricao',
								'dsc_alias' => 'descricao',
								'titulo'    => 'Fonte de Recursos',
								'tabela'    => 'financeiro.fonterecursos',
					//			'join'		=> ' inner join financeiro.fonterecursos fr on fr.codigo = substr( orc.foncodsiafi, 3, 2) ',
								'join'		=> ' INNER JOIN financeiro.fonterecursos fr ON fr.codigo = substr( orc.foncod, 2, 2) ',
								'join_dep'	=> array()
							),
							'fontesof'	=> array(
								'cod_campo' => 'fs.foncod',
								'cod_alias' => 'foncod',
								'dsc_campo' => 'fs.fondsc',
								'dsc_alias' => 'fondsc',
								'titulo'    => 'Fonte SOF',
								'tabela'    => 'financeiro.fontesof',
								'join'		=> ' inner join financeiro.fontesof fs on fs.foncod = orc.foncod',
								'join_dep'	=> array()
							),
							'fontesiafi'	=> array(
								'cod_campo' => 'frs.foscod',
								'cod_alias' => 'foscod',
								'dsc_campo' => 'frs.fosdsc',
								'dsc_alias' => 'fosdsc',
								'titulo'    => 'Fonte Detalhada',
								'tabela'    => 'financeiro.fontesiafi',
								'join'		=> ' inner join financeiro.fontesiafi frs on frs.foscod = orc.foncodsiafi',
								'join_dep'	=> array()
							),
							'gnd'	=> array(
								'cod_campo' => 'gnd.gndcod',
								'cod_alias' => 'gndcod',
								'dsc_campo' => 'gnd.gnddsc',
								'dsc_alias' => 'gnddsc',
								'titulo'    => 'GND',
								'tabela'    => 'public.gnd',
								'join'		=> ' inner join gnd on gnd.gndcod = orc.gndcod  ',
								'join'		=> ' inner join gnd on gnd.gndcod = cast(orc.gndcod as integer)  ',
								'join_dep'	=> array()
							),
							'natureza'	=> array(
								'cod_campo' => 'cast(nd.ctecod as varchar) || nd.gndcod || nd.mapcod || nd.edpcod',
								'cod_alias' => 'natureza',
								'dsc_campo' => 'nd.ndpdsc',
								'dsc_alias' => 'ndpdsc',
								'titulo'    => 'Natureza de Despesa',
								'tabela'    => 'public.naturezadespesa',
								'join'		=> ' inner join public.naturezadespesa nd on cast(nd.ctecod as varchar) || nd.gndcod || nd.mapcod || nd.edpcod = cast(orc.ctecod as varchar) || cast(orc.gndcod as varchar) || orc.mapcod || orc.edpcod and nd.sbecod = \'00\' ',
								'join_dep'	=> array()
							),
							'localizador'	=> array(
								'cod_campo' => "loc.prgcod || '.' || loc.acacod || '.' || loc.unicod || '.' || loc.loccod",
								'cod_alias' => 'funcionalcod',
								'dsc_campo' => "loc.acadsc || ' ' || loc.sacdsc",
								'dsc_alias' => 'funcionaldsc',
								'titulo'    => 'Localizador',
								'tabela'    => '',
								'join'		=> ' inner join planejamento.acao loc on loc.acacod = orc.acacod and loc.prgcod = orc.prgcod and loc.unicod = orc.unicod and loc.loccod = orc.loccod and loc.prgano = orc.rofano and loc.acasnrap = \'f\' ',
								'join_dep'	=> array()
							),
							'mapcod'	=> array(
								'cod_campo' => 'ma.mapcod',
								'cod_alias' => 'mapcod',
								'dsc_campo' => 'ma.mapdsc',
								'dsc_alias' => 'mapdsc',
								'titulo'    => 'Modalidade de Aplicação',
								'tabela'    => 'public.modalidadeaplicacao',
								'join'		=> ' inner join modalidadeaplicacao ma on ma.mapcod = orc.mapcod ',
								'join_dep'	=> array()
							),
							'grf'	=> array(
								'cod_campo' => 'gf.grfid',
								'cod_alias' => 'grfid',
								'dsc_campo' => 'gf.grfdsc',
								'dsc_alias' => 'grfdsc',
								'titulo'    => 'Grupo de Fonte',
								'tabela'    => 'public.grupofonte',
								'join'		=> ' inner join grupofonte gf ON gf.grfid = cast( substring( orc.foncod, 1, 1 ) as integer )',
								'join_dep'	=> array()
							),
							'catecon'	=> array(
								'cod_campo' => 'cte.ctecod',
								'cod_alias' => 'ctecod',
								'dsc_campo' => 'cte.ctedsc',
								'dsc_alias' => 'ctedsc',
								'titulo'    => 'Categoria Econômica',
								'tabela'    => 'public.categoriaeconomica',
								'join'		=> ' inner join public.categoriaeconomica cte on cte.ctecod = orc.ctecod ',
								'join_dep'	=> array()
							),
							'elemento'	=> array(
								'cod_campo' => 'ele.edpcod',
								'cod_alias' => 'edpcod',
								'dsc_campo' => 'ele.edpdsc',
								'dsc_alias' => 'edpdsc',
								'titulo'    => 'Elemento de Despesa',
								'tabela'    => 'public.elementodespesa',
								'join'		=> ' inner join public.elementodespesa ele on ele.edpcod = orc.edpcod ',
								'join_dep'	=> array()
							),
								'sub-elemento'	=> array(
								'cod_campo' => 'orc.sbecod',
								'cod_alias' => 'sbecod',
								'dsc_campo' => 'orc.sbecod',
								'dsc_alias' => 'sbepdsc',
								'titulo'    => 'Sub-Elemento de Despesa',
								'tabela'    => 'financeiro.execucao',
								'join'		=> '' ,
								'join_dep'	=> array()
							),
							'plicod'	=> array(
								'cod_campo' => 'pli.plicod',
								'cod_alias' => 'plicod',
								'dsc_campo' => 'pli.plititulo',
								'dsc_alias' => 'plidsc',
								'titulo'    => 'Plano Interno',
								'tabela'    => 'monitora.pi_planointerno ',
								'join'		=> "inner join monitora.pi_planointerno pli on pli.plicod = orc.plicod and pli.plistatus = 'A' and pli.pliano = '{$_SESSION['exercicio']}'",
								'join_dep'	=> array()
							),
							'sbacod'	=> array(
								'cod_campo' => 'sba.sbacod',
								'cod_alias' => 'sbacod',
								'dsc_campo' => 'sba.sbatitulo',
								'dsc_alias' => 'sbadsc',
								'titulo'    => 'Subação',
								'tabela'    => 'monitora.pi_subacao',
								'join'		=> "inner join monitora.pi_subacao sba on sba.sbacod = substring( orc.plicod, 2, 4 ) and sba.sbastatus = 'A'",
								'join_dep'	=> array()
							)
						);
		
	}
	
	public function existe( $nome )
	{
		return array_key_exists( $nome, $this->traducao );
	}
	
	public function pegar( $nome, $tipo )
	{
		if ( isset( $this->traducao[$nome][$tipo] ) == false )
		{
			return '';
		}
		return $this->traducao[$nome][$tipo];
	}
	
	public function pegarAliasCodigo( $nome )
	{
		return self::pegar( $nome, 'cod_alias' );
	}
	
	public function pegarAliasDescricao( $nome )
	{
		return self::pegar( $nome, 'dsc_alias' );
	}
	
	public function pegarCampoCodigo( $nome )
	{
		return self::pegar( $nome, 'cod_campo' );
	}
	
	public function pegarCampoDescricao( $nome )
	{
		return self::pegar( $nome, 'dsc_campo' );
	}
	
	public function pegarTabela( $nome )
	{
		return self::pegar( $nome, 'tabela' );
	}
	
	public function pegarTitulo( $nome )
	{
		return self::pegar( $nome, 'titulo' );
	}
	
	public function pegarJoin( $nome )
	{
		return self::pegar( $nome, 'join' );
	}
	
	public function pegarJoinDependencia( $nome )
	{
		return self::pegar( $nome, 'join_dep' );
	}
	
	public function pegarAgrupadores()
	{
		$proibidos = array('ano');
		$retorno = array();
		
		foreach ( $this->traducao as $codigo => $dados )
		{
			if ( in_array( $codigo, $proibidos ) == true )
			{
				continue;
			}
			$novoItem = array(
				'codigo' => $codigo,
				'descricao' => $dados['titulo']
			);
			array_push( $retorno, $novoItem );
		}
		
		return $retorno;
	}
	
}


class ConsultaFinanceiro
{
	
	
	protected $tabela = 'financeiro.execucao';
	
	protected $agrupador = array();
	
	/**
	 * Enter description here...
	 *
	 * @var integer
	 */
	protected $escala = 1;
	
	protected $filtro = array();
	
	protected $filtroExcludente = array();
	
	protected $titulo = '';
	
	protected $filtrados = array();
	
	/**
	 * Adiciona um filtro a ser acrescentado à requisição.
	 * 
	 * @param string $campo
	 * @param string $valor
	 * @param boolean $excludente
	 * @return void
	 */
	public function adicionarFiltro( $campo, $valor, $excludente = false )
	{
		$oConsultaFinanceiroTraducao = new ConsultaFinanceiroTraducao();
		$campoCod = $oConsultaFinanceiroTraducao->pegarCampoCodigo( $campo );
		if ( array_key_exists( $campoCod, $this->filtro ) == false )
		{
			$this->filtro[$campoCod] = array();
		}
		if ( is_array( $valor ) == true )
		{
			foreach ( $valor as $item )
			{
				array_push( $this->filtro[$campoCod], $item );
			}
		}
		else
		{
			array_push( $this->filtro[$campoCod], $valor );
		}
		if ( $excludente == true )
		{
			$this->filtroExcludente[$campo] = $campoCod;
		}
		array_push( $this->filtrados, $campo );
	}
	
	public function adicionarAgrupador( $campo )
	{
		$oConsultaFinanceiroTraducao = new ConsultaFinanceiroTraducao();
		//if ( ConsultaFinanceiroTraducao::existe( $campo ) == false || in_array( $campo, $this->agrupador ) == true )
		if ( $oConsultaFinanceiroTraducao->existe( $campo ) == false || in_array( $campo, $this->agrupador ) == true )
		{
			return;
		}
		array_push( $this->agrupador, $campo );
		array_push( $this->filtrados, $campo );
	}
	
	protected function agrupar( &$itens, $nivelAgrupador )
	{
		if ( !$itens || count( $itens ) < 1 || array_key_exists( $nivelAgrupador, $this->agrupador ) == false )
		{
			return array();
		}
		$this->agruparPorCampo( $itens, $nivelAgrupador  );
		if ( $nivelAgrupador < count( $this->agrupador ) - 2 )
		{
			foreach ( $itens as &$item )
			{
				$this->agrupar( $item['itens'], $nivelAgrupador + 1 );
			}
		}
	}
	
	protected function agruparPorCampo( &$itens, $nivelAgrupador )
	{
		$oConsultaFinanceiroTraducao = new ConsultaFinanceiroTraducao();
		$agrupador = $this->agrupador[$nivelAgrupador];
		$aliasCod = $oConsultaFinanceiroTraducao->pegarAliasCodigo( $agrupador );
		$aliasDsc = $oConsultaFinanceiroTraducao->pegarAliasDescricao( $agrupador );
		//dbg( $itens, 1 );
		$resultado = array();
		foreach ( $itens as $item )
		{
			$chaveGrupo = $item[$aliasCod] . $item[$aliasDsc];
			
			// captura a descrição do item no nivel que está agrupado
			// os itens sempre terão os índices 'cod' e 'dsc' que conterão o código e descrição no agrupador atual
			if ( count( $item ) >= 3 )
			{
				$nivelSelecionado = $nivelAgrupador;
				if ( array_key_exists( $nivelAgrupador + 1, $this->agrupador ) == true )
				{
					$nivelSelecionado++;
				}
				$campoCod = $oConsultaFinanceiroTraducao->pegarAliasCodigo( $this->agrupador[$nivelSelecionado] );
				$campoDsc = $oConsultaFinanceiroTraducao->pegarAliasDescricao( $this->agrupador[$nivelSelecionado] );
				$item['cod'] = $item[$campoCod];
				$item['dsc'] = $item[$campoDsc];
			}
			
			// registra item na lista de resultado
			// caso só exista um nível de agrupador o array terá apenas um nível de profundidade
			// caso contrário os itens agrupador por índices de seus grupos
			if ( count( $this->agrupador ) > 1 )
			{
				if ( array_key_exists( $chaveGrupo, $resultado ) == false )
				{
					$resultado[$chaveGrupo] = array(
						'cod' => $item[$aliasCod],
						'dsc' => $item[$aliasDsc],
						'itens' => array()
					);
				}
				$chaveItem = $item['cod'].$item['dsc'];
				array_push( $resultado[$chaveGrupo]['itens'], $item );
			}
			else
			{
				$resultado[$chaveGrupo] = $item;
			}
		}
		$itens = $resultado;
	}
	
	public function alterarEscala( $escala )
	{
		$this->escala = abs( (integer) $escala );
	}
	
	public function alterarTitulo( $titulo )
	{
		$this->titulo = (string) $titulo;
	}
	
	public function pegarAgrupadores( $titulo = false )
	{
		if ( $titulo == false )
		{
			return $this->agrupador;
		}
		$agrupadores = array();
		foreach ( array_keys( $this->agrupador ) as $nivel )
		{
			array_push( $agrupadores, $this->pegarTituloAgrupador( $nivel ) );
		}
		return $agrupadores;
	}
	
	public function pegarEscala()
	{
		return $this->escala;
	}
	
	public function pegarFixos()
	{
		return
		' sum( coalesce( rofdot_ini, 0 ) ) / ' . $this->escala . ' as dotacao_inicial, ' .
		' sum( coalesce( rofautorizado, 0 ) ) / ' . $this->escala . ' as autorizado_valor, ' .
		' sum( coalesce( rof_valorpropostopi, 0 ) ) / ' . $this->escala . ' as valorproposto, ' .
		' sum( coalesce( rofempenhado, 0 ) ) / ' . $this->escala . ' as empenhado, ' .
		' sum( coalesce( rofliquidado_favorecido, 0 ) ) / ' . $this->escala . ' as liquidado, ' .
		' sum( coalesce( rofpago, 0 ) ) / ' . $this->escala . ' as pago ';		
	}
	
	public function pegarTitulo()
	{
		return $this->titulo;
	}
	
	public function pegarTituloAgrupador( $nivel = null )
	{
		$oConsultaFinanceiroTraducao = new ConsultaFinanceiroTraducao();
		$titulo = array();
		if ( $nivel === null )
		{
			foreach ( $this->agrupador as $agrupador )
			{
				array_push( $titulo, $oConsultaFinanceiroTraducao->pegarTitulo( $agrupador ) );
			}
		}
		else
		{
			array_push( $titulo, $oConsultaFinanceiroTraducao->pegarTitulo( $this->agrupador[$nivel] ) );
		}
		return implode( '/', $titulo );
	}
	
	/**
	 * Captura os campos selecionados. Caso o primeiro parâmetro seja verdadeiro
	 * é adicionado o alias do campos.
	 *
	 * @param boolean $alias
	 * @return string
	 */
	protected function pegarSelecionados( $alias )
	{
		$selecionados = array();
		$oConsultaFinanceiroTraducao = new ConsultaFinanceiroTraducao();
		foreach ( $this->agrupador as $campo )
		{
			$codigo = $oConsultaFinanceiroTraducao->pegarCampoCodigo( $campo );
			$descricao = $oConsultaFinanceiroTraducao->pegarCampoDescricao( $campo );
			if ( $alias )
			{
				$codigo .= ' as ' . $oConsultaFinanceiroTraducao->pegarAliasCodigo( $campo );
				$descricao .= ' as ' . $oConsultaFinanceiroTraducao->pegarAliasDescricao( $campo );
			}
			array_push( $selecionados, $codigo );
			array_push( $selecionados, $descricao );
		}

		return $selecionados;
	}
	
	public function pegarValoresFiltro( $campo )
	{
		$oConsultaFinanceiroTraducao = new ConsultaFinanceiroTraducao();
		$nomeCampo = $oConsultaFinanceiroTraducao->pegarCampoCodigo( $campo );
		if ( array_key_exists( $nomeCampo, $this->filtro ) == false )
		{
			return array();
		}
		return $this->filtro[$nomeCampo];
	}
	
	protected function montaFiltroSql()
	{
		$where = '';
		foreach ( $this->filtro as $campo => $valores )
		{
			$quantidadeValores = count( $valores );
			$excludente = in_array( $campo, $this->filtroExcludente );
			$valores = '\'' . implode( '\',\'', $valores ) . '\'';
			if ( $quantidadeValores > 1 )
			{
				$operador = $excludente ? ' not in ' : ' in ' ;
				$valores = ' ( ' . $valores . ' ) ';
			}
			else
			{
				$operador = $excludente ? ' != ' : ' = ' ;
			}
			$where .= ' and ' . $campo . $operador . $valores ;
		}
		if ( $where != '' )
		{
			$where = ' where ' . substr( $where, 5 );
		}
		return $where;
	}
	
	protected function pegarCamposFiltrados()
	{
		//ver($this->filtrados,d);
		return array_unique( $this->filtrados );
	}
	
	/**
	 * Monta requisição a ser executada para a realização da consulta. O texto
	 * retornado retorna os campos agrupados pelo agrupador e sumarizador, após
	 * a reqlização da requisição com o selecte retornado é necessário chamar o
	 * método ConsultaFinanceiro::agrupar()
	 * Caso o primeiro parâmetro seja falso é retornado somente o esqueleto da
	 * query, onde não há campos selecionados nem agrupamento e ordenação.
	 * 
	 * @param string $campos
	 * @return string
	 */
	public function montaRequisicao( $campos = true )
	{
		$oConsultaFinanceiroTraducao = new ConsultaFinanceiroTraducao();
		// verifica se há campos a ser selecionados
		$selecionados = $this->pegarSelecionados( true );
		
		//ver($campos, $selecionados,count( $this->agrupador ) ,d);
		if ( count( $selecionados ) < 1 || count( $this->agrupador ) < 1 )
		{
			return '';
		}

		// define campos selecionados, agrupamento e ordenação
		$select = '';
		$join  = '';
		$groupby = '';
		$orderby = '';
		if ( $campos == true )
		{
			$select = implode( ', ', $selecionados );
			$camposSemAlias = implode( ', ', $this->pegarSelecionados( false ) );
			$groupby = ' group by ' . $camposSemAlias;
			$orderby = ' order by ' . $camposSemAlias;
			$selecionados = array_merge( $this->agrupador, array_keys( $this->filtro ), array_keys( $this->filtroExcludente ) );
			$join = array();
			foreach ( $this->pegarCamposFiltrados() as $campo )
			{
				foreach ( $oConsultaFinanceiroTraducao->pegarJoinDependencia( $campo ) as $campoJoinDep )
				{
					$join[$campoJoinDep] = $oConsultaFinanceiroTraducao->pegarJoin( $campoJoinDep );
				}
				$join[$campo] = $oConsultaFinanceiroTraducao->pegarJoin( $campo );
			}
			$join = implode( ' ', $join );
		}

		// define filtro
		$where = $this->montaFiltroSql();
		
		// define escala dos valores
		$escala = '';
		if ( $this->escala > 1 )
		{
			$escala = ' / ' . $this->escala;
		}
		
		$fixos = $this->pegarFixos();
		$fixos = empty( $fixos ) ? '' : ', '. $fixos;

		// monta requisição
		$sql =
			' select ' .
				$select .
				$fixos .
			//' from financeiro.execucao orc ' .
			' from ' . $this->tabela . ' orc ' .
			$join . $where . $groupby . $orderby ;//.' limit 5';
//		ver($sql,d);

			return $sql;
	}
	
	/**
	 * Realiza consulta com os dados presentes na classe.
	 * 
	 * @return string[]
	 */
	public function consultar()
	{
		global $db;
		
		# [performace] Se não encontrar o agrupador/filtro de mês utiliza a tabela execucao1
		/*if ( false === array_search( 'mes', $this->pegarCamposFiltrados() ) ){
			$this->tabela = 'financeiro.execucao1';
		}
		*/
		$sql = $this->montaRequisicao();
		
		if ( $sql == '' )
		{
			return array();
		}
		$itens = $db->carregar( $sql );
		$this->agrupar( $itens, 0 );
		return array( 'itens' => $itens );
	}
	
	public function filtroExcludente( $campo )
	{
		return array_key_exists( $campo, $this->filtroExcludente );
	}

}

class ConsultaFinanceiroOrcamento extends ConsultaFinanceiro
{

	protected $tabela = 'financeiro.execucaomec ';
	
	static protected $selecionados = array(
		'orc.gstcod' => 'gstcod',
		'orc.frscod' => 'frscod'
	);

	protected function pegarSelecionados( $alias )
	{
		$selecionados = array();
		
		foreach ( ConsultaFinanceiroOrcamento::$selecionados as $campo => $apelido )
		{
			if ( $alias )
			{
				$campo .= ' as '. $apelido;
			}
			array_push( $selecionados, $campo );
		}
		return array_merge( parent::pegarSelecionados( $alias ), $selecionados );
	}

	public function pegarFixos()
	{

		if( $_REQUEST['rap'] == null ) 
		{
			return
			' sum( coalesce( execredrecdestadmdir , 0 ) + coalesce( execredrecdestadmind , 0 ) + coalesce( execredrecprov , 0 ) ) / ' . $this->escala . ' as credito_recebido, ' .
			' sum( coalesce( exeempemit, 0 ) ) / ' . $this->escala . ' as empenhos_emitidos, ' .
			' sum( coalesce( exeempliqpagosapagar, 0 ) ) / ' . $this->escala . ' as empenhos_liquidados, ' .
			' sum( coalesce( exevalrepasreceb, 0 ) ) as repasse_recebido, ' . 
			' sum( coalesce( exevalliqpagos, 0 ) ) / ' . $this->escala . ' as valores_pagos, ' .
			' sum( coalesce( exeempliqpagosapagar, 0 ) - coalesce( exevalliqpagos, 0 ) ) / ' . $this->escala . ' as liquidado_pagar, ' .
			' sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) / ' . $this->escala . ' as saldo_financeiro, ' .
			' sum( coalesce( exevallimsaque, 0 ) ) / ' . $this->escala . ' as limite_saque, ' .
			' case when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) < 0 then ' .
			' ( sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) )  / ' . $this->escala . ' ) ' .
			' else 0 end as valor_recompor, ' .
			' case when sum( coalesce( exevallimsaque, 0) ) < sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) then ' .
			' ( sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) - sum( coalesce( exevallimsaque, 0) ) / ' . $this->escala . ') ' .
			' else 0 end as valor_detalhar, ' .
	
			' case sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) ' .
			
			' when 0 then ' .
			'	case when sum( coalesce( exevallimsaque, 0 ) ) > 0 then sum( coalesce( exevallimsaque, 0 ) ) / ' . $this->escala . ' else 0 end ' .
	
			' when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) then ' .
			'	case when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) < 0 then ' .
			'		( sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) / ' . $this->escala . ')  ' .
			'	end ' .
			' when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) then ' .
			'	case when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) > 0 then ' .
			'			case when sum( coalesce( exevallimsaque, 0 ) ) = sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) then 0 end ' .
			'	end ' .
			' when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) then ' .
			'	case when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) > 0 then ' .
			'			case when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) < sum( coalesce( exevallimsaque, 0 ) ) then sum( coalesce( exevallimsaque, 0 ) ) - sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) / ' . $this->escala . ' end ' .
			'	end ' .
	
			' when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) then ' .
			'	case when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) > 0 then ' .
			'			case when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) > sum( coalesce( exevallimsaque, 0 ) ) then sum( coalesce( exevallimsaque, 0 ) ) - sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) / ' . $this->escala . ' end ' .
			'	end ' .
			
			' end as valore_desdetalhar, ' .
			
			' case when sum( coalesce( exeempliqpagosapagar, 0 ) - coalesce( exevalrepasreceb, 0 ) + coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) < 0 then ' .
			'		sum( coalesce( exeempliqpagosapagar, 0 ) - coalesce( exevalrepasreceb, 0 ) + coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) / ' . $this->escala . ' else 0 end as repassado_excesso, ' .
			
			' sum( coalesce( exeempliqpagosapagar, 0 ) ) / ' . $this->escala . ' as valor_repassar, ' .
			' sum( coalesce( 0, 0 ) ) / ' . $this->escala . ' as valor_proposto ';
			}
		else
		{
					
			return
			' sum( coalesce( exerapliquidado , 0 ) ) / ' . $this->escala . ' as processado_mes0, ' .
			' sum( coalesce( exerapaliqbloq, 0 ) ) / ' . $this->escala . ' as nao_processado_mes0, ' .
			' sum( coalesce( exerapliquidado, 0 ) + coalesce( exerapaliqbloq, 0 ) ) / ' . $this->escala . ' as total_mes0, ' .
			' sum( coalesce( exerapliquidado , 0 ) ) / ' . $this->escala . ' as processado_atual, ' .
			' sum( coalesce( exerapaliqbloq, 0 ) ) / ' . $this->escala . ' as nao_processado_atual, ' .
			' sum( coalesce( exerapliquidado, 0 ) + coalesce( exerapaliqbloq, 0 ) ) / ' . $this->escala . ' as total_atual, ' .
			' sum( coalesce( exerappago, 0 ) ) as pago, ' . 
			' sum( coalesce( exerapliquidado, 0 ) + coalesce( exerapaliqbloq, 0 ) ) / ' . $this->escala . ' as atual_total, ' .
			' sum( coalesce( exerapliqexercantrapprocapagar, 0 ) ) / ' . $this->escala . ' as processado_apagar, ' .
			' sum( coalesce( exeraptransfreceb, 0 ) ) / ' . $this->escala . ' as nao_processado_apagar, ' .
			' sum( coalesce( exerapaliq, 0 ) ) / ' . $this->escala . ' as total_apagar, ' .
			' sum( coalesce( exeraptransf, 0 ) ) / ' . $this->escala . ' as valora_repassar '; 

		}
		
	}

}

?>