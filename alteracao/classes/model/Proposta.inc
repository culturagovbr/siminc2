<?php
/**
 * Classe de mapeamento da entidade proposta.proposta
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <teste@teste.com.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-04-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Proposta_Model_Proposta
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Proposta_Model_Proposta extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'proposta.proposta';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'proid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'ptrid' => array('tabela' => 'ptres', 'pk' => 'ptrid'),
        'eqdid' => array('tabela' => 'pi_enquadramentodespesa', 'pk' => 'eqdid'),
        'suoid' => array('tabela' => 'subunidadeorcamentaria', 'pk' => 'suoid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'proid' => null,
        'suoid' => null,
        'eqdid' => null,
        'ptrid' => null,
        'proquantidade' => null,
        'proquantidadeexpansao' => null,
        'projustificativa' => null,
        'projustificativaexpansao' => null,
        'prostatus' => null,
        'prsano' => null,
    );


    public function recuperarListagem(stdClass $filtros, $filtroprop)
    {
        $where = '';
        if($filtros->listaSubUnidadeUsuario && !empty(join_simec(',', $filtros->listaSubUnidadeUsuario))){
            $where .= "\n AND suo.suocod::INTEGER IN(". join_simec(',', $filtros->listaSubUnidadeUsuario). ") ";
        }
        if ($filtroprop['suoid'])$where .= "\n AND suo.suoid = ".$filtroprop['suoid'];
        if ($filtroprop['ptrid'])$where .= "\n AND pro.ptrid = ".$filtroprop['ptrid'];
        if ($filtroprop['eqdid'])$where .= "\n AND pro.eqdid = ".$filtroprop['eqdid'];
        $sql = "
            SELECT
                pro.proid,
                ptr.funcional,
                ptr.acatitulo,
                ptr.plodsc,
                eqd.eqddsc,
                suo.unosigla || ' - ' || suo.suonome subunidade,
                pro.proquantidade,
                pro.proquantidadeexpansao,
                sum(prd.prdvalor) prdvalor,
                sum(prd.prdvalorexpansao) prdvalorexpansao
            FROM proposta.proposta pro
                JOIN monitora.vw_ptres ptr ON pro.ptrid = ptr.ptrid
                JOIN public.vw_subunidadeorcamentaria suo ON suo.suoid = pro.suoid
                JOIN planejamento.enquadramento_despesa eqd ON eqd.eqdid = pro.eqdid
                LEFT JOIN proposta.propostadetalhe prd ON prd.proid = pro.proid AND prd.prdstatus = 'A'
            WHERE
                prostatus = 'A'
                AND pro.prsano = '". (int)$filtros->exercicio. "'
                $where
            GROUP BY
                pro.proid,
                ptr.funcional,
                ptr.acatitulo,
                ptr.plodsc,
                eqd.eqddsc,
                subunidade,
                pro.proquantidade,
                pro.proquantidadeexpansao
        ";
        return $this->carregar($sql);
    }

    public static function montarSqlEnvioSiop($exercicio, $momento){
        $sql = "
            SELECT DISTINCT
                a.identificadorunico identificadorunicoacao,
                a.codigoesfera,
                aca.unicod,
                suo.suocod,
                suo.unosigla,
                suo.suonome,
                suo.suosigla,
                aca.prgcod,
                a.codigofuncao,
                a.codigosubfuncao,
                aca.acaid,
                aca.acacod,
                aca.loccod,
                aca.locquantidadeproposta,
                ptr.plocod,
                ndp.ndpcod,
                fr.foncod,
                idu.iducod,
                idoc.idocod,
                prdvalor,
                pro.proquantidade,
                po.identificadorunico identificadorunicopo,
                pro.projustificativa,
                pro.proquantidadeexpansao,
                pro.projustificativaexpansao,
                pro.eqdid,
                tpd.tpdcod,
                prd.prdvalorexpansao
            FROM proposta.proposta pro
                JOIN public.vw_subunidadeorcamentaria suo ON(pro.suoid = suo.suoid)
                JOIN proposta.propostadetalhe prd ON(
                    prd.prdstatus = 'A'
                    AND prd.proid = pro.proid
                )
                JOIN public.naturezadespesa ndp ON ndp.ndpid = prd.ndpid
                JOIN public.idoc ON idoc.idoid = prd.idoid
                JOIN monitora.ptres ptr ON ptr.ptrid = pro.ptrid
                JOIN monitora.acao aca ON aca.acaid = ptr.acaid
                JOIN wssof.ws_acoesdto a ON(
                    a.exercicio = pro.prsano::int
                    AND a.codigomomento = ". (int)$momento. "
                    AND a.codigoorgao = aca.unicod
                    AND a.codigoacao = aca.acacod
                    AND a.codigoprograma = aca.prgcod
                )
                JOIN planejamento.enquadramento_despesa eqd ON(pro.eqdid = eqd.eqdid)
                JOIN proposta.tipodetalhamento tpd ON(eqd.tpdid = tpd.tpdid)
                LEFT JOIN wssof.ws_planosorcamentariosdto po ON(
                    po.identificadorunicoacao = a.identificadorunico
                    AND po.exercicio = pro.prsano::int
                    AND po.planoorcamentario = ptr.plocod
                )
                LEFT JOIN public.fonterecurso fr ON prd.fonid = fr.fonid
                LEFT JOIN public.identifuso idu ON prd.iduid = idu.iduid
            WHERE
                pro.prsano = '". (int)$exercicio. "'
                AND pro.prostatus = 'A'
            ORDER BY
                aca.prgcod,
                aca.unicod,
                aca.acacod,
                aca.loccod,
                ptr.plocod
        ";
//ver($sql, d);
        return $sql;
    }
    
    public function recuperarDadosProposta($exercicio, $momento)
    {
        /********************************************************************************************************************************************************
         *                                                                                                                                                       *
         *  DOCUMENTAÇÃO DO SIOP PARA MONTAGEM DOS DADOS: https://www1.siop.planejamento.gov.br/siopdoc/doku.php/webservices:manual-wsquantitativo#propostadto   *
         *                                                                                                                                                       *
         ********************************************************************************************************************************************************/

        $sql = self::montarSqlEnvioSiop($exercicio, $momento);
        $dados = $this->carregar($sql);

        $aEnvioWS = [];
        foreach ($dados as $dado) {
            $agrupador = $dado['tpdcod']. '_'. $dado['unicod']. '_'. $dado['acacod']. '_'. $dado['loccod']. '_'. $dado['prgcod']. '_'. $dado['identificadorunicoacao'];

            # Cálculos auxiliares
            $aEnvio[$agrupador]['valorFisico'] += $dado['locquantidadeproposta'];
            $aEnvio[$agrupador]['quantidadeFisico'][$dado['proid']] = 0;
            $aEnvio[$agrupador]['quantidadeFisicoPo'][$dado['plocod']][$dado['proid']] = $dado['proquantidade'];
            $aEnvio[$agrupador]['expansaoFisicaSolicitada'][$dado['proid']] = 0;
            $aEnvio[$agrupador]['expansaoFisicaSolicitadaPo'][$dado['plocod']][$dado['proid']] = $dado['proquantidadeexpansao'];
            
            # Incrementando cada Justificativa dentro dos POs do registro agrupado(Funcional do SIOP até o nível de localizador)
            if(!empty($dado['projustificativa'])){
                $aEnvio[$agrupador]['lista_po_justificativa'][$dado['plocod']]['justificativa'] = $dado['projustificativa'];
            }
            $aEnvio[$agrupador]['propostas'][] = $dado['proid'];

            /**
             * @TODO Verificar se pode passar a justificativa. Por enquanto não está passando
             */
            if($dado['proquantidadeexpansao']){
                $aEnvio[$agrupador]['justificativaExpansaoSolicitada'][$dado['proid']] = $dado['projustificativaexpansao'];
            }

            # Fix para limpar idOC cadastrados errados
            $idOCvalidos = array('0001', '0002', '0003', '0004', '0005', '9999');
            if (!in_array($dado['idocod'], $idOCvalidos)) {
                $dado['idocod'] = '9999';
            }

            # Montando no loop o array de FinanceiroDTO
            $aEnvio[$agrupador]['FinanceiroDTO'][] = [
                'idUso'                          => $dado['iducod'],
                'idOC'                           => $dado['idocod'],
                'naturezaDespesa'                => $dado['ndpcod'],
                'fonte'                          => $dado['foncod'],
                'resultadoPrimarioLei'           => $dado['eqdcodigorp'],
                'resultadoPrimarioAtual'         => $dado['eqdcodigorp'], // aceita nulo
                'identificadorPlanoOrcamentario' => $dado['identificadorunicopo'],
                'valor'                          => number_format($dado['prdvalor'], 0, '', ''),
                'expansaoSolicitada'             => ($dado['prdvalorexpansao'] > 0 ? $dado['prdvalorexpansao'] : null),
            ];

            # Montando no loop o array de FinanceiroDTO
            $aEnvio[$agrupador]['MetaPlanoOrcamentarioDTO'][$dado['plocod']] = [
                'identificadorUnicoPlanoOrcamentario' => $dado['identificadorunicopo'],
                'quantidadeFisico' => (array_sum($aEnvio[$agrupador]['quantidadeFisicoPo'][$dado['plocod']]) == 0? NULL: array_sum($aEnvio[$agrupador]['quantidadeFisicoPo'][$dado['plocod']])),
                'expansaoFisicaSolicitada' => ($aEnvio[$agrupador]['expansaoFisicaSolicitadaPo'][$dado['plocod']] > 0 ? array_sum($aEnvio[$agrupador]['expansaoFisicaSolicitadaPo'][$dado['plocod']]) : null),
                'expansaoFisicaConcedida' => null
            ];

            if(count($aEnvio[$agrupador]['lista_po_justificativa']) > 1){
                $justificativa = '';
                foreach($aEnvio[$agrupador]['lista_po_justificativa'] as $plocod => $po){
                    $justificativa .= 'Justificativa do PO '. $plocod. ': '. $po['justificativa']. "\n";
                }
            } else {
                $justificativa = substr($dado['projustificativa'], 0, 4999);
            }
            
            # Motagem do Array de PropostaDTO
            $aEnvioWS[$agrupador] = [
                'exercicio'                       => $exercicio,
                'codigoEsfera'                    => $dado['codigoesfera'],
                'codigoOrgao'                     => $dado['unicod'],
                'subUnidade'                      => $dado['unosigla']. ' - '. $dado['suonome']. ($dado['suosigla']?' - '. $dado['suosigla']: NULL). ' ('. $dado['suocod']. ')',
                'codigoFuncao'                    => $dado['codigofuncao'],
                'codigoSubFuncao'                 => $dado['codigosubfuncao'],
                'codigoPrograma'                  => $dado['prgcod'],
                'codigoAcao'                      => $dado['acacod'],
                'codigoTipoInclusaoAcao'          => '1',                                                       // Valores válidos: 1 - PLOA, 2 - Emenda, 4 - PPA, 5 - Crédito Especial, 6 - Crédito Extraordinário
                'codigoLocalizador'               => $dado['loccod'],
                'codigoTipoInclusaoLocalizador'   => '1',                                                       // Valores válidos: 1 - PLOA, 2 - Emenda, 4 - PPA, 5 - Crédito Especial, 6 - Crédito Extraordinário
                'codigoMomento'                   => $momento,                                                  // São os momentos (etapas) do processo no SIOP: 1000 (UO), 2000 (OS), 5000 (PLOA) e 8000 (LOA).
                'codigoTipoDetalhamento'          => $dado['tpdcod'],
                'quantidadeFisico'                => (array_sum($aEnvio[$agrupador]['quantidadeFisico']) == 0? NULL: array_sum($aEnvio[$agrupador]['quantidadeFisico'])),        // aceita nulo
                'valorFisico'                     => ($aEnvio[$agrupador]['valorFisico'] == 0? NULL: $aEnvio[$agrupador]['valorFisico']),                        // aceita nulo
                'financeiros'                     => $aEnvio[$agrupador]['FinanceiroDTO'],                        // lista de FinanceiroDTO
                // 'receitas'                        => null,                                                   // aceita nulo e lista de ReceitaDTO ?????????????????????????????
                'metaPlanoOrcamentario'           => array_values($aEnvio[$agrupador]['MetaPlanoOrcamentarioDTO']),
                'justificativa'                   => $justificativa,// substr($dado['projustificativa'], 0, 4999),   // aceita nulo
                'identificadorUnicoAcao'          => $dado['identificadorunicoacao'],                             // aceita nulo ?????????????????????????????
                'expansaoFisicaSolicitada'        => array_sum($aEnvio[$agrupador]['expansaoFisicaSolicitada']),
                'justificativaExpansaoSolicitada' => ($aEnvio[$agrupador]['justificativaExpansaoSolicitada'] ? implode('. ', $aEnvio[$agrupador]['justificativaExpansaoSolicitada']) : null),
                'tdpid'                           => $dado['tdpid'],
                'propostas'                       => array_unique($aEnvio[$agrupador]['propostas'])
            ];
        }

        return $aEnvioWS;
    }

    public function gerarPlanilhaImportacao($momento = 2000, $unoid = null)
    {
        require_once APPRAIZ . 'includes/php-excel/PHPExcel.php';
        $exercicio = $_SESSION['exercicio'];

        $aEnvioWS = simec_utf8_encode_recursive($this->recuperarDadosProposta($exercicio, $momento, $unoid));
//ver($aEnvioWS, d);
        $PHPExcel = new PHPExcel();

        # Criando Planilha "Financeiro"
        $planilhaFinanceiro = $PHPExcel->getActiveSheet();
        $planilhaFinanceiro->setTitle("Financeiro");

        # Criando Planilha "Físico Localizador"
        $planilhaFisicoLocalizador = $PHPExcel->createSheet();
        $planilhaFisicoLocalizador->setTitle(utf8_encode("Físico Localizador"));

        # Criando Planilha "Físico PO"
        $planilhaFisicoPo = $PHPExcel->createSheet();
        $planilhaFisicoPo->setTitle(utf8_encode("Físico PO"));


        $aPadrao = [
            'Momento' => 'codigoMomento',
            'Tipo de detalhamento' => 'codigoTipoDetalhamento',
            'Esfera' => 'codigoEsfera',
            'Unidade' => 'codigoOrgao',
            'SubUO' => 'subUnidade',
            'Programa' => 'codigoPrograma',
            'Função' => 'codigoFuncao',
            'SubFunção' => 'codigoSubFuncao',
            'Ação' => 'codigoAcao',
            'Localizador' => 'codigoLocalizador',
        ];

        $aFinanceiro = ['PO' => 'PO', 'Natureza' => 'naturezaDespesa', 'Fonte' => 'fonte', 'Iduso' => 'idUso', 'Idoc' => 'idOC', 'Valor financeiro' => 'valor'];
        $aLocalizador = ['Físico' => 'valorFisico', 'Justificativa' => 'justificativa'];
        $aPo = ['PO' => 'PO', 'Físico PO' => 'quantidadeFisico'];

        # Criando cabeçalhos
        $coluna = 0;
        foreach($aPadrao as $titulo => $campo){
            $planilhaFinanceiro->setCellValueByColumnAndRow($coluna,1,utf8_encode($titulo));
            $planilhaFisicoLocalizador->setCellValueByColumnAndRow($coluna,1,utf8_encode($titulo));
            $planilhaFisicoPo->setCellValueByColumnAndRow($coluna,1,utf8_encode($titulo));
            $coluna++;
        }
        # Cabeçalho Planilha "Financeiro"
        $coluna = 10;
        foreach($aFinanceiro as $titulo => $campo){
            $planilhaFinanceiro->setCellValueByColumnAndRow($coluna++,1,utf8_encode($titulo));
        }

        # Cabeçalho Planilha "Físico Localizador"
        $coluna = 10;
        foreach($aLocalizador as $titulo => $campo){
            $planilhaFisicoLocalizador->setCellValueByColumnAndRow($coluna++,1,utf8_encode($titulo));
        }
        # Cabeçalho Planilha "Físico PO"
        $coluna = 10;
        foreach($aPo as $titulo => $campo){
            $planilhaFisicoPo->setCellValueByColumnAndRow($coluna++,1,utf8_encode($titulo));
        }

        $aPoSiop = $this->carregar("select * from wssof.ws_planosorcamentariosdto where exercicio = '{$exercicio}' and codigomomento = {$momento}");
        $aPoSiopCodigo = [];
        foreach($aPoSiop as $po){
            $aPoSiopCodigo[$po['identificadorunico']] = $po['planoorcamentario'];
        }

        $linhaFinanceiro = $linhaLocalizador = $linhaPo = 2;
        foreach($aEnvioWS as $chave => $dadosPloa){

            # Dados da Planilha "Financeiro"
            foreach($dadosPloa['financeiros'] as $aCampo){
                $coluna = 0;
                foreach($aPadrao as $campo){
                    $dado = $campo == 'codigoMomento' ? utf8_encode('Órgão Setorial') : $dadosPloa[$campo];
                    $planilhaFinanceiro->setCellValueExplicitByColumnAndRow($coluna++,$linhaFinanceiro,$dado);
                }
                foreach($aFinanceiro as $campo){
                    $dado = $campo == 'PO' ? $aPoSiopCodigo[$aCampo['identificadorPlanoOrcamentario']] : $aCampo[$campo];
                    $planilhaFinanceiro->setCellValueExplicitByColumnAndRow($coluna++,$linhaFinanceiro,$dado);
                }
                $linhaFinanceiro++;
            }

            # Dados da Planilha "Físico Localizador"
            $coluna = 0;
            foreach(($aPadrao+$aLocalizador) as $campo){
                $dado = $campo == 'codigoMomento' ? utf8_encode('Órgão Setorial') : $dadosPloa[$campo];
                $planilhaFisicoLocalizador->setCellValueExplicitByColumnAndRow($coluna++,$linhaLocalizador,$dado);
            }

            $linhaLocalizador++;

            # Dados da Planilha "Físico PO"
            foreach($dadosPloa['metaPlanoOrcamentario'] as $aCampo){
                $coluna = 0;
                foreach($aPadrao as $campo){
                    $dado = $campo == 'codigoMomento' ? utf8_encode('Órgão Setorial') : $dadosPloa[$campo];
                    $planilhaFisicoPo->setCellValueExplicitByColumnAndRow($coluna++,$linhaPo,$dado);
                }
                foreach($aPo as $campo){
                    $dado = $campo == 'PO' ? $aPoSiopCodigo[$aCampo['identificadorUnicoPlanoOrcamentario']] : $aCampo[$campo];
                    $planilhaFisicoPo->setCellValueExplicitByColumnAndRow($coluna++,$linhaPo,$dado);
                }
                $linhaPo++;
            }
        }

        # Indicação da criação do ficheiro
        $objWriter = PHPExcel_IOFactory::createWriter($PHPExcel, 'Excel5');

        # Encaminhar o ficheiro resultante para abrir no browser ou fazer download
        header('Content-Type: application/vnd.ms-excel');
        header('Content-Disposition: attachment;filename="PlanilhaImportacao.xls"');
        header('Cache-Control: max-age=0');
        $objWriter->save('php://output');
    }
    
    Public Function recuperarSqlFuncional($suoid, $eqdid){
        if ($suoid){
            $sql = "
                SELECT distinct 
                    ptr.ptrid codigo,
                    coalesce(ptr.prgcod, '') || ' - ' || coalesce(ptr.acacod, '') || ' - ' || coalesce(ptr.loccod, '') || ' - ' || coalesce(ptr.acaobjetivocod, '') || ' - ' || coalesce(ptr.plocod, '') || ' - ' || ptr.acatitulo || ' - ' || ptr.plodsc descricao
                FROM monitora.vw_ptres ptr
                    JOIN spo.ptressubunidade psu ON psu.ptrid = ptr.ptrid
                    JOIN planejamento.enquadramentorp erp ON(
                        erp.irpcod = ptr.irpcod
                    )
                WHERE
                    ptr.ptrano = '{$_SESSION['exercicio']}'
                    AND ptr.ptrstatus = 'A'
                    AND ptr.plocod != 'EIND'
                    and psu.suoid = ".$suoid;
            if ($eqdid){
                $sql .= "\n and erp.eqdid = ".$eqdid;
            }                    
            $sql .= "\n ORDER BY descricao ";
            return $sql;        
        }else{
            return array();
        }
    }
}//end Class
?>