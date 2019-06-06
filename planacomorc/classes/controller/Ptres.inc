<?php

class Planacomorc_Controller_Ptres
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/funcionais&acao=A';

        try {
            $mPtres = new Planacomorc_Model_Ptres($dados['ptrid']);
            $mPtres->popularDadosObjeto($dados);

            $acao = new Planacomorc_Model_Acao($mPtres->acaid);
            $mPtres->acacod = $acao->acacod;
            $mPtres->prgcod = $acao->prgcod;
            $mPtres->loccod = $acao->loccod;
            $mPtres->funcod = $acao->funcod;
            $mPtres->sfucod = $acao->sfucod;

            # Dados monetários
//            $mPtres->ptrdotacaocusteio = (int)str_replace(array('.', ','), array('', '.'), $dados['ptrdotacaocusteio']);
//            $mPtres->ptrdotacaocapital = (int)str_replace(array('.', ','), array('', '.'), $dados['ptrdotacaocapital']);
            $mPtres->ptrano = $_SESSION['exercicio'];
            $mPtres->salvar(null, null, ['ptrnomeprodutopo', 'ptrnomeunidadepo', 'ptrano', 'prgcod', 'acacod', 'loccod', 'unicod', 'irpcod', 'ptrdotacao', 'ptrstatus', 'ptrdata', 'plocod', 'esfcod', 'ptrdotacaocapital', 'ptrdotacaocusteio', 'plodsc']);
            $mPtres->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mPtres->rollback();
            simec_redirecionar($url, 'error');
        }
    }
    
    public function excluir($ptrid)
    {
        $url = '?modulo=apoio/funcionais&acao=A';

        try {
            $mPtres = new Planacomorc_Model_Ptres($ptrid);
            
            if($mPtres->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mPtres->ptrid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mPtres->ptrstatus = 'I';
            $mPtres->salvar();
            $mPtres->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mPtres->rollback();
            simec_redirecionar($url, 'error');
        }
    }
}
