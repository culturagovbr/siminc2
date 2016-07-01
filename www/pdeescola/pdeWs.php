<?php
// error handler function
function myErrorHandler($errno, $errstr, $errfile, $errline){
    /* Don't execute PHP internal error handler */
    return true;
}
//include_once APPRAIZ . 'includes/nusoap/lib/nusoap.php';

/**
public String valorEstimadoEscola(String anoExercicio, String coEscola)
public boolean isEscolaPaga(String anoExercicio, String coEscola)
public boolean atualizaAnaliseEscola(String anoExercicio, String coEscola)
**/
class PdeWs {	 
	
 	private $entcodent;
 	private $entid; 
 	private $wsdl;
 	private $parametros;
 	private $registrosWebService;
 	private $objRetorno;
 	private $method;  
 	private $config;
	private $link;
	
		 /*
		 * @method: pdeEscolaWs
		 * @author: Pedro Dantas
		 * @date: 06/04/2009
		 * @params: method, $ano, $coEscola ,$coProgramaFNDE, 
		 * 			$tipoConexao ('P' -> Produ��o, 'D' -> Desenvolvimento)
		 * @return: no caso de chamar o m�todo "valorEstimadoEscola" o retorno � uma string
		 * 		    no caso dos outros m�todos o retorno � boleano. 
		 */
	


 		public function pdeEscolaWs( $method, $ano, $coEscola , $coProgramaFNDE, $tipoConexao = 'P') {
 			//dbg($method .'<br>'. $ano .'<br>'. $coEscola .'<br>'. $coProgramaFNDE .'<br>'. $tipoConexao,1);
 			$config = array(
 						"proxy_host"     => "",
                        "proxy_port"     => "",
 			 			"encoding" => "ISO-8859-1",
						"compression" => SOAP_COMPRESSION_ACCEPT | SOAP_COMPRESSION_GZIP,
						"trace" => true,
 						"exceptions" => true
					);
					 	
			// set to the user defined error handler
			$old_error_handler = set_error_handler("myErrorHandler");
					
			// Conex�o de Produ��o
			if($tipoConexao ==  'P') {
				try {
					//$link = new SoapClient('http://www.fnde.gov.br/pddeWebService/services/PddeDelegate?wsdl', $config );
					
					//$link = new SoapClient('http://www.fnde.gov.br/pddewebservice/server.php?wsdl', $config );
					
					$link = new SoapClient('http://www.fnde.gov.br/pddewebservice/server.php?wsdl');
				}
				catch (SoapFault  $e) {}
			}
			
			// Conex�o de Desenvolvimento 
			else {
				try {
					//$link = new SoapClient('http://www.fnde.gov.br/pddeWebService/services/PddeDelegate?wsdl', $config ); 				
					$link = new SoapClient('http://www.fnde.gov.br/pddewebservice/server.php?wsdl', $config );
				} catch (SoapFault  $e) {}
			}
						
			$param = array(
							$method=>array(
	                          	'anoExercicio'=>$ano,
	                        	'coEscola'=>$coEscola,
	                       		'coProgramaFnde'=>$coProgramaFNDE,
							    'senha'=>'simec',
								'tipo'=> 3
			                   )			
			);

				if($link) {
					try {
						$result = $link->__call($method,$param);
						//$result = $link->__soapCall($method,$param);
					} catch (SoapFault  $e) {
					}
					
					if($result)
						return $result;
					else
						return "errowebservice";
					
				} else {
					return "errowebservice";
				}
 		}
 		
 		 /*
		 * @method: maisEducacaoWs
		 * @author: Felipe Carvalho
		 * @date: 25/06/2009
		 * @params: $method, $ano, $coEscola ,$coProgramaFNDE, 
		 * 			$tipoConexao ('P' -> Produ��o, 'D' -> Desenvolvimento),
		 * 			$coDestinacao, $vlCusteio, $vlCapital, $vlTotal
		 */
 		public function maisEducacaoWs( $method, $ano, $coEscola , $coProgramaFNDE, $tipoConexao = 'P', $coDestinacao = null, $vlCusteio = null, $vlCapital = null, $vlTotal = null) {
 			$config = array(
 						"proxy_host"     => "",
                        "proxy_port"     => "",
 			 			"encoding" => "ISO-8859-1",
						"compression" => SOAP_COMPRESSION_ACCEPT | SOAP_COMPRESSION_GZIP,
						"trace" => true
					); 	

			// Conex�o de Produ��o
			if($tipoConexao ==  'P') {
				//$link = new SoapClient('http://www.fnde.gov.br/pddewebservice/server.php?wsdl', $config );
				$link = new SoapClient('http://www.fnde.gov.br/pddewebservice/server.php?wsdl');
			}
			// Conex�o de Desenvolvimento 
			else {
				$link = new SoapClient('http://www.fnde.gov.br/pddeWebService/services/PddeDelegate?wsdl', $config );
			}
			
			$param = array(
						$method=>array(
                          	'anoExercicio'		=>	$ano,
                       		'coProgramaFnde'	=>	$coProgramaFNDE,
                        	'coEscola'			=>	$coEscola,
							'coDestinacao'		=>	$coDestinacao,
							'vlCusteio'			=>	$vlCusteio,
							'vlCapital'			=>	$vlCapital,
							'vlTotal'			=>	$vlTotal
		                   )			
			);
			
			$result = $link->__call($method,$param);
  
			return $result;
 		}
 		
		/*
		 * @method: escolaAcessivelWs
		 * @author: Felipe Carvalho
		 * @date: 17/09/2010
		 * @params: $method, $ano, $coEscola ,$coProgramaFNDE, 
		 * 			$tipoConexao ('P' -> Produ��o, 'D' -> Desenvolvimento)
		 */
 		public function escolaAcessivelWs( $method, $ano, $coEscola , $coProgramaFNDE, $tipoConexao = 'P' )
 		{
 			$config = array(
 						"proxy_host"     => "",
                        "proxy_port"     => "",
 			 			"encoding" => "ISO-8859-1",
						"compression" => SOAP_COMPRESSION_ACCEPT | SOAP_COMPRESSION_GZIP,
						"trace" => true
					); 	

			// Conex�o de Produ��o
			if($tipoConexao ==  'P')
			{
				//$link = new SoapClient('http://www.fnde.gov.br/pddewebservice/server.php?wsdl', $config );
				$link = new SoapClient('http://www.fnde.gov.br/pddewebservice/server.php?wsdl');
			}
			// Conex�o de Desenvolvimento 
			else {
				$link = new SoapClient('http://www.fnde.gov.br/pddewebservice/server.php?wsdl', $config );
			}
			
			$param = array(
						$method=>array(
                          	'p_an_exercicio'		=>	$ano,
                        	'p_co_escola'			=>	$coEscola,
                       		'p_co_programa_fnde'	=>	$coProgramaFNDE
		                   )			
			);
			
			$result = $link->__call($method,$param);
  
			return $result;
 		}
}  
?>