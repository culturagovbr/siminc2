<?php
/**
 * Classe de mapeamento da entidade monitora.ptres.
 *
 * $Id: Ptres.php 100401 2015-07-22 21:06:58Z maykelbraz $
 */

/**
 * Mapeamento da entidade monitora.ptres.
 *
 * @see Modelo
 */
class Spo_Model_Ptres extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'monitora.ptres';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'ptrid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'acaid' => array('tabela' => 'acao', 'pk' => 'acaid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'ptrid' => null,
        'ptres' => null,
        'acaid' => null,
        'ptrano' => null,
        'funcod' => null,
        'sfucod' => null,
        'prgcod' => null,
        'acacod' => null,
        'loccod' => null,
        'unicod' => null,
        'irpcod' => null,
        'ptrdotacao' => null,
        'ptrstatus' => null,
        'ptrdata' => null,
        'plocod' => null,
        'esfcod' => null,
    );

    public static function queryCombo(stdClass $filtro)
    {
        $where = '';
        if($filtro->listaSubUnidadeUsuario){
            $where = "\n                AND suo.suocod::INTEGER IN(". join(',', $filtro->listaSubUnidadeUsuario). ") ";
        }

        $where .= "\n                AND pt.ptrstatus = 'A' ";

        $sql = "
            SELECT
                pt.ptrid AS codigo,
                '(PTRES:'||pt.ptres||') - '|| aca.unicod ||'.'|| aca.prgcod ||'.'|| aca.acacod AS descricao
            FROM monitora.ptres pt
                JOIN monitora.acao aca USING(acaid)
                LEFT JOIN spo.ptressubunidade ps USING(ptrid) -- SELECT * FROM spo.ptressubunidade
                LEFT JOIN public.vw_subunidadeorcamentaria suo USING(suoid) -- SELECT * FROM public.vw_subunidadeorcamentaria
            WHERE
                aca.prgano::INTEGER = ". (int)$filtro->exercicio. "
                AND pt.ptrano::INTEGER = ". (int)$filtro->exercicio. "
                AND aca.acasnrap = false
                {$where}
            GROUP BY
                codigo,
                descricao
            ORDER BY
                1
        ";
//ver($sql,d);
        return $sql;
    }

    public function recuperarPtresSubunidade($prsano, $tipo = null)
    {
        $sql = "
            SELECT DISTINCT
                ptr.ptrid,
                ptr.ptres,
                ptr.unicod,
                ptr.prgcod,
                ptr.acacod,
                ptr.loccod,
                ptr.plocod,
                case when uo.unocod = '". (int)UNICOD_MC. "' then '". (int)UNICOD_MC. "-MINISTÉRIO DA CIDADANIA' else uo.unocod || '-' || uo.unonome end AS unidade_orcamentaria,
                case when uo.unocod IN ('". (int)UNICOD_MC. "', '". (int)UNICOD_MINC. "', '". (int)UNICOD_ES. "') then 'D'
                     when uo.unocod IN('". (int)UNICOD_FNC. "', '". (int)UNICOD_FRGPS. "') then 'F'
                     else 'V'
                end as tipo,
                vptr.plodsc,
                uo.unocod,
                uo.unofundo,
                vptr.acatitulo,
                vptr.funcional,
                vptr.irpcod,
                ptr.ptrdotacaocusteio,
                ptr.ptrdotacaocapital
                
            FROM monitora.ptres ptr
                JOIN public.vw_subunidadeorcamentaria uo ON(
                    uo.suostatus = 'A'
                    AND ptr.unicod = uo.unocod
                    AND ptr.ptrano = uo.prsano
                )
                JOIN monitora.vw_ptres vptr ON ptr.ptrid = vptr.ptrid
            WHERE
                ptr.ptrstatus = 'A'
                AND ptr.ptrano = '". (int)$prsano. "' 
                and ptr.ptres <> '0'
                and ptr.plocod not in (select plocod from monitora.ptres where plocod like 'E%')
            ORDER BY
                ptr.ptres DESC,
                uo.unocod,
                ptr.acacod,
                vptr.acatitulo,
                vptr.funcional,
                ptr.plocod,
                vptr.plodsc,
                vptr.irpcod
        ";
//ver($sql,d);
        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }
    
    /**
     * Monta consulta para retornar Enquadramento.
     * 
     * @param stdClass $filtro
     * @return string
     */
    public static function queryComboEnquadramento(stdClass $filtro)
    {
        return "
            SELECT
                eqdid AS codigo,
                eqddsc AS descricao
            FROM planejamento.programa
            WHERE
                eqdstatus = 'A'
                AND eqdano::INTEGER = ". (int)$filtro->exercicio. "
            ORDER BY
                eqddsc
        ";
    }
    
    public function recuperarPtridSubUnidade(){
        $sql = "select ptrid, suoid from spo.ptressubunidade";
        $arrptrid = $this->carregar($sql);
        $arrRetorno;
        for ($i=0;$i<count($arrptrid);$i++){
            $arrRetorno[$arrptrid[$i]['ptrid']][] = $arrptrid[$i]['suoid'];
        }
        return $arrRetorno;
    }
}