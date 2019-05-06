<?php


// ATIVIDADE ///////////////////////////////////////////////////////////////////


function atividade_inserir( $atividade, $titulo, $subacao = false, $boVerificaSubacao = true){
	global $db;
	
	extract($_POST);
	ver($_POST,d);
	//ver($boVerificaSubacao,$atividade, $titulo, $subacao,$_POST,d);
	if($boVerificaSubacao){
		if($sbaid) {
			$existe_sbaid = $db->pegaUm("SELECT sbaid FROM monitora.pi_subacao WHERE sbaid != '".$sbaid."' AND sbacod = '".$sbacod."' AND sbastatus = 'A'");
		} else {
			$existe_sbaid = $db->pegaUm("SELECT sbaid FROM monitora.pi_subacao WHERE sbacod = '".$sbacod."' AND sbastatus = 'A'");
		}
	
		if($existe_sbaid) {
			die("<script>alert('Código da subação encontra-se cadastrado.');window.close()</script>");
		}		
	}
	
	$sql = sprintf(
		"insert into pde.atividade (
			atiidpai, atidescricao, unicod, atiordem, _atiprojeto, acaid
		) values (
			%d,
			'%s',
			'%s',
			( select coalesce( max(atiordem), 0 ) + 1 from pde.atividade where atistatus = 'A' and atiidpai = %d ),
			( select _atiprojeto from pde.atividade where atiid = %d ),
			( select acaid from pde.atividade where atiid = %d )
		) returning atiid",
		$atividade,
		$titulo,
		$_SESSION['monitora_var']['unicod'],
		$atividade,
		$atividade,
		$atividade # adaptação necessária para que o módulo de monitoramento funcione
	);
	
	$atiid = $db->pegaUm($sql);
	
	// verifica se a atividade pai possui subacao, caso sim, a filha devera ser a mesma subacao
	$sql = "SELECT sbaid FROM monitora.pi_subacaoatividade WHERE atiid='".$atividade."'";
	$sbaid = $db->pegaUm($sql);
	
	if($sbaid) {
		$sql = "INSERT INTO monitora.pi_subacaoatividade(sbaid, atiid) VALUES ('".$sbaid."', '".$atiid."');";
		$db->executar($sql);
		$db->commit();
	}
		
	// verifica se a atividade é do tipo subacao
	if($subacao['subatv']) {
		if(!$subacao['sbaid']){
			$sql = "INSERT INTO monitora.pi_subacao(sbatitulo, sbacod, sbadsc, sbastatus, usucpf, sbadata, sbasituacao, sbaobras, sbaano)
	    			VALUES ('".$subacao['sbatitulo']."', 
	    					'".$subacao['sbacod']."', 
	    					'".$subacao['sbadsc']."', 
	    					'A', 
	    					'".$_SESSION['usucpf']."', 
	    					NOW(),
	    					'P',
	    					".(($subacao['sbaobras'])?"TRUE":"FALSE").",
                               '".$_SESSION['exercicio']."') RETURNING sbaid;";
                        
                        
			
			$sbaid = $db->pegaUm($sql);			
		} else {
			$sbaid = $subacao['sbaid'];
		}
		
		$sql = "INSERT INTO monitora.pi_subacaoatividade(sbaid, atiid, sbaatividade)
    			VALUES ('".$sbaid."', '".$atiid."', true);";
		$db->executar($sql);
		
		if(!$subacao['sbaid']){
			$sql = "INSERT INTO monitora.pi_subacaohistorico(sbaid, sahobs, sahdata, usucpf, sahsituacao)
	    			VALUES ('".$sbaid."', NULL, NOW(), '".$_SESSION['usucpf']."', 'P');";
			$db->executar($sql);
		}
		
		$sql = "INSERT INTO monitora.pi_subacaounidade(sbaid, unicod, unitpocod) VALUES ('".$sbaid."', '".$_SESSION['monitora_var']['unicod']."', 'U');";
		$db->executar($sql);
		
		$sql = "UPDATE pde.atividade SET atidescricao='".$subacao['sbatitulo']." - (".$subacao['sbacod'].")' WHERE atiid='".$atiid."'";
		$db->executar($sql);
		
		$db->commit();
		
	}
	
	if($boVerificaSubacao){
		return true;
	} else {
		return $sbaid;
	}
	
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
	if ( $projeto != PROJETO ) {
		$atividade = (integer) PROJETO;
		$projeto   = (integer) PROJETO;
	}
	
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
			a.atiporcentoexec,
			a._atiprojeto,
			--a._atiordem,
			a._atinumero,
			a._atiprofundidade,
			a._atiirmaos,
			a._atifilhos,
			ea.esadescricao,
			u.usunome,
			u.usunomeguerra,
			--u.usucpf,
			u.usuemail,
			u.usufoneddd,
			u.usufonenum,
			uni.unidsc,
			ug.ungdsc,
			aga.graid,
			coalesce( restricoes, 0 ) as qtdrestricoes,
			coalesce( anexos, 0 ) as qtdanexos,
			(SELECT COUNT(*) FROM monitora.pi_planointerno pi LEFT JOIN monitora.pi_planointernoatividade pia ON pia.pliid = pi.pliid WHERE pia.atiid = a.atiid) AS qtdpi,
			a._atiprofundidade as profundidade,
			a._atinumero as numero,
			a._atifilhos as filhos
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
	
	
	$sql = "select a.sbaid from monitora.pi_subacaoatividade a inner join monitora.pi_subacao b ON b.sbaid = a.sbaid where b.sbaplanotrabalho = 't' and a.atiid = $atiid";
	$sbaid = $db->pegaUm($sql);
	
	if($sbaid){
		$sql = "update monitora.pi_subacao set sbaplanotrabalho = 'f' where sbaid = $sbaid";
		$db->executar($sql);		
	}
	
	// exclui a subacaoatividade
	$sql = sprintf( "DELETE FROM monitora.pi_subacaoatividade where atiid = %s", $atividade['atiid'] );
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
	$sql = sprintf("SELECT a.*, e.esadescricao, sub.numero, sub.projeto
					FROM pde.atividade a
					LEFT JOIN pde.estadoatividade e ON e.esaid = a.esaid
					INNER JOIN pde.f_dadosatividade( %d ) as sub ON sub.atiid = a.atiid
					WHERE a.atiid = %d AND atistatus = 'A'",
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
	}
	return true;
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
	/*
	 * GATILHOOOOO!!!! Solicitado por Henrique Xavier (20/01/09)
	 */
	if($db->testa_superuser() || $usuario['plfcod'] == 18 || $usuario['plfcod'] == 4) {
		return true;
	}
	$usuario = $usuario ? $usuario : $_SESSION['usucpf'];
	if ( $_SESSION["sisid"] == 1 ) {
		return acao_verificar_responsabilidade( $projeto, $usuario );
	}
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
		inner join planejamento.acao aca on aca.acaid = ati.acaid
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

