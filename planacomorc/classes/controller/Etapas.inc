<?php

class Planacomorc_Controller_Etapas
{
    public function salvar($dados)
    {

        try {
            $mEtapas = new Planacomorc_Model_Etapas($dados['etaid']);
            $mEtapas->popularDadosObjeto($dados);
            $mEtapas->salvar(null, null, []);
            $mEtapas->commit();
            echo json_encode(true);
        } catch (Exception $e){
            $mEtapas->rollback();
            echo json_encode(false);
        }
    }

}