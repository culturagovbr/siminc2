<?php

class Planacomorc_Controller_DimensaoEstrategica
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/dimensao-estrategica&acao=A';
        try {
            $mDimensao = new Planacomorc_Model_DimensaoEstrategica($dados['dimeid']);
            $mDimensao->dimecadastro = 'NOW()';
            $mDimensao->popularDadosObjeto($dados);
            $mDimensao->salvar();
            $mDimensao->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mDimensao->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function excluir($dimeid)
    {
        $url = '?modulo=apoio/dimensao-estrategica&acao=A';
        try {
            $mDimensao = new Planacomorc_Model_DimensaoEstrategica($dimeid);
            $mDimensao->dimestatus = 'I';
            $mDimensao->salvar();
            $mDimensao->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mDimensao->rollback();
            simec_redirecionar($url, 'error');
        }
    }

}