function verificaPerfilPlanoTrabalho() {
	global $db;
	
	$sql = "SELECT p.pflcod FROM seguranca.perfil p 
			LEFT JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod 
			WHERE pu.usucpf = '". $_SESSION['usucpf'] ."' and p.pflstatus = 'A' and p.sisid =  '". SISTEMA_PPA ."' ORDER BY p.pflcod";
	//$perfilid = $db->carregar($sql);
	$perfilid = (array) $db->carregarColuna($sql,'pflcod');
	if($db->testa_superuser() ||
	   in_array(PERFIL_COORDEUNIDMONITORA,$perfilid) ||
	   in_array(PERFIL_UNIDMONITORAAVALIA,$perfilid) ) {
		// permissao para remover e gravar
		$permissoes['remover'] = true;
		$permissoes['gravar'] = true;
	} else {
			/*
			switch($perfilid) {
				case PERFIL_COORDACAO:
				case PERFIL_EQCOOACAO:
					$sql = "SELECT ac.unicod FROM monitora.usuarioresponsabilidade ur
							LEFT JOIN planejamento.acao ac ON ac.acaid = ur.acaid
							WHERE ur.usucpf = '".$_SESSION['usucpf']."' AND ur.pflcod = '".$perfilid."' AND ur.prsano='".$_SESSION['exercicio']."'";
					$unicods = (array) $db->carregar($sql);
					if($unicods[0]) {
						foreach($unicods as $unicod) {
							$permissoes['verunidades'][] = $unicod['unicod'];
						}
					} else {
						$permissoes['naoverunidades'] = true;
					}
					$permissoes['remover'] = true;
					$permissoes['gravar'] = true;
					break;
				case PERFIL_GESTORUNIDPLANEJAM:
				case PERFIL_EQUIPAPOIOGESTORUP:
					$sql = "SELECT ur.unicod FROM monitora.usuarioresponsabilidade ur
							WHERE ur.usucpf = '".$_SESSION['usucpf']."' AND ur.pflcod = '".$perfilid."' AND ur.prsano='".$_SESSION['exercicio']."'";
					$unicods = (array) $db->carregar($sql);
					if($unicods[0]) {
						foreach($unicods as $unicod) {
							$permissoes['verunidades'][] = $unicod['unicod'];
						}
					} else {
						$permissoes['naoverunidades'] = true;
					}
					$permissoes['remover'] = true;
					$permissoes['gravar'] = true;
					break;
					
			}
			*/

	//		if( in_array(PERFIL_EQCOOACAO,$perfilid) || in_array(PERFIL_EQUIPAPOIOGESTORUP,$perfilid) ){
				
					// Pega unidades por acao				
					$sql = "SELECT 
								distinct a.unicod 
							FROM 
								monitora.usuarioresponsabilidade ur
								inner join planejamento.acao a on a.acaid=ur.acaid 
							WHERE 
								ur.rpustatus='A' and 
								ur.usucpf = '".$_SESSION['usucpf']."' AND 
								ur.pflcod in (".implode(',', $perfilid).") AND 
								ur.prsano='".$_SESSION['exercicio']."'";
					$unicods = (array) $db->carregar($sql);

					if($unicods[0]) {
						foreach($unicods as $unicod) {
							$permissoes['verunidades'][] = $unicod['unicod'];
						}
					}/* else {
						$permissoes['naoverunidades'] = true;
					}*/

					// Pega unidades por unidade orçamentaria
					$sql = "SELECT 
								distinct ur.unicod 
							FROM 
								monitora.usuarioresponsabilidade ur
							WHERE 
								ur.rpustatus='A' and 
								ur.unicod IS NOT NULL AND
								ur.unicod != '' AND
								ur.usucpf = '".$_SESSION['usucpf']."' AND 
								ur.pflcod in (".implode(',', $perfilid).") AND 
								ur.prsano='".$_SESSION['exercicio']."'";
					$unicods = (array) $db->carregar($sql);

					if($unicods[0]) {
						foreach($unicods as $unicod) {
							$permissoes['verunidades'][] = $unicod['unicod'];
						}
					}					

					// Pega unidades gestoras
					$sql = "SELECT 
								distinct ur.ungcod 
							FROM 
								monitora.usuarioresponsabilidade ur
							WHERE 
								ur.rpustatus='A' and 
								ur.ungcod IS NOT NULL AND
								ur.ungcod != '' AND
								ur.usucpf = '".$_SESSION['usucpf']."' AND 
								ur.pflcod in (".implode(',', $perfilid).") AND 
								ur.prsano='".$_SESSION['exercicio']."'";
					$unicods = (array) $db->carregar($sql);

					if($unicods[0]) {
						$permissoes['verunidades'][] = '26101';
						foreach($unicods as $unicod) {
							$permissoes['verunidadesgestoras'][] = $unicod['ungcod'];
						}
					}
					
					if (!is_array($permissoes['verunidades']) && !is_array($permissoes['verunidadesgestoras']) ){
						$permissoes['naoverunidades'] = true;
					}
					
					$permissoes['remover'] = true;
					$permissoes['gravar'] = true;
//			}
	}
	return $permissoes;
}

