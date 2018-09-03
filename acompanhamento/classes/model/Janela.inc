<?php
/**
 * Classe de mapeamento da entidade acompanhamento.janela
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 23-01-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Acompanhamento_Model_Janela
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_Janela extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.janela';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'janid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'tipid' => array('tabela' => 'acompanhamento.tipo', 'pk' => 'tipid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'janid' => null,
        'jandsc' => null,
        'janinicio' => null,
        'janfim' => null,
        'janstatus' => null,
        'prsano' => null,
        'tipid' => null,
    );

    public function recuperarJanelaAtiva($tipid, $exercicio = null)
    {
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        $sql = "
            SELECT
                *,
                to_char(janinicio, 'DD/MM/YYYY') janinicio,
                to_char(janfim, 'DD/MM/YYYY') janfim,
                to_char(now(), 'YYYYMMDD') BETWEEN to_char(janinicio, 'YYYYMMDD')
                AND coalesce(to_char(janfim, 'YYYYMMDD'), '". (int)date('Y'). "-12-31') atual
            FROM acompanhamento.janela
            WHERE
                prsano = '". (int)$exercicio. "'
                AND janela.janstatus = 'A'
                AND to_char(now(), 'YYYYMMDD') BETWEEN to_char(janinicio, 'YYYYMMDD')
                AND to_char(now(), 'YYYYMMDD')
                AND tipid = ". (int)$tipid. "
            ORDER BY
                janela.janinicio DESC,
                janela.janfim DESC
            LIMIT 1
        ";

        return $this->pegaLinha($sql);
    }

    public function isAtual()
    {
        $sql = "
            SELECT
                to_char(now(), 'YYYYMMDD') BETWEEN to_char(janinicio, 'YYYYMMDD') AND coalesce(to_char(janfim, 'YYYYMMDD'), '". (int)date('Y'). "-12-31') atual
            FROM acompanhamento.janela
            WHERE
                janela.janstatus = 'A'
                AND janid = ". (int)$this->janid;

        return $this->pegaUm($sql);
    }

    public function recuperarSqlCombo($where = [], $order = '') {
        $where = count($where) ? "\n WHERE \n janstatus = 'A' AND " . implode("\n AND ", $where): NULL;
        $order = $order? $order: 'descricao';
        $sql = "
            SELECT
                janid AS codigo,
                jandsc || ' - (' || TO_CHAR(janinicio, 'dd/mm/yyyy') || ' - ' || TO_CHAR(janfim, 'dd/mm/yyyy') || ')' AS descricao
            FROM acompanhamento.janela
            $where
            ORDER BY
                $order";
//ver($sql, d);
        return $sql;
    }
    
}//end Class
?>