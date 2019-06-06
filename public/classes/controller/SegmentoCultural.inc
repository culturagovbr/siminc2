<?php

class Public_Controller_SegmentoCultural
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/area_cultural_form&acao=A';
        try {
            $mSegmentoCultural = new Public_Model_SegmentoCultural($dados['secid']);
            $mSegmentoCultural->popularDadosObjeto($dados, true);
            $mSegmentoCultural->seccod = strtoupper($mSegmentoCultural->seccod);
            $mSegmentoCultural->secano = $mSegmentoCultural->secano ? $mSegmentoCultural->secano : $_SESSION['exercicio'];
            $mSegmentoCultural->salvar(null,null);
            $mSegmentoCultural->commit();
        } catch (Exception $e){
            $mSegmentoCultural->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function excluir($dados)
    {
        $url = '?modulo=apoio/area_cultural_form&acao=A';
        try {
            $mSegmentoCultural = new Public_Model_SegmentoCultural($dados['secid']);
            $mSegmentoCultural->secstatus = 'I';
            $mSegmentoCultural->salvar();
            $mSegmentoCultural->commit();
            simec_redirecionar($url . '&arceid=' . $mSegmentoCultural->arceid, 'success');
        } catch (Exception $e){
            $mSegmentoCultural->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function montarListagem($secid)
    {
        $mSegmentoCultural = new Public_Model_SegmentoCultural($secid);
        $perfis = pegaPerfilGeral();
        /**
         * @todo colcoar a constante correta do acompanhamento para atualizar a regra.
         */
        $podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;

        $lista = $mSegmentoCultural->recuperarTodos('*' , ["secstatus = 'A'", "secano = '{$_SESSION['exercicio']}'", "secid = '{$mSegmentoCultural->secid}'"], 'secdsc');
        include_once APPRAIZ . 'planacomorc/modulos/apoio/segmento-cultural.inc';
    }
}
