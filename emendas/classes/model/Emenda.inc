<?php
/**
 * Classe de mapeamento da entidade emendas.emenda
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-11-2017
 * @link     no link
 */


require_once APPRAIZ. 'includes/classes/Modelo.class.inc';
include_once APPRAIZ. 'emendas/classes/service/WSIntegracaoSiconv.class.inc';


/**
 * Emendas_Model_Emenda
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Emendas_Model_Emenda extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.emenda';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'emeid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'prsano' => array('tabela' => 'emendas.programacaoexercicio', 'pk' => 'prsano'),
        'autid' => array('tabela' => 'emendas.autor', 'pk' => 'autid'),
        'ptrid' => array('tabela' => 'monitora.ptres', 'pk' => 'ptrid'),
        'unoid' => array('tabela' => 'unidadeorcamentaria', 'pk' => 'unoid')
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'emeid' => null,
        'emenumero' => null,
        'unoid' => null,
        'ptrid' => null,
        'emejustificativa' => null,
        'emeimpositiva' => null,
        'autid' => null,
        'prsano' => null,
        'emestatus' => null,
        'emeatualizacaosiconv' => null,
    );

    /**
     * Monta SQL com filtros da consulta principal de Emedas.
     * 
     * @param stdClass $filtro
     * @return string
     */
    public function montarSqlFiltrosListaEmendas(stdClass $filtro) {
        # Filtros
        $where = NULL;
        if($filtro->listaSubUnidadeUsuario){
            # Expressão para verificar se o estado é após enviado para preenchimento da unidade.
            $where .= "\n AND doce.esdid IN(". ESD_BENEFICIARIO_PREENCHIMENTO_UNIDADE. ','. ESD_BENEFICIARIO_CORRECAO. ','. ESD_BENEFICIARIO_PI_GERADO. ','. ESD_BENEFICIARIO_PI_APROVADO. ")";
            # Expressão pra verificar se o beneficiario é da mesa unidade do Usuário Logado com perfil de Unidade.
            $where .= "\n AND (suo.suocod::INTEGER IN(". join(',', $filtro->listaSubUnidadeUsuario). ") OR suoed.suocod::INTEGER IN(". join(',', $filtro->listaSubUnidadeUsuario). ") ) ";
        }
        
        if($filtro->unicod){
            $where .= "\n AND suo.unocod = '". (int)$filtro->unicod. "'";
        }
        if($filtro->suoid){
            $where .= "\n AND suo.suoid =  ". (int)$filtro->suoid;
        }
        if($filtro->unicod_delegada){
            $where .= "\n AND suoed.unocod = '". (int)$filtro->unicod_delegada. "'";
        }
        if($filtro->suoid_delegada){
            $where .= "\n AND suoed.suoid =  ". (int)$filtro->suoid_delegada;
        }
        if($filtro->ptrid){
            $where .= "\n AND ptr.ptrid =  ". (int)$filtro->ptrid;
        }
        if($filtro->lista_esdid){
            $where .= "\n AND doce.esdid IN(". join(',', $filtro->lista_esdid). ")";
        }
        if($filtro->lista_sitid){
            $where .= "\n AND
                (
		    SELECT
                        sic.sitid
                    FROM emendas.siconvbeneficiario sbe
                        JOIN emendas.siconv sic ON sic.sicid = sbe.sicid
                    WHERE
			sbe.benid = ben.benid
                    ORDER BY
                        sicsequencial DESC
                    LIMIT 1
                ) IN(". join(',', $filtro->lista_sitid). ")";
        }
        if($filtro->benimpedimento){
            $where .= "\n AND ben.impid ". ($filtro->benimpedimento == 't'? 'IS NOT NULL': 'IS NULL');
        }
        
        return $where;
    }
    
    public function recuperarListagem(stdClass $filtro)
    {
        # Filtros
        $where = $this->montarSqlFiltrosListaEmendas($filtro);

        $sql = "
            SELECT DISTINCT
                eme.emeid,
                eme.emenumero,
                eme.unoid,
                eme.ptrid,
                eme.emeimpositiva,
                eme.autid,
                eme.prsano,
                uno.unosigla unidade,
                aut.autnome,
                aut.autcod,
                COALESCE(rep.qtdrepetida, 0) qtdrepetida,
                pro.pronome,
                (
		    SELECT
                        sit.sitdsc
                    FROM emendas.siconvbeneficiario sbe
                        JOIN emendas.siconv sic ON sic.sicid = sbe.sicid
                        JOIN emendas.siconvsituacao sit ON sit.sitid = sic.sitid
                    WHERE
			sit.sitstatus = 'A'
			AND sbe.benid = ben.benid
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
                ben.benid,
                suo.suosigla subunidade,
                (SELECT ARRAY_TO_STRING(ARRAY(
                    SELECT
                        unosigla || ' - ' || suonome
                    FROM emendas.delegacao d
                        JOIN public.vw_subunidadeorcamentaria suod ON(d.suoid = suod.suoid)
                    WHERE
                        d.benid = ben.benid
                ), '\n ')) AS subunidade_delegada,
                aut.estuf,
                ben.impid,
                doce.esddsc,
                pi.pliid,
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
                pi.empenhado,
                pi.pago
            FROM emendas.emenda eme
                LEFT JOIN emendas.beneficiario ben ON(ben.emeid = eme.emeid AND benstatus = 'A')
                LEFT JOIN emendas.proponente pro ON pro.proid = ben.proid
                JOIN public.unidadeorcamentaria uno ON uno.unoid = eme.unoid
                JOIN emendas.autor aut ON aut.autid = eme.autid
                JOIN monitora.ptres ptr ON ptr.ptrid = eme.ptrid
                LEFT JOIN public.vw_subunidadeorcamentaria suo ON ben.suoid = suo.suoid
                LEFT JOIN emendas.delegacao ed ON ben.benid = ed.benid
                LEFT JOIN public.vw_subunidadeorcamentaria suoed ON ed.suoid = suoed.suoid
                LEFT JOIN workflow.documento doc ON ben.docid = doc.docid
                LEFT JOIN workflow.estadodocumento doce ON doc.esdid = doce.esdid
                LEFT JOIN monitora.vw_planointerno pi ON ben.pliid = pi.pliid
                LEFT JOIN (
                    SELECT
                        eme.emeid,
                        pro.procnpj,
                        COUNT(pro.proid) qtdrepetida
                    FROM
                        emendas.emenda eme
                        INNER JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid
                        INNER JOIN emendas.proponente pro ON pro.proid = ben.proid
                    WHERE
                        EXISTS (
                            SELECT
                                1
                            FROM emendas.siconvbeneficiario sib
                            WHERE
                                sib.emenumero = eme.emenumero
                                AND sib.procnpj = pro.procnpj
                        )
                    GROUP BY
                        eme.emeid,
                        pro.procnpj
                ) rep ON(rep.emeid = eme.emeid AND pro.procnpj = rep.procnpj)
            WHERE
                eme.emestatus = 'A'
                AND eme.prsano = '". (int)$filtro->exercicio. "'
                $where
            ORDER BY
                aut.autnome,
                eme.emenumero,
                pro.pronome
        ";

//ver($sql, d);
        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }

    /**
     * Monta consulta pra buscar dados do relat?rio geral
     *
     * @param stdClass $dto
     * @return string
     */
    public function montarSqlRelatorioGeral(stdClass $dto){

        $where = '';
        $where .= $dto->exercicio? "\n AND eme.prsano = '". (int)$dto->exercicio. "'": NULL;
        $where .= $dto->listaSubUnidadeUsuario? "\n AND (suo.suocod::INTEGER IN(". join(',', $dto->listaSubUnidadeUsuario). ") OR suoed.suocod::INTEGER IN(". join(',', $dto->listaSubUnidadeUsuario). ") ) ": NULL;
        $where .= $dto->suoid? "\n AND suo.suoid IN(".join($dto->suoid, ','). ")": NULL;
        $where .= $dto->suoid_delegada? "\n AND suoed.suoid IN(".join($dto->suoid_delegada, ','). ")": NULL;
        $where .= $dto->sitid? "\n AND sic.sitid IN(".join($dto->sitid, ','). ")": NULL;
        $where .= $dto->sitid? "\n AND sic.sitid IN(".join($dto->sitid, ','). ")": NULL;
        $where .= $dto->esdid? "\n AND doc.esdid IN(".join($dto->esdid, ','). ")": NULL;
        $where .= $dto->autid? "\n AND eme.autid IN(".join($dto->autid, ','). ")": NULL;
        $where .= $dto->estuf? "\n AND aut.estuf IN('".join($dto->estuf, "','"). "')": NULL;
        $where .= $dto->parid? "\n AND aut.parid IN(".join($dto->parid, ','). ")": NULL;
        $where .= $dto->capid? "\n AND ben.capid IN(".join($dto->capid, ','). ")": NULL;
        $where .= $dto->mdeid? "\n AND ben.mdeid IN(".join($dto->mdeid, ','). ")": NULL;
        $where .= $dto->aloid? "\n AND ben.benid IN(
                SELECT DISTINCT
                    ba.benid
                FROM emendas.beneficiarioalteracao ba
                    LEFT JOIN emendas.alteracaoorcamentaria alo ON ba.aloid = alo.aloid
                WHERE
                    alo.alodsc IS NOT NULL
                    AND alo.aloid IN(". join($dto->aloid, ','). ")
            )
        ": NULL;
        if($dto->benparecertecnico == 't'){
            $where .= "\n AND ben.benparecertecnico = 1";
        }
        if($dto->benparecertecnico == 'f'){
            $where .= "\n AND ben.benparecertecnico = 2";
        }
        if($dto->benparecerjuridico == 't'){
            $where .= "\n AND ben.benparecerjuridico = 1";
        }
        if($dto->benparecerjuridico == 'f'){
            $where .= "\n AND ben.benparecerjuridico = 2";
        }
        $where .= $dto->impid? "\n AND ben.impid IN(".join($dto->impid, ','). ")": NULL;

        $sql = "
           SELECT DISTINCT
		-- DADOS DA EMENDA
		eme.emeid,
		SUBSTRING(eme.emenumero::TEXT FROM 1 FOR 4) || ' ' || SUBSTRING(eme.emenumero::TEXT FROM 5 FOR LENGTH(eme.emenumero::TEXT)) AS emenumero,
                -- AUTOR
                aut.autcod,
                aut.autnome,
                CASE WHEN aut.autsexo = 'M' THEN 'Homem' when aut.autsexo = 'F' THEN 'Mulher' ELSE 'A Identificar' end genero,
                autip.atpdsc AS tipo_parlamentar,
                aut.estuf,
                p.parsigla,
                -- UNIDADE
		uno.unocod,
		uno.unosigla,
		ptr.funcional || ' - ' || COALESCE(ptr.acatitulo, '') AS funcional,
                CASE WHEN eme.emeimpositiva = 't' THEN 'Sim' ELSE 'Não' END AS impositiva,
                eme.emejustificativa,
                -- DADOS FINANCEIROS
                COALESCE((SELECT SUM(COALESCE(emed.emdvalor,0)) FROM emendas.emendadetalhe emed WHERE emed.emeid = eme.emeid and gndcod<=3),0) AS custeio,
                COALESCE((SELECT SUM(COALESCE(emed.emdvalor,0)) FROM emendas.emendadetalhe emed WHERE emed.emeid = eme.emeid and gndcod>3),0) AS capital,
                -- BENEFICIARIOS
		ben.benid,
                suo.suocod AS cod_unidade,
                suo.suonome AS unidade,
                suoed.suocod AS cod_unidade_delegada,
                suoed.suonome AS unidade_delegada,
                REPLACE(TO_CHAR(pro.procnpj::NUMERIC, '00:000:000/0000-00'), ':', '.') procnpj,
                pro.pronome,
		esf.esfdsc AS localizacao,
                REPLACE(REPLACE(ARRAY(
                    SELECT DISTINCT
                        p.paidescricao
                    FROM emendas.beneficiariolocalizacao bl
                        JOIN territorios.pais p ON bl.paiid = p.paiid
                    WHERE
                        p.paidescricao IS NOT NULL
                        AND bl.benid = ben.benid
                )::TEXT, '{', ''), '}', '') AS lista_pais,
                REPLACE(REPLACE(ARRAY(
                    SELECT DISTINCT
                        bl.estuf
                    FROM emendas.beneficiariolocalizacao bl
                    WHERE
                        bl.estuf IS NOT NULL
                        AND bl.benid = ben.benid
                )::TEXT, '{', ''), '}', '') AS lista_uf,
                REPLACE(REPLACE(ARRAY(
                    SELECT DISTINCT
                        m.mundescricao || '-' || m.estuf
                    FROM emendas.beneficiariolocalizacao bl
                        JOIN territorios.municipio m ON bl.muncod = m.muncod
                    WHERE
                        m.mundescricao IS NOT NULL
                        AND bl.benid = ben.benid
                )::TEXT, '{', ''), '}', '') AS lista_municipio,
		ben.bennumeroprocesso AS processo,
		TO_CHAR(ben.beninicio,'dd/mm/YYYY') AS vigencia,
		pca.capdsc,
                CASE WHEN bented IS TRUE THEN 'Sim' ELSE 'Não' END AS ted,
                mde.arcdsc,
		nee.needsc,
                REPLACE(REPLACE(REPLACE(ARRAY(
                    SELECT DISTINCT
                        alo.alodsc
                    FROM emendas.beneficiarioalteracao ba
                        LEFT JOIN emendas.alteracaoorcamentaria alo ON ba.aloid = alo.aloid
                    WHERE
                        alo.alodsc IS NOT NULL
                        AND ba.benid = ben.benid
                )::TEXT, '{', ''), '}', ''), '\"', '') AS lista_alteracoes,
		CASE WHEN ben.benparecertecnico = 0 THEN 'Não' WHEN ben.benparecertecnico = 1 THEN 'Sim' WHEN ben.benparecertecnico = 2 THEN 'Não se aplica' END AS parecer_tecnico,
		CASE WHEN ben.benparecerjuridico = 0 THEN 'Não' WHEN ben.benparecerjuridico = 1 THEN 'Sim' WHEN ben.benparecerjuridico = 2 THEN 'Não se aplica' END AS parecer_juridico,
		CASE WHEN ben.benimpedimento IS NULL THEN 'Não' WHEN ben.benimpedimento = 0 THEN 'Não' WHEN ben.benimpedimento = 1 THEN 'Sim' WHEN ben.benimpedimento = 2 THEN 'Não se aplica' END AS empedimento,
                CASE WHEN ben.benimpedimento = 1 THEN imp.impdsc END AS motivo,
                CASE WHEN ben.benimpedimento = 1 THEN benmotivoimpedimento END AS justificativa,
                
                -- DADOS FINANCEIROS DO BENFICIÁRIO
		COALESCE((
                    SELECT
                        SUM(COALESCE(
                            CASE WHEN eme.gndcod <= 3 THEN
                                bed.bedvalor
                            ELSE
                                0
                            END, 0)) AS bedvalor
                    FROM emendas.beneficiariodetalhe bed
                        LEFT JOIN emendas.emendadetalhe eme ON bed.emdid = eme.emdid
                    WHERE
                        bed.benid = ben.benid
                        AND bed.bedstatus = 'A'
                ), 0) AS bedvalorcusteio,
		COALESCE((
                    SELECT
                        SUM(COALESCE(
                            CASE WHEN eme.gndcod > 3 THEN
                                bed.bedvalor
                            ELSE
                                0
                            END, 0)) AS bedvalor
                    FROM emendas.beneficiariodetalhe bed
                        LEFT JOIN emendas.emendadetalhe eme ON bed.emdid = eme.emdid
                    WHERE
                        bed.benid = ben.benid
                        AND bed.bedstatus = 'A'
                ), 0) AS bedvalorcapital,
                (
                    SELECT
                        SUM(COALESCE(
                            CASE WHEN eme.gndcod <= 3 THEN
                                bed.bedvalorpriorizado
                            ELSE
                                0
                            END, 0))
                    FROM emendas.beneficiariodetalhe bed
                        LEFT JOIN emendas.emendadetalhe eme ON bed.emdid = eme.emdid
                    WHERE
                        bed.benid = ben.benid
                        AND bed.bedstatus = 'A'
                ) AS bedvalorpriorizadocusteio,
                (
                    SELECT
                        SUM(COALESCE(CASE WHEN eme.gndcod > 3 THEN
                            bed.bedvalorpriorizado
                        ELSE
                            0
                        END, 0))
                    FROM emendas.beneficiariodetalhe bed
                        LEFT JOIN emendas.emendadetalhe eme ON bed.emdid = eme.emdid
                    WHERE
                        bed.benid = ben.benid
                        AND bed.bedstatus = 'A'
                ) AS bedvalorpriorizadocapital,
		COALESCE((
                    SELECT
                        SUM(CASE WHEN eme.gndcod <= 3 THEN
                            COALESCE(bed.bedvalor, 0) - COALESCE(bed.bedvalorpriorizado, 0)
                        ELSE
                            0
                        END)
                    FROM emendas.beneficiariodetalhe bed
                        LEFT JOIN emendas.emendadetalhe eme ON bed.emdid = eme.emdid
                    WHERE
                        bed.benid = ben.benid
                        AND bed.bedstatus = 'A'
                ), 0) AS contigenciadocusteio,
		COALESCE((
                    SELECT
                        SUM(CASE WHEN eme.gndcod > 3 THEN
                            COALESCE(bed.bedvalor, 0) - COALESCE(bed.bedvalorpriorizado, 0)
                        ELSE
                            0
                        END)
                    FROM emendas.beneficiariodetalhe bed
                        LEFT JOIN emendas.emendadetalhe eme ON bed.emdid = eme.emdid
                    WHERE
                        bed.benid = ben.benid
                        AND bed.bedstatus = 'A'
                ), 0) AS contigenciadocapital,
                COALESCE((
                    SELECT pf.vlcusteiopedido
                      from emendas.pedido_financeiro pf
                     where pf.benid = ben.benid
                       and pf.nroanoreferencia = date_part('year', CURRENT_DATE)
                       and pf.nromesreferencia = date_part('month', CURRENT_DATE)),0) as vlcusteiopedido,
                COALESCE((
                    SELECT pf.vlcapitalpedido
                      from emendas.pedido_financeiro pf
                     where pf.benid = ben.benid
                       and pf.nroanoreferencia = date_part('year', CURRENT_DATE)
                       and pf.nromesreferencia = date_part('month', CURRENT_DATE)),0) as vlcapitalpedido,
                COALESCE((
                    SELECT v.vindsc
                      from emendas.pedido_financeiro pf
                     inner join emendas.vinculacao v
                        on pf.vinid=v.vinid
                     where pf.benid = ben.benid
                       and pf.nroanoreferencia = date_part('year', CURRENT_DATE)
                       and pf.nromesreferencia = date_part('month', CURRENT_DATE)),'') as vindsc,
		-- DADOS SICONV
		CASE WHEN prg.prgcodigo IS NOT NULL THEN
                    substr(prg.prgcodigo::TEXT, 1, 6) || ' ' || substr(prg.prgcodigo::TEXT, 6, 4) || ' ' || substr(prg.prgcodigo::TEXT, 10, 4)
                ELSE
                    NULL
                END AS numero_programa,
		prg.prgtitulo AS titulo_programa,
		ben.benprogramatitulo AS objeto_programa,
		CASE WHEN bensic.sicsequencial IS NOT NULL THEN bensic.sicsequencial || '/' || bensic.prsano END AS numero_proposta,
		bensic.sicjustificativa AS titulo_proposta,
		bensic.sicobjeto AS objeto_proposta,
		bensit.sitdsc AS situacao_proposta,
                -- SITUAÇÃO WORKFLOW
                doce.esddsc,
		-- PLANO INTERNO
                pi.pliid,
                pi.plicod,
                pi.empenhado,
                pi.pago,
                (select string_agg(numnotaempenho,',') as numnotaempenho from emendas.nota_empenho ne where ne.benid = ben.benid and nestatus='A') as numnotaempenho
            FROM emendas.emenda eme
                -- DADOS DA EMENDA
                LEFT JOIN public.unidadeorcamentaria uno ON uno.unoid = eme.unoid
                LEFT JOIN emendas.autor aut ON(aut.autid = eme.autid AND aut.autstatus = 'A')
                LEFT JOIN emendas.autortipo autip ON aut.atpid = autip.atpid
                LEFT JOIN emendas.partido p ON aut.parid = p.parid
                LEFT JOIN monitora.vw_ptres ptr ON ptr.ptrid = eme.ptrid
                -- BENEFICIARIOS
                LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid
                LEFT JOIN emendas.proponente pro ON pro.proid = ben.proid
                LEFT JOIN emendas.siconv sic ON sic.sicid = ben.sicid
                LEFT JOIN emendas.siconvsituacao sit ON sit.sitid = sic.sitid
                LEFT JOIN public.vw_subunidadeorcamentaria suo ON ben.suoid = suo.suoid
                LEFT JOIN emendas.delegacao ed ON ben.benid = ed.benid
                LEFT JOIN public.vw_subunidadeorcamentaria suoed ON ed.suoid = suoed.suoid
		LEFT JOIN territorios.esfera esf ON ben.esfid = esf.esfid
		LEFT JOIN planejamento.categoria_apropriacao pca ON ben.capid = pca.capid
		LEFT JOIN public.area_cultural mde ON ben.mdeid = mde.mdeid
		LEFT JOIN planejamento.segmento_cultural nee ON ben.secid = nee.secid
		LEFT JOIN emendas.impedimento imp ON ben.impid = imp.impid
		-- DADOS SICONV
		LEFT JOIN emendas.siconvbeneficiario sib ON(ben.benid = sib.benid)
		LEFT JOIN emendas.siconvprograma sip ON(sib.sicid = sip.sicid)
		LEFT JOIN emendas.programa prg ON prg.prgid = sip.prgid
                LEFT JOIN emendas.siconv bensic ON bensic.sicid = sib.sicid
                LEFT JOIN emendas.siconvsituacao bensit ON bensic.sitid = bensit.sitid
                -- SITUAÇÃO
		LEFT JOIN workflow.documento doc ON ben.docid = doc.docid
		LEFT JOIN workflow.estadodocumento doce ON doc.esdid = doce.esdid
		-- PLANO INTERNO
                LEFT JOIN monitora.vw_planointerno pi ON ben.pliid = pi.pliid
            WHERE
                eme.emestatus = 'A'
                AND ben.benstatus = 'A'
                $where
            ORDER BY
                1,
                2
        ";
//ver($sql, d);
        return $sql;
    }
    
    /**
     * Monta as Colunas que receberão formatação de moeda para o Relatório de PI e PI-FNC do Módulo Planejamento Orçamentário.
     * 
     * @return array
     */
    public static function montarColunasRelatorioEmendas()
    {
        $colunas = array(
            array('codigo' => 'emeid',                      'descricao' => 'ID Emenda'),
            array('codigo' => 'emenumero',                  'descricao' => 'Número'),
            array('codigo' => 'autcod',                     'descricao' => 'Cod. Autor'),
            array('codigo' => 'autnome',                    'descricao' => 'Autor'),
            array('codigo' => 'genero',                     'descricao' => 'Gênero'),
            array('codigo' => 'tipo_parlamentar',           'descricao' => 'Tipo Parlamentar'),
            array('codigo' => 'estuf',                      'descricao' => 'UF Parlamentar'),
            array('codigo' => 'parsigla',                   'descricao' => 'Partido'),
            array('codigo' => 'unocod',                     'descricao' => 'Cod. Unidade'),
            array('codigo' => 'unosigla',                   'descricao' => 'Unidade'),
            array('codigo' => 'funcional',                  'descricao' => 'Funcional'),
            array('codigo' => 'impositiva',                 'descricao' => 'Impositiva'),
            array('codigo' => 'emejustificativa',           'descricao' => 'Justificativa'),
            # DADOS FINANCEIROS
            array('codigo' => 'custeio',                    'descricao' => 'Valor Custeio'),
            array('codigo' => 'capital',                    'descricao' => 'Valor Capital'),
            # BENEFICIARIOS
            array('codigo' => 'benid',                      'descricao' => 'Id Beneficiario'),
            array('codigo' => 'cod_unidade',                'descricao' => 'Código Subunidade'),
            array('codigo' => 'unidade',                    'descricao' => 'Subunidade'),
            array('codigo' => 'cod_unidade_delegada',       'descricao' => 'Código Subunidade delegada'),
            array('codigo' => 'unidade_delegada',           'descricao' => 'Subunidade delegada'),
            array('codigo' => 'procnpj',                    'descricao' => 'CNPJ'),
            array('codigo' => 'pronome',                    'descricao' => 'Proponente'),
            array('codigo' => 'localizacao',                'descricao' => 'Localização'),
            array('codigo' => 'lista_pais',                 'descricao' => 'Países'),
            array('codigo' => 'lista_uf',                   'descricao' => 'UFs'),
            array('codigo' => 'lista_municipio',            'descricao' => 'Municipios'),
            array('codigo' => 'processo',                   'descricao' => 'Número do Processo'),
            array('codigo' => 'vigencia',                   'descricao' => 'Data de Início da Vigência'),
            array('codigo' => 'capdsc',                     'descricao' => 'Modalidade de Pactuação'),
            array('codigo' => 'ted',                        'descricao' => 'TED'),
            array('codigo' => 'arcdsc',                     'descricao' => 'Área Cultural'),
            array('codigo' => 'needsc',                     'descricao' => 'Segmento Cultural'),
            array('codigo' => 'lista_alteracoes',           'descricao' => 'Alterações'),
            array('codigo' => 'parecer_tecnico',            'descricao' => 'Parecer Técnico'),
            array('codigo' => 'parecer_juridico',           'descricao' => 'Parecer Jurídico'),
            array('codigo' => 'empedimento',                'descricao' => 'Impedimento'),
            array('codigo' => 'motivo',                     'descricao' => 'Motivo'),
            array('codigo' => 'justificativa',              'descricao' => 'Justivicativa'),
            array('codigo' => 'vindsc',                     'descricao' => 'Vinculação'),
            # DADOS FINANCEIROS DO BENFICIÁRIO
            array('codigo' => 'bedvalorcusteio',            'descricao' => 'Vl. Beneficiário Custeio'),
            array('codigo' => 'bedvalorcapital',            'descricao' => 'Vl. Beneficiário Capital'),
            array('codigo' => 'bedvalorpriorizadocusteio',  'descricao' => 'Vl. Priorizado Custeio'),
            array('codigo' => 'bedvalorpriorizadocapital',  'descricao' => 'Vl. Priorizado Capital'),
            array('codigo' => 'contigenciadocusteio',       'descricao' => 'Vl. Contingenciado Custeio'),
            array('codigo' => 'contigenciadocapital',       'descricao' => 'Vl. Contingenciado Capital'),
            # DADOS SICONV
            array('codigo' => 'numero_programa',            'descricao' => 'Número Programa - SICONV'),
            array('codigo' => 'titulo_programa',            'descricao' => 'Título Programa - SICONV'),
            array('codigo' => 'objeto_programa',            'descricao' => 'Objeto Programa'),
            array('codigo' => 'numero_proposta',            'descricao' => 'Número Proposta - SICONV'),
            array('codigo' => 'titulo_proposta',            'descricao' => 'Título Proposta - SICONV'),
            array('codigo' => 'objeto_proposta',            'descricao' => 'Objeto Proposta'),
            array('codigo' => 'situacao_proposta',          'descricao' => 'Situação Proposta'),
            # SITUAÇÃO WORKFLOW
            array('codigo' => 'esddsc',                     'descricao' => 'Situação'),
            # PLANO INTERNO
            array('codigo' => 'pliid',                      'descricao' => 'Id PI'),
            array('codigo' => 'plicod',                     'descricao' => 'Código PI'),
            array('codigo' => 'empenhado',                  'descricao' => 'Empenhado'),
            array('codigo' => 'pago',                       'descricao' => 'Pago'),
            array('codigo' => 'numnotaempenho',             'descricao' => 'Nota de Empenho'),
            array('codigo' => 'vlcusteiopedido',            'descricao' => 'Custeio do Pedido'),
            array('codigo' => 'vlcapitalpedido',            'descricao' => 'Capital do Pedido'));

        return $colunas;
    }
    
    /**
     * Monta as Colunas que receberão formatação de moeda para o Relatório de PI e PI-FNC do Módulo Planejamento Orçamentário.
     *
     * @return array
     */
    public static function montarColunasFormatoMoedaRelatorioEmendas()
    {
        $colunas = [
            'custeio',
            'capital',
            'bedvalorcusteio',
            'bedvalorcapital',
            'bedvalorpriorizadocusteio',
            'bedvalorpriorizadocapital',
            'vlcusteiopedido',
            'vlcapitalpedido',
            'contigenciadocusteio',
            'contigenciadocapital',
            'empenhado',
            'pago',
            'vlcusteiopedido',
            'vlcapitalpedido'
        ];
        return $colunas;
    }

    public function gerarGraficoUf($exercicio)
    {
        $sql = "select  case when coalesce(aut.estuf, '') = '' then 'A Identificar' else  aut.estuf end categoria,
                        'Valor LOA' descricao, sum(bed.bedvalor) valor
                from emendas.emenda eme
                        inner join emendas.autor aut on aut.autid = eme.autid
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                where
                    eme.emestatus = 'A'
                    AND ben.benstatus = 'A'
                    AND eme.prsano = '$exercicio'
                group by aut.estuf, categoria
                union all
                select  case when coalesce(aut.estuf, '') = '' then 'A Identificar' else  aut.estuf end categoria,
                        'Valor Após Contingenciamento' descricao, sum(bed.bedvalorpriorizado) valor
                from emendas.emenda eme
                        inner join emendas.autor aut on aut.autid = eme.autid
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                where
                    eme.emestatus = 'A'
                    AND ben.benstatus = 'A'
                    AND eme.prsano = '$exercicio'
                group by aut.estuf, categoria
                order by categoria, descricao desc";

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];

        $dadosAgrupados = [];
        foreach ($dados as $dado) {
            $dadosAgrupados[$dado['categoria']][$dado['descricao']] = $dado['valor'];
        }

        $sqlPizza = "  select  case when coalesce(reg.regdescricao, '') = '' then 'A Identificar' else  reg.regdescricao end descricao, reg.regcod, sum(bed.bedvalor) valor
                        from emendas.emenda eme
                                inner join emendas.autor aut on aut.autid = eme.autid
                                left join territorios.estado est on est.estuf = aut.estuf
                                left join territorios.regiao reg on reg.regcod = est.regcod
                                inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                                inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                        where
                            eme.emestatus = 'A'
                            AND ben.benstatus = 'A'
                            AND eme.prsano = '$exercicio'
                        group by descricao, reg.regcod
                        order by reg.regcod";

        $sql = "select  case when coalesce(reg.regdescricao, '') = '' then 'A Identificar' else  reg.regdescricao end categoria, reg.regcod,
                        'Valor LOA' descricao, sum(bed.bedvalor) valor
                from emendas.emenda eme
                        inner join emendas.autor aut on aut.autid = eme.autid
                        left join territorios.estado est on est.estuf = aut.estuf
                        left join territorios.regiao reg on reg.regcod = est.regcod
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                where
                    eme.emestatus = 'A'
                    AND ben.benstatus = 'A'
                    AND eme.prsano = '$exercicio'
                group by descricao, reg.regcod, categoria
                union all
                select  case when coalesce(reg.regdescricao, '') = '' then 'A Identificar' else  reg.regdescricao end categoria, reg.regcod,
                        'Valor Após Contingenciamento' descricao, sum(bed.bedvalorpriorizado) valor
                from emendas.emenda eme
                        inner join emendas.autor aut on aut.autid = eme.autid
                        left join territorios.estado est on est.estuf = aut.estuf
                        left join territorios.regiao reg on reg.regcod = est.regcod
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                where
                    eme.emestatus = 'A'
                    AND ben.benstatus = 'A'
                    AND eme.prsano = '$exercicio'
                group by descricao, reg.regcod, categoria
                order by regcod, categoria desc";

        $dadosRegiao = $this->carregar($sql);
        $dadosRegiao = $dadosRegiao ? $dadosRegiao : [];

        $dadosAgrupadosRegiao = [];
        foreach ($dadosRegiao as $dado) {
            $dadosAgrupadosRegiao[$dado['categoria']][$dado['descricao']] = $dado['valor'];
        }

        include_once APPRAIZ . 'emendas/modulos/principal/grafico_uf.inc';
    }

    public function gerarGraficoPartido($exercicio)
    {
        $sql = "select  case when coalesce(par.parsigla, '') = '' then 'A Identificar' else par.parsigla end categoria, par.pardsc,
                        'Valor LOA' descricao, sum(bed.bedvalor) valor
                from emendas.emenda eme
                        inner join emendas.autor aut on aut.autid = eme.autid
                        inner join emendas.partido par on par.parid = aut.parid
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                where
                    eme.emestatus = 'A'
                    AND ben.benstatus = 'A'
                    AND eme.prsano = '$exercicio'
                group by categoria, par.pardsc, descricao
                union all
                select  case when coalesce(par.parsigla, '') = '' then 'A Identificar' else par.parsigla end categoria, par.pardsc,
                        'Valor Após Contingenciamento' descricao, sum(bed.bedvalorpriorizado) valor
                from emendas.emenda eme
                        inner join emendas.autor aut on aut.autid = eme.autid
                        inner join emendas.partido par on par.parid = aut.parid
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                where
                    eme.emestatus = 'A'
                    AND ben.benstatus = 'A'
                    AND eme.prsano = '$exercicio'
                group by categoria, par.pardsc, descricao
                order by categoria, descricao desc";

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];

        $dadosAgrupados = [];
        foreach ($dados as $dado) {
            $dadosAgrupados[$dado['categoria']][$dado['descricao']] = $dado['valor'];
        }

        $sql = "select  case when coalesce(par.parlado, '') = '' then 'A Identificar' else par.parlado end categoria,
                        'Valor LOA' descricao, sum(bed.bedvalor) valor
                from emendas.emenda eme
                        inner join emendas.autor aut on aut.autid = eme.autid
                        inner join emendas.partido par on par.parid = aut.parid
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                where
                    eme.emestatus = 'A'
                    AND ben.benstatus = 'A'
                    AND eme.prsano = '$exercicio'
                group by categoria, descricao
                union all
                select  case when coalesce(par.parlado, '') = '' then 'A Identificar' else par.parlado end categoria,
                        'Valor Após Contingenciamento' descricao, sum(bed.bedvalorpriorizado) valor
                from emendas.emenda eme
                        inner join emendas.autor aut on aut.autid = eme.autid
                        inner join emendas.partido par on par.parid = aut.parid
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                where
                    eme.emestatus = 'A'
                    AND ben.benstatus = 'A'
                    AND eme.prsano = '$exercicio'
                group by categoria, descricao
                order by categoria, descricao desc";

        $dadosLado = $this->carregar($sql);
        $dadosLado = $dadosLado ? $dadosLado : [];

        $dadosAgrupadosLado = [];
        foreach ($dadosLado as $dado) {
            $dadosAgrupadosLado[$dado['categoria']][$dado['descricao']] = $dado['valor'];
        }

        include_once APPRAIZ . 'emendas/modulos/principal/grafico_partido.inc';
    }

    public function gerarGraficoSexo($exercicio, $tipo = null)
    {
        $sql = "select  case when aut.autsexo = 'M' then 'Homem' when aut.autsexo = 'F' then 'Mulher' else 'A Identificar' end categoria,
                        'Valor LOA' descricao, sum(bed.bedvalor) valor
                from emendas.emenda eme
                        inner join emendas.autor aut on aut.autid = eme.autid
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                where
                    eme.emestatus = 'A'
                    AND ben.benstatus = 'A'
                    AND eme.prsano = '$exercicio'
                group by categoria, descricao
                union all
                select  case when aut.autsexo = 'M' then 'Homem' when aut.autsexo = 'F' then 'Mulher' else 'A Identificar' end categoria,
                        'Valor Após Contingenciamento' descricao, sum(bed.bedvalorpriorizado) valor
                from emendas.emenda eme
                        inner join emendas.autor aut on aut.autid = eme.autid
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                where
                    eme.emestatus = 'A'
                    AND ben.benstatus = 'A'
                    AND eme.prsano = '$exercicio'
                group by categoria, descricao
                order by categoria, descricao desc";

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];

        $dadosAgrupados = [];
        foreach ($dados as $dado) {
            $dadosAgrupados[$dado['categoria']][$dado['descricao']] = $dado['valor'];
        }


        $item = 'Gênero';
        if($tipo){
            $colors =  "'#ff0066', '#00BFFF', '#FFD700'";

            $sqlPizza = "select case when aut.autsexo = 'M' then 'Homem' when aut.autsexo = 'F' then 'Mulher' else 'A Identificar' end descricao, sum(bed.bedvalor) valor
                    from emendas.emenda eme
                            inner join emendas.autor aut on aut.autid = eme.autid
                            inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                            inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                    where eme.prsano = '$exercicio'
                    group by descricao";
            include APPRAIZ . 'emendas/modulos/principal/grafico_pizza.inc';

        } else {
            include APPRAIZ . 'emendas/modulos/principal/grafico_coluna.inc';
        }
    }

    public function gerarGraficoGnd($exercicio, $tipo = null)
    {
        $sql = "select  gnd.gnddsc categoria, gnd.gndcod, 'Valor LOA' descricao, sum(bed.bedvalor) valor
                from emendas.emenda eme
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                        inner join emendas.emendadetalhe emd on bed.emdid = emd.emdid
                        inner join public.gnd gnd on gnd.gndcod = emd.gndcod
                where
                    eme.emestatus = 'A'
                    AND ben.benstatus = 'A'
                    AND eme.prsano = '$exercicio'
                group by categoria, gnd.gndcod, descricao
                union all
                select  gnd.gnddsc categoria, gnd.gndcod, 'Valor Após Contingenciamento' descricao, sum(bed.bedvalorpriorizado) valor
                from emendas.emenda eme
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                        inner join emendas.emendadetalhe emd on bed.emdid = emd.emdid
                        inner join public.gnd gnd on gnd.gndcod = emd.gndcod
                where
                    eme.emestatus = 'A'
                    AND ben.benstatus = 'A'
                    AND eme.prsano = '$exercicio'
                group by categoria, gnd.gndcod, descricao
                order by gndcod, categoria, descricao desc";

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];

        $dadosAgrupados = [];
        foreach ($dados as $dado) {
            $dadosAgrupados[$dado['categoria']][$dado['descricao']] = $dado['valor'];
        }

        $item = 'GND';

        if($tipo){
            $sqlPizza = "select  gnd.gnddsc descricao, sum(bed.bedvalor) valor
                    from emendas.emenda eme
                            inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                            inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                            inner join emendas.emendadetalhe emd on bed.emdid = emd.emdid
                            inner join public.gnd gnd on gnd.gndcod = emd.gndcod
                    where eme.prsano = '$exercicio'
                    group by descricao";
            include APPRAIZ . 'emendas/modulos/principal/grafico_pizza.inc';

        } else {
            include APPRAIZ . 'emendas/modulos/principal/grafico_coluna.inc';
        }
    }

    public function atualizarSiconv($benid = NULL)
    {

        $sql = "
            SELECT
                eme.*,
                uno.unocodorcamento
            FROM emendas.emenda eme
                JOIN public.unidadeorcamentaria uno ON uno.unoid = eme.unoid
            WHERE
                eme.emeid = {$this->emeid}
        ";
        $emenda = $this->pegaLinha($sql);

        # Caso seja passado o código do Beneficiario o Sistema verifica se existe Subunidade delegada
        if($benid){
            $beneficiario = new Emendas_Model_Beneficiario($benid);
            $sqlSubUnidadeDelegada = $beneficiario->montarSqlRecuperarDelegacao((object)array('exercicio' => $_SESSION['exercicio']));
            $resultadoSubUnidadeDelegada = $this->pegaLinha($sqlSubUnidadeDelegada);
            # Caso exista Unidade delegada o Sistema insere o código orcamento da UO da Subunidade delegada para buscar a proposta SICONV
            if($resultadoSubUnidadeDelegada){
                $subUnidadeDelegada = (object)$resultadoSubUnidadeDelegada;
                $emenda['unocodorcamento'] = $subUnidadeDelegada->unocodorcamento;
            }
        }

        $mUsuarioSiconv = (new Emendas_Model_UsuarioSiconv())->RetornaConfiguracaoSiconv();
        $usuario = $mUsuarioSiconv['usucpf'];
        $senha = md5(base64_decode($mUsuarioSiconv['ususenhasiconv']));
        
        $arrParam = array(
            'usuario' => $usuario,
            'senha'   => $senha,
            'url'     => WEB_SERVICE_SICONV_URL,
        );
        $obWS = new WSIntegracaoSiconv($arrParam);

        $sql = "delete from siconv.propostaws";
        $this->executar($sql);
                                                                                                                                                                                                                                                                                                            
        if($emenda){
            try{
                $teste = $this->recuperarPropostaWs($obWS, $emenda);
            }catch(Exception $e){
                switch (trim($e->getMessage())) {
                    case ERR_USUARIO_INCORRETO:
                        $mensagem = "O usuário ou senha estão incorretos para conexão com o WebService do SICONV!";
                        break;
                    case ERR_QUANTIDADE_VEZES_EXCEDIDA:
                        $mensagem = "O sistema recebeu mais de uma solicitação de atualização em menos de 5 segundos. Tente novamente daqui alguns instantes!";
                        break;
                    default:
                        $mensagem = "Erro ao conectar ao WebService do SICONV.";
                        break;
                }
                simec_redirecionar('?modulo=principal/beneficiario_form&acao=A&benid=7966&benid='. $benid, 'error', $mensagem);
            }
            $mSiconv = new Emendas_Model_Siconv();
            $mSiconv->atualizar();
        }

        $this->emeatualizacaosiconv = date('Y-m-d H:i:s');
        $this->salvar();
        $this->commit();
    }

    public function atualizarPrograma()
    {
     
        $mPrograma = new Emendas_Model_Programa();
        $programas = $mPrograma->recuperarTodos();

        $mUsuarioSiconv = (new Emendas_Model_UsuarioSiconv())->RetornaConfiguracaoSiconv();
        $usuario = $mUsuarioSiconv['usucpf'];
        $senha = md5(base64_decode($mUsuarioSiconv['ususenhasiconv']));

        $arrParam = array(
            'usuario' => $usuario,
            'senha' 	=> $senha,
            'url' 	=> WEB_SERVICE_SICONV_URL,
        );
        $obWS = new WSIntegracaoSiconv($arrParam);

        foreach ($programas as $programa){

            $filtro = [
                'prgcodigo'=>$programa['prgcodigo']
            ];

            $mPrograma->atualizar($obWS, $filtro);
            $mPrograma->commit();
            if(count($programas)>1){
                sleep(5);
            }
        }
    }

    public function recuperarPropostaWs($obWS, $emenda)
    {
        $filtro = [
            'unocod'=>$emenda['unocodorcamento'],
            'emenumero'=>substr($emenda['emenumero'], 4)
        ];
        $retorno = $obWS->consultarPropostaPorEmenda($filtro);

        if(isset($retorno->propostas)){

            $aPropostas = is_array($retorno->propostas) ? $retorno->propostas : [$retorno->propostas];
            foreach ($aPropostas as $PropostaWS){

                $mPropostaWS = new Siconv_Model_PropostaWs();

                foreach ($PropostaWS as $indice => $dados){
                    $indice = strtolower($indice);

                    if(strpos($indice, 'data')!== false){

                    } elseif(
                        'ws' == substr($indice, -2) ||
                        in_array($indice, ['parecerplanotrabalho', 'parecerproposta'])
                    ){
                        $dados = json_encode($dados);
                    }

                    if(key_exists($indice, $mPropostaWS->arAtributos)){
                        $mPropostaWS->{$indice} = $dados;
                    }
                }

                $mPropostaWS->emeid = $emenda['emeid'];
                $mPropostaWS->salvar();
                $mPropostaWS->commit();

                unset($mPropostaWS);
            }
        }
    }

}
