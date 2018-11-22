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

    public function retornaListaAvaliacoes(){
        $sql = "select '<center>
                            <a href=\"javascript:alterarAvaliacao('||ag.agid||');\"><img src=\"/imagens/alterar.gif \" border=0 title=\"Visualizar\"></a>
                            <a href=\"javascript:excluirAvaliacao('||ag.agid||');\"><img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\"></a>
                        </center>',
                       ag.nuanoreferencia,
                       '' as dsmesreferencia,
                       case when ag.indavalgestor='S' then '<i style=\"font-size:2.5em\" id=\"indthumbsup\" class=\"fa fa-thumbs-up\"></i>'
                            when ag.indavalgestor='N' then '<i style=\"font-size:2.5em\" id=\"indthumbsup\" class=\"fa fa-thumbs-down\"></i>'
                            when ag.indavalgestor='E' then '<i style=\"font-size:2.5em\" id=\"indthumbsup\" class=\"fa fa-exclamation-circle\"></i>'
                       else 'Não Avaliado'
                       end as indavalgestor, 
                       ag.indobsgestor,
                       ag.indobsgestorindicador,
                       ag.agid,
                       ag.indid,
                       ag.numesreferencia,
                       ag.nucpf
                  from painel.avaliacao_gestor ag
                 where ag.indid = ".$_SESSION['indid'];
//        ver($sql,d);
        return $this->carregar($sql);
    }

    public function RetornaRegistro($agid){
        $sql = " SELECT * FROM $this->stNomeTabela WHERE {$this->arChavePrimaria[0]} = '$agid'; ";
        return $this->pegaLinha($sql);
    }
}//end Class
?>