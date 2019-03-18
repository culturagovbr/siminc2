<?php
/**
 * Classe de mapeamento da entidade monitora.programa
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 23-11-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Monitora_Model_Programa
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Monitora_Model_Programa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'monitora.programa';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'prgid',
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
        'prgid' => null,
        'prgano' => null,
        'prgcod' => null,
        'prgdsc' => null,
        'prgstatus' => null,
    );
    
    public function recuperarSqlCombo($descricao = null, $where = [], $order = '') {

        foreach (array_keys($this->arAtributos) as $atributo) {
            if (substr($atributo, -6) == 'status') {
                $where[] = "{$atributo} = 'A'";
            } elseif (substr($atributo, -3) == 'ano') {
//                $where[] = "$atributo = '{$_SESSION['exercicio']}'";
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

        $sql = "select prgid AS codigo, {$descricao} AS descricao
                from {$this->stNomeTabela}
                $where
                order by $order   
        ";

        return $sql;
    }

    /**
     * Monta listagem de todos os programas na tela de cadastro de Programa.
     * @param stdClass $dto
     * @return array
     */
    public function buscarTodos(stdClass $dto) {

        $sql = "
            SELECT
                prgid,
                prgcod,
                prgdsc
            FROM monitora.programa
            WHERE
                prgstatus = 'A'
                AND prgano = '". (int)$dto->exercicio. "'";

        return $this->carregar($sql);
    }

    /**
     * Recuperar programa vinculado à ação na tela de cadastro de ação
     * @param stdClass $dto
     * @return array
     */
    public static function recuperaSqlComboPrgcod(stdClass $dto)
    {
        $sql = "
            SELECT
                prgcod AS codigo,
                prgcod || ' - ' || prgdsc AS descricao
            FROM monitora.programa
            WHERE
                prgstatus = 'A'
                AND prgano = '". $dto->exercicio ."'
        ";

        return $sql;
    }

}//end Class
?>