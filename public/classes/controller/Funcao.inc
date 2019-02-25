<?php

class Public_Controller_Funcao
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/funcao_form&acao=A';

        try {
            $mFuncao = new Public_Model_Funcao($dados['funcod']);
            $mFuncao->popularDadosObjeto($dados);
            $mFuncao->funcod = $dados->funcod;
            $mFuncao->fundsc = $dados->fundsc;
            $mFuncao->salvar(null,null, ['funcod']);
            $mFuncao->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mFuncao->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function excluir($funcod)
    {
        $url = '?modulo=apoio/funcao_form&acao=A';

        try {
            $mFuncao = new Public_Model_Funcao($funcod);

            $mFuncao->funstatus = 'I';
            $mFuncao->salvar();
            $mFuncao->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mFuncao->rollback();
            simec_redirecionar($url, 'error');
        }
    }
}
