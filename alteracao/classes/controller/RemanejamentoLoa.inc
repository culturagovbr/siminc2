<?php

class Alteracao_Controller_RemanejamentoLoa
{
    public function salvar($dados)
    {
        try {
            $mLoa = new Alteracao_Model_RemanejamentoLoa();
            $mLoa->popularDadosObjeto($dados);
            $mLoa->salvar(null, null, null);
            $mLoa->commit();
            echo json_encode(true);
        } catch (Exception $e){
            $mLoa->rollback();
            echo json_encode(false);
        }
    }
    
    public function excluir($rlid)
    {
        try {
            $mLoa = new Alteracao_Model_RemanejamentoLoa($rlid);
            $mLoa->rlstatus = 'I';
            $mLoa->salvar();
            $mLoa->commit();
            echo json_encode(true);
        } catch (Exception $e){
            $mLoa->rollback();
            echo json_encode(false);
        }
    }

}
