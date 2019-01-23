<?php

class Alteracao_Controller_PlanoInternoSelecionado
{
    public function salvar($dados)
    {
        try {
            $mPedido = new Alteracao_Model_PlanoInternoSelecionado();
            $mPedido->popularDadosObjeto($dados);
            $arrDadosPI = $mPedido->RetornaValoresPI($dados['pliid']);
            $mPedido->vlcusteio = $arrDadosPI['custeio'];
            $mPedido->vlcapital = $arrDadosPI['capital'];
            $mPedido->vlfisico = $arrDadosPI['fisico'];
            $mPedido->vldotacaocusteio = $mPedido->vlcusteio+$mPedido->vlsuplementarcusteio+$mPedido->vlsuplementarexcessocusteio+$mPedido->vlsuplementarsuperavitcusteio-$mPedido->vlcancelarcusteio;
            $mPedido->vldotacaocapital = $mPedido->vlcapital+$mPedido->vlsuplementarcapital+$mPedido->vlsuplementarexcessocapital+$mPedido->vlsuplementarsuperavitcapital-$mPedido->vlcancelarcapital;
            $mPedido->vldotacaofisico = $mPedido->vlfisico+$mPedido->vlsuplementarfisico+$mPedido->vlsuplementarexcessofisico+$mPedido->vlsuplementarsuperavitfisico-$mPedido->vlcancelarfisico;
            $mPedido->salvar(null, null, ['vlcusteio','vlcapital', 'vlfisico', 'vlsuplementarcusteio','vlsuplementarcapital','vlcancelarcusteio','vlcancelarcapital','vlsuplementarexcessocusteio','vlsuplementarexcessocapital','vlsuplementarsuperavitcusteio','vlsuplementarsuperavitcapital','vldotacaocusteio','vldotacaocapital','vldotacaofisico']);
            $mPedido->commit();
            echo json_encode(true);
        } catch (Exception $e){
            $mPedido->rollback();
            echo json_encode(false);
        }
    }
    
    public function excluir($dados)
    {
        try {
            $mPedido = new Alteracao_Model_PlanoInternoSelecionado($dados['pliselid']);
            $mPedido->pliselstatus = 'I';
            $mPedido->salvar();
            $mPedido->commit();
            echo json_encode(array("return" => true));
        } catch (Exception $e){
            $mPedido->rollback();
            echo json_encode(array("return" => false));
        }
    }

    public function salvarCampo($dados)
    {
        try {
            include_once APPRAIZ . "planacomorc/classes/Pi_Complemento.class.inc";

            $mPedido = new Alteracao_Model_PlanoInternoSelecionado($dados['pliselid']);
            $mPiComplemento = new Pi_Complemento($mPedido->pliid);

            $mPedido->popularDadosObjeto($dados);
            $mPedido->{$dados['campo']} = $dados['valor']? $dados['valor']: NULL;
            $mPedido->vldotacaocusteio = $mPedido->vlcusteio+$mPedido->vlsuplementarcusteio+$mPedido->vlsuplementarexcessocusteio+$mPedido->vlsuplementarsuperavitcusteio-$mPedido->vlcancelarcusteio;
            $mPedido->vldotacaocapital = $mPedido->vlcapital+$mPedido->vlsuplementarcapital+$mPedido->vlsuplementarexcessocapital+$mPedido->vlsuplementarsuperavitcapital-$mPedido->vlcancelarcapital;
            $mPedido->vldotacaofisico = $mPedido->vlfisico+$mPedido->vlsuplementarfisico+$mPedido->vlsuplementarexcessofisico+$mPedido->vlsuplementarsuperavitfisico-$mPedido->vlcancelarfisico;
            $mPedido->salvar(null, null, ['vlcusteio','vlcapital', 'vlfisico', 'vlsuplementarcusteio','vlsuplementarcapital','vlcancelarcusteio','vlcancelarcapital','vlsuplementarexcessocusteio','vlsuplementarexcessocapital','vlsuplementarsuperavitcusteio','vlsuplementarsuperavitcapital','vldotacaocusteio','vldotacaocapital','vldotacaofisico']);
            $mPedido->commit();
            echo json_encode(true);
        } catch (Exception $e){
            $mPedido->rollback();
            echo json_encode(false);
        }
    }

}
