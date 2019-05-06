<?php

class Planacomorc_Controller_AreaCultural
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/area_cultural_form&acao=A';
        try {
            $mdareaCultural = new Planacomorc_Model_AreaCultural($dados['mdeid']);
            $mdareaCultural->popularDadosObjeto($dados);
            $mdareaCultural->mdecod = strtoupper($mdareaCultural->mdecod);
            $mdareaCultural->mdeano = $mdareaCultural->mdeano ? $mdareaCultural->mdeano : $_SESSION['exercicio'];
            $mdareaCultural->salvar(null,null, ['mdedsc', 'mdestatus', 'mdeano', 'mdecod']);
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
            $mdareaCultural->mdestatus = 'I';
            $mdareaCultural->salvar();
            $mdareaCultural->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mdareaCultural->rollback();
            simec_redirecionar($url, 'error');
        }
    }
}
