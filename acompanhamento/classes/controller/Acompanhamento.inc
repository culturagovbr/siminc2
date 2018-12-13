<?php

class Acompanhamento_Controller_Acompanhamento
{
    public function salvar($dados, $urlModulo = NULL)
    {
        $url = '?modulo=principal/monitorar-ppa&acao=A';
        if(!empty($dados['ipnid'])){
            $url = '?modulo=principal/monitorar-pnc&acao=A';
        } elseif(!empty($dados['pliid'])){
            $url = $urlModulo. '&pliid='. $dados['pliid'];
        }

        try {
            $mAcompanhamento = new Acompanhamento_Model_Acompanhamento($dados['acoid']);
            $mAcompanhamento->popularDadosObjeto($dados);

            $mAcompanhamento->acoquantidade = $mAcompanhamento->acoquantidade ? desformata_valor($mAcompanhamento->acoquantidade) : null;
            $mAcompanhamento->acodata = $mAcompanhamento->acodata ? formata_data_sql($mAcompanhamento->acodata) : null;

            $mAcompanhamento->prsano = $mAcompanhamento->prsano ? $mAcompanhamento->prsano : $_SESSION['exercicio'];

            $mAcompanhamento->salvar(null, null, [
                'acoanalise',
                'acoprovidencias',
                'claid'
            ]);

            $mAcompanhamento->associarAnexo($dados);
            $this->_vincularMedidas($mAcompanhamento->acoid, $dados);
            $this->_gravarHistorico($mAcompanhamento->acoid);

            $mAcompanhamento->commit();
            simec_redirecionar($url. '&acoid='. $mAcompanhamento->acoid, 'success');
        } catch (Exception $e){
            $mAcompanhamento->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($acoid)
    {
        $url = '?modulo=principal/monitorar-ppa&acao=A';

        try {
            $mAcompanhamento = new Acompanhamento_Model_Acompanhamento($acoid);
            
            if($mAcompanhamento->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mAcompanhamento->acoid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mAcompanhamento->acostatus = 'I';
            $mAcompanhamento->salvar();
            $mAcompanhamento->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAcompanhamento->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    private function _vincularMedidas($acoid, $dados)
    {
        $mAcompanhamentoMedidas = new Acompanhamento_Model_AcompanhamentoMedida();
        $mAcompanhamentoMedidas->excluirVarios('acoid = ' . (int)$acoid);

        $mAcompanhamentoMedidas->acoid = $acoid;
        if(isset($dados['medidas']) && is_array($dados['medidas'])){
            foreach($dados['medidas'] as $medid){
                $mAcompanhamentoMedidas->medid = $medid;
                $mAcompanhamentoMedidas->salvar();
                $mAcompanhamentoMedidas->acmid = null;
            }
        }
    }

    private function _gravarHistorico($acoid)
    {
        $mHistorico = new Acompanhamento_Model_Historico();
        
        $mHistorico->acoid = $acoid;
        $mHistorico->usucpf = $_SESSION['usucpforigem'];
        $mHistorico->hisdata = date('Y-m-d H:i:s');
        
        $mHistorico->salvar();
    }

    public function recuperarPiPorMetaESubunidade($mppid, $suoid)
    {
        $mPi = new Pi_PlanoInterno();
        $listaPi = $mPi->recuperarPiPorMetaESubunidade($mppid, $suoid);

        include_once APPRAIZ . 'acompanhamento/modulos/principal/detalhe-pi.inc';
    }

    public function recuperarPiPorIndicadorESubunidade($ipnid, $suoid)
    {
        $mPi = new Pi_PlanoInterno();
        $aPis = $mPi->recuperarPiPorIndicadorESubunidade($ipnid, $suoid);

        include_once APPRAIZ . 'acompanhamento/modulos/principal/detalhe-pi.inc';
    }
    
    public function recuperarPiPorMetaPncESubunidade($mpnid, $suoid)
    {
        $mPi = new Pi_PlanoInterno();
        $aPis = $mPi->recuperarPiPorMetaPncESubunidade($mpnid, $suoid);

        include_once APPRAIZ . 'acompanhamento/modulos/principal/detalhe-pi.inc';
    }

    public function recuperarHistorico($acoid)
    {
        $aHistorico = (new Acompanhamento_Model_Historico())->recuperarHistorico($acoid);

        include_once APPRAIZ . 'acompanhamento/modulos/principal/historico.inc';
    }

    public function listarMetasPpa($suoid, $janid = null)
    {
        $listaPerfis = pegaPerfilGeral();
        $mAcompanhamento = new Acompanhamento_Model_Acompanhamento();

        $mJanela = new Acompanhamento_Model_Janela();
        $janelaAtiva = $mJanela->recuperarJanelaAtiva(Acompanhamento_Model_Tipo::K_TIPO_META_PPA);
        $aJanelas = $mJanela->recuperarTodos('*', ["prsano = '{$_SESSION['exercicio']}'", "janstatus = 'A'", "tipid = " . Acompanhamento_Model_Tipo::K_TIPO_META_PPA], 'janinicio desc');

        include_once APPRAIZ . 'acompanhamento/modulos/principal/listar-metas-ppa.inc';
    }

}