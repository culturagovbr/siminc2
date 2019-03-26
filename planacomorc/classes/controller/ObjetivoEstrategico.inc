<?php

class Planacomorc_Controller_ObjetivoEstrategico
{
    public function salvar($dados)
    {

        try {
            $mDiretriz = new Planacomorc_Model_ObjetivoEstrategico($dados['obeid']);
            $mDiretriz->popularDadosObjeto($dados);
            $mDiretriz->salvar(null, null, []);
            $mDiretriz->commit();
            echo json_encode(true);
        } catch (Exception $e) {
            $mDiretriz->rollback();
            echo json_encode(false);
        }
    }

    public function excluir($obeid)
    {
        try {
            $mDiretriz = new Planacomorc_Model_ObjetivoEstrategico($obeid);
            $mDiretriz->obestatus = 'I';
            $mDiretriz->salvar(null, null, []);
            $mDiretriz->commit();
            echo json_encode(true);
        } catch (Exception $e) {
            $mDiretriz->rollback();
            echo json_encode(false);
        }
    }

}
