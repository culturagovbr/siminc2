<?php

function executaEscape( $strTexto )
{
	global $db;
	return $db->escape( $strTexto );
}

function executaBusca( $strSql )
{
	global $db;
//	dbg( $strSql );
	$arrLinha = $db->pegaLinha( $strSql );
//	dbg( $arrLinha );
	return $arrLinha;
}

function executaInsercao( $strSql )
{
	global $db;
//	dbg( $strSql , 1);
	$objResultado = $db->executar( $strSql );
	$arrLinha = pg_fetch_assoc($objResultado);
//	dbg( $arrLinha );
	return $arrLinha;
}

function pegaNomeDoProjetoAtual( $intAnoExercicioAtual )
{
	return "PPA " . $intAnoExercicioAtual;	
}

function pegaNomeDaUnidadeAtual( $entid ) {
	switch($_SESSION['monitora_var']['tipo']) {
		case 'unidade':
			$strSql = "SELECT entunicod  || ' - ' || entnome as nome  
					   FROM entidade.entidade 
			   		   WHERE entid = '".$entid."'";
			break;
		case 'unidadegestora':
			$strSql = "SELECT entungcod  || ' - ' || entnome as nome  
					   FROM entidade.entidade 
			   		   WHERE entid = '".$entid."'";
			break;
		default:
			return "Nome não especificado";
	}
	$arrUnidade= executaBusca( $strSql );
	$strNome = $arrUnidade[ 'nome'];
	$strNome = str_replace( array( chr( 13) , chr( 10 ) , "\n" , "	" ) , array( " " , " " , " " , " " ) , $strNome );
	return $strNome;
	
}

function procuraTarefaProjeto( $intAnoExercicioAtual )
{
	global $db;
	$strSql = sprintf( "
			 select * 
			 from pde.atividade 
			 where atiidpai is null 
			 and atistatus = 'A'
			 and atidescricao = %s
		",
			executaEscape( pegaNomeDoProjetoAtual( $intAnoExercicioAtual ) )
	);

	$arrProjeto = executaBusca( $strSql );
	return $arrProjeto;
}

function procuraTarefaUnidade( $entid , $intAnoExercicioAtual ) {
	global $db;
	$arrProjeto = retornaTarefaProjeto( $intAnoExercicioAtual, $entid );

	$strSql = "SELECT * 
			   FROM pde.atividade 
			   WHERE atiidpai = '".$arrProjeto[ 'atiid' ]."' AND entid = '".$entid."' AND atistatus = 'A'";

/*	$strSql = sprintf( "
		 select at.* 
			 from pde.atividade at
         INNER JOIN pde.planointernoatividade p ON p.atiid = at.atiid
		 inner JOIN financeiro.planointerno pl on pl.pliid = p.pliid
		 inner join planejamento.acao a on a.acaid = pl.acaid
		 inner JOIN public.unidade u on u.unicod = a.unicod and u.unitpocod='U'
		 left join financeiro.execucao v on v.plicod=pl.plicod and v.ptres=pl.pliptres
		 where a.acaid = %d
			 and at.atistatus = 'A' 
			 AND at._atiprojeto = 3
			",
			$intIdAcao,
			executaEscape( pegaNomeDaAcaoAtual( $intIdAcao ) )
	);
	*/

	$arrUnidade = executaBusca( $strSql );
	return $arrUnidade;
	
}

function criaTarefaProjeto( $intAnoExercicioAtual, $entid)
{
	global $db;
	
	$arrProjeto = array();
	$arrProjeto[ 'atiidpai' ] = 'null';
	$arrProjeto[ 'atistatus' ] = 'A';
	$arrProjeto[ 'entid' ] = $entid;
	$arrProjeto[ 'atidescricao' ] = pegaNomeDoProjetoAtual( $intAnoExercicioAtual );
	
	$strSql = sprintf( "
			 insert 
			  into
			 pde.atividade
			(	
			  atiidpai ,
			  atistatus ,
			  atidescricao ,
			  entid ,
			  atiordem
			)
			 values
			(	
				%s,
			    %s,
				%s,
				%d
			)
			returning
			 atiid
		",
			$arrProjeto[ 'atiidpai' ],
			executaEscape( $arrProjeto[ 'atistatus' ] ) ,
			executaEscape( $arrProjeto[ 'atidescricao' ] ),
			executaEscape( $arrProjeto[ 'entid' ] ),
			0
		);
		
	$arrLinha = executaInsercao( $strSql );
	$arrProjeto[ 'atiid' ] = $arrLinha[ 'atiid' ];
	
	$strSql = sprintf( "
			 update
				 pde.atividade
			 set
			 	 _atiprojeto = %d
			 where
				 atiid = %d 
			",
			$arrProjeto[ 'atiid' ],
			$arrProjeto[ 'atiid' ]
		);
		
	$arrProjeto[ '_atiprojeto' ] = $arrProjeto[ 'atiid' ];
	
	$arrLinha = executaBusca( $strSql );
	atividade_calcular_dados( $arrProjeto[ 'atiid' ] );
	return $arrProjeto;	
}

function criaTarefaUnidade( $entid , $intAnoExercicioAtual )
{
	global $db;
	
	$arrProjeto = retornaTarefaProjeto( $intAnoExercicioAtual, $entid );
	
	$arrUnidade = array();
	$arrUnidade[ 'atiidpai' ]		= $arrProjeto[ "atiid" ];
	$arrUnidade[ 'atistatus' ]		= 'A';
	$arrUnidade[ 'atidescricao' ]	= pegaNomeDaUnidadeAtual( $entid );
	$arrUnidade[ 'entid' ]			= $entid;
	
	$strSql = "INSERT 
			   INTO pde.atividade 
			   (
			   		atiidpai ,
			  		atistatus ,
			  		atidescricao ,
				    entid,
			  		_atiprojeto ,
			  		atiordem
			   ) 
			   VALUES
			   (
				'".$arrUnidade[ 'atiidpai' ]."',
			    ".executaEscape( 'A' ).",
				".executaEscape( $arrUnidade[ 'atidescricao' ] ).",
				'".$arrUnidade[ 'entid' ]."',
				'".$arrUnidade[ 'atiidpai' ]."',
				( select coalesce( max(atiordem), 0 ) + 1 as novaordem from pde.atividade as ativ2 where ativ2.atiidpai = '".$arrUnidade[ 'atiidpai' ]."' and atistatus = ".executaEscape( 'A' )." )
				)
			returning
				atiid";
		
	$arrLinha = executaInsercao( $strSql );
	$arrUnidade[ 'atiid' ] = $arrLinha[ 'atiid' ];
	
	atividade_calcular_dados( $arrProjeto[ "atiid" ]);
	return $arrUnidade;	
}

function retornaTarefaProjeto( $intAnoExercicioAtual, $entid ) {
	$arrProjeto = procuraTarefaProjeto( $intAnoExercicioAtual );
	if( $arrProjeto == null ) {
		$arrProjeto = criaTarefaProjeto( $intAnoExercicioAtual, $entid );
	}
	return $arrProjeto;
}

function retornaTarefaUnidade( $entid , $intAnoExercicioAtual )
{
	global $db;
	$arrProjeto = retornaTarefaProjeto( $intAnoExercicioAtual, $entid );
	if( procuraTarefaUnidade( $entid , $intAnoExercicioAtual ) == null ) {
		criaTarefaUnidade( $entid , $intAnoExercicioAtual );
	}
	return procuraTarefaUnidade( $entid , $intAnoExercicioAtual );
}

?>