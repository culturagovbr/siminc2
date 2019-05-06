<?php
//echo '<br />Debug: <br />'; var_dump($GLOBALS['servidor_bd']); die;
//$GLOBALS["memcacheport"] = "11211";

//if($_SERVER['HTTP_HOST']=='simec.mec.gov.br') $GLOBALS["memcachehost"] = "tcp://10.1.3.108:11215?persistent=0&amp;weight=1&amp;timeout=1&amp;retry_interval=15";
//elseif($_SERVER['HTTP_HOST']=='treinamentosimec.mec.gov.br') $GLOBALS["memcachehost"] = "tcp://200.130.3.80:11210?persistent=0&amp;weight=1&amp;timeout=1&amp;retry_interval=15";
//else $GLOBALS["memcachehost"] = "tcp://10.1.3.157:11212?persistent=0&amp;weight=1&amp;timeout=1&amp;retry_interval=15";

/*
 Sistema Sistema Simec
 Setor responsável: SPO/MEC
 Desenvolvedor: Desenvolvedores Simec
 Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
 Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
 Módulo: classes_simec.inc
 Finalidade: reunião de todas as classes do sistema
 Data de criação: 24/06/2005
 */
class cls_classes_simec {
	var $mess_erro;
	var $ultimo_erro;
	function cls_classes_simec() {
		$this->mess_erro[101] = "Não foi possível estabelecer uma conexão com o servidor LDAP.";
		$this->mess_erro[102] = "Não foi possível autenticar no servidor LDAP.";
		$this->mess_erro[103] = "A pesquisa não retornou resultado.";
		$this->mess_erro[104] = "Não foi possível estabelecer uma conexão com o cls_banco de Dados.";
		$this->mess_erro[105] = "Falha na seleção da base de dados.";
		$this->mess_erro[106] = "Falha na execução da operação. Foi enviada uma mensagem para o Administrador.";
		$this->mess_erro[107] = "Usuário ou senha inválida!";
		$this->mess_erro[108] = "Ocorreu um erro durante a execução do Bind.";
		$this->mess_erro[200] = "Registro cancelado.";
	}

	function erro($codErro, $param){
		return $this->mess_erro[$codErro];
	}
}

require_once(APPRAIZ.'includes/FirePHPCore/FirePHP.class.php');

/**
 * Classe manipuladora do Banco de Dados.
 *
 * @example
 * <code>
 * $db = new cls_banco();
 * </code>
 *
 * @package Simec\Core\Db
 * @author Gilberto Arruda Cerqueira Xavier
 * @since 24/06/2005
 */
class cls_banco
{
//	var $link;
	public static $link;
	public $nome_bd;
	var $resultado;
	var $num_rows;
	var $ultimo_SQL;
	var $sql;
	var $oci_id;
	var $tentativas;
	var $arrayQuery; // Variavel Global para o FirePHP
	var $numQuery; // Variavel Global para o FirePHP
	// objeto de debug
	private $cls_dbug;
        
	function cls_banco()
	{
            $this->nome_bd = ($this->nome_bd ? $this->nome_bd : $GLOBALS['nome_bd']);

            $cls_classes_simec = new cls_classes_simec();
            $cls_classes_simec->cls_classes_simec();

            self::$link[$this->nome_bd] = pg_connect("host=".$GLOBALS["servidor_bd"]." port=".$GLOBALS["porta_bd"]." dbname=".$GLOBALS['nome_bd']."  user=".$GLOBALS["usuario_db"] ." password=".$GLOBALS["senha_bd"] ."");

            if($GLOBALS["servidor_bd"] != ''){
                pg_query(self::$link[$this->nome_bd], "SET SEARCH_PATH TO public, seguranca, auditoria, monitora, elabrev");
            } else {
                pg_query(self::$link[$this->nome_bd], "SET SEARCH_PATH TO public, seguranca, auditoria");
            }

            pg_query(self::$link[$this->nome_bd], "SET STANDARD_CONFORMING_STRINGS TO OFF;");

            pg_set_client_encoding(self::$link[$this->nome_bd],'LATIN1');

            if((!self::$link[$this->nome_bd]))
                cls_classes_simec::erro(104,2);
            else
                $this->tentativas = 0;
            return 1;
	}

	/**
	 * function destrutora
	 * @description : Encerra a conexão com o Banco de Dados.
	 * @author Gilberto Arruda Cerqueira Xavier
	 * @since 24/06/2005
	 */
	public function __destruct()
	{
		if (isset($_SESSION['transacao']))
		{
			pg_query(self::$link[$this->nome_bd], 'rollback; ');
			unset($_SESSION['transacao']);
		}
		if (is_resource(self::$link[$this->nome_bd])) {@pg_close( self::$link[$this->nome_bd] );}
	}




	///////////////////Grficos do sistema
	function grafico_validacao_prg($prgid=0,$refcod=0)
	{
		$sql= 'select * from validacaostat where refcod='.$refcod.' and prgid='.$prgid;
		$rs=$this->recuperar($sql);
		if (!is_array($rs))
		return '<img src="../imagens/valida3.gif" align="absmiddle" width="15" height="15">';
		else
		return monta_grafico_validacao($rs['faltap'],$rs['total_cor1'],$rs['total_cor2'],$rs['total_cor3'],$rs['acatotal']);

	}

	////////////////////Fim Grficos

	/**
	 * function eof
	 * @description : Retorna verdadeiro se existir dados para a consulta executada.
	 * @param string sql (obrigatório)
	 * @return : boolean
	 * @author Gilberto Arruda Cerqueira Xavier
	 * @since 24/06/2005
	 */
	function eof($sql)
	{
		$RS = $this->carregar($sql);
		$nlinhas = $RS ? count($RS) : 0;
		if ($nlinhas==0) return true; else return false;
	}

	/**
	 * Função que retorna uma string retirando caracteres de ataque sql
	 *
	 * @param unknown_type $string
	 * @param unknown_type $p
	 * @return string
	 */
	/**
	 * function antiInjection
	 * @description : Retorna a string SQL retirando possíveis ataques SQL.
	 * @param string string (obrigatório)
	 * @param string p (opcional)
	 * @return : string
	 * @author Gilberto Arruda Cerqueira Xavier
	 * @since 24/06/2005
	 */
	function antiInjection($string,$p="")
	{
		// remove palavras que contenham sintaxe sql
		$string = str_replace('"',"\'",$string);
		$string = str_replace("'","\'",$string);

		$string = ($string);
		if($p !=2){
			$string = str_replace( "|" , "" , $string);
		}
		$string = preg_replace(sql_regcase("/(from|select|insert|delete|where|drop table|show tables|#|--|\\\\)/"),"",$string);
		$string = simec_trim($string);//limpa espaços vazio
		$string = addslashes($string);//Adiciona barras invertidas a uma string
		if($p!=1){
			if(strlen(simec_trim($string))==0) $string ='';
		}

		return $string;

	}

	/**
	 * function salvaQueryArray
	 * @description : Retorna a string de resultado da execução do SQL. / Retorna todas as consultas para o FirePHP
	 * @param string $query (obrigatório)
	 * @param string $momentoAnterior (obrigatório)
	 * @param string $momentoPosterior (obrigatório)
	 * @return : boleano
	 * @author Thiago Tasca Barbosa
	 * @since 03/05/2010
	 */
	function salvaQueryArray($query,$momentoAnterior, $momentoPosterior){
		$timestart    = explode (' ', $momentoAnterior);
		$timefinish   = explode (' ', $momentoPosterior);
		$seg =  $timefinish[0] - $timestart[0];
		$mseg = $timefinish[1] - $timestart[1];

		if ($seg < 0){ // Deixa resultado positivo caso necessario.
	    	$seg++; $mseg--;
	  	}
		$tempoFinal = $seg + $mseg;

		$this->arrayQuery[$this->numQuery]['query'] = $query;
		$this->arrayQuery[$this->numQuery]['tempo'] = $tempoFinal;
		$this->numQuery +=  1;
		return true;
	}

	/**
         * Executa consulta na base de dados e Cria um log de auditoria.
         * 
	 * @param string SQL (obrigatório)
	 * @param boolean auditoria (opcional) Cria um log de auditoria
	 * @return string
	 */
	function executar($SQL, $auditoria = NULL) {
            $auditoria = $auditoria ? $auditoria : EXECUTAR_AUDITORIA;
            
            if (gettype(self::$link[$this->nome_bd]) != "resource") {
                self::$link[$this->nome_bd] = null;
                self::cls_banco();
            }

            $SQL = simec_trim($SQL);

            # Detecta tipo de operacao de tabela(Insert, Update ou Delete)
            preg_match('/(CREATE\s+TABLE|ALTER\s+TABLE|DROP\s+TABLE|SELECT.*FROM|INSERT\s+INTO|UPDATE|DELETE\s+FROM)\s+([A-Za-z0-1.]+).*/smui', utf8_encode($SQL), $matches);
            $audtipoCompleto = strtoupper($matches[1]);
            $audtipo = substr($audtipoCompleto, 0, 1);
            $audtabela = strtoupper($matches[2]);

            if($audtipoCompleto == 'ALTER TABLE' || $audtipoCompleto == 'DROP TABLE'){
                throw new Exception('A execução do comando "' . $audtipoCompleto . '" não é permitido pelo sistema');
            }

            $_SESSION['sql'] = $SQL;
            $momentoAnterior = microtime();

            # Inicia a transação quando nao estiver iniciada e obrigatoriamente quando a operação for diferente de SELECT
            if (!isset($_SESSION['transacao']) && $audtipo != 'S') {
                $this->resultado = pg_query(self::$link[$this->nome_bd], 'begin transaction; ');
                $_SESSION['transacao'] = '1';
            }
            
//            try {
                $this->resultado = @pg_query(self::$link[$this->nome_bd], $SQL);
//            } catch(Exception $e) {
//                ver($e->getMessage());
//            }

            $momentoPosterior = microtime();
            $this->salvaQueryArray($_SESSION['sql'], $momentoAnterior, $momentoPosterior);

            if ($this->resultado == null) {
                if (is_resource(self::$link[$this->nome_bd])) {
//                    throw new Exception( $SQL );
                }
            }

            if ($auditoria && $audtipo != 'S') {
                //Se no tiver vindo de algum módulo
                if ($_SESSION['mnuid'] == ''){
                    $_SESSION['mnuid'] = 1;
                }
                //Se no tiver sessão validada
//                if ($_SESSION['usucpf'] == ''){
//                    $_SESSION['usucpf'] = '';
//                }
                //Insere dados na tabela auditoria
                $id = $_SESSION['sisid']? $_SESSION['sisid']: 4;
                
                $SQL = pg_escape_string($SQL);
                $sql_audit = self::montarSqlAuditar($SQL, $audtabela, $audtipo, $id);

                pg_query(self::$link[$this->nome_bd], $sql_audit);
            }

            return $this->resultado;
        }
        
        /**
         * Monta sql para inserir dado de auditoria.
         * 
         * @param string $sqlOperacao Sql com a operação no banco.
         * @param string $tabela
         * @param string $tipoManipulacao
         * @param integer $sistema
         * @return string
         */
        protected function montarSqlAuditar($sqlOperacao, $tabela, $tipoManipulacao, $sistema){
            $sql = "
                INSERT INTO auditoria(
                    usucpf,
                    mnuid,
                    audsql,
                    auddata,
                    audtabela,
                    audtipo,
                    audip,
                    sisid,
                    audscript
                ) values (
                    '". $_SESSION['usucpforigem']. "',
                    ". $_SESSION['mnuid']. ",
                    '". $sqlOperacao. "',
                    '". date('Y-m-d H:i:s'). "',
                    '". $tabela. "',
                    '". $tipoManipulacao. "',
                    '". $_SERVER["REMOTE_ADDR"]. "',
                    $sistema,
                    '". pg_escape_string($_SERVER['REQUEST_URI']). "'
                )";

            return $sql;
        }

    /**
	 * function executar
	 * @description : Retorna um objeto como resultado da execução do sql.
	 * @param string sql (obrigatório)
	 * @param boolean class (opcional)
	 * @return : object
	 * @author Gilberto Arruda Cerqueira Xavier
	 * @since 24/06/2005
	 */
	function pegaUmObjeto( $sql, $class = null ){
		if (!$sql or $sql == '')
        	return (object) '';

		$res = $class ? pg_fetch_object( $this->executar($sql), null, $class) : pg_fetch_object( $this->executar($sql) );
		return $res;

	}

	/**
	 * function carregarObjeto
	 * @description : Retorna um array de objetos como resultado da execução do sql.
	 * @param string sql (obrigatório)
	 * @param boolean class (opcional)
	 * @return : array
	 * @author Gilberto Arruda Cerqueira Xavier
	 * @since 24/06/2005
	 */
	function carregarObjeto( $sql, $class = null ){
		if (!$sql or $sql == '')
        	return (object) '';

		$res = $this->executar( $sql );
        while ( $linhaObj = ($class ? pg_fetch_object( $res, null, $class ) : pg_fetch_object( $res )) ){
//        	$linhaObj = ($class ? pg_fetch_object( $res, null, $class ) : pg_fetch_object( $res ));
        	$result[] = $linhaObj;
        }

		return $result;

	}

	/**
	 * function apagatudo
	 * @description : Não definido.
	 * @param string texto (obrigatório)
	 * @param string tabela (obrigatório)
	 * @return : boolean (false)
	 * @author Gilberto Arruda Cerqueira Xavier
	 * @since 24/06/2005
	 */
	function apagatudo($texto,$tabela)
    {
        return false;
	}

	/**
	 * function recuperar
	 * @description : Retorna os dados da consulta sql. / Atribui o resultado da consulta a uma variável global.
	 * @param string SQL (obrigatório)
	 * @param string var (opcional)
	 * @return : array / null
	 * @author Gilberto Arruda Cerqueira Xavier
	 * @since 24/06/2005
	 */
	function recuperar($SQL, $var = null) {
		if (! $SQL or $SQL=='')
            return null;
		else {
			$res = pg_fetch_all($this->executar($SQL));
			if ( $var != null )
			{
				global ${$var};
				${$var} = $res[0];
			}
			return $res[0];
		}
	}

	/**
	 * function busca_explicacao
	 * @description : Retorna os dados detalhados de um campo.
	 * @param string campo (obrigatório)
	 * @param string tabela (obrigatório)
	 * @return : string
	 * @author Gilberto Arruda Cerqueira Xavier
	 * @since 24/06/2005
	 */
	function busca_explicacao($campo,$tabela)
	{
		$sql="select p2.atttypmod-4 as tmax, p4.typname as tipo from pg_attribute p2 left join pg_type p4 on p2.atttypid=p4.oid left join pg_description p1 on p2.attnum=p1.objsubid left join pg_class p3 on p3.relfilenode=p2.attrelid and p3.relname='$tabela' where p2.attname='$campo' limit 1";

		$RS = $this->record_set($sql);
		$res = @$this->carrega_registro($RS,0);
		if ($res['explicacao']=='')
		{

			$sql="select distinct p1.description as explicacao from pg_description p1 inner join pg_attribute p2 on p2.attrelid=p1.objoid and simec_trim(p2.attname) = '$campo' and p2.attnum=p1.objsubid inner join pg_class p3 on p3.relname='$tabela' and p3.reltype::integer=p2.attrelid::integer +1 ";


			$RS = $this->record_set($sql);
			$nlinhas = $this->conta_linhas($RS);

			if ($nlinhas>=0)
			{
				$res2 = @$this->carrega_registro($RS,0);
				$res['explicacao'] = $res2['explicacao'];

			} else   $res['explicacao'] = 'Campo sem descrição detalhada';
		}

		return $res;

	}

	/**
	 * function carregar
	 * @description : Retorna os dados da consulta sql. / Atribui o resultado da consulta a uma variável global.
	 * @param string SQL (obrigatório)
	 * @param string var (opcional)
	 * @return : array / null
	 * @author Gilberto Arruda Cerqueira Xavier
	 * @since 24/06/2005
	 */
	function carregar($SQL, $var = null, $tempocache = null){
		//if($_SERVER['HTTP_HOST']=='simec-local')$tempocache=null;
    	if($tempocache) {

    		//global $memcache_obj;
			/* conectando com o memcached server */
    		//if(!$memcache_obj) $memcache_obj = memcache_connect($GLOBALS["memcachehost"], $GLOBALS["memcacheport"]);
			/* pegando informações do memcached server, key igual ao md5 do SQL */

    		if (function_exists('zend_shm_cache_fetch')) {$cache_result = zend_shm_cache_fetch(md5($SQL));}
			//$cache_result = memcache_get($memcache_obj, md5($SQL));

    		if($cache_result) { /* se existir cache, carregar com o resultado do memcached server */
    			$res = $cache_result;
    		} else { /* senão executa o SQL e guarda o resultado no memcached server */
    			$res = pg_fetch_all($this->executar($SQL));
    			/* Armazenando os dados memcached server na chave md5(SQL), 0 => sem compressão, tempo para expirar de 30 seconds */
    			//memcache_set($memcache_obj, md5($SQL), $res, 0, $tempocache);
    			if (function_exists('zend_shm_cache_store')) {
    				if(zend_shm_cache_store(md5($SQL), $res, $tempocache) === false) echo '[ZEND CACHE FALHOU]';
    			}
    		}

    	} else {
            $res = $this->executar($SQL);
            if($res){
                $res = pg_fetch_all($res);
            }

    	}

		if ( $var != null )
		{
			global ${$var};
			${$var} = $res;
		}

		return $res;
	}

	function carregarColuna($SQL, $coluna = '',$var = null)
    {

		$res = pg_fetch_all($this->executar($SQL));

		if ( $var != null )
		{
			global ${$var};
			${$var} = $res;
		}

		if (!$res) return array();

		$result = array();

		foreach ($res as  $row ){
			if(!$coluna){
				array_push($result,reset($row));
			}
			else{
				array_push($result,$row[$coluna]);
			}
		}
		return $result;
	}

	function verifica_momento() {
		// esta função verifica o momento
		$sql="select refcod from elabrev.referencia where refdata_inicio<=current_date and refdata_limite_momento1 >=current_date and refstatus='A' and refano_ref='".$_SESSION['exercicio']."'";
		if ($this->pegaUm($sql)) return 1;
		else {
			$sql="select refcod from elabrev.referencia where refdata_inicio<=current_date and refdata_limite_momento2 >=current_date and refstatus='A' and refano_ref='".$_SESSION['exercicio']."'";
			if ($this->pegaUm($sql)) return 2;
			else return 0;
		}
	}

	function verifica_momento2() {
		// esta função verifica, pelo cpf do usuário logado e pela data se ele tem direito de agir no sistema e se está dentro do seu momento.
		//1. verificando se o usuário é coordenador de planejamento ou equipe de apoio, ou superusuário ou UMA ou coordenador da UMA
		$retorna=0;
		$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (33,68) and pu.usucpf='".$_SESSION['usucpf']."'";
		$resp = $this->pegaUm($sql);
		if ($resp > 0 ) {$ok=1;$respeqpplan = $resp;}
		$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (24,67) and pu.usucpf='".$_SESSION['usucpf']."'";
		$resp = $this->pegaUm($sql);
		if ($resp > 0 ) {$ok=1;$respcoordplan = $resp;}
		$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (63,66) and pu.usucpf='".$_SESSION['usucpf']."'";
		$resp = $this->pegaUm($sql);
		if ($resp > 0 ) {$ok=1;$respuma = $resp;}
		$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (36) and pu.usucpf='".$_SESSION['usucpf']."'";
		$resp = $this->pegaUm($sql);
		if ($resp > 0 ) {$ok=1;$respcoorduma = $resp;}
		$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (62) and pu.usucpf='".$_SESSION['usucpf']."'";
		$resp = $this->pegaUm($sql);
		if ($resp > 0 ) {$ok=1;$respsupusu = $resp;	}

		if ($ok > 0)
		{
			// então pode editar
			// verifica se a data está dentro do período autorizado
			// $sql="select refdata_inicio as inicio, refdata_limite_momento1 as momento1,refdata_limite_momento2 as momento2,refdata_limite_momento3 as momento3 from elabrev.referencia where refstatus='A' and refano_ref='".$_SESSION['exercicio']."'";
			//$momentos=$this->pegaLinha($sql,0);
			// se for coordenador de planejamento ou equipe de planejamento - perfil 24 ou 33 e se estiver dentro do momento 1, então ok.
			if ($respcoordplan or $respeqpplan or $respsupusu)
			{
				$sql="select refcod from elabrev.referencia where refdata_inicio<=current_date and refdata_limite_momento1 >=current_date and refstatus='A' and refano_ref='".$_SESSION['exercicio']."'";
				$ref=$this->pegaUm($sql) ;
			}
			// se for uma ou coordenador da uma e se estiver dentro do momento 2, então ok.
			if ($respcoorduma or $respuma or $respsupusu)
			{
				$sql="select refcod from elabrev.referencia where refdata_inicio<=current_date and refdata_limite_momento2 >=current_date and refstatus='A' and refano_ref='".$_SESSION['exercicio']."'";
				$ref=$this->pegaUm($sql) ;
			}
			if ($ref) $retorna=1;

		}
		return $retorna;
	}

