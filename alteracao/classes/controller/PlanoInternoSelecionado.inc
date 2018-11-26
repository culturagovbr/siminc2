<?php

class Alteracao_Controller_PlanoInternoSelecionado
{
    public function salvar($dados)
    {
        try {
            $mPedido = new Alteracao_Model_PlanoInternoSelecionado();
            $mPedido->popularDadosObjeto($dados);
            $mPedido->salvar(null, null, ['vlsaldoempenhocusteio','vlsaldoempenhocapital','vlsuplementarcusteio','vlsuplementarcapital','vlcancelarcusteio','vlcancelarcapital','vlsuplementarexcessocusteio','vlsuplementarexcessocapital','vlsuplementarsuperavitcusteio','vlsuplementarsuperavitcapital','vldotacaocusteio','vldotacaocapital']);
            $mPedido->commit();
            echo json_encode(true);
        } catch (Exception $e){
            $mPedido->rollback();
            echo json_encode(false);
        }
    }
    
    public function excluir($pedid)
    {
        $url = 'alteracao.php?modulo=inicio&acao=C';

        try {
            $mPedido = new Alteracao_Model_PlanoInternoSelecionado($pedid);
            $mPedido->excluir();
            $mPedido->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mPedido->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function salvarCampo($dados)
    {
        try {
            $mPedido = new Alteracao_Model_PlanoInternoSelecionado($dados['pliselid']);
            $mPedido->popularDadosObjeto($dados);
            $mPedido->{$dados['campo']} = $dados['valor'];
            $mPedido->salvar(null, null, ['vlsaldoempenhocusteio','vlsaldoempenhocapital','vlsuplementarcusteio','vlsuplementarcapital','vlcancelarcusteio','vlcancelarcapital','vlsuplementarexcessocusteio','vlsuplementarexcessocapital','vlsuplementarsuperavitcusteio','vlsuplementarsuperavitcapital','vldotacaocusteio','vldotacaocapital', 'plisuplementarquantidade', 'plicancelarquantidade', 'plisuplementarsuperavitquantidade', 'plisuplementarexcessoquantidade']);
            $mPedido->commit();
            echo json_encode(true);
        } catch (Exception $e){
            $mPedido->rollback();
            echo json_encode(false);
        }
    }

}
