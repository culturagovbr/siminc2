<?
/**
 * Recupera o(s) perfil(is) do usu�rio no m�dulo
 * 
 * @return array $pflcod
 */
function arrayPerfil()
{
	/*** Vari�vel global de conex�o com o bando de dados ***/
	global $db;

	/*** Executa a query para recuperar os perfis no m�dulo ***/
	$sql = "SELECT
				pu.pflcod
			FROM
				seguranca.perfilusuario pu
			INNER JOIN 
				seguranca.perfil p ON p.pflcod = pu.pflcod
								  AND p.sisid = ".SISID_AGENDA."
			WHERE
				pu.usucpf = '".$_SESSION['usucpf']."'
			ORDER BY
				p.pflnivel";
	$pflcod = $db->carregarColuna($sql);
	
	/*** Retorna o array com o(s) perfil(is) ***/
	return (array)$pflcod;
}

function abaEdicaoEvento($tipo=''){
	$menu = array();
	switch ($tipo){
		case 'area':
			$menu[] = array("id" 		 => 1, 
				  			 "descricao" => "Agendamento", 
				  			 "link" 	 => "?modulo=principal/cadastroAgenda&acao=E");
			
			$menu[] = array("id" 		 => 2, 
				  		     "descricao" => "Lista de Eventos",    		 	 
				  		     "link" 	 => "?modulo=principal/listaEvento&acao=A");
			
			$menu[] = array("id" 		 => 3, 
				  			 "descricao" => "Evento", 
				  			 "link" 	 => "?modulo=principal/cadastroEvento&acao=E");

			if ( verificaPerfil( array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR, PERFIL_GESTOR) ) ){			
				$menu[] = array("id" 		 => 4, 
					  		     "descricao" => "Extrato de Eventos",    		 	 
					  		     "link" 	 => "?modulo=relatorio/extratoEvento&acao=A");
			}
			
			$menu[] = array("id" 		 => 5, 
				  		     "descricao" => "Lista de �reas Vinculadas",    		 	 
				  		     "link" 	 => "?modulo=principal/listaVinculoArea&acao=A");
			
			$menu[] = array("id" 		 => 6, 
				  		     "descricao" => "�rea Vinculada",    		 	 
				  		     "link" 	 => "?modulo=principal/vinculoArea&acao=A");
			
			if ( verificaPerfil( array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR, PERFIL_GESTOR) ) ){
				$menu[] = array("id" 		 => 7, 
					  		     "descricao" => "Anexos",    		 	 
					  		     "link" 	 => "?modulo=principal/anexoEvento&acao=A");
				$menu[] = array("id" 		 => 8, 
					  		     "descricao" => "Par�metros da Ag�nda",    		 	 
					  		     "link" 	 => "?modulo=principal/parametrosAgenda&acao=A");
			}
			
			break;
		case 'novo':
			$menu[] = array("id" 		 => 1, 
				  			 "descricao" => "Agendamento", 
				  			 "link" 	 => "?modulo=principal/cadastroAgenda&acao=E");
			
			$menu[] = array("id" 		 => 2, 
				  		     "descricao" => "Lista de Eventos",    		 	 
				  		     "link" 	 => "?modulo=principal/listaEvento&acao=A");
			
			$menu[] = array("id" 		 => 3, 
				  			 "descricao" => "Evento", 
				  			 "link" 	 => "?modulo=principal/cadastroEvento&acao=E");
			
			if ( verificaPerfil( array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR, PERFIL_GESTOR) ) ){
				$menu[] = array("id" 		 => 4, 
					  		     "descricao" => "Extrato de Eventos",    		 	 
					  		     "link" 	 => "?modulo=relatorio/extratoEvento&acao=A");
				$menu[] = array("id" 		 => 5, 
					  		     "descricao" => "Par�metros da Ag�nda",    		 	 
					  		     "link" 	 => "?modulo=principal/parametrosAgenda&acao=A");
			}
			
			break;
		default:
			$menu[] = array("id" 		 => 1, 
				  			 "descricao" => "Agendamento", 
				  			 "link" 	 => "?modulo=principal/cadastroAgenda&acao=E");
			
			$menu[] = array("id" 		 => 2, 
				  		     "descricao" => "Lista de Eventos",    		 	 
				  		     "link" 	 => "?modulo=principal/listaEvento&acao=A");
			
			$menu[] = array("id" 		 => 3, 
				  			 "descricao" => "Evento", 
				  			 "link" 	 => "?modulo=principal/cadastroEvento&acao=E");
			
			if ( verificaPerfil( array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR, PERFIL_GESTOR) ) ){
				$menu[] = array("id" 		 => 4, 
					  		     "descricao" => "Extrato de Eventos",    		 	 
					  		     "link" 	 => "?modulo=relatorio/extratoEvento&acao=A");
			}
			
			$menu[] = array("id" 		 => 5, 
				  		     "descricao" => "Lista de �reas Vinculadas",    		 	 
				  		     "link" 	 => "?modulo=principal/listaVinculoArea&acao=A");
			
			if ( verificaPerfil( array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR, PERFIL_GESTOR) ) ){
				$menu[] = array("id" 		 => 6, 
					  		     "descricao" => "Anexos",    		 	 
					  		     "link" 	 => "?modulo=principal/anexoEvento&acao=A");
				$menu[] = array("id" 		 => 7, 
					  		     "descricao" => "Par�metros da Ag�nda",    		 	 
					  		     "link" 	 => "?modulo=principal/parametrosAgenda&acao=A");
			}	
	}
				  	  
	return $menu;				  
}