	// $db->ehdecisor($_SESSION['acaid'],'A',$momento);
	function ehcoorduma($cpf)
	{
		// esta função verifica se o usuário possui perfil de coordenador da uma
		$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (36) and pu.usucpf='$cpf'";
		if ($this->pegaum($sql)) return 1; else return 0;

	}
	function ehdecisor($cod,$tab,$mom)
	{
		// esta função verifica, pelo cpf do usuário logado e pelo momento se ele é decisor externo, interno ou não é decisor
		if ($this->testa_superuser()) return 1;
		if ($mom==1)
		{
			if ($tab=='A')
			{
				// ação no momento 1
				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (24,67) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql))
				{
					// então o usuário possui perfil de coordenador de planejamento de unidade ou ação
					// testo a unidade
					$sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ua.acaid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
					if ($this->pegaUm($sql)) return 1;
					else
					{
						// testo a responsabilidade em ação
						$sql = "select distinct ur.acaid from elabrev.usuarioresponsabilidade ur where ur.acaid=$cod and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						if ($this->pegaUm($sql)) return 1;
						else return 0;
					}
				}

			}
			if ($tab=='P')
			{
				// programa no momento 1
				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (24,67) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql))
				{
					// então o usuário possui perfil de coordenador de planejamento de unidade ou ação
					// testo a unidade
					$sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ap.prgid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
					if ($this->pegaUm($sql)) return 1;
					else
					{
						// testo a responsabilidade em programa
						$sql = "select distinct ur.prgid from elabrev.usuarioresponsabilidade ur where ur.prgid=$cod and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						if ($this->pegaUm($sql)) return 1;
						else return 0;
					}
				}

			}
		}
		if ($mom==2)
		{
			if ($tab=='A')
			{
				// ação no momento 2
				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (36) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql)) return 1; // coordenador da UMA

				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (63,66) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql))
				{
					// então o usuário possui perfil de uma de unidade ou ação ou coord uma
					// testo a unidade
					$sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ua.acaid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
					if ($this->pegaUm($sql)) return 1;
					else
					{
						// testo a responsabilidade em ação
						$sql = "select distinct ur.acaid from elabrev.usuarioresponsabilidade ur where ur.acaid=$cod and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						if ($this->pegaUm($sql)) return 1;
						else return 0;
					}
				}

			}
			if ($tab=='P')
			{
				// programa no momento 2
				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (36) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql)) return 1; // coordenador da UMA

				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (63,66) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql))
				{
					// então o usuário possui perfil de uma de unidade ou ação ou coord uma
					// testo a unidade
					$sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ap.prgid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
					if ($this->pegaUm($sql)) return 1;
					else
					{
						// testo a responsabilidade em ação
						$sql = "select distinct ur.prgid from elabrev.usuarioresponsabilidade ur where ur.prgid=$cod and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						if ($this->pegaUm($sql)) return 1;
						else return 0;
					}
				}

			}
		}
	}

	function ehdigitador($cod,$tab,$mom)
	{

		// esta função verifica, pelo cpf do usuário logado e pelo momento se ele é digitador externo, interno ou não é digitador
		if ($mom==1)
		{
			if ($tab=='A')
			{
				// ação no momento 1
				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (33,68) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql))
				{
					// então o usuário possui perfil de equipe de planejamento de unidade ou ação
					// testo a unidade
					$sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ua.acaid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
					if ($this->pegaUm($sql)) return 1;
					else
					{
						// testo a responsabilidade em ação
						$sql = "select distinct ur.acaid from elabrev.usuarioresponsabilidade ur where ur.acaid=$cod and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						if ($this->pegaUm($sql)) return 1;
						else return 0;
					}
				}

			}
			if ($tab=='P')
			{
				// programa no momento 1
				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (33,68) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql))
				{
					// então o usuário possui perfil de equipe de planejamento de unidade ou ação
					// testo a unidade
					$sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ap.prgid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
					if ($this->pegaUm($sql)) return 1;
					else
					{
						// testo a responsabilidade em programa
						$sql = "select distinct ur.prgid from elabrev.usuarioresponsabilidade ur where ur.prgid=$cod and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						if ($this->pegaUm($sql)) return 1;
						else return 0;
					}
				}
			}
		}
		if ($mom==2)
		{
			if ($tab=='A')
			{
				// ação no momento 2
				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (36,62) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql)) return 1; // é coordenador da UMA ou super usuário

				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (63,66) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql))
				{
					// então o usuário possui perfil de UMA unidade ou programa
					// testo a unidade
					$sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ua.acaid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
					if ($this->pegaUm($sql)) return 1;
					else
					{
						// testo a responsabilidade em ações no programa específico
						$sql = "select distinct ur.prgid from elabrev.usuarioresponsabilidade ur where ur.prgid in (select prgid from elabrev.ppaacao_proposta where acaid=$cod) and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						if ($this->pegaUm($sql)) return 1;
						else return 0;
					}
				}
			}
			if ($tab=='P')
			{
				// programa no momento 2
				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (36,62) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql)) return 1; // é coordenador da UMA ou super usuário

				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (63,66) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql))
				{
					// então o usuário possui perfil de equipe de perfil de UMA unidade ou programa
					// testo a unidade
					$sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta ap on ap.acaid = ua.acaid and ap.prgid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
					if ($this->pegaUm($sql)) return 1;
					else
					{
						// testo a responsabilidade em programa
						$sql = "select distinct ur.prgid from elabrev.usuarioresponsabilidade ur where ur.prgid=$cod and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						if ($this->pegaUm($sql)) return 1;
						else return 0;
					}
				}

			}
		}
	}

	function ehcriador($cod,$busca=0)
	{
		if (! $busca or $busca=='A')
		$sql = "select acaid from elabrev.ppaacao_proposta where usucpf='".$_SESSION['usucpf']."' and acaid=$cod and prsano='".$_SESSION['exercicio']."'";
		else if ($busca == 'P')
		$sql = "select prgid from elabrev.ppaprograma_proposta where usucpf='".$_SESSION['usucpf']."' and prgid=$cod and prsano='".$_SESSION['exercicio']."'";
		else if ($busca == 'EA')
		$sql = "select acaid from elabrev.proposta_exclusao_acao where usucpf='".$_SESSION['usucpf']."' and acaid=$cod and prsano='".$_SESSION['exercicio']."'";
		else if ($busca == 'MA')
		$sql = "select acaid from elabrev.proposta_migracao_acao where usucpf='".$_SESSION['usucpf']."' and acaid=$cod and prsano='".$_SESSION['exercicio']."'";
		else if ($busca == 'EP')
		$sql = "select prgid from elabrev.proposta_exclusao_programa where usucpf='".$_SESSION['usucpf']."' and prgid=$cod and prsano='".$_SESSION['exercicio']."'";
		else if ($busca == 'CP')
		$sql = "select eraid from elabrev.elaboracaorevisao where usucpf='".$_SESSION['usucpf']."' and eraid=$cod and prsano='".$_SESSION['exercicio']."'";

		if ($this->pegaUm($sql)) return true;
		else return false;
	}


	/**
	 * function escape
	 * @description : Escapa o valor, aplicando o tratamento conforme o tipo do mesmo.
	 * @param string Valor do Campo (obrigatório)
	 * @return : array / null / numeric
	 * @author Gilberto Arruda Cerqueira Xavier
	 * @since 24/06/2005
	 */
	function escape( $mixValorCampo  ){


		if( ( gettype( $mixValorCampo ) == 'integer' ) || ( gettype( $mixValorCampo ) == 'float') )
		{
			return $mixValorCampo;
		}
		if( is_string( $mixValorCampo ) )
		{
			return "'" . pg_escape_string( self::$link[$this->nome_bd], $mixValorCampo ) . "'" ;
		}
		if( is_null( $mixValorCampo ) )
		{
			return 'NULL';
		}
		return $mixValorCampo;
	}



	function verifica_resp_elabrev($cod=0,$tab=0)
	{
		unset($_SESSION['uniorc']);
		//$cod= ação ou programa
		//$tab = tabela de ação ou tabela de programa
		// esta função verifica, pelo cpf do usuário logado e pela id da ação e do programa se ele pode editar o registro.
		//1. verificando se o usuário é coordenador de planejamento ou equipe de apoio, ou superusuário ou UMA ou coordenador da UMA
		$retorna=0;
		$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (24,33,36,62,63,66,67,68) and pu.usucpf='".$_SESSION['usucpf']."'";
		$resp = @$this->carregar( $sql );
		if (  $resp && count($resp) > 0 )
		{
			foreach ( $resp as $linha )
			{
				foreach($linha as $k=>$v) ${$k}=$v;
				if ($pflcod == 24 or $pflcod==33)
				{
					// então pode editar
					//verifica se a ação pertence à unidade
					if ($_SESSION['acaid'])
					{
						$sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur
inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod
inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ua.acaid=".$_SESSION['acaid']." where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						$ret1=$this->pegaUm($sql);
					}
					//$retorna=$resp;
					// verifica, se for perfil 24 ou 33 quais as unidades que ele pode operar
					$sql= "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
					$rs = $this->carregar($sql);
					if (  $rs && count($rs) > 0 )
					{
						foreach ( $rs as $ln )
						{
							foreach($ln as $k=>$v) ${$k}=$v;
							$_SESSION['uniorc'][] = $unicod;
						}
					}
				}
				if ($pflcod == 67 or $pflcod==68)
				{
					// então pode editar
					//verifica se está responsável pela ação
					if ($_SESSION['acaid'])
					{
						$sql = "select distinct ur.acaid from elabrev.usuarioresponsabilidade ur where ur.acaid=".$_SESSION['acaid']." and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						$ret2=$this->pegaUm($sql);
					}
				}
				if ($pflcod == 67 or $pflcod==68)
				{
					// então pode editar
					// verifica se está responsável pela ação
					if ($_SESSION['acaid'])
					{
						$sql = "select distinct ur.acaid from elabrev.usuarioresponsabilidade ur where ur.acaid=".$_SESSION['acaid']." and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						$ret3=$this->pegaUm($sql);
					}
				}
				if ($pflcod == 62 or $pflcod==34 or $pflcod==36)
				{
					unset($_SESSION['uniorc']);
					$ret4 = 1;
				}
			}
		}
		if ($ret1) $retorna=$ret1;
		if ($ret2) $retorna=$ret2;
		if ($ret3) $retorna=$ret3;
		if ($ret4) $retorna=$ret4;

		return $retorna;
	}


	function verifica_geral()
	{
		// esta funação verifica vários itens
		// verificando o plano de trabalho
		// fase 1 - verifica se o usuário tem responsabilidades em ação
		$sql="select ur.acaid from monitora.usuarioresponsabilidade ur inner join seguranca.usuario u on u.usucpf=ur.usucpf inner join monitora.acao a on a.acaid=ur.acaid and a.prgano='".$_SESSION['exercicio']."' where ur.acaid is not null and ur.usucpf='".$_SESSION['usucpf']."'";

		$RS = $this->record_set($sql);
		$nlinhas = $this->conta_linhas($RS);
		if ($nlinhas >= 0) {
			// então o usuário possui responsabilidade
			for ($i=0;$i<=$nlinhas;$i++)
			{
				$res =  $this->carrega_registro($RS,$i);
				if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
				// para cada ação, verifico se existe plano de trabalho
				$sql = "select p.ptoavisoantecedencia  as antec from monitora.plantrabacao pa inner join monitora.planotrabalho p on p.ptoid=pa.ptoid and p.ptotipo in ('F','E') and to_char(p.ptodata_fim,'YYYY') >='".date('Y')."' "." where pa.acaid=$acaid";
				$res =  $this->pegaUm($sql);
				if ($res) $antec=$res;
				if (! $antec) $antec=7;
				$sql = "select pa.* , p.ptodata_ini,p.ptodata_fim, p.ptodsc,a.acacod, a.acadsc from monitora.plantrabacao pa inner join monitora.planotrabalho p on p.ptoid=pa.ptoid and p.ptotipo in ('F','E') and ptodata_ini <= current_date- interval '$antec days' and to_char(p.ptodata_fim,'YYYY') >='".date('Y')."' "." inner join acao a on a.acaid=pa.acaid where pa.acaid=$acaid";

				$RS2 = $this->record_set($sql);
				$nlinhas2 = $this->conta_linhas($RS2);
				// agora tenho os planos de trabalho da ação testada.
				// para cada ptoid vou verificar se já foi lançado algum evento.
				$ok=1;
				if ($nlinhas2 >= 0) {
					for ($ii=0;$ii<=$nlinhas2;$ii++)
					{
						$res =  $this->carrega_registro($RS2,$ii);
						$ok=0;
						if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
						$sql = "select e.expid from monitora.execucaopto e where ptoid = $ptoid and acaid = $acaid";
						$RS3 = $this->record_set($sql);
						$nlinhas3 = $this->conta_linhas($RS3);
						if ($nlinhas3 >= 0) {$ok=1;break;}
						else
						{
							// então não fez nada que devia
							$_SESSION['texto'][] ="Você cadastrou atividades no Plano de Trabalho - $ptodsc da ação $acacod-$acadsc e até o momento não monitorou a atividade";
							$popup=1;
						}
					}
				}

			}
			if ($popup)
			{
				?>
						<script>
             	     	e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/msg_geral&acao=A";
             	     	window.open(e, "Associação_de_Responsáveis","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=600,height=400'");
             	     	</script>
				<?php
			}
		}
	}

	/**
	 * Monta combobox html retornando ou imprimindo o componente
	 *
	 * @param string $var (1) Nome da variável que possui o valor selecionado
	 * @param string|array $sql (2) Sql ou array com as opções do combobox
	 * @param string $habil (3) 'S'|'N' Define se será habilitado para o usuário
	 * @param string $titulo (4) Titulo
	 * @param string $acao (5) Nome da funcao a ser executada, ele complementa sozinho o ( this.value )
	 * @param string $opc
	 * @param string $txtdica
	 * @param integer $size (8) Tamanho do campo
	 * @param char $obrig (9) 'S'|'N' Define se é campo obrigatório
	 * @param string $id (10) Nome do id
	 * @param bool $return (11) true|false Retorna(true) o valor ou imprime(false)
	 * @param string $value
     * @param string $title
     * @param string $complemento (14) Atributos complementares do select
	 * @return VOID
	 */
	function monta_combo($var,$sql=null,$habil=true,$titulo='',$acao='',$opc='',$txtdica='',$size='',$obrig='', $id = '', $return = false, $value = null, $title= null, $complemento = null, $classe = null, $complementoOption = array(array('codigo'=>array(), 'complemento'=>'')), $percentual = null)
	{
		global ${$var};

		if (is_array($var)) {
			extract($var);
			$var = $name;
		}

		$value = $value ? $value : ${$var};

		if (is_array($sql)) {
	        $rescombo = $sql;
		} else {
		    $rescombo = $this->carregar( $sql );
		}

		$select = "";
		if ( $txtdica )
		$select = "<span onmouseover=\"return escape('". $txtdica . "');\">";

		// define atributos do select
		if ( $habil == 'S' )
		$select .= "<select {$complemento} name='" . strtolower( $var ) . "' ";
		else
		$select .= "<select {$complemento} name='" . strtolower( $var ) . "_disable' ";

		$class = "CampoEstilo";

		if ( $obrig == 'S' )
			$class .= " obrigatorio";

                $class .= !empty($classe)?' '.$classe:'';

		$select .= " class='$class' ";

		if($title){
			$select .= "title='".  $title  ."'";
		}

		$tpTamanho = $percentual ? '%' : 'px';

		if ( $size <> '' )
		$select .= " style='width:".$size.$tpTamanho.";'";
		else $select .= " style='width: auto'";
		if ( $habil == 'N' )
		$select = $select. ' disabled="disabled" ';
		if ( isset( $tamanho ) && $tamanho > 1 )
		$select .= ' size="' . $tamanho . '" ';
		if ( $acao )
			$select .= ' onchange="' . $acao . '(this.value)"';
		if ( $id )
		$select .= ' id="' . simec_htmlentities( $id ).  '"';
		$select .= ">";

		// cria os options
		if ( $titulo )
		$select .= '<option value="">'.$titulo.'</option>';
		if($rescombo){
			for ( $i = 0; $i < count( $rescombo ); $i++ ) {

                /* Comparando dentro de array para combo com m?tiplas op??es */
                /*
                 * A classe __strict__, é uma classe especial que causa uma alteração no comportamento do in_array,
                 * para maiores detalhes, veja o terceiro parametro da função
                 * in_array: http://php.net/manual/en/function.in-array.php
                 */
                if (is_array($value) && in_array($rescombo[$i]['codigo'], $value, (bool)strpos($classe, '__strict__'))) {
                    $sel = 'selected="selected"';
                } elseif (!is_array($value) && simec_trim($value) == $rescombo[$i]['codigo'] ) {

                    $sel = 'selected="selected"';
                } else {
                    $sel='';
                }

                /* FIM Comparando dentro de array para combo com mútiplas opções */
				$compOption = array();

				if( is_array($complementoOption) ){
					foreach( $complementoOption as $co ){
						if( is_array($co['codigo']) ){
							foreach( $co['codigo'] as $codigo ){
								$compOption[$codigo] = $co['complemento'] ? $co['complemento'] : '';
							}
						}
					}
				}
				$select .= "<option value='" . $rescombo[$i]['codigo'] . "' " . $sel . " " . $compOption[$rescombo[$i]['codigo']] . " >" .
				$rescombo[$i]['descricao'] .
				"</option>\n";
			}
		}
		if ( $value == 'x')
		$sel = 'selected="selected"';
		else
		$sel='';
		if ( $opc )
		$select .= "<option " . $sel . " value='x'>" . $opc . "</option>\n";

		// finaliza impressão do select
		$select .= '</select>';
		if ( $txtdica )
		$select .= "</span>";
		if ( $habil == 'N' )
		$select .= "<input type='hidden' name ='" . strtolower( $var ) . "' value='" . ${$var} . "' />";
		if ( $obrig == 'S' )
		$select .= obrigatorio();

		if ($return)
		return $select;
		else
		echo $select;
	}


	/**
	 * Monta combobox_multipla html imprimindo o componente
	 *
	 * @param string  $var Nome da variável que possui o valor selecionado
	 * @param string|array $sql Sql ou array com as opções do combobox
	 * @param string  $habil 'S'|'N' Define se será habilitado para o usuário
	 * @param string  $titulo Titulo
	 * @param string  $acao Nome da funcao JAVASCRIPT a ser executada, ele complementa sozinho o ( this.value )
	 * @param string  $opc
	 * @param string  $txtdica
	 * @param integer $tamanho Quantidade de opções que serão exibidas
	 * @param integer $size Tamanho do campo
         * @param array $arrSelected Opções que estarão selecionadas ao montar a combo
         * @param integer $id
         * @param boolean $return Define se a função vai retornar uma string ou vai imprimir na tela a combo
         * @param string $classe
	 * @return void
	 */
	function monta_combo_multiplo($var,$sql,$habil,$titulo='',$acao,$opc,$txtdica='', $tamanho=4, $size='',$arrSelected = array(), $id = null,$return = false, $classe = null) {
		// este mtodo monta um combobox onde: var = nome do combo; sql  a query,
		//titulo  o ttulo da primeira linha do combo
		global ${$var};

		if(!is_array(${$var}) || @count(${$var})<1) {
			${$var} = array();
		}

	    if (is_array($sql))
	        $rescombo = $sql;
	    else
		    $rescombo = $this->carregar( $sql );

                $class .= !empty($classe)?' '.$classe:'';

		$select = "";
		if ($txtdica) $select = "<span onmouseover=\"return escape('$txtdica');\">";
		$select .= "<select name='".strtolower($var)."[]' class='CampoEstilo $class' size='" . $tamanho . "' multiple='multiple'";
		if ( $size <> '' )
			$select .= " style='width:".$size."px;'";
		else
			$select .= " style='width: auto'";

		if($id){
			$select .= " id='$id'";
		}

		if ($habil=='N')      $select = $select. ' disabled="disabled" ';

		if ($acao) $select = $select. ' onChange="'.$acao.'(this.value)"';
		$select = $select .">";


		if($titulo) $select .= '<option ' . ((!isset(${$var}) || (@count(${$var})==1 && !(bool)${$var}[0]) ) ? 'selected' : ''). ' value="">'.$titulo.'</option>';
		for ($i=0;$i<count($rescombo);$i++) {
			if (array_search($rescombo[$i]['codigo'], ${$var}) !== false) $sel = '  SELECTED'; else $sel='';
			$select .= "<option ".(is_array($arrSelected) && in_array($rescombo[$i]['codigo'],$arrSelected) ? " selected='selected' " : "").$sel." value='".$rescombo[$i]['codigo']."'>".$rescombo[$i]['descricao']."</option>";
		}
		if (array_search('x', ${$var}) !== false) $sel = '  SELECTED'; else $sel='';
		if ($opc and $opc<>'') $select .="<option ".$sel." value='x'>".$opc."</option>";
		$select .= '</select>';
		if ($txtdica) $select .= "</span>";
		if ($habil=='N') $select .= "<input type='hidden' name ='".strtolower($var)."' value='".${$var}."' />";

		if($return){
			return $select;
		}else{
			echo $select;
		}
	}

	/*
	 * Monta checkboxes a partir de um select as colunas codigo e descricao
	 *
	 * @param $nome String nome do campo
	 * @param $query String a consulta sql que vai ser executada
	 * @param $marcados Array traz os itens que devem ser selecinados
	 * @param $separador String elemento a ser usado entre dois checkboxes por padrão
	 * @param $comLabel ???
         * @param $onclick String nome da função JavaScript usada para alguma ação
	 * @return void
	 */
	function monta_checkbox($nome, $query, $marcados, $separador='  ', $comLabel = false, $onclick = '') {
            if(!is_array($marcados) || @count($marcados)<1) {
                $marcados = array();
            }

            $saida = "";
            $rescombo=$this->carregar($query);

            if(is_array($rescombo) && @count($rescombo)>0) {
                for ($i=0, $j=count($rescombo)-1; $i<count($rescombo); $i++) {
                    $checked = array_search($rescombo[$i]['codigo'], $marcados) !== false ? 'checked="checked"' : '';

                    if (!$comLabel) {
                        echo $rescombo[$i]['indice']."<input type=\"checkbox\" name=\"{$nome}\" id=\"".str_replace('[','',str_replace(']','',$nome))."_{$rescombo[$i]['codigo']}\" value=\"{$rescombo[$i]['codigo']}\" {$checked} onclick=\"{$onclick}\" /> {$rescombo[$i]['descricao']}";
                    } else {
                        $id = str_replace(array(']', '['), '', "{$nome}_{$i}");

                        echo "
                            <input type=\"checkbox\" name=\"{$nome}\" value=\"{$rescombo[$i]['codigo']}\" {$checked} id=\"$id\" onclick=\"{$onclick}\" />
                            &nbsp;
                            <label for=\"$id\"> {$rescombo[$i]['descricao']} </label> &nbsp;&nbsp;&nbsp;
                        ";
                    }
                    if($i!=$j) echo $separador;
                }
            }
	}

	/**
	 * Monta abas de sistema através dos parametros
	 *
	 * @author Gilberto Arruda Cerqueira Xavier
	 * @since 24/06/2005
	 * @param integer 		$abacod_tela codigo da aba (obrigatório)
	 * @param string  		$url diz qual aba ficará destacada (obrigatório)
	 * @param string|array  $parametros adiciona novos parametros ao link da aba (obrigatório)
	 * @param array  		$arMnuid não mostra as abas passada pelo array (não obrigatório)
	 * @return : void
	 */
	function cria_aba($abacod_tela,$url,$parametros, Array $arMnuid = array())
	{
        $parametro = is_array( $parametros ) ? $parametros[$j] : $parametros;

		$where = "";
		if($_SESSION['sisid']){
			$where = " and menu.sisid = {$_SESSION['sisid']} ";
		}

		if( $arMnuid ){
			$filtro = "and menu.mnuid not in (".implode(',', $arMnuid).")";
		}

		//Função cria aba que monta as abas visualmente
		if (simec_trim($abacod_tela)<>'')
		{
			$sql = "select menu.mnuid, menu.mnudsc, menu.mnulink, menu.mnutransacao
	 				  from seguranca.menu, seguranca.aba_menu
					 where menu.mnuid=aba_menu.mnuid
					   and aba_menu.abacod=".$abacod_tela." $where
					   and menu.mnuid in (
					   		select distinct m2.mnuid from seguranca.perfilmenu m2, seguranca.perfilusuario p
					   		where m2.pflcod=p.pflcod and p.usucpf='".$_SESSION['usucpf']."'
					   ) $filtro
				  order by menu.mnucod";
//ver($sql,d);
                        unset($_SESSION['abamenu']);
			if (isset($_SESSION['abamenu'][$abacod_tela]))
			{
				$RS = $_SESSION['abamenu'][$abacod_tela];
			}
			else
			{
				$RS = $this->carregar($sql);
				$_SESSION['abamenu'][$abacod_tela] = $RS;
			}


			if(is_array($RS))
			{


                ?>

                <div class="row">
                <br />
                <div class="col-md-12">
                <ul class="nav nav-tabs">
                    <?php foreach($RS as $tab): ?>
                    <li <?php if($tab['mnulink'] == $url) echo 'class="active"' ?>><a id="tab<?php echo str_replace(' ' , '' , $tab['mnutransacao']) ?>" href="<?php echo $tab['mnulink'] . $parametro ?>"><?php echo $tab['mnudsc'] ?></a></li>
                <?php endforeach; ?>
<!--                    <li><a id="tabSprint" href="#sprint">Ciclos</a></li>-->
<!--                    <li><a id="tabPostit" href="#postit">Entregáveis</a></li>-->
<!--                    <li><a id="tabStory" href="#story">Estórias</a></li>-->
<!--                    <li><a id="tabProject" href="#subprogram">Sub-projeto</a></li>-->
<!--                    <li><a id="tabProgram" href="#program">Projetos</a></li>-->
                </ul>

                </div>
                </div>
                <br />

                <?php
//				print '<table border="0" cellspacing="0" cellpadding="0" align="center" class="notprint nav-tabs"><tr><td><table cellpadding="0" cellspacing="0" align="left"><tr class="nav-tabs">';
//				$nlinhas = count($RS)-1;
//				for ($j=0; $j<=$nlinhas;$j++)
//				{
//					foreach($RS[$j] as $k=>$v) ${$k}=$v;
//					if ($url<>$mnulink and $j==0) $gifaba = "aba_nosel_ini.gif";
//					elseif ($url==$mnulink and $j==0) $gifaba = "aba_esq_sel_ini.gif";
//					elseif ($gifaba=='aba_esq_sel_ini.gif' or $gifaba=='aba_esq_sel.gif') $gifaba = "aba_dir_sel.gif";
//					elseif ($url<>$mnulink) $gifaba = "aba_nosel.gif";
//					elseif ($url==$mnulink) $gifaba = "aba_esq_sel.gif";
//					$parametro = is_array( $parametros ) ? $parametros[$j] : $parametros;
//					if ($url==$mnulink) {$giffundo_aba = "aba_fundo_sel.gif";$cor_fonteaba="#000055"; $active = 'active';} else {$giffundo_aba = "aba_fundo_nosel.gif";$cor_fonteaba="#4488cc"; $active = '';}
////					print '<td height="20" valign="top"></td>';
//					print '<td height="20" class="' . $active . ' tab" align="center" valign="middle" style="color:'.$cor_fonteaba.'; padding-left: 10px; padding-right: 10px;">';
//					if ($mnulink<>$url){print '<a  href="'.$mnulink.$parametro.'" style="color:'.$cor_fonteaba.';" title="'.$mnutransacao.'">'.$mnudsc.'</a>';} else {print $mnudsc.'</td>';}
//				}
////				if ($gifaba=='aba_esq_sel_ini.gif' or $gifaba=='aba_esq_sel.gif') $gifaba= "aba_dir_sel_fim.gif"; else $gifaba = "aba_nosel_fim.gif";
//				print '<td height="20" valign="top"></td></tr></table></td></tr></table> <br />';
			}
		}
	}

	/*
	 * Monta um arquivo Excel a partir de uma Query
	 *
	 * @param $sql String Query ou um Array a ser executada
	 * @param $arquivo String prefixo do arquivo a ser gerado
	 * @param $cabecalho Array Opcional, nome dos campos no cabecalho do arquivo
	 * @param $formatocoluna Array pode ser n (Numero) ou s (String)
	 * @return void
	 */
	function sql_to_excel($sql,$arquivo,$cabecalho="",$formatocoluna="") {
		// este método transforma uma query em excel
		global $nomeDoArquivoXls;
		$nomeDoArquivoXls = "SIMEC_".date("His")."_".$arquivo;

		$xls = new GeraExcel();

		$RS= !is_array( $sql ) ? $this->carregar($sql) : $sql;
		$nlinhas = $RS ? count($RS) : 0;
		if (! $RS) $nl = 0; else $nl=$nlinhas;
		if ($nlinhas>0)
		{
			//Monta Cabeçalho
			if(is_array($cabecalho))
			{
				for ($i=0;$i<count($cabecalho);$i++)
				{
					$xls->MontaConteudoString(0, $i, $cabecalho[$i]);
				}
			}
			else
			{
				$col=0;
				$lin=0;
				foreach($RS[0] as $k=>$v)
				{
					$xls->MontaConteudoString($lin, $col, $k);
					$col++;
				}
			}
			//Monta Listagem
			for ($i=0;$i<$nlinhas;$i++)
			{
				$lin = $i+1;
				$col = 0;
				foreach($RS[$i] as $k=>$v)
				{
					if ($formatocoluna[$col]=='n')
					$xls->MontaConteudoNumero($lin, $col, $v);
					else
					$xls->MontaConteudoString($lin, $col, $v);
					$col++;
				}
			}
			$xls->GeraArquivo();
		}
	}


    /*
    * Monta um arquivo Excel a partir de uma Query utilizando o formato XML
    *
    * @param $sql String Query ou um Array a ser executada
    * @param $arquivo String
    * @param $cabecalho Array Opcional, nome dos campos no cabecalho do arquivo
    * @return void
    */
    function sql_to_xml_excel($sql, $nome_arquivo, array $cabecalho = array())
    {
        ob_clean();

        // este método transforma uma query em excel
        $extension = ".xls";
        $arquivo = "SIMEC_" . $nome_arquivo . "_" . date("d-m-Y_his") . $extension;


        $data = !is_array($sql) ? $this->carregar($sql) : $sql;
        $data = is_array($data) ? $data : array();
        array_unshift($data, $cabecalho);

        $xls = new Excel_XML;
        $xls->addWorksheet($nome_arquivo, $data);
        $xls->sendWorkbook($arquivo);

        die;
    }


	/**
	 * Monta lista simples HTML, com estilo padrão
	 *
	 * @author Gilberto Arruda Cerqueira Xavier
	 * @since 24/06/2005
	 * @param string|array  $sql 			sql ou array com os itens da listagem (obrigatório)
	 * @param array  		$cabecalho 		array contendo os itens do cabeçalho (opcional)
	 * @param inteiro  		$perpage 		Nº de registros por página (obrigatório)
	 * @param inteiro  		$pages			Nº de índices de página visível na barra de paginação (obrigatório)
	 * @param string  		$soma			'S'|'N' define se a lista terá somatória (opcional)
	 * @param string  		$largura		define largura da tabela da lista "padrão 95%" (opcional)
	 * @param string  		$valormonetario	'S'|'N' define formatação dos valores numéricos (opcional)
	 * @return void
	 */
	function monta_lista_tabulado($sql,$cabecalho="",$perpage,$pages,$soma='N',$largura='95%', $valormonetario='S', $totalregistro=false , $arrHeighTds = false , $heightTBody = false, $boImprimiTotal = false ) {
		if(!(bool)$largura) $largura = '95%';
		// este método monta uma listagem na tela baseado na sql passada
		//Registro Atual (instanciado na chamada)
		if ($_REQUEST['numero']=='') $numero = 1; else $numero = intval($_REQUEST['numero']);

	    if (is_array($sql))
	        $RS = $sql;
	    else
	        $RS = $this->carregar($sql);

		$nlinhas = $RS ? count($RS) : 0;
		$totalRegistro = $nlinhas;
		if (! $RS) $nl = 0; else $nl=$nlinhas;
		if (($numero+$perpage)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $numero+$perpage-1;

//			$header .= '<table>';

			//Monta Cabeçalho
			if(is_array($cabecalho))
			{
//				$header.= '<tr>';
//				$header.= chr(13).chr(10);
				for ($i=0;$i<count($cabecalho);$i++)
				{
					$header.= $cabecalho[$i].(($i===$totalregistro)?' (Total:'.count($RS).')':''). chr(9);
				}
			}

//			$header.= "</table>";

			if ($nlinhas>0)
			{

//				$body.= '<table>';

				//Monta Listagem
				$totais = array();
				$tipovl = array();
				for ($i=($numero-1);$i<$reg_fim;$i++)
				{
					$c = 0;
					$body .= chr(13);
					foreach($RS[$i] as $k=>$v) {
						if(is_numeric($v)){
							$v = str_replace(array(",","."),array("",","),$v);
						}
						$body.= str_replace(array(chr(10),chr(13),chr(11),chr(12),chr(9)), ' ',$v);
						$body .= chr(9);
						$c = $c + 1;
					}
//					$body .= chr(13);
				}

//		        $body.= '</table>';

			}

			echo $header;
			echo $body;

	}


       /**
        * Monta lista simples HTML, com estilo padrão
        *
        * @author Gilberto Arruda Cerqueira Xavier
        * @since 24/06/2005
        * @param string|array $sql (1) sql ou array com os itens da listagem (obrigatório)
        * @param array $cabecalho (2) array contendo os itens do cabeçalho (opcional)
        * @param inteiro $perpage (3) Nº de registros por página (obrigatório)
        * @param inteiro $pages (4) Nº de índices de página visível na barra de paginação (obrigatório)
        * @param string $soma (5) 'S'|'N' define se a lista terá somatória (opcional)
        * @param string $largura (6) define largura da tabela da lista "padrão 95%" (opcional)
        * @param string $valormonetario (7) 'S'|'N' define formatação dos valores numéricos (opcional)
        * @param bool $totalregistro (8) ???
        * @param array $arrHeighTds (9) Array de largura das colunas, só funciona se $heightTBody estiver definido
        * @param int $heightTBody (10) Altura da tabela, se os registros não couberem, é criada uma barra de rolagem
        * @param boll $boImprimiTotal (11) Imprime no rodapé da tabela o total de registros
        * @return void
        */
	function monta_lista_simples($sql,$cabecalho="",$perpage,$pages,$soma='N',$largura='95%', $valormonetario='S', $totalregistro=false , $arrHeighTds = false , $heightTBody = false, $boImprimiTotal = false ) {
		if(!(bool)$largura) $largura = '95%';
		// este método monta uma listagem na tela baseado na sql passada
		//Registro Atual (instanciado na chamada)
		if ($_REQUEST['numero']=='') $numero = 1; else $numero = intval($_REQUEST['numero']);

	    if (is_array($sql))
	        $RS = $sql;
	    else
	        $RS = $this->carregar($sql);

		$nlinhas = $RS ? count($RS) : 0;
		$totalRegistro = $nlinhas;
		if (! $RS) $nl = 0; else $nl=$nlinhas;
		if (($numero+$perpage)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $numero+$perpage-1;

		if( is_array( $arrHeighTds ) && is_int($heightTBody) ){

			$header = "<div>";
			$header .= '<table width="'. $largura . '" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem"   >';

			//Monta Cabeçalho
			if(is_array($cabecalho))
			{
				$header.= '<thead><tr>';
				for ($i=0;$i<count($cabecalho);$i++)
				{
					$header.= '<td align="center" width='. $arrHeighTds[$i] .'% valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">'.$cabecalho[$i].(($i===$totalregistro)?' (Total:'.count($RS).')':'').'</label>';
				}
				$header.= '<td><div style="width:10px" ></div></td>';
				$header.= '</tr> </thead>';
			}

			$header.= "</table>";
			$header.= "</div>";


			if ($nlinhas>0)
			{

				$body = "<div style=\"margin: 0px 0px 0px 0px; padding: 0px 0px 0px 0px ; overflow-y:auto;height:$heightTBody\" >";

				$body.= '<table width="'. $largura . '" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';

				//Monta Listagem
				$totais = array();
				$tipovl = array();
				for ($i=($numero-1);$i<$reg_fim;$i++)
				{
					$c = 0;
					if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
					$body.= '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';
					foreach($RS[$i] as $k=>$v) {
						// Clausula que faz soma numeros monetários
						if (!is_numeric($v)) {
							if (is_numeric(str_replace(array(".", ","),array("", "."),$v))) {
								$v = str_replace(array(".", ","),array("", "."),$v);
								$valormonetario="S";
							}
						}
						// fim - Clausula que faz soma numeros monetários
						if (is_numeric($v))
						{
							//cria o array totalizador
							if (!$totais['0'.$c]) {$coluna = array('0'.$c => $v); $totais = array_merge($totais, $coluna);} else $totais['0'.$c] = $totais['0'.$c] + $v;
							//Mostra o resultado
							if (strpos($v,'.')) {$v = number_format($v, 2, ',', '.'); if (!$tipovl['0'.$c]) {$coluna = array('0'.$c => 'vl'); $tipovl = array_merge($totais, $coluna);} else $tipovl['0'.$c] = 'vl';}
							if ($v<0) $body.= '<td width='. $arrHeighTds[$c] .'% align="right" valign="top" style="color:#cc0000;" title="'.simec_strip_tags($cabecalho[$c]).'">('.$v.')'; else $body.= '<td width='. $arrHeighTds[$c] .'% align="right" valign="top" style="color:#999999;" title="'.simec_strip_tags($cabecalho[$c]).'">'.$v;
							$body.= ('<br>'.$totais[$c]);
						}
						elseif( strpos(simec_trim($v),"<tr") > 0){
							$body.= $v;
						}else{
							$body.= '<td width='. $arrHeighTds[$c] .'% valign="top" title="'.simec_strip_tags($cabecalho[$c]).'">'.$v;
						}
						$body.= '</td>';
						$c = $c + 1;
					}
					$body.= '</tr>';
				}

		        $body.= '</tbody>';
		        $body.= '</table>';


				$body.= "</div>";


			}
			else
			{

				$body = "<div>";

				$body.= '<table width="'. $largura . '" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';

				$body.= "<tr>";

				$body.= '<td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td>';

				$body.= "</tr>";

				$body.= "<div>";


			}

			//inicio totais
			$totaisN = "<div>";
			$totaisN.= '<table width="'. $largura . '" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';
			$somarCampos = $soma!='S' && is_array($soma) && (@count($soma)>0);
			if ($soma=='S' || $somarCampos){
				//totaliza (imprime totais dos campos numericos)
				$totaisN.= '<tr bgcolor=#DEDEDE >';
				for ($i=0;$i<$c;$i++)
				{

					$totaisN.= '<td width='. $arrHeighTds[$i] .'% align="right" title="'.simec_strip_tags($cabecalho[$i]).'">';
					if ($i==0) $totaisN .= 'Totais:   ';
					if(($somarCampos && $soma[$i]) || $soma=='S') {
						if (is_numeric($totais['0'.$i]))
							if($valormonetario == 'S'){
								$totaisN.= "<b>".number_format($totais['0'.$i], 2, ',', '.')."</b>";
							}else{
								$totaisN.= "<b>".number_format( $totais['0'.$i], 0, ',', '.')."</b>";
							}
						else
							$totaisN.= $totais['0'.$i];
					}
					$totaisN.= '</td>';

				}
				$totaisN.= '<td><div style="width:10px" ></div></td>';
				$totaisN.= '</tr>';
			}
			$totaisN.= "</table>";
			$totaisN.= "</div>";
				//fim totais

			echo $header;
			echo $body;
			echo $totaisN;
			if( $boImprimiTotal && $totalRegistro != 0 )
				print '<div>
						<table width="'.$largura.'" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
							<tr bgcolor="#ffffff">
								<td><b>Total de Registros: ' . $totalRegistro . '</b></td>
							<tr>
						</table>
						</div>';

		}else{

			print '<table width="'. $largura . '" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';
			if ($nlinhas>0)
			{

				//Monta Cabeçalho
				if(is_array($cabecalho))
				{
					print '<thead><tr>';
					for ($i=0;$i<count($cabecalho);$i++)
					{
						print '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">'.$cabecalho[$i].(($i===$totalregistro)?' (Total:'.count($RS).')':'').'</label>';
					}
					print '</tr> </thead>';
				}

		        echo '<tbody>';

				//Monta Listagem
				$totais = array();
				$tipovl = array();

				for ($i=($numero-1);$i<$reg_fim;$i++)
				{
					$c = 0;
					if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
					print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';
					foreach($RS[$i] as $k=>$v) {
						// Clausula que faz soma numeros monetários
						if (!is_numeric($v)) {
							if (is_numeric(str_replace(array(".", ","),array("", "."),$v))) {
								$v = str_replace(array(".", ","),array("", "."),$v);
								$valormonetario="S";
							}
						}
						// fim - Clausula que faz soma numeros monetários
						if (is_numeric($v))
						{
							//cria o array totalizador
							if (!$totais['0'.$c]) {$coluna = array('0'.$c => $v); $totais = array_merge($totais, $coluna);} else $totais['0'.$c] = $totais['0'.$c] + $v;
							//Mostra o resultado
							if (strpos($v,'.')) {$v = number_format($v, 2, ',', '.'); if (!$tipovl['0'.$c]) {$coluna = array('0'.$c => 'vl'); $tipovl = array_merge($totais, $coluna);} else $tipovl['0'.$c] = 'vl';}
							if ($v<0) print '<td align="right" valign="top" style="color:#cc0000;" title="'.simec_strip_tags($cabecalho[$c]).'">('.$v.')'; else print '<td align="right" valign="top" style="color:#999999;" title="'.simec_strip_tags($cabecalho[$c]).'">'.$v;
							print ('<br>'.$totais[$c]);
						}
						elseif( strpos(simec_trim($v),"<tr") > 0){
							print $v;
						}else print '<td valign="top" title="'.simec_strip_tags($cabecalho[$c]).'">'.$v;
						print '</td>';
						$c = $c + 1;
					}
					print '</tr>';
				}

		        print '</tbody>';

				$somarCampos = $soma!='S' && is_array($soma) && (@count($soma)>0);
				if ($soma=='S' || $somarCampos){
					//totaliza (imprime totais dos campos numericos)
					print '<tfoot><tr>';
					for ($i=0;$i<$c;$i++)
					{
						print '<td align="right" title="'.simec_strip_tags($cabecalho[$i]).'">';
						if ($i==0) print 'Totais:   ';
						if(($somarCampos && $soma[$i]) || $soma=='S') {
							if (is_numeric($totais['0'.$i]))
								if($valormonetario == 'S'){
									print number_format($totais['0'.$i], 2, ',', '.');
								}else{
									print number_format( $totais['0'.$i], 0, ',', '.');
								}
							else
								print $totais['0'.$i];
						}
						print '</td>';
					}
					print '</tr></tfoot>';
					//fim totais
				}
			}
			else {
				print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
			}
			print '</table>';

			if( $boImprimiTotal && $totalRegistro != 0 )
				print '<div>
					<table width="'.$largura.'" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
						<tr bgcolor="#ffffff">
							<td><b>Total de Registros: ' . $totalRegistro . '</b></td>
						<tr>
					</table>
					</div>';
		}
	}

	/**
	 * Monta uma lista com sumário a partir de um array multidimensional
	 *
	 */
	function monta_lista_agrupado($arrRsAgrupado, $cabecalho="",$soma='',$largura='95%') {
		if(!(bool)$largura) $largura = '95%';

		$somarCampos = $soma!='' && is_array($soma) && (@count($soma)>0);
		$nlinhas = $arrRsAgrupado ? count($arrRsAgrupado) : 0;

		print '<table width="'. $largura . '" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';
		if ($nlinhas>0)
		{
			// Monta Cabeçalho
			if(is_array($cabecalho))
			{
				print '<thead><tr>';
				foreach($cabecalho as $cab) {
					print '<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">'.$cab."</td>";
				}
				print '</tr></thead>';
			}

			// Monta Listagem
			$totais = array();
			foreach($arrRsAgrupado as $grupo=>$dados) {
				foreach ($dados as $pos=>$linha) {
					$marcado = $pos % 2 ? '' : '#F7F7F7';
					echo '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';
					$i=0;
					foreach($linha as $col=>$val) {
						if(!isset($totais[$grupo][$col]))
						$totais[$grupo][$col] = 0;

						if (!is_numeric($val)) {
							print '<td title="'.simec_strip_tags($cabecalho[$i]).'">'.$val.'</td>';
						}
						else {
							$totais[$grupo][$col] += $val;
							$valTela = $somarCampos && $soma[$i] ? number_format($val, 2, ',', '.') : $val;

							if ($val<0) {
								echo '<td align="right" style="color:#cc0000;" title="'.simec_strip_tags($cabecalho[$i]).'">(' . $valTela . ')</td>';
							}
							else {
								echo '<td align="right" style="color:#999999;" title="'.simec_strip_tags($cabecalho[$i]).'">'. $valTela .'</td>';
							}
						}
						$i++;
					}
					echo "</tr>\n";
				}
				//
				// Subtotal
				if ($somarCampos) {
					print '<thead><tr>';
					$i=0;
					foreach ($totais[$grupo] as $col=>$total) {
						print '<td align="right" title="'.simec_strip_tags($cabecalho[$i]).'">';
						if ($i==0) print 'Subtotal:   ';
						if ($somarCampos && $soma[$i]) {
							if (is_numeric($total))
							print number_format($total, 2, ',', '.');
							else
							print $total;
						}
						else {
							print " ";
						}
						print '</td>';
						$i++;
					}
					print '</tr></thead>';
				}
			}
			//
			// Total geral
			if ($somarCampos) {
				$totalGeral = array();
				foreach ($totais as $grupo=>$colunas) {
					foreach ($colunas as $col=>$total) {
						$totalGeral[$col] += $total;
					}
				}

				$i=0;
				print '<tr><td colspan="' . count($cabecalho) . '"> </td></tr>';
				print '<thead><tr>';

				foreach($totalGeral as $col=>$total) {
					print '<td align="right" title="'.simec_strip_tags($cabecalho[$i]).'">';
					if ($i==0) print '<b>Total geral:</b>   ';
					if ($somarCampos && $soma[$i]) {
						if (is_numeric($total))
						print number_format($total, 2, ',', '.');
						else
						print $total;
					}
					else {
						print " ";
					}
					print '</td>';
					$i++;
				}
				print '</tr></thead>';
			}
		}
		else {
			print '<tr><td align="center" style="color:#cc0000;">não foram encontrados registros.</td></tr>';
		}
		print '</table>';
	}

	function busca_pai($cod,$i)
	{
		// busca, num plano de trabalho, se o registro está subordinado
		$sql = "select ptoid_pai from planotrabalho where ptostatus='A' and ptoid=".$cod;
		//print $sql;
		//exit();
		$RS = $this->record_set($sql);
		$res = $this->carrega_registro($RS,0);
		if ($res['ptoid_pai']<>'')
		return 2;
		else
		{
			// verifico se tem filho alm de no ter pai
			$sql = "select ptoid from planotrabalho where ptostatus='A' and ptoid_pai=".$cod;
			$RS = $this->record_set($sql);
			$nlinhas = $this->conta_linhas($RS);
			if ($nlinhas < 0)
			return 1;
			else return 0;
		}

	}

	function monta_lista($sql,$cabecalho="",$perpage,$pages,$soma,$alinha,$valormonetario="S",$nomeformulario="",$celWidth="",$celAlign="",$tempocache=null,$param=Array()) {
		// este mtodo monta uma listagem na tela baseado na sql passada (tem que estar fora de tags FORM'S)
		//$sql = Texto - sql que vai gerar a lista
		//$cabecaho = Vetor - contendo o nome que vai ser exibido, deve ter a mesma quantidade dos campos da sql
		//Parmetros de paginao
		//$perpage = Numrico - Registros por pgina
		//$pages = Numrico - Numrico - Mx de Paginas que sero mostradas na barrinha de paginao
		//$soma = Boleano - Mostra somatrio de campos numricos no fim da lista
		//$ordem = alinhamento dos títulos (left, rigth, center)
		//$valormonetario = Define se o valor a ser tratado é monetário (com casas decimais) ou numérico (sem casas decimais)
		//$nomeformulario = Caso seja necessário um formulário para acessar objetos dentro da tabela, informar o nome do formulário, que o componente irá criar um <form name="nomedoformularioinformado">...</form>
		//Registro Atual (instanciado na chamada)
		//$sql = str_replace(";", "", $sql);

		/**************************************************************************************************
		 -> NOVOS PARAMETROS (programador: Felipe Carvalho / data: 24/07/2009)
		 $celWidth => array com o width(tamanho) de cada célula. Pode-se usar qualquer unidade (px, %, pt, etc)
		 $celAlign => array com o alinhamento de cada célula.

		 NOS DOIS PARAMETROS O TAMANHO DO ARRAY DEVE SER O MESMO DO
		 NUMERO DE ELEMENTOS QUE IRAO COMPOR A LISTA.

		 PARAMETROS NAO OBRIGATORIOS!
		 SE NAO FOREM INFORMADOS USA-SE O PADRAO DO COMPONENTE.

		 EXEMPLO:
		 	$cabecalho 		= array( "Comando", "Ação"); ou Array('comando',Array('label'=>'parte de cima','colunas'=>Array('subcoluna1','subcoluna2')))
			$tamanho		= array( '10%', '90%' );
			$alinhamento	= array( 'center', 'left' );
			$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '', $tamanho, $alinhamento);
		**************************************************************************************************/
		/**************************************************************************************************
		 -> NOVOS PARAMETROS (programador: Eduardo Dunice / data: 08/09/2012)
		 $param => array que comporta vários parametros extras.

		 	Opções suportadas:
		 	- ordena 	   -> ativa e desativa algoritmo de ordenação do monta lista (contorna bug com SQL q possui subselect com clausula ORDER BY).
		 	- totalLinhas  -> ativa e desativa rodapé com total de linhas listadas.
			- managerOrder -> deve conter os parâmetros de ordenamento usando como índice o número correspondente a coluna (iniciando de 1)
			- classTable -> deve conter os parâmetros do tipo (string) para adiciona classes css adicionais para a tabela gerada

		 PARAMETROS NAO OBRIGATORIOS!

		 EXEMPLO:
		 	$param['ordena'] 	   = (true ou false);
		 	$param['totalLinhas']  = (true ou false);
		 	$param['managerOrder'] = array(12 => 'o.obrnome',
												15 => 'oc.ocrdtinicioexecucao',
												16 => 'oc.ocrdtterminoexecucao',
												18 => array('campo' => "DATE_PART('days', NOW() - o.obrdtultvistoria)", 'alias' => "obrdtultvistoria"),
												19 => 'obrdtultvistoria'
                        $param['classTable'] = 'lista listagem listaClica'
											   );
			$db->monta_lista($sql, $cabecalho, 50, 20, '', '100%', '', '', '', '', '', $param);
		**************************************************************************************************/

		$ordena = isset($param['ordena']) ? $param['ordena'] : true;
		$param['totalLinhas'] = isset($param['totalLinhas']) && is_bool($param['totalLinhas']) ? $param['totalLinhas'] : true;

		if ($_REQUEST['numero']=='') $numero = 1; else $numero = intval($_REQUEST['numero']);
		//Controla o Order by
		if (!is_array($sql) && $_REQUEST['ordemlista']<>'' && $ordena)
		{
			if ($_REQUEST['ordemlistadir'] <> 'DESC') {
				$ordemlistadir  = 'ASC';
				$ordemlistadir2 = 'DESC';
			} else {
				$ordemlistadir  = 'DESC';
				$ordemlistadir2 = 'ASC';
			}

                        /**
                         * Bug no ordernar colunas no projeto Obras2.0, quando existe subselect com order by dentro da consulta
                         * alterado de "strpos" para "strrpos" para pegar sempre o último order by
                         */
			$subsql 	= substr($sql,0,strrpos(simec_trim(strtoupper($sql)),'ORDER '));

			if ( !empty($param['managerOrder'][$_REQUEST['ordemlista']]) ){
				if ( is_array( $param['managerOrder'][$_REQUEST['ordemlista']] ) ){
					$campoOrder = $param['managerOrder'][$_REQUEST['ordemlista']]['campo'];
					$aliasOrder = $param['managerOrder'][$_REQUEST['ordemlista']]['alias'];
				} else {
					$campoOrder = $param['managerOrder'][$_REQUEST['ordemlista']];
					$aliasOrder = $param['managerOrder'][$_REQUEST['ordemlista']];
				}
				$ordemLista = $aliasOrder . '_oculto';

				$strReplace = 'SELECT';
				if ( strpos( simec_trim(strtoupper($subsql)), 'DISTINCT' ) ){
					$posIni 	    = strpos( simec_trim(strtoupper($subsql)), 'SELECT' ) + 6;
					$posFim 	    = strpos( simec_trim(strtoupper($subsql)), 'DISTINCT' ) - 6;
					$espacoDistinct = simec_trim(substr($subsql, $posIni, $posFim) );

					if ( empty($espacoDistinct) ){
						$strReplace = 'DISTINCT';
					}
				}
				$subsql = preg_replace('/' . $strReplace . '/i',
									   $strReplace . " " . $campoOrder . ' AS ' . $aliasOrder . '_oculto, ',
									   $subsql,
									   1);
			}else{
				$ordemLista = $_REQUEST['ordemlista'];
			}

			$sql 		= (!$subsql ? $sql : $subsql).' order by '.$ordemLista.' '.$ordemlistadir;
		}

	    if (is_array($sql)){
	        $RS = $sql;
	        $totalRegistro = count($RS);
		}else{
			$sql = simec_trim($sql);
			$char = substr($sql, -1);
			if ($char == ";") $sql = substr($sql, 0, -1);

			$sqlCount = "select
							count(1)
						 from (" . $sql . ") rs";
//$totalRegistro = 2;
			$totalRegistro = $this->pegaUm($sqlCount,0,$tempocache);
//ver($sqlCount,d);
//			$totalRegistro = $this->pegaUm($sqlCount);
//ver('ok', d);
			$sql = $sql . " LIMIT {$perpage} offset ".($numero-1);

			$RS = $this->carregar($sql,null,$tempocache);
	    }
		$nlinhas = count($RS);

		if (! $RS) $nl = 0; else $nl=$nlinhas;
	//	if (($numero+$perpage)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $numero+$perpage-1;
		$reg_fim = $nlinhas;
		if ($nl>0)
		{
			$total_reg = $totalRegistro;

			//monta o formulario da lista mantendo os parametros atuais da pgina
			print '<form name="formlista" method="post"><input type="Hidden" name="numero" value="" /><input type="Hidden" name="ordemlista" value="'.$_REQUEST['ordemlista'].'"/><input type="Hidden" name="ordemlistadir" value="'.$ordemlistadir.'"/>';

			foreach($_POST as $k=>$v){
				if ($k<>'ordemlista' and $k<>'ordemlistadir' and $k<>'numero'){
					if( is_array($v)){
						foreach($v as $k2 => $v2){
							if( is_array($v2)){
								foreach($v2 as $k3 => $v3){
									print '<input type="Hidden" name="'.$k.'['.$k2.']['.$k3.']" value="'.$v3.'"/>';
								}
							}else{
								print '<input type="Hidden" name="'.$k.'['.$k2.']" value="'.$v2.'"/>';
							}
						}
					}else{
						print '<input type="Hidden" name="'.$k.'" value="'.$v.'"/>';
					}
				}
			}

			print '</form>';

			if($nomeformulario != "")
				print '<form name="'.$nomeformulario.'" id="'.$nomeformulario.'" action="" enctype="multipart/form-data" method="post">';

                        $classTable = (array_key_exists('classTable', $param)) ? $param['classTable'] : '';
			print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem '.$classTable.'">';
			//Monta Cabealho
			if ( $cabecalho === null ) {

			}else if(is_array($cabecalho))
			{
				foreach($cabecalho as $cab){
					if(is_array($cab)){
						$cab_rowspan = " rowspan='2' ";
					}
				}

				print '<thead><tr>';
				for ($i=0;$i<count($cabecalho);$i++)
				{
					if(!is_array($cabecalho[$i])){
						if ($_REQUEST['ordemlista'] == ($i+1)) {
							$ordemlistadirnova = $ordemlistadir2;
							$imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
						} else {
							$ordemlistadirnova = 'ASC';
							$imgordem = '';
						}
						$arrColunasComuns[] = 1;
						$onclick = $ordena ? 'onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');"' : '';
						print '<td '.$cab_rowspan.' align="' . $alinha . '" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" '.$onclick.' title="Ordenar por '.simec_strip_tags($cabecalho[$i]).'">'.$imgordem.'<strong>'.$cabecalho[$i].'</strong></label>';
					}else{
						print '<td colspan="'.count($cabecalho[$i]['colunas']).'"  align="' . $alinha . '" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" ><center><strong>'.$cabecalho[$i]['label'].'</strong></center></label>';
						foreach($cabecalho[$i]['colunas'] as $col_b){
							$arrColunasDebaixo[] = $col_b;
						}
					}
				}
				if($arrColunasDebaixo){
					print "<tr>";
					$i = count($arrColunasComuns);
					foreach($arrColunasDebaixo as $colunaDeBaixo){
						if ($_REQUEST['ordemlista'] == $i+1) {
							$ordemlistadirnova = $ordemlistadir2;
							$imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
						} else {
							$ordemlistadirnova = 'ASC';
							$imgordem = '';
						}
						print '<td align="' . $alinha . '" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.simec_strip_tags($colunaDeBaixo).'">'.$imgordem.'<strong>'.$colunaDeBaixo.'</strong></label>';
						$i++;
					}
				}
				print '</tr> </thead>';
			}
			else
			{
				print '<thead><tr>'; $i=0;
				foreach($RS[0] as $k=>$v)
				{
					if ($_REQUEST['ordemlista'] == ($i+1)) {
						$ordemlistadirnova = $ordemlistadir2;
						$imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
					} else {
						$ordemlistadirnova = 'ASC';
						$imgordem = '';}
						print '<td valign="top" class="title" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.simec_strip_tags($k).'">'.$imgordem.'<strong>'.$k.'</strong></label>';
						$i=$i+1;}
						print '</tr> </thead>';
			}
			//Monta Listagem
			$totais = array();
			$tipovl = array();
	//		for ($i=($numero-1);$i<$reg_fim;$i++)

			for ($i=0; $i < $reg_fim; $i++)
			{
				$c = 0;
				if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
				print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';

				// contador -> numero de celulas
				$numCel = 0;
				foreach($RS[$i] as $k=>$v) {
					if ( strpos($k, '_oculto') ){
						continue;
					}

					// Setando o alinhamento da célula usando o array $celAlign.
					// Se não for passado o parâmetro, usa o padrão do componente.
					if(is_array($celAlign)) {
						$alignNumeric = $alignNotNumeric = $celAlign[$numCel];
					} else {
						$alignNumeric 		= 'right';
						$alignNotNumeric	= 'left';
					}
					// Setando o tamanho da célula usando o array $celWidth.
					// Se não for passado o parâmetro, usa o padrão do componente.
					$width = (is_array($celWidth)) ? 'width="'.$celWidth[$numCel].'"' : '';

					if (is_numeric($v))
					{
						//cria o array totalizador
						if (!$totais['0'.$c]) {$coluna = array('0'.$c => $v); $totais = array_merge($totais, $coluna);} else $totais['0'.$c] = $totais['0'.$c] + $v;
						//Mostra o resultado
						if (strpos($v,'.')||$valormonetario == 'S') {$v = number_format($v, 2, ',', '.');
						if (!$tipovl['0'.$c]) {$coluna = array('0'.$c => 'vl'); $tipovl = array_merge($totais, $coluna);} else $tipovl['0'.$c] = 'vl';}
						if ($v<0) print '<td align="'.$alignNumeric.'" '.$width.' style="color:#cc0000;" title="'.simec_strip_tags($cabecalho[$c]).'">('.$v.')'; else print '<td align="'.$alignNumeric.'" '.$width.' style="color:#0066cc;" title="'.simec_strip_tags($cabecalho[$c]).'">'.$v;
						print ('<br>'.$totais[$c]);
					}
					else {
						print '<td align="'.$alignNotNumeric.'" '.$width.' title="'.simec_strip_tags($cabecalho[$c]).'">'.$v;
					}
					print '</td>';
					$c = $c + 1;
					$numCel++;
				}
				print '</tr>';
			}

			if ($soma=='S'){
				//totaliza (imprime totais dos campos numericos)
				print '<thead><tr>';
				for ($i=0;$i<$c;$i++)
				{
					print '<td align="right" title="'.simec_strip_tags($cabecalho[$i]).'">';

					if ($i==0) print 'Totais:   ';
					if (is_numeric($totais['0'.$i])) {

						if($valormonetario == 'S'){
							print number_format($totais['0'.$i], 2, ',', '.');
						}else{
							print $totais['0'.$i];
						}
					}
						else print $totais['0'.$i];
					print '</td>';
				}
				print '</tr>';
				//fim totais
			}

			print '</table>';

			if($nomeformulario != "")
				print '</form>';
			if( $param['totalLinhas'] ){
				print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem"><tr bgcolor="#ffffff"><td><b>Total de Registros: ' . $totalRegistro . '</b></td><td>';
                print '</td></tr></table>';
			}

			include APPRAIZ."includes/paginacao.inc";

			print '<script language="JavaScript">function ordena(ordem, direcao) {document.formlista.ordemlista.value=ordem;document.formlista.ordemlistadir.value=direcao;document.formlista.submit();} function pagina(numero) {document.formlista.numero.value=numero;document.formlista.submit();}</script>';
		}
		else
		{
			print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
			print '</table>';
		}
	}

	// SEM BUG ORBER BY
	// Para que funcione, coloque um GROUP BY em sua SQL
	function monta_lista_ordenaGROUPBY($sql,$cabecalho="",$perpage,$pages,$soma,$alinha,$valormonetario="S",$nomeformulario="",$celWidth="",$celAlign="",$tempocache="") {
		// este mtodo monta uma listagem na tela baseado na sql passada (tem que estar fora de tags FORM'S)
		//$sql = Texto - sql que vai gerar a lista
		//$cabecaho = Vetor - contendo o nome que vai ser exibido, deve ter a mesma quantidade dos campos da sql
		//Parmetros de paginao
		//$perpage = Numrico - Registros por pgina
		//$pages = Numrico - Numrico - Mx de Paginas que sero mostradas na barrinha de paginao
		// $soma = Boleano - Mostra somatrio de campos numricos no fim da lista
		// $ordem = alinhamento dos títulos (left, rigth, center)
		// $valormonetario = Define se o valor a ser tratado é monetário (com casas decimais) ou numérico (sem casas decimais)
		//$nomeformulario = Caso seja necessário um formulário para acessar objetos dentro da tabela, informar o nome do formulário, que o componente irá criar um <form name="nomedoformularioinformado">...</form>
		//Registro Atual (instanciado na chamada)
		//$sql = str_replace(";", "", $sql);

		/**************************************************************************************************
		 -> NOVOS PARAMETROS (programador: Felipe Carvalho / data: 24/07/2009)
		 $celWidth => array com o width(tamanho) de cada célula. Pode-se usar qualquer unidade (px, %, pt, etc)
		 $celAlign => array com o alinhamento de cada célula.

		 NOS DOIS PARAMETROS O TAMANHO DO ARRAY DEVE SER O MESMO DO
		 NUMERO DE ELEMENTOS QUE IRAO COMPOR A LISTA.

		 PARAMETROS NAO OBRIGATORIOS!
		 SE NAO FOREM INFORMADOS USA-SE O PADRAO DO COMPONENTE.

		 EXEMPLO:
		 	$cabecalho 		= array( "Comando", "Ação");
			$tamanho		= array( '10%', '90%' );
			$alinhamento	= array( 'center', 'left' );
			$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '', $tamanho, $alinhamento);
		**************************************************************************************************/

		if ($_REQUEST['numero']=='') $numero = 1; else $numero = intval($_REQUEST['numero']);
		//Controla o Order by
		if (!is_array($sql) && $_REQUEST['ordemlista']<>'')
		{
			if ($_REQUEST['ordemlistadir'] <> 'DESC') {$ordemlistadir = 'ASC';$ordemlistadir2 = 'DESC';} else {$ordemlistadir = 'DESC'; $ordemlistadir2 = 'ASC';}
			$qtd = strlen($sql);
			$achou = 0;
			for($x=1;$achou!=1;$x++){
				$pos     = strpos(substr($sql,($qtd-(10*$x)),$qtd-($qtd-(10*$x))),'ORDER ');
				$posFrom = strpos(substr($sql,($qtd-(10*$x)),$qtd-($qtd-(10*$x))),'GROUP BY ');
				if($pos){
					$pos = ($qtd-(10*$x)) + $pos;
					$achou = 1;
				}
				if($posFrom){
					$achou = 1;
				}
			}
			if($pos){
				$subsql = substr($sql,0,$pos);
			}else{
				$subsql = false;
			}
			$sql = (!$subsql ? $sql : $subsql).' order by '.$_REQUEST['ordemlista'].' '.$ordemlistadir;
		}

	    if (is_array($sql)){
	        $RS = $sql;
		}else{
			$sql = simec_trim($sql);
			$char = substr($sql, -1);
			if ($char == ";") $sql = substr($sql, 0, -1);

            $temporaryTable = "tmp_sql" . $_SESSION['usucpforigem'] . date('YmsHis');

            // OBS: Não deixar o $sql na memsa linha dos parênteses, pois pode haver comentário de uma linha que quebre o fechamento
            $sqlTmp = "CREATE temporary TABLE $temporaryTable  as (
                                                                    $sql
                                                                  )";
            $this->executar($sqlTmp);

            $sql = "select * from $temporaryTable";

			$sqlCount = "select
							count(1)
						 from (" . $sql . ") rs";

			$totalRegistro = $this->pegaUm($sqlCount, 0, $tempocache);
				$sql = $sql . " LIMIT {$perpage} offset ".($numero-1);

			$RS = $this->carregar($sql, null, $tempocache);
	    }
		$nlinhas = count($RS);

		if (! $RS) $nl = 0; else $nl=$nlinhas;
	//	if (($numero+$perpage)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $numero+$perpage-1;
		$reg_fim = $nlinhas;
		if ($nl>0)
		{
			$total_reg = $totalRegistro;

			//monta o formulario da lista mantendo os parametros atuais da pgina
			print '<form name="formlista" method="post"><input type="Hidden" name="numero" value="" /><input type="Hidden" name="ordemlista" value="'.$_REQUEST['ordemlista'].'"/><input type="Hidden" name="ordemlistadir" value="'.$ordemlistadir.'"/>';
			foreach($_POST as $k=>$v){if ($k<>'ordemlista' and $k<>'ordemlistadir' and $k<>'numero') print '<input type="Hidden" name="'.$k.'" value="'.$v.'"/>';}
			print '</form>';

			if($nomeformulario != "")
				print '<form name="'.$nomeformulario.'" id="'.$nomeformulario.'" action="" enctype="multipart/form-data" method="post">';

			print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			//Monta Cabealho
			if ( $cabecalho === null ) {

			}else if(is_array($cabecalho))
			{
				print '<thead><tr>';
				for ($i=0;$i<count($cabecalho);$i++)
				{
					if ($_REQUEST['ordemlista'] == ($i+1)) {
						$ordemlistadirnova = $ordemlistadir2;
						$imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
					} else {
						$ordemlistadirnova = 'ASC';
						$imgordem = '';
					}
					print '<td align="' . $alinha . '" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.simec_strip_tags($cabecalho[$i]).'">'.$imgordem.'<strong>'.$cabecalho[$i].'</strong></label>';
				}
				print '</tr> </thead>';
			}
			else
			{
				print '<thead><tr>'; $i=0;
				foreach($RS[0] as $k=>$v)
				{
					if ($_REQUEST['ordemlista'] == ($i+1)) {
						$ordemlistadirnova = $ordemlistadir2;
						$imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
					} else {
						$ordemlistadirnova = 'ASC';
						$imgordem = '';}
						print '<td valign="top" class="title" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.simec_strip_tags($k).'">'.$imgordem.'<strong>'.$k.'</strong></label>';
						$i=$i+1;}
						print '</tr> </thead>';
			}
			//Monta Listagem
			$totais = array();
			$tipovl = array();
	//		for ($i=($numero-1);$i<$reg_fim;$i++)

			for ($i=0; $i < $reg_fim; $i++)
			{
				$c = 0;
				if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
				print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';

				// contador -> numero de celulas
				$numCel = 0;
				foreach($RS[$i] as $k=>$v) {
					// Setando o alinhamento da célula usando o array $celAlign.
					// Se não for passado o parâmetro, usa o padrão do componente.
					if(is_array($celAlign)) {
						$alignNumeric = $alignNotNumeric = $celAlign[$numCel];
					} else {
						$alignNumeric 		= 'right';
						$alignNotNumeric	= 'left';
					}
					// Setando o tamanho da célula usando o array $celWidth.
					// Se não for passado o parâmetro, usa o padrão do componente.
					$width = (is_array($celWidth)) ? 'width="'.$celWidth[$numCel].'"' : '';

					if (is_numeric($v))
					{
						//cria o array totalizador
						if (!$totais['0'.$c]) {$coluna = array('0'.$c => $v); $totais = array_merge($totais, $coluna);} else $totais['0'.$c] = $totais['0'.$c] + $v;
						//Mostra o resultado
						if (strpos($v,'.')) {$v = number_format($v, 2, ',', '.'); if (!$tipovl['0'.$c]) {$coluna = array('0'.$c => 'vl'); $tipovl = array_merge($totais, $coluna);} else $tipovl['0'.$c] = 'vl';}
						if ($v<0) print '<td align="'.$alignNumeric.'" '.$width.' style="color:#cc0000;" title="'.simec_strip_tags($cabecalho[$c]).'">('.$v.')'; else print '<td align="'.$alignNumeric.'" '.$width.' style="color:#0066cc;" title="'.simec_strip_tags($cabecalho[$c]).'">'.$v;
						print ('<br>'.$totais[$c]);
					}
					else {
						print '<td align="'.$alignNotNumeric.'" '.$width.' title="'.simec_strip_tags($cabecalho[$c]).'">'.$v;
					}
					print '</td>';
					$c = $c + 1;
					$numCel++;
				}
				print '</tr>';
			}

			if ($soma=='S'){
				//totaliza (imprime totais dos campos numericos)
				print '<thead><tr>';
				for ($i=0;$i<$c;$i++)
				{
					print '<td align="right" title="'.simec_strip_tags($cabecalho[$i]).'">';

					if ($i==0) print 'Totais:   ';
					if (is_numeric($totais['0'.$i])) {

					if($valormonetario == 'S'){
								print number_format($totais['0'.$i], 2, ',', '.');
							}else{
								print $totais['0'.$i];
							}
					}
						else print $totais['0'.$i];
					print '</td>';
				}
				print '</tr>';
				//fim totais
			}

			print '</table>';

			if($nomeformulario != "")
				print '</form>';

			print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem"><tr bgcolor="#ffffff"><td><b>Total de Registros: ' . $totalRegistro . '</b></td><td>';

			include APPRAIZ."includes/paginacao.inc";
			print '</td></tr></table>';
			print '<script language="JavaScript">function ordena(ordem, direcao) {document.formlista.ordemlista.value=ordem;document.formlista.ordemlistadir.value=direcao;document.formlista.submit();} function pagina(numero) {document.formlista.numero.value=numero;document.formlista.submit();}</script>';
		}
		else
		{
			print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
			print '</table>';
		}
	}

	function mudaPosicao($id1, $id2, $tabela, $atributoOrdem, $id){

		if(!$id1 || !$id2){
			return false;
		}else{
			$sql = "SELECT $atributoOrdem
					  FROM $tabela
					WHERE
					  $id = $id1";
			$ordem1 = $this->pegaUm($sql);

			$sql = "SELECT $atributoOrdem
					  FROM $tabela
					WHERE
					  $id = $id2";
			$ordem2 = $this->pegaUm($sql);

			$sql = "update
						$tabela
						set
					$atributoOrdem = $ordem2
					where
						$id = $id1";

			$this->executar($sql);

			$sql = "update
						$tabela
						set
					$atributoOrdem = $ordem1
					where
						$id = $id2";
			$this->executar($sql);

			$this->commit();

			return true;

		}
	}

	/*
	 * função responsavel pela geração de lista agrupada
	 *
	 *
	*/

	/*	 este mtodo monta uma listagem na tela baseado na sql passada (tem que estar fora de tags FORM'S)
	 *	$sql = Texto - sql que vai gerar a lista
	 *	$cabecaho = Vetor - contendo o nome que vai ser exibido, deve ter a mesma quantidade dos campos da sql
	 *	Parmetros de paginao
	 *	$perpage = Numrico - Registros por pgina
	 *	$pages = Numrico - Numrico - Mx de Paginas que sero mostradas na barrinha de paginao
	 *	 $soma = Boleano - Mostra somatrio de campos numricos no fim da lista
	 *	 $par2 = Reservado para o futuro
	 *	$nomeformulario = Caso seja necessário um formulário para acessar objetos dentro da tabela, informar o nome do formulário, que o componente irá criar um <form name="nomedoformularioinformado">...</form>
	 *	Registro Atual (instanciado na chamada)
	 *	$sql = str_replace(";", "", $sql);
	 *
	 *	$nomeformulario = nome do formulario.
	 *	$grupo = nome da coluna que realizará o agrupamento
	 *	$ordem = neste parametro e necessário passar um array: $ordem = array("nomeDoEsquena.nomeDaTabela", "nomeDaColunaOrdem", "nomeDoId")
	 *
	 *  segue exemplo abaixo para utilização desse parametro
	 *
	 *	Obs.: para utilizar este componente e necessário utilizar alias na query,
	 * 		 exemplo de query para um bom funcionamento:
	 * 			 |------------|
	 * 			 |  tblTeste  |
	 * 			 |------------|
	 * 			 | id		  |
	 * 			 | nome		  |
	 * 			 | sexo		  |
	 * 			 | ordem	  |
	 * 			 |------------|
	 *
	 * 			* select t.id, t.nome, t.sexo, t.id as ordem from teste t
	 * 			   order by t.ordem
	 *
	 *    		* caso seja agrupado e so passar o nome da coluna que deseja realizar o agrupamento.
	 */

	function monta_lista_grupo($sql,$cabecalho="",$perpage,$pages,$soma,$alinha,$par2,$nomeformulario="", $grupo="", $ordem=array(), $mostraGrupo = false ) {
		if($_REQUEST['ordemId1'] && $_REQUEST['ordemId2'] && $_REQUEST['ordemTable'] && $_REQUEST['ordemAtributo'] && $_REQUEST['ordemId']){
			$this->mudaPosicao($_REQUEST['ordemId1'], $_REQUEST['ordemId2'], $_REQUEST['ordemTable'], $_REQUEST['ordemAtributo'], $_REQUEST['ordemId']);
		}

		if ($_REQUEST['numero']=='')
			$numero = 1;
		else
			$numero = intval($_REQUEST['numero']);
		//Controla o Order by
		if (!is_array($sql) && $_REQUEST['ordemlista']<>''){
			if ($_REQUEST['ordemlistadir'] <> 'DESC') {
				$ordemlistadir = 'ASC';$ordemlistadir2 = 'DESC';
			}else{
				$ordemlistadir = 'DESC'; $ordemlistadir2 = 'ASC';
			}
			$subsql = substr($sql,0,strpos(simec_trim(strtoupper($sql)),'ORDER '));
			if($grupo){
				$sql = (!$subsql ? $sql : $subsql).' order by '.$grupo.', '.$_REQUEST['ordemlista'].' '.$ordemlistadir;
			}else{
				$sql = (!$subsql ? $sql : $subsql).' order by '.$_REQUEST['ordemlista'].' '.$ordemlistadir;
			}
		}

	    if (is_array($sql)){
	        $RS = $sql;
	        $totalRegistro = count($sql);
		}else{
			$sql = simec_trim($sql);
			$char = substr($sql, -1);
			if ($char == ";") $sql = substr($sql, 0, -1);

			$sqlCount = "select
							count(1)
						 from (" . $sql . ") rs";

			$totalRegistro = $this->pegaUm($sqlCount);

			$sql = $sql . " LIMIT {$perpage} offset ".($numero-1);

			$RS = $this->carregar($sql);
	    }
		$nlinhas = count($RS);

		if (! $RS)
			$nl = 0;
		else
			$nl=$nlinhas;
	//	if (($numero+$perpage)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $numero+$perpage-1;
		$reg_fim = $nlinhas;
		if ($nl>0)
		{
			$total_reg = $totalRegistro;

			//monta o formulario da lista mantendo os parametros atuais da pgina
			print '<form name="formlista" method="post">
					<input type="hidden" name="numero" value="'.$_REQUEST['numero'].'" />
					<input type="hidden" name="ordemlista" value="'.$_REQUEST['ordemlista'].'"/>
					<input type="hidden" name="ordemlistadir" value="'.$ordemlistadir.'"/>

					<input type="hidden" name="ordemId1" value=""/>
					<input type="hidden" name="ordemId2" value=""/>
					<input type="hidden" name="ordemTable" value=""/>
					<input type="hidden" name="ordemAtributo" value=""/>
					<input type="hidden" name="ordemId" value=""/>';

			foreach($_POST as $k=>$v){
				if ($k<>'ordemlista' and $k<>'ordemlistadir' and $k<>'numero' and $k<>'ordemId1' and $k<>'ordemId2' and $k<>'ordemTable' and $k<>'ordemAtributo' and $k<>'ordemId' )
					print '<input type="hidden" name="'.$k.'" value="'.$v.'"/>';
			}
			print '</form>';

			if($nomeformulario != "")
				print '<form name="'.$nomeformulario.'" id="'.$nomeformulario.'">';

			print '<table width="95%" id="tabelalista" name="tabelalista" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			//Monta Cabealho
			if ( $cabecalho === null ) {

			}else if(is_array($cabecalho))
			{
				print '<thead><tr>';
				for ($i=0;$i<count($cabecalho);$i++)
				{
					if ($_REQUEST['ordemlista'] == ($i+1)) {
						$ordemlistadirnova = $ordemlistadir2;
						$imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
					} else {
						$ordemlistadirnova = 'ASC';
						$imgordem = '';
					}
					print '<td align="' . $alinha . '" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.simec_strip_tags($cabecalho[$i]).'">'.$imgordem.'<strong>'.$cabecalho[$i].'</strong></label>';
				}
				print '</tr> </thead>';
			}
			else
			{
				print '<thead><tr>'; $i=0;
				foreach($RS[0] as $k=>$v)
				{
					if ($_REQUEST['ordemlista'] == ($i+1)) {
						$ordemlistadirnova = $ordemlistadir2;
						$imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
					} else {
						$ordemlistadirnova = 'ASC';
						$imgordem = '';}
						print '<td valign="top" class="title" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.simec_strip_tags($k).'">'.$imgordem.'<strong>'.$k.'</strong></label>';
						$i=$i+1;}
						print '</tr> </thead>';
			}
			//Monta Listagem
			$totais = array();
			$tipovl = array();
	//		for ($i=($numero-1);$i<$reg_fim;$i++)
			$w = 1;
			$contador = 0;
			$gruNomeAnt = "Nenhum Grupo";
			for ($i=0; $i < $reg_fim; $i++){
				$c = 0;
				if (fmod($i,2) == 0)
					$marcado = '';
				else
					$marcado='#F7F7F7';

				foreach($RS[$i] as $k=>$v) {

					if($k != $grupo){
						if($grupo){ //verifica se foi passado algum parametro para realizar o agrupamento
							if( ($RS[$i][$grupo] != $gruNomeAnt) ){
								$gruNomeAnt = $RS[$i][$grupo];
								$w = $i;
								$contador = 0;
								if($gruNomeAnt){
									print "<tr bgcolor=''>";
									print "		<td class=\"title\" style=\"background: #CCCCCC;\" colspan=\"".count($cabecalho)."\">" . ($mostraGrupo ? "<strong>Grupo:</strong>" : "" ) . " <i>".$RS[$i][$grupo]."</i></td>";
									print "</tr>";
									print "<tr bgcolor='".$marcado."' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='".$marcado."'\">";

								}else{
									print "<tr bgcolor=''>";
									print "		<td class=\"title\" style=\"background: #CCCCCC;\" colspan=\"".count($cabecalho)."\"><strong>Grupo:</strong> <i>Nenhum Grupo</i></td>";
									print "</tr>";
									print "<tr bgcolor='".$marcado."' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='".$marcado."'\">";
								}
							}
						}

						if( $i != $w  ){
							print "<tr bgcolor='".$marcado."' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='".$marcado."'\">";
							$w = $i;
						}
						if($grupo){ //verifica se foi passado algum parametro para realizar o agrupamento com ordenação
							if($ordem[0]){
								if($k == $ordem[1]){
									$contador++;
									$key = $i + 1;

									$grupoNome1 = $RS[$key][$grupo];

									print "<td style=\"text-align: center;\"><input type=\"hidden\" name=\"ordem_".$v."_ordem\" value=\"".$v."\" >";

									if( ($gruNomeAnt != $grupoNome1) || ($i == (count($RS) - 1) )){
										print "<img src=\"../imagens/seta_baixod.gif\" />";
									}else{
										print "<img onclick=\"mudaPosicao('tabelalista','baixo',this.parentNode.parentNode.rowIndex, '{$ordem[0]}', '{$ordem[1]}', '{$ordem[2]}');\" style=\"cursor: pointer;\" src=\"../imagens/seta_baixo.gif\" />";
									}

									if($contador == 1  ){
										print "<img src=\"../imagens/seta_cimad.gif\" />";
									}else {
										print "<img onclick=\"mudaPosicao('tabelalista','cima',this.parentNode.parentNode.rowIndex, '{$ordem[0]}', '{$ordem[1]}', '{$ordem[2]}');\" style=\"cursor: pointer;\" src=\"../imagens/seta_cima.gif\" />";
									}
									print "</td>";
									break;
								}
							}
						}else{
							if($ordem[0]){
								if($k == $ordem[1]){
									$contador++;

									$key = $i + 1;

									print "<td style=\"text-align: center;\"><input type=\"hidden\" name=\"ordem_".$v."_ordem\" value=\"".$v."\" >";

									if( $i == (count($RS) - 1) ){
										print "<img src=\"../imagens/seta_baixod.gif\" />";
									}else{
										print "<img onclick=\"mudaPosicao('tabelalista','baixo',this.parentNode.parentNode.rowIndex, '{$ordem[0]}', '{$ordem[1]}', '{$ordem[2]}');\" style=\"cursor: pointer;\" src=\"../imagens/seta_baixo.gif\" />";
									}

									if($contador == 1  ){
										print "<img src=\"../imagens/seta_cimad.gif\" />";
									}else {
										print "<img onclick=\"mudaPosicao('tabelalista','cima',this.parentNode.parentNode.rowIndex, '{$ordem[0]}', '{$ordem[1]}', '{$ordem[2]}');\" style=\"cursor: pointer;\" src=\"../imagens/seta_cima.gif\" />";
									}
									print "</td>";
									break;
								}
							}
						}
							if (is_numeric($v)){
								//cria o array totalizador

								if (!$totais['0'.$c]) {
									$coluna = array('0'.$c => $v);
									$totais = array_merge($totais, $coluna);
								}else
									$totais['0'.$c] = $totais['0'.$c] + $v;
								//Mostra o resultado
								if (strpos($v,'.')) {
									$v = number_format($v, 2, ',', '.');
									if (!$tipovl['0'.$c]) {
										$coluna = array('0'.$c => 'vl');
										$tipovl = array_merge($totais, $coluna);
									} else
										$tipovl['0'.$c] = 'vl';
								}
								if ($v<0)
									print '<td align="right" style="color:#cc0000;" title="'.simec_strip_tags($cabecalho[$c]).'">('.$v.')';
								else
									print '<td align="right" style="color:#0066cc;" title="'.simec_strip_tags($cabecalho[$c]).'">'.$v;
								print ('<br>'.$totais[$c]);
							}
							else{
								print '<td title="'.simec_strip_tags($cabecalho[$c]).'">'.$v;
							}
							print '</td>';
							$c = $c + 1;
					}
				}//if do grupo
				print '</tr>';
			}

			if ($soma=='S'){
				//totaliza (imprime totais dos campos numericos)
				if($ordem[0])
					$c++;
				print '<thead><tr>';
				for ($i=0;$i<$c;$i++){

					print '<td align="right" title="'.simec_strip_tags($cabecalho[$i]).'">';

					if ($i==0)
						print 'Totais:   ';
					if (is_numeric($totais['0'.$i]))
						print number_format($totais['0'.$i], 2, ',', '.');
					else
						print $totais['0'.$i];
					print '</td>';
				}
				print '</tr>';
				//fim totais
			}

			print '</table>';

			if($nomeformulario != "")
				print '</form>';

			print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem"><tr bgcolor="#ffffff"><td><b>Total de Registros: ' . $totalRegistro . '</b></td><td>';

			include APPRAIZ."includes/paginacao.inc";
			print '</td></tr></table>';
			print '<script language="JavaScript">function ordena(ordem, direcao) {document.formlista.ordemlista.value=ordem;document.formlista.ordemlistadir.value=direcao;document.formlista.submit();} function pagina(numero) {document.formlista.numero.value=numero;document.formlista.submit();}</script>';
		}
		else
		{
			print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
			print '</table>';
		}
	}

	function criarColunasDeOrdenacaoEmArray($arrayOriginal,$arrayDeTiposParaOrdenacao,$ordem) {
		$arrayInteger = array();
		$arrayDate = array();
		if($arrayDeTiposParaOrdenacao){
			foreach( $arrayOriginal as $linhaOriginal ) {
				foreach( $linhaOriginal as $chaveOriginal => $valorOriginal ) {
					foreach( $arrayDeTiposParaOrdenacao as $linhaParaOrdenacao ) {
						foreach( $linhaParaOrdenacao as $chaveParaOrdenacao => $tipoParaOrdenacao ) {
							if( $chaveOriginal==$chaveParaOrdenacao && $chaveOriginal==$ordem ) {
								if( $tipoParaOrdenacao == "integer" ) {
									$chaveComplementar = $chaveOriginal . "_campoextra";
									$arrayInteger[] = array( $chaveComplementar => ((int)$valorOriginal) );
									$arrayOriginal = $this->merge($arrayInteger, $arrayOriginal);
								}
								if( $tipoParaOrdenacao == "date" && $valorOriginal != "" ) {
									$dataPartes = explode("/", $valorOriginal);
									$dataComoNumeroOrdenavel = preg_replace( "/[^\d]/", "", $dataPartes[2].$dataPartes[1].$dataPartes[0] );
									$chaveComplementar = $chaveOriginal . "_campoextra";
									$arrayDate[] = array( $chaveComplementar => ((int)$dataComoNumeroOrdenavel) );
									$arrayOriginal = $this->merge($arrayDate, $arrayOriginal);
								}
							}
						}
					}
				}
			}
		}
		return $arrayOriginal;
	}

	function merge($array1, $array2) {
	        if(sizeof($array1)>sizeof($array2)) {
	            $size = sizeof($array1);
	        }else{
	            $a = $array1;
	            $array1 = $array2;
	            $array2 = $a;

	            $size = sizeof($array1);
	        }

	        $keys2 = array_keys($array2);

	        for($i = 0;$i<$size;$i++)
	        {
	            $array1[$keys2[$i]] = $array1[$keys2[$i]] + $array2[$keys2[$i]];
	        }

	        $array1 = array_filter($array1);
	        return $array1;
	}

	function monta_lista_array($dados,$cabecalho="",$perpage,$pages,$soma,$alinha,$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista") {
		// este mtodo monta uma listagem na tela baseado na sql passada (tem que estar fora de tags FORM'S)
		//$sql = Texto - sql que vai gerar a lista
		//$cabecaho = Vetor - contendo o nome que vai ser exibido, deve ter a mesma quantidade dos campos da sql
		//Parmetros de paginao
		//$perpage = Numrico - Registros por pgina
		//$pages = Numrico - Numrico - Mx de Paginas que sero mostradas na barrinha de paginao
		// $soma = Boleano - Mostra somatrio de campos numricos no fim da lista
		// $ordem = alinhamento dos títulos (left, rigth, center)
		//$html = passa-se um array com os respesctivos HTML's que seram substituidos pelos campos do resultado,
		//		  sendo que onde se quiser colocar o valor vindo do sql deve-se escrever {campo[1]},{campo[2]},... "começando de {campo[1]}"
		//		  EX.: <a href="javascript:void(0);" id="campo_popup_checkbox_{campo[0]}_1" onclick="{campo[3]}">{campo[1]} - {campo[2]}</a>
		//Registro Atual (instanciado na chamada)
		/**
		$arrayDeTiposParaOrdenacao = Vetor contendo campos e seus respectivos tipos especiais, permitindo a correta ordenação dos mesmos.
		Não há necessidade de passar todos os campos já presentes no vetor(array) de dados, mas somente aqueles que são diferentes do
		   tipo textual (string).
		Para um dado array contendo todos os dados da lista:
			...
			$dados_array[$posicaoNoLoop] = array("acao" 		=> $acao						  ,
												 "num_processo" => $val['prcnumsidoc']		 ." " ,
												 "interessado" 	=> $val['prcnomeinteressado']." " ,
												 "prioridade" 	=> $val['prioridade']			  ,
												 "data" 		=> $val['dataultimocadastro']	  ,
												 "dttramite" 	=> $val['dttramite']			  ,
												 "coord" 		=> $val['coodsc']				  ,
												 "situacao" 	=> $val['esddsc']
								   		   );
			}
			...
			Passar também o parâmetro tal como (somente campos diferentes de 'string'):
			...
			$arrayDeTiposParaOrdenacao = array();
			$arrayDeTiposParaOrdenacao[] = array( "data"   		 => "date"    );
			$arrayDeTiposParaOrdenacao[] = array( "num_processo" => "integer" );
			...
		 */

		if ($_REQUEST[$formName.'_numero']=='')
			$numero = 1;
		else
			$numero = intval($_REQUEST[$formName.'_numero']);
		//Controla o Order by
		if ($_REQUEST[$formName.'_ordemlista']<>'') {

			$dados = $this->criarColunasDeOrdenacaoEmArray($dados,$arrayDeTiposParaOrdenacao,$_REQUEST[$formName.'_ordemlista']);
			$campoParaOrdenacao = $_REQUEST[$formName.'_ordemlista'];
//			dbg($campoParaOrdenacao,1);

			if($arrayDeTiposParaOrdenacao){
				foreach( $arrayDeTiposParaOrdenacao as $linhaParaOrdenacao ) {
					foreach( $linhaParaOrdenacao as $chaveParaOrdenacao => $tipoParaOrdenacao ) {
						if( $chaveParaOrdenacao == $campoParaOrdenacao ) {
							$campoParaOrdenacao .= "_campoextra";
						}
					}
				}
			}

			if ($_REQUEST[$formName.'_ordemlistadir'] <> 'DESC') {
				$ordemlistadir = 'ASC';
				$ordemlistadir2 = 'DESC';
			} else {
				$ordemlistadir = 'DESC';
				$ordemlistadir2 = 'ASC';
			}

			// 	Obter uma lista de colunas
			foreach ($dados as $key => $row) {
				$row = preg_replace("/[^a-zA-Z0-9_]/", "", strtr($row[($campoParaOrdenacao)], "áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ", "aaaaeeiooouucAAAAEEIOOOUUC"));
		    	$crt[$key]  = $row;
			}

			switch($ordemlistadir) {
				case 'ASC':
					array_multisort($crt, SORT_ASC , $dados);
					break;
				case 'DESC':
					array_multisort($crt, SORT_DESC, $dados);
					break;
			}
		}

	    $RS = $dados;

		$nlinhas = count($RS);
		if (! $RS)
			$nl = 0;
		else
			$nl=$nlinhas;
		if (($numero+$perpage)>$nlinhas)
			$reg_fim = $nlinhas;
		else
			$reg_fim = $numero+$perpage-1;
		if ($nl>0){

			$ordenador = array_keys($RS[0]);

			$total_reg = $nlinhas;
			print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			//monta o formulario da lista mantendo os parametros atuais da pgina
			print '<form name="'.$formName.'" method="post">
						<input type="Hidden" name="'.$formName.'_numero" value="" />
						<input type="Hidden" name="'.$formName.'_ordemlista" value="'.$_REQUEST[$formName.'_ordemlista'].'"/>
						<input type="Hidden" name="'.$formName.'_ordemlistadir" value="'.$ordemlistadir.'"/>';

			foreach($_POST as $k=>$v){
				if ($k<>$formName.'_ordemlista' and $k<>$formName.'_ordemlistadir' and $k<>$formName.'_numero')
					if ( is_array($v) ){
						while ($val = current($v)){
							print '<input type="hidden" name="'.$k.'[]" value=\'' . $val . '\'/>';
							next($v);
						}
					}else{
						print '<input type="hidden" name="'.$k.'" value=\'' . $v . '\'/>';
					}
			}

			print '</form>';
			//Monta Cabealho

			$campoOrdenacaoOriginal = $_REQUEST[$formName.'_ordemlista'];
			//$posicao_ordenador_original = $camposOrdenacaoSubstituicao['posicao_ordenador_original'];

			if ( $cabecalho === null ) {

			}else if(is_array($cabecalho)){
				print '<thead><tr>';
				for ($i=0;$i<count($cabecalho);$i++)
				{
					if ($campoParaOrdenacao==($ordenador[$i]) || $campoOrdenacaoOriginal==($ordenador[$i]) ) {
						$ordemlistadirnova = $ordemlistadir2;
						$imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
					} else {
						$ordemlistadirnova = 'ASC';
						$imgordem = '';
					}
					print '<td align="' . $alinha . '" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena'.$formName.'(\''.($ordenador[$i]).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.simec_strip_tags($cabecalho[$i]).'">'.$imgordem.'<strong>'.$cabecalho[$i].'</strong></label>';
				}
				print '</tr> </thead>';
			}
			else
			{
				print '<thead><tr>'; $i=0;
				foreach($RS[0] as $k=>$v)
				{
					if ($campoParaOrdenacao==($i+1) ) {
						$ordemlistadirnova = $ordemlistadir2;
						$imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
					} else {
						$ordemlistadirnova = 'ASC';
						$imgordem = '';}
						print '<td valign="top" class="title" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena'.$formName.'(\''.($i+1).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.$k.'">'.$imgordem.'<strong>'.$k.'</strong></label>';
						$i=$i+1;}
						print '</tr> </thead>';
			}
			//Monta Listagem
			$totais = array();
			$tipovl = array();

			//Recebe padrão de substituição de HTML
			$search = array();
			for ($i=($numero-1);$i<$reg_fim;$i++)
			{
				$c = 0;
				if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
				print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';

				if ( count($RS[$i]) != count($search) ):
					for ($z=1; $z <= count($RS[$i]); $z++){
						$search[$z] = '{campo['.$z.']}';
					}
				endif;
				$col = 0;
				foreach($RS[$i] as $k=>$v) {
					if ( $col > count($cabecalho)-1 && count($cabecalho) ) break;

					if (is_numeric($v)){
						//cria o array totalizador
						if (!$totais['0'.$c]) {$coluna = array('0'.$c => $v); $totais = array_merge($totais, $coluna);} else $totais['0'.$c] = $totais['0'.$c] + $v;
						//Mostra o resultado
						if (strpos($v,'.')) {
							$v = number_format($v, 2, ',', '.');
							if (!$tipovl['0'.$c]) {
								$coluna = array('0'.$c => 'vl');
								$tipovl = array_merge($totais, $coluna);
							}else{
								$tipovl['0'.$c] = 'vl';
							}
						}
						if ($v<0)
							print '<td align="right" style="color:#cc0000;" title="'.simec_strip_tags($cabecalho[$c]).'">('.( $html[$col] ? str_replace($search, $RS[$i], $html[$col]) : $v).')';
						else
							print '<td align="right" style="color:#0066cc;" title="'.simec_strip_tags($cabecalho[$c]).'">'.( $html[$col] ? str_replace($search, $RS[$i], $html[$col]) : $v);

						print ('<br>'.$totais[$c]);

					}else{
						print '<td title="'.simec_strip_tags($cabecalho[$c]).'">'.( $html[$col] ? str_replace($search, $RS[$i], $html[$col]) : $v);
					}
					print '</td>';

					$c = $c + 1;
					$col++;
				}
				print '</tr>';
			}

			if ($soma=='S'){
				//totaliza (imprime totais dos campos numericos)
				print '<thead><tr>';
				for ($i=0;$i<$c;$i++)
				{
					print '<td align="right" title="'.simec_strip_tags($cabecalho[$i]).'">';

					if ($i==0) print 'Totais:   ';
					if (is_numeric($totais['0'.$i])) print number_format($totais['0'.$i], 2, ',', '.'); else print $totais['0'.$i];
					print '</td>';
				}
				print '</tr>';
				//fim totais
			} else if($soma=='INT'){
                //totaliza (imprime totais dos campos numericos)
                print '<thead><tr>';
                for ($i=0;$i<$c;$i++)
                {
                    print '<td align="right" title="'.simec_strip_tags($cabecalho[$i]).'">';

                    if ($i==0) print 'Totais:   ';
                    if (is_numeric($totais['0'.$i])) print $totais['0'.$i]; else print $totais['0'.$i];
                    print '</td>';
                }
                print '</tr>';
                //fim totais
            }

			print '</table>';
			print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem"><tr bgcolor="#ffffff"><td><b>Total de Registros: '.$nl.'</b></td><td>';

			include APPRAIZ."includes/paginacao_lista_array.inc";
			print '</td></tr></table>';
			print '<script language="JavaScript">';
			print 'function ordena'.$formName.'(ordem, direcao) {';
			print 	'document.'.$formName.'.'.$formName.'_ordemlista.value=ordem;';
			print	'document.'.$formName.'.'.$formName.'_ordemlistadir.value=direcao;';
			print 	'document.'.$formName.'.submit()';
			print 	'} ';
			print 'function pagina'.$formName.'(numero) {';
			print 	'document.'.$formName.'.'.$formName.'_numero.value=numero;';
			print	'document.'.$formName.'.submit();';
			print	'} ';
			print '</script>';
		}
		else
		{
			print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
			print '</table>';
		}
	}

	function monta_radio($var,$sql,$habil,$op) {
		// este mtodo monta uma sequencia html radio onde:
		// var = Texto - Nome do campo Radio;
		// sql  = Texto - Consulta para gerar a lista;
		// op = 0, 1 ou 2, onde 0 - significa &nbsp&nbsp, 1 - significa <br>, 2 - significa <br><br>
		global ${$var};
		if ($habil <> 'N') $habil = '' ; else $habil='  disabled="disabled" ';
		$esp = '  ';
		if ($op == 1) $esp = '<br>' ;
		if ($op == 2) $esp = '<br><br>';
		$res=$this->carregar($sql);
		for ($i=0;$i<count($res);$i++) {
			if (${$var}== $res[$i]['codigo']) $sel = '  checked '; else $sel='';
			print "<input type='radio' value='".$res[$i]['codigo']."' name='".$var."'".$sel.$habil."/> &nbsp;".$res[$i]['descricao']. ' &nbsp;&nbsp;&nbsp;' .$esp;
		}
	}

	function record_set ($SQL)
	{
		$res = @$this->executar($SQL);
		return $res;
	}

	function carregaAgrupado($sql, $agrupador) {
		$rs = $this->record_set($sql);
		$retorno = array();
		while($linha = pg_fetch_assoc($rs)) {
			$retorno[$linha[$agrupador]][] = $linha;
		}
		return $retorno;
	}

	function conta_linhas ($recurso)
	{
		// este mtodo conta as linhas de um recurso e subtrai 1
		$linhas = pg_num_rows($recurso);
		if( $linhas >= 0 )
		return $linhas-1;
		else
		return -1;
		//           if (! is_array(pg_fetch_all($recurso))) return -1;
		//           else return count(pg_fetch_all($recurso)) -1;
	}

	function pegaLinha($SQL, $linha=0, $tempocache=null) {

	    if($tempocache) {
    		//global $memcache_obj;
			/* conectando com o memcached server */
			//if(!$memcache_obj) $memcache_obj = memcache_connect($GLOBALS["memcachehost"], $GLOBALS["memcacheport"]);
			/* pegando informações do memcached server, key igual ao md5 do SQL */
			//$cache_result = memcache_get($memcache_obj, md5($SQL));
    		if (function_exists('zend_shm_cache_fetch')) {$cache_result = zend_shm_cache_fetch(md5($SQL));}
    		if($cache_result) { /* se existir cache, carregar com o resultado do memcached server */
    			$res = $cache_result;
    		} else { /* senão executa o SQL e guarda o resultado no memcached server */
				if(($RS = $this->executar($SQL)) && (pg_num_rows($RS)>=1)) {
					$res = pg_fetch_assoc($RS, $linha);
    				/* Armazenando os dados memcached server na chave md5(SQL), 0 => sem compressão, tempo para expirar de 30 seconds */
					//memcache_set($memcache_obj, md5($SQL), $res, 0, $tempocache);
					if (function_exists('zend_shm_cache_store')) {
						if(zend_shm_cache_store(md5($SQL), $res, $tempocache) === false) echo '[ZEND CACHE FALHOU]';
					}

				}
    		}
    	} else {
   			//Retorna um registro de uma query, a partir da coluna especificada
			if(($RS = $this->executar($SQL)) && (pg_num_rows($RS)>=1)) {
				$res = pg_fetch_assoc($RS, $linha);
			}
    	}

    	if($res) return $res;
		else return false;

	}

	function pegaUm($SQL, $coluna=0, $tempocache=null) {

	    if($tempocache) {
    		//global $memcache_obj;
			/* conectando com o memcached server */
			//if(!$memcache_obj) $memcache_obj = memcache_connect($GLOBALS["memcachehost"], $GLOBALS["memcacheport"]);
			/* pegando informações do memcached server, key igual ao md5 do SQL */
			//$cache_result = memcache_get($memcache_obj, md5($SQL));

    		if (function_exists('zend_shm_cache_fetch')) {$cache_result = zend_shm_cache_fetch(md5($SQL));}

    		if($cache_result) { /* se existir cache, carregar com o resultado do memcached server */
    			$res = $cache_result;
    		} else { /* senão executa o SQL e guarda o resultado no memcached server */
				if(($RS = $this->executar($SQL)) && (pg_num_rows($RS)>=1)) {
					$res = pg_fetch_result($RS, 0, $coluna);
    				/* Armazenando os dados memcached server na chave md5(SQL), 0 => sem compressão, tempo para expirar de 30 seconds */
					//memcache_set($memcache_obj, md5($SQL), $res, 0, $tempocache);
					if (function_exists('zend_shm_cache_store')) {
						if(zend_shm_cache_store(md5($SQL), $res, $tempocache) === false) echo '[ZEND CACHE FALHOU]';
					}

				}
    		}
    	} else {
   			//Retorna um registro de uma query, a partir da coluna especificada
			if(($RS = $this->executar($SQL)) && (pg_num_rows($RS)>=1)) {
				$res = pg_fetch_result($RS, 0, $coluna);
			}
    	}

    	if($res) return $res;
		else return false;
	}

	/**
	 * Retorna o ano do exercício ativo.
	 */
	function pega_ano_atual()
	{
		static $ano = false;
		if ( !$ano )
		{
			$esquema = $_SESSION['sisdiretorio'];
			$tabela = "programacaoexercicio";
			$sql =
				" select count(*) " .
				" from pg_class pc " .
				" inner join pg_namespace ns on " .
				" ns.oid = pc.relnamespace and " .
				" ns.nspname='" . $esquema . "' " .
				" where " .
				" relname = '" . $tabela . "'";
			$existe = (boolean) $this->pegaUm( $sql );

			$sql = "select
						count(sisexercicio)
					from
						seguranca.sistema
					where
						sisdiretorio = '$esquema'
					and
						sisexercicio = true";
			$sisexercicio = (boolean) $this->pegaUm( $sql );

			if ( $existe && $sisexercicio)
			{
				$sql =
					" select prsano " .
					" from " . $esquema . "." . $tabela .
					" where prsexerccorrente = 't' " .
					" order by prsano desc " .
					" limit 1 ";
				$ano = (integer) $this->pegaUm( $sql );
			}
			if ( !$ano )
			{
                $exercicio = ANO_EXERCICIO;
                if($exercicio){
                    $ano = $exercicio;
                } else {
                    $ano = date("Y");
                }
			}
		}
		return $ano;
	}

	/**
	 * Captura o nome das colunas de uma tabela.
	 *
	 * @param string $table
	 * @param string $schema
	 * @return string[]
	 */
	function pegarColunas( $table, $schema = 'public' )
	{
		$table = str_replace( "'", "\\'", $table );
		$schema = str_replace( "'", "\\'", $schema );
		$sql = "select column_name from information_schema.columns where table_schema = '" . $schema . "' and table_name = '" . $table . "'";
		$linhas = $this->carregar( $sql );
		if ( !$linhas )
		{
			return array();
		}
		$colunas = array();
		foreach ( $linhas as $linha )
		{
			array_push( $colunas, $linha['column_name'] );
		}
		return $colunas;
	}

	/**
	 * Captura as tabelas de um schema, caso o schema não seja indicado
	 * todas as tabelas são retornadas.
	 *
	 * @param string $schema
	 * @return string[]
	 */
	function pegarTabelas( $schema = null )
	{
		$where = '';
		if ( $schema != null )
		{
			$schema = str_replace( "'", "\\'", $schema );
			$where = " where table_schema = '" . $schema. "' ";
		}
		$sql = 'select distinct table_name from information_schema.columns ' . $where;
		$linhas = $this->carregar( $sql );
		if ( !$linhas )
		{
			return array();
		}
		$tabelas = array();
		foreach ( $linhas as $linha )
		{
			array_push( $tabelas, $linha['table_name'] );
		}
		return $tabelas;
	}

	function carrega_registro ( $recurso, $posicao = 0 )
	{
		// este metodo carrega em um array os campos de um registro
		$registro = array();
		if ( is_resource( $recurso ) && pg_num_rows( $recurso ) > 0 )
		{
			$registro = pg_fetch_array( $recurso, $posicao );
		}
		return $registro;
	}

	function carrega_tudo ($recurso)
	{
		// este mtodo carrega em um array os campos de um registro
		return pg_fetch_all($recurso);
	}

    #DESENVOLVEDOR: Luciano F. Ribeiro.
    #Data: 14/08/2013.
    #PARÂMETROS:
    #$modulo: Diretório no qual é localizado o arquivo, caso seja passado em branco é retornado a pagian anterior. EX: principal/cessao_prorrogacao/cad_dados_servidor_siape
    #$parametros: parâmetros a ser passado na URL, caso seja necessário. EX: '&cprid=1', '&cprid='.$variavel, '&cprid='.$_SESSION['gestao']['cprid']
    #$msg: mensagem a ser mostrada no final da operação.
    #$close: Caso seja passado com parametro 'S' é fechado a tela apos a operação.
    #$upJanelaPai: Essa parametro se 'S', usa o comando "window.opener" que atualiza a janela Pai, para que funcione perfeitamente o parâmentro $modulo é obrigatório e deve ser passado o "endereço" da pagina Pai.
    #$paginaSistema: Caso queira usar em alguma pagina do sistema tipo "usuario responsabilidade", não é necessário passar o parametro $modulo, apenas marcar esse opção com S
    function sucesso($modulo, $parametros='', $msg='Operação realizada com sucesso', $close='N', $upJanelaPai='N', $paginaSistema=''){
        echo "<html>";
        echo "<head>";
        echo "<script>";
        echo "alert('".$msg."');";

        if($modulo != ''){
            if($upJanelaPai == 'N'){
                $saida = $_SESSION['sisarquivo'] . ".php?modulo=" . $modulo . "&acao=" . $_REQUEST['acao'] . $parametros;
                echo "location.href='".$saida."';";
            }else{
                $saida = $_SESSION['sisarquivo'] . ".php?modulo=" . $modulo . "&acao=" . $_REQUEST['acao'] . $parametros;
                echo "window.opener.location.href='".$saida."';";
            }
        }else{
            if( $especificarPag != '' ){
                echo "window.parent.opener.location.reload();";
            }else{
                echo "history.back();";
            }
        }

        if($close == 'S'){
           echo 'window.close();';
        }

        echo "</script>";
        echo "</head>";
        echo "<body></body>";
        echo "</html>";

        exit();
    }

    function insucesso( $mensagem='', $parametros='', $modulo='' ){

        $modulo = $modulo ? $modulo : "inicio";

	$saida = $_SESSION['sisdiretorio'] . '/' . $_SESSION['sisdiretorio'] . ".php?modulo=" . $modulo . "&acao=C" . $parametros;
		$url = "../insucesso.php?saida=" . urlencode( $saida )."&mensagem=" . $mensagem;
?>
    <html>
	<head>
            <script type="text/javascript">
                location.href = "<?= $url ?>";
            </script>
	</head>
	<body></body>
	</html>
<?PHP
        exit();
    }

	function rollback() {
		pg_query(self::$link[$this->nome_bd], 'rollback; ');
		unset($_SESSION['transacao']);
	}

	function commit() {
		if ($_SESSION['usucpf']==$_SESSION['usucpforigem'] or $_SESSION['superuser'] ){
			pg_query(self::$link[$this->nome_bd], 'commit; ');
			unset($_SESSION['transacao']);
			return true;
		} else {
			pg_query(self::$link[$this->nome_bd], 'rollback; ');
			unset($_SESSION['transacao']);
			return false;
		}
	}

	function close() {
		if ($_SESSION['transacao'])
		{
			pg_query(self::$link[$this->nome_bd], 'rollback; ');
			unset($_SESSION['transacao']);
		}
		if (is_resource(self::$link[$this->nome_bd])) {@pg_close( self::$link[$this->nome_bd] );}
	}

	/* Store login informations */
	function StoreLogin($username,$password) {
		/* Generate a mask and store the result */
		$data = $username . chr(1) . $password;
		$key = $this->randomkey(32);
		/* Store encrypted username and password */
		$_SESSION[serverauth] = criptografia::rc4($key,$data,"en");
		/* Store the mask */
		SetCookie( "cookieauth",base64_encode($key),0,"/");
		return 1;
	}

	function GetLogin(&$username,&$password) {
		if( strlen( $_SESSION[serverauth] ) > 0 and  strlen( $_COOKIE[cookieauth] ) > 0 ) {
			$key=base64_decode(stripslashes($_COOKIE[cookieauth]));
			if( strlen( $key ) > 0 ) {
				$edata=stripslashes($_SESSION[serverauth]);
				$data=criptografia::rc4($key,$edata,"de");
				if( strlen( $data ) > 0 ) {
					$chr1pos = strpos( $data, chr( 1 ) );
					if( $chr1pos > 0 ) {
						$username = substr( $data, 0, $chr1pos );
						$password = substr( $data, $chr1pos + 1 );
					}
				}
			}
			return 1;
		} else
		return 0;
	}

	/* generate a random string (length $len) */
	function randomstr( $len ) {
		$c = "";
		for( $i = 0; $i < $len; $i++ )
		$c .= chr( rand( 0, 255 ) );
		return $c;
	}

	/* A random key */
	function randomkey( $len ) {
		srand( ( double )microtime() * 1000000 );
		return $this->randomstr( $len );
	}

	/* Retrieve login informations */
	function testa_coordenador($acao,$t)
	{
		// verifica se  coordenador de ao ou se  responsvel por subao
		if ($t == 'A')
		$sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=1 and acaid='.$acao." and usucpf = '".$_SESSION['usucpf']."' and rpustatus <> 'I' ";
		if ($t == 'S')
		$sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=9 and acaid='.$acao." and usucpf = '".$_SESSION['usucpf']."' and rpustatus <> 'I' ";

		$registro=$this->recuperar($sql);
		if (is_array($registro)) return true;
		else return false;
	}

	function testa_emenda()
	{
		$sql= "select pu.usucpf from seguranca.perfilusuario pu inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflcod=21 and pu.usucpf ='".$_SESSION['usucpf']."'";
		if ($this->pegaUm($sql)) return true;
		else return  false;
	}

	function testa_altagestao()
	{
		$sql= "select pu.usucpf from seguranca.perfilusuario pu inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflcod=11 and pu.usucpf ='".$_SESSION['usucpf']."'";
		if ($this->pegaUm($sql)) return true;
		else return  false;
	}
	function testa_altagestaopje($cod=0)
	{
		// verifica se é alta-gestão ou gerente do projeto
		$sql= "select pu.usucpf from seguranca.perfilusuario pu inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflcod in (58,12) and pu.usucpf ='".$_SESSION['usucpf']."' inner join monitora.usuarioresponsabilidade ur on ur.usucpf=pu.usucpf and ur.pjeid=".$_SESSION['pjeid'];
		if ($cod)
		{
			// então está testando um específico projeto
			$sql= "select pu.usucpf from seguranca.perfilusuario pu inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflcod in (58,12) and pu.usucpf ='".$_SESSION['usucpf']."' inner join monitora.usuarioresponsabilidade ur on ur.usucpf=pu.usucpf and ur.pjeid=$cod";

		}
		if ($this->pegaUm($sql)) return true;
		else return  false;
	}

	function testa_coordenador_plan($cod)
	{
		// verifica se  coordenador de planejamento ou super-usuario
		if ($this->testa_superuser())  return true;
		else {
			$sql= 'select usucpf from '.$_SESSION['sisdiretorio'].'.usuarioresponsabilidade where pflcod in (24,23) and acaid='.$cod." and usucpf = '".$_SESSION['usucpf']."'";

			$registro=$this->recuperar($sql);
			if (is_array($registro)) return true;
			else return false;
		}
	}

	function testa_gerente($prg,$t)
	{
		// verifica se  gerente de programa
		$sql= 'select usucpf from '.$_SESSION['sisdiretorio'].'.usuarioresponsabilidade where pflcod=2 and prgid='.$prg." and usucpf = '".$_SESSION['usucpf']."'";
		$registro=$this->recuperar($sql);
		if (is_array($registro)) return true;
		else return false;
	}


	function testa_responsavel_projespec($cod=0)
	{
		// verifica se  é responsavel por projeto especial ou super usuário
		if ($cod)
		{
			//
			$sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod in (12,47,56) and pjeid='.$cod." and usucpf = '".$_SESSION['usucpf']."'";
		}
		else
		$sql= "select usucpf from seguranca.perfilusuario where pflcod in (62) and usucpf = '".$_SESSION['usucpf']."'";
		$resp = $this->pegaUm($sql);

		if ($resp) return true;
		else return false;
	}


	function testa_documentador_projespec($cod=0)
	{
		// verifica se  é responsavel por documentar o projeto especial ou super usuário
		$sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod in (75,56) and pjeid='.$cod." and usucpf = '".$_SESSION['usucpf']."'";
		$resp = $this->pegaUm($sql);
		if ($resp) return true;
		else return false;
	}

	function testa_gerente_exe($acao,$t)
	{
		// verifica se  gerente-executivo de ao ou de programa
		if ($t == 'A')
		$sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=3 and acaid='.$acao." and usucpf = '".$_SESSION['usucpf']."'";
		else
		$sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=3 and prgid='.$acao." and usucpf = '".$_SESSION['usucpf']."'";
		$registro=$this->recuperar($sql);
		if (is_array($registro)) return true; else return false;
	}

	function testa_digitador($acao,$t)
	{
		// verifica se  digitador de ao, de programa ou de subao ou projeto especial
		if ($t == 'A')
		$sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=8 and acaid='.$acao." and usucpf = '".$_SESSION['usucpf']."'";
		if ($t == 'P')
		$sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=8 and prgid='.$acao." and usucpf = '".$_SESSION['usucpf']."'";
		if ($t == 'S')
		$sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=8 and saoid='.$acao." and usucpf = '".$_SESSION['usucpf']."'";
		if ($t == 'E')
		$sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=51 and pjeid='.$acao." and usucpf = '".$_SESSION['usucpf']."'";

		$registro=$this->recuperar($sql);
		if (is_array($registro)) return true; else return false;
	}

	function testa_proprietario($cod)
	{
		// verifica se  digitador é o proprietário do registro
		$sql= "select ptosntemdono as temdono,usucpf   from monitora.planotrabalho where ptoid=$cod ";
		$res=$this->pegalinha($sql);
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
		if ($temdono == 'f') return true;
		else if ($temdono == 't' and $usucpf==$_SESSION['usucpf']) return true;
		else return false;
	}


	function testa_superuser()
	{
		// testa se  super usurio
		if ( !$_SESSION['usucpf'] || !$_SESSION['sisid'] ) {
			return false;
		}

        if (!isset($_SESSION['testa_superuser_cache'][$_SESSION['usucpf']][$_SESSION['sisid']])) {
        	$sql= "select pu.usucpf from seguranca.perfilusuario pu inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflsuperuser='t' and pu.usucpf ='".$_SESSION['usucpf']."' and p.sisid=".$_SESSION['sisid'];
        	$registro = $this->conta_linhas($this->executar($sql));

        	$_SESSION['testa_superuser'] = $registro;
        	$_SESSION['testa_superuser_cache'][$_SESSION['usucpf']][$_SESSION['sisid']] = $registro;
        }

        return $_SESSION['testa_superuser_cache'][$_SESSION['usucpf']][$_SESSION['sisid']] >= 0 ? true : false;
	}

	function testa_coorduma()
	{ // testa se  é coordenador da UMA
	$sql= "select pu.usucpf from seguranca.perfilusuario pu inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflcod=4 and pu.usucpf ='".$_SESSION['usucpf']."' and p.sisid=".$_SESSION['sisid'];
	$registro=$this->recuperar($sql);
	if (is_array($registro)) return true;
	else return false;
	}

	function testa_uma()
	{
        // verifica se o usuário pode emular outro usuário
        $sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflsuporte='t' and usucpf ='".$_SESSION['usucpf']."' and p.sisid=".$_SESSION['sisid'];
        $registro = $this->conta_linhas($this->executar($sql));
        $_SESSION['testa_uma'] = $registro;

		return $registro >= 0 ? true : false;
	}

	function testa_cgo()
	{
		// verifica se  perfil CGO  ou equipe cgo
		if ($_SESSION['sisid']==1)
		$sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod in (4,6,18) and pu.usucpf ='".$_SESSION['usucpf']."' and p.sisid=".$_SESSION['sisid'];
		else if ($_SESSION['sisid']==2)
		$sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod in (50,52) and pu.usucpf ='".$_SESSION['usucpf']."' and p.sisid=".$_SESSION['sisid'];
		$registro=$this->recuperar($sql);
		if (is_array($registro)) return true;
		else return false;
	}

	function testa_responsabilidade($id,$pflcod, $campo)
	{
		// verifica se  gerente de programa
		$sql= "select usucpf from usuarioresponsabilidade where pflcod=$pflcod and $campo = '$id' and usucpf = '".$_SESSION['usucpf']."';";
		$registro=$this->recuperar($sql);
		if (is_array($registro)) return true;
		else return false;
	}

	function mostra_resp( $chave_valor, $chave_nome,$ano=true,$schema='monitora' )
	{
		// carrega os registros
		if ($ano){
		$sql = sprintf(
			"select distinct p.pflnivel, p.pfldsc, usu.usucpf, usu.usunome, usu.usufoneddd, usu.usufonenum, uni.unidsc
					from seguranca.perfil p
					inner join $schema.usuarioresponsabilidade ur on ur.pflcod = p.pflcod
					inner join seguranca.usuario usu on usu.usucpf = ur.usucpf
					inner join unidade uni on uni.unicod = usu.unicod
					where p.pflstatus = 'A' and ur.%s = '%s' and ur.rpustatus = 'A' and p.pflresponsabilidade != 'H' and ur.prsano = '%s'
					order by p.pflnivel",
		$chave_nome,
		$chave_valor,
		$_SESSION['exercicio']
		);


		}else
		$sql = sprintf(
			"select distinct p.pflnivel, p.pfldsc, usu.usucpf, usu.usunome, usu.usufoneddd, usu.usufonenum, uni.unidsc
					from seguranca.perfil p
					inner join $schema.usuarioresponsabilidade ur on ur.pflcod = p.pflcod
					inner join seguranca.usuario usu on usu.usucpf = ur.usucpf
					inner join unidade uni on uni.unicod = usu.unicod
					where p.pflstatus = 'A' and ur.%s = '%s' and ur.rpustatus = 'A' and p.pflresponsabilidade != 'H'
					order by p.pflnivel",
		$chave_nome,
		$chave_valor
		);
		$responsaveis = $this->carregar( $sql );
		if ( !$responsaveis ) {
			return;
		}

		// exibe o primeiro registro
		echo "<script>
					function exibirEquipeApoio(){
						elemento = document.getElementById( 'responsaveis' );
						imagem = document.getElementById( 'botao_mais_menos' );
						if ( elemento.style.display == 'block' ) {
							elemento.style.display = 'none';
							imagem.src = '../imagens/mais.gif';
						} else {
							imagem.src = '../imagens/menos.gif';
							elemento.style.display = 'block';
						}
					}
				</script>";
		$responsavel = array_shift( $responsaveis );
		$htm = sprintf(
			"<tr><td width='250' align='right' class='SubTituloDireita'><a href='#' title='exibir equipe de apoio' onclick='exibirEquipeApoio();'><img id='botao_mais_menos' src='../imagens/mais.gif' border='0'/></a> %s:</td><td><img src='../imagens/email.gif' title='Enviar e-mail ao Gestor' border='0' onclick='envia_email(\"%s\");'> %s<br><font color=#888888>%s - Tel: (%s) %s</font></td></tr>",
		$responsavel['pfldsc'],
		$responsavel['usucpf'],
		$responsavel['usunome'],
		$responsavel['unidsc'],
		$responsavel['usufoneddd'],
		$responsavel['usufonenum']
		);
		echo $htm;

		// exibe os demais registros
		echo '<tr><td colspan="2" style="border: 0; padding: 0;"><div id="responsaveis" style="display: none"><table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 100%; border: 0">';
		foreach ( $responsaveis as $indice => $responsavel ) {
			// monta o html
			$htm = sprintf(
				"<tr><td width='250' align='right' class='SubTituloDireita' width='20%%'>%s:</td><td><img src='../imagens/email.gif' title='Enviar e-mail ao Gestor' border='0' onclick='envia_email(\"%s\");'> %s<br><font color=#888888>%s - Tel: (%s) %s</font></td></tr>",
			$responsavel['pfldsc'],
			$responsavel['usucpf'],
			$responsavel['usunome'],
			$responsavel['unidsc'],
			$responsavel['usufoneddd'],
			$responsavel['usufonenum']
			);
			echo $htm;
		}
		echo "</table></div></td></tr>";
	}


	function cabecalho_projeto($pjeid)
	{
		$sql = "select p.*,u.ungabrev from monitora.projetoespecial p inner join unidadegestora u using (ungcod) where pjeid=".$pjeid;
		$RS = $this->record_set($sql);
		$nlinhas = $this->conta_linhas($RS);
		$nl=$nlinhas;
		//if ($nlinhas >= 0) {
		$res =  $this->carrega_registro($RS,0);
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
		// a linha abaixo transforma em variÃ¡veis todos os campos do array

		?>
	<tr>
		<td align='right' class="subtitulodireita" width="20%">Denominação:</td>
		<td><?=$ungabrev.'-'.$pjecod.'  '.$pjedsc?></td>
	</tr>
		<?php
		@$this -> mostra_resp($pjeid, 'pjeid',false);
	}


	function cabecalho_acao($acaid)
	{
		global $prgcod,$acacod,$loccod,$unicod;
		$sql = "select * from acao where acaid=".$acaid;
		$RS = $this->record_set($sql);
		$nlinhas = $this->conta_linhas($RS);
		$nl=$nlinhas;
		//if ($nlinhas >= 0) {
		$res =  $this->carrega_registro($RS,0);
		// a linha abaixo transforma em variÃ¡veis todos os campos do array
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
		$unidsc = $this->pegaUm( "select unidsc from public.unidade where unicod = '" . $unicod . "'" );
		?>
	<tr>
		<td align='right' class="subtitulodireita" width="20%">Ação:</td>
		<td><?=$prgcod.'.'.$acacod.'.'.$unicod.'.'.$loccod.' - '.$acatitulo?></td>
	</tr>
	</tr>
	<td align='right' class="subtitulodireita">Unidade:</td>
	<td><?=$unicod.' - '.$unidsc?></td>
	</tr>
		<?php
		@$this -> mostra_resp($acaid, 'acaid');
	}

	function cabecalho_programa($prgid)
	{

		$sql = "select * from planejamento.programa where prgid=".$prgid;
		$RS = $this->record_set($sql);
		$nlinhas = $this->conta_linhas($RS);
		$nl=$nlinhas;
		//if ($nlinhas >= 0) {
		$res =  $this->carrega_registro($RS,0);
		// a linha abaixo transforma em variÃ¡veis todos os campos do array
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
		$orgdsc = $this->pegaUm( "select orgdsc from public.orgao where orgcod = '" . $orgcod . "' limit 1" );
		?>
	<tr>
		<td align='right' class="subtitulodireita" width="20%">Programa:</td>
		<td><?=$prgcod . ' - ' . $prgdsc?></td>
	</tr>
	</tr>
	<td align='right' class="subtitulodireita">Órgão:</td>
	<td><?=$orgcod.' - '.$orgdsc?></td>
	</tr>
		<?php
		@$this -> mostra_resp($prgid, 'prgid');
	}


	function relatsubacao($res,$acaoid)
	{

		unset($inicio,$fim,$soma);
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
		$nivel = $this->busca_pai($ptoid,0);
		if (! in_array($ptoid,$_SESSION['ptoid']))

		{
			if ($ptotipo=='S' and $nivel==0)
			{
				include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/relatorio/acao/dadosfisevo2.inc";
				$sql = "select p.acaid,p.ptoid, ptoid_pai,ptotipo,p.ptocod, case when p.ptotipo='S' then 'Subao' when p.ptotipo='E' then 'Etapa' else 'Fase' end as tipo,p.ptodsc, p.ptoprevistoexercicio as previsto, p.ptosnpercent, p.ptosnsoma, u.unmdsc, case when sum(e.exprealizado) is null then 0 else sum(e.exprealizado) end as totalrealizado,to_char(p.ptodata_ini,'DD/MM/YYYY') as inicio, to_char(p.ptodata_fim,'DD/MM/YYYY') as fim from planotrabalho p inner join unidademedida u on p.unmcod=u.unmcod left join execucaopto e on p.ptoid=e.ptoid where p.ptostatus='A' and p.ptoid_pai=".$ptoid."  and p.acaid=".$acaoid."  group by p.acaid,p.ptoid_pai,p.ptoid,p.ptotipo, p.ptocod, p.ptodsc, p.ptoprevistoexercicio, p.ptosnpercent, p.ptosnsoma,p.ptoordem, u.unmdsc,p.ptodata_ini,p.ptodata_fim order by p.ptoordem,p.ptotipo desc,p.ptoid_pai, p.ptocod";

				$RSp = $this->record_set($sql);
				$nlinhasp = $this->conta_linhas($RSp);
				for ($im=0; $im<=$nlinhasp;$im++)
				{
					$res = $this->carrega_registro($RSp,$im);
					$this->relatsubacao($res,$acaoid);
				}
			}
			else if ($nivel==1)
			include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/relatorio/acao/dadosfisevo2.inc";
			else if ($nivel==2) {
				if (in_array($ptoid_pai,$_SESSION['ptoid']))
				include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/relatorio/acao/dadosfisevo2.inc";
			}
		}
	}


	// Returns true if $string is valid UTF-8 and false otherwise.
	function is_utf8($string) {

		// From http://w3.org/International/questions/qa-forms-utf-8.html
		return preg_match('%^(?:
	         [\x09\x0A\x0D\x20-\x7E]            # ASCII
	       | [\xC2-\xDF][\x80-\xBF]            # non-overlong 2-byte
	       |  \xE0[\xA0-\xBF][\x80-\xBF]        # excluding overlongs
	       | [\xE1-\xEC\xEE\xEF][\x80-\xBF]{2}  # straight 3-byte
	       |  \xED[\x80-\x9F][\x80-\xBF]        # excluding surrogates
	       |  \xF0[\x90-\xBF][\x80-\xBF]{2}    # planes 1-3
	       | [\xF1-\xF3][\x80-\xBF]{3}          # planes 4-15
	       |  \xF4[\x80-\x8F][\x80-\xBF]{2}    # plane 16
	   )*$%xs', $string);

	} // function is_utf8

	function converttoxml( &$dbconnection, $strsql, $filename, $arquivo )
	{
		ini_set( 'memory_limit', '128M' );
		$rs = pg_query($strsql);
		$nlinhas = !is_array( pg_fetch_all( $rs ) ) ? -1 : count( pg_fetch_all( $rs ) ) - 1;
		$dir = IS_PRODUCAO ? '/var/www/html/simec/arquivos/SIGPLAN/exportacao/' : '/var/www/html/simec/arquivos/SIGPLAN/exportacao/';
		$filename = basename( $filename, '.xml' ) . date( '-d-m-y' ) . '.xml';
		$path = $dir . $filename;
		$xml_file = fopen( $path, 'w' );
		if ( !$xml_file )
		{
			return 0;
		}
		$xml = '<?xml version="1.0" encoding="utf-8"?'.">\n";
		$xml .= '<ArrayOf' . $arquivo . ' xmlns="http://www.sigplan.gov.br/xml/">';
		//$xml.='<ArrayOf'.$arquivo.' xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns="http://www.sigplan.gov.br/xml/">';
		$sucesso = fwrite( $xml_file, $xml );
		for ( $j = 0; $j <= $nlinhas; $j++ )
		{
			$xml = '';
			$res = pg_fetch_array( $rs, $j );
			if( is_array( $res ) )
			{
				$xml .= "\n  <" . $arquivo. ">\n";
				foreach( $res as $k => $v )
				{
					${$k} = $v;
					if ( $v == 't' )
					{
						$v = '1';
					}
					elseif ( $v == 'f' )
					{
						$v = '0';
					}
					if ( !is_int( $k ) && $v != '' )
					{
						$v = utf8_encode( simec_htmlspecialchars( $v ) );
						$v = str_replace( "'", "&apos;", $v );
						$xml .= "    <" . $k . ">" . $v . "</" . $k . ">\n";
					}
				}
				$xml .= "  </" . $arquivo . ">";
				$sucesso = fwrite( $xml_file, $xml );
			}
		}
		$xml = "\n</ArrayOf" . $arquivo . ">";
		$sucesso = fwrite( $xml_file, $xml );
		fclose( $xml_file );
		return $sucesso > 0 ? 1 : 0;
	}




	function chama_etapafase($cod)
	{
		unset($referencias);
		$referencias = array();
		if ($coordaca or $digit){
			$sql = "select refcod from referencia where cast(current_date as date)>=refdata_inicio and cast(current_date as date)<=refdata_limite_avaliacao_aca";
			$RS = $db->record_set($sql);
			$nlinhas = $db->conta_linhas($RS);
			if ($nlinhas >= 0) {
				for ($i=0; $i<=$nlinhas;$i++){
					$res = $db->carrega_registro($RS,$i);
					foreach($res as $k=>$v) ${$k}=$v;
					array_push($referencias, $v);
				}
			}
		}
		$sql = "select p.ptoid, p.ptocod, p.ptodsc, p.ptoprevistoexercicio as previsto, p.ptosnpercent, p.ptosnsoma, u.unmdsc, case when sum(e.exprealizado) is null then 0 else sum(e.exprealizado) end as totalrealizado from planotrabalho p inner join unidademedida u on p.unmcod=u.unmcod left join execucaopto e on p.ptoid=e.ptoid where p.ptoid = $cod and p.acaid=".$_SESSION['acaid']."  group by p.ptoid, p.ptocod, p.ptodsc, p.ptoprevistoexercicio, p.ptosnpercent, p.ptosnsoma, u.unmdsc";
		$RS = $this->record_set($sql);
		$nlinhas = $this->conta_linhas($RS);
		if ($nlinhas >= 0)
		{
			$res = $this->carrega_registro($RS,0);
			foreach($res as $k=>$v) ${$k}=$v;
			$porcentorealizado = $totalrealizado*100/$previsto;
			if ($porcentorealizado > 100) $mostraporcentorealizado = 100; else $mostraporcentorealizado = $porcentorealizado;
			$porcentoexecutado = 100 - $mostraporcentorealizado;?>
	<THEAD bgcolor="#f5f5f5">
		<tr style="background-color: #ececec;">
			<TD colspan="12" align="left" style="color: #000099;">Cd.: <strong><?=$ptocod?>
			- <?=$ptodsc?></strong></TD>
			<TD colspan="2" align="right">Unid. Medida:<strong><?=$unmdsc?></strong><br>
			0% <label
				style="border: 1px solid #000000; font-size: 8px; border-top: 1px solid #c0c0c0; border-right: 1px solid #c0c0c0; border-left: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; background-color: #ffffff;"
				title="Realizado <?=number_format($porcentorealizado, 0, '.', '')?>%"><span
				style="background-color: #33cc00; font-size: 8px;"><?for ($k=0; $k<$mostraporcentorealizado/2;$k++) print " ";?></span><?for ($k=0; $k<$porcentoexecutado/2;$k++) print " ";?></label>
				<?
				if ($porcentorealizado < 100) print number_format($porcentorealizado, 0, '.', ''); else print '100'; ?>%</TD>
		</tr>
	</THEAD>
	<TBODY>
		<TR style="color: #808080;" bgcolor="#f6f6f6">
		<?php
		$sql = "select refmes_ref, refano_ref from referencia where refdata_limite_avaliacao_aca is not null and refsnmonitoramento='t'   and refano_ref='".$_SESSION['exercicio']."'   order by refano_ref,refmes_ref";
		$RS2 = $this->record_set($sql);
		$nlinhas2 = $this->conta_linhas($RS2);
		if ($nlinhas2 >= 0)
		{
			for ($j=0; $j<=$nlinhas2;$j++)
			{
				$res2 = $this->carrega_registro($RS2,$j);
				foreach($res2 as $k=>$v) ${$k}=$v;
				?>
			<TD align="right"><?=$refmes_ref.'/'.$refano_ref?></TD>
			<?php  }
	}
	$colspan = 12 - $j;
	if ($colspan>0) print "<TD colspan='".$colspan."'></TD>";
	?>
			<TD align="right">Realizado</TD>
			<TD align="right">Previsto</TD>
		</TR>
		<TR style="height: 30px;">
		<?php
		$sql = "select r.refcod, case when e.exprealizado is null then -1 else e.exprealizado end as exprealizado,  e.expobs, e.tpscod, t.tpscod, t.tpsdsc from referencia r left join execucaopto e on r.refcod=e.refcod and e.ptoid=".$ptoid." left join tiposituacao t on e.tpscod=t.tpscod where r.refdata_limite_avaliacao_aca is not null and r.refsnmonitoramento='t' and r.refano_ref='".$_SESSION['exercicio']."'   order by refano_ref,refmes_ref";
		$RS2 = $this->record_set($sql);
		$nlinhas2 = $this->conta_linhas($RS2);
		$totalrealizado = 0;
		if ($nlinhas2 >= 0)
		{
			for ($j=0; $j<=$nlinhas2;$j++)
			{
				$res2 = $this->carrega_registro($RS2,$j);
				foreach($res2 as $k=>$v) ${$k}=$v;
				if ($exprealizado == -1) $v_exprealizado=$exprealizado+1; else $v_exprealizado=$exprealizado;
				if (in($refcod,$referencias) and ($coordaca or $digit)) $txtexprealizado = '<input type="Text" size="5" value="'.$v_exprealizado.'" class="CampoEstilo" style="text-align : right; color:#808080;"  readonly="readonly" onclick="edita(\''.$refcod.'\',\''.$ptoid.'\');" onmouseover="MouseOver(this);" onmouseout="MouseOut(this);"/>'; else $txtexprealizado = $v_exprealizado;?>
			<TD align="right" style="color: #3366ff;"><?=$txtexprealizado?></TD>
			<?php	if ($ptosnsoma=='r') { if($exprealizado>-1) $totalrealizado = $exprealizado;}
			else
			{
				if ($exprealizado == -1) $exprealizado=$exprealizado+1;
				$totalrealizado = $totalrealizado + $exprealizado;
			}
	}
	}
	$colspan = 12 - $j;
	if ($colspan>0) print "<TD colspan='".$colspan."'></TD>";
	?>
			<TD align="right" style="color: #cc0000;"><?=number_format($totalrealizado, 0, '.', '');?></TD>
			<TD align="right" style="color: #0000CC;"><?=$previsto?></TD>
		</TR>
	</TBODY>
	<tr>
		<td colspan="14"
			style="height: 1px; background-color: #e5e5e5; padding: 0px"></td>
	</tr>
	<tr>
		<td colspan="14"
			style="height: 2px; background-color: #000000; padding: 0px"></td>
	</tr>
	<?php
	}
	}


	function chama_subacao($cod)
	{
		print ' uma subao'.$cod;
	}


	/**
	 * Verifica se o usuário possui algum perfil que permite manipular
	 * todas as ações. Os perfis que permitem essa manipulação são os
	 * que não possuem registros em elabrev.tprperfil.
	 *
	 * @return boolean
	 */
	function usuarioPossuiPermissaoTodasUnidades( $esquema = 'elabrev' )
	{
		$cpf = $_SESSION['usucpf'];
		$sisid = $_SESSION['sisid'];
		$sql =
				"select count(*)
				from seguranca.perfilusuario
					inner join seguranca.perfil using ( pflcod )
					left join " . $esquema . ".tprperfil using ( pflcod )
				where
					usucpf = '" . $cpf . "' and
					tprcod is null and
					sisid = " . $sisid . " ";
		return (boolean) $this->pegaUm( $sql );
	}

	/**
	 * Verifica se o usuário possui algum perfil que permite manipular
	 * alguma unidade. Os perfis que permitem essa manipulação são os
	 * que possuem registros em elabrev.tprperfil com o tprcod = 9.
	 *
	 * @return boolean
	 */
	function usuarioPossuiPermissaoAlgumaUnidade( $esquema = 'elabrev' )
	{
		$cpf = $_SESSION['usucpf'];
		$sql = "select count(*) from seguranca.perfilusuario left join " . $esquema . ".tprperfil using ( pflcod ) where usucpf = '" . $cpf . "'";
		return (boolean) $this->pegaUm( $sql );
	}

	/**
	 * Verifica se usuário pode manipular uma determinada unidade.
	 *
	 * @param string $unicod
	 * @return boolean
	 */
	function usuarioPossuiPermissaoUnidade( $unicod, $esquema = 'elabrev' )
	{
		if ( $this->usuarioPossuiPermissaoTodasUnidades( $esquema ) )
		{
			return true;
		}
		if ( !$this->usuarioPossuiPermissaoAlgumaUnidade( $esquema ) )
		{
			return false;
		}
		$unicod = str_replace( "'", "\\'", $unicod );
		$cpf = $_SESSION['usucpf'];
		$sql = "select count(*) from " . $esquema . ".usuarioresponsabilidade where usucpf = '" . $cpf . "' and rpustatus = 'A' and unicod = '" . $unicod . "'";
		return (boolean) $this->pegaUm( $sql );
	}

	/**
	 * Monta join a ser utilizado por algum outro query com as unidades do
	 * usuário. não é possível adivinhar qual o nome da tabela que conterá
	 * o unicod para fazer a restrição, portanto A RESTRICAO DO UNICOD DEVE
	 * SER ADICIONADO AO FINAL DO RETORNO.
	 *
	 * Exemplo:
	 * $join = $db->usuarioJoinUnidadesPermitidas();
	 * $join .= " and minhatabela.unicod = unijoin.unicod ";
	 *
	 * O nome das tabelas são unijoin (unidade) e usujoin
	 * (usuarioresponsabilidade). Este segundo só aparece caso o usuário não
	 * possui um perfil que permita trabalhar com todas as unidades
	 *
	 * @return string
	 */
	function usuarioJoinUnidadesPermitidas( $esquema = 'elabrev' )
	{
		$join = "";
		$podeTodas = $this->usuarioPossuiPermissaoTodasUnidades( $esquema );
		$join_exercicio = '';
		if ( $esquema == 'elabrev' ) $join_exercicio = " usujoin.prsano = '" . $_SESSION['exercicio'] . "' and ";

		if ( !$podeTodas )
		{
			$join .=
					" inner join " . $esquema . ".usuarioresponsabilidade usujoin on " .
						" usujoin.usucpf = '" . $_SESSION['usucpf'] . "' and " . $join_exercicio .
						" usujoin.rpustatus = 'A' and " .
						" usujoin.pflcod in ( " .
							" select pu.pflcod " .
							" from seguranca.perfilusuario pu" .
							" inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.sisid = " .$_SESSION['sisid'].
							" where pu.usucpf = '" . $_SESSION['usucpf'] . "'" .
						" ) ";
		}
		$join .=
				" inner join unidade unijoin on " .
					" unijoin.unicod != '26100' and " .
					" unijoin.unicod != '". CODIGO_ORGAO_SISTEMA. "' and " .
					" unijoin.orgcod = '". CODIGO_ORGAO_SISTEMA. "' ";
		if ( !$podeTodas )
		{
			$join .= " and unijoin.unicod = usujoin.unicod ";
		}
		return $join;
	}

	/**
	 * Captura os códigos da unidades que o usuário pode manipular.
	 *
	 * @return string[]
	 */
	function usuarioUnidadesPermitidas( $esquema = 'elabrev' )
	{
		if ( $this->usuarioPossuiPermissaoTodasUnidades( $esquema ) )
		{
			$sql = "
					select unicod
					from unidade
					where
						unistatus = 'A' and
						(
							( orgcod = '". CODIGO_ORGAO_SISTEMA. "' and unicod != '26100' )
							or
							( unicod = '74902' )
						)
				";
		}
		else
		{
			$sql =
				" select " .
				" distinct unicod " .
				" from " . $esquema . ".usuarioresponsabilidade " .
				" inner join unidade using ( unicod ) " .
				" inner join seguranca.perfilusuario using ( pflcod, usucpf ) " .
				" where " .
				" usucpf = '" . $_SESSION['usucpf'] . "' and " .
				" rpustatus = 'A' and " .
				" unistatus = 'A' and " .
				" orgcod = '". CODIGO_ORGAO_SISTEMA. "' and " .
				" unicod != '26100' ";
		}
		if ( !$this->usuarioPossuiPermissaoAlgumaUnidade( $esquema ) )
		{
			return array();
		}
		$dados =  $this->carregar( $sql );
		$retorno = array();
		if ( $dados )
		{
			foreach ( $dados as $dado )
			{
				array_push( $retorno, $dado['unicod'] );
			}
		}
		return $retorno;
	}

	public function alterar_status_usuario( $cpf, $status, $justificativa, $sisid = null ){
		// $sql_status é a query que altera o status em algum sistema/geral
		// $sql_cascata é a query que altear o status em alguma sistema/geral devido a alteração inicialmente definida
		// $sql_historico é a query que gera histórico para a alteração que está sendo efetuada
		// $sql_historico_cascata é a query que gera histórico para as alterações que estão sendo efetuadas em cascata
		$usucpfadm = $cpf != $_SESSION['usucpf'] ? "'" . $_SESSION['usucpf'] . "'" : 'null';
		$sql_cascata = '';
		$sql_historico_cascata = array();
		if ( $sisid ) {
			$sql_status = sprintf(
				"UPDATE seguranca.usuario_sistema SET suscod = '%s' WHERE sisid = %d AND usucpf = '%s'",
			$status,
			$sisid,
			$cpf
			);
			$sql_historico = sprintf(
				"INSERT INTO seguranca.historicousuario ( htudsc, usucpf, sisid, suscod, usucpfadm ) VALUES ( '%s', '%s', %d, '%1s', %s )",
			$justificativa,
			$cpf,
			$sisid,
			$status,
			$usucpfadm
			);
			if ( $status == 'A' ) // verifica se está alterando para ativo
			{
				$sql_status_geral = sprintf(
					"SELECT suscod FROM seguranca.usuario WHERE usucpf = '%s'",
				$cpf
				);
				$status_geral = $this->pegaUm( $sql_status_geral );
				if ( $status_geral != 'A' ) // verifica se status geral não é ativo
				{
					// altera em cascata no geral
					$sql_cascata = sprintf(
						"UPDATE seguranca.usuario SET suscod = '%s' WHERE usucpf = '%s'",
					$status,
					$cpf
					);
					$sql_historico_cascata = array( sprintf(
						"INSERT INTO seguranca.historicousuario ( htudsc, usucpf, suscod, usucpfadm ) VALUES ( '%s', '%s', '%1s', %s )",
					$justificativa,
					$cpf,
					$status,
					$usucpfadm
					) );
				}
			}
		} else {
			$sql_status = sprintf(
				"UPDATE seguranca.usuario SET suscod = '%s' WHERE usucpf = '%s'",
			$status,
			$cpf
			);
			$sql_historico = sprintf(
				"INSERT INTO seguranca.historicousuario ( htudsc, usucpf, suscod, usucpfadm ) VALUES ( '%s', '%s', '%1s', %s )",
			$justificativa,
			$cpf,
			$status,
			$usucpfadm
			);
			if ( $status != 'A' )
			{
				// altera em cascata para todos os sistemas
				$sql_cascata = sprintf(
					"UPDATE seguranca.usuario_sistema SET suscod = '%s' WHERE usucpf = '%s'",
				$status,
				$cpf
				);
				// captura os sistemas do usuário
				$sql_sistemas_usuario = sprintf(
					"SELECT DISTINCT sisid, suscod FROM seguranca.usuario_sistema WHERE usucpf = '%s'",
				$cpf
				);
				$sistemas = $this->carregar( $sql_sistemas_usuario );
				$sistemas = $sistemas ? $sistemas : array();
				foreach ( $sistemas as $sistema )
				{
					if ( $sistema['suscod'] == $status ) // verifica se precisa gerar historico
					{
						continue;
					}
					array_push(
					$sql_historico_cascata,
					sprintf(
						"INSERT INTO seguranca.historicousuario ( htudsc, usucpf, sisid, suscod, usucpfadm ) VALUES ( '%s', '%s', %d, '%1s', %s )",
					$justificativa,
					$cpf,
					$sistema['sisid'],
					$status,
					$usucpfadm
					)
					);
				}
			}
		}
		$this->executar( $sql_status );
		if ( $sql_cascata != '' )
		{
			$this->executar( $sql_cascata );
		}
		if ( count( $sql_historico_cascata ) > 0 )
		{
			foreach ( $sql_historico_cascata as $sql )
			{
				$this->executar( $sql );
			}
		}

		$this->executar( $sql_historico );

	}

	public function gerar_senha(){
		$dicionario = array(
			'a','b','c','d','e','f','g','h','i','j','l','m','n','o','p','q','r','s','t','u','v','x','z'
			);
			$j = rand( 4, 6 );
			$senha = '';
			for( $i=0; $i < $j ;$i++ ) {
				$senha .= $dicionario[array_rand( $dicionario )];
			}
		$dicionarionumeros = array('1','2','3','4','5','6','7','8','9','0');
			$j = rand( 2, 3 );
			for( $i=0; $i < $j ;$i++ ) {
				$senha .= $dicionarionumeros[array_rand( $dicionarionumeros )];
			}

			return $senha;
	}
}

