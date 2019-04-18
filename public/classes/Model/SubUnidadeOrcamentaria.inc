<?php
/**
 * Classe de mapeamento da entidade public.subunidadeorcamentaria
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 16-08-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Public_Model_Subunidadeorcamentaria
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Public_Model_SubUnidadeOrcamentaria extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.subunidadeorcamentaria';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'suoid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'unoid' => array('tabela' => 'unidadeorcamentaria', 'pk' => 'unoid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'suoid' => null,
        'unoid' => null,
        'suocod' => null,
        'suosigla' => null,
        'suonome' => null,
        'suostatus' => null,
        'suoflaglimiteliberado' => null,
        'suocodigopi' => null,
    );

    /**
     * Função recuperarTodos
     * Método usado para recuperar todos os registros do banco, seguinto parâmetros
     * @param string $stCampos - String contendo nomes dos campos a serem carregados
     * @param array $arClausulas - Array contendo dados da cláusula where
     * @param string $stOrdem - String contendo dados da cláusula ordey by
     * @param array $opcoes Opções adicionais do método.
     * @example - $obNomeObjeto->recuperarTodos( 'campo1, campo2', array( "id = 123" ), nome )
     * @example - $obNomeObjeto->recuperarTodos() --> Se não passar parâmetros reconhece como todos os registros do banco.
     * @access public
     * @author Orion Teles de Mesquita
     * @since 12/02/2009
     */
    public function recuperarTodos( $stCampos = '*', $arClausulas = null, $stOrdem = null, array $opcoes = array())
    {
        // -- Retornar a query ao invés dos dados
        $opcoes['query'] = isset($opcoes['query'])?$opcoes['query']:false;
        // -- Faz join com as tabelas mapeadas nas foreing keys
        $opcoes['join'] = isset($opcoes['join'])?$opcoes['join']:false;

        $sql = "SELECT DISTINCT $stCampos FROM public.vw_subunidadeorcamentaria t1";

        $contagemTabelas = 2;
        if ($opcoes['join']) {
            if (empty($this->arChaveEstrangeira)) {
                throw new Exception("Não existe nenhuma chave estranjeira definida para a tabela {$this->stNomeTabela}.");
            }
            $sql .= $this->gerarJoin($contagemTabelas, $opcoes['join']);
        }
        if ($opcoes['leftjoin']) {
            if (empty($this->arChaveEstrangeira)) {
                throw new Exception("Não existe nenhuma chave estranjeira definida para a tabela {$this->stNomeTabela}.");
            }
            $sql .= $this->gerarJoin($contagemTabelas, $opcoes['leftjoin'], 'LEFT');
        }

        $sql .= $arClausulas ? " WHERE ". implode( " AND ", $arClausulas ) : "";
        $sql .= $stOrdem ? " ORDER BY $stOrdem" : "";

        if ($opcoes['query']) {
            return $sql;
        }

        $resultado = $this->carregar($sql);
        return $resultado ? $resultado : array();
    }

    /**
     * Monta consulta para retornar Sub-Unidades.
     *
     * @param stdClass $filtro
     * @return string
     */
    public static function queryCombo(stdClass $filtro)
    {
        $where = '';
        if($filtro->listaSubUnidadeUsuario && !empty(join_simec(',', $filtro->listaSubUnidadeUsuario))){
            $where .= "\n                AND suo.suocod::INTEGER IN(". join_simec(',', $filtro->listaSubUnidadeUsuario). ") ";
        }
        if($filtro->unicod){
            if(is_array($filtro->unicod)){
                if( !empty(join_simec(',', $filtro->unicod))){
                    $where .= "\n AND suo.unocod::INTEGER IN(". join_simec(',', $filtro->unicod). ") ";
                }
            } else {
                $where .= "\n AND suo.unocod = '". (int)$filtro->unicod. "' ";
            }
        }
        if($filtro->unofundo){
            $where .= "\n AND suo.unofundo = ". $filtro->unofundo;
        }

        $sql = "
            SELECT
                suo.suoid,
                suo.unoid,
                suo.suocod AS codigo,
                suo.suocod || ' - ' || suonome AS descricao
            FROM public.vw_subunidadeorcamentaria suo
            WHERE
                suo.suostatus = 'A'
                AND suo.prsano::INTEGER = ". (int)$filtro->exercicio. "
                $where
            ORDER BY
                suo.suonome
        ";
        // ver($sql,d);
        return $sql;
    }

    /**
     * Monta consulta para retornar Sub-Unidades.
     *
     * @param stdClass $filtro
     * @return string
     */
    public static function queryComboChaveSuoid(stdClass $filtro)
    {
        $where = '';
        if($filtro->listaSubUnidadeUsuario && !empty(join_simec(',', $filtro->listaSubUnidadeUsuario))){
            $where .= "\n AND suo.suocod::INTEGER IN(". join_simec(',', $filtro->listaSubUnidadeUsuario). ") ";
        }
        if($filtro->unicod){
            if(is_array($filtro->unicod)){
                if( !empty(join_simec(',', $filtro->unicod))){
                    $where .= "\n AND suo.unocod::INTEGER IN(". join_simec(',', $filtro->unicod). ") ";
                }
            } else {
                $where .= "\n AND suo.unocod = '". (int)$filtro->unicod. "' ";
            }
        }
        if($filtro->unofundo){
            $where .= "\n AND suo.unofundo = ". $filtro->unofundo;
        }

        $sql = "
            SELECT
                suo.suoid AS codigo,
                suo.suocod || ' - ' || suonome AS descricao
            FROM public.vw_subunidadeorcamentaria suo
            WHERE
                suo.suostatus = 'A'
                AND suo.prsano::INTEGER = ". (int)$filtro->exercicio. "
                $where
            ORDER BY
                suo.suonome
        ";
        return $sql;
    }

    /**
     * Função que retorna a lista de Unidade vinculadas ao tipo de perfil.
     *
     * @param null $usucpf
     * @param null $exercicio
     * @return array
     */
    public function recuperarPorUsuario($usucpf = null, $exercicio = null)
    {
        $perfis = pegaPerfilGeral();
        $usucpf = $usucpf ? $usucpf : $_SESSION['usucpf'];
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        # Se o perfil for de Unidade.
        if(in_array(PFL_UNIDADE, $perfis)) {
            $sql = "
                SELECT
                    suo.suoid,
                    suo.suosigla,
                    suo.suonome,
                    suo.unoid,
                    suo.unosigla,
                    suo.unonome
                FROM public.vw_subunidadeorcamentaria suo
                    JOIN acompanhamento.usuarioresponsabilidade usr ON usr.ungcod = suo.suocod
                WHERE
                    suo.prsano = '$exercicio'
                    AND suo.suostatus = 'A'
                    AND suo.unofundo = 'f'
                    AND usr.usucpf = '$usucpf'
                    AND usr.rpustatus = 'A'
                ORDER BY
                    suo.unosigla
            ";
        } else {
            $sql = "
                SELECT
                    suo.suoid,
                    suo.suosigla,
                    suo.suonome,
                    suo.unoid,
                    suo.unosigla,
                    suo.unonome
                FROM public.vw_subunidadeorcamentaria suo
                WHERE
                    suo.prsano = '$exercicio'
                    AND suo.suostatus = 'A'
                    AND suo.unofundo = 'f'
                ORDER BY
                    suo.unosigla
            ";
    }
//ver($sql,d);
        $dados = $this->carregar($sql);

        $aUnidades = [];
        foreach($dados as $dado){
            $aUnidades[$dado['suoid']] = "{$dado['unosigla']} - {$dado['suonome']} ";
        }
        return $aUnidades;
    }

    public function recuperarSqlCombo($descricao = null, $where = [], $order = '') {
        $pk = $this->arChavePrimaria[0];

        $descricao = $descricao ? $descricao : ['unosigla', 'suonome'];

        $where[] = "suostatus = 'A'";
        $where[] = "prsano = '{$_SESSION['exercicio']}'";
        
        if ($descricao) {
            $descricao = implode(" || ' - ' || ", (array)$descricao);
        }

        $order = $order ? $order : $descricao;
        $where = count($where) ? implode("\n AND ", $where) : '';

        $sql = "
            SELECT
                {$pk} AS codigo,
                {$descricao} AS descricao
            FROM public.vw_subunidadeorcamentaria
            WHERE
                $where
            ORDER BY
                $order
        ";

        return $sql;
    }
    
    /**
     * Retorna SQL da consulta pra preencher o componente de combo referente à Subunidade.
     * 
     * @param stdClass $filtro
     * @return string
     */
    public function recuperarSqlComboUnidade(stdClass $filtro) {
        $sql = "
            SELECT DISTINCT
                suo.suoid AS codigo,
                suo.unosigla || ' - ' || suo.suonome AS descricao
            FROM public.vw_subunidadeorcamentaria suo
            WHERE
                suo.prsano = '". (int)$filtro->exercicio. "'
                AND suo.suostatus = 'A'
            ORDER BY
                2
        ";
        return $sql;
    }
    
    /**
     * Monta SQL para consultar subunidades com total de pré-pi e total do limite.
     * 
     * @param stdClass $parametros
     * @return string
     */
    public function montarSqlPropostaTotaisLimites(stdClass $parametros){

        # Filtros
        $whereTotalPrePi = $parametros->aprovados? "\n AND doc.esdid = ". (int)ESD_PREPI_APROVADO: NULL;
        $where = $parametros->exercicio? "\n AND suo.prsano = '". (int)$parametros->exercicio. "'": NULL;
        $where .= $parametros->fundo === TRUE? "\n AND suo.unofundo IS TRUE ": 'AND suo.unofundo IS FALSE';
        
        $sql = "
           SELECT DISTINCT
                suo.suoid,
                suo.unoid,
                suo.unocod,
                suo.unosigla,
                suo.unonome,
                suo.suosigla,
                suo.suocod,
                suo.suonome,
                (
                    SELECT
                        COALESCE(SUM((COALESCE(pre.plivalorcusteio, 0) + COALESCE(pre.plivalorcapital, 0))), 0) AS total_pre_pi
                    FROM proposta.preplanointerno pre
                        LEFT JOIN workflow.documento doc ON(pre.docid = doc.docid)
                    WHERE
                        pre.plistatus = 'A'
                        AND pre.prsano = suo.prsano
                        AND pre.suoid = suo.suoid
                        ". $whereTotalPrePi. "
                ) AS total_pre_pi,
                (
                    SELECT
                        COALESCE(SUM(COALESCE(srl.srllimite, 0)), 0) AS total_limite_rp
                    FROM proposta.subunidaderplimite srl
                    WHERE
                        srl.srlstatus = 'A'
                        AND srl.suoid = suo.suoid
                ) AS total_limite_rp
            FROM public.vw_subunidadeorcamentaria suo
            WHERE
                suo.suostatus = 'A'
		". $where. "
            ORDER BY
                suo.unonome,
                suo.suonome,
                total_pre_pi,
                total_limite_rp
        ";
//ver($sql, d);
        return $sql;
    }
    
    /**
     * Busca subunidades com total de pré-pi e total do limite.
     * 
     * @param stdClass $parametros
     * @return array
     */
    public function buscarPropostaTotaisLimites(stdClass $parametros)
    {
        $sql = $this->montarSqlPropostaTotaisLimites($parametros);
        $resultado = $this->carregar($sql);
        $listaSubunidadeRp = $resultado? $resultado: array();
        return $listaSubunidadeRp;
    }
    
    /**
     * Monta SQL para consultar subunidades com total de pré-pi e total do limite.
     * 
     * @param stdClass $parametros
     * @return string
     */
    public function montarSqlPropostaRpTotaisLimites(stdClass $parametros){

        # Filtros PrePi
        $whereTotalPrePi = $parametros->exercicio? "\n AND pre.prsano = '". (int)$parametros->exercicio. "'": NULL;
        $whereTotalPrePi .= $parametros->suoid? "\n AND pre.suoid = ". (int)$parametros->suoid: NULL;
        
        # Filtros Limites
        $whereTotalLimite = $parametros->suoid? "\n AND srl.suoid = ". (int)$parametros->suoid: NULL;
        
        $sql = "
            SELECT
                irp.irpcod,
                irp.irpdsc,
                (
                    SELECT
                        COALESCE(SUM((COALESCE(pre.plivalorcusteio, 0) + COALESCE(pre.plivalorcapital, 0))), 0) AS total_pre_pi
                    FROM proposta.preplanointerno pre
                        JOIN monitora.ptres ptr ON pre.ptrid = ptr.ptrid -- SELECT * FROM monitora.ptres
                        LEFT JOIN workflow.documento doc ON(pre.docid = doc.docid)
                    WHERE
                        pre.plistatus = 'A'
                        AND ptr.irpcod = irp.irpcod
                        $whereTotalPrePi
                ) AS total_pre_pi,
                COALESCE((
                    SELECT
                        COALESCE(srl.srllimite, 0) AS total_limite_rp
                    FROM proposta.subunidaderplimite srl
                    WHERE
                        srl.srlstatus = 'A'
                        AND srl.irpcod = irp.irpcod
                        $whereTotalLimite
                ), 0) AS total_limite_rp
            FROM public.identresultprimario irp
            WHERE
                irp.irpstatus = 'A'
            ORDER BY
                irp.irpcod
        ";
//ver($sql, d);
        return $sql;
    }
    
    /**
     * Busca subunidades com total de pré-pi e total do limite.
     * 
     * @param stdClass $parametros
     * @return array
     */
    public function buscarPropostaRpTotaisLimites(stdClass $parametros)
    {
        $sql = $this->montarSqlPropostaRpTotaisLimites($parametros);
        $resultado = $this->carregar($sql);
        $listaSubunidadeRp = $resultado? $resultado: array();
        return $listaSubunidadeRp;
    }
    
    /**
     * Monta SQL para consultar subunidades FNC com total de pré-pi e total do limite.
     * 
     * @param stdClass $parametros
     * @return string
     */
    public function montarSqlPropostaRpTotaisLimitesFnc(stdClass $parametros){

        # Filtros PrePi
        $whereTotalPrePi = $parametros->exercicio? "\n AND pre.prsano = '". (int)$parametros->exercicio. "'": NULL;
        $whereTotalPrePi .= $parametros->suoid? "\n AND pre.suoid = ". (int)$parametros->suoid: NULL;
        
        # Filtros Limites
        $whereTotalLimite = $parametros->suoid? "\n AND srl.suoid = ". (int)$parametros->suoid: NULL;
        
        $sql = "
            SELECT
                irp.irpcod,
                irp.irpdsc,
                (
                    SELECT
                        COALESCE(SUM((COALESCE(pre.plivalorcusteio, 0) + COALESCE(pre.plivalorcapital, 0))), 0) AS total_pre_pi
                    FROM proposta.preplanointerno pre
                        JOIN monitora.ptres ptr ON pre.ptrid = ptr.ptrid -- SELECT * FROM monitora.ptres
                        LEFT JOIN workflow.documento doc ON(pre.docid = doc.docid)
                    WHERE
                        pre.plistatus = 'A'
                        AND ptr.irpcod = irp.irpcod
                        $whereTotalPrePi
                ) AS total_pre_pi,
                COALESCE((
                    SELECT
                        COALESCE(srl.srllimite, 0) AS total_limite_rp
                    FROM proposta.subunidaderplimite srl
                    WHERE
                        srl.srlstatus = 'A'
                        AND srl.irpcod = irp.irpcod
                        $whereTotalLimite
                ), 0) AS total_limite_rp
            FROM public.identresultprimario irp
            WHERE
                irp.irpstatus = 'A'
            ORDER BY
                irp.irpcod
        ";
//ver($sql, d);
        return $sql;
    }
    
    /**
     * Busca subunidades FNC com total de pré-pi e total do limite.
     * 
     * @param stdClass $parametros
     * @return array
     */
    public function buscarPropostaRpTotaisLimitesFnc(stdClass $parametros)
    {
        $sql = $this->montarSqlPropostaRpTotaisLimitesFnc($parametros);
        $resultado = $this->carregar($sql);
        $listaSubunidadeRp = $resultado? $resultado: array();
        return $listaSubunidadeRp;
    }
    

}
