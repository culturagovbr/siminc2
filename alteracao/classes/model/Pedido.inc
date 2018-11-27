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
    public function recuperarListagem($filtroprop)
    {
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
                    AND doc.tpdid = ".$filtroprop['tpdid'];

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
            if (is_array($filtroprop['suoid'])){
                $sql .= " and ped.suoid in (".implode(' , ',$filtroprop['suoid']).')';
            }else{
                $sql .= " and ped.suoid in ".$filtroprop['suoid'];
            }
        }
//        ver($sql, $filtroprop,d);
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
    
    public function recuperarPedidoPorId($pedid){
        $sql = "SELECT
                    ped.pedid,
                    ped.pedtitulo,
                    ped.suoid,
                    ped.tpaid,
                    ped.janid,
                    tpa.tpacod,
                    tpa.tpadsc,
                    esd.esddsc,
                    jan.jannome,
                    suo.suonome,
                    case when tpa.tpafluxo='I' then 269 
                         when tpa.tpafluxo='E' then 270
                    end as tpdid
                FROM alteracao.pedido AS ped
                JOIN alteracao.tipo AS tpa ON ped.tpaid = tpa.tpaid
                JOIN alteracao.janela AS jan ON ped.janid = jan.janid
                JOIN public.subunidadeorcamentaria AS suo ON ped.suoid = suo.suoid
                LEFT JOIN workflow.documento AS doc ON ped.docid = doc.docid
                LEFT JOIN workflow.estadodocumento AS esd ON esd.esdid = doc.esdid
                WHERE
                    ped.pedstatus = 'A'
                and ped.pedid = ".$pedid;
        return $this->pegaLinha($sql);
    }
    
    public function listaPisSelecionados($pedid){
        $sql = "select pis.pliselid,
                       pis.pedid,
                       pis.pliid,
                       coalesce(pc.picvalorcusteio,0.00) AS custeio,
                       coalesce(pc.picvalorcapital,0.00) AS capital,
                       pis.vlsaldoempenhocusteio,
                       pis.vlsaldoempenhocapital,
                       pis.vlsuplementarcusteio,
                       pis.vlsuplementarcapital,
                       pis.vlcancelarcusteio,
                       pis.vlcancelarcapital,
                       pis.vlsuplementarexcessocusteio,
                       pis.vlsuplementarexcessocapital,
                       pis.vlsuplementarsuperavitcusteio,
                       pis.vlsuplementarsuperavitcapital,
                       pis.vldotacaocusteio,
                       pis.vldotacaocapital,
                       coalesce(pis.plisuplementarquantidade,0) as plisuplementarquantidade,
                       coalesce(pis.plicancelarquantidade,0) as plicancelarquantidade,
                       coalesce(pis.plisuplementarexcessoquantidade,0) as plisuplementarexcessoquantidade,
                       coalesce(pis.plisuplementarsuperavitquantidade,0) as plisuplementarsuperavitquantidade,
                       coalesce(pis.plisuplementarsuperavitquantidade,0)+coalesce(pis.plisuplementarexcessoquantidade,0)+coalesce(pis.plisuplementarquantidade,0)-coalesce(pis.plicancelarquantidade,0) as vlrprovavel,
                       coalesce((SELECT sum(coalesce(se.vlrempenhado,0::numeric)) AS vlrempenhado
		                   FROM spo.siopexecucao se
		                  where se.plicod = pli.plicod
		                    and se.exercicio = pli.pliano),0.00) as empenhado,
                       pp.pprnome,
                       suo.unonome,
                       pc.picquantidade,
                       COALESCE(pli.plititulo, 'N/A') AS plititulo,
                       TRIM(aca.prgcod) || '.' || TRIM(aca.acacod) || '.' || TRIM(aca.loccod) || '.' || (CASE WHEN LENGTH(COALESCE(aca.acaobjetivocod, '-')) <= 0 THEN '-' ELSE COALESCE(aca.acaobjetivocod, '-') END) || '.' || (CASE WHEN LENGTH(COALESCE(ptr.plocod, '-')) <= 0 THEN '-' ELSE COALESCE(ptr.plocod, '-') END) AS funcional
                  from alteracao.plano_interno_selecionado pis
                 inner join monitora.pi_planointerno pli on pis.pliid = pli.pliid 
                 inner JOIN planacomorc.pi_complemento pc on pis.pliid = pc.pliid 
                 inner join monitora.pi_produto pp on pc.pprid = pp.pprid
                  LEFT JOIN monitora.pi_planointernoptres ppt on pis.pliid = ppt.pliid
                  LEFT JOIN monitora.ptres ptr ON( ppt.ptrid = ptr.ptrid AND pli.pliano = ptr.ptrano)
                  LEFT JOIN monitora.acao aca on ptr.acaid = aca.acaid
                 inner join public.vw_subunidadeorcamentaria suo ON(
                    suo.suocod = pli.ungcod 
                    AND suo.unocod = pli.unicod 
                    AND suo.prsano = pli.pliano
                )                  
                 where pis.pedid = $pedid
                 order by pis.pliselid";
        return $this->carregar($sql);
    }
}
