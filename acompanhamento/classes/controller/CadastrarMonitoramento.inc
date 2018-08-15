<?php

class Acompanhamento_Controller_CadastrarMonitoramento
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/cadastrarMonitoramento&acao=A';

        try {
            $mMonitoramento = new Acompanhamento_Model_Tipo($dados['tipid']);
            $mMonitoramento->popularDadosObjeto($dados);
            $mMonitoramento->salvar(null,null);
            $mMonitoramento->commit();
            simec_redirecionar($url . '&tipid=' . $mMonitoramento->tipid, 'success');
        } catch (Exception $e){
            $mMonitoramento->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function excluir($tipid)
    {
        $url = '?modulo=apoio/cadastrarMonitoramento&acao=A';

        try {
            $mMonitoramento = new Acompanhamento_Model_Tipo($tipid);
//            $mdMonitoramento->idostatus = 'I';
            $mMonitoramento->salvar();
            $mMonitoramento->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mMonitoramento->rollback();
            simec_redirecionar($url, 'error');
        }
    }

}
