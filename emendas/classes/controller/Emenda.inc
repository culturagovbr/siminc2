<?php

class Emendas_Controller_Emenda
{
    public function salvar($dados)
    {
        $url = '?modulo=principal/emenda_form&acao=A';

        try {
            $mEmenda = new Emendas_Model_Emenda($dados['emeid']);
            $mEmenda->popularDadosObjeto($dados);
            $mEmenda->prsano = $mEmenda->prsano ? $mEmenda->prsano : $_SESSION['exercicio'];
                
            $mEmenda->salvar(null, null, ['emejustificativa', 'emeimpositiva', ]);
            $mEmenda->commit();
            simec_redirecionar($url . '&emeid=' . $mEmenda->emeid, 'success');
        } catch (Exception $e){
            $mEmenda->rollback();
            simec_redirecionar($url, 'error');
        }
    }
    
    public function excluir($emeid)
    {
        $url = '?modulo=inicio&acao=C';

        try {
            $mEmenda = new Emendas_Model_Emenda($emeid);
            
            if($mEmenda->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mEmenda->emeid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mEmenda->emestatus = 'I';
            $mEmenda->salvar();
            $mEmenda->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mEmenda->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function montarFormularioDetalhes($emeid = null, $emdid = null)
    {
        global $simec;
        
        $perfis = pegaPerfilGeral();
        $podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;
        $simec->setPodeEditar($podeEditar);
        
        $listaEmendadetalhe = (new Emendas_Model_EmendaDetalhe)->recuperarTodos('*', ['emeid = ' . (int)$emeid]);
        
        if(!$podeEditar && $listaEmendadetalhe){
            $emdid = $emdid? $emdid: $listaEmendadetalhe[0]['emdid'];
        }
        
        $mEmendadetalhe = new Emendas_Model_EmendaDetalhe($emdid);

        if(!$mEmendadetalhe->emeid){ $mEmendadetalhe->emeid = $emeid; }
        include_once APPRAIZ . 'emendas/modulos/principal/emendadetalhe_form.inc';
    }

    public function montarListagemDetalhes($emeid)
    {
        $perfis = pegaPerfilGeral();
        $podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;
        $mEmendaDetalhe = new Emendas_Model_EmendaDetalhe();
        $mEmendaDetalhe->emeid = $emeid;
        $aEmendadetalhe = $mEmendaDetalhe->recuperarTodosPorEmeid();
        include_once APPRAIZ . 'emendas/modulos/principal/emendadetalhe.inc';
    }

}