function validaAcessoUnidade($permissoes, $unicod) {
	if($permissoes) { // verifica se existe restriçoes de acesso a unidade
		$permissoes = array_flip($permissoes);
		if(!isset($permissoes[$unicod])) {
			echo "<script>
					alert('Você não possui acesso a UNIDADE.');
					window.location = '?modulo=principal/planotrabalho/plano&acao=A';
			 	  </script>";
			exit;
		}
	}
}




//***************ENVIA EMAIL PI******************************
function enviaEmailStatusPi($pi){
		global $db;
		global $servidor_bd;
		
        $emailCopia[] = $_SESSION['email_sistema'];
		
		# Recupera email cadastrador do PI
		$arCadastradorPI = $db->pegaLinha("SELECT pi.unicod, u.usuemail FROM seguranca.usuario u inner join monitora.usuarioresponsabilidade ur on u.usucpf = ur.usucpf and ur.rpustatus = 'A' inner join monitora.pi_planointerno pi on u.usucpf = pi.usucpf WHERE pi.pliid = $pi");
		if($_SERVER["HTTP_HOST"] == 'simec' || $_SERVER["HTTP_HOST"] == 'simec.mec.gov.br') {
			if($arCadastradorPI['usuemail']){
				$emailCopia[] = "{$arCadastradorPI['usuemail']}";
			}			
		}
		
		# Recupera email gestor unidade obrigatoria
		$arEmailGestorUnidadeObrig = $db->carregar("SELECT u.usuemail FROM monitora.usuarioresponsabilidade ur inner join seguranca.usuario u on ur.usucpf = u.usucpf where ur.rpustatus = 'A' and ur.pflcod = '".PERFIL_MONITORA_GESTORUNIDORCAMENTO."' and ur.ungcod = '{$arCadastradorPI['unicod']}' ");
		$arEmailGestorUnidadeObrig = ($arEmailGestorUnidadeObrig) ? $arEmailGestorUnidadeObrig : array();
		if($_SERVER["HTTP_HOST"] == 'simec' || $_SERVER["HTTP_HOST"] == 'simec.mec.gov.br') {
			foreach($arEmailGestorUnidadeObrig as $emailGestorUnidadeObrig){
				if($emailGestorUnidadeObrig['usuemail']){
					$emailCopia[] = "{$emailGestorUnidadeObrig['usuemail']}";
				}
			}
		}
		
		$emailCopia = array_unique($emailCopia);
		
		// Seta remetente
		$remetente = array('nome'=>'SPO - Plano interno', 'email'=>$_SESSION['email_sistema']);
	
		$sql = "SELECT distinct
					pi.plicod  as plicod,
					pi.plititulo as plititulo,
					e.unidsc as unidade,
					CASE WHEN pi.plisituacao = 'P' THEN ' Pendente '
						WHEN pi.plisituacao = 'A' THEN ' Aprovado '
						WHEN pi.plisituacao = 'C' THEN ' Cadastrado no SIAFI '
						WHEN pi.plisituacao = 'R' THEN ' Revisado '
						WHEN pi.plisituacao = 'H' THEN ' Homologado '
						WHEN pi.plisituacao = 'E' THEN ' Enviado para Revisão '
						WHEN pi.plisituacao = 'S' THEN ' Confirmado no SIAFI '
					END as situacao,
					to_char(pi.plidata, 'DD/MM/YYYY') ||' '|| to_char(pi.plidata, 'HH24') ||':'|| to_char(pi.plidata, 'MI') as plidata,
					u.usuemail as usuemail
				FROM
					monitora.pi_planointerno pi
					LEFT JOIN monitora.pi_subacao sa ON sa.sbaid = pi.sbaid
					LEFT JOIN monitora.pi_subacaounidade su ON su.sbaid = sa.sbaid
					LEFT JOIN monitora.pi_obra o ON o.pliid = pi.pliid
					LEFT JOIN obras.obrainfraestrutura obr ON obr.obrid = o.obrid
					LEFT JOIN public.unidade e ON e.unicod = pi.unicod
					LEFT JOIN seguranca.usuario u ON u.usucpf = pi.usucpf
				WHERE
					pi.plistatus = 'A' 
					AND pi.pliid = '" . $pi . "'
					and pi.usucpf not in ('40074641115', '18431445149', '30420202668', '26873559172')
				ORDER BY
					plidata";
		$dado = (array) $db->pegaLinha($sql);
		
		// seta dados da demanda
		$dadoPi = array(
							 "Código do PI"			   => $dado['plicod'],	
							 "Título"			 	   => $dado['plititulo'],
							 "Unidade" 			 	   => $dado['unidade'],
							 "Data Inclusão" 		   => $dado['plidata'],
							 "Situação" 			   => $dado['situacao'] 	 
							);
							
							
	    // Busca historicos
	    $sql = "SELECT 
					CASE WHEN pih.pihsituacao = 'P' THEN ' Pendente '
						WHEN pih.pihsituacao = 'A' THEN ' Aprovado '
						WHEN pih.pihsituacao = 'C' THEN ' Cadastrado no SIAFI '
						WHEN pih.pihsituacao = 'H' THEN ' Homologado '
						WHEN pih.pihsituacao = 'R' THEN ' Revisado '
						WHEN pih.pihsituacao = 'E' THEN ' Enviado para Revisão '
						WHEN pih.pihsituacao = 'S' THEN ' Confirmado no SIAFI '
					END as situacao,
						pih.pihobs,
						to_char(pih.pihdata, 'DD/MM/YYYY  hh24:mi:ss') as pihdata,
						u.usunome 
					FROM 
						monitora.pi_planointernohistorico pih
						INNER JOIN seguranca.usuario u on pih.usucpf = u.usucpf 
					WHERE	
						pih.pliid = '" . $pi . "'
					ORDER BY
						pih.pihdata desc";
	    $dadoObs = $db->carregar($sql);
	    
	    // Se for produção, envia email para os usuários.
	    if($_SERVER["HTTP_HOST"] == 'simec' || $_SERVER["HTTP_HOST"] == 'simec.mec.gov.br') {
			$emailUsuPi	 = $dado['usuemail'];
	    }
		// Seta Assunto
		$assunto = "Plano Interno -  Histórico / Observações.";
		
		// Seta Conteúdo
		$conteudo = textMail("Acompanhamento do Plano Interno", $dadoPi, $dadoObs);
		//echo "$remetente | $emailUsuPi | $assunto | $conteudo | $emailCopia";
		//exit;
		enviar_email( $remetente, $emailUsuPi, $assunto, $conteudo, $emailCopia );
		
}		
//***************FIM ENVIA EMAIL**************************	


