<?php

class Public_Controller_SegmentoCultural
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/area_cultural_form&acao=A';

        try {
            $mdsegCultural = new Monitora_Model_PiNivelEtapaEnsino($dados['secid']);
            $mdsegCultural->popularDadosObjeto($dados, true);
            $mdsegCultural->neecod = strtoupper($mdsegCultural->neecod);
            $mdsegCultural->neeano = $mdsegCultural->neeano ? $mdsegCultural->neeano : $_SESSION['exercicio'];
//            ver($mdsegCultural,d);

            $mdsegCultural->salvar(null,null);
            $mdsegCultural->commit();
        } catch (Exception $e){
            $mdsegCultural->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function excluir($dados)
    {
        $url = '?modulo=apoio/area_cultural_form&acao=A';

        try {
            $mdsegCultural = new Monitora_Model_PiNivelEtapaEnsino($dados['secid']);

            $mdsegCultural->neestatus = 'I';
            $mdsegCultural->salvar();
            $mdsegCultural->commit();
            simec_redirecionar($url . '&mdeid=' . $mdsegCultural->mdeid, 'success');
        } catch (Exception $e){
            $mdsegCultural->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function montarListagem($secid)
    {
        $mdsegCultural = new Monitora_Model_PiNivelEtapaEnsino($secid);
        $perfis = pegaPerfilGeral();
        /**
         * @todo colcoar a constante correta do acompanhamento para atualizar a regra.
         */
        $podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;

        $lista = (new Monitora_Model_PiNivelEtapaEnsino())->RecuperaSegmentoPorArea($mdsegCultural->secid);

        include_once APPRAIZ . 'planacomorc/modulos/apoio/segmento-cultural.inc';

    }
}