class criptografia {
	function rc4($pwd, $data, $case) {
		if ($case == 'de') {
			$data = urldecode($data);
		}
		$key[] = "";
		$box[] = "";
		$temp_swap = "";
		$pwd_length = 0;
		$pwd_length = strlen($pwd);
		for ($i = 0; $i <= 255; $i++) {
			$key[$i] = ord(substr($pwd, ($i % $pwd_length), 1));
			$box[$i] = $i;
		}
		$x = 0;
		for ($i = 0; $i <= 255; $i++) {
			$x = ($x + $box[$i] + $key[$i]) % 256;
			$temp_swap = $box[$i];
			$box[$i] = $box[$x];
			$box[$x] = $temp_swap;
		}
		$temp = "";
		$k = "";
		$cipherby = "";
		$cipher = "";
		$a = 0;
		$j = 0;
		for ($i = 0; $i < strlen($data); $i++) {
			$a = ($a + 1) % 256;
			$j = ($j + $box[$a]) % 256;
			$temp = $box[$a];
			$box[$a] = $box[$j];
			$box[$j] = $temp;
			$k = $box[(($box[$a] + $box[$j]) % 256)];
			$cipherby = ord(substr($data, $i, 1)) ^ $k;
			$cipher .= chr($cipherby);
		}
		if ($case == 'de') {
			$cipher = urldecode(urlencode($cipher));
		} else {
			$cipher = urlencode($cipher);
		}
		return $cipher;
	}
	function hash_senha($senha) {
		return "{MD5}".base64_encode(pack("H*",md5($senha)));
	}
}

