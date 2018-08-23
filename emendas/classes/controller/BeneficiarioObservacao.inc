<?php

class Emendas_Controller_BeneficiarioObservacao
{
    public function salvar($dados)
    {
        try {
            $mBeneficiarioHistorico = new Emendas_Model_BeneficiarioObservacao($dados['bobid']);

            $mBeneficiarioHistorico->usucpf = $_SESSION['usucpf'];
            $mBeneficiarioHistorico->benid = $dados['benid'];
            $mBeneficiarioHistorico->bobdata = $dados['bobdata'] ? formata_data_sql($dados['bobdata']) : null;
            $mBeneficiarioHistorico->bobdsc = $dados['bobdsc']? utf8_decode($dados['bobdsc']): null;

            $mBeneficiarioHistorico->salvar(null, null, []);
            $mBeneficiarioHistorico->commit();
            return true;
        } catch (Exception $e){
            $mBeneficiarioHistorico->rollback();
            return false;
        }
    } //end salvar()
    
    public function excluir($bobid)
    {
        try {
            $mBeneficiarioHistorico = new Emendas_Model_BeneficiarioObservacao($bobid);

            $mBeneficiarioHistorico->bobstatus = 'I';
            $mBeneficiarioHistorico->usucpf = $_SESSION['usucpforigem'];

            $mBeneficiarioHistorico->salvar();
            $mBeneficiarioHistorico->commit();
            return $mBeneficiarioHistorico->benid;
        } catch (Exception $e){
            $mBeneficiarioHistorico->rollback();
            return false;
        }
    } //end salvar()
}
