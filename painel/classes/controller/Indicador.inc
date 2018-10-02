<?php

class Painel_Controller_Indicador
{
    public function salvar($dados)
    {
        try {
            $mIndicador = new Painel_Model_Indicador($dados['indid']);
            $mIndicador->popularDadosObjeto($dados);
            $mIndicador->salvar(null, null, null);
            $mIndicador->commit();
            return true;
        } catch (Exception $e){
            $mIndicador->rollback();
            return false;
        }
    } //end salvar()
    
}