/**
 * Agrupadora de testes e iterador. Permite dizer se uma bateria de testes foi
 * realizada com sucesso, alm de permitir percorrer os testes realizados
 * @author Adonias Malosso <adonias@mesote.com.br>
 * @version 1.0
 */
class Testes implements Iterator {
	private $_testes;

	public function __construct() {
		$this->_testes = array();
	}

	public function addTeste(Teste $teste) {
		$this->_testes[] = $teste;
	}

	public function delTeste($pos) {
		if(isset($this->_testes[$pos]))
		unset($this->_testes[$pos]);
	}

	public function rewind() {
		reset($this->_testes);
	}

	public function current() {
		$var = current($this->_testes);
		return $var;
	}

	public function key() {
		$var = key($this->_testes);
		return $var;
	}

	public function next() {
		$var = next($this->_testes);
		return $var;
	}

	public function valid() {
		$var = $this->current() !== false;
		return $var;
	}

	public function resultado() {
		foreach ($this->_testes as $teste)
		if(!$teste->status)
		return false;

		return true;
	}
}


class  GeraExcel{

        public $countLinhas = 0;
        public $intLinha = 0;
        public $arrDados = array();

	// define parametros(init)
	function  GeraExcel(){
		global $nomeDoArquivoXls;
		$this->armazena_dados   = ""; // Armazena dados para imprimir(temporario)
		$this->nomeDoArquivoXls = $nomeDoArquivoXls; // Nome do arquivo excel
		$this->ExcelStart();
	}// fim constructor


