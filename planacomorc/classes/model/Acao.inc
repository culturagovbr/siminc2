<?php
/**
 * Classe de mapeamento da entidade planejamento.acao
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 16-11-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Planacomorc_Model_Acao
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Planacomorc_Model_Acao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'planejamento.acao';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'acaid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'esfcod' => array('tabela' => 'esfera', 'pk' => 'esfcod'),
        'funcod' => array('tabela' => 'public.ppafuncao', 'pk' => 'funcod'),
        'irpcod' => array('tabela' => 'identresultprimario', 'pk' => 'irpcod'),
        'prgano, prgcod, prgid' => array('tabela' => 'programa', 'pk' => 'prgano, prgcod, prgid'),
        'unicod, unitpocod' => array('tabela' => 'unidade', 'pk' => 'unicod, unitpocod'),
        'sfucod' => array('tabela' => 'public.ppasubfuncao', 'pk' => 'sfucod'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'acaid' => null,
        'prgid' => null,
        'acacod' => null,
        'acadsc' => null,
        'saccod' => null,
        'loccod' => null,
        'esfcod' => null,
        'unicod' => null,
        'unitpocod' => null,
        'funcod' => null,
        'fundsc' => null,
        'sfucod' => null,
        'sfundsc' => null,
        'prgano' => null,
        'prgcod' => null,
        'sacdsc' => null,
        'acasnmetanaocumulativa' => null,
        'acadscsituacaoatual' => null,
        'acasnrap' => null,
        'acadescricao' => null,
        'acabaselegal' => null,
        'acasntransfoutras' => null,
        'acadetalhamento' => null,
        'acastatus' => null,
        'acasnestrategica' => null,
        'acasnbgu' => null,
        'acadataatualizacao' => null,
        'irpcod' => null,
        'acatipoinclusao' => null,
        'acatipoinclusaolocalizador' => null,
        'descricaomomento' => null,
        'acatitulo' => null,
        'ididentificadorunicosiop' => null,
        'acainiciativacod' => null,
        'acainiciativadsc' => null,
        'acaobjetivocod' => null,
        'acaobjetivodsc' => null,
        'prgdsc' => null,
        'codtipoinclusaoacao' => null,
        'codtipoinclusaolocalizador' => null,
        'locquantidadeproposta' => null,
        'acanomeproduto' => null,
        'acanomeunidade' => null,
    );

    public function recuperarSqlCombo($descricao = null, $where = []) {
        $pk = $this->arChavePrimaria[0];

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

        $where = count($where) ? " where  " . implode(' and ', $where) : '';

        $sql = "select {$pk} AS codigo, {$descricao} AS descricao
                from {$this->stNomeTabela} a
                $where
                order by $descricao
        ";
        return $sql;
    }

    /**
     * Recupera lista com registros de Ação.
     * @return array|string
     */
    public function recuperarDados()
    {
        $sql = "
            SELECT DISTINCT
                aca.unicod,
                aca.funcod,
                aca.sfucod,
                aca.prgcod,
                aca.prgcod || '-' || aca.prgdsc AS programa,
                aca.acastatus,
                aca.prgano,
                aca.loccod,
                aca.acaobjetivocod,
                aca.acacod,
                aca.acaid,
                aca.acatitulo,
                aca.unicod || '-' || COALESCE((
                    SELECT
                        uni.unonome
                    FROM public.unidadeorcamentaria uni
                    WHERE
                        uni.prsano = '". (int)$_SESSION['exercicio']. "'
                        AND aca.unicod = uni.unocod
                    LIMIT 1
                ), '') AS unidade
            FROM planejamento.acao aca
                JOIN public.unidadeorcamentaria uni ON(
                    uni.unocod = aca.unicod
                    AND uni.prsano = aca.prgano
                )
            WHERE
                aca.acastatus = 'A'
                AND aca.prgano = '". (int)$_SESSION['exercicio']. "'
            ORDER BY
                aca.unicod,
                aca.prgcod,
                aca.acacod,
                aca.loccod,
                aca.acaobjetivocod,
                unidade,
                aca.acatitulo
            ";

        $resultado = $this->carregar($sql);

        $dados = $resultado? $resultado : [];
      return $dados;
    }

}
