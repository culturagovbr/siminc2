<?php

class Planacomorc_Controller_MetaUnidade
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/meta-unidade&acao=A';
        try {
            $mDiretriz = new Planacomorc_Model_MetaUnidade($dados['dieid']);
            $mDiretriz->prsano = $_SESSION['exercicio'];
            $mDiretriz->diecadastro = 'NOW()';
            $mDiretriz->popularDadosObjeto($dados);
            $mDiretriz->salvar(null, null);
            $mDiretriz->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $mDiretriz->rollback();
            simec_redirecionar($url, "error");
        }
    }

    public function excluir($dieid)
    {
        $url = '?modulo=apoio/meta-unidade&acao=A';
        try {
            $mDiretriz = new Planacomorc_Model_MetaUnidade($dieid);
            $mDiretriz->diestatus = 'I';
            $mDiretriz->salvar(null, null);
            $mDiretriz->commit();
            $mDiretriz->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $mDiretriz->rollback();
            simec_redirecionar($url, "error");
        }
    }

}