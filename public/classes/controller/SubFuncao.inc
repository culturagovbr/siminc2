<?php

class Public_Controller_SubFuncao
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/subfuncao_form&acao=A';

        try {
            $mSubFuncao = new Public_Model_SubFuncao($dados['sfucod']);
            $mSubFuncao->popularDadosObjeto($dados);
            $mSubFuncao->sfucod = $dados->sfucod;
            $mSubFuncao->funcod = $dados->funcod;
            $mSubFuncao->fundsc = $dados->fundsc;
            $mSubFuncao->salvar(null,null, ['funcod', 'fundsc']);
            $mSubFuncao->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mSubFuncao->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function excluir($sfucod)
    {
        $url = '?modulo=apoio/subfuncao_form&acao=A';

        try {
            $mSubFuncao = new Public_Model_SubFuncao($sfucod);

            $mSubFuncao->funstatus = 'I';
            $mSubFuncao->salvar();
            $mSubFuncao->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mSubFuncao->rollback();
            simec_redirecionar($url, 'error');
        }
    }
}
