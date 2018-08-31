<?php

class Emendas_Controller_Beneficiario
{
    public function salvar($dados_post)
    {
        $url = '?modulo=principal/beneficiario_form&acao=A';

        try {
            $mBeneficiario = new Emendas_Model_Beneficiario($dados['benid']);
            $mBeneficiario->popularDadosObjeto($dados);

            $mBeneficiario->benempenhado = $dados['benempenhado'] == 't' ? 't' : 'f';
            $mBeneficiario->benconveniado = $dados['benconveniado'] == 't' ? 't' : 'f';
            $mBeneficiario->benpago = $dados['benpago'] == 't' ? 't' : 'f';
            $mBeneficiario->bented = $dados['bented'] == 't' ? 't' : 'f';
            $mBeneficiario->plititulo = substr($dados['plititulo'], 0, 230);

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