	// Monta cabecario do arquivo(tipo xls)
	function ExcelStart(){

		//inicio do cabecario do arquivo
		$this->armazena_dados = pack( "vvvvvv", 0x809, 0x08, 0x00,0x10, 0x0, 0x0 );
	}

	// Fim do arquivo excel
	function FechaArquivo(){
		$this->armazena_dados .= pack( "vv", 0x0A, 0x00);
	}




	// monta conteudo
	//function MontaConteudoString( $excel_linha, $excel_coluna, $value){
	function MontaConteudoString( $excel_linha, $excel_coluna, $value){
		$tamanho = strlen($value);
		$this->armazena_dados .= pack( "v*", 0x0204, 8 + $tamanho, $excel_linha, $excel_coluna, 0x00, $tamanho );
		$this->armazena_dados .= $value;
        $this->countLinhas =   $this->countLinhas+1;
        $this->arrDados[$excel_linha][$excel_coluna] = utf8_encode($value);
        }//Fim, monta Col/Lin

	function MontaConteudoNumero($row, $col, $num)
	{
		$record    = 0x0203;
		$length    = 0x000E;
		$xf	= 0x0F;

		$header    = pack("vv",  $record, $length);
		$data      = pack("vvv", $row, $col, $xf);
		$xl_double = pack("d",   $num);
		$this->armazena_dados .= ($header.$data.$xl_double);
	}

