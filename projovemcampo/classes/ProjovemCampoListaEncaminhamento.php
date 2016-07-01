<?php
class ProjovemCampoListaEncaminhamento {

	private $perfis;
	private $tituloPagina;
	private $formulario;
	private $registros;
	private $mensagem = array('diario'=>
								array( 	'pendencia' => 'Pend�ncia de fechamento',
										'reabrir' 	=> 'Reabrir Turma(s)',
										'historico' => 'Hist�rico Tramita��o'
								)
						);
	//Monta Workflow
// 	public function montaWorkflow(){
		
// 	}
	// 
	public function inicioPerfilEquipeMec( $var )
	{
		if( $this->arrayEstaVazio( $var ) ){
			throw new Exception( 'Erro ao carregar Perfil MEC.' );
		}
	}
	
	public function inicioPerfilCoordenador( $var )
	{
		if( $this->arrayEstaVazio( $var ) ){
			throw new Exception( 'Erro ao carregar Coordenador.' );
		}
	}
	
	public function inicioPerfilCoordenadorTurma( $var )
	{
		if( $this->arrayEstaVazio( $var ) ){
			throw new Exception( 'Erro ao carregar Coordenador de Turma.' );
		}
	}
	
	public function inicioPerfilDiretorEscola( $var )
	{
		if( $this->arrayEstaVazio( $var ) ){
			throw new Exception( 'Erro ao carregar Diretor de N�cleo.' );
		}
	}
	
	/* Recebe os registros do banco de dados */
	public function setRegistros( $var )
	{
		if( $this->arrayEstaVazio( $var ) ){
			throw new Exception( 'Nenhum registro encontrado.' );
		}else{
			$this->registros = $var;
		}
	}
	
	public function getRegistros()
	{
		return $this->registros;
	}


	/* Vari�veis de Formul�rio */
	public function setFormulario( $var )
	{
		if( !empty($this->formulario) ){
			array_push( $var, $this->formulario );
		}else{
			$this->formulario = $var;
		}
	}
	
	public function getFormulario()
	{
		if( empty( $this->formulario ) ){
			throw new Exception( 'Erro ao recuperar dados do formul�rio.' );
		}else{
			return $this->formulario;
		}
	}	
	
	/* T�tulo P�gina */
	public function getTituloPagina()
	{
		if( empty( $this->tituloPagina ) ){
			throw new Exception( 'Erro ao recuperar t�tulo da p�gina.' );
		}else{
			return $this->tituloPagina;
		}
	}
	
	public function setTituloPagina( $var )
	{
		if( empty( $var) ){
			throw new Exception( 'Erro ao definir t�tulo da p�gina.' );
		}else{
			$this->tituloPagina = $var;
		}
	}

	/* Perfil */
	public function setPerfis( $var )
	{
		if( empty( $var) ){
			throw new Exception( 'Erro ao carregar perfis.' );
		}else{
			$this->perfis = $var;
		}
	}
	
	public function getPerfis( )
	{
		if( empty( $this->perfis ) ){
			throw new Exception( 'Erro ao recuperar t�tulo da p�gina.' );
		}else{
			return $this->perfis;
		}
	}

	/* Fun��es de Apoio */

	// Tratamento de vari�veis
	public function getArrayDadosFormulario()
	{
		$parametros = array();
	
		if( ( !empty($this->formulario['estuf'])
				&& $this->formulario['estuf_campo_flag'] == 1)
				|| !empty($this->formulario['estuf'][0]) ){
	
			$parametros['estuf'] = implode("','", $this->formulario['estuf']);
		}
	
		if( !empty($this->formulario['entid'])
				&& $this->formulario['entid_campo_flag'] == 1
				|| !empty($this->formulario['entid'][0]) ) {
			$parametros['entid'] = implode(',', $this->formulario['entid']);
		}
	
		if( !empty($this->formulario['estudantesaptos']) ) {
			$parametros['estudantesaptos'] = $this->formulario['estudantesaptos'];
		}
	
		if( !empty($this->formulario['estudantesinaptos']) ) {
			$parametros['estudantesinaptos'] = $this->formulario['estudantesinaptos'];
		}
	
		if( !empty($this->formulario['mundescricao']) ) {
			$parametros['mundescricao'] = $this->formulario['mundescricao'];
		}
	
		if( !empty($this->formulario['naopagamento']) ) {
			$parametros['naopagamento'] = $this->formulario['naopagamento'];
		}
	
		if( !empty($this->formulario['simpagamento']) ) {
			$parametros['simpagamento'] = $this->formulario['simpagamento'];
		}
	
		if( !empty($this->formulario['esfera']) ) {
			$parametros['esfera'] = $this->formulario['esfera'];
		}
	
		if( !empty($this->formulario['esdid']) ) {
			$parametros['esdid'] = $this->formulario['esdid'];
		}
	
		if( !empty($this->formulario['perid']) ) {
			$parametros['perid'] = $this->formulario['perid'];
		}
	
		if( !empty($this->formulario['apcid']) ) {
			$parametros['apcid'] = $this->formulario['apcid'];
		}
	
		if( !empty($this->formulario['usucpf']) ) {
			$parametros['usucpf'] = $this->formulario['usucpf'];
		}
	
	
		return $parametros;
	}
	
	// Verifica se o par�metro � um ARRAY e se seus elementos n�o est�o vazios 
	public function arrayEstaVazio( $var )
	{
		$retorno  = true;
		$contador = 0;

		if( !empty($var) && is_array($var) && count($var) > 0 )
		{
			//throw new Exception( 'Par�metro n�o � do tipo array.' );
			foreach( $var as $chave=>$valor ){

				if( empty( $var[$chave] ) ){
					$contador++;
				}
			}
			
			// Valida se foi encontrado registro vazio
			if( $contador > 0 ){
				$retorno = true;
			}else{
				$retorno = false;
			}
		}

		return $retorno;
	}
	
	// Fun��o de Debug
	public function db( $var )
	{
		echo '<br>';
		var_dump( $var );
		exit;	
	}
}?>