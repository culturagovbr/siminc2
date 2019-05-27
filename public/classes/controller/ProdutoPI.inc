<?php

class Public_Controller_ProdutoPI
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/produtos_pi&acao=A';

        try {
            $mProdutosPI = new Monitora_Model_PiProduto($dados['pprid']);
            $mProdutosPI->popularDadosObjeto($dados);
            $mProdutosPI->prsano = $mProdutosPI->prsano ? $mProdutosPI->prsano : $_SESSION['exercicio'];
            $mProdutosPI->salvar(null,null, ['pprdescricao', 'tipo']);
            $mProdutosPI->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mProdutosPI->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function excluir($pprid)
    {
        $url = '?modulo=apoio/produtos_pi&acao=A';

        try {
            $mProdutosPI = new Monitora_Model_PiProduto($pprid);
            $mProdutosPI->pprstatus = 'I';
            $mProdutosPI->salvar();
            $mProdutosPI->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mProdutosPI->rollback();
            simec_redirecionar($url, 'error');
        }
    }
}
