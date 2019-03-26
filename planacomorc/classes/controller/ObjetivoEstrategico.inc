<?php

class Planacomorc_Controller_ObjetivoEstrategico
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/objetivo_meta_diretriz_form&acao=A';
        try {
            $mDiretriz = new Planacomorc_Model_ObjetivoEstrategico($dados['obeid']);
            $mDiretriz->prsano = $_SESSION['exercicio'];
            $mDiretriz->obecadastro = 'NOW()';
            $mDiretriz->popularDadosObjeto($dados);
            $mDiretriz->salvar(null, null);
            $mDiretriz->commit();
            simec_redirecionar($url."&obeid=" . $mDiretriz->obeid, 'success');
        } catch (Exception $e) {
            $mDiretriz->rollback();
            simec_redirecionar($url, "error");
        }
    }

    public function excluir($obeid)
    {
        $url = "?modulo=apoio/objetivo_meta_diretriz&acao=A";
        try {
            $mDiretriz = new Planacomorc_Model_ObjetivoEstrategico($obeid);
            $mDiretriz->obestatus = 'I';
            $mDiretriz->salvar(null, null, []);
            $mDiretriz->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $mDiretriz->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function montarFormularioMetaEstrategica($obeid = null, $meeid = null)
    {
        global $simec;
        $perfis = pegaPerfilGeral();

        $podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;
        $simec->setPodeEditar($podeEditar);

        $aMeta = (new Planacomorc_Model_MetaEstrategica())->recuperarTodos('*', ['obeid = ' . (int)$obeid]);

        if(!$podeEditar && $aMeta){
            $obeid = $obeid? $obeid: $aMeta[0]['obeid'];
        }

        $mMetaEstrategica = new Planacomorc_Model_MetaEstrategica($meeid);

        include_once APPRAIZ. 'planacomorc/modulos/apoio/meta_estrategica_form.inc';
    }

    public function montarListagemMetaEstrategica($obeid)
    {
        $perfis = pegaPerfilGeral();
        $podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;
        $listaMetasEstrategicas = (new Planacomorc_Model_MetaEstrategica())->recuperarTodos('*', ['obeid = ' . (int)$obeid, "meestatus = 'A'"], 'meecod');

        include_once APPRAIZ . 'planacomorc/modulos/apoio/lista_meta_estrategica.inc';
    }

    public function montarFormularioDiretrizEstrategica($obeid = null, $dieid = null)
    {
        global $simec;
        $perfis = pegaPerfilGeral();

        $podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;
        $simec->setPodeEditar($podeEditar);

        $listaDiretrizEstrategica = (new Planacomorc_Model_DiretrizEstrategica())->recuperarTodos('*', ['obeid = ' . (int)$obeid]);

        if(!$podeEditar && $listaDiretrizEstrategica){
            $obeid = $obeid? $obeid: $listaDiretrizEstrategica[0]['obeid'];
        }
        $mDiretrizEstrategica = new Planacomorc_Model_DiretrizEstrategica($dieid);

        include_once APPRAIZ. 'planacomorc/modulos/apoio/diretriz_estrategica_form.inc';
    }

    public function montarListagemDiretrizEstrategica($obeid)
    {
        $perfis = pegaPerfilGeral();
        $podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;
        $listaDiretrizEstrategica = (new Planacomorc_Model_DiretrizEstrategica())->recuperarTodos('*', ['obeid = ' . (int)$obeid, "diestatus = 'A'"], 'diecod');

        include_once APPRAIZ . 'planacomorc/modulos/apoio/lista_diretriz_estrategica.inc';
    }

}
