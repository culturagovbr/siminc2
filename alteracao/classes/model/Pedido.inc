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
        'tpaid' => array('tabela' => 'alteracao.tipo', 'pk' => 'tpaid'),
        'janid' => array('tabela' => 'alteracao.janela', 'pk' => 'janid'),
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
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
        'tpaid' => null,
        'janid' => null,
        'pedstatus' => null,
        'pedano' => null,
        'usucpf' => null,
    );

    /**
     * Carrega a lista de pedidos de Alteração Orçamentária.
     *
     * @param $filtroalteracao
     * @return array|string
     */
    public function recuperarListagem($filtroalteracao)
    {
        $sql = "
            SELECT DISTINCT
                ped.pedid,
                ped.pedtitulo,
                ped.tpaid,
                ped.janid,
                tpa.tpacod,
                esd.esddsc,
                esd.esdid,
                jan.jannome,
		REPLACE(REPLACE(REPLACE(REPLACE(ARRAY(
			SELECT
				lista_suo.suocod || ' - ' || lista_suo.suonome
			FROM public.vw_subunidadeorcamentaria lista_suo
	                JOIN alteracao.pedido_unidade lista_suo_pu ON(lista_suo.suoid = lista_suo_pu.suoid and lista_suo_pu.pedunistatus='A')
			WHERE
				lista_suo_pu.pedid = ped.pedid
		)::TEXT, '{', ''), '}', ''), '\"', ''), ',', ', ') AS lista_subunidades,
                COALESCE((
                    SELECT
                        SUM(COALESCE(pisvalor.vlcancelarcusteio, 0)+ COALESCE(pisvalor.vlcancelarcapital, 0)) AS cancelado
                    FROM alteracao.plano_interno_selecionado pisvalor
                    WHERE
                        pisvalor.pedid = ped.pedid
                      and pliselstatus = 'A'
		), 0) AS cancelado,
                COALESCE((
                    SELECT
                        SUM(
                            COALESCE(pisvalor.vlsuplementarcusteio, 0)+
                            COALESCE(pisvalor.vlsuplementarcapital, 0)+
                            COALESCE(pisvalor.vlsuplementarexcessocusteio, 0)+
                            COALESCE(pisvalor.vlsuplementarexcessocapital, 0)+
                            COALESCE(pisvalor.vlsuplementarsuperavitcusteio, 0)+
                            COALESCE(pisvalor.vlsuplementarsuperavitcapital, 0)
                        ) AS suplementado
                    FROM alteracao.plano_interno_selecionado pisvalor
                    WHERE
                        pisvalor.pedid = ped.pedid
                      and pliselstatus = 'A'
		), 0) AS suplementado,
                to_char(ped.dtalteracao, 'dd/mm/yyyy') as dtalteracao,
                to_char(ped.dtalteracao, 'hh:mi') as hralteracao
            FROM alteracao.pedido AS ped
                JOIN alteracao.tipo AS tpa ON ped.tpaid = tpa.tpaid
                JOIN alteracao.janela AS jan ON ped.janid = jan.janid
                LEFT JOIN alteracao.plano_interno_selecionado AS pis ON ped.pedid = pis.pedid and pis.pliselstatus = 'A'
                LEFT JOIN monitora.vw_planointerno pli ON pis.pliid = pli.pliid
                LEFT JOIN workflow.documento AS doc ON ped.docid = doc.docid
                LEFT JOIN workflow.estadodocumento AS esd ON esd.esdid = doc.esdid
                LEFT join alteracao.pedido_unidade pu on ped.pedid = pu.pedid and pu.pedunistatus='A'
                JOIN public.vw_subunidadeorcamentaria suo ON pu.suoid = suo.suoid and suo.unostatus = 'A' and suo.prsano=ped.pedano
                --JOIN public.vw_subunidadeorcamentaria suo ON suo.suoid = pu.suoid
                ";
        $lista = pegaPerfilPorUsuario($_SESSION['usucpf']);
        foreach($lista as $value){
            $listaPerfis[] = $value['pflcod'];
        }
        if (in_array(PFL_SUBUNIDADE, $listaPerfis)){
            $sql .= " JOIN alteracao.usuarioresponsabilidade rpu on rpu.ungcod = suo.suocod and rpu.rpustatus='A' and rpu.usucpf = '".$_SESSION['usucpf']."'
                    ";
        }
        $sql .= " WHERE ped.pedstatus = 'A'
                    AND doc.tpdid IS NOT NULL \n";
        # Filtros
        $sql .= $filtroalteracao['pliid']? " AND pis.pliid = ". (int)$filtroalteracao['pliid']: NULL;
        $sql .= $filtroalteracao['plicod']? " AND pli.plicod = '". strtoupper($filtroalteracao['plicod']). "'": NULL;
        $sql .= $filtroalteracao['plititulo']? " AND pli.plititulo ILIKE '%". $filtroalteracao['plititulo']. "%'": NULL;
        $sql .= $filtroalteracao['tpdid']? " AND doc.tpdid = ". (int)$filtroalteracao['tpdid']: NULL;
        $sql .= $filtroalteracao['pedid']? " AND ped.pedid = ". (int)$filtroalteracao['pedid']: NULL;
        $sql .= $filtroalteracao['esdid']? " AND doc.esdid = ". (int)$filtroalteracao['esdid']: NULL;
        $sql .= $filtroalteracao['tpaid']? " AND ped.tpaid = ". (int)$filtroalteracao['tpaid']: NULL;
        $sql .= $filtroalteracao['janid']? " AND ped.janid = ". (int)$filtroalteracao['janid']: NULL;
        $sql .= $filtroalteracao['pedtitulo']? " AND ped.pedtitulo like '".$filtroalteracao['pedtitulo']."'": NULL;
        if($filtroalteracao['suoid']){
            if (is_array($filtroalteracao['suoid'])){
                $sql .= " AND pu.suoid in (".implode(' , ',$filtroalteracao['suoid']).')';
            }else{
                $sql .= " AND pu.suoid in ".$filtroalteracao['suoid'];
            }
        }
        if($filtroalteracao['listaSubUnidadeUsuario']){
            $sql .=  " AND rpu.ungcod in ('". implode("' , '", $filtroalteracao['listaSubUnidadeUsuario'])."')";
        }

        # Ordenação
        $sql .= "
            GROUP BY
                ped.pedid,
                ped.pedtitulo,
                ped.tpaid,
                ped.janid,
                tpa.tpacod,
                esd.esddsc,
                esd.esdid,
                jan.jannome
        ";

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
    
    public function alterarTpdidDocumento($tpdid, $docid){
        $sql = "update workflow.documento set tpdid = ".$tpdid." where docid = ".$docid;
        $this->executar($sql);
    }

    public function recuperarPedidoPorId($pedid)
    {
        $sql = "
            SELECT
                ped.pedid,
                ped.pedtitulo,
                ped.tpaid,
                ped.janid,
                tpa.tpacod,
                tpa.tpadsc,
                esd.esdid,
                esd.esddsc,
                jan.jannome,
                usr.usunome,
                '(' || usr.usufoneddd || ') ' || usr.usufonenum AS usufone,
                usr.usuemail,
                CASE WHEN tpa.tpafluxo= 'I' THEN
                    ". (int)WF_TPDID_INTERNO. "
                WHEN tpa.tpafluxo='E' THEN
                    ". (int)WF_TPDID_EXTERNO. "
                END AS tpdid,
                ped.docid
            FROM alteracao.pedido AS ped
                JOIN alteracao.tipo AS tpa ON ped.tpaid = tpa.tpaid
                JOIN alteracao.janela AS jan ON ped.janid = jan.janid
                LEFT JOIN seguranca.usuario AS usr ON usr.usucpf = ped.usucpf
                LEFT JOIN workflow.documento AS doc ON ped.docid = doc.docid
                LEFT JOIN workflow.estadodocumento AS esd ON esd.esdid = doc.esdid
            WHERE
                ped.pedstatus = 'A'
                AND ped.pedid = ". (int)$pedid;

        return $this->pegaLinha($sql);
    }

    public function listaPisSelecionados(stdClass $params){
        $sql = "
            SELECT DISTINCT
                pis.pliselid,
                pis.pedid,
                pis.pliid,
                pli.plicod,
                COALESCE(pis.vlcusteio,0.00) AS custeio,
                COALESCE(pis.vlcapital,0.00) AS capital,
                COALESCE(pis.vlfisico,0.00) AS fisico,
                pis.vlsuplementarcusteio,
                pis.vlsuplementarcapital,
                pis.vlsuplementarfisico,
                pis.vlcancelarcusteio,
                pis.vlcancelarcapital,
                pis.vlcancelarfisico,
                pis.vlsuplementarexcessocusteio,
                pis.vlsuplementarexcessocapital,
                pis.vlsuplementarexcessofisico,
                pis.vlsuplementarsuperavitcusteio,
                pis.vlsuplementarsuperavitcapital,
                pis.vlsuplementarsuperavitfisico,
                pis.vldotacaocusteio,
                pis.vldotacaocapital,
                pis.vldotacaofisico,
                COALESCE((
                    SELECT
                        SUM(COALESCE(se.vlrempenhado,0::NUMERIC)) AS vlrempenhadocusteio
                    FROM spo.siopexecucao se
                    WHERE
                        se.plicod = pli.plicod
                        AND se.exercicio = pli.pliano
                        AND gndcod::INTEGER <= 3
                ),0.00) AS vlrempenhadocusteio,
                COALESCE((
                    SELECT
                        SUM(COALESCE(se.vlrempenhado,0::NUMERIC)) AS vlrempenhadocapital
                    FROM spo.siopexecucao se
                    WHERE
                        se.plicod = pli.plicod
                        AND se.exercicio = pli.pliano
                        AND gndcod::INTEGER >= 4
                ),0.00) AS vlrempenhadocapital,
                COALESCE((
                    SELECT
                        SUM(COALESCE(se.vlrempenhado,0::NUMERIC)) AS vlrempenhado
                    FROM spo.siopexecucao se
                    WHERE
                        se.plicod = pli.plicod
                        AND se.exercicio = pli.pliano
                ),0.00) AS empenhado,
                pp.pprnome,
                (
                    SELECT
                        SUM(COALESCE(vlcancelarcusteio,0)) AS vldotacaocusteio
                    FROM alteracao.plano_interno_selecionado pisi
                        JOIN alteracao.pedido p ON pisi.pedid = p.pedid
                        JOIN workflow.documento d ON p.docid = p.docid
                    WHERE
                        p.pedstatus = 'A'
                        AND pliid = pis.pliid
                        AND pis.pedid <> ". (int)$params->pedid. "
                        AND d.esdid NOT IN(". (int)ESDID_ALTERACAO_EFETIVADO_INTERNO. ", ". (int)ESDID_ALTERACAO_EFETIVADO_EXTERNO. ")
                ) AS vltotalcancelarcusteio,
                (
                    SELECT
                        SUM(COALESCE(vlcancelarcapital,0)) AS vldotacaocapital
                    FROM alteracao.plano_interno_selecionado pisi
                        JOIN alteracao.pedido p ON pisi.pedid = p.pedid
                        JOIN workflow.documento d ON p.docid = p.docid
                    WHERE
                        p.pedstatus = 'A'
                        AND pliid = pis.pliid
                        AND pis.pedid <> ". (int)$params->pedid. "
                        AND d.esdid NOT IN(". (int)ESDID_ALTERACAO_EFETIVADO_INTERNO. ", ". (int)ESDID_ALTERACAO_EFETIVADO_EXTERNO. ")
                ) AS vltotalcancelarcapital,
                (
                    SELECT
                        SUM(COALESCE(vlcancelarfisico,0)) AS vldotacaofisico
                    FROM alteracao.plano_interno_selecionado pisi
                        JOIN alteracao.pedido p ON pisi.pedid = p.pedid
                        JOIN workflow.documento d ON p.docid = p.docid
                    WHERE
                        p.pedstatus = 'A'
                        AND pliid = pis.pliid
                        AND pis.pedid <> ". (int)$params->pedid. "
                        AND d.esdid NOT IN(". (int)ESDID_ALTERACAO_EFETIVADO_INTERNO. ", ". (int)ESDID_ALTERACAO_EFETIVADO_EXTERNO. ")
                ) AS vltotalcancelarfisico,
                suo.suonome AS unidade,
                COALESCE(pli.plititulo, 'N/A') AS plititulo,
                ptr.funcional
            FROM alteracao.plano_interno_selecionado pis
                JOIN monitora.pi_planointerno pli ON pis.pliid = pli.pliid
                JOIN planacomorc.pi_complemento pc ON pis.pliid = pc.pliid
                JOIN monitora.pi_produto pp ON pc.pprid = pp.pprid
                LEFT JOIN monitora.pi_planointernoptres ppt ON pis.pliid = ppt.pliid
                LEFT JOIN monitora.vw_ptres ptr ON(ppt.ptrid = ptr.ptrid AND pli.pliano = ptr.ptrano)
                LEFT JOIN monitora.acao aca ON ptr.acaid = aca.acaid
                LEFT JOIN public.vw_subunidadeorcamentaria suo ON(
                    pli.unicod = suo.unocod
                    AND pli.ungcod = suo.suocod
                    AND suo.unostatus = 'A'
                    AND suo.prsano = '". (int)$params->exercicio. "'
                )
            WHERE
                pis.pedid = ". (int)$params->pedid. "
                AND pis.pliselstatus = 'A'
            ORDER BY
                pis.pliselid
        ";
//ver($sql);
        return $this->carregar($sql);
    }

    /**
     * Cria sql da lista principal de PIs.
     * 
     * @param stdClass $filtros
     * @return string
     */
    public static function listarPis(stdClass $filtros){
        $where = self::montarFiltro($filtros);
        $sql = "
            SELECT DISTINCT
                pli.pliid::VARCHAR AS pliid,
                ben.benid::VARCHAR AS benid,
                pli.pliid::VARCHAR AS id,
                pli.plicod,
                ed.esdid,
                ed.esddsc,
                pli.plicod AS codigo_pi,
                pli.ungcod || '-' || suo.suonome AS sub_unidade,
                COALESCE(pli.plititulo, 'N/A') AS plititulo,
                TRIM(aca.prgcod) || '.' || TRIM(aca.acacod) || '.' || TRIM(aca.loccod) || '.' || (CASE WHEN LENGTH(COALESCE(aca.acaobjetivocod, '-')) <= 0 THEN '-' ELSE COALESCE(aca.acaobjetivocod, '-') END) || '.' || (CASE WHEN LENGTH(COALESCE(ptr.plocod, '-')) <= 0 THEN '-' ELSE COALESCE(ptr.plocod, '-') END) AS funcional,
		ed.esddsc AS situacao,
		pc.picvalorcusteio AS custeio,
		pc.picvalorcapital AS capital,
                pc.picquantidade as fisico,
                (select coalesce(pliselid,0) from alteracao.plano_interno_selecionado pis where pis.pliid=pli.pliid and pis.pedid = ".(int)$filtros->pedid." and pis.pliselstatus = 'A') as pliselid,
		coalesce((SELECT 
			sum(coalesce(se.vlrautorizado,0::numeric)) AS vlrautorizado
		   FROM spo.siopexecucao se
		  where se.plicod = pli.plicod
		    and se.exercicio = pli.pliano
		    ),0.00) as autorizado,
		coalesce((SELECT 
			sum(coalesce(se.vlrempenhado,0::numeric)) AS vlrempenhado
		   FROM spo.siopexecucao se
		  where se.plicod = pli.plicod
		    and se.exercicio = pli.pliano
		    ),0.00) as empenhado,
		coalesce((SELECT 
			sum(coalesce(se.vlrliquidado,0::numeric)) AS vlrliquidado
		   FROM spo.siopexecucao se
		  where se.plicod = pli.plicod
		    and se.exercicio = pli.pliano
		    ),0.00) as liquidado,		    
		coalesce((SELECT 
			sum(coalesce(se.vlrpago,0::numeric)) AS vlrpago
		   FROM spo.siopexecucao se
		  where se.plicod = pli.plicod
		    and se.exercicio = pli.pliano
		    ),0.00) as pago,
                pli.plistatus
            FROM monitora.pi_planointerno pli
		JOIN planacomorc.pi_complemento pc USING(pliid)
                LEFT JOIN monitora.pi_planointernoptres ppt USING(pliid)
                JOIN public.vw_subunidadeorcamentaria suo ON(
                    suo.suostatus = 'A'
                    AND pli.unicod = suo.unocod
                    AND pli.ungcod = suo.suocod
                    AND suo.prsano = pli.pliano
		)
                LEFT JOIN monitora.ptres ptr ON(
                    ppt.ptrid = ptr.ptrid
                    AND pli.pliano = ptr.ptrano)
	        LEFT JOIN monitora.acao aca on ptr.acaid = aca.acaid
		LEFT JOIN workflow.documento wd ON(pli.docid = wd.docid)
		LEFT JOIN workflow.estadodocumento ed ON(wd.esdid = ed.esdid)
		LEFT JOIN emendas.beneficiario ben ON(ben.pliid = pli.pliid)
            WHERE
                (pli.plistatus = 'A' OR (pli.plistatus = 'I' AND ed.esdid = ". (int)ESD_PI_CANCELADO. "))
                AND pli.pliano = '". (int)$filtros->exercicio. "'
                $where
            ORDER BY
                codigo_pi
        ";
//ver($sql,d);
        return $sql;
    }
    
    /**
     * Monta filtros para a consulta do método listar.
     * 
     * @param stdClass $filtros
     * @return string
     */
    public static function montarFiltro(stdClass $filtros){
        $where = "";
        # Sub-Unidades e Sub-Unidades Delegadas do Usuário.
        //$where .= self::montarFiltroSubUnidadeUsuario($filtros);
        # Código do PI.
        $where .= $filtros->plicod? "\n AND (pli.plicod = '". pg_escape_string($filtros->plicod). "' OR pli.pliid = '". (int)pg_escape_string($filtros->plicod). "') ": NULL;
        # Unidade Orçamentária.
        $where .= $filtros->unicod && !empty(join_simec(',', $filtros->unicod))? "\n AND pli.unicod::INTEGER IN(". join_simec(',', $filtros->unicod). ") ": NULL;
        # Sub-Unidade Orçamentária.
        $where .= $filtros->ungcod && !empty(join_simec(',', $filtros->ungcod))? "\n AND pli.ungcod::INTEGER IN(". join_simec(',', $filtros->ungcod). ") ": NULL;
        # PTRES - Plano de trabalho resumido.
        $where .= $filtros->ptres && !empty(join_simec(',', $filtros->ptres))? "\n AND ptr.ptres::INTEGER IN(". join_simec(',', $filtros->ptres). ") ": NULL;
        # Título ou Descrição.
        $where .= $filtros->descricao? "\n AND ( pli.plititulo ILIKE('%". pg_escape_string($filtros->descricao). "%') OR pli.plidsc ILIKE('%". pg_escape_string($filtros->descricao). "%') ) ": NULL;
        # Enquadramento.
        $where .= $filtros->eqdid && !empty(join_simec(',', $filtros->eqdid))? "\n AND pli.eqdid::INTEGER IN(". join_simec(',', $filtros->eqdid). ") ": NULL;
        # Opção da Situação.
        $where .= $filtros->esdid && !empty(join_simec(',', $filtros->esdid))? "\n AND ed.esdid::INTEGER IN(". join_simec(',', $filtros->esdid). ") ": NULL;
        # Descrição da situação.
        $where .= $filtros->esddsc? "\n AND ed.esddsc ILIKE('%". pg_escape_string($filtros->esddsc). "%') ": NULL;
        # Emenda.
        if ($filtros->pliemenda == 't') {
            $where .= "\n AND ben.pliid IS NOT NULL ";
        } elseif ($filtros->pliemenda == 'f') {
            $where .= "\n AND ben.pliid IS NULL ";
        }
        # FNC
        $where .= $filtros->unofundo? "\n AND suo.unofundo = ". $filtros->unofundo: NULL;
        # Busca pelo ID do Beneficiário
        $where .= $filtros->benid ? "\n AND ben.benid = ". $filtros->benid: NULL;
        # Busca pelas subunidades vinculadas ao código do Pedido
        $where .= $filtros->subunidade_pedido? " AND suo.suoid IN(SELECT suoid FROM alteracao.pedido_unidade WHERE pedid = ". (int)$filtros->subunidade_pedido. " and pedunistatus = 'A')": NULL;

        return $where;
    }

    /**
     * Monta o filtro de Sub-Unidades vinculadas aos perfis do Usuário da sessão.
     * 
     * @param stdClass $filtros
     * @return string
     */
    public static function montarFiltroSubUnidadeUsuario(stdClass $filtros){
        $where = "";
        # Sub-Unidades e Sub-Unidades Delegadas do Usuário.
        $listaSubUnidadeUsuario = buscarSubUnidadeUsuario($filtros);
        if($listaSubUnidadeUsuario){
            $where .= "\n AND suo.suocod::INTEGER IN(". join(',', $listaSubUnidadeUsuario). ")
            ";
        }
        
        return $where;
    }
    
    public function atualizaValoresPI($pliid, $picvalorcusteio, $picvalorcapital, $picquantidade){
        $sql = "UPDATE planacomorc.pi_complemento 
                   SET picvalorcusteio = ".(int)$picvalorcusteio.",
                       picvalorcapital = ".(int)$picvalorcapital.",
                       picquantidade = ".(int)$picquantidade."
                 WHERE pliid = ".(int)$pliid;
        $this->executar($sql);
    }

    /**
     * Verifica se o pedido está no estado do documento que pode ser editado.
     *
     * @param $esdid -> estado do documento.
     * @return bool
     */
    public static function podeEditarPedido($esdid)
    {
        if(
            (int)$esdid === ESD_EM_CADASTRAMENTO_INTERNO ||
            (int)$esdid === ESD_EM_CADASTRAMENTO_EXTERNO ||
            (int)$esdid === ESD_AGURADANDO_CORRECAO_INTERNO ||
            (int)$esdid === ESD_AGURADANDO_CORRECAO_EXTERNO
        ){
            $podeEditar = true;
        }else{
            $podeEditar = false;
        }
        return $podeEditar;
    }
}
