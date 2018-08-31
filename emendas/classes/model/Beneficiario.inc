<?php

/**
 * Classe de mapeamento da entidade emendas.beneficiario
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-11-2017
 * @link     no link
 */
require_once APPRAIZ. 'includes/classes/Modelo.class.inc';

/**
 * Emendas_Model_Beneficiario
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Emendas_Model_Beneficiario extends Modelo
{

    const K_PARECER_NAO         = 0;
    const K_PARECER_SIM         = 1;
    const K_PARECER_NAO_APLICA  = 2;

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.beneficiario';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'benid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'esfid' => array('tabela' => 'territorios.esfera', 'pk' => 'esfid'),
        'proid' => array('tabela' => 'emendas.proponente', 'pk' => 'proid'),
        'suoid' => array('tabela' => 'subunidadeorcamentaria', 'pk' => 'suoid'),
        'oppid' => array('tabela' => 'objetivoppa', 'pk' => 'oppid'),
        'mppid' => array('tabela' => 'metappa', 'pk' => 'mppid'),
        'ippid' => array('tabela' => 'iniciativappa', 'pk' => 'ippid'),
        'mpnid' => array('tabela' => 'metapnc', 'pk' => 'mpnid'),
        'ipnid' => array('tabela' => 'indicadorpnc', 'pk' => 'ipnid'),
        'pumid' => array('tabela' => 'monitora.pi_unidade_medida', 'pk' => 'pumid'),
        'pprid' => array('tabela' => 'monitora.pi_produto', 'pk' => 'pprid'),
        'pliid' => array('tabela' => 'monitora.pi_planointerno', 'pk' => 'pliid')
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'benid' => null,
        'emeid' => null,
        'suoid' => null,
        'proid' => null,
        'esfid' => null,
        'benformaexecucao' => null,
        'bennumeroprocesso' => null,
        'beninicio' => null,
        'benpprogramanumero' => null,
        'benprogramatitulo' => null,
        'benprogramaobjeto' => null,
        'benstatus' => null,
        'benpropostanumero' => null,
        'benpropostatitulo' => null,
        'benpropostaobjeto' => null,
        'benempenhado' => null,
        'benconveniado' => null,
        'benpago' => null,
        'benmotivoimpedimento' => null,
        'sicid' => null,
        'impid' => null,
        'mdeid' => null,
        'neeid' => null,
        'capid' => null,
        'bented' => null,
        'benparecertecnico' => null,
        'benparecerjuridico' => null,
        'docid' => null,
        'plititulo' => null,
        'plidsc' => null,
        'oppid' => null,
        'mppid' => null,
        'ippid' => null,
        'mpnid' => null,
        'ipnid' => null,
        'pumid' => null,
        'pprid' => null,
        'picquantidade' => null,
        'pliid' => null,
        'benimpedimento' => null
    );

    /**
     * Busca dados de beneficiario.
     * 
     * @param integer $benid
     * @return array
     */
    public function buscarBeneficiario($benid){
        $sql = "
            SELECT DISTINCT
                ben.benid,
                ben.suoid,
                ben.proid,
                ben.esfid,
                ben.benformaexecucao,
                ben.bennumeroprocesso,
                ben.beninicio,
                ben.benprogramatitulo,
                ben.benprogramaobjeto,
                ben.benstatus,
                ben.emeid,
                ben.benpprogramanumero,
                ben.benpropostatitulo,
                ben.benpropostaobjeto,
                ben.benpropostanumero,
                ben.benempenhado,
                ben.benconveniado,
                ben.benpago,
                ben.benimpedimento,
                ben.benmotivoimpedimento,
                ben.sicid,
                ben.impid,
                ben.mdeid,
                ben.neeid,
                ben.capid,
                ben.bented,
                ben.benparecertecnico,
                ben.benparecerjuridico,
                ben.docid,
                ben.plititulo,
                ben.plidsc,
                ben.oppid,
                ben.mppid,
                ben.ippid,
                ben.mpnid,
                ben.ipnid,
                ben.pumid,
                ben.pprid,
                ben.picquantidade,
                pi_planointerno.pliid AS pliid,
                vptres.ptres,
                vptres.funcional,                
                vpi.empenhado,
                vpi.pago
            FROM $this->stNomeTabela ben
            	JOIN emendas.emenda em ON(ben.emeid = em.emeid)
                LEFT JOIN monitora.vw_ptres vptres ON(em.ptrid = vptres.ptrid)
                LEFT JOIN monitora.pi_planointerno ON(
                    ben.pliid = pi_planointerno.pliid
                    AND pi_planointerno.plistatus = 'A')
                LEFT JOIN monitora.vw_planointerno vpi ON(ben.pliid = vpi.pliid)
            WHERE
                {$this->arChavePrimaria[0]} = ". (int)$benid;
        
        $beneficiario = $this->pegaLinha($sql);
        return $beneficiario;
    }
    
    public function carregarPorId($id) {
        $arResultado = $this->buscarBeneficiario($id);
        $this->popularObjeto(array_keys($this->arAtributos), $arResultado);
    }
    
    public function recuperarTiposParecer()
    {
        return [
            self::K_PARECER_SIM => 'Sim',
            self::K_PARECER_NAO => 'Não',
            self::K_PARECER_NAO_APLICA => 'Não se Aplica',
        ];
    }

    public function recuperarTodos($stCampos = '*', $arClausulas = null, $stOrdem = null, array $opcoes = array()) {

        $sql = "
            SELECT DISTINCT
                ben.*,
                REPLACE(to_char(pro.procnpj::numeric, '00:000:000/0000-00'), ':', '.') procnpj,
                pro.pronome,
                suo.suocod,
                suo.suonome,
                suo.unocod,
                suo.unonome,
                suo.unoid,
                esf.esfdsc,
                CASE WHEN benformaexecucao = 'S' THEN
                    'SICONV'
                WHEN benformaexecucao = 'D' THEN
                    'DIRETA'
                WHEN benformaexecucao = 'T' THEN
                    'TED'
                END formaexecucao,
                (
                    SELECT
                        sit.sitdsc
                    FROM emendas.siconvbeneficiario sbe
                        JOIN emendas.siconv sic ON sic.sicid = sbe.sicid
                        JOIN emendas.siconvsituacao sit ON sit.sitid = sic.sitid
                    WHERE
                        sbe.benid = ben.benid
                    ORDER BY
                        sicsequencial DESC
                    LIMIT 1
                ) AS sitdsc,
                (
                    SELECT
                        sic.sicsequencial || '/' || sic.prsano
                    FROM emendas.siconvbeneficiario sbe
                        JOIN emendas.siconv sic ON sic.sicid = sbe.sicid
                        JOIN emendas.siconvsituacao sit ON sit.sitid = sic.sitid
                    WHERE
                        sbe.benid = ben.benid
                    ORDER BY
                        sicsequencial DESC
                    LIMIT 1
                ) AS numero_proposta,
                (
                    SELECT
                        SUM(bedvalor)
                    FROM emendas.beneficiariodetalhe bd
                        JOIN emendas.emendadetalhe ed ON bd.emdid = ed.emdid
                    WHERE
                        bd.bedstatus = 'A'
                        AND bd.benid = ben.benid
                        AND ed.gndcod <= 3
                ) AS custeio,
                (
                    SELECT
                        SUM(bedvalor)
                    FROM emendas.beneficiariodetalhe bd
                        JOIN emendas.emendadetalhe ed ON bd.emdid = ed.emdid
                    WHERE
                        bd.bedstatus = 'A'
                        AND bd.benid = ben.benid
                        AND ed.gndcod >= 4
                ) AS capital,
                pli.empenhado,
                pli.pago
            FROM emendas.beneficiario ben
                LEFT JOIN emendas.proponente pro ON pro.proid = ben.proid
                LEFT JOIN public.vw_subunidadeorcamentaria suo ON suo.suoid = ben.suoid
                LEFT JOIN territorios.esfera esf ON esf.esfid = ben.esfid
                LEFT JOIN monitora.vw_planointerno pli ON (ben.pliid = pli.pliid)
        ";

        $sql .= $arClausulas ? " WHERE " . implode(" AND ", $arClausulas) : "";
        $sql .= $stOrdem ? " ORDER BY $stOrdem" : "";

        if ($opcoes['query']) {
            return $sql;
        }

        $resultado = $this->carregar($sql);
        return $resultado ? $resultado : array();
    }

    /**
     * Remove valores vazios da lista. Obs: Correção de bug do compontente chosen.
     * 
     * @param type $lista
     * @return array
     */
    public function limparLista($lista){
        $listaLimpa = array();
        foreach($lista as $valor){
            
        }
        return $listaLimpa;
    }
    
    public function associarDelegacao($dados)
    {
        if($benid = $this->benid){

            $mDelegacao = new Emendas_Model_Delegacao();
            $mDelegacao->excluirVarios("benid = '$benid'");
            if (isset($dados['radioDelegacao'])){
                if(isset($dados['delegacao']) && is_array($dados['delegacao'])){

                    $mDelegacao->benid = $benid;

                    foreach($dados['delegacao'] as $suoid){
                        if ($suoid != ''){
                            $mDelegacao->suoid = $suoid;
                            $mDelegacao->salvar();

                            $mDelegacao->edeid = null;
                        }
                    }
                }
            }
        }
    }

    public function associarAlteracao($dados)
    {
        if($benid = $this->benid){

            $mBeneficiarioAlteracao = new Emendas_Model_BeneficiarioAlteracao();
            $mBeneficiarioAlteracao->excluirVarios("benid = '$benid'");

            if(isset($dados['alteracao']) && is_array($dados['alteracao'])){

                $mBeneficiarioAlteracao->benid = $benid;

                foreach($dados['alteracao'] as $aloid){
                    $mBeneficiarioAlteracao->aloid = $aloid;
                    $mBeneficiarioAlteracao->salvar();

                    $mBeneficiarioAlteracao->beaid = null;
                }
            }
        }
    }
    
    public function associarSniic($dados)
    {
        if($this->benid){

            $mSniic = new Emendas_Model_Sniic();
            $mSniic->excluirVarios("benid = '$this->benid'");

            if(isset($dados['lista_sniic']) && is_array($dados['lista_sniic'])){

                $mSniic->benid = $this->benid;

                foreach($dados['lista_sniic'] as $sniic){
                    $mSniic->sninumero = $sniic;
                    $mSniic->salvar();

                    $mSniic->sniid = null;
                }
            }
        }
    }
    
    public function associarPronac($dados)
    {
        if($this->benid){

            $mPronac = new Emendas_Model_Pronac();
            $mPronac->excluirVarios("benid = '$this->benid'");

            if(isset($dados['lista_pronac']) && is_array($dados['lista_pronac'])){

                $mPronac->benid = $this->benid;

                foreach($dados['lista_pronac'] as $pronac){
                    $mPronac->pronumero = $pronac;
                    $mPronac->salvar();

                    $mPronac->proid = null;
                }
            }
        }
    }
    
    public function associarAnexo($dados)
    {
        if($this->benid){

            $mBeneficiarioAnexo = new Emendas_Model_BeneficiarioAnexo();
            $mBeneficiarioAnexo->excluirVarios("benid = '$this->benid'");

            if(isset($dados['listaAnexos']) && is_array($dados['listaAnexos'])){

                $mBeneficiarioAnexo->benid = $this->benid;

                foreach($dados['listaAnexos'] as $arqid){
                    $mBeneficiarioAnexo->arqid = $arqid;
                    $mBeneficiarioAnexo->salvar();

                    $mBeneficiarioAnexo->beaid = null;
                }
            }
        }
    }
    
    public function montarSqlRecuperarDelegacao(stdClass $filtro){
        $sql = "
            SELECT DISTINCT
                suo.suoid,
                suo.suocod,
                suo.suosigla,
                suo.suonome,
                suo.unoid,
                suo.unocod,
		uo.unocodorcamento,
                suo.unosigla,
                suo.unonome
            FROM public.vw_subunidadeorcamentaria suo
		JOIN public.unidadeorcamentaria uo ON(suo.unoid = uo.unoid)
                JOIN emendas.delegacao d ON(suo.suoid = d.suoid)
            WHERE
                suo.suostatus = 'A'
                AND d.benid = ". (int)$this->benid. "
                AND suo.prsano = '". (int)$filtro->exercicio. "' ";
        
        return $sql;
    }

    public function recuperarDelegacao($exercicio = null)
    {
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        $sql = $this->montarSqlRecuperarDelegacao((object)array('exercicio' => $exercicio));

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];

        $listaId = [];
        foreach($dados as $dado){
            $listaId[] = $dado['suoid'];
        }
        return $listaId;
    }

    public function recuperarAlteracao($exercicio = null)
    {
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        $sql = "SELECT aloid as codigo, aloid as descricao
                FROM emendas.beneficiarioalteracao 
                WHERE benid = ". (int)$this->benid;

        $dados = $this->carregar($sql);
        return simec_preparar_array($dados);
    }

    public function recuperarDadosSiconv($sicid)
    {
        $sql = "select * 
                from emendas.siconv sic
                where sic.sicid = " . (int) $sicid;

        $dadosSiconv['siconv'] = $this->pegaLinha($sql);

        $sql = "select * 
                from emendas.siconvparecer spa
                where spa.sicid = " . (int) $sicid . "
                order by spadata desc ";

        $aParecer = $this->carregar($sql);
        $aParecer = $aParecer ? $aParecer : [];

        $dadosSiconv['parecer'] = [];
        foreach($aParecer as $parecer){
            $dadosSiconv['parecer'][$parecer['spatipo']][] = $parecer;
        }

        $sql = "select * 
                from emendas.siconvprograma sip
                  inner join emendas.programa prg on prg.prgid = sip.prgid
                where sip.sicid = " . (int) $sicid . "
                ";

        $aPrograma = $this->carregar($sql);

        $dadosSiconv['programa'] = [];
        foreach($aPrograma as $programa){
            $dadosSiconv['programa'][] = $programa;
        }

        return $dadosSiconv;
    }

    /**
     * Grava informações do usuário que está alterando o beneficiário.
     * 
     * @param array $post
     */
    public function gravarHistorico(array $post) {
        $beneficiarioHistorico = new Emendas_Model_BeneficiarioHistorico();
        $beneficiarioHistorico->benid = $this->benid;
        $beneficiarioHistorico->usucpf = $_SESSION['usucpf'];
        $beneficiarioHistorico->behdata = 'NOW()';
        $beneficiarioHistorico->salvar();
        $beneficiarioHistorico->gravarHistoricoSubUnidade($post, $this);
        $beneficiarioHistorico->gravarHistoricoDelegacao($post, $this);
        $beneficiarioHistorico->gravarHistoricoProponente($post, $this);
        $beneficiarioHistorico->gravarHistoricoLocalizacao($post, $this);
//        $beneficiarioHistorico->gravarHistoricoLocalizacao($post, $this);
    }

}