	// Gera arquivo(xls)
	function GeraArquivo(){

		//Fecha arquivo(xls)
     if($this->countLinhas <= 65536){
		  $this->FechaArquivo();
		  header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		  header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		  header ( "Pragma: no-cache" );
		  header ( "Content-type: application/xls; name=$this->nomeDoArquivoXls".".xls");
		  header ( "Content-Disposition: attachment; filename=$this->nomeDoArquivoXls".".xls");
		  header ( "Content-Description: MID Gera excel" );
		print  ( $this->armazena_dados);
       }else{
          //xlsx
          $this->GeraArquivoXlsx();
       }


	}// fecha funcao

	function GeraArquivoXlsx(){

        $this->FechaArquivo();
        include "classes/xlsxwriter.class.php";

        $header = $this->arrDados[0];
        $header =  array_flip($header);
        foreach($header as $key => $cabecalho):
          $header[$key] = 'string';
        endforeach;
        unset($this->arrDados[0]);
        $this->arrDados = array_slice($this->arrDados, 0, count($this->arrDados));

        $filename = 'OFFICE_2010_'.$this->nomeDoArquivoXls.".xlsx";
        header('Content-disposition: attachment; filename="'.XLSXWriter::sanitize_filename($filename).'"');
        header("Content-Type: application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
        header('Content-Transfer-Encoding: binary');
        header('Cache-Control: must-revalidate');
        header('Pragma: public');

        $writer = new XLSXWriter();
        $writer->setAuthor('MID Gera excel');
        $writer->writeSheet($this->arrDados,'Sheet1',$header);
        $writer->writeToStdOut();
        $this->arrDados = array();


	}// fecha funcao
	# Fim da classe que gera excel
}


/**
 * Abstrai o conceito de teste atravs de nome, informao, status e msg de erro.
 *
 * @author Adonias Malosso <adonias@mesotec.com.br>
 * @example $t = new Teste("Permisso de escrita no diretrio", "/var/log", is_writeable("/var/log");
 * @version 1.0
 */
class Teste {
	const IMGOK = "<img src='../imagens/valida1.gif' border='0' title='OK'/>";
	const IMGERRO = "<img src='../imagens/valida3.gif' border='0' title='Erro'/>";
	public $nome;
	public $info;
	public $status;
	public $msg;

