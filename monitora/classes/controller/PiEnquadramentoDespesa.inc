<?php

class Monitora_Controller_PiEnquadramentoDespesa
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/enquadramento_despesa&acao=A';

        try {
            $mEnquadramento = new Monitora_Model_PiEnquadramentoDespesa($dados['eqdid']);
            $mEnquadramento->popularDadosObjeto($dados);
            $mEnquadramento->eqdano = $mEnquadramento->eqdano ? $mEnquadramento->eqdano : $_SESSION['exercicio'];

            $mEnquadramento->salvar(null, null, ['eqdcod', 'eqddsc', 'eqdano', 'eqdstatus', 'mpneid', 'eqdstreduzido', 'tpdid' ]);

            $mEnquadramentoRp = new Monitora_Model_EnquadramentoRp();
            $mEnquadramentoRp->excluirPorExercicio($_SESSION['exercicio'], $mEnquadramento->eqdid);

            if($dados['lista_rp']){
              foreach($dados['lista_rp'] as $irpcod){
                  $mEnquadramentoRp->eqdid = $mEnquadramento->eqdid;
                  $mEnquadramentoRp->irpcod = $irpcod;
                  $mEnquadramentoRp->salvar();
                  $mEnquadramentoRp->erpid = null;
              }
            }

            $mEnquadramento->commit();
            simec_redirecionar($url . '&eqdid=' . $mEnquadramento->eqdid, 'success');
        } catch (Exception $e){
            $mEnquadramento->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function excluir($eqdid)
    {
        $url = '?modulo=apoio/enquadramento_despesa&acao=A';

        try {
            $mEnquadramento = new Monitora_Model_PiEnquadramentoDespesa($eqdid);

            if($mEnquadramento->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/enquadramento_despesa-form&acao=A&eqdid=' . $mEnquadramento->eqdid;
                simec_redirecionar($url, 'error', $mensagem);
            }

            $mEnquadramento->eqdstatus = 'I';
            $mEnquadramento->salvar();
            $mEnquadramento->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mEnquadramento->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}
