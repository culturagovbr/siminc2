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
        switch ($tipo){
            # Somente Vinculadas
            case 'V':
                $where = "AND uo.unocod NOT IN('". (int)UNICOD_MINC. "', '". (int)UNICOD_FNC. "')";
                break;
            # Somente Administração Direta
            case 'D':
                $where = "AND uo.unocod IN('". (int)UNICOD_MINC. "')";
                break;
            # Somente Fundo
            case 'F':
                $where = "
                    AND uo.unocod IN('". (int)UNICOD_FNC. "')
                    AND uo.unofundo IS FALSE
                    UNION ALL
                    SELECT DISTINCT
                        p.ptrid,
                        p.ptres, 
                        p.acaid, 
                        p.ptrano, 
                        p.funcod, 
                        p.sfucod, 
                        p.prgcod, 
                        p.acacod, 
                        p.loccod, 
                        p.plocod, 
                        p.esfcod,  
                        uo.unocod,
                        uo.unonome,
                        uo.suocod,
                        uo.suonome,
                        uo.unofundo,
                        uo.suosigla,
                        uo.unosigla,
                        uo.unoid,
                        uo.suoid
                    FROM monitora.ptres p
                        JOIN public.vw_subunidadeorcamentaria uo ON(
                            uo.unocod = p.unicod 
                            AND uo.prsano = '". (int)$prsano. "' 
                            AND uo.suostatus = 'A'
                        )
                    WHERE
                        ptrano = '". (int)$prsano. "'
                        AND p.ptrstatus = 'A'
                        AND p.plocod NOT LIKE 'E%'
                        AND uo.unocod IN('". (int)UNICOD_FNC. "')
                        AND uo.unofundo IS TRUE
                ";
                break;
            # Todas
            default:
                $where = '';
        }

        $sql = "
            SELECT
                p.ptrid,
                p.ptres,
                p.acaid,
                p.ptrano,
                p.funcod,
                p.sfucod,
                p.prgcod,
                p.acacod,
                p.loccod,
                p.plocod,
                p.esfcod,
                uo.unocod,
                uo.unonome,
                uo.suocod,
                uo.suonome,
                uo.unofundo,
                uo.suosigla,
                uo.unosigla,
                uo.unoid,
                uo.suoid
            FROM monitora.ptres p
                JOIN public.vw_subunidadeorcamentaria uo ON(
                    uo.unocod = p.unicod
                    AND uo.prsano = '$prsano'
                    AND uo.suostatus = 'A'
                )
            WHERE
                ptrano = '$prsano'
                AND p.ptrstatus = 'A'
                AND p.plocod NOT LIKE 'E%'
                $where
            ORDER BY
                unofundo,
                suocod,
                suonome,
                unosigla,
                ptres DESC,
                prgcod,
                acacod,
                loccod,
                plocod
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
            FROM monitora.pi_enquadramentodespesa
            WHERE
                eqdstatus = 'A'
                AND eqdano::INTEGER = ". (int)$filtro->exercicio. "
            ORDER BY
                eqddsc
        ";
    }
}