<?php
/**
 * Classe de mapeamento da entidade planejamento.enquadramentorp
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
 * Planacomorc_Model_Enquadramentorp
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Planacomorc_Model_Enquadramentorp extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'planejamento.enquadramentorp';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'erpid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'irpcod' => array('tabela' => 'identresultprimario', 'pk' => 'irpcod'),
        'eqdid' => array('tabela' => 'pi_enquadramentodespesa', 'pk' => 'eqdid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'erpid' => null,
        'eqdid' => null,
        'irpcod' => null,
    );

    public function excluirPorExercicio($exercicio, $eqdid = null){
      if($eqdid){
        $condicao = "eqdid = ". (int)$eqdid;
      } else {
        $condicao = "
          eqdid IN(
            SELECT
              eqdid
            FROM planejamento.enquadramento_despesa
            WHERE
              eqdano = '{$exercicio}'
          )
        ";
      }

      $sql = "
          DELETE from planejamento.enquadramentorp
          WHERE
            $condicao
      ";
      return $this->executar($sql);
    }

    public function recuperarPorExercicio($exercicio)
    {
        $sql = "
            SELECT
                *
            FROM
                planejamento.enquadramentorp
            WHERE
                eqdid IN (
                    SELECT
                        eqdid
                    FROM
                        planejamento.enquadramento_despesa
                    WHERE
                        eqdano = '". (int)$exercicio. "')
        ";
        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];

        $dadosAgrupados = [];
        foreach($dados as $dado){
            $dadosAgrupados[$dado['eqdid']][] = $dado['irpcod'];
        }
        return $dadosAgrupados;
    }
}//end Class
?>
