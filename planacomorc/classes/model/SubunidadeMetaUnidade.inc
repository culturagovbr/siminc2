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
 * Planacomorc_Model_SubunidadeMetaUnidade
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU siminc2.cultura.gov.br
 * @version  Release:
 * @link     no link
 */
class Planacomorc_Model_SubunidadeMetaUnidade extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'planacomorc.subunidade_meta_unidade';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'smuid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'suoid' => array('tabela' => 'public.subunidadeorcamentaria',        'pk' => 'suoid'),
        'meuid' => array('tabela' => 'planacomorc.meta_unidade',             'pk' => 'meuid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'smuid' => null,
        'suoid' => null,
        'meuid' => null,
    );

    /**
     * Exclui todos os vínculos de Subunidade com Meta Unidade de acordo com o exercício.
     *
     * @param $exercicio
     * @return string
     * @throws Exception
     */
    public function excluirPorExercicio($exercicio)
    {
        $sql = "
            DELETE FROM {$this->stNomeTabela}
            WHERE
                suoid IN (
                    SELECT
                        suoid
                    FROM public.vw_subunidadeorcamentaria
                    WHERE
                        suostatus = 'A'
                        AND prsano = '{$exercicio}'
                )
        ";

        return $this->executar($sql);
    }

    /**
     * Busca todos os vinculos de acordo com as Metas Unidades existentes no exercício.
     * @param $exercicio
     * @return array
     */
    public function recuperarPorExercicio($exercicio)
    {
        $sql = "
            SELECT
                *
            FROM {$this->stNomeTabela}
            WHERE
                suoid IN(
                    SELECT
                        suoid
                    FROM public.vw_subunidadeorcamentaria
                    WHERE
                        prsano = '{$exercicio}'
                        AND suostatus = 'A'
                )
       ";
        $dados = $this->carregar($sql);

        $dados = $dados ? $dados : [];

        $dadosAgrupados = [];
        foreach($dados as $dado){
            $dadosAgrupados[$dado['meuid']][] = $dado['suoid'];
        }

        return $dadosAgrupados;
    }

}
