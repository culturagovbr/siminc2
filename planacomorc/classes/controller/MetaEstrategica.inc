<?php

class Planacomorc_Controller_MetaEstrategica
{
    public function salvar($dados)
    {
        try {
            $mMetaEstrategica = new Planacomorc_Model_MetaEstrategica($dados['meeid']);
            $mMetaEstrategica->prsano = $_SESSION['exercicio'];
            $mMetaEstrategica->obeid = $dados['obeid'];
            $mMetaEstrategica->meecadastro = 'NOW()';
            $mMetaEstrategica->popularDadosObjeto($dados);
            $mMetaEstrategica->salvar(null, null);
            $mMetaEstrategica->commit();
            echo json_encode(array("return" => true));
        } catch (Exception $e) {
            $mMetaEstrategica->rollback();
            echo json_encode(array("return" => false));
        }
    }

    public function excluir($meeid)
    {
        try {
            $mMetaEstrategica = new Planacomorc_Model_MetaEstrategica($meeid);
            $mMetaEstrategica->meestatus = 'I';
            $mMetaEstrategica->salvar(null, null, []);
            $mMetaEstrategica->commit();
            echo json_encode(array("return" => true));
        } catch (Exception $e) {
            $mMetaEstrategica->rollback();
            echo json_encode(array("return" => false));
        }
    }

}
