<?php

class Acompanhamento_Controller_Tipo
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/tipo&acao=A';

        try {
            $mMonitoramento = new Acompanhamento_Model_Tipo($dados['tipid']);
            $mMonitoramento->popularDadosObjeto($dados);
            $mMonitoramento->usucpf = $_SESSION['usucpf'];
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
        $url = '?modulo=apoio/tipo&acao=A';
        try {
            $mMonitoramento = new Acompanhamento_Model_Tipo($tipid);
            $mMonitoramento->tipstatus = 'I';
            $mMonitoramento->salvar();
            $mMonitoramento->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mMonitoramento->rollback();
            simec_redirecionar($url, 'error');
        }
    }

}
