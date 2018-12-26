<?php
/**
 * Classe de mapeamento da entidade painel.estrategia_meta_indicador
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO <rafael.carneiro@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 03-09-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Public_Model_Gnd
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Painel_Model_AvaliacaoGestor extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'painel.avaliacao_gestor';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'agid',
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
        'agid' => null,
        'indid' => null,
        'nuanoreferencia' => null,
        'numesreferencia' => null,
        'nucpf' => null,
        'indavalgestor' => null,
        'indobsgestor' => null,
        'indobsgestorindicador' => null,
    );

    /**
     * Retorna array de Avaliações.
     *
     * @param $indid => Indicador atual gravado na sessão.
     * @return array|string
     */
    public function retornaListaAvaliacoes($indid){

        $sql = "SELECT '<center>
                            <a href=\"javascript:alterarAvaliacao('||ag.agid||');\"><img src=\"/imagens/alterar.gif \" border=0 title=\"Visualizar\"></a>
                            <a href=\"javascript:excluirAvaliacao('||ag.agid||');\"><img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\"></a>
                        </center>',
                       ag.nuanoreferencia,
                       '' AS dsmesreferencia,
                       CASE WHEN ag.indavalgestor='S' THEN '<i style=\"font-size:2.5em\" class=\"fa fa-thumbs-up corAzul\"></i>'
                            WHEN ag.indavalgestor='N' THEN '<i style=\"font-size:2.5em\" class=\"fa fa-thumbs-down corAzul\"></i>'
                            WHEN ag.indavalgestor='E' THEN '<i style=\"font-size:2.5em\" class=\"fa fa-exclamation-circle corAzul\"></i>'
                       ELSE 'Não Avaliado'
                       END AS indavalgestor,
                       ag.indobsgestor,
                       ag.indobsgestorindicador,
                       ag.agid,
                       ag.indid,
                       ag.numesreferencia,
                       ag.nucpf
                FROM painel.avaliacao_gestor ag
                WHERE
                    ag.indid = ".$indid;

        return $this->carregar($sql);
    }

    public function RetornaRegistro($agid){
        $sql = " SELECT * FROM $this->stNomeTabela WHERE {$this->arChavePrimaria[0]} = '$agid'; ";
        return $this->pegaLinha($sql);
    }
}
