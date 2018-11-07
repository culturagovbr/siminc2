<?php
/**
 * Classe de mapeamento da entidade alteracao.pedido
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO <rafael.carneiro@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-04-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Alteracao_Model_Pedido
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO <rafael.carneiro@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Alteracao_Model_Pedido extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'alteracao.pedido';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'pedid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'docid' => array('tabela' => 'workflow.documento', 'pk' => 'docid'),
        'suoid' => array('tabela' => 'subunidadeorcamentaria', 'pk' => 'suoid'),
        'tpaid' => array('tabela' => 'alteracao.tipo', 'pk' => 'tpaid'),
        'janid' => array('tabela' => 'alteracao.janela', 'pk' => 'janid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pedid' => null,
        'pedtitulo' => null,
        'docid' => null,
        'suoid' => null,
        'tpaid' => null,
        'janid' => null,
        'pedstatus' => null,
        'pedano' => null,
    );

    /**
     * Carrega a lista de pedidos de Alteração Orçamentária.
     *
     * @param $filtroprop
     * @return array|string
     */
    public function recuperarListagem($filtroprop){
        $sql = "SELECT
                    ped.pedid,
                    ped.pedtitulo,
                    ped.suoid,
                    ped.tpaid,
                    ped.janid,
                    tpa.tpacod,
                    esd.esddsc,
                    jan.jannome,
                    suo.suonome
                FROM alteracao.pedido AS ped
                JOIN alteracao.tipo AS tpa ON ped.tpaid = tpa.tpaid
                JOIN alteracao.janela AS jan ON ped.janid = jan.janid
                JOIN public.subunidadeorcamentaria AS suo ON ped.suoid = suo.suoid
                LEFT JOIN workflow.documento AS doc ON ped.docid = doc.docid
                LEFT JOIN workflow.estadodocumento AS esd ON esd.esdid = doc.esdid
                WHERE
                    ped.pedstatus = 'A'
        ";

        if ($filtroprop['pedid']){
            $sql .= " and ped.pedid = ".$filtroprop['pedid'];
        }
        if ($filtroprop['esdid']){
            $sql .= " and doc.esdid = ".$filtroprop['esdid'];
        }
        if ($filtroprop['tpaid']){
            $sql .= " and ped.tpaid = ".$filtroprop['tpaid'];
        }
        if ($filtroprop['janid']){
            $sql .= " and ped.janid = ".$filtroprop['janid'];
        }
        if ($filtroprop['pedtitulo']){
            $sql .= " and ped.pedtitulo like '".$filtroprop['pedtitulo']."'";
        }
        if ($filtroprop['suoid']){
            $sql .= " and ped.suoid in ".implode(' AND '.$filtroprop['suoid']);
        }
        return $this->carregar($sql);
    }
    
    function pegarDocidPedido($pedid, $tipoFluxo){
        global $db;
        $sql = "select docid from alteracao.pedido where pedid = {$pedid}";
        $docid = $db->pegaUm($sql);
        if (!$docid) {
            $docid = wf_cadastrarDocumento($tipoFluxo, "Pedido {$pedid}");

            $db->executar("UPDATE alteracao.pedido SET docid = $docid where pedid = {$pedid}");
            $db->commit();
        }

        return $docid;
    }
}
