<?php

/**
 * Classe de mapeamento da entidade acompanhamento.acompanhamento_pnc
 */

require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Acompanhamento_Model_Acompanhamentopnc
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_Acompanhamentopnc extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.acompanhamento_pnc';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'acpid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'suoid' => array('tabela' => 'public.subunidadeorcamentaria', 'pk' => 'suoid'),
        'janid' => array('tabela' => 'acompanhamento.janela', 'pk' => 'janid'),
        'mpnid' => array('tabela' => 'public.metapnc', 'pk' => 'mpnid')
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'acpid' => null,
        'suoid' => null,
        'janid' => null,
        'mpnid' => null,
        'acpenviado' => null,
        'docid' => null,
        'acpdata' => null,
        'acpstatus' => null,
    );

    /**
     * Busca Metas PNC
     * 
     * @param stdClass $filtro
     * @return array
     */
    public static function getMetasPnc(stdClass $filtro){
        global $db;
        $sql = "
            SELECT
                mpncod,
                mpnid,
                mpndsc,
                suoid,
                prsano,
                acompanhamento,
                pis
            FROM(
                SELECT
                    m.mpncod,
                    m.mpnid,
                    m.mpndsc,
                    sm.suoid,
                    m.prsano,
                    (
                        SELECT
                            count(0) AS qtd
                        FROM acompanhamento.acompanhamento_pnc a1
                        WHERE
                            a1.acpstatus = 'A'
                            AND a1.acpenviado IS TRUE
                            AND a1.mpnid = m.mpnid
                            AND a1.janid = ". (int)$filtro->janid. "
                            AND a1.suoid = '". (int)$filtro->suoid. "'
                    ) AS acompanhamento,
                    (
                        SELECT
                            count(0) AS qtd
                        FROM planacomorc.pi_complemento c
                            JOIN monitora.pi_planointerno p ON c.pliid = p.pliid
                            JOIN public.vw_subunidadeorcamentaria su ON p.ungcod = su.suocod
                        WHERE
                            c.mpnid = m.mpnid
                            AND su.suoid = '". (int)$filtro->suoid. "'
                    ) AS pis
                FROM public.metapnc m
                    JOIN spo.subunidademetapnc sm ON m.mpnid = sm.mpnid
                WHERE
                    m.mpnstatus = 'A'
                    AND m.prsano = '". (int)$filtro->exercicio. "'
                    AND sm.suoid = '". (int)$filtro->suoid. "'

                UNION

                SELECT
                    m.mpncod,
                    m.mpnid,
                    m.mpndsc,
                    '". (int)$filtro->suoid. "',
                    m.prsano,
                    (
                        SELECT
                            count(0) AS qtd
                        FROM acompanhamento.acompanhamento_pnc a1
                        WHERE
                            a1.mpnid = m.mpnid
                            AND a1.acpenviado IS TRUE
                            AND a1.acpstatus = 'A'
                            AND a1.janid = ". (int)$filtro->janid. "
                            AND a1.suoid = '". (int)$filtro->suoid. "'
                    ) AS acompanhamento,
                    (
                        SELECT
                            count(0) AS qtd
                        FROM planacomorc.pi_complemento c
                            JOIN monitora.pi_planointerno p ON c.pliid = p.pliid
                            JOIN public.vw_subunidadeorcamentaria su ON(
                                p.ungcod = su.suocod
                                AND p.unicod = su.unocod
                                AND p.pliano = su.prsano
                                AND p.plistatus = 'A'
                                AND su.suoid = '". (int)$filtro->suoid. "'
                            )
                        WHERE
                            c.mpnid = m.mpnid
                    ) AS pis
                FROM public.metapnc m
                LEFT JOIN spo.subunidademetapnc sm ON(
                    m.mpnid = sm.mpnid
                )
                WHERE
                    sm.mpnid IS NULL
                    AND m.prsano = '". (int)$filtro->exercicio. "'
                    AND m.mpnstatus = 'A'
            ) AS x
            ORDER BY
                mpncod::int
        ";

        $listaMetas = $db->carregar($sql);
        return $listaMetas? $listaMetas: array();
    }
    
    /**
     * Busca anexos do acompanhamento da Meta PNC.
     * 
     * @return array
     */
    public function buscarAnexos()
    {
        $sql = "
            SELECT
                a.arqid,
                a.arqnome || '.' || a.arqextensao AS arqnome,
                a.arqdescricao
            FROM acompanhamento.acompanhamentopncarquivo entanexo
                JOIN public.arquivo a ON(entanexo.arqid = a.arqid)
            WHERE
                a.arqstatus = 'A'
                AND entanexo.acpid = ". (int)$this->acpid;

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }
    
    /**
     * Busca atividades do acompanhamento da Meta PNC.
     * 
     * @return array/boolean
     */
    public function buscarAtividades()
    {
        $sql = "
            SELECT
                atv.atvid,
                atv.acpid,
                atv.atvtitulo,
                atv.atvproduto,
                atv.atvprodutoqtd,
                atv.muncod,
                mun.mundescricao,
                mun.estuf,
                atv.atvfonteinformacao,
                atv.atvdatacadastro
            FROM acompanhamento.atividade_pnc atv
                LEFT JOIN territorios.municipio mun ON(atv.muncod = mun.muncod)
            WHERE
                atv.atvstatus = 'A'
                AND atv.acpid = ". (int)$this->acpid;
        $listaAtividades = $this->carregar($sql);
        return $listaAtividades;
    }
    
    /**
     * Verifica se existe pelo menos uma atividade inserida no acompanhamento da Meta PNC
     * 
     * @return boolean
     */
    public function verificarPreenchimentoAtividade(){
        $listaAtividades = $this->buscarAtividades();
        $resultado = $listaAtividades? TRUE: FALSE;
        return $resultado;
    }
    
    /**
     * Verifica se os Indicadores do Ano atual foram preenchidos para o acompanhamento da Meta PNC.
     * 
     * @return boolean
     */
    public function verificarPreenchimentoIndicadoresAnoAtual(){
        $qtdIndicadorPreenchido = 0;
        $listaIndicadorPnc = $this->buscarIndicadoresPnc();
        # Busca informações da Unidade e Subunidade pra exibir informações extras pra o usuário
        $subunidade = new Public_Model_SubUnidadeOrcamentaria($this->suoid);
        $unidadeOrcamentaria = new Public_Model_UnidadeOrcamentaria($subunidade->unoid);
        # Busca o exercicio atual do monitoramento da Unidade\Subunidade
        $exercicio = (int)$unidadeOrcamentaria->prsano;
        if($listaIndicadorPnc){
            foreach($listaIndicadorPnc as $indicador){
                $acompanhamentoIndicadorPnc = new Acompanhamento_Model_Indicadorpnc();
                $acompanhamentoIndicadorPnc->acpid = $this->acpid;
                $acompanhamentoIndicadorPnc->unocod = $unidadeOrcamentaria->unocod;
                $acompanhamentoIndicadorPnc->suocod = $subunidade->suocod;
                $acompanhamentoIndicadorPnc->ipncod = $indicador['ipncod'];
                $acompanhamentoIndicadorPnc->prsano = $exercicio;
                $acompanhamentoIndicadorPnc->buscarPorAcompanhamentoPncUnidadeIndicadorAno();

                if($acompanhamentoIndicadorPnc->idpid){
                    $qtdIndicadorPreenchido++;
                }
            }
        }
        
        # Verificar se todos os indicadores foram preenchidos para o ano(atual) da Subunidade
        $resultado = $qtdIndicadorPreenchido >= count($listaIndicadorPnc)? TRUE: FALSE;
        return $resultado;
    }
    
    /**
     * Busca Indicadores do acompanhamento da Meta PNC.
     * 
     * @return array/boolean
     */
    public function buscarIndicadoresPnc()
    {
        $listaIndicadoresPnc = array();

        # Busca dados da Subunidade
        $subunidade = new Public_Model_SubUnidadeOrcamentaria((int)$this->suoid);
        # Busca dados da UO para usar o Ano
        $uo = new Public_Model_UnidadeOrcamentaria((int)$subunidade->unoid);

        $sql = "
                -- Busca todos os indicadores que são da Subunidade e Meta informada
                SELECT DISTINCT
                    ipn.ipnid,
                    ipn.mpnid,
                    ipn.ipncod,
                    ipn.ipndsc,
                    ipn.prsano
                FROM public.indicadorpnc ipn
                    JOIN public.metapnc mpn ON(
                        mpn.mpnid = ipn.mpnid
                        AND mpn.mpnstatus = 'A'
                    )
                    JOIN spo.subunidadeindicadorpnc sic ON sic.ipnid = ipn.ipnid
                    JOIN public.vw_subunidadeorcamentaria suo ON(
                        sic.suoid = suo.suoid
                        AND ipn.prsano = suo.prsano
                    )
                WHERE
                    ipn.ipnstatus = 'A'
                    AND sic.suoid = ". (int)$this->suoid. "
                    AND mpn.mpnid = ". (int)$this->mpnid. "
            UNION ALL
                -- Busca todos os indicadores que têm Subunidade vinculada e que seja da Meta informada
                SELECT DISTINCT
                    ipn.ipnid,
                    ipn.mpnid,
                    ipn.ipncod,
                    ipn.ipndsc,
                    ipn.prsano
                FROM public.indicadorpnc ipn
                    JOIN public.metapnc mpn ON(
                        mpn.mpnid = ipn.mpnid
                        AND mpn.mpnstatus = 'A'
                    )
                    LEFT JOIN spo.subunidadeindicadorpnc sic ON sic.ipnid = ipn.ipnid
                    LEFT JOIN public.vw_subunidadeorcamentaria suo ON(
                        sic.suoid = suo.suoid
                        AND ipn.prsano = suo.prsano
                    )
                WHERE
                    ipn.ipnstatus = 'A'
                    AND sic.suoid IS NULL
                    AND mpn.mpnid = ". (int)$this->mpnid. "
                    AND ipn.prsano = '". (int)$uo->prsano. "'
                ORDER BY
                    4 ASC
        ";
//ver($sql, d);
        $listaIndicadoresPnc = $this->carregar($sql);

        return $listaIndicadoresPnc;
    }
    
    /**
     * Monta consulta pra buscar acompanhamento por Janela, Subunidade e Meta.
     * 
     * @return string
     */
    public function montarSqlbuscarPorJanelaSubunidadeMeta(){
        $sql = "
            SELECT
                acp.acpid,
                acp.suoid,
                acp.janid,
                acp.mpnid,
                acp.acpenviado,
                acp.docid,
                acp.acpdata
            FROM acompanhamento.acompanhamento_pnc acp
            WHERE
                acp.acpstatus = 'A'
                AND acp.suoid = ". (int)$this->suoid. "
                AND acp.janid = ". (int)$this->janid. "
                AND acp.mpnid = ". (int)$this->mpnid;

        return $sql;
    }
    
    public function buscarPorJanelaSubunidadeMeta() {
        $sql = $this->montarSqlbuscarPorJanelaSubunidadeMeta();
        $resultado = $this->pegaLinha($sql);
        $acompanhamento = $resultado? (object)$resultado: (object)$this->arAtributos;
        
        return $acompanhamento;
    }    

}