	public function __construct($pnome, $pinfo, $pstatus, $pmsg="") {
		$this->nome = $pnome;
		$this->info = $pinfo;
		$this->status = $pstatus;
		$this->msg = $pmsg;
	}

	public function getImagem() {
		switch ($this->status) {
			case 0:
				return self::IMGERRO;
				break;
			case 1:
				return self::IMGOK;
				break;
		}
	}
}


/**
 * Class Excel_XML
 * https://github.com/oliverschwarz/php-excel
 */
class Excel_XML
{

    /**
     * MicrosoftXML Header for Excel
     * @var string
     */
    const sHeader = "<?xml version=\"1.0\" encoding=\"%s\"?\>\n<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\" xmlns:x=\"urn:schemas-microsoft-com:office:excel\" xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\" xmlns:html=\"http://www.w3.org/TR/REC-html40\">";

    /**
     * MicrosoftXML Footer for Excel
     * @var string
     */
    const sFooter = "</Workbook>";

    /**
     * Worksheet & Data
     * @var array
     */
    private $aWorksheetData;

    /**
     * Output string
     * @var string
     */
    private $sOutput;

    /**
     * Encoding to be used
     * @var string
     */
    private $sEncoding;

    /**
     * Constructor
     *
     * Instanciates the class allowing a user-defined encoding.
     *
     * @param string $sEncoding Charset encoding to be used
     */
    public function __construct($sEncoding = 'ISO-8859-1')
    {
        $this->sEncoding = $sEncoding;
        $this->sOutput = '';
    }

