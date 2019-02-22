<?php
/**
 * Classe de mapeamento da entidade public.unidade.
 *
 * $Id: Unidade.php 100919 2015-08-06 19:01:37Z maykelbraz $
 */

/**
 * Mapeamento da entidade public.unidade.
 *
 * @see Modelo
 */
class Spo_Model_Unidade extends Modelo
{
    /**
     * Código do Órgão ao qual o MEC está associado.
     */
    const CODIGO_ORGAO = CODIGO_ORGAO_SISTEMA;

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.unidade';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'unicod',
        'unitpocod',
        'unicod',
        'unitpocod',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'gstcod' => array('tabela' => 'financeiro.gestao', 'pk' => 'gstcod'),
        'gunid' => array('tabela' => 'grupounidade', 'pk' => 'gunid'),
        'orgcod, organo' => array('tabela' => 'orgao', 'pk' => 'orgcod, organo'),
        'tpocod' => array('tabela' => 'tipoorgao', 'pk' => 'tpocod'),
        'ungcodresponsavel' => array('tabela' => 'unidadegestora', 'pk' => 'ungcod'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'unicod' => null,
        'unitpocod' => null,
        'orgcod' => null,
        'organo' => null,
        'tpocod' => null,
        'uniano' => null,
        'unidsc' => null,
        'unistatus' => null,
        'uniid' => null,
        'uniabrev' => null,
        'gunid' => null,
        'ungcodresponsavel' => null,
        'gstcod' => null,
        'orgcodsupervisor' => null,
        'uniddd' => null,
        'unitelefone' => null,
        'uniemail' => null,
        'unidataatualiza' => null,
    );

    /**
     * @var string[] Lista de unidade orçamentárias obrigatórias
     */
    protected static $obrigatorias = array(
        '26101',
        '26291',
        '26290',
        '26298',
        '26443',
        '74902',
        '73107'
    );

    public static function getObrigatorias($retornarString = false, $separador = ', ')
    {
        if ($retornarString) {
            return "'" . implode("'{$separador}'", self::$obrigatorias) . "'";
        }

        return self::$obrigatorias;
    }

    public static function queryCombo(stdClass $filtro)
    {
        $filtro->unostatus = 'A';
        $where = self::montarFiltros((array)$filtro);
        
        $sql = "
            SELECT DISTINCT
                suo.unocod AS codigo,
                CASE WHEN suo.unocod='55101' THEN '55101 - MINISTERIO DA CIDADANIA - ADMINISTRAÇÃO DIRETA'
                    ELSE suo.unocod || ' - ' || unonome
                END AS descricao
            FROM vw_subunidadeorcamentaria suo
            WHERE
                {$where}
            ORDER BY
                codigo
        ";
//ver($sql,d);
        return $sql;
    }
    
    /**
     * Monta consulta para retornar Sub-Unidades.
     * 
     * @param stdClass $filtro
     * @return string
     */
    public static function queryComboSubUnidade(stdClass $filtro)
    {
        return "
            SELECT
                suo.suocod AS codigo,
                suo.suocod || ' - ' || suonome AS descricao
            FROM public.vw_subunidadeorcamentaria suo
            WHERE
                suo.suostatus = 'A'
                AND suo.prsano::INTEGER = ". (int)$filtro->exercicio. "
                {$filtro->whereUO}
            ORDER BY
                suo.suonome
        ";
    }
    
    /**
     * Monta consulta para retornar Estados.
     * 
     * @todo Refatorar constantes e colocar método em classe correta[PRAZO CURTO]
     * @param stdClass $filtro
     * @return string
     */
    public static function queryComboEstados(stdClass $filtro)
    {
        # Filtros
        $where = self::montarFiltros((array)$filtro);
        
        $sql = "
            SELECT DISTINCT
                esdid AS codigo,
                esddsc AS descricao
            FROM workflow.estadodocumento
            WHERE
                $where
            ORDER BY
                1
        ";
//ver($sql,d);
        return $sql;
    }
}