<?php

class Alteracao_Controller_Justificativa
{
    public function salvar($dados)
    {
        try {
            $mJustificativa = new Alteracao_Model_Justificativa($dados['jstid']);
            $mJustificativa->popularDadosObjeto($dados);
            $mJustificativa->jstnecessidade = utf8_decode($dados['jstnecessidade']);
            $mJustificativa->jstcausa = utf8_decode($dados['jstcausa']);
            $mJustificativa->jstfinanciamento = utf8_decode($dados['jstfinanciamento']);
            $mJustificativa->jstfontes = utf8_decode($dados['jstfontes']);
            $mJustificativa->jstmedida = utf8_decode($dados['jstmedida']);
            $mJustificativa->jstlegislacao = utf8_decode($dados['jstlegislacao']);
            $mJustificativa->jstoutros = utf8_decode($dados['jstoutros']);
            $mJustificativa->salvar(null, null, ['jstfinanciamento', 'jstfontes', 'jstmedida', 'jstlegislacao', 'jstoutros']);
            $mJustificativa->commit();
            echo json_encode(true);
        } catch (Exception $e){
            $mJustificativa->rollback();
            echo json_encode(false);
        }
    }

}