/**************
  Função que monta texto do email, no formato HTML.
***************/
function textMail($msg=null, $dadoPi = array("" => ""), $dadoObs = array("" => "")){
	
	$cabecalho = array("Situação", "Observação","Data de Inclusão");
	
	$text = '<html xmlns:v="urn:schemas-microsoft-com:vml" xmlns:o="urn:schemas-microsoft-com:office:office" xmlns:w="urn:schemas-microsoft-com:office:word" xmlns:m="http://schemas.microsoft.com/office/2004/12/omml" xmlns="http://www.w3.org/TR/REC-html40">
			<head>
			<style>
			.table_mail{
			    width: 80%;
			    border:outset #0099CC 2px;
			}
			
			.tit_td{
				background:#6699CC; 
				font-size:10.0pt;
				font-family:"Arial","sans-serif";
				color:white;
				font-weight: bold;
				text-align: center;
				border-bottom: 1px solid white;
				margin:2px;
			}
			
			.item_1{			
				font-size:9.0pt;
				text-align: right;
				font-weight: bold;
				font-family:"Arial","sans-serif";
				padding: 3px;
				padding-right: 5px;
				border-right: 1px solid white;
			}
			
			.item_2{
				font-size:9.0pt;			
				text-align: left;
				font-family:"Arial","sans-serif";
				padding-left: 3px;
				border-right: 1px solid white;	
			}
			
			</style>
			</head>
			<body lang=PT-BR link=blue vlink=purple>
			    <table cellpadding="1" cellspacing="0" class="table_mail">
			    	<tr>
			    		<td class="tit_td" colspan="4">'.$msg.'</td>
			    	</tr>
			      	<tr>
			    		<td class="tit_td" colspan="4">Dados do PI</td>
			    	</tr>';
	$a = 0;
	$dadoPi = ($dadoPi) ? $dadoPi : array();
    foreach($dadoPi as $ind=>$val){  
    	$text .= '<tr bgcolor="'.(is_int($a/2) ? '#EFEFEF' : '#DFDFDF').'">
		    		<td class="item_1" colspan="2" nowrap width="40%">'.$ind.':</td>
		    		<td class="item_2" colspan="2">'.$val.'</td>
		    	  </tr>'; 
    	$a++; 	
    }

    
    $text .= ' 	<tr>
		    		<td class="tit_td" colspan="4">Histórico / Observações</td>
		    	</tr>';

    $text .= '<tr bgcolor="#EFEFEF">
		    		<td class="item_2" colspan="4">
		    			<table width="100%" border="0" cellpadding="0" cellspacing="0">    	
					    	<tr>
					    		<td class="tit_td" >Situação</td>
					    		<td class="tit_td" colspan="2" width="50%">Observação</td>
					    		<td class="tit_td" >Data / Hora</td>
					    		<td class="tit_td" >Cadastrado Por</td>
					    	</tr>';
    
						$a = 0;
						$dadoObs = ($dadoObs) ? $dadoObs : array();
					    foreach($dadoObs as $val){  
					    	$text .= '<tr bgcolor="'.(is_int($a/2) ? '#EFEFEF' : '#DFDFDF').'">
							    		<td class="item_2" ><center>'.$val["situacao"].'</center></td>
							    		<td class="item_2" colspan="2">'.$val["pihobs"].'</td>
							    		<td class="item_2" ><center>'.$val["pihdata"].'</center></td>
							    		<td class="item_2" ><center>'.$val["usunome"].'</center></td>
							    	  </tr>'; 
					    	$a++; 	
					    }	   	
		    		
	$text .= '			</table>
					</td>
		    	  </tr>	
		    </table>
		<font color=red size=\'3\' face=\'Tahoma\'><b>* Favor não responder este e-mail, realize suas alterações diretamente no SIMEC.</b></font>
		</body>
		</html>';
	return $text;
}


