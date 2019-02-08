<?php

class Public_Controller_Objetivoppa
{
    public function salvar($dados)
    {
        $url = "?modulo=apoio/objetivoppa_form&acao=A";
        try {
            $mObjetivoppa = new Public_Model_ObjetivoPpa($dados['oppid']);
            $mObjetivoppa->popularDadosObjeto($dados);
            $mObjetivoppa->prsano = $mObjetivoppa->prsano ? $mObjetivoppa->prsano : $_SESSION['exercicio'];
            $mObjetivoppa->salvar(null,null,['prsano', 'oppdsc',  'oppnome', 'oppcod', 'oppstatus']);
            $this->salvarMetasObjetivos($mObjetivoppa->oppid);
            $mObjetivoppa->commit();
            simec_redirecionar($url."&oppid=".$mObjetivoppa->oppid, 'success');
        } catch (Exception $e){
            $mObjetivoppa->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function salvarMetas($dados){

        $url = '?modulo=apoio/objetivoppa_form&acao=A&oppid='.$dados['oppid'];

        try {
            $mMetappa = new Public_Model_MetaPpa($dados['mppid']);
            $mdObjetivoMetappa = new Public_Model_ObjetivoMetaPpa();
            $mMetappa->popularDadosObjeto($dados);
            $mMetappa->mppcod = strtoupper($mMetappa->mppcod);
            $mMetappa->prsano = $mMetappa->prsano ? $mMetappa->prsano : $_SESSION['exercicio'];
            $mMetappa->mppquantificavel = $dados['mppquantificavel'] == 't'? 't': 'f';
            $mMetappa->mppanualizavel = $dados['mppanualizavel'] == 't'? 't': 'f';
            
            $mMetappa->salvar(null, null, ['mppdsc', 'mppcod', 'mppnome', 'mppstatus', 'prsano', 'mppquantificavel', 'mppquantidade', ]);
            $mdObjetivoMetappa->salvarMetas($mMetappa->mppid, $dados['oppid']);
            $mMetappa->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mMetappa->rollback();
            simec_redirecionar($url, 'error');
        }        
    }

    public function salvarMetasObjetivos($oppid){
        $mdObjetivoMetappa = new Public_Model_ObjetivoMetaPpa();
        $mppid = $_REQUEST['mppid'];
        $mdObjetivoMetappa->excluirPorObjetivo($oppid);
        foreach($mppid as $value){
            $mdObjetivoMetappa->salvarMetas($value, $oppid);
        }        
    }

    public function excluir($oppid)
    {
        $url = "?modulo=apoio/objetivoppa&acao=A";

        try {
            $mObjetivoppa = new Public_Model_ObjetivoPpa($oppid);
            $mObjetivoppa->oppstatus = 'I';
            $mObjetivoppa->salvar();
            $mObjetivoppa->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mObjetivoppa->rollback();
            simec_redirecionar($url, 'error');
        }
    }
}
