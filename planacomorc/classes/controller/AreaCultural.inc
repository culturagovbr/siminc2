<?php

class Planacomorc_Controller_AreaCultural
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/area_cultural_form&acao=A';
        try {
            $mdareaCultural = new Planacomorc_Model_AreaCultural($dados['mdeid']);
            $mdareaCultural->popularDadosObjeto($dados);
            $mdareaCultural->arccod = strtoupper($mdareaCultural->arccod);
            $mdareaCultural->arcano = $mdareaCultural->arcano ? $mdareaCultural->arcano : $_SESSION['exercicio'];
            $mdareaCultural->salvar(null,null, ['arcdsc', 'arcstatus', 'arcano', 'arccod']);
            $mdareaCultural->commit();
            simec_redirecionar($url . '&mdeid=' . $mdareaCultural->mdeid, 'success');
        } catch (Exception $e){
            $mdareaCultural->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function excluir($mdeid)
    {
        $url = '?modulo=apoio/area_cultural&acao=A';
        try {
            $mdareaCultural = new Planacomorc_Model_AreaCultural($mdeid);
            $mdareaCultural->arcstatus = 'I';
            $mdareaCultural->salvar();
            $mdareaCultural->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mdareaCultural->rollback();
            simec_redirecionar($url, 'error');
        }
    }
}
