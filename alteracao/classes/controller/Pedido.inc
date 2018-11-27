<?php

class Alteracao_Controller_Pedido
{
    public function salvar($dados)
    {
        try {
            $mPedido = new Alteracao_Model_Pedido($dados['pedid']);
            $mPedido->popularDadosObjeto($dados);
            $mPedido->pedano = $_SESSION['exercicio'];
            $mPedido->pedstatus = 'A';
            $mPedido->pedtitulo = utf8_decode($dados['pedtitulo']);
            $mPedido->salvar(null, null, ['docid']);
            $mPedido->commit();
            echo json_encode(["result" => true, "pedid" => $mPedido->pedid]);
        } catch (Exception $e){
            $mPedido->rollback();
            echo json_encode(["result" => false]);
        }
    }
    
    public function excluir($pedid)
    {
        $url = 'alteracao.php?modulo=inicio&acao=C';

        try {
            $mPedido = new Alteracao_Model_Pedido($pedid);
            $mPedido->pedstatus = 'I';
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
