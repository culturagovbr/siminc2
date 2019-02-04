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
            $mPedido->usucpf = $_SESSION['usucpf'];
            $mPedido->salvar(null, null, ['docid']);
            $this->removerUnidades($mPedido->pedid);
            $this->salvarUnidades($mPedido->pedid, $dados['suoid']);
            $tpdid = $this->retornaTpdid($dados['tpaid']);
            if ($mPedido->docid){
                $mPedido->alterarTpdidDocumento($tpdid, $mPedido->docid);
            }
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
    
    public function retornaTpdid($tpaid){
        $mTipo = new Alteracao_Model_Tipo($tpaid);
        switch ($mTipo->tpafluxo) {
            case 'I':
                return WF_TPDID_INTERNO;
            case 'E':
                return WF_TPDID_EXTERNO;
        }
    }
    
    public function removerUnidades($pedid){
        $mPedidoUnidade = new Alteracao_Model_PedidoUnidade();
        $mPedidoUnidade->removerUnidades($pedid);
        $mPedidoUnidade->commit();
    }
    
    public function salvarUnidades($pedid, $suoid){
        foreach($suoid as $value){
            $mPedidoUnidade = new Alteracao_Model_PedidoUnidade();
            $mPedidoUnidade->suoid = $value;
            $mPedidoUnidade->pedid = $pedid;
            $mPedidoUnidade->salvar();
            $mPedidoUnidade->commit();
        }
    }
    public function alterarEstadoAlteracao($pedid)
    {
        $mPedido = new Alteracao_Model_Pedido($pedid);
        $estadoAtual = wf_pegarEstadoAtual($mPedido->docid);
        if($this->retornaTpdid($mPedido->tpaid) == WF_TPDID_INTERNO)
        {
            if ($estadoAtual['esdid']==ESD_EM_CADASTRAMENTO_INTERNO){
                $aedid = AED_ALTERACAO_INTERNO_CADASTRAMENTO_EM_ANALISE;
            }else{
                $aedid = AED_ALTERACAO_INTERNO_CORRECAO_EM_ANALISE;
            }

        }
        else if ($this->retornaTpdid($mPedido->tpaid) == WF_TPDID_EXTERNO)
        {
            if ($estadoAtual['esdid']==ESD_EM_CADASTRAMENTO_EXTERNO){
                $aedid = AED_ALTERACAO_EXTERNO_CADASTRAMENTO_EM_ANALISE;
            }else{
                $aedid = AED_ALTERACAO_EXTERNO_CORRECAO_EM_ANALISE;
            }
        }
        $return = wf_alterarEstado($mPedido->docid, $aedid, '', array());
        echo json_encode($return);
    }
}
