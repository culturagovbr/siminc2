<?php

class Painel_Controller_Estrategia_Meta_Indicador
{
    public function salvar($dados)
    {
//        ver($dados,d);
        $url = '?modulo=principal/cadastroEstrategiaMetas&acao=A';
        try {
            $mEstrategiaMeta = new Painel_Model_Estrategia_Meta_Indicador($dados['emiid']);
            $mEstrategiaMeta->popularDadosObjeto($dados);

            $mEstrategiaMeta->salvar(null, null, null);
            $mEstrategiaMeta->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mEstrategiaMeta->rollback();
            return false;
        }
    } //end salvar()

    public function excluir($emiid)
    {
        try {
            $mEstrategiaMeta = new Painel_Model_Estrategia_Meta_Indicador($emiid);

            $mEstrategiaMeta->excluir();
            $mEstrategiaMeta->commit();
            return true;
        } catch (Exception $e){
            $mEstrategiaMeta->rollback();
            return false;
        }
    } //end salvar()
}