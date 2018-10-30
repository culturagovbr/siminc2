<?php

class Alteracao_Controller_Pedido
{
    public function salvar($dados)
    {
        $url = '?modulo=principal/cadastro_alteracoes&acao=C';
        try {
            ver($dados);
            $mPedido = new Alteracao_Model_Pedido($dados['pedid']);
            $mPedido->popularDadosObjeto($dados);
            $mPedido->pedano = $_SESSION['exercicio'];
            $mPedido->pedstatus = 'A';
            $mPedido->salvar(null, null, ['docid']);
            $mPedido->commit();
            simec_redirecionar($url.'&pedid='.$mPedido->pedid, 'success');
        } catch (Exception $e){
            $mPedido->rollback();
            simec_redirecionar($url, 'error');
        }
    }
    
    public function excluir($tpaid)
    {
        $url = '?modulo=apoio/tipo&acao=A';

        try {
            $mPedido = new Alteracao_Model_Pedido($tpaid);
            $mPedido->tpastatus = 'I';
            $mPedido->salvar();
            $mPedido->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mPedido->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    /**
     * Valida se o código do Pedido de alteração é existente
     *
     * @return string
     */
    public function validarPedido()
    {
        $resposta = array();
        $mPedido = new Alteracao_Model_Pedido();
        $mPedido->tpacod = $_REQUEST['tpacod'];
        $mPedido->tpaid = $_REQUEST['tpaid'];
        $verifica = $mPedido->carregarPorCodigo();

        if (!empty($verifica)) {
            $resposta['title'] = NULL;
            $resposta['text'] = 'O código do Pedido de Alteração ' . $mPedido->tpacod . ' já está sendo usado!';
            $resposta['type'] = 'error';
        } else {
            $resposta['type'] = 'success';
        }
        return simec_json_encode($resposta);
    }

}
