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
            $mAvaliacaoGestor->indobsgestor = utf8_decode($dados['indobsgestor']);
            $mAvaliacaoGestor->indobsgestorindicador = utf8_decode($dados['indobsgestorindicador']);
            $mAvaliacaoGestor->salvar(null, null, ['indavalgestor', 'indobsgestor', 'indobsgestorindicador']);
            $mAvaliacaoGestor->commit();
            return true;
        } catch (Exception $e){
            $mAvaliacaoGestor->rollback();
            return false;
        }
    } //end salvar()

    public function excluir($agid)
    {
        try {
            $mEstrategiaMeta = new Painel_Model_AvaliacaoGestor($agid);
            $mEstrategiaMeta->excluir();
            $mEstrategiaMeta->commit();
            return true;
        } catch (Exception $e){
            $mEstrategiaMeta->rollback();
            return false;
        }
    } //end salvar()
}