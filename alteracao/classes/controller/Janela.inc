<?php

class Alteracao_Controller_Janela
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/janela&acao=A';
        try {
            $mJanela = new Alteracao_Model_Janela($dados['janid']);
            $mJanela->popularDadosObjeto($dados);
            $mJanela->janinicio = $mJanela->janinicio ? formata_data_sql($mJanela->janinicio) : NULL;
            $mJanela->janfim = $mJanela->janfim ? formata_data_sql($mJanela->janfim) : NULL;
            $mJanela->janinicioprogramacao = $mJanela->janinicioprogramacao ? formata_data_sql($mJanela->janinicioprogramacao) : NULL;
            $mJanela->janfimprogramacao = $mJanela->janfimprogramacao ? formata_data_sql($mJanela->janfimprogramacao) : NULL;
            $mJanela->prsano = $mJanela->prsano ? $mJanela->prsano : $_SESSION['exercicio'];
            $mJanela->salvar(null, null, ['janinicioprogramacao', 'janfimprogramacao']);
            $mJanela->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mJanela->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function excluir($janid)
    {
        $url = '?modulo=apoio/janela&acao=A';
        try {
            $mJanela = new Alteracao_Model_Janela($janid);
            $mJanela->janstatus = 'I';
            $mJanela->salvar();
            $mJanela->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mJanela->rollback();
            simec_redirecionar($url, 'error');
        }
    }
}
