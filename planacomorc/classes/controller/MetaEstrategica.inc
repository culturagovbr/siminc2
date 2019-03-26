<?php

class Planacomorc_Controller_MetaEstrategica
{
    public function salvar($dados)
    {

        try {
            $mMetaEstrategica = new Planacomorc_Model_MetaEstrategica($dados['meeid']);
            $mMetaEstrategica->popularDadosObjeto($dados);
            $mMetaEstrategica->salvar(null, null, []);
            $mMetaEstrategica->commit();
            echo json_encode(true);
        } catch (Exception $e) {
            $mMetaEstrategica->rollback();
            echo json_encode(false);
        }
    }

    public function excluir($meeid)
    {
        try {
            $mMetaEstrategica = new Planacomorc_Model_MetaEstrategica($meeid);
            $mMetaEstrategica->meestatus = 'I';
            $mMetaEstrategica->salvar(null, null, []);
            $mMetaEstrategica->commit();
            echo json_encode(true);
        } catch (Exception $e) {
            $mMetaEstrategica->rollback();
            echo json_encode(false);
        }
    }

}
