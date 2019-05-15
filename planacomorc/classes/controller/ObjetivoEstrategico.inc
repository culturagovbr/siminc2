<?php

class Planacomorc_Controller_ObjetivoEstrategico
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/objetivo-estrategico&acao=A';
        try {
            $mObjetivo = new Planacomorc_Model_ObjetivoEstrategico($dados['obeid']);
            $mObjetivo->obecadastro = 'NOW()';
            $mObjetivo->popularDadosObjeto($dados);
            $mObjetivo->salvar();
            $mObjetivo->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $mObjetivo->rollback();
            simec_redirecionar($url, "error");
        }
    }

    public function excluir($obeid)
    {
        $url = '?modulo=apoio/objetivo-estrategico&acao=A';
        try {
            $mObjetivo = new Planacomorc_Model_ObjetivoEstrategico($obeid);
            $mObjetivo->obestatus = 'I';
            $mObjetivo->salvar();
            $mObjetivo->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $mObjetivo->rollback();
            simec_redirecionar($url, 'error');
        }
    }

}
