<?php

class Emendas_Controller_PedidoFinanceiro
{
    public function salvar($dados)
    {
        try {
            
            $url = '?modulo=principal/beneficiario_form&acao=A';
            
            $mPedidoFinanceiro = new Emendas_Model_PedidoFinanceiro();
            $mPedidoFinanceiro->pefid = $mPedidoFinanceiro->verificaPedidoFinanceiroAnoMes($dados['benid'], date('Y'), (int)date('m'));
            $mPedidoFinanceiro->popularDadosObjeto($dados);
            $mPedidoFinanceiro->usucpf = $_SESSION['usucpf'];
            $mPedidoFinanceiro->nroanoreferencia = date('Y');
            $mPedidoFinanceiro->nromesreferencia = (int)date('m');
            
            $mPedidoFinanceiro->salvar(null, null, []);
            $mPedidoFinanceiro->commit();
            simec_redirecionar($url.'&benid='.$dados['benid'], 'success');
        } catch (Exception $e){
            $mPedidoFinanceiro->rollback();
            simec_redirecionar($url.'&benid='.$dados['benid'], 'error');
        }
    } //end salvar()
    
}            