<?php

class Painel_Controller_AvaliacaoGestor
{
    public function salvar($dados)
    {
//        ver($dados,d);
        $url = '?modulo=principal/avaliacaoGestor&acao=A';
        try {
            $mAvaliacaoGestor = new Painel_Model_AvaliacaoGestor($dados['agid']);
            $mAvaliacaoGestor->popularDadosObjeto($dados);
            $mAvaliacaoGestor->nucpf = $_SESSION['usucpf'];
            $mAvaliacaoGestor->salvar(null, null, ['indavalgestor', 'indobsgestor', 'indobsgestorindicador']);
            $mAvaliacaoGestor->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAvaliacaoGestor->rollback();
            return false;
        }
    } //end salvar()

    public function excluir($emiid)
    {
        try {
            $mEstrategiaMeta = new Painel_Model_AvaliacaoGestor($emiid);

            $mEstrategiaMeta->excluir();
            $mEstrategiaMeta->commit();
            return true;
        } catch (Exception $e){
            $mEstrategiaMeta->rollback();
            return false;
        }
    } //end salvar()
}