/* Função para recuperar o nome e a cor da situação, passada por parâmetro (código) */

function recuperaCorSituacao($situacao) {
	$arSituacao = array();
	
	switch($situacao) {
		case 'P':
			$arSituacao["nome"] = 'Pendente';
			$arSituacao["cor"] = 'red';
			break;
		case 'C':
			$arSituacao["nome"] = 'Aprovado';
			$arSituacao["cor"] = 'green';
			break;
		case 'H':
			$arSituacao["nome"] = 'Homologado';
			$arSituacao["cor"] = 'blue';
			break;
		case 'V':
			$arSituacao["nome"] = 'Revisado';
			$arSituacao["cor"] = '#3F85FF';
			break;
		case 'S':
			$arSituacao["nome"] = 'Cadastrado no SIAFI';
			$arSituacao["cor"] = '#AF7817';
			break;
		case 'R':
			$arSituacao["nome"] = 'Enviado para Revisão';
			$arSituacao["cor"] = '#EAC117';
			break;
	}
	
	return $arSituacao;
}

/**
 * @return boolean
 */
function boPerfilSomenteLeitura(){
	global $db;
	if ( $db->testa_superuser() ) {
		//return true;
		return false;
	}
	$sql = "select count(1) from seguranca.perfilusuario where usucpf = '{$_SESSION['usucpf']}' and pflcod in ('".PERFIL_MONITORA_GESTORUNIDORCAMENTO."','".PERFIL_MONITORA_EQAGESTORUNIDORCAMENTO."')";
	return (boolean) $db->pegaUm( $sql );
}