    /**
     * Add a worksheet
     *
     * Creates a new worksheet and adds the given data to it.
     * @param string $title Title of worksheet
     * @param array $data 2-dimensional array of data
     */
    public function addWorksheet($title, $data)
    {
        $this->aWorksheetData[] = array(
            'title' => $this->getWorksheetTitle($title),
            'data'  => $data
        );
    }

    /**
     * Send workbook to browser
     *
     * Sends the finished workbook to the browser using PHP's header
     * directive.
     *
     * @param string $filename Filename to use for sending the workbook
     */
    public function sendWorkbook($filename)
    {
        if (!preg_match('/\.(xml|xls)$/', $filename)):
            throw new Exception('Filename mimetype must be .xml or .xls');
        endif;
        $filename = $this->getWorkbookTitle($filename);
        $this->generateWorkbook();
        if (preg_match('/\.xls$/', $filename)):
            header("Content-Type: application/vnd.ms-excel; charset=" . $this->sEncoding);
            header("Content-Disposition: inline; filename=\"" . $filename . "\"");
        else:
            header("Content-Type: application/xml; charset=" . $this->sEncoding);
            header("Content-Disposition: attachment; filename=\"" . $filename . "\"");
        endif;
        echo $this->sOutput;
    }

    /**
     * Write workbook to file
     *
     * Writes the workbook into the file/path given as a parameters.
     * The method checks whether the directory is writable and the
     * file is not existing and writes the file.
     *
     * @param string $filename Filename to use for writing (must contain mimetype)
     * @param string $path Path to use for writing [optional]
     */
    public function writeWorkbook($filename, $path = '')
    {
        $this->generateWorkbook();
        $filename = $this->getWorkbookTitle($filename);
        if (!$handle = @fopen($path . $filename, 'w+')):
            throw new Exception(sprintf("Not allowed to write to file %s", $path . $filename));
        endif;
        if (@fwrite($handle, $this->sOutput) === false):
            throw new Exception(sprintf("Error writing to file %s", $path . $filename));
        endif;
        @fclose($handle);
        return sprintf("File %s written", $path . $filename);
    }

    /**
     * Get workbook output
     *
     * Just returns the generated workbook content.
     *
     * @return string Output generated by the class
     */
    public function getWorkbook()
    {
        $this->generateWorkbook();
        return $this->sOutput;
    }

    /**
     * Compatibility: Add an array
     *
     * This method implements compatibility to version 1.1. Though using
     * self::addWorksheet surely is an improvement, nobody should be
     * forced to rewrite his code.
     *
     * @param array $data Data to be added
     */
    public function addArray($data)
    {
        $this->addWorksheet('Table1', $data);
    }

    /**
     * Compatibility: Generate XML
     *
     * This method implements compatibility to version 1.1 and generates
     * the excel file.
     *
     * @param string $filename Filename (without mimetype)
     */
    public function generateXML($filename)
    {
        $filename = $this->getWorkbookTitle($filename);
        $filename .= '.xls';
        $this->sendWorkbook($filename);
    }

    /**
     * Workbook title correction
     *
     * Corrects filename (if necessary) stripping out non-allowed
     * characters.
     *
     * @param string $filename Desired filename
     * @return string Corrected filename
     */
    private function getWorkbookTitle($filename)
    {
        return preg_replace('/[^aA-zZ0-9\_\-\.]/', '', $filename);
    }

    /**
     * Worksheet title correction
     *
     * Corrects the worksheet title (given by the user) by the allowed
     * characters by Excel.
     *
     * @param string $title Desired worksheet title
     * @return string Corrected worksheet title
     */
    private function getWorksheetTitle($title)
    {
        $title = preg_replace ("/[\\\|:|\/|\?|\*|\[|\]]/", "", $title);
        return substr ($title, 0, 31);
    }

    /**
     * Generate the workbook
     *
     * This is the main wrapper to generate the workbook.
     * It will invoke the creation of worksheets, rows and
     * columns.
     */
    private function generateWorkbook()
    {
        $this->sOutput .= stripslashes(sprintf(self::sHeader, $this->sEncoding)) . "\n";
        foreach ($this->aWorksheetData as $item):
            $this->generateWorksheet($item);
        endforeach;
        $this->sOutput .= self::sFooter;
    }

    /**
     * Generate the Worksheet
     *
     * The second wrapper generates the worksheet. When the worksheet
     * data seems to be more than the excel allowed maximum lines, the
     * array is sliced.
     *
     * @param array $item Worksheet data
     * @todo Add a security check to testify whether this is an array
     */
    private function generateWorksheet($item)
    {
        $this->sOutput .= sprintf("<Worksheet ss:Name=\"%s\">\n    <Table>\n", $item['title']);
        if (count($item['data']))
            $item['data'] = array_slice($item['data'], 0, 65536);
        foreach ($item['data'] as $k => $v):
            $this->generateRow($v);
        endforeach;
        $this->sOutput .= "    </Table>\n</Worksheet>\n";
    }

    /**
     * Generate the single row
     * @param array Item with row data
     */
    private function generateRow($item)
    {
        $this->sOutput .= "        <Row>\n";
        foreach ($item as $k => $v):
            $this->generateCell($v);
        endforeach;
        $this->sOutput .= "        </Row>\n";
    }

    /**
     * Generate the single cell
     * @param string $item Cell data
     */
    private function generateCell($item)
    {
        $type = 'String';
//        if (is_numeric($item)):
//            $type = 'Number';
//            if ($item{0} == '0' && strlen($item) > 1 && $item{1} != '.'):
//                $type = 'String';
//            endif;
//        endif;
        $item = str_replace('&#039;', '&apos;', simec_htmlspecialchars($item, ENT_QUOTES));
        $this->sOutput .= sprintf("            <Cell><Data ss:Type=\"%s\">%s</Data></Cell>\n", $type, $item);
    }

    /**
     * Deconstructor
     * Resets the main variables/objects
     */
    public function __destruct()
    {
        unset($this->aWorksheetData);
        unset($this->sOutput);
    }

}

