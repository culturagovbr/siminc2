<?php

function removerExecutor()
{
	global $db;
	$sql = "delete from pde.monitorametaentidade where mnmid =".$_REQUEST['mnmid']." and tpvid =1";
	$db->executar($sql);

	
	echo "<script>
		alert('Executor removido com sucesso');
	</script>";
}

function removerValidador()
{
	global $db;
	$sql = "delete from pde.monitorametaentidade where mnmid =".$_REQUEST['mnmid']." and tpvid =2";
	$db->executar($sql);


	echo "<script>
		alert('Validador removido com sucesso');
	</script>";
}


// ATIVIDADE ///////////////////////////////////////////////////////////////////
function atividade_inserir( $atividade, $titulo ){
	global $db;
	$sql = sprintf(
		"insert into pde.atividade (
			atiidpai, atidescricao, atiordem, _atiprojeto, acaid, usucpfcadastro
		) values (
			%d,
			'%s',
			( select coalesce( max(atiordem), 0 ) + 1 from pde.atividade where atistatus = 'A' and atiidpai = %d ),
			( select _atiprojeto from pde.atividade where atiid = %d ),
			( select acaid from pde.atividade where atiid = %d ),
			'{$_SESSION['usucpf']}'
		) returning atiid",
		$atividade,
		$titulo,
		$atividade,
		$atividade,
		$atividade # adaptação necessária para que o módulo de monitoramento funcione
	);
	
	$atiid = $db->pegaUm($sql);
	
		// verifica se a atividade pai possui subacao, caso sim, a filha devera ser a mesma subacao
	$sql = "SELECT sbaid FROM monitora.pi_subacaoatividade WHERE atiid='".$atividade."'";
	$sbaids = $db->carregar($sql);
	
	if($sbaids[0]) {
		foreach($sbaids as $sbaid) {
			$sql = "INSERT INTO monitora.pi_subacaoatividade(sbaid, atiid) VALUES ('".$sbaid['sbaid']."', '".$atiid."');";
			$db->executar($sql);
		}
	}
	
	$db->commit();	
	return true;
}

function atividade_listar( $atividade, $profundidade = 0, $situacao = array(), $usuario = null, $perfil = array() ){
	global $db;
		
	// captura as opções
	$atividade    = (integer) $atividade;
	$profundidade = (integer) $profundidade;
	$situacao     = (array) $situacao;
	$usuario      = (string) $usuario;
	
	// identifica a atividade e o projeto
	$atividade = $atividade ? $atividade : PROJETO;
	$projeto   = (integer) $db->pegaUm( "select _atiprojeto from pde.atividade where atiid = $atividade" );
	if ( $projeto != PROJETO && $_SESSION['sisid'] != 10) {
		$atividade = (integer) PROJETO;
		$projeto   = (integer) PROJETO;
	}
	//dbg($projeto);
	// identifica o nó de origem
	$sql_filhas = "";
	if ( $atividade ) {
		$numero = $db->pegaUm( "select _atinumero from pde.atividade where atiid = $atividade" );
		if ( $numero ) {
			//$sql_filhas = " and ( a._atinumero like '$numero.%' ) ";
			$sql_filhas = " and ( substr( a._atinumero, 0, " . ( strlen( $numero ) + 2 ) . " ) = '" . $numero .  ".' ) ";
		}
	}
	
	// restringe a profundidade
	$sql_profundidade = "";
	if ( $profundidade > 0 ) {
		$sql_profundidade = " and ( a._atiprofundidade <= $profundidade ) ";
	}
	
	// restringe as situações
	$sql_situacao = "";
	if ( count( $situacao ) > 0 ) {
		$sql_situacao = " and a.esaid in (". implode( ',', $situacao ) .") ";
		$sql_situacao_restricao = " and a.esaid not in (". implode( ',', $situacao ) .") ";
	}
	
	// restringe por responsabilidade
	$sql_responsabilidade = "";
	if ( $usuario ) {
		$sql_perfil = "";
		if ( !empty( $perfil ) ) {
			$sql_perfil = " and ur.pflcod in ( ". implode( ",", $perfil ) ." ) ";
		}
		$sql = sprintf(
			"select a._atinumero
			from seguranca.usuario u
			inner join pde.usuarioresponsabilidade ur on ur.usucpf = u.usucpf %s
			inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
			inner join pde.atividade a on a.atiid = ur.atiid
			where
				u.suscod = 'A'
				and u.usucpf = '%s'
				and ur.rpustatus = 'A'
				and a._atiprojeto = %d
				and a.atiid != a._atiprojeto
				and a.atistatus = 'A'
				%s %s %s",
			$sql_perfil,
			$usuario,
			$projeto,
			$sql_filhas,
			$sql_profundidade,
			$sql_situacao
		);
		$numeros = array();
		foreach( (array) $db->carregar( $sql ) as $responsabilidade ) {
			$rastro = array();
			foreach( explode( ".", $responsabilidade['_atinumero'] ) as $item ){
				array_push( $rastro, sprintf( "%04d", $item ) );
			}
			$numero = implode( $rastro );
			//array_push( $numeros, " a._atiordem like '" . $numero ."%' " );
			array_push( $numeros, " substr( a._atinumero, 0, " . ( strlen( $numero ) + 1 ) . " ) = '" . $numero . "' " );
			foreach ( $rastro as $chave => $ordem ) {
				if ( $chave == 0 ) continue;
				$numero = implode( "", array_slice( $rastro, 0, $chave ) );
				array_push( $numeros, " a._atiordem = '" . $numero ."' " );
			}
		}
		$numeros = array_unique( $numeros );
		$sql_responsabilidade = " and ( ". implode( ' or ', $numeros ) ." ) ";
	}
	
	$sql_restricao = "";
	if ( $sql_situacao_restricao ) {
		$sql = sprintf(
			"select a._atinumero
			from pde.atividade a
			where
				a._atiprojeto = %d
				and a.atiid != a._atiprojeto
				and a.atistatus = 'A'
				%s %s %s",
			$projeto,
			$sql_filhas,
			$sql_profundidade,
			$sql_situacao_restricao
		);
		$restricao = array();
		$atinumeros = array();
		foreach( (array) $db->carregar( $sql ) as $atividade ) {
			if ( !$atividade['_atinumero'] ) {
				break;
			}
			array_push( $atinumeros, $atividade['_atinumero'] );
		}
		$numerosFinais = array();
		foreach ( array_unique( $atinumeros ) as $atinumero ) {
			$tamanho = strlen($atinumero );
			if ( !array_key_exists( $tamanho, $numerosFinais ) )
			{
				$numerosFinais[$tamanho] = array();
			}
			array_push( $numerosFinais[$tamanho], $atinumero . "." );
		}
		foreach ( $numerosFinais as $tamanho => $valores )
		{
			array_push( $restricao, " substr( a._atinumero, 0, " . ( $tamanho + 2 ) . " ) not in ( '" . implode( "','", $valores ) . "' ) " );
		}
		if ( count( $restricao ) > 0 ) {
			$sql_restricao = " and ( ". implode( ' and ', $restricao ) . " ) ";
		}
	}
	
	$sql = sprintf(
		"select
			a.atiid,
			a.aticodigo,
			a.atidescricao,
			--a.atidetalhamento,
			--a.atimeta,
			--a.atiinterface,
			a.atidatainicio,
			a.atidatafim,
			--a.atisndatafixa,
			a.atistatus,
			a.atiordem,
			--a.atinumeracao,
			--a.atiidpredecessora,
			a.atiidpai,
			--a.usucpf,
			--a.tatcod,
			a.esaid,
			a.atidataconclusao,
			
			CASE 
			WHEN (a.atitipoandamento = 'p' OR a.atitipoandamento IS NULL) THEN a.atiporcentoexec
			WHEN (a.atitipoandamento = 'q') THEN ( ( coalesce(a.atiquantidadeexec, 0) / a.atimetanumerica ) * 100 )
			END as atiporcentoexec,
						
			a.atitipoandamento,
			a.atiquantidadeexec,
			a.atimetanumerica,
			
			a._atiprojeto,
			--a._atiordem,
			a._atinumero,
			a._atiprofundidade,
			a._atiirmaos,
			a._atifilhos,
			ea.esadescricao,
			u.usunome,
			u.usunomeguerra,
			u.usucpf,
			u.usuemail,
			u.usufoneddd,
			u.usufonenum,
			uni.unidsc,
			ug.ungdsc,
			aga.graid,
			coalesce( restricoes, 0 ) as qtdrestricoes,
			coalesce( anexos, 0 ) as qtdanexos,
			a._atiprofundidade as profundidade,
			a._atinumero as numero,
			a._atifilhos as filhos,
			a.atitipoandamento, 
			a.atimetanumerica,
			a.atiquantidadeexec,
			a.usucpfcadastro,
			u2.usunome as usunomecadastro
		from pde.atividade a
		inner join pde.estadoatividade ea on
			ea.esaid = a.esaid
		left join pde.usuarioresponsabilidade ur on
			ur.atiid = a.atiid and
			ur.rpustatus = 'A' and
			ur.pflcod = %d
		left join pde.atividadegrupoatividade aga on
			aga.atiid = a.atiid and aga.graid=1
		left join seguranca.perfilusuario pu on
			pu.pflcod = ur.pflcod and
			pu.usucpf = ur.usucpf
		left join seguranca.usuario u on
			u.usucpf = pu.usucpf and
			u.suscod = 'A'
		left join seguranca.usuario u2 on
			u2.usucpf = a.usucpfcadastro
		left join public.unidade uni on
			uni.unicod = u.unicod and
			uni.unitpocod = 'U' and
			uni.unistatus = 'A'
		left join public.unidadegestora ug on
			ug.ungcod = u.ungcod and
			ug.ungstatus = 'A'
		left join (
			select atiid, count(*) as restricoes
			from pde.observacaoatividade
			where obsstatus = 'A' and obssolucao = false
			group by atiid ) restricao on
				restricao.atiid = a.atiid
		left join (
			select atiid, count(*) as anexos
			from pde.anexoatividade
			where anestatus = 'A'
			group by atiid ) anexo on
				anexo.atiid = a.atiid
		where
			a._atiprojeto = %d
			and a.atiid != a._atiprojeto
			and a.atistatus = 'A'
			%s %s %s %s %s 
		order by _atiordem",
		PERFIL_GERENTE,
		$projeto,
		$sql_filhas,
		$sql_profundidade,
		$sql_situacao,
		$sql_restricao,
		$sql_responsabilidade
	);
	//dbg( $sql, 1 );
	return $db->carregar( $sql );
}

function atividade_excluir( $atiid ){
	global $db;
	// captura as informações da atividade a ser excluída
	$sql = sprintf( "select * from pde.atividade a where a.atiid = %s and a.atistatus = 'A'", $atiid );
	$atividade = $db->pegaLinha( $sql );
	if ( !$atividade ) {
		return false;
	}
	// exclui a atividade
	$sql = sprintf( "update pde.atividade set atistatus = 'I' where atiid = %s", $atividade['atiid'] );
	if ( !$db->executar( $sql ) ) {
		return false;
	}
	// reordena as atividades que tem o mesmo pai
	$sql = sprintf(
		"update pde.atividade set atiordem = atiordem - 1 where atiidpai = %s and atiordem > %s and atistatus = 'A'",
		$atividade['atiidpai'],
		$atividade['atiordem']
	);
	if ( !$db->executar( $sql ) ) {
		return false;
	}
	return true;
}

function atividade_pegar( $atividade ){
	global $db;
	$sql = sprintf(
		"select a.*, e.esadescricao, sub.numero, sub.projeto
		from pde.atividade a
		left join pde.estadoatividade e on e.esaid = a.esaid
		inner join pde.f_dadosatividade( %d ) as sub on sub.atiid = a.atiid
		where a.atiid = %d and atistatus = 'A'",
		(integer) $atividade,
		(integer) $atividade
	);
	$registro = $db->pegaLinha( $sql );
	if ( is_array( $registro ) ) {
		return $registro;
	}
	return null;
}

function atividade_pegar_projeto( $atividade ){
	global $db;
	$sql = sprintf( "select _atiprojeto from pde.atividade where atiid = %d", $atividade );
	return $db->pegaUm( $sql );
}

/**
 * Verifica se o andamento da atividade está corretamente cadastrado.
 * @param $atiid integer
 * @return boolean
 */
function verificaCadastroAndamentoAtividade($atiid)
{
	/*** Instância global do objeto de conexão do banco ***/
	global $db;
	/*** Variável de retorno ***/
	$retorno = false;
	
	/*** Recupera o tipo de andamento ***/
	$sql 				= "SELECT atitipoandamento FROM pde.atividade WHERE atiid = " . $atiid;
	$atitipoandamento 	= $db->pegaUm($sql);
	
	/*** Se o tipo de andamento estiver corretamento cadastrado ***/
	if( $atitipoandamento )
	{
		/*** Se for percentual ***/
		if( $atitipoandamento == 'p' )
		{
			/*** Recupera o andamento ***/
			$sql 		= "SELECT atiporcentoexec FROM pde.atividade WHERE atiid = " . $atiid;
			$andamento	= $db->pegaUm($sql);
		}
		/*** Se for quantitativo ***/
		if( $atitipoandamento == 'q' )
		{
			/*** Recupera o andamento ***/
			$sql 		= "SELECT atiquantidadeexec FROM pde.atividade WHERE atiid = " . $atiid;
			$andamento	= $db->pegaUm($sql);
		}
		
		/*** Se o andamento estiver corretamente cadastrado, retorna true ***/
		$retorno = ( $andamento ) ? true : false;
	}
	
	return $retorno;
}

/**
 * Retorna as atividades que estão acima da atividade indicada exceto o projeto,
 * que é a atividade raiz. 
 * 
 * @return array
 */
function atividade_pegar_rastro( $numero ){
	global $db;
	$numero_original = $numero;
	$condicao = array();
	array_push( $condicao, " a._atinumero = '$numero' " );
	while( ( $posicao = strrpos( $numero, '.' ) ) !== false ) {
		$numero = substr( $numero, 0, $posicao );
		array_push( $condicao, " a._atinumero = '$numero' " );
	}
	if ( count( $condicao ) == 0 ) {
		return array();
	}
	$sql = sprintf(
		"select
			a._atinumero as numero,
			a._atiprofundidade as profundidade,
			a._atiirmaos as irmaos,
			a._atifilhos as filhos,
			a.atidescricao,
			a.atiid,
			a.atiidpai,
			a.atidatainicio,
			a.atidatafim,
			a.atiordem,
			a.atiporcentoexec,
			a.esaid,
			ea.esadescricao,
			u.usunome,
			u.usunomeguerra,
			u.usucpf,
			uni.unidsc,
			ug.ungdsc
		from pde.atividade a
			left join pde.estadoatividade ea on
				ea.esaid = a.esaid
			left join pde.usuarioresponsabilidade ur on
				ur.atiid = a.atiid and ur.rpustatus = 'A' and ur.pflcod = %d 
			left join seguranca.usuario u on
				u.usucpf = ur.usucpf and u.usustatus = 'A'
			left join public.unidade uni on
				uni.unicod = u.unicod and
				uni.unitpocod = 'U' and
				uni.unistatus = 'A'
			left join public.unidadegestora ug on
				ug.ungcod = u.ungcod and
				ug.ungstatus = 'A'
		where
			a._atiprojeto = %d and
			a.atiidpai is not null and
			a.atistatus = 'A' and
			( %s )
		order by a._atiordem",
		PERFIL_GERENTE,
		PROJETO,
		implode( ' or ', $condicao )
	);
	$rastro = $db->carregar( $sql );
	return $rastro && count( $rastro ) == substr_count( $numero_original, "." ) + 1 ? $rastro : array();
}

function atividade_pegar_filhas( $projeto, $atividade = null, $usuario = null, $profundidade = null ){
	global $db;
	$profundidade = (string) $profundidade;
	if ( $profundidade != '' ) {
		$profundidade = (integer) $profundidade;
		if ( $atividade ) {
			$sql = "select profundidade from pde.f_dadosatividade( " . $atividade . " )";
			$profundidade = $db->pegaUm( $sql ) + $profundidade;
		} else {
			$profundidade++;
		}
		$condicao_profundidade = " and la.profundidade <= " . $profundidade;
	} else {
		$condicao_profundidade = "";
		$profundidade = null;
	}
	if ( $usuario ) {
		return atividade_pegar_sob_responsabilidade( $projeto, $usuario, $profundidade );
	}
	if ( $atividade ) {
		$sql = sprintf(
			"select
				la.numero,
				la.profundidade,
				la.irmaos,
				la.filhos,
				a.atidescricao,
				a.atiid,
				a.atiidpai,
				a.atidatainicio,
				a.atidatafim,
				a.atidataconclusao,
				a.atiordem,
				a.atiporcentoexec,
				a.esaid,
				ea.esadescricao,
				u.usunome,
				u.usunomeguerra,
				u.usucpf,
				u.usuemail,
				u.usufoneddd,
				u.usufonenum,
				uni.unidsc,
				ug.ungdsc,
				coalesce(qtdrestricoes,0) as qtdrestricoes,
				coalesce(qtdanexos,0) as qtdanexos
			from pde.f_dadosatividade( %d ) as da
				inner join pde.f_dadostodasatividades() as la on
					la.numero like da.numero || '.%%' or
					la.numero = da.numero 
				inner join pde.atividade a on
					a.atiid = la.atiid
				left join pde.estadoatividade ea on
					ea.esaid = a.esaid
				left join pde.usuarioresponsabilidade ur on
					ur.atiid = la.atiid and
					ur.rpustatus = 'A' and
					ur.pflcod = %d
				left join seguranca.perfilusuario pu on
					pu.pflcod = ur.pflcod and
					pu.usucpf = ur.usucpf
				left join seguranca.usuario u on
					u.usucpf = pu.usucpf and
					u.suscod = 'A'
				left join public.unidade uni on
					uni.unicod = u.unicod and
					uni.unitpocod = 'U' and
					uni.unistatus = 'A'
				left join public.unidadegestora ug on
					ug.ungcod = u.ungcod and
					ug.ungstatus = 'A'
				left join (
					select atiid, count(*) as qtdrestricoes
					from pde.observacaoatividade
					where obsstatus = 'A' and obssolucao = false
					group by atiid ) restricao on restricao.atiid = a.atiid
				left join (
					select atiid, count(*) as qtdanexos
					from pde.anexoatividade
					where anestatus = 'A'
					group by atiid ) anexo on anexo.atiid = a.atiid
			where
				la.projeto = %d and
				la.projeto != la.atiid and
				a.atistatus = 'A'
				%s
			order
				by la.ordem",
			$atividade,
			PERFIL_GERENTE,
			$projeto,
			$condicao_profundidade
		);
	} else {
		$sql = sprintf(
			"
			select
				la.numero,
				la.profundidade,
				la.irmaos,
				la.filhos,
				a.atidescricao,
				a.atiid,
				a.atiidpai,
				a.atidatainicio,
				a.atidatafim,
				a.atidataconclusao,
				a.atiordem,
				a.atiporcentoexec,
				a.esaid,
				ea.esadescricao,
				u.usunome,
				u.usunomeguerra,
				u.usucpf,
				u.usuemail,
				u.usufoneddd,
				u.usufonenum,
				uni.unidsc,
				ug.ungdsc,
				coalesce(qtdrestricoes,0) as qtdrestricoes,
				coalesce(qtdanexos,0) as qtdanexos
			from pde.f_dadostodasatividades() la
				inner join pde.atividade a on
					a.atiid = la.atiid
				left join pde.estadoatividade ea on
					ea.esaid = a.esaid
				left join pde.usuarioresponsabilidade ur on
					ur.atiid = la.atiid and
					ur.rpustatus = 'A' and
					ur.pflcod = %d
				left join seguranca.perfilusuario pu on
					pu.pflcod = ur.pflcod and
					pu.usucpf = ur.usucpf
				left join seguranca.usuario u on
					u.usucpf = pu.usucpf and
					u.suscod = 'A'
				left join public.unidade uni on
					uni.unicod = u.unicod and
					uni.unitpocod = 'U' and
					uni.unistatus = 'A'
				left join public.unidadegestora ug on
					ug.ungcod = u.ungcod and
					ug.ungstatus = 'A'
				left join (
					select atiid, count(*) as qtdrestricoes
					from pde.observacaoatividade
					where obsstatus = 'A' and obssolucao = false
					group by atiid ) restricao on restricao.atiid = a.atiid
				left join (
					select atiid, count(*) as qtdanexos
					from pde.anexoatividade
					where anestatus = 'A'
					group by atiid ) anexo on anexo.atiid = a.atiid
			where
				la.projeto = %d and
				la.projeto != la.atiid and
				a.atistatus = 'A'
				%s
			order by
				la.ordem",
			PERFIL_GERENTE,
			$projeto,
			$condicao_profundidade
		);
	}
	$lista = $db->carregar( $sql );
	if ( is_array( $lista ) ) {
		return $lista;
	}
	return array();
}

function atividade_pegar_sob_responsabilidade( $projeto, $usuario, $profundidade = null ){
	global $db;
	if ( $profundidade !== null ) {
		$condicao_profundidade = " and folha.profundidade <= " . $profundidade;
	} else {
		$condicao_profundidade = "";
	}
	$sql = sprintf(
		"select
			folha.numero,
			folha.profundidade,
			folha.irmaos,
			folha.filhos,
			a.atidescricao,
			a.atiid,
			a.atiidpai,
			a.atidatainicio,
			a.atidatafim,
			a.atidataconclusao,
			a.atiordem,
			a.atiporcentoexec,
			a.esaid,
			ea.esadescricao,
			u.usunome,
			u.usunomeguerra,
			u.usucpf,
			u.usuemail,
			u.usufoneddd,
			u.usufonenum,
			uni.unidsc,
			ug.ungdsc,
			coalesce(qtdrestricoes,0) as qtdrestricoes,
			coalesce(qtdanexos,0) as qtdanexos
		from pde.usuarioresponsabilidade ur
			inner join pde.f_dadostodasatividades() as raiz on
				raiz.atiid = ur.atiid
			inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
			inner join pde.f_dadostodasatividades() as folha on
				folha.atiid = raiz.atiid or folha.numero like raiz.numero || '.%%'
			inner join pde.atividade a on
				a.atiid = folha.atiid
			left join pde.estadoatividade ea on
				ea.esaid = a.esaid
			left join pde.usuarioresponsabilidade ur2 on
				ur2.atiid = folha.atiid and ur2.rpustatus = 'A' and ur2.pflcod = %d 
			left join seguranca.perfilusuario pu2 on pu2.pflcod = ur2.pflcod and pu2.usucpf = ur2.usucpf
				left join seguranca.usuario u on
					u.usucpf = pu2.usucpf and
					u.suscod = 'A'
			left join public.unidade uni on
				uni.unicod = u.unicod and
				uni.unitpocod = 'U' and
				uni.unistatus = 'A'
			left join public.unidadegestora ug on
				ug.ungcod = u.ungcod and
				ug.ungstatus = 'A'
			left join (
				select atiid, count(*) as qtdrestricoes
				from pde.observacaoatividade
				where obsstatus = 'A' and obssolucao = false
				group by atiid ) restricao on restricao.atiid = a.atiid
			left join (
				select atiid, count(*) as qtdanexos
				from pde.anexoatividade
				where anestatus = 'A'
				group by atiid ) anexo on anexo.atiid = a.atiid
		where
			ur.rpustatus = 'A' and
			ur.usucpf = '%s' and
			folha.projeto = %d and
			folha.projeto != folha.atiid and
			raiz.projeto = %d
			%s
		order by folha.ordem",
		PERFIL_GERENTE,
		$usuario,
		$projeto,
		$projeto,
		$condicao_profundidade
	);

	$lista = $db->carregar( $sql );
	if ( !is_array( $lista ) ) {
		return array();
	}
	$lista_final = array();
	foreach ( $lista as $item ) {
		if ( array_key_exists( $item['numero'], $lista_final ) ) {
			continue;
		}
		// adiciona pais (caso o pai não esteja na lista)
		$numero_pai = substr( $item['numero'], 0, strrpos( $item['numero'], '.' ) );
		if ( $numero_pai && !array_key_exists( $numero_pai, $lista_final ) ) {
			$rastro_pai = atividade_pegar_rastro( $item['numero'] );
			foreach ( $rastro_pai as $item_pai ) {
				if ( !array_key_exists( $item_pai['numero'], $lista_final ) ) {
					$lista_final[$item_pai['numero']] = $item_pai;
				}
			}
		}
		// adiciona item à lista
		$lista_final[$item['numero']] = $item;
	}
	return array_values( $lista_final );
}

function atividade_calcular_dados( $atividade ){
	global $db;
	// pega dados da atividade
	$atividade = (integer) $atividade;
	$sql = "select _atiordem, _atinumero, _atiprofundidade, _atiprojeto from pde.atividade where atiid = " . $atividade;
	$pai = $db->recuperar( $sql );
	
	// pega filhos
	$sql = "select atiid, atiordem from pde.atividade where atiidpai = " . $atividade . " and atistatus = 'A'";
	$filhos = $db->carregar( $sql );
	$filhos = $filhos ? $filhos : array();
	$sql = "update pde.atividade set _atifilhos = " . count( $filhos ) . " where atiid = " . $atividade;
	$db->executar( $sql, false );
	
	// atualiza filhos
	foreach ( $filhos as $filho ){
		$_atinumero  = ( $pai['_atinumero'] ? $pai['_atinumero'] . "." : '' ) . $filho['atiordem'];
		$_atiordem   = ( $pai['_atiordem'] ? $pai['_atiordem'] : '' ) . sprintf( '%04d', $filho['atiordem'] );
		$_atiprojeto = (integer) $pai['_atiprojeto']; 
		$sql = "
			update pde.atividade
			set
				_atinumero = '" . $_atinumero . "',
				_atiordem = '" . $_atiordem . "',
				_atiprofundidade = " . ( $pai['_atiprofundidade'] + 1 ) . ",
				_atiirmaos = " . count( $filhos ) . ",
				_atiprojeto = " . $_atiprojeto . "
			where atiid = " . $filho['atiid'];
		$db->executar( $sql, false );
		atividade_calcular_dados( $filho['atiid'] );
	}
}

function atividade_calcular_possibilidade_mudar_data( $intIdAtividade , $strNovaDataInicio = null, $strNovaDataFim = null, $strNovaDataConclusao = null ){
	global $db;
	
	$sql = sprintf(
		"SELECT atidatainicio , atidatafim, atidataconclusao, esaid from pde.atividade where atiid = %d",
		$intIdAtividade
	);
	
	$arrAtiDatas = $db->recuperar( $sql );

	if( $strNovaDataInicio !== null )
	{
		$arrAtiDatas[ 'atidatainicio'  ] = formata_data_sql( $strNovaDataInicio );
	}
	if( $strNovaDataFim !== null )
	{
		$arrAtiDatas[ 'atidatafim'  ] = formata_data_sql( $strNovaDataFim );
	}
	if( $strNovaDataConclusao !== null )
	{
		$arrAtiDatas[ 'atidataconclusao'  ] = formata_data_sql( $strNovaDataConclusao );
	}
	
	$intDataInicio	= strtotime( $arrAtiDatas[ 'atidatainicio' ] );
	
	if( (integer) $arrAtiDatas['esaid'] == (integer) STATUS_CONCLUIDO )
	{
		if( $arrAtiDatas[ 'atidataconclusao' ] != null )
		{
			$intDataTermino = strtotime(  $arrAtiDatas[ 'atidataconclusao' ] );
		}
		else
		{
			$intDataTermino = null;
		}
	}
	else
	{
		if(  $arrAtiDatas[ 'atidatafim' ] != null )
		{
			$intDataTermino = strtotime( $arrAtiDatas[ 'atidatafim' ] );
		}
		else
		{
			$intDataTermino = null; 
		}
	}
	if	(
			( $intDataInicio !== null )
			&&
			( $intDataTermino !== null )
			&&
			( $intDataInicio > $intDataTermino )
		)
	{
		return false;
	}
	return true;
}


// RESPONSABILIDADE ////////////////////////////////////////////////////////////


/**
 * Atribui responsabilidade aos usuários na atividade indicada segundo o perfil
 * especificado.
 * 
 * @return boolean
 */
function atividade_atribuir_responsavel( $atividade, $perfil, $usuarios ){
	global $db;
	$sql = sprintf(
		"update pde.usuarioresponsabilidade
		set rpustatus = 'I'
		where pflcod = %d and atiid  = %d",
		$perfil,
		$atividade
	);
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	foreach ( $usuarios as $usuario ) {
	
		if ( empty( $usuario ) ) {
			continue;
		}
		$sql = "select count(*) from seguranca.perfilusuario where pflcod = $perfil and usucpf = '$usuario'";
		$possui_perfil = $db->pegaUm( $sql );
		if ( !$possui_perfil )
		{
			$sql = "insert into seguranca.perfilusuario ( pflcod, usucpf ) values ( $perfil, '$usuario' )";
			$db->executar( $sql );
		}
		$sql = sprintf(
			"select count(*) from pde.usuarioresponsabilidade
			where usucpf = '%s' and pflcod = %d and atiid = %d",
			$usuario,
			$perfil,
			$atividade
		);
		if( (boolean) $db->pegaUm( $sql ) ) {
			$sql = sprintf(
				"update pde.usuarioresponsabilidade
				set rpustatus = 'A'
				where usucpf = '%s' and pflcod = %d and atiid = %d",
				$usuario,
				$perfil,
				$atividade
			);
		} else {
			$sql = sprintf(
				"insert into pde.usuarioresponsabilidade (
					usucpf, pflcod, atiid, rpustatus
				) values (
					'%s', %d, %d, 'A'
				)",
				$usuario,
				$perfil,
				$atividade
			);
		}
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			return false;
		}
		$db->alterar_status_usuario( $usuario, 'A', 'Atribuição de responsabilidade em atividade ou projeto.', $_SESSION['sisid'] );
		$usuariodados = $db->pegaLinha("SELECT * FROM seguranca.usuario WHERE usucpf='".$usuario."'");
		if($usuariodados['usuchaveativacao'] == "f") {
			$remetente = array("nome" => "Simec","email" => "simec@mec.gov.br");
			$destinatario = $usuariodados['usuemail'];
            $assunto = "Aprovação do Cadastro no ".SIGLA_SISTEMA;
			$conteudo = "
						<br/>
						<span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span>
						<br/>
						<span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span>
						<br/>
						";
					$conteudo .= sprintf(
					"%s %s<p>Sua conta está ativa. Sua Senha de acesso é: %s</p>",
					$usuariodados['ususexo'] == 'M' ? 'Prezado Sr.' : 'Prezada Sra.',
					$usuariodados['usunome'],
					md5_decrypt_senha( $usuariodados['ususenha'], '' )
					);
					enviar_email( $remetente, $destinatario, $assunto, $conteudo );
		}
	
		
	}
	return true;
}

