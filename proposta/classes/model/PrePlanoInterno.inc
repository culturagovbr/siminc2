<?php
/**
 * Classe de mapeamento da entidade proposta.prePlanoInterno
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <teste@teste.com.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 09-04-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Proposta_Model_PrePlanoInterno
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Proposta_Model_PrePlanoInterno extends Modelo
{
    const TIPO_CUSTEIO_NORMAL = 'Normal';
    const TIPO_CUSTEIO_EXPANSAO = 'Expansao';
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'proposta.preplanointerno';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'pliid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'docid' => array('tabela' => 'workflow.documento', 'pk' => 'docid'),
        'neeid' => array('tabela' => 'pi_niveletapaensino', 'pk' => 'neeid'),
        'mdeid' => array('tabela' => 'pi_modalidadeensino', 'pk' => 'mdeid'),
        'pumid' => array('tabela' => 'pi_unidade_medida', 'pk' => 'pumid'),
        'pprid' => array('tabela' => 'pi_produto', 'pk' => 'pprid'),
        'ipnid' => array('tabela' => 'indicadorpnc', 'pk' => 'ipnid'),
        'mpnid' => array('tabela' => 'metapnc', 'pk' => 'mpnid'),
        'ippid' => array('tabela' => 'iniciativappa', 'pk' => 'ippid'),
        'mppid' => array('tabela' => 'metappa', 'pk' => 'mppid'),
        'oppid' => array('tabela' => 'objetivoppa', 'pk' => 'oppid'),
        'ptrid' => array('tabela' => 'ptres', 'pk' => 'ptrid'),
        'masid' => array('tabela' => 'planacomorc.manutencaosubitem', 'pk' => 'masid'),
        'maiid' => array('tabela' => 'planacomorc.manutencaoitem', 'pk' => 'maiid'),
        'eqdid' => array('tabela' => 'pi_enquadramentodespesa', 'pk' => 'eqdid'),
        'suoid' => array('tabela' => 'subunidadeorcamentaria', 'pk' => 'suoid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pliid' => null,
        'plititulo' => null,
        'plidsc' => null,
        'suoid' => null,
        'eqdid' => null,
        'maiid' => null,
        'masid' => null,
        'ptrid' => null,
        'oppid' => null,
        'mppid' => null,
        'ippid' => null,
        'mpnid' => null,
        'ipnid' => null,
        'pprid' => null,
        'pumid' => null,
        'pliquantidade' => null,
        'mdeid' => null,
        'neeid' => null,
        'plivalorcusteio' => null,
        'plivalorcapital' => null,
        'docid' => null,
        'esfid' => null,
        'prsano' => null,
        'plistatus' => null,
        'plivalorcusteioadicional' => null,
        'plivalorcapitaladicional' => null,
        'pliquantidadeadicional' => null,
        'plijustificativaadicional' => null,
    );

    public function recuperarListagem(stdClass $filtros, $filtroprepi)
    {
        $where = '';
        if($filtros->listaSubUnidadeUsuario && !empty(join_simec(',', $filtros->listaSubUnidadeUsuario))){
            $where .= "\n AND suo.suocod::INTEGER IN(". join_simec(',', $filtros->listaSubUnidadeUsuario). ") ";
        }
        if($filtroprepi['suoid']) $where .= "\n AND pli.suoid = ".$filtroprepi['suoid'];
        if($filtroprepi['eqdid']) $where .= "\n AND pli.eqdid = ".$filtroprepi['eqdid'];
        if($filtroprepi['esdid']) $where .= "\n AND esd.esdid in (".implode(',',$filtroprepi['esdid']).")";
        $sql = "
            SELECT DISTINCT
                ptr.funcional,
                ptr.acatitulo,
                ptr.plodsc,
                eqd.eqddsc,
                suo.unosigla || ' - ' || suo.suonome subunidade,
                pli.*,
                esd.esddsc
            FROM proposta.preplanointerno pli
                JOIN monitora.vw_ptres ptr ON pli.ptrid = ptr.ptrid
                JOIN public.vw_subunidadeorcamentaria suo ON suo.suoid = pli.suoid
                JOIN planejamento.enquadramento_despesa eqd ON eqd.eqdid = pli.eqdid
                LEFT JOIN workflow.documento doc ON doc.docid = pli.docid
                LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
            WHERE
                plistatus = 'A'
                AND pli.prsano = '". (int)$filtros->exercicio. "'
                $where
        ";
//ver($sql, d);
        return $this->carregar($sql);
    }

    public function recuperarLocalizacao(){
        $sql = "select * 
                from  proposta.preplanointernolocalizacao
                where pliid = " . (int) $this->pliid;

        $dados = $this->carregar($sql);

        $aLocalizacoes = ['paiid'=>[], 'estuf'=>[], 'muncod'=>[]];
        foreach($dados as $dado){
            if($dado['paiid']){ $aLocalizacoes['paiid'][] = $dado['paiid']; }
            if($dado['estuf']){ $aLocalizacoes['estuf'][] = $dado['estuf']; }
            if($dado['muncod']){ $aLocalizacoes['muncod'][] = $dado['muncod']; }
        }

        return $aLocalizacoes;
    }
    
    public function recuperarLocalizacaoPi($pliid){
        $sql = "
            SELECT
                *
            FROM planacomorc.pi_localizacao
            WHERE
                pliid = " . (int)$pliid;

        $dados = $this->carregar($sql);

        $listaLocalizacoes = ['paiid'=>[], 'estuf'=>[], 'muncod'=>[]];
        foreach($dados as $dado){
            if($dado['paiid']){ $listaLocalizacoes['paiid'][] = $dado['paiid']; }
            if($dado['estuf']){ $listaLocalizacoes['estuf'][] = $dado['estuf']; }
            if($dado['muncod']){ $listaLocalizacoes['muncod'][] = $dado['muncod']; }
        }
//ver($sql, $listaLocalizacoes, d);
        return $listaLocalizacoes;
    }

    public function recuperarValoresPorSubnidadeEPtres($suoid, $ptrid)
    {
        $sql = "SELECT
                    SUM(COALESCE(pli.plivalorcapital, 0)) plivalorcapital,
                    SUM(COALESCE(pli.plivalorcusteio, 0)) plivalorcusteio,
                    SUM(COALESCE(pli.plivalorcapitaladicional, 0)) plivalorcapitaladicional,
                    SUM(COALESCE(pli.plivalorcusteioadicional, 0)) plivalorcusteioadicional
                FROM
                    proposta.preplanointerno pli
                   JOIN workflow.documento doc ON pli.docid = doc.docid
                WHERE
                    pli.suoid = ". (int)$suoid. "
                    AND pli.ptrid = ". (int)$ptrid. "
                    AND pli.plistatus = 'A'
                    AND esdid = ". (int)ESD_PREPI_APROVADO;

        return $this->pegaLinha($sql);
    }

    /**
     * Monta o Grafico com valores de custeio e capital de Pré-Pi ou expansão de custeio e capital.
     * 
     * @param string $tipoCusteio Ex: Proposta_Model_PrePlanoInterno::TIPO_CUSTEIO_EXPANSAO
     * @param array $filtro
     * @param boolean $percentual
     * @return VOID
     */
    function carregarGrafico($tipoCusteio, $filtro = NULL, $percentual = FALSE, $situacao = NULL){
        # Cores dos Graficos
        $colors = "'#FF0000', '#FFD700', '#55BF3B', '#eeaaee', '#00BFFF', '#aaeeee', '#7798BF', '#DDDF0D', '#7CCD7C', '#DF5353', '#008000', '#CD0000', '#FF4500', '#ff0066', '#4B0082', '#808000', '#800000', '#2F4F4F', '#006400', '#FFA500'";
        echo '<div class="panel-body">';

        $estatistica = $this->recuperarEstatisticaPagamento($filtro, $tipoCusteio, $situacao);
        $grafico = new Grafico(Grafico::K_TIPO_COLUNA, false);
        $grafico
            ->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)
            ->setColors($colors)
            ->setEvent(array('click' => "exibirModalDetalheGrafico(0, event.point.series.name, event.point.category);"))
            ->gerarGrafico($estatistica, $percentual);
        echo '</div>';
    }

    /**
     * Busca Unidades com estatistica de orçamento e valores de pagamento.
     * 
     * @param array $aFiltro
     * @param string $tipo Ex: Proposta_Model_PrePlanoInterno::TIPO_CUSTEIO_EXPANSAO
     * @return array
     */
    public function recuperarEstatisticaPagamento($aFiltro = NULL, $tipo, $situacao = NULL)
    {
        $filtro = '';
        $filtro .= $aFiltro['unosigla'] ? " AND suo.unosigla = '{$aFiltro['unosigla']}' ": NULL;
        $filtro .= $aFiltro['unidades'] ? " AND {$aFiltro['unidades']} ": NULL;

        $descricao .= $aFiltro['unosigla'] ? "suo.suosigla": "suo.unosigla";
        
        $prdvalor = 'prdvalor';
        $plivalorcapital = 'plivalorcapital';
        $plivalorcusteio = 'plivalorcusteio';
        if ($tipo===self::TIPO_CUSTEIO_EXPANSAO){
            $prdvalor='prdvalorexpansao';
            $plivalorcapital='plivalorcapitaladicional';
            $plivalorcusteio = 'plivalorcusteioadicional';
        }

        # Filtro de Valores(Custeio e Capital) de Pré-PI por Situação(Todas/Aprovados)esdid
        if($situacao){
            $whereCusteioCapitalPrePi = "\n AND situacao_doc.esdid = ". (int)$situacao;
        }
        
        $sql = "
            SELECT DISTINCT
                2 AS ordem,
                'Pré-pi' AS descricao,
                $descricao AS categoria,
                (sum(COALESCE (pli.".$plivalorcusteio.", 0)::DECIMAL) + sum(COALESCE (pli.".$plivalorcapital.", 0)::DECIMAL)) AS valor
            FROM public.vw_subunidadeorcamentaria suo
            LEFT JOIN(
                SELECT DISTINCT
                    situacao_pli.*
                FROM proposta.preplanointerno situacao_pli
                    LEFT JOIN workflow.documento situacao_doc ON(situacao_pli.docid = situacao_doc.docid)
                WHERE
                    situacao_pli.plistatus = 'A'
                    $whereCusteioCapitalPrePi
            ) pli ON(
                suo.suoid = pli.suoid
                AND suo.suoid = pli.suoid
                AND pli.prsano = suo.prsano
            )
            WHERE
                suo.prsano = '{$_SESSION['exercicio']}'
                AND suo.suostatus = 'A'
                AND suo.unofundo = FALSE
                $filtro
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                1 AS ordem,
                'Limite' AS descricao,
                $descricao AS categoria,
                SUM(COALESCE(srl.srllimite, 0)) AS valor
            FROM
                public.vw_subunidadeorcamentaria suo
                LEFT JOIN proposta.subunidaderplimite srl ON( -- SELECT * FROM proposta.subunidaderplimite
                    suo.suoid = srl.suoid
                    AND srl.srlstatus = 'A'
                )
            WHERE
                suo.prsano = '{$_SESSION['exercicio']}'
                AND suo.suostatus = 'A'
                AND suo.unofundo = FALSE
                $filtro
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                3 AS ordem,
                'Detalhado' AS descricao,
                $descricao AS categoria,
                sum(COALESCE (prod.".$prdvalor.", 0))::DECIMAL AS valor
            FROM
                public.vw_subunidadeorcamentaria suo
                INNER JOIN proposta.proposta pro 
                   ON suo.suoid = pro.suoid 
                  and pro.prostatus='A'
                inner join proposta.propostadetalhe prod 
                   on pro.proid = prod.proid 
                  and prod.prdstatus = 'A'
            WHERE
                suo.prsano = '{$_SESSION['exercicio']}'
                AND suo.suostatus = 'A'
                AND suo.unofundo = FALSE
                $filtro
            GROUP BY
                ordem,
                descricao,
                categoria
            ORDER BY
                ordem,
                categoria
        ";
//ver($sql, d);
        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }
    
    private function montarSQlImportarPi($pliid = NULL) {
        $sql = "
            SELECT DISTINCT
                pli.pliid,
                pli.mdeid,
                mde.mdecod,
                pli.eqdid,
                ptr.ptrid,
                eqd.eqdcod,
                pli.neeid,
                nee.neecod,
                pli.capid,
                cap.capcod,
                pli.sbaid, pli.plititulo, pli.plicodsubacao, pli.plicod, pli.plilivre, pli.plidsc, pli.unicod, pli.ungcod, pli.pliano, pli.plicadsiafi, pli.docid,
                to_char(pli.plidata, 'dd/mm/YYYY') as plidata,
                pc.oppid, pc.mppid, pc.ippid, pc.mpnid, pc.ipnid, pc.pprid, pc.pumid, pc.picquantidade, pc.picted, pc.picedital, pc.esfid, pc.picvalorcusteio, pc.picvalorcapital, pc.mescod, pc.acuid, pc.secid, pc.maiid, pc.masid, pc.picexecucao, pc.picpublico, pc.picpriorizacao, pc.pijid, pc.ptaid,
                CASE plisituacao WHEN 'A' THEN
                    'Aprovado'
                WHEN 'E' THEN
                    'Enviado para revisao'
                WHEN 'P' THEN
                    'Pendente'
                WHEN 'C' THEN
                    'Cadastrado no SIAFI'
                WHEN 'R' THEN
                    'Revisado'
                WHEN 'H' THEN
                    'Homologado'
                WHEN 'T' THEN
                    '<span style=\"color:red\">Cadastrado no SIAFI</span>'
                ELSE 'Tendencioso'
                END AS plisituacao,
                sba.sbaid, sba.sbasigla || ' - ' AS sbasigla, sba.sbacod, ben.benid, em.emenumero
            FROM monitora.pi_planointerno pli
                LEFT JOIN monitora.pi_planointernoptres ptr ON pli.pliid = ptr.pliid
                LEFT JOIN emendas.beneficiario ben ON(pli.pliid = ben.pliid)
                LEFT JOIN emendas.emenda em ON(ben.emeid = em.emeid)
                LEFT JOIN planacomorc.pi_complemento pc on pc.pliid = pli.pliid
                LEFT JOIN monitora.pi_subacao sba ON (pli.sbaid = sba.sbaid AND pli.pliano = sba.sbaano)
                LEFT JOIN planejamento.enquadramento_despesa eqd ON (pli.eqdid = eqd.eqdid AND pli.pliano = eqd.eqdano)
                LEFT JOIN monitora.pi_niveletapaensino nee ON (pli.neeid = nee.neeid AND pli.pliano = nee.neeano)
                LEFT JOIN planejamento.categoria_apropriacao cap ON (pli.capid = cap.capid AND pli.pliano = cap.capano)
                LEFT JOIN monitora.pi_modalidadeensino mde ON (pli.mdeid = mde.mdeid) --ON (pli.mdeid = mde.mdeid AND pli.pliano = mde.mdeano)
            WHERE
                pli.pliid = $pliid
        ";
//ver($sql, d);
        return $sql;
    }

    /**
     * Importa plano interno baseado do ano anterior ao de exercício
     *
     * @param $pliid
     */
    public function importarPI($pliid = NULL) {
        $sql = $this->montarSQlImportarPi($pliid);
        $pi = $this->pegaLinha($sql);

        $this->plititulo     = $pi['plititulo'];
        $this->plidsc        = $pi['plidsc'];
        $this->suoid         = $this->getSubUnidadeIDAnoAnterior($pi['unicod'], $pi['ungcod']);
        $this->eqdid         = $this->getEnquadramentoIDAnoAnterior($pi['eqdid']);
        $this->ptrid         = $this->getFuncionalIDAnoAnterior($pi['ptrid']);
        $this->pprid         = $this->getProdutoIDAnoAnterior($pi['pprid']);
        $this->pumid         = $this->getUnidadeMedidaIDAnoAnterior($pi['pumid']);
        $this->pliquantidade = $pi['picquantidade'];
        $this->oppid         = $this->getObjPPAIDAnoAnterior($pi['oppid']);
        $this->mppid         = $this->getMetasPPAIDAnoAnterior($pi['mppid']);
        $this->ippid         = $this->getIniciativaPPAIDAnoAnterior($this->oppid, $pi['ippid']);
        $this->mpnid         = $this->getMetaPNCIDAnoAnterior($pi['mpnid']);
        $this->ipnid         = $this->getIndicadorPNCIDAnoAnterior($pi['ipnid']);
        $this->mdeid         = $this->getAreaCulturalIDAnoAnterior($pi['mdeid']);
        $this->neeid         = $this->getSegmentoCulturalIDAnoAnterior($pi['neeid']);
        $this->esfid         = $pi['esfid'];
        $this->plivalorcusteio = $pi['picvalorcusteio'];
        $this->plivalorcapital = $pi['picvalorcapital'];
        $this->pliid = NULL;
    }

    /**
     * Retorna a ID do produto PI do ano atual, baseado no ano anterior
     *
     * @param $pprid
     * @return array
     */
    private function getProdutoIDAnoAnterior($pprid = NULL) {
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $sql = "
            SELECT 
                pprid
            FROM 
                monitora.pi_produto
            WHERE 
                prsano = '{$_SESSION['exercicio']}'
                AND pprstatus = 'A'
                AND pprnome = (
                    SELECT 
                        pprnome
                    FROM  monitora.pi_produto
                    WHERE 
                        prsano = '{$anoAnterior}'
                        AND pprstatus = 'A'
                        AND pprid = '". (int)$pprid. "'
              )
        ";

        return $this->pegaUm($sql);
    }

    /**
     * Retorna a ID da Unidade de Medida do ano atual, baseado no ano anterior
     *
     * @param $pumid
     * @return array
     */
    private function getUnidadeMedidaIDAnoAnterior($pumid = NULL){
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $pumid = $pumid ? $pumid : 0;
        $sql = "
            SELECT
                pumid
            FROM monitora.pi_unidade_medida
            WHERE
                prsano = '{$_SESSION['exercicio']}'
                AND pumstatus = 'A'
                AND pumdescricao = (
                    SELECT
                        pumdescricao
                    FROM monitora.pi_unidade_medida
                    WHERE
                        prsano = '{$anoAnterior}'
                        AND pumstatus = 'A'
                        AND pumid = '". (int)$pumid. "'
                )
        ";

        return $this->pegaUm($sql);
    }

    /**
     * Retorna a ID da Subunidade do ano atual, baseado no ano anterior
     *
     * @param $unocod
     * @param $suocod
     * @return array
     */
    private function getSubUnidadeIDAnoAnterior($unocod = NULL, $suocod = NULL){
        $sql = "
            SELECT DISTINCT
                suo.suoid
            FROM public.vw_subunidadeorcamentaria suo
            WHERE
                suo.suostatus = 'A'
                AND suo.prsano = '{$_SESSION['exercicio']}'
                AND suo.unocod = '{$unocod}'
                AND suo.suocod = '{$suocod}'
                AND suo.unofundo IS FALSE
        ";

        return $this->pegaUm($sql);
    }

    /**
     * Retorna a ID do campo Enquadramento da Despesa do ano atual, baseado no ano anterior
     *
     * @param $eqdid
     * @return bool|mixed|NULL|string
     */
    private function getEnquadramentoIDAnoAnterior($eqdid = NULL) {
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $sql = "
            SELECT
                eqdid,
                eqddsc
            FROM planejamento.enquadramento_despesa
            WHERE
                eqdstatus = 'A'
                AND eqdano = '{$_SESSION['exercicio']}'
                AND eqddsc = (
                    SELECT
                        eqddsc
                    FROM planejamento.enquadramento_despesa
                    WHERE
                        eqdstatus = 'A'
                        AND eqdano = '{$anoAnterior}'
                        AND eqdid = '". (int)$eqdid. "'
                )
        ";

        return $this->pegaUm($sql);
    }
    
    /**
     * Retorna a ID da Funcional do ano atual, baseado no ano anterior
     *
     * @param $ptrid
     * @return bool|mixed|NULL|string
     */
    private function getFuncionalIDAnoAnterior($ptrid = NULL) {
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $sql = "
            SELECT DISTINCT
                ptr.ptrid
            FROM monitora.ptres ptr
                JOIN monitora.acao aca ON ptr.acaid = aca.acaid
                JOIN (
                    SELECT DISTINCT
                        ptranterior.ptrid,
                        ptranterior.unicod,
                        ptranterior.prgcod,
                        ptranterior.acacod,
                        ptranterior.loccod,
                        ptranterior.plocod
                    FROM monitora.ptres ptranterior
                        JOIN monitora.acao acaanterior ON ptranterior.acaid = acaanterior.acaid
                    WHERE
                        ptranterior.ptrano = '{$anoAnterior}'
                        AND ptranterior.ptrstatus = 'A'
                ) funcionalanterior ON(
                        ptr.unicod = funcionalanterior.unicod
                        AND ptr.prgcod = funcionalanterior.prgcod
                        AND aca.acacod = funcionalanterior.acacod
                        AND ptr.loccod = funcionalanterior.loccod
                        AND ptr.plocod = funcionalanterior.plocod
                )
            WHERE
                ptr.ptrstatus = 'A'
                AND ptr.ptrano = '{$_SESSION['exercicio']}'
                AND funcionalanterior.ptrid = '". (int)$ptrid. "'
        ";

        return $this->pegaUm($sql);
    }

    /**
     * Retorna a ID do campo Objetivo PPA do ano atual, baseado no ano anterior
     *
     * @param $oppid
     * @return bool|mixed|NULL|string
     */
    private function getObjPPAIDAnoAnterior($oppid = NULL) {
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $sql = "
            SELECT
                oppid,
                oppcod || ' - ' || oppnome AS descricao,
                prsano
            FROM public.objetivoppa
            WHERE
                oppstatus = 'A'
                AND prsano = '{$_SESSION['exercicio']}'
                AND oppcod = (
                    SELECT
                        oppcod
                    FROM public.objetivoppa
                    WHERE
                        oppstatus = 'A'
                        AND prsano = '{$anoAnterior}'
                        and oppid = '". (int)$oppid. "'
                )
        ";

        return $this->pegaUm($sql);
    }

    /**
     * Retorna a ID do campo Metas PPA do ano atual, baseado no ano anterior
     *
     * @param $mppid
     * @return bool|mixed|NULL|string
     */
    private function getMetasPPAIDAnoAnterior($mppid = NULL) {
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $sql = "
            SELECT DISTINCT
                m.mppid
            FROM public.metappa m
                JOIN public.objetivometappa om ON m.mppid = om.mppid
            WHERE
                m.mppstatus = 'A'
                AND m.prsano = '{$_SESSION['exercicio']}'
                AND m.mppcod = (
                    SELECT DISTINCT
                        m.mppcod
                    FROM public.metappa m
                        JOIN public.objetivometappa om ON m.mppid = om.mppid
                    WHERE
                        m.mppstatus = 'A'
                        AND m.prsano = '{$anoAnterior}'
                        AND om.mppid = '". (int)$mppid. "'
                )
        ";

        return $this->pegaUm($sql);
    }

    /**
     * Retorna a ID do campo Iniciativa PPA do ano atual, baseado no ano anterior
     *
     * @param $oppid
     * @param $ippid
     * @return bool|mixed|NULL|string
     */
    private function getIniciativaPPAIDAnoAnterior($oppid = NULL, $ippid = NULL) {
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $ippid = $ippid ? $ippid : 0;
        $sql = "
            SELECT
                ippid
            FROM public.iniciativappa
            WHERE
                ippstatus = 'A'
                AND prsano = '{$_SESSION['exercicio']}'
                AND oppid = '".(int)$oppid. "'
                AND ippcod = (
                    SELECT
                        ippcod
                    FROM public.iniciativappa
                    WHERE
                        ippstatus = 'A'
                        AND prsano = '{$anoAnterior}'
                        AND ippid = '". (int)$ippid. "'
                )
        ";

        return $this->pegaUm($sql);
    }

    /**
     * Retorna a ID do campo Meta PNC do ano atual, baseado no ano anterior
     *
     * @param $mpnid
     * @return bool|mixed|NULL|string
     */
    private function getMetaPNCIDAnoAnterior($mpnid = NULL) {
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $sql = "
            SELECT
                mpn.mpnid AS codigo
            FROM public.metapnc mpn
            WHERE
                mpn.mpnstatus = 'A'
                AND mpn.prsano  = '{$_SESSION['exercicio']}'
                AND mpn.mpncod = (
                    SELECT
                        mpn.mpncod
                    FROM public.metapnc mpn
                    WHERE
                        mpn.mpnstatus = 'A'
                        AND mpn.prsano = '{$anoAnterior}'
                        and mpn.mpnid = '". (int)$mpnid. "'
                )	
        ";
//ver($sql, d);
        return $this->pegaUm($sql);
    }

    /**
     * Retorna a ID do campo Indicador PNC do ano atual, baseado no ano anterior
     *
     * @param $ipnid
     * @return bool|mixed|NULL|string
     */
    private function getIndicadorPNCIDAnoAnterior($ipnid = NULL) {
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $sql = "
            SELECT
                ipnid
            FROM public.indicadorpnc
            WHERE
                ipnstatus = 'A'
                AND prsano = '{$_SESSION['exercicio']}'
                AND ipncod = (
                    SELECT
                        ipncod
                    FROM public.indicadorpnc
                    WHERE
                        ipnstatus = 'A'
                        AND prsano = '{$anoAnterior}'
                        AND ipnid = '". (int)$ipnid. "'
                )
        ";

        return $this->pegaUm($sql);
    }

    /**
     * Retorna a ID do campo Área Cultural do ano atual, baseado no ano anterior
     *
     * @param $mdeid
     * @return bool|mixed|NULL|string
     */
    private function getAreaCulturalIDAnoAnterior($mdeid = NULL) {
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $sql = "
            SELECT
                MAX(mdeid),
                upper(public.removeacento(mdedsc)) as descricao
            FROM monitora.pi_modalidadeensino
            WHERE
                mdestatus = 'A'
                AND mdeano = '{$_SESSION['exercicio']}'
                AND upper(public.removeacento(mdedsc)) = (
                    SELECT
                        upper(public.removeacento(mdedsc))
                    FROM monitora.pi_modalidadeensino
                    WHERE
                        mdestatus = 'A'
                        AND mdeano = '{$anoAnterior}'
                        AND mdeid = '". (int)$mdeid. "'
                )
            GROUP BY
                descricao
        ";
        return $this->pegaUm($sql);
    }

    /**
     * Retorna a ID do campo Segmento Cultural do ano atual, baseado no ano anterior
     *
     * @param $neeid
     * @return bool|mixed|NULL|string
     */
    private function getSegmentoCulturalIDAnoAnterior($neeid = NULL) {
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $sql = "
            SELECT
                neeid AS codigo
            FROM monitora.pi_niveletapaensino
            WHERE
                neeano = '{$_SESSION['exercicio']}'
                AND neestatus = 'A'
                AND needsc = (
                    SELECT
                        needsc AS descricao
                    FROM monitora.pi_niveletapaensino
                    WHERE
                        neeano = '{$anoAnterior}'
                        AND neestatus = 'A'
                        AND neeid = '". (int)$neeid. "'
                )
        ";

        return $this->pegaUm($sql);
    }
    
    public function recuperaEstadoDocumento(){
        $sql = "select distinct esd.esdid as codigo, esd.esddsc as descricao
                  from proposta.preplanointerno pli
                 inner join workflow.documento doc
                    on pli.docid = doc.docid
                 inner join workflow.estadodocumento esd
                    on doc.esdid = esd.esdid
                 where esd.esdstatus = 'A'";
        return $sql;
    }
    
    public function recuperarExecucaoOrcamentaria($aFiltro = [], $tipo, $situacao = NULL)
    {
        $sql = $this->montarSqlExecucaoOrcamentaria($aFiltro, $tipo, $situacao);
//ver($tipo, d);
        $dados = $this->carregar($sql, $tipo);
        $dados = $dados ? $dados : [];
        $dadosAgrupados = [];
        foreach ($dados as $dado) {
            $dadosAgrupados[$dado['suocod']]['detalhe'][] = $dado;
            $dadosAgrupados[$dado['suocod']]['dotacaoCapital'] += $dado['ptrdotacao_capital'];
            $dadosAgrupados[$dado['suocod']]['dotacaoCusteio'] += $dado['ptrdotacao_custeio'];
            $dadosAgrupados[$dado['suocod']]['planejadoCapital'] += $dado['picvalorcapital'];
            $dadosAgrupados[$dado['suocod']]['planejadoCusteio'] += $dado['picvalorcusteio'];
            $dadosAgrupados[$dado['suocod']]['totalPlanejado'] = ($dadosAgrupados[$dado['suocod']]['planejadoCapital'] + $dadosAgrupados[$dado['suocod']]['planejadoCusteio']);
        }

        return $dadosAgrupados;
    }

    public function montarSqlExecucaoOrcamentaria($filtro = [], $tipo, $situacao = NULL){
        $filtro[] = "suo.suostatus = 'A'";
        $filtro[] = "ptr.irpcod <> '6'";
        $where = $this->montarFiltros($filtro);
        $prdvalor = 'prdvalor';
        $plivalorcapital = 'plivalorcapital';
        $plivalorcusteio = 'plivalorcusteio';
        if ($tipo===self::TIPO_CUSTEIO_EXPANSAO){
            $prdvalor='prdvalorexpansao';
            $plivalorcapital='plivalorcapitaladicional';
            $plivalorcusteio = 'plivalorcusteioadicional';
        }
        
        # Filtro de Valores(Custeio e Capital) de Pré-PI por Situação(Todas/Aprovados)esdid
        if($situacao){
            $whereCusteioCapitalPrePi = "\n AND situacao_doc.esdid = ". (int)$situacao;
        }
        
        $sql = "
            SELECT
                suo.suoid,
                suo.unoid,
                suo.unocod,
                suo.unosigla,
                suo.unonome,
                suo.suosigla,
                suo.suocod,
                suo.suonome,
                ptr.ptrano,
                ptr.funcional,
                ptr.acatitulo,
                ptr.irpcod,
                ptr.plodsc,
                ptr.ptrid,
                (
                    SELECT
                        SUM(COALESCE(srl.srllimite, 0)) AS limite_subunidade
                    FROM proposta.subunidaderplimite srl
                    WHERE
                        srl.srlstatus = 'A'
                        AND srl.suoid = suo.suoid
                ) AS limite_subunidade,
                psu.psuid,
                sum(COALESCE(pli.".$plivalorcapital.", 0)) ptrdotacao_capital,
                sum(COALESCE(pli.".$plivalorcusteio.", 0)) ptrdotacao_custeio,                    
                coalesce((
                    select
                        sum(case when cast(substring(ndpcus.ndpcod,1,1) as integer) < 4 then prodcus.".$prdvalor." else 0 end) custeio
                    from proposta.proposta procus
                        join proposta.propostadetalhe prodcus on procus.proid = prodcus.proid
                        JOIN public.naturezadespesa ndpcus ON prodcus.ndpid = ndpcus.ndpid
                    where
                        procus.ptrid = ptr.ptrid
                        and procus.suoid = suo.suoid
                        and procus.prostatus = 'A'
                        AND prodcus.prdstatus = 'A'
                ),0) as picvalorcusteio,
                coalesce((
                    select
                        sum(case when cast(substring(ndpcap.ndpcod,1,1) as integer) > 3 then prodcap.".$prdvalor." else 0 end) capital
                    from proposta.proposta procap
                    inner join proposta.propostadetalhe prodcap on procap.proid = prodcap.proid
                    JOIN public.naturezadespesa ndpcap ON prodcap.ndpid = ndpcap.ndpid
                    where
                        procap.ptrid = ptr.ptrid
                        and procap.suoid = suo.suoid
                        and procap.prostatus = 'A'
                        AND prodcap.prdstatus = 'A'
                ),0) as picvalorcapital,
                (
                    SELECT
                        COUNT(1)
                    FROM spo.ptressubunidade
                    WHERE
                        ptressubunidade.ptrid = ptr.ptrid
                ) AS compartilhada
            FROM public.vw_subunidadeorcamentaria suo
                JOIN spo.ptressubunidade psu ON psu.suoid = suo.suoid
                JOIN monitora.vw_ptres ptr ON(
                    ptr.ptrid = psu.ptrid
                    AND ptr.ptrano = suo.prsano
                    and ptr.ptrstatus = 'A'
                )
                LEFT JOIN(
                    SELECT DISTINCT
                        situacao_pli.*
                    FROM proposta.preplanointerno situacao_pli
                        LEFT JOIN workflow.documento situacao_doc ON(situacao_pli.docid = situacao_doc.docid)
                    WHERE
                        situacao_pli.plistatus = 'A'
                        $whereCusteioCapitalPrePi
                ) pli ON(
                    ptr.ptrid = pli.ptrid
                    AND suo.suoid = pli.suoid
                )
            WHERE
                {$where}
            GROUP BY
                suo.suoid,
                suo.unoid,
                suo.unocod,
                suo.unosigla,
                suo.unonome,
                suo.suocod,
                suo.suonome,
                suo.suosigla,
                ptr.ptrano,
                ptr.funcional,
                ptr.acatitulo,
                ptr.irpcod,
                ptr.plodsc,
                ptr.ptrid,
                limite_subunidade,
                psu.psuid
            ORDER BY
                suo.unonome,
                suo.suonome,
                ptr.funcional
        ";
//ver($sql,d);
        return $sql;
    }
    
    /**
     * Adapta os dados de Pre-Pi do módulo Proposta Orçamentária para PI do módulo Planejamento Orçamentário.
     * 
     * @return array
     */
    public function adaptarPiPropostaPlanejamento() {
        $planejamento = [];
        if($this->pliid){
            $planejamento['plititulo'] = $this->plititulo;
            $planejamento['plidsc'] = $this->plidsc;
            $planejamento['pliano'] = $this->prsano;
            # Subunidade
            $subunidade = new Public_Model_SubUnidadeOrcamentaria($this->suoid);
            $planejamento['ungcod'] = $subunidade->suocod;
            # UO
            $uo = new Public_Model_UnidadeOrcamentaria($subunidade->unoid);
            $planejamento['unicod'] = $uo->unocod;
            $planejamento['picquantidade'] = $this->pliquantidade;
            $planejamento['picvalorcusteio'] = $this->plivalorcusteio;
            $planejamento['picvalorcapital'] = $this->plivalorcapital;
            $planejamento['eqdid'] = $this->eqdid;
            $planejamento['maiid'] = $this->maiid;
            $planejamento['masid'] = $this->masid;
            $planejamento['mppid'] = $this->mppid;
            $planejamento['mdeid'] = $this->mdeid;
            $planejamento['pprid'] = $this->pprid;
            $planejamento['pumid'] = $this->pumid;
            $planejamento['oppid'] = $this->oppid;
            $planejamento['ippid'] = $this->ippid;
            $planejamento['mpnid'] = $this->mpnid;
            $planejamento['ipnid'] = $this->ipnid;
            $planejamento['neeid'] = $this->neeid;
            $planejamento['esfid'] = $this->esfid;
            $planejamento['ptrid'] = $this->ptrid;
        }
//ver($planejamento, d);
        return $planejamento;
    }
    
    /**
     * Adapta os dados de Localização do Pre-Pi do módulo Proposta Orçamentária para PI do módulo Planejamento Orçamentário.
     * 
     * @return array
     */
    public function adaptarLocalizacaoPiPropostaPlanejamento() {
        if(!$this->esfid || $this->esfid == Territorios_Model_Esfera::K_FEDERAL){
            return [];
        }

        switch($this->esfid){
            case Territorios_Model_Esfera::K_EXTERIOR:
                $sql = "
                    SELECT
                        NULL AS pilid,
                        p.paidescricao,
                        p.paiid
                    FROM proposta.preplanointernolocalizacao l
                        JOIN territorios.pais p on p.paiid = l.paiid
                    WHERE
                        l.pliid = ". (int)$this->pliid;
            break;
            case Territorios_Model_Esfera::K_ESTADUAL:
                $sql = "
                    SELECT
                        NULL AS pilid,
                        e.estuf,
                        e.estdescricao,
                        e.estcod 
                    FROM proposta.preplanointernolocalizacao l
                        JOIN territorios.estado e on e.estuf = l.estuf
                    WHERE
                        l.pliid = ". (int)$this->pliid;
            break;
            case Territorios_Model_Esfera::K_MUNICIPAL:
                $sql = "
                    SELECT
                        NULL AS pilid,
                        e.estuf,
                        e.estdescricao,
                        e.estcod,
                        m.muncod,
                        m.mundescricao
                    FROM proposta.preplanointernolocalizacao l
                        JOIN territorios.municipio m on m.muncod = l.muncod
                        JOIN territorios.estado e on e.estuf = m.estuf
                    WHERE
                        l.pliid = ". (int)$this->pliid;
            break;
        }

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }

}