function boPerfilSubacao(){
	global $db;
	if ( $db->testa_superuser() ) {
		//return true;
		return false;
	}
	$sql = "select count(1) from seguranca.perfilusuario where usucpf = '{$_SESSION['usucpf']}' and pflcod in ('".PERFIL_GESTORUNIDPLANEJAM."','".PERFIL_EQUIPAPOIOGESTORUP."')";
	return (boolean) $db->pegaUm( $sql );
}

function boNaoVePlanoInterno(){
	global $db;
	if ( $db->testa_superuser() ) {
		//return true;
		return false;
	}
	$sql = "select count(1) from seguranca.perfilusuario where usucpf = '{$_SESSION['usucpf']}' and pflcod in ('".PERFIL_MONITORA_GESTORUNIDORCAMENTO."','".PERFIL_MONITORA_EQAGESTORUNIDORCAMENTO."')";

	if ((boolean)$db->pegaUm( $sql )) { 
		return false;
		die();
	}
	$sql = "select count(1) from seguranca.perfilusuario where usucpf = '{$_SESSION['usucpf']}' and pflcod in ('".PERFIL_GESTORUNIDPLANEJAM."','".PERFIL_EQUIPAPOIOGESTORUP."')";
	return (boolean) $db->pegaUm( $sql );
}

