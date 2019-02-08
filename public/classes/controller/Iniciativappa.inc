<?php

class Public_Controller_Iniciativappa
{
    public function salvar($dados)
    {
        try {
            $url = "?modulo=apoio/objetivoppa_form&acao=A&oppid=".$dados['oppid'];
            $mdObjetivoppa = new Public_Model_IniciativaPpa($dados['ippid']);
            $mdObjetivoppa->popularDadosObjeto($dados);
            $mdObjetivoppa->ippcod = strtoupper($mdObjetivoppa->ippcod);
            $mdObjetivoppa->prsano = $mdObjetivoppa->prsano ? $mdObjetivoppa->prsano : $_SESSION['exercicio'];
            $mdObjetivoppa->salvar(null,null);
            $mdObjetivoppa->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mdObjetivoppa->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function excluir($dados)
    {
        $url = "?modulo=apoio/objetivoppa_form&acao=A&oppid=".$dados['oppid'];

        try {
            $mdObjetivoppa = new Public_Model_IniciativaPpa($dados['ippid']);

            $mdObjetivoppa->ippstatus = 'I';
            $mdObjetivoppa->salvar();
            $mdObjetivoppa->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mdObjetivoppa->rollback();
            simec_redirecionar($url, 'error');
        }
    }

}
