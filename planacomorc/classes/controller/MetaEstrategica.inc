<?php

class Planacomorc_Controller_MetaEstrategica
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/meta-estrategica&acao=A';
        try {
            $mMetaEstrategica = new Planacomorc_Model_MetaEstrategica($dados['meeid']);
            $mMetaEstrategica->meecadastro = 'NOW()';
            $mMetaEstrategica->popularDadosObjeto($dados);
            $mMetaEstrategica->salvar();
            $mMetaEstrategica->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mMetaEstrategica->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function excluir($meeid)
    {
        $url = '?modulo=apoio/meta-estrategica&acao=A';
        try {
            $mMetaEstrategica = new Planacomorc_Model_MetaEstrategica($meeid);
            $mMetaEstrategica->meestatus = 'I';
            $mMetaEstrategica->salvar();
            $mMetaEstrategica->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mMetaEstrategica->rollback();
            simec_redirecionar($url, 'error');
        }
    }

}
