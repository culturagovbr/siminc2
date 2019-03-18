<?php

class Public_Controller_Metappa
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/metappa&acao=A';

        try {
            $mMetappa = new Public_Model_MetaPpa($dados['mppid']);
            $mMetappa->popularDadosObjeto($dados);
            $mMetappa->prsano = $mMetappa->prsano ? $mMetappa->prsano : $_SESSION['exercicio'];
            $mMetappa->mppquantificavel = $dados['mppquantificavel'] == 't'? 't': 'f';

            $mMetappa->salvar(null, null, ['mppdsc', 'mppcod', 'mppnome', 'mppstatus', 'prsano', 'mppquantificavel', 'mppquantidade', ]);
            $mMetappa->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mMetappa->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function excluir($mppid)
    {
        $url = '?modulo=apoio/metappa&acao=A';

        try {
            $mMetappa = new Public_Model_MetaPpa($mppid);
            
            if($mMetappa->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mMetappa->mppid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mMetappa->mppstatus = 'I';
            $mMetappa->salvar();
            $mMetappa->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mMetappa->rollback();
            simec_redirecionar($url, 'error');
        }
    }
}
