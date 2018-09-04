<?php

class Emendas_Controller_BeneficiarioHistorico
{
    public function salvar($dados)
    {
        try {
            $mBeneficiarioHistorico = new Emendas_Model_BeneficiarioHistorico($dados['behid']);
            $mBeneficiarioHistorico->usucpf = $mBeneficiarioHistorico->usucpf ? $dados['usucpf'] : $_SESSION['usucpf'];
            $mBeneficiarioHistorico->behdata = 'NOW()';
            $mBeneficiarioHistorico->salvar();
            $mBeneficiarioHistorico->commit();
            return true;
        } catch (Exception $e){
            $mBeneficiarioHistorico->rollback();
            return false;
        }
    }

}
