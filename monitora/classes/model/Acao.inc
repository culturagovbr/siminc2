<?php
/**
 * Classe de mapeamento da entidade monitora.acao
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
 * Monitora_Model_Acao
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Monitora_Model_Acao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'monitora.acao';

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
        'regcod' => array('tabela' => 'regiao', 'pk' => 'regcod'),
        'tincod' => array('tabela' => 'tipoinclusaoacao', 'pk' => 'tincod'),
        'taccod' => array('tabela' => 'tipoacao', 'pk' => 'taccod'),
        'unicod, unitpocod' => array('tabela' => 'unidade', 'pk' => 'unicod, unitpocod'),
        'unmcod' => array('tabela' => 'unidademedida', 'pk' => 'unmcod'),
        'sfucod' => array('tabela' => 'public.ppasubfuncao', 'pk' => 'sfucod'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'acaid' => null,
        'acaid' => null,
        'acacod' => null,
        'saccod' => null,
        'loccod' => null,
        'esfcod' => null,
        'unicod' => null,
        'unitpocod' => null,
        'tincod' => null,
        'funcod' => null,
        'unmcod' => null,
        'regcod' => null,
        'taccod' => null,
        'osicod' => null,
        'procod' => null,
        'prgano' => null,
        'prgcod' => null,
        'prgid' => null,
        'acacodppa' => null,
        'sitcodestagio' => null,
        'sitcodandamento' => null,
        'sitcodcronograma' => null,
        'acapercexecucao' => null,
        'acadsc' => null,
        'sacdsc' => null,
        'acadsccomentarios' => null,
        'acanomecoordenador' => null,
        'acadscunresp' => null,
        'acadscunexecutora' => null,
        'acasnmedireta' => null,
        'acasnmedesc' => null,
        'acasnmelincred' => null,
        'acasnmetanaocumulativa' => null,
        'acamesinicio' => null,
        'acaanoinicio' => null,
        'acamestermino' => null,
        'acaanotermino' => null,
        'acavlrrealateanoanterior' => null,
        'acadsccomentsituacao' => null,
        'acadscsituacaoatual' => null,
        'acadscresultadosespobt' => null,
        'acamesprevisaoconclusao' => null,
        'acaanoprevisaoconclusao' => null,
        'acadsccomentexecfisica' => null,
        'acadsccomentexecfinanceira' => null,
        'acadsccomentexecfisicabgu' => null,
        'acadsccomentexecfinanceirabgu' => null,
        'acasnrap' => null,
        'acadsccomentexecucao' => null,
        'acadsccomentexecucaorap' => null,
        'acasnfiscalseguridade' => null,
        'acasninvestatais' => null,
        'acasnoutrasfontes' => null,
        'cod_referencia' => null,
        'acadscproduto' => null,
        'acafinalidade' => null,
        'acadescricao' => null,
        'acabaselegal' => null,
        'acarepercfinanceira' => null,
        'acasnpadronizada' => null,
        'acasnsetpadronizada' => null,
        'acasntransfobrigatoria' => null,
        'acasntransfvoluntaria' => null,
        'acasntransfoutras' => null,
        'acasndespesaobrigatoria' => null,
        'acasnbloqueioprogramacao' => null,
        'acadetalhamento' => null,
        'acavlrcustototal' => null,
        'acavlrcustoateanoanterior' => null,
        'acaqtdprevistoanocorrente' => null,
        'acaordemprioridade' => null,
        'acaobs' => null,
        'acacodsof' => null,
        'acaqtdcustototal' => null,
        'acacodreferenciasof' => null,
        'acavlrrepercfinanceira' => null,
        'sfucod' => null,
        'acastatus' => null,
        'acasnemenda' => null,
        'acasnestrategica' => null,
        'acaqtdateanoanterior' => null,
        'acavlrcustoprevistoanocorrente' => null,
        'acasnbgu' => null,
        'acaptres' => null,
        'acadataatualizacao' => null,
        'irpcod' => null,
        'acatipoinclusao' => null,
        'acatipoinclusaolocalizador' => null,
        'unmdsc' => null,
        'prodsc' => null,
        'descricaomomento' => null,
        'unidaderesponsavel' => null,
        'tipoinclusao' => null,
        'tipoacao' => null,
        'inicioacao' => null,
        'terminoacao' => null,
        'acatitulo' => null,
        'ididentificadorunicosiop' => null,
        'unmcodsof' => null,
        'procodsof' => null,
        'acatipocod' => null,
        'acatipodsc' => null,
        'acainiciativacod' => null,
        'acainiciativadsc' => null,
        'acaobjetivocod' => null,
        'acaobjetivodsc' => null,
        'esfdsc' => null,
        'fundsc' => null,
        'sfundsc' => null,
        'prgdsc' => null,
        'prgtipo' => null,
        'codtipoinclusaoacao' => null,
        'codtipoinclusaolocalizador' => null,
        'metalocalizador' => null,
        'financeirolocalizador' => null,
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
                acao.unicod,
                funcod,
                sfucod,
                prgcod,
                prgcod || '-' || prgdsc AS programa,
                acastatus,
                prgano,
                loccod,
                acaobjetivocod,
                acacod,
                acaid,
                acatitulo,
                acao.unicod || '-' || COALESCE((
                    SELECT
                        uni.unonome
                    FROM public.unidadeorcamentaria uni
                    WHERE
                        uni.prsano = '". (int)$_SESSION['exercicio']. "'
                        AND acao.unicod = uni.unocod
                    LIMIT 1
                ), '') AS unidade
            FROM monitora.acao acao
                JOIN public.unidadeorcamentaria uni ON(
                    uni.unocod = acao.unicod
                    AND uni.prsano = acao.prgano
                )
            WHERE
                acao.acastatus = 'A'
                AND acao.prgano = '". (int)$_SESSION['exercicio']. "'
            ORDER BY
                acao.unicod,
                prgcod,
                acacod,
                loccod,
                acaobjetivocod,
                unidade,
                acatitulo
            ";

        $resultado = $this->carregar($sql);

        $dados = $resultado? $resultado : [];
      return $dados;
    }

}
