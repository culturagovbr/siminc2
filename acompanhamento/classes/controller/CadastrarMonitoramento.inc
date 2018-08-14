<?php

class Acompanhamento_Controller_CadastrarMonitoramento
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/cadastrarMonitoramento_form&acao=A';

        try {
            $mdMonitoramento = new Acompanhamento_Model_Tipo($dados['tipid']);
            $mdMonitoramento->popularDadosObjeto($dados);
            $mdMonitoramento->salvar(null,null);
            $mdMonitoramento->commit();
            simec_redirecionar($url . '&tipid=' . $mdMonitoramento->tipid, 'success');
        } catch (Exception $e){
            $mdMonitoramento->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function excluir($tipid)
    {
        $url = '?modulo=apoio/cadastrarMonitoramento&acao=A';

        try {
            $mdMonitoramento = new Acompanhamento_Model_Tipo($tipid);
//            $mdMonitoramento->idostatus = 'I';
            $mdMonitoramento->salvar();
            $mdMonitoramento->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mdMonitoramento->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    /**
     * Valida se o código do Idoc pode ser alterado ou inserido.
     *
     * @return string
     */
    public function validarmonitora()
    {
        $resposta = array();
        $mdMonitoramento = new Acompanhamento_Model_Tipo();
        $mdMonitoramento->tipdsc = $_REQUEST['tipdsc'];
        $mdMonitoramento->tipid = $_REQUEST['tipid'];
        $verifica = $mdMonitoramento->carregarPorCodigo();
        if (!empty($verifica)) {
            $resposta['title'] = NULL;
            $resposta['text'] = 'A  Descrição do Monitoramento ' . $mdMonitoramento->tipdsc . ' já está sendo usado!';
            $resposta['type'] = 'error';
        } else {
            $resposta['type'] = 'success';
        }
        return simec_json_encode($resposta);
    }
}
