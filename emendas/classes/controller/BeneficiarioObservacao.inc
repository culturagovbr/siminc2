<?php

class Emendas_Controller_BeneficiarioObservacao
{
    public function salvar($dados)
    {
        try {
            $mBeneficiarioObservacao = new Emendas_Model_BeneficiarioObservacao($dados['bobid']);

            $mBeneficiarioObservacao->usucpf = $_SESSION['usucpf'];
            $mBeneficiarioObservacao->benid = $dados['benid'];
            $mBeneficiarioObservacao->bobdata = $dados['bobdata'] ? formata_data_sql($dados['bobdata']) : null;
            $mBeneficiarioObservacao->bobdsc = $dados['bobdsc']? utf8_decode($dados['bobdsc']): null;
            $mBeneficiarioObservacao->salvar(null, null, []);
            $mBeneficiarioObservacao->commit();
            return true;
        } catch (Exception $e){
            $mBeneficiarioObservacao->rollback();
            return false;
        }
    }
    
    public function excluir($bobid)
    {
        try {
            $mBeneficiarioObservacao = new Emendas_Model_BeneficiarioObservacao($bobid);

            $mBeneficiarioObservacao->bobstatus = 'I';
            $mBeneficiarioObservacao->usucpf = $_SESSION['usucpforigem'];
            $mBeneficiarioObservacao->salvar();
            $mBeneficiarioObservacao->commit();
            return $mBeneficiarioObservacao->benid;
        } catch (Exception $e){
            $mBeneficiarioObservacao->rollback();
            return false;
        }
    }
}
