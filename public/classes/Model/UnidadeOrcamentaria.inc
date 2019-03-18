<?php
/**
 * Classe de mapeamento da entidade public.unidadeorcamentaria
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
 * Public_Model_Unidadeorcamentaria
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Public_Model_UnidadeOrcamentaria extends Modelo
{
    const SUOID_ES = 96;
    const SUOID_DS = 94;
    const SUOID_CULTURA = 88;

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.unidadeorcamentaria';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'unoid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'unoid' => null,
        'prsano' => null,
        'unocod' => null,
        'unonome' => null,
        'unostatus' => null,
        'unocodigopi' => null,
        'unofundo' => null,
        'unosigla' => null,
        'unocodorcamento'=> null,
    );
    
    public function recuperarSqlComboPorUnocod($descricao = null, $where = [], $order = '') {

        foreach (array_keys($this->arAtributos) as $atributo) {
            if (substr($atributo, -6) == 'status') {
                $where[] = "{$atributo} = 'A'";
            } elseif (substr($atributo, -3) == 'ano') {
                $where[] = "$atributo = '{$_SESSION['exercicio']}'";
            }
            if ($descricao) {
                $descricao = implode(" || ' - ' || ", (array)$descricao);
            } else {
                $aPossibilidades = ['dsc', 'desc', 'nome', 'descricao'];
                foreach ($aPossibilidades as $possibilidade) {
                    if (substr($atributo, -(strlen($possibilidade))) == $possibilidade) {
                        $descricao = $atributo;
                        break;
                    }
                }
            }
        }
        $order = $order ? $order : $descricao;
        $where = count($where) ? " where  " . implode(' and ', $where) : '';

        $sql = "
            SELECT
                unocod AS codigo,
                {$descricao} AS descricao
            FROM {$this->stNomeTabela}
                $where
                AND unoid NOT IN(". self::SUOID_ES. ", ". self::SUOID_DS. " , ". self::SUOID_CULTURA. ")
            ORDER BY
                $order   
        ";

        return $sql;
    }

    public function recuperarSqlComboUnocod($descricao = null, $where = [], $order = '') {

        foreach (array_keys($this->arAtributos) as $atributo) {
            if (substr($atributo, -6) == 'status') {
                $where[] = "{$atributo} = 'A'";
            } elseif (substr($atributo, -3) == 'ano') {
                $where[] = "$atributo = '{$_SESSION['exercicio']}'";
            }
            if ($descricao) {
                $descricao = implode(" || ' - ' || ", (array)$descricao);
            } else {
                $aPossibilidades = ['dsc', 'desc', 'nome', 'descricao'];
                foreach ($aPossibilidades as $possibilidade) {
                    if (substr($atributo, -(strlen($possibilidade))) == $possibilidade) {
                        $descricao = $atributo;
                        break;
                    }
                }
            }
        }
        $order = $order ? $order : $descricao;
        $where = count($where) ? " where  " . implode(' and ', $where) : '';

        $sql = "select unocod AS codigo, {$descricao} AS descricao
                from {$this->stNomeTabela}
                $where
                order by $order   
        ";

        return $sql;
    }
    
    /**
     * Monta SQL para consultar UO com total de pré-pi e total do limite.
     * 
     * @param stdClass $parametros
     * @return string
     */
    public function montarSqlPropostaTotaisLimitesFnc(stdClass $parametros){

        # Filtros
        $whereTotalPrePi = $parametros->aprovados? "\n AND doc.esdid = ". (int)ESD_PREPI_APROVADO: NULL;
        $where = $parametros->exercicio? "\n AND suo.prsano = '". (int)$parametros->exercicio. "'": NULL;
        $where .= $parametros->unocod? "\n AND suo.unocod = '". (int)$parametros->unocod. "'": NULL;
        
        $sql = "
            SELECT DISTINCT
                suo.unoid,
                suo.unocod,
                suo.unosigla,
                suo.unonome,
                (
                    SELECT
                        COALESCE(SUM((COALESCE(pre.plivalorcusteio, 0) + COALESCE(pre.plivalorcapital, 0))), 0) AS total_pre_pi
                    FROM proposta.preplanointerno pre
                        JOIN public.vw_subunidadeorcamentaria pre_suo ON(pre.suoid = pre_suo.suoid)
                        LEFT JOIN workflow.documento doc ON(pre.docid = doc.docid)
                    WHERE
                        pre.plistatus = 'A'
                        AND pre.prsano = suo.prsano
                        AND pre_suo.unoid = suo.unoid
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
                AND suo.unofundo IS TRUE
		". $where. "
            ORDER BY
                suo.unonome,
                total_pre_pi,
                total_limite_rp DESC
        ";
//ver($sql, d);
        return $sql;
    }
    
    /**
     * Busca UO com total de pré-pi e total do limite.
     * 
     * @param stdClass $parametros
     * @return array
     */
    public function buscarPropostaTotaisLimitesFnc(stdClass $parametros)
    {
        $sql = $this->montarSqlPropostaTotaisLimitesFnc($parametros);
        $resultado = $this->carregar($sql);
        $listaSubunidadeRp = $resultado? $resultado: array();
        return $listaSubunidadeRp;
    }
    
}//end Class
?>
