<?php

class Alteracao_Controller_Tipo
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/tipo&acao=A';
        try {
            $mAlteracao = new Alteracao_Model_Tipo($dados['tpaid']);
            $mAlteracao->popularDadosObjeto($dados);
            $mAlteracao->salvar(null, null, ['tpaobs']);
            $mAlteracao->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAlteracao->rollback();
            simec_redirecionar($url, 'error');
        }
    }
    
    public function excluir($tpaid)
    {
        $url = '?modulo=apoio/tipo&acao=A';

        try {
            $mAlteracao = new Alteracao_Model_Tipo($tpaid);
            $mAlteracao->tpastatus = 'I';
            $mAlteracao->salvar();
            $mAlteracao->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAlteracao->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    /**
     * Valida se o código do Tipo de alteração é existente
     *
     * @return string
     */
    public function validarTipo()
    {
        $resposta = array();
        $mAlteracao = new Alteracao_Model_Tipo();
        $mAlteracao->tpacod = $_REQUEST['tpacod'];
        $mAlteracao->tpaid = $_REQUEST['tpaid'];
        $verifica = $mAlteracao->carregarPorCodigo();

        if (!empty($verifica)) {
            $resposta['title'] = NULL;
            $resposta['text'] = 'O código do Tipo de Alteração ' . $mAlteracao->tpacod . ' já está sendo usado!';
            $resposta['type'] = 'error';
        } else {
            $resposta['type'] = 'success';
        }
        return simec_json_encode($resposta);
    }

}
