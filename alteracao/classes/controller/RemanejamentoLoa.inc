<?php

class Alteracao_Controller_RemanejamentoLoa
{
    public function salvar($dados)
    {
        try {
            $mPedido = new Alteracao_Model_RemanejamentoLoa();
            $mPedido->popularDadosObjeto($dados);
            $mPedido->salvar(null, null, ['vldotacao','vlsuplementar','vlcancelado','vlsuplementarexcesso','vlsuplementarsuperavit']);
            $mPedido->commit();
            echo json_encode(true);
        } catch (Exception $e){
            $mPedido->rollback();
            echo json_encode(false);
        }
    }
    
    public function excluir($rlid)
    {
        try {
            $mPedido = new Alteracao_Model_RemanejamentoLoa();
            $mPedido->excluir($rlid);
            $mPedido->commit();
            echo json_encode(true);
        } catch (Exception $e){
            $mPedido->rollback();
            echo json_encode(false);
        }
    }

}
