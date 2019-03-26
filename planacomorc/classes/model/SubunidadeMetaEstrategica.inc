<?php
/**
 * Classe de mapeamento da entidade planacomorc.subunidademeta_estrategica
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 26-03-2019
 * @link     no link
 */


require_once APPRAIZ . 'includes/classes/Modelo.class.inc';


/**
 * Planacomorc_Model_SubunidadeMetaEstrategica
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU siminc2.cultura.gov.br
 * @version  Release:
 * @link     no link
 */
class Planacomorc_Model_SubunidadeMetaEstrategica extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'planacomorc.subunidademeta_estrategica';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'smeid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'suoid' => array('tabela' => 'public.subunidadeorcamentaria', 'pk' => 'suoid'),
        'meeid' => array('tabela' => 'planacomorc.meta_estrategica', 'pk' => 'meeid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'smeid' => null,
        'suoid' => null,
        'meeid' => null,
    );

    /**
     * @param $exercicio
     * @return string
     * @throws Exception
     */
    public function excluirPorExercicio($exercicio)
    {
        $sql = "
            DELETE FROM {$this->stNomeTabela}
            WHERE
                meeid IN (
                    SELECT
                        meeid
                    FROM planacomorc.meta_estrategica
                    WHERE
                        prsano = '{$exercicio}'
                )
        ";

        return $this->executar($sql);
    }

    public function recuperarPorExercicio($exercicio)
    {
        $sql = "
            SELECT
                *
            FROM {$this->stNomeTabela}
            WHERE
                smeid IN(
                        SELECT
                            meeid
                        FROM planacomorc.meta_estrategica
                        WHERE
                            prsano = '{$exercicio}'
               )
       ";
        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];

        $dadosAgrupados = [];
        foreach($dados as $dado){
            $dadosAgrupados[$dado['mppid']][] = $dado['suoid'];
        }
        return $dadosAgrupados;
    }
}
