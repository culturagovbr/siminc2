<?php

class Planacomorc_Controller_EnquadramentoRp
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/acao-enquadramento&acao=A';
        try {
            $mEnquadramentoRp = new Planacomorc_Model_Enquadramentorp();
            $mEnquadramentoRp->excluirPorExercicio($_SESSION['exercicio']);

            if(isset($dados['vinculos']) && is_array($dados['vinculos'])){
                foreach($dados['vinculos'] as $eqdid => $vinculos){
                    foreach($vinculos as $irpcod){
                        $mEnquadramentoRp->eqdid = $eqdid;
                        $mEnquadramentoRp->irpcod = $irpcod;
                        $mEnquadramentoRp->salvar();
                        $mEnquadramentoRp->erpid = null;
                    }
                }
            }

            $mEnquadramentoRp->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mEnquadramentoRp->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function salvar2($dados)
    {
        $url = 'planacomorc.php?modulo=apoio/vincular-meta-ppa&acao=A';
        try {
            $oModel = new Spo_Model_SubUnidadeMetaPpa();
            $oModel->excluirPorExercicio($_SESSION['exercicio']);

            if(isset($dados['vinculos']) && is_array($dados['vinculos'])){
                foreach($dados['vinculos'] as $mppid => $vinculos){
                    foreach($vinculos as $suoid){
                        $oModel->mppid = $mppid;
                        $oModel->suoid = $suoid;
                        $oModel->salvar();
                        $oModel->smpid = null;
                    }
                }
            }
            $oModel->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $oModel->rollback();
            simec_redirecionar($url, 'error');
        }
    }
}
