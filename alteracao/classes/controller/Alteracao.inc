<?php

class Proposta_Controller_Proposta
{
    public function salvar($dados)
    {
        $url = '?modulo=principal/proposta_form&acao=A';
        try {
            $mProposta = new Proposta_Model_Proposta($dados['proid']);
            $mProposta->popularDadosObjeto($dados);
            $mProposta->prsano = $mProposta->prsano ? $mProposta->prsano : $_SESSION['exercicio'];
            $mProposta->proquantidade = $mProposta->proquantidade ? str_replace(array('.', ','), array('', '.'), $mProposta->proquantidade) : 0;
            $mProposta->proquantidadeexpansao = $mProposta->proquantidadeexpansao ? str_replace(array('.', ','), array('', '.'), $mProposta->proquantidadeexpansao) : 0;

            $mProposta->salvar(null, null, ['proquantidade', 'proquantidadeexpansao', 'projustificativa', 'projustificativaexpansao', ]);

            $mPtres = (new Monitora_Model_Ptres($dados['ptrid']))->salvarQuantidadeLocalizador($dados['locquantidadeproposta']);

            $mProposta->commit();
            simec_redirecionar($url . '&proid='. $mProposta->proid, 'success');
        } catch (Exception $e){
            $mProposta->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($proid)
    {
        $url = '?modulo=principal/proposta&acao=A';

        try {
            $mProposta = new Proposta_Model_Proposta($proid);
            
            if($mProposta->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=principal/modalidade-pactuacao-form&acao=A&capid=' . $mProposta->proid;
                simec_redirecionar($url, 'error', $mensagem);
            }
            
            $mProposta->prostatus = 'I';
            $mProposta->salvar();
            $mProposta->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mProposta->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()


    public function montarFormularioDetalhes($proid = null, $prdid = null)
    {
        global $simec;
        include_once APPRAIZ . 'proposta/modulos/principal/propostadetalhe_form.inc';
    }

    public function montarListagemDetalhes($proid)
    {
        include_once APPRAIZ . 'proposta/modulos/principal/propostadetalhe.inc';
    }

}
