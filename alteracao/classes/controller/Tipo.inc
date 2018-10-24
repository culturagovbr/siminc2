<?php

class Alteracao_Controller_Tipo
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/tipo&acao=A';
        try {
            $mAlteracao = new Alteracao_Model_Tipo($dados['tpaid']);
            $mAlteracao->popularDadosObjeto($dados);
            $mAlteracao->salvar(null, null);
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

}