function envia_email_responsavel( $gerente = null, $arrApoio = null ){
	global $db;
	
	if($gerente[0]){
		$sql = "select usuemail from seguranca.usuario where usucpf = '{$gerente[0]}'";
		$email = $db->pegaUm($sql);
		$sql = "select _atinumero as numero, atidatafim as data, atidetalhamento as detalhamento
			 	from pde.atividade where atiid = ".$_REQUEST['atiid'];
		$atidescricao = $db->pegaLinha($sql);
		$remetente = array("nome" => "Simec","email" => "simec@mec.gov.br");
		if($email){
			$assunto  = "[SIMEC] Demandas SEB";
			$conteudo = "
				<font size='2'>E-mail para envio: {$email}.<font>
				<br><br>
				<font size='2'>Prezado(a) Diretor(a), o Gabinete da Secretaria de Educação Básica cadastrou no SIMEC 
				atividade de número ".utf8_decode($atidescricao['numero']).",  que deverá ser impreterivelmente executada 
				por esta Diretoria até o prazo ".formata_data($atidescricao['data'])." definido naquele registro.
				<br><br>
				Descrição da atividade cadastrada: ".utf8_decode($atidescricao['detalhamento']).".
				<font>
				<br><br>
				Atenciosamente,
				<br>
				Gabinete - Secretaria de Educação Básica";

				enviar_email($remetente, $email, $assunto, $conteudo );
		}
	}

	if($arrApoio){
		if( is_array($arrApoio) ){
			foreach( $arrApoio as $apoio ){
				$sql = "select usuemail from seguranca.usuario where usucpf = '{$apoio}'";
				$email = $db->pegaUm($sql);
				$sql = "select _atinumero as numero, atidatafim as data, atidetalhamento as detalhamento 
						from pde.atividade where atiid = ".$_REQUEST['atiid'];
				$atidescricao = $db->pegaLinha($sql);
				$remetente = array("nome" => "Simec","email" => "simec@mec.gov.br");
				if($email){
					$assunto  = "[SIMEC] Demandas SEB";
					$conteudo = "
						<font size='2'>E-mail para envio: {$email}.<font>
						<br><br>
						<font size='2'>Prezado(a) Diretor(a), o Gabinete da Secretaria de Educação Básica cadastrou no SIMEC 
						atividade de número ".utf8_decode($atidescricao['numero']).",  que deverá ser impreterivelmente executada 
						por esta Diretoria até o prazo ".formata_data($atidescricao['data'])." definido naquele registro.
						<br><br>
						Descrição da atividade cadastrada: ".utf8_decode($atidescricao['detalhamento']).".
						<font>
						<br><br>
						Atenciosamente,
						<br>
						Gabinete - Secretaria de Educação Básica";
			
						enviar_email($remetente, $email, $assunto, $conteudo );
				}
			}
		}
	}
}

/**
 * @return boolean
 */
function atividade_verificar_responsabilidade( $atividade, $usuario = null ){
	global $db;
	static $permissoes = array(); # responsabilidades atribuídas
	if ( $db->testa_superuser() ) {
		return true;
	}
	$usuario = $usuario ? $usuario : $_SESSION['usucpf'];
	if ( $_SESSION["sisid"] == 1 ) {
		return acao_verificar_responsabilidade( $atividade, $usuario );
	}
	if ( !array_key_exists( $usuario, $permissoes ) ) {
		/*
		$sql = sprintf(
			"select folha.atiid
			from pde.usuarioresponsabilidade ur
				inner join pde.f_dadostodasatividades() as raiz on
					raiz.atiid = ur.atiid
				inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
				inner join pde.f_dadostodasatividades() as folha on
					folha.atiid = raiz.atiid or folha.numero like raiz.numero || '.%%'
			where ur.rpustatus = 'A' and ur.usucpf = '%s' and ur.pflcod in ( %d, %d )
			group by folha.atiid",
			$usuario,
			PERFIL_GERENTE,
			PERFIL_EQUIPE_APOIO_GERENTE
		);
		*/
		
		
		$sql = sprintf(
			"select folha.atiid 
			from pde.usuarioresponsabilidade ur 
				inner join pde.atividade as raiz on 
					raiz.atiid = ur.atiid 
				inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf 
				inner join pde.atividade as folha on 
					folha.atiid = raiz.atiid or folha._atinumero like raiz._atinumero || '.%%' 
			where raiz.atistatus = 'A' and folha.atistatus = 'A' and ur.rpustatus = 'A' and ur.usucpf = '%s' and ur.pflcod in ( %d, %d ) 
			group by folha.atiid",
			$usuario,
			PERFIL_GERENTE,
			PERFIL_EQUIPE_APOIO_GERENTE
		);
		
		$lista = $db->carregar( $sql );
		$permissoes[$usuario] = array();
		if ( is_array( $lista ) ) {
			foreach ( $lista as $item ) {
				array_push( $permissoes[$usuario], $item['atiid'] );
			}
		}
	}
	
	if ( !in_array( $atividade, $permissoes[$usuario] ) ) {
		$projeto = $db->pegaUm( "select _atiprojeto from pde.atividade where atiid = " . $atividade );
		if ( !$projeto ) {
			$projeto = $atividade;
		}
		
		return projeto_verificar_responsabilidade( $projeto );
	}
	return true;
}

/**
 * @return boolean
 */
function atividade_verificar_perfil( $atividade, $perfil, $usuario = null ){
	global $db;
	static $permissoes = array(); # responsabilidades atribuídas
	
	$usuario = $usuario ? $usuario : $_SESSION['usucpf'];
	
	// CASO ESPECÍFICO PARA GERENTE DE PROJETO
	if ( $perfil == PERFIL_GERENTE )
	{
		$sql = "select usucpf from pde.atividade where atiid = " . $atividade;
		return $db->pegaUm( $sql ) == $usuario;
	}
	
	if ( $db->testa_superuser() ) {
		return true;
	}
	if ( !array_key_exists( $perfil, $permissoes ) ) {
		$sql = sprintf(
			"select folha.atiid
			from pde.usuarioresponsabilidade ur
				inner join pde.f_dadostodasatividades() as raiz on
					raiz.atiid = ur.atiid
				inner join pde.f_dadostodasatividades() as folha on
					folha.atiid = raiz.atiid or folha.numero like raiz.numero || '.%%'
			where ur.rpustatus = 'A' and ur.usucpf = '%s' and ur.pflcod = %d
			group by folha.atiid",
			$usuario,
			$perfil
		);
		$lista = $db->carregar( $sql );
		$permissoes[$usuario] = array();
		if ( is_array( $lista ) ) {
			foreach ( $lista as $item ) {
				array_push( $permissoes[$usuario], $item['atiid'] );
			}
		}
	}
	return in_array( $atividade, $permissoes[$usuario] );
}

function projeto_verificar_responsabilidade( $projeto, $usuario = null ){
	global $db;
	static $permissoes = array(); # responsabilidades atribuídas
	if ( $db->testa_superuser() ) {
		return true;
	}
	$usuario = $usuario ? $usuario : $_SESSION['usucpf'];
	if ( $_SESSION["sisid"] == 1 ) {
		return acao_verificar_responsabilidade( $projeto, $usuario );
	}
	/*
	$sql = sprintf(
		"select count(*)
		from pde.usuarioresponsabilidade ur
			inner join pde.f_dadostodasatividades() as raiz on
				raiz.atiid = ur.atiid
			inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
			inner join pde.f_dadostodasatividades() as folha on
				folha.atiid = raiz.atiid or folha.numero like raiz.numero || '.%%'
		where ur.atiid = %d and ur.rpustatus = 'A' and ur.usucpf = '%s' and ur.pflcod in ( %d, %d )
		",
		$projeto,
		$usuario,
		PERFIL_GESTOR,
		PERFIL_EQUIPE_APOIO_GESTOR
	);
	*/
	
	$sql = sprintf(
		"select count(*)
		from pde.usuarioresponsabilidade ur
			inner join pde.atividade as raiz on
				raiz.atiid = ur.atiid
			inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
			inner join pde.atividade as folha on
				folha.atiid = raiz.atiid or folha._atinumero like raiz._atinumero || '.%%'
		where raiz.atistatus = 'A' and folha.atistatus = 'A' and ur.atiid = %d and ur.rpustatus = 'A' and ur.usucpf = '%s' and ur.pflcod in ( %d, %d )
		",
		$projeto,
		$usuario,
		PERFIL_GESTOR,
		PERFIL_EQUIPE_APOIO_GESTOR
	);
	
	return $db->pegaUm( $sql ) > 0;
}

/**
 * @return boolean
 */
function usuario_possui_perfil( $perfil, $usuario = null ){
	global $db;
	if ( $db->testa_superuser() ) {
		return true;
	}
	$usuario = $usuario ? $usuario : $_SESSION['usucpf'];
	$sql = sprintf(
		"select count( * )
		from seguranca.perfilusuario
		where
			usucpf = '%s' and
			pflcod = %d",
		$usuario,
		$perfil
	);
	return (boolean) $db->pegaUm( $sql );
}


// ORDEM E NÍVEL DAS ATIVIDADES //////////////////////////////////////////////////////


