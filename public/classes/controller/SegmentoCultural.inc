<?php

class Public_Controller_SegmentoCultural
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/area_cultural_form&acao=A';
        try {
            $mdsegCultural = new Planacomorc_Model_SegmentoCultural($dados['secid']);
            $mdsegCultural->popularDadosObjeto($dados, true);
            $mdsegCultural->seccod = strtoupper($mdsegCultural->seccod);
            $mdsegCultural->secano = $mdsegCultural->secano ? $mdsegCultural->secano : $_SESSION['exercicio'];
            $mdsegCultural->salvar(null,null);
            $mdsegCultural->commit();
        } catch (Exception $e){
            $mdsegCultural->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function excluir($dados)
    {
        $url = '?modulo=apoio/area_cultural_form&acao=A';
        try {
            $mdsegCultural = new Planacomorc_Model_SegmentoCultural($dados['secid']);
            $mdsegCultural->secstatus = 'I';
            $mdsegCultural->salvar();
            $mdsegCultural->commit();
            simec_redirecionar($url . '&arceid=' . $mdsegCultural->arceid, 'success');
        } catch (Exception $e){
            $mdsegCultural->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function montarListagem($secid)
    {
        $mdsegCultural = new Planacomorc_Model_SegmentoCultural($secid);
        $perfis = pegaPerfilGeral();
        /**
         * @todo colcoar a constante correta do acompanhamento para atualizar a regra.
         */
        $podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;

        $lista = (new Planacomorc_Model_SegmentoCultural())->RecuperaSegmentoPorArea($mdsegCultural->secid);

        include_once APPRAIZ . 'planacomorc/modulos/apoio/segmento-cultural.inc';

    }
}
