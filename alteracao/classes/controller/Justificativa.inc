<?php

class Alteracao_Controller_Justificativa
{
    public function salvar($dados)
    {
        try {
            $mJustificativa = new Alteracao_Model_Justificativa($dados['jstid']);
            $mJustificativa->popularDadosObjeto($dados);
            $mJustificativa->salvar(null, null, ['jstfinanciamento', 'jstfontes', 'jstmedida', 'jstlegislacao', 'jstoutros']);
            $mJustificativa->commit();
            echo json_encode(true);
        } catch (Exception $e){
            $mJustificativa->rollback();
            echo json_encode(false);
        }
    }

}