function perfil_unidade($desc = false){
	global $db;
	
	$var = $inner = "";
	if($desc){
		$var = " || ' - ' || u.unidsc as descricao";
		$inner = "INNER JOIN public.unidade u ON ur.unicod = u.unicod";
	}
	
	return $db->carregar("SELECT ur.unicod $var FROM monitora.usuarioresponsabilidade ur $inner where ur.pflcod in ('".PERFIL_GESTORUNIDPLANEJAM."','".PERFIL_EQUIPAPOIOGESTORUP."','".PERFIL_MONITORA_GESTORUNIDORCAMENTO."','".PERFIL_MONITORA_EQAGESTORUNIDORCAMENTO."') and ur.usucpf = '".$_SESSION['usucpf']."' AND ur.unicod is not null ");
}
function carrega_unidade_titulo(){
	global $db;
	
	$arUnicodTemp = perfil_unidade(true);
	$arUnicodTemp = ($arUnicodTemp) ? $arUnicodTemp : array();
	if($arUnicodTemp[0]){
		$arUnicod = array();
		foreach($arUnicodTemp as $uniid){
			if($uniid['descricao']){
				array_push($arUnicod,$uniid['descricao']);								
			}
		}
		$unidade = implode(", ", $arUnicod);
	}
	
	return $unidade;
}
function boUnidadesObrigatorias(){
	global $db;
	
	if(!$db->testa_superuser()){
		if(possui_perfil(array(PERFIL_GESTORUNIDPLANEJAM,PERFIL_EQUIPAPOIOGESTORUP))){
			if($db->pegaUm("SELECT count(1) as count FROM monitora.usuarioresponsabilidade ur where ur.unicod in ('".AD."','".CAPES."','".INEP."','".FNDE."','".FIES."') and ur.usucpf = '".$_SESSION['usucpf']."' AND ur.unicod is not null ")){
				die("<script>alert('Favor acessar o Menu Principal -> Plano Trabalho - Obrigatórias!'); history.back(-1);</script>");
			}
		}
	}
	
}

function possui_perfil( $pflcods ){

	global $db;

	if ( is_array( $pflcods ) )
	{
		$pflcods = array_map( "intval", $pflcods );
		$pflcods = array_unique( $pflcods );
	}
	else
	{
		$pflcods = array( (integer) $pflcods );
	}
	if ( count( $pflcods ) == 0 )
	{
		return false;
	}
	$sql = "
		select
			count(*)
		from seguranca.perfilusuario
		where
			usucpf = '" . $_SESSION['usucpf'] . "' and
			pflcod in ( " . implode( ",", $pflcods ) . " ) ";
	return $db->pegaUm( $sql ) > 0;
}

function arConfiguracoesPerfis(&$arPerfilUnidadeOrcamento = array(), &$arPerfilUnidadePlanejamento = array()){

	$arPerfilUnidadeOrcamento = array( PERFIL_MONITORA_GESTORUNIDORCAMENTO
						   		  	  ,PERFIL_MONITORA_EQAGESTORUNIDORCAMENTO
						   		 	  );
						   		 
	$arPerfilUnidadePlanejamento = array( PERFIL_GESTORUNIDPLANEJAM
						   		  	 	 ,PERFIL_EQUIPAPOIOGESTORUP
						   		 		);
}

?>