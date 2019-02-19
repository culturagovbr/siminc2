<?php

class Emendas_Controller_Vinculacao
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/vinculacao&acao=A';

        try {
            $mVinculacao = new Emendas_Model_Vinculacao($dados['vinid']);
            $mVinculacao->popularDadosObjeto($dados);
            
            $mVinculacao->salvar(null, null, []);
            $mVinculacao->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mVinculacao->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($vinid)
    {
        $url = '?modulo=apoio/vinculacao&acao=A';

        try {
            $mVinculacao = new Emendas_Model_Vinculacao($vinid);
            $mVinculacao->vinstatus = 'I';
            $mVinculacao->salvar();
            $mVinculacao->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mVinculacao->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            