<?php

class Emendas_Controller_Beneficiario
{
    public function salvar($dados_post)
    {
        $url = '?modulo=principal/beneficiario_form&acao=A';

        try {
            $mBeneficiario = new Emendas_Model_Beneficiario($dados_post['benid']);

            # Verifica se está sendo feita alteração.
            if ($dados_post['benid']):
                $mBeneficiarioHistorico = new Emendas_Model_BeneficiarioHistorico();

                $mBeneficiarioHistorico->benid = $mBeneficiario->benid;
                $mBeneficiarioHistorico->usucpf = $_SESSION['usucpf'];
                $mBeneficiarioHistorico->behdata = 'NOW()';
                $behid = $mBeneficiarioHistorico->salvar();

                $banco = $mBeneficiario->buscarBeneficiario($dados_post['benid']);
                $mBeneficiarioHistoricoAlteracao = new Emendas_Model_BeneficiarioHistoricoAlteracao();

                if ($banco['suoid'] != $dados_post['suoid']):

//                    $suoid[] = array();
//                    $suoid['behid'] = $behid;
//                    $suoid['bahcampo'] = 'SubUnidade';
//                    $suoid['bahanterior'] = $banco['suoid'];
//                    $suoid['bahnovo'] = $dados_post['suoid'];
//                    $mBeneficiarioHistorico->popularDadosObjeto($suoid);
                    $mBeneficiarioHistoricoAlteracao->behid = $behid;
                    $mBeneficiarioHistoricoAlteracao->bahcampo = 'SubUnidade';
                    $mBeneficiarioHistoricoAlteracao->bahanterior = $banco['suoid'];
                    $mBeneficiarioHistoricoAlteracao->bahnovo = $dados_post['suoid'];
                    $mBeneficiarioHistoricoAlteracao->salvar();
                endif;
                if ($banco['delegacao'] != $dados_post['delegacao'] && $banco['delegacao'] != NULL):

//                    $delegacao[] = array();
//                    $delegacao['behid'] = $behid;
//                    $delegacao['bahcampo'] = 'Delegacao';
//                    $delegacao['bahanterior'] = $banco['delegacao'];
//                    $delegacao['bahnovo'] = $dados_post['delegacao'];
//                    $mBeneficiarioHistorico->popularDadosObjeto($delegacao);
                    $mBeneficiarioHistoricoAlteracao->behid = $behid;
                    $mBeneficiarioHistoricoAlteracao->bahcampo = 'Delegacao';
                    $mBeneficiarioHistoricoAlteracao->bahanterior = $banco['delegacao'];
                    $mBeneficiarioHistoricoAlteracao->bahnovo = $dados_post['delegacao'];
                    $mBeneficiarioHistoricoAlteracao->salvar();

                endif;
                if ($banco['proid'] != $dados_post['proid']):

//                    $proid[] = array();
//                    $proid['behid'] = $behid;
//                    $proid['bahcampo'] = 'Proponente';
//                    $proid['bahanterior'] = $banco['proid'];
//                    $proid['bahnovo'] = $dados_post['proid'];
//                    $mBeneficiarioHistorico->popularDadosObjeto($proid);
                    $mBeneficiarioHistoricoAlteracao->behid = $behid;
                    $mBeneficiarioHistoricoAlteracao->bahcampo = 'Proponente';
                    $mBeneficiarioHistoricoAlteracao->bahanterior = $banco['proid'];
                    $mBeneficiarioHistoricoAlteracao->bahnovo = $dados_post['proid'];
                    $mBeneficiarioHistoricoAlteracao->salvar();
                endif;
                if ($banco['esfid'] != $dados_post['esfid']):

//                    $esfid[] = array();
//                    $esfid['behid'] = $behid;
//                    $esfid['bahcampo'] = 'Localização';
//                    $esfid['bahanterior'] = $banco['esfid'];
//                    $esfid['bahnovo'] = $dados_post['esfid'];
//                    $mBeneficiarioHistorico->popularDadosObjeto($esfid);
                    $mBeneficiarioHistoricoAlteracao->behid = $behid;
                    $mBeneficiarioHistoricoAlteracao->bahcampo = 'Localização';
                    $mBeneficiarioHistoricoAlteracao->bahanterior = $banco['esfid'];
                    $mBeneficiarioHistoricoAlteracao->bahnovo = $dados_post['esfid'];
                    $mBeneficiarioHistoricoAlteracao->salvar();

                endif;
                if ($banco['bennumeroprocesso'] != $dados_post['bennumeroprocesso']):

//                    $bennumeroprocesso[] = array();
//                    $bennumeroprocesso['behid'] = $behid;
//                    $bennumeroprocesso['bahcampo'] = 'Nº do Processo';
//                    $bennumeroprocesso['bahanterior'] = $banco['bennumeroprocesso'];
//                    $bennumeroprocesso['bahnovo'] = $dados_post['bennumeroprocesso'];
//                    $mBeneficiarioHistorico->popularDadosObjeto($bennumeroprocesso);
                    $mBeneficiarioHistoricoAlteracao->behid = $behid;
                    $mBeneficiarioHistoricoAlteracao->bahcampo = 'Nº do Processo';
                    $mBeneficiarioHistoricoAlteracao->bahanterior = $banco['bennumeroprocesso'];
                    $mBeneficiarioHistoricoAlteracao->bahnovo = $dados_post['bennumeroprocesso'];
                    $mBeneficiarioHistoricoAlteracao->salvar();

                endif;
                $banco['beninicio'] = formata_data($banco['beninicio'], '%d/%m/%Y');

                if ($banco['beninicio'] != $dados_post['beninicio']):

//                    $beninicio[] = array();
//                    $beninicio['behid'] = $behid;
//                    $beninicio['bahcampo'] = 'Data de Início';
//                    $beninicio['bahanterior'] = $banco['beninicio'];
//                    $beninicio['bahnovo'] = $dados_post['beninicio'];
//                    $mBeneficiarioHistorico->popularDadosObjeto($beninicio);
                    $mBeneficiarioHistoricoAlteracao->behid = $behid;
                    $mBeneficiarioHistoricoAlteracao->bahcampo = 'Data de Início';
                    $mBeneficiarioHistoricoAlteracao->bahanterior = $banco['beninicio'];
                    $mBeneficiarioHistoricoAlteracao->bahnovo = $dados_post['beninicio'];
                    $mBeneficiarioHistoricoAlteracao->salvar();

                endif;
                if ($banco['capid'] != $dados_post['capid']):

//                    $capid[] = array();
//                    $capid['behid'] = $behid;
//                    $capid['bahcampo'] = 'Modalidade de Pactuação';
//                    $capid['bahanterior'] = $banco['capid'];
//                    $capid['bahnovo'] = $dados_post['capid'];
//                    $mBeneficiarioHistorico->popularDadosObjeto($capid);
                    $mBeneficiarioHistoricoAlteracao->behid = $behid;
                    $mBeneficiarioHistoricoAlteracao->bahcampo = 'Modalidade de Pactuação';
                    $mBeneficiarioHistoricoAlteracao->bahanterior = $banco['capid'];
                    $mBeneficiarioHistoricoAlteracao->bahnovo = $dados_post['capid'];
                    $mBeneficiarioHistoricoAlteracao->salvar();
                endif;
            endif;

            $mBeneficiarioHistoricoAlteracao->commit();
            $mBeneficiario->popularDadosObjeto($dados_post);

            $mBeneficiario->benempenhado = $dados_post['benempenhado'] == 't' ? 't' : 'f';
            $mBeneficiario->benconveniado = $dados_post['benconveniado'] == 't' ? 't' : 'f';
            $mBeneficiario->benpago = $dados_post['benpago'] == 't' ? 't' : 'f';
            $mBeneficiario->bented = $dados_post['bented'] == 't' ? 't' : 'f';

            $aCamposNulos = [
                # Cadastro Inicial
                'suoid', 'proid', 'esfid', 'benformaexecucao', 'bennumeroprocesso', 'beninicio', 'benpprogramanumero', 'benpropostanumero', 'benpropostatitulo', 'benpropostaobjeto', 'impid', 'mdeid', 'neeid', 'capid', 'bented',
                # Cadastro Complementar pra o PI
                'plititulo', 'plidsc', 'oppid', 'mppid', 'ippid', 'mpnid', 'ipnid', 'pprid', 'pumid', 'picquantidade'
            ];
            $mBeneficiario->salvar(null, null, $aCamposNulos);

            if($dados_post['req'] == 'salvar-complemento'){
                $mBeneficiario->associarSniic($dados_post);
                $mBeneficiario->associarPronac($dados_post);
                $mBeneficiario->associarAnexo($dados_post);
            } else {
                $mBeneficiario->associarDelegacao($dados_post);
                $mBeneficiario->associarAlteracao($dados_post);

                // Salvando dados da Localização
                $dados_post['benid'] = $mBeneficiario->benid;
                (new Emendas_Controller_BeneficiarioLocalizacao())->salvar($dados_post);
            }

            $mBeneficiario->commit();
            simec_redirecionar($url. '&benid='. $mBeneficiario->benid. ($dados_post['req'] == 'salvar-complemento'? '&aba=1': NULL), 'success');
        } catch (Exception $e){
            $mBeneficiario->rollback();
            simec_redirecionar($url. ($dados_post['req'] == 'salvar-complemento'? '&aba=1': NULL), 'error');
        }
    }

