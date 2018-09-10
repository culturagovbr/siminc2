<?php

class Acompanhamento_Controller_Janela
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/janela&acao=A';

        try {
            $mJanela = new Acompanhamento_Model_Janela($dados['janid']);
            $mJanela->popularDadosObjeto($dados);
            $mJanela->janinicio = $mJanela->janinicio? formata_data_sql($mJanela->janinicio): NULL;
            $mJanela->janfim = $mJanela->janfim? formata_data_sql($mJanela->janfim): NULL;
            $mJanela->prsano = $mJanela->prsano ? $mJanela->prsano : $_SESSION['exercicio'];

            $mJanela->salvar(null, null, ['janfim', ]);
            $mJanela->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mJanela->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($janid)
    {
        $url = '?modulo=apoio/janela&acao=A';

        try {
            $mJanela = new Acompanhamento_Model_Janela($janid);
            
            if($mJanela->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mJanela->janid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mJanela->janstatus = 'I';
            $mJanela->salvar();
            $mJanela->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mJanela->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            