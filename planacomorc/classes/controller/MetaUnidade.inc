<?php

class Planacomorc_Controller_MetaUnidade
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/meta-unidade&acao=A';
        try {
            $mMetaUnidade = new Planacomorc_Model_MetaUnidade($dados['meuid']);

            if ($dados['meuquantidade']) $dados['meuquantidade'] = str_replace(".", "", $dados['meuquantidade']);

            $mMetaUnidade->meuquantidade = $dados['meuquantidade'];
            $mMetaUnidade->meuquantificavel = $dados['meuquantificavel'] == 't' ? 't' : 'f';
            $mMetaUnidade->meuprioritario = $dados['meuprioritario'] == 't' ? 't' : 'f';
            $mMetaUnidade->meucadastro = 'NOW()';
            $mMetaUnidade->popularDadosObjeto($dados);
            $mMetaUnidade->salvar(null, null, ['meeid', 'dimeid', 'obeid', 'meuquantidade']);
            $mMetaUnidade->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $mMetaUnidade->rollback();
            simec_redirecionar($url, "error");
        }
    }

    public function excluir($meuid)
    {
        $url = '?modulo=apoio/meta-unidade&acao=A';
        try {
            $mMetaUnidade = new Planacomorc_Model_MetaUnidade($meuid);
            $mMetaUnidade->meustatus = 'I';
            $mMetaUnidade->salvar(null, null);
            $mMetaUnidade->commit();
            $mMetaUnidade->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $mMetaUnidade->rollback();
            simec_redirecionar($url, "error");
        }
    }

}