    public function excluir($benid)
    {
        $url = '?modulo=principal/beneficiario_form&acao=A&benid='. (int)$benid;

        try {
            $mBeneficiario = new Emendas_Model_Beneficiario($benid);

            $url = '?modulo=principal/emenda_form&acao=A&emeid='. (int)$mBeneficiario->emeid;

            $mBeneficiario->benstatus = 'I';
            $mBeneficiario->salvar();
            $mBeneficiario->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mBeneficiario->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function montarFormularioObservacao($benid = null, $bobid = null)
    {
        global $simec;
        $mBeneficiarioObservacao = new Emendas_Model_BeneficiarioObservacao($bobid);
        if(!$mBeneficiarioObservacao->benid){ $mBeneficiarioObservacao->benid = $benid; }
        $mBeneficiarioObservacao->bobdata = $mBeneficiarioObservacao->bobdata ? $mBeneficiarioObservacao->bobdata : date('d/m/Y');
        include_once APPRAIZ . 'emendas/modulos/principal/beneficiarioobservacao_form.inc';
    }

    public function montarListagemObservacao($benid, $podeEditar)
    {
        $aBeneficiarioObservacao = (new Emendas_Model_BeneficiarioObservacao)->recuperarPorUnidade($benid);
        include_once APPRAIZ . 'emendas/modulos/principal/beneficiario_observacao.inc';
    }

    public function montarFormularioDetalhe($emeid, $benid = null, $bedid = null)
    {
        global $simec;
        $mBeneficiarioDetalhe = new Emendas_Model_BeneficiarioDetalhe($bedid);
        $emeid = $emeid;
        if(!$mBeneficiarioDetalhe->benid){
            $mBeneficiarioDetalhe->benid = $benid;
        }
        $mBeneficiario = new Emendas_Model_Beneficiario($mBeneficiarioDetalhe->benid);
        include_once APPRAIZ . 'emendas/modulos/principal/beneficiariodetalhe_form.inc';
    }

    public function montarListagemDetalhe($benid, $podeEditar)
    {
        $aBeneficiarioDetalhe = (new Emendas_Model_BeneficiarioDetalhe)->recuperarListagem($benid);
        include_once APPRAIZ . 'emendas/modulos/principal/beneficiariodetalhe.inc';
    }

    public function montarListagemSiconv($benid)
    {
        $aSiconv = (new Emendas_Model_Siconv)->recuperarListagem($benid);
        include_once APPRAIZ . 'emendas/modulos/principal/siconv.inc';
    }

    public function montarDetalheSiconv($sicid)
    {
        $aSiconv = (new Emendas_Model_Beneficiario())->recuperarDadosSiconv($sicid);
        include_once APPRAIZ . 'emendas/modulos/principal/siconv-detalhe.inc';
    }

    public function atualizarSiconv($emeid, $benid)
    {
        $mEmenda = new Emendas_Model_Emenda($emeid);
        $mEmenda->atualizarSiconv($benid);
        simec_redirecionar('?modulo=principal/beneficiario_form&acao=A&benid=7966&benid='. $benid, 'success');
    }

    public function atualizarPrograma($emeid)
    {
        $mEmenda = new Emendas_Model_Emenda($emeid);
        $mEmenda->atualizarPrograma();
//        ver($benid, $mEmenda, d);
    }
}