function abaEdicaoAgenda($tipo=''){
	$menu = array();
	switch ($tipo){
		default:
			$menu[] = array("id" 		 => 1, 
				  			 "descricao" => "Agendamento", 
				  			 "link" 	 => "?modulo=principal/cadastroAgenda&acao=E");
			
			$menu[] = array("id" 		 => 2, 
				  		     "descricao" => "Lista de Eventos",    		 	 
				  		     "link" 	 => "?modulo=principal/listaEvento&acao=A");
			
			if ( verificaPerfil( array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR, PERFIL_GESTOR) ) ){
				$menu[] = array("id" 		 => 3, 
					  	 	     "descricao" => "Extrato de Eventos",    		 	 
					  		     "link" 	 => "?modulo=relatorio/extratoEvento&acao=A");
				
				$menu[] = array("id" 		 => 4, 
					  	 	     "descricao" => "Par�metros da Ag�nda",    		 	 
					  		     "link" 	 => "?modulo=principal/parametrosAgenda&acao=A");
			}
			
			
	}
				  	  
	return $menu;				  
}

/**
 * WORKFLOW (EVENTO/�REA) - IN�CIO
 */
function criarDocidEventoArea( $evaid ){
	
	global $db;

	require_once APPRAIZ . 'includes/workflow.php';

	// descri��o do documento
	$docdsc = "Fluxo de evento/�rea do m�dulo Agenda - evaid " . $evaid;

	// cria documento do WORKFLOW
	$docid = wf_cadastrarDocumento( FLUXO_AGENDA_TPDID, $docdsc );

	// atualiza o DOCID no evento
	$eventoArea 	   = new EventoArea($evaid);
	$eventoArea->docid = $docid;
	$eventoArea->salvar();
	
	$db->commit();

	return $docid;
}

function pegaDocidEventoArea( $evaid ){
	global $db;

	$eventoArea = new EventoArea($evaid);
	$docid 		= $eventoArea->docid;
	if( !$eventoArea->docid ){
		$docid = criarDocidEventoArea( $evaid );
	}
	
	return $docid;
}

function pegaEstadoEventoArea( $docid ){
	global $db;
	
	$docid = ($docid ? $docid : 0);
	
	$sql = "SELECT
				esdid
			FROM
				workflow.documento d
			WHERE
				docid = {$docid}";
		
	$esdid = $db->pegaUm( $sql );
	
	return $esdid;
}
/**
 * WORKFLOW (EVENTO/�REA) - FIM
 */

?>