function atividade_ordem_subir( $atiid ){
	global $db;
	// verifica se está no topo
	$sql = sprintf(
		"select a.* from pde.atividade a where a.atiid = %d and a.atistatus = 'A' and a.atiordem > 1",
		$atiid
	);
	$atividade = $db->pegaLinha( $sql );
	if ( !$atividade ) {
		return true;
	}
	// altera a posição dos irmãos
	$sql = sprintf(
		"update pde.atividade set atiordem = %d where atiordem = %d and atiidpai = %d and atistatus = 'A'",
		$atividade['atiordem'],
		$atividade['atiordem'] - 1,
		$atividade['atiidpai']
	);
	if( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	// altera a posição da atividade
	$sql = sprintf(
		"update pde.atividade set atiordem = %d where atiid = %d and atistatus = 'A'",
		$atividade['atiordem'] - 1,
		$atividade['atiid']
	);
	if( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	return true;
}

function atividade_ordem_descer( $atiid ){
	global $db;
	// verifica se está no final
	$sql = sprintf(
		"select a1.* from pde.atividade a1 where atiid = %d and atistatus = 'A' and a1.atiordem < ( select count(*) from pde.atividade a2 where a2.atiidpai = a1.atiidpai and atistatus = 'A' )",
		$atiid
	);
	$atividade = $db->pegaLinha( $sql );
	if ( !$atividade ) {
		return true;
	}
	// altera a posição dos irmãos
	$sql = sprintf(
		"update pde.atividade set atiordem = %d where atiordem = %d and atiidpai = %d and atistatus = 'A'",
		$atividade['atiordem'],
		$atividade['atiordem'] + 1,
		$atividade['atiidpai']
	);
	if( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	$sql = sprintf(
		"update pde.atividade set atiordem = %d where atiid = %d and atistatus = 'A'",
		$atividade['atiordem'] + 1,
		$atividade['atiid']
	);
	if( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	return true;
}

function atividade_profundidade_esquerda( $atiid ){
	global $db;
	// carrega os dados da atividade
	$sql = sprintf(
		"select a1.* from pde.atividade a1 where atiid = %d and atistatus = 'A'",
		$atiid
	);
	$atividade = $db->pegaLinha( $sql );
	if ( !$atividade ) {
		return false;
	}
	// carrega os dados do antigo pai da atividade
	$sql = sprintf(
		"select a1.* from pde.atividade a1 where atiid = %d and atistatus = 'A'",
		$atividade['atiidpai']
	);
	$atividade_pai = $db->pegaLinha( $sql );
	if ( !$atividade_pai ) {
		$db->rollback();
		return false;
	}
	// desloca os novos irmãos para baixo
	$sql = sprintf(
		"update pde.atividade set atiordem = atiordem + 1 where atistatus = 'A' and atiidpai = %d and atiordem > %d",
		$atividade_pai['atiidpai'],
		$atividade_pai['atiordem']
	);
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	// desloca os antigos irmãos para cima
	$sql = sprintf(
		"update pde.atividade set atiordem = atiordem - 1 where atistatus = 'A' and atiidpai = %d and atiordem > %d",
		$atividade['atiidpai'],
		$atividade['atiordem']
	);
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	// troca o pai (pelo avô)
	$sql = sprintf(
		"update pde.atividade set atiidpai = %d, atiordem = %d where atistatus = 'A' and atiid = %d",
		$atividade_pai['atiidpai'],
		$atividade_pai['atiordem'] + 1,
		$atividade['atiid']
	);
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	return true;
}

function atividade_profundidade_direita( $atiid ){
	global $db;
	// carrega os dados da atividade
	$sql = sprintf(
		"select a1.* from pde.atividade a1 where atiid = %d and atistatus = 'A'",
		$atiid
	);
	$atividade = $db->pegaLinha( $sql );
	if ( !$atividade ) {
		return false;
	}
	// carrega o novo pai (irmão que está uma posição acima)
	$sql = sprintf(
		"select a1.* from pde.atividade a1 where atiidpai = %d and atiordem = %d and atistatus = 'A'",
		$atividade['atiidpai'],
		$atividade['atiordem'] - 1
	);
	$atividade_pai = $db->pegaLinha( $sql );
	if ( !$atividade_pai ) {
		$db->rollback();
		return false;
	}
	// desloca os antigos irmãos para cima
	$sql = sprintf(
		"update pde.atividade set atiordem = atiordem - 1 where atiidpai = %d and atiordem > %d and atistatus = 'A' ",
		$atividade['atiidpai'],
		$atividade['atiordem']
	);
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	// troca o pai (pelo antigo irmão)
	$sql = sprintf(
		"update pde.atividade set atiidpai = %d, atiordem = 1 + ( select count(*) from pde.atividade where atiidpai = %d and atistatus = 'A' ) where atiid = %d and atistatus = 'A'",
		$atividade_pai['atiid'],
		$atividade_pai['atiid'],
		$atividade['atiid']
	);
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	return true;
}


// ÁRVORE //////////////////////////////////////////////////////////////////////


function arvore_ocultar_item( $atividade ){
	if ( !isset( $_SESSION['arvore'] ) ) {
		$_SESSION['arvore'] = array();
	}
	$_SESSION['arvore'][$atividade] = $atividade;
}

function arvore_exibir_item( $atividade ){
	if ( !isset( $_SESSION['arvore'] ) ) {
		$_SESSION['arvore'] = array();
	}
	unset( $_SESSION['arvore'][$atividade] );
}

function arvore_verificar_exibicao_item( $numero, $ignorar = array() ){
	if ( !array_key_exists( 'arvore', $_SESSION ) ) {
		arvore_iniciar_dados_sessao();
	}
	$ignorar = $ignorar ? $ignorar : array();
	$arvore = arvore_pegar_estado_exibicao( $numero );
	// verifica o estado de exibição do item
	$numero = explode( '.', substr( $numero, 0, strrpos( $numero, '.' ) ) );
	for ( $i = count( $numero ); $i > 0; $i-- ) {
		$numero_atual = implode( '.', array_slice( $numero, 0, $i ) );
		if ( in_array( $numero_atual, $arvore ) && !in_array( $numero_atual, $ignorar ) ) {
			return false;
		}
	}
	return true;
}

function arvore_verificar_exibicao_filhos( $numero ){
	if ( !array_key_exists( 'arvore', $_SESSION ) ) {
		arvore_iniciar_dados_sessao();
	}
	$arvore = arvore_pegar_estado_exibicao( $numero );
	return in_array( $numero, $arvore );
}

function arvore_pegar_estado_exibicao( $numero ){
	static $arvore = null;
	global $db;
	if ( !array_key_exists( 'arvore', $_SESSION ) ) {
		arvore_iniciar_dados_sessao();
	}
	// verifica se há alguma informação na sessão
	if ( empty( $_SESSION['arvore'] ) ) {
		return array();
	}
	// carrega os números a partir dos ids gravados na sessão
	if ( !is_array( $arvore ) ) {
		$sql = sprintf(
			"select numero from pde.f_dadostodasatividades() where atiid in ( %s )",
			implode( ',', $_SESSION['arvore'] )
		);
		$arvore = array();
		$atividades = $db->carregar( $sql );
		if ( is_array( $atividades ) ) {
			foreach ( $atividades as $atividade ) {
				array_push( $arvore, $atividade['numero'] );
			}
		}
	}
	return $arvore;
}

function arvore_iniciar_dados_sessao(){
	global $db;
	$sql = "select atiid from pde.atividade where atistatus = 'A' and _atiprojeto = " . PROJETO;
	$linhas = $db->carregar( $sql );
	$linhas = $linhas ? $linhas : array();
	foreach ( $linhas as $linha ){
		arvore_ocultar_item( (integer) $linha['atiid'] );
	}
} 


// OUTRAS FUNÇÕES //////////////////////////////////////////////////////////////


/**
 * Redireciona o navegador para a tela indicada.
 * 
 * @return void
 */
function redirecionar( $modulo, $acao, $parametros = array() ) {

	$parametros = http_build_query( (array) $parametros, '', '&' );
	header( "Location: ?modulo=$modulo&acao=$acao&$parametros" );
	exit();
}

/**
 * Verifica se um projeto está selecionado.
 * 
 * Caso uma atividade seja passada como parâmetro verifica se além de algum
 * projeto está selecionado essa atividade pertença ao projeto selecionado.
 * Essa função redireciona para a tela de projetos caso a verificação falhe.
 *
 * @param integer $atividade
 * @return void
 */
function projeto_verifica_selecionado( $atividade = null ) {
	global $db;
	
	$atividade = (integer) $atividade;
	// verifica se projeto está escolhido
	$sql = sprintf( "select count(atiid) from pde.atividade where atiid = %d and atistatus = 'A'", $_SESSION['projeto'] );

	if ( $db->pegaUm( $sql ) != 1 ) {
		redirecionar( $_SESSION['paginainicial'], 'A' );
	}

	// verifica se a atividade indicada pertence ao projeto atual
	if ( !$atividade ) {
		return;
	}
	$sql = sprintf( "select _atiprojeto from pde.atividade where atiid = %d", $atividade );

	if ( $db->pegaUm( $sql ) != $_SESSION['projeto'] ) {
		redirecionar( $_SESSION['paginainicial'], 'A' );
	}
}


// OUTRAS FUNÇÕES //////////////////////////////////////////////////////////////


function registrar_mensagem( $mensagem ){
	if ( !isset( $_SESSION['mensagem'] ) ) {
		$_SESSION['mensagem'] = array();
	}
	array_push( $_SESSION['mensagem'], $mensagem );
}

function exibir_mensagens(){
	if ( !isset( $_SESSION['mensagem'] ) ) {
		$_SESSION['mensagem'] = array();
	}
	if ( count( $_SESSION['mensagem'] ) == 0 ) {
		return;
	}
	$htm = '<script language="javascript" type="text/javascript">';
	$htm .= 'alert("'. implode( "\n", $_SESSION['mensagem'] ) .'")';
	$htm .= '</script>';
	$_SESSION['mensagem'] = array();
	return $htm;
}

function acao_verificar_responsabilidade( $atividade, $usuario ){
	global $db;
	$ano = $_SESSION['exercicio'];
	$sql = <<<EOS
		select count( u.usucpf )
		from pde.atividade ati
		inner join monitora.acao aca on aca.acaid = ati.acaid
		inner join monitora.usuarioresponsabilidade ur on ur.acaid = aca.acaid
		inner join seguranca.perfil p on p.pflcod = ur.pflcod
		inner join seguranca.usuario u on u.usucpf = ur.usucpf
		inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
		where
		ati.atistatus = 'A' and ati.atiid = $atividade
		and aca.acastatus = 'A'
		and ur.rpustatus = 'A' and ur.prsano = '$ano'
		and p.pflstatus = 'A'
		and u.suscod = 'A' and u.usucpf = '$usuario'
		and us.suscod = 'A'
EOS;
	return $db->pegaUm( $sql ) > 0;
}

function arrayPerfil(){ 
	global $db;

	$sql = sprintf("SELECT
					 pu.pflcod
					FROM
					 seguranca.perfilusuario pu
					 INNER JOIN seguranca.perfil p ON p.pflcod = pu.pflcod AND
					 	p.sisid = 11
					WHERE
					 pu.usucpf = '%s'
					ORDER BY
					 p.pflnivel",
				$_SESSION['usucpf']);
	return (array) $db->carregarColuna($sql,'pflcod');
}

function verificaPermissaoNivel($atividade){
	global $db;

	$sql = "SELECT
				distinct usucpf 
			FROM 
				pde.usuarioresponsabilidade ur 
			WHERE 
				ur.atiid = {$atividade['atiid']} AND 
				rpustatus = 'A' 
				AND usucpf = '{$atividade['usucpf']}' 
				AND pflcod IN (".PERFIL_EQUIPE_APOIO_GERENTE.",".PERFIL_GERENTE.")";

	if( $db->pegaUm( $sql ) ){
		return true;
	} else {
		return false;
	}
}

function temPerfilSomenteConsulta()
{
	global $db;
	
	if( $db->testa_superuser() )
	{
		return false;
	}
	else
	{
		$sql = "SELECT count(1) FROM perfilusuario WHERE usucpf = '".$_SESSION["usucpf"]."' AND pflcod = ".PERFIL_SOMENTE_CONSULTA;
		$perfil = $db->pegaUm($sql);
		
		if($perfil > 0)
			return true;
		else
			return false;
	}
}

function temPerfilAdministrador()
{
	global $db;
	
	if( $db->testa_superuser() )
	{
		return false;
	}
	else
	{
		$sql = "SELECT count(1) FROM perfilusuario WHERE usucpf = '".$_SESSION["usucpf"]."' AND pflcod = ".PERFIL_ADMINISTRADOR;
		$perfil = $db->pegaUm($sql);
		
		if($perfil > 0)
			return true;
		else
			return false;
	}
}

function cadastrarMonitoraItemCheckList()
{
	global $db;
	$micid = $_GET['micid'];
	$atiid = $_GET['atiid'];
	$mtiid = $_POST['mtiid'] ? $_POST['mtiid'] : "NULL";
	
	$micestrategico = $_REQUEST['micestrategico'] == 'on'? 't':'f';
	$micenviasms 	= $_REQUEST['micenviasms']    == 'on'? 't':'f';
	
	$micetapa = !empty($_REQUEST['micetapa']) ? "'{$_REQUEST['micetapa']}'" : "NULL";
	
	if($micid){
		$sql = "select indid from pde.monitoraitemchecklist where micid = $micid";
		$indid = $db->pegaUm($sql);
	
		$sql = "update pde.monitoraitemchecklist set micestrategico = '$micestrategico', micenviasms = '$micenviasms', micetapa=$micetapa  where micid = $micid";

		$db->executar($sql);
	}

	if($mtmid == 1){
		$_REQUEST['umeid'] = 6;
	}
		
	$exoid 				= $_REQUEST['exoid'];
	$unmid 				= $_REQUEST['unmid']; 
	$cliid 				= $_REQUEST['cliid'];
	$tpiid 				= $_REQUEST['tpiid'];  
	$secid  			= $_REQUEST['secid'];  
	$secidgestora		= $_REQUEST['secidgestora'];
	$acaid   			= $_REQUEST['acaid']; 
	$perid  			= $_REQUEST['perid'];  
	$peridatual			= $_REQUEST['peridatual'];
	$estid  			= $_REQUEST['estid'];
	$regid				= $_REQUEST['regid'];
	$umeid				= $_REQUEST['umeid'];
	$colid  			= $_REQUEST['colid'];  
	$usucpf  			= $_SESSION['usucpf'];  
	$indnome  			= $_REQUEST['indnome'];  
	$indapelido  		= $_REQUEST['indapelido'];  
	$indobjetivo  		= "N/A"; 
	$indformula  		= "NULL";  
	$indtermos  		= "NULL";  
	$indfontetermo  	= "NULL";
	if( $unmid == UNIDADEMEDICAO_MOEDA || $_REQUEST['mtmid'] == 1 || $_REQUEST['mtmid'] == 2 ){
		$indqtdevalor	= "false";
	}else{
		$indqtdevalor	= "true";
	}
	$indcumulativo		= (($_REQUEST['indcumulativo'])?$_REQUEST['indcumulativo']:"N");
	$indcumulativovalor = $_REQUEST['indcumulativovalor'];
	$indpublicado		= "false";
	$indpublico			= "false";
	$indobservacao		= $_REQUEST['indobservacao'];
	$indescala			= "false";
	
	if($indid){						
		
		$sql = "
		SELECT 
			indid				
		FROM  painel.indicador i
		WHERE 
		i.indid = ".$indid."
		";					
		$dados = $db->pegaUm($sql);
	}
	
	//Campos Não obrigatorios
	(!$cliid)? $cliid = 'null' : $cliid = $cliid;
	($umeid == "")? $umeid = 'null' : $umeid = $umeid;
	($regid == "")? $regid = 'null' : $regid = $regid;
	($tpiid == "")? $tpiid = '1' : $tpiid = $tpiid;
	($indnormalinicio == "")?	$indnormalinicio 	= "null" : $indnormalinicio 	= $indnormalinicio;
	($indnormalfinal == "")? 	$indnormalfinal 	= "null" : $indnormalfinal 		= $indnormalfinal;
	($indatencaoinicio == "")? 	$indatencaoinicio 	= "null" : $indatencaoinicio 	= $indatencaoinicio;
	($indatencaofinal == "")? 	$indatencaofinal 	= "null" : $indatencaofinal 	= $indatencaofinal;
	($indcriticoinicio == "")? 	$indcriticoinicio 	= "null" : $indcriticoinicio 	= $indcriticoinicio;
	($indcriticofinal == "")? 	$indcriticofinal 	= "null" : $indcriticofinal 	= $indcriticofinal;
	($indmetavalor == "")? 		$indmetavalor 		= "null" : $indmetavalor	 	= $indmetavalor;
	($secidgestora == "")? 		$secidgestora 		= "null" : $secidgestora	 	= $secidgestora;
	
	$usucpf = $_SESSION['usucpf'];
	
	//ver($dados,d);
	
	// caso não exista o indicador insere um novo
	if(!$dados){	
		$sql_I = "INSERT 
				  INTO painel.indicador
					  (indstatus,unmid, cliid, tpiid, secid, secidgestora,acaid, perid, estid, colid, usucpf, indnome, indobjetivo,
					  indformula, indtermos, indfontetermo, indnormalinicio, indnormalfinal, indatencaoinicio,
					  indatencaofinal, indcriticoinicio, indcriticofinal, indmetavalor, indmetadatalimite,exoid,regid,umeid,indqtdevalor,indcumulativo,indpublicado,indescala,indobservacao,peridatual,indcumulativovalor,indpublico,mtiid,indhomologado, indapelido)
				  VALUES 
				  	   ('I',".$unmid.", ".$cliid.", ".$tpiid.", ".$secid.", ".$secidgestora.", ".$acaid.", ".$perid.", ".$estid.", ".$colid.", '".$usucpf."', '".$indnome."', '".$indnome."',
					  NULL, NULL, '".$indfontetermo."', ".$indnormalinicio.", ".$indnormalfinal.", ".$indatencaoinicio.",
					  ".$indatencaofinal.", ".$indcriticoinicio.", ".$indcriticofinal.", ".$indmetavalor.", '".$indmetadatalimite."','".$exoid."',$regid,$umeid,$indqtdevalor,'$indcumulativo',$indpublicado,$indescala,'$indobservacao','$peridatual','$indcumulativovalor',$indpublico,$mtiid,true, '$indapelido')
				  RETURNING	indid
				  ";
	
		$indid = $db->pegaUm($sql_I);
		
		//AGENDA DE GOVERNO
		foreach($_REQUEST['aggid'] as $aggid) {
			if($aggid){
				$sql = "INSERT INTO painel.agendaindicador(aggid, indid) VALUES (". $aggid .", ". $indid .")";			
				$db->executar($sql);
			}
		}
		
		//INDICADORES VINCULADOS
		
		$sql = "UPDATE painel.indicadoresvinculados set idvstatus = 'I' WHERE indidvinculo =  ". $indid ."";			
		$db->executar($sql);
		$_POST['indid_vinculado'] = $_POST['indid_vinculado'] ? $_POST['indid_vinculado'] : array();
		foreach($_POST['indid_vinculado'] as $key => $indid_vinculado) {
			if($indid_vinculado){
				$idvfiltro = $_POST['indid_filtro'][$key];
				$idvdsc    = $_POST['idvdsc'][$key];
				$idvmeta   = $_POST['idvmeta'][$key];
				$idvmeta   = ($idvmeta ? desformata_valor( $idvmeta ) : 'null');
				$sql 	   = "INSERT INTO painel.indicadoresvinculados(
								indid, indidvinculo,idvstatus, idvfiltro, idvdsc, idvmeta
							  ) VALUES (
							  	". $indid_vinculado .", ". $indid .",'A', '" . $idvfiltro . "', '" . $idvdsc . "', " . $idvmeta . "
							  )";			
				$db->executar($sql);
			}
		}
		
		
	//caso exista o indicador altera os dados do mesmo	
	} else {
	
	
		$sql_U = "
			UPDATE painel.indicador
			SET
			 unmid = 				".$unmid.",
			 exoid = 				".$exoid.",
			 cliid = 				".$cliid.",
			 tpiid = 				".$tpiid.",
			 estid = 				".$estid.",
			 secid = 				".$secid.",
			 secidgestora =			".$secidgestora.",
			 acaid = 				".$acaid.",
			 perid = 				".$perid.",
			 peridatual =			'".$peridatual."',
			 colid = 				".$colid.",
			 usucpf = 				'".$usucpf."',
			 indnome = 				'".$indnome."',
			 indapelido = 				'".$indapelido."',
			 indfontetermo = 		'".$indfontetermo."',
			 indnormalinicio = 		".$indnormalinicio.",
			 indnormalfinal = 		".$indnormalfinal.",
			 indatencaoinicio = 	".$indatencaoinicio.",
			 indatencaofinal = 		 ".$indatencaofinal.",	
			 indcriticoinicio = 	".$indcriticoinicio.",
			 indcriticofinal = 		".$indcriticofinal.",
			 indmetavalor = 		".$indmetavalor.",
			 regid	= 				".$regid.",
			 umeid	= 				".$umeid.",
			 indmetadatalimite = 	'".$indmetadatalimite."',
			 indqtdevalor = 		'".$indqtdevalor."',
			 indcumulativo = 		'".($indcumulativo==''?'N' : $indcumulativo)."',	
			 indpublicado = 		".($indpublicado=='' ? 'indpublicado' : "'".$indpublicado."'").",
			 indescala = 			'".$indescala."',
			 indobservacao = 		'".$indobservacao."',
			 indcumulativovalor	=	'$indcumulativovalor',
			 indpublico = 		".($indpublico=='' ? 'indpublico' : "'".$indpublico."'").",
			 mtiid = $mtiid,
			 indhomologado = true
			WHERE indid = 			".$indid."	
		";
		
		$db->executar($sql_U);
				
		//AGENDA DE GOVERNO
		$sql = "DELETE FROM painel.agendaindicador WHERE indid =  ". $indid ."";			
		$db->executar($sql);
		$_POST['aggid'] = $_POST['aggid'] ? $_POST['aggid'] : array();
		foreach($_POST['aggid'] as $aggid) {
			if($aggid){
				$sql = "INSERT INTO painel.agendaindicador(aggid, indid) VALUES (". $aggid .", ". $indid .")";			
				$db->executar($sql);
			}
		}
		
		//INDICADORES VINCULADOS
		
		
		if ($_POST['idvid_meta'] || $_POST['nova_meta']) {
			if (!$_POST['nova_meta'] && $_POST['idvid_meta']) {
				$sql = "UPDATE painel.indicadoresvinculados set idvstatus = 'I' WHERE idvid =  ". $_POST['idvid_meta'] ."";
				$db->executar($sql);
			}
			
			$indid_vinculado = $_POST['indid_vinculado_meta'];
			$idvfiltro       = $_POST['indid_filtro_meta'];
			$idvdsc          = $_POST['idvdsc_meta'];
			$idvmeta         = $_POST['idvmeta_meta'];
			$idvmeta         = ($idvmeta ? desformata_valor( $idvmeta ) : 'null');
			$idvdatameta     = $_POST['idvdatameta_meta'] ? "'" . formata_data_sql($_POST['idvdatameta_meta']) . "'" : 'null';
			
			$sql = "INSERT INTO painel.indicadoresvinculados(
						indid, indidvinculo, idvstatus, idvfiltro, idvdsc, idvmeta, idvdatameta
					) VALUES (
						". $indid_vinculado .", ". $indid .", 'A', '" . $idvfiltro . "', '" . $idvdsc . "', " . $idvmeta . ", " . $idvdatameta . "
					)";

			$db->executar($sql);
			
		} else {
			$sql = "UPDATE painel.indicadoresvinculados set idvstatus = 'I' WHERE indidvinculo =  ". $indid ."";
			$db->executar($sql);
			
			$_POST['indid_vinculado'] = $_POST['indid_vinculado'] ? $_POST['indid_vinculado'] : array();
			
			foreach($_POST['indid_vinculado'] as $key => $indid_vinculado) {
				if($indid_vinculado){
					$idvfiltro   = $_POST['indid_filtro'][$key];
					$idvdsc      = $_POST['idvdsc'][$key];
					$idvmeta     = $_POST['idvmeta'][$key];
					$idvmeta     = ($idvmeta ? desformata_valor( $idvmeta ) : 'null');
					$idvdatameta = $_POST['idvdatameta'][$key] ? "'" . formata_data_sql($_POST['idvdatameta'][$key]) . "'" : 'null';
					
					$sql = "INSERT INTO painel.indicadoresvinculados(
								indid, indidvinculo, idvstatus, idvfiltro, idvdsc, idvmeta, idvdatameta
							) VALUES (
								". $indid_vinculado .", ". $indid .", 'A', '" . $idvfiltro . "', '" . $idvdsc . "', " . $idvmeta . ", " . $idvdatameta . "
							)";			
					$db->executar($sql);
				}
			}
		}
	}	
	
	
	$sqlOrdem = "select max(micordem) from pde.monitoraitemchecklist where atiid = {$_POST['atiid']} and micstatus = 'A'";
	$micordem = $db->pegaUm($sqlOrdem);
	$micordem = !$micordem ? 1 : (int)$micordem+1;
	
	if($micid){
		$sql = "update
					pde.monitoraitemchecklist
				set
					indid = $indid,
					mtiid = $mtiid
				where
					micid = $micid";
		$db->executar($sql);
	}else{
		$sql = "insert into
					pde.monitoraitemchecklist
				(indid,atiid,micordem,micstatus,mtiid,micestrategico,micenviasms,micetapa)
					values
				($indid,$atiid,$micordem,'A',$mtiid,'$micestrategico','$micenviasms',$micetapa) returning micid";
		$micid = $db->pegaUm($sql);
	}
	unset($_POST['micid']);
	//Salva a Meta
	foreach($_POST as $chave => $valor){
		if($valor){
			${$chave} = "'$valor'";
		}else{
			${$chave} = "NULL";
		}
	}
	$sql = "select mnmid from  pde.monitorameta where micid = $micid and mnmstatus = 'A' order by mnmid desc limit 1";
	
	$mnmid = $db->pegaUm($sql);
	$atiid = $_POST['atiid'];
	$mnmordem = "NULL";
	$mnmdsc = "'Meta - ".$_POST['indnome']."'";
	$mtmid = !$_POST['mtmid'] ? "NULL" : $mtmid;
	
	if($mnmid){
		$sql = "UPDATE
					pde.monitorameta 
				set
					mtmid = $mtmid,
					mnmdsc = $mnmdsc,
					docid = null
				where
					mnmid = $mnmid";
		$db->executar($sql);
		
	}else{
		//$tpdid 	= TIPO_FLUXO_MONITORAMENTO;
		//$docdsc = "Meta - ".$_POST['mnmdsc']."";
		//$docid = wf_cadastrarDocumento( $tpdid, $docdsc );
		$sql = "INSERT INTO 
					pde.monitorameta 
				(mnmdsc,micid,docid,mtmid,mnmordem)
					VALUES 
				($mnmdsc,$micid,null,$mtmid,$mnmordem)
						RETURNING mnmid";
				$mnmid = $db->pegaUm($sql);
				
	}
	
	//Se não for apenas tipo prazo(mtmid = 1), cadastra a meta no Painel de Controle
	$sql = "select metid from pde.monitorameta where mnmid = $mnmid";
	$metid = $db->pegaUm($sql);
	if($mtmid != "'1'" && $metid){//Se não for tipo prazo e houver meta cadastrada no painel
		$sql = "update 
					painel.metaindicador
				set
					metdesc = $mnmdsc,
					metstatus = 'A'
				where
					metid = $metid";
		$db->executar($sql);
	}elseif($metid){//Se houver meta cadastrada no painel
		$sql = "update painel.metaindicador set metstatus = 'I' where metid = $metid";
		$db->executar($sql);
	}elseif(!$metid){//Se não houver meta cadastrada no painel
		$sql = "select 
					ind.indid,
					ind.perid 
				from 
					pde.monitoraitemchecklist mic
				inner join
					painel.indicador ind ON ind.indid = mic.indid 
				where 
					micid = $micid";
		$indid = $db->pegaLinha($sql);
		$sql = "insert into 
					painel.metaindicador
				(indid,perid,metdesc,metstatus,metcumulativa)
					values
				({$indid['indid']},$perid,$mnmdsc,'A','S') returning metid";
		$metid = $db->pegaUm($sql);
		$sql = "update pde.monitorameta set metid = $metid where mnmid = $mnmid";
		$db->executar($sql);
	}
	
	$db->commit();
	$db->sucesso("principal/atividade_estrategico/item_checklist","&atiid=$atiid&micid=$micid");
	
}

function recuparMonitoraItemCheckList($micid)
{
	global $db;
	$sql = "	select
					mic.*,
					ind.*,
					mtm.*,
					mic.mtiid
				from
					pde.monitoraitemchecklist mic
				inner join
					painel.indicador ind ON ind.indid = mic.indid
				left join
					pde.monitorameta mtm ON mtm.micid = mic.micid and mnmstatus = 'A'
				where
					mic.micid = $micid 
				order by 
					mnmid desc limit 1";
	$arrDados = $db->pegaLinha($sql);
	
	return $arrDados ? $arrDados : array();
}

function listarMonitoraItemCheckList($atiid)
{
	global $db;
	$sql = "SELECT
				'<img src=\"../imagens/alterar.gif\" title=\"Editar Item\" class=\"link img_middle\" onclick=\"editarMonitoraItemChecklist(' || mic.micid || ')\"  />'||
				CASE WHEN foo.qtd > 0
					THEN '&nbsp;<img src=\"../imagens/excluir_01.gif\" title=\"Excluir Item\" class=\"link img_middle\"  />'
					ELSE '&nbsp;<img src=\"../imagens/excluir.gif\" title=\"Excluir Item\" class=\"link img_middle\" onclick=\"excluirMonitoraItemChecklist(' || mic.micid || ')\"  />'
				END
				||'&nbsp;<img src=\"../imagens/seta_cima.gif\" title=\"Aumentar Ordem\" class=\"link img_middle\" onclick=\"ordemMonitoraItemChecklist(this,' || mic.micid || ',\'cima\')\"  />
				&nbsp;<img src=\"../imagens/seta_baixo.gif\" title=\"Diminuir Ordem\" class=\"link img_middle\" onclick=\"ordemMonitoraItemChecklist(this,' || mic.micid || ',\'baixo\')\"  />' as acao,
				indnome
			FROM
				pde.monitoraitemchecklist mic
			INNER JOIN painel.indicador 		ind ON ind.indid = mic.indid
			INNER JOIN painel.eixo 				exo ON exo.exoid = ind.exoid
			INNER JOIN painel.acao 				aca ON aca.acaid = ind.acaid
			INNER JOIN painel.estilo 		 	est ON est.estid = ind.estid
			INNER JOIN painel.unidademedicao 	unm ON unm.unmid = ind.unmid
			LEFT JOIN (SELECT 
							ind2.indid,
							count(dmi.metid) as qtd
						FROM 
							painel.metaindicador met
						INNER JOIN painel.indicador 		   ind2 ON ind2.indid = met.indid
						INNER JOIN painel.unidademeta 			ume ON ind2.umeid = ume.umeid
						INNER JOIN painel.detalhemetaindicador 	dmi ON dmi.metid = met.metid
						WHERE
							dmi.dmistatus = 'A'
						GROUP BY ind2.indid) foo ON foo.indid = ind.indid
			WHERE
				atiid = $atiid
				AND micstatus = 'A'
			ORDER BY
				micordem,micid";
//	ver($sql);
	$arrCab = array("Ação","Descrição do Item");
	//$arrCab = array("Ação","Descrição do Item","Estilo","Eixo","Ação","Unidade de Medição");
	$db->monta_lista($sql,$arrCab,100,10,"N","center","N");
}

function excluirMonitoraItemChecklist()
{
	global $db;
	
	extract($_GET);
	if($micid){
		$sql = "update 
					painel.indicador
				set
					indstatus ='I' 
				where
					indid = (select indid from pde.monitoraitemchecklist where micid = $micid);
				update
					pde.monitoraitemchecklist
				set
					micstatus = 'I'
				where
					micid = $micid";
		$db->executar($sql);
		$db->commit();
		$db->sucesso("principal/atividade_estrategico/listar_checklist","&atiid=$atiid");
	}else{
		echo "<script>alert('Não foi possível excluir o Ítem de Check List.');window.location='estrategico.php?modulo=principal/atividade_estrategico/cadastro_checklist&acao=A&atiid=$atiid'</script>";
	}
	exit;
}

function recuperaMetaExercicioProjeto($atiid)
{
	global $db;
	$sql = "select * from pde.monitoraexercicioprojeto where atiid = $atiid";
	return $db->carregar($sql);
}

function temPerfilExecValidCertif()
{
	global $db;
	
	if( $db->testa_superuser() )
	{
		return false;
	}
	else
	{
		$sql = "SELECT count(1) FROM perfilusuario WHERE usucpf = '".$_SESSION["usucpf"]."' AND pflcod in (".PERFIL_EXECUTOR.",".PERFIL_VALIDADOR.",".PERFIL_CERTIFICADOR.")";
		$perfil = $db->pegaUm($sql);
		
		if($perfil > 0)
			return true;
		else
			return false;
	}
}

function cadastrarMonitoraMeta()
{
	global $db;
	
	$atiid = $_POST['atiid'];
	$micid = $_POST['micid'];
	$mnmid = $_POST['mnmid'];
	
	/*** Execução ***/
	$sql = "delete from pde.monitoraetapascontrole where mnmid = $mnmid and tpvid = 1";
	$db->executar($sql);
	if( $_REQUEST["entid_executor"] )
	{
		if( $_REQUEST["opcao_evidencia_execucao"] == 'S' )
		{
			$etcopcaoevidencia = 't';
			$etcevidencia = "'".$_REQUEST["etcevidenciaexecucao"]."'";
		}
		else
		{
			$etcopcaoevidencia = 'f';
			$etcevidencia = 'null';
		}
				
		$sql = "INSERT INTO pde.monitoraetapascontrole (tpvid,mnmid,mecopcaoevidencia,mecevidencia)
				VALUES (1, ".$mnmid.", '".$etcopcaoevidencia."', ".$etcevidencia.")";
		$db->executar($sql);
		
		if( $_REQUEST["entid_executor"] )
		{
			$sql = "INSERT INTO pde.monitorametaentidade(mnmid,entid,tpvid) VALUES(".$mnmid.",".$_REQUEST["entid_executor"].",1)";
			$db->executar($sql);
		}
	}
	
	/*** Validação ***/
	$sql = "delete from pde.monitoraetapascontrole where mnmid = $mnmid and tpvid = 2";
	$db->executar($sql);
	if( $_REQUEST["entid_validador"] )
	{
		if( $_REQUEST["opcao_evidencia_validacao"] == 'S' )
		{
			$etcopcaoevidencia = 't';
			$etcevidencia = "'".$_REQUEST["etcevidenciavalidacao"]."'";
		}
		else
		{
			$etcopcaoevidencia = 'f';
			$etcevidencia = 'null';
		}
				
		$sql = "INSERT INTO pde.monitoraetapascontrole (tpvid,mnmid,mecopcaoevidencia,mecevidencia)
				VALUES (2, ".$mnmid.", '".$etcopcaoevidencia."', ".$etcevidencia.")";
		$db->executar($sql);
			
		if( $_REQUEST["entid_validador"] ){
			$sql = "INSERT INTO pde.monitorametaentidade(mnmid,entid,tpvid) VALUES(".$mnmid.",".$_REQUEST["entid_validador"].",2)";;
			$db->executar($sql);
		}
	}
	
	$db->commit();
	echo "<script>
			alert('Dados Gravados com sucesso');
			window.location='estrategico.php?modulo=principal/atividade_estrategico/meta_checklist&acao=A&atiid=$atiid&micid=$micid';
		  </script>";
	exit;
		
}

function listarMonitoraMetas($atiid,$micid)
{
	global $db;
	$sql = "select
				CASE WHEN met.metid IS NOT NULL 
					THEN
						'<img src=\"../imagens/gif_inclui.gif\" title=\"Adicionar valor da meta\" class=\"link img_middle\" onclick=\"addValorMonitoraMeta(' || met.metid || ')\"  />
						<img src=\"../imagens/alterar.gif\" title=\"Editar Meta\" class=\"link img_middle\" onclick=\"editarMonitoraMeta(' || met.mnmid || ')\"  />
						<img src=\"../imagens/excluir.gif\" title=\"Excluir meta\" class=\"link img_middle\" onclick=\"excluirMonitoraMeta(' || met.mnmid || ')\"  />'
					ELSE
						'<img src=\"../imagens/alterar.gif\" title=\"Editar Meta\" class=\"link img_middle\" onclick=\"editarMonitoraMeta(' || met.mnmid || ')\"  />
						<img src=\"../imagens/excluir.gif\" title=\"Excluir meta\" class=\"link img_middle\" onclick=\"excluirMonitoraMeta(' || met.mnmid || ')\"  />'
				END as acao,
				mnmdsc,
				mtmdsc,
				CASE WHEN mnmprazo IS NOT NULL and mtm.mtmid != 2
					THEN to_char(mnmprazo,'DD/MM/YYYY')
					ELSE 'N/A'
				END as data
			from
				pde.monitorameta met
			inner join
				pde.monitoratipometa mtm ON mtm.mtmid = met.mtmid
			where
				micid = $micid
			and
				mnmstatus = 'A'
			order by
				mnmordem,mnmdsc";
	$arrCab = array("Ação","Descrição da Meta","Tipo da Meta","Prazo");
	$db->monta_lista($sql,$arrCab,100,10,"N","center","N");
}

function recuparMonitoraMeta($mnmid)
{
	global $db;
	$sql = "	select
					*
				from
					pde.monitorameta
				where
					mnmid = $mnmid";
	$arrDados = $db->pegaLinha($sql);
	return $arrDados ? $arrDados : array();
}

function mascaraglobalMeta($value, $mask) {
	$casasdec = explode(",", $mask);
	// Se possui casas decimais
	if($casasdec[1])
		$value = sprintf("%01.".strlen($casasdec[1])."f", $value);

	$value = str_replace(array("."),array(""),$value);
	if(strlen($mask)>0) {
		$masklen = -1;
		$valuelen = -1;
		while($masklen>=-strlen($mask)) {
			if(-strlen($value)<=$valuelen) {
				if(substr($mask,$masklen,1) == "#") {
						$valueformatado = trim(substr($value,$valuelen,1)).$valueformatado;
						$valuelen--;
				} else {
					if(trim(substr($value,$valuelen,1)) != "") {
						$valueformatado = trim(substr($mask,$masklen,1)).$valueformatado;
					}
				}
			}
			$masklen--;
		}
	}
	return $valueformatado;
}

function recuparMonitoraMetaExecutor($mnmid)
{
	global $db;
	
	$sql = "select
				mecopcaoevidencia as evidencia_execucao,
				mecevidencia as etcevidenciaexecucao,
				ent.entid as entid_executor,
				ent.entnome as nome_executor,
				'(' || entnumdddcelular || ') ' ||entnumcelular as celular_executor
			from
				pde.monitoraetapascontrole eta
			inner join
				pde.monitorametaentidade me ON me.mnmid = eta.mnmid and eta.tpvid = me.tpvid
			inner join
				entidade.entidade ent ON ent.entid = me.entid
			where
				eta.mnmid = $mnmid
			and
				eta.tpvid = 1
			order by
				mmeid desc";
	$arrDados = $db->pegaLinha($sql);
	return $arrDados ? $arrDados : array();
}

function recuparMonitoraMetaValidador($mnmid)
{
	global $db;
	
	$sql = "select
				mecopcaoevidencia as evidencia_validacao,
				mecevidencia as etcevidenciavalidacao,
				ent.entid as entid_validador,
				ent.entnome as nome_validador
			from
				pde.monitoraetapascontrole eta
			inner join
				pde.monitorametaentidade me ON me.mnmid = eta.mnmid and eta.tpvid = me.tpvid
			inner join
				entidade.entidade ent ON ent.entid = me.entid
			where
				eta.mnmid = $mnmid
			and
				eta.tpvid = 2
			order by
				mmeid desc";
	$arrDados = $db->pegaLinha($sql);
	return $arrDados ? $arrDados : array();
}

function excluirMonitoraMeta()
{
	global $db;
	
	extract($_POST);
	
	if($mnmid){
		$sql = "update
					pde.monitorameta
				set
					mnmstatus = 'I'
				where
					mnmid = $mnmid";
		$db->executar($sql);
		$db->commit();
		$db->sucesso("principal/atividade_estrategico/cadastro_meta","&atiid=$atiid&micid=$micid");
	}else{
		echo "<script>alert('Não foi possível excluir o Ítem de Check List.');window.location='estrategico.php?modulo=principal/atividade_estrategico/cadastro_meta&acao=A&atiid=$atiid&micid=$micid'</script>";
	}
	exit;
}

function wf_gerencimentoFluxoMonitoramentoEstrategico($tpdid, $docids = array(), $cxentrada = 'pendencias', $parametros = array(), $filtro_pendencias = null) {
	global $db;
		
	$sql = "SELECT * FROM workflow.tipodocumento WHERE tpdid='".$tpdid."'";
	$tipodocumento = $db->pegaLinha($sql);
	
	if($docids['pendencias']) {
		foreach($docids['pendencias'] as $docid) {
			
			$arrDocumentos[] = $db->pegaLinha("SELECT to_char(dmi.dmidataexecucao, 'DD-MM-YYYY') as dmidataexecucao,
													  to_char(dmi.dmidatavalidacao, 'DD-MM-YYYY') as dmidatavalidacao,
													  '<p>' || REPLACE(mnmdsc, 'Meta - ', '') || '</p>' as descricao_documento,
													  -- '<p>' || atidescricao || '</p>' || '<p>' || mnmdsc || '</p>' as descricao_documento,
													  esd.esddsc as descricao_estado,
													  doc.docid  as documento,
													  (SELECT '<span style=\"cursor:pointer\" onclick=\"abrePainelEstrategico(' || ati._atiprojeto || ')\" >' || proj.atidescricao || '</span>' 
													   FROM painel.acao aca2 
													   INNER JOIN pde.atividade ati2 ON ati2.atiacaid = aca2.acaid AND ati2.atistatus = 'A'
													   INNER JOIN pde.atividade proj ON proj.atiid = ati2._atiprojeto AND proj.atistatus = 'A'
													   WHERE ati2.atiid = ati._atiprojeto) as programa
											   FROM workflow.documento doc 
											   INNER JOIN workflow.estadodocumento 		esd ON esd.esdid = doc.esdid
											   INNER JOIN painel.detalhemetaindicador 	dmi on dmi.docid = doc.docid
											   INNER JOIN pde.monitorameta 				mnm ON dmi.metid = mnm.metid
											   INNER JOIN pde.monitoraitemchecklist 	mic ON mic.micid = mnm.micid AND mic.micstatus = 'A'
											   INNER JOIN painel.indicador 				ind ON ind.indid = mic.indid
											   INNER JOIN pde.atividade ati ON ati.atiid = mic.atiid											   
											   WHERE doc.docid = '".$docid."' 
											   ORDER BY dmidatameta");
			
		}
		
	}
	
	if($docids['atrazados']) {
		
		foreach($docids['atrazados'] as $docid) {
			
				$arrDocumentosAtraz[] = $db->pegaLinha("SELECT to_char(dmi.dmidataexecucao, 'DD-MM-YYYY') as dmidataexecucao,
															   to_char(dmi.dmidatavalidacao, 'DD-MM-YYYY') as dmidatavalidacao,
											  '<p>' || REPLACE(mnmdsc, 'Meta - ', '') || '</p>' as descricao_documento,
											  -- '<p>' || atidescricao || '</p>' || '<p>' || mnmdsc || '</p>' as descricao_documento,
											  esd.esddsc as descricao_estado,
											  doc.docid  as documento,
											  esd.esdid as estado,
											  (SELECT '<span style=\"cursor:pointer\" onclick=\"abrePainelEstrategico(' || ati._atiprojeto || ')\" >' || proj.atidescricao || '</span>' 
											   FROM painel.acao aca2 
											   INNER JOIN pde.atividade ati2 ON ati2.atiacaid = aca2.acaid AND ati2.atistatus = 'A'
											   INNER JOIN pde.atividade proj ON proj.atiid = ati2._atiprojeto AND proj.atistatus = 'A'
											   WHERE ati2.atiid = ati._atiprojeto) as programa
									   FROM workflow.documento doc 
									   INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
									   INNER JOIN painel.detalhemetaindicador dmi on dmi.docid = doc.docid
									   INNER JOIN pde.monitorameta mnm ON dmi.metid = mnm.metid
									   INNER JOIN pde.monitoraitemchecklist mic ON mic.micid = mnm.micid AND mic.micstatus = 'A'
									   INNER JOIN painel.indicador ind ON ind.indid = mic.indid
									   INNER JOIN pde.atividade ati ON ati.atiid = mic.atiid
									   WHERE doc.docid = '".$docid."' 
									   ORDER BY dmidatameta");
			
		}
		
	}
	
	if($docids['futuras']) {
		foreach($docids['futuras'] as $docid) {
			
			$arrDocumentosFut[] = $db->pegaLinha("SELECT to_char(dmi.dmidataexecucao, 'DD-MM-YYYY') as dmidataexecucao,
														 to_char(dmi.dmidatavalidacao, 'DD-MM-YYYY') as dmidatavalidacao,
													  '<p>' || REPLACE(mnmdsc, 'Meta - ', '') || '</p>' as descricao_documento,
													  --'<p>' || atidescricao || '</p>' || '<p>' || mnmdsc || '</p>' as descricao_documento,
													  esd.esddsc as descricao_estado,
													  doc.docid  as documento,
													  esd.esdid as estado,
													  (SELECT '<span style=\"cursor:pointer\" onclick=\"abrePainelEstrategico(' || ati._atiprojeto || ')\" >' || proj.atidescricao || '</span>' 
													   FROM painel.acao aca2 
													   INNER JOIN pde.atividade ati2 ON ati2.atiacaid = aca2.acaid AND ati2.atistatus = 'A'
													   INNER JOIN pde.atividade proj ON proj.atiid = ati2._atiprojeto AND proj.atistatus = 'A'
													   WHERE ati2.atiid = ati._atiprojeto) as programa
											   FROM workflow.documento doc 
											   INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
											   INNER JOIN painel.detalhemetaindicador dmi on dmi.docid = doc.docid
											   INNER JOIN pde.monitorameta mnm ON dmi.metid = mnm.metid
											   INNER JOIN pde.monitoraitemchecklist mic ON mic.micid = mnm.micid AND mic.micstatus = 'A'
											   INNER JOIN painel.indicador ind ON ind.indid = mic.indid
											   INNER JOIN pde.atividade ati ON ati.atiid = mic.atiid
											   WHERE doc.docid = '".$docid."' 
											   ORDER BY dmidatameta");
			
		}
		
	}
	
	$sql = "SELECT
				doc.docid as documento,
				to_char(dmi.dmidataexecucao, 'DD-MM-YYYY') as dmidataexecucao,
				to_char(dmi.dmidatavalidacao, 'DD-MM-YYYY') as dmidatavalidacao,
				'<p>' || REPLACE(mnmdsc, 'Meta - ', '') || '</p>' as docdsc,
				-- '<p>' || atidescricao || '</p>' || '<p>' || mnmdsc || '</p>' as docdsc,
				ed.esddsc as origem,
				ed2.esddsc as destino,
				ed3.esddsc as estado,
				ac.aeddscrealizada,
				us.usunome,
				to_char(hd.htddata, 'dd/mm/YYYY HH24:MI') as htddata,
				cd.cmddsc,
				  (SELECT '<span style=\"cursor:pointer\" onclick=\"abrePainelEstrategico(' || ati._atiprojeto || ')\" >' || proj.atidescricao || '</span>' 
				   FROM painel.acao aca2 
				   INNER JOIN pde.atividade ati2 ON ati2.atiacaid = aca2.acaid AND ati2.atistatus = 'A'
				   INNER JOIN pde.atividade proj ON proj.atiid = ati2._atiprojeto AND proj.atistatus = 'A'
				   WHERE ati2.atiid = ati._atiprojeto) as programa
			FROM workflow.historicodocumento hd
			INNER JOIN workflow.documento doc ON hd.docid = doc.docid 
			INNER JOIN workflow.acaoestadodoc ac ON	ac.aedid = hd.aedid
			INNER JOIN workflow.estadodocumento ed ON ed.esdid = ac.esdidorigem
			INNER JOIN workflow.estadodocumento ed2 ON ed2.esdid = ac.esdiddestino
			INNER JOIN workflow.estadodocumento ed3 ON ed3.esdid = doc.esdid
			INNER JOIN seguranca.usuario us ON us.usucpf = hd.usucpf
			LEFT JOIN workflow.comentariodocumento cd ON cd.hstid = hd.hstid
			INNER JOIN painel.detalhemetaindicador dmi ON dmi.docid = doc.docid 
			INNER JOIN pde.monitorameta mnm ON mnm.metid = dmi.metid
			INNER JOIN pde.monitoraitemchecklist mic on mnm.micid = mic.micid AND mic.micstatus = 'A'
			INNER JOIN painel.indicador ind ON ind.indid = mic.indid
			LEFT JOIN pde.atividade ati ON ati.atiid = mic.atiid
			WHERE hd.usucpf='".$_SESSION['usucpf']."' AND doc.tpdid='".TIPO_FLUXO_MONITORAMENTO."'
			$filtro_pendencias 
			ORDER BY hd.htddata DESC";
	
	$arrHistoricos = $db->carregar($sql);
	?>
<script>
	
function abrirDocumento(docid, obj) {

	var linha  = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;

	for(var i=0;i<tabela.rows.length;i++) {
		tabela.rows[i].style.backgroundColor='';
	}
	obj.parentNode.style.backgroundColor='#ffffcc';
		
	<? if($tipodocumento['tpdendereco']) { ?>
	$("[id^='detalhes_documento_']").html("");
	document.getElementById('detalhes_documento_' + docid).innerHTML="Carregando...";
	
	$.ajax({
   		type: "POST",
   		url: "<? echo $tipodocumento['tpdendereco']; ?>",
   		data: "docid="+docid,
   		async: false,
   		success: function(msg){
   			$("[id^='detalhes_documento_']").html("");
   			document.getElementById('detalhes_documento_' + docid).innerHTML=msg;
   			}
 		});
 	<? } else { ?>
 	
	$.ajax({
   		type: "POST",
   		url: "../geral/workflow/workflow_gerenciamento.php",
   		data: "docid="+docid,
   		async: false,
   		success: function(msg){
   			$("[id^='detalhes_documento_']").html("");
   			document.getElementById('detalhes_documento_' + docid).innerHTML=msg;
   			}
 		});
 		
 	
 	<? } ?>
}

</script>

<table width="100%">
<tr>
<td colspan="2"><h1><? echo $tipodocumento['tpddsc']; ?></h1></td>
</tr>
<tr>
	<td valign="top" width="15%">
	<h2>Caixa de Entrada</h2>
	<ul>
	  <li><a style="cursor:pointer;" href="estrategico.php?modulo=principal/atividade_estrategico/minhasPendencias&acao=A&atiidraiz=<?=$_REQUEST['atiidraiz']?>&cxentrada=pendencias"><? echo (($cxentrada == 'pendencias')?'<b>Pendências</b>':'Pendências'); ?> (<? echo count($docids['pendencias']); ?>)</a></li>
	  <li><a style="cursor:pointer;" href="estrategico.php?modulo=principal/atividade_estrategico/minhasPendencias&acao=A&atiidraiz=<?=$_REQUEST['atiidraiz']?>&cxentrada=resolvidas"><? echo (($cxentrada == 'resolvidas')?'<b>Resolvidas</b>':'Resolvidas'); ?> (<? echo (($arrHistoricos[0])?count($arrHistoricos):"0"); ?>)</a></li>
	  <li><a style="cursor:pointer;" href="estrategico.php?modulo=principal/atividade_estrategico/minhasPendencias&acao=A&atiidraiz=<?=$_REQUEST['atiidraiz']?>&cxentrada=futuras"><? echo (($cxentrada == 'futuras')?'<b>Futuras</b>':'Futuras'); ?> (<? echo count($docids['futuras']); ?>)</a></li>
	</ul>	
	</td>
	<td valign="top">
	
	<? if($cxentrada == 'pendencias') : ?>
	
	<h2>Lista de Pendências</h2>
	<table class="listagem" width="100%" cellSpacing="1" cellPadding="3">
	<thead>
	<tr>
	<!-- 
	<td align="center"><b>Data</b></td>
	 -->
	<td align="center"><b>Data da Meta</b></td>
	<td align="center"><b>Descrição</b></td>
	<td align="center"><b>Programa</b></td>
	<td align="center"><b>Situação Atual</b></td>
	<td align="center"><b>Ação</b></td>
	<? echo ((count($arrDocumentos) > 15)?"<td style=width:12px;>&nbsp;</td>":""); ?>
	</tr>
	</thead>
	<tbody style="<? echo ((count($arrDocumentos) > 15)?"height:250px;overflow-y:scroll;overflow-x:hidden;":""); ?>">
	<?
	if($arrDocumentos) {
		foreach($arrDocumentos as $documento) {
			echo "<tr>";
			echo "<td>".str_replace("-","/",$documento['dmidataexecucao'])."</td>";
			echo "<td style=cursor:pointer; onclick=abrirDocumento('".$documento['documento']."',this)>".$documento['descricao_documento']."</td>";
			echo "<td>".$documento['programa']."</td>";
			echo "<td>".$documento['descricao_estado']."</td>";
			echo "<td align=center><img align=absmiddle src=../../imagens/valida2.gif border=0 style=cursor:pointer; onclick=abrirDocumento('".$documento['documento']."',this) onmouseover=\"return escape('Clique aqui para alterar a situação atual.');\"> <img align=absmiddle style=cursor:pointer; src=../imagens/fluxodoc.gif onclick=\"window.open('../geral/workflow/historico.php?modulo=principal/tramitacao&acao=C&docid=".$documento['documento']."', 'alterarEstado','width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no');\" /></td>";
			echo "</tr>";
			echo "<tr><td colspan=3 id=\"detalhes_documento_{$documento['documento']}\" ></td></tr>";
			
		}
	} else {
		echo "<tr>";
		echo "<td colspan=4>Não existem pendências.</td>";
		echo "</tr>";
	}
	?>
	</tbody>
	</table>
	<table class="listagem" width="100%">
	<tr>
	<td><div id="detalhes_documento"><b>Clique na ação para detalhar</b></div></td>
	</tr>
	</table>
	
	<? elseif($cxentrada == 'atrazados') : ?>
	
	<h2>Lista de Futuras em atraso</h2>
	<table class="listagem" width="100%" cellSpacing="1" cellPadding="3">
	<thead>
	<tr>
	<!--
	<td align="center"><b>Data/Hora</b></td>
	 -->
	<td align="center"><b>Data da Meta</b></td>
	<td align="center"><b>Descrição</b></td>
	<td align="center"><b>Programa</b></td>
	<? echo (($parametros['capturar_responsavel'])?"<td align=\"center\"><b>Responsável</b></td>":""); ?>
	<td align="center"><b>Estado atual</b></td>
	<td align="center"><b>Possíveis ações realizadas</b></td>
	<td align="center"><b>Possíveis estados finais</b></td>
	<? echo ((count($arrDocumentosAtraz) > 15)?"<td style=width:12px;>&nbsp;</td>":""); ?>
	</tr>
	</thead>
	<tbody style="<? echo ((count($arrDocumentosAtraz) > 15)?"height:250px;overflow-y:scroll;overflow-x:hidden;":""); ?>">
	<?
	if($arrDocumentosAtraz) {
		foreach($arrDocumentosAtraz as $atrazado) {
			echo "<tr>";
			echo "<td>".str_replace("-","/",$atrazado['dmidataexecucao'])."</td>";
			echo "<td>".$atrazado['descricao_documento']."</td>";
			echo "<td>".$atrazado['programa']."</td>";
			if($parametros['capturar_responsavel']) echo "<td>".$parametros['capturar_responsavel']($atrazado['documento'])."</td>";
			echo "<td>".$atrazado['descricao_estado']."</td>";
			$possiveisAcoes = $db->carregar("SELECT aed.aeddscrealizar, esd.esddsc FROM workflow.acaoestadodoc aed
											 INNER JOIN workflow.estadodocumento esd ON esd.esdid = aed.esdiddestino  
											 WHERE aed.esdidorigem='".$atrazado['estado']."'");
			
			unset($possRealizadas,$possEstFinais);
			if($possiveisAcoes[0]) {
				foreach($possiveisAcoes as $key => $acoes) {
					$possRealizadas[] = ($key+1).". ".$acoes['aeddscrealizar'];
					$possEstFinais[]  = ($key+1).". ".$acoes['esddsc'];
				}
			}
			echo "<td>".(($possRealizadas)?implode("<br/>",$possRealizadas):"&nbsp;")."</td>";
			echo "<td>".(($possEstFinais)?implode("<br/>",$possEstFinais):"&nbsp;")."</td>";
			echo "</tr>";
		}
	} else {
		echo "<tr>";
		echo "<td colspan=6>Não existem pendências a serem repassadas.</td>";
		echo "</tr>";
	}
	?>
	</tbody>
	</table>
	
	<? elseif($cxentrada == 'resolvidas') : ?>
	
	<h2>Lista de Resolvidas</h2>
	<table class="listagem" width="100%" cellSpacing="1" cellPadding="3">
	<thead>
	<tr>
	<td align="center"><b>Data/Hora</b></td>
	<td align="center"><b>Descrição</b></td>
	<td align="center"><b>Programa</b></td>
	<td align="center"><b>Estado inicial</b></td>
	<td align="center"><b>Açao realizada</b></td>
	<td align="center"><b>Estado final</b></td>
	<td align="center"><b>Ação</b></td>
	<td align="center"><b>Estado atual</b></td>
	<? echo ((count($arrHistoricos) > 15)?"<td style=width:12px;>&nbsp;</td>":""); ?>
	</tr>
	</thead>
	<tbody style="<? echo ((count($arrHistoricos) > 15)?"height:250px;overflow-y:scroll;overflow-x:hidden;":""); ?>">
	<?
	if($arrHistoricos) {
		foreach($arrHistoricos as $historico) {
			echo "<tr>";
			echo "<td>".$historico['htddata']."</td>";
			echo "<td>".$historico['docdsc']."</td>";
			echo "<td>".$historico['programa']."</td>";
			echo "<td>".$historico['origem']."</td>";
			echo "<td>".$historico['aeddscrealizada']."</td>";
			echo "<td>".$historico['destino']."</td>";
			echo "<td align=center><img align=absmiddle style=cursor:pointer; src=../imagens/fluxodoc.gif onclick=\"window.open('../geral/workflow/historico.php?modulo=principal/tramitacao&acao=C&docid=".$historico['documento']."', 'alterarEstado','width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no');\" /></td>";
			echo "<td>".$historico['estado']."</td>";
			echo "</tr>";
		}
	} else {
		echo "<tr>";
		echo "<td colspan=8>Não existem pendências resolvidas.</td>";
		echo "</tr>";
	}
	?>
	</tbody>
	</table>
	
	<? elseif($cxentrada == 'futuras') : ?>
	
	<h2>Lista de Futuras</h2>
	<table class="listagem" width="100%" cellSpacing="1" cellPadding="3">
	<thead>
	<tr>
	<!-- 
	<td align="center"><b>Data</b></td>
	 -->
	<td align="center"><b>Data da Meta</b></td>
	<td align="center"><b>Descrição</b></td>
	<td align="center"><b>Programa</b></td>
	<td align="center"><b>Situação Atual</b></td>
	<td align="center"><b>Ação</b></td>
	<? echo ((count($arrDocumentosFut) > 15)?"<td style=width:12px;>&nbsp;</td>":""); ?>
	</tr>
	</thead>
	<tbody style="<? echo ((count($arrDocumentosFut) > 15)?"height:250px;overflow-y:scroll;overflow-x:hidden;":""); ?>">
	<?
	if($arrDocumentosFut) {
		foreach($arrDocumentosFut as $documento) {
			echo "<tr>";
			echo "<td>".str_replace("-","/",$documento['dmidataexecucao'])."</td>";
			echo "<td style=cursor:pointer; onclick=abrirDocumento('".$documento['documento']."',this)>".$documento['descricao_documento']."</td>";
			echo "<td>".$documento['programa']."</td>";
			echo "<td>".$documento['descricao_estado']."</td>";
			echo "<td align=center><img align=absmiddle src=../../imagens/valida2.gif border=0 style=cursor:pointer; onclick=abrirDocumento('".$documento['documento']."',this) onmouseover=\"return escape('Clique aqui para alterar a situação atual.');\"> <img align=absmiddle style=cursor:pointer; src=../imagens/fluxodoc.gif onclick=\"window.open('../geral/workflow/historico.php?modulo=principal/tramitacao&acao=C&docid=".$documento['documento']."', 'alterarEstado','width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no');\" /></td>";
			echo "</tr>";
			echo "<tr><td colspan=3 id=\"detalhes_documento_{$documento['documento']}\" ></td></tr>";
			
		}
	} else {
		echo "<tr>";
		echo "<td colspan=4>Não existem pendências.</td>";
		echo "</tr>";
	}
	?>
	</tbody>
	</table>
	<table class="listagem" width="100%">
	<tr>
	<td><div id="detalhes_documento"><b>Clique na ação para detalhar</b></div></td>
	</tr>
	</table>

	<? endif; ?>
	</td>
</tr>
</table>
<?
}

function alteraSituacaoAtividade($atiid, $esaid = '', $atiporcentoexec = ''){
	global $db;
	
	if( $esaid == 5 ) $filtro = ' , atidataconclusao=now() ';
	
	$sql = "UPDATE pde.atividade SET 
		  		esaid = $esaid,
		  		atiporcentoexec = '$atiporcentoexec'
		  		$filtro		 
			WHERE 
		  		atiid = $atiid";
	
	$db->executar( $sql );
	pegaPaiAtividade( $atiid );
}

function pegaPaiAtividade( $atiid ){
	global $db;
	$atiidPai = $db->pegaUm("select atiidpai from pde.atividade where atistatus = 'A' and atiid = ".$atiid);
	$boPai = $db->pegaUm("select atiidpai from pde.atividade where atistatus = 'A' and atiid = ".$atiidPai);
	if($boPai){
		verificaSituacaoAtividade( $atiidPai );
	} else {
		$db->commit();
		return true;
	}
}

function comboOpcaoCritico($name,$value = null,$obrigatorio = "N")
{
	global $db;

	$arrOpcoes[0] = array("codigo" => ">", "descricao" => "Maior que");
	$arrOpcoes[1] = array("codigo" => ">=", "descricao" => "Maior ou igual a");
	$arrOpcoes[2] = array("codigo" => "<", "descricao" => "Menor que");
	$arrOpcoes[3] = array("codigo" => "<=", "descricao" => "Menor ou igual a");
	$arrOpcoes[4] = array("codigo" => "=", "descricao" => "Igual a");
	
	$db->monta_combo($name,$arrOpcoes,"S",false,"","","","",$obrigatorio,"","",$value);
}

function verificaSituacaoAtividade( $atiid ){
	global $db;
	
	$totalFilhos = $db->pegaUm("select count(atiid) from pde.atividade a 
											where atiidpai = $atiid and a.atistatus = 'A'");	
	$totalFilhosPerc = $db->pegaUm("select count(atiid) from pde.atividade a 
											where atiidpai = $atiid and (atiporcentoexec <> 100 or atiporcentoexec is null) and a.atistatus = 'A'");	
	$totalPerc = $db->pegaUm( "select atiporcentoexec from pde.atividade where atiid = $atiid and atistatus = 'A'" );
	
	$atiporcentoexec = (float) 100 / (float) $totalFilhos;
	$atiporcentoexec = round($atiporcentoexec) + $totalPerc;
	$atiporcentoexec = round( $atiporcentoexec );
	
	#arredondando os valor inteiro
	$atiporcentoexec = $atiporcentoexec / 10;
	$atiporcentoexec = round( $atiporcentoexec ) * 10;
	
	$esaid = 2;
	if( $totalFilhosPerc == 0 ){
		$esaid = 5;
		$atiporcentoexec = 100;
	}
	
	/*$atiidNome = $db->pegaUm("select atidescricao from pde.atividade where atiid = ".$atiid);
	ver('Atividade Nome: '.$atiidNome.'<br>Atividade: '.$atiid.'<br>Porcentagem: '.$atiporcentoexec.
		'<br>Total Filhos: '.$totalFilhos.'<br>Filhos <> 100%: '.$totalFilhosPerc.'<br>Total Perc: '.$totalPerc );*/	
		
	alteraSituacaoAtividade( $atiid, $esaid, $atiporcentoexec );
}

function pegarFormatoInput($indid = null) {
	global $db;
	/*
	 * Verificando o tipo de unidade de medição do indicador
	 * regra 1: se for moeda (unmid=5), o formato dos campos devem ser ###.###.###,##
	 * regra 2: se for Inteiro (unmid=3), verificar se indqtdevalor == true, caso sim, mostrar os dois campos
	 */
	$indid = !$indid ? $_SESSION['indid'] : $indid;
	if($indid) {
		$ind = $db->pegaLinha("SELECT unmid, indqtdevalor FROM painel.indicador WHERE indid='".$indid."'");
		switch($ind['unmid']) {
			case '1':
				$formatoinput = array('mascara'             => '###,##',
									  'size'                => '7',
									  'maxlength'           => '6',
									  'label'               => 'Porcentagem',
									  'unmid'				=> $ind['unmid']);
				break;
			case '2':
				$formatoinput = array('mascara'             => '###.###,##',
									  'size'                => '11',
									  'maxlength'           => '10',
									  'label'               => 'Razão',
									  'unmid'				=> $ind['unmid']);
				break;
			case '4':
				$formatoinput = array('mascara'             => '###,##',
									  'size'                => '7',
									  'maxlength'           => '6',
									  'label'               => 'Indíce',
									  'unmid'				=> $ind['unmid']);
				break;
			case '5':
				$formatoinput = array('mascara'             => '###.###.###.###,##',
									  'size'                => '18',
									  'maxlength'           => '17',
									  'label'               => 'Valor',
									  'unmid'				=> $ind['unmid']);
				break;
			case '3':
				$formatoinput = array('mascara'             => '##########',
									  'size'                => '11',
									  'maxlength'           => '10',
									  'label'               => 'Quantidade',
									  'unmid'				=> $ind['unmid']);
				
				if($ind['indqtdevalor'] == "t") {
					// mostar os dois campos (quantidade e valor)
					$formatoinput['campovalor'] = array('mascara'             => '###.###.###.###,##',
									  					'size'                => '18',
									  					'maxlength'           => '17',
									  					'label'               => 'Valor',
									  					'unmid'				  => $ind['unmid']);
				}
				break;
			default:
				$formatoinput = array('mascara'             => '##########',
									  'size'                => '11',
									  'maxlength'           => '10',
									  'label'               => 'Quantidade',
									  'unmid'				=> $ind['unmid']);
		}
		return $formatoinput;
	} else {
		echo "<p align='center'>Problemas na identificação do indicador. <b><a href=\"?modulo=inicio&acao=C\">Clique aqui</a></b> e refaça os procedimentos.</p>";
		exit;
	}

}

function naoImportaIndicador()
{
	$_SESSION['estrategico']['nao_importar_indicador'] = true;
}

function importaIndicador()
{
	global $db;
	$indid = $_POST['indid'];
	$atiid=  $_GET['atiid'];
	
	$sql = "insert into painel.indicador 
			( unmid,cliid,tpiid,secid,acaid,perid,estid,colid,usucpf,indnome,indobjetivo,indformula,indtermos,
			indfontetermo,indnormalinicio,indnormalfinal,indatencaoinicio,indatencaofinal,indcriticoinicio,indcriticofinal,
			indmetavalor,indmetadata,indstatus,indmetadatalimite,indmetavalorlimite,umeid,regid,exoid,indqtdevalor,
			indpermiteagregacao,secidgestora,indpublicado,indescala,indobservacao,indvispadrao,indrelatorio,
			peridatual,indcumulativovalor,indcumulativo,indpublico,indhomologado)
				
			 select unmid,cliid,tpiid,secid,acaid,perid,estid,colid,usucpf,indnome,indobjetivo,indformula,indtermos,
			indfontetermo,indnormalinicio,indnormalfinal,indatencaoinicio,indatencaofinal,indcriticoinicio,indcriticofinal,
			indmetavalor,indmetadata,'I',indmetadatalimite,indmetavalorlimite,umeid,regid,exoid,indqtdevalor,
			indpermiteagregacao,secidgestora,indpublicado,indescala,indobservacao,indvispadrao,indrelatorio,
			peridatual,indcumulativovalor,indcumulativo,indpublico,true from painel.indicador where indid = $indid
				returning indid";
	
	$novo_indid = $db->pegaUm($sql);
	
	$sqlOrdem = "select max(micordem) from pde.monitoraitemchecklist where atiid = $atiid and micstatus = 'A'";
	$micordem = $db->pegaUm($sqlOrdem);
	$micordem = !$micordem ? 1 : (int)$micordem+1;
	
	$sql = "insert into 
				pde.monitoraitemchecklist 
			(atiid,micordem,micstatus,indid,micimportado)
				values
			($atiid,$micordem,'A',$novo_indid,true)
				returning micid";
	$micid = $db->pegaUm($sql);
	
	$db->commit();
	echo $micid;
}

function salvarPendenciaExecutor($dmiid)
{
	global $db;
	
	$sql = "select 
			mnm.mnmid,
			mnm.mnmdsc,	
			(select entid from pde.monitorametaentidade mme where mme.mnmid =  exec.mnmid and mme.tpvid = 1 order by mmeid desc limit 1) as entid_executor,
			(select entid from pde.monitorametaentidade mme where mme.mnmid =  vali.mnmid and mme.tpvid = 2 order by mmeid desc limit 1) as entid_validador,
			exec.mecopcaoevidencia as executor_evidencia,
			exec.mecevidencia as executor_nomeevidencia,
			vali.mecopcaoevidencia as validador_evidencia,
			vali.mecevidencia as validador_nomeevidencia,
			to_char(dmidataexecucao,'DD/MM/YYYY') as dmddatainiexecucao,
			dmi.docid
		from 
			painel.detalhemetaindicador dmi
		inner join
			pde.monitorameta mnm ON dmi.metid = mnm.metid
		inner join
			pde.monitoraetapascontrole exec ON exec.mnmid = mnm.mnmid and exec.tpvid = 1
		left join
			pde.monitoraetapascontrole vali ON vali.mnmid = mnm.mnmid and vali.tpvid = 2
		where 
			dmi.dmiid = $dmiid
		";
	
	$arrDados = $db->pegaLinha($sql);
	
	//Cria o Documento
	if(!$arrDados['docid']){
		include APPRAIZ . 'includes/workflow.php';
		$tpdid 	= TIPO_FLUXO_MONITORAMENTO;
		$docdsc = "Meta - {$arrDados['mnmdsc']} de {$arrDados['dmddatainiexecucao']}.";
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );
		$sql = "update painel.detalhemetaindicador set docid = $docid where dmiid = $dmiid";
		$db->executar($sql);
	}
	
	
	if($arrDados['entid_executor']){
		$sql = "select 
					mhmid 
				from 
					pde.monitorahistoricometa 
				where 
					mhmacao = 'EXECUTOR'
				and 
					mnmid = {$arrDados['mnmid']}
				and
					entid = {$arrDados['entid_executor']}";
		$mhmid = $db->pegaUm($sql);
		
		if(!$mhmid){
		
			$db->executar("INSERT INTO pde.monitorahistoricometa 
										(mhmacao, mnmid, usucpf, mnmdsc, prdid,
										micid, docid, mhmprazo, mhmordem,entid) 
								   SELECT
								   			'EXECUTOR', ".$arrDados['mnmid'].", '".$_SESSION['usucpf']."', mnmdsc,
											NULL, micid, docid, NULL,mnmordem,{$arrDados['entid_executor']}
								   		FROM
								   			pde.monitorameta
								   		WHERE
								   			mnmid = ".$arrDados['mnmid']);
		}
	}
	
	if($arrDados['entid_validador']){
		$sql = "select 
					mhmid 
				from 
					pde.monitorahistoricometa 
				where 
					mhmacao = 'VALIDADOR'
				and 
					mnmid = {$arrDados['mnmid']}
				and
					entid = {$arrDados['entid_executor']}";
		$mhmid = $db->pegaUm($sql);
		if(!$mhmid){
			$db->executar("INSERT INTO pde.monitorahistoricometa 
										(mhmacao, mnmid, usucpf, mnmdsc, prdid,
										micid, docid, mhmprazo, mhmordem,entid) 
								   SELECT
								   			'VALIDADOR', ".$arrDados['mnmid'].", '".$_SESSION['usucpf']."', mnmdsc,
											NULL, micid, docid, NULL,mnmordem,{$arrDados['entid_validador']}
								   		FROM
								   			pde.monitorameta
								   		WHERE
								   			mnmid = ".$arrDados['mnmid']);
		}
	}
	$db->commit();
}

function retornaCorMeta($arrDados,$valor)
{
	if($arrDados){
		foreach($arrDados as $n => $dado){
			if($n == 0){
				if($valor >= 0 && $valor <= $dado['fim']){
					return $dado['bgcolor'];
				}
			}else{
				if($valor >= $dado['inicio'] && $valor <= $dado['fim']){
					return $dado['bgcolor'];
				}
			}
		}
	}else{
		return false;
	}
}

function recuperaDadosProjeto($atiid = null)
{
	global $db;
	
	if(!$atiid){
		return array();
	}else{
		$sql = "select
					atiacaid as acaid_indicador,
					exoid
				from
					pde.atividade
				where
					atiid = $atiid";
		$arrDados = $db->pegaLinha($sql);
		
		$sql = "select atiid from pde.agendaatividade where atiid = $atiid";
		$agenda = $db->pegaUm($sql);
		if($agenda){
			$arrDados['agenda_projeto'] = $agenda;
		}
		
		return $arrDados;
	}
}

function verificaAtividadePai($atiid)
{
	global $db;
	$sql = "select
				ati.atiid,
				ati.atiidpai,
				ati._atinumero,
				ati._atiprofundidade,
				ati.atidescricao
			from
				pde.atividade ati
			where
				atiid = $atiid
			and
				ati.atistatus = 'A'";
	$arrAtividade = $db->pegaLinha($sql);
	if($arrAtividade){
		if($arrAtividade['atiidpai']){
			verificaAtividadePai($arrAtividade['atiidpai']);
		}
		if(!$arrAtividade['_atiprofundidade']){
			
		}else{
			echo "<p><img style=\"margin-left:".($arrAtividade['_atiprofundidade']*10)."px\" src=\"../imagens/seta_retorno.gif\" /> <b>".$arrAtividade['_atinumero']." ".$arrAtividade['atidescricao']."</b></p>";	
		}
	}
}

function alteraOrdemItemChecklist()
{
	global $db;
	$micid = $_POST['micid'];
	$tipo = $_POST['tipo'];
	$sql = "select atiid,micordem from pde.monitoraitemchecklist where micid = $micid";
	$arrDados = $db->pegaLinha($sql);
	
	if(!$arrDados['micordem']){
		$sql = "select micid from pde.monitoraitemchecklist where atiid = {$arrDados['atiid']} and micstatus = 'A'";
		$arrMic = $db->carregar($sql);
		if($arrMic){
			$n = 1;
			foreach($arrMic as $m){
				$sqlU.="update pde.monitoraitemchecklist set micordem = $n where micid = {$m['micid']};";
				$n++;
			}
		}
		if($sqlU){
			$db->executar($sqlU);
			$db->commit();
		}
	}
	
	if($tipo == "cima"){
		$ordem = $arrDados['micordem'] ? (int)$arrDados['micordem']-1 : 1;
		$ordem2 = $arrDados['micordem'] ? (int)$arrDados['micordem'] : 2;
	}else{
		$ordem = $arrDados['micordem'] ? (int)$arrDados['micordem']+1 : 2;
		$ordem2 = $arrDados['micordem'] ? (int)$arrDados['micordem'] : 1;
	}
	$sql = "update pde.monitoraitemchecklist set micordem = $ordem where micid = $micid;
			update pde.monitoraitemchecklist set micordem = $ordem2 where micid != $micid and atiid = {$arrDados['atiid']} and micordem = $ordem";
	echo $sql;
	$db->executar($sql);
	$db->commit();
}

function listaMetasPainelEstrategico($atiprojeto)
{
	global $db;
	
	$arrWhere[] = "ati._atiprojeto = '$atiprojeto'";
	
	$sql = "select distinct
				ati.atiid,
				ati.atiidpai,
				ati.atidescricao,
				ati._atinumero as num,
				ati._atiprofundidade,
				ati.atiordem
			from
				pde.atividade ati
			inner join
				pde.monitoraitemchecklist mic ON mic.atiid = ati.atiid
			inner join
				painel.indicador ind ON ind.indid = mic.indid
			inner join
				painel.metaindicador met ON met.indid = ind.indid
			inner join
				painel.detalhemetaindicador dmi ON met.metid = dmi.metid
			inner join
				pde.atividade proj ON proj.atiid = ati._atiprojeto
				".($arrInner ? implode("",$arrInner) : "")."
			where
				ati.atistatus = 'A'
			and
				mic.micstatus = 'A'
			and
				ind.indstatus = 'I'
			and
				met.metstatus = 'A'
			and
				dmi.dmistatus = 'A'
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			order by
				ati._atinumero,ati.atiordem";
	$arrAtividades = $db->carregar($sql);
	$arrAtividades = !$arrAtividades ? array() : $arrAtividades;
	?>
		<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="5" align="center" >
			<tr>
				<td class="SubtituloTabela center bold" >Descrição</td>
				<td class="SubtituloTabela center bold" >Tipo</td>
				<td class="SubtituloTabela center bold" >Referência</td>
				<td class="SubtituloTabela center bold" >Executado/Validado</td>
				<td class="SubtituloTabela center bold" >Meta</td>
			</tr>
	<?php
	$n=0;foreach($arrAtividades as $ati):  ?>
		<?php $n++;$cor = $n%2 == 1 ? "#ffffff" : "" ?>
		<tr bgcolor="<?php echo $cor ?>" >
			<td>
				<?php verificaAtividadePai($ati['atiid']) ?>
			</td>
			<td class="center" >-</td>
			<td class="center" >-</td>
			<td class="center" >-</td>
			<td class="center" >-</td>
		</tr>
		<?php $sql = "select distinct
							met.metid,
							met.indid,
							met.metdesc as mnmdsc,
							ind.umeid,
							mnm.mtmid
						from
							pde.atividade ati
						inner join
							pde.monitoraitemchecklist mic ON mic.atiid = ati.atiid
						inner join
							painel.indicador ind ON ind.indid = mic.indid
						inner join
							painel.metaindicador met ON met.indid = ind.indid
						inner join
							pde.monitorameta mnm ON mnm.metid = met.metid
						inner join
							painel.detalhemetaindicador dmi ON met.metid = dmi.metid
						--inner join
							--painel.seriehistorica seh ON seh.dmiid = dmi.dmiid
						inner join
							pde.atividade proj ON proj.atiid = ati._atiprojeto
							".($arrInner ? implode("",$arrInner) : "")."
						where
							ati.atistatus = 'A'
						and
							mic.micstatus = 'A'
						and
							ind.indstatus = 'I'
						--and
							--ind.unmid != ".UNIDADEMEDICAO_BOLEANA."
						and
							met.metstatus = 'A'
						and
							dmi.dmistatus = 'A'
						and
							dmi.docid is not null
						and
							ati.atiid = {$ati['atiid']}
						".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
						order by
							met.metid";
			
			$arrMetas = $db->carregar($sql);
			$arrMetas = !$arrMetas ? array() : $arrMetas;
			
			foreach($arrMetas as $met):
				$n++;$cor = $n%2 == 1 ? "#ffffff" : "";
				$sql = "select 
							dmi.dmiid,
							dmi.dmiqtde as meta,
							dmi.dmivalor,
							dmi.docid,
							dpe.dpedsc as referencia,
							dmi.dmidataexecucao,
							dmi.dmidatavalidacao::date as dmidatavalidacao,
							dmi.dmdestavel as mnmqtdestavel,
							dmi.dmdcritico as mnmqtdcritico,
							mtidsc,
							mic.micid
						from 
							painel.detalhemetaindicador dmi
						inner join
							pde.monitorameta mnm ON mnm.metid = dmi.metid
						inner join
							pde.monitoraitemchecklist mic ON mic.micid = mnm.micid
						left join
							 pde.monitoratipoindicador mti ON mti.mtiid = mic.mtiid
						inner join
							painel.detalheperiodicidade dpe ON dpe.dpeid = dmi.dpeid
						inner join
							workflow.documento doc ON doc.docid = dmi.docid --and doc.esdid = ".WK_ESTADO_DOC_FINALIZADO."
						where 
							dmi.dmistatus = 'A' 
						and
							mnm.metid = {$met['metid']}
						order by
							--dpedatafim desc
							dmi.dmidatameta 
						limit 1";
				$arrDados = $db->pegaLinha($sql);
				if($arrDados['dmidataexecucao'] && $met['indid'] && $arrDados['dmiid']){
					$sql = "select 
								sehvalor as mnmvalor,
								sehqtde as mnmqtd,
								sehdtcoleta::date as mvddata 
							from
								painel.seriehistorica
							where
								indid = {$met['indid']}
							and
								dmiid = {$arrDados['dmiid']}
							--and
								--sehdtcoleta::date between '{$arrDados['dmddatainiexecucao']}' and '{$arrDados['dmddatafimexecucao']}'
							and
								sehstatus = 'A'
							order by
								sehid desc
							limit
								1";
					$arrExecucao = $db->pegaLinha($sql);
				}else{
					$arrExecucao = false;
				}
				
				//Verifica se a execução foi realizada em atrazo
				if($arrExecucao){
					if((int)str_replace("-","",$arrExecucao['mvddata']) > (int)str_replace("-","",$arrDados['dmddatafimexecucao'])){
						$atraso = " <span style=\"cursor:pointer\" title='Execução realizada com atraso' >(A)</span>";
					}else{
						$atraso = "";	
					}
				}else{
					$atraso = "";
				}
				
				/*
				$sql = "select
							COALESCE(mnmqtd,0) as mnmqtd,
							mnmvalor,
							mvddata
						from
							pde.monitoravalidacao
						where
							mnmid = {$met['mnmid']}
						and
							tpvid = 2";
				$arrExecucao = $db->pegaLinha($sql);
				*/
				
				if($arrExecucao['mnmqtd'] && $arrDados['meta']){
					
					$arrExecucao['mnmqtd'] = (float)$arrExecucao['mnmqtd'];
					$arrDados['meta'] = (float)$arrDados['meta'];
					$porcentagem = round((($arrExecucao['mnmqtd'] ? $arrExecucao['mnmqtd'] : 1)/($arrDados['meta'] ? $arrDados['meta'] : 1))*100,2);
				}else{
					$porcentagem = 0;
				}
				
				//Verifica se o executado é maior que a meta
				if($arrExecucao['mnmqtd'] > $arrDados['meta']){
					$porcentagem = 100;
				}
				
				
				if($ind['estid'] == 2){ //Menor melhor
					$img_indicador = "indicador-vermelha.png";
				}else{
					$img_indicador = "indicador-verde.png";
				}

				if($ind['estid'] == 2 && $arrDados['mnmqtdestavel'] && $arrDados['mnmqtdcritico']){ //Menor melhor
					$arrMedidor[0] = array("inicio" => 0, "fim" => $arrDados['mnmqtdestavel'], "cor" => "#80BC44", "bgcolor" => "#80BC44");
					$arrMedidor[1] = array("inicio" => $arrDados['mnmqtdestavel'], "fim" => $arrDados['mnmqtdcritico'], "cor" => "#FFFF00", "bgcolor" => "#FFC211");
					$arrMedidor[2] = array("inicio" => $arrDados['mnmqtdcritico'], "fim" => 100, "cor" => "#E95646", "bgcolor" => "#E95646");
				}elseif($ind['estid'] != 2 && $arrDados['mnmqtdestavel'] && $arrDados['mnmqtdcritico']){ //Maior Melhor
					$arrMedidor[0] = array("inicio" => 0, "fim" => $arrDados['mnmqtdcritico'], "cor" => "#E95646", "bgcolor" => "#E95646");
					$arrMedidor[1] = array("inicio" => $arrDados['mnmqtdcritico'], "fim" => $arrDados['mnmqtdestavel'], "cor" => "#FFC211", "bgcolor" => "#FFC211");
					$arrMedidor[2] = array("inicio" => $arrDados['mnmqtdestavel'], "fim" => 100, "cor" => "#80BC44", "bgcolor" => "#80BC44");
				}
				$valor = $porcentagem;
				
				?>
				<tr bgcolor="<?php echo $cor ?>" >
					<td>
						<img style="margin-left:<?php echo (($ati['_atiprofundidade']+1)*10) ?>px" src="../imagens/seta_retorno.gif" /> 
						<?php echo $met['mnmdsc']?> 
						<img class="img_middle link" onclick="wf_exibirHistorico(<?php echo $arrDados['docid']?>)" src="../imagens/fluxodoc.gif" />
						<?php if($met['unmid'] != UNIDADEMEDICAO_BOLEANA && $met['mtmid'] != 1): ?>
							<img class="img_middle link" onclick="exibeGraficoMeta(<?php echo $arrDados['micid']?>)" src="../imagens/graph.gif" />
						<?php endif; ?>
					</td>
					<td align="center">
						<?php echo $arrDados['mtidsc'] ? $arrDados['mtidsc'] : "-" ?>
					</td>
					<td class="center" >
						<?php echo $arrDados['referencia'] ? $arrDados['referencia'] : "-" ?>
					</td>
					<?php if($met['unmid'] == UNIDADEMEDICAO_BOLEANA || $met['mtmid'] == 1): ?>
						<?php 
							if($arrDados['referencia'] && $arrExecucao['mvddata']){
								if(strlen($atraso) > 5){
									$cor_td = "style='color:#FFFFFF;font-weight:bold;font-size:12px;border:solid 1px #000000;' bgcolor='#E95646' ";
								}else{
									$cor_td = "style='color:#FFFFFF;font-weight:bold;font-size:12px;border:solid 1px #000000;' bgcolor='#80BC44' ";
								}
							}else{
								$cor_td = "";
							}
						
						?>
						<td <?php echo $cor_td ?> class="center" ><?php echo $arrDados['referencia'] && $arrExecucao['mvddata'] ? ($arrExecucao['mvddata'] ? " em ".formata_data($arrExecucao['mvddata']) : "") : "-" ?><?php echo $atraso ?></td>
						<td class="center" ></td>
					<?php else: ?>
						<td  style="color:#FFFFFF;font-weight:bold;font-size:12px;<?php echo $arrDados['referencia'] && $arrExecucao && $arrMedidor ? "border:solid 1px #000000" : "" ?>" bgcolor="<?php echo $arrDados['referencia'] && $arrExecucao && $arrMedidor ? retornaCorMeta($arrMedidor,$valor) : "" ?>" class="direita numero" ><?php echo $arrExecucao['mnmqtd'] ? "<span class='font_numero' >".number_format($arrExecucao['mnmqtd'],0,'','.')."</span> <img src='../imagens/$img_indicador' /> (".str_replace(".",",",$porcentagem)."%) <br />".($arrExecucao['mvddata'] ? " em ".formata_data($arrExecucao['mvddata']) : "") : "<center>-<center>"?><?php echo $atraso ?></td>
						<td class="direita numero" ><?php echo $arrDados['meta'] ? "<span class='font_numero' >".number_format($arrDados['meta'],0,'','.')."</span>".($arrDados['referencia'] ? "<br />".$arrDados['referencia'] : "") : "<center>-<center>" ?></td>
					<?php endif; ?>
				</tr>
			<?php endforeach; ?>
	<?php endforeach;
	?></table> <?php
}

function recuperaIndicadoresVinculados($arrInd = array())
{
	global $db;
	//Pegar todos os indicadores do projeto
	
	if ($_REQUEST['micestrategico'])
		$where = 'and micestrategico';
	
	if(!$arrInd){
		$sql = "select distinct
					mic.indid
				from
					pde.monitoraitemchecklist mic 
				inner join
					pde.atividade ati ON ati.atiid = mic.atiid
				where
					ati._atiprojeto = {$_REQUEST['atiprojeto']}
				and
					mic.micstatus = 'A' $where ";
		$arrIndicadores = $db->carregarColuna($sql);
	}else{
		$arrIndicadores = $arrInd;
	}
	
	
	
	if($arrIndicadores){
		$sql = "select 
					indid
				from 
					painel.indicadoresvinculados 
				where 
					idvstatus = 'A' and 
					indidvinculo in (".implode(",",$arrIndicadores).")
				and
					indid not in (".implode(",",$arrIndicadores).")";
		$arrIndicadores2 = $db->carregar($sql);
		if($arrIndicadores2){
			foreach($arrIndicadores2 as $ind2){
				if(!in_array($ind2['indid'],$arrIndicadores)){
					$arrIndicadores[] = $ind2['indid'];
				}
			}
		}
		
		/*$sql = "select 
					indidvinculo as indid
				from 
					painel.indicadoresvinculados 
				where 
					idvstatus = 'A' and 
					indid in (".implode(",",$arrIndicadores).")
				and
					indidvinculo not in (".implode(",",$arrIndicadores).")";
		$arrIndicadores3 = $db->carregar($sql);
		if($arrIndicadores3){
			foreach($arrIndicadores3 as $ind3){
				if(!in_array($ind3['indid'],$arrIndicadores)){
					$arrIndicadores[] = $ind3['indid'];
				}
			}
		}*/
	}
	
	if($arrInd == $arrIndicadores){
		return $arrIndicadores;
	}else{
		return recuperaIndicadoresVinculados($arrIndicadores);
	}
}

function atualizaOrdem( $dados){
	
	global $db;
	
	extract($dados);
	
	$sql = "UPDATE painel.indicador SET
				indordempainel = $ordem
			WHERE
				indid = $indid";
	$db->executar($sql);
	$db->commit();
}

function listaLogMetas( $request ){
	
	global $db;
	
	$sql = "";
}

function transformaFiltroIndicadorParaArray($textFiltro){
	$textFiltro = trim( $textFiltro );
	if ( empty( $textFiltro ) ){
		return array();
	}
	
	$textFiltro = explode(";", $textFiltro);
	
	foreach ( $textFiltro as $item ){
		list($k, $v) = explode("=", $item);
		switch ( true ){
			case in_array($k, array('tidid_1', 'tidid_2')):
				$arFiltro[$k] = explode(",", $v);
				break;
			default:
				$arFiltro[$k] = $v;
				break;
		}
	}
	
	return $arFiltro;
}

function listaIndicadoresPainelMonitoramento($indid = null, $n = 0,$nivel = 0,$nivelInicial = 2, $filhos = true,$cor_padrao = null)
{
	global $db;
	$n = $n+1;
	$nivel=0;
			
	$arrIndicadores = recuperaIndicadoresVinculados();
	
//	dbg($arrIndicadores, d);
	
	if(!$arrIndicadores){
		return false;
	}
	
	if($_POST['mtiid']){
		$arrWhere[] = "i.mtiid = {$_POST['mtiid']}";
	}

	if($_REQUEST["atiidraiz"]){
		$arrWhere[] = "i.indid in (select mic.indid from pde.monitoraitemchecklist mic inner join pde.atividade ati ON ati.atiid = mic.atiid where ati._atinumero like('{$_REQUEST["atiidraiz"]}%'))";
	}
	
	//Perfis que pode editar a data de execução
	$arrPerfil = pegaPerfilGeral();
	if($db->testa_superuser() || in_array(PERFIL_DATA_EXECUCAO,$arrPerfil)){
		$permiteEditarData = true;
	}
	
	if($_POST['indestrategico']){
		$arrWhere[] = "(i.indestrategico = 't' or idc.idvestrategico = 't' )";
	}
	
	if($_POST['micetapa']){
		$arrWhere[] = "(mic.micetapa = '{$_REQUEST['micetapa']}' )";
	}
	
	$sql = "SELECT DISTINCT
				0 as mtinivel, 
				mti.mtiid,
				dmi.dmiid,
				sh.sehid,
				mti.mtidsc,
				i.indid,
				i.unmid,
				indnome,
				estid,
				dmidatameta,
				micordem,
				_atinumero,
				--CASE WHEN idc.idvmeta IS NULL THEN
					CASE WHEN mti.mtiid = 2 
						THEN count(distinct met.metid)
						ELSE 1
					END
				--ELSE idc.idvmeta END
				as qtd_meta,
				indordempainel,
				doc.esdid,
				hst2.aedid,
				mvd.mvdsituacao
				,idc.idvmeta, idc.idvfiltro, sh.sehqtde, met.metid
				,(
					select 
						count(*)
					from
						painel.detalhemetaindicador dmi2
					inner join
						painel.detalheperiodicidade dpe2 ON dmi2.dpeid = dpe2.dpeid					
					where
						dmi2.metid = met.metid
					and
						dmi2.dmistatus = 'A'					
					
				) as total_metas,
				(SELECT  umedesc FROM painel.unidademeta where umeid = i.umeid ) as umedesc,
				ent.entnome
			FROM 
				painel.indicador i
			INNER JOIN pde.monitoratipoindicador 	mti ON mti.mtiid = i.mtiid
			INNER JOIN painel.metaindicador 		met ON met.indid = i.indid
			INNER JOIN painel.detalhemetaindicador 	dmi ON dmi.metid = met.metid
			LEFT  JOIN painel.seriehistorica 		sh ON sh.dmiid = dmi.dmiid
			LEFT  JOIN workflow.documento		 	doc ON doc.docid = dmi.docid
			--LEFT  JOIN (SELECT max(hstid) as hstid, docid FROM workflow.historicodocumento GROUP BY docid ) hst ON hst.docid = doc.docid
			LEFT  JOIN workflow.historicodocumento  hst2 ON hst2.hstid = doc.hstid
 			LEFT  JOIN pde.monitoraitemchecklist 	mic ON mic.indid = i.indid
			LEFT  JOIN pde.atividade 				ati ON ati.atiid = mic.atiid
			LEFT  JOIN pde.monitorameta				mnm ON mnm.metid = met.metid
			--LEFT  JOIN pde.monitorametaentidade     mme ON mme.mnmid = mnm.mnmid 
			LEFT JOIN
				(SELECT
					m1.mnmid, m1.entid
				FROM
					pde.monitorametaentidade m1
				WHERE
					m1.mmeid = (select m2.mmeid from pde.monitorametaentidade m2 where m2.mnmid = m1.mnmid and m2.tpvid = 1 order by m2.mmeid desc limit 1)) AS mme ON mme.mnmid = mnm.mnmid
			LEFT JOIN entidade.entidade 			ent on ent.entid = mme.entid
			LEFT  JOIN pde.monitoravalidacao		mvd ON mvd.mnmid = mnm.mnmid
			LEFT  JOIN painel.indicadoresvinculados idc ON idc.indidvinculo = i.indid
			LEFT  JOIN painel.unidademeta 		    ume on ume.umeid = i.umeid 	
						AND idc.idvstatus = 'A'
			WHERE
				i.indid in(".implode(",",$arrIndicadores).")
				 AND dmi.dmistatus = 'A'
			".($arrWhere ? " AND ".implode(" and ",$arrWhere) : "")."
			GROUP BY
				mtinivel,
				mti.mtiid, 
				dmi.dmiid,
				sh.sehid,
				mti.mtidsc,
				i.mtiid,
				i.indid,
				i.unmid,
				indnome,
				estid,
				dmidatameta,
				micordem,
				_atinumero,
				indordempainel,
				doc.esdid,
				hst2.aedid,
				mvd.mvdsituacao
				,idc.idvmeta, idc.idvfiltro, sh.sehqtde, met.metid, i.umeid, ent.entnome
			ORDER BY
				mtiid,indordempainel,_atinumero,micordem,dmidatameta";
//ver($sql);
// die;
	$arrDados = $db->carregar($sql);

	$arrIndicadoresUsados = array();
	if($arrDados):
		$linha = 0;
		foreach($arrDados as $k => $dado):
			$cor = $n%2==1 ? "#f5f5f5" : ""; 
			
			if(in_array($dado['indid'],$arrIndicadoresUsados)){				
				continue;	
			}
			$arrIndicadoresUsados[] = $dado['indid'];
			
 			$executado = recuperaValorExecutado($dado['indid']);
			
 			
 			//if ( $dado['mtidsc'] == 'Produto' ){
 			//se houver indicador vinculado, irá trazer os dados destes
				$sql = "select distinct
							mic.indid
						from
							pde.monitoraitemchecklist mic 
						inner join
							pde.atividade ati ON ati.atiid = mic.atiid
						where
							ati._atiprojeto = {$_REQUEST['atiprojeto']}
						and
							mic.micstatus = 'A'";
				
				
				
				$arIndicador = $db->carregarColuna($sql);
				
				$sql = "SELECT 
							idvfiltro,
							idvdsc,
							idvmeta
						FROM 
							painel.indicadoresvinculados 
						WHERE 
							idvstatus = 'A' AND 
							indidvinculo IN (".implode(",",$arIndicador).") AND
							indid = {$dado['indid']}";
				
				
				
				$dadosIndicadorVinculado = $db->pegaLinha( $sql );
				
				$idvfiltro 		 = $dadosIndicadorVinculado['idvfiltro'] ;
				$dado['idvdsc']  = $dadosIndicadorVinculado['idvdsc'] ;
				$dado['idvmeta'] = $dadosIndicadorVinculado['idvmeta'];
				
				$arFiltro = transformaFiltroIndicadorParaArray($idvfiltro);
				
				if( !empty($arFiltro) ){
					$vlr = getValorIndicador($dado['indid'], $arFiltro);					
					$executado['qtde'] = $vlr;
				}
			//}
			
			$arr_ultima_meta = recuperaUltimaMeta($dado['indid']);

			// Ajuste na meta se tiver mais de uma
			$arUltimaMeta = array();
			$dataAnteriorUltimaMeta = '';
			if(count($arr_ultima_meta)>1){
				foreach($arr_ultima_meta as $um){
					if(empty($um['aedid'])){
						$arUltimaMeta[WK_ESTADO_DOC_EM_EXECUCAO][] = $um;
					}else{				 
						$arUltimaMeta['outro'][] = $um;
					}
				}
				
				if(count($arUltimaMeta[WK_ESTADO_DOC_EM_EXECUCAO])){
					$arr_ultima_meta = $arUltimaMeta[WK_ESTADO_DOC_EM_EXECUCAO][0];
					if( count($arUltimaMeta['outro']) && !($arr_ultima_meta['data']-date('Ymd')<=5) ){
						$ultimoIndice = count($arUltimaMeta['outro'])-1;
						$arr_ultima_meta = $arUltimaMeta['outro'][$ultimoIndice];
					}else
					if(count($arUltimaMeta[WK_ESTADO_DOC_EM_EXECUCAO])==0){
						unset($executado);
					}				
				}else{
					$ultimoIndice = count($arUltimaMeta['outro'])-1;
					$arr_ultima_meta = $arUltimaMeta['outro'][$ultimoIndice];					
				}
			}else{
				$arr_ultima_meta = $arr_ultima_meta[0] ? $arr_ultima_meta[0] : false;
			}
			
// 			$ultima_meta 	 = ($dado['idvmeta'] ? $dado['idvmeta'] : $arr_ultima_meta['qtde']);
// 			$meta 			 = ($dado['idvmeta'] ? $dado['idvmeta'] : $arr_ultima_meta['qtde']);
			
			$ultima_meta 	 = ($arr_ultima_meta['qtde'] ? $arr_ultima_meta['qtde'] : $dado['idvmeta']);
			$meta 			 = ($arr_ultima_meta['qtde'] ? $arr_ultima_meta['qtde'] : $dado['idvmeta']);
			
			$sql = "select * from painel.indicador where indid = {$dado['indid']}";
			$ind = $db->pegaLinha($sql);
			
			if($ind['unmid'] == 5 || $ind['unmid'] == 1){
				$qtde_final = number_format($executado['qtde'],2,',','.').( $ind['unmid'] == 1 ? ' %' : '' );;
				$ultimaMeta = number_format($ultima_meta,2,',','.').( $ind['unmid'] == 1 ? ' %' : '' );
			}else{
				$qtde_final = number_format($executado['qtde'],0,'','.');
				$ultimaMeta = number_format($ultima_meta,0,',','.');
			}
			
			if($ind['estid'] == 2){ //Menor melhor
				$img_indicador = "indicador-vermelha.png";
				$arrMedidor[0] = array("inicio" => 50, "fim" => 66.6, "cor" => "#80BC44", "bgcolor" => "#80BC44");
				$arrMedidor[1] = array("inicio" => 66.6, "fim" => 83.2, "cor" => "#FFFF00", "bgcolor" => "#FFC211");
				$arrMedidor[2] = array("inicio" => 83.2, "fim" => 100, "cor" => "#E95646", "bgcolor" => "#E95646");
			}else{
				$arrMedidor[0] = array("inicio" => 50, "fim" => 66.6, "cor" => "#E95646", "bgcolor" => "#E95646");
				$arrMedidor[1] = array("inicio" => 66.6, "fim" => 83.2, "cor" => "#FFFF00", "bgcolor" => "#FFC211");
				$arrMedidor[2] = array("inicio" => 83.2, "fim" => 100, "cor" => "#80BC44", "bgcolor" => "#80BC44");
				$img_indicador = "indicador-verde.png";
			}
			
			//Verifica se existe medidor do detalhe
			$sql = "select dmdestavel, dmdcritico from painel.detalhemetaindicador where dmiid = {$arr_ultima_meta['dmiid']}";
			$arrEstavel = $db->pegaLinha($sql);
			if($arrEstavel['dmdestavel'] && $arrEstavel['dmdcritico']){
				unset($arrMedidor);
				$arrMedidor[0] = array("inicio" => 50, "fim" => ($arrEstavel['dmdcritico'] >= 66.6 ? $arrEstavel['dmdcritico'] : 66.6), "cor" => "#E95646", "bgcolor" => "#E95646");
				$arrMedidor[1] = array("inicio" => ($arrEstavel['dmdcritico'] >= 66.6 ? $arrEstavel['dmdcritico'] : 66.6), "fim" => ($arrEstavel['dmdestavel'] >= 83.2 ? $arrEstavel['dmdestavel'] : 83.2), "cor" => "#FFFF00", "bgcolor" => "#FFC211");
				$arrMedidor[2] = array("inicio" => ($arrEstavel['dmdestavel'] >= 83.2 ? $arrEstavel['dmdestavel'] : 83.2), "fim" => 100, "cor" => "#80BC44", "bgcolor" => "#80BC44");
			}
			
			if($executado['qtde'] && $meta && $meta != "0.00"){
				$porcentagem = round((($executado['qtde'] ? $executado['qtde'] : 1)/($meta ? $meta : 1))*100,2);
			}else{
				$porcentagem = 0;
			}
			$valor_final = $porcentagem;
			//Verifica se o executado é maior que a meta
			if($executado['qtde'] > $meta){
				$porcentagem = 100;
			}
			$cor_meta = retornaCorMeta($arrMedidor,$porcentagem);
			$porcentagem = $valor_final;

			//Se for Prazo, verifica se a execução foi antes da data para preencher de vermelho
			$data_meta 			= $arr_ultima_meta['data'] ? (int)$arr_ultima_meta['data'] : false;
			$data_execucao 		= $executado['data'] ? (int)$executado['data'] : false;
			$situacao 			= $arr_ultima_meta['aedid']; //$dado['mvdsituacao'];
			$cor_fonte 			= false;
			$cor_fonte_geral 	= false;
			$img_atrazo 		= "";
			$cor_prazo 			= "";
			$texto_prazo 		= false;
			$ico 				= '';
			$icoMsg 			= '';
			
			// Não executado
			if($situacao == WK_MON_EST_AEDID_NAO_EXECUTAR){
				$cor_prazo 		= "#E95646";
				$texto_prazo 	= "Não Executado";
				$ico 			= 'exclamacao_checklist_vermelho.png';
				$icoMsg 		= 'Não Executado';
				// 				ver($arr_ultima_meta);
			}else
			// Executado			
			if($data_execucao && $data_meta){					
				if($data_execucao <= $data_meta){
					$cor_prazo  = "#80BC44";
					$ico 		= 'check_checklist.png';
					$icoMsg 	= 'Executado';
				}else{
					$cor_prazo  = "#80BC44";
					$cor_fonte  = "#FF0000";
					$ico 		= 'check_checklist_vermelho.png';
					$icoMsg 	= 'Executado fora do prazo';
				}
			}else
			// À executar
			if(($data_meta-date('Ymd'))<=5 && ($data_meta-date('Ymd'))>=0 && empty($situacao) && empty($data_execucao)){
				$cor_prazo 		= "#FFC211";
				$texto_prazo 	= "A Executar";
				$ico 			= 'exclamacao_checklist.png';
				$icoMsg 		= 'A Executar';
			}else
			// Não informado
			if($data_meta < date('Ymd') && empty($situacao) && empty($data_execucao)){
				$cor_prazo 		= "#E95646";
				$texto_prazo 	= "Não Informado";
				$ico 			= 'exclamacao_checklist_vermelho.png';
				$icoMsg 		= 'Não Informado';
			}
			
// 			ver($situacao, $data_execucao);
			if( !$tipo || $tipo != $dado['mtidsc'] ){
				if( $tipo ){
			?>
			<tr bgcolor="#c5c5c5">
				<td colspan="5">
				</td>
			</tr>
			<?php 
				}
				$tipo = $dado['mtidsc'];
			}
			
			if( $dado['mtiid'] == MON_MTIID_PROCESSO ){
				$linha = $linha+1;
				atualizaOrdem(Array('indid'=>$dado['indid'], 'ordem' => $linha));
			}else{
				atualizaOrdem(Array('indid'=>$dado['indid'], 'ordem' => 'null'));
			}
			?>
			<tr bgcolor="<?php echo $cor_padrao ? $cor_padrao : $cor ?>" id="tr_<?=$linha ?>_<?=$dado['indid'] ?>" >
				<td>
				  
					<?php 
					//Verifica se possui filhos para criar o link para o popup 
					$sql = "select indidvinculo from painel.indicadoresvinculados where idvstatus = 'A' and indid = {$dado['indid']}";
					$possui_filhos = $db->pegaUm($sql);
					
					if($dado['total_metas']>1 && ! $possui_filhos): ?>		
						<img class="img_middle link historicoMetas" id="img_<?php echo $dado['indid'].'_'.$dado['metid']?>" title="Expandir" src="../imagens/mais.gif" />
					<?php endif; ?>
				
					<?php if( $dado['mtiid'] == 3 ){?>
						<?php $disableSobe = ($linha > 1) ? '' : 'display:none'?>
						<img class="img_middle sobe" src="../imagens/indicador-verde2.png" style="cursor:pointer; <?=$disableSobe ?>" id="sobe_<?=$linha ?>_<?=$dado['indid'] ?>" />
						<?php $disableDesce = ( $k < (count($arrDados)-1) ) ? '' : 'display:none' ?> 
						<img class="img_middle desce" src="../imagens/indicador-verde2_para_baixo.png" style="cursor:pointer; <?=$disableDesce ?>" id="desce_<?=$linha ?>_<?=$dado['indid'] ?>" />
					<?php }?>
					<?php echo $dado['mtinivel'] > 1 ? "<img style=\"margin-left:".($nivel*10)."px\" src=\"../imagens/seta_retorno.gif\" /> " : "" ?><?php echo ($dado['idvdsc'] ? $dado['idvdsc'] : $dado['indid'] . " " . $dado['indnome']) ?>
					<?php if($dado['mtiid'] != 3): ?>
						<img class="img_middle link" onclick="exibeGraficoMeta(<?php echo $dado['indid']?>)" src="../imagens/seriehistorica_ativa.gif" /> 
					<?php endif; ?>
					<?php
					
					
					if($possui_filhos): ?>
						<img class="img_middle link" onclick="exibeFilhosIndicador(<?php echo $dado['indid']?>)" src="../imagens/consultar.gif" />&nbsp;
					<?php endif; ?>
					<?php
					//Verifica se possui mais de uma meta
					if($dado['qtd_meta']>1): ?>
<!--						<img class="img_middle link" onclick="exibeMetas(<?php echo $dado['indid']?>)" title="Lista de Metas" src="../imagens/lista_verde.gif" />-->
					<?php endif; ?>
					<?php if( is_array( pegaQtdMetaItemChecklist( $dado['indid'] ) ) ):?>
						<img class="img_middle link" title="Lista Histórico de Metas" onclick="exibeHistoricoMetas(<?php echo $dado['indid']?>)" src="../imagens/fluxodocgraf.gif" />
					<?php endif;?>
					
					<?php echo ($dado['idvdsc'] ? "<br>(" . $dado['indid'] . " " . $dado['indnome'] . " )" : '') ?>
				</td>
				<td>
					<?=$dado['entnome'] ?>
				</td>
				<td>
					<?php echo $dado['mtidsc'] ? $dado['mtidsc'] : "N/A" ?>
				</td>
				<td <?php if($permiteEditarData && $executado['sehid'] && $situacao != WK_MON_EST_AEDID_NAO_EXECUTAR): ?> onclick="editaDataExecucao('<?= $executado['sehid']?>')" <?php endif; ?> bgcolor="<?php echo $cor_prazo ? $cor_prazo : "" ?>" style="border:solid 1px black;color:<?php echo $cor_fonte_geral ? $cor_fonte_geral : "#FFFFFF" ?>;font-weight:bold;text-align:right;" >
					<?php echo $texto_prazo ? $texto_prazo : "" ?>
					<?php if($executado && $situacao != WK_MON_EST_AEDID_NAO_EXECUTAR): ?>
						<?php 
						//$stAgrupador = '';
						if(!empty($arFiltro['agrupador']) && in_array($dado['mtidsc'], array('Produto', 'Impacto'))){
							$arrAgrupadores = getAgrupadoresPorDetalhe($dado['indid']);
							$arrAgrPorReg = getAgrupadorPorRegionalizador($dado['indid']);
							if(is_array($arrAgrPorReg)){
								foreach($arrAgrPorReg as $arrAgr){
									array_push($arrAgrupadores,$arrAgr);
								}
							}
							/*
							foreach($arrAgrupadores as $agp){
								if($arFiltro['agrupador'] == $agp['codigo']){
									$stAgrupador = $agp['descricao'];
									break;
								}								
							}
							echo $stAgrupador ? $stAgrupador.'<br/>' : '';
							*/
						}

						if($arFiltro['extracao'] != 'valor'){
							$qtde_final = explode(',', $qtde_final);
							$qtde_final = $qtde_final[0];
							$ultimaMeta = explode(',', $ultimaMeta);
							$ultimaMeta = $ultimaMeta[0];
						}
						?>						
						<?php if($dado['unmid'] == UNIDADEMEDICAO_BOLEANA): ?>
							<span style="color:<?php echo $cor_fonte && ! $possui_filhos ? $cor_fonte : "#FFFFFF" ?>">em <?php echo $executado['data_execucao'] ?></span>
						<?php else: 
						?>
							<span style="font-size:14px" ><?php  echo $possui_filhos . $executado ? $qtde_final . ' ' .str_replace( "Percentual", "%",$dado['umedesc'] ) : "" ?></span><br />(<?php echo str_replace(".",",",$porcentagem) ?>%) <br /><span style="color:<?php echo $cor_fonte && ! $possui_filhos? $cor_fonte : "#FFFFFF" ?>" > em <?php echo $executado['data_execucao'] ?></span>
						<?php endif; ?>
					<?php endif;?>
				</td>
				<td style="text-align:right" >
					<?php if($dado['unmid'] == UNIDADEMEDICAO_BOLEANA): ?>
						em <?php echo $arr_ultima_meta['data_meta']  ?>
					<?php else: ?>
						<span style="color:blue;" ><?php echo  $dado['idvmeta'] ?  number_format( $dado['idvmeta'],2,",",".") :  $ultimaMeta. ' '  .str_replace( "Percentual", "%",$dado['umedesc'] )   ?></span><br />em <?php echo $arr_ultima_meta['data_meta'] ?>
					<?php endif; ?>
				</td>
				<td align="center" >
					<?php if( $executado && $dado['unmid'] != UNIDADEMEDICAO_BOLEANA && $situacao != WK_MON_EST_AEDID_NAO_EXECUTAR ){?>
						<?php $executado && $dado['unmid'] != UNIDADEMEDICAO_BOLEANA ? $medidor = new MedidorDesempenho($arrMedidor,$porcentagem,$ultimaMeta) : ""; ?>
						<?php //$executado && $dado['unmid'] != UNIDADEMEDICAO_BOLEANA ? $medidor = new MedidorDesempenho($arrMedidor,$porcentagem,number_format($ultima_meta,2,',','.')) : ""; ?>
					<?php }else{?>
						<?php if( $ico != '' ){?>
							<div style="float:center;margin-left:5px;">
								<img class="img_middle link" title="<?=$icoMsg ?>" src="../imagens/<?=$ico ?>" />
							</div>
						<?php }?>
					<?php }?>
				</td>
			</tr>
			<tr id="tr_<?php echo $dado['indid'].'_'.$dado['metid']; ?>" style="display:none;">
				<td colspan="5" align="left" valign="top">
					
				</td>				
			</tr>
			<?php if($filhos): ?>
				<?php //$n = listaIndicadoresPainelMonitoramento($dado['indid'],$n,$nivel) ?>
			<?php endif; ?>
		<?php unset($arFiltro);$n++;endforeach;?>
	<?php endif;
	return $n;
	
}

function exibeMetas()
{
	global $db;
	?>
	<html>
		<head>
			<script language="JavaScript" src="../includes/funcoes.js"></script>
			<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
			<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		</head>
	<body>
		<?php monta_titulo("Metas",""); ?>
	<?php
	$indid = $_GET['indid'];
	$sql = "select
				ind.indnome
			from
				painel.indicador ind
			inner join
				painel.indicadoresvinculados iv ON iv.indidvinculo = ind.indid
			where
				idvstatus = 'A' and iv.indid = $indid
			order by
				ind.indid";
	$arrCab = array("Indicador");
	$db->monta_lista($sql,$arrCab,100,10,"N","center","N");
	?>
		</body>
	</html>
	<?php
	exit;
	
}

function downloadArquivoGeral(Array $param ){
	global $db;
	
	$sql ="SELECT 
			* 
		   FROM 
		   	public.arquivo 
		   WHERE 
		   	arqid = " . $param['arqid'];
	$arquivo = current($db->carregar($sql));
	$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arquivo['arqid']/1000) .'/'.$arquivo['arqid'];
	if ( !is_file( $caminho ) ) {
		$_SESSION['MSG_AVISO'][] = "Arquivo não encontrado.";
	}
	if ( is_file( $caminho ) ) {
		$filename = str_replace(" ", "_", $arquivo['arqnome'].'.'.$arquivo['arqextensao']);
		header( 'Content-type: '. $arquivo['arqtipo'] );
		header( 'Content-Disposition: attachment; filename='.$filename);
		readfile( $caminho );
		exit();
	} else {
		die("<script>alert('Arquivo não encontrado.'); history.go(-1)</script>");
		
	}
}

function exibeHistoricoMetas()
{
	global $db;
	?>
	<html>
		<head>
			<script language="JavaScript" src="../includes/funcoes.js"></script>
			<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
			<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
			<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
			<script>
				$(Document).ready(function(){
					$('.exibeMetas').live('click',function(){
						if( $('.antigos'+$(this).attr('id')).css('display') == 'none' ){
							$('.antigos'+$(this).attr('id')).show();
						}else{
							$('.antigos'+$(this).attr('id')).hide();
						}
					})
				});
				function wf_exibirHistorico( docid )
				{
					var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php' +
						'?modulo=principal/tramitacao' +
						'&acao=C' +
						'&docid=' + docid;
					window.open(
						url,
						'alterarEstado',
						'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
					);
				}
			</script>
		</head>
		<body>
		<?php monta_titulo("Histórico de Metas",""); ?>
	<?php
	$indid = $_GET['indid'];
	$sql = "SELECT DISTINCT
				met.metid as id,
				met.metdesc as desc
			FROM
				pde.monitoravalidacao mvd
			LEFT  JOIN pde.monitoraanexochecklist mac ON mac.mvdid = mvd.mvdid
			INNER JOIN pde.monitorameta mnm ON mnm.mnmid = mvd.mnmid
			INNER JOIN painel.metaindicador met ON met.metid = mnm.metid
			WHERE
				met.indid = $indid 
				AND mnm.mnmstatus = 'A'
				AND met.metstatus = 'A'
			ORDER BY
				id ASC";
	$itensChecklist = $db->carregar($sql);
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="5" align="center" >
	<?php 
	if( is_array($itensChecklist) ){
		foreach( $itensChecklist as $i => $itenChecklist ){
			$metas = carregarMetasChecklist( $itenChecklist['id'] );
	?>
		<tr>
			<td>
				<b><?=$itenChecklist['desc'] ?></b>
				</br></br>
				<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="5" align="center" >
					<tr bgcolor="#DCDCDC">
						<td> Valor</td>
						<td> Quantidade</td>
						<td> Data de Execução</td>
						<td> Data de Validação</td>
						<td> Estado</td>
					</tr>
					<?php 
					foreach( $metas as $m => $meta ){
						if( $m != 0 && count($metas) > 1 ){
							$hidden = 'class="antigos'.$itenChecklist['id'].'" style="display:none"';
						}
						$histsMetas = carregarHistoricoMetasChecklist( $meta['meta_docid'] );
					?>
					<tr bgcolor="white" <?=$hidden ?>>
						<td> <?=$meta['meta_valor'] ?></td>
						<td> <?=$meta['meta_qtde'] ?></td>
						<td> <?=$meta['meta_dtexecucao'] ?></td>
						<td> <?=$meta['meta_dtvalidacao'] ?></td>
						<td> 
							<div style="valign:middle;float:left;margin-top:6px;"><?=$meta['esddsc'] ?></div> 
							<img style="cursor: pointer;" title="Ver histórico de tramitação."
								 src="http://<?php echo $_SERVER['SERVER_NAME'] ?>/imagens/fluxodoc.gif"
								 onclick="wf_exibirHistorico( '<?=$meta['meta_docid'] ?>' );" />
						</td>
					</tr>
					<tr bgcolor="#f5f5f5" <?=$hidden ?>>
						<td colspan="5">
						
							<fieldset>
								<legend>Detalhe(s) de Pendência(s)</legend>
							
							<table class="tabela" cellspacing="1" cellpadding="5" align="center">
								<tr>
									<td> Data</td>
									<td> Observação</td>
									<td> Quantidade</td>
									<td> Valor</td>
									<td> Arquivo</td>
								</tr>
							<?php 
								if( is_array( $histsMetas ) ){
									foreach( $histsMetas as $histMetas){
							?>
								<tr style="background: #FFFFFF">
									<td valign="top"> <?=$histMetas['data'] ?></td>
									<td valign="top"> <?=$histMetas['mvdobservacao'] ?></td>
									<td valign="top"> <?=$histMetas['mnmqtd'] ?></td>
									<td valign="top"> <?=$histMetas['mnmvalor'] ?></td>
									<td valign="top"> <?=($histMetas['arqnome'] ? '<a href="?modulo=principal/painel_estrategico&acao=A&requisicao=downloadArquivoGeral&arqid=' . $histMetas['arqid'] . '">' . $histMetas['arqnome'] . '</a>' : '') ?></td>
								</tr>
							<?php 
									}
								}else{?>
							<?php }?>
							</table>
							
							</fieldset>
							
						</td>
					</tr>
					<tr <?=$hidden ?>>
						<td colspan="5">
					<?php if( $m == 0 && count($metas) > 1 ){ ?>
							<a class="exibeMetas" id="<?=$itenChecklist['id'] ?>">Exibir metas antigas</a>
					<?php }?>
						</td>
					</tr>
					<?php 	
					}
					?>
				</table>
			</td>
		</tr>
	<?php 
		}
	}else{
	?>
		<tr>
			<td>
				Não possui itens de checklist.
			</td>
		</tr>
	<?php 
	}
	?>
	</table>
		</body>
	</html>
	<?php
	exit;
}

function carregarHistoricoMetasChecklist( $docid ){
	
	global $db;
	
	$sql = "SELECT
				--validado
				to_char(mvd.mvddata,'DD/MM/YYYY') as data,
				mvd.mvdobservacao,
				mvd.mnmqtd,
				mvd.mnmvalor,
				mac.arqid,
				a.arqnome || '.' || a.arqextensao AS arqnome
				--fim validado
			FROM
				pde.monitoravalidacao mvd
			LEFT  JOIN pde.monitoraanexochecklist mac ON mac.mvdid = mvd.mvdid
			LEFT  JOIN public.arquivo a ON a.arqid = mac.arqid
			INNER JOIN pde.monitorameta mnm ON mnm.mnmid = mvd.mnmid
			INNER JOIN painel.metaindicador met ON met.metid = mnm.metid
			INNER JOIN painel.detalhemetaindicador dmi ON dmi.metid = met.metid
			INNER JOIN workflow.documento doc ON doc.docid = dmi.docid
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			WHERE
				dmi.docid =  $docid
				AND mnm.mnmstatus = 'A'
				AND met.metstatus = 'A'
			ORDER BY
				mvd.mvddata ASC";
	return $db->carregar($sql);
}

function carregarMetasChecklist( $metid ){
	
	global $db;
	
	$sql = "SELECT DISTINCT
						--datos metas
						dmi.docid as meta_docid,
						dmi.dmivalor as meta_valor,
						dmi.dmiqtde as meta_qtde,
						to_char(dmi.dmidataexecucao,'DD/MM/YYYY') as meta_dtexecucao,
						to_char(dmi.dmidatavalidacao,'DD/MM/YYYY') as meta_dtvalidacao,
						esd.esdid,
						esd.esddsc
						--fim dados metas
					FROM
						pde.monitoravalidacao mvd
					LEFT  JOIN pde.monitoraanexochecklist mac ON mac.mvdid = mvd.mvdid
					INNER JOIN pde.monitorameta mnm ON mnm.mnmid = mvd.mnmid
					INNER JOIN painel.metaindicador met ON met.metid = mnm.metid
					INNER JOIN painel.detalhemetaindicador dmi ON dmi.metid = met.metid
					INNER JOIN workflow.documento doc ON doc.docid = dmi.docid
					INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
					WHERE
						met.metid = ".$metid."
						AND mnm.mnmstatus = 'A'
						AND met.metstatus = 'A'
					ORDER BY
						meta_docid DESC, esd.esdid ASC";
	return $db->carregar($sql);
}

function pegaQtdMetaItemChecklist( $indid ){
	
	global $db;
	
	$sql = "SELECT
				count(distinct dmi.docid) as qtd_metas,
				met.metid as id_item_checklist
			FROM
				pde.monitoravalidacao mvd
			LEFT  JOIN pde.monitoraanexochecklist mac ON mac.mvdid = mvd.mvdid
			INNER JOIN pde.monitorameta mnm ON mnm.mnmid = mvd.mnmid
			INNER JOIN painel.metaindicador met ON met.metid = mnm.metid
			INNER JOIN painel.detalhemetaindicador dmi ON dmi.metid = met.metid
			WHERE
				met.indid = $indid 
				AND mnm.mnmstatus = 'A'
				AND met.metstatus = 'A'
			GROUP BY
				met.metid";
	
	return $db->carregar($sql);
}

function exibeFilhosIndicador()
{
	global $db;
	?>
	<html>
		<head>
			<script language="JavaScript" src="../includes/funcoes.js"></script>
			<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
			<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		</head>
	<body>
		<?php monta_titulo("Indicadores Vinculados",""); ?>
	<?php
	$indid = $_GET['indid'];
	$sql = "select
			ind.indnome
		from
			painel.indicador ind
		inner join
			painel.indicadoresvinculados iv ON iv.indidvinculo = ind.indid
		where
			idvstatus = 'A' and iv.indid = $indid
		order by
			ind.indid";
	$arrCab = array("Indicador");
	$db->monta_lista($sql,$arrCab,100,10,"N","center","N");
	?>
		</body>
	</html>
	<?php
	exit;
	
}

function listaObrasPainelEstrategico($atidescricao)
{
	global $db;
	
	$atidescricao = strtoupper($atidescricao);
	$sql = "select orgid,prfid from obras.programafonte where removeacento(upper(prfdesc)) ilike(removeacento('%$atidescricao%'))";
	$arrDados = $db->pegaLinha($sql);
	
	if(!$arrDados['orgid'] || !$arrDados['prfid']){
		return false;
	}
	
	$sql = "select
				upper(oi.obrdesc),
				ee.entnome as entdescricao,
				tm.mundescricao || ' / ' || ed.estuf as municipiouf,
				'<div style=\"display:none\">'||obrdtinicio||'</div>' || to_char(obrdtinicio, 'DD/MM/YYYY') as inicio,
				CASE WHEN max(ta.tradtinclusao) IS NOT NULL
					THEN '<div style=\"display:none\">'||max(ta.traterminoexec)||'</div>' || to_char(max(ta.traterminoexec), 'DD/MM/YYYY')
					ELSE '<div style=\"display:none\">'||obrdttermino||'</div>' || to_char(obrdttermino, 'DD/MM/YYYY') 
				END as termino,
				so.stodesc as situacao
			from
				obras.obrainfraestrutura oi
			INNER JOIN
				entidade.entidade ee ON ee.entid = oi.entidunidade
			INNER JOIN
				entidade.endereco ed ON ed.endid = oi.endid
			LEFT JOIN
				territorios.municipio tm ON tm.muncod = ed.muncod
			LEFT JOIN
				obras.termoaditivo ta ON ta.obrid = oi.obrid
			LEFT JOIN
				obras.situacaoobra so ON so.stoid = oi.stoid
			where
				oi.obsstatus = 'A' 
			and
				oi.prfid = {$arrDados['prfid']}
			and
				oi.orgid = {$arrDados['orgid']}
			group by
				oi.obrdesc,
				ee.entnome,
				tm.mundescricao,
				ed.estuf,
				oi.obrdtinicio,
				oi.obrdttermino,
				so.stodesc";
	$arrCab = array("Nome da Obra","Unidade Implantadora","	Município/UF","	Data de Início","Data de Término","Situação da Obra");
	$db->monta_lista($sql,$arrCab,100,10,"N","center","N");
}

function relatorioFinanceiroMetasPainelEstrategico($descricao)
{
	global $db;
	$sql = "SELECT 
			prtdsc AS descricao 
		FROM 
			public.parametros_tela 
		WHERE 
			mnuid = 2526
		AND 
			prtpublico = TRUE";
	$db->monta_lista_simples( $sql, null, 50, 50, null, null, null );
}

function exibeGraficoMeta($micid)
{
	global $db;
	$sql = "select 
			*
		from 
			painel.metaindicador met
		inner join
			painel.indicador ind ON ind.indid = met.indid
		inner join
			pde.monitoraitemchecklist mic ON mic.indid = ind.indid
		inner join
			painel.unidademeta ume ON ind.umeid = ume.umeid
		inner join
			painel.detalhemetaindicador dmi ON dmi.metid = met.metid  
		where 
			mic.micid = $micid
		and
			ind.indstatus = 'I'
		and
			met.metstatus = 'A'
		order by
			met.metid";
	$arrDados = $db->pegaLinha($sql);
	$indnome = $arrDados['indnome'];
	$metid = $arrDados['metid'];
	if($arrDados['unmid'] != UNIDADEMEDICAO_BOLEANA){
		$sql = "select
				dpe.dpedsc,
				CASE WHEN dmi.dmidatameta IS NOT NULL
					THEN to_char(dmi.dmidatameta,'DD/MM/YYYY')
					ELSE 'N/A' 
				END as dmidatameta,
				CASE WHEN dmi.dmidataexecucao IS NOT NULL
					THEN to_char(dmi.dmidataexecucao,'DD/MM/YYYY')
					ELSE 'N/A' 
				END as dtexecucao,
				CASE WHEN dmi.dmidatavalidacao IS NOT NULL
					THEN to_char(dmi.dmidatavalidacao,'DD/MM/YYYY')
					ELSE 'N/A' 
				END as dmidatavalidacao,
				seh.sehqtde,
				to_char(seh.sehdtcoleta,'dd/mm/yyyy') as sehdtcoleta,
				dmi.dmiqtde
			from
				painel.detalhemetaindicador dmi
			inner join
				painel.detalheperiodicidade dpe ON dmi.dpeid = dpe.dpeid
			inner join
				painel.seriehistorica seh ON seh.dmiid = dmi.dmiid
			where
				dmi.metid = $metid
			and
				dmi.dmistatus = 'A'
			order by
				dmi.dmidatameta";
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		foreach($arrDados as $dado){
			$dado['sehqtde'] = (float)$dado['sehqtde'];
			$dado['dmiqtde'] = (float)$dado['dmiqtde'];
			$arrData[] = $dado['dmidatameta'];
			$arrMeta[] = $dado['dmiqtde'];
			$arrExecutada[] = $dado['sehqtde'];
		}
	}
	$arrData 	  = $arrData ? $arrData : array(0);
	$arrMeta 	  = $arrMeta ? $arrMeta : array(0);
	$arrExecutada = $arrExecutada ? $arrExecutada : array("N/A");
	
	?>
		<html>
			<head>
				<script language="JavaScript" src="../includes/funcoes.js"></script>
				<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
				<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
				<link type="text/css" href="/includes/jquery-jqplot-1.0.0/jquery.jqplot.min.css" rel="stylesheet" />
				<script src="/includes/jquery.mobile-1.0.1/jquery-1.7.1.min.js"></script>
				<script type="text/javascript" src="/includes/jquery-jqplot-1.0.0/jquery.jqplot.min.js"></script>
				<script type="text/javascript" src="/includes/jquery-jqplot-1.0.0/plugins/jqplot.logAxisRenderer.js"></script>
		    	<script type="text/javascript" src="/includes/jquery-jqplot-1.0.0/plugins/jqplot.categoryAxisRenderer.min.js"></script>
		    	<script type="text/javascript" src="/includes/jquery-jqplot-1.0.0/plugins/jqplot.pointLabels.min.js"></script>
		    </head>
			<body>
				<script>
					jQuery(document).ready(function(){
						var line1 = [<?php echo implode(",",$arrMeta) ?>];
						var line2 = [<?php echo implode(",",$arrExecutada) ?>];
						var ticks = ['<?php echo implode("','",$arrData) ?>'];
					 	jQuery.jqplot('grafico',[line1,line2],{
						 	seriesDefaults:{
					            renderer:jQuery.jqplot.BarRenderer,
					            rendererOptions: {fillToZero: true},
					            pointLabels: { show:true } 
					        },
						 	series:[
						            {label:'Valores das Metas'},
						            {label:'Valores Executados'},
						        ],
						    legend: {
					            show: true,
					            placement: 'outsideGrid'
					        },
					         axes: {
					            // Use a category axis on the x axis and use our custom ticks.
					            xaxis: {
					                renderer: jQuery.jqplot.CategoryAxisRenderer,
					                ticks: ticks
					            },
					            // Pad the y axis just a little so bars can get close to, but
					            // not touch, the grid boundaries.  1.2 is the default padding.
					            yaxis: {
					                pad: 1.05,
					                tickOptions: {formatString: '%d'}
					            }
					        }
					 	});
					});
				</script>
				<?php monta_titulo("Gráfico das Metas",$indnome); ?>
				<table class="tabela" width="100%" height="70%" align="center" >
					<tr>
						<td>
							<center>
								<div id="grafico" style="height:90%;width:100%"></div>
							</center>
						</td>
					</tr>
				</table>
			</body>    
		</html>
	<?php
	exit;	
}

function exibeListaGraficoMeta()
{
	global $db;
	$micid = $_POST['micid'];
	$sql = "select 
			*
		from 
			painel.metaindicador met
		inner join
			painel.indicador ind ON ind.indid = met.indid
		inner join
			pde.monitoraitemchecklist mic ON mic.indid = ind.indid
		inner join
			painel.unidademeta ume ON ind.umeid = ume.umeid
		inner join
			painel.detalhemetaindicador dmi ON dmi.metid = met.metid  
		where 
			mic.micid = $micid
		and
			ind.indstatus = 'I'
		and
			met.metstatus = 'A'
		order by
			met.metid";
	$arrDados = $db->pegaLinha($sql);
	$indnome = $arrDados['indnome'];
	$metid = $arrDados['metid'];
	if($arrDados['unmid'] != UNIDADEMEDICAO_BOLEANA){
		$sql = "select
				dpe.dpedsc,
				CASE WHEN dmi.dmidatameta IS NOT NULL
					THEN to_char(dmi.dmidatameta,'DD/MM/YYYY')
					ELSE 'N/A' 
				END as dmidatameta,
				CASE WHEN dmi.dmidataexecucao IS NOT NULL
					THEN to_char(dmi.dmidataexecucao,'DD/MM/YYYY')
					ELSE 'N/A' 
				END as dtexecucao,
				CASE WHEN dmi.dmidatavalidacao IS NOT NULL
					THEN to_char(dmi.dmidatavalidacao,'DD/MM/YYYY')
					ELSE 'N/A' 
				END as dmidatavalidacao,
				seh.sehqtde,
				to_char(seh.sehdtcoleta,'dd/mm/yyyy') as sehdtcoleta,
				dmi.dmiqtde
			from
				painel.detalhemetaindicador dmi
			inner join
				painel.detalheperiodicidade dpe ON dmi.dpeid = dpe.dpeid
			inner join
				painel.seriehistorica seh ON seh.dmiid = dmi.dmiid
			where
				dmi.metid = $metid
			and
				dmi.dmistatus = 'A'
			order by
				dmi.dmidatameta";
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		foreach($arrDados as $dado){
			$dado['sehqtde'] = (float)$dado['sehqtde'];
			$dado['dmiqtde'] = (float)$dado['dmiqtde'];
			$arrData[] = $dado['dmidatameta'];
			$arrMeta[] = $dado['dmiqtde'];
			$arrExecutada[] = $dado['sehqtde'];
		}
	}
	$arrData 	  = $arrData ? $arrData : array(0);
	$arrMeta 	  = $arrMeta ? $arrMeta : array(0);
	$arrExecutada = $arrExecutada ? $arrExecutada : array("N/A");
	
	?>
		<link type="text/css" href="/includes/jquery-jqplot-1.0.0/jquery.jqplot.min.css" rel="stylesheet" />
		<script type="text/javascript" src="/includes/jquery-jqplot-1.0.0/jquery.jqplot.min.js"></script>
		<script type="text/javascript" src="/includes/jquery-jqplot-1.0.0/plugins/jqplot.logAxisRenderer.js"></script>
    	<script type="text/javascript" src="/includes/jquery-jqplot-1.0.0/plugins/jqplot.categoryAxisRenderer.min.js"></script>
    	<script type="text/javascript" src="/includes/jquery-jqplot-1.0.0/plugins/jqplot.pointLabels.min.js"></script>
		<script>
			jQuery(document).ready(function(){
				var line1 = [<?php echo implode(",",$arrMeta) ?>];
				var line2 = [<?php echo implode(",",$arrExecutada) ?>];
				var ticks = ['<?php echo implode("','",$arrData) ?>'];
			 	jQuery.jqplot('grafico',[line1,line2],{
				 	seriesDefaults:{
			            renderer:jQuery.jqplot.BarRenderer,
			            rendererOptions: {fillToZero: true},
        				pointLabels: { show:true } 
			        },
				 	series:[
				            {label:'Valores das Metas'},
				            {label:'Valores Executados'},
				        ],
				    legend: {
			            show: true,
			            location: 's',
			            placement: 'outsideGrid'
			        },
			         axes: {
			            // Use a category axis on the x axis and use our custom ticks.
			            xaxis: {
			                renderer: jQuery.jqplot.CategoryAxisRenderer,
			                ticks: ticks
			            },
			            // Pad the y axis just a little so bars can get close to, but
			            // not touch, the grid boundaries.  1.2 is the default padding.
			            yaxis: {
			                pad: 1.05,
			                tickOptions: {formatString: '%d'}
			            },
			            highlighter: {
					        show: true,
					        sizeAdjust: 7.5
					    },
					      cursor: {
					        show: false
					    }
			        }
			 	});
			});
		</script>
		<?php monta_titulo($indnome,""); ?>
		<table class="tabela" width="100%" height="70%" align="center" >
			<tr>
				<td valign="top" width="50%" >
					<center>
						<div id="grafico" style="height:100%;width:90%"></div>
					</center>
				</td>
				<td valign="top" >
					<?php 
					$sql = "select
								CASE WHEN dmi.dmidatameta IS NOT NULL
									THEN to_char(dmi.dmidatameta,'DD/MM/YYYY')
									ELSE 'N/A' 
								END as dmidatameta,
								to_char(seh.sehdtcoleta,'dd/mm/yyyy') as sehdtcoleta,
								seh.sehqtde,
								dmi.dmiqtde
							from
								painel.detalhemetaindicador dmi
							inner join
								painel.detalheperiodicidade dpe ON dmi.dpeid = dpe.dpeid
							inner join
								painel.seriehistorica seh ON seh.dmiid = dmi.dmiid
							where
								dmi.metid = $metid
							and
								dmi.dmistatus = 'A'
							order by
								dmi.dmidatameta"; ?>
					<?php $arrCab = array("Data da Meta","Data de Execução","Valor da Meta","Valor Executado") ?>
					<?php $db->monta_lista($sql,$arrCab,100,10,"N","center","N") ?>
				</td>
			</tr>
		</table>
	<?php
}

function recuperaValorExecutado($indid, $sehid = null)
{
	global $db;
	
	$stWhere = '';
	if($sehid){
		$stWhere .= " AND seh.sehid = {$sehid} ";
	}
	
	$sql = "SELECT 
				sehqtde as qtde,
				seh.sehid,
				dpe.dpeid,
				to_char(sehdtcoleta,'YYYYMMDD') as data,
				CASE WHEN ind.indstatus = 'A' AND ind.perid = 3 THEN
					dpeanoref 
				ELSE 
					to_char(sehdtcoleta,'DD/MM/YYYY') 
				END as data_execucao,
				dpedsc,
				dpeanoref 
			FROM 
				painel.seriehistorica seh
			INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid 
			INNER JOIN painel.indicador			   ind ON ind.indid = seh.indid
			WHERE 
				seh.indid = $indid 
				AND seh.sehstatus NOT IN ('I')
				{$stWhere}
			ORDER BY
				sehid desc --dpedatainicio desc
			LIMIT
				1";

	$arrDados = $db->pegaLinha($sql);
	
	if(!$arrDados){
		return false;
	}
	//verifica se o indicador pode acumular
	$sql = "select indcumulativo from painel.indicador where indid = $indid";
	$indcumulativo = $db->pegaUm($sql);
	if($indcumulativo == "S"){ //Se for cumulativo
		$sql = "select 
					sum(sehqtde) as qtde
				from 
					painel.seriehistorica seh
				inner join
					painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid 
				where 
					seh.indid = $indid 
				and 
					seh.sehstatus != 'I'
				--and
					--dpeanoref = '{$arrDados['dpeanoref']}'";
		$qtde = $db->pegaUm($sql);
		$arrDados['qtde'] = $qtde;
	}elseif($indcumulativo == "A"){ //Se for cumulativo por ano
		$sql = "select 
					sum(sehqtde) as qtde
				from 
					painel.seriehistorica seh
				inner join
					painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid 
				where 
					seh.indid = $indid 
				and 
					seh.sehstatus != 'I'
				and
					dpeanoref = '{$arrDados['dpeanoref']}'";
		$qtde = $db->pegaUm($sql);
		$arrDados['qtde'] = $qtde;
	}
	
	return $arrDados;
}

function verificaMetasCadastradas()
{
	global $db;
	$micid = $_POST['micid'];
	$sql = "select 
				met.perid
			from
				pde.monitorameta mnm
			inner join
				painel.metaindicador met ON mnm.metid = met.metid
			where
				mnm.micid = $micid
			and
				metstatus = 'A'";
	echo $db->pegaUm($sql);
	exit;
}

function excluirTodasMetas()
{
	global $db;
	$micid = $_POST['micid'];
	$sql = "update
				pde.monitorameta
			set
				mnmstatus = 'I'
			where
				micid = $micid;
			update 
				painel.metaindicador 
			set 
				metstatus = 'I' 
			where 
				metid in( select 
								 met.metid
							from
								pde.monitorameta mnm
							inner join
								painel.metaindicador met ON mnm.metid = met.metid
							where
								mnm.micid = $micid
							and
								metstatus = 'A')";
	$db->executar($sql);
	$db->commit();
	exit;
}

function recuperaMetaIndicador($indid = null,$dpeid = null)
{
	global $db;
	if(!$indid || !$dpeid){
		return false;
	}
	$sql = "select
				dmiqtde as qtde,
				dpe.dpedsc
			from
				painel.metaindicador met
			inner join
				painel.detalhemetaindicador dmi ON dmi.metid = met.metid
			inner join
				painel.detalheperiodicidade dpe ON dpe.dpeid = dmi.dpeid
			where
				met.indid = $indid
			and
				dpe.dpeid = $dpeid";
	$qtde = $db->pegaLinha($sql);
	if(!$qtde){
		$sql = "select
				dmiqtde as qtde,
				dpe.dpedsc,
				dpe.perid,
				dpe.dpeordem
			from
				painel.metaindicador met
			inner join
				painel.detalhemetaindicador dmi ON dmi.metid = met.metid
			inner join
				painel.detalheperiodicidade dpe ON dpe.dpeid = dmi.dpeid
			where
				met.indid = $indid
			and
				dpedatainicio > (select dpedatainicio from painel.detalheperiodicidade dpe2 where dpe2.dpeid = $dpeid and dpe.perid = dpe2.perid)
			limit 1";
		$qtde_fim = $db->pegaLinha($sql);
		$sql = "select
				dmiqtde as qtde,
				dpe.dpedsc,
				dpe.perid,
				dpe.dpeordem
			from
				painel.metaindicador met
			inner join
				painel.detalhemetaindicador dmi ON dmi.metid = met.metid
			inner join
				painel.detalheperiodicidade dpe ON dpe.dpeid = dmi.dpeid
			where
				met.indid = $indid
			and
				dpedatainicio < (select dpedatainicio from painel.detalheperiodicidade dpe2 where dpe2.dpeid = $dpeid and dpe.perid = dpe2.perid)
			limit 1";
		$qtde_ini = $db->pegaLinha($sql);
		
		$sql = "select
			sehqtde as qtde,
			dpe.dpedsc,
			dpe.perid,
			dpe.dpeordem
		from
			painel.seriehistorica seh
		inner join
			painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
		where
			seh.indid = $indid
		and
			sehstatus != 'I'
		order by
			dpedatainicio
		limit
			1";
		$seh_ini = $db->pegaLinha($sql);
		
		if($dpeordem['perid'] == 3){
			$dpeordem['dpeordem'] = (int)$dpeordem['dpedsc'];
		}
		if($qtde_fim['perid'] == 3){
			$qtde_fim['dpeordem'] = (int)$qtde_fim['dpedsc'];
		}
		if($seh_ini['perid'] == 3){
			$seh_ini['dpeordem'] = (int)$seh_ini['dpedsc'];
		}
		if($seh_ini['dpeordem'] > $qtde_ini['dpeordem']){
			$qtde_ini = $seh_ini;
		}
		$sql = "select
					dpeordem,
					perid,
					dpedsc
				from
					painel.detalheperiodicidade
				where
					dpeid = $dpeid";
		$dpeordem = $db->pegaLinha($sql);
		if($dpeordem['perid'] == 3){
			$dpeordem['dpeordem'] = (int)$dpeordem['dpedsc'];
		}
		$range = $qtde_fim['dpeordem'] - $qtde_ini['dpeordem'];
		$valor = ((($qtde_fim['qtde']-$qtde_ini['qtde'])/$range));
		$loop = $dpeordem['dpeordem'] - $qtde_ini['dpeordem'];
		for($i=0;$i<=$loop;$i++){
			$valorFinal+= $valor;
		}
		$qtde = (float)$qtde_ini['qtde']+$valorFinal;
		return array("dpedsc" => $dpeordem['dpedsc'], "qtde" => $qtde);
	}else{
		return $qtde;
	}
}

function recuperaUltimaMeta($indid = null, $metid = null)
{
	global $db;
	
	$stWhere = '';
	if($indid){
		$stWhere .= " AND met.indid = {$indid} ";
	}
	if($metid){
		$stWhere .= " AND met.metid = {$metid} ";
	}
	
	$sql = "SELECT
				dmiqtde as qtde,
				dmi.dmiid,
				dpedsc,
				CASE WHEN dmidataexecucao IS NOT NULL THEN 
					to_char(dmidataexecucao,'YYYYMMDD')
				ELSE 
					to_char(dpedatafim,'YYYYMMDD')
				END as data,
				CASE WHEN dmidataexecucao IS NOT NULL THEN 
					to_char(dmidataexecucao,'DD/MM/YYYY')
				ELSE 
					dpedsc
				END as data_meta
				,doc.esdid, hst2.aedid
			FROM
				painel.metaindicador met
			INNER JOIN painel.detalhemetaindicador 	dmi ON dmi.metid = met.metid
			INNER JOIN painel.detalheperiodicidade 	dpe ON dmi.dpeid = dpe.dpeid
			LEFT JOIN workflow.documento 			doc ON doc.docid = dmi.docid		
			LEFT  JOIN workflow.historicodocumento  hst2 ON hst2.hstid = doc.hstid	
			WHERE
				dmi.dmistatus = 'A'
			{$stWhere}	
				--AND dpedatafim >= now()
			ORDER BY
				dpedatainicio asc";

	$qtde = $db->carregar($sql);
	
	return $qtde;
}

function recuperaMetasPorAtividadePainel($atiid)
{
	global $db;
	$sql = "select
				*,
				to_char((select max(dmi.dmidatameta) from painel.detalhemetaindicador dmi where dmi.metid = mmi.metid limit 1),'DD/MM/YYYY') as data_meta
			from
				 pde.monitoraitemchecklist mic
			inner join
				painel.indicador ind ON ind.indid = mic.indid
			left join
				painel.eixo exo ON exo.exoid = ind.exoid
			left join
				painel.acao aca ON aca.acaid = ind.acaid 
			left join
				pde.monitorameta mmi ON mic.micid = mmi.micid
			left join
				pde.monitoratipoindicador mti ON mti.mtiid = mic.mtiid
			where
				atiid = $atiid
			and
				micstatus = 'A'
			and
				mnmstatus = 'A'
			and
				ind.indstatus = 'I'
			order by
				mic.micordem";
	$arrDados = $db->carregar($sql);
	return $arrDados ? $arrDados : array();
}

function curPageURL() {
	$pageURL = 'http';
	if ($_SERVER["HTTPS"] == "on") {$pageURL .= "s";}
		$pageURL .= "://";
	if ($_SERVER["SERVER_PORT"] != "80") {
		$pageURL .= $_SERVER["SERVER_NAME"].":".$_SERVER["SERVER_PORT"].$_SERVER["REQUEST_URI"];
	} else {
		$pageURL .= $_SERVER["SERVER_NAME"].$_SERVER["REQUEST_URI"];
	}
	return $pageURL;
}

function listaPainel() {
	global $db;
	
	switch($_REQUEST['atiprojeto']) {
		case '129391':
			$_REQUEST['detalhes']='pais';
			$_REQUEST['acaid']=22;
			break;
		case '129209':
			$_REQUEST['detalhes']='pais';
			$_REQUEST['acaid']=13;
			break;
		case '129267':
			$_REQUEST['detalhes']='pais';
			$_REQUEST['acaid']=7;
			break;
		case '129250':
			$_REQUEST['detalhes']='pais';
			$_REQUEST['acaid']=143;
			break;
		case '129348':
			$_REQUEST['detalhes']='pais';
			$_REQUEST['acaid']=5;
			break;
		case '129347':
			$_REQUEST['detalhes']='pais';
			$_REQUEST['acaid']=33;
			break;
		case '129708':
			$_REQUEST['detalhes']='pais';
			$_REQUEST['acaid']=30;
			break;
		case '129779':
			$_REQUEST['detalhes']='pais';
			$_REQUEST['acaid']=104;
			break;
		case '129773':
			$_REQUEST['detalhes']='pais';
			$_REQUEST['acaid']=28;
			break;
		case '129399':
			$_REQUEST['detalhes']='pais';
			$_REQUEST['acaid']=16;
			break;
		case '129361':
			$_REQUEST['detalhes']='pais';
			$_REQUEST['acaid']=1;
			break;
		case '129397':
			$_REQUEST['detalhes']='pais';
			$_REQUEST['acaid']=145;
			break;
		case '129326':
			$_REQUEST['detalhes']='pais';
			$_REQUEST['acaid']=146;
			break;
		case '129185':
			$_REQUEST['detalhes']='pais';
			$_REQUEST['acaid']=21;
			break;
		default:
			$_REQUEST['detalhes']='pais';
			$_REQUEST['acaid']=21;
			
	}
	

	
	/* configurações */
	ini_set("memory_limit", "1024M");
	set_time_limit(0);
	/* FIM configurações */
	
	?>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<script language="JavaScript" src="../includes/prototype.js"></script>
	<script language="JavaScript" src="../painel/js/painel.js"></script>
	<script language="JavaScript" src="../painel/js/detalhamentoindicador.js"></script>
	<script language="javascript" type="text/javascript" src="/includes/open_flash_chart/swfobject.js"></script>
	
	<?
	
	$local= explode("/",curPageURL());
	?>
	<?if ($local[2]=="simec.mec.gov.br" ){ ?>
		<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxQhVwj8ALbvbyVgNcB-R-H_S2MIRxTIdhrqjcwTK3xxl_Nu_YMC5SdLWg" type="text/javascript"></script>
		<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxQhVwj8ALbvbyVgNcB-R-H_S2MIRxTIdhrqjcwTK3xxl_Nu_YMC5SdLWg"; ?>
	<? } ?>
	<?if ($local[2]=="simec-d.mec.gov.br"){ ?>
		<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxRYtD8tuHxswJ_J7IRZlgTxP-EUtxT_Cz5IMSBe6d3M1dq-XAJNIvMcpg" type="text/javascript"></script>
		<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxRYtD8tuHxswJ_J7IRZlgTxP-EUtxT_Cz5IMSBe6d3M1dq-XAJNIvMcpg"; ?> 
	<? } ?>
	<?if ($local[2]=="simec" ){ ?>
		<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxTNzTBk8zukZFuO3BxF29LAEN1D1xSIcGWxF7HCjMwks0HURg6MTfdk1A" type="text/javascript"></script>
		<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxTNzTBk8zukZFuO3BxF29LAEN1D1xSIcGWxF7HCjMwks0HURg6MTfdk1A"; ?>
	<? } ?>
	<?if ($local[2]=="simec-d"){ ?>
		<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxTFm3qU4CVFuo3gZaqihEzC-0jfaRTY9Fe8UfzYeoYDxtThvI3nGbbZEw" type="text/javascript"></script>
		<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxTFm3qU4CVFuo3gZaqihEzC-0jfaRTY9Fe8UfzYeoYDxtThvI3nGbbZEw"; ?> 
	<? } ?>
	<?if ($local[2]=="simec-local"){ ?>
		<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxRzjpIxsx3o6RYEdxEmCzeJMTc4zBSMifny_dJtMKLfrwCcYh5B01Pq_g" type="text/javascript"></script>
		<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxRzjpIxsx3o6RYEdxEmCzeJMTc4zBSMifny_dJtMKLfrwCcYh5B01Pq_g"; ?> 	
	<? } ?>
	<?if ($local[2]=="painel.mec.gov.br"){ ?>
		<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxTPkFYZwQy2nvpGvFj08HQmPOt9ZBT2EJmQsTms0WQqU_5GvEj7bMZd7g" type="text/javascript"></script>
		<? $GKey = "ABQIAAAAwN0kvNsueYw8CBs704pusxTPkFYZwQy2nvpGvFj08HQmPOt9ZBT2EJmQsTms0WQqU_5GvEj7bMZd7g"; ?> 	
	<? } ?>
	
	
	<script>
	
	var marcadores = new Array();
	var detalhesMarcadores = new Array();
	
	function controleAcoes(ac) {
		for(i=0;i<document.getElementById("tabela").rows.length;i++) {
			if(document.getElementById("tabela").rows[i].id.substr(0,5) == "tr_m_") {
				document.getElementById("tabela").rows[i].cells[0].childNodes[0].title=ac;
				document.getElementById("tabela").rows[i].cells[0].childNodes[0].onclick();
			}
		}
	}
	
	function carregarAcao(acaid, indids, obj) {
	
		var tabela = obj.parentNode.parentNode.parentNode;
		var linha  = obj.parentNode.parentNode;
		if(obj.title == "mais") {
			obj.title="menos";
			obj.src="../imagens/menos.gif";
			var nlinha = tabela.insertRow(linha.rowIndex+1);
			nlinha.style.background = '#f5f5f5';
			var col0 = nlinha.insertCell(0);
			col0.colSpan=tabela.rows[0].cells.length;
			col0.id="colid_"+acaid;
			divCarregando(col0);
			ajaxatualizar('requisicao=listaIndicadores&detalhes=<? echo $_REQUEST['detalhes']; ?>&acaid='+acaid+'&indids='+indids, 'colid_'+acaid);
			divCarregado(col0);
		} else {
			obj.title="mais";
			obj.src="../imagens/mais.gif"
			tabela.deleteRow(linha.rowIndex+1);
		}
	
	}
	
	function createMarker(posn, title, icon, html,muncod) {
		var marker = new GMarker(posn, {title: title, icon: icon, draggable:false });
		if(html != false){
			GEvent.addListener(marker, "click", function() {
			marker.openInfoWindowHtml(html);
			});
		}
		return marker;
	}
	
	function exibePontos(obj){
		
		if(obj.checked == true){
			if(markerGroups[obj.value]){
				for (var i = 0; i < markerGroups[obj.value].length; i++) {
			        var marker = markerGroups[obj.value][i];
			        marker.show();
				}
			}	
		}else{
			if(markerGroups[obj.value]){
				for (var i = 0; i < markerGroups[obj.value].length; i++) {
			        var marker = markerGroups[obj.value][i];
			        marker.hide();
				}
			}
		}		
	
	}
	
	
	var markerGroups = { '1': []};
	
	function initialize(indid) {
		if (GBrowserIsCompatible()) { // verifica se o navegador é compatível
				document.getElementById('linha_mapa_'+indid).style.display='';
				document.getElementById('linha_mapa_'+indid+'_').style.display='';
				map = new GMap2(document.getElementById('div_exibe_mapa_'+indid)); // inicila com a div mapa
				var zoom = 4;	var lat_i = -14.689881; var lng_i = -52.373047;	//Brasil	
				map.setCenter(new GLatLng(lat_i,lng_i), parseInt(zoom)); //Centraliza e aplica o zoom
				
				// Início Controles
				map.addControl(new GMapTypeControl());
				map.addControl(new GLargeMapControl3D());
	//	        map.addControl(new GOverviewMapControl());
		        map.enableScrollWheelZoom();
		        map.addMapType(G_PHYSICAL_MAP);
		        // Fim Controles
		
		}
	}
	
	function fecharGlobo(indid) {
		document.getElementById('linha_mapa_'+indid).style.display = 'none';
		document.getElementById('linha_mapa_'+indid+'_').style.display = 'none';
		document.getElementById('div_exibe_mapa_'+indid).innerHTML = '';
		document.getElementById('info_mapa_'+indid).innerHTML = '';
	}
	
	function exibeGrafico(indid, params) {
	
		initialize(indid);
		divCarregando(document.getElementById('linha_mapa_'+indid));	
		var baseIcon2 = new GIcon();
		baseIcon2.iconSize = new GSize(14, 14);
		baseIcon2.iconAnchor = new GPoint(14, 14);
		baseIcon2.infoWindowAnchor = new GPoint(9, 2);
		baseIcon2.image='/imagens/tachinha_y.png';
		
		xml_filtro = 'estrategico.php?modulo=principal/painel_estrategico&acao=A&requisicao=exibeMapaDetalhamento&indid='+indid+'&filtro='+params;
			
		// Criando os Marcadores com o resultado
		GDownloadUrl(xml_filtro, function(data) {
			var xml = GXml.parse(data);
			
			var markers = xml.documentElement.getElementsByTagName("marker");
			
			if(markers.length > 0) {
				var lat_ant=0;
				var lng_ant=0;
					
				for (var i = 0; i < markers.length; i++) {
							
					var mundsc = markers[i].getAttribute("mundsc");
					var estuf = markers[i].getAttribute("estuf");
					var muncod = markers[i].getAttribute("muncod");
					var qtde = markers[i].getAttribute("qtde");
					var valor = markers[i].getAttribute("valor");
					var descricao = markers[i].getAttribute("descricao");
					var endereco = markers[i].getAttribute("endereco");
					var telefone = markers[i].getAttribute("telefone");
					title = mundsc; 
											
					icon = baseIcon2;
					
					var lat = markers[i].getAttribute("lat");
					var lng = markers[i].getAttribute("lng");
					
					var html;
							
					html = "<div style=\"font-family:verdana;font-size:11px;padding:10px\" >";
					html += "<b>Localização:</b> " + mundsc + "/" + estuf + "<br /><br />";
					html += "<b>Quantidade:</b> " + qtde + "<br /><br />";
					if(valor != '') {
						html += "<b>Valor R$:</b> " + valor + "<br /><br />";
					}
					html += "</div>";
	
					// Verifica pontos em um mesmo lugar e move o seguinte para a direita
					if(lat_ant==markers[i].getAttribute("lat") && lng_ant==markers[i].getAttribute("lng"))
						var point = new GLatLng(markers[i].getAttribute("lat"),	markers[i].getAttribute("lng"));
					else
						var point = new GLatLng(markers[i].getAttribute("lat"),	parseFloat(markers[i].getAttribute("lng"))+0.0005);				
		
					lat_ant=markers[i].getAttribute("lat");
					lng_ant=markers[i].getAttribute("lng");
					
					// Cria o marcador na tela
					var marker = createMarker(point,title,icon,html);
					
					markerGroups['1'].push(marker);
					
					marcadores[muncod]=marker;
					detalhesMarcadores[muncod]=html;
					
					map.addOverlay(marker);
					
			}
			}else{
				alert("Não existem municipios neste indicador")
			}
		});
		
	
		var myAjax = new Ajax.Request(
			'estrategico.php?modulo=principal/painel_estrategico&acao=A',
			{
				method: 'post',
				parameters: 'requisicao=exibeInformacoesMapa&indid='+indid+'&filtro='+params,
				asynchronous: false,
				onComplete: function(resp) {
					extrairScript(resp.responseText);
					document.getElementById('info_mapa_'+indid).innerHTML = resp.responseText;
				},
				onLoading: function(){
					document.getElementById(iddestinatario).innerHTML = 'Carregando...';
				}
			});
		
		
		divCarregado(document.getElementById('linha_mapa_'+indid));
	
	
	}
	
	function abrebalao(marcador){
		marcadores[marcador].openInfoWindowHtml(detalhesMarcadores[marcador]);
	}
	
	</script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<style>
	.boxindicador {
		width:400px;
		height:100px;
		position: absolute;
		background-color: #FCFCFC;
		border: solid 1px #000000;
		padding: 3px;
		margin-left:30px;
		display:none;
	}
	</style>
	<?
	if($_REQUEST['requisicao']) {
		$_REQUEST['requisicao']($_REQUEST);
		exit;
	}
	?>
	
	<?php
	
	if($_REQUEST['indid']) {
		$sql = "SELECT * FROM painel.indicador WHERE indid='".$_REQUEST['indid']."'";
		$dadosi = $db->pegaLinha($sql);
		$estrutura = getEstruturaRegionalizacao($dadosi['regid']);
		$estloop = $estrutura;
	}
	
	if($estloop) {
		
		$html .= "<table cellSpacing=0 cellPadding=3 style=\"width:100%;background-color:#FCFCFC;\">";
		
		do {
			$estor[] = $estloop['atu'];
			$estloop = $estloop['sub'];
		} while ($estloop['atu']);
		
		for($i=(count($estor)-1);$i >= 0;$i--) {
			$estruturanomeordenada[] = "<b>".$estor[$i]['regdescricao']."</b>";
			
			if($estor[$i]['rgavisao'] == $_REQUEST['detalhes']) {
				
				if($estor[$i]['rgafiltroreg']) {
					$filtro1 .= str_replace(array("{".$estor[$i]['rgaidentificador']."}"),array($_REQUEST[$estor[$i]['rgaidentificador']]),$estor[$i]['rgafiltroreg']);
				} else {
					$filtro1 .= str_replace(array("{".$estor[$i]['rgaidentificador']."}"),array($_REQUEST[$estor[$i]['rgaidentificador']]),$estor[$i]['rgafiltro']);
				}
				
				$sql = str_replace(array("{".$estor[$i]['rgaidentificador']."}", "{clausulaindicador}","{clausulaacao}", "{clausulasecretaria}","{ano}"),array($_REQUEST[$estor[$i]['rgaidentificador']], (($_REQUEST['indid'])?"AND ind.indid!='".$_REQUEST['indid']."'":""), (($_REQUEST['acaid'])?"AND aca.acaid='".$_REQUEST['acaid']."'":""), (($_REQUEST['secid'])?"AND ind.secid='".$_REQUEST['secid']."'":""), date("Y")), $estor[$i]['rgasqlindicadores']);
				$dadosreg = $db->pegaLinha(str_replace(array("{".$estor[$i]['rgaidentificador']."}"),array($_REQUEST[$estor[$i]['rgaidentificador']]),$estor[$i]['regsql']));
				$icones = str_replace(array("{ano}","{estuf}","{municipiocod}","{entnumcpfcnpj}","{estcod}","{anos}","{mundescricao}","{muncod}","{muncodcompleto}","{unicod}","{entid}","{estdescricao}","{entcodent}"),array(date("Y"),$dadosreg['estuf'],substr($dadosreg['muncod'],0,6),$dadosreg['entnumcpfcnpj'],$dadosreg['estcod'],(date("Y")-1),$dadosreg['mundescricao'],$dadosreg['muncod'],$dadosreg['muncodcompleto'],$dadosreg['unicod'],$dadosreg['entid'],$dadosreg['estdescricao'], $dadosreg['entcodent']),stripslashes($estor[$i]['regicones']));
			}
		}
		
		$html .= "<tr>
					<td rowspan='4' align='center' width='140'><img src=\"../painel/images/".$dadosi['regid'].".gif\"></td>
					<td>".implode(" >> ", $estruturanomeordenada)."</td></tr>";
		$html .= "<tr><td style=\"font-size:12px;\"><b>".str_replace(array("{indid}"),array($_REQUEST['indid']),$dadosreg['descricao'])."</b></td></tr>";
		$html .= "<tr><td>".$icones."</td></tr>";
		$html .= "</table>";
		
	} else {
		
		
		
		$qry = "SELECT * FROM painel.regagreg rga LEFT JOIN painel.regionalizacao reg ON reg.regid=rga.regid WHERE rgavisao='".$_REQUEST['detalhes']."'";
		
		$rga = $db->pegaLinha($qry);
		
		$sql = str_replace(array("{".$rga['rgaidentificador']."}", "{clausulaindicador}","{clausulaacao}", "{clausulasecretaria}", "{ano}"),array($_REQUEST[$rga['rgaidentificador']], (($_REQUEST['indid'])?"AND ind.indid!='".$_REQUEST['indid']."'":""), (($_REQUEST['acaid'])?"AND aca.acaid='".$_REQUEST['acaid']."'":""), (($_REQUEST['secid'])?"AND ind.secid='".$_REQUEST['secid']."'":""), date("Y")),$rga['rgasqlindicadores']);
		
		
		
		$dadosreg = $db->pegaLinha(str_replace(array("{".$rga['rgaidentificador']."}"),array($_REQUEST[$rga['rgaidentificador']]),$rga['regsql']));

		
		if($rga['rgafiltroreg']) {
			$filtro1 .= str_replace(array("{".$rga['rgaidentificador']."}"),array($_REQUEST[$rga['rgaidentificador']]),$rga['rgafiltroreg']);
		} else {
			$filtro1 .= str_replace(array("{".$rga['rgaidentificador']."}"),array($_REQUEST[$rga['rgaidentificador']]),$rga['rgafiltro']);
		}
		
		$estrutura = getEstruturaRegionalizacao($rga['regid']);
		
		$icones = str_replace(array("{ano}","{estuf}","{municipiocod}","{entnumcpfcnpj}","{estcod}","{anos}","{mundescricao}","{muncod}","{muncodcompleto}","{unicod}","{entid}","{estdescricao}", "{entcodent}"),array(date("Y"),$dadosreg['estuf'],substr($dadosreg['muncod'],0,6),$dadosreg['entnumcpfcnpj'],$dadosreg['estcod'],(date("Y")-1),$dadosreg['mundescricao'],$dadosreg['muncod'],$dadosreg['muncodcompleto'],$dadosreg['unicod'],$dadosreg['entid'],$dadosreg['estdescricao'], $dadosreg['entcodent']),stripslashes($rga['regicones']));
		
		$estloop = $estrutura;
		
		do {
			$estor[] = $estloop['atu'];
			$estloop = $estloop['sub'];
		} while ($estloop['atu']);
	
		for($i=(count($estor)-1);$i >= 0;$i--) {
			$estruturanomeordenada[] = "<b>".$estor[$i]['regdescricao']."</b>";
		}
		
	}
	
	$inds = $db->carregar($sql);
	
	// agrupando indices por eixo
	if($inds[0]) {
		foreach($inds as $ind) {
			if($ind['indid'] != $_REQUEST['indid'])
				$arrIndAgrup[$ind['acaid']][] = array("indid" => $ind['indid']);
				$arrAcaInfo[$ind['acaid']] = $ind['acadsc'];
		}
	}
	
	// processando estrutura
	$html .= "<table cellSpacing=0 cellPadding=3 class=listagem style=\"width:100%;color:#888888;\" id=\"tabela\">";
	
	if($_REQUEST['indid']) {
		
		$sql = "SELECT indid, acaid, unmid, foo.regid, indcumulativo, indcumulativovalor, indnome, sum(qtde) as qtde, indqtdevalor, CASE WHEN indqtdevalor = TRUE THEN to_char(sum(valor), '999g999g999g999d99') ELSE '-' END as valor, secdsc, umedesc, regdescricao from(
					SELECT d.indid, acaid, d.unmid, d.indnome, d.secid, d.umeid, d.regid, d.indcumulativo, d.indcumulativovalor, d.indqtdevalor, CASE WHEN d.indcumulativo='S' THEN sum(d.qtde)
						WHEN d.indcumulativo='N' THEN
							CASE WHEN d.sehstatus='A' THEN sum(d.qtde)
							ELSE 0 END
						WHEN d.indcumulativo='A' THEN
							CASE when d.dpeanoref=( SELECT dd.dpeanoref FROM painel.seriehistorica ss 
										   INNER JOIN painel.detalheperiodicidade dd ON dd.dpeid=ss.dpeid 
										   WHERE ss.indid = d.indid AND ss.sehstatus='A') THEN sum(d.qtde)
							ELSE 0 END
						END as qtde,
					CASE 	WHEN d.indcumulativovalor='S' THEN sum(d.valor)
						WHEN d.indcumulativovalor='N' THEN
							CASE when d.sehstatus='A' THEN sum(d.valor)
							ELSE 0 END
						WHEN d.indcumulativovalor='A' then
							CASE when d.dpeanoref=( SELECT dd.dpeanoref FROM painel.seriehistorica ss 
										   INNER JOIN painel.detalheperiodicidade dd ON dd.dpeid=ss.dpeid 
										   WHERE ss.indid = d.indid AND ss.sehstatus='A') THEN sum(d.valor)
							ELSE 0 end
						END as valor
					FROM painel.v_detalheindicadorsh d 
					WHERE d.indid=".$_REQUEST['indid']."
					".$filtro1." GROUP BY d.indid, acaid, d.unmid,d.indnome,d.indcumulativo,d.indcumulativovalor,d.sehstatus,d.dpeanoref,d.secid,d.umeid,d.regid,d.indqtdevalor
					) foo 
					INNER JOIN painel.secretaria sec ON sec.secid=foo.secid 
					INNER JOIN painel.unidademeta ume ON ume.umeid=foo.umeid
					INNER JOIN painel.regionalizacao reg ON reg.regid=foo.regid 
					GROUP BY indid, acaid, foo.unmid, indnome, secdsc, umedesc, regdescricao, indcumulativovalor, indcumulativo, indqtdevalor, foo.regid 
					ORDER BY indid";
		
		$indicadorP = $db->pegaLinha($sql);
		
		$html .= "<tr>";
		$html .= "<td style=\"width:100%;text-align:center;font-weight:bold;background-color:#DBDBDB;font-size:14px;color: rgb(0, 85, 0);\" colspan=9 >";
		$html .= $indicadorP['acadsc'];
		$html .= "</td></tr>";
		
		$html .= "<tr>";
		$html .= "<td>";
		
		
		$html .= "<table cellSpacing=0 cellPadding=3 class=listagem style=\"width:100%;color:#888888;\">";
		
		$html .= "<tr>";
		$html .= "<td class=\"SubTituloCentro\">&nbsp;</td>";
		$html .= "<td class=\"SubTituloCentro\">Cod</td>";
		$html .= "<td class=\"SubTituloCentro\">Nome do indicador</td>";
		$html .= "<td class=\"SubTituloCentro\">Secretaria</td>";
		$html .= "<td class=\"SubTituloCentro\">Regionalização</td>";
		$html .= "<td class=\"SubTituloCentro\">Produto</td>";
		$html .= "<td class=\"SubTituloCentro\">Qtde</td>";
		$html .= "<td class=\"SubTituloCentro\">R$</td>";
		$html .= "</tr>";
	
		if($_REQUEST['detalhes'])
			$rgaidentificador = $db->pegaUm("select rgaidentificador from painel.regagreg where rgavisao = '".$_REQUEST['detalhes']."'");
		
		$html .= processarLinhaDetalhamentoIndicadores($indicadorP, array('detalhes' => $_REQUEST['detalhes'], "rgaidentificador" => $rgaidentificador), true);
		
		$html .= "</table>";
		
		$html .= "</td>";
		$html .= "</tr>";
		
		$html .= "<tr>";
		$html .= "<td class=\"SubTituloEsquerda\">Acesse outros indicadores...</td>";
		$html .= "</tr>";
		
	
	}
	
	if($arrAcaInfo) {
		foreach($arrAcaInfo as $acaid => $acadsc) {
			$html .= "<tr id=\"tr_m_".$acaid."\">";
			$html .= "<td style=\"width:100%;text-align:left;font-weight:bold;background-color:#F3F3F3;font-size:14px;color: rgb(0, 85, 0);\" colspan=8 >";
			$html .= "<img src=\"../imagens/mais.gif\" style=\"cursor:pointer;\" title=\"mais\" id=\"imgc_".$acaid."\" onclick=\"carregarAcao('".$acaid."', '".md5_encrypt(serialize($arrIndAgrup[$acaid]))."', this);\"> ";
			$html .= "<span style=\"cursor:pointer;\" onclick=\"document.getElementById('imgc_".$acaid."').onclick();\">".$acadsc."</span>";
			$html .= "</td></tr>";
		}
	} else {
		$html .= "<tr>";
		$html .= "<td align=\"center\" colspan=\"9\">Não existem indicadores.</td>";
		$html .= "</tr>";
	}
	
	$html .= "</table>";
	
	$html .= "<script>controleAcoes('mais');</script>";
	
	echo $html;
	
	
}

function dadosFinanceiros($dados) {
	global $db;
	
	$_REQUEST = array("espandir"=>true,"selecao"=>"pac","nottpmid"=>false,"notesdid_"=>false,"notesdid"=>false,"notestuf"=>false,"notmuncod"=>false,"notusucpfanalista"=>false,"notresid"=>false,"colunas"=>"{tipoobra}{ptodescricao}","esdid_"=>"{228}{214}","esdid"=>"{228}{214}","esferafiltro"=>"m");
	
	include_once APPRAIZ."/par/modulos/principal/painelGerenciamento_resultado.inc";
	
}

function zerarHistorico(){
	
	global $db;
	$sehid = !$sehid ? $_GET['sehid'] : $sehid;
	$docid = !$docid ? $_GET['docid'] : $docid;
	
	$sql = "UPDATE workflow.documento SET hstid = NULL WHERE docid = $docid;
			UPDATE painel.seriehistorica SET sehstatus = 'I' WHERE sehid = $sehid;
			DELETE FROM workflow.historicodocumento WHERE docid = $docid;
			UPDATE workflow.documento SET esdid = ".WK_ESTADO_DOC_EM_EXECUCAO." WHERE docid = $docid;";
	$db->executar($sql);
	$db->commit(); 
	echo "<script>
			window.location = 'estrategico.php?modulo=principal/painel_estrategico&acao=A&requisicao=editaDataExecucao&sehid=$sehid';
		</script>";
}

function editaDataExecucao(){
	global $db;
	$sehid = !$sehid ? $_GET['sehid'] : $sehid;
	$sql = "SELECT
				docid
			FROM 
				painel.detalhemetaindicador dmi
			INNER JOIN painel.seriehistorica seh ON seh.dmiid = dmi.dmiid
			WHERE
				sehid = $sehid";
	$docid = $db->pegaUm($sql);
	if( $docid ){
		$sql = "SELECT
					true
				FROM 
					workflow.historicodocumento
				WHERE
					docid = $docid";
		$possuiHistorico = $db->pegaUm($sql);
	}
	if($_POST['sehqtde']){
		$sehqtde = str_replace(Array('.',','),Array('','.'),$_POST['sehqtde']);
		$sql = "UPDATE painel.seriehistorica SET sehqtde = '$sehqtde' WHERE sehid = $sehid";
		$db->executar($sql);
		$db->commit($sql);
	}else{
		$sql = "SELECT sehqtde FROM painel.seriehistorica WHERE sehid = $sehid AND sehstatus = 'A'";
		$sehqtde = $db->pegaUm($sql);
	}
	if($_POST['sehdtcoleta']){
		$sehdtcoleta = formata_data_sql($_POST['sehdtcoleta']);
		$sql = "UPDATE painel.seriehistorica SET sehdtcoleta = '$sehdtcoleta' WHERE sehid = $sehid";
		$db->executar($sql);
		$db->commit($sql);
	}else{
		$sql = "SELECT sehdtcoleta FROM painel.seriehistorica WHERE sehid = $sehid AND sehstatus = 'A'";
		$sehdtcoleta = $db->pegaUm($sql);
	}
	?>
	<html>
			<head>
				<script language="JavaScript" src="../includes/funcoes.js"></script>
				<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
				<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
				<script src="/includes/jquery.mobile-1.0.1/jquery-1.7.1.min.js"></script>
				<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
				<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
		    </head>
			<body>
			<?php monta_titulo("Editar Data",""); ?>
				<form id="form_data"  action="" method="post" >
					<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
						<tr>
							<td width="25%" class="SubtituloDireita" >Data de Execução:</td>
							<td>
							<?php 
								$sehdtcoleta = $sehdtcoleta ? formata_data($sehdtcoleta) : ""; 
								echo campo_data2("sehdtcoleta","S","S","","","","",$sehdtcoleta); 
							?>
							</td>
						</tr>
						<?php
						
						$sql = "SELECT DISTINCT
									i.indid
								FROM 
									pde.monitoratipoindicador mti
								INNER JOIN painel.indicador 			  i ON mti.mtiid = i.mtiid
								INNER JOIN painel.metaindicador 		met ON met.indid = i.indid
								INNER JOIN painel.detalhemetaindicador  dmi ON dmi.metid = met.metid
								INNER JOIN painel.seriehistorica 		seh ON seh.dmiid = dmi.dmiid
								WHERE
									sehid = $sehid AND mti.mtiid = 3";
						
						$indid = $db->pegaUm($sql); 
						$indid = $indid ? $indid : 'null';
						$sql = "select indcumulativo from painel.indicador where indid = $indid";
						$indcumulativo = $db->pegaUm($sql);
						$indid = $indid == 'null' ? null : $indid;
						if( $indid && $indcumulativo != 'A' && $indcumulativo != 'I' ){
						?>
						<tr>
							<td width="25%" class="SubtituloDireita" >Valor de Execução:</td>
							<td>
								<?php echo campo_texto('sehqtde', 'N', 'S', '', 11, 11, '[.###].###,##', '','','','','','',formata_valor($sehqtde,2,true)); ?>
								<script>jQuery('[name=sehqtde]').keyUp();</script>
							</td>
						</tr>
						<?php }?>
						<tr>
							<td width="25%" class="SubtituloTabela" ></td>
							<td class="SubtituloTabela" >
								<input type="hidden" id="docid" name="docid" value="<?=$docid ?>" />
								<input type="hidden" id="dmiid" name="dmiid" value="<?php echo $dmiid ?>" />
								<input type="button" id="btn_salvar"   value="Salvar" 	onclick="jQuery('#form_data').submit()" />
								<input type="button" id="btn_cancelar" value="Cancelar" onclick="window.close()" />
								<?php if( $possuiHistorico == 't' || $sehqtde != '' || $sehdtcoleta != '' ){?>
								<input type="button" style="float:right" value="Zerar Histórico" onclick="if(confirm('Deseja zerar o histórico deste item?')){window.location = 'estrategico.php?modulo=principal/painel_estrategico&acao=A&requisicao=zerarHistorico&docid=<?=$docid ?>&sehid=<?=$sehid ?>';}" />
								<?php }?>
							</td>
						</tr>
					</table>
				</form>
				<?php 
				if($_POST){
					echo "<script>window.opener.location.reload();</script>";
				}	
				?>
			</body>
		</html>
	<?php	
	exit;
}

function exibeMapaIndicador()
{
	global $db;
	$indid = $_GET['indid'];
	?>
	<html>
		<head>
			<title>Mapa do Indicador</title>
			<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
			<script language="JavaScript" src="../includes/funcoes.js"></script>
			<script language="JavaScript" src="../includes/prototype.js"></script>
			<script language="JavaScript" src="../painel/js/painel.js"></script>
			<script language="JavaScript" src="../painel/js/detalhamentoindicador.js"></script>
			<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
			<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		</head>
		<body>
			<?php
			$local= explode("/",curPageURL());
			?>
			<?if ($local[2]=="simec.mec.gov.br" ){ ?>
				<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxQhVwj8ALbvbyVgNcB-R-H_S2MIRxTIdhrqjcwTK3xxl_Nu_YMC5SdLWg" type="text/javascript"></script>
				<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxQhVwj8ALbvbyVgNcB-R-H_S2MIRxTIdhrqjcwTK3xxl_Nu_YMC5SdLWg"; ?>
			<? } ?>
			<?if ($local[2]=="simec-d.mec.gov.br"){ ?>
				<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxRYtD8tuHxswJ_J7IRZlgTxP-EUtxT_Cz5IMSBe6d3M1dq-XAJNIvMcpg" type="text/javascript"></script>
				<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxRYtD8tuHxswJ_J7IRZlgTxP-EUtxT_Cz5IMSBe6d3M1dq-XAJNIvMcpg"; ?> 
			<? } ?>
			<?if ($local[2]=="simec" ){ ?>
				<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxTNzTBk8zukZFuO3BxF29LAEN1D1xSIcGWxF7HCjMwks0HURg6MTfdk1A" type="text/javascript"></script>
				<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxTNzTBk8zukZFuO3BxF29LAEN1D1xSIcGWxF7HCjMwks0HURg6MTfdk1A"; ?>
			<? } ?>
			<?if ($local[2]=="simec-d"){ ?>
				<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxTFm3qU4CVFuo3gZaqihEzC-0jfaRTY9Fe8UfzYeoYDxtThvI3nGbbZEw" type="text/javascript"></script>
				<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxTFm3qU4CVFuo3gZaqihEzC-0jfaRTY9Fe8UfzYeoYDxtThvI3nGbbZEw"; ?> 
			<? } ?>
			<?if ($local[2]=="simec-local"){ ?>
				<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxRzjpIxsx3o6RYEdxEmCzeJMTc4zBSMifny_dJtMKLfrwCcYh5B01Pq_g" type="text/javascript"></script>
				<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxRzjpIxsx3o6RYEdxEmCzeJMTc4zBSMifny_dJtMKLfrwCcYh5B01Pq_g"; ?> 	
			<? } ?>
			<?if ($local[2]=="painel.mec.gov.br"){ ?>
				<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxTPkFYZwQy2nvpGvFj08HQmPOt9ZBT2EJmQsTms0WQqU_5GvEj7bMZd7g" type="text/javascript"></script>
				<? $GKey = "ABQIAAAAwN0kvNsueYw8CBs704pusxTPkFYZwQy2nvpGvFj08HQmPOt9ZBT2EJmQsTms0WQqU_5GvEj7bMZd7g"; ?> 	
			<? } ?>
			<script>
				var marcadores = new Array();
				var detalhesMarcadores = new Array();
				
				function controleAcoes(ac) {
					for(i=0;i<document.getElementById("tabela").rows.length;i++) {
						if(document.getElementById("tabela").rows[i].id.substr(0,5) == "tr_m_") {
							document.getElementById("tabela").rows[i].cells[0].childNodes[0].title=ac;
							document.getElementById("tabela").rows[i].cells[0].childNodes[0].onclick();
						}
					}
				}
				function createMarker(posn, title, icon, html,muncod) {
					var marker = new GMarker(posn, {title: title, icon: icon, draggable:false });
					if(html != false){
						GEvent.addListener(marker, "click", function() {
						marker.openInfoWindowHtml(html);
						});
					}
					return marker;
				}
				
				function exibePontos(obj){
					
					if(obj.checked == true){
						if(markerGroups[obj.value]){
							for (var i = 0; i < markerGroups[obj.value].length; i++) {
						        var marker = markerGroups[obj.value][i];
						        marker.show();
							}
						}	
					}else{
						if(markerGroups[obj.value]){
							for (var i = 0; i < markerGroups[obj.value].length; i++) {
						        var marker = markerGroups[obj.value][i];
						        marker.hide();
							}
						}
					}		
				
				}
				
				
				var markerGroups = { '1': []};
				
				function initialize(indid) {
					if (GBrowserIsCompatible()) { // verifica se o navegador é compatível
							document.getElementById('linha_mapa_'+indid).style.display='';
							document.getElementById('linha_mapa_'+indid+'_').style.display='';
							map = new GMap2(document.getElementById('div_exibe_mapa_'+indid)); // inicila com a div mapa
							var zoom = 4;	var lat_i = -14.689881; var lng_i = -52.373047;	//Brasil	
							map.setCenter(new GLatLng(lat_i,lng_i), parseInt(zoom)); //Centraliza e aplica o zoom
							
							// Início Controles
							map.addControl(new GMapTypeControl());
							map.addControl(new GLargeMapControl3D());
				//	        map.addControl(new GOverviewMapControl());
					        map.enableScrollWheelZoom();
					        map.addMapType(G_PHYSICAL_MAP);
					        // Fim Controles
					
					}
				}
				
				function exibeGrafico(indid, params) {
					initialize(indid);
					divCarregando(document.getElementById('linha_mapa_'+indid));	
					var baseIcon2 = new GIcon();
					baseIcon2.iconSize = new GSize(14, 14);
					baseIcon2.iconAnchor = new GPoint(14, 14);
					baseIcon2.infoWindowAnchor = new GPoint(9, 2);
					baseIcon2.image='/imagens/tachinha_y.png';
					
					xml_filtro = 'estrategico.php?modulo=principal/painel_estrategico&acao=A&requisicao=exibeMapaDetalhamento&indid='+indid+'&filtro='+params;
						
					// Criando os Marcadores com o resultado
					GDownloadUrl(xml_filtro, function(data) {
						var xml = GXml.parse(data);
						
						var markers = xml.documentElement.getElementsByTagName("marker");
						
						if(markers.length > 0) {
							var lat_ant=0;
							var lng_ant=0;
								
							for (var i = 0; i < markers.length; i++) {
										
								var mundsc = markers[i].getAttribute("mundsc");
								var estuf = markers[i].getAttribute("estuf");
								var muncod = markers[i].getAttribute("muncod");
								var qtde = markers[i].getAttribute("qtde");
								var valor = markers[i].getAttribute("valor");
								var descricao = markers[i].getAttribute("descricao");
								var endereco = markers[i].getAttribute("endereco");
								var telefone = markers[i].getAttribute("telefone");
								title = mundsc; 
														
								icon = baseIcon2;
								
								var lat = markers[i].getAttribute("lat");
								var lng = markers[i].getAttribute("lng");
								
								var html;
										
								html = "<div style=\"font-family:verdana;font-size:11px;padding:10px\" >";
								html += "<b>Localização:</b> " + mundsc + "/" + estuf + "<br /><br />";
								html += "<b>Quantidade:</b> " + qtde + "<br /><br />";
								if(valor != '') {
									html += "<b>Valor R$:</b> " + valor + "<br /><br />";
								}
								html += "</div>";
				
								// Verifica pontos em um mesmo lugar e move o seguinte para a direita
								if(lat_ant==markers[i].getAttribute("lat") && lng_ant==markers[i].getAttribute("lng"))
									var point = new GLatLng(markers[i].getAttribute("lat"),	markers[i].getAttribute("lng"));
								else
									var point = new GLatLng(markers[i].getAttribute("lat"),	parseFloat(markers[i].getAttribute("lng"))+0.0005);				
					
								lat_ant=markers[i].getAttribute("lat");
								lng_ant=markers[i].getAttribute("lng");
								
								// Cria o marcador na tela
								var marker = createMarker(point,title,icon,html);
								
								markerGroups['1'].push(marker);
								
								marcadores[muncod]=marker;
								detalhesMarcadores[muncod]=html;
								
								map.addOverlay(marker);
								
						}
						}else{
							alert("Não existem municipios neste indicador")
						}
					});
					
				
					var myAjax = new Ajax.Request(
						'estrategico.php?modulo=principal/painel_estrategico&acao=A',
						{
							method: 'post',
							parameters: 'requisicao=exibeInformacoesMapa&indid='+indid+'&filtro='+params,
							asynchronous: false,
							onComplete: function(resp) {
								extrairScript(resp.responseText);
								document.getElementById('info_mapa_'+indid).innerHTML = resp.responseText;
							},
							onLoading: function(){
								document.getElementById(iddestinatario).innerHTML = 'Carregando...';
							}
						});
					
					
					divCarregado(document.getElementById('linha_mapa_'+indid));
				
				
				}
				
				function abrebalao(marcador){
					marcadores[marcador].openInfoWindowHtml(detalhesMarcadores[marcador]);
				}
			</script>
			<table width="100%" height="100%" >
				<tr>
					<td width="50%" height="100%" valign="top" >
						<div style="width:100%;height:100%"  id="div_exibe_mapa_<?php echo $indid ?>" ></div>
						<div style="width:100%;height:100%"  id="linha_mapa_<?php echo $indid ?>" ></div>
						<div style="width:100%;height:100%"  id="linha_mapa_<?php echo $indid ?>_" ></div>
					</td>
					<td valign="top" ><?php exibeInformacoesMapa(array("indid" => $indid)) ?></td>
				</tr>
			</table>
			<script>
				initialize(<?php echo $indid ?>);
				exibeGrafico(<?php echo $indid ?>,'');
			</script>
						
		</body>
	</html>
	<?php
}

function exibeHistoricoMetasIndicador($post){

	global $db;
	
	$sql = "select 
				i.indid
				,sh.sehid
				,i.indnome
 				,mti.mtidsc
 				,dmi.dmiqtde
 				,dpe.dpedsc
 				,sehdtcoleta
 				,to_char(sehdtcoleta, 'YYYYMMDD') as data_execucao
 				,to_char(sehdtcoleta, 'DD/MM/YYYY') as data_execucao_br
 				,dmidataexecucao
 				,dpedatafim			
 				,case when dmidataexecucao is not null then to_char(dmidataexecucao, 'YYYYMMDD')
 					  else to_char(dpedatafim, 'YYYYMMDD') end as data_meta
 				,hst.aedid
 				,i.unmid
 				,sh.sehqtde
 				,dmi.dmiqtde
 				,i.estid
 				,dmi.dmiid
			from
				painel.indicador i
			inner join
				pde.monitoratipoindicador 		mti ON mti.mtiid = i.mtiid
			inner join 
				painel.metaindicador 			met ON met.indid = i.indid
			inner join
				painel.detalhemetaindicador 	dmi ON dmi.metid = met.metid
			inner join
				painel.detalheperiodicidade 	dpe ON dmi.dpeid = dpe.dpeid					
			left join
				painel.seriehistorica 			sh ON sh.dmiid = dmi.dmiid
			left join
				painel.detalheseriehistorica	dsh on dsh.sehid = sh.sehid
			left join
				workflow.documento				doc ON doc.docid = dmi.docid
			left join
				workflow.historicodocumento  	hst ON hst.hstid = doc.hstid				
			where
				dmi.metid = {$post['metid']}
			and
				dmi.dmistatus = 'A'
			order by dmidataexecucao desc, dpedatafim desc";

// 	ver($sql);
// 	die;

	$rs = $db->carregar($sql);
	
	if($rs){
		
		echo '<table width="100%"><tr><td width="15" align="left" valign="top"><img src="../imagens/seta_filho.gif" border="0" /></td><td align="left" valign="top">
				  <table class="listagem" width="100%" align="center" cellpadding="0" cellspacing="0">
					<thead>
						<tr>
							<th>Descrição</th>
							<th>Tipo</th>
							<th width="145">Executado</th>
							<th width="120">Meta</th>
							<th width="260">Desempenho</th>
						</tr>
					</thead>
					<tbody>';
		
		foreach($rs as $dados){
			
			// Parametros
			$ico 				= '';
			$icoMsg 			= '';
			$cor_fonte 			= false;
			$cor_fonte_geral 	= false;
			$img_atrazo 		= "";
			$cor_prazo 			= "";
			$texto_prazo 		= false;
			
			// Não executado
			if($dados['aedid'] == WK_MON_EST_AEDID_NAO_EXECUTAR){
				$cor_prazo 		= "#E95646";
				$texto_prazo 	= "Não Executado";
				$ico 			= 'exclamacao_checklist_vermelho.png';
				$icoMsg 		= 'Não Executado';
			}else
			// Executado
			if($dados['data_execucao'] && $dados['data_meta']){
				if($dados['data_execucao'] <= $dados['data_meta']){
					$cor_prazo  = "#80BC44";
					$ico 		= 'check_checklist.png';
					$icoMsg 	= 'Executado';
				}else{
					$cor_prazo  = "#80BC44";
					$cor_fonte  = "#FF0000";
					$ico 		= 'check_checklist_vermelho.png';
					$icoMsg 	= 'Executado fora do prazo';
				}
			}else			
			// À executar
			if(($dados['data_meta']-date('Ymd'))<=5 && ($dados['data_meta']-date('Ymd'))>=0 && empty($situacao) && empty($dados['data_execucao'])){
				$cor_prazo 		= "#FFC211";
				$texto_prazo 	= "A Executar";
				$ico 			= 'exclamacao_checklist.png';
				$icoMsg 		= 'A Executar';
			}else
				// Não informado
				if($dados['data_meta'] < date('Ymd') && empty($dados['aedid']) && empty($dados['data_execucao'])){
				$cor_prazo 		= "#E95646";
				$texto_prazo 	= "Não Informado";
				$ico 			= 'exclamacao_checklist_vermelho.png';
				$icoMsg 		= 'Não Informado';
			}
			
			// Calcula porcentagem
			if($dados['sehqtde'] && $dados['dmiqtde']>0){
				$porcentagem = round((($dados['sehqtde'] ? $dados['sehqtde'] : 1)/($dados['dmiqtde'] ? $dados['dmiqtde'] : 1))*100,2);
			}else{
				$porcentagem = 0;
			}
			
			//Verifica se existe medidor do detalhe
			$sql = "select dmdestavel, dmdcritico from painel.detalhemetaindicador where dmiid = {$dados['dmiid']}";
			$arrEstavel = $db->pegaLinha($sql);
			
			// Monta medidores
			unset($arrMedidor);
			if($arrEstavel['dmdestavel'] && $arrEstavel['dmdcritico']){				
				$arrMedidor[0] = array("inicio" => 50, "fim" => ($arrEstavel['dmdcritico'] >= 66.6 ? $arrEstavel['dmdcritico'] : 66.6), "cor" => "#E95646", "bgcolor" => "#E95646");
				$arrMedidor[1] = array("inicio" => ($arrEstavel['dmdcritico'] >= 66.6 ? $arrEstavel['dmdcritico'] : 66.6), "fim" => ($arrEstavel['dmdestavel'] >= 83.2 ? $arrEstavel['dmdestavel'] : 83.2), "cor" => "#FFFF00", "bgcolor" => "#FFC211");
				$arrMedidor[2] = array("inicio" => ($arrEstavel['dmdestavel'] >= 83.2 ? $arrEstavel['dmdestavel'] : 83.2), "fim" => 100, "cor" => "#80BC44", "bgcolor" => "#80BC44");
			}else
			if($dados['estid'] == 2){ //Menor melhor
				$img_indicador = "indicador-vermelha.png";
				$arrMedidor[0] = array("inicio" => 50, "fim" => 66.6, "cor" => "#80BC44", "bgcolor" => "#80BC44");
				$arrMedidor[1] = array("inicio" => 66.6, "fim" => 83.2, "cor" => "#FFFF00", "bgcolor" => "#FFC211");
				$arrMedidor[2] = array("inicio" => 83.2, "fim" => 100, "cor" => "#E95646", "bgcolor" => "#E95646");
			}else{
				$arrMedidor[0] = array("inicio" => 50, "fim" => 66.6, "cor" => "#E95646", "bgcolor" => "#E95646");
				$arrMedidor[1] = array("inicio" => 66.6, "fim" => 83.2, "cor" => "#FFFF00", "bgcolor" => "#FFC211");
				$arrMedidor[2] = array("inicio" => 83.2, "fim" => 100, "cor" => "#80BC44", "bgcolor" => "#80BC44");
				$img_indicador = "indicador-verde.png";
			}

			// Retira casas decimais caso seja igual a zero
			$dados['sehqtde'] = explode('.',$dados['sehqtde']);
			$dados['sehqtde'] = $dados['sehqtde'][1]>0 ? $dados['sehqtde'][0].'.'.$dados['sehqtde'][1] : $dados['sehqtde'][0];
			
			// Monta html da bandeira
			$htmlBandeira = $texto_prazo ? $texto_prazo : "";
			if($dados['sehid'] && $dados['aedid'] != WK_MON_EST_AEDID_NAO_EXECUTAR){
				if($dados['unmid'] == UNIDADEMEDICAO_BOLEANA){
					$htmlBandeira .= '<span style="color:'.($cor_fonte ? $cor_fonte : "#FFFFFF").'">em '.$dados['data_execucao_br'].'</span>';
				}else{
					$dados['sehqtde'] = $dados['sehqtde'] ? str_replace('.', ',', $dados['sehqtde']) : '';
					$htmlBandeira .= '<span style="font-size:14px" >
										'.($dados['sehqtde'] ? $dados['sehqtde']  : "").'</span><br />
									 ('.(str_replace(".",",",$porcentagem)).'%) <br />
									 <span style="color:'.($cor_fonte ? $cor_fonte : "#FFFFFF").'" > 
										em '.($dados['data_execucao_br']).'</span>';
				}
			}
			
			// Retira casas decimais caso seja igual a zero
			$dados['dmiqtde'] = explode('.',$dados['dmiqtde']);
			$dados['dmiqtde'] = $dados['dmiqtde'][1]>0 ? $dados['dmiqtde'][0].'.'.$dados['dmiqtde'][1] : $dados['dmiqtde'][0];
			
			echo '<tr>
						  <td>'.$dados['indid'].' '.$dados['indnome'].'</td>
						  <td style="border-right:solid 1px black">'.$dados['mtidsc'].'</td>
						  <td bgcolor="'.($cor_prazo ? $cor_prazo : "").'" style="border:solid 1px black;color:'.($cor_fonte_geral ? $cor_fonte_geral : "#FFFFFF").';font-weight:bold;text-align:right;">
						  '.$htmlBandeira.'
						  </td>
						  <td align="right"><span style="color:blue;">'.$dados['dmiqtde'].'</span><br/>em '.$dados['dpedsc'].'</td>
						  <td align="center">';
			
								// Monta html do icone
								$htmlIco = '';
								$medidor = '';
								if( $dados['sehid'] && $dados['unmid'] != UNIDADEMEDICAO_BOLEANA && $dados['aedid'] != WK_MON_EST_AEDID_NAO_EXECUTAR){
									include_once APPRAIZ . 'includes/classes/MedidorDesempenho.class.inc';
									$medidor = new MedidorDesempenho($arrMedidor,$porcentagem,$ultimaMeta);
								}else{
									if( $ico != '' ){
										echo '<div style="float:center;margin-left:5px;">
												<img class="img_middle link" title="'.$icoMsg.'" src="../imagens/'.$ico.'" />
											  </div>';
									}
								}
							
			  echo '</td>
					   </tr>';
		}
		
		echo '</tbody>
				</table></td></tr></table>';
	}else{
		echo 'Sem histórico.'; 
	}
}