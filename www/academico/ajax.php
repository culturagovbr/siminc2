<?php
// carrega as fun��es gerais
include_once "config.inc";
//include "verificasistema.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
// carrega as fun��es espec�ficas do m�dulo
include_once '_constantes.php';


// Verifica a possibilidade de excluir um cargo
if( $_REQUEST["ajax_excluircargo"]){
	$db = new cls_banco();
	
	$lepid = (integer) $_REQUEST["lepid"];
	$tpeid = (integer) $_REQUEST["tpeid"];
	
	//caso n�o exista lan�amento autoriza a exclus�o
	if( ! $lepid ) die();
	
	$sql = "SELECT edpid, crgid, lepano
					FROM academico.lancamentoeditalportaria AS lp 
					WHERE lp.lepid = $lepid 					
					";
	
	$dados = $db->pegaLinha($sql);	
	
	switch ($tpeid) {
		case ACA_TPEDITAL_PUBLICACAO:						
		
			$sql_edpid_homo = "SELECT edpid
										FROM academico.editalportaria AS ep
										WHERE ep.edpidhomo = ".$dados['edpid']."
										AND edpstatus = 'A'
										";	
			$edpid_homo = $db->carregarColuna($sql_edpid_homo);
			$edpids_homo = implode(',', $edpid_homo); 
						
			if( ! $edpids_homo ) die();
			else{
				$sql_pub = "SELECT lp.lepid
							FROM academico.lancamentoeditalportaria AS lp
							WHERE 
							lp.edpid in (".$edpids_homo.") AND
							lp.crgid = ".$dados['crgid']." AND
							lp.lepano = '".$dados['lepano']."'
						";
				
				$result = $db->pegaUm($sql_pub);
				if($result){
					die('N�o foi poss�vel excluir o cargo. Exclua o lan�amento de homologa��o deste cargo.');
				}else die();
			}	
			
		;
		break;
		
		case ACA_TPEDITAL_HOMOLOGACAO:		
			
	
				$sql_edpid_efe = "SELECT edpid
									FROM academico.editalportaria AS ep
									WHERE ep.edpideditalhomologacao = ".$dados['edpid']."
									AND edpstatus = 'A'
									";
				
				$edpid_efe = $db->carregarColuna($sql_edpid_efe);
				
				if( ! $edpid_efe ) die();
				else{
					
					$edpids = implode(',', $edpid_efe); 
					
					$sql_pub = "SELECT lp.lepid
								FROM academico.lancamentoeditalportaria AS lp
								WHERE 
								lp.edpid in (".$edpids.") AND
								lp.crgid = ".$dados['crgid']." AND
								lp.lepano = '".$dados['lepano']."'
								";
					$result = $db->pegaLinha($sql_pub);
					if($result){
						die('N�o foi poss�vel excluir o cargo. Exclua o lan�amento de efetiva��o deste cargo.');
					}else die();
				}
		break;
	}